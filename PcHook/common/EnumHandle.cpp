#include "stdafx.h"
#include <windows.h>
#include <winternl.h>
#include "EnumHandle.h"

#pragma warning(disable: 4996)

#define STATUS_SUCCESS 0x00UL
#define STATUS_INFO_LENGTH_MISMATCH 0xC0000004

#define SystemHandleInformation 16
#define SE_DEBUG_PRIVILEGE 0x14

typedef enum _OBJECT_INFORMATION_CLASSEX {
	ObjBasicInformation = 0,
	ObjNameInformation,
	ObjTypeInformation,
} OBJECT_INFORMATION_CLASSEX;

typedef enum _PROCESSINFOCLASSEX
{
	ProcessHandleInformation = 20,
}PROCESSINFOCLASSEX;

typedef struct _SYSTEM_HANDLE
{
	ULONG ProcessId;
	BYTE ObjectTypeNumber;
	BYTE Flags;
	USHORT Handle;
	PVOID Object;
	ACCESS_MASK GrantAccess;
}SYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	DWORD HandleCount;
	SYSTEM_HANDLE Handles[1];
}SYSTEM_HANDLE_INFORMATION;

typedef struct _OBJECT_NAME_INFORMATION
{
	UNICODE_STRING ObjectName;
}OBJECT_NAME_INFORMATION;

typedef NTSTATUS(WINAPI *ZwQueryInformationProcessProc)(HANDLE, PROCESSINFOCLASSEX, LPVOID, DWORD, PDWORD);
ZwQueryInformationProcessProc ZwQueryInformationProcess;

typedef NTSTATUS(WINAPI *ZwQuerySystemInformationProc)(DWORD, PVOID, DWORD, DWORD*);
ZwQuerySystemInformationProc ZwQuerySystemInformation;

typedef NTSTATUS(WINAPI *ZwQueryObjectProc)(HANDLE, OBJECT_INFORMATION_CLASSEX, PVOID, ULONG, PULONG);
ZwQueryObjectProc ZwQueryObject;

typedef NTSTATUS(WINAPI *RtlAdjustPrivilegeProc)(DWORD, BOOL, BOOL, PDWORD);
RtlAdjustPrivilegeProc RtlAdjustPrivilege;

typedef DWORD(WINAPI *ZwSuspendProcessProc)(HANDLE);
ZwSuspendProcessProc ZwSuspendProcess;

typedef DWORD(WINAPI *ZwResumeProcessProc)(HANDLE);
ZwResumeProcessProc ZwResumeProcess;

BOOL ElevatePrivileges()
{
	static bool s_bInit = FALSE;
	if (!s_bInit)
	{
		s_bInit = TRUE;
		
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		tkp.PrivilegeCount = 1;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return FALSE;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL GetUnDocumentAPI()
{
	if ((ZwQuerySystemInformation == NULL) || \
		(ZwQueryObject == NULL) || \
		(ZwQueryInformationProcess == NULL))
	{
		ZwQuerySystemInformation = (ZwQuerySystemInformationProc)
			GetProcAddress(GetModuleHandle(L"ntdll.dll"), "ZwQuerySystemInformation");

		ZwQueryObject = (ZwQueryObjectProc)
			GetProcAddress(GetModuleHandle(L"ntdll.dll"), "ZwQueryObject");

		ZwQueryInformationProcess = (ZwQueryInformationProcessProc)
			GetProcAddress(GetModuleHandle(L"ntdll.dll"), "ZwQueryInformationProcess");
	}
	
	if ((ZwQuerySystemInformation == NULL) || \
		(ZwQueryObject == NULL) || \
		(ZwQueryInformationProcess == NULL))
		return FALSE;

	return TRUE;
}


HANDLE FindHandle(ULONG pid, LPCTSTR szObjectType, LPCTSTR szObjectName, DWORD dwOptions /*= DUPLICATE_CLOSE_SOURCE*/)
{
	if (!GetUnDocumentAPI() || !ElevatePrivileges())
		return NULL;
	
	HANDLE duplicateHnd = NULL;
	HANDLE sourceHnd = NULL;
	DWORD procHndNum = 0;
	DWORD dwLenObjectNameInfo = 0x1000;

	/*SYSTEM_HANDLE* currnetHnd;
	DWORD buffLen = 0x1000;

	SYSTEM_HANDLE_INFORMATION* buff = (SYSTEM_HANDLE_INFORMATION*)malloc(buffLen);
	NTSTATUS status = ZwQuerySystemInformation(SystemHandleInformation, buff, buffLen, &buffLen);
	if (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		free(buff);
		buff = (SYSTEM_HANDLE_INFORMATION*)malloc(buffLen);
		if (ZwQuerySystemInformation(SystemHandleInformation, buff, buffLen, &buffLen))
		{
			free(buff);
			return NULL;
		}
	}*/

	OBJECT_NAME_INFORMATION* objNameInfo = (OBJECT_NAME_INFORMATION*)malloc(dwLenObjectNameInfo);
	OBJECT_NAME_INFORMATION* objTypeInfo = (OBJECT_NAME_INFORMATION*)malloc(dwLenObjectNameInfo);

	//for (DWORD idx = 0; idx<buff->HandleCount; idx++)
	{
		//currnetHnd = &(buff->Handles[idx]);

		//if (currnetHnd->ProcessId == pid)
		{
			sourceHnd = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_DUP_HANDLE , FALSE, pid);
			ZwQueryInformationProcess(sourceHnd, ProcessHandleInformation, &procHndNum, sizeof(DWORD), NULL);

			unsigned short hndNum = 4;
			for (DWORD i = 0; i<procHndNum; hndNum += 4, i++)
			{
				if (!DuplicateHandle(sourceHnd,
					(HANDLE)hndNum,
					GetCurrentProcess(),
					&duplicateHnd, 0, FALSE, DUPLICATE_SAME_ACCESS))
				{
					continue;
				}
				else
				{
					memset(objNameInfo, 0, dwLenObjectNameInfo);
					memset(objTypeInfo, 0, dwLenObjectNameInfo);

					ZwQueryObject((HANDLE)duplicateHnd, ObjNameInformation, objNameInfo, dwLenObjectNameInfo, NULL);
					ZwQueryObject((HANDLE)duplicateHnd, ObjTypeInformation, objTypeInfo, dwLenObjectNameInfo, NULL);

					if (!objTypeInfo->ObjectName.Buffer || !objNameInfo->ObjectName.Buffer)
						continue;

					if (wcsicmp(objTypeInfo->ObjectName.Buffer, szObjectType) == 0)
					{
						if (wcsstr(objNameInfo->ObjectName.Buffer, szObjectName) != 0)
						{
							CloseHandle(duplicateHnd);
							
							if (DuplicateHandle(sourceHnd,
								(HANDLE)hndNum,
								GetCurrentProcess(),
								&duplicateHnd, 0, FALSE, DUPLICATE_CLOSE_SOURCE))
							{
								//free(buff);
								free(objTypeInfo);
								free(objNameInfo);
								return duplicateHnd;
							}
						}
					}

					CloseHandle(duplicateHnd);
				}
			}
		}
	}

	//free(buff);
	free(objTypeInfo);
	free(objNameInfo);
	return NULL;
}
#include "stdafx.h"
#include <shlobj.h>
#include "sync_msg.h"
#include "send_msg.h"


INIT_SINGLETON(CSyncMsg);

static void __stdcall SyncMsgCb(unsigned char *pMsg)
{
	try
	{
		//版本兼容
		if (pBaseAddrMgr->VersionCompare(pBaseAddrMgr->GetVersion(), VERSION_2_6_5_38) < 0)
		{
			UINT64 nMsgSrvId = *(UINT64 *)(pMsg + 0x28);
			int nType = *(int *)(pMsg + 0x30);
			int nTime = *(int *)(pMsg + 0x3c);
			CMMString *mmstrToWxid = (CMMString *)(pMsg + 0x1B8);
			CMMString *mmstrFromWxid = (CMMString *)(pMsg + 0x1CC);
			CMMString *mmstrTalkerInChatroom = (CMMString *)(pMsg + 0xF4);
			CMMString *mmstrContent = (CMMString *)(pMsg + 0x54);
			CMMString *mmstrMsgSource = (CMMString *)(pMsg + 0x148);

			//Utf8编码
			CStringA strFromWxid = CW2UTF8(mmstrFromWxid->m_szBuf);
			CStringA strToWxid = CW2UTF8(mmstrToWxid->m_szBuf);
			CStringA strTalkerInChatroom = CW2UTF8(mmstrTalkerInChatroom->m_szBuf);
			CStringA strContent = CW2A(mmstrContent->m_szBuf);
			CStringA strMsgSource = CW2A(mmstrMsgSource->m_szBuf);

			LOG("From:%s\nTo:%s\nTalker:%s\nTime:%d\nType:%d\nMsgSrvId：%lld\nContent:%s\nMsgSource:%s\n ", strFromWxid, strToWxid, strTalkerInChatroom, nTime, nType, nMsgSrvId, strContent, strMsgSource);
		}
		else
		{
			UINT64 nMsgSrvId = *(UINT64 *)(pMsg + 0x28);
			int nType = *(int *)(pMsg + 0x30);
			int nTime = *(int *)(pMsg + 0x3c);
			CMMString *mmstrToWxid = (CMMString *)(pMsg + 0x1E0);
			CMMString *mmstrFromWxid = (CMMString *)(pMsg + 0x1F4);
			CMMString *mmstrTalkerInChatroom = (CMMString *)(pMsg + 0x114);
			CMMString *mmstrContent = (CMMString *)(pMsg + 0x68);
			CMMString *mmstrMsgSource = (CMMString *)(pMsg + 0x168);

			//Utf8编码
			CStringA strFromWxid = CW2UTF8(mmstrFromWxid->m_szBuf);
			CStringA strToWxid = CW2UTF8(mmstrToWxid->m_szBuf);
			CStringA strTalkerInChatroom = CW2UTF8(mmstrTalkerInChatroom->m_szBuf);
			CStringA strContent = CW2A(mmstrContent->m_szBuf);
			CStringA strMsgSource = CW2A(mmstrMsgSource->m_szBuf);

			LOG("From:%s\nTo:%s\nTalker:%s\nTime:%d\nType:%d\nMsgSrvId：%lld\nContent:%s\nMsgSource:%s\n ", strFromWxid, strToWxid, strTalkerInChatroom, nTime, nType, nMsgSrvId, strContent, strMsgSource);
		}		
	}
	catch (...)
	{
		LOG("Crash!");
	}
	
}

//Hook SyncMsg 返回地址
static DWORD s_dwSyncMsgRet = NULL;

void __declspec(naked) __stdcall Hook_SyncMsg()
{
	_asm
	{
		pushad
		pushfd
			lea ecx, [ebp - 0x500]			//不同版本临时变量位置可能不同
			push ecx						//存放msg结构体指针
			call SyncMsgCb
		popfd
		popad
		
		cmp eax, 0x270E

		push s_dwSyncMsgRet
		ret
	}
}


static void __stdcall CreateCdnDownloadTaskCb(char **szFileId , char **szFilePath)
{
	try
	{
		if (szFileId && szFilePath && *szFileId && *szFilePath)
		{	
			LOG("FielId:%s\nFilePath:%s", *szFileId, *szFilePath);
		}
		
	}
	catch (...)
	{
		LOG("Crash!");
	}
}

//Hook create_cdn_download_task 返回地址
static DWORD s_dwCreateCdnDownloadTaskRet = NULL;

void __declspec(naked) __stdcall Hook_CreateCdnDownloadTask()
{
	_asm
	{
		pushad								//esp - 0x20
		pushfd								//esp - 4
			push [esp + 0x10 + 0x20 + 4]	//local file path
			push [esp + 0x20 + 4]			//fileid
			call CreateCdnDownloadTaskCb
		popfd
		popad

		lea eax, [ebp - 0x8C]

		push s_dwCreateCdnDownloadTaskRet
		ret
	}
}

static void __stdcall CdnDownloadTaskOnSucceedCb(char **szTaskInfo)
{
	try
	{
		if (szTaskInfo && *szTaskInfo)
		{
			LOG("CdnTaskInfo:%s", *szTaskInfo);
		}
	}
	catch (...)
	{
		LOG("Crash!");
	}
}

//Hook cdn_download_task_on_succeed 返回地址
static DWORD s_dwCdnDownloadTaskOnSucceedRet = NULL;
void __declspec(naked) __stdcall Hook_CdnDownloadTaskOnSucceed()
{
	_asm
	{
		lea ecx, [ebp - 0x1A8]

		
		pushad								
		pushfd								
			push ecx							//taskinfo
			call CdnDownloadTaskOnSucceedCb
		popfd
		popad

		push s_dwCdnDownloadTaskOnSucceedRet
		ret
	}
}

BOOL CSyncMsg::Start()
{
	//newsync Hook
	auto spBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_HOOK_SYNC_MSG);
	if (spBase && spBase->GetAddr())
	{
		//兼容 lea ecx, [ebp - 0x500]
		DWORD dwOriginCode = (DWORD)Hook_SyncMsg;
		if (*(byte *)dwOriginCode == 0xe9)
			dwOriginCode = GET_CALL_ADDR(dwOriginCode);
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(dwOriginCode + 2), (LPCVOID)(spBase->GetAddr() - 0x17), 6, NULL);
	}
	
	CHOOK hook(BASEADDR_TYPE_HOOK_SYNC_MSG, 5, (DWORD)Hook_SyncMsg, s_dwSyncMsgRet);
	if (!hook.HookOn())
	{
		LOG("同步消息开启失败!");
		return FALSE;
	}

	//hook create_cdn_download_task
	CHOOK hookCreateCdn(BASEADDR_TYPE_HOOK_CREATE_CDN_DOWNLOAD_TASK, 6, (DWORD)Hook_CreateCdnDownloadTask, s_dwCreateCdnDownloadTaskRet);
	if (!hookCreateCdn.HookOn())
	{
		LOG("Create CDN HOOK开启失败!");
		return FALSE;
	}

	//hook cdn_download_task_on_succeed
	CHOOK hookCdnOnSucceed(BASEADDR_TYPE_HOOK_CDN_TASK_ONSUCCEED, 6, (DWORD)Hook_CdnDownloadTaskOnSucceed, s_dwCdnDownloadTaskOnSucceedRet);
	if (!hookCdnOnSucceed.HookOn())
	{
		LOG("CDN On Succeed HOOK开启失败!");
		return FALSE;
	}

	return TRUE;
}

BOOL CSyncMsg::MMXor(CString strEncryptFileName, CString strSaveAsFileName)
{
	try
	{
		string strBuf;
		FILE *fp = _wfopen(strEncryptFileName, L"rb");
		if (fp)
		{
			//读取加密数据
			fseek(fp, 0, SEEK_END);
			int nSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			strBuf.resize(nSize);
			fread(&strBuf[0], 1, nSize, fp);
			fclose(fp);

			//解密jpg数据
			auto spBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_MMXOR_CALL);
			if (spBase && spBase->GetAddr())
			{
				DWORD dwMMXorFun = spBase->GetAddr();
				char *szBuf = (char *)strBuf.c_str();
				__asm
				{
					pushad
						mov ecx, szBuf
						mov edx, nSize
						mov eax, dwMMXorFun			//登陆后调用
						call eax					//jpg文件解密
					popad
				}

				//建目录
				CString strDirPath = strSaveAsFileName;
				PathRemoveFileSpec(strDirPath.GetBuffer(MAX_PATH));
				strDirPath.ReleaseBuffer();
				SHCreateDirectoryEx(NULL, strDirPath, NULL);

				//保存解密后文件
				fp = _wfopen(strSaveAsFileName, L"wb");
				if (fp)
				{
					fwrite(&strBuf[0], 1, nSize, fp);
					fclose(fp);

					CStringA strEncryptFileNameA = CW2A(strEncryptFileName);
					CStringA strSaveAsFileNameA = CW2A(strSaveAsFileName);
					LOG("文件 %s 另存为 %s", strEncryptFileNameA, strSaveAsFileNameA);
					
					return TRUE;
				}
			}			
		}
	}
	catch (...)
	{
		LOG("Crash!");
	}
	
	return FALSE;
}

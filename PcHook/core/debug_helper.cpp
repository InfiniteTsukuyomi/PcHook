#include "stdafx.h"
#include <Shlobj.h>
#include <TlHelp32.h>
#include "debug_helper.h"



INIT_SINGLETON(CDebugHelper);

static void __stdcall PrintXlog(const char *szLog, DWORD dwLen)
{
	pDebugHelper->PrintXlog(szLog, dwLen);
}

//Hook xlog 返回地址
static DWORD s_dwXlogRet = NULL;
void __declspec(naked) __stdcall hook_xlog()
{
	__asm
	{
		pushad
		pushfd
		push DWORD PTR[ebp - 0x4020]		//xlog buffer
		push DWORD PTR[ebp - 0x4028]		//xlog len
		call PrintXlog
		popfd
		popad

		push DWORD PTR[ebp - 0x4028]

		push s_dwXlogRet
		ret
	}
}


void CDebugHelper::OpenXlog()
{
	//开启所有级别日志
	auto spBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_XLOGGER_LEVEL);
	if (spBase && spBase->GetAddr())
	{
		DWORD dwNewLevel = 0;
		void *pLoggerLevel = (void *)GET_DWORD(spBase->GetAddr());
		WriteProcessMemory(GetCurrentProcess(), pLoggerLevel, &dwNewLevel, 4, NULL);
	}

	//打印日志
	CHOOK hook(BASEADDR_TYPE_OPEN_XLOG, 6, (DWORD)hook_xlog, s_dwXlogRet);

	wchar_t szPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szPath);
	wcscat(szPath, XLOG_FILE_DIR);
	SHCreateDirectoryEx(NULL, szPath, NULL);

	SYSTEMTIME st;
	GetLocalTime(&st);

	wchar_t szFileName[MAX_PATH] = { 0 };
	wsprintf(szFileName, XLOG_FILE_NAME, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	wcscat(szPath, szFileName);

	m_fpXlog = _wfopen(szPath, L"wb");

	if (m_fpXlog && hook.HookOn())
	{
		thread t(WriteXLogThread);
		t.detach();

		LOG("已开启xlog!");

#if OPEN_CONSOLE_XLOG
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		setlocale(LC_ALL, "");
#endif
	}
	else
	{
		LOG("xlog开启失败!");
	}
}

void CDebugHelper::CloseAutoUpdate()
{
	WCHAR szConfigPath[MAX_PATH] = { 0 };
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
	if (pidl   &&   SHGetPathFromIDList(pidl, szConfigPath))
	{
		CoTaskMemFree(pidl);
		
		wcscat(szConfigPath, L"\\Tencent\\WeChat\\All Users\\config\\configEx.ini");
		if (!_waccess(szConfigPath, 0))
		{
			WritePrivateProfileStringW(L"Update", L"CrashCnt", L"0", szConfigPath);
			WritePrivateProfileStringW(L"Update", L"NeedUpdateType", L"0", szConfigPath);
		}
	}

	HKEY phkResult = NULL;
	DWORD dwDisposition = NULL;
	if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Tencent\\WeChat", 0, 0, 0, 0x2001Fu, 0, &phkResult, &dwDisposition))
	{
		DWORD dwData = 0;
		RegSetValueEx(phkResult, L"CrashCnt", 0, REG_DWORD, (const BYTE *)&dwData, 4u);
		RegSetValueEx(phkResult, L"NeedUpdateType", 0, REG_DWORD, (const BYTE *)&dwData, 4u);
		RegCloseKey(phkResult);
	}
}

void CDebugHelper::EnableMultiProcess()
{
	HANDLE handle = FindHandle(GetCurrentProcessId(), L"Mutant", L"_WeChat_App_Instance_Identity_Mutex_Name");
	if (handle)
		CloseHandle(handle);
	
	/*handle = FindHandle(GetCurrentProcessId(), L"Mutant", L"WeChat_GlobalConfig_Multi_Process_Mutex");
	if (handle)
		CloseHandle(handle);*/

	LOG("已解除多开限制!");
}

static CStringA Hex2String(const unsigned char *szHex, int nLen)
{
	CStringA str;

	for (int i=0; i <nLen; i++)
	{
		CStringA strTemp;
		strTemp.Format("%02x ", szHex[i]);
		str += strTemp;
	}

	return str;
}

static struct AutoBuffer
{
	const unsigned char* buf;
	size_t pos_;
	size_t length_;
};

static void __stdcall PrintEncodeReqbuf(int nTaskId, AutoBuffer *bufInput, AutoBuffer *bufOutput)
{
	try
	{
		if (bufInput)
		{
			CStringA str;
			str.Format("[taskid=%d][len=%d][input]%s", nTaskId, bufInput->length_, Hex2String(bufInput->buf, bufInput->length_));
			LOG("%s", str.GetBuffer());
		}

		if (bufOutput)
		{
			CStringA str;
			str.Format("[taskid=%d][len=%d][output]%s", nTaskId, bufOutput->length_, Hex2String(bufOutput->buf, bufOutput->length_));
			LOG("%s", str.GetBuffer());
		}
	}
	catch (...)
	{
		LOG("Crash!");
	}
}

//Hook encode_reqbuf 返回地址
static DWORD s_dwEncodeReqbufRet = NULL;
void __declspec(naked) __stdcall hook_encode_reqbuf()
{
	__asm
	{
		pushad
		pushfd
		push DWORD PTR[ebp + 8]				//Output AutoBuffer pointer
		mov eax, [ebp + 0xc]				
		push [eax]							//Input AutoBuffer pointer
		push DWORD PTR[ebp + 0x18]			//task id;
		call PrintEncodeReqbuf
		popfd
		popad

		mov eax, [ecx + 8]
		cmp [ecx + 4], eax

		push s_dwEncodeReqbufRet
		ret
	}
}


static void __stdcall PrintDecodeRespbuf(int nTaskId, AutoBuffer *bufInput, AutoBuffer *bufOutput)
{
	try
	{
		if (bufInput)
		{
			CStringA str;
			str.Format("[taskid=%d][len=%d][input]%s", nTaskId, bufInput->length_, Hex2String(bufInput->buf, bufInput->length_));
			LOG("%s", str.GetBuffer());
		}

		if (bufOutput)
		{
			CStringA str;
			str.Format("[taskid=%d][len=%d][output]%s", nTaskId, bufOutput->length_, Hex2String(bufOutput->buf, bufOutput->length_));
			LOG("%s", str.GetBuffer());
		}
	}
	catch (...)
	{
		LOG("Crash!");
	}
}

//orgin_decode_respbuf_call 返回地址
static DWORD s_dwOriginDecodeRespbufRet = NULL;
void __declspec(naked) __cdecl origin_decode_respbuf_call(int a1, int a2, int a3)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		and     esp, 0xFFFFFFF8

		push s_dwOriginDecodeRespbufRet
		ret
	}
}

void __declspec(naked) __cdecl hook_decode_respbuf(int a1, int a2, int a3)
{
	__asm
	{
		push ebp
		mov ebp, esp
			pushad
			pushfd
				push a3
				push a2
				push a1
				call origin_decode_respbuf_call		   //首先调用原解包函数解包 
				add esp, 0xc
			popfd
			popad

			pushad
			pushfd
				mov eax, edx
				push [eax]								//Output AutoBuffer pointer
				mov eax, ecx
				push [eax]								//Input AutoBuffer pointer
				push DWORD PTR[ebp + 0x24 + 4]			//task id;
				call PrintDecodeRespbuf
			popfd
			popad
		mov esp, ebp
		pop ebp
		ret
	}
}

void CDebugHelper::DumpProto()
{
	//hook 发包
	m_spHookEncode = make_shared<CHOOK>(BASEADDR_TYPE_HOOK_ENCODE_REQBUF, 6, (DWORD)hook_encode_reqbuf, s_dwEncodeReqbufRet);
	m_spHookEncode->HookOn();

	//hook 收包
	m_spHookDecode = make_shared<CHOOK>(BASEADDR_TYPE_HOOK_DECODE_RESPBUF, 6, (DWORD)hook_decode_respbuf, s_dwOriginDecodeRespbufRet);
	m_spHookDecode->HookOn();
}

void CDebugHelper::CancelDumpProto()
{
	if (m_spHookEncode)
	{
		m_spHookEncode->HookOff();
		m_spHookEncode = NULL;
	}
		
	if (m_spHookDecode)
	{
		m_spHookDecode->HookOff();
		m_spHookDecode = NULL;
	}
}

void CDebugHelper::DisableMmtls()
{
	auto spBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_IS_MMTLS_ENABLE);
	if (spBase && spBase->GetAddr())
	{
		//B8 00000000 mov eax,00000000
		//C3		  ret
		const unsigned char szCode[] = { 0xB8, 0, 0, 0, 0, 0xC3 };
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)spBase->GetAddr(), szCode, sizeof(szCode), NULL);

		LOG("已禁用mmtls!");
	}
}

void CDebugHelper::DisableLonglink()
{
	auto spBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_LONGLINK_RUN_CONNECT);
	if (spBase && spBase->GetAddr())
	{
		//B8 FFFFFFFF  mov eax, INVALID_SOCKET
		//C2 0400      ret 4
		const unsigned char szCode[] = { 0xB8, 0xFF, 0xFF,  0xFF,  0xFF,  0xC2,  0x04,  0x00 };
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)spBase->GetAddr(), szCode, sizeof(szCode), NULL);

		LOG("已禁用mmtls longlink!");
	}
}

void CDebugHelper::FuckReport()
{
	//禁用CreateToolhelp32Snapshot
	//B8 FFFFFFFF   mov eax, INVALID_HANDLE_VALUE
	//C2 0800       ret 0008
	LPVOID lpCreateToolhelp32SnapshotFun = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "CreateToolhelp32Snapshot");
	if (lpCreateToolhelp32SnapshotFun)
	{
		const unsigned char szCode[] = { 0xB8, 0xFF, 0xFF,  0xFF,  0xFF,  0xC2,  0x08,  0x00 };
		WriteProcessMemory(GetCurrentProcess(), lpCreateToolhelp32SnapshotFun, szCode, sizeof(szCode), NULL);
		LOG("已禁用CreateToolhelp32Snapshot:0x%x", lpCreateToolhelp32SnapshotFun);
	}	
}

void CDebugHelper::PrintXlog(const char *szLog, DWORD dwLen)
{
	LOCK(m_mutex_xlog);

#if OPEN_CONSOLE_XLOG
	//控制台输出
	printf(szLog);
#endif

	//写入日志文件
	m_list_xlog.push_back(CStringA(szLog));
}

void CDebugHelper::WriteXLogThread()
{
	LOG("++++++");

	while (TRUE)
	{
		CStringA str = pDebugHelper->FetchXlog();

		if (!str.IsEmpty())
		{
			if (pDebugHelper->m_fpXlog)
			{
				fwrite(str, 1, str.GetLength(), pDebugHelper->m_fpXlog);
				fflush(pDebugHelper->m_fpXlog);
			}
		}

		Sleep(XLOG_FILE_FLUSH_TIME);
	}

	LOG("------");
}

CStringA CDebugHelper::FetchXlog()
{
	LOCK(m_mutex_xlog);

	CStringA strRet;
	while (m_list_xlog.size())
	{
		strRet += m_list_xlog.front();
		m_list_xlog.pop_front();
	}

	return strRet;
}

#include "stdafx.h"
#include "hook.h"

#pragma comment(lib,"Version.lib")

INIT_SINGLETON(CBaseAddrMgr)

//基址特征码	
void CBaseAddrMgr::InitFeatureCode()
{
	//wechatwin.dll版本号
	CString strVer = GetVersion();

	if ((VERSION_2_6_4_38 == strVer) || (VERSION_2_6_4_56 == strVer))
	{
		CBaseAddr baseAddr[BASEADDR_TYPE_CNT] =
		{
			{ BASEADDR_TYPE_OPEN_XLOG, "open_xlog",{ (char)0xFF, (char)0xB5, (char)0xE0, (char)0xBF, (char)0xFF, (char)0xFF, (char)0xFF, (char)0xB5, (char)0xD8, (char)0xBF, (char)0xFF, (char)0xFF },{}, 6, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_XLOGGER_LEVEL, "xlogger_level",{ (char)0x2B, (char)0x8B, (char)0x50, (char)0x02, (char)0x00, (char)0x00 },{}, 0x2D, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_ENCODE_REQBUF, "hook_encode_reqbuf",{ (char)0x8B, (char)0x4F, (char)0x08, (char)0x8B, (char)0x42, (char)0x04, (char)0x51 },{ (char)0xC7, (char)0x41, (char)0x04, (char)0x00, (char)0x00 }, 7, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_DECODE_RESPBUF, "hook_decode_respbuf",{ (char)0x8A, (char)0xC3, (char)0x24, (char)0xFC, (char)0x3C, (char)0xB0 },{}, -19, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_IS_MMTLS_ENABLE, "is_mmtls_enable_from_auth",{ (char)0x8D, (char)0x86, (char)0x38, (char)0x0C, (char)0x00, (char)0x00, (char)0xC6, (char)0x45, (char)0xEC, (char)0x00 },{}, -62, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_LONGLINK_RUN_CONNECT, "longlink_run_connect",{ (char)0xFF, (char)0x10, (char)0xC6, (char)0x45, (char)0xFC, (char)0x0D },{ (char)0x83, (char)0x66, (char)0x2C, (char)0x00, (char)0x8B, (char)0xCB }, -45, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_TEXT_MSG_CALL, "send_text_msg",{ (char)0xB8, (char)0x39, (char)0x8E, (char)0xE3, (char)0x38, (char)0xF7, (char)0xE9, (char)0xC1, (char)0xFA, (char)0x03, (char)0x8B, (char)0xC2, (char)0xC1, (char)0xE8, (char)0x1F, (char)0x03, (char)0xC2, (char)0x83, (char)0xF8, (char)0x01 },{}, -10, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_IMAGE_MSG_GET_MGR_CALL, "send_image_msg_mgr_call",{ (char)0x53, (char)0xff, (char)0x75, (char)0xcc, (char)0x8d },{ (char)0xE8 }, 0, TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_IMAGE_MSG_CALL, "send_image_msg_call",{ (char)0x53, (char)0xFF, (char)0x75, (char)0xCC, (char)0x8D },{ (char)0x00, (char)0xC7, (char)0x45, (char)0xFC }, -4, TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_FILE_MSG_GET_MGR_CALL, "send_file_msg_mgr_call",{ (char)0x8B, (char)0x81, (char)0x9C, (char)0x00, (char)0x00, (char)0x00, (char)0xC3 },{}, 0x20, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_FILE_MSG_CALL, "send_file_msg_call",{ (char)0xC7, (char)0x45, (char)0xFC, (char)0x02, (char)0x00, (char)0x00, (char)0x00, (char)0x83, (char)0x7D, (char)0x10, (char)0x00 },{}, -65, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_SYNC_MSG, "hook_sync_msg",{ (char)0x3D, (char)0x0E, (char)0x27, (char)0x00, (char)0x00 },{}, 0,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_CREATE_CDN_DOWNLOAD_TASK, "hook_create_cdn_download_task",{ (char)0x8D, (char)0x85, (char)0xA4, (char)0xFC, (char)0xFF, (char)0xFF, (char)0x50, (char)0x8D, (char)0x85, (char)0x94, (char)0xFD, (char)0xFF, (char)0xFF },{}, 0x1C,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_CDN_TASK_ONSUCCEED, "hook_cdn_download_task_on_succeed",{ (char)0x8D, (char)0x8D, (char)0x58, (char)0xFE, (char)0xFF, (char)0xFF, (char)0xC6, (char)0x45, (char)0xFC, (char)0x0B },{}, 0,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_MMXOR_CALL, "mmxor_call",{ (char)0x69, (char)0xC9, (char)0x01, (char)0x01, (char)0x01, (char)0x01, (char)0x56, (char)0x57 },{}, -22,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_LOGIN_WXID, "get_login_wxid",{ (char)0x57, (char)0xc7, (char)0x06, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0xc7, (char)0x46, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x8b, (char)0x1d }, {}, 0x16,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_DB_INSTANCE, "db_get_db_instance",{ (char)0x33, (char)0xff, (char)0x83, (char)0xC3, (char)0x03, (char)0xC1 }, { (char)0x85, (char)0xF6, (char)0x74, (char)0x15, (char)0x8B, (char)0xCE }, -4,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_PREPARE_V2_CALL, "sqlite3_prepare_v2_call",{ (char)0x6a, (char)0x00, (char)0x56, (char)0x6a, (char)0x00, (char)0x6a, (char)0x01, (char)0x50, (char)0xe8 },{}, -47,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_STEP_CALL, "sqlite3_step_call",{ (char)0x83, (char)0xA6, (char)0x90, (char)0x00, (char)0x00, (char)0x00, (char)0xFD },{}, -143,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_FINALIZE_CALL, "sqlite3_finalize_call",{ (char)0x33, (char)0xDB, (char)0x3D, (char)0xA3, (char)0x0D, (char)0xF2, (char)0x2D },{}, -125,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_INT64_CALL, "sqlite3_column_int64_call",{ (char)0x6A, (char)0x10, (char)0x53, (char)0x88, (char)0x87, (char)0x94, (char)0x00, (char)0x00, (char)0x00 },{}, 0x9,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_TEXTA_CALL, "sqlite3_column_textA_call",{ (char)0xEB, (char)0x18, (char)0x6A, (char)0x0B, (char)0x56, (char)0xE8 },{}, 0x5,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_TEXTW_CALL, "sqlite3_column_textW_call",{ (char)0xEB, (char)0x18, (char)0x6A, (char)0x0B, (char)0x56, (char)0xE8 },{}, -17,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_BLOB_CALL, "sqlite3_column_blob_call",{ (char)0x6A, (char)0x0F, (char)0x56, (char)0x89, (char)0x44, (char)0x24, (char)0x20, (char)0xE8 },{}, -5,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_BYTES_CALL, "sqlite3_column_bytes_call",{ (char)0x6A, (char)0x0F, (char)0x56, (char)0x89, (char)0x44, (char)0x24, (char)0x20, (char)0xE8 },{}, 7,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
		};

		for (auto i : baseAddr)
			m_baseAddr.push_back(make_shared<CBaseAddr>(i));
	}
	else
	{
		//默认使用最新版本特征码
		CBaseAddr baseAddr[BASEADDR_TYPE_CNT] =
		{
			{ BASEADDR_TYPE_OPEN_XLOG, "open_xlog",{ (char)0xFF, (char)0xB5, (char)0xE0, (char)0xBF, (char)0xFF, (char)0xFF, (char)0xFF, (char)0xB5, (char)0xD8, (char)0xBF, (char)0xFF, (char)0xFF },{}, 6, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_XLOGGER_LEVEL, "xlogger_level",{ (char)0x2B, (char)0x8B, (char)0x50, (char)0x02, (char)0x00, (char)0x00 },{}, 0x2D, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_ENCODE_REQBUF, "hook_encode_reqbuf",{ (char)0x8B, (char)0x4F, (char)0x08, (char)0x8B, (char)0x42, (char)0x04, (char)0x51 },{ (char)0xC7, (char)0x41, (char)0x04, (char)0x00, (char)0x00 }, 7, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_DECODE_RESPBUF, "hook_decode_respbuf",{ (char)0x8A, (char)0xC3, (char)0x24, (char)0xFC, (char)0x3C, (char)0xB0 },{}, -19, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_IS_MMTLS_ENABLE, "is_mmtls_enable_from_auth",{ (char)0x8D, (char)0x86, (char)0x38, (char)0x0C, (char)0x00, (char)0x00, (char)0xC6, (char)0x45, (char)0xEC, (char)0x00 },{}, -62, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_LONGLINK_RUN_CONNECT, "longlink_run_connect",{ (char)0xFF, (char)0x10, (char)0xC6, (char)0x45, (char)0xFC, (char)0x0D },{ (char)0x83, (char)0x66, (char)0x2C, (char)0x00, (char)0x8B, (char)0xCB }, -45, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_TEXT_MSG_CALL, "send_text_msg",{ (char)0xB8, (char)0x39, (char)0x8E, (char)0xE3, (char)0x38, (char)0xF7, (char)0xE9, (char)0xC1, (char)0xFA, (char)0x03, (char)0x8B, (char)0xC2, (char)0xC1, (char)0xE8, (char)0x1F, (char)0x03, (char)0xC2, (char)0x83, (char)0xF8, (char)0x01 },{}, -10, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_IMAGE_MSG_GET_MGR_CALL, "send_image_msg_mgr_call",{ (char)0x53, (char)0xff, (char)0x75, (char)0xcc, (char)0x8d },{ (char)0xE8 }, 0, TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_IMAGE_MSG_CALL, "send_image_msg_call",{ (char)0x53, (char)0xFF, (char)0x75, (char)0xCC, (char)0x8D },{ (char)0x00, (char)0xC7, (char)0x45, (char)0xFC }, -4, TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_FILE_MSG_GET_MGR_CALL, "send_file_msg_mgr_call",{ (char)0x8B, (char)0x81, (char)0x9C, (char)0x00, (char)0x00, (char)0x00, (char)0xC3 },{  }, 0x20, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SEND_FILE_MSG_CALL, "send_file_msg_call",{ (char)0xC7, (char)0x45, (char)0xFC, (char)0x02, (char)0x00, (char)0x00, (char)0x00, (char)0x83, (char)0x7D, (char)0x10, (char)0x00 },{  }, -65, FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_SYNC_MSG, "hook_sync_msg",{ (char)0x3D, (char)0x0E, (char)0x27, (char)0x00, (char)0x00 },{}, 0,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_CREATE_CDN_DOWNLOAD_TASK, "hook_create_cdn_download_task",{ (char)0x8D, (char)0x85, (char)0xA4, (char)0xFC, (char)0xFF, (char)0xFF, (char)0x50, (char)0x8D, (char)0x85, (char)0x94, (char)0xFD, (char)0xFF, (char)0xFF },{}, 0x1C,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_HOOK_CDN_TASK_ONSUCCEED, "hook_cdn_download_task_on_succeed",{ (char)0x8D, (char)0x8D, (char)0x58, (char)0xFE, (char)0xFF, (char)0xFF, (char)0xC6, (char)0x45, (char)0xFC, (char)0x0B },{}, 0,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_MMXOR_CALL, "mmxor_call",{ (char)0x69, (char)0xC9, (char)0x01, (char)0x01, (char)0x01, (char)0x01, (char)0x56, (char)0x57 },{}, -22,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_LOGIN_WXID, "get_login_wxid",{ (char)0x57, (char)0xc7, (char)0x06, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0xc7, (char)0x46, (char)0x04, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x8b, (char)0x1d },{}, 0x16,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_DB_INSTANCE, "db_get_db_instance",{ (char)0x33, (char)0xff, (char)0x83, (char)0xC3, (char)0x03, (char)0xC1 },{ (char)0x85, (char)0xF6, (char)0x74, (char)0x15, (char)0x8B, (char)0xCE }, -4,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_PREPARE_V2_CALL, "sqlite3_prepare_v2_call",{ (char)0x6a, (char)0x00, (char)0x56, (char)0x6a, (char)0x00, (char)0x6a, (char)0x01, (char)0x50, (char)0xe8 },{}, -47,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_STEP_CALL, "sqlite3_step_call",{ (char)0x83, (char)0xA6, (char)0x90, (char)0x00, (char)0x00, (char)0x00, (char)0xFD }, {}, -143,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_FINALIZE_CALL, "sqlite3_finalize_call",{ (char)0x33, (char)0xDB, (char)0x3D, (char)0xA3, (char)0x0D, (char)0xF2, (char)0x2D },{}, -125,  FALSE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_INT64_CALL, "sqlite3_column_int64_call",{ (char)0x6A, (char)0x10, (char)0x53, (char)0x88, (char)0x87, (char)0x94, (char)0x00, (char)0x00, (char)0x00 },{}, 0x9,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_TEXTA_CALL, "sqlite3_column_textA_call",{ (char)0xEB, (char)0x18, (char)0x6A, (char)0x0B, (char)0x56, (char)0xE8 },{}, 0x5,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_TEXTW_CALL, "sqlite3_column_textW_call",{ (char)0xEB, (char)0x18, (char)0x6A, (char)0x0B, (char)0x56, (char)0xE8 },{}, -17,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_BLOB_CALL, "sqlite3_column_blob_call",{ (char)0x6A, (char)0x0F, (char)0x56, (char)0x89, (char)0x44, (char)0x24, (char)0x20, (char)0xE8 },{}, -5,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
			{ BASEADDR_TYPE_SQLITE3_COLUMN_BYTES_CALL, "sqlite3_column_bytes_call",{ (char)0x6A, (char)0x0F, (char)0x56, (char)0x89, (char)0x44, (char)0x24, (char)0x20, (char)0xE8 },{}, 7,  TRUE, SEARCH_START_ADDR, SEARCH_END_ADDR },
		};

		for (auto i : baseAddr)
			m_baseAddr.push_back(make_shared<CBaseAddr>(i));
	}
}



CBaseAddr::CBaseAddr(BASEADDR_TYPE type, char *szBaseName, string strMainCode, string strSubCode, int nOffset /*= 0*/, BOOL bRelativeCall/* = FALSE*/, DWORD dwStartAddr /*= 0x00401000*/, DWORD dwEndAddr /*= 0x01000000*/, DWORD dwSubSearchLen /*= 0x100*/)
{
	m_type = type;
	m_strBaseName = szBaseName;
	m_strMainCode = strMainCode;
	m_strSubCode = strSubCode;
	m_nOffset	= nOffset;
	m_bRelativeCall = bRelativeCall;
	m_dwStartAddr = dwStartAddr;
	m_dwEndAddr = dwEndAddr;
	m_dwSubSearchLen = dwSubSearchLen;
}

BOOL CBaseAddr::SearchCode()
{
	try
	{
		if (!m_strMainCode.size())	return FALSE;

		DWORD dwResultAddr = 0;

		for (unsigned char *p = (unsigned char *)m_dwStartAddr; (DWORD)p < (m_dwEndAddr); p++)
		{
			if (memcmp(p, m_strMainCode.c_str(), m_strMainCode.size()))
				continue;

			//LOG("[%s]p = 0x%x\n", m_strBaseName.c_str(), (DWORD)p);
			dwResultAddr = (DWORD)p;

			if (m_strSubCode.size())
			{
				for (unsigned char *q = (unsigned char *)p; (DWORD)q < (DWORD)p + m_dwSubSearchLen; q++)
				{
					if (memcmp(q, m_strSubCode.c_str(), m_strSubCode.size()))
						continue;

					//LOG("[%s]q = 0x%x\n", m_strBaseName.c_str(), (DWORD)q);
					dwResultAddr = (DWORD)q;
					break;
				}

				if (dwResultAddr == (DWORD)p)
					continue;
			}

			m_dwBaseAddr = dwResultAddr + m_nOffset;

			if (m_bRelativeCall)
			{
				m_dwBaseAddr = GET_CALL_ADDR(m_dwBaseAddr);
			}
#ifdef DEBUG
			LOG("%s特征码搜索结果:0x%x(Base+0x%x)", m_strBaseName.c_str(), m_dwBaseAddr, m_dwBaseAddr - pBaseAddrMgr->GetModuleBaseAddr());
#endif
			return TRUE;
		}
	}
	catch (...)
	{
		LOG("Crash!!!!!");
	}
	
	m_dwBaseAddr = 0;
	return FALSE;
}

CHOOK::CHOOK(BASEADDR_TYPE type, DWORD dwLenCode, DWORD dwMyFunAddr, DWORD &dwHookRetAddr)
{
	shared_ptr<CBaseAddr> pBase	= pBaseAddrMgr->GetBaseAddr(type);
	m_dwMyFunAddr	= dwMyFunAddr;
	dwHookRetAddr	= NULL;

	if (pBase && pBase->GetAddr())
	{
		m_dwHookAddr = pBase->GetAddr();
		m_strOriginalCode = string((const char *)m_dwHookAddr, dwLenCode);
		dwHookRetAddr = m_dwHookAddr + dwLenCode;
	}
}

CHOOK::CHOOK(DWORD dwHookAddr, DWORD dwLenCode, DWORD dwMyFunAddr, DWORD &dwHookRetAddr)
{
	m_dwHookAddr = dwHookAddr;
	m_dwMyFunAddr = dwMyFunAddr;
	dwHookRetAddr = NULL;

	if (m_dwHookAddr)
	{
		m_strOriginalCode = string((const char *)m_dwHookAddr, dwLenCode);
		dwHookRetAddr = m_dwHookAddr + dwLenCode;
	}
}

BOOL CHOOK::HookOn()
{
	if (m_bHookOn)
	{
		return TRUE;
	}
	
	if (m_dwMyFunAddr && m_dwHookAddr)
	{
		unsigned char szCode[5] = { 0xe9 };
		DWORD dwJmp = m_dwMyFunAddr - m_dwHookAddr - 5;
		memcpy(szCode + 1, &dwJmp, 4);

		WriteProcessMemory(GetCurrentProcess(), (LPVOID)m_dwHookAddr, szCode, 5, NULL);

		m_bHookOn = TRUE;

		return TRUE;
	}
	
	return FALSE;
}

BOOL CHOOK::HookOff()
{
	if (!m_bHookOn)
	{
		return TRUE;
	}

	if (m_dwHookAddr)
	{
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)m_dwHookAddr, m_strOriginalCode.c_str(), m_strOriginalCode.size(), NULL);

		m_bHookOn = FALSE;

		return TRUE;
	}

	return FALSE;
}

CBaseAddrMgr::CBaseAddrMgr()
{
	//模块基址
	m_dwModuleBaseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
}

shared_ptr<CBaseAddr> CBaseAddrMgr::GetBaseAddr(BASEADDR_TYPE type)
{
	for (auto baseAddr : m_baseAddr)
	{
		if (type == baseAddr->GetType())
		{
			return baseAddr;
		}
	}

	return NULL;
}

CString CBaseAddrMgr::GetVersion()
{
	if (!m_strVersion.IsEmpty())
		return m_strVersion;
	
	try
	{
		wchar_t szDllPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, szDllPath);
		wcscat(szDllPath, L"\\WeChatWin.dll");

		DWORD dwVerHandle = 0;
		DWORD dwVerInfoSize = ::GetFileVersionInfoSize(szDllPath, &dwVerHandle);
		if (dwVerInfoSize)
		{
			HANDLE hMem;
			LPVOID lpvMem;
			unsigned int uInfoSize = 0;
			VS_FIXEDFILEINFO * pFileInfo;

			hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
			lpvMem = ::GlobalLock(hMem);
			::GetFileVersionInfo(szDllPath, dwVerHandle, dwVerInfoSize, lpvMem);
			::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);

			WORD nVer[4];
			nVer[0] = HIWORD(pFileInfo->dwFileVersionMS);
			nVer[1] = LOWORD(pFileInfo->dwFileVersionMS);
			nVer[2] = HIWORD(pFileInfo->dwFileVersionLS);
			nVer[3] = LOWORD(pFileInfo->dwFileVersionLS);
			m_strVersion.Format(_T("%d.%d.%d.%d"), nVer[0], nVer[1], nVer[2], nVer[3]);

			::GlobalUnlock(hMem);
			::GlobalFree(hMem);
		}
	}
	catch (...)
	{
		LOG("Crash!!!!!");
	}
	
	return m_strVersion;
}

int CBaseAddrMgr::VersionCompare(CString strVer1, CString strVer2)
{
	int nIndex1 = strVer1.Find(L".");
	int nIndex2 = strVer2.Find(L".");

	while ((-1 != nIndex1) && (-1 != nIndex2))
	{
		int nVer1 = _wtoi(strVer1.Left(nIndex1));
		int nVer2 = _wtoi(strVer2.Left(nIndex2));
		if (nVer1 == nVer2)
		{
			strVer1 = strVer1.Mid(nIndex1 + 1);
			strVer2 = strVer2.Mid(nIndex2 + 1);

			nIndex1 = strVer1.Find(L".");
			nIndex2 = strVer2.Find(L".");
		}
		else
			return (nVer1 > nVer2) ? 1 : -1;
	}

	int nVer1 = _wtoi(strVer1);
	int nVer2 = _wtoi(strVer2);
	if (nVer1 == nVer2)
		return 0;
	else
		return (nVer1 > nVer2) ? 1 : -1;

	return -1;
}

void CBaseAddrMgr::SearchFeatureCode(atomic<DWORD> *lpSearchIndex)
{
	DWORD nSearchIndex = (*lpSearchIndex)++;
	while (nSearchIndex < pBaseAddrMgr->m_baseAddr.size())
	{
		auto base = pBaseAddrMgr->m_baseAddr[nSearchIndex];
		if (!base->SearchCode())
		{
			LOG("%s特征码失效!", base->GetName().c_str());
		}

		nSearchIndex = (*lpSearchIndex)++;
	}
}

void CBaseAddrMgr::Init()
{
	LOG("开始搜索基址特征码......");
	
	//初始化特征码
	InitFeatureCode();

	//特征码搜索(多线程加快搜索速度)
	atomic<DWORD> nSearchIndex = 0;
	list<shared_ptr<thread>> l;	
	for (DWORD i = 0; i < max(1, (thread::hardware_concurrency() - 1)); i++)
	{
		shared_ptr<thread> t = make_shared<thread>(SearchFeatureCode, &nSearchIndex);
		l.push_back(t);
	}
	for (auto t : l)
		t->join();		

	LOG("基址特征码搜索完成!");
}


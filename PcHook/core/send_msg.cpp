#include "stdafx.h"
#include "send_msg.h"

INIT_SINGLETON(CSendMsg);

//发送文字call
static DWORD s_dwSendTextMsgCall  = NULL;
//发图片基址
static DWORD s_dwSendImageMsgBaseCall = NULL;
//发送图片call
static DWORD s_dwSendImageMsgCall = NULL;
//发文件基址
static DWORD s_dwSendFileMsgBaseCall = NULL;
//发文件call
static DWORD s_dwSendFileMsgCall = NULL;


void __declspec(naked) __stdcall _SendTextMsg_old(void *pRet, CMMString *pToWxid, CMMString *pContent)
{
	_asm
	{
		push ebp
		mov ebp, esp

			push 1
			push 0
			push pContent
			push pToWxid
			push pRet

			mov eax, s_dwSendTextMsgCall
			call eax

		mov esp, ebp
		pop ebp
		ret 0xc
	}
}

void __declspec(naked) __stdcall _SendTextMsg_2_6_5_38(void *pRet, CMMString *pToWxid, CMMString *pContent)
{
	_asm
	{
		push ebp
		mov ebp, esp

		push 1
		push 0
		push pContent
		mov edx, pToWxid					//usercall
		mov ecx, pRet

		mov eax, s_dwSendTextMsgCall
		call eax

		mov esp, ebp
		pop ebp
		ret 0xc
	}
}


void __declspec(naked) __stdcall _SendImageMsg_old(void *pRet, CMMString *pToWxid, CMMString *pImagePath)
{
	_asm
	{
		push ebp
		mov ebp, esp
		pushad

			push pImagePath
			push pToWxid
			push pRet

			mov eax, s_dwSendImageMsgCall
			call eax

		popad
		mov esp, ebp
		pop ebp
		ret 0xc
	}
}

void __declspec(naked) __stdcall _SendImageMsg_2_6_5_38(void *pRet, CMMString *pToWxid, CMMString *pImagePath)
{
	_asm
	{
		push ebp
		mov ebp, esp
		pushad

			push pImagePath
			push pToWxid
			push pRet

			mov eax, s_dwSendImageMsgBaseCall
			call eax
			mov ecx, eax						//this call

			mov eax, s_dwSendImageMsgCall
			call eax

		popad
		mov esp, ebp
		pop ebp
		ret 0xc
	}
}

void __declspec(naked) __stdcall _SendFileMsg(void *pRet, wchar_t *szToWxid, int nLenToWxid, wchar_t *szFilePath, int nLenFilePath)
{
	_asm
	{
		push ebp
		mov ebp, esp
		pushad

			//push 0							//兼容旧版本

			push 0
			push 0
			push 0
			push 0
			push 0								//null string

			push 0
			push 0
			push nLenFilePath
			push nLenFilePath
			push szFilePath

			push 0
			push 0
			push nLenToWxid
			push nLenToWxid
			push szToWxid

			push pRet

			mov eax, s_dwSendFileMsgBaseCall
			call eax
			mov ecx, eax

			mov eax, s_dwSendFileMsgCall
			call eax
		
		popad
		mov esp, ebp
		pop ebp
		ret 0x14
	}
}

CSendMsg::CSendMsg()
{
	shared_ptr<CBaseAddr>  pSendTextMsgBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SEND_TEXT_MSG_CALL);
	if (pSendTextMsgBase)
	{
		DWORD dwTemp = pSendTextMsgBase->GetAddr() + *(BYTE *)(pSendTextMsgBase->GetAddr() + 1) + 2;
		if (0xE8 == *(BYTE *)dwTemp)
		{
			for (int i = 1; i < 100; i++)
			{
				if (0xE8 == *(BYTE *)(dwTemp + i))
				{
					s_dwSendTextMsgCall = GET_CALL_ADDR(dwTemp + i);
					break;
				}
			}
		}
	}

	shared_ptr<CBaseAddr>  pSendImageMsgMgrBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SEND_IMAGE_MSG_GET_MGR_CALL);
	if (pSendImageMsgMgrBase)
	{
		s_dwSendImageMsgBaseCall = pSendImageMsgMgrBase->GetAddr();
	}

	shared_ptr<CBaseAddr>  pSendImageMsgBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SEND_IMAGE_MSG_CALL);
	if (pSendImageMsgBase)
	{
		s_dwSendImageMsgCall = pSendImageMsgBase->GetAddr();
	}

	shared_ptr<CBaseAddr>  pSendFileMsgMgrBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SEND_FILE_MSG_GET_MGR_CALL);
	if (pSendFileMsgMgrBase)
	{
		s_dwSendFileMsgBaseCall = pSendFileMsgMgrBase->GetAddr();
	}

	shared_ptr<CBaseAddr>  pSendFileMsgCall = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SEND_FILE_MSG_CALL);
	if (pSendFileMsgCall)
	{
		s_dwSendFileMsgCall = pSendFileMsgCall->GetAddr();
	}
}

void CSendMsg::SendTextMsg(wstring strToWxid, wstring strText)
{
	if (s_dwSendTextMsgCall)
	{
		char pRet[0x1000] = { 0 };

		CMMString mmstrContent, mmstrToWxid;
		mmstrContent.m_szBuf = strText.c_str();
		mmstrContent.m_dwLen = strText.size();
		mmstrToWxid.m_szBuf = strToWxid.c_str();
		mmstrToWxid.m_dwLen = strToWxid.size();

		//版本兼容
		if (pBaseAddrMgr->VersionCompare(pBaseAddrMgr->GetVersion(), VERSION_2_6_5_38) < 0)
			_SendTextMsg_old(pRet, &mmstrToWxid, &mmstrContent);
		else
		{
			_SendTextMsg_2_6_5_38(pRet, &mmstrToWxid, &mmstrContent);
		}		
	}	
}

void CSendMsg::SendImageMsg(wstring strToWxid, wstring strImagePath)
{
	if (s_dwSendImageMsgCall)
	{
		char pRet[0x1000] = { 0 };

		CMMString mmstrImagePath, mmstrToWxid;
		mmstrImagePath.m_szBuf = strImagePath.c_str();
		mmstrImagePath.m_dwLen = strImagePath.size();
		mmstrToWxid.m_szBuf = strToWxid.c_str();
		mmstrToWxid.m_dwLen = strToWxid.size();

		//版本兼容
		if (pBaseAddrMgr->VersionCompare(pBaseAddrMgr->GetVersion(), VERSION_2_6_5_38) < 0)
			_SendImageMsg_old(pRet, &mmstrToWxid, &mmstrImagePath);
		else
		{
			if (s_dwSendImageMsgBaseCall)
				_SendImageMsg_2_6_5_38(pRet, &mmstrToWxid, &mmstrImagePath);
		}		
	}
}

void CSendMsg::SendFileMsg(wstring strToWxid, wstring strFilePath)
{
	if (s_dwSendFileMsgBaseCall && s_dwSendFileMsgCall)
	{
		char pRet[0x1000] = { 0 };

		//不能直接使用new申请内存, 否则跨模块free时会crash
		HANDLE hHeap = GetProcessHeap();
		wchar_t *szFilePath = (wchar_t *)HeapAlloc(hHeap, 0, (strFilePath.size() + 1) * 2);
		wcscpy(szFilePath, strFilePath.c_str());
		wchar_t *szToWxid  = (wchar_t *)HeapAlloc(hHeap, 0, (strToWxid.size() + 1) * 2);
		wcscpy(szToWxid, strToWxid.c_str());

		_SendFileMsg(pRet, szToWxid, strToWxid.size(), szFilePath, strFilePath.size());
	}
}

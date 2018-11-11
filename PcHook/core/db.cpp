#include "stdafx.h"
#include "db.h"
#include "sub_class_wnd.h"

INIT_SINGLETON(CDbUtil);

sqlite3_column_textA_f sqlite3_column_textA = NULL;
sqlite3_column_textW_f sqlite3_column_textW = NULL;
sqlite3_column_blob_f sqlite3_column_blob = NULL;
sqlite3_column_bytes_f sqlite3_column_bytes = NULL;

static DWORD s_sqlite3_prepare_v2_fun = NULL;
static DWORD s_sqlite3_column_int64_fun = NULL;

typedef int(__cdecl *sqlite3_step_f)(void *);
sqlite3_step_f sqlite3_step = NULL;

typedef int(__cdecl *sqlite3_finalize_f)(void *);
sqlite3_finalize_f sqlite3_finalize = NULL;

void __declspec(naked) __stdcall sqlite3_prepare_v2(void *pDbInstance, void *ppStmt, const char *szSql)
{
	__asm
	{
		push ebp
		mov ebp, esp

			push szSql
			push ppStmt
			mov ecx, pDbInstance
			mov eax, s_sqlite3_prepare_v2_fun
			call eax

		mov esp, ebp
		pop ebp
		ret 0xc
	}
}

INT64 __declspec(naked) __stdcall sqlite3_column_int64(void *pStmt, int col)
{
	__asm
	{
		push ebp
		mov ebp, esp

			push col
			push pStmt
			mov eax, s_sqlite3_column_int64_fun
			call eax
		
		mov esp, ebp
		pop ebp
		ret 0x8
	}
}

CDbUtil::CDbUtil()
{
	shared_ptr<CBaseAddr>  pBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SQLITE3_PREPARE_V2_CALL);
	if (pBase)
		s_sqlite3_prepare_v2_fun = pBase->GetAddr();
	
	pBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SQLITE3_STEP_CALL);
	if (pBase)
		sqlite3_step = (sqlite3_step_f)pBase->GetAddr();

	pBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SQLITE3_FINALIZE_CALL);
	if (pBase)
		sqlite3_finalize = (sqlite3_finalize_f)pBase->GetAddr();

	pBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SQLITE3_COLUMN_INT64_CALL);
	if (pBase)
		s_sqlite3_column_int64_fun = pBase->GetAddr();

	pBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SQLITE3_COLUMN_TEXTA_CALL);
	if (pBase)
		sqlite3_column_textA = (sqlite3_column_textA_f)pBase->GetAddr();

	pBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SQLITE3_COLUMN_TEXTW_CALL);
	if (pBase)
		sqlite3_column_textW = (sqlite3_column_textW_f)pBase->GetAddr();

	pBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SQLITE3_COLUMN_BLOB_CALL);
	if (pBase)
		sqlite3_column_blob = (sqlite3_column_blob_f)pBase->GetAddr();

	pBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_SQLITE3_COLUMN_BYTES_CALL);
	if (pBase)
		sqlite3_column_bytes = (sqlite3_column_bytes_f)pBase->GetAddr();

	pBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_DB_INSTANCE);
	if (pBase && pBase->GetAddr())
		m_pDbMicroMsgInstance = (void *)GET_DWORD(GET_DWORD(pBase->GetAddr()));

}

BOOL CDbUtil::ExcuteSql(void *pInstance, LPCSTR szSql, std::function<BOOL(void *)> *cb)
{
	BOOL bRet = FALSE;

	try
	{
		if (pInstance)
		{
			void *pStmt = NULL;
			sqlite3_prepare_v2(pInstance, &pStmt, szSql);
			if (pStmt)
			{
				int nRet = sqlite3_step(pStmt);
				while (SQLITE_ROW == nRet)
				{
					if (cb)
						(*cb)(pStmt);

					nRet = sqlite3_step(pStmt);
				}				

				sqlite3_finalize(pStmt);
				bRet = (SQLITE_DONE == nRet);
			}
		}		
	}
	catch (...)
	{
		LOG("Crash!");
	}	

	return bRet;
}

list<shared_ptr<CContactInfo>> CDbUtil::GetContactList()
{
	list<shared_ptr<CContactInfo>> l;

	std::function<BOOL(void *)> cb = [&](void *pStmt)
	{
		if (pStmt)
		{
			auto spContactInfo = make_shared<CContactInfo>();
			spContactInfo->m_strWxid = sqlite3_column_textA(pStmt, 0);
			spContactInfo->m_strAlias = sqlite3_column_textA(pStmt, 1);
			spContactInfo->m_strEncryptUserName = sqlite3_column_textA(pStmt, 2);
			spContactInfo->m_strRemark = sqlite3_column_textA(pStmt, 10);
			spContactInfo->m_strNickName = sqlite3_column_textA(pStmt, 11);
			spContactInfo->m_strDomainList = sqlite3_column_textA(pStmt, 13);
			spContactInfo->m_strBigHeadImgUrl = sqlite3_column_textA(pStmt, 19);
			spContactInfo->m_strSmallHeadImgUrl = sqlite3_column_textA(pStmt, 20);
			spContactInfo->m_nDelFlag = (int)sqlite3_column_int64(pStmt, 3);
			spContactInfo->m_nType = (int)sqlite3_column_int64(pStmt, 4);
			spContactInfo->m_nVerifyFlag = (int)sqlite3_column_int64(pStmt, 5);
			spContactInfo->m_nChatRoomType = (int)sqlite3_column_int64(pStmt, 14);
			spContactInfo->m_nChatRoomNotify = (int)sqlite3_column_int64(pStmt, 23);
				
			//spContactInfo->ToString();

			l.push_back(spContactInfo);
			
			return true;
		}

		return false;
	};

	CMsgParam param;
	param.m_cstringA[0] = "select * from Contact where (Type&1) = 1";
	param.m_lpvoid[0] = GetContactInstance();
	param.m_lpvoid[1] = &cb;
	pSubClassMainWnd->SendMessage(WM_USER_SQL, &param);

	return l;
}

int CDbUtil::GetChatRecord()
{
	int nRet = 0;

	std::function<BOOL(void *)> cb = [&](void *pStmt)
	{
		if (pStmt)
		{
			CStringA strTalker = sqlite3_column_textA(pStmt, 10);
			CStringA strContent = CUTF82Gbk(sqlite3_column_textA(pStmt, 11));
			CStringA strReserved1 = CUTF82Gbk(sqlite3_column_textA(pStmt, 13));
			CStringA strTranslatedContent = CUTF82Gbk(sqlite3_column_textA(pStmt, 16));
			CStringA strTranslatedBrand = CUTF82Gbk(sqlite3_column_textA(pStmt, 17));
			CStringA strReserved3 = CUTF82Gbk(sqlite3_column_textA(pStmt, 19));
			CStringA strReserved4 = CUTF82Gbk(sqlite3_column_textA(pStmt, 20));
			UINT64 nLocalId = sqlite3_column_int64(pStmt, 0);
			UINT64 nTalkerId = sqlite3_column_int64(pStmt, 1);
			UINT64 nMsgSvrID = sqlite3_column_int64(pStmt, 2);
			UINT64 nType = sqlite3_column_int64(pStmt, 3);
			UINT64 nCreateTime = sqlite3_column_int64(pStmt, 9);

			//LOG("[CreateTime=%lld][LocalId=%lld][TalkerId=%lld][MsgSvrID=%lld][Type=%lld]Talker:%s, Content:%s, Reserved1:%s, TranslatedContent:%s, TranslatedBrand:%s, Reserved3:%s, Reserved4:%s", nCreateTime, nLocalId, nTalkerId, nMsgSvrID, nType, strTalker, strContent, strReserved1, strTranslatedContent, strTranslatedBrand, strReserved3, strReserved4);

			nRet++;

			return true;
		}

		return false;
	};


	CMsgParam param; 
	param.m_cstringA[0] = "select * from ChatCRMsg";			//新版本消息记录已迁移至MSG表
	param.m_lpvoid[0] = GetChatCRMsgInstance();
	param.m_lpvoid[1] = &cb;
	pSubClassMainWnd->SendMessage(WM_USER_SQL, &param);

	return nRet;
}

CStringA CDbUtil::GetLoginWxid()
{
	auto spLoginWxidBase = pBaseAddrMgr->GetBaseAddr(BASEADDR_TYPE_LOGIN_WXID);
	if (spLoginWxidBase && spLoginWxidBase->GetAddr())
	{
		string *pStrWxid = (string *)GET_DWORD(spLoginWxidBase->GetAddr());
		if (pStrWxid)
			return pStrWxid->c_str();
	}

	return "";
}

void * CDbUtil::GetContactInstance()
{
	if (m_pDbMicroMsgInstance)
		return (void *)((DWORD)m_pDbMicroMsgInstance + 0x24);

	return NULL;
}

void * CDbUtil::GetChatCRMsgInstance()
{
	if (pBaseAddrMgr->VersionCompare(pBaseAddrMgr->GetVersion(), VERSION_2_6_5_38) < 0)
	{
		if (m_pDbMicroMsgInstance)
			return (void *)((DWORD)m_pDbMicroMsgInstance + 0x560);
	}

	//新版本消息记录已迁移至MSG表
	return NULL;
}
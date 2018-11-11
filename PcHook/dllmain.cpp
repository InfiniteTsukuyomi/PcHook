// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

#include "core/debug_helper.h"
#include "core/send_msg.h"
#include "core/sync_msg.h"
#include "core/sub_class_wnd.h"
#include "core/db.h"


void TestSendMsg()
{
#ifndef DEBUG
	return;
#endif

#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

	//测试发图
	WCHAR szTempShortPath[MAX_PATH] = { 0 };
	WCHAR szTempPath[MAX_PATH] = { 0 };
	GetTempPath(MAX_PATH, szTempShortPath);
	GetLongPathName(szTempShortPath, szTempPath, MAX_PATH);
	PathRemoveBackslash(szTempPath);
	wsprintf(szTempPath, L"%s\\test_%d_%d.jpg", szTempPath, (DWORD)time(NULL), GetCurrentThreadId());

	srand(GetCurrentThreadId());
	CString strUrl;
	strUrl.Format(L"https://pics.javcdn.pw/cover/%d_b.jpg", rand()%5000 + 1);
	if (!URLDownloadToFile(NULL, strUrl, szTempPath, NULL, NULL))
	{
		CStringA strUrlA = CW2A(strUrl);
		CStringA strTempPathA = CW2A(szTempPath);
		LOG("测试文件%s下载成功, 文件路径:%s", strUrlA, strTempPathA);

		{
			//测试发文字消息
			CMsgParam param;
			param.m_wstring[0] = L"filehelper";
			param.m_wstring[1] = L"Hello!";
			pSubClassMainWnd->SendMessage(WM_USER_SEND_TEXT, &param);
		}
		
		{
			//测试发图片
			CMsgParam param;
			param.m_wstring[0] = L"filehelper";
			param.m_wstring[1] = szTempPath;
			pSubClassMainWnd->SendMessage(WM_USER_SEND_IMAGE, &param);
		}

		{
			//测试发文件
			CMsgParam param;
			param.m_wstring[0] = L"filehelper";
			param.m_wstring[1] = szTempPath;
			pSubClassMainWnd->SendMessage(WM_USER_SEND_FILE, &param);
		}

		{
			//测试获取联系人
			list<shared_ptr<CContactInfo>> l = pDbUtil->GetContactList();
			LOG("共有联系人%d位", l.size());

			//测试获取本地历史聊天记录
			int CntChatRecord = pDbUtil->GetChatRecord();
			LOG("本地历史消息记录共有%d条", CntChatRecord);
		}
	}
	else
	{
		LOG("测试文件%s下载失败！", CW2A(strUrl));
	}
}

void Start()
{
	try
	{
		LOG("加载成功, 当前客户端版本号:%S", pBaseAddrMgr->GetVersion());

		//允许多开
		pDebugHelper->EnableMultiProcess();

		//初始化基址
		pBaseAddrMgr->Init();

		//开启Xlog
		pDebugHelper->OpenXlog();

		//禁用长链接
		//pDebugHelper->DisableLonglink();
		//打印封包
		//pDebugHelper->DumpProto();
		
		//同步消息处理
		pSyncMsg->Start();

		//主窗口子类化(等待登录成功)
		pSubClassMainWnd->SubClassMainWnd();

		//发送消息函数测试
		thread t(TestSendMsg);
		t.join();
	}
	catch (...)
	{
		LOG("Crash!\n");
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Start, NULL, NULL, NULL));
		break;
	case DLL_THREAD_ATTACH:
		//LOG("线程启动:tid=%d", GetCurrentThreadId());
		break;
	case DLL_THREAD_DETACH:
		//LOG("线程退出:tid=%d", GetCurrentThreadId());
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
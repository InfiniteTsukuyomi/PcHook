#pragma once


enum WM_USER_MSG
{
	WM_USER_SEND_INVALID = WM_USER + 2018,
	
	//发送文字
	WM_USER_SEND_TEXT,
	
	//发送图片
	WM_USER_SEND_IMAGE,

	//发送文件
	WM_USER_SEND_FILE,

	//本地数据库查询
	WM_USER_SQL,
};

struct CMsgParam
{
	BOOL		m_bRet = FALSE;
	WM_USER_MSG m_msg = WM_USER_SEND_INVALID;
	DWORD		m_dword[5] = { 0 };
	LPVOID		m_lpvoid[5] = { NULL };
	CString		m_cstring[5];
	CStringA	m_cstringA[5];
	string      m_string[5];
	wstring     m_wstring[5];
};

class CSubClassMainWnd : public Singleton<CSubClassMainWnd>
{
public:
	//拦截处理主窗口消息
	void SubClassMainWnd();

	//设置主窗口句柄
	void SetWeChatHwnd(HWND hwnd) { m_hWeChatWnd = hwnd; }
	HWND GetWeChatHwnd() { return m_hWeChatWnd; }

	//自定义窗口过程
	static LRESULT __stdcall MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//对主窗口发消息
	BOOL SendMessage(WM_USER_MSG msg, CMsgParam *param);

	//原始窗口过程
	WNDPROC m_oldWndProc = NULL;
private:
	//枚举本进程主窗口
	HWND GetMainWnd();
	static BOOL __stdcall GetMainWndCallback(HWND hwnd, LPARAM lParam);

	HWND    m_hWeChatWnd = NULL;		//主窗口句柄
};
#define pSubClassMainWnd		(CSubClassMainWnd::GetInstance())




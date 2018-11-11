#pragma once

#define OPEN_CONSOLE_XLOG			0
#define XLOG_FILE_FLUSH_TIME		1*1000
#define XLOG_FILE_DIR				L"\\xLog\\"
#define XLOG_FILE_NAME				L"MM_%04d-%02d-%02d_%02d%02d%02d.xlog"


class CDebugHelper : public Singleton<CDebugHelper>
{
public:
	//开启xlog
	void OpenXlog();

	//关闭自动更新
	void CloseAutoUpdate();

	//允许多开
	void EnableMultiProcess();

	//打印封包
	void DumpProto();
	void CancelDumpProto();

	//关闭mmtls(仅支持旧版本, 当前版本关闭tls后部分封包返回SessionTimeout)
	void DisableMmtls();

	//关闭长链接
	void DisableLonglink();

	//禁用CreateToolhelp32Snapshot
	void FuckReport();
private:
	//保存xlog日志文件
	static void WriteXLogThread();

	//取出xlog
	CStringA FetchXlog();

	FILE *m_fpXlog = NULL;
	std::mutex m_mutex_xlog;
	list<CStringA> m_list_xlog;

	shared_ptr<CHOOK>  m_spHookEncode = NULL;
	shared_ptr<CHOOK>  m_spHookDecode = NULL;
public:
	//打印xlog
	void PrintXlog(const char *szLog, DWORD dwLen);
};
#define pDebugHelper	(CDebugHelper::GetInstance())



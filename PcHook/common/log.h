#pragma once

#ifdef DEBUG

//开启控制台log
#define USE_CONSOLE_LOG

//使用文件记录日志
#define USE_FILE_LOG

#endif

//调试器输出
//#define USE_OUTPUT_DEBUG_LOG

//日志缓冲区长度
#define MAX_LOG_BUFF		(10*1024)

//日志目录
#define LOG_PATH			L".\\log\\"
#define LOG_FILE_NAME		L"%04d-%02d-%02d.log"

class CLog
{
public:
	CLog();
	
	//记录日志
	void Log(const char * szLog, ...);

	static CLog *GetInstance();
private:
	FILE *m_fp = NULL;
	static CLog *m_instance;
};
#define pLog	(CLog::GetInstance())

#define LOG(fmt, ...)  pLog->Log("[%s] "fmt"\n",   __FUNCTION__,  ##__VA_ARGS__)
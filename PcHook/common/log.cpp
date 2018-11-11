#include "stdafx.h"
#include <atlbase.h>
#include <atlstr.h>
#include <shlobj.h>
#include <time.h>
#include "log.h"



CLog *CLog::m_instance = new CLog();

CLog::CLog()
{
#ifdef USE_CONSOLE_LOG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	setlocale(LC_ALL, "");
#endif

#ifdef USE_FILE_LOG
	if (m_fp)	
		fclose(m_fp);

	wchar_t szPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, szPath);
	wcscat(szPath, LOG_PATH);
	SHCreateDirectoryEx(NULL, szPath, NULL);
	
	wchar_t szFileName[MAX_PATH] = { 0 };
	time_t t = time(NULL);
	struct tm *tt = localtime(&t);
	wsprintf(szFileName, LOG_FILE_NAME, tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday);
	wcscat(szPath, szFileName);
	
	m_fp = _wfopen(szPath, L"ab+");
#endif
}

void CLog::Log(const char * szLog, ...)
{
	//±ÜÃâ»º³åÇøÒç³ö
	va_list va;
	va_start(va, szLog);
	CStringA strTemp;
	int nLenNeed = vsnprintf(strTemp.GetBuffer(MAX_LOG_BUFF + 1), MAX_LOG_BUFF + 1, szLog, va);
	if (-1 == nLenNeed)
		return;
	else if (nLenNeed > MAX_LOG_BUFF)
	{
		int nLen = vsnprintf(strTemp.GetBuffer(nLenNeed + 1), nLenNeed + 1, szLog, va);
		if (-1 == nLen || nLen > nLenNeed)
			return;
	}
	strTemp.ReleaseBuffer();
	va_end(va);
	
	time_t t = time(NULL);
	struct tm *tt = localtime(&t);
	
	CStringA strLog;
	strLog.Format("[%04d-%02d-%02d %02d:%02d:%02d]%s", tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday, tt->tm_hour, tt->tm_min, tt->tm_sec, strTemp);

#ifdef USE_FILE_LOG
	if (m_fp)
	{
		fwrite(strLog, 1, strLog.GetLength(), m_fp);
		fflush(m_fp);
	}
		
#endif

#ifdef USE_CONSOLE_LOG
	printf("%s", strLog.GetBuffer());
#endif

#ifdef USE_OUTPUT_DEBUG_LOG
	OutputDebugStringA("%s", strLog.GetBuffer());
#endif
}

CLog * CLog::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new CLog();
	}
	return m_instance;
}

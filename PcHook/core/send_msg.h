#pragma once

struct CMMString
{
	const wchar_t	*m_szBuf = NULL;
	DWORD			m_dwLen = 0;
	DWORD			m_dwMaxLen = 0;
	DWORD			m_padding[2] = { 0 };
};

class CSendMsg : public Singleton<CSendMsg>
{
public:
	CSendMsg();

	//发送文字消息
	void SendTextMsg(wstring strToWxid, wstring strText);

	//发送图片消息
	void SendImageMsg(wstring strToWxid, wstring strImagePath);

	//发送文件
	void SendFileMsg(wstring strToWxid, wstring strFilePath);
private:

};
#define pSendMsg	(CSendMsg::GetInstance())
#pragma once

class CSyncMsg : public Singleton<CSyncMsg>
{
public:
	//开启Hook 同步消息
	BOOL Start();

	//图片解密(异或)另存为
	BOOL MMXor(CString strEncryptFileName, CString strSaveAsFileName);
};
#define pSyncMsg		(CSyncMsg::GetInstance())
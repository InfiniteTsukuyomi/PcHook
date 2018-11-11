#pragma  once
#include <vector>

#define VERSION_2_6_4_38									L"2.6.4.38"
#define VERSION_2_6_4_56									L"2.6.4.56"
#define VERSION_2_6_5_38									L"2.6.5.38"

//基址类型
enum BASEADDR_TYPE
{
	BASEADDR_TYPE_INVALID = -1,

	BASEADDR_TYPE_OPEN_XLOG,								//开启xlog
	BASEADDR_TYPE_XLOGGER_LEVEL,							//xlogger_Level
	BASEADDR_TYPE_HOOK_ENCODE_REQBUF,						//Hook EncodeReqbuf
	BASEADDR_TYPE_HOOK_DECODE_RESPBUF,						//Hook DecodeRespbuf

	BASEADDR_TYPE_IS_MMTLS_ENABLE,							//mmtls开关(最新版本已失效)
	BASEADDR_TYPE_LONGLINK_RUN_CONNECT,						//长链接connect

	BASEADDR_TYPE_SEND_TEXT_MSG_CALL ,						//发文字消息call
	BASEADDR_TYPE_SEND_IMAGE_MSG_GET_MGR_CALL,				//发送图片消息mgr基址
	BASEADDR_TYPE_SEND_IMAGE_MSG_CALL,						//发图片消息call
	BASEADDR_TYPE_SEND_FILE_MSG_GET_MGR_CALL,				//发送文件消息mgr基址
	BASEADDR_TYPE_SEND_FILE_MSG_CALL,						//发送文件消息call

	BASEADDR_TYPE_HOOK_SYNC_MSG,							//Hook newsync同步消息
	BASEADDR_TYPE_HOOK_CREATE_CDN_DOWNLOAD_TASK,			//Hook cdn开始下载任务
	BASEADDR_TYPE_HOOK_CDN_TASK_ONSUCCEED,					//Hook cdn下载任务完成
	BASEADDR_TYPE_MMXOR_CALL,								//本地图片解密 mmxor call

	BASEADDR_TYPE_LOGIN_WXID,								//当前登录wxid基址
	BASEADDR_TYPE_DB_INSTANCE,								//MicroMsg数据库基址
	BASEADDR_TYPE_SQLITE3_PREPARE_V2_CALL,					//sqlite3_prepare_v2 call
	BASEADDR_TYPE_SQLITE3_STEP_CALL,						//sqlite3_step call
	BASEADDR_TYPE_SQLITE3_FINALIZE_CALL,					//sqlite3_finalize call
	BASEADDR_TYPE_SQLITE3_COLUMN_INT64_CALL,				//sqlite3_column_int64 call
	BASEADDR_TYPE_SQLITE3_COLUMN_TEXTA_CALL,				//sqlite3_column_textA call
	BASEADDR_TYPE_SQLITE3_COLUMN_TEXTW_CALL,				//sqlite3_column_textW call
	BASEADDR_TYPE_SQLITE3_COLUMN_BLOB_CALL,					//sqlite3_column_blob call
	BASEADDR_TYPE_SQLITE3_COLUMN_BYTES_CALL,				//sqlite3_column_bytes call
	
	BASEADDR_TYPE_CNT
};

//基址
class CBaseAddr
{
public:
	CBaseAddr(BASEADDR_TYPE type, char *szBaseName, string strMainCode, string strSubCode, int nOffset = 0, BOOL bRelativeCall = FALSE, DWORD dwStartAddr = 0x00401000, DWORD dwEndAddr = 0x01000000, DWORD dwSubSearchLen = 0x100);

	//特征码搜索
	BOOL SearchCode();
 
	DWORD GetAddr() { return m_dwBaseAddr; }
	BASEADDR_TYPE GetType() { return m_type; }
	string		  GetName() { return m_strBaseName; }
private:

	BASEADDR_TYPE	m_type = BASEADDR_TYPE_INVALID;		//baseaddr type
	string			m_strBaseName;						//基址功能名称
	DWORD			m_dwBaseAddr = NULL;				//基址
	string			m_strMainCode;						//特征码1
	string			m_strSubCode;						//特征码2
	int				m_nOffset;							//Hook点相对于特征码搜索结果的偏移
	BOOL			m_bRelativeCall;					//相对call地址
	DWORD			m_dwStartAddr;						//搜索起始地址
	DWORD			m_dwEndAddr;						//搜索结束地址
	DWORD			m_dwSubSearchLen;					//特征码2搜索长度
};

//Hook
class CHOOK
{
public:
	CHOOK(DWORD dwHookAddr, DWORD dwLenCode, DWORD dwMyFunAddr, DWORD &dwHookRetAddr);
	CHOOK(BASEADDR_TYPE type, DWORD dwLenCode, DWORD dwMyFunAddr, DWORD &dwHookRetAddr);
	
	//开启Hook
	BOOL HookOn();

	//关闭Hook
	BOOL HookOff();

private:
	DWORD					m_dwHookAddr = NULL;					//Hook基址
	BOOL					m_bHookOn = FALSE;						//当前Hook状态
	string					m_strOriginalCode;						//Hook点原始代码
	DWORD					m_dwMyFunAddr = NULL;					//Hook跳转函数地址
};

class CBaseAddrMgr : public Singleton<CBaseAddrMgr>
{
public:
	CBaseAddrMgr();

	//初始化所有基址
	void Init();

	//获取基址
	shared_ptr<CBaseAddr> GetBaseAddr(BASEADDR_TYPE type);

	//获取dll版本号
	CString GetVersion();

	//比较版本号
	int VersionCompare(CString strVer1, CString strVer2);

	//dll基址
	DWORD GetModuleBaseAddr() { return m_dwModuleBaseAddr; }
	DWORD GetModuleSize()	  { return m_dwModuleSize; }
private:
	//根据版本号初始化特征码
	void InitFeatureCode();

	//特征码搜索
	static void SearchFeatureCode(atomic<DWORD> *nSearchIndex);

	//特征码搜索起始地址
	DWORD  m_dwModuleBaseAddr = 0x00401000;
	DWORD  m_dwModuleSize     = 0x01000000;
	
	//dll版本号
	CString m_strVersion;

	//基址
	vector<shared_ptr<CBaseAddr>> m_baseAddr;
};
#define pBaseAddrMgr				(CBaseAddrMgr::GetInstance())

#define MODULE_BASE_ADDR		    (pBaseAddrMgr->GetModuleBaseAddr())
#define MODULE_SIZE					(pBaseAddrMgr->GetModuleSize())
#define SEARCH_START_ADDR			 MODULE_BASE_ADDR
#define SEARCH_END_ADDR				(MODULE_BASE_ADDR + MODULE_SIZE)

#define GET_DWORD(x)				(*(DWORD *)(x))
#define GET_CALL_ADDR(x)			(GET_DWORD(x+1) + x + 5)
#pragma once

#include "stdafx.h"
#include "IOCP.h"
#include "ServerGroup.h"
#include "AtumPreDBManager.h"

class CPreTickManager;

// client�� pre server���� ����
enum CP_State
{
	CP_NOTCONNECTED					= 0,	// Client�� �������� ���� ����
	CP_CONNECTED					= 1,	// Client�� Socket ������ ����
	CP_SINGLE_FILE_VERSIONCHECKED	= 2,	// Client�� SINGLE FILE VERSION�� update�� ����
	CP_VERSIONCHECKED				= 3,	// Client�� VERSION�� update�� ����
	CP_LOGGEDIN						= 4,	// Client�� AccountName�� ������ Login �� ����, Launcher�� ���� �α���
	CP_FIELD_LOGGEDIN				= 5,	// Client�� �ʵ� ������ ���� �α��� �� ����, ĳ���� ���� ȭ�鿡 �ִ� ��Ȳ
	CP_GAMESTARTED					= 6		// Client�� ������ �����Ͽ� �ʿ� �ִ� ��Ȳ
};

struct CAccountInfo
{
	char				AccountName[SIZE_MAX_ACCOUNT_NAME];
	UID32_t				AccountUniqueNumber;
	USHORT				AccountType;								// 2006-06-02 by cmkwon
	char				PublicClientIP[SIZE_MAX_IPADDRESS];
	char				PrivateClientIP[SIZE_MAX_IPADDRESS];
	char				CurrentCharacterName[SIZE_MAX_CHARACTER_NAME];
	UID32_t				CurrentCharacterUniqueNumber;
	char				CurrentServerGroup[SIZE_MAX_SERVER_NAME];
	SERVER_ID			CurrentFieldServerID;
	ClientIndex_t		CurrentClientIndex;			// �ʵ� ���������� client index
	ClientIndex_t		CurrentPreServerClientIndex;	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - PreServer ClientIndex
	MAP_CHANNEL_INDEX	CurrentMapChannel;
	CP_State			ClientState;
	ATUM_DATE_TIME		LauncherLoginTime;
	ATUM_DATE_TIME		AccountRegisteredDate;		// 2006-06-01 by cmkwon, ������ DB�� ��ϵ� �Ͻ�
	int					nExtAuthAccountIDNum;		// 2006-05-22 by cmkwon, �ܺ� ������ AccountIDNum
	char				PasswordFromDB[SIZE_MAX_PASSWORD_MD5_STRING];		// 2006-06-02 by cmkwon
	int					GameContinueTimeInSecondOfToday;				// 2006-11-15 by cmkwon, ���� �Ϸ� ���� ���� �ð�
	ATUM_DATE_TIME		LastGameEndDate;								// 2006-11-15 by cmkwon, ������ ���� ���� �ð�
	ATUM_DATE_TIME		atBirthday;						// 2007-06-28 by cmkwon, �߱� ��������(�������� FielServer�� ��������) - ���� ����
	char				SecondaryPassword[SIZE_MAX_PASSWORD_MD5_STRING];		// 2007-09-12 by cmkwon, ��Ʈ�� 2���н����� ���� - CAccountInfo �� �ʵ� �߰�
	BOOL				bSelectiveShutdownTarget;										// 2012-07-11 by hskim, ������ �˴ٿ�
	char				SelectiveShutdownInfo[SIZE_MAX_SELECTIVE_SHUTDOWN_INFO];		// 2012-07-11 by hskim, ������ �˴ٿ�

	void SetBirthday(int i_nBirthday);	// 2007-06-28 by cmkwon, �߱� ��������(�������� FielServer�� ��������) - �Լ� �߰�
};

inline MEX_ACCOUNT_INFO_PRE_SERVER& MEX_ACCOUNT_INFO_PRE_SERVER::operator=(const CAccountInfo& rhs)
{
	strncpy(this->AccountName, rhs.AccountName, SIZE_MAX_ACCOUNT_NAME);
	this->AccountUniqueNumber = rhs.AccountUniqueNumber;
	strncpy(this->PublicClientIP, rhs.PublicClientIP, SIZE_MAX_IPADDRESS);
	strncpy(this->PrivateClientIP, rhs.PrivateClientIP, SIZE_MAX_IPADDRESS);
	strncpy(this->CurrentCharacterName, rhs.CurrentCharacterName, SIZE_MAX_CHARACTER_NAME);
	this->CurrentCharacterUniqueNumber = rhs.CurrentCharacterUniqueNumber;
	strncpy(this->CurrentServerGroup, rhs.CurrentServerGroup, SIZE_MAX_SERVER_NAME);
	this->CurrentFieldServerID = rhs.CurrentFieldServerID;
	this->CurrentMapChannel = rhs.CurrentMapChannel;
	this->ClientState = rhs.ClientState;

	return *this;
}

typedef mt_map<string, CAccountInfo*, greater<string> >		mtmapAccountInfoPtr;
// 2007-01-09 by cmkwon, AtumParam.h�� ��ġ ���� typedef mt_map<VersionInfo, VersionInfo>					mtmapVersionInfo;

class CPreIOCPSocket;
class CAccountBlockManager;
class CInnovaLibrary;			// 2010-04-26 by cmkwon, ���þ� Innva ����/���� �ý��� ���� - 
class CHttpManager;				// PreServer ���� �� �ڵ� ���� ������Ʈ ����

class CPreIOCP : public CIOCP
{
	friend class CPreIOCPSocket;
	friend class CAtumPreDBManager;

public:
	explicit CPreIOCP(int nPort, char *szLocalIP = "127.0.0.1");
	
	~CPreIOCP();

	BOOL IsLogined(char* accountName);
	BOOL GetAccountInfo(CAccountInfo *o_pAccountInfo, const char* accountName);
	CAccountInfo *GetAccountInfoPtr(const char *i_szAccountName);
	BOOL InsertAccountInfo(char* accountName, CAccountInfo* accountInfo);
	BOOL DeleteAccountInfo(const char* accountName);
	void DeleteAccountInfoByServerGroupName(const char *i_szServerGroup);

	BOOL CreateVersionInfoFromLocalFile();
	BOOL WriteVersionInfoToLocalFile();
	BOOL GetLatestUpdateVersion(VersionInfo *o_pLatestVersion);
	BOOL GetNextUpdateVersion(VersionInfo *o_pNextVersion, VersionInfo *i_pVersion);

	BOOL BlockAccount(const SBLOCKED_ACCOUNT_INFO *i_pBlockedAccountInfo, CPreIOCPSocket *i_pSocketToNotify, BOOL i_bDisconnectUser=TRUE);
	BOOL UnblockAccount(const MSG_PA_ADMIN_UNBLOCK_ACCOUNT *i_pUnblockAccount, CPreIOCPSocket *i_pSocketToNotify);
	BOOL DisconnectAccount(CAccountInfo *i_pAccountInfo, Err_t i_nError, CPreIOCPSocket *i_pSocketToNotify);

	int On_MSG_FP_CONNECT_AUTH_USER(CAccountInfo *o_AccountInfo, const MSG_FP_CONNECT_AUTH_USER *i_pMsg);
	int On_MSG_FP_EVENT_NOTIFY_WARP(MSG_FP_EVENT_NOTIFY_WARP *i_pMsg);
	int On_MSG_FP_EVENT_ENTER_SELECT_SCREEN(CP_State *o_pState, const MSG_FP_EVENT_ENTER_SELECT_SCREEN *i_pMsg);
	int On_MSG_FP_EVENT_GAME_STARTED(CP_State *o_pState, const MSG_FP_EVENT_GAME_STARTED *i_pMsg);
	int On_MSG_FP_EVENT_MAP_CHANGED(CP_State *o_pState, const MSG_FP_EVENT_MAP_CHANGED *i_pMsg);
	int On_MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE(const MSG_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE *i_pMsg);

	void DBG_PrintAccountMap();

	BOOL InitDBTable(CODBCStatement *i_pODBCStmt);

	BOOL LoadSystemEvent(CODBCStatement *i_pODBCStmt);

	const vector<CServerGroup>& GetAllServerGroupVector() const { return m_AllServerGroupVector; }
	CServerGroup * GetServerGroup(const char * szServerGruopName);
	void ResetServerGroup(const char *i_szServerGroupName, BOOL i_bExcludeIMServer=FALSE);
	void SendServerInfoForAdmin(CPreIOCPSocket *i_pSocket);
	BOOL SendErrorToFieldServer(char *i_szServerGroupName, SERVER_ID i_FieldServerid, MessageType_t msgType, Err_t err, int errParam1=0, int errParam2=0, char* errMsg=NULL, BOOL bCloseConnection=FALSE);

	void SendMessageToAllFieldServer(BYTE *i_pBuff, int i_nSize);
	void SendMsgToAllIMServer(BYTE *i_pBuff, int i_nSize);

	template<template<MessageType_t> typename data_t, MessageType_t msgtype>
	void SendMessageToAllFieldServer(data_t<msgtype>& data) { SendMessageToAllFieldServer(PBYTE(&data), data.size()); }
	template<template<MessageType_t> typename data_t, MessageType_t msgtype>
	void SendMsgToAllIMServer(data_t<msgtype>& data) { SendMsgToAllIMServer(PBYTE(&data), data.size()); }

	void SendMessageToAllFieldServer(MessageType_t msgtype) { SendMessageToAllFieldServer(PBYTE(&msgtype), sizeof(msgtype)); }
	void SendMsgToAllIMServer(MessageType_t msgtype) { SendMsgToAllIMServer(PBYTE(&msgtype), sizeof(msgtype)); }

	void OnDoMinutelyWorkIOCP(ATUM_DATE_TIME *pDateTime);

	void CheckOnlyLauncherLoginedList(ATUM_DATE_TIME *pDateTime);

	void StoredLogTotalUser();

	CPreIOCPSocket* GetPreIOCPSocket(int idx) { return (CPreIOCPSocket*)GetIOCPSocket(idx); }

	//BOOL PreGetVersionInfoFromFTP(ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion);
	//BOOL PreGetVersionInfoByHTTP(ez_map<VersionInfo, VersionInfo> *o_pMapVersionOld2New, VersionInfo *o_pLauncherVersion, VersionInfo *o_pDeleteFileListVersion, VersionInfo *o_pNoticeVersion);
	CString GetOnlyPathWithoutFileName(CString & i_szFilePath);

	BOOL IOCPInit() override;
	void IOCPClean() override;
	SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude) override;

private:
	mtmapAccountInfoPtr		m_AccountInfoMap;
	CAtumPreDBManager		*m_pAtumDBManager;
	CPreTickManager			*m_pPreTickManager;	
	VersionInfo				m_LatestClientVersion;

	BOOL					m_bPauseService;				// ��� ���񽺸� �ߴ��� �� ���(��: version reloading...)
	mt_set<string>			m_setBlockedAccounts;			// ��� ���� ����
	mtmapVersionInfo		m_mtmapVersionOld2New;			// OldVersion -> NewVersion
	VersionInfo				m_LatestLauncherVersion;		// launcher ����
	VersionInfo				m_LatestDeleteFileListVersion;	// delete file list ����
	VersionInfo				m_LatestNoticeVersion;			// notice.txt ����

	// 2007-01-09 by cmkwon, ���� Reload�� ���� �ӽ� ������
	//int						m_tmTotalNumOfClientVersions;	
	mtmapVersionInfo		m_tmmtmapVersionOld2New;
	VersionInfo				m_tmLatestLauncherVersion;
	VersionInfo				m_tmLatestDeleteFileListVersion;
	VersionInfo				m_tmLatestNoticeVersion;

	CAccountBlockManager	*m_pAccountBlockManager;		// ���� �� ������

	// ���� �׷� ����
	vector<CServerGroup>	m_AllServerGroupVector;

	///////////////////////////////////////////////////////////////////////////////
	// 2006-04-27 by cmkwon
	CODBCStatement			m_odbcExtAuth;					// 2006-04-27 by cmkwon

	///////////////////////////////////////////////////////////////////////////////
	// PreServer ���� �� �ڵ� ���� ������Ʈ ����	
	char					m_szDownloadHttpServerIP[SIZE_MAX_FTP_URL];

	// 2011-12-21 by hskim, EP4 [������ 1ȸ ���]
	ATUM_DATE_TIME			m_OpeningMovieStandard;			// ������ ��� ���� ���� �ð�
};
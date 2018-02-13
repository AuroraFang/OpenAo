#pragma once

#include "GlobalGameServer.h"
#include "AtumProtocol.h"			// 2007-05-02 by cmkwon

using vectSGAME_SERVER_GROUP = vector<SGAME_SERVER_GROUP>;		// 2007-05-02 by cmkwon

class CPreIOCP;
class CPreIOCPSocket;
class CPreGlobal;
class CServerGroup;

extern CPreGlobal* g_pPreGlobal;

class CPreGlobal : public CGlobalGameServer
{
	friend class CPreIOCP;

public:
	CPreGlobal();
	~CPreGlobal();


	void Reset();

	VersionInfo GetClientReinstallVersion() const { return m_ClientReinstallVersion; }

	bool GetRandomFieldSelect() const { return m_bRandomFieldSelect; }

	const char* GetDownloadServerIP() const { return GetRandomDownloadHttpIP(); }
	int GetDownloadServerPort() const { return m_nDownloadHttpServerPort; }

	const char*	GetClientUpdateDownloadDir() const { return m_szClientHttpUpdateDownloadDir; }
	const char*	GetLauncherFileDownloadPath() const { return m_szLauncherFileDownloadHttpPath; }
	const char*	GetDeleteFileListDownloadPath() const { return m_szDeleteFileListDownloadHttpPath; }
	const char*	GetNoticeFileDownloadPath() const { return m_szNoticeFileDownloadHttpPath; }

	void AddDownloadHttpIP(const char * i_szIP);
	const char* GetRandomDownloadHttpIP() const;



	BOOL CheckAllowedList(CServerGroup *i_pGroupInfo, char * i_szAccountName, char * i_szPrivateIP, USHORT i_nAccountType);
	void PrintAllowedList() const;
	BOOL CheckAllowedIP(const char *i_szPublicIP) const;		// 2007-10-19 by cmkwon, AllowedIP �ý��� ���� - �㰡 IP üũ �Լ� 

	CServerGroup* GetServerGroupForLoading(const char * szServerGruopName);
	int GetEnableServerGroupCount();	// 2008-02-22 by cmkwon, ServerPreServer->MasangPreServer �� ���� ���� ���� �ý��� �߰� - 

	// 2007-05-02 by cmkwon
	// 2007-09-05 by cmkwon, EXE_1�� �α��� ���� ���� �������̽� ���� - �����߰�(, USHORT i_usTab8OrderIndex), ���� ����(, USHORT i_usPreServerPort, ��Ʈ�� �⺻ ��Ʈ ���)
	bool InsertGameServerGroup(char *i_szGameServerGroupName, char *i_szPreServerIP, int i_nTab8OrderIndex);
	bool Make_MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST(MSG_PC_CONNECT_GET_GAME_SERVER_GROUP_LIST_OK *o_pGameServerGroupList);
	bool Make_MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK(MSG_PC_CONNECT_GET_NEW_GAME_SERVER_GROUP_LIST_OK *o_pGameServerGroupList);	// 2007-09-05 by cmkwon, EXE_1�� �α��� ���� ���� �������̽� ���� - �Ź��� �Լ� �߰�

	// 2007-07-06 by cmkwon, OnlyChoiceServer �÷��� ���� �߰�
	bool GetOnlyChoiceServer() const { return m_bIsOnlyChoiceServer; }
	const char* GetGamePublisherName() const { return m_szGamePublisherName; }		// 2010-06-01 by shcho, GLogDB ���� -

	// virtual Function
	bool InitServerSocket() override;				// ���ϰ��� �ʱ�ȭ �Լ�
	bool EndServerSocket() override;					// ���ϰ��� ���� �Լ�

	void VMemAlloc() override;

	bool LoadConfiguration();				// Server ������ ���� Configuration Load

protected:
	VersionInfo				m_ClientReinstallVersion;		// re-install version
	vector<VersionInfo>		m_vectorCriticalUpdateVersion;	// critical update version list
	vector<string>			m_AllowedIPList;
	set<string>				m_AllowedAccounts;
	BOOL					m_bRandomFieldSelect;			// Pre Server���� ���ʷ� �����ؾ� �� Field Server�� IP�� ���� �� Random���� �������� ���� ����
	vector<CServerGroup>	m_AllServerGroupVectorForLoading;	// �� �ε��� �Ŀ��� �ٷ� ������

	///////////////////////////////////////////////////////////////////////////////	
	// 2007-05-02 by cmkwon, PreServer ������ ���
	vectSGAME_SERVER_GROUP	m_vectGameServerGroupList;

	// 2007-07-06 by cmkwon, OnlyChoiceServer �÷��� ���� �߰�
	BOOL					m_bIsOnlyChoiceServer;				// 2007-07-06 by cmkwon, OnlyChoiceServer �÷��� ���� �߰�

	char					m_szGamePublisherName[SIZE_MAX_GAME_PUBLISHER_NAME];		// 2010-06-01 by shcho, GLogDB ���� -

public:
	BOOL					m_bIgnoreClientVersionUpdate;
	int						m_nServerGroupLimiteUserCount;		// 2006-10-11 by cmkwon

	char					m_szLauncherFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];	// 
	char					m_szDeleteFileListDownloadPath[SIZE_MAX_FTP_FILE_PATH];	// DELFILELIST_FILE_NAME
	char					m_szNoticeFileDownloadPath[SIZE_MAX_FTP_FILE_PATH];		// NOTICE_FILE_NAME


	vector<string>			m_vectDownloadHttpServer;
	USHORT					m_nDownloadHttpServerPort;

	char					m_szClientHttpUpdateDownloadDir[SIZE_MAX_FTP_FILE_PATH];
	char					m_szLauncherFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char					m_szDeleteFileListDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char					m_szNoticeFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
	char					m_szVersionListFileDownloadHttpPath[SIZE_MAX_FTP_FILE_PATH];
};

namespace PreGlobal
{	
	inline bool CheckAllowedIP(const char* ip) { return g_pPreGlobal->CheckAllowedToolIP(ip); }

	bool Initialize(HINSTANCE hInstance, UINT IDI_Icon, int nCmdShow);

	void Destroy();

	bool Create(HWND hWnd);

	void DestroySocket();

	void OnTimerTraffic();
	void OnTimerPingAlive();
}
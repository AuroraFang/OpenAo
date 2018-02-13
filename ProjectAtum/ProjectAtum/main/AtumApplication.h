#pragma once

#include "D3DApp.h"
#include "IMEKey.h"		// imm32.lib
//#include "INFGameMainQSlot.h"
// 2008-03-19 by bhsohn Ep3�ɼ�â
#include "INFOptionSystem.h"

#include "CInput.h"

//#include "sha256.h"

class CFrustum;
class CD3DHanFont;
class CKeyBoardInput;
class CCamera;
class CShuttleRender;
class CShuttleChild;
class CCharacterChild;
class CTraceRender;
class IMEKey;
class CChat;
class CFieldWinSocket;
class CIMSocket;
// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
class CIMSocketManager;
class CFieldWinSocketManager;
class CArenaManager;

class CGameData;
class CEffectRender;
class CAtumNode;
class CInterface;
class CAbuseFilter;
class CAtumSound;
class CSceneData;
class CAtumDatabase;
class CINFStageEffect;
class CINFGameMain;
class CStoreData;
class CQuestData;
class CBackground;
class CINFSelect;
class CINFChannelList;
class CUnitRender;
class CCharacterRender;						// 2005-07-21 by ispark
class CFxSystem;
class CTutorialSystem;
class CInputManager;
class CGlobalImage;
class CUnitData;
class CSinglePlayGame;	// 2008-08-11 by bhsohn �ܵ����� ���� �ǰԲ�
// 2008-11-13 by bhsohn ���̽�ƽ �۾�
class CJoyStickInput;
class CJoyStickFreedback;
// end 2008-11-13 by bhsohn ���̽�ƽ �۾�

class CWorldRankManager;		// 2009-02-13 by bhsohn ���� ��ŷ �ý���

// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
class CInfinityManager;
// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
class CSkinnedMesh;
class CMeshInitThread;
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

// 2010-06-15 by shcho&hslee ��ý���
class CPetManager;
// end 2010-06-15 by shcho&hslee ��ý���

class CINFEnemyItemInfo;	// 2012-06-14 by isshin ���۹̸�����

class CMapLoad;    // 2010. 10. 05 by jskim �ʷε� ���� ����

// 2011. 10. 10 by jskim UI�ý��� ����
class CINFGroupManager;
class CINFImageList;
// end 2011. 10. 10 by jskim UI�ý��� ����
class CVTCThread;			// 2011. 1. 18 by jskim ��Ʈ�� VTC����

class CScreenShotManager;	// 2015-07-18 Future, Screen Shot Manager

typedef struct
{
	INT			ItemNum;						// ������ ������ȣ, ���� �������� �� (ITEM_BASE*)
	BYTE		byItemKind;				// ������ ����(�����, ��, ����, ��ų.....)
	UID64_t		nUniqueNumber;			// ������ ������ȣ
	INT			nAmount;				// ������ ����
	UINT		nBuildingIndex;			// �ǹ� �ε���
	char		szIconName[20];			// ������ ID
	char		szName[50];				// ������ �̸�
	UINT		uSellingPrice;			// �Ǹ� ���� 
	INT			SourceIndex;			// ����Ʈ, ������(��/����) ���ҽ� ����Ÿ
	BYTE		bySelectType;
	POINT		ptIcon;
} stMultiSelectItem;

// 2007-11-20 by bhsohn �ʷε� �ϴ� ��� ����
// ���� ���� �� ����ü
typedef struct
{
	AVECTOR3		vPos;
	BitFlag16_t		MapWeather;
} stGameStartInfo;

// 2007-12-21 by bhsohn ���α׷� ���� �� ���� ����
// ������ ���
#define		MULTI_LOCK_ALL_MATCHING		0		//	��� ��Ī �Ǿ����� Ǯ����.
#define		MULTI_LOCK_ONE_MATCHING		1		//	�ϳ��� ��Ī�ǵ� Ǯ����.

// ���� �����ϱ� ���� �⺻ ����
typedef struct
{
	MessageType_t			nLockMsg;
	vector<MessageType_t>	vecUnLockMsg;
	int						nLockType;
	BOOL					bFiledServer;
} stMultiLockInfo;
// end 2007-12-21 by bhsohn ���α׷� ���� �� ���� ����

// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
// ���� ��ŸƮ �Ϸ�� ó�� �Ǵ� ����ü
typedef struct
{
	char			ServerGroupName0[SIZE_MAX_SERVER_NAME];			// 2007-04-09 by cmkwon
	char			MainORTestServerName[SIZE_MAX_SERVER_NAME];		// 2007-04-09 by cmkwon
	char			GamePublisher[SIZE_MAX_GAME_PUBLISHER_NAME];	// 2007-04-09 by cmkwon

	MAP_CHANNEL_INDEX	MapChannelIndex;		// ĳ���Ͱ� ���� �� �� ä��
	BYTE				CharacterMode0;				// 

	AVECTOR3			PositionVector;
	BitFlag16_t			MapWeather;
	BOOL				bMemberPCBang;			// ���� PC�� �÷���
}structGameStartOkInfo;
// end 2007-11-22 by bhsohn �Ʒ��� ���ռ���

// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
struct structLoadingGameData
{
	structLoadingGameData() : LoadingPriority { _NOTHING_PRIORITY } { }

	CGameData*		pGameData;
	int				MeshIndex;
	int				MeshType;
	int				Step;					// �ε� ����(1, 6)
	int				Text_Cnt;				// �ؽ��� �ε� ���� ��
	int				LoadingPriority;
	CSkinnedMesh*	pSkinnedMesh;
};
struct structLoadingGameInfo
{
	structLoadingGameInfo() : LoadingPriority { _NOTHING_PRIORITY } { }
	char			MeshName[16];
	int				MeshType;
	CSkinnedMesh*	pSkinnedMesh;
	int				LoadingPriority;
};
// 2011. 1. 18 by jskim ��Ʈ�� VTC����
typedef bool(_stdcall *func1) ();
typedef bool(_stdcall *func2) ();
typedef bool(_stdcall *func3) (char*);
typedef void(_stdcall *func4) ();
// end 2011. 1. 18 by jskim ��Ʈ�� VTC����

// 2007-07-10 by bhsohn ��� Ÿ�� ������ ���� ó��
#define SIZE_MAX_ERROR_CHAT_MESSAGE			512

// 2008-11-13 by bhsohn ���̽�ƽ �۾�
#define MAX_JOYCTL_LIST			10			// MAX_JOY_LIST�� ������ ���̾���Ѵ�.


#define USE_GAMEGUARD_NONE					0		//���Ӱ��� ������
#define USE_GAMEGUARD_AHNLAB_HACKSHIELD		1		//�ٽ��� ���Ӱ���

#define GAMEGUARD_TYPE	USE_GAMEGUARD_NONE


class CAtumApplication : public CD3DApplication
{
	friend class CShuttleChild;
public:
	CAtumApplication();
	virtual ~CAtumApplication();

	HRESULT InitDShowTextureRenderer();
	HRESULT MovieRender();

	HRESULT CheckMovieStatus();
	void CleanupDShow();


	HRESULT AddToROT(IUnknown *pUnkGraph);
	void RemoveFromROT();

	D3DSURFACE_DESC & GetBackBufferDesc() { return m_d3dsdBackBuffer; }
	FLOAT 	GetFPS();
	FLOAT	GetElapsedTime() const { return m_fElapsedTime; }
	HWND	GetHwnd() const { return m_hWnd; }
	D3DCAPS9 & GetDeviceCaps() { return m_d3dCaps; }
	void	SendMovePacket(BOOL bActive);	// 2004-11-08 by jschoi
	HRESULT	InitStageEffect(int nIndex, float fPlayTime = 5.0f, BOOL bEnableSkip = TRUE);
	DWORD	ResourceLoadThread();	// load thread function(CInitThread�� �Բ� ����Ѵ�.)
	DWORD	ResourceRestoreThread();
	void	RenderGame();
	void	CleanText();
	void	CheckError(DWORD err);
	void	LoadPath(char* strPath, UINT nType, char * strFileName);
	void	CalcObjectSourceScreenCoords(D3DXVECTOR3 vObjPos, int iScreenWidth, int iScreenHeight, int &iCoordX, int &iCoordY, int &iCoordW);
	void	CheckRenderState();
	void	SetCamPosInit();
	FLOAT	Distance3DTo2D(D3DXVECTOR3 vPos1, D3DXVECTOR3 vPos2);
	void	ChangeEffectTexture(CAtumNode * pNode, char * str);

	void	ChangeGameState(DWORD dwType, BOOL bUseThread = FALSE);
	void	OnScreenShotCreated(char* screenShotName);
	void	DeleteEffectList();
	void	InitSpeedHackTimer();
	BOOL	IsWindowMode() const { return m_bWindowed; }

	DWORD	GetGameState();

	HRESULT OneTimeSceneInit() override;
	HRESULT InitDeviceObjects() override;
	HRESULT RestoreDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;
	HRESULT Render() override;
	HRESULT FrameMove() override;
	HRESULT InActiveFrameMove() override;										// 2006-07-06 by ispark
	HRESULT FinalCleanup() override;
	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	void	NetworkErrorMsgBox(char* strMsg);

	void	RenderCity();
	void	SendFieldSocketConnectLogin(CFieldWinSocket* pFieldSocket);
	void	SendFieldSocketChangeCharacterBodyCondition(BodyCond_t hySingleBodyCondition, BOOL bSet);//FieldSocketCharacterChangeCurrentHPDPSPEP,FieldSocketCharacterChangeCurrentHP
	void	SendFieldSocketCityGetBuildingList(MAP_CHANNEL_INDEX mapChannelIndex);
	void	SendFieldSocketGetShopWarpTargetMapList(MAP_CHANNEL_INDEX mapChannelIndex);
	void	SendArenaLoginOk();
	void	SendAreneEnterRoomWarp();
	void	SendFCArenaCharacterGameStart();					// ���μ������� �Ʒ����� �ε��
	void	SendFCCharacterGameStartFromArenaToMainServer();	// �Ʒ��� ���ҽ� �ε�Ϸ� 
	int		NextStepArenaState();		// �Ʒ��� ���°� ������Ʈ 
	void	ConevertArenaRenderUserName(int nArenaState, char* pSrcDst);
	CHARACTER* GetMFSMyShuttleInfo();	//MainServer ������ ���´�.
	void	SetMFSMyShuttleInfo(CHARACTER *pMyShuttleInfo);//MainServer ������ �����Ѵ�. 

	// 2007-02-12 by bhsohn Item ���� ���� ó��
	void	DelMultiItemList(BOOL bInitCtlBtn = FALSE);
	BOOL	CheckMultItemSel(UID64_t	 nUniqueNumber);
	BOOL	CheckCityStoreMultItemSel(UID64_t	 nUniqueNumber); // â���� ������ ���� ���� �Ǵ�.��
	int		GetMultiSelectItem();
	int		GetCityStoreMultiSelectItem();
	void	GetMultiItemSPI(int nShopId, char* pDstSpi);

	// 2007-03-02 by bhsohn ���� ���� �߰� ����
	deque<stMultiSelectItem>* GetMultiSelVector(BYTE byType);

	// by bhsohn 2007-03-12 China Working
	void DxSetDialogBoxMode(BOOL bEnable);

	// 2007-04-05 by bhsohn �ʷε��, üũ�� �߰�
	MAP_CHANNEL_INDEX GetMyShuttleMapChannelIndex();

	// 2007.04.24 by bhsohn China IME Working
	void	UpdateReadingString(char* pText, int nTextLen);
	void	UpdateIMEType(char* pText, int nTextLen);

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	bool	IsMyShuttleCharacter() const { return m_bCharacter; }
	// 2007-05-21 by bhsohn China IME Working
	void	RenderIMEType(int x, int y);

	// 2007-07-11 by bhsohn omi üũ�� �߰�
	void	SendOMICheckSum();

	// 2010. 03. 12 by ckPark Interface.tex üũ�� �߰�
	void	SendInterfaceCheckSum();
	// end 2010. 03. 12 by ckPark Interface.tex üũ�� �߰�

	void	StartNarrationSound(char* pNarrationSound, char* pErNarrationSound = nullptr);
	void	EndNarrationSound();
	void	SetFirstUserIntoFreeSka(BOOL bTake);
	BOOL	GetFirstUserIntoFreeSka();

	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	BOOL	SendMeshObjectCheckSum(int nMeshIndex);

	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	void	ReLoadEnemyRenderList();

	// 2007-09-06 by bhsohn �����̾� ������ �Ѿ˼� 2��
	BOOL	IsPrimiumUser();

	// 2007-09-05 by bhsohn ���� ������
	void	EnableChatMode(BOOL bChat, BOOL bUpdatIMEDevice);
	BOOL	IsMyShuttleLeader();
	BOOL	IsMyShuttleGuildMaster();
	BOOL	IsLeaderOutPost();

	// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
	BYTE	IsUseSecondaryPasswordSystem();
	BYTE	IsSetSecondaryPassword();
	void	EnterLock(BOOL bUnLock);
	BOOL	GetEnterLock();
	void	SendDeleteCharcter(char* pEncordPassWord);

	// 2008-07-07 by bhsohn 2�� �н����� Ȱ��ȭ��, �κ� �ȿ����� ��
	BYTE	IsSecondPassLock();
	void	SetSecondPassLock(BYTE bySecondPassLock);

	// 2007-12-21 by bhsohn ���α׷� ���� �� ���� ����
	void	EnterMultiLock(BOOL bFiledServer, MessageType_t nLockMsg, vector<MessageType_t> vecUnLockMsg, int nLockType);
	void	CheckMultiLock(BOOL bFiledServer, MessageType_t msgUnLockMsg);
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���

	// �ش� �޽��� Lock�� ���� Ȯ��.
	bool	IsEnableMultiLock(MessageType_t nLockMsg);

	void	OnClickArenaShop();
	void	RequestArenaTeamList();
	void	SetArenaState(int nState);
	int		GetArenaState();
	void	ArenaFinishWarp(BOOL bForceCloseArenaSocket);	// �Ʒ��� ���� â�� ���� Ȯ�� 
	void	ArenaGameStarOk(structGameStartOkInfo *pMsg, BOOL bGotoArena);
	void	CloseArenaSocket();
	CHARACTER*	GetArenaCharacterInfo();
	void	StartGmConnectAenenaServer();		// GM�� �Ʒ��� ������ ���� 
	void	GmConnectAenenaIMServer();			// GM�� �Ʒ��� IM������ ���� 
	void	SetGmModeConnect(BOOL bGmModeConnect);
	BOOL	IsGmModeConnect();

	// 2008-03-03 by bhsohn ���� �Ʒ��� IM���� ������� ����
	void	UserConnectAenenaIMServer();


	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	void	OnClickInfinityField();
	void	UserConnectInfinityIMServer();
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���


	// 2008-02-15 by bhsohn ��Ʈ��ũ �ӵ� üũ
	void	StartNetCheck(char* pCmd);
	void	SendCheckNetState();

	// 2008-03-07 by bhsohn ��������, �Ʒ��� �̿� ���ϰ� ����
	BOOL	IsDoingInfluenceWar();

	// 2008-03-17 by bhsohn ������ �̵��� Tick�ȵ����� ���� ó��
	BOOL	IsPossibleWindowMove();

	// 2008-03-27 by bhsohn �̼Ǽ���â�� ���ִ� ���¿����� ������ �ȿ����� ����
	BOOL	IsPossibleShopOpen();

	// 2008-04-07 by bhsohn �ý���Ű �ɷ����� �ý��� �߰�
	BOOL	IsSysKeyDownPermisiion(WPARAM wParam, LPARAM lParam);

	// 2008-06-16 by bhsohn �±� ���� ������ �������� �߿�� ��
	BOOL	ExcuteExplore();

	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	void	InitOptionEtcInfo(sOPTION_ETC* pEtc);
	BOOL	IsOptionEtc(int nOptionIdx);
	stcuctRateRect GetOptionInterfacePOS();
	void	SetOptionInterfacePOS(stcuctRateRect* pRect);

	void RenderDbg();

	// 2009. 02. 03 by ckPark ä��â �������̽� ũ�� ����

	stcuctRateRect GetChatInterfacePOS();
	void	SetChatInterfacePOS(stcuctRateRect* pRect);

	// end 2009. 02. 03 by ckPark ä��â �������̽� ũ�� ����


	int		GetFriendListOnCount();

	// 2008-08-11 by bhsohn �ܵ����� ���� �ǰԲ�
	BOOL	IsSingletonMode();
	void	SingPlayGameStartOk(MSG_FC_CHARACTER_GAMESTART_OK * pMsg);

	// 2008-09-17 by bhsohn ���� �������ϸ� üũ�� �߰�
	void	SendAtmCheckSum();

	// 2008-11-11 by bhsohn ����Ʈ CheckSum�߰�
	void	SendQuestCheckSum();

	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	// ��Ʈ�� ���̽�ƽ
	CJoyStickInput *GetJoystickControl();
	BOOL GetAsyncKeyState_DIK_DIJ(int code);
	// ���̽�ƽ ��Ʈ�� ����
	int GetJoystickCtrlDeviceListLen();
	char* GetJoystickCtrlDeviceList(int i_nIdx);

	// �����е�
	CJoyStickFreedback *GetJoyStickFeedback();
	void SetJoyStickFeedback(BOOL bSetFeedBack, int nPowerIndex);

	// ���̽�ƽ �������
	BOOL IsUseJoyStick();
	BOOL IsUseJoyFeedBack();	// ���� ��� ����
	void SetUseJoyStick(BOOL bUse);
	void SetUseJoyFeedBack(BOOL bUse);

	// ���̽�ƽ �ɼ�����
	void InitJoyStickOptionInfo(structJoyStickOptionInfo* pJoysticOp);

	// ���̽�ƽ ��ġ �ʱ�ȭ
	int InitJoyStickDevice(BOOL bJoyControl, BOOL bJoyFeed, char* o_pDevice);
	// end 2008-11-13 by bhsohn ���̽�ƽ �۾�

	// 2008-12-29 by bhsohn ��Ű �ӽ� ���� ����Ʈ �߰�
	void CreateMyAppEffectData(int i_nType, D3DXVECTOR3	i_vPos, D3DXVECTOR3	i_vVel, D3DXVECTOR3	i_vUp); // �� ��ġ�� ����Ʈ ����
	void SendCharcterShowMapEffect(MSG_FC_CHARACTER_SHOW_MAP_EFFECT* i_pMsg);
	D3DXVECTOR3 GetMyShuttleCharacterPos();
	D3DXVECTOR3 GetViewDirVel();
	// end 2008-12-29 by bhsohn ��Ű �ӽ� ���� ����Ʈ �߰�

	// 2009-01-28 by bhsohn nProtector ���� ���� �߰�
	BOOL InitGameGuardNProtector();
	BOOL SetGameGuardHWND();
	BOOL TickGetCheckGameGuardNProtector();
	void	HS_SendHackShieldErrorMsg(long lHackClientCode, char * pMsg = nullptr);
	// end 2009-01-28 by bhsohn nProtector ���� ���� �߰�
	// 2009. 09. 21 by jskim ���Ӱ��� XTRAP ����(��Ʈ��)
	void TickGetCheckGameGuardXTrap();
	//end 2009. 09. 21 by jskim ���Ӱ��� XTRAP ����(��Ʈ��)

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	CWorldRankManager*	GetWorldRankManager();
	void RqAllWorldRankInfo();
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

	// 2009-02-26 by bhsohn Japan IME
	void CleanIMEControl();
	// end 2009-02-26 by bhsohn Japan IME


	// 2009-04-08 by bhsohn ���� ���ý�, ����ũ �� ��æƮ �� ������ �ǸŽ� ��� �޽��� ��� ��ȹ ����(K0000174)
	void AddMultiSelWarningMsg(UID64_t	UniqueNumber);
	// end 2009-04-08 by bhsohn ���� ���ý�, ����ũ �� ��æƮ �� ������ �ǸŽ� ��� �޽��� ��� ��ȹ ����(K0000174)

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	CInfinityManager*	GetInfinityManager();
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	void LoadMeshPorcess();
	BOOL ObjectLoadingTimeChack();
	BOOL IsEmptyLoadingGameDataList();
	structLoadingGameData* vecFrontGameData();
	void DeleteIsLoadingGameData(structLoadingGameData* nGameData);
	void vecPushGameData(structLoadingGameData* i_pGameData);
	BOOL DeleteLoadingGameData(CSkinnedMesh *i_pSkinnedMesh);
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

	void ShotDownGameTime(); // 2012-07-17 by jhahn, �˴ٿ� �ð� ǥ��

	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
	void FieldSocketWarInflConseVictites(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES* pMsg);
	void FieldSocketShopInflBuffOk(MSG_FC_SHOP_INFLUENCE_BUFF_OK* pMsg);
	// END 2013-05-07 by bhsohn ��������Ʈ ������ �ý���

	void SetCharacterChangeCurrentHPAndDamageBodyCondition(float fHP) { CharacterChangeCurrentHPAndDamageBodyCondition(fHP); }			// 2013-06-25 by ssjung ������ ����Ʈ�� �ȳ����� ���� ����

protected:
	// MsgProc~
	int MsgProcGame(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int MsgProcCity(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int MsgProcCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int		OnRecvFieldSocketMessage(DWORD wParam, UINT nSocketNotifyType);

	void	OnRecvFieldSocketWS_CONNECTED(USHORT nConnect, CFieldWinSocket* pFieldSocket);
	void	OnRecvFieldSocketWS_CLOSED();

	void	FieldSocketConnectLoginOk(MSG_FC_CONNECT_LOGIN_OK * pMsg);
	void	FieldSocketCharacterGetCharacterOk(MSG_FC_CHARACTER_GET_CHARACTER_OK * pMsg);
	void	FieldSocketCharacterCreateOk(MSG_FC_CHARACTER_CREATE_OK* pMsg);
	void	FieldSocketCharacterDeleteOk();
	void	FieldSocketCharacterGetOtherInfoOk(MSG_FC_CHARACTER_GET_OTHER_INFO_OK* pMsg);
	void	FieldSocketCharacterGetMonsterInfoOk(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK* pMsg);
	void	FieldSocketCharacterGameStartOk(MSG_FC_CHARACTER_GAMESTART_OK * pMsg);
	void	FieldSocketCharacterConnectGameStartOk(MSG_FC_CHARACTER_CONNECT_GAMESTART_OK * pMsg);
	void	FieldSocketCharacterGameEndOk(UINT nSocketNotifyType, MSG_FC_CHARACTER_GAMEEND_OK * pMsg);
	void	FieldSocketMoveOk(MSG_FC_MOVE_OK* pMsg);
	void	FieldSocketMonsterMoveOk(MSG_FC_MONSTER_MOVE_OK* pMsg);
	void	FieldSocketCharacterChangeHPDPSPEP(MSG_FC_CHARACTER_CHANGE_HPDPSPEP * pMsg);
	void	FieldSocketCharacterChangeCurrentHPDPSPEP(MSG_FC_CHARACTER_CHANGE_CURRENTHPDPSPEP * pMsg);
	void	FieldSocketCharacterChangeCurrentHP(MSG_FC_CHARACTER_CHANGE_CURRENTHP * pMsg);
	void	FieldSocketCharacterChangeCurrentDP(MSG_FC_CHARACTER_CHANGE_CURRENTDP * pMsg);
	void	FieldSocketCharacterChangeCurrentSP(MSG_FC_CHARACTER_CHANGE_CURRENTSP * pMsg);
	void	FieldSocketCharacterChangeCurrentEP(MSG_FC_CHARACTER_CHANGE_CURRENTEP * pMsg);
	void	FieldSocketCharacterChangeBodycondition(MSG_FC_CHARACTER_CHANGE_BODYCONDITION * pMsg);
	void	FieldSocketCharacterChangeUnitkind(MSG_FC_CHARACTER_CHANGE_UNITKIND * pMsg);
	void	FieldSocketCharacterPayWarPoint(MSG_FC_CHARACTER_PAY_WARPOINT* pMsg);
	void	FieldSocketCharacterUpdateSubleaderOk(MSG_FC_CHARACTER_UPDATE_SUBLEADER_OK * pMsg);
	void	FieldSocketPartyCreateOk(MSG_FC_PARTY_CREATE_OK* pMsg);
	void	FieldSocketPartyRequestInviteQuestion(MSG_FC_PARTY_REQUEST_INVITE_QUESTION* pMsg);
	void	FieldSocketPartyRequestInviteQuestion_THE_ARENA(MSG_FC_PARTY_REQUEST_INVITE_QUESTION_THE_ARENA* pMsg);	/// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : ��Ƽ ���� �غ�
	void	FieldSocketPartyAutoCreated_THE_ARENA(MSG_FC_PARTY_AUTO_CREATED_THE_ARENA* pMsg);	/// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : �ڵ����� ��Ƽ�� ������ ��Ŷ.
	void	FieldSocketPartyRejectInviteOk(MSG_FC_PARTY_REJECT_INVITE_OK* pMsg);
	void	FieldSocketPartyPutMember(MSG_FC_PARTY_PUT_MEMBER* pMsg);
	void	FieldSocketPartyUpdateMemberInfoAll(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL* pMsg);
	void	FieldSocketPartyUpdateMemberInfoLevel(MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL* pMsg);
	void	FieldSocketPartyUpdateMemberInfoHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP* pMsg);
	void	FieldSocketPartyUpdateMemberInfoCurrentHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP* pMsg);
	void	FieldSocketPartyUpdateMemberInfoDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP* pMsg);
	void	FieldSocketPartyUpdateMemberInfoCurrentDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP* pMsg);
	void	FieldSocketPartyUpdateMemberInfoSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP* pMsg);
	void	FieldSocketPartyUpdateMemberInfoCurrentSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP* pMsg);
	void	FieldSocketPartyUpdateMemberInfoEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP* pMsg);
	void	FieldSocketPartyUpdateMemberInfoCurrentEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP* pMsg);
	void	FieldSocketPartyUpdateMemberInfoBodyCondition(MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION* pMsg);
	void	FieldSocketCityCheckWarpStateOk(MSG_FC_CITY_CHECK_WARP_STATE_OK* pMsg);
	void	FieldSocketEventWarpOtherFieldServer(MSG_FC_EVENT_WARP * pMsg);
	void	FieldSocketEventWarpSameMap(MSG_FC_EVENT_WARP_SAME_MAP * pMsg);
	void	FieldSocketEventWarpSameFieldServer(MSG_FC_EVENT_WARP_SAME_FIELD_SERVER * pMsg);
	void	FieldSocketEventOtherWarped(MSG_FC_EVENT_OTHER_WARPED * pMsg);
	void	FieldSocketEventWarpConnectOk(MSG_FC_EVENT_WARP_CONNECT_OK* pMsg);
	void	FieldSocketStorePutItemHeader(MSG_FC_STORE_PUT_ITEM_HEADER* pMsg);
	void	FieldSocketStorePutItem(MSG_FC_STORE_PUT_ITEM* pMsg);
	void	FieldSocketStorePutItemDone(MSG_FC_STORE_PUT_ITEM_DONE* pMsg);
	void	FieldSocketCharacterChangeLevel(MSG_FC_CHARACTER_CHANGE_LEVEL* pMsg);
	void	FieldSocketCharacterChangeExp(MSG_FC_CHARACTER_CHANGE_EXP* pMsg);
	void	FieldSocketCharacterChangeStat(MSG_FC_CHARACTER_CHANGE_STAT * pMsg);
	void	FieldSocketCharacterChangeTotalGearStat(MSG_FC_CHARACTER_CHANGE_TOTALGEAR_STAT * pMsg);
	void	FieldSocketCharacterChangeStatus(MSG_FC_CHARACTER_CHANGE_STATUS* pMsg);
	void	FieldSocketItemChangeWindowPositionOk(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK * pMsg);
	void	FieldSocketMoveLandingOk(MSG_FC_MOVE_LANDING_OK* pMsg);
	void	FieldSocketMoveTakeOffOk(MSG_FC_MOVE_TAKEOFF_OK* pMsg);
	void	FieldSocketItemShowItem(MSG_FC_ITEM_SHOW_ITEM* pMsg);
	void	FieldSocketBattleDropMineOk(MSG_FC_BATTLE_DROP_MINE_OK* pMsg);
	void	FieldSocketItemGetItemOk(MSG_FC_ITEM_GET_ITEM_OK* pMsg);
	void	FieldSocketItemHideItem(MSG_FC_ITEM_HIDE_ITEM* pMsg);
	void	FieldSocketPartyPutItemOther(MSG_FC_PARTY_PUT_ITEM_OTHER* pMsg);
	void	FieldSocketItemGetItemFail(MSG_FC_ITEM_GET_ITEM_FAIL* pMsg);
	void	FieldSocketMissileMoveOk(MSG_FC_MISSILE_MOVE_OK* pMsg);
	void	FieldSocketCityRequestEnterBuildingOk(MSG_FC_CITY_REQUEST_ENTER_BUILDING_OK* pMsg);
	void	FieldSocketEventEnterBuilding(MSG_FC_EVENT_ENTER_BUILDING* pMsg);
	void	FieldSocketEventLeaveBuildingOk(MSG_FC_EVENT_LEAVE_BUILDING_OK* pMsg);
	void	FieldSocketShopCheckGiveTargetOK(MSG_FC_SHOP_CHECK_GIVE_TARGET_OK* pMsg);
	void	FieldSocketShopBuyCashItemOK(MSG_FC_SHOP_BUY_CASH_ITEM_OK* pMsg);
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	void	FieldSocketShopBuyItemUpdate(MSG_FC_SHOP_BUY_ITEM_UPDATE* pMsg);
#endif

	void	FieldSocketShopPutItemHeader(MSG_FC_SHOP_PUT_ITEM_HEADER* pMsg);
	void	FieldSocketShopPutItem(MSG_FC_SHOP_PUT_ITEM* pMsg);
	void	FieldSocketShopPutItemDone(MSG_FC_SHOP_PUT_ITEM_DONE* pMsg);
	void	FieldSocketShopGetItemInfoOk(MSG_FC_SHOP_GET_ITEMINFO_OK* pMsg);
	void	FieldSocketShopBuyItemOk(MSG_FC_SHOP_BUY_ITEM_OK* pMsg);
	void	FieldSocketShopSellItemOk(MSG_FC_SHOP_SELL_ITEM_OK* pMsg);
	void	FieldSocketShopGetUsedItemPriceOk(MSG_FC_SHOP_GET_USED_ITEM_PRICE_OK* pMsg);
	void	FieldSocketItemThrowAwayItemOk(MSG_FC_ITEM_THROW_AWAY_ITEM_OK* pMsg);

	void	FieldSocketItemUseEnergyOk(MSG_FC_ITEM_USE_ENERGY_OK* pMsg);
	void	FieldSocketItemUseItemOk(MSG_FC_ITEM_USE_ITEM_OK* pMsg);
	void	FieldSocketItemUseCardItemMonstersummonNotify(MSG_FC_ITEM_USE_CARDITEM_MONSTERSUMMON_NOTIFY* pMsg);
	void	FieldSocketItemUseCardItemChangecharacternameNotify(MSG_FC_ITEM_USE_CARDITEM_CHANGECHARACTERNAME_NOTIFY* pMsg);
	void	FieldSocketItemUseCardItemSkillinitializeNotify(MSG_FC_ITEM_USE_CARDITEM_SKILLINITIALIZE_NOTIFY* pMsg);
	void	FieldSocketItemUseCardItemChangepilotfaceNotify(MSG_FC_ITEM_USE_CARDITEM_CHANGEPILOTFACE_NOTIFY* pMsg);
	void	FieldSocketItemUseCarditemGuildNotify(MSG_FC_ITEM_USE_CARDITEM_GUILD_NOTIFY* pMsg);
	void	FieldSocketItemUseCarditemDeleteAllEnchant(MSG_FC_ITEM_DELETE_ALL_ENCHANT* pMsg);
	void	FieldSocketItemUseCardItemChangeNationConfirmed();	// 2015-12-17 Future, Nation Change Card

	void	FieldSocketChatRegion(MSG_FC_CHAT_REGION* pMsg);
	void    FieldSocketChatCashAll(MSG_FC_CHAT_CASH_ALL* pMsg);
	void	FieldSocketChatMap(MSG_FC_CHAT_MAP* pMsg);
	void	FieldSocketChatArena(MSG_FC_CHAT_ARENA* pMsg);
	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	void	FieldSocketChatInfinity(MSG_FC_CHAT_INFINITY* pMsg);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���


	void	FieldSocketTradeRequestTradeOk(MSG_FC_TRADE_REQUEST_TRADE_OK* pMsg);
	void	FieldSocketTradeAcceptTradeOk(MSG_FC_TRADE_ACCEPT_TRADE_OK* pMsg);
	void	FieldSocketTradeRejectTradeOk(MSG_FC_TRADE_REJECT_TRADE_OK* pMsg);
	void	FieldSocketTradeRejectTrading(MSG_FC_TRADE_REJECT_TRADING* pMsg);
	void	FieldSocketTradeShowTradeWindow(MSG_FC_TRADE_SHOW_TRADE_WINDOW* pMsg);
	void	FieldSocketTradeOkTradeOk(MSG_FC_TRADE_OK_TRADE_OK* pMsg);
	void	FieldSocketTradeCancelTradeOk(MSG_FC_TRADE_CANCEL_TRADE_OK* pMsg);
	void	FieldSocketTradeDeleteItem(MSG_FC_TRADE_DELETE_ITEM* pMsg);
public:
	void	FieldSocketTradeUpdateItemCount(MSG_FC_TRADE_UPDATE_ITEM_COUNT* pMsg);
	void	FieldSocketTradeInsertItem(MSG_FC_TRADE_INSERT_ITEM* pMsg);
protected:
	void	FieldSocketTradeTransItemOk(MSG_FC_TRADE_TRANS_ITEM_OK* pMsg);
	void	FieldSocketStoreUpdateEndurance(MSG_FC_STORE_UPDATE_ENDURANCE* pMsg);
	void	FieldSocketTradeSeeItemOk(MSG_FC_TRADE_SEE_ITEM_OK* pMsg);
	// 2010-06-15 by shcho&hslee ��ý��� - �ŷ� �� �� ���� ����
	void	FieldSocketTradeSeePetData(MSG_TRADE_PET_DATA* pMsg);
	void	FieldSocketTradeDeletePetData(MSG_TRADE_PET_DATA* pMsg); // �ŷ��� �� ���� ���� ó��
	void	FieldSocketTradeInsertPetData(MSG_FC_ITEM_PET_BASEDATA* pMsg); // �ŷ��� �� ���� �߰� ó��
	// end 2010-06-15 by shcho&hslee ��ý��� - �ŷ� �� �� ���� ����
	void	FieldSocketCitywarGetOccupyinfoOk(MSG_FC_CITYWAR_GET_OCCUPYINFO_OK* pMsg);
	void	FieldSocketCitywarBringSumoftexOk(MSG_FC_CITYWAR_BRING_SUMOFTEX_OK* pMsg);
	void	FieldSocketCitywarStartWar(MSG_FC_CITYWAR_START_WAR* pMsg);
	void	FieldSocketCitywarEndWar(MSG_FC_CITYWAR_END_WAR* pMsg);
	void	FieldSocketCharacterGetCashMoneyCountOk(MSG_FC_CHARACTER_GET_CASH_MONEY_COUNT_OK* pMsg);
	void	FieldSocketCharacterChangeCharacterModeOk(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK* pMsg);
	void	FieldSocketSkillUseSkill(MSG_FC_SKILL_USE_SKILL_OK *pMsg);
	void	FieldSocketInvenItempHidePetData(MSG_FC_ITEM_NOTIFY_WINDOW_POSITION* pMsg);	   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	void	FieldSocketSkillCancelSkillOK(MSG_FC_SKILL_CANCEL_SKILL_OK * pMsg);
	void	FieldSocketInvalidateSkill(MSG_FC_SKILL_INVALIDATE_SKILL *pMsg);
	void	FieldSocketCharacterGetAccountUniqueNumberOk(MSG_FC_CHARACTER_GET_ACCOUNTUNIQUENUMBER_OK* pMsg);
	void	FieldSocketBattleRequestPkOk(MSG_FC_BATTLE_REQUEST_PK_OK* pMsg);
	void	FieldSocketBattleCancelPK();
	void	FieldSocketBattleDropBundleOk(MSG_FC_BATTLE_DROP_BUNDLE_OK* pMsg);
	void	FieldSocketCharacterDeleteOtherInfo(MSG_FC_CHARACTER_DELETE_OTHER_INFO* pMsg);
	void	FieldSocketCharacterMonsterCreated(MSG_FC_MONSTER_CREATED* pMsg);
	void	FieldSocketMonsterChangeBodycondition(MSG_FC_MONSTER_CHANGE_BODYCONDITION* pMsg);
	void	FieldSocketCharacterChangeMonsterHP(MSG_FC_MONSTER_CHANGE_HP* pMsg);
	void	FieldSocketCharacterShowEffectOk(MSG_FC_CHARACTER_SHOW_EFFECT_OK* pMsg);
	void	FieldSocketBattleRequestP2PPkOk(MSG_FC_BATTLE_REQUEST_P2P_PK_OK* pMsg);
	void	FieldSocketBattleAcceptRequestP2PPkOk(MSG_FC_BATTLE_ACCEPT_REQUEST_P2P_PK_OK* pMsg);
	void	FieldSocketBattleRejectRequestP2PPkOk(MSG_FC_BATTLE_REJECT_REQUEST_P2P_PK_OK* pMsg);
	void	FieldSocketBattleSurrenderP2PPkOk(MSG_FC_BATTLE_SURRENDER_P2P_PK_OK* pMsg);
	void	FieldSocketCharacterChangeCharacterMent(MSG_FC_CHARACTER_CHANGE_CHARACTER_MENT* pMsg);
	void	FieldSocketBattleRejectSurrenderP2PPkOk(MSG_FC_BATTLE_REJECT_SURRENDER_P2P_PK_OK* pMsg);
	void	FieldSocketBattleEndP2PPk(MSG_FC_BATTLE_END_P2P_PK* pMsg);
	void	FieldSocketCharacterChangePKPoint(MSG_FC_CHARACTER_CHANGE_PKPOINT* pMsg);
	void	FieldSocketBattleDropDummyOk(MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg);
	void	FieldSocketBattleDeleteDummyOK(MSG_FC_BATTLE_DELETE_DUMMY_OK* pMsg);
	void	FieldSocketBattleExplodeDummyOK(MSG_FC_BATTLE_EXPLODE_DUMMY_OK* pMsg);
	void	FieldSocketBattleAttackHideItem(MSG_FC_BATTLE_ATTACK_HIDE_ITEM* pMsg);
	void	FieldSocketBattleAttackExplodeItem(MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM* pMsg);
	void	FieldSocketMonsterHide(MSG_FC_MONSTER_HIDE* pMsg);
	void	FieldSocketMonsterShow(MSG_FC_MONSTER_SHOW* pMsg);
	void	FieldSocketBattleDropFixerOk(MSG_FC_BATTLE_DROP_FIXER_OK* pMsg);
	void	FieldSocketInfoGetItemInfoOk(ITEM & item);
	void	FieldSocketInfoGetRareItemInfoOk(RARE_ITEM_INFO & rareItem);
	void	FieldSocketInfoGetMonsterInfoOk(MSG_FC_INFO_GET_MONSTER_INFO_OK *mon);
	void	FieldSocketInfoGetMapObjectInfoOk(MAPOBJECTINFO & obj);
	void	FieldSocketBattleAttackHideItemWKind(MSG_FC_BATTLE_ATTACK_HIDE_ITEM_W_KIND* pMsg);
	void	FieldSocketBattleAttackExplodeItemWKind(MSG_FC_BATTLE_ATTACK_EXPLODE_ITEM_W_KIND* pMsg);
	void	FieldSocketConnectSyncTime(MSG_FC_CONNECT_SYNC_TIME* pMsg);
	void	FieldSocketItemReloadItemInfo();
	void	FieldSocketItemUseEnchantOk(MSG_FC_ITEM_USE_ENCHANT_OK* pMsg);
	void	FieldSocketItemPutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg);
	void	FieldSocketQuestRequestStartResult(MSG_FC_QUEST_REQUEST_START_RESULT* pMsg);
	void	FieldSocketQuestRequestSuccessResult(MSG_FC_QUEST_REQUEST_SUCCESS_RESULT* pMsg);
	void	FieldSocketQuestRequestSuccessCheckResult(MSG_FC_QUEST_REQUEST_SUCCESS_CHECK_RESULT* pMsg);
	void	FieldSocketQuestPutAllQuest(MSG_FC_QUEST_PUT_ALL_QUEST* pMsg);
	void	FieldSocketSkillSetupSkillOk(MSG_FC_SKILL_SETUP_SKILL_OK* pMsg);
	void	FieldSocketRequestRequestOk(MSG_FC_REQUEST_REQUEST_OK* pMsg);
	void	FieldSocketRequestRejectRequestOk(MSG_FC_REQUEST_REJECT_REQUEST_OK* pMsg);
	void	FieldSocketRequestCancelRequest(MSG_FC_REQUEST_CANCEL_REQUEST* pMsg);
	void	FieldSocketRequestAcceptRequestOk(MSG_FC_REQUEST_ACCEPT_REQUEST_OK* pMsg);
	void	FieldSocketPartyBattleStart(MSG_FC_PARTY_BATTLE_START* pMsg);
	void	FieldSocketPartyBattleEnd(MSG_FC_PARTY_BATTLE_END* pMsg);
	void	FieldSocketQuestUpdateMonsterCount(MSG_FC_QUEST_UPDATE_MONSTER_COUNT* pMsg);
	void	FieldSocketQuestPutAllQuestMonsterCount(MSG_FC_QUEST_PUT_ALL_QUEST_MONSTER_COUNT* pMsg);
	void	FieldSocketItemResetSkillEnchantToLoadedItem();
	void	FieldSocketBattleToggleShieldResult(MSG_FC_BATTLE_TOGGLE_SHIELD_RESULT* pMsg);
	void	FieldSocketBattleToggleDecoyOk(MSG_FC_BATTLE_TOGGLE_DECOY_OK* pMsg);
	void	FieldSocketBattlePriBulletReloaded(MSG_FC_BATTLE_PRI_BULLET_RELOADED* pMsg);
	void	FieldSocketBattleSecBulletReloaded(MSG_FC_BATTLE_SEC_BULLET_RELOADED* pMsg);
	void	FieldSocketEventChangeWeather(MSG_FC_EVENT_CHANGE_WEATHER* pMsg);
	void	FieldSocketItemUpdateItemPos(MSG_FC_ITEM_UPDATE_ITEM_POS* pMsg);
	void	FieldSocketCharacterGetOtherRenderInfoOk(MSG_FC_CHARACTER_GET_OTHER_RENDER_INFO_OK* pMsg);
	void	FieldSocketInfoGetBuildingNPCInfoOk(MSG_FC_INFO_GET_BUILDINGNPC_INFO_OK* pMsg);
	void	FieldSocketEventSuggestChannels(MSG_FC_EVENT_SUGGEST_CHANNELS* pMsg);
	void	FieldSocketCityGetBuildingListOk(MSG_FC_CITY_GET_BUILDING_LIST_OK* pMsg);
	void	FieldSocketGetShopWarpTargetMapListOk(MSG_FC_EVENT_GET_SHOP_WARP_TARGET_MAP_LIST_OK* pMsg);
	void	FieldSocketAdminGetServerStatOk(MSG_FC_ADMIN_GET_SERVER_STAT_OK* pMsg);
	void	FieldSocketConnectClose(MSG_FC_CONNECT_CLOSE* pMsg);
	void	FieldSocketString128(MSG_FC_STRING_128* pMsg);
	void	FieldSocketString256(MSG_FC_STRING_256* pMsg);
	void	FieldSocketString512(MSG_FC_STRING_512* pMsg);
	void	FieldSocketWorldNotification(MSF_FC_WORLD_NOTIFICATION* pMsg);
	void	FieldSocketError(MSG_ERROR* pMsg);
	void	FieldSocketInvalidPacket(char* pPacket);
	void	FieldSocketMonsterSkillUseSkill(MSG_FC_MONSTER_SKILL_USE_SKILL* pMsg);
	void	FieldSocketMonsterSkillEndSkill(MSG_FC_MONSTER_SKILL_END_SKILL* pMsg);
	void	FieldSocketInfoGetSimpleItemInfoOk(MEX_ITEM_INFO & ItemInfo);
	void	FieldSocketInfoGetEnchantCostOk(MSG_FC_INFO_GET_ENCHANT_COST_OK* pMsg);
	void	FieldSocketItemMixItemsResult(MSG_FC_ITEM_MIX_ITEMS_RESULT* pMsg);
	void	FieldSocketGuildGetWarInfoOk(MSG_FC_GUILD_GET_WAR_INFO_OK* pMsg);
	void	FieldSocketStoreMoveItemOk(MSG_FC_STORE_MOVE_ITEM_OK* pMsg);
	void	FieldSocketCharacterGetOtherParamfactorOk(MSG_FC_CHARACTER_GET_OTHER_PARAMFACTOR_OK* pMsg);
	void	FieldSocketStoreUpdatePossess(MSG_FC_STORE_UPDATE_POSSESS* pMsg);
	void	FieldSocketStoreUpdateRareFix(MSG_FC_STORE_UPDATE_RARE_FIX* pMsg);
	void	FieldSocketQuestDiscardQuestOk(MSG_FC_QUEST_DISCARD_QUEST_OK* pMsg);
	void	FieldSocketTimerStartTimer(MSG_FC_TIMER_START_TIMER* pMsg);
	void	FieldSocketTimerStopTimer(MSG_FC_TIMER_STOP_TIMER* pMsg);
	void	FieldSocketTimerUpdateTimer(MSG_FC_TIMER_UPDATE_TIMER* pMsg);
	void	FieldSocketTimerPauseTimer(MSG_FC_TIMER_PAUSE_TIMER* pMsg);
	void	FieldSocketTimerContinueTimer(MSG_FC_TIMER_CONTINUE_TIMER* pMsg);
	void	FieldSocketSKillPrepareUseOk(MSG_FC_SKILL_PREPARE_USE_OK* pMsg);
	void	FieldSocketCancelPrepareOk(MSG_FC_SKILL_CANCEL_PREPARE_OK* pMsg);
	void	FieldSocketMoveWeaponVelOk(MSG_FC_MOVE_WEAPON_VEL_OK* pMsg);
	void	FieldSocketCountdownStart(MSG_FC_COUNTDOWN_START* pMsg);
	void	FieldSocketBattleShowDamage(MSG_FC_BATTLE_SHOW_DAMAGE* pMsg);
	void	FieldSocketItemUseBuffOk(MSG_FC_ITEM_USE_INFLUENCE_BUFF_OK* pMsg);
	void	FieldSocketItemUseInfluenceGameEventOk(MSG_FC_ITEM_USE_INFLUENCE_GAMEEVENT_OK* pMsg);
	void	FieldSocketShopBuyWarPointItemOK(MSG_FC_SHOP_BUY_WARPOINT_ITEM_OK* pMsg);


	void	FieldSocketBattleAttackOk(MSG_FC_BATTLE_ATTACK_OK* pMsg);
	void	FieldSocketBattleAttackFindOk(MSG_FC_BATTLE_ATTACK_FIND_OK* pMsg);
	void	FieldSocketBattleMineAttackOk(MSG_FC_BATTLE_MINE_ATTACK_OK* pMsg);
	void	FieldSocketBattleMineAttackFindOk(MSG_FC_BATTLE_MINE_ATTACK_FIND_OK* pMsg);
	void	FieldSocketObjectChangeBodyConditionOk(MSG_FC_OBJECT_CHANGE_BODYCONDITION_OK* pMsg);
	void	FieldSocketBattleAttackEvasionOk(MSG_FC_BATTLE_ATTACK_EVASION_OK* pMsg);
	void	FieldSocketWarNotifyInvasion(MSG_FC_WAR_NOTIFY_INVASION *pMsg);
	void	FieldSocketWarNotifyInfluenceMonsterDead(MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD *pMsg);
	void	FieldSocketWarNotifyInfluenceAutoMonsterDead(MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_DEAD *pMsg); // 2007-02-15 by bhsohn �� �ڵ� �Ҹ�� �̺�Ʈ ����.
	void	FieldSocketWarNotifyInfluenceMonsterIvasion(MSG_FC_WAR_NOTIFY_INFLUENCE_MONSTER_INVASION* pMsg);
	void	FieldSocketWarBossMonsterSummonDataInvasion(MSG_FC_WAR_BOSS_MONSTER_SUMMON_DATA* pMsg);
	void	FieldSocketWarSignBoardInsertString(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING* pMsg);
	void	FieldSocketWarReqSignBoardStringListOk(MSG_FC_WAR_REQ_SIGN_BOARD_STRING_LIST_OK* pMsg);
	void	FieldSocketWarSignBoardDeleteString(MSG_FC_WAR_SIGN_BOARD_DELETE_STRING* pMsg);
	void	FieldSocketWarUpdateContributionPointOk(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK* pMsg);
	void	FieldSocketWarJacoMonsterSummon(MSG_FC_WAR_JACO_MONSTER_SUMMON* pMsg);
	void 	FieldSocketWarInfluenceData(MSG_FC_WAR_INFLUENCE_DATA* pMsg);
	void	FieldSocketEventNotifyMsgGetOK(MSG_FC_EVENT_NOTIFY_MSG_GET_OK* pMsg);
	void	FieldSocketCityWarInfoInfluenceOK(MSG_FC_CITY_WARINFO_INFLUENCE_OK* pMsg);
	void	FieldSocketCityWarInfoOutPostOK(MSG_FC_CITY_WARINFO_OUTPOST_OK * pMsg);
	void	FieldSocketInfoGetGuildMarkOK(MSG_FC_INFO_GET_GUILDMARK_OK* pMsg);

	// 2007-07-04 by bhsohn ���۷����� �߰�
	void	FieldSocketWarStrategypointMonsterSummon(MSG_FC_WAR_STRATEGYPOINT_MONSTER_SUMMON* pMsg);

	void	FieldSocketAuctionInsertItem(MSG_FC_AUCTION_INSERT_ITEM* pMsg);
	void	FieldSocketAuctionRegisterItemOk(MSG_FC_AUCTION_REGISTER_ITEM_OK* pMsg);
	void	FieldSocketAuctionCancerRegisterOk(MSG_FC_AUCTION_CANCEL_REGISTER_OK* pMsg);
	void	FieldSocketAuctionBuyItemOk(MSG_FC_AUCTION_BUY_ITEM_OK* pMsg);
	void	FieldSocketAuctionPutEnchant(MSG_FC_AUCTION_PUT_ENCHANT* pMsg);

	void	FieldSocketMoveLockonOk(MSG_FC_MOVE_LOCKON_OK* pMsg);
	void	FieldSocketMoveUnlockonOk(MSG_FC_MOVE_UNLOCKON_OK* pMsg);
	void	FieldSocketMoveRolling(MSG_FC_MOVE_ROLLING_OK *pMsg);
	void	FieldSocketInfoGetCurrentMapInfoOk(MSG_FC_INFO_GET_CURRENT_MAP_INFO_OK* pMsg);
	void	FieldSocketCharacterCashPremiumCardInfo(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO* pMsg);
	void	FieldSocketStoreInsertUsingItem(MSG_FC_STORE_INSERT_USINGITEM* pMsg);
	void	FieldSocketStoreDeleteUsingItem(MSG_FC_STORE_DELETE_USINGITEM* pMsg);
	void	FieldSocketGuildSummonMember(MSG_FC_GUILD_SUMMON_MEMBER* pMsg);
	void	FieldSocketItemUseCarditemRestoreNotify(MSG_FC_ITEM_USE_CARDITEM_RESTORE_NOTIFY* pMsg);
	void	FieldSocketItemUseCarditemGuildsummonNotify(MSG_FC_ITEM_USE_CARDITEM_GUILDSUMMON_NOTIFY* pMsg);
	// 2008-03-14 by bhsohn �������� ���� ���Ǿƿ�� ó��
	void	FieldSocketInfoGetHappyHourEventInfoOk(BOOL bLevelUp, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	void	FieldSocketInfoGetGameEventInfoOk(MSG_FC_INFO_GET_GAME_EVENT_INFO_OK* pMsg);
	void	FieldSocketMonsterTutorialMonsterDeadNotify(MSG_FC_MONSTER_TUTORIAL_MONSTER_DEAD_NOTIFY* pMsg);

	// 2009. 09. 18 by ckPark ���Ǵ�Ƽ �ʵ� HP�� ���� ���� ��ȭ
	void	FieldSocketMonsterHPTalk(MSG_FC_MONSTER_HPTALK* pMsg);
	// end 2009. 09. 18 by ckPark ���Ǵ�Ƽ �ʵ� HP�� ���� ���� ��ȭ
// 2011-02-08 by jhahn EP4 Ʈ����  �ý��� ������Ʈ ����			
	void	FieldObjectChangeOK(MSG_FC_OBJECT_CHANGE* pMsg);
	void	FieldSocketShopClose(MSG_FC_SHOP_CLOSE* pMsg);
	void	FieldSocketSetLimitTime(MSG_FC_SET_LIMITTIME* pMsg);
	//end 2011-02-08 by jhahn EP4 Ʈ����  �ý��� ������Ʈ ����			

		// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	void	FieldSocketMonsterSkill(MSG_FC_MONSTER_SKILL* pMsg);
	void	FieldSocketMonsterMonsterBarrierUsing(MSG_FC_BATTLE_MONSTER_BARRIER_USING* pMsg);
	void	FieldSocketMonsterMonsterBarrierUse(MSG_FC_BATTLE_MONSTER_BARRIER_USE* pMsg);
	void	FieldSocketCharacterDebuffDotInfo(MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK* pMsg);
	void	FieldSocketCharacterDebuffDotApply(MSG_FC_CHARACTER_DEBUFF_DOT_APPLYING* pMsg);
	void	FieldSocketCharacterDebuffDotRelease(MSG_FC_CHARACTER_DEBUFF_DOT_RELEASE* pMsg);
	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	//2011-02-08 by jhahn EP4 Ʈ����  �ý���
	void	FieldSocketTriggerMapBuffSkill(MSG_FC_TRIGGER_MAP_BUFF_SKILL* pMsg);				//2011-10-24 by jhahn Ʈ���� �ý��� 
	//end 2011-02-08 by jhahn EP4 Ʈ����  �ý���

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	void	FieldSocketInfinityModeListOK(MSG_FC_INFINITY_MODE_LIST_OK* pMsg);
	void	FieldSocketInfinityReadyListOK(MSG_FC_INFINITY_READY_LIST_OK* pMsg);
	void	FieldSocketInfinityCreateOK(MSG_FC_INFINITY_CREATE_OK* pMsg);
	void	FieldSocketInfinityMemberInfoList(MSG_FC_INFINITY_MEMBER_INFO_LIST_OK* pMsg);
	void	FieldSocketInfinityLeaveOK(MSG_FC_INFINITY_LEAVE_OK* pMsg);
	void	FieldSocketInfinityBanOK(MSG_FC_INFINITY_BAN_OK* pMsg);
	void	FieldSocketInfinityJoinRequest(MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pMsg);
	void	FieldSocketInfinityJoinOK(MSG_FC_INFINITY_JOIN_OK* pMsg);
	void	FieldSocketInfinityEnter(MSG_FC_INFINITY_ENTER* pMsg);
	void	FieldSocketInfinityReadyOK(MSG_FC_INFINITY_READY_OK* pMsg);
	void	FieldSocketInfinityReadyCancelOK(MSG_FC_INFINITY_READY_CANCEL_OK* pMsg);
	void	FieldSocketInfinityChangeMaster(MSG_FC_INFINITY_CHANGE_MASTER_OK* pMsg);
	void	FieldSocketInfinityCinema(MSG_FC_INFINITY_CINEMA* pMsg);
	void	FieldSocketInfinitySuccessFIN();
	void	FieldSocketInfinityFailFIN();
	void	FieldSocketInfinityReadyFinishMainSvrStart();
	void	FieldSocketInfinityTenderDropItemInfo(MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pMsg);
	void	FieldSocketInfinityTenderStart(MSG_FC_INFINITY_TENDER_START* pMsg);
	void	FieldSocketInfinityPutInTenderOK(MSG_FC_INFINITY_TENDER_PUT_IN_TENDER_OK* pMsg);
	void	FieldSocketInfinityTenderResult(MSG_FC_INFINITY_TENDER_RESULT* pMsg);
	void	FieldSocketInfinityTenderResultTimeOver(MSG_FC_INFINITY_TENDER_RESULT_TIMEOVER* pMsg);
	void	FieldSocketInfinityTenderResultAllGiveUp(MSG_FC_INFINITY_TENDER_RESULT_ALLGIVEUP* pMsg);
	void	FieldSocketInfinityApplyResistanceItem(MSG_FC_INFO_APPLY_RESISTANCE_ITEM* pMsg);
	void	FieldSocketInfinityShopItemHeader();
	void	FieldSocketInfinityShopItem(MSG_FC_SHOP_INFINITY_ITEM* pMsg);
	void	FieldSocketInfinityShopItemDone();
	void	FieldSocketInfinityChangeLimitTime(MSG_FC_INFINITY_CHANGE_LIMITTIME* pMsg);
	void	FieldSocketInfinityJoinCancelRequestMaster(MSG_FC_INFINITY_JOIN_CANCEL_REQUEST_MASTERUSER* pMsg);
	void	FieldSocketInfinityRequestRestartByDisconnect();
	void	FieldSocketInfinityDeletedCinema(MSG_FC_INFINITY_DELETED_CINEMA* pMsg);
	void	FieldSocketInfinityEnterByDisConnect(MSG_FC_INFINITY_ENTER_BY_DISCONNECT* pMsg);
	void	FieldSocketInfinityPartyAutoCreated(MSG_FC_PARTY_AUTO_CREATED* pMsg);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	void	FieldSocketInfinityRegsiterDifficultyBonusInfoListData(MSG_INFINITY_DIFFICULTY_LIST_OK* pMsg);
	void	FieldSocketInfinityChangeRoomDifficultyLevelOK(MSG_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL_OK* pMsg);
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

	// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
	void	FieldSocketInfinityReadyCancelAllOK(MSG_FC_INFINITY_READY_CANCEL_ALL_OK* pMsg);
	// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)

	// 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
	void	FieldSocketInfinityCinemaSkip(MSG_FC_INFINITY_SKIP_ENDING_CINEMA_OK * pMsg);
	// End 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	void	FieldSocketInfoApplyDestParam(MSG_FC_INFO_APPLY_DESTPARAM* pMsg);
	void	FieldSocketInfoApplyDestParamList(MSG_FC_INFO_APPLY_DESTPARAM_LIST* pMsg);
	void	FieldSocketItemEndCoolItemOK(MSG_FC_ITEM_END_COOLINGTIME_ITEM_OK* pMsg);
	void	FieldSocketItemGetCoolTimeInfoOK(MSG_FC_ITEM_GET_COOLINGTIME_INFO_OK* pMsg);
	void	FieldSocketItemUseInvokingWearItemBuff(MSG_FC_ITEM_USE_INVOKING_WEAR_ITEM_BUFF* pMsg);
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	// 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)
	void	FieldSocketInfinityChangeAliveForGameClearMonsterHP(MSG_FC_INFINITY_CHANGE_ALIVE_FOR_GAMECLEAR_MONSTERHP* pMsg);
	void	FieldSocketMonsterCreatedAliveForGameClear(MSG_FC_MONSTER_CREATED_ALIVE_FOR_GAMECLEAR* pMsg);
	// end 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)

	// 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)
	void	FieldSocketMonsterCancelSkill(MSG_FC_MONSTER_SKILL_CANCEL* pMsg);
	// end 2010. 03. 18 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ��ų �߰�)

	void	FieldSocketCharacterTutorialSkipOk(MSG_FC_CHARACTER_TUTORIAL_SKIP_OK* pMsg);
	void	FieldSocketTutorialStartOK(MSG_FC_TUTORIAL_START_OK* pMsg);
	void	FieldSocketTutorialCompleteOK(MSG_FC_TUTORIAL_COMPLETE_OK* pMsg);
	void	FieldSocketTutorialEndOK();
	void	FieldSocketCharacterDeadNotify(MSG_FC_CHARACTER_DEAD_NOTIFY* pMsg);
	void	FieldSocketCitywarMonsterDead(MSG_FC_CITYWAR_MONSTER_DEAD* pMsg);
	void	FieldSocketCharacterChangeInfluenceType(MSG_FC_CHARACTER_CHANGE_INFLUENCE_TYPE* pMsg);
	void	FieldSocketCharacterChangePropensity(MSG_FC_CHARACTER_CHANGE_PROPENSITY* pMsg);
	void	FieldSocketRequestRepairOk(MSG_FC_SHOP_REQUEST_REPAIR_OK* pMsg);
	void	FieldSocketSkillConfirmUseAck(MSG_FC_SKILL_CONFIRM_USE_ACK* pMsg);
	void	FieldSocketSkillConfirmUse(MSG_FC_SKILL_CONFIRM_USE* pMsg);
	void	FieldSocketStoreUpdateItemStorage(MSG_FC_STORE_UPDATE_ITEMSTORAGE* pMsg);
	void	FieldSocketShopBuyColorItemOk(MSG_FC_SHOP_BUY_COLOR_ITEM_OK* pMsg);
	void	FieldSocketStoreUpdateUsingItem(MSG_FC_STORE_UPDATE_USINGITEM* pMsg);
	void	FieldSocketStoreUpdateItemNum(MSG_FC_STORE_UPDATE_ITEMNUM* pMsg);
	void	FieldSocketStoreLogGuildItemOK(MSG_FC_STORE_LOG_GUILD_ITEM_OK* pMsg);
	void	FieldSocketStoreLogGuildItemOKHeader();
	void	FieldSocketStoreLogGuildItemOKDone();
	void	FieldSocketEventCallWarpEventReqeust(MSG_FC_EVENT_CALL_WARP_EVENT_REQUEST* pMsg);

	// 2007-03-29 by dgwoo ��ۿ� ������ ���.
	void	FieldSocketCharacterObserverStartOK(MSG_FC_CHARACTER_OBSERVER_START* pMsg);
	void	FieldSocketCharacterObserverEndOK(MSG_FC_CHARACTER_OBSERVER_END* pMsg);
	void	FieldSocketCharacterObserverRegOK(MSG_FC_CHARACTER_OBSERVER_REG* pMsg);
	void	FieldSocketCharacterObserverInfoOK(MSG_FC_CHARACTER_OBSERVER_INFO* pMsg);

	void	FieldSocketArenaWatchReady(MSG_FC_ARENA_WATCH_READY* pMsg);
	void 	FieldSocketArenaWatchStart(MSG_FC_ARENA_WATCH_START* pMsg);
	void	FieldSocketArenaWatchEnd();
	void	FieldSocketArenaWatchRemoveUser(MSG_FC_ARENA_WATCH_REMOVE_USER* pMsg);
	void	FieldSocketCharacterWatchInfo(MSG_FC_CHARACTER_WATCH_INFO* pMsg);

	// 2007-08-16 by dgwoo ��������
	void	FieldSocketOutPostWarBefore(MSG_FC_OUTPOST_WAR_BEFORE* pMsg);
	void	FieldSocketOutPostWarStart(MSG_FC_OUTPOST_WAR_START* pMsg);
	void	FieldSocketOutPostProtectorDestroy(MSG_FC_OUTPOST_PROTECTOR_DESTROY* pMsg);
	void	FieldSocketOutPostResetStartOk(MSG_FC_OUTPOST_RESET_START_OK* pMsg);
	void	FieldSocketOutPostResetDestroy(MSG_FC_OUTPOST_RESET_DESTROY* pMsg);
	void	FieldSocketOutPostResetSuccess(MSG_FC_OUTPOST_RESET_SUCCESS* pMsg);
	void	FieldSocketOutPostWarEnd(MSG_FC_OUTPOST_WAR_END* pMsg);
	void	FieldSocketInfoNoticeRequestOk(MSG_FC_INFO_NOTICE_REQUEST_OK* pMsg);
	void	FieldSocketInfoNoticeRegOk();
	void	FieldSocketInfoNoticeModifyOk();
	void	FieldSocketInfoExpediencyfundPayBackOk(MSG_FC_INFO_EXPEDIENCYFUND_PAYBACK_OK* pMsg);
	void	FieldSocketInfoExpediencyfundRequestOk(MSG_FC_INFO_EXPEDIENCYFUND_REQUEST_OK* pMsg);
	void	FieldSocketOutPostNextWarInfoRequestOk(MSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK* pMsg);
	void 	FieldSocketOutPostNextWarSetTimeOk(MSG_FC_OUTPOST_NEXTWAR_SET_TIME_OK* pMsg);
	void	FieldSocketOutPostWarInfo(MSG_FC_OUTPOST_WAR_INFO* pMsg);

	// 2007-09-19 by dgwoo �ڷ���Ʈ.
	void	FieldSocketEventTeleportBuildComplete(MSG_FC_EVENT_TELEPORT_BUILDCOMPLETE* pMsg);
	void	FieldSocketEventTeleportDestroy(MSG_FC_EVENT_TELEPORT_DESTROY* pMsg);

	// 2007-04-19 by dgwoo �Ʒ��� �ý���.
	void	FieldSocketArenaRequestTeamOK(MSG_FC_ARENA_REQUEST_TEAM_OK* pMsg);
	void	FieldSocketArenaCreateTeamOK(MSG_FC_ARENA_CREATE_TEAM_OK* pMsg);
	void	FieldSocketArenaEnterTeamOK(MSG_FC_ARENA_ENTER_TEAM_OK* pMsg);
	void	FieldSocketArenaTeamMemberList(MSG_FC_ARENA_TEAM_MEMBER_LIST* pMsg);
	void	FieldSocketArenaReaveTeamOK();
	void	FieldSocketArenaTeamReady(MSG_FC_ARENA_TEAM_READY* pMsg);
	void	FieldSocketArenaEnterRoom(MSG_FC_ARENA_ENTER_ROOM* pMsg);
	void	FieldSocketArenaRoomWarStart();
	void	FieldSocketArenaRoomWarInfo(MSG_FC_ARENA_ROOM_WAR_INFO* pMsg);
	void	FieldSocketArenaRoomWarFinishHeader(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER* pMsg);
	void	FieldSocketArenaRoomWarFinish(MSG_FC_ARENA_ROOM_WAR_FINISH* pMsg);
	void	FieldSocketArenaRoomWarFinishDone(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE* pMsg);
	void	FieldSocketArenaRoomWarFinishDraw(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW* pMsg);
	void	FieldSocketArenaFinishWarpOK(MSG_FC_ARENA_FINISH_WARP_OK* pMsg);
	void	FieldSocketArenaBeforRoomWarFinish();
	void	FieldSocketArenaRequestCreateTeamInfoOK(MSG_FC_ARENA_REQUEST_CREATE_TEAMINFO_OK* pMsg);
	void	FieldSocketArenaTeamMatching();
	void	FieldSocketArenaRequestOtherTeamInfoOK(MSG_FC_ARENA_REQUEST_OTHER_TEAM_INFO_OK* pMsg);
	void	FieldSocketArenaGMCommandInfoHeader();
	void	FieldSocketArenaGMCommandInfo(MSG_FC_ARENA_GM_COMMAND_INFO* pMsg);
	void	FieldSocketArenaGMCommandInfoDone(MSG_FC_ARENA_GM_COMMAND_INFO_DONE* pMsg);

	void	FieldSocketBazaarSellPutItemOK(MSG_FC_BAZAAR_SELL_PUT_ITEM_OK* pMsg);
	void	FieldSocketBazaarBuyPutItemOK(MSG_FC_BAZAAR_BUY_PUT_ITEM_OK* pMsg);
	void	FieldSocketBazaarSellCancelItemOK(MSG_FC_BAZAAR_SELL_CANCEL_ITEM_OK* pMsg);
	void	FieldSocketBazaarBuyCancelItemOK(MSG_FC_BAZAAR_BUY_CANCEL_ITEM_OK* pMsg);
	void	FieldSocketBazaarSellStartOK();
	void	FieldSocketBazaarBuyStartOK();
	void	FieldSocketBazaarInfoOK(MSG_FC_BAZAAR_INFO_OK* pMsg);
	void	FieldSocketBazaarSellReQuestItemListOK(MSG_FC_BAZAAR_SELL_REQUEST_ITEMLIST_OK* pMsg);
	void	FieldSocketBazaarBuyReQuestItemListOK(MSG_FC_BAZAAR_BUY_REQUEST_ITEMLIST_OK* pMsg);
	void	FieldSocketBazaarSellItemEnchantlistOK(MSG_FC_BAZAAR_SELL_ITEM_ENCHANTLIST_OK* pMsg);
	void	FieldSocketBazaarCustomerInfoOK(MSG_FC_BAZAAR_CUSTOMER_INFO_OK* pMsg);
	void	FieldSocketBazaarSellBuyItemOK(MSG_FC_BAZAAR_SELL_BUY_ITEM_OK* pMsg);
	void	FieldSocketBazaarBuySellItemOK(MSG_FC_BAZAAR_BUY_SELL_ITEM_OK* pMsg);

	void	FieldSocketItemUseRandomBoxOk(MSG_FC_ITEM_USE_RANDOMBOX_OK* pMsg);
	void	FieldSocketItemUseRandomBoxOkDone();	// 2008-08-26 by bhsohn �����ڽ� �ý��� ����
	void	FieldSocketStoreRequestQuickslotOK(MSG_FC_STORE_REQUEST_QUICKSLOT_OK* pMsg);
	void	FieldSocketCharacterUseBonusstatOK(MSG_FC_CHARACTER_USE_BONUSSTAT_OK* pMsg);
	void	FieldSocketUseSkillSupportItemOK(MSG_FC_ITEM_USE_SKILL_SUPPORT_ITEM_OK* pMsg);
	void	FieldSocketGetServerDataTimeOK(MSG_FC_INFO_GET_SERVER_DATE_TIME_OK* pMsg);
	void	FieldSocketRequestPartyWarp(MSG_FC_QUEST_REQUEST_PARTY_WARP* pMsg);

	// 2007-08-07 by bhsohn ����Ŀ ������ �߰�
	void	FieldSocketChatAllInfluenceOK(MSG_FC_CHAT_ALL_INFLUENCE * pMsg);
	void	FieldSocketChatOutPostGuild(MSG_FC_CHAT_OUTPOST_GUILD* pMsg);

	// 2007-10-29 by dgwoo ���ý���
	void	FieldSocketCityPollRequestLeaderCandidateListOKHeader(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK_HEADER* pMsg);
	void	FieldSocketCityPollRequestLeaderCandidateListOK(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK* pMsg);
	void	FieldSocketCityPollRequestLeaderCandidateListOKDone();
	void	FieldSocketCityPollRequestLeaderCandidateInfoOK(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK* pMsg);
	void	FieldSocketCityPollRequestLeaderCandidateInfoOkGuildMark(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK_GUILDMARK* pMsg);
	void	FieldSocketCityPollRegLeaderCandidateOK(MSG_FC_CITY_POLL_REG_LEADER_CANDIDATE_OK* pMsg);
	void	FieldSocketCityPollDeleteLeaderCandidateOK();
	void	FieldSocketCityPollRequestPollDateOK(MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK* pMsg);
	void	FieldSocketCityPollVoteOK();
	void	FieldSocketCityPollLeaderElectionInfo(MSG_FC_CITY_POLL_LEADER_ELECTION_INFO* pMsg);
	// 2008-12-09 by dgwoo �̼Ǹ�����.
	void	FieldSocketQuestRequestMissionMasterHelpInvite(MSG_FC_QUEST_REQUEST_MISSIONMASTER_HELP_INVITE* pMsg);
	void	FieldSocketQuestMissionMasterHelpInviteOK(MSG_FC_QUEST_MISSIONMASTER_HELP_INVITE_OK* pMsg);
	void	FieldSocketQuestMissionMasterHelpRejectOK(MSG_FC_QUEST_MISSIONMASTER_HELP_REJECT_OK* pMsg);
	// end 2008-12-09 by dgwoo �̼Ǹ�����.

		// 2008-12-30 by bhsohn ���ﺸ���߰� ��ȹ��
	void	FieldSocketBossContributionGuild(MSG_FC_WAR_BOSS_CONTRIBUTION_GUILD* pMsg);
	// end 2008-12-30 by bhsohn ���ﺸ���߰� ��ȹ��


	// 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����
	void	SetWarDeclareInfo(SDECLARATION_OF_WAR* pWarInfo, SDECLARATION_OF_WAR_FORBID_TIME* pForbidTime, BOOL bSetDefault);
	// end 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����


	// 2009. 01. 12 by ckPark ���� ���� �ý���
	// �������� ������ �������� ���
	void	FieldSocketWarDeclareInfoOK(MSG_FC_INFO_DECLARATION_MSWAR_INFO_OK* pMsg);
	// end 2009. 01. 12 by ckPark ���� ���� �ý���




	// HackShield
	// 2008-04-04 by bhsohn �ٽ��� ��� ����
	//void	FieldSocketMoveHackShield_GuidReqMsg(MSG_FC_MOVE_HACKSHIELD_GuidReqMsg* pMsg);
	void	FieldSocketMoveHackShield_CRCReqMsg(MSG_FC_MOVE_HACKSHIELD_CRCReqMsg* pMsg);


	// ETC
	void	InitCharacterGameStartOk(AVECTOR3 vPos, BitFlag16_t MapWeather);// CharacterGameStartOk, CharacterConnectGameStartOk
	void	CharacterChangeCurrentHPAndDamageBodyCondition(float fCurrentHP);//FieldSocketCharacterChangeCurrentHP,FieldSocketCharacterChangeCurrentHPDPSPEP
	void	CharacterChangeCurrentDP(float sCurrentDP);
	void	CharacterChangeCurrentSP(SHORT sCurrentSP);
	void	CharacterChangeCurrentEP(float fCurrentEP);
	void	EventWarpOtherMap(MAP_CHANNEL_INDEX &channelIndex);

	void	FieldSocketNotifyServerShutdown();
	void	FieldSocketWarMonsterCreated(MSG_FC_WAR_MONSTER_CREATED* pMsg);
	void	FieldSocketWarMonsterAutoDestroyed(MSG_FC_WAR_MONSTER_AUTO_DESTROYED* pMsg);
	void	FieldSocketWarMonsterDead(MSG_FC_WAR_MONSTER_DEAD* pMsg);

	// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
	void	FieldSocketInfoSecondarypasswordCheckPassword(BOOL bSuccess);
	void	FieldSecondLockOk(MSG_FC_INFO_SECONDARYPASSWORD_LOCK_OK* pMsg);
	void	FieldSocketSecondPasswordUpdateOk(BOOL bSuccess);
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	void	FieldSocketConnectArenaServerInfo(MSG_FC_CONNECT_ARENASERVER_INFO* pMsg);
	void	FieldSocketConnectArenaServerLoginOk(MSG_FC_CONNECT_ARENASERVER_LOGIN_OK* pMsg);
	void	FieldSocketArenaPossibleGameStart(MSG_FC_ARENA_POSSIBLE_GAMESTART* pMsg);
	void	FieldSocketCharacterReadyGamestartFromArenaToMainserver(MSG_FC_CHARACTER_READY_GAMESTART_FROM_ARENA_TO_MAINSERVER_OK* pMsg);


	// 2009. 04. 06 by ckPark ���ӽ��۽� �߷� ��æƮ ���� �ȵȻ��·� ������ ����(0001388)
	void	FieldSocketPutEnchantDone();
	// end 2009. 04. 06 by ckPark ���ӽ��۽� �߷� ��æƮ ���� �ȵȻ��·� ������ ����(0001388)

	void	FieldSocketEventCouponEventUseCouponOK(MSG_FC_EVENT_COUPON_EVENT_USE_COUPON_OK* pMsg);
	void	FieldSocketEventCouponEventInfo(MSG_FC_EVENT_COUPON_EVENT_INFO* pMsg);

	// 2008-02-15 by bhsohn ��Ʈ��ũ �ӵ� üũ
	void	FieldSocketConnectNetWorkCheckOk(MSG_FC_CONNECT_NETWORK_CHECK_OK* pMsg);

	// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
	void	FieldSocketArenaServerSServerGroupForClient(MSG_FC_CONNECT_ARENASERVER_SSERVER_GROUP_FOR_CLIENT* pMsg);

	// 2015-05-1 by killburne
	void	FieldSocketInfoStrategicPointInfoDisplay(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY* pMsg);
	void	FieldSocketInfoStrategicPointInfoListDisplay(MSG_FC_INFO_STRATEGICPOINTINFO_DISPLAY_LIST* pMsg);
	// 2008-03-10 by bhsohn �Ʒ������� ��ȯ ��Ż ó��
	void	FieldSocketArenaUseCityWarpItem();
	void	FieldSocketInfoMSWarInfoDisplay(MSG_FC_INFO_MSWARINFO_DISPLAY* pMsg);
	void	FieldSocketInfoMSWarInfoDisplayOptionOK(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK*);
	// 2008-03-19 by bhsohn ����, ������ ����â
	void FieldSocketInfoMsWarInfoResultOk(MSG_FC_INFO_MSWARINFO_RESULT_OK* pMsg);
	void FieldSocketInfoSpWarInfoResultOkHeader();
	void FieldSocketInfoSpWarInfoResultOk(MSG_FC_INFO_SPWARINFO_RESULT_OK* pMsg);
	// end 2008-03-19 by bhsohn ����, ������ ����â
	void FieldSocketEventClickTeleportOK(MSG_FC_EVENT_CLICK_TELEPORT_OK* pMsg);
	// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	void FieldSocketInfoSpWarInfoResultOkDone();

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	// ���� ����Ʈ �޽���
	void FieldSocketChatLetterRqTitleHeader();
	void FieldSocketChatLetterRqTitle(MSG_FC_CHAT_LETTER_REQUEST_TITLE_OK* pMsg);
	void FieldSocketChatLetterRqTitleOkDone();

	void FieldSocketChatALLLetterRqTitle(MSG_FC_CHAT_ALLLETTER_REQUEST_TITLE_OK* pMsg); // ��ü ����
	void FieldSocketChatALLLetterRqTitleOkDone();	 // ��ü ���� �� �޾���

	void FieldSocketLetterReadOk(MSG_FC_CHAT_LETTER_READ_OK* pMsg);		// ���� ���� ��û
	void FieldSocketAllLetterReadOk(MSG_FC_CHAT_ALLLETTER_READ_OK* pMsg);		// ��ü���� ���� ��û
	void FieldSocketChatLetterDelOk();	// ���� ����
	void FieldSocketChatSendOk();		// ���� ������
	void FiedldSocketChatLetterReceive(MSG_FC_CHAT_LETTER_RECEIVE* pMsg);
	void FiedldSocketChatAllLetterReceive(MSG_FC_CHAT_ALLLETTER_RECEIVE* pMsg);
	void FieldSocketCharacterGetUserInfoOk(MSG_FC_CHARACTER_GET_USER_INFO_OK* pMsg);

	void FieldSocketCharacterGetUserItemInfoSecretOk(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_SECRET_OK* pMsg);	// 2012-06-14 by isshin ���۹̸�����					// 2012-07-12 by isshin ���۹̸����� On / Off ��� - ���� ����
	void FieldSocketCharacterGetUserItemInfoOk(MSG_FC_ENEMY_ITEM_INFO* pMsg);		// 2012-06-14 by isshin ���۹̸�����
	void FieldSocketCharacterGetUserItemInfoDone(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE* pMsg);	// 2012-06-14 by isshin ���۹̸�����		
	void FieldSocketCharacterChangeInfoOptionSecetOk();
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	// 2008-11-04 by dgwoo LuckyMachine��ȹ �߰�.
	void FieldSocketItemUseRuckyItemOK(MSG_FC_ITEM_USE_LUCKY_ITEM_OK* pMsg);
	// 2008-11-04 by dgwoo LuckyMachine��ȹ �߰�.

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	void FieldSocketItemChangedShapeItemNum(MSG_FC_ITEM_CHANGED_SHAPEITEMNUM* pMsg);
	void FieldSocketItemChangedEffectItemNum(MSG_FC_ITEM_CHANGED_EFFECTITEMNUM* pMsg);
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	// 2010. 01. 12 by ckPark ���� ������ ���� ������ ������ �ƴ� ���� �������� ����ϴ� ���� �ذ�
	void FieldSocketPutAllQuestDone();
	// end 2010. 01. 12 by ckPark ���� ������ ���� ������ ������ �ƴ� ���� �������� ����ϴ� ���� �ذ�

	// 2009-02-03 by bhsohn �̼� ���۽� �Ϸ� üũ üũ
	void FieldSocketQuestPutAllQuestMonsterCountDone();
	// end 2009-02-03 by bhsohn �̼� ���۽� �Ϸ� üũ üũ

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	void FieldSocketInfoWRKGetServiceInfoOk(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK* pMsg);
	void FieldSocketInfoWRKGetServiceInfoOkImage(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE* pMsg);
	void FieldSocketInfoWRKGetServiceInfoOkDone();
	void FieldSocketInfoWRKGetLevelRankerListOk(MSG_FC_INFO_WRK_GET_LEVEL_RANKER_LIST_OK* pMsg);
	void FieldSocketInfoWRKGetFameRankerListOk(MSG_FC_INFO_WRK_GET_FAME_RANKER_LIST_OK* pMsg);
	void FieldSocketInfoWRKGetPVPRankerListOk(MSG_FC_INFO_WRK_GET_PVP_RANKER_LIST_OK* pMsg);
	void FieldSocketCharacterChangeNickNameOk(MSG_FC_CHARACTER_CHANGE_NICKNAME_OK* pMsg);
	void FieldSocketInfoWrkGetSelfRankingOk(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK* pMsg);
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

	// 2009. 10. 14 by jskim ������ī ���� 
	void FieldSocketCharacterChangeStart(MSG_FC_CHARACTER_CHANGE_START_CITY_MAPINDEX_OK* pMsg);
	//end 2009. 10. 14 by jskim ������ī ���� 
	// 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
	void FieldSocketChangeAddedInventoryCount(MSG_FC_CHARACTER_CHANGE_ADDED_INVENTORY_COUNT* pMsg);
	//end 2009. 11. 3 by jskim ĳ��(�κ�/â�� Ȯ��) ������ �߰� ����
	// 2010. 03. 18 by jskim ���ͺ��� ī��
	void FieldSocketItemUpdateTransformerOk(MSG_FC_ITEM_UPDATE_TRANSFORMER_OK* pMsg);
	//end 2010. 03. 18 by jskim ���ͺ��� ī��
	// 2009-04-01 by bhsohn ���� �ұ���, ���� �ʱ�ȭ �ý���
	void FieldSocketQuestInserQuest(MSG_FC_QUEST_INSERT_QUEST* pMsg);
	void FieldSocketQuestDeleteQuest(MSG_FC_QUEST_DELETE_QUEST* pMsg);
	// end 2009-04-01 by bhsohn ���� �ұ���, ���� �ʱ�ȭ �ý���

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void FieldSocketCharacterKillCount(MSG_FC_CHARACTER_KILLCOUNT* pMsg);
	void FieldSocketCharacterRestCount(MSG_FC_CHARACTER_RESTCOUNT* pMsg);
	void FieldSocketCharacterBonusExpRate(MSG_FC_CHARACTER_BONUS_EXP_RATE* pMsg);
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	// 2010. 06. 08 by jskim Ȩ�����̾� �������� ó��
	void FieldSocketHomePremium(MSG_FC_ITEM_HOMEPREMIUM_INFO* pMsg);
	//end 2010. 06. 08 by jskim Ȩ�����̾� �������� ó��

	// 2010-06-15 by shcho&hslee ��ý��� - �������� ó��
	void FieldSocketCurrentPetBaseData(MSG_FC_ITEM_PET_BASEDATA* pMsg);

	void FieldSocketStoreUpdateArmorItemEventInfo(MSG_FC_STORE_UPDATE_ARMOR_ITEM_EVENT_INFO* pMsg); // 2013-05-15 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ���� ��Ŷ ���� 

	// 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���
	void FieldSocketCollectionInfo(MSG_FC_COLLECTION_INFO* pMsg);
	void FieldSocketCollectionShapeChange(MSG_FC_COLLECTION_SHAPE_CHANGE* pMsg);
	void UpdateStoreShapeItemNum(MSG_FC_ITEM_CHANGED_SHAPEITEMNUM* pMsg);
	// END 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���

	// 2013-06-26 by bhsohn �Ƹ� �÷��� �߰� ����
	void PopupUseRandomBoxOk(INT nResultItemNum0);	// ������ ItemNum
	void PopupGetCollectionInfo(INT nResultItemNum0);	// ���� �Ƹ� �÷��� ����
	// END 2013-06-26 by bhsohn �Ƹ� �÷��� �߰� ����

	// 2013-06-14 by ssjung �Ƹ� �÷��� �ý��� �̴��� �Ƹ� 
	void FieldSocketCollectionMontylyAromorInit();
	void FieldSocketCollectionMontylyAromorInfo(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* pMsg);
	// end 2013-06-14 by ssjung �Ƹ� �÷��� �ý��� �̴��� �Ƹ� 

	void FieldSocketWarInfluenceConsecutivePoint(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_POINT* pMsg);	// 2013-08-02 by ssjung ������ ���� ����ǥ��

	// OnRecvIMSocket~
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	//int	OnRecvIMSocketMessage(DWORD wParam);
	int	OnRecvIMSocketMessage(DWORD wParam, UINT uMsg);
	// IMSocket~
	void	IMSocketConnectLoginOk();
	void	IMSocketChatPtoP(MSG_IC_CHAT_PTOP* pMsg);
	void	IMSocketChatParty(MSG_IC_CHAT_PARTY* pMsg);
	void	IMSocketChatGuild(MSG_IC_CHAT_GUILD* pMsg);
	void	IMSocketChatAll(char* pPacket);
	void	IMSocketChatSellAll(MSG_IC_CHAT_SELL_ALL* pMsg);
	void	IMSocketChatCashAll(MSG_IC_CHAT_CASH_ALL* pMsg);
	void	IMSocketChatInfluenceAll(char* pPacket);
	void	IMSocketChatWar(MSG_IC_CHAT_WAR* pMsg);
	void	IMSocketChatChatRoom(MSG_IC_CHAT_CHATROOM* pMsg);
	void	IMSocketChatCrossNation(MSG_IC_CHAT_CNC* pMsg);	// 2014-08-03 Future, adding Cross Nation Chat

	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	void	IMSocketChatGetGuildOk(MSG_IC_CHAT_GET_GUILD_OK* pMsg, UINT nSocketNotifyType);
	void	IMSocketChatChangeGuild(MSG_IC_CHAT_CHANGE_GUILD* pMsg, UINT nSocketNotifyType);
	void	IMSocketChatChangeParty(MSG_IC_CHAT_CHANGE_PARTY* pMsg, UINT nSocketNotifyType);
	// end 2007-11-22 by bhsohn �Ʒ��� ���ռ���

	void	FieldSocketEventInfluenceMark(MSG_FC_EVENT_INFLUENCEMARK* pMsg);
	void	FieldSocketEventInfluenceMarkEnd();

	// 2008-11-21 by bhsohn �ŷ� �Ϸ� �ý��� ó��
	void	FieldSocketTradeOkTradeNotify(MSG_FC_TRADE_OK_TRADE_NOTIFY* pMsg);
	// end 2008-11-21 by bhsohn �ŷ� �Ϸ� �ý��� ó��

	// 2010-08-31 by shcho&&jskim, ���� �ý��� ����
	void	FieldSocketDissolDissolutionPutItem(MSG_FC_DISSOLVED_SUCCESS_AND_FAILED* pMsg);
	// end 2010-08-31 by shcho&&jskim, ���� �ý��� ����

	// 2011-05-17 by jhahn	����3�� ���� ��ü
	void	FieldMonsterChangeOK(MSG_FC_MONSTER_CHANGE_INDEX* pMsg);
	// end 2011-05-17 by jhahn	����3�� ���� ��ü
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	void	FieldPetNameChangeOK(MSG_FC_ITEM_PET_SET_NAME_OK* pMsg);
	void	FieldPetExpRationOK(MSG_FC_ITEM_PET_SET_EXP_RATIO_OK* pMsg);

	void	FieldPetLevelChangeOK(MSG_FC_ITEM_PET_CHANGE_LEVEL* pMsg);
	void	FieldPetExpChangeOK(MSG_FC_ITEM_PET_CHANGE_EXP* pMsg);

	void	FieldPetSetSocket(MSG_FC_ITEM_PET_SET_SOCKET_OK* pMsg);
	void	FieldPetSetKitSlot(MSG_FC_ITEM_PET_SET_KIT_SLOT_OK* pMsg);
	void	FieldPetSetAutoSkillSlot(MSG_FC_ITEM_PET_SET_AUTOSKILL_SLOT_OK* pMsg);
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	// 2011-05-30 by jhahn, ����3�� ���� ����
	void	FieldMonsterDeleteOK(MSG_FC_MONSTER_CINEMA_DELETE_NOTIFY* pMsg);
	// end 2011-05-30 by jhahn, ����3�� ���� ����

	// 2011-05-30 by jhahn, ����3�� �ð� ����
	void	FieldSocketInfinityResetTime(MSG_FC_INFINITY_SET_LIMITTIME* pMsg);
	// end 2011-05-30 by jhahn, ����3�� �ð� ����


	// 2011-05-17 by jhahn	����3���ó׸� ����
	void	FieldInfinityCinemaOK(MSG_FC_INFINITY_MOVIE* pMsg);
	// end 2011-05-17 by jhahn	����3���ó׸� ����

	void	IMSocketPartyAcceptInviteOk(MSG_IC_PARTY_ACCEPT_INVITE_OK* pMsg);
	void	IMSocketPartyPutMember(MSG_IC_PARTY_PUT_MEMBER* pMsg);
	void	IMSocketPartyLeaveOk(MSG_IC_PARTY_LEAVE_OK* pMsg);
	void	IMSocketPartyBanMemberOk(MSG_IC_PARTY_BAN_MEMBER_OK* pMsg);
	void	IMSocketPartyDismemberOk(MSG_IC_PARTY_DISMEMBER_OK* pMsg);
	void	IMSocketPartyTransferMasterOk(MSG_IC_PARTY_TRANSFER_MASTER_OK* pMsg);
	void	IMSocketPartyPutAllMember(MSG_IC_PARTY_PUT_ALL_MEMBER* pMsg);
	void	IMSocketPartyChangeFlightFormationOk(MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK* pMsg);
	void	IMSocketPartyGetFlightPosition(MSG_IC_PARTY_GET_FLIGHT_POSITION* pMsg);
	void	IMSocketPartyChangeFlightPosition(MSG_IC_PARTY_CHANGE_FLIGHT_POSITION* pMsg);
	void	IMSocketPartyCancelFlightPosition(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION* pMsg);
	void	IMSocketPartyAllFlightPosition(MSG_IC_PARTY_ALL_FLIGHT_POSITION* pMsg);
	void	IMSocketPartyPutLastPartyInfo(MSG_IC_PARTY_PUT_LAST_PARTY_INFO* pMsg);
	void	IMSocketPartyUpdateMemberInfoMapName(MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME* pMsg);
	void	IMSocketPartyMemberInvalidated(MSG_IC_PARTY_MEMBER_INVALIDATED* pMsg);
	void	IMSocketPartyMemberRejoined(MSG_IC_PARTY_MEMBER_REJOINED* pMsg);
	void	IMSocketPartyUpdateItemPos(MSG_IC_PARTY_UPDATE_ITEM_POS* pMsg);
	void	IMSocketGuildCreateOk(MSG_IC_GUILD_CREATE_OK* pMsg);
	void	IMSocketGuildGetGuildInfoOk(MSG_IC_GUILD_GET_GUILD_INFO_OK* pMsg);
	void	IMSocketGuildLeaveOk(MSG_IC_GUILD_LEAVE_OK* pMsg);
	void	IMSocketGuildBanMemberOk(MSG_IC_GUILD_BAN_MEMBER_OK* pMsg);
	void	IMSocketGuildRequestInviteQuestion(MSG_IC_GUILD_REQUEST_INVITE_QUESTION* pMsg);
	void	IMSocketGuildAcceptInviteOk(MSG_IC_GUILD_ACCEPT_INVITE_OK* pMsg);
	void	IMSocketGuildRejectInviteOk(MSG_IC_GUILD_REJECT_INVITE_OK* pMsg);
	void	IMSocketGuildSetMemberState(MSG_IC_GUILD_SET_MEMBER_STATE* pMsg);
	void	IMSocketGuildDismemberOk(MSG_IC_GUILD_DISMEMBER_OK* pMsg);
	void	IMSocketGuildCancelDismemberOk(MSG_IC_GUILD_CANCEL_DISMEMBER_OK* pMsg);
	void	IMSocketGuildGetDismemberDateOk(MSG_IC_GUILD_GET_DISMEMBER_DATE_OK* pMsg);
	void	IMSocketGuildChangeGuildNameOk(MSG_IC_GUILD_CHANGE_GUILD_NAME_OK* pMsg);
	void	IMSocketGuildSetGuildMarkOk(MSG_IC_GUILD_SET_GUILD_MARK_OK* pMsg);
	void	IMSocketGuildGetGuildMarkOk(MSG_IC_GUILD_GET_GUILD_MARK_OK* pMsg);
	void	IMSocketGuildSetRankOk(MSG_IC_GUILD_SET_RANK_OK* pMsg);
	void	IMSocketGuildChangeGuildState(MSG_IC_GUILD_CHANGE_GUILD_STATE* pMsg);
	void	IMSocketGuildLoadingGuildDone(MSG_IC_GUILD_LOADING_GUILD_DONE* pMsg);
	void	IMSocketGuildGetOtherGuildInfoOk(MSG_IC_GUILD_GET_OTHER_GUILD_INFO_OK* pMsg);
	void	IMSocketAdminGetServerStatOk(MSG_IC_ADMIN_GET_SERVER_STAT_OK* pMsg);
	void	IMSocketConnectClose(MSG_IC_CONNECT_CLOSE* pMsg);
	void	IMSocketString128(MSG_IC_STRING_128* pMsg);
	void	IMSocketString256(MSG_IC_STRING_256* pMsg);						// 2006-04-13 by ispark
	void	IMSocketString512(MSG_IC_STRING_512* pMsg);						// 2006-04-13 by ispark
	void	IMSocketGuildWarReady(MSG_IC_GUILD_WAR_READY* pMsg);
	void	IMSocketGuildStartWar(MSG_IC_GUILD_START_WAR* pMsg);
	void	IMSocketGuildEndWar(MSG_IC_GUILD_END_WAR* pMsg);
	void	IMSocketError(MSG_ERROR* pMsg);
	void	IMSocketInvalidPacket(char* pPacket);
	void	IMSocketChatFriendlistAll(MSG_IC_CHAT_FRIENDLIST_LOADING_OK* pMsg);
	void	IMSocketChatFriendlistInsertOk(MSG_IC_CHAT_FRIENDLIST_INSERT_OK* pMsg);
	void	IMSocketChatRejectlistAll(MSG_IC_CHAT_REJECTLIST_LOADING_OK* pMsg);
	void	IMSocketChatRejectlistInsertOk(MSG_IC_CHAT_REJECTLIST_INSERT_OK* pMsg);
	void	IMSocketChatFriendlistRefreshOk(MSG_IC_CHAT_FRIENDLIST_REFRESH_OK* pMsg);
	void	IMSocketChatFriendlistDeleteOk(MSG_IC_CHAT_FRIENDLIST_DELETE_OK* pMsg);
	void	IMSocketChatRejectlistDeleteOk(MSG_IC_CHAT_REJECTLIST_DELETE_OK* pMsg);
	void	IMSocketCountdownStart(MSG_IC_COUNTDOWN_START* pMsg);
	void	IMSocketItemUseCarditemGuildChangeMemberCapacity(MSG_IC_GUILD_CHANGE_MEMBER_CAPACITY* pMsg);
	void	IMSocketGuildGetGuildMemberListOk(MSG_IC_GUILD_GET_GUILD_MEMBER_LIST_OK* pMsg);
	void	IMSocketAdminCallGMInfoOk(MSG_IC_ADMIN_CALLGM_INFO_OK* pMsg);
	void	IMSocketAdminCallGMViewOk(char* pPacket);
	void	IMSocketAdminCallGMBringOk(char* pPacket);
	void	IMSocketGuildEndWarAdminNotify(MSG_IC_GUILD_END_WAR_ADMIN_NOTIFY* pMsg);

	void	IMSocketChatRoomCreateOK(MSG_IC_CHATROOM_CREATE_OK* pMsg);
	void	IMSocketChatRoomListInfoOK(MSG_IC_CHATROOM_LIST_INFO_OK* pMsg);
	void	IMSocketChatRoomRequestInviteQuestion(MSG_IC_CHATROOM_REQUEST_INVITE_QUESTION *pMsg);
	void	IMSocketChatRoomJoinOK();
	void	IMSocketChatRoomAcceptInviteOK(MSG_IC_CHATROOM_ACCEPT_INVITE_OK* pMsg);
	void	IMSocketChatRoomRejectInviteOK();
	void	IMSocketChatRoomLeaveOK(MSG_IC_CHATROOM_LEAVE_OK* pMsg);
	void	IMSocketChatRoomBanOK(MSG_IC_CHATROOM_BAN_OK* pMsg);
	void	IMSocketChatRoomChangeNameOK(MSG_IC_CHATROOM_CHANGE_NAME_OK* pMsg);
	void	IMSocketChatroomChangeMasterOK(MSG_IC_CHATROOM_CHANGE_MASTER_OK* pMsg);
	void	IMSocketChatRoomChangeLockPWOK(MSG_IC_CHATROOM_CHANGE_LOCK_PW_OK* pMsg);
	void	IMSocketChatRoomChangeMaxMemberOK(MSG_IC_CHATROOM_CHANGE_MAX_MEMBER_OK* pMsg);
	void	IMSocketChatRoomMemberInfoOK(MSG_IC_CHATROOM_MEMBER_INFO_OK* pMsg);
	void	IMSocketChatRoomOtherMemberInfoOK(MSG_IC_CHATROOM_MEMBER_INFO_OK* pMsg);
	// 2008-07-11 by bhsohn ������ ģ�� ����Ʈ �ý��� �߰�
	void	IMSocketChatOnlineEachOtherFriendCount(MSG_IC_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT* pMsg);

	// 2008-06-16 by bhsohn EP3���� ���׼���
	void	IMSocketGuildUpdateIntroductionOk();
	void	IMSocketGuildDelIntroductionOk();
	void	IMSocketUpdateSelfIntroOk();
	void	IMSocketDelSelfIntroOk();

	// 2008-06-18 by bhsohn ���ܿ����� ī�� ���� ó��
	void	IMSocketGuildChangeMemberShip(MSG_IC_GUILD_CHANGE_MEMBERSHIP* pMsg);


	// 2009. 01. 12 by ckPark ���� ���� �ý���
	// �������� ������ �������� ���
	void	IMSocketWarDeclareSetOK(MSG_IC_INFO_DECLARATION_MSWAR_SET_OK* pMsg);
	// end 2009. 01. 12 by ckPark ���� ���� �ý���
	// 2010. 03. 18 by jskim ���ͺ��� ī��
	void	IMSocketPartyUpdateItemTransformerOk(MSG_IC_PARTY_UPDATE_ITEM_TRANSFORMER_OK* pMsg);
	//end 2010. 03. 18 by jskim ���ͺ��� ī��
	// 2008-07-17 by bhsohn ���� ��ũ ���Ž� �ñ�� ���� �ذ�
	void	IMGuildChangeFameRank(MSG_IC_GUILD_CHANGE_FAME_RANK* pMsg);

	// 2009-01-12 by bhsohn ģ�� ��� ���ϴ� ���� �޽���â ��� �߰�
	void	IMChatFriendListInsertNotify(MSG_IC_CHAT_FRIENDLIST_INSERT_NOTIFY* pMsg);
	// end 2009-01-12 by bhsohn ģ�� ��� ���ϴ� ���� �޽���â ��� �߰�

	// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
	// ��ġ ���� ����Ʈ ���
	void	FieldSocketCharacterMapEffectOk(MSG_FC_CHARACTER_SHOW_MAP_EFFECT_OK* pMsg);

	// 2007.04.24 by bhsohn China IME Working
	BOOL	RenderReadingText(int x, int y);

	// 2007-07-04 by bhsohn ���۷����� �߰�
	void ShowOpMissionComplete(MSG_FC_QUEST_REQUEST_SUCCESS_RESULT* pMsg);

	// 2007-07-24 by bhsohn �����̼� mp3�߰�
	void	CompleteNarrationSound(char* pNarrationMusic);

	// 2007-11-01 by bhsohn ���� �ƿ�� �̺�Ʈ ó�� ��� ����
	void	RequestHappyHourInfo();
	// 2007-11-28 by dgwoo ���� ���ӽ� ���� �޽����� �ִ��� ������ ��û.
	void	RequestNotifyMsg();

	// 2007-11-05 by bhsohn ���� ������Ʈ üũ�� ó��
	BOOL	IsMeshMonsterObject(int nMeshIndex);

	// 2007-12-21 by bhsohn ���α׷� ���� �� ���� ����
	void	InitMultiLockInfo();

	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	BOOL	ForceMousePos(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void	ShowPartyChangeInfo(SPARTY_INFO* pNewSetupInfo);

	// 2008-12-30 by bhsohn ���ﺸ���߰� ��ȹ��
	void	CreateChatWarContribution(BYTE i_ItemUpdateType, int i_nOrgItemCount, int i_nNewItemCount);

private:
	// 2004-12-13 by jschoi - Fade-In & Out �ý���
	void	FadeEffectTick();
	void	FadeEffectRender();

	// 2005-03-03 by jschoi - RequestEnable 
	void	RenderRequestEnable();
	void	RequestEnableTick(float fElapsedTime);

	// 2005-03-17 by jschoi - Error ó��
	BOOL	FieldSocketErrorByMsgType(MSG_ERROR* pMsg);
	BOOL	FieldSocketErrorByErrorCode(MSG_ERROR* pMsg);
	BOOL	IMSocketErrorByMsgType(MSG_ERROR* pMsg);
	BOOL	IMSocketErrorByErrorCode(MSG_ERROR* pMsg);

	/*--------------------------------------------------------------------------*/
	// 2006-06-05 by ispark, �� ���� �� �Լ�
	BOOL	HS_Init();
	BOOL	HS_UnInit();
	BOOL	HS_StartService();
	BOOL	HS_StopService();
	/*--------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------*/
	// �� ���� ���� �Լ�
	BOOL	HS_SaveFuncAdress();

	// 2006-06-15 by ispark, ���� �ð��� ��Ŷ ������ �Լ�
	void	SendOtherPacket();
	/*--------------------------------------------------------------------------*/

	// 2006-11-16 by ispark, ���� ������
	void	RenderAlpha();

	// 2007-02-12 by bhsohn Item ���� ���� ó��
	void	AddMultiItemList(deque<stMultiSelectItem>* pVector, const stMultiSelectItem* pSelItem, BOOL bfrontAdd);
	void	DelMultiItem(deque<stMultiSelectItem>* pVector, INVEN_DISPLAY_INFO *pDisplayInfo);
	void	DelCityStoreMultiItem(UID64_t	 nUniqueNumber);	// â���� ���� ���
	void	SellMultiResponseOK();
	void	SellMutltiOneItem();
	void	Convert_DWORD_To_String(DWORD dwNum, char* pDstSpi);
	void	ToStoreMultiResponseOK();
	void	UpdateMutltiItemError();
	void	UpdateMultiResponseOK();
	void	ToStoreMultiItemError();
	void	SellMutltiItemError();
	void	FromStoreMultiResponseOK();
	void	FromStoreMultiItemError();
	BOOL	IsCheckCityStoreFull();
	BOOL	IsCheckInvenFull();
	BOOL	IsCheckSPIEmpty();
	BOOL	IsCheckGuildStore();

	// 2007-02-28 by bhsohn ���� �ǸŽ� 0 spi�� ���� ó��
	void	UpdateMultiSellZeroSPIItem(UID64_t nUniqueNumber);

	// 2007-03-09 by bhsohn ���� ���� â�� �κ� ����
	BOOL	IsStoreHave(int nItemNum);
	BOOL	IsInvenHave(int nItemNum);

	// 2007-04-02 by bhsohn Move��Ŷ���� ���� ���� üũ
	void	UpdateEnemySkillState(bool bUseSkill, CUnitData* pUnit, ClientIndex_t	ClientIndex, INT ItemNum);

	// 2007-07-09 by bhsohn ���� ������ ���� ���� ���½ÿ� ���� ó��
	void	CancelBazzarSkill(int nSkillItemNum);

	// 2007-07-24 by bhsohn �����̼� mp3�߰�
	void	PlayNarrationSound();

	// 2007-08-03 by bhsohn ĳ���� ������Ʈ üũ�� ����
	void	ResetCharecterOmiInfo();

	// 2007-08-07 by bhsohn ����Ŀ ������ �߰�
	void	AddSpeakerGMString(char* pGmChat, BOOL bGmColor);

	// 2007-08-23 by bhsohn �������� �⿩���� ū ������ ���� �޽��� ó��
	void	AlertMsgWarMonsterInfo(MEX_MONSTER_INFO* pMonsterInfo, char* pBestGuildName);

	// 2007-09-05 by bhsohn China IME Working
	void	TickImeChange();

	// 2008-01-11 by bhsohn 1�е��� ������ Ŭ���̾�Ʈ ���߰� �ϸ� Ŭ���̾�Ʈ ���� ����
	BOOL	IsCompultionGame(DWORD dwCapTime);		// ���� ���� ���� ���� �Ǵ�
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	CFieldWinSocket* GetFieldWinSocket(UINT nSocketNotifyType);		// ���� ���¿� �´� �ʵ� ����
	CIMSocket* GetIMWinSocket(UINT nSocketNotifyType);				// ���� ���¿� �´� IM����
	void DoModalAreanQuestion();
	void ReCreateAllGameObject();										// ���� ������Ʈ�� �� ������ �ٽ� ����
	void SetMyShuttleInfo(CHARACTER* pCharacter);		// ����ü�� Ŭ���̾�Ʈ �ε��� �� ����
	void SendArenaIMLogin();

	// 2016-12-23 Call when you edit MyShuttleInfo without calling SetMyShuttleInfo(CHARACTER*)
	void MyShuttleInfoChanged();

	// 2008-03-03 by bhsohn ���� �Ʒ��� IM���� ������� ����
	void ArenaIMSocketConnectLoginOk();
	void IMSocketConnectFmToImOk();

	// 2008-05-28 by bhsohn Multi Lock������ ��û�� ���� �����ÿ� ���� ó��
	void MultiLockFinished(MessageType_t	nPreLockMsg);

	// 2008-06-03 by bhsohn EP3 ���� ���� ó��
	void IMSocketNoticeWriteOk(MSG_IC_GUILD_NOTICE_WRITE_OK* pMsg);		// ���� ���� ���� ���
	// ���� ������ ����
	void IMSocketGetApplicantOkHeader();
	void IMSocketGetApplicantOKDone();
	void IMSocketGetApplicantOk(MSG_IC_GUILD_GET_APPLICANT_OK *pMsg);
	void IMSocketGetIntroducitonOk(MSG_IC_GUILD_GET_INTRODUCTION_OK* pMsg);
	void IMSocketGuildSearchIntroOk(MSG_IC_GUILD_SEARCH_INTRODUCTION_OK* pMsg);
	void IMSocketGuildSearchIntroDone();
	void IMSocketGuildMemerLevelUp(MSG_IC_GUILD_MEMBER_LEVEL_UP* pMsg);

	// 2008-06-03 by bhsohn EP3 ��� ���� ó��
	void IMSocketPartyListInfoOk(MSG_IC_PARTY_LIST_INFO_OK* pMsg); // ��� ����Ʈ 
	void IMSocketPartyReCommandMemberOk(MSG_IC_PARTY_RECOMMENDATION_MEMBER_OK* pMsg);
	void IMSocketPartyChangeInfoOk();
	void IMSocketPartyInfo(MSG_IC_PARTY_INFO* pMsg);
	void IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg);

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	void IMSocketPartyGetAutoInfoOK(MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK* pMsg);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���


	// 2008-08-11 by bhsohn �ܵ����� ���� �ǰԲ�
	BOOL SetTempCharcaterInfo();

	// 2008-08-18 by bhsohn 1�� ���� ������ �̵� �ý��� 
	void TickItemSell();
	void IntervalSellMutltiOneItem();	// ������ �ְ� �Ǹ�


	// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	void TickInfinityStart();
	// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)


	// 2010. 03. 23 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���Ǵ�Ƽ �ʵ� ���� ĳ��������)
	void	TickInfintyMapLoadedSend();
	// end 2010. 03. 23 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���Ǵ�Ƽ �ʵ� ���� ĳ��������)


	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	void TickJoyStickInfo();

	// 2009-04-21 by bhsohn �κ��� ������ �̺�Ʈ ���޽� �޽���â �������� �����ذ�
	void TradeUpdateItemAddChat(MSG_FC_TRADE_UPDATE_ITEM_COUNT* pMsg);
	// end 2009-04-21 by bhsohn �κ��� ������ �̺�Ʈ ���޽� �޽���â �������� �����ذ�

public:

	void	StartFadeEffect(BOOL bFadeIn, int nTime, D3DCOLOR dwColor);
	void	EndFadeEffect();
	float	GetFadeAlpha();

	// 2012-11-28 by jhjang �׼� ���� ���� ó�� �߰�
	BYTE	GetDBSid() const { return m_nDBSid; }
	void	SetDBSid(BYTE nDBSid) { m_nDBSid = nDBSid; }
	// end 2012-11-28 by jhjang �׼� ���� ���� ó�� �߰�
	void	RenderCandidate(int x, int y);
	int		SerchMissionCondition(int nNewItemCount = 0);
	void	SetHanFontLang(LPARAM lParam);
	const char*	GetFontStyle() const { return m_strFont; }
	void	SendMissionComplete(int nPartyMissionIndex = 0);// 2006-03-28 by ispark, ��Ƽ�̼��� ��� �ε����� �Ѱ� �޴´�.

	int		GetUnitArmorColorNum() const { return m_nArmorNum; }
	int		GetUnitArmorColorMax() const { return m_nArmorNumMax; }
	void	SetUnitArmorColorNum(int nNum) { m_nArmorNum = nNum; }
	void	SetUnitArmorColorMax(int nNum) { m_nArmorNumMax = nNum; }

	// 2005-03-18 by jschoi
	void	SetPremiumCardInfo(MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO* sMsg);
	MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO* GetPrimiumCardInfo() { return &m_PremiumCardInfo; }

	// 2005-12-21 by ispark
	void	SendHackTime_WeaponInfo();
	void	SendHackTime_TotalWeight();
	void	SendHackTime_EngineInfo();
	void	SendHackTime_MemoryCheck(float ValidDistance, float CurrentDistance, float fElapsedTime);

	// 2006-04-13 by ispark
	void	CheckReadyStringFiltering(char *szChat, int *nFTWordCounts, int *nFTTotalBytes);


	// 2006-06-05 by ispark, �� ����
	BOOL	HS_Start();
	void	HS_Close();
	int	__stdcall HS_CallbackProc(long lCode, long lParamSize, void* pParam);

	// 2008-11-28 by bhsohn XignCode�߰�
	BOOL StartGameGuard();
	BOOL CloseGameGuard();
	// end 2008-11-28 by bhsohn XignCode�߰�

	// 2006-09-27 by ispark
	void LostDeviceTick() override;

	void SetEffectUseItem(ITEM* pItemInfo);
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void SetPlayEffect(INT nEffectNum, INT nSoundNum);
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	// 2007-02-12 by bhsohn ��Ŀ�� �Ҿ� �Ծ��� ���� ���� ó��
	void UpdateCheckTime();
	FLOAT GetCheckElapsedTime();

	// 2007-02-12 by bhsohn Item ���� ���� ó��
	void	StartSellMultiItem();
	BOOL	SetMultiSelectItem(INVEN_DISPLAY_INFO *pDisplayInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon, BYTE bySelectType);

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	//BOOL	SetMultiSelectCityStoreItem(CItemInfo* pItemInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon, BYTE bySelectType);
	BOOL	SetMultiSelectCityStoreItem(CItemInfo* pItemInfo, INVEN_DISPLAY_INFO	*pSelectItem, POINT ptIcon, BYTE bySelectType, INVEN_DISPLAY_INFO  *pDisSelectItem);
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	void	OnCtrlBtnClick(BOOL bCtrlBtnClick);
	BOOL	GetCtrlBtnClick();
	void	StartToStoreMultiItem();
	void	ToStoreMultiOneItem();
	void	StartFromStoreMultiItem();
	void	FromStoreMultiOneItem();

	// 2007-05-21 by bhsohn �Ʒ��� �ʿ��� �̼�â Ŭ�� �ȵǰ� ����
	MAP_INFO* GetMyShuttleMapInfo();
	BOOL	IsWarMotherShip();

	// 2007-06-18 by bhsohn ��Ʈ�� ���� ���� ó��
	void	SetKeyBoardLostDevice(bool bLostDevice) const;

	// 2007-11-20 by bhsohn �ʷε� �ϴ� ��� ����
	void	ConnectLoginIMServer(char* pUserId, char* pCharacterName, char* pUserPassWord);
	void	SendGameStart(ClientIndex_t	ClientIndex, UID32_t CharacterUniqueNumber);
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	void	InitOnlyGameStartOk(AVECTOR3 vPos, BitFlag16_t MapWeather, BOOL bArenaRestart);
	void	UpdateGameStartMapInfo();

	// 2007-12-17 by bhsohn ������ ������ �� �ȿ��� ���� �����·�
	BOOL	IsLockMode();

	// 2008-08-11 by bhsohn �ܵ����� ���� �ǰԲ�
	void	InitItemList();
	void	AddItemList(MSG_FC_TRADE_INSERT_ITEM* pMsg);
	void	SaveItemList();
	void	SetUnitDetailInfo(CHARACTER* pCharac);
	void	SaveUnitDetailInfo();
	void	SetLoginOkInfo(MSG_FC_CONNECT_LOGIN_OK* pLoginInfo);
	void	SaveLoginOkInfo();
	void	SetGameStartOkInfo(MSG_FC_CHARACTER_GAMESTART_OK* pGamestartInfo);
	void	SaveGameStartOkInfo();
	void	SaveSinglePlayInfo();

	// 2009-03-16 by bhsohn A��� ���� ���� �������� �ּ�ȭ
	DWORD	GetCurrentGameTime();
	// end 2009-03-16 by bhsohn A��� ���� ���� �������� �ּ�ȭ

	// 2009. 11. 03 by ckPark �±� APEX ���Ӱ��� �۾�
	void	StartApexClient();
	// end 2009. 11. 03 by ckPark �±� APEX ���Ӱ��� �۾�

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	void	ChangeMapObject(ObjectIdx_t nOrigObjIndex, ObjectNum_t nChangeObjIndex);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	// 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����
	void SetPlaySearchEyeEffect(CUnitData * pUnit, INT nEffectNum, INT nSoundNum, bool bMoveCheck = true);
	void ClearPlaySearchEyeEffect(CUnitData * pUnit, INT nEffectNum);
	// end 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����

	// 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
	int AddINFCnt();	//�������̽� â �ߺ� �������� ���ϰ�, �������� ����Ͽ� ����
	int RemoveINFCnt();	//�������̽� â �ߺ� �������� ����, �������� ����Ͽ� ����
	int GetINFCnt();	//�������̽� â �ߺ� �������� ����
	void SetINFCnt(int nInfCnt);
	// end 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����

	// 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����
	void SetItemNum(INT nItemNum);
	// end 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����

	// 2012-12-21 by bhsohn ��Ʈ�� ���� ���� �ȵǴ� ���� �ذ�
	void SetClientLogOff(BOOL bClientLogOff) { m_bClientLogOff = bClientLogOff; }
	BOOL GetClientLogOff() const { return m_bClientLogOff; }
	// END 2012-12-21 by bhsohn ��Ʈ�� ���� ���� �ȵǴ� ���� �ذ�

	// 2013-03-25 by bhsohn ��Ʈ�� ŰƮ �ڵ� ��� ���� ���� ���� ����
	DWORD GetGameCurrentTime() const { return m_dwCurrentTime; }
	// END 2013-03-25 by bhsohn ��Ʈ�� ŰƮ �ڵ� ��� ���� ���� ���� ����

	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
	MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES*	GetInflConVictorites() { return &m_structInflConVictorites; }
	void	SetInflConVictorites(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES* pMsg) { memcpy(&m_structInflConVictorites, pMsg, sizeof(MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES)); }
	// END 2013-05-07 by bhsohn ��������Ʈ ������ �ý���

	float GetPVPBuffPercent() const { return m_fPVPBuffPercent; }		// 2013-08-02 by ssjung ������ ���� ����ǥ��

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	void LookChangeLimitTimeMSG();	// 2013-05-09 by ssjung ���� ���� �Ⱓ���� ��� �޽��� ����
#endif
	BOOL GetInitFlag() const { return m_bInitFlag; }									// 2013-07-02 ������ ó�� �ε� �� �ź��� ä�� ���̴� ���� ����
	void SetInitFlag(BOOL bFlag) { m_bInitFlag = bFlag; }						// 2013-07-02 ������ ó�� �ε� �� �ź��� ä�� ���̴� ���� ����

	// 2015-09-26 Future, global method to end the game properly (logOff = whether its a log off (true) or game quit (false))
	void EndGame(BOOL logOff);
	void SendFieldSocketCharacterGameEnd(BOOL logOff);

	// 2015-10-16 Future, added extra handler for staffs when they login
	void OnStaffLogin(USHORT Race);

	// 2015-10-16 Future, staffs can login twice
	void SetMonoMutexInstance(HANDLE instance);
	HANDLE GetMonoMutexInstance();

public:
	// 2004-11-11 by jschoi ���ǵ��� ���� ����
	int							m_nSendMovePacketCount;	// MovePacket�� ���� ��
	int							m_nCheckSpeedHackCount;	// ���ǵ��� üũ ī��Ʈ 1�ʴ� 5��
	DWORD						m_dwCheckSpeedHackTime;		// ���ǵ��� üũ Ÿ�� timeGetTime()
	DWORD						m_dwSpeedHackTimer;
	DWORD						m_dwHackCheckTime;		// 2005-12-21 by ispark, �� üũ Ÿ��(1�д�)
	DWORD						m_dwMoveCheckTime;		// 2006-01-10 by ispark, ���� üũ Ÿ��(1�д�)
	CD3DHanFont		*			m_pFontDebug;				// �ؽ�Ʈ ǥ�ÿ� ��Ʈ
	CD3DHanFont		*			m_pFontCandidate[CHAR_INF_CANDIDATE];				// ����Ʈ
	// 2007.04.24 by bhsohn China IME Working
	CD3DHanFont		*			m_pFontCompositon;
	CD3DHanFont		*			m_pFontIMEType;

	CKeyBoardInput	*			m_pKeyBoard;				// KeyBoard ����
	CCamera			*			m_pCamera;					// Camera ����

	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	CJoyStickInput*				m_pJoyStick;				// ���̽�ƽ ����
	CJoyStickFreedback*			m_pJoyStickFreedback;				// ���� ���̽�ƽ ����	
	// end 2008-11-13 by bhsohn ���̽�ƽ �۾�

	CUnitRender		*			m_pUnitRender;
	CCharacterRender*			m_pCharacterRender;			// 2005-07-13 by ispark ĳ���� ������
	CShuttleChild	*			m_pShuttleChild;			// ��Ʋ Data
	CCharacterChild *			m_pCharacterChild;			// ĳ���� Data// 2005-07-21 by ispark

	DWORD						m_dwStateBlock;				// 
	DWORD						m_dwGameState;
	LPDIRECT3DVERTEXBUFFER9		m_pVBSleep;
	BOOL						m_bBazaar;					// ���ڸ�
	// 2012-07-17 by jhahn, �˴ٿ� �ð� ǥ��
	BOOL						FristSeting;
	int							nCount;
	int							nOldTime;
	int							nNewTime;
	//end 2012-07-17 by jhahn, �˴ٿ� �ð� ǥ��

	CD3DHanFont		*			m_pFontInput;
	IMEKey						m_inputkey;
	CChat			*			m_pChat;
	BOOL						m_bChatMode;
	BOOL						m_bFixUnitDirMode;				// ���� ������� ���� ���
	BOOL						m_bMessageBox;					// �޽��� �ڽ��� ȭ��� �����ϴ°�?
	char						m_strChat[SIZE_MAX_CHAT_MESSAGE];

	CFieldWinSocketManager *	m_pFieldWinSocket;				// �ʵ� ����

	BOOL						m_bReconnectFieldServer;		// �ٸ� IP �������� ������

	CIMSocketManager		*	m_pIMSocket;			// ä�� ����


	char						m_strUserID[SIZE_MAX_ACCOUNT_NAME];
	char						m_strUserPassword[SIZE_MAX_PASSWORD_MD5_STRING];
	UINT						m_accountUniqueNumber;
	char						m_strFieldIP[SIZE_MAX_IPADDRESS];
	char						m_strFieldIPCurrent[SIZE_MAX_IPADDRESS];
	INT							m_nFieldPort;
	char						m_strChatIP[SIZE_MAX_IPADDRESS];
	INT							m_nChatPort;

	BOOL						m_bOtherFieldWarping;
	USHORT						m_sWarpAreaIndex;

	INT							m_nInvalidClientIndexCont;		// Ÿĳ���� ����Ʈ�� �ε����� 0 �� ����Ʈ ��
	///////////// Thread ////////////////
	CRITICAL_SECTION			m_cs;							// ����ȭ ��ü
	///////////// Tile Texture Buffer /////////////
	CGameData		*			m_pData;						// ����Ÿ���� (��Ÿ������)

	D3DXVECTOR3					m_vClientOldPos;				// Move ���� �����ÿ� ������ �Ÿ��� üũ�ϱ����� ���� ��Ʋ��ǥ ����
	///////////// Render ���� /////////////
	INT							m_nCheckRenderPos;				// ������ �˻縦 �ؾ��� �κ��� üũ�ϴ� ����
	FLOAT						m_fRenderDeleteCheckTime;		// ������� ���� ���ҽ��� ���� �ð� üũ 
	BYTE						m_nItemStorage;					//������â, â�� ����
	/////////////////////////////////////////////////////////////////////////
	// Effect
	CEffectRender	*			m_pEffectRender;//by dhkwon	2003.5.13
	/////////////////////////////////////////////////////////////////////////
	FLOAT						m_fCheckSocketTime;				// Socket Check Time
	FLOAT						m_fOldTickTime;					// ���� ElapsedTime
	FLOAT						m_fReturnKeyTime;				// Return Key Check Time

	CAtumNode		*			m_pEffectList;

	/////////////////////////////////////////////////////////////////////////
	BOOL						m_bDebugText;
	/////////////////////////////////////////////////////////////////////////
	BOOL						m_bClientQuit;
	// 2012-12-21 by bhsohn ��Ʈ�� ���� ���� �ȵǴ� ���� �ذ�
	BOOL						m_bClientLogOff;
	// END 2012-12-21 by bhsohn ��Ʈ�� ���� ���� �ȵǴ� ���� �ذ�

	BOOL						m_bResCheck;
	BOOL						m_bReadyBeginScene;

	BOOL						m_bDeleteItemSend;			// Item �� ������ ������ ����

	CInterface				*	m_pInterface;
	BOOL						m_IsFullMode;					// â�������
	CAbuseFilter	*			m_pAbuseFilter;
	CGameData		*			m_pAbuseFilterData;

	FLOAT						m_fBattleCountCheckTime;	// ��Ƽ ���� ī��Ʈ �ٿ� ���� ���� ����
	INT							m_nBattlePartyCount;
	INT							m_bDegree;					// 0 �����, 1 �߻�� , 2 ����, 3 �浹ó��(����)
	INT							m_bMovePlayer;				// ������ �÷��� ����

	char						m_strDebug[512];			// ������ ����׿� ���� �޼���
	FLOAT						m_fUserCheckTime;
	char						m_strSeverUserNum[128];
	char						m_strMapUserNum[128];

	// 2012-11-07 by bhsohn ��Ʈ�� ��Ŷ ó��
	FLOAT						m_fServerLiveTime;
	// END 2012-11-07 by bhsohn ��Ʈ�� ��Ŷ ó��

	// remodeling, 2004-03-19 by dhkwon
	CAtumSound					*m_pSound;
	CSceneData					*m_pScene;
	CAtumDatabase				*m_pDatabase;

	BOOL						m_bReadyToGetGuildInfo;

	// 2007-01-07 by bhsohn ��� ���� ����ȭ ���� ���� ó��
	BOOL						m_bReadyFieldGameStartOk;

	CINFStageEffect	*			m_pStageEffect;
	CQuestData		*			m_pQuestData;
	CFxSystem *					m_pFxSystem;
	BOOL						m_bRequestEnable;		// ������ ��û �� �� �ִ� �����ΰ�?
	BOOL						m_bInputItemDone;		// 2007-07-16 by dgwoo ó�� ���Խ� ��� �������� �޾Ҵ°�?
	float						m_fRequestEnableTime;	// �޽��� �ִϸ��̼��� ���� �ð� ����
	DWORD						m_dwTargetting;			// 2005-03-25 by jschoi - Ÿ���� ���� 
	CTutorialSystem*			m_pTutorial;			// Ʃ�丮�� �ý���
	CInputManager*				m_pInputManager;

	// ĳ����
	BOOL						m_bCharacter;			// TRUE : ĳ���͸��, FLASE: �����

	// 2005-08-10 by ispark
	// �׸��� ������ ��ġ�� ���� ���� üũ (�׷��� ī�忡�� ������ ���� ����)
	int							m_nDephbias;

	// �Ƹ� Į���� �������ϱ� ���� ��������
	int							m_nArmorNum;		// ���� ��Ʋ Į��
	int							m_nArmorNumMax;		// �ƸӰ� ������ �ִ� �ִ� Į���

	D3DXVECTOR3					m_vShuttleOldPos;		// �� ��ǥ

	// 2010. 10. 05 by jskim �ʷε� ���� ����
	CMapLoad*					m_pMaploading;
	// end 2010. 10. 05 by jskim �ʷε� ���� ����
#ifdef _DEBUG
	// �̺�Ʈ ������Ʈ ����
	BOOL						m_bEventObjectRender;	// TURE : ����, FALSE : �Ұ���
#endif

	// IME���� �ĺ�����Ʈ ����
	vector<string>	m_vecStingCandidate;
	int				m_nGlabalImageCase;
	char			m_strFont[64];

	/*--------------------------------------------------------------------------*/
	// �����͸� ���� �ð����� ������ ���� ����
	vectorClientIndex			m_vecCharacterOtherInfo;
	vectorClientIndex			m_vecCharacterOtherInfoRequest;
	vectorClientIndex			m_vecCharacterOtherMove;
	vectorClientIndex			m_vecCharacterOtherRenderInfo;
	vectorClientIndex			m_vecCharacterOtherRenderInfoRequest;
	vectorClientIndex			m_vecMoveTarget;

	vector<INT>					m_vecMapObjectInfo;
	vector<INT>					m_vecMonsterInfo;
	vector<INT>					m_vecSimpleItemInfo;
	vector<INT>					m_vecItemInfo;
	/*--------------------------------------------------------------------------*/

	/*--------------------------------------------------------------------------*/
	// 2006-07-05 by ispark, ���� ����� ó���ϴ� �κ�
	BOOL						m_bShutDown;				// ���� ����?
	// 2007-07-10 by bhsohn ��� Ÿ�� ������ ���� ó��
	//char						m_strMsgLastError[SIZE_MAX_CHAT_MESSAGE];
	char						m_strMsgLastError[SIZE_MAX_ERROR_CHAT_MESSAGE];

	BOOL						m_bGameShutDown;		// 2006-08-04 by ispark, ���� ���� ����
	float						m_fGameShutDownTime;	// 2006-08-04 by ispark, ���� ���� ���� �ð�
	/*--------------------------------------------------------------------------*/

	BOOL						m_bCheckQuickSlotSave;

	// 2007-07-27 by dgwoo ���� ������ �� �����ΰ�?
	BOOL						m_bInfregular;

	vector<MSG_FC_TRADE_INSERT_ITEM>			m_vecTutorialInsertItem;
	vector<MSG_FC_TRADE_UPDATE_ITEM_COUNT>     m_vecTutorialUpdateItem;

	vector<CItemInfo*>							m_vecWearDisplayInfo;		// 2012-06-14 by isshin ���۹̸�����

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	CInfinityManager*			m_pInfinityManager;		// ���Ǵ�Ƽ UI���� ����

	enum ARENASOCKET_TYPE { ARENASOCKET_ARENA, ARENASOCKET_INFINITY, ARENASOCKET_NONE };

	ARENASOCKET_TYPE			m_ArenaSocketType;		// �Ʒ��� ���� Ÿ��

	BOOL						m_bInfinityMapload;		// ���Ǵ�Ƽ �� �ε� ���ΰ�?(���Ǵ�Ƽ �� �ε��� ������ ������ �ö����� ����Ѵ�)

	BOOL						m_bEnterInfinityAllowed;// ���Ǵ�Ƽ �ʿ� ���� �㰡(��� ��Ƽ������ �ʷε��� �� ��������?TRUE)

	BOOL						m_bInfinityRestart;		// ���Ǵ�Ƽ ƨ������ �������ΰ�?
	BOOL						m_bAddInfinityRestartMessageBox;	// ���Ǵ�Ƽ ������ �޼��� �ڽ��� �����ΰ�?
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���


	// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	BOOL						m_bWaitInfinityMapLoad;	// ���Ǵ�Ƽ �� �ε��� ��ٸ��� ���ΰ�?

	DWORD						m_nInfinityMapLoadTime;	// ���Ǵ�Ƽ �� �ε��� ������ �ð�

	MSG_FC_ARENA_POSSIBLE_GAMESTART	m_ArenaStartPacket;	// ���Ǵ�Ƽ �� �ε��� ����� ��Ŷ ����
	// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)


	// 2010. 03. 23 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���Ǵ�Ƽ �ʵ� ���� ĳ��������)
	BOOL						m_bInfinityMapLoadedSendEnable;
	BOOL						m_bInfinitySkillSetupOKDone;
	// end 2010. 03. 23 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���Ǵ�Ƽ �ʵ� ���� ĳ��������)


	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	DWORD							m_dwTimeCheck;
	vector<structLoadingGameData*>	m_vecLoadingGameData;
	CMeshInitThread*				m_pMeshInitThread;
	structLoadingGameData*			m_pLoadingGameData;
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

	// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageList*				m_pImageList;
	// end 2011. 10. 10 by jskim UI�ý��� ����

	bool		b_CanadaMutex;// 2012-09-17 by jhahn	ĳ���� �ٽ��� ���� ����

#ifdef _WIREFRAME
	bool		m_bWireframe; //2014-09-03 St0rmy wireframe
#endif
#ifdef _NOCLIP
	bool		m_bNoclip; //2014-09-03 St0rmy NoClip
#endif
#ifdef _DRAW_EVENTS
	bool		m_bDrawEvents; //2015-01-29 by St0rmy, Draw Events
#endif
#ifdef _SHOW_LATENCY
	float		m_fLatency;
#endif

private:
	BYTE						m_nDBSid;
	// PK�� �� ���ֿ��� �׾���? TRUE : �� ���ֿ��� �׾���.
	BOOL						m_bDeadByP2PPK;
	MSG_FC_CHARACTER_CASH_PREMIUM_CARD_INFO		m_PremiumCardInfo;
	// 2007-02-12 by bhsohn ��Ŀ�� �Ҿ� �Ծ��� ���� ���� ó��	
	FLOAT						m_fCheckElapsedTime;
	DWORD						m_dwCurrentTime;
	// 2007-02-12 by bhsohn Item ���� ���� ó��
	// �Ǹ� ����
	deque<stMultiSelectItem>	m_vecSellMultiSelectItem;
	deque<stMultiSelectItem>	m_vecInvenStoreMultiSelectItem;
	deque<stMultiSelectItem>	m_vecCityStoreMultiSelectItem;
	BOOL						m_bCtrlBtnClick;
	BOOL						m_bStartMultiSell;
	BOOL						m_bStartMultiStore;
	// 2007-02-28 by bhsohn ���� �ǸŽ� 0 spi�� ���� ó��
	stMultiSelectItem			m_struLastSelMultiItem;
	// 2007-04-09 by bhsohn ������ ���� ǥ�� ���� ó��
	char						m_strConnectSeverName[1024];
	// 2007.04.24 by bhsohn China IME Working
	char						m_szReadingString[1024];
	char						m_szImeString[1024];
	// 2007-05-21 by bhsohn China IME Working
	FLOAT						m_fIMETypeShowTime;
	vector<MSG_FC_ARENA_GM_COMMAND_INFO> m_vecTeamInfo;			// GM��ɾ� /�Ʒ���.
	// 2007-07-10 by bhsohn ��� Ÿ�� ������ ���� ó��
	int							m_nPreComType;
	int							m_nPreIMComType;
	// 2007-07-24 by bhsohn �����̼� mp3�߰�
	char				m_chNarrationSound[256];
	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	char				m_chErNarrationSound[256];
	// 2007-08-03 by bhsohn ĳ���� ������Ʈ üũ�� ����
	vector<int>					m_vecSendCharacterOmiInfo;
	// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
	BYTE			m_bIsUseSecondaryPasswordSystem;	// �ý����� ������� ����
	BYTE			m_bIsSetSecondaryPassword;		// ��ȣ�� �����ߴ��� ����
	// 2007-10-30 by dgwoo ������ ����� �ð��� �������� �ѹ��� �������ش�.
	BOOL			m_bResortingItem;
	// 2007-11-20 by bhsohn �ʷε� �ϴ� ��� ����
	stGameStartInfo		m_struGameStartInfo;
	BOOL				m_bGameStartInfo;
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	CArenaManager*		m_pArenaManager;
	// 2007-12-21 by bhsohn ���α׷� ���� �� ���� ����	
	stMultiLockInfo		m_vecLockProtocol;

	// 2008-01-11 by bhsohn 1�е��� ������ Ŭ���̾�Ʈ ���߰� �ϸ� Ŭ���̾�Ʈ ���� ����
	DWORD				m_dwLastestTickTime;		// Tick�� ���������� 	
	// 2008-02-15 by bhsohn ��Ʈ��ũ �ӵ� üũ
	structNetCheckInfo	m_stNetCheckInfo;
	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	float				m_fMouseLockTime;
	BOOL				m_bWndMoving;
	int					m_nOnFriendCount;
	// 2008-07-07 by bhsohn 2�� �н����� Ȱ��ȭ��, �κ� �ȿ����� ��
	BYTE				m_bSecondPassLock;
	// 2008-08-11 by bhsohn �ܵ����� ���� �ǰԲ�
	CSinglePlayGame*	m_pSinglePlayGame;
	// 2008-08-18 by bhsohn 1�� ���� ������ �̵� �ý��� 	
	DWORD				m_dwSendTermTime;
	BOOL				m_bSendTerm;
	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	float				m_fJoyStickCheckTime;
	float				m_fJoyStickFeedbackPlayTime;	// �����ð�
	int					m_nJoyStickFeedbackType;		// ����Ÿ��	
	int					m_nJoyContorlLen;
	char				m_chJoystickCtrlList[MAX_JOYCTL_LIST][MAX_PATH];
	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	CWorldRankManager*		m_pWorldRankManager;
	// 2009-04-08 by bhsohn ���� ���ý�, ����ũ �� ��æƮ �� ������ �ǸŽ� ��� �޽��� ��� ��ȹ ����(K0000174)
	stMultiSelectItem		m_stMultiWarningSelItem;
	// 2009. 06. 09 by ckPark OMI ���� ���۽� üũ�� ����ϵ��� ����
	BYTE					m_nOMICheckSum[32];
	long					m_nOMIFileSize;
	// 2010. 03. 12 by ckPark Interface.tex üũ�� �߰�
	BYTE					m_nInterfaceCheckSum[32];
	long					m_nInterfaceFileSize;
	// 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
	int m_nINFCnt;				// �������̽�â �ߺ� ������
	// 2012-11-29 by mspark, ��ġ���� ��ġ�� �̵��ϴ� ĳ������ ��ġ�� �����ϵ��� ����
	INT						m_nItemNum;
	//2013-04-11 by ssjung ���� �غ������ �� �� �̵��� �ٽ� �����÷��̾� �Ǵ� ��Ȳ����
	BOOL m_bBossMonsterSummeryReady;
	BYTE m_byInfluence;
	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
	MSG_FC_WAR_INFLUENCE_CONSECUTIVE_VICTORITES		m_structInflConVictorites;
	// 2013-07-02 ������ ó�� �ε� �� �ź��� ä�� ���̴� ���� ����
	BOOL				m_bInitFlag;
	// 2013-07-05 by bhsohn ���������, ����� �ý���
	float				m_fServerLockTime;
	// 2013-08-02 by ssjung ������ ���� ����ǥ��
	float				m_fPVPBuffPercent;
	// 2015-07-18 Future, GDI+ for ScreenShot Conversions
	ULONG_PTR m_GdiplusToken;
	CScreenShotManager*	m_pScreenShotManager;

#ifdef C_DISCONNECT_INACTIVE_PLAYERS
	float	m_fLastActivity;	// 2015-09-25 Future, kicking of inactive players
#endif

	// 2015-10-16 Future, staffs can login twice
	HANDLE	m_MonoMutexHandle;

#ifdef _ADVANCED_CONFIG
public:
	//St0rmy: Advanced Configuration Variables
	BYTE				m_nAdvConfRenderLevel; // 1, 2, 3, 4
	BYTE				m_nAdvConfAntiAliasing;// 0, 2, 4, 8
	BYTE				m_nAdvConfAntiAliasQuality;//1, 2, 3, 4 (Only on 4 samples): 5, 6, 7, 8
	BOOL				m_bAdvConfBufferOptimize; //TRUE, FALSE
	BYTE				m_nAdvConfTextureFilter;//1, 2, 3, 4 -> None, Point, Linear, Anisotropic
	BYTE				m_nAdvConfAnisotropicLevel;//1-16
	BOOL				m_bAdvConfVSync;//TRUE, FALSE
	BYTE				m_bAdvConfAffinity;//0, 1, 2
	void				ReadAdvancedConfig();
#endif

public:
	BOOL				m_bVSync;	// 2015-12-16 Future, optional VSync
	INT					m_nPVPoption = 0;
};

// ������������
extern CAtumApplication*		g_pD3dApp;
extern LPDIRECT3DDEVICE9		g_pD3dDev;	// ���� ����̽� PTR
extern sOPTION_SYSTEM*			g_pSOption;
extern sOPTION_CHARACTER*		g_pSOptionCharacter;
extern int						g_nMoveCountPerRate;
extern float					g_fMoveCountFrame;
// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
extern sOPTION_ETC				*g_pSOptionEtc;

// 2008-11-13 by bhsohn ���̽�ƽ �۾�	
extern structJoyStickOptionInfo*		g_pJoysticOp;
// end 2008-11-13 by bhsohn ���̽�ƽ �۾�

#ifdef NEMERIAN_NATION_BALANCE_BUFF_PENALITY
extern float					g_buffPenality;
#endif

extern CSceneData				*g_pScene;
extern CShuttleChild			*g_pShuttleChild;
extern CCharacterChild			*g_pCharacterChild;		// 2005-07-21 by ispark
extern CAtumDatabase			*g_pDatabase;
extern CInterface				*g_pInterface;
extern CINFGameMain				*g_pGameMain;
extern CStoreData				*g_pStoreData;
extern CQuestData				*g_pQuestData;
extern CCamera					*g_pCamera;
extern CBackground				*g_pGround;

extern CFieldWinSocketManager	*g_pFieldWinSocket;
extern CIMSocketManager			*g_pIMSocket;

extern CINFSelect				*g_pSelect;
extern CINFChannelList			*g_pChannelList;
extern CFrustum					*g_pFrustum;
extern float					g_fSendMovePacket;
extern BOOL						g_bDetailDrawFrame;		// ���� �ø� �ɼ�
extern CTutorialSystem			*g_pTutorial;			// Ʃ�丮��
extern CInputManager			*g_pInputManager;
extern CInput					g_input;
extern int						g_nRenderCandidate;		// IME���� �ĺ�����Ʈ ��� ��ġ
extern CGlobalImage				*g_pGImage;
extern BOOL						g_bEventRecovery;		// �̺�Ʈ ȸ���� ���� Flag
extern float					g_fEventRecoveryRate;	// �̺�Ʈ ȸ����
extern ATUM_DATE_TIME			g_ServerDateTime;		// 2006-10-11 by ispark, ���� �ð�
extern int						g_GameStartTime;		// 2006-10-11 by ispark, ���� ���� �ð�

extern ATUM_DATE_TIME			g_ArenaServerDateTime;
extern int						g_ArenaStartTime;
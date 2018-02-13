// Interface.h: interface for the CInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACE_H__6912F02E_DF5A_4009_BE9A_D988E4C7386B__INCLUDED_)
#define AFX_INTERFACE_H__6912F02E_DF5A_4009_BE9A_D988E4C7386B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

// 2008-02-27 by bhsohn �Ʒ��� �̵��� ä������ ����
#include "ChatTab_t.h"

// ���� ������â �켱���� ���� ����ü
typedef struct  {
	int		nWindowIndex;		// ������ �ε���
	int		nWindowOrder;		// ������ ó�� �켱����
} GameWindowWnd;


class CINFTarget;
class CINFGameMain;
class CINFPilotFace;
class CINFSelect;
class CINFMapLoad;
class CINFImage;
class CINFCityBase;
class CINFChannelList;
class CINFFadeEffect;
class CINFStageEffectData;
class CINFMapName;
class CGlobalImage;
class CINFCityBazaar;
class CINFCityBazaarOpen;
class CINFCityBazaarVisit;
class CINFGameArena;

// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
class CINFInfinity;
// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
class CINFCityInfinityFieldPopUp;
// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)


// 2007-07-04 by bhsohn ���۷����� �߰�
class CINFOpMain;
// 2007-07-23 by bhsohn ���۷����� �߰��۾�
class CSetupConfig;

class CINFToolTip;

class DataHeader;	// 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�

// 2007-07-04 by bhsohn ���۷����� �߰�
// ���۷����� ���
#define	OPERATOR_USER_HELPER_NONE			0
#define	OPERATOR_USER_HELPER_FREESKA	1
#define	OPERATOR_USER_TUTORIAL_COMPLETE		2
#define	OPERATOR_USER_TUTORIAL_FAIL			3
#define	OPERATOR_USER_TUTORIAL_HELPER		4
#define	OPERATOR_USER_TUTORIAL_ALL_COMPLETE	5
#define	OPERATOR_USER_SEL_VCU				6
#define	OPERATOR_USER_SEL_ANI				7
#define	OPERATOR_USER_TUTORIAL_NOT_L10		8



class CInterface : public CAtumNode  
{
public:
	CInterface();
	virtual ~CInterface();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void Render();
	virtual void Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
//	void OnButtonClick(int nButton);
	
	HRESULT InitGameObjects();
	HRESULT RestoreGameObjects();
	HRESULT InvalidateGameObjects();
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	//HRESULT DeleteGameObjects();
	HRESULT DeleteGameObjects(BOOL bArenaMode);

	HRESULT InitSelectObjects();
	HRESULT RestoreSelectObjects();
	HRESULT InvalidateSelectObjects();
	HRESULT DeleteSelectObjects();

	HRESULT	InitMapLoadObjects();
	HRESULT RestoreMapLoadObjects();
	HRESULT InvalidateMapLoadObjects();
	HRESULT DeleteMapLoadObjects();
	
	HRESULT	InitCityObjects();
	HRESULT RestoreCityObjects();
	HRESULT InvalidateCityObjects();
	HRESULT DeleteCityObjects();
	
	HRESULT	InitChannelListObjects();
	HRESULT RestoreChannelListObjects();
	HRESULT InvalidateChannelListObjects();
	HRESULT DeleteChannelListObjects();

	void InitCharacterData();
	void InitOptionData();
	BOOL SaveCharacterFile(int SlotPurpose);	// 2012-06-21 by jhseol, �Ʒ��� �߰�����part2 - �Ʒ��� ���� ������ �Ķ���� �߰� ( 0 = ����, 1 = �Ʒ��� )
	BOOL SaveOptionFile();
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	//BOOL LoadCharacterFile();
	BOOL LoadCharacterFile(BOOL bArenaRestart);
	
	BOOL LoadOptionFile();

	HRESULT InitSoundScript(char* szFileName, STAGE_EFFECT_DATA * pStageEffectData = NULL, BYTE byEndAct = NOT_ACTION, BOOL bSkip = TRUE);
	HRESULT RestoreSoundScript();
	HRESULT InvalidateSoundScript();
	HRESULT DeleteSoundScript();

	// 2006-07-28 by ispark, ���λ���
	void OpenBazaarShop(ITEM* pSkillItem);
	void CloseBazaarShop();
	void ReadyCloseBazaarShop(BOOL bErr = FALSE);
	void VisitBazaarShop();
	void SetBazaarLogInfo(BYTE byLogState);						// �α�
	void ErrBazaarItemList();
	BOOL SendBazaarRequestItemList(CEnemyData * pEnemy, BOOL bFlag);

	void SetShutDownStart();									// 2006-08-04 by ispark, ���� ���� ����

	void ScriptEndAct(BYTE byEndAct);							// 2006-09-07 by ispark, �������� ����Ʈ ������ �ؾ��ϴ� �ൿ

	// 2007-05-11 by bhsohn �Ʒ��� ���׼���
	BOOL IsArenaStart();

	// 2007-07-09 by bhsohn ��ݰ� ���ڻ��� ���� ���� ������ ó��
	BOOL IsBazarOpen();

	// 2007-07-04 by bhsohn ���۷����� �߰�
	DataHeader * FindResource_LoadOp(char* szRcName);
	void ShowOpUserHelper(BOOL bShow, int nHelpMode, char* pTxt);


	// 2009. 01. 12 by ckPark ���� ���� �ý���
	// ��������� ���۷�����
	void ShowOpWarDeclare(BYTE Influence, ATUM_DATE_TIME MSWarStartTime, BOOL GiveUp, BYTE SelectCount);

	// end 2009. 01. 12 by ckPark ���� ���� �ý���


	void ShowOpMissionComplete(INT i_nExpOfCompensation, BYTE i_byBonusStatOfCompensation, vector<QUEST_PAY_ITEM_INFO> vecQuestPay);
	void AddInflWarMonster(char *pTxt, MapIndex_t	MapIndex, SHORT	MapInfluenceType, ATUM_DATE_TIME		CreateTime);
	void ShowOpBossMonsterSummon(BYTE byBelligerence, int nHour, int nMinute);
	void ShowOpFirstInfluenceQuest(BYTE InfluenceType);

	// 2007-07-23 by bhsohn ���۷����� �߰��۾�
	int GetOperatorMode();
	void SetOperatorMode(int nMode);
	void CloseFirstUserExplain();
	void DetroySummonMonster(MapIndex_t	MapIndex);

	// 2007-07-27 by bhsohn ���� �߰� �� ���� ���۷����Ϳ� ����
	VOID ShowOpWndTxt(char* pTxt);
	BOOL IsBuildingShow();
	void ShowOpWnd(BOOL bShow);

	// 2007-08-07 by bhsohn ����Ŀ ������ �߰�
	int GetSpeakerMode();
	void SetSpeakerMode(int nMode);

	// 2007-09-12 by bhsohn 2�� ��ȣ �ý��� ����
	DataHeader * FindResource_LoadSelect(char* szRcName);

	// 2007-11-01 by bhsohn ���� �̿��߿� ���� ó��
	void CloseVisitShop();

	// 2007-11-19 by bhsohn ���۷����� ������ �ؽ�Ʈ ó��
	VOID ShowOpWndMultiTxt(vector<string> *pvecEditText);

	// 2007-12-07 by dgwoo ���ܸ�ũ�� ���ŵǾ����� �� �����ϴ� �Լ�.
	BOOL SetGuildMark(UID32_t i_nGuildUID);

	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	BOOL IsArenaReady();
	BOOL IsArenaGameState(int nState);

	// 2008-02-27 by bhsohn �Ʒ��� �̵��� ä������ ����
	void BackupChatInfo();		// ������ �޴´�.
	void RefreshChatInfo();		// ä�� ������ �����Ѵ�.

	// 2008-03-07 by bhsohn ��������, �Ʒ��� �̿� ���ϰ� ����
	BOOL IsDoingStrategyWar();

	// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	void	UpdateShoptPlayingSPWarInfo();
	void	RefreshWarSPWarInfo();			// ������ 
	void	RefreshWarMotherWarInfo();		// ���� 

	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	int		GetEtcOptionInfo(int nIdx);


	// 2009. 02. 03 by ckPark ä��â �������̽� ũ�� ����
// 	void	GetEtcOptionInterPosValue(float* fRateX, float* fRateY, 
// 										   float* fRateWidth, float* fRateHeight, 
// 										   float* fGameWidth, float* fGameHeight);
	void	GetEtcOptionInterPosValue(float* fRateX, float* fRateY, 
							          float* fRateWidth, float* fRateHeight, 
									  float* fGameWidth, float* fGameHeight,
									  float* fChatWidth, float* fChatHeight);
	// end 2009. 02. 03 by ckPark ä��â �������̽� ũ�� ����


	void	SetEtcOptionInfo(char* pSetupInfo, int nMode);
	void	GetEtcOptionIdx_To_String(int nIdx, char* pDst);
	void	LoadOptionEtcInfo(sOPTION_ETC	*pEtcOption);
	void	SetOptionEtcInfo(sOPTION_ETC	*pEtcOption);
	void	SaveOptionEtcInfo();
	BOOL	GetShowInterface();
	void	SetShowInterface(BOOL bShowInterface);


	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	void	InitJoystickInfo(char* pFilePath);
	void	LoadJoystickInfo(char* pFilePath);
	void	GetJoystickInfo(int i_nOptionIdx, int i_nSlotIdx, int*	o_nKeyIdx, int* o_nKeyValue);
	void	LoadFileJoysticInfo(CSetupConfig*	i_pJoySticConfig, structJoystickSetupKeyTable	*o_pJoystickSetupKeyTable);	// �ɼ����� ���� ��ħ
	void	RefreshJoystickOption();
	void	SetJoysticInfoBehavior(int i_nBehaviorIdx, int i_nJoyKeyIndex, int i_nJoyKeyValue);
	int		GetJoysticInfoString_To_Idx(char* i_pTitle);
	int		GetCullingString(int nPos, char* i_pString, char i_chFind, char* o_pDst);
	BOOL	GetJostickStats(int code);
	void	InitJoystickSetupInfo(structJoystickSetupKeyTable		*o_pJoystickSetupKeyTable);
	BOOL	IsChangeJoySitckInfo(int code);
	void	GetJoysticInfoIdx_To_String(int nIdx, char* o_pDst);
	BOOL	OnJoystickKeyDown(int i_nCode);
	void	LoadJoysticOptionInfo(structJoyStickOptionInfo* pJoysticOp);
	void	SaveJoystickOptionInfo(structJoyStickOptionInfo* pJoysticOp);
	structJoystickSetupKeyTable	 *GetJoystickSetupKeyTable(int nKeyIndex);
	void	SaveJoysticSetupInfo(char* pJoyKeyFile, structJoyStickOptionInfo	*pstruJoyStickOptionInfo, structJoystickSetupKeyTable	*i_pJoystickSetupKeyTable);
	

	// 2010-06-15 by shcho&hslee ��ý��� - �� Ÿ�� String�� ����.
	char *GetString_PetType ( const INT a_nPetType );

	void ResetMapNameList();

	// 2013-06-26 by bhsohn �Ƹ� �÷��� �߰� ����
	BOOL GetSetupInfo(char* i_pTitle, char* o_pTxt);
	void SetSetupInfo(char* i_pTitle, char* o_pTxt);
	// END 2013-06-26 by bhsohn �Ƹ� �÷��� �߰� ����
	
public:
	CINFTarget			*	m_pTarget ;
	CINFGameMain		*	m_pGameMain;
	CINFPilotFace		*	m_pPilotFace;
	CINFMapName			*	m_pMapNameImg;
	BOOL					m_bShowInterface;
	CGlobalImage		*	m_pGameGlobalImage;

	CINFSelect			*	m_pSelect;
	D3DLIGHT9				m_light0;
	D3DLIGHT9				m_light1;

	CINFMapLoad			*	m_pMapLoad;
	CINFImage			*	m_pNormalMouse[2];

	CINFCityBase		*	m_pCityBase;
	CINFChannelList		*	m_pChannelList;
	CINFFadeEffect		*	m_pFadeEffect;
	CINFStageEffectData *	m_pStageEffectData;		// ���� ��ũ��Ʈ
	
	CINFCityBazaar		*	m_pBazaarShop;			// 2006-07-25 by ispark, ���� ����
	CINFGameArena		*	m_pGameArena;

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	CINFInfinity		*	m_pInfinity;
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	CINFCityInfinityFieldPopUp*		m_pInfinityPopup;
	// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)

	// 2007-07-04 by bhsohn ���۷����� �߰�
	CINFOpMain			*	m_pINFOpMain;

	// 2007-07-23 by bhsohn ���۷����� �߰��۾�
	CSetupConfig*			m_pSetupConfig;	
	CINFToolTip	*			m_pToolTip;

	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	CSetupConfig*			m_pJoySticConfig;	
	structJoystickKey			m_struJoystickOption[MAX_JOSTICK_OPTION][MAX_JOSTICK_OPTION_BEHAVIOR];
	structJoystickSetupKeyTable		*m_pJoystickSetupKeyTable;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CGameData				*	m_pGameData;
	CGameData				*	m_pGameBigIconData;
#endif
	//////////////////////////////////////////////////////////////////////////
	// ���� ������ â �켱 ���� ������, �޼��� ó�� ����
	// ydkim 2005.10.28
	//
	// �޼��� �Է� ����ü ��� �켱������ �޽��� INDEX �Է�.
	// ����ü ������ Int�� ���� 2����� 
	// ���� �߰� �� ����
	// m_vecGameWindowWnd			������ ����(��������)
	// m_vecGameWindowWndReverse	������ ����(ó����)
	//////////////////////////////////////////////////////////////////////////
	vector<GameWindowWnd>	m_vecGameWindowWnd;
	vector<GameWindowWnd>	m_vecGameWindowWndReverse;
	void SetWindowOrder(int nWndIndex);
	int  GetWindowFirstOrder();
	void InitWindowsFirstFormat();

	int  WindowsWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void WindowsRender();

	// ���� ��ü Rendering
	void RenderOrderWindows(int nRenderNum);
	void RenderProcMapName();

	// ���� ��ü WndProc
	int WndProcGlob(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcGame(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcCity(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcBaza(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcInfW(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcMiss(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcFAQs(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//int WndProcVoIP(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcMini(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcMenu(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcMp3P(UINT uMsg, WPARAM wParam, LPARAM lParam);
	// 2008-07-10 by bhsohn �ý��� â �켱���� �߰�
	int WndProcSystemMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//
	//////////////////////////////////////////////////////////////////////////
	// 2007-07-04 by bhsohn ���۷����� �߰�
	int WndProcOperator(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	//////////////////////////////////////////////////////////////////////////	
	// ���� ������ ����˻� (���� ����)
	void SetShopItemUseSerch();
	void SetLeaveShopKind(int nLeaveBuildingIndex);
	//
	//////////////////////////////////////////////////////////////////////////

	// 2007-10-01 by bhsohn ó�� Ʃ�丮�� ����� ���ѷα� �ȳ����� ���� ó��
	void SendTutorialChartEnd();

	// 2007-11-28 by bhsohn ��ũ�� �� ��� �߰�
	BOOL IsScreenShotMode();
	void SetScreenShotMode(BOOL bScreenShotMode);

	// 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�
	void SetDummyDataHeader(DataHeader *pDataHeader);
	DataHeader *GetDummyDataHeader(char* i_pFileName);

protected:
	void GameShutDownTick();

	// 2007.04.24 by bhsohn China IME Working
	BOOL IsPermissionIME(HKL hkl);	

	// 2007-07-23 by bhsohn ���۷����� �߰��۾�
	void InitSetupInfo(char* pPath);

	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	void	RenderWndIntfaceHideMode();

	// 2008-10-27 by bhsohn �������� ���� ������ ���� ����
	void	ReleaseBazaarShop();

private:
	// 2007-11-28 by bhsohn ��ũ�� �� ��� �߰�
	BOOL		m_bScreenShotMode;

	// 2008-02-27 by bhsohn �Ʒ��� �̵��� ä������ ����
	ChatLine_t	m_chatBackup[CHAT_BUFFER_NUMBER];

	// 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�
	DataHeader			*m_pDummyDataHeader;
	// end 2008-10-15 by bhsohn ���ҽ� �޸� ��ȣ ��� �߰�
};


// 2010. 05. 12 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. (�ΰ��� ���� ��� �ܰ� ǥ��.)
BOOL IsElapsedTime ( DWORD dwTime , DWORD *p_dwElapsedTime );
// End 2010. 05. 12 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. (�ΰ��� ���� ��� �ܰ� ǥ��.)

// 2010. 05. 17 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. �ܰ� ǥ�� ���� ����.
BOOL GetStrPara ( char *pStrBuff , INT iNum , char pKey , char *pGetPara );
// 2010. 05. 17 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. �ܰ� ǥ�� ���� ����.

#endif // !defined(AFX_INTERFACE_H__6912F02E_DF5A_4009_BE9A_D988E4C7386B__INCLUDED_)
// INFGameMainUnitInfoBar.h: interface for the CINFGameMainUnitInfoBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINUNITINFOBAR_H__D89CEB23_A523_4B16_A8DD_203B1EEFA70F__INCLUDED_)
#define AFX_INFGAMEMAINUNITINFOBAR_H__D89CEB23_A523_4B16_A8DD_203B1EEFA70F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define GAMEMAIN_BACK_UP_PIXEL			0
#define GAMEMAIN_BACK_DOWN_PIXEL		1

#define GAMEMAIN_GAGE_HP_UP				0
#define GAMEMAIN_GAGE_DP_UP				1
#define GAMEMAIN_GAGE_BURN_UP			2
#define GAMEMAIN_GAGE_SP_UP				3
#define GAMEMAIN_GAGE_EP_UP				4
#define GAMEMAIN_GAGE_SPEED_UP			5
#define GAMEMAIN_GAGE_EXP_UP			6
//�߰� - ��������� ���ؼ�
#define GAMEMAIN_NEW_STRING_HP			7
#define GAMEMAIN_NEW_STRING_MAX_HP		8
#define GAMEMAIN_NEW_STRING_DP			9
#define GAMEMAIN_NEW_STRING_MAX_DP		10
#define GAMEMAIN_NEW_STRING_MAX_SP		11

#define GAMEMAIN_GAGE_NUMBER			9		
#define GAMEMAIN_GAGE_LINE_NUMBER		12		
#define GAMEMAIN_GAGE_LINE_LENGTH		32

#define GAMEMAIN_BUTTON_SHOW			0
#define GAMEMAIN_BUTTON_HIDE			1

#define GAMEMAIN_GAGE_HP_MAX_GRADE		20
#define GAMEMAIN_GAGE_DP_MAX_GRADE		20
#define GAMEMAIN_GAGE_SP_MAX_GRADE		20
#define GAMEMAIN_GAGE_BOOSTER_MAX_GRADE	18
#define GAMEMAIN_GAGE_FUEL_MAX_GRADE	17

#define GAMEMAIN_EVENT_RECOVERY_MAX		5

#define GAMEMAIN_POS_X					0		// 2011. 10. 10 by jskim UI�ý��� ����
#define GAMEMAIN_POS_Y					0		// 2011. 10. 10 by jskim UI�ý��� ����

//2015-08-15 by St0rmy, better Ping Display
#define PING_DISPLAY_POS_X					g_pD3dApp->GetBackBufferDesc().Width - 40
#define PING_DISPLAY_POS_Y					160

#define PING_DISPLAY_SIZE_X				24
#define PING_DISPLAY_SIZE_Y				24

#define PING_DISPLAY_COUNT				4		//Count of possible display options
#define PING_STATUS_GOOD				0		//Best one, green 4 bars
#define PING_STATUS_MEDIUM				1		//Medium one, yellow 3 bars
#define PING_STATUS_BAD					2		//Bad one, orange 2 bars
#define PING_STATUS_WORST				3		//Worst one, red 1 bar (point)

#define PING_LIMIT_GOOD					0.150f	//100 ms
#define PING_LIMIT_MEDIUM				0.250f	//200 ms
#define PING_LIMIT_BAD					0.350f	//300 ms


// 2006-08-29 by dgwoo ���� �̺�Ʈ ���� ����
struct EVENT_INFO
{//�߰��Ǵ� ��Ҹ� ������ �߰�...
	float			fEXPRate;
	float			fSPIRate;
	float			fEXPRepairRate;
	float			fDropItemRate;
	float			fDropRareRate;
	float			fWarPointRate;
	EVENT_INFO(){
		fEXPRate = 0;
		fSPIRate = 0;
		fEXPRepairRate = 0;
		fDropItemRate = 0;
		fDropRareRate = 0;
		fWarPointRate = 0;
	};
};
// 2007-11-28 by dgwoo �����޽��� ����ü.
#define	GIFT_ICON_IMG_COUNT				2				// ���� �̹��� ����.

struct GIFT_NOTIFY_MSG
{
	UID64_t		NotifyMsgUID;
	UID64_t		CharacterUID;
	char		NotifyMsgString[SIZE_MAX_NOTIFY_MSG_STRING];
};

class CINFImage;
class CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
class CINFImageBtn;
class CINFGroupImage;		// 2011. 10. 10 by jskim UI�ý��� ����
class CD3DHanFont;
class CINFGameMainUnitInfoBar : public CINFBase  
{
public:
	CINFGameMainUnitInfoBar(CAtumNode* pParent);
	virtual ~CINFGameMainUnitInfoBar();

	void			UpdateAllGage();
	void			SetHP(float fCurrent, float fMax);
	void			SetDP(float fCurrent, float fMax);
	void			SetBURN(float fCurrent, float fMax);
	void			SetSP(float fCurrent, float fMax);
	void			SetEP(float fCurrent, float fMax);
	void			SetSpeed(float fCurrent, float fMin, float fMax);
	void			SetEXP(float fExp);
	void			SetFame();
	void			SetHappyHourEventIconPos();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void			Render();
	void			RenderCityUpInfo();							// ���ÿ��� ���
	void			RenderSpeed();
	void			HideRender();								// GUI�� ������ ��Ȳ(FŰ)���� Render()
	void			RenderMainGage(BOOL bShowAll = TRUE);
	void			RenderGageString(BOOL bShowAll = TRUE);
	void			RenderPremiumCard(BOOL bIsCity);			// PremiumCard ������
	void			RenderMapPosition(UINT x, UINT y);
	BOOL			RenderCurrentEvent(UINT x, UINT y);
	void			RenderHappyHourEvent(BOOL bIsCity);
	void			RenderTextHappyHourEvent();					
	void			RenderEventRecovery();
	void			RenderStringBar();
	void			RenderGiftIcon(BOOL i_bCity);							// 2007-11-28 by dgwoo ���� �޾��� ��� ������.

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void			RenderBonusExpRate();
	void			RenderBonusExpRateText();					
	void			RenderRestKillCount(BOOL bIsMinimap);
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	virtual void	Tick();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProcCity(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				GetMaxSize(int nNum);						// nNum�� �ڸ����� ���Ѵ�.
	int				CutOffFirstNumber(int& nNum,int nMaxNum);	// nNum�� ù�ڸ��� �ڸ��� - ©���� ����.
	void			ShowMapNameTitle(UINT x, UINT y);							// ���� Ÿ��Ʋ �ٿ� ���̸� ǥ��
	BOOL			IsRenderMapName();					// ���콺 �����Ͱ� ���̴��� ��ġ ���� ������ ���̸� ������	

	// 2007-11-01 by bhsohn ���� �ƿ�� �̺�Ʈ ó�� ��� ����
	BOOL			IsHappyHourEvent();
	BOOL			IsHappyHourPcBang();
	void			SetHappyHourInfluence(BOOL bLevelup, BOOL bHappyHour, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	void			SetHappyHourPcBang(BOOL bLevelup, BOOL bPcBang, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	void			InitHappyHourInfo();
	void			AddGiftMsg(UINT i_NotifyMsgUID,UINT i_CharacterUID,char * NotifyString);
	void			DeleteGiftMsg();

	// 2008-05-21 by bhsohn ����, ������ ���� �ƿ�� �߰�
	void			SetHappyHourMotherShip(BOOL bLevelup, BOOL bStart, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	void			SetHappyHourItem(BOOL bLevelup, BOOL bStart, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg);
	// end 2008-05-21 by bhsohn ����, ������ ���� �ƿ�� �߰�

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void	SetRestCount(INT nRestCount);
	void	SetKillCount(INT nKillCount);
	INT		GetRestCount() { return m_nRestCount; };
	INT		GetKillCount() { return m_nKillCount; };
	void	SetBonusExpRate(INT nPartyRate,INT nFriendRate,INT nGuildRate);
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	// 2012-11-16 by mspark, ��ī�� ��� ǥ�� ������
	void	RestCountImageOnOff();
	void	KillCountImageOnOff();
	// end 2012-11-16 by mspark, ��ī�� ��� ǥ�� ������

// 2013. 04. 04 by ssjung ���ʽ� EXP, ����� �� ���콺�� ������ ���� �� �ý��� �޽����� ǥ�� ���ϰ� ����
	BOOL    GetRenderBonusExpRateTextOnOff();
	BOOL	GetRenderPremiumToolTipTextOnOff();

// 2013. 04. 04 by ssjung ���ʽ� EXP, ����� �� ���콺�� ������ ���� �� �ý��� �޽����� ǥ�� ���ϰ� ����

protected:

	// 2007-11-01 by bhsohn ���� �ƿ�� �̺�Ʈ ó�� ��� ����
	// 2008-03-14 by bhsohn �������� ���� ���Ǿƿ�� ó��
	void			SetHappyHourInfo(BOOL bLevelup, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* pMsg, EVENT_INFO* pEventInfo);	
	void			UpdateHappyHourTooltip();	

	void			CreateMsgBox();

	void DecaToolTipFunction(BOOL nMode, int nY);	//TRUE�� ��ī�� �ູ FALSE�� ��ī�� ��� 
	// 2013-07-15 by ssjung ĳ���� ��ī�� �ູ, ��� ���� ���� ǥ��
	
protected:
	BOOL			m_bRestored;
	BOOL			m_bRenderMapName;
	float			m_fWarningTime;
	BOOL			m_bWarning;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
    CINFGroupImage*		m_pGageBarInfo;				// 2011. 10. 10 by jskim UI�ý��� ����
#else
	CINFImageEx*		m_pImgBackPixel[2];			// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*		m_pImgBackMinimap;
#endif


	CINFImageEx* m_fpsNumbers[10];
								  
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	// 2004-12-08 by jschoi - �߰�
	// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*		m_pImgMainGageBack;										// ���� ������ ���(�»��)
	CINFImageEx*		m_pImgMainExpBack;										// ���� ����ġ ���(�߾��ϴ�)
	CINFImageEx*		m_pImgMainExpBar;										// ���� ����ġ ����(�߾��ϴ�)
	CINFImageEx*		m_pImgMainExpBar1;										// ���� ����ġ ����(�߾��ϴ�)
	CINFImageEx*		m_pImgMainGageHP[GAMEMAIN_GAGE_HP_MAX_GRADE];			// ���� ������ HP
	CINFImageEx*		m_pImgMainGageDP[GAMEMAIN_GAGE_DP_MAX_GRADE];			// ���� ������ DP
	CINFImageEx*		m_pImgMainGageSP[GAMEMAIN_GAGE_SP_MAX_GRADE];			// ���� ������ SP
	CINFImageEx*		m_pImgMainGageBooster[GAMEMAIN_GAGE_BOOSTER_MAX_GRADE];	// ���� ������ Booster
	CINFImageEx*		m_pImgMainGageFuel[GAMEMAIN_GAGE_FUEL_MAX_GRADE];		// ���� ������ Fuel
	CINFImageEx*		m_pImgMainIconFuel[2];									// ���� ������ Fuel
	CINFImageEx*		m_pImgMainSpeedNum;										// ���� ������ ���ǵ�
	CINFImageEx*		m_pImgMainMinus;										// ���� ������ ���ǵ� ���̳ʽ�(�߰�)	
	CINFImageEx*		m_pImgMainWarning;										// ���� ���
	CINFImageEx*		m_pImgGeneralPremiumCard;								// General Premium Card Image
#else 
    CINFImageEx*		m_pImgMainGageBack;										// ���� ������ ���(�»��)
	CINFImageEx*		m_pImgMainExpBack;										// ���� ����ġ ���(�߾��ϴ�)
	CINFImageEx*		m_pImgMainExpBar;										// ���� ����ġ ����(�߾��ϴ�)
	CINFImageEx*		m_pImgMainGageHP[GAMEMAIN_GAGE_HP_MAX_GRADE];			// ���� ������ HP
	CINFImageEx*		m_pImgMainGageDP[GAMEMAIN_GAGE_DP_MAX_GRADE];			// ���� ������ DP
	CINFImageEx*		m_pImgMainGageSP[GAMEMAIN_GAGE_SP_MAX_GRADE];			// ���� ������ SP
	CINFImageEx*		m_pImgMainGageBooster[GAMEMAIN_GAGE_BOOSTER_MAX_GRADE];	// ���� ������ Booster
	CINFImageEx*		m_pImgMainGageFuel[GAMEMAIN_GAGE_FUEL_MAX_GRADE];		// ���� ������ Fuel
	CINFImageEx*		m_pImgMainIconFuel[2];									// ���� ������ Fuel
	CINFImageEx*		m_pImgMainSpeedNum;										// ���� ������ ���ǵ�
	CINFImageEx*		m_pImgMainMinus;										// ���� ������ ���ǵ� ���̳ʽ�(�߰�)	
	CINFImageEx*		m_pImgMainWarning;										// ���� ���
	CINFImageEx*		m_pImgGeneralPremiumCard;								// General Premium Card Image
#endif
	// end 2011. 10. 10 by jskim UI�ý��� ����
	// 2009. 03. 16 by ckPark ����� ���� �߰�

	BOOL			m_bPremiumToolTip;

	// end 2009. 03. 16 by ckPark ����� ���� �߰�
	// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*	m_pImgSuperPremiumCard;									// Super Premium Card Image
	CINFImageEx*	m_pImgHappyHourEvent;									// ���Ǿƿ� ���� �̺�Ʈ �̹���
	CINFImageEx*	m_pImgPCHappyHourEvent;									// PC�� ���Ǿƿ� ���� �̺�Ʈ �̹���
	CINFImageEx*	m_pImgPCHomePremiumEvent;								// 2010. 06. 08 by jskim Ȩ�����̾� UI �۾�
	CINFImageEx*	m_pImgEventRecovery[GAMEMAIN_EVENT_RECOVERY_MAX];		// �̺�Ʈ ȸ���� ���� �̺�Ʈ �̹���
	CINFImageEx*	m_pImgGiftIcon[GIFT_ICON_IMG_COUNT];					// ���� ������ �̹��� 
	//CINFImageBtn*	m_pImgVoiceChatIcon;									// ����ä�� ������.
	CINFImageEx*	m_pImgVoiceChatIcon;									// ����ä�� ������.
	CINFImageEx*	m_pImgVoiceSPKIcon;									// ����Ŀ ������.
	// end 2011. 10. 10 by jskim UI�ý��� ����
	CD3DHanFont*	m_pFontEventRecovery;									// �̺�Ʈ ȸ���� ����
	CD3DHanFont*	m_pFontDecaToolTip;										// 2013-07-15 by ssjung ĳ���� ��ī�� �ູ, ��� ���� ���� ǥ��


	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	CINFImageEx*	m_pImgBonusExpRate;
	CINFImageEx*	m_pImgRestCount;
	CINFImageEx*	m_pImgKillCount;
	CINFImageEx*	m_pImgKillCount2;
	CINFImageEx*	m_pImgKillEff;

	CINFGroupImage*	m_pGroupImgRestCountBack;
	CINFGroupImage*	m_pGroupImgKillCountBack;

	BOOL			m_bRenderBonusExpRateText;
	INT				m_nPartyRate;
	INT				m_nFriendRate;
	INT				m_nGuildRate;
	
	INT				m_nRestCount;
	INT				m_nKillCount;
	float			m_fKillCountScale;
	bool			m_bChangeKillCount;
	bool			m_bCompleteKillCount;
	INT				m_nCompleteKillCountBlank;
	BYTE			m_nCompleteKillCountAlpha;
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	// 2012-11-16 by mspark, ��ī�� ��� ǥ�� ������
	CINFImageBtn*			m_pImgRestCountOnBtn;
	CINFImageBtn*			m_pImgRestCountOffBtn;
	CINFImageBtn*			m_pImgKillCountOnBtn;
	CINFImageBtn*			m_pImgKillCountOffBtn;

	UINT			m_nStateRestCountOnBtn;
	UINT			m_nStateRestCountOffBtn;
	UINT			m_nStateKillCountOnBtn;
	UINT			m_nStateKillCountOffBtn;

	UINT			m_nRestCountPosX;
	UINT			m_nRestCountPosY;
	UINT			m_nKillCountPosX;
	UINT			m_nKillCountPosY;

	bool			m_bCheckRestCountON;
	bool			m_bCheckKillCountON;

	bool			m_bCheckRestCountBtnClick;
	bool			m_bCheckKillCountBtnClick;

	float			m_fPlusRestCountPosX;
	float			m_fPlusKillCountPosX;
	// end 2012-11-16 by mspark, ��ī�� ��� ǥ�� ������


	float			m_fGageRate[GAMEMAIN_GAGE_NUMBER];

	CD3DHanFont*	m_pFontGageInfo[GAMEMAIN_GAGE_LINE_NUMBER];
#ifdef _WARRIOR_CLASS_KILL_BAR
	char			m_szGageInfo[GAMEMAIN_GAGE_LINE_NUMBER][128];	
#else
	char			m_szGageInfo[GAMEMAIN_GAGE_LINE_NUMBER][GAMEMAIN_GAGE_LINE_LENGTH];	
#endif

	CD3DHanFont*	m_pFontUnitPosInfo;

//	CINFImage*		m_pButtonShowHide[2][BUTTON_STATE_NUMBER];	//	GAMEMAIN_BUTTON_SHOW, GAMEMAIN_BUTTON_HIDE
	int				m_nButtonState;

	// ���̸� ǥ��
	CD3DHanFont*	m_pFontMapName;
	
	// ���� �̺�Ʈ ǥ��
	CD3DHanFont*	m_pFontCurrentEvent;
	CD3DHanFont*	m_pFontEventHappyHour;

	D3DXVECTOR2		m_v2MousePt;
	D3DXVECTOR2		m_v2RaderCenter;

	// �̼� �̸� ǥ��
	CD3DHanFont*	m_pFontMissionName;	

public:
	vector<string>	m_vecEventHappyHour;
	vector<GIFT_NOTIFY_MSG> m_vecGiftMsg;
	EVENT_INFO		m_HappyEventInfo;
	// 2007-11-01 by bhsohn ���� �ƿ�� �̺�Ʈ ó�� ��� ����
	EVENT_INFO		m_HappyEventAllInfluInfo;		

	EVENT_INFO		m_PCHappyEventInfo;
	BOOL			m_bRenderEvent;
	BOOL			m_bRenderEventRecovery;
	int				m_nHappyHourX;
	int				m_nHappyHourY;
	int				m_nPCHappyHourX;
	int				m_nPCHappyHourY;
	BOOL			m_bMemberPCBang;
	// 2010. 06. 08 by jskim Ȩ�����̾� UI �۾�
	BOOL			m_pMemberPremiumEvent;
	//end 2010. 06. 08 by jskim Ȩ�����̾� UI �۾�
	//--------------------------------------------------------------------------//
	//			m_BHappyHourEvent Flag				
	//			0x01				HappyHourEvent		���� �ƿ� �̺�Ʈ
	//			0x10				PCHappyHourEvent	PC���� �ƿ� �̺�Ʈ.
	//--------------------------------------------------------------------------//
	// 2007-11-01 by bhsohn ���� �ƿ�� �̺�Ʈ ó�� ��� ����
	//BYTE			m_BHappyHourEvent;				
	BYTE			m_byHappyHourInfluence;
	BOOL			m_bHappyHourAllInfluence;
	BOOL			m_bPcBangHappyHour;
	
	// 2008-05-21 by bhsohn ����, ������ ���� �ƿ�� �߰�
	BOOL			m_bMotherHappyHour;
	EVENT_INFO		m_MotherShipEventInfo;

	BOOL			m_bItemHappyHour;
	EVENT_INFO		m_ItemEventInfo;
	

	int				m_nGiftX;
	int				m_nGiftY;
	POINT			m_ptGift;
	float			m_fGiftTime;

// 2013-07-15 by ssjung ĳ���� ��ī�� �ູ, ��� ���� ���� ǥ��
	BOOL			m_bRestCountToolTip;
	BOOL			m_bKillCountToolTip;
// end 2013-07-15 by ssjung ĳ���� ��ī�� �ູ, ��� ���� ���� ǥ��

	//2015-08-15 by St0rmy, better Ping Display
	CINFImageEx*	m_pImgPing[PING_DISPLAY_COUNT];

};

#endif // !defined(AFX_INFGAMEMAINUNITINFOBAR_H__D89CEB23_A523_4B16_A8DD_203B1EEFA70F__INCLUDED_)

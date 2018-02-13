// INFCommunityParty.h: interface for the CINFCommunityParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYPARTY_H__F45C5684_4895_4B19_B2E8_8658E7FAF677__INCLUDED_)
#define AFX_INFCOMMUNITYPARTY_H__F45C5684_4895_4B19_B2E8_8658E7FAF677__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
#include "ClientParty.h"
// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

#define SIZE_PARTY_BUTTON_X			36
#define MAX_PARTY_NUMBER			10
#define PARTY_FORMATION_NUMBER		9
#define FLIGHT_FORM_NONE_NAME				STRMSG_C_PARTY_0033
#define FLIGHT_FORM_2_COLUMN_NAME			STRMSG_C_PARTY_0034
#define FLIGHT_FORM_2_LINE_NAME				STRMSG_C_PARTY_0035
#define FLIGHT_FORM_TRIANGLE_NAME			STRMSG_C_PARTY_0036
#define FLIGHT_FORM_INVERTED_TRIANGLE_NAME	STRMSG_C_PARTY_0037
#define FLIGHT_FORM_BELL_NAME				STRMSG_C_PARTY_0038
#define FLIGHT_FORM_INVERTED_BELL_NAME		STRMSG_C_PARTY_0039
#define FLIGHT_FORM_X_NAME					STRMSG_C_PARTY_0040
#define FLIGHT_FORM_STAR_NAME				STRMSG_C_PARTY_0041

#define FORMATION_NAME_LENGTH		20


#define PARTY_LIST_START_X		23
#define PARTY_LIST_HEIGHT		17
#define PARTY_INFO_START_X		146

#define PARTY_COMBO_X			77//95
#define PARTY_COMBO_WIDTH		72//70
#define PARTY_COMBO_HEIGHT		13

#define PARTY_COMBO_START_X		19
#define PARTY_COMBO_START_Y		231
#define PARTY_INFO_BUTTON_X		25
#define PARTY_INFO_START_X2		109
#define PARTY_JOIN_WIDTH		80
#define PARTY_JOIN_HEIGHT		16

#define PARTY_VOIP_X			165
#define PARTY_VOIP_Y			231
#define PARTY_VOIP_WIDTH		35
#define PARTY_VOIP_HEIGHT		16
#define PARTY_VOIP_BUTTON_STATE_NORMAL		0
#define PARTY_VOIP_BUTTON_STATE_UP			1
#define PARTY_VOIP_BUTTON_STATE_DOWN		2
#define PARTY_VOIP_BUTTON_STATE_DISABLE		3


#define PARTY_JOIN_BUTTON_STATE_NORMAL		0
#define PARTY_JOIN_BUTTON_STATE_UP			1
#define PARTY_JOIN_BUTTON_STATE_DOWN		2

// 2008-07-10 by bhsohn Ÿ������ �ȵǴ� ���� �ذ�
//#define PARTY_FRAME_SIZE_X			130
//#define PARTY_FRAME_SIZE_Y			34
#define PARTY_FRAME_ID_START_X		5
#define PARTY_FRAME_ID_START_Y		4
#define PARTY_FRAME_HP_START_X		6
#define PARTY_FRAME_HP_START_Y		21
#define PARTY_FRAME_FUEL_START_X	6
#define PARTY_FRAME_FUEL_START_Y	27

// 2006-03-07 by ispark, �� ���� ��ġ ����
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
#define PARTY_LIST_START_Y		54//56
#define PARTY_COMBO_Y			231//233
#else
#define PARTY_LIST_START_Y		54//56
#define PARTY_COMBO_Y			231//233
#endif

// ������ư ����
#define		RADIO_JOINT_COMMAND				0	// ����� �ʴ�
#define		RADIO_JOINT_FREE				1	// ���� ����
#define		MAX_RADIO_JOINT					2

// ������ư ����
#define		RADIO_AUDIO_CHAT				0	// ���� ä�ù��
#define		RADIO_AUDIO_KEY					1	// Ű �Է¹��
#define		RADIO_AUDIO_FREE				2	// �������
#define		MAX_RADIO_AUDIO					3

// ����ġ �й� �޺� �ڽ�
#define		COMBO_DIVISION_EXP			0	// ����ġ
#define		COMBO_DIVISION_ITEM			1	// ������ �й�
#define		MAX_COMBO_DIVISION			2

// ��� ���� ����
//���� ������ �̹��� ���� 
#define COMBO_FLIGHT_FORM_2_COLUMN			0	// �̷� ����, �̷� ���� ������� �� �ٷ� ������ �� ����̴�
#define COMBO_FLIGHT_FORM_2_LINE			1	// �̷� Ⱦ��, �̷� Ⱦ�� ������� �� �ٷ� ������ �� ����̴�
#define COMBO_FLIGHT_FORM_TRIANGLE			2	// �ﰢ ���, �ﰢ�� ������� ��ܺ��� 1, 2, 3���� ������ ��ġ�Ѵ�
#define COMBO_FLIGHT_FORM_INVERTED_TRIANGLE	3	// ���ﰢ ���, �� �ﰢ�� ������� ��ܺ��� 3, 2, 1���� ������ ��ġ�Ѵ�
#define COMBO_FLIGHT_FORM_BELL				4	// �� ����, �� ������� ��ܺ��� 1, 3, 2���� ������ ��ġ�Ѵ�
#define COMBO_FLIGHT_FORM_INVERTED_BELL		5	// ���� ����, ���� ������� ��ܺ��� 2, 3, 1���� ������ ��ġ�Ѵ�
#define COMBO_FLIGHT_FORM_X					6 // X�� ����
#define COMBO_FLIGHT_FORM_STAR				7	// �� ����
#define MAX_COMBO_FLIGHT_FORM				8

#define	WND_PARTYMODE_OPTION		0		// ��Ƽ �ɼ�
#define	WND_PARTYMODE_SEARCH		1		// ��Ƽ �˻�
#define	MAX_WND_PARTYMODE			2		

struct structPartyListInfo
{
	PartyID_t	nPartyId;
	int			nIdx;	// ��ȣ
	BOOL		bPartyLock;	// ���� ����
	char		chPartyName[SIZE_MAX_PARTY_NAME];	// ����
	char		chPartyMasterName[SIZE_MAX_ARENA_FULL_NAME];	// �����
	int			nMinLV;								// �ּҷ���
	int			nMaxLV;								// �ּҷ���
	int			nPartyMem;							// ��Ƽ��
	int			nMaxPartyMem;						// �ִ���Ƽ��
	BYTE		ExpDistributeType;
	BYTE		ItemDistributeType;

};


class CINFImage;
class CD3DHanFont;
class CINFMessageBox;
// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
class CINFImageBtn;			
class CINFEditBox;
class CINFSecuEditBox;
class CINFNumEditBox;
class CINFArenaScrollBar;
class CINFImageRadioBtn;
class CINFListBox;
// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
class CINFImageEx;
class CINFGroupImage;
class CINFGroupManager;

// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
class CINFSingleCurselEditBox;
// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
class CINFCommunityParty : public CINFBase  
{
public:
	CINFCommunityParty(CAtumNode* pParent);
	virtual ~CINFCommunityParty();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	
	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	void	RenderPartyDebuffIcon( PARTYENEMYINFO* pPartyMember, const int nLineNum );
	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

	void Render(POINT ptPos);
	void RenderPartyFrame();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ��� function
	void OnButtonClicked(int nButton);	// GUI �޴� Ŭ��	
	void PartySendCreate();				// ��� ���� ���������� ������.
	BOOL PartyQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y);		// ȭ��� x,y�� ��ġ�� ������ �ʴ뿩�θ� �����.
	void PartySendInviteUser();				// ��� �ʴ� ���������� ������.
	void PartyQuestionJoin(char* strMasterName, PartyID_t nPartyID);	// ��� �ʴ� ���� ��� �޽��� �ڽ� ����.
	// 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
	//void PartyRecvInvite(int i);			// ���� �ʴ� ���� ��� ����(i=0)/����(i=1)�� �����Ѵ�.
	void PartyRecvInvite(int i, PartyID_t PartyId=0);			// ���� �ʴ� ���� ��� ����(i=0)/����(i=1)�� �����Ѵ�.
	//end 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
	void PartySendSecede();					// ��� Ż��
	//void PartyQuestionBanUser();			// �߹��ǻ縦 �����.
	//void PartySendBanUser();				// �߹� ���������� ������.
	//void PartyQuestionTransferMaster();		// ���� �ǻ縦 �����.
	void PartySendTransferMaster();			// ���� ���������� ������.
	void ChagePartyFormation(int nFormation);// ��� ���� ����
	void SetEnablePartyMenu(BOOL bEnable); // ������� ��� ��� �޴� ��� ��ư ����

	// 2008-02-27 by bhsohn ���� �Ʒ��� ����
	void ISendPartyLeaveArena();

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	void RefreshSearchInfo();	
	void RefreshOptionInfo();
	

	// 2008-06-03 by bhsohn EP3 ��� ���� ó��
	void RqPartyList();
	void AddPartyList(SPARTY_LIST_INFO* pInfo);
	void DeletePartyList();
	void SetTotalPartyCnt(int nTotalPartyCnt);
	void CheckPartyListInfo();
	void ChangePartySubMode(int nSubWndMode);
	void ShowPartyFrame();
	
	// ������ ��Ƽ��
	UID32_t GetPartyBanCharacterUniqueNumber();
	void SetPartyBanCharacterUniqueNumber(UID32_t nPartyBanCharacterUniqueNumber);
	
	// ������ ��Ƽ��
	UID32_t GetPartyTransferMasterCharacterUniqueNumber();
	void SetPartyTransferMasterCharacterUniqueNumber(UID32_t nPartyTransferMasterCharacterUniqueNumber);

	SPARTY_INFO* GetSPartyInfoSetup();

	void RqClickJoinBtn(char* pPass);	// �濡 ����.
	void UpdateSetupInfo(SPARTY_INFO* pInfo);	// �¾����� ����

	void SetRefreshClickState(BOOL bClickRefresh);
	BOOL GetRefreshClickState();

	void UpdateBtnPos();

private:
	// 2008-06-03 by bhsohn EP3 ��� ���� ó��
	void RenderParyInfo(POINT ptPos);
	void RenderPartyNumInfo();
	void RenderPartyInfo();
	
	void RenderOption(POINT ptPos);
	void RenderSeach(POINT ptPos);

	int WndProcOption(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcSearch(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void UpdateUIPos();

	DWORD GetParyInfoColor(int nIdx, PARTY_TYPE PartyType, char* pUserName);
	void AllEditBoxDisable();
	void UpdateRadioBtn();

	void OnClickJoint(int nId);
	void OnClickAudioOp(int nId);
	void InitComboInfo();

	void InitControl();
	void LoadControlInfo(SPARTY_INFO* pSPartyInfo);

	void PartyQuestionBanUser();//�߹��� �����.
	void PartyQuestionTransferMaster();	// ����üũ
	void OnClickOkBtn();	// ���� Ŭ�� 
	void RqChangePartyInfo();	// ��Ƽ ���� ���� ��û

	void OnClickRefresh();

	void OnClickJoinBtn();	
	void SetVoiceChatVolum(DWORD i_nVolum);

	// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
	int WndProcEdit(UINT uMsg, WPARAM wParam, LPARAM lParam);
	// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�


	void TestList();
public:
	BOOL			m_bRestored;
	BOOL			m_bInvalidated;

	BOOL			m_bEnablePartyMenu;
	char			m_strInviteUser[20];

	// ����Ŀ ��� ������
	float			m_fSpeakingTimers;
	BOOL			m_bRenderingSpeak;

protected:
	//CINFImage	*	m_pPartyCreateButton[3];
	//CINFImage	*	m_pPartyInviteButton[3];
	//CINFImage	*	m_pPartySecedeButton[3];
	//CINFImage	*	m_pPartyBanButton[3];
	//CINFImage	*	m_pPartyTrustButton[3];
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx	*	m_pFormat;
#endif
	CINFImageEx	*	m_pForcus;
	//CINFImage	*	m_pCombo;
	//CINFImage	*	m_pCaptainCombo;
	//CINFImage	*	m_pJoinFormationButton[3];
	//CINFImage	*	m_pFormationInfo;
	CINFImageEx	*	m_pPartyFrame;
	CINFImageEx	*	m_pPartyFrameFuel;
	CINFImageEx	*	m_pPartyFrameHP;
	//CINFImage	*	m_pPartyVOIPButton[4];
	CINFImageEx	*	m_pPartyTalking;

	int				m_nPartyButtonState[5];
	int				m_nJoinFormationButtonState;
	// 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
	//PartyID_t		m_nPartyRequestID;
	//end 2009. 12. 03 by jskim ��Ƽ ������ ��Ƽ�� ���ԵǾ� �ִ��� üũ
	int				m_nPartyBanCharacterUniqueNumber;
	int				m_nPartyTransferMasterCharacterUniqueNumber;
	int				m_nPartySelectMember;// -1(���þ���), 0 ~ 6:����
	//CD3DHanFont *	m_pPartyFormation[PARTY_FORMATION_NUMBER];
	CD3DHanFont	*	m_pPartyList[MAX_PARTY_NUMBER];
	CD3DHanFont	*	m_pPartyInfo[MAX_PARTY_NUMBER];
	CD3DHanFont	*	m_pFontPartyFrame[MAX_PARTY_NUMBER];
	char			m_strPartyFormation[PARTY_FORMATION_NUMBER][FORMATION_NAME_LENGTH];
	BOOL			m_bShowFormationList;

	int				m_nVOIPButtonState;	
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*				m_pImgBk[MAX_WND_PARTYMODE];	// ���
	CINFGroupImage*				m_pPartyControlBtn[MAX_WND_PARTYMODE];	// ��Ʈ�� ��ư			  
#else					
	CINFImageEx*				m_pImgBk[MAX_WND_PARTYMODE];	// ���
#endif

	//////////// ��� �˻� //////////// 	
	CINFImageBtn*				m_pSCrateBtn;			// ����
	CINFImageBtn*				m_pSJoinBtn;				// ����
	CINFImageBtn*				m_pSRefreshBtn;			// ���ΰ�ħ
	PARTY_TYPE					m_bPartyTypeBtn;
	vector<structPartyListInfo*>	m_vecPartyListInfo;
	
	int							m_nTotalPartyCnt;
	
	CINFArenaScrollBar*			m_pScrollPartyList;
	CINFImageEx*				m_pImgSelect;
	
	CINFImageEx*				m_pLockImage;
	CINFImageEx*				m_pUnLockImage;

	int							m_nSelParty;

	int							m_nSubWndMode;			// ��Ƽ �ɼ�
	ATUM_DATE_TIME				m_curRqPartyListTime;	// ��Ƽ ���� ��û�ð�

	//////////// ��� �ɼ� //////////// 
	// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
	//CINFEditBox*			m_pEditPartyName;
	CINFSingleCurselEditBox*			m_pEditPartyName;
	// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
	CINFSecuEditBox*		m_pINFSecuEditBox;
	CINFNumEditBox*			m_pNumMinEditBox;	// �ּ� ����
	CINFNumEditBox*			m_pNumMaxEditBox;	// �ִ� ����	

	CINFImageBtn*			m_pOJoinBtn;			// �ʴ�
	CINFImageBtn*			m_pOLeaveBtn;			// Ż��
	CINFImageBtn*			m_pOPurgeBtn;			// �߹�
	CINFImageBtn*			m_pOChargeBtn;			// ����	
	CINFImageBtn*			m_pOOkBtn;				// Ȯ��	
	CD3DHanFont*			m_pOPartyList;			// ��Ƽ�� ���
	CINFImageBtn*			m_pOPartyBattle;			// ��� ����.
	
	CINFImageEx*			m_pSelImage;			// ��Ƽ�������̹���
	int						m_nPartyOPSel;

	SPARTY_INFO				m_pSPartyInfoSetup;

	// �������
	CINFImageRadioBtn*		m_pPartyJoint[MAX_RADIO_JOINT];	
	
	// ���� �ɼ�
	CINFImageRadioBtn*		m_pAudioOp[MAX_RADIO_AUDIO];
	int						m_nAduioIdx;
	
	// �й� ���
	CINFListBox*			m_pComboDivision[MAX_COMBO_DIVISION];	

	// ��� ����
	CINFListBox *			m_pComboForMation;
	CINFImageEx*			m_pFormatImage[MAX_COMBO_FLIGHT_FORM];		

	
	BOOL					m_bClickRefresh;

	POINT						m_ptWindowPos;
	CINFImageEx*			m_pImgSpkVolBar;
	

	DWORD						m_nVolum;
	BOOL						m_bVolumCon;							// �����Ҹ� ������.
	float						m_fVolumConTimeGap;						// 
	POINT						m_ptOldVolumPos;						// ������ ���� ��ġ.
	INT							m_nPosVolum;							// ������ġ(0) + m_nPosVolum      : m_nPosVolum = (m_nVolum / VOICECHAT_SPK_W) * 100


};

#endif // !defined(AFX_INFCOMMUNITYPARTY_H__F45C5684_4895_4B19_B2E8_8658E7FAF677__INCLUDED_)

// INFCommunityGuild.h: interface for the CINFCommunityGuild class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYGUILD_H__182E95F2_E402_480F_86DB_26C11C5016E6__INCLUDED_)
#define AFX_INFCOMMUNITYGUILD_H__182E95F2_E402_480F_86DB_26C11C5016E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define BUTTON_MAKE							0	// ����
#define BUTTON_LEAVE						1	// Ż��
#define BUTTON_INVITE						2	// �ʴ�
#define BUTTON_MARK							3	// ��ũ
#define BUTTON_BANMEMBER					4	// �߹�
#define BUTTON_DISBAND						5	// ��ü
#define BUTTON_CANCEL						6	// ��ü ���
#define BUTTON_NUMBER						7	// ���� ����

#define BUTTON_VOIP							10	// VOIP

//#define MAX_GUILD_NUMBER					9	// ȭ��� ���� ��
#define MAX_GUILD_NUMBER					13	// ȭ��� ���� ��


// 2008-06-16 by bhsohn EP3���� ���׼���
#define MAX_COMBO_GUILD_RANK				13	// ��� ��ũ ����
#define BUTTON_STATE_VOIP					4

// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#define GUILD_TAB_ID						0	// ���̵�
#define GUILD_TAB_GEAR						1	// ���
#define GUILD_TAB_LEVEL						2	// ����
#define GUILD_TAB_RANK						3	// ���
#define GUILD_TAB_VOICE						4	// ����
#define GUILD_TAB_CONNECT					5	// ����
#define MAX_GUILD_TAB						6
// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â


//typedef vector<MEX_GUILD_MEMBER_INFO*>				CVectorGuildMemberInfo;
//typedef vector<MEX_GUILD_MEMBER_INFO*>::iterator	CVectorGuildMemberInfoIterator;

///////////////////////////////////////////////////////////////////////////////
/// \class		CINFCommunityGuild
///
/// \brief		��� ����, GUI
/// \author		dhkwon
/// \version	
/// \date		2004-05-20 ~ 2004-05-20
/// \warning	SendGetGuildInfo -> resetGuildInfo -> AddGuildMember(n)
///////////////////////////////////////////////////////////////////////////////

class CINFMessageBox;
class CINFImage;
class CD3DHanFont;
class CINFScrollBar;
class CINFComboBox;
class CINFImageBtn;
class CINFArenaScrollBar;
class CINFPopupMenu;	// �޴�â
class CINFImageComboBox;		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
class CINFCommunityGuildManager;
//class CINFEditBox;
class CINFCurselEditBox;	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
class CINFGroupImage;
class CINFImageEx;
class CEnemyData; // 2012-04-09 by isshin ���̴� �� �̴ϸ� ��Ƽ,���ܿ� ǥ��

class CINFCommunityGuild : public CINFBase  
{
public:
	CINFCommunityGuild(CAtumNode* pParent);
	virtual ~CINFCommunityGuild();

	BOOL IMSocketSendGuildSetRank( int nGuildRank, UID32_t	MemberUniqueNumber);
	void IMSocketSendGetGuildInfo(UINT nGuildUniqueNumber);
	void IMSocketSendGuildMark(char* szFileName);
	void ResetGuildInfo(MSG_IC_GUILD_GET_GUILD_INFO_OK *pInfo, BOOL bEraseMemberList);
	void AddGuildMember(MEX_GUILD_MEMBER_INFO* pInfo);
	void DeleteGuildMember(UINT nUniqueNumber);
	
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	//MEX_GUILD_MEMBER_INFO* GetGuildMemberInfo(UINT nUniqueNumber);
	structGuildInfo* GetGuildMemberInfo(UINT nUniqueNumber);

	MSG_IC_GUILD_GET_GUILD_INFO_OK * GetGuildInfo() { return m_pGuildInfo; }
	void RenderGuildMark(int x, int y, int nGuildUniqueNumber, int nCharacterUniqueNumber);
	void RenderGuildRank(int x, int y, int nGuildRank );

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	void Render(POINT ptPos);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);

	BYTE GetMyGuildRank() { return m_nGuildRank; }
	static constexpr bool GetNtoNCallState() { return false; } // { return m_bNtoNCallState; }
	void SetNtoNCallState(BOOL bState);						// ������ȭ��!
	void IMSocketSendVoipNtoNInviteUser( UID32_t nMemberUniqueNumber );
	BOOL pGuildQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y);
	void SetMaxGuildMember(int nNum);
	

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	//CVectorGuildMemberInfo * GetGuildMemberList(){return &m_vecGuildMember;}
	vector<structGuildInfo*>* GetGuildMemberList(){return &m_stGuildInfo;}

	void RefreshGuildInfo();	// ��� ����
	void SetGuildOnOffInfo(UINT nUniqueNumber, BYTE IsOnline);	// ���� ���� ����	
	void SetLevelInfo(UID32_t CharacterUID, INT	nLevel);	// ���� ����
	void SetGuildDSetRank(UINT nUniqueNumber, BYTE GuildRank);	// ���� ��ŷ
	void CloseGuildWnd();
	void OnClickAppoint(); // �Ӹ�
	void SetNoticeWrite(char* pNotice);

	// 2008-06-18 by bhsohn ���ܿ����� ī�� ���� ó��
	void SetGuildChangeMemberShip(MSG_IC_GUILD_CHANGE_MEMBERSHIP* pMsg);

	BYTE GetGuildOrderIdx(BYTE byRank);

	void UpdateBtnPos();

	BOOL SetVoiceGuild(UID32_t i_CharacterUID);
	BOOL NoneVoiceGuild(UID32_t i_CharacterUID);
	BOOL IsGuildMember()  {		return m_bUIGuildMember;	}			// ��尡 �ִ°�?

	void SetGuildRank(INT nGuildTotalFameRank, INT nGuildMonthlyFameRank);

	// 2008-10-14 by bhsohn VOIP����
	void SetVOIPSpeaking(char *i_pUserName, BOOL i_bEnableTalking);
	void ReleaseVOIPInfo(char *i_pUserName);

	// 2008-10-20 by bhsohn VOIP ���� ����
	BOOL IsVoiceONGuildMaster();
	void ReleaseVOIPGuildInfo();	
	

protected:
	void RenderGuildMemberList(BYTE nGuildState);	
	void DeleteAllGuildInfo();	
	void OnClickPopupMenu(int nPopupMenuId);	
	void RenderQuildRanking();
	void RefreshGuildRankCombo();	
	void SetGuildMemberSelect(int nGuildSelect);
	void OnClickTab(int nTab);
	int ConvertUnitKind_TO_Order(USHORT i_nUnitKind);
	structGuildInfo* GetGuildMemeber(int nSelect);	

	void UpdateGuildMemberSelect(POINT pt, BOOL bPopupMenu);
	
	void PopupGuildRankInfo(UID32_t SelMemberUniqueNumber);
	BOOL UpdateGuildRankInfo(POINT pt);
	void AllHideComboItem();	

	void OnClickRegister();		// ���� ���� ���
	void GetGuidPeopleInfo(char* pGuildMasterName, char* pGuildSecondName);
	void UpdateMenuInfo();

	int GetMaxGuildSize();
	void RqCharacterUserInfo();
	void UpdateComboBoxItem();

	void TestDB();
	BOOL IsPossbleChangeRank(int nGuildRank);


protected:
	MSG_IC_GUILD_GET_GUILD_INFO_OK *m_pGuildInfo;
	
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	//CVectorGuildMemberInfo	m_vecGuildMember;

	BOOL		m_bGuildMember;	// ���� ���� 
	BYTE		m_nGuildRank;	// ��� : �����,����,���,�����(GUILD_RANK_~)
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	//CINFImage	*m_pBack;
	CINFGroupImage*	m_pBack;
	CINFGroupImage*	m_pBackControl;
#else 
	CINFImageEx	*m_pBack;
#endif
	CINFImageEx	*m_pImgSelect;
	//CINFImage	*m_pButton[BUTTON_NUMBER][BUTTON_STATE_NUMBER];
	CINFImageEx	*m_pImgGuildRank[MAX_COMBO_GUILD_RANK];
	//CINFImage	*m_pGuildVOIPButton[BUTTON_STATE_VOIP];
	int			m_nButtonState[BUTTON_NUMBER];
	int			m_nVOIPButtonState;

	CD3DHanFont	*	m_pFontGuildMaster;
	CD3DHanFont	*	m_pFontGuildMember[MAX_GUILD_NUMBER];
	CD3DHanFont	*	m_pFontGuildOnline[MAX_GUILD_NUMBER];	// color : online:BLUE, offline:RED
	CD3DHanFont	*	m_pFontGuildRank[MAX_GUILD_NUMBER];		// ���Ŀ� �������� �ٲ�
	CD3DHanFont	*	m_pFontGuildAll;

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	//CINFScrollBar*	m_pScroll;
	CINFArenaScrollBar*			m_pScrollMember;	
	CINFImageComboBox*	m_pINFImageComboBox[MAX_GUILD_NUMBER];
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â

	//CINFComboBox*	m_pComboBox[MAX_GUILD_NUMBER];

	//BOOL		m_bNtoNCallState;	// ���� ��ȭ ����

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	CINFImageBtn*	m_pSpManageBtn;		// ������ ����
	CINFImageBtn*	m_pRegisterBtn;		// ���
	
	CINFImageBtn*	m_pInviteBtn;		// �ʴ�
	CINFImageBtn*	m_pMarkBtn;			// ��ũ
	CINFImageBtn*	m_pOutBtn;			// Ż��
	CINFImageBtn*	m_pCancelBtn;		// ����
	CINFImageBtn*	m_pOpBtn;			// �ɼ�

	CINFImageBtn*	m_pSearchBtn;		// ���ܰ˻�
	CINFImageBtn*	m_pGuildBattleBtn;	// ���� ����.
	CINFImageBtn*	m_pOpOpenBtn;		// ���� ����
	
	CINFImageBtn*	m_pCrateBtn;		// ���� â��

	CINFImageBtn*	m_pCancelQuildCancelBtn;		// ���� ��ü ���

	CINFImageBtn*	m_pImageTabBtn[MAX_GUILD_TAB];
	BOOL			m_bTabState[MAX_GUILD_TAB];

	vector<structGuildInfo*>	m_stGuildInfo;
	
	// ���õ� ����
	int				m_nGuildSelect;			
	UID32_t			m_SelMemberUniqueNumber;
	

	// �޴�â
	CINFPopupMenu*		m_pPopupMenu;

	BOOL			m_bShowRank;

	CINFImageEx*	m_pImgVOIP;
	CINFImageEx*	m_pImgVOIPSpeaking;		// VOIP����

	// ���� ��������
	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
	//CINFEditBox*	m_pEditOpenArea;
	CINFCurselEditBox*			m_pEditOpenArea;
	CINFArenaScrollBar*			m_pScrollGuildOpen;		

	BOOL		m_bUIGuildMember;	// ���� ���� 
	BYTE		m_nUIGuildRank;	// ��� : �����,����,���,�����(GUILD_RANK_~)

	char		m_chOpenNotice[SIZE_MAX_NOTICE];
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
};

#endif // !defined(AFX_INFCOMMUNITYGUILD_H__182E95F2_E402_480F_86DB_26C11C5016E6__INCLUDED_)

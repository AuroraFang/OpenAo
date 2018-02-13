// INFCommunityGuildSearch.h: interface for the CINFCommunityGuildSearch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYGUILDSEARCH_H__8D69F6B5_56DC_4606_962A_8925549605F3__INCLUDED_)
#define AFX_INFCOMMUNITYGUILDSEARCH_H__8D69F6B5_56DC_4606_962A_8925549605F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
struct structGuildSearchInfo
{
	UID32_t	nGuildUniqueNumber;								// ��� �Ƶ�
	char	GuildName[SIZE_MAX_GUILD_NAME];							// ��� �̸�
	char	GuildCommanderCharacterName[SIZE_MAX_ARENA_FULL_NAME];		// �����	
	ATUM_DATE_TIME	RegsterDateTime;								// ��� ����
	char	chNotice[SIZE_MAX_NOTICE];		// ��������
};

class CINFEditBox;
class CINFGroupImage;

class CINFCommunityGuildSearch  : public CINFBase  
{
public:	
	CINFCommunityGuildSearch(CAtumNode* pParent);
	virtual ~CINFCommunityGuildSearch();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	// ������ ����
	void AddGuildList(UINT nGuildUniqueNumber, char *pGuildName, 
												char *pGuildCommanderCharacterName, 
												ATUM_DATE_TIME	RegsterDateTime,
												char* pNotice);	

	BOOL GetGuildSearchSelGuidUID(UID32_t *pUID);
	BOOL GetGuildSearchSelGuidName(char* pGuildName);

	void IMSocketGuildSearchIntroDone();
	void IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg);

	MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* GetMySelfIntro();
	void OnClickGuildRqCancel();

private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
	BOOL OnIMEEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, 
					int nMaxLine, CINFEditBox* pEdit, CINFArenaScrollBar* pScroll);
	
	BOOL OnClickMemberList(POINT pt);
	void RenderMemberList();
	void OnClickGuildRq();		//��� ��û
	void RqGuildSearchIntro();	// ���� ����Ʈ ��û
	void DelGuildSearchVector(); // ���� �ʱ�ȭ

	void TestDB();
	void UpdateBtnState();
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	
	CINFGroupImage*	m_pFormat;																	  
#else
    CINFImageEx*    m_pFormat;
#endif

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	
	
	CINFImageBtn*			m_pRqBtn;			// ����
	CINFImageBtn*			m_pRqCancelBtn;				// ����

	CINFImageBtn*				m_pCloseBtn ;	// �ݱ� ��ư
	
	CINFEditBox*			m_pEditGuildIntro;		// �ڱ� �Ұ���
	CINFArenaScrollBar*		m_pScrollGuildIntro;

	CINFArenaScrollBar*		m_pScrollGuildList;	// ��� ����Ʈ

	CINFImageEx*			m_pImgSelect;			// �����̹���
	int						m_nSelMemberList;
	CD3DHanFont*			m_pFontSel;

	vector<structGuildSearchInfo*>	m_vecGuildSearchInfo;	// ������ ����

	MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK	m_myShuttleIntroduceInfo;
};

#endif // !defined(AFX_INFCOMMUNITYGUILDSEARCH_H__8D69F6B5_56DC_4606_962A_8925549605F3__INCLUDED_)

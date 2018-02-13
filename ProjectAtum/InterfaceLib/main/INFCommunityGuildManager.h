// INFCommunityGuildManager.h: interface for the CINFCommunityGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYGUILDMANAGER_H__F8C97E4D_889B_4218_B582_47455597F63E__INCLUDED_)
#define AFX_INFCOMMUNITYGUILDMANAGER_H__F8C97E4D_889B_4218_B582_47455597F63E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
struct structGuildApplicantInfo
{
	UID32_t	CharacterUID;
	char chUser[SIZE_MAX_ARENA_FULL_NAME];					// ���� ����
	char chUnitKind[SIZE_MAX_CHARACTER_NAME];				// ������ ����
	USHORT UnitKind;
	char chLevel[SIZE_MAX_CHARACTER_NAME];					// ���� ���� 
};

class CINFEditBox;
class CINFCurselEditBox;	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
class CINFImageEx;

class CINFCommunityGuildManager  : public CINFBase  
{
public:	
	CINFCommunityGuildManager(CAtumNode* pParent);
	virtual ~CINFCommunityGuildManager();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	void AddMemberList(UID32_t	CharacterUID, char *pUser, USHORT UnitKind, int nLevel);	// ������ ����
	void RqGuildGetIntodution();		// ���� �Ұ��� ���´�.
	void UpdateGuildIntoduce(char* pNotice);// ���� �Ұ��� ���´�.
	void IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg);

private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
	BOOL OnIMEEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, 
					int nMaxLine, CINFEditBox* pEdit, CINFArenaScrollBar* pScroll);

	void OnClickEditCtl(CINFEditBox*	pSelEdit);
	BOOL OnClickMemberList(POINT pt);
	void RenderMemberList();

	void RqAPPLICANTList();			// ������ ���� ����Ʈ ��û
	void OnClickGuildIntoduce();			// ���� �Ұ� ���
	void OnClickGuildIntoduceCancel();		// ���� �Ұ� ���

	void RqMemberInfo(UID32_t	CharacterUID);	// ���� ������ ��û�Ѵ�.

	void DelMemberList(UID32_t CharacterUID);

	// ���
	void OnClickAccept();
	void RqAccept();

	// ����
	void OnClickRef();
	void RqRef();

	void TestDB();
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage* m_pFormat;
	CINFGroupImage* m_pFormatControl;															  
#else					
    CINFImageEx*	m_pFormat;	
#endif


	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	

	CINFImageBtn*			m_pRegesterBtn;			// ��� 
	CINFImageBtn*			m_pCancelBtn;			// ����
	CINFImageBtn*			m_pAcceptBtn;			// ����
	CINFImageBtn*			m_pRefuBtn;				// ����

	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
	//CINFEditBox*			m_pEditGuildIntro;		// ���� �Ұ�
	CINFCurselEditBox*			m_pEditGuildIntro;		// ���� �Ұ�
	CINFArenaScrollBar*		m_pScrollGuildIntro;

	CINFEditBox*			m_pEditMemberIntro;		// �ڱ� �Ұ���
	CINFArenaScrollBar*		m_pScrollMemberIntro;

	CINFArenaScrollBar*		m_pScrollMemberList;	// ��� ����Ʈ

	CINFImageEx*			m_pImgSelect;			// �����̹���
	int						m_nSelMemberList;
	CD3DHanFont*			m_pFontSel;

	vector<structGuildApplicantInfo>	m_vecGuildApplicantInfo;	// ������ ����

	CINFImageBtn*				m_pCloseBtn ;	// �ݱ� ��ư

};

#endif // !defined(AFX_INFCOMMUNITYGUILDMANAGER_H__F8C97E4D_889B_4218_B582_47455597F63E__INCLUDED_)

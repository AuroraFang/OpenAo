// INFCommunityGuildIntroMe.h: interface for the CINFCommunityGuildIntroMe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYGUILDINTROME_H__E80D8E42_7067_46A0_B621_3905C26CB829__INCLUDED_)
#define AFX_INFCOMMUNITYGUILDINTROME_H__E80D8E42_7067_46A0_B621_3905C26CB829__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

//class CINFEditBox;
class CINFCurselEditBox;	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
class CINFGroupImage;

class CINFCommunityGuildIntroMe  : public CINFBase  
{
public:	
	CINFCommunityGuildIntroMe(CAtumNode* pParent);
	virtual ~CINFCommunityGuildIntroMe();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	void RqGuildIntroMe();
	

private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();
//	BOOL OnIMEEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, 
//					int nMaxLine, CINFCurselEditBox* pEdit, CINFArenaScrollBar* pScroll);

	void OnClickEditCtl(CINFCurselEditBox*	pSelEdit);	

	void OnClickRqGuild();				// ���Խ�û
	void OnClickRqCancelGuild();		// ���Խ�û ���
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
	
	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
	//CINFEditBox*			m_pEditGuildIntro;		// �ڱ� �Ұ���
	CINFCurselEditBox*			m_pEditGuildIntro;		// �ڱ� �Ұ���

	CINFArenaScrollBar*		m_pScrollGuildIntro;
	CINFImageBtn*				m_pCloseBtn ;	// �ݱ� ��ư

};

#endif // !defined(AFX_INFCOMMUNITYGUILDINTROME_H__E80D8E42_7067_46A0_B621_3905C26CB829__INCLUDED_)

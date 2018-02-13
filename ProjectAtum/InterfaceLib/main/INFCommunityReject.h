// INFCommunityReject.h: interface for the CINFCommunityReject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYREJECT_H__CAB71D99_F963_4DCD_AB22_05510CEE28CC__INCLUDED_)
#define AFX_INFCOMMUNITYREJECT_H__CAB71D99_F963_4DCD_AB22_05510CEE28CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class CINFScrollBar;
class CD3DHanFont;
class CINFArenaScrollBar;
class CINFImageBtn;
class CINFEditBox;
class CINFGroupImage;
class CINFImageEx;

typedef struct
{
	char			szRejectName[SIZE_MAX_CHARACTER_NAME];
	ATUM_DATE_TIME	atimeRegTime;			// �ź� ����Ʈ �������
}Reject_t;

#define MAX_REJECT_NUMBER					9	// ȭ��� ���� ��
#define MAX_REJECT_LIST						50  // �����Ҽ� �ִ� �ִ� ģ�� ��

class CINFCommunityReject : public CINFBase  
{
public:
	CINFCommunityReject(CAtumNode* pParent);
	virtual ~CINFCommunityReject();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	void Render(POINT ptPos);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	void AddRejectList(Reject_t struRej);
	void InitRejectList();
	
	void DeleteRejectList(char *RejectName);
	void SendDeleteRejectList();

	void InitEditBox();
	void UpdateBtnPos();
private:
	void TestDBRejectList();		// Test�� 
	void RenderRejectList(POINT ptPos);
	void OnClickReject();			//	�ź� ��� 
protected:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*			m_pBack;
#else								
	CINFImageEx*            m_pBack;
#endif
	CINFImageEx*			m_pImgSelect;
	//CINFImage*				m_pImgButton[4];

	CINFArenaScrollBar*			m_pScroll;	
	CD3DHanFont				*m_pFontReject[MAX_REJECT_NUMBER];

	BOOL					m_bAddRejectClick;
	BOOL					m_bDeleteRejectClick;	
	
	int						m_nRejectSelect;
	int						m_nButtonState;	

	int						m_nRejectRenderX;
	int						m_nRejectRenderY;

	CINFImageBtn*				m_pRegisterReject;	// ���
	CINFImageBtn*				m_pDelReject;		// ����
	CINFEditBox*				m_pINFUserName;

public:
	//BOOL					m_bDeleteOK;
	vector<Reject_t>		m_vecReject;
};

#endif // !defined(AFX_INFCOMMUNITYREJECT_H__CAB71D99_F963_4DCD_AB22_05510CEE28CC__INCLUDED_)

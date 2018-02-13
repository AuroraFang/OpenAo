// INFWorldRankWnd.h: interface for the CINFWorldRankWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFWORLDRANKWND_H__C2EF2BD5_6F6C_4AAE_80DC_AAC6DC21DB39__INCLUDED_)
#define AFX_INFWORLDRANKWND_H__C2EF2BD5_6F6C_4AAE_80DC_AAC6DC21DB39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFDefaultWnd.h"
#include "WorldRankManager.h"


// �� ��ǥ �ִ�
#define		MAX_WR_INFO_X				10


class CD3DHanFont;
class CINFListBox;
class CINFGroupImage;

class CINFWorldRankWnd  : public CINFDefaultWnd
{
public:
	CINFWorldRankWnd();
	virtual ~CINFWorldRankWnd();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();
	virtual void	ShowWnd(BOOL bShowWnd, POINT *ptPos=NULL, int nWndWidth=0);
	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void UpdateBtnPos(int nWidth, int nHeight);

	void	RqWorldRankInfo();	// ��ŷ ���� ��û

private:
	int OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnKeyDown(WPARAM wParam, LPARAM lParam);

	
	void OnClickRankBtn(int i_nSelIdx);

	void	RenderWorldRankTxt();	
	void	RenderWorldRankTxt_Level();
	void	RenderWorldRankTxt_Honor();
	void	RenderWorldRankTxt_PVP();

	void	RenderSelectPage();
	
	void	InitWorldRankInfo();	// �ʱ�ȭ
	void	TestDB();

	BOOL	IsRqPossibleStats();
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*     m_pBk;
#endif
	CINFImageEx*		m_pBkImage[MAX_WORLDRANK_SERVICE];

	int				m_nSelLocalWorld;

	// � ��ŷ�� ����
	CINFImageBtn*			m_pRankBtn[MAX_WORLDRANK_INFO];
	CINFImageEx*				m_pRankBk[MAX_WORLDRANK_INFO];
	int						m_nSelectRankInfo;


	// �ڽ��� ��ŷ ����
	CD3DHanFont*			m_pFontRankInfo;	

	// ���� ����
	CD3DHanFont*			m_pFontTable[MAX_WORLDRANK_ONEPAGE][MAX_WR_INFO_X];	
	
	// ������ ������
	CD3DHanFont*			m_pFontPage[MAX_WORLDRANK_PAGE];	
	int						m_nSelectPage;

	DWORD					m_dwSendTermTime;	// �ð�

	// ��� ����
	CINFListBox*			m_pComboGear;

};

#endif // !defined(AFX_INFWORLDRANKWND_H__C2EF2BD5_6F6C_4AAE_80DC_AAC6DC21DB39__INCLUDED_)

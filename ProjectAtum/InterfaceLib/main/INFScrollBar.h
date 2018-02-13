// INFScrollBar.h: interface for the CINFScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSCROLLBAR_H__B95568B8_C946_4170_92DA_D3D6155D1C0D__INCLUDED_)
#define AFX_INFSCROLLBAR_H__B95568B8_C946_4170_92DA_D3D6155D1C0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define INFSCROLL_TYPE_VERTICAL		0	// ����
#define INFSCROLL_TYPE_HORIZONTAL	1	// ����

class CINFImage;
class CINFImageEx;													 // 2011. 10. 10 by jskim UI�ý��� ����
class CINFScrollBar : public CINFBase  
{
public:
	CINFScrollBar(CAtumNode* pParent, int nStartX, int nStartY, int nLineLength,
		int nNumberOfWindowList, int nScrollBarSizeX = 11, int nScrollBarSizeY = 38,
		char *strIconImg = NULL, int nScrollStyle=INFSCROLL_TYPE_VERTICAL);
	virtual ~CINFScrollBar();

	void Reset();
	void SetScrollLinePos(int nStartX, int nStartY) { m_nStartX = nStartX; m_nStartY = nStartY; } // ��ũ�� ���� ��ġ
	void SetWheelRect(int nStartX, int nStartY, int nEndX, int nEndY); // �ٸ��콺 ���� ����
	void SetNumberOfData(int nNum);		//	����Ÿ ���� ��ȭ�� ���� ���
	void SetIncreaseCurrent(int nNum);	//	����Ÿ ���� ��ȭ�� ���� ���
	void SetCurrentIndex(int nCurrent){m_nCurrentScrollIndex = nCurrent;}
	void SetNumberOfWindowList(int nWindowList) {m_nNumberOfWindowList = nWindowList;}
	int GetCurrentScrollIndex() { return m_nCurrentScrollIndex; }
//	int GetCurrentSelectWindowIndex() { return m_nCurrentSelectWindowIndex; }
	int GetCurrentSelectDataIndex() { return m_nCurrentSelectDataIndex; }
	int GetCurrentSelectWindowIndex() { return m_nCurrentSelectDataIndex - m_nCurrentScrollIndex; }
	void SetCurrentScrollBar(int nScrollStyle);
	int  GetScrollBarIndex() { }

   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	//void Render();
	// 2012-02-27 by mspark, ��ų���� ��ũ�� ��ġ ���� ���߱�
	void Render(int nScrollX = 0);
	// end 2012-02-27 by mspark, ��ų���� ��ũ�� ��ġ ���� ���߱�
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	CINFImageEx*	m_pScrollBar;
	CINFImageEx*	m_pScrollLine[3];		//	��ũ�� ������ ���κ�, �߰�, �Ʒ��κ�
	RECT			m_rcWheel;				// �� ���콺�� �۵��� ����(����Ʈ�ڽ� ����)
	POINT			m_ptOldMouse;			// m_bLockMouse���� ����� ���� ���콺 ��ǥ
	BOOL			m_bLockMouse;
	int				m_nNumberOfWindowList;	//	������ ����Ʈ ���μ�
	int				m_nNumberOfData;		// ��ü ����Ÿ ��
	int				m_nCurrentScrollIndex;	// ���� ��ũ�ѵ� �ε���
	int				m_nStartX;				// ��ũ�� ���� ���� ��ǥ
	int				m_nStartY;
	int				m_nScrollLineLength;	// ��ũ�� ���� ����
	int				m_nCurrentSelectDataIndex;	//	���� ���õ� ����Ÿ �ε���(-1:���õ� ���� ����)
	int				m_nScrollSizeX;
	int				m_nScrollSizeY;			// ��ũ�� �� Y ����
	int				m_nScrollStyle;
	char			m_strScrollImgName[64];
	int				m_nCurrentScrollBarX;
	int				m_nCurrentScrollBarY;
};

//////////////////////////////////////////////////////////////////////////
enum{ SCROLL_BTN_NON, SCROLL_BTN_OVER, SCROLL_BTN_PUSH, SCROLL_BTN_INACTIVE };
enum{ SCROLLPOS_TOP_BTN = 1, SCROLLPOS_BOTTOM_BTN, SCROLLPOS_BAR };

class CINFScrollBarEX : public CINFBase  
{
public:
	CINFScrollBarEX(CGameData* pGameData, 
					int * pParentStartX, 
					int * pParentStartY, 
					int nStartX, 
					int nStartY, 
					int nScrollBarW,
					int nScrollBarH,
					int nMaxCount,					// �Ѷ��μ�.
					int nMaxLine,					// ���̴� ���μ�.
					int nLineLength);				//
	virtual ~CINFScrollBarEX();

   	virtual HRESULT			InitDeviceObjects(char * pScrollBar = NULL, char * pScrollLine = NULL, char * pScrollTop = NULL, char * pScrollBottom = NULL);
	virtual HRESULT			RestoreDeviceObjects();
	virtual HRESULT			DeleteDeviceObjects();
	virtual HRESULT			InvalidateDeviceObjects();
	virtual void			Render();
	virtual void			Tick();
	virtual int				WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int						GetCurrentScrollLine()		{ return m_nScrollLine; }
	void					SetMouseWheel(int nwParam);
	
	void					SetScrollImgSize(int nTopBtnSize, int nBottomBtnSize);
	void					SetScrollData(int nMaxCount, int nMaxLine);
	void					SetScrollEndLine();
	void					SetScrollStartLine();

protected:
	int						GetScrollLineNumber(float nLength);
	float					GetScrollLineInterval(int nScrollLineNum);
	int						GetScrollLine();

	BOOL					CheckINFPosition(POINT pt, BYTE byType);

protected:
	CINFImageEx*				m_ImgScrollBar;
	CINFImageEx*				m_ImgScrollLine;
	CINFImageEx*				m_ImgScrollTopBtn[4];
	CINFImageEx*				m_ImgScrollBottomBtn[4];

	int					*	m_nParentX;
	int					*	m_nParentY;
	int						m_nStartX;
	int						m_nStartY;

	int						m_nTopBtnSize;
	int						m_nBottomBtnSize;

	int						m_nScrollMaxCount;				// ��ũ�� ������ ������ ����
	int						m_nScrollMaxLine;				// �ѹ��� �����ִ� ���� ����
	int						m_nScrollLineLength;			// ��ũ�� ����
	int						m_nScrollMoveLength;			// ��ũ���� ������ �� �ִ� ����
	int						m_nScrollBarWidth;
	int						m_nScrollBarHeight;

	int						m_nScrollBtn;					// ��ũ�� ���� ��ġ
	int						m_nScrollPosition;				// ��ũ�� ��ġ
	int						m_nScrollLine;					// ��ũ�� ����

	BYTE					m_byScrollTopBtnState;			// ��ư ���� ����
	BYTE					m_byScrollBottomBtnState;
	BOOL					m_bScrollLock;					// ��ũ�� �����°�

};

#endif // !defined(AFX_INFSCROLLBAR_H__B95568B8_C946_4170_92DA_D3D6155D1C0D__INCLUDED_)

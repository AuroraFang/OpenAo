// INFAutoEditBox.h: interface for the CINFAutoEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFAUTOEDITBOX_H__7B7ECFAB_287A_4E78_B040_7ED91DF61790__INCLUDED_)
#define AFX_INFAUTOEDITBOX_H__7B7ECFAB_287A_4E78_B040_7ED91DF61790__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFEditBox.h"

class CINFArenaScrollBar;

class CINFAutoEditBox  : public CINFEditBox
{
public:
	CINFAutoEditBox();
	virtual ~CINFAutoEditBox();

	virtual HRESULT InitDeviceObjects(int nFontHeight, POINT ptEditPos, int nStrWidth, BOOL bUseCap, int nCap, BOOL bCullText=FALSE, int nMouseHeight=-1);		
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	
	virtual void Render();
	virtual void Render(int nScroll, int nOnePageLine);
	virtual void EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus);	
	// 2013-06-17 by bhsohn ���ս� ���� 2���� �������� ���� ó��
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL, BOOL bAddString=FALSE, BOOL bUseBackSpace=TRUE);
	
	// ��ư Ŭ�� 
	virtual BOOL OnLButtonDown(POINT ptPos);	
	virtual BOOL OnLButtonUp(POINT ptPos);
	
	virtual void Tick();

	BOOL OnMouseMove(POINT ptPos);
	BOOL OnMouseWheel(POINT ptPos, WPARAM wParam, LPARAM lParam);
	

	// by mspark, �ΰ��� ����
	INT	AddElement(char* c);
	void ItemAddDone();
	void ItemClear();
	void SetElementArea(int cx,int cy,int cw,int ch);
	char * GetItem(UINT num);
	INT	GetSelect();
	void SetSelect(INT nSelect);
	void SetItemShow(bool bItemShow);
	void SetBGPos(int nBGPosX, int nBGPosY, int nWidth ,int nHeight);
	// by mspark, �ΰ��� ����

	void RenderScroll(int nWndHeight);

	void	SetAutoEditBoxString(char* pEditString);
	char*	GetAutoEditBoxString();

protected:
	
	// by mspark, �ΰ��� ����
	int						m_nSelect;					// ������ �������� ��ȣ.	
	int						m_nRenderSelect;					// ������ �������� ��ȣ.	
	
	int						m_nItemSize;				// �������� ����.
	
	RECT					m_rtElementArea;			// ���ҵ��� ����.	
	CINFImageEx	*			m_pImgElementBG;			// ����Ʈâ�� Ŭ���� ����� ���ҵ��� ���.
	CINFImageEx*			m_pImgHeader;			
	CINFImageEx*			m_pImgSelItem;			
	CINFImageEx*			m_pImgBottom;			

	CINFArenaScrollBar*		m_pScroll;

	CD3DHanFont *			m_pFontItem;
	
	// �޺��ڽ� �����ۼ� 
	BOOL					m_bItemShow;				// �������� �������� �ִ� �����ΰ�?	
//	char					m_szListItem[MAX_SEARCH_ITEM][512];
	vector<string>			m_vecComboItem;
	
	// �������
	char					m_szBlankBuff[512];
	
	// ��� ��ġ
	int						m_nBGPosX;
	int						m_nBGPosY;
	int						m_nBGPosWidth;
	int						m_nBGPosHeight;

	std::string				m_strListString;
	

};

#endif // !defined(AFX_INFAUTOEDITBOX_H__7B7ECFAB_287A_4E78_B040_7ED91DF61790__INCLUDED_)

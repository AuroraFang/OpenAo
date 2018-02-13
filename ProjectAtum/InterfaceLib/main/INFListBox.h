// INFListBox.h: interface for the CINFListBox class.
// // 2007-06-08 by dgwoo ListBox Class
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFLISTBOX_H__3BE779C7_7FA5_4840_843D_05440F356410__INCLUDED_)
#define AFX_INFLISTBOX_H__3BE779C7_7FA5_4840_843D_05440F356410__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

//struct STRUCT_LIST_ITEM
//{
//	char		szListItem[512];
//};

#define		MAX_COMBO_ITEM			25

class CINFImageEx;
class CINFListBox : public CINFBase  
{
public:
	CINFListBox(char * cImgMainName, char * cImgElementBGName);
	virtual ~CINFListBox();
	virtual HRESULT InitDeviceObjects();
	HRESULT			InitDeviceObjects_Select();
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	void			Render();
	void			SetMainArea(int cx,int cy, int cw,int ch);				// ����Ʈ â�� ����.
	void			SetElementArea(int cx,int cy,int cw,int ch);			// Item �Ѱ��� ����
	INT				AddElement(char * c);
	char *			GetItem(UINT num);
	INT				GetSelect();											// 0���� ����.
	void			ItemClear();
	INT				UpdateItem();
	INT				SetSelectItem(UINT nSelect);							// ������ �������� �����Ѵ�.
	VOID			SetMainButtonDisable();
	VOID			SetMainButtonEnisable();

	INT				LButtonDown(POINT pt);
	INT				LButtonUp(POINT pt);
	INT				MouseMove(POINT pt);
	int				WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL			IsShowItem();
	void			ShowItem(BOOL bItemShow);
	void			SetBGPos(int nBGPosX, int nBGPosY, int nWidth ,int nHeight);

	UINT					m_nSelect;					// ������ �������� ��ȣ.	
	
	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	void			SetUseCulling(BOOL i_bUseCulling);	// �ø� �������

private:
	//vector<STRUCT_LIST_ITEM>			m_vecElement;				// ������.
	int						m_nItemSize;					// �������� ����.

	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	BOOL			m_bUseCulling;	// �ø� �������

	
	BOOL					m_bItemShow;				// �������� �������� �ִ� �����ΰ�?
	UINT					m_nMainStateB;				// ����Ʈ ��ư�� ����.

	RECT					m_rtMainArea;				// ����Ʈâ�� ����.
	RECT					m_rtElementArea;			// ���ҵ��� ����.	
	CINFImageEx	*			m_pImgMain[BUTTON_STATE_NUMBER];	// ����Ʈ�ڽ��� â �̹���.
	CINFImageEx	*			m_pImgElementBG;			// ����Ʈâ�� Ŭ���� ����� ���ҵ��� ���.
	CD3DHanFont *			m_pFontItem;

	// �޺��ڽ� �����ۼ� 
	char					m_szListItem[MAX_COMBO_ITEM][512];

	char					m_cMainName[512];
	char					m_cElementBGName[512];
	char					m_cViewData[512];

	// �������
	char					m_szBlankBuff[512];

	// ��� ��ġ
	int						m_nBGPosX;
	int						m_nBGPosY;
	int						m_nBGPosWidth;
	int						m_nBGPosHeight;
	

};

#endif // !defined(AFX_INFLISTBOX_H__3BE779C7_7FA5_4840_843D_05440F356410__INCLUDED_)

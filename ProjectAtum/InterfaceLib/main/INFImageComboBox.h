// INFImageComboBox.h: interface for the CINFImageComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGECOMBOBOX_H__7FBD67FC_0A51_4BED_A6F1_F08078B44772__INCLUDED_)
#define AFX_INFIMAGECOMBOBOX_H__7FBD67FC_0A51_4BED_A6F1_F08078B44772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// ������ ���ϻ�Ȳ
#define		LBTN_COMBO_NONE			-1
#define		LBTN_COMBO_SHOWITEM		1		// ������ ���̱�
#define		LBTN_COMBO_HIDEITEM		2		// ������ �����
#define		LBTN_COMBO_CHANGEITEM		3		// Ŀ������

class CINFImage;
class CINFImageEx;
struct structComboInfo
{
	int				nIdx;			// �̹���
	CINFImage*		pInfImage;			// �̹���
	char			chComboTxt[256];	// ��Ʈ������
	BOOL			bShowItem;			// �������ϋ� ������ ����
};


class CINFImageComboBox  : public CINFBase  
{
public:
	CINFImageComboBox();
	virtual ~CINFImageComboBox();

	virtual HRESULT InitDeviceObjects(POINT ptPos, int nWidth, int nHeight);
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	void			Render();
	
	void			AddItem(int nCnt, char *pComboTxt, CINFImage* pInfImage, BOOL bShowItem);
	void			UpdateItemInfo(int nIdx, BOOL bShowItem);

	BOOL			OnLButtonDown(POINT ptPos);
	BOOL			OnMouseMove(POINT ptPos);
	BOOL			OnLButtonUp(POINT ptPos);
	
	BOOL			IsShowWindow();
	void			ShowWindow(BOOL bShowWnd, POINT *pPos);
	
	BOOL			IsShowItem();
	void			ShowItem(BOOL bShowItem);

	int				GetCurSel();
	void			SetCurSel(int nCurSel);

	void			ResetItem();
	void			SetPos(POINT ptPos);

private:
	vector<structComboInfo*>		m_vecComboInfo;
	
	POINT			m_ptPos;
	int				m_nWidth;
	int				m_nHeight;

	BOOL			m_bShowItem;
	BOOL			m_bShowWnd;

	CD3DHanFont	*	m_pFontTxt;
	int				m_nCurSel;
	int				m_nCurSelItem;

	CINFImageEx*		m_pImgCombo;
	CINFImageEx*		m_pImgSelect;
};

#endif // !defined(AFX_INFIMAGECOMBOBOX_H__7FBD67FC_0A51_4BED_A6F1_F08078B44772__INCLUDED_)

// INFComboBox.h: interface for the CINFComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMBOBOX_H__55DBF5D1_ADAC_46CA_8970_C66946822A97__INCLUDED_)
#define AFX_INFCOMBOBOX_H__55DBF5D1_ADAC_46CA_8970_C66946822A97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CD3DHanFont;
class CAtumNode;
class CINFImage;
class CINFImageEx;
struct COMBO_BOX 
{
	CD3DHanFont* pFont;
	CINFImage* pImage;	// �ܺ� �̹���(����� ���� �ʿ䰡 ����)
	char szString[64];
	DWORD dwData;
};

class CINFComboBox : public CINFBase  
{
public:
	CINFComboBox(CAtumNode* pParent, int nStartX, int nStartY, int nSizeX, int nSizeY, int nDefaultSelect=0);	// ������ǥ�� ��ĭ ������
	virtual ~CINFComboBox();

	void Init();
	void AddString(char* szString, CINFImage* pImage = NULL, DWORD dwData=0);
	BOOL IsSelectChanged() { BOOL b = m_bSelectChanged; m_bSelectChanged = FALSE; return b; }
	DWORD GetCurSelData();
	int  GetCurSelIndex() { return m_nCurrentSelectIndex; }
	void SetCurSelData(int nIndex) { m_nCurrentSelectIndex = nIndex; }
	void SetStartPos(int nStartX, int nStartY) { m_nStartX = nStartX; m_nStartY = nStartY; }

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	void Render(BOOL bShowString);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	void RenderComboList();

protected:
	CINFImageEx					*m_pImgCombo;
	CINFImageEx					*m_pImgSelect;
	vector<COMBO_BOX*>			m_vecComboBox;
	int							m_nStartX;				// �� ��ĭ(�⺻ �ڽ�) ������
	int							m_nStartY;
	int							m_nSizeX;				// ��ĭ ������
	int							m_nSizeY;
	int							m_nCurrentSelectIndex;	// ���� ���õ� �ε���
	int							m_nMoveSelectIndex;		// �����̴� ��� ���õ� �ε���
	BOOL						m_bShowList;
	BOOL						m_bSelectChanged;
};

#endif // !defined(AFX_INFCOMBOBOX_H__55DBF5D1_ADAC_46CA_8970_C66946822A97__INCLUDED_)

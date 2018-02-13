// INFChangeCharactor.h: interface for the CINFChangeCharactor class.
//	// 2007-11-21 by dgwoo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCHANGECHARACTOR_H__27F1FF14_9E52_4B46_A278_D4CC4708AFCB__INCLUDED_)
#define AFX_INFCHANGECHARACTOR_H__27F1FF14_9E52_4B46_A278_D4CC4708AFCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"

class CD3DHanFont;
class CINFImage;
class CINFImageEx;							 // 2011. 10. 10 by jskim UI�ý��� ����
class CINFGroupImage;

#define CHARACTOR_FACE_SHOW_MAX				5

typedef struct {
	CINFImageEx*	pImgFace;
	char			szName[SIZE_MAX_CHARACTER_NAME];
	DWORD			nFaceNum;

} FACE_IMG_NUM;


class CINFChangeCharactor : public CINFBase  
{
public:
	CINFChangeCharactor(CAtumNode * pParent);
	virtual ~CINFChangeCharactor();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetActive(BOOL i_bActive,UID64_t i_nUniqueNum = 0);
	BOOL GetActive()					{ return m_bActive;	}
	void SendChangeCharactor();
	void SetSelectFaceNum();

protected:
	CAtumNode			*			m_pParent;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM			// 2011. 11. 17 by jskim EP4 UI ����
	CINFGroupImage		*			m_pImgBG;						// ĳ������ �������� ���.
#else
	CINFImageEx			*			m_pImgBG;						// ĳ������ �������� ���.
#endif
	CINFImageEx			*			m_pImgSelect;					// ������ �̹���.
	CINFImageBtn		*			m_pFemaleRightButton;			// ���� ������ �ѱ� ��ư.
	CINFImageBtn		*			m_pFemaleLeftButton;			// ���� ������ �ѱ� ��ư.
	CINFImageBtn		*			m_pMaleRightButton;				// ���� ������ �ѱ� ��ư.
	CINFImageBtn		*			m_pMaleLeftButton;				// ���� ������ �ѱ� ��ư.
	CINFImageBtn		*			m_pChangeButton;				// �����ϱ� ��ư.
	CINFImageBtn		*			m_pCancelButton;				// ��� ��ư.
	CD3DHanFont			*			m_pFontName;					// ĳ���͵��� �̸�.

	vector<FACE_IMG_NUM>			m_vecFaceFemale;				// ���� ��.
	vector<FACE_IMG_NUM>			m_vecFaceMale;					// ���� ��.

	BOOL							m_bActive;
	UID64_t							m_nUniqueNum;						// ����� �������� ��ȣ.

	//--------------------------------------------------------------------------//
	//								 DATA										//
	DWORD							m_nWindowX;						// ��ü â�� X��ǥ.
	DWORD							m_nWindowY;						// ��ü â�� Y��ǥ.

	INT								m_nSelect;						// ������ ĳ���Ͱ� ���°��?
	INT								m_nSelectFaceNum;				// ������ ĳ������ FaceNum
	BOOL							m_bFemale;						// �������̳�?
	
	INT								m_nStartFemale;					// ó�� �����ϴ� ���� ��ũ���� ��ġ.
	INT								m_nStartMale;					// ó�� �����ϴ� ���� ��ũ���� ��ġ.


	//--------------------------------------------------------------------------//
	


};

#endif // !defined(AFX_INFCHANGECHARACTOR_H__27F1FF14_9E52_4B46_A278_D4CC4708AFCB__INCLUDED_)

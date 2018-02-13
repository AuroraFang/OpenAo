// INFImageRadioBtn.h: interface for the CINFImageRadioBtn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGERADIOBTN_H__B94678DE_28BD_4F24_BF13_834099A30038__INCLUDED_)
#define AFX_INFIMAGERADIOBTN_H__B94678DE_28BD_4F24_BF13_834099A30038__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// ���� ��ư ����
#define	RADIO_STATUS_OFF		0
#define	RADIO_STATUS_ON			1
#define	MAX_RADIO_STATUS		2

class CINFImageRadioBtn  : public CINFBase
{
public:
	CINFImageRadioBtn();
	virtual ~CINFImageRadioBtn();

	HRESULT InitDeviceObjects(char* pRadioup, char* pRadioSel);	
	HRESULT InitDeviceObjects_LoadSelect(char* pRadioup, char* pRadioSel);
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void SetPosition(float i_fPosX, float i_fPosY, float i_fStrCap);
	void SetRadioBtn(BOOL i_bSelRadio);		// ���� ��ư ���� 
	int GetRadioBtn();		// ���� ��ư ���� 
	BOOL OnLButtonDown(POINT pt);

	// 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
	void ShowWindow(BOOL bShowWnd);

private:	
	CINFImageEx	*	m_pImageRadio[MAX_RADIO_STATUS];	

	int				m_nRadioStatus;		// ���� ��ư ����

	float		m_fPosX;
	float		m_fPosY;	
	
	float		m_fWidth;
	float		m_fHeight;	

	// 2012-10-17 by bhsohn ������ �̸����� ������ ON���� ����
	BOOL		m_bShowWnd;

};

#endif // !defined(AFX_INFIMAGERADIOBTN_H__B94678DE_28BD_4F24_BF13_834099A30038__INCLUDED_)

// INFToolTip.h: interface for the CINFToolTip class.
//	2007-09-19 by dgwoo ���� ������ ȭ�鿡 �̹����� ��� Ŭ����.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFTOOLTIP_H__DC6DA211_4954_4DFC_8831_7850FC864D34__INCLUDED_)
#define AFX_INFTOOLTIP_H__DC6DA211_4954_4DFC_8831_7850FC864D34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
#include "D3DHanFont.h"

#define TOOLTIP_FONTSIZE			512

class CINFImage;
class DataHeader;
class CGameData;
class CINFImageEx;								   // 2011. 10. 10 by jskim UI�ý��� ����


class CINFToolTip : public CINFBase  
{
public:
	CINFToolTip();
	virtual ~CINFToolTip();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();

	void SetImgType(UINT i_nType);

	map<UINT, CINFImageEx*> m_mapToolTip;				// ���� ������ �������ִ� map
	CINFImageEx*	m_pImgTemp;						// ���� ��ϵ� �̹��� �ּҰ�.
	BOOL			m_IsShow;						// ���� �������� �ϴ°�?
	float			m_fLifeTime;					// �������� ���� �ð�.
// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	CD3DHanFont*    m_pToolTipFont;			    	// ��ư ����
	vector< pair< string, string > >		m_vecToolTipName;		// 
	BOOL					m_bToolTipState;
	POINT					m_nPoint;
	char					m_pViewName[TOOLTIP_FONTSIZE];

	void   SetNameToolTip(POINT pt, char* strKey);
	TCHAR* ExecuteDirFullName(); /* ini���� ��� */
	int OutIniInt( TCHAR* _lpAppName, TCHAR* _lpKeyName ); /* ini���� ���ڰ� �б� */
	void OutIniString( TCHAR* _lpAppName, TCHAR* _lpKeyName, TCHAR* _lpReturnedString, int _nSize );
// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
};

#endif // !defined(AFX_INFTOOLTIP_H__DC6DA211_4954_4DFC_8831_7850FC864D34__INCLUDED_)

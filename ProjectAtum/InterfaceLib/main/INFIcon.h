// INFIcon.h: interface for the CINFIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFICON_H__76C63A2B_57D9_485B_BE09_08B60678507E__INCLUDED_)
#define AFX_INFICON_H__76C63A2B_57D9_485B_BE09_08B60678507E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

class CINFImage;
class CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
class CGameData;
class CINFIcon : public CAtumNode  
{
protected:
	map<string,CINFImageEx*>	m_mapIcon;		// 2011. 10. 10 by jskim UI�ý��� ����
	
	char						m_strIconName[20];
	int							m_nX, m_nY;
	float						m_fScale;
	CGameData				*	m_pGameData ;

public:
	CINFIcon();
	virtual ~CINFIcon();

	void SetIcon(char* strName, int x, int y, float fScale );
	CINFImageEx* FindIcon(char* strName);		// 2011. 10. 10 by jskim UI�ý��� ����
	HRESULT	SetResourceFile(char* szFileName) ;

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	POINT GetIconSize();
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	virtual void Render();



};

#endif // !defined(AFX_INFICON_H__76C63A2B_57D9_485B_BE09_08B60678507E__INCLUDED_)

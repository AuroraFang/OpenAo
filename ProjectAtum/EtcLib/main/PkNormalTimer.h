// PkNormalTimer.h: interface for the CPkNormalTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PKNORMALTIMER_H__E5BB3127_6832_4691_82DF_25D66169C3CF__INCLUDED_)
#define AFX_PKNORMALTIMER_H__E5BB3127_6832_4691_82DF_25D66169C3CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windows.h"

class CPkNormalTimer  
{
public:
	CPkNormalTimer();
	virtual ~CPkNormalTimer();
	void		SetPkDelay();
	void		ReleasePkDelay();
	void		SetCityWar();
	void		ReleaseCityWar();
	void		SetImmediatePk();						// ��� ���� ����
	void		Tick(float fElapsedTime);
	BOOL		IsPkEnableNormalOrderTarget();
	BOOL		IsCityWar() { return m_bIsCityWar; }	// ���� ������ ���ΰ�?
	
private:
	float		m_fPkDelayTime;				// PK�� 5�� ������
	BOOL		m_bPkDelayStart;			// PK ������ ����
	BOOL		m_bIsCityWar;				// ������ ���ΰ�?
	DWORD		m_dwPkDelayTimeState;		// �ð� ǥ�� ����
};

#endif // !defined(AFX_PKNORMALTIMER_H__E5BB3127_6832_4691_82DF_25D66169C3CF__INCLUDED_)

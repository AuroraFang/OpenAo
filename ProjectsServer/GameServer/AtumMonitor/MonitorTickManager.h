// MonitorTickManager.h: interface for the CMonitorTickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORTICKMANAGER_H__13683692_4749_476F_8093_A587139ABF42__INCLUDED_)
#define AFX_MONITORTICKMANAGER_H__13683692_4749_476F_8093_A587139ABF42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"

class CMonitorServerNode;

class CMonitorTickManager : public CTickManager  
{
public:
	CMonitorTickManager();
	virtual ~CMonitorTickManager();

	BOOL InitTickManager(CMonitorServerNode *i_pMonitorServerNode);
	void CleanTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) {}
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// �Ź� �����ؾ� �� ��
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// �Ϸ翡 �� �� �� �ؾ� �� ��
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// �� �ð����� �� �� �� �ؾ� �� ��
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// �� �и��� �� �� �� �ؾ� �� ��
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// �� �ʸ��� �� �� �� �ؾ� �� ��
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime){};	// 2005-12-27 by cmkwon, �Ѵ޿� �� �� �� �ؾ� �� ��

public:
	HANDLE		m_hFile;
	CMonitorServerNode	*m_pMonitorServerNode;
};

#endif // !defined(AFX_MONITORTICKMANAGER_H__13683692_4749_476F_8093_A587139ABF42__INCLUDED_)

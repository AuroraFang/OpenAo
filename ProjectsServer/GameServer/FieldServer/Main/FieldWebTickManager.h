#ifndef _ATUM_WEB_TICK_MANAGER_H_
#define _ATUM_WEB_TICK_MANAGER_H_

#include "TickManager.h"

class CFieldWebIOCP;

class CFieldWebTickManager : public CTickManager
{
public:
	CFieldWebTickManager(CFieldWebIOCP *i_pFieldWebIOCP, DWORD i_nTickInterval = 1000);
	virtual ~CFieldWebTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) {}
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// �Ź� �����ؾ� �� ��
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// �Ϸ翡 �� �� �� �ؾ� �� ��
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// �� �ð����� �� �� �� �ؾ� �� ��
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// �� �и��� �� �� �� �ؾ� �� ��
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// �� �ʸ��� �� �� �� �ؾ� �� ��
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime){};	// �Ѵ޿� �� �� �� �ؾ� �� ��

public:
	CFieldWebIOCP *m_pFieldWebIOCP1;
};

#endif // _ATUM_WEB_TICK_MANAGER_H_

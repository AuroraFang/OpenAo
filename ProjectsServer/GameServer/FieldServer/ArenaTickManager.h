// ArenaTickManager.h: interface for the CArenaTickManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENATICKMANAGER_H__ED1FDFDA_5533_4DD7_B44A_F9954C8A611A__INCLUDED_)
#define AFX_ARENATICKMANAGER_H__ED1FDFDA_5533_4DD7_B44A_F9954C8A611A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"
#include "ArenaManager.h"

class CArenaTickManager  : public CTickManager
{
public:
	CArenaTickManager(CArenaManager *i_pArenaManager, DWORD i_nTickInterval = ARENA_TICK_INTERVAL);
	virtual ~CArenaTickManager();

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);	// �Ź� �����ؾ� �� ��
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);		// �Ϸ翡 �� �� �� �ؾ� �� ��
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);		// �� �ð����� �� �� �� �ؾ� �� ��
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);		// �� �и��� �� �� �� �ؾ� �� ��
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);		// �� �ʸ��� �� �� �� �ؾ� �� ��
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);
public:
	CArenaManager			*m_pArenaManager;
};

#endif // !defined(AFX_ARENATICKMANAGER_H__ED1FDFDA_5533_4DD7_B44A_F9954C8A611A__INCLUDED_)

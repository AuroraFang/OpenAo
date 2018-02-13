#include "PreIOCP.h"
#include "TickManager.h"

class CPreTickManager : public CTickManager
{
public:
	explicit CPreTickManager(CPreIOCP *i_pPreIOCP, DWORD i_nTickInterval = 1000) : CTickManager { i_nTickInterval }, m_pPreIOCP1 { i_pPreIOCP } { }
	
	~CPreTickManager() = default;

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) override { }
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime) override { }	// �Ź� �����ؾ� �� ��
	void DoDailyWork(ATUM_DATE_TIME *pDateTime) override { }		// �Ϸ翡 �� �� �� �ؾ� �� ��
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime) override { }		// �� �ð����� �� �� �� �ؾ� �� ��
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime) override { m_pPreIOCP1->OnDoMinutelyWorkIOCP(pDateTime); }		// �� �и��� �� �� �� �ؾ� �� ��
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime) override { }		// �� �ʸ��� �� �� �� �ؾ� �� ��
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime) override { }	// 2005-12-27 by cmkwon, �Ѵ޿� �� �� �� �ؾ� �� ��

	CPreIOCP* m_pPreIOCP1;
};

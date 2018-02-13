#ifndef _TICK_MANAGER_H_
#define _TICK_MANAGER_H_

#include "AtumParam.h"
#include "mt_stl.h"

#include <mutex>
#include <thread>

struct TICK_EVENT
{
	INT		TickEventType;
	INT		nTickEventParam1;
	INT		nTickEventParam2;
	void	*vpTickEventParam1;
	void	*vpTickEventParam2;
};

class CTickManager
{
public:
	explicit CTickManager(DWORD i_nTickInterval);
	virtual ~CTickManager() { CleanThread(); }

	DWORD ThreadWorker();

	void SetTickInterval(DWORD i_nTickInterval) { m_nTickInterval = i_nTickInterval; }
	DWORD GetTickInterval() const { return m_nTickInterval; }

	virtual void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent) = 0;
	virtual void DoEveryTickWork(ATUM_DATE_TIME *pDateTime) = 0;	// �Ź� �����ؾ� �� ��
	virtual void DoDailyWork(ATUM_DATE_TIME *pDateTime) = 0;		// �Ϸ翡 �� �� �� �ؾ� �� ��
	virtual void DoHourlyWork(ATUM_DATE_TIME *pDateTime) = 0;		// �� �ð����� �� �� �� �ؾ� �� ��
	virtual void DoMinutelyWork(ATUM_DATE_TIME *pDateTime) = 0;		// �� �и��� �� �� �� �ؾ� �� ��
	virtual void DoSecondlyWork(ATUM_DATE_TIME *pDateTime) = 0;		// �� �ʸ��� �� �� �� �ؾ� �� ��
	virtual void DoMonthlyWork(ATUM_DATE_TIME *pDateTime) = 0;		// 2005-12-27 by cmkwon, �Ѵ޿� �� �� �� �ؾ� �� ��

	void InsertTickEvent(INT			i_TickEventType,
						ATUM_DATE_TIME	*i_pStartTime,
						INT				i_nTickEventParam1 = 0,
						INT				i_nTickEventParam2 = 0,
						void			*i_vpTickEventParam1 = NULL,
						void			*i_vpTickEventParam2 = NULL);

	void InsertTickEvent(INT			i_TickEventType,
						INT				i_nTimeIntervalInSeconds,
						INT				i_nTickEventParam1 = 0,
						INT				i_nTickEventParam2 = 0,
						void			*i_vpTickEventParam1 = NULL,
						void			*i_vpTickEventParam2 = NULL);

private:
	void ProcessTickEvent(ATUM_DATE_TIME *i_pDateTime);

public:
	ATUM_DATE_TIME	m_nPrevDateTime;
	DWORD			m_nTickInterval;	// in milli-seconds

private:
	mt_multimap<ATUM_DATE_TIME, TICK_EVENT>	m_multimapTickEvent;

public:
	bool InitThread();
	bool CleanThread();

	void SetShutDownFlag()
	{
		lock_guard<mutex> { m_mutexWorker };

		m_flagWorker = true;
	}

	bool GetShutDownFlag()
	{
		lock_guard<mutex> { m_mutexWorker };

		return m_flagWorker;
	}

	cv_status SleepAndWaitForEvent(DWORD duration)
	{
		unique_lock<mutex> lock { m_mutexWorker };

		return m_eventWorker.wait_for(lock, chrono::milliseconds(duration));
	}

protected:
	thread			m_threadWorker;
	DWORD			m_dwThreadID;

private:
	using condition = condition_variable;

	mutex			m_mutexWorker;
	bool			m_flagWorker;
	condition		m_eventWorker;
};

#endif // _ATUM_TICK_MANAGER_H_

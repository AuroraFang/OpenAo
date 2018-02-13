#ifndef _FIELD_TIMER_MANAGER_H_
#define _FIELD_TIMER_MANAGER_H_

#include "FieldDataManager.h"
#include "AtumProtocol.h"

///////////////////////////////////////////////////////////////////////////////
// 2006-04-21 by cmkwon, ����(���� ���¿����� 3�ʴ� 27�� ȸ��)
#define HP_RECOVERY_INTERVAL			3000	// ms
#define HP_MIN_RECOVERY_INTERVAL		250		// ms // 2006-12-13 by cmkwon, �ּҰ�

///////////////////////////////////////////////////////////////////////////////
// 2006-04-21 by cmkwon, ����(���� ���¿��� 3�ʴ� 27�� ȸ��, ���� ���¿����� 3�ʴ� 14�� ȸ��)
#define DP_RECOVERY_INTERVAL			3000	// ms
#define DP_MIN_RECOVERY_INTERVAL		250		// ms // 2006-12-13 by cmkwon, �ּҰ�
#define DP_RECOVERY_PENALTY_SPEED		50

///////////////////////////////////////////////////////////////////////////////
// 2006-04-21 by cmkwon, ����(���¿� ������� 3�ʴ� 2�� ȸ��)
// 2006-09-27 by cmkwon, ����(3�ʴ� 3������ ����)
#define SP_RECOVERY_INTERVAL			3000	// ms
#define SP_MIN_RECOVERY_INTERVAL		250		// ms // 2006-12-13 by cmkwon, �ּҰ�

#define EP_CONSUMPTION_VALUE			5.0f	// 2005-08-04 by cmkwon, ����Ҹ��� ������ ���δ�(10 ==> 5)
#define EP_CONSUMPTION_DISTANCE			30000
#define EP_CONSUMPTION_TIME				60000	// 60�� = 1��

typedef mt_list<MEX_TIMER_EVENT>	mtlistMexTimerEvent;

class CFieldItemManager;

class CFieldTimerManager : public CFieldDataManager  
{
public:
	CFieldTimerManager();

	void ResetTimerData();

	// TimerEvent ��� ó�� �Լ�
	void HandleTimerEventRecoverHP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventRecoverDP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventRecoverSP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventDecreaseSP(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);

	void HandleTimerEventDeleteMine(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventDeleteDummy(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventDeleteFixer(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventDeleteDecoy(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventSequential(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventRequestTimeOut(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);
	void HandleTimerEventTest(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);		// �׽�Ʈ �� ������
	void HandleTimerEventDoMinutelyWork(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	
	void HandleTimerEventDotSkillStealing(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);	// Stealing ��ų - SP���� (SP�� ���� �Ǿ ��� ���� �Ǿ�� �Ѵ�.)
	void HandleTimerEventDotSkillDrain(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);	// Drain ��ų - �ǰݴ�� HP���ҽ��� ���ݴ�󿡰� ä���ش�.
	void HandleTimerEventDotSkillTimeBomb(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);	// TimeBomb ��ų

	void HandleTimerFixedTermShape(MEX_TIMER_EVENT *pTimerEvent, TimeUnit_t currentTime);			// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)

	// Ÿ�̸� ���� �Լ�
	UID32_t StartTimerField(TimerEventType i_type, TimeUnit_t i_nInterval, UINT i_nParam1 = 0, UINT i_nParam2 = 0, UINT i_nParam3 = 0, INT i_nRepeatCount = 0);
	void StartTimerFieldNotCreateTimerUID(TimerEventType i_type, TimeUnit_t i_nInterval, UID32_t i_nTimerUID, UINT i_nParam1 = 0, UINT i_nParam2 = 0, UINT i_nParam3 = 0, INT i_nRepeatCount = 0);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ܺο��� TimerUID �Է� �޾� ����ϴ� �Լ� �߰�
	BOOL StopTimerField(UID32_t i_nTimerUID);
	UID32_t UpdateTimerField(UID32_t i_nTimerUID);		// ����: 0�� return�Ǹ� �ش� TimerEvent�� ���� �Լ����� delete��!
	UID32_t PauseTimerField(UID32_t i_nTimerUID);
	UID32_t ContinueTimerField(UID32_t i_nTimerUID);

	int PauseAllTimerEvents(TIMER_EVENT_4_EXCHANGE *ArrTimerEvent4Exchange = NULL);		// pause�� ������ TimerEvent�� ������ ��ȯ
	int ContinueAllTimerEvents();	// continue�� TimerEvent�� ������ ��ȯ

	const MEX_TIMER_EVENT* GetTimerEventByEventID(UID32_t i_nTimerUID);
	const MEX_TIMER_EVENT* GetTimerEventByItemNum(INT i_nItemNum);
	BOOL DeleteTimerEventByTimerUID(UID32_t i_nTimerUID);

	BOOL CheckRecoveryTimer(TimerEventType TEType, BOOL i_bFlagCheck=TRUE);

public:
	// TimerEvent ����
	CUID32Generator			m_TimerUIDGenerator;
	mtlistMexTimerEvent		m_listWatingTimerEvents;	// ������� Timer Event�� ����Ʈ

	// Timer ����
	DWORD					m_dwDoMinutelyCounts;

	BOOL					m_bRecoverHPTimerRunning;
	DWORD					m_dwRecoverHPRunningTick;		// 2005-12-01 by cmkwon
	UINT					m_uiHPTimerUID;					// 2006-09-27 by cmkwon

	BOOL					m_bRecoverDPTimerRunning;
	DWORD					m_dwRecoverDPRunningTick;		// 2005-10-26 by cmkwon
	UINT					m_uiDPTimerUID;					// 2006-09-27 by cmkwon

	BOOL					m_bRecoverSPTimerRunning;
	DWORD					m_dwRecoverSPRunningTick;		// 2005-12-01 by cmkwon
	UINT					m_uiSPTimerUID;					// 2006-09-27 by cmkwon
	
	BOOL					m_bDoMinutelyWorkRunning;
	BOOL					m_bImmediateHPTimerRunning;
	BOOL					m_bImmediateDPTimerRunning;
	BOOL					m_bImmediateSPTimerRunning;
	BOOL					m_bImmediateEPTimerRunning;
	
	int						m_nRemainedTimeOfGradualHPUP;	// 0�̸� ����(or cancel)��
	int						m_nRemainedTimeOfGradualDPUP;	// 0�̸� ����(or cancel)��
	int						m_nRemainedTimeOfGradualSPUP;	// 0�̸� ����(or cancel)��
	int						m_nRemainedTimeOfGradualEPUP;	// 0�̸� ����(or cancel)��


	// timer event�� �ߺ��� ���� ����
	UID32_t					m_TEIDGradualHPUP;	// GradualHPUP�� �ߺ��� ���� ����

	// DP �Ҹ� ����
	float					m_fSumDistanceForDPConsumption;		// DP �Ҹ�� ĳ������ �̵� �Ÿ��� ��
	float					m_fDPRecoveryWeight;				// ���ݽ� ȸ�� �ð��� ���̱� ���� ���

	// EP �Ҹ� ����
	float					m_fSumDistanceForEPConsumption;		// EP �Ҹ�� ĳ������ �̵� �Ÿ��� ��
	TimeUnit_t				m_EPConsumptionTimeStamp;			// EP �Ҹ�� �ð�

};

#endif // _FIELD_TIMER_MANAGER_H_

// TriggerFunctionCrystal.h: interface for the CTriggerFunctionCrystal class.
//
/// \brief		// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
/// \author		hskim
/// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERFUNCTIONCRYSTAL_H__3DEB20C4_4877_47F0_A41A_EED0662EB3CE__INCLUDED_)
#define AFX_TRIGGERFUNCTIONCRYSTAL_H__3DEB20C4_4877_47F0_A41A_EED0662EB3CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TriggerFunction.h"

#define EVENT_OPTION_NONE					0		// �ɼ� - ����
#define EVENT_OPTION_HELL_WARP_CLOSE		1		// �ɼ� - ���� �ݱ�
#define EVENT_OPTION_HELL_BACK_MAP_WARP		2		// �ɼ� - ��� ���� Back Map ���� ����

class CTriggerCrystalGroup;

class CTriggerFunctionCrystal : public CTriggerFunction  
{
	friend class CTriggerCrystalGroup;
	friend class CTriggerCrystalDestroyGroup;

public:
	CTriggerFunctionCrystal();
	virtual ~CTriggerFunctionCrystal();

	void OnClear();
	void OnDestroy();
	BOOL OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType);

	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);

	void OnEventSkip(BOOL bCheckCreateCrystal = TRUE);		// 2012-12-17 by hskim, NPC ���� ����۽� - Ʈ���� �ý��� �ʱ�ȭ ����

	BOOL OnMonsterCreate(CFieldMonster *pMonster);
	BOOL OnMonsterDead(CFieldMonster *pMonster);
	BOOL OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage);
	BOOL OnIsInvincible(MonIdx_t MonsterIdx, CFieldIOCPSocket *pIOCPSocket);
	BOOL OnIsCrystal();	// 2013-01-23 by jhseol, ���� �� ũ����Ż�� ���� �Ұ��� �ϵ��� ����

public:
	void SetCrystalInfo(CrystalGroupID_t CrystalGroupID, INT PeriodTime, BYTE RandomSequence);
	BOOL InsertTriggerCrystal(CTriggerCrystalGroup *pTriggerCrystalGroup);

	BOOL IsCreatedCrystal()		{ return m_bCreatedCrystal; }
	BOOL IsBelongCrystal(MonIdx_t MonsterIdx);

	void ResetLocalData();				// 2012-12-17 by hskim, NPC ���� ����۽� - Ʈ���� �ý��� �ʱ�ȭ ����
	void ResetEventData();

	void SetNextCreateTime();
	void FindFirstDamageUser();			// ���� ������ ĳ���� 1���� ã�´�
	void FindWinnerInfluence();			// ���� ������ ���� 1���� ã�´�

	void InsertDestroyedSequence(MonIdx_t MonsterIdx);

protected:
	BOOL RunDestroyEvent();
	BOOL SetAllShopRemainCountZero();

	void RunEvent(EventID_t EventID);
	void StepEvent(EventID_t EventID, INT EventOption = EVENT_OPTION_NONE);
	void StopEvent(EventID_t EventID);

	void MixEvent();					// ũ����Ż �̺�Ʈ ����

	// 1�� �̺�Ʈ

	void EventHellWarp(BOOL bStart, INT EventOption = EVENT_OPTION_NONE);
	void EventSummonBossMonster(BOOL bStart);
	void EventShopKit(BOOL bStart);
	void EventExp100(BOOL bStart);
	void EventShopHyperCard(BOOL bStart);
	void EventShopSeal(BOOL bStart);
	void EventDrop100(BOOL bStart);
	void EventExpSPIDrop100(BOOL bStart);

	// 2�� �̺�Ʈ

	void Event2NDWarPoint(BOOL bStart);
	void Event2NDSummonBossMonster(BOOL bStart);
	void Event2NDExp150(BOOL bStart);
	void Event2NDDrop150(BOOL bStart);
	void Event2NDExpSPIDrop150(BOOL bStart);
	void Event2NDCapsuleItem(BOOL bStart);
	void Event2NDShopPrefixSuffix(BOOL bStart);
	void Event2NDShopKit(BOOL bStart);
	void Event2NDShopHyperCard(BOOL bStart);
	void Event2NDShopWepone(BOOL bStart);
	void Event2NDHellWarp(BOOL bStart, INT EventOption = EVENT_OPTION_NONE);

	void SendNotifyHappyHourEvent(float fEXPRate2, float fSPIRate2, float fEXPRepairRate2, float fDropItemRate2, float fDropRareRate2, float fWarPointRate2);
	void SendNotifyHappyHourEvent2ND(float fEXPRate2, float fSPIRate2, float fEXPRepairRate2, float fDropItemRate2, float fDropRareRate2, float fWarPointRate2);

protected:
	CrystalGroupID_t m_CrystalGroupID;
	INT					m_PeriodTime;
	BYTE				m_RandomSequence;

	BOOL m_bCreatedCrystal;

	CTriggerCrystalGroup			*m_pDefaultCrystalGroup;			// �⺻ �ı��� ũ����Ż �׷� ����	
	vector<CTriggerCrystalGroup *>	m_vectorTriggerCrystal;				// �ʱ� ���ý� �����ǰ� ������� �ʴ´�. ����ȭ �ʿ� ����

	mtvectTriggerAttackUserDamage	m_mtvectTriggerAttackUserDamage;	// �� ������ ����
	vectorDestroyedSequence			m_vectorDestroyedSequence;			// �ı��� ũ����Ż (������� ����)

protected:
	UID32_t		m_FirstCharacterUniqueNumber;		// 1���� ĳ������ ������ȣ
	UID32_t		m_FirstGuildUniqueNumber;			// 1���� ĳ������ ��� ��ȣ
	BYTE		m_FirstInfluenceType;				// 1���� ĳ������ ���� Ÿ��

	BYTE		m_WinnerInfluenceType;				// �̱� ����

	EventID_t	m_ProcessEventID;					// �������� �̺�Ʈ

	TimeUnit_t	m_NextTickEventCreate;				// ���� �̺�Ʈ �ð�	- ���� �̺�Ʈ		GetTickCount()
	TimeUnit_t	m_NextTickEvent1st;					// ���� �̺�Ʈ ���� - ù��° �̺�Ʈ		GetTickCount()
	TimeUnit_t	m_NextTickEvent2nd;					// ���� �̺�Ʈ ���� - �ι�° �̺�Ʈ		GetTickCount()
};

#endif // !defined(AFX_TRIGGERFUNCTIONCRYSTAL_H__3DEB20C4_4877_47F0_A41A_EED0662EB3CE__INCLUDED_)

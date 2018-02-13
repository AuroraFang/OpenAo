// TriggerFunctionNGCInflWar.h: interface for the CTriggerFunctionNGCInflWar class.
//
/// \brief		// 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
/// \author		hskim
/// \date		2011-11-07
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERFUNCTIONNGCINFLWAR_H__3705FC12_22C8_4256_9ED5_F1527275D0CF__INCLUDED_)
#define AFX_TRIGGERFUNCTIONNGCINFLWAR_H__3705FC12_22C8_4256_9ED5_F1527275D0CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TriggerFunction.h"

class CTriggerNGCInflWarGroup;

class CTriggerFunctionNGCInflWar : public CTriggerFunction  
{
public:
	CTriggerFunctionNGCInflWar();
	virtual ~CTriggerFunctionNGCInflWar();

	void OnClear();
	void OnDestroy();
	BOOL OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType);

	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);

	void OnEventSkip(BOOL bCheckCreateCrystal = TRUE);		// 2012-12-17 by hskim, NPC ���� ����۽� - Ʈ���� �ý��� �ʱ�ȭ ����

	BOOL OnMonsterCreate(CFieldMonster *pMonster);
	BOOL OnMonsterDead(CFieldMonster *pMonster);
	BOOL OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage);

public:
	void SetNGCInflWarInfo(NGCInflWarGroupID_t NGCInflWarGroupID, INT PeriodTime, MonIdx_t BossMonster, INT WinInfluenceWP, INT LossInfluenceWP, INT FirstDamageGuildWP, INT FixedTimeType, INT i_nDestroyCrystalcount);	// 2013-07-08 by jhseol, Ʈ���� �ý��� Ȯ��
	BOOL InsertNGCInflWarMonster(CTriggerNGCInflWarMonsterGroup *pTriggerNGCInflWarGroup);

	BOOL IsCreatedNGCInflWar()		{ return m_bCreatedNGCInflWar; }

	void ResetLocalData();		// 2012-12-17 by hskim, NPC ���� ����۽� - Ʈ���� �ý��� �ʱ�ȭ ����

	void SetNextCreateTime();

	void FindFirstDamageUser();		// ���� ������ 1�� ���� ã��
	void FindSequenceAmountAttack();	// ���� ������ ���� ������ �������� ������ ���� ����Ʈ
	void FindWinInfluenc();			// �̱� ���� ã��

	// 2013-07-08 by jhseol, Ʈ���� �ý��� Ȯ��
	void InsertBossMonsterKillInfo(STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO* i_BossKillInfo);
	void InitBossKillPlayStep();
	void CreateCrystalMonster();
	void DestroyCrystalMonster();
	void CrystalRegen();
	BOOL IsPossibleBossAttack();
	BOOL OnIsPossibleBossAttack(MonIdx_t MonsterIdx);
	void RunKeyMonsterKillEvent(MonIdx_t i_nMonsterNum);
	void InsertCrystalBuff(STRIGGER_CRYSTAL_BUFF *i_CrystalBuff);
	INT GetDestroyCrystalcount();
	// end 2013-07-08 by jhseol, Ʈ���� �ý��� Ȯ��
protected:
	void RunEvent();
	void StopEvent();

	void CreateAllMonster();
	void DestroyAllMonster();

protected:
	BOOL m_bCreatedNGCInflWar;
	
	NGCInflWarGroupID_t m_NGCInflWarGroupID;
	INT					m_PeriodTime;
	MonIdx_t			m_BossMonster;
	INT					m_WinInfluenceWP;
	INT					m_LossInfluenceWP;
	INT					m_FirstDamageGuildWP;
	INT					m_FixedTimeType;

	mtvectTriggerAttackUserDamage				m_mtvectTriggerAttackUserDamage;		// �� ������ ����
	vector<CTriggerNGCInflWarMonsterGroup *>	m_vectorTriggerNGCInflWarMonster;		// �ʱ� ���ý� �����ǰ� ������� �ʴ´�. ����ȭ �ʿ� ����

protected:
	TimeUnit_t	m_NextCreateTick;

	UID32_t		m_FirstCharacterUniqueNumber;		// 1�� ĳ���� ������ȣ
	UID32_t		m_FirstGuildUniqueNumber;			// 1�� ��� ��� ��ȣ
	BYTE		m_WinInfluenceType;					// �̱� ���� Ÿ��
	BYTE		m_DefeatInfluenceType;				// �й� ���� Ÿ��

	mtvectTriggerAttackUserDamage				m_mtvectTriggerDamageSortUser;			// ���� ���Ϳ� ������ ������ ������ �������� ���� ����Ʈ

	// 2013-07-08 by jhseol, Ʈ���� �ý��� Ȯ��
	BOOL		m_bPossibleBossAttack;			// �������� ���� ���� ����
	BOOL		m_bBeforePossibleBossAttack;	// ���� �������� ���� ���� ����
	BOOL		m_bIsBossDestroy;				// ���� �ı� ����
	INT			m_nDestroyCrystalcount;			// �ı��ؾ� �ϴ� ũ����Ż�� ��
	vectorTriggerOutPostBossKillInfo			m_vectTriggerNGCInflWarBossKillInfo;
	mtvectorTriggerOutPostMonsterKillInfo		m_mtvectTriggerNGCInflWarMonsterKillInfo;
	vectorTriggerCrystalBuff					m_vectCrystalBuff;						// ũ����Ż ���� ����Ʈ
	// end 2013-07-08 by jhseol, Ʈ���� �ý��� Ȯ��
};

#endif // !defined(AFX_TRIGGERFUNCTIONNGCINFLWAR_H__3705FC12_22C8_4256_9ED5_F1527275D0CF__INCLUDED_)

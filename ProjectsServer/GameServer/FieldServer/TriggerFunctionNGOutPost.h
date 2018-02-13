// TriggerFunctionNGOutPost.h: interface for the CTriggerFunctionNGOutPost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERFUNCTIONNGOUTPOST_H__02F8538D_6693_439E_A335_4C55F13974B6__INCLUDED_)
#define AFX_TRIGGERFUNCTIONNGOUTPOST_H__02F8538D_6693_439E_A335_4C55F13974B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TriggerFunction.h"
#include "MapTriggerManager.h"
#include "OutPost.h"

class CTriggerNGCInflWarGroup;

class CTriggerFunctionNGOutPost : public CTriggerFunction
{
public:
	CTriggerFunctionNGOutPost();
	virtual ~CTriggerFunctionNGOutPost();

	void OnClear();
	void OnDestroy();
	BOOL OnCreate(CMapTriggerManager *pMapTriggerManager, MapTriggerID_t MapTriggerID, MapIndex_t MapIndex, ChannelIndex_t MapChannel, FunctionID_t FunctionID, MapTriggerType_t TriggerType);
	
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);
	
	void OnEventSkip(BOOL i_bCheckCreateCrystal = TRUE);
	void OnFixedBuffItem(INT i_nFixedBuffItem = 0);
	
	BOOL OnMonsterCreate(CFieldMonster *pMonster);
	BOOL OnMonsterDead(CFieldMonster *pMonster);
	BOOL OnAddDamage(MonIdx_t MonsterIdx, UID32_t CharacterUniqueNumber, UID32_t GuildUniqueNumber, BYTE InfluenceType, float fDamage);
	BOOL OnIsPossibleBossAttack(MonIdx_t MonsterIdx);	// �ܺ� Ŭ�������� ���� ���������� ���� ���� ���θ� Ȯ���� �Լ�
	
public:
	void SetNGCOutPostInfo(SummonMosterGroupID_t i_nSummonMonsterGroupID, MapIndex_t i_nStandardOutPostMap, MonIdx_t i_nBossMonster, INT i_nWinInfluenceWP, INT i_nLossInfluenceWP, MapIndex_t i_nNextOutPostMap, INT i_nDestroyCrystalcount);
	void InsertBossMonsterKillInfo(STRIGGER_TRIGGER_OUTPOST_BOSS_KILL_INFO* i_BossKillInfo);
	BOOL InsertNGCOutPostSummonMonster(CTriggerNGCInflWarMonsterGroup *i_pTriggerFnctionNGCOutPostSummonMonster);
	void InsertCrystalBuff(STRIGGER_CRYSTAL_BUFF *i_vectCrystalBuff);

protected:
	void InitBossKillPlayStep();		// �������� �����ܰ踦 �ʱ�ȭ �Լ�
	void StopEvent();
	
	BOOL IsPossibleBossAttack();		// ���������� ���� ���θ� �Ǵ��ϴ� �Լ�
	
	void RunKeyMonsterKillEvent(MonIdx_t i_nMonsterNum);	// ũ����Ż �ı��� �̺�Ʈ�� ���� ��Ű�� �Լ�
	void RunBossKillEvent();							// �������� ���� ������ �ߵ��Ǵ� �̺�Ʈ �Լ�
	
	BYTE FindWinInfluenc(MonIdx_t i_nMonsterNum);				// �̱� ���� ã��

	void CreateAllMonster();
	void CreateCrystalMonster();
	void DestroyAllMonster();
	void DestroyCrystalMonster();

	void CrystalRegen();
	
protected:

	CFieldIOCP* m_pFieldIOCP;
	CFieldMapChannel	*m_pFieldNextOutPostMapChannel;
	CFieldMapChannel	*m_pFieldOutPostCityMapChannel;

	BOOL m_bTodayTriggerEventPlay;		// ���� ���������� Ʈ���Ű� �ߵ� �Ǿ����� üũ�� ����.
	BOOL m_bTriggerOn;					// Ʈ���� �ߵ� üũ����
	BOOL m_bIsOutPostWarRuning;			// ������������ �������̴�.
	BOOL m_bCommendReset;				// ��ɾ�� Ʈ���� ����
	BOOL m_bPossibleBossAttack;			// �������� ���� ���� ����
	BOOL m_bBeforePossibleBossAttack;	// ���� �������� ���� ���� ����
	BOOL m_bIsBossDestroy;	// ���� �������� ���� ���� ����
	
	SummonMosterGroupID_t		m_nSummonMonsterGroupID;		// ��ȯ ���� �׷� ID
	MapIndex_t					m_nStandardOutPostMap;			// ������ �� �������� ��, �ش� ���������� �߻��� ������ ���� �ð��� Ʈ���Ű� �ߵ� �ȴ�.
	MonIdx_t					m_nBossMonster;
	INT							m_nWinInfluenceWP;
	INT							m_nLossInfluenceWP;
	MapIndex_t					m_nNextOutPostMap;				// ���� �������� ��
	INT							m_nDestroyCrystalcount;			// �ı��ؾ� �ϴ� ũ����Ż�� ��

	INT							m_nFixedBuffItem;				// �׽�Ʈ�� Ư�� ������ �������� ����

	mtvectTriggerAttackUserDamage				m_mtvectTriggerAttackUserDamage;		// ���� ���Ϳ��� ���� ������ ����
	vector<CTriggerNGCInflWarMonsterGroup *>	m_vectorTriggerNGCOutPostSummonMonsterGroup;		// �ʱ� ���ý� �����ǰ� ������� �ʴ´�. ����ȭ �ʿ� ����
	vectorTriggerOutPostBossKillInfo			m_vectTriggerOutPostBossKillInfo;		// ���� ���͸� ��� ���� ���� ��ƾ� �ϴ� ���� ���� ������ ���� ����
	mtvectorTriggerOutPostMonsterKillInfo		m_mtvectTriggerOutPostMonsterKillInfo;	// ���� �������� ���������� ���� �ܰ踦 ������ ����
	vectorTriggerCrystalBuff					m_vectCrystalBuff;						// ũ����Ż ���� ����Ʈ
};

#endif // !defined(AFX_TRIGGERFUNCTIONNGOUTPOST_H__02F8538D_6693_439E_A335_4C55F13974B6__INCLUDED_)

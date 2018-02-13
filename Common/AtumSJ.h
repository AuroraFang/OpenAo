#pragma once

#include "AtumParam.h"
#include "AtumTime.h"

constexpr auto STORE_KEEPING_COST = 30;
constexpr auto DAMAGE_KIND_ANGLE_DISTANCE = 100.0f;	// Target five variables for each comparison, @CalcDamageKind ()

struct LEVEL_EXPERIENCE
{
	INT				Level;
	Experience_t	ExperienceIncreament;		// (Up to that level of experience) can not be used because (best experience at that level -1), inaccurate!
	Experience_t	Experience;					// Initial experience of (its level -1)
	Experience_t	ExperienceLossOnDeath;
	INT				PenaltySPIOnDead;
};

// start 2011-11-15 by hskim, EP4 [Ʈ���� �ý���] - ���׸� ����ġ �߰�
struct BURNING_MAP_LEVEL_ADD_EXPERIENCE
{
	INT				Level;						// ����
	FLOAT			IncrementPercent;			// �߰� ����ġ�� (�ۼ�Ʈ)
};
// end 2011-11-15 by hskim, EP4 [Ʈ���� �ý���] - ���׸� ����ġ �߰�

struct GEAR_STAT_CHANGE_VALUE
{
	USHORT		UnitKind;
	GEAR_STAT	GearStat1;
};
struct GEAR_AUTO_STAT_VALUE_PER_LEVEL
{
	INT			AutoStatType1;
	INT			Level;
	GEAR_STAT	GearStat1;
};

// Level Ȥ�� Stat�� ���� ���� ��
struct LEVEL_STAT_VALUE
{
	INT		LevelOrStat;
	double	AttackPart_AttackIncreaseRate;			// ���� ���ȿ� ���� attack ������, @GetAttackC()
	double	AttackPart_AttackIncreaseProbability;	// ���߷� - ���ݽ��ȿ� ���� ����Ȯ���� ������ ����� ������ �߰� Ȯ��
	INT		AttackPart_PierceAttackProbability;		// �Ǿ��(256Ȯ��) - ���� ��������� �� ���� �����Ѱ��� ���� ���� ������� �ȴ�.
	double	DefensePart_DefenseDecreaseRate;		// ��� ���ȿ� ���� defense ���Һ�, @GetDefenseC()
	double	ShieldPart_DP;							// ShieldPart�� ���� Shield(DP)��
	double	DodgePart_DefenseProbabilityRate;		// ȸ�� ���ȿ� ���� ���Ȯ���� ����, @GetDefenseProbabilityC()
	INT		HPOfLevel;								// Level�� ���� HP
	INT		Level_MonsertPierceAttackProbability;	// MonsterLevel�� ���� ���� PierceAttackȮ��
};

struct STATUS_NAME
{
	BYTE		Status;				// STATUS_XXX
	const char	*StatusNameEN;
	const char	*StatusNameKO;
};

struct ITEMKIND_STRING				// 2006-07-28 by cmkwon
{
	BYTE		byItemKind0;	
	char		*szItemKindString;		// 2006-07-28 by cmkwon, ITEMKIND_XXX --> "ITEMKIND_XXX"�� ��ȯ
	char		*szItemKindName;		// 2006-07-28 by cmkwon, �������� �����ִ� ItemKindString
	BOOL		bIsBazaarItemKind;		// 2006-07-28 by cmkwon, ���� �������� �ŷ� ���� ITEMKIND_XXX
};
extern const ITEMKIND_STRING g_arrItemKindString[];

extern setString				g_setAllLetterList;		// 2007-05-04 by cmkwon

struct CParamFactor;
class CODBCStatement;
struct MEX_TARGET_INFO;

///////////////////////////////////////////////////////////////////////////////
/// \class		CAtumSJ
///
/// \brief		���� ���� ���ϴ� Class - ��ȹ ���� ����
/// \author		kelovon
/// \version
/// \date		2004-04-10 ~ 2004-04-10
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CLocalization;

namespace AtumSJ
{
#ifdef _ATUM_SERVER
	// ���� ��ġ ������ �ε�
	BOOL LoadData(CODBCStatement *i_pOdbcStmt);
	vectorMAP_INFO *GetVectPtrALL_MAP_INFO();
#endif // _ATUM_SERVER

	// �� ����
	const MAP_INFO* GetMapInfo(MapIndex_t i_nMapIndex);
	const char* GetMapName(MapIndex_t i_nMapIndex);

	// ���� & ����ġ ����
	const LEVEL_EXPERIENCE* GetLevelExperience(int level);
	float GetBurningMapAddExperience(int level);		// start 2011-11-15 by hskim, EP4 [Ʈ���� �ý���] - ���׸� ����ġ �߰�
	Experience_t GetInitialExperienceOfLevel(int level);
	float GetPercentageOfExperience(Experience_t exp, int level);
	Experience_t GetExperienceFromPercentage(float i_fPercentage, int level);

	// ĳ���� �Ӽ� ����
	USHORT GetTransport(CHARACTER *pCharacter);				// ��ݷ�
	float GetSpeed(CHARACTER *pCharacter, ITEM *pItem);		// �̵� �ӵ�

	// 2007-11-02 by cmkwon, �߷� ���ϴ� �ý��� ���� - ���ϰ� �ڷ��� ����(<--int)
	float GetItemWeight(ITEM *i_pItem, int i_nCountableItemCount, float i_fDesWeight = 0.0f);	// �������� ���Ը� ��ȯ, countable item�� ��츸 i_nCountableItemCount�� �ǹ̸� ����
	BYTE GetStatusByPropensity(INT i_nPropensity);
	const char*  GetStatusByLevel(BYTE i_byLevel);				// 2008-06-20 by dhjin, EP3 ���������ɼ� -
	const char* GetStatusNameKO(BYTE i_nStatus);
	const char* GetStatusNameEN(BYTE i_nStatus);

	// ���� ����
	BOOL CheckTargetErrorAngle(CHARACTER* i_pAttackCharacter, D3DXVECTOR3& i_ServerAttackPosition, D3DXVECTOR3& i_ServerTargetPosition, D3DXVECTOR3& i_ClientTargetPosition, float& o_fServerDistance);
	float GetAttackProbabilityC(ITEM *pItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, Stat_t i_nAttackPart, BYTE i_byLevel, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, ������ ���� ������ - i_PvPBuffPercent �߰�	// 2013-05-09 by hskim, ���� ����Ʈ ���� - // 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)// ĳ������ ���� Ȯ��
	float GetDefenseProbabilityC(CHARACTER *pCharacter, ITEM *pDefenseItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, ������ ���� ������ - i_PvPBuffPercent �߰�	// 2013-05-09 by hskim, ���� ����Ʈ ���� - // ĳ������ ��� Ȯ��
	float GetAttackProbabilityM(const MONSTER_INFO *a_pMonsterInfo, const ITEM *a_pItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);	// ������ ���� Ȯ��	// 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (���� ���ݷ� �߰� ����, PierceȮ���� ���߷� ����. )
	float GetDefenseProbabilityM(const MONSTER_INFO *a_pMonster, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);	// ������ ��� Ȯ�� // 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� Ȯ�� ���� ���� �߰�.

	float GetAttackC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, ������ ���� ������ - i_PvPBuffPercent �߰�		// 2013-05-09 by hskim, ���� ����Ʈ ���� - // ĳ������ ���ݷ�
	float GetMinAttackC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, ������ ���� ������ - i_PvPBuffPercent �߰�	// 2013-05-09 by hskim, ���� ����Ʈ ���� - // ĳ������ �ּҰ��ݷ�
	float GetMaxAttackC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, ������ ���� ������ - i_PvPBuffPercent �߰�	// 2013-05-09 by hskim, ���� ����Ʈ ���� - // ĳ������ �ִ���ݷ�
	
	// returns defense percentage in [0, 100]
	float GetDefenseC(CHARACTER *pCharacter, const CParamFactor *pParamFactor,
		bool bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);		
	
	float GetAttackM(const ITEM *a_pItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);							// ������ ���ݷ� // 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� Ȯ�� ���� ���� �߰�.
	float GetMinAttackM(const ITEM *a_pMonsterAttackItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);			// ������ �ּ� ���ݷ� // 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� Ȯ�� ���� ���� �߰�.
	float GetMaxAttackM(const ITEM *a_pMonsterAttackItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);			// ������ �ִ� ���ݷ� // 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� Ȯ�� ���� ���� �߰�.
	float GetDefenseM(const MONSTER_INFO *a_pMonster, BOOL i_bIsArenaServer = FALSE, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);		// ������ ���� // 2009-12-24 by cmkwon, ���� Defense�� ���Ǵ�Ƽ����(�Ʒ�������)���� ���������� ó�� - // 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) ) - ���� Ȯ������ ���� �߰�.

	// 2009-12-17 by cmkwon, ������ ���� ������ �ʵ�(����,ȸ��,�Ǿ,Ȯ��)�� 255�̻� ���� ���� �����ϰ� ���� - ���� ���� �߰�
	float GetPierceAttackProbabilityM(const int i_MonsterLevel, const ITEM *i_pAttItem, const MONSTER_INFO_EXTEND *a_pMonInfoExtend = NULL);	// ������ PierceAttackȮ�� // 2010. 06. 14 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (���� ���ݷ� �߰� ����, PierceȮ���� ���߷� ����. )

	float GetMinAttackPerSecondC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, ������ ���� ������ - i_PvPBuffPercent �߰�	// 2013-05-09 by hskim, ���� ����Ʈ ���� - // ĳ������ �ʴ� �ּ� ���ݷ�
	float GetMaxAttackPerSecondC(CHARACTER *pCharacter, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, ������ ���� ������ - i_PvPBuffPercent �߰�	// 2013-05-09 by hskim, ���� ����Ʈ ���� - // ĳ������ �ʴ� �ִ� ���ݷ�

	// ������ �Ӽ� ����
	int GetTotalPrimaryShotCountPerReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);	// ���� Reattack Time ���� �� �� �ִ� 1�� ������ �ִ� �Ѿ� ��
	int GetTotalSecondaryShotCountPerReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);	// ���� Reattack Time ���� �� �� �ִ� 2�� ������ �ִ� �Ѿ� ��
	int GetTotalPrimaryShotNumPerReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);		// ���� Reattack Time ���� �� �� �ִ� 1�� ������ �ִ� �߻� Ƚ��
	int GetTotalSecondaryShotNumPerReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);	// ���� Reattack Time ���� �� �� �ִ� 2�� ������ �ִ� �߻� Ƚ��
	int GetSecondaryMultiNumCount(ITEM *pItem, const CParamFactor *pParamFactor);		// �� ���� ������ �Ѿ� ��
	DWORD GetReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);				// ����� �ð�
	float GetAttackRange(ITEM *pItem, const CParamFactor *pParamFactor);				// ���� ����
	float GetOverheatTime(ITEM *pItem, const CParamFactor *pParamFactor);				// ���� �ð�
	float GetShotCountReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);		// shotnum �� ���� �߻�� ���� �ð�
	float GetShotNumReattackTime(ITEM *pItem, const CParamFactor *pParamFactor);		// ReattackTime ���� ShotNum ��ŭ �߻� �ɶ� ���� ���� �ð�
	float GetPrepareTime(ITEM *pItem, const CParamFactor *pParamFactor);				// �ߵ� �ð�
	int GetMultiTargetNum(ITEM *pItem, const CParamFactor *pParamFactor);				// Multi Target ��
	float GetAttackRangeAngle(ITEM *pItem, const CParamFactor *pParamFactor);			// ȭ�� ����
	UINT GetItemSellingPriceAtShop(ITEM* pItem);									// ������ ������ �ȶ� ����
	float GetItemSellingPriceRate();												// ������ ������ �ȶ� ����
	float GetExplosionRange(ITEM *i_pItem, const CParamFactor *i_pParamFactor);		// ���� �ݰ�
	int GetCountMineAtATime(ITEM *i_pItem, const CParamFactor *i_pParamFactor);		// �� ĳ���ʹ� �ѷ��� �� �ִ� �ִ� ������ ����
	USHORT GetReactionRange(ITEM *i_pItem, const CParamFactor *i_pParamFactor);		// ���� �ݰ�
	float GetMinAttackPerSecond(ITEM *i_pRealItem);								// �������� �ʴ� �ּ� ���ݷ�
	float GetMaxAttackPerSecond(ITEM *i_pRealItem);								// �������� �ʴ� �ִ� ���ݷ�
	float GetPrimaryRadarRange(ITEM *i_pItem, const CParamFactor *i_pParamFactor);		// 1�� ���̴� ���� �Ÿ�
	float GetSecondaryRadarRange(ITEM *i_pItem, const CParamFactor *i_pParamFactor);	// 2�� ���̴� ���� �Ÿ�
	float GetCircularAngle(ITEM *i_pItem, const CParamFactor *i_pParamFactor);				// ������ ����(�����ӵ�), 2005-12-02 by cmkwon
	float GetEngineRangeAngle(ITEM *i_pItem, const CParamFactor *i_pParamFactor);			// ������ ����(��ȸ��), 2005-12-02 by cmkwon
	float GetEngineBoosterRangeAngle(ITEM *i_pItem, const CParamFactor *i_pParamFactor);	// ������ ����(�ν��� ��ȸ��), 2005-12-02 by cmkwon
	float GetWarHeadSpeed(ITEM *i_pItem, const CParamFactor *i_pParamFactor);				// 176, 2007-06-11 by cmkwon, ź���� �ӵ�


	// ����ġ ����
	float GetMonsterDeadExpWeight(int i_nCharacterLevel, int i_nMonsterLevel);	// ���Ͱ� �׾��� �� �й��ϴ� ����ġ�� ���� weight ����
	float GetPartyBonusExpWeight(int nPartyMemberCnts);							// 

	// ���� ��� ������ ���� �� ���� ���� ���� ���� ����
	BOOL CanPickUpDropMoney(int i_nCharacterLevel, MONSTER_INFO *i_pMonInfo);
	BOOL CanPickUpDropItem(int i_nCharacterLevel, MONSTER_INFO *i_pMonInfo);
	BOOL CanPickUpDropRare(int i_nCharacterLevel, MONSTER_INFO *i_pMonInfo);

	// ���ʽ� ���� ���� ��ġ ����
	const GEAR_STAT_CHANGE_VALUE* GetGearStatChangeValueStruct(USHORT i_UnitKind);
	Stat_t GetGearStatChangeValue(USHORT i_UnitKind, BYTE i_nStatKind);
	BOOL GetGearStatBYAutoStatTypeAndLevel(GEAR_AUTO_STAT_VALUE_PER_LEVEL *o_pAutoStatValue, INT i_autoStatType, INT i_level);
	char * GetAutoStatTypeString(INT i_autoStatType);

	// ��Ʈ�� ��ȯ
// 2006-07-28 by cmkwon, �ʿ� ���� �Լ�
//	const char *GetItemKindString(BYTE i_nKind);
	const char *GetItemKindName(BYTE i_nKind);
	BYTE GetItemKindByItemKindName(char *i_szItemKindString);
	const char *GetRaceString(USHORT i_nRace);
	const char *GetUnitKindString(USHORT i_nUnitKind);
	const char *GetStatString(BYTE i_nStatKind);
	const char *GetTimerTypeString(TimerEventType i_nTimerEventType);
	const char *GetInfluenceTypeString(BYTE i_byInfluenceTy);
	const char *GetPositionString(BYTE i_byPosition);
	const char *GetReqUnitKindString(USHORT i_usUnitKind);
	const char *GetGuildRankString(BYTE i_byGuildRank);

	// ���� ����
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT GetCharacterHP(BYTE i_nLevel);
	INT GetCharacterDP(Stat_t i_nShieldPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)// DP(����)��
#else
	SHORT GetCharacterHP(BYTE i_nLevel);
	SHORT GetCharacterDP(Stat_t i_nShieldPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)// DP(����)��
#endif	
	SHORT GetCharacterSP(Stat_t i_nSoulPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)
	SHORT GetCharacterEP(Stat_t i_nFuelPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)
	float GetAttackC_IncreaseRate(Stat_t i_nAttackPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)		// AttackC ������
	float GetAttackC_IncreaseProbability(Stat_t i_nAttackPart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)// ���߷� - ���ݽ��ȿ� ���� ����Ȯ���� ������ ����� ������ �߰� Ȯ��
	float GetAttackC_PierceAttackProbability(Stat_t i_nAttackPart, ITEM *pAttackItem, const CParamFactor *pParamFactor, BOOL bIsPrimaryAttack, BYTE i_byLevel, enumAttackToTarget eAttackToTarget = NULL2NULL, float i_PvPBuffPercent = 0.0f);	// 2013-08-01 by jhseol, ������ ���� ������ - i_PvPBuffPercent �߰�		// 2013-05-09 by hskim, ���� ����Ʈ ���� - // 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)// 2008-09-22 by dhjin, �ű� ��þƮ, // PierceAttack�� - ���ݽ��ȿ� ���� ������� ��Ⱑ �Ǵ� ���� ��� �� �ϳ�
	double GetDefenseC_DecreaseRate(Stat_t i_nDefensePart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)// DefenseC�� ���� Damage ���� ����
	double GetDefenseProbabilityC_Rate(Stat_t i_nDodgePart, BYTE i_byLevel);	// 2010-02-19 by cmkwon, 100���� ���� �ִ뽺�� ��ġ ���� ���� - �����߰�(, BYTE i_byLevel)// ��� Ȯ���� ����
	const LEVEL_STAT_VALUE* GetLevelStatValue(int i_nLevelOrStat);

	BOOL GetRepairCost(int *o_pnCost, int *o_pnRealAmountToRepair, int i_nDesParam, int i_nRequestAmountToRepair, ITEM *i_pBulletItem = NULL);	// ���� ��� ���(����:spi)

	enumAttackToTarget GetAttackToTarget(ClientIndex_t i_AttackIndex, MEX_TARGET_INFO &i_MexTargetInfo);
	enumAttackToTarget GetAttackToTarget(ClientIndex_t i_AttackIndex, ClientIndex_t i_TargetIndex, UINT i_TargetItemFieldIndex);
	BOOL IsCharacterAttacker(enumAttackToTarget i_nAttackToTarget);
	BOOL IsMonsterAttacker(enumAttackToTarget i_nAttackToTarget);
	BOOL IsCharacterTarget(enumAttackToTarget i_nAttackToTarget);
	BOOL IsMonsterTarget(enumAttackToTarget i_nAttackToTarget);
	BOOL IsItemTarget(enumAttackToTarget i_nAttackToTarget);

	BOOL CheckReqItemKind(BYTE i_nReqItemKind, BYTE i_nTargetItemKind);

	void GetAtumTime(AtumTime *o_pAtumTime);

	INT64 GetCityWarTex(INT64 i_n64ItemPrice, float i_fTexPercent);
	INT64 GetItemTexSPI(INT64 i_n64ItemPrice, float i_fTexPercent);

	float GetPenaltyCollisionBYSpeed(INT i_nCurSpeed);

	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - �����߰�(MapIndex_t i_nStartCityMapIdx)
	MapIndex_t GetCityMapIndexByInfluence(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx = VCN_CITY_MAP_INDEX);

	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - CAtumSJ::IsOtherInfluenceMap#, �߰�
	MapIndex_t GetGargenMapIndexByInfluence(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx = VCN_CITY_MAP_INDEX, BOOL i_bMotherShipWar = FALSE);		// 2012-07-02 by hskim, ���� ���� - ���� ����� ������ ���� ���� �Ұ�	
	BOOL IsOtherInfluenceMap(BYTE i_byInfluence, MapIndex_t i_nStartCityMapIdx, int i_nMapInflTy);
	BOOL IsSameCharacterInfluence8MapInfluence(BYTE i_byInflTy, int i_nMapInflTy);

	D3DXVECTOR3 GetRandomXZVec3(D3DXVECTOR3 *i_pVec3Center, int i_nRandomRadiusDistance);

	BOOL IsAlphaNum(char i_cParam);
	BOOL IsValidCharacterName(char *i_szCharName);
	BOOL IsValidGuildName(char *i_szGuildName);
	BOOL LoadAllLetterList(const char *i_szFilePath);
	BOOL IsValidOneByteCharacter(const char *i_szSource);	// 2007-12-05 by cmkwon, ĳ���͸� üũ�� Ư������ üũ ��ƾ ���� - CAtumSJ::IsValidOneByteCharacter() �߰�

	// 2006-09-05 by cmkwon
	INT GetSPIPenaltyOnDead(int i_nLevel);

	// 2006-09-14 by dhjin, �ɹ��� ���񽺽� ����� ���� ���� �� ����Ǵ� �κ��丮, â��, ���ܿ� �ƽ��� ������
// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - ���� ����
//	INT GetMaxInventorySize(BOOL i_IsMembership);
//	INT GetMaxStoreSize(BOOL i_IsMembership);
	INT GetMaxInventorySize(BOOL i_IsMembership, int i_nAddedCnt);		// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - ���� ����
	INT GetMaxStoreSize(BOOL i_IsMembership, int i_nAddedCnt);			// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - ���� ����
	INT GetMaxGuildSize(INT i_nGuildMemberCapacity, BOOL i_IsMembership);	// 2008-05-27 by dhjin, EP3 ���� ���� ���� - ���ܿ� ���� ĳ�� ������

	// 2007-04-10 by cmkwon
	char * GetJamboreePreAddCharacterString(int i_nNumber);

	// 2007-11-21 by cmkwon, PilotFace ���� ī�� ���� - 
	BOOL IsValidPiotFace(BYTE i_byPilotFace);

	// 2008-11-26 by cmkwon, ��� �� �ð�(����ð�) ���� ������ ���� - 
	// 2009-10-22 by cmkwon, �ð� �������� ������ �ý��� ���� - CAtumSJ::GetTimeSecondByItemKind8ItemAttribute#, ����
	INT GetTimeSecondByItemKind8ItemAttribute(ITEM *i_pItemInfo, TIME_TERM_t i_enTimeTermTy = TIME_TERM_USING_ITEM);
	INT GetTimeMinuteByItemKind8ItemAttribute(ITEM *i_pItemInfo, TIME_TERM_t i_enTimeTermTy = TIME_TERM_USING_ITEM);

	BOOL IsAlphaNum(char *i_szString);				// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	BOOL IsValidNickName(char *i_szNickName);		// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 

	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - 
	INT FindPartyFormationSkillNum(BYTE i_byFormationTy);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-14 by cmkwon, ���巩ŷ�ý��� ���� ����(���񽺻���,�����׷����) - 
	BOOL IsValidWRankingServiceName(char *i_szServiceName);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - 
	int GetLuckyMachineSlotCount(int i_machineKind);
	int GetRareItemInfoList(vectRARE_ITEM_INFOPtrList *o_pValidRareList, BOOL i_bIsPrefix, mapRARE_ITEM_INFOPtrList *i_pRareItemInfoList, ITEM *i_pTargetItemInfo, ITEM *i_pRareItemInfo);
	// 2010-06-01 by shcho, GLogDB ���� -
	const char* GetInfluenceTypeGLogString(BYTE i_byInfluenceTy);
	const char* GetGLOGUnitKindString(USHORT i_nUnitKind);

	void GetDHMSFromTimeInS(const LONGLONG timeInSecs, UINT* pastDays, BYTE* pastHours, BYTE* pastMinutes, BYTE* pastSeconds);

	BYTE GetOppositeNation(BYTE byInflType);	// 2015-12-17 Future - retrival of the opposite Nation
}

namespace CAtumSJ = AtumSJ;

///////////////////////////////////////////////////////////////////////////////
// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - ���� BOOL ������ ��Ʈ����ü�� ����, 
struct BitS_CParamFactor
{
	UINT	pfb_SKILL_ROLLING_TIME			:1;		// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - ���� BOOL ������ ��Ʈ����ü�� ����, 
	UINT	pfb_SKILL_Barrier				:1;		// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - ���� BOOL ������ ��Ʈ����ü�� ����, 
	UINT	pfb_SKILL_SHIELD_PARALYZE		:1;		// 2009-09-09 ~ 2010-02-18 by dhjin, ���Ǵ�Ƽ - Debuff üũ�� �߰� // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Debuff üũ�� ����	// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - ���� BOOL ������ ��Ʈ����ü�� ����, 
	UINT	pfb_SKILL_Invincible			:1;		// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - ���� BOOL ������ ��Ʈ����ü�� ����, 
	UINT	pfb_SKILL_DamageDistribution	:1;		// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - ���� BOOL ������ ��Ʈ����ü�� ����, �߰���
	UINT	pfb_ITEM_IgnoreDefence			:1;		// 203, // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ������ ��� üũ
	UINT	pfb_ITEM_IgnoreAvoid			:1;		// 204, // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ȸ�Ƿ� ���� ������ ��� üũ
};


///////////////////////////////////////////////////////////////////////////////
/// \class		CParamFactor
///
/// \brief		ĳ������ ���� �Ķ���Ϳ� ����� factors, DES_XXX�� ������
///				pfp:Plus ���ִ� Param, Param�� �����ش� ó��
///				pfp:Multiple ���ִ� Param, Param�� �����ִ� ó��
/// \author		kelovon
/// \version
/// \date		2004-04-10 ~ 2004-04-10
/// \warning	ĳ������ ParamFactor�� ITEM�� ����ÿ� ������ ���⿡�� �ش�ȴ�.
///////////////////////////////////////////////////////////////////////////////
struct CParamFactor
{
	// DevX removed this constructor to make it POD
	// CParamFactor() { Reset(); }

	void Reset() { util::zero(this, sizeof(CParamFactor)); }

	CParamFactor operator+(const CParamFactor &rhs) const;
	CParamFactor& operator+=(const CParamFactor &rhs);
	CParamFactor operator-(const CParamFactor &rhs) const;
	CParamFactor& operator-=(const CParamFactor &rhs);

	float		pfp_ATTACK_PART;			// 1, ���� ��Ʈ
	float		pfp_DEFENSE_PART;			// 2, ��� ��Ʈ
	float		pfp_FUEL_PART;				// 3, ���� ��Ʈ
	float		pfp_SOUL_PART;				// 4, ���� ��Ʈ
	float		pfp_SHIELD_PART;			// 5, ���� ��Ʈ
	float		pfp_DODGE_PART;				// 6, ȸ�� ��Ʈ
	float		pfp_BODYCONDITION;			// 7, ������
	float		pfp_ENDURANCE_01 ;			// 8, ������ 01
	float		pfp_ENDURANCE_02;			// 9, ������ 02
	float		pfp_CHARGING_01;			// 10, ��ź�� 01
	float		pfp_CHARGING_02;			// 11, ��ź�� 02
	INT			pfp_PROPENSITY;				// 12, ����
	float		pfp_HP;						// 13, ��Ʈ����Ʈ
	float		pfp_SP;						// 14, �ҿ�����Ʈ, ���� �ð� �Ŀ� SP�� ������Ű�⸸ �ϸ� ��
	float		pfp_DP;						// 89, ����(DEFENSE)����Ʈ
	float		pfp_EP;						// 15, ��������Ʈ
	float		pfp_SPRECOVERY;				// 16, �ҿ�����Ʈȸ����
	float		pfp_HPRECOVERY;				// 17, ����������Ʈȸ����
	float		pfm_MINATTACK_01;			// 18, (*) �ּ� ���ݷ� 01
	float		pfm_MAXATTACK_01;			// 71, (*) �ִ� ���ݷ� 02
	float		pfm_MINATTACK_02;			// 19, (*) �ּ� ���ݷ� 01
	float		pfm_MAXATTACK_02;			// 72, (*) �ִ� ���ݷ� 02
	float		pfp_ATTACKPROBABILITY_01;	// 20, // 2010-07-19 by dhjin, Ȯ�� ���� ���� ����Ȯ�� 01, 0 ~ 255	
	float		pfp_ATTACKPROBABILITY_02;	// 21, // 2010-07-19 by dhjin, Ȯ�� ���� �������Ȯ�� 02, 0 ~ 255
	float		pfp_DEFENSE_01;				// 22, // 2010-07-19 by dhjin, Ȯ�� ���� ������� 01, 0 ~ 255
	float		pfp_DEFENSE_02;				// 23, // 2010-07-19 by dhjin, Ȯ�� ���� ������� 02, 0 ~ 255
	float		pfp_DEFENSEPROBABILITY_01;	// 24, // 2010-07-19 by dhjin, Ȯ�� ���� ������Ȯ�� 01, 0 ~ 255
	float		pfp_DEFENSEPROBABILITY_02;	// 25, // 2010-07-19 by dhjin, Ȯ�� ���� ������Ȯ�� 02, 0 ~ 255
	Prob255_t	pfp_SKILLPROBABILITY;		// 26, ��ų����Ȯ��, 0 ~ 255
	Prob255_t	pfp_FACTIONRESISTANCE;		// 27, �Ӽ����׷�, 0 ~ 255
	float		pfm_SPEED;					// 28, (*) �̵��ӵ�
	float		pfp_TRANSPORT;				// 29, ��ݷ�
	float		pfp_MATERIAL;				// 30, ����
	float		pfm_REATTACKTIME_01;		// 31, (*) ������Ÿ�� 01
	float		pfm_REATTACKTIME_02;		// 32, (*) ������Ÿ�� 02
	Prob255_t	pfp_ABRASIONRATE_01;		// 33, ������ 01, 0 ~ 255
	Prob255_t	pfp_ABRASIONRATE_02;		// 34, ������ 02, 0 ~ 255
	float		pfm_RANGE_01;				// 35, (*) ��ȿ�Ÿ� 01
	float		pfm_RANGE_02;				// 36, (*) ��ȿ�Ÿ� 02
	float		pfp_RANGEANGLE_01;			// 37, ��ȿ���� 01, �������� ���ʿ�
	float		pfp_RANGEANGLE_02;			// 38, ��ȿ���� 02, �������� ���ʿ�
	int			pfp_MULTITAGET;				// 39, ��ƼŸ��, �������� ���ʿ�
	float		pfp_EXPLOSIONRANGE_01;		// 40, ���߹ݰ� 01
	float		pfp_EXPLOSIONRANGE_02;		// 67, ���߹ݰ� 02
	float		pfp_UNIT;					// 41, ������ ���� (28 ~ 29�� ���� ���� ���ָ����� ���������� ����)
	float		pfp_REVISION;				// 42, ������ ������ (28 ~ 29�� ���� ���� ���ָ����� ���������� ����)
	Prob255_t	pfp_FACTIONPROBABILITY;		// 43, �Ӽ��� ���� ���Ȯ��, 0 ~ 255
	int			pfp_SHOTNUM_01;				// 44, ������ �� �߻� �� 01
	int			pfp_SHOTNUM_02;				// 69, ������ �� �߻� �� 02
	int			pfp_MULTINUM_01;			// 45, ���� �߻� �� 01
	int			pfp_MULTINUM_02;			// 70, ���� �߻� �� 02
	float		pfp_ATTACKTIME_01;			// 46, ó�� ���� ���� Ÿ�� 01
	float		pfp_ATTACKTIME_02;			// 47, ó�� ���� ���� Ÿ�� 02
	float		pfm_TIME_01;				// 48, (*) ���� �ð� 01
	float		pfm_TIME_02;				// 49, (*) ���� �ð� 02
	float		pfm_WEIGHT_01;				// 75, (*) ���� 01
	float		pfm_WEIGHT_02;				// 76, (*) ���� 02
	USHORT		pfp_REACTION_RANGE;			// 91, ITEM�� ReactionRange ����
	int			pfp_REQ_MIN_LEVEL;			// 96, (�����̻��)���������� �䱸 MinLevel�� �����
	int			pfp_REQ_MAX_LEVEL;			// 97, (�����̻��)���������� �䱸 MaxLevel�� �����
	float		pfm_SKILL_REDUCE_SHIELD_DAMAGE;		// 121, 2005-11-21 by cmkwon - ���忡 ���� �������� �ٿ��ش�.(*)
	float		pfm_ATTACK_RANGE_01;				// 129, 2005-11-21 by cmkwon - ���̴��� �⺻ ���� ���� �Ÿ��� ������Ų��.(*)
	float		pfm_ATTACK_RANGE_02;				// 130, 2005-11-21 by cmkwon - ���̴��� ��� ���� ���� �Ÿ��� ������Ų��.(*)
// 2005-12-02 by cmkwon	float		pfm_SKILL_HYPERMOVING;				// 132, 2005-11-28 by cmkwon - ������ ��ü �̵��ӵ��� ��������, �ν��� ����� ���� �ʴ´�.(*)
	float		pfm_SKILL_COLLISIONDAMAGE_DOWN;		// 140, 2005-11-21 by cmkwon - ��� ������Ʈ,�ٴ� �浹�� �������� ���ҽ�Ų��.(*)
	float		pfm_SKILL_SMARTSP;					// 148, 2005-11-21 by cmkwon - �ڽ��� ����ϴ� ��� ��ų�� ��뷮�� �ٿ��ش�.(*)	
	float		pfm_SKILL_REACTIONSPEED;			// 154, 2005-12-02 by cmkwon - ������ ����(�����ӵ�)
	float		pfm_SKILL_ENGINEANGLE;				// 155, 2005-12-02 by cmkwon - ������ ����(��ȸ��)
	float		pfm_SKILL_ENGINEBOOSTERANGLE;		// 156, 2005-12-02 by cmkwon - ������ ����(�ν��� ��ȸ��)

	float		pfm_DROP_EXP;						// 157, 2006-03-30 by cmkwon - ����ġ, �����̾��� ��ø �Ұ�
	float		pfm_DROP_SPI;						// 158, 2006-03-30 by cmkwon - SPI, �����̾��� ��ø �Ұ�
	float		pfm_DROP_ITEM;						// 159, 2006-03-30 by cmkwon - ������ �����, �����̾��� ��ø �Ұ�
	float		pfm_DROP_RAREITEM;					// 238, 2010-11-30 by shcho, ��������� ��� Ȯ�� ���� ������ ����
	float		pfm_HP_REPAIR_RATE_FLIGHTING;		// 160, 2006-03-30 by cmkwon - ����� HP ȸ����
	float		pfm_DP_REPAIR_RATE;					// 161, 2006-03-30 by cmkwon - DP ȸ����
	float		pfm_SP_REPAIR_RATE;					// 162, 2006-03-30 by cmkwon - SP ȸ����
// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - ���� BOOL ������ ��Ʈ����ü�� ����, �Ʒ��� ���� ��Ʈ����ü�� ����
// 	BOOL		pfb_SKILL_ROLLING_TIME;				// 170, 2006-11-17 by dhjin - 30�ʰ� �Ѹ� ���� �ð��� �����ش� 
// 	BOOL		pfb_SKILL_Barrier;					// 173, 2006-11-24 by dhjin - A����� ��޹��⿡ ���� ������ų
// 	BOOL		pfb_SKILL_SHIELD_PARALYZE;			// 175,	2006-11-24 by dhjin - A����� ���帶�� ��ų, ������� ���� ȸ������ 0���� �����
// 	BOOL		pfb_SKILL_Invincible;				// 122, 2006-11-27 by dhjin - M����� ���� ��ų
	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - ���� BOOL ������ ��Ʈ����ü�� ����, 
	BitS_CParamFactor	pfb_BitFlag;				

	float		pf_SKILL_Big_Boom_damage;			// 174, 2006-11-29 by dhjin - A����� �⺻���� ��¡��, ���÷��� �������� Range

	float		pfm_WARHEAD_SPEED;					// 176, 2007-06-11 by cmkwon, ź���� �ӵ�

	// 2008-09-22 by dhjin, �ű� ��þƮ
	float		pfn_ENGINE_BOOSTER_TIME_UP;			// 178		// 2008-09-22 by dhjin, �ν��� �ð� ����
	float		pfn_ENGINE_MAX_SPEED_UP;			// 179,	// 2008-09-22 by dhjin, ���� �Ϲݼӵ�(�ִ�) ����
	float		pfn_ENGINE_MIN_SPEED_UP;			// 180,	// 2008-09-22 by dhjin, ���� �Ϲݼӵ�(�ּ�) ����
	float		pfn_ENGINE_BOOSTER_SPEED_UP;		// 181,	// 2008-09-22 by dhjin, ���� �ν��ͼӵ� ����
	float		pfn_ENGINE_GROUND_SPEED_UP;			// 182,	// 2008-09-22 by dhjin, ���� ����ӵ� ����
	int			pfn_RADAR_OBJECT_DETECT_RANGE;		// 183, // 2008-09-22 by dhjin, ���̴� ��ü ���� �ݰ�
	float		pfm_PIERCE_UP_01;					// 184, // 2008-09-22 by dhjin, �⺻���� �Ǿ�� ���� ī��
	float		pfm_PIERCE_UP_02;					// 185, // 2008-09-22 by dhjin, ��޹��� �Ǿ�� ���� ī��
	float		pfm_ENGINE_ANGLE_UP;				// 186,	// 2008-09-30 by dhjin, ���� ȸ���� ���� ī��
	float		pfm_ENGINE_BOOSTERANGLE_UP;			// 187,	// 2008-09-30 by dhjin, ���� �ν��� ȸ���� ���� ī��	
	ParamValue_t	pf_ITEM_Resistance;				// 201, // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������ ��� üũ 
	ParamValue_t	pf_ITEM_AddAttack;				// 202, // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���밪 Ÿ��ġ ������ ��� üũ 
	ParamValue_t	pf_ITEM_ReduceDamage;			// 205, // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ������ ���밪 ���� ������ ��� üũ
	ParamValue_t	pf_ITEM_AddAttack_Sec;			// 206, // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���밪 Ÿ��ġ ������ ��޹����(�⹫�� �з�)
	ParamValue_t	pf_ITEM_OnceResistance;			// 207, // 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - �� ���� �����ϰ� �������� ���� ������ �߰�
	float			pfm_PLUS_WARPOINT_RATE;			// 234, // 2010-05-18 by cmkwon, WarPoint ���� ������ ����(�Ϻ���û) - 
	ParamValue_t	pf_ITEMS_MOVE_SPEED_RATE;		// 239, // 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����

	ParamValue_t	pfp_SKILL_BUFF_MON_ATTACK_POWER;			// 300, // 2013-05-09 by hskim, ���� ����Ʈ ���� - // ���� ���ݽ� - ���ݷ� ���� : Value ���� %
	ParamValue_t	pfp_SKILL_BUFF_MON_ATTACK_PROBABILITY;		// 301, // 2013-05-09 by hskim, ���� ����Ʈ ���� - // ���� ���ݽ� - ���ݷ� Ȯ�� : Value ���� %
	ParamValue_t	pfp_SKILL_BUFF_MON_ATTACK_PIERCE;			// 302, // 2013-05-09 by hskim, ���� ����Ʈ ���� - // ���� ���ݽ� - �Ǿ ���� : Value ���� %
	ParamValue_t	pfp_SKILL_BUFF_MON_DEFENCE;					// 303, // 2013-05-09 by hskim, ���� ����Ʈ ���� - // ���� ���� - ���� ���� : Value ���� %
	ParamValue_t	pfp_SKILL_BUFF_MON_DEFENCE_AVOID;			// 304, // 2013-05-09 by hskim, ���� ����Ʈ ���� - // ���� ���� - ȸ�Ƿ� ���� : Value ���� %
	ParamValue_t	pfp_SKILL_BUFF_PVP_ATTACK;					// 305, // 2013-05-09 by hskim, ���� ����Ʈ ���� - // PVP - ���ݷ� ���� : Value ���� %
	ParamValue_t	pfp_SKILL_BUFF_PVP_ATTACK_PROBABILITY;		// 306,	// PVP - ���߷� ���� : Value ���� %		// 2013-08-01 by jhseol, ������ ���� ������
	ParamValue_t	pfp_SKILL_BUFF_PVP_ATTACK_PIERCE;			// 307,	// PVP - �Ǿ ���� : Value ���� %		// 2013-08-01 by jhseol, ������ ���� ������
	ParamValue_t	pfp_SKILL_BUFF_PVP_DEFENCE;					// 308,	// PVP - ���� ���� : Value ���� %		// 2013-08-01 by jhseol, ������ ���� ������
	ParamValue_t	pfp_SKILL_BUFF_PVP_DEFENCE_PROBABILITY;		// 309,	// PVP - ȸ�Ƿ� ���� : Value ���� %		// 2013-08-01 by jhseol, ������ ���� ������
};
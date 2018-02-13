#pragma once

#include <VMemPool.h>
#include "MapBlock.h"

#define	MON2MON_IMPACT_DISTANCE_GAP							20.0f		// ���Ͱ� �浹 ó�� ����
#define MONSTER_MIN_ATTACK_DISTANCE							100.0f		// ���Ͱ� ���ݽ� �ּ� ���� �Ÿ�
#define MONSTER_TARGET_DISTANCE								200.0f		// ���� ������ ���Ͱ� �������� Target�� ��� �Ÿ�
#define MONSTER_BASIC_ARROUND_MOVABLE_RANGE					500.0f		// ���Ͱ� ������������ ���� �̵� ������ ���� �Ÿ�
#define MONSTER_ANGLE_PI_5									0.0872f		// 5���� PI�� ��ȯ�Ѱ�
#define MONSTER_MAX_QUICK_TURN_ANGLE						150			// 150��
#define MONSTER_ATTACKOBJ_SAMERACE_MAX_DISTANCE				300			// Attack Object�� ������
#define MONSTER_MAX_PARTY_DISTANCE							100.0f		// ��Ƽ�� �����ϱ� ���� �ִ�Ÿ�, ��Ƽ�� �Ҹ�Ǵ� �Ÿ��� + 50 = 200.0f
#define MONSTER_MAX_PARTY_MEMBER_COUNTS						10			// ���� ��Ƽ �ִ� ī��Ʈ
#define MONSTER_NOT_MOVE_AFTER_CREATED_TERM_TICK			2000		// ���Ͱ� �����ǰ� �������� �ʴ� �ð�
#define MONSTER_EVENT_MON_NOT_MOVE_AFTER_CREATED_TERM_TICK			10000	// �̺�Ʈ ���Ͱ� �����ǰ� �������� �ʴ� �ð�
#define MONSTER_DELETE_AFTER_CREATED_TERM_TICK				10000		// ���Ͱ� �����Ǿ����� FieldServer�� ���� ������ ���޾������ �����ϴ� �ð�, 2006-07-25 by cmkwon 5000-->10000
#define MONSTER_MIN_ATTACK_TERM_TICK						100			// ���Ͱ� Attack�� ���� �ּ��� �����̻��� ������ ���� ������ ������(�ּ� ShotNum Term)
#define MONSTER_ATTACKED_INFO_DELETE_TERM_TICK				10000		// ���Ͱ� Attackedlist���� Attack�� ������ ����� Tick
#define MONSTER_UPDATE_MOVE_TERM_TICK						200			// ������ �̵��� ���� ��
#define MONSTER_MOVPAT_GROUNDTELEPORT_ANIMATION_COUNT		2000/MONSTER_UPDATE_MOVE_TERM_TICK	// 2�� ���� �̵� ���Ͱ� ������ ���� �����µ� ī��Ʈ
#define MONSTER_MAP_IMPACK_HEIGHT_GAP						40.0f		// ���Ͱ� �̵��� �ʰ� �浹�� �������� �̺��� ũ�� MSS_MAP_IMPACT���°� �ȴ�.
#define MONSTER_COPTER_MAXTARGET_PINPOINT					PI/6		// 35��
#define MONSTER_COPTER_MAXMOVE_PINPOINT						PI/9		// 20��



#define MON_MOVE_RANDOMCOUNT_0						2
#define MON_MOVE_RANDOMCOUNT_1						10
#define MON_MOVE_RANDOMCOUNT_2						20

#define MON_MOVE_LRCOUNT_0							30			// FLYING_RIGHT, LeftRight Counts
#define MON_MOVE_LRCOUNT_1							40			// FLYING_RIGHT, LeftRight Counts
#define MON_MOVE_LRCOUNT_2							50			// FLYING_RIGHT, LeftRight Counts

#define MON_MOVE_FBCOUNT_0							15			// FLYING_RIGHT, FrontBack Counts
#define MON_MOVE_FBCOUNT_1							20			// FLYING_RIGHT, FrontBack Counts
#define MON_MOVE_FBCOUNT_2							25			// FLYING_RIGHT, FrontBack Counts

#define MON_MOVE_UDCOUNT_0							30			// FLYING_RIGHT, UpDown Counts
#define MON_MOVE_UDCOUNT_1							40			// FLYING_RIGHT, UpDown Counts
#define MON_MOVE_UDCOUNT_2							50			// FLYING_RIGHT, UpDown Counts

typedef enum
{
	MS_NULL				= 0,			// Monster�� ������ �ʴ� ����
	CS_NULL				= 0,			// Ŭ���Ʈ�� ������ �ʴ� ����
	MS_CREATED			= 1,			// Monster�� �����Ǿ� Field Server�� ���� ��Ŷ�� ��������
	MS_PLAYING			= 2,			// Monster�� �������� ����
	CS_GAMESTARTED		= 2,			// Ŭ���̾�Ʈ�� Field Server�� ������ ����
	MS_DEAD				= 3,			// Monster�� ������ ����
	CS_DEAD				= 3				// Ŭ���̾�Ʈ�� ������ ����
} UNIT_STATE;

typedef enum
{
	MSS_NORMAL					= 0,		// Monster ���������� �̵��ϴ� ����
	MSS_MAP_IMPACT				= 1,		// ��, Object�� �浹 ����
	MSS_RANGE_DISTANCE_IMPACT	= 2,		// ���Ͱ� �� �������϶� �̵������� �����
	MSS_OUT_OF_ATTACK_RANGE		= 3,		// ó�� ���ݽ� �Ÿ��� �־� ������ ������ ����
	MSS_QUICK_TURN_GENERAL		= 4,		// ���Ͱ� �����̵��� QuickTurn�� ������̴�,
	MSS_QUICK_TURN_SKILL		= 5			// ���Ͱ� ��ü ��ų�� QuickTurn�� ������̴�,
} Unit_Move_State;

typedef enum
{
	MSD_FRONT					=  1,
	MSD_BACK					= -1,
	MSD_LEFT					=  1,
	MSD_RIGHT					= -1,
	MSD_UP						=  1,
	MSD_DOWN					= -1,
	MSD_SPEED_UP				=  1,
	MSD_SPEED_DOWN				= -1,

	MSD_TARGET					=  0,					// Monster�� �̵� ������ Target Vector�� ����
	MSD_LEFT_1					=  1,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_1					= -1,
	MSD_LEFT_2					=  2,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_2					= -2,
	MSD_LEFT_3					=  3,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_3					= -3,
	MSD_LEFT_4					=  4,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_4					= -4,
	MSD_LEFT_5					=  5,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_5					= -5,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_10					=  10,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_10				= -10,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_15					=  15,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_15				= -15,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_20					=  20,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_20				= -20,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_25					=  25,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_25				= -25,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_30					=  30,					// Monster�� �̵� ������ Target Vector�� ���� 30�� ����
	MSD_RIGHT_30				= -30,					// Monster�� �̵� ������ Target Vector�� ���� 30�� ����
	MSD_LEFT_35					=  35,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_35				= -35,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_40					=  40,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_40				= -40,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_45					=  45,					// Monster�� �̵� ������ Target Vector�� ���� 45�� ����
	MSD_RIGHT_45				= -45,					// Monster�� �̵� ������ Target Vector�� ���� 45�� ����
	MSD_LEFT_50					=  50,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_50				= -50,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_55					=  55,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_55				= -55,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_60					=  60,					// Monster�� �̵� ������ Target Vector�� ���� 60�� ����
	MSD_RIGHT_60				= -60,					// Monster�� �̵� ������ Target Vector�� ���� 60�� ����
	MSD_LEFT_65					=  65,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_65				= -65,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_70					=  70,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_70				= -70,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_75					=  75,					// Monster�� �̵� ������ Target Vector�� ���� 75�� ����
	MSD_RIGHT_75				= -75,					// Monster�� �̵� ������ Target Vector�� ���� 75�� ����
	MSD_LEFT_80					=  80,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_80				= -80,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_85					=  85,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_RIGHT_85				= -85,					// Monster�� �̵� ������ Target Vector�� ���� 15�� ����
	MSD_LEFT_90					=  90,					// Monster�� �̵� ������ Target Vector�� ���� 90�� ����
	MSD_RIGHT_90				= -90,					// Monster�� �̵� ������ Target Vector�� ���� 90�� ����
	MSD_LEFT_100				=  100,
	MSD_RIGHT_100				= -100,
	MSD_LEFT_110				=  110,
	MSD_RIGHT_110				= -110,
	MSD_LEFT_120				=  120,
	MSD_RIGHT_120				= -120,
	MSD_TARGET_180				=  180,					// Monster�� �̵� ������ Target Vector�� �ݴ� ����

	MSD_UP_1					=  1,
	MSD_DOWN_1					= -1,
	MSD_UP_2					=  2,
	MSD_DOWN_2					= -2,
	MSD_UP_3					=  3,
	MSD_DOWN_3					= -3,
	MSD_UP_4					=  4,
	MSD_DOWN_4					= -4,
	MSD_UP_5					=  5,
	MSD_DOWN_5					= -5,
	MSD_UP_10					=  10,
	MSD_DOWN_10					= -10,
	MSD_UP_15					=  15,
	MSD_DOWN_15					= -15,
	MSD_UP_20					=  20,
	MSD_DOWN_20					= -20,
	MSD_UP_25					=  25,
	MSD_DOWN_25					= -25,
	MSD_UP_30					=  30,
	MSD_DOWN_30					= -30,
	MSD_UP_35					=  35,
	MSD_DOWN_35					= -35,
	MSD_UP_40					=  40,
	MSD_DOWN_40					= -40,
	MSD_UP_45					=  45,
	MSD_DOWN_45					= -45,
	MSD_UP_50					=  50,
	MSD_DOWN_50					= -50,
	MSD_UP_55					=  55,
	MSD_DOWN_55					= -55,
	MSD_UP_60					=  60,
	MSD_DOWN_60					= -60,
	MSD_UP_65					=  65,
	MSD_DOWN_65					= -65,
	MSD_UP_70					=  70,
	MSD_DOWN_70					= -70,
	MSD_UP_75					=  75,
	MSD_DOWN_75					= -75,
	MSD_UP_80					=  80,
	MSD_DOWN_80					= -80,
	MSD_UP_85					=  85,
	MSD_DOWN_85					= -85,
	MSD_UP_90					=  90,
	MSD_DOWN_90					= -90,
} Unit_Move_Direction;

struct ActionInfo
{
	ClientIndex_t	UnitIndex;
	DWORD			dwStartTick;
	DWORD			dwCurTick;
	float			fSumDamage;
	float			fSumAggro;		// 2006-07-21 by cmkwon

	ActionInfo()
		:UnitIndex(0), dwStartTick(0), dwCurTick(0), fSumDamage(0), fSumAggro(0)
	{};
	ActionInfo(ClientIndex_t i_cIdx, DWORD i_dwTick, float i_fDamage, float i_fAggro=0.0f)
		:UnitIndex(i_cIdx), dwStartTick(i_dwTick), dwCurTick(i_dwTick), fSumDamage(i_fDamage), fSumAggro(i_fAggro)
	{
	}

	void * operator new(size_t size)
	{
		return VMemPool::vmObjectNew(size);
	}
	void operator delete(void* p)
	{
		VMemPool::vmObjectDelete(p, sizeof(ActionInfo));
	}
};

typedef mt_vector<ActionInfo*>	mtvectorActionInfoPtr;
typedef mt_vector<ActionInfo>	mtvectActionInfo;
typedef vector<ActionInfo*>		vectorActionInfoPtr;
typedef vector<ActionInfo>		vectActionInfo;


struct ActionInfo_Sort_Object: binary_function<ActionInfo*, ActionInfo*, bool>
{
	bool operator()(ActionInfo *pAInfo1, ActionInfo *pAInfo2)
	{
        return pAInfo1->fSumDamage > pAInfo2->fSumDamage;	// ���� ���� ������ ���� �ݴ�� ��
    };
};

////////////////////////////////////////////////////////////////////////////////
//
// Ŭ���� �̸�  : listActionInfoPtr_UnitIndex_Find_Object
// Ŭ���� ����  : vectorActionInfoPtr���� UnitIndex�� ã�� Function Object�̴�
//					�������� ���ڴ� ã�����ϴ� UnitIndex�� �ִ´�.
struct listActionInfoPtr_UnitIndex_Find_Object
{
	listActionInfoPtr_UnitIndex_Find_Object(int index): m_nFindIndex(index){};
	bool operator()(const ActionInfo *pAInfo)
	{
		return pAInfo->UnitIndex == m_nFindIndex;
	}
	int m_nFindIndex;
};

class CMonster;
typedef mt_vector<CMonster*>			mtvectorMonsterPtr;

///////////////////////////////////////////////////////////////////////////////
/// \class		CMonster
///
/// \brief
/// \author		cmkwon
/// \version
/// \date		2004-03-26 ~ 2004-03-26
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CMonster : public MONSTER
{
public:
	CMonster();
	virtual ~CMonster();

	///////////////////////////////////////////////////////////////////////////
	// Property
	void SetMonsterIndex(int i_nMonsterIndex);

	//void SetMonsterInfoPtr(MONSTER_INFO * i_pMonsterInfo);
// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	void SetMonsterInfoPtr(MONSTER_INFO * i_pMonsterInfo , const bool a_bReset_ExtendInfo = true );
	void SetMonsterInfoExtend_AllData ( const MONSTER_BALANCE_DATA *a_pBalanceInfo );
	void SetMonsterInfoExtend_StatParam ( const FLOAT a_fHPPer = 1.0f , const FLOAT a_fDefensePer = 1.0f , const FLOAT a_fEvasionPer = 1.0f , const FLOAT a_fAtkPer = 1.0f );
	void SetMonsterInfoExtend_BonusParam ( const float a_fIncreaseExpRatio = 1.0f , const float a_fIncreaseDropItemProbabilityRatio = 1.0f , const float a_fInCreaseDropItemCountRatio = 1.0f );
// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

	BOOL GetMonsterDeadFlag(void);
	void SetMonsterDeadFlag(BOOL i_bDeadFlag);
	
	///////////////////////////////////////////////////////////////////////////
	// Method
	BOOL CheckDeadBodyCondition(void){		return COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK);}
	BOOL OnDead(vectorActionInfoPtr *o_pmtvecAttackedInfoPtr);
	ClientIndex_t GetClientIndexForTargetType(void);

	// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
	// ���� ����ġ ����.
	Experience_t rtn_MonsterExperience ( void );
	// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)

	// start 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
	D3DXVECTOR3	GetNearMultiTarget(D3DXVECTOR3 Target,  int *i_pMultiTargetIndex);
	D3DXVECTOR3	GetPointMultiTarget(int i_pMultiTargetIndex);
	BOOL IsMultiTargetMonster();
	// end 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
	
	// virtual
	virtual void ResetMonster(void);
	virtual void InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo);
	virtual void DeleteAttackedInfowithIndex(int i_AttackIndex = 0);

	mtvectorActionInfoPtr *GetAttackedInfoPtrList(void);
	void GetAttackedInfoClientIndexList(vectorClientIndex *o_pvectClientIdxList);

	// ���� ����
	BYTE						m_enMonsterState;				// ������ ���� ����(MS_NULL, MS_CREATED, MS_PLAYING, MS_DEAD)

	// ���� ����
	ClientIndex_t				m_nTargetIndex;					// ���Ͱ� �������� �ε���
	mtvectorActionInfoPtr		m_mtvectorAttackedInfoPtr;		// ���Ͱ� ���� ���� ActionInfo�� ������ ����Ʈ

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
	DWORD						m_dwChangedTargetIndexTick;
	void SetChangedTargetIndexTick(DWORD i_dwTick);
	DWORD GetChangedTargetIndexTick(void);
	ClientIndex_t GetMaxDamageUser(void);
	inline ClientIndex_t GetTargetIndex(void) { return m_nTargetIndex; }
	void ResetSumDamageInAttackedInfoList(void);

	BOOL IsValidMonster(BOOL i_bLiveCheck=TRUE);		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - CFieldMonster Ŭ������ �ִ����� �̰����� �̵�

	// ���� ����
	BYTE						m_byObjectMonsterType;			// Object Monster Type(0 = No Object Monster, 6 = EVENT_TYPE_OBJECT_MONSTER)
	
	BYTE						m_byMonsterTargetType;			// ���Ͱ� Ÿ������ ���� �� �ִ� Ÿ���� ����
	INT							m_nTargetTypeData;				// 
	mtvectClientIndex_t			m_mtvectClientIdxForTargetType;	// 

	BOOL						m_bDeadFlag;					// ���� ���� �÷���, 2005-09-05 by cmkwon
};
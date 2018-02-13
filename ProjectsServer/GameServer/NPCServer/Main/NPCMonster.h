// NPCMonster.h: interface for the CNPCMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMONSTER_H__17A3BE4F_8D53_496C_8631_94D066B1AEC7__INCLUDED_)
#define AFX_NPCMONSTER_H__17A3BE4F_8D53_496C_8631_94D066B1AEC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Monster.h"
#include "HPAction.h"		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -
#include "NPCSkillManager.h"	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -

typedef struct
{
	ClientIndex_t	AttackIndex;
	ITEM			*pAttackITEM;
	DWORD			dwExpireTick;			// 2005-12-30 by cmkwon, ���� Tick���� ����
	UINT			AttackedItemIndex;
} AttackedItemInfo;
typedef mt_vector<AttackedItemInfo>		mtvectorAttackedItemInfo;

typedef struct
{
	ClientIndex_t	m_TargetIndex;						// �������� �ε���
	INT				WeaponIndex;
	MONSTER_ITEM	*pAttackMonsterItem;
	DWORD			m_dwShootedTick;					// ���Ͱ� �������� ����� �ð�	
} ShootedSecondaryItemInfo;
typedef mt_vector<ShootedSecondaryItemInfo*>		mtvectorShootedSecondaryItemInfoPtr;

typedef struct _MOVEFLAGS
{
	SHORT	MovableFlag:2;						// �̵� ���� Flag
	SHORT	FBFlag:2;							// FrontAndBack Flag
	SHORT	FBDirect:2;							// FrontAndBack Direction
	SHORT	LRDirect:2;							// LeftAndRight	Direction
	SHORT	UDDirect:2;							// UpAndDown Direction
	SHORT	SpeedDirect:2;						// Speed Up or Down
} MOVEFLAGS;

typedef struct _MOVEINFO: public MOVEFLAGS
{	
	BYTE 	FBChangeBaseCount;					// FrontAndBack
	BYTE 	FBChangeCount;						// FrontAndBack
	BYTE	FBChangeRandomCount;
	BYTE	FBCurrentCount;						//

	BYTE 	LRChangeBaseCount;					// LeftAndRight
	BYTE 	LRChangeCount;						// LeftAndRight
	BYTE	LRChangeRandomCount;
	BYTE	LRCurrentCount;						// 
	BYTE	LRMoveDirection;					// ���� �̵� �ּҰ�
	BYTE	LRMoveDirectionRandomCount;			// ���� �̵� �ִ밪	
	
	BYTE 	LRSubChangeBaseCount;				// LeftAndRight
	BYTE 	LRSubChangeCount;					// LeftAndRight
	BYTE 	LRSubChangeRandomCount;				//
	BYTE	LRSubCurrentCount;					//	
	BYTE	LRSubMoveDirection;

	BYTE 	UDChangeBaseCount;					// UpAndDown
	BYTE 	UDChangeCount;						// UpAndDown
	BYTE 	UDChangeRandomCount;				// UpAndDown
	BYTE	UDCurrentCount;						//
	BYTE	UDMoveDirection;
	BYTE	UDMoveDirectionRandomCount;

	BYTE	UDSubChangeBaseCount;
	BYTE	UDSubChangeCount;
	BYTE	UDSubChangeRandomCount;
	BYTE	UDSubChangePercent;
	BYTE	UDSubCurrentCount;
	BYTE	UDSubMoveDirection;
	
	SHORT	MoveCount;
} MOVEINFO;

#define GET_STRING_VECTOR3(pVec3Pos)	GNPCGetStringVector3(pVec3Pos, string())
const char * GNPCGetStringVector3(D3DXVECTOR3 *i_pVec3Pos, string &str);


//////////////////////////////////////////////////////////////////////
// Local Global Function
//////////////////////////////////////////////////////////////////////
int GNPCRotateTargetVectorHorizontal(D3DXVECTOR3 *pDestTargetVector3, const D3DXVECTOR3 *pSourTargetVector3, int enDirection, int nRandomRange);
int GNPCRotateTargetVectorVertical(D3DXVECTOR3 *pDestTargetVector3, const D3DXVECTOR3 *pSourTargetVector3, int enDirection, int nRandomRange);
D3DXMATRIX GNPCGetMaxTargetVector(D3DXVECTOR3 *pCurTarget, D3DXVECTOR3 *pNextTarget, float fMaxRotationAngle);

///////////////////////////////////////////////////////////////////////////////
/// \class		CNPCMonster
///
/// \brief		NPC Server ���� ����ϴ� ���� Ŭ����
/// \author		cmkwon
/// \version	
/// \date		2004-03-25 ~ 2004-03-25
/// \warning	
///////////////////////////////////////////////////////////////////////////////
// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
class CWayPointManager;
// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����

class CNPCMapChannel;
class CNPCMapProject;
class CLIENT_INFO;
class CNPCMonster : public CMonster  
{
	friend CNPCMapChannel;
	friend CNPCMapProject;

	typedef struct
	{
		BYTE	EnforceTargetVector:1;				// ���� Ÿ�� ���� ������ ���� �������� �÷���
		BYTE	MoveTargetVectorSetAttack:1;		// ���ݽ� MoveTargetVector�� Ÿ���� ���ϵ��� ó���� �Ǿ������� �÷���
	} ST_NPCMONSTER_FLAG;
	
public:
	CNPCMonster();
	virtual ~CNPCMonster();

	///////////////////////////////////////////////////////////////////////////
	// Property
	void SetTargetIndex(ClientIndex_t i_nTargetIdx);
	void SetMoveState(SHORT i_MoveState);
	void SetMoveTargetVector(D3DXVECTOR3 * i_pVector3);
	void SetTargetVector(D3DXVECTOR3 * i_pVector3, BOOL i_bUnconditionalFlag=FALSE);
	void SetCurrentSpeed(float i_fSpeed);
	float GetCurrentSpeed(void);
	void SetTimeSetEnforceTargetVector(DWORD i_dwCurTick);
	void SetTimeEnforceTargetVectorTerm(USHORT i_usTerm);
	void SetTimeQuickTurnTerm(USHORT i_usRandTerm);
	void SetSpeedDirectMoveInfo(SHORT i_sSpeedDirect);
	void SetCurrentMonsterForm(SHORT i_sMonForm);
	void SetCurrentMovePattern(SHORT i_sMovePat);
	void SetUsingMonsterItem(MONSTER_ITEM *i_pMonItem);
	void SetCurrentTick(DWORD i_dwCurTick);

	// NPCFlag
	void SetNPCMonsterFlagEnforceTargetVector(BOOL i_bFlag);
	void SetNPCMonsterFlagMoveTargetVectorSetAttack(BOOL i_bFlag);


	///////////////////////////////////////////////////////////////////////////
	// Method
	// ���� ����
	void CreateNPCMonster(MONSTER_INFO * i_pMonInfo, D3DXVECTOR3 *i_pCreatePos, DWORD i_dwCurTick, int i_CreateInfoIndex
		, BYTE i_byMonTargetType, int i_nTargetTypeData, ClientIndex_t i_cltIdxForTargetType, int i_nMapMonsterVisibleDiameter
		, int i_nTargetIndex=0, BYTE i_byObjectMonsterType=EVENT_TYPE_NO_OBJECT_MONSTER, D3DXVECTOR3 *i_pUnitVec3ObjMonsterTarget=NULL
		, MONSTER_BALANCE_DATA * i_pMonInfoBalance = NULL );
	void InitializeMonster(void);
	void SetMonsterMoveInfo(void);

	// �̵� ����
	float GetSpeed(void);
	float GetQuickSpeed(void);
	void UpdateCurrentSpeed(void);
	void ResetLastAttackTime(DWORD i_dwCurTick);
	void ResetAllLastReAttackTime(DWORD i_dwCurTick);	
	void ResetCurrentShotNumCount(void);
	void UpdatePositionVector(float i_fTimeRate);
	void UpdateEnforceTargetVector(void);
	BOOL CheckMoveRange(void);
	void UpdateMoveInfoAllCurrentCount(void);
	void UpdateMoveInfoAttack(D3DXVECTOR3 *i_pVec3Pos, D3DXVECTOR3 * i_pUnitVec3Tar);
	void SetEnforceTargetVector(D3DXVECTOR3 * i_pVector3, float i_fSpeed, SHORT i_MoveState);
	void ResetPartyVariable(void);
	void ChangeBodyCondition(BodyCond_t * pBodyCon);
	void ResetAttackBodyCondition(void);
		
	// ���� ����
	int GetFirstTargetIndexfromAttackedInfo(void);
	void SelectTargetIndex(int nTargetIdx = 0);
	void LockVectorAttackedItemInfo(void);
	void UnlockVectorAttackedItemInfo(void);
	void InsertAttackedItemInfo(AttackedItemInfo * i_pItemInfo);
	BOOL SelectUsingMonsterItem(void);
	BOOL CheckEnableSearchTarget(void);

	// virtual
	virtual void ResetMonster(void);
	virtual void InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo);
	virtual void DeleteAttackedInfowithIndex(int i_AttackIndex = 0);


	static void GetRandomPlaneUnitVector(D3DXVECTOR3 * o_pVec3Tar);
	static void GetRandomVector(D3DXVECTOR3 * o_pVec3Tar);

	///////////////////////////////////////////////////////////////////////////////
	// 2008-04-16 by cmkwon, ���� ��� �� ���� ��ȯ �̺�Ʈ �ý��� ���� - 
	mtvectSSUMMON_EVENT_MONSTER		m_summonEventMonsterListAfterDead;
	void SetSummonEventMonsterListAfterDead(mtvectSSUMMON_EVENT_MONSTER *i_pSummonEvMonList);
	BOOL CheckSummonEventMonsterListAfterDead(SSUMMON_EVENT_MONSTER *o_pSummonEventMonster);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	void SetUserPositionVector(D3DXVECTOR3 * i_pUserPos, D3DXVECTOR3 * i_pUserTarget);
	BOOL SetCurrentMapChannel(CNPCMapChannel * i_pCurrentMapChannel);

	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
	BOOL IsChangeableTarget(void);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-06 by cmkwon, ���� ���ݽ� Ÿ�� ���� ���� üũ �߰�(Ÿ�ٺ���) - 
	DWORD						m_dwTimeCheckValidTarget;			// 1�ʸ��� Ÿ�� ��ȿ�� üũ�� ���� �ð�
	DWORD						m_dwTimeCheckedLastValidTarget;		// ������ ��ȿ�� Ÿ�� üũ �ð�
	DWORD GetTimeCheckValidTarget(void);
	void SetTimeCheckValidTarget(DWORD i_dwTick);
	DWORD GetTimeCheckedLastValidTarget(void);
	void SetTimeCheckedLastValidTarget(DWORD i_dwTick);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-16 by cmkwon, ����2�� MtoM, MtoC Ÿ�� ���� ���� ���� - 
	DWORD						m_dwTimeLastCheckChangeTarget;
	DWORD GetTimeLastCheckChangeTarget(void);
	void SetTimeLastCheckChangeTarget(DWORD i_dwTick);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	void CheckExpireSkill();			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - 
	void GetSkillTargets(vector<ClientIndex_t> *o_pVectorClientIndex, ClientIndex_t i_TargetIdx);	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ��ų Ÿ�� Ÿ������ ��� �ε��� ��������
	void UseSkill(ClientIndex_t i_TargetIdx);			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ��ų ���!!
	BOOL UseOneSkillETC(ClientIndex_t i_TargetIdx);			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - ���� ó���� �ʿ��� �ѹ� ��� ��ų 
	void UseSkillClick(vectClientIndex_t * i_pTargetIdxList);		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - Ŭ���� 
	void UseSkillTimeLimit(vectClientIndex_t * i_pTargetIdxList);	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - Ÿ�Ӹ�����

	// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
	CWayPointManager *GetWayPointManager();
	// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����

protected:
	// NPC Monster���� �ʿ��� Monster DB����
	SHORT						CurrentMonsterForm;					// Current Monster ����, NPC Server������ ��ȭ �Ѵ�.
	SHORT						CurrentMovePattern;					// Current MovePattern
	SHORT						SendMoveRange;						// Move ���� Range, ����
	
	// CNPCMonster���� ����ϴ� ��� Flag
	ST_NPCMONSTER_FLAG			m_FlagNPCMonster;

	// ���� ����
	USHORT						m_dwIndexCreatedMonsterData;		// ���Ͱ� ������ ���� ���� �ε���
	DWORD						m_dwTimeCreated;					// ���Ͱ� ������ Tick
	DWORD						m_dwTimeDeath;						// 2005-12-17 by cmkwon, ���Ͱ� ���ߵ� Tick
	
	// �̵� ����
	SHORT						m_enMoveState;						// ������ �̵� ���� ����(MSS_NORMAL, MSS_NORMAL_MAP_IMPACT, MSS_NORMAL_MONSTER_IMPACT, ...)
	MOVEINFO					m_MoveInfo;							//
	float						m_CurrentSpeed;						// ������ ���� Speed
	USHORT						m_usSpeedPercent;					// ������ Speed �����(2-2�� ������ �������� ����ϱ� ���Ѱ�)
	D3DXVECTOR3					m_BeforePosition;					//
	D3DXVECTOR3					m_MoveTargetVector;					// ���� ����(������, �δ�����)�� ������ ������ ���� �̵��� ���� Ÿ�ٺ��ͺ���
	D3DXVECTOR3					m_CreatedPosition;					//
	DWORD						m_dwTimeLastMoved;					// ������ ������ �̵� �ð�
//	DWORD						m_dwTimeDead;						// 2008-04-16 by cmkwon, ���� ��� �� ���� ��ȯ �̺�Ʈ �ý��� ���� - ������� �ʴ� ������, // ���Ͱ� ������ �ð�		
	int							m_nTimeGapChangeMonsterForm;		// 2���� ������ MonsterForm�� �������Ͱ� ���
	DWORD						m_dwLastRealMoveTime;				// ���Ͱ� �̵��� ������ ĳ���Ͱ� ������ �̵��� ���� �ʴ´�. ĳ���Ͱ� ������ �̵��� ������ �̵��ð�

	// ������ ���� �̵� ����
	DWORD						m_dwAttackMinDistance;				// ������ ���Ͱ� ���ݽ� ĳ���� ��������� �ּ� �Ÿ�, �� �Ÿ� ���� ��������� QuickTurn�̵���
	
	// ������ �̵� ����
	D3DXVECTOR3					m_EnforceTargetVector;				// ���Ͱ� ������ �ִ��� ���߾���� Ÿ�ٺ���
	DWORD						m_dwTimeSetEnforceTargetVector;		// EnforceTargetVector�� ������ �ð�
	USHORT						m_usTimeEnforceTargetVectorTerm;	// EnforceTargetVector�� ���ӵ� �ð�
	USHORT						m_usTimeQuickTurnTerm;				// �������� ���Ͱ� ���ݽ� QuickTurn�� �� ����ϱ� ���� �ð�

	// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
	INT							m_nWayPointNowStep;					// ���� ��������Ʈ ��ġ
	INT							m_nWayPointDirection;				// ���� ��������Ʈ ���� ���� (������ +1, ������ -1)
	// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����

	// ���� ó�� ��ƾ ����
	DWORD						m_dwTimeLastAttackRoutine;			// ���� ��ƾ�� �� ������ �ð�

	// ���� ����	
	mtvectorAttackedItemInfo	m_mtvectorAttackedItemInfo;			// ���Ͱ� ���ݴ������� Item ����
	MONSTER_ITEM				*m_pUsingMonsterItem;				// ���Ͱ� ���� ������� �������� ������
	DWORD						m_dwTimeChangeUsingMonsterItem;		// ���Ͱ� ��� �������� �ٲ� �ð�
	DWORD						m_dwCurrentTick;					
	DWORD						m_dwTimeMonsterLastAttack;			// ���Ͱ� ���������� ������ ������ �ð�
// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - CMonster�� �̵�
//	DWORD						m_dwSetTargetIndexLastTick;			// ���Ͱ� TargetIndex�� ������ ������ �ð�

	
	int							m_nCurrentShotNumCount;
	DWORD						m_ArrLastReattackTime[ARRAY_SIZE_MONSTER_ITEM];
	DWORD						m_dwLastAttackTime;

	// ���� ��Ƽ ����
	ClientIndex_t				m_nPartyManagerIndex;				// ���� ��Ƽ�� Index(�ڽŰ� ������ �ڽ��� ��Ƽ���̸� 0�� �ʱⰪ���� ��Ƽ ���°� �ƴ�)
	BYTE						m_byPartyMemberCounts;				// ��Ƽ�� ī��Ʈ - �ڽ��� �� ������ ��Ƽ������ ī��Ʈ, ��Ƽ�� ī��Ʈ�� 0�̸� ��Ƽ�� �����Ѵ�
	BYTE						m_byPartyFormationIndex;			// ��Ƽ�� �ε���

	// ��Ÿ
	DWORD						m_dwLastHPRecoveryTime;				// ���� HP Recovery Time
// 20040528 cmkwon, 2�� ����� NPC���� �߻縸 �ϰ� ������ ���� �ʴ´�, �̵��̳� Ÿ���� Client���� ó���Ѵ�.
//	mtvectorShootedSecondaryItemInfoPtr	m_mtvectorShootedSecondaryItemInfoPtr;		// ���Ͱ� �߻��� 2�� ���� ����, ItemKind�� ���� life Time�� ó���ϱ� ����, �̵��� Ŭ���̾�Ʈ����

	DWORD						m_dwSearchTargetLastTick;			// 2007-06-26 by cmkwon, ���� Ÿ�� �˻� �ð� ���� - ������ �˻� �ð� 

	/////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -
	CHPAction					m_HPAction;						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - HP�� ���� �ൿ
	ItemIdx_t					m_BarrierUseItemArrayIdx;			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �踮�� ��ų �迭 �ε���
	CNPCMapChannel	*			m_CurrentNPCMapChannel;				
	CNPCSkillManager			m_SkillManager;					// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
};

#endif // !defined(AFX_NPCMONSTER_H__17A3BE4F_8D53_496C_8631_94D066B1AEC7__INCLUDED_)

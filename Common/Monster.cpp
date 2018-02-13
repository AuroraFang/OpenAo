// Monster.cpp: implementation of the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Monster.h"

#ifdef _ATUM_FIELD_SERVER
#include "FieldIOCP.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonster::CMonster()
{
	MonsterIndex					= 0;
	m_nTargetIndex					= 0;
	m_mtvectorAttackedInfoPtr.lock();
	m_mtvectorAttackedInfoPtr.reserve(10);
	m_mtvectorAttackedInfoPtr.unlock();
	CMonster::ResetMonster();
}

CMonster::~CMonster()
{
	mt_auto_lock mtA(&m_mtvectorAttackedInfoPtr);
	if(m_mtvectorAttackedInfoPtr.empty() == false)
	{
		for (auto x : m_mtvectorAttackedInfoPtr) delete x;
		m_mtvectorAttackedInfoPtr.clear();
	}
}


///////////////////////////////////////////////////////////////////////////////
// Property
///////////////////////////////////////////////////////////////////////////////
void CMonster::SetMonsterIndex(int i_nMonsterIndex)
{
	ASSERT_REQUIRE(i_nMonsterIndex >= MONSTER_CLIENT_INDEX_START_NUM && i_nMonsterIndex < MONSTER_CLIENT_INDEX_END_NUM);
	MonsterIndex = i_nMonsterIndex;
}


// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

/***************************************************************************************************************************************
**
**	���� �⺻ ���� ������ ���.
**
**	Create Info :	??. ??. ??.
**
**	Update Info :	2010. 05. 25. by hsLee. - Ȯ�� ���� ���� ó��. ���� �߰��� ���� ������ �⺻���� ���� ���� �߰�.
**
****************************************************************************************************************************************/
/*void CMonster::SetMonsterInfoPtr(MONSTER_INFO * i_pMonsterInfo)
{
	ASSERT_ASSERT(i_pMonsterInfo != NULL);
	MonsterInfoPtr		= i_pMonsterInfo;
}*/
void CMonster :: SetMonsterInfoPtr(MONSTER_INFO * i_pMonsterInfo , const bool a_bReset_ExtendInfo /*= true*/ )
{
	ASSERT_ASSERT ( i_pMonsterInfo != NULL );
	MonsterInfoPtr = i_pMonsterInfo;

	if ( a_bReset_ExtendInfo )
	{
		SetMonsterInfoExtend_StatParam ();
		SetMonsterInfoExtend_BonusParam ();
	}
}


/********************************************************************************************
**
**	���� Ȯ�� ���� ����. - ������ ����ü�� ��� ����.
**
**	Create Info :	2010. 05. 25. by hsLee.
**
*********************************************************************************************/
void CMonster :: SetMonsterInfoExtend_AllData ( const MONSTER_BALANCE_DATA *a_pBalanceInfo )
{

	ASSERT_ASSERT ( MonsterInfoPtr != NULL );

	if ( NULL == MonsterInfoPtr || NULL == a_pBalanceInfo )
		return;


	// 2010. 07. 05 by hsLee. ���� ��ȯ ����. (�뷱�� ������ ���� ����.) - �뷱�� ������ ��ȿ�� üũ.
	if ( a_pBalanceInfo->IsValidData() )
	{
		SetMonsterInfoExtend_StatParam ( a_pBalanceInfo->fMaxHPRatio , a_pBalanceInfo->fDefenseRatio ,
											a_pBalanceInfo->fDefenseProbabilityRatio , a_pBalanceInfo->fAtkMsgRatio );
		
		SetMonsterInfoExtend_BonusParam ( a_pBalanceInfo->fIncreaseExpRatio , 
										a_pBalanceInfo->fIncreaseDropItemProbabilityRatio , 
										a_pBalanceInfo->fIncreaseDropItemCountRatio );
		
		MonsterInfoBalance = *a_pBalanceInfo;
	}
	else
	{
		MONSTER_BALANCE_DATA sTemp_DefBalanceData(0);

		SetMonsterInfoExtend_StatParam ( a_pBalanceInfo->fMaxHPRatio , a_pBalanceInfo->fDefenseRatio ,
											a_pBalanceInfo->fDefenseProbabilityRatio , a_pBalanceInfo->fAtkMsgRatio );

		SetMonsterInfoExtend_BonusParam ( a_pBalanceInfo->fIncreaseExpRatio ,
										a_pBalanceInfo->fIncreaseDropItemProbabilityRatio ,
										a_pBalanceInfo->fIncreaseDropItemCountRatio );

		MonsterInfoBalance = sTemp_DefBalanceData;
	}

}


/****************************************************************************************************
**
**	���� Ȯ�� ���� ���� - ���� �뷱�� ������. ( �ִ� HP, ����, ���Ȯ��, ���°� )
**
**	Create Info :	2010. 05. 20. by hsLee
**
*****************************************************************************************************/
void CMonster :: SetMonsterInfoExtend_StatParam ( const FLOAT a_fHPPer , const FLOAT a_fDefensePer , const FLOAT a_fEvasionPer , const FLOAT a_fAtkPer )
{
	ASSERT_ASSERT ( MonsterInfoPtr != NULL );

	if ( NULL == MonsterInfoPtr )
		return;

	MonsterInfoExtend.fMaxHP = (INT)(MonsterInfoPtr->MonsterHP * a_fHPPer);
	// 2010-07-19 by dhjin, Ȯ�� ���� ����
//	MonsterInfoExtend.Defense = (Prob255_t)(MonsterInfoPtr->Defense * a_fDefensePer);
//	MonsterInfoExtend.DefenseProbability = (Prob255_t)(MonsterInfoPtr->DefenseProbability * a_fEvasionPer);
	MonsterInfoExtend.Defense = MonsterInfoPtr->Defense * a_fDefensePer;
	MonsterInfoExtend.DefenseProbability = MonsterInfoPtr->DefenseProbability * a_fEvasionPer;
	MonsterInfoExtend.fAtkDmgRatio = a_fAtkPer;

	MonsterInfoBalance.fMaxHPRatio = a_fHPPer;
	MonsterInfoBalance.fDefenseRatio = a_fDefensePer;
	MonsterInfoBalance.fDefenseProbabilityRatio = a_fEvasionPer;
	MonsterInfoBalance.fAtkMsgRatio = a_fAtkPer;

}


/**************************************************************************************************
**
**	���� Ȯ�� ���� ���� - ���ʽ� ������. ( ����ġ, ������ ���Ȯ�� , ������ ��� ���� )
**
**	Create Info :	2010. 05. 20. by hsLee
**
***************************************************************************************************/
void CMonster :: SetMonsterInfoExtend_BonusParam ( const float a_fIncreaseExpRatio /*= 1.0f*/ , const float a_fIncreaseDropItemProbabilityRatio /*= 1.0f*/ , const float a_fInCreaseDropItemCountRatio /*= 1.0f*/ )
{
	ASSERT_ASSERT ( MonsterInfoPtr != NULL );

	if ( NULL == MonsterInfoPtr )
		return;

	MonsterInfoExtend.fIncreaseExpRatio = a_fIncreaseExpRatio;
	MonsterInfoExtend.fIncreaseDropProbabilityRatio = a_fIncreaseDropItemProbabilityRatio;
	MonsterInfoExtend.fIncreaseDropItemCountRatio = a_fInCreaseDropItemCountRatio;

	MonsterInfoBalance.fIncreaseExpRatio = a_fIncreaseExpRatio;
	MonsterInfoBalance.fIncreaseDropItemProbabilityRatio = a_fIncreaseDropItemProbabilityRatio;
	MonsterInfoBalance.fIncreaseDropItemCountRatio = a_fInCreaseDropItemCountRatio;

}

// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )


///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CMonster::GetMonsterDeadFlag(void)
/// \brief		
/// \author		cmkwon
/// \date		2005-09-05 ~ 2005-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMonster::GetMonsterDeadFlag(void)
{
	return m_bDeadFlag;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMonster::SetMonsterDeadFlag(BOOL i_bDeadFlag)
/// \brief		
/// \author		cmkwon
/// \date		2005-09-05 ~ 2005-09-05
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CMonster::SetMonsterDeadFlag(BOOL i_bDeadFlag)
{
	m_bDeadFlag = i_bDeadFlag;
}


///////////////////////////////////////////////////////////////////////////////
// virtual
///////////////////////////////////////////////////////////////////////////////

mtvectorActionInfoPtr *CMonster::GetAttackedInfoPtrList(void)
{
	return &m_mtvectorAttackedInfoPtr;
}

void CMonster::GetAttackedInfoClientIndexList(vectorClientIndex *o_pvectClientIdxList)
{
	mt_auto_lock mtA(&m_mtvectorAttackedInfoPtr);

	if(m_mtvectorAttackedInfoPtr.empty())
	{
		return;
	}

	mtvectorActionInfoPtr::iterator itr(m_mtvectorAttackedInfoPtr.begin());
	for(; itr != m_mtvectorAttackedInfoPtr.end(); itr++)
	{
		o_pvectClientIdxList->push_back((*itr)->UnitIndex);
	}
}

///////////////////////////////////////////////////////////////////////////////
// virtual
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMonster::ResetAll(void)
/// \brief
/// \author		cmkwon
/// \date		2004-03-25 ~ 2004-03-25
/// \warning
///
/// \param
/// \return
///////////////////////////////////////////////////////////////////////////////
void CMonster::ResetMonster(void)
{
// 2005-01-04 by cmkwon, �ʱ�ȭ ���� �ʾƵ� �ȴ�
//	MonsterInfoPtr					= NULL;
	CurrentHP						= 0;
	memset(&PositionVector, 0x00, sizeof(D3DXVECTOR3));
	memset(&TargetVector, 0x00, sizeof(D3DXVECTOR3));
	memset(&UpVector, 0x00, sizeof(D3DXVECTOR3));
	BodyCondition					= 0;

	m_enMonsterState				= MS_NULL;

	m_byObjectMonsterType			= EVENT_TYPE_NO_OBJECT_MONSTER;

	mt_auto_lock mtA(&m_mtvectorAttackedInfoPtr);	
	if(m_mtvectorAttackedInfoPtr.empty() == false)
	{
		for (auto x : m_mtvectorAttackedInfoPtr) delete x;
		m_mtvectorAttackedInfoPtr.clear();
	}
	mtA.auto_unlock_cancel();

	m_byMonsterTargetType			= MONSTER_TARGETTYPE_NORMAL;
	m_nTargetTypeData				= 0;
	m_mtvectClientIdxForTargetType.clearLock();

	m_bDeadFlag						= FALSE;

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CMonster::InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo)
/// \brief
/// \author		cmkwon
/// \date		2004-03-25 ~ 2004-03-25
/// \warning
///
/// \param
/// \return
///////////////////////////////////////////////////////////////////////////////
void CMonster::InserttoAttackedInfoList(ActionInfo *i_pAttackedInfo)
{
	ASSERT_NEVER_GET_HERE();
}

void CMonster::DeleteAttackedInfowithIndex(int i_AttackIndex)
{	
	mt_auto_lock mtA(&m_mtvectorAttackedInfoPtr);
	mtvectorActionInfoPtr::iterator it = find_if(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end(),
		listActionInfoPtr_UnitIndex_Find_Object(i_AttackIndex));
	if(it != m_mtvectorAttackedInfoPtr.end())
	{
		ActionInfo *pAInfo = *it;
		util::del(pAInfo);
		m_mtvectorAttackedInfoPtr.erase(it);
	}
}



BOOL CMonster::OnDead(vectorActionInfoPtr *o_pmtvecAttackedInfoPtr)
{
	mt_auto_lock mtA(&this->m_mtvectorAttackedInfoPtr);
	if(this->GetMonsterDeadFlag())
	{// ���Ͱ� �̹� ���� �����̴�.
		return FALSE;
	}
	
	this->SetMonsterDeadFlag(TRUE);
	*o_pmtvecAttackedInfoPtr = this->m_mtvectorAttackedInfoPtr;
	this->m_mtvectorAttackedInfoPtr.clear();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ClientIndex_t CMonster::GetClientIndexForTargetType(void)
/// \brief		
/// \author		cmkwon
/// \date		2006-08-09 ~ 2006-08-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ClientIndex_t CMonster::GetClientIndexForTargetType(void)
{
	mt_auto_lock mtA(&m_mtvectClientIdxForTargetType);
	if(m_mtvectClientIdxForTargetType.empty())
	{
		return 0;
	}

	return m_mtvectClientIdxForTargetType[0];
}


// 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)
/********************************************************************************************************
**
**	���� ����ġ ����. ( ���� óġ �� ȹ�淮. )
**
**	Create Info :	2010. 05. 31. by hsLee.
**
*********************************************************************************************************/
Experience_t CMonster :: rtn_MonsterExperience ( void )
{

	Experience_t rtn_exp = 0;

	if ( NULL == MonsterInfoPtr )
		return rtn_exp;

	rtn_exp = (Experience_t)(MonsterInfoPtr->Experience * MonsterInfoExtend.fIncreaseExpRatio);

	return rtn_exp;

}
// End 2010. 05. 31 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (������Ʈ ���� �뷱�� ���� ���� ����.)

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
/// \author		cmkwon
/// \date		2009-12-15 ~ 2009-12-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CMonster::SetChangedTargetIndexTick(DWORD i_dwTick)
{
	m_dwChangedTargetIndexTick = i_dwTick;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
/// \author		cmkwon
/// \date		2009-12-15 ~ 2009-12-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
DWORD CMonster::GetChangedTargetIndexTick(void)
{
	return m_dwChangedTargetIndexTick;
}


struct sort_ActionInfo_by_fSumDamage		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
{
	bool operator()(ActionInfo *op1, ActionInfo *op2)
	{
		return op1->fSumDamage > op2->fSumDamage;		// �������� ����
	}
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - �ְ� ������ ClientIndex�� �����Ѵ�.
/// \author		cmkwon
/// \date		2009-12-15 ~ 2009-12-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ClientIndex_t CMonster::GetMaxDamageUser(void)
{
	if(m_mtvectorAttackedInfoPtr.empty())
	{
		return 0;
	}

	mt_auto_lock mtA(&m_mtvectorAttackedInfoPtr);
	sort(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end(), sort_ActionInfo_by_fSumDamage());

	ActionInfo *pActionInfo = *(m_mtvectorAttackedInfoPtr.begin());
	mtA.auto_unlock_cancel();

	return pActionInfo->UnitIndex;
}

///////////////////////////////////////////////////////////////////////////////
// �Լ��� Ŭ����
//	ĳ���Ͱ� �Ÿ��ȿ� �ְ� ��ȿ�ϸ� �,������,������ ������ �ƴϸ� Vector�� �߰�
//
///////////////////////////////////////////////////////////////////////////////
class for_each_ResetSumDamage_IN_ActionInfoPtr
{
public:
	void operator()(ActionInfo *i_pActionInfo)
	{
		i_pActionInfo->fSumDamage	= 0.0f;
	}
};

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
/// \author		cmkwon
/// \date		2009-12-16 ~ 2009-12-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CMonster::ResetSumDamageInAttackedInfoList(void)
{
	if(m_mtvectorAttackedInfoPtr.empty())
	{
		return;
	}
	
	mt_auto_lock mtA(&m_mtvectorAttackedInfoPtr);
	for_each(m_mtvectorAttackedInfoPtr.begin(), m_mtvectorAttackedInfoPtr.end(), for_each_ResetSumDamage_IN_ActionInfoPtr());
	mtA.auto_unlock_cancel();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CMonster::IsValidMonster(BOOL i_bLiveCheck/*=TRUE*/)
/// \brief		
/// \author		dhjin
/// \date		2006-12-01 ~ 2006-12-01
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CMonster::IsValidMonster(BOOL i_bLiveCheck/*=TRUE*/)
{
	if(MS_NULL == m_enMonsterState
		|| NULL == MonsterInfoPtr)
	{
		return FALSE;
	}
	
	if(i_bLiveCheck)
	{
		if(MS_DEAD == m_enMonsterState
			|| COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

// start 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3	CMonster::GetNearMultiTarget(D3DXVECTOR3 Target)
/// \brief		��Ƽ Ÿ���� ������ �ִ��� Ȯ�� 
/// \author		hskim
/// \date		2011-03-22
/// \warning	
///
/// \param		
/// \return		��Ƽ Ÿ���� ������ ������ TRUE, ������ FASLE
///////////////////////////////////////////////////////////////////////////////
BOOL CMonster::IsMultiTargetMonster()
{
	// ���� �۾� - ���� �ý��� ������ ���� �̸� ����س��� ������� ó���� ����

	int a = 0;
	D3DXVECTOR3 ZeroVector= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	for(a=0; a<ARRAY_SIZE_MONSTER_TARGET; a++)
	{
		if( MonsterInfoPtr->MultiTargetInfo[a].Position == ZeroVector ) continue;

		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3	CMonster::GetPointMultiTarget(D3DXVECTOR3 Target)
/// \brief		�־��� �ε����� ��Ƽ Ÿ�� ��ǥ�� ��� �Լ�
/// \author		hskim
/// \date		2011-03-22
/// \warning	
///
/// \param		[i_pMultiTargetIndex, ��ƼŸ���� �ε��� - 0�� ��� �⺻ ��ǥ]
/// \return		���õ� ��Ƽ Ÿ�� ��ǥ
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	CMonster::GetPointMultiTarget(int i_pMultiTargetIndex)
{
	if( i_pMultiTargetIndex < 0 || i_pMultiTargetIndex > ARRAY_SIZE_MONSTER_TARGET ) return D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �߸��� �ε���
	if( i_pMultiTargetIndex == 0 ) return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	return (PositionVector + MonsterInfoPtr->MultiTargetInfo[i_pMultiTargetIndex - 1].Position);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			D3DXVECTOR3	CMonster::GetNearMultiTarget(D3DXVECTOR3 Target)
/// \brief		��Ƽ Ÿ���� �־��� ��ǥ���� ���� ����� Ÿ���� ��ǥ�� ����
/// \author		hskim
/// \date		2011-03-22
/// \warning	
///
/// \param		[Target, ��Ȱ Ÿ���� ��ǥ] [i_pMultiTargetIndex, ��ƼŸ���� �ε��� - 0�� ��� �⺻ ��ǥ]
/// \return		��Ƽ Ÿ���� ���� ����� Ÿ���� ��ǥ (���� ��ǥ)
///////////////////////////////////////////////////////////////////////////////
D3DXVECTOR3	CMonster::GetNearMultiTarget(D3DXVECTOR3 Target, int *i_pMultiTargetIndex)
{
	int a = 0;
	int fMinTargetCount = 0;		// ��Ƽ Ÿ���� ��� ���� ��� 0 ����
	float fTargetDist = 0.0f;
	float fMinTargetDist = D3DXVec3LengthSq(&(PositionVector - Target));
	D3DXVECTOR3 ZeroVector= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 NearVector = PositionVector;

	for(a=0; a<ARRAY_SIZE_MONSTER_TARGET; a++)
	{
		if( MonsterInfoPtr->MultiTargetInfo[a].Position == ZeroVector ) continue;

		// ������� ���� ���ϹǷ� ���ɸ� ���� D3DXVec3Length ���� D3DXVec3LengthSq �Լ� ���
		fTargetDist = D3DXVec3LengthSq(&(PositionVector + MonsterInfoPtr->MultiTargetInfo[a].Position - Target));

		if( fTargetDist < fMinTargetDist )
		{
			fMinTargetDist = fTargetDist;
			fMinTargetCount = a + 1;
			NearVector = PositionVector + MonsterInfoPtr->MultiTargetInfo[a].Position;
		}
	}

	*i_pMultiTargetIndex = fMinTargetCount;

	return NearVector;
}
// end 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰�
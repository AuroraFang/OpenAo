// EXPDivisionManager.h: interface for the CEXPDivisionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPDIVISIONMANAGER_H__A47BFE0B_8793_423B_ADF6_F9945E84C0F5__INCLUDED_)
#define AFX_EXPDIVISIONMANAGER_H__A47BFE0B_8793_423B_ADF6_F9945E84C0F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapBlock.h"
#include "FieldMonster.h"

class CFieldIOCP;
class CFieldIOCPSocket;
class CFieldParty;
class CFieldMapChannel;
struct SEXP_PARTYMEMBER
{
	CFieldIOCPSocket	*pFISock0;
	float				fDamage;
	float				fAggro;		// 2006-07-21 by cmkwon, �߰���

	SEXP_PARTYMEMBER()
	{
		pFISock0	= NULL;
		fDamage		= 0.0f;
		fAggro		= 0.0f;			// 2006-07-21 by cmkwon
	}
	SEXP_PARTYMEMBER(CFieldIOCPSocket *i_pFISock, float i_fDamage, float i_fAggro)
	{
		pFISock0	= i_pFISock;
		fDamage		= i_fDamage;
		fAggro		= i_fAggro;		// 2006-07-21 by cmkwon
	}

	float GetPartyDamage(void)
	{
		return fDamage + fAggro;
	}
};
struct SEXP_DIVISION
{
	SEXP_PARTYMEMBER	ArrSEXP_PARTYMEMBER[SIZE_MAX_PARTY_MEMBER];
	CFieldParty			*pFParty0;
	int					nPartyMemberCnts;
	float				fSumOfDamage;
	float				fSumOfAggro;
	float				fPartyBonus;
	BOOL				bIsApplyPartyBonus;			// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ���� �߰�

	SEXP_DIVISION()
	{
		util::zero(ArrSEXP_PARTYMEMBER, sizeof(ArrSEXP_PARTYMEMBER[0])*SIZE_MAX_PARTY_MEMBER);
		pFParty0				= NULL;
		nPartyMemberCnts		= 0;
		fSumOfDamage			= 0.0f;
		fPartyBonus				= 1.0f;
		bIsApplyPartyBonus		= FALSE;			// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ���� �߰�
	}
	SEXP_DIVISION(CFieldIOCPSocket *i_pFISock, CFieldParty *i_pFParty, float i_fDamage, float i_fAggro)
	{
		util::zero(ArrSEXP_PARTYMEMBER, sizeof(ArrSEXP_PARTYMEMBER[0])*SIZE_MAX_PARTY_MEMBER);
		ArrSEXP_PARTYMEMBER[0]	= SEXP_PARTYMEMBER(i_pFISock, i_fDamage, i_fAggro);
		pFParty0				= i_pFParty;
		nPartyMemberCnts		= 1;
		fSumOfDamage			= i_fDamage;
		fSumOfAggro				= i_fAggro;
		fPartyBonus				= 1.0f;
		bIsApplyPartyBonus		= FALSE;			// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ���� �߰�
	}

	void AddPartyMember(SEXP_DIVISION *i_pExpDivision)
	{
		ArrSEXP_PARTYMEMBER[nPartyMemberCnts]	= i_pExpDivision->ArrSEXP_PARTYMEMBER[0];
		fSumOfDamage	+= i_pExpDivision->ArrSEXP_PARTYMEMBER[0].fDamage;
		fSumOfAggro		+= i_pExpDivision->ArrSEXP_PARTYMEMBER[0].fAggro;
		nPartyMemberCnts++;
	}
};
typedef vector<SEXP_DIVISION>		vectSEXP_DIVISION;

class CEXPDivisionManager  
{
public:
	CEXPDivisionManager(CFieldIOCP *i_pFIOCP, CFieldMapChannel *i_pFMChann);
	~CEXPDivisionManager();

	float GetTotalOfDamage(void);
	int GetvectSEXP_DIVISIONListSize(void);
	listMonsterDropItem *GetlitMonsterDropItemPtr(void);

	void AddEXPDivision(CFieldIOCPSocket *i_pFISock, float i_fDamage, float i_fAggro);
	SEXP_DIVISION *FindEXPDivisionByFPartyPtr(CFieldParty *i_pFParty);
	void SortByDamageDecrease(void);
	BOOL GetSEXP_DIVISIONOfTopDamage(SEXP_DIVISION *o_pExpDivision);
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void ProcessEXPDivision(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster,CFieldMonster *pTargetMonster = NULL); 	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void ProcessPickUpDropItems(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, CFieldIOCPSocket *i_pFISockTopDamage);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void ProcessEXPDivisionInfinityMap(DROPITEM *o_pDropItem, MSG_FL_LOG_MONSTER_BOSS *o_pLogMonsterBoss, Experience_t i_exp, BOOL i_bBossMonster,CFieldMonster *pTargetMonster = NULL);	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������ - // 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �ʿ��� ��Ƽ ������� �� ��� ���� ����ġ �յ�й�.
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void ProcessPickUpDropItemsInfinityMap(string *o_pStrDropItemlist, DROPITEM *i_pDropItem, CFieldMonster *i_pDeadFMonster, CInfinityManager *i_pInfiMan);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� �ʿ��� ������ ó��, Tender�� InfinityBase���� ó���Ѵ�.

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-14 by cmkwon, 2009 ũ���������̺�Ʈ(�Ѹ�� ������ �ϳ���) - 
	CFieldIOCPSocket * GetNextUser(CFieldIOCPSocket *i_pCurFISoc, BOOL i_bLiveCheck=TRUE);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-04-09 by cmkwon, ����2�� �߰� ����(�ܰ躰 ���� �߰�) - 
	void ProcessPickUpTenderItemsInInfinityMap(string *o_pStrDropItemlist, CFieldMonster *i_pDeadFMonster, CInfinityManager *i_pInfiMan);

protected:
	CFieldIOCP				*m_pFieldIOCP15;
	CFieldMapChannel		*m_pFieldMapChannel1;

	float					m_fTotalOfDamage;
	vectSEXP_DIVISION		m_vectSEXP_DIVISIONList;
	listMonsterDropItem		m_litMonsterDropItem;
};

#endif // !defined(AFX_EXPDIVISIONMANAGER_H__A47BFE0B_8793_423B_ADF6_F9945E84C0F5__INCLUDED_)

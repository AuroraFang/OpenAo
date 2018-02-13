// CMemoryHackHistory.cpp: implementation of the CMemoryHackHistory class.
// 2012-12-14 by hskim, �޸��� �ڵ� �� ��� ����
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MemoryHackHistory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemoryHackHistory::CMemoryHackHistory()
{
	Clear();
}

CMemoryHackHistory::~CMemoryHackHistory()
{
}

void CMemoryHackHistory::Clear()
{
	m_nBlockThreshold = MEMORY_HACK_HISTORY_THRESHOLD;
	util::zero(m_nHistory, sizeof(m_nHistory));
}

void CMemoryHackHistory::SetBlockThreshold(int nThreshold)
{
	m_nBlockThreshold = nThreshold;
}

void CMemoryHackHistory::InsertResult(int nHackCheckType, BYTE nItemKind, int bResult)
{
	if( FALSE == IsValidHackCheckType(nHackCheckType) || FALSE == IS_VALID_ITEMKIND(nItemKind) )
	{
		return ;
	}

	if( TRUE == IsIgnore(nHackCheckType) )
	{
		return ;
	}

	if( TRUE == bResult )
	{
		m_nHistory[GetItemKindType(nItemKind)][nHackCheckType - 1]++;
	}
	else
	{
		m_nHistory[GetItemKindType(nItemKind)][nHackCheckType - 1] = 0;
	}
}

BOOL CMemoryHackHistory::IsBlock(int nHackCheckType, BYTE nItemKind)
{
	if( FALSE == IsValidHackCheckType(nHackCheckType) || FALSE == IS_VALID_ITEMKIND(nItemKind) )
	{
		return FALSE;
	}

	if( 0 == m_nBlockThreshold )
	{
		return FALSE;
	}

	if( m_nHistory[GetItemKindType(nItemKind)][nHackCheckType - 1] < m_nBlockThreshold )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CMemoryHackHistory::IsValidHackCheckType(int nHackCheckType)
{
	if( 0 <= nHackCheckType && nHackCheckType > HACK_CHECK_TYPE_END )
	{
		return FALSE;
	}
	
	return TRUE;
}

BYTE CMemoryHackHistory::GetItemKindType(BYTE nItemKind)
{
	/*		
	// ���⸦ �ٲ㵵 ����ǵ���  �ּ� ó��
	// ���� �ּ� ���� ����

	if(IS_PRIMARY_WEAPON(nItemKind))
	{
		return HISTORY_ITEMKIND_TYPE_PRIMARY_WEAPON;
	}
	else if(IS_SECONDARY_WEAPON(nItemKind))
	{
		return HISTORY_ITEMKIND_TYPE_SECONDARY_WEAPON;
	}
	*/

	return nItemKind;
}

BOOL CMemoryHackHistory::IsIgnore(int nHackCheckType)
{
	if( HACK_CHECK_TYPE_TOTALWEIGHT == nHackCheckType )		// ���Դ� ����
	{
		return TRUE;
	}

	return FALSE;
}

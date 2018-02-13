// TriggerNGCInflWarMonsterGroup.cpp: implementation of the CTriggerNGCInflWarMonsterGroup class.
//
/// \brief		// 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
/// \author		hskim
/// \date		2011-11-07
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TriggerNGCInflWarMonsterGroup.h"
#include "MapTriggerManager.h"
#include "TriggerFunctionNGCInflWar.h"
#include "FieldMapChannel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriggerNGCInflWarMonsterGroup::CTriggerNGCInflWarMonsterGroup()
{
	Clear();
}

CTriggerNGCInflWarMonsterGroup::~CTriggerNGCInflWarMonsterGroup()
{
	Destroy();
}

void CTriggerNGCInflWarMonsterGroup::Clear()
{
	m_Monster = 0;
	m_iSummonCount = 0;
	m_iSummonRandomPos = 0;

	m_pMapTriggerManager = NULL;
	m_pTriggerFunction = NULL;	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
}

BOOL CTriggerNGCInflWarMonsterGroup::Create(CMapTriggerManager *pMapTriggerManager, CTriggerFunction *pTriggerFunction, MonIdx_t Monster, INT iSummonCount, INT iSummonRandomPos)	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
{
	m_Monster = Monster;
	m_iSummonCount = iSummonCount;
	m_iSummonRandomPos = iSummonRandomPos;

	m_pMapTriggerManager = pMapTriggerManager;
	m_pTriggerFunction = pTriggerFunction;	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����

	return TRUE;
}

void CTriggerNGCInflWarMonsterGroup::Destroy()
{
	Clear();
}

BOOL CTriggerNGCInflWarMonsterGroup::CreateMonster()
{
	// NPC Server�� ���� (NPC ������ ���� �Ǿ� �ִ��� Ȯ���ϰ� ȣ��)

	if( NULL != m_pTriggerFunction && NULL != m_pTriggerFunction->m_pFieldMapChannel )	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pSummon, SendBuf2);
		pSummon->mapChann			= m_pTriggerFunction->m_pFieldMapChannel->GetMapChannelIndex();	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
		pSummon->MonsterUnitKind	= m_Monster;
		pSummon->MonsterSummonCount = m_iSummonCount;
		pSummon->MaxRandomDistance	= m_iSummonRandomPos;
		pSummon->MonsterBalanceInfo.Init(0);
		((CFieldIOCP*)(m_pTriggerFunction->m_pFieldMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBuf2, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE));	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
	}

	return TRUE;
}

BOOL CTriggerNGCInflWarMonsterGroup::DestroyMonster()
{
	if( NULL != m_pTriggerFunction && NULL != m_pTriggerFunction->m_pFieldMapChannel )	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
	{
		INIT_MSG_WITH_BUFFER(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, pDestroy, SendBufDestroy);
		pDestroy->mapChann			= m_pTriggerFunction->m_pFieldMapChannel->GetMapChannelIndex();	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
		pDestroy->MonsterUnitKind	= m_Monster;
		((CFieldIOCP*)(m_pTriggerFunction->m_pFieldMapChannel->m_pFieldMapWorkspace->m_pIOCPServer))->Send2NPCServerByTCP(SendBufDestroy, MSG_SIZE(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY));	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
	}

	return TRUE;
}

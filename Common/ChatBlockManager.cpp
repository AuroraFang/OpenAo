// ChatBlockManager.cpp: implementation of the CChatBlockManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatBlockManager.h"
#include <timeapi.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatBlockManager::CChatBlockManager()
{

}

CChatBlockManager::~CChatBlockManager()
{
	mt_auto_lock mtA(&m_mtmapChatBlockInfoPtr);
	
	for (auto& x : m_mtmapChatBlockInfoPtr) delete x.second;
	
	m_mtmapChatBlockInfoPtr.clear();
}


void CChatBlockManager::InsertChatBlockList(char *i_szCharacterName, int i_nBlockTermMinutes)
{
	// 2008-12-30 by cmkwon, ä�� ���� ��ҹ��� ���� ���� - 
	char szCapitalCharName[SIZE_MAX_CHARACTER_NAME];
	util::strncpy(szCapitalCharName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME);
	_strupr(szCapitalCharName);

	m_mtmapChatBlockInfoPtr.lock();
	{
		DWORD			dwCurTick = timeGetTime();
		DWORD			dwExpireTick = dwCurTick + i_nBlockTermMinutes*60*1000;
		SChatBlockInfo	*pBlockInfo = m_mtmapChatBlockInfoPtr.findNoLock(szCapitalCharName);	// 2008-12-30 by cmkwon, ä�� ���� ��ҹ��� ���� ���� - 
		if(NULL == pBlockInfo)
		{
			pBlockInfo = new SChatBlockInfo;
			strncpy(pBlockInfo->szBlockedCharacterName, szCapitalCharName, SIZE_MAX_CHARACTER_NAME);	// 2008-12-30 by cmkwon, ä�� ���� ��ҹ��� ���� ���� - 
			pBlockInfo->dwChatBlockedTick	= dwCurTick;
			pBlockInfo->dwExpireTick		= dwExpireTick;			// ms ������ ����

			///////////////////////////////////////////////////////////////////////////////
			// ä�ú�����Ʈ�� �߰��Ѵ�
			m_mtmapChatBlockInfoPtr.insertLock(szCapitalCharName, pBlockInfo);		// 2008-12-30 by cmkwon, ä�� ���� ��ҹ��� ���� ���� - 
		}
		else if(pBlockInfo->dwExpireTick < dwExpireTick)
		{
			pBlockInfo->dwChatBlockedTick	= dwCurTick;
			pBlockInfo->dwExpireTick		= dwExpireTick;
		}
	}
	m_mtmapChatBlockInfoPtr.unlock();

}

void CChatBlockManager::DeleteChatBlockList(char *i_szCharacterName)
{
	// 2008-12-30 by cmkwon, ä�� ���� ��ҹ��� ���� ���� - 
	char szCapitalCharName[SIZE_MAX_CHARACTER_NAME];
	util::strncpy(szCapitalCharName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME);
	_strupr(szCapitalCharName);

	auto pBlockInfo = m_mtmapChatBlockInfoPtr.popLock(szCapitalCharName);		// 2008-12-30 by cmkwon, ä�� ���� ��ҹ��� ���� ���� - 
	util::del(pBlockInfo);
}

BOOL CChatBlockManager::IsExistChatBlockList(int *o_pRemainMinute, char *i_szCharacterName)
{
	// 2008-12-30 by cmkwon, ä�� ���� ��ҹ��� ���� ���� - 
	char szCapitalCharName[SIZE_MAX_CHARACTER_NAME];
	util::strncpy(szCapitalCharName, i_szCharacterName, SIZE_MAX_CHARACTER_NAME);
	_strupr(szCapitalCharName);

	m_mtmapChatBlockInfoPtr.lock();
	{
		SChatBlockInfo	*pBlockInfo = m_mtmapChatBlockInfoPtr.findNoLock(szCapitalCharName);	// 2008-12-30 by cmkwon, ä�� ���� ��ҹ��� ���� ���� - 
		if(pBlockInfo)
		{
			DWORD dwCurTick = timeGetTime();
			if(dwCurTick < pBlockInfo->dwExpireTick)
			{// ä�� �ð��� ������ ����

				*o_pRemainMinute = ((pBlockInfo->dwExpireTick-dwCurTick)/1000)/60;		// �����ð�(������ ����)
				m_mtmapChatBlockInfoPtr.unlock();
				return TRUE;
			}

			///////////////////////////////////////////////////////////////////////////////
			// ä�� �ð��� �������Ƿ� ä�������� ������
			this->DeleteChatBlockList(szCapitalCharName);		// 2008-12-30 by cmkwon, ä�� ���� ��ҹ��� ���� ���� - 
		}
	}
	m_mtmapChatBlockInfoPtr.unlock();

	return FALSE;
}

// 2008-12-30 by cmkwon, ������ ä�� ���� ī�� ���� - CChatBlockManager::CheckChatBlockList()�� ���� �߰�(vectChatBlockInfoPtr *o_pvectReleasedBlockInfoPtr=NULL)
void CChatBlockManager::CheckChatBlockList(vectChatBlockInfoPtr *o_pvectReleasedBlockInfoPtr/*=NULL*/)
{
	m_mtmapChatBlockInfoPtr.lock();
	{
		DWORD	dwCurTick = timeGetTime();
		mtmapChatBlockInfoPtr::iterator itr(m_mtmapChatBlockInfoPtr.begin());
		while (itr != m_mtmapChatBlockInfoPtr.end())
		{
			SChatBlockInfo *pBlockInfo = itr->second;
			if(pBlockInfo->dwExpireTick < dwCurTick)
			{// ä�� �ð��� ������
				
				// 2008-12-30 by cmkwon, ������ ä�� ���� ī�� ���� - �Ʒ��� ���� ����
				//util::del(pBlockInfo);
				if(NULL == o_pvectReleasedBlockInfoPtr)
				{
					util::del(pBlockInfo);
				}
				else
				{
					o_pvectReleasedBlockInfoPtr->push_back(pBlockInfo);	// 2008-12-30 by cmkwon, �˾Ƽ� �޸� ���� �ؾ� �Ѵ�.
					pBlockInfo	= NULL;
				}
				m_mtmapChatBlockInfoPtr.erase(itr++);
				continue;
			}

			itr++;
		}
	}
	m_mtmapChatBlockInfoPtr.unlock();
}
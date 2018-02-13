// StoreData.cpp: implementation of the CStoreData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StoreData.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFGameMain.h"
#include "AtumDatabase.h"
#include "Chat.h"
#include "SceneData.h"
#include "FieldWinSocket.h"
//#include "INFShop.h"
#include "INFCityShop.h"
#include "Interface.h"
#include "INFCityBase.h"
#include "MonsterData.h"
#include "EnemyData.h"
#include "ItemInfo.h"
#include "RangeTime.h"
#include "INFInven.h"
#include "WeaponItemInfo.h"
#include "Skill.h"
#include "AtumSound.h"
#include "QuestData.h"
#include "dxutil.h"
#include "TutorialSystem.h"
#include "GlobalImage.h"

#include "CustomOptimizer.h" // 2013-05-23 by bhsohn Ŭ���̾�Ʈ ���Ϸα�(LOG) �ý��� �߰�

// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
#include "INFLuckyMachine.h"
// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��

// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
#include "PetManager.h"
#include "INFOpMain.h"
#include "INFOpWnd.h"
// end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����

// 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���
#include "ArmorCollectManager.h"
// END 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2012-04-12 by mspark, �κ��丮 �ڵ� ����
struct CompareKind
{
	bool operator()(CItemInfo *ci1, CItemInfo *ci2)
	{
		if(ci1->GetItemInfo()->Kind < ci2->GetItemInfo()->Kind)
		{
			return true;
		}
		return false;
	}
};

struct CompareItemNum
{
	bool operator()(CItemInfo *ci1, CItemInfo *ci2)
	{
		if(ci1->GetItemInfo()->ItemNum < ci2->GetItemInfo()->ItemNum)
		{
			return true;
		}
		return false;
	}
};


struct CompareEnchantNumber
{
	bool operator()(CItemInfo *ci1, CItemInfo *ci2)
	{
		if(ci1->GetEnchantNumber() < ci2->GetEnchantNumber())
		{
			return true;
		}
		return false;
	}
};
// end 2012-04-12 by mspark, �κ��丮 �ڵ� ����

CStoreData::CStoreData()
{
	g_pStoreData = this;
	m_fWeight = 0;
	m_bTutorialBackUp = FALSE;

	// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
	m_byMarkInfluence = 0;
	m_bInfluenceMarkWar = FALSE;
	memset(&m_struInfluenceWarBackupWaingInfo, 0x00, sizeof(struInfluenceWarBackupWaingInfo));

	// 2009-01-12 by bhsohn �Ϻ� �߰� ��������
	m_fGearChatWeight = -1;
	m_bRqChangeItemPos = FALSE;
	// end 2009-01-12 by bhsohn �Ϻ� �߰� ��������

	// 2012-04-12 by mspark, �κ��丮 �ڵ� ����
	m_vecInvenItem.clear();
	// end 2012-04-12 by mspark, �κ��丮 �ڵ� ����

	// 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���
	m_pArmorCollectManager = new CArmorCollectManager;
	// END 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���
}

CStoreData::~CStoreData()
{
	g_pStoreData = NULL;
	CMapItemInventoryIterator itItem = m_mapItemUniqueNumber.begin();
	while(itItem != m_mapItemUniqueNumber.end())
	{
		util::del(itItem->second);
		itItem++;
	}
	// 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���
	if(m_pArmorCollectManager)
	{
		util::del(m_pArmorCollectManager);
		m_pArmorCollectManager = NULL;
	}
	// END 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���
}

void CStoreData::CreateChatOfItemUpdate( int nItemNum, int nOrgItemCount, int nNewItemCount )
{
	// 2005-04-29 by jschoi
	if(g_pTutorial->IsTutorialMode() == TRUE)
	{
		return;
	}
	
	int nUpdate = nNewItemCount - nOrgItemCount;
	if(nItemNum == MONEY_ITEM_NUMBER)
	{
		char buf[128];
		if( nUpdate > 0)
		{
			wsprintf( buf, STRMSG_C_ITEM_0002, nUpdate );//"%d ���ǰ� �߰� �Ǿ����ϴ�."
		}
		else if( nUpdate < 0)
		{
			nUpdate *= -1;
			wsprintf( buf, STRMSG_C_ITEM_0003, nUpdate );//"%d ���ǰ� ���� �Ǿ����ϴ�."
		}
		else
		{
			return;
		}
		g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE,CHAT_TAB_SYSTEM);
	}
	else
	{
		ITEM* pITEM = g_pDatabase->GetServerItemInfo( nItemNum );
		if( pITEM )
		{
			char buf[128];
			memset(buf, 0x00, 128);
			if( nNewItemCount == 0)
			{
				wsprintf(buf, STRMSG_C_ITEM_0004, pITEM->ItemName );//"������ %s��(��) �����Ǿ����ϴ�."
				
				// 2007-04-20 by bhsohn ����Ʈ �Ϸ� ���� ��Ʈ�� ��ġ ����
				if(strlen(buf)>0)
				{
					g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE,CHAT_TAB_SYSTEM );
				}
				// end 2007-04-20 by bhsohn ����Ʈ �Ϸ� ���� ��Ʈ�� ��ġ ����
			}
			else if( nUpdate > 0)
			{
				// 2007-04-20 by bhsohn ����Ʈ �Ϸ� ���� ��Ʈ�� ��ġ ����
				wsprintf(buf, STRMSG_C_ITEM_0006, pITEM->ItemName, nUpdate );//"������ %s, %d���� �߰� �Ǿ����ϴ�."
				if(strlen(buf)>0)
				{
					g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE ,CHAT_TAB_SYSTEM);				
				}
				// end 2007-04-20 by bhsohn ����Ʈ �Ϸ� ���� ��Ʈ�� ��ġ ����

				CMapCharacterQuestIterator it = g_pQuestData->m_mapCharacterQuest.begin();
				while(it != g_pQuestData->m_mapCharacterQuest.end())
				{
				         // 2005-08-02 ispark
					if(QUEST_STATE_IN_PROGRESS == it->second->QuestState)
					{	
						CQuest* pQuest = g_pQuestData->FindQuest( it->second->QuestIndex );
						if( pQuest != NULL )
						{
							BOOL bFail = FALSE;
							vector<ITEM_W_COUNT_MAP_AREA>::iterator itArea = pQuest->TargetMonsterItemVector.begin();
							while(itArea != pQuest->TargetMonsterItemVector.end())
							{
								if(0 >= itArea->ItemWithCount.ItemNum
									|| 0 > itArea->ItemWithCount.Count)
								{
									itArea++;
									continue;
								}
								
								CItemInfo *pItem = FindItemInInventoryByItemNum((*itArea).ItemWithCount.ItemNum);
								if((*itArea).ItemWithCount.ItemNum == pITEM->ItemNum)
								{
									wsprintf(buf, STRMSG_C_ITEM_0005, pITEM->ItemName, nNewItemCount, (*itArea).ItemWithCount.Count );//"������ %s, (%d/%d)���� �߰� �Ǿ����ϴ�."
									g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE );
									if(nNewItemCount < (*itArea).ItemWithCount.Count)
										bFail = TRUE;
								}
								else
								{
									if(pItem)
									{
										if(pItem->CurrentCount >= (*itArea).ItemWithCount.Count)
										{
										}
										else
										{
											bFail = TRUE;
										}
									}
									else{
										bFail = TRUE;
									}
								}

								itArea++;						
							}
							// �̼� �̿ϼ�
							if(bFail == FALSE)
							{								
								// �̼� �Ϸ� ������
								int nMissionIndex = g_pD3dApp->SerchMissionCondition(nNewItemCount);
								if(nMissionIndex != NULL)
								{
									// 2006-03-27 by ispark, ��Ƽ�̼� �߰�
									if(pQuest->IsPartyQuest())
									{
										g_pQuestData->SendFieldSocketQuestRequestSuccessCheck(nMissionIndex);
									}
									else
									{
										g_pGImage->SetImageInfo(MISSION_SUC, TRUE);
									}
								}
								break;
							}
						}
					}
					it++;
				}
				// 2007-04-20 by bhsohn ����Ʈ �Ϸ� ���� ��Ʈ�� ��ġ ����
				//wsprintf(buf, STRMSG_C_ITEM_0006, pITEM->ItemName, nUpdate );//"������ %s, %d���� �߰� �Ǿ����ϴ�."
			}
			else if( nUpdate < 0)
			{
				nUpdate *= -1;
//				wsprintf(buf, "������ %s, %d���� ���ҵǾ����ϴ�.", pITEM->ItemName, nUpdate );
			}
			// 2007-04-20 by bhsohn ����Ʈ �Ϸ� ���� ��Ʈ�� ��ġ ����
//			if(strlen(buf)>0)
//			{
//				g_pD3dApp->m_pChat->CreateChatChild( buf, COLOR_SKILL_USE );				
//			}
			// end 2007-04-20 by bhsohn ����Ʈ �Ϸ� ���� ��Ʈ�� ��ġ ����
		}
	}
}

int CStoreData::PutItemStore(char * p)
{
	FLOG("CStoreData::PutItemStore(char * p)");
	int usebyte = 0;
	int i=POS_ITEMWINDOW_OFFSET;
	if(IS_SKILL_ITEM(((ITEM_GENERAL*)p)->Kind))
	{
#ifdef _DEBUG
		char buf[256];
		wsprintf(buf,STRERR_C_SKILL_0001);//"��ų�� â�� ��. ��ڿ��� ���׸� �Ű��Ͽ� �ֽʽÿ�."
		g_pD3dApp->m_pChat->CreateChatChild(buf,COLOR_ERROR);
#endif
	}
	ITEM_GENERAL pItem;
	memcpy(&pItem, p, sizeof(ITEM_GENERAL));
	pItem.ItemInfo = g_pDatabase->GetServerItemInfo(pItem.ItemNum);
	usebyte = sizeof(ITEM_GENERAL);
	return usebyte;
}

void CStoreData::AddItemDataToUniqueNumberAndWindowPosition(CItemInfo* pItemInfo)
{
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.find(pItemInfo->UniqueNumber);
	if(it != m_mapItemUniqueNumber.end())
	{
		// 2006-10-16 by ispark, ����. �������� �����, ������ ����(������ ������ ���� �� �������� ������)
//		DBGOUT("\n\nERROR : CStoreData::AddItemDataToUniqueNumberAndWindowPosition(...)\n\n\n");
		CMapItemWindowInventoryIterator it2 = m_mapItemWindowPosition.find(it->second->ItemWindowIndex);
		if( it2 != m_mapItemWindowPosition.end())
		{
			m_mapItemWindowPosition.erase(it2);
		}

		// 2007-06-26 by bhsohn ������ ī���ͺ� ������ ���� ���� ����
		if(IS_COUNTABLE_ITEM(pItemInfo->ItemInfo->Kind))
		{
			CItemInfo* pTmpItemInfo = (it->second);
			pItemInfo->CurrentCount += pTmpItemInfo->CurrentCount;
		}
		// end 2007-06-26 by bhsohn ������ ī���ͺ� ������ ���� ���� ����

		util::del(it->second);
		m_mapItemUniqueNumber.erase(it);
		
	}
	m_mapItemUniqueNumber[pItemInfo->UniqueNumber] = pItemInfo;
	CMapItemWindowInventoryIterator it2 = m_mapItemWindowPosition.find(pItemInfo->ItemWindowIndex);

	if(pItemInfo->ItemWindowIndex == POS_INVALID_POSITION)
	{
		int nIndex = SearchEmpty();
		pItemInfo->ItemWindowIndex = nIndex;
		m_mapItemWindowPosition[nIndex] = pItemInfo;
	}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	else if (pItemInfo->ItemWindowIndex == POS_HIDDEN_ITEM)		
		return;
										  
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	
	else if(it2 != m_mapItemWindowPosition.end())
	{
		// ���õ� ���Կ� �������� ������ �� ������ ã�Ƽ� �ִ´�.
		int index=POS_ITEMWINDOW_OFFSET;
		while(1)
		{
			CMapItemWindowInventoryIterator itEmpty = m_mapItemWindowPosition.find(index);
			// �˻��� ������ ����ִٸ� �װ��� ���� �������� �ְ� ���ο� �������� �� ��ġ�� �ִ´�.
			if(itEmpty == m_mapItemWindowPosition.end())
			{
//				pItemInfo->ItemWindowIndex = index;
//				m_mapItemWindowPosition[index] = pItemInfo;
				it2->second->ItemWindowIndex = index;
				m_mapItemWindowPosition[index] = it2->second;
				m_mapItemWindowPosition[pItemInfo->ItemWindowIndex]=pItemInfo;
				break;
			}
			index++;
		}
	}
	else
	{
		m_mapItemWindowPosition[pItemInfo->ItemWindowIndex]=pItemInfo;
	}

	// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
	if(g_pGameMain && g_pGameMain->m_pInven && g_pGameMain->m_pInven->IsShowInven())
	{
		g_pGameMain->m_pInven->UpdateInvenScrollMax();
	}
	// end 2008-08-22 by bhsohn EP3 �κ��丮 ó��
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int CStoreData::PutItem(char * p, BOOL bUpdate)
/// \brief		������ �߰�
/// \author		dhkwon
/// \date		2004-06-23 ~ 2004-06-23
/// \warning	bUpdate == FALSE ? ITEM_GENERAL ������ ������Ʈ�� �Ǿ �� ����(������ ����)
///				bUpdate == TRUE ? ITEM_GENERAL ������ ī��Ʈ�� ���ؾ� �Ѵ�.(�ŷ�)
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CStoreData::PutItem(char * p, BOOL bUpdate)
{
	FLOG("CStoreData::PutItem(char * p, BOOL bUpdate)");
	int usebyte = 0;

	if(IS_SKILL_ITEM(((ITEM_BASE*)p)->Kind))
	{
		//		ITEM_SKILL* pItem = new ITEM_SKILL;
		//		memcpy(pItem, p, sizeof(ITEM_SKILL));
		((ITEM_SKILL*)p)->ItemInfo = g_pDatabase->GetServerItemInfo(((ITEM_SKILL*)p)->ItemNum);
		usebyte = sizeof(ITEM_SKILL);
		g_pShuttleChild->m_pSkill->PutSkill((ITEM_SKILL*)p);		// 2004-07-23 jschoi
	}
	else
	{
		CItemInfo* pItemInfo = new CItemInfo((ITEM_GENERAL*)p);
		usebyte = sizeof(ITEM_GENERAL);
		// ���� ��Ȳ ó��///////////////////////
		if(pItemInfo->ItemNum == MONEY_ITEM_NUMBER)
		{
			if(g_pGameMain && g_pGameMain->m_pInven)
			{
				if(bUpdate)
				{
					g_pGameMain->m_pInven->AddItemSpi(pItemInfo->CurrentCount);
				}
				else
				{
					if(0 != g_pGameMain->m_pInven->GetItemSpiUniqueNumber())
					{
						g_pGameMain->m_pInven->AddItemSpi(pItemInfo->CurrentCount);
					}
					else
					{
						g_pGameMain->m_pInven->SetItemSpi(pItemInfo->CurrentCount);
					}
				}
//				g_pGameMain->m_pInven->SetItemSpi(pItemInfo->CurrentCount);
				if(0 == g_pGameMain->m_pInven->GetItemSpiUniqueNumber())
				{
					g_pGameMain->m_pInven->SetItemSpiUniqueNumber(pItemInfo->UniqueNumber);
				}
			}
			delete pItemInfo;
			return usebyte;
		}

		switch(pItemInfo->Kind)
		{
		case ITEMKIND_AUTOMATIC:
		case ITEMKIND_VULCAN:
		case ITEMKIND_DUALIST:					// 2005-08-02 by ispark
//		case ITEMKIND_GRENADE:                                   // ��� ����
		case ITEMKIND_CANNON:
		case ITEMKIND_RIFLE:
		case ITEMKIND_GATLING:
		case ITEMKIND_LAUNCHER:
		case ITEMKIND_MASSDRIVE:
		case ITEMKIND_ROCKET:
		case ITEMKIND_MISSILE:
		case ITEMKIND_BUNDLE:
		case ITEMKIND_MINE:
		case ITEMKIND_SHIELD:
		case ITEMKIND_DUMMY:
		case ITEMKIND_FIXER:
		case ITEMKIND_DECOY:
		case ITEMKIND_DEFENSE:
		case ITEMKIND_SUPPORT:
//		case ITEMKIND_CARD:
//		case ITEMKIND_ENCHANT:
//		case ITEMKIND_GAMBLE:
		case ITEMKIND_TANK:
		case ITEMKIND_QUEST:
		case ITEMKIND_RADAR:
		case ITEMKIND_COMPUTER:
//		case ITEMKIND_PREVENTION_DELETE_ITEM:
		case ITEMKIND_BLASTER:					// 2005-08-02 by ispark
		case ITEMKIND_RAILGUN:					// 2005-08-02 by ispark
		case ITEMKIND_ACCESSORY_UNLIMITED:		// 2006-03-30 by ispark �Ǽ��縮 �߰� (ex. ���� ������)
		case ITEMKIND_ACCESSORY_TIMELIMIT:		// 2006-03-30 by ispark �Ǽ��縮 �߰� (ex. ���� ������)
		case ITEMKIND_INFLUENCE_BUFF:
		case ITEMKIND_INFLUENCE_GAMEEVENT:
		case ITEMKIND_RANDOMBOX:				// 2006-08-10 by dgwoo
		case ITEMKIND_MARK:						// 2006-08-21 by ispark

		case ITEMKIND_PET_SOCKET_ITEM:			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		// 2010-06-15 by shcho&hslee ��ý���
		case ITEMKIND_PET_ITEM :
			{
				// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
				tPET_CURRENTINFO * tempCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData( pItemInfo->UniqueNumber );
				if(tempCurrentInfo)
				{
					tPET_LEVEL_DATA* tempLevelData = g_pDatabase->GetPtr_PetLevelData( tempCurrentInfo->PetIndex,tempCurrentInfo->PetLevel );
					tPET_BASE_ALL_DATA* tempBaseData = g_pDatabase->GetPtr_PetAllDataByIndex(tempCurrentInfo->PetIndex);//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
					if(tempLevelData)
					{
						pItemInfo->SetPetLevel(tempLevelData->Level);
					}
					pItemInfo->SetPetExp( tempCurrentInfo->PetExp );
					//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
					pItemInfo->SetPetEnableLevelUp(tempBaseData->BaseData.EnableLevel );
					pItemInfo->SetReName(tempBaseData->BaseData.EnableName);
	
				//	pItemInfo->SetPetEnableLevelUp(tempCurrentInfo-> EnableLevelUp);
				//	pItemInfo->SetReName(tempCurrentInfo->EnableEditPetName);
					//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				}
                // end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����										
				AddItemDataToUniqueNumberAndWindowPosition(pItemInfo);
				if(pItemInfo->Wear != WEAR_NOT_ATTACHED)
				{
					SetWearItemData(pItemInfo);
					// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
					if((pItemInfo->Wear==WEAR_ATTACHED) && (pItemInfo->Kind == ITEMKIND_MARK))
					{
						// ����� ��ũ�� �ִٸ� ����
						UpdateInfluenceMark();

					}
					// end 2008-08-19 by bhsohn ä�ù��� ���� ����

					// 2010. 02. 11 by ckPark �ߵ��� ����������
					// ����� �������̰� �ߵ������Ķ��� �������ִٸ� ��Ÿ�� ����Ʈ�� �ִ´�
					if( pItemInfo->Wear	== WEAR_ATTACHED
						&& (pItemInfo->ItemInfo->InvokingDestParamID || pItemInfo->ItemInfo->InvokingDestParamIDByUse)
						&& (pItemInfo->ItemInfo->ReAttacktime > 0) )
					{
						g_pGameMain->PushInvokeWearItem( pItemInfo->UniqueNumber );
					}
					// end 2010. 02. 11 by ckPark �ߵ��� ����������
				}
			}
			break;
		case ITEMKIND_CARD:
		case ITEMKIND_ENCHANT:
		case ITEMKIND_GAMBLE:
		case ITEMKIND_PREVENTION_DELETE_ITEM:

		case ITEMKIND_ENERGY:
		case ITEMKIND_INGOT:
		case ITEMKIND_BULLET:
		case ITEMKIND_SKILL_SUPPORT_ITEM:		// 2006-10-02 by ispark
			{
				// Energy���� �������� ���ļ� ���Ǿ�� ������ ������ ���� �˻��Ͽ����Ѵ�.
				CMapItemInventoryIterator it = m_mapItemUniqueNumber.find(pItemInfo->UniqueNumber);
				if(it != m_mapItemUniqueNumber.end())
				{
					if(bUpdate)
					{
						it->second->CurrentCount += pItemInfo->CurrentCount;
					}
					else
					{
						it->second->CurrentCount = pItemInfo->CurrentCount;
						//memcpy((void*)(it->second), (void*)pItemInfo, sizeof(ITEM_GENERAL));
					}
					delete pItemInfo;
				}
				else
				{
					AddItemDataToUniqueNumberAndWindowPosition(pItemInfo);
				}
			}
			break;
		}
		// 2007-04-09 by dgwoo �ͽ� ����Ʈ �߰��� ������ �Ϸ� ������ �ѹ��� �߰��Ѵ�.
		//--------------------------------------------------------------------------//
		int nMissionIndex = g_pD3dApp->SerchMissionCondition();
		if(nMissionIndex != NULL)
		{
//			CQuest * pQuest = g_pQuestData->FindQuest(nMissionIndex);
//			if(pQuest->TargetMixItemTarget.Count > 0)
//			{
				g_pGImage->SetImageInfo(MISSION_SUC, TRUE);
//			}
		}
		//--------------------------------------------------------------------------//
	}
	return usebyte;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CStoreData::SetWearItemData(CItemInfo* pItemInfo)
/// \brief		���� ������ �����Ѵ�.
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	pItemInfo->ItemInfo�� ���� ����Ÿ�� �ƴϴ�.
///				���Ŀ� pItemInfo->GetRealItemInfo();�� �ٲٵ��� �Ѵ�.
///				m_mapSkillItemCheckTime�̰͵� CItemInfo�� �־�� �Ѵ�.
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::SetWearItemData(CItemInfo* pItemInfo)
{
//	if(pItemInfo->Wear==WEAR_LOADED)
	if(pItemInfo->Wear==WEAR_ATTACHED)
	{
		if(IS_PRIMARY_WEAPON(pItemInfo->Kind))
		{
			ASSERT_ASSERT(g_pShuttleChild->m_pPrimaryWeapon == NULL);
			float fOverHeatTime = CAtumSJ::GetOverheatTime(pItemInfo->GetRealItemInfo(),&g_pShuttleChild->m_paramFactor);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_PROWOUT, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_PROWOUT, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			g_pShuttleChild->m_pSkill->PutSkillFromWearItem(pItemInfo->GetRealItemInfo()->Kind);
			g_pShuttleChild->m_pPrimaryWeapon = 
				new CWeaponItemInfo( pItemInfo, &g_pShuttleChild->m_paramFactor, fOverHeatTime, FALSE ); // 0, 0 : overheat
			memset(&g_pShuttleChild->m_PrimaryAttack, 0x00, sizeof(ATTACK_DATA));
			g_pShuttleChild->m_PrimaryAttack.nMultiNumCount = g_pShuttleChild->GetMultiNum( pItemInfo->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor );
			g_pShuttleChild->SetWeaponData(0,TRUE);//,TRUE);
		}
		else if(IS_SECONDARY_WEAPON(pItemInfo->Kind))
		{
			ASSERT_ASSERT(g_pShuttleChild->m_pSecondaryWeapon == NULL);
			float fOverHeatTime = CAtumSJ::GetOverheatTime(pItemInfo->GetRealItemInfo(),&g_pShuttleChild->m_paramFactor);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_WINGOUT, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_WINGOUT, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			g_pShuttleChild->m_pSkill->PutSkillFromWearItem(pItemInfo->GetRealItemInfo()->Kind);
			g_pShuttleChild->m_pSecondaryWeapon = 
				new CWeaponItemInfo( pItemInfo, &g_pShuttleChild->m_paramFactor, fOverHeatTime, FALSE ); // 0, 0 : overheat
			memset(&g_pShuttleChild->m_SecondaryAttack, 0x00, sizeof(ATTACK_DATA));
			g_pShuttleChild->m_SecondaryAttack.nMultiNumCount = g_pShuttleChild->GetMultiNum( pItemInfo->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor );
			g_pShuttleChild->SetWeaponData(1,TRUE);//,TRUE);
		}
//	}
//	else if(pItemInfo->Wear == WEAR_ATTACHED)
//	{
		else if(pItemInfo->Kind == ITEMKIND_SUPPORT)
		{
			DBGOUT("Engine Setting\n");
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_REAR, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_REAR, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			g_pShuttleChild->ChangeEngine(pItemInfo, pItemInfo->CurrentCount);
			g_pShuttleChild->SetShuttleSpeed();
		}
		else if(pItemInfo->Kind == ITEMKIND_DEFENSE)
		{
			int nColorCode = 0;
			if(pItemInfo->ColorCode == 0)	nColorCode = pItemInfo->ItemNum;
			else nColorCode = pItemInfo->ColorCode;

			// 2006-02-17 by ispark
//			g_pD3dApp->SetUnitArmorColorNum(max(1, nColorCode%100));
			g_pShuttleChild->SetAmorColor(max(1,nColorCode%100));
			g_pShuttleChild->SetInvenAmorColor(max(1,nColorCode%100));		// 2006-05-03 by ispark
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_CENTER, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_CENTER, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			// 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
			//g_pShuttleChild->SetAmorHeight(pItemInfo->ItemInfo->SourceIndex);
			// end 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
			// ��ü�� ���õ� ���̹Ƿ� FALSE
//			g_pShuttleChild->ChangeUnitCharacterInfo(pItemInfo->ItemNum, 
//				g_pShuttleChild->m_myShuttleInfo.UnitKind, g_pShuttleChild->GetCurrentBodyCondition(), FALSE);
			// 2005-11-24 by ispark, ItemNum -> SourceIndex���� ����


			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
// 			g_pShuttleChild->ChangeUnitCharacterInfo(pItemInfo->ItemInfo->SourceIndex, 
// 				g_pShuttleChild->m_myShuttleInfo.UnitKind, g_pShuttleChild->GetCurrentBodyCondition(), FALSE);

			int nDefenseNum = 0;
#ifdef _REWORKED_COLORSHOP
			if (pItemInfo->ColorCode)
			{
				nDefenseNum = pItemInfo->ColorCode;
			}
			else
			{
#endif
				if (!pItemInfo->ShapeItemNum)
					nDefenseNum = pItemInfo->ItemInfo->SourceIndex;
				else
				{
					ITEM* pShapeItem = g_pDatabase->GetServerItemInfo(pItemInfo->ShapeItemNum);
					if (pShapeItem)
						nDefenseNum = pShapeItem->SourceIndex;
					else
						nDefenseNum = pItemInfo->ItemInfo->SourceIndex;
				}
#ifdef _REWORKED_COLORSHOP
			}
#endif
			// 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
			g_pShuttleChild->SetAmorHeight(nDefenseNum);
			// end 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
			// 2010. 03. 18 by jskim ���ͺ��� ī��
// 			g_pShuttleChild->ChangeUnitCharacterInfo( nDefenseNum,
// 				g_pShuttleChild->m_myShuttleInfo.UnitKind,
// 				g_pShuttleChild->GetCurrentBodyCondition(),
// 													  FALSE );
			BOOL bMonTras = FALSE;

			if( g_pShuttleChild->GetMonsterTransformer() )
			{
				g_pShuttleChild->ChangeUnitCharacterInfo(nDefenseNum, g_pShuttleChild->m_myShuttleInfo.UnitKind, g_pShuttleChild->GetCurrentBodyCondition(), FALSE);
				g_pShuttleChild->SetMonsterTransPrimarySidePos(g_pShuttleChild->m_PrimaryAttack.vSidePos);
				g_pShuttleChild->SetMonsterTransSecondarySidePos(g_pShuttleChild->m_SecondaryAttack.vSidePos);
			}

			if( g_pShuttleChild->GetMonsterTransformer() )
			{
				bMonTras = TRUE;
				MEX_MONSTER_INFO * pTempIn = g_pGameMain->CheckMonsterInfo( g_pShuttleChild->GetMonsterTransformer() );
				if(pTempIn)
					nDefenseNum = pTempIn->SourceIndex;
			}
			g_pShuttleChild->ChangeUnitCharacterInfo( nDefenseNum,
													  g_pShuttleChild->m_myShuttleInfo.UnitKind,
													  g_pShuttleChild->GetCurrentBodyCondition(),
													  FALSE, bMonTras );
			//end 2010. 03. 18 by jskim ���ͺ��� ī��
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����


			// 2007-05-28 by bhsohn �����ۿ� ���� üũ�� �߰�			
			// �������� ã�Ҵ�.
			SendItemCheckSum(WEAR_ATTACHED, pItemInfo->Kind, pItemInfo->ItemInfo->ItemNum);			


			// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)
			
			// �Ƹ� �����
			// ���� ����Ʈ ������ ���ϵ��� ����
			g_pShuttleChild->ShowWeapon( FALSE );
			// A��� ���뵵 �����
			g_pShuttleChild->DeleteRobotAGearWeapon();
			
			// �κ� �ƸӰ� �ƴҰ�� �ٽ� ������ �ϵ��� ����
			if( !g_pShuttleChild->IsRobotArmor() )			
			{
				// 2010. 03. 18 by jskim ���ͺ��� ī��
				//g_pShuttleChild->ShowWeapon( TRUE );
				if(g_pShuttleChild->GetMonsterTransformer())
					g_pShuttleChild->ShowWeapon( FALSE );
				else
					g_pShuttleChild->ShowWeapon( TRUE );
				//end 2010. 03. 18 by jskim ���ͺ��� ī��
			}
			else
			{	
				// �κ� �Ƹ��� ���
				int		type = 0;
				ITEM*	pPrimaryWeapon = NULL;
				if(g_pShuttleChild->m_dwPartType == _SHUTTLE)
				{
					type = GetUnitTypeDecimal(g_pShuttleChild->m_myShuttleInfo.UnitKind);
					CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find( POS_PROWOUT );
					if( it != g_pStoreData->m_mapItemWindowPosition.end() )
						pPrimaryWeapon = it->second->ItemInfo;
				}
				
				// A����̰�, �ֹ��⸦ �����Ͽ��� ��� A���� �ӽ� ���� ����Ʈ �߰�
				if( type == 2 && pPrimaryWeapon )
				{
					ITEM* pAGearWeapon = g_pDatabase->GetServerItemInfo( A_GEAR_ROBOT_PRIMARY_WEAPON_ITEM_NUM );
					if( pAGearWeapon )
						g_pShuttleChild->CreateRobotAGearWeapon( pAGearWeapon, type );
				}
			}
			
			// ���â�� �޽��� �ʱ�ȭ���� ������, ���� ����Ʈ�� ���ļ� ���δ�
			g_pShuttleChild->InitInvenMesh();
			// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)

		}
		else if(pItemInfo->Kind == ITEMKIND_RADAR)
		{
			// 2005-08-23 by ispark
//			g_pShuttleChild->CreateWearItem(WEAR_ITEM_KIND_PROW, EFFECT_NUMBER_WEAR_RADAR(pItemInfo->ItemNum) );
			g_pShuttleChild->CreateWearItem(WEAR_ITEM_KIND_PROW, EFFECT_NUMBER_WEAR_RADAR(pItemInfo->ItemInfo->SourceIndex) );
			g_pShuttleChild->m_pRadarItemInfo = pItemInfo;			// 2005-08-17 by ispark
			
			// 2008-09-26 by bhsohn �ű� ��þƮ ó��
			g_pShuttleChild->SetRadarRange(g_pShuttleChild->m_pRadarItemInfo);
			// end 2008-09-26 by bhsohn �ű� ��þƮ ó��
		}
		// 2006-04-14 by ispark, �߰�
		// 2006-06-28 by ispark, ����Ʈ �߰�
		else if(pItemInfo->Kind == ITEMKIND_ACCESSORY_UNLIMITED)
		{
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED);
			g_pShuttleChild->CreateWearItem(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, pItemInfo->ItemInfo->SourceIndex, g_pD3dApp->m_bCharacter);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_ATTACHMENT, pItemInfo->GetRealItemInfo()  );
			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_ACCESSORY_UNLIMITED, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
		}
		else if(pItemInfo->Kind == ITEMKIND_ACCESSORY_TIMELIMIT)
		{
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT);
			g_pShuttleChild->CreateWearItem(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, pItemInfo->ItemInfo->SourceIndex, g_pD3dApp->m_bCharacter);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_PET, pItemInfo->GetRealItemInfo()  );
			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_ACCESSORY_TIME_LIMIT, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
		}
		else if ( pItemInfo->Kind == ITEMKIND_PET_ITEM )
		{
			g_pShuttleChild->DeleteWearItem ( WEAR_ITEM_KIND_PET );
			g_pShuttleChild->CreateWearItem ( WEAR_ITEM_KIND_PET , g_pShuttleChild->GetPetManager()->GetSourceIndex_PetCurrentData( pItemInfo->UniqueNumber , (g_pD3dApp->m_bCharacter == TRUE ? true : false ) ) , g_pD3dApp->m_bCharacter );

			// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
			CWeaponItemInfo* tempPetWeapon = new CWeaponItemInfo( pItemInfo, &g_pShuttleChild->m_paramFactor);
			g_pShuttleChild->GetPetManager()->SetWeaponItemInfo( tempPetWeapon );
			g_pShuttleChild->GetPetManager()->InitAttackData();
			memset(&g_pShuttleChild->m_PetAttack, 0x00, sizeof(ATTACK_DATA));
			g_pShuttleChild->SetPetPosition();
			// end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_PET , pItemInfo );
			// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
			g_pCharacterChild->SetPetWearItem( TRUE );
			g_pCharacterChild->SetCharacterAddSpeed( 0.0f );
			g_pCharacterChild->SetPetAddSpeed( FALSE );
			// end 2010-11-29 by jskim, �� ������ �̵��ӵ� ���� 
		}
		else if(pItemInfo->Kind == ITEMKIND_MARK)
		{
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_WINGIN);
			g_pShuttleChild->CreateWearItem(WEAR_ITEM_KIND_WINGIN, pItemInfo->ItemInfo->SourceIndex, g_pD3dApp->m_bCharacter);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_WINGIN, pItemInfo->GetRealItemInfo()  );
			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_WINGIN, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			// 2011-03-21 by jhAhn ��ũ�ý��� ��ũ ��ġ ����
			g_pShuttleChild->SetMarkPosition();
			// end 2011-03-21 by jhAhn ��ũ�ý��� ��ũ ��ġ ����
		}
		
	}
}

void CStoreData::UpdateWearItemData( CItemInfo* pItemInfo, BOOL bWearItem)
{
	CMapItemWindowInventoryIterator it1 = m_mapItemWindowPosition.find(POS_PROWOUT);
	if(it1 == m_mapItemWindowPosition.end() && g_pShuttleChild->m_pPrimaryWeapon)
	{
		g_pShuttleChild->PutUsedItem(g_pShuttleChild->m_pPrimaryWeapon);
		g_pShuttleChild->ReleaseWearItemToCharacterParamFactor( POS_PROWOUT );
		util::del(g_pShuttleChild->m_pPrimaryWeapon);
		g_pShuttleChild->SetWeaponData(0,TRUE);//,FALSE);
		if(bWearItem==FALSE)
			g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OFF_WEAPON, g_pShuttleChild->m_vPos, FALSE);
		memset(&g_pShuttleChild->m_PrimaryAttack, 0x00, sizeof(ATTACK_DATA));
	}
	CMapItemWindowInventoryIterator it2 = m_mapItemWindowPosition.find(POS_WINGOUT);
	if(it2 == m_mapItemWindowPosition.end() && g_pShuttleChild->m_pSecondaryWeapon)
	{
		g_pShuttleChild->PutUsedItem(g_pShuttleChild->m_pSecondaryWeapon);
		g_pShuttleChild->ReleaseWearItemToCharacterParamFactor( POS_WINGOUT );
		util::del(g_pShuttleChild->m_pSecondaryWeapon);
		g_pShuttleChild->SetWeaponData(1,TRUE);//,FALSE);
		if(bWearItem==FALSE)
			g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
		g_pD3dApp->m_pSound->PlayD3DSound(SOUND_OFF_WEAPON, g_pShuttleChild->m_vPos, FALSE);
		memset(&g_pShuttleChild->m_SecondaryAttack, 0x00, sizeof(ATTACK_DATA));
	}
	CMapItemWindowInventoryIterator it3 = m_mapItemWindowPosition.find(POS_CENTER);
	if(it3 == m_mapItemWindowPosition.end() )
	{
		g_pShuttleChild->ReleaseWearItemToCharacterParamFactor( POS_CENTER );
		// ��ü�� ���õ� ���̹Ƿ� FALSE
		g_pShuttleChild->ChangeUnitCharacterInfo(0,	g_pShuttleChild->m_myShuttleInfo.UnitKind, 
			g_pShuttleChild->GetCurrentBodyCondition(), FALSE);
	}
	if( pItemInfo->Wear == WEAR_ATTACHED )
	{
		if(IS_PRIMARY_WEAPON(pItemInfo->Kind))
		{
			DBGOUT("PRIMARY_WEAPON Change\n");
			if(g_pShuttleChild->m_pPrimaryWeapon)
			{
				g_pShuttleChild->PutUsedItem(g_pShuttleChild->m_pPrimaryWeapon);
			}
			util::del(g_pShuttleChild->m_pPrimaryWeapon);
			USED_ITEM* pUsedItem = g_pShuttleChild->GetUsedItem(pItemInfo->UniqueNumber);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_PROWOUT, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_PROWOUT, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			if(pUsedItem)
			{
				g_pShuttleChild->m_pPrimaryWeapon = 
					new CWeaponItemInfo(pItemInfo, &g_pShuttleChild->m_paramFactor,pUsedItem->fOverHeatCheckTime,pUsedItem->bIsOverHeat);// 0,0:over heat				
			}
			else
			{
				float fOverHeatTime = CAtumSJ::GetOverheatTime(pItemInfo->GetRealItemInfo(),&g_pShuttleChild->m_paramFactor);
				g_pShuttleChild->m_pPrimaryWeapon = 
					new CWeaponItemInfo(pItemInfo, &g_pShuttleChild->m_paramFactor,fOverHeatTime,FALSE);// 0,0:over heat
			}
			g_pShuttleChild->m_pSkill->PutSkillFromWearItem(g_pShuttleChild->m_pPrimaryWeapon->GetRealItemInfo()->Kind);
			memset(&g_pShuttleChild->m_PrimaryAttack, 0x00, sizeof(ATTACK_DATA));
			g_pShuttleChild->m_PrimaryAttack.nMultiNumCount = g_pShuttleChild->GetMultiNum( pItemInfo->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor );
			g_pShuttleChild->SetWeaponData(0,TRUE);//,TRUE);
			g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_WEAR_WEAPON, g_pShuttleChild->m_vPos, FALSE);

		}
		else if(IS_SECONDARY_WEAPON(pItemInfo->Kind))
		{
			DBGOUT("SECONDARY_WEAPON Change\n");
			if(g_pShuttleChild->m_pSecondaryWeapon)
			{
				g_pShuttleChild->PutUsedItem(g_pShuttleChild->m_pSecondaryWeapon);
			}
			util::del(g_pShuttleChild->m_pSecondaryWeapon);
			USED_ITEM* pUsedItem = g_pShuttleChild->GetUsedItem(pItemInfo->UniqueNumber);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_WINGOUT, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_WINGOUT, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			if(pUsedItem)
			{
				g_pShuttleChild->m_pSecondaryWeapon = 
					new CWeaponItemInfo(pItemInfo, &g_pShuttleChild->m_paramFactor,pUsedItem->fOverHeatCheckTime,pUsedItem->bIsOverHeat);// 0,0:over heat				
			}
			else
			{
				float fOverHeatTime = CAtumSJ::GetOverheatTime(pItemInfo->GetRealItemInfo(),&g_pShuttleChild->m_paramFactor);
				g_pShuttleChild->m_pSecondaryWeapon = 
					new CWeaponItemInfo(pItemInfo, &g_pShuttleChild->m_paramFactor,fOverHeatTime,FALSE);// 0,0:over heat
			}
			g_pShuttleChild->m_pSecondaryWeapon->DelTargetSecondary();	// 2006-09-08 by ispark, 2������ ������ ���� �߻�� 2�� ���� Ÿ�� NULL
			g_pShuttleChild->m_pSkill->PutSkillFromWearItem(g_pShuttleChild->m_pSecondaryWeapon->GetRealItemInfo()->Kind);
			memset(&g_pShuttleChild->m_SecondaryAttack, 0x00, sizeof(ATTACK_DATA));
			g_pShuttleChild->m_SecondaryAttack.nMultiNumCount = g_pShuttleChild->GetMultiNum( pItemInfo->GetRealItemInfo(), &g_pShuttleChild->m_paramFactor );
			g_pShuttleChild->SetWeaponData(1,TRUE);//,TRUE);
			g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_WINGOUT );
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_WEAR_WEAPON, g_pShuttleChild->m_vPos, FALSE);
		}
		else if(pItemInfo->Kind == ITEMKIND_SUPPORT)
		{
			DBGOUT("SUPPORT Change\n");
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_REAR, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_REAR, pItemInfo );
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			g_pShuttleChild->m_pSkill->PutSkillFromWearItem(ITEMKIND_SUPPORT);
			g_pShuttleChild->ChangeEngine(pItemInfo, pItemInfo->CurrentCount);
			g_pShuttleChild->SetShuttleSpeed();
		}
		else if(pItemInfo->Kind == ITEMKIND_DEFENSE)
		{
			DBGOUT("DEFENSE Change\n");
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_CENTER, pItemInfo->GetRealItemInfo() 
			g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_CENTER, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			g_pShuttleChild->m_pSkill->PutSkillFromWearItem(ITEMKIND_DEFENSE);


			// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
// 			g_pShuttleChild->ChangeUnitCharacterInfo(pItemInfo->ItemInfo->SourceIndex, 
// 				g_pShuttleChild->m_myShuttleInfo.UnitKind, g_pShuttleChild->GetCurrentBodyCondition(), FALSE);

			int nDefenseNum = 0;
#ifdef _REWORKED_COLORSHOP
			if (pItemInfo->ColorCode)
			{
				nDefenseNum = pItemInfo->ColorCode;
			}
			else
			{
#endif
				if (!pItemInfo->ShapeItemNum)
					nDefenseNum = pItemInfo->ItemInfo->SourceIndex;
				else
				{
					ITEM* pShapeItem = g_pDatabase->GetServerItemInfo(pItemInfo->ShapeItemNum);
					if (pShapeItem)
						nDefenseNum = pShapeItem->SourceIndex;
					else
						nDefenseNum = pItemInfo->ItemInfo->SourceIndex;
				}
#ifdef _REWORKED_COLORSHOP
			}
#endif
			// 2010. 03. 18 by jskim ���ͺ��� ī��
// 			g_pShuttleChild->ChangeUnitCharacterInfo( nDefenseNum,
// 													  g_pShuttleChild->m_myShuttleInfo.UnitKind,
// 													  g_pShuttleChild->GetCurrentBodyCondition(),
// 													  FALSE );
			BOOL bMonTras = FALSE;
			if(g_pShuttleChild->GetMonsterTransformer())
			{	
				g_pShuttleChild->ChangeUnitCharacterInfo( nDefenseNum,
														g_pShuttleChild->m_myShuttleInfo.UnitKind,
														g_pShuttleChild->GetCurrentBodyCondition(),
														FALSE);
				g_pShuttleChild->SetMonsterTransPrimarySidePos( g_pShuttleChild->m_PrimaryAttack.vSidePos);
				g_pShuttleChild->SetMonsterTransSecondarySidePos( g_pShuttleChild->m_SecondaryAttack.vSidePos );

				bMonTras = TRUE;
				MEX_MONSTER_INFO * pTempIn = g_pGameMain->CheckMonsterInfo(g_pShuttleChild->GetMonsterTransformer());
				if(pTempIn)
					nDefenseNum = pTempIn->SourceIndex;		
			}
			g_pShuttleChild->ChangeUnitCharacterInfo( nDefenseNum,
														g_pShuttleChild->m_myShuttleInfo.UnitKind,
														g_pShuttleChild->GetCurrentBodyCondition(),
														FALSE, bMonTras );
			//end 2010. 03. 18 by jskim ���ͺ��� ī��
			// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

			// 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
			//g_pShuttleChild->SetAmorHeight(pItemInfo->ItemInfo->SourceIndex);
			g_pShuttleChild->SetAmorHeight(nDefenseNum );
			// end 2013-01-08 by jhjang �κ��Ƹӿ� ���� ���� Ŷ�� ��������� ������ �߻��ϴ� �κ� ����
			
			int nColorCode = 0;
			if(pItemInfo->ColorCode == 0)	nColorCode = pItemInfo->ItemNum;
			else nColorCode = pItemInfo->ColorCode;

			// 2006-02-17 by ispark
//			g_pD3dApp->SetUnitArmorColorNum(max(1, nColorCode%100));
			g_pShuttleChild->SetAmorColor(max(1, nColorCode%100));
			if(bWearItem == FALSE)
			{
				// �������� �������� �ٲپ������
				if(g_pGameMain)
				{
					g_pGameMain->ResetWearItem(POS_CENTER);
				}
			}



			// 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)

			// �Ƹ� �����
			// ���� ����Ʈ ������ ���ϵ��� ����
			g_pShuttleChild->ShowWeapon( FALSE );
			// A��� ���뵵 �����
			g_pShuttleChild->DeleteRobotAGearWeapon();
			
			// �κ� �ƸӰ� �ƴҰ�� �ٽ� ������ �ϵ��� ����
			// 2010. 03. 18 by jskim ���ͺ��� ī��
 			if( !g_pShuttleChild->IsRobotArmor() )
 			{
				// 2010. 03. 18 by jskim ���ͺ��� ī��
				//g_pShuttleChild->ShowWeapon( TRUE );
				if(g_pShuttleChild->GetMonsterTransformer())
					g_pShuttleChild->ShowWeapon( FALSE );
				else
					g_pShuttleChild->ShowWeapon( TRUE );
				//end 2010. 03. 18 by jskim ���ͺ��� ī��
 			}			
			else
			{	
				// �κ� �Ƹ��� ���
				int		type = 0;
				ITEM*	pPrimaryWeapon = NULL;
				if(g_pShuttleChild->m_dwPartType == _SHUTTLE)
				{
					type = GetUnitTypeDecimal(g_pShuttleChild->m_myShuttleInfo.UnitKind);
					CMapItemWindowInventoryIterator it = g_pStoreData->m_mapItemWindowPosition.find( POS_PROWOUT );
					if( it != g_pStoreData->m_mapItemWindowPosition.end() )
						pPrimaryWeapon = it->second->ItemInfo;
				}

				// A����̰�, �ֹ��⸦ �����Ͽ��� ��� A���� �ӽ� ���� ����Ʈ �߰�
				if( type == 2 && pPrimaryWeapon )
				{
					ITEM* pAGearWeapon = g_pDatabase->GetServerItemInfo( A_GEAR_ROBOT_PRIMARY_WEAPON_ITEM_NUM );
 					if( pAGearWeapon )
						g_pShuttleChild->CreateRobotAGearWeapon( pAGearWeapon, type );
				}
			}
			// ���â�� �޽��� �ʱ�ȭ���� ������, ���� ����Ʈ�� ���ļ� ���δ�
			g_pShuttleChild->InitInvenMesh();
			// end 2009. 07. 07 by ckPark �κ���� ��û����(�Ѹ�, ����ȭ��, ����, A�������)



			// 2007-05-28 by bhsohn �����ۿ� ���� üũ�� �߰�			
			// �������� ã�Ҵ�.
			SendItemCheckSum(pItemInfo->Wear, pItemInfo->Kind, pItemInfo->ItemInfo->ItemNum);			
	
		}
		else if(pItemInfo->Kind == ITEMKIND_RADAR)
		{
			DBGOUT("RADAR Change\n");
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_PROW);
			g_pShuttleChild->CreateWearItem(WEAR_ITEM_KIND_PROW, EFFECT_NUMBER_WEAR_RADAR(pItemInfo->ItemInfo->SourceIndex) );
			g_pShuttleChild->m_pRadarItemInfo = pItemInfo;				// 2005-08-16 by ispark ����
			
			// 2008-09-26 by bhsohn �ű� ��þƮ ó��
			g_pShuttleChild->SetRadarRange(g_pShuttleChild->m_pRadarItemInfo);
			// end 2008-09-26 by bhsohn �ű� ��þƮ ó��
		}
		// 2006-04-14 by ispark, �߰�
		// 2006-06-28 by ispark, ����Ʈ �߰�
		else if(pItemInfo->Kind == ITEMKIND_ACCESSORY_UNLIMITED)
		{
//			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_ATTACHMENT, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED);
			g_pShuttleChild->CreateWearItem(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED, pItemInfo->ItemInfo->SourceIndex, g_pD3dApp->m_bCharacter);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_ATTACHMENT, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_ACCESSORY_UNLIMITED, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
		}
		else if(pItemInfo->Kind == ITEMKIND_ACCESSORY_TIMELIMIT)
		{
//			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_PET, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT);
			g_pShuttleChild->CreateWearItem(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT, pItemInfo->ItemInfo->SourceIndex, g_pD3dApp->m_bCharacter);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ��������
			//g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_PET, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_ACCESSORY_TIME_LIMIT, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
		}
		else if(pItemInfo->Kind == ITEMKIND_MARK)
		{
//			g_pShuttleChild->SetWearItemToCharacterParamFactor( POS_PET, pItemInfo->GetRealItemInfo() );
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_WINGIN);
			g_pShuttleChild->CreateWearItem(WEAR_ITEM_KIND_WINGIN, pItemInfo->ItemInfo->SourceIndex, g_pD3dApp->m_bCharacter);
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			//g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_WINGIN, pItemInfo );
			g_pShuttleChild->ChangeWearItemToCharacterParamFactor( POS_WINGIN, pItemInfo );
			//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			// 2011-03-21 by jhAhn ��ũ�ý��� ��ũ ��ġ ����
			g_pShuttleChild->SetMarkPosition();
			//end 2011-03-21 by jhAhn ��ũ�ý��� ��ũ ��ġ ����
		}
		else if ( pItemInfo->Kind == ITEMKIND_PET_ITEM )		// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ������ ���� ���ҽ� �ε�.
		{
			g_pShuttleChild->DeleteWearItem ( WEAR_ITEM_KIND_PET );
			g_pShuttleChild->CreateWearItem ( WEAR_ITEM_KIND_PET , g_pShuttleChild->GetPetManager()->GetSourceIndex_PetCurrentData( pItemInfo->UniqueNumber , (g_pD3dApp->m_bCharacter == TRUE ? true : false ) ) , g_pD3dApp->m_bCharacter );
			g_pShuttleChild->ChangeWearItemToCharacterParamFactor ( POS_PET , pItemInfo );
			g_pShuttleChild->GetPetManager()->SetSelectSocket(FALSE);  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

			// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
			CWeaponItemInfo* tempPetWeapon = new CWeaponItemInfo( pItemInfo, &g_pShuttleChild->m_paramFactor);
			g_pShuttleChild->GetPetManager()->SetWeaponItemInfo( tempPetWeapon );
			memset(&g_pShuttleChild->m_PetAttack, 0x00, sizeof(ATTACK_DATA));
			g_pShuttleChild->SetPetPosition();
			// end 2010-06-15 by shcho&hslee ��ý��� - �� ���� ����
			// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
			g_pCharacterChild->SetPetWearItem( TRUE );
			g_pCharacterChild->SetPetAddSpeed( FALSE );
			g_pCharacterChild->SetCharacterAddSpeed( 0.0f );
			// end 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
		}

		// 2010. 02. 11 by ckPark �ߵ��� ����������
		// �����Ϸ��� �������� �ߵ��� �����Ķ��� ������������ ��Ÿ�� ����Ʈ�� �ִ´�
		if( (pItemInfo->ItemInfo->InvokingDestParamID || pItemInfo->ItemInfo->InvokingDestParamIDByUse)
			&& (pItemInfo->ItemInfo->ReAttacktime > 0) )
			g_pGameMain->PushInvokeWearItem( pItemInfo->UniqueNumber );
		// end 2010. 02. 11 by ckPark �ߵ��� ����������
	}
	else // if(pItemInfo->Wear == WEAR_NOT_ATTACHED)
	{
		// 2010-06-15 by shcho&hslee ��ý��� - ���� ���� ó�� �ڵ� ����.
		switch ( pItemInfo->Kind )
		{
			case ITEMKIND_RADAR :

				g_pShuttleChild->DeleteWearItem ( WEAR_ITEM_KIND_PROW );
				g_pShuttleChild->m_pRadarItemInfo = NULL;
				break;
			
			case ITEMKIND_ACCESSORY_UNLIMITED :
			case ITEMKIND_ACCESSORY_TIMELIMIT :
			case ITEMKIND_MARK :
			case ITEMKIND_PET_ITEM :
				{
					short shWearItemKind = 0;
					short shEqPos = 0;

					switch ( pItemInfo->Kind )
					{
						case ITEMKIND_ACCESSORY_UNLIMITED :	
							
							shWearItemKind = WEAR_ITEM_KIND_ACCESSORY_UNLIMITED; 
							shEqPos = POS_ACCESSORY_UNLIMITED;
							break;

						case ITEMKIND_ACCESSORY_TIMELIMIT :	

							shWearItemKind = WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT; 
							shEqPos = POS_ACCESSORY_TIME_LIMIT;
							break;

						case ITEMKIND_MARK :

							shWearItemKind = WEAR_ITEM_KIND_WINGIN; 
							shEqPos = POS_WINGIN;
							break;
							
						case ITEMKIND_PET_ITEM :							
							// 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
							g_pCharacterChild->SetPetWearItem( FALSE );
							// end 2010-11-29 by jskim, �� ������ �̵��ӵ� ����
							shWearItemKind = WEAR_ITEM_KIND_PET;
							shEqPos = POS_PET;

							break;
					}
 
					if ( shWearItemKind != 0 && shEqPos != 0 )
					{
						// 2011. 01. 07 by jskim �Ǽ��縮 ������ ������ �� ���� �ȵǴ� ���� ����
						if(g_pCharacterChild->GetPetWearItem() == FALSE )
						{
							// 2010. 12. 02 by jskim �� ���� ������ ������ ���� ����
							g_pShuttleChild->GetPetManager()->SetWeaponItemInfo ( NULL );
							// end 2010. 12. 02 by jskim �� ���� ������ ������ ���� ����
						}
						// end 2011. 01. 07 by jskim �Ǽ��縮 ������ ������ �� ���� �ȵǴ� ���� ����
						g_pShuttleChild->DeleteWearItem ( shWearItemKind );
						g_pShuttleChild->ReleaseWearItemToCharacterParamFactor ( shEqPos );
					}

				}
				break;
		}

		/*if(pItemInfo->Kind == ITEMKIND_RADAR)
		{
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_PROW);
			g_pShuttleChild->m_pRadarItemInfo = NULL;					// 2005-08-16 by ispark ����
		}
		// 2006-04-14 by ispark, �߰�
		else if(pItemInfo->Kind == ITEMKIND_ACCESSORY_UNLIMITED)
		{
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_ACCESSORY_UNLIMITED);
			g_pShuttleChild->ReleaseWearItemToCharacterParamFactor( POS_ACCESSORY_UNLIMITED );
		}
		else if(pItemInfo->Kind == ITEMKIND_ACCESSORY_TIMELIMIT)
		{
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_ACCESSORY_TIME_LIMIT);
			g_pShuttleChild->ReleaseWearItemToCharacterParamFactor( POS_ACCESSORY_TIME_LIMIT );
		}
		else if(pItemInfo->Kind == ITEMKIND_MARK)
		{
			g_pShuttleChild->DeleteWearItem(WEAR_ITEM_KIND_WINGIN);
			g_pShuttleChild->ReleaseWearItemToCharacterParamFactor( POS_WINGIN );
		}
		else if ( pItemInfo->Kind == ITEMKIND_PET_ITEM )	// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ������ ���� ���ҽ� ����.
		{
			g_pShuttleChild->DeleteWearItem ( WEAR_ITEM_KIND_PET );
			g_pShuttleChild->ReleaseWearItemToCharacterParamFactor ( POS_PET );
		}*/
		// End 2010-06-15 by shcho&hslee ��ý��� - ���� ���� ó�� �ڵ� ����.

		// 2010. 02. 11 by ckPark �ߵ��� ����������
		// ������ �������� �ߵ��� �����Ķ��� �ִٸ� ��Ÿ�ӿ��� ����
		if( pItemInfo->ItemInfo->InvokingDestParamID || pItemInfo->ItemInfo->InvokingDestParamIDByUse )
			g_pGameMain->PopInvokeWearItem( pItemInfo->UniqueNumber );
		// end 2010. 02. 11 by ckPark �ߵ��� ����������
	}

	// 2010. 03. 25 by jskim Speed, Angle �߰� ���
	g_pShuttleChild->RecalculateSpeedAndAngle();
	g_pShuttleChild->SetShuttleSpeed();
	//end 2010. 03. 25 by jskim Speed, Angle �߰� ���

	// ��������� ���
	if(bWearItem == FALSE && g_pD3dApp->m_dwGameState == _SHOP)
	{
		g_pGameMain->BackUpWearItem();
	}
}

void CStoreData::SendFieldSocketItemIndexInfo(void)
{
	FLOG("CStoreData::SendFieldSocketItemIndexInfo(void)");
	// ���� �ִ� ������ ������ ������ �����ϰ� �����.
	int itemcount = 0;
	MSG_FC_ITEM_UPDATE_WINDOW_ITEM_LIST sMsg;
	memset(&sMsg,0x00,sizeof(sMsg));
	char buffer[SIZE_MAX_PACKET];
	int nType = T_FC_ITEM_UPDATE_WINDOW_ITEM_LIST;
	memcpy(buffer, &nType, SIZE_FIELD_TYPE_HEADER);
	CMapItemWindowInventoryIterator it = m_mapItemWindowPosition.begin();
	while(it != m_mapItemWindowPosition.end())
	{
		if( SIZE_FIELD_TYPE_HEADER+
			sizeof(MSG_FC_ITEM_UPDATE_WINDOW_ITEM_LIST)+
			(itemcount+1)*sizeof(ITEM_WINDOW_INFO)  
			>  SIZE_MAX_PACKET)
		{
			sMsg.NumOfItems = itemcount;
			memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
			g_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg) + itemcount*sizeof(ITEM_WINDOW_INFO));
			itemcount = 0;
		}
		ITEM_WINDOW_INFO mes;
		mes.ItemUniqueNumber = it->second->UniqueNumber;
		mes.ItemKind = it->second->Kind;
		mes.Wear = it->second->Wear;
		mes.ItemWindowIndex = it->second->ItemWindowIndex;
		memcpy(buffer+SIZE_FIELD_TYPE_HEADER+sizeof(MSG_FC_ITEM_UPDATE_WINDOW_ITEM_LIST)+itemcount*sizeof(ITEM_WINDOW_INFO),
			&mes, 
			sizeof(ITEM_WINDOW_INFO));
		itemcount++;
		it++;
	}
	sMsg.NumOfItems = itemcount;
	memcpy(buffer+SIZE_FIELD_TYPE_HEADER, &sMsg, sizeof(sMsg));
	g_pD3dApp->m_pFieldWinSocket->Write(buffer, SIZE_FIELD_TYPE_HEADER + sizeof(sMsg) + itemcount*sizeof(ITEM_WINDOW_INFO));
}


//BOOL CStoreData::PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg)
//{
//	FLOG( "CStoreData::PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg)" );
//	CMapItemInventoryIterator it = m_mapItemUniqueNumber.find(pMsg->Enchant.TargetItemUniqueNumber);
//	if(it != m_mapItemUniqueNumber.end())
//	{
//		it->second->PutEnchant( pMsg->Enchant.DestParameter, pMsg->Enchant.ParameterValue );
//		return TRUE;
//	}
//	return FALSE;
//}
BOOL CStoreData::PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg)
{
	FLOG( "CStoreData::PutEnchant(MSG_FC_ITEM_PUT_ENCHANT* pMsg)" );
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.find(pMsg->Enchant.TargetItemUniqueNumber);
	if(it != m_mapItemUniqueNumber.end())
	{
		it->second->AddEnchantItem( pMsg->Enchant.EnchantItemNum );
//		ITEM* pEnchantITEM = g_pDatabase->GetServerItemInfo( pMsg->Enchant.EnchantItemNum );
//		if( pEnchantITEM )
//		{
//			if( pEnchantITEM->DestParameter1 != DES_NULL )
//			{
//				it->second->PutEnchant(pEnchantITEM->DestParameter1, pEnchantITEM->ParameterValue1);
//			}
//			if( pEnchantITEM->DestParameter2 != DES_NULL )
//			{
//				it->second->PutEnchant(pEnchantITEM->DestParameter2, pEnchantITEM->ParameterValue2);
//			}
//			if( pEnchantITEM->DestParameter3 != DES_NULL )
//			{
//				it->second->PutEnchant(pEnchantITEM->DestParameter3, pEnchantITEM->ParameterValue3);
//			}
//			if( pEnchantITEM->DestParameter4 != DES_NULL )
//			{
//				it->second->PutEnchant(pEnchantITEM->DestParameter4, pEnchantITEM->ParameterValue4);
//			}
//		}
		// 2006-01-24 by ispark, ��þƮ �� �ٽ� ���� ����.
		m_fWeight = GetTotalItemWeight();
		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
		// ���� ��þ���� ����
		if(g_pShuttleChild )
		{
			if(ITEMKIND_SUPPORT == it->second->GetRealItemInfo()->Kind)
			{
				g_pShuttleChild->UpdateEngineInfo();
			}			
			else if(ITEMKIND_RADAR == it->second->GetRealItemInfo()->Kind)
			{
				CItemInfo* pRadarKind = g_pStoreData->FindItemInInventoryByWindowPos(POS_PROW); 
				if(pRadarKind)
				{
					g_pShuttleChild->SetRadarRange(pRadarKind);
				}
			}
		}
		// end 2008-09-26 by bhsohn �ű� ��þƮ ó��
		
		return TRUE;
	}
	return FALSE;

}

BOOL CStoreData::PutRareInfo(MSG_FC_STORE_UPDATE_RARE_FIX* pMsg)
{
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.find(pMsg->ItemUID);
	if( it != m_mapItemUniqueNumber.end())
	{
		it->second->ChangeRareInfo(pMsg->PrefixCodeNum, pMsg->SuffixCodeNum);
		return TRUE;
	}
	return FALSE;
}

CItemInfo* CStoreData::FindItemInInventoryByWindowPos( int nWindowPos )
{
	CMapItemWindowInventoryIterator it = m_mapItemWindowPosition.find( nWindowPos );
	if(it != m_mapItemWindowPosition.end())
	{
		return it->second;
	}
	return NULL;
}

UINT CStoreData::GetCurrentCountItemInInventoryByItemNum( INT nItemNum )
{
	UINT nCount = 0;
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();
	while(it != m_mapItemUniqueNumber.end())
	{
		if(it->second->ItemNum == nItemNum)
		{
			nCount += it->second->CurrentCount;
		}
		it++;
	}
	return nCount;
}

CItemInfo* CStoreData::FindItemInInventoryByItemNum( INT nItemNum )
{
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();
	while(it != m_mapItemUniqueNumber.end())
	{
		if(it->second->ItemNum == nItemNum)
		{
			return it->second;
		}
		it++;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
/// \date		2013-04-08 ~ 2013-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::FindItemInInventoryByItemNumList( INT nItemNum, vector<CItemInfo*> *pvecItemList)
{
	pvecItemList->clear();

	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();
	while(it != m_mapItemUniqueNumber.end())
	{
		CItemInfo* pItemInfo = it->second;
		if(pItemInfo->ItemNum == nItemNum)
		{
			pvecItemList->push_back(pItemInfo);			
		}
		it++;
	}	
}

CItemInfo* CStoreData::FindItemInInventoryByUniqueNumber( UID64_t hyUniqueNumber )
{
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.find( hyUniqueNumber );
	if(it != m_mapItemUniqueNumber.end())
	{
		return it->second;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		ItemNum���� ��ü ������ ī���͸� ã��
/// \author		// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
/// \date		2013-03-18 ~ 2013-03-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
INT CStoreData::GetCntInven_ItemNum_To_AllItemCnt( INT nItemNum )
{
	INT nItemNumCnt = 0;
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();
	while(it != m_mapItemUniqueNumber.end())
	{
		CItemInfo* pItemInfo = it->second;
		if(pItemInfo && (pItemInfo->ItemNum == nItemNum))
		{
			if( IS_COUNTABLE_ITEM(pItemInfo->Kind) )
			{
				nItemNumCnt += pItemInfo->CurrentCount;				
			}
			else
			{
				nItemNumCnt += 1;							
			}
		}
		it++;
	}
	return nItemNumCnt;
}
// 2012-06-14 by isshin ���۹̸�����
CItemInfo* CStoreData::FindItemInEnemyByWindowPos( int nWindowPos )
{
	vector<CItemInfo*>::iterator iter = g_pD3dApp->m_vecWearDisplayInfo.begin();
	while(iter != g_pD3dApp->m_vecWearDisplayInfo.end())
	{
		if((*iter)->ItemWindowIndex == nWindowPos)
		{
			return (*iter);
		}
		iter++;
	}
	return NULL;
}

CItemInfo* CStoreData::FindItemInEnemyByUniqueNumber( UID64_t hyUniqueNumber )
{
	vector<CItemInfo*>::iterator iter = g_pD3dApp->m_vecWearDisplayInfo.begin();
	while(iter != g_pD3dApp->m_vecWearDisplayInfo.end())
	{
		if((*iter)->GetUniqueNumber() == hyUniqueNumber)
		{
			return (*iter);
		}
		iter++;
	}
	return NULL;
}
// end 2012-06-14 by isshin ���۹̸�����

// 2010. 02. 11 by ckPark �ߵ��� ����������
// ������ ���������� �������� ã�´�
CItemInfo*	CStoreData::FindItemInWearByItemNum( INT nItemNum )
{
	for( BYTE i=POS_PROW; i<=POS_PET; ++i )
	{
		CItemInfo* pRet = FindItemInInventoryByWindowPos( i );
		if( pRet && (pRet->ItemNum == nItemNum) )
			return pRet;
	}

	return NULL;
}
// end 2010. 02. 11 by ckPark �ߵ��� ����������


/********************************************************************************************************
**
**	�κ� ������ �˻�. ( ������ ���� ����. )
**
**	Create Info :	2010. 09. 08. by hsLee.
**
**	Base Info : 1. Index, ����, ����, ��æƮ�� ���� üũ.
**
**				2. ���� ������ �ϴ� ���� üũ�� �����մϴ�. (���߿� �ɼ� ���ڷ� ������ �ʿ䰡 ���� ���� ���Ͽ�...)
**
*********************************************************************************************************/
const CItemInfo* CStoreData :: FindItemInInventoryByItemInfo ( CItemInfo *pcItemInfo )
{
	
	if ( NULL == pcItemInfo )
		return NULL;

	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();

	while(it != m_mapItemUniqueNumber.end())
	{

		CItemInfo *pcInvenItemInfo = (CItemInfo *)it->second;

		if ( pcInvenItemInfo->ItemNum == pcItemInfo->GetItemInfo()->ItemNum )
		{
			if ( pcInvenItemInfo->PrefixCodeNum != pcItemInfo->PrefixCodeNum || 
				pcInvenItemInfo->SuffixCodeNum != pcItemInfo->SuffixCodeNum )
			{
				it++;
				continue;
			}

			if ( pcInvenItemInfo->GetEnchantNumber() > 0 )
			{
				it++;
				continue;
			}

			return it->second;
		}

		it++;
	}

	return NULL;

}


void CStoreData::TutorialNonWearItem(BYTE bPos)
{
	//CMapItemWindowInventoryIterator itSrc = m_mapItemWindowPosition.begin();
	CMapItemWindowInventoryIterator itSrc = m_mapItemWindowPosition.find(bPos);
	if(itSrc != m_mapItemWindowPosition.end())
	{
		CItemInfo* pItemInfoSrc = itSrc->second;
		//if(pItemInfoSrc->ItemWindowIndex == bPos)
		if(pItemInfoSrc)
		{
			itSrc = m_mapItemWindowPosition.erase(itSrc);
			int index = GetEmptySlotNumber();
			pItemInfoSrc->ItemWindowIndex = index;
			pItemInfoSrc->Wear = 0;
			m_mapItemWindowPosition[index] = pItemInfoSrc;

			UpdateWearItemData(pItemInfoSrc);
			//break;
		}
	}


}
void CStoreData::SetTutorialWearItemMap(BYTE bPos,int nItemNum)
{
	CItemInfo * pItemInfo = NULL;
	//CMapItemWindowInventoryIterator it = m_mapItemWindowPosition.find(bPos);

	ITEM *Item = g_pDatabase->GetServerItemInfo(nItemNum);
	ITEM_GENERAL stItem;
	memset(&stItem,0x00,sizeof(ITEM_GENERAL));
	stItem.ItemInfo = Item;
	stItem.ItemNum = Item->ItemNum;
	stItem.Kind = Item->Kind;
	pItemInfo = new CItemInfo(&stItem);
	pItemInfo->UniqueNumber = bPos;
	pItemInfo->ItemWindowIndex = bPos;
	pItemInfo->Wear = 1;
	pItemInfo->CurrentCount = IS_WEAPON(pItemInfo->Kind)?200:1;

	m_mapItemWindowPosition[bPos] = pItemInfo;
	m_mapItemUniqueNumber[bPos] = pItemInfo;

	UpdateWearItemData(pItemInfo);

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStoreData::SetTutorialWearItem(int nCenter,int nProw,int nRear,int nWingOut,int nProowOut)
/// \brief		������ ������ ���� ������ �ڽſ��� ������Ų��.
/// \author		dgwoo
/// \date		2007-06-29 ~ 2007-06-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::SetTutorialWearItem(int nCenter,int nProw,int nRear,int nWingOut,int nProwOut)
{
	SetTutorialWearItemMap(POS_CENTER,nCenter);
	SetTutorialWearItemMap(POS_PROW,nProw);
	SetTutorialWearItemMap(POS_REAR,nRear);
	SetTutorialWearItemMap(POS_WINGOUT,nWingOut);
	SetTutorialWearItemMap(POS_PROWOUT,nProwOut);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStoreData::TutorialBackUpItemAll()
/// \brief		map�� �����ϴ� ��� �������� �����Ѵ�.
/// \author		dgwoo
/// \date		2007-06-27 ~ 2007-06-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::TutorialBackUpItemAll()
{
	if(m_bTutorialBackUp)	
		return;
	DBGOUT("TutorialBackUpItemAll() mapItemUniqueNumber Size = %d, mapItemWindowPosion Size = %d\n"
		,m_mapItemUniqueNumber.size(),m_mapItemWindowPosition.size());
	//�ʱ�ȭ.
	m_mapItemUniqueNumberBackUp.clear();
	m_mapItemWindowPositionBackUp.clear();

	m_mapItemUniqueNumberBackUp = m_mapItemUniqueNumber;
	m_mapItemWindowPositionBackUp = m_mapItemWindowPosition;
	
	m_mapItemUniqueNumber.clear();
	m_mapItemWindowPosition.clear();

	//--------------------------------------------------------------------------//
	// 2007-06-29 by dgwoo Ʃ�丮���� ���� ������ ����.
	if(IS_DT(g_pShuttleChild->GetShuttleInfo()->UnitKind))
	{// ���̱��.
		SetTutorialWearItem(8012100,7011500,7002590,7900440,7000050);
	}else if(IS_BT(g_pShuttleChild->GetShuttleInfo()->UnitKind))
	{// ����
		SetTutorialWearItem(8010100,7000970,7001230,7900000,7001950);
	}else if(IS_OT(g_pShuttleChild->GetShuttleInfo()->UnitKind))
	{// �����.
		SetTutorialWearItem(8011100,7000970,7002570,7900000,7001950);
	}else if(IS_ST(g_pShuttleChild->GetShuttleInfo()->UnitKind))
	{// ���̱��.
		SetTutorialWearItem(8013100,7000970,7002580,7900000,7001950);
	}

	//--------------------------------------------------------------------------//
	

	g_pShuttleChild->ResortingItem();
	g_pShuttleChild->SetShuttleData(5);
	g_pGameMain->m_pInven->ReSetScrollCurrentLine();				// 2006-09-26 by ispark
	g_pGameMain->m_pInven->SetAllIconInfo();

	g_pGameMain->SetItemInfo(0, 0, 0, 0);							// 2006-10-27 by ispark
	m_bTutorialBackUp = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStoreData::TutorialRollBackItemAll()
/// \brief		����� ���������� �ٽ� ���� �ǵ�����.
/// \author		dgwoo
/// \date		2007-06-29 ~ 2007-06-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::TutorialRollBackItemAll()
{
	if(!m_bTutorialBackUp)
		return;
	DBGOUT("TutorialRollBackItemAll() mapItemUniqueNumber Size = %d, mapItemUniqueNumberBackUp Size = %d,mapItemWindowPosion Size = %d, mapItemWindowPosionBackUp Size = %d\n"
		,m_mapItemUniqueNumber.size(),m_mapItemWindowPosition.size()
		,m_mapItemUniqueNumberBackUp.size(),m_mapItemWindowPositionBackUp.size());
	
	m_mapItemWindowPosition.clear();
	m_mapItemUniqueNumber.clear();



	m_mapItemUniqueNumber = m_mapItemUniqueNumberBackUp;
	m_mapItemWindowPosition = m_mapItemWindowPositionBackUp;

	int i = 0;

	// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
	//for(i = 0 ; i < WEAR_ITEM_NUMBER ; i++)
	for(i = 0 ; i < MAX_EQUIP_POS; i++)
	{
		CMapItemWindowInventoryIterator it = m_mapItemWindowPosition.find(i);
		if(it->second != NULL)
		{// ������ �� �������̶��.
			UpdateWearItemData(it->second);
		}
	}
	m_bTutorialBackUp = FALSE;
	// 2007-07-24 by dgwoo Ʃ�丮�� ����߿� ���� �������� ���� ������ ���Ϳ� �� �����Ѵ�.
	vector<MSG_FC_TRADE_INSERT_ITEM>::iterator itInsert = g_pD3dApp->m_vecTutorialInsertItem.begin();
	while(itInsert != g_pD3dApp->m_vecTutorialInsertItem.end())
	{
		g_pD3dApp->FieldSocketTradeInsertItem(&*itInsert);
		itInsert++;
	}
	vector<MSG_FC_TRADE_UPDATE_ITEM_COUNT>::iterator itUpdate = g_pD3dApp->m_vecTutorialUpdateItem.begin();
	while(itUpdate != g_pD3dApp->m_vecTutorialUpdateItem.end())
	{
		g_pD3dApp->FieldSocketTradeUpdateItemCount(&*itUpdate);
		itUpdate++;
	}

	//--------------------------------------------------------------------------//
	
	
	m_mapItemUniqueNumberBackUp.clear();
	m_mapItemWindowPositionBackUp.clear();

	g_pShuttleChild->ResortingItem();
	g_pShuttleChild->SetShuttleData(5);
	g_pGameMain->m_pInven->ReSetScrollCurrentLine();				// 2006-09-26 by ispark
	g_pGameMain->m_pInven->SetAllIconInfo();

	g_pGameMain->SetItemInfo(0, 0, 0, 0);							// 2006-10-27 by ispark
	
}
void CStoreData::DeleteItem( UID64_t hyUniqueNumber )
{
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.find(hyUniqueNumber);
	if(it != m_mapItemUniqueNumber.end())
	{
		CItemInfo* pItemInfo = it->second;
		m_mapItemUniqueNumber.erase(it);
		CMapItemWindowInventoryIterator it2 = m_mapItemWindowPosition.find(pItemInfo->ItemWindowIndex);
		if(it2 != m_mapItemWindowPosition.end())
		{
			m_mapItemWindowPosition.erase(it2);
		}

// 2012-01-13 by jhahn EP4 ��Ʈ�� ������ â�� and ���丮 ��� ���� ����
		// 2010-06-15 by shcho&hslee ��ý��� - ������ ������ �� ���� ������ ���� �߰� ó��.
//		if ( pItemInfo->ItemInfo->Kind == ITEMKIND_PET_ITEM )
//			g_pShuttleChild->GetPetManager()->DeletePetCurrentData ( hyUniqueNumber );
//end 2012-01-13 by jhahn EP4 ��Ʈ�� ������ â�� and ���丮 ��� ���� ����

		util::del(pItemInfo);
	}
	g_pShuttleChild->ResortingItem();
	g_pShuttleChild->SetShuttleData(5);
	g_pGameMain->m_pInven->ReSetScrollCurrentLine();				// 2006-09-26 by ispark
	g_pGameMain->m_pInven->SetAllIconInfo();

	g_pGameMain->SetItemInfo(0, 0, 0, 0);							// 2006-10-27 by ispark

}
BOOL CStoreData::IsTutorialLesson6Success()
{
	BOOL bResult = TRUE;
	CMapItemWindowInventoryIterator it = m_mapItemWindowPosition.find(POS_PROWOUT);
	if(it == m_mapItemWindowPosition.end())
		bResult = FALSE;
	it = m_mapItemWindowPosition.find(POS_WINGOUT);
	if(it == m_mapItemWindowPosition.end())
		bResult = FALSE;
	return bResult;

}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStoreData::TutorialCompulsionItemSetting(CItemInfo * pItemInfo)
/// \brief		Ʃ�丮�� �ʿ��� ������ ���� ������ �ϱ� ���� �ҽ�.
/// \author		dgwoo
/// \date		2007-07-16 ~ 2007-07-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::TutorialCompulsionItemSetting(CItemInfo * pItemInfo)
{
	INT nWindowPostion;
	if(IS_PRIMARY_WEAPON(pItemInfo->Kind))
	{
		nWindowPostion = POS_PROWOUT;
		pItemInfo->ItemWindowIndex = nWindowPostion;

	}
	else
	{
		nWindowPostion = POS_WINGOUT;
		pItemInfo->ItemWindowIndex = nWindowPostion;
	}

	CMapItemWindowInventoryIterator itSrc = m_mapItemWindowPosition.begin();
	while(itSrc != m_mapItemWindowPosition.end())
	{
		CItemInfo* pItemInfoSrc = itSrc->second;
		if(pItemInfoSrc->UniqueNumber == pItemInfo->UniqueNumber)
		{
			m_mapItemWindowPosition.erase(itSrc);
			if(pItemInfo->UniqueNumber != 0)
			{
				CMapItemWindowInventoryIterator itDest = m_mapItemWindowPosition.begin();
				while(itDest != m_mapItemWindowPosition.end())
				{
					CItemInfo* pItemInfoDest = itDest->second;
					if(pItemInfoDest->UniqueNumber == pItemInfo->UniqueNumber)
					{
						pItemInfoDest->ItemWindowIndex = pItemInfo->ItemWindowIndex;
						pItemInfoDest->Wear = 1;
						m_mapItemWindowPosition.erase(itDest);
						m_mapItemWindowPosition[pItemInfo->ItemWindowIndex] = pItemInfo;

						break;
					}

					itDest++;
				}
			}

			CMapItemWindowInventoryIterator itTemp = m_mapItemWindowPosition.find(nWindowPostion);
			if(itTemp == m_mapItemWindowPosition.end())
			{
				pItemInfoSrc->ItemWindowIndex = pItemInfo->ItemWindowIndex;
				pItemInfoSrc->Wear = 1;
				m_mapItemWindowPosition[pItemInfo->ItemWindowIndex] = pItemInfoSrc;
			}
			else
			{
				int index = GetEmptySlotNumber();
				pItemInfoSrc->ItemWindowIndex = index;
				pItemInfoSrc->Wear = 1;
				m_mapItemWindowPosition[index] = pItemInfoSrc;
			}

			UpdateWearItemData(pItemInfoSrc);
			break;
		}

		itSrc++;
	}
//	g_pGameMain->m_pInven->SetAllIconInfo();
	g_pShuttleChild->ResortingItem();// ����	// 2005-09-27 by ispark ���� �ǽð����� ������ ���� ���� �ٲ�
	// 2006-07-26 by ispark, ���������� �̵��� ���� ���������� �̵�
	g_pGameMain->m_pInven->SetAllIconInfo();
	if(pItemInfo->ItemWindowIndex > POS_ITEMWINDOW_OFFSET)
	{
		g_pGameMain->m_pInven->SetScrollEndLine();
		g_pGameMain->m_pInven->SetAllIconInfo();
	}
}	
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CStoreData::ItemChangeWindowPosition(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK * pMsg)
/// \brief		������ ��ġ �ٲ�
/// \author		dhkwon
/// \date		2004-03-21 ~ 2004-03-21
/// \warning	1,2,3���� �������� ���� ShuttleChild�� ���� �ٲ�(���� ���� ���)
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::ItemChangeWindowPosition(MSG_FC_ITEM_CHANGE_WINDOW_POSITION_OK * pMsg)
{
//	CMapItemWindowInventoryIterator itSrc = m_mapItemWindowPosition.find(pMsg->ItemWindowIndexDest);
//	if(itSrc != m_mapItemWindowPosition.end())
//	{
//		CItemInfo* pItemInfoSrc = itSrc->second;
//		m_mapItemWindowPosition.erase(itSrc);
//		if(pMsg->UniqueNumberDest != 0)
//		{
//			CMapItemWindowInventoryIterator itDest = m_mapItemWindowPosition.find(pMsg->ItemWindowIndex);
//			CItemInfo* pItemInfoDest = itDest->second;
//			pItemInfoDest->ItemWindowIndex = pMsg->ItemWindowIndexDest;
//			pItemInfoDest->Wear = pMsg->WearDest;
//			m_mapItemWindowPosition.erase(itDest);
//			m_mapItemWindowPosition[pMsg->ItemWindowIndexDest] = pItemInfoDest;
//		}
//
//		CMapItemWindowInventoryIterator itTemp = m_mapItemWindowPosition.find(pMsg->ItemWindowIndex);
//		if(itTemp == m_mapItemWindowPosition.end())
//		{
//			pItemInfoSrc->ItemWindowIndex = pMsg->ItemWindowIndex;
//			pItemInfoSrc->Wear = pMsg->Wear;
//			m_mapItemWindowPosition[pMsg->ItemWindowIndex] = pItemInfoSrc;
//		}
//		else
//		{
//			int index = GetEmptySlotNumber();
//			pItemInfoSrc->ItemWindowIndex = index;
//			pItemInfoSrc->Wear = pMsg->Wear;
//			m_mapItemWindowPosition[index] = pItemInfoSrc;
//		}
//		UpdateWearItemData( pItemInfoSrc );
//	}
	// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	if(GetInfluenceMarkShow(myShuttleInfo.InfluenceType))
	{		
		if(POS_WINGIN == pMsg->ItemWindowIndex )
		{
			// ���� ������ �̵��� �������� �� ��ġ�� ��ũ�� ��ũ ������ �ʱ�ȭ
			// ��ũ ������ ������
			RestoreInfluenceMark();
		}
		else 
		{
			int nOldSrcPos = 0;
			CMapItemWindowInventoryIterator itTmpSrc = m_mapItemWindowPosition.begin();
			while(itTmpSrc != m_mapItemWindowPosition.end())
			{
				CItemInfo* pTmpItemInfoSrc = itTmpSrc->second;
				
				if(pTmpItemInfoSrc->UniqueNumber == pMsg->UniqueNumber)
				{
					nOldSrcPos = pTmpItemInfoSrc->ItemWindowIndex;
					break;
				}
				itTmpSrc++;
			}
			if(POS_WINGIN == nOldSrcPos)
			{
				RestoreInfluenceMark();
			}
			
		}
	}
	
	// end 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�

	// 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����
	// ���� ���� ��ȭ��
	Stat_t nChangedFuelStat	= 0;
	// end 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����
	
	// 2006-06-20 by ispark, ���������� ã�� ���� ����ũ �ѹ��� ã�� ���ο� ���
	CMapItemWindowInventoryIterator itSrc = m_mapItemWindowPosition.begin();
	while(itSrc != m_mapItemWindowPosition.end())
	{
		CItemInfo* pItemInfoSrc = itSrc->second;

		if(pItemInfoSrc->UniqueNumber == pMsg->UniqueNumber)
		{
			m_mapItemWindowPosition.erase(itSrc);
			if(pMsg->UniqueNumberDest != 0)
			{
				CMapItemWindowInventoryIterator itDest = m_mapItemWindowPosition.begin();
				while(itDest != m_mapItemWindowPosition.end())
				{
					CItemInfo* pItemInfoDest = itDest->second;
					if(pItemInfoDest->UniqueNumber == pMsg->UniqueNumberDest)
					{
						pItemInfoDest->ItemWindowIndex = pMsg->ItemWindowIndexDest;
						pItemInfoDest->Wear = pMsg->WearDest;
						m_mapItemWindowPosition.erase(itDest);
						m_mapItemWindowPosition[pMsg->ItemWindowIndexDest] = pItemInfoDest;

						// 2010. 02. 11 by ckPark �ߵ��� ����������
						// ������ �������� �ߵ��� �����Ķ��� �ִٸ� ��Ÿ�ӿ��� ����
						if( pItemInfoDest->ItemInfo->InvokingDestParamID || pItemInfoDest->ItemInfo->InvokingDestParamIDByUse )
							g_pGameMain->PopInvokeWearItem( pItemInfoDest->UniqueNumber );
						// end 2010. 02. 11 by ckPark �ߵ��� ����������

						// 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����
						for( int i=0; i<SIZE_MAX_DESPARAM_COUNT_IN_ITEM; ++i )
						{
							if( pItemInfoDest->ItemInfo->ArrDestParameter[ i ] == DES_FUEL_PART )
								nChangedFuelStat -= pItemInfoDest->ItemInfo->ArrParameterValue[ i ];
						}
						// end 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����

						break;
					}

					itDest++;
				}
			}

			CMapItemWindowInventoryIterator itTemp = m_mapItemWindowPosition.find(pMsg->ItemWindowIndex);
			if(itTemp == m_mapItemWindowPosition.end())
			{
				pItemInfoSrc->ItemWindowIndex = pMsg->ItemWindowIndex;
				pItemInfoSrc->Wear = pMsg->Wear;
				m_mapItemWindowPosition[pMsg->ItemWindowIndex] = pItemInfoSrc;
			}
			else
			{
				int index = GetEmptySlotNumber();
				pItemInfoSrc->ItemWindowIndex = index;
				pItemInfoSrc->Wear = pMsg->Wear;
				m_mapItemWindowPosition[index] = pItemInfoSrc;
			}
		
			UpdateWearItemData( pItemInfoSrc );
			// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
			if( pItemInfoSrc->Kind == ITEMKIND_PET_ITEM && pItemInfoSrc->Wear == WEAR_ATTACHED)
			{
				g_pShuttleChild->GetPetManager()->SetrenderOnOff(FALSE);   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
				g_pInterface->m_pINFOpMain->GetOpInfo()->SetOperatorAction(1,6);
			}
			// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
			g_pD3dApp->m_bRequestEnable = TRUE;			// 2006-06-19 by ispark

			// 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����
			for( int i=0; i<SIZE_MAX_DESPARAM_COUNT_IN_ITEM; ++i )
			{
				// �����Ķ��߿� ���� ������ ������
				if( pItemInfoSrc->ItemInfo->ArrDestParameter[ i ] == DES_FUEL_PART )
				{
					if( pMsg->Wear )
						// �����ߴٸ� ���
						nChangedFuelStat += pItemInfoSrc->ItemInfo->ArrParameterValue[ i ];
					else
						// �����ߴٸ� ����
						nChangedFuelStat -= pItemInfoSrc->ItemInfo->ArrParameterValue[ i ];
				}
			}
			// end 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����

			break;
		}

		itSrc++;
	}

	// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
	if((POS_WINGIN == pMsg->ItemWindowIndex) && (GetInfluenceMarkShow(myShuttleInfo.InfluenceType)))
	{
		// ���� ������ �̵��� �������� �� ��ġ�� ��ũ�� ��ũ ������ �ʱ�ȭ
		// ��ũ ������ ������
		UpdateInfluenceMark();
	}
	// end 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�

//	g_pGameMain->m_pInven->SetAllIconInfo();
	g_pShuttleChild->ResortingItem();// ����	// 2005-09-27 by ispark ���� �ǽð����� ������ ���� ���� �ٲ�
	// 2006-07-26 by ispark, ���������� �̵��� ���� ���������� �̵�
	g_pGameMain->m_pInven->SetAllIconInfo();
	if(pMsg->ItemWindowIndex > POS_ITEMWINDOW_OFFSET)
	{
		g_pGameMain->m_pInven->SetScrollEndLine();
		g_pGameMain->m_pInven->SetAllIconInfo();
	}	

	// 2010. 03. 11 by jskim �κ�â���� ��� ������ ������ ������ ������Ʈ �ȵǴ� ���� ����
	g_pGameMain->m_pInven->ReSetScrollCurrentLine();
	//end 2010. 03. 11 by jskim �κ�â���� ��� ������ ������ ������ ������Ʈ �ȵǴ� ���� ����

	// 2009-01-12 by bhsohn �Ϻ� �߰� ��������

	// 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����
	//CreateChatWeight();
	if( nChangedFuelStat )
		Create_FuelStatChanged_WeightOver_Message( nChangedFuelStat );
	else
		CreateChatWeight();
	// end 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����

	// end 2009-01-12 by bhsohn �Ϻ� �߰� ��������
}

void CStoreData::UpdateItemCount( UID64_t nUniqueNumber, INT nCount)
{
	CItemInfo* pItem = FindItemInInventoryByUniqueNumber( nUniqueNumber );
	if( pItem )
	{
		pItem->CurrentCount = nCount;
		if(nCount == 0 && IS_COUNTABLE_ITEM(pItem->GetItemInfo()->Kind))
		{
			DeleteItem( nUniqueNumber );
			//DeleteItemAndEnchant(pItem->UniqueNumber);
			return;
		}
		g_pShuttleChild->ResortingItem();
		g_pShuttleChild->SetShuttleData(5);
		g_pGameMain->m_pInven->SetAllIconInfo();
	}
	if( pItem == FindItemInInventoryByWindowPos(POS_PROWOUT) )
	{
		g_pShuttleChild->SetWeaponData(0,FALSE);//,TRUE);
	}
	else if( pItem == FindItemInInventoryByWindowPos(POS_WINGOUT) )
	{
		g_pShuttleChild->SetWeaponData(1,FALSE);//,TRUE);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
/// \date		2009-04-02 ~ 2009-04-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::UpdateCoinItemCount(UID64_t nUniqueNumber)
{
	GUI_BUILDINGNPC* pCurrentBuildingNPC = g_pInterface->m_pCityBase->GetCurrentBuildingNPC();
	if(NULL == pCurrentBuildingNPC)
	{
		return;
	}
	else if(BUILDINGKIND_LUCKY != pCurrentBuildingNPC->buildingInfo.BuildingKind)
	{
		return;
	}
	CMapCityShopIterator itLucky = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_LUCKY);
	if(itLucky == g_pInterface->m_pCityBase->m_mapCityShop.end())
	{		
		return;
	}			
	CINFLuckyMachine* pINFLuckyMachine = ((CINFLuckyMachine*)itLucky->second);
	if(NULL == pINFLuckyMachine)
	{
		return;
	}
	CItemInfo* pCoinItem = pINFLuckyMachine->GetConinItemInfo();
	if(NULL == pCoinItem)
	{
		return;
	}
	else if(pCoinItem->UniqueNumber != nUniqueNumber)
	{
		return;
	}

	CItemInfo* pInvenConiItem = FindItemInInventoryByUniqueNumber( pCoinItem->UniqueNumber);
	if(NULL == pInvenConiItem)
	{
		return;
	}
	if(NULL == IS_COUNTABLE_ITEM(pInvenConiItem->GetItemInfo()->Kind))
	{
		return;
	}
	
	int nCurrentCount = pInvenConiItem->CurrentCount - pCoinItem->CurrentCount;			
	if(nCurrentCount > 0 )
	{
		pInvenConiItem->CurrentCount = nCurrentCount;		
	}
	else
	{
		pINFLuckyMachine->ClearConinItemInfo(); // �����ʱ�ȭ
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CStoreData::UpdateUnitInfoByUpdateItemInfo( ITEM* pItem )
/// \brief		���� ������� �ʴ´�.
/// \author		dhkwon
/// \date		2004-08-03 ~ 2004-08-03
/// \warning	2015-05-28 Future, function never used => Commented out
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
//void CStoreData::UpdateUnitInfoByUpdateItemInfo( ITEM* pItem )
//{
//	// �߷��� ��ȭ���� ������ �ְ� �ȴٸ� ���� ���õǾ���Ѵ�.
//	if( g_pD3dApp->m_dwGameState == _CITY || 
//		g_pD3dApp->m_dwGameState == _SHOP)
//	{
//		if(g_pInterface->m_pCityBase->GetCurrentBuildingNPC())
//		{
//			int nBuildingKind = g_pInterface->m_pCityBase->GetCurrentBuildingNPC()->buildingInfo.BuildingKind;
//			CMapCityShopIterator it = g_pInterface->m_pCityBase->m_mapCityShop.find(nBuildingKind);
//			if(it != g_pInterface->m_pCityBase->m_mapCityShop.end())
//			{
//				((CINFCityShop*)it->second)->UpdateItem(pItem->ItemNum);
//			}
//			else
//			{
//				DBGOUT("ERROR : CStoreData::UpdateUnitInfoByUpdateItemInfo( ItemNum:%d )\n", pItem->ItemNum);
//			}
//		}
//	}
//	else if((g_pD3dApp->m_dwGameState == _GAME ||
//			g_pD3dApp->m_dwGameState == _CITY ||
//			g_pD3dApp->m_dwGameState == _SHOP) && 
//			g_pGameMain)
//	{
//		g_pGameMain->m_pQuickSlot->UpdateQick(pItem->ItemNum);
//	}
//
//}

void CStoreData::UpdateItemInfo( int nItemNum)
{
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();
	while(it != m_mapItemUniqueNumber.end())
	{
		CItemInfo* pItemInfo = it->second;
		if(pItemInfo->ItemNum == nItemNum)
		{
			pItemInfo->ResetRealItemInfo();
		}
		it++;
		
	}
	
}

void CStoreData::ResortingItemInInventory()
{
	int i = POS_ITEMWINDOW_OFFSET;
	CMapItemWindowInventory mapResort;
	CMapItemWindowInventoryIterator it = m_mapItemWindowPosition.begin();
//	DBGOUT("ResortingItemInInventory---------------------------Start\n");
	while(it != m_mapItemWindowPosition.end())
	{
		CItemInfo* pItem = it->second;
//		DBGOUT("ResortingItemInInventory (Postion: %4d,Name: %s Num : %d)\n",pItem->ItemWindowIndex,pItem->ItemInfo->ItemName,i);
		if(pItem->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET)
		{
			pItem->ItemWindowIndex = i;
			i++;
		}
		mapResort[pItem->ItemWindowIndex] = pItem;
		it++;
	}
//	DBGOUT("ResortingItemInInventory---------------------------End\n");
	m_mapItemWindowPosition.clear();
	m_mapItemWindowPosition = mapResort;

	m_fWeight = GetTotalItemWeight();
}

// 2013-04-30 by ssjung, �κ��丮 �ڵ� ���� �ȵǴ� ���� ����
void CStoreData::InventorySort(vector<CItemInfo *> item, ITEMSORT nNum)
{
	vector<CItemInfo *>::iterator it;
 	vector<CItemInfo *>::iterator it02;
 
 	for(it = m_vecInvenItem.begin(); it!=m_vecInvenItem.end(); ++it)
 	{
 		for(it02 = it; it02!=m_vecInvenItem.end(); ++it02)
 		{
			if(INCHANT_NUM_SORT == nNum)
			{
				if((*it)->GetEnchantNumber() >= (*it02)->GetEnchantNumber())
					iter_swap(it,it02);
			}
			else if(ITEM_NUM_SORT == nNum)
			{
				if((*it)->GetItemInfo()->ItemNum >= (*it02)->GetItemInfo()->ItemNum)
					iter_swap(it,it02);
			}
			else if(ITEM_KIND_SORT == nNum)
 			{
				if((*it)->GetItemInfo()->Kind >= (*it02)->GetItemInfo()->Kind)
					iter_swap(it,it02);
			}
			else{break;}
 		}
 	}
}
// end 2013-04-30 by ssjung, �κ��丮 �ڵ� ���� �ȵǴ� ���� ����

// 2012-04-12 by mspark, �κ��丮 �ڵ� ����
void CStoreData::ResortingItemInInventorySort()
{
	m_vecInvenItem.clear();
	CMapItemWindowInventoryIterator itor = m_mapItemWindowPosition.begin();
	while(itor != m_mapItemWindowPosition.end())
	{
		m_vecInvenItem.push_back(itor->second);
		itor++;
	} 
// 2013-04-30 by ssjung, �κ��丮 �ڵ� ���� �ȵǴ� ���� ����
	InventorySort(m_vecInvenItem,INCHANT_NUM_SORT);				// ��þƮ �ѹ� ���� INCHANT_NUM_SORT
	InventorySort(m_vecInvenItem,ITEM_NUM_SORT);				// ������ �ѹ� ���� ITEM_NUM_SORT
	InventorySort(m_vecInvenItem,ITEM_KIND_SORT);				// ������ ���� ���� ITEM_KIND_SORT
// end 2013-04-30 by ssjung, �κ��丮 �ڵ� ���� �ȵǴ� ���� ����

//	sort(m_vecInvenItem.begin(), m_vecInvenItem.end(), CompareEnchantNumber());
//	sort(m_vecInvenItem.begin(), m_vecInvenItem.end(), CompareItemNum());
//	sort(m_vecInvenItem.begin(), m_vecInvenItem.end(), CompareEnchantNumber());
//	sort(m_vecInvenItem.begin(), m_vecInvenItem.end(), CompareItemNum());
//	sort(m_vecInvenItem.begin(), m_vecInvenItem.end(), CompareKind());

	int i = POS_ITEMWINDOW_OFFSET;
	CMapItemWindowInventory mapResort;
	vector<CItemInfo *>::iterator it = m_vecInvenItem.begin();
	while(it != m_vecInvenItem.end())
	{
		CItemInfo* pItem = (*it);
		if(pItem->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET)
		{
			pItem->ItemWindowIndex = i;
			i++;
		}
		mapResort[pItem->ItemWindowIndex] = pItem;
		it++;
	}
	m_mapItemWindowPosition.clear();
	m_mapItemWindowPosition = mapResort;
	
	m_fWeight = GetTotalItemWeight();
}
// end 2012-04-12 by mspark, �κ��丮 �ڵ� ����

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM* CStoreData::GetItemInfoFromItemGenral(ITEM_GENERAL* pItem)
/// \brief		Store�� ����ִ� �������� ����(ITEM) �����͸� �����´�.
/// \author		dhkwon
/// \date		2004-05-18 ~ 2004-05-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM* CStoreData::GetItemInfoFromItemGeneral(ITEM_GENERAL* pItem)
{
	if(!pItem)
	{
		return NULL;
	}
	if(!pItem->ItemInfo)
	{
		pItem->ItemInfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
	}
	return pItem->ItemInfo;
}

float CStoreData::GetTotalItemWeight()
{
	float fWeight = 0;
//	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();
//	while(it != m_mapItemUniqueNumber.end())
//	{
//		ITEM* pITEM = it->second->GetRealItemInfo();
//		if(pITEM)
//		{	
//			if(IS_COUNTABLE_ITEM(pITEM->Kind))
//			{
//				fWeight += ((float)pITEM->Weight) * it->second->CurrentCount / pITEM->MinTradeQuantity;
//			}
//			else
//			{
//				fWeight += (float)pITEM->Weight;
//			}
//		}
//		it++;
//	}
	// 2005-09-27 by ispark
	// ���� �������� ���� �����۸� ����
	// 2006-06-20 by ispark, ���Ը� ���� �� ������ ���� ������ �ٲ۴�.
	if(g_pGameMain)
	{
		// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
		//for(int i = 0; i < WEAR_ITEM_NUMBER; i++)
		for(int i = 0; i < MAX_EQUIP_POS; i++)
		{
//			if(g_pGameMain->m_pInven->m_pWearDisplayInfo[i])
			CMapItemWindowInventoryIterator it = m_mapItemWindowPosition.find(i);
			if(it != m_mapItemWindowPosition.end())
			{
				CItemInfo* pItemInfoSrc = it->second;
				CMapItemInventoryIterator it = m_mapItemUniqueNumber.find(pItemInfoSrc->UniqueNumber);
				if(it != m_mapItemUniqueNumber.end())
				{
					// 2007-11-02 by dgwoo ���� 80%���� ���� �ν��Ͱ� ������ ����.
					CItemInfo* pItemInfo = (CItemInfo*)it->second;										
					if(pItemInfo->ItemInfo)
					{
						fWeight += CAtumSJ::GetItemWeight(pItemInfo->ItemInfo, pItemInfo->CurrentCount, pItemInfo->GetDesItemWeight());
					}					
				}
			}
		}
	}

	return fWeight;
}

BOOL CStoreData::IsWearItem(BYTE nReqItemKind)
{
	for(int i=0;i<9;i++)
	{
		CMapItemWindowInventoryIterator it = m_mapItemWindowPosition.find(i);
		if( it != m_mapItemWindowPosition.end() &&
			CAtumSJ::CheckReqItemKind(nReqItemKind,it->second->GetRealItemInfo()->Kind) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

CItemInfo* CStoreData::FindBulletItem( BYTE bCaliber )
{
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();
	while(it != m_mapItemUniqueNumber.end())
	{
		CItemInfo* pItemInfo = it->second;
		if( pItemInfo->Kind == ITEMKIND_BULLET &&
			pItemInfo->ItemInfo->Caliber == bCaliber )
		{
			return pItemInfo;
		}
		it++;
	}
	return NULL;
}

ITEM* CStoreData::FindBulletItemToDB(BYTE bCaliber)
{
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();
	while(it != m_mapItemUniqueNumber.end())
	{
		CItemInfo* pItemInfo = it->second;
		if(pItemInfo->ItemInfo->Caliber == bCaliber )
		{
			return g_pDatabase->GetServerItemInfo(pItemInfo->ItemInfo->LinkItem);
		}
		it++;
	}
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CStoreData::GetEmptySlotNumber()
/// \brief		�� ���� ��ȣ ��������
/// \author		ispark
/// \date		2005-11-14 ~ 2005-11-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CStoreData::GetEmptySlotNumber()
{
	int index = POS_ITEMWINDOW_OFFSET;

	CMapItemWindowInventoryIterator it = m_mapItemWindowPosition.begin();
	for(; it != m_mapItemWindowPosition.end(); it++)
	{
		CItemInfo* pItem = it->second;
		if(pItem->ItemWindowIndex >= POS_ITEMWINDOW_OFFSET)
		{
			index++;
		}
	}

	return index;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CStoreData::SerchRevivalUseItem()
/// \brief		Ȯ���� ���� ���� ��Ȱ ������ ã��
/// \author		ispark
/// \date		2006-04-10 ~ 2006-04-10
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemInfo* CStoreData::SerchRevivalUseItem()
{
	float fHighProbability = 0.0f;
	CItemInfo* pItemInfoTemp = NULL;
	CMapItemInventoryIterator it = m_mapItemUniqueNumber.begin();
	while(it != m_mapItemUniqueNumber.end())
	{
		ITEM* pRealItemInfo = it->second->GetRealItemInfo();

		// 2009-04-21 by bhsohn ������ DesParam�߰�
		// �ΰ� DestParameter1�� ã�´�.
// 		if(pRealItemInfo->DestParameter1 == DES_CASH_NORMAL_RESTORE ||
// 			pRealItemInfo->DestParameter1 == DES_CASH_SUPER_RESTORE)
// 		{
// 			// ���� ���� Ȯ���� �����Ѵ�.
// 			if(pRealItemInfo->ParameterValue1 > fHighProbability)
// 			{
// 				fHighProbability = pRealItemInfo->ParameterValue1;
// 				pItemInfoTemp = it->second;
// 			}
// 		}
		if(pRealItemInfo->ArrDestParameter[0] == DES_CASH_NORMAL_RESTORE ||
			pRealItemInfo->ArrDestParameter[0] == DES_CASH_SUPER_RESTORE)
		{
			// ���� ���� Ȯ���� �����Ѵ�.
			if(pRealItemInfo->ArrParameterValue[0] > fHighProbability)
			{
				fHighProbability = pRealItemInfo->ArrParameterValue[0];
				pItemInfoTemp = it->second;
			}
		}
		// end 2009-04-21 by bhsohn ������ DesParam�߰�
		it++;
	}
	return pItemInfoTemp;		
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CStoreData::SendItemCheckSum()
/// \brief		
/// \author		// 2007-05-28 by bhsohn �����ۿ� ���� üũ�� �߰�
/// \date		2007-05-28 ~ 2007-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::SendItemCheckSum(int nItemWear, int nItemKind, int nItemNum)
{
	// �ƸӸ� üũ���� �����ش�. 
	if(!( nItemWear == WEAR_ATTACHED )&& (  nItemKind == ITEMKIND_DEFENSE))
	{
		return;
	}
	
	char chFullMapPath[MAX_PATH], chMapIdx[64];		

	memset(chFullMapPath, 0x00, MAX_PATH);
	memset(chMapIdx, 0x00, 64);

	int nUnitNum = g_pShuttleChild->GetUnitNum();	
	
	wsprintf(chMapIdx, "%08d.obj", nUnitNum);
	strncpy(chFullMapPath, chMapIdx, MAX_PATH);
	g_pD3dApp->LoadPath( chFullMapPath, IDS_DIRECTORY_OBJECT, chMapIdx);			
	
	int nFileSize = 0;

	// 2009. 05. 29 by ckPark SHA256 üũ�� �ý���
// 	UINT uiCheckSum = 0;
// 	if(FALSE == g_pShuttleChild->GetCheckSum(&uiCheckSum, &nFileSize, chFullMapPath))

#ifdef CHECK_SUM_ON			
	BYTE checkSum[32];
	if(FALSE == g_pShuttleChild->GetCheckSum(checkSum, &nFileSize, chFullMapPath))
	// end 2009. 05. 29 by ckPark SHA256 üũ�� �ý���


	{
		DBGOUT("[WM_CLOSE][CheckSum error] StoreData.cpp, g_pShuttleChild->GetCheckSum(%s) ", chFullMapPath );
#ifdef C_CLIENT_FILE_LOG
		g_cCustomOptimizer.WriteLog("[WM_CLOSE][CheckSum error] StoreData.cpp"); // 2013-05-23 by bhsohn Ŭ���̾�Ʈ ���Ϸα�(LOG) �ý��� �߰�
#endif
		SendMessage( g_pD3dApp->GetHwnd(), WM_CLOSE, 0, 0 );
		return;
	}	

	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_CHECK_RESOBJ_CHECKSUM, T_FC_INFO_CHECK_RESOBJ_CHECKSUM, pSMsg, SendBuf);
	
	pSMsg->nItemNum0	= nItemNum;
	strncpy(pSMsg->szObjFileName, chMapIdx, SIZE_MAX_RESOBJ_FILE_NAME);

	// 2009. 05. 29 by ckPark SHA256 üũ�� �ý���
	//pSMsg->uiCheckSum0 = uiCheckSum;
	memcpy(pSMsg->byDigest, checkSum, sizeof(checkSum));
	// end 2009. 05. 29 by ckPark SHA256 üũ�� �ý���

	pSMsg->nFileSize0 = nFileSize;	

	g_pD3dApp->m_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_INFO_CHECK_RESOBJ_CHECKSUM));	
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		������ �����ϰ� �ִ� �������̳�?
/// \author		// 2007-06-12 by bhsohn �̼� �������̽� ������
/// \date		2007-06-12 ~ 2007-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CStoreData::IsShuttleWearItem(int nWearItem)
{
	CMapItemWindowInventoryIterator it;
	it = m_mapItemWindowPosition.find(nWearItem);
	if(it == m_mapItemWindowPosition.end())
	{
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ũ�� ������ ���� ��������
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::ChangeInfluenceMark(BYTE i_byInfluence)
{
	m_byMarkInfluence = i_byInfluence;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ũ�� ������ ���� ��������
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BYTE CStoreData::GetChangeInfluenceMark()
{
	return m_byMarkInfluence;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ũ�� ������ ����
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::UpdateInfluenceMark()
{	
	BYTE byMarkInfluence = GetChangeInfluenceMark();
	if(g_pGameMain && g_pGameMain->GetSummonMotherShipCnt() > 0)
	{
		// ���� �������� ������ ����

	}
	else if(0 == byMarkInfluence)
	{
		// ��ũ �ý��� ������
		RestoreInfluenceMark();	// ���� ��� ����		
		return;
	}		
	const auto& myShuttleInfo = g_pShuttleChild->GetMyShuttleInfo();
	//myShuttleInfo.InfluenceType
	INT nObjectNum = GetInfluenceMarkShow(myShuttleInfo.InfluenceType);	
	
	// �ڽ��� ��ü ���� ����
	if(FALSE != nObjectNum)
	{
		if(0 != m_struInfluenceWarBackupWaingInfo.nRealItemNum)
		{
			// ���� ������ ��ũ�� �ִٸ� ������� ������.
			RestoreInfluenceMark();
		}

		// ���� ���� �ִ� �������� �ٲ۴�.
		CMapItemWindowInventoryIterator itInven = m_mapItemWindowPosition.begin();		
		while(itInven != m_mapItemWindowPosition.end())
		{
			CItemInfo* pItem = itInven->second;			
			if(POS_WINGIN == pItem->ItemWindowIndex)
			{
				// ���� ���� ���?
				m_struInfluenceWarBackupWaingInfo.UniqueNumber = pItem->UniqueNumber;
				m_struInfluenceWarBackupWaingInfo.nRealItemNum = pItem->ItemNum;
				m_struInfluenceWarBackupWaingInfo.nInfluenceItemNum = nObjectNum;
				
				pItem->ChangeItemNum(nObjectNum);
				UpdateWearItemData(pItem);
				break;
			}			
			itInven++;
		}
	}
	else
	{
		if(0 != m_struInfluenceWarBackupWaingInfo.nRealItemNum)
		{
			// ���� ������ ��ũ�� �ִٸ� ������� ������.
			RestoreInfluenceMark();
		}
	}
	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�� ��ü ���� ��ũ�� ������ ����
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::UpdateEnemyInfluenceMark()
{
	CMapEnemyIterator itEnemy = g_pScene->m_mapEnemyList.begin();
	while(itEnemy != g_pScene->m_mapEnemyList.end())
	{
		CEnemyData *pEnemy = itEnemy->second;
		pEnemy->RefreshMarkItem();
		
		itEnemy++;
	}


}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ũ ���� �ʱ�ȭ
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::InitInfluenceMark()
{	
	memset(&m_struInfluenceWarBackupWaingInfo, 0x00, sizeof(struInfluenceWarBackupWaingInfo));
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		��ũ�� ������� �������´�.
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::RestoreInfluenceMark()
{	
	if(0 == m_struInfluenceWarBackupWaingInfo.nRealItemNum)
	{
		return;
	}

	CMapItemWindowInventoryIterator itInven = m_mapItemWindowPosition.begin();		
	
	while(itInven != m_mapItemWindowPosition.end())
	{
		CItemInfo* pItem = itInven->second;			
		if(m_struInfluenceWarBackupWaingInfo.UniqueNumber == pItem->UniqueNumber)
		{				
			pItem->ChangeItemNum(m_struInfluenceWarBackupWaingInfo.nRealItemNum);
			UpdateWearItemData(pItem);
			InitInfluenceMark();
			
			break;
		}			
		itInven++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ũ���� �ϰ� �ִ��� ���� �Ǵ�
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CStoreData::IsInfluenceMark()
{
	return m_bInfluenceMarkWar;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ��ũ�� ������ ���� ��������
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::SetInfluenceMarkInfo(BOOL bStart)
{
	m_bInfluenceMarkWar = bStart;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���¸�ũ �� ���� 
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CStoreData::GetInfluenceMarkShow(BYTE i_byInfluence)
{
	if(!g_pGameMain || !g_pStoreData)
	{
		return FALSE;
	}
	// ���� �ȶ������� 
	BYTE byInfluenceMark = g_pStoreData->GetChangeInfluenceMark();
	if(g_pGameMain && g_pGameMain->GetSummonMotherShipCnt() > 0)
	{
		// ���� �������� ������ ����
		byInfluenceMark = INFLUENCE_TYPE_ALL_MASK;
	}
	else
	{
		// ���� ����.
		byInfluenceMark = g_pStoreData->GetChangeInfluenceMark();
		if(0 == byInfluenceMark || !g_pStoreData->IsInfluenceMark())
		{
			// ��ũ ���� �ƴϴ�.
			return FALSE;
		}
	}
	
	if(INFLUENCE_TYPE_ALL_MASK == byInfluenceMark)
	{		
		if(COMPARE_INFLUENCE(i_byInfluence, INFLUENCE_TYPE_VCN))
		{
			return MARK_INFLUENCE_VCU_OBJECT;// ����������
		}
		return MARK_INFLUENCE_ANI_OBJECT;// �˸���		
	}
	if(COMPARE_INFLUENCE(byInfluenceMark, INFLUENCE_TYPE_VCN))
	{
		// ����������
		if(COMPARE_INFLUENCE(i_byInfluence, INFLUENCE_TYPE_VCN))
		{
			return MARK_INFLUENCE_VCU_OBJECT;// ����������
		}		
	}
	if(COMPARE_INFLUENCE(byInfluenceMark, INFLUENCE_TYPE_ANI))
	{
		//�˸��� 				
		if(COMPARE_INFLUENCE(i_byInfluence, INFLUENCE_TYPE_ANI))
		{
			return MARK_INFLUENCE_ANI_OBJECT;// �˸���
		}		
	}	
	return FALSE;

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-01-12 by bhsohn �Ϻ� �߰� ��������
/// \date		2009-01-12 ~ 2009-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::CreateChatWeight()
{
	if(NULL == g_pD3dApp
		|| NULL == g_pD3dApp->m_pChat)
	{
		return ;
	}
	float fWeightTmp = g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport(&g_pShuttleChild->m_myShuttleInfo);
	float fWeight = fWeightTmp;

	if(fWeightTmp >= 0.9f)
	{
		fWeight = 0.9f;
	}
	else if(fWeightTmp > WEIGHT_RATE_VERY_HEAVY)
	{
		fWeight = WEIGHT_RATE_VERY_HEAVY+0.01f;		
	}
	else if(fWeightTmp >= WEIGHT_RATE_HEAVY)
	{
		fWeight = WEIGHT_RATE_HEAVY+0.01f;		
	}	

	if(m_fGearChatWeight == fWeight)
	{
		return;
	}
	m_fGearChatWeight = fWeight;

	if(FALSE == m_bRqChangeItemPos)
	{
		return;
	}
	m_bRqChangeItemPos = FALSE;

	if(fWeight >= 0.9f)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0044,COLOR_SYSTEM);//"��� ���緮�� 90%�� �ʰ� �Ͽ����ϴ�."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0045,COLOR_SYSTEM);//"�ּ� �ӵ��θ� �̵������ϸ�, ����Ҹ��� �����մϴ�."
	}
	else if(fWeight > WEIGHT_RATE_VERY_HEAVY)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0046,COLOR_SYSTEM);//"��� ���緮�� 80%�� �ʰ� �Ͽ����ϴ�."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0047,COLOR_SYSTEM);//"�ν����� ����� ���ѵǸ�, ����Ҹ��� �����մϴ�."
	}
	else if(fWeight >= WEIGHT_RATE_HEAVY)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0048,COLOR_SYSTEM);//"��� ���緮�� 70%�� �ʰ��Ͽ� ����Ҹ��� �����մϴ�."
	}	

	
	
}

// 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����
void	CStoreData::Create_FuelStatChanged_WeightOver_Message( Stat_t nChangedFuelStat )
{
	if( !g_pD3dApp || !g_pD3dApp->m_pChat )
		return;

	CHARACTER characterinfo = g_pShuttleChild->m_myShuttleInfo;
	characterinfo.TotalGearStat.FuelPart += nChangedFuelStat;

	float fWeightTmp = g_pStoreData->GetTotalWeight() / CAtumSJ::GetTransport( &characterinfo );
	float fWeight = fWeightTmp;

	if(fWeightTmp >= 0.9f)
		fWeight = 0.9f;
	else if(fWeightTmp > WEIGHT_RATE_VERY_HEAVY)
		fWeight = WEIGHT_RATE_VERY_HEAVY+0.01f;		
	else if(fWeightTmp >= WEIGHT_RATE_HEAVY)
		fWeight = WEIGHT_RATE_HEAVY+0.01f;		

	if(m_fGearChatWeight == fWeight)
		return;

	m_fGearChatWeight = fWeight;

	m_bRqChangeItemPos = FALSE;

	if(fWeight >= 0.9f)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0044,COLOR_SYSTEM);//"��� ���緮�� 90%�� �ʰ� �Ͽ����ϴ�."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0045,COLOR_SYSTEM);//"�ּ� �ӵ��θ� �̵������ϸ�, ����Ҹ��� �����մϴ�."
	}
	else if(fWeight > WEIGHT_RATE_VERY_HEAVY)
	{
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0046,COLOR_SYSTEM);//"��� ���緮�� 80%�� �ʰ� �Ͽ����ϴ�."
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0047,COLOR_SYSTEM);//"�ν����� ����� ���ѵǸ�, ����Ҹ��� �����մϴ�."
	}
	else if(fWeight >= WEIGHT_RATE_HEAVY)
		g_pD3dApp->m_pChat->CreateChatChild(STRMSG_C_INTERFACE_0048,COLOR_SYSTEM);//"��� ���緮�� 70%�� �ʰ��Ͽ� ����Ҹ��� �����մϴ�."
}
// end 2010. 03. 04 by ckPark ������ ���ϴ� �������� �����Ͽ��� ��� ���� ��� ���� ����


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-01-12 by bhsohn �Ϻ� �߰� ��������
/// \date		2009-01-12 ~ 2009-01-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::RqChangeItemPos(BOOL i_bRqPos)
{
	m_bRqChangeItemPos = i_bRqPos;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���
/// \date		2013-05-28 ~ 2013-05-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CStoreData::AddArmorCollectInfo(ITEM* pInfo)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	m_pArmorCollectManager->AddArmorCollectInfo(pInfo);	// �Ƹ� �÷¼� ���� �ð�
#endif
}
struArmorCollectionInfo *CStoreData::GetArmorCollectInfo(INT nUnitCollectType, INT nItemNum)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	return m_pArmorCollectManager->GetArmorCollectInfo(nUnitCollectType, nItemNum);
#else
	return NULL;
#endif

}
// 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
struArmorCollectionInfo *CStoreData::GetArmorCollectInfo_SourceIndex(INT nUnitCollectType, INT nItemNum)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	return m_pArmorCollectManager->GetArmorCollectInfo_SourceIndex(nUnitCollectType, nItemNum);
#else
	return NULL;
#endif
}
// end 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��


void CStoreData::ReleaseArmorCollectInfo()
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	m_pArmorCollectManager->ReleaseArmorCollectInfo();	// �Ƹ� �÷¼� ���� �ð�
#endif
}

CVecArmorCollecInfo* CStoreData::GetVecArmorCollectInfo(INT nUnitCollectType)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	return m_pArmorCollectManager->GetVecArmorCollectInfo(nUnitCollectType);
#else
	return NULL;
#endif
}

void CStoreData::SetArmorCollectInfo(MSG_FC_COLLECTION_INFO* pMsg)	
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	m_pArmorCollectManager->SetArmorCollectInfo(pMsg);
#endif
}

INT CStoreData::Convert_UNITKIND_To_UnitCollectType(USHORT ushUnitKind)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	return m_pArmorCollectManager->Convert_UNITKIND_To_UnitCollectType(ushUnitKind);
#else
	return 0;
#endif
}


void CStoreData::RqCollectionShapeChange(ItemUID_t uTargetItemUID,// ���������� ������� ������UID
										ItemNum_t nShapeNum)			// ���� ��ȣ(�ҽ��ε���))
{	
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	m_pArmorCollectManager->RqCollectionShapeChange(uTargetItemUID, nShapeNum);
#endif
}

// 2013-06-14 by ssjung �Ƹ� �÷��� �ý��� �̴��� �Ƹ� 
void CStoreData::SetMontylyArmorCollectInit()
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	m_pArmorCollectManager->SetMontylyArmorCollectInit();
#endif
}
void CStoreData::SetMontylyArmorCollectInfo(MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* pMsg)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	m_pArmorCollectManager->SetMontylyArmorCollectInfo(pMsg);
#endif
}
MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO* CStoreData::GetMonthlyArmorCollectInfo(INT nUnitCollectType, INT nItemNum)
{
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	return m_pArmorCollectManager->GetMonthlyArmorCollectInfo(nUnitCollectType,nItemNum);
#else
	return NULL;
#endif
}
// end 2013-06-14 by ssjung �Ƹ� �÷��� �ý��� �̴��� �Ƹ� 
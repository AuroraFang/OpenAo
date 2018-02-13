// Petmanager.cpp: implementation of the CPetdata class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumDatabase.h"
#include "StoreData.h"		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#include "PetManager.h"
#include "WeaponItemInfo.h"
#include "AtumData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CAtumDatabase			*g_pDatabase;
extern CStoreData				*g_pStoreData;

CPetManager::CPetManager()
{
	DeleteAllPetCurrentData();
	InitAttackData();
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	m_nSelectSockt = NULL;
	m_nSocktNum = -1;
	m_nselecting_num = -1;

	OnOFf = FALSE;
	m_bRenderFlag = FALSE;			

	for (int i = 0 ; i < 3 ; i++)
	{
		m_nSerching[i] = FALSE;
	}
	m_bRenderOn = FALSE;
	
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	// 2012-10-31 by jhjang ������ �ʱ�ȭ �ڵ� �߰�
	m_pPetWeapon = NULL;
	// end 2012-10-31 by jhjang ������ �ʱ�ȭ �ڵ� �߰�
}

CPetManager::~CPetManager()
{
	DeleteAllPetCurrentData();
}
//������ �ֱ�
BOOL CPetManager::InsertPetCurrentData( tPET_CURRENTINFO* i_PetCurrentInfo)
{
	if(i_PetCurrentInfo == NULL)
	{ // ������ ������ ���� 
		return FALSE;
	}

	if ( GetPtr_PetCurrentData( i_PetCurrentInfo->CreatedPetUID ) )
	{
		UpdatePetCurrentData( i_PetCurrentInfo );
		return TRUE;
	}

	m_vectPetCurrentData.push_back( i_PetCurrentInfo );	

	// vecPetCurrentInfo::iterator itr = m_vectPetCurrentData.begin(); //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	return TRUE;
}

//������ ������Ʈ
BOOL CPetManager::UpdatePetCurrentData(tPET_CURRENTINFO* i_PetCurrentInfo)
{
	if(i_PetCurrentInfo == NULL)
	{ // ������ ������ ����
		return FALSE;
	}

	vecPetCurrentInfo::iterator itr = m_vectPetCurrentData.begin();
	for( ; itr != m_vectPetCurrentData.end() ; ++itr )
	{

		tPET_CURRENTINFO *pPetCurData = (tPET_CURRENTINFO*)(*itr);

		if ( pPetCurData->CreatedPetUID == i_PetCurrentInfo->CreatedPetUID )
		{
			memcpy( pPetCurData , i_PetCurrentInfo , sizeof( tPET_CURRENTINFO ) );
			break;
		}
	}

	return TRUE;
}

//������ ����
BOOL CPetManager::DeletePetCurrentData( UID64_t CreatedPetUID )
{
	for (auto itr = m_vectPetCurrentData.begin(); itr != m_vectPetCurrentData.end(); ++itr)
	{
		// �����Ͱ� ������ ��Ƽ��~
		if (*itr == nullptr) continue;

		if ((*itr)->CreatedPetUID != CreatedPetUID) continue;

		util::del(*itr);

		m_vectPetCurrentData.erase(itr); 

		break;
	}

	return TRUE;
}


/**************************************************************************
**
**	�� ���� ��� ����.
**
**	Create Info :	2010-06-15 by shcho&hslee
**
***************************************************************************/
BOOL CPetManager :: DeleteAllPetCurrentData ( void )
{
	for (auto ptr : m_vectPetCurrentData) delete ptr;

	m_vectPetCurrentData.clear(); 

	return true;
}


/**************************************************************************
**
**	�� ���� ������ ����.
**
**	Create Info :	2010-06-15 by shcho&hslee
**
***************************************************************************/
tPET_CURRENTINFO *CPetManager :: GetPtr_PetCurrentData ( UID64_t PetUID )
{

	vecPetCurrentInfo::iterator itr = m_vectPetCurrentData.begin();

	for (; itr != m_vectPetCurrentData.end(); ++itr )
	{

		if ( (*itr)->CreatedPetUID == PetUID )
			return (*itr);
	}

	return NULL;

}


/**************************************************************
**
**	���� ��� SourceIndex�� ����.
**
**	Create Info :	2010. 07. 09. by hsLee.
**
***************************************************************/
int CPetManager :: GetSourceIndex_PetCurrentData ( UID64_t PetUID , bool bCharacter )
{

	int iRtn_SrcIdx = 0;

	tPET_CURRENTINFO *psPetCurInfo = GetPtr_PetCurrentData( PetUID );

	if ( !psPetCurInfo )
		return iRtn_SrcIdx;

	tPET_LEVEL_DATA *psPetLevelData = g_pDatabase->GetPtr_PetLevelData ( psPetCurInfo->PetIndex , psPetCurInfo->PetLevel );
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	tPET_BASE_ALL_DATA *psPetAllData = g_pDatabase->GetPtr_PetAllDataByIndex( psPetCurInfo->PetIndex);

	CItemInfo* pItem = g_pStoreData->FindItemInInventoryByWindowPos( POS_PET );
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	if ( !psPetLevelData )
		return iRtn_SrcIdx;

	// 2012-11-29 by bhsohn ��Ʈ�� ���� ���� ��Ʈ�� ����
#ifdef	SC_PARTNER_SHAPE_CHANGE_HSKIM
	INT		nDstSourceNum = pItem->GetItemInfo()->SourceIndex;
	INT		nShapeItemNum = pItem->ShapeItemNum;	
	
	if( pItem->ShapeItemNum > 0)
	{
		ITEM* pShapeItem = g_pDatabase->GetServerItemInfo( pItem->ShapeItemNum );
		if( pShapeItem )
		{
			nDstSourceNum = pShapeItem->SourceIndex;
		}
	}	
#endif
	// END 2012-11-29 by bhsohn ��Ʈ�� ���� ���� ��Ʈ�� ����

	
    tPET_CURRENTINFO* tempCurrentInfo = GetPtr_PetCurrentData( PetUID);      //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	// 2012-11-29 by bhsohn ��Ʈ�� ���� ���� ��Ʈ�� ����
#ifdef	SC_PARTNER_SHAPE_CHANGE_HSKIM
// 	if ( bCharacter )
// 		iRtn_SrcIdx = pItem->GetItemInfo()->SourceIndex - 3100000;//psPetLevelData->CitySourceIndex;		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
// 	else
// 		iRtn_SrcIdx = pItem->GetItemInfo()->SourceIndex - 3200000;//psPetLevelData->FieldSourceIndex;		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if ( bCharacter )
	{
		iRtn_SrcIdx = nDstSourceNum - 3100000;//psPetLevelData->CitySourceIndex;		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	}
	else
	{
		iRtn_SrcIdx = nDstSourceNum - 3200000;//psPetLevelData->FieldSourceIndex;		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	}
#else
	if ( bCharacter )
		iRtn_SrcIdx = pItem->GetItemInfo()->SourceIndex - 3100000;//psPetLevelData->CitySourceIndex;		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	else
		iRtn_SrcIdx = pItem->GetItemInfo()->SourceIndex - 3200000;//psPetLevelData->FieldSourceIndex;		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#endif
	// END 2012-11-29 by bhsohn ��Ʈ�� ���� ���� ��Ʈ�� ����

	return iRtn_SrcIdx;
}

void CPetManager::InitAttackData( void )
 {
// 	if(m_pPetAttack)
// 	{
// 		util::del(m_pPetAttack);
// 	}
// 
// 	m_pPetAttack = new ATTACK_DATA;
}
// void CPetManager::SetPetBaseData(MSG_FC_ITEM_PET_BASEDATA i_baseData)
// void CPetManager::SetPetSkillData(MSG_FC_ITEM_PET_SKILLDATA i_skillData)
// void CPetManager::SetPetSocketData(MSG_FC_ITEM_PET_SOCKETDATA i_socketData)
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void	CPetManager::SetSelectingCheck(int serching ,BOOL flag)
{
	if(flag)
		m_nSerching[serching] = flag;
	else 
		m_nSerching[serching] = FALSE;
}

BOOL	CPetManager::GetSelectingCheck(int checking)
{
	if( m_nSerching[checking] == TRUE )
		return TRUE;
	else
		return FALSE;
}

void	CPetManager::SetSelectingCheckSocket(int serching ,BOOL flag)
{
	if(flag)
		m_nSerchingSocket[serching] = flag;
	else 
		m_nSerchingSocket[serching] = FALSE;
}

BOOL	CPetManager::GetSelectingCheckSocket(int checking)
{
	if( m_nSerchingSocket[checking] == TRUE )
		return TRUE;
	else
		return FALSE;
}

void CPetManager::SetWearPetSocketPosition(int wearposition)
{

	if(wearposition == POS_HIDDEN_ITEM)
	{
		m_bRenderOn = TRUE;
	}
	else
		m_bRenderOn = FALSE;

}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
// ItemInfo.cpp: implementation of the CItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemInfo.h"
#include "AtumDatabase.h"

extern CAtumDatabase * g_pDatabase;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemInfo::CItemInfo(ITEM_GENERAL* pItem)
{
	Kind = pItem->Kind;
	UniqueNumber = pItem->UniqueNumber;
	ItemNum = pItem->ItemNum;
	Wear = pItem->Wear;
	AccountUniqueNumber = pItem->AccountUniqueNumber;
	Possess = pItem->Possess;
	CurrentCount = pItem->CurrentCount;
	
	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	//ScarcityNumber = pItem->ScarcityNumber;
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	ItemWindowIndex = pItem->ItemWindowIndex;
	NumOfEnchants = pItem->NumOfEnchants;
	PrefixCodeNum = pItem->PrefixCodeNum;
	SuffixCodeNum = pItem->SuffixCodeNum;
	CurrentEndurance = pItem->CurrentEndurance;
	ColorCode = pItem->ColorCode;

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	ShapeItemNum	= pItem->ShapeItemNum;
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	UsingStartTime = pItem->UsingStartTime;
	CreatedTime = pItem->CreatedTime;
	ItemInfo = g_pDatabase->GetServerItemInfo(pItem->ItemNum);
	m_pRefPrefixRareInfo = NULL;
	m_pRefSuffixRareInfo = NULL;
	m_pEnchantParamFactor = NULL;
	m_pAllParamFactor = NULL;
	m_nEnchantNumber = 0;
	m_pItemInfo = NULL;
	m_fItemPassTime = (float)pItem->UsingTimeStamp;
	m_nReUsable = 0;
	m_bUse = TRUE;
	GetPrefixRareInfo();
	GetSuffixRareInfo();
	GetRealItemInfo();

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	CoolingTimeStamp = pItem->CoolingTimeStamp;
	CoolingStartTime = pItem->CoolingStartTime;
	// �⺻ ���� �ð�����
	SetCoolElapsedTime( pItem->CoolingTimeStamp * 1000 );
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	// 2010-06-15 by shcho&hslee ��ý��� - �ŷ� ����
	m_nPetLevel		= NULL;
	m_bReName		= FALSE;
	m_dPetExp		= NULL;
	// end 2010-06-15 by shcho&hslee ��ý��� - �ŷ� ����
	
	m_UniqueNumber = pItem->UniqueNumber;	// 2012-06-14 by isshin ���۹̸�����

	FixedTermShape = pItem->FixedTermShape;	 // 2012-10-13 by jhahn �Ⱓ�� ������ �߰� 

	m_bLimitTime = FALSE;			// 2013-05-09 by ssjung ���� ���� �Ⱓ���� ��� �޽��� ����
	
	// 2013-05-15 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ���� ��Ŷ ���� 
	nMonthlyOptionItemNum = pItem->nMonthlyOptionItemNum;
	atMonthlyEventEndDate = pItem->atMonthlyEventEndDate;
	// end 2013-05-15 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ���� ��Ŷ ���� 
}

CItemInfo::~CItemInfo()
{
//	util::del(m_pRefPrefixRareInfo);// AtumDatabase
//	util::del(m_pRefSuffixRareInfo);// AtumDatabase
	util::del(m_pEnchantParamFactor);
	util::del(m_pAllParamFactor);
	util::del(m_pItemInfo);
}

RARE_ITEM_INFO* CItemInfo::GetPrefixRareInfo() 
{ 
	if(PrefixCodeNum != 0 && m_pRefPrefixRareInfo == NULL)
	{
		m_pRefPrefixRareInfo = g_pDatabase->GetServerRareItemInfo(PrefixCodeNum);
		SetRareItemInfo( m_pRefPrefixRareInfo, PrefixCodeNum );
	}
	return m_pRefPrefixRareInfo; 
}

RARE_ITEM_INFO* CItemInfo::GetSuffixRareInfo() 
{ 
	if(SuffixCodeNum != 0 && m_pRefSuffixRareInfo == NULL)
	{
		m_pRefSuffixRareInfo = g_pDatabase->GetServerRareItemInfo(SuffixCodeNum);
		SetRareItemInfo( m_pRefSuffixRareInfo, SuffixCodeNum );
	}
	return m_pRefSuffixRareInfo; 
}
void CItemInfo::ChangeRareInfo(INT nPrefixCodeNum, INT nSuffixCodeNum)
{
	if(PrefixCodeNum != 0)
	{
		ReleaseRareItemInfo(m_pRefPrefixRareInfo, PrefixCodeNum);
	}
	if(SuffixCodeNum != 0)
	{
		ReleaseRareItemInfo(m_pRefSuffixRareInfo, SuffixCodeNum);
	}
	PrefixCodeNum = nPrefixCodeNum;
	m_pRefPrefixRareInfo = NULL;
	GetPrefixRareInfo();
	SuffixCodeNum = nSuffixCodeNum;
	m_pRefSuffixRareInfo = NULL;
	GetSuffixRareInfo();
	GetRealItemInfo();	
	// 2007-10-16 by bhsohn ��þ ������ ���� ����
	ResetRealItemInfo();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			void CItemInfo::SetRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex)
/// \brief		m_pPrefixRareInfo, m_pSuffixRareInfo�� ���� ���ÿ�
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	m_pAllParamFactor�� ������ ���� �����.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::SetRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex)
{
	if(pInfo)
	{
		if(m_pAllParamFactor == NULL)
		{
			m_pAllParamFactor = new CParamFactor;
			m_pAllParamFactor->Reset();
		}
		for(int i=0; i<SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if(pInfo->DesParameter[i] != DES_NULL)
			{
				SetParamFactor_DesParam( *m_pAllParamFactor, pInfo->DesParameter[i], 
							  pInfo->ParameterValue[i] );
			}
		}
	}
}

void CItemInfo::ReleaseRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex)
{
	if(pInfo)
	{
		ASSERT_ASSERT(m_pAllParamFactor != NULL);
		for(int i=0; i<SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if(pInfo->DesParameter[i] != DES_NULL)
			{
				ReleaseParamFactor_DesParam( *m_pAllParamFactor, pInfo->DesParameter[i], 
							  pInfo->ParameterValue[i] );
			}
		}
	}
}


void CItemInfo::AddEnchantItem(INT nEnchantItemNum)
{
	ITEM* pEnchantITEM = g_pDatabase->GetServerItemInfo( nEnchantItemNum );
	if( pEnchantITEM )
	{
		// 2009-04-21 by bhsohn ������ DesParam�߰�
// 		if( pEnchantITEM->DestParameter1 != DES_NULL )
// 		{
// 			PutEnchant(pEnchantITEM->DestParameter1, pEnchantITEM->ParameterValue1);
// 		}
// 		if( pEnchantITEM->DestParameter2 != DES_NULL )
// 		{
// 			PutEnchant(pEnchantITEM->DestParameter2, pEnchantITEM->ParameterValue2);
// 		}
// 		if( pEnchantITEM->DestParameter3 != DES_NULL )
// 		{
// 			PutEnchant(pEnchantITEM->DestParameter3, pEnchantITEM->ParameterValue3);
// 		}
// 		if( pEnchantITEM->DestParameter4 != DES_NULL )
// 		{
// 			PutEnchant(pEnchantITEM->DestParameter4, pEnchantITEM->ParameterValue4);
// 		}
		int nArrParamCnt = 0;
		for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			if( pEnchantITEM->ArrDestParameter[nArrParamCnt] != DES_NULL )
			{
				PutEnchant(pEnchantITEM->ArrDestParameter[nArrParamCnt], 
							pEnchantITEM->ArrParameterValue[nArrParamCnt]);

				// 2010. 02. 11 by ckPark �ߵ��� ����������
				PushDefEnchantParam( ENCHANT_PARAM( pEnchantITEM->ArrDestParameter[nArrParamCnt],
													pEnchantITEM->ArrParameterValue[nArrParamCnt] ) );
				// end 2010. 02. 11 by ckPark �ߵ��� ����������
			}
		}		
		// end 2009-04-21 by bhsohn ������ DesParam�߰�

		// 2010. 02. 11 by ckPark �ߵ��� ����������
		// �ߵ��� �����Ķ� ��æƮ
		if( pEnchantITEM->InvokingDestParamID )
		{
			// �ش� ID�� ��æƮ �����Ķ� ���� ��������
			CVectorInvokingWearItemDP vec;
			g_pDatabase->GetInvokingWearItemDPList( &vec, pEnchantITEM->InvokingDestParamID );

			// �����Ķ� ��æƮ ���� �߰�
			CVectorInvokingWearItemDPIt it = vec.begin();
			while( it != vec.end() )
			{
				PushInvokeEnchantParam( ENCHANT_PARAM( (*it)->InvokingDestParam, (*it)->InvokingDestParamValue ) );
				++it;
			}
		}

		// ���� �����Ķ� ��æƮ
		if( pEnchantITEM->InvokingDestParamIDByUse )
		{
			// �ش� ID�� �����Ķ� ��������
			CVectorInvokingWearItemDP vec;
			g_pDatabase->GetInvokingWearItemDPByUseList( &vec, pEnchantITEM->InvokingDestParamIDByUse );

			// �����Ķ� ��æƮ ���� �߰�
			CVectorInvokingWearItemDPIt it = vec.begin();
			while( it != vec.end() )
			{
				PushInvokeEnchantParam( ENCHANT_PARAM( (*it)->InvokingDestParam, (*it)->InvokingDestParamValue ) );
				++it;
			}
		}
		// end 2010. 02. 11 by ckPark �ߵ��� ����������
	}
	m_nEnchantNumber++;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemInfo::PutEnchant(BYTE nDestParameter, float fParamterValue)
/// \brief		��æƮ ������ ��� m_pEnchantParamFactor�� �ְ�, m_pAllParamFactor���� �ִ´�.
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	m_pEnchantParamFactor, m_pAllParamFactor�� ������ ���� �����.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::PutEnchant(BYTE nDestParameter, float fParameterValue)
{
//	m_nEnchantNumber++;
	if(m_pEnchantParamFactor == NULL)
	{
		m_pEnchantParamFactor = new CParamFactor;
		m_pEnchantParamFactor->Reset();
	}
	SetParamFactor_DesParam( *m_pEnchantParamFactor, nDestParameter, fParameterValue );
	if(m_pAllParamFactor == NULL)
	{
		m_pAllParamFactor = new CParamFactor;
		m_pAllParamFactor->Reset();
	}
	SetParamFactor_DesParam( *m_pAllParamFactor, nDestParameter, fParameterValue );
	ResetRealItemInfo();

//	���� ���۽� ������ �ڵ�(����)
//	if( Wear == WEAR_LOADED &&
//		IS_PRIMARY_WEAPON(Kind) &&
//		(nDestParameter == DES_SHOTNUM_01 || 
//		nDestParameter == DES_REATTACKTIME_01) )
//	{
//		g_pShuttleChild->FieldSocketCharacterSendParamfactorInRange( POS_PROWOUT );
//	}
}

void CItemInfo::DeleteEnchantInfo()
{
	m_nEnchantNumber = 0;
	
	// 2010. 02. 11 by ckPark �ߵ��� ����������
	ClearDefEnchantParam();
	ClearInvokeEnchantParam();
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	if(m_pEnchantParamFactor == NULL)
	{
		return;
	}
	if(m_pAllParamFactor == NULL)
	{
		m_pAllParamFactor = new CParamFactor;
		m_pAllParamFactor->Reset();
	}
	else
	{
		if(m_pEnchantParamFactor)
		{
			*m_pAllParamFactor -= *m_pEnchantParamFactor;
		}
		else
		{
			//ERROR;
//			DBGOUT("Error : Delete Enchant information \n");// ����Ʈ �������� ����
		}
	}
	util::del(m_pEnchantParamFactor);
	ResetRealItemInfo();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CItemInfo::SetEnchantParam( CParamFactor * pEnchantParam, int nEnchantNumber )
/// \brief		EnchantParamFactor�� �ܺο��� �����Ѵ�.
/// \author		dhkwon
/// \date		2004-07-18 ~ 2004-07-18
/// \warning	������ ������ �� ��æƮ ������ ���� �̾Ƽ� �����Ѵ�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////


// 2010. 02. 11 by ckPark �ߵ��� ����������
// void CItemInfo::SetEnchantParam( CParamFactor * pEnchantParam, int nEnchantNumber )
// {
// 	if(pEnchantParam)
// 	{
// 		m_nEnchantNumber = nEnchantNumber;
// 		if(m_pEnchantParamFactor == NULL)
// 		{
// 			m_pEnchantParamFactor = new CParamFactor;
// 			m_pEnchantParamFactor->Reset();
// 		}
// 		*m_pEnchantParamFactor = *pEnchantParam;
// 		if(m_pAllParamFactor == NULL)
// 		{
// 			m_pAllParamFactor = new CParamFactor;
// 			m_pAllParamFactor->Reset();
// 		}
// 		ParamFactor_plus_ParamFactor( *m_pAllParamFactor, *m_pEnchantParamFactor );
// 		ResetRealItemInfo();
// 	}
// }
// ��æƮ ���� ����
void CItemInfo::SetEnchantParam( CParamFactor* pEnchantParam,
								 std::vector<ENCHANT_PARAM>* pDefEnchant,
								 std::vector<ENCHANT_PARAM>* pEnchantParamList,
								 int nEnchantNumber )
{
	// �⺻ �����Ķ� ��æƮ
	if(pEnchantParam)
	{
		if(m_pEnchantParamFactor == NULL)
		{
			m_pEnchantParamFactor = new CParamFactor;
			m_pEnchantParamFactor->Reset();
		}
		*m_pEnchantParamFactor = *pEnchantParam;
		if(m_pAllParamFactor == NULL)
		{
			m_pAllParamFactor = new CParamFactor;
			m_pAllParamFactor->Reset();
		}
		ParamFactor_plus_ParamFactor( *m_pAllParamFactor, *m_pEnchantParamFactor );
		ResetRealItemInfo();

		m_vecDefEnchant = *pDefEnchant;
	}

	// �ߵ��� �����Ķ� ��æƮ
	if( pEnchantParamList )
		m_vecInvokeEnchant = *pEnchantParamList;

	// ����æƮ ����
	m_nEnchantNumber = nEnchantNumber;
}
// end 2010. 02. 11 by ckPark �ߵ��� ����������


///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM* CItemInfo::GetItemInfo()
/// \brief		������ ���� ������ �����Ѵ�.
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	�������� �ٸ� ��æƮ,���������� ���Ե� ���̴�.
///				��æƮ, ���� ������ m_pAllParamFactor�� �̹� �� �ִ�.
///				SetRareItemInfo, PutEnchant
/// \param		
/// \return		ITEM*
///////////////////////////////////////////////////////////////////////////////
ITEM* CItemInfo::GetRealItemInfo()
{
	if(m_pItemInfo == NULL)
	{
		ITEM* pITEM = g_pDatabase->GetServerItemInfo(ItemNum);	// �⺻ ����
		if(pITEM)
		{
			if(pITEM != ItemInfo)
			{
				ItemInfo = pITEM;
			}
			m_pItemInfo = new ITEM;
			memcpy( m_pItemInfo, pITEM, sizeof(ITEM));
			if(m_pAllParamFactor)
			{
				SetItem_ParamFactor( *m_pItemInfo, *m_pAllParamFactor );	// ParamFactor ����
			}
		}
	}
	
	return m_pItemInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM* CItemInfo::ResetRealItemInfo()
/// \brief		m_pItemInfo�� �ٽ� �����Ѵ�.
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	��æƮ ������ ����� ��� �����Ѵ�.
///
/// \param		
/// \return		ITEM*
///////////////////////////////////////////////////////////////////////////////
ITEM* CItemInfo::ResetRealItemInfo()
{
	util::del(m_pItemInfo);
	GetRealItemInfo();
	return m_pItemInfo;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ITEM* CItemInfo::GetItemInfo()
/// \brief		������ ����(���� ����)
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ITEM* CItemInfo::GetItemInfo()
{
	if(ItemInfo == NULL)
	{
		ItemInfo = g_pDatabase->GetServerItemInfo(ItemNum);
	}
	return ItemInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int		TickReUsable(float fElapsedTime);
/// \brief		
/// \author		dgwoo
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		���� �ð��� �����Ѵ�.(ms) ��밡���Ҷ� 0�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
int	CItemInfo::TickReUsable(float fElapsedTime)
{
	if(!m_bUse)
	{
		m_nReUsable -= (int)(fElapsedTime * 1000);
		if(m_nReUsable <= 0)
		{
			m_bUse = TRUE;
			m_nReUsable = 0;
		}else
		{
			return m_nReUsable;
		}
	}
	return m_nReUsable;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int		UseItem()
/// \brief		
/// \author		dgwoo
/// \date		2007-06-21 ~ 2007-06-21
/// \warning	
///
/// \param		
/// \return		���� �ð��� �������ش�.
///////////////////////////////////////////////////////////////////////////////
int	CItemInfo::UseItem()
{
	m_nReUsable = ItemInfo->ReAttacktime;
	m_bUse = FALSE;

	return m_nReUsable;
}
float CItemInfo::GetDesItemWeight()
{
	if(m_pAllParamFactor == NULL)
	{
		return 0.0f;
	}

	return (m_pAllParamFactor->pfm_WEIGHT_01 + m_pAllParamFactor->pfm_WEIGHT_02);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�������� �ٲ۴�.
/// \author		// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
/// \date		2008-08-19 ~ 2008-08-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::ChangeItemNum(INT nChangeItemNum)
{
	ItemNum = nChangeItemNum;
	GetPrefixRareInfo();
	GetSuffixRareInfo();
	GetRealItemInfo();	
	ResetRealItemInfo();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�������� Ǯ������ ���´�.
/// \author		// 2009-04-08 by bhsohn ���� ���ý�, ����ũ �� ��æƮ �� ������ �ǸŽ� ��� �޽��� ��� ��ȹ ����(K0000174)
/// \date		2009-04-08 ~ 2009-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::GetItemFullName(char* o_pItemName)
{
	o_pItemName[0] = NULL;
	RARE_ITEM_INFO *pRefPrefixRareInfo = GetPrefixRareInfo();
	RARE_ITEM_INFO *pRefSuffixRareInfo = GetSuffixRareInfo();
	if(pRefPrefixRareInfo)
	{
		// ���λ�
		wsprintf( o_pItemName, "\\g%s\\g",pRefPrefixRareInfo->Name );
	}
	ITEM* pRealItem = GetRealItemInfo();
	if(pRealItem)
	{
		char chRealName[100];
		ZERO_MEMORY(chRealName);
		if(COMPARE_BIT_FLAG(pRealItem->ItemAttribute, ITEM_ATTR_UNIQUE_ITEM))
		{
			wsprintf( chRealName, "%s",pRealItem->ItemName);
			SortItemName(chRealName);
			// ��þƮ �����۸�
			wsprintf( o_pItemName,"%s %s", o_pItemName, chRealName);		
			
		}	
		else if(COMPARE_BIT_FLAG(pRealItem->ItemAttribute, ITEM_ATTR_LEGEND_ITEM))
		{
			wsprintf( chRealName, "%s",pRealItem->ItemName);
			SortItemName(chRealName);
			// ��þƮ �����۸�
			wsprintf( o_pItemName,"%s %s", o_pItemName, chRealName);		
		}	
		else if(GetEnchantNumber() > 0)
		{
			wsprintf( chRealName, "\\e%s\\e",pRealItem->ItemName);
			// ��þƮ �����۸�
			wsprintf( o_pItemName,"%s %s", o_pItemName, chRealName);		
		}		
		else
		{
			wsprintf( o_pItemName,"%s %s", o_pItemName, pRealItem->ItemName);
		}
	}

	if(pRefSuffixRareInfo)
	{
		char chSuffixName[100];
		ZERO_MEMORY(chSuffixName);
		wsprintf( chSuffixName, "\\g%s\\g",pRefSuffixRareInfo->Name );
		// ���̻�
		wsprintf( o_pItemName,"%s %s", o_pItemName, chSuffixName);		
	}
	//�� Ȧ���� ������ ������ �߻��ϹǷ� ���� ������ �־��ش�.
	SortItemName(o_pItemName);
}

// 2010. 02. 11 by ckPark �ߵ��� ����������
INT		CItemInfo::GetCoolElapsedTime( void )
{
	return m_nCoolElapsedTime;
}

VOID	CItemInfo::SetCoolElapsedTime( const INT nCoolElapsedTime )
{
	m_nCoolElapsedTime	= nCoolElapsedTime;
}


void	CItemInfo::PushDefEnchantParam( ENCHANT_PARAM ep )
{
	m_vecDefEnchant.push_back( ep );
}

ParamValue_t	CItemInfo::GetDefEnchantParamValue( DestParam_t nDesParam )
{
	// ���� �����Ķ��� ���� �� �ջ�
	ParamValue_t fDesValue = 0.0f;
	std::vector<ENCHANT_PARAM>::iterator it = m_vecDefEnchant.begin();
	while( it != m_vecDefEnchant.end() )
	{
		if( (*it).m_nDesParam == nDesParam )
			fDesValue += (*it).m_fDesValue;
		
		++it;
	}
	
	return fDesValue;
}

std::vector<ENCHANT_PARAM>*	CItemInfo::GetDefEnchantParamList( void )
{
	return &m_vecDefEnchant;
}

void	CItemInfo::ClearDefEnchantParam( void )
{
	m_vecDefEnchant.clear();
}

void	CItemInfo::PushInvokeEnchantParam( ENCHANT_PARAM ep )
{
	m_vecInvokeEnchant.push_back( ep );
}

ParamValue_t	CItemInfo::GetInvokeEnchantParamValue( DestParam_t nDesParam )
{
	// ���� �����Ķ��� ���� �� �ջ�
	ParamValue_t fDesValue = 0.0f;
	std::vector<ENCHANT_PARAM>::iterator it = m_vecInvokeEnchant.begin();
	while( it != m_vecInvokeEnchant.end() )
	{
		if( (*it).m_nDesParam == nDesParam )
			fDesValue += (*it).m_fDesValue;
		
		++it;
	}
	
	return fDesValue;
}

std::vector<ENCHANT_PARAM>*	CItemInfo::GetInvokeEnchantParamList( void )
{
	return &m_vecInvokeEnchant;
}

void	CItemInfo::ClearInvokeEnchantParam( void )
{
	m_vecInvokeEnchant.clear();
}

void	CItemInfo::CopyItemInfo( CItemInfo* pSrcItem )
{
	// ��æƮ ���� ����
	SetEnchantParam( pSrcItem->GetEnchantParamFactor(),
					 pSrcItem->GetDefEnchantParamList(),
					 pSrcItem->GetInvokeEnchantParamList(),
					 pSrcItem->GetEnchantNumber() );

	// ��Ÿ�� ���� ����
	SetCoolElapsedTime( pSrcItem->GetCoolElapsedTime() );

	FixedTermShape = pSrcItem->FixedTermShape;		// 2012-10-13 by jhahn �Ⱓ�� ������ �߰�

}
// end 2010. 02. 11 by ckPark �ߵ��� ����������

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�� Ȧ���� ������ ������ �߻��ϹǷ� ���� ������ �־��ش�.
/// \author		// 2009-04-08 by bhsohn ���� ���ý�, ����ũ �� ��æƮ �� ������ �ǸŽ� ��� �޽��� ��� ��ȹ ����(K0000174)
/// \date		2009-04-08 ~ 2009-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::SortItemName(char* o_pItemName)
{
	int nCnt = 0;
	int nFindColorCnt = 0;
	char chLastColor = 0;
	int nItemLen = strlen(o_pItemName);
	BOOL bFindBackSpace = FALSE;
	for(nCnt = 0 ;nCnt < nItemLen; nCnt++)
	{
		if('\\' == o_pItemName[nCnt])
		{
			bFindBackSpace = TRUE;
		}
		else if(bFindBackSpace && (0 != GetFontColor( o_pItemName[nCnt])))
		{
			bFindBackSpace = FALSE;
			chLastColor = o_pItemName[nCnt];
			nFindColorCnt++;			
		}
		else
		{
			bFindBackSpace = FALSE;
		}

	}
	if(1 == (nFindColorCnt %2) && (0 != chLastColor))
	{
		// �� ������ Ȧ���� �������� ������ �־�����
		o_pItemName[nItemLen] = '\\';
		o_pItemName[nItemLen+1] = chLastColor;
		o_pItemName[nItemLen+2] = NULL;
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�ΰ��� ���ս� �߰�����
/// \author		// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
/// \date		2013-04-08 ~ 2013-04-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CItemInfo::GetItemGeneral(ITEM_GENERAL* pItem)
{
	pItem->Kind = Kind;	 
	pItem->UniqueNumber = UniqueNumber ;
	pItem->ItemNum = ItemNum ;
	pItem->Wear = Wear;
	pItem->AccountUniqueNumber = AccountUniqueNumber;
	pItem->Possess = Possess;
	pItem->CurrentCount = CurrentCount;
	
	pItem->ItemWindowIndex = ItemWindowIndex;
	pItem->NumOfEnchants = NumOfEnchants;
	pItem->PrefixCodeNum = PrefixCodeNum ;
	pItem->SuffixCodeNum = SuffixCodeNum ;
	pItem->CurrentEndurance = CurrentEndurance;
	pItem->ColorCode = ColorCode;
		
	pItem->ShapeItemNum = ShapeItemNum;
	
	
	pItem->UsingStartTime = UsingStartTime;
	pItem->CreatedTime = CreatedTime ;
	pItem->ItemInfo = NULL;
	
	pItem->UsingTimeStamp = m_fItemPassTime;
	
	// �ߵ��� ����������
	pItem->CoolingTimeStamp = CoolingTimeStamp ;
	pItem->CoolingStartTime = CoolingStartTime;
	
	pItem->UniqueNumber = m_UniqueNumber;	// 2012-06-14 by isshin ���۹̸�����	
	pItem->FixedTermShape = FixedTermShape;	 // 2012-10-13 by jhahn �Ⱓ�� ������ �߰� 
}

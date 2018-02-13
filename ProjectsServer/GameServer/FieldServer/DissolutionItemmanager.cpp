// // 2010-08-31 by shcho&jskim �����ۿ��� �ý��� - ���� �ý��� Ŭ����	
// DissolutionItemmanager.cpp: implementation of the DissolutionItemmanager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DissolutionItemmanager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDissolutionItemManager::CDissolutionItemManager()
{
	Initialize();
}

CDissolutionItemManager::~CDissolutionItemManager()
{
	Release();
}

void CDissolutionItemManager::Initialize ( void )
{
	m_mapDissolutionItemData.clear();
}

void CDissolutionItemManager::Release ( void )
{
	m_mapDissolutionItemData.clear();
}

BOOL CDissolutionItemManager::LoadDBDissolutionItemData ( CODBCStatement *i_odbcstmt )
{
	if ( NULL == i_odbcstmt )
	{
		return FALSE;
	}

	Release();
	
	CAtumDBHelper::DBLoadDissolutionItemDataInfo( i_odbcstmt , &m_mapDissolutionItemData, FALSE);
	
	return TRUE;
}

// ���� ������ üũ �Լ�
tDissolutionItemInfo CDissolutionItemManager::CheckDissolutionItem(INT i_itemnum)
{	
	m_mapDissolutionItemData.lock();  // �� �������....
	
	tDissolutionItemInfo tmpDisItem;
	util::zero(&tmpDisItem,sizeof(tDissolutionItemInfo));
	mtmapDissolutionItemInfo::iterator iter = m_mapDissolutionItemData.find(i_itemnum);

	if(iter !=m_mapDissolutionItemData.end()) 
	{ // �����Ͱ� �������
			tmpDisItem.SourceItemnum = iter->first;
			
			for(int icnt = 0 ; icnt < SIZE_MAX_DISSOLUTION_ITEMS ; icnt++)
			{
				tmpDisItem.ResultItemNum[icnt]			=	iter->second.ResultItemNum[icnt];
				tmpDisItem.ResultItemMinCount[icnt]		=	iter->second.ResultItemMinCount[icnt]; 
				tmpDisItem.ResultItemMaxCount[icnt]		=	iter->second.ResultItemMaxCount[icnt]; 
				tmpDisItem.ResultItemProbabillity[icnt]	=	iter->second.ResultItemProbabillity[icnt]; 
			}
			m_mapDissolutionItemData.unlock(); // ���
			return tmpDisItem;
			
	}
	
	m_mapDissolutionItemData.unlock(); // ���
	return tmpDisItem;
}

// ������ ���� �Լ� ���ϰ�: TRUE OR FALSE
BOOL CDissolutionItemManager::CreateDissolutionItem(INT i_itemnum, tDissolutionItemInfo i_DissolutionData, DissolutionItems* o_Dissolved_ItemData)
{
	INT DissolutionItemNum = 0;
	INT SuccessPersent = NULL;

	if(!i_itemnum) // ������ ���� ������
	{
		return FALSE;
	}

	for(int icnt = 0 ; icnt < SIZE_MAX_DISSOLUTION_ITEMS ; ++icnt)
	{
		SuccessPersent = RANDI(0,99); // 0~99���� ���� ���Ѵ�.
		
		if(SuccessPersent <= i_DissolutionData.ResultItemProbabillity[icnt]) // 100������ ���� Ȯ�������� �۰ų� ���� ��쿡�� �����Ų��.
		{
			o_Dissolved_ItemData->Itemnum[icnt] = i_DissolutionData.ResultItemNum[icnt]; // ���ص� ������ ����(3��)
			// ���ص� 3���� �������� ī��Ʈ
			o_Dissolved_ItemData->Itemcount[icnt] = RANDI(i_DissolutionData.ResultItemMinCount[icnt],i_DissolutionData.ResultItemMaxCount[icnt]);
		}
		else 
		{
			o_Dissolved_ItemData->Itemnum[icnt]  = 0;
			o_Dissolved_ItemData->Itemcount[icnt] = 0;
		}  
		SuccessPersent = NULL; // 100�� �ʱ�ȭ
	}
	//..

	return TRUE;
}


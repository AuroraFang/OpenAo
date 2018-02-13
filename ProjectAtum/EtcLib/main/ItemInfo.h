// ItemInfo.h: interface for the CItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMINFO_H__1B0BB4FA_5A16_4585_8186_2F1C911EE29C__INCLUDED_)
#define AFX_ITEMINFO_H__1B0BB4FA_5A16_4585_8186_2F1C911EE29C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumParam.h"

// 2010. 02. 11 by ckPark �ߵ��� ����������
struct ENCHANT_PARAM
{
	DestParam_t		m_nDesParam;		// �����Ķ�
	ParamValue_t	m_fDesValue;		// �����Ķ� ��

	ENCHANT_PARAM( DestParam_t nDesParam = 0, ParamValue_t fDesValue = 0.0f ) : m_nDesParam( nDesParam ), m_fDesValue( fDesValue )
	{

	}
};
// end 2010. 02. 11 by ckPark �ߵ��� ����������

///////////////////////////////////////////////////////////////////////////////
/// \class		CItemInfo
///
/// \brief		������(������ ���� ����ũ�ѹ� ����)�� ���� ���̺�
///				CStoreData�� m_mapItemInInventory�� �� ������ ��� �־�� �Ѵ�. 
/// \author		dhkwon
/// \version	
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CItemInfo : public ITEM_GENERAL
{
public:
	CItemInfo(ITEM_GENERAL* pItem);
	virtual ~CItemInfo();

	void AddEnchantItem(INT nEnchantItemNum);
	RARE_ITEM_INFO  *GetPrefixRareInfo();// { return m_pRefPrefixRareInfo; }
	RARE_ITEM_INFO  *GetSuffixRareInfo();// { return m_pRefSuffixRareInfo; }
	CParamFactor	*GetEnchantParamFactor() { return m_pEnchantParamFactor; }	//	��æƮ �Ķ���͸� ����� ��
	int				GetEnchantNumber() { return m_nEnchantNumber; }
	CParamFactor	*GetParamFactor() { return m_pAllParamFactor; }	// m_pPrefixRareInfo,m_pSuffixRareInfo,m_pEnchantParamFactor �� ��� ����� �����
	ITEM* GetRealItemInfo();			// ������ ��¥ ����(��æƮ,���� ���� ��)
	ITEM* GetItemInfo();				// ������ ����(����,�⺻ ����)
	ITEM* ResetRealItemInfo();			// ������ ��¥ ���� �ٽ� ����(��æƮ ������)

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	//void SetEnchantParam( CParamFactor * pEnchantParam, int nEnchantNumber );	// ������ ������(��æƮ ������ ����)
	// �ߵ��� �����Ķ� ��æƮ �߰�
	void SetEnchantParam( CParamFactor* pEnchantParam,
						  std::vector<ENCHANT_PARAM>* pDefEnchant,
						  std::vector<ENCHANT_PARAM>* pEnchantParamList,
						  int nEnchantNumber );
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	void ChangeRareInfo(INT nPrefixCodeNum, INT nSuffixCodeNum);
	void DeleteEnchantInfo();
	void ReleaseRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex);
	float GetItemPassTime() { return m_fItemPassTime; }
	void SetItemPassTime(float fPassTime) { m_fItemPassTime = fPassTime; }
	float GetDesItemWeight();
	
	int		TickReUsable(float fElapsedTime);
	int		UseItem();

	// 2008-08-19 by bhsohn ���� ��ũ �ý��� �߰�
	void	ChangeItemNum(INT nChangeItemNum);

	// 2009-04-08 by bhsohn ���� ���ý�, ����ũ �� ��æƮ �� ������ �ǸŽ� ��� �޽��� ��� ��ȹ ����(K0000174)
	void	GetItemFullName(char* o_pItemName);
	// end 2009-04-08 by bhsohn ���� ���ý�, ����ũ �� ��æƮ �� ������ �ǸŽ� ��� �޽��� ��� ��ȹ ����(K0000174)

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	// ��Ÿ��
	INT							GetCoolElapsedTime( void );
	VOID						SetCoolElapsedTime( const INT nCoolElapsedTime );


	// �⺻ ��æƮ ���� �߰�
	void						PushDefEnchantParam( ENCHANT_PARAM ep );
	// �⺻ ��æƮ ��
	ParamValue_t				GetDefEnchantParamValue( DestParam_t nDesParam );
	// �⺻ ��æƮ ����Ʈ
	std::vector<ENCHANT_PARAM>*	GetDefEnchantParamList( void );
	// ��� �⺻ ��æƮ ���� ������
	void						ClearDefEnchantParam( void );


	// �ߵ��� ��æƮ ���� �߰�
	void						PushInvokeEnchantParam( ENCHANT_PARAM ep );
	// �ߵ��� ��æƮ ��
	ParamValue_t				GetInvokeEnchantParamValue( DestParam_t nDesParam );
	// �ߵ��� ��æƮ ����Ʈ
	std::vector<ENCHANT_PARAM>*	GetInvokeEnchantParamList( void );
	// ��� �ߵ��� ��æƮ ���� ������
	void						ClearInvokeEnchantParam( void );


	// ��æƮ �� ��Ÿ�� ���� ����
	void						CopyItemInfo( CItemInfo* pSrcItem );
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	// 2010-06-15 by shcho&hslee ��ý��� - �ŷ� ����
	void SetPetLevel( int level ) { m_nPetLevel = level; }
	int  GetPetLevel( ) { return m_nPetLevel; }

	void SetReName( BOOL rename ) { m_bReName = rename; }
	BOOL GetReName( ) { return m_bReName; }

	void SetPetEnableLevelUp( BOOL EnableLevelUp ) { m_bEnableLevelUp = EnableLevelUp; }
	BOOL GetPetEnableLevelUp() { return m_bEnableLevelUp; } 

	void SetPetExp( Experience_t Exp ) { m_dPetExp = Exp; }
	Experience_t GetPetExp() { return m_dPetExp; }
	// end 2010-06-15 by shcho&hslee ��ý��� - �ŷ� ����

	UID64_t GetUniqueNumber(){return m_UniqueNumber;}	// 2012-06-14 by isshin ���۹̸�����

	// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
	void GetItemGeneral(ITEM_GENERAL* pItemGeneral);
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	BOOL GetLimitTimeFlag(){return m_bLimitTime;}					// 2013-05-09 by ssjung ���� ���� �Ⱓ���� ��� �޽��� ����
	void SetLimitTimeFlag(BOOL bFlag){m_bLimitTime = bFlag;}		// 2013-05-09 by ssjung ���� ���� �Ⱓ���� ��� �޽��� ����
#endif
protected:
	void SetRareItemInfo(RARE_ITEM_INFO *pInfo, int nRareIndex);// m_pPrefixRareInfo, m_pSuffixRareInfo�� ���� ���ÿ�
	void PutEnchant(BYTE nDestParameter, float fParameterValue);					// �������� ��æƮ ���� ���� ��

	// 2009-04-08 by bhsohn ���� ���ý�, ����ũ �� ��æƮ �� ������ �ǸŽ� ��� �޽��� ��� ��ȹ ����(K0000174)
	void SortItemName(char* o_pItemName);
	// end 2009-04-08 by bhsohn ���� ���ý�, ����ũ �� ��æƮ �� ������ �ǸŽ� ��� �޽��� ��� ��ȹ ����(K0000174)
	
protected:
	RARE_ITEM_INFO  *m_pRefPrefixRareInfo;
	RARE_ITEM_INFO  *m_pRefSuffixRareInfo;
	CParamFactor	*m_pEnchantParamFactor;
	int				m_nEnchantNumber;
	CParamFactor	*m_pAllParamFactor;		
	ITEM			*m_pItemInfo;			// m_pAllParamFactor �� ����� ������

	float			m_fItemPassTime;		// 2006-03-31 by ispark, �ð����� ������ ����� �ð�
	int				m_nReUsable;			// �� ���ð�.
	BOOL			m_bUse;					// ����Ҽ� �ִ� �����ΰ�?

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	INT				m_nCoolElapsedTime;		// ��Ÿ��

	std::vector<ENCHANT_PARAM> m_vecDefEnchant;		// �⺻ ��æƮ ����Ʈ

	std::vector<ENCHANT_PARAM> m_vecInvokeEnchant;	// �ߵ��� ��æƮ ����Ʈ
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	// 2010-06-15 by shcho&hslee ��ý��� - �ŷ� ����
	int				m_nPetLevel;
	BOOL			m_bReName;
	BOOL			m_bEnableLevelUp;
	Experience_t	m_dPetExp;
	// end 2010-06-15 by shcho&hslee ��ý��� - �ŷ� ����
		
	UID64_t			m_UniqueNumber;			// 2012-06-14 by isshin ���۹̸�����
	BOOL			m_bLimitTime;			// 2013-05-09 by ssjung ���� ���� �Ⱓ���� ��� �޽��� ����

};

#endif // !defined(AFX_ITEMINFO_H__1B0BB4FA_5A16_4585_8186_2F1C911EE29C__INCLUDED_)

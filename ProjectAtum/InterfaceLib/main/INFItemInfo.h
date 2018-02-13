// INFItemInfo.h: interface for the CINFItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFITEMINFO_H__3EF422A5_2E9F_43E3_B059_879ADEDAF780__INCLUDED_)
#define AFX_INFITEMINFO_H__3EF422A5_2E9F_43E3_B059_879ADEDAF780__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

//#define ITEMINFO_TITLE_NUMBER		9
#define ITEMINFO_PARAMETER_NUMBER	24//14	// desc�� ����.	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
#define ITEMINFO_DESC_LINE_NUMBER	6	// desc�� 6�ٱ��� ����
#define ITEMINFO_EXTENDITEM_LINE_NUMBER	6	// Extenditeminfo�� 6�ٱ��� ����				  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
// 2008-04-14 by bhsohn ���� ������ ���� ��Ʈ������ ó��
//#define ITEMINFO_DESC_SIZE			40	// 20 x 6
#define ITEMINFO_DESC_SIZE			256	// 20 x 6

#define ITEMINFO_ITEM_FULL_NAME		140	// 140 = desc title len(20) + prefix len + item name len + suffix len
#define ITEMINFO_BIGICON_WIDTH		84
#define ITEMINFO_TOP_GAB			15
#define ITEMINFO_NAME_IMAGE_GAB		80
#define ITEMINFO_BIGICON_GAB		70

#define ITEMINFO_SIDE_TOPSIZE_WIDTH		4
#define ITEMINFO_SIDE_TOPSIZE_HEIGHT	5
#define ITEMINFO_SIDE_MIDSIZE_WIDTH		4
#define ITEMINFO_SIDE_MIDSIZE_HEIGHT	1
#define ITEMINFO_SIDE_BOTSIZE_WIDTH		4
#define ITEMINFO_SIDE_BOTSIZE_HEIGHT	4
#define ITEMINFO_SIDE_WIDTH_ENGLISH		8							// ���� ���� WIDTH

// 2006-03-07 by ispark, �� ���� ��ġ ����
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
#define INCHANTNUM_X				15
#else
#define INCHANTNUM_X				9
#endif

enum FUNCTION_VALUE_TYPE
{
	FUNCTION_VALUE_TYPE_NORMAL,
	FUNCTION_VALUE_TYPE_PROB,
	FUNCTION_VALUE_TYPE_PROB100,
	FUNCTION_VALUE_TYPE_PROB255,
	FUNCTION_VALUE_TYPE_PERCENT	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
};

enum 
{
	FUNCTION_INDEX_NOT_ACTION = 0,
	FUNCTION_INDEX_ITEM_REMAIN_TIME,
	FUNCTION_INDEX_ITEM_DEL_REMAIN_TIME,
	// 2010. 02. 11 by ckPark �ߵ��� ����������
	FUNCTION_INDEX_ITEM_COOL_TIME
	// end 2010. 02. 11 by ckPark �ߵ��� ����������
};

// 2010. 02. 11 by ckPark �ߵ��� ����������
typedef enum 
{
	FUNCTIONTYPE_NORMAL = 1,
	FUNCTIONTYPE_EQUIP,
	FUNCTIONTYPE_USE,
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	FUNCTIONTYPE_LOOKCHANGE,	// 2013-04-19 by ssjung ���� ���� Ŷ ��ȭ �ý���
	FUNCTIONTYPE_TIMELIMINT_ATTACK		    //2013-05-20 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ǥ��
#endif
} FUNCTION_TYPE;
// end 2010. 02. 11 by ckPark �ߵ��� ����������

struct stTickFuntionIndex
{
	int		nFuntionIndex;
	int		nDataLineIndex;
	int		nParam1;
	int		nParam2;
	int		nParam3;

	stTickFuntionIndex()
	{
		nFuntionIndex	= FUNCTION_INDEX_NOT_ACTION;
		nDataLineIndex	= 0;
		nParam1 = 0;
		nParam2 = 0;
		nParam3 = 0;
	}
};

class CD3DHanFont;
class CParamFactor;
class CINFImage;
class DataHeader;
class CINFImageEx;

class CINFItemInfo : public CAtumNode  
{
protected:
//	CParamFactor	m_paramFactor;
	BOOL			m_bEnableItem;		 // ���� ������ ������ : TRUE
	BOOL			m_bShow;
	char			m_strItemInfo[ITEMINFO_PARAMETER_NUMBER][ITEMINFO_ITEM_FULL_NAME];
	char			m_strDesc[ITEMINFO_DESC_LINE_NUMBER][ITEMINFO_DESC_SIZE];
	char			m_strExtendItemInfo[ITEMINFO_DESC_LINE_NUMBER][ITEMINFO_DESC_SIZE];				  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 

	CD3DHanFont	*	m_pFontItemInfo[ITEMINFO_PARAMETER_NUMBER];
	CD3DHanFont	*	m_pFontDescInfo[ITEMINFO_DESC_LINE_NUMBER];
	CD3DHanFont	*	m_pFontExtendItemInfo[ITEMINFO_DESC_LINE_NUMBER];							   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 

	CD3DHanFont	*	m_pFontItemName;
	CD3DHanFont	*	m_pInchantNum;
	char			m_strItemName[ITEMINFO_ITEM_FULL_NAME];

	int				m_nDescIndex;	// ��ü ���� �߿� DESC�� �ε���
	int				m_nExtendItemIndex;																					  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	int				m_nMaxLength;
	int				m_nDescLine;
	RARE_ITEM_INFO *m_pRefPrefixRareInfo;
	RARE_ITEM_INFO *m_pRefSuffixRareInfo;
	CParamFactor *	m_pRefEnchant;
	ITEM		*	m_pRefITEM;					// DB�� �ִ� ITEM�� ����.
	CItemInfo	*	m_pRefItemInfo;				// ���� �ڽ��� ������ �־�߸� ���� ������.
	POINT			m_ptItemInfo;
	CINFImageEx *		m_pBigIcon;
	CINFImageEx	*	m_pInfoBoxSide[9];
	DataHeader*		m_pDataHeader;
	CGameData	*	m_pGameData;

	// 2006-03-31 by ispark, ������ �����ϴ� ����
	int				m_nOtherItemCount;		// �̼ǿ��� ���޵Ǵ� ������ ����

	// 2006-10-12 by ispark, ����� ��� �Ǿ�� �ϴ� ������ Tick
	vector<stTickFuntionIndex>	m_vecTickFuntionIndex;

	// 2009-02-03 by bhsohn ���� ������ �� ����	
	BOOL			m_bMyEquipItem;
	CD3DHanFont	*	m_pFontMyEquipItem;
	SIZE			m_szTooltip;	
	// end 2009-02-03 by bhsohn ���� ������ �� ����

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	std::vector< std::pair<ItemNum_t, InfinityShopItemCnt_t> >	m_vecExchageMtrl;
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
public:
	CINFItemInfo(CAtumNode* pParent);
	virtual ~CINFItemInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Render();
	virtual void Tick();

	void ShowItemInfo(BOOL bShow) { m_bShow = bShow; }	
	// 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
	void SetItemInfoNormal( ITEM* pITEM, int x, int y, BOOL bShop=TRUE, int nItemCount = 0, 
					int nLinkItem=0 , BOOL ArmorCollect = FALSE);	// 2006-03-14 by ispark, nItemCount : �̼����޾����� ���� �߰�
	// END 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
	// 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
//	void SetItemInfoUser( CItemInfo* pItemInfo, int x, int y );		// 2006-03-31 by ispark, nItemTime : �ð����� ������ �߰�
	void SetItemInfoUser( CItemInfo* pItemInfo, int x, int y, int nLinkItem=0 ,BOOL bInven = FALSE);		// 2006-03-31 by ispark, nItemTime : �ð����� ������ �߰�
	// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	// END 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
	void InitItemInfo();
	void SetGameData(CGameData * pData);
	CINFImageEx* FindBigIcon(int nItemNum);

	BOOL			m_bRestored;

	// 2009-02-03 by bhsohn ���� ������ �� ����
	BOOL			IsShowItemInfo();
	void			SetMyEquipItem(BOOL bMyEquipItem);
	int				GetMaxLength();
	CItemInfo	*	GetRefItemInfo();
	POINT			GetItemInfoPos();
	void			SetItemInfoPos(POINT i_ptItemInfoPos);
	SIZE			GetItemInfoTooltipSize();
	void			SetItemInfoTooltipSize(SIZE	i_szTooltip);
	// end 2009-02-03 by bhsohn ���� ������ �� ����

	BOOL IsShowFunction(int nType);	// 2013-02-14 by bhsohn Ư�� �Ӽ� ������ ���� �̻��ϰ� ������ ���� ó��

	// 2010. 03. 31 by jskim ���� ������ �����ִ� ������ �� ����
	// 2013-03-27 by bhsohn DestParam �ڷ��� ����
//	void SetParameterInfo( char * str, BYTE bType,float fValue, BOOL bDefEnchant = TRUE );
//	void SetParameterInfo( char * str, DestParam_t bType,float fValue, BOOL bDefEnchant = TRUE);
	void SetParameterInfo( char * str, DestParam_t bType,float fValue, BOOL bDefEnchant = TRUE , BOOL bMonthlyArmor = FALSE); // 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
	//end 2010. 03. 31 by jskim ���� ������ �����ִ� ������ �� ���� protected -> public
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	void SetRareParameterInfo(char* str, float fValue);
	void DefferentFunction(int* index, CItemInfo* pRefItemInfo);
	//int	 GetRareParameterValue(CItemInfo* pRefItemInfo, int num);
	float GetRareParameterValue(CItemInfo* pRefItemInfo, int num);
	//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����

	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
	const ITEM	*GetRefITEM()	{return m_pRefITEM;}					// DB�� �ִ� ITEM�� ����.
	// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

	// 2013-05-07 by bhsohn M��� ������ ������ �۾�
	// �г�Ƽ ���� ��������
	float GetBuffPanaltyInfo(int nParamSize, DestParam_t *i_bType1, float *i_fValue1, USHORT shUnitKind); 
	// END 2013-05-07 by bhsohn M��� ������ ������ �۾�

protected:
	//void SetParameter(int* index, BOOL bLinkItem = FALSE);				// 2006-04-25 by ispark
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	//void SetParameter(int* index, BOOL bLinkItem = FALSE, BOOL bArmorItem = FALSE);				// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	void SetParameter(int* index, BOOL bLinkItem = FALSE, BOOL bArmorItem = FALSE, float RareValue = 0.0f);				// 2008-09-26 by bhsohn �ű� ��þƮ ó��

	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	void SetItemAllTime(int nParameterIndex, BOOL bCard = FALSE, BOOL bLinkItem = FALSE);	// 2006-03-31 by ispark
	void SetItemRemainTime(int nParameterIndex, BOOL bShop, BOOL bLinkItem = FALSE, BOOL bSetTick = TRUE);// 2006-03-31 by ispark
	void SetItemDelAllTime(int nParameterIndex);						// 2006-10-11 by ispark
	void SetItemDelRemainTime(int nParameterIndex, BOOL bSetTick = TRUE);	// 2006-10-11 by ispark
	void SetWeaponAngle(int nParameterIndex);
	void SetWeaponSpeed(int nParameterIndex);
	void SetReUseTime(int nParameterIndex);					// 2005-12-07 by ispark
	void SetSkillReAttackTime(int nParameterIndex);			// 2005-11-22 by ispark
	void SetEnginTurningAngle(int nParameterIndex);			// 2005-08-17 by ispark
	void SetRadarRange(int nParameterIndex);
	void SetSecondaryRange(int nParameterIndex);
	void SetPrimaryRange(int nParameterIndex);

	BOOL IsEnableItem(ITEM* pITEM);
	void SetMaxLength();
	void SetItemName( int nParameterIndex );
	void SetItemKind( int nParameterIndex );
	void SetUnitKind( int nParameterIndex );
	void SetReqLevel( int nParameterIndex );
	void SetReqStat(int nParameterIndex );
	void SetReqItemKind( int nParameterIndex );
	void SetShopReqItem(int nParameterIndex );
	void SetAttack( int nParameterIndex );
	void SetSecAttack( int nParameterIndex );
	void SetDefense( int nParameterIndex );
	void SetRate( int nParameterIndex );
	void SetRange( int nParameterIndex );
	void SetAttackTime( int nParameterIndex );
	void SetReAttackTime( int nParameterIndex );
	void SetOverHeatTime( int nParameterIndex );
	void SetRangeAngle( int nParameterIndex );
	void SetExplosionRange( int nParameterIndex );
	void SetReactionRange( int nParameterIndex );
	void SetSkillTime( int nParameterIndex);
	void SetWeight( int nParameterIndex );

	void SetItemLimit(int* nParameterIndex ); // 2012-10-11 by jhahn �Ⱓ�� ������ ǥ��

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	void SetShapeInfo( int nParameterIndex );
	void SetEffectInfo( int nParameterIndex );
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	// �ߵ��� �����Ķ� ����
	void SetInvokeDestParam( int* pParameterIndex );
	// ������ ��Ÿ�� ����
	void SetItemCoolTime( int* pParameterIndex, BOOL bSetTick = TRUE );
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	void SetCountableWeight( int nParameterIndex );
	void SetReqSP( int nParameterIndex );
	void SetSkillLevel( int nParameterIndex );
	void SetBullet( int nParameterIndex );
	void SetFUEL( int nParameterIndex );
	void SetCount( int nParameterIndex );
	void SetSpeed( int nParameterIndex );
	void SetBoosterSpeed( int nParameterIndex );
	void SetCharging(int nParameterIndex);
	void SetBoosterTime( int nParameterIndex );
	void SetRotateAngle( int nParameterIndex );
	void SetBoosterRotateAngle( int nParameterIndex );
	void SetDefenseColorInfo(int  nParameterIndex );
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	void SetPrimaryPierce(int nParameterIndex);
	void SetSecondaryPierce(int nParameterIndex);

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	//void SetFunction(int nParameterIndex, BYTE bType1, float fValue1, BYTE bType2, float fValue2);
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
// 	// �⺻ ��æƮ���� �ߵ����� ��æƮ �߰�
// 	void SetFunction( int nParameterIndex,
// 					  BYTE bType1,
// 					  float fValue1,
// 					  BYTE bType2,
// 					  float fValue2,
// 					  FUNCTION_TYPE nFunctionType = FUNCTIONTYPE_NORMAL );
// 	// end 2010. 02. 11 by ckPark �ߵ��� ����������
	// �⺻ ��æƮ���� �ߵ����� ��æƮ �߰�
	void SetFunction( int nParameterIndex,
					  DestParam_t bType1,    // 2011-08-02 by jhahn ��Ʈ�� ������ �ý��� Byte����  USHORT�� �����Ķ����� ����
					  float fValue1,
					  DestParam_t bType2, //BYTE bType2,// 2013-03-27 by bhsohn DestParam �ڷ��� ����					  
					  float fValue2,
		FUNCTION_TYPE nFunctionType = FUNCTIONTYPE_NORMAL,
		float fRareValue = 0.0f, 
		BOOL bMonthlyArmor = FALSE);		// 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
	// end 2010. 02. 11 by ckPark �ߵ��� ����������
	//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����


	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	void SetUseAreaInfinity( int* p_nParameterIndex );
	void SetExchangeMaterial( BOOL bShop = FALSE );
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	void SetDesc( int nParameterIndex );
	void SetShopSellInfo( int nParameterIndex );
	// 2010. 03. 31 by jskim ���� ������ �����ִ� ������ �� ���� - ��ġ �̵�
	// 	// 2010. 02. 11 by ckPark �ߵ��� ����������
	// 	//void SetParameterInfo(char * str, BYTE bType,float fValue);
	// 	// �⺻ ��æƮ���� �ߵ����� ��æƮ �߰�
	// 	void SetParameterInfo( char * str, BYTE bType,float fValue, BOOL bDefEnchant = TRUE );
	// 	// end 2010. 02. 11 by ckPark �ߵ��� ����������
	//end 2010. 03. 31 by jskim ���� ������ �����ִ� ������ �� ����

	// 2010-06-15 by shcho&hslee ��ý��� - �� Ÿ�� ���� �߰�.
	void SetPetType ( int nParameterIndex );

	void SetPetReName ( int nParameterIndex );
	void SetPetEnableLevelUp ( int nParameterIndex, BOOL bShop = FALSE );
	void SetPetExp    ( int nParameterIndex );
	// End 2010-06-15 by shcho&hslee ��ý��� - �� Ÿ�� ���� �߰�.

// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	void SetRadarItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);						// 2005-08-17 by ispark
	void SetPrimaryWeaponInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetSecondaryWeaponInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetSkillItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetDefenseItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetSupportItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetEnergyItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetIngotItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetCardItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetEnchantItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
//	void SetTankItemInfo(BOOL bShop=FALSE);							// 2006-04-24 by ispark, -> SetAccessoryUnLimitItemInfo()�� ����
	void SetBulletItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetComputerItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetArmorColorInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void PrevetionDeleteItem(BOOL bShop=FALSE, BOOL bInven = FALSE);
	void SetAccessoryUnLimitItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);				// 2006-04-24 by ispark

	void SetPetSoketItemInfo(BOOL bShop=FALSE, BOOL bInven = FALSE);						//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	void SetAccessoryTimeLimitItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);			// 2006-03-30 by ispark
	void SetInfluenceBuffItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);				// 2006-04-25 by ispark
	void SetInfluenceGameEventItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);			// 2006-04-25 by ispark
	void SetRandomBoxItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);					// 2006-08-11 by dgwoo �����ڽ�.
	void SetExclusiveUser(int nParameterIndex);
	void SetMarkItemInfo(BOOL bShop = FALSE, BOOL bInven = FALSE);						// 2006-08-21 by ispark
	void SetSkillSupportItem(BOOL bShop = FALSE, BOOL bInven = FALSE);					// 2006-10-02 by ispark
	void SetPetItemInfo ( BOOL bShop = FALSE , BOOL bInven = FALSE);						// 2010-06-15 by shcho&hslee ��ý���
// end 2013-06-26 by ssjung �κ��丮 �߰� ���� 		
	void RenderInfoWindows(int x, int y, int cx, int cy);		
	DataHeader* FindResource(char* szRcName);

	void SetOtherFuntionTick();

	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	void SetItemAttribute( int nParameterIndex);

	// 2008-04-14 by bhsohn ���� ������ ���� ��Ʈ������ ó��
	int GetItemStringLen();
	BOOL IsStringColor(char *i_szStr);

	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	float GetEnchantPreSuffixInfo(int nDesParameter);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
//	BOOL	SetRemainTime_Imp( CItemInfo* pRefItem, int* pIndex, BOOL bShop );
	BOOL	SetRemainTime_Imp( ITEM* pRefItem, int* pIndex, BOOL bShop ); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	BOOL SingofDecimalDecision(float fVal);

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	void SetLookChangeSystem(int* pParameterIndex, int nSourceItemNum);	//2013-05-20 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ǥ��
	
// 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
	void SetArmorCollectionItemInfo(BOOL bShop = FALSE);			
    void SetTimeLimt(int* nParameterIndex);
	void CoatingCount(int* nParameterIndex);			
	void SetMonthlyArmorValue(int* nParameterIndex, ATUM_DATE_TIME MonthlyEventEndDate, ItemNum_t MonthlyOptionItemNum);
	void SetArmorCollectionParameter(int* nParameterIndex, int nSourceItemNum = 0);
	void SetMonthlyArmorCollectionValue(int* nParameterIndex);
	void SetArmorAbilityValue(int* nParameterIndex, ITEM* pTempItem, int nParamValue, BOOL bCard);
// end 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
#endif
	void SetItemExtendInfo(BOOL bShow = FALSE, BOOL bflag = FALSE);									  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
};

#endif // !defined(AFX_INFITEMINFO_H__3EF422A5_2E9F_43E3_B059_879ADEDAF780__INCLUDED_)

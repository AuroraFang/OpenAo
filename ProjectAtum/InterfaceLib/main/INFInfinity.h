// INFInfinity.h: interface for the INFInfinity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINFINITY_H__764D1815_2DF5_49F4_B411_53F0EB0C0714__INCLUDED_)
#define AFX_INFINFINITY_H__764D1815_2DF5_49F4_B411_53F0EB0C0714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define FADEINTIME	1000
#define NORMALTIME	3000
#define FADEOUTTIME	1000
#define WHOLETIME	( FADEINTIME + NORMALTIME + FADEOUTTIME )

// 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)
#define TIP_FADEINTIME	1000
#define TIP_NORMALTIME	5000
#define TIP_FADEOUTTIME	1000
#define TIP_WHOLETIME	( TIP_FADEINTIME + TIP_NORMALTIME + TIP_FADEOUTTIME )

// 2011-07-22 by jhahn ����3�� (���� ó��) �ð�����
#define TIP_NORMALTIMEINFI	2000


// 2010. 05. 12 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. (�ΰ��� ���� ��� �ܰ� ǥ��.)
struct tALARM_DEFENSESTEPINFO
{
	tALARM_DEFENSESTEPINFO()
	{
		memset ( this , 0x00 , sizeof( tALARM_DEFENSESTEPINFO ) );
	}

	void Start ( const INT a_iStep , const D3DXVECTOR2 &a_v2StartPos , const D3DXVECTOR2 &a_v2EndPos )
	{
		
		v2Pos = v2Start= a_v2StartPos;
		v2End = a_v2EndPos;

		bFrame = TRUE;

		fFrame = .0f;
		fAlpha = .0f;

		iAction = 0;

		dwDelayTime = 2000;
		dwElapsedTime = 0;

		iStep = a_iStep;
		
	}

	D3DXVECTOR2	v2Pos , 
				v2Start , 
				v2End;

	BOOL	bFrame;

	float	fFrame;
	float	fAlpha;

	INT		iAction;

	DWORD	dwDelayTime;
	DWORD	dwElapsedTime;

	INT		iStep;

};

typedef std::vector < tALARM_DEFENSESTEPINFO >	tdf_vecAlarmDefenseStep;
// End 2010. 05. 12 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. (�ΰ��� ���� ��� �ܰ� ǥ��.)


struct KeyMonsterInfo
{
	MEX_MONSTER_INFO*	m_pMonsterInfo;		// ���� ����

	ClientIndex_t		m_nMonsterUID;		// ���� UID

	INT					m_nCurHP;			// ���� ü��

	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	INT					m_nMaxHP;			// �ִ� ü��
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

	BOOL				m_bWarningTime;		// ���� �������� �ϴ°�?

	BOOL				m_bWarning;			// true�� ������ ����, false�� �ƹ��͵� �ȱ׸�

	DWORD				m_dwWarningStartTime;// �������� ������ �ð�

	DWORD				m_dwChangeWaringTime;// �������� ������ �ٲ� �ð�

	KeyMonsterInfo( MEX_MONSTER_INFO* pMonsterInfo,
					ClientIndex_t nMonsterUID,
					INT nCurHP , INT nMaxHP ) : m_pMonsterInfo( pMonsterInfo )
									,m_nMonsterUID( nMonsterUID )
									,m_nCurHP( nCurHP )
									,m_bWarningTime( FALSE )
									,m_bWarning( FALSE )
									,m_dwWarningStartTime( 0 )
									,m_dwChangeWaringTime( 0 )

								// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
									, m_nMaxHP( nMaxHP )
								// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

	{

	}
};
// end 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)

// 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)
#include "ItemInfo.h"

struct TenderItem
{
	MSG_FC_INFINITY_TENDER_DROPITEM_INFO m_TenderItem;	// ���� ������

	//UINT			m_nTenderEndTime;					// �ð�

	ATUM_DATE_TIME	m_sTenderStartTime;					// ���� ���� �ð�.

	UINT			m_nTenderLeftTime;

	ITEM*			m_pItem;

	CINFImageBtn*	m_pTenderYes;						// ����

 	CINFImageBtn*	m_pTenderNo;						// ��������

	CItemInfo*		m_pItemInfo;

	CINFImageEx*	m_pTenderItemBigIcon;				// ���������� �������

	POINT			m_pt;

	BOOL				m_bRender;

	BOOL				m_bTendered;

	TenderItem( MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pTenderItem,
				//UINT nTenderEndTime,
				ATUM_DATE_TIME sTenderStartTime ,
				ITEM* pItem,
				CINFImageEx* pTenderItemBigIcon) : //m_nTenderEndTime( nTenderEndTime ),
												 m_nTenderLeftTime( 0 ),
												 m_pItem( pItem ),
												 m_pTenderYes( NULL ),
												 m_pTenderNo( NULL ),
												 m_pItemInfo( NULL ),
												 m_pTenderItemBigIcon( pTenderItemBigIcon ),
												 m_bRender( FALSE ),
												 m_bTendered( FALSE )
	{
		memcpy( &m_TenderItem, pTenderItem, sizeof( MSG_FC_INFINITY_TENDER_DROPITEM_INFO ) );

		ITEM_GENERAL stItem;
		memset( &stItem, 0x00, sizeof(ITEM_GENERAL) );
		stItem.ItemInfo			= pItem;
		stItem.ItemNum			= pItem->ItemNum;
		stItem.Kind				= pItem->Kind;
		stItem.PrefixCodeNum	= m_TenderItem.PrefixCodeNum;
		stItem.SuffixCodeNum	= m_TenderItem.SuffixCodeNum;

		m_sTenderStartTime = sTenderStartTime;

		m_pItemInfo	= new CItemInfo( &stItem );

		m_pt.x = m_pt.y = 0;
	}

	~TenderItem()
	{
		DeleteDeviceObject();
	}

	HRESULT	InitDeviceObject( void )
	{
		if( m_pTenderYes == NULL )
		{
			m_pTenderYes = new CINFImageBtn;
			m_pTenderYes->InitDeviceObjects( "if_tdyes3", "if_tdyes1", "if_tdyes0", "if_tdyes2" );
		}

		if( m_pTenderNo == NULL )
		{
			m_pTenderNo = new CINFImageBtn;
			m_pTenderNo->InitDeviceObjects( "if_tdno3", "if_tdno1", "if_tdno0", "if_tdno2" );
		}

		return S_OK;
	}

	HRESULT	RestoreDeviceObject( void )
	{
		if( m_pTenderYes )
			m_pTenderYes->RestoreDeviceObjects();

		if( m_pTenderNo )
			m_pTenderNo->RestoreDeviceObjects();

		return S_OK;
	}

	HRESULT	DeleteDeviceObject( void )
	{
		if( m_pTenderYes )
		{
			m_pTenderYes->DeleteDeviceObjects();
			util::del( m_pTenderYes );
		}

		if( m_pTenderNo )
		{
			m_pTenderNo->DeleteDeviceObjects();
			util::del( m_pTenderNo );
		}

		return S_OK;
	}

	HRESULT	InvalidateDeviceObject( void )
	{
		if( m_pTenderYes )
			m_pTenderYes->InvalidateDeviceObjects();

		if( m_pTenderNo )
			m_pTenderNo->InvalidateDeviceObjects();

		return S_OK;
	}
};
// end 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)

class CINFInfinity  : public CINFBase
{
private:
	CINFImageEx*		m_pTimeNumImage[ 10 ];		// ���� �̹���
	
	CINFImageEx*		m_pColonImage;				// �ݷ�

	CINFImageEx*		m_pClearImage;				// Clear

	CINFImageEx*		m_pFailImage;				// Fail

	CINFImageEx*		m_pResultImage;

	ATUM_DATE_TIME	m_InfinityStartTime;		// ���Ǵ�Ƽ ���۽ð�

	MSec_t			m_nInfinityLimitTime;		// ���ѽð�

	MSec_t			m_nInfinityPenaltyTime;		// ����� ���ѽð�

	CINFImageBtn*	m_pGiveUpBtn;				// �����ư

	CINFImageBtn*	m_pReturnCityBtn;			// ������ ���÷α�ȯ ��ư

	UINT			m_nFadeStartTime;			// �ð�����

	DWORD			m_dwAlphaColor;

	BOOL			m_bRenderTimeLimit;

	UINT			m_nHour, m_nMin, m_nSec;

	BOOL			m_bRenderResult;

	UINT			m_nInfinityReturnTime;		// ���Ǵ�Ƽ ���� Ÿ�̸�(���Ǵ�Ƽ ������ 5������ ����)

	BOOL			m_bAlert5Min, m_bAlert3Min, m_bAlert1Min, m_bAlert30Sec;


	CINFImageEx*		m_pTenderBG;				// ���� ��׶���

	CINFImageEx*		m_pTenderProgressFullBar;	// ���α׷��� ������

	CINFImageEx*		m_pTenderProgressHalfBar;	// ���α׷��� ������

	CINFImageEx*		m_pTenderTime[ 10 ];		// ���� ī��Ʈ �ð�

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*		m_pRenewInfiTenderImage;
#endif


	// 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)
// 	CINFImageBtn*	m_pTenderYes;				// ����
//
// 	CINFImageBtn*	m_pTenderNo;				// ��������
//
// 	UINT			m_nTenderEndTime;			// �ð�
// 
// 	UINT			m_nTenderLeftTime;
// 
// 	CINFImage*		m_pTenderItemBigIcon;		// ���������� �������
	// end 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)


	// 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)
	CINFImageEx*		m_pTipImg;					// �� �̹���

	BOOL			m_bTipShow;					// ���� �����ٰǰ�?

	DWORD			m_dwTipShowStartTime;		// ���� �����ֱ� �ð��� �ð�

	DWORD			m_dwTipAlphaColor;			// �� ���İ�


	std::vector<KeyMonsterInfo*>	m_vecKeyMonster;		// Ű���� ����Ʈ

	std::map<INT, CINFImageEx*>		m_mapKeyMonsterImg;		// Ű���� �̹��� ����Ʈ

	CINFImageEx*						m_pHPBG;				// HP���

	CINFImageEx*						m_pHP;					// HP��

	CINFImageEx*						m_pWarning;				// ������ ��� ǥ��
	// end 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)

	// 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)
	std::vector<TenderItem*>		m_vecTenderItemList;
	// end 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)


	// 2010. 05. 12 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. (�ΰ��� ���� ��� �ܰ� ǥ��.)
	D3DXVECTOR2						m_v2AlarmTargetPos;

	tdf_vecAlarmDefenseStep			m_vecAlarmDefenseStep;

	INT								m_iAlarm_DefenseStep;
	INT								m_iAlarm_DefenseStepFrame;

	CINFImageEx*						m_pIMG_Label_Step;

	CINFImageEx*						m_pIMG_Label_BNum[10];
	CINFImageEx*						m_pIMG_Label_SNum[10];
	// end 2010. 05. 12 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. (�ΰ��� ���� ��� �ܰ� ǥ��.)

	// 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
	BOOL							m_bEnableCinemaSkip;		// ��� �ó׸� ���� ��ŵ üũ��.


	CD3DHanFont*					m_pFontInfinity;

// 2011-07-22 by jhahn ����3�� (���� ó��) �ð�����
	int								m_iTip_Normaltime;
	int								m_iTip_Fadeintime;
	int								m_iTip_Fadeouttime;
	int								m_iTip_Wholetime;
//end 2011-07-22 by jhahn ����3�� (���� ó��) �ð�����

// 2011-11-15 EP4 ������ �ð� ó��
	int								m_nOldtime; 
	int								tick;
//end 2011-11-15 EP4 ������ �ð� ó��

public:
	CINFInfinity();
	virtual ~CINFInfinity();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual int		WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void	Tick( void );
	
	void	RenderTimeLimit( void );

	// ���� �ð� ����
	void	SetInfinityLimitTime( ATUM_DATE_TIME* pInfinityStartTime, MSec_t nLimitTime );

	// 2011-05-30 by jhahn	����3�� �ð� ����
	void	SetInfinityLimitTimeReset(  MSec_t nLimitTime );
	// end 2011-05-30 by jhahn	����3�� �ð� ����

	//  2012-04-27 by jhahn	Ʈ���� �ý��� �ð� ����
	void	SetInfinityTriggerLimitTimeReset(  MSec_t nLimitTime );
	// end 2012-04-27 by jhahn	Ʈ���� �ý��� �ð� ����
	// ���� �ð� ���� ����
	void	SetInfinityChangeLimitTime( MSG_FC_INFINITY_CHANGE_LIMITTIME* pMsg );
	// ���Ƽ �ð� ����
	inline void	SetInfinityPenaltyTime( MSec_t nPenaltyTime )
	{
		m_nInfinityPenaltyTime	= nPenaltyTime;
	}

	void		SetHellMap();
	BOOL		HellMapOn;

	void	RenderInfinityResult( void );
	// ���Ǵ�Ƽ ��� ����
	void	SetResult( BOOL bSucceeded );

	void	RenderTenderItem();
	// 2010. 05. 27 by jskim �ó׸� ���� ī�޶� ����
	void	RenderResult();
	// 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)
// 	inline	void	SetTenderItemBigIcon( CINFImage* pBigIcon )
// 	{
// 		m_pTenderItemBigIcon	= pBigIcon;
// 	}
// 	
// 	inline	void	SetTenderEndTime( UINT nTenderEndTime )
// 	{
// 		m_nTenderEndTime	= nTenderEndTime;
// 	}
//

	// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ���
// 	void	SendPutInTender( BOOL bGiveUp );
//void	SendPutInTender( UINT nItemFieldIndex, BOOL bGiveUp );
	void	SendPutInTender( UINT nItemFieldIndex, ItemNum_t nItemNum, BOOL bGiveUp );
	// end 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ���
	// end 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)

	void	SetTimelimit(){	m_bRenderTimeLimit = FALSE;}	  // 2011-11-15 EP4 ������ �ð� ó��
	void	SetInfinityEnd( void );

	// 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)
	// �� �̹��� ����
	void	SetTipImg( char* szTipFileName );

// 2011-07-22 by jhahn ����3�� (���� ó��) �ð�����
	void SetTipTick(BYTE choice);

	void SerchTipImg( char* szTipFileName );
//end 2011-07-22 by jhahn ����3�� (���� ó��) �ð�����

	// �� ������
	void	RenderTipImg( void );

	// Ű���� �߰�
	void	AddKeyMonster( MEX_MONSTER_INFO* pMonsterInfo, ClientIndex_t nMonsterUID, INT nCurHP , INT nMaxHP );

	// Ű���� �̹��� �߰�
	void	AddKeyMonsterImg( MEX_MONSTER_INFO* pMonsterInfo );

	// Ű���� ������
	KeyMonsterInfo*	GetKeyMonsterInfo( ClientIndex_t nMonsterUID );

	// Ű���� ü�� ����
	void	SetKeyMonsterHP( ClientIndex_t nMonsterUID, INT nCurHP );

	// Ű���� �׸���
	void	RenderKeyMonster( void );
	// end 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)

	// 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)
	void	AddTenderItem( MSG_FC_INFINITY_TENDER_DROPITEM_INFO* pTenderItem );

	void	RemoveTenderItem( UINT nItemFieldIndex );

	TenderItem*	GetTenderItem( UINT nItemFieldIndex );

	void	ReArrangeTenderItemPosition( void );

	void	ClearTenderItemList( void );

	int		WndProcTender( UINT uMsg, WPARAM wParam, LPARAM lParam );
	// end 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)

	// 2010. 05. 12 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. (�ΰ��� ���� ��� �ܰ� ǥ��.)

	// ���� ��� �ܰ� �˸� ���� ����.
	void	SetAlarm_DefenseStep ( const INT a_iStep );

	// ���� ��� �ܰ� �˸� ���� ����.
	void	SetAlarm_DefenseStep ( char *pStepMsg );

	// ���� ��� �ܰ� �˸� ���� ���.
	void	RenderAlarm_DefenseStep ( void );

	// ���� ��� �ܰ� ���� ó��.
	void	SetCount_Alarm_DefenseStep ( const INT a_iAddCount = 1 );

	inline	void SetValue_Alarm_DefenseStep ( const INT a_iStep ) { m_iAlarm_DefenseStep = a_iStep; }

	inline	INT GetValue_Alarm_DefenseStep ( void ) { return m_iAlarm_DefenseStep; }

	// 2010. 05. 17 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. �ܰ� ǥ�� ���� ����.

	BOOL	IsEvent_AlarmDefenseStep ( char *pStrBuff );

	// End 2010. 05. 17 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. �ܰ� ǥ�� ���� ����.

	// End 2010. 05. 12 by hsLee ���Ǵ�Ƽ �ʵ� 2�� UI �߰� ����. (�ΰ��� ���� ��� �ܰ� ǥ��.)


	// 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
	inline BOOL GetEnableCinemaSkip ( void ) { return m_bEnableCinemaSkip; }			// �ó׸� ���� ��ŵ Ȱ��ȭ�� ����.
	inline void	SetEnableCinemaSkip ( BOOL Enable ) { m_bEnableCinemaSkip = Enable; }	// �ó׸� ���� ��ŵ Ȱ��ȭ�� ����.

	void SkipEndingCinema ( char *pCName , const bool bNormalEnding = false );			// �ó׸� ���� ��ŵ ó��.

	void SendPacket_SkipEndingCinema ( const bool a_bEnableNormalEnding = false );		// �ó׸� ���� ��ŵ ��û ��ȣ ���.

	void UpdateTenderAllItemInfo_TenderStartTime ( const ATUM_DATE_TIME a_sUpdateTime );		// ���� �������� ���� ���� �ð� ����.
	// End 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.


};

#endif // !defined(AFX_INFINFINITY_H__764D1815_2DF5_49F4_B411_53F0EB0C0714__INCLUDED_)
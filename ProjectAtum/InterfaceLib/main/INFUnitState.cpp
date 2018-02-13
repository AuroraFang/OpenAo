// INFUnitState.cpp: implementation of the CINFUnitState class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "RangeTime.h"
#include "INFUnitState.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "ShuttleChild.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "TutorialSystem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#define NUM_OF_INFUNITSTATE_BIT		32

#define UNIT_STATE_SIZE_X			311
#define UNIT_STATE_SIZE_Y			60
#define UNIT_STATE_STR_SIZE_X		311
#define UNIT_STATE_STR_SIZE_Y		60

#define UNIT_STATE_START_X			((g_pD3dApp->GetBackBufferDesc().Width)/2)
#define UNIT_STATE_START_Y			(UNIT_STATE_SIZE_Y-7)
#define UNIT_STATE_STR_START_X		((g_pD3dApp->GetBackBufferDesc().Width)/2)
#define UNIT_STATE_STR_START_Y		(UNIT_STATE_SIZE_Y-7)

#define TIME_POSITION_1				0
#define TIME_POSITION_2				1
#define INSERT_STATE_VIEWING_TIME	5
#else
#define NUM_OF_INFUNITSTATE_BIT		32

#define UNIT_STATE_SIZE_X			311
#define UNIT_STATE_SIZE_Y			60
#define UNIT_STATE_STR_SIZE_X		311
#define UNIT_STATE_STR_SIZE_Y		60

#define UNIT_STATE_START_X			((g_pD3dApp->GetBackBufferDesc().Width - UNIT_STATE_SIZE_X)/2)
#define UNIT_STATE_START_Y			(UNIT_STATE_SIZE_Y-7)
#define UNIT_STATE_STR_START_X		((g_pD3dApp->GetBackBufferDesc().Width - UNIT_STATE_SIZE_X)/2)
#define UNIT_STATE_STR_START_Y		(UNIT_STATE_SIZE_Y-7)

#define TIME_POSITION_1				0
#define TIME_POSITION_2				1
#define INSERT_STATE_VIEWING_TIME	5
#endif


CINFUnitState::CINFUnitState()
{
	m_bRestored = FALSE;

}

CINFUnitState::CINFUnitState(CAtumNode* pParent)
{
	m_pParent = pParent;
	m_bRestored = FALSE;
	
	
	// tick
	m_dwState					= 0;//INF_UW_DANGER_MASK;
	m_dwInsertedState			= 0;	// �߰��� ���ԉ�����
	m_dwInsertedStateCountSum	= 0;	// ���Ե� ���¸� �󸶵��� �����ֳ�

	m_fElapsedTime				= 0;	// ���ø����̼� Tick������ ����ð�
	m_fElapsedTimeSum			= 0;	// m_fElapsedTime�� ��

	m_fTimeMiddlePos			= 0.4f;	// �������� ���� ������ �߰�����
    m_fTimeDirection			= 1.0f;	// �ð��� ��ȸ ����
	memset(m_pImgState, 0x00, sizeof(DWORD)*UNIT_WARN_STATE_MAX);
	memset(m_pImgStateStr, 0x00, sizeof(DWORD)*UNIT_WARN_STATE_MAX);

	m_tInsertedStateRemainTime.Set(INSERT_STATE_VIEWING_TIME);
	m_tBlinkTime.Set( 0.0f, 2.0f, 0.7f );
}

CINFUnitState::~CINFUnitState()
{
	for( int i=0; i<UNIT_WARN_STATE_MAX; i++)
	{
		util::del(m_pImgState[i]);
		util::del(m_pImgStateStr[i]);
	}

}



HRESULT CINFUnitState::InitDeviceObjects()
{
	ASSERT_ASSERT(m_pGameData);
	DataHeader *pDataHeader = NULL;
	char buf[64];
	for(int i=0;i<UNIT_WARN_STATE_MAX;i++)
	{
		wsprintf(buf, "us%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgState[i] = new CINFImageEx;
		m_pImgState[i]->InitDeviceObjects(pDataHeader );

		wsprintf(buf, "us_str%02d", i);
		pDataHeader = m_pGameData->Find(buf);
		m_pImgStateStr[i] = new CINFImageEx;
		m_pImgStateStr[i]->InitDeviceObjects(pDataHeader );
	}

	return S_OK;
}

HRESULT CINFUnitState::RestoreDeviceObjects()
{
	if(!m_bRestored)
	{
		for(int i=0;i<UNIT_WARN_STATE_MAX;i++)
		{
			m_pImgState[i]->RestoreDeviceObjects();
			m_pImgStateStr[i]->RestoreDeviceObjects();
		}
		
		m_bRestored = TRUE;
	}
	return S_OK;
}

HRESULT CINFUnitState::DeleteDeviceObjects()
{
	for(int i=0;i<UNIT_WARN_STATE_MAX;i++)
	{
		m_pImgState[i]->DeleteDeviceObjects();
		m_pImgStateStr[i]->DeleteDeviceObjects();

		util::del(m_pImgState[i]);
		util::del(m_pImgStateStr[i]);
	}
	return S_OK;
}

HRESULT CINFUnitState::InvalidateDeviceObjects()
{
	if(m_bRestored)
	{
		
		for(int i=0;i<UNIT_WARN_STATE_MAX;i++)
		{
			m_pImgState[i]->InvalidateDeviceObjects();
			m_pImgStateStr[i]->InvalidateDeviceObjects();
		}

		m_bRestored = FALSE;
	}
	return S_OK;
}


///////////////////////////////////////////////////////////////////////////////////////////
/// \fn		void CINFUnitState::Tick()
///
/// \brief		| ---------------|-----------------------|
///			timePos1		timeMiddlePos				timePos2
///
///			�ð��� timePos1 ���� ~ timePos2 ���̸� �ݺ��Ѵ�.
///			timePos1 ~ timeMiddlePos  ������ �̹����� �Ⱥ��� �ְ�
///			�̹����� �������� ó���Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////
void CINFUnitState::Tick()
{
	m_fElapsedTime = g_pD3dApp->GetElapsedTime();
/*	
	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_1) )
	{
		InsertState( INF_UW_DANGER_MASK );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_2) )
	{
		OffState( INF_UW_DANGER_MASK );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_3) )
	{
		SetState( INF_UW_OVERHEAT_MASK );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_4) )
	{
		OffState( INF_UW_OVERHEAT_MASK );
	}
	
	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_5) )
	{
		SetState( INF_US_TAKEOFF_MASK );
	}

	if( g_pD3dApp->m_pKeyBoard->GetAsyncKeyState(DIK_6) )
	{
		OffState( INF_US_TAKEOFF_MASK );
	}
*/
	
	m_tBlinkTime.Tick( m_fElapsedTime );
	m_tInsertedStateRemainTime.Tick( m_fElapsedTime );
	
	if( m_tBlinkTime.IsOverEnd() )
		m_tBlinkTime.Start();

	if( m_dwInsertedState )
	{
		BOOL bRemain = m_tInsertedStateRemainTime.IsRemain();
		if( !bRemain )
		{
			SetState( m_dwInsertedState );
			m_dwInsertedState = 0;
		}
	}

	/*
	m_fElapsedTime = m_fElapsedTime*m_fTimeDirection;

	if( m_fElapsedTimeSum > TIME_POSITION_2 || m_fElapsedTimeSum < TIME_POSITION_1) 
	{
		if( m_fElapsedTimeSum > TIME_POSITION_2 )
		{
			m_fElapsedTimeSum = TIME_POSITION_2;
			m_fTimeDirection = -1.0f;
		}
		else
		{
			m_fElapsedTimeSum = TIME_POSITION_1;
			m_fTimeDirection = 1.0f;
		}
		
		// ���Ե� ���� ���´� INSERT_STATE_VIEWING_TIME �ð��� ������ SetState( ) �� �߰� ���� �ش�
		if( m_dwInsertedState )
		{
			m_dwInsertedStateCountSum += 1;
			if( m_dwInsertedStateCountSum > INSERT_STATE_VIEWING_TIME )
			{
				m_dwInsertedStateCountSum = 0;
				SetState( m_dwInsertedState );
				m_dwInsertedState = 0;
			}
		}
	}
	
	m_fElapsedTimeSum += m_fElapsedTime;
	*/
	
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \fn		void CINFUnitState::Render()
/// \brief  1. CheckState( TRUE )�� ���� ���ԵȻ���(m_dwInsertedState)�� �ִ��� üũ�Ѵ�.
///			2. index�� ���õ� ������ �̹��� �迭 ��ȣ�μ� 
///				index ��ȣ�� �Ϲ����λ���(UNIT_STATE_TAKEOFF�̻�)�� �̸� �׻� goto RENDER �Ѵ�.
///			3. index�� WARNING ���¸� �ð��� üũ�Ͽ� �����δ�.
/////////////////////////////////////////////////////////////////////////////////////////////
void CINFUnitState::Render()
{
	// 2005-04-19 by jschoi - Tutorial
	// 2005-08-02 by ispark - Character - ĳ���� ����
	if(g_pTutorial->IsTutorialMode() == TRUE 
		|| g_pD3dApp->m_bCharacter == TRUE
		|| g_pShuttleChild->IsObserverMode())
		return;
	int index = -1;

//	//���Ե� ���°� �ִ��� ýũ	
//	index = CheckState( TRUE );
//	if( index >= UNIT_STATE_TAKEOFF )		// ���°� STATE_TAKEOFF�̻��̸� �׻� �����ֱ�
//		goto RENDER;
//
//	//�Ϲ� ���°� �ִ��� üũ
//	index = CheckState( FALSE );
//	if( index >= UNIT_STATE_TAKEOFF )		// ���°� STATE_TAKEOFF�̻��̸� �׻� �����ֱ�
//		goto RENDER;
//	
//	if( index < 0 )							// ���°� ������ �Ⱥ��� �ֱ�
//		return;
//	
//	if( m_tBlinkTime.IsUnderMiddle() )	// �ð��� Middle �Ʒ��� �������� �׷��ֱ�(������)
//		goto RENDER;
//
//	/*
//	if( m_fElapsedTimeSum > m_fTimeMiddlePos )
//	{
//		goto RENDER;
//	}
//	*/
//	return;
	
	// 2005-07-08 by ispark
	// �� �ҽ��� ���� goto���� ����

	//���Ե� ���°� �ִ��� ýũ	
	index = CheckState(TRUE);
	if(index < UNIT_STATE_TAKEOFF)
	{
		//�Ϲ� ���°� �ִ��� üũ
		index = CheckState(FALSE);
		if(index < UNIT_STATE_TAKEOFF)
		{
			if(index <0)
				return;

			if(!m_tBlinkTime.IsUnderMiddle())
				return;
		}
	}
	

//RENDER:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	m_pImgState[index]->Move(UNIT_STATE_START_X - m_pImgState[index]->GetImgSize().x / 2, UNIT_STATE_START_Y);
	m_pImgState[index]->Render();
	
	if( g_pShuttleChild->m_myShuttleInfo.Level <= 11  )
	{
		m_pImgStateStr[index]->Move(UNIT_STATE_STR_START_X - m_pImgStateStr[index]->GetImgSize().x / 2, UNIT_STATE_STR_START_Y);
		m_pImgStateStr[index]->Render();
	}
#else
	m_pImgState[index]->Move(UNIT_STATE_START_X, UNIT_STATE_START_Y);
	m_pImgState[index]->Render();
	
	if( g_pShuttleChild->m_myShuttleInfo.Level <= 11  )
	{
		m_pImgStateStr[index]->Move(UNIT_STATE_STR_START_X, UNIT_STATE_STR_START_Y);
		m_pImgStateStr[index]->Render();
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////
/// \fn		int CINFUnitState::CheckState( BOOL bInsertedStateCheck )
/// \brief  1. bInsertedStateCheck �� ���� ýũ���� ����
///			2. dwCheckState �� #define�� ����ũ ���� &���� �ؼ� ���̸� �̹��� index�� ����
////////////////////////////////////////////////////////////////////////////////////

int CINFUnitState::CheckState( BOOL bInsertedStateCheck )
{
	int index = -1;
	DWORD dwCheckState = 0;			//ýũ�� ������Ʈ
	
	if( bInsertedStateCheck )
	{
		dwCheckState = m_dwInsertedState;
	}
	else
	{
		dwCheckState = m_dwState;
	}
	
	if( dwCheckState )
	{
		if( dwCheckState & INF_UW_FULLITEM3_MASK )		// ������ �ʰ�
		{
			index = 8;
			return index;
		}
		if( dwCheckState & INF_UW_FULLITEM2_MASK )		// ������ �ʰ�
		{
			index = 7;
			return index;
		}
		if( dwCheckState & INF_UW_FULLITEM1_MASK )		// ������ �ʰ�
		{
			index = 6;
			return index;
		}
		if( dwCheckState & INF_UW_DANGER_MASK )			// ��ü �ջ� ����
		{
			index = 0;
			return index;
		}
		
		if( dwCheckState & INF_UW_OVERHEAT_MASK)		// �ν��� ����
		{
			index = 1;
			return index;
		}
		
		if( dwCheckState & INF_UW_LOWFUEL_MASK )			// ���� ����
		{
			index = 2;
			return index;
		}
		if( dwCheckState & INF_UW_FULLITEM_MASK )		// ������ �ʰ�
		{
			index = 3;
			return index;
		}
		if( dwCheckState & INF_UW_TOOHIGH_MASK )		// ������ �ʰ�
		{
			index = 4;
			return index;
		}
		if( dwCheckState & INF_UW_LOWSHIELD_MASK )		// ������ �ʰ�
		{
			index = 5;
			return index;
		}
		
		
		// Inf(�������̽�) Unit State
		if( dwCheckState & INF_US_TAKEOFF_MASK )		// �̷��� ����(Take Off ) - �ٽ�����-0(ins)
		{
			index = 9;
			return index;
		}
		
		if( dwCheckState & INF_US_STOP_MASK )			// ���� ���߿� ������ - ���� -����Ű W
		{
			index = 10;
			return index;
		}
		if( dwCheckState & INF_US_LOCK_MASK )			// �����ϼ� ���� ����(Unit Lock) - R
		{
			index = 11;
			return index;
		}
		if( dwCheckState & INF_US_FORM_MASK )			// formation ��������( �շ�Ű f2)
		{
			index = 12;
			return index;
		}
		if( dwCheckState & INF_US_LAND_MASK )			// ���� Landing, ������ - �ٽ� �̷� W
		{
			index = 13;
			return index;
		}
	}
	
	return index;
}

void CINFUnitState::InsertState( DWORD dwState )
{
	if( m_dwInsertedState )
	{
		SetState( m_dwInsertedState );
	}
	m_dwInsertedState = dwState;	// �߰��� ���ԉ�����
	m_tInsertedStateRemainTime.Set(INSERT_STATE_VIEWING_TIME);// ���Ե� ���¸� �󸶵��� �����ֳ�
	//m_dwInsertedStateCountSum = 0;	// ���Ե� ���¸� �󸶵��� �����ֳ�
}

void CINFUnitState::SetState( DWORD dwState )
{
	m_dwState |= dwState;
	m_tBlinkTime.Start();
}

BOOL CINFUnitState::GetState( DWORD dwState )
{
	return ( m_dwState & dwState ) ? TRUE : FALSE;
}

void CINFUnitState::OffState( DWORD dwState )
{
	m_dwState &= ~dwState;
	m_dwInsertedState &= ~dwState;
}

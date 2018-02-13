// INFImageBtn.cpp: implementation of the CINFImageBtn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "Interface.h"


#include "INFImageBtn.h"
#include "INFImageList.h"		// 2011. 10. 10 by jskim UI�ý��� ����
#include "INFToolTip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFImageBtn::CINFImageBtn()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_BTN_STATUS;nCnt++)
	{
		m_pImageBtn[nCnt] = NULL;
	}	
	m_nBtnStatus = BTN_STATUS_UP;
	m_nBtnBlingStatus = BTN_STATUS_UP; // 2013-07-11 by bhsohn �Ƹ� �÷��� ������
	m_fPosX = m_fPosY = 0;
	m_fWidth =  m_fHeight = 1;

	m_bShowBtn = TRUE;

	m_bBlingBtn = 0;
	m_fBlingTime = 0;
	m_fCapBlingTime = 0;

	m_bClick = FALSE;
	m_bPush					= FALSE;

	// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
	memset(m_pToolTipType, 0x00, 512);
	memset(m_pToolTipDisable, 0x00, 512);
	// END 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����

	m_bBtnShowTooltip = FALSE; //2013-04-12 by ssjung �ɼ�â ���̽�ƽ ���� ���� ������� �ʴ� ���� ���� 

	// 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
	m_bLoopPlay = FALSE;
	m_bSelectBtn = FALSE;
	// END 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
}

CINFImageBtn::~CINFImageBtn()
{
	CINFImageBtn::DeleteDeviceObjects();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			HRESULT CINFImageBtn::InitDeviceObjects(char* i_szButtonName)
/// \brief		������ ��ư�� define�� �⺻���� ��ư �̸��� �Է¹޾� ��ư�� �����Ѵ�.
/// \author		dgwoo
/// \date		2007-10-23 ~ 2007-10-23
/// \warning	
// �������̽� ��ư ����
//#define BUTTON_STATE_UP			0
//#define BUTTON_STATE_DOWN			1
//#define BUTTON_STATE_DISABLE		2
//#define BUTTON_STATE_NORMAL		3
//
//#define BUTTON_STATE_NUMBER		4
//
///	
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
HRESULT CINFImageBtn::InitDeviceObjects(const char* i_szButtonName, char* pToolTipType)
{
	DataHeader*		pDataHeader = NULL;
	char			buf[32];
	DeleteDeviceObjects();

	strcpy(m_pToolTipType,pToolTipType);
	// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
	strncpy(m_pToolTipDisable, m_pToolTipType, 512);
	// END 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����

	{		
		wsprintf(buf,"%s3",i_szButtonName);
		pDataHeader = g_pGameMain->FindResource(buf);
		if(NULL == pDataHeader)
		{
			DeleteDeviceObjects();
			return S_FALSE;
		}
		if(NULL == m_pImageBtn[BTN_STATUS_UP])
		{
			m_pImageBtn[BTN_STATUS_UP] = new CINFImageEx;
		}
		m_pImageBtn[BTN_STATUS_UP]->InitDeviceObjects( pDataHeader );
	}

	{		
		wsprintf(buf,"%s1",i_szButtonName);
		pDataHeader = g_pGameMain->FindResource(buf);
		if(NULL == pDataHeader)
		{
			DeleteDeviceObjects();
			return S_FALSE;
		}
		if(NULL == m_pImageBtn[BTN_STATUS_DOWN])
		{
			m_pImageBtn[BTN_STATUS_DOWN] = new CINFImageEx;
		}	

		m_pImageBtn[BTN_STATUS_DOWN]->InitDeviceObjects( pDataHeader );		
	}

	{		
		wsprintf(buf,"%s0",i_szButtonName);
		pDataHeader = g_pGameMain->FindResource(buf);
		if(NULL == pDataHeader)
		{
			DeleteDeviceObjects();
			return S_FALSE;
		}
		if(NULL == m_pImageBtn[BTN_STATUS_SEL])
		{
			m_pImageBtn[BTN_STATUS_SEL] = new CINFImageEx;
		}
		m_pImageBtn[BTN_STATUS_SEL]->InitDeviceObjects( pDataHeader );		
	}

	{		
		wsprintf(buf,"%s2",i_szButtonName);
		pDataHeader = g_pGameMain->FindResource(buf);
		if(NULL == pDataHeader)
		{
			DeleteDeviceObjects();
			return S_FALSE;
		}
		if(NULL == m_pImageBtn[BTN_STATUS_DISABLE])
		{
			m_pImageBtn[BTN_STATUS_DISABLE] = new CINFImageEx;
		}	
		m_pImageBtn[BTN_STATUS_DISABLE]->InitDeviceObjects( pDataHeader );		
	}
	
	return S_OK;
}
HRESULT CINFImageBtn::InitDeviceObjects(const char* pBtup, char* pBtDown, char* pSel, char* pDisable, char* pToolTipType)
{
	DataHeader	* pDataHeader = NULL;
	DeleteDeviceObjects();

	strcpy(m_pToolTipType,pToolTipType);
	
	// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
	strncpy(m_pToolTipDisable, m_pToolTipType, 512);
	// END 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����

	{
		if(NULL == m_pImageBtn[BTN_STATUS_UP])
		{
			m_pImageBtn[BTN_STATUS_UP] = new CINFImageEx;
		}	
		
		pDataHeader = g_pGameMain->FindResource(pBtup);
		m_pImageBtn[BTN_STATUS_UP]->InitDeviceObjects( pDataHeader );		
	}

	{
		if(NULL == m_pImageBtn[BTN_STATUS_DOWN])
		{
			m_pImageBtn[BTN_STATUS_DOWN] = new CINFImageEx;
		}	
		
		pDataHeader = g_pGameMain->FindResource(pBtDown);
		m_pImageBtn[BTN_STATUS_DOWN]->InitDeviceObjects( pDataHeader );		
	}

	{
		if(NULL == m_pImageBtn[BTN_STATUS_SEL])
		{
			m_pImageBtn[BTN_STATUS_SEL] = new CINFImageEx;
		}	
		
		pDataHeader = g_pGameMain->FindResource(pSel);
		m_pImageBtn[BTN_STATUS_SEL]->InitDeviceObjects( pDataHeader );		
	}

	{
		if(NULL == m_pImageBtn[BTN_STATUS_DISABLE])
		{
			m_pImageBtn[BTN_STATUS_DISABLE] = new CINFImageEx;
		}	
		
		pDataHeader = g_pGameMain->FindResource(pDisable);
		m_pImageBtn[BTN_STATUS_DISABLE]->InitDeviceObjects( pDataHeader );		
	}
	
	return S_OK;
}

// ���۷����� �̹��� �ε�
HRESULT CINFImageBtn::InitDeviceObjects_LoadOp(char* pBtup, char* pBtDown, char* pSel, char* pDisable)
{
	DataHeader	* pDataHeader = NULL;
	DeleteDeviceObjects();
	{
		if(NULL == m_pImageBtn[BTN_STATUS_UP])
		{
			m_pImageBtn[BTN_STATUS_UP] = new CINFImageEx;
		}	
		
		pDataHeader = g_pGameMain->FindResource_LoadOp(pBtup);
		m_pImageBtn[BTN_STATUS_UP]->InitDeviceObjects( pDataHeader );
	}

	{
		if(NULL == m_pImageBtn[BTN_STATUS_DOWN])
		{
			m_pImageBtn[BTN_STATUS_DOWN] = new CINFImageEx;
		}	
		
		pDataHeader = g_pGameMain->FindResource_LoadOp(pBtDown);
		m_pImageBtn[BTN_STATUS_DOWN]->InitDeviceObjects( pDataHeader );
	}

	{
		if(NULL == m_pImageBtn[BTN_STATUS_SEL])
		{
			m_pImageBtn[BTN_STATUS_SEL] = new CINFImageEx;
		}	
		
		pDataHeader = g_pGameMain->FindResource_LoadOp(pSel);
		m_pImageBtn[BTN_STATUS_SEL]->InitDeviceObjects( pDataHeader );		
	}

	{
		if(NULL == m_pImageBtn[BTN_STATUS_DISABLE])
		{
			m_pImageBtn[BTN_STATUS_DISABLE] = new CINFImageEx;
		}	
		
		pDataHeader = g_pGameMain->FindResource_LoadOp(pDisable);
		m_pImageBtn[BTN_STATUS_DISABLE]->InitDeviceObjects( pDataHeader );		
	}
	
	return S_OK;
}

HRESULT CINFImageBtn::InitDeviceObjects_LoadSelect(char* pBtup, char* pBtDown, char* pSel, char* pDisable)
{
	DataHeader	* pDataHeader = NULL;
	DeleteDeviceObjects();
	{
		if(NULL == m_pImageBtn[BTN_STATUS_UP])
		{
			m_pImageBtn[BTN_STATUS_UP] = new CINFImageEx;
		}	
		
		pDataHeader = g_pInterface->FindResource_LoadSelect(pBtup);
		m_pImageBtn[BTN_STATUS_UP]->InitDeviceObjects( pDataHeader );		
	}

	{
		if(NULL == m_pImageBtn[BTN_STATUS_DOWN])
		{
			m_pImageBtn[BTN_STATUS_DOWN] = new CINFImageEx;
		}	
		
		pDataHeader = g_pInterface->FindResource_LoadSelect(pBtDown);
		m_pImageBtn[BTN_STATUS_DOWN]->InitDeviceObjects( pDataHeader );		
	}

	{
		if(NULL == m_pImageBtn[BTN_STATUS_SEL])
		{
			m_pImageBtn[BTN_STATUS_SEL] = new CINFImageEx;
		}	
		
		pDataHeader = g_pInterface->FindResource_LoadSelect(pSel);
		m_pImageBtn[BTN_STATUS_SEL]->InitDeviceObjects( pDataHeader );		
	}

	{
		if(NULL == m_pImageBtn[BTN_STATUS_DISABLE])
		{
			m_pImageBtn[BTN_STATUS_DISABLE] = new CINFImageEx;
		}	
		
		pDataHeader = g_pInterface->FindResource_LoadSelect(pDisable);
		m_pImageBtn[BTN_STATUS_DISABLE]->InitDeviceObjects( pDataHeader );		
	}
	
	return S_OK;
}


HRESULT CINFImageBtn::RestoreDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_BTN_STATUS;nCnt++)
	{
		if(m_pImageBtn[nCnt])
		{
			m_pImageBtn[nCnt]->RestoreDeviceObjects();
		}
	}
	
	return S_OK;
}

HRESULT CINFImageBtn::DeleteDeviceObjects()
{
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_BTN_STATUS;nCnt++)
	{
		if(m_pImageBtn[nCnt])
		{
			m_pImageBtn[nCnt]->DeleteDeviceObjects();	
			util::del(m_pImageBtn[nCnt]);
		}
	}	
	return S_OK;
}
HRESULT CINFImageBtn::InvalidateDeviceObjects()
{	
	int nCnt = 0;
	for(nCnt = 0;nCnt < MAX_BTN_STATUS;nCnt++)
	{
		m_pImageBtn[nCnt]->InvalidateDeviceObjects();
	}
	
	return S_OK;
}

void CINFImageBtn::Render()
{
	if(FALSE == IsShowWindow())
	{
		return ;
	}	
	// 2013-07-11 by bhsohn �Ƹ� �÷��� ������
	if(m_bBlingBtn ) 
	{
		m_fBlingTime -= g_pD3dApp->GetCheckElapsedTime();
		if(m_fBlingTime < 0)
		{
			m_fBlingTime = m_fCapBlingTime;
			switch(m_nBtnBlingStatus)
			{
			case BTN_STATUS_UP:
				{
					// 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
//					m_nBtnBlingStatus = BTN_STATUS_DOWN;
					m_nBtnBlingStatus = BTN_STATUS_SEL;
				}
				break;
			case BTN_STATUS_DOWN:
				{
					m_nBtnBlingStatus = BTN_STATUS_UP;
				}
				break;				
				// 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
			case BTN_STATUS_SEL:
				{
					m_nBtnBlingStatus = BTN_STATUS_UP;
				}
				break;
				// END 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
			default:
				{
					// 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
					if(!m_bLoopPlay)
					{
						m_bBlingBtn = FALSE;
					}
					else
					{
						m_nBtnBlingStatus = BTN_STATUS_SEL;
					}
					// END 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��					
				}
				break;
			}
		}
		if(BTN_STATUS_DOWN != m_nBtnStatus)
		{
			if(!m_bSelectBtn)
			{
				m_nBtnStatus = m_nBtnBlingStatus;
			}
			else
			{
				m_nBtnStatus =BTN_STATUS_SEL;
				
			}
		}
		// END 2013-07-11 by bhsohn �Ƹ� �÷��� ������
	}
	m_pImageBtn[m_nBtnStatus]->Move(m_fPosX, m_fPosY);
	m_pImageBtn[m_nBtnStatus]->Render();	
}

void CINFImageBtn::SetBtnPosition(float i_fPosX, float i_fPosY)
{
	POINT ptGoCityBtn;
	ptGoCityBtn = GetImgSize();
	m_fPosX		= i_fPosX;
	m_fPosY		= i_fPosY;
	m_fWidth	= ptGoCityBtn.x;
	m_fHeight	= ptGoCityBtn.y;
}

POINT CINFImageBtn::GetImgSize()
{
	return m_pImageBtn[m_nBtnStatus]->GetImgSize();
}

// ��ư ���� ���콺�� �ֳ�?
BOOL CINFImageBtn::IsMouseOverlab(POINT ptPos)
{
	if((m_fPosX <= ptPos.x) &&(ptPos.x <= (m_fPosX + m_fWidth * m_pImageBtn[m_nBtnStatus]->GetAniScale().x )))		
	{
		if((m_fPosY <= ptPos.y)	&&( ptPos.y <= (m_fPosY + m_fHeight * m_pImageBtn[m_nBtnStatus]->GetAniScale().y ) ))
		{
			// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����	
// 			if(g_pInterface->m_pToolTip)
// 				g_pInterface->m_pToolTip->SetNameToolTip(ptPos,m_pToolTipType);
			if(g_pInterface->m_pToolTip)
			{
				m_bBtnShowTooltip = TRUE;		//2013-04-12 by ssjung �ɼ�â ���̽�ƽ ���� ���� ������� �ʴ� ���� ���� 
				if(BTN_STATUS_DISABLE != m_nBtnStatus)
				{
					g_pInterface->m_pToolTip->SetNameToolTip(ptPos,m_pToolTipType);
				}
				else
				{
					g_pInterface->m_pToolTip->SetNameToolTip(ptPos,m_pToolTipDisable);
				}
			}
			// END 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����	

			return TRUE;
		}
	}
//2013-04-12 by ssjung �ɼ�â ���̽�ƽ ���� ���� ������� �ʴ� ���� ���� 
	if(m_bBtnShowTooltip && g_pInterface->m_pToolTip)
	{
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	}
	m_bBtnShowTooltip = FALSE;
//end 2013-04-12 by ssjung �ɼ�â ���̽�ƽ ���� ���� ������� �ʴ� ���� ���� 
	
	return FALSE;
}
// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
void CINFImageBtn::SetTooltipType(char *pToolTipType, char *pToolTipDisable)
{
	strncpy(m_pToolTipType, pToolTipType, 512);
	strncpy(m_pToolTipDisable, pToolTipDisable, 512);
}

BOOL CINFImageBtn::OnMouseMove(POINT ptPos)
{
	if(FALSE == IsShowWindow())
	{
		return FALSE;
	}

	m_bSelectBtn = FALSE; // 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��

	if(BTN_STATUS_DISABLE == m_nBtnStatus)
	{
		IsMouseOverlab(ptPos);
		// Disalbe��ư 
		return FALSE;
	}
	if(IsMouseOverlab(ptPos))
	{			
		// 2007-09-05 by bhsohn ���� ������
		if(BTN_STATUS_DOWN != m_nBtnStatus)
		{
			// ���콺 ���� ��ư�� �ִ�.
			if(m_nBtnStatus != BTN_STATUS_SEL )
			{
				// 2011. 10. 10 by jskim UI�ý��� ����
				for(int i=0; i < MAX_BTN_STATUS; i++)
				{
					if(m_pImageBtn[i]->GetAniType() != -1 )
					{
						m_pImageBtn[ i ]->Begin();
					}					
				}
			}
				// end 2011. 10. 10 by jskim UI�ý��� ����			
			m_nBtnStatus = BTN_STATUS_SEL;

			// 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��			
			m_bSelectBtn = TRUE; 			
			// END 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
		}
		// end 2007-09-05 by bhsohn ���� ������
		return TRUE;
	}
	// 2008-05-13 by dgwoo Push botton�̸� ���¸� �������� �ʴ´�.
// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����	
	if( m_nBtnStatus == BTN_STATUS_SEL && g_pInterface->m_pToolTip)
	{
		g_pInterface->m_pToolTip->m_bToolTipState = FALSE;
	}
// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����	
	if(m_bPush)
	{
		return FALSE;
	}

	if(FALSE == m_bBlingBtn )
	{
		m_nBtnStatus = BTN_STATUS_UP;
	}
	return FALSE;
}
BOOL CINFImageBtn::OnLButtonDown(POINT ptPos)
{
	if(FALSE == IsShowWindow())
	{
		return FALSE;
	}
	
	if(IsMouseOverlab(ptPos))
	{
		if(BTN_STATUS_DISABLE == m_nBtnStatus)
		{
			// Disalbe��ư 
			return TRUE;
		}
		m_bClick = TRUE;
		
		// 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
		if(!m_bLoopPlay)
		{
			m_bBlingBtn = FALSE;
		}
		// 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
		// ���콺 ���� ��ư�� �ִ�.
		m_nBtnStatus = BTN_STATUS_DOWN;
		return TRUE;
	}	
	m_bClick = FALSE;
	return FALSE;
}

BOOL CINFImageBtn::OnLButtonUp(POINT ptPos)
{
	if(FALSE == IsShowWindow())
	{
		return FALSE;
	}
	BOOL bTmpClick = m_bClick;
	m_bClick = FALSE;

	if((BTN_STATUS_DOWN != m_nBtnStatus)
		&&(BTN_STATUS_SEL != m_nBtnStatus))
	{				
		return FALSE;
	}
	if(FALSE == m_bBlingBtn )
	{
		m_nBtnStatus = BTN_STATUS_UP;	
	}
	// 2013-07-11 by bhsohn �Ƹ� �÷��� ������
	else 
	{
		m_nBtnStatus = m_nBtnBlingStatus;
	}
	// END 2013-07-11 by bhsohn �Ƹ� �÷��� ������
	if(IsMouseOverlab(ptPos))
	{		
		if(bTmpClick)
		{			
			// ���콺 ���� ��ư�� �ִ�.			
		
			return TRUE;
		}
	}	
	return FALSE;
}

void CINFImageBtn::ShowWindow(BOOL bShow)
{
	m_bShowBtn = bShow;
}
BOOL CINFImageBtn::IsShowWindow()
{
	return m_bShowBtn;
}
// ��ư ������ ���� ����
void CINFImageBtn::SetBtnBlingBling(BOOL bSet, float fBlingTime)
{
	m_bBlingBtn = bSet;
	m_fCapBlingTime = fBlingTime;
	m_fBlingTime = fBlingTime;
}

void CINFImageBtn::PushButton(BOOL i_bPush)
{
	if(m_nBtnStatus == BTN_STATUS_DISABLE)
	{
		return;
	}
	m_bPush = i_bPush;	
	if(m_bPush)
	{
		m_nBtnStatus = BTN_STATUS_DOWN;
	}
	else
	{
		m_nBtnStatus = BTN_STATUS_UP;
	}

}
void CINFImageBtn::EnableBtn(BOOL bEnable)
{
	if(!bEnable)
	{
		m_nBtnStatus = BTN_STATUS_DISABLE;
	}
	else
	{
		m_nBtnStatus = BTN_STATUS_UP;
	}
}

// 2008-11-13 by bhsohn ���̽�ƽ �۾�
void CINFImageBtn::SetBtnState(int i_nBtnStatus)
{
	if(i_nBtnStatus < 0 || i_nBtnStatus >= MAX_BTN_STATUS)
	{
		return;
	}
	else if(NULL == m_pImageBtn[i_nBtnStatus])
	{
		return;
	}
	m_nBtnStatus = i_nBtnStatus;
}

// 2011. 10. 10 by jskim UI�ý��� ����
void CINFImageBtn::Tick()
{
	for(int i=0; i < MAX_BTN_STATUS; i++)
	{
		if(m_pImageBtn[ i ])
			m_pImageBtn[ i ]->Tick();
	}
// 	if(strcmp( m_pToolTipType, "STRTOOLTIP0" ))
// 	{
// 		POINT point;
// 		GetCursorPos( &point );
// 		ScreenToClient( g_pD3dApp->GetHwnd(), &point );
// 		IsMouseOverlab(point);	
// 	}
}

void CINFImageBtn::SetAnimation( int nType, int nState, BOOL bLoop, float fPlayTime, float fScaleX, float fScaleY )
{
	m_pImageBtn[ nState ]->SetLoop( bLoop );
	switch( nType )
	{
	case UVANI_TYPE:
		{

		}
		break;
	case SCALE_TYPE:
		{
			m_pImageBtn[ nState ]->SetScaleAnimation( SCALE_TYPE, fPlayTime, fScaleX, fScaleY );
		}
		break;
	}
}

void CINFImageBtn::SetAniScale( float fScaleX, float fScaleY )
{
	for( int i=0; i < MAX_BTN_STATUS; i++ )
	{
		m_pImageBtn[ i ]->SetAniScale( fScaleX, fScaleY );
	}
}

void CINFImageBtn::SetLineUp( int nLine )
{
	for(int i=0; i < MAX_BTN_STATUS; i++)
	{
		m_pImageBtn[ i ]->SetLine( nLine );
	}	
}
// end 2011. 10. 10 by jskim UI�ý��� ����

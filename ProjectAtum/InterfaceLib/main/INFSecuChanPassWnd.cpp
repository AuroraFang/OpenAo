// INFSecuChanPassWnd.cpp: implementation of the CINFSecuChanPassWnd class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "AtumSound.h"
#include "INFSecuMain.h"

#include "INFSecuChanPassWnd.h"
#include "INFGroupImage.h"
#include "INFGroupManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	SECURITY_CHANGE_WND_TITLEBAR_HEIGHT		20

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
// OK ��ư 
#define		SECURITY_CHANGE_WND_OK_X			150
#define		SECURITY_CHANGE_WND_OK_Y			166

// Cancel ��ư 
#define		SECURITY_CHANGE_WND_CANCEL_X		185
#define		SECURITY_CHANGE_WND_CANCEL_Y		166

// Edit ��Ʈ�� (Pass)
#define		SECURITY_CHANGE_WND_PASS_EDIT_X		22
#define		SECURITY_CHANGE_WND_PASS_EDIT_Y		51
#define		SECURITY_CHANGE_WND_PASS_EDIT_W		200
#define		SECURITY_CHANGE_WND_PASS_EDIT_H		23

// Edit ��Ʈ�� (Change)
#define		SECURITY_CHANGE_WND_CHANGE_EDIT_X		22
#define		SECURITY_CHANGE_WND_CHANGE_EDIT_Y		97
#define		SECURITY_CHANGE_WND_CHANGE_EDIT_W		200
#define		SECURITY_CHANGE_WND_CHANGE_EDIT_H		23

// Edit ��Ʈ�� (Ȯ��)
#define		SECURITY_CHANGE_WND_CONFIRM_EDIT_X		22
#define		SECURITY_CHANGE_WND_CONFIRM_EDIT_Y		143
#define		SECURITY_CHANGE_WND_CONFIRM_EDIT_W		200
#define		SECURITY_CHANGE_WND_CONFIRM_EDIT_H		23

#else
// OK ��ư 
#define		SECURITY_CHANGE_WND_OK_X			135
#define		SECURITY_CHANGE_WND_OK_Y			172

// Cancel ��ư 
#define		SECURITY_CHANGE_WND_CANCEL_X		180
#define		SECURITY_CHANGE_WND_CANCEL_Y		172

// Edit ��Ʈ�� (Pass)
#define		SECURITY_CHANGE_WND_PASS_EDIT_X		19
#define		SECURITY_CHANGE_WND_PASS_EDIT_Y		48
#define		SECURITY_CHANGE_WND_PASS_EDIT_W		200
#define		SECURITY_CHANGE_WND_PASS_EDIT_H		20

// Edit ��Ʈ�� (Change)
#define		SECURITY_CHANGE_WND_CHANGE_EDIT_X		19
#define		SECURITY_CHANGE_WND_CHANGE_EDIT_Y		94
#define		SECURITY_CHANGE_WND_CHANGE_EDIT_W		200
#define		SECURITY_CHANGE_WND_CHANGE_EDIT_H		20

// Edit ��Ʈ�� (Ȯ��)
#define		SECURITY_CHANGE_WND_CONFIRM_EDIT_X		19
#define		SECURITY_CHANGE_WND_CONFIRM_EDIT_Y		139
#define		SECURITY_CHANGE_WND_CONFIRM_EDIT_W		200
#define		SECURITY_CHANGE_WND_CONFIRM_EDIT_H		20


#endif

// �ִ� �۾� �� 
#define		SECURITY_WND_MAX_STRING_LEN					8

// � ��Ʈ���� �����Ҳ���?
#define		SECURITY_CHANGE_WND_PASS_EDIT				0
#define		SECURITY_CHANGE_WND_CHANGE_EDIT				1
#define		SECURITY_CHANGE_WND_CONFIRM_EDIT			2


CINFSecuChanPassWnd::CINFSecuChanPassWnd(CINFSecuMain* pMain)
{
	m_pINFSecuMain = pMain;

	m_pSetPassBk = NULL;	

	m_nBackPosX = m_nBackPosY= 0;
	m_nCx = m_nCy = 1;

	m_bMouseLock = FALSE;
	m_ptMouseLockCap.x = m_ptMouseLockCap.y= 0;

	m_pSetOk = NULL;
	m_pSetCancel = NULL;
	m_pINFPassEditBox = NULL;
	m_pINFSecuChangeEditBox = NULL;
	m_pINFSecuComfirmEditBox= NULL;

	m_nSelEdit = SECURITY_CHANGE_WND_PASS_EDIT;

}

CINFSecuChanPassWnd::~CINFSecuChanPassWnd()
{
	CINFSecuChanPassWnd::DeleteDeviceObjects();
}


HRESULT CINFSecuChanPassWnd::InitDeviceObjects()
{
	DeleteDeviceObjects();
	char buf[30] ;
	memset(buf, 0x00, 30);

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	{
		DataHeader	* pDataHeader;
		pDataHeader = g_pGameMain->m_GruopImagemanager->FindResource("changePW");
		m_pSetPassBk = g_pGameMain->m_GruopImagemanager->GetGroupImage( pDataHeader );
		m_pSetPassBk->InitDeviceObjects( g_pD3dApp->m_pImageList );
	}
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "oks03");
		wsprintf(szDownBtn, "oks01");
		wsprintf(szSelBtn, "oks00");
		wsprintf(szDisBtn, "oks02");
		if(NULL == m_pSetOk)
		{
			m_pSetOk = new CINFImageBtn;
		}
		m_pSetOk->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "cans03");
		wsprintf(szDownBtn, "cans01");
		wsprintf(szSelBtn, "cans00");
		wsprintf(szDisBtn, "cans02");
		if(NULL == m_pSetCancel)
		{
			m_pSetCancel = new CINFImageBtn;
		}
		m_pSetCancel->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#else
	{
		if(NULL == m_pSetPassBk)
		{
			m_pSetPassBk = new CINFImageEx;					   // 2011. 10. 10 by jskim UI�ý��� ����
		}
		wsprintf(buf,"se_pchbk");
		pDataHeader = g_pGameMain->FindResource(buf);
		m_pSetPassBk->InitDeviceObjects(pDataHeader);		
	}
	{	
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "shlaok00");
		wsprintf(szDownBtn, "shlaok01");
		wsprintf(szSelBtn, "shlaok03");
		wsprintf(szDisBtn, "shlaok02");
		if(NULL == m_pSetOk)
		{
			m_pSetOk = new CINFImageBtn;
		}
		m_pSetOk->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
	
	{
		char szUpBtn[30], szDownBtn[30], szSelBtn[30], szDisBtn[30];		
		wsprintf(szUpBtn, "shmcan00");
		wsprintf(szDownBtn, "shmcan01");
		wsprintf(szSelBtn, "shmcan03");
		wsprintf(szDisBtn, "shmcan02");
		if(NULL == m_pSetCancel)
		{
			m_pSetCancel = new CINFImageBtn;
		}
		m_pSetCancel->InitDeviceObjects(szUpBtn, szDownBtn, szSelBtn, szDisBtn);		
	}
#endif

	
	{
		if(NULL == m_pINFPassEditBox)
		{
			m_pINFPassEditBox = new CINFSecuEditBox;
		}
		
		POINT ptPos = {SECURITY_CHANGE_WND_PASS_EDIT_X, SECURITY_CHANGE_WND_PASS_EDIT_Y};
		m_pINFPassEditBox->InitDeviceObjects(9, ptPos, SECURITY_CHANGE_WND_PASS_EDIT_W, TRUE, SECURITY_CHANGE_WND_PASS_EDIT_H);		
	}

	{
		if(NULL == m_pINFSecuChangeEditBox)
		{
			m_pINFSecuChangeEditBox = new CINFSecuEditBox;
		}
		
		POINT ptPos = {SECURITY_CHANGE_WND_CHANGE_EDIT_X, SECURITY_CHANGE_WND_CHANGE_EDIT_Y};
		m_pINFSecuChangeEditBox->InitDeviceObjects(9, ptPos, SECURITY_CHANGE_WND_CHANGE_EDIT_W, TRUE, SECURITY_CHANGE_WND_CHANGE_EDIT_H);		
	}
	
	{
		if(NULL == m_pINFSecuComfirmEditBox)
		{
			m_pINFSecuComfirmEditBox = new CINFSecuEditBox;
		}
		
		POINT ptPos = {SECURITY_CHANGE_WND_CONFIRM_EDIT_X, SECURITY_CHANGE_WND_CONFIRM_EDIT_Y};
		m_pINFSecuComfirmEditBox->InitDeviceObjects(9, ptPos, SECURITY_CHANGE_WND_CONFIRM_EDIT_W, TRUE, SECURITY_CHANGE_WND_CONFIRM_EDIT_H);		
	}
	
	m_pINFPassEditBox->EnableEdit(TRUE);
	m_nSelEdit = SECURITY_CHANGE_WND_PASS_EDIT;

	m_pINFPassEditBox->SetMaxStringLen(SECURITY_WND_MAX_STRING_LEN);
	m_pINFSecuChangeEditBox->SetMaxStringLen(SECURITY_WND_MAX_STRING_LEN);
	m_pINFSecuComfirmEditBox->SetMaxStringLen(SECURITY_WND_MAX_STRING_LEN);

	return S_OK ;
}

HRESULT CINFSecuChanPassWnd::RestoreDeviceObjects()
{
	{	
		m_pSetPassBk->RestoreDeviceObjects();		
		POINT ptBk = m_pSetPassBk->GetImgSize();
				
		if(g_pD3dApp->GetBackBufferDesc().Width > ptBk.x)
		{
			m_nBackPosX = (g_pD3dApp->GetBackBufferDesc().Width/2)-(ptBk.x/2);
		}
		if(g_pD3dApp->GetBackBufferDesc().Height > ptBk.y)
		{
			m_nBackPosY = (g_pD3dApp->GetBackBufferDesc().Height/2)-(ptBk.y/2);
		}
		m_nCx = ptBk.x;
		m_nCy = ptBk.y;
	}
	{
		m_pSetOk->RestoreDeviceObjects();
		m_pSetCancel->RestoreDeviceObjects();
		m_pINFPassEditBox->RestoreDeviceObjects();		
		m_pINFSecuChangeEditBox->RestoreDeviceObjects();		
		m_pINFSecuComfirmEditBox->RestoreDeviceObjects();		
	}
	// ��ư ��ġ ������Ʈ 
	UpdateBtnPos(m_nBackPosX, m_nBackPosY);	
	
	return S_OK ;
}

HRESULT CINFSecuChanPassWnd::DeleteDeviceObjects()
{
	if(m_pSetPassBk)
	{
		m_pSetPassBk->DeleteDeviceObjects();	
		util::del(m_pSetPassBk);
	}
	if(m_pSetOk)
	{
		m_pSetOk->DeleteDeviceObjects();	
		util::del(m_pSetOk);
	}
	if(m_pSetCancel)
	{
		m_pSetCancel->DeleteDeviceObjects();	
		util::del(m_pSetCancel);
	}
	
	if(m_pINFPassEditBox)
	{
		m_pINFPassEditBox->DeleteDeviceObjects();	
		util::del(m_pINFPassEditBox);
	}
	if(m_pINFSecuChangeEditBox)
	{
		m_pINFSecuChangeEditBox->DeleteDeviceObjects();	
		util::del(m_pINFSecuChangeEditBox);
	}
	
	if(m_pINFSecuComfirmEditBox)
	{
		m_pINFSecuComfirmEditBox->DeleteDeviceObjects();	
		util::del(m_pINFSecuComfirmEditBox);
	}
	
	return S_OK ;
}

HRESULT CINFSecuChanPassWnd::InvalidateDeviceObjects()
{
	m_pSetPassBk->InvalidateDeviceObjects();
	m_pSetOk->InvalidateDeviceObjects();		
	m_pSetCancel->InvalidateDeviceObjects();		
	m_pINFPassEditBox->InvalidateDeviceObjects();		
	m_pINFSecuChangeEditBox->InvalidateDeviceObjects();		
	m_pINFSecuComfirmEditBox->InvalidateDeviceObjects();		
	return S_OK ;
}

void	CINFSecuChanPassWnd::Render()
{
	{
		m_pSetPassBk->Move(m_nBackPosX,m_nBackPosY);
		m_pSetPassBk->Render();
	}
	m_pSetOk->Render();
	m_pSetCancel->Render();
	{	
		m_pINFPassEditBox->Render();
		m_pINFSecuChangeEditBox->Render();
		m_pINFSecuComfirmEditBox->Render();
	}
}
void	CINFSecuChanPassWnd::Tick()
{
	m_pINFPassEditBox->Tick();
	m_pINFSecuChangeEditBox->Tick();
	m_pINFSecuComfirmEditBox->Tick();
}

int CINFSecuChanPassWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(wParam, lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		{			
			switch(m_nSelEdit)
			{
			case SECURITY_CHANGE_WND_PASS_EDIT:
				{					
					if(m_pINFPassEditBox->WndProc(uMsg, wParam, lParam))
					{
						return INF_MSGPROC_BREAK;
					}

				}
				break;
			case SECURITY_CHANGE_WND_CHANGE_EDIT:
				{
					if(m_pINFSecuChangeEditBox->WndProc(uMsg, wParam, lParam))
					{
						return INF_MSGPROC_BREAK;
					}
				}
				break;
			case SECURITY_CHANGE_WND_CONFIRM_EDIT:
				{
					if(m_pINFSecuComfirmEditBox->WndProc(uMsg, wParam, lParam))
					{
						return INF_MSGPROC_BREAK;
					}
				}
				break;
			}			
		}
		break;
	}
	return INF_MSGPROC_NORMAL;		
}

int CINFSecuChanPassWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	CheckMouseReverse(&pt);
	if((m_nBackPosX < pt.x && pt.x < (m_nBackPosX+m_nCx)  )
		&&(m_nBackPosY < pt.y && pt.y < (m_nBackPosY+m_nCy)  ))
	{
	}
	else
	{
		return INF_MSGPROC_NORMAL;
	}

		
	{
		if(TRUE == m_pINFPassEditBox->OnLButtonDown(pt))
		{				
			RefreshEditCtl(SECURITY_CHANGE_WND_PASS_EDIT, m_pINFPassEditBox);			
			
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}
	}
	{
		if(TRUE == m_pINFSecuChangeEditBox->OnLButtonDown(pt))
		{				
			RefreshEditCtl(SECURITY_CHANGE_WND_CHANGE_EDIT, m_pINFSecuChangeEditBox);			
			
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{		
		if(TRUE == m_pINFSecuComfirmEditBox->OnLButtonDown(pt))
		{			
			RefreshEditCtl(SECURITY_CHANGE_WND_CONFIRM_EDIT, m_pINFSecuComfirmEditBox);						
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}
	}

	{
		if(TRUE == m_pSetOk->OnLButtonDown(pt))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	{
		if(TRUE == m_pSetCancel->OnLButtonDown(pt))
		{
			// ��ư���� ���콺�� �ִ�.
			return  INF_MSGPROC_BREAK;
		}		
	}
	
	if(pt.y < (m_nBackPosY+SECURITY_CHANGE_WND_TITLEBAR_HEIGHT) )
	{
		m_bMouseLock = TRUE;
		
		m_ptMouseLockCap.x = pt.x - m_nBackPosX;
		m_ptMouseLockCap.y = pt.y - m_nBackPosY;	
	}

	return INF_MSGPROC_BREAK;	
}
int CINFSecuChanPassWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	m_pSetOk->OnMouseMove(pt);	
	m_pSetCancel->OnMouseMove(pt);	


	if(FALSE == m_bMouseLock )
	{
		return INF_MSGPROC_NORMAL;
	}

	m_nBackPosX = pt.x - m_ptMouseLockCap.x;
	m_nBackPosY = pt.y - m_ptMouseLockCap.y;

	// X ��ǥ ����
	if(m_nBackPosX < 0)
	{
		m_nBackPosX = 0;
	}
	else if((m_nBackPosX+m_nCx) > g_pD3dApp->GetBackBufferDesc().Width)
	{
		m_nBackPosX = g_pD3dApp->GetBackBufferDesc().Width - m_nCx;
	}
	
	// Y ��ǥ ����
	if(m_nBackPosY < 0)
	{
		m_nBackPosY = 0;
	}
	else if((m_nBackPosY+m_nCy) > g_pD3dApp->GetBackBufferDesc().Height)
	{
		m_nBackPosY = g_pD3dApp->GetBackBufferDesc().Height - m_nCy;
	}	

	// ��ư ��ġ ������Ʈ 
	UpdateBtnPos(m_nBackPosX, m_nBackPosY);	

	return INF_MSGPROC_BREAK;	
}
void CINFSecuChanPassWnd::UpdateBtnPos(int nBackPosX, int nBackPosY)
{
 	int nPosX, nPosY;
	{
		nPosX = nBackPosX + SECURITY_CHANGE_WND_OK_X;
		nPosY = nBackPosY + SECURITY_CHANGE_WND_OK_Y;		
		m_pSetOk->SetBtnPosition(nPosX, nPosY);
	}
	{
		nPosX = nBackPosX + SECURITY_CHANGE_WND_CANCEL_X;
		nPosY = nBackPosY + SECURITY_CHANGE_WND_CANCEL_Y;		
		m_pSetCancel->SetBtnPosition(nPosX, nPosY);
	}
	
	{
		nPosX = nBackPosX + SECURITY_CHANGE_WND_PASS_EDIT_X;
		nPosY = nBackPosY + SECURITY_CHANGE_WND_PASS_EDIT_Y;
		m_pINFPassEditBox->SetPos(nPosX, nPosY);
	}
	{
		nPosX = nBackPosX + SECURITY_CHANGE_WND_CHANGE_EDIT_X;
		nPosY = nBackPosY + SECURITY_CHANGE_WND_CHANGE_EDIT_Y;
		m_pINFSecuChangeEditBox->SetPos(nPosX, nPosY);
	}
	{		
		nPosX = nBackPosX + SECURITY_CHANGE_WND_CONFIRM_EDIT_X;
		nPosY = nBackPosY + SECURITY_CHANGE_WND_CONFIRM_EDIT_Y;
		m_pINFSecuComfirmEditBox->SetPos(nPosX, nPosY);
	}
}

int CINFSecuChanPassWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);

	{
		if(TRUE == m_pSetOk->OnLButtonUp(pt))
		{				
			OnClickOk();

			// ��ư Ŭ�� 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}
	
	{
		if(TRUE == m_pSetCancel->OnLButtonUp(pt))
		{			
			OnClickCancel();

			// ��ư Ŭ�� 			
			g_pD3dApp->m_pSound->PlayD3DSound(SOUND_SELECT_BUTTON, D3DXVECTOR3(0,0,0), FALSE);			
			return  INF_MSGPROC_BREAK;
		}
	}

	if(FALSE == m_bMouseLock )
	{
		return INF_MSGPROC_NORMAL;
	}
	m_bMouseLock = FALSE;
	return INF_MSGPROC_BREAK;	
}

// �ݱ� ��ư Ŭ��
void CINFSecuChanPassWnd::OnCloseInfWnd()
{	
	m_pINFPassEditBox->InitString();		
	m_pINFPassEditBox->EnableEdit(FALSE);

	m_pINFSecuChangeEditBox->InitString();		
	m_pINFSecuChangeEditBox->EnableEdit(FALSE);

	m_pINFSecuComfirmEditBox->InitString();		
	m_pINFSecuComfirmEditBox->EnableEdit(FALSE);
}

// Show Pass ������ 
void CINFSecuChanPassWnd::ShowSetPassWnd()
{	
	OnCloseInfWnd();
	m_nSelEdit = SECURITY_CHANGE_WND_PASS_EDIT;
	m_pINFPassEditBox->EnableEdit(TRUE);	
}

// Ok��ư Ŭ�� 
void CINFSecuChanPassWnd::OnClickOk()
{
	char chEdit[512], chChangeEdit[512], chComfirmEdit[512];
	memset(chEdit, 0x00, 512);
	memset(chChangeEdit, 0x00, 512);	
	memset(chComfirmEdit, 0x00, 512);

	m_pINFPassEditBox->GetString(chEdit, 512);
	m_pINFSecuChangeEditBox->GetString(chChangeEdit, 512);	
	m_pINFSecuComfirmEditBox->GetString(chComfirmEdit, 512);

	if((0 == strlen(chEdit))
		||(0 == strlen(chChangeEdit))
		||(0 == strlen(chComfirmEdit)))
	{
		g_pGameMain->CreateChatChild(STRMSG_C_070917_0203, COLOR_ERROR);// "\\y���� ��й�ȣ�� �߸� �Է��ϼ̽��ϴ�."
		return;
	}

	if(0 == strncmp(chChangeEdit, chComfirmEdit, 512))
	{	// �ٲٱ� �ϰ� Ȯ���ϰ� ����. 		
		m_pINFSecuMain->ChangeSecurityPassword(chEdit, chChangeEdit);
	}
	else
	{
		g_pGameMain->CreateChatChild(STRMSG_C_070430_0101, COLOR_ERROR);// "��й�ȣ�� Ʋ���ϴ�."
	}
}

void CINFSecuChanPassWnd::OnClickCancel()
{
	m_pINFSecuMain->ShowSecurityWnd(SECURITY_MODE_SETUP_CHANGE_PASS, FALSE);	
}

void CINFSecuChanPassWnd::RefreshEditCtl(int nEditMode, CINFSecuEditBox* pIEdit)
{
	{
		m_pINFPassEditBox->EnableEdit(FALSE);
		m_pINFSecuChangeEditBox->EnableEdit(FALSE);
		m_pINFSecuComfirmEditBox->EnableEdit(FALSE);
	}
	pIEdit->EnableEdit(TRUE);
	m_nSelEdit = nEditMode;
}
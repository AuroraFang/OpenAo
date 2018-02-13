// INFSecuMain.cpp: implementation of the CINFSecuMain class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFGameMain.h"
#include "md5_lib_src.h"

#include "INFSecuMain.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFSecuMain::CINFSecuMain()
{
	m_pINFSecuLockWnd = NULL;
	m_pINFSecuSetPassWnd = NULL;
	m_pINFSecuChangePassWnd = NULL;
	m_pINFSecuPassWnd = NULL;
	m_nShowWnd = 0;
	m_nPassChangeMode = -1;
	// 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
	m_nCnt = 0;
	// end 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
}

CINFSecuMain::~CINFSecuMain()
{
	CINFSecuMain::DeleteDeviceObjects();
}

HRESULT CINFSecuMain::InitDeviceObjects()
{
	DeleteDeviceObjects();
	
	{
		m_pINFSecuLockWnd = new CINFSecuLockWnd(this);
		m_pINFSecuLockWnd->InitDeviceObjects();		
	}
	{
		m_pINFSecuSetPassWnd = new CINFSecuSetPassWnd(this);
		m_pINFSecuSetPassWnd->InitDeviceObjects();		
	}
	
	{
		m_pINFSecuChangePassWnd= new CINFSecuChanPassWnd(this);
		m_pINFSecuChangePassWnd->InitDeviceObjects();		
	}
	
	{
		m_pINFSecuPassWnd= new CINFSecuPassWnd(this);
		m_pINFSecuPassWnd->InitDeviceObjects();		
	}
	return S_OK ;
}

HRESULT CINFSecuMain::RestoreDeviceObjects()
{
	m_pINFSecuLockWnd->RestoreDeviceObjects();
	m_pINFSecuSetPassWnd->RestoreDeviceObjects();
	m_pINFSecuChangePassWnd->RestoreDeviceObjects();
	m_pINFSecuPassWnd->RestoreDeviceObjects();
	return S_OK ;
}

HRESULT CINFSecuMain::DeleteDeviceObjects()
{
	if(m_pINFSecuLockWnd)
	{
		m_pINFSecuLockWnd->DeleteDeviceObjects();
		util::del(m_pINFSecuLockWnd);
	}
	if(m_pINFSecuSetPassWnd)
	{
		m_pINFSecuSetPassWnd->DeleteDeviceObjects();
		util::del(m_pINFSecuSetPassWnd);
	}
	if(m_pINFSecuChangePassWnd)
	{
		m_pINFSecuChangePassWnd->DeleteDeviceObjects();
		util::del(m_pINFSecuChangePassWnd);
	}
	
	if(m_pINFSecuPassWnd)
	{
		m_pINFSecuPassWnd->DeleteDeviceObjects();
		util::del(m_pINFSecuPassWnd);
	}
	return S_OK ;
}

HRESULT CINFSecuMain::InvalidateDeviceObjects()
{
	m_pINFSecuLockWnd->InvalidateDeviceObjects();
	m_pINFSecuSetPassWnd->InvalidateDeviceObjects();
	m_pINFSecuChangePassWnd->InvalidateDeviceObjects();
	m_pINFSecuPassWnd->InvalidateDeviceObjects();
	return S_OK ;
}

void	CINFSecuMain::Render()
{
	if(IsShowWnd(SECURITY_MODE_LOCK))
	{
		m_pINFSecuLockWnd->Render();
	}
	if(IsShowWnd(SECURITY_MODE_SETUP_PASSWORD))
	{
		m_pINFSecuSetPassWnd->Render();
	}
	if(IsShowWnd(SECURITY_MODE_SETUP_CHANGE_PASS))
	{
		m_pINFSecuChangePassWnd->Render();
	}
	if(IsShowWnd(SECURITY_MODE_PASS_WND))
	{
		m_pINFSecuPassWnd->Render();
	}
	
}
void	CINFSecuMain::Tick()
{
	if(IsShowWnd(SECURITY_MODE_LOCK))
	{
		m_pINFSecuLockWnd->Tick();
	}
	if(IsShowWnd(SECURITY_MODE_SETUP_PASSWORD))
	{
		m_pINFSecuSetPassWnd->Tick();
	}
	if(IsShowWnd(SECURITY_MODE_SETUP_CHANGE_PASS))
	{
		m_pINFSecuChangePassWnd->Tick();
	}
	if(IsShowWnd(SECURITY_MODE_PASS_WND))
	{
		m_pINFSecuPassWnd->Tick();
	}

}

int CINFSecuMain::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(IsShowWnd(SECURITY_MODE_PASS_WND))
	{
		return m_pINFSecuPassWnd->WndProc(uMsg, wParam, lParam);		
	}
	if(IsShowWnd(SECURITY_MODE_SETUP_CHANGE_PASS))
	{
		return m_pINFSecuChangePassWnd->WndProc(uMsg, wParam, lParam);
	}
	if(IsShowWnd(SECURITY_MODE_SETUP_PASSWORD))
	{		
		return m_pINFSecuSetPassWnd->WndProc(uMsg, wParam, lParam);
	}
	if(IsShowWnd(SECURITY_MODE_LOCK))
	{
		return m_pINFSecuLockWnd->WndProc(uMsg, wParam, lParam);
	}

	return INF_MSGPROC_NORMAL;

	//return INF_MSGPROC_BREAK;		// 2010. 06. 21 by hsLee. �ǹ� ���� ���Ϲ� �ּ�.
}

BOOL CINFSecuMain::IsShowWnd(int nShowWnd)
{
	if(m_nShowWnd & (1<<nShowWnd))
	{
		return TRUE;
	}
	return FALSE;
}
void CINFSecuMain::ShowSecurityWnd(int nShowWnd, BOOL bShow, int nTmpParameter /*= 0*/)
{
	if(bShow)
	{
		m_nShowWnd |= (1<<nShowWnd);				
	}
	else
	{
		m_nShowWnd &= ~(1<<nShowWnd);
	}

	// 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
	if( nShowWnd == 0 )
	{
 		if(bShow)
		{
			// 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
			//g_INFCnt++;
			g_pD3dApp->AddINFCnt();
			// end 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
			g_pGameMain->m_bChangeMousePoint = TRUE;
			m_nCnt = 0;
		}
		else
		{	
			m_nCnt++;
			if(m_nCnt < 2)
			{
				// 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
				//g_INFCnt--;
				//if(g_INFCnt==0)
				if(g_pD3dApp->RemoveINFCnt() == 0)
				{
					g_pGameMain->m_bChangeMousePoint = FALSE;
				}
				// end 2012-12-17 by jhjang ������ ���콺 Ŀ���� ����� �������� �ʴ� ���� ����
			}
		}
	}
	// end 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����

	switch(nShowWnd)
	{
	case SECURITY_MODE_LOCK:
		{
			if(bShow)
			{
				m_pINFSecuLockWnd->InitBtn();
			}		
		}
		break;
	case SECURITY_MODE_SETUP_PASSWORD:
		{
			g_pD3dApp->EnableChatMode(bShow, FALSE);
			if(bShow)
			{
				m_pINFSecuSetPassWnd->ShowSetPassWnd();		
			}

			else
			{				
				m_pINFSecuSetPassWnd->OnCloseInfWnd();
			}
		}
		break;
	case SECURITY_MODE_SETUP_CHANGE_PASS:
		{
			g_pD3dApp->EnableChatMode(bShow, FALSE);
			if(bShow)
			{
				m_pINFSecuChangePassWnd->ShowSetPassWnd();		
			}
			else
			{				
				m_pINFSecuChangePassWnd->OnCloseInfWnd();
			}
		}
		break;
	case SECURITY_MODE_PASS_WND:
		{
			g_pD3dApp->EnableChatMode(bShow, FALSE);
			if(bShow)
			{			
				m_pINFSecuPassWnd->ShowSetPassWnd(nTmpParameter);		
			}
			else
			{				
				m_pINFSecuPassWnd->OnCloseInfWnd();
			}
		}
		break;
	}
	
}
BOOL CINFSecuMain::IsShowSecurityWnd()
{
	if(m_nShowWnd)
	{
		return TRUE;
	}
	return FALSE;	

}
void CINFSecuMain::AllHideSecurityWnd()
{	
	int nId = 0;
	for(nId = 0; nId < MAX_SECURITY_MODE_WND;nId++)
	{
		ShowSecurityWnd(nId, FALSE);
	}
}
// Setup Password Ŭ�� 
void CINFSecuMain::OnClickSetupPass()
{
	ShowSecurityWnd(SECURITY_MODE_SETUP_PASSWORD, TRUE);
}
void CINFSecuMain::OnClickSetupChangePass()
{
	ShowSecurityWnd(SECURITY_MODE_SETUP_CHANGE_PASS, TRUE);
}

// UnLock ��ư Ŭ�� 
void CINFSecuMain::OnClickLock(int nUnLockMode)
{
	ShowSecurityWnd(SECURITY_MODE_PASS_WND, TRUE, nUnLockMode);
}

void CINFSecuMain::SendCheckPassword(char* pPass, BYTE bUnLock)
{
	// Lock ���̳�?
	if(FALSE == g_pD3dApp->GetEnterLock())
	{
		return;
	}
	MD5 MD5_instance;
	unsigned char md5_string[16];
	char szEncodedString[33];
	
	MD5_instance.MD5Encode(pPass, md5_string);
	MD5_instance.MD5Binary2String(md5_string, szEncodedString);
		
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_SECONDARYPASSWORD_LOCK, T_FC_INFO_SECONDARYPASSWORD_LOCK, pSMsg, SendBuf);
	
	pSMsg->bIsUnlockFlag = bUnLock;
	strncpy(pSMsg->CurrentSecPassword, szEncodedString, SIZE_MAX_PASSWORD_MD5_STRING);	
	
	g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_INFO_SECONDARYPASSWORD_LOCK));	

	// Lock
	g_pD3dApp->EnterLock(FALSE);
	
}
// ��ȣ ���� 
VOID CINFSecuMain::FieldSecondLockOk(BOOL bIsUnlockFlag, BOOL bSuccess)
{
	// UnLock
	g_pD3dApp->EnterLock(TRUE);

	if(bSuccess)
	{
		if(bIsUnlockFlag)
		{
			g_pGameMain->CreateChatChild(STRMSG_C_070917_0202, COLOR_SYSTEM);//"\\y���� �ý����� �����Ǿ����ϴ�."
		}
		else
		{
			g_pGameMain->CreateChatChild(STRMSG_C_070917_0205, COLOR_SYSTEM);//"\\y���� �ý����� Ȱ��ȭ �Ǿ����ϴ�."			
		}

		ShowSecurityWnd(SECURITY_MODE_PASS_WND, FALSE);
		m_pINFSecuLockWnd->UpdateLockBtn(bIsUnlockFlag);
	}
	else
	{
		m_pINFSecuPassWnd->ClearEdit();

	}
}
// ��ȣ ó�� ����
VOID CINFSecuMain::SetSecurityPassword(char* pPass)
{
	// Lock ���̳�?
	if(FALSE == g_pD3dApp->GetEnterLock())
	{
		return;
	}
	
	// �Ⱦ��� ����
	m_nPassChangeMode = -1;

	MD5 MD5_instance;
	unsigned char md5_string[16];
	char szEncodedString[33];
	
	MD5_instance.MD5Encode(pPass, md5_string);
	MD5_instance.MD5Binary2String(md5_string, szEncodedString);
		
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD, T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD, pSMsg, SendBuf);
		
	memset(pSMsg->CurrentSecPassword, 0x00, SIZE_MAX_PASSWORD_MD5_STRING);	
	strncpy(pSMsg->NewSecPassword, szEncodedString, SIZE_MAX_PASSWORD_MD5_STRING);	
	
	g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD));	

	// Lock
	g_pD3dApp->EnterLock(FALSE);
}

// ��ȣ ���� 
VOID CINFSecuMain::ChangeSecurityPassword(char* pCurrentPass, char* pNewPass)
{
	// Lock ���̳�?
	if(FALSE == g_pD3dApp->GetEnterLock())
	{
		return;
	}

	// �Ⱦ��� ����
	m_nPassChangeMode = -1;
	
	MD5 curr_MD5_instance, new_MD5_instance;
	unsigned char curr_md5_string[16], new_md5_string[16];
	char curr_szEncodedString[33], new_szEncodedString[33];
	
	curr_MD5_instance.MD5Encode(pCurrentPass, curr_md5_string);
	curr_MD5_instance.MD5Binary2String(curr_md5_string, curr_szEncodedString);

	new_MD5_instance.MD5Encode(pNewPass, new_md5_string);
	new_MD5_instance.MD5Binary2String(new_md5_string, new_szEncodedString);
	
		
	//T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD, T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD, pSMsg, SendBuf);
		
	strncpy(pSMsg->CurrentSecPassword, curr_szEncodedString, SIZE_MAX_PASSWORD_MD5_STRING);	
	strncpy(pSMsg->NewSecPassword, new_szEncodedString, SIZE_MAX_PASSWORD_MD5_STRING);	
	
	g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD));	

	// Lock
	g_pD3dApp->EnterLock(FALSE);

}
// ��ȣ ����� ��ȣ ó�� ���� 
void CINFSecuMain::FieldSocketSecondPasswordUpdateOk(BOOL bSuccess, BOOL bFirstUseSecondPassword)
{
	// UnLock
	g_pD3dApp->EnterLock(TRUE);

	switch(m_nPassChangeMode)
	{
	case SECURITY_MODE_PASS_SENCOND_CANCEL:
		{
			//2�� �н����� ��� ����.
			if(bSuccess)
			{
				// �ٽ� �ε带 �ؾ��Ѵ�.
				m_pINFSecuLockWnd->SetFirstMode(FALSE);
				g_pGameMain->CreateChatChild(STRMSG_C_070928_0202, COLOR_SYSTEM);//"\\y���� �ý����� �����Ǿ����ϴ�. �ٽ� �����Ͻ÷��� F10 ��ư�� Ŭ���ϼ���."
				ShowSecurityWnd(SECURITY_MODE_PASS_WND, FALSE);		
			}			
		}
		break;
	default:
		{
			//2�� �н����� ����
			if(bSuccess)
			{		
				g_pGameMain->CreateChatChild(STRMSG_C_070917_0206, COLOR_SYSTEM);//"\\y���� �ý����� �����Ǿ����ϴ�."		
				ShowSecurityWnd(SECURITY_MODE_SETUP_PASSWORD, FALSE);		
				ShowSecurityWnd(SECURITY_MODE_SETUP_CHANGE_PASS, FALSE);		
				if(bFirstUseSecondPassword)
				{
					// �ٽ� �ε带 �ؾ��Ѵ�.
					m_pINFSecuLockWnd->SetFirstMode(FALSE);
					ShowSecurityWnd(SECURITY_MODE_LOCK, TRUE);
				}
			}
			else
			{
				m_pINFSecuChangePassWnd->ShowSetPassWnd();
			}
		}
		break;		
	}
}
// 2�� �н����� ���
void CINFSecuMain::SendSecndPassWordCancel(char* pCurrentPass)
{
	// Lock ���̳�?
	if(FALSE == g_pD3dApp->GetEnterLock())
	{
		return;
	}
	m_nPassChangeMode = SECURITY_MODE_PASS_SENCOND_CANCEL;

	MD5 curr_MD5_instance;
	unsigned char curr_md5_string[16];
	char curr_szEncodedString[33];
	
	curr_MD5_instance.MD5Encode(pCurrentPass, curr_md5_string);
	curr_MD5_instance.MD5Binary2String(curr_md5_string, curr_szEncodedString);

	
	//T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD
	INIT_MSG_WITH_BUFFER(MSG_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD, T_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD, pSMsg, SendBuf);
		
	strncpy(pSMsg->CurrentSecPassword, curr_szEncodedString, SIZE_MAX_PASSWORD_MD5_STRING);	
	memset(pSMsg->NewSecPassword, 0x00, SIZE_MAX_PASSWORD_MD5_STRING);	
	
	g_pFieldWinSocket->Write(SendBuf, MSG_SIZE(MSG_FC_INFO_SECONDARYPASSWORD_UPDATE_PASSWORD));	

	// Lock
	g_pD3dApp->EnterLock(FALSE);
	
}

// 2�� �н����� ��� ����
int CINFSecuMain::GetPassChangeMode()
{
	return m_nPassChangeMode;
}
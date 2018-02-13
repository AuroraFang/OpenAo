// INFCurselEditBox.cpp: implementation of the CINFCurselEditBox class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "D3DHanFont.h"
#include "INFGameMainChat.h"

#include "INFCurselEditBox.h"

#include "INFImageEx.h"	// 2011. 10. 10 by jskim UI�ý��� ����

#define		TICK_CURSEL_TIME			0.5f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFCurselEditBox::CINFCurselEditBox()
{
	m_bEnableEdit = FALSE;
	m_pTxtFont = NULL;

	m_nBuffLen = 512;	
	m_pTxtString = new char[m_nBuffLen+1];		
	memset(m_pTxtString, 0x00, m_nBuffLen+1);	

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	m_pBackupTxtString = new char[m_nBuffLen+1];		
	memset(m_pBackupTxtString, 0x00, m_nBuffLen+1);	

	m_pTmpTxtString = new char[m_nBuffLen+1];		
	memset(m_pTmpTxtString, 0x00, m_nBuffLen+1);	
	
		
	m_vecStringCulling.clear();

	m_ptCurSelPixel.x= m_ptCurSelPixel.y =0;
	m_ptCurSelPos.x = m_ptCurSelPos.y =0;	// Ŀ�� ��ġ
	m_ptEditPos.x = m_ptEditPos.y = 0;
	m_ptEndCurSelPos.x= m_ptEndCurSelPos.y =0;
	m_nStringHeightCap = 0;
	m_nStrWidth = 1;

	m_pImgCurSel = NULL;
	m_bShowCursel = TRUE;
	m_fShowCurselTick = 0;

	m_nMaxBuffLen = SIZE_MAX_STRING_1024 - SIZE_MAX_CHARACTER_NAME;

	m_bCullText = FALSE;
	

	m_nMouseHeight = 0;
	m_nStrOverFlowCntY = 0;		// ��Ʈ���� �ʰ��ƴ��� ���� �Ǵ�.

	m_nOnePageItemCnt = 1;		// �������� ǥ���Ҽ� �ִ� ��

	memset(m_strIMETmp, 0x00, SIZE_MAX_STRING_1024);
	memset(m_strIMEFullTmp, 0x00, SIZE_MAX_STRING_1024);	

	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	m_bBlockMode	= FALSE;
	ResetBlockPosition();

	m_bCtrlKeyState	= FALSE;
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
}

CINFCurselEditBox::~CINFCurselEditBox()
{
	CINFCurselEditBox::DeleteDeviceObjects();
	m_vecStringCulling.clear();
	util::del_array(m_pTxtString);

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	util::del_array(m_pBackupTxtString);	
	util::del_array(m_pTmpTxtString);

}

HRESULT CINFCurselEditBox::InitDeviceObjects(int nFontHeight, POINT ptEditPos, 
									   int nStrWidth, BOOL bUseCap, int nCap, 
									   BOOL bCullText/*=FALSE*/, int nMouseHeight/*=-1*/)
{
	DeleteDeviceObjects();
	m_bCullText = bCullText;

	// ��ġ���� 
	m_ptEditPos = ptEditPos;
	m_nStringHeightCap = nCap;
	m_nStrWidth = nStrWidth;

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	m_nMouseHeight = nCap;
	if(nMouseHeight > nCap)
	{
		m_nMouseHeight = nMouseHeight;
	}

	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;
	
	{
		m_pTxtFont = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), nFontHeight, D3DFONT_ZENABLE, TRUE,1024,32, bCullText);
		m_pTxtFont->InitDeviceObjects(g_pD3dDev) ;
	}

	char buf[16];

	// Ŀ�� 
	if(g_pGameMain && bUseCap)
	{
		DataHeader	* pDataHeader;

		m_pImgCurSel = new CINFImageEx;
		wsprintf(buf, "e_sel");
		pDataHeader = g_pGameMain->FindResource(buf);
		if(pDataHeader)
		{
			m_pImgCurSel->InitDeviceObjects(pDataHeader ) ;		
		}
	}


	return S_OK;
}

HRESULT CINFCurselEditBox::RestoreDeviceObjects()
{
	m_pTxtFont->RestoreDeviceObjects();	
	if(m_pImgCurSel)
	{
		m_pImgCurSel->RestoreDeviceObjects() ;
	}
	return S_OK;
}

HRESULT CINFCurselEditBox::DeleteDeviceObjects()
{
	if(m_pTxtFont)
	{
		m_pTxtFont->DeleteDeviceObjects();
		util::del(m_pTxtFont);
	}
	if(m_pImgCurSel)
	{
		m_pImgCurSel->DeleteDeviceObjects() ;
		util::del(m_pImgCurSel);
	}
	return S_OK;
}

HRESULT CINFCurselEditBox::InvalidateDeviceObjects()
{
	m_pTxtFont->InvalidateDeviceObjects();
	if(m_pImgCurSel)
	{
		m_pImgCurSel->InvalidateDeviceObjects() ;
	}
	
	return S_OK;
}

void CINFCurselEditBox::Render(int nScroll, int nOnePageLine)
{
	int nLine = 0;

	vector<string>::iterator it = m_vecStringCulling.begin();
	int nCnt = 0;
	for(nCnt=0; nCnt < nScroll;nCnt++)
	{
		if(it != m_vecStringCulling.end())
		{
			it++;
		}
	}
	int nLastLienWidth = -1;
	int nLastLine = -1;
	char chTmpBuff[256];
	char chLastStr = 0;	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����

	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	POINT ptBlockStart, ptBlockEnd;
	DetermineBlockFrontBack( &ptBlockStart, &ptBlockEnd );
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	
	while(it != m_vecStringCulling.end())
	{
		if(nLine >= nOnePageLine)
		{
			break;
		}
		char* pTxt = (char*)(*it).c_str();
		if(pTxt)
		{
			if(m_bCullText)
			{
				m_pTxtFont->SetTextureWidth(m_nStrWidth);
			}
			
			// 2008-10-13 by bhsohn ���� �б� Return �ȵǴ� ���� ó��			
			// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
			//strncpy(chTmpBuff, pTxt, strlen(pTxt)+1);
			if('\\' == chLastStr)
			{
				// 2009-02-02 by bhsohn ���� Ŭ����, n�� ������ ���� �ذ�
				chLastStr = 0;
				// end 2009-02-02 by bhsohn ���� Ŭ����, n�� ������ ���� �ذ�

				// ���� '\\'�̾����� ù���ڰ� n�̸� n�� �����. 
				if('n' == pTxt[0] )
				{
					memset(chTmpBuff, 0x00, strlen(pTxt)+1);
					strncpy(chTmpBuff, &pTxt[1], strlen(pTxt)+1);					
				}
				else
				{
					strncpy(chTmpBuff, pTxt, strlen(pTxt)+1);
				}
			}
			else
			{
				strncpy(chTmpBuff, pTxt, strlen(pTxt)+1);
			}
			int nLastIdx = strlen(chTmpBuff)-1;
			if(nLastIdx >= 0)
			{
				if('\\' == chTmpBuff[nLastIdx])
				{
					chTmpBuff[nLastIdx] = NULL;
					// 2009-02-02 by bhsohn ���� Ŭ����, n�� ������ ���� �ذ�
					chLastStr = '\\';
					// end 2009-02-02 by bhsohn ���� Ŭ����, n�� ������ ���� �ذ�
				}				
			}

			//chLastStr = g_pGameMain->DelCharacterRetrn(chTmpBuff);	// \\n ����
			g_pGameMain->DelCharacterRetrn(chTmpBuff);	// \\n ����
			// end 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����

			// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
// 			m_pTxtFont->DrawText(m_ptEditPos.x, 
// 				m_ptEditPos.y+(nLine * m_nStringHeightCap), 
// 				GUI_FONT_COLOR_W, 
// 				chTmpBuff);

			if( (nScroll + nLine) < ptBlockStart.y
				|| (nScroll + nLine) > ptBlockEnd.y
				|| !IsValidBlockPostion( ptBlockStart )
				|| !IsValidBlockPostion( ptBlockEnd ) )
			{
				m_pTxtFont->SetReLoadString( TRUE );
				m_pTxtFont->DrawText( m_ptEditPos.x,
									  m_ptEditPos.y + (nLine * m_nStringHeightCap),
									  GUI_FONT_COLOR_W,
									  chTmpBuff,
									  0 );
			}
			else
			{
				RECT fillRect = { 0, 0, GetStringLen((char*)((*it).c_str())), 0 };
				if( (nScroll + nLine) == ptBlockStart.y )
					fillRect.left	= ptBlockStart.x;

				if( (nScroll + nLine) == ptBlockEnd.y )
					fillRect.right	= ptBlockEnd.x;

				m_pTxtFont->SetReLoadString( TRUE );
				m_pTxtFont->DrawText( m_ptEditPos.x,
									  m_ptEditPos.y + (nLine * m_nStringHeightCap),
									  GUI_FONT_COLOR_W,
									  chTmpBuff,
									  0,
									  &fillRect );
			}
			// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����

			nLastLienWidth = m_pTxtFont->GetStringSize(pTxt).cx;
			nLastLine = nLine;
			nLine++;
		}		
		it++;
	}	
	
	// Ŀ�� ������
	if(IsEditMode() && m_bShowCursel )
	{	
		m_pImgCurSel->Move(m_ptCurSelPixel.x, m_ptCurSelPixel.y);
		m_pImgCurSel->Render();
	}
}

// ���� ¥�� ������
//void CINFCurselEditBox::Render()
//{
//	char chTxtBuff[512];
//	memset(chTxtBuff, 0x00, 512);
//	vector<string>::iterator it = m_vecStringCulling.begin();		
//	if(it != m_vecStringCulling.end())
//	{		
//		char* pTxt = (char*)(*it).c_str();
//		if(pTxt)
//		{
//			strncpy(chTxtBuff, pTxt, strlen(pTxt));			
//		}
//	}		
//	if(IsEditMode())
//	{
//		strcat(chTxtBuff, "_");
//	}
//	if(m_bCullText)
//	{
//		m_pTxtFont->SetTextureWidth(m_nStrWidth);
//	}
//	m_pTxtFont->DrawText(m_ptEditPos.x, 
//		m_ptEditPos.y, 
//		GUI_FONT_COLOR_W, 
//		chTxtBuff);			
//}

BOOL CINFCurselEditBox::IsEditMode()
{
	return m_bEnableEdit;
}

void CINFCurselEditBox::EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus)
{		
	m_bEnableEdit = bEnableEdit;
	// 2009-02-26 by bhsohn Japan IME
	//g_pD3dApp->EnableChatMode(m_bEnableEdit, TRUE);
	if(FALSE == bEnableEdit && FALSE == bOnlyFocus)
	{
		// ä�� ���� �ʱ�ȭ
		g_pD3dApp->CleanText();
	}
	// 2009-02-26 by bhsohn Japan IME
	// �ݵ�� IME-Compo ���� �ʱ�ȭ�ϰ� IME�� �����Ѵ�.
	g_pD3dApp->EnableChatMode(m_bEnableEdit, TRUE);
	// end 2009-02-26 by bhsohn Japan IME
}

void CINFCurselEditBox::BackupTxtString()
{
	if(NULL == m_pBackupTxtString)
	{
		return;
	}
	// ���ſ� TRUE���ٰ� ������ ������ŰŲ�?
	strncpy(m_pBackupTxtString, m_pTxtString, strlen(m_pTxtString)+1);
}

void CINFCurselEditBox::GetString(char* pTxt, int nBufferCnt)
{
	if(NULL == m_pTxtString)
	{
		return;
	}
	// 2009-02-02 by bhsohn Edit ��Ʈ�� �ִ� �۾����� ��Ʈ�� ������ ��� ����
	//strncpy(pTxt, m_pTxtString, nBufferCnt); //m_nMaxBuffLen
	memset(pTxt,0x00,nBufferCnt);

	int nMaxCnt = strlen(m_pTxtString)+1;
	int nCnt = 0;
	int nPos = 0;
	for(nCnt = 0;nCnt < nMaxCnt;nCnt++)
	{
		char* pFineTxt = CharNext(&m_pTxtString[nPos]);
		if(!pFineTxt )
		{			
			break;
		}
		int nTmpPos = pFineTxt - m_pTxtString;	// ù��ġ���� �Ÿ�				
		if(nTmpPos >= nBufferCnt )
		{
			break;
		}
		nPos = nTmpPos;
	}
	// ���������� �۾� ����
	strncpy(pTxt, m_pTxtString, nPos); 
	pTxt[nPos] = NULL;
	// end 2009-02-02 by bhsohn Edit ��Ʈ�� �ִ� �۾����� ��Ʈ�� ������ ��� ����
}

int  CINFCurselEditBox::SetString(char* pTxt, int nBufferCnt, int nScrollStep, BOOL bCurselInit)
{
	if((nBufferCnt > m_nBuffLen)
		|| (NULL == m_pTxtString))
	{
		util::del_array(m_pTxtString);
		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
		util::del_array(m_pBackupTxtString);	
		util::del_array(m_pTmpTxtString);

		m_nBuffLen = nBufferCnt;
		if(m_nBuffLen <= 1)
		{
			// �ּ� ���۷��� 16���� ũ��
			m_nBuffLen = 16;
		}
		
		m_pTxtString = new char[m_nBuffLen+1];		
		memset(m_pTxtString, 0x00, m_nBuffLen+1);

		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
		m_pBackupTxtString = new char[m_nBuffLen+1];		
		memset(m_pBackupTxtString, 0x00, m_nBuffLen+1);		

		m_pTmpTxtString = new char[m_nBuffLen+1];		
		memset(m_pTmpTxtString, 0x00, m_nBuffLen+1);			
	}
	strncpy(m_pTxtString, pTxt, m_nBuffLen);
	m_vecStringCulling.clear();

	if(!m_bCullText)
	{		
		// 2008-10-13 by bhsohn ���� �б� Return �ȵǴ� ���� ó��			
		g_pGameMain->StringCullingUserData_ToNormal(m_pTxtString, m_nStrWidth, &m_vecStringCulling, m_pTxtFont);
	}
	else
	{
		m_vecStringCulling.push_back(m_pTxtString);
	}

	// �ø� �۾��� ¥������?
	// ������ �۾��� "\\n"�̴�
	int nLastPos = strlen(m_pTxtString)-1;
	if(nLastPos >= 1)
	{
		if(('\\' == m_pTxtString[nLastPos-1])
			&&('n' == m_pTxtString[nLastPos]))
		{
			char chBlank[8];
			wsprintf(chBlank, "");

			m_vecStringCulling.push_back(chBlank);
		}
	}

	// Ŀ���� ��ġ ������	
	m_ptEndCurSelPos = RefreshEndCurselPos();
	
	if(bCurselInit)
	{			
		// Ŀ�� ��ġ ���������� ����
		m_ptCurSelPos = m_ptEndCurSelPos;
	}	


	// ������ �ȼ� ��ġ�� �˷��ش�. 	
	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;

	// Ŀ�� ��ġ ����
	UpdateCurselPos(nScrollStep, (int)m_vecStringCulling.size());
//
//	int nStringLine = m_vecStringCulling.size() -1;
//	if(nStringLine >= 0)
//	{
//		char* pTxt = (char*)m_vecStringCulling[nStringLine].c_str();
//		if(pTxt)
//		{
//			m_ptCurSelPixel.x += m_pTxtFont->GetStringSize(pTxt).cx;			
//			m_ptCurSelPixel.y += (nStringLine * m_nStringHeightCap);
//		}
//	}	
	return m_vecStringCulling.size();
}

int  CINFCurselEditBox::SetStrcat(char* pTxt, int nBufferCnt, int nIMEBufLen, int nScrollStep)
{
	if((nBufferCnt > m_nBuffLen)
		|| (NULL == m_pTxtString))
	{
		util::del_array(m_pTxtString);
		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
		util::del_array(m_pBackupTxtString);	
		util::del_array(m_pTmpTxtString);
		
		m_nBuffLen = nBufferCnt;
		if(m_nBuffLen <= 1)
		{
			// �ּ� ���۷��� 16���� ũ��
			m_nBuffLen = 16;
		}
		
		m_pTxtString = new char[m_nBuffLen+1];		
		memset(m_pTxtString, 0x00, m_nBuffLen+1);
		
		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
		m_pBackupTxtString = new char[m_nBuffLen+1];		
		memset(m_pBackupTxtString, 0x00, m_nBuffLen+1);		
		
		m_pTmpTxtString = new char[m_nBuffLen+1];		
		memset(m_pTmpTxtString, 0x00, m_nBuffLen+1);			
	}
	strncpy(m_pTxtString, pTxt, m_nBuffLen);
	m_vecStringCulling.clear();
	
	// �ø� �۾��� ¥������?
	if(!m_bCullText)
	{		
		// 2008-10-13 by bhsohn ���� �б� Return �ȵǴ� ���� ó��			
		g_pGameMain->StringCullingUserData_ToNormal(m_pTxtString, m_nStrWidth, &m_vecStringCulling, m_pTxtFont);
	}
	else
	{
		m_vecStringCulling.push_back(m_pTxtString);
	}		
	
	// Ŀ���� ��ġ ������	
	m_ptEndCurSelPos = RefreshEndCurselPos();	
		
	// Ŀ�� ��ġ ����
	UpdateCurselPos(nScrollStep, (int)m_vecStringCulling.size());	

	return m_vecStringCulling.size();
}

void CINFCurselEditBox::InitString()
{
	if(NULL == m_pTxtString)
	{
		return;
	}
	m_vecStringCulling.clear();
	memset(m_pTxtString, 0x00, m_nBuffLen+1);	
	// ������ ��ġ�� �˷��ش�. 
	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;
}

BOOL CINFCurselEditBox::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int nScrollStep, int *pLienStep /*= NULL*/, 
								int *o_pScrollStep /*= NULL*/, BOOL *o_pStrcat/*=NULL*/)
{
	if(FALSE == IsEditMode())
	{
		return FALSE;
	}
	switch(uMsg)
	{
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:	
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_SETCONTEXT:	
	case WM_CHAR:
		{			
			// 2008-07-14 by bhsohn EP3 ���� ó��
			// 2008-12-29 by bhsohn 2�� �н����� ���� �޸� ���� ����
			if(g_pGameMain && g_pGameMain->m_pChat && g_pGameMain->m_pChat->IsChatingMode())
			{
				return FALSE;
			}
			// end 2008-07-14 by bhsohn EP3 ���� ó��
			
			int bFlag = FALSE;
			g_nRenderCandidate = SET_MAIN_CHAT_CANDIDATE;

					
			if(INF_MSGPROC_BREAK == g_pD3dApp->m_inputkey.KeyMapping(g_pD3dApp->GetHwnd(), uMsg, wParam, lParam))
			{
				bFlag = TRUE;
			}	
			
			{
				/////////////////// �ڿ� �߰� ���ִ� ��� ///////////////////												
//				if((WM_CHAR == uMsg)
//					&& (0 == strlen(g_pD3dApp->m_inputkey.GetIMEFullStr()))
//					&& ('\b' == wParam))
//				{
//					ClickBackSpace();											
//				}

				BOOL bStrCat = FALSE;
				int nImeCopeLen = GetImeStringLen();
				// ���۾��� �ֳ�?
				strncpy(m_pTxtString, m_pBackupTxtString, m_nBuffLen);

				int nBuffLen = strlen(m_pTxtString)+strlen(g_pD3dApp->m_inputkey.GetIMEFullStr());
				if(strlen(g_pD3dApp->m_inputkey.GetIMEFullStr())!=0 && (nBuffLen < m_nBuffLen))
				{						
					if((m_ptCurSelPos.x == m_ptEndCurSelPos.x)
						&&(m_ptCurSelPos.y == m_ptEndCurSelPos.y))
					{
						// Ŀ���� Ŀ���� ���� Ŀ���� �����Ƿ� �ڿ��� �߰�						
						strcat(m_pTxtString, g_pD3dApp->m_inputkey.GetIMEFullStr());
						SetString(m_pTxtString, strlen(m_pTxtString)+1, 
									nScrollStep, TRUE);
					}
					else
					{
						int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());						
						//DBGOUT("nIMELen[%s] ", g_pD3dApp->m_inputkey.GetIMEFullStr());

						memset(m_pTmpTxtString, 0x00, nBuffLen);		// ���� �ʱ�ȭ
						int nStringXLen = m_ptCurSelPos.x;
						
						int nStrPos = GetStringPos(m_pTxtString, nStringXLen, m_ptCurSelPos.y);						
						
						if(nStrPos > 0)
						{
							strncpy(m_pTmpTxtString, m_pTxtString, nStrPos);
							//DBGOUT("nIMELen[%d] nStrPos[%d] m_pTmpTxtString""%s"" \n", nIMELen, nStrPos, m_pTmpTxtString);
						}
						strcat(m_pTmpTxtString, g_pD3dApp->m_inputkey.GetIMEFullStr()); // �ڿ� ��Ʈ���� ���δ�.
						strcat(m_pTmpTxtString, &m_pTxtString[nStrPos]);
						strncpy(m_pTxtString, m_pTmpTxtString,nBuffLen);			// ���� ���ۿ� ����
						SetStrcat(m_pTxtString, strlen(m_pTxtString)+1, nIMELen, nScrollStep);			// ��Ʈ���� �����ϴ� ���

						bStrCat = TRUE;
					}
				}
				else if(WM_IME_STARTCOMPOSITION != uMsg)
				{
					SetString(m_pTxtString, strlen(m_pTxtString)+1, nScrollStep, FALSE);
				}
				
				if(pLienStep)
				{
					(*pLienStep) = m_vecStringCulling.size();					
				}
				if(o_pStrcat)
				{
					(*o_pStrcat)= bStrCat;
				}
				// �ִ� 512�۾� ���Ѱ� ����
				// 2009-02-02 by bhsohn Edit ��Ʈ�� �ִ� �۾����� ��Ʈ�� ������ ��� ����

				// 2009-02-11 by bhsohn Japan MemBug
// 				if(((strlen(m_pBackupTxtString) + GetImeStringLen()) >= m_nBuffLen)
// 					||((strlen(m_pTxtString) + GetImeStringLen()) >= m_nBuffLen))

				// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
// 				if(((strlen(m_pBackupTxtString) + GetImeStringLen()) >= 250)
// 					||((strlen(m_pTxtString) + GetImeStringLen()) >= 250))
// 				{
// 					keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
// 				}
				if(((strlen(m_pBackupTxtString) + GetImeStringLen()) >= m_nBuffLen)
					||((strlen(m_pTxtString) + GetImeStringLen()) >= m_nBuffLen))
				{
					keybd_event(VK_BACK, 0, KEYEVENTF_EXTENDEDKEY, 0);
				}
			}

			// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
			if( !IsBlockMode() )
				ResetBlockPosition();
			// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
			
			if(TRUE == bFlag)
				return TRUE;
		}
		break;
	case WM_KEYDOWN:
		{
			// 2008-07-14 by bhsohn EP3 ���� ó��
			// 2008-12-29 by bhsohn 2�� �н����� ���� �޸� ���� ����
			if(g_pGameMain && g_pGameMain->m_pChat && g_pGameMain->m_pChat->IsChatingMode())
			{
				return FALSE;
			}
			// end 2008-07-14 by bhsohn EP3 ���� ó��

			int nCurselLine = 0;
			switch(wParam)
			{
			case VK_RETURN:
				{
					ClickEnter(nScrollStep);
//					BOOL bEditMode = IsEditMode();
//					if(bEditMode)
//					{
//						BackupTxtString();
//					}
//					bEditMode ^= TRUE;
//					EnableEdit(bEditMode, FALSE);
//					return TRUE;
				}
				break;				
			case VK_BACK:
				{					
					// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
// 					if(ClickBackSpace())
// 					{
// 						return TRUE;
// 					}
					nCurselLine = ClickBackSpace( nScrollStep );
					if( !nCurselLine )
						return FALSE;
					// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
					
//					char *pNextStr = CharNext(&m_pBackupTxtString[nPrevPos]);		
//					if(nPrevPos>=0 && pNextStr)
//					{
//						int nNextPos = pNextStr - m_pBackupTxtString;
//						m_pBackupTxtString[nNextPos] = NULL;
//					}
					
				}
				break;

			// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
			case VK_DELETE:
				{
					nCurselLine = OnDeleteKeyDown( nScrollStep );
					if( !nCurselLine )
						return FALSE;
				}
				break;

			case VK_HOME:
				{
					OnHomeKeyDown( nScrollStep );
					return TRUE;
				}
				break;

			case VK_END:
				{
					OnEndKeyDown( nScrollStep );
					return TRUE;
				}
				break;

			case VK_SHIFT:
				{
					OnShiftKeyDown();
					return TRUE;
				}
				break;

			case VK_CONTROL:
				{
					OnCtrlKeyDown();
				}
				break;

			case 'c':
			case 'C':
				{
					if( IsControlKeyDown() )
					{
						std::string str;
						GetBlockedString( &str );
						StringToClipBoard( str.c_str() );
						return TRUE;
					}
				}
				break;

			case 'v':
			case 'V':
				{
					if( IsControlKeyDown() )
					{
						std::string str;
						GetStringFromClipBoard( &str );
						if( str.length() > 0
							&& str.length() + strlen( m_pTxtString ) < m_nBuffLen )
						{
							PasteString( str.c_str(), nScrollStep );
							return TRUE;
						}
					}
				}
				break;
			// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����

			case VK_LEFT:	// �������� �̵�
				{
					// ����Ű Ŭ��
					nCurselLine = OnArrowKeyDown(VK_LEFT, nScrollStep); 					
				}
				break;
			case VK_UP:
				{
					nCurselLine = OnArrowKeyDown(VK_UP, nScrollStep);
				}
				break;
			case VK_RIGHT:
				{
					nCurselLine = OnArrowKeyDown(VK_RIGHT, nScrollStep);					
				}
				break;
			case VK_DOWN:
				{
					nCurselLine = OnArrowKeyDown(VK_DOWN, nScrollStep);								
					
				}
				break;
			}
			if(o_pScrollStep && pLienStep)
			{
				(*pLienStep) = m_vecStringCulling.size();
				(*o_pScrollStep) = nCurselLine;
			}
		}
		break;
	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	case WM_KEYUP:
		{
			switch( wParam )
			{
				case VK_SHIFT:
				{
					OnShiftKeyUp();
					return TRUE;
				}
				break;

				case VK_CONTROL:
				{
					OnCtrlKeyUp();
					return TRUE;
				}
				break;
			}
		}
		break;
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	}
	return FALSE;
}

// 2007-11-05 by bhsohn ���� â�� �ִ� �۾� ���Ѱ� ����
int CINFCurselEditBox::GetImeStringLen()
{
	return g_pD3dApp->m_inputkey.GetImeStringLen();
}

BOOL CINFCurselEditBox::OnLButtonDown(POINT ptPos)
{
	if((m_ptEditPos.x < ptPos.x && ptPos.x < (m_ptEditPos.x + m_nStrWidth))
		//&&(m_ptEditPos.y < ptPos.y && ptPos.y < (m_ptEditPos.y + m_nStringHeightCap)))
		&&(m_ptEditPos.y < ptPos.y && ptPos.y < (m_ptEditPos.y + m_nMouseHeight)))
	{		
		return TRUE;
	}	

	return FALSE;
	
	int nLine = 0;
	int nCharcter = 0;
	int nMinCapX, nMinCapY;
	int nRealX, nRealY;
	int nStrPointGap=0;	

	nMinCapX = m_nStrWidth;
	nMinCapY = m_nStrWidth;

	ptPos.x -= m_ptEditPos.x;
	ptPos.y -= m_ptEditPos.y;

	nRealX = nRealY = -1;

	int nSelLine = 0;
	int nSelCharacter = 0;

	
	char chBuff[MAX_PATH], chFulllPath[1024];
	memset(chBuff, 0x00, MAX_PATH);
	ZERO_MEMORY(chFulllPath);	

	for(nLine = 0; nLine < m_vecStringCulling.size(); nLine++)
	{
		char* pTxt = (char*)m_vecStringCulling[nLine].c_str();
		if(NULL == pTxt)
		{
			continue;
		}
		nStrPointGap = 0;		
		for(nCharcter = 0;nCharcter < strlen(pTxt);nCharcter++)
		{
			char	*pStr = CharNext(&pTxt[nStrPointGap]);
			if(NULL == pStr)
			{
				break;
			}
			nStrPointGap = pStr - pTxt;
			memset(chBuff, 0x00, MAX_PATH);
			strncpy(chBuff, pTxt, nStrPointGap);

			int nStringLen = m_pTxtFont->GetStringSize(chBuff).cx;

			int nTmpX = abs(nStringLen - ptPos.x);
			int nTmpY = abs((nLine * m_nStringHeightCap) - ptPos.y);
			
			if((nTmpX < nMinCapX))
			{
				nSelCharacter = nStrPointGap;
				nMinCapX = nTmpX;
				nRealX = nStringLen;
			}			
			if(nTmpY < nMinCapY )
			{				
				nSelLine = nLine;
				nMinCapY = nTmpY;				
				nRealY = (nLine * m_nStringHeightCap);				
			}			
		}
	}
	// Y���� �ٽ� �˻� 
	// �ֳ��ϸ� Y�� ���� ���� ����� �ʰ� �ϱ� ���� 
	BOOL bBreak = FALSE;
	for(nLine = m_vecStringCulling.size()-1; nLine >= 0 ; nLine--)
	{
		if(TRUE == bBreak)
		{
			break;
		}
		char* pTxt = (char*)m_vecStringCulling[nLine].c_str();
		if(NULL == pTxt)
		{
			continue;
		}
		if(nLine == nSelLine)
		{
			int nFullTmpX = m_pTxtFont->GetStringSize(pTxt).cx;
			if(nRealX > nFullTmpX)
			{
				nSelLine--;
				if(nSelLine < 0){nSelLine=0;}
				nRealY = nSelLine * m_nStringHeightCap;				
				bBreak = TRUE;
			}
		}	
	}

	// �ø� ���� 
	bBreak = FALSE;
	for(nLine = 0; nLine < m_vecStringCulling.size(); nLine++)
	{
		if(TRUE == bBreak)
		{
			break;
		}
		char* pTxt = (char*)m_vecStringCulling[nLine].c_str();
		if(NULL == pTxt)
		{
			continue;
		}
		if(nLine < nSelLine)
		{
			// Ǯ�н� ã�����ؼ�.
			strcat(chFulllPath, pTxt);
			continue;
		}
		else if(nLine > nSelLine)
		{
			bBreak = TRUE;
			break;
		}
		
		nStrPointGap = 0;		
		for(nCharcter = 0;nCharcter < strlen(pTxt);nCharcter++)
		{
			char	*pStr = CharNext(&pTxt[nStrPointGap]);
			if(NULL == pStr)
			{
				break;
			}
			nStrPointGap = pStr - pTxt;
			memset(chBuff, 0x00, MAX_PATH);
			strncpy(chBuff, pTxt, nStrPointGap);
			
			if(nRealX == m_pTxtFont->GetStringSize(chBuff).cx)				
			{
				// ��Ʈ�� ��ġ�� ã�Ҵ�.
				strcat(chFulllPath, chBuff);
				int nFindPos = strlen(chFulllPath);				
				// ��Ʈ�� ��ġ ����
				//g_pD3dApp->m_inputkey.SetStrPos(nFindPos);
				bBreak = TRUE;
				break;
								
			}
		}
	}

	if((nRealX != -1)&&(nRealY != -1))
	{
		m_ptCurSelPixel.x = m_ptEditPos.x + nRealX;			
		m_ptCurSelPixel.y = m_ptEditPos.y + nRealY;
		return TRUE;
	}
	return FALSE;
}
void CINFCurselEditBox::Tick()
{
	m_fShowCurselTick -= g_pD3dApp->GetCheckElapsedTime();
	if(m_fShowCurselTick < 0)
	{
		m_fShowCurselTick = TICK_CURSEL_TIME;
		m_bShowCursel ^= TRUE;
	}
}

void CINFCurselEditBox::SetPos(int nPosX, int nPosY)
{
	m_ptEditPos.x = nPosX;
	m_ptEditPos.y = nPosY;
	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;
}

void CINFCurselEditBox::SetStringMaxBuff(int nMaxBuffLen)
{
	m_nMaxBuffLen = nMaxBuffLen;	

	if(m_nBuffLen < nMaxBuffLen)
	{
		m_nBuffLen = nMaxBuffLen;
		
		util::del_array(m_pTxtString);
		m_pTxtString = new char[m_nBuffLen+1];		
		memset(m_pTxtString, 0x00, m_nBuffLen+1);	
		
		// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
		util::del_array(m_pBackupTxtString);
		m_pBackupTxtString = new char[m_nBuffLen+1];		
		memset(m_pBackupTxtString, 0x00, m_nBuffLen+1);	

		util::del_array(m_pTmpTxtString);
		m_pTmpTxtString = new char[m_nBuffLen+1];		
		memset(m_pTmpTxtString, 0x00, m_nBuffLen+1);	
	}

}

// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
//BOOL CINFCurselEditBox::ClickBackSpace()
BOOL CINFCurselEditBox::ClickBackSpace( int nScrollStep )
// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
{
	if(strlen(g_pD3dApp->m_inputkey.GetIMEFullStr()) > 0
		|| strlen(m_pBackupTxtString)==0)
	{
		return FALSE;
	}
	else if(0 == m_ptCurSelPos.x && 0 == m_ptCurSelPos.y)
	{
		return FALSE;
	}
	

	BOOL bLastPos = IsLastPos();

	if(bLastPos)
	{	
		
		int nStringPos = strlen(m_pBackupTxtString);
		int	nPrevPos = nStringPos; 
		int nCnt = 0;
		BOOL bBreak = FALSE;
		BOOL bChaRtn = FALSE;
		for(nCnt = 0;nCnt < nStringPos; nCnt++)
		{
			char	*pPrevStr = CharPrev(m_pBackupTxtString, &m_pBackupTxtString[nPrevPos]);		
			if(NULL == pPrevStr)
			{
				return FALSE;
			}
			nPrevPos = pPrevStr - (m_pBackupTxtString);		
			if(nPrevPos < 0)
			{
				break;
			}
			if(NULL == m_pBackupTxtString[nPrevPos])
			{
				break;
			}
			else if((FALSE == bChaRtn) && ('n' == m_pBackupTxtString[nPrevPos]))
			{
				bChaRtn = TRUE;				
			}			
			else if((TRUE == bChaRtn) && ('\\' == m_pBackupTxtString[nPrevPos]))
			{
				bChaRtn = FALSE;
				bBreak = TRUE;
			}	
			else
			{
				bBreak = TRUE;
			}
			
			m_pBackupTxtString[nPrevPos] = NULL;
			if(bBreak)
			{
				break;
			}
		}		
	}
	else
	{
		int nStartCursel = m_ptCurSelPos.x-1;
		int nEndCursel = nStartCursel+1;
		int nLine = m_ptCurSelPos.y;
		if(nLine >= (int)m_vecStringCulling.size())
		{
			return FALSE;
		}		
		
		if(nStartCursel < 0 && nLine >0)
		{
			int nOldLine = nLine;
			nLine--;
			if(nLine < 0)
			{
				return FALSE;
			}		
			char* pTxt = (char*)m_vecStringCulling[nLine].c_str();	
			
			int nPosY=0;
			int nLineChar, nLineStr;
			nLineChar = nLineStr = 0;

			for(nPosY=0;nPosY<=nLine;nPosY++)
			{
				// Y�� ����
				char* pTxt = (char*)m_vecStringCulling[nPosY].c_str();
				nLineStr += GetStringLen(pTxt);
				nLineChar+= strlen(pTxt);

			}

			m_ptCurSelPos.x = nLineStr;
			m_ptCurSelPos.y = nLine;				
			
			nStartCursel = nLineStr-1;
			nEndCursel = nStartCursel+1;			
		}
		else
		{
			m_ptCurSelPos.x--;
		}
		
		if(m_ptCurSelPos.x < 0)
		{
			m_ptCurSelPos.x = 0;			
		}
		if(m_ptCurSelPos.y < 0)
		{
			m_ptCurSelPos.y = 0;
		}
		
		
		int nMaxStrLen = strlen(m_pTxtString);
		
		int nStartPos = GetStringPos(m_pTxtString, nStartCursel, nLine);
		int nEndPos = GetStringPos(m_pTxtString, nEndCursel, nLine);

		
		{
			memset(m_pTmpTxtString, 0x00, strlen(m_pTmpTxtString)+1);		// ���� �ʱ�ȭ
			strncpy(m_pTmpTxtString, m_pTxtString, nStartPos);
			strcat(m_pTmpTxtString, &m_pTxtString[nEndPos]);
			strncpy(m_pBackupTxtString, m_pTmpTxtString, strlen(m_pTmpTxtString)+1);			
		}		
	}

	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	//return TRUE;

	ResetBlockPosition();
	return UpdateCurselPos( nScrollStep, (int)m_vecStringCulling.size() );
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
}

// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
BOOL	CINFCurselEditBox::OnDeleteKeyDown( int nScrollStep )
{
	if( strlen( g_pD3dApp->m_inputkey.GetIMEFullStr() ) > 0
		|| strlen(m_pBackupTxtString)==0)
	{
		return FALSE;
	}

	int		nStartBuffPos	= 0;
	char*	pBuff;

	if( !IsValidBlockPostion( m_ptBlockStartPos )
		|| !IsValidBlockPostion( m_ptBlockStartPos ) )
	{
		if(IsLastPos())
			return FALSE;
		
		nStartBuffPos	= GetStringPos( m_pTxtString, m_ptCurSelPos.x, m_ptCurSelPos.y );
		pBuff			= m_pBackupTxtString + nStartBuffPos;

		if( *pBuff != '\\' )
			pBuff = CharNext( pBuff );
		else
		{
			pBuff = CharNext( pBuff );
			pBuff = CharNext( pBuff );
		}
	}
	else
	{
		POINT ptBlockStart, ptBlockEnd;
		DetermineBlockFrontBack( &ptBlockStart, &ptBlockEnd );

		nStartBuffPos	= GetStringPos( m_pTxtString, ptBlockStart.x, ptBlockStart.y );
		pBuff			= m_pBackupTxtString + GetStringPos( m_pTxtString, ptBlockEnd.x, ptBlockEnd.y );
		m_ptCurSelPos	= ptBlockStart;
	}

	strcpy( m_pBackupTxtString + nStartBuffPos, pBuff );
	strcpy( m_pTmpTxtString, m_pBackupTxtString );
	strcpy( m_pTxtString, m_pBackupTxtString );

	SetStrcat( m_pTxtString, strlen( m_pTxtString ) + 1, 0, nScrollStep );
	ResetBlockPosition();

	return UpdateCurselPos( nScrollStep, (int)m_vecStringCulling.size() );
}

void	CINFCurselEditBox::OnHomeKeyDown( int nScrollStep )
{
	if(m_nStrOverFlowCntY)
	{
		m_ptCurSelPos.y += m_nStrOverFlowCntY;		
		m_ptCurSelPos.x = GetStringLen(m_strIMETmp);
	}

	POINT ptPrevPos = m_ptCurSelPos;

	int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());
	if( ptPrevPos.x - nIMELen > 0
		&& ptPrevPos.x + nIMELen < GetStringLen( (char*)(m_vecStringCulling[ptPrevPos.y].c_str()) ) )
	{
		ptPrevPos.x += nIMELen;
	}

	m_ptCurSelPos.x = 0;

	BackupTxtString();
	g_pD3dApp->CleanText();

	// Ŀ�� ��ġ ����
	UpdateCurselPos(nScrollStep, (int)m_vecStringCulling.size());

	if( !IsBlockMode() )
		ResetBlockPosition();
	else
	{
		if( !IsValidBlockPostion(m_ptBlockStartPos) )
			m_ptBlockStartPos = ptPrevPos;

		m_ptBlockEndPos = m_ptCurSelPos;
	}
}

void	CINFCurselEditBox::OnEndKeyDown( int nScrollStep )
{
	if(m_nStrOverFlowCntY)
	{
		m_ptCurSelPos.y += m_nStrOverFlowCntY;		
		m_ptCurSelPos.x = GetStringLen(m_strIMETmp);
	}

	POINT ptPrevPos = m_ptCurSelPos;
	int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());
	if( ptPrevPos.x - nIMELen > 0
		&& ptPrevPos.x + nIMELen < GetStringLen( (char*)(m_vecStringCulling[ptPrevPos.y].c_str()) ) )
	{
		ptPrevPos.x += nIMELen;
	}

	m_ptCurSelPos.x = GetStringLen( (char*)(m_vecStringCulling[m_ptCurSelPos.y].c_str()) );

	BackupTxtString();
	g_pD3dApp->CleanText();

	// Ŀ�� ��ġ ����
	UpdateCurselPos(nScrollStep, (int)m_vecStringCulling.size());

	if( !IsBlockMode() )
		ResetBlockPosition();
	else
	{
		if( !IsValidBlockPostion(m_ptBlockStartPos) )
			m_ptBlockStartPos = ptPrevPos;

		m_ptBlockEndPos = m_ptCurSelPos;
	}
}

void	CINFCurselEditBox::OnShiftKeyDown( void )
{
	m_bBlockMode	= TRUE;

	ResetBlockPosition();
}

void	CINFCurselEditBox::OnShiftKeyUp( void )
{
	m_bBlockMode	= FALSE;
}

void	CINFCurselEditBox::OnCtrlKeyDown( void )
{
	m_bCtrlKeyState	= TRUE;
}

void	CINFCurselEditBox::OnCtrlKeyUp( void )
{
	m_bCtrlKeyState	= FALSE;
}

void	CINFCurselEditBox::ResetBlockPosition( void )
{
	m_ptBlockStartPos.x	= m_ptBlockStartPos.y = -1;
	m_ptBlockEndPos		= m_ptBlockStartPos;
}

BOOL	CINFCurselEditBox::IsValidBlockPostion( POINT pt )
{
	return pt.x > -1 && pt.y > -1;
}

void	CINFCurselEditBox::DetermineBlockFrontBack( POINT* ptBlockStart, POINT* ptBlockEnd )
{
	if( m_ptBlockStartPos.y > m_ptBlockEndPos.y )
	{
		*ptBlockStart	= m_ptBlockEndPos;
		*ptBlockEnd		= m_ptBlockStartPos;
	}
	else if( m_ptBlockStartPos.y < m_ptBlockEndPos.y )
	{
		*ptBlockStart	= m_ptBlockStartPos;
		*ptBlockEnd		= m_ptBlockEndPos;
	}
	else
	{
		if( m_ptBlockStartPos.x > m_ptBlockEndPos.x )
		{
			*ptBlockStart	= m_ptBlockEndPos;
			*ptBlockEnd		= m_ptBlockStartPos;
		}
		else
		{
			*ptBlockStart	= m_ptBlockStartPos;
			*ptBlockEnd		= m_ptBlockEndPos;
		}
	}
}

void	CINFCurselEditBox::GetBlockedString( std::string* pStr )
{
	if( !IsValidBlockPostion( m_ptBlockStartPos )
		|| !IsValidBlockPostion( m_ptBlockEndPos ) )
	{
		return;
	}

	POINT ptBlockStart, ptBlockEnd;
	DetermineBlockFrontBack( &ptBlockStart, &ptBlockEnd );
	
	int nStartBuffPos	= GetStringPos( m_pTxtString, ptBlockStart.x, ptBlockStart.y );
	char*	pStart		= m_pBackupTxtString + nStartBuffPos;
	char*	pEnd		= m_pBackupTxtString + GetStringPos( m_pTxtString, ptBlockEnd.x, ptBlockEnd.y );

	char	temp[2048] = {0, };
	strncpy( temp, pStart, pEnd - pStart );

	pStr->assign( temp );
}

void	CINFCurselEditBox::StringToClipBoard( const char* pStr )
{
	if( OpenClipboard( NULL ) )
	{
		if( EmptyClipboard() )
		{
			HGLOBAL hMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, strlen( pStr ) + 1 );
			if( hMem )
			{
				LPTSTR pClipData = (LPTSTR)GlobalLock( hMem );
				strcpy( pClipData, pStr );

				SetClipboardData( CF_OEMTEXT, hMem );
				GlobalUnlock( hMem );
				GlobalFree( hMem );
			}
		}

		CloseClipboard();
	}
}

void	CINFCurselEditBox::GetStringFromClipBoard( std::string* pStr )
{
	if( OpenClipboard( NULL ) )
	{
		HGLOBAL hGlobal = GetClipboardData( CF_TEXT );

		// 2011-03-22 by hsson ��� ������ ���Ǵ� ���ڿ� ���� ����
		const int nstrSize = 512;
		char strBuff[nstrSize] = {0, };
		// end 2011-03-22 by hsson ��� ������ ���Ǵ� ���ڿ� ���� ����

		if(hGlobal)
		{
			PTSTR pGlobal = (char*)GlobalLock(hGlobal);

			// 2011-03-22 by hsson ��� ������ ���Ǵ� ���ڿ� ���� ����
			strncpy( strBuff, pGlobal, nstrSize - 1 );
			// end 2011-03-22 by hsson ��� ������ ���Ǵ� ���ڿ� ���� ����

			GlobalUnlock(hGlobal);
		}
		CloseClipboard();
		pStr->assign( strBuff );
	}
}

void	CINFCurselEditBox::PasteString( const char* pStr, int nScrollStep )
{
	if( strlen(pStr) + strlen( m_pTxtString ) > m_nBuffLen )
		return;

	int		nStartBuffPos	= 0;

	if( !IsValidBlockPostion( m_ptBlockStartPos )
		|| !IsValidBlockPostion( m_ptBlockStartPos ) )
	{
		nStartBuffPos	= GetStringPos( m_pTxtString, m_ptCurSelPos.x, m_ptCurSelPos.y );
	}
	else
	{
		POINT ptBlockStart, ptBlockEnd;
		DetermineBlockFrontBack( &ptBlockStart, &ptBlockEnd );

		nStartBuffPos	= GetStringPos( m_pTxtString, ptBlockStart.x, ptBlockStart.y );
	}

	std::string temp( m_pBackupTxtString + nStartBuffPos );
	strcpy( m_pBackupTxtString + nStartBuffPos, pStr );
	strcat( m_pBackupTxtString, temp.c_str() );
	strcpy( m_pTmpTxtString, m_pBackupTxtString );
	strcpy( m_pTxtString, m_pBackupTxtString );

	// 2010. 05. 25 by jskim �������� ���� ����
	char tempchr[256];
	memset(tempchr, 0x00, 256);

	// 2011-03-22 by hsson ��� ������ ���Ǵ� ���ڿ� ���� ����
	strncpy(tempchr, pStr, 256);
	// end 2011-03-22 by hsson ��� ������ ���Ǵ� ���ڿ� ���� ����

	m_ptCurSelPos.x += GetStringBuffLen(tempchr);
	//end 2010. 05. 25 by jskim �������� ���� ����

	SetStrcat( m_pTxtString, strlen( m_pTxtString ) + 1, 0, nScrollStep );
	ResetBlockPosition();
}
// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		2����Ʈ�� �ѱ��ڷ� �ν��Ѵ�.
/// \author		// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
/// \date		2008-06-23 ~ 2008-06-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCurselEditBox::GetStringLen(char* pTxt)
{
	int nCheckPoint = 0;
	int nStringLen = 0;
	int nCnt = 0;
	BOOL bChRtn = FALSE;
	int nMaxSize = strlen(pTxt);
	for(nCnt = 0;nCnt < strlen(pTxt); nCnt++)
	{
		char	*pNextStr = CharNext(&pTxt[nCheckPoint]);
		if(NULL == pNextStr)
		{
			break;
		}
		else if(NULL == pTxt[nCheckPoint])
		{
			break;
		}
		else if(!bChRtn && ('\\' == pTxt[nCheckPoint]))
		{
			bChRtn = TRUE;
		}
		else if(bChRtn && ('n' == pTxt[nCheckPoint]))
		{
			bChRtn = FALSE;			
		}
		else
		{
			// \n �ν� ���ϰ� ��.
			nStringLen++;
		}
		nCheckPoint = pNextStr - pTxt;		
	}
	return nStringLen;
}
// CharNext�� ���� ��ġ�� �������� �޸� ���� ��� �´�.
int CINFCurselEditBox::GetCharLen(char* pTxt, int nNextChatLen)
{
	int nCheckPoint = 0;	
	int nCnt = 0;
	int nMaxSize = strlen(pTxt);
	for(nCnt = 0;nCnt < nNextChatLen; nCnt++)
	{
		char	*pNextStr = CharNext(&pTxt[nCheckPoint]);
		if(NULL == pNextStr)
		{
			break;
		}
		else if(NULL == pTxt[nCheckPoint])
		{
			break;
		}
		nCheckPoint = pNextStr - pTxt;		
	}
	return nCheckPoint;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
/// \date		2008-06-23 ~ 2008-06-23
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFCurselEditBox::UpdateCurselPos(int nScrollStep, int nMaxScrollStep)
{
	m_nStrOverFlowCntY = 0;
	memset(m_strIMETmp, 0x00, strlen(m_strIMETmp));

	int nLastLinePos = ((int)m_vecStringCulling.size())-1;
	
	if(nLastLinePos < 0 )
	{
		// Ŀ�� ��ġ �ʱ�ȭ
		m_ptCurSelPixel.x = m_ptEditPos.x;
		m_ptCurSelPixel.y = m_ptEditPos.y;
		m_ptCurSelPos.x = m_ptCurSelPos.y =0;	// Ŀ�� ��ġ
		return 0;
	}	
	else if(m_ptCurSelPos.y > nLastLinePos)
	{
		char* pLastTxt = (char*)m_vecStringCulling[nLastLinePos].c_str();			
		m_ptCurSelPos.x = GetStringLen(pLastTxt);
		m_ptCurSelPos.y = nLastLinePos;
	}
	int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());
	BOOL bLastPos = IsLastPos();

	int nCheckPoint = 0;
	int nPosX = 0;
	char chStrBuff[512];
	memset(chStrBuff, 0x00, 512);

	// Y��ǥ �ּ�/�ִ밪 �޸� �ȳѾ�� ����
	{
		if(m_ptCurSelPos.y < 0)
		{
			m_ptCurSelPos.y = 0;
		}
		else if(m_ptCurSelPos.y > nLastLinePos)
		{
			m_ptCurSelPos.y = nLastLinePos;
		}
	}

	char* pTxt = (char*)m_vecStringCulling[m_ptCurSelPos.y].c_str();	
	
	// X��ǥ �ּ�/�ִ밪 �޸� �ȳѾ�� ����
	{
		int nMaxCurselX = GetStringLen(pTxt);			
		if(m_ptCurSelPos.x > nMaxCurselX)
		{
			// �ִ� ���� �־�� �ְ��Ѵ�.
			m_ptCurSelPos.x = nMaxCurselX;
		}
	}
	
	
	// X�ȼ���ǥ ����
	{
		m_ptCurSelPixel.x = m_ptEditPos.x;
		int nCheckPoint = GetCharLen(pTxt, m_ptCurSelPos.x);	// CharNext�� ���� ��ġ�� �������� �޸� ���� ��� �´�.
		if(pTxt && nCheckPoint > 0)
		{
			strncpy(chStrBuff, pTxt, nCheckPoint);		
			m_ptCurSelPixel.x += m_pTxtFont->GetStringSize(chStrBuff).cx;		

			// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
			int nMaxPixelX = (m_ptEditPos.x + m_nStrWidth);
			if(m_ptCurSelPixel.x > nMaxPixelX)
			{
				m_ptCurSelPixel.x = nMaxPixelX;
			}
			// end 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
		}
		if(nIMELen&& !bLastPos)
		{
			m_nStrOverFlowCntY = UpdateCurselX();							
		}			
	}	

	// y��ǥ ����
	int nMinY, nMaxY;
	nMinY = nMaxY = 0;

	nMinY = nScrollStep;
	nMaxY = nScrollStep+m_nOnePageItemCnt-1;

	int nCurPosY= m_ptCurSelPos.y - nScrollStep;
	int nCurMaxPosY = (nMaxScrollStep-nScrollStep);
	{
		int nCurPixelPosY = m_ptEditPos.y;		 
		if(m_nStrOverFlowCntY)
		{
			nCurPosY+=m_nStrOverFlowCntY;			
		}		
		if(nCurPosY < 0)
		{
			nCurPixelPosY = m_ptEditPos.y;
		}
		else if(nCurPosY > (m_nOnePageItemCnt-1))
		{
			nCurPixelPosY += ((m_nOnePageItemCnt-1) * m_nStringHeightCap);
		}
		else
		{
			nCurPixelPosY += (nCurPosY * m_nStringHeightCap);
		}
		m_ptCurSelPixel.y = nCurPixelPosY;
	}

//	DBGOUT("m_ptCurSelPos[%d][%d] m_ptEndCurSelPos[%d][%d] \n", m_ptCurSelPos.x, m_ptCurSelPos.y, 
//																	m_ptEndCurSelPos.x, m_ptEndCurSelPos.y);
//
//	DBGOUT("nCurPosY[%d] nMinY[%d] nMaxY[%d] nScrollStep[%d] \n", nCurPosY, nMinY, nMaxY, nScrollStep);
	
	// ��ũ�� �̵���
	int nScrollDiff = 0;	
	if(m_ptCurSelPos.y < nMinY)
	{
		nScrollDiff = m_ptCurSelPos.y - nMinY;	
	}
	else if(m_ptCurSelPos.y > nMaxY)
	{
		nScrollDiff =  m_ptCurSelPos.y - nMaxY;	
	}

	return nScrollDiff;
}

int  CINFCurselEditBox::UpdateCurselX()
{
	if(0 == m_nStrWidth)
	{
		return 0;
	}
	int nOverFlowY = 0;
	int nLinePosY = m_ptCurSelPos.y;
	int nIMEStrPixel = 0;
	int nCurSelWidth = 0;
	
	int nCnt =0;
	int nCurslePixelX = m_ptCurSelPixel.x;

	char *pImeTxt = g_pD3dApp->m_inputkey.GetIMEFullStr();
	int nImeBufLen = strlen(pImeTxt) +1;
	strncpy(m_strIMEFullTmp, pImeTxt, nImeBufLen);
	BOOL bBreak = FALSE;
	int nOverStringPos = 0;
	memset(m_strIMETmp, 0x00, nImeBufLen);
	
	nIMEStrPixel = m_pTxtFont->GetStringSize(m_strIMEFullTmp).cx;	
	nCurslePixelX = m_ptCurSelPixel.x + nIMEStrPixel;
	nCurSelWidth = nCurslePixelX - m_ptEditPos.x;
	
	nOverFlowY = (nCurSelWidth/m_nStrWidth); // Y���� �˾Ƴ���.
	

	for(nCnt = 0;nCnt < 2000; nCnt++)	//����� ���� for
	{
		if(bBreak)
		{
			break;
		}

		nIMEStrPixel = m_pTxtFont->GetStringSize(m_strIMEFullTmp).cx;	
		nCurslePixelX = m_ptCurSelPixel.x + nIMEStrPixel;
		
		nCurSelWidth = nCurslePixelX - m_ptEditPos.x;
		if(nCurSelWidth < m_nStrWidth)
		{
			bBreak = TRUE;
			break;
		}				

		nOverStringPos = strlen(m_strIMEFullTmp);
		char	*pPrevStr = CharPrev(m_strIMEFullTmp, &m_strIMEFullTmp[nOverStringPos]);		
		if(NULL == pPrevStr)
		{
			bBreak = TRUE;
			break;
		}
		int	nPrevPos = pPrevStr - (m_strIMEFullTmp);	
		int nBakPos = (nImeBufLen - nPrevPos);
		if(nBakPos < 0 || nPrevPos<0)
		{
			break;
		}
		strncpy(m_strIMETmp,	&pImeTxt[nPrevPos], nBakPos);
		strncpy(m_strIMEFullTmp, pImeTxt, nPrevPos);
		
		m_strIMEFullTmp[nPrevPos] = NULL;
		m_strIMETmp[nBakPos] = NULL;

	}
	if(strlen(m_strIMETmp) > 0)
	{
		nIMEStrPixel = m_pTxtFont->GetStringSize(m_strIMETmp).cx;	
		nCurslePixelX = m_ptEditPos.x + nIMEStrPixel;
		m_ptCurSelPixel.x = nCurslePixelX;			
	}
	else
	{
		m_ptCurSelPixel.x = nCurslePixelX;	
	}

	return nOverFlowY;
}

int CINFCurselEditBox::OnArrowKeyDown(int nArrow, int nScrollStep)
{	
	if(m_nStrOverFlowCntY)
	{
		// ��Ʈ�� �����÷��ϋ��� ���� ��ġ�����ϳ�ũ��.
		m_ptCurSelPos.y += m_nStrOverFlowCntY;		
		m_ptCurSelPos.x = GetStringLen(m_strIMETmp);
	}
	int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());
	BOOL bLastPos = IsLastPos();

	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	POINT ptPreCurPos	= m_ptCurSelPos;
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����

	switch(nArrow)
	{
	case VK_LEFT:	// �������� �̵�
		{
			// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
// 			if(bLastPos || !nIMELen || m_nStrOverFlowCntY)
// 			{
// 				m_ptCurSelPos.x--;
// 				if(m_ptCurSelPos.x <0)
// 				{
// 					m_ptCurSelPos.x = 0;
// 				}
// 			}
// 			else
// 			{
// 				m_ptCurSelPos.x += (nIMELen-1);
// 			}

			if(bLastPos || !nIMELen || m_nStrOverFlowCntY)
			{
				m_ptCurSelPos.x--;

				if( m_ptCurSelPos.x < 0 )
				{
					if( m_ptCurSelPos.y <= 0 )
						m_ptCurSelPos.x = 0;
					else
					{
						--m_ptCurSelPos.y;
						m_ptCurSelPos.x	= m_vecStringCulling[m_ptCurSelPos.y].size();
					}
				}
			}
			else
			{
				m_ptCurSelPos.x += (nIMELen-1);
				ptPreCurPos.x	+= nIMELen;
			}
			// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
		}
		break;
	case VK_UP:
		{
			// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
// 			if(bLastPos || !nIMELen)
// 			{
// 				m_ptCurSelPos.y--;
// 				if(m_ptCurSelPos.x <0)
// 				{
// 					m_ptCurSelPos.x = 0;
// 				}
// 			}
// 			else
// 			{
// 				if(!m_nStrOverFlowCntY)
// 				{
// 					m_ptCurSelPos.x += (nIMELen);
// 				}
// 				m_ptCurSelPos.y--;
// 				if(m_ptCurSelPos.x <0)
// 				{
// 					m_ptCurSelPos.x = 0;
// 				}
// 			}

			if( --m_ptCurSelPos.y < 0 )
			{
				m_ptCurSelPos.x = 0;
				m_ptCurSelPos.y	= 0;
			}
			else
			{
				if( nIMELen && !m_nStrOverFlowCntY )
				{
					m_ptCurSelPos.x += nIMELen;
					if( ptPreCurPos.x + nIMELen <= GetStringLen( (char*)(m_vecStringCulling[ptPreCurPos.y].c_str()) ) )
						ptPreCurPos.x += nIMELen;
				}

				char	szBuff[ 1024 ]	= { 0, };
				char	*pString, *pBuff;
				pString = pBuff = (char*)(m_vecStringCulling[m_ptCurSelPos.y+1].c_str());

				int i=0;
				for( i=0; i<m_ptCurSelPos.x; ++i )
				{
					if( *pBuff != '\\ ')
						pBuff = CharNext( pBuff );
					else
					{
						pBuff = CharNext( pBuff );
						pBuff = CharNext( pBuff );
					}
				}

				strncpy( szBuff, pString, pBuff - pString );
				szBuff[ pBuff - pString + 1 ] = '\0';

				int nStringWidth = m_pTxtFont->GetStringSize( szBuff ).cx;

				pString			= (char*)(m_vecStringCulling[m_ptCurSelPos.y].c_str());

				if( nStringWidth >= m_pTxtFont->GetStringSize( pString ).cx )
					m_ptCurSelPos.x = GetStringLen( pString );
				else
				{
					for( i=0; i<=GetStringLen( pString ); ++i )
					{
						memset( szBuff, 0, sizeof( szBuff ) );
						pBuff = pString;
						for( int j=0; j<i; ++j )
							pBuff = CharNext( pBuff );
						
						strncpy( szBuff, pString, pBuff - pString );
						szBuff[ pBuff - pString + 1 ] = '\0';
						
						if( m_pTxtFont->GetStringSize( szBuff ).cx >= nStringWidth )
						{
							m_ptCurSelPos.x = i;
							break;
						}
					}
				}
			}
			// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����						
		}
		break;
	case VK_RIGHT:
		{
			if(!m_nStrOverFlowCntY)
			{
				m_ptCurSelPos.x += (nIMELen);
			}
			m_ptCurSelPos.x++;

			// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
			if( m_ptCurSelPos.x > GetStringLen( (char*)(m_vecStringCulling[m_ptCurSelPos.y].c_str()) ) )
			{
				if( ++m_ptCurSelPos.y < m_vecStringCulling.size() )
					m_ptCurSelPos.x	= 1;
				else
				{
					m_ptCurSelPos.y = m_vecStringCulling.size() - 1;
					m_ptCurSelPos.x = m_vecStringCulling[m_ptCurSelPos.y].size();
				}
			}

			ptPreCurPos.x	+= nIMELen;
			// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
		}
		break;
	case VK_DOWN:
		{
			// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
// 			if(bLastPos || !nIMELen)
// 			{
// 				m_ptCurSelPos.y++;
// 				int nMaxLine = ((int)m_vecStringCulling.size())-1;
// 				if(m_ptCurSelPos.y > nMaxLine)
// 				{
// 					m_ptCurSelPos.y = nMaxLine;
// 				}	
// 			}
// 			else
// 			{
// 				if(!m_nStrOverFlowCntY)
// 				{
// 					m_ptCurSelPos.x += (nIMELen);
// 				}
// 				m_ptCurSelPos.y++;
// 				int nMaxLine = ((int)m_vecStringCulling.size())-1;
// 				if(m_ptCurSelPos.y > nMaxLine)
// 				{
// 					m_ptCurSelPos.y = nMaxLine;
// 				}	
// 			}

			if( m_vecStringCulling.size() == 0 )
			{
				m_ptCurSelPos.x = 0;
				m_ptCurSelPos.y = 0;
				break;
			}

			if( ++m_ptCurSelPos.y >= m_vecStringCulling.size() )
			{
				m_ptCurSelPos.y	= m_vecStringCulling.size() - 1;
				m_ptCurSelPos.x	= GetStringLen( (char*)(m_vecStringCulling[m_ptCurSelPos.y].c_str()) );
			}
			else
			{
				if( nIMELen && !m_nStrOverFlowCntY )
				{
					m_ptCurSelPos.x += nIMELen;
					if( ptPreCurPos.x + nIMELen <= GetStringLen( (char*)(m_vecStringCulling[ptPreCurPos.y].c_str()) ) )
						ptPreCurPos.x += nIMELen;
				}

				char	szBuff[ 1024 ]	= { 0, };
				char	*pString, *pBuff;
				pString = pBuff = (char*)(m_vecStringCulling[m_ptCurSelPos.y-1].c_str());

				int i=0;
				for( i=0; i<m_ptCurSelPos.x; ++i )
				{
					if( *pBuff != '\\ ')
						pBuff = CharNext( pBuff );
					else
					{
						pBuff = CharNext( pBuff );
						pBuff = CharNext( pBuff );
					}
				}

				strncpy( szBuff, pString, pBuff - pString );
				szBuff[ pBuff - pString + 1 ] = '\0';

				int nStringWidth = m_pTxtFont->GetStringSize( szBuff ).cx;

				pString	= (char*)(m_vecStringCulling[m_ptCurSelPos.y].c_str());

				if( nStringWidth >= m_pTxtFont->GetStringSize( pString ).cx )
					m_ptCurSelPos.x = GetStringLen( pString );
				else
				{
					for( i=0; i<=GetStringLen( pString ); ++i )
					{
						memset( szBuff, 0, sizeof( szBuff ) );
						pBuff = pString;
						for( int j=0; j<i; ++j )
							pBuff = CharNext( pBuff );

						strncpy( szBuff, pString, pBuff - pString );
						szBuff[ pBuff - pString + 1 ] = '\0';

						if( m_pTxtFont->GetStringSize( szBuff ).cx >= nStringWidth )
						{
							m_ptCurSelPos.x = i;
							break;
						}
					}
				}
			}
			// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
		}
		break;
	}
	
	BackupTxtString();	// 
	g_pD3dApp->CleanText();

	// Ŀ�� ��ġ ����
	int nScrollLine = UpdateCurselPos(nScrollStep, (int)m_vecStringCulling.size());
	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	if( !IsBlockMode() )
		ResetBlockPosition();
	else
	{
		if( !IsValidBlockPostion( m_ptBlockStartPos ) )
			m_ptBlockStartPos = ptPreCurPos;

		m_ptBlockEndPos	= m_ptCurSelPos;
	}
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	return nScrollLine;
}

int CINFCurselEditBox::GetStringPos(char* pTxtString, int nStringXLen, int nLinePos)
{
	int nPosX, nPosY;
	int nStrPos = 0;
	nPosX = nPosY = 0;
	if(m_vecStringCulling.empty())
	{
		return nStrPos;
	}	
	int nMaxBuf = ((int)m_vecStringCulling.size()) -1;	
	if(nLinePos > nMaxBuf)
	{
		nLinePos = nMaxBuf;
	}	
	for(nPosY=0;nPosY<nLinePos;nPosY++)
	{
		// Y�� ����
		char* pTxt = (char*)m_vecStringCulling[nPosY].c_str();
		nStrPos += strlen(pTxt);
	}
	{
		char* pTxt = (char*)m_vecStringCulling[nLinePos].c_str();
		int nCheckPoint = 0;
		int nChCnt = 0;
		BOOL bChRtn = FALSE;
		for(nPosX = 0;nPosX<strlen(pTxt);nPosX++)
		{
			char	*pNextStr = CharNext(&pTxt[nCheckPoint]);
			if(NULL == pNextStr)
			{					
				break;
			}
			else if(NULL == pTxt[nCheckPoint])
			{			
				break;
			}
			else if(nStringXLen == nChCnt)
			{
				break;
			}
			else if('\\' != pTxt[nCheckPoint])
			{
				nChCnt++;
			}			
			nCheckPoint = pNextStr - pTxt;						
		}
		nStrPos += nCheckPoint;
	}
	return nStrPos;

}

POINT CINFCurselEditBox::RefreshEndCurselPos()
{
	POINT	ptEndCurSelPos;
	ptEndCurSelPos.x = ptEndCurSelPos.y = 0;		
	// ������ ��ġ�� �˷��ش�.
	int nLastLinePos = ((int)m_vecStringCulling.size())-1;
	if(nLastLinePos >= 0)
	{
		// ������ ��ġ�� ������ ���ش�.
		char* pCurselTxt = (char*)m_vecStringCulling[nLastLinePos].c_str();
		if(pCurselTxt)
		{
			ptEndCurSelPos.x = GetStringLen(pCurselTxt);
		}
		ptEndCurSelPos.y = nLastLinePos;
	}	
	DBGOUT("ptEndCurSelPos[%d][%d] \n", ptEndCurSelPos.x, ptEndCurSelPos.y);

	return ptEndCurSelPos;
}

void CINFCurselEditBox::SetOnePageItemCnt(int nOnePageItemCnt)
{
	m_nOnePageItemCnt = nOnePageItemCnt;		// �������� ǥ���Ҽ� �ִ� ��
}


BOOL CINFCurselEditBox::IsLastPos()
{
	BOOL bLastPos = ((m_ptCurSelPos.x == m_ptEndCurSelPos.x)&&(m_ptCurSelPos.y == m_ptEndCurSelPos.y));
	return bLastPos;

}
BOOL CINFCurselEditBox::ClickEnter(int nScrollStep)
{
	if(NULL == m_pTxtString)
	{
		return FALSE;
	}
//	int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());
//
//	if(nIMELen > 0)
//	{
//		// IME���ۿ� �� �ִ�.
//		strcat(m_pTxtString, g_pD3dApp->m_inputkey.GetIMEFullStr());
//	}

	char chReturn[8];
	memset(chReturn, 0x00, 8);
	wsprintf(chReturn, "\\n");

	int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());						

	int nBuffLen = strlen(m_pTxtString)+strlen(g_pD3dApp->m_inputkey.GetIMEFullStr());
	if(nBuffLen >= m_nBuffLen)
	{
		return FALSE;
	}

	BOOL bLastPos = IsLastPos();// IME���� �ʱ�ȭ	
	if(bLastPos)
	{	
		// "\\n"�� �ڿ� �߰����ش�.
		strcat(m_pTxtString, chReturn);
		m_vecStringCulling.push_back(chReturn);
	}
	else
	{		
		int nIMELen = GetStringLen(g_pD3dApp->m_inputkey.GetIMEFullStr());						
		//DBGOUT("nIMELen[%s] ", g_pD3dApp->m_inputkey.GetIMEFullStr());
		
		memset(m_pTmpTxtString, 0x00, nBuffLen);		// ���� �ʱ�ȭ		
		int nCurSelPosX = m_ptCurSelPos.x + nIMELen;
		int nStrPos = GetStringPos(m_pTxtString, nCurSelPosX, m_ptCurSelPos.y);			
		
		if(nStrPos > 0)
		{
			strncpy(m_pTmpTxtString, m_pTxtString, nStrPos);
			//DBGOUT("nIMELen[%d] nStrPos[%d] m_pTmpTxtString""%s"" \n", nIMELen, nStrPos, m_pTmpTxtString);
		}
		//strcat(m_pTmpTxtString, g_pD3dApp->m_inputkey.GetIMEFullStr()); // �ڿ� ��Ʈ���� ���δ�.
		strcat(m_pTmpTxtString, chReturn); // \\n�� ���δ�.		
		strcat(m_pTmpTxtString, &m_pTxtString[nStrPos]);
		strncpy(m_pTxtString, m_pTmpTxtString, strlen(m_pTmpTxtString)+1);			// ���� ���ۿ� ����
		

		m_vecStringCulling.clear();
		if(!m_bCullText)
		{	
			// 2008-10-13 by bhsohn ���� �б� Return �ȵǴ� ���� ó��			
			g_pGameMain->StringCullingUserData_ToNormal(m_pTxtString, m_nStrWidth, &m_vecStringCulling, m_pTxtFont);
		}
		else
		{
			m_vecStringCulling.push_back(m_pTxtString);
		}				
		
	}
	g_pD3dApp->CleanText(); 
	
	// �ø� �۾��� ¥������?
//	m_vecStringCulling.clear();
//	if(!m_bCullText)
//	{		
//		StringCulling(m_pTxtString, m_nStrWidth, &m_vecStringCulling, m_pTxtFont);
//	}
//	else
//	{
//		m_vecStringCulling.push_back(m_pTxtString);
//	}		
	
	DBGOUT("Cull Line[%d] \n", (int)m_vecStringCulling.size());
	
	// Ŀ���� ��ġ ������	
	m_ptEndCurSelPos = RefreshEndCurselPos();	

	if(bLastPos)
	{
		// Ŀ�� ��ġ ���������� ����
		m_ptCurSelPos = m_ptEndCurSelPos;
	}
	else
	{
		m_ptCurSelPos.x = 0;
		m_ptCurSelPos.y++;
		if(m_ptCurSelPos.y > m_ptEndCurSelPos.y)
		{
			m_ptCurSelPos.y = m_ptEndCurSelPos.y;
		}

	}

	// ������ �ȼ� ��ġ�� �˷��ش�. 	
	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;	
		
	// Ŀ�� ��ġ ����
	UpdateCurselPos(nScrollStep, (int)m_vecStringCulling.size());	

	BackupTxtString();	// ���� ���?

	

	return TRUE;

}

// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
void CINFCurselEditBox::SetFirstLine()
{
	// Ŀ�� ��ġ �ʱ�ȭ
	m_ptCurSelPixel.x = m_ptEditPos.x;
	m_ptCurSelPixel.y = m_ptEditPos.y;
	m_ptCurSelPos.x = m_ptCurSelPos.y =0;	// Ŀ�� ��ġ

}
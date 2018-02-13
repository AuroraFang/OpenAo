// INFItemInfo.cpp: implementation of the CINFItemInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDataLast.h"
#include "INFItemInfo.h"
#include "AtumApplication.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "AtumDatabase.h"
#include "D3DHanFont.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "ItemInfo.h"
#include "INFImage.h"
#include "GameDataLast.h"
#include "StoreData.h"
#include "dxutil.h"

// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
#include "Interface.h"
#include "INFIcon.h"
#include "INFCityBase.h"
#include "INFCityInfinityShop.h"
// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

#include "PetManager.h"		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#include "INFImageEx.h"		  // 2011. 10. 10 by jskim UI�ý��� ����
#include "INFArmorCollectWnd.h" // 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��

// 2008-04-14 by bhsohn ���� ������ ���� ��Ʈ������ ó��
#if defined(LANGUAGE_ENGLISH) || defined(LANGUAGE_VIETNAM)|| defined(LANGUAGE_THAI)// 2008-04-30 by bhsohn �±� ���� �߰�
#define STRING_CULL ::StringCullingUserData_ToBlank
#else
#define STRING_CULL ::StringCullingUserDataEx
#endif
// end 2008-04-14 by bhsohn ���� ������ ���� ��Ʈ������ ó��

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINFItemInfo::CINFItemInfo(CAtumNode* pParent)
{
	FLOG("CINFItemInfo(CAtumNode* pParent)");
	m_pParent = pParent;
	m_bShow = FALSE;
	memset(m_strItemInfo, 0x00, ITEMINFO_PARAMETER_NUMBER*ITEMINFO_ITEM_FULL_NAME);
	memset(m_strDesc, 0x00, ITEMINFO_DESC_SIZE*ITEMINFO_DESC_LINE_NUMBER);
	memset(m_strExtendItemInfo, 0x00, ITEMINFO_DESC_SIZE*ITEMINFO_EXTENDITEM_LINE_NUMBER);		 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	memset(m_pFontItemInfo, 0x00, ITEMINFO_PARAMETER_NUMBER * sizeof(DWORD));
	memset(m_pFontDescInfo, 0x00, ITEMINFO_DESC_LINE_NUMBER * sizeof(DWORD));
	memset(m_pFontExtendItemInfo, 0x00, ITEMINFO_EXTENDITEM_LINE_NUMBER * sizeof(DWORD));			// 2013-06-26 by ssjung �κ��丮 �߰� ���� 

	m_pFontItemName = NULL;
	m_pInchantNum = NULL;

	m_nDescIndex = 0;
	m_nMaxLength = 0;
	m_nDescLine = 0;
	m_nExtendItemIndex = 0;						  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 

	m_ptItemInfo.x = 0;
	m_ptItemInfo.y = 0;
	m_bRestored = FALSE;
	m_bEnableItem = FALSE;

	m_pRefItemInfo = NULL;
	m_pRefPrefixRareInfo = NULL;
	m_pRefSuffixRareInfo = NULL;
	m_pRefEnchant = NULL;
	m_pRefITEM = NULL;
	m_pDataHeader = NULL;
	m_pBigIcon = NULL;
	m_pGameData = NULL;

	memset(m_pInfoBoxSide, 0x00, sizeof(m_pInfoBoxSide[0]) * 9);
	memset(m_strItemName, 0x00, ITEMINFO_ITEM_FULL_NAME);

	m_nOtherItemCount = 0;
	m_vecTickFuntionIndex.clear();

	// 2009-02-03 by bhsohn ���� ������ �� ����
	m_bMyEquipItem = FALSE;
	m_pFontMyEquipItem = NULL;
	m_szTooltip.cx = 0;
	m_szTooltip.cy = 0;
	// end 2009-02-03 by bhsohn ���� ������ �� ����
}

CINFItemInfo::~CINFItemInfo()
{
	FLOG("~CINFItemInfo()");

	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		util::del(m_pFontItemInfo[i]);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		util::del(m_pFontDescInfo[i]);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		util::del(m_pFontExtendItemInfo[i]);								 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	}

	util::del(m_pFontItemName);
	// 2009-02-03 by bhsohn ���� ������ �� ����
	util::del(m_pFontMyEquipItem);
	// end 2009-02-03 by bhsohn ���� ������ �� ����
	util::del(m_pInchantNum);
	util::del(m_pDataHeader);
	util::del(m_pBigIcon);

	for (i = 0; i < 9; i++)
	{
		util::del(m_pInfoBoxSide[i]);
	}

}

void CINFItemInfo::InitItemInfo()
{
	FLOG("CINFItemInfo::InitItemInfo()");
	m_bShow = FALSE;
	memset(m_strItemInfo, 0x00, ITEMINFO_PARAMETER_NUMBER*ITEMINFO_ITEM_FULL_NAME);
	memset(m_strDesc, 0x00, ITEMINFO_DESC_LINE_NUMBER*ITEMINFO_DESC_SIZE);
	memset(m_strExtendItemInfo, 0x00, ITEMINFO_DESC_LINE_NUMBER*ITEMINFO_DESC_SIZE);							  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 

	m_nDescIndex = 0;
	m_nMaxLength = 0;
	m_nDescLine = 0;
	m_nExtendItemIndex = 0;													  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	m_bEnableItem = FALSE;
	m_pRefItemInfo = NULL;
	m_pRefPrefixRareInfo = NULL;
	m_pRefSuffixRareInfo = NULL;
	m_pRefEnchant = NULL;
	m_pRefITEM = NULL;
	m_vecTickFuntionIndex.clear();
}

HRESULT CINFItemInfo::InitDeviceObjects()
{
	FLOG("CINFItemInfo::InitDeviceObjects()");
	DataHeader	* pDataHeader;
	char buf[16];

	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		m_pFontItemInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE, 512, 32);
		m_pFontItemInfo[i]->InitDeviceObjects(g_pD3dDev);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontDescInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE, 512, 32);
		m_pFontDescInfo[i]->InitDeviceObjects(g_pD3dDev);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontExtendItemInfo[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE, FALSE, 512, 32);			  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		m_pFontExtendItemInfo[i]->InitDeviceObjects(g_pD3dDev);
	}

	m_pFontItemName = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE | D3DFONT_BOLD, TRUE, 512, 32);
	m_pFontItemName->InitDeviceObjects(g_pD3dDev);

	// 2009-02-03 by bhsohn ���� ������ �� ����
	if (NULL == m_pFontMyEquipItem)
	{
		m_pFontMyEquipItem = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 9, D3DFONT_ZENABLE | D3DFONT_BOLD, TRUE, 512, 32);
		m_pFontMyEquipItem->InitDeviceObjects(g_pD3dDev);
	}
	// end 2009-02-03 by bhsohn ���� ������ �� ����

	m_pInchantNum = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()), 8, D3DFONT_ZENABLE | D3DFONT_BOLD, FALSE, 512, 32);
	m_pInchantNum->InitDeviceObjects(g_pD3dDev);

	for (i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m_pInfoBoxSide[i * 3 + j] = new CINFImageEx;
			wsprintf(buf, "w_whi%d%d", i, j);
			pDataHeader = FindResource(buf);
			m_pInfoBoxSide[i * 3 + j]->InitDeviceObjects(pDataHeader);
		}
	}

	return S_OK;
}

HRESULT CINFItemInfo::RestoreDeviceObjects()
{
	FLOG("CINFItemInfo::RestoreDeviceObjects()");
	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		m_pFontItemInfo[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontDescInfo[i]->RestoreDeviceObjects();
	}
	for (i = 0; i < ITEMINFO_EXTENDITEM_LINE_NUMBER; i++)
	{
		m_pFontExtendItemInfo[i]->RestoreDeviceObjects();											 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	}
	m_pFontItemName->RestoreDeviceObjects();
	// 2009-02-03 by bhsohn ���� ������ �� ����
	if (m_pFontMyEquipItem)
	{
		m_pFontMyEquipItem->RestoreDeviceObjects();
	}
	// end 2009-02-03 by bhsohn ���� ������ �� ����

	m_pInchantNum->RestoreDeviceObjects();
	if (m_pBigIcon) m_pBigIcon->RestoreDeviceObjects();

	for (i = 0; i < 9; i++)
	{
		m_pInfoBoxSide[i]->RestoreDeviceObjects();
	}
	m_bRestored = TRUE;
	return S_OK;
}

HRESULT CINFItemInfo::InvalidateDeviceObjects()
{
	FLOG("CINFItemInfo::InvalidateDeviceObjects()");
	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		m_pFontItemInfo[i]->InvalidateDeviceObjects();
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontDescInfo[i]->InvalidateDeviceObjects();
	}
	for (i = 0; i < ITEMINFO_EXTENDITEM_LINE_NUMBER; i++)
	{
		m_pFontExtendItemInfo[i]->InvalidateDeviceObjects();								 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	}
	m_pFontItemName->InvalidateDeviceObjects();
	// 2009-02-03 by bhsohn ���� ������ �� ����
	if (m_pFontMyEquipItem)
	{
		m_pFontMyEquipItem->InvalidateDeviceObjects();
	}
	// end 2009-02-03 by bhsohn ���� ������ �� ����
	m_pInchantNum->InvalidateDeviceObjects();
	if (m_pBigIcon) m_pBigIcon->InvalidateDeviceObjects();

	for (i = 0; i < 9; i++)
	{
		m_pInfoBoxSide[i]->InvalidateDeviceObjects();
	}
	m_bRestored = FALSE;

	return S_OK;

}

HRESULT CINFItemInfo::DeleteDeviceObjects()
{
	FLOG("CINFItemInfo::DeleteDeviceObjects()");
	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		m_pFontItemInfo[i]->DeleteDeviceObjects();
		util::del(m_pFontItemInfo[i]);
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		m_pFontDescInfo[i]->DeleteDeviceObjects();
		util::del(m_pFontDescInfo[i]);
	}
	for (i = 0; i < ITEMINFO_EXTENDITEM_LINE_NUMBER; i++)
	{
		m_pFontExtendItemInfo[i]->DeleteDeviceObjects();												 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		util::del(m_pFontExtendItemInfo[i]);
	}

	m_pFontItemName->DeleteDeviceObjects();
	util::del(m_pFontItemName);
	// 2009-02-03 by bhsohn ���� ������ �� ����
	if (m_pFontMyEquipItem)
	{
		m_pFontMyEquipItem->DeleteDeviceObjects();
		util::del(m_pFontMyEquipItem);
	}
	// end 2009-02-03 by bhsohn ���� ������ �� ����

	m_pInchantNum->DeleteDeviceObjects();
	util::del(m_pInchantNum);

	if (m_pBigIcon)
	{
		m_pBigIcon->DeleteDeviceObjects();
		util::del(m_pBigIcon);
	}

	for (i = 0; i < 9; i++)
	{
		m_pInfoBoxSide[i]->DeleteDeviceObjects();
		util::del(m_pInfoBoxSide[i]);
	}
	//util::del(m_pGameData);
	return S_OK;

}

void CINFItemInfo::Render()
{
	FLOG("CINFItemInfo::Render()");

	// 2006-10-12 by ispark, ���� �Լ� Tick�� ó��
	SetOtherFuntionTick();

	int i;

	if (m_bShow && m_nDescIndex > 0 && m_bRestored)
	{
		int icongab;
		if (m_pBigIcon)
		{
			icongab = ITEMINFO_BIGICON_GAB;
			//m_ptItemInfo.y = m_ptItemInfo.y + ITEMINFO_BIGICON_GAB;
		}
		else icongab = 0;

		// 2006-03-08 by ispark 6*(m_nMaxLength+2) --> (m_nMaxLength+12)
		if (m_ptItemInfo.x + (m_nMaxLength + 12) > g_pD3dApp->GetBackBufferDesc().Width)
		{
			m_ptItemInfo.x = g_pD3dApp->GetBackBufferDesc().Width - (m_nMaxLength + 12);
		}
		if (m_ptItemInfo.y + 14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + 1) + 20 > g_pD3dApp->GetBackBufferDesc().Height)
		{
			m_ptItemInfo.y = (g_pD3dApp->GetBackBufferDesc().Height - ((14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + 1) + 20)));
		}
		// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		if (m_nExtendItemIndex > 0)
		{
			if (m_ptItemInfo.y + 14 * (m_nDescIndex + m_nDescLine) + 14 * (m_nDescIndex + m_nDescLine) + 20 > g_pD3dApp->GetBackBufferDesc().Height)
			{
				m_ptItemInfo.y = (g_pD3dApp->GetBackBufferDesc().Height - ((14 * (m_nDescIndex + m_nDescLine) + 14 * (m_nDescIndex + m_nDescLine + 1) + 20)));
			}
		}
		// end 2013-06-26 by ssjung �κ��丮 �߰� ���� 

	   /////////////////////////////////////////////////////////////////////////////
	   // ������				

	   // 2009-02-03 by bhsohn ���� ������ �� ����
	   //RenderInfoWindows(m_ptItemInfo.x,m_ptItemInfo.y-icongab,m_nMaxLength+12,14*(m_nDescIndex+1)+14*(m_nDescLine+1)+20+icongab);
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
		RenderInfoWindows(m_ptItemInfo.x, m_ptItemInfo.y - icongab,
			m_szTooltip.cx,
			m_szTooltip.cy);
		// end 2009-02-03 by bhsohn ���� ������ �� ����
#else
		g_pGameMain->m_pInfWindow->RenderCenterWindow(m_ptItemInfo.x, m_ptItemInfo.y - icongab, m_szTooltip.cx, m_szTooltip.cy, FALSE);
#endif

		int temp;

		if (m_strItemInfo[0][0])
		{
			char buff[128];
			memset(buff, 0x00, sizeof(buff[0] * 128));
			strcpy(buff, m_strItemInfo[0]);
			// 2006-03-08 by ispark, ��� �ٲ�
//			temp = ((strlen(m_strItemName)/2)*6.5)+7;
			temp = m_pFontItemName->GetStringSize(m_strItemName).cx / 2;
			if (m_pBigIcon)
			{
				m_pFontItemName->DrawText(m_ptItemInfo.x + ((m_nMaxLength + 12) / 2) - temp,
					(m_ptItemInfo.y - icongab) + ITEMINFO_NAME_IMAGE_GAB,
					GUI_FONT_COLOR,
					buff, 0L);
			}
			// 2008-09-26 by bhsohn �ű� ��þƮ ó��
			else if (m_pRefItemInfo && m_pRefItemInfo->GetEnchantNumber() > 0)
			{
				m_pFontItemName->DrawText(m_ptItemInfo.x + ((m_nMaxLength + 12) / 2) - temp,
					(m_ptItemInfo.y - icongab) + ITEMINFO_NAME_IMAGE_GAB - ITEMINFO_BIGICON_GAB + 8,
					GUI_FONT_COLOR,
					buff, 0L);
			}
			// end 2008-09-26 by bhsohn �ű� ��þƮ ó��
			else
			{
				m_pFontItemName->DrawText(m_ptItemInfo.x + ((m_nMaxLength + 12) / 2) - temp,
					(m_ptItemInfo.y - icongab) + ITEMINFO_NAME_IMAGE_GAB - ITEMINFO_BIGICON_GAB,
					GUI_FONT_COLOR,
					buff, 0L);
			}
		}

		if (m_pBigIcon)
		{
			temp = ITEMINFO_BIGICON_WIDTH;
			temp = ((m_nMaxLength + 12) / 2) - temp / 2;
			m_pBigIcon->Move(m_ptItemInfo.x + temp, (m_ptItemInfo.y - icongab) + ITEMINFO_TOP_GAB);
			m_pBigIcon->Render();

			if (m_pRefItemInfo && m_pRefItemInfo->GetEnchantNumber())
			{
				char buf[62];
				// 2007-03-29 by bhsohn China String
				//sprintf(buf, "\\eEnchant:%d\\e", m_pRefItemInfo->GetEnchantNumber());
				sprintf(buf, STRMSG_C_070329_0100, m_pRefItemInfo->GetEnchantNumber());
				// end 2007-03-29 by bhsohn China String
				m_pInchantNum->DrawText(m_ptItemInfo.x + temp + INCHANTNUM_X, (m_ptItemInfo.y - icongab) + ITEMINFO_TOP_GAB + 42, GUI_FONT_COLOR, buf, 0L);
			}
		}
		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
		else if (m_pRefItemInfo && m_pRefItemInfo->GetEnchantNumber() > 0)
		{
			temp = ITEMINFO_BIGICON_WIDTH;
			temp = ((m_nMaxLength + 12) / 2) - temp / 2;

			char buf[62];
			sprintf(buf, STRMSG_C_070329_0100, m_pRefItemInfo->GetEnchantNumber());
			m_pInchantNum->DrawText(m_ptItemInfo.x + temp + INCHANTNUM_X,
				(m_ptItemInfo.y - icongab) + ITEMINFO_NAME_IMAGE_GAB - ITEMINFO_BIGICON_GAB - 5,
				GUI_FONT_COLOR, buf, 0L);
		}
		// end 2008-09-26 by bhsohn �ű� ��þƮ ó��

		// 2009-02-03 by bhsohn ���� ������ �� ����
		int nItemPos = 0;
		for (i = 1; i < m_nDescIndex; i++)
		{
			if (m_strItemInfo[i][0])
			{
				m_pFontItemInfo[i]->DrawText(m_ptItemInfo.x + 5, m_ptItemInfo.y + 20 + 14 * i, GUI_FONT_COLOR, m_strItemInfo[i], 0L);
				nItemPos++;
			}
		}
		for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
		{
			if (m_strDesc[i][0])
			{
				m_pFontDescInfo[i]->DrawText(m_ptItemInfo.x + 5, m_ptItemInfo.y + 20 + 14 * (i + m_nDescIndex), GUI_FONT_COLOR, m_strDesc[i], 0L);
				nItemPos++;
			}
		}
		// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
		for (i = 0; i < ITEMINFO_EXTENDITEM_LINE_NUMBER; i++)
		{
			if (m_strExtendItemInfo[i][0])
			{
				m_pFontExtendItemInfo[i]->DrawText(m_ptItemInfo.x + 5, m_ptItemInfo.y + 20 + 14 * (i + m_nDescIndex + m_nDescLine), GUI_FONT_COLOR, m_strExtendItemInfo[i], 0L);
				nItemPos++;
			}
		}
#endif
		// end 2013-06-26 by ssjung �κ��丮 �߰� ���� 

		if (m_bMyEquipItem)
		{
			nItemPos++;
			char chTmpBuff[256];
			ZERO_MEMORY(chTmpBuff);
			wsprintf(chTmpBuff, STRMSG_C_090203_0202);
			m_pFontMyEquipItem->DrawText(m_ptItemInfo.x + 5, m_ptItemInfo.y + 20 + 14 * (nItemPos), GUI_FONT_COLOR, chTmpBuff, 0L);
		}
		// end 2009-02-03 by bhsohn ���� ������ �� ����

		// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
		int nX = m_ptItemInfo.x + 5;
		int nY = m_ptItemInfo.y + 20 + 14 * (nItemPos + 1) + 7;
		char szTemp[16];

		// ���Ǵ�Ƽ ��ȯ ������
		if (!m_vecExchageMtrl.empty())
		{
			for (std::vector< std::pair<ItemNum_t, InfinityShopItemCnt_t> >::iterator it = m_vecExchageMtrl.begin();
				it != m_vecExchageMtrl.end();
				++it)
			{
				// ������
				sprintf(szTemp, "%08d", (*it).first);
				g_pGameMain->m_pIcon->SetIcon(szTemp, nX, nY, 1.0f);
				g_pGameMain->m_pIcon->Render();

				// ����
				sprintf(szTemp, "%d", (*it).second);
				m_pFontMyEquipItem->DrawText(nX + 14 - m_pFontMyEquipItem->GetStringSize(szTemp).cx / 2, nY + 30, GUI_FONT_COLOR, szTemp);

				nX += 46;
			}
		}
		// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	}
}

void CINFItemInfo::Tick()
{
	FLOG("CINFItemInfo::Tick()");

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetItemName(int nParameterIndex)
/// \brief		������ �̸��� ���Ѵ�.
/// \author		dhkwon
/// \date		2004-04-02 ~ 2004-04-02
/// \warning	prefix, suffix�� �ٿ��ش�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemName(int nParameterIndex)
{
	if (m_pBigIcon)
		m_ptItemInfo.y = m_ptItemInfo.y + ITEMINFO_BIGICON_GAB;

	memset(m_strItemInfo[nParameterIndex], 0x00, ITEMINFO_ITEM_FULL_NAME);
	memset(m_strItemName, 0x00, ITEMINFO_ITEM_FULL_NAME);
	if (m_pRefPrefixRareInfo)
	{
		wsprintf(m_strItemName, "%s", m_pRefPrefixRareInfo->Name);
		wsprintf(m_strItemInfo[nParameterIndex], "\\g%s\\g", m_pRefPrefixRareInfo->Name);
	}
	if (m_bEnableItem)
	{
		if (IsStringColor(m_pRefITEM->ItemName))
		{
			wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
			wsprintf(m_strItemInfo[nParameterIndex], "%s %s", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
		}
		else if (m_pRefPrefixRareInfo || m_pRefSuffixRareInfo)
		{
			if (m_pRefEnchant)
			{
				wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
				wsprintf(m_strItemInfo[nParameterIndex], "%s \\e%s\\e", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
			}
			else
			{
				wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
				wsprintf(m_strItemInfo[nParameterIndex], "%s \\g%s\\g", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
			}
		}
		else
		{
			if (m_pRefEnchant)
			{
				wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
				wsprintf(m_strItemInfo[nParameterIndex], "%s \\e%s\\e", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
			}
			else
			{
				wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
				wsprintf(m_strItemInfo[nParameterIndex], "%s %s", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
			}
		}
	}
	else
	{
		wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefITEM->ItemName);
		wsprintf(m_strItemInfo[nParameterIndex], "%s \\r%s\\r", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemName);
	}
	if (m_pRefSuffixRareInfo)
	{
		wsprintf(m_strItemName, "%s %s", m_strItemName, m_pRefSuffixRareInfo->Name);
		wsprintf(m_strItemInfo[nParameterIndex], "%s \\g%s\\g", m_strItemInfo[nParameterIndex], m_pRefSuffixRareInfo->Name);
	}


	if (COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR) && m_pRefItemInfo)
	{
		wsprintf(m_strItemName, "%s (%I64d)", m_strItemName, m_pRefItemInfo->UniqueNumber);
		wsprintf(m_strItemInfo[nParameterIndex], "%s (%I64d)", m_strItemInfo[nParameterIndex], m_pRefItemInfo->UniqueNumber);
	}

	if (COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR))
	{
		wsprintf(m_strItemName, "%s (Idx:%d)", m_strItemName, m_pRefITEM->ItemNum);
		wsprintf(m_strItemInfo[nParameterIndex], "%s (Idx:%d)", m_strItemInfo[nParameterIndex], m_pRefITEM->ItemNum);
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CINFItemInfo::IsStringColor(char *i_szStr)
/// \brief		��Ʈ���� ������ �ִ��� üũ.
/// \author		dgwoo
/// \date		2008-04-16 ~ 2008-04-16
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFItemInfo::IsStringColor(char *i_szStr)
{
	BOOL bColor = FALSE;
	int  nCnt = 0;
	if (NULL == i_szStr)
		return bColor;
	while (strlen(i_szStr) > nCnt)
	{
		if (i_szStr[nCnt] == '\\')
		{
			bColor = TRUE;
			break;
		}
		nCnt++;
	}
	return bColor;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetItemKind( int nParameterIndex )
/// \brief		������ ����
/// \author		dhkwon
/// \date		2004-09-03 ~ 2004-09-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemKind(int nParameterIndex)
{
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0017, CAtumSJ::GetItemKindName(m_pRefITEM->Kind));//"���� : %s"
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetUnitKind(int nParameterIndex)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	RareInfo, Enchant ����
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetUnitKind(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetUnitKind(int nParameterIndex, USHORT nUnitKind)");
	strcpy(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0018);//"���� : "

	USHORT nUnitKind = m_pRefITEM->ReqUnitKind;
	if (UNITKIND_ALL_MASK == nUnitKind)
	{
		strcat(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0019);//"��������"
	}
	else if (0 == nUnitKind)
	{
		strcat(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0020);//"�Ϲ�"
	}
	else
	{
		if ((g_pShuttleChild->m_myShuttleInfo.UnitKind & m_pRefITEM->ReqUnitKind) == 0)
		{
			strcat(m_strItemInfo[nParameterIndex], "\\r");
		}
		// 2007-03-29 by bhsohn China String
		char chGear[64];
		memset(chGear, 0x00, 64);
		if (IS_BT(nUnitKind))
		{
			//strcat(m_strItemInfo[nParameterIndex],"B-GEAR ");
			wsprintf(chGear, "%s ", STRCMD_CS_UNITKIND_BGEAR);
			strcat(m_strItemInfo[nParameterIndex], chGear);
		}
		if (IS_OT(nUnitKind))
		{
			//strcat(m_strItemInfo[nParameterIndex],"M-GEAR ");
			wsprintf(chGear, "%s ", STRCMD_CS_UNITKIND_MGEAR);
			strcat(m_strItemInfo[nParameterIndex], chGear);
		}
		if (IS_DT(nUnitKind))
		{
			//strcat(m_strItemInfo[nParameterIndex],"A-GEAR ");
			wsprintf(chGear, "%s ", STRCMD_CS_UNITKIND_AGEAR);
			strcat(m_strItemInfo[nParameterIndex], chGear);
		}
		if (IS_ST(nUnitKind))
		{
			//strcat(m_strItemInfo[nParameterIndex],"I-GEAR ");
			wsprintf(chGear, "%s ", STRCMD_CS_UNITKIND_IGEAR);
			strcat(m_strItemInfo[nParameterIndex], chGear);
		}
		// end 2007-03-29 by bhsohn China String
	}
}
// 2012-10-11 by jhahn �Ⱓ�� ������ ǥ��
void CINFItemInfo::SetItemLimit(int* nParameterIndex)
{
	if (m_pRefItemInfo == NULL)
	{
		return;
	}




	if (m_pRefItemInfo->FixedTermShape.bActive)
	{
		CItemInfo* temp = g_pStoreData->FindItemInInventoryByUniqueNumber(m_pRefItemInfo->UniqueNumber);

		if (temp)
		{
			temp->FixedTermShape = m_pRefItemInfo->FixedTermShape;
		}


		ATUM_DATE_TIME curServerTime = GetServerDateTime();
		int nRemainSecond = (m_pRefItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - curServerTime.GetTimeInSeconds());

		int m_nDay = (nRemainSecond * 1000) / 86400000;
		int m_nHour = (nRemainSecond * 1000) % 86400000 / 3600000;
		int m_nMin = (nRemainSecond * 1000) % 86400000 % 3600000 / 60000;
		int m_nSec = (nRemainSecond * 1000) % 86400000 % 3600000 % 60000 / 1000;

		int nStrarttime = (m_pRefItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - m_pRefItemInfo->FixedTermShape.StartDate.GetTimeInSeconds());



		if ((nStrarttime * 1000) / 86400000)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "���Ⱓ : %d ��", (nStrarttime * 1000) / 86400000); // "��� �������� : ���Ǵ�Ƽ �ʵ�"
		}
		else if ((((nStrarttime * 1000) / 86400000) == NULL) && ((nStrarttime * 1000) / 3600000))
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "���Ⱓ : %d �ð�", (nStrarttime * 1000) / 3600000); // "��� �������� : ���Ǵ�Ƽ �ʵ�"
		}
		else if ((((nStrarttime * 1000) / 86400000) == NULL) && (((nStrarttime * 1000) / 3600000) == NULL))
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "���Ⱓ : %d ��", (nStrarttime * 1000) / 60000); // "��� �������� : ���Ǵ�Ƽ �ʵ�"
		}

		if (m_nDay)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "���� �ð� : %d �� , %d �ð� , %d �� ", m_nDay, m_nHour, m_nMin); // "��� �������� : ���Ǵ�Ƽ �ʵ�"
		}
		else if ((m_nDay == NULL) && m_nHour)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "���� �ð� : %d �ð� , %d �� ", m_nHour, m_nMin); // "��� �������� : ���Ǵ�Ƽ �ʵ�"
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && m_nMin)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "���� �ð� : %d �� ", m_nMin); // "��� �������� : ���Ǵ�Ƽ �ʵ�"
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && (m_nMin == NULL) && m_nSec)
		{
			sprintf(m_strItemInfo[(*nParameterIndex)++], "���� �ð� : 1�� ���� "); // "��� �������� : ���Ǵ�Ƽ �ʵ�"
		}
	}

}
// end 2012-10-11 by jhahn �Ⱓ�� ������ ǥ��
///////////////////////////////////////////////////////////////////////////////
/// \fn			SetReqLevel(int nParameterIndex, BYTE nReqLevel )
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	RareInfo, Enchant ����
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReqLevel(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetReqLevel(int nParameterIndex, BYTE nReqLevel )");
	if (m_pRefItemInfo != NULL)
	{
		ITEM* pRealItem = m_pRefItemInfo->GetRealItemInfo();

		if (pRealItem == NULL)
			return;


		// 2006-09-13 by ispark, ���� ���� �͸� �Ǵ����� �Ǿ� �־ ReqMaxLevel ���Խ�Ŵ
//		if(m_bEnableItem || (g_pShuttleChild->m_myShuttleInfo.Level >= pRealItem->ReqMinLevel &&
//			g_pShuttleChild->m_myShuttleInfo.Level <= pRealItem->ReqMinLevel))
		if (m_bEnableItem || (g_pShuttleChild->m_myShuttleInfo.Level >= pRealItem->ReqMinLevel &&
			g_pShuttleChild->m_myShuttleInfo.Level <= pRealItem->ReqMaxLevel))
		{
			if (pRealItem->ReqMaxLevel == 0)
			{
				// 2008-09-26 by bhsohn �ű� ��þƮ ó��
				float fPreSuffixLV = GetEnchantPreSuffixInfo(DES_REQ_MIN_LEVEL);

				CParamFactor *pPFactor = m_pRefItemInfo->GetParamFactor();
				if (pPFactor
					&& 0 != pPFactor->pfp_REQ_MIN_LEVEL)
				{
					if (fPreSuffixLV > pPFactor->pfp_REQ_MIN_LEVEL)
					{
						int nMinLevel = pPFactor->pfp_REQ_MIN_LEVEL - fPreSuffixLV;
						if (nMinLevel != 0)
						{
							// ���� ��þ���� ����
							wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0022, m_pRefITEM->ReqMinLevel);//"�䱸 : LEVEL[%d]"
							wsprintf(m_strItemInfo[nParameterIndex], "%s\\e[%d]\\e", m_strItemInfo[nParameterIndex], nMinLevel);//"�䱸 : LEVEL[%d]"
						}
						else
						{
							// ���� ��þ �� ����, ���̿ɼ�
							wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0021, m_pRefITEM->ReqMinLevel, pPFactor->pfp_REQ_MIN_LEVEL);//"�䱸 : LEVEL(%d\\g[%d]\\g)"						
							wsprintf(m_strItemInfo[nParameterIndex], "%s\\e[%d]\\e", m_strItemInfo[nParameterIndex], nMinLevel);//"�䱸 : LEVEL[%d]"
						}

					}
					else
					{
						// ����, ���� �ɼǸ� �ִ� ����
						wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0021, m_pRefITEM->ReqMinLevel, pPFactor->pfp_REQ_MIN_LEVEL);//"�䱸 : LEVEL(%d\\g[%d]\\g)"						

					}
				}
				else
				{
					wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0022, m_pRefITEM->ReqMinLevel);//"�䱸 : LEVEL[%d]"
				}
			}
			else
			{
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0023, //"�䱸 : LEVEL[%d]~[%d]"
					pRealItem->ReqMinLevel, pRealItem->ReqMaxLevel);
			}
		}
		else
		{
			if (pRealItem->ReqMaxLevel == 0)
			{
				CParamFactor *pPFactor = m_pRefItemInfo->GetParamFactor();
				if (pPFactor
					&& 0 != pPFactor->pfp_REQ_MIN_LEVEL)
				{
					wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0024, m_pRefITEM->ReqMinLevel, pPFactor->pfp_REQ_MIN_LEVEL);//"�䱸 : \\rLEVEL(%d\\g[%d]\\r)\\r"
				}
				else
				{
					wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0025, pRealItem->ReqMinLevel); //"�䱸 : \\rLEVEL[%d]\\r"
				}
			}
			else
			{
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0026, //"�䱸 : \\rLEVEL[%d]~[%d]"
					pRealItem->ReqMinLevel, pRealItem->ReqMaxLevel);
			}
		}
	}
	else
	{
		if (m_bEnableItem || (g_pShuttleChild->m_myShuttleInfo.Level >= m_pRefITEM->ReqMinLevel &&
			g_pShuttleChild->m_myShuttleInfo.Level <= m_pRefITEM->ReqMinLevel))
		{
			if (m_pRefITEM->ReqMaxLevel == 0)
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0022, m_pRefITEM->ReqMinLevel);
			else
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0023,
					m_pRefITEM->ReqMinLevel, m_pRefITEM->ReqMaxLevel);
		}
		else
		{
			if (m_pRefITEM->ReqMaxLevel == 0)
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0025, m_pRefITEM->ReqMinLevel);
			else
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0026,
					m_pRefITEM->ReqMinLevel, m_pRefITEM->ReqMaxLevel);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SetShopReqItem(int nParameterIndex )
/// \brief		��ų �������� ���Խÿ� �ʿ��� ������ �����ֱ�
/// \author		dhkwon
/// \date		2004-11-11 ~ 2004-11-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetShopReqItem(int nParameterIndex)
{
	ITEM* pITEM = g_pDatabase->GetServerItemInfo(m_pRefITEM->LinkItem);
	if (pITEM == NULL)
	{
		return;
	}
	if (g_pStoreData->FindItemInInventoryByItemNum(m_pRefITEM->LinkItem) == NULL)
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0027, pITEM->ItemName); //"���Խ� �ʿ� : \\r%s\\r"
	}
	else
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0028, pITEM->ItemName); //"���Խ� �ʿ� : %s"
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetReqStat(int nParameterIndex )
/// \brief		
/// \author		dhkwon
/// \date		2004-09-09 ~ 2004-09-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReqStat(int nParameterIndex)
{
	strcpy(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0008);//"�䱸���� :"
	if (m_pRefITEM->ReqGearStat.AttackPart != 0)
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart >= m_pRefITEM->ReqGearStat.AttackPart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0009, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.AttackPart); //"%s ����[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0010, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.AttackPart); //"%s \\r����[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.DefensePart != 0)//����->���
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart >= m_pRefITEM->ReqGearStat.DefensePart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0011, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.DefensePart); //"%s ���[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0012, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.DefensePart); //"%s \\r���[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.FuelPart != 0)
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart >= m_pRefITEM->ReqGearStat.FuelPart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0013, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.FuelPart); //"%s ����[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0014, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.FuelPart); //"%s \\r����[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.SoulPart != 0)
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart >= m_pRefITEM->ReqGearStat.SoulPart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0015, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.SoulPart); //"%s ����[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0016, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.SoulPart); //"%s \\r����[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.ShieldPart != 0)//���->����
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart >= m_pRefITEM->ReqGearStat.ShieldPart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0017, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.ShieldPart); //"%s ����[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0018, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.ShieldPart); //"%s \\r����[%d]\\r"
		}
	}
	if (m_pRefITEM->ReqGearStat.DodgePart != 0)
	{
		if (g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart >= m_pRefITEM->ReqGearStat.DodgePart)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0019, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.DodgePart); //"%s ȸ��[%d]"
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_STAT_0020, m_strItemInfo[nParameterIndex], m_pRefITEM->ReqGearStat.DodgePart); //"%s \\rȸ��[%d]\\r"
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetReqItemKind( int nParameterIndex )
/// \brief		�䱸 ������ ����
/// \author		dhkwon
/// \date		2004-09-03 ~ 2004-09-03
/// \warning	��ų,��æƮ ī�忡�� ���
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReqItemKind(int nParameterIndex)
{
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0029, CAtumSJ::GetItemKindName(m_pRefITEM->ReqItemKind)); //"���� ������ : %s"
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetAttack(int nParameterIndex)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	Enchant����, RareInfo���� ���� 
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetAttack(int nParameterIndex)
{
	float fEnchantAttackMin = m_pRefEnchant ? m_pRefEnchant->pfm_MINATTACK_01 + m_pRefEnchant->pfm_MINATTACK_02 : 0;
	float fEnchantAttackMax = m_pRefEnchant ? m_pRefEnchant->pfm_MAXATTACK_01 + m_pRefEnchant->pfm_MAXATTACK_02 : 0;
	float fRareInfoAttackMin = 0;
	float fRareInfoAttackMax = 0;
	BYTE nEnchantShotNum = m_pRefEnchant ? m_pRefEnchant->pfp_SHOTNUM_01 : 0;
	BYTE nEnchantMultiNum = m_pRefEnchant ? m_pRefEnchant->pfp_MULTINUM_02 : 0;
	BYTE nRareInfoShotNum = 0;
	BYTE nRareInfoMultiNum = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_MINATTACK_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_MINATTACK_02)
			{
				fRareInfoAttackMin += m_pRefPrefixRareInfo->ParameterValue[i];
			}
			else if (m_pRefPrefixRareInfo->DesParameter[i] == DES_MAXATTACK_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_MAXATTACK_02)
			{
				fRareInfoAttackMax += m_pRefPrefixRareInfo->ParameterValue[i];
			}
			else if (m_pRefPrefixRareInfo->DesParameter[i] == DES_SHOTNUM_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_SHOTNUM_02)
			{
				nRareInfoShotNum += (BYTE)m_pRefPrefixRareInfo->ParameterValue[i];
			}
			else if (//m_pRefPrefixRareInfo->DesParameter[i] == DES_MULTINUM_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_MULTINUM_02)
			{
				nRareInfoMultiNum += (BYTE)m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_MINATTACK_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_MINATTACK_02)
			{
				fRareInfoAttackMin += m_pRefSuffixRareInfo->ParameterValue[i];
			}
			else if (m_pRefSuffixRareInfo->DesParameter[i] == DES_MAXATTACK_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_MAXATTACK_02)
			{
				fRareInfoAttackMax += m_pRefSuffixRareInfo->ParameterValue[i];
			}
			else if (m_pRefSuffixRareInfo->DesParameter[i] == DES_SHOTNUM_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_SHOTNUM_02)
			{
				nRareInfoShotNum += (BYTE)m_pRefSuffixRareInfo->ParameterValue[i];
			}
			else if (//m_pRefSuffixRareInfo->DesParameter[i] == DES_MULTINUM_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_MULTINUM_02)
			{
				nRareInfoMultiNum += (BYTE)m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}

	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0030, m_pRefITEM->AbilityMin);//"���� : (%.0f"
	if (fEnchantAttackMin > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantAttackMin*100.0f);
	else if (fEnchantAttackMin < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantAttackMin*100.0f);
	if (fRareInfoAttackMin > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoAttackMin*100.0f);
	else if (fRareInfoAttackMin < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoAttackMin*100.0f);

	sprintf(m_strItemInfo[nParameterIndex], "%s ~ %.0f", m_strItemInfo[nParameterIndex], m_pRefITEM->AbilityMax);
	if (fEnchantAttackMax > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantAttackMax*100.0f);
	else if (fEnchantAttackMin < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantAttackMax*100.0f);
	if (fRareInfoAttackMax > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoAttackMax*100.0f);
	else if (fRareInfoAttackMin < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoAttackMax*100.0f);
	sprintf(m_strItemInfo[nParameterIndex], "%s) X (%d", m_strItemInfo[nParameterIndex], m_pRefITEM->ShotNum);

	if (nEnchantShotNum > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%d]\\e", m_strItemInfo[nParameterIndex], nEnchantShotNum);
	else if (nEnchantShotNum < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%d]\\e", m_strItemInfo[nParameterIndex], nEnchantShotNum);
	if (nRareInfoShotNum > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%d]\\g", m_strItemInfo[nParameterIndex], nRareInfoShotNum);
	else if (nRareInfoShotNum < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%d]\\g", m_strItemInfo[nParameterIndex], nRareInfoShotNum);
	sprintf(m_strItemInfo[nParameterIndex], "%s X %d", m_strItemInfo[nParameterIndex], m_pRefITEM->MultiNum);

	if (nEnchantMultiNum > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%d]\\e", m_strItemInfo[nParameterIndex], nEnchantMultiNum);
	else if (nEnchantMultiNum < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%d]\\e", m_strItemInfo[nParameterIndex], nEnchantMultiNum);
	if (nRareInfoMultiNum > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%d]\\g", m_strItemInfo[nParameterIndex], nRareInfoMultiNum);
	else if (nRareInfoMultiNum < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%d]\\g", m_strItemInfo[nParameterIndex], nRareInfoMultiNum);
	strcat(m_strItemInfo[nParameterIndex], ")");
}
void CINFItemInfo::SetSecAttack(int nParameterIndex)
{
	if (m_pRefItemInfo)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0031, CAtumSJ::GetMinAttackPerSecond(m_pRefItemInfo->GetRealItemInfo()));//"�ʴ� ���ݷ� : %.0f"
		sprintf(m_strItemInfo[nParameterIndex], "%s ~ %.0f", m_strItemInfo[nParameterIndex], CAtumSJ::GetMaxAttackPerSecond(m_pRefItemInfo->GetRealItemInfo()));
	}
	else
	{
		ASSERT_ASSERT(m_pRefITEM);
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0031, CAtumSJ::GetMinAttackPerSecond(m_pRefITEM));
		sprintf(m_strItemInfo[nParameterIndex], "%s ~ %.0f", m_strItemInfo[nParameterIndex], CAtumSJ::GetMaxAttackPerSecond(m_pRefITEM));
	}
}
void CINFItemInfo::SetDefense(int nParameterIndex)
{
	float fEnchantDefense = m_pRefEnchant ? m_pRefEnchant->pfp_DEFENSE_01 + m_pRefEnchant->pfp_DEFENSE_02 : 0;
	float fRareInfoDefense = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_DEFENSE_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_DEFENSE_02)
			{
				fRareInfoDefense += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_DEFENSE_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_DEFENSE_02)
			{
				fRareInfoDefense += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0032, m_pRefITEM->AbilityMin);//"��� : %.0f"
	if (fEnchantDefense > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.0f]\\e", m_strItemInfo[nParameterIndex], fEnchantDefense);
	else if (fEnchantDefense < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.0f]\\e", m_strItemInfo[nParameterIndex], fEnchantDefense);
	if (fRareInfoDefense > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.0f]\\g", m_strItemInfo[nParameterIndex], fRareInfoDefense);
	else if (fRareInfoDefense < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.0f]\\g", m_strItemInfo[nParameterIndex], fRareInfoDefense);
}

void CINFItemInfo::SetRate(int nParameterIndex)
{
	// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
	float nEnchantHitRate = m_pRefEnchant ? m_pRefEnchant->pfp_ATTACKPROBABILITY_01 + m_pRefEnchant->pfp_ATTACKPROBABILITY_02 : 0;
	float nRareInfoHitRate = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_ATTACKPROBABILITY_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_ATTACKPROBABILITY_02)
			{
				// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
				//nRareInfoHitRate += (Prob255_t)m_pRefPrefixRareInfo->ParameterValue[i];
				nRareInfoHitRate += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_ATTACKPROBABILITY_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_ATTACKPROBABILITY_02)
			{
				// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
				//nRareInfoHitRate += (Prob255_t)m_pRefSuffixRareInfo->ParameterValue[i];
				nRareInfoHitRate += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0033, m_pRefITEM->HitRate);//"Ȯ�� : %.2f%%"
	
	if (nEnchantHitRate > 0)
		
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.2f%%]\\e", m_strItemInfo[nParameterIndex], nEnchantHitRate);
	
	else if (nEnchantHitRate < 0)
		
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.2f%%]\\e", m_strItemInfo[nParameterIndex], nEnchantHitRate);
	
	if (nRareInfoHitRate > 0)
		
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.2f%%]\\g", m_strItemInfo[nParameterIndex], nRareInfoHitRate);
	
	else if (nRareInfoHitRate < 0)
		
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.2f%%]\\g", m_strItemInfo[nParameterIndex], nRareInfoHitRate);
}

void CINFItemInfo::SetRange(int nParameterIndex)
{
	float fEnchantRange = m_pRefEnchant ? m_pRefEnchant->pfm_RANGE_01 + m_pRefEnchant->pfm_RANGE_02 : 0;
	float fRareInfoRange = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_RANGE_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_RANGE_02)
			{
				fRareInfoRange += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_RANGE_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_RANGE_02)
			{
				fRareInfoRange += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0034, m_pRefITEM->Range);//"�Ÿ� : %dm"
	if (fEnchantRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantRange * 100);
	else if (fEnchantRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantRange * 100);
	if (fRareInfoRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoRange * 100);
	else if (fRareInfoRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoRange * 100);

}

void CINFItemInfo::SetReAttackTime(int nParameterIndex)
{
	float fEnchantReAttacktime = m_pRefEnchant ? m_pRefEnchant->pfm_REATTACKTIME_01 + m_pRefEnchant->pfm_REATTACKTIME_02 : 0;
	float fRareInfoReAttacktime = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_REATTACKTIME_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_REATTACKTIME_02)
			{
				fRareInfoReAttacktime += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_REATTACKTIME_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_REATTACKTIME_02)
			{
				fRareInfoReAttacktime += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0035, (float)m_pRefITEM->ReAttacktime / 1000.0f);//"����ݽð� : %.2fsec"
	if (fEnchantReAttacktime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantReAttacktime * 100);
	else if (fEnchantReAttacktime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantReAttacktime * 100);
	if (fRareInfoReAttacktime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoReAttacktime * 100);
	else if (fRareInfoReAttacktime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoReAttacktime * 100);

}

void CINFItemInfo::SetOverHeatTime(int nParameterIndex)
{
	float fEnchantOverheatTime = m_pRefEnchant ? m_pRefEnchant->pfm_TIME_01 + m_pRefEnchant->pfm_TIME_02 : 0;
	float fRareInfoOverheatTime = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_TIME_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_TIME_02)
			{
				fRareInfoOverheatTime += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_TIME_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_TIME_02)
			{
				fRareInfoOverheatTime += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0036, (float)m_pRefITEM->Time / 1000.0f);//"�����ð� : %.2fsec"
	if (fEnchantOverheatTime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantOverheatTime * 100);
	else if (fEnchantOverheatTime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantOverheatTime * 100);
	if (fRareInfoOverheatTime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoOverheatTime * 100);
	else if (fRareInfoOverheatTime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoOverheatTime * 100);

}

void CINFItemInfo::SetRangeAngle(int nParameterIndex)
{
	float fEnchantRangeAngle = m_pRefEnchant ? m_pRefEnchant->pfp_RANGEANGLE_01 + m_pRefEnchant->pfp_RANGEANGLE_02 : 0;
	float fRareInfoRangeAngle = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_RANGEANGLE_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_RANGEANGLE_02)
			{
				fRareInfoRangeAngle += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_RANGEANGLE_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_RANGEANGLE_02)
			{
				fRareInfoRangeAngle += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0037, (int)(m_pRefITEM->RangeAngle / PI * 180));//"��ȿ���� : %d��"
	if (fEnchantRangeAngle > 0)
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0038, m_strItemInfo[nParameterIndex], fEnchantRangeAngle / PI * 180);//"%s\\e[+%.1f��]\\e"
	else if (fEnchantRangeAngle < 0)
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0039, m_strItemInfo[nParameterIndex], fEnchantRangeAngle / PI * 180);//"%s\\e[%.1f��]\\e"
	if (fRareInfoRangeAngle > 0)
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0040, m_strItemInfo[nParameterIndex], fRareInfoRangeAngle / PI * 180);//"%s\\g[+%.1f��]\\g"
	else if (fRareInfoRangeAngle < 0)
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0041, m_strItemInfo[nParameterIndex], fRareInfoRangeAngle / PI * 180);//"%s\\g[%.1f��]\\g"


}

void CINFItemInfo::SetExplosionRange(int nParameterIndex)
{
	float fEnchantExplosionRange = m_pRefEnchant ? m_pRefEnchant->pfp_EXPLOSIONRANGE_01 + m_pRefEnchant->pfp_EXPLOSIONRANGE_02 : 0;
	float fRareInfoExplosionRange = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_EXPLOSIONRANGE_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_EXPLOSIONRANGE_02)
			{
				fRareInfoExplosionRange += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_EXPLOSIONRANGE_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_EXPLOSIONRANGE_02)
			{
				fRareInfoExplosionRange += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0042, m_pRefITEM->ExplosionRange);//"���߹ݰ� : %dm"
	if (fEnchantExplosionRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%dm]\\e", m_strItemInfo[nParameterIndex], (int)fEnchantExplosionRange);
	else if (fEnchantExplosionRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%dm]\\e", m_strItemInfo[nParameterIndex], (int)fEnchantExplosionRange);
	if (fRareInfoExplosionRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%dm]\\g", m_strItemInfo[nParameterIndex], (int)fRareInfoExplosionRange);
	else if (fRareInfoExplosionRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%dm]\\g", m_strItemInfo[nParameterIndex], (int)fRareInfoExplosionRange);
}

void CINFItemInfo::SetReactionRange(int nParameterIndex)
{
	float fEnchantReactionRange = m_pRefEnchant ? m_pRefEnchant->pfp_REACTION_RANGE : 0;
	float fRareInfoReactionRange = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_REACTION_RANGE)
			{
				fRareInfoReactionRange += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_REACTION_RANGE)
			{
				fRareInfoReactionRange += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0043, m_pRefITEM->ExplosionRange);//"�����ݰ� : %dm"
	if (fEnchantReactionRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%dm]\\e", m_strItemInfo[nParameterIndex], (int)fEnchantReactionRange);
	else if (fEnchantReactionRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%dm]\\e", m_strItemInfo[nParameterIndex], (int)fEnchantReactionRange);
	if (fRareInfoReactionRange > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%dm]\\g", m_strItemInfo[nParameterIndex], (int)fRareInfoReactionRange);
	else if (fRareInfoReactionRange < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%dm]\\g", m_strItemInfo[nParameterIndex], (int)fRareInfoReactionRange);
}

void CINFItemInfo::SetSkillLevel(int nParameterIndex)
{
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_SKILL_0005, SKILL_LEVEL(m_pRefITEM->ItemNum));//"��ų���� : [%d]"
}


void CINFItemInfo::SetSkillTime(int nParameterIndex)
{
	float fEnchantTime = m_pRefEnchant ? m_pRefEnchant->pfm_TIME_01 + m_pRefEnchant->pfm_TIME_02 : 0;
	float fRareInfoTime = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_TIME_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_TIME_02)
			{
				fRareInfoTime += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_TIME_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_TIME_02)
			{
				fRareInfoTime += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	//	sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_SKILL_0006, (float)m_pRefITEM->Time/1000.0f) ;//"�ð� : %.2fsec"
	// 2005-11-22 by ispark �ð� -> �ߵ� �ð�, ��ߵ� �ð� : �߰�
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051122_0006, (float)m_pRefITEM->Time / 1000.0f);//"�ߵ� �ð� : %.2fsec"
	if (fEnchantTime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantTime * 100);
	else if (fEnchantTime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.2f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantTime * 100);
	if (fRareInfoTime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoTime * 100);
	else if (fRareInfoTime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.2f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoTime * 100);
}

void CINFItemInfo::SetAttackTime(int nParameterIndex)
{
	UINT nEnchantAttacktime = m_pRefEnchant ? m_pRefEnchant->pfp_ATTACKTIME_01 + m_pRefEnchant->pfp_ATTACKTIME_02 : 0;
	UINT nRareInfoAttacktime = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_ATTACKTIME_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_ATTACKTIME_02)
			{
				nRareInfoAttacktime += (UINT)m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_ATTACKTIME_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_ATTACKTIME_02)
			{
				nRareInfoAttacktime += (UINT)m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0044, (float)m_pRefITEM->AttackTime / 1000.0f);//"�ߵ� : %.2fsec"
	if (nEnchantAttacktime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%.2fsec]\\e", m_strItemInfo[nParameterIndex], (float)nEnchantAttacktime / 1000.0f);
	else if (nEnchantAttacktime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%.2fsec]\\e", m_strItemInfo[nParameterIndex], (float)nEnchantAttacktime / 1000.0f);
	if (nRareInfoAttacktime > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%.2fsec]\\g", m_strItemInfo[nParameterIndex], (float)nRareInfoAttacktime / 1000.0f);
	else if (nRareInfoAttacktime < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%.2fsec]\\g", m_strItemInfo[nParameterIndex], (float)nRareInfoAttacktime / 1000.0f);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SetWeight( int nParameterIndex)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	Enchant ���� , RareInfo ���� ����
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetWeight(int nParameterIndex)
{
	//USHORT nEnchantWeight = m_pRefEnchant ? m_pRefEnchant->pfm_WEIGHT : 0;
	float fEnchantWeight = m_pRefEnchant ? m_pRefEnchant->pfm_WEIGHT_01 + m_pRefEnchant->pfm_WEIGHT_02 : 0;
	float fRareInfoWeight = 0;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == DES_WEIGHT_01 ||
				m_pRefPrefixRareInfo->DesParameter[i] == DES_WEIGHT_02)
			{
				fRareInfoWeight += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == DES_WEIGHT_01 ||
				m_pRefSuffixRareInfo->DesParameter[i] == DES_WEIGHT_02)
			{
				fRareInfoWeight += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0045, m_pRefITEM->Weight);//"�߷� : %d"
	if (fEnchantWeight > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantWeight * 100);
	else if (fEnchantWeight < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantWeight * 100);
	if (fRareInfoWeight > 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%2.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoWeight * 100);
	else if (fRareInfoWeight < 0)
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[%2.0f%%]\\g", m_strItemInfo[nParameterIndex], fRareInfoWeight * 100);

}

// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
void CINFItemInfo::SetShapeInfo(int nParameterIndex)
{
	char buf[256];
	ZERO_MEMORY(buf);

	if (!m_pRefItemInfo->ShapeItemNum)
		strcpy(buf, STRMSG_C_090901_0302);	// "�⺻����"
	else
	{
		ITEM* pITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->ShapeItemNum);
		if (!pITEM)
			strcpy(buf, STRMSG_C_090901_0302);// "�⺻����"
		else
		{
			// 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
			if (0 < m_pRefItemInfo->FixedTermShape.nStatLevel && m_pRefItemInfo->FixedTermShape.nStatShapeItemNum)
			{
				wsprintf(buf, "\\e" STRMSG_C_070607_0107 "\\e", m_pRefItemInfo->FixedTermShape.nStatLevel);		//"%dȸ"   //2013-05-20 by ssjung +���ڸ� +ȸ�� ���� 
				wsprintf(buf, STRMSG_C_130419_0002, buf, pITEM->ItemName);						//"%s %s"
			}
			else strcpy(buf, pITEM->ItemName);
#else
			strcpy(buf, pITEM->ItemName);
#endif
		}
	}
	// 2013-04-19 by ssjung ���� ���� Ŷ ��ȭ �ý���
	// 	if(m_pRefItemInfo->FixedTermShape.nStatShapeItemNum)
	// 	{
	// 		ITEM* pLOOKITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->FixedTermShape.nStatShapeItemNum);
	// 		if(pLOOKITEM)
	// 		{
	// 			if(0 < m_pRefItemInfo->FixedTermShape.nStatLevel)
	// //				wsprintf(buf, STRMSG_C_130419_0001, m_pRefItemInfo->FixedTermShape.nStatLevel);		//"\\e+%d\\e"
	// 				wsprintf(buf, "\\e"STRMSG_C_070607_0107"\\e", m_pRefItemInfo->FixedTermShape.nStatLevel);		//"%dȸ"   //2013-05-20 by ssjung +���ڸ� +ȸ�� ���� 
	// 			wsprintf(buf, STRMSG_C_130419_0002, buf, pLOOKITEM->ItemName);						//"%s %s"
	// 		}
	// 	}
	// #endif
	// end 2013-04-19 by ssjung ���� ���� Ŷ ��ȭ �ý���

	// 2012-10-13 by jhahn �Ⱓ�� ������ �߰�
	if (m_pRefItemInfo->FixedTermShape.bActive)
	{

		ATUM_DATE_TIME curServerTime = GetServerDateTime();
		int nRemainSecond = (m_pRefItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - curServerTime.GetTimeInSeconds());

		// 		int m_nDay	= (nRemainSecond * 1000) / 86400000;
		// 		int m_nHour	= (nRemainSecond * 1000) % 86400000 / 3600000;
		// 		int m_nMin	= (nRemainSecond * 1000) % 86400000 % 3600000 /60000;
		// 		int m_nSec	= (nRemainSecond * 1000) % 86400000 % 3600000 % 60000 / 1000;

		int m_nDay = (nRemainSecond) / 86400;
		int m_nHour = (nRemainSecond) % 86400 / 3600;
		int m_nMin = (nRemainSecond) % 86400 % 3600 / 60;
		int m_nSec = (nRemainSecond) % 86400 % 3600 % 60 / 1;

		int nStrarttime = (m_pRefItemInfo->FixedTermShape.EndDate.GetTimeInSeconds() - m_pRefItemInfo->FixedTermShape.StartDate.GetTimeInSeconds());

		// 2013-05-06 by ssjung ���� �ð��� -�� ���� ���ܻ��� ó�� �߰�
		if (0 > nStrarttime)
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "���� : %s"
			return;
		}
		// end 2013-05-06 by ssjung ���� �ð��� -�� ���� ���ܻ��� ó�� �߰�

		if (m_nDay)
		{
			wsprintf(buf, STRMSG_C_121013_0101, buf, m_nDay, m_nHour, m_nMin);
			//			wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "���� : %s"				
		}
		else if ((m_nDay == NULL) && m_nHour)
		{
			wsprintf(buf, STRMSG_C_121013_0102, buf, m_nHour, m_nMin);
			//			wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "���� : %s"		// 2013-04-22 by ssjung "���� : %s"�� �κ��� �ߺ� �Ǿ�  �ڿ� ó�� �ϵ��� ���� 
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && m_nMin)
		{
			wsprintf(buf, STRMSG_C_121013_0103, buf, m_nMin);
			//			wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "���� : %s"
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && (m_nMin == NULL) && m_nSec)
		{
			wsprintf(buf, STRMSG_C_121013_0104, buf);
			//			wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "���� : %s"
		}
	}
	else
	{
		// 2013-04-19 by ssjung ���� ���� Ŷ ��ȭ �ý���
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM 
		if (0 < m_pRefItemInfo->FixedTermShape.nStatLevel && 10 > m_pRefItemInfo->FixedTermShape.nStatLevel && g_pD3dApp->GetArenaState() != ARENA_STATE_ARENA_GAMING) //�ܺ�Ŷ ���� 
			wsprintf(buf, STRMSG_C_130419_0003, buf);				// "%s(\\r�Ⱓ ����\\r)"
#endif
// end 2013-04-19 by ssjung ���� ���� Ŷ ��ȭ �ý���
	}
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_090901_0301, buf);	// "���� : %s"
//end 2012-10-13 by jhahn �Ⱓ�� ������ �߰�
}

void CINFItemInfo::SetEffectInfo(int nParameterIndex)
{
	char buf[256];
	ZERO_MEMORY(buf);

	if (!m_pRefItemInfo->ColorCode)
		strcpy(buf, STRMSG_C_090901_0304);	// "�⺻����Ʈ"
	else
	{
		ITEM* pITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->ColorCode);
		if (!pITEM)
			strcpy(buf, STRMSG_C_090901_0304);// "�⺻����Ʈ"
		else
			strcpy(buf, pITEM->ItemName);
	}

	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_090901_0303, buf);	// "����Ʈ : %s"
}
// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����


// 2010. 02. 11 by ckPark �ߵ��� ����������
void CINFItemInfo::SetInvokeDestParam(int* pParameterIndex)
{
	// �ߵ��� �����Ķ� ���� �߰�
	if (m_pRefITEM && m_pRefITEM->InvokingDestParamID)
	{
		// �����Ķ� ��� ������
		CVectorInvokingWearItemDP dpList;
		g_pDatabase->GetInvokingWearItemDPList(&dpList, m_pRefITEM->InvokingDestParamID);

		if (!dpList.empty())
		{
			CVectorInvokingWearItemDPIt it = dpList.begin();
			while (it != dpList.end())
			{
				// ��� ���� �߰�
				SetFunction((*pParameterIndex)++, (*it)->InvokingDestParam, (*it)->InvokingDestParamValue, 0, 0, FUNCTIONTYPE_EQUIP);
				++it;
			}
		}
	}

	// ���� �����Ķ� ���� �߰�
	if (m_pRefITEM && m_pRefITEM->InvokingDestParamIDByUse)
	{
		// �����Ķ� ��� ������
		CVectorInvokingWearItemDP dpList;
		g_pDatabase->GetInvokingWearItemDPByUseList(&dpList, m_pRefITEM->InvokingDestParamIDByUse);

		if (!dpList.empty())
		{
			CVectorInvokingWearItemDPIt it = dpList.begin();
			while (it != dpList.end())
			{
				// ��� ���� �߰�
				SetFunction((*pParameterIndex)++, (*it)->InvokingDestParam, (*it)->InvokingDestParamValue, 0, 0, FUNCTIONTYPE_USE);
				++it;
			}
		}
	}
}

void CINFItemInfo::SetItemCoolTime(int* pParameterIndex, BOOL bSetTick /* = TRUE */)
{
	// ��Ÿ�� ���� �߰�
	if (m_pRefItemInfo)
	{
		int nSec = ((int)(m_pRefItemInfo->ItemInfo->ReAttacktime) - m_pRefItemInfo->GetCoolElapsedTime()) / 1000;
		if (nSec < 0)
			nSec = 0;

		sprintf(m_strItemInfo[*pParameterIndex],
			STRMSG_C_100218_0307, nSec);	// "��ߵ� �ð� : %2dsec"

		if (bSetTick)
		{
			// ���� ��Ÿ�ӿ� �߰�
			stTickFuntionIndex stAddTemp;
			stAddTemp.nFuntionIndex = FUNCTION_INDEX_ITEM_COOL_TIME;
			stAddTemp.nDataLineIndex = *pParameterIndex;
			m_vecTickFuntionIndex.push_back(stAddTemp);

			++(*pParameterIndex);
		}
	}
}
// end 2010. 02. 11 by ckPark �ߵ��� ����������


///////////////////////////////////////////////////////////////////////////////
/// \fn			SetDefenseColorInfo(int  nParameterIndex )
/// \brief		
/// \author		ydkim
/// \date		2005-12-09
/// \warning	�Ƹ� Į�� ����
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetDefenseColorInfo(int  nParameterIndex)
{
	char buf[256];
	ZERO_MEMORY(buf);

	ITEM* pITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->ColorCode);
	if (NULL == pITEM)
	{
		pITEM = g_pDatabase->GetServerItemInfo(m_pRefITEM->SourceIndex + 1);
	}

	if (pITEM)
		strcpy(buf, pITEM->ItemName);
	else
		strcpy(buf, STRMSG_C_051208_0001);	//"�⺻Į��"

	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051208_0002, buf);	// "�������� : %s"
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			SetCountableWeight( int nParameterIndex)
/// \brief		COUNTABLE ITEM�� ���� ����
/// \author		dhkwon
/// \date		2004-09-17 ~ 2004-09-17
/// \warning	Enchant,Prefix,Suffix�� ����.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetCountableWeight(int nParameterIndex)
{
	int nCount = 1;
	if (m_pRefItemInfo)
	{
		nCount = m_pRefItemInfo->CurrentCount / m_pRefITEM->MinTradeQuantity;
		int nRemain = m_pRefItemInfo->CurrentCount%m_pRefITEM->MinTradeQuantity;
		if (nRemain > 0)
		{
			nCount += 1;
		}
	}
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0045,
		(int)m_pRefITEM->Weight*nCount);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			SetReqSP( int nParameterIndex)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-14 ~ 2004-06-14
/// \warning	Enchant, UTC ���� ����
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReqSP(int nParameterIndex)
{
	if (m_bEnableItem || g_pShuttleChild->m_myShuttleInfo.SP >= m_pRefITEM->ReqSP)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_SKILL_0007, m_pRefITEM->ReqSP);//"SP(��ų����Ʈ) : %d"
	}
	else
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_SKILL_0008, m_pRefITEM->ReqSP);//"SP(��ų����Ʈ) : \\r%d\\r"
	}

}

void CINFItemInfo::SetBullet(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetBullet( int nParameterIndex, int nCount )");
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0046, m_pRefItemInfo ? m_pRefItemInfo->CurrentCount : 0); //"���� : %d"
}

void CINFItemInfo::SetFUEL(int nParameterIndex)
{
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0047, m_pRefItemInfo ? m_pRefItemInfo->CurrentCount : 0, m_pRefITEM->Charging); //"���� : %d / %d"
}

void CINFItemInfo::SetCount(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetCount( int nParameterIndex, int nCount )");
	//	wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0048, m_pRefItemInfo ? m_pRefItemInfo->CurrentCount : 0 ); //"���� : %d "
		// 2006-03-14 by ispark, ������ ������ ������ ������ ������ ī��� �Ѵ�.
	wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0048, m_pRefItemInfo ? m_pRefItemInfo->CurrentCount : m_nOtherItemCount); //"���� : %d "
}

void CINFItemInfo::SetSpeed(int nParameterIndex)
{
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0049, m_pRefITEM->AbilityMin, m_pRefITEM->AbilityMax );//"�ӵ� : %.0fm/s ~ %.0fm/s"
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_080929_0209);//

	// �ּ� �ӵ�
	char chTmp[ITEMINFO_ITEM_FULL_NAME];
	memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
	{
		sprintf(chTmp, STRMSG_C_080929_0210, m_pRefITEM->AbilityMin);
		sprintf(m_strItemInfo[nParameterIndex], "%s%s", m_strItemInfo[nParameterIndex], chTmp);//
		float fEnchant = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_MIN_SPEED_UP : 0;
		if (fEnchant > 0)
		{
			sprintf(chTmp, STRMSG_C_080929_0210, fEnchant);
			sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//
		}
		strcat(m_strItemInfo[nParameterIndex], " ~ ");//
	}
	// �ִ� �ӵ�
	{
		sprintf(chTmp, STRMSG_C_080929_0210, m_pRefITEM->AbilityMax);
		sprintf(m_strItemInfo[nParameterIndex], "%s%s", m_strItemInfo[nParameterIndex], chTmp);//
		float fEnchant = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_MAX_SPEED_UP : 0;
		if (fEnchant > 0)
		{
			sprintf(chTmp, STRMSG_C_080929_0210, fEnchant);
			sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//
		}
	}



}
void CINFItemInfo::SetBoosterSpeed(int nParameterIndex)
{
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0050, m_pRefITEM->Range );//"�ν��ͼӵ� : %dm/s"
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0050, m_pRefITEM->Range);//"�ν��ͼӵ� : %dm/s"
	float fEnchant = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_BOOSTER_SPEED_UP : 0;
	if (fEnchant > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);

		sprintf(chTmp, STRMSG_C_080929_0210, fEnchant);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//
	}

}
void CINFItemInfo::SetCharging(int nParameterIndex)
{
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	//sprintf(m_strItemInfo[nParameterIndex],STRMSG_C_061018_0101,m_pRefITEM->Charging);//"����ӵ� : %dm/s"
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061018_0101, m_pRefITEM->Charging);//"����ӵ� : %dm/s"
	float fEnchant = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_GROUND_SPEED_UP : 0;
	if (fEnchant > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);

		sprintf(chTmp, STRMSG_C_080929_0210, fEnchant);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//
	}
}
void CINFItemInfo::SetBoosterTime(int nParameterIndex)
{
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0051, m_pRefITEM->Time/1000 );//"�ν��ͽð� : %d��"
	float fEnchantHitRate = m_pRefEnchant ? m_pRefEnchant->pfn_ENGINE_BOOSTER_TIME_UP : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0051, m_pRefITEM->Time / 1000);//"�ν��ͽð� : %d��"
	if (fEnchantHitRate > 0)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_080929_0208, m_strItemInfo[nParameterIndex], fEnchantHitRate);
	}
}
void CINFItemInfo::SetRotateAngle(int nParameterIndex)
{
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��	
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0052, (m_pRefITEM->RangeAngle/PI)*180.0f );//"�Ϲ�ȸ���� : %.0f��"		
	float fEnEnchantAngle = m_pRefEnchant ? (m_pRefEnchant->pfm_ENGINE_ANGLE_UP) : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0052, (m_pRefITEM->RangeAngle / PI)*180.0f);//"�Ϲ�ȸ���� : %.0f��"
	if (fEnEnchantAngle > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080929_0213, (fEnEnchantAngle / PI)*180.0f);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//": %.0f��"

	}
}
void CINFItemInfo::SetBoosterRotateAngle(int nParameterIndex)
{
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��	
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0053, (m_pRefITEM->BoosterAngle/PI)*180.0f );//"�ν���ȸ���� : %.0f��"
	float fEnEnchantAngle = m_pRefEnchant ? (m_pRefEnchant->pfm_ENGINE_BOOSTERANGLE_UP) : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0053, (m_pRefITEM->BoosterAngle / PI)*180.0f);//"�ν���ȸ���� : %.0f��"

	if (fEnEnchantAngle > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080929_0213, (fEnEnchantAngle / PI)*180.0f);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//": %.0f��"

	}
}
// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
// 2010. 02. 11 by ckPark �ߵ��� ����������
//void CINFItemInfo::SetFunction(int nParameterIndex, BYTE bType1, float fValue1, BYTE bType2, float fValue2)
//void CINFItemInfo::SetFunction( int nParameterIndex,
//								BYTE bType1,
//								float fValue1,
//								BYTE bType2,
//								float fValue2,
//								FUNCTION_TYPE nFunctionType /* = FUNCTIONTYPE_NORMAL */ )
//// end 2010. 02. 11 by ckPark �ߵ��� ����������
//// 2010. 02. 11 by ckPark �ߵ��� ����������
void CINFItemInfo::SetFunction(int nParameterIndex,
	DestParam_t bType1, // 2011-08-02 by jhahn ��Ʈ�� ������ �ý��� Byte����  USHORT�� �����Ķ����� ����
	float fValue1,
	//							   BYTE bType2,
	DestParam_t bType2, // 2013-03-27 by bhsohn DestParam �ڷ��� ����		
	float fValue2,
	FUNCTION_TYPE nFunctionType /* = FUNCTIONTYPE_NORMAL */,
	float fRareValue,
	BOOL bMonthlyArmor /*FALSE*/) // 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
// end 2010. 02. 11 by ckPark �ߵ��� ����������
//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
{
	FLOG("CINFItemInfo::SetFunction(int nParameterIndex, BYTE bType1, float fValue1, BYTE bType2, float fValue2)");
	char buf1[128], buf2[128];
	memset(buf1, 0x00, sizeof(buf1));
	memset(buf2, 0x00, sizeof(buf2));


	// 2010. 02. 11 by ckPark �ߵ��� ����������
/*
//	if(fValue1 != 0) {
	if(bType1 != 0) {
		SetParameterInfo( buf1, bType1, fValue1); }
//	if(fValue2 != 0) {
	if(bType2 != 0) {
		SetParameterInfo( buf2, bType2, fValue2); }

	if(strlen(buf1)>0 && strlen(buf2)>0){
		wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0054, buf1, buf2);}//"��� : \\g%s/%s"
	else if(strlen(buf1)>0){
		wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0055, buf1);}//"��� : \\g%s"
	else if(strlen(buf2)>0){
		wsprintf( m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0055, buf2);}
*/

	BOOL bDefEnchant;
	if (nFunctionType == FUNCTIONTYPE_NORMAL)
		bDefEnchant = TRUE;
	else	//	FUNCTIONTYPE_EQUIP or FUNCTIONTYPE_USE
		bDefEnchant = FALSE;

	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	if (fRareValue > 0)
	{
		fValue1 += fRareValue;
	}
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����

	if (bType1 != 0)
		SetParameterInfo(buf1, bType1, fValue1, bDefEnchant, bMonthlyArmor);	// 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
	if (bType2 != 0)
		SetParameterInfo(buf2, bType2, fValue2, bDefEnchant);

	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
//  	if(fRareValue > 0)
//  	{
// 		if(bType1 != 0)
// 				SetRareParameterInfo(buf1, fRareValue);
// 		if(bType2 != 0)
// 				SetRareParameterInfo(buf2, fRareValue);
// 	}
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����



	if (m_pRefITEM
		&& m_pRefITEM->Kind == ITEMKIND_ENCHANT)
	{
		nFunctionType = FUNCTIONTYPE_NORMAL;
	}

	switch (nFunctionType)
	{
	case FUNCTIONTYPE_NORMAL:
	{
		if (strlen(buf1) > 0 && strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0054, buf1, buf2);	//"��� : \\g%s/%s"
		else if (strlen(buf1) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0055, buf1);		//"��� : \\g%s"
		else if (strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0055, buf2);		//"��� : \\g%s"
	}
	break;

	case FUNCTIONTYPE_EQUIP:
	{
		if (strlen(buf1) > 0 && strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0309, buf1, buf2);//"������ : \\g%s/%s"
		else if (strlen(buf1) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0310, buf1);		//"������ : \\g%s"
		else if (strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0310, buf2);		//"������ : \\g%s"
	}
	break;

	case FUNCTIONTYPE_USE:
	{
		if (strlen(buf1) > 0 && strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0311, buf1, buf2);//"����� : \\g%s/%s"
		else if (strlen(buf1) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0312, buf1);		//"����� : \\g%s"
		else if (strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100218_0312, buf2);		//"����� : \\g%s"
	}
	break;
	// 2013-04-19 by ssjung ���� ���� Ŷ ��ȭ �ý���
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	case FUNCTIONTYPE_LOOKCHANGE:
	{
		//			if(fValue1 > 0)
		if (strlen(buf1) > 0)
		{
			//  				memset(buf1, 0x00, sizeof(buf1));
			//  				sprintf(buf1,STRMSG_C_130419_0005, (int)fValue1);								//"\\y[��罺��:+%d]\\y"
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_130419_0004, buf1);			//"������� : %s"
		}
	}
	break;
	//2013-05-20 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ǥ��
	case FUNCTIONTYPE_TIMELIMINT_ATTACK:
	{
		if (fValue2 > 0)
		{
			int nTempTimeSec = (int)fValue2;
			int m_nDay = (nTempTimeSec) / 86400;
			int m_nHour = (nTempTimeSec) % 86400 / 3600;
			int m_nMin = (nTempTimeSec) % 86400 % 3600 / 60;
			int m_nSec = (nTempTimeSec) % 86400 % 3600 % 60 / 1;

			memset(buf2, 0x00, sizeof(buf2));

			if (m_nDay)
			{
				wsprintf(buf2, STRMSG_C_121013_0101, buf2, m_nDay, m_nHour, m_nMin);
			}
			else if ((m_nDay == NULL) && m_nHour)
			{
				wsprintf(buf2, STRMSG_C_121013_0102, buf2, m_nHour, m_nMin);
			}
			else if ((m_nDay == NULL) && (m_nHour == NULL) && m_nMin)
			{
				wsprintf(buf2, STRMSG_C_121013_0103, buf2, m_nMin);
			}
			else if ((m_nDay == NULL) && (m_nHour == NULL) && (m_nMin == NULL) && m_nSec)
			{
				wsprintf(buf2, STRMSG_C_121013_0104, buf2);
			}
		}
		if (strlen(buf1) > 0 && strlen(buf2) > 0)
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_130516_0001, buf1, buf2);		//"�Ⱓ�� ��� : \\m%s\\m %s"	
	}
	break;
	//2013-05-20 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ǥ��
#endif
// end 2013-04-19 by ssjung ���� ���� Ŷ ��ȭ �ý���

	}
	// end 2010. 02. 11 by ckPark �ߵ��� ����������
}

// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
// ��� ���� ���� ����
void CINFItemInfo::SetUseAreaInfinity(int* p_nParameterIndex)
{
	if (m_pRefITEM
		&& COMPARE_BIT_FLAG(m_pRefITEM->ItemAttribute, ITEM_ATTR_ONLY_USE_INFINITY))
	{
		sprintf(m_strItemInfo[(*p_nParameterIndex)++], STRMSG_C_091103_0325); // "��� �������� : ���Ǵ�Ƽ �ʵ�"
	}
}

void CINFItemInfo::SetExchangeMaterial(BOOL bShop /* = FALSE */)
{
	m_vecExchageMtrl.clear();

	if (bShop)
	{
		if (m_pRefITEM)
		{
			CMapCityShopList::iterator it = g_pInterface->m_pCityBase->m_mapCityShop.find(BUILDINGKIND_INFINITY_SHOP);
			if (it != g_pInterface->m_pCityBase->m_mapCityShop.end())
			{
				INFINITY_SHOP_INFO info;
				memset(&info, 0, sizeof(INFINITY_SHOP_INFO));

				if (((CINFCityInfinityShop*)(it->second))->FindInfinityShopInfo_From_CurrentTab(m_pRefITEM->ItemNum, &info))
				{
					for (int i = 0; i < MAX_ORB_KIND; ++i)
					{
						ItemNum_t				nItemNum;
						InfinityShopItemCnt_t	nItemCnt;

						((CINFCityInfinityShop*)(it->second))->GetOrbInfo(&info, i, &nItemNum, &nItemCnt);

						if (nItemNum)
							m_vecExchageMtrl.push_back(std::pair<ItemNum_t, InfinityShopItemCnt_t>(nItemNum, nItemCnt));
					}
				}
			}
		}
	}
}
// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

void CINFItemInfo::SetDesc(int nParameterIndex)
{
	FLOG("CINFItemInfo::SetDesc( int nParameterIndex, char* strDesc )");
	m_nDescIndex = nParameterIndex;
	memset(m_strDesc, 0x00, ITEMINFO_DESC_SIZE*ITEMINFO_DESC_LINE_NUMBER);
	//	if(strlen(strDesc) == 0)
	if (strlen(m_pRefITEM->Description) == 0)
	{
		//		DBGOUT("Item discription is NULL.\n");
		return;
	}
	int i = 0;
	int nPoint = 0;
	int nCheckPoint = 0;
	int nBreakPoint = 0;
	int nLine = 0;
	char strBuf[SIZE_MAX_ITEM_DESCRIPTION + 15];
	memset(strBuf, 0x00, SIZE_MAX_ITEM_DESCRIPTION + 15);
	strcpy(strBuf, STRMSG_C_ITEM_0056);//"���� : "
//	strcat( strBuf, strDesc );
	strcat(strBuf, m_pRefITEM->Description);
	if (COMPARE_BIT_FLAG(m_pRefITEM->ItemAttribute, ITEM_ATTR_CASH_ITEM))
	{
		for (int i = 0; i < strlen(strBuf); i++)
		{
			if (strBuf[i] == '$')
				strBuf[i] = ' ';
		}
	}
	//	while(TRUE)
	//	{
	//		if(strBuf[i] == ' ' || strBuf[i] == '.' || strBuf[i] == '!' || strBuf[i] == NULL)
	//		{
	//			if(nPoint >= ITEMINFO_DESC_SIZE-1)
	//			{
	//				if(nLine >= ITEMINFO_DESC_LINE_NUMBER)
	//				{
	//					DBGOUT("CINFItemInfo::SetDesc(),1 Item Discription is Too Long to Draw .\n");
	//					return;
	//				}
	//				memcpy(m_strDesc[nLine], strBuf + nCheckPoint, nBreakPoint+1);
	//				nPoint -= nBreakPoint;
	//				nCheckPoint += nBreakPoint+1;
	//				nBreakPoint = nPoint-1;
	//				nLine ++;
	//				i++;
	//				continue;
	//			}
	//			if(strBuf[i] == NULL)
	//			{
	//				if(nLine >= ITEMINFO_DESC_LINE_NUMBER)
	//				{
	//					DBGOUT("CINFItemInfo::SetDesc(),2 Item Discription is too Long to Draw .\n");
	//					return;
	//				}
	//				memcpy(m_strDesc[nLine], strBuf + nCheckPoint, nPoint);
	//				break;
	//			}
	//			nBreakPoint = nPoint;
	//		}
	//		i++;
	//		nPoint++;
	//	}
	vector<string> vectemp;
	// 2008-04-14 by bhsohn ���� ������ ���� ��Ʈ������ ó��
	//::StringCullingUserData(strBuf, 40, &vectemp);	
	vector<string> vectempCulling;
	::StringCullingUserData(strBuf, 40, &vectempCulling);
	int nItemStringLen = 0;
	if (vectempCulling.size() > 0)
	{
		nItemStringLen = m_pFontDescInfo[0]->GetStringSize((char*)vectempCulling[0].c_str()).cx;
	}
	else
	{
		nItemStringLen = GetItemStringLen();
		if (nItemStringLen < 40)
		{
			nItemStringLen = 40;
		}
	}

	STRING_CULL(strBuf, nItemStringLen, &vectemp, m_pFontDescInfo[0]);

	m_nDescLine = vectemp.size();
	for (i = 0; i < vectemp.size(); i++)
	{
		//if(i>ITEMINFO_DESC_LINE_NUMBER) return;
		if (i >= ITEMINFO_DESC_LINE_NUMBER)
		{
			break;
		}
		strcpy(m_strDesc[i], vectemp[i].c_str());
	}
	// end 2008-04-14 by bhsohn ���� ������ ���� ��Ʈ������ ó��
}

void CINFItemInfo::SetShopSellInfo(int nParameterIndex)
{
	if (m_pRefITEM->MinTradeQuantity > 1)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0057, //"�������� : %d(����) / %d����"
			CAtumSJ::GetItemSellingPriceAtShop(m_pRefITEM), m_pRefITEM->MinTradeQuantity);
	}
	else
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_ITEM_0058, //"�������� : %d(����)"
			CAtumSJ::GetItemSellingPriceAtShop(m_pRefITEM));
	}
}


// 2010-06-15 by shcho&hslee ��ý��� - �� Ÿ�� ���� �߰�.
void CINFItemInfo::SetPetType(int nParameterIndex)
{
	tPET_BASE_ALL_DATA *psPetBaseAllData = g_pDatabase->GetPtr_PetAllDataByIndex(m_pRefITEM->LinkItem);


	if (NULL == psPetBaseAllData || NULL == psPetBaseAllData->rtn_LevelData(m_pRefITEM->SkillLevel))
		return;

	char szTemp[256] = { 0, };
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	if (m_pRefItemInfo)
	{
		tPET_CURRENTINFO *pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData(m_pRefItemInfo->UniqueNumber);   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���	
		// 2011. 1. 13 by jskim UI ��Ʈ�� ���� ���� ����
		//tPET_LEVEL_DATA *psPetLevelData = psPetBaseAllData->rtn_LevelData( pPetCurrentInfo->PetLevel );					  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		//sprintf ( szTemp, STRMSG_C_100812_0401 , pPetCurrentInfo->PetLevel );//"Lv.%d"				 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		if (pPetCurrentInfo)
		{
			tPET_LEVEL_DATA *psPetLevelData = psPetBaseAllData->rtn_LevelData(pPetCurrentInfo->PetLevel);					  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			sprintf(szTemp, STRMSG_C_100812_0401, pPetCurrentInfo->PetLevel);//"Lv.%d"				 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		}
		// end 2011. 1. 13 by jskim UI ��Ʈ�� ���� ���� ����
	}
	else
		sprintf(szTemp, STRMSG_C_100812_0401, 1);//"Lv.%d"				 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0402, g_pInterface->GetString_PetType(psPetBaseAllData->BaseData.PetKind), szTemp); //"Ÿ�� : %s[%s]"

}

void CINFItemInfo::SetPetReName(int nParameterIndex)
{
	char szTemp[256] = { 0, };
	char buf[256] = { 0, };
	if (m_pRefItemInfo->GetReName())
	{
		strcpy(buf, STRMSG_C_100812_0404);		//"����"	
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0403, buf);  //"�̸� ���� : %s"
	}
	else
	{
		strcpy(buf, STRMSG_C_100812_0405);		//"�Ұ���"
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0403, buf);  //"�̸� ���� : %s"
	}
}
void CINFItemInfo::SetPetEnableLevelUp(int nParameterIndex, BOOL bShop /* = FALSE */)
{
	char szTemp[256] = { 0, };
	char buf[256] = { 0, };
	BOOL bTemp;

	if (bShop)
	{
		tPET_BASE_ALL_DATA *psPetBaseAllData = g_pDatabase->GetPtr_PetAllDataByIndex(m_pRefITEM->LinkItem);

		if (psPetBaseAllData->BaseData.EnableLevel)
		{
			bTemp = TRUE;
		}
		else
		{
			bTemp = FALSE;
		}
	}
	else
	{
		tPET_BASE_ALL_DATA *psPetBaseAllData = g_pDatabase->GetPtr_PetAllDataByIndex(m_pRefITEM->LinkItem);

		if (m_pRefItemInfo && m_pRefItemInfo->GetPetEnableLevelUp())
		{
			bTemp = TRUE;
		}
		else if (psPetBaseAllData->BaseData.EnableLevel)
		{
			bTemp = TRUE;
		}
		else
		{
			bTemp = FALSE;
		}
	}

	if (bTemp)
	{
		strcpy(buf, STRMSG_C_100812_0404);		//"����"
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0407, buf); //"���忩�� : %s"
	}
	else
	{
		strcpy(buf, STRMSG_C_100812_0405);		//"�Ұ���"
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0407, buf); //"���忩�� : %s"
	}
}

void CINFItemInfo::SetPetExp(int nParameterIndex)
{
	tPET_LEVEL_DATA* tempPetData = g_pDatabase->GetPtr_PetLevelData(m_pRefITEM->LinkItem, m_pRefITEM->SkillLevel);
	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	double Temp = 0.0f;

	tPET_BASE_ALL_DATA *psPetBaseAllData = g_pDatabase->GetPtr_PetAllDataByIndex(m_pRefITEM->LinkItem);
	tPET_CURRENTINFO *pPetCurrentInfo = g_pShuttleChild->GetPetManager()->GetPtr_PetCurrentData(m_pRefItemInfo->UniqueNumber);
	// 2011. 1. 13 by jskim UI ��Ʈ�� ���� ���� ����	
	//if (tempPetData->NeedExp > .0f )	
	if (pPetCurrentInfo && tempPetData != nullptr && tempPetData->NeedExp > .0f) // 2016-01-10 "tempPetData != nullptr" to deal with exception
	// end 2011. 1. 13 by jskim UI ��Ʈ�� ���� ���� ����
	{
		int templevel;
		if (pPetCurrentInfo->PetLevel - 1 <= 0)
			templevel = 1;
		else
			templevel = pPetCurrentInfo->PetLevel - 1;

		tPET_LEVEL_DATA *psPetLevelDataPrev = g_pDatabase->GetPtr_PetLevelData(pPetCurrentInfo->PetIndex, pPetCurrentInfo->PetLevel - 1);
		tPET_LEVEL_DATA *psPetLevelDataNext = g_pDatabase->GetPtr_PetLevelData(pPetCurrentInfo->PetIndex, pPetCurrentInfo->PetLevel);

		float	ExpNow, ExpMax;

		if (psPetLevelDataPrev)
		{
			ExpNow = (float)pPetCurrentInfo->PetExp - (float)psPetLevelDataPrev->NeedExp;
			ExpMax = (float)psPetLevelDataNext->NeedExp - (float)psPetLevelDataPrev->NeedExp;
		}
		else
		{
			ExpNow = (float)pPetCurrentInfo->PetExp;
			ExpMax = (float)psPetLevelDataNext->NeedExp;
		}
		Temp = ExpNow / ExpMax * 100;
	}
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	char szTemp[256] = { 0, };
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_100812_0406, Temp);  //"EXP : %.2f%%" 
}
// End 2010-06-15 by shcho&hslee ��ý��� - �� Ÿ�� ���� �߰�.


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetPrimaryRange(int nParameterIndex)
/// \brief		�⺻���� ���� �Ÿ�
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetPrimaryRange(int nParameterIndex)
{
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
//	sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_050817_0001, 
//			CAtumSJ::GetPrimaryRadarRange(m_pRefITEM, &g_pShuttleChild->m_paramFactor));	

	float fRangeMin = m_pRefITEM ? m_pRefITEM->AbilityMin : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_050817_0001, fRangeMin);

	if (m_pRefEnchant && m_pRefEnchant->pfm_ATTACK_RANGE_01 > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080923_0200, m_pRefEnchant->pfm_ATTACK_RANGE_01*100.0f);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);
	}

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetSecondaryRange(int nParameterIndex)
/// \brief		��޹��� ���ذŸ�
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetSecondaryRange(int nParameterIndex)
{
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
//	sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_050817_0002, 
//			CAtumSJ::GetSecondaryRadarRange(m_pRefITEM, &g_pShuttleChild->m_paramFactor));
	float fRangeMax = m_pRefITEM ? m_pRefITEM->AbilityMax : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_050817_0002, fRangeMax);

	if (m_pRefEnchant && m_pRefEnchant->pfm_ATTACK_RANGE_02 > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080923_0200, m_pRefEnchant->pfm_ATTACK_RANGE_02*100.0f);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetRadarRange(int nParameterIndex)
/// \brief		���̴� ��ü ���� �ݰ�
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetRadarRange(int nParameterIndex)
{
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��	
	//sprintf( m_strItemInfo[nParameterIndex], STRMSG_C_050817_0003, m_pRefITEM->Range);	
	float fEnRadarRange = m_pRefEnchant ? (m_pRefEnchant->pfn_RADAR_OBJECT_DETECT_RANGE) : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_050817_0003, m_pRefITEM->Range);
	if (fEnRadarRange > 0)
	{
		char chTmp[ITEMINFO_ITEM_FULL_NAME];
		memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);
		sprintf(chTmp, STRMSG_C_080929_0212, fEnRadarRange);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//	
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetEnginTurningAngle(int nParameterIndex)
/// \brief		���� ��ȸ ���ӷ�
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetEnginTurningAngle(int nParameterIndex)
{
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_050817_0004, m_pRefITEM->SpeedPenalty);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetReUseTime(int nParameterIndex)
/// \brief		������ ��ߵ� �ð�
/// \author		ispark
/// \date		2005-12-07 ~ 2005-12-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetReUseTime(int nParameterIndex)
{
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051207_0002, (float)m_pRefITEM->ReAttacktime / 1000);
}

void CINFItemInfo::SetPrimaryWeaponInfo(BOOL bShop, BOOL bInven)							  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop);	// 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetAttack(index++);
	SetSecAttack(index++);
	SetRate(index++);
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	SetPrimaryPierce(index++);	// �Ǿ ��

	SetRange(index++);
	SetReAttackTime(index++);
	if (m_pRefITEM->AttackTime > 0)
	{
		SetAttackTime(index++);
	}
	SetOverHeatTime(index++);
	SetWeight(index++);

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	if (m_pRefItemInfo)
	{
		SetShapeInfo(index++);
		SetEffectInfo(index++);
	}
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);													// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetSecondaryWeaponInfo(BOOL bShop, BOOL bInven)				  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetAttack(index++);
	SetSecAttack(index++);
	SetRate(index++);
	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	SetSecondaryPierce(index++);	// �Ǿ ��
	SetRange(index++);
	SetReAttackTime(index++);
	SetRangeAngle(index++);
	SetWeaponSpeed(index++);
	SetWeaponAngle(index++);
	if (m_pRefITEM->Kind == ITEMKIND_ROCKET ||
		m_pRefITEM->Kind == ITEMKIND_BUNDLE ||
		m_pRefITEM->Kind == ITEMKIND_MINE ||
		m_pRefITEM->Kind == ITEMKIND_MISSILE)// 2007-02-05 by bhsohn �̻��� ������ ���� �ȳ����� �κ� ����
	{
		SetExplosionRange(index++);
	}
	if (m_pRefITEM->Kind == ITEMKIND_MINE)
	{
		SetReactionRange(index++);
	}
	SetWeight(index++);

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	if (m_pRefItemInfo)
	{
		SetShapeInfo(index++);
		SetEffectInfo(index++);
	}
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																		 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetSkillItemInfo(BOOL bShop, BOOL bInven)									  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetSkillLevel(index++);
	SetReqLevel(index++);
	if (bShop == TRUE && m_pRefITEM->LinkItem != 0)
	{
		SetShopReqItem(index++);
	}
	if (m_pRefITEM->ReqItemKind != ITEMKIND_ALL_ITEM)
	{
		SetReqItemKind(index++);
	}
	if (m_pRefITEM->ReqSP > 0)
	{
		SetReqSP(index++);
	}
	if (m_pRefITEM->SkillType == SKILLTYPE_CLICK ||
		m_pRefITEM->SkillType == SKILLTYPE_TIMELIMIT)
	{
		SetSkillTime(index++);
		SetSkillReAttackTime(index++);
	}

	SetParameter(&index);

	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		// 2007-10-16 by bhsohn ��ų�������� �ŷ����� ǥ�� �ȵǰ� ����
		//SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetDefenseItemInfo(BOOL bShop, BOOL bInven)										  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetWeight(index++);

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
#ifdef _REWORKED_COLORSHOP
	if (m_pRefItemInfo != 0)
		SetDefenseColorInfo(index++);
#endif

	if (m_pRefItemInfo)
		SetShapeInfo(index++);
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	//SetParameter(&index);
	SetParameter(&index, FALSE, TRUE);

	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
	}
	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

//	SetItemLimit(&index);		// 2012-10-13 by jhahn �Ⱓ�� ������ �߰�
	SetDesc(index);
	SetItemExtendInfo(bInven, TRUE);																				// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetSupportItemInfo(BOOL bShop, BOOL bInven)								  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetReqStat(index++);
	SetSpeed(index++);
	SetBoosterSpeed(index++);
	// 2006-10-18 by dgwoo A��� �������� ���� �ӵ��� �����ش�.
	if (IS_DT(m_pRefITEM->ReqUnitKind))
		SetCharging(index++);
	SetBoosterTime(index++);
	SetRotateAngle(index++);
	SetBoosterRotateAngle(index++);
	SetEnginTurningAngle(index++);							// 2005-08-17 by ispark
	SetWeight(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																		  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();

}

void CINFItemInfo::SetEnergyItemInfo(BOOL bShop, BOOL bInven)								  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetUnitKind(index++);
	if (COMPARE_BIT_FLAG(m_pRefITEM->ItemAttribute, ITEM_ATTR_CASH_ITEM))
	{
		SetReqLevel(index++);
	}
	SetCount(index++);

	SetParameter(&index);

	//	if( IS_COUNTABLE_ITEM(m_pRefITEM->Kind) )
	//	{
//		SetCountableWeight( index++ );
//	}
//	else
//	{
//		SetWeight( index++ );
//	}
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																			   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();

}

void CINFItemInfo::SetIngotItemInfo(BOOL bShop, BOOL bInven)							// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	//	SetUnitKind( index++ );
	SetCount(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	// 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
// 	//2013-05-20 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ǥ��
// #ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
// 	if(m_pRefItemInfo)
// 	{
// 		SetArmorCollectionParameter(&index);
// 		if(m_pRefItemInfo->nMonthlyOptionItemNum)
// 			SetMonthlyArmorValue(&index, m_pRefItemInfo->atMonthlyEventEndDate,m_pRefItemInfo->nMonthlyOptionItemNum);
// // 		if(m_pRefItemInfo->ItemInfo->SkillLevel)
// // 		{
// // 			SetLookChangeSystem(&index,m_pRefItemInfo->ItemInfo->SkillLevel);
// // 		}
// 	}
// #endif
// 	//end 2013-05-20 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ǥ��

	SetWeight(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																		   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetCardItemInfo(BOOL bShop, BOOL bInven)							// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	// 2006-04-24 by ispark, ����
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);

	// 2010. 02. 10 by ckPark �����ڽ������ۿ� ���� ���� �߰�, ų��ũ ������������ ����
	//SetReqLevel( index++ );
	if (m_pRefITEM)
	{
		if (!COMPARE_BIT_FLAG(m_pRefITEM->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM))
			SetReqLevel(index++);
		else
		{
			if (g_pShuttleChild->m_myShuttleInfo.Level >= m_pRefITEM->ReqMinLevel)
				wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0022, m_pRefITEM->ReqMinLevel);// "�䱸 : LEVEL[%d]"
			else
				wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0025, m_pRefITEM->ReqMinLevel); // "�䱸 : \\rLEVEL[%d]\\r"
		}
	}
	// end 2010. 02. 10 by ckPark �����ڽ������ۿ� ���� ���� �߰�, ų��ũ ������������ ����

	SetCount(index++);
	// 2009. 09. 24 by jskim �ʵ� â�� ĳ�� ������
	//SetItemAllTime(index++, TRUE);
	// 2008-12-02 by bhsohn �ű� ������ �Ӽ� �߰�
		//if(m_pRefItemInfo 
		//&& m_pRefItemInfo->ItemInfo
		//&& COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
		//{
		//	SetItemRemainTime(index++,bShop);		
		//}	
	// end 2008-12-02 by bhsohn �ű� ������ �Ӽ� �߰�
	// SetParameter(&index);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
// 	if( m_pRefItemInfo && m_pRefItemInfo->ItemInfo && !m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE ) )
// 	{
// 		SetItemAllTime(index++, TRUE);
// 	}
// 
//	if(m_pRefItemInfo && m_pRefItemInfo->ItemInfo)
// 	{
// 		if(COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
// 		{
// 			SetItemRemainTime(index++,bShop);		
// 		}
// 		else if(COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
// 		{
// 			SetItemDelRemainTime(index++);
// 		}
// 	}

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	{
		// 2009. 12. 16 by jskim â�� Ȯ��, �κ� Ȯ���� ���� ��� ���ڿ� ����
		//if( m_pRefItemInfo && m_pRefItemInfo->ItemInfo && !m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE ) 
// 		if( m_pRefItemInfo && m_pRefItemInfo->ItemInfo && 
// 			!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE ) &&
// 			!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_INCREASE_INVENTORY_SPACE ) &&
// 			!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_INCREASE_STORE_SPACE ))
		if (m_pRefITEM &&
			!m_pRefITEM->IsExistDesParam(DES_FIELD_STORE) &&
			!m_pRefITEM->IsExistDesParam(DES_INCREASE_INVENTORY_SPACE) &&
			!m_pRefITEM->IsExistDesParam(DES_INCREASE_STORE_SPACE))
			SetItemAllTime(index++, TRUE);
		// end 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
		//end 2009. 12. 16 by jskim â�� Ȯ��, �κ� Ȯ���� ���� ��� ���ڿ� ����
	}

	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����


	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

// 	// 2009. 12. 16 by jskim â�� Ȯ��, �κ� Ȯ���� ���� ��� ���ڿ� ����
// 	//if( m_pRefItemInfo && m_pRefItemInfo->ItemInfo && !m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE ) )
// 	if( m_pRefItemInfo && 
// 		m_pRefItemInfo->ItemInfo && 
// 		!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_FIELD_STORE &&
// 		!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_INCREASE_INVENTORY_SPACE ) &&
// 		!m_pRefItemInfo->ItemInfo->IsExistDesParam( DES_INCREASE_STORE_SPACE )) )
// 	//end 2009. 12. 16 by jskim â�� Ȯ��, �κ� Ȯ���� ���� ��� ���ڿ� ����

	if (m_pRefITEM
		&& !m_pRefITEM->IsExistDesParam(DES_FIELD_STORE)
		&& !m_pRefITEM->IsExistDesParam(DES_INCREASE_INVENTORY_SPACE)
		&& !m_pRefITEM->IsExistDesParam(DES_INCREASE_STORE_SPACE))
		// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	{
		SetParameter(&index);
	}
	//end 2009. 09. 24 by jskim �ʵ� â�� ĳ�� ������

// 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	if (m_pRefItemInfo)
	{
		SetArmorCollectionParameter(&index);
		if (m_pRefItemInfo->nMonthlyOptionItemNum)
			SetMonthlyArmorValue(&index, m_pRefItemInfo->atMonthlyEventEndDate, m_pRefItemInfo->nMonthlyOptionItemNum);
		// 		if(m_pRefItemInfo->ItemInfo->SkillLevel)
		// 		{
		// 			SetLookChangeSystem(&index,m_pRefItemInfo->ItemInfo->SkillLevel);
		// 		}
	}
#endif
	// end 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��

	//	SetWeight( index++ );
	//	if(bShop)
	//	{
	//		SetShopSellInfo( index++ );
	//	}
	// 2006-08-18 by ispark, ���� ���� �߰�
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::PrevetionDeleteItem(BOOL bShop, BOOL bInven)							 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	SetCount(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	//	SetWeight( index++ );
	//	SetParameter(&index);


	// 2009. 06. 16 by ckpark ��æƮȮ������ī��, �Ĺ�ī�� ���� ���� ����

	// 	// 2009. 01. 21 by ckPark ��æƮ Ȯ�� ���� ī��
	// 	//wsprintf( m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_060424_0020); //"��þƮ ���н� ������ �ı� ����"
	// 
	// 	// DES_ENCHANT_INCREASE_PROBABILITY�� �����Ѵٸ� ��æƮ Ȯ�� ������ �ν� �ƴϸ� �Ĺ����� �ν�z
	// 	if(m_pRefItemInfo->ItemInfo->GetParameterValue(DES_ENCHANT_INCREASE_PROBABILITY) != 0.0f)
// 		wsprintf( m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_090123_0302); //"��æƮ Ȯ�� ����"
// 	else
// 		wsprintf( m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_060424_0020); //"��þƮ ���н� ������ �ı� ����"
// 
// 	// end 2009. 01. 21 by ckPark ��æƮ Ȯ�� ���� ī��

	if (m_pRefITEM)
	{
		if (m_pRefITEM->GetParameterValue(DES_ENCHANT_INCREASE_PROBABILITY) != 0.0f)
			wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_090123_0302); //"��æƮ Ȯ�� ����"
		else
			wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_060424_0020); //"��þƮ ���н� ������ �ı� ����"
	}

	// end 2009. 06. 16 by ckpark ��æƮȮ������ī��, �Ĺ�ī�� ���� ���� ����


	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetArmorColorInfo(BOOL bShop, BOOL bInven)												 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetUnitKind(index++);
	SetReqLevel(index++);
	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	SetMaxLength();
}

void CINFItemInfo::SetEnchantItemInfo(BOOL bShop, BOOL bInven)								// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	SetCount(index++);
	if (m_pRefITEM->ReqItemKind != ITEMKIND_ALL_ITEM)
	{
		SetReqItemKind(index++);
	}

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	//SetItemAllTime(index++, TRUE);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++, TRUE);
	}
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetParameter(&index);

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	SetInvokeDestParam(&index);
	// end 2010. 02. 11 by ckPark �ߵ��� ����������



	//	SetWeight( index++ );
	//	if(bShop == FALSE)
	//	{
	//		SetShopSellInfo( index++ );
	//	}
	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																			// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetAccessoryUnLimitItemInfo(BOOL bShop, BOOL bInven)					 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	SetReqLevel(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	//SetItemAllTime(index++);
	//SetItemRemainTime(index++,bShop);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++);
	}
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetParameter(&index);

	// 2010. 02. 11 by ckPark �ߵ��� ����������
	// �ߵ��� �����Ķ� ��� ����
	SetInvokeDestParam(&index);
	// ������ ��Ÿ�� ����
	SetItemCoolTime(&index);
	// end 2010. 02. 11 by ckPark �ߵ��� ����������

	SetWeight(index++);
	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();

}
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void CINFItemInfo::SetPetSoketItemInfo(BOOL bShop, BOOL bInven)							// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	//SetItemAllTime(index++);
	//SetItemRemainTime(index++,bShop);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++);
	}
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

//	SetParameter(&index);

	SetWeight(index++);
	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																			// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
void CINFItemInfo::SetBulletItemInfo(BOOL bShop, BOOL bInven)									  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetUnitKind(index++);
	SetBullet(index++);
	SetCountableWeight(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
	}
	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

void CINFItemInfo::SetComputerItemInfo(BOOL bShop, BOOL bInven)							  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetUnitKind(index++);
	SetReqLevel(index++);
	SetParameter(&index);
	SetWeight(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
// 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
void CINFItemInfo::SetArmorCollectionItemInfo(BOOL bShop)
{
	int index = 0;
	SetItemName(index++);

	CoatingCount(&index);						//����Ƚ��

	SetArmorCollectionParameter(&index);		//��� ����   

	SetMonthlyArmorCollectionValue(&index);		//�̴��� �Ƹ��� �� (SetMonthlyArmorValue(&index,);�� �Լ� �� �ҷ� ó�� 

	SetTimeLimt(&index);					//�����Ⱓ 

	SetDesc(index);
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}
// end 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
#endif
void CINFItemInfo::SetMaxLength()
{
	// 2006-03-08 by ispark, ��Ʈ ���� ��� �ٲ�
	FLOG("CINFItemInfo::SetMaxLength()");
	int len = 0;
	m_nMaxLength = 0;
	int i;
	for (i = 0; i < ITEMINFO_PARAMETER_NUMBER; i++)
	{
		len = m_pFontItemInfo[0]->GetStringSize(m_strItemInfo[i]).cx;
		//		len = strlen(m_strItemInfo[i]);
		if (m_nMaxLength < len) {
			m_nMaxLength = len;
		}
	}
	for (i = 0; i < ITEMINFO_DESC_LINE_NUMBER; i++)
	{
		len = m_pFontDescInfo[0]->GetStringSize(m_strDesc[i]).cx;
		//		len = strlen(m_strDesc[i]);

		if (i == 0 && len == 0)		// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
			m_nDescLine = 0;

		if (m_nMaxLength < len) {
			m_nMaxLength = len;
		}
	}

	// 2009-02-03 by bhsohn ���� ������ �� ����	
	m_szTooltip.cx = m_nMaxLength + 12;

	int nIcongab = 0;
	if (m_pBigIcon)
	{
		nIcongab = ITEMINFO_BIGICON_GAB;
	}
	//	m_szTooltip.cy = 14*(m_nDescIndex+1)+14*(m_nDescLine+1)+20+nIcongab;
	m_szTooltip.cy = 14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + m_nExtendItemIndex) + 20 + nIcongab;		// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	// end 2009-02-03 by bhsohn ���� ������ �� ����

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	if (!m_vecExchageMtrl.empty())
		m_szTooltip.cy += 30;
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetItemInfoUser( CItemInfo* pItemInfo, int x, int y )
/// \brief		�������� ������ �����Ѵ�.
/// \author		dhkwon
/// \date		2004-04-20 ~ 2004-04-20
/// \warning	�����ΰ�� pItem�� NULL�̴�.
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
//void CINFItemInfo::SetItemInfoUser( CItemInfo* pItemInfo, int x, int y, int nLinkItem)
void CINFItemInfo::SetItemInfoUser(CItemInfo* pItemInfo, int x, int y, int nLinkItem/*=0*/, BOOL bInven /*FALSE*/)				 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	m_ptItemInfo.x = x;
	m_ptItemInfo.y = y;// + ITEMINFO_BIGICON_GAB;

	if (m_pBigIcon)
	{
		//		m_ptItemInfo.y = m_ptItemInfo.y + ITEMINFO_BIGICON_GAB;
		m_pBigIcon->InvalidateDeviceObjects();
		m_pBigIcon->DeleteDeviceObjects();
		util::del(m_pBigIcon);
	}

	if (pItemInfo)
	{

		// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

		//m_pBigIcon = FindBigIcon(pItemInfo->ItemInfo->SourceIndex );			// 2005-08-23 by ispark		
		if (!pItemInfo->ShapeItemNum)
		{
			// 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
//			m_pBigIcon = FindBigIcon(pItemInfo->ItemInfo->SourceIndex );
			if (!nLinkItem)
			{
				m_pBigIcon = FindBigIcon(pItemInfo->ItemInfo->SourceIndex);
			}
			else
			{
				m_pBigIcon = FindBigIcon(nLinkItem);
			}
			// END 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
		}
		else
		{
			ITEM* pShapeItem = g_pDatabase->GetServerItemInfo(pItemInfo->ShapeItemNum);
			if (pShapeItem)
				m_pBigIcon = FindBigIcon(pShapeItem->SourceIndex);
			else
				m_pBigIcon = FindBigIcon(pItemInfo->ItemInfo->SourceIndex);
		}
		// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

		m_pRefItemInfo = pItemInfo;
		m_pRefITEM = pItemInfo->GetItemInfo();
		m_pRefPrefixRareInfo = m_pRefItemInfo->GetPrefixRareInfo();
		m_pRefSuffixRareInfo = m_pRefItemInfo->GetSuffixRareInfo();
		m_pRefEnchant = m_pRefItemInfo->GetEnchantParamFactor();
		m_bEnableItem = IsEnableItem(m_pRefItemInfo->GetRealItemInfo());
		m_bShow = TRUE;
		m_vecTickFuntionIndex.clear();
		if (IS_PRIMARY_WEAPON(m_pRefITEM->Kind))
		{
			SetPrimaryWeaponInfo(FALSE, bInven);																   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (IS_SECONDARY_WEAPON(m_pRefITEM->Kind))
		{
			SetSecondaryWeaponInfo(FALSE, bInven);															 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (IS_SKILL_ITEM(m_pRefITEM->Kind))
		{
			SetSkillItemInfo(FALSE, bInven);																			// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_DEFENSE == m_pRefITEM->Kind)
		{
			SetDefenseItemInfo(FALSE, bInven);																	  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_SUPPORT == m_pRefITEM->Kind)
		{
			SetSupportItemInfo(FALSE, bInven);																		 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_ENERGY == m_pRefITEM->Kind)
		{
			SetEnergyItemInfo(FALSE, bInven);																	  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_INGOT == m_pRefITEM->Kind)
		{
			SetIngotItemInfo(FALSE, bInven);																		 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_CARD == m_pRefITEM->Kind)
		{
			SetCardItemInfo(FALSE, bInven);																			// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_ENCHANT == m_pRefITEM->Kind || ITEMKIND_GAMBLE == m_pRefITEM->Kind)
		{
			SetEnchantItemInfo(FALSE, bInven);																			// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_TANK == m_pRefITEM->Kind)
		{
			SetAccessoryUnLimitItemInfo(FALSE, bInven);																// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_BULLET == m_pRefITEM->Kind)
		{
			SetBulletItemInfo(FALSE, bInven);																				// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_QUEST == m_pRefITEM->Kind)
		{
			SetIngotItemInfo(FALSE, bInven);																			   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_RADAR == m_pRefITEM->Kind)
		{
			// 2005-08-17 by ispark
			SetRadarItemInfo(FALSE, bInven);																				 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
//			SetIngotItemInfo();
			//SetEnchantItemInfo();
		}
		else if (ITEMKIND_COMPUTER == m_pRefITEM->Kind)
		{
			SetComputerItemInfo(FALSE, bInven);																		   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
			//SetEnchantItemInfo();
		}
		else if (ITEMKIND_PREVENTION_DELETE_ITEM == m_pRefITEM->Kind)
		{
			PrevetionDeleteItem(FALSE, bInven);																				  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
			//SetEnchantItemInfo();
		}
		else if (ITEMKIND_COLOR_ITEM == m_pRefITEM->Kind)
		{
			SetArmorColorInfo(FALSE, bInven);																			 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
			//SetEnchantItemInfo();
		}
		// 2006-03-30 by ispark �Ǽ��縮 �߰� (ex. ���� ������)
		else if (ITEMKIND_ACCESSORY_UNLIMITED == m_pRefITEM->Kind)
		{
			SetAccessoryUnLimitItemInfo(FALSE, bInven);															 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_ACCESSORY_TIMELIMIT == m_pRefITEM->Kind)
		{
			SetAccessoryTimeLimitItemInfo(FALSE, bInven);															// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_INFLUENCE_BUFF == m_pRefITEM->Kind)
		{
			SetInfluenceBuffItemInfo(FALSE, bInven);																	 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_INFLUENCE_GAMEEVENT == m_pRefITEM->Kind)
		{
			SetInfluenceGameEventItemInfo(FALSE, bInven);															// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_RANDOMBOX == m_pRefITEM->Kind)
		{
			SetRandomBoxItemInfo(FALSE, bInven);																		 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_MARK == m_pRefITEM->Kind)
		{
			SetMarkItemInfo(FALSE, bInven);																					  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_SKILL_SUPPORT_ITEM == m_pRefITEM->Kind)
		{
			SetSkillSupportItem(FALSE, bInven);																			   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		else if (ITEMKIND_PET_ITEM == m_pRefITEM->Kind)
		{	// 2010-06-15 by shcho&hslee ��ý���
			SetPetItemInfo(FALSE, bInven);																						// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}

		/*
		if(COMPARE_RACE(g_pShuttleChild->m_myShuttleInfo.Race,RACE_OPERATION|RACE_GAMEMASTER) && m_pRefItemInfo)
		{
			wsprintf( m_strItemInfo[0], "%s (%I64d)", m_strItemInfo[0], m_pRefItemInfo->UniqueNumber );
		}
		*/
		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		else if (ITEMKIND_PET_SOCKET_ITEM == m_pRefITEM->Kind)
		{	// 2010-06-15 by shcho&hslee ��ý���
			SetPetSoketItemInfo(FALSE, bInven);																			// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
		}
		//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���



		if (m_ptItemInfo.x > g_pD3dApp->GetBackBufferDesc().Width - (m_nMaxLength + 12))
		{
			m_ptItemInfo.x = g_pD3dApp->GetBackBufferDesc().Width - (m_nMaxLength + 12);
		}
		if (m_ptItemInfo.y > g_pD3dApp->GetBackBufferDesc().Height - (14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + 1) + 20))
		{
			m_ptItemInfo.y = g_pD3dApp->GetBackBufferDesc().Height - (14 * (m_nDescIndex + 1) + 14 * (m_nDescLine + 1) + 20);
		}
	}
	else
	{
		InitItemInfo();
	}

}
// 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
//void CINFItemInfo::SetItemInfoNormal( ITEM* pITEM, int x, int y, BOOL bShop, int nItemCount )//(ITEM_GENERAL* pItem, ITEM* pItemInfo,ITEM_ENCHANT* pEnchant, int x, int y)
void CINFItemInfo::SetItemInfoNormal(ITEM* pITEM, int x, int y, BOOL bShop, int nItemCount, int nLinkItem/*=0*/, BOOL ArmorCollect/*FALSE*/)//(ITEM_GENERAL* pItem, ITEM* pItemInfo,ITEM_ENCHANT* pEnchant, int x, int y)// 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
{
	m_ptItemInfo.x = x;
	m_ptItemInfo.y = y;// + ITEMINFO_BIGICON_GAB;

	if (m_pBigIcon)
	{
		//		m_ptItemInfo.y = m_ptItemInfo.y + ITEMINFO_BIGICON_GAB;
		m_pBigIcon->InvalidateDeviceObjects();
		m_pBigIcon->DeleteDeviceObjects();
		util::del(m_pBigIcon);
	}

	if (pITEM)
	{
		// 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�
//		m_pBigIcon = FindBigIcon(pITEM->SourceIndex);			// 2005-08-23 by ispark
		if (!nLinkItem)
		{
			m_pBigIcon = FindBigIcon(pITEM->SourceIndex);
		}
		else
		{
			m_pBigIcon = FindBigIcon(nLinkItem);
		}
		// END 2013-04-11 by bhsohn �ΰ��� ����â ���� ���� ��þƮ �ý��� �˻�

		m_nOtherItemCount = nItemCount;	
		m_pRefItemInfo = NULL;
		m_pRefEnchant = NULL;
		m_pRefPrefixRareInfo = NULL;
		m_pRefSuffixRareInfo = NULL;
		m_pRefITEM = pITEM;
		m_bEnableItem = IsEnableItem(pITEM);
		m_bShow = TRUE;
		m_vecTickFuntionIndex.clear();

#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
		// 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
		if (ArmorCollect)	//ssjung _111
		{
			SetArmorCollectionItemInfo(bShop);
		}
		else
#endif
		{

			// end 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
			if (IS_PRIMARY_WEAPON(pITEM->Kind))
			{
				SetPrimaryWeaponInfo(bShop);
			}
			else if (IS_SECONDARY_WEAPON(pITEM->Kind))
			{
				SetSecondaryWeaponInfo(bShop);
			}
			else if (IS_SKILL_ITEM(pITEM->Kind))
			{
				SetSkillItemInfo(bShop);
			}
			else if (ITEMKIND_DEFENSE == pITEM->Kind)
			{
				SetDefenseItemInfo(bShop);
			}
			else if (ITEMKIND_SUPPORT == pITEM->Kind)
			{
				SetSupportItemInfo(bShop);
			}
			else if (ITEMKIND_ENERGY == pITEM->Kind)
			{
				SetEnergyItemInfo(bShop);
			}
			else if (ITEMKIND_INGOT == pITEM->Kind)
			{
				SetIngotItemInfo(bShop);
			}
			else if (ITEMKIND_CARD == pITEM->Kind)
			{
				SetCardItemInfo(bShop);
			}
			else if (ITEMKIND_ENCHANT == pITEM->Kind || ITEMKIND_GAMBLE == pITEM->Kind)
			{
				SetEnchantItemInfo(bShop);
			}
			else if (ITEMKIND_TANK == pITEM->Kind)
			{
				SetAccessoryUnLimitItemInfo(bShop);
			}
			else if (ITEMKIND_BULLET == pITEM->Kind)
			{
				SetBulletItemInfo(bShop);
			}
			else if (ITEMKIND_QUEST == m_pRefITEM->Kind)
			{
				SetIngotItemInfo(bShop);
				//SetEnchantItemInfo();
			}
			else if (ITEMKIND_RADAR == m_pRefITEM->Kind)
			{
				SetRadarItemInfo(bShop);
				//SetEnchantItemInfo();
			}
			else if (ITEMKIND_COMPUTER == m_pRefITEM->Kind)
			{
				SetComputerItemInfo(bShop);
				//SetEnchantItemInfo();
			}
			else if (ITEMKIND_PREVENTION_DELETE_ITEM == m_pRefITEM->Kind)
			{
				PrevetionDeleteItem(bShop);
			}
			else if (ITEMKIND_COLOR_ITEM == m_pRefITEM->Kind)
			{
				SetArmorColorInfo(bShop);
			}
			// 2006-03-30 by ispark �Ǽ��縮 �߰� (ex. ���� ������)
			else if (ITEMKIND_ACCESSORY_UNLIMITED == m_pRefITEM->Kind)
			{
				SetAccessoryUnLimitItemInfo(bShop);
			}
			else if (ITEMKIND_ACCESSORY_TIMELIMIT == m_pRefITEM->Kind)
			{
				SetAccessoryTimeLimitItemInfo(bShop);
			}
			else if (ITEMKIND_INFLUENCE_BUFF == m_pRefITEM->Kind)
			{
				SetInfluenceBuffItemInfo(bShop);
			}
			else if (ITEMKIND_INFLUENCE_GAMEEVENT == m_pRefITEM->Kind)
			{
				SetInfluenceGameEventItemInfo(bShop);
			}
			else if (ITEMKIND_RANDOMBOX == m_pRefITEM->Kind)
			{
				SetRandomBoxItemInfo(bShop);
			}
			else if (ITEMKIND_MARK == m_pRefITEM->Kind)
			{
				SetMarkItemInfo();
			}
			else if (ITEMKIND_SKILL_SUPPORT_ITEM == pITEM->Kind)
			{
				SetSkillSupportItem(bShop);
			}
			else if (ITEMKIND_PET_ITEM == m_pRefITEM->Kind)
			{
				SetPetItemInfo(bShop);
			}
			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			else if (ITEMKIND_PET_SOCKET_ITEM == m_pRefITEM->Kind)
			{	// 2010-06-15 by shcho&hslee ��ý���
				SetPetSoketItemInfo();
			}
			//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
		}
	}
	else
	{
		InitItemInfo();
	}

}
// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
void CINFItemInfo::SetRareParameterInfo(char* str, float fValue)
{
	char  temp[256];
	wsprintf(temp, "[%.2f]", fValue);
	sprintf(str, "\\g%s\\g", temp);
}
//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����

// 2010. 02. 11 by ckPark �ߵ��� ����������
//void CINFItemInfo::SetParameterInfo(char * str, BYTE bType,float fValue)
// 2013-03-27 by bhsohn DestParam �ڷ��� ����
//void CINFItemInfo::SetParameterInfo( char * str, BYTE bType,float fValue, BOOL bDefEnchant /* = TRUE */ )
void CINFItemInfo::SetParameterInfo(char * str, DestParam_t bType, float fValue, BOOL bDefEnchant /* = TRUE */, BOOL bMonthlyArmor /*FALSE*/)// 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
// end 2010. 02. 11 by ckPark �ߵ��� ����������

{
	FLOG("CINFItemInfo::SetParameterInfo(char * str, BYTE bType,float fValue)");
	char buf[128];
	FUNCTION_VALUE_TYPE nValueType = FUNCTION_VALUE_TYPE_NORMAL;
	memset(buf, 0x00, sizeof(buf));
	switch (bType)
	{
	case DES_NULL:					// ��� �Ķ���Ͱ� ���� ��� ���
	{
		wsprintf(str, STRMSG_C_PARAM_0001);//"���� ����"
		return;
	}
	break;
	case DES_ATTACK_PART:			// ����
		wsprintf(buf, STRMSG_C_PARAM_0002);//"[����"
		break;
	case DES_DEFENSE_PART:		// ����-->���
		wsprintf(buf, STRMSG_C_PARAM_0003);//"[���"
		break;
	case DES_FUEL_PART:				// ����
		wsprintf(buf, STRMSG_C_PARAM_0004);//"[����"
		break;
	case DES_SOUL_PART:				// ����
		wsprintf(buf, STRMSG_C_PARAM_0005);//"[����"
		break;
	case DES_SHIELD_PART:			// ���-->����
		wsprintf(buf, STRMSG_C_PARAM_0006);//"[����"
		break;
	case DES_DODGE_PART:			// ȸ��
		wsprintf(buf, STRMSG_C_PARAM_0007);//"[ȸ��"
		break;
	case DES_BODYCONDITION:			// ������(������,���ݸ��)
		wsprintf(str, STRMSG_C_PARAM_0008);//"[���ֻ��º�ȭ]"
		return;
	case DES_ENDURANCE_01: 			// ������ 01
		wsprintf(buf, STRMSG_C_PARAM_0009);//"[�⺻���� ������"
		break;
	case DES_ENDURANCE_02:			// ������ 02
		wsprintf(buf, STRMSG_C_PARAM_0009);//"[�⺻���� ������"
		break;
	case DES_CHARGING_01:			// ��ź�� 01
		wsprintf(buf, STRMSG_C_PARAM_0010);//"[�⺻���� ��ź��"
		break;
	case DES_CHARGING_02:			// ��ź�� 02
		wsprintf(buf, STRMSG_C_PARAM_0011);//"[��޹��� ��ź��"
		break;
	case DES_PROPENSITY:			// ����
		wsprintf(buf, STRMSG_C_PARAM_0012);//"[����"
		break;
	case DES_HP:					// ��Ʈ����Ʈ
		wsprintf(buf, STRMSG_C_PARAM_0013);//"[������"
		break;
	case DES_MAX_SP_UP:					// 2010-08-26 by shcho&&jskim, �� ������ ����
	case DES_SP:					// �ҿ�����Ʈ
		wsprintf(buf, STRMSG_C_PARAM_0014);//"[��ų����Ʈ"
		break;
	case DES_EP:					// ��������Ʈ
		wsprintf(buf, STRMSG_C_PARAM_0004);//"[����"
		break;
	case DES_DP:					// ��������Ʈ
		//wsprintf(buf,STRMSG_C_PARAM_0006);//"[����"
		wsprintf(buf, STRMSG_C_PARAM_0006_1);//"[����"	// 2013-01-23 by mspark, ĳ���� CPU ���� ����, ��� ���差 ����
		break;
	case DES_SPRECOVERY:			// �ҿ�����Ʈȸ����			C
		wsprintf(buf, STRMSG_C_PARAM_0015);//"[��ų����Ʈȸ����"
		break;
	case DES_HPRECOVERY:			// ����������Ʈȸ����		C
		wsprintf(buf, STRMSG_C_PARAM_0016);//"[������ȸ����"
		break;
	case DES_MINATTACK_01:				//  ���ݷ� 1��			C
		wsprintf(buf, STRMSG_C_PARAM_0017);//"[�⺻���� ���ݷ�(�ּ�)"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_MINATTACK_02:	            //  ���ݷ� 2��			C
		wsprintf(buf, STRMSG_C_PARAM_0018);//"[��޹��� ���ݷ�(�ּ�)"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_MAXATTACK_01:				//  ���ݷ� 1��			C
		wsprintf(buf, STRMSG_C_PARAM_0019);//"[�⺻���� ���ݷ�(�ִ�)"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_MAXATTACK_02:	            //  ���ݷ� 2��			C
		wsprintf(buf, STRMSG_C_PARAM_0020);//"[��޹��� ���ݷ�(�ִ�)"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_ATTACKPROBABILITY_01:	// ����Ȯ�� 01				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0021);//"[�⺻���� ����Ȯ��"
		// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_ATTACKPROBABILITY_02:    // ����Ȯ�� 02				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0022);//"[��޹��� ����Ȯ��"
		// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DEFENSE_01:			// ���� 01				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0023);//"[�⺻���⿡ ���� ����"
		// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DEFENSE_02:			// ���� 02				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0024);//"[��޹��⿡ ���� ����"
		// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DEFENSEPROBABILITY_01:	// ���Ȯ�� 01				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0025);//"[�⺻���⿡ ���� ȸ�Ƿ�"
		// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DEFENSEPROBABILITY_02:	// ���Ȯ�� 02				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0026);//"[��޹��⿡ ���� ȸ�Ƿ�"
		// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_SKILLPROBABILITY_01:		// ���ε� ��Ʈ�Ѱ���Ȯ��				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0027);//"[��ų �⺻���� ����Ȯ��"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_SKILLPROBABILITY_02:		// ���ε� ��Ʈ�Ѱ���Ȯ��				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0028);//"[��ų ��޹��� ����Ȯ��"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_FACTIONRESISTANCE_01:		// �Ӽ����׷�				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0029);//"[�⺻���� �Ӽ����׷�"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_FACTIONRESISTANCE_02:		// �Ӽ����׷�				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0030);//"[��޹��� �Ӽ����׷�"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_SPEED:					// �̵��ӵ�					C
		wsprintf(buf, STRMSG_C_PARAM_0031);//"[�̵��ӵ�"
		nValueType = FUNCTION_VALUE_TYPE_PROB100; // 2005-11-26 by ispark
		break;
	case DES_TRANSPORT:				// ��ݷ�
		wsprintf(buf, STRMSG_C_PARAM_0032);//"[��ݷ�"
		break;
	case DES_MATERIAL:				// ����
		wsprintf(buf, STRMSG_C_PARAM_0033);//"[����"
		break;
	case DES_REATTACKTIME_01:		// (*) ������Ÿ�� 01		C
		wsprintf(buf, STRMSG_C_PARAM_0034);//"[�⺻���� ����ݽð�"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_REATTACKTIME_02:		// (*) ������Ÿ�� 02		C
		wsprintf(buf, STRMSG_C_PARAM_0035);//"[��޹��� ����ݽð�"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_ABRASIONRATE_01:		// ������ 01				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0036);//"[�⺻���� ������"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_ABRASIONRATE_02:		// ������ 02				C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0037);//"[��޹��� ������"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_RANGE_01:				// (*) ��ȿ�Ÿ� 01			C
		wsprintf(buf, STRMSG_C_PARAM_0038);//"[�⺻���� ��ȿ�Ÿ�"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_RANGE_02:				// (*) ��ȿ�Ÿ� 02			C
		wsprintf(buf, STRMSG_C_PARAM_0039);//"[��޹��� ��Ÿ�"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_RANGEANGLE_01:			// ��ȿ���� 01				C
		wsprintf(buf, STRMSG_C_PARAM_0040);//"[�⺻���� ��ȿ����"
		break;
	case DES_RANGEANGLE_02:			// ��ȿ���� 02				C
		wsprintf(buf, STRMSG_C_PARAM_0041);//"[��޹��� ��ȿ����"
		break;
	case DES_MULTITAGET_01:			// ��ƼŸ��					C
		wsprintf(buf, STRMSG_C_PARAM_0042);//"[�⺻���� ��ƼŸ��"
		break;
	case DES_MULTITAGET_02:			// ��ƼŸ��					C
		wsprintf(buf, STRMSG_C_PARAM_0043);//"[��޹��� ��ƼŸ��"
		break;
	case DES_EXPLOSIONRANGE_01:		// ���߹ݰ�					C
		wsprintf(buf, STRMSG_C_PARAM_0044);//"[�⺻���� ���߹ݰ�"
		break;
	case DES_EXPLOSIONRANGE_02:		// ���߹ݰ�					C
		wsprintf(buf, STRMSG_C_PARAM_0045);//"[��޹��� ���߹ݰ�"
		break;
	case DES_UNIT:					// ������ ���� (28 ~ 29�� ���� ���� ���ָ����� ���������� ����)
		wsprintf(buf, STRMSG_C_PARAM_0046);//"[������ ����"
		break;
	case DES_REVISION:				// ������ ������ (28 ~ 29�� ���� ���� ���ָ����� ���������� ����)
		wsprintf(buf, STRMSG_C_PARAM_0047);//"[������ ������"
		break;
	case DES_FACTIONPROBABILITY_01:		// �Ӽ��� ���� ���Ȯ��		C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0048);//"[�⺻���� �Ӽ��� ���� ȸ�Ƿ�"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_FACTIONPROBABILITY_02:		// �Ӽ��� ���� ���Ȯ��		C, PROB256_MAX_VALUE
		wsprintf(buf, STRMSG_C_PARAM_0049);//"[��޹��� �Ӽ��� ���� ȸ�Ƿ�"
		nValueType = FUNCTION_VALUE_TYPE_PROB255;
		break;
	case DES_SHOTNUM_01:				// ������ �� �߻� ��		C
		wsprintf(buf, STRMSG_C_PARAM_0050);//"[�⺻���� �����"
		break;
	case DES_SHOTNUM_02:				// ������ �� �߻� ��		C
		wsprintf(buf, STRMSG_C_PARAM_0051);//"[��޹��� �����"
		break;
	case DES_MULTINUM_01:				// ���� �߻� ��				C
		wsprintf(buf, STRMSG_C_PARAM_0052);//"[�⺻���� ��Ƽ��"
		break;
	case DES_MULTINUM_02:				// ���� �߻� ��				C
		wsprintf(buf, STRMSG_C_PARAM_0053);//"[��޹��� ��Ƽ��"
		break;
	case DES_ATTACKTIME_01:			// ó�� ���� ���� Ÿ�� 01	C
		wsprintf(buf, STRMSG_C_PARAM_0054);//"[�⺻���� �ߵ��ð�"
		break;
	case DES_ATTACKTIME_02:			// ó�� ���� ���� Ÿ�� 02	C
		wsprintf(buf, STRMSG_C_PARAM_0055);//"[��޹��� �ߵ��ð�"
		break;
	case DES_TIME_01:				// (*)���� �ð� 01				C
		wsprintf(buf, STRMSG_C_PARAM_0056);//"[�⺻���� �����ð�"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_TIME_02:				// (*)���� �ð� 02				C
		wsprintf(buf, STRMSG_C_PARAM_0057);//"[��޹��� �����ð�"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_WEIGHT_01:				// (*)���� 			C
		wsprintf(buf, STRMSG_C_PARAM_0058);//"[�⺻���� ����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_WEIGHT_02:				// (*)���� 			C
		wsprintf(buf, STRMSG_C_PARAM_0059);//"[��޹��� ����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_REACTION_RANGE:
		wsprintf(buf, STRMSG_C_PARAM_0060);//"[��޹��� �����Ÿ�"
		break;
	case DES_OVERHITTIME_01:// (*) ������
	case DES_OVERHITTIME_02:// (*) ������
		wsprintf(buf, STRMSG_C_050620_0001);//"[������ȸ���ð�"
		break;
	case DES_BAZAAR_SELL:
		wsprintf(str, STRMSG_C_060801_0008);//"[�ǸŻ����� ������ �� �ִ�. ���ڸ��� ���� ?������ ��밡��]"
		return;
	case DES_BAZAAR_BUY:
		wsprintf(str, STRMSG_C_060801_0009);//"[���Ż����� ������ �� �ִ�. ���ڸ��� ���� ?������ ��밡��]"
		return;
	case DES_UNITKIND:
	{// ��ü ���׷��̵�� �ش� ��ü
		wsprintf(buf, STRMSG_C_PARAM_0061);//"[���׷��̵� ��ü:"
		switch ((USHORT)fValue)
		{
		case UNITKIND_BT01:
			sprintf(str, STRMSG_C_PARAM_0062, buf);//"%sB-GEAR 1��]"
			break;
		case UNITKIND_BT02:
			sprintf(str, STRMSG_C_PARAM_0063, buf);//"%sB-GEAR 2��]"
			break;
		case UNITKIND_BT03:
			sprintf(str, STRMSG_C_PARAM_0064, buf);//"%sB-GEAR 3��]"
			break;
		case UNITKIND_BT04:
			sprintf(str, STRMSG_C_PARAM_0065, buf);//"%sB-GEAR 4��]"
			break;
		case UNITKIND_OT01:
			sprintf(str, STRMSG_C_PARAM_0066, buf);//"%sM-GEAR 1��]"
			break;
		case UNITKIND_OT02:
			sprintf(str, STRMSG_C_PARAM_0067, buf);//"%sM-GEAR 2��]"
			break;
		case UNITKIND_OT03:
			sprintf(str, STRMSG_C_PARAM_0068, buf);//"%sM-GEAR 3��]"
			break;
		case UNITKIND_OT04:
			sprintf(str, STRMSG_C_PARAM_0069, buf);//"%sM-GEAR 4��]"
			break;
		case UNITKIND_DT01:
			sprintf(str, STRMSG_C_PARAM_0070, buf);//"%sA-GEAR 1��]"
			break;
		case UNITKIND_DT02:
			sprintf(str, STRMSG_C_PARAM_0071, buf);//"%sA-GEAR 2��]"
			break;
		case UNITKIND_DT03:
			sprintf(str, STRMSG_C_PARAM_0072, buf);//"%sA-GEAR 3��]"
			break;
		case UNITKIND_DT04:
			sprintf(str, STRMSG_C_PARAM_0073, buf);//"%sA-GEAR 4��]"
			break;
		case UNITKIND_ST01:
			sprintf(str, STRMSG_C_PARAM_0074, buf);//"%sI-GEAR 1��]"
			break;
		case UNITKIND_ST02:
			sprintf(str, STRMSG_C_PARAM_0075, buf);//"%sI-GEAR 2��]"
			break;
		case UNITKIND_ST03:
			sprintf(str, STRMSG_C_PARAM_0076, buf);//"%sI-GEAR 3��]"
			break;
		case UNITKIND_ST04:
			sprintf(str, STRMSG_C_PARAM_0077, buf);//"%sI-GEAR 4��]"
			break;
		}
		return;
	}
	break;
	case DES_ITEMKIND:				// ����������
	{
		wsprintf(buf, STRMSG_C_PARAM_0078);//"[������:"
		switch ((USHORT)fValue)
		{
		case ITEMKIND_AUTOMATIC:
			sprintf(str, STRMSG_C_PARAM_0079, buf);//"%s�����ƽ]"
			break;
		case ITEMKIND_VULCAN:
			sprintf(str, STRMSG_C_PARAM_0080, buf);//"%s��ĭ]"
			break;
			//				case ITEMKIND_GRENADE:
			//					sprintf(str,STRMSG_C_PARAM_0081,buf);//"%s�׷����̵�]" -> "%s��󸮽�Ʈ]"
		case ITEMKIND_DUALIST:						// 2005-08-02 by ispark
			sprintf(str, STRMSG_C_050802_0001, buf);//"%s��󸮽�Ʈ]"
			break;
		case ITEMKIND_CANNON:
			sprintf(str, STRMSG_C_PARAM_0082, buf);//"%sĳ��]"
			break;
		case ITEMKIND_RIFLE:
			sprintf(str, STRMSG_C_PARAM_0083, buf);//"%s������]"
			break;
		case ITEMKIND_GATLING:
			sprintf(str, STRMSG_C_PARAM_0084, buf);//"%s��Ʋ��]"
			break;
		case ITEMKIND_LAUNCHER:
			sprintf(str, STRMSG_C_PARAM_0085, buf);//"%s����]"
			break;
		case ITEMKIND_MASSDRIVE:
			sprintf(str, STRMSG_C_PARAM_0086, buf);//"%s�Ž�����̺�]"
			break;
		case ITEMKIND_ROCKET:
			sprintf(str, STRMSG_C_PARAM_0087, buf);//"%s���Ϸ�]"
			break;
		case ITEMKIND_MISSILE:
			sprintf(str, STRMSG_C_PARAM_0088, buf);//"%s�̻��Ϸ�]"
			break;
		case ITEMKIND_BUNDLE:
			sprintf(str, STRMSG_C_PARAM_0089, buf);//"%s�����]"
			break;
		case ITEMKIND_MINE:
			sprintf(str, STRMSG_C_PARAM_0090, buf);//"%s���η�]"
			break;
		case ITEMKIND_SHIELD:
			sprintf(str, STRMSG_C_PARAM_0091, buf);//"%s����]"
			break;
		case ITEMKIND_DUMMY:
			sprintf(str, STRMSG_C_PARAM_0092, buf);//"%s����]"
			break;
		case ITEMKIND_FIXER:
			sprintf(str, STRMSG_C_PARAM_0093, buf);//"%s�ȼ�]"
			break;
		case ITEMKIND_DECOY:
			sprintf(str, STRMSG_C_PARAM_0094, buf);//"%s������]"
			break;
		case ITEMKIND_ALL_WEAPON:
			sprintf(str, STRMSG_C_PARAM_0095, buf);//"%s��� ����]"
			break;
		case ITEMKIND_PRIMARY_WEAPON_ALL:
			sprintf(str, STRMSG_C_PARAM_0096, buf);//"%s�⺻����]"
			break;
		case ITEMKIND_PRIMARY_WEAPON_1:
			sprintf(str, STRMSG_C_PARAM_0097, buf);//"%s�Ѿ��� �⺻����]"
			break;
		case ITEMKIND_PRIMARY_WEAPON_2:
			sprintf(str, STRMSG_C_PARAM_0098, buf);//"%s������ �⺻����]"
			break;
		case ITEMKIND_SECONDARY_WEAPON_ALL:
			sprintf(str, STRMSG_C_PARAM_0099, buf);//"%s��޹���]"
			break;
		case ITEMKIND_SECONDARY_WEAPON_1:
			sprintf(str, STRMSG_C_PARAM_0099, buf);//"%s��޹���]"
			break;
		case ITEMKIND_SECONDARY_WEAPON_2:
			sprintf(str, STRMSG_C_PARAM_0100, buf);//"%s��޹���(����)]"
			break;
		case ITEMKIND_DEFENSE:
			sprintf(str, STRMSG_C_PARAM_0101, buf);//"%s�Ƹ�]"
			break;
		case ITEMKIND_SUPPORT:
			sprintf(str, STRMSG_C_PARAM_0102, buf);//"%s����/����]"
			break;
		case ITEMKIND_BLASTER:					// 2005-08-02 by ispark
			sprintf(str, STRMSG_C_050802_0002, buf);//"%s������]"
			break;
		case ITEMKIND_RAILGUN:					// 2005-08-02 by ispark
			sprintf(str, STRMSG_C_050802_0003, buf);//"%s���ϰ�]"
			break;
		}
		return;
	}
	break;
	case DES_GROUNDMODE:			// ���� ���
		wsprintf(buf, STRMSG_C_PARAM_0103);//"[���� ���"
		break;
	case DES_SIEGEMODE:				// ���� ���
		wsprintf(buf, STRMSG_C_PARAM_0104);//"[���� ���"
		break;
	case DES_IMMEDIATE_HP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0105);//"[������"
		break;
	case DES_IMMEDIATE_DP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0106);//"[����"
		break;
	case DES_IMMEDIATE_SP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0107);//"[��ųP"
		break;
	case DES_IMMEDIATE_EP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0108);//"[����"
		break;
		// 2004-12-06 by ydkim ��æƮ ����
	case DES_RARE_FIX_PREFIX:
		wsprintf(str, STRMSG_C_PARAM_0109);//"���λ�"
		return;
	case DES_RARE_FIX_SUFFIX:
		wsprintf(str, STRMSG_C_PARAM_0110);//"���̻�"
		return;
	case DES_RARE_FIX_BOTH:
		wsprintf(str, STRMSG_C_PARAM_0111);//"���λ�, ���̻�"
		return;
	case DES_RARE_FIX_PREFIX_INITIALIZE:
		wsprintf(str, STRMSG_C_PARAM_0112);//"��繫���� ���λ� ���� �ɼ� �ʱ�ȭ"
		return;
	case DES_RARE_FIX_SUFFIX_INITIALIZE:
		wsprintf(str, STRMSG_C_PARAM_0113);//"��繫���� ���̻� ���� �ɼ� �ʱ�ȭ"
		return;
		// 2005-11-22 by ispark ���� ������ ��ų ����
	case DES_SKILL_REDUCE_SHIELD_DAMAGE:
		wsprintf(buf, STRMSG_C_051122_0001);// "[���� ������ ����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_ATTACK_RANGE_01:
		wsprintf(buf, STRMSG_C_051122_0003);// "[���̴� 1�� ��Ÿ� ����"
		nValueType = FUNCTION_VALUE_TYPE_PERCENT;
		break;
	case DES_ATTACK_RANGE_02:
		wsprintf(buf, STRMSG_C_051122_0004);// "[���̴� 2�� ��Ÿ� ����"
		nValueType = FUNCTION_VALUE_TYPE_PERCENT;
		break;
	case DES_SKILL_COLLISIONDAMAGE_DOWN:
		wsprintf(buf, STRMSG_C_051122_0002);// "[��� ������ ����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SKILL_REVERSEENGINE:
		wsprintf(str, STRMSG_C_051126_0001);//  "[���� ����]"
		return;
	case DES_SKILL_SMARTSP:
		wsprintf(buf, STRMSG_C_051122_0008);// "[SP �Ҹ� ����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SKILL_SUMMON_FORMATION_MEMBER:
		wsprintf(str, STRMSG_C_051125_0001);// "[���� 1�� ��ȯ]"
		return;
	case DES_SKILL_REACTIONSPEED:
		wsprintf(buf, STRMSG_C_051128_0001);// "[���� ��ȸ��"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SKILL_ENGINEANGLE:
		wsprintf(buf, STRMSG_C_051202_0001);//  "[���� ��ȸ��"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SKILL_ENGINEBOOSTERANGLE:
		wsprintf(buf, STRMSG_C_051202_0002);// "[�ν��Ϳ��� ��ȸ��"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_WARP:
		wsprintf(str, STRMSG_C_051222_0100);// "[���÷� ��ȯ]"
		return;
	case DES_SKILL_SLOWMOVING:
		wsprintf(buf, STRMSG_C_051230_0100);// "[�̵��ӵ� ����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_CASH_NORMAL_RESTORE:
		wsprintf(buf, STRMSG_C_060424_0003);// "[��ȰȮ��"
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
	case DES_DROP_EXP:
		wsprintf(buf, STRMSG_C_060424_0007);// "[����ġ"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_DROP_SPI:
		wsprintf(buf, STRMSG_C_060424_0008);// "[SPI �����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_DROP_ITEM:
		wsprintf(buf, STRMSG_C_060424_0009);// "[������ �����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_HP_REPAIR_RATE_FLIGHTING:
		wsprintf(str, STRMSG_C_060424_0010);// "����� HPȸ�� ����"
		return;
	case DES_DP_REPAIR_RATE:
		wsprintf(buf, STRMSG_C_060424_0011);// "[������ ���� ȸ����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_SP_REPAIR_RATE:
		wsprintf(buf, STRMSG_C_060424_0012);// "[SP ȸ���ӵ�"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
	case DES_CASH_STAT_ALL_INITIALIZE:
		wsprintf(str, STRMSG_C_060424_0013);// "ĳ������ ��� ������ �ʱ�ȭ"
		return;
	case DES_CASH_STAT_PART_INITIALIZE:
		wsprintf(str, STRMSG_C_060424_0014);// "ĳ������ ������ �Ѱ��� ������ �ʱ�ȭ"
		return;
	case DES_CASH_CHANGE_CHARACTERNAME:
		wsprintf(str, STRMSG_C_060424_0015);// "ĳ������ �̸��� ������"
		return;
	case DES_CASH_STEALTH:
		wsprintf(str, STRMSG_C_060424_0021);// "��� ������ �񼱰�ȭ"
		return;
	case DES_KILLMARK_EXP:
		wsprintf(buf, STRMSG_C_060424_0007);// "[����ġ"
		break;
	case DES_GRADUAL_DP_UP:
		wsprintf(buf, STRMSG_C_PARAM_0106);//"[����"
		break;
	case DES_SKILL_CHAFF_HP:
		wsprintf(buf, STRMSG_C_061206_0103);//[ü��HP
		break;
	case DES_SKILL_HALLUCINATION:
		wsprintf(buf, STRMSG_C_061208_0101);//"[ü������"
		break;
	case DES_SKILL_HYPERSHOT:
		wsprintf(buf, STRMSG_C_PARAM_0044);
		break;
	case DES_SKILL_ROLLING_TIME:
		wsprintf(str, STRMSG_C_061206_0104);
		return;
	case DES_SKILL_SHIELD_PARALYZE:
		wsprintf(str, STRMSG_C_061206_0105);
		return;
	case DES_SKILL_BARRIER:
		wsprintf(str, STRMSG_C_061206_0106);
		return;
	case DES_SKILL_BIG_BOOM:
		wsprintf(str, STRMSG_C_061206_0107);
		return;
	case DES_INVISIBLE:
		wsprintf(str, STRMSG_C_061206_0108);
		return;
	case DES_SKILL_CANCELALL:
		wsprintf(str, STRMSG_C_061206_0109);
		return;
	case DES_SKILL_INVINCIBLE:
		wsprintf(str, STRMSG_C_061206_0110);
		return;
	case DES_SKILL_FULL_RECOVERY:
		wsprintf(str, STRMSG_C_061206_0111);
		return;
	case DES_SKILL_SCANNING:
		wsprintf(str, STRMSG_C_061206_0112);
		return;
	case DES_SKILL_NO_WARNING:
		wsprintf(str, STRMSG_C_061206_0113);
		return;
	case DES_SKILL_CAMOUFLAGE:
		wsprintf(str, STRMSG_C_061206_0114);
		return;
	case DES_WARHEAD_SPEED:
		wsprintf(buf, STRMSG_C_070614_0100);
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;
		// 2007-09-12 by bhsohn ���� ���� ��Ż ������ ���� �߰�
	case DES_WARP_OUTPOST:
	{
		wsprintf(str, STRMSG_C_070912_0201);
		return;
	}
	break;
	// end2007-09-12 by bhsohn ���� ���� ��Ż ������ ���� �߰�
	// 2007-09-12 by bhsohn ����Ŀ ������ ���� �߰�
	case DES_CHAT_ALL_INFLUENCE:
	{
		wsprintf(str, STRMSG_C_070912_0202);
		return;
	}
	break;
	// end 2007-09-12 by bhsohn ����Ŀ ������ ���� �߰�
// 2007-11-23 by dgwoo ĳ���� ���� ī�� 
	case DES_CASH_CHANGE_PILOTFACE:
	{
		wsprintf(str, STRMSG_C_071123_0100);
		return;
	}
	break;
	// 2007-12-17 by bhsohn ��þƮ �ʱ�ȭ ī�� ���� �߰�
	case DES_ENCHANT_INITIALIZE:
	{
		wsprintf(str, STRMSG_C_071217_0201);
		return;
	}
	break;
	// end 2007-12-17 by bhsohn ��þƮ �ʱ�ȭ ī�� ���� �߰�
		// 2008-06-18 by bhsohn ���ܿ����� ī�� ���� ó��
	case DES_CASH_GUILD:
	{
		wsprintf(str, STRMSG_C_080619_0203);	//"���� ���� �ִ� ���� �ο� ����"
		return;
	}
	break;
	// end 2008-06-18 by bhsohn ���ܿ����� ī�� ���� ó��
// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	case DES_ENGINE_BOOSTER_TIME_UP:	// �ν��� �ð� ����
	{
		sprintf(str, STRMSG_C_080929_0200, fValue);
		return;
	}
	break;
	case DES_ENGINE_MAX_SPEED_UP:		// ���� �Ϲݼӵ�(�ִ�) ����
	{
		sprintf(str, STRMSG_C_080929_0201, fValue);
		return;
	}
	break;
	case DES_ENGINE_MIN_SPEED_UP:		// ���� �Ϲݼӵ�(�ּ�) ����
	{
		sprintf(str, STRMSG_C_080929_0202, fValue);
		return;
	}
	break;
	case DES_ENGINE_BOOSTER_SPEED_UP:		// ���� �ν��ͼӵ� ����
	{
		sprintf(str, STRMSG_C_080929_0203, fValue);
		return;
	}
	break;
	case DES_ENGINE_GROUND_SPEED_UP:		// ���� ����ӵ� ����
	{
		sprintf(str, STRMSG_C_080929_0204, fValue);
		return;
	}
	break;
	case DES_RADAR_OBJECT_DETECT_RANGE:		// ���̴� ��ü ���� �ݰ�
	{
		sprintf(str, STRMSG_C_080929_0205, fValue);
		return;
	}
	break;
	case DES_PIERCE_UP_01:		// �⺻���� �Ǿ�� ���� ī��
	{
		sprintf(buf, STRMSG_C_080929_0206);
		// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;	
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_PIERCE_UP_02:		// ��޹��� �Ǿ�� ���� ī��
	{
		sprintf(buf, STRMSG_C_080929_0207);
		// 2010-07-28 by dgwoo Ȯ�� ���� ���� (255 => 100%)
		//nValueType = FUNCTION_VALUE_TYPE_PROB255;	
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	// 2010. 12. 05 by jskim, ��������� ���Ȯ�� ���� ������ ����
	case DES_RARE_ITEM_DROP_RATE:
	{
		wsprintf(buf, STRMSG_C_101206_0401);	 ///	"[���� ������ �����"
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
	}
	break;
	// end 2010. 12. 05 by jskim, ��������� ���Ȯ�� ���� ������ ����
	// 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����
	case DES_RARE_ITEM_PARTNER_SPEED:
	{
		sprintf(str, STRMSG_C_101221_0401, fValue * 100);		// "���� �̵� �ӵ� %.2f%% ����"
		return;
	}
	break;
	// end 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����
	// 2010-12-21 by jskim, ��Ʈ�� ������ ���� ������ ����
	case DES_RARE_ITEM_PARTNER_DAMAGE:
	{
		sprintf(str, STRMSG_C_101221_0402, fValue * 100);		// "��Ʈ�� ������ %.2f%% ����"
		return;
	}
	// end 2010-12-21 by jskim, ��Ʈ�� ������ ���� ������ ����
	// 2010-12-21 by jskim, HP, DP ŰƮ ���뷮 ���� ������ ����
	case DES_RARE_ITEM_HPDP:
	{
		sprintf(str, STRMSG_C_101221_0403, fValue * 100);		// "����, ���� ŰƮ ���뷮 %.2f%% ����"
		return;
	}
	// end 2010-12-21 by jskim, HP, DP ŰƮ ���뷮 ���� ������ ����
	case DES_REQ_MIN_LEVEL:		// ���������� �䱸 MinLevel�� �����
	case DES_REQ_MAX_LEVEL:		// ���������� �䱸 MaxLevel�� �����	
		sprintf(str, STRMSG_C_080929_0211, fValue);
		return;
	case DES_ENGINE_ANGLE_UP:	// ���� ȸ���� ���� ī��
		sprintf(str, STRMSG_C_080929_0214, (fValue / PI*180.0f));
		return;
	case DES_ENGINE_BOOSTERANGLE_UP:	// ���� �ν��� ȸ���� ���� ī��
		sprintf(str, STRMSG_C_080929_0215, (fValue / PI*180.0f));
		return;
		// end 2008-09-26 by bhsohn �ű� ��þƮ ó��
		// 2008-12-30 by bhsohn ������ ä�� ���� ī�� ��ȹ��
	case DES_CHAT_BLOCK:	// ������ ä�� ���� ī�� ���� - 
		wsprintf(str, STRMSG_C_081230_0208, (int)(fValue / 60.0f));
		return;
		// end 2008-12-30 by bhsohn ������ ä�� ���� ī�� ��ȹ��
		// 2009-01-19 by bhsohn "���� ��ȯī��" ���� �߰�
	case DES_CASH_MONSTER_SUMMON:
		wsprintf(str, STRMSG_C_090120_0201);
		return;
		// end 2009-01-19 by bhsohn "���� ��ȯī��" ���� �߰�
		// 2010. 03. 18 by jskim ���ͺ��� ī��
	case DES_TRANSFORM_TO_MONSTER:
		wsprintf(str, STRMSG_C_100401_0401);
		return;
	case DES_TRANSFORM_TO_GEAR:
		wsprintf(str, STRMSG_C_100401_0402);
		return;
		//end 2010. 03. 18 by jskim ���ͺ��� ī��
	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	case DES_ITEM_RESISTANCE:
		sprintf(str, STRMSG_C_091103_0326, fValue);	// "[���� �̻� ����:+%.2f%%]"
		return;
	case DES_ITEM_ADDATTACK:
		if (!bMonthlyArmor) sprintf(str, STRMSG_C_091103_0342, fValue);	// "[�⺻���� �߰� Ÿ��ġ:+%.2f]"
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM		
		else sprintf(str, STRMSG_C_130617_0001, (int)fValue);	//"[���� �⺻���� �߰� Ÿ��ġ:+%d]"		// 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
#endif
		return;
	case DES_ITEM_ADDATTACK_SEC:
		if (!bMonthlyArmor) sprintf(str, STRMSG_C_091103_0343, fValue);	// "[��޹��� �߰� Ÿ��ġ:+%.2f]"
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
		else sprintf(str, STRMSG_C_130617_0002, (int)fValue);	//"[���� ��޹��� �߰� Ÿ��ġ:+%d]"			// 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
#endif
		return;
	case DES_ITEM_IGNOREDEFENCE:
		sprintf(str, STRMSG_C_091103_0328, fValue);	// "[���� ����:+%.2f%%]"
		return;
	case DES_ITEM_IGNOREAVOID:
		sprintf(str, STRMSG_C_091103_0329, fValue);	// "[ȸ�Ƿ� ����:+%.2f%%]"
		return;
	case DES_ITEM_REDUCEDAMAGE:
	{
		sprintf(str, STRMSG_C_091103_0330, fValue);	// "[������ �л�:+%.2f]"
		return;
	}
	case DES_SKILL_RELEASE:
		sprintf(str, STRMSG_C_091103_0347);	// "����� ����� ��ų�� ���� �Ѵ�."
		return;
		// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

		// 2010. 02. 11 by ckPark �ߵ��� ����������
	case DES_PAIR_DRAIN_1_RATE:
		strcpy(buf, STRMSG_C_100218_0301);	// "[�巹�� Ȯ��
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;

	case DES_PAIR_DRAIN_2_HP_DP_UP_RATE:
		strcpy(buf, STRMSG_C_100218_0302);	// "[�ߵ� �� �����
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;

	case DES_PAIR_REFLECTION_1_RATE:
		strcpy(buf, STRMSG_C_100218_0303);	// "[�̷��� Ȯ��
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;

	case DES_PAIR_REFLECTION_2_DAMAGE_RATE:
		strcpy(buf, STRMSG_C_100218_0304);	// "[������ �ݻ緮
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
		break;

	case DES_ANTI_DRAIN_RATE:
		strcpy(buf, STRMSG_C_100218_0305);	// "[�巹�� ���� Ȯ��
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;

	case DES_ANTI_REFLECTION_RATE:
		strcpy(buf, STRMSG_C_100218_0306);	// "[�̷��� ���� Ȯ��
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		break;
		// end 2010. 02. 11 by ckPark �ߵ��� ����������

		// 2010. 03. 23 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���Ǵ�Ƽ �ʵ� ���� ĳ��������)
	case DES_INFINITY_REENTRY_TICKET:
		sprintf(str, STRMSG_C_100310_0309);	// "���Ǵ�Ƽ�ʵ� �߰� ����"
		return;
		// end 2010. 03. 23 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���Ǵ�Ƽ �ʵ� ���� ĳ��������)

		// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	case DES_SKILL_MON_SILENCE_PRIMARY:
		strcpy(str, STRMSG_C_100310_0312);	// "[�⺻���� ���Ұ�]"
		return;
	case DES_SKILL_MON_SILENCE_SECOND:
		strcpy(str, STRMSG_C_100310_0313);	// "[��޹��� ���Ұ�]"
		return;
	case DES_SKILL_MON_FREEZE_HP:
		strcpy(str, STRMSG_C_100310_0315);	// "[������ ȸ�� �Ұ�]"
		return;
	case DES_SKILL_MON_FREEZE_DP:
		strcpy(str, STRMSG_C_100310_0316);	// "[�ǵ� ȸ�� �Ұ�]"
		return;
	case DES_SKILL_MON_FREEZE_SP:
		strcpy(str, STRMSG_C_100310_0317);	// "[SP ȸ�� �Ұ�]"
		return;
	case DES_SKILL_MON_HOLD:
		strcpy(str, STRMSG_C_100310_0318);	// "[�̵� �Ұ�]"
		return;
	case DES_SKILL_MON_STEALING:
		sprintf(str, STRMSG_C_100310_0314, fValue);	// "[SP ���:-%.2f]"
		return;
	case DES_SKILL_MON_DRAIN:
		sprintf(str, STRMSG_C_100310_0311, fValue);	// "[HP ���:-%.2f]"
		return;
	case DES_SKILL_MON_SILENCE_SKILL:
		break;
		// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
		// 2010. 05. 18 by jskim �������� ���� ���� ����
	case DES_PLUS_WARPOINT_RATE:
		sprintf(str, STRMSG_C_100518_0401, fValue + 1);	// "[WarPoint :%.2f�� ����]"
		return;
		// 2013-06-17 by ssjung �������� ���� �� �����Ķ� ���� 
	case DES_PLUS_WARPOINT_RATE_FOR_RETURN_USER:
		sprintf(str, STRMSG_C_100518_0401, fValue + 1);	// "[WarPoint :%.2f�� ����]"
		return;
		// end 2013-06-17 by ssjung �������� ���� �� �����Ķ� ���� 
		//end 2010. 05. 18 by jskim �������� ���� ���� ����
		// 2010. 06. 08 by jskim Ȩ�����̾� UI �۾�
	case DES_PCROOM_USE_CARD:
		sprintf(str, STRMSG_C_100610_0401);	// "[Ȩ �����̾� ����]"
		return;
		//end 2010. 06. 08 by jskim Ȩ�����̾� UI �۾�
		// 2010-08-26 by shcho&&jskim, WARPOINT ���� ������ ����
	case DES_WAR_POINT_UP:
		sprintf(str, STRMSG_C_100826_0401, fValue);	// "WARPOINT %d ����"
		return;
	case DES_DONATE_POINT_UP:
		sprintf(str, STRMSG_C_100826_0402, fValue);	// "WARPOINT %d ����"
		return;
		// end 2010-08-26 by shcho&&jskim, WARPOINT ���� ������ ����
		// 2013-02-14 by bhsohn Ư�� �Ӽ� ������ ���� �̻��ϰ� ������ ���� ó��
	case DES_SUMMON_POSITION_X:		// ��ȯ�� ��ġ ���� (��밪)
	case DES_SUMMON_POSITION_Y:		// ��ȯ�� ��ġ ���� (��밪)
	case DES_SUMMON_POSITION_Z:		// ��ȯ�� ��ġ ���� (��밪)	
	case DES_MAPBUFF_RANDOM_ADD_REATTACKTIME:	// �ʹ��� �߰� �ð� ����
	case DES_MAPBUFF_RANDOM_ADD_TIME:			// �ʹ��� �߰� �ð� ����
	case DES_ENCHANT_ONCE_APPLY:	// 2011-10-19 by hskim, EP4 [Free to play] - 10ȸ ��þƮ ������ ��� ����
	case DES_GAMBLE_RARE_ITEM_FIX:	// 2011-10-20 by hskim, EP4 [Free to play] - ����/���� ���� �ɼ�
	case DES_ITEM_BUFF_PARTY:	// 2013-02-19 by bhsohn �̼� ������ ���� ������ ���� �߸� ������ ���� �ذ�
		sprintf(str, "");
		return;
		// END 2013-02-14 by bhsohn Ư�� �Ӽ� ������ ���� �̻��ϰ� ������ ���� ó��
			// 2013-05-15 by bhsohn ��������Ʈ ������ ���� �ý���
#ifdef C_UPGRADE_INFLUENCE_POINT_BHSOHN
	case DES_SKILL_BUFF_MON_ATTACK_POWER:			// ���� ���ݽ� - ���ݷ� ���� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130520_0001);
		nValueType = FUNCTION_VALUE_TYPE_PROB100;
	}
	break;
	case DES_SKILL_BUFF_MON_ATTACK_PROBABILITY:		// ���� ���ݽ� - ���ݷ� Ȯ�� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130520_0002);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_SKILL_BUFF_MON_ATTACK_PIERCE:			// ���� ���ݽ� - �Ǿ ���� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130520_0003);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_SKILL_BUFF_MON_DEFENCE:				// ���� ���� - ���� ���� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130520_0004);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_SKILL_BUFF_MON_DEFENCE_AVOID:			// ���� ���� - ȸ�Ƿ� ���� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130520_0005);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
	}
	break;
	case DES_SKILL_BUFF_PVP_ATTACK_POWER:			// PVP - ���ݷ� ���� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130520_0006);
		// 2013-08-02 by ssjung ������ ���� ����ǥ��
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
	}
	break;
	case DES_SKILL_BUFF_PVP_ATTACK_PROBABILITY:	// PVP - ���߷� ���� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130801_0001);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
	}
	break;
	case DES_SKILL_BUFF_PVP_ATTACK_PIERCE:			// PVP - �Ǿ ���� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130801_0002);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
	}
	break;
	case DES_SKILL_BUFF_PVP_DEFENCE:				// PVP - ���� ���� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130801_0003);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
	}
	break;
	case DES_SKILL_BUFF_PVP_DEFENCE_PROBABILITY:	// PVP - ȸ�Ƿ� ���� : Value ���� %
	{
		wsprintf(buf, STRMSG_C_130801_0004);
		nValueType = FUNCTION_VALUE_TYPE_PROB;
		fValue = TURN_AROUND_BUFF_SKILL_100P_VALUE * g_pD3dApp->GetPVPBuffPercent();
		// end 2013-08-02 by ssjung ������ ���� ����ǥ��
	}
	break;
#endif
	// END 2013-05-15 by bhsohn ��������Ʈ ������ ���� �ý���
	// 2013-03-27 by bhsohn DestParam �ڷ��� ����
	// 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
	case DES_ALL_PART:
	{
		sprintf(str, STRMSG_C_130419_0005, (int)fValue);				//"\\y[��罺��:+%d]\\y"
		return;
	}
	break;
	// end 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��
	default:
	{
		if (0 == fValue)
		{
			// ���� ����. ���� ��Ȳ
			sprintf(str, "");
			DBGOUT("CINFItemInfo::SetParameterInfo FALSE[%d]", bType);
			return;
		}
	}
	break;
	// END2013-03-27 by bhsohn DestParam �ڷ��� ����
	}
	if (fValue > 0)
	{
		switch (nValueType)
		{
		case FUNCTION_VALUE_TYPE_NORMAL:
		{
			// 2010. 02. 11 by ckPark �ߵ��� ����������
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str,"%s:+%.2f]",buf,fValue);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str,"%s:+%.2f\\e[+%.2f]\\g]",buf,fValue, fEnchant);
// 						}
// 						else
// 						{
// 							sprintf(str,"%s:+%.2f\\e[%.2f]\\g]",buf,fValue, fEnchant);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					// 2006-12-14 by dgwoo ��ȿ������ ���� ���Ȱ��� ����ؼ� ���
// 					if(bType == DES_RANGEANGLE_02)
// 					{
// 						sprintf(str,"%s:+%.2f]",buf,fValue/PI*180);
// 					}
// 					else
// 					{
// 						sprintf(str,"%s:+%.2f]",buf,fValue);
// 					}
// 				}

			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant != 0.0f)
			{
				if (fEnchant > 0.0f)
				{
					//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f\\e[+%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d\\e[+%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f\\e[%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d\\e[%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
			}
			else
			{
				if (bType == DES_RANGEANGLE_02)
					sprintf(str, "%s:+%.2f]", buf, fValue / PI * 180);
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f]", buf, fValue);
					else
						sprintf(str, "%s:+%d]", buf, (int)fValue);
					//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				}
			}
			// end 2010. 02. 11 by ckPark �ߵ��� ����������
		}
		break;
		// 2006-04-24 by ispark
		case FUNCTION_VALUE_TYPE_PROB:
		{
			// 2010. 02. 11 by ckPark �ߵ��� ����������
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:+%2.0f%%]",buf,fValue);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[+%2.0f%%]\\g]",buf,fValue, fEnchant);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[%2.0f%%]\\g]",buf,fValue, fEnchant);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:+%2.0f%%]",buf,fValue);
// 				}

			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��

				if (SingofDecimalDecision(fValue))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)fValue);


			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
					//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				}
			}
			// end 2010. 02. 11 by ckPark �ߵ��� ����������
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB100:
		{
			// 2010. 02. 11 by ckPark �ߵ��� ����������
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:+%2.0f%%]",buf,fValue*100);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[+%2.0f%%]\\g]",buf,fValue*100, fEnchant*100);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[%2.0f%%]\\g]",buf,fValue*100, fEnchant*100);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:+%2.0f%%]",buf,fValue*100);
// 				}

			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				if (SingofDecimalDecision(fValue * 100))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue * 100);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)(fValue * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				else
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
			}
			// end 2010. 02. 11 by ckPark �ߵ��� ����������
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB255:
		{
			// 2010. 02. 11 by ckPark �ߵ��� ����������
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:+%.2f%%]",buf,((fValue/255)*100));
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:+%.2f%%\\e[+%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:+%.2f%%\\e[%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:+%.2f%%]",buf,(fValue/255)*100);
// 				}

			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				if (SingofDecimalDecision((fValue / 255) * 100))
					sprintf(str, "%s:+%.2f%%]", buf, (fValue / 255) * 100);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)((fValue / 255) * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:+%.2f%%\\e[+%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:+%d%%\\e[+%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
				}
				else
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:+%.2f%%\\e[%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else

						sprintf(str, "%s:+%d%%\\e[%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
					//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��	
				}
			}
			// end 2010. 02. 11 by ckPark �ߵ��� ����������
		}
		break;
		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
		case FUNCTION_VALUE_TYPE_PERCENT:
		{
			//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
			if (SingofDecimalDecision(fValue*100.0f))
				sprintf(str, "%s:+%.2f%%]", buf, (fValue*100.0f));
			else
				sprintf(str, "%s:+%d%%]", buf, (int)(fValue*100.0f));
			//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
		}
		break;
		// end 2008-09-26 by bhsohn �ű� ��þƮ ó��
		}
	}
	else if (fValue < 0)
	{
		switch (nValueType)
		{
		case FUNCTION_VALUE_TYPE_NORMAL:
		{
			// 2010. 02. 11 by ckPark �ߵ��� ����������
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str,"%s:\\r%.2f\\g]",buf,fValue);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str,"%s:\\r%.2f\\e[+%.2f]\\g]",buf,fValue, fEnchant);
// 						}
// 						else
// 						{
// 							sprintf(str,"%s:\\r%.2f\\e[%.2f]\\g]",buf,fValue, fEnchant);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str,"%s:\\r%.2f\\g]",buf,fValue);
// 				}

			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				if (SingofDecimalDecision(fValue))
					sprintf(str, "%s:\\r%.2f\\g]", buf, fValue);
				else
					sprintf(str, "%s:\\r%d\\g]", buf, (int)fValue);
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:\\r%.2f\\e[+%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:\\r%d\\e[+%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:\\r%.2f\\e[%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:\\r%d\\e[%.2f]\\g]", buf, (int)fValue, fEnchant);
					//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				}
			}
			// end 2010. 02. 11 by ckPark �ߵ��� ����������
		}
		break;
		// 2006-04-24 by ispark
		case FUNCTION_VALUE_TYPE_PROB:
		{
			// 2010. 02. 11 by ckPark �ߵ��� ����������
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:+%2.0f%%]",buf,fValue);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[+%2.0f%%]\\g]",buf,fValue, fEnchant);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:+%2.0f%%\\e[%2.0f%%]\\g]",buf,fValue, fEnchant);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:+%2.0f%%]",buf,fValue);
// 				}

			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				if (SingofDecimalDecision(fValue))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)fValue);
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
				}
				//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
			}
			// end 2010. 02. 11 by ckPark �ߵ��� ����������
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB100:
		{
			// 2010. 02. 11 by ckPark �ߵ��� ����������
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:\\r%2.0f%%\\g]",buf,fValue*100);
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:\\r%2.0f%%\\e[+%2.0f%%]\\g]",buf,fValue*100, fEnchant*100);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:\\r%2.0f%%\\e[%2.0f%%]\\g]",buf,fValue*100, fEnchant*100);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:\\r%2.0f%%\\g]",buf,fValue*100);
// 				}

			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				if (SingofDecimalDecision(fValue * 100))
					sprintf(str, "%s:\\r%2.2f%%\\g]", buf, fValue * 100);
				else
					sprintf(str, "%s:\\r%d%%\\g]", buf, (int)(fValue * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:\\r%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:\\r%d%%\\e[+%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				else
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:\\r%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:\\r%d%%\\e[%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
			}
			// end 2010. 02. 11 by ckPark �ߵ��� ����������
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB255:
		{
			// 2010. 02. 11 by ckPark �ߵ��� ����������
// 				if( m_pRefEnchant != NULL )
// 				{
// 					float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 					if( fEnchant == 0)
// 					{
// 						sprintf(str, "%s:\\r%.2f%%\\g]",buf,((fValue/255)*100));
// 					}
// 					else
// 					{
// 						if( fEnchant > 0)
// 						{
// 							sprintf(str, "%s:\\r%.2f%%\\e[+%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 						}
// 						else
// 						{
// 							sprintf(str, "%s:\\r%.2f%%\\e[%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 						}
// 					}
// 				}
// 				else
// 				{
// 					sprintf(str, "%s:\\r%.2f%%\\g]",buf,((fValue/255)*100));
// 				}

			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				if (SingofDecimalDecision((fValue / 255) * 100))
					sprintf(str, "%s:\\r%.2f%%\\g]", buf, ((fValue / 255) * 100));
				else
					sprintf(str, "%s:\\r%d%%\\g]", buf, (int)((fValue / 255) * 100));

			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:\\r%.2f%%\\e[+%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:\\r%d%%\\e[+%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
				}
				else
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:\\r%.2f%%\\e[%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:\\r%d%%\\e[%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
				}
				//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
			}
			// end 2010. 02. 11 by ckPark �ߵ��� ����������
		}
		break;
		}
		//		if(bPercent)
		//		{
		//			sprintf(str, "%s:\\r%d%%\\g]",buf,(int)(fValue*100));
		//		}
		//		else
		//		{
		//			sprintf(str,"%s:\\r%.2f\\g]",buf,fValue);
		//		}
	}

	// 2010. 02. 11 by ckPark �ߵ��� ����������

// 	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
// 	else if(0 ==fValue 
// 		&& ((DES_DEFENSE_01 == bType)
// 		    ||(DES_DEFENSE_02 == bType)
// 			||(DES_DEFENSEPROBABILITY_01 == bType)
// 			||(DES_DEFENSEPROBABILITY_02 == bType)))
// 	{
// 		if( m_pRefEnchant != NULL )
// 		{
// 			if( m_pRefEnchant != NULL )
// 			{
// 				float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, bType );
// 				if( fEnchant == 0)
// 				{
// 					sprintf(str, "%s:+%.2f%%]",buf,((fValue/255)*100));
// 				}
// 				else
// 				{
// 					if( fEnchant > 0)
// 					{
// 						sprintf(str, "%s:+%.2f%%\\e[+%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 					}
// 					else
// 					{
// 						sprintf(str, "%s:+%.2f%%\\e[%.2f%%]\\g]",buf,(fValue/255)*100, (fEnchant/255)*100);
// 					}
// 				}
// 			}
// 			else
// 			{
// 				sprintf(str, "%s:+%.2f%%]",buf,(fValue/255)*100);
// 			}
// 		}
// 		else
// 		{
// 			wsprintf(str, "%s]", buf);
// 		}
// 	}
// 	// end 2008-09-26 by bhsohn �ű� ��þƮ ó��
// 	else
// 	{
// 		wsprintf(str, "%s]", buf);
// 	}


	else if (fValue == 0.0f)		// ��æƮ�� ���� ���� �ο��� ���
	{
		switch (nValueType)
		{
		case FUNCTION_VALUE_TYPE_NORMAL:
		{
			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant != 0.0f)
			{
				if (fEnchant > 0.0f)
				{
					//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f\\e[+%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d\\e[+%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f\\e[%.2f]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d\\e[%.2f]\\g]", buf, (int)fValue, fEnchant);
				}
			}
			else
			{
				if (bType == DES_RANGEANGLE_02)
					sprintf(str, "%s:+%.2f]", buf, fValue / PI * 180);
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%.2f]", buf, fValue);
					else
						sprintf(str, "%s:+%d]", buf, (int)fValue);
				}
				//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
			}
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB:
		{
			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				if (SingofDecimalDecision(fValue))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)fValue);
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)fValue, fEnchant);

				}
				else
				{
					if (SingofDecimalDecision(fValue))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue, fEnchant);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)fValue, fEnchant);
					//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				}
			}
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB100:
		{
			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				if (SingofDecimalDecision(fValue * 100))
					sprintf(str, "%s:+%2.2f%%]", buf, fValue * 100);
				else
					sprintf(str, "%s:+%d%%]", buf, (int)(fValue * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:+%2.2f%%\\e[+%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:+%d%%\\e[+%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
				}
				else
				{
					if (SingofDecimalDecision(fValue * 100))
						sprintf(str, "%s:+%2.2f%%\\e[%2.2f%%]\\g]", buf, fValue * 100, fEnchant * 100);
					else
						sprintf(str, "%s:+%d%%\\e[%2.2f%%]\\g]", buf, (int)(fValue * 100), fEnchant * 100);
					//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				}
			}
		}
		break;
		case FUNCTION_VALUE_TYPE_PROB255:
		{
			float fEnchant = 0.0f;
			// �⺻ ��æƮ�����Ķ��� CParamFactor����
			if (bDefEnchant)
			{
				if (m_pRefEnchant != NULL)
					fEnchant = GetParamFactor_DesParam(*m_pRefEnchant, bType);
			}
			else
			{
				// �ߵ��� �����Ķ��� �ߵ��� ��æƮ ��������
				if (m_pRefItemInfo)
					fEnchant = m_pRefItemInfo->GetInvokeEnchantParamValue(bType);
			}

			if (fEnchant == 0.0f)
			{
				//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				if (SingofDecimalDecision((fValue / 255) * 100))
					sprintf(str, "%s:+%.2f%%]", buf, ((fValue / 255) * 100));
				else
					sprintf(str, "%s:+%d%%]", buf, (int)((fValue / 255) * 100));
			}
			else
			{
				if (fEnchant > 0.0f)
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:+%.2f%%\\e[+%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:+%d%%\\e[+%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
				}
				else
				{
					if (SingofDecimalDecision((fValue / 255) * 100))
						sprintf(str, "%s:+%.2f%%\\e[%.2f%%]\\g]", buf, (fValue / 255) * 100, (fEnchant / 255) * 100);
					else
						sprintf(str, "%s:+%d%%\\e[%.2f%%]\\g]", buf, (int)((fValue / 255) * 100), (fEnchant / 255) * 100);
					//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
				}
			}
		}
		break;
		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
		case FUNCTION_VALUE_TYPE_PERCENT:
		{
			if (SingofDecimalDecision(fValue*100.0f))
				sprintf(str, "%s:+%.2f%%]", buf, (fValue*100.0f));
			else
				sprintf(str, "%s:+%d%%]", buf, (int)(fValue*100.0f));
		}
		break;
		}
	}
	// end 2010. 02. 11 by ckPark �ߵ��� ����������
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::IsEnableItem(ITEM* pITEM)
/// \brief		
/// \author		dhkwon
/// \date		2004-06-12 ~ 2004-06-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFItemInfo::IsEnableItem(ITEM* pITEM)
{
	if (pITEM->ReqGearStat.AttackPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.AttackPart &&
		pITEM->ReqGearStat.ShieldPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.ShieldPart &&
		pITEM->ReqGearStat.DodgePart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DodgePart &&
		pITEM->ReqGearStat.DefensePart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.DefensePart &&
		pITEM->ReqGearStat.FuelPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.FuelPart &&
		pITEM->ReqMinLevel <= g_pShuttleChild->m_myShuttleInfo.Level &&
		pITEM->ReqMaterial <= g_pShuttleChild->m_myShuttleInfo.Material &&
		CompareBitFlag(pITEM->ReqRace, g_pShuttleChild->m_myShuttleInfo.Race) &&
		pITEM->ReqGearStat.SoulPart <= g_pShuttleChild->m_myShuttleInfo.TotalGearStat.SoulPart &&
		pITEM->ReqSP <= g_pShuttleChild->m_myShuttleInfo.SP &&
		CompareBitFlag(pITEM->ReqUnitKind, g_pShuttleChild->m_myShuttleInfo.UnitKind))
	{
		if (pITEM->ReqMaxLevel == 0 ||
			pITEM->ReqMaxLevel >= g_pShuttleChild->m_myShuttleInfo.Level)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::LoadNPCImage(int nNPCIndex)
/// \brief		
/// \author		ydkim
/// \date		2004-08-13~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CINFImageEx* CINFItemInfo::FindBigIcon(int nItemNum)
{
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	char szName[32];
	wsprintf(szName, "%08d", nItemNum);
	DataHeader*	pDataHeader = g_pInterface->m_pGameBigIconData->FindFromFile(szName);
	if (pDataHeader == NULL)
	{
		return NULL;
	}

	CINFImageEx *pImage = new CINFImageEx;
	pImage->InitDeviceObjects(pDataHeader);
	pImage->RestoreDeviceObjects();
#else
	util::del(m_pDataHeader);
	CGameData gameData;
	gameData.SetFile(".\\Res-Tex\\bigitem.tex", FALSE, NULL, 0, FALSE);
	char szName[32];
	wsprintf(szName, "%08d", nItemNum);
	m_pDataHeader = gameData.FindFromFile(szName);
	if (m_pDataHeader == NULL)
	{
		return NULL;
	}

	CINFImageEx *pImage = new CINFImageEx;
	pImage->InitDeviceObjects(m_pDataHeader);
	pImage->RestoreDeviceObjects();
#endif


	return pImage;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::RenderInfoWindows(int x, int y, int cx, int cy)
/// \brief		
/// \author		ydkim
/// \date		2004-09-01~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::RenderInfoWindows(int x, int y, int cx, int cy)
{
	m_pInfoBoxSide[0]->Move(x, y);
	m_pInfoBoxSide[0]->Render();
	m_pInfoBoxSide[1]->Move(x + ITEMINFO_SIDE_TOPSIZE_WIDTH, y);
	m_pInfoBoxSide[1]->SetScale(cx - (2 * ITEMINFO_SIDE_TOPSIZE_WIDTH), 1);
	m_pInfoBoxSide[1]->Render();
	m_pInfoBoxSide[2]->Move(x + cx - ITEMINFO_SIDE_TOPSIZE_WIDTH, y);
	m_pInfoBoxSide[2]->Render();

	m_pInfoBoxSide[3]->Move(x, y + ITEMINFO_SIDE_TOPSIZE_HEIGHT);
	m_pInfoBoxSide[3]->SetScale(1, cy - (ITEMINFO_SIDE_TOPSIZE_HEIGHT + ITEMINFO_SIDE_BOTSIZE_HEIGHT));
	m_pInfoBoxSide[3]->Render();
	m_pInfoBoxSide[4]->Move(x + ITEMINFO_SIDE_TOPSIZE_WIDTH, y + ITEMINFO_SIDE_TOPSIZE_HEIGHT);
	m_pInfoBoxSide[4]->SetScale(cx - (2 * ITEMINFO_SIDE_TOPSIZE_WIDTH), cy - (ITEMINFO_SIDE_TOPSIZE_HEIGHT + ITEMINFO_SIDE_BOTSIZE_HEIGHT));
	m_pInfoBoxSide[4]->Render();
	m_pInfoBoxSide[5]->Move(x + cx - ITEMINFO_SIDE_TOPSIZE_WIDTH, y + ITEMINFO_SIDE_TOPSIZE_HEIGHT);
	m_pInfoBoxSide[5]->SetScale(1, cy - (ITEMINFO_SIDE_TOPSIZE_HEIGHT + ITEMINFO_SIDE_BOTSIZE_HEIGHT));
	m_pInfoBoxSide[5]->Render();

	m_pInfoBoxSide[6]->Move(x, y + cy - ITEMINFO_SIDE_BOTSIZE_HEIGHT);
	m_pInfoBoxSide[6]->Render();
	m_pInfoBoxSide[7]->Move(x + ITEMINFO_SIDE_BOTSIZE_WIDTH, y + cy - ITEMINFO_SIDE_BOTSIZE_HEIGHT);
	m_pInfoBoxSide[7]->SetScale(cx - (2 * ITEMINFO_SIDE_BOTSIZE_WIDTH), 1);
	m_pInfoBoxSide[7]->Render();
	m_pInfoBoxSide[8]->Move(x + cx - ITEMINFO_SIDE_BOTSIZE_WIDTH, y + cy - ITEMINFO_SIDE_BOTSIZE_HEIGHT);
	m_pInfoBoxSide[8]->Render();

}

DataHeader * CINFItemInfo::FindResource(char* szRcName)
{
	FLOG("CINFSelect::FindResource(char* szRcName)");
	DataHeader* pHeader = NULL;
	if (m_pGameData)
	{
		pHeader = m_pGameData->Find(szRcName);
	}
	return pHeader;
}

void CINFItemInfo::SetGameData(CGameData * pData)
{
	FLOG("CINFBase::SetGameData(CGameData * pData)");
	m_pGameData = pData;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetRadarInfo()
/// \brief		���̴� �����Է�
/// \author		ispark
/// \date		2005-08-17 ~ 2005-08-17
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetRadarItemInfo(BOOL bShop, BOOL bInven)											   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetUnitKind(index++);				// 2005-12-26 by ispark, SetCount() -> SetUnitKind()�� ����
	SetReqLevel(index++);				// 2007-06-14 by dgwoo ���̴ٵ� �䱸 ������ ��.
	SetWeight(index++);

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	if (m_pRefItemInfo)
		SetShapeInfo(index++);
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	SetPrimaryRange(index++);
	SetSecondaryRange(index++);
	SetRadarRange(index++);
	if (bShop == FALSE)
	{
		SetShopSellInfo(index++);
		// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetSkillReAttackTime(int nParameterIndex)
/// \brief		��ߵ� �ð� 
/// \author		ispark
/// \date		2005-11-22 ~ 2005-11-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetSkillReAttackTime(int nParameterIndex)
{
	// 2005-11-22 by ispark ��ߵ� �ð� : �߰�
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051122_0007, (float)m_pRefITEM->ReAttacktime / 1000.0f);//"��ߵ� �ð� : %.2fsec"
//	if(fEnchantTime > 0)
//		sprintf( m_strItemInfo[nParameterIndex], "%s\\e[+%2.0f%%]\\e",m_strItemInfo[nParameterIndex], fEnchantTime*100) ;
//	else if(fEnchantTime < 0)
//		sprintf( m_strItemInfo[nParameterIndex], "%s\\e[%2.0f%%]\\e",m_strItemInfo[nParameterIndex], fEnchantTime*100) ;
//	if(fRareInfoTime > 0)
//		sprintf( m_strItemInfo[nParameterIndex], "%s\\g[+%2.0f%%]\\g",m_strItemInfo[nParameterIndex], fRareInfoTime*100) ;
//	else if(fRareInfoTime < 0)
//		sprintf( m_strItemInfo[nParameterIndex], "%s\\g[%2.0f%%]\\g",m_strItemInfo[nParameterIndex], fRareInfoTime*100) ;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SecondaryWeaponSpeed(int nParameterIndex)
/// \brief		���� �ӵ�
/// \author		ispark
/// \date		2005-12-15 ~ 2005-12-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetWeaponSpeed(int nParameterIndex)
{
	FLOAT fEnchantSpeed = m_pRefEnchant ? (m_pRefEnchant->pfm_WARHEAD_SPEED * 100) : 0;
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051215_0100, m_pRefITEM->RepeatTime);//"�ӵ� : %dm/s"
	if (fEnchantSpeed > 0)
	{
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%2.0f%%]\\e", m_strItemInfo[nParameterIndex], fEnchantSpeed);
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetWeaponAngle(int nParameterIndex)
/// \brief		���� �ִ� ��ȸ��
/// \author		ispark
/// \date		2005-12-15 ~ 2005-12-15
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetWeaponAngle(int nParameterIndex)
{
	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_051215_0101, m_pRefITEM->BoosterAngle);//"��ȸ�� : %.0f��"
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetAccessoryTimeLimitItemInfo(BOOL bShop)
/// \brief		�Ǽ��縮
/// \author		ispark
/// \date		2006-03-30 ~ 2006-03-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetAccessoryTimeLimitItemInfo(BOOL bShop, BOOL bInven)					   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	//SetItemAllTime(index++);
	//SetItemRemainTime(index++,bShop);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++);
	}
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	SetParameter(&index);

	SetWeight(index++);
	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																				// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetItemRemainTime(int nParameterIndex, BOOL bLinkItem)
/// \brief		������ ��� ���� �ð�
/// \author		ispark
/// \date		2006-03-31 ~ 2006-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemRemainTime(int nParameterIndex, BOOL bShop, BOOL bLinkItem, BOOL bSetTick)
{
	int nTime = 0;
	int nItemTime = m_pRefITEM->Time;
	if (bLinkItem == TRUE && m_pRefITEM->LinkItem)
	{
		ITEM* pItemInfo = g_pDatabase->GetServerItemInfo(m_pRefITEM->LinkItem);
		nItemTime = pItemInfo->Time;
	}
	// 2010. 05. 18 by jskim �������� ���� ���� ����
	if (nItemTime >= MAX_INT_VALUE)
	{
		return;
	}
	//end 2010. 05. 18 by jskim �������� ���� ���� ����
	// 2008-12-02 by bhsohn �ű� ������ �Ӽ� �߰�
	if ((nItemTime > 0.0f)
		&& m_pRefItemInfo
		&& m_pRefItemInfo->ItemInfo
		&& COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
	{
		char strRemain[1024] = { 0, };
		int nSecTime = (int)(nItemTime);
		if (!bShop && m_pRefItemInfo)
		{
			nSecTime = (int)((nItemTime)-m_pRefItemInfo->GetItemPassTime());
		}

		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061012_0002, GetStringDateTimeFormSecond(strRemain, nSecTime));

		if (bSetTick)
		{
			stTickFuntionIndex stAddTemp;
			stAddTemp.nFuntionIndex = FUNCTION_INDEX_ITEM_REMAIN_TIME;
			stAddTemp.nDataLineIndex = nParameterIndex;
			stAddTemp.nParam1 = bLinkItem;
			m_vecTickFuntionIndex.push_back(stAddTemp);
		}
	}
	// end 2008-12-02 by bhsohn �ű� ������ �Ӽ� �߰�
	else if (nItemTime > 0.0f)

	{
		char strRemain[1024] = { 0, };
		int nSecTime = (int)(nItemTime / 1000);
		if (!bShop && m_pRefItemInfo)
		{
			nSecTime = (int)((nItemTime / 1000) - m_pRefItemInfo->GetItemPassTime());
		}

		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061012_0002, GetStringDateTimeFormSecond(strRemain, nSecTime));

		if (bSetTick)
		{
			stTickFuntionIndex stAddTemp;
			stAddTemp.nFuntionIndex = FUNCTION_INDEX_ITEM_REMAIN_TIME;
			stAddTemp.nDataLineIndex = nParameterIndex;
			stAddTemp.nParam1 = bLinkItem;
			m_vecTickFuntionIndex.push_back(stAddTemp);
		}
	}
	else
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0006); // "�����ð� : ������"
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetItemAllTime(int nParameterIndex, BOOL bCard, BOOL bLinkItem)
/// \brief		�ð� ���� ������ �� �ð�
/// \author		ispark
/// \date		2006-03-31 ~ 2006-03-31
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemAllTime(int nParameterIndex, BOOL bCard, BOOL bLinkItem)
{
	int nTime = 0;
	if (bCard)
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0004, nTime); // "�ð� : ���"
	}
	else
	{
		int nItemTime = m_pRefITEM->Time;
		if (bLinkItem == TRUE && m_pRefITEM->LinkItem)
		{
			ITEM* pItemInfo = g_pDatabase->GetServerItemInfo(m_pRefITEM->LinkItem);
			if (pItemInfo == NULL)
				return;
			nItemTime = pItemInfo->Time;
		}

		if (nItemTime > 0.0f)
		{
			if (nItemTime >= 60000)
			{
				nTime = (int)(nItemTime / 60000);
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060331_0002, nTime); // "�ð� : %d��"
			}
			else
			{
				nTime = (int)(nItemTime / 1000);
				wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061011_0002, nTime); // "�ð� : %d��"			
			}
		}
		else
		{
			wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0005, nTime); // "�ð� : ����"
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetItemDelAllTime(int nParameterIndex)
/// \brief		������ ���� �� �ð�
/// \author		ispark
/// \date		2006-10-11 ~ 2006-10-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemDelAllTime(int nParameterIndex)
{
	// 2006-10-11 by ispark, �ڵ� ���� �ð�
	if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
	{
		int nItemTime = m_pRefItemInfo->ItemInfo->Endurance;

		int nTime = (int)(nItemTime / 60000);
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060331_0002, nTime); // "�ð� : %d��"
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetItemDelRemainTime(int nParameterIndex)
/// \brief		������ ���� ���� �ð�
/// \author		ispark
/// \date		2006-10-11 ~ 2006-10-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemDelRemainTime(int nParameterIndex, BOOL bSetTick)
{
	// 2009-01-12 by bhsohn ������ ���� �߸� �Ǿ� ���� ����
	memset(m_strItemInfo[nParameterIndex], 0x00, ITEMINFO_ITEM_FULL_NAME);
	// end 2009-01-12 by bhsohn ������ ���� �߸� �Ǿ� ���� ����


	// 2009. 09. 15 by ckPark �ð����� ������ ���� ���� ����
	if (!m_pRefItemInfo)
		return;
	// end 2009. 09. 15 by ckPark �ð����� ������ ���� ���� ����


	// 2006-10-11 by ispark, �ڵ� ���� ���� �ð�
	if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
	{
		char strDelRemain[1024] = { 0, };
		ATUM_DATE_TIME curRemainTime;
		ATUM_DATE_TIME curServerTime = GetServerDateTime();
		int nRemainSecond = ((int)m_pRefItemInfo->ItemInfo->Endurance * 3600) - (curServerTime.GetTimeInSeconds() - m_pRefItemInfo->CreatedTime.GetTimeInSeconds());

		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_061011_0003, GetStringDateTimeFormSecond(strDelRemain, nRemainSecond));

		if (bSetTick)
		{
			stTickFuntionIndex stAddTemp;
			stAddTemp.nFuntionIndex = FUNCTION_INDEX_ITEM_DEL_REMAIN_TIME;
			stAddTemp.nDataLineIndex = nParameterIndex;
			m_vecTickFuntionIndex.push_back(stAddTemp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetInfluenceBuffItemInfo(BOOL bShop)
/// \brief		
/// \author		ispark
/// \date		2006-04-25 ~ 2006-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetInfluenceBuffItemInfo(BOOL bShop, BOOL bInven)											// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	// 2008-01-05 by bhsohn �̼� ���� ������
	//SetExclusiveUser( index++ );
	if (m_pRefITEM
		&& (ITEMKIND_INFLUENCE_BUFF == m_pRefITEM->Kind)
		// 2009-04-21 by bhsohn ������ DesParam�߰�
		//&& (DES_ITEM_BUFF_PARTY == m_pRefITEM->DestParameter1)) // ������ ������
		&& (DES_ITEM_BUFF_PARTY == m_pRefITEM->ArrDestParameter[0])) // ������ ������
	{
	}
	else
	{
		SetExclusiveUser(index++);
	}
	// end 2008-01-05 by bhsohn �̼� ���� ������
	SetCount(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	//SetItemAllTime(index++, FALSE, TRUE);
	//SetItemRemainTime(index++, bShop, TRUE);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++, FALSE, TRUE);
	}
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	// ��ũ ������ ���
	SetParameter(&index, TRUE);

	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																	   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetInfluenceGameEventItemInfo(BOOL bShop)
/// \brief		
/// \author		ispark
/// \date		2006-04-25 ~ 2006-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetInfluenceGameEventItemInfo(BOOL bShop, BOOL bInven)						   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetUnitKind(index++);
	// 2008-01-05 by bhsohn �̼� ���� ������
	//SetExclusiveUser( index++ );
	if (m_pRefITEM
		&& (ITEMKIND_INFLUENCE_BUFF == m_pRefITEM->Kind)
		// 2009-04-21 by bhsohn ������ DesParam�߰�
		//&& (DES_ITEM_BUFF_PARTY == m_pRefITEM->DestParameter1)) // ������ ������)
		&& (DES_ITEM_BUFF_PARTY == m_pRefITEM->ArrDestParameter[0])) // ������ ������)
	{
	}
	else
	{
		SetExclusiveUser(index++);
	}
	// end 2008-01-05 by bhsohn �̼� ���� ������
	SetCount(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
	//SetItemAllTime(index++);
	//SetItemRemainTime(index++,bShop);

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++, TRUE);
	}
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	// ����
	wsprintf(m_strItemInfo[index++], STRMSG_C_ITEM_0055, STRMSG_C_060424_0019); //"EXP, SPI, ������ ����� �����ð� ���"

	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																				 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			VOID CINFItemInfo::SetRandomBoxItemInfo(BOOL bShop)
/// \brief		�����ڽ� ���� ����.
/// \author		dgwoo
/// \date		2006-08-11 ~ 2006-08-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetRandomBoxItemInfo(BOOL bShop, BOOL bInven)								 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);

	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
// 	// 2009. 09. 15 by ckPark �ð����� ������ ���� ���� ����
// 	
// 	//SetItemDelRemainTime(index++);			// 2006-10-11 by ispark
// 	if( m_pRefItemInfo )
// 		SetItemDelRemainTime(index++);
// 	
// 	// end 2009. 09. 15 by ckPark �ð����� ������ ���� ���� ����

	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	// 2010. 02. 10 by ckPark �����ڽ������ۿ� ���� ���� �߰�, ų��ũ ������������ ����
	SetReqLevel(index++);
	// end 2010. 02. 10 by ckPark �����ڽ������ۿ� ���� ���� �߰�, ų��ũ ������������ ����

	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																				  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			VOID CINFItemInfo::SetMarkItemInfo(BOOL bShop)
/// \brief		��ũ ������
/// \author		ispark
/// \date		2006-08-21 ~ 2006-08-21
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetMarkItemInfo(BOOL bShop, BOOL bInven)											 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);
	SetWeight(index++);

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	if (m_pRefItemInfo)
		SetShapeInfo(index++);
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	// 2007-09-07 by bhsohn �ð��� ��ũ ������ �߰�


	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
// 	// 2009. 09. 15 by ckPark �ð����� ������ ���� ���� ����
// 	
// 	//SetItemDelRemainTime(index++);
// 	if( m_pRefItemInfo )
// 		SetItemDelRemainTime(index++);
// 	
// 	// end 2009. 09. 15 by ckPark �ð����� ������ ���� ���� ����

	SetRemainTime_Imp(m_pRefITEM, &index, bShop); // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}
	SetParameter(&index);
	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																			   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetExclusiveUser(int nParameterIndex)
/// \brief		������
/// \author		ispark
/// \date		2006-04-25 ~ 2006-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetExclusiveUser(int nParameterIndex)
{
	switch (m_pRefITEM->Kind)
	{
	case ITEMKIND_INFLUENCE_BUFF:
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0016, STRMSG_C_060424_0017);
	}
	break;
	case ITEMKIND_INFLUENCE_GAMEEVENT:
	{
		wsprintf(m_strItemInfo[nParameterIndex], STRMSG_C_060424_0016, STRMSG_C_060424_0017);
	}
	break;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			CINFItemInfo::SetParameter(int* index, BOOL bLinkItem)
/// \brief		������ ���
/// \author		ispark
/// \date		2006-04-25 ~ 2006-04-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
//void CINFItemInfo::SetParameter(int* index, BOOL bLinkItem, BOOL bArmorItem/*= FALSE*/)
void CINFItemInfo::SetParameter(int* index, BOOL bLinkItem, BOOL bArmorItem/*= FALSE*/, float RareValue/*= 0.0f*/)
//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
{
	// ����
	if (bLinkItem == TRUE)
	{
		// ��ũ������ ���
		// 2013-07-05 by ssjung �������� ũ���� ���� ����ó��
		if (m_pRefItemInfo)
			ITEM* pItemInfo = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->ItemInfo->LinkItem);
		else
			ITEM* pItemInfo = g_pDatabase->GetServerItemInfo(m_pRefITEM->LinkItem);
		// end 2013-07-05 by ssjung �������� ũ���� ���� ����ó��

		// 2009-04-21 by bhsohn ������ DesParam�߰�
// 		if(pItemInfo->DestParameter1 != 0)
// 			SetFunction( (*index)++, pItemInfo->DestParameter1, pItemInfo->ParameterValue1, 0,0);
// 		if(pItemInfo->DestParameter2 != 0)
// 			SetFunction( (*index)++, pItemInfo->DestParameter2, pItemInfo->ParameterValue2, 0,0);
// 		if(pItemInfo->DestParameter3 != 0)
// 			SetFunction( (*index)++, pItemInfo->DestParameter3, pItemInfo->ParameterValue3, 0,0);
// 		if(pItemInfo->DestParameter4 != 0)
// 			SetFunction( (*index)++, pItemInfo->DestParameter4, pItemInfo->ParameterValue4, 0,0);
		int nArrParamCnt = 0;
		for (nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
// 			if(m_pRefITEM->ArrDestParameter[nArrParamCnt]!= 0)
// 			{
			// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
// 				if(!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
// 					continue;
			//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
// 				SetFunction( (*index)++, m_pRefITEM->ArrDestParameter[nArrParamCnt], 
// 							m_pRefITEM->ArrParameterValue[nArrParamCnt], 0,0);
			float tempValue = 0.0f;
			if (m_pRefItemInfo)
				tempValue = GetRareParameterValue(m_pRefItemInfo, nArrParamCnt);
			if (m_pRefITEM->ArrDestParameter[nArrParamCnt] != 0)
			{
				// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
				if (!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
					continue;
				//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
				// 2011-10-20 by jhahn	��æƮ ���� 	
				if ((int)m_pRefITEM->ArrDestParameter[nArrParamCnt] == DES_ENCHANT_ONCE_APPLY)
					continue;
				//end 2011-10-20 by jhahn	��æƮ ���� 	
				// 2013-02-19 by bhsohn �̼� ������ ���� ������ ���� �߸� ������ ���� �ذ�
				if (!IsShowFunction(m_pRefITEM->ArrDestParameter[nArrParamCnt]))
				{
					continue;
				}
				// END 2013-02-19 by bhsohn �̼� ������ ���� ������ ���� �߸� ������ ���� �ذ�
				SetFunction((*index)++, m_pRefITEM->ArrDestParameter[nArrParamCnt],
					m_pRefITEM->ArrParameterValue[nArrParamCnt], 0, 0, FUNCTIONTYPE_NORMAL, tempValue);
			}
		}
		if (m_pRefItemInfo)
			DefferentFunction(index, m_pRefItemInfo);
		//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
		// end 2009-04-21 by bhsohn ������ DesParam�߰�

	}
	else
	{
		// 2009-04-21 by bhsohn ������ DesParam�߰�
		// 		if(m_pRefITEM->DestParameter1 != 0)
		// 			SetFunction( (*index)++, m_pRefITEM->DestParameter1, m_pRefITEM->ParameterValue1, 0,0);
		// 		if(m_pRefITEM->DestParameter2 != 0)
		// 			SetFunction( (*index)++, m_pRefITEM->DestParameter2, m_pRefITEM->ParameterValue2, 0,0);
		// 		if(m_pRefITEM->DestParameter3 != 0)
		// 			SetFunction( (*index)++, m_pRefITEM->DestParameter3, m_pRefITEM->ParameterValue3, 0,0);
		// 		if(m_pRefITEM->DestParameter4 != 0)
		// 			SetFunction( (*index)++, m_pRefITEM->DestParameter4, m_pRefITEM->ParameterValue4, 0,0);
		int nArrParamCnt = 0;
		for (nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{
			// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
			// 			if(m_pRefITEM->ArrDestParameter[nArrParamCnt]!= 0)
			// 			{
			// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
			// 				if(!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
			// 					continue;
			//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
			// 				SetFunction( (*index)++, m_pRefITEM->ArrDestParameter[nArrParamCnt], 
			// 										m_pRefITEM->ArrParameterValue[nArrParamCnt], 0,0);
			float tempValue = 0.0f;
			tempValue = GetRareParameterValue(m_pRefItemInfo, nArrParamCnt);
			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			if (m_pRefITEM->ArrDestParameter[nArrParamCnt] != 0 &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOKIT_HP &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOKIT_SHIELD &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOKIT_SP &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOSKILL_AGEAR &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOSKILL_BGEAR &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOSKILL_IGEAR &&
				m_pRefITEM->ArrDestParameter[nArrParamCnt] != DES_PET_SLOT_ITEM_AUTOSKILL_MGEAR)

				//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
			{
				// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
				if (!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
					continue;
				//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
				// 2011-10-20 by jhahn	��æƮ ���� 
				if ((int)m_pRefITEM->ArrDestParameter[nArrParamCnt] == DES_ENCHANT_ONCE_APPLY || (int)m_pRefITEM->ArrDestParameter[nArrParamCnt] == DES_GAMBLE_RARE_ITEM_FIX)
					continue;
				//end 2011-10-20 by jhahn	��æƮ ���� 
				// 2013-03-27 by bhsohn DestParam �ڷ��� ����
				if (!IsShowFunction(m_pRefITEM->ArrDestParameter[nArrParamCnt]))
				{
					continue;
				}
				// END 2013-03-27 by bhsohn DestParam �ڷ��� ����
				SetFunction((*index)++, m_pRefITEM->ArrDestParameter[nArrParamCnt],
					m_pRefITEM->ArrParameterValue[nArrParamCnt], 0, 0, FUNCTIONTYPE_NORMAL, tempValue);
			}
		}
		// 2013-04-19 by ssjung ���� ���� Ŷ ��ȭ �ý���
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
		if (m_pRefItemInfo)
		{
			// 2013-06-17 by ssjung �Ƹ� �÷��� �Ʒ������� ���� ���ܻ��� �߰� 
			if (0 < m_pRefItemInfo->FixedTermShape.nStatLevel)
				//			if((0 < m_pRefItemInfo->FixedTermShape.nStatLevel && !m_pRefItemInfo->FixedTermShape.bIsEnd) || 10 <= m_pRefItemInfo->FixedTermShape.nStatLevel)
			{
				if (m_pRefItemInfo->FixedTermShape.bActive || ARENA_STATE_ARENA_GAMING == g_pD3dApp->GetArenaState() || SHAPE_STAT_FIXED_TIME_LIMITE_LEVEL <= m_pRefItemInfo->FixedTermShape.nStatLevel)
					SetLookChangeSystem(index, m_pRefItemInfo->FixedTermShape.nStatShapeItemNum);	//2013-05-20 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ǥ��
			}
			//end 2013-06-17 by ssjung �Ƹ� �÷��� �Ʒ������� ���� ���ܻ��� �߰� 
		}
#endif
		// end 2013-04-19 by ssjung ���� ���� Ŷ ��ȭ �ý���

		DefferentFunction(index, m_pRefItemInfo);
		//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
		// end 2009-04-21 by bhsohn ������ DesParam�߰� 
	}

	// 2010. 02. 11 by ckPark �ߵ��� ����������

// 	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
// 	if(bArmorItem && m_pRefEnchant)
// 	{
// 		// �Ƹ� �������� ��þ�� ������ �ִ�.
// 		int nArmorCheck[4] =
// 		{
// 			DES_DEFENSE_01,
// 			DES_DEFENSE_02,
// 			DES_DEFENSEPROBABILITY_01,
// 			DES_DEFENSEPROBABILITY_02
// 		};		
// 		int nCnt = 0;		
// 		for(nCnt = 0;nCnt < 4;nCnt++)
// 		{
// 			float fEnchant = GetParamFactor_DesParam( *m_pRefEnchant, nArmorCheck[nCnt] );
// 			if( fEnchant <= 0)
// 			{
// 				// ��þƮ ���� �ִ�üũ
// 				continue;
// 			}
// 
// 			// 2009-04-21 by bhsohn ������ DesParam�߰�
// // 			if((m_pRefITEM->DestParameter1 != nArmorCheck[nCnt])
// // 				&& (m_pRefITEM->DestParameter2 != nArmorCheck[nCnt])
// // 				&& (m_pRefITEM->DestParameter3 != nArmorCheck[nCnt])
// // 				&& (m_pRefITEM->DestParameter4 != nArmorCheck[nCnt]))
// // 			{
// // 				SetFunction( (*index)++, nArmorCheck[nCnt], 0, 0,0);
// // 			}			
// 			int nArrParamCnt = 0;
// 			BOOL bDesParam = TRUE;
// 			for(nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
// 			{
// 				bDesParam &= (m_pRefITEM->ArrDestParameter[nArrParamCnt] != nArmorCheck[nCnt]);	
// 			}
// 			if(bDesParam)				
// 			{
// 				// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
// 				if(!(int)m_pRefITEM->ArrParameterValue[nArrParamCnt] && m_pRefITEM->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
// 					continue;
// 				//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
// 				SetFunction( (*index)++, nArmorCheck[nCnt], 0, 0,0);
// 			}			
// 			// end 2009-04-21 by bhsohn ������ DesParam�߰�
// 		}
// 		
// 
// 	}
// 	// end 2008-09-26 by bhsohn �ű� ��þƮ ó��

	if (m_pRefItemInfo)
	{
		std::vector<DestParam_t>	vecAdditionalFunctionList;		// ��æƮ�� ���� ���� �ο��� ��ɸ���Ʈ
		std::vector<ENCHANT_PARAM>	vecDefEnchant = *(m_pRefItemInfo->GetDefEnchantParamList());
		std::vector<ENCHANT_PARAM>::iterator it = vecDefEnchant.begin();
		while (it != vecDefEnchant.end())
		{
			// ���� ������ ����� �ƴϰ� ���� �ο��� ����� ��� ������ �߰��Ѵ�
			// 2013-02-14 by bhsohn Ư�� �Ӽ� ������ ���� �̻��ϰ� ������ ���� ó��
// 			if( !m_pRefItemInfo->ItemInfo->IsExistDesParam( (*it).m_nDesParam )
// 				&& vecAdditionalFunctionList.end() == std::find( vecAdditionalFunctionList.begin(), vecAdditionalFunctionList.end(), (*it).m_nDesParam ) )
			if (!m_pRefItemInfo->ItemInfo->IsExistDesParam((*it).m_nDesParam)
				&& IsShowFunction((*it).m_nDesParam)		// ������ �������� ���� �Ǵ�
				&& vecAdditionalFunctionList.end() == std::find(vecAdditionalFunctionList.begin(), vecAdditionalFunctionList.end(), (*it).m_nDesParam))

			{
				SetFunction((*index)++, (*it).m_nDesParam, 0.0f, 0, 0.0f);
				// ������ ��ɿ��� ��æƮ���� ������ ����� ����
				vecAdditionalFunctionList.push_back((*it).m_nDesParam);
			}

			++it;
		}
	}

	// end 2010. 02. 11 by ckPark �ߵ��� ����������
}
// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
//int	 CINFItemInfo::GetRareParameterValue(CItemInfo* pRefItemInfo, int num)
float CINFItemInfo::GetRareParameterValue(CItemInfo* pRefItemInfo, int num)
{
	float tempValue = 0.0f;
	for (int i = 0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		if (pRefItemInfo && pRefItemInfo->GetPrefixRareInfo())
		{
			if (m_pRefITEM->ArrDestParameter[num] == pRefItemInfo->GetPrefixRareInfo()->DesParameter[i])
				tempValue += m_pRefItemInfo->GetPrefixRareInfo()->ParameterValue[i];
		}
		if (m_pRefItemInfo && m_pRefItemInfo->GetSuffixRareInfo())
		{
			if (m_pRefITEM->ArrDestParameter[num] == m_pRefItemInfo->GetSuffixRareInfo()->DesParameter[i])
				tempValue += m_pRefItemInfo->GetSuffixRareInfo()->ParameterValue[i];
		}
	}
	return tempValue;
}

void CINFItemInfo::DefferentFunction(int* index, CItemInfo* pRefItemInfo)
{
	// 2013-03-27 by bhsohn DestParam �ڷ��� ����
//	vector<BYTE> tempDesParameter;
	vector<DestParam_t> tempDesParameter;

	for (int i = 0; i < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; i++)
	{
		BOOL tempPreCmp = FALSE;
		BOOL tempSufCmp = FALSE;
		for (int j = 0; j < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; j++)
		{
			if (pRefItemInfo && pRefItemInfo->GetPrefixRareInfo())
			{
				if (pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] == m_pRefITEM->ArrDestParameter[j] &&
					pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] != 0)
				{
					tempPreCmp = TRUE;
				}
			}
			if (pRefItemInfo && pRefItemInfo->GetSuffixRareInfo())
			{
				if (pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] == m_pRefITEM->ArrDestParameter[j] &&
					pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] != 0)
				{
					tempSufCmp = TRUE;
				}
			}
			if (pRefItemInfo && pRefItemInfo->GetPrefixRareInfo() && pRefItemInfo->GetSuffixRareInfo() &&
				pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] != NULL &&
				pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] != NULL)
			{
				if (pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] == pRefItemInfo->GetSuffixRareInfo()->DesParameter[j])
				{
					tempDesParameter.push_back(pRefItemInfo->GetPrefixRareInfo()->DesParameter[i]);
				}
			}
		}

		float tempCmp = 0.0f;
		if (!tempPreCmp)
		{
			if (pRefItemInfo && pRefItemInfo->GetPrefixRareInfo())
			{
				if ((!pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] == NULL || !pRefItemInfo->GetPrefixRareInfo()->ParameterValue[i] == NULL))
				{
					for (int j = 0; j < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; j++)
					{
						if (pRefItemInfo->GetPrefixRareInfo() && pRefItemInfo->GetSuffixRareInfo())
						{
							if (pRefItemInfo->GetPrefixRareInfo()->DesParameter[i] == pRefItemInfo->GetSuffixRareInfo()->DesParameter[j])
							{
								tempCmp += pRefItemInfo->GetSuffixRareInfo()->ParameterValue[j];
							}
						}
					}
					SetFunction((*index)++, pRefItemInfo->GetPrefixRareInfo()->DesParameter[i],
						pRefItemInfo->GetPrefixRareInfo()->ParameterValue[i] + tempCmp, 0, 0);
				}
			}
		}
		if (!tempSufCmp)
		{
			if (pRefItemInfo && pRefItemInfo->GetSuffixRareInfo())
			{
				if (!pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] == NULL || !pRefItemInfo->GetSuffixRareInfo()->ParameterValue[i] == NULL)
				{
					BOOL tempcmp1 = false;
					for (int j = 0; j < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; j++)
					{
						if (pRefItemInfo->GetSuffixRareInfo() && pRefItemInfo->GetPrefixRareInfo())
						{
							if (pRefItemInfo->GetSuffixRareInfo()->DesParameter[i] == pRefItemInfo->GetPrefixRareInfo()->DesParameter[j])
							{
								tempcmp1 = true;
								break;
							}
						}
					}
					if (!tempcmp1)
					{
						SetFunction((*index)++, pRefItemInfo->GetSuffixRareInfo()->DesParameter[i],
							pRefItemInfo->GetSuffixRareInfo()->ParameterValue[i], 0, 0);
					}
				}
			}
		}
	}
}
//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetSkillSupportItem(BOOL bShop)
/// \brief		��ų ���� ������
/// \author		ispark
/// \date		2006-10-02 ~ 2006-10-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetSkillSupportItem(BOOL bShop, BOOL bInven)												 // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int index = 0;
	SetItemName(index++);
	SetItemKind(index++);
	SetReqLevel(index++);


	// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
//	SetItemAllTime(index++);			// 2006-10-11 by ispark
// 	// 2009. 09. 15 by ckPark �ð����� ������ ���� ���� ����
// 	
// 	//SetItemDelRemainTime(index++);
// 	if( m_pRefItemInfo )
// 		SetItemDelRemainTime(index++);
// 	
// 	// end 2009. 09. 15 by ckPark �ð����� ������ ���� ���� ����

	if (!SetRemainTime_Imp(m_pRefITEM, &index, bShop)) // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
	{
		if (m_pRefITEM)
			SetItemAllTime(index++);
	}
	// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����


	// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
	if (FALSE == bShop)
	{
		SetItemAttribute(index++);
	}

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	SetUseAreaInfinity(&index);
	SetExchangeMaterial(bShop);
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	SetDesc(index);
	SetItemExtendInfo(bInven, FALSE);																   // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(index <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();
}


// 2010-06-15 by shcho&hslee ��ý���
/********************************************************************************
**
**	�� ������ ���� ���� ����.
**
**	Create Info :	2010. 06. 24 by hsLee.
**
*********************************************************************************/
void CINFItemInfo::SetPetItemInfo(BOOL bShop /*= FALSE*/, BOOL bInven)						// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
{
	int iIndex = 0;

	SetItemName(iIndex++);
	SetItemKind(iIndex++);
	// 2012-11-29 by bhsohn ��Ʈ�� ���� ���� ��Ʈ�� ����
#ifdef	SC_PARTNER_SHAPE_CHANGE_HSKIM
	if (m_pRefItemInfo)
	{
		SetShapeInfo(iIndex++);
	}
#endif
	// END 2012-11-29 by bhsohn ��Ʈ�� ���� ���� ��Ʈ�� ����
	SetReqLevel(iIndex++);
	SetPetType(iIndex++);

	SetPetEnableLevelUp(iIndex++);

	if (!bShop)
	{
		//SetPetReName ( iIndex++ );		
		SetPetExp(iIndex++);
	}

	// 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����
	if (m_pRefITEM
		&& !m_pRefITEM->IsExistDesParam(DES_FIELD_STORE)
		&& !m_pRefITEM->IsExistDesParam(DES_INCREASE_INVENTORY_SPACE)
		&& !m_pRefITEM->IsExistDesParam(DES_INCREASE_STORE_SPACE))
	{
		SetParameter(&iIndex);
	}
	// end 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����

	if (FALSE == bShop)
		SetItemAttribute(iIndex++);

	SetUseAreaInfinity(&iIndex);

	SetDesc(iIndex);
	SetItemExtendInfo(bInven, FALSE);																  // 2013-06-26 by ssjung �κ��丮 �߰� ���� 
	ASSERT_ASSERT(iIndex <= ITEMINFO_PARAMETER_NUMBER);
	SetMaxLength();


}
// End 2010-06-15 by shcho&hslee ��ý���


///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetOtherFuntionTick()
/// \brief		������ �Լ��� Tick�� ���� ó��
/// \author		ispark
/// \date		2006-10-12 ~ 2006-10-12
/// \warning	
///
/// \param		FALSE�� ������ ����.
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetOtherFuntionTick()
{
	vector<stTickFuntionIndex>::iterator itFuntionIndex = m_vecTickFuntionIndex.begin();

	while (m_vecTickFuntionIndex.end() != itFuntionIndex)
	{
		switch (itFuntionIndex->nFuntionIndex)
		{
		case FUNCTION_INDEX_ITEM_REMAIN_TIME:
		{
			SetItemRemainTime(itFuntionIndex->nDataLineIndex, FALSE, itFuntionIndex->nParam1, FALSE);
		}
		break;
		case FUNCTION_INDEX_ITEM_DEL_REMAIN_TIME:
		{
			SetItemDelRemainTime(itFuntionIndex->nDataLineIndex, FALSE);
		}
		break;
		// 2010. 02. 11 by ckPark �ߵ��� ����������
		case FUNCTION_INDEX_ITEM_COOL_TIME:
		{
			SetItemCoolTime(&((*itFuntionIndex).nDataLineIndex), FALSE);
		}
		break;
		// end 2010. 02. 11 by ckPark �ߵ��� ����������
		}

		itFuntionIndex++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CINFItemInfo::SetItemAttribute( int nParameterIndex)
/// \brief		
/// \author		// 2007-09-07 by bhsohn ������ �ŷ����� ǥ��
/// \date		2007-09-07 ~ 2007-09-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemAttribute(int nParameterIndex)
{
	if (NULL == m_pRefItemInfo)
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0203);//"�ŷ�����, ��������, â����"
		return;
	}

	// 2011-06-07 by shcho, ��Ʈ�� ��û â���� ���� �Ұ� �Ӽ� ����
	if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_NOT_STORE_SAVE))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_110608_0001);// "\\r�ŷ��Ұ�\\r, \\r�����Ұ�\\r, \\râ��Ұ�\\r"
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_NO_TRANSFER))
		// end 2011-06-07 by shcho, ��Ʈ�� ��û â���� ���� �Ұ� �Ӽ� ����
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0201);// "\\r�ŷ��Ұ�\\r, \\r�����Ұ�\\r, \\râ��Ұ�\\r"
	}
	// 2015-06-13 Future, new Trade Option (only Trash allowed)
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_TRASH_ONLY))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0205);
	}
#ifdef NEWSTRING_ITEMINFO
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_WAREHOUSE_SHARE_BANNED))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0206);
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0206);
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_BAZAAR_ITEM))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0207);
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_LEGEND_ITEM))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0208);
	}
	else if (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_UNIQUE_ITEM))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0208);
	}
#endif
	// 2007.09.19 by bhsohn ���������� ǥ�� �߰� ��ȹ�� ó��
	else if ((COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_KILL_MARK_ITEM))
		|| (COMPARE_BIT_FLAG(m_pRefItemInfo->ItemInfo->ItemAttribute, ITEM_ATTR_ACCOUNT_POSSESSION)))
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0202);//"\\r�ŷ��Ұ�\\r, ��������, â����"
	}
	else
	{
		sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070907_0204, STRMSG_C_070907_0203);//"�ŷ�����, ��������, â����"
	}

}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2008-04-14 by bhsohn ���� ������ ���� ��Ʈ������ ó��
/// \date		2008-04-14 ~ 2008-04-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CINFItemInfo::GetItemStringLen()
{
	int i = 0;
	int nMaxLen, nLen;
	nMaxLen = nLen = 0;
	//for(int i=0;i<ITEMINFO_PARAMETER_NUMBER;i++)
	for (i = 1; i < m_nDescIndex; i++)
	{
		if (NULL == m_strItemInfo[i][0])
		{
			continue;
		}
		nLen = m_pFontItemInfo[i]->GetStringSize(m_strItemInfo[i]).cx;
		if (nMaxLen < nLen)
		{
			nMaxLen = nLen;
		}
	}
	return nMaxLen;
}



///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// �Ǿ ��
/// \author		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetPrimaryPierce(int nParameterIndex)
{
	char chTmp[ITEMINFO_ITEM_FULL_NAME];
	memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);

	float fWeaponPierce = 0.0f;
	if (NULL != m_pRefItemInfo)
	{
		ITEM* pRealItem = m_pRefItemInfo->GetRealItemInfo();
		if (pRealItem)
		{
			fWeaponPierce = pRealItem->FractionResistance;
		}

	}
	else if (NULL != m_pRefITEM)
	{
		fWeaponPierce = m_pRefITEM->FractionResistance;
	}
	if (fWeaponPierce > 0)
	{
		// 2008-03-19 by bhsohn FLOAT�� ������ �Ͽ� ���	
		fWeaponPierce = FloatSecRangeSharp(fWeaponPierce);
	}
	else
	{
		fWeaponPierce = 0.0f;
	}
	FLOAT fEhchantPierce = m_pRefEnchant ? (m_pRefEnchant->pfm_PIERCE_UP_01) : 0;

	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070116_0100, fWeaponPierce);//"�Ǿ�� : %.2f%%"	

	// 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��
	float fPreSuffixPierce = GetEnchantPreSuffixInfo(DES_PIERCE_UP_01);
	if (fPreSuffixPierce > 0)
	{	
		auto fPreSuffixPierceRate = FloatSecRangeSharp(fPreSuffixPierce);

		sprintf(chTmp, STRMSG_C_080923_0200, fPreSuffixPierceRate);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%s]\\g", m_strItemInfo[nParameterIndex], chTmp);//"�Ǿ�� : %.2f%%"			
	}
	// end 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��

	if (fEhchantPierce > 0)
	{	
		auto fEhchantPierceRate = FloatSecRangeSharp(fEhchantPierce);

		sprintf(chTmp, STRMSG_C_080923_0200, fEhchantPierceRate);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//"�Ǿ�� : %.2f%%"	
	}


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// �Ǿ ��
/// \author		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetSecondaryPierce(int nParameterIndex)
{
	char chTmp[ITEMINFO_ITEM_FULL_NAME];
	memset(chTmp, 0x00, ITEMINFO_ITEM_FULL_NAME);

	float fWeaponPierce = 0.0f;
	if (NULL != m_pRefItemInfo)
	{
		ITEM* pRealItem = m_pRefItemInfo->GetRealItemInfo();
		if (pRealItem)
		{
			fWeaponPierce = pRealItem->FractionResistance;
		}

	}
	else if (NULL != m_pRefITEM)
	{
		fWeaponPierce = m_pRefITEM->FractionResistance;
	}
	if (fWeaponPierce > 0) fWeaponPierce = FloatSecRangeSharp(fWeaponPierce);
	else fWeaponPierce = 0.0f;


	auto fEhchantPierce = m_pRefEnchant ? (m_pRefEnchant->pfm_PIERCE_UP_02) : 0.0f;

	sprintf(m_strItemInfo[nParameterIndex], STRMSG_C_070116_0100, fWeaponPierce);//"�Ǿ�� : %.2f%%"	

	// 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��
	float fPreSuffixPierce = GetEnchantPreSuffixInfo(DES_PIERCE_UP_02);
	if (fPreSuffixPierce > 0)
	{
		float fPreSuffixPierceRate = fPreSuffixPierce;
		fPreSuffixPierceRate = FloatSecRangeSharp(fPreSuffixPierceRate);

		sprintf(chTmp, STRMSG_C_080923_0200, fPreSuffixPierceRate);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\g[+%s]\\g", m_strItemInfo[nParameterIndex], chTmp);//"�Ǿ�� : %.2f%%"			
	}
	// end 2009-02-16 by bhsohn ����, ���� �Ǿ �� �Ⱥ��̴� ���� ó��


	if (fEhchantPierce > 0)
	{						
		auto fEhchantPierceRate = FloatSecRangeSharp(fEhchantPierce);

		sprintf(chTmp, STRMSG_C_080923_0200, fEhchantPierceRate);
		sprintf(m_strItemInfo[nParameterIndex], "%s\\e[+%s]\\e", m_strItemInfo[nParameterIndex], chTmp);//"�Ǿ�� : %.2f%%"			
	}

}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// �Ǿ ��
/// \author		// 2008-09-26 by bhsohn �ű� ��þƮ ó��
/// \date		2008-09-26 ~ 2008-09-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CINFItemInfo::GetEnchantPreSuffixInfo(int nDesParameter)
{
	float fRareInfoReAttacktime = 0;
	float fEnchantInfo = 0.0f;
	if (m_pRefPrefixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefPrefixRareInfo->DesParameter[i] == nDesParameter)
			{
				fEnchantInfo += m_pRefPrefixRareInfo->ParameterValue[i];
			}
		}
	}
	if (m_pRefSuffixRareInfo)
	{
		for (int i = 0; i < SIZE_DES_PARAM_PER_RARE_ITEM_INFO; i++)
		{
			if (m_pRefSuffixRareInfo->DesParameter[i] == nDesParameter)
			{
				fEnchantInfo += m_pRefSuffixRareInfo->ParameterValue[i];
			}
		}
	}
	return fEnchantInfo;
}

// 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����
BOOL CINFItemInfo::SetRemainTime_Imp(ITEM* pRefItem, int* pIndex, BOOL bShop) // 2013-07-05 by ssjung �������� ũ���� ���� (���ڰ��� CItemInfo*���� ITEM*���� �ٲ�)
{
	BOOL ret = FALSE;

	//if(pRefItem && pRefItem->ItemInfo)
	if (pRefItem)
	{
		//	if(COMPARE_BIT_FLAG(pRefItem->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
		if (COMPARE_BIT_FLAG(pRefItem->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE_AFTER_USED))
		{	// ��� �� �Ⱓ���� ������
			SetItemRemainTime((*pIndex)++, bShop);
			ret = TRUE;
		}
		//else if(COMPARE_BIT_FLAG(pRefItem->ItemInfo->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
		else if (COMPARE_BIT_FLAG(pRefItem->ItemAttribute, ITEM_ATTR_TIME_LIMITE)
			|| pRefItem->Kind == ITEMKIND_ACCESSORY_TIMELIMIT)
		{	// ��� �� �ð����� ������
			SetItemRemainTime((*pIndex)++, bShop);
			ret = TRUE;
		}

		// ���� �� �Ⱓ���� ������			   
		//if(COMPARE_BIT_FLAG(pRefItem->ItemInfo->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		if (COMPARE_BIT_FLAG(pRefItem->ItemAttribute, ITEM_ATTR_DELETED_TIME_LIMITE))
		{
			SetItemDelRemainTime((*pIndex)++);
			ret = TRUE;
		}
	}
	return FALSE;
}
// end 2009. 10. 28 by ckPark �ð�/�Ⱓ ���� ������ ���� ����

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn ���� ������ �� ����
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int	CINFItemInfo::GetMaxLength()
{
	return m_nMaxLength;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn ���� ������ �� ����
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetMyEquipItem(BOOL bMyEquipItem)
{
	m_bMyEquipItem = bMyEquipItem;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn ���� ������ �� ����
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CItemInfo*	CINFItemInfo::GetRefItemInfo()
{
	return m_pRefItemInfo;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn ���� ������ �� ����
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
POINT CINFItemInfo::GetItemInfoPos()
{
	POINT ptItemInfoPos = m_ptItemInfo;
	int icongab;
	if (m_pBigIcon)
	{
		icongab = ITEMINFO_BIGICON_GAB;
	}
	else
	{
		icongab = 0;
	}
	ptItemInfoPos.y -= icongab;

	return ptItemInfoPos;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn ���� ������ �� ����
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemInfoPos(POINT i_ptItemInfoPos)
{
	int icongab;
	if (m_pBigIcon)
	{
		icongab = ITEMINFO_BIGICON_GAB;
	}
	else
	{
		icongab = 0;
	}
	i_ptItemInfoPos.y += icongab;

	m_ptItemInfo = i_ptItemInfoPos;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn ���� ������ �� ����
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
SIZE CINFItemInfo::GetItemInfoTooltipSize()
{
	return m_szTooltip;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn ���� ������ �� ����
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CINFItemInfo::SetItemInfoTooltipSize(SIZE	i_szTooltip)
{
	m_szTooltip = i_szTooltip;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2009-02-03 by bhsohn ���� ������ �� ����
/// \date		2009-02-03 ~ 2009-02-03
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFItemInfo::IsShowItemInfo()
{
	return m_bShow;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// ������ �������� ���� �Ǵ�
/// \author		// 2013-02-14 by bhsohn Ư�� �Ӽ� ������ ���� �̻��ϰ� ������ ���� ó��
/// \date		2013-02-14 ~ 2013-02-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CINFItemInfo::IsShowFunction(int nType)
{
	switch (nType)
	{
		// 2013-03-27 by bhsohn DestParam �ڷ��� ����
	case DES_NULL:					// ��� �Ķ���Ͱ� ���� ��� ���			
	case DES_ATTACK_PART:			// ����				
	case DES_DEFENSE_PART:		// ����-->���	
	case DES_FUEL_PART:				// ����		
	case DES_SOUL_PART:				// ����		
	case DES_SHIELD_PART:			// ���-->����		
	case DES_DODGE_PART:			// ȸ��		
	case DES_BODYCONDITION:			// ������(������,���ݸ��)		
	case DES_ENDURANCE_01: 			// ������ 01		
	case DES_ENDURANCE_02:			// ������ 02		
	case DES_CHARGING_01:			// ��ź�� 01		
	case DES_CHARGING_02:			// ��ź�� 02		
	case DES_PROPENSITY:			// ����		
	case DES_HP:					// ��Ʈ����Ʈ		
	case DES_MAX_SP_UP:					// 2010-08-26 by shcho&&jskim, �� ������ ����
	case DES_SP:					// �ҿ�����Ʈ		
	case DES_EP:					// ��������Ʈ		
	case DES_DP:					// ��������Ʈ		
	case DES_SPRECOVERY:			// �ҿ�����Ʈȸ����			C		
	case DES_HPRECOVERY:			// ����������Ʈȸ����		C		
	case DES_MINATTACK_01:				//  ���ݷ� 1��			C		
	case DES_MINATTACK_02:	            //  ���ݷ� 2��			C		
	case DES_MAXATTACK_01:				//  ���ݷ� 1��			C		
	case DES_MAXATTACK_02:	            //  ���ݷ� 2��			C		
	case DES_ATTACKPROBABILITY_01:	// ����Ȯ�� 01				C, PROB256_MAX_VALUE		
	case DES_ATTACKPROBABILITY_02:    // ����Ȯ�� 02				C, PROB256_MAX_VALUE		
	case DES_DEFENSE_01:			// ���� 01				C, PROB256_MAX_VALUE		
	case DES_DEFENSE_02:			// ���� 02				C, PROB256_MAX_VALUE		
	case DES_DEFENSEPROBABILITY_01:	// ���Ȯ�� 01				C, PROB256_MAX_VALUE		
	case DES_DEFENSEPROBABILITY_02:	// ���Ȯ�� 02				C, PROB256_MAX_VALUE		
	case DES_SKILLPROBABILITY_01:		// ���ε� ��Ʈ�Ѱ���Ȯ��				C, PROB256_MAX_VALUE		
	case DES_SKILLPROBABILITY_02:		// ���ε� ��Ʈ�Ѱ���Ȯ��				C, PROB256_MAX_VALUE		
	case DES_FACTIONRESISTANCE_01:		// �Ӽ����׷�				C, PROB256_MAX_VALUE		
	case DES_FACTIONRESISTANCE_02:		// �Ӽ����׷�				C, PROB256_MAX_VALUE		
	case DES_SPEED:					// �̵��ӵ�					C		
	case DES_TRANSPORT:				// ��ݷ�		
	case DES_MATERIAL:				// ����		
	case DES_REATTACKTIME_01:		// (*) ������Ÿ�� 01		C		
	case DES_REATTACKTIME_02:		// (*) ������Ÿ�� 02		C		
	case DES_ABRASIONRATE_01:		// ������ 01				C, PROB256_MAX_VALUE		
	case DES_ABRASIONRATE_02:		// ������ 02				C, PROB256_MAX_VALUE		
	case DES_RANGE_01:				// (*) ��ȿ�Ÿ� 01			C		
	case DES_RANGE_02:				// (*) ��ȿ�Ÿ� 02			C		
	case DES_RANGEANGLE_01:			// ��ȿ���� 01				C		
	case DES_RANGEANGLE_02:			// ��ȿ���� 02				C		
	case DES_MULTITAGET_01:			// ��ƼŸ��					C		
	case DES_MULTITAGET_02:			// ��ƼŸ��					C		
	case DES_EXPLOSIONRANGE_01:		// ���߹ݰ�					C		
	case DES_EXPLOSIONRANGE_02:		// ���߹ݰ�					C		
	case DES_UNIT:					// ������ ���� (28 ~ 29�� ���� ���� ���ָ����� ���������� ����)		
	case DES_REVISION:				// ������ ������ (28 ~ 29�� ���� ���� ���ָ����� ���������� ����)		
	case DES_FACTIONPROBABILITY_01:		// �Ӽ��� ���� ���Ȯ��		C, PROB256_MAX_VALUE
	case DES_FACTIONPROBABILITY_02:		// �Ӽ��� ���� ���Ȯ��		C, PROB256_MAX_VALUE		
	case DES_SHOTNUM_01:				// ������ �� �߻� ��		C		
	case DES_SHOTNUM_02:				// ������ �� �߻� ��		C		
	case DES_MULTINUM_01:				// ���� �߻� ��				C		
	case DES_MULTINUM_02:				// ���� �߻� ��				C		
	case DES_ATTACKTIME_01:			// ó�� ���� ���� Ÿ�� 01	C		
	case DES_ATTACKTIME_02:			// ó�� ���� ���� Ÿ�� 02	C		
	case DES_TIME_01:				// (*)���� �ð� 01				C		
	case DES_TIME_02:				// (*)���� �ð� 02				C		
	case DES_WEIGHT_01:				// (*)���� 			C		
	case DES_WEIGHT_02:				// (*)���� 			C		
	case DES_REACTION_RANGE:
	case DES_OVERHITTIME_01:// (*) ������
	case DES_OVERHITTIME_02:// (*) ������		
	case DES_BAZAAR_SELL:
	case DES_BAZAAR_BUY:
	case DES_UNITKIND:
	case DES_ITEMKIND:				// ����������				
	case DES_GROUNDMODE:			// ���� ���		
	case DES_SIEGEMODE:				// ���� ���		
	case DES_IMMEDIATE_HP_UP:
	case DES_IMMEDIATE_DP_UP:
	case DES_IMMEDIATE_SP_UP:
	case DES_IMMEDIATE_EP_UP:
	case DES_RARE_FIX_PREFIX:
	case DES_RARE_FIX_SUFFIX:
	case DES_RARE_FIX_BOTH:
	case DES_RARE_FIX_PREFIX_INITIALIZE:
	case DES_RARE_FIX_SUFFIX_INITIALIZE:
	case DES_SKILL_REDUCE_SHIELD_DAMAGE:
	case DES_ATTACK_RANGE_01:
	case DES_ATTACK_RANGE_02:
	case DES_SKILL_COLLISIONDAMAGE_DOWN:
	case DES_SKILL_REVERSEENGINE:
	case DES_SKILL_SMARTSP:
	case DES_SKILL_SUMMON_FORMATION_MEMBER:
	case DES_SKILL_REACTIONSPEED:
	case DES_SKILL_ENGINEANGLE:
	case DES_SKILL_ENGINEBOOSTERANGLE:
	case DES_WARP:
	case DES_SKILL_SLOWMOVING:
	case DES_CASH_NORMAL_RESTORE:
	case DES_DROP_EXP:
	case DES_DROP_SPI:
	case DES_DROP_ITEM:
	case DES_HP_REPAIR_RATE_FLIGHTING:
	case DES_DP_REPAIR_RATE:
	case DES_SP_REPAIR_RATE:
	case DES_CASH_STAT_ALL_INITIALIZE:
	case DES_CASH_STAT_PART_INITIALIZE:
	case DES_CASH_CHANGE_CHARACTERNAME:
	case DES_CASH_STEALTH:
	case DES_KILLMARK_EXP:
	case DES_GRADUAL_DP_UP:
	case DES_SKILL_CHAFF_HP:
	case DES_SKILL_HALLUCINATION:
	case DES_SKILL_HYPERSHOT:
	case DES_SKILL_ROLLING_TIME:
	case DES_SKILL_SHIELD_PARALYZE:
	case DES_SKILL_BARRIER:
	case DES_SKILL_BIG_BOOM:
	case DES_INVISIBLE:
	case DES_SKILL_CANCELALL:
	case DES_SKILL_INVINCIBLE:
	case DES_SKILL_FULL_RECOVERY:
	case DES_SKILL_SCANNING:
	case DES_SKILL_NO_WARNING:
	case DES_SKILL_CAMOUFLAGE:
	case DES_WARHEAD_SPEED:
	case DES_WARP_OUTPOST:
	case DES_CHAT_ALL_INFLUENCE:
	case DES_CASH_CHANGE_PILOTFACE:
	case DES_ENCHANT_INITIALIZE:
	case DES_CASH_GUILD:
	case DES_ENGINE_BOOSTER_TIME_UP:	// �ν��� �ð� ����				
	case DES_ENGINE_MAX_SPEED_UP:		// ���� �Ϲݼӵ�(�ִ�) ����				
	case DES_ENGINE_MIN_SPEED_UP:		// ���� �Ϲݼӵ�(�ּ�) ����				
	case DES_ENGINE_BOOSTER_SPEED_UP:		// ���� �ν��ͼӵ� ����				
	case DES_ENGINE_GROUND_SPEED_UP:		// ���� ����ӵ� ����				
	case DES_RADAR_OBJECT_DETECT_RANGE:		// ���̴� ��ü ���� �ݰ�				
	case DES_PIERCE_UP_01:		// �⺻���� �Ǿ�� ���� ī��				
	case DES_PIERCE_UP_02:		// ��޹��� �Ǿ�� ���� ī��			
	case DES_RARE_ITEM_DROP_RATE:
	case DES_RARE_ITEM_PARTNER_SPEED:
	case DES_RARE_ITEM_PARTNER_DAMAGE:
	case DES_RARE_ITEM_HPDP:
	case DES_REQ_MIN_LEVEL:		// ���������� �䱸 MinLevel�� �����
	case DES_REQ_MAX_LEVEL:		// ���������� �䱸 MaxLevel�� �����				
	case DES_ENGINE_ANGLE_UP:	// ���� ȸ���� ���� ī��				
	case DES_ENGINE_BOOSTERANGLE_UP:	// ���� �ν��� ȸ���� ���� ī��				
	case DES_CHAT_BLOCK:	// ������ ä�� ���� ī�� ���� - 				
	case DES_CASH_MONSTER_SUMMON:
	case DES_TRANSFORM_TO_MONSTER:
	case DES_TRANSFORM_TO_GEAR:
	case DES_ITEM_RESISTANCE:
	case DES_ITEM_ADDATTACK:
	case DES_ITEM_ADDATTACK_SEC:
	case DES_ITEM_IGNOREDEFENCE:
	case DES_ITEM_IGNOREAVOID:
	case DES_ITEM_REDUCEDAMAGE:
	case DES_SKILL_RELEASE:
	case DES_PAIR_DRAIN_1_RATE:
	case DES_PAIR_DRAIN_2_HP_DP_UP_RATE:
	case DES_PAIR_REFLECTION_1_RATE:
	case DES_PAIR_REFLECTION_2_DAMAGE_RATE:
	case DES_ANTI_DRAIN_RATE:
	case DES_ANTI_REFLECTION_RATE:
	case DES_INFINITY_REENTRY_TICKET:
	case DES_SKILL_MON_SILENCE_PRIMARY:
	case DES_SKILL_MON_SILENCE_SECOND:
	case DES_SKILL_MON_FREEZE_HP:
	case DES_SKILL_MON_FREEZE_DP:
	case DES_SKILL_MON_FREEZE_SP:
	case DES_SKILL_MON_HOLD:
	case DES_SKILL_MON_STEALING:
	case DES_SKILL_MON_DRAIN:
	case DES_SKILL_MON_SILENCE_SKILL:
	case DES_PLUS_WARPOINT_RATE:
	case DES_PLUS_WARPOINT_RATE_FOR_RETURN_USER:	// 2013-06-17 by ssjung �������� ���� �� �����Ķ� ���� 
	case DES_PCROOM_USE_CARD:
	case DES_WAR_POINT_UP:
	case DES_DONATE_POINT_UP:
		// 2013-05-15 by bhsohn ��������Ʈ ������ ���� �ý���
	case DES_SKILL_BUFF_MON_ATTACK_POWER:			// ���� ���ݽ� - ���ݷ� ���� : Value ���� %
	case DES_SKILL_BUFF_MON_ATTACK_PROBABILITY:		// ���� ���ݽ� - ���ݷ� Ȯ�� : Value ���� %
	case DES_SKILL_BUFF_MON_ATTACK_PIERCE:			// ���� ���ݽ� - �Ǿ ���� : Value ���� %
	case DES_SKILL_BUFF_MON_DEFENCE:				// ���� ���� - ���� ���� : Value ���� %
	case DES_SKILL_BUFF_MON_DEFENCE_AVOID:			// ���� ���� - ȸ�Ƿ� ���� : Value ���� %
	case DES_SKILL_BUFF_PVP_ATTACK_POWER:			// PVP - ���ݷ� ���� : Value ���� %
	// 2013-08-02 by ssjung ������ ���� ����ǥ��
	case DES_SKILL_BUFF_PVP_ATTACK_PROBABILITY:		// PVP - ���߷� ���� : Value ���� %
	case DES_SKILL_BUFF_PVP_ATTACK_PIERCE:			// PVP - �Ǿ ���� : Value ���� %
	case DES_SKILL_BUFF_PVP_DEFENCE:				// PVP - ���� ���� : Value ���� %
	case DES_SKILL_BUFF_PVP_DEFENCE_PROBABILITY:	// PVP - ȸ�Ƿ� ���� : Value ���� %
	// end 2013-08-02 by ssjung ������ ���� ����ǥ��
		// END 2013-05-15 by bhsohn ��������Ʈ ������ ���� �ý���
	{
		return TRUE;
	}
	break;
	// END 2013-03-27 by bhsohn DestParam �ڷ��� ����
	case DES_SUMMON_POSITION_X:		// ��ȯ�� ��ġ ���� (��밪)
	case DES_SUMMON_POSITION_Y:		// ��ȯ�� ��ġ ���� (��밪)
	case DES_SUMMON_POSITION_Z:		// ��ȯ�� ��ġ ���� (��밪)	
	case DES_MAPBUFF_RANDOM_ADD_REATTACKTIME:	// �ʹ��� �߰� �ð� ����
	case DES_MAPBUFF_RANDOM_ADD_TIME:			// �ʹ��� �߰� �ð� ����
	case DES_ENCHANT_ONCE_APPLY:	// 2011-10-19 by hskim, EP4 [Free to play] - 10ȸ ��þƮ ������ ��� ����
	case DES_GAMBLE_RARE_ITEM_FIX:	// 2011-10-20 by hskim, EP4 [Free to play] - ����/���� ���� �ɼ�
	case DES_ITEM_BUFF_PARTY:	// 2013-02-19 by bhsohn �̼� ������ ���� ������ ���� �߸� ������ ���� �ذ�
	{
		return FALSE;
	}
	break;
	default:
	{
		DBGOUT("CINFItemInfo::IsShowFunction FALSE[%d]\n", nType);
		return FALSE;
	}
	break;
	}
	return TRUE;
}

//2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��
BOOL CINFItemInfo::SingofDecimalDecision(float fVal)
{
	int nTemp = fVal * 100;
	nTemp = nTemp % 100;
	if (0 < nTemp) return TRUE;
	else return FALSE;
}
//end 2013-04-11 by ssjung �Ҽ����� �������� �Ҽ��� ǥ��

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 
/// \author		// 2013-05-07 by bhsohn M��� ������ ������ �۾�
/// \date		2013-05-07 ~ 2013-05-07
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
float CINFItemInfo::GetBuffPanaltyInfo(int nParamSize, DestParam_t *i_bType1, float *i_fValue1, USHORT shUnitKind)
{
	float fPanelty = 1.0f;
	USHORT shGear = 0;
	int nCnt = 0;
	for (nCnt = 0; nCnt < nParamSize; nCnt++)
	{
		switch (i_bType1[nCnt])
		{
		case DES_BUFF_PENALTY_RATIO:				// �г�Ƽ ���� (1.0f = 100%)
		{
			fPanelty = i_fValue1[nCnt];
		}
		break;
		case DES_BUFF_PENALTY_APPLIED_UNIT_KIND:	// �г�Ƽ ���� ��� (ReqUnitKind)
		{
			shGear = (USHORT)i_fValue1[nCnt];
		}
		break;
		}
	}

	//�г�Ƽ �Դ� ������ ����� �˷��ش�.
	if (IS_BT(shUnitKind) && IS_BT(shGear))
	{
		// B-GEAR
		return fPanelty;
	}

	if (IS_OT(shUnitKind) && IS_OT(shGear))
	{
		// M-GEAR
		return fPanelty;
	}

	if (IS_DT(shUnitKind) && IS_DT(shGear))
	{
		// A-GEAR
		return fPanelty;

	}

	if (IS_ST(shUnitKind) && IS_ST(shGear))
	{
		// I-GEAR
		return fPanelty;
	}

	return 1.0f;
}

//2013-05-20 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ǥ��   
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
void CINFItemInfo::SetLookChangeSystem(int* pParameterIndex, int nSourceItemNum)
{
	//SetFunction((*pParameterIndex)++, DES_NULL, nStatNum ,0,0,FUNCTIONTYPE_LOOKCHANGE);

	SetArmorCollectionParameter(pParameterIndex, nSourceItemNum);
	if (m_pRefItemInfo && m_pRefItemInfo->nMonthlyOptionItemNum)
		SetMonthlyArmorValue(pParameterIndex, m_pRefItemInfo->atMonthlyEventEndDate, m_pRefItemInfo->nMonthlyOptionItemNum);
	// 	if(m_pRefItemInfo->nMonthlyOptionItemNum)
	// 	{
	// 		ITEM* pLOOKITEM = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->nMonthlyOptionItemNum);
	// 		for(int nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	// 		{
	// 			ATUM_DATE_TIME curServerTime = GetServerDateTime();
	// 			int nRemainSecond = (m_pRefItemInfo->atMonthlyEventEndDate.GetTimeInSeconds() - curServerTime.GetTimeInSeconds() );
	// 			
	// 			if(DES_ITEM_ADDATTACK == pLOOKITEM->ArrDestParameter[nArrParamCnt] || DES_ITEM_ADDATTACK_SEC == pLOOKITEM->ArrDestParameter[nArrParamCnt])
	// 			{
	// 				SetFunction((*pParameterIndex)++, pLOOKITEM->ArrDestParameter[nArrParamCnt], pLOOKITEM->ArrParameterValue[nArrParamCnt], 0,(float)nRemainSecond, FUNCTIONTYPE_TIMELIMINT_ATTACK);
	// 			}
	// 		}
	// 	}
}

//end 2013-05-20 by ssjung �̴��� �Ƹ� - �̴��� �Ƹ� ���� ǥ��

// 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
void CINFItemInfo::CoatingCount(int* nParameterIndex)	//���� Ƚ�� 
{
	struArmorCollectionInfo* pAmorCollectItemData = NULL;

	//	pAmorCollectItemData = 	g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind),m_pRefITEM->ItemNum);
	pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);

	if (pAmorCollectItemData && pAmorCollectItemData->struCollectItemInfo.EnchantLevel)
	{
		wsprintf(m_strItemInfo[(*nParameterIndex)++], STRMSG_C_130610_0107 STRMSG_C_130529_0001, pAmorCollectItemData->struCollectItemInfo.EnchantLevel);	//�������� ���� 
	}
}

void CINFItemInfo::SetArmorCollectionParameter(int* nParameterIndex, int nSourceItemNum)		//���� ��� ���� ���� 
{
	int nParamValue = 1;
	struArmorCollectionInfo* pAmorCollectItemData = NULL;
	ITEM* pTempItem = NULL;
	BOOL bCard = TRUE;					// 2013-06-17 by ssjung �Ƹ� �÷��� ���� ǥ�� �� ī�� �϶� ���ܻ��� �߰�

	pTempItem = m_pRefITEM;

	if (nSourceItemNum)
	{
		if (m_pRefItemInfo)
		{
			pTempItem = g_pDatabase->GetServerItemInfo(m_pRefItemInfo->FixedTermShape.nStatShapeItemNum);
			if (pTempItem)
				pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo(g_pStoreData->Convert_UNITKIND_To_UnitCollectType
				(pTempItem->ReqUnitKind), m_pRefItemInfo->FixedTermShape.nStatShapeItemNum);
			if (pAmorCollectItemData)
				nParamValue = pAmorCollectItemData->struCollectItemInfo.EnchantLevel;

			if (ARENA_STATE_ARENA_GAMING == g_pD3dApp->GetArenaState())					// 2013-06-17 by ssjung �Ƹ� �÷��� �Ʒ������� ���� ���ܻ��� �߰� 
				nParamValue = m_pRefItemInfo->FixedTermShape.nStatLevel;

			bCard = FALSE;			// 2013-06-17 by ssjung �Ƹ� �÷��� ���� ǥ�� �� ī�� �϶� ���ܻ��� �߰�
		}
	}
	else
	{
		//		pAmorCollectItemData = 	g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind),m_pRefITEM->ItemNum);
		if (pTempItem && ITEMKIND_DEFENSE == pTempItem->Kind)
		{
			pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);

			if (pAmorCollectItemData)
			{
				pTempItem = g_pDatabase->GetServerItemInfo(pAmorCollectItemData->struCollectItemInfo.ShapeItemNum);
				nParamValue = pAmorCollectItemData->struCollectItemInfo.EnchantLevel;
				bCard = FALSE;			// 2013-06-17 by ssjung �Ƹ� �÷��� ���� ǥ�� �� ī�� �϶� ���ܻ��� �߰�
			}
			else
				return;
		}
	}

	if (pTempItem)
	{
		SetArmorAbilityValue(nParameterIndex, pTempItem, nParamValue, bCard);			// 2013-06-17 by ssjung ī�� �϶� ���ܻ��� �߰� 
	}

}

void CINFItemInfo::SetMonthlyArmorCollectionValue(int* nParameterIndex)	//�Ƹ� �÷��ǿ��� �̴��� �Ƹ� �Լ� 
{
	struArmorCollectionInfo*						pAmorCollectItemData = NULL;
	MSG_FC_COLLECTION_MONTHLY_AROMOR_EVENT_INFO*	pMonthlyArmorCollectItemData = NULL;

	//	pAmorCollectItemData = 	g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind),m_pRefITEM->ItemNum);
	pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);
	pMonthlyArmorCollectItemData = g_pStoreData->GetMonthlyArmorCollectInfo(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);

	if (pAmorCollectItemData && pMonthlyArmorCollectItemData)
	{
		if (pAmorCollectItemData->struCollectItemInfo.ShapeNum == pMonthlyArmorCollectItemData->CollectionShapeNum) //��Ŷ�� �Ƹ� �÷��� ���� ������ �ѹ��� ������ 
		{
			SetMonthlyArmorValue(nParameterIndex, pMonthlyArmorCollectItemData->atEndDate, pMonthlyArmorCollectItemData->nOptionItemNum); /*����Ǿ� �ִ� ��Ŷ�� enddata, �̺�Ʈ������ �ѹ�*/
		}
	}
}
void CINFItemInfo::SetMonthlyArmorValue(int* nParameterIndex, ATUM_DATE_TIME MonthlyEventEndDate, ItemNum_t MonthlyOptionItemNum) //������ �̴��� �Ƹ� �Լ� 
{
	ITEM* pLOOKITEM = g_pDatabase->GetServerItemInfo(MonthlyOptionItemNum);

	if (!pLOOKITEM)	return;

	for (int nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
	{
		ATUM_DATE_TIME curServerTime = GetServerDateTime();
		int nRemainSecond = (MonthlyEventEndDate.GetTimeInSeconds() - curServerTime.GetTimeInSeconds());

		if (DES_ITEM_ADDATTACK == pLOOKITEM->ArrDestParameter[nArrParamCnt] || DES_ITEM_ADDATTACK_SEC == pLOOKITEM->ArrDestParameter[nArrParamCnt])
		{
			SetFunction((*nParameterIndex)++, pLOOKITEM->ArrDestParameter[nArrParamCnt], pLOOKITEM->ArrParameterValue[nArrParamCnt], 0, (float)nRemainSecond, FUNCTIONTYPE_TIMELIMINT_ATTACK, 0, TRUE);
		}
	}
}
void CINFItemInfo::SetArmorAbilityValue(int* nParameterIndex, ITEM* pTempItem, int nParamValue, BOOL bCard)
{
	float tempValue = 0.0f;
	if (pTempItem
		&& !pTempItem->IsExistDesParam(DES_FIELD_STORE)
		&& !pTempItem->IsExistDesParam(DES_INCREASE_INVENTORY_SPACE)
		&& !pTempItem->IsExistDesParam(DES_INCREASE_STORE_SPACE))
		// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	{


		for (int nArrParamCnt = 0; nArrParamCnt < SIZE_MAX_DESPARAM_COUNT_IN_ITEM; nArrParamCnt++)
		{

			if (pTempItem->ArrDestParameter[nArrParamCnt] != 0)
			{
				// 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
				if (pTempItem->ArrDestParameter[0] == DES_CASH_HP_AND_DP_UP)
					continue;
				//end 2009. 12. 16 by jskim DES_CASH_HP_AND_DP_UP �����ۿ� ���� ���� ó��  
				// 2011-10-20 by jhahn	��æƮ ���� 
				if ((int)pTempItem->ArrDestParameter[nArrParamCnt] == DES_ENCHANT_ONCE_APPLY || (int)pTempItem->ArrDestParameter[nArrParamCnt] == DES_GAMBLE_RARE_ITEM_FIX)
					continue;
				//end 2011-10-20 by jhahn	��æƮ ���� 
				// 2013-03-27 by bhsohn DestParam �ڷ��� ����

				if (pTempItem->ArrDestParameter[nArrParamCnt] == DES_ALL_PART)//DES_ALL_PART
				{
					//	SetFunction( (*nParameterIndex)++, m_pRefITEM->ArrDestParameter[nArrParamCnt]/*�̳� ��*/, m_pRefITEM->ArrParameterValue[nArrParamCnt], 0,0, FUNCTIONTYPE_LOOKCHANGE);	
					if (nParamValue)
						SetFunction((*nParameterIndex)++, DES_ALL_PART, pTempItem->ArrParameterValue[nArrParamCnt] * nParamValue, 0, 0, FUNCTIONTYPE_LOOKCHANGE);
				}
				if (!IsShowFunction(pTempItem->ArrDestParameter[nArrParamCnt]))
				{
					continue;
				}

				if (bCard)
					continue;				// 2013-06-17 by ssjung �Ƹ� �÷��� ���� ǥ�� �� ī�� �϶� ���ܻ��� �߰�

				SetFunction((*nParameterIndex)++, pTempItem->ArrDestParameter[nArrParamCnt],
					pTempItem->ArrParameterValue[nArrParamCnt], 0, 0, FUNCTIONTYPE_NORMAL, tempValue);
			}

		}
	}
}
void CINFItemInfo::SetTimeLimt(int* nParameterIndex)//�Ƹ� �÷��ǿ��� �����ð��� ��Ÿ�� 
{
	char buf[256] = { 0, };
	int nRemainSecond = 0;

	struArmorCollectionInfo* pAmorCollectItemData = NULL;
	//	pAmorCollectItemData = 	g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pStoreData->Convert_UNITKIND_To_UnitCollectType(g_pD3dApp->m_pShuttleChild->m_myShuttleInfo.UnitKind),m_pRefITEM->ItemNum);

	pAmorCollectItemData = g_pStoreData->GetArmorCollectInfo_SourceIndex(g_pGameMain->GetArmorCollectType(), m_pRefITEM->ItemNum);

	if (!pAmorCollectItemData || MAX_ARMOR_COLLECT_ENCHANT <= pAmorCollectItemData->struCollectItemInfo.EnchantLevel || !pAmorCollectItemData->struCollectItemInfo.EnchantLevel)
		return;

	//	if( m_pRefItemInfo->FixedTermShape.bActive)			
	if (pAmorCollectItemData->struCollectItemInfo.RemainSeconds)
	{
		ATUM_DATE_TIME curServerTime = GetServerDateTime();

		if (0 == pAmorCollectItemData->struCollectItemInfo.ActivedCount)
			nRemainSecond = pAmorCollectItemData->struCollectItemInfo.RemainSeconds;
		else
			nRemainSecond = (pAmorCollectItemData->struCollectItemInfo.EndTime.GetTimeInSeconds() - curServerTime.GetTimeInSeconds());

		int m_nDay = (nRemainSecond) / 86400;
		int m_nHour = (nRemainSecond) % 86400 / 3600;
		int m_nMin = (nRemainSecond) % 86400 % 3600 / 60;
		int m_nSec = (nRemainSecond) % 86400 % 3600 % 60 / 1;

		int nStrarttime = nRemainSecond;

		if (0 > nStrarttime)
		{
			wsprintf(buf, STRMSG_C_130610_0106, buf);
			wsprintf(m_strItemInfo[(*nParameterIndex)++], STRMSG_C_130610_0105, buf);
			return;
		}

		if (m_nDay)
		{
			wsprintf(buf, STRMSG_C_130610_0101, buf, m_nDay, m_nHour, m_nMin);
		}
		else if ((m_nDay == NULL) && m_nHour)
		{
			wsprintf(buf, STRMSG_C_130610_0102, buf, m_nHour, m_nMin);
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && m_nMin)
		{
			wsprintf(buf, STRMSG_C_130610_0103, buf, m_nMin);
		}
		else if ((m_nDay == NULL) && (m_nHour == NULL) && (m_nMin == NULL) && m_nSec)
		{
			wsprintf(buf, STRMSG_C_130610_0104, buf);
		}
	}
	else
	{
		wsprintf(buf, STRMSG_C_130610_0106, buf);
	}

	wsprintf(m_strItemInfo[(*nParameterIndex)++], STRMSG_C_130610_0105, buf);
}
// end 2013-06-10 by ssjung �Ƹ� �÷��� ���� ǥ��
#endif
// 2013-06-26 by ssjung �κ��丮 �߰� ���� 
void CINFItemInfo::SetItemExtendInfo(BOOL bShow, BOOL bflag)
{
	if (!bShow)
		return;
#ifdef SC_COLLECTION_ARMOR_JHSEOL_BCKIM
	int nIndex = 0;
	m_nExtendItemIndex = 0;
	wsprintf(m_strExtendItemInfo[nIndex++], " ");
	wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0001);
	if (bflag)
		wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0002);
	else
		wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0003);

	wsprintf(m_strExtendItemInfo[nIndex++], " ");
	wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0004);
	wsprintf(m_strExtendItemInfo[nIndex++], STRMSG_C_130613_0005);

	m_nExtendItemIndex = nIndex;
#endif
}
// end 2013-06-26 by ssjung �κ��丮 �߰� ���� 

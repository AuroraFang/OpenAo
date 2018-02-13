// INFCityOccupy.cpp: implementation of the CINFCityOccupy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "INFCityOccupy.h"
#include "AtumApplication.h"
#include "INFImage.h"
#include "D3DHanFont.h"
#include "GameDataLast.h"
#include "INFGameMain.h"
#include "INFWindow.h"
#include "ShuttleChild.h"
#include "CharacterChild.h"				// 2005-07-21 by ispark
#include "INFCityBase.h"
#include "dxutil.h"

#define OCCUPY_NPC_BOX_FONT_START_X			(CITY_BASE_NPC_BOX_START_X+18)
#define OCCUPY_NPC_BOX_FONT_START_Y			(CITY_BASE_NPC_BOX_START_Y-228)
#define OCCUPY_FONT_LINE_HEIGHT				20							// �� ������ ����
#define OCCUPY_FONT_SELECT_LINE				300

typedef enum 
{
	OCCUPY_TEX			= 1,		// ���� ����
	OCCUPY_DATE_WAR,				// ���� ������ ��¥
//	OCCUPY_USE_STORE,				// ���ܿ��� â�� ���
	OCCUPY_OCCUPY_SHOP,				// ���� ����	
	OCCUPY_SUCCESS_DEFENCE,			// ��� ���� Ƚ��
	OCCUPY_TEX_INFO,				// �������� �� ���� ��������
	OCCUPY_GREATING						// ������ �λ縻
} OCCUPY_LIST;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CINFCityOccupy::CINFCityOccupy(CAtumNode* pParent, BUILDINGNPC* pBuilding)
{
	FLOG( "CINFCityOccupy()" );	
	
	m_pParent					= pParent;
	m_pBuildingInfo				= pBuilding;
//	m_pImgInputBase				= NULL;
	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
		m_pFontMenuList[i] = NULL;
	m_pFontPopup = NULL;
	m_vecIndexName.clear();
	m_vecInfoOccupy.clear();

	memset(strOccupyGuild, 0x00, 128);
	strncpy(strOccupyGuild, "NPC", sizeof("NPC"));
	
	m_nSelectText = 100;
	m_nOccupyGuildMasterUID	= 0;
	m_nOccupyGuildUID = 0;
}
CINFCityOccupy::~CINFCityOccupy()
{
	FLOG( "~CINFCityOccupy()" );
	
//	util::del(m_pImgInputBase);
	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
		util::del(m_pFontMenuList[i]);
	util::del(m_pFontPopup);

}
HRESULT CINFCityOccupy::InitDeviceObjects()
{
	FLOG( "CINFCityOccupy::InitDeviceObjects()" );

//	DataHeader* pDataHeader;

	for(int i=0;i<OCCUPY_MENULIST_COUNT;i++)
	{
		m_pFontMenuList[i] = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,512,32);
		m_pFontMenuList[i]->InitDeviceObjects(g_pD3dDev) ;
	}
	m_pFontPopup = new CD3DHanFont(_T(g_pD3dApp->GetFontStyle()),9, D3DFONT_ZENABLE,  FALSE,512,32);
	m_pFontPopup->InitDeviceObjects(g_pD3dDev) ;

	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0001);//"���� �޴�"
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0002);//"- ���� ����"
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0003);//"- ���� ����� ��¥,�ð� ����"
//	m_vecIndexName.push_back("3. ���ܿ� �� �ݳ��� �̿����");
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0004);	//"- ���� �����Ͽ� �ִ� ���� ����Ʈ"
//	m_vecIndexName.push_back("6. ��NPC�� �������� �̿� �������� �񼱰� ����");
//	m_vecIndexName.push_back("7. ���� ��� NPC��ȯ(���� ����)");
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0005);//"- ����� ���� ȸ��"
//	m_vecIndexName.push_back("9. NPC�� ���� ������ �̿� �������� ���ݼ��� ����");
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0006);//"- ���� ����"
	m_vecIndexName.push_back(STRMSG_C_OCCUPY_0007);//"- �λ縻 ����"

	// vector index ��� ���� ���� �Է� 
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0008);			// index 0//"\\g*���Է½� �ʱⰪ ����*"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0009);										// index 1	OCCUPY_TEX//"[�� �Է�]"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0010);							// index 2	OCCUPY_DATE_WAR//"[00��00��00��00��]"
//	m_vecInfoOccupy.push_back("[���, �̻��]");								// index 3	OCCUPY_USE_STORE
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0011);						// index 4	OCCUPY_OCCUPY_SHOP	//"[���ɵ� ������ �����ϴ�]"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0012);									// index 6	OCCUPY_SUCCESS_DEFENCE//"[XX ȸ ����]"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0013);							// index 7	OCCUPY_TEX_INFO//"���� XXXXXXXXXXX ����"
	m_vecInfoOccupy.push_back(STRMSG_C_OCCUPY_0014);									// index 5	OCCUPY_GREATING//"[�ʱ� �λ縻]"
	
//	pDataHeader = m_pGameData->Find("wocu");
//	m_pImgInputBase = new CINFImage;
//	m_pImgInputBase->InitDeviceObjects( pDataHeader->m_pData, pDataHeader->m_DataSize );

	return S_OK;
}
HRESULT CINFCityOccupy::RestoreDeviceObjects()
{
	FLOG( "CINFCityOccupy::RestoreDeviceObjects()" );

	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
		m_pFontMenuList[i]->RestoreDeviceObjects();
	m_pFontPopup->RestoreDeviceObjects();

//	m_pImgInputBase->RestoreDeviceObjects();

	return S_OK;
}
HRESULT CINFCityOccupy::InvalidateDeviceObjects()
{
	FLOG( "CINFCityOccupy::InvalidateDeviceObjects()" );

	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
		m_pFontMenuList[i]->InvalidateDeviceObjects();
	m_pFontPopup->InvalidateDeviceObjects();

//	m_pImgInputBase->InvalidateDeviceObjects();

	return S_OK;
}
HRESULT CINFCityOccupy::DeleteDeviceObjects()
{
	FLOG( "CINFCityOccupy::DeleteDeviceObjects()" );

	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
	{
		m_pFontMenuList[i]->DeleteDeviceObjects();
		util::del(m_pFontMenuList[i]);
	}
	m_pFontPopup->DeleteDeviceObjects();
	util::del(m_pFontPopup);
	
//	m_pImgInputBase->DeleteDeviceObjects();
//	util::del(m_pImgInputBase);

	return S_OK;
}
void CINFCityOccupy::Render()
{
	FLOG( "CINFCityOccupy::Render()" );
	
//	m_pImgInputBase->Move(CITY_BASE_NPC_BOX_START_X, CITY_BASE_NPC_BOX_START_Y);
//	m_pImgInputBase->Render();

	DWORD dwFontColor = GUI_FONT_COLOR;
	BOOL  bGuildMaster = FALSE;
	if(m_nOccupyGuildMasterUID == g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
		bGuildMaster = TRUE;
	if(!bGuildMaster)
	{
		dwFontColor = RGB(150,150,150);
	}

	char buf[128];
	int ncount = 0;
	
	for(int i=0; i<OCCUPY_MENULIST_COUNT; i++)
	{
		// ��� ������, ���, ��Ÿ�� ���� ����Ʈ ������
		if(m_nOccupyGuildUID == g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber)
		{
			if(bGuildMaster)
			{
				m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
				m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
					(char *)m_vecIndexName[i].c_str(),0L);
			}
			else
			{
				if(i != OCCUPY_GREATING)
				{
					m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
					m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
						(char *)m_vecIndexName[i].c_str(),0L);
				}
			}
		}
		else
		{
			if(i != OCCUPY_GREATING && i != OCCUPY_TEX_INFO)
			{
				m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
				m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
					(char *)m_vecIndexName[i].c_str(),0L);
			}
		}
		
		if(0 == i)
		{
			m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X+70, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
			m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
			strOccupyGuild,0L);
		}

		if( i == OCCUPY_GREATING && m_nSelectText == OCCUPY_GREATING && bGuildMaster)
		{
			g_pGameMain->RenderPopUpWindowImage(OCCUPY_NPC_BOX_FONT_START_X+80,OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*OCCUPY_GREATING)-5,
				strlen((char *)m_vecInfoOccupy[OCCUPY_GREATING].c_str())*7, 1);
			m_pFontPopup->DrawText(OCCUPY_NPC_BOX_FONT_START_X+80, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*OCCUPY_GREATING)-5,
				RGB(255,0,0),
				(char *)m_vecInfoOccupy[m_nSelectText].c_str(),0L);
		}
		else if( i == OCCUPY_OCCUPY_SHOP && m_nSelectText == OCCUPY_OCCUPY_SHOP)
		{			
			vector<int>::iterator itn = vecOccupy.begin();
			while(itn != vecOccupy.end())
			{				
				memset(buf, 0x00, 128);
				sprintf(buf, STRMSG_C_OCCUPY_0015, g_pGameMain->GetMapNameByIndex(*itn));//"���� ���� : %s"
				
				g_pGameMain->RenderPopUpWindowImage(OCCUPY_NPC_BOX_FONT_START_X+80,
					OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*(OCCUPY_OCCUPY_SHOP+ncount))-5,
					strlen(buf)*7, 1);
				
				m_pFontPopup->DrawText(OCCUPY_NPC_BOX_FONT_START_X+80, 
					OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*(OCCUPY_OCCUPY_SHOP+ncount))-5,
					GUI_FONT_COLOR_Y,
					buf,0L);
				
				ncount++;
				itn++;
			}

			// 1���� ���̸��� ����
			vector<int>::iterator it = vecNotOccupy.begin();
			while(it != vecNotOccupy.end())
			{				
				memset(buf, 0x00, 128);
				sprintf(buf, STRMSG_C_OCCUPY_0016, g_pGameMain->GetMapNameByIndex(*it));//"������ ���� : %s"

				g_pGameMain->RenderPopUpWindowImage(OCCUPY_NPC_BOX_FONT_START_X+80,
				OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*(OCCUPY_OCCUPY_SHOP+ncount))-5,
				strlen(buf)*7, 1);
			
				m_pFontPopup->DrawText(OCCUPY_NPC_BOX_FONT_START_X+80, 
				OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*(OCCUPY_OCCUPY_SHOP+ncount))-5,
				RGB(255,0,0),
				buf,0L);

				ncount++;
				it++;
			}
		}		
		if(i != OCCUPY_GREATING)
		{
			if(i == OCCUPY_TEX_INFO)
			{
				if(m_nOccupyGuildUID == g_pShuttleChild->m_myShuttleInfo.GuildUniqueNumber)
				{
					m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X+230, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
					m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
						(char *)m_vecInfoOccupy[i].c_str(),0L);
				}
			}			
			else
			{
				// �����޴� ���ǥ�� 
				if(0 == i)
				{
					if(bGuildMaster)
					{
						m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X+230, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
							m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
							(char *)m_vecInfoOccupy[i].c_str(),0L);
					}
				}
				else
				{
					m_pFontMenuList[i]->DrawText(OCCUPY_NPC_BOX_FONT_START_X+230, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i),
						m_nSelectText == i ? GUI_FONT_COLOR_Y : dwFontColor,
						(char *)m_vecInfoOccupy[i].c_str(),0L);
				}

			}
		}
	}

//	if(m_nSelectText == OCCUPY_OCCUPY_SHOP)
//	{
//		g_pGameMain->RenderPopUpWindowImage(OCCUPY_NPC_BOX_FONT_START_X+80,OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*OCCUPY_OCCUPY_SHOP)+15,
//			strlen((char *)m_vecInfoOccupy[OCCUPY_OCCUPY_SHOP].c_str())*7, 1);
//		m_pFontPopup->DrawText(OCCUPY_NPC_BOX_FONT_START_X+80, OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*OCCUPY_OCCUPY_SHOP)+15,
//			RGB(255,0,0),
//			(char *)m_vecInfoOccupy[m_nSelectText].c_str(),0L);
//	}
}
void CINFCityOccupy::Tick()
{
	FLOG( "CINFCityOccupy::Tick()" );
}
int CINFCityOccupy::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	FLOG( "CINFCityOccupy::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)" );

	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			m_nSelectText = 100;

			if( pt.x > OCCUPY_NPC_BOX_FONT_START_X&& 
				pt.x < OCCUPY_NPC_BOX_FONT_START_X+OCCUPY_FONT_SELECT_LINE)
			{
				for(int i=1; i<OCCUPY_MENULIST_COUNT; i++)
				{
					if( pt.y > OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)&& 
						pt.y < OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)+15)
					{
						m_nSelectText = i;
						INF_MSGPROC_BREAK;
					}
				
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if(m_nOccupyGuildMasterUID != g_pShuttleChild->m_myShuttleInfo.CharacterUniqueNumber)
				break;

			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);

			char buf[512];
			memset(buf, 0x00, 512);
			
			if( pt.x > OCCUPY_NPC_BOX_FONT_START_X&& 
				pt.x < OCCUPY_NPC_BOX_FONT_START_X+OCCUPY_FONT_SELECT_LINE)
			{
				for(int i=1; i<OCCUPY_MENULIST_COUNT; i++)
				{
					if( pt.y > OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)&& 
						pt.y < OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)+15)
					{
						switch(i) {
						case OCCUPY_TEX:
							{
								wsprintf( buf, STRMSG_C_OCCUPY_0017);//"������ �����Ͻ� ������ �Է��Ͽ� �ֽʽÿ�"
								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_TEX_SET);
							}
							break;
						case OCCUPY_DATE_WAR:
							{
								wsprintf( buf, STRMSG_C_OCCUPY_0018);//"���� ������ (����Ͻú� ex[200502151720])����"
								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_DATE_WAR);
							}
							break;
//						case OCCUPY_USE_STORE:
//							{
//								wsprintf( buf, "���ܿ� �� â������ �����Ͻðڽ��ϱ�?");
//								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_STORE_USE);
//							}
//							break;
						case OCCUPY_OCCUPY_SHOP:
							break;
						case OCCUPY_GREATING:
							{
								wsprintf( buf, STRMSG_C_OCCUPY_0019);//"�����޴����� �����ֽ� �ʱ� �λ縻�� �����Ͽ��ֽʽÿ�"
								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_GREATING_SET);
							}
							break;
						case OCCUPY_SUCCESS_DEFENCE:
							break;
						case OCCUPY_TEX_INFO:
							{
								wsprintf( buf, STRMSG_C_OCCUPY_0020);//"������ �������ðڽ��ϱ�?"
								g_pGameMain->m_pInfWindow->AddMsgBox(buf, _Q_GET_TEX);
							}
							break;
						default:
							break;
						}						
						INF_MSGPROC_BREAK;
					}
				
				}
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			CheckMouseReverse(&pt);
			
			if( pt.x > OCCUPY_NPC_BOX_FONT_START_X&& 
				pt.x < OCCUPY_NPC_BOX_FONT_START_X+OCCUPY_FONT_SELECT_LINE)
			{
				for(int i=1; i<OCCUPY_MENULIST_COUNT; i++)
				{
					if( pt.y > OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)&& 
						pt.y < OCCUPY_NPC_BOX_FONT_START_Y+(OCCUPY_FONT_LINE_HEIGHT*i)+15)
					{
						INF_MSGPROC_BREAK;
					}
				
				}
			}
		}
		break;
	}

	return INF_MSGPROC_NORMAL;
}

struct CompareVecOccupy
{
	template<typename T>
		bool operator()(const T *pPtr1, const T *pPtr2)
	{
		if( pPtr1->OwnerOrderNumber == NULL || pPtr2->OwnerOrderNumber == NULL )
		{
			return false;
		}
		if(pPtr1->OwnerOrderNumber < pPtr2->OwnerOrderNumber)
		{
			return true;
		}

		return false;
	}
};

void CINFCityOccupy::SortStoreOccupyByVecBuildingNPC(vector<BUILDINGNPC*> *VecBuildingNPC)
{
	sort(VecBuildingNPC->begin(), VecBuildingNPC->end(), CompareVecOccupy());
}

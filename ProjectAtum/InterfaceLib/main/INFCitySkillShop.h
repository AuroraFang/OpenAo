// INFCitySkillShop.h: interface for the CINFCitySkillShop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYSKILLSHOP_H__4400101C_3F11_4453_BECA_919258E27AC6__INCLUDED_)
#define AFX_INFCITYSKILLSHOP_H__4400101C_3F11_4453_BECA_919258E27AC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class CD3DHanFont;
class CINFImageEx;									  // 2011. 10. 10 by jskim UI�ý��� ����
class CINFCitySkillShop : public CINFBase  
{
public:
	CINFCitySkillShop(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFCitySkillShop();

	BOOL IsInputItemDone() { return m_bInputItemDone; }

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitData();
	int InputItem(char* pMsg);
	void InputItemDone();//m_pnSelectItemNumber �迭 �ʱ�ȭ
//	void SellItem( LONGLONG nUniqueNumber, int nAmount/*����*/, BYTE itemKind );
//	void BuyItem(int nItemNum, int nAmount);
	UINT GetTotalPrice();
	void ReloadSkillItemList();

	void OnOkButtonClicked();
	void OnCancelButtonClicked();
//	void OnBuyButtonClicked(int nSlotNumber);

//	void UpdateItem(int nItemNum);	// 2015-05-28 Future

protected:
	SHOP_ITEM* FindSkillShopData(SHOP_ITEM* pShopSkill);

public:
	BOOL			m_bInputItemDone;// InputItemDone�� �� ��� TRUE, �� ���� TRUE�̸� ������ ����Ʈ�� �ٽ� ���� �ʴ´�.
	BOOL			m_bRestored;
	BOOL			m_bInvalidated;
//	BOOL			m_bCashErr;

	CINFImageEx	*	m_pOkButton[4];
	CINFImageEx	*	m_pCancelButton[4];
	CINFImageEx	*	m_pBuyButton[4];
	CINFImageEx	*	m_pCash;
	CINFImageEx	*	m_pSlot;
	CINFImageEx	*	m_pItemTab[4];
	CINFImageEx	*	m_pScrollUpDown;
	CINFImageEx	*	m_pScrollBar;
	CINFImageEx	*	m_pSelectSlot;

	int				m_nCurrentTab;
	int				m_nOkButtonState;
	int				m_nCancelButtonState;
	int				m_nBuyButtonState[SHOP_ITEM_SLOT_NUMBER];

//	map<int,SHOP_ITEM*>	m_mapItemInfo[SHOP_ITEM_TAB_NUMBER];//���� ������ �ֱ�
	vector<SHOP_ITEM*> m_vecItemInfo[SHOP_ITEM_TAB_NUMBER];
	vector<SHOP_ITEM*> m_vecAllSkillData;

	int				m_nCurrentScrollNumber[SHOP_ITEM_TAB_NUMBER];
	BOOL			m_bScrollLock;
	POINT			m_ptMouse;
	CD3DHanFont*	m_pFontItemName[SHOP_ITEM_SLOT_NUMBER];
	CD3DHanFont*	m_pFontItemPrice[SHOP_ITEM_SLOT_NUMBER];
	CD3DHanFont*	m_pFontItemNumber[SHOP_ITEM_SLOT_NUMBER];
	CD3DHanFont*	m_pFontTotalPrice;

	int*			m_pnSelectItemNumber[SHOP_ITEM_TAB_NUMBER];// array, �� �������� ������ ������ ������ �����Ѵ�.
	int				m_nCurrentSelectSlot[SHOP_ITEM_TAB_NUMBER];
	BUILDINGNPC*	m_pBuildingInfo;

	CD3DHanFont*	m_pInfluenceTex;
};

#endif // !defined(AFX_INFCITYSKILLSHOP_H__4400101C_3F11_4453_BECA_919258E27AC6__INCLUDED_)

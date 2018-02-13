#pragma once

#include "INFBase.h"
#include "INFImageBtn.h"

#define CASH_SHOP_BUTTON_STATE_ALL	3		// ��ư ����
#define CASH_SHOP_ITEM_FONT			15		// �����ۼ� ������ ����(�̸�, ����, ĳ��)
#define CASH_SHOP_ITEM_INFO			13		// �����ۼ� ������ ����(�뵵)

#define INVEN_ROTATION_NUM		5	// �����̼� �̹��� 
#define INVEN_ROTATION_STATE	2	// �����̼� �̹��� 


#ifdef C_EPSODE4_UI_CHANGE_JSKIM
#define MAX_CASH_SHOP_TAB					7		// ĳ���� ��ü �Ǽ�+1 (������ 1���� �������̴�.) // 2009-01-28 by bhsohn ĳ���� ó��
#else
#define MAX_CASH_SHOP_TAB					11		// ĳ���� ��ü �Ǽ�+1 (������ 1���� �������̴�.) // 2009-01-28 by bhsohn ĳ���� ó��
#endif

#define		CASH_SHOP_VIEW_TAB					MAX_CASH_SHOP_TAB	// ���� �ִ��

#define		MAX_NEW_CASHSHOP_IMAGE				2

class CINFImage;
class CINFImageEx;
class DataHeader;
class CGameData;
class CD3DHanFont;
class CINFScrollBar;
class CINFGroupImage;
class CINFImageBtn;

struct CashItemIcon
{
	int						nItemNum;
	DataHeader*				pDataHeader;
	CINFImageEx	*			vecImgCashIcon;
};

class CINFCityCashShop: public CINFBase
{
protected:
	CINFCityCashShop() = default;

public:
	CINFCityCashShop(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFCityCashShop();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 2007-08-08 by dgwoo ĳ���ǿ� �̷� �߰�.
	int ProcessRotationUnitWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int ProcessRotationUnit(POINT pt, UINT uMsg);
	void SetByPushButton(BOOL bFlag);
	void OnChangeWearItem();
	void InputCashColorShopItem();
	void SendMsgGift(MSG_FC_SHOP_CHECK_GIVE_TARGET_OK* pMsg);

	// 2009-01-28 by bhsohn ĳ���� ó��
	//SHOP_ITEM* FindCashShopData(int nTabIndex, SHOP_ITEM* pCashShop);
	SHOP_ITEM* FindCashShopData(int nTabIndex, UINT ItemNum);
	// end 2009-01-28 by bhsohn ĳ���� ó��
private:
	// 2007-08-21 by bhsohn ĳ�� ������ alt+tab���� ����
	CINFImageEx* FindCashIcon(int nItemNum, DataHeader*	pDataHeader);

	// 2009-01-28 by bhsohn ĳ���� ó��
	void UpdateGiftButton(int i_nCurrentSelectTab, int i_nItemSelect);
	void RefreshGetSkinItemIndex();
	int GetSkinItemIndex();
	int GetNonEmptyNextIndex(int i_nStartIdx);	
	void ReLoadSkinItemVector();
	int GetTabToItemIdx(int i_nIdx);
	void RenderNewShopInfo(int nPosX, int nPosY, SHOP_ITEM* i_pShopInfo);
	// end 2009-01-28 by bhsohn ĳ���� ó��

protected:
	// 2011. 11. 17 by jskim EP4 UI ����
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*			m_pImgBack;
	CINFImageBtn*			m_pCloseBtn;
#else
	CINFImageEx	*			m_pImgBack;
#endif
	
	vector< CINFImageEx* > m_vecCINFImageInfo;


	// end 2011. 11. 17 by jskim EP4 UI ����
	CINFImageEx	*			m_pImgSelectItem;
	CINFImageEx	*			m_pImgTab[CASH_SHOP_VIEW_TAB];
	CINFImageEx	*			m_pImgChargeButton[CASH_SHOP_BUTTON_STATE_ALL];
	CINFImageBtn	*		m_pImgGiftButton;
	CINFImageEx	*			m_pImgBuyButton[CASH_SHOP_BUTTON_STATE_ALL];
	CINFImageEx	*			m_pImgMoreItem[4];

	vector<CashItemIcon>	m_vecImgCashIcon[MAX_CASH_SHOP_TAB];
	CD3DHanFont	*			m_pFontItem[CASH_SHOP_ITEM_FONT];
	CD3DHanFont	*			m_pFontItemInfo[CASH_SHOP_ITEM_INFO];
	BUILDINGNPC	*			m_pBuildingInfo;
	BOOL					m_bInputItemDone;				// InputItemDone�� �� ��� TRUE, �� ���� TRUE�̸� ������ ����Ʈ�� �ٽ� ���� �ʴ´�.
	CINFScrollBar*			m_pScroll;
	POINT					m_pointBeforeMousePoints;			// ���콺 ������ǥ
	ITEM		*			m_pItem;
	
	int						m_nCurrentMouseStateCharge;		// ĳ�� ���� ��ư ���� 
	BOOL					m_bCurrentMouseStateCharge;	// ĳ�� ���� ��ư ���� 
	int						m_nCurrentMouseStateBuy;		// ��� ��ư ����
	BOOL					m_bCurrentMouseStateBuy;		// ��� ��ư ����
	int						m_nMyCashPoint;					// �� ĳ�� ����(��ü : ĳ�� + ��ǰ��)
	int						m_nGiftCardPoint;				// �� ĳ���� ��ǰ��
	int						m_nGiftButtonState;				// ������ư ����.
	
	int						m_nShopOldPosX;
	int						m_nShopOldPosY;
	float					m_fTimeMoreIcon;
	BOOL					m_bMoreItemRenderFlag;

	int						m_nShopTapEndPosX[CASH_SHOP_VIEW_TAB];

	// 2007-08-08 by dgwoo �̷� �߰�.
	CINFImageEx*			m_pDirection[INVEN_ROTATION_STATE][INVEN_ROTATION_NUM];
	CINFImageEx*			m_pRotationBase;

	BOOL					m_bByPush[4];
	int						m_nRotationState;
	int						m_nButtonState;
	
	BOOL					m_bTurnLeft;
	BOOL					m_bTurnRight;
	BOOL					m_bTurnUp;
	BOOL					m_bTurnDown;

	// 2009-01-28 by bhsohn ĳ���� ó��
	int						m_nTabToItemIdx[MAX_CASH_SHOP_TAB];
	int						m_nSkinItemIdx;
	CINFImageEx	*			m_pImgNewTab[MAX_NEW_CASHSHOP_IMAGE];
	int						m_nNewPlayIndex;
	float					m_fNewPlayTime;
	// end 2009-01-28 by bhsohn ĳ���� ó��
	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	CINFImageEx*				m_pSoldOut_Item;
	CINFImageEx*				m_pSoldOut_ToolTip;
	CINFImageEx*				m_pLimitedEdtion[ 2 ];

	BOOL					m_bBling;
	DWORD					m_nBlingStartTime;
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���


public:
	BOOL		IsInputItemDone() { return m_bInputItemDone; }
	void		InitData();
	void		InputItemDone();
	void		SendBuyItem(ITEM *pItem);
	void		SendChargeCash();
	int			InputItem(char* pMsg);
	void		SetCashMoney(int nMoney){m_nMyCashPoint = nMoney; }
	void		SetGiftCardCashPoint(int nGiftCardPoint) { m_nGiftCardPoint = nGiftCardPoint; }
	void		RenderDescIcon();

	void		ClearItemList();

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	void		RenderDescIcon( POINT pPos, ITEM* pItem, int nWidht, int nHeight );
#endif
	void		CashItemSort();

	// 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���
	void		UpdateCashItemRemainCount( ItemNum_t nItemNum, INT nRemainCount );
	// end 2010. 01. 27 by ckPark ĳ�� ������ ���� �Ǹ� �ý���


	int			m_nItemSelect;								// ������ ����
	int			m_nShopMovePosX;							// â�̵� ������
	int			m_nShopMovePosY;							// â�̵� ������
	int			m_nCurrentSelectTab;
	
	BOOL		m_bItemBuySend;
	BOOL		m_bMoveShop;
	
	UID32_t		m_nGiveTargetCharacterUID;					// 2007-11-15 by dgwoo ������ �� ĳ������ UID

	vector<SHOP_ITEM*> m_vecItemInfo[MAX_CASH_SHOP_TAB];	
};
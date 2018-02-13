// INFCityBazaar.h: interface for the CINFCityBazaar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYBAZAAR_H__500337C6_49C9_4801_AF8B_CC7F7D26F2EB__INCLUDED_)
#define AFX_INFCITYBAZAAR_H__500337C6_49C9_4801_AF8B_CC7F7D26F2EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFComboBoxEX.h"
#include "ChatTab_t.h"

#define BAZAARSHOP_ITEMCOUNT		16
#define ITEMINFO_ITEM_FULL_NAME		140	// 140 = desc title len(20) + prefix len + item name len + suffix len

/*--------------------------------------------------------------------------*/
//��ư�� ����.
#define	BTN_STATE_OVER					0
#define	BTN_STATE_CLICK					1
#define	BTN_STATE_NORMAL				3
#define	BTN_STATE_DEACTIVE				2
/*--------------------------------------------------------------------------*/

enum {
		BAZAAR_VISIT_BUY_COUNT = 1,				//�湮�� ����
		BAZAAR_VISIT_BUY_PRICE,					//�湮�� ����
		BAZAAR_BTN,								//��� ���
		BAZAAR_BTN_VISIT_OUT,					//������ ��ư.
		BAZAAR_ICON,							//������ ������
		BAZAAR_VISIT_SEL_COUNT,					//

		BAZAAR_OPEN_1_BUY,
		BAZAAR_OPEN_3_BUY,
		BAZAAR_OPEN_2_BUY,
		BAZAAR_OPEN_4_BUY,
		
		BAZAAR_OPEN_2_SEL,
		BAZAAR_OPEN_3_SEL,

		BAZAAR_SHOP_WINDOW,						//����â �������� ������.
		BAZAAR_SHOP_TAB,						//�� ������
		BAZAAR_SHOP_SCROLL,						//���� ��ũ����ġ.
		BAZAAR_SHOP_VISIT_SCROLL,				//�湮�� ��ũ����ġ
		BAZAAR_SHOP_TAB_WHEEL,					//�Ǿ��� ������
		BAZAAR_SHOP_OPEN_START,					//���� 
		BAZAAR_SHOP_OPEN_STOP,					//�ߴ�.
		BAZAAR_SHOP_ITEM,						//�Ѱ��� ������
		BAZAAR_OPEN_NAME,						//����
		BAZAAR_SHOP_ITEM_AREA,					//�����۵��� ����.
		BAZAAR_SHOP_MIN,						//�ּ�ȭ��ư ����.
		BAZAAR_SHOP_ACT							//Ȱ��ȭ ��ư ����.
		

};

class CINFImage;
class CINFImageEx;
class CD3DHanFont;
class CItemInfo;
class CINFGroupImage;

class CINFCityBazaar : public CINFBase  
{
public:
	CINFCityBazaar();
	virtual ~CINFCityBazaar();

	virtual HRESULT InitDeviceObjects()				{return S_OK;}
	virtual HRESULT RestoreDeviceObjects()			{return S_OK;}
	virtual HRESULT DeleteDeviceObjects()			{return S_OK;}
	virtual HRESULT InvalidateDeviceObjects()		{return S_OK;}

	virtual void	Render()														{}
	virtual void	Render_Disable()												{}
	virtual void	Tick()															{}
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void	Recv_OpenShopOK()												{}
	virtual	void	Send_SellBuyItem(BOOL bRequest = FALSE)							{}
	virtual void	Recv_RegSellItemOK(MSG_FC_BAZAAR_SELL_PUT_ITEM_OK* pMsg)		{}
	virtual void	Recv_DelSellItemOK(MSG_FC_BAZAAR_SELL_CANCEL_ITEM_OK* pMsg)		{}
	virtual void	Recv_SetSellItem(SBAZAAR_SELL_ITEM * pSellItem)					{}
	virtual void	Recv_SetSellEnchantItem(UID64_t itemUID, int EnchantItemNum)	{}
	virtual void	Recv_SellBuyItemOK(MSG_FC_BAZAAR_SELL_BUY_ITEM_OK* pMsg)		{}
	virtual void	Recv_BuySellItemOK(MSG_FC_BAZAAR_BUY_SELL_ITEM_OK* pMsg)		{}

	virtual void	Recv_RegBuyItemOK(MSG_FC_BAZAAR_BUY_PUT_ITEM_OK* pMsg)			{}
	virtual void	Recv_DelBuyItemOK(MSG_FC_BAZAAR_BUY_CANCEL_ITEM_OK* pMsg)		{}
	virtual void	Recv_SetBuyItem(SBAZAAR_BUY_ITEM * pBuyItem)					{}

	virtual void	Recv_CustomerInfoOK(MSG_FC_BAZAAR_CUSTOMER_INFO_OK* pMsg)		{}
	virtual void	InitOpenData()													{}
	virtual void	InitVisitData()													{}

	virtual int		GetShopItemCount(UID64_t itemUID);
	virtual CItemInfo *	GetShopItemInfo(UID64_t itemUID);

	// 2007-11-01 by bhsohn ���� �̿��߿� ���� ó��
	virtual void	CloseVisitShop();

	BOOL			GetbBazaarShopStart()			{return m_bBazaarShopStart;}
	BOOL			GetbBazaarShopEnd()				{return m_bBazaarShopEnd;}
	void			SetbBazaarShopEnd(BOOL bEnd)	{m_bBazaarShopEnd = bEnd;}
	BYTE			GetbBazaarShopType()			{return m_byBazaarType;}
	BYTE			GetbBazaarShopTypeState()		{return m_byBazaarTypeState;}
	ClientIndex_t	GetBazaarClientIndex()			{return m_ClientIndex;}
	BOOL			GetBazaarDisable()				{return !m_bBazaarMinAct;}

	int				GetScrollLineNumber(float nLength);
	float			GetScrollLineInterval(int nScrollLineNum);
	BOOL			GetPointInType(POINT &Pt,int Type,int Num = 0);
	int				GetScrollLine();
	void			SetScrollEndLine();
	void			InitScrollLine();

	void			SetItemInfo(CItemInfo* pSelectItem, int x, int y);
	void			UnderBarDrawText(CD3DHanFont *pHanFont,FLOAT sx,FLOAT sy,DWORD dwColor,TCHAR* strText,DWORD dwFlags);

protected:
	BYTE			m_byBazaarType;				//1 : Sell ,  2 : Buy;
	BYTE			m_byBazaarTypeState;		//1 : Open,	  2 : Visit;

	/*--------------------------------------------------------------------------*/
	// 2006-07-25 by dgwoo
#ifdef C_EPSODE4_UI_CHANGE_JSKIM    // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*	m_pBackWindow;					//���� ���.
#else
	CINFImageEx	*	m_pBackWindow;					//���� ���.
#endif
	CINFImageEx	*	m_pPutItemBtn[4];				//������ ��� ��ư �̹��� 
	CINFImageEx	*	m_pDelItemBtn[4];				//������ ��� ��ư �̹���
	CINFImageEx	*	m_pStartBtn[4];					//���� ���� ��ư �̹���
	CINFImageEx	*	m_pStopBtn[4];					//���� �ߴ� ��ư �̹���
	CINFImageEx	*	m_pMinBtn[4];					//�ּ�ȭ ��ư.
	CINFImageEx	*	m_pActBtn[4];					//Ȱ��ȭ ��ư.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM    // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx	*	m_pMinBar1;						//�ּ�ȭ�� ��(�Ǹ� ����)
#endif
	CINFImageEx	*	m_pMinBar;						//�ּ�ȭ�� ��.

	CINFImageEx	*	m_pScrollButton;				// ��ũ�� ��ư

	int				m_nStartBtnState;				//���� ��ư ����
	int				m_nStopBtnState;				//�ߴ� ��ư ����
	int				m_nMinBtnState;					//����â�� �ּ�ȭ & Ȱ��ȭ ��ư�� ����.
	int				m_nOutBtnState;					//������ ��ư ����.
	int				m_nPutDelState[5];				//��� ���� ��ư�� ����.


	/*--------------------------------------------------------------------------*/
	CItemInfo	*	m_pCurrentItemInfo;				// ���� ���̴� ������ ����

	/*--------------------------------------------------------------------------*/
	int				m_nScrollBtn;					// ��ũ�� ���� ��ġ
	int				m_nScrollPosition;				// ��ũ�� ��ġ
	int				m_nScrollLine;					// ��ũ�� ����
	int				m_nSelectIndex;					// ���� ����

	BOOL			m_bLButtonDown;
	BOOL			m_bScrollLock;

	int				m_nScrollMaxCount;
	int				m_nScrollMaxLine;
	
	/*--------------------------------------------------------------------------*/
	// ���� ����
	BOOL			m_bBazaarShopStart;				// ���� ����
	BOOL			m_bBazaarShopEnd;				// ���� ����
	BOOL			m_bBazaarMinAct;				//������ ��Ȱ��ȭ : 0,Ȱ��ȭ : 1

	/*--------------------------------------------------------------------------*/
	// ��Ÿ
	ClientIndex_t	m_ClientIndex;				// ���� ������ Ŭ���̾�Ʈ �ε���
};



//////////////////////////////////////////////////////////////////////////
// ���� ���� ������
enum {TEXT_NON = 0, TEXT_NAME, TEXT_COUNT, TEXT_PRICE};
enum {COMBO_NORMAL = 0,COMBO_UNDER,COMBO_CLICK};
enum {OPEN_START = 1, OPEN_END, OPEN_REGISTER, OPEN_CANCEL};

typedef struct OPENITEM
{
	CItemInfo *		pItemBase;
	int				nItemCount;
	int				nItemPrice;
	int				nItemTotal;

	int				nItemIndex;

	BOOL			bRegister;				//0: ��� , 1: ���

	// 2007-11-14 by bhsohn ���� ������ ���⸦ �÷ȴ� ��ҽ�, ���� �Ѿ� ���� �߸������� ���� ����
	INT				nBulletCount;			// ����: ���� �߼�, ������: ���� ����

	OPENITEM()
	{
		pItemBase		= NULL;
		nItemCount		= 1;
		nItemPrice		= 0;
		nItemTotal		= 0;
		nItemIndex		= 0;
		bRegister		= FALSE;

		// 2007-11-14 by bhsohn ���� ������ ���⸦ �÷ȴ� ��ҽ�, ���� �Ѿ� ���� �߸������� ���� ����
		nBulletCount	= 0;
	}

	~OPENITEM()
	{
		pItemBase		= NULL;
	}
	
} OPEN_BAZAAR_ITEM;

class CINFCityBazaarOpen : public CINFCityBazaar
{
public:
	CINFCityBazaarOpen(CAtumNode* pParent, CGameData* m_pGameData, int nBazaarType);
	virtual ~CINFCityBazaarOpen();

	virtual void	InitOpenData();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	virtual void	Render();
	virtual void	Tick();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void	Recv_RegSellItemOK(MSG_FC_BAZAAR_SELL_PUT_ITEM_OK* pMsg);
	virtual void	Recv_DelSellItemOK(MSG_FC_BAZAAR_SELL_CANCEL_ITEM_OK* pMsg);
	virtual void	Recv_RegBuyItemOK(MSG_FC_BAZAAR_BUY_PUT_ITEM_OK* pMsg);
	virtual void	Recv_DelBuyItemOK(MSG_FC_BAZAAR_BUY_CANCEL_ITEM_OK* pMsg);
	virtual void	Recv_OpenShopOK();
	virtual void	Recv_SellBuyItemOK(MSG_FC_BAZAAR_SELL_BUY_ITEM_OK* pMsg);
	virtual void	Recv_BuySellItemOK(MSG_FC_BAZAAR_BUY_SELL_ITEM_OK* pMsg);
	virtual void	Recv_CustomerInfoOK(MSG_FC_BAZAAR_CUSTOMER_INFO_OK* pMsg);

	virtual int		GetShopItemCount(UID64_t itemUID);
	virtual CItemInfo *	GetShopItemInfo(UID64_t itemUID);

private:
	void			Render_Common();
	void			Render_Sell();
	void			Render_Buy();
	void			Render_Log();
	void			Render_Disable();

	int				WndProc_Sell(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProc_Buy(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProc_Common(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProc_Disable(UINT uMsg,WPARAM wParam, LPARAM lParam);
	

	void			Send_OpenShop();
	void			Send_CloseShop();
	void			Send_DelItem(int nSelectItem);
	void			Send_RegItem(int nSelectItem);

	void			OnSellItem(CItemInfo* pSelectItem);
	void			OnSellDelItem(UID64_t itemUID);
	void			OnBuyItem(int ItemNum, int ItemKind);
	void			OnReadyBuyItem();
	void			OnButtonClicked(int nButton);

	void			SetDataCombo(int nComboActive);
	void			SetSelectItem(CItemInfo* pSelectItem, POINT pt);

	int				GetItemIndexByPos(int nPosY);
	bool			PointInRect(POINT &pt,RenderRect &rt);

	OPEN_BAZAAR_ITEM * FindItem(CItemInfo* pItemInfo);
	OPEN_BAZAAR_ITEM * FindItem(UID64_t itemUID);
	vector<OPEN_BAZAAR_ITEM>::iterator FindItemItr(UID64_t itemUID);
	OPEN_BAZAAR_ITEM * FindItem(int nItemNum, BYTE nItemType = 1, BOOL bRegister = FALSE);
	vector<OPEN_BAZAAR_ITEM>::iterator FindItemItr(int nItemNum, BYTE nItemType = 1, BOOL bRegister = FALSE);

	void			CheckBtnState();
	void			CheckSellItemCount();
	BOOL			CheckBuyCountItem(int nKind, int nItemNum);
	void			CheckLogMsg(char* msgLog, DWORD dwColor);
	BOOL			CheckOnSellItem(CItemInfo* pSelectItem);
	BOOL			CheckOnBuyItem(int nItemNum, int nItemKind);

	void			CalTotalPrice();

protected:
	CINFImageEx *		m_pShopName;				//������ �̸� �̹���.
	CINFImageEx *		m_pBackTab[2];				//�� ���
	CINFImageEx *		m_pBackList;				//����Ʈ ���
#ifdef C_EPSODE4_UI_CHANGE_JSKIM    // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx *		m_pBackListBox;				//����Ʈ ���
#endif
	CINFImageEx *		m_pBackItem;				//Ȱ��ȭ�� �������� ���.
	CINFImageEx *		m_pSelectBar;				// ���� ��
	CINFImageEx *		m_pBackTotal;				//�Ѿ� ���.
	CINFImageEx *		m_pArrowIcon[2];			//comboBoxȭ��ǥ ������.
	CINFImageEx *		m_pBackCombo;				//�޺��ڽ�.
	CINFImageEx *		m_pBackComboScroll;			//�޺��ڽ� ��ũ��.
	CINFImageEx *		m_pBackComboSel;			//�޺��ڽ����� ���þ�����.

	CD3DHanFont *	m_pFontItemName;			//������ �̸�
	CD3DHanFont *	m_pFontItemPrice;			//������ ����
	CD3DHanFont	*	m_pFontItemCount;			//������ ����
	CD3DHanFont *	m_pFontItemSum;				//������ �� ���� 
	CD3DHanFont *	m_pFontName;				//����
	CD3DHanFont *	m_pFontTotal;				//�Ѿ�
	CD3DHanFont *	m_pFontLogMsg;				//�α�
	CD3DHanFont	*	m_pFontShopType;			//�ǸŻ���, ���Ż���.

	/*--------------------------------------------------------------------------*/
	BYTE			m_byTabIndex;	
	int				m_nTotal;					//�Ѿ�
	char			m_strInputMessage[SIZE_MAX_BAZAAR_NAME];		//�̸�
	int				m_nComboState[5][2];						//ComboBox Arrow Icon State
	int				m_nComboActive;								//�޺��ڽ� Ȱ��ȭ.0:��Ȱ��ȭ.1:Kind 2:Name

	int				m_nLogMaxCount;				
	
	/*--------------------------------------------------------------------------*/
	// Ű �Է�
	BYTE			m_byFocusText;
	BOOL			m_bSelectItem;

	/*--------------------------------------------------------------------------*/
	// ��Ÿ
	INVEN_DISPLAY_INFO m_DisplayInfo;

private:
	vector<OPEN_BAZAAR_ITEM> m_vecOpenBazaarItem;
	CINFComboBoxEX		*	m_pComboKindEx;						//���� �޺�
	CINFComboBoxEX		*	m_pComboNameEx;						//���� �޺�

	ChatTab_t		m_LogChat;					// �α�

};


//////////////////////////////////////////////////////////////////////////
// ���� ���� �湮��
typedef struct VISITITEM
{
	char			cItemName[ITEMINFO_ITEM_FULL_NAME];
	CItemInfo*		pItemInfo;

	int				nAmount0;
	int				nEachPrice0;

	int				nCurrentCount;

	int				nItemIndex;

	BOOL			bExistence;

	VISITITEM()
	{
		memset(cItemName, 0x00, ITEMINFO_ITEM_FULL_NAME);
		pItemInfo		= NULL;

		nAmount0		= 0;
		nEachPrice0		= 0;

		nCurrentCount	= 0;
		nItemIndex		= 0;

		bExistence		= FALSE;
	}

	~VISITITEM()
	{
		memset(cItemName, 0x00, ITEMINFO_ITEM_FULL_NAME);
		pItemInfo		= NULL;
	}

} VISIT_BAZAAR_ITEM;

class CINFCityBazaarVisit : public CINFCityBazaar
{
public:
	CINFCityBazaarVisit(CAtumNode* pParent, CGameData* m_pGameData, int nBazaarType, ClientIndex_t clientIndex);
	virtual ~CINFCityBazaarVisit();

	virtual void	InitVisitData();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	virtual void	Render();
	virtual void	Tick();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	virtual void	Recv_SetSellItem(SBAZAAR_SELL_ITEM *pSellItem);
	virtual void	Recv_SetSellEnchantItem(UID64_t itemUID, int EnchantItemNum);
	virtual void	Recv_SellBuyItemOK(MSG_FC_BAZAAR_SELL_BUY_ITEM_OK* pMsg);
	virtual void	Recv_SetBuyItem(SBAZAAR_BUY_ITEM * pBuyItem);
	virtual void	Recv_BuySellItemOK(MSG_FC_BAZAAR_BUY_SELL_ITEM_OK* pMsg);

	virtual	void	Send_SellBuyItem(BOOL bRequest = FALSE);
	
	// 2007-11-01 by bhsohn ���� �̿��߿� ���� ó��
	virtual	void	CloseVisitShop();


private:
	void			Render_Common();
	void			Render_Sell();
	void			Render_Buy();

	int				WndProc_Sell(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProc_Buy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int				GetItemIndexByPos(int nPosY);
	void			CheckItemCount();

	// 2007-11-01 by bhsohn ���� �̿��߿� ���� ó��
	//void			CloseVisitShop();

	void			SetItemFullName(VISIT_BAZAAR_ITEM* pVisitItem);

	VISIT_BAZAAR_ITEM * FindItem(UID64_t itemUID);
	vector<VISIT_BAZAAR_ITEM>::iterator FindItemItr(UID64_t itemUID);
	VISIT_BAZAAR_ITEM * FindItem(int nItemIndex);
	vector<VISIT_BAZAAR_ITEM>::iterator FindItemItr(int nItemIndex);

protected:
	CINFImageEx *		m_pSum;						//�հ� ���.
	CINFImageEx *		m_pBackList;				//����Ʈ ���.
	CINFImageEx *		m_pBackTab;					//�� ���.
	CINFImageEx *		m_pBackItem;				//������ ���.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM    // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx *		m_pBackListBox;				//����Ʈ ���
#endif
	CINFImageEx *		m_pSelectBar;				// ���� ��
	CINFImageEx *		m_pOKBtn[4];				//����, �Ǹ� ��ư
	
	CD3DHanFont *	m_pFontItemName;			//������ �̸�
	CD3DHanFont *	m_pFontItemPrice;			//�ƾ��� ����
	CD3DHanFont *	m_pFontItemCount;			//������ ���� 
	CD3DHanFont *	m_pFontBuySellItemCount;	//������ �Ǹ�, ���� ���� 
	CD3DHanFont *	m_pFontSumPrice;								//��ü ����

	int				m_nSum;											//�հ�.
	int				m_nOKState[5];									//���� �Ǹ� ��ư ����.
	int				m_byFocusText;				//�ؽ�Ʈ Ȱ��ȭ.

	vector<VISIT_BAZAAR_ITEM> m_vecVisitBazaarItem;


};

#endif // !defined(AFX_INFCITYBAZAAR_H__500337C6_49C9_4801_AF8B_CC7F7D26F2EB__INCLUDED_)

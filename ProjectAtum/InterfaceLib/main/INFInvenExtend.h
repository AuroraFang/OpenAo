// INFInvenExtend.h: interface for the CINFInvenExtend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINVENEXTEND_H__72C927DE_F4B8_40AF_8E52_F525B1E5541E__INCLUDED_)
#define AFX_INFINVENEXTEND_H__72C927DE_F4B8_40AF_8E52_F525B1E5541E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "atumparam.h"
#include "RangeTime.h"

#include "INFInvenItem.h"								 // 2011. 10. 10 by jskim UI�ý��� ����



#define INVEN_X_NUMBER	10
#define INVEN_Y_NUMBER	6
#define INVEN_NUMBER	(INVEN_X_NUMBER*INVEN_Y_NUMBER)	// �� �κ���
#define WEAR_ITEM_NUMBER 9

#define INVEN_ROTATION_NUM		5	// �����̼� �̹��� 
#define INVEN_ROTATION_STATE	2	// �����̼� �̹��� 

#define INVEN_SLOT_SIZE			30
#define INVEN_SLOT_INTERVAL		32
#define INVEN_ITEM_SLOT_START_X 309
#define INVEN_ITEM_SLOT_START_Y 30

// �κ�â ����
#define INVEN_ITEM_WND			0	// ������ â
#define INVEN_EQ_WND			1	// ���â


class CGameData;
class CINFImage;
class CD3DHanFont;
class CItemInfo;

class CINFInvenEquip;
class CINFInvenItem;

class CINFItemMenuList;	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

typedef struct 
{	
	INT			CurrentCount;
	UID64_t		UniqueNumber;					// ������ ������ȣ
} structDelItemInfo;

class CINFInvenExtend  : public CINFBase 
{
public:
	CINFInvenExtend(CAtumNode* pParent);
	virtual ~CINFInvenExtend();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void			Render();
	void			RenderOnCityBase();
	void			RenderSpi(int x, int y);
	virtual void	Tick();

	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven = FALSE);
	int				WndProcOnCityBase(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void			SetAllIconInfo();
	void			SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo, POINT *pIconPos=NULL);
	void			SetItemSpi(int nSpi){ m_nItemSpi = nSpi; }
	void			AddItemSpi(int nSpi){ m_nItemSpi += nSpi; }
	void			SetItemSpiUniqueNumber(LONGLONG nUniqueNumber) { m_hyItemSpiUniqueNumber = nUniqueNumber; }
	int				GetItemSpi() { return m_nItemSpi; }
	LONGLONG		GetItemSpiUniqueNumber() { return m_hyItemSpiUniqueNumber; }
	void			DeleteSelectItem(int count);
	void			SendUseItem(ITEM_BASE* pItem);
	void			DeleteEnchantItem(UINT nUniqueNumber);
	// 2009-02-03 by bhsohn ���� ������ �� ����
	//void			SetItemInfo(INVEN_DISPLAY_INFO *pItemInfo, int x, int y);
	void			SetItemInfo(INVEN_DISPLAY_INFO *pItemInfo, int x, int y, BOOL bShowMyEq=TRUE);
	void			SetEnemyItemInfo(CItemInfo *pItemInfo, int x, int y, BOOL bShowMyEq=TRUE);	// 2012-06-14 by isshin ���۹̸�����

	HRESULT			SetResourceFile(BOOL encode,char* szFileName);
	BOOL			IsTradingItem(ITEM_GENERAL* pItem);
	BOOL			IsAbleSetItem(CItemInfo* pItemInfo, int nWindowPosition);
	BOOL			IsAbleReleaseItem(CItemInfo* pItemInfo, int nWindowPosition);
	BOOL			IsAbleChangeItem(CItemInfo* pSetItemInfo,CItemInfo* pReleaseItemInfo,int nWindowPosition);
	void			SetGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue);
	void			ReleaseGearStatByItem(GEAR_STAT& sGearStat, int nDestParam, float fDestValue);
	
	void			InitInven();

	void			SetScrollEndLine();
	void			ReSetScrollCurrentLine();

	// Item ���� ���� ó��
	void			SetMultiSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo);	
	int				GetInvenFreeSize();
	
	// ���� ���� â�� �κ� ����
	BOOL			IsInvenHave(int nItemNum);
	
	// ������ ������, �޸� ���� ����
	void			InitDeleteItemInfo();
	void			SetDeleteItemInfo(CItemInfo* pSelectItem);		

	// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
	void			ShowInven(POINT *pItem, POINT *pEq, BOOL bClick=FALSE, BOOL bShop=FALSE);
	BOOL			IsShowInven();
	INVEN_DISPLAY_INFO* GetInvenDisplayInfo(int nIdx);
	void			SetWearPosition(int iWearPosition);	
	void			ClearMultiSeletItem();// ���� ���� �߰� ����
	void			SetSingleInvenIconInfo(CItemInfo* pItemInfo);
	INVEN_DISPLAY_INFO* GetWearDisplayInfo(int nIdx);
	UID64_t			GetDeleteItemUID();
	void			SendChangeWearWindowPos(int nWindowPosition);	
	void			ShowEqInven();
	void			ShowAllInven(BOOL bShow);
	void			ShowInvenAndMirror(BOOL i_bInven,BOOL i_bMirror = FALSE );
	
	// 2013-01-17 by jhjang ������ �巡���� ������ ���� �巡������ ������ ������ �ٲ�� ���� ����
	INVEN_DISPLAY_INFO* GetInvenDisplayInfoToUniqueNum(UID64_t nUniqueNum);
	// end 2013-01-17 by jhjang ������ �巡���� ������ ���� �巡������ ������ ������ �ٲ�� ���� ����

	
	// ���� ������	
	void			RenderMirror(POINT *pMirrorPos=NULL);
	BOOL			IsEquipInvenShow();
	D3DXMATRIX		GetInvenMatInven();
	CD3DHanFont*	GetFontSpi();
	CD3DHanFont*	GetFontWeight(int nIdx);

	void			UpdateInvenScrollMax();
	void			SetWndOrder(int nWndIdx);
	POINT			GetEqInvenBkPos();

	void			SetInvenPosInfo(structInvenPosInfo* pstruInvenPosInfo);
	structInvenPosInfo		GetInvenPosInfo();

	// 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�
	void			UpdateIcon( CItemInfo* pItemInfo );
	// end 2010. 04. 01 by ckPark ���ҽ� ���� �ý��۽� ���丮�� �����ҿ��� ������ ȸ���� �� ������ �ȵǴ� ���� �ذ�

	// 2010-06-15 by shcho&hslee ��ý��� - ���������� ���� ���� ������ ����.
	INVEN_DISPLAY_INFO *rtnPtr_AttachmentItemInfo ( const INT a_nPosKind );

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	POINT			GetBkSize();

	BOOL			GetInvenMove();	
#endif

protected:
	int				GetScrollLine();
	int				GetScrollLineNumber(float nLength);
	float			GetScrollLineInterval(int nScrollLineNum);

	int				GetWearItemNum();
	void			InitInvenIconInfo();
	void			InitWearIconInfo();
	
	
	
	void			SetSingleWearIconInfo(CItemInfo* pItemInfo);	

	// ����Ŀ ������ �߰�
	void			DoModalChatAllInfluence(UID64_t		UniqueNumber);

	// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	void			RenderLuckyMechine();
	// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	
public:
	//CGameData *		m_pGameData;
	BOOL			m_bRestored;
	BOOL			m_bInvalidated;
	CINFImageEx	*	m_pItemSl;	
	
	CD3DHanFont *	m_pFontSpi;
	CD3DHanFont *	m_pFontWeight[3];
	CD3DHanFont *	m_pFontItemNum[INVEN_Y_NUMBER];
	int				m_nItemSpi;
	LONGLONG		m_hyItemSpiUniqueNumber;

	// [0][1] ... [5]
	// [6] ...
	// [12] ...
	INVEN_DISPLAY_INFO *m_pInvenDisplayInfo[INVEN_NUMBER];
	//	[2]	[0]	[4]	
	//	[1]	[5]	[3]
	//	[8]	[6]	[7]

	// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ���� ����.
	//INVEN_DISPLAY_INFO *m_pWearDisplayInfo[WEAR_ITEM_NUMBER];
	INVEN_DISPLAY_INFO *m_pWearDisplayInfo[MAX_EQUIP_POS];
	
	//int					m_nInvenCurrentScroll;
	//int					m_nInvenCurrentScrollPosition;
	
	POINT				m_ptMouse;

	CINFImageEx*			m_pSelectIcon;// ���콺 Ŭ���� ���� ���õ� ������ �̹���(reference)
	CItemInfo*			m_pSelectItem;// ������ ������(reference)
	
	// ������ ������, �޸� ���� ����
	//CItemInfo*			m_pDeleteItem;// ���� ������ ������
	structDelItemInfo		m_struDeleteItem;

//	int					m_nSelectItemWindowPosition;// Ŭ���� �������� Window������ ��ġ // 2006-06-21 by ispark, m_SelectItem->ItemWindowIndex �� ������ ����� ��

	BOOL				m_bEnchantState;
	CItemInfo*			m_pEnchantItem;

	int					m_nRenderMoveIconIntervalHeight;
	int					m_nRenderMoveIconIntervalWidth;

	INVEN_DISPLAY_INFO *m_pCurrentItemInfo;// ���� �������� �ִ� ������ ����
	
	BOOL				m_bSelectWearItem;
	BOOL				m_bMouseDownState;

	//////////////////////////////////////////////////////////////////////////	
	// ���� �κ��丮 �̹��� (�����̹����� ������ ��ȯâ ������ ���� ���� ���
	// ydkim 2005.11.04
	// ���� ������ �Լ����� �ʿ�	

	// 2011-07-22 by hsson �� �ߺ� �������� ��æƮ �Ǵ� ���� ����
	enum { SHOP_CONCURRENT_DELAY = 200, };
	DWORD				m_bShopConcurrent;
	// end 2011-07-22 by hsson �� �ߺ� �������� ��æƮ �Ǵ� ���� ����

	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
	// Item �޴� ����Ʈ
	void OnClickItemMenuListWnd(BOOL bShow, POINT ptPoint, INT nItemNum, UID64_t uItemUniNum);
	int WndProcItemMenuListWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void RenderItemMenuListWnd();	
	void SetBkPos(POINT ptBkPos);
	// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

	// 2013-03-18 by bhsohn ���丮 ���� �߰�
	POINT GetBkPos();	
	// END 2013-03-18 by bhsohn ���丮 ���� �߰�
		
protected:

	// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
	// �κ��丮
	CINFInvenEquip*		m_pINFInvenEquip;	// ����â
	CINFInvenItem*		m_pINFInvenItem;	// ������ â
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	deque<int>			m_vecWndOrder;

	POINT				m_pEqIvenNormalPos;
	BOOL				m_bEqSetPos;		// ��ġ�� �����߳�?

	POINT				m_pEqIvenShopPos;
	BOOL				m_bEqShopSetPos;		// ��ġ�� �����߳�?
 	BOOL				m_bShowEqWnd;		// �Ϲ� ���â
 	BOOL				m_bShowEqShopWnd;	// ���� ���â
#endif
	
	POINT				m_pItemIvenPos;	
	BOOL				m_bItemSetPos;		// ��ġ�� �����߳�?

	CINFItemMenuList*			m_pItemMenuList;// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

};

#endif // !defined(AFX_INFINVENEXTEND_H__72C927DE_F4B8_40AF_8E52_F525B1E5541E__INCLUDED_)

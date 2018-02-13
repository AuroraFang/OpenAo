// INFInven.h: interface for the CINFInven class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINVEN_H__072A8788_5E57_4A56_B1FB_F569534E9105__INCLUDED_)
#define AFX_INFINVEN_H__072A8788_5E57_4A56_B1FB_F569534E9105__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "atumparam.h"
#include "RangeTime.h"
#include "INFInvenExtend.h" // 2008-08-22 by bhsohn EP3 �κ��丮 ó��


// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
//#define INVEN_X_NUMBER	3
//#define INVEN_Y_NUMBER	6
//#define INVEN_NUMBER	18
//#define WEAR_ITEM_NUMBER 9
//
//#define INVEN_ROTATION_NUM		5	// �����̼� �̹��� 
//#define INVEN_ROTATION_STATE	2	// �����̼� �̹��� 

class CGameData;
class CINFImage;
class CD3DHanFont;
class CItemInfo;
class CINFImageEx;


// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
//typedef struct 
//{	
//	INT			CurrentCount;
//	UID64_t		UniqueNumber;					// ������ ������ȣ
//} structDelItemInfo;

class CINFInven : public CINFBase 
{
public:
	CINFInven(CAtumNode* pParent);
	virtual ~CINFInven();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void			Render();
	void			RenderOnCityBase();
	void			RenderSpi(int x, int y);
	virtual void	Tick();

	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProcOnCityBase(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void			SetAllIconInfo();
	void			SetSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo);
	void			SetItemSpi(int nSpi){ m_nItemSpi = nSpi; }
	void			AddItemSpi(int nSpi){ m_nItemSpi += nSpi; }
	void			SetItemSpiUniqueNumber(LONGLONG nUniqueNumber) { m_hyItemSpiUniqueNumber = nUniqueNumber; }
	int				GetItemSpi() { return m_nItemSpi; }
	LONGLONG		GetItemSpiUniqueNumber() { return m_hyItemSpiUniqueNumber; }
	void			DeleteSelectItem(int count);
	void			SendUseItem(ITEM_BASE* pItem);
	void			DeleteEnchantItem(UINT nUniqueNumber);
	void			SetItemInfo(INVEN_DISPLAY_INFO *pItemInfo, int x, int y);

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

	// 2007-02-12 by bhsohn Item ���� ���� ó��
	void			SetMultiSelectItem(INVEN_DISPLAY_INFO* pDisplayInfo);	
	int				GetInvenFreeSize();
	
	// 2007-03-09 by bhsohn ���� ���� â�� �κ� ����
	BOOL			IsInvenHave(int nItemNum);
	
	// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
	void			InitDeleteItemInfo();
	void			SetDeleteItemInfo(CItemInfo* pSelectItem);	
	void			RenderInvenBack(BOOL bShop = FALSE);

protected:
	int				GetScrollLine();
	int				GetScrollLineNumber(float nLength);
	float			GetScrollLineInterval(int nScrollLineNum);

	int				GetWearItemNum();
	void			InitInvenIconInfo();
	void			InitWearIconInfo();
	void			RenderWearPosition( int m_iWearPosition );
	
	void			SetSingleInvenIconInfo(CItemInfo* pItemInfo);
	void			SetSingleWearIconInfo(CItemInfo* pItemInfo);
	void			SendChangeWearWindowPos(int nWindowPosition);	


	// 2007-03-02 by bhsohn ���� ���� �߰� ����
	void			ClearMultiSeletItem();

	// 2007-08-07 by bhsohn ����Ŀ ������ �߰�
	void			DoModalChatAllInfluence(UID64_t		UniqueNumber);


public:
	CGameData *		m_pGameData;
	BOOL			m_bRestored;
	BOOL			m_bInvalidated;

	CINFImageEx	*	m_pEqip;
	CINFImageEx	*	m_pInven;
	CINFImageEx	*	m_pItemSl;
	CINFImageEx	*	m_pSpi;
	CINFImageEx	*	m_pScrollLine;
	CINFImageEx	*	m_pScrollButton;
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
	int					m_nInvenCurrentScroll;
	int					m_nInvenCurrentScrollPosition;
	BOOL				m_bScrollLock;
	POINT				m_ptMouse;

	CINFImageEx*			m_pWearPositionBlink;//�κ��ȿ� �ִ� �����ۿ� ���콺�� ���� ������ ����â ��ġ���� ������ �̹���
	int					m_iWearPosition;// ������ ����â�� ��ġ
	CRangeTime			m_tBlinkTime;

	CINFImageEx*			m_pSelectIcon;// ���콺 Ŭ���� ���� ���õ� ������ �̹���(reference)
	CItemInfo*			m_pSelectItem;// ������ ������(reference)
	
	// 2007-06-20 by bhsohn ������ ������, �޸� ���� ����
	//CItemInfo*			m_pDeleteItem;// ���� ������ ������
	structDelItemInfo		m_struDeleteItem;

//	int					m_nSelectItemWindowPosition;// Ŭ���� �������� Window������ ��ġ // 2006-06-21 by ispark, m_SelectItem->ItemWindowIndex �� ������ ����� ��

	BOOL				m_bEnchantState;
	CItemInfo*			m_pEnchantItem;

	int					m_nRenderMoveIconIntervalHeight;
	int					m_nRenderMoveIconIntervalWidth;

	INVEN_DISPLAY_INFO *m_pCurrentItemInfo;// ���� �������� �ִ� ������ ����
	BOOL				m_bSelectItem;
	BOOL				m_bSelectWearItem;
	BOOL				m_bMouseDownState;

	//////////////////////////////////////////////////////////////////////////	
	// ���� �κ��丮 �̹��� (�����̹����� ������ ��ȯâ ������ ���� ���� ���
	// ydkim 2005.11.04
	// ���� ������ �Լ����� �ʿ�
	
	CINFImageEx*			m_pInvenBase;
	CINFImageEx*			m_pInvenDirection[INVEN_ROTATION_STATE][INVEN_ROTATION_NUM];
	CINFImageEx*			m_pInvenRotationBase;

	int					m_nRotationState;
	int					m_nButtonState;
	
	BOOL				m_bTurnLeft;
	BOOL				m_bTurnRight;
	BOOL				m_bTurnUp;
	BOOL				m_bTurnDown;
	BOOL				m_bByPush[4];

	int					ProcessRotationUnitWnd(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	int					ProcessRotationUnit(POINT pt, UINT uMsg);
	void				SetByPushButton(BOOL bFlag);

	// 2007-02-12 by bhsohn Item ���� ���� ó��
	CINFImageEx*			m_pMultiItemSelImage;	

	//
	//////////////////////////////////////////////////////////////////////////
	// 2006-04-04 by ispark
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pMirrorVB;
	LPDIRECT3DTEXTURE9	m_pMirrorTexture;
	D3DXMATRIX			m_pMatInvenBackProj;	// 2006-04-04 by ispark, �κ��丮 ��� ��Ʈ����
	
};

#endif // !defined(AFX_INFINVEN_H__072A8788_5E57_4A56_B1FB_F569534E9105__INCLUDED_)

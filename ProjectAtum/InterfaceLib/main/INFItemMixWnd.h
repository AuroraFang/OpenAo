// INFItemMixWnd.h: interface for the CINFItemMixWnd class.
//
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-02-20 by bhsohn �ΰ��� ����â ó��
/// \date		2013-02-20 ~ 
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFITEMMIXWND_H__872742CD_5E6C_46B0_869F_C6F429B0A473__INCLUDED_)
#define AFX_INFITEMMIXWND_H__872742CD_5E6C_46B0_869F_C6F429B0A473__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CINFBase;
class CINFGroupImage;
class CINFEditBox;
class CINFAutoEditBox;
class CINFComboBoxEX;
class CINFListBox;
class CINFArenaScrollBar;

#define MIX_SEARCH_KIND_COUNT				3

// �ִ� ���μ�
#define	MAX_INGAME_SEARCH_X					(COUNT_ITEM_MIXING_SOURCE)
#define	MAX_INGAME_SEARCH_Y					5

// �ִ� 10���̱��� ��������
#define	MAX_SEARCH_PAGE						10

// �ִ� ������ ���� ��ư 
#define PAGE_BTN_FIRST						0
#define PAGE_BTN_PREV						1
#define PAGE_BTN_NEXT						2
#define PAGE_BTN_LAST						3
#define MAX_PAGE_BTN						4

// ���� �޴� Ŀ��
#define		WND_TYPE_MIXITEM						0		// ����â
#define		WND_TYPE_FACTORY						1		// Factoryâ

// Iconeũ�Ⱚ
#define		ICON_WIDTH							28
#define		ICON_HEIGHT							28
#define		ICON_CAPX							20
#define		ICON_CAPY							28

#define MAX_SCROLL_CNT						30						// �ִ� ������

class CINFItemMixWnd  : public CINFBase
{	
protected:
	CINFItemMixWnd() {}
	
public:	
	CINFItemMixWnd(CAtumNode* pParent);
	virtual ~CINFItemMixWnd();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	InitWndData();
	
	virtual void	Tick();
	void			Render();	

	DataHeader* FindResource(char* szRcName);

	void			RenderCommonCtrl();	
	void			RenderSearchItem(int nPage, int nScroll);		
	void			RenderPageCnt();	
	void			RenderCtrl();		// 	

	BOOL IsShowWnd();
	virtual void ShowWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum, POINT *tMixWndPos=NULL);
	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	virtual int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual int OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void UpdateBtnPos();
	virtual int OnCilckSelectItem(POINT pt);		// �˻��׸� Ŭ��
	virtual INT GetCntInven_ItemNum_To_AllItemCnt( INT nItemNum );

	BOOL IsMouseCaps(POINT ptPos);
	

	// �˻���ư ó��
	void OnClickSearchItem();

	int OnClickResultNameSearch(int nItemCnt);		// ����� �˻�[�̸����� �˻�]
	int OnClickMaterialNameSearch(int nItemCnt);	// [���]�� �˻�

	void UpdateSearchEditList();		// [�ڵ��ϼ�] ����Ʈ�� �������ش�.

	// �˻� Ÿ��
	int	 GetSearchType() {return m_nSearchType;}

	// �˻� Ÿ��
	void SetSearchItemNum(INT nSearchItemNum) {m_nSearchItemNum = nSearchItemNum;}
	INT GetSearchItemNum() {return m_nSearchItemNum;}

	// ������ ���� ǥ��
	void SetTooltipItemInfo(INT	nItemNum, int x, int y, int nLinkItem);

	void FindTooltipItemInfo_POS(POINT pt, BOOL* pTarget, int *nFindPosX, int *nFindPosY);
	INT GetTooltipItemInfo_ItemNum(int nPage, int nScroll, BOOL bTarget, int nFindPosX, int nFindPosY, int* pLinkItem);

	INT GetTooltip_Target_ItemInfo_ItemNum(int nPage, int nScroll, int nFindPosX, int nFindPosY);

	void InitItemListScroll();
	void UpdateItemListScroll(int nCurrentPage, int nStartPage);

	void InitPagePosList();
	
	int OnCilckPageCnt(POINT pt);
	

	void OnClickPageBtn(int nPageMode);

	POINT GetBkSize();	
	void SetShowItemNumShowWnd(BOOL bShow, int nShowItemNum, UID64_t uItemUniNum);

protected:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					       
	CINFGroupImage*		m_pImgBackg;
#endif
	CINFAutoEditBox*		m_pEditItemName;		// Edit�ڽ�
	CINFImageBtn*		m_pSearchBtn;			// ��ư
	//CINFComboBoxEX*		m_pComboKindEx;
	CINFListBox*		m_pLBMixKind;
	CINFArenaScrollBar*		m_pScroll;
	

	CD3DHanFont*		m_pItemListFont; // ������ ��Ʈ

	CD3DHanFont*		m_pConuntFont; // ������ ��Ʈ

	CINFImageBtn*		m_pCloseBtn;

	char		m_strMixKind[MIX_SEARCH_KIND_COUNT][MAX_PATH];		// �˻� ����

	BOOL		m_bShowWnd;
	
	POINT					m_ptBkPos;	// ������ ������ġ
	
	// ������ �̵� ���� ó��
	BOOL					m_bMove;
	POINT					m_ptCommOpMouse;

	int						m_nSearchType;
	INT						m_nSearchItemNum;

	char					m_szPrice[64];
	vector<CItemInfo*>		m_vecSource;
	vector<CItemInfo*>		m_vecTarget;
	BOOL					m_bShowTarget;

	int						m_nEditElementIndex;

	vector<structResultMixingInfo>		m_vecMaterialList;						// �˻����
	vector<structResultMixingInfo>		m_vecMixingItemList;		// ��� ���
	

	vector<structResultMixingInfo>		m_vecRenderItemList;			// ������ �� ����

	// ������ ���ڷ� �˻�
	RECT					m_rcShowItem;


	// ���� ����Ʈ
	RECT					m_rcSearchItem[MAX_INGAME_SEARCH_Y][MAX_INGAME_SEARCH_X];

	// ����� ����Ʈ
	RECT					m_rcTargetItem[MAX_INGAME_SEARCH_Y];

	INT						m_nMaxShowItemLen;// �ִ� ���μ� // ������ �ִ� ���μ�
	int						m_pPageCntBtnPosY;
	int						m_nDiffScrollHeight;

	//////////////////// ������ ���� ó�� ////////////////////
	// ������ ��ġ
	POINT					m_ptPagePosList[MAX_SEARCH_PAGE][MAX_SEARCH_PAGE];

	int						m_nStartPage;		// ���� �׸��� ���� ������	
	int						m_nCurrentPage;		// ���� ���� �ִ� ������
	int						m_nMaxScrollPage;	// �ִ� ������
	
	CINFImageBtn*			m_pPageBtn[MAX_PAGE_BTN];		// 	

	//////////////////// �����׸���� ó�� ////////////////////
	RECT				m_rcSearchList[MAX_INGAME_SEARCH_Y];
	CINFImageEx*		m_pSearchIconBk;		
	

	CINFImageEx*		m_pSearchListBk;		// ItemList���
	CINFImageEx*		m_pSearchListSel;		// ������ ItemList���

	int					m_nSelItem;

	INT					m_nShowItemNum;				
	UID64_t				m_uItemUniNum;
	INT					m_WndType;

	char				m_nBkImageGroup[MAX_PATH];
};
#endif // !defined(AFX_INFITEMMIXWND_H__872742CD_5E6C_46B0_869F_C6F429B0A473__INCLUDED_)

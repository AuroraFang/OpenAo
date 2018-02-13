// INFItemMixWnd.h: interface for the CINFArmorCollectWnd class.
//
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		
/// \author		// 2013-05-28 by bhsohn �Ƹ� �÷��� �ý���
/// \date		2013-05-28 ~ 
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARMOR_COLLECT_WND_H__872742CD_5E6C_46B0_869F_C6F429B0A473__INCLUDED_)
#define AFX_ARMOR_COLLECT_WND_H__872742CD_5E6C_46B0_869F_C6F429B0A473__INCLUDED_

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
#define	MAX_ARMOR_X							4
#define	MAX_ARMOR_Y							3

// Iconeũ�Ⱚ
#define		BIGICON_WIDTH							84
#define		BIGICON_HEIGHT							58
#define		ICON_CAPX							85
#define		ICON_CAPY							52

#define MAX_SCROLL_CNT						30						// �ִ� ������

// �ִ� ��þ ��
#define		MAX_ARMOR_COLLECT_ENCHANT		10

class CINFArmorCollectWnd  : public CINFBase
{		
public:	
	CINFArmorCollectWnd(CAtumNode* pParent);
	virtual ~CINFArmorCollectWnd();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	InitWndData();

	virtual void	Tick();
	void			Render();	

	DataHeader* FindResource(char* szRcName);

	void			RenderCommonCtrl();			

	void			RenderInitArmor();
	void			RenderCurrentArmor();
	void			RenderApplyArmor();
	
	void			RenderArmorList();		// �Ƹ� ���� ������
	void			RenderHaveArmor(int nStartX, int nStartY, CINFImageEx* pArmorIcon, struArmorCollectionInfo* pArmorInfo);		// �Ƹ� ���� ������
	void			RenderNotHaveArmor(int nStartX, int nStartY, CINFImageEx* pArmorIcon, struArmorCollectionInfo* pArmorInfo);		// �Ƹ� ���� ������

	void			ConvertSecond_To_DayHourMin(INT nSecond, int *pDay, int *pHour, int *pMin);

	BOOL IsShowWnd();
	virtual void ShowWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum, POINT *tMixWndPos=NULL);
	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	virtual int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual int OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void UpdateBtnPos();
	virtual int OnCilckSelectItem(POINT pt);		// �˻��׸� Ŭ��	
	virtual int OnCilckGearTab(POINT ptPos);

	BOOL IsMouseCaps(POINT ptPos);
	
	// �˻� Ÿ��
	void SetSearchItemNum(INT nSearchItemNum) {m_nSearchItemNum = nSearchItemNum;}
	INT GetSearchItemNum() {return m_nSearchItemNum;}

	// ������ ���� ǥ��
	void SetTooltipItemInfo(INT	nItemNum, int x, int y, int nLinkItem);

	void FindTooltipItemInfo_POS(POINT pt, int *nFindPosX, int *nFindPosY);
	struArmorCollectionInfo* GetTooltipItemInfo_ItemNum(int nScroll, int nFindPosX, int nFindPosY, int* pLinkItem);

	INT GetTooltip_Target_ItemInfo_ItemNum(int nPage, int nScroll, int nFindPosX, int nFindPosY);

	void InitItemListScroll();
	void UpdateItemListScroll(int nCurrentPage, int nStartPage);

	
	POINT GetBkSize();	
	void SetShowItemNumShowWnd(BOOL bShow, int nShowItemNum, UID64_t uItemUniNum);
	
	void ResotreAllArmorIcon();		// �Ƹ� ������ ��ü ����
	void InvalidateAllArmorIcon();		// �Ƹ� ������ ��ü ����
	void DeleteAllArmorIcon();
	CINFImageEx* GetArmorIcon(INT nUnitCollectType, INT nItemNum);

	INT GetSelUnitCollectType(){return m_nSelUnitCollectType;}	// 2013-06-12 by ssjung �Ƹ� �÷��� ���� ǥ��

protected:
	void OnClickApplyArrmor();
	void OnClickInitArmor();

	void RenderGearTab();

	
	INT GetSelectArmorShapeNum(POINT ptSelectPos);
	INT GetSelectArmorItemNum(POINT ptSelectPos);


protected:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					       
	CINFGroupImage*		m_pImgBackg;
#endif	
	CINFArenaScrollBar*		m_pScroll;
	

	CD3DHanFont*		m_pItemListFont; // ������ ��Ʈ

	CD3DHanFont*		m_pConuntFont; // ������ ��Ʈ

	CINFImageBtn*		m_pCloseBtn;

	CINFImageBtn*		m_pArmorApplyBtn;
	CINFImageBtn*		m_pArmorInitBtn;

	char		m_strMixKind[MIX_SEARCH_KIND_COUNT][MAX_PATH];		// �˻� ����

	BOOL		m_bShowWnd;
	
	POINT					m_ptBkPos;	// ������ ������ġ
	
	// ������ �̵� ���� ó��
	BOOL					m_bMove;
	POINT					m_ptCommOpMouse;
	
	INT						m_nSearchItemNum;

	char					m_szPrice[64];
	vector<CItemInfo*>		m_vecSource;
	vector<CItemInfo*>		m_vecTarget;
	BOOL					m_bShowTarget;

	int						m_nEditElementIndex;

	vector<structResultMixingInfo>		m_vecMaterialList;						// �˻����
	vector<structResultMixingInfo>		m_vecMixingItemList;		// ��� ���
		


	// ������ ���� �Ƹ� 
	RECT					m_rcInitItem;
	
	// ���� ���� �Ƹ�
	RECT					m_rcShowItem;

	RECT					m_rcApplayItem;


	// ���� ����Ʈ
	RECT					m_rcArmorItem[MAX_ARMOR_Y][MAX_ARMOR_X];

	INT						m_nMaxShowItemLen;// �ִ� ���μ� // ������ �ִ� ���μ�
	int						m_pPageCntBtnPosY;
	int						m_nDiffScrollHeight;

	//////////////////// �����׸���� ó�� ////////////////////	
	CINFImageEx*		m_pSearchIconBk;		
	

	CINFImageEx*		m_pSearchListBk;		// ItemList���
	CINFImageEx*		m_pApplyItem;			// ����� ItemList���
	CINFImageEx*		m_pSearchListSel;		// ������ ItemList���
	CINFImageEx*		m_pUseSel;
	

	INT					m_nShowItemNum;				
	UID64_t				m_uItemUniNum;	

	char				m_nBkImageGroup[MAX_PATH];

	map<INT, CINFImageEx*>		m_mapBigIconInfo[MAX_COLLECT_UNIT];		// BigIcon

	CINFImageEx*		m_pGearTabImg[MAX_COLLECT_UNIT];		
	RECT				m_rcGearTab[MAX_COLLECT_UNIT];

	INT				m_nSelUnitCollectType;

	POINT				m_ptSelectPos;
	
	CINFImageEx*		m_pLackItemImage ;
};
#endif // !defined(AFX_ARMOR_COLLECT_WND_H__872742CD_5E6C_46B0_869F_C6F429B0A473__INCLUDED_)

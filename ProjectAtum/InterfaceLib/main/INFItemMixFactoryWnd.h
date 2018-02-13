// INFWorldRankWnd.h: interface for the CINFItemMixFactoryWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFITEMMIXFACTORYWND_H__C2EF2BD5_6F6C_4AAE_80DC_AAC6DC21DB39__INCLUDED_)
#define AFX_INFITEMMIXFACTORYWND_H__C2EF2BD5_6F6C_4AAE_80DC_AAC6DC21DB39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <vector>
#include "AtumDatabase.h"

#include "INFBase.h"
#include "INFItemMixWnd.h"
#include "WorldRankManager.h"


// �� ��ǥ �ִ�
#define		MAX_WR_INFO_X				10

#define		MAX_FACTORY_ANI				10

#define		MAX_MIXSUCCESS				2

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�ΰ��� ���丮
/// \author		// 2013-02-20 by bhsohn �ΰ��� ����â ó��
/// \date		2013-02-20 ~ 
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
class CD3DHanFont;
class CINFListBox;
class CINFGroupImage;
class CINFItemMixWnd;
class CINFNumEditBox;

struct struMakingItemInfo
{
	ITEM_W_COUNT struSourceItem[COUNT_ITEM_MIXING_SOURCE];	
	int	nItemCount;
};

class CINFItemMixFactoryWnd  : public CINFItemMixWnd
{
public:
	CINFItemMixFactoryWnd(CAtumNode* pParent);
	virtual ~CINFItemMixFactoryWnd();	

	virtual void InitWndData();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	// Tick���� �Լ���
	virtual void	Tick();
	void	TickMakingAni();
	void	TickMakeSuccess();
	void	TickMakeFail();
	
	void			Render();	

	void			RenderSourceItem();	
	void			RenderFactoryTargetItem();	
	void			RenderItemInfo(CItemInfo* pItem, int nIconStartX, int nIconStartY, CD3DHanFont*	 pFont, INT nCurrentCount=-1, BOOL bCenter=FALSE);
	CINFIcon*		RenderMixSourceItemInfo(INT nItemNum, INT nCurrentCount, int nIconStartX, int nIconStartY, CD3DHanFont* pFont, BOOL bCenter=FALSE);	
	void			RenderRollingImage();


	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	virtual int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual int OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void UpdateBtnPos();	
	virtual int OnCilckSelectItem(POINT pt);		// �˻��׸� Ŭ��
	
	void SearchItemToSourceItem(ITEM_W_COUNT	SourceItem[COUNT_ITEM_MIXING_SOURCE]);
	void InitSearchItemSourceItem();

	void ShowWnd(BOOL bShow, INT nShowItemNum, UID64_t uItemUniNum, POINT *tMixWndPos=NULL);

	void OnClickAllMake();
	void OnClickFactoryMake();
	void OnClickCancelMake();
	void OnClickInitMake();

	void OnClickCntBtn(BOOL bUpBtn);
	
	void InitFactoryAniValue(BOOL bPlay);
	void InitFactorySuccessValue(BOOL bPlay);
	void InitFactoryFail(BOOL bPlay);

	void SetTargetMixInfo(ITEM_MIXING_INFO* pTargetMixInfo);

	void InitData(BOOL bItemsResult);
	void ItemMixItemsResult(MSG_FC_ITEM_MIX_ITEMS_RESULT* pMsg);

	// ������ ���� ����
	void SendMakeItem();
	BOOL IsPossbileSearchMixItem();

	INT GetMakeNumCnt();	// ������ ������ ������
	INT GetSendMakeNumCnt();

	void SetMakeNumCnt(INT nCnt);

	void UpdateMixPrice();		// ���հ��� ����
	void UpdateMixTarget();

	void MakeItem_InvenInputMode();
	
	void MakeItem_SearchSelectMode();
	void MakeItem_SearchSelectMode_CountItem(int nItemCount);
	void MakeItem_SearchSelectMode_NonCountItem(int nItemCount);

	CItemInfo* FindItemInInventoryByItemNum_MixPossible(INT nItemNum);
	void InvenToSourceItem(ITEM_W_COUNT m_struSearchSourceItem[COUNT_ITEM_MIXING_SOURCE], int nItemCount);

	// Making����Ʈ�� �ִ� ������ �ϳ��� ������.
	void ReadyMakeItemInfo();	// ���� ������ ���� ���� �����͸� ��������.
	
	BOOL StartListMakingItem();	
	void ClearListMakingItem();	

	void ErrorMixItem(Err_t ErrorCode);

	INT GetTargetVecItemCount(INT nItemNum);	

	void SetFinalResultTargetUniqueNum(UID64_t	uFinalResultTargetUniqueNum);

	void UpdateMakeBtn();

protected:
	RECT	m_rcFactorySourceItem[MAX_INGAME_SEARCH_X];
	RECT	m_rcFactoryTargetItem;

	CINFImageBtn*		m_pFactoryAllBtn;
	
	CINFImageBtn*		m_pFactoryMakeBtn;
	CINFImageBtn*		m_pFactoryCancelBtn;
	CINFImageBtn*		m_pFactoryInitBtn;
	CINFImageBtn*		m_pFactoryGetItemBtn;

	CINFGroupImage*		m_pFactoryMakeGroup;

	CINFImageBtn*		m_pUpCntBtn;
	CINFImageBtn*		m_pDownCntBtn;

	CINFImageEx	*		m_pRollingImage;		// ������ ������ �̹���

	CINFImageEx	*		m_pLackItemImage;			// ������ ����
	
	////////////////////// ���ռ��� ����  //////////////////////
	CINFImageEx	*		m_pSuccessImage[MAX_MIXSUCCESS];		
	CINFImageEx	*		m_pFailImage;
	

	CINFNumEditBox*		m_pNumEditBox;

	CAtumNode*			m_pParent;


	////////////////////// ������ ���� ��� //////////////////////
	int					m_nMixMode;
	
	////////////////// �������� �ִϸ��̼� ���� ���� //////////////////	
	int					m_nAniPlayIndex;
	float				m_fAniElapseTime;
	float				m_fAllAniElapseTime;
	RECT				m_rcAniRect[MAX_FACTORY_ANI];

	ITEM_MIXING_INFO	m_struWillResultTargetMixInfo;	
	
	UID64_t				m_uFinalResultTargetUniqueNum;	// ���� ��� ������

	////////////////// ������ �������� //////////////////	
	ITEM_W_COUNT		m_struSearchSourceItem[COUNT_ITEM_MIXING_SOURCE];	

	////////////////// ���� �ִϸ��̼� ���� ���� //////////////////	
	int					m_nSuccessIndex;
	float				m_fSuccessElapseTime;
	float				m_fAllSuccessElapseTime;

	////////////////// ���� �̹��� ǥ��//////////////////	
	float				m_fAllFailElapseTime;

//	vector<ITEM_GENERAL>		m_vecTargetItem;

	list<struMakingItemInfo>	m_listMakingItem;

	INT					m_nTargetMakingCount;
		
};

#endif // !defined(AFX_INFWORLDRANKWND_H__C2EF2BD5_6F6C_4AAE_80DC_AAC6DC21DB39__INCLUDED_)

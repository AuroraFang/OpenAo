// INFLuckyMachine.h: interface for the CINFLuckyMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFLUCKYMACHINE_H__C96FAC40_4C48_40C6_A5C7_E6796220621A__INCLUDED_)
#define AFX_INFLUCKYMACHINE_H__C96FAC40_4C48_40C6_A5C7_E6796220621A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"
// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
#include "INFAUTOSPRITEBTN.h"
// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����

#define	LUCKY_ITEM_LIST_NUM							7
#define	LUCKY_ITEM_LIST_NUM_LINE					3
#define LUCKY_ITEM_POS								3		// ��� ���� ��ġ

enum {
	LUCKY_STATE_READY,
	LUCKY_STATE_RULLET,
	LUCKY_STATE_PRIZE
};

// ���� ���� ���� ��� �ִ� ��
#define			MAX_SLOT_RULLET_TYPE				5

// ���� ���� ���� �ִ� ����
#define			MAX_SLOT_RULLET_STEP				10

struct ST_TIME_GAP
{
	FLOAT		fHoldTime;				// ��ĭ ������ �ð�.
	FLOAT		fSlotTime;				// ������ ���ư����ϴ� �ð� 
	int			nSlotStep;				// ������ ���� ��
	BOOL		bCompleteStep;			// �Ϸ� �Ǵ� ����
};

struct struLuckyMachineScenario
{
	ST_TIME_GAP stRulTime[MAX_SLOT_RULLET_STEP];
};

struct ST_ICON_ITEM_NUMBER{
	INT			nIconNum;
	INT			nItemNum;
};

class CINFImage;
class CItemInfo;

// 2009-03-04 by bhsohn ��Ű �ӽ� ������
// ��Ű �ӽ� ����
#define		LUCKY_MAINSTEP_SELECT_SHOP			0	// ��Ű �ӽ� ���� ����
#define		LUCKY_MAINSTEP_SELECT_GAMING		1	// ��Ű �ӽ� ������
#define		MAX_LUCKY_MAINSTEP					2	// 

// ��Ű �ӽ� ������
#define		LUCKY_SELECT_TYPE_MINI				1	// �̴� Ÿ�� ��Ű �ӽ�
#define		LUCKY_SELECT_TYPE_WEAPON			2	// �����ӽ�	 
// 2010. 05. 31 by jskim �ű� ���� �ӽ� ����
#define		LUCKY_SELECT_TYPE_NORMAL			3	// �ƸӸӽ�	 
#define		LUCKY_SELECT_MAX				    4
//end 2010. 05. 31 by jskim �ű� ���� �ӽ� ����	


// ���� ��� �̹��� ��
#define		MAX_SHOP_BG							2


struct structNPCList
{	
	// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
	//CINFImageBtn*		pInfImageBtn;
	CINFAUTOSPRITEBTN*	pInfImageBtn;
	// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
	CINFImageEx*			pInfShopImage[MAX_SHOP_BG];
	BOOL				bReStore;	
	LUCKY_MACHINE_OMI	struLuckyMachineOmi;
};

#define CHG_LUCKY_TITLE_IMAGE			2		// ��Ű �ӽ� ���� �ε���
#define MAX_LUCKY_TITLE_IMAGE			4		// ��Ű �ӽ� �ִ� ��� �̹���

// ��Ű ��� ��
#define	MAX_LUCKY_YOULUCKY			2

// end 2009-03-04 by bhsohn ��Ű �ӽ� ������



// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����

#define LUCKY_MACHINE_BTN_FRAME			1000
#define LUCKY_MACHINE_BTN_FRAME_COUNT	4

// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����


class CINFLuckyMachine : public CINFBase  
{
public:	
	virtual ~CINFLuckyMachine();

	CINFLuckyMachine(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	void			TickSlot(int i_nSlot,float i_fElapsedTime);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void		OnCloseInfWnd();
	
	INT			FindSearchItem(INT i_nSlotNum, INT i_nMysteryNum);				// �Է¹��� �������� ������ �ִ� �������� ���Ϳ� �־���.
	void		ClearSearchItem();
	void		TakeItem(LUCKY_ITEM_SIMPLEINFO i_sItemInfo);
	void		GambleButtonOK();
	
	void		ChangeState(int i_nState);
	void		ResetData();
	void		Init();
	void		SetPrizeItem(ITEM_GENERAL i_Item);
	void		SetPrizeItem(MSG_FC_TRADE_UPDATE_ITEM_COUNT *pMsg);
	void		SetDeletePrizeItem ( const UID64_t a_ItemUniqueNumber );
	void		PrizeItemGiveMe();
	BOOL		IsCloseLucky();
	BOOL		SlotSpeedStepUp(int i_nSlot);	
	UID64_t		GetSelUID();

	// 2009-03-04 by bhsohn ��Ű �ӽ� ������
	void		SetLuckyMechineStep(int nLuckyMechineStep);
	void		EnterMachineShop(UINT i_uBuildingIndex);	
	void		ResetLuckyMachineInfo();

	BOOL		IsInfluenceOneShopMode();

	BOOL		IsPossibleUpLoadCoin(ITEM* i_pItem);
	void		UpLoadCoin(ITEM_GENERAL* i_pItem);
	CItemInfo*	GetConinItemInfo();
	
	void		ClearConinItemInfo();
	void		RenderYouLucky();
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������

	// 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
	void		ErrorLuckyMechineInvenFull();
	// end 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��

	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	LUCKY_MACHINE_OMI GetLuckyMachineInfo() { return m_struSelLuckyMachine; }
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�



private:
	// 2008-12-29 by bhsohn ��Ű �ӽ� ���� ����Ʈ �߰�
	BOOL		IsShowLucySuccessEffect(INT	i_MysteryItemDropNum, int i_nMysteryNum);
	void		ShowLuckySuccessEffect();
	
	// 2009-03-04 by bhsohn ��Ű �ӽ� ������
	// �귿 ���� �ʱ�ȭ
	void		InitRulType();

	void		CreateNPCListBtn();
	void		InitDeviceObjectsNPCListBtn();
	void		RestoreDeviceObjectsNPCListBtn();
	void		InvalidateDeviceObjectsNPCListBtn();
	void		DeleteDeviceObjectsNPCListBtn();	

	BOOL		IsMyBelignece(LUCKY_MACHINE_OMI* pLuckyMechineOmi);	// ���� ���� ��Ű �ӽ��̳�?
	BOOL		IsHasNPCBtn(LUCKY_MACHINE_OMI* pLuckyMechineOmi);	
	void		ShowNPCListBtn(BOOL i_bShow);
	BOOL		RefreshNPCListBtnPos();
	void		RefreshLuckyMachineCtrl();
	void		OnClickSelectNPCShop(LUCKY_MACHINE_OMI	*pSelLuckyMachine);
	
	
	void		RefreshNPCListBtnStat();

	// ������ ��Ű �ӽ�	
	void		TickLuckyStepStepSelectShop();
	void		TickLuckyStepStepSelectGaming();

	// ������ ��Ű �ӽ�		
	void		RenderLuckyStepStepSelectShop();
	void		RenderLuckyStepStepSelectGaming();

	void		RenderLuckyMachineSlot(int nSlotNum);

	int			WndProcCommon(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	int			WndProcLuckyStepStepSelectShop(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcLuckyStepStepSelectGaming(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ��Ű �ӽ� ����
	void		OnClickNPCListPos(int i_nStep);
	void		OnClickPrevStepBtn();
	
	
	// ��� �귿 �Ϸ�
	BOOL		IsAllRuletDone();
	// �ø��� �ִ� ��ȭ��?
	// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	//BOOL		IsPossibleUpLoadCoin(CItemInfo* i_pItem, int i_nMachineNum);

	// �Ѱ��� ������ �Ϸ� �Ƴ�
	BOOL		IsSlotDone(int i_nSlot);

	// 
	void		UpdateSuccessSlot();

	CINFImage*	GetShopImageBk(int nEffectBG);

	void		UpdateCurrentSlotPos(int i_nSlot);

	BOOL		GetInfluenceOneShop(LUCKY_MACHINE_OMI* o_pSelLuckyMachine);

	void		TickYouLucky();
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������
protected:

	//INT						m_nTakeItem;
	vector<LUCKY_ITEM_SIMPLEINFO>		m_vecTakeItem;
	int						m_nState;
	vector<ITEM_GENERAL>	m_vecPrizeItem;
	vector<MSG_FC_TRADE_UPDATE_ITEM_COUNT> 	m_vecPrizeItemCount;
	vector<ST_ICON_ITEM_NUMBER>		m_vecIconNum[LUCKY_ITEM_LIST_NUM_LINE];		// ������ ������ ����.
	
	struLuckyMachineScenario		m_stRulTime[LUCKY_ITEM_LIST_NUM_LINE][MAX_SLOT_RULLET_TYPE];		// ������ ���ư��� �ð��� ���س��� ��.		
	BOOL					m_bSlotDone[LUCKY_ITEM_LIST_NUM_LINE];		// ��÷�Ǿ� ������ �����ִ°�?
	int						m_nSlotStep[LUCKY_ITEM_LIST_NUM_LINE];		// ������ ȸ�� �ӵ�.

	// �Ѱ� ���Ը��� ���� �ð�
	float					m_fTimeHold[LUCKY_ITEM_LIST_NUM_LINE];		
	
	// �Ѱ� ���� ���� �ð� 
	float					m_fTimeOneStepSlot[LUCKY_ITEM_LIST_NUM_LINE];		
	// �� ������ �Ϸ� ����
	BOOL					m_bCompleteStep[LUCKY_ITEM_LIST_NUM_LINE];

	// ���Կ� ó������ �������� ī��Ʈ.
	int						m_nSlotStartPosIdx[LUCKY_ITEM_LIST_NUM_LINE];			

	// ���� Ÿ��
	int						m_nSlotType[LUCKY_ITEM_LIST_NUM_LINE];			

	// ��ü ���� �ð�
	float					m_fTimeAllTimeStepSlot[LUCKY_ITEM_LIST_NUM_LINE];		

	// �Ϸ� ���� ��ġ
	int						m_nSuccessSlot[LUCKY_ITEM_LIST_NUM_LINE];		
	
	
	CINFImageEx*				m_pImgQuestionMark;				// ����ǥ 
//	CINFImage*				m_pImgArrow;					// ȭ��ǥ.
	
	CINFImageEx*				m_pImgMainBG[LUCKY_SELECT_MAX];						// ���� ���.

	CINFImageEx*				m_pImgEffectBG[MAX_LUCKY_TITLE_IMAGE];				// ���� ���� ���.

	CINFImageEx*				m_pImgEffBG[LUCKY_SELECT_MAX][7];

	CINFImageEx*				m_pImgYouLuckyBG[MAX_LUCKY_YOULUCKY];				// ���� ���

	// 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�
	CINFImageEx*				m_pImgDisableItem;				// ��� ������ �׸���
	//end 2010. 05. 12 by jskim �ű� ��Ű �ӽ� �߰���� - ��Ű�ӽſ��� �޴� �����۸� �����ֱ�

	int						m_nEffectBG;					// ��� ������.
	int						m_nLuckyEffectBG;				// ��� ������.
	float					m_fTimeEffectBG;				// ��� ������ ���� �ð�.
	float					m_fLuckyTimeEffectBG;			// ��� ������ ���� �ð�.
	
	CINFImageBtn*			m_pBtnGamble;					// �̱� ��ư.
	CINFImageBtn*			m_pBtnTake;						// �ޱ� ��ư.

	// 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����
	CINFImageBtn*			m_pBtnGamble1;					// �̱� ��ư_���Ǹӽ�.
	CINFImageBtn*			m_pBtnTake1;					// �ޱ� ��ư_���Ǹӽ�.
	//end 2010. 06. 14 by jskim ���Ǹӽ� ��ư UI ����

	// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	CINFImageEx*				m_pAME_slot[2];
	float					m_nAME_EffectNum[LUCKY_ITEM_LIST_NUM_LINE];
	// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����

	CItemInfo*				m_pCoinItemInfo;
	ITEM_GENERAL			m_pSelectCoin;

	BOOL					m_bPrizeItem;					// ���� �������� �ִ°�?

	// 2009-03-04 by bhsohn ��Ű �ӽ� ������
	// �ڷ� ���� ��ư
	CINFImageBtn*			m_pPrevStepBtn;			// �ڷ� ���� ��ư
	
	// ��Ű �ӽ� ���� ����
	CINFImageEx*				m_pImgSelctShopBK;
	CINFImageBtn*			m_pSelectShopPrevBtn;			// ��Ű �ӽ� ���� ��
	CINFImageBtn*			m_pSelectShopNextBtn;			// ��Ű �ӽ� ���� ����
	vector<structNPCList*>	m_vecstruShopList;				// NPC����Ʈ		

	int						m_nLuckyMechineStep;			// ���� //LUCKY_MAINSTEP_SELECT_TYPE	

	int						m_nLuckySelectStartIdx;			// ���� ���� ������

	LUCKY_MACHINE_OMI		m_struSelLuckyMachine;			// ������ ��Ű ����
	int						m_nLoadNpcListInfluenceType;

	// 2009-05-13 by bhsohn ��Ű ���� ���� ����
	UINT					m_uLoadBuildingIndex;
	// end 2009-05-13 by bhsohn ��Ű ���� ���� ����

	BOOL					m_bInfluenceOneShopMode;
	UINT					m_uBuildingIndex;
	
	
	// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	BOOL					m_bYouLucky;	
	float					m_fYouLuckyViewTime;
	DWORD					m_dwYouLuckyAlpha;
	
	// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	// end 2009-03-04 by bhsohn ��Ű �ӽ� ������


	// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����

	DWORD					m_dwLastFrameTime;
	int						m_nCurFrame;

	// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����

	// 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
	ITEM_GENERAL			*m_pItemCoinItemTmp;
	// end 2009-05-18 by bhsohn ��Ű�������� �κ�Ǯ�ϋ��� ������ ó��
};

#endif // !defined(AFX_INFLUCKYMACHINE_H__C96FAC40_4C48_40C6_A5C7_E6796220621A__INCLUDED_)

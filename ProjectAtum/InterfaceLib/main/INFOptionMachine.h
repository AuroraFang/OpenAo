// INFOptionMachine.h: interface for the INFOptionMachine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPTIONMACHINE_H__CDAC2CA9_6BE1_4B0A_B4BD_FDA9ED7DADEB__INCLUDED_)
#define AFX_INFOPTIONMACHINE_H__CDAC2CA9_6BE1_4B0A_B4BD_FDA9ED7DADEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"

class CINFImage;
class CINFImageEx;			// 2011. 10. 10 by jskim UI�ý��� ����
class CItemInfo;

#define OPTION_ITEM_LIST_NUM							5			// ȭ�鿡 �����ִ� �ɼ� ��
#define OPTION_ITEM_LIST_CENTER							2			// �ɼ� �ӽ��� �߰�

#define OPTION_SELECT_ITEM								3			// ����, ����, �Ƹ�

#define OPTION_ITEM_LIST_FIX							2			// ����, ���� ���

#define OPTION_ITEM_PRE_FIX								0			// ����
#define OPTION_ITEM_SUF_FIX								1			// ����
#define OPTION_TARGET_ITEM								2			// �Ƹ�, ����


#define SUPER_OPTION_ITEM								0			// ���� �ɼǸӽ� 
#define NORMAL_OPTION_ITEM								1			// �Ϲ� �ɼǸӽ�

enum			
{
	OPTION_STATE_READY,												// ���
	OPTION_STATE_RULLET,											// ����
	OPTION_STATE_PRIZE												// ��� 
};

#define	MAX_RULLET_PATTERN								2			// ������ ���ư��� ����

enum
{
	PATTERN1,
	PATTERN2,
	PATTERN3,
};

#define	MAX_SLOT_SPEED_STEP								5			// ������ ���ǵ� �ܰ�

#define	OPTION_SPEED_NORMAL								3			// �⺻
#define OPTION_SPEED_SLOW								1			// ����
#define	OPTION_SPEED_FAST								5			// ���� 

#define	STEP_SPEED										0.02f		
#define TRICK_SPEED										1.0f
		
struct ST_ITEM_INFO
{
	int		nItemCode;					// �ڵ� ��ȣ 
	char*	nItemName;					// ������ �̸�
	int		nItemPosition;				// ������ ��ġ 
};

struct ST_RENDER_ITEM
{
	char*					nItemName;
	int						nItemPosition;				// ������ ��ġ 
	int						nImageType;
};

struct ST_SLOT_INFO
{
	ST_SLOT_INFO()
	{
		fSlotTime		= NULL;
		fAllSlotTime	= NULL;
		bCompleteStep	= FALSE;
		nSlotNum		= NULL;
		nSpeed			= OPTION_SPEED_NORMAL;
		nSpeedCourse	= NULL;
		nTrick			= NULL;
		nItemPosition	= NULL;
		nUseSlot		= FALSE;
	}
	vector<ST_ITEM_INFO*> nItemInfo;

	FLOAT		fSlotTime;				// ��ĭ ������ �ð�
	FLOAT		fAllSlotTime;			// ������ �� ������ �ð� 

	int			nSpeed;					// ���� ���ǵ� �ܰ�
	int			nSpeedCourse;			// �����̴� ����
	int			nItemPosition;			// ���� �������� ��ġ
	BOOL		bCompleteStep;			// �Ϸ� üũ
	BOOL		bChosePattern;			// ���� üũ 
	int			nSlotNum;
	int	        nTrick;
	BOOL		nUseSlot;			// ������ ����ϴ���
};

class CINFOptionMachine : public CINFBase
{
public:
	virtual ~CINFOptionMachine();
	CINFOptionMachine(CAtumNode* pParent, BUILDINGNPC* pBuilding);

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Render();
	virtual void Tick();
	void		TickSlot(float i_fElapsedTime);
	BOOL		IsAllRuletDone();
	void		RenderSlot();
	void		RenderCard(CItemInfo* nSelectItem, int nPosition);
	
	float		GetStepSpeed(int Step);
	void		OptionSlotDraw(int x, int y, ST_RENDER_ITEM* RenderItem, int nPosition, BOOL slotDark = FALSE);
	void		ChangeState(int i_nState);
	void		ChangePattern(ST_SLOT_INFO* i_nInfo);
	void		ChangeSpeed(ST_SLOT_INFO* i_nInfo);
	void		SlotTranslate(ST_SLOT_INFO* i_nInfo);
	BOOL		TrickPattern(ST_SLOT_INFO* i_nInfo);
	int			GetItemPosition(ST_SLOT_INFO* i_nInfo, ITEM_GENERAL* i_Item);
	int			RestPosition(ST_SLOT_INFO* i_nInfo, int Position);

	void		GambleButtonOK();

	void		SetSlotinfo(vectRARE_ITEM_INFOPtrList mRareitem, int position);
	void		ResetInfo();
	void		ResetInfo(vector<ST_RENDER_ITEM*> mRenderItem);

	void		UpDataInfo();
	void		PrizeItemGiveMe();
	
	int			PossibleUpLoadItemNum(ITEM* pItem);
	BOOL		IsPossibleUpLoadItem(ITEM* pItem, int nPosition);
	void		UpLoadItem(CItemInfo* i_pItem, int nPosition);
	void		ClearItemInfo(int nPosition);
	BOOL		PutRareInfo(MSG_FC_STORE_UPDATE_RARE_FIX* pMsg);
	void		DeleteTargetItem(int nItemUniqueNumber);
	
	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - ������ �÷� ���� ���� ����
	//void		UsingItemLoad(int nPosition);
	void		UsingItemLoad();
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - ������ �÷� ���� ���� ����
	BOOL		IsCloseOptionMachine();
	void		EnterMachineShop();
	void		OnCloseInfWnd();
	
private:
	CINFImageEx*				m_pImgOptionBG;				// �ɼ� ������ ��� �̹���

	CINFImageEx*				m_pImgOptionEffBG[7];		// �ɼ� ����Ʈ ��� �̹���

// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	CINFImageEx*				m_pImgSlotDark;				// ������ �Ⱥ��̴� ������ ������ �̹���

	CINFImageEx*				m_pImgPreArrow[3];			// ���� ȭ��ǥ ����Ʈ �̹���
	CINFImageEx*				m_pImgSufArrow[3];			// ���� ȭ��ǥ ����Ʈ �̹���
//end 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	CINFImageEx*				m_pImgSlotPreQuestion;		// ���� ����ǥ
	CINFImageEx*				m_pImgSlotPreBG[2];			// ���� ���ư��� ���
	
// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	CINFImageEx*				m_pAME_slot[2];				// ���� ������ ����Ʈ ����
//end 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	
	CINFImageEx*				m_pImgSlotSufQuestion;		// ���� ����ǥ
	CINFImageEx*				m_pImgSlotSufBG[2];			// ���� ���� ���ư��� ���
	
	CINFImageEx*				m_pImgPrizePrePositionCur;	// ���� ���� ��ġ ǥ��
	CINFImageEx*				m_pImgPrizeSufPositionCur;	// ���� ���� ��ġ ǥ��

	CINFImageBtn*			m_pBtnStart;				// �ӽ� ���� 
	CINFImageBtn*			m_pBtnTake;					// ������ �ޱ�

// 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����
	BOOL					m_IsArrow[2];				// ȭ��ǥ
	int						m_isArrowCnt[2];			// ȭ��ī����
	BOOL					m_isEffect;					// ���� ������ ����Ʈ
	int						m_bisInit;					//						
	//end 2010. 05. 13 by jskim �ű� ��Ű �ӽ� �߰���� - �޴� ������ ����Ʈ ����	
	int						m_nEffectIdx;				// ���� �� �̹��� �ε���
	float					m_fTimeEffectBG;			// ��� ������ ���� �ð�

	BOOL					m_IsShopMode;				// ��Ű�ӽ� ����ϰ� �ִ���
	int						m_nState;					// ������ ����	
	
	CD3DHanFont*			m_pOptionFont;				// �ɼǿ� ������ �۾�ü	

	ST_SLOT_INFO*			m_SlotInfo[OPTION_ITEM_LIST_FIX];	// ������ ����
	
	float					m_fTrickTime;				// Ʈ�� �ð�
	vector<ST_RENDER_ITEM*>	m_RenderPreOption;			// ����
	vector<ST_RENDER_ITEM*>	m_RenderSufOption;			// ����
	
	vectRARE_ITEM_INFOPtrList m_RareItemList[OPTION_ITEM_LIST_FIX];			// ��ü ������ ����Ʈ
	vector<ST_ITEM_INFO*>	m_vecItemInfo[OPTION_ITEM_LIST_FIX];			// ���� �����ۿ� �� �ɼ� ����
	CItemInfo*				m_pPrizeItem;				// ���� ������ ����
	CItemInfo*				m_pSelectItemInfo[OPTION_SELECT_ITEM];		// �Ƹ� ������, ����, ����
};

#endif // !defined(AFX_INFOPTIONMACHINE_H__CDAC2CA9_6BE1_4B0A_B4BD_FDA9ED7DADEB__INCLUDED_)

// INFCharacterInfoExtend.h: interface for the CINFCharacterInfoExtend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCHARACTERINFOEXTEND_H__F4A3ED92_70D3_48E3_BE94_8B21CBDB874D__INCLUDED_)
#define AFX_INFCHARACTERINFOEXTEND_H__F4A3ED92_70D3_48E3_BE94_8B21CBDB874D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "RangeTime.h"						   // 2011-10-06 by jhahn ��Ʈ�� ������ �ý���
//#include "INFInven.h"


#define CHARACTER_INFO		0
#define CHARACTER_AMMO		1
#define CHARACTER_UTC		2
#define CHARACTER_ARENA		3
#define CHARACTER_PARTNER	4		// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â

#define CHARACTER_INTERFACE_FONT_NUM		6

#define STAT_ATTACK			0
#define STAT_DURABILITY		1
#define STAT_FUEL			2
#define STAT_INDUCTION		3
#define STAT_AVOID			4
#define STAT_KALISMA		5
#define STAT_NONE			8

#define STAT_BUTTON_DISABLE		0
#define STAT_BUTTON_UP			1
#define STAT_BUTTON_DOWN		2

#define CHAR_UTC_TYPE_NUMBER	4
#define CHAR_UTC_SLOT_NUMBER	5
#define MAX_CHAR_UTC_SLOT_NUMBER	10
#define MAX_PARTNER_UTC_SLOT_NUMBER	8					  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

#define C_QUEST_LIST_NUM		6
#define C_QUEST_DESC_NUM		5

// 2007-05-15 by bhsohn ��� ���� ���� ó��
#define STAT_SHUTTLEINFO_ATTACK		0		// ����
#define STAT_SHUTTLEINFO_DEFENCE	1		// ���
#define STAT_SHUTTLEINFO_FUEL		2		// ����
#define STAT_SHUTTLEINFO_SOUL		3		// ����
#define STAT_SHUTTLEINFO_DODGE		4		// ȸ��
#define STAT_SHUTTLEINFO_SHIELD		5		// ����
#define	MAX_STAT_SHUTTLEINFO		6

 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
#define  SOKET_TYPE_NONE	0  
#define  SOKET_TYPE_PRODUC	1  
#define  SOKET_TYPE_ITEM	2 
#define  SOKET_TYPE_SKILL	3
#define  SOKET_TYPE_SPEED	4


#define	 SOCKET_LINE		5
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���					   


class CD3DHanFont;
class CINFImage;
class CAtumNode;
class CINFScrollBar;
class CSkillInfo;
class CINFImageBtn;		// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
class CINFArenaScrollBar;	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
// 2011. 10. 10 by jskim UI�ý��� ����
class CINFImageEx;
class CINFGroupManager;
class CINFGroupImage;				  
// end 2011. 10. 10 by jskim UI�ý��� ����
class CINFCharacterInfoExtend : public CINFBase
{
public:
	CINFCharacterInfoExtend(CAtumNode* pParent);
	virtual ~CINFCharacterInfoExtend();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void RenderSkillInShop();
	virtual void Tick();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int InfoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int UTCWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int AmmoWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcSkillInShop(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int QuestWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int ArenaWndProc(UINT uMsg,WPARAM wParam,LPARAM lParam);
	int PartnerWndProc(UINT uMsg,WPARAM wParam,LPARAM lParam); // 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â

	void SetAllUTCInfo();
	void SetSelectUTC( INVEN_DISPLAY_INFO* pDisplayInfo );
	void SendUseSkill( ITEM_BASE* pSkill );

	int GetCharacterQuestCount();
	void SetQuestDesc(int nQuestWindowDataIndex);
	int GetUtcMaxScrollNumber();
	void RenderShowStatInfo() ;
	void SendDeleteSelectItemSkill();
	void RenderNonQuest(int nWindowPosY, int nLine);
	void RenderStatDescToolTip(int nType, int x, int y);

	BOOL CheckUseSkillApprovalEnd(ITEM_BASE* pSkill);

	// 2008-09-22 by bhsohn EP3 ĳ���� â
	void SetCharacterInfoType(int nCharacterInfoType);
	BOOL IsShowWnd();
	void ShowCharacterInfoWnd(BOOL bShowWnd, POINT* pPos=NULL);
	void GetItemAttackInfo(CItemInfo *pItemInfo, char* o_pDstBuff);
	BOOL IsEnableItem(ITEM* pITEM);
	float GetDefaultPierce(BOOL bIsPrimaryAttack);
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	void Patner3D();
	void PatnerInfo();
	void PatnerDtail();
	void LevelExpBar( const Experience_t a_ExpNow , const Experience_t a_ExpMax );
	void StaminaBar( const int a_StaminaNow , const int a_StaminaMax );
	void StateBarUpdate();
	void ExpScrollUpdate();


	void CheckSocketSetting();				 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	void ItemScrollUpdate();				 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	void SetBar(CINFImage* Image1, CINFImage* Image2, float MaxValue, float Value, float x, float y);
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â

	// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ���� ������ ���� �Ѱ��ֱ�.
	void GetPtr_AttachmentedPetInfo ( INVEN_DISPLAY_INFO **o_ppInvenDisplayInfo , tPET_CURRENTINFO **o_ppPetCurInfo , tPET_BASEDATA **o_ppPetBaseData , tPET_LEVEL_DATA **o_ppPetLevelData );


	void SetSockeitem(int Sockenum,CItemInfo* pItemInfo,UID64_t UniqueNum);		   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	void SetItemSoket(int Sockenum,CItemInfo* pItemInfo,UID64_t UniqueNum);		   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	void SetSkillSoket(int Sockenum,CSkillInfo* pItemInfo,UID64_t UniqueNum);	   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	int	GetPartnerState(){ return m_nCharacterInfoType;}							//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

public:
	BOOL					m_bRestored;
	ITEM_SKILL*			m_pDeleteSkill;// ���� ��ų ������(reference)


private:	
	BOOL					m_bInvalidated;

	//CINFImage			*	m_pAmmo;
	//CINFImage			*	m_pStatImage ;
	//CINFImage			*	m_pStat[3];
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx			*	m_pInfo;
	//CINFImageEx			*	m_pInoBk;
	//CINFImage			*	m_pId;
	CINFImageEx			*	m_pUtc;
	CINFImageEx			*	m_pUtcScr;
//	CINFImage			*	m_pQuest;
//	CINFImage			*	m_pImgArenaTab;
	CINFImageEx			*	m_pQuestSelect;
	CINFImageEx			*	m_pSkillShopBack;
//	CINFImage			*	m_pScrollBar;
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	CINFImageEx			*	m_pPartnerInfoBk;
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	CINFGroupImage		*	m_pInformationBK;
#else 
	CINFImageEx			*	m_pInfo;
	CINFImageEx			*	m_pInoBk;
	//CINFImage			*	m_pId;
	CINFImageEx			*	m_pUtc;
	CINFImageEx			*	m_pUtcScr;
//	CINFImage			*	m_pQuest;
//	CINFImage			*	m_pImgArenaTab;
	CINFImageEx			*	m_pQuestSelect;
	CINFImageEx			*	m_pSkillShopBack;
//	CINFImage			*	m_pScrollBar;
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	CINFImageEx			*	m_pPartnerInfoBk;
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
#endif

	CD3DHanFont			*   m_CharacInfoFont[5];
	CD3DHanFont			*   m_StatInfoFont[7];
	CD3DHanFont			*	m_pFontName;
	CD3DHanFont			*	m_pFontLevel;
	CD3DHanFont			*	m_pFontGuild;
	CD3DHanFont			*	m_pFontGeneral;
	CD3DHanFont			*	m_pFontPropensity;
	CD3DHanFont			*	m_pFontItemNum[MAX_CHAR_UTC_SLOT_NUMBER];

	CD3DHanFont			*	m_pFontQuestList[C_QUEST_LIST_NUM];
//	CD3DHanFont			*	m_pFontQuestState[6];
	CD3DHanFont			*	m_pFontQuestDesc[C_QUEST_DESC_NUM];
	CD3DHanFont			*	m_pFontAmmoWeapon[CHARACTER_INTERFACE_FONT_NUM];	// 1�� ����/����, 2�� ����/��Ƽ��, ���, ȸ��
	CD3DHanFont			*	m_pFontAutoStat;
	CD3DHanFont			*	m_pFontStatDec;

	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	CINFImageBtn		*	m_pChangeNameBtn;		// �̸� ���� ��ư

	CD3DHanFont			*	m_pPartnerName;			// ��Ʈ�� �̸�
	CD3DHanFont			*	m_pPartnerLv;			// ��Ʈ�� ����
	CD3DHanFont			*	m_pPartnerType;			// ��Ʈ�� Ÿ�� ( ������, �����, ������, ���λ���)
	CD3DHanFont			*	m_pStamina;				// ��Ʈ�� ���׹̳�
	CD3DHanFont			*	m_pPartnerLvBar;		// ��Ʈ�� ������
	CD3DHanFont			*	m_pPartnerStaminaBar;	// ��Ʈ�� ���׹̳ʹ�
	CD3DHanFont			*	m_pPartnerGiveExp;		// ��Ʈ�ʿ��� �Ѱ��� ����ġ �ۼ�Ʈ

	CD3DHanFont			*	m_pPartnerItem;		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	
	CINFImageEx			*	m_pPartnerImageLvBar[2];		// ��Ʈ�� ������
	CINFImageEx			*	m_pPartnerImageStaminaBar[2];	// ��Ʈ�� ���׹̳ʹ�

	CINFArenaScrollBar	*	m_pExpScroll;			// ��Ʈ�� ����ġ ȹ�� Ȯ�� ��ũ�ѹ�

	CINFArenaScrollBar	*	m_pHpItemScroll;			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	CINFArenaScrollBar	*	m_pDpItemScroll;			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	CINFArenaScrollBar	*	m_pSpItemScroll;			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	
	CINFImageEx			*	m_pPartnerImage;
	//CINFImageBtn		*	m_pPartnerBtnCancel;		//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	//CINFImageBtn		*	m_pPartnerBtnOK;			//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	//2011. 03. 08 by jhAhn ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â(UI����)
	CINFImageEx			*	m_pPartnerSocet;
	CINFImageEx			*	m_pPartnerSocetClose;
	CINFImageEx			*	m_pPartnerSocetSelect;

	CINFImageEx			*	m_pPartnerItemLine;					  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	//end 2011. 03. 08 by jhAhn ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â(UI����)
 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	
	CINFImageEx			*	m_pPartnerItemUseSelect;
	CINFImageEx			*	m_pPartnerSkillSocket;
	CINFImageEx			*	m_pUseSkillSocket;

	CINFImageEx			*	m_pPartnerSkillshopSocket;


	CINFImageEx			*	m_pBigSlotSocket;

	CINFImageEx			*	m_pPartnerSkillShop[9];
	CINFImageEx			*	m_pOkSocket[9];

	CINFImageEx			*	m_pItemSocketHP[4];
	CINFImageEx			*	m_pItemSocketShield[4];	
	CINFImageEx			*	m_pItemSocketSp[4];
	
	
	
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	int						m_nCharacterInfoType;
	int						m_nStatType ;
	int						m_nButtonState ;
	int						m_nMissionProCount;		// ������ �̼� ī��Ʈ



	int					m_nCurrentUTCScroll;
//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	int						m_nSockenum;

	BOOL					m_bShowSoketSetting;
	BOOL					m_bCheckSoketSetting;

	UID64_t					m_uCheckSocketNum;

	CINFImageBtn		*	m_pPartnerSocketSettingOk;
// 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 
	CINFImageEx*				m_pImgDissolutionTakeEffect[2];

	CINFImageEx*				m_pImgPnUseHp;
	CINFImageEx*				m_pImgPnUseSh;
	CINFImageEx*				m_pImgPnUseSp;	
//end 2011-12-120 by jhahn ������ ��Ʈ�� �ý��� 

	CINFImageEx*			m_pWearPositionBlink;//�κ��ȿ� �ִ� �����ۿ� ���콺�� ���� ������ ����â ��ġ���� ������ �̹���

	CRangeTime			m_tBlinkTime;


	int						m_nEffStep;					// ����Ʈ ����
	float					m_fTimeEffectBG;			// ��� ������ ���� �ð�
//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	
	INVEN_DISPLAY_INFO	m_pSkillShopSkillDisplayInfo[CHAR_UTC_TYPE_NUMBER][MAX_CHAR_UTC_SLOT_NUMBER];
	INVEN_DISPLAY_INFO	m_pCharacterSkillDisplayInfo[CHAR_UTC_TYPE_NUMBER][MAX_CHAR_UTC_SLOT_NUMBER];
	INVEN_DISPLAY_INFO	m_pPartnerSkilldisplay[MAX_PARTNER_UTC_SLOT_NUMBER];		 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	CINFImageEx*			m_pSelectIcon;// ���콺 Ŭ���� ���� ���õ� ������ �̹���(reference)
	ITEM_SKILL*			m_pSelectSkill;// ��ų ������(reference)	
	BOOL				m_bUTCScrollLock;
	POINT				m_ptMouse;

//	int					m_nCurrentQuestListScroll;
//	int					m_nCurrentSelectQuestList;
//	BOOL				m_bQuestListLock;
//	int					m_nCurrentQuestDescScroll;
//	int					m_nCurrentSelectQuestDesc;
//	BOOL				m_bQuestDescLock;
	int					m_nRenderMoveIconIntervalWidth;
	int					m_nRenderMoveIconIntervalHeight;

	CINFScrollBar*		m_pQuestListScrollBar;
	CINFScrollBar*		m_pQuestDescScrollBar;
	vector<string>		m_vecQuestDescription;
	int					m_nOldQuestSelectDataIndex;

	CINFScrollBar*		m_pShopSkillScrollBar;
	BOOL				m_bLbuttonDown;	
//	BOOL				m_bToolTipStat;				// 2005-07-04 by ispark �ڵ� ���� �й� ����

private:
	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	void				GetStatShowInfo();
	void				RenderCharacterInfo();
	void				RenderCommandInfo();
	void				RenderSkillInfo();
	// 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	void				RenderPartnerInfo();
	//end 2010. 06. 07 by jskim ��Ʈ�� �ý��� ���� - ��Ʈ�� ����â
	void				OnClickStat(int nPos);
	void				UpdateBtnPos();
	BOOL				IsMouseCaps(POINT ptPos);
	void				GetDefenceRateString(BOOL bIsPrimaryAttack, char* pDefaultRate, char* pArmorRate, char* pPetRate);
	void				GetDodgeRateString(BOOL bIsPrimaryAttack, char* pDefaultRate, char* pArmorRate, char* pPetRate);
	void				RenderWeightStats();
	BOOL				IsMouseCaps2(POINT ptPos);						   //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	void				SkillRender();
	void				ItemRender();
	void				ProductRender();

	void				SocketOkRender();

	void				RenderSocket(int num, int nSelectItem);

	void				RenderItemSocket(int num,CItemInfo* nSelectItem,BOOL OnOff);
	void				RenderSkillSocket(int num,CSkillInfo* nSelectItem,BOOL OnOff);

	int					CheckSocket(int num);
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	// 2008-12-16 by bhsohn A��� ������Ƽ�� ��ų ����� ���� ��ų ���� ���������� ��ų ���
	void				RefreshAGearSkillInfo(int nSkillItemNum);

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	void				OnClickNickNameBtn();
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���


private:
	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	CD3DHanFont			*   m_StatInfoBoldFont;		// �β��� �۾�ü 
	CD3DHanFont			*   m_StatOverInfoFont;
	BOOL					m_bStatShow[MAX_STAT_SHUTTLEINFO];
	BOOL					m_nShowOverStat[MAX_STAT_SHUTTLEINFO];

	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	CINFImageBtn*			m_pOpenBtn;			// �ݱ�	
	CINFImageBtn*			m_pCloseBtn;
	
	
	CINFImageBtn*			m_pBonusStatBtn[MAX_STAT_SHUTTLEINFO];			// ���ʽ� ����

	POINT					m_ptBkPos;
	POINT					m_ptSkillBkPos;									//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	BOOL					m_bShowWnd;

	BOOL						m_bMove;
	BOOL						m_bMoveSkill;								//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	POINT						m_ptCommOpMouse;
	POINT						m_ptSkillCommOpMouse;						//2011-10-06 by jhahn ��Ʈ�� ������ �ý���


	vector<string> vectempCulling;											 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	char strBuf[SIZE_MAX_ITEM_DESCRIPTION];									//2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	CINFImageBtn*			m_pNickNameBtn;
	CD3DHanFont*			m_pFontNickName;
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���


	CD3DHanFont			*   m_SocketDescInfoFont;							 //2011-10-06 by jhahn ��Ʈ�� ������ �ý���


};

#endif // !defined(AFX_INFCHARACTERINFOEXTEND_H__F4A3ED92_70D3_48E3_BE94_8B21CBDB874D__INCLUDED_)

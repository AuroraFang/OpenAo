// INFMissionTreeInfo.h: interface for the CINFMissionTreeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMISSIONTREEINFO_H__AA8A3F7B_5F6B_4110_8D3B_A96B2282984C__INCLUDED_)
#define AFX_INFMISSIONTREEINFO_H__AA8A3F7B_5F6B_4110_8D3B_A96B2282984C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"
#include "INFImageRadioBtn.h"
#include "INFImageListCtrl.h"
#include "INFImageListBox.h"
#include "INFMissionMapViewCtrl.h"
#include "INFMissionAniCtrl.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	#define		TAB_MISSION_MODE_EP2			0		// Ep2�̼�
	#define		TAB_MISSION_MODE_EP3			1		// Ep3�̼�
	#define		TAB_MISSION_MODE_EP4			2		// Ep4�̼�
	#define		MAX_TAB_MISSION_MODE_EP			3		// 
#else
#define		TAB_MISSION_MODE_EP2			0		// Ep2�̼�
#define		TAB_MISSION_MODE_EP3			1		// Ep3�̼�
#define		MAX_TAB_MISSION_MODE_EP			2		// 
#endif
// ū����



// �̼� ����
#define		TAB_MISSION_MODE_SCEN			0		// �ó�����
#define		TAB_MISSION_MODE_NORMAL			1		// �Ϲ�
#define		TAB_MISSION_MODE_OUT			2		// ���� �̼�
#define		MAX_TAB_MISSION_MODE			3		// 

#define		TAB_MISSION_ALL_SIZE			30		// ��ü���� 

// �̼� ��� ����
#define		TAB_MISSION_ALL				0		// ��ü���� 
#define		TAB_MISSION_COMPLETE		1		// �ذ� �̼� ����
#define		TAB_MISSION_NOTCOMPLETE		2		// ���ذ� �̼� ����
#define		MAX_TAB_MISSION				3		// 

// �̼� ����
#define		TAB_COMPENS_EXP				0		// ����ġ
#define		TAB_COMPENS_LEV				1		// ����
#define		TAB_COMPENS_PRO				2		// ��
#define		TAB_COMPENS_BUN				3		// ���ʽ� ����
#define		MAX_TAB_COMPENS				5

typedef struct 
{	
	BOOL			bComplete;
	int				nQuestKind;
	int				nQuestIndex;	
	int				nMainIdx;
	int				nSubIdx;	
	DWORD			dwColor;
	int				nOrderIdx;			// ���� �ε���
	char			szItemTxt[256];
	// 2007-07-30 by bhsohn �Ϲ�, ���� ����Ʈ ������
	int				nMinLevel;
	int				nMaxLevel;
	// end 2007-07-30 by bhsohn �Ϲ�, ���� ����Ʈ ������
	BYTE			QuestEpisodeType;	// 2008-06-18 by bhsohn EP3 ����Ʈ �߰�
} structQuestInfo;


class CINFImage;
class CD3DHanFont;
class CINFMissionMain;
class CINFImageEx;
class CINFGroupImage;

class CINFMissionTreeInfo  
{
public:
	CINFMissionTreeInfo(CINFMissionMain* i_pParent);
	virtual ~CINFMissionTreeInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetMissionEnterShop(BOOL bEnterShopState);

	// �̼Ǹ����� �̵�
	void OnClickGoMissionMap(DWORD i_dwSelQuestIndex);
	
	// ������ ����Ʈ
	int HideMissionCheckProgressMission(CQuest*	 i_pHidePosQuest);

	// �̼� ���� ��������
	void RefreshMission(BOOL bClick, INT QuestIndex);

	void InitMissionInfo(BOOL bFirstPopup);

	void SelectNextMission(INT nQuestIndex);
// 2008-12-09 by dgwoo �̼Ǹ�����.
	INT	 GetMissionMasterIdx(){
		return m_nMissionMasterQuestIdx;
	}
	void UpdateMissionMasterButton();
	
private:	
	void RednerMissionMode();
	void RenderCompensationItem();

	void SelectRadioBtn(int nSelRadioBtn);
	
	int	 OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int	 OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int	 OnMouseMove(WPARAM wParam, LPARAM lParam);
	int	 OnMouseWheel(WPARAM wParam, LPARAM lParam);

	void LoadListItem();
	
	
	void AddScenItem(int nMainId);
	void AddNormalItem(int nMainId);
	void AddOutItem(int nMainId);

	int GetQuestKind_TO_Tab(BYTE QuestKind);

	int CheckProgressMission(CQuest *pQuestInfo, 
								vector<int>* o_vecErrPart, 
								vector<string>*  o_vecErrQuestName,
								vector<string>*  o_vecErritem);
	int	GetListCtrlSel_TO_QuestIndex(int nSelMainIdx, int nSelSubIdx);

	void OnSelectListCtrl(INT QuestIndex);
	void OnSelectMission(int nQuestIndex);
	
	void SetErrorString(CQuest* i_pQuest, 
							vector<string>* o_vecPossibleDesc,
							vector<int>* o_vecErrPart, 
							vector<string>*  o_vecErrQuestName,
							vector<string>* o_vecErritem);	

	// ��������� �̸�����
	void SetQuestPay(CQuest* pQuest, vector<QUEST_PAY_ITEM>	*o_vecCompensationItemName, int *o_nCompensation);
	
	// ��ü ����Ʈ ���� ����
	void LoadQuest(int nSelMission);
	// �Ϸ�� ����Ʈ ����Ʈ ������	
	void LoadQuestInfo(int nSelMission, vector<int>* pVecQuestIndex);

	// �̼� ���� 
	void GetMissionShow(int nSelMission, BOOL *o_bAllShow,BOOL *o_bShowComplete);

	void OnRadioButtonDown(int nRadioBtn);

	// �̼� ���� ����
	BOOL IsMouseMoveMissoinComp(POINT pt);	

	void OnClickGiveupMission(DWORD dwSelQuestIndex);	// �̼� ���� ��ư

	void SortQuestInfo(vector<structQuestInfo>* i_vecQuestInfo);

	int ConvertOrderIdx(char* pItemTxt);

	void InitBtn();

	void SortQuestInfo_SideMision(vector<structQuestInfo>* i_vecQuestInfo);
	BOOL SortSideMision(int nFindQuestIdx, int nOrderIdx, vector<structQuestInfo>* i_vecQuestInfo);

	void SortQuestInfo_Scen(vector<structQuestInfo>* i_vecQuestInfo);		// 2012-11-29 by bhsohn ȥ���� �縷 ����Ʈ �ȳ����� ���׼���

	void ResortLoadQuest();
// 2008-12-09 by dgwoo �̼Ǹ�����.
	void MissionMasterButton();
	void MissionMasterHelpButton();
	void MissionMasterPartyButton();
	BOOL IsMissionMasterBtnAct();


private:
	// ��� �̹���
	//CINFImage*		m_pMissionBk;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*		m_pMissionBk;
#else
	CINFImageEx*		m_pMissionBk;
#endif

	// �̼� ��� �̹���
	CINFImageRadioBtn*		m_pMissionMode[MAX_TAB_MISSION];
	int				m_nSelMission;	// ������ �̼�

	// ����Ʈ ���� ����
	vector<structQuestInfo>		m_vecQuestInfo[MAX_TAB_MISSION_MODE_EP][MAX_TAB_MISSION_MODE];

	// ��� �̹��� ��ġ�� ũ��
	float			m_fBackPosX;
	float			m_fBackPosY;

	// ��� �̹��� ��ġ�� ũ��
	float			m_fBackWidth;
	float			m_fBackHeight;

	// err Vector
	vector<int>				m_vecErrPart;
	vector<string>			m_vecErrQuestName;
	vector<string>			m_vecErritem;

	// �̼� ����
	vector<string>			m_vecMissionName;			// �̼� �̸�
	vector<string>			m_vecPossibleDesc;			// �̼� ����

	// ���� ������
	CD3DHanFont	*			m_pCompensation;
	vector<QUEST_PAY_ITEM>	m_vecCompensationItemName;	// �̼� ��������� ��Ī
	int						m_nCompensation[MAX_TAB_COMPENS];
	BOOL					m_bCompenToolTib[MAX_TAB_COMPENS];

	// ����Ʈ ��Ʈ��
	CINFImageListCtrl*		m_pMissionListCtrl;
	// �̼� ���� ����Ʈ �ڽ�
	CINFImageListBox*		m_pINFImageListBox; // �̼� ���� ����Ʈ �ڽ�
	// �̼� ���� ��ư
	CINFImageBtn*			m_pStartMissionBtn;
	// �̼Ǹ����� �̵� 
	CINFImageBtn*			m_pGoMissionBtn;	
	// �̼�����
	CINFImageBtn*			m_pGiveupMissionBtn;
	// 2008-12-09 by dgwoo �̼� ������.
	CINFImageBtn*			m_pBtnHelper;
	CINFImageBtn*			m_pBtnPartyHelper;
	CINFImageBtn*			m_pBtnMissionMaster;
	CINFImageBtn*			m_pBtnMissionUnMaster;

	float					m_fMMBtnTime;					// ��ư�� ��Ÿ�� ��������
	// end 2008-12-09 by dgwoo �̼� ������.
	// �ݱ� â
	CINFImageBtn*			m_pCloseBtn;
	// �� ���� ������
	CINFMissionMapViewCtrl*		m_pINFMissionMapViewCtrl;
	// �ִϸ��̼� ��Ʈ��
	#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFMissionAniCtrl*			m_pINFAniCtrl;
	#endif

	// ������ ����Ʈ	
	DWORD					m_dwSelQuestIndex;

	// �θ� ������
	CINFMissionMain*		m_pParentMissionMain;
	INT						m_nMissionMasterQuestIdx;	// 2008-12-15 by dgwoo �̼� ������ - �̼ǹ�ȣ 

	
};

#endif // !defined(AFX_INFMISSIONTREEINFO_H__AA8A3F7B_5F6B_4110_8D3B_A96B2282984C__INCLUDED_)

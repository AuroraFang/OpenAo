// CINFMissionInfo.h: interface for the CINFMissionInfo class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------

  * �̼� �������̽� ���� 2005-07-07 by ydkim
  * ���� �̼Ǽ� ���� ����

  MissionProcFlag(BOOL bFlag) : ��ü ���� �̼� �÷��׷� �̼�â ����, ���ǰ˻�, �̼�â �ݱ� �˻����
  - ������ 0
  - ���н� 0�̾ƴ� �ٸ����� �����Ѵ�  

 
*///------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_INFMISSIONINFO_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFMISSIONINFO_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

typedef enum 
{
	MISS_BACK				=				0,		// �⺻ ���
	MISS_NOM				=				1,		// �Ϲ� �̼� ���
	MISS_SCEN				=				2,		// �ó����� �̼� ���
	MISS_SCEN_D				=				3,		// �ó����� ���� ���
	MISS_CANCEL				=				4,		// �̼� ���
	MISS_SEL_BACK			=				5,		// ���� ���
	MISS_SEL_RADIO			=				6,		// ���� ���� ��ư
	MISS_RADIO				=				7,		// �Ϲ� ���� ��ư
	MISS_RADIO_S			=				8,		// ���� ���ù�ư
	MISS_START_N			=				9,		// �Ϲ� �̼� ��ư 
	MISS_START_O			=				10,		
	MISS_START_S			=				11,
	MISS_START_NOT			=				12,
	MISS_COPLETE_N			=				13,		// �Ϲ� �̼� �Ϸ� ��ư
	MISS_COPLETE_O			=				14,
	MISS_COPLETE_S			=				15,
	MISS_COPLETE_NOT		=				16,
	MISS_LAUNCH_SHUTTLE_N	=				17,
	MISS_LAUNCH_SHUTTLE_O	=				18,
	MISS_LAUNCH_SHUTTLE_S	=				19,
	MISS_LAUNCH_SHUTTLE_NOT	=				20,
	MISS_MAP_MOVE_N			=				21,
	MISS_MAP_MOVE_O			=				22,
	MISS_MAP_MOVE_S			=				23,
	MISS_MAP_MOVE_NOT		=				24,
	MISS_MAP_GO_N			=				25,
	MISS_MAP_GO_O			=				26,
	MISS_MAP_GO_S			=				27,
	MISS_MAP_GO_NOT			=				28,
	MISS_INFLUENCE_BK		=				29,		// ���� �κ� �߰�
	MISS_INFLUENCE_NR		=				30,
	MISS_INFLUENCE_OV		=				31,
	MISS_INFLUENCE_PS		=				32,

	IMAGE_MAX_COUNT			=				33		// �ִ� �̹��� ��
} MISSION_IMG_LIST;

typedef enum 
{
	COMPENS_EXP				=				0,		// ����ġ
	COMPENS_LEV				=				1,		// ����
	COMPENS_PRO				=				2,		// ��
	COMPENS_BUN				=				3,		// ���ʽ� ����

	COMPENS_MAX				=				5//4
} MISSION_COMP;

#define LEFTFONT_MAX_LINE_COUNT				15		// �ִ� ��Ʈ ���μ�
#define RIGHTFONT_MAX_LINE_COUNT			10		// �ִ� ��Ʈ ���μ�
#define MISSION_TARGET_LIST					10		// �̼� ��ǥ
#define	MISSION_SCROLL_FLUENCE_MAX_LINE		20		// ���¼��� �ִ� ���μ�

class CINFImage;
class DataHeader;
class CGameData;
class CD3DHanFont;
class CINFScrollBar;
class CINFImageEx;

class CINFMissionInfo: public CINFBase
{

public:
	CINFMissionInfo();
	virtual ~CINFMissionInfo();
	virtual HRESULT			InitDeviceObjects();
	virtual HRESULT			RestoreDeviceObjects();
	virtual HRESULT			DeleteDeviceObjects();
	virtual HRESULT			InvalidateDeviceObjects();
	virtual void			Render();
	virtual void			Tick();
	virtual int				WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	DWORD					MissionProcFlag(BOOL bFlag);
	BOOL					GetMissionWindowState(){return m_bMissionWindowStart;}
	void					SetMissionGiveUp(int nflag){ m_nMissionGiveUp = nflag;}
	void					RefreshMission();
	int						GetMissionMapIndex(){return m_nMissionMapIndex;}
	void					SetEnterShop(BOOL bFlag){m_bFlagEnterShop = bFlag;}
	CQuest*					GetMissionIndex() {return m_pQuestInfoProgOutDoor;}
	char*					GetMissionList(int nListIndex) {return (char*)m_vecMissionList[nListIndex].c_str();}
	void					SetInfluenceMission(BOOL nFlag, float fANIPercent, float fVANPercent);	// ���¹̼� ����, // 2006-02-08 by ispark, �ۼ�Ʈ �߰�
	BOOL					GetInfluenceMission() {return m_bInfluenceFlag;}	// ���¹̼� ����, // 2006-02-08 by ispark, �ۼ�Ʈ �߰�

	void					SetPosMissionSearchStart(BOOL nstate){	m_bSearchPosQuest = nstate; }
	void					SetPosMissionInfo(CQuest* pQuest){m_pHidePosQuest = pQuest;}
	void					MissionStart(DWORD dMissionIndex);				// �̼� �����ϱ�
	void					SetPosMissionComplete(BOOL bFlag){m_bPosMissionComplete = bFlag; }
	void					SetFirstMissionSelect(BOOL bFlag){m_bViewMissionSelectFirst = bFlag;}
	int						GetSelectIndexCurrent(){return m_nSelectMissionIndex;}
	void					SetSelectIndexCurrent(int nIndex){m_nSelectMissionIndex = nIndex;}

	void					RenderMissionTarget();		// 2006-08-11 by ispark, �ٸ� ������ ������
protected:
	void					InitMission();				// �̼� �ʱ�ȭ
	void					InitMissionTitle();			// �̼� ����Ʈ ����
	void					InitInfluenceMission();		// ���� �̼� �ʱ�ȭ
	BOOL					MissionWindowRunState();	// �̼�â ���� ���� �˻�
	void					StartPossibleMission();		// ���� ���� �̼� ����Ÿ ����	
	void					MissionEnd(DWORD dMissionIndex);				// �̼� �Ϸ��ϱ�
	
	void					RenderCompensationItem();	// ��������� ������
	void					RenderPossibleMission();	// ���� �� �� �ִ� �̼�
	void					RenderMissionList();		// �̼� ����Ʈ ������
	void					RenderInfluenceMission();	// ���� �̼� ������
	int						CheckProgressMission(CQuest *pQuestInfo);
	int						WndProcInfluence(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL					CheckMissionStart();		// �̼� ������ ���� ���� �˻�

	// �̼� ������ ���� �κ� (���� ����)
	void					SetCompleteMissionInput();	// ��ü �̼�, ���� �̼� �з�
	void					SetMissionClassiFy();		// �Ǻ�, �ذ�, ���ذ�, ��ü �з�
	CQuest*					SetMissionSorting();		// �̼� �ó������� �з� �� ���� �̼� ã��
	void					SetMissionInfo();			// ���� �Ҽ��ִ� �̼ǰ� ���� �̼� ����

	void					OverlapSortPayItem(CQuest *pQuest);// �ߺ� ������ ����	

protected:
	CINFImageEx	*			m_pMissionImg[IMAGE_MAX_COUNT];
	CINFImageEx*			m_pImgOkButton[4];					// ��ư �̹���
	CINFImageEx*			m_pImgCancelButton[4];				// ��ư �̹���
	CINFImageEx*			m_pImgWarning;						// ��ư �̹���
	CD3DHanFont	*			m_pMissionFontLeft[LEFTFONT_MAX_LINE_COUNT];
	CD3DHanFont	*			m_pMissionFontRight[RIGHTFONT_MAX_LINE_COUNT];
	CD3DHanFont	*			m_pCompensation;
	CD3DHanFont	*			m_pMissionTarget[MISSION_TARGET_LIST];
	CD3DHanFont	*			m_pInfluenceWarning[3];
	CINFScrollBar *			m_pScrollLeftWindow;
	CINFScrollBar *			m_pScrollRightWindow;

	POINT					m_PointMissionPos;			// �̼� ������ ��ǥ
	POINT					m_PointMissionPosOld;		// �̼� ������ ��ǥ(â�̵�)
	POINT					m_PointMissionMouse;		// �̼� ���콺 ��ǥ(â�̵�)
	BOOL					m_bMissionWindowStart;		// �̼� ������ ����
	int						m_nMissionGiveUp;			// �̼� ����
	int						m_nSelectMissionTab;		// �̼� �Ǽ���
	int						m_nMissionViewCase;			// �̼� �� ���
	BOOL					m_bProgressMission;			// ������ �̼�
	BOOL					m_bMoveMissionWindows;		// �̼�â �̵�
	
	// �̼Ǹ���Ʈ ����
	vector<string>			m_vecMissionList;			// �̼��̸� ����Ʈ.
	vector<int>				m_vecTabMissionNum;			// �̼ǳѹ� ����Ʈ.
	vector<int>				m_vecQuestIndex;			// ��� �̼� �ε��� �ѹ�.
	int						m_nSelectMissionIndex;
	int						m_nMaxNum;

	// �� �� �ִ� �̼� ����
//	char					m_strMissionName[SIZE_MAX_QUEST_NAME];
	vector<string>			m_vecMissionName;
	vector<QUEST_PAY_ITEM>	m_vecCompensationItemName;	// �̼� ��������� ��Ī
	vector<string>			m_vecPossibleDesc;			// �̼� ����

	// �̼� ��ư
	int						m_nMissionButtonStateStart;
	int						m_nMissionButtonStateEnd;
	int						m_nMissionButtonMapMove;
	BOOL					m_bButtonStateStart;
	BOOL					m_bButtonStateEnd;
	BOOL					m_bButtonStateMapMove;
	BOOL					m_bMissionComButtonFlag;	// �̼� �Ϸ� ��ư ���
	BOOL					m_bMissionStartButtonUse;	// �̼� ���� ��ư ��� (�̻��� Disable�̹���)

	// ����ϱ� �̼Ǹ��̵�
	int						m_nLaunchShuttleState;
	BOOL					m_bLaunchShuttleState;
	int						m_nMissionMapMove;
	BOOL					m_bMissionMapMove;
	CQuest					*m_pQuestInfoProg;			// �������� �̼ǰ˻�(�̼�â)
	CQuest					*m_pQuestInfoProgOutDoor;			// �������� �̼ǰ˻�
	int						m_nMissionMapIndex;
	BOOL					m_bFlagEnterShop;

	// ���� ������
	int						m_nCompensation[COMPENS_MAX];
	BOOL					m_bCompenToolTib[COMPENS_MAX];

	// ���� ���� �̼�	
	int						m_nInfluencePosX;
	int						m_nInfluencePosY;
	int						m_nVCUButton;
	int						m_nANIButton;
	BOOL					m_bVCUButton;	
	BOOL					m_bANIButton;
	BOOL					m_bInfluenceFlag;			// ���¹̼� ������
	int						m_nInflChoiceOKButton;
	int						m_nInflChoiceCANButton;

	CINFScrollBar *			m_pScrollLeftfluence;
	CINFScrollBar *			m_pScrollRightfluence;

	vector<string>			m_vecVCUfluenceDesc;			// �̼� ����
	vector<string>			m_vecANIfluenceDesc;			// �̼� ����
	CD3DHanFont	*			m_pMissionFontfluenceL[MISSION_SCROLL_FLUENCE_MAX_LINE];
	CD3DHanFont	*			m_pMissionFontfluenceR[MISSION_SCROLL_FLUENCE_MAX_LINE];

	int						m_nOldMissionIndex;
	vector<string>			m_vecOldString;
	BOOL					m_bSearchPosQuest;
	CQuest*					m_pHidePosQuest;
	// err Vector
	vector<int>				m_vecErrPart;
	vector<string>			m_vecErrQuestName;
	vector<string>			m_vecErritem;
	// Ư������ �Ϸ� �̼�
	BOOL					m_bPosMissionComplete;
	BOOL					m_bViewMissionSelectFirst;

	
	

public:
	// 2006-02-08 by ispark, ���� �ۼ�Ʈ
	int						m_nInflChoiceOver;
	float					m_fVCNInflDistributionPercent;	// 2006-02-08 by cmkwon, ���������� ���Ա� ���º��� - ���¼��� �̼ǽø� ����
	float					m_fANIInflDistributionPercent;	// 2006-02-08 by cmkwon, �˸��� ���Ա� - ���¼��� �̼ǽø� ����

};

#endif

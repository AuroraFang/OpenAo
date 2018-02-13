// TutorialSystem.h: interface for the CTutorialSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUTORIALSYSTEM_H__78E3D7AE_3E51_4771_8F9B_2C9CA3BB537D__INCLUDED_)
#define AFX_TUTORIALSYSTEM_H__78E3D7AE_3E51_4771_8F9B_2C9CA3BB537D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	TUTORIAL_LESSON_COUNT				10

enum {L1_1 = 1, L1_2, L1_3,L1_4,L1_5,L1_6,L1_7};
enum {L2_1 = 1, L2_2,L2_3};
enum {L3_1 = 1, L3_2};
enum {L4_1 = 1};
enum {L5_1 = 1, L5_2};
enum {L6_1 = 1};
enum {L7_1 = 1};
enum {L8_1 = 1};
enum {L9_1 = 1};
enum {L10_1 = 1, L10_2, L10_3,L10_4};

// 2007-07-05 by dgwoo
// ��� ������ �Ʒ��� ���� �����Ѵ�.
//  �غ� �ܰ� - Ʃ�丮���� ������ ����� ���۷������� ����.
//  ���� �ܰ� - ������ ������ �����ϸ� ��ǥ�� �޼��ϱ� ���� ����.
//  ��� �ܰ� - ���� �Ǵ� ������ ����� �����ָ� �����ܰ�� �Ѿ�� ����.(����, ����)
//  ���� �ܰ� - ���۷����Ͱ����� ���� �����ϴ� �ܰ�.
//  ���� �ܰ� - �����ܰ�� �Ѿ������ �ܰ�
enum{TUTORIAL_STATE_READY = 1, 
		TUTORIAL_STATE_MOTION,
		TUTORIAL_STATE_SUCCESS,
		TUTORIAL_STATE_OPERATOR,
		TUTORIAL_STATE_NEXT,
		TUTORIAL_STATE_FAIL
};
//--------------------------------------------------------------------------//


struct Lesson2State
{
	float	fSKey;
	BOOL	nBKey;
};

struct Lesson3State
{
	BOOL	bLRolling;
	BOOL	bRRolling;
	BOOL	bLMove;
	BOOL	bRMove;
};

struct Lesson7State
{
	BOOL	bFire;
};

struct Lesson8State
{
	DWORD	nCount;
};

struct Lesson9State
{
	int		nDeadMonster;
};

struct TutorialGateInfo
{
	D3DXVECTOR3		vPos;
	BOOL			bShow;
	CObjectChild*	pNode;
};

class CINFImage;
class CGameData;
class CTutorialSystem  
{
public:
	CTutorialSystem();
	virtual ~CTutorialSystem();

	void		Tick(float fElapsedTime);
	void		TickSystemWindow(float fElapsedTime);
	void		Lesson0(float fElapsedTime);
	void		Lesson1(float fElapsedTime);
	void		Lesson1_1_5(float fElapsedTime);
	void		Lesson1_6(float fElapsedTime);
	void		Lesson1_7(float fElapsedTime);

	void		Lesson2(float fElapsedTime);
	void		Lesson2_1(float fElapsedTime);
	void		Lesson2_2(float fElapsedTime);
	void		Lesson2_3(float fElapsedTime);

	void		Lesson3(float fElapsedTime);
	void		Lesson3_1(float fElapsedTime);
	void		Lesson3_2(float fElapsedTime);
	void		Lesson4(float fElapsedTime);
	void		Lesson5(float fElapsedTime);
	void		Lesson5_1(float fElapsedTime);
	void		Lesson5_2(float fElapsedTime);
	void		Lesson7(float fElapsedTime);
	void		Lesson6(float fElapsedTime);
	void		Lesson8(float fElapsedTime);
	void		Lesson9(float fElapsedTime);
	void		Lesson10(float fElapsedTime);


	void		SetShuttleEffectPos(float fElapsedtime);
	void		InitTutorialSystem();

	void		StartTutorial();
	void		EndTutorial();

	void		Render();
	void		RenderInterface();				// ������ �ϴܿ� Ű ���� ��ư.
	void		RenderSystemWindow();
	void		RenderLesson0();				// ����ȭ��.
	void		RenderLesson1();
	void		RenderLesson2();

	void		RenderLesson3();
	void		RenderLesson4();
	void		RenderLesson5();
	void		RenderLesson6();
	void		RenderLesson7();
	void		RenderLesson8();
	void		RenderLesson9();
	void		RenderLesson10();

	DWORD		ChangeLesson(DWORD dwLesson,DWORD dwLessonState = 1);
	void		NextState();
	D3DXVECTOR3	GetGatePos();
	CObjectChild * GetActiveGate();
	void		SetCompleteLesson(INT nTutorialNum , BYTE bComplete = 1);
	// SystemWindow ó��.
	void		ChangeSystemWindow();
	void		SystemPlayingButton();
	void		SystemAgainButton();
	void		SystemMainButton();
	void		SystemEndButton();

	void		StartButtonMainChart();					// ����â���� ���� ��ư ó��.
	void		ChapterStateOperator();

	void		RenderETC();
	
	HRESULT		InitDeviceObjects();
	HRESULT		DeleteDeviceObjects();
	HRESULT		RestoreDeviceObjects();
	HRESULT		InvalidateDeviceObjects();
	int			WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcChart(UINT uMsg,WPARAM wParam,LPARAM lParam);

	BOOL		IsTutorialMode() { return m_bTutorialMode; }
	void		SetTutorialMode(BOOL bTutorial) { m_bTutorialMode = bTutorial; }

	BOOL		IsUseShuttleTick() { return m_bUseShuttleTick; }
	void		SetUseShuttleTick(BOOL bUseShuttleTick) { m_bUseShuttleTick = bUseShuttleTick; }

	BOOL		IsUseCameraTick() { return m_bUseCameraTick; }
	void		SetUseCameraTick(BOOL bUseCameraTick) { m_bUseCameraTick = bUseCameraTick; }

	// GameData ���� �Լ�
	void		SetResourceFile(char* szFileName);
	DataHeader* FindResource(char* szRcName);
	void		SetImgBoxCenterPos(CINFImageEx * pImg);
	void		SetImgBoxCenterGoalPos(CINFImageEx * pImg);


	void		ResetLessonPosition(float fElapsedTime);
	DWORD		GetLesson() { return m_dwLesson; }
	DWORD		GetChapter() {return m_dwChapter; }
	DWORD		GetChapterState() { return m_dwChapterState; }

	void		AddTutorialGateInfo(CObjectChild* pNode);
	BOOL		IsEnableTutorialGate(D3DXVECTOR3 vPos);
	void		ClearTutorialGateInfo();
	void		MakeDisableAllTutorialGate();
	void		MakeEnableTutorialGate(D3DXVECTOR3 vPos);
	void		MakeDisableTutorialGate(D3DXVECTOR3 vPos);
	
	void		DisableTutorialGateFarDistance();

	int			CutOffFirstNumber(int& nNum,int nMaxNum);

	BOOL		IsRenderTargetCenter() { return m_bRenderTargetCenter; }

	BOOL		IsRenderInterface() { return m_bRenderInterface; }
	BOOL		IsRenderSpaceBar() { return m_bRenderSpaceBar; }

	DWORD		GetMessageType() { return m_dwMessageType; }

	void		MonsterDeadNotify(ClientIndex_t MonsterIndex);
	void		ClearTutorialMonster();

	void		SetShuttleOrderTarget();
	BOOL		GetNeedOrderTarget() { return m_bNeedOrderTarget; }
	void		SetNeedOrderTarget(BOOL bNeedOrderTarget) { m_bNeedOrderTarget = bNeedOrderTarget; }
	void		SetOrderTargetPos(int x, int y);
	BOOL		CheckCancel();
	BOOL		GetShowHelpDesk() { return m_bShowHelpDesk; }
	void		SetShowHelpDesk(BOOL bFlag) { m_bShowHelpDesk = bFlag; }
	void		SetOrderTargetPosTo2D(D3DXVECTOR3 vPos);

	void		SetTutorialEd(BOOL bTurotialEd) { m_bTutorialEd = bTurotialEd; }
	BOOL		GetTutorialEd() { return m_bTutorialEd; }

	void		SetMonsterTick();
	void		SetMonsterPosition();
	CMonsterData *	GetNearTutorialMonster();

	void		TutorialLessonSkip(float fElapsedTime);
	D3DXVECTOR3 CheckMob2Postion(D3DXVECTOR3 *vDir, D3DXVECTOR3 *vZigZag);

	// 2007-07-04 by bhsohn ���۷����� �߰�
	// ���� é���� ��Ʈ���� ���´�. 
	void		GetOpTxt(char* pTxt, char* pNarration);

	// 2007-07-23 by bhsohn ���۷����� �߰��۾�
	void		SetFirstUserIntoFreeSka(BOOL bTake);
	BOOL		GetFirstUserIntoFreeSka();
	void		ChangeChapterReady();
	BOOL		IsSystemWin()			{return m_bSystemWin;}		// �ý��� �޴��� �����ִ°�?
private:
	// 2007-07-04 by bhsohn ���۷����� �߰�	
	void		UpdateOpInfo();

	BOOL		IsCompleteAllTutorial();

	void		GetLession0Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession1Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession2Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession3Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession4Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession5Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession6Txt(DWORD dwChapter, char* pTxt, char* pNarration);
	void		GetLession7Txt(DWORD dwChapter, char* pTxt, char* pNarration);	
	void		GetLession8Txt(DWORD dwChapter, char* pTxt, char* pNarration);	
	void		GetLession9Txt(DWORD dwChapter, char* pTxt, char* pNarration);	
	void		GetLession10Txt(DWORD dwChapter, char* pTxt, char* pNarration);	
	
	void		RenderHelper();	
	void		TickHelper(float fElapsedTime);
	void		SetHelper();


private:
	CGameData*		m_pGameData;
	BOOL			m_bTutorialMode;	// Ʃ�丮�� ����� TRUE
	BOOL			m_bUseShuttleTick;	// ��Ʋ ƽ�� �ʿ��ϸ� TRUE
	BOOL			m_bUseCameraTick;	// ī�޶� ƽ�� �ʿ��ϸ� TRUE
	DWORD			m_dwLesson;			// ���� 
	DWORD			m_dwChapter;		// ����.
	DWORD			m_dwChapterState;	// �ش� ���������� ����
	float			m_fStateTime;		// �ð�(�ð� üũ �뵵�� ���)
	float			m_fRemainTime;		
	BOOL			m_bStateOk;			// ���� 
	DWORD			m_dwMessageType;	// �޽��� Ÿ��
	POINT			m_MousePt;


	BOOL			m_bNeedOrderTarget;		// ���� ���� ���� Ÿ���� �ʿ��Ѱ�..

	// 2007-07-03 by dgwoo Ʃ�丮�� ����.
	
	CINFImageEx *		m_pImgMain;
	CINFImageEx *		m_pImgChart;
	CINFImageEx *		m_pImgCari;
	CINFImageEx *		m_pImgComplete;
	CINFImageEx *		m_pImgSelect;
	CINFImageEx *		m_pImgStartB[BUTTON_STATE_NUMBER];
	CINFImageEx *		m_pImgEndB[BUTTON_STATE_NUMBER];
	CINFImageEx *		m_pImgLesson1[L1_7];
	CINFImageEx *		m_pImgLesson1Goal[L1_7];
	CINFImageEx *		m_pImgLesson2[L2_3];
	CINFImageEx *		m_pImgLesson2Goal[L2_3];
	CINFImageEx *		m_pImgLesson3[L3_2];
	CINFImageEx *		m_pImgLesson3Goal[L3_2];
	CINFImageEx *		m_pImgLesson4;
	CINFImageEx *		m_pImgLesson4Goal;
	CINFImageEx *		m_pImgLesson5[L5_2];
	CINFImageEx *		m_pImgLesson5Goal[L5_2];
	CINFImageEx *		m_pImgLesson6;
	CINFImageEx *		m_pImgLesson6Goal;
	CINFImageEx *		m_pImgLesson7;
	CINFImageEx *		m_pImgLesson7Goal;
	CINFImageEx *		m_pImgLesson8;
	CINFImageEx *		m_pImgLesson8Goal;
	CINFImageEx *		m_pImgLesson9;
	CINFImageEx *		m_pImgLesson9Goal;
	CINFImageEx *		m_pImgLesson10[L10_4];
	CINFImageEx *		m_pImgLesson10Goal[L10_4];

	// Interface���� �̹��� Ű���� �� ���콺.
	CINFImageEx *		m_pImgInterBG;
	CINFImageEx *		m_pImgInterMouseL;
	CINFImageEx *		m_pImgInterMouseR;
	CINFImageEx *		m_pImgInterMouse;
	CINFImageEx *		m_pImgInterSpace;
	CINFImageEx *		m_pImgInterShift;
	CINFImageEx *		m_pImgInterKey;

	BOOL			m_bInterMouseL;
	BOOL			m_bInterMouseR;
	BOOL			m_bInterMouse;
	BOOL			m_bInterSpace;
	BOOL			m_bInterShift;
	BOOL			m_bInterKeyW;
	BOOL			m_bInterKeyA;
	BOOL			m_bInterKeyS;
	BOOL			m_bInterKeyD;
	BOOL			m_bInterKeyC;
	BOOL			m_bInterKeyB;
	BOOL			m_bLButtonState;	// TRUE : ����
	BOOL			m_bRButtonState;	// TRUE : ����

	///////////////////////////////////////////////////////////




	float			m_fMainX;								// ��Ʈ â�� ��ġ.
	float			m_fMainY;
	BOOL			m_bLessonComplete[TUTORIAL_LESSON_COUNT];

	int				m_nStartB;
	int				m_nEndB;

	int				m_nSelect;

	BOOL			m_bSystemWin;							// �ý���â�� Ȱ��ȭ ����.
	float			m_fSystemWinX;							// �ý���â�� ��ġ.
	float			m_fSystemWinY;
	CINFImageEx *		m_pImgSysBG;							// ��� Ʋ.
	CINFImageEx *		m_pImgSysPlayB[BUTTON_STATE_NUMBER];	// ��� ����.
	CINFImageEx *		m_pImgSysAgainB[BUTTON_STATE_NUMBER];	// �ٽ� ����.
	CINFImageEx *		m_pImgSysMainB[BUTTON_STATE_NUMBER];	// ���� ȭ��.
	CINFImageEx *		m_pImgSysEndB[BUTTON_STATE_NUMBER];		// ���� ��ư.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
    CINFImageEx *		m_pImgFadeBG;							// ��� Ʋ.
#endif
	

	int				m_nSysPlayB;
	int				m_nSysAgainB;
	int				m_nSysMainB;
	int				m_nSysEndB;

	BOOL			m_bStatePlus;
	D3DXVECTOR3		m_vecGatePos;
	int				m_nImagePosition_X;
	int				m_nImagePosition_Y;
	int				m_nImageGoalPos_X;
	int				m_nImageGoalPos_Y;
	int				m_nImageInterPos_X;
	int				m_nImageInterPos_Y;

	CINFImageEx *		m_pImgETCBooster;
	CINFImageEx *		m_pImgETCBull1;						// ź��� 1��.
	CINFImageEx *		m_pImgETCBull2;						// ź��� 2��.
	CINFImageEx *		m_pImgETCHeat;
	CINFImageEx *		m_pImgETCReAtt;
	CINFImageEx *		m_pImgETCSP;
	CINFImageEx *		m_pImgETCWeapone1;					// �⺻ ����.
	CINFImageEx *		m_pImgETCWeapone2;					// ��� ����.

	CINFImageEx *		m_pImgHelpTex;						// ������ �Ʒ����ο� ������ ����.

	int				m_nOrderTargetPos_X;
	int				m_nOrderTargetPos_Y;

	BOOL			m_bChapSuccess;						// �̰����� �����ߴ°�?
	BOOL			m_bTutorialFirst;					// Ʃ�丮�������� �ѹ��� ó���ؾ��ϴ� ��.

	INT				m_nHelpTexPosX;						// ������ ������� �Ʒ� X��ġ.
	BOOL			m_bHelperRender;					// ������ ������ �ؾ��ϴ°�?
	float			m_fHelperTime;						// ���ݱ��� �������� �ð�.
	//--------------------------------------------------------------------------//
	


	int				m_nSkipBoxPosition_X;
	int				m_nSkipBoxPosition_Y;
	int				m_nStartButtonPos_X;
	int				m_nStartButtonPos_Y;
	int				m_nEndButtonPos_X;
	int				m_nEndButtonPos_Y;
	int				m_nCenterButtonPos_X;
	int				m_nCenterButtonPos_Y;
	int				m_nInputButtonPos_X;
	int				m_nInputButtonPos_Y;
	int				m_nInputMousePos_X;
	int				m_nInputMousePos_Y;


	BOOL			m_bPushStartButton;
	BOOL			m_bPushCenterButton;
	BOOL			m_bPushEndButton;
	BOOL			m_bTargetReady;

	BOOL			m_bETC;
	DWORD			m_dwETCState;

	BOOL			m_bRenderTargetCenter;

	BOOL			m_bRenderSpaceBar;
	BOOL			m_bRenderInterface;

	BOOL			m_bShowHelpDesk;

	CINFImageEx*		m_pSkipBoxImage[10];
	CINFImageEx*		m_pStartButtonImage[3];
	CINFImageEx*		m_pCenterButtonImage[3];
	CINFImageEx*		m_pEndButtonImage[3];
	CINFImageEx*		m_pImgLessonWelcome;
	CINFImageEx*		m_pImgLessonSucceed;
	CINFImageEx*		m_pImgLessonFailed;
	CINFImageEx*		m_pCurrentImage;
	CINFImageEx*		m_pCurrentGoalImage;
	CINFImageEx*		m_pCButtonImage[2];
	CINFImageEx*		m_pWButtonImage[2];
	CINFImageEx*		m_pSButtonImage[2];
	CINFImageEx*		m_pAButtonImage[2];
	CINFImageEx*		m_pDButtonImage[2];
	CINFImageEx*		m_pBButtonImage[2];
	CINFImageEx*		m_pSpaceBarImage[2];
	CINFImageEx*		m_pMouseImage[4];
	CINFImageEx*		m_pRemainTime[11];
	
	vector<TutorialGateInfo>	m_vecTutorialGateInfo;

	BOOL			m_bTutorialEd;			// 2005-10-17 by ispark Ʃ�丮���� ���� ���ΰ�? (��ŵ�� ����)

	D3DXVECTOR3		m_vecMob1;

	// 2007-07-23 by bhsohn ���۷����� �߰��۾�
	BOOL			m_bFristIntoFreeSka;

public:
	Lesson2State	m_Lesson2State;
	Lesson3State	m_Lesson3State;
	Lesson7State	m_Lesson7State;
	Lesson8State	m_Lesson8State;
	Lesson9State	m_Lesson9State;
	

};

#endif // !defined(AFX_TUTORIALSYSTEM_H__78E3D7AE_3E51_4771_8F9B_2C9CA3BB537D__INCLUDED_)

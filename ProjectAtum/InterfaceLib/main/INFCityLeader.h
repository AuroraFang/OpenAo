// INFCityLeader.h: interface for the CINFCityLeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYLEADER_H__46F2F410_1D1C_46BF_B880_E31E4A6458E0__INCLUDED_)
#define AFX_INFCITYLEADER_H__46F2F410_1D1C_46BF_B880_E31E4A6458E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "INFImageBtn.h"
#include "INFEditBox.h"
#include "INFArenaScrollBar.h"

#include "INFBase.h"

#define	LEADER_STATE_NOTICE					0			// ��������.
#define	LEADER_STATE_EXPENCE				1			// �ǰ��� 
#define	LEADER_STATE_POLL					2			// ������ ����.
#define LEADER_STATE_POLLDATE				3			// ���� ����.
#define LEADER_STATE_WARINFO				4			// ���� ����.

//#define	LEADER_STATE_POLL			���� ����.
#define LEADER_POLL_APP						1			// ��� ����.
#define	LEADER_POLL_LIST					2			// ����Ʈ ����.
#define	LEADER_POLL_INFO					3			// �ĺ� ����.



// 2009. 01. 12 by ckPark ���� ���� �ý���


//// #define LEADER_STATE_WARINFO			4			// ���� ����.
// #define LEADER_WARINFO_INFLUENCE			0			// ��������.
// #define LEADER_WARINFO_MOTHERSHIP		1			// ���� ����.
// #define LEADER_WARINFO_OUTPOST			2			// ���������� ����.
// #define LEADER_WARINFO_POINT				3			// ���� ����.
// #define LEADER_WARINFO_END				4			// end.

#define LEADER_WARINFO_INFLUENCE			0			// ��������.
#define LEADER_WARINFO_DECLAREWAR			1			// ��������
#define LEADER_WARINFO_MOTHERSHIP			2			// ���� ���
#define LEADER_WARINFO_OUTPOST				3			// ���������� ����.
#define LEADER_WARINFO_POINT				4			// ���� ����.
#define LEADER_WARINFO_END					5			// end.


#define ANI_MOTHERSHIP_NUMBER				2027300		// �� ���� �ѹ��̸�
#define BCU_MOTHERSHIP_NUMBER				2027200

// end 2009. 01. 12 by ckPark ���� ���� �ý���



// 2008-10-16 by bhsohn �������� ����, ������ ����â
// 2008-03-19 by bhsohn ����, ������ ����â
//struct structMotherShipInfo
//{
//	ATUM_DATE_TIME SummonTime;					// 2008-08-19 by bhsohn ������, ���� ���� ��ȯ �ð����� ����
//	char chAttackInfluence[64];							// ���� ����
//	char chWinInfluence[64];							// �¸� ����
//	char chMotherShipName[SIZE_MAX_MONSTER_NAME];		// ���� ���� ��
//	char chWarPoint[64];								// ��������Ʈ
//	char chTimeCap[512];								// ������ ���� �ð�	
//};
//
//struct structWarPointInfo
//{
//	ATUM_DATE_TIME SummonTime;					// 2008-08-19 by bhsohn ������, ���� ���� ��ȯ �ð����� ����
//	char chAttackInfluence[64];							// ���� ����
//	char chWinInfluence[64];							// �¸� ����
//	char MapName[SIZE_MAX_MAP_NAME];					// ������ ���̸�	
//	char chTimeCap[512];								// ������ ���� �ð�	
//};
// end 2008-03-19 by bhsohn ����, ������ ����â

class CINFCurselEditBox;	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����

class CINFImage;
class CD3DHanFont;
class CINFPilotFace;
class CINFImageEx;												// 2011. 10. 10 by jskim UI�ý��� ����
class CINFGroupImage;											// 2011. 10. 10 by jskim UI�ý��� ����
class CINFCityLeader  : public CINFBase  
{
public:
	CINFCityLeader();
	virtual ~CINFCityLeader();

	CINFCityLeader(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);		

	int WndProcPollInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcPollList(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcPollApp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void SetNotice(char *i_szNotice);
	void ChangeLeaderState(DWORD i_dState);
	DWORD GetLeaderState()   {return m_nRWindowState;}
	void ChangePollState(BYTE i_BPollState);
	void ChangeWarInfoState(BYTE i_BWarInfoState);
	void RenderPoll();
	void RenderWarInfo();
	

	int WndProcNotice(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcExpence(UINT uMsg, WPARAM wParam, LPARAM lParam);	
	int WndProcPoll(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcWarInfo(UINT uMsg,WPARAM wParam,LPARAM lParam);

	BUILDINGNPC*		m_pBuildingInfo;

	// �ǰ��� ���� ���� ������Ʈ
	void				SetExpenceInfo(float fExplate,DWORD nCumulativeExp, DWORD nExp);
	// ���� �ð� ���� ���� ó��
	void				SetNextWarInfo(ATUM_DATE_TIME OutPostNextWarTime, INT nOutPostNextWarSelectTimeChoice, vector<ATUM_DATE_TIME> vectmpTimeList);

	VOID				ReFresh();
	// �ݱ� ��ư Ŭ��
	void				OnCloseInfWnd();

	void AddCandidateList(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK * pMsg);
	void AddCandidateListDone();
	void AddOutPostInfo(SCITY_WARINFO_OUTPOST * i_pOutPostInfo);
	void SetWarInfoInfluence(MSG_FC_CITY_WARINFO_INFLUENCE_OK *pMsg);
	void ClearOutPostInfo();
	void ClearCandidateList();
	void SetCandidate(BOOL i_bCandidate);
	void SetCandidateInfo(MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK * pMsg);
	void SetCandidateInfoGuildMark(UID32_t i_nGuildUID);
	void SetPollDate(MSG_FC_CITY_POLL_REQUEST_POLL_DATE_OK * pMsg);
	void PollRegLeaderCandidateOk(INT i_nCandidateNum);
	void DeleteCandidateOk();
	INT  GetCandidateNum()   {return m_nCandidateNum;}
	void SendLeaderCandidateList();
	BOOL SetGuildMark(UID32_t i_nGuildUID);
	const MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK * GetCandidateInfo() {return &m_sCandidateInfo;}

	// 2008-03-19 by bhsohn ����, ������ ����â
	// ���� 
	void InitWarInfoMothership();	// ���� ���� �ʱ�ȭ
	void AddWarInfoMothership(BYTE byAttackBelligerence,	// ���ݼ���
											BYTE byWinBelligerence,		// �¸�����
											char* pMonName,				// ���̸�
											int nWarpoint,				// ��������Ʈ
											ATUM_DATE_TIME	timeStartWarTime,	// ���۽ð�
											ATUM_DATE_TIME	timeEndWarTime);		//����ð�	

	// ���¹�
	void InitWarInfoPointWar();
	void AddWarInfoPointWar(BYTE byAttackBelligerence,	// ���ݼ���
											BYTE byWinBelligerence,		// �¸�����
											char* pMapName,				// ���̸�											
											ATUM_DATE_TIME	timeStartWarTime,	// ���۽ð�
											ATUM_DATE_TIME	timeEndWarTime);		//����ð�	

	// ���� ��û
	void RqMotherShipDB();		//����
	void RqPointWarDB();		//������
	// end 2008-03-19 by bhsohn ����, ������ ����â



private:
	void ButtonClickWrite();
	void ButtonClickApp();
	void ButtonClickVote();
	void ButtonClickPollAppSuc();
	void ButtonClickPollApp();
	void ButtonClickDeleteCandidate();
	void ButtonClickPollInfo();
	INT  SetChangeRadioButton(INT i_nSel);
	void SendCandidateInfo();

	void OnClickExpenceOk();

	void RqNextWarTime();
	void WriteAppPLedge();

	// 2008-03-19 by bhsohn ����, ������ ����â	
	int WndProcWarInfoSub(UINT uMsg, WPARAM wParam, LPARAM lParam);



	// 2009. 01. 12 by ckPark ���� ���� �ý���
	// �ǿ� ���� �޼��� ���ν����� �� ����
	int		WndProcInfluenceInfoTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcMotherShipResultTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcWarDeclareTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcOutPostTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcPointWarTab(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// end 2009. 01. 12 by ckPark ���� ���� �ý���



	void TestMotherDB();
	void TestPointWarDB();

	// ����
	void RenderWarInfoMothership();



	// 2009. 01. 12 by ckPark ���� ���� �ý���

	// �̹��� ������ �� ���� �������� ���� ������
	void RenderWarInfoDeclareWar( void );

	// end 2009. 01. 12 by ckPark ���� ���� �ý���




	// ������
	void RenderWarInfoPointWar();	
	
	// end 2008-03-19 by bhsohn ����, ������ ����â
	
	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
	void UpdateScrollPos(float fPosX, float fPosY, float fWidth, float fHeight, int nWhellHeight);
	

private:

	CINFImageEx*			m_pImgBrigadeNoticeB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgDelB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgExpenceB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgWriteB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgRevB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgTitle;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*			m_pImgOutPostLBG;
	CINFGroupImage*			m_pImgRightBG[2];
	CINFGroupImage*			CityLeaderControl;
#else
	CINFImageEx*			m_pImgOutPostLBG;	
#endif
	CINFImageEx*			m_pImgBriNoticeBG;

	//CINFImage*			m_pImgOutPostTimeSetBG;
	CINFImageEx*			m_pImgExpenceBG;
	// 2007-10-10 by dgwoo ���ý��� 
	//-------------------------------------------------------------------------//
	CINFImageBtn*		m_pImgLPollAB;										// ������ ���� ��ư(�˸���).
	CINFImageBtn*		m_pImgLPollVB;										// ������ ���� ��ư(����������).
	CINFImageBtn*		m_pImgLDateB;										// ���� ����.
	CINFImageBtn*		m_pImgLWarInfoB;									// ���� ���� ��ư.
	CINFImageEx*			m_pImgAniBG;										// �˸��� ���.
	CINFImageEx*			m_pImgVniBG;										// ���������� ���.
	CINFImageEx*			m_pImgAppBG;										// ���� �ۼ� ���.
	CINFImageBtn*		m_pImgAppSucB;										// ��û �Ϸ�.
	CINFImageBtn*		m_pImgPAppB;										// �ĺ� ��� ��ư.
	CINFImageBtn*		m_pImgPUnAppB;										// �ĺ� Ż�� ��ư.
	CINFImageBtn*		m_pImgPInfoB;										// �ĺ� ���� ��ư.
	CINFImageEx*			m_pImgPInfoBG;										// �ĺ� ���� ���.
	CINFImageEx*			m_pImgListBG;										// ����Ʈ ���.
	CINFImageEx*			m_pImgRSelect;										// ������ ����Ʈ.
	CINFImageEx*			m_pImgPollDateABG;									// �˸��� ���� ���� ���.
	CINFImageEx*			m_pImgPollDateVBG;									// ���������� ���� ���� ���.
	CINFImageEx*			m_pImgRSelectB[2][BUTTON_BOTH_STATE_NUMBER];		// ����(���) ��ư(�յ�)
	CINFImageBtn*		m_pImgRVoteB;										// ��ǥ ��ư.

	CINFImage*			m_pImgGuildMark;									// ���� �ĺ� ��� ��ũ.
	CINFImage*			m_pImgFace;											// ���� �ĺ� ��.
	CINFPilotFace*		m_pPilotFace;										// �������� �ε��ؼ� �������ִ´�.

	CINFArenaScrollBar*	m_pScrollPollList;									// �� ����Ʈ ��ũ��.
	CINFArenaScrollBar* m_pScrollPollPledge;								// ���� ��ũ��.
	CINFArenaScrollBar* m_pScrollPollPledgeView;							// ���� ��ũ��.

	// 2008-03-19 by bhsohn ����, ������ ����â
	CINFArenaScrollBar*		m_pScrollMotherShipInfo;							// �� ����â ��ũ��

	// ��������â.
	CINFImageEx*			m_pImgWarInfoBG[LEADER_WARINFO_END];				// �� ����.
	CINFImageEx*			m_pImgWarInfoInflAni;								// �˸��� ���.
	CINFImageEx*			m_pImgWarInfoInflBcu;								// ���������� ���.


	DWORD				m_nLPollB;											// ������ ���� ��ư ����.
	DWORD				m_nLDateB;											// ���� ���� ��ư ����.
	DWORD				m_nPAppB;											// ���
	DWORD				m_nPUnAppB;											// Ż��.
	DWORD				m_nPInfoB;											// �ĺ� ���� ��ư.
	DWORD				m_nRSelectB_1;										// 0 ����. 1 ��� 
	DWORD				m_nRSelectB_2;										// ���, �븻 - ���� ��� 
	DWORD				m_nRVoteB;											// ��ǥ ��ư.

	BYTE				m_BInfluence;										// �ڽ��� ����.
	BYTE				m_BPollState;
	BOOL				m_bCandidate;										// �ĺ����ΰ�?

	vector<MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_LIST_OK> m_vecCandidateList;
	MSG_FC_CITY_POLL_REQUEST_LEADER_CANDIDATE_INFO_OK m_sCandidateInfo;
	INT					m_nCandidateNum;						// �ĺ� �ѹ�.
	INT					m_nSelectNum;							// ��Ͽ� ������ ����� ��ȣ ��밪
	ATUM_DATE_TIME		m_AppStartDate;							// �ĺ� ��û ���� ��
	ATUM_DATE_TIME		m_AppEndDate;							// �ĺ� ��û ������ ��
	ATUM_DATE_TIME		m_VoteStartDate;						// ��ǥ ���� ��
	ATUM_DATE_TIME		m_VoteEndDate;							// ��ǥ ������ ��
	ATUM_DATE_TIME		m_Election;								// ������
	//--------------------------------------------------------------------------//
	CD3DHanFont *		m_pFontExp;
	DWORD				m_nRWindowState;

	DWORD				m_nSelWarTimeRadioB;
	DWORD				m_nWarTimeMaxCount;

	DWORD				m_nBrigadeNoticeB;
	DWORD				m_nDelB;
	DWORD				m_nExpenceB;
	DWORD				m_nExpenceOkB;
	DWORD				m_nOkB;
	DWORD				m_nAppB;
	DWORD				m_nRevB;

	float				m_fExplate;
	DWORD				m_nCumulativeExp;
	DWORD				m_nExp;

	// 2007-09-05 by bhsohn ���� ������
	///////////////////���� ����///////////////////
	// ����Ʈ ��Ʈ��
	//CINFEditBox*			m_pNoticeEditBox;
	CINFCurselEditBox*			m_pNoticeEditBox;

	//CINFEditBox*			m_pEditPledge;							// ����.
	CINFCurselEditBox*			m_pEditPledge;							// ����.

	CINFEditBox*			m_pEditPledgeView;						// ���� ��.
	// ����
	CINFImageBtn*			m_pNoticeWrite;
	// ���
	CINFImageBtn*			m_pRegist;
	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
	// ��ũ�� �� 
	CINFArenaScrollBar*		m_pINFScrollBar;


	///////////////////�ǰ��� ����///////////////////
	// ����
	CINFImageBtn*			m_pExpenceOkBtn;

	///////////////////���� �ð� ���� ///////////////////
	vector<ATUM_DATE_TIME>	m_vecNextWarTimeList;
	ATUM_DATE_TIME			m_OutPostNextWarTime;
	// Ȯ�� 
	CINFImageBtn*			m_pWarNextOkBtn;

	/////////////////���� ���� //////////////////////////
	BYTE					m_bStateWarInfo;					// Ȱ��ȭ ���� ����â�� �ǹ�ȣ.

	MSG_FC_CITY_WARINFO_INFLUENCE_OK m_sInfluenceInfo;
	vector<ST_WARINFO_OUTPOST> m_vecOutPostInfo;				// �������� ����.

	// 2008-03-19 by bhsohn ����, ������ ����â
	CD3DHanFont *		m_pFontWarInfo;
	// ���� �α�
	vector<structMotherShipInfo>	m_vecMotherShipInfo;
	// ������ �α�
	vector<structWarPointInfo>	m_vecWarPointInfo;	
	CINFImageBtn*				m_pBtnMotherShipInfo;

	
	
	// 2009. 01. 12 by ckPark ���� ���� �ý���

	CD3DHanFont*				m_pFontWarDeclare;
	CINFImageBtn*				m_pBtnWarDeclare;					// �������� ��ư
	char						m_szOurThisTakeOff[MAX_PATH];		// �̹��� �츮 �� �ð�
	char						m_szOurNextTakeOff[MAX_PATH];		// ������ �츮 �� �ð�
	char						m_szEnemyThisTakeOff[MAX_PATH];		// �̹��� �� �� �ð�
	char						m_szEnemyNextTakeOff[MAX_PATH];		// ������ �� �� �ð�
	char						m_szOurThisTakeOffTime[MAX_PATH];	// �̹��� �츮 ���� �� �ð�
	char						m_szOurNextTakeOffTime[MAX_PATH];	// ������ �츮 ���� �� �ð�
	char						m_szEnemyThisTakeOffTime[MAX_PATH];	// �̹��� �� ���� �� �ð�
	char						m_szEnemyNextTakeOffTime[MAX_PATH];	// ������ �� ���� �� �ð�

public:
	// ���� �ð� ����
	void SetWarDeclareTime( const ATUM_DATE_TIME thisWarDeclare, char* pStr );
	
	
	// Getter
	inline char*				GetOurThisTakeOffTime( void ){ return m_szOurThisTakeOffTime; }
	inline char*				GetOurNextTakeOffTime( void ){ return m_szOurNextTakeOffTime; }
	inline char*				GetEnemyThisTakeOffTime( void ){ return m_szEnemyThisTakeOffTime; }
	inline char*				GetEnemyNextTakeOffTime( void ){ return m_szEnemyNextTakeOffTime; }

	// end 2009. 01. 12 by ckPark ���� ���� �ý���
};

#endif // !defined(AFX_INFCITYLEADER_H__46F2F410_1D1C_46BF_B880_E31E4A6458E0__INCLUDED_)

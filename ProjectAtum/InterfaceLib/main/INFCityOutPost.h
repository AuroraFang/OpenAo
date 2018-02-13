// INFCityOutPost.h: interface for the CINFCityOutPost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYOUTPOST_H__F4225218_78B0_44F7_A83C_87F73C35B52B__INCLUDED_)
#define AFX_INFCITYOUTPOST_H__F4225218_78B0_44F7_A83C_87F73C35B52B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 2007-09-05 by bhsohn ���� ������
#include "INFImageBtn.h"
#include "INFEditBox.h"
#include "INFArenaScrollBar.h"

#include "INFBase.h"

#define	OUTPOST_STATE_NOTICE				0
#define	OUTPOST_STATE_EXPENCE				1
#define	OUTPOST_STATE_WARTIME				2
#define OUTPOST_STATE_WARINFO				3


// 2009. 01. 12 by ckPark ���� ���� �ý���


//// #define LEADER_STATE_WARINFO			4			// ���� ����.
// #define LEADER_WARINFO_INFLUENCE			0			// ��������.
// #define LEADER_WARINFO_MOTHERSHIP		1			// ���� ����.
// #define LEADER_WARINFO_OUTPOST			2			// ���������� ����.
// #define LEADER_WARINFO_POINT				3			// ���� ����.
// #define LEADER_WARINFO_END				4			// end.

#define LEADER_WARINFO_INFLUENCE			0			// ��������.
#define LEADER_WARINFO_DECLAREWAR			1			// ���� ����.
#define LEADER_WARINFO_MOTHERSHIP			2			// ���� ����.
#define LEADER_WARINFO_OUTPOST				3			// ���������� ����.
#define LEADER_WARINFO_POINT				4			// ���� ����.
#define LEADER_WARINFO_END					5			// end.

#define ANI_MOTHERSHIP_NUMBER				2027300
#define BCU_MOTHERSHIP_NUMBER				2027200


// end 2009. 01. 12 by ckPark ���� ���� �ý���


class CINFCurselEditBox;	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
class CINFImage;
class CD3DHanFont;
class CINFGroupImage;
class CINFImageEx;
class CINFCityOutPost : public CINFBase  
{
public:
	CINFCityOutPost();
	virtual ~CINFCityOutPost();


	CINFCityOutPost(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);		
	
	void SetNotice(char *i_szNotice);
	void ChangeOutPostState(DWORD i_dState);

	int WndProcNotice(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcExpence(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcWarTime(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BUILDINGNPC*		m_pBuildingInfo;

	// �ǰ��� ���� ���� ������Ʈ
	void				SetExpenceInfo(float fExplate,DWORD nCumulativeExp, DWORD nExp);
	// ���� �ð� ���� ���� ó��
	void				SetNextWarInfo(ATUM_DATE_TIME OutPostNextWarTime, INT nOutPostNextWarSelectTimeChoice, vector<ATUM_DATE_TIME> vectmpTimeList);

	VOID				ReFresh();

	void				OnCloseInfWnd();

	void				ChangeWarInfoState(BYTE i_BWarInfoState);
	void AddOutPostInfo(SCITY_WARINFO_OUTPOST * i_pOutPostInfo);
	void ClearOutPostInfo();
	void SetWarInfoInfluence(MSG_FC_CITY_WARINFO_INFLUENCE_OK *pMsg);
	DWORD GetOutPostState() {return m_nRWindowState;}
	BOOL SetGuildMark(UID32_t i_nGuildUID);

	// 2008-10-16 by bhsohn �������� ����, ������ ����â
	// ����
	void				AddWarInfoMothership(BYTE byAttackBelligerence,	// ���ݼ���
											BYTE byWinBelligerence,		// �¸�����
											char* pMonName,				// ���̸�
											int nWarpoint,				// ��������Ʈ
											ATUM_DATE_TIME	timeStartWarTime,	// ���۽ð�
											ATUM_DATE_TIME	timeEndWarTime);		//����ð�	
	void				InitWarInfoMothership();

	// ������
	void				InitWarInfoPointWar();
	void				AddWarInfoPointWar(BYTE byAttackBelligerence,	// ���ݼ���
											BYTE byWinBelligerence,		// �¸�����
											char* pMapName,				// ���̸�											
											ATUM_DATE_TIME	timeStartWarTime,	// ���۽ð�
											ATUM_DATE_TIME	timeEndWarTime);		//����ð�	

	
private:
	void ButtonClickWrite();
	void ButtonClickApp();
	INT SetChangeRadioButton(INT i_nSel);
	// ����ð� ���� ��ư Ŭ��
	void OnClickNextWarTime();
	void RenderRadioBtn();
	void RenderWarInfo();
	int	 WndProcWarInfo(UINT uMsg,WPARAM wParam,LPARAM lParam);


	void				OnClickExpenceOk();

	void				RqNextWarTime();
	void				UpdateScrollPos(float fPosX, float fPosY, float fWidth, float fHeight, int nWhellHeight);

	// 2008-10-16 by bhsohn �������� ����, ������ ����â
	int					WndProcWarInfoSub(UINT uMsg, WPARAM wParam, LPARAM lParam);


	// 2009. 01. 12 by ckPark ���� ���� �ý���
	
	int		WndProcInfluenceInfoTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcMotherShipResultTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcWarDeclareTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcOutPostTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		WndProcPointWarTab(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	// end 2009. 01. 12 by ckPark ���� ���� �ý���


	// ���� 	
	void				RqMotherShipDB();	
	void				RenderWarInfoMothership();
	

	
	// 2009. 01. 12 by ckPark ���� ���� �ý���
	
	void RenderWarInfoDeclareWar( void );
	
	// end 2009. 01. 12 by ckPark ���� ���� �ý���





	// ����������
	void				RqPointWarDB();
	void				RenderWarInfoPointWar();

	

private:

	CINFImageEx*			m_pImgBrigadeNoticeB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgDelB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgExpenceB[BUTTON_STATE_NUMBER];
	//CINFImage*			m_pImgExpenceOkB[BUTTON_STATE_NUMBER];
	//CINFImage*			m_pImgAppB[BUTTON_STATE_NUMBER];
	// CINFImage*			m_pImgOkB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgOutPostWarTimeB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgWriteB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgRevB[BUTTON_STATE_NUMBER];
	CINFImageEx*			m_pImgRadioB[RADIOBUTTON_STATE_NUMBER];
	CINFImageBtn*		m_pImgLWarInfoB;									// ���� ���� ��ư.

#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*			m_pImgOutPostLBG;
	CINFGroupImage*			m_pImgRightBG[2];
	CINFGroupImage*			CityOutPostControl;
#else
	CINFImageEx*			m_pImgOutPostLBG;
#endif
	CINFImageEx*			m_pImgBriNoticeBG;
	CINFImageEx*			m_pImgOutPostTimeSetBG;
	CINFImageEx*			m_pImgExpenceBG;
	CINFImageEx*			m_pImgTitle;

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
	DWORD				m_nOutPostWarTimeB;
	//DWORD				m_nWriteB;
	DWORD				m_nRevB;
	//DWORD				m_bWriteMode;

	float				m_fExplate;
	DWORD				m_nCumulativeExp;
	DWORD				m_nExp;

	// 2007-09-05 by bhsohn ���� ������
	///////////////////���� ����///////////////////
	// ����Ʈ ��Ʈ��
	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����
	//CINFEditBox*			m_pNoticeEditBox;
	CINFCurselEditBox*			m_pNoticeEditBox;

	// ����
	CINFImageBtn*			m_pNoticeWrite;
	// ���
	CINFImageBtn*			m_pRegist;
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


	// ��������â.
	CINFImageEx*			m_pImgWarInfoBG[LEADER_WARINFO_END];				// �� ����.
	CINFImageEx*			m_pImgWarInfoInflAni;								// �˸��� ���.
	CINFImageEx*			m_pImgWarInfoInflBcu;								// ���������� ���.

	/////////////////���� ���� //////////////////////////
	BYTE					m_bStateWarInfo;					// Ȱ��ȭ ���� ����â�� �ǹ�ȣ.

	MSG_FC_CITY_WARINFO_INFLUENCE_OK m_sInfluenceInfo;
	vector<ST_WARINFO_OUTPOST> m_vecOutPostInfo;				// �������� ����.

	// 2008-10-16 by bhsohn �������� ����, ������ ����â
	CINFArenaScrollBar*		m_pScrollMotherShipInfo;							// �� ����â ��ũ��
	CD3DHanFont *			m_pFontWarInfo;
	CINFImageBtn*			m_pBtnMotherShipInfo;
	vector<structMotherShipInfo>	m_vecMotherShipInfo;
	// ������ �α�
	vector<structWarPointInfo>	m_vecWarPointInfo;




	// 2009. 01. 12 by ckPark ���� ���� �ý���
	
	CD3DHanFont*				m_pFontWarDeclare;
	CINFImageBtn*				m_pBtnWarDeclare;
	char						m_szOurThisTakeOff[MAX_PATH];
	char						m_szOurNextTakeOff[MAX_PATH];
	char						m_szEnemyThisTakeOff[MAX_PATH];
	char						m_szEnemyNextTakeOff[MAX_PATH];
	char						m_szOurThisTakeOffTime[MAX_PATH];
	char						m_szOurNextTakeOffTime[MAX_PATH];
	char						m_szEnemyThisTakeOffTime[MAX_PATH];
	char						m_szEnemyNextTakeOffTime[MAX_PATH];

public:
	void SetWarDeclareTime( const ATUM_DATE_TIME thisWarDeclare, char* pStr );

	inline char*				GetOurThisTakeOffTime( void ){ return m_szOurThisTakeOffTime; }
	inline char*				GetOurNextTakeOffTime( void ){ return m_szOurNextTakeOffTime; }
	inline char*				GetEnemyThisTakeOffTime( void ){ return m_szEnemyThisTakeOffTime; }
	inline char*				GetEnemyNextTakeOffTime( void ){ return m_szEnemyNextTakeOffTime; }
	
	// end 2009. 01. 12 by ckPark ���� ���� �ý���
};



#endif // !defined(AFX_INFCITYOUTPOST_H__F4225218_78B0_44F7_A83C_87F73C35B52B__INCLUDED_)

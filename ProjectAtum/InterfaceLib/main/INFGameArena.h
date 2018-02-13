// INFGameArena.h: interface for the CINFGameArena class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEARENA_H__76C45020_2F56_44DA_8390_5CE4FC12164E__INCLUDED_)
#define AFX_INFGAMEARENA_H__76C45020_2F56_44DA_8390_5CE4FC12164E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaTxt.h"
#include "INFArenaScore.h"
#include "INFArenaResult.h"
#include "INFArenaTeamInfo.h"
class CINFImage;
class CD3DHanFont;
class CINFImageEx;
struct SARENA_TEAM_CHARACTERINFO_CLIENT
{
	ClientIndex_t	ClientIndex;
	CUnitData *		pUnitData;
};

// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
struct STRUCT_WATCH_START_USER_INFO
{
#ifdef NEMERIAN_INCREASE_HPDP_LIMIT
	INT				HP;
	INT				DP;
#else
	SHORT			HP;
	SHORT			DP;
#endif
	float			CurrentHP;
	float			CurrentDP;
	BYTE			Influence;
	ClientIndex_t	ClientIndex;
	CHAR			CharacterName[SIZE_MAX_ARENA_FULL_NAME];		// ���� �̸�
};

//--------------------------------------------------------------------------//
//			�Ʒ��� ���� STATE
enum{	
	ARENA_GAME_STATE_NORMAL					= 0,		// �ƹ����µ� �ƴ�.
	ARENA_GAME_STATE_TEAMINFO,							// ������ �ִ� ����.
	ARENA_GAME_STATE_TXT,								// 
	ARENA_GAME_STATE_SCORE,								// �Ʒ��� ������ 
	ARENA_GAME_STATE_RESULT,							// ����� �����ִ� ����.
	ARENA_GAME_STATE_TIMER								// 

};
//--------------------------------------------------------------------------//

// �Ʒ��� ���
#define		ARENA_TXT_MODE_NONE			0
#define		ARENA_TXT_MODE_CNT			1
#define		ARENA_TXT_MODE_TXT			2
#define		ARENA_TXT_TEAMINFO_TXT		3	// 2008-04-17 by bhsohn �Ʒ��� ���Խ�, �ڽ��� �� ����

class CINFGameArena : public CINFBase  
{
public:
	CINFGameArena();
	virtual ~CINFGameArena();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void  Render();
	
	virtual void Tick();

	BOOL		IsArenaGameState(int nState);
	void		SetArenaGameState(int nState,BOOL bUpdate = TRUE);
	INT			GetTeamNum();
	void		ShowArenaInteraface(int nIdx, BOOL bShow);
	
	VOID		AddEnemyOperation(MSG_FC_ARENA_WATCH_START * pMsg);
	VOID		ClearEnemyOperation();
	VOID		ChangeOperationUser(BYTE Influence,int i);
	
	VOID		AddTeamUserInfo(MSG_FC_ARENA_TEAM_MEMBER_LIST * pMsg);
	VOID		CreateTeamOK(MSG_FC_ARENA_CREATE_TEAM_OK* pMsg);
	VOID		EnterTeamOK(MSG_FC_ARENA_ENTER_TEAM_OK* pMsg);
	VOID		TeamReady();
	VOID		TeamMatching();
	VOID		EnterRoom(ATUM_DATE_TIME,BYTE BPlayLimitedTime);
	VOID		StartCountWar();
	VOID		StartWar();
	VOID		SetArenaScore(MSG_FC_ARENA_ROOM_WAR_INFO* pMsg);				// ����.
	VOID		FinishHeader(MSG_FC_ARENA_ROOM_WAR_FINISH_HEADER* pMsg);		// ���� ����.
	VOID		AddResult(MSG_FC_ARENA_ROOM_WAR_FINISH* pMsg);					// ����� ���.
	VOID		EndWar(MSG_FC_ARENA_ROOM_WAR_FINISH_DONE* pMsg);						// ���� ����
	VOID		EndWarDraw(MSG_FC_ARENA_ROOM_WAR_FINISH_DRAW* pMsg);			// ��������.
	VOID		Finish();														// ��� �� ���� ���� ����.

	VOID		ReaveArena();
	// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
	//SARENA_WATCH_START_USER_INFO *	GetFindEnemyOperation(ClientIndex_t CIndex);
	STRUCT_WATCH_START_USER_INFO *	GetFindEnemyOperation(ClientIndex_t CIndex);
	
	ClientIndex_t					GetClientIndexOperation(BYTE Influence,int i);
	BOOL		ChangeWatchInfo(MSG_FC_CHARACTER_WATCH_INFO *pMsg);
	VOID		RenderOperation();
	
	// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
	//VOID		RenderUserInfo(UINT nX,UINT nY,SARENA_WATCH_START_USER_INFO* it, UINT nCount);
	VOID		RenderUserInfo(UINT nX,UINT nY,STRUCT_WATCH_START_USER_INFO* it, UINT nCount);
	
	BOOL		DeleteOperationUser(ClientIndex_t CIndex);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcOperation(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	BOOL		IsArenaReady();
	void		SendArenaLeaveTeam(BYTE	ArenaMode);				/// �Ʒ��� �� Ż��
	BOOL		IsAlreadyArenaStart();
	void		SetAlreadyArenaStart(BOOL bSet);

	// 2008-03-04 by bhsohn �Ʒ��� �ð� ����ȭ ���� ó��
	void		RefreshArenaGameTime();

	// 2008-04-17 by bhsohn �Ʒ��� ���Խ�, �ڽ��� �� ����
	void		ShowMyTeamLogo(BYTE byInfluence);
	void		HideMyTeamLogo();

	CINFArenaTxt*		m_pINFArenaTxt;				// �۾� ����
	CINFArenaScore*		m_pINFArenaScore;			// ������
	CINFArenaResult*	m_pINFArenaResult;			// �Ʒ��� ��� 
	CINFArenaTeamInfo*	m_pINFArenaTeamInfo;		// ���� ����.

	BYTE				m_nArenaMode;				// ������ �Ʒ����� ���.
	INT					m_nTeamNum;					// ������ �Ʒ��� ��ȣ.
	int					m_nArenaGameState;

	INT						m_nWarPoint;
	INT						m_nInfPoint;


	BOOL				m_bString30;					// 30���� ������� �޽���.
	BOOL				m_bString60;					// 60���� ������� �޽���.

	ATUM_DATE_TIME		m_StartTime;					// ���� �ð�.
	//ATUM_DATE_TIME		m_EndTime;						
	float				m_BPlayLimitedTime;				// ������ ����Ǵ� �ð�.

	// 2008-03-04 by bhsohn �Ʒ��� �ð� ����ȭ ���� ó��
	BYTE				m_byArenaPlayLTimeMinute;

	vector<SARENA_TEAM_CHARACTERINFO_CLIENT> m_vecMinimapInfo;

    // 2007-06-18 by dgwoo ���� ���� �ʿ��� ������ .
	BOOL				m_bShowOperUserInfo;

	CINFImageEx *			m_pOperNor;
	CINFImageEx *			m_pOperDie;
	CINFImageEx *			m_pOperSel;
	CINFImageEx *			m_pOperBCU;
	CINFImageEx *			m_pOperANI;
	CINFImageEx *			m_pImgHp;
	CINFImageEx *			m_pImgDp;

	CD3DHanFont *		m_pFontID;

	// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
//	vector<SARENA_WATCH_START_USER_INFO> m_vecOperationBCU;
//	vector<SARENA_WATCH_START_USER_INFO> m_vecOperationANI;
	vector<STRUCT_WATCH_START_USER_INFO> m_vecOperationBCU;
	vector<STRUCT_WATCH_START_USER_INFO> m_vecOperationANI;
	
	//--------------------------------------------------------------------------//
private:
	// 2008-04-21 by bhsohn �Ʒ��� ���Խ�, 1��/30�� ���� ǥ��
	void TickAreanaTimer();

private:
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	BOOL				m_bAlreadyArenaStart;	// �ʰ� ���� ������ ���� �̹� �Ʒ����� ���۵Ǿ���.

};

#endif // !defined(AFX_INFGAMEARENA_H__76C45020_2F56_44DA_8390_5CE4FC12164E__INCLUDED_)

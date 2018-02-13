// INFArenaTxt.h: interface for the CINFArenaTxt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFARENATXT_H__898FDAB2_9755_49E4_956D_4DC82A6B7065__INCLUDED_)
#define AFX_INFARENATXT_H__898FDAB2_9755_49E4_956D_4DC82A6B7065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// �Ʒ��� ī��Ʈ
#define		MAX_ARENA_CNT				6

// 2008-04-17 by bhsohn �Ʒ��� ���Խ�, �ڽ��� �� ����
// �� ����
#define		ARENA_TEAM_BLUE				0	// �����
#define		ARENA_TEAM_RED				1	// ������
#define		MAX_ARENA_TEAM_CNT			2

#define	MAX_ARENA_NUM					10	// 2012-06-07 by mspark, �Ʒ��� ���� ���� UI �۾�

class CINFImage;
class CINFImageEx;										// 2011. 10. 10 by jskim UI�ý��� ����

class CINFArenaTxt  : public CINFBase
{
public:
	CINFArenaTxt(CAtumNode* pParent);
	CINFArenaTxt();
	virtual ~CINFArenaTxt();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void Tick();

	void StartArenaCnt();
	void SetIssue(int issue,float fArenaTick = 0);
	void SetShowState(int ShowState);

	// 2008-04-17 by bhsohn �Ʒ��� ���Խ�, �ڽ��� �� ����
	void ShowMyTeamLogo(BYTE byInfluence);

	// 2012-06-07 by mspark, �Ʒ��� ���� ���� UI �۾�
	void CheckScore(int VCNDeathCount, int ANIDeathCount);
	void AddArenaTxt(MSG_FC_ARENA_ROOM_WAR_FINISH*  pArenaInfo);
	// end 2012-06-07 by mspark, �Ʒ��� ���� ���� UI �۾�
private:
	// 2008-04-17 by bhsohn �Ʒ��� ���Խ�, �ڽ��� �� ����
	void HidingMyTeamLogo();	// ����� �ִ� ����
	void HideMyTeamLogo();		// ������ ����
	void TickFadeMode();	
	// end 2008-04-17 by bhsohn �Ʒ��� ���Խ�, �ڽ��� �� ����
	

public:
private:
	CINFImageEx*	m_pArenaCnt[MAX_ARENA_CNT];					// ���� 
	CINFImageEx*	m_pArenaTxt[ARENA_ISSUE_DRAW];				// �Ʒ��� ����.
	// 2008-04-17 by bhsohn �Ʒ��� ���Խ�, �ڽ��� �� ����
	CINFImageEx*	m_pArenaTeamTxt[MAX_ARENA_TEAM_CNT];				// �Ʒ��� ��

	
	int			m_nArenaIssue;								// 
	int			m_nArenaShowState;

	// ī��Ʈ ���� 
	int			m_nArenaCnt;
	float		m_fArenaCntTick;

	// 2008-04-17 by bhsohn �Ʒ��� ���Խ�, �ڽ��� �� ����
	// ���̵� ��
	int			m_nFadeMode;
	float		m_fFadeInTime;
	DWORD		m_dwBkAlpha;
	float		m_fTeamInfoShowTime;
	// ����
	int			m_nInflIdx;

	// 2012-06-07 by mspark, �Ʒ��� ���� ���� UI �۾�
	CINFImageEx*	m_pArenaGageBaseTxt;
	CINFImageEx*	m_pArenaGageBlueTxt;
	CINFImageEx*	m_pArenaGageRedTxt;
		
	CINFImageEx*	m_pArenaTeamAverageTxt[MAX_ARENA_TEAM_CNT][MAX_ARENA_NUM];
	CINFImageEx*	m_pArenaPercentTxt[MAX_ARENA_TEAM_CNT];

	int			m_nArenaTeamAverage[MAX_ARENA_TEAM_CNT];
	int			m_nStartPosX[MAX_ARENA_TEAM_CNT];

	int			m_nBlueScore;
	int			m_nRedScore;
	// end 2012-06-07 by mspark, �Ʒ��� ���� ���� UI �۾�
};

#endif // !defined(AFX_INFARENATXT_H__898FDAB2_9755_49E4_956D_4DC82A6B7065__INCLUDED_)

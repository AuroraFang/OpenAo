// INFArenaResult.h: interface for the CINFArenaResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFARENARESULT_H__9124FFDF_D17D_4F50_850D_9C0530B81F55__INCLUDED_)
#define AFX_INFARENARESULT_H__9124FFDF_D17D_4F50_850D_9C0530B81F55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"

// ���� ���
#define	ARENA_RESULTMODE_DEATHMATCH		0
#define	ARENA_RESULTMODE_ROUND			1
#define	MAX_ARENA_RESULTMODE			2

// �Ʊ�, ����
#define	ARENA_RESULT_FRIEND			0		// �Ʊ�
#define	ARENA_RESULT_ENEMY			1		// ����
#define	MAX_ARENA_RESULT			2

// ����������/�˸���
#define	ARENA_RESULTINFLUENCE_BCU				0		// ����������
#define	ARENA_RESULTINFLUENCE_ANI				1		// �˸���
#define	MAX_ARENA_RESULTINFLUENCE				2

// �¸�/�й�
#define	ARENA_RESULTSTATE_DRAW					-1		// ���º�-1
#define	ARENA_RESULTSTATE_WIN					0		// ��
#define	ARENA_RESULTSTATE_LOSE					1		// ��
#define	MAX_ARENA_RESULTSTATE					2

// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
struct STRUCT_ARENA_ROOM_WAR_FINISH
{
	BYTE			Influence;				// 2007-04-17 by dhjin, ���� 
	BYTE 			ShootingDown;			// 2007-04-17 by dhjin, ���߼� 
	BYTE 			SufferingAttack;		// 2007-04-17 by dhjin, �ǰݼ� 
	BYTE			LostPoint;				// 2007-06-01 by dhjin, LostPoint
	USHORT			UnitKind;				// 2007-04-17 by dhjin, ��� ���� 
	CHAR			CharacterLevelRank[SIZE_MAX_LEVELRANK];	// 2007-04-17 by dhjin, ���� ���� ���
	// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
	CHAR			CharacterName[SIZE_MAX_ARENA_FULL_NAME];
	//FLOAT			Average;				// 2012-06-07 by mspark, �Ʒ��� ���� ���� UI �۾�
	INT				TeamAverage;			// 2012-06-07 by mspark, �Ʒ��� ���� ���� UI �۾�
};


class CINFImage;
class CD3DHanFont;

class CINFArenaResult  : public CINFBase
{
public:
	CINFArenaResult();
	virtual ~CINFArenaResult();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void AddArenaResult(MSG_FC_ARENA_ROOM_WAR_FINISH* pMsg);
	void UserInfoClear();
	void SetIssue(int issue);

private:
	void RenderWinLose(float fStartBkX, float fStartBkY, POINT ptArenaBK, int nArenaIdx,CHARACTER myShuttleInfo);
	void RenderScroll(int nArenaIdx);
	
	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);

	int GetInfluenIndex(BYTE byBelligerence);
	void RenderUserInfo(float fStartBkX, float fStartBkY, POINT ptArenaBK, int nArenaIdx);	
	void GetUnitKindString(USHORT i_nUnitKind,char* pTxt);
	int GetFriendIndex(BYTE byBelligerence);

	void TestDB();

private:
	CINFImageEx*	m_pArenaResultBk[MAX_ARENA_RESULTMODE];	
	CINFImageEx*	m_pArenaResultInfluence[MAX_ARENA_RESULTINFLUENCE];		// ���� 
	CINFImageEx*	m_pArenaResultState[MAX_ARENA_RESULTSTATE];		// ���� 	

	float		m_fBackPosX;
	float		m_fBackPosY;

	// ��ư 
	CINFImageBtn*			m_pGoCiyBtn;

	// ��ũ�� ��
	CINFArenaScrollBar*		m_pINFArenaScrollBar[MAX_ARENA_RESULT];

	int				m_nArenaResultMode;
	int				m_nArenaResultInfluence[MAX_ARENA_RESULT];		// ���� 	

	// 2008-02-25 by bhsohn �Ʒ��� ���� ���� ������ ǥ���ϰ� ����
	//vector<MSG_FC_ARENA_ROOM_WAR_FINISH>		m_vecUserInfo[MAX_ARENA_RESULTINFLUENCE];				
	vector<STRUCT_ARENA_ROOM_WAR_FINISH>		m_vecUserInfo[MAX_ARENA_RESULTINFLUENCE];
	
	CD3DHanFont*			m_pFontUserInfo;
	CD3DHanFont*			m_pFontTotal;

	int						m_nScrollPos;

	float					m_fShowTime;					// ������� �����ִ� �ð�.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*		m_pRenewArenaResultBackImage;
#endif

	// 2012-06-07 by mspark, �Ʒ��� ���� ���� UI �۾�
	int						m_nBCURanking;
	int						m_nANIRanking;
	// end 2012-06-07 by mspark, �Ʒ��� ���� ���� UI �۾�
	
};

#endif // !defined(AFX_INFARENARESULT_H__9124FFDF_D17D_4F50_850D_9C0530B81F55__INCLUDED_)

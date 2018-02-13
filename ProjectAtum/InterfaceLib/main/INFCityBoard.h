// INFCityBoard.h: interface for the CINFCityBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCITYBOARD_H__B48F0081_1E45_4458_804D_9BC8E462F689__INCLUDED_)
#define AFX_INFCITYBOARD_H__B48F0081_1E45_4458_804D_9BC8E462F689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 2009-02-13 by bhsohn ���� ��ŷ �ý���
#include "WorldRankManager.h"

class CINFImage;
class CD3DHanFont;
class CBoardData;

typedef struct stSignBoardString
{
	BOOL			IsInfluenceLeader;								// 2006-04-17 by cmkwon, TRUE:����������, FALSE:��� Ȥ�� ������
	INT				StringIndex;									// ������ ��Ʈ�� �ε���
	ATUM_DATE_TIME	SignBoardExprieATime;							// ������ ��Ʈ�� ���� �ð�
	char			SingBoardString[SIZE_MAX_SIGN_BOARD_STRING];	// ������ ��Ʈ��

	stSignBoardString()
	{
		IsInfluenceLeader = FALSE;
		StringIndex = -1;
		SignBoardExprieATime.Reset();
		memset(SingBoardString, 0x00, SIZE_MAX_SIGN_BOARD_STRING);
	}
} SIGN_BOARD_STRING;

// 2009-02-13 by bhsohn ���� ��ŷ �ý���
// �ִ���� ��ŷ ���� 
#define MAX_WORLDRANK_ITEM		5		// �ִ� �����ۼ�
#define MAX_WORLDRANK_LINE		5		// �ִ� ���μ�
// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

class CINFCityBoard  
{
private:
	CBoardData	*		m_pBoard;
	CBoardData	*		m_pBoardBack;
	
	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	CBoardData	*		m_pWBoard;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
    CBoardData	*		m_pWBoard2;
#endif
	
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

	// 2006-04-18 by ispark
	// ������ �޼��� ����
	vector<SIGN_BOARD_STRING*> m_vecSignBoardString;

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���	
	float				m_fCheckWorldRankTime;
	int					m_nWRankShowIdx;
	BOOL				m_bFindWRInfo;
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

public:
	HRESULT				InitDeviceObjects(int nMapNumber);
	HRESULT				RestoreDeviceObjects();
	HRESULT				DeleteDeviceObjects();
	HRESULT				InvalidateDeviceObjects();
	void				Render(float* fOffset);
	void				Tick(float fElapsedTime);

	void				DelString(int nStringIndex);
	void				DelAllString();
	void				SetString(MSG_FC_WAR_SIGN_BOARD_INSERT_STRING* pStr);
	void				SetContributionPoint(MSG_FC_WAR_UPDATE_CONTRIBUTION_POINT_OK* pMsg);
	void				SendReqSignBoardStringList();

	void				DrawSignBoardString();

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	void				TickWorldRank(float fElapsedTime);
	HRESULT				InitDeviceObjects_WBoard(int nMapNumber);
	int					SetWorldRankTitle(int i_nWRankShowIdx, int i_nBoardStringIndex);
	int					SetWorldRankString(int i_nWRankShowIdx, int i_nBoardStringIndex, int i_Rank, structRankInfo *p_struWorldRank);
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

public:
	CINFCityBoard();
	virtual ~CINFCityBoard();

};

#endif // !defined(AFX_INFCITYBOARD_H__B48F0081_1E45_4458_804D_9BC8E462F689__INCLUDED_)

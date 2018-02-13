// INFOpMain.h: interface for the CINFOpMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPMAIN_H__53FA5783_C8E4_4243_81AF_D49BE0369316__INCLUDED_)
#define AFX_INFOPMAIN_H__53FA5783_C8E4_4243_81AF_D49BE0369316__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFOpWnd.h"
#include "INFOpUserHelper.h"
//#include "INFOpNPC.h"
// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����	
#include "INFOpInfo.h"
// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����	

#define	SHOW_OP_USER_HELPER		0
#define	SHOW_OP_WND				1

// ������ ���۽ÿ� ���� ����
typedef struct  
{
	MapIndex_t	MapIndex;
	float fSummonLiveTime;
	BOOL bPopupOp;	
	ATUM_DATE_TIME timeSummon;	// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	short MapInfluenceType;		// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����

} structWarMonsterInfo;

// �ð� ������ ������ ������ ���� ����
typedef struct  
{
	MapIndex_t	MapIndex;
	short MapInfluenceType;
	char chStringTxt[1024];
	ATUM_DATE_TIME timeSummonMonsterTime;
} structPreWarMonsterInfo;

class CINFOpMain  : public CINFBase
{
public:
	CINFOpMain();
	virtual ~CINFOpMain();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	void	Tick(DWORD dwGameState);
	void	RenderOpHelper(DWORD dwGameState);
	void	RenderOpWnd();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	DataHeader * FindResource(char* szRcName);
	
	
	void ShowOpUserHelper(BOOL bShow, int nHelpMode, char* pTxt=NULL);
	void ShowOpWnd(BOOL bShow);


	// 2009. 01. 12 by ckPark ���� ���� �ý���

	// ���� ���� ���۷�����
	void ShowOpWarDeclare(BYTE Influence, ATUM_DATE_TIME MSWarStartTime, BOOL GiveUp, BYTE SelectCount);

	// end 2009. 01. 12 by ckPark ���� ���� �ý���

	void ShowOpMissionComplete(INT i_nExpOfCompensation, BYTE i_byBonusStatOfCompensation, vector<QUEST_PAY_ITEM_INFO> vecQuestPay);
	void AddStrategyPoint(char *pTxt, MapIndex_t	MapIndex, SHORT	MapInfluenceType, ATUM_DATE_TIME		CreateTime);
	void ShowOpBossMonsterSummon(BYTE byBelligerence, int nHour, int nMinute);
	
	void OnUserHelperClickCloseBtn(int nHelperMode);

	void SetNarrationSound(char* pNarrationSound);

	void CloseFirstUserExplain();

	// ���� �ı�
	void DetroySummonMonster(MapIndex_t	MapIndex);

	VOID ShowOpWndTxt(char* pTxt);
	BOOL IsRevivalUseItemWarMonster(MapIndex_t i_MapIndex);							// ��Ȱ �������� ����Ҽ� �ִ� ���ΰ�(��������)?

	// 2007-11-19 by bhsohn ���۷����� ������ �ؽ�Ʈ ó��
	VOID ShowOpWndMultiTxt(vector<string>* pvecEditText);
	
	// 2008-03-07 by bhsohn ��������, �Ʒ��� �̿� ���ϰ� ����
	BOOL IsDoingStrategyWar();

	// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	void GetStrategyWarInfo(int nIdx, MapIndex_t	*o_MapIndex, ATUM_DATE_TIME *o_timeSummon, short *o_MapInfluenceType);
	int  GetStrategyWarCnt();

	// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	CINFOpInfo* GetOpInfo() { return m_pINFOpInfo; };
	// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	

private:
	HRESULT GetResourceFile(char* szFileName);
	BOOL IsShowWnd(int nIdx);
	void AddStrategyPointInfo(MapIndex_t	MapIndex, short MapInfluenceType, char* pTxt, ATUM_DATE_TIME *pSummonMonsterTime);
	void AddPreStrategyPointInfo(MapIndex_t	MapIndex, short MapInfluenceType, char* pTxt, ATUM_DATE_TIME *pSummonMonsterTime);

	void RefreshWarMonsterInfo();
	void TickWarMonsterInfo();

	void PopupWarSummonLiveTime10Minute(MapIndex_t	MapIndex);

private:
	CGameData				*	m_pResGameData ;

	CINFOpWnd*				m_pINFOpWnd;
	CINFOpUserHelper*		m_pINFOpUserHelper;
	// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	CINFOpInfo*				m_pINFOpInfo;
	// end 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
	//CINFOpNPC*		m_pINFOpNPC;

	DataHeader*			m_pDataHeader;

	int					m_nShowWnd;

	vector<structWarMonsterInfo>	m_vecWarMonsterInfo;	// ������ ����
	vector<structPreWarMonsterInfo>	m_vecPreWarMonsterInfo;	// �ð������� �������� ������ ����
	float				m_fWarMonsterCreated;

	char				m_chNarration[64];
	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	char				m_chErNarration[64];

	float				m_fNarrationTime;
};

#endif // !defined(AFX_INFOPMAIN_H__53FA5783_C8E4_4243_81AF_D49BE0369316__INCLUDED_)

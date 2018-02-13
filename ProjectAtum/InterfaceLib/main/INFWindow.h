// INFWindow.h: interface for the CINFWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFWINDOW_H__D024D596_0FBA_44E2_B6DA_E37D3D328FD8__INCLUDED_)
#define AFX_INFWINDOW_H__D024D596_0FBA_44E2_B6DA_E37D3D328FD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
//���� â���� ��ȭ�� ������ ���¸� ��Ÿ���� �͵�..
#define	LEFT_WEAPON_WINDOW_INIT		0	// ó�� ���� ����â�� ������� �Ҷ�.
#define	LEFT_WEAPON_WINDOW_SHOWING	1	// ���� ����â�� �����ִ� ����..
#define	LEFT_WEAPON_WINDOW_HIDING	2	// ���� ����â�� ����� ����..
#define LEFT_WEAPON_WINDOW_END		3	// �����ִ� �׼��� �Ϸ�(�������� ����)
#define	RIGHT_WEAPON_WINDOW_INIT	4
#define	RIGHT_WEAPON_WINDOW_SHOWING	5
#define	RIGHT_WEAPON_WINDOW_HIDING	6
#define RIGHT_WEAPON_WINDOW_END		7	// �����ִ� �׼��� �Ϸ�(���������� ����)
#define LEFT_WINDOW_INIT			8	// ���ʿ� ������(��Ƽ,�κ���)�� �����ֱ� �����ϴ� ����.
#define LEFT_WINDOW_SHOWING			9	// �����츦 �������� ��.
#define LEFT_WINDOW_HIDING			10	// �����츦 ���ߴ� ��.
#define LEFT_WINDOW_END				11	// �۾��� ��ħ.. 
#define RIGHT_WINDOW_INIT			12
#define RIGHT_WINDOW_SHOWING		13
#define RIGHT_WINDOW_HIDING			14
#define RIGHT_WINDOW_END			15


#define SIZE_WEAPON_Y				71
#define SIZE_WEAPON_X				101
#define SIZE_ROLL_X					25
#define SIZE_ROLL_Y					36
//#define SIZE_RNORMAL_WINDOW_X		219
#define SIZE_RNORMAL_WINDOW_X		426
#define SIZE_NORMAL_WINDOW_X		219
#define SIZE_BIG_WINDOW_X			445
#define SIZE_NORMAL_WINDOW_Y		450
#define SIZE_SUPPLY_WINDOW_X		423
#define SIZE_SUPPLY_WINDOW_Y		336
#define SIZE_ARENA_WINDOW_X			567
#define SIZE_ARENA_WINDOW_Y			278
#define	SIZE_CITYLEADER_WINDOWL_X	184
#define	SIZE_CITYLEADER_WINDOWL_Y	275
#define	SIZE_CITYLEADER_WINDOWR_X	338
#define	SIZE_CITYLEADER_WINDOWR_WARINFO_X	536
#define	SIZE_CITYLEADER_WINDOWR_Y	275
#define	LEFT_WINDOW_MAX_Y			(g_pD3dApp->GetBackBufferDesc().Height-(600-448))//DEAULT_WINDOW_POS_Y*2 - SIZE_ROLL_Y
#define DEAULT_WINDOW_POS_Y			(350.0f*(float)g_pD3dApp->GetBackBufferDesc().Height / 600.0f)
#define	RIGHT_WINDOW_MAX_Y			g_pD3dApp->GetBackBufferDesc().Height-SIZE_ROLL_Y

#define DELETE_MASSAGEBOX_TIME		30.0f

#define INFO_LINE					14

#define SIZE_SKILL_SHOP_WINDOW_X	720	// 2008-08-22 by bhsohn EP3 �κ��丮 ó��

// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
#define	WM_ADDMSGBOX	(WM_USER+18000)
// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

#define SIZE_RIGNT_WINDOW_X			295	  
#else
//���� â���� ��ȭ�� ������ ���¸� ��Ÿ���� �͵�..
#define	LEFT_WEAPON_WINDOW_INIT		0	// ó�� ���� ����â�� ������� �Ҷ�.
#define	LEFT_WEAPON_WINDOW_SHOWING	1	// ���� ����â�� �����ִ� ����..
#define	LEFT_WEAPON_WINDOW_HIDING	2	// ���� ����â�� ����� ����..
#define LEFT_WEAPON_WINDOW_END		3	// �����ִ� �׼��� �Ϸ�(�������� ����)
#define	RIGHT_WEAPON_WINDOW_INIT	4
#define	RIGHT_WEAPON_WINDOW_SHOWING	5
#define	RIGHT_WEAPON_WINDOW_HIDING	6
#define RIGHT_WEAPON_WINDOW_END		7	// �����ִ� �׼��� �Ϸ�(���������� ����)
#define LEFT_WINDOW_INIT			8	// ���ʿ� ������(��Ƽ,�κ���)�� �����ֱ� �����ϴ� ����.
#define LEFT_WINDOW_SHOWING			9	// �����츦 �������� ��.
#define LEFT_WINDOW_HIDING			10	// �����츦 ���ߴ� ��.
#define LEFT_WINDOW_END				11	// �۾��� ��ħ.. 
#define RIGHT_WINDOW_INIT			12
#define RIGHT_WINDOW_SHOWING		13
#define RIGHT_WINDOW_HIDING			14
#define RIGHT_WINDOW_END			15


#define SIZE_WEAPON_Y				71
#define SIZE_WEAPON_X				101
#define SIZE_ROLL_X					25
#define SIZE_ROLL_Y					36
//#define SIZE_RNORMAL_WINDOW_X		219
#define SIZE_RNORMAL_WINDOW_X		426
#define SIZE_NORMAL_WINDOW_X		219
#define SIZE_BIG_WINDOW_X			445
#define SIZE_NORMAL_WINDOW_Y		275
#define SIZE_SUPPLY_WINDOW_X		423
#define SIZE_SUPPLY_WINDOW_Y		336
#define SIZE_ARENA_WINDOW_X			567
#define SIZE_ARENA_WINDOW_Y			278
#define	SIZE_CITYLEADER_WINDOWL_X	184
#define	SIZE_CITYLEADER_WINDOWL_Y	275
#define	SIZE_CITYLEADER_WINDOWR_X	338
#define	SIZE_CITYLEADER_WINDOWR_WARINFO_X	536
#define	SIZE_CITYLEADER_WINDOWR_Y	275
#define	LEFT_WINDOW_MAX_Y			(g_pD3dApp->GetBackBufferDesc().Height-(600-448))//DEAULT_WINDOW_POS_Y*2 - SIZE_ROLL_Y
#define DEAULT_WINDOW_POS_Y			(350.0f*(float)g_pD3dApp->GetBackBufferDesc().Height / 600.0f)
#define	RIGHT_WINDOW_MAX_Y			g_pD3dApp->GetBackBufferDesc().Height-SIZE_ROLL_Y

#define DELETE_MASSAGEBOX_TIME		30.0f

#define INFO_LINE					14

#define SIZE_SKILL_SHOP_WINDOW_X	720	// 2008-08-22 by bhsohn EP3 �κ��丮 ó��

// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
#define	WM_ADDMSGBOX	(WM_USER+18000)
// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

#define SIZE_RIGNT_WINDOW_X			295
#endif

class CD3DHanFont;
class CINFImage;
class CINFImageEx;							 // 2011. 10. 10 by jskim UI�ý��� ����
class CINFGroupImage;
class CINFMessageBox : public CAtumNode  
{
public:
	int				m_nMsgType;
	int				m_nX;
	int				m_nY;
	int				m_nCx;
	int				m_nCy;
	CD3DHanFont*	m_pFontMessage;
	CD3DHanFont*	m_pFontInput[2];
	char			m_strMessage[256];
	char			m_strInputMessage[256];
	int				m_nButtonState[2];// ok, cancel
	BOOL			m_bLockWindow;
	POINT			m_ptMouse;
	DWORD			m_dwData;
	int				m_nAllButtonState;
	int				m_nAllNumber;
	int				m_nAllCurrentData;
	char			m_strTimeMessage[256];				// �ð��� �ִ� �޽��� �ڽ��� �޽���.
	float			m_fLiveTime;
	UID64_t			m_UniqueNumber;

	CD3DHanFont*	m_pFontRadio;
	int				m_nRadioCount;	
	int				m_nSelectRadio;	

	// 2013-04-05 by bhsohn ������ 5�� ��Ȱ �ý��� �߰�
	char			m_strTimeMessage1[256];				// �ð��� �ִ� �޽��� �ڽ��� �޽���.
	
public:
	CINFMessageBox(CAtumNode* pParent);
	virtual ~CINFMessageBox();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetPos(int x, int y, int cx, int cy);
	
	// 2013-06-26 by bhsohn �Ƹ� �÷��� �߰� ����
//	void SetText(char* strText, int nMsgType);
	void SetText(char* strText, int nMsgType, BOOL bChRtn=FALSE);
	// END 2013-06-26 by bhsohn �Ƹ� �÷��� �߰� ����

	// 2013-04-05 by bhsohn ������ 5�� ��Ȱ �ý��� �߰�
//	void OnButtonClick(int i);
	void OnButtonClick(int i, BOOL bTimerClick=FALSE);
	// END 2013-04-05 by bhsohn ������ 5�� ��Ȱ �ý��� �߰�
	void GetInputMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetData(DWORD dwData1, DWORD dwData2=0, UID64_t UniqueNumber = 0);
	void SetMessageBoxTitleChange(char *strTitle);
	void ClearMessageBoxString(){ ZERO_MEMORY(m_strInputMessage);}
	void SetInputStrName(char * strName);
	void ResetMessageString();
	char* GetMsgString(char* strMsg);								// 2006-07-24 by ispark

	// 2009-01-16 by bhsohn ���̽�ƽ �߰� ����/��� �߰�
	BOOL IsPossiblOkCancel();
	// end 2009-01-16 by bhsohn ���̽�ƽ �߰� ����/��� �߰�

	// 2013-04-05 by bhsohn ������ 5�� ��Ȱ �ý��� �߰�
	void SendCharacterDeadGameStart(BOOL bGoCityMap);
	void GetMsg1String(char* strMsg);
	void SetTimeMessage1(char* pMsg);	
	// END 2013-04-05 by bhsohn ������ 5�� ��Ȱ �ý��� �߰�

	// 2013-04-23 by ssjung �߿� ������ ������ 2�� ���(����) �˾�â ����
	BOOL GetWillbeDeleteFlag(){return m_isWillbeDelete;}
protected:
	void CheckStringMsg();

	// 2007-08-07 by bhsohn ����Ŀ ������ �߰�
	void UseSpeakerItem(int nNotOk, char* strInputMessage);

	// 2008-12-30 by bhsohn ������ ä�� ���� ī�� ��ȹ��
	void UseLeaderBanChatTItem(int nNotOk, char* strInputMessage);

	// 2007-09-05 by bhsohn ���� ������
	// ȯ���� ��û
	void OnSendRequestExpence(int nCout);

	void RenderCashMsgBox(int nX, int nY, char* pTxt);

	// 2016-12-23 panoskj
	bool EnterMsgbox() const;

private:
	// 2007-10-16 by bhsohn Ư�� �۾��� ����� ó��
	CD3DHanFont*	m_pFontBold;

	// 2013-04-23 by ssjung �߿� ������ ������ 2�� ���(����) �˾�â ����
	BOOL			m_isWillbeDelete;				//�̷��� ���� �޽���â �� ���� 
};

class CINFWindow : public CINFBase  
{
public:
	CINFWindow(CAtumNode* pParent);
	virtual ~CINFWindow();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void DrawLeftWindow(int x, int y, int cx, int cy, BOOL bInven=FALSE);
	void DrawRightWindow(int x, int y, int cx, int cy);
	void RenderCenterWindow(int x, int y, int cx, int cy, BOOL bRenderClose=FALSE);
	void RenderItemInfoBox(int x, int y, int cx, int cy);

	// message box func
	void RenderMessaegBox();
	int MsgBoxWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void DeleteMsgBox(int nType);
	// 2013-06-26 by bhsohn �Ƹ� �÷��� �߰� ����
//	void AddMsgBox(char* strMsg, int nType, DWORD dwData1=0, DWORD dwData2=0, int nLiveTime=0,UID64_t UniqueNumber=0, char* strName=NULL);
	void AddMsgBox(char* strMsg, int nType, DWORD dwData1=0, DWORD dwData2=0, int nLiveTime=0,UID64_t UniqueNumber=0, char* strName=NULL, BOOL bChRtn=FALSE);
	// END 2013-06-26 by bhsohn �Ƹ� �÷��� �߰� ����
	void AddTimeMsgBox(char* strMsg, int nType, int nLiveTime, char* strTime, DWORD dwData1 = 0, DWORD dwData2 = 0,UID64_t UniqueNumber = 0);
	void AddRadioOption(char *strRadio,  int nRadioArrayType = 2);
	void SetMessageBoxTitleChange(char *strTitle, int nMsgType);
	BOOL IsExistMsgBox( int nType);
	void RadioClear() {m_vecRadioString.clear();}	
	
	// message box option
	void MessageBoxProgressOption(int nType, DWORD dwData1, DWORD dwData2, char* strName);
	void SetMessageFlag(BOOL nFlag){m_bMessageBoxFlag = nFlag;}
	void ClearAllMessageString();
	void ResetMessageString();

	HRESULT InitMsgBoxObjects();
	HRESULT RestoreMsgBoxObjects();
	HRESULT InvalidateMsgBoxObjects();
	HRESULT DeleteMsgBoxObjects();

	BOOL SearchMsgBox(int nType);

	// 2007-11-12 by bhsohn ĳ�������� �޽��� �ڽ� ó��
	void RenderBlackBK(int x, int y, int cx, int cy, BOOL bRenderClose=TRUE);

	// 2008-07-14 by bhsohn EP3 ���� ó��
	BOOL IsExistMsgBoxString( int nType, char* pInputMessage);

	// 2009-01-16 by bhsohn ���̽�ƽ �߰� ����/��� �߰�
	void OnButtonClick(int i);// if i==0 then ok, if i==1 then cancel
	// ebd 2009-01-16 by bhsohn ���̽�ƽ �߰� ����/��� �߰�

	// 2013-04-05 by bhsohn ������ 5�� ��Ȱ �ý��� �߰�
	float CheckHellMapLiveTime(int nType, int nLiveTime, CINFMessageBox* pMsgBox);
	void InitHellMapLiveTime();

public:
	CINFImageEx*	m_pBoxImage[9];										 // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*	m_pRollImage[4];
	CINFImageEx*	m_pXclose;
	CINFImageEx*	m_pOkButton[4];
	CINFImageEx*	m_pCancelButton[4];
	CINFImageEx*	m_pAllBox;
	CINFImageEx*	m_pAllButton[2]; // 0 : over, 1 : push
	CINFImageEx*	m_pRadioButton[2]; // 0 : over, 1 : push
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*	m_pTradeBase;
#else
	CINFImageEx*	m_pTradeBase;
#endif

	BOOL			m_bLeftWindow ;			//���� �Ѿ� â�̳� �κ��丮 â�� ������ ��ġ ��ȭ�� �����Ұ��ΰ�?
	BOOL			m_bRightWindow ;
	int				m_nLeftWindowInfo;
	BOOL			m_bLockWindowPosY[2];
	POINT			m_ptMouse[2];
	BOOL			m_bMessageBoxFlag;
	
	vector<string>	m_vecRadioString;
	int				m_nRadioButtonArray;

	vector<CINFMessageBox*>	m_vecMessageBox; // message list
// 	vector<CINFMessageBox*> m_vecStandByMessageBox;

private:
	// 2007-11-12 by bhsohn ĳ�������� �޽��� �ڽ� ó��
	CINFImageEx*	m_pBoldBoxImage[9];

	// 2013-04-05 by bhsohn ������ 5�� ��Ȱ �ý��� �߰�
	float			m_fAllHallMapLiveWndTime;
};

#endif // !defined(AFX_INFWINDOW_H__D024D596_0FBA_44E2_B6DA_E37D3D328FD8__INCLUDED_)

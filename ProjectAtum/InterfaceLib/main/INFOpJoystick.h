// INFOpJoystick.h: interface for the CINFOpJoystick class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPJOYSTICK_H__F4DB1741_5A7A_46D1_9779_1808E35488D6__INCLUDED_)
#define AFX_INFOPJOYSTICK_H__F4DB1741_5A7A_46D1_9779_1808E35488D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// ���̽�ƽ �ɼ� 
#define		JOY_OPTION_RADIO_ONOFF		0		// ���̽�ƽ �������
#define		JOY_OPTION_RADIO_FEED		1		// ���̽�ƽ ��������
#define		MAX_JOY_OPTION_RADIO		2


// �Ѱ� ������ ǥ���� 
#define ONEPAGE_JOY_SETUP_X				5
#define ONEPAGE_JOY_SETUP_Y				10

class CINFImage;
class CINFImageRadioBtn;
class CINFListBox;
class CINFImageBtn;
class CINFArenaScrollBar;
class CINFImageComboBoxEx;
class CSetupConfig;
class CINFImageEx;
class CINFGroupImage;

class CINFOpJoystick  : public CINFBase  
{
public:
	CINFOpJoystick(CAtumNode* pParent);
	virtual ~CINFOpJoystick();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	void Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL IsWndRect(POINT ptPos);
	BOOL IsShowWnd();
	BOOL IsMouseCaps(POINT ptPos);
	void ShowWnd(BOOL bShowWnd, POINT *ptPos=NULL);

	void InitString();
	void SetString(char* i_vecDesc);
	POINT GetWndPos();

	void UpdateString(int nIdx, char* pStr);
	void SaveAsJoysticSetupInfo(char* pSaveAsFilename);


private:
	int OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void RenderCenterWindow(int x, int y, int cx, int cy);

	void UpdateBtnPos(int nBackPosX, int nBackPosY);
	void UpdateOptionInterface(structJoyStickOptionInfo* pOptionInfo);
	void SetRadioInfo(int nMainId, BOOL bRadioCheck);

	void InitJoysticSense();
	void FindJoyFile();
	void RenderKeyInfomation();

	void UpdateKeyInfomation();
	void RenderClickKey();

	void UpdateKeyComboControls();
	void UpdateKeyCombo(structJoystickSetupKeyTable	*i_pstruKeyTable, int i_nPosY, int i_nPosX);
	structJoystickSetupKeyTable	 *GetJoystickSetupKeyTable(int nKeyIndex);
	void AllComboBoxHide(int i_nPosY, int i_nPosX);
	BOOL OnLButtonDownUpdateComoboBox(POINT pt);

	void OnClickSaveBtn();
	void OnClickSaveAsBtn();

	void SelComboBoxRefresh(int i_nSelectComboY, int i_nSelectComboX);
	void InitSetupKeyTable();
	void InifJoystickList();

	int CheckJoySticBtn();
	void OnDeviceChange(int nOldCursel) ;

private:
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*	m_pBoxImage;
#else
	CINFImageEx*	m_pBoxImage;		// ����̹���
#endif
	CINFImageEx*	m_pSelImage;		// �����̹���

	CINFListBox*	m_pComboJoySense;
	CINFListBox*	m_pComboJoyFile;
	CINFListBox*	m_pComboJoyDevice;

	CINFImageComboBoxEx*	m_pComboJoySetup[ONEPAGE_JOY_SETUP_Y][ONEPAGE_JOY_SETUP_X];
	
	CINFImageRadioBtn*		m_pJoyOpRadioBtn[MAX_JOY_OPTION_RADIO][MAX_OPTION_SYS_CNT];

	BOOL		m_bShowWnd;
	int			m_nMaxLine;
	
	POINT					m_ptBkPos;	
	BOOL					m_bMove;
	POINT					m_ptCommOpMouse;

	int						m_nWidth;
	int						m_nHeight;

	CD3DHanFont		*		m_pFontTxt;

	structJoyStickOptionInfo	m_struJoyStickOptionInfo;

	CINFImageBtn*		m_pOptionJoySave;		// ����
	CINFImageBtn*		m_pOptionClose;			// �ݱ�
	CINFImageBtn*		m_pOptionJoySaveAs;		// ���̸����� ����

	CINFArenaScrollBar*			m_pScroll;

	structJoystickSetupKeyTable*		m_pJoystickSetupKeyTable;

	// ���� ���̰� �ִ� �޺� �ڽ�
	int						m_nShowItemComboX;
	int						m_nShowItemComboY;

	CSetupConfig*			m_pJoySticTmpConfig;	

};

#endif // !defined(AFX_INFOPJOYSTICK_H__F4DB1741_5A7A_46D1_9779_1808E35488D6__INCLUDED_)

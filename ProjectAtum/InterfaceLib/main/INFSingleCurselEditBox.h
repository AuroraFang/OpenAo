// INFSingleCurselEditBox.h: interface for the CINFSingleCurselEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSINGLECURSELEDITBOX_H__A7F38416_E734_4EB2_8193_D7A285F0A0CA__INCLUDED_)
#define AFX_INFSINGLECURSELEDITBOX_H__A7F38416_E734_4EB2_8193_D7A285F0A0CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define		MAX_EDIT_SINGLE_LINE_BUFF		512

class CD3DHanFont;

class CINFSingleCurselEditBox  : public CINFBase
{
public:
	CINFSingleCurselEditBox();
	virtual ~CINFSingleCurselEditBox();

	HRESULT InitDeviceObjects(int nFontHeight, POINT ptEditPos, int nStrWidth, BOOL bUseCap, int nCap, BOOL bCullText=FALSE, int nMouseHeight=-1);		
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	
	//virtual void Render();
	void Render(int nScroll, int nOnePageLine);
	void EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus);
	//virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL);
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// ��ư Ŭ�� 
	virtual BOOL OnLButtonDown(POINT ptPos);	

	void Tick();

	BOOL IsEditMode();
	
	// ��Ʈ�� ����
	int  SetString(char* pTxt, int nBufferCnt, int nScrollStep, BOOL bCurselInit);
	void GetString(char* pTxt, int nBufferCnt);	// ��Ʈ�� ����.

	// ��Ʈ�� �ʱ��
	void InitString();	

	// ��ġ ���� 
	void SetPos(int nPosX, int nPosY);

	// 2010. 06. 07 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.) - �Է°���.
	// String ��� Width����� ����.
	inline void SetStringWidth ( const int a_iWidth ) { m_nStrWidth = a_iWidth; }
	inline int GetStringWidth ( void ) { return m_nStrWidth; }

	// String �Է� ���� Ȱ��/��Ȱ��ȭ.
	inline void SetEnable_InputLimit ( const bool a_bEnable ) { m_bEnable_InputLimit = a_bEnable; }
	inline bool GetEnable_InputLimit ( void ) { return m_bEnable_InputLimit; }

	// String �Է� ���� ���� ������ ����.
	inline void SetBuffLen_InputLimit ( const int a_nLimit_BuffLen ) { m_iLimit_InputBuffLen = a_nLimit_BuffLen; }
	inline int GetBuffLen_InputLimit ( void ) { return m_iLimit_InputBuffLen; }

	inline void SetInputNumMode ( const bool a_bInputNumMode ) { m_bInputNumMode = a_bInputNumMode; }
	inline bool GetInputNumMode ( void ) { return m_bInputNumMode; }

	// ��� ��ǥ ��������.
	inline const POINT *rtnPos ( void ) { return &m_ptEditPos; }
	// 2010. 06. 07 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.) - �Է°���.

	// End 2010. 06. 07 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.) - �Է°���.

	// 2007-11-05 by bhsohn ���� â�� �ִ� �۾� ���Ѱ� ����
	int GetImeStringLen();
	void SetStringMaxBuff(int nMaxBuffLen);

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	void BackupTxtString();
	BOOL ClickBackSpace();		
	BOOL IsLastPos();	
	
	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����	
	void SetFirstLine();

	// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�	
	BOOL ClickDelBtn();
	void SetChatMsgBuff(char* i_pImeChat);

	// 2010. 06. 03 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.)
	void InitSetChatMsgBuff ( char *i_pText );
	// End 2010. 06. 03 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.)

	int OnKeyDownCursel(WPARAM wParam, LPARAM lParam);
	int OnKeyDownArrow(WPARAM wParam, LPARAM lParam);
	void InitChatMsgBuff();
	void RenderCursel();
	POINT RefreshEndCurselPos();
	int OnKeyDownFunc(WPARAM wParam, LPARAM lParam);
	// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�

private:	
	int  UpdateCurselPos(int nScrollStep, int nMaxScrollStep);
	int OnArrowKeyDown(int nArrow, int nScrollStep);
	int GetStringPos(char* pTxtString, int nStringLen, int nLinePos);	

	int GetStringLen(char* pTxt);
	int GetCharLen(char* pTxt, int nNextChatLen);

	int  UpdateCurselX();	
	
protected:

	// 2010. 06. 07 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.) - �Է°���.
	bool	m_bEnable_InputLimit;
	bool	m_bInputNumMode;

	int		m_iLimit_InputBuffLen;
	// End 2010. 06. 07 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.) - �Է°���.

	BOOL	m_bEnableEdit;

	CD3DHanFont	*			m_pTxtFont;
	
	char*					m_pTxtString;	
	int						m_nBuffLen;

	vector<string>			m_vecStringCulling;

	// ����Ʈ �ڽ� ��ġ
	POINT					m_ptEditPos;
	// ��Ʈ�� ũ��
	int						m_nStrWidth;
	// �۾��� ����
	int						m_nStringHeightCap;
	
	// Ŀ�� �̹���
	CINFImageEx*			m_pImgCurSel;								// 2011. 10. 10 by jskim UI�ý��� ����
	POINT					m_ptCurSelPixel;

	BOOL					m_bShowCursel;
	float					m_fShowCurselTick;

	int						m_nMaxBuffLen;

	// �ø� �۾� ���� �Ǵ�.
	BOOL					m_bCullText;

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	char*					m_pBackupTxtString;
	char*					m_pTmpTxtString;	
	int						m_nMouseHeight;		// ���콺 �ν� ����

	POINT					m_ptCurSelPos;		// Ŀ���� ��ġ
	POINT					m_ptEndCurSelPos;	// Ŀ�� �������� ��ġ

	int						m_nStrOverFlowCntY;		// ��Ʈ���� �ʰ��ƴ��� ���� �Ǵ�.

	int						m_nOnePageItemCnt;

	char					m_strIMEFullTmp[SIZE_MAX_STRING_1024];
	char					m_strIMETmp[SIZE_MAX_STRING_1024];

	// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�	
	char						m_strInputMessage[MAX_EDIT_SINGLE_LINE_BUFF];	
	char				m_strPreBackupMessage[MAX_EDIT_SINGLE_LINE_BUFF];	// ��Ʈ�� �տ� ����
	char				m_strNextBackupMessage[MAX_EDIT_SINGLE_LINE_BUFF];	// ��Ʈ�� �ڿ� ����
	char				m_strBkInputMessage[MAX_EDIT_SINGLE_LINE_BUFF];		// ���� ����
	// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�

};

#endif // !defined(AFX_INFSINGLECURSELEDITBOX_H__A7F38416_E734_4EB2_8193_D7A285F0A0CA__INCLUDED_)

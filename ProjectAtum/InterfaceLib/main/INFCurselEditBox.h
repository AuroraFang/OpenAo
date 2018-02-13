// INFCurselEditBox.h: interface for the CINFCurselEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCURSELEDITBOX_H__717961A7_345B_4320_A19D_D058D7EF87D2__INCLUDED_)
#define AFX_INFCURSELEDITBOX_H__717961A7_345B_4320_A19D_D058D7EF87D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CD3DHanFont;

class CINFCurselEditBox  : public CINFBase
{
public:
	CINFCurselEditBox();
	virtual ~CINFCurselEditBox();

	HRESULT InitDeviceObjects(int nFontHeight, POINT ptEditPos, int nStrWidth, BOOL bUseCap, int nCap, BOOL bCullText=FALSE, int nMouseHeight=-1);		
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	
	//virtual void Render();
	void Render(int nScroll, int nOnePageLine);
	void EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus);
	//virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL);
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int nScrollStep, 
							int *pLienStep = NULL, int *o_pScrollStep = NULL, BOOL *o_pStrcat=NULL);

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

	// 2007-11-05 by bhsohn ���� â�� �ִ� �۾� ���Ѱ� ����
	int GetImeStringLen();
	void SetStringMaxBuff(int nMaxBuffLen);

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	void BackupTxtString();
	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	//BOOL ClickBackSpace();
	BOOL ClickBackSpace( int nScrollStep );
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����

	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	BOOL	OnDeleteKeyDown( int nScrollStep );
	void	OnHomeKeyDown( int nScrollStep );
	void	OnEndKeyDown( int nScrollStep );
	void	OnShiftKeyDown( void );
	void	OnShiftKeyUp( void );
	void	OnCtrlKeyDown( void );
	void	OnCtrlKeyUp( void );

	inline	BOOL	IsBlockMode( void )	{ return m_bBlockMode; }
	void	ResetBlockPosition( void );
	BOOL	IsValidBlockPostion( POINT pt );
	void	DetermineBlockFrontBack( POINT* ptBlockStart, POINT* ptBlockEnd );

	inline	BOOL	IsControlKeyDown( void ) { return m_bCtrlKeyState; }
	void	GetBlockedString( std::string* pStr );
	void	StringToClipBoard( const char* pStr );
	void	GetStringFromClipBoard( std::string* pStr );
	void	PasteString( const char* pStr, int nScrollStep );
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����

	void SetOnePageItemCnt(int nOnePageItemCnt);
	BOOL IsLastPos();
	POINT RefreshEndCurselPos();
	BOOL ClickEnter(int nScrollStep);

	// 2008-10-29 by bhsohn ����Ʈ �ڽ� ���� ����	
	void SetFirstLine();

private:	
	int  UpdateCurselPos(int nScrollStep, int nMaxScrollStep);
	int OnArrowKeyDown(int nArrow, int nScrollStep);
	int GetStringPos(char* pTxtString, int nStringLen, int nLinePos);
	int  SetStrcat(char* pTxt, int nBufferCnt, int nIMEBufLen, int nScrollStep);	// �ڿ� ��Ʈ�� ���̱�

	int GetStringLen(char* pTxt);
	int GetCharLen(char* pTxt, int nNextChatLen);

	int  UpdateCurselX();	
	
protected:
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
	CINFImageEx*			m_pImgCurSel;
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

	// 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
	BOOL					m_bBlockMode;
	POINT					m_ptBlockStartPos;
	POINT					m_ptBlockEndPos;

	BOOL					m_bCtrlKeyState;
	// end 2010. 04. 12 by ckPark �������� ���� ����Ʈ �ڽ� ��� ����
};

#endif // !defined(AFX_INFCURSELEDITBOX_H__717961A7_345B_4320_A19D_D058D7EF87D2__INCLUDED_)

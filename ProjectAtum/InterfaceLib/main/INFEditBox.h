// INFEditBox.h: interface for the CINFEditBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFEDITBOX_H__345C4E04_2C34_4062_9F20_D0086895C39F__INCLUDED_)
#define AFX_INFEDITBOX_H__345C4E04_2C34_4062_9F20_D0086895C39F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CD3DHanFont;
class CINFImageEx;

#define		MAX_SEARCH_ITEM			10					// 2013-02-20 by bhsohn �ΰ��� ����â ó��

class CINFEditBox  : public CINFBase
{
public:
	CINFEditBox();
	virtual ~CINFEditBox();

	virtual HRESULT InitDeviceObjects(int nFontHeight, POINT ptEditPos, int nStrWidth, BOOL bUseCap, int nCap, BOOL bCullText=FALSE, int nMouseHeight=-1);		// 2013-02-20 by bhsohn �ΰ��� ����â ó��
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();	
	
	virtual void Render();
	virtual void Render(int nScroll, int nOnePageLine); // 2013-02-20 by bhsohn �ΰ��� ����â ó��
	virtual void EnableEdit(BOOL bEnableEdit, BOOL bOnlyFocus); // 2013-02-20 by bhsohn �ΰ��� ����â ó��
	//virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL);
	// 2013-06-17 by bhsohn ���ս� ���� 2���� �������� ���� ó��
	virtual BOOL WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, int *pLienStep = NULL, BOOL bAddString=FALSE, BOOL bUseBackSpace=TRUE);
	// END 2013-06-17 by bhsohn ���ս� ���� 2���� �������� ���� ó��

	// ��ư Ŭ�� 
	virtual BOOL OnLButtonDown(POINT ptPos);	

	virtual void Tick(); // 2013-02-20 by bhsohn �ΰ��� ����â ó��

	BOOL IsEditMode();
	
	// ��Ʈ�� ����
	int  SetString(const char* pTxt, int nBufferCnt, BOOL bChRtn=FALSE);	// 2008-10-13 by bhsohn ���� �б� Return �ȵǴ� ���� ó��
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
	BOOL ClickBackSpace();	

protected:// 2013-02-20 by bhsohn �ΰ��� ����â ó��
	void StringCulling(char *pLineString, int nCullStringNum, vector<string> *vecStorage, CD3DHanFont* pHanFont);
	
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
	POINT					m_ptCurSel;

	BOOL					m_bShowCursel;
	float					m_fShowCurselTick;

	int						m_nMaxBuffLen;

	// �ø� �۾� ���� �Ǵ�.
	BOOL					m_bCullText;

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	char*					m_pBackupTxtString;
	BOOL					m_bBackAdd;	
	int						m_nMouseHeight;		// ���콺 �ν� ����
	
	// 2008-10-13 by bhsohn ���� �б� Return �ȵǴ� ���� ó��
	BOOL					m_bManageChRtn;	//	\\n ó��

};

#endif // !defined(AFX_INFEDITBOX_H__345C4E04_2C34_4062_9F20_D0086895C39F__INCLUDED_)

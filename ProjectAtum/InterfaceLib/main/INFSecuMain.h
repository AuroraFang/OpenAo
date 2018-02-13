// INFSecuMain.h: interface for the CINFSecuMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSECUMAIN_H__FF4458F7_13C6_4CDD_AC6A_13AD2BCAC680__INCLUDED_)
#define AFX_INFSECUMAIN_H__FF4458F7_13C6_4CDD_AC6A_13AD2BCAC680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFSecuLockWnd.h"
#include "INFSecuSetPassWnd.h"
#include "INFSecuChanPassWnd.h"
#include "INFSecuPassWnd.h"

class CINFImage;
class CD3DHanFont;

class CINFSecuMain  : public CINFBase
{
public:
	CINFSecuMain();
	virtual ~CINFSecuMain();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL IsShowSecurityWnd();
	void AllHideSecurityWnd();
	void ShowSecurityWnd(int nShowWnd, BOOL bShow, int nTmpParameter = 0);
	BOOL IsShowWnd(int nShowWnd);

	void OnClickSetupPass();
	void OnClickSetupChangePass();
	void OnClickLock(int nUnLockMode);

	// ��ȣ üũ 
	void SendCheckPassword(char* pPass, BYTE bLock);
	VOID FieldSecondLockOk(BOOL bIsUnlockFlag, BOOL bSuccess);

	// ��ȣ ó�� ����
	VOID SetSecurityPassword(char* pCurrentPass);

	// ��ȣ ���� 
	VOID ChangeSecurityPassword(char* pCurrentPass, char* pNewPass);
	void FieldSocketSecondPasswordUpdateOk(BOOL bSuccess, BOOL bFirstUseSecondPassword);

	// 2�� �н����� ���
	void SendSecndPassWordCancel(char* chPass);

	// 2�� �н����� ��� ����
	int GetPassChangeMode();
private:
	
private:
	CINFSecuLockWnd*		m_pINFSecuLockWnd;
	CINFSecuSetPassWnd*		m_pINFSecuSetPassWnd;
	CINFSecuChanPassWnd*	m_pINFSecuChangePassWnd;
	CINFSecuPassWnd*		m_pINFSecuPassWnd;

	int				m_nShowWnd;

	int					m_nPassChangeMode;
	// 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����
	int				m_nCnt;
	// end 2009. 08. 19 by jsKim ���� �� �޴� ������ ��� Ŀ���� ������ �ʴ� ����

};

#endif // !defined(AFX_INFSECUMAIN_H__FF4458F7_13C6_4CDD_AC6A_13AD2BCAC680__INCLUDED_)

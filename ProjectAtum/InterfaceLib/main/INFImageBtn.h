// INFImageBtn.h: interface for the CINFImageBtn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFIMAGEBTN_H__30BBF036_84B2_46BF_A6EF_8ADF9C968B2F__INCLUDED_)
#define AFX_INFIMAGEBTN_H__30BBF036_84B2_46BF_A6EF_8ADF9C968B2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define	BTN_STATUS_UP		0
#define	BTN_STATUS_DOWN		1
#define	BTN_STATUS_SEL		2
#define	BTN_STATUS_DISABLE	3
#define	MAX_BTN_STATUS		4

class CINFImageEx;		// 2011. 10. 10 by jskim UI�ý��� ����
class CINFImageBtn  : public CINFBase
{
public:
	CINFImageBtn();
	virtual ~CINFImageBtn();
// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	//HRESULT InitDeviceObjects(char* pBtup, char* pBtDown, char* pSel, char* pDisable);	
	//HRESULT InitDeviceObjects(char* i_szButtonName);
	HRESULT InitDeviceObjects(const char* pBtup, char* pBtDown, char* pSel, char* pDisable, char* pToolTipType = "STRTOOLTIP0" );	
	HRESULT InitDeviceObjects(const char* i_szButtonName, char* pToolTipType = "STRTOOLTIP0");
// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	HRESULT InitDeviceObjects_LoadOp(char* pBtup, char* pBtDown, char* pSel, char* pDisable);
	HRESULT InitDeviceObjects_LoadSelect(char* pBtup, char* pBtDown, char* pSel, char* pDisable);
	virtual HRESULT RestoreDeviceObjects();	
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Tick();			// 2011. 10. 10 by jskim UI�ý��� ����
	
	void Render();
	void SetBtnPosition(float i_fPosX, float i_fPosY);
	// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
	inline	POINT GetBtnPosition( void )
	{
		POINT pt;
		pt.x = m_fPosX;
		pt.y = m_fPosY;

		return pt;
	}
	// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
	POINT GetImgSize();		
	BOOL OnMouseMove(POINT ptPos);			// ���콺 �̵�
	BOOL OnLButtonDown(POINT ptPos);		// LButton�ٿ�
	BOOL OnLButtonUp(POINT ptPos);			// LButtonUp

	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	void SetBtnBlingBling(BOOL bSet, float fBlingTime);

	void EnableBtn(BOOL bEnable);
	void PushButton(BOOL i_bPush);

	// 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����
	//BOOL IsMouseOverlab(POINT ptPos);			// ��ư ���� ���콺�� �ֳ�?
	virtual BOOL IsMouseOverlab(POINT ptPos);			// ��ư ���� ���콺�� �ֳ�?
	// end 2009. 05. 07 by ckPark ��Ű�ӽ� ��ư ����

	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	void SetBtnState(int i_nBtnStatus);


	// 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.
	// 2008.05.03. by hsLee ��ư ���°� ����.
	inline int GetBtnState ( void ) { return m_nBtnStatus; }
	// End. 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.

	// 2009. 05. 7 by ckPark ��Ű �ӽ� ��ư ����
	inline CINFImage*	GetImg( const int nBtnState ){ return ((CINFImage*)m_pImageBtn[ nBtnState ]); }
	// end 2009. 05. 7 by ckPark ��Ű �ӽ� ��ư ����

	void SetAnimation( int nType, int nState, BOOL bLoop, float fPlayTime, float fScaleX, float fScaleY );
	void SetAniScale( float fScaleX, float fScaleY );
	void SetLineUp( int Line );

	// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
	void SetTooltipType(char *pToolTipType, char *pToolTipDisable);
	// END 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����

	// 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
	void SetLoopPlay(BOOL bLoopPlay) { m_bLoopPlay = bLoopPlay;}
	BOOL IsCheckMouseOverlab(POINT ptPos);
	// END 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��



private:
	CINFImageEx	*	m_pImageBtn[MAX_BTN_STATUS];
	int				m_nBtnStatus;

	int				m_nBtnBlingStatus; // 2013-07-11 by bhsohn �Ƹ� �÷��� ������

	float			m_fPosX;
	float			m_fPosY;
	
	float			m_fWidth;
	float			m_fHeight;

	// 2007-06-12 by bhsohn �̼� �������̽� ������
	BOOL			m_bShowBtn;

	// 2007-07-04 by bhsohn ���۷����� �߰�
	BOOL			m_bBlingBtn;
	float			m_fBlingTime;
	float			m_fCapBlingTime;

	// 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��
	BOOL			m_bLoopPlay;
	BOOL			m_bSelectBtn;
	// END 2013-06-25 by bhsohn �Ƹ� �÷��� ������ ó��

	BOOL			m_bClick;

	// 2008-05-13 by dgwoo �������� ��ư�ΰ�?
	BOOL			m_bPush;
	// 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����
	char            m_pToolTipType[512];
	// end 2011. 1. 12 by jskim UI �̹��� ��ư ���� ����

	// 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����
	char            m_pToolTipDisable[512];
	// END 2013-04-08 by bhsohn �ΰ��� ����â �߰� ����

	//2013-04-12 by ssjung �ɼ�â ���̽�ƽ ���� ���� ������� �ʴ� ���� ���� 
	BOOL m_bBtnShowTooltip; 
	//end 2013-04-12 by ssjung �ɼ�â ���̽�ƽ ���� ���� ������� �ʴ� ���� ���� 

};

#endif // !defined(AFX_INFIMAGEBTN_H__30BBF036_84B2_46BF_A6EF_8ADF9C968B2F__INCLUDED_)

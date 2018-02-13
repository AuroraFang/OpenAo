// INFWarDeclare.h: interface for the CINFWarDeclare class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFWARDECLARE_H__7E014AA6_A927_4E99_BDE5_E402520FF02F__INCLUDED_)
#define AFX_INFWARDECLARE_H__7E014AA6_A927_4E99_BDE5_E402520FF02F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImage.h"
#include "INFImageBtn.h"
#include "INFNumEditBox.h"
#include "INFWindow.h"
#include "D3DHanFont.h"

class CINFBase;
class CINFImage;
class CINFImageBtn;
class CINFNumEditBox;
class CD3DHanFont;
class CINFGroupImage;										 // 2011. 10. 10 by jskim UI�ý��� ����


// 2009. 01. 12 by ckPark ���� ���� �ý���
#include "CalendarBuilder.h"
// end 2009. 01. 12 by ckPark ���� ���� �ý���


class CINFWarDeclare : CINFBase
{
private:
	// 2009. 01. 12 by ckPark ���� ���� �ý���
	POINT				m_ptOldPoint;
	
	BOOL				m_bLButtonClick;

	BOOL				m_bWarDeclareAct;			// â �۵����ΰ�?
	
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*			m_pImgWarDeclareBG;			// ��׶���
	CINFGroupImage*			m_pImgWarDeclareControl;
#else
	CINFImageEx*			m_pImgWarDeclareBG;			// ��׶���
#endif
	CINFImageBtn*		m_pCalendarLeftBtn;			// �޷� ���� ��ư
	CINFImageBtn*		m_pCalendarRightBtn;		// �޷� ������ ��ư
	
	CINFImageEx*			m_pCalendarRect;			// �޷¿� �Ѹ� �簢��
	CINFImageEx*			m_pCalendarCheck;			// �޷¿� �������� vǥ��
	
	CINFImageBtn*		m_pWarDeclareNoonSelBtnUp;	// ���� / ���� ��ư
	CINFImageBtn*		m_pWarDeclareNoonSelBtnDown;// ���� / ���� ��ư
	
	CINFNumEditBox*		m_pWarDeclareHourEditBox;	// �ð� �Է� �ڽ�
	CINFNumEditBox*		m_pWarDeclareMinEditBox;	// �� �Է� �ڽ�
	
	CINFImageEx*			m_pANIWhiteFlag;			// �˸��� ���� ���
	CINFImageEx*			m_pBCUWhiteFlag;			// ������ ���� ���
	CINFImageEx*			m_pANICombatFlag;			// �˸��� ���� ���
	CINFImageEx*			m_pBCUCombatFlag;			// ������ ���� ���
	
	CINFImageBtn*		m_pGiveupWarBtn;			// ���� ��ư
	CINFImageBtn*		m_pSaveWarDeclareOptionBtn;	// �������� ����
	CINFImageBtn*		m_pCloseDeclareOption;		// �������� �ݱ�
	
	CD3DHanFont*		m_pFontWarDeclare;
	
	INT					m_nWarDeclarePosX;
	INT					m_nWarDeclarePosY;
	
	Calendar			m_Calendar;					// �ѷ��� �޷� ����
	RECT				m_CalendarMouseRect;		// �޷� ����
	
	YEAR				m_nCurCalYear;				// ���� �޷� �⵵
	MONTH				m_nCurCalMonth;				// ���� �޷� ��
	
	BOOL				m_bWarDeclareNoon;			// �����ΰ�?
	
	ATUM_DATE_TIME		m_WarDeclareDate;			// ����� �������� ����
	ATUM_DATE_TIME		m_WarDeclareDateView;		// ������ �������� ����

	BYTE				m_nForbidDayOfWeek;				// ���� ����
	ATUM_DATE_TIME		m_WarDeclareForbidStartPeriod;	// ���� ���� �ð�(��,��,���� �ǹ� ����)
	ATUM_DATE_TIME		m_WarDeclareForbidEndPeriod;	// ���� �� �ð�(��,��,���� �ǹ� ����)

	ATUM_DATE_TIME		m_WarDeclareEnableStartPeriod;	// ���� ���� ���� �ð�
	ATUM_DATE_TIME		m_WarDeclareEnableEndPeriod;	// ���� ���� �� �ð�
	
	BYTE				m_nWarDeclareLeftChance;		// ���� ���� ���� Ƚ��

	BYTE				m_nMSWarStep;					// ���� ���� �ֱ�
	
	// end 2009. 01. 12 by ckPark ���� ���� �ý���

	// 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����
	BOOL				m_bDisableWarDeclare;
	// end 2009. 02. 10 by ckPark ���� ���� �ý��� �߰� ����

public:
	CINFWarDeclare(CGameData * pData);
	virtual ~CINFWarDeclare();

public:
	// 2009. 01. 12 by ckPark ���� ���� �ý���
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	// ���� �Ⱓ ����
	void			SetWarDeclareForbidPeriod(const BYTE nForbidDayOfWeek, const ATUM_DATE_TIME* pStart, const ATUM_DATE_TIME* pEnd);

	// ���� ���� �Ⱓ ����
	void			SetWarDecalreEnablePeriod(const ATUM_DATE_TIME* pStart, const ATUM_DATE_TIME* pEnd);

	// �޷� ���� ���
	void			CalcCalendarRect(void);

	void			ShowWarDeclare(void);
	void			HideWarDeclare(void);
	// �޷� ������
	void			RenderCalendar(void);

	// �޷� ���� Ŭ��
	void			OnClickCalLeft(void);

	// �޷� ������ Ŭ��
	void			OnClickCalRight(void);

	// �޷¿� ���콺 ����
	void			OnMouseMoveCal(const POINT pt);

	// ���� ���� Ŭ����
	void			OnClickNoonSel(void);

	// �޷� Ŭ����
	void			OnClickCalendar(const POINT pt);

	// ���� ��ư Ŭ����
	void			OnClickGiveUpWar(void);

	// ���� ��ư Ŭ����
	void			OnClickSaveBtn(void);

	// ���ν���
	int				WndProcWarDeclare(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// �������� ���� ����
	void			SetWarDeclareDate(ATUM_DATE_TIME* pWarDeclareData);

	// �������� ���� Ƚ�� ����
	void			SetWarDeclareLeftChance(BYTE nWarDeclareLeftChance) { m_nWarDeclareLeftChance = nWarDeclareLeftChance; }

	// ���� �ֱ� Get Set
	BYTE			GetMSWarStep(void) const { return m_nMSWarStep; }
	void			SetMSWarStep(BYTE nMsgWarStep) { m_nMSWarStep = nMsgWarStep; }

	ATUM_DATE_TIME	GetWarDeclareDateView() const { return m_WarDeclareDateView; }
	// end 2009. 01. 12 by ckPark ���� ���� �ý���

	inline void		SetDisableWarDeclare(const BOOL bDisable) { m_bDisableWarDeclare = bDisable; }
};

#endif // !defined(AFX_INFWARDECLARE_H__7E014AA6_A927_4E99_BDE5_E402520FF02F__INCLUDED_)
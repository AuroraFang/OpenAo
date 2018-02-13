// INFSelectOption.h: interface for the CINFSelectOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSELECTOPTION_H__A8F8C38A_7A61_436B_A840_31BD1770723B__INCLUDED_)
#define AFX_INFSELECTOPTION_H__A8F8C38A_7A61_436B_A840_31BD1770723B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFSystem.h"


class CGameData;
class CINFImage;
class CINFImageBtn;
class CINFImageRadioBtn;
class CINFListBox;
class CD3DHanFont;
class CINFImageEx;												   // 2011. 10. 10 by jskim UI�ý��� ����

#define		OPTION_GRAPHIC_SEE_RANGE		0	// �þ߰Ÿ�
#define		OPTION_GRAPHIC_UNIT_DETAIL		1	// ���� ������
#define		OPTION_GRAPHIC_SHADOW			2	// �׸�������
#define		OPTION_GRAPHIC_EFFECT			3	// ����Ʈ ����
#define		OPTION_GRAPHIC_GAMMA			4	// ���� ����
#define		OPTION_GRAPHIC_CONTRAST			5	// ��� ����
#define		OPTION_GRAPHIC_FILTER			6	// ���� ȿ��
#define		OPTION_GRAPHIC_END				7	// ���� ȿ��


class CINFSelectOption : public CINFBase  
{
public:
	CINFSelectOption();
	virtual ~CINFSelectOption();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();

	virtual void Tick();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL			IsShow() {		return m_bShow;	}
	BOOL			SetShow(BOOL i_bShow);
	int				OnMouseMove(WPARAM wParam, LPARAM lParam);
	int				OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int				OnLButtonUp(WPARAM wParam, LPARAM lParam);

	void			InitGraphicSetting();
	void			UpdateBtn();
	void			SetOptionList();
	void			GetOptionList();
	int				GetGarphicOption_To_Cursel(int nMode, int nState);
	int				GetGarphicCursel_To_Option(int nMode, int nCursel);
	void			UpdateQuality(sOPTION_SYSTEM* pOption);
	BOOL			OptionQualityCombo();
	BOOL			OptionReset();						// �ɼ� �ʱ�ȭ.
	void			InitOptionGameInfo();
	BOOL			UpdateOption();						// �ɼ� ����.

	sOPTION_SYSTEM					m_stOption;
	
	BOOL							m_bShow;
	POINT							m_ptPos;
	CINFImageEx *					m_pBG;								// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageBtn *					m_pResetBtn;
	CINFImageBtn *					m_pOK;
	CINFImageBtn *					m_pClose;
	CINFListBox *					m_pGraphicLB[OPTION_GRAPHIC_END];
	char							m_szHaesangTxt[256];			// �ػ�.
	POINT							m_ptHaesangPos;
	CD3DHanFont*					m_pFontHaeSang;
	CINFImageRadioBtn*				m_pMiniFrame;	


};

#endif // !defined(AFX_INFSELECTOPTION_H__A8F8C38A_7A61_436B_A840_31BD1770723B__INCLUDED_)

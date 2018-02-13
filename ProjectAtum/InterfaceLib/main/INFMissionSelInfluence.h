// INFMissionSelInfluence.h: interface for the CINFMissionSelInfluence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMISSIONSELINFLUENCE_H__EDFFCB03_BB80_47C2_89D5_BF31C0A6D7F1__INCLUDED_)
#define AFX_INFMISSIONSELINFLUENCE_H__EDFFCB03_BB80_47C2_89D5_BF31C0A6D7F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFAlertMsgBox.h"


class CINFMissionMain;


class CINFImageEx;
class CINFGroupImage;
// ��Ʈ ����
#define DEC_FONT_TITLE		0
#define DEC_FONT_TXT		1
#define MAX_DEC_FONT		2

// ���� ����
#define INFLUENCE_BCU		0
#define INFLUENCE_ANI		1
#define MAX_INFLUENCE		2


class CINFMissionSelInfluence  
{
public:
	CINFMissionSelInfluence(CINFMissionMain* i_pParent);
	virtual ~CINFMissionSelInfluence();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InitInfluenceMission();
	
	void HideSelInfluence();

private:
	void RenderMissionInfo();
	void UpdateScrollPos();

	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);

	void OnSelectInfluence(int nInfluence);
	void OnSelectVCN();
	void OnSelectANI();
	// 2009-02-26 by bhsohn ���¼��ý�, ������ Ȯ��â �߰���
	//void ShowMsgBox(int nInflChoiceOver);
	void ShowMsgBox(int nInflChoiceOver, BOOL bOverInfluence);

	// �޽��� �ڽ� Ŭ��
	void OnAlerertMsgBox(BOOL bOk);
private:
	CINFMissionMain*		m_pParent;
	// ��� 
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*				m_pMissionImg;
#else
	CINFImageEx*				m_pMissionImg;
#endif
	// ���â
	CINFAlertMsgBox*		m_pINFAlertMsgBox;

	// ��� �̹��� ��ġ�� ũ��
	float			m_fBackPosX;
	float			m_fBackPosY;

	// ���� ���� �̼�	
	int						m_nInfluencePosX;
	int						m_nInfluencePosY;

	// �̼� ����
	vector<string>			m_vecVCUfluenceDesc;			// �̼� ����
	vector<string>			m_vecANIfluenceDesc;			// �̼� ����

	// ���� ���� ��������
	CD3DHanFont	*			m_pMissionFontfluenceL[MAX_DEC_FONT];
	CD3DHanFont	*			m_pMissionFontfluenceR[MAX_DEC_FONT];

	// ��ũ�� ��
	CINFArenaScrollBar*		m_pINFScrollBar[MAX_INFLUENCE];

	// ���� ����
	CINFImageBtn*			m_pSelInfluenceBtn[MAX_INFLUENCE];	

	// ������ ����
	int						m_nInflChoiceOver;

};

#endif // !defined(AFX_INFMISSIONSELINFLUENCE_H__EDFFCB03_BB80_47C2_89D5_BF31C0A6D7F1__INCLUDED_)

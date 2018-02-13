// INFOpUserHelper.h: interface for the CINFOpUserHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPUSERHELPER_H__709F5D23_BF85_4BB8_BFE4_7640AA1A294A__INCLUDED_)
#define AFX_INFOPUSERHELPER_H__709F5D23_BF85_4BB8_BFE4_7640AA1A294A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "INFImageBtn.h"
#include "INFOpSmallHelper.h"
#include "INFOpBigHelper.h"


// ����
#define HELPER_NPC_IMAGE_VCU		0		// ����������
#define HELPER_NPC_IMAGE_ANI		1		// �˸���
#define MAX_HELPER_NPC_IMAGE		2

// ���۷����� �̹��� ũ��
#define HELPER_NPCIMAGE_TUTO		0		// Ʃ�丮�� 
#define HELPER_NPCIMAGE_NOR			1		// ���� 
#define	MAX_HELPER_NPCIMAGE			2



class CINFOpMain;

class CINFOpUserHelper  
{
public:
	CINFOpUserHelper(CINFOpMain* pParent);
	virtual ~CINFOpUserHelper();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// �۾��� ����.
	void SetHelperString(char* pTxt, int nShowModeBtn);

	int OnMouseWheel(WPARAM wParam, LPARAM lParam);
	int OnLButtonDown(WPARAM wParam, LPARAM lParam);
	int OnMouseMove(WPARAM wParam, LPARAM lParam);
	int OnLButtonUp(WPARAM wParam, LPARAM lParam);
	int OnKeyDown(WPARAM wParam, LPARAM lParam);

	DataHeader * FindResource(char* szRcName);

	// ������ī �� ���� ����
	void OnClickFreeSkaHelperStart();
	void OnClickNextBtn();		// ���� ��ư 
	void OnClickGoMainBtn();	// ������ �̵� 
	void OnClickCloseBtn();	// ������ �̵� 
	

	void StartFirstUserExplain();
	void EndFirstUserExplain();
	void CloseFirstUserExplain();

	void EndCompleteOpHelper();

	// Ʃ�丮����� 
	void TutorialComplete();		// ����
	void TutorialFail();			// ���� 
	void TutorialHelper(char* pTxt);
	void TutorialAllComplete();		// ��� �Ϸ� 
	void TutorialNOTL10();

	// ������ �����Ͽ� ù���� ����Ʈ
	void SelInfluence(int nSelMode);

	void PlayFreeSkaNarrationSound(int nSelctExplain);

	BOOL OnClickEscape();

	int GetHelperMode();

private:
	// ������ī ����
	void RenderFreeSkaHelper(int nHelperSubMode);	

private:
	CINFOpMain*		m_pParent;
	CINFImageEx*		m_pNPCImage[MAX_HELPER_NPCIMAGE][MAX_HELPER_NPC_IMAGE];
	int				m_nSelectNpc;	
	int				m_nSelectMode;	
	
	// ���� �����ְ� �ִ� ���
	int						m_nHelperMode;
	int						m_nHelperSubMode;

	CINFOpSmallHelper*		m_pINFOpSmallHelper;
	CINFOpBigHelper*		m_pINFOpBigHelper;
	
	// ���� �� ���İ�
	float					m_fNPCViewTime;
	DWORD					m_dwNPCAlpha;

	BOOL					m_bStartAlphaNPC;
};

#endif // !defined(AFX_INFOPUSERHELPER_H__709F5D23_BF85_4BB8_BFE4_7640AA1A294A__INCLUDED_)

// INFAniCtrl.h: interface for the CINFAniCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFANICTRL_H__2F488604_7257_46D8_918F_154436496EDF__INCLUDED_)
#define AFX_INFANICTRL_H__2F488604_7257_46D8_918F_154436496EDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// ������ ���ϸ��̼�
#define	PANI_INFLUENCE_BCU		0
#define	PANI_INFLUENCE_ANI		1
#define	MAX_PANI_INFLUENCE		2

class CINFAniCtrl  : public CINFBase
{
public:
	CINFAniCtrl();
	virtual ~CINFAniCtrl();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	void SetWindowPos(float fPosX, float fPosY, float fWidth, float fHeight);
	void SetAnimation(BOOL bErrorQuest);
	void SetAniStepPosY(int nStep, int nMaxStep);

	void StartAnimation();
	void SetAniInfluence(BYTE byInfluence);
	//2011-06-13 by jhahn ����3�� ����Ʈ â ����
	void	RenderNoise();
	//end 2011-06-13 by jhahn ����3�� ����Ʈ â ����

private:
	CINFImageEx	*			m_pImageAni[MAX_PANI_INFLUENCE];
	CINFImageEx	*			m_pImageGrrr;				// ��� ��� ����
	CINFImageEx	*			m_pInfiImageGrrr;			//2011-06-23 by jhahn ����3�� ������ ȭ�� 
		
	int				m_nInfluencePos;

	int				m_nAniStep;
	int				m_nAniStepX;
	int				m_nAniStepY;

	int				m_nGrrrStep;
	int				m_nGrrrStepX;
	int				m_nGrrrStepY;

	float			m_fAniTime;
	float			m_fGrrrTime;		// ���� �ð� 

	float			m_fPosX;
	float			m_fPosY;

	float			m_fWidth;
	float			m_fHeight;

	float			m_fFrameWidth[MAX_PANI_INFLUENCE];
	float			m_fFrameHeight[MAX_PANI_INFLUENCE];

	float			m_fShowY[MAX_PANI_INFLUENCE];

	float			m_fGrrrWidth;
	float			m_fGrrrHeight;

	//2011-06-23 by jhahn ����3�� ������ ȭ�� 
	float			m_fInfyGrrrWidth; 
	float			m_fInfyGrrrHeight;		  
    //END 2011-06-23 by jhahn ����3�� ������ ȭ�� 

	BOOL			m_bStartAnimation;

};

#endif // !defined(AFX_INFANICTRL_H__2F488604_7257_46D8_918F_154436496EDF__INCLUDED_)

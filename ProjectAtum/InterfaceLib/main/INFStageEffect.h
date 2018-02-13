// INFStateEffect.h: interface for the CINFStageEffect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSTATEEFFECT_H__6EC66A09_2DC8_412C_9453_14D9EB971AE5__INCLUDED_)
#define AFX_INFSTATEEFFECT_H__6EC66A09_2DC8_412C_9453_14D9EB971AE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"
#define STAGE_IMG_NUM		4

///////////////////////////////////////////////////////////////////////////////
/// \class		CINFStageEffect
///
/// \brief		illust image�� FADEIN/FADEOUT, sound �÷���, SKIP ���
/// \author		dhkwon
/// \version	
/// \date		2004-06-03 ~ 2004-06-03
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CINFImage;
class DataHeader;
class CINFImageEx;									  // 2011. 10. 10 by jskim UI�ý��� ����
class CINFStageEffect : public CAtumNode  
{
public:
	CINFStageEffect(int nImageIndex, float fPlayTime, BOOL bSkipEnable=FALSE);
	virtual ~CINFStageEffect();

	BOOL IsRuning() { return (m_bRunning || !m_bEndWork); } // StageEffect�� FadeIn/FadeOut�Ϸ�� ���������۾��� �Ϸᰡ �Ǿ�� �Ѵ�.
	void SetEndWork( BOOL bSet ) { m_bEndWork = bSet; }	// ���� StageEffect�� ���������ϴ� �۾��� �Ϸ�Ǹ� �����Ѵ�.
	BOOL GetEndWork() { return m_bEndWork; }

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void Tick();
	void Render();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	BOOL					m_bRestored;
	BOOL					m_bRunning;
	int						m_nState;
	int						m_nIndex;
	float					m_fPlayTime;
	BOOL					m_bSkipEnable;
	float					m_fCurrentTime;
	BOOL					m_bEndWork;
//	CINFImage				*m_pImage[STAGE_IMG_NUM];
//	DataHeader				*m_pDataHeader[STAGE_IMG_NUM];

	// 2006-11-14 by ispark, �̹��� ����
	CINFImage				*m_pImage;					// 2011. 10. 10 by jskim UI�ý��� ����
	DataHeader				*m_pDataHeader;
};

#endif // !defined(AFX_INFSTATEEFFECT_H__6EC66A09_2DC8_412C_9453_14D9EB971AE5__INCLUDED_)

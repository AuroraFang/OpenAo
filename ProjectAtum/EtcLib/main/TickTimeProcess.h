// INFImageAnimation.h: interface for the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TICKTIME_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_TICKTIME_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// Ŭ���̾�Ʈ Ÿ�̸� ���
typedef struct
{
	MEX_TIMER_EVENT mexTimerEvent;
	float		fTimeflow;
	BOOL		bPause;
} ClientTimer_t;


class CTickTimeProcess
{

public:
	CTickTimeProcess();
	virtual ~CTickTimeProcess();	
	virtual void Tick(float fElapsedTime);

	void StartTimer(MSG_FC_TIMER_START_TIMER* pMsg);
	void StopTimer(MSG_FC_TIMER_STOP_TIMER* pMsg);
	void UpdateTimer(MSG_FC_TIMER_UPDATE_TIMER* pMsg);
	void PauseTimer(MSG_FC_TIMER_PAUSE_TIMER* pMsg);
	void ContinueTimer(MSG_FC_TIMER_CONTINUE_TIMER* pMsg);

public:
	// 2004-10-01 Ŭ���̾�Ʈ Ÿ�̸� ���
	vector<ClientTimer_t>	m_vecClientTimer;				// Ŭ���̾�Ʈ Ÿ�̸� ���
	float					m_fFlowTotalTime;
};

#endif

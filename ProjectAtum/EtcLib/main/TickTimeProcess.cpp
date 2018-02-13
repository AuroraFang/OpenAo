// CTickTimeProcess.cpp: implementation of the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TickTimeProcess.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTickTimeProcess::CTickTimeProcess()
{
	FLOG( "CTickTimeProcess()" );

	m_vecClientTimer.reserve(40);
	m_vecClientTimer.clear();

	m_fFlowTotalTime = 0.0f;
}

CTickTimeProcess::~CTickTimeProcess()
{
	FLOG( "~CTickTimeProcess()" );
	
	m_vecClientTimer.clear();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			TickTimeClientProcess(float fElapsedTime)
/// \brief		�������� �޼����� �޾� Ŭ���̾�Ʈ���� Tick Ÿ�Ӱ�� 
/// \author		ydkim
/// \date		2004-10-01~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTickTimeProcess::Tick(float fElapsedTime)
{
	FLOG( "CTickTimeProcess::Tick()" );

	// ����
	m_fFlowTotalTime = m_fFlowTotalTime + fElapsedTime;

	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->bPause == FALSE)
		{
			//it->fTimeflow = m_fFlowTotalTime*1000;
			if(it->fTimeflow < m_fFlowTotalTime)
			{
				it->fTimeflow += ((float)it->mexTimerEvent.TimeInterval)/1000;
				// �ð� ������ �޼��� ������				
				MSG_FC_TIMER_TIMEOUT sMsg;				
//				DBGOUT("ƽ Ÿ�� �׽�Ʈ Interval(%d) Size(%d)\n", (int *)it->mexTimerEvent.TimeInterval, m_vecClientTimer.size());
				if(it->mexTimerEvent.bRepeatable)
				{
//					DBGOUT("ƽ Ÿ�� �׽�Ʈ �����ͺ� Interval(%d) bRepeatable(%d) nRemainedRepeatCount(%d)\n",
//						(int *)it->mexTimerEvent.TimeInterval, it->mexTimerEvent.bRepeatable, it->mexTimerEvent.nRemainedRepeatCount);
					it->mexTimerEvent.nRemainedRepeatCount--;
					
					sMsg.TimerEvent = it->mexTimerEvent;				
					g_pFieldWinSocket->SendMsg( T_FC_TIMER_TIMEOUT, (char*)&sMsg, sizeof(sMsg) );

					if(it->mexTimerEvent.nRemainedRepeatCount <= 0)
					{
						it = m_vecClientTimer.erase(it);
						continue;
					}					
				}
				else
				{
					sMsg.TimerEvent = it->mexTimerEvent;				
					g_pFieldWinSocket->SendMsg( T_FC_TIMER_TIMEOUT, (char*)&sMsg, sizeof(sMsg) );
//					DBGOUT("������ UID %d\n",sMsg.TimerEvent.TimerUID);					
//					DBGOUT("ƽ Ÿ�� �׽�Ʈ �̷����� Interval(%d) Size(%d)\n",
//						(int *)it->mexTimerEvent.TimeInterval,m_vecClientTimer.size());
					it = m_vecClientTimer.erase(it);
					continue;
				}				

				
			}
		}

		it++;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			TickTimeClientProcess(float fElapsedTime)
/// \brief		�������� �޼����� �޾� Ŭ���̾�Ʈ���� Tick Ÿ�Ӱ�� 
/// \author		ydkim
/// \date		2004-10-01~
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CTickTimeProcess::StartTimer(MSG_FC_TIMER_START_TIMER* pMsg)
{
	ClientTimer_t timeTemp;
	timeTemp.mexTimerEvent = pMsg->TimerEvent;
	timeTemp.bPause = FALSE;
	timeTemp.fTimeflow = m_fFlowTotalTime + ((float)pMsg->TimerEvent.TimeInterval)/1000;

	m_vecClientTimer.push_back(timeTemp);	
}

void CTickTimeProcess::StopTimer(MSG_FC_TIMER_STOP_TIMER* pMsg)
{
	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->mexTimerEvent.TimerUID == pMsg->TimerUID)
		{
			m_vecClientTimer.erase(it);

			break;
		}
		it++;
	}
}

void CTickTimeProcess::UpdateTimer(MSG_FC_TIMER_UPDATE_TIMER* pMsg)
{
	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->mexTimerEvent.TimerUID == pMsg->TimerEvent.TimerUID)
		{
			ClientTimer_t timeTemp;
			timeTemp.mexTimerEvent = pMsg->TimerEvent;
			timeTemp.bPause = FALSE;
			timeTemp.fTimeflow = pMsg->TimerEvent.TimeInterval;

			m_vecClientTimer.erase(it);
			m_vecClientTimer.push_back(timeTemp);

			break;
		}
		it++;
	}
}

void CTickTimeProcess::PauseTimer(MSG_FC_TIMER_PAUSE_TIMER* pMsg)
{
	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->mexTimerEvent.TimerUID == pMsg->TimerUID)
		{
			it->bPause = TRUE;
		}
		it++;
	}
}

void CTickTimeProcess::ContinueTimer(MSG_FC_TIMER_CONTINUE_TIMER* pMsg)
{
	vector<ClientTimer_t>::iterator it = m_vecClientTimer.begin();
	while(it != m_vecClientTimer.end())
	{
		if(it->mexTimerEvent.TimerUID == pMsg->TimerUID)
		{
			it->bPause = FALSE;
		}
		it++;
	}
}
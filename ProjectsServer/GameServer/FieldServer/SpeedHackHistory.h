// SpeedHackHistory.h: interface for the CSpeedHackHistory class.
// 2013-01-29 by hskim, ���ǵ��� �ڵ� �� ��� ����
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPEEDHACKHISTORY_H__D4BEE252_29F2_4FD5_B84C_9DDB40C0D97B__INCLUDED_)
#define AFX_SPEEDHACKHISTORY_H__D4BEE252_29F2_4FD5_B84C_9DDB40C0D97B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SPEED_HACK_HISTORY_THRESHOLD		50
#define SPEED_HACK_RESET_PERIOD_SEC			1800		// 30 ��

class CSpeedHackHistory  
{
public:
	CSpeedHackHistory();
	virtual ~CSpeedHackHistory();

	void Clear();
	void SetBlockThreshold(int nThreshold);		// 30�� ���ش� �� �� �̻��� ��� �� ó��

	void InsertResult(BOOL bResult);
	BOOL IsBlock();

private:
	int m_nBlockThreshold;
	int m_nDetectedCount;
	ATUM_DATE_TIME m_atimeResetRecord;
};

#endif // !defined(AFX_SPEEDHACKHISTORY_H__D4BEE252_29F2_4FD5_B84C_9DDB40C0D97B__INCLUDED_)


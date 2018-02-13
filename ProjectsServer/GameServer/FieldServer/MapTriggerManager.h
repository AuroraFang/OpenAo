// MapTriggerManager.h: interface for the CMapTriggerManager class.
//
// \brief		// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
// \author		hskim
// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPTRIGGERMANAGER_H__AF36F11A_0D7F_41FE_9EC7_272A4D48B19B__INCLUDED_)
#define AFX_MAPTRIGGERMANAGER_H__AF36F11A_0D7F_41FE_9EC7_272A4D48B19B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TickManager.h"

class CFieldIOCP;
class CODBCStatement;
class CTriggerFunction;
class CFieldMapChannel;

class CMapTriggerManager : public CTickManager
{
public:
	CMapTriggerManager(DWORD i_nTickInterval = 1000);
	virtual ~CMapTriggerManager();

	void Clear();
	BOOL Create(CFieldIOCP *pFieldIOCP, CODBCStatement *pOdbcStmt);
	BOOL LoadDB(CODBCStatement *pOdbcStmt);
	void Destroy();

	BOOL InsertTrigger(CTriggerFunction *pTriggerFunction);

	int LinkToTriggerFunction(MAP_CHANNEL_INDEX MapChannel, CFieldMapChannel *pFieldMapChannel, vector<CTriggerFunction *> *pvectorTriggerFunction);
	CFieldIOCP *GetFieldIOCP()		{ return m_pFieldIOCP; }

	BOOL ResetEventByMapChannel(MAP_CHANNEL_INDEX MapChannelIndex);		// 2012-12-17 by hskim, NPC ���� ����۽� - Ʈ���� �ý��� �ʱ�ȭ ����
	
	BOOL SkipEventByTriggerID(INT iMapTriggerID);
	BOOL SkipEventByMapChannelIndex(MAP_CHANNEL_INDEX MapChannelIndex);
	BOOL FixedBuffItemChannelIndex(MAP_CHANNEL_INDEX MapChannelIndex, INT i_nFixedBuffItem);	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���	- �׽�Ʈ�� ũ����Ż ���� ����

protected:
	BOOL CreateSubClass();
	BOOL MixCrystal();		// ũ����Ż ���� ����

protected:

	void DoTickEvent(ATUM_DATE_TIME *pDateTime, TICK_EVENT *pTickEvent);
	void DoEveryTickWork(ATUM_DATE_TIME *pDateTime);
	void DoDailyWork(ATUM_DATE_TIME *pDateTime);
	void DoHourlyWork(ATUM_DATE_TIME *pDateTime);
	void DoMinutelyWork(ATUM_DATE_TIME *pDateTime);
	void DoSecondlyWork(ATUM_DATE_TIME *pDateTime);
	void DoMonthlyWork(ATUM_DATE_TIME *pDateTime);

protected:
	CFieldIOCP *m_pFieldIOCP;

	vector<CTriggerFunction *>			m_vectorTriggerFunction;		// �ʱ� ���ý� �����ǰ� ������� �ʴ´�. ����ȭ �ʿ� ����

private:
	vectorTriggerMap					m_vectTriggerMap;				
	vectorTriggerFunctionCrystal		m_vectTriggerFunctionCrystal;
	vectorTriggerCrystalGroup			m_vectTriggerCrystalGroup;
	vectorTriggerCrystalDestroyGroup	m_vectTriggerCrystalDestroyGroup;

	vectorTriggerFunctionNGCInflWar		m_vectTriggerFunctionNGCInflWar;
	vectorTriggerNGCInflWarMonsterGroup	m_vectTriggerNGCInflWarMonsterGroup;

	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���
	vectorTriggerFunctionNGCOutPost		m_vectTriggerFunctionNGCOutPost;	// ���� ���� ���� ����ü ����
	vectorTriggerOutPostBossKillInfo	m_vectTriggerOutPostBossKillInfo;	// ���� ���� �������� ����ü ����
	vectorTriggerCrystalBuff			m_vectTriggerCrystalBuff;			// Ʈ���� �� ũ����Ż ���� ����
	// end 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���
};

#endif // !defined(AFX_MAPTRIGGERMANAGER_H__AF36F11A_0D7F_41FE_9EC7_272A4D48B19B__INCLUDED_)

// HappyHourEventManager.h: interface for the CHappyHourEventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HAPPYHOUREVENTMANAGER_H__E66E6D10_70DE_4DE2_8345_8D7497383BBE__INCLUDED_)
#define AFX_HAPPYHOUREVENTMANAGER_H__E66E6D10_70DE_4DE2_8345_8D7497383BBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef mt_vector<SHAPPY_HOUR_EVENT>		mtvectHAPPY_HOUR_EVENT;		// 2006-08-22 by cmkwon, vector<SHAPPY_HOUR_EVENT> --> mt_vector<SHAPPY_HOUR_EVENT>

class CGameEventManager;
class CFieldIOCP;
class CFieldIOCPSocket;		// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� -
class CHappyHourEventManager  
{
public:
	CHappyHourEventManager(CFieldIOCP *i_pFIOCP, CGameEventManager *i_pGameEventManager);
	~CHappyHourEventManager();

	BOOL InitHappyHourEventManager(mtvectHAPPY_HOUR_EVENT *i_pvectHappyEventList);
	void ResetHappyHourEventManager(BOOL i_bNotify=FALSE);	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - ���� �߰�(BOOL i_bNotify=FALSE)
	mtvectHAPPY_HOUR_EVENT *GetVectHappyHourEventListPtr(void);

// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �Լ� �ּ� ó����
//	void SetHappyHourEventTime(ATUM_DATE_TIME i_atimeStart, ATUM_DATE_TIME i_atimeEnd, BOOL i_bStoreDB=FALSE);
//	SHAPPY_HOUR_EVENT *GetCurrentHappyHourEvent(void);
//	SHAPPY_HOUR_EVENT *FindHappyHourEventByTime(ATUM_DATE_TIME *pCurDateTime, int i_CurDayOfWeek);
//	BOOL GetPCBangHappyHourEventInfo(MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK *o_pHappyInfoOK);

	BOOL OnDoMinutelyWorkHappyHourEventManager(ATUM_DATE_TIME *pDateTime);


	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - 
	void OnDoMinutelyWorkHappyHourEventManagerOfPCBang(ATUM_DATE_TIME *pDateTime);
	void OnDoMinutelyWorkHappyHourEventManagerAllInfluenceType(ATUM_DATE_TIME *pDateTime);
	void OnDoMinutelyWorkHappyHourEventManagerOfInfluenceType(ATUM_DATE_TIME *pDateTime, SHAPPY_HOUR_EVENT_PERIOD *i_pHappyEvPeriod);
	SHAPPY_HOUR_EVENT *GetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy);
	BOOL SetCurrentHappyHourEventByInfluenceType(BYTE i_byInflTy, SHAPPY_HOUR_EVENT *i_pHappyEv);
	SHAPPY_HOUR_EVENT *FindHappyHourEventByInfluenceType8Time(BYTE i_byInflTy, ATUM_DATE_TIME *pCurDateTime, int i_CurDayOfWeek);
	BOOL SendHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MessageType_t i_msgTy);		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - SendHappyHourEventInfo() ���� �߰�
	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - SendHappyHourEventInfo() �� msgTy ���ڸ� �߰��Ͽ� ó�� ��. ���Լ��� �ʿ� ����
	//BOOL SendHappyHourEventInfoByLevel(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy);		// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
	BOOL SetHappyHourEventPeriod(BYTE i_byInflTy, ATUM_DATE_TIME i_atimeStart, ATUM_DATE_TIME i_atimeEnd, BOOL i_bStoreDB=FALSE);

	//////////////////////////////////////////////////////////////////////
	// 2012-10-05 by jhseol, NPlay PC�� ��� - ���Ǿƿ� ���� ��������
	BOOL LoadHappyHourEventInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy, MSG_FC_INFO_GET_HAPPY_HOUR_EVENT_INFO_OK* i_msg);

protected:
	mtvectHAPPY_HOUR_EVENT	m_mtvectHappyHourEventList;
// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �Ʒ��� ������ ������
//	INT						m_nAllEventUniqueNumber;
//	ATUM_DATE_TIME			m_atimeStartTime;
//	ATUM_DATE_TIME			m_atimeEndTime;
//	SHAPPY_HOUR_EVENT		*m_pCurrentHappyHourEvent;
	SHAPPY_HOUR_EVENT		m_PCBangHappyHourEvent;				// 2006-08-23 by cmkwon, PC�� ���Ǿƿ� �̺�Ʈ
	BYTE					m_byPCBangHappyEvState;				// 2006-08-23 by cmkwon
	SHAPPY_HOUR_EVENT_PERIOD 	m_arrPeriodEachInfluenceType[INFLUENCE_TYPE_COUNT];				// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �߰��� ��� ����
	SHAPPY_HOUR_EVENT *			m_pArrCurHappyHourEventEachInfluenceType[INFLUENCE_TYPE_COUNT];	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �߰��� ��� ����

	CFieldIOCP				*m_pFieldIOCP13;
	CGameEventManager		*m_pGameEventManager;
};

#endif // !defined(AFX_HAPPYHOUREVENTMANAGER_H__E66E6D10_70DE_4DE2_8345_8D7497383BBE__INCLUDED_)

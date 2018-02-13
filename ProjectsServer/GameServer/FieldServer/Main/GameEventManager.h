// GameEventManager.h: interface for the CGameEventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEEVENTMANAGER_H__90AE5331_2500_4855_A8B8_E9FCF5D4D444__INCLUDED_)
#define AFX_GAMEEVENTMANAGER_H__90AE5331_2500_4855_A8B8_E9FCF5D4D444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum EN_GAME_EVENT_TYPE
{
	GAME_EVENT_TYPE_EXP				= 0,
	GAME_EVENT_TYPE_SPI				= 1,
	GAME_EVENT_TYPE_EXP_NO_DOWN		= 2,
	GAME_EVENT_TYPE_DROP_ITEM		= 3,
	GAME_EVENT_TYPE_DROP_REAR_ITEM	= 4,
	GAME_EVENT_TYPE_WARPOINT		= 5,				// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
	GAME_EVENT_TYPE_MAX_COUNT		= 6
};

///////////////////////////////////////////////////////////////////////////////
// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
enum EN_GAME_EVENT_GROUP
{
	GAME_EVENT_GROUP_HAPPYHOUR		= 0,
	GAME_EVENT_GROUP_MOTHERSHIP		= 1,
	GAME_EVENT_GROUP_ITEM			= 2,
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM		= 3,	// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	GAME_EVENT_GROUP_CRYSTAL_SYSTEM_2ND	= 4,	// 2012-05-08 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý��� 2��
	GAME_EVENT_GROUP_MAX_COUNT			= 5		
};

struct SGameEvent
{
	BOOL			bUsingFlag;
	BOOL			bSendEventEndMessage;
	ATUM_DATE_TIME	atimeStartedTime;				
	ATUM_DATE_TIME	atimeEndTime;					
	DWORD			dwTimeLimitMinute;
	float			fApplyRate;
	BYTE			byInfluenceMask;				// 2006-04-12 by cmkwon, �̺�Ʈ�� ����� ����(INFLUENCE_TYPE_XXX)
	INT				MinLevel;						// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
	INT				MaxLevel;						// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -
	MapIndex_t		MapIndex;						// 2012-06-16 by hskim, [Ʈ���� �ý���] - ũ����Ż �ý��� 2�� : Ư�� �ʿ����� ���Ǿƿ� �̺�Ʈ ���� - 0 �̸� ���� ���� ����
};

typedef SGameEvent (*SGAEM_EVENT_PPTR)[GAME_EVENT_TYPE_MAX_COUNT];

class CFieldIOCP;
class CFieldIOCPSocket;
class CGameEventManager  
{
public:
	CGameEventManager();
	~CGameEventManager();

	void InitEventManager(CFieldIOCP *i_pFIOCP);
// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - ��� �ʿ� ������ �Լ� ��
//	BOOL IsDoingAllGameEvent(BYTE i_byInflTy);
	BOOL IsProgressGameEventGroup(BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR, SGameEvent **o_ppUsingGameEv=NULL);	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 

	void OnDoMinutelyWorkGameEventTime(ATUM_DATE_TIME *pDateTime);
	BOOL StartGameEvent(EN_GAME_EVENT_TYPE i_eventType, float i_fRate, int i_nLimitMinute, int i_nMinLevel, int i_nMaxLevel, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR, MapIndex_t i_Mapindex = 0);	// 2012-06-16 by hskim, [Ʈ���� �ý���] - ũ����Ż �ý��� 2�� : Ư�� �ʿ����� ���Ǿƿ� �̺�Ʈ ���� - // 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - ���� �߰�. // 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - MinLevel, MaxLevel
	void EndAllGameEvent(BOOL i_bEndNotify=FALSE, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	BOOL EndGameEvent(EN_GAME_EVENT_TYPE i_eventType, BOOL i_bSendEndMsg, BYTE i_byInflMask=INFLUENCE_TYPE_ALL_MASK, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	BOOL GetGameEventInfoString(char *o_szInfo, BYTE *o_pbyInflMask, EN_GAME_EVENT_TYPE i_eventType, BOOL i_bAdminFlag=FALSE, BYTE i_byInflType=INFLUENCE_TYPE_ALL_MASK);
	void SenGameEventGroupMotherShip8ItemInfo(CFieldIOCPSocket *i_pToFISock, BYTE i_byInflTy);	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 

	///////////////////////////////////////////////////////////////////////////////
	// 2006-08-23 by cmkwon
	SGameEvent *GetPCBangGameEventPtr(EN_GAME_EVENT_TYPE i_eventType);
	SGameEvent *GetPCBangUsingGameEventPtr(EN_GAME_EVENT_TYPE i_eventType);
	BOOL StartPCBangGameEvent(EN_GAME_EVENT_TYPE i_eventType, float i_fRate, int i_nLimitMinute);
	BOOL EndPCBangGameEvent(void);

// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - �ý��� �������� �Լ���� �Լ� ������
//	float GetExperienceRate(BYTE i_byInflType, BOOL i_bPCBangClient);			// ��ġ �̺�Ʈ
//	float GetSPIRate(BYTE i_byInflType, BOOL i_bPCBangClient);					// ���� �̺�Ʈ
//	float GetExpRepairRate(BYTE i_byInflType, BOOL i_bPCBangClient);			// ��ġ���� �̺�Ʈ
//	float GetDropItemRate(BYTE i_byInflType, BOOL i_bPCBangClient);
//	float GetDropRareItemRate(BYTE i_byInflType, BOOL i_bPCBangClient);
//	float GetWarPointRate(BYTE i_byInflType, BOOL i_bPCBangClient);				// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-30 by cmkwon, ���º� ���Ǿƿ� �̺�Ʈ ���� - 
	SGameEvent *GetArrGameEventByInflType(BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 

// 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� - �Ʒ��� CGameEventManager::GetTotalEventPlusRate() �Լ��� ��ü��
//	float GetTotalEventPlusRate(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, BOOL i_bPCBangClient);
	float GetTotalEventPlusRate(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);		// 2012-06-16 by hskim, [Ʈ���� �ý���] - ũ����Ż �ý��� 2�� : Ư�� �ʿ����� ���Ǿƿ� �̺�Ʈ ���� - // 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -	

	float GetExperiencePlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);	// 2012-06-16 by hskim, [Ʈ���� �ý���] - ũ����Ż �ý��� 2�� : Ư�� �ʿ����� ���Ǿƿ� �̺�Ʈ ���� - // 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -	// 2007-10-30 by cmkwon, ��ġ �̺�Ʈ
	float GetSPIPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);			// 2012-06-16 by hskim, [Ʈ���� �ý���] - ũ����Ż �ý��� 2�� : Ư�� �ʿ����� ���Ǿƿ� �̺�Ʈ ���� - // 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -		// 2007-10-30 by cmkwon, ���� �̺�Ʈ
	float GetExpRepairPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);	// 2012-06-16 by hskim, [Ʈ���� �ý���] - ũ����Ż �ý��� 2�� : Ư�� �ʿ����� ���Ǿƿ� �̺�Ʈ ���� - // 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -	// 2007-10-30 by cmkwon, ��ġ���� �̺�Ʈ
	float GetDropItemPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);	// 2012-06-16 by hskim, [Ʈ���� �ý���] - ũ����Ż �ý��� 2�� : Ư�� �ʿ����� ���Ǿƿ� �̺�Ʈ ���� - // 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -		// 2007-10-30 by cmkwon, ���������
	float GetDropRareItemPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);	// 2012-06-16 by hskim, [Ʈ���� �ý���] - ũ����Ż �ý��� 2�� : Ư�� �ʿ����� ���Ǿƿ� �̺�Ʈ ���� - // 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -		// 2007-10-30 by cmkwon, ��������ۿ� ������� Ȯ��
	float GetWarPointPlusRate(BYTE i_byInflType, BOOL i_bPCBangClient, INT i_nLevel, MapIndex_t i_MapIndex = 0);		// 2012-06-16 by hskim, [Ʈ���� �ý���] - ũ����Ż �ý��� 2�� : Ư�� �ʿ����� ���Ǿƿ� �̺�Ʈ ���� - // 2008-03-14 by dhjin, Level�� ���Ǿƿ� �̺�Ʈ ���� -		// 2007-10-30 by cmkwon, ������Ʈ �̺�Ʈ ���� �߰�
	BOOL EndGameEventAllByInfluenceType(BYTE i_byInflMask, BOOL i_bSendEndMsg=FALSE, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	
protected:	
	SGameEvent *GetGameEventPtr(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);		// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	SGameEvent *GetUsingGameEventPtr(EN_GAME_EVENT_TYPE i_eventType, BYTE i_byInflType, EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 
	SGAEM_EVENT_PPTR GetGameEventGroupArray(EN_GAME_EVENT_GROUP i_enGameEvGroup=GAME_EVENT_GROUP_HAPPYHOUR);					// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - 	

protected:
	SGameEvent			m_arrGameEvent[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];	// 2007-10-30 by cmkwon, 0=Normal, 1=VCN, 2=ANI, 3=ALL
	SGameEvent			m_PCBangGameEvent[GAME_EVENT_TYPE_MAX_COUNT];						// 2006-08-23 by cmkwon, PCBang �̺�Ʈ ����
	SGameEvent			m_arrGameEventofMotherShip[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];	// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - ���� ������ ��� ���� �߰� 2007-10-30 by cmkwon, 0=Normal, 1=VCN, 2=ANI, 3=ALL
	SGameEvent			m_arrGameEventofItem[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];			// 2008-05-20 by cmkwon, ��� �̺�Ʈ(HappyHoure,MotherShip,Item) �׷� ���ÿ� �����ϰ� - ���� ������ ��� ���� �߰� 2007-10-30 by cmkwon, 0=Normal, 1=VCN, 2=ANI, 3=ALL
	SGameEvent			m_arrGameEventofCrystal[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];			// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	SGameEvent			m_arrGameEventofCrystal2ND[INFLUENCE_TYPE_COUNT][GAME_EVENT_TYPE_MAX_COUNT];	// 2012-05-08 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý��� 2��
	
public:
	CFieldIOCP			*m_pFieldIOCP10;
	
	static char *GetGameEventTypeString(int i_nEvType);
	static float		ms_fDefaultExpRate;
	static float		ms_fDefaultSPIRate;
	static float		ms_fDefaultExpRepareRate;
	static float		ms_fDefaultDropItemRate;
	static float		ms_fDefaultDropRareItemRate;						
	static float		ms_fDefaultWarPointRate;							// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�			
};



#endif // !defined(AFX_GAMEEVENTMANAGER_H__90AE5331_2500_4855_A8B8_E9FCF5D4D444__INCLUDED_)

// GiveItemManager.h: interface for the CGiveItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIVEITEMMANAGER_H__587F770B_FAA0_45E0_9EBB_556823A0EC7F__INCLUDED_)
#define AFX_GIVEITEMMANAGER_H__587F770B_FAA0_45E0_9EBB_556823A0EC7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef	mt_vector<SITEM_EVENT_INFO>	mtvectItemEventInfo;

class CFieldIOCPSocket;
class CGiveItemManager  
{
public:
	BOOL InitItemEventManager(vectItemEventInfo *i_pItemEventList);
	mtvectItemEventInfo * GetVectItemEventListPtr(void);
	CGiveItemManager();
	virtual ~CGiveItemManager();

// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
// 	void GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bPCBangCheck, BYTE i_byInfluenecType, BOOL i_bMemberShip, ATUM_DATE_TIME *i_Time, INT i_nUnitkind, ATUM_DATE_TIME *i_patLastGameEndDate);          // 2007-07-23 by dhjin, ���� ���� �ð��� �������� �̺�Ʈ ������ �Լ�(�̺�Ʈ ��Ⱦȿ� ������ ���� ��� üũ �߰�)
// 	void GetActEventItemInflChange(vectItemEventInfo *o_ItemEventInfo, ATUM_DATE_TIME *i_Time, INT i_nUnitkind);				// 2007-07-20 by dhjin, ITEM_EVENT_TYPE_NEWMEMBER_INFLCHANGE �̺�Ʈ ���� üũ
// 	void GetActEventItemLevelUp(vectItemEventInfo *o_ItemEventInfo, ATUM_DATE_TIME *i_Time, BYTE i_nLevel, INT i_nUnitkind);				// 2007-07-20 by dhjin, ITEM_EVENT_TYPE_NEWMEMBER_LEVELUP �̺�Ʈ ���� üũ
	//void GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, ATUM_DATE_TIME *i_patLastGameEndDate);          // 2007-07-23 by dhjin, ���� ���� �ð��� �������� �̺�Ʈ ������ �Լ�(�̺�Ʈ ��Ⱦȿ� ������ ���� ��� üũ �߰�)
	// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - ���� �߰� 
	void GetActEventItemGameStart(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, ATUM_DATE_TIME *i_patLastGameEndDate, BYTE b_Level, UID32_t i_AccountUniqueNumber, INT i_LoginCheckNum);  // 2010-06-16 by jskim, ������ �̺�Ʈ üũ ��� -
	void GetActEventItemInflChange(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate);				// 2007-07-20 by dhjin, ITEM_EVENT_TYPE_NEWMEMBER_INFLCHANGE �̺�Ʈ ���� üũ
	void GetActEventItemLevelUp(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, BYTE i_nLevel);				// 2007-07-20 by dhjin, ITEM_EVENT_TYPE_NEWMEMBER_LEVELUP �̺�Ʈ ���� üũ
	void GetActEventItemFirstPurchase(vectItemEventInfo *o_ItemEventInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME i_atAccountLastBuyDate, ATUM_DATE_TIME i_atCharacterLastBuyDate);			// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������

	// 2008-02-01 by cmkwon, ItemEvent �� LastGameEndDate üũ ��ƾ �߰� - 
	// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - üũ�ѹ� �����߰� 
	BOOL IsEnableItemEvent(SITEM_EVENT_INFO *i_pItemEvInfo, BOOL i_bMemberShip, BOOL i_bPCBangUser, BYTE i_byInfluenecType, INT i_nUnitkind, ATUM_DATE_TIME *i_patRegisteredDate, INT i_LoginEventCheckNumber=0, UID32_t i_AccountUniqueNumber = 0);
	
	// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - �α����̺�Ʈ �������� �Ǵ� �Լ�
	BOOL IsEnableCheckLoginEvent();

	SITEM_EVENT_INFO* GetItemEventInfoByEventUID(int i_nItemEventUID);	// 2013-03-29 by jhseol, ������ �̺�Ʈ - �ſ� ù ����� ��������

protected:
	mtvectItemEventInfo	m_mtEventItemList;
};

#endif // !defined(AFX_GIVEITEMMANAGER_H__587F770B_FAA0_45E0_9EBB_556823A0EC7F__INCLUDED_)

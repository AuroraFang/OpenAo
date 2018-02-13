#ifndef _ATUM_FIELD_PARTY_H_
#define _ATUM_FIELD_PARTY_H_

#include "mt_stl.h"
#include "AtumParam.h"
#include "AtumProtocol.h"
#include "FieldIOCPSocket.h"

class CFieldIOCP;
class CFieldIOCPSocket;
class CFieldMapChannel;
class CFieldPartyBattle;

struct FieldPartyMember
{
	CFieldIOCPSocket	*PM_CharacterSocket;

	FieldPartyMember() : PM_CharacterSocket(NULL) {}
	FieldPartyMember(CFieldIOCPSocket *pSock) : PM_CharacterSocket(pSock) {}
};

class CFieldParty;
class CFieldPartyBattle
{
public:
	CFieldPartyBattle();

	void Reset();

	void InitFieldPartyBattle(CFieldParty *pMyParty, CFieldParty *pPeerParty, CFieldMapChannel *pMapChannel);
	void CleanFieldPartyBattle();

	BOOL IsUsing();

public:
	CFieldParty			*m_pPeerParty;
	mtsetUID32			m_setLiveMember;		// ����ִ� ��Ƽ���� CharacterUID
	mtsetUID32			m_setLossWaiters;		// ��Ƽ �� �й� ��, ó���� ��ٸ��� ���� ����Ʈ(�̹� ó���� ��Ƽ���� ������), CharacterUID
	CFieldMapChannel	*m_pBattleMapChannel;	// ��Ƽ���� �Ͼ�� ��
	INT					m_nAverageLevel;		// ��Ƽ�� ���۽��� ��� level
	INT					m_nPeerAverageLevel;	// ��Ƽ�� ���۽��� ������ ��� level
};

typedef mt_vector<FieldPartyMember>		mtlistFieldPartyMember;

class CFieldParty
{
public:
	CFieldParty(CFieldIOCP* pFieldIOCP, PartyID_t pid, UID32_t masterUniqueNumber);
	~CFieldParty();

	BOOL InsertMember(CFieldIOCPSocket *pSocket);
	BOOL DeleteByCharacterUniqueNumber(UID32_t characterUniqueNumber, FieldPartyMember& ret);
	UID32_t GetMasterCharacterUniqueNumber();
	BOOL empty();
	BOOL GetMemberByCharacterUniqueNumber(UID32_t characterUniqueNumber, FieldPartyMember& ret);
	BYTE GetLowestMemberLevel(UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER);
	BYTE GetMaxMemberLevel(UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER);
	BOOL UpdateMemberInfoAllInMap(CFieldIOCPSocket *pSocket, CFieldMapChannel *pMapChannel);		// �ڽ��� ������ �ٸ� ��Ƽ������ �����ϰ� �ٸ� ��Ƽ���� ������ �޾ƿ�. ������ call��.
	BYTE GetMaxMemberLevelInSameMap(CFieldMapChannel *i_pFMapChann, BOOL i_bLiveCheck=FALSE);		// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - �Լ� �߰�
	BOOL SetReturnUserBuffSkill(CFieldIOCPSocket *pSocket);						// 2013-02-28 by bckim, �������� �����߰�
	// CFieldIOCPSocket[SIZE_MAX_PARTY_MEMBER]�� param���� �ش�.
	// ����: return���� pMemberSocketArray�� �� ó���� �� ���� lock�� ��Ƶξ� ��
	// 2007-06-26 by cmkwon, ��Ƽ ����ġ ���� ����, ���� ����(���� - CFieldIOCPSocket **pMemberSocketArray, CFieldMapChannel *pMapChannel, D3DXVECTOR3 *pPos = NULL, float fRadius = 0, BYTE *o_byMaxLevel=NULL)
	int GetMembersInSameMap(CFieldIOCPSocket **o_pArrMemFISock, CFieldMapChannel *i_pFMapChann, BOOL i_bLiveCheck=FALSE, BYTE *o_byMaxLevel=NULL);
	int QuestWarpGetMembers(CFieldIOCPSocket **o_pArrMemFISock, CFieldMapChannel *i_pFMapChann);		// 2008-12-16 by dhjin, �̼Ǹ����� - ����Ʈ ���� �� ���� �޼��� ���� ���� �̱�.
	int GetMembersInSameMapToVector(vector<ClientIndex_t> *i_pVectorClientIndex, CFieldMapChannel *pMapChannel, D3DXVECTOR3 *pPos = NULL, float fRadius = 0, ClientIndex_t nClientIndexToExclude = INVALID_CLIENT_INDEX);
// 2007-02-12 by cmkwon, �Ʒ��Լ��� ������	BOOL CheckAllMemberEnableWarp(CFieldMapChannel *i_pFMChan, int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex);
	static BOOL CheckAllMemberEnableWarp(vectCFieldIOCPSocket *i_pvectFISockList, CFieldMapChannel *i_pFMChan, int i_nMapInfluenceTy, BOOL i_bConflictAraeMap, int i_nRequetCompletionQuestIndex);
	int GetAllMembers(CFieldIOCPSocket *pMemberSocketArray[SIZE_MAX_PARTY_MEMBER]);	// 2007-06-26 by cmkwon, ��Ƽ ����ġ ���� ����, ���� ����
	int GetAllMembers(vectCFieldIOCPSocket *i_pvectFISockList);

	// ���� �ʿ� �ִ� ��Ƽ�����Ը� ������.
	BOOL SendMsgToMembersMap(BYTE* buffer, int size, CFieldMapChannel *pMapChannel, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER, MSG_FC_COUNTDOWN_START *i_pCountdownStart=NULL);
	
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	void SendMsgToMembersMap(msg_t<t>& msg, CFieldMapChannel *pMapChannel, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER, MSG_FC_COUNTDOWN_START *i_pCountdownStart=NULL)
	{
		this->SendMsgToMembersMap(reinterpret_cast<BYTE*>(&msg), msg.size(), pMapChannel, CharacterUIDToExclude, i_pCountdownStart);
	}
	
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	void SendMsgToMembers(BYTE* buffer, int size, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER);
	void SendMsgToMembersExcludeMap(BYTE* buffer, int size, CFieldMapChannel *pMapChannel, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER);

	template<template<MessageType_t> typename msg_t, MessageType_t t>
	void SendMsgToMembers(const msg_t<t>& msg, UID32_t CharacterUIDToExclude = INVALID_UNIQUE_NUMBER)
	{
		this->SendMsgToMembers(reinterpret_cast<const BYTE*>(&msg), msg.size(), CharacterUIDToExclude);
	}

	void Print(CFieldIOCPSocket *pSocket);
	
	BYTE GetExpDistributeType(void);				// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - �Լ� �߰�
	void SetExpDistributeType(BYTE i_byExpDistTy);	// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - �Լ� �߰�
	BYTE GetItemDistributeType(void);					// 2008-06-03 by dhjin, EP3 ��� ���� - ������ �й� ��� 
	void SetItemDistributeType(BYTE i_byItemDistTy);	// 2008-06-03 by dhjin, EP3 ��� ���� - ������ �й� ��� 

	CFieldIOCPSocket * GetItemByRandomDistributeType(CFieldMapChannel *i_pFMapChann);		// 2008-06-03 by dhjin, EP3 ��� ���� - ������ ���� ���� ��÷��
	CFieldIOCPSocket * GetItemByOrderDistributeType(CFieldMapChannel *i_pFMapChann, BYTE * o_byPivotPartyMember);		// 2008-06-03 by dhjin, EP3 ��� ���� - ������ ���� ���� ��÷��
	void IncreasePivotPartyMember(BYTE i_byPivotPartyMember);					// 2008-06-10 by dhjin, EP3 ��� ���� - ������ ���� ���� ���� ��� ���� ����

public:
	PartyID_t					m_PartyID;
	UID32_t						m_MasterUniqueNumber;	// ��Ƽ���� CharacterUniqueNumber
	MEX_FIELD_PARTY_INFO		m_FieldPartyInfo;		// IMServer���� FieldServer�� �Ѱ��־�� �ϴ� ����
	mtlistFieldPartyMember		m_ListPartyMember;		// ���� �ʵ� �������� �����ȴ� ��Ƽ���� ����Ʈ
	CFieldIOCP					*m_pFieldIOCP7;
	PartyID_t					m_PeerBattleRequesterPartyID;	// ��Ƽ�� ��û���� ��Ƽ ���̵�, ��Ƽ�� ��û�ú��� ��Ƽ�� ���� ������ �Ҵ�ȴ�.
	PartyID_t					m_PeerBattlePartyID;	// ��Ƽ�� �� ���� ��Ƽ�� ���̵�, ��Ƽ�� ���� �� �Ҵ�ȴ�.
	CFieldPartyBattle			m_PartyBattle;			// ��Ƽ�� �� ���
	MAP_CHANNEL_INDEX			m_PartyBattleMapChannelIndex;
	DWORD						m_dwPartyDeletedTick;
	BYTE						m_byExpDistributeType;	// 2007-06-27 by cmkwon, ��Ƽ�� ����ġ �й� ��� �ý��� �߰� - ����ġ �й� Ÿ��(EXP_DISTRIBUTE_TYPE_XXX)
	BYTE						m_byItemDistributeType;	// 2008-06-03 by dhjin, EP3 ��� ���� - ������ �й� ���
	BYTE						m_byPivotPartyMember;	// 2008-06-04 by dhjin, EP3 ��� ���� - ������ ���� ���� ���� ��� ����
};

#endif // _ATUM_FIELD_PARTY_H_

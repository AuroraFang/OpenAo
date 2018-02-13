#ifndef __ATUM_IM_DB_MANAGER_H__
#define __ATUM_IM_DB_MANAGER_H__

#include "AtumDBManager.h"

class CIMIOCP;
class CIMIOCPSocket;

class CAtumIMDBManager : public CAtumDBManager
{
public:
	struct MSG_DB_GUILD_CREATE
	{
		char		szGuildName[SIZE_MAX_GUILD_NAME];				// �����̸�
		UID32_t		uidCommanderCharacter;							// ������ CharacterUniqueNumber
		int			nMemberCounts;									// ���ܿ� ī��Ʈ
		UID32_t		arrMemberCharacterUID[SIZE_MAX_PARTY_MEMBER];	// ���ܿ� CharacterUniqueNumber(������ ����)
		BYTE		arrGuildRank[SIZE_MAX_PARTY_MEMBER];
	};
	
public:
	CAtumIMDBManager(CIMIOCP *pIMIOCP);

	BOOL ProcessServerQuery(DB_QUERY dbquery, SQLHSTMT &hstmt, SQLHSTMT &hstmt_mc, SQLHSTMT &hstmt_extAuth, SQLHSTMT &hstmt_GLog);		// 2013-06-20 by jhseol,bckim GLog ����

	void QP_AuthChatLogin(DB_QUERY q, SQLHSTMT hstmt);
	void QP_SaveLastPartyID(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildCreate(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildAddMember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildAddOffMember(DB_QUERY q, SQLHSTMT hstmt);			// 2008-06-12 by dhjin, EP3 - ���� ���� ���� - �������� ���� ����
	void QP_GuildLeaveMember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildBanMember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildUpdateMemberCapacity(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildLoadGuild(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildDismember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildCancelDismember(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildChangeGuildName(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildSetGuildMark(DB_QUERY q, SQLHSTMT hstmt);
// 2007-08-02 by cmkwon, ���� ��ũ �ɻ� �ý��� ���� - ������� �ȴ� �Լ���
//	void QP_GuildGetGuildMark(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildSetRank(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildDeleteGuild(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildSaveGuildWarPoint(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildDeleteGuildUIDOfCharacter(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildAddGuildFame(DB_QUERY q, SQLHSTMT hstmt);
	void QP_GuildUpdateCommander(DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-20 by dhjin, EP3 - ���� ���� ���� - ������ ����
	void QP_GuildNotice(DB_QUERY q, SQLHSTMT hstmt);				// 2008-05-20 by dhjin, EP3 - ���� ���� ���� - ���� ����
	void QP_GuildGetApplicant(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ�
	void QP_GuildGetIntroduction(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ�
	void QP_GuildDeleteIntroduction(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ�
	void QP_GetSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� ������ �Ұ���
	void QP_GuildSearchIntroduction(DB_QUERY q, SQLHSTMT hstmt);			// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - ���� �Ұ� �˻�
	void QP_GuildUpdateIntroduction(DB_QUERY q, SQLHSTMT hstmt);		// 2008-05-27 by dhjin,	EP3 - ���� ���� ���� - ���� �Ұ� �ۼ�
	void QP_GuildUpdateSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt);	// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - �ڱ� �Ұ� �ۼ� 
	void QP_GuildDeleteSelfIntroduction(DB_QUERY q, SQLHSTMT hstmt);	// 2008-05-27 by dhjin, EP3 - ���� ���� ���� - �ڱ� �Ұ� ����� 
	void QP_GuildDeleteSelfIntroductionOffUser(DB_QUERY q, SQLHSTMT hstmt);	// 2008-06-13 by dhjin, EP3 - ���� ���� ���� - �ڱ� �Ұ� ����� 
	void QP_InsertNotifyMsg(DB_QUERY q, SQLHSTMT hstmt);	// 2008-06-13 by dhjin, EP3 - ���� ���� ���� - ���� �޼��� ������

	void QP_FriendInsertFriend(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FriendDeleteFriend(DB_QUERY q, SQLHSTMT hstmt);
	void QP_FriendLoadFriends(DB_QUERY q, SQLHSTMT hstmt);

	void QP_ReloadAdminAutoNotice(DB_QUERY q, SQLHSTMT hstmt);		// 2009-01-14 by cmkwon, ��� �ڵ� ���� �ý��� ���� - 

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-01 by cmkwon, �������� �����带 ����� ó���ϴ� �ý��� ���� - 
	virtual BOOL ProcessDynamicServerQuery(DB_QUERY *i_qQuery, CODBCStatement *i_pODBC);

private:
	void ProcessLogMessagesIM(SQLSMALLINT plm_handle_type, SQLHANDLE &plm_handle, char *logstring, int ConnInd, CIOCPSocket *pIOCPSocket);

public:
	CIMIOCP	*m_pIMIOCP5;
};

struct QPARAM_SAVELASTPARTYID
{
	UID32_t		CharacterUniqueNumber;
	PartyID_t	LastPartyID;
};

struct QPARAM_CHANGE_GUILD_NAME
{
	UID32_t		GuildUniqueNumber;
	char		NewGuildName[SIZE_MAX_GUILD_NAME];
};

//////////////////////////////////////////////////////////////////////////
// 2008-05-28 by dhjin, EP3 - ���� ���� ����
struct QPARAM_GUILD_NOTICE
{
	char		Notice[SIZE_MAX_NOTICE];
};

struct QPARAM_GUILD_UPDATE_INTRODUCTION
{
	char			GuildIntroduction[SIZE_MAX_NOTICE];
	ATUM_DATE_TIME	WriteDate;
};

struct QPARAM_GUILD_UPDATE_SELFINTRODUCTION
{
	UID32_t		TargetGuildUID;
	char		SelfIntroduction[SIZE_MAX_NOTICE];
};

#endif

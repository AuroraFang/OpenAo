// ClientParty.h: interface for the CClientParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTPARTY_H__DB818FDB_228D_4091_B3DF_73DF6410F56E__INCLUDED_)
#define AFX_CLIENTPARTY_H__DB818FDB_228D_4091_B3DF_73DF6410F56E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "AtumDatabase.h"		// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����

class CEnemyData;

// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
struct sDebuffDotInfo
{
	INT		nSkillItemNum;	// ����� ��Ʈ, ����� ������ �ѹ�
	INT		nSourceIndex;	// ��Ʈ, ����� �ҽ� �ε���

	sDebuffDotInfo( ITEM* pSkillItem )
	{
		nSkillItemNum	= pSkillItem->ItemNum;
		// 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
//		nSourceIndex	= SKILL_BASE_NUM( pSkillItem->SourceIndex );
		nSourceIndex	= SKILL_BASE_SOURCEINDEX( pSkillItem->SourceIndex );
		// END 2013-03-13 by bhsohn ��ų ��ȣ ���� ����
	}
};
// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����

struct PARTYENEMYINFO
{
	LONGLONG		m_nUniqueNumber;		// ������ UniqueNumber
	PARTY_TYPE		m_bPartyType;
	BOOL			m_bFormationFlight;		// ��� �������� ��Ƽ���ΰ�?
	BYTE			m_bFormationFlyingType;
	BYTE			m_bFormationFlyingPositionType;
	IM_PARTY_MEMBER_INFO m_ImPartyMemberInfo;
	CEnemyData*		m_pEnemyData;
	BOOL			m_bSpeakingRender;
	EN_CHECK_TYPE	VoipType;
	BOOL			m_bUserLogOn;		// 2008-12-02 by bhsohn ���� �¿������� ����üũ

	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	std::vector<sDebuffDotInfo> m_vecDebuffDotInfo;
	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
};

typedef struct 
{
	LONGLONG		m_nUniqueNumber;
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vVel;
	D3DXVECTOR3		m_vSide;
	D3DXVECTOR3		m_vUp;
} MASTERINFO;

typedef struct
{
	ClientIndex_t	ClientIndex;
	UID32_t			CharacterUniqueNumber;
} OTHERPARTYINFO;

class CClientParty  
{
public:
	CClientParty();
	virtual ~CClientParty();
	void			Tick(float fElapsedTime) const;	// ���� ������
	static constexpr bool	GetNtoNCallState() { return false; } // m_bNtoNCallState; }
	BOOL			IsParty() const { return m_bParty; }
	BOOL			IsFormationFlight() const { return m_bFormationFlight; }	// ���������ΰ�?
	BOOL			IsFormationFlightMove();							// ������ �̵��� �ؾ� �ϴ°�?
	void			SetMasterInfo();
	MASTERINFO		GetMasterInfo();
	void			SetPartyInfo(PARTYINFO infoParty) { memcpy((char*)&m_infoParty,(char*)&infoParty,sizeof(PARTYINFO)); }
	PARTYINFO		GetPartyInfo() const { return m_infoParty; }
	void			SetFormationFlight() { m_bFormationFlight = TRUE; }
	void			ReleaseFormationFlight() { m_bFormationFlight = FALSE; }
	void			FormationFlightDismember();											// �������� ��ü��. 
	void			FormationFlightClear();												// ������ ���� �ʱ�ȭ
	PARTYENEMYINFO*	PartyFindMemberByClientIndex(ClientIndex_t nClientIndex);			// ��Ƽ�� ������ ����
	PARTYENEMYINFO*	PartyFindMemberByUniqueNumber(LONGLONG nUniqueNumber);				// ��Ƽ�� ������ ����
	PARTYENEMYINFO* PartyFindMemberByUnitName(char* strName);							// ��Ƽ�� ������ ����
	PARTYENEMYINFO*	FormationFlightFindMemberByClientIndex(ClientIndex_t nClientIndex);	// ���� ������ ����
	PARTYENEMYINFO*	FormationFlightFindMemberByUniqueNumber(LONGLONG nUniqueNumber);	// ���� ������ ����

// ������ ���� ó�� �Լ���
	void	FormationFlightMoveForShuttleChild();
	void	FormationFlightMoveForEnemy();

// ����� ���� �Լ�
	BOOL	IsFVFEnemy(UID32_t nUniqueNum);

// ��Ƽ ���� �������� �޾��� �� ó�� �Լ���(I->C)
	void	IPartyPutMember(MSG_IC_PARTY_PUT_MEMBER* pMsg);												// ��Ƽ�� �Ѹ��� �߰��Ͽ��� ��					MSG_IC_PARTY_PUT_MEMBER			
	void	IPartyAcceptInviteOk(MSG_IC_PARTY_ACCEPT_INVITE_OK* pMsg);									// ��Ƽ �ʴ븦 �������� ��						MSG_IC_PARTY_ACCEPT_INVITE_OK
	void	IPartyBanMemberOk(MSG_IC_PARTY_BAN_MEMBER_OK* pMsg);										// ��Ƽ�� �Ѹ��� �߹��Ͽ��� ��					MSG_IC_PARTY_BAN_MEMBER_OK
	void	IPartyLeaveOk(MSG_IC_PARTY_LEAVE_OK* pMsg);													// ��Ƽ�� �Ѹ��� ������ ��						MSG_IC_PARTY_LEAVE_OK
	void	IPartyDismemberOk(MSG_IC_PARTY_DISMEMBER_OK* pMsg);											// ��Ƽ�� �ػ�Ǿ��� ��							MSG_IC_PARTY_DISMEMBER_OK
	void	IPartyPutAllMember(MSG_IC_PARTY_PUT_ALL_MEMBER* pMsg);										// ��� ��Ƽ�� ����Ʈ�� �޾��� ��				MSG_IC_PARTY_PUT_ALL_MEMBER
	void	IPartyPutLastPartyInfo(MSG_IC_PARTY_PUT_LAST_PARTY_INFO* pMsg);								// ������ ��Ƽ�� ���� ��						MSG_IC_PARTY_PUT_LAST_PARTY_INFO
	void	IPartyTransferMasterOk(MSG_IC_PARTY_TRANSFER_MASTER_OK* pMsg);								// ��Ƽ �����Ͱ� ����Ǿ��� ��					MSG_IC_PARTY_TRANSFER_MASTER_OK
	void	IPartyMemberInvalidated(MSG_IC_PARTY_MEMBER_INVALIDATED* pMsg);								// �߸��� ��Ƽ���� �߰��Ǿ��� ��?				MSG_IC_PARTY_MEMBER_INVALIDATED
	void	IPartyMemberRejoined(MSG_IC_PARTY_MEMBER_REJOINED* pMsg);									// ��Ƽ���� �ٽ� �������� ��?					MSG_IC_PARTY_MEMBER_REJOINED
	void	IPartyUpdateMemberInfoMapname(MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME* pMsg);				// ?											MSG_IC_PARTY_UPDATE_MEMBER_INFO_MAPNAME
	void	IPartyUpdateItemPos(MSG_IC_PARTY_UPDATE_ITEM_POS* pMsg);

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	void	IPartyGetAutoPartyInfoOK( MSG_IC_PARTY_GET_AUTO_PARTY_INFO_OK* pMsg );
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

// ��Ƽ ���� �������� �޾��� �� ó�� �Լ���(F->C)
	void	FPartyBattleEnd(MSG_FC_PARTY_BATTLE_END* pMsg);												// ��Ƽ���� ������ ��							MSG_FC_PARTY_BATTLE_END
	void	FPartyBattleReady(MSG_FC_PARTY_BATTLE_START* pMsg);											// ��Ƽ���� ���۵Ǿ��� ��						MSG_FC_PARTY_BATTLE_READY
	void	FPartyCreateOk(MSG_FC_PARTY_CREATE_OK* pMsg);												// ��Ƽ�� �������� ��							MSG_FC_PARTY_CREATE_OK
	void	FPartyPutItemOther(MSG_FC_PARTY_PUT_ITEM_OTHER* pMsg);										// ��Ƽ���� �������� ȹ������ ��				MSG_FC_PARTY_PUT_ITEM_OTHER
	void	FPartyPutMember(MSG_FC_PARTY_PUT_MEMBER* pMsg);												// ��Ƽ���� �߰����� ��	? ������?				MSG_FC_PARTY_PUT_MEMBER
	void	FPartyRejectInviteOk(MSG_FC_PARTY_REJECT_INVITE_OK* pMsg);									// ��Ƽ �ʴ븦 �������� ��						MSG_FC_PARTY_REJECT_INVITE_OK
	void	FPartyRequestInviteQuestion(MSG_FC_PARTY_REQUEST_INVITE_QUESTION* pMsg);					// ?											MSG_FC_PARTY_REQUEST_INVITE_QUESTION
	void	FPartyUpdateMemberInfoAll(MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL* pMsg);						// ��Ƽ�� �Ѹ��� ��� ������ �޾��� ��			MSG_FC_PARTY_UPDATE_MEMBER_INFO_ALL
	void	FPartyUpdateMemberInfoBodyCondition(MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION* pMsg);	// ��Ƽ�� �Ѹ��� �ٵ�������� �޾��� ��			MSG_FC_PARTY_UPDATE_MEMBER_INFO_BODYCONDITION
	void	FPartyUpdateMemberInfoCurrentDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP* pMsg);			//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_DP
	void	FPartyUpdateMemberInfoCurrentEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP* pMsg);			//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_EP
	void	FPartyUpdateMemberInfoCurrentHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP* pMsg);			//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_HP
	void	FPartyUpdateMemberInfoCurrentSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP* pMsg);			//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_CURRENT_SP
	void	FPartyUpdateMemberInfoDP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP* pMsg);							//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_DP
	void	FPartyUpdateMemberInfoEP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP* pMsg);							//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_EP
	void	FPartyUpdateMemberInfoHP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP* pMsg);							//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_HP
	void	FPartyUpdateMemberInfoSP(MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP* pMsg);							//												MSG_FC_PARTY_UPDATE_MEMBER_INFO_SP
	void	FPartyUpdateMemberInfoLevel(MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL* pMsg);					// ��Ƽ�� �Ѹ��� ���������� �޾��� ��			MSG_FC_PARTY_UPDATE_MEMBER_INFO_LEVEL
	// 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	void	FPartyUpdateMemberInfoDebuffDotInfo( MSG_FC_CHARACTER_DEBUFF_DOT_INFO_OK* pMsg );
	void	FPartyUpdateMemberInfoDebuffDotApply( MSG_FC_CHARACTER_DEBUFF_DOT_APPLYING* pMsg );
	void	FPartyUpdateMemberInfoDebuffDotRelease( MSG_FC_CHARACTER_DEBUFF_DOT_RELEASE* pMsg );
	// end 2009. 09. 21 by ckPark ���Ǵ�Ƽ �ʵ� ���� ��ų ����
	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	void	FPartyAutoCreateOK( MSG_FC_PARTY_AUTO_CREATED* pMsg );
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

// ������ ���� �������� �޾��� �� ó�� �Լ���(I->C)
	void	IPartyAllFlightPosition(MSG_IC_PARTY_ALL_FLIGHT_POSITION* pMsg);							// ��� ������ ������ �������� ���� ��		MSG_IC_PARTY_ALL_FLIGHT_POSITION
	void	IPartyGetFlightPosition(MSG_IC_PARTY_GET_FLIGHT_POSITION* pMsg);							// ������ �߰��Ǿ��� ��						MSG_IC_PARTY_GET_FLIGHT_POSITION
	void	IPartyCancelFlightPosition(MSG_IC_PARTY_CANCEL_FLIGHT_POSITION* pMsg);						// ������ ������ ��							MSG_IC_PARTY_CANCEL_FLIGHT_POSITION
	void	IPartyChangeFlightFormationOk(MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK* pMsg);				// ������ Ÿ���� ����Ǿ��� ��				MSG_IC_PARTY_CHANGE_FLIGHT_FORMATION_OK
	void	IPartyChangeFlightPosition(MSG_IC_PARTY_CHANGE_FLIGHT_POSITION* pMsg);						// ?											MSG_IC_PARTY_CHANGE_FLIGHT_POSITION

//	�������� ���� �� ó�� �Լ���
	void	FSendPartyRequestPartyWarp();																// ��Ƽ ������ ������ ��û
	void	FSendPartyRequestPartyObjectEvent(CObjectChild * pObj);										// ��Ƽ ������Ʈ �̺�Ʈ�� ������ ��û
	void	ISendPartyChangeFlightFormation(int nFormationFlightType);									// ������ �����̼� Ÿ�� ������ ������ ��û
	void	FSendEventSelectChannelWithParty(MSG_FC_EVENT_SELECT_CHANNEL msgEventSelectChannel);															// ������ �̺�Ʈ�� �������Բ� ������ ��û      
	void	FSendPartyRequestPartyWarpWithMapName(char* pName);	
	void	ISendPartyGetFlightPosition();
	void	FSendPartyAcceptInvite(PartyID_t nPartyRequestID);
	void	FSendPartyRejectInvite(PartyID_t nPartyRequestID);
	void	ISendPartyLeave() const;
	void	ISendPartyBanMember(LONGLONG nUniqueNumber) const;
	void	ISendPartyTransferMaster(LONGLONG nUniqueNumber) const;
	static void	ISendPartyCreate();
	static void	FSendPartyRequestInvite(const char* pName);
	// 2008-02-27 by bhsohn ���� �Ʒ��� ����
	static void	ISendPartyLeaveArena();


	// 2008-06-03 by bhsohn EP3 ��� ���� ó��
	void			SetSPartyInfo(SPARTY_INFO *pSPartyInfo);
	SPARTY_INFO*	GetSPartyInfo() { return &m_stPartyInfo; }
	void			FSendPartyJoinFree(PartyID_t	nPartyRequestID, char* pPass);
	void			ChagePartyFormation(int nFormation);
	void			SetFirstPartySetupInfo(BOOL bFirstPartySetupInfo);
	BOOL			GetFirstPartySetupInfo();

	// 2008-06-17 by bhsohn ��� ���� ó��
	void			RefreshFormationType();						// ���� ���������� ����
	void			TempPartyFormation(int nFormation);		// ��ų�� ����Ͽ� ��� ���� ����
	int				GetOldFormationFlyingType();			// ���� ��밡�־���.

	// 2008-10-20 by bhsohn VOIP ���� ����
	static constexpr EN_CHECK_TYPE GetVoiceTypePartyMaster() { return CHECK_TYPE_VOIP_NONE; }

	// 2008-12-16 by dgwoo �̼� ������ ���� �Լ�.
	void			SetMissionMasterPartyID(PartyID_t i_nID);
	PartyID_t		GetMissionMasterPartyID();

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	INT		GetInSameMapPartyMemberCount();
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

private:
// ��Ƽ ����
	void			PartyCreate(PartyID_t PartyID);											// ��Ƽ ����
	void			PartyDeleteAll();														// ��� ��Ƽ ������ ����
	void			PartyInsertMember(IM_PARTY_MEMBER_INFO& ImPartyMemberInfo);								// ��Ƽ�� �߰�
	void			PartyDeleteMember(LONGLONG nUniqueNumber,char* pName);					// ��Ƽ�� ����
	void			PartyChangeMaster(LONGLONG nNewMasterUniqueNumber,LONGLONG nOldMasterUniqueNumber,char* pName);			// ������ ����

// ������ ����
	void			FormationFlightInsertMember(LONGLONG nUniqueNumber);				// ������ �߰��Ǿ��� ��
	void			FormationFlightDeleteMember(LONGLONG nUniqueNumber);				// ������ ������ ��
	void			FormationFlightChangeType(BYTE nFormationFlightType);				// ������ Ÿ���� ����Ǿ��� ��
	BOOL			FormationFlightSortPosition();										// �������� �������� �����Ѵ�.

// ������ ����
	void	FormationFlightForShuttleChildType1();
	void	FormationFlightForShuttleChildType2();
	void	FormationFlightForShuttleChildType3();
	void	FormationFlightForShuttleChildType4();
	void	FormationFlightForShuttleChildType5();
	void	FormationFlightForShuttleChildType6();
	void	FormationFlightForShuttleChildType7();
	void	FormationFlightForShuttleChildType8();
	void	FormationFlightForEnemyType1(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType2(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType3(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType4(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType5(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType6(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType7(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemyType8(PARTYENEMYINFO* pEnemyData);
	void	FormationFlightForEnemySetPos(PARTYENEMYINFO* pEnemyData) const;

public:
	vector<PARTYENEMYINFO*>		m_vecPartyEnemyInfo;		// ���� ������� EnemyList
	vector<PEER_PARTY_MEMBER>	m_vecPeerPartyEnemyInfo;	// �츮 ���� �������� �ٸ� ��Ƽ�� List
private:
	BOOL		m_bParty;								// ���� ��Ƽ���ΰ�?
	BOOL		m_bFormationFlight;						// ���� ���������ΰ�?
	PARTYINFO	m_infoParty;							// ���� ��Ƽ����
	MASTERINFO	m_infoMaster;							// ��� ������ ����
	
	// 2016-07-19 panoskj disabled this													
	//BOOL		m_bNtoNCallState;						// ��� ��ȭ ��?

	// 2008-06-03 by bhsohn EP3 ��� ���� ó��
	SPARTY_INFO			m_stPartyInfo;

	int				m_nOldFormationFlyingType;
	BOOL			m_bFirstPartySetupInfo;

	// 2008-12-16 by dgwoo �̼Ǹ����� �ӽ� ��Ƽ ��ȣ.
	PartyID_t		m_nTempMMasterPartyID;
};

#endif // !defined(AFX_CLIENTPARTY_H__DB818FDB_228D_4091_B3DF_73DF6410F56E__INCLUDED_)

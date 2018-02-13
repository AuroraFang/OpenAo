// IMParty.h: interface for the CIMParty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPARTY_H__851E1E96_0EBA_43A7_A37A_CC68D4255EE9__INCLUDED_)
#define AFX_IMPARTY_H__851E1E96_0EBA_43A7_A37A_CC68D4255EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumProtocol.h"
#include "MTCriticalSection.h"


class CIMIOCP;
class CIMIOCPSocket;
class CIMParty;

struct IMPartyMember
{
public:
	IMPartyMember();
	IMPartyMember(CIMParty *i_pParty);
	IMPartyMember(char *i_szCharName, UID32_t i_CharacterUID, CIMIOCPSocket *i_pIISocket, CIMParty *i_pParty);
	void ResetIMPartyMember(void);

	char			PM_CharacterName[SIZE_MAX_CHARACTER_NAME];	// ���ӿ��� �����ִ� ��Ƽ���� �̸��� �˷��ֱ� ����
	UID32_t			PM_CharacterUniqueNumber;					// ���ӿ��� �����ִ� ��Ƽ���� �������ش�
	CIMIOCPSocket	*PM_pIMIOCPSocket;							// ���� ��Ƽ���� ������� CIMIOCPSocket�� ������, NULL�̸� �¶��λ��°� �ƴϴ�
	CIMParty		*PM_pIMParty;

	BOOL IsUsingMember(void);

	BOOL IsOnlineMember(void);									// �������� ��Ƽ������ Ȯ��
	void SetOnlineMember(CIMIOCPSocket *pCharacterSocket);		// ���� ���� �� �ٽ� ������ ��, CHECK_REJOIN�� ������ ������
	void SetOfflineMember( void );	// ������������ ��������Ǿ��� ��
};

class CIMParty
{
public:
	CIMParty(CIMIOCP* pIMIOCP, UID32_t CharacterUniqueNumber, DWORD i_dwCreateTick, UID32_t UniqueNumber, SPARTY_INFO * i_pSPartyInfo); 	// 2008-06-02 by dhjin, EP3 ��� ���� - ��� ���� );
	~CIMParty();
	
	BOOL IsValidIMParty(void);
	BOOL IsExistMember(UID32_t i_uidCharacter, BOOL i_bIsOnline=TRUE);
	
	BOOL PushBackMember(IMPartyMember &partyMember);
	BOOL LeaveMember(UID32_t i_uidCharacter);
	BOOL TransferMaster(UID32_t i_uidNewMasterCharacter=INVALID_UNIQUE_NUMBER, BOOL i_bResetFlag=FALSE, BOOL i_bSetOffline=FALSE);
	
	char *GetMasterCharacterName(void);
	UID32_t GetMasterCharacterUniqueNumber(void);
	CIMIOCPSocket *GetMasterIMIOCPSocket(void);

	IMPartyMember* GetMemberPtrByCharacterUniqueNumber(UID32_t characterUniqueNumber);
	BYTE GetLowestMemberLevel(UINT CharacterToExclude = INVALID_UNIQUE_NUMBER);

	BOOL SendMsgToMembers(BYTE* buffer, int size, UINT CharacterToExclude=INVALID_UNIQUE_NUMBER, BOOL i_bResetIMIOCPSocketParty=FALSE);
//	BOOL SendChatMsgToMembers(BYTE* buffer, int size, BitFlag8_t i_bitFlag8);
	BOOL SendChatMsgToMembers(BYTE* buffer, int size, BitFlag16_t i_bitFlag16);			// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����
	
	BOOL SetOfflineMemberW(UID32_t characterUniqueNumber );	// ��Ƽ�� ��������Ǿ��� ��

	int GetMemberCount(BOOL bOnlyValidMember = TRUE);

	void LockIMParty(void){			m_mtcsParty.Enter();}
	void UnlockIMParty(void){		m_mtcsParty.Leave();}

	void MakeMsg_IC_PARTY_PUT_ALL_MEMBER(UINT *o_pnMemberCounts, IM_PARTY_MEMBER_INFO *o_pIMPMemberInfo, UID32_t i_uidCharacterToExclude);
	int MakeMsg_DB_GUILD_CREATE(int *o_pnMemberCounts, UID32_t o_arrUIDMember[]);

	//////////////////////////////////////////////////////////////////////////
	// 2008-06-04 by dhjin, EP3 ��� ����
	void SetPartyInfo(SPARTY_INFO * i_pPartyInfo);		// 2008-06-04 by dhjin, EP3 ��� ���� - ��� ���� ����
	void SendRecommendationMember(CIMIOCPSocket * i_pIISocket);	// 2008-06-04 by dhjin, EP3 ��� ���� - ��õ �ɸ��� ��û
	INT  MakeMSG_IC_PARTY_RECOMMENDATION_MEMBER_OK(SRECOMMENDATION_MEMBER_INFO * o_pRecommendationMember, CIMIOCPSocket * i_pPartyMasterSocket, INT i_nMinLevel, INT i_nMaxLevel);	// 2008-11-18 by dhjin, ��Ƽ���� ���������� �����ϴ��� üũ. �������� IM���� �ٿ� ���� ����
	
	BOOL CheckVoipUse();				// 2008-06-17 by dhjin, EP3 VOIP - ��������� üũ
	BOOL m_bArenaIMServerCheck;			// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : �Ʒ��� �������� üũ

public:
	CMTCriticalSection		m_mtcsParty;
	IMPartyMember			m_arrPartyMember[SIZE_MAX_PARTY_MEMBER];
	
	PartyID_t				m_PartyID;
	DWORD					m_PartyCreationTick;		// ��Ƽ ���� �ð�, timeGetTime()���� �Ҵ���
	DWORD					m_PartyDeletedTick;			// ��Ƽ ���� �ð�, timeGetTime()���� �Ҵ���
	IMPartyMember			*m_pMasterIMPartyMember;	// ��Ƽ���� IMPartyMember

	static CIMIOCP			*ms_pIMIOCP2;

	SPARTY_INFO				m_SPartyInfo;				// 2008-06-02 by dhjin, EP3 ��� ���� - ��� ���� 
};

inline IM_PARTY_MEMBER_INFO& IM_PARTY_MEMBER_INFO::operator=(const IMPartyMember& rhs)
{
	memset(this, 0, sizeof(IM_PARTY_MEMBER_INFO));
	this->CharacterUniqueNumber	= rhs.PM_CharacterUniqueNumber;
	util::strncpy(this->CharacterName, rhs.PM_CharacterName, SIZE_MAX_CHARACTER_NAME);
	this->MapChannelIndex.Invalidate();
	return *this;
}

#endif // !defined(AFX_IMPARTY_H__851E1E96_0EBA_43A7_A37A_CC68D4255EE9__INCLUDED_)

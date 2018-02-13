// IMWinSocket.cpp: implementation of the CIMWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IMWinSocket.h"
#include "FieldIOCP.h"
#include "FieldGlobal.h"
#include "AtumError.h"
#include "FieldParty.h"
#include "AtumLogSender.h"
//#include "SecurityManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIMWinSocket::CIMWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify) :
	CWinSocket(ST_IM_SERVER, hWnd, wmSock, wmNotify, FALSE)
{
	m_pFieldIOCP4 = NULL;
}

CIMWinSocket::~CIMWinSocket()
{
	
}

void CIMWinSocket::SetFieldIOCP(CFieldIOCP *pFieldIOCP)
{
	m_pFieldIOCP4 = pFieldIOCP;
}


BOOL CIMWinSocket::OnCloseSocket(int nErrorCode)
{
	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CIMWinSocket::OnConnect(int nErrorCode)
{
	CWinSocket::OnConnect(nErrorCode);
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////
	return	TRUE;
}


BOOL CIMWinSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int					nBytesUsed	= 0;
	MessageType_t		nType		= 0;
	MessageType_t		prevMsgType = 0;
	ProcessResult		procRes		= RES_BREAK;

	while(nBytesUsed < nLength)
	{
		nType = *(MessageType_t*)(pPacket+nBytesUsed);
		nBytesUsed += SIZE_FIELD_TYPE_HEADER;

#ifdef _DEBUG
		PrintExchangeMsg(RECV_TYPE, nType, m_szPeerIP, ST_IM_SERVER, GGetexchangeMsgPrintLevel());
#endif
		g_pFieldGlobal->m_dwLastSocketMessage = nType;
		switch(nType)
		{
		case T_FI_CONNECT_OK:
			{
				DBGOUT("FieldServer successfully registered to IMServer...\n");
			}
			procRes = RES_RETURN_TRUE;
			break;
		// 2007-08-27 by cmkwon, �����ٿ��غ� ��ɾ� �߰�(SCAdminTool���� SCMonitor�� PrepareShutdown�� ���� �� �� �ְ�)
		case T_FI_CONNECT_PREPARE_SHUTDOWN:
			procRes = Process_FI_CONNECT_PREPARE_SHUTDOWN(pPacket, nLength, nBytesUsed);
			break;


		case T_FI_PARTY_CREATE_OK:
			procRes = Process_FI_PARTY_CREATE_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_ACCEPT_INVITE_OK:
			procRes = Process_FI_PARTY_ACCEPT_INVITE_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_LEAVE_OK:
			procRes = Process_FI_PARTY_LEAVE_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_TRANSFER_MASTER_OK:
			procRes = Process_FI_PARTY_TRANSFER_MASTER_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_BAN_MEMBER_OK:
			procRes = Process_FI_PARTY_BAN_MEMBER_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_DISMEMBER_OK:
			procRes = Process_FI_PARTY_DISMEMBER_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_ADD_MEMBER:
			procRes = Process_FI_PARTY_ADD_MEMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_DELETE_MEMBER:
			procRes = Process_FI_PARTY_DELETE_MEMBER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_UPDATE_PARTY_INFO:
			procRes = Process_FI_PARTY_UPDATE_PARTY_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE:	// 2008-06-04 by dhjin, EP3 ��� ���� - ����ġ �й� ��� ���� 
			procRes = Process_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE: // 2008-06-04 by dhjin, EP3 ��� ���� - ������ �й� ��� ����
			procRes = Process_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_CHANGE_FORMATION_SKILL:		// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - 
			procRes = Process_FI_PARTY_CHANGE_FORMATION_SKILL(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_AUTO_CREATE_OK:				// 2009-09-09 ~ 2010-01-26 by dhjin, ���Ǵ�Ƽ - �ڵ� ��� 
			procRes = Process_FI_PARTY_AUTO_CREATE_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_PARTY_DISCONNECT_LEAVE_OK:				// 2009-09-09 ~ 2010-01-27 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� ��Ƽ���� Ż��ó��!
			procRes = Process_FI_PARTY_DISCONNECT_LEAVE_OK(pPacket, nLength, nBytesUsed);
			break;

		case T_FI_CHAT_MAP:
			procRes = Process_FI_CHAT_MAP(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_REGION:
			procRes = Process_FI_CHAT_REGION(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_CHANGE_CHAT_FLAG:
			procRes = Process_FI_CHAT_CHANGE_CHAT_FLAG(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_CASH_ALL:
			procRes = Process_FI_CHAT_CASH_ALL(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_ARENA:// 2007-05-02 by dhjin
			procRes = Process_FI_CHAT_ARENA(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK:		// 2006-07-19 by cmkwon
			procRes = Process_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_OUTPOST_GUILD:	// 2007-10-06 by cmkwon, ���� ���� ������ ������ ���� ä�� ���� -
			procRes = Process_FI_CHAT_OUTPOST_GUILD(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_INFINITY:	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ä��
			procRes = Process_FI_CHAT_INFINITY(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHAT_CNC:		// 2015-07-20 Future, CnC costs money
			procRes = Process_FI_CHAT_CNC(pPacket, nLength, nBytesUsed);
			break;

		case T_FI_ADMIN_GET_CHARACTER_INFO:
			procRes = Process_FI_ADMIN_GET_CHARACTER_INFO(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_ADMIN_CALL_CHARACTER:
			procRes = Process_FI_ADMIN_CALL_CHARACTER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_ADMIN_MOVETO_CHARACTER:
			procRes = Process_FI_ADMIN_MOVETO_CHARACTER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_ADMIN_CHANGE_WEATHER:
			procRes = Process_FI_ADMIN_CHANGE_WEATHER(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CHARACTER_UPDATE_GUILD_INFO:
			procRes = Process_FI_CHARACTER_UPDATE_GUILD_INFO(pPacket, nLength, nBytesUsed);
			break;
// 2004-05-24 cmkwon, ������
//		case T_FI_GET_FIELD_USER_COUNTS:
//			procRes = Process_FI_GET_FIELD_USER_COUNTS(pPacket, nLength, nBytesUsed);
//			break;
		case T_FI_GUILD_NOTIFY_END_WAR:
			procRes = Process_FI_GUILD_NOTIFY_END_WAR(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_GUILD_REG_DELETE_GUILD:
			procRes = Process_FI_GUILD_REG_DELETE_GUILD(pPacket, nLength, nBytesUsed);
			break;
// 2005-07-27 by cmkwon, �ٸ� �ʵ弭������ ������ �����Ƿ� ������
//		case T_FI_EVENT_NOTIFY_WARP_OK:
//			procRes = Process_FI_EVENT_NOTIFY_WARP_OK(pPacket, nLength, nBytesUsed);
//			break;

// 2004-12-16 by cmkwon, �ٸ� �ʵ弭������ ������ �����Ƿ� ������
//		case T_FI_EVENT_GET_WARP_INFO_OK:
//			procRes = Process_FI_EVENT_GET_WARP_INFO_OK(pPacket, nLength, nBytesUsed);
//			break;
		case T_FI_ADMIN_PRINT_DEBUG_MSG:
			{
// 2008-02-14 by cmkwon, ó�� ��� �������� �ּ� ó����
// 				if (!m_pFieldIOCP4->m_mapCharacterUniqueNumber.empty())
// 				{
// 					CFieldIOCPSocket *pSocket
// 						= m_pFieldIOCP4->m_mapCharacterUniqueNumber.begin()->second;
// 
// 					char *dpm = NULL;
// 					pSocket->PRINT_DEBUG_MESSAGE(dpm);
// 				}
// 				else
// 				{
// 					// ����ڰ� �ƹ��� ����.
// 					char szTime[128];
// 					time_t ltime;
// 					struct tm *today = NULL;
// 					time(&ltime);
// 					today = localtime(&ltime);
// 					strftime(szTime, 128, "[%m-%d %H:%M:%S]", today );
// 					DBGOUT("\n[Party Info] - %s\n", szTime);
// 					DBGOUT("[Character Info] - %s\n", szTime);
// 					DBGOUT("\n");
// 				}
			}
			procRes = RES_RETURN_TRUE;
			break;

		case T_FI_GUILD_RANK_OK:		// 2006-09-26 by dhjin
			procRes = Process_FI_GUILD_RANK_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_GUILD_DELETE_INFO_OK:		// 2006-09-28 by dhjin
			procRes = Process_FI_GUILD_DELETE_INFO_OK(pPacket, nLength, nBytesUsed);
			break;
		case T_FI_CREATE_GUILD_BY_SUBLEADER:		// 2007-10-06 by dhjin
			procRes = Process_FI_CREATE_GUILD_BY_SUBLEADER(pPacket, nLength, nBytesUsed);
			break;	
			// 2012-10-17 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
		case T_FI_GUILD_BONUS_EXP_RATE:
			procRes = Process_FI_GUILD_BOUNUEXP_RATE(pPacket , nLength , nBytesUsed);
			break;
			// end 2012-10-17 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
		case T_ERROR:
			procRes = Process_ERROR(pPacket, nLength, nBytesUsed);
			break;
		default:
			{
				// check: unknown message type --> error!
				DBGOUT("[Error]Unknown Message Type from IM Server: %s(%#04X), prev. msg: %s(%#04X)\n", GetProtocolTypeString(nType), nType, GetProtocolTypeString(prevMsgType), prevMsgType);
				return TRUE;
			}
		} // end - switch

		g_pFieldGlobal->m_dwLastSocketMessage = 0;

		// ���� ������ ���� �޼��� ����
		prevMsgType = nType;

		// MSG ��� ó��
		if (procRes == RES_BREAK)
		{
			// ����� ������. ������ ���� �ʴ´�.
			// do nothing, �׳� �����ִ� ���� packet�� ó���Ѵ�
		}
		else if (procRes == RES_PACKET_ERROR)
		{
			// ��Ŷ�� �ջ�� ���. ���� packet�� ó������ �ʰ� �ٷ� �����Ѵ�. ������ ���ῡ�� ���.
			return TRUE;
		}
		else if (procRes == RES_RETURN_TRUE)
		{
			// ����
			// do nothing, �׳� �����ִ� ���� packet�� ó���Ѵ�
		}
		else if (procRes == RES_RETURN_FALSE)
		{
			return FALSE;
		}

	} // end - while


	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CONNECT_PREPARE_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2007-08-27 by cmkwon, �����ٿ��غ� ��ɾ� �߰�(SCAdminTool���� SCMonitor�� PrepareShutdown�� ���� �� �� �ְ�)
/// \author		cmkwon
/// \date		2007-08-27 ~ 2007-08-27
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CONNECT_PREPARE_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_CONNECT_PREPARE_SHUTDOWN,
										MSG_FI_CONNECT_PREPARE_SHUTDOWN, pRMsg);

	m_pFieldIOCP4->SetPrepareShutdown(pRMsg->bPrepareShutdown);	
	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_PARTY_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_CREATE_OK,
										MSG_FI_PARTY_CREATE_OK, pMsgPartyCreateOK);

	CFieldIOCPSocket *pFieldIOCPSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgPartyCreateOK->CharacterUniqueNumber);
	if (NULL == pFieldIOCPSocket || !pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		return RES_BREAK;
	}
// 2007-06-27 by cmkwon, �ҽ� ���� - �Ʒ��� ���� ����
//	pFieldIOCPSocket->m_pFieldParty = new CFieldParty(m_pFieldIOCP4, pMsgPartyCreateOK->PartyID, pMsgPartyCreateOK->CharacterUniqueNumber);
//	pFieldIOCPSocket->m_pFieldParty->InsertMember(pFieldIOCPSocket);
//	pFieldIOCPSocket->m_pFieldParty->m_FieldPartyInfo = pMsgPartyCreateOK->FieldPartyInfo;
	CFieldParty * pFParty = new CFieldParty(m_pFieldIOCP4, pMsgPartyCreateOK->PartyID, pMsgPartyCreateOK->CharacterUniqueNumber);
	
	pFParty->SetExpDistributeType(pMsgPartyCreateOK->ExpDistributeType);
	pFParty->SetItemDistributeType(pMsgPartyCreateOK->ItemDistributeType);
	pFParty->m_FieldPartyInfo			= pMsgPartyCreateOK->FieldPartyInfo;
	pFParty->InsertMember(pFieldIOCPSocket);
	pFieldIOCPSocket->m_pFieldParty		= pFParty;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - T_FI_PARTY_CREATE_OK, ��Ƽ ������ ó��
	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ ������ ó��, ���� ����
	vectCFISockList.push_back(pFieldIOCPSocket);	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ ������ ó��, ���� �߰�
	pFieldIOCPSocket->ArrangePartyMemberList(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ ������ ó��, ���� ��Ƽ�� ����Ʈ ����

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( NULL != pFParty && NULL != pFieldIOCPSocket )		//	2013-07-07 by bckim, ���� ����� ���� 
	{
	pFParty->SetReturnUserBuffSkill(pFieldIOCPSocket); // ��Ƽ������ ArrangePartyMemberList
	}
#endif

	// ��Ƽ���� list�� �߰�
	m_pFieldIOCP4->m_MapParties.insertLock(pMsgPartyCreateOK->PartyID, pFParty);

	INIT_MSG_WITH_BUFFER(MSG_FC_PARTY_CREATE_OK, T_FC_PARTY_CREATE_OK, pCreateOK, pCreateOKBuf);
	pCreateOK->PartyID = pMsgPartyCreateOK->PartyID;
	pFieldIOCPSocket->SendAddData(pCreateOKBuf, MSG_SIZE(MSG_FC_PARTY_CREATE_OK));

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_ACCEPT_INVITE_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_ACCEPT_INVITE_OK,
										MSG_FI_PARTY_ACCEPT_INVITE_OK, pMsgAcceptOK);

	vectCFieldIOCPSocket vectCFISockList;	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - T_FI_PARTY_ACCEPT_INVITE_OK, ��Ƽ �ʴ� ���ν� ó��

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);
	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgAcceptOK->PartyID);

	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_ACCEPT_INVITE_OK, ERR_PROTOCOL_NO_SUCH_PARTY, LODWORD(pMsgAcceptOK->PartyID), HIDWORD(pMsgAcceptOK->PartyID));
		return RES_BREAK;
	}

	// lock m_ListPartyMember
	mt_auto_lock pmlock(&pParty->m_ListPartyMember);

	// ��Ƽ ���� ����
	pParty->m_FieldPartyInfo = pMsgAcceptOK->FieldPartyInfo;

	CFieldIOCPSocket *pInviteeSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgAcceptOK->CharacterUniqueNumber);
	if (pInviteeSocket && pInviteeSocket->IsValidCharacter(FALSE))
	{
		// ���� �ʵ� ������ �ִ� ���
		pParty->InsertMember(pInviteeSocket);
		pInviteeSocket->m_pFieldParty = pParty;

		pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - T_FI_PARTY_ACCEPT_INVITE_OK, ��Ƽ �ʴ� ���ν� ó��
	}
	else
	{
		// check: fatal error, ����� ������ ���� �ʵ� �������� �� �޼����� ���� �� �ȴ�.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, STRERR_S_F2NOTIFY_0018);
		return RES_BREAK;
	}
	pmlock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� -
	pslock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� -

	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - T_FI_PARTY_ACCEPT_INVITE_OK, ��Ƽ �ʴ� ���ν� ó��

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( NULL != pParty && NULL != pInviteeSocket)					//	2013-07-07 by bckim, ���� ����� ���� 
	{
	pParty->SetReturnUserBuffSkill(pInviteeSocket); // ��Ƽ �ʴ� ���ν� ArrangeAllPartyMember
	}
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_LEAVE_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_LEAVE_OK,
										MSG_FI_PARTY_LEAVE_OK, pLeaveOK);

	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ Ż��� ó�� 

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	// ��Ƽ���� character ����
	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pLeaveOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_LEAVE_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pLeaveOK->PartyID);
		return RES_BREAK;
	}

	// lock, ����: �߰��� return �ϸ� unlock���ֱ�!
	pParty->m_ListPartyMember.lock();

	// character�� party ���� ����
	CFieldIOCPSocket *pFieldIOCPSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pLeaveOK->CharacterUniqueNumber);
	if (pFieldIOCPSocket == NULL || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{
		SendErrorMessage(T_FI_PARTY_LEAVE_OK, ERR_COMMON_NO_SUCH_CHARACTER);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	// ����� ���̸� ó��
	if (pParty->m_PeerBattlePartyID != NULL)
	{
		pFieldIOCPSocket->OnPartyBattleMemberDead(TRUE);
	}

	// OnPartyBattleMemeberDead()���� m_pFieldParty�� ����ϹǷ�,
	// ����� ó�� ���Ŀ� ���� �ʱ�ȭ��
	pFieldIOCPSocket->m_pFieldParty = NULL;

	FieldPartyMember pm;
	if (FALSE == pParty->DeleteByCharacterUniqueNumber(pLeaveOK->CharacterUniqueNumber, pm))
	{
		SendErrorMessage(T_FI_PARTY_LEAVE_OK, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, pLeaveOK->CharacterUniqueNumber);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ Ż��� ó��

//#ifdef _DEBUG
//	char *dpm = NULL;
//	pFieldIOCPSocket->PRINT_DEBUG_MESSAGE(dpm);
//#endif

	// ��Ƽ ���� ����
	pParty->m_FieldPartyInfo = pLeaveOK->FieldPartyInfo;

	// �� FieldServer ���� ��Ƽ���� �������� ����
	if (pParty->empty())
	{
		m_pFieldIOCP4->m_MapParties.deleteLock(pParty->m_PartyID);
		// unlock
		pParty->m_ListPartyMember.unlock();

		pslock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, ���� ������ ����
		m_pFieldIOCP4->DeleteFieldParty(pParty);
		pParty = NULL;
	}
	else
	{
		// unlock
		pParty->m_ListPartyMember.unlock();
	}
	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ Ż��� ó��

	pFieldIOCPSocket->m_bMissionMaster = FALSE;		// 2008-12-09 by dhjin, �̼Ǹ����� - �ʱ�ȭ�Ѵ�.

	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ Ż��� ó��

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( NULL != pParty && NULL != pFieldIOCPSocket )	//	2013-07-07 by bckim, ���� ����� ���� 
	{
	pParty->SetReturnUserBuffSkill(pFieldIOCPSocket); // ��Ƽ Ż��� ArrangeAllPartyMember
	}	
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_TRANSFER_MASTER_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_TRANSFER_MASTER_OK,
										MSG_FI_PARTY_TRANSFER_MASTER_OK, pTransOK);

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pTransOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_TRANSFER_MASTER_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pTransOK->PartyID);
		return RES_BREAK;
	}

	pParty->m_ListPartyMember.lock();
	// �� ������ �Ҵ�
	pParty->m_MasterUniqueNumber = pTransOK->NewMasterCharacterUniqueNumber;
	pParty->m_ListPartyMember.unlock();

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( NULL != pParty )							//	2013-07-07 by bckim, ���� ����� ���� 
	{
	CFieldIOCPSocket *pFieldIOCPSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pParty->m_MasterUniqueNumber);
		if( NULL != pFieldIOCPSocket)				//	2013-07-07 by bckim, ���� ����� ���� 
		{	
	pParty->SetReturnUserBuffSkill(pFieldIOCPSocket);	// ����� ����� 
		}
	}
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_BAN_MEMBER_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_BAN_MEMBER_OK,
										MSG_FI_PARTY_BAN_MEMBER_OK, pBanOK);

	vectCFieldIOCPSocket vectCFISockList;	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - �߹�� ó��, ���� ����

	// character�� party ���� ����
	CFieldIOCPSocket *pFieldIOCPSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pBanOK->CharacterUniqueNumber);
	if (pFieldIOCPSocket == NULL || FALSE == pFieldIOCPSocket->IsValidCharacter(FALSE))
	{// 2008-12-30 by cmkwon, üũ �߰�
		// ĳ���Ͱ� ���ӿ��� ���� ����� NULL�� �� ����, do nothing
//		SendErrorMessage(T_FI_PARTY_BAN_MEMBER_OK, ERR_COMMON_NO_SUCH_CHARACTER, pBanOK->CharacterUniqueNumber);
//		return RES_BREAK;
	}
	else
	{
		pFieldIOCPSocket->m_pFieldParty = NULL;
	}

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pBanOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_BAN_MEMBER_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pBanOK->PartyID);
		return RES_BREAK;
	}

	// ��Ƽ ���� ����
	pParty->m_FieldPartyInfo = pBanOK->FieldPartyInfo;

	///////////////////////////////////////////////////////////////////////////
	// lock, ����: �߰��� return �ϸ� unlock���ֱ�!
	pParty->m_ListPartyMember.lock();

	FieldPartyMember ret;
	if (FALSE == pParty->DeleteByCharacterUniqueNumber(pBanOK->CharacterUniqueNumber, ret))
	{
		SendErrorMessage(T_FI_PARTY_BAN_MEMBER_OK, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, pBanOK->CharacterUniqueNumber);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - �߹�� ó��, ��Ƽ�� ��������

	// �� FieldServer ���� ��Ƽ���� �������� ����
	if (pParty->empty())
	{
		m_pFieldIOCP4->m_MapParties.deleteLock(pParty->m_PartyID);
		// unlock
		pParty->m_ListPartyMember.unlock();

		pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, ���� ������ ����
		m_pFieldIOCP4->DeleteFieldParty(pParty);
		pParty = NULL;
	}
	else
	{
		// unlock
		pParty->m_ListPartyMember.unlock();
	}
	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - 

	if(pFieldIOCPSocket && pFieldIOCPSocket->IsValidCharacter(FALSE))
	{// 2008-12-30 by cmkwon, ���� ����, üũ �ؾ� �� ������ ����
		pFieldIOCPSocket->m_bMissionMaster = FALSE;		// 2008-12-09 by dhjin, �̼Ǹ����� - �ʱ�ȭ�Ѵ�.

		pFieldIOCPSocket->CleanPartyMemberList();					// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - �߹�� ó��, ���� ��Ƽ�� ����Ʈ �ʱ�ȭ
	}

	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - �߹�� ó��, ��Ƽ�� ����Ʈ ����

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( NULL != pParty && NULL != pFieldIOCPSocket)			// 2013-07-07 by bckim, ���� ����� ����
	{
	pParty->SetReturnUserBuffSkill(pFieldIOCPSocket);	// ��Ƽ�� �߹��  ArrangeAllPartyMember
	}	
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_DISMEMBER_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_DISMEMBER_OK,
										MSG_FI_PARTY_DISMEMBER_OK, pDismemberOK);

	vectCFieldIOCPSocket vectCFISockList;	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ ��ü�� ó��

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pDismemberOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_DISMEMBER_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pDismemberOK->PartyID);
		return RES_BREAK;
	}

	///////////////////////////////////////////////////////////////////////////
	// lock, ����: �߰��� return �ϸ� unlock���ֱ�!
	pParty->m_ListPartyMember.lock();

	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ ��ü�� ó��

	// ����� ���̸� ó��
	if (pParty->m_PeerBattlePartyID != NULL && pParty->m_PartyBattle.IsUsing())
	{
		// ���� ��� �ִ� ��Ƽ������ ��� lossWaiter�� ó��
		CFieldPartyBattle *pTmpBattle = &pParty->m_PartyBattle;
		pTmpBattle->m_setLiveMember.lock();		// lock
		pTmpBattle->m_setLossWaiters.lock();	// lock

		mtsetUID32::iterator itrLiveMember = pTmpBattle->m_setLiveMember.begin();
		while (itrLiveMember != pTmpBattle->m_setLiveMember.end())
		{
			pTmpBattle->m_setLossWaiters.insertLock(*itrLiveMember);
			itrLiveMember++;
		}
		pTmpBattle->m_setLiveMember.clearLock();

		pTmpBattle->m_setLossWaiters.unlock();	// unlock
		pTmpBattle->m_setLiveMember.unlock();	// unlock

		// ��Ƽ�� ��� ó��
		CFieldIOCPSocket::OnPartyBattleEnd(pParty->m_PartyBattle.m_pPeerParty, pParty);
	}

	mtlistFieldPartyMember::iterator itr = pParty->m_ListPartyMember.begin();
	while(itr != pParty->m_ListPartyMember.end())
	{
		CFieldIOCPSocket *pSocket = itr->PM_CharacterSocket;
		if (pSocket != NULL)
		{
			pSocket->m_pFieldParty = NULL;
			pSocket->m_bMissionMaster = FALSE;		// 2008-12-09 by dhjin, �̼Ǹ����� - �ʱ�ȭ�Ѵ�.
		}
		itr++;
	}

	// ��Ƽ ����
	m_pFieldIOCP4->m_MapParties.deleteLock(pDismemberOK->PartyID);

	// unlock
	pParty->m_ListPartyMember.unlock();

	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, ���� ������ ����
	m_pFieldIOCP4->DeleteFieldParty(pParty);
	pParty = NULL;

	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ ��ü�� ó��
	if(false == vectCFISockList.empty())
	{
		vectCFieldIOCPSocket::iterator itr(vectCFISockList.begin());
		for(; itr != vectCFISockList.end(); itr++)
		{
			CFieldIOCPSocket *pFISoc = *itr;
			if(pFISoc && pFISoc->IsValidCharacter(FALSE))
			{
				pFISoc->CleanPartyMemberList();
			}
		}
	}

	return RES_RETURN_TRUE;
}

// 2008-12-30 by cmkwon, �ű� ��Ƽ�� �߰���, ���� ��Ƽ���� ���� ���� �� �ٽ� ���ӽ�
ProcessResult CIMWinSocket::Process_FI_PARTY_ADD_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_ADD_MEMBER,
										MSG_FI_PARTY_ADD_MEMBER, pMsgAddMember);

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgAddMember->CharacterUniqueNumber);
	if (NULL == pCharacterSocket
		|| FALSE == pCharacterSocket->IsValidCharacter(FALSE))
	{// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - üũ�� ���� ó�� �Ѵ�.
		// check: fatal error, ����� ������ ���� �ʵ� �������� �� �޼����� ���� �� �ȴ�.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, STRERR_S_F2NOTIFY_0018);
		return RES_BREAK;
	}

	vectCFieldIOCPSocket vectCFISockList;	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - T_FI_PARTY_ADD_MEMBER, �ű� ��Ƽ�� �߰���, ���� ��Ƽ���� ���� ���� �� �ٽ� ���ӽ� ó��
	
	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgAddMember->PartyID);

	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_ADD_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY, LODWORD(pMsgAddMember->PartyID), HIDWORD(pMsgAddMember->PartyID));
		return RES_BREAK;
	}

	// lock m_ListPartyMember
	mt_auto_lock pmlock(&pParty->m_ListPartyMember);
	if (pCharacterSocket
		&& pCharacterSocket->IsValidCharacter(FALSE))
	{// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - �ѹ��� üũ �Ѵ�.
		// ���� �ʵ� ������ �ִ� ���
		pParty->InsertMember(pCharacterSocket);
		pCharacterSocket->m_pFieldParty = pParty;
		pParty->m_FieldPartyInfo = pMsgAddMember->FieldPartyInfo;

		pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - T_FI_PARTY_ADD_MEMBER, �ű� ��Ƽ�� �߰���, ���� ��Ƽ���� ���� ���� �� �ٽ� ���ӽ� ó��

		if(pParty->m_PeerBattleRequesterPartyID
			&& pParty->m_PartyBattleMapChannelIndex == pCharacterSocket->m_character.MapChannelIndex)
		{// �������̰� �������� �ʿ��� �߰��ɶ�(������ ����â���� �����ٰ� ���°��)

			// ���� ��� ��Ƽ�� ������ ����
			CFieldParty *pPeerParty = m_pFieldIOCP4->m_MapParties.findLock(pParty->m_PeerBattlePartyID);
			if (pPeerParty
				&& pParty->m_PartyID == pPeerParty->m_PeerBattleRequesterPartyID)
			{
				// ����� ��Ƽ�� üũ
				CFieldIOCPSocket *pPeerMasterSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pPeerParty->GetMasterCharacterUniqueNumber());
				if (pPeerMasterSocket
					&& pPeerMasterSocket->IsUsing())
				{
					int i = 0;
					CFieldIOCPSocket *ArrayPartyMember[SIZE_MAX_PARTY_MEMBER];
					util::zero(ArrayPartyMember, sizeof(ArrayPartyMember[0]) * SIZE_MAX_PARTY_MEMBER);
					INIT_MSG_WITH_BUFFER(MSG_FC_PARTY_BATTLE_START, T_FC_PARTY_BATTLE_START, pSMsg, pSendBuf);
					
					// Send MSG to Target(this) Party
					pSMsg->PeerPartyID					= pParty->m_PeerBattlePartyID;
					pSMsg->PeerPartyMasterClientIndex	= pPeerMasterSocket->m_character.ClientIndex;
					pSMsg->nPeerPartyMemberToBattle		= 0;
					int nMemCount = pPeerParty->GetMembersInSameMap(ArrayPartyMember, pCharacterSocket->GetCurrentFieldMapChannel());
					nMemCount = min(nMemCount, SIZE_MAX_PARTY_MEMBER);
					PEER_PARTY_MEMBER *pMember = (PEER_PARTY_MEMBER*)(pSendBuf + MSG_SIZE(MSG_FC_PARTY_BATTLE_START));
					for (i = 0; i < nMemCount; i++)
					{
						if(ArrayPartyMember[i] && ArrayPartyMember[i]->IsUsing())
						{
							pMember->ClientIndex			= ArrayPartyMember[i]->m_character.ClientIndex;
							pMember->CharacterUniqueNumber	= ArrayPartyMember[i]->m_character.CharacterUniqueNumber;
							pSMsg->nPeerPartyMemberToBattle++;
							pMember++;
						}
					}
					pCharacterSocket->SendAddData(pSendBuf
						, MSG_SIZE(MSG_FC_PARTY_BATTLE_START) + pSMsg->nPeerPartyMemberToBattle*sizeof(PEER_PARTY_MEMBER));
				}
			}
		}
	}
	else
	{
		// check: fatal error, ����� ������ ���� �ʵ� �������� �� �޼����� ���� �� �ȴ�.
		g_pFieldGlobal->WriteSystemLogEX(TRUE, STRERR_S_F2NOTIFY_0018);
		return RES_BREAK;
	}
	pmlock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - 
	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - 

	
	///////////////////////////////////////////////////////////////////////////////
	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - T_FI_PARTY_ADD_MEMBER, �ű� ��Ƽ�� �߰���, ���� ��Ƽ���� ���� ���� �� �ٽ� ���ӽ� ó��
	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( NULL != pParty && NULL != pCharacterSocket )	//	2013-07-07 by bckim, ���� ����� ���� 
	{
	pParty->SetReturnUserBuffSkill(pCharacterSocket);   // ��� ��� �߰��� ArrangeAllPartyMember
	}
#endif

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_PARTY_DELETE_MEMBER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_DELETE_MEMBER,
										MSG_FI_PARTY_DELETE_MEMBER, pDelMember);

	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ�� ���� �����, ���� ����

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pDelMember->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_DELETE_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY, pDelMember->PartyID);
		return RES_BREAK;
	}


	/////////////////////////////////////////////////////////////////
	// ��Ƽ���� character ����
	/////////////////////////////////////////////////////////////////

	// lock, ����: �߰��� return �ϸ� unlock���ֱ�!
	pParty->m_ListPartyMember.lock();

	// ��Ƽ ���� ������Ʈ
	pParty->m_FieldPartyInfo = pDelMember->FieldPartyInfo;

	// character�� party ���� ����
	CFieldIOCPSocket *pFieldIOCPSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pDelMember->CharacterUniqueNumber);
	if (pFieldIOCPSocket == NULL)
	{
		SendErrorMessage(T_FI_PARTY_DELETE_MEMBER, ERR_COMMON_NO_SUCH_CHARACTER);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	pFieldIOCPSocket->m_pFieldParty = NULL;

	FieldPartyMember pm;
	if (FALSE == pParty->DeleteByCharacterUniqueNumber(pDelMember->CharacterUniqueNumber, pm))
	{// ��Ƽ���� ������ ��Ƽ���� �����Ѵ�

		SendErrorMessage(T_FI_PARTY_DELETE_MEMBER, ERR_PROTOCOL_NO_SUCH_PARTY_MEMBER, pDelMember->CharacterUniqueNumber);

		// unlock
		pParty->m_ListPartyMember.unlock();

		return RES_BREAK;
	}

	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ�� ���� �����, ��Ƽ�� ��������

	if (pParty->empty())
	{// ��Ƽ�� ��Ƽ���� �ƹ��� ������ ��Ƽ�� �����Ѵ�

		m_pFieldIOCP4->m_MapParties.deleteLock(pParty->m_PartyID);

		// unlock
		pParty->m_ListPartyMember.unlock();

		pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, ���� ������ ����
		m_pFieldIOCP4->DeleteFieldParty(pParty);
		pParty = NULL;
	}
	else
	{
		// unlock
		pParty->m_ListPartyMember.unlock();
	}

	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - 

	pFieldIOCPSocket->m_bMissionMaster = FALSE;		// 2008-12-09 by dhjin, �̼Ǹ����� - �ʱ�ȭ�Ѵ�.

	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ�� ���� �����, ��Ƽ�� ����Ʈ ����

	// 2013-07-06 by hskim, ���� �״� ���� ����
#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( NULL != pParty && NULL != pFieldIOCPSocket)			//	2013-07-07 by bckim, ���� ����� ���� 
	{
		pParty->SetReturnUserBuffSkill(pFieldIOCPSocket);	// ��Ƽ�� ��������� 	ArrangeAllPartyMember
	}
#endif
	// end 2013-07-06 by hskim, ���� �״� ���� ����

	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_PARTY_UPDATE_PARTY_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_UPDATE_PARTY_INFO,
										MSG_FI_PARTY_UPDATE_PARTY_INFO, pMsgUpdateInfo);

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgUpdateInfo->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_UPDATE_PARTY_INFO, ERR_PROTOCOL_NO_SUCH_PARTY, pMsgUpdateInfo->PartyID);
		return RES_BREAK;
	}

	// ���� ����
	pParty->m_FieldPartyInfo = pMsgUpdateInfo->FieldPartyInfo;

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 ��� ���� - ����ġ �й� ��� ���� 
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE,
										MSG_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE, pMsgUpdateInfo);

	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgUpdateInfo->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_CHANGE_EXP_DISTRIBUTE_TYPE, ERR_PROTOCOL_NO_SUCH_PARTY, pMsgUpdateInfo->PartyID);
		return RES_BREAK;
	}

	// ���� ����
	pParty->SetExpDistributeType(pMsgUpdateInfo->ExpDistributeType);

	return RES_RETURN_TRUE;


}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		EP3 ��� ���� - ������ �й� ��� ����
/// \author		dhjin
/// \date		2008-06-04 ~ 2008-06-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE,
										MSG_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE, pMsgUpdateInfo);
	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);

	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pMsgUpdateInfo->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_CHANGE_ITEM_DISTRIBUTE_TYPE, ERR_PROTOCOL_NO_SUCH_PARTY, pMsgUpdateInfo->PartyID);
		return RES_BREAK;
	}

	// ���� ����
	pParty->SetItemDistributeType(pMsgUpdateInfo->ItemDistributeType);

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_FORMATION_SKILL(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - 
/// \author		cmkwon
/// \date		2009-08-04 ~ 2009-08-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_CHANGE_FORMATION_SKILL(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_CHANGE_FORMATION_SKILL,
										MSG_FI_PARTY_CHANGE_FORMATION_SKILL, pRecvMsg);

// 2009-11-17 by cmkwon, ��� ���� ��ų ���� - 
//	return RES_BREAK;		// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - �ӽ÷� ����� ���� �д�.

	CFieldIOCPSocket *pPartyMasterFISoc = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRecvMsg->PartyMasterCharcUID);

	if(NULL == pPartyMasterFISoc
		|| FALSE == pPartyMasterFISoc->IsValidCharacter(FALSE))
	{// 2009-08-04 by cmkwon, �ش� ��Ƽ���� ��ȿ���� ����
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] PFS error(CIMWinSocket::Process_FI_PARTY_CHANGE_FORMATION_SKILL#) 11000 !! PartyMasterCharcUID(%ld) \r\n"
			, pRecvMsg->PartyMasterCharcUID);
		return RES_BREAK;
	}

	vectCFieldIOCPSocket vectCFISockList;
	if(FALSE == pPartyMasterFISoc->GetPartyMemberList(&vectCFISockList))
	{// 2009-08-04 by cmkwon, ��Ƽ�� ����Ʈ�� ��� ����
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] PFS error(CIMWinSocket::Process_FI_PARTY_CHANGE_FORMATION_SKILL#) 12000 !! %s %d \r\n"
			, GetCharacterString(pPartyMasterFISoc->GetCharacter(), string()), vectCFISockList.size());
		return RES_BREAK;
	}

	pPartyMasterFISoc->ChangePartyFormationSkill(&vectCFISockList, pRecvMsg->Is_On_or_Off, pRecvMsg->FlightFormation);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_AUTO_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		���Ǵ�Ƽ - �ڵ� ��� 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_AUTO_CREATE_OK(const char* pPacket, int nLength, int &nBytesUsed) {
	int							nRecvTypeSize;
	MSG_FI_PARTY_AUTO_CREATE_OK	* pRecvAutoCreateOK = NULL;
	nRecvTypeSize = sizeof(MSG_FI_PARTY_AUTO_CREATE_OK);
	if(nLength - nBytesUsed < nRecvTypeSize) {
		// Protocl Error ó��
		// - Client�� ���� ���� Data Size�� Field Type�� ���� Data Size���� �۴�
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FI_PARTY_AUTO_CREATE_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_BREAK;
	}
	pRecvAutoCreateOK = (MSG_FI_PARTY_AUTO_CREATE_OK*)(pPacket+nBytesUsed);	
	nBytesUsed += nRecvTypeSize;
	
	nRecvTypeSize = pRecvAutoCreateOK->PartyMemberListCount*sizeof(UID32_t);
	if(nLength-nBytesUsed < nRecvTypeSize)
	{
		// Protocl Error ó��
		// - Client�� ���� ���� Data Size�� Field Type�� ���� Data Size���� �۴�
		// Error Code : ERR_PROTOCOL_INVALID_FIELD_DATA
		SendErrorMessage(T_FI_PARTY_AUTO_CREATE_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_BREAK;
	}
	UID32_t *pPartyMemberUID = (UID32_t*)(pPacket+nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pMasterFISoc = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pRecvAutoCreateOK->MasterUID);
	if (NULL == pMasterFISoc) {
		return RES_BREAK;
	}
	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);
	// ��Ƽ ����
	CFieldParty * pFParty = new CFieldParty(m_pFieldIOCP4, pRecvAutoCreateOK->PartyID, pRecvAutoCreateOK->MasterUID);
	// lock m_ListPartyMember
	mt_auto_lock pmlock(&pFParty->m_ListPartyMember);
	pFParty->SetExpDistributeType(EXP_DISTRIBUTE_TYPE_DAMAGE);
	pFParty->SetItemDistributeType(ITEM_DISTRIBUTE_TYPE_DAMAGE);
	pFParty->m_FieldPartyInfo			= pRecvAutoCreateOK->FieldPartyInfo;
	pFParty->InsertMember(pMasterFISoc);
	pMasterFISoc->m_pFieldParty		= pFParty;
	// ��Ƽ���� list�� �߰�
	m_pFieldIOCP4->m_MapParties.insertNoLock(pRecvAutoCreateOK->PartyID, pFParty);

	// ��Ƽ�� ó��
	UID32_t * pRecvPartyMemberUID = (UID32_t*)( ((BYTE*)pRecvAutoCreateOK) + sizeof(MSG_FI_PARTY_AUTO_CREATE_OK));
	int PartyMemberListCount = 0;
	for(int i=0; i < pRecvAutoCreateOK->PartyMemberListCount; i++) {
		CFieldIOCPSocket *pRecvMemberFISoc = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pRecvPartyMemberUID[i]);
		if(NULL != pRecvMemberFISoc) {
			pFParty->InsertMember(pRecvMemberFISoc);
			pRecvMemberFISoc->m_pFieldParty = pFParty;		
		}
	}

	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ ������ ó��, ���� ����
	pFParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - T_FI_PARTY_ACCEPT_INVITE_OK, ��Ƽ �ʴ� ���ν� ó��
	
	pmlock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� -
	pslock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� -
	
	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - T_FI_PARTY_ACCEPT_INVITE_OK, ��Ƽ �ʴ� ���ν� ó��

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( NULL != pFParty )							//	2013-07-07 by bckim, ���� ����� ���� 
	{
	pFParty->SetReturnUserBuffSkill(pMasterFISoc);	// ��Ƽ �ڵ� ������ ArrangeAllPartyMember
	}
#endif

	// ��Ƽ������ ����
	INIT_MSG_WITH_BUFFER(MSG_FC_PARTY_AUTO_CREATED, T_FC_PARTY_AUTO_CREATED, pSMsg, pSendBuf);
	pSMsg->PartyID	= pRecvAutoCreateOK->PartyID;
	pFParty->SendMsgToMembers(pSendBuf, MSG_SIZE(MSG_FC_PARTY_AUTO_CREATED));

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_PARTY_DISCONNECT_LEAVE_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		���Ǵ�Ƽ - ���� ���� ���� ��Ƽ���� Ż��ó��! 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_PARTY_DISCONNECT_LEAVE_OK(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_PARTY_DISCONNECT_LEAVE_OK,
		MSG_FI_PARTY_DISCONNECT_LEAVE_OK, pLeaveOK);
	
	vectCFieldIOCPSocket vectCFISockList;			// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ Ż��� ó�� 
	
	// lock m_MapParties
	mt_auto_lock pslock(&m_pFieldIOCP4->m_MapParties);
	
	// ��Ƽ���� character ����
	CFieldParty *pParty
		= m_pFieldIOCP4->m_MapParties.findLock(pLeaveOK->PartyID);
	if (pParty == NULL)
	{
		SendErrorMessage(T_FI_PARTY_DISCONNECT_LEAVE_OK, ERR_PROTOCOL_NO_SUCH_PARTY, pLeaveOK->PartyID);
		return RES_BREAK;
	}
	
	// lock, ����: �߰��� return �ϸ� unlock���ֱ�!
	pParty->m_ListPartyMember.lock();
	
	pParty->GetAllMembers(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ Ż��� ó��

	// ��Ƽ ���� ����
	pParty->m_FieldPartyInfo = pLeaveOK->FieldPartyInfo;
	
	// �� FieldServer ���� ��Ƽ���� �������� ����
	if (pParty->empty())
	{
		m_pFieldIOCP4->m_MapParties.deleteLock(pParty->m_PartyID);
		// unlock
		pParty->m_ListPartyMember.unlock();
		
		pslock.auto_unlock_cancel();	// 2008-12-30 by cmkwon, ���� ������ ����
		m_pFieldIOCP4->DeleteFieldParty(pParty);
		pParty = NULL;
	}
	else
	{
		// unlock
		pParty->m_ListPartyMember.unlock();
	}
	pslock.auto_unlock_cancel();		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ Ż��� ó��	
	
	CFieldIOCPSocket::ArrangeAllPartyMember(&vectCFISockList);		// 2008-12-30 by cmkwon, ��Ƽ�� Move ó�� �ý��� ���� - ��Ƽ Ż��� ó��

#ifdef S_ITEM_EVENT_RETURN_USER_BCKIM				// 2013-02-28 by bckim, �������� �����߰�
	if( NULL != pParty )					//	2013-07-07 by bckim, ���� ����� ���� 
	{
	CFieldIOCPSocket *pFieldIOCPSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pParty->m_MasterUniqueNumber);
		if( NULL != pFieldIOCPSocket )		//	2013-07-07 by bckim, ���� ����� ���� 
		{
	pParty->SetReturnUserBuffSkill(pFieldIOCPSocket);	// ��Ƽ Ż��� // ArrangeAllPartyMember
		}
	}
#endif

	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_CHAT_MAP(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_MAP			*pMsgChatMap = (MSG_FI_CHAT_MAP*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_MAP) + pMsgChatMap->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_MAP, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pMsgChatMap) + sizeof(MSG_FI_CHAT_MAP);

	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChatMap->CharacterUniqueNumber);

	if (NULL == pSocket
		|| FALSE == pSocket->IsValidCharacter(FALSE))
	{
		// check: ������
		return RES_BREAK;
	}
	else if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Map Chat Error, CharUID(%8d)\r\n", pMsgChatMap->CharacterUniqueNumber);
		return RES_BREAK;
	}
	
	if (NULL == pSocket->m_pCurrentFieldMapChannel)
	{
		// check: ������
		return RES_BREAK;
	}

	CFieldMapProject *pMapPro = pSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject;
	if(NULL == pMapPro)
	{
		// check: ������
		return RES_BREAK;
	}

	// MSG_FC_CHAT_MAP ����
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_MAP, T_FC_CHAT_MAP, msgChatMap, msgChatMapBuf);
	util::strncpy(msgChatMap->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	msgChatMap->MessageLength = pMsgChatMap->MessageLength;
	msgChatMap->NameColor = pSocket->GetCharacter()->Color;
	memcpy(msgChatMapBuf + MSG_SIZE(MSG_FC_CHAT_MAP), chatString, pMsgChatMap->MessageLength);

	BYTE byInfluenceTyMask = pSocket->m_character.GetInfluenceMask();
	if(COMPARE_RACE(pSocket->m_character.Race, RACE_GAMEMASTER))
	{// 2005-12-22 by cmkwon, GM�� ä���� ��ο��� ����
		byInfluenceTyMask = INFLUENCE_TYPE_ALL_MASK;
	}

	///////////////////////////////////////////////////////////////////////////////	
	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - CIMWinSocket::Process_FI_CHAT_MAP#, 
	pMapPro->SendMessageToAllInAllMapChannels(msgChatMapBuf, MSG_SIZE(MSG_FC_CHAT_MAP) + pMsgChatMap->MessageLength, byInfluenceTyMask, pSocket->m_character.GetStartCityMapIndex());
	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_CHAT_REGION(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_REGION		*pMsgChatRegion = (MSG_FI_CHAT_REGION*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_REGION) + pMsgChatRegion->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_REGION, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pMsgChatRegion) + sizeof(MSG_FI_CHAT_REGION);
	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChatRegion->CharacterUniqueNumber);

	if (NULL == pSocket
		|| FALSE == pSocket->IsValidCharacter(FALSE))
	{
		// check: ������
		return RES_BREAK;
	}
	
	if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Region Chat Error, CharUID(%8d)\r\n", pMsgChatRegion->CharacterUniqueNumber);
		return RES_BREAK;
	}

	if (strncmp(chatString, "/", 1) == 0)
	{
		// 2007-10-02 by cmkwon, COMMAND �α� �����
		if (g_pFieldGlobal->CheckWriteLog(chatString))		// 2011-06-22 by hskim, �缳 ���� ����
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "[Notify] : %s, IP(%s), command(%s)\r\n", GetCharacterString(&pSocket->m_character, string()), pSocket->GetPeerIP(), chatString);

		// ��ɾ���
		BOOL ret = pSocket->HandleAdminCommands(chatString);
		if (FALSE == ret)
		{
			pSocket->SendString128(STRING_128_ADMIN_CMD, STRMSG_S_F2NOTIFY_0150);
		}
		return RES_BREAK;
	}

	// MSG_FC_CHAT_REGION ����
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_REGION, T_FC_CHAT_REGION, msgChatRegion, msgChatRegionBuf);
	util::strncpy(msgChatRegion->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	msgChatRegion->MessageLength = pMsgChatRegion->MessageLength;
	msgChatRegion->NameColor = pSocket->GetCharacter()->Color;
	memcpy(msgChatRegionBuf + MSG_SIZE(MSG_FC_CHAT_REGION), chatString, pMsgChatRegion->MessageLength);

	int nSendSize = MSG_SIZE(MSG_FC_CHAT_REGION) + pMsgChatRegion->MessageLength;
	BYTE byInfluenceTyMask = pSocket->m_character.GetInfluenceMask();
	if(COMPARE_RACE(pSocket->m_character.Race, RACE_GAMEMASTER))
	{// 2005-12-22 by cmkwon, GM�� ä���� ��ο��� ����
		byInfluenceTyMask = INFLUENCE_TYPE_ALL_MASK;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2009-10-12 by cmkwon, ������ī ���� ��� ���� - CIMWinSocket::Process_FI_CHAT_REGION#, 
	pSocket->SendInRangeMessageAroundMe(msgChatRegionBuf, nSendSize, TRUE, byInfluenceTyMask, pSocket->m_character.GetStartCityMapIndex());
	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_CHAT_CHANGE_CHAT_FLAG(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_CHANGE_CHAT_FLAG	*pMsgOnOff = NULL;

	nRecvTypeSize = sizeof(MSG_FI_CHAT_CHANGE_CHAT_FLAG);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_CHANGE_CHAT_FLAG, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	pMsgOnOff = (MSG_FI_CHAT_CHANGE_CHAT_FLAG*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgOnOff->CharacterUniqueNumber);

	if (pSocket == NULL)
	{
		// check: ������
		return RES_BREAK;
	}
// 2008-05-15 by dhjin, EP3 - ä�� �ý��� ����
//	pSocket->m_enableChatFlag8 = pMsgOnOff->ChatFlag;
	pSocket->m_enableChatFlag16 = pMsgOnOff->ChatFlag;
	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_CHAT_CASH_ALL(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_CASH_ALL	*pRMsgChatCash = (MSG_FI_CHAT_CASH_ALL*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_CASH_ALL) + pRMsgChatCash->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_CASH_ALL, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pRMsgChatCash) + sizeof(MSG_FI_CHAT_CASH_ALL);
	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsgChatCash->CharacterUniqueNumber);

	if (NULL == pSocket
		|| FALSE == pSocket->IsUsing())
	{
		// check: ������
		return RES_BREAK;
	}

	if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Map Chat Error, CharUID(%8d)\r\n", pRMsgChatCash->CharacterUniqueNumber);
		return RES_BREAK;
	}
	
	if (NULL == pSocket->m_pCurrentFieldMapChannel)
	{
		// check: ������
		return RES_BREAK;
	}

	CFieldMapProject *pMapPro = pSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject;
	if(NULL == pMapPro)
	{
		// check: ������
		return RES_BREAK;
	}

	// ����ȭ ���� �߰��ؾ���	
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_CASH_ALL, T_FC_CHAT_CASH_ALL, pSChatCash, BufCash);
	util::strncpy(pSChatCash->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	pSChatCash->MessageLength = pRMsgChatCash->MessageLength;
	pSChatCash->NameColor = pSocket->GetCharacter()->Color;
	memcpy(BufCash+MSG_SIZE(MSG_FC_CHAT_CASH_ALL), chatString, pRMsgChatCash->MessageLength);
	
	// ����� �׽�Ʈ�� �ڽſ��Ը� ����
	pSocket->SendAddData(BufCash, MSG_SIZE(MSG_FC_CHAT_CASH_ALL)+pRMsgChatCash->MessageLength);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CHAT_ARENA(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		�Ʒ��� ä��
/// \author		dhjin
/// \date		2007-05-02 ~ 2007-05-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CHAT_ARENA(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_MAP			*pMsgChatArena = (MSG_FI_CHAT_ARENA*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_ARENA) + pMsgChatArena->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_ARENA, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pMsgChatArena) + sizeof(MSG_FI_CHAT_ARENA);

	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChatArena->CharacterUniqueNumber);

	if (NULL == pSocket
		|| FALSE == pSocket->IsValidCharacter(FALSE)
		|| ARENA_STATE_NONE == pSocket->m_ArenaInfo.State)
	{
		// check: ������
		return RES_BREAK;
	}
	else if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME))
	{
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Map Chat Error, CharUID(%8d)\r\n", pMsgChatArena->CharacterUniqueNumber);
		return RES_BREAK;
	}
	
	if (NULL == pSocket->m_pCurrentFieldMapChannel)
	{
		// check: ������
		return RES_BREAK;
	}

	// MSG_FC_CHAT_ARENA ����
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_ARENA, T_FC_CHAT_ARENA, msgChatArena, msgChatArenaBuf);
	util::strncpy(msgChatArena->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	msgChatArena->MessageLength = pMsgChatArena->MessageLength;
	msgChatArena->NameColor = pSocket->GetCharacter()->Color;
	memcpy(msgChatArenaBuf + MSG_SIZE(MSG_FC_CHAT_ARENA), chatString, msgChatArena->MessageLength);
	int nSendSize = MSG_SIZE(MSG_FC_CHAT_ARENA) + pMsgChatArena->MessageLength;

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-02 by dhjin, ���� �Ʒ��� ������ �޼��� ����
	pSocket->ms_pFieldIOCP->m_ArenaManager.SendTeamChat(msgChatArenaBuf, nSendSize, pSocket->m_ArenaInfo.TeamNum);
	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2006-07-19 ~ 2006-07-19
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK	*pRMsg = (MSG_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_CASH_ALL);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_ONLINE_EACHOTHER_FRIEND_COUNT_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pFISock = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsg->CharacterUniqueNumber);

	if (NULL == pFISock
		|| FALSE == pFISock->IsValidCharacter(FALSE))
	{// check: ������

		return RES_BREAK;
	}
// 2006-07-19 by cmkwon
//	DBGOUT("	060718 ==> CharInfo(%20s:%8d) OnlineEachOtherFriendCount(%d)\r\n"
//		, pFISock->GetCharacter()->CharacterName, pFISock->GetCharacter()->CharacterUniqueNumber
//		, pRMsg->OnlineFriendCnts);

	pFISock->m_nOnlineEachOtherFriendCnts = min(SIZE_MAX_FRIENDLIST_COUNT, pRMsg->OnlineFriendCnts);	// 2006-07-19 by cmkwon, ���� ����� �¶��� ģ�� ī��Ʈ
	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
#if S_BONUSEXPSYSTEM_RENEWAL || S_BONUS_KILL_SYSTEM_RENEWAL
	pFISock->SendToClient_BonusExpRate(); // ���ʽ� ����ġ ���� ����
#endif // S_BONUSEXPSYSTEM_RENEWAL
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CHAT_OUTPOST_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2007-10-06 by cmkwon, ���� ���� ������ ������ ���� ä�� ���� -
/// \author		cmkwon
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CHAT_OUTPOST_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FI_CHAT_OUTPOST_GUILD	*pMsgChat = (MSG_FI_CHAT_OUTPOST_GUILD*)(pPacket + nBytesUsed);

	nRecvTypeSize = sizeof(MSG_FI_CHAT_OUTPOST_GUILD) + pMsgChat->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_OUTPOST_GUILD, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	char *chatString = ((char*)pMsgChat) + sizeof(MSG_FI_CHAT_OUTPOST_GUILD);

	CFieldIOCPSocket *pFISoc = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChat->CharacterUniqueNumber);
	if (NULL == pFISoc
		|| FALSE == pFISoc->IsValidCharacter(FALSE))
	{
		// check: ������
		return RES_BREAK;
	}

	if(FALSE == COMPARE_INFLUENCE(pFISoc->m_character.InfluenceType, INFLUENCE_TYPE_VCN|INFLUENCE_TYPE_ANI))
	{// 2007-10-06 by cmkwon, �⺻ ���� üũ
		return RES_BREAK;
	}

	if(FALSE == IS_VALID_UNIQUE_NUMBER(pFISoc->GetCharacter()->GuildUniqueNumber))
	{// 2007-10-06 by cmkwon, ���� ���� ���� üũ
		return RES_BREAK;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// 2007-10-06 by cmkwon, ���� ���� ������ ������ ���� ä�� ���� - �������� ������ �������� üũ
	int tmOutPostCityMapIndex = m_pFieldIOCP4->m_OutPostManager.GetOutPostCityMapIndexByGuildUID(pFISoc->GetCharacter()->GuildUniqueNumber);
	if(0 >= tmOutPostCityMapIndex)
	{
		return RES_BREAK;
	}

	// 2007-10-06 by cmkwon, �������� ������ ������ üũ
	if(FALSE == m_pFieldIOCP4->m_CityLeader.CheckIsValidCharacter(tmOutPostCityMapIndex, pFISoc->GetCharacter()->CharacterUniqueNumber))
	{
		return RES_BREAK;
	}
	
	auto pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChat->CharacterUniqueNumber);

	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_OUTPOST_GUILD, T_FC_CHAT_OUTPOST_GUILD, pSChat, SendBuf);
	_snprintf(pSChat->FromCharacterName, SIZE_MAX_CHARACTER_NAME-1, STRCMD_OUTPOST_GUILD_MASTER, CAtumSJ::GetMapName(tmOutPostCityMapIndex));	// 2008-06-19 by cmkwon, �������������� ��ü ä�� ���� ����(K0000244) - 
	pSChat->MessageLength	= pMsgChat->MessageLength;
	pSChat->NameColor = pSocket->GetCharacter()->Color;
	memcpy(SendBuf + MSG_SIZE(MSG_FC_CHAT_OUTPOST_GUILD), chatString, pSChat->MessageLength);
	int nSendSize = MSG_SIZE(MSG_FC_CHAT_OUTPOST_GUILD) + pSChat->MessageLength;

	// 2007-10-06 by cmkwon, ���� ���� ��� �������� ����
	m_pFieldIOCP4->SendMessageToAllClients(SendBuf, nSendSize, pFISoc->m_character.InfluenceType);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CHAT_INFINITY(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		���Ǵ�Ƽ - ���� ä��
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CHAT_INFINITY(const char* pPacket, int nLength, int &nBytesUsed) {
	int						nRecvTypeSize = 0;
	MSG_FI_CHAT_MAP			*pMsgChat = (MSG_FI_CHAT_INFINITY*)(pPacket + nBytesUsed);
	
	nRecvTypeSize = sizeof(MSG_FI_CHAT_INFINITY) + pMsgChat->MessageLength;
	if(nLength - nBytesUsed < nRecvTypeSize) {
		SendErrorMessage(T_FI_CHAT_INFINITY, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;
	
	char *chatString = ((char*)pMsgChat) + sizeof(MSG_FI_CHAT_INFINITY);
	
	CFieldIOCPSocket *pSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pMsgChat->CharacterUniqueNumber);
	
	if (NULL == pSocket
		|| FALSE == pSocket->IsValidCharacter(FALSE)
		|| INFINITY_STATE_NONE == pSocket->m_InfinityPlayingInfo.InfinityState) {
		// check: ������
		return RES_BREAK;
	}
	else if (0 == strncmp(pSocket->GetCharacter()->CharacterName, "", SIZE_MAX_CHARACTER_NAME)) {
		g_pFieldGlobal->WriteSystemLogEX(FALSE, "[ERROR] Map Chat Error, CharUID(%8d)\r\n", pMsgChat->CharacterUniqueNumber);
		return RES_BREAK;
	}
	
	if (NULL == pSocket->m_pCurrentFieldMapChannel) {
		// check: ������
		return RES_BREAK;
	}
	
	// MSG_FC_CHAT_INFINITY ����
	INIT_MSG_WITH_BUFFER(MSG_FC_CHAT_INFINITY, T_FC_CHAT_INFINITY, msgChat, msgChatBuf);
	util::strncpy(msgChat->FromCharacterName, pSocket->GetCharacter()->CharacterName, SIZE_MAX_CHARACTER_NAME);
	msgChat->MessageLength = pMsgChat->MessageLength;
	msgChat->NameColor = pSocket->GetCharacter()->Color;
	memcpy(msgChatBuf + MSG_SIZE(MSG_FC_CHAT_INFINITY), chatString, msgChat->MessageLength);
	int nSendSize = MSG_SIZE(MSG_FC_CHAT_INFINITY) + pMsgChat->MessageLength;
	
	//////////////////////////////////////////////////////////////////////////
	// ���� ���Ǵ�Ƽ ������ �޼��� ����
	pSocket->ms_pFieldIOCP->m_InfinityManager.SendInfinityTeamChatW(pSocket->m_InfinityPlayingInfo.ModeTypeNum, pSocket->m_InfinityPlayingInfo.InfinityCreateUID, msgChatBuf, nSendSize);
	
	return RES_RETURN_TRUE;
}

// 2015-07-20 Future, CnC costs money
ProcessResult CIMWinSocket::Process_FI_CHAT_CNC(const char* pPacket, int nLength, int &nBytesUsed)
{
	// Getting Message and received size
	MSG_FI_CHAT_CNC*	recvMsgCnC = (MSG_FI_CHAT_CNC*)(pPacket + nBytesUsed);
	int nRecvTypeSize = sizeof(MSG_FI_CHAT_CNC) + recvMsgCnC->MessageLength;

	// Validating Received Size
	if (nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHAT_CNC, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	nBytesUsed += nRecvTypeSize;

	// Find sending character
	CFieldIOCPSocket* sender = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterName(recvMsgCnC->FromCharacterName);
	if (!sender
		|| !sender->IsValidCharacter())
	{
		g_pFieldGlobal->WriteSystemLogEX(TRUE, "ERROR: Cross Nation Chat Sender not found or not valid. Character name (%s)\r\n", recvMsgCnC->FromCharacterName);
		return RES_BREAK;
	}

	// Lock Inventory
	mt_auto_lock(&sender->m_ItemManager.m_mapItemGeneral);

	// Check if character has enough money
	ITEM_GENERAL* senderMoney = sender->m_ItemManager.GetFirstItemGeneralByItemNum(MONEY_ITEM_NUMBER);
	if (!senderMoney || senderMoney->CurrentCount < _CROSS_NATION_CHAT_SPI_COST)
	{
		sender->SendString128(STRING_128_USER_NOTICE, STRMSG_081230_0004, _CROSS_NATION_CHAT_SPI_COST);
		return RES_BREAK;
	}

	// Remove costs for chatting
	sender->m_ItemManager.UpdateItemCountByPointer(senderMoney, -_CROSS_NATION_CHAT_SPI_COST, IUT_GENERAL);

	// Task IM Server to send the chat
	Write(((BYTE*)recvMsgCnC) - SIZE_FIELD_TYPE_HEADER, MSG_SIZE(MSG_FI_CHAT_CNC) + recvMsgCnC->MessageLength);
}

ProcessResult CIMWinSocket::Process_FI_ADMIN_GET_CHARACTER_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	int								nRecvTypeSize = 0;
	MSG_FI_ADMIN_GET_CHARACTER_INFO	*pMsgAdminGetCharInfo = NULL;

	nRecvTypeSize = sizeof(MSG_FI_ADMIN_GET_CHARACTER_INFO);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_ADMIN_GET_CHARACTER_INFO, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_PACKET_ERROR;
	}
	pMsgAdminGetCharInfo = (MSG_FI_ADMIN_GET_CHARACTER_INFO*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pCharacSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgAdminGetCharInfo->CharacterUniqueNumber);

	if (pCharacSocket == NULL)
	{
		// abnormal error, do nothing
		return RES_BREAK;
	}

	INIT_MSG_WITH_BUFFER(MSG_FI_ADMIN_GET_CHARACTER_INFO_OK, T_FI_ADMIN_GET_CHARACTER_INFO_OK, pMsgGetOK, pMsgGetOKBuf);
	pMsgGetOK->Type = pMsgAdminGetCharInfo->Type;
	pMsgGetOK->AdminCharacterUniqueNumber = pMsgAdminGetCharInfo->AdminCharacterUniqueNumber;
	util::strncpy(pMsgGetOK->AccountName, pCharacSocket->m_character.AccountName, SIZE_MAX_ACCOUNT_NAME);
	util::strncpy(pMsgGetOK->CharacterName, pCharacSocket->m_character.CharacterName, SIZE_MAX_CHARACTER_NAME);
	pMsgGetOK->MapChannelIndex = pCharacSocket->m_character.MapChannelIndex;
	pMsgGetOK->Position = pCharacSocket->m_character.PositionVector;
	pMsgGetOK->Level = pCharacSocket->m_character.Level;

	Write((char*)pMsgGetOKBuf, MSG_SIZE(MSG_FI_ADMIN_GET_CHARACTER_INFO_OK));

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_ADMIN_CALL_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FI_ADMIN_CALL_CHARACTER	*pMsgCallCharacter = NULL;

	nRecvTypeSize = sizeof(MSG_FI_ADMIN_CALL_CHARACTER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_ADMIN_CALL_CHARACTER, ERR_PROTOCOL_INVALID_FIELD_DATA, nLength - nBytesUsed, nRecvTypeSize);
		return RES_PACKET_ERROR;
	}
	pMsgCallCharacter = (MSG_FI_ADMIN_CALL_CHARACTER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pCharacSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgCallCharacter->CharacterUniqueNumber);

	if (pCharacSocket == NULL
		|| FALSE == pCharacSocket->IsValidCharacter())
	{
		// abnormal error, do nothing
		return RES_BREAK;
	}

	CMapProject *pMapProject
		= m_pFieldIOCP4->GetMapWorkspace()->GetMapProjectByMapIndex(pMsgCallCharacter->MapChannelIndex.MapIndex);
	if (NULL == pMapProject)
	{
		return RES_BREAK;
	}

	if(ERR_NO_ERROR != pCharacSocket->IsEnableWarp(pMapProject->m_nMapInfluenceType, pMapProject->IsConflictAreaMap(), pMapProject->m_nQuestIndexForWarp))
	{
		return RES_BREAK;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-22 by dhjin, �Ʒ��� �ʿ��� ���� �Ʒ��� ���� ���̸� ������ Ż�� ó�� �Ѵ�.
	if(IS_MAP_INFLUENCE_ARENA(pCharacSocket->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW())
		&& pCharacSocket->m_ArenaInfo.IsValidArenaState() )
	{
		if(pCharacSocket->m_ArenaInfo.WatchState)
		{// 2007-06-21 by dhjin, �������̸� ���� ������ �ʱ�ȭ�Ѵ�.
			pCharacSocket->WatchEnd();
		}
		else
		{
			m_pFieldIOCP4->m_ArenaManager.DeleteMember(pCharacSocket);			// 2007-04-23 by dhjin, ���� ����
			pCharacSocket->m_ArenaInfo.ResetArenaInfo();		
			pCharacSocket->SendAddMessageType(T_FC_ARENA_REAVE_TEAM_OK);			// 2007-05-03 by dhjin, �Ʒ������� ���� ���� ���� ����
		}
	}	
	
	EventResult_t ret = pCharacSocket->WarpToMap(pMsgCallCharacter->MapChannelIndex, &A2DX(pMsgCallCharacter->Position));
	// event result�� ���� �۾�
	if (ret == EVENT_RESULT_BREAK)
	{
#ifdef _DEBUG
		DBGOUT(STRERR_S_F2NOTIFY_0019,
			pMsgCallCharacter->MapChannelIndex.MapIndex, GetCharacterString(&pCharacSocket->m_character, string()));
#endif
		return RES_RETURN_TRUE;
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_ADMIN_MOVETO_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int								nRecvTypeSize = 0;
	MSG_FI_ADMIN_MOVETO_CHARACTER	*pMsgMoveTo = NULL;

	nRecvTypeSize = sizeof(MSG_FI_ADMIN_MOVETO_CHARACTER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_ADMIN_MOVETO_CHARACTER, ERR_PROTOCOL_INVALID_FIELD_DATA, nLength - nBytesUsed, nRecvTypeSize);
		return RES_PACKET_ERROR;
	}
	pMsgMoveTo = (MSG_FI_ADMIN_MOVETO_CHARACTER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pAdminSocket
		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgMoveTo->AdminCharacterUniqueNumber);

	if (pAdminSocket == NULL)
	{
		// abnormal error, do nothing
		return RES_BREAK;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2007-05-22 by dhjin, �Ʒ��� �ʿ��� ���� �Ʒ��� ���� ���̸� ������ Ż�� ó�� �Ѵ�.
	if(IS_MAP_INFLUENCE_ARENA(pAdminSocket->m_pCurrentFieldMapChannel->GetMapInfluenceTypeW())
		&& pAdminSocket->m_ArenaInfo.IsValidArenaState() )
	{
		if(pAdminSocket->m_ArenaInfo.WatchState)
		{// 2007-06-21 by dhjin, �������̸� ���� ������ �ʱ�ȭ�Ѵ�.
			pAdminSocket->WatchEnd();
		}
		else
		{
			m_pFieldIOCP4->m_ArenaManager.DeleteMember(pAdminSocket);			// 2007-04-23 by dhjin, ���� ����
			pAdminSocket->m_ArenaInfo.ResetArenaInfo();		
			pAdminSocket->SendAddMessageType(T_FC_ARENA_REAVE_TEAM_OK);			// 2007-05-03 by dhjin, �Ʒ������� ���� ���� ���� ����
		}
	}

	CMapProject *pMapProject
		= m_pFieldIOCP4->GetMapWorkspace()->GetMapProjectByMapIndex(pMsgMoveTo->MapChannelIndex.MapIndex);
	if (pMapProject != NULL)
	{
		EventResult_t ret = pAdminSocket->WarpToMap(pMsgMoveTo->MapChannelIndex, &A2DX(pMsgMoveTo->Position));

		// event result�� ���� �۾�
		if (ret == EVENT_RESULT_BREAK)
		{
#ifdef _DEBUG
			DBGOUT(STRERR_S_F2NOTIFY_0020,
				pMsgMoveTo, GetCharacterString(&pAdminSocket->m_character, string()));
#endif
			return RES_RETURN_TRUE;
		}
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_ADMIN_CHANGE_WEATHER(const char* pPacket, int nLength, int &nBytesUsed)
{
	int							nRecvTypeSize = 0;
	MSG_FI_ADMIN_CHANGE_WEATHER	*pMsgWeather = NULL;
	CFieldMapProject			*pMapProject = NULL;
	BOOL						bAllMaps = FALSE;

	nRecvTypeSize = sizeof(MSG_FI_ADMIN_CHANGE_WEATHER);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_ADMIN_CHANGE_WEATHER, ERR_PROTOCOL_INVALID_FIELD_DATA, nLength - nBytesUsed, nRecvTypeSize);
		return RES_PACKET_ERROR;
	}
	pMsgWeather = (MSG_FI_ADMIN_CHANGE_WEATHER*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	if (MAP_INDEX_ALL == pMsgWeather->MapIndex)
	{
		bAllMaps = TRUE;
	}

	if (bAllMaps)
	{
		// ��ü ���� ������ ����
		CFieldMapWorkspace *pWorkspace = (CFieldMapWorkspace*)m_pFieldIOCP4->GetMapWorkspace();

		for (int i=0; i < pWorkspace->GetMapProjectCounts(); i++)
		{
			pMapProject = (CFieldMapProject*)pWorkspace->GetMapProjectByIndex(i);
			if (NULL == pMapProject)
			{
				continue;
			}
			pMapProject->ChangeWeather(pMsgWeather->MapWeather, pMsgWeather->OnOff);
		}
	}
	else
	{
		// Ư�� ���� ������ ����
		pMapProject
			= (CFieldMapProject*)m_pFieldIOCP4->GetMapWorkspace()->GetMapProjectByMapIndex(pMsgWeather->MapIndex);

		if (pMapProject == NULL
			|| !pMapProject->ChangeWeather(pMsgWeather->MapWeather, pMsgWeather->OnOff))
		{
			return RES_BREAK;
		}
	}

	return RES_RETURN_TRUE;
}

ProcessResult CIMWinSocket::Process_FI_CHARACTER_UPDATE_GUILD_INFO(const char* pPacket, int nLength, int &nBytesUsed)
{
	int									nRecvTypeSize = 0;
	MSG_FI_CHARACTER_UPDATE_GUILD_INFO	*pMsgUpdateGuild = NULL;

	nRecvTypeSize = sizeof(MSG_FI_CHARACTER_UPDATE_GUILD_INFO);
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_FI_CHARACTER_UPDATE_GUILD_INFO, ERR_PROTOCOL_INVALID_FIELD_DATA, nLength - nBytesUsed, nRecvTypeSize);
		return RES_PACKET_ERROR;
	}
	pMsgUpdateGuild = (MSG_FI_CHARACTER_UPDATE_GUILD_INFO*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgUpdateGuild->CharacterUniqueNumber);
	if (pCharacterSocket == NULL
		|| !pCharacterSocket->IsUsing()
		|| pCharacterSocket->m_character.CharacterUniqueNumber != pMsgUpdateGuild->CharacterUniqueNumber)
	{
		return RES_BREAK;
	}
	
	// 2007-11-09 by dhjin, ���� ���� �α� �����
	CAtumLogSender::SendLogMessageDisMemberGuild(pCharacterSocket);

	pCharacterSocket->m_character.GuildUniqueNumber = pMsgUpdateGuild->GuildUniqueNumber;
	util::strncpy(pCharacterSocket->m_character.GuildName, pMsgUpdateGuild->GuildName, SIZE_MAX_GUILD_NAME);

	pCharacterSocket->SendCharacterOtherInfo(pCharacterSocket->m_character.ClientIndex, TRUE);
	
	// 2011-01-14 by shcho, ���� ��ũ �ʱ�ȭ �߰� - ���� â�� ���� ����
	if(0 == pCharacterSocket->m_character.GuildUniqueNumber)
	{ // ��� Ż����� ��쿡�� ��ũ�� �ʱ�ȭ�� �ش�.
		pCharacterSocket->m_GuildRank = GUILD_RANK_PRIVATE_NULL;
	}

	return RES_RETURN_TRUE;
}

// 2004-05-24 cmkwon, ������
//ProcessResult CIMWinSocket::Process_FI_GET_FIELD_USER_COUNTS(const char* pPacket, int nLength, int &nBytesUsed)
//{
//	int		nTotalClientCount = 0;
//	int		nTotalMonsterCount = 0;
//// check: MapChannel ������, 20040324, kelovon, ��� ä�ο� ���� ���� ������ �����ؾ� ��
////	for (int k = 0; k < m_pFieldIOCP4->GetMapWorkspace()->m_vectorPtrMapProject.size(); k++)
////	{
////		int nClientCount = m_pFieldIOCP4->GetMapWorkspace()->m_vectorPtrMapProject[k]->GetNumClients();
////		nTotalClientCount += nClientCount;
////		m_pFieldIOCP4->GetMapWorkspace()->m_vectorPtrMapProject[k]->UpdateMaxUserCounts(nClientCount);
////		int nMonsterCount = m_pFieldIOCP4->GetMapWorkspace()->m_vectorPtrMapProject[k]->GetNumMonsters();
////		nTotalMonsterCount += nMonsterCount;
////	}
//	if(m_pFieldIOCP4->m_uiMaxUserCounts < nTotalClientCount)
//	{
//		m_pFieldIOCP4->m_uiMaxUserCounts = nTotalClientCount;
//	}
//
//	char	SendBuf[256];
//	INIT_MSG(MSG_FI_GET_FIELD_USER_COUNTS_ACK, T_FI_GET_FIELD_USER_COUNTS_ACK, pSendUserCountsAck, SendBuf);
//	pSendUserCountsAck->FieldServerID = m_pFieldIOCP4->m_FieldServerID;
//	pSendUserCountsAck->AccumulatedFieldUserCounts = m_pFieldIOCP4->m_uiAccumulatedUserCounts;
//	pSendUserCountsAck->CurrentFieldUserCounts = nTotalClientCount;
//	pSendUserCountsAck->MaxFieldUserCounts = m_pFieldIOCP4->m_uiMaxUserCounts;
//	pSendUserCountsAck->AccumulatedFieldMonsterCounts = m_pFieldIOCP4->m_uiAccumulatedMonsterCounts;
//	pSendUserCountsAck->CurrentFieldMonsterCounts = nTotalMonsterCount;
//
//	Write(SendBuf, MSG_SIZE(MSG_FI_GET_FIELD_USER_COUNTS_ACK));
//
//	return RES_RETURN_TRUE;
//}

ProcessResult CIMWinSocket::Process_ERROR(const char* pPacket, int nLength, int &nBytesUsed)
{
	int			nRecvTypeSize	= 0;
	MSG_ERROR	*pMsgRecvErr	= NULL;

	nRecvTypeSize = sizeof(MSG_ERROR) + ((MSG_ERROR*)(pPacket+nBytesUsed))->StringLength;
	if(nLength - nBytesUsed < nRecvTypeSize)
	{
		SendErrorMessage(T_ERROR, ERR_PROTOCOL_INVALID_FIELD_DATA);
		return RES_BREAK;
	}
	pMsgRecvErr = (MSG_ERROR*)(pPacket + nBytesUsed);
	nBytesUsed += nRecvTypeSize;

	//  ó��
	char buf[128];
	Err_t error = pMsgRecvErr->ErrorCode;

	g_pFieldGlobal->WriteSystemLogEX(TRUE, "  T_ERROR %s(%#04X) received from %s[%s]\r\n", GetErrorString(pMsgRecvErr->ErrorCode), pMsgRecvErr->ErrorCode, "ST_IM_SERVER", m_szPeerIP);

	switch (error)
	{
	case ERR_PROTOCOL_DUPLICATE_LOGIN:
		{
			CFieldIOCPSocket *pFieldIOCPSocket = (CFieldIOCPSocket*)(m_pFieldIOCP4->GetIOCPSocket(pMsgRecvErr->ErrParam1));
			if (pFieldIOCPSocket->GetClientState() != CS_NOTCONNECT)
			{
				pFieldIOCPSocket->SendErrorMessage(T_ERROR, ERR_PROTOCOL_DUPLICATE_LOGIN);
				DBGOUT("[Error]ERR_PROTOCOL_DUPLICATE_LOGIN: %s\n", pFieldIOCPSocket->GetCharacter()->AccountName);
				pFieldIOCPSocket->Close(0x14060);
			}
		}
		break;
	case ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED:
		{
			CFieldIOCPSocket *pFieldIOCPSocket = (CFieldIOCPSocket*)(m_pFieldIOCP4->GetIOCPSocket(pMsgRecvErr->ErrParam1));
			if (pFieldIOCPSocket->GetClientState() != CS_NOTCONNECT)
			{
				pFieldIOCPSocket->SendErrorMessage(T_ERROR, ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED);
				DBGOUT("[Error]ERR_PROTOCOL_CLIENT_IP_NOT_MATCHED: %s\n", pFieldIOCPSocket->GetCharacter()->AccountName);
				pFieldIOCPSocket->Close(0x14061);
			}
		}
		break;
	case ERR_PROTOCOL_NOT_LOGINED:
		{
			CFieldIOCPSocket *pFieldIOCPSocket = (CFieldIOCPSocket*)(m_pFieldIOCP4->GetIOCPSocket(pMsgRecvErr->ErrParam1));
			if (pFieldIOCPSocket->GetClientState() != CS_NOTCONNECT)
			{
				pFieldIOCPSocket->SendErrorMessage(T_ERROR, ERR_PROTOCOL_NOT_LOGINED);
			}
			DBGOUT("[Error]ERR_PROTOCOL_NOT_LOGINED: %d\n", pMsgRecvErr->ErrParam1);
		}
		break;
	case ERR_COMMON_NO_SUCH_CHARACTER:
		{
			DBGOUT("[Error]ERR_COMMON_NO_SUCH_CHARACTER: %d\n", pMsgRecvErr->ErrParam1);
			break;
		}
		break;
	// 2012-07-11 by hskim, ������ �˴ٿ�
	case ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT:
		{
			CFieldIOCPSocket *pFieldIOCPSocket = (CFieldIOCPSocket*)(m_pFieldIOCP4->GetIOCPSocket(pMsgRecvErr->ErrParam1));
			if (pFieldIOCPSocket->GetClientState() != CS_NOTCONNECT)
			{
				pFieldIOCPSocket->SendErrorMessage(T_ERROR, ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT);
				DBGOUT("[Error]ERR_PROTOCOL_SELECTIVE_SHUTDOWN_APPLY_LOGOUT: %s\n", pFieldIOCPSocket->GetCharacter()->AccountName);
				pFieldIOCPSocket->Close(0x14060);
			}
		}
		break;
	// end 2012-07-11 by hskim, ������ �˴ٿ�
	case ERR_PROTOCOL_NO_SUCH_SERVER_GROUP:
	case ERR_PROTOCOL_NO_SUCH_FIELD_SERVER:
	default:
		{
			sprintf(buf, "Cannt Handle Error(From IM Server): %s[%#04X]\n", GetErrorString(pMsgRecvErr->ErrorCode), pMsgRecvErr->ErrorCode);
			//MessageBox(NULL, buf, "Error", MB_OK);
			DBGOUT(buf);
		}
		break;
	}

	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_GUILD_NOTIFY_END_WAR(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_NOTIFY_END_WAR,
									MSG_FI_GUILD_NOTIFY_END_WAR, msgEndGuildWar);

	///////////////////////////////////////////////////////////////////////////////
	// 2004-12-09 by cmkwon, ���� �ʿ��� ó�� �߰��ϸ��(ex, �ʿ� �˸�)

	///////////////////////////////////////////////////////////////////////////////
	// ������ ������ �����Ѵ�
	m_pFieldIOCP4->m_GuildWarManager.EndGuildWar(msgEndGuildWar->WinnerGuildUID);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_GUILD_REG_DELETE_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		���� ���� ��û ���� ó��
/// \author		dhjin
/// \date		2007-11-09 ~ 2007-11-09
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_GUILD_REG_DELETE_GUILD(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_REG_DELETE_GUILD,
									MSG_FI_GUILD_REG_DELETE_GUILD, msg);

	// 2007-11-09 by dhjin, NPC������ ����
	m_pFieldIOCP4->m_OutPostManager.OutPostPossessNPCByDeleteGuild(msg->DeleteGuildUID);

	return RES_RETURN_TRUE;
}


ProcessResult CIMWinSocket::Process_FI_EVENT_NOTIFY_WARP_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
// 2005-07-27 by cmkwon, �ٸ� �ʵ弭������ ������ �����Ƿ� ������
//	int							nRecvTypeSize = 0;
//	MSG_FI_EVENT_NOTIFY_WARP_OK	*pMsgWarpOK = NULL;
//
//	nRecvTypeSize = sizeof(MSG_FI_EVENT_NOTIFY_WARP_OK);
//	if(nLength - nBytesUsed < nRecvTypeSize)
//	{
//		SendErrorMessage(T_FI_EVENT_NOTIFY_WARP_OK, ERR_PROTOCOL_INVALID_FIELD_DATA);
//		return RES_PACKET_ERROR;
//	}
//	pMsgWarpOK = (MSG_FI_EVENT_NOTIFY_WARP_OK*)(pPacket + nBytesUsed);
//	nBytesUsed += nRecvTypeSize;
//
//	CFieldIOCPSocket *pFieldIOCPSocket
//		= m_pFieldIOCP4->m_mapCharacterUniqueNumber.findLock(pMsgWarpOK->CharacterUniqueNumber);
//
//	if (pFieldIOCPSocket == NULL)
//	{
//		// abnormal error, do nothing
//		return RES_BREAK;
//	}
//
//	CHARACTER *pCharacter = &pFieldIOCPSocket->m_character;
//
//	// m_pCurrentEventInfo�� NULL���� Ȯ��
//	if (pFieldIOCPSocket->m_pCurrentEventInfo == NULL)
//	{
//		pFieldIOCPSocket->DBGOUT2(DO2_ALL, "FATAL ERROR@CIMWinSocket::Process_FI_EVENT_NOTIFY_WARP_OK() -> pFieldIOCPSocket->m_pCurrentEventInfo == NULL");
//		pFieldIOCPSocket->Close(70000);
//		return RES_BREAK;
//	}
//
//	CFieldMapProject *pMapProj
//		= (CFieldMapProject*)(m_pFieldIOCP4->GetMapWorkspace()->GetMapProjectByMapIndex(pFieldIOCPSocket->m_pCurrentEventInfo->m_EventwParam3));
//	if (pMapProj == NULL)
//	{
//		DBGOUT("[%d] No Such MapProject: [%d]\n", pCharacter->ClientIndex, pFieldIOCPSocket->m_pCurrentEventInfo->m_EventwParam3);
//		return RES_BREAK;
//	}
//
//	INIT_MSG_WITH_BUFFER(MSG_FP_EVENT_NOTIFY_WARP, T_FP_EVENT_NOTIFY_WARP, pSendMsgNotifyWarp, pSendMsgNotifyWarpBuf);
//	strncpy(pSendMsgNotifyWarp->AccountName, pCharacter->AccountName, SIZE_MAX_ACCOUNT_NAME);
//	pSendMsgNotifyWarp->FieldServerID.SetValue(pMapProj->m_strFieldIP, pMapProj->m_sFieldListenPort);
//	pSendMsgNotifyWarp->CharacterUniqueNumber = pCharacter->CharacterUniqueNumber;
//	m_pFieldIOCP4->m_pPreWinSocket->Write((char*)pSendMsgNotifyWarpBuf, MSG_SIZE(MSG_FP_EVENT_NOTIFY_WARP));

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_GUILD_RANK_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2006-09-25 ~ 2006-09-25
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_GUILD_RANK_OK(const char* pPacket, int nLength, int &nBytesUsed)
{

	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_GUILD_RANK_OK
		, MSG_FI_GUILD_RANK, pRMsg);

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsg->charUID);
	if (NULL == pCharacterSocket
		|| FALSE == pCharacterSocket->IsValidCharacter(FALSE))
	{
		return RES_BREAK;
	}

		
	pCharacterSocket->m_GuildRank = pRMsg->GuildRank;	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_GUILD_DELETE_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2006-09-29 ~ 2006-09-29
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_GUILD_DELETE_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_GUILD_DELETE_INFO_OK
		, MSG_FI_CHARACTER_UPDATE_GUILD_INFO, pRMsg);

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsg->CharacterUniqueNumber);
	if (NULL == pCharacterSocket
		|| FALSE == pCharacterSocket->IsValidCharacter(FALSE))
	{
		return RES_BREAK;
	}

	if (TRUE == pRMsg->GuildDelete)
	{// 2006-09-29 by dhjin, ������ �������� ���� â�� �����۵� ����
		QPARAM_DELETE_GUILDSTORE *pQParam	= new QPARAM_DELETE_GUILDSTORE;
		pQParam->GuildUID				= pRMsg->GuildUniqueNumber;
		pQParam->byItemStorage			= ITEM_IN_GUILD_STORE;
		m_pFieldIOCP4->m_pAtumDBManager->MakeAndEnqueueQuery(QT_AllDeleteGuildStoreItem, pCharacterSocket, pCharacterSocket->GetCharacter()->AccountUniqueNumber, pQParam);
	}

	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CIMWinSocket::Process_FI_CREATE_GUILD_BY_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		dhjin
/// \date		2007-10-06 ~ 2007-10-06
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CIMWinSocket::Process_FI_CREATE_GUILD_BY_SUBLEADER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FI_CREATE_GUILD_BY_SUBLEADER
		, MSG_FI_CREATE_GUILD_BY_SUBLEADER, pRMsg);

	// 2007-10-06 by dhjin, ����¡�� �������� ������ �����Ѵ�.
	m_pFieldIOCP4->m_InflWarManager.SetSubLeaderGuildUID(pRMsg->SubLeaderRank, pRMsg->Influence, pRMsg->CharacterUID);

	return RES_RETURN_TRUE;
}


// 2012-10-17 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
ProcessResult CIMWinSocket::Process_FI_GUILD_BOUNUEXP_RATE(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FI_GUILD_BONUS_EXP_RATE,
		MSG_FI_GUILD_BONUS_EXP_RATE, pRMsg);

	CFieldIOCPSocket *pCharacterSocket = m_pFieldIOCP4->GetFieldIOCPSocketByCharacterUniqueNumber(pRMsg->CharacterUID);
	if (NULL == pCharacterSocket
		|| FALSE == pCharacterSocket->IsValidCharacter(FALSE))
	{
		return RES_BREAK;
	}

	pCharacterSocket->m_nGuildBonusExpRate = pRMsg->nBonusExpRate;
	
	//Ŭ���̾�Ʈ�� ���� ����
	pCharacterSocket->SendToClient_BonusExpRate();
	
	return RES_RETURN_TRUE;
}
// end 2012-10-17 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
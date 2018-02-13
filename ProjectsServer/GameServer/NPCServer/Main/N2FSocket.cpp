// N2FSock.cpp: implementation of the CN2FSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "N2FSocket.h"
#include "IOCP.h"
#include "NPCGlobal.h"
#include "AtumProtocol.h"
#include "AtumError.h"
#include "NPCIOCP.h"
#include "NPCMapChannel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN2FSocket::CN2FSocket(HWND hWnd, UINT wmSock, UINT wmNotify)
:CWinSocket(ST_FIELD_SERVER, hWnd, wmSock, wmNotify, FALSE)
{
	m_pNPCIOCP1		= NULL;
}

CN2FSocket::~CN2FSocket()
{

}

void CN2FSocket::SetNPCIOCP(CNPCIOCP *pNPCIOCP)
{
	m_pNPCIOCP1 = pNPCIOCP;
}


BOOL CN2FSocket::OnCloseSocket(int nErrorCode)
{
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////

	return CWinSocket::OnCloseSocket(nErrorCode);
}

BOOL CN2FSocket::OnConnect(int nErrorCode)
{
	CWinSocket::OnConnect(nErrorCode);
	//////////////////////////////////////////////////////////////////////
	// TODO: Add your specialized code here

	//
	//////////////////////////////////////////////////////////////////////
	return	TRUE;
}


BOOL CN2FSocket::OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq)
{
	int					nBytesUsed	= 0;
	MessageType_t		nType		= 0;
	int					nMsgSize	= 0;
	ProcessResult		procRes		= RES_BREAK;
	char				*pRMsg		= NULL;

	while(nBytesUsed < nLength)
	{
		nType = *(MessageType_t*)(pPacket+nBytesUsed);

#ifdef _DEBUG
		PrintExchangeMsg(RECV_TYPE, nType, m_szPeerIP);
#endif

		switch(nType)
		{
			////////////////////////////////////////////////////////////////////////
			// NPC <-- Field
		case T_FN_NPCSERVER_START_OK:
			{
				nMsgSize	= SIZE_FIELD_TYPE_HEADER;
				pRMsg		= new char[nMsgSize];
				memcpy(pRMsg, pPacket + nBytesUsed, nMsgSize);
				EnterCriticalSection(&m_criticalRecvMessageQueue);
				m_queueRecvMessage.push(pRMsg);
				LeaveCriticalSection(&m_criticalRecvMessageQueue);
				PostNotify(WS_RECEIVED, 0, (char*)this);
				nBytesUsed += nMsgSize;
			}
			procRes	= RES_BREAK;
			break;
		case T_FN_NPCSERVER_SUMMON_JACO_MONSTER:
			nBytesUsed += SIZE_FIELD_TYPE_HEADER;
			procRes = Process_FN_NPCSERVER_SUMMON_JACO_MONSTER(pPacket, nLength, nBytesUsed);
			break;
		case T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL:		// 2007-08-22 by cmkwon, �ش� ��ä�� ���� ��� �����ϱ� ��� �߰� - F->N ����
			nBytesUsed += SIZE_FIELD_TYPE_HEADER;
			procRes = Process_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL(pPacket, nLength, nBytesUsed);
			break;
		case T_FN_NPCSERVER_CINEMA_MONSTER_CREATE:// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Key���� ����
			nBytesUsed += SIZE_FIELD_TYPE_HEADER;
			procRes = Process_FN_NPCSERVER_CINEMA_MONSTER_CREATE(pPacket, nLength, nBytesUsed);
			break;
// start 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ����/���� ��� �߰�
		case T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY:
			nBytesUsed += SIZE_FIELD_TYPE_HEADER;
			procRes = Process_FN_NPCSERVER_CINEMA_MONSTER_DESTROY(pPacket, nLength, nBytesUsed);
			break;
		case T_FN_NPCSERVER_CINEMA_MONSTER_CHANGE:
			nBytesUsed += SIZE_FIELD_TYPE_HEADER;
			procRes = Process_FN_NPCSERVER_CINEMA_MONSTER_CHANGE(pPacket, nLength, nBytesUsed);
			break;
// end 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ����/���� ��� �߰�
// start 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����
		case T_FN_NPCSERVER_CINEMA_MONSTER_REGEN:
			nBytesUsed += SIZE_FIELD_TYPE_HEADER;
			procRes = Process_FN_NPCSERVER_CINEMA_MONSTER_REGEN(pPacket, nLength, nBytesUsed);
			break;
// end 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����
		case T_FN_NPCSERVER_NEW_CHANGE_OBJECT:	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!! 
			nBytesUsed += SIZE_FIELD_TYPE_HEADER;
			procRes = Process_FN_NPCSERVER_NEW_CHANGE_OBJECT(pPacket, nLength, nBytesUsed);
			break;	
		case T_FN_NPCSERVER_RESET_CHANGE_OBJECT:	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!! 
			nBytesUsed += SIZE_FIELD_TYPE_HEADER;
			procRes = Process_FN_NPCSERVER_RESET_CHANGE_OBJECT(pPacket, nLength, nBytesUsed);
			break;

		default:
			{
				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CN2FSocket::OnRecvdPacket Unknown Message Type, %d (0x%08X)\r\n"
					, nType, nType);
				g_pNPCGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return TRUE;
			}
		}

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
/// \fn			ProcessResult CN2FSocket::Process_FN_NPCSERVER_SUMMON_JACO_MONSTER(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		
/// \author		cmkwon
/// \date		2006-04-18 ~ 2006-04-18
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CN2FSocket::Process_FN_NPCSERVER_SUMMON_JACO_MONSTER(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FN_NPCSERVER_SUMMON_JACO_MONSTER,
		MSG_FN_NPCSERVER_SUMMON_JACO_MONSTER, pJacoMonster);

	m_pNPCIOCP1->SetSummonJacoMonster(pJacoMonster->Belligerence0, pJacoMonster->IsSummonJacoMonster);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CN2FSocket::Process_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2007-08-22 by cmkwon, �ش� ��ä�� ���� ��� �����ϱ� ��� �߰�
/// \author		cmkwon
/// \date		2007-08-22 ~ 2007-08-22
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CN2FSocket::Process_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL(const char* pPacket, int nLength, int &nBytesUsed)
{
	DECLARE_MESSAGE_AND_CHECK_SIZE(pPacket, nLength, nBytesUsed, T_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL,
		MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL, pRMsg);

	CNPCMapChannel *pNMChann = m_pNPCIOCP1->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann)
	{
		return RES_BREAK;
	}

	pNMChann->m_mtvectMSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL.pushBackLock(*pRMsg);
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CN2FSocket::Process_FN_NPCSERVER_CINEMA_MONSTER_CREATE(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - �Լ� �̸� �� ����ü �̸� ����, ���Ǵ�Ƽ - Key���� ����
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CN2FSocket::Process_FN_NPCSERVER_CINEMA_MONSTER_CREATE(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_NPCSERVER_CINEMA_MONSTER_CREATE,
		MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE, pRMsg);		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	
	CNPCMapChannel * pNMChann = m_pNPCIOCP1->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}
	
	pNMChann->NPCOnMonsterCinemaMonsterCreate(pRMsg);
	
	return RES_RETURN_TRUE;
}

// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ����/���� ��� �߰�
///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CN2FSocket::Process_FN_NPCSERVER_CINEMA_MONSTER_DESTROY(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
/// \author		hskim
/// \date		2011-04-28
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CN2FSocket::Process_FN_NPCSERVER_CINEMA_MONSTER_DESTROY(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_NPCSERVER_CINEMA_MONSTER_DESTROY,
		MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY, pRMsg);
	
	CNPCMapChannel * pNMChann = m_pNPCIOCP1->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}
	
	pNMChann->NPCOnMonsterCinemaMonsterDestroy(pRMsg);
	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CN2FSocket::Process_FN_NPCSERVER_CINEMA_MONSTER_Change(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
/// \author		hskim
/// \date		2011-05-11
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CN2FSocket::Process_FN_NPCSERVER_CINEMA_MONSTER_CHANGE(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_NPCSERVER_CINEMA_MONSTER_CHANGE,
		MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE, pRMsg);
	
	CNPCMapChannel * pNMChann = m_pNPCIOCP1->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}
	
	pNMChann->NPCOnMonsterCinemaMonsterChange(pRMsg);
	
	return RES_RETURN_TRUE;
}
// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ����/���� ��� �߰�

// start 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����
///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CN2FSocket::Process_FN_NPCSERVER_CINEMA_MONSTER_REGEN(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����
/// \author		hskim
/// \date		2011-06-02
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CN2FSocket::Process_FN_NPCSERVER_CINEMA_MONSTER_REGEN(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_NPCSERVER_CINEMA_MONSTER_REGEN,
		MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN, pRMsg);
	
	CNPCMapChannel * pNMChann = m_pNPCIOCP1->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}
	
	pNMChann->NPCOnMonsterCinemaMonsterRegen(pRMsg);
	
	return RES_RETURN_TRUE;
}
// end 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CN2FSocket::Process_FN_EVENT_NEW_CHANGE_OBJECT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!! 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CN2FSocket::Process_FN_NPCSERVER_NEW_CHANGE_OBJECT(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_NPCSERVER_NEW_CHANGE_OBJECT,
		MSG_FN_NPCSERVER_NEW_CHANGE_OBJECT, pRMsg);
	
	CNPCMapChannel * pNMChann = m_pNPCIOCP1->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}
	
	pNMChann->ChangeNewObject(pRMsg->DeleteObjectUID, pRMsg->NewObjectNum);
	
	return RES_RETURN_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			ProcessResult CN2FSocket::Process_FN_NPCSERVER_RESET_CHANGE_OBJECT(const char* pPacket, int nLength, int &nBytesUsed)
/// \brief		���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!! 
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
ProcessResult CN2FSocket::Process_FN_NPCSERVER_RESET_CHANGE_OBJECT(const char* pPacket, int nLength, int &nBytesUsed) {
	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(pPacket, nLength, nBytesUsed, T_FN_NPCSERVER_RESET_CHANGE_OBJECT,
		MSG_FN_NPCSERVER_RESET_CHANGE_OBJECT, pRMsg);
	
	CNPCMapChannel * pNMChann = m_pNPCIOCP1->GetNPCMapChannelByMapChannelIndex(pRMsg->mapChann);
	if(NULL == pNMChann){			return RES_BREAK;}
	
	pNMChann->ResetChangeObject();
	
	return RES_RETURN_TRUE;
}


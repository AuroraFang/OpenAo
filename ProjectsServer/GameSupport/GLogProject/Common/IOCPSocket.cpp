      ///////////////////////////////////////////////////////////////////////////////
//  IOCPSocket.cpp :
//
//  Date	: 2004-03-17 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IOCPSocket.h"
#include "IOCP.h"
#include "Global.h"
#include "MTAutoCriticalSection.h"

CIOCP		*CIOCPSocket::ms_pIOCP = NULL;							// IOCP �ν��Ͻ� ������



//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////
CIOCPSocket::CIOCPSocket()
:m_ovRecvBuf(COverlapped::EN_OPERATION_MODE_READ, COverlapped::EN_OVERLAPPED_TYPE_STATIC)
{
	m_hSocket						= INVALID_SOCKET;
	m_bUsing						= FALSE;
	m_bFlagDelayClose				= FALSE;
	m_nCloseReasonCode				= 0;
	m_dwCountClientCheck			= 0;

	m_bRecvOperationFlag			= FALSE;

	m_nMaxWriteBufCounts			= 0;
	m_nCurrentWriteBufCounts		= 0;
	m_nBeforMaxWriteBufCountsAtCheckTime = 0;
	m_dwTickLastCheckTimeNetwork	= 0;
	m_dwBadNetworkContinueTime		= 0;
	m_mtlistWriteBuf.clear();
	m_dwCountOverlappedSending		= 0;

	m_bUDPFlag						= FALSE;
	m_bPeerUDPReady					= FALSE;
	m_nOpenedUDPPort				= 0;
	m_nPeerPort						= 0;
	memset(m_szPeerIP, 0x00, SIZE_MAX_IPADDRESS);

	m_bPeerSequenceNumberInitFlag	= FALSE;

	memset(&m_TrafficInfo, 0x00, sizeof(STrafficInfo));
	InitializeCriticalSection(&m_crtlTrafficInfo);

	m_bMustClose					= FALSE;				// 2007-03-12 by cmkwon

	m_LastRecvedMsgType				= 0;					// 2008-03-06 by cmkwon, IOCPSocket �� ������ �޽��� Ÿ�� üũ �ý��� �߰� - 
	m_vectSendedOverlappedInfoList.clear();					// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
}

//////////////////////////////////////////////////////////////////
//
CIOCPSocket::~CIOCPSocket()
{
	if(IsUsing())
	{
		Close(0x00000);
	}

	///////////////////////////////////////////////////////////////////////////
	// ������ ���� list�� �ʱ�ȭ �Ѵ�.
	m_mtlistWriteBuf.lock();
	for (auto x : m_mtlistWriteBuf) delete x;
	m_mtlistWriteBuf.clear();
	m_mtlistWriteBuf.unlock();
	DeleteCriticalSection(&m_crtlTrafficInfo);
}


//////////////////////////////////////////////////////////////////////////
// Property
//////////////////////////////////////////////////////////////////////////
void CIOCPSocket::SetClintArrayIndex(int i_nIndex)
{
	m_nClientArrayIndex = i_nIndex;
}

int CIOCPSocket::GetClientArrayIndex(void)
{
	return m_nClientArrayIndex;
}

// Listener�� ���� ������ ������ CIOCPSocket �ν��Ͻ��� �����Ѵ�.
BOOL CIOCPSocket::SetSocket(SOCKET s)
{
	if(TRUE == m_bUsing || TRUE == m_bUDPFlag)
	{
		return FALSE;
	}
	m_hSocket	= s;
	m_bUsing	= TRUE;

	return TRUE;
}

// CIOCPSocket�� ���� �ڵ��� �����Ѵ�.
SOCKET CIOCPSocket::GetSocket(void)
{
	return m_hSocket;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIOCPSocket::SetRecvOperationFlag(BOOL bFlag)
/// \brief		�ϳ��� Receive ������ �Ϸ� �Ǿ����� �÷��׸� FALSE�� �����ϱ� ���� �Լ�
/// \author		cmkwon
/// \date		2004-03-17 ~ 2004-03-17
/// \warning
///
/// \param		bFlag [in] ���� �� �÷���
/// \return		����
///////////////////////////////////////////////////////////////////////////////
void CIOCPSocket::SetRecvOperationFlag(BOOL bFlag)
{
	m_bRecvOperationFlag = bFlag;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-11-04 by cmkwon, �±� ���Ӱ��� Apex�� ���� - 
/// \author		cmkwon
/// \date		2009-11-04 ~ 2009-11-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
int CIOCPSocket::GetPeerIP4(void)
{
	return inet_addr(this->GetPeerIP());	
}

//////////////////////////////////////////////////////////////////////////
// Method
//////////////////////////////////////////////////////////////////////////
BOOL CIOCPSocket::SetOption(int level, int nOption, char* pValue, int nValueSize)
{
	if (INVALID_SOCKET == m_hSocket){ return FALSE;}
    return	!setsockopt(m_hSocket, level, nOption, pValue, nValueSize);
}

int CIOCPSocket::Read(void)
{
	DWORD	dwRecvBytes = 0;
	DWORD	dwFlag = 0;
	int		nRet = 0;
	if(FALSE == m_bUsing || INVALID_SOCKET == m_hSocket || TRUE == m_bRecvOperationFlag )
	{
		return FALSE;
	}

	m_bRecvOperationFlag = TRUE;					// Overlapped ���� �÷��׸� TRUE�� ����
	m_ovRecvBuf.ResetOverlapped();

	if(m_bUDPFlag == FALSE)
	{
		nRet = WSARecv(m_hSocket, m_ovRecvBuf.GetWSABUFPtr(), 1, &dwRecvBytes, &dwFlag, &m_ovRecvBuf, NULL);
	}
	else
	{
		nRet = WSARecvFrom(m_hSocket, m_ovRecvBuf.GetWSABUFPtr(), 1, &dwRecvBytes, &dwFlag,
			(sockaddr*)m_ovRecvBuf.Getsocketaddr_inPtr(), m_ovRecvBuf.GetSizesocketaddr_inPtr(), &m_ovRecvBuf, NULL);
	}
	if(0 != nRet)
	{
		int err = GetLastError();
		SetLastError(0);
		if(IsError(err) == TRUE)
		{
			m_bRecvOperationFlag = FALSE;
			this->Close(0x10000);

			char	szError[1024];
			sprintf(szError, "CIOCPSocket::Read1 WSARecv(), LastError[%d] Index[%d]\r\n"
				, err, m_nClientArrayIndex);
			g_pGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			return FALSE;
		}
	}
	return dwRecvBytes;
}

int CIOCPSocket::Write(BOOL bWaitFlag)
{
	DWORD	dwSendBytes	= 0;
	DWORD	dwFlag		= 0;
	int		nRet		= 0;

	if(this->IsUsing() == FALSE
		|| m_mtlistWriteBuf.empty() == true)
	{
		return FALSE;
	}

	if(m_dwCountOverlappedSending > 3){						return FALSE;}

// 2008-03-25 by cmkwon, bWaitFlag �� ������� ������ lock() �� ��ƾ� �� �� ����
// 	if(bWaitFlag)
// 	{
// 		m_mtlistWriteBuf.lock();
// 	}
// 	else
// 	{
// 		if(m_mtlistWriteBuf.tryLock() == 0){		return FALSE;}
// 	}
 	m_mtlistWriteBuf.lock();	// 2008-03-25 by cmkwon, bWaitFlag �� ������� ������ lock() �� ��ƾ� �� �� ����

	if(m_mtlistWriteBuf.empty() == true)
	{
		m_mtlistWriteBuf.unlock();
		return FALSE;
	}

	COverlapped* pOver = *m_mtlistWriteBuf.begin();
	m_mtlistWriteBuf.pop_front();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - pOver ���� ������ �����Ѵ�.
	this->DeleteSendedOverlappedInfo(pOver);

	///////////////////////////////////////////////////////////////////////
	// TCP ����
	if(m_bUDPFlag == FALSE)
	{
		pOver->EncodePacket(m_byHostSequenceNumber);

		int tmpSeq;
		tmpSeq = (m_byHostSequenceNumber + SEQNO_VAR_A) * SEQNO_VAR_B;
		if(tmpSeq > SEQNO_VAR_C)
		{
			tmpSeq = tmpSeq % SEQNO_VAR_C;
		}
		m_byHostSequenceNumber = ++tmpSeq;

		InterlockedIncrement((LPLONG)&m_dwCountOverlappedSending);
		nRet = WSASend(m_hSocket, pOver->GetWSABUFPtr(), 1, &dwSendBytes, dwFlag, pOver, NULL);
		if(0 != nRet)
		{
			// 10054 An existing connection was forcibly closed by the remote host. WSAECONNRESET
			// 10054
			int err = WSAGetLastError();
			WSASetLastError(0);
			if(IsError(err) == TRUE)
			{
				util::del(pOver);
				m_mtlistWriteBuf.unlock();
				InterlockedDecrement((LPLONG)&m_dwCountOverlappedSending);
				this->Close(0x10001);

				char	szError[1024];
				sprintf(szError, "CIOCPSocket::Write1 WSASend(), LastError[%d] Index[%d]\r\n"
					, err, m_nClientArrayIndex);
				g_pGlobal->WriteSystemLog(szError);
				DBGOUT(szError);
				return FALSE;
			}
		}
		m_mtlistWriteBuf.unlock();
		return dwSendBytes;
	}

	///////////////////////////////////////////////////////////////////////
	// UDP ����
	if(strncmp(m_szPeerIP, "", SIZE_MAX_IPADDRESS) == 0
		|| 0 == m_nPeerPort
		|| this->GetPeerUDPReady() == FALSE)
	{
		util::del(pOver);
		m_mtlistWriteBuf.unlock();
		return FALSE;
	}

	InterlockedIncrement((LPLONG)&m_dwCountOverlappedSending);
	pOver->Getsocketaddr_inPtr()->sin_family = AF_INET;
	pOver->Getsocketaddr_inPtr()->sin_addr.s_addr = inet_addr(m_szPeerIP);
	pOver->Getsocketaddr_inPtr()->sin_port = htons(m_nPeerPort);
	nRet = WSASendTo(m_hSocket, pOver->GetWSABUFPtr(), 1, &dwSendBytes, dwFlag, (sockaddr*)pOver->Getsocketaddr_inPtr()
		, pOver->GetSizesocketaddr_in(), pOver, NULL);

	if(0 != nRet)
	{
		// 10054
		int err = WSAGetLastError();
		WSASetLastError(0);
		if(IsError(err) == TRUE)
		{
			util::del(pOver);
			m_mtlistWriteBuf.unlock();
			InterlockedDecrement((LPLONG)&m_dwCountOverlappedSending);
			this->Close(0x10002);

			char	szError[1024];
			sprintf(szError, "CIOCPSocket::Write2 WSASend(), LastError[%d] Index[%d]\r\n"
				, err, m_nClientArrayIndex);
			g_pGlobal->WriteSystemLog(szError);
			DBGOUT(szError);
			return FALSE;
		}
	}
	m_mtlistWriteBuf.unlock();
	return TRUE;
}


// ������ �Ϸ�� COverlapped ����ü�� �����͸� ���ڷ� �޴´�. TCP�� UDP���� �ٸ� ������� ó��
void CIOCPSocket::OnWrite(COverlapped *pOverlapped, int nWriteBytes)
{
	InterlockedDecrement((LPLONG)&m_dwCountOverlappedSending);
	delete pOverlapped;

	if(ms_pIOCP->GetFlagCalcTrafficInfo())
	{
		LockTrafficInfo();
		m_TrafficInfo.dwBytesSend += nWriteBytes;
		m_TrafficInfo.dwCountSendPacket++;
		UnlockTrafficInfo();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIOCPSocket::CheckNetworkState(void)
/// \brief		// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - CIOCPSocket::CheckNetworkState() �߰�
/// \author		cmkwon
/// \date		2008-03-13 ~ 2008-03-13
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIOCPSocket::CheckNetworkState(void)
{
	DWORD	dwCur = timeGetTime();
	if(3000 > dwCur - m_dwTickLastCheckTimeNetwork)
	{// 2008-03-13 by cmkwon, 3�ʿ� �ѹ����� üũ
		return;
	}

	if(30 < m_nCurrentWriteBufCounts)
	{
		m_dwBadNetworkContinueTime			+= dwCur-m_dwTickLastCheckTimeNetwork;
		if(m_nBeforMaxWriteBufCountsAtCheckTime < m_nMaxWriteBufCounts
			|| 50 < m_nCurrentWriteBufCounts)
		{
			// 2004-11-13 by cmkwon, m_dwTickLastCheckTimeNetwork�� Update�ϰ� ȣ���ؾ��Ѵ�(�׷��� ������ ������ �߻��Ѵ�)
			m_dwTickLastCheckTimeNetwork = dwCur;				
			this->SendNetworkErrorMessage(m_nCurrentWriteBufCounts, m_dwBadNetworkContinueTime);
		}
	}
	else
	{
		m_dwBadNetworkContinueTime = 0;
	}
	m_nBeforMaxWriteBufCountsAtCheckTime = m_nMaxWriteBufCounts;
	m_dwTickLastCheckTimeNetwork = dwCur;
}

//int CIOCPSocket::ReWrite(char * pData, int nLength)
//{
//	DWORD			dwSendBytes	= 0;
//	DWORD			dwFlag		= 0;
//	int				nRet		= 0;
//	COverlapped*	pOver		= NULL;
//
//	if(FALSE == m_bUsing || INVALID_SOCKET == m_hSocket)
//	{	// Write�Լ��� ����ȴ°��� �����ϱ� ����
//
//		return FALSE;
//	}
//
//	m_mtlistWriteBuf.lock();
//	pOver = new COverlapped(COverlapped::OV_MODE_WRITE, COverlapped::OV_TYPE_DYNAMIC, FALSE);
//	if(pOver->AddData((BYTE*)pData, nLength) == FALSE)
//	{
//		util::del(pOver);
//		m_mtlistWriteBuf.unlock();
//		return FALSE;
//	}
//
//	nRet = WSASend(m_hSocket, &pOver->wsaBuf, 1, &dwSendBytes, dwFlag, pOver, NULL);
//	if(0 != nRet)
//	{
//		m_mtlistWriteBuf.unlock();
//		int err = WSAGetLastError();
//		if(IsError(err) == TRUE)
//		{
//			util::del(pOver);
//			this->Close(0x10003);
//
//			char szSystemLog[256];
//			sprintf(szSystemLog, "	Socket Rewrite Error SocketIndex[%3d] ==> Error Code [%d]\r\n",
//				m_nClientArrayIndex, err);
//			g_pGlobal->WriteSystemLog(szSystemLog);
//			DBGOUT(szSystemLog);
//
//
//		}
//		return dwSendBytes;
//	}
//	m_mtlistWriteBuf.unlock();
//
//	return dwSendBytes;
//}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIOCPSocket::Close(int reason/*=0*/, BOOL bDelayCloseFlag/*=FALSE*/, DWORD i_dwSleepTime/*=0*/)
/// \brief		
/// \author		cmkwon
/// \date		2006-06-08 ~ 2006-06-08
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIOCPSocket::Close(int reason/*=0*/, BOOL bDelayCloseFlag/*=FALSE*/, DWORD i_dwSleepTime/*=0*/)
{
	if(0 < i_dwSleepTime)
	{
		i_dwSleepTime = min(10000, i_dwSleepTime);
		Sleep(i_dwSleepTime);
	}

	CMTAutoCriticalSection mtAuto(m_mtCritSecForClose.GetCriticalSectionPtr());
	if(m_bUsing == FALSE)
	{
		return;
	}

	if(FALSE == m_bUDPFlag)
	{
	//		if(TRUE == bDelayCloseFlag)
	//		{
	//			m_dwCountClientCheck		= 0;
	//			m_nCloseReasonCode			= reason;
	//			m_bFlagDelayClose			= bDelayCloseFlag;
	//			return;
	//		}
	//		else if(TRUE == m_bFlagDelayClose && m_nCloseReasonCode != 0)
		{
			// 2009-03-19 by cmkwon, �ý��� �α� �߰� - 
			//reason = m_nCloseReasonCode;
		}
	}
	m_bUsing						= FALSE;
	m_bFlagDelayClose				= FALSE;
	m_dwCountClientCheck			= 0;
	if(m_hSocket != INVALID_SOCKET)
	{
		shutdown(m_hSocket, SD_SEND);
		closesocket(m_hSocket);
	}
	m_hSocket						= INVALID_SOCKET;
	// 2005-01-03 by cmkwon, m_mtCritSecForClose�� �߰��ϸ鼭 ������
	//Sleep(500);	// �ٸ� thread���� ó������ message�� ���� �� �����Ƿ�, �̸� ���� sleep...

	//////////////////////////////////////////////////////////////////////////////////////////
	// User Overloading function
	// OnClose() ���� �Լ��� CRouteIOCPSocket Ŭ������ OnClose()�Լ��� ȣ���Ѵ�.
	// CRouteIOCPSocket Ŭ������ OnClose()�Լ������� �ڽ��� UDP�� ���� ������ �����ϰ��ִ� ���
	// ������ UDP ������ Close�� �ʱ�ȭ ó���� �Ѵ�.
	OnClose(reason);

	///////////////////////////////////////////////////////////////////////////
	// ������ ���� list�� �ʱ�ȭ �Ѵ�.
	m_nMaxWriteBufCounts			= 0;
	m_nCurrentWriteBufCounts		= 0;
	m_mtlistWriteBuf.lock();
	for (auto x : m_mtlistWriteBuf) delete x;
	m_mtlistWriteBuf.clear();
	m_vectSendedOverlappedInfoList.clear();		// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
	m_mtlistWriteBuf.unlock();
	m_bRecvOperationFlag			= FALSE;
	m_ovRecvBuf.ResetOverlapped();
	m_RecvPacket.Init();

	if(ms_pIOCP->GetFlagCalcTrafficInfo())
	{
		LockTrafficInfo();
		if (m_bUDPFlag == TRUE){	ms_pIOCP->UpdateUDPTrafficInfo(&m_TrafficInfo);}
		else{						ms_pIOCP->UpdateTCPTrafficInfo(&m_TrafficInfo);}
		memset(&m_TrafficInfo, 0x00, sizeof(STrafficInfo));
		UnlockTrafficInfo();
	}

	m_bUDPFlag						= FALSE;
	SetPeerUDPReady(FALSE);
	m_nOpenedUDPPort				= 0;
	m_nPeerPort						= 0;
	memset(m_szPeerIP, 0x00, sizeof(m_szPeerIP));
	m_bPeerSequenceNumberInitFlag	= FALSE;
	m_bMustClose					= FALSE;			// 2007-03-12 by cmkwon

	ms_pIOCP->PushClientIndex(m_nClientArrayIndex);
}

// char* ���ۿ�, ���Ͽ� ������ ����Ÿ�� �����Ѵ�.
BOOL CIOCPSocket::SendAddData(char* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority)
{
	return SendAddData((BYTE*)pData, nSize, i_enPacketPriority);
}


// ���Ͽ� ������ ����Ÿ�� �����Ѵ�.

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIOCPSocket::SendAddData(BYTE* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority)
/// \brief		IOCPSocket���� ������ ����Ÿ�� ���� list�� �߰��Ѵ�
/// \author		cmkwon
/// \date		2004-03-22 ~ 2004-03-22
/// \warning
///
/// \param
/// \return
///////////////////////////////////////////////////////////////////////////////
BOOL CIOCPSocket::SendAddData(BYTE* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority)
{
	if(m_bUsing == FALSE 
		|| m_bFlagDelayClose == TRUE
		|| (TRUE == m_bUDPFlag && FALSE == GetPeerUDPReady())
		|| m_bMustClose)			// 2007-03-12 by cmkwon, ����� ���� �÷��� üũ
	{
		return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////
	// ��Ŷ ���� �ý��� üũ
	// ����� CFieldIOCPSocket���� ��Ŷ ���� ó���� �ǰ� ����
	if(ms_pIOCP->GetIOCPNetworkState() > i_enPacketPriority)
	{	// ��Ŷ ���� �ý������� ��Ŷ �������� ����, return�� TRUE�� �ؾ���

#ifdef _DEBUG
//		///////////////////////////////////////////////////////////////////////////
//		// �׽�Ʈ�� ���� �ڵ���
//		char	szSystemLog[256];
//		sprintf(szSystemLog, "[Notify] CIOCPSocket::SendAddData_1 ��Ŷ ���� �ý������� ���۵��� ����, SocketIndex[%d], NetworkState[%d], PacketPriority[%d]\r\n",
//			m_nClientArrayIndex, m_pIOCP->GetIOCPNetworkState(), i_enPacketPriority);
//		g_pGlobal->WriteSystemLog(szSystemLog);
//		DBGOUT(szSystemLog);
#endif //  _DEBUG_endif
		return TRUE;
	}

	SSendedOverlappedInfo tmSendedOverInfo;		// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
	util::zero(&tmSendedOverInfo, sizeof(SSendedOverlappedInfo));

	COverlapped* pOver = NULL;

	m_mtlistWriteBuf.lock();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - this->PreSendAddData() �Լ����� FALSE �� ���� ������ �߰����� �ʰ� TRUE �� �����Ѵ�.
	if(FALSE == this->PreSendAddData(pData, nSize, m_mtlistWriteBuf.size(), &m_vectSendedOverlappedInfoList))
	{
		m_mtlistWriteBuf.unlock();
		return TRUE;
	}

	if(m_mtlistWriteBuf.empty() == true)
	{
		if(m_bUDPFlag == FALSE)
		{
			pOver = new COverlapped(COverlapped::EN_OPERATION_MODE_WRITE,
				COverlapped::EN_OVERLAPPED_TYPE_DYNAMIC, TRUE);				// Encoding => TRUE
		}
		else
		{
			pOver = new COverlapped(COverlapped::EN_OPERATION_MODE_WRITE,
				COverlapped::EN_OVERLAPPED_TYPE_DYNAMIC, FALSE);			// Encoding => FALSE			
		}
		// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
		//if(pOver->AddData(pData, nSize) == FALSE)
		if(NULL == pOver
			|| FALSE == pOver->AddData(&tmSendedOverInfo, pData, nSize))
		{
			util::del(pOver);
			m_mtlistWriteBuf.unlock();
			return FALSE;
		}
		m_mtlistWriteBuf.push_back(pOver);
	}
	else
	{
		pOver = m_mtlistWriteBuf.back();
		// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
		//if(pOver->AddData(pData, nSize) == FALSE)
		if(FALSE == pOver->AddData(&tmSendedOverInfo, pData, nSize))
		{
			if(m_bUDPFlag == FALSE)
			{
				pOver = new COverlapped(COverlapped::EN_OPERATION_MODE_WRITE,
					COverlapped::EN_OVERLAPPED_TYPE_DYNAMIC, TRUE);				// Encoding => TRUE
			}
			else
			{
				pOver = new COverlapped(COverlapped::EN_OPERATION_MODE_WRITE,
					COverlapped::EN_OVERLAPPED_TYPE_DYNAMIC, FALSE);			// Encoding => FALSE
			}
			// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
			//if(pOver->AddData(pData, nSize) == FALSE)
			if(NULL == pOver
				|| FALSE == pOver->AddData(&tmSendedOverInfo, pData, nSize))
			{
				util::del(pOver);
				m_mtlistWriteBuf.unlock();
				return FALSE;
			}
			m_mtlistWriteBuf.push_back(pOver);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - �߰��� ������ ������ OnSendAddData() �� ȣ�� �Ѵ�.
	this->OnSendAddData(&tmSendedOverInfo, nSize, m_mtlistWriteBuf.size());

	m_nCurrentWriteBufCounts	= m_mtlistWriteBuf.size() + m_dwCountOverlappedSending;
	m_nMaxWriteBufCounts		= max(m_nMaxWriteBufCounts, m_nCurrentWriteBufCounts);
	m_mtlistWriteBuf.unlock();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - ���� �ҽ��� CheckNetworkState() �Լ����� ó��
	this->CheckNetworkState();

	
// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - ���� ���� CheckNetworkState() �� ó��	
// 	DWORD	dwCur = timeGetTime();
// 	if(3000 < dwCur - m_dwTickLastCheckTimeNetwork)
// 	{
// 		// 2008-03-10 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 50 ���� ����(<==30)
// 		if(50 < m_nCurrentWriteBufCounts)
// 		{
// 			m_dwBadNetworkContinueTime += dwCur-m_dwTickLastCheckTimeNetwork;	// 2007-10-23 by cmkwon, ���� ����
// 
// 			// 2008-03-10 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 100 ���� ����(<==40)
// 			if(m_nBeforMaxWriteBufCountsAtCheckTime < m_nMaxWriteBufCounts
// 				|| 100 < m_nCurrentWriteBufCounts)
// 			{
// 				// 2004-11-13 by cmkwon, m_dwTickLastCheckTimeNetwork�� Update�ϰ� ȣ���ؾ��Ѵ�(�׷��� ������ ������ �߻��Ѵ�)
// 				m_dwTickLastCheckTimeNetwork = dwCur;				
// 				this->SendNetworkErrorMessage(m_nCurrentWriteBufCounts, m_dwBadNetworkContinueTime);
// 			}
// 			char szTemp[1024];
// 			wsprintf(szTemp, "	idx(%4d) ==> CurBufCnt(%d) OverlappedSendingCnt(%d) BadNetworkContinueTime(%d)\r\n"
// 				, m_nClientArrayIndex, m_nCurrentWriteBufCounts - m_dwCountOverlappedSending, m_dwCountOverlappedSending, m_dwBadNetworkContinueTime);
// 			DBGOUT(szTemp);
// 			g_pGlobal->WriteSystemLog(szTemp);
// 		}
// 		else
// 		{
// 			m_dwBadNetworkContinueTime = 0;
// 		}
// 		m_nBeforMaxWriteBufCountsAtCheckTime = m_nMaxWriteBufCounts;
// 		m_dwTickLastCheckTimeNetwork = dwCur;
// 	}

	return TRUE;
}

BOOL CIOCPSocket::SendAddLongData(BYTE* pData, int nSize)
{
	BOOL ret = TRUE;
	int offset = 0;

	while ( offset < nSize )
	{
		if (nSize - offset > SIZE_MAX_PACKET)
		{
			ret = SendAddData((BYTE*)(pData + offset), SIZE_MAX_PACKET);
			offset += SIZE_MAX_PACKET;
			if (!ret)
			{
				return ret;
			}
		} else {
			ret = SendAddData((BYTE*)(pData + offset), nSize - offset);
			offset += (nSize - offset);
			if (!ret)
			{
				return ret;
			}
		}
	}

	return ret;
}

BOOL CIOCPSocket::SendAddMessageType(MessageType_t msgType)
{
	return SendAddData((BYTE*)&msgType, SIZE_FIELD_TYPE_HEADER);
}

// error �޼����� ����
void CIOCPSocket::SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1, int errParam2, char* errMsg, BOOL bCloseConnection)
{
}

void CIOCPSocket::SendNetworkErrorMessage(int i_nWriteBufCounts, int i_nBadNetworkContinueTime)
{	
}

 

///////////////////////////////////////////////////////////////////////////////
/// \fn			BOOL CIOCPSocket::PreSendAddData(BYTE *i_pbyData, int i_nSize, int i_nWriteBufCnts, vectSSendedOverlappedInfo *i_pSendedOverInfoList)
/// \brief		// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
/// \author		cmkwon
/// \date		2008-03-12 ~ 2008-03-12
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CIOCPSocket::PreSendAddData(BYTE *i_pbyData, int i_nSize, int i_nWriteBufCnts, vectSSendedOverlappedInfo *i_pSendedOverInfoList)
{
	// 2008-03-14 by cmkwon, FALSE �� �����ϸ� ���� write ���ۿ� �߰����� �ʰ� �׳� ��������. 
	//						 �׷��Ƿ� CIOCPSocket::PreSendAddData() ������ �׻� TRUE �� �����Ѵ�.
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIOCPSocket::OnSendAddData(SSendedOverlappedInfo *i_pSendedOverInfo, int i_nSize, int i_nWriteBufCnts)
/// \brief		// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
/// \author		cmkwon
/// \date		2008-03-14 ~ 2008-03-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIOCPSocket::OnSendAddData(SSendedOverlappedInfo *i_pSendedOverInfo, int i_nSize, int i_nWriteBufCnts)
{
	// do nothing!!
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIOCPSocket::AddSendedOverlappedInfo(SSendedOverlappedInfo *i_pSendedOverInfo)
/// \brief		// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
/// \author		cmkwon
/// \date		2008-03-14 ~ 2008-03-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIOCPSocket::AddSendedOverlappedInfo(SSendedOverlappedInfo *i_pSendedOverInfo)
{
	m_vectSendedOverlappedInfoList.push_back(*i_pSendedOverInfo);
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			void CIOCPSocket::DeleteSendedOverlappedInfo(COverlapped *i_pOverlapped)
/// \brief		// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
/// \author		cmkwon
/// \date		2008-03-14 ~ 2008-03-14
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void CIOCPSocket::DeleteSendedOverlappedInfo(COverlapped *i_pOverlapped)
{
	if(m_vectSendedOverlappedInfoList.empty())
	{
		return;
	}

	vectSSendedOverlappedInfo::iterator itr(m_vectSendedOverlappedInfoList.begin());
	while(itr != m_vectSendedOverlappedInfoList.end())
	{
		SSendedOverlappedInfo *pSendedOver = itr;
		if(pSendedOver->pOverlapped == i_pOverlapped)
		{
			itr = m_vectSendedOverlappedInfoList.erase(itr);
			continue;
		}
		itr++;
	}
}

void CIOCPSocket::OnReceive(char* pBlock, int length, ENServerType ServerType, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	if(this->IsUsing() == FALSE || m_bFlagDelayClose){	return;}

	if(ms_pIOCP->GetFlagCalcTrafficInfo())
	{
		LockTrafficInfo();
		m_TrafficInfo.dwBytesRecv += length;
		m_TrafficInfo.dwCountRecvPacket++;
		UnlockTrafficInfo();
	}

	int		nBytesRecvd		= length;
	int		nBytesTotalUsed	= 0;
	int		nBytesUsed		= 0;
	BYTE	bySeq			= 0;
	BOOL	bRet			= FALSE;

	if(this->m_bUDPFlag)
	{
		bySeq = *((BYTE*)pBlock + SIZE_BODY_LENGTH + SIZE_ENCODE_FLAG);
		switch(ServerType)
		{
		case ST_PRE_SERVER:
			OnRecvdPacketPreServer(pBlock+SIZE_PACKET_HEADER, length - SIZE_PACKET_HEADER, bySeq, pPeerIP, nPeerPort, i_pThreadInfo);
			break;
		case ST_IM_SERVER:
			OnRecvdPacketIMServer(pBlock+SIZE_PACKET_HEADER, length - SIZE_PACKET_HEADER, bySeq, pPeerIP, nPeerPort, i_pThreadInfo);
			break;
		case ST_LOG_SERVER:
			OnRecvdPacketLogServer(pBlock+SIZE_PACKET_HEADER, length - SIZE_PACKET_HEADER, bySeq, pPeerIP, nPeerPort, i_pThreadInfo);
			break;
		case ST_FIELD_SERVER:
			OnRecvdPacketFieldServer(pBlock+SIZE_PACKET_HEADER, length - SIZE_PACKET_HEADER, bySeq, pPeerIP, nPeerPort, i_pThreadInfo);
			break;
		case ST_NPC_SERVER:
			OnRecvdPacketNPCServer(pBlock+SIZE_PACKET_HEADER, length - SIZE_PACKET_HEADER, bySeq, pPeerIP, nPeerPort, i_pThreadInfo);
			break;
		}
		return;
	}

	while(this->IsUsing() && m_bFlagDelayClose == FALSE && nBytesRecvd > 0)
	{
		bRet = m_RecvPacket.AddTail(pBlock + nBytesTotalUsed, nBytesRecvd, &nBytesUsed);
		if(bRet < 0)
		{	// ���ϰ��� ���̳ʽ��̸� ����, size ����� SIZE_MAX_PACKET(1500 bytes)���� ũ��, ���� �߻� ������ �ݴ´�.

			Close(0x01000);
			return;
		}

		nBytesTotalUsed += nBytesUsed;
		nBytesRecvd -= nBytesUsed;
		bRet = FALSE;
		if(m_RecvPacket.RecvCompleted() == TRUE)
		{
#if defined(SERVICE_TYPE_RUSSIAN_SERVER_1)			// 2009-03-27 by cmkwon, ���þ� ���ν�Ʈ �ý��� ����(���� ��Ŷ) - ���þƸ� �̷��� ó��
			// 2009-03-27 by cmkwon, ���þ� ���ν�Ʈ �ý��� ����(���� ��Ŷ) - ���� �ؾ��� ��Ŷ �� 8bytes ¥�� (|2bytes(4)|1bytes(0)|1bytes(0xFF)|4bytes(data)|), 
			// 2009-06-01 by cmkwon, ���þ� ���ν�Ʈ �ý��� ����(���� ��Ŷ) - ���� �ؾ��� ��Ŷ �� 10bytes ¥�� (|2bytes(0x06)|1bytes(0x00)|1bytes(0xFF)|6bytes(data)|), 
			if(FALSE == m_RecvPacket.IsPacketEncoded()
				&& 6 == m_RecvPacket.GetLength()
				&& 0xFF == m_RecvPacket.GetSequenceNumber())
			{
				if(IS_MASANG_IP(this->GetPeerIP())				// 2009-12-28 by cmkwon, ����ȸ�� IP ���� - ����(121.134.114.)
					|| 0 == strncmp(this->GetPeerIP(), "87.241.186.", 11))
				{// 2009-06-25 by cmkwon, �⺻ ��ƾ�� üũ�Ǿ����� �α״� ����� �̳�ٸ� ������ ����
					// 2009-03-27 by cmkwon, �ӽ÷� �ý��� �α� ����, �ý��ۿ� ������ ������ �����ϴ� ���� ����
					g_pGlobal->WriteSystemLogEX(TRUE, "[TEMP] Recved ignore StreamLenth(%d) PacketLength(%d) RecvData(%4d) \r\n", length, m_RecvPacket.GetLength(), *(DWORD*)m_RecvPacket.GetPacket());
				}

				m_RecvPacket.Init();
				continue;
			}
#endif
			if(m_RecvPacket.IsPacketEncoded() == FALSE)
			{
				switch(ServerType)
				{
				case ST_PRE_SERVER:
					bRet = OnRecvdPacketPreServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_byPeerSequenceNumber, NULL, 0, i_pThreadInfo);
					break;
				case ST_IM_SERVER:
					bRet = OnRecvdPacketIMServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_byPeerSequenceNumber, NULL, 0, i_pThreadInfo);
					break;
				case ST_LOG_SERVER:
					bRet = OnRecvdPacketLogServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_byPeerSequenceNumber, NULL, 0, i_pThreadInfo);
					break;
				case ST_FIELD_SERVER:
					bRet = OnRecvdPacketFieldServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_byPeerSequenceNumber, NULL, 0, i_pThreadInfo);
					break;
				case ST_NPC_SERVER:
					bRet = OnRecvdPacketNPCServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_byPeerSequenceNumber, NULL, 0, i_pThreadInfo);
					break;
				case ST_NORMAL_SERVER:
					bRet = OnRecvdPacketNormalServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_byPeerSequenceNumber, NULL, 0, i_pThreadInfo);
					break;
				}
			}
			else if(m_RecvPacket.IsValidPacket() == TRUE)
			{
				switch(ServerType)
				{
				case ST_PRE_SERVER:
					bRet = OnRecvdPacketPreServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_RecvPacket.GetSequenceNumber(), NULL, 0, i_pThreadInfo);
					break;
				case ST_IM_SERVER:
					bRet = OnRecvdPacketIMServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_RecvPacket.GetSequenceNumber(), NULL, 0, i_pThreadInfo);
					break;
				case ST_LOG_SERVER:
					bRet = OnRecvdPacketLogServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_RecvPacket.GetSequenceNumber(), NULL, 0, i_pThreadInfo);
					break;
				case ST_FIELD_SERVER:
					bRet = OnRecvdPacketFieldServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_RecvPacket.GetSequenceNumber(), NULL, 0, i_pThreadInfo);
					break;
				case ST_NPC_SERVER:
					bRet = OnRecvdPacketNPCServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_RecvPacket.GetSequenceNumber(), NULL, 0, i_pThreadInfo);
					break;
				case ST_NORMAL_SERVER:
					bRet = OnRecvdPacketNormalServer(m_RecvPacket.GetPacket(), m_RecvPacket.GetLength(), m_RecvPacket.GetSequenceNumber(), NULL, 0, i_pThreadInfo);
					break;
				}
			}
			else
			{	// Encoded Packet������ ��ȿ���� ���� ��Ŷ�̴�.
				Close(0x10001, TRUE);

				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CIOCPSocket::OnReceive invalid packet, SocketIndex[%d]\r\n", m_nClientArrayIndex);
				g_pGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);
				return;
			}

			if(bRet == FALSE)
			{
				// Protocol Error
				char	szSystemLog[256];
				sprintf(szSystemLog, "[Error] CIOCPSocket::OnReceive Error! Closing Connection!, SocketIndex[%d]\r\n", m_nClientArrayIndex);
				g_pGlobal->WriteSystemLog(szSystemLog);
				DBGOUT(szSystemLog);

				// close connection
				Close(0x10001, TRUE);

				return;
			}
			m_RecvPacket.Init();
		}
	}
}

BOOL CIOCPSocket::OnRecvdPacketNormalServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	// This function should not be called.
	// Instead, the function of child class should be called.
	DBGOUT("Implementation ERROR! The function of child class should be called!!!");
#ifdef _DEBUG
	assert(0);
#endif

	return FALSE;
}

BOOL CIOCPSocket::OnRecvdPacketFieldServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	// This function should not be called.
	// Instead, the function of child class should be called.
	DBGOUT("Implementation ERROR! The function of child class should be called!!!");
#ifdef _DEBUG
	assert(0);
#endif

	return FALSE;
}

BOOL CIOCPSocket::OnRecvdPacketIMServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	// This function should not be called.
	// Instead, the function of child class should be called.
	DBGOUT("Implementation ERROR! The function of child class should be called!!!");
#ifdef _DEBUG
	assert(0);
#endif

	return FALSE;
}

BOOL CIOCPSocket::OnRecvdPacketNPCServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	// This function should not be called.
	// Instead, the function of child class should be called.
	DBGOUT("Implementation ERROR! The function of child class should be called!!!");
#ifdef _DEBUG
	assert(0);
#endif

	return FALSE;
}

BOOL CIOCPSocket::OnRecvdPacketPreServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	// This function should not be called.
	// Instead, the function of child class should be called.
	DBGOUT("Implementation ERROR! The function of child class should be called!!!");
#ifdef _DEBUG
	assert(0);
#endif

	return FALSE;
}

BOOL CIOCPSocket::OnRecvdPacketLogServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	// This function should not be called.
	// Instead, the function of child class should be called.
	DBGOUT("Implementation ERROR! The function of child class should be called!!!");
#ifdef _DEBUG
	assert(0);
#endif

	return FALSE;
}

void CIOCPSocket::OnConnect()
{
	m_nMaxWriteBufCounts			= 0;
	m_nCurrentWriteBufCounts		= 0;
	m_nBeforMaxWriteBufCountsAtCheckTime = 0;
	m_dwBadNetworkContinueTime		= 0;
	m_dwTickLastCheckTimeNetwork	= 0;

	m_dwCountOverlappedSending		= 0;
	m_RecvPacket.Init();
	m_dwCountClientCheck			= 0;
	m_byHostSequenceNumber = RANDI(0, SEQNO_VAR_C - 1);								// TCP �ʱⰪ�� SEQNO_VAR_C ������ �۰� ����
	if(m_byHostSequenceNumber == 116)
	{
		m_byHostSequenceNumber++;
	}
	m_bPeerSequenceNumberInitFlag = FALSE;

	m_LastRecvedMsgType				= 0;		// 2008-03-06 by cmkwon, IOCPSocket �� ������ �޽��� Ÿ�� üũ �ý��� �߰� - 

	///////////////////////////////////////////////////////////////////////////////
	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - m_mtlistWriteBuf �� lock �ϰ� �ʱ�ȭ �Ѵ�.
	mt_auto_lock mtA(&m_mtlistWriteBuf);
	m_vectSendedOverlappedInfoList.clear();
	mtA.auto_unlock_cancel();
}

void CIOCPSocket::OnClose(int reason)
{
}

BOOL CIOCPSocket::OnError(int errCode)
{
	return FALSE;
}

void CIOCPSocket::InitIOCPSocket(int nIdx)
{
	m_nClientArrayIndex	= nIdx;
}

BOOL CIOCPSocket::IsUsing(void)
{
	return m_bUsing;
}

void CIOCPSocket::SetPeerAddress(char* pIP, int nPort)
{
	util::strncpy(m_szPeerIP, pIP, SIZE_MAX_IPADDRESS);
	m_nPeerPort = nPort;
}


BOOL CIOCPSocket::IsError(int errCode)
{
	if(0 == errCode)
	{
		errCode = WSAGetLastError();
		WSASetLastError(0);
	}

	switch (errCode)
	{
		case 0:
		case WSA_IO_INCOMPLETE:			// 996L Overlapped I/O event is not in a signaled state.
		case WSA_IO_PENDING:			// 997L Overlapped I/O operation is in progress.
			break;
		case WSA_INVALID_HANDLE:		// 6L The handle is invalid.
		case WSA_NOT_ENOUGH_MEMORY:		// 8L Not enough storage is available to process this command.
		case ERROR_NETNAME_DELETED:		// 64L The specified network name is no longer available.
		case WSA_INVALID_PARAMETER:		// 87L The parameter is incorrect.
		case WSA_OPERATION_ABORTED:		// 995L The I/O operation has been aborted because of either a thread exit or an application request.
		case WSAEINTR:					// 10004 A blocking operation was interrupted by a call to WSACancelBlockingCall.
		case WSAEBADF:					// 10009 The file handle supplied is not valid.
		case WSAEACCES:					// 10013 An attempt was made to access a socket in a way forbidden by its access permissions.
		case WSAEFAULT:					// 10014 The system detected an invalid pointer address in attempting to use a pointer argument in a call.
		case WSAEINVAL:					// 10022 An invalid argument was supplied.
		case WSAEMFILE:					// 10024 Too many open sockets.
		case WSAEWOULDBLOCK:			// 10035 A non-blocking socket operation could not be completed immediately.
		case WSAEINPROGRESS:			// 10036 A blocking operation is currently executing.
		case WSAEALREADY:				// 10037 An operation was attempted on a non-blocking socket that already had an operation in progress.
		case WSAENOTSOCK:				// 10038 An operation was attempted on something that is not a socket.
		case WSAEDESTADDRREQ:			// 10039 A required address was omitted from an operation on a socket.
		case WSAEMSGSIZE:				// 10040 A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself.
		case WSAEPROTOTYPE:				// 10041 A protocol was specified in the socket function call that does not support the semantics of the socket type requested.
		case WSAENOPROTOOPT:			// 10042 An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call.
		case WSAEPROTONOSUPPORT:		// 10043 The requested protocol has not been configured into the system, or no implementation for it exists.
		case WSAESOCKTNOSUPPORT:		// 10044 The support for the specified socket type does not exist in this address family.
		case WSAEOPNOTSUPP:				// 10045 The attempted operation is not supported for the type of object referenced.
		case WSAEPFNOSUPPORT:			// 10046 The protocol family has not been configured into the system or no implementation for it exists.
		case WSAEAFNOSUPPORT:			// 10047 An address incompatible with the requested protocol was used.
		case WSAEADDRINUSE:				// 10048 Only one usage of each socket address (protocol/network address/port) is normally permitted.
		case WSAEADDRNOTAVAIL:			// 10049 The requested address is not valid in its context.
		case WSAENETDOWN:				// 10050 A socket operation encountered a dead network.
		case WSAENETUNREACH:			// 10051 A socket operation was attempted to an unreachable network.
		case WSAENETRESET:				// 10052 The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.
		case WSAECONNABORTED:			// 10053 An established connection was aborted by the software in your host machine.
		case WSAECONNRESET:				// 10054 An existing connection was forcibly closed by the remote host.
		case WSAENOBUFS:				// 10055 An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.
		case WSAEISCONN:				// 10056 A connect request was made on an already connected socket.
		case WSAENOTCONN:				// 10057 A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.
		case WSAESHUTDOWN:				// 10058 A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.
		case WSAETOOMANYREFS:			// 10059 Too many references to some kernel object.
		case WSAETIMEDOUT:				// 10060 A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.
		case WSAECONNREFUSED:			// 10061 No connection could be made because the target machine actively refused it.
		case WSAELOOP:					// 10062 Cannot translate name.
		case WSAENAMETOOLONG:			// 10063 Name component or name was too long.
		case WSAEHOSTDOWN:				// 10064 A socket operation failed because the destination host was down.
		case WSAEHOSTUNREACH:			// 10065 A socket operation was attempted to an unreachable host.
		case WSAENOTEMPTY:				// 10066 Cannot remove a directory that is not empty.
		case WSAEPROCLIM:				// 10067 A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously.
		case WSAEUSERS:					// 10068 Ran out of quota.
		case WSAEDQUOT:					// 10069 Ran out of disk quota.
		case WSAESTALE:					// 10070 File handle reference is no longer available.
		case WSAEREMOTE:				// 10071 Item is not available locally.
		case WSASYSNOTREADY:			// 10091 WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.
		case WSAVERNOTSUPPORTED:		// 10092 The Windows Sockets version requested is not supported.
		case WSANOTINITIALISED:			// 10093 Either the application has not called WSAStartup, or WSAStartup failed.
		case WSAEDISCON:				// 10101 Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence.
		case WSAENOMORE:				// 10102 No more results can be returned by WSALookupServiceNext.
		case WSAECANCELLED:				// 10103 A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.
		case WSAEINVALIDPROCTABLE:		// 10104 The procedure call table is invalid.
		case WSAEINVALIDPROVIDER:		// 10105 The requested service provider is invalid.
		case WSAEPROVIDERFAILEDINIT:	// 10106 The requested service provider could not be loaded or initialized.
		case WSASYSCALLFAILURE:			// 10107 A system call that should never fail has failed.
		case WSASERVICE_NOT_FOUND:		// 10108 No such service is known. The service cannot be found in the specified name space.
		case WSATYPE_NOT_FOUND:			// 10109 The specified class was not found.
		case WSA_E_NO_MORE:				// 10110 No more results can be returned by WSALookupServiceNext.
		case WSA_E_CANCELLED:			// 10111 A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.
		case WSAEREFUSED:				// 10112 A database query failed because it was actively refused.
		case WSAHOST_NOT_FOUND:			// 11001 No such host is known.
		case WSATRY_AGAIN:				// 11002 This is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server.
		case WSANO_RECOVERY:			// 11003 A non-recoverable error occurred during a database lookup.
		case WSANO_DATA:				// 11004 The requested name is valid, but no data of the requested type was found.
			return TRUE;
	}
	return FALSE;
}

BOOL CIOCPSocket::OpenUDPPort(int nPort, int nRetryCount)
{
	if(m_bUsing || m_hSocket != INVALID_SOCKET){ return FALSE;}

	SOCKADDR_IN udp_addr;
	int			nLen;
	BOOL		quit = TRUE;

	m_hSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}

	m_dwCountOverlappedSending = 0;
	do
	{
		ZeroMemory(&udp_addr, sizeof(udp_addr));
		udp_addr.sin_port = htons((u_short)nPort);
		udp_addr.sin_family = AF_INET;
		udp_addr.sin_addr.s_addr = INADDR_ANY;
		nLen = sizeof(SOCKADDR_IN);

		if(SOCKET_ERROR == bind(m_hSocket, (LPSOCKADDR)&udp_addr, nLen))
		{
			nPort+=1;
		}
		else
		{
			m_bUsing			= TRUE;
			m_bUDPFlag			= TRUE;
//			this->SetPeerUDPReady(TRUE);
			m_nOpenedUDPPort	= nPort;
		}

		nRetryCount--;
	}while(nRetryCount >= 0);

	if(!m_bUDPFlag)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
	return m_bUDPFlag;
}

void CIOCPSocket::SetPeerUDPReady(BOOL bReadyFlag)
{
	m_bPeerUDPReady = bReadyFlag;
}

BOOL CIOCPSocket::GetPeerUDPReady(void)
{
	return m_bPeerUDPReady;
}

void CIOCPSocket::ClientCheck(void)
{
	m_dwCountClientCheck++;
	if(m_dwCountClientCheck > 5)
	{// üũ�ð���*5 ���� AlivePacekt�� ���� ����

		Close(0x10007);
	}
}

void CIOCPSocket::LockTrafficInfo(void)
{
	EnterCriticalSection(&m_crtlTrafficInfo);
}

void CIOCPSocket::UnlockTrafficInfo(void)
{
	LeaveCriticalSection(&m_crtlTrafficInfo);
}


void CIOCPSocket::OnRecvdAlivePacket(void)
{
	if(FALSE == m_bFlagDelayClose)
	{
		m_dwCountClientCheck = 0;
	}
}
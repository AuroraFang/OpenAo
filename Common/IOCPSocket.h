#pragma once

#include "mt_stl.h"
#include "WinPacket.h"
#include "Overlapped.h"
#include "ThreadCheck.h"
#include "MTCriticalSection.h"

#include "ProcList.h"

enum EN_NETWORK_STATE
{
	EN_NETWORK_STATE_WORST = 0,
	EN_NETWORK_STATE_BAD = 1,
	EN_NETWORK_STATE_NORMAL = 2,
	EN_NETWORK_STATE_GOOD = 3,
	EN_NETWORK_STATE_BEST = 4				// Network State �� ���� ���� ����
};

enum EN_PACKET_PRIORITY
{
	EN_PACKET_PRIORITY_LOWEST = 0,
	EN_PACKET_PRIORITY_LOW = 1,
	EN_PACKET_PRIORITY_NORMAL = 2,
	EN_PACKET_PRIORITY_HIGH = 3,
	EN_PACKET_PRIORITY_HIGHEST = 4				// ���� ���� �켱����
};

struct STrafficInfo
{
	DWORD	dwBytesSend;
	DWORD	dwBytesRecv;
	DWORD	dwCountSendPacket;
	DWORD	dwCountRecvPacket;
	DWORD	dwTimeGapSecond;

	void Reset(DWORD currentTick)
	{
		dwBytesSend = 0;
		dwBytesRecv = 0;
		dwCountSendPacket = 0;
		dwCountRecvPacket = 0;

		dwTimeGapSecond = currentTick;
	}

	STrafficInfo& operator+=(STrafficInfo& value)
	{
		dwBytesSend += value.dwBytesSend;
		dwBytesRecv += value.dwBytesRecv;

		dwCountSendPacket += value.dwCountSendPacket;
		dwCountRecvPacket += value.dwCountRecvPacket;

		return *this;
	}
};

typedef mt_list<COverlapped*>	mtlistCOverlappedPtr;

///////////////////////////////////////////////////////////////////////////////
/// \class		CIOCPSocket
///
/// \brief		IOCPSocket ���� ���� �ֻ��� Ŭ����
/// \author		cmkwon
/// \version
/// \date		2004-03-17 ~ 2004-03-17
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CIOCPSocket
{
	friend class CIOCP;
	friend class CFieldIOCP;
	friend class CNPCIOCP;

public:

	template<typename socket_t, T0 t0> ProcessResult CallHandlerT1(MessageType_t msgtype, const char* data, int size, int& offset)
	{
		using thisproclist = metaseries256::transform<proclist::get_handlersT1<socket_t, t0>>;

		auto handler = thisproclist::values[msgtype & 0xFF];

		if (handler) return handler(static_cast<socket_t*>(this), data, size, offset);

		return RES_PACKET_NA;
	}

	template<typename socket_t> ProcessResult CallHandlerT0(MessageType_t msgtype, const char* data, int size, int& offset)
	{
		using thisproclist = metaseries256::transform<proclist::get_handlersT0<socket_t>>;

		auto handler = thisproclist::values[msgtype >> 8];

		if (handler) return handler(static_cast<socket_t*>(this), msgtype, data, size, offset);

		return RES_PACKET_NA;
	}


	CIOCPSocket();

	virtual ~CIOCPSocket();

	///////////////////////////////////////////////////////////////////////////
	// Property
	void SetClientArrayIndex(int i_nIndex) { m_nClientArrayIndex = i_nIndex; }
	int GetClientArrayIndex() const { return m_nClientArrayIndex; }
	BOOL SetSocket(SOCKET s);
	void SetRecvOperationFlag(BOOL bFlag) { m_bRecvOperationFlag = bFlag; }
	const char* GetPeerIP() const { return m_szPeerIP; };		// ����� ���� IP Address�� ����
	int GetPeerPort() const { return m_nPeerPort; };		// ����� ���� Port�� ����
	int GetPeerIP4() const { return inet_addr(m_szPeerIP); }	// 2009-11-04 by cmkwon, �±� ���Ӱ��� Apex�� ���� - ����� ���� IP Address�� 4����Ʈ�� ����

	void InitIOCPSocket(int nIdx) { m_nClientArrayIndex = nIdx; }		// IOCPSocket ���� �� ���� ȣ�� �ؾ���
	bool IsUsing() const { return m_bUsing; }									// CIOCPSocket�� �ν��Ͻ��� ��� �������� ���θ� ����
	void SetPeerAddress(char* pIP, int nPort);			// TCP:����� ���� IP�� Port�� ����,  UDP:UDP ����� �� Client�� IP�� Port�� �����Ѵ�.
	void ClientCheck();
	void LockTrafficInfo() { EnterCriticalSection(&m_crtlTrafficInfo); }
	void UnlockTrafficInfo() { LeaveCriticalSection(&m_crtlTrafficInfo); }
	void OnRecvdAlivePacket();
	BOOL OpenUDPPort(int nPort, int nRetryCount = 0);

	void SetPeerUDPReady(bool bReadyFlag) { m_bPeerUDPReady = bReadyFlag; }
	bool GetPeerUDPReady() const { return m_bPeerUDPReady; }


	int Read();												// �������κ��� ����Ÿ�� �ޱ����� ���۸� �غ�, m_ovRecvBuf�� ������ ���ο��� ó��
	int Write(BOOL bWaitFlag);											// �������� ����Ÿ�� �����Ѵ�. m_listWriteBuf ������ ���ο��� ó��
	void OnWrite(COverlapped *pOverlapped, int nWriteBytes);	// ������ �Ϸ�� COverlapped ����ü�� �����͸� ���ڷ� �޴´�. TCP�� UDP���� �ٸ� ������� ó��

	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
	void CheckNetworkState();


	//int ReWrite(char * pData, int nLength);					// ������ ����Ÿ�� ��� ���۵��� ������� ���� ����Ÿ�� �������Ѵ�. m_listWriteBuf ������ ���ο��� ó��
	void Close(int reason = 0, BOOL bDelayCloseFlag = FALSE, DWORD i_dwSleepTime = 0);	// ������ �ݴ´�.
	BOOL SendAddData(const BYTE* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority = EN_PACKET_PRIORITY_HIGHEST, BOOL bRawSend = FALSE, BOOL bSessionEnd = FALSE);	// 2013-03-13 by hskim, �� ĳ�� ���� - RawData ���� ��� �߰� - // ���Ͽ� ������ ����Ÿ�� �����Ѵ�.
	BOOL SendAddLongData(PBYTE pData, int nSize);				// ���Ͽ� ������ ����Ÿ�� �����Ѵ�. (nSize > SIZE_MAX_PACKET)
	BOOL SendAddMessageType(MessageType_t msgType) { return SendAddData(PBYTE(&msgType), SIZE_FIELD_TYPE_HEADER); }				// Message Type�� �����Ѵ�.
	BOOL SendAddData(char* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority = EN_PACKET_PRIORITY_HIGHEST) { return SendAddData((BYTE*)pData, nSize, i_enPacketPriority); }
	BOOL SendAddLongData(char* pData, int nSize) { return SendAddLongData(PBYTE(pData), nSize); }	// char* ���ۿ�, ���Ͽ� ������ ����Ÿ�� �����Ѵ�. (nSize > SIZE_MAX_PACKET)
	BOOL SendAddRawData(BYTE* pData, int nSize, BOOL bSessionEnd = FALSE) { return SendAddData(pData, nSize, EN_PACKET_PRIORITY_HIGHEST, TRUE, bSessionEnd); }	// 2013-03-13 by hskim, �� ĳ�� ���� - RawData ���� ��� �߰�


	template<template<MessageType_t> typename data_t, MessageType_t msgtype>
	bool SendAddData(const data_t<msgtype>& packet) { return this->SendAddData(reinterpret_cast<const BYTE*>(&packet), packet.size()); }

	////////////////////////////////////////////////////////////////////////////
	// virtual����Լ�
	// Data Receive ����
	void OnReceive(char* pBlock, int length, ENServerType ServerType = ST_NORMAL_SERVER, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo = NULL);
	
	
	// Return values matter only for TCP connections
	// true, packets were processed fine, keep socket alive
	// false, fetal error, close the socket!
	virtual BOOL OnRecvdPacket(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo = NULL) = 0;

	// ���� ����
	virtual void OnConnect();
	virtual void OnClose(int reason) { }

	// Error ����
	virtual BOOL OnError(int errCode) { return false; }
	virtual void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, const char* errMsg = NULL, BOOL bCloseConnection = FALSE) { }
	virtual void SendNetworkErrorMessage(int i_nWriteBufCounts, int i_nBadNetworkContinueTime) { }

	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
	//virtual BOOL PreSendAddData(const BYTE *i_pbyData, int i_nSize, int i_nWriteBufCnts, vectSSendedOverlappedInfo *i_pSendedOverInfoList) { return true; }
	virtual void OnSendAddData(SSendedOverlappedInfo *i_pSendedOverInfo, int i_nSize, int i_nWriteBufCnts) { }

	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
	void AddSendedOverlappedInfo(SSendedOverlappedInfo *i_pSendedOverInfo) { m_vectSendedOverlappedInfoList.push_back(*i_pSendedOverInfo); }
	void DeleteSendedOverlappedInfo(COverlapped *i_pOverlapped);

	static bool IsError(int errCode = 0);						// ���� ���۽� �߻��� ������ Error���� ���θ� ����

protected:
	int						m_nClientArrayIndex;				// IOCP���� ����ϴ� Client �迭 �ε���
	SOCKET					m_hSocket;							// ������ Handle
	bool					m_bUsing;							// IOCPSocket ��ü�� ��� �������� �÷���
	bool					m_bFlagDelayClose;					// Ŭ���̾�Ʈ���� Close()�� �����ϵ��� �޼����� ���´ٴ� �÷��׷� �����ð����� Close���� ������ ���� ����
	DWORD					m_dwCountClientCheck;				// Alive Check�� ���� ��� ����
	int						m_nCloseReasonCode;					// ����� ���� �ڵ�
	CMTCriticalSection		m_mtCritSecForClose;

	BOOL					m_bRecvOperationFlag;				// Receive�� ���� ���� �غ� 1���� �߻��ϵ��� �ϱ� ����(�����δ� �ʿ䰡 ������ �߸��� ����� ���� ó��
	COverlapped				m_ovRecvBuf;						// Receive�� ���� Overlapped(static) ����ü ����
	CRecvPacket				m_RecvPacket;						// Received ����Ÿ�� ��Ŷ������ ������ ���� ����
	//CRecvHTTPPacket			m_RecvHTTPPacket;				// 2016 DevX	// 2013-03-13 by hskim, �� ĳ�� ���� - Received HTTP ����Ÿ�� ����� �ٵ�� ������ ���� ����

	int						m_nMaxWriteBufCounts;				// �ִ� writeBuf counts
	int						m_nCurrentWriteBufCounts;			// ���� writeBuf counts
	int						m_nBeforMaxWriteBufCountsAtCheckTime;	//
	DWORD					m_dwTickLastCheckTimeNetwork;		// 1�ʿ� �ѹ� üũ�� ����
	DWORD					m_dwBadNetworkContinueTime;			// ���°� ���� ���� ��Ʈ��ũ ���� �ð�


	mtlistCOverlappedPtr	m_mtlistWriteBuf;					// �����Ϸ��� Overlapped ����ü�� �����͸� ������ list
	DWORD					m_dwCountOverlappedSending;			// �������� �������� �ִ� COverlapped�� ī��Ʈ

	BYTE					m_byHostSequenceNumber;
	BYTE					m_byPeerSequenceNumber;
	bool					m_bPeerSequenceNumberInitFlag;		// ������ �޾ƾ��� Sequence Number�� �ʱ�ȭ ���� �÷���

	bool					m_bUDPFlag;							// ICOPSocket�� TCP�� UDP�� ������ ���� �÷���
	bool					m_bPeerUDPReady;					// UDP�� ���� ������ ��Ʈ�� �غ� �Ǿ��ִ����� �÷���
	int						m_nOpenedUDPPort;					// Open�� UDP Port ��ȣ
	int						m_nPeerPort;						// UDP ����� ���� ���� Port
	char					m_szPeerIP[SIZE_MAX_IPADDRESS];		// TCP : ���� ����� Client�� IP Address,  UDP : ����� ���� ���� IP Address

	STrafficInfo			m_TrafficInfo;
	CRITICAL_SECTION		m_crtlTrafficInfo;

	bool					m_bMustClose;						// 2007-03-12 by cmkwon, ����� ���� �÷���

	MessageType_t			m_LastRecvedMsgType;				// 2008-03-06 by cmkwon, IOCPSocket �� ������ �޽��� Ÿ�� üũ �ý��� �߰� - 

	vectSSendedOverlappedInfo	m_vectSendedOverlappedInfoList;	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 

	static CIOCP*			ms_pIOCP;							// IOCP �ν��Ͻ� ������

public:
	ENServerType			m_PeerSocketType;
};

using mtvectorIOCPSocket = mt_vector<CIOCPSocket*>;


// macro	DECLARE_MESSAGE_AND_CHECK_SIZE
// brief	CXXXIOCPSocket::Process_XXX() �Լ� ������ �ʱ⿡ �޼��� ���� �� ������ Ȯ�� ��ũ��
// author	kelovon
// date		2004-04-09
// warning	CXXXIOCPSocket::Process_XXX() �Լ� �������� ����ؾ� ��!
// 2009-06-12 by cmkwon, ������ - ���� ���� �߰�
#define DECLARE_MESSAGE_AND_CHECK_SIZE(__PPACKET, __NLENGTH, __NBYTESUSED, __MSGTYPE, __MSGSTRUCT, __PMESSAGEVAR)	\
	INT			__NRECVTYPESIZE	= sizeof(__MSGSTRUCT);					\
	__MSGSTRUCT	*__PMESSAGEVAR	= nullptr;								\
	if (__NLENGTH - __NBYTESUSED < __NRECVTYPESIZE)						\
	{																	\
		SendErrorMessage(__MSGTYPE, ERR_PROTOCOL_INVALID_FIELD_DATA, __NLENGTH - __NBYTESUSED, __NRECVTYPESIZE);	\
		return RES_RETURN_FALSE;										\
	}																	\
	__PMESSAGEVAR = (__MSGSTRUCT*)(__PPACKET+__NBYTESUSED);				\
	__NBYTESUSED += __NRECVTYPESIZE;

// macro	DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER - �������� �޽��� ��ȯ������ ���
// brief	CXXXIOCPSocket::Process_XXX() �Լ� ������ �ʱ⿡ �޼��� ���� �� ������ Ȯ�� ��ũ��
// author	kelovon
// date		2004-06-02
// warning	CXXXIOCPSocket::Process_XXX() �Լ� �������� ����ؾ� ��!
#define DECLARE_MESSAGE_AND_CHECK_SIZE_SERVER(__PPACKET, __NLENGTH, __NBYTESUSED, __MSGTYPE, __MSGSTRUCT, __PMESSAGEVAR)	\
	INT			__NRECVTYPESIZE	= 0;									\
	__MSGSTRUCT	*__PMESSAGEVAR	= NULL;									\
	__NRECVTYPESIZE = sizeof(__MSGSTRUCT);								\
	if (__NLENGTH - __NBYTESUSED < __NRECVTYPESIZE)						\
	{																	\
		SendErrorMessage(__MSGTYPE, ERR_PROTOCOL_INVALID_FIELD_DATA);	\
		return RES_PACKET_ERROR;										\
	}																	\
	__PMESSAGEVAR = (__MSGSTRUCT*)(__PPACKET+__NBYTESUSED);				\
	__NBYTESUSED += __NRECVTYPESIZE;
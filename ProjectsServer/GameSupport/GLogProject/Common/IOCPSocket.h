///////////////////////////////////////////////////////////////////////////////
//  IOCPSocket.h :
//
//  Date	: 2004-03-17 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#ifndef _IOCP_SOCKET_H_
#define _IOCP_SOCKET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MT_stl.h"
#include "WinPacket.h"
#include "Overlapped.h"
#include "ThreadCheck.h"
#include "MTCriticalSection.h"

// macro	DECLARE_MESSAGE_AND_CHECK_SIZE
// brief	CXXXIOCPSocket::Process_XXX() �Լ� ������ �ʱ⿡ �޼��� ���� �� ������ Ȯ�� ��ũ��
// author	kelovon
// date		2004-04-09
// warning	CXXXIOCPSocket::Process_XXX() �Լ� �������� ����ؾ� ��!
// 2009-06-12 by cmkwon, ������ - ���� ���� �߰�
#define DECLARE_MESSAGE_AND_CHECK_SIZE(__PPACKET, __NLENGTH, __NBYTESUSED, __MSGTYPE, __MSGSTRUCT, __PMESSAGEVAR)	\
	INT			__NRECVTYPESIZE	= 0;									\
	__MSGSTRUCT	*__PMESSAGEVAR	= NULL;									\
	__NRECVTYPESIZE = sizeof(__MSGSTRUCT);								\
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

typedef enum
{
	EN_NETWORK_STATE_WORST		= 0,
	EN_NETWORK_STATE_BAD		= 1,
	EN_NETWORK_STATE_NORMAL		= 2,
	EN_NETWORK_STATE_GOOD		= 3,
	EN_NETWORK_STATE_BEST		= 4				// Network State �� ���� ���� ����
} EN_NETWORK_STATE;

typedef enum
{
	EN_PACKET_PRIORITY_LOWEST	= 0,
	EN_PACKET_PRIORITY_LOW		= 1,
	EN_PACKET_PRIORITY_NORMAL	= 2,
	EN_PACKET_PRIORITY_HIGH		= 3,
	EN_PACKET_PRIORITY_HIGHEST	= 4				// ���� ���� �켱����
} EN_PACKET_PRIORITY;

typedef struct
{
	DWORD	dwBytesSend;
	DWORD	dwBytesRecv;
	DWORD	dwCountSendPacket;
	DWORD	dwCountRecvPacket;
	DWORD	dwTimeGapSecond;
} STrafficInfo;

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
public:
	CIOCPSocket();
	virtual ~CIOCPSocket();

	///////////////////////////////////////////////////////////////////////////
	// Property
	void SetClintArrayIndex(int i_nIndex);
	int GetClientArrayIndex(void);
	BOOL SetSocket(SOCKET s);
	SOCKET GetSocket(void);
	void SetRecvOperationFlag(BOOL bFlag);
	char* GetPeerIP(void){	return m_szPeerIP;};		// ����� ���� IP Address�� ����
	int GetPeerPort(void){	return m_nPeerPort;};		// ����� ���� Port�� ����
	int GetPeerIP4(void);		// 2009-11-04 by cmkwon, �±� ���Ӱ��� Apex�� ���� - ����� ���� IP Address�� 4����Ʈ�� ����


	///////////////////////////////////////////////////////////////////////////
	// Method
	//
	void InitIOCPSocket(int nIdx);		// IOCPSocket ���� �� ���� ȣ�� �ؾ���
	BOOL IsUsing(void);									// CIOCPSocket�� �ν��Ͻ��� ��� �������� ���θ� ����
	void SetPeerAddress(char* pIP, int nPort);			// TCP:����� ���� IP�� Port�� ����,  UDP:UDP ����� �� Client�� IP�� Port�� �����Ѵ�.
	BOOL IsError(int errCode = 0);						// ���� ���۽� �߻��� ������ Error���� ���θ� ����
	void ClientCheck(void);
	void LockTrafficInfo(void);
	void UnlockTrafficInfo(void);
	void OnRecvdAlivePacket(void);
	BOOL OpenUDPPort(int nPort, int nRetryCount = 0);

	void SetPeerUDPReady(BOOL bReadyFlag);
	BOOL GetPeerUDPReady(void);

	BOOL SetOption(int level, int nOption, char* pValue, int nValueSize);

	int Read(void);												// �������κ��� ����Ÿ�� �ޱ����� ���۸� �غ�, m_ovRecvBuf�� ������ ���ο��� ó��
	int Write(BOOL bWaitFlag);											// �������� ����Ÿ�� �����Ѵ�. m_listWriteBuf ������ ���ο��� ó��
	void OnWrite(COverlapped *pOverlapped, int nWriteBytes);	// ������ �Ϸ�� COverlapped ����ü�� �����͸� ���ڷ� �޴´�. TCP�� UDP���� �ٸ� ������� ó��

	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
	void CheckNetworkState(void);


	//int ReWrite(char * pData, int nLength);					// ������ ����Ÿ�� ��� ���۵��� ������� ���� ����Ÿ�� �������Ѵ�. m_listWriteBuf ������ ���ο��� ó��
	void Close(int reason=0, BOOL bDelayCloseFlag=FALSE, DWORD i_dwSleepTime=0);	// ������ �ݴ´�.
	BOOL SendAddData(BYTE* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority = EN_PACKET_PRIORITY_HIGHEST);	// ���Ͽ� ������ ����Ÿ�� �����Ѵ�.
	BOOL SendAddLongData(BYTE* pData, int nSize);				// ���Ͽ� ������ ����Ÿ�� �����Ѵ�. (nSize > SIZE_MAX_PACKET)
	BOOL SendAddMessageType(MessageType_t msgType);				// Message Type�� �����Ѵ�.
	BOOL SendAddData(char* pData, int nSize, EN_PACKET_PRIORITY i_enPacketPriority = EN_PACKET_PRIORITY_HIGHEST);	// char* ���ۿ�, ���Ͽ� ������ ����Ÿ�� �����Ѵ�.
	BOOL SendAddLongData(char* pData, int nSize) { return SendAddLongData((BYTE*)pData, nSize); }	// char* ���ۿ�, ���Ͽ� ������ ����Ÿ�� �����Ѵ�. (nSize > SIZE_MAX_PACKET)
	
	////////////////////////////////////////////////////////////////////////////
	// virtual����Լ�
	// Data Receive ����
	void OnReceive(char* pBlock, int length, ENServerType ServerType = ST_NORMAL_SERVER, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	virtual BOOL OnRecvdPacketNormalServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	virtual BOOL OnRecvdPacketFieldServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	virtual BOOL OnRecvdPacketIMServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	virtual BOOL OnRecvdPacketNPCServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	virtual BOOL OnRecvdPacketPreServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	virtual BOOL OnRecvdPacketLogServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	
	// ���� ����
	virtual void OnConnect(void);
	virtual void OnClose(int reason);

	// Error ����
	virtual BOOL OnError(int errCode);
	virtual void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection = FALSE);
	virtual void SendNetworkErrorMessage(int i_nWriteBufCounts, int i_nBadNetworkContinueTime);

	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
	virtual BOOL PreSendAddData(BYTE *i_pbyData, int i_nSize, int i_nWriteBufCnts, vectSSendedOverlappedInfo *i_pSendedOverInfoList);
	virtual void OnSendAddData(SSendedOverlappedInfo *i_pSendedOverInfo, int i_nSize, int i_nWriteBufCnts);

	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
	void AddSendedOverlappedInfo(SSendedOverlappedInfo *i_pSendedOverInfo);
	void DeleteSendedOverlappedInfo(COverlapped *i_pOverlapped);

protected:
	int						m_nClientArrayIndex;				// IOCP���� ����ϴ� Client �迭 �ε���
	SOCKET					m_hSocket;							// ������ Handle
	BOOL					m_bUsing;							// IOCPSocket ��ü�� ��� �������� �÷���
	BOOL					m_bFlagDelayClose;					// Ŭ���̾�Ʈ���� Close()�� �����ϵ��� �޼����� ���´ٴ� �÷��׷� �����ð����� Close���� ������ ���� ����
	DWORD					m_dwCountClientCheck;				// Alive Check�� ���� ��� ����
	int						m_nCloseReasonCode;					// ����� ���� �ڵ�
	CMTCriticalSection		m_mtCritSecForClose;

	BOOL					m_bRecvOperationFlag;				// Receive�� ���� ���� �غ� 1���� �߻��ϵ��� �ϱ� ����(�����δ� �ʿ䰡 ������ �߸��� ����� ���� ó��
	COverlapped				m_ovRecvBuf;						// Receive�� ���� Overlapped(static) ����ü ����
	CRecvPacket				m_RecvPacket;						// Received ����Ÿ�� ��Ŷ������ ������ ���� ����

	int						m_nMaxWriteBufCounts;				// �ִ� writeBuf counts
	int						m_nCurrentWriteBufCounts;			// ���� writeBuf counts
	int						m_nBeforMaxWriteBufCountsAtCheckTime;	//
	DWORD					m_dwTickLastCheckTimeNetwork;		// 1�ʿ� �ѹ� üũ�� ����
	DWORD					m_dwBadNetworkContinueTime;			// ���°� ���� ���� ��Ʈ��ũ ���� �ð�


	mtlistCOverlappedPtr	m_mtlistWriteBuf;					// �����Ϸ��� Overlapped ����ü�� �����͸� ������ list
	DWORD					m_dwCountOverlappedSending;			// �������� �������� �ִ� COverlapped�� ī��Ʈ

	BYTE					m_byHostSequenceNumber;
	BYTE					m_byPeerSequenceNumber;
	BOOL					m_bPeerSequenceNumberInitFlag;		// ������ �޾ƾ��� Sequence Number�� �ʱ�ȭ ���� �÷���

	BOOL					m_bUDPFlag;							// ICOPSocket�� TCP�� UDP�� ������ ���� �÷���
	BOOL					m_bPeerUDPReady;					// UDP�� ���� ������ ��Ʈ�� �غ� �Ǿ��ִ����� �÷���
	int						m_nOpenedUDPPort;					// Open�� UDP Port ��ȣ
	int						m_nPeerPort;						// UDP ����� ���� ���� Port
	char					m_szPeerIP[SIZE_MAX_IPADDRESS];		// TCP : ���� ����� Client�� IP Address,  UDP : ����� ���� ���� IP Address

	STrafficInfo			m_TrafficInfo;
	CRITICAL_SECTION		m_crtlTrafficInfo;

	BOOL					m_bMustClose;						// 2007-03-12 by cmkwon, ����� ���� �÷���

	MessageType_t			m_LastRecvedMsgType;				// 2008-03-06 by cmkwon, IOCPSocket �� ������ �޽��� Ÿ�� üũ �ý��� �߰� - 

	vectSSendedOverlappedInfo	m_vectSendedOverlappedInfoList;	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 

	static CIOCP*			ms_pIOCP;							// IOCP �ν��Ͻ� ������

public:
	ENServerType			m_PeerSocketType;
};

typedef mt_vector<CIOCPSocket*>				mtvectorIOCPSocket;


#endif // _IOCP_SOCKET_H_endif

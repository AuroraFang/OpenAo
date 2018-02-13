// WinSocket.h: interface for the CWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSOCKET_H__22EBB374_73E5_4603_A72D_9B79226CA850__INCLUDED_)
#define AFX_WINSOCKET_H__22EBB374_73E5_4603_A72D_9B79226CA850__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SocketHeader.h"
#include "WinPacket.h"
#include "mt_stl.h"
#include <queue>

using namespace std;

// socket HWND���� �޴� �޽���(�Ʒ� �޽����� �������ؼ� �ʿ��� �޽����� �ٲ� �� �ִ�.)
#define	WM_USER_BASE				(WM_USER + 222)
#define	WM_ASYNC_EVENT				(WM_USER_BASE + 1)				//
#define	WM_PACKET_NOTIFY			(WM_USER_BASE + 2)				// HWND�� ���޵Ǵ� �޽���


class CWinSocket
{
public:
	enum
	{
		WS_ERROR					= 0x0001,
		WS_CONNECTED				= 0x0002,
		WS_ACCEPTED					= 0x0003,		// 2007-06-21 by cmkwon, Ŭ���̾�Ʈ�� �ҽ��� ����
		WS_RECEIVED					= 0x0004,
		WS_CLOSED					= 0x0005
	};
	CWinSocket(HWND hWnd, UINT wmSock = WM_ASYNC_EVENT, UINT wmNotify = WM_PACKET_NOTIFY, BOOL bEncoding = FALSE);
	virtual ~CWinSocket();

protected:
	HWND					m_hMsgWnd;							// Socket Event�� Notify �޽��� ���� ������
	SOCKET					m_Socket;							// ������ �ڵ�
	UINT					m_wmSocket;							// HWND���� Socket ó�� �޽���(WSAAsyncSelect�Լ� ������)
	UINT					m_wmNotify;							// HWND�� �뺸�Ǵ� �޽���(HWND�� ���� ó����� �뺸)
	BOOL					m_bConnected;						// ������ remote�� ����Ǿ������� �÷���
	int						m_nLastError;						// ������ Error code
	int						m_nAsyncFlag;						// WSAAsyncSelect Event parameter
	BOOL					m_bEncodingFlag;

	SOCKADDR_IN				m_SockAddrLocal;					// local�� ���� ���� �ּ� ����ü(Bind�ÿ� �ʿ�)
	SOCKADDR_IN				m_SockAddrRemote;					// remote�� ���� ���� �ּ� ����ü(Accetp�� Connect�ÿ� �ʿ�)
	list<CSendPacket*>		m_listSendPacket;					// Send Packet Pointer list
	CRITICAL_SECTION		m_criticalSendList;					// Send Packet List �� ����ȭ�� ����
	char					m_RecvBuffer[SIZE_MAX_SOCKET_BUFFER];	// Receive Buffer
	CRecvPacket				m_RecvPacket;						//
	queue<char*>			m_queueRecvMessage;					// Received Message�� ������ ���� ť
	CRITICAL_SECTION		m_criticalRecvMessageQueue;			// Received Message Queue�� ����ȭ�� ����

	BYTE					m_byHostSequenceNumber;
	BYTE					m_byPeerSequenceNumber;
	BOOL					m_bPeerSequenceNumberInitFlag;		// ������ �޾ƾ��� Sequence Number�� �ʱ�ȭ ���� �÷���

	MessageType_t			m_msgTyLastPacket;					// 2008-02-22 by cmkwon, ������ ��Ŷ �޽��� Ÿ��

public:
	int						m_nPeerPort;						// UDP ����� ���� ���� Port
	char					m_szPeerIP[SIZE_MAX_IPADDRESS];		// TCP : ���� ����� Client�� IP Address,  UDP : ����� ���� ���� IP Address
	ENServerType			m_PeerSocketType;
	BOOL					m_bSendNotify;						// Send just one time Notify message 20060514 by dhkwon

protected:
    // Socket Misc. operation
	BOOL CreateSocket();
	inline void SetAsyncFlag(int nMode);
	inline BOOL IsError(int nError = 0);
// Operations
public:
	inline BOOL AsyncSelect(int nMode);
	inline BOOL AsyncSelect();
	inline SOCKET GetSocketHandle(void);
	BOOL SetOptionWinSocket(int level, int nOption, char* pValue, int nValueSize);		// 2007-06-21 by cmkwon, TCP Nagle �˰��� ������ ����

	// 2007-12-21 by cmkwon, CWinSocket �� ��ŷ ��� ���� - �����߰�(BOOL i_bBlockingMode=FALSE)
	BOOL Connect(LPCSTR strPeerIP, int nPort, BOOL i_bBlockingMode=FALSE);	// Client�μ� ����, Server �� ����õ�
	BOOL Bind(int nPort);										// Server�μ� ����, ������ ��Ʈ�� ������´�. Listen�ÿ� �ʿ�
	BOOL Listen(int nBackLog = 1);								// Server�μ� ����, �����Ϸ��� Clinet�� ��ٸ���.
	BOOL CloseSocket(int reason = 0);							// Socket�� �ݴ´�.

	BOOL Write(LPCSTR pPacket, int nLength);
	inline BOOL Write(BYTE *pPacket, int nLength) { return Write((LPCSTR)pPacket, nLength); }	// BYTE* ���ۿ�
	BOOL WriteMessageType(MessageType_t msgType);
	BOOL Read(LPSTR *pPacket, int &nLength);

	///////////////////////////////////////////////////////////////////////////////
	// 2007-12-21 by cmkwon, CWinSocket �� ��ŷ ��� ���� - 
	BOOL WriteBlockingMode(LPCSTR pPacket, int nLength, int i_nLimiteTimeInSec=-1);		// 2007-12-21 by cmkwon, CWinSocket �� ��ŷ ��� ���� - CWinSocket::WriteBlockingMode() �߰�
	BOOL ReadBlockingMode(LPSTR o_pBuffer, int nRLength, int i_nLimiteTimeInSec=-1);	// 2007-12-21 by cmkwon, CWinSocket �� ��ŷ ��� ���� - CWinSocket::ReadBlockingMode() �߰�

	void SendErrorMessage(MessageType_t msgType, Err_t err, int errParam1 = 0, int errParam2 = 0, char* errMsg = NULL, BOOL bCloseConnection = FALSE);

	int GetSendBufferCounts(void){				return m_listSendPacket.size();}
	int GetLastErrorCode(void);									//
	BOOL IsConnected(void);										//
	BOOL PostNotify(WORD nNotify, WORD nParam = 0, char* pData = NULL);
	BOOL SendNotify(WORD nNotify, WORD nParam = 0, char* pData = NULL);

public:
	// Message Operations
	LONG OnAsyncEvent(LONG lParam);								// ���� �߻��ϴ� Socket Event�� ����� �������� Handle�� �ҷ��� Event handler
	BOOL OnAccept(int nErrorCode);								// Accept�� ���Ŀ� �߻�
	BOOL OnReceive(int nErrorCode);								// ���� ����Ÿ�� ������ �߻�
	BOOL OnSendReady(int nErrorCode);							// ������ ����Ÿ �����Ҽ� ������ �߻�
	BOOL OnOutOfBand(int nErrorCode);							//

	virtual BOOL OnConnect(int nErrorCode);						// ������ ������Ŀ� �߻�
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);		// �ϳ��� ��Ŷ�� �������� �� �߻�
	virtual BOOL OnCloseSocket(int nErrorCode);					// ������ ������ �߻�

	static BOOL m_bSocketInitFlag;
	static BOOL SocketInit(void);
	static void SocketClean(void);
};

SOCKET CWinSocket::GetSocketHandle(void)
{
	return m_Socket;
}

void CWinSocket::SetAsyncFlag(int nMode)
{
	m_nAsyncFlag = nMode;
}

BOOL CWinSocket::AsyncSelect()
{
	return	AsyncSelect(m_nAsyncFlag);
}

BOOL CWinSocket::AsyncSelect(int nMode)
{
	if (INVALID_SOCKET == m_Socket || !IsWindow(m_hMsgWnd)){ return FALSE;}
 	if (SOCKET_ERROR == WSAAsyncSelect(m_Socket, m_hMsgWnd, nMode ? m_wmSocket : 0, nMode))
	{
		return !IsError();
	}

	return TRUE;
}

BOOL CWinSocket::IsError(int nError)
{
	if(0 == nError)
	{
		nError = WSAGetLastError();
		WSASetLastError(0);
	}
//	DbgOut("Is Error(%d)\n", nError);
	switch (nError)
	{
		case WSAENOBUFS:
		case WSAEWOULDBLOCK:
		case WSAEFAULT:
		case 0:
			return	FALSE;
		case WSAEACCES:
		case WSAEADDRINUSE:
		case WSAEADDRNOTAVAIL:
		case WSAEAFNOSUPPORT:
		case WSAEALREADY:
		case WSAECONNABORTED:
		case WSAECONNREFUSED:
		case WSAECONNRESET:
		case WSAEDESTADDRREQ:
		case WSAEHOSTDOWN:
		case WSAEHOSTUNREACH:
		case WSAEINPROGRESS:
		case WSAEINTR:
		case WSAEINVAL:
		case WSAEISCONN:
		case WSAEMFILE:
		case WSAEMSGSIZE:
		case WSAENETDOWN:
		case WSAENETRESET:
		case WSAENETUNREACH:
		case WSAENOPROTOOPT:
		case WSAENOTCONN:
		case WSAENOTSOCK:
		case WSAEOPNOTSUPP:
		case WSAEPFNOSUPPORT:
		case WSAEPROCLIM:
		case WSAEPROTONOSUPPORT:
		case WSAEPROTOTYPE:
		case WSAESHUTDOWN:
		case WSAESOCKTNOSUPPORT:
		case WSAETIMEDOUT:
		case WSATYPE_NOT_FOUND:
		case WSAHOST_NOT_FOUND:
		case WSA_INVALID_HANDLE:
		case WSA_INVALID_PARAMETER:
		case WSA_IO_INCOMPLETE:
		case WSA_IO_PENDING:
		//case WSA_NOT_ENOUGH_MEMORY:
		case WSANOTINITIALISED:
		case WSANO_DATA:
		case WSANO_RECOVERY:
		case WSAEPROVIDERFAILEDINIT:
		case WSASYSCALLFAILURE:
		case WSAEINVALIDPROCTABLE:
		case WSAEINVALIDPROVIDER:
		case WSASYSNOTREADY:
		case WSATRY_AGAIN:
		case WSAVERNOTSUPPORTED:
		case WSAEDISCON:
		case WSA_OPERATION_ABORTED:
			break;
		default:
			return FALSE;

	}
	m_nLastError = nError;
	CloseSocket(0x10003);
	return	TRUE;
}

#endif // !defined(AFX_WINSOCKET_H__22EBB374_73E5_4603_A72D_9B79226CA850__INCLUDED_)

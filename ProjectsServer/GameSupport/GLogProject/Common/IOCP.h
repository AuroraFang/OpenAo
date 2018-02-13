// IOCP.h: interface for the IOCP.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCP_H__57FB97C3_C117_4E67_99B3_4CA554A27807__INCLUDED_)
#define AFX_IOCP_H__57FB97C3_C117_4E67_99B3_4CA554A27807__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include <Mmsystem.h>
#include "winpacket.h"
#include "IOCPSocket.h"
#include "ThreadCheck.h"
#include "QueueINT.h"

#define	COUNT_MAX_IOCP_THREAD							20		// IOCP ������ �ִ� ����, IOCP ������ �迭�� ũ��, ���� �����Ǵ� ������ ������ �ƴ�
#define COUNT_SOCKET_WRITE_THREAD						2		// IOCP Wirte Operation�� ���� �������

// DBGOUT2
#define DO2_NONE	(BitFlag8_t)0x00
#define DO2_DBGOUT	(BitFlag8_t)0x01
#define DO2_SYSLOG	(BitFlag8_t)0x02
#define DO2_STR128	(BitFlag8_t)0x04
#define DO2_ALL		(BitFlag8_t)(DO2_DBGOUT|DO2_SYSLOG|DO2_STR128)


DWORD WINAPI WorkerThread(LPVOID lpParam);
DWORD WINAPI ListenerThread(LPVOID lpParam);

struct IOCPWorkerTLSDATA
{
#ifdef _ATUM_FIELD_SERVER
	IOCPWorkerTLSDATA()
	{
		clientIndexVector.reserve(1000);
	}

	~IOCPWorkerTLSDATA()
	{
	}

	vector<ClientIndex_t>	clientIndexVector;		// �ٸ� ĳ���͵鿡�� ������ ���� �� set�� ���� buffer�� ���
#endif	// _ATUM_FIELD_SERVER_endif	
};

///////////////////////////////////////////////////////////////////////////////
/// \class		CIOCP
///
/// \brief		IOCP �ֻ��� Ŭ����
/// \author		cmkwon
/// \version	
/// \date		2004-03-17 ~ 2004-03-17
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CIOCP
{
	friend class CFieldIOCPSocket;
	friend class CNPCIOCPSocket;
	friend class CPreIOCPSocket;
	friend class CFieldMapWorkspace;
	friend class CFieldMapProject;
	friend class CIMIOCPSocket;

public:
	CIOCP(DWORD dwStartIndex, DWORD dwSessionSize, int nPort,
		char *szLocalIP="127.0.0.1", ENServerType ServerType=ST_NORMAL_SERVER);
	virtual ~CIOCP();

	///////////////////////////////////////////////////////////////////////////
	// Property
	DWORD GetServerType(void){				return m_nServerType;}
	char* GetLocalIPAddress(void){			return m_szLocalIPAddress;};
	int GetListenerPort(void){				return m_nListenerPort;}
	DWORD GetCurrentClientCounts(void){		return m_nCurrentClientCounts;}
	DWORD GetArrayClientSize(void){			return m_dwArrayClientSize;}
	BOOL GetListeningFlag(void){			return m_bListeningFlag;}
	BOOL GetServiceStartFlag(void){			return m_bServiceStartFlag;}
	BOOL GetFlagCalcTrafficInfo(void){		return m_bFlagCalcTrafficInfo;}
	void SetFlagCalcTrafficInfo(BOOL bFlag){	m_bFlagCalcTrafficInfo = bFlag;}
	void SetIOCPNetworkState(EN_NETWORK_STATE i_enNetState){	m_enIOCPNetworkState = i_enNetState;};
	EN_NETWORK_STATE GetIOCPNetworkState(void){		return m_enIOCPNetworkState;};


	///////////////////////////////////////////////////////////////////////////
	// Method
	//
	BOOL Bind(UINT nPort);
	BOOL AddIoCompletionPort(CIOCPSocket* pIOCPSocket);
	CIOCPSocket* FindEmptyIOCPSocket(DWORD dwIdx = COUNT_MAX_SOCKET_SESSION);
	CIOCPSocket* GetIOCPSocket(DWORD dwIdx);	
	void ListenerClose(void);
	DWORD GetCurrentServerTime(void);				// ������ ����� ���� ���� �ð��� �ʴ����� ��ȯ
	DWORD GetCurrentServerTimeInMilliSeconds(void);	// ������ ����� ���� ���� �ð��� Milli-Seconds ������ ��ȯ
	void GetLocalAddress(char *o_szIP);
	void LockClientArrayIndex(void);
	void UnlockClientArrayIndex(void);
	BOOL PushClientIndex(int i_nIdx);
	
	// ������ Bandwidth ��� ����
	void LockTotalTrafficInfo(void);
	void UnlockTotalTrafficInfo(void);	
	STrafficInfo *GetPTCPTrafficInfo(void);
	STrafficInfo *GetPUDPTrafficInfo(void);
	void UpdateTCPTrafficInfo(STrafficInfo *pTCPTraffic);
	void UpdateUDPTrafficInfo(STrafficInfo *pUDPTraffic);
	

	// TLS ����
	BOOL TLSDataInit(LPVOID &lpvData);
	BOOL TLSDataClean(LPVOID &lpvData);
	IOCPWorkerTLSDATA* GetIOCPWorkerTLSDATA();



	////////////////////////////////////////////////////////////////////////////
	// virtual����Լ�
	// CIOCP ��ü �ʱ�ȭ ����
	virtual BOOL IOCPInit(void);
	virtual void IOCPClean(void);

	// Thread Proc
	virtual DWORD Worker(void);
	virtual BOOL Listen(void);
	BOOL Writer(void);

	// ������ Alive Check
	virtual void CalcTotalTrafficInfo(void);
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);
	void ClientCheck(void);
	
	////////////////////////////////////////////////////////////////////////////
	// static ����Լ�
	// WinSock Library �ʱ�ȭ ����
	static BOOL SocketInit(void);
	static void SocketClean(void);

	///////////////////////////////////////////////////////////////////////////
	//
	static EN_NETWORK_STATE SeekNetworkState(DWORD i_dwMaxBandwidth, DWORD i_dwCurrentBandwidth);
	
	///////////////////////////////////////////////////////////////////////////
	// Monitor
	void SendMessageToMonitor(BYTE *pData, int nDataSize);
	void SendMessageToAdminTool(BYTE *pData, int nDataSize);
	BOOL InsertMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket);
	BOOL DeleteMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket);

protected:
	DWORD				m_nServerType;							// IOCP Server Type, �������� ���ڷ� ������										
	HANDLE				m_hCompletionPort;						// IOCompletionPort �ڵ�	
	HANDLE				m_hWorkerThread[COUNT_MAX_IOCP_THREAD];	// Worker Thread �ڵ� �迭
	HANDLE				m_hThreadSocketWriteArray[COUNT_SOCKET_WRITE_THREAD];	// ���� Socket�� ����Ÿ�� Write�ϴ� ������ �ڵ� �迭
	BOOL				m_bThreadSocketWriteEndFlag;			// Write Thread ���� �÷���

	///////////////////////////////////////////////////////////////////////////
	// ���� Listenning ����
	HANDLE				m_hListenerThread;						// ������ ������ �ڵ�
	int					m_nListenerPort;						// Listener port, Open�� ��Ʈ Ȥ�� Opened ��Ʈ
	SOCKET				m_hListener;							// ������ ���� �ڵ�	
	BOOL				m_bListeningFlag;						// �����ʰ� �����尡 ���������� �۵��ϴ����� �÷���
	BOOL				m_bServiceStartFlag;					// ���� �÷���(FALSE�̸� ���� ������, TRUE�̸� ��������)	
	
	DWORD				m_nCurrentClientCounts;					// ���� ����� IOCPSocket Counts
	CIOCPSocket			*m_pArrayIOCPSocket;					// CIOCPSocket�� ������, �����ڿ��� �������� �ִ� ������ ��ŭ �����Ǿ� �Ҵ�ȴ�.
	CIOCPSocket*		m_ArrayClient[COUNT_MAX_SOCKET_SESSION];// CIOCPSocket�� ������ �迭�� ���� m_pArrayIOCPSocket�� ���ϰ� ����ϱ� ���� ����

	CQueueINT			m_queueClientIndex;
	DWORD				m_dwArrayClientSize;					// �ִ� ���� �����ڼ�, �������� ���ڷ� ����

	DWORD				m_dwStartClientIndex;					// ������ �ʴ� IOCPSocket�� ��û�� �̺������� ū Index�� �Ҵ��
	DWORD				m_dwLastClientIndex;					// IOCPSocket�� �������� �Ҵ��ϱ� ���Ѻ���
	int					m_dwWorkerCount;						// ����� Worker ������ ����, Worker Thread count = CPU_NUM + 2
	CRITICAL_SECTION	m_crtlClientArray;						// ���� �迭�� �ε������� ����ȭ�� ���� criticalsection
	
	DWORD				m_dwTimeStarted;						// ������ ����� �ð�, �ʴ���
	DWORD				m_dwTickStarted;						// ������ ����� �ð�, �и����������
	char				m_szLocalIPAddress[SIZE_MAX_IPADDRESS];	// ������ ����� PC�� LocalIP Address
	
	
	///////////////////////////////////////////////////////////////////////////
	// TLS ����
	DWORD				m_dwTlsIndex;							// For TLS


	///////////////////////////////////////////////////////////////////////////
	// Bandwidth ��� ����
	CRITICAL_SECTION	m_crtlTotalTrafficInfo;					// Bandwidth ����� ���� �������� ����ȭ�� ���� ũ��Ƽ�� ����
	BOOL				m_bFlagCalcTrafficInfo;					// Bandwidth ����� �Ұ������� �÷���
	STrafficInfo		m_TCPTrafficInfo[2];					// TCP ��� Bandwidth ����� ���� ����, �ι�°�� ���Ȱ���� ����ȴ�.
	STrafficInfo		m_UDPTrafficInfo[2];					// UDP ��� Bandwidth ����� ���� ����, �ι�°�� ���Ȱ���� ����ȴ�.
	

	///////////////////////////////////////////////////////////////////////////
	// ������ üũ ����
	CThreadCheck		m_IOCPThreadCheck;						// Worker Ȥ�� Listener �����尡 �װų� Block ���¸� üũ�ϱ����� ����

	///////////////////////////////////////////////////////////////////////////
	// IOCP Server ��Ŷ ���� �ý��� ����
	EN_NETWORK_STATE	m_enIOCPNetworkState;
	UINT				m_uTrafficMaxBandwidth;					// ��Ŷ ���� �ý��ۿ��� ����� ���� Bandwidth�� ������ bps(bit per second)

	///////////////////////////////////////////////////////////////////////////
	// static ��� ����
	static BOOL			m_bSocketInitFlag;						// WinSock Library �ʱ�ȭ �÷���

	///////////////////////////////////////////////////////////////////////////
	// Monitor
	mtvectorIOCPSocket	m_MonitorIOCPSocketPtrVector;

};

#endif // !defined(AFX_IOCPSOCKET_H__57FB97C3_C117_4E67_99B3_4CA554A27807__INCLUDED_)

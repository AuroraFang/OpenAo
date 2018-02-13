#pragma once

#include <thread>
#include <timeapi.h>

#include "IOCPSocket.h"
#include "ThreadCheck.h"
#include "QueueINT.h"


constexpr auto COUNT_MAX_IOCP_THREAD = 500;		// IOCP ������ �ִ� ����, IOCP ������ �迭�� ũ��, ���� �����Ǵ� ������ ������ �ƴ�
constexpr auto COUNT_SOCKET_WRITE_THREAD = 50;		// IOCP Wirte Operation�� ���� �������

enum : BitFlag8_t
{
	DO2_NONE = 0x00,
	DO2_DBGOUT = 0x01,
	DO2_SYSLOG = 0x02,
	DO2_STR128 = 0x04,
	DO2_ALL = (DO2_DBGOUT | DO2_SYSLOG | DO2_STR128)
};

DWORD WINAPI WorkerThread(void* lpParam);
DWORD WINAPI ListenerThread(void* lpParam);

struct IOCPWorkerTLSDATA
{
#ifdef _ATUM_FIELD_SERVER
	vector<ClientIndex_t> clientIndexVector;		// �ٸ� ĳ���͵鿡�� ������ ���� �� set�� ���� buffer�� ���

	IOCPWorkerTLSDATA() : clientIndexVector { } { clientIndexVector.reserve(1000); }
#endif
};

class CIOCP
{
	friend class CFieldIOCPSocket;
	friend class CNPCIOCPSocket;
	friend class CPreIOCPSocket;
	friend class CFieldMapWorkspace;
	friend class CFieldMapProject;
	friend class CIMIOCPSocket;

public:
	CIOCP(DWORD dwStartIndex, DWORD dwSessionSize, int nPort, char *szLocalIP, ENServerType ServerType, size_t SizeOfSocket);
	
	virtual ~CIOCP()
	{
		DeleteCriticalSection(&m_crtlClientArray);
		DeleteCriticalSection(&m_crtlTotalTrafficInfo);
	}

	///////////////////////////////////////////////////////////////////////////
	// Property
	DWORD GetServerType() const { return m_nServerType; }
	const char* GetLocalIPAddress() const { return  m_szLocalIPAddress; } // todo : check this out, temporaray workaround

	int GetListenerPort() const { return m_nListenerPort; }
	DWORD GetCurrentClientCounts() const { return m_nCurrentClientCounts; }
	DWORD GetArrayClientSize() const { return m_dwArrayClientSize; }
	bool GetListeningFlag() const { return m_bListeningFlag; }
	bool GetServiceStartFlag() const { return m_bServiceStartFlag; }

	bool GetFlagCalcTrafficInfo() const { return m_bFlagCalcTrafficInfo; }
	void SetFlagCalcTrafficInfo(bool bFlag) { m_bFlagCalcTrafficInfo = bFlag; }

	EN_NETWORK_STATE GetIOCPNetworkState() const { return m_enIOCPNetworkState; };
	void SetIOCPNetworkState(EN_NETWORK_STATE i_enNetState) { m_enIOCPNetworkState = i_enNetState; };

	BOOL Bind(UINT nPort);
	BOOL AddIoCompletionPort(CIOCPSocket* pIOCPSocket) const;
	CIOCPSocket* FindEmptyIOCPSocket(DWORD dwIdx = 0xFFFFFFFF); // todo : fix this temporary workaround // COUNT_MAX_SOCKET_SESSION);
	CIOCPSocket* GetIOCPSocket(DWORD dwIdx) const;
	
	// this call may block
	void ListenerClose();
	
	DWORD GetCurrentServerTime() const { return m_dwTimeStarted + (timeGetTime() - m_dwTickStarted) / 1000; }	// ������ ����� ���� ���� �ð��� �ʴ����� ��ȯ
	DWORD GetCurrentServerTimeInMilliSeconds() const { return timeGetTime() - m_dwTickStarted; }	// ������ ����� ���� ���� �ð��� Milli-Seconds ������ ��ȯ
	
	static void GetLocalAddress(char *o_szIP);
	
	void LockClientArrayIndex() { EnterCriticalSection(&m_crtlClientArray); }
	void UnlockClientArrayIndex() { LeaveCriticalSection(&m_crtlClientArray); }
	
	BOOL PushClientIndex(int i_nIdx);
	
	// DevX manages bandwidth

	void LockTotalTrafficInfo() { EnterCriticalSection(&m_crtlTotalTrafficInfo); }
	void UnlockTotalTrafficInfo() { LeaveCriticalSection(&m_crtlTotalTrafficInfo); }

	STrafficInfo *GetPTCPTrafficInfo() { return &m_TCPTrafficInfo[1]; }
	STrafficInfo *GetPUDPTrafficInfo() { return &m_UDPTrafficInfo[1]; }

	void UpdateTCPTrafficInfo(STrafficInfo *pTCPTraffic) { LockTotalTrafficInfo(); m_TCPTrafficInfo[0] += *pTCPTraffic; UnlockTotalTrafficInfo(); }
	void UpdateUDPTrafficInfo(STrafficInfo *pUDPTraffic) { LockTotalTrafficInfo(); m_UDPTrafficInfo[0] += *pUDPTraffic; UnlockTotalTrafficInfo(); }

	// TLS ����

	//BOOL TLSDataInit(void* &lpvData);
	//BOOL TLSDataClean(void* &lpvData);
	static IOCPWorkerTLSDATA* GetIOCPWorkerTLSDATA() { return &workerdata; }

	thread_local static IOCPWorkerTLSDATA workerdata;

	// virtual����Լ�
	// CIOCP ��ü �ʱ�ȭ ����
	virtual BOOL IOCPInit();
	virtual void IOCPClean();

	// Thread Proc
	virtual DWORD Worker();
	virtual BOOL Listen();
	BOOL Writer();

	// ������ Alive Check
	virtual void CalcTotalTrafficInfo();
	virtual SThreadInfo *CheckIOCPThread(DWORD i_dwThreadIdToExclude);
	void ClientCheck();

	// 2010-04-08 by cmkwon, �������� �ʷε��� AlivePacket ���� �߰� - 
	virtual void SendAlivePacket2OtherServers() { } // do nothing

	// static ����Լ�
	// WinSock Library �ʱ�ȭ ����
	static bool SocketInit();
	static void SocketClean();

	// Monitor
	void SendMessageToMonitor(BYTE *pData, int nDataSize);
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	void SendMessageToMonitor(msg_t<t>& msg) { this->SendMessageToMonitor(reinterpret_cast<BYTE*>(&msg), msg.size()); }

	void SendMessageToAdminTool(BYTE *pData, int nDataSize);
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	void SendMessageToAdminTool(msg_t<t>& msg) { this->SendMessageToAdminTool(reinterpret_cast<BYTE*>(&msg), msg.size()); }

	BOOL InsertMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket);
	BOOL DeleteMonitorIOCPSocketPtr(CIOCPSocket *i_pSocket);

protected:

	DWORD				m_nServerType;							// IOCP Server Type, �������� ���ڷ� ������										
	HANDLE				m_hCompletionPort;						// IOCompletionPort �ڵ�	
	//HANDLE				m_hWorkerThread[COUNT_MAX_IOCP_THREAD];	// Worker Thread �ڵ� �迭
	//HANDLE				m_hThreadSocketWriteArray[COUNT_SOCKET_WRITE_THREAD];	// ���� Socket�� ����Ÿ�� Write�ϴ� ������ �ڵ� �迭
	thread				m_threadWorker[COUNT_MAX_IOCP_THREAD];
	thread				m_threadSocketWrite[COUNT_SOCKET_WRITE_THREAD];
	BOOL				m_bThreadSocketWriteEndFlag;			// Write Thread ���� �÷���

	// ���� Listenning ����
	//HANDLE				m_hListenerThread;						// ������ ������ �ڵ�
	thread				m_threadListener;
	int					m_nListenerPort;						// Listener port, Open�� ��Ʈ Ȥ�� Opened ��Ʈ
	SOCKET				m_hListener;							// ������ ���� �ڵ�	
	BOOL				m_bListeningFlag;						// �����ʰ� �����尡 ���������� �۵��ϴ����� �÷���
	BOOL				m_bServiceStartFlag;					// ���� �÷���(FALSE�̸� ���� ������, TRUE�̸� ��������)	

	DWORD				m_nCurrentClientCounts;					// ���� ����� IOCPSocket Counts
	CIOCPSocket*		m_pArrayIOCPSocket;					// CIOCPSocket�� ������, �����ڿ��� �������� �ִ� ������ ��ŭ �����Ǿ� �Ҵ�ȴ�.
	//CIOCPSocket*		m_ArrayClient[COUNT_MAX_SOCKET_SESSION];// CIOCPSocket�� ������ �迭�� ���� m_pArrayIOCPSocket�� ���ϰ� ����ϱ� ���� ����

	const size_t		m_SizeOfSocket;
	CIOCPSocket*		getSocket(size_t index) const { return reinterpret_cast<CIOCPSocket*>(reinterpret_cast<char*>(m_pArrayIOCPSocket) + (index * m_SizeOfSocket)); }

	CQueueINT			m_queueClientIndex;
	DWORD				m_dwArrayClientSize;					// �ִ� ���� �����ڼ�, �������� ���ڷ� ����

	DWORD				m_dwStartClientIndex;					// ������ �ʴ� IOCPSocket�� ��û�� �̺������� ū Index�� �Ҵ��
	DWORD				m_dwLastClientIndex;					// IOCPSocket�� �������� �Ҵ��ϱ� ���Ѻ���
	int					m_dwWorkerCount;						// ����� Worker ������ ����, Worker Thread count = CPU_NUM + 2
	CRITICAL_SECTION	m_crtlClientArray;						// ���� �迭�� �ε������� ����ȭ�� ���� criticalsection

	time_t				m_dwTimeStarted;						// ������ ����� �ð�, �ʴ���
	DWORD				m_dwTickStarted;						// ������ ����� �ð�, �и����������
	char				m_szLocalIPAddress[SIZE_MAX_IPADDRESS];	// ������ ����� PC�� LocalIP Address

	// TLS ����
	//DWORD				m_dwTlsIndex;							// For TLS

	// Bandwidth ��� ����
	CRITICAL_SECTION	m_crtlTotalTrafficInfo;					// Bandwidth ����� ���� �������� ����ȭ�� ���� ũ��Ƽ�� ����
	BOOL				m_bFlagCalcTrafficInfo;					// Bandwidth ����� �Ұ������� �÷���
	STrafficInfo		m_TCPTrafficInfo[2];					// TCP ��� Bandwidth ����� ���� ����, �ι�°�� ���Ȱ���� ����ȴ�.
	STrafficInfo		m_UDPTrafficInfo[2];					// UDP ��� Bandwidth ����� ���� ����, �ι�°�� ���Ȱ���� ����ȴ�.

	// ������ üũ ����
	CThreadCheck		m_IOCPThreadCheck;						// Worker Ȥ�� Listener �����尡 �װų� Block ���¸� üũ�ϱ����� ����

	// IOCP Server ��Ŷ ���� �ý��� ����
	EN_NETWORK_STATE	m_enIOCPNetworkState;
	UINT				m_uTrafficMaxBandwidth;					// ��Ŷ ���� �ý��ۿ��� ����� ���� Bandwidth�� ������ bps(bit per second)

	// static ��� ����
	static bool			m_bSocketInitFlag;						// WinSock Library �ʱ�ȭ �÷���
	
	// Monitor
	mtvectorIOCPSocket	m_MonitorIOCPSocketPtrVector;

};
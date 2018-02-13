// PreWinSocket.h: interface for the CPreWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_)
#define AFX_PREWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

#define	WM_PRE_ASYNC_EVENT				(WM_USER + 3)				//
#define	WM_PRE_PACKET_NOTIFY			(WM_USER + 4)				// HWND�� ���޵Ǵ� �޽���

class CFieldIOCP;

class CPreWinSocket : public CWinSocket
{
public:
	CPreWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify);
	virtual ~CPreWinSocket() { }

	BOOL OnCloseSocket(int nErrorCode) override;
	BOOL OnConnect(int nErrorCode) override;

	void SetFieldIOCP(CFieldIOCP* pFieldIOCP) { m_pFieldIOCP2 = pFieldIOCP; }

	// Message ó�� �Լ�
	ProcessResult Process_FP_EVENT_NOTIFY_WARP_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_CONNECT_NOTIFY_FIELDSERVER_CHANGE_OK(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_CONNECT_AUTH_USER_OK(const char* pPacket, int nLength, int &nBytesUsed);
	//ProcessResult Process_FP_MONITOR_SET_MGAME_EVENT_TYPE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_CONNECT_PREPARE_SHUTDOWN(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_MONITOR_RELOAD_VERSION_INFO_OK(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-09-08 by cmkwon, SCMonitor���� ReloadVersionInfo�ÿ� �Ϻ� üũ������(.\Res-Tex\*.*)�� ���ε��ϱ� - 


	// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
	ProcessResult Process_FP_CONNECT_UPDATE_DBSERVER_GROUP(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_CONNECT_CHECK_CONNECTABLE_ACCOUNT(const char* pPacket, int nLength, int &nBytesUsed);

	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	ProcessResult Process_FP_ADMIN_RELOAD_WORLDRANKING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_ADMIN_RELOAD_INFLUENCERATE(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-16 by cmkwon, ���� �ʱ�ȭ�� ���¡ ���� ���� - 


	ProcessResult Process_FP_EVENT_RELOAD_HAPPYEV(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_EVENT_RELOAD_ITEMEV(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_EVENT_UPDATE_PCBANGLIST(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_EVENT_UPDATE_STRATEGYPOINT_NOTSUMMONTIM(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_ADMIN_BLOCKACCOUNT_OK(const char* pPacket, int nLength, int &nBytesUsed);		// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - 
	ProcessResult Process_FP_ADMIN_UNBLOCKACCOUNT_OK(const char* pPacket, int nLength, int &nBytesUsed);	// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - 

	ProcessResult Process_FP_ADMIN_STRATRGYPOINT_INFO_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-11-13 by jhseol, ���� �ý��� ������ - ������

	// start 2012-01-08 by hskim, GLog 2��
	ProcessResult Process_FP_GLOG_CONNECT_USER_NEW(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FP_GLOG_CONNECT_USER_LOGIN(const char* pPacket, int nLength, int &nBytesUsed);
	// end 2012-01-08 by hskim, GLog 2��

	// error
	ProcessResult Process_ERROR(const char* pPacket, int nLength, int &nBytesUsed);

public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

public:
	CFieldIOCP		*m_pFieldIOCP2;
};

#endif // !defined(AFX_PREWINSOCKET_H__CE9F7F73_C6DF_4E82_A0FE_5DB8F40B2E69__INCLUDED_)

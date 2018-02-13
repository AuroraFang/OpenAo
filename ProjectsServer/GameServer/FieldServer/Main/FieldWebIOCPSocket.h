// FieldWebIOCPSocket.h: interface for the CFieldWebIOCPSocket class.
// 2013-03-13 by hskim, �� ĳ�� ����
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)
#define AFX_WEBIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FieldWebIOCP.h"
#include "AtumProtocol.h"
#include "XmlRpc.h"

#define SIZE_MAX_XMLRPC_METHODNAME							512					// �޼ҵ� �̸�
#define SIZE_MAX_XMLRPC_PARAMTYPE							100					// �Ķ���� Ÿ�� ��

#define MIN_WEB_GAME_INSERT_COUNTABLEITEM_COUNT				1					// ������ ���� : ī���ͺ� �ּ�
#define MAX_WEB_GAME_INSERT_COUNTABLEITEM_COUNT				20000000			// ������ ���� : ī���ͺ� �ִ�

#define MIN_WEB_GAME_INSERT_NONCOUNTABLEITEM_COUNT			1					// ������ ���� : ��ī���ͺ� �ּ�
#define MAX_WEB_GAME_INSERT_NONCOUNTABLEITEM_COUNT			10					// ������ ���� : ��ī���ͺ� �ִ�

#define MIN_WEB_GAME_DELETE_COUNTABLEITEM_COUNT				1					// ������ ȸ�� : ī���ͺ� �ּ�
#define MAX_WEB_GAME_DELETE_COUNTABLEITEM_COUNT				20000000			// ������ ȸ�� : ī���ͺ� �ִ�

#define MIN_WEB_GAME_DELETE_NONCOUNTABLEITEM_COUNT			1					// ������ ȸ�� : ��ī���ͺ� �ּ�
#define MAX_WEB_GAME_DELETE_NONCOUNTABLEITEM_COUNT			10000				// ������ ȸ�� : ��ī���ͺ� �ִ�

#define MIN_WEB_BILLING_BUY_COUNT							1					// ������ ���� : �ּ�
#define MAX_WEB_BILLING_BUY_COUNT							10					// ������ ���� : �ִ�


typedef struct
{
	char szMethodName[SIZE_MAX_XMLRPC_METHODNAME];
	int (CFieldWebIOCPSocket::*pFunction)(XmlRpc::XmlRpcValue &rParamsValue);
	int ParamCount;
	int ParamType[SIZE_MAX_XMLRPC_PARAMTYPE];

} XMLRPC_METHOD_INFO;

class CFieldIOCP;

class CFieldWebIOCPSocket : public CIOCPSocket
{
	friend class CFieldWebIOCP;
	
public:
	CFieldWebIOCPSocket();
	~CFieldWebIOCPSocket();

public:
	BOOL OnRecvdPacketFieldWebServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP = "", int nPeerPort = 0, SThreadInfo *i_pThreadInfo=NULL);
	void OnConnect(void);
	void OnClose(int reason = 0);

	// XMLRPC �������� ���� �޼ҵ�
	int Process_XMLRPC_METHOD_GET_SERVER_STATUS(XmlRpc::XmlRpcValue &rParamsValue);

	// Game ���� �޼ҵ�
	int Process_XMLRPC_METHOD_GAME_ITEM_INSERT(XmlRpc::XmlRpcValue &rParamsValue);
	int Process_XMLRPC_METHOD_GAME_ITEM_DELETE(XmlRpc::XmlRpcValue &rParamsValue);
	int Process_XMLRPC_METHOD_BILLING_BUY_ITEM(XmlRpc::XmlRpcValue &rParamsValue);
	int Process_XMLRPC_METHOD_BILLING_GET_CASH(XmlRpc::XmlRpcValue &rParamsValue);

protected:
	CFieldIOCP *GetFieldIOCP()				{ return ms_pFieldWebIOCP->m_pFieldIOCP; }
	CFieldWebIOCP *GetFieldWebIOCP()		{ return ms_pFieldWebIOCP; }

private:
	static CFieldWebIOCP		*ms_pFieldWebIOCP;

	CXmlRpc		m_XmlRpc;
};

#endif // !defined(AFX_WEBIOCPSOCKET_H__9C62870A_F7ED_49A2_9583_956845931320__INCLUDED_)

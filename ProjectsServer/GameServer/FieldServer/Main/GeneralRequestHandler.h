#ifndef _GENERAL_REQUEST_HANDLER_H_
#define _GENERAL_REQUEST_HANDLER_H_

#define FC_REQUEST_TIMEOUT_INTERVAL		30000

class CFieldIOCPSocket;

class CGeneralRequestHandler  
{
public:
	CGeneralRequestHandler();

	void InitGeneralRequestHandler(CFieldIOCPSocket *i_pFieldIOCPSocket);

	void Reset();
	void SendCancelRequsetAndReset(BOOL i_bSendToPeer = TRUE);	// Reset() ������ ȣ��Ǿ�� ��

	BOOL IsRequestableAndResetIfTimeOut(TimeUnit_t i_TimeOut = FC_REQUEST_TIMEOUT_INTERVAL);
	BYTE GetWaitingRequestType();	// returns, REQUEST_TYPE_XXX
	void SetPeerRequest(BYTE i_RequestType, ClientIndex_t i_PeerRequesterClientIndex);
	ClientIndex_t GetPeerClientIndex();

private:
	BYTE				m_RequestType;				// REQUEST_TYPE_XXX
	ClientIndex_t		m_PeerRequesterClientIndex;	// ���� ClientIndex
	TimeUnit_t			m_RequestTimeStamp;			// ��û �ð�

	CFieldIOCPSocket	*m_pFieldIOCPSocket;
};

#endif // _GENERAL_REQUEST_HANDLER_H_

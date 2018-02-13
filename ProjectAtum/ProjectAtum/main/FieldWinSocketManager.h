// FieldWinSocketManager.h: interface for the CFieldWinSocketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDWINSOCKETMANAGER_H__52C754A0_683B_4C24_B626_84476785ADFA__INCLUDED_)
#define AFX_FIELDWINSOCKETMANAGER_H__52C754A0_683B_4C24_B626_84476785ADFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldWinSocketManager  
{
public:
	CFieldWinSocketManager(HWND hwnd);
	virtual ~CFieldWinSocketManager();

	BOOL Connect(LPCSTR strPeerIP, int nPort);
	BOOL CloseSocket();
	BOOL Write(LPCSTR pPacket, int nLength);
	BOOL Write(BYTE *pPacket, int nLength) ;
	BOOL SendMsg( int nType, char *pPacket, int nSize );
	BOOL WriteMessageType(MessageType_t msgType);

	BOOL IsConnected();	
	LONG OnAsyncEvent(LONG lParam);
	LONG OnArenaAsyncEvent(LONG lParam);
	//int Read(LPSTR *pPacket, int &nLength);

	// �Ʒ��� ���� ����
	BOOL ConnectArena(LPCSTR strPeerIP, int nPort, BOOL* bAlReadyConnect);
	BOOL CloseSocketArena();

	// �Ʒ��� �ʵ����
	CFieldWinSocket* GetFieldWinSocket(UINT nSocketNotifyType);
	CFieldWinSocket* GetArenaFieldWinSocket();

	// �Ʒ��� ���� ���� ���� 
	BOOL IsFCConnectedArenaServer();

	BOOL IsRequestArenaSocketClose();

private:
	BOOL IsArenaLoadSuccess();

	
private:
	CFieldWinSocket *			m_pMainFieldWinSocket;				// ���� ���� ����
	CFieldWinSocket *			m_pArenaFieldWinSocket;				// �Ʒ��� ���� ����

	HWND						m_hMainWnd;

	BOOL						m_bRequestArenaSocketClose;			// ���� ��û�� ���� Ŭ�ο����?

};

#endif // !defined(AFX_FIELDWINSOCKETMANAGER_H__52C754A0_683B_4C24_B626_84476785ADFA__INCLUDED_)

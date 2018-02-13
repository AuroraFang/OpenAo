// FieldWinSocketManager.cpp: implementation of the CFieldWinSocketManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FieldWinSocket.h"
#include "AtumApplication.h"

#include "FieldWinSocketManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldWinSocketManager::CFieldWinSocketManager(HWND hwnd)
{		
	m_hMainWnd = hwnd;

	//CFieldWinSocket(HWND hWnd, UINT wmSock = WM_ASYNC_EVENT, UINT wmNotify = WM_PACKET_NOTIFY);
	m_pMainFieldWinSocket = new CFieldWinSocket(hwnd, WM_ASYNC_EVENT, WM_PACKET_NOTIFY);	
	m_pArenaFieldWinSocket = NULL;
	g_pFieldWinSocket = this;

	// ��û�� ���� ���� �����?
	m_bRequestArenaSocketClose = FALSE;

	// 2009. 06. 17 by ckPark Frost System
#if defined( DEFINE_USE_GAMEGUARD_FROST )
	m_pMainFieldWinSocket->SetFrostEnable( TRUE );
#endif
	// end 2009. 06. 17 by ckPark Frost System
}

CFieldWinSocketManager::~CFieldWinSocketManager()
{
	g_pFieldWinSocket = NULL;
	util::del(m_pMainFieldWinSocket);
	util::del(m_pArenaFieldWinSocket);	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���� ����
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::Connect(LPCSTR strPeerIP, int nPort)
{
	return m_pMainFieldWinSocket->Connect(strPeerIP, nPort);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���μ����� �Ʒ��� ������ ���Ӳ���
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::CloseSocket()
{
	DBGOUT("BOOL CFieldWinSocketManager::CloseSocket() \n");
	if(IsFCConnectedArenaServer())
	{
		// �Ʒ��� ���� Close
		CloseSocketArena();
	}
	return m_pMainFieldWinSocket->CloseSocket();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���¸� �Ǵ��Ͽ� ��Ŷ�� ��� ������ ���Ѵ�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::Write(LPCSTR pPacket, int nLength)
{
	if(IsArenaLoadSuccess())
	{
		if(IsFCConnectedArenaServer())
		{
			return m_pArenaFieldWinSocket->Write(pPacket, nLength);		
		}
	}
	else
	{
		return m_pMainFieldWinSocket->Write(pPacket, nLength);	
	}
	return 0;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���¸� �Ǵ��Ͽ� ��Ŷ�� ��� ������ ���Ѵ�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::Write(BYTE *pPacket, int nLength)
{
	if(IsArenaLoadSuccess())
	{
		if(IsFCConnectedArenaServer())
		{
			return m_pArenaFieldWinSocket->Write(pPacket, nLength);		
		}		
	}	
	else
	{
		return m_pMainFieldWinSocket->Write(pPacket, nLength);
	}
	return TRUE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		��Ŷ���� ��� ������ �����Ѵ�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::SendMsg( int nType, char *pPacket, int nSize )
{	
	BOOL bArenaSend = FALSE;
	BOOL bBothSend = FALSE;		// ���� �� ������ �޽���
	switch(nType)
	{	
	case T_FC_CONNECT_ARENASERVER_LOGIN:		// �Ʒ��� �ʵ� ���� �α���
	case T_FC_ARENA_REQUEST_TEAM:				// �Ʒ��� ����û
	case T_FC_ARENA_CREATE_TEAM:				// �Ʒ��� �� ����
	case T_FC_ARENA_ENTER_TEAM:					// �� ����
	case T_FC_ARENA_REAVE_TEAM:					// �� Ż��	
	case T_FC_ARENA_ENTER_ROOM_WARP:			// �Ʒ��� ������ �̵�	
	case T_FC_ARENA_REQUEST_CREATE_TEAMINFO:	// �Ʒ��� ��
	case T_FC_ARENA_QUICK_ENTER_TEAM:			// ���� ��û
	case T_FC_ARENA_TEAM_READY_FINISH:			// �Ʒ��� �� ����
	case T_FC_ARENA_TEAM_READY_FINISH_CANCEL:	// �Ʒ��� �� ���� ���
	case T_FC_ARENA_CHARACTER_GAMESTART:		// Ŭ���̾�Ʈ�� �Ʒ��� ���� ��ŸƮ 		
	case T_FC_ARENA_FINISH_WARP:				// �Ʒ��� ���� ������ �̵�
	case T_FC_CONNECT_ARENASERVER_TO_IMSERVER:				// �Ʒ��� ���� IM�������� ������ �Ϸ�Ǿ���.
		{
			bArenaSend = TRUE;			
		}
		break;		
	case T_FC_CHARACTER_GAMESTART_FROM_ARENA_TO_MAINSERVER: // �Ʒ��� ���Ḧ �˷���		
	// 2010. 04. 22 by ckPark ��ŷ�� ���� ��Ŷ�� �Ʒ����� �ƴ� ���� �ʵ� ������ �������� ����
	case T_FC_MOVE_HACKSHIELD_CRCAckMsg:
	case T_FC_MOVE_HACKSHIELD_HACKING_CLIENT:
	// end 2010. 04. 22 by ckPark ��ŷ�� ���� ��Ŷ�� �Ʒ����� �ƴ� ���� �ʵ� ������ �������� ����
		{
			bArenaSend = FALSE;
		}
		break;		
	case T_FC_CHARACTER_GAMEEND:// ���� ����
	case T_FC_CHARACTER_GET_CHARACTER:	// ĳ���� ���� ��û
		{
		}
		break;
	case T_FC_CONNECT_ALIVE:// ������ ���� ����ִ�.
	case T_FC_ARENA_PLAY_CHARACTER_TYPE:	// 2012-06-04 by mspark, �Ʒ��� ��� ���� UI �۾�
		{
			bArenaSend = TRUE;			
			bBothSend = TRUE;
		}
		break;

	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
		case T_FC_INFINITY_MODE_LIST:
		case T_FC_INFINITY_READY_LIST:
		case T_FC_INFINITY_CREATE:
		case T_FC_INFINITY_MEMBER_INFO_LIST:
		case T_FC_INFINITY_LEAVE:
		case T_FC_INFINITY_BAN:
		case T_FC_INFINITY_JOIN:
		case T_FC_INFINITY_JOIN_REQUEST_MASTERUSER_OK:
		case T_FC_INFINITY_READY:
		case T_FC_INFINITY_READY_CANCEL:
		case T_FC_INFINITY_START:
		case T_FC_INFINITY_MAP_LOADED:
		case T_FC_INFINITY_FIN_OK:
		case T_FC_INFINITY_JOIN_CANCEL:
		case T_FC_INFINITY_RESTART_BY_DISCONNECT:
		case T_FC_INFINITY_MAP_LOADED_RESTART_BY_DISCONNECT:

		// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
		case T_FC_INFINITY_CHANGE_DIFFICULTY_LEVEL :

		// 2010. 07. 27 by hsLee ���Ǵ�Ƽ 2�� ���� ��� �ó׸� ���� ��ŵ ó��.
		case T_FC_INFINITY_SKIP_ENDING_CINEMA :

		{
			bArenaSend = TRUE;
		}
		break;
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���

	default:
		{
			if(IsArenaLoadSuccess())
			{
				bArenaSend = TRUE;
			}
		}
		break;
	}
	if(bArenaSend)
	{
		BOOL bRtn = FALSE;
		if(IsFCConnectedArenaServer())
		{			
			bRtn = m_pArenaFieldWinSocket->SendMsg( nType, pPacket, nSize );					
		}		
		if(FALSE == bBothSend)
		{
			// ���� �� ������ �޽����� �ƴϸ� ����
			return bRtn;
		}		
		return m_pMainFieldWinSocket->SendMsg( nType, pPacket, nSize );
	}
	else
	{
		return m_pMainFieldWinSocket->SendMsg( nType, pPacket, nSize );		
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���¸� �Ǵ��Ͽ� ��Ŷ�� ��� ������ ���Ѵ�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::WriteMessageType(MessageType_t msgType)
{
	if(IsArenaLoadSuccess())
	{
		if(IsFCConnectedArenaServer())
		{
			return m_pArenaFieldWinSocket->WriteMessageType(msgType);		
		}
	}
	else
	{
		return m_pMainFieldWinSocket->WriteMessageType(msgType);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���� ���Ῡ�� �Ǵ�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::IsConnected()
{
	return m_pMainFieldWinSocket->IsConnected();
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� ���� ���� ������ ���� �Լ�
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
LONG CFieldWinSocketManager::OnAsyncEvent(LONG lParam)
{	
	return m_pMainFieldWinSocket->OnAsyncEvent(lParam);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� �������� ������ ���� �Լ�
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
LONG CFieldWinSocketManager::OnArenaAsyncEvent(LONG lParam)
{	
	if(NULL == m_pArenaFieldWinSocket)
	{
		return 0;
	}
	return m_pArenaFieldWinSocket->OnAsyncEvent(lParam);
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� �����Ϳ��� �õ�
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::ConnectArena(LPCSTR strPeerIP, int nPort, BOOL* bAlReadyConnect)
{	
	if(NULL == m_pArenaFieldWinSocket)
	{
		m_pArenaFieldWinSocket = new  CFieldWinSocket(m_hMainWnd, WM_ASYNC_ARENA_EVENT, WM_PACKET_ARENA_NOTIFY);
	}
	(*bAlReadyConnect) = FALSE;
	if(m_pArenaFieldWinSocket->IsConnected())
	{
		(*bAlReadyConnect) = TRUE;
		return TRUE;
	}
	m_bRequestArenaSocketClose = FALSE;
	BOOL bConnect = m_pArenaFieldWinSocket->Connect(strPeerIP, nPort);

	return bConnect;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� ������ ���Ӳ���
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::CloseSocketArena()
{
	DBGOUT("BOOL CFieldWinSocketManager::CloseSocketArena() \n");

	m_bRequestArenaSocketClose = TRUE;
	BOOL bRtn = TRUE;
	if(IsFCConnectedArenaServer())
	{
		bRtn = m_pArenaFieldWinSocket->CloseSocket();
	}	
	return bRtn;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		���� Ÿ���� ���� ��� �������� �˷��ִ� �Լ�
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldWinSocket* CFieldWinSocketManager::GetFieldWinSocket(UINT nSocketNotifyType)
{
	if(WM_PACKET_ARENA_NOTIFY == nSocketNotifyType)
	{
		return GetArenaFieldWinSocket();
	}
	return m_pMainFieldWinSocket;
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� ������ �����Ѵ�.
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
CFieldWinSocket* CFieldWinSocketManager::GetArenaFieldWinSocket()
{
	return m_pArenaFieldWinSocket;	
}
///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� ������ ����Ǿ��ִ��� ���� �Ǵ�
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::IsFCConnectedArenaServer()
{
	if(NULL == m_pArenaFieldWinSocket)
	{
		return FALSE;
	}
	return m_pArenaFieldWinSocket->IsConnected();
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� �����̳� �Ǵ�
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::IsArenaLoadSuccess()
{
	if((g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_LOAD_GAME_INFO )
		|| (g_pD3dApp->GetArenaState() == ARENA_STATE_ARENA_GAMING ))
	{
		return TRUE;
	}
	return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		�Ʒ��� �����̳� �Ǵ�
/// \author		// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
/// \date		2008-01-30 ~ 2008-01-30
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
BOOL CFieldWinSocketManager::IsRequestArenaSocketClose()
{
	return m_bRequestArenaSocketClose;
}
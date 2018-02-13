// N2FSocket.h: interface for the CN2FSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N2FSOCK_H__3D8B9313_775E_4A34_98DE_E5373AEC1FFA__INCLUDED_)
#define AFX_N2FSOCK_H__3D8B9313_775E_4A34_98DE_E5373AEC1FFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

#define	WM_N2F_ASYNC_EVENT				(WM_USER + 1)				// 
#define	WM_N2F_PACKET_NOTIFY			(WM_USER + 2)				// HWND�� ���޵Ǵ� �޽���




class CNPCIOCP;
class CN2FSocket : public CWinSocket  
{
public:
	CN2FSocket(HWND hWnd, UINT wmSock, UINT wmNotify);
	virtual ~CN2FSocket();

	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnConnect(int nErrorCode);

	void SetNPCIOCP(CNPCIOCP *pNPCIOCP);

	ProcessResult Process_FN_NPCSERVER_SUMMON_JACO_MONSTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL(const char* pPacket, int nLength, int &nBytesUsed);	// 2007-08-22 by cmkwon, �ش� ��ä�� ���� ��� �����ϱ� ��� �߰�
	ProcessResult Process_FN_NPCSERVER_CINEMA_MONSTER_CREATE(const char* pPacket, int nLength, int &nBytesUsed);	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Key���� ����
	ProcessResult Process_FN_NPCSERVER_CINEMA_MONSTER_DESTROY(const char* pPacket, int nLength, int &nBytesUsed);	// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	ProcessResult Process_FN_NPCSERVER_CINEMA_MONSTER_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);	// 2011-05-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	ProcessResult Process_FN_NPCSERVER_CINEMA_MONSTER_REGEN(const char* pPacket, int nLength, int &nBytesUsed);		// 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����
	ProcessResult Process_FN_NPCSERVER_NEW_CHANGE_OBJECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!! 
	ProcessResult Process_FN_NPCSERVER_RESET_CHANGE_OBJECT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!! 
	
public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

public:
	CNPCIOCP		*m_pNPCIOCP1;
};


#endif // !defined(AFX_N2FSOCK_H__3D8B9313_775E_4A34_98DE_E5373AEC1FFA__INCLUDED_)

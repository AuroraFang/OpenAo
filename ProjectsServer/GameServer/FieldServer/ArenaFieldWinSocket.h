// ArenaFieldWinSocket.h: interface for the CArenaFieldWinSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARENAFIELDWINSOCKET_H__078F259B_A907_4803_AE97_42DA4107846C__INCLUDED_)
#define AFX_ARENAFIELDWINSOCKET_H__078F259B_A907_4803_AE97_42DA4107846C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinSocket.h"

#define	WM_FIELD_ASYNC_EVENT			(WM_USER_BASE + 7)
#define	WM_FIELD_PACKET_NOTIFY			(WM_USER_BASE + 8)

class CFieldIOCP;
class CFieldIOCPSocket;
class CArenaFieldWinSocket : public CWinSocket 
{
public:
	CArenaFieldWinSocket(HWND hWnd, UINT wmSock, UINT wmNotify);
	virtual ~CArenaFieldWinSocket();

	BOOL OnCloseSocket(int nErrorCode);
	BOOL OnConnect(int nErrorCode);

	void SetFieldIOCP(CFieldIOCP *pFieldIOCP);
	BOOL CheckInvalidCharacter(CFieldIOCPSocket * i_pFISoc, UID32_t i_AccountUID, UID32_t i_CharacterUID);

	ProcessResult Process_FtoA_AUTH_CHARACTER(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_STATE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_TEAM_MATCHING(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_SERVER_PAY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_CHARACTER_PAY(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_CHARACTER_DISCONNECT(const char* pPacket, int nLength, int &nBytesUsed);
	ProcessResult Process_FtoA_ARENA_CHARACTERARENAINFO_UPDATA(const char* pPacket, int nLength, int &nBytesUsed);	// 2012-04-12 by jhseol, �Ʒ��� �߰����� - ���� : AS���� ���� CharacterArena ���� ������Ʈ
	ProcessResult Process_FtoA_INFINITY_START(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� MainFieldServer�� �˸�, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_IMPUTE(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� �ͼ� ���� ����, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_UPDATE_USER_INFO(const char* pPacket, int nLength, int &nBytesUsed);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ���� ����ȭ ���� ����, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_UPDATE_ALL_ITEM_COUNT(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_DELETE_ITEM(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_UPDATE_ITEM	(const char* pPacket, int nLength, int &nBytesUsed);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_INSERT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� ������ ����ȭ ���� ����, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_LOG(const char* pPacket, int nLength, int &nBytesUsed);						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� �α� ����, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_USING_TIME_LIMIT_ITEM(const char* pPacket, int nLength, int &nBytesUsed);						// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� MainFieldServer�� �α� ����, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_STATE_CHANGE(const char* pPacket, int nLength, int &nBytesUsed);		// 2009-09-09 ~ 2010-01-20 by dhjin, ���Ǵ�Ƽ - ���� ���� ���� ���μ����� �����Ѵ�. ���� ���� üũ
	ProcessResult Process_FtoA_INFINITY_START_CHECK(const char* pPacket, int nLength, int &nBytesUsed);		// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 
	ProcessResult Process_FtoA_INFINITY_UPDATE_USER_MAP_INFO(const char* pPacket, int nLength, int &nBytesUsed);	// 2010-04-06 by cmkwon, ����2�� �߰� ���� - 
	ProcessResult Process_FtoA_INFINITY_TENDERITEM_LOG(const char* pPacket, int nLength, int &nBytesUsed);			// 2010-06-25 by shcho, ���Ǵ�Ƽ ���÷α� ��� - ���� ������ ���� DB����		
	ProcessResult Process_FtoA_INFINITY_COME_BACK_POSTWORK(const char* pPacket, int nLength, int &nBytesUsed);		// 2012-01-16 by hskim, ��� - ȭ�� ���� MainFieldServer�� ���ư��� ���� �� �۾� ó�� ���, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_COME_BACK_PREWORK(const char* pPacket, int nLength, int &nBytesUsed);		// 2011-09-29 by hskim, ��Ʈ�� �ý��� 2�� - ���� MainFieldServer�� ���ư��� ���� ���� �۾� ó�� ���, AFS -> MFS
	ProcessResult Process_FtoA_INFINITY_UPDATE_ITEM_PET(const char* pPacket, int nLength, int &nBytesUsed);			// 2011-09-30 by hskim, ��Ʈ�� �ý��� 2�� - ���� MainFieldServer�� ��Ʈ�� ������ ����ȭ ���� ����, AFS -> MFS

	// error
	ProcessResult Process_ERROR(const char* pPacket, int nLength, int &nBytesUsed);

public:
	virtual BOOL OnRecvdPacket(LPSTR pPacket, int nLength, BYTE nSeq);

public:
	CFieldIOCP		*m_pFieldIOCP20;
};

#endif // !defined(AFX_ARENAFIELDWINSOCKET_H__078F259B_A907_4803_AE97_42DA4107846C__INCLUDED_)

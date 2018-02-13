// IMGlobal.h: interface for the CIMGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGLOBAL_H__3195FC3F_0985_4E7C_88E3_9F768B9CB2DD__INCLUDED_)
#define AFX_IMGLOBAL_H__3195FC3F_0985_4E7C_88E3_9F768B9CB2DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlobalGameServer.h"
class CIMGlobal;
extern CIMGlobal *			g_pIMGlobal;

///////////////////////////////////////////////////////////////////////////////
/// \class		CIMGlobal
///
/// \brief		
/// \author		cmkwon
/// \version	
/// \date		2004-03-19 ~ 2004-03-19
/// \warning	
///////////////////////////////////////////////////////////////////////////////
class CIMIOCP;
class CPreIMWinSocket;
class CIMGlobal : public CGlobalGameServer  
{
public:
	CIMGlobal();
	virtual ~CIMGlobal();

	///////////////////////////////////////////////////////////////////////////
	// Property
	UINT GetTimerIDServerGroupInfo(void);
	CIMIOCP * GetIMIOCPPtr(void);
	CPreIMWinSocket * GetIM2PreWinSocket(void);

	CSystemLogManager * GetPtrChattingLogManager(void);

	///////////////////////////////////////////////////////////////////////////
	// Method
	// 2007-11-13 by cmkwon, ������, ��� ä�� �α� �����ϱ� - �����߰�(, USHORT i_usRace)
	BOOL WriteChattingLog(char * i_szChatting, USHORT i_usRace);
	BOOL CreateIM2PreWinSokcet(HWND i_hWnd);
	void DestroyIM2PreWinSocket(void);
	BOOL StartTimerSendServerGroupInfo(void);
	void EndTimerSendServerGroupInfo(void);
	
	// virtual Function
	bool InitServerSocket() override;				// ���ϰ��� �ʱ�ȭ �Լ�
	bool EndServerSocket() override;					// ���ϰ��� ���� �Լ�

	void VMemAlloc() override;

	virtual BOOL LoadConfiguration(void);				// Server ������ ���� Configuration Load

	BOOL		m_bArenaIMServerCheck;					// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : �Ʒ��� �������� üũ

protected:
	//////////////////////////////////////////////////////////////////////////
	// ������ Ÿ�̹� ���� ��� ����
	UINT				m_nTimerIDServerGroupInfo;		// 

	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	CSystemLogManager	m_ChattingLogManager;							// IM ���� Chatting �� ���� �ϱ� ���� ȭ��

	CPreIMWinSocket *	m_pIM2PreWinSocket;
};

#endif // !defined(AFX_IMGLOBAL_H__3195FC3F_0985_4E7C_88E3_9F768B9CB2DD__INCLUDED_)

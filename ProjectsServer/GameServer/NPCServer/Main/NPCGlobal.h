#pragma once

#include "d3dapp.h"

class CNPCGlobal;
extern CNPCGlobal *			g_pNPCGlobal;

///////////////////////////////////////////////////////////////////////////////
/// \class		CNPCGlobal
///
/// \brief
/// \author		cmkwon
/// \version
/// \date		2004-03-19 ~ 2004-03-19
/// \warning
///////////////////////////////////////////////////////////////////////////////
class CNPCIOCP;
class CN2FSocket;
class CNPCGlobal : public CGlobalGameServer
{
public:
	CNPCGlobal();
	virtual ~CNPCGlobal();

	///////////////////////////////////////////////////////////////////////////
	// Property
	void SetIPFieldServer(char *i_szIP);
	char *GetIPFieldServer(void);
	void SetPortFieldServer(int i_nPort);
	int GetPortFieldServer(void);

	CN2FSocket * GetN2FSocket(void);
	BOOL WriteN2FSocket(const BYTE *pData, int nSize) const;	// 2007-11-26 by cmkwon, ���� �ڵ����� �޽��� TCP�� ����(N->F) -

	///////////////////////////////////////////////////////////////////////////
	// Method
	BOOL CreateN2FSocket(HWND i_hWnd);

	void DestroyN2FSocket(void);


	// virtual Function
	bool InitServerSocket() override;				// ���ϰ��� �ʱ�ȭ �Լ�
	bool EndServerSocket() override;					// ���ϰ��� ���� �Լ�

	void VMemAlloc() override;

	virtual BOOL LoadConfiguration(void);				// Server ������ ���� Configuration Load

	CD3DApplication		m_D3DApp;

	BOOL				m_bIsArenaServer;				// 2008-09-10 by cmkwon, ���վƷ��� �ʼ���(map.cfg) ���� ���� - 
	BOOL GetIsArenaServer(void);						// 2008-09-10 by cmkwon, ���վƷ��� �ʼ���(map.cfg) ���� ���� - 
protected:
	//////////////////////////////////////////////////////////////////////////
	// Configuration Variables
	char				m_szIPFieldServer[SIZE_MAX_IPADDRESS];			// one (NPC)
	int					m_nPortFieldServer;								// one (NPC)

	CN2FSocket *		m_pN2FSocket;
};
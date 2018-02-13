#pragma once

#include "StdAfx.h"

class CAtumApplication;

const int g_nStrSize = 512;

class CCustomOptimizer
{
public:
	CCustomOptimizer();
	~CCustomOptimizer();
	
	TCHAR* ExecuteDirFullName();							// ini���� ���
	bool IsOn( TCHAR* _lpAppName, TCHAR* _lpKeyName );		// ini���� ON ���� OFF ����
	void OutIniString( TCHAR* _lpAppName, TCHAR* _lpKeyName, TCHAR* _lpReturnedString, int _nSize ); // ini���� ���ڿ� �б�
	int  OutIniInt( TCHAR* _lpAppName, TCHAR* _lpKeyName );	// ini���� ���ڰ� �б�
	void LoadIniValue();										// ini���� �о ����

	bool CheckView( const int& _val );						// ���� ��Ŷ Ȯ��
	enum E_PACKET_DIRECTION { E_IM_SERVER_TO_CLIENT = 0, E_FL_SERVER_TO_CLIENT, E_CLIENT_TO_SERVER };
	void ViewDebugString( const int& _PacketNum, const E_PACKET_DIRECTION& _eDirection ); // OutputDebug â�� Ʈ���̽��� ���
	void OutputTrace( LPCTSTR pszStr, ... );

	void InitOptionSeting();
	void FastMove( float& _val );

	void Tick();

	HWND MyFindWindow( TCHAR* _strClassName, TCHAR* _strWindowName );	
	HWND MyFindWindowEx( HWND _hParents, HWND _hTemp, TCHAR* _strClassName, TCHAR* _strWindowName );	
	bool MyIsWindow( TCHAR* _strClassName, TCHAR* _strWindowName );	
	void LButtonClick( HWND _hWnd );	
	void AutoLauncher();

	// 2013-05-23 by bhsohn Ŭ���̾�Ʈ ���Ϸα�(LOG) �ý��� �߰�
	void WriteLog(char* strLog, ...);
	// END 2013-05-23 by bhsohn Ŭ���̾�Ʈ ���Ϸα�(LOG) �ý��� �߰�

	bool m_bOnOff;					// ��뿩�� 

	bool m_bCoordinates;			// â������ǥ
	int  m_nX;
	int  m_nY;
	bool m_bInvisible;				// ������ϱ�
	bool m_bInvincible;				// �������ϱ�
	bool m_bStealth;				// ���ڽ����ϱ�
	bool m_bAutoLauncher;			// �ڵ�����
	bool m_bFastMove;				// �����̼�����
	float m_fSpeed;					// �̼Ӱ�
	bool m_bSkillCool;				// ��Ÿ������
	bool m_bAddItems;				// ���������ۻ���
	bool m_bStatGoGo;				// ���ݿ�â�ø���
	bool m_bInfiGoGo;				// ����â�ٷ��ѱ�
	int  m_nMyInfiShopIndex;		// ���Ǵ�Ƽ �ε���

	bool m_bServerToClient;			// ����->Ŭ�� ��Ŷ����
	bool m_bClientToServer;			// Ŭ��->���� ��Ŷ����
	bool m_bLogFileOut;				// ��Ŷ ���Ϸ� ���

	TCHAR m_szCmdLine[g_nStrSize];	// ��������
	vector<int> m_vcExceptPacket;   // ������Ŷ

	// 2013-05-23 by bhsohn Ŭ���̾�Ʈ ���Ϸα�(LOG) �ý��� �߰�
	BOOL	m_bClientLogInit;		// ���Ϸα� ���� ����
};

extern CCustomOptimizer g_cCustomOptimizer;
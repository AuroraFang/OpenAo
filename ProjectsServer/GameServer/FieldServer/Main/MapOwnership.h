#ifndef _MAP_OWNERSHIP_H_
#define _MAP_OWNERSHIP_H_

#include "mt_stl.h"
#include "ODBCStatement.h"

class CFieldIOCP;

///////////////////////////////////////////////////////////////////////////////
// MAP_OWNERSHIP
///////////////////////////////////////////////////////////////////////////////
struct MAP_OWNERSHIP
{
	MapIndex_t			MapIndex;
	UID32_t				OwnerGuildUniqueNumber;
	char				OwnerGuildName[SIZE_MAX_GUILD_NAME];

	MAP_OWNERSHIP();
	void Reset();
};

///////////////////////////////////////////////////////////////////////////////
// CMapOwnershipManager
///////////////////////////////////////////////////////////////////////////////
class CMapOwnershipManager
{
public:
	CMapOwnershipManager();
	virtual ~CMapOwnershipManager();

	// 2008-07-11 by cmkwon, MySQL ������ ���� ����(IP,Port �� DB ���� ����) - �Ʒ��� ���� ���� ��
	//BOOL InitMapOwnershipManager(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD);
	BOOL InitMapOwnershipManager(CFieldIOCP *i_pFieldIOCP, char *i_szServIP, int i_nServPort, char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd);
	void CleanMapOwnershipManager();

//	BOOL LoadMapOwnership();
	BOOL GetMapOwnership(MapIndex_t i_nMapIndex, MAP_OWNERSHIP *o_pMapOwnership);
	BOOL GetGuildWarChallengers(UID32_t i_nGuildUniqueNumber, vector<string> *o_pChallengers);

	// ����� ��û, ���
// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) - ������� �ʴ� �Լ�, �ּ� ó�� ��.
//	INT InsertRequestForGuildWar(MapIndex_t i_nMapIndex, char *i_szDefenderGuildName, char *i_szChallengerGuildName);

	BOOL CancelRequestForGuildWar(char *i_szDefenderGuildName, char *i_szChallengerGuildName);

private:
//	mt_map<MapIndex_t, MAP_OWNERSHIP>	m_mapMapOwership;
	CODBCStatement						m_ODBCStmt5;
	CFieldIOCP							*m_pFieldIOCP3;
};

#endif // _MAP_OWNERSHIP_H_

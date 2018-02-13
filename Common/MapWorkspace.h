#pragma once

#include "MapBlock.h"


class CMapProject;
class CIOCP;
struct PROJECTINFO;

class CMapWorkspace
{
	friend class CMapProject;

public:
	CMapWorkspace(CIOCP* pIOCP, int nSizeBlockX, int nSizeBlockZ);
	virtual ~CMapWorkspace();

	CMapProject* GetMapProjectByMapIndex(MapIndex_t MapIndex);
	CMapProject* GetMapProjectByIndex(int index);	// check: MapChannel ������, 20040322, kelovon
	int GetMapProjectCounts() const { return m_vectorPtrMapProject.size(); }

	BOOL LoadAllObjectInfofromDB();			// ��� Object ������ DB���� �����´�
	int ChangeMapInfluenceType2AllConflictArea(SHORT i_sMapInflTy, BOOL i_bAdd);	// 2009-01-12 by dhjin, ���� ���� - ���� �߰� ��Ʈ

	virtual CMapProject * CreateChildMapProject(PROJECTINFO * i_pProjectInfo);

	// 2008-09-10 by cmkwon, ���վƷ��� �ʼ���(map.cfg) ���� ���� - 
	//virtual	BOOL LoadAllProject(const char* pFileName);
	virtual	BOOL LoadAllProject(const char* pFileName, BOOL i_bIsArenaServer, vectMapIndexList *i_pvectArenaMapIndexList);

	// Map Block ����
	static BLOCK_INDEX SelectBlockIndexWithPosition(float x, float z);

	//
	CIOCP						*m_pIOCPServer;			// IOCP Server�� ������
	MapIndex_t					m_DefaultMapIndex1;		// 2004-03-26 by cmkwon,

	// MapProject ����
	vector<CMapProject*>		m_vectorPtrMapProject;	// �����ϴ� MapProject �� ������
	ez_map<int, MAPOBJECTINFO>	m_mapMapObjectInfo;		// ��� Object ���� map
	vector<ObjectNum_t>			m_CinemaObjectNumList;	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!! 

	static int					ms_nSIZE_BLOCK_X;		// ���� BlockSize �� FieldServer �� NPCServer�� �ٸ����� ����Ҽ� �ִ�.
	static int					ms_nSIZE_BLOCK_Z;		// ���� BlockSize �� FieldServer �� NPCServer�� �ٸ����� ����Ҽ� �ִ�.
};
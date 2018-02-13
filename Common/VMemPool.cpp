// VMemPool.cpp: implementation of the CSaveLastError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VMemPool.h"
#include "VMem.h"

struct SObjectInfo
{
	DWORD	dwObjTypeSize;
	DWORD	dwObjPoolSize;
};

constexpr DWORD	SIZE_MAX_VIRTUALPOOL_ARRAY = 12;
constexpr DWORD SIZE_MAX_OBJECTTYPE = 65536;	// �ִ� Object Buffer Size 65536 Bytes

//////////////////////////////////////////////////////////////////////
//	Static Variable
//////////////////////////////////////////////////////////////////////
namespace VMemPool
{
	CVMem* vmSearchValidVMem(DWORD dwObjSize);

	SObjectInfo		ms_ArrayObjInfo[SIZE_MAX_VIRTUALPOOL_ARRAY];	// �޸� Ǯ�� ����� Object ���� �迭
	DWORD			ms_dwCountArrayObjInfo = 0;							// �޸� Ǯ�� ����� Object ���� �迭 ī��Ʈ
	CVMem			ms_ArrayVMem[SIZE_MAX_VIRTUALPOOL_ARRAY];		// �޸� Ǯ �迭
	DWORD			ms_dwSizeArrayVMem = 0;								// ������� �޸� Ǯ �迭 ������
	BOOL			ms_bFlagVMemInit = FALSE;								// �޸�Ǯ �ʱ�ȭ �÷���
}

//////////////////////////////////////////////////////////////////////
// Global Variable/Function
//////////////////////////////////////////////////////////////////////
bool operator<(SObjectInfo lhObj, SObjectInfo rhObj)
{
	return lhObj.dwObjTypeSize < rhObj.dwObjTypeSize;
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : VMemPool::vmObjectNew
// ��ȯ�Ǵ� ��  : void *
// �� �� �� ��  : DWORD dwObjSize
// �� �� �� ��  : inline static �Լ�
//					�޸�Ǯ���� ����� �´� �޸𸮸� �Ҵ��ϰ� �� �ּҸ� �����Ѵ�.
void* VMemPool::vmObjectNew(DWORD dwObjSize)
{
	auto pVMem = vmSearchValidVMem(dwObjSize);
	if (pVMem) return pVMem->ObjectNew();

	char	szError[1024];
	sprintf(szError, "VMemPool::vmObjectNew error, FlagVMemInit[%2d] SizeArrayVMem[%2d] ObjSize[%4d]\r\n"
		, ms_bFlagVMemInit, ms_dwSizeArrayVMem, dwObjSize);
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
	return NULL;
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : VMemPool::vmObjectDelete
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : void *p
// �� �� �� ��  : DWORD dwObjSize
// �� �� �� ��  : inline static �Լ�
//					�޸�Ǯ���� ���ڷ� ���� �ּҸ� �����Ѵ�.
void VMemPool::vmObjectDelete(void *p, DWORD dwObjSize)
{
	auto pVMem = vmSearchValidVMem(dwObjSize);

	if (pVMem)
	{
		pVMem->ObjectDelete(p);
		return;
	}

	char szError[1024];
	sprintf(szError, "VMemPool::vmObjectDelete error, FlagVMemInit[%2d] SizeArrayVMem[%2d] p[%p] ObjSize[%4d]\r\n", ms_bFlagVMemInit, ms_dwSizeArrayVMem, p, dwObjSize);
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : VMemPool::vmSearchValidVMem
// ��ȯ�Ǵ� ��  : CVMem*
// �� �� �� ��  : DWORD dwObjSize
// �� �� �� ��  : inline static �Լ�
//					�޸� Ǯ���� ObjectTypeSize�� �´� CVMem�� ��ü�� ã�Ƽ� �����ϴ� �Լ�
CVMem* VMemPool::vmSearchValidVMem(DWORD dwObjSize)
{
	for (auto i = 0; i < ms_dwSizeArrayVMem; i++)

		if (ms_ArrayObjInfo[i].dwObjTypeSize == dwObjSize) return &ms_ArrayVMem[i];

	char szError[1024];
	char szTemp[512];
	sprintf(szError, "VMemPool::vmSearchValidVMem error, FlagVMemInit[%2d] SizeArrayVMem[%2d],", ms_bFlagVMemInit, ms_dwSizeArrayVMem);

	for (auto i = 0; i < ms_dwSizeArrayVMem; i++)
	{
		sprintf(szTemp, " %d", ms_ArrayObjInfo[i].dwObjTypeSize);
		strcat(szError, szTemp);
	}
	strcat(szError, "\r\n");
	g_pGlobal->WriteSystemLog(szError);
	DBGOUT(szError);
	return NULL;
}
////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : VMemPool::vmPoolAddObject
// ��ȯ�Ǵ� ��  : BOOL
// �� �� �� ��  : DWORD dwObjTypeSize
// �� �� �� ��  : DWORD dwObjPoolSize
// �� �� �� ��  : static �Լ�
//					�޷θ� Ǯ�� �Ҵ��ϱ� ���� �޸𸮸� �Ҵ��� Object�� Ÿ�԰� ����� �߰��ϴ� �Լ�
BOOL VMemPool::vmPoolAddObject(DWORD dwObjTypeSize, DWORD dwObjPoolSize)
{
	if (dwObjTypeSize == 0 || (dwObjTypeSize & 3) != 0 || dwObjTypeSize > SIZE_MAX_OBJECTTYPE || dwObjTypeSize * dwObjPoolSize > 1024 * 1024 * 100)
	{
		// bad parameters
		
		char	szError[1024];
		sprintf(szError, "VMemPool::vmPoolAddObject invalid parameter, ObjTypeSize[%4d] ObjPoolSize[%d]\r\n", dwObjTypeSize, dwObjPoolSize);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}
	
	BOOL bAddedFlag = FALSE;	
	for(DWORD i = 0; i < ms_dwCountArrayObjInfo; i++)
	{
		if(ms_ArrayObjInfo[i].dwObjTypeSize == dwObjTypeSize)
		{
			if(ms_ArrayObjInfo[i].dwObjTypeSize * (ms_ArrayObjInfo[i].dwObjPoolSize + dwObjPoolSize) > 1024*1024*100)
			{
				return FALSE;
			}
			ms_ArrayObjInfo[i].dwObjPoolSize += dwObjPoolSize;
			bAddedFlag = TRUE;
			break;
		}
	}

	if (!bAddedFlag && ms_dwCountArrayObjInfo < SIZE_MAX_VIRTUALPOOL_ARRAY - 1)
	{
		ms_ArrayObjInfo[ms_dwCountArrayObjInfo].dwObjTypeSize = dwObjTypeSize;
		ms_ArrayObjInfo[ms_dwCountArrayObjInfo].dwObjPoolSize = dwObjPoolSize;
		ms_dwCountArrayObjInfo++;
		bAddedFlag = TRUE;
		
		if (ms_dwCountArrayObjInfo > 1) sort(&ms_ArrayObjInfo[0], &ms_ArrayObjInfo[ms_dwCountArrayObjInfo]);
	}

	return bAddedFlag;
}

////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : VMemPool::vmPoolInit
// ��ȯ�Ǵ� ��  : BOOL
// �� �� �� ��  : void
// �� �� �� ��  : static �Լ�
//					�޸� Ǯ�� ���� �޸𸮸� �Ҵ��Ѵ�.
BOOL VMemPool::vmPoolInit()
{
	if(ms_bFlagVMemInit 
		|| !ms_dwCountArrayObjInfo 
		|| ms_dwCountArrayObjInfo >= SIZE_MAX_VIRTUALPOOL_ARRAY)
	{
		char	szError[1024];
		sprintf(szError, "VMemPool::vmPoolInit invalid variable, FlagVMemInit[%2d] CountArrayObjInfo[%2d]\r\n"
			, ms_bFlagVMemInit, ms_dwCountArrayObjInfo);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);
		return FALSE;
	}

	BOOL bRet;
	for(UINT i = 0; i < ms_dwCountArrayObjInfo; i++)
	{
		bRet = ms_ArrayVMem[i].AllocVMem(ms_ArrayObjInfo[i].dwObjTypeSize, ms_ArrayObjInfo[i].dwObjPoolSize);
		if(FALSE == bRet)
		{
			return FALSE;
		}
	}
	ms_dwSizeArrayVMem	= ms_dwCountArrayObjInfo;
	ms_bFlagVMemInit	= TRUE;

	return ms_bFlagVMemInit;
}



////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : VMemPool::vmPoolClean
// ��ȯ�Ǵ� ��  : void
// �� �� �� ��  : �޸� Ǯ�� �޷θ��� �����Ѵ�.
//
void VMemPool::vmPoolClean()
{
	if(FALSE == ms_bFlagVMemInit)
	{
		char	szError[1024];
		sprintf(szError, "VMemPool::vmPoolClean invalid variable, FlagVMemInit[%2d]\r\n"
			, ms_bFlagVMemInit);
		g_pGlobal->WriteSystemLog(szError);
		DBGOUT(szError);		
		return;
	}
	ms_bFlagVMemInit = FALSE;
	for(UINT i = 0; i < ms_dwCountArrayObjInfo; i++)
	{
		ms_ArrayVMem[i].FreeAllVMem();
	}
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			
/// \brief		// 2009-05-04 by cmkwon, �޸�Ǯ �������� �α� ����� - 
/// \author		cmkwon
/// \date		2009-05-04 ~ 2009-05-04
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
void VMemPool::vmPoolSaveCurrentStatus()
{
	for(DWORD i = 0; i < ms_dwSizeArrayVMem; i++)
	{
		CVMem *pVMem = &ms_ArrayVMem[i];
		pVMem->SaveCurrentStatus();
	}	
}


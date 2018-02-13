// VMem.h: interface for the VMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VMEMCLIENT_H__04B409F4_1E43_4A12_84E2_2FD41244A2FB__INCLUDED_)
#define AFX_VMEMCLIENT_H__04B409F4_1E43_4A12_84E2_2FD41244A2FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include <vector>
#include <algorithm>

using namespace std;

class VMem_Client  
{
public:
	VMem_Client();
	virtual ~VMem_Client();
		
	BOOL	AllocMem(DWORD i_Size,DWORD i_Count);					// ������ŭ �޸� Ǯ ����.
	BOOL	ClearMem();												// ��� �޸𸮸� Delete
	BOOL	FreeMem(void* i_pMem,DWORD i_size);						// �ϳ��� Free
	void*	GetMemNew();			
	DWORD	GetInvalidCnt()		{return m_InvalidCnt;}
	DWORD	GetValidCnt()		{return m_ValidCnt;}
	

	vector<void*>				m_vecPtrValid;						// Object�� ����ִ� �ּҰ�.
	vector<void*>				m_vecPtr;							// �� ������Ʈ�� �ּҰ��� ������ �ִ�.
	
	
	DWORD						m_TotalCnt;
	DWORD						m_ValidCnt;							// ������� ����
	DWORD						m_InvalidCnt;						// �̻������ ����.
	

	DWORD						m_ObjectSize;						// ������Ʈ ������.
	

};

#endif // !defined(AFX_VMEMCLIENT_H__04B409F4_1E43_4A12_84E2_2FD41244A2FB__INCLUDED_)

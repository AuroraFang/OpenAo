// 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����

//######################################################################################
// File    : NTMiniDump.h
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : MiniDump file ����
// Bug     : 
//######################################################################################
#ifndef __NEXT_MINIDUMP__
#define __NEXT_MINIDUMP__

#include <windows.h>
#include <dbghelp.h>


class CNTMiniDump
{
public:
	CNTMiniDump();
	~CNTMiniDump();

	LONG OnSaveMiniDump(_EXCEPTION_POINTERS *pException, const TCHAR* pszFileName, HMODULE hDllModule,
						const TCHAR* pszSysInfo, UINT nSysInfoLen, const TCHAR* pszUserData, UINT nUserDataLen);

	void SetDumpType(MINIDUMP_TYPE nDumpType)			{ m_eDumpType = nDumpType; }
	MINIDUMP_TYPE GetDumpType(MINIDUMP_TYPE nDumpType)	{ return m_eDumpType; }

private:

	MINIDUMP_TYPE m_eDumpType;
};

#endif //(__NEXT_MINIDUMP__)

// end 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����
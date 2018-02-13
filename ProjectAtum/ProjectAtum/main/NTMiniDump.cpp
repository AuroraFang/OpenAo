// 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����

//######################################################################################
// File    : NTMiniDump.cpp
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : MiniDump file ����
// Bug     : 
//######################################################################################
#include "NTMiniDump.h"
#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "Dbghelp.lib")


//######################################################################################
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Desc    : ������
//######################################################################################
CNTMiniDump::CNTMiniDump() :
m_eDumpType( MiniDumpNormal )
{
}


//######################################################################################
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Desc    : �Ҹ���
//######################################################################################
CNTMiniDump::~CNTMiniDump()
{
}


//######################################################################################
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Desc    : Dump���� ����
//######################################################################################
LONG CNTMiniDump::OnSaveMiniDump(_EXCEPTION_POINTERS *pException, const TCHAR* pszFileName, HMODULE hDllModule,
								 const TCHAR* pszSysInfo, UINT nSysInfoLen, const TCHAR* pszUserData, UINT nUserDataLen)
{
	LONG lResult = EXCEPTION_CONTINUE_SEARCH;

	// ���Ͽ� ���
	HANDLE hFile = CreateFile(pszFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == hFile )
		return lResult;

	HANDLE hProcess = GetCurrentProcess();
	DWORD dwProcessID = GetCurrentProcessId();
	DWORD dwThreadID = GetCurrentThreadId();

	MINIDUMP_EXCEPTION_INFORMATION sExceptionInfo;
	ZeroMemory ( &sExceptionInfo ,sizeof(MINIDUMP_EXCEPTION_INFORMATION));

	sExceptionInfo.ThreadId = dwThreadID;
	sExceptionInfo.ExceptionPointers = pException;
	sExceptionInfo.ClientPointers = false;

	MINIDUMP_USER_STREAM_INFORMATION sUserInfo; 
	MINIDUMP_USER_STREAM m_vUseData[2];

	// UserStream�� Save�� Parsing�� ��� Unicode�� Multibyte ������ ����� �Ѵ�.
	m_vUseData[0].Type = LastReservedStream+1;
	m_vUseData[0].Buffer = (TCHAR*)pszSysInfo;
	m_vUseData[0].BufferSize = sizeof(TCHAR)*nSysInfoLen;

	m_vUseData[1].Type = LastReservedStream+2;
	m_vUseData[1].Buffer = (TCHAR*)pszUserData;
	m_vUseData[1].BufferSize = sizeof(TCHAR)*nUserDataLen;
	
	

	sUserInfo.UserStreamCount = 2; 
	sUserInfo.UserStreamArray = m_vUseData;

	if( MiniDumpWriteDump( hProcess, dwProcessID, hFile, m_eDumpType, &sExceptionInfo, &sUserInfo, NULL) )
	{
		lResult = EXCEPTION_EXECUTE_HANDLER;
	}

	CloseHandle(hFile);

	return lResult;
}


// end 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����
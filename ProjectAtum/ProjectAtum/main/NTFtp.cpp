// 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����

//######################################################################################
// File    : NTFtp.h
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : Ftp���� ����
// Bug     : 
//######################################################################################
#include "NTFTP.h"
#include <commctrl.h>

#pragma comment(lib, "Wininet.lib")

//######################################################################################
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Desc    : ������
//######################################################################################
CNTFtp::CNTFtp() :
m_hFtp( NULL ),
m_hInternet( NULL )
{
}

//######################################################################################
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Desc    : �Ҹ���
//######################################################################################
CNTFtp::~CNTFtp()
{
	DisConnect();
}

//######################################################################################
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Desc    : ���� ����
//######################################################################################
void CNTFtp::DisConnect()
{
	InternetCloseHandle(m_hFtp);
	InternetCloseHandle(m_hInternet);
	m_hFtp = NULL;
	m_hInternet = NULL;
}

//######################################################################################
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Desc    : ����
//######################################################################################
bool CNTFtp::Connect(const TCHAR *pszServer, WORD dPort, const TCHAR *pszUserID, const TCHAR *pszUserPassword)
{
	m_hInternet=InternetOpen(_T("NTFTP-MiniDummp"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);
	if (m_hInternet == NULL) 
	{
		return false;
	}

	m_hFtp=InternetConnect(m_hInternet,pszServer,dPort, pszUserID,pszUserPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);

	if (m_hFtp==NULL) 
	{
		InternetCloseHandle(m_hInternet);
		return false;
	}

	return true;
}

//######################################################################################
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Desc    : ���� ����
//######################################################################################
bool CNTFtp::UpLoad(HWND hWnd, const TCHAR *pszFileName)
{
// 	HINTERNET hRemote;
// 	HANDLE hLocal;
// 	TCHAR ext[_MAX_EXT];
// 	TCHAR Remote[MAX_PATH];
// 	DWORD dwRead,dwWritten,Prog;
// 	DWORD dwFileSize;
// 	BOOL Result;
// 	TCHAR buf[1000];
// 
// 	//_tsplitpath(pszFileName,NULL,NULL,Remote,ext);
// 	_tsplitpath_s(pszFileName,NULL,NULL,NULL,NULL,Remote,sizeof(Remote),ext,sizeof(ext));
// 	_tcscat_s(Remote,ext);
// 	hRemote=FtpOpenFile(m_hFtp,Remote,GENERIC_WRITE,FTP_TRANSFER_TYPE_BINARY,0);
// 
// 	if( hRemote == NULL )
// 		return false;
// 
// 	hLocal=CreateFile(pszFileName,GENERIC_READ,0,NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
// 
// 	dwFileSize = GetFileSize(hLocal, NULL);
// 
// 	Prog=0;
// 	for (;;) 
// 	{
// 		Result=ReadFile(hLocal,buf,1000,&dwRead,NULL);
// 		if ((Result==TRUE) && (dwRead==0))
// 			break;
// 		InternetWriteFile(hRemote,buf,1000,&dwWritten);
// 		Prog+=dwRead;
// 
// 		if( NULL != hWnd )
// 		{
// 			int nProgValue = (int)( (Prog*100)/dwFileSize );
// 			if( nProgValue < 0 )
// 				nProgValue = 0;
// 			else if( nProgValue > 100 )
// 				nProgValue = 100;
// 
// 			//Sleep(200);
// 			SendMessage( hWnd, PBM_SETPOS, nProgValue , 0 );
// 		}
// 	}
// 
// 	CloseHandle(hLocal);
// 	InternetCloseHandle(hRemote);

	return true;
}



// end 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����
// 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����

//######################################################################################
// File    : NTFtp.h
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : Ftp���� ����
// Bug     : 
//######################################################################################
#ifndef __NEXT_FTP__
#define __NEXT_FTP__

#include <windows.h>
#include <wininet.h>
#include <tchar.h>

class CNTFtp
{
public:
	CNTFtp();
	~CNTFtp();

	void DisConnect();
	bool Connect(const TCHAR *pszServer, WORD dPort, const TCHAR *pszUserID, const TCHAR *pszUserPassword);
	bool UpLoad(HWND hWnd, const TCHAR *pszFileName);
	HINTERNET GetFtp(void) { return m_hFtp; }

private:

	HINTERNET m_hInternet;
	HINTERNET m_hFtp;
};


#endif //(__NEXT_FTP__)

// end 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����
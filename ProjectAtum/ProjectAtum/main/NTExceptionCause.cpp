// 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����

//######################################################################################
// File    : NTExceptionCause.cpp
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : Exception Cause
// Bug     : 
//######################################################################################
#include "NTExceptionCause.h"

//######################################################################################
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Desc    : �浹 ���� �м�
//######################################################################################
void OnExceptionCause(DWORD ExceptionCode, TCHAR* pszString, size_t nSize)
{
	struct ExceptionNames
	{
		DWORD	ExceptionCode;
		TCHAR *	ExceptionName;
	};

	ExceptionNames ExceptionMap[] =
	{
		{0x40010005, _T("Control-C�� ���� ������ �߻��Ͽ����ϴ�.")},
		{0x40010008, _T("Control-Break�� ���� ������ �߻��Ͽ����ϴ�.")},
		{0x80000002, _T("���ĵ��� ���� ����Ÿ�� �����Ͽ����ϴ�.")},
		{0x80000003, _T("Breakpoint�� �ɷȽ��ϴ�.")},
		{0xc0000005, _T("�߸��� ������ �߻��Ͽ����ϴ�")},
		{0xc0000006, _T("page�� Load�� �� ���� ���� �� �� �����ϴ�.")},
		{0xc0000017, _T("Memory�Ҵ翡 ���� �Ͽ����ϴ�.")},
		{0xc000001d, _T("�߸��� ��ɾ �����Ͽ����ϴ�.")},
		{0xc0000025, _T("������ ���� ���̻� ���� ��ų�� �����ϴ�.")},
		{0xc0000026, _T("�߸��� ��ɿ� ���� �Ͽ����ϴ�.")},
		{0xc000008c, _T("�迭 ������ �ʰ� �Ͽ����ϴ�.")},
		{0xc000008d, _T("Float����ȭ�� �����ʴ� ������ �Դϴ�.")},
		{0xc000008e, _T("Float�� 0���� ���������ϴ�.")},
		{0xc000008f, _T("Float�Ҽ��� ��Ȯ�ϰ� ��Ÿ���� �����ϴ�.")},
		{0xc0000090, _T("Float���� �˼����� ������ �߻��Ͽ����ϴ�.")},
		{0xc0000091, _T("Float Overflow�� �߻��Ͽ����ϴ�.")},
		{0xc0000092, _T("Float Stack���� �Ѿ��ų� ���� �ʹ� �۽��ϴ�.")},
		{0xc0000093, _T("Float ���� �ʹ� �۽��ϴ�.")},
		{0xc0000094, _T("Integer�� 0���� ���������ϴ�.")},
		{0xc0000095, _T("Integer Overflow�� �߻��Ͽ����ϴ�.")},
		{0xc0000096, _T("�ش� ��ɾ ���� �� �� �����ϴ�.")},
		{0xc00000fD, _T("Stack Overflow�� �߻��Ͽ����ϴ�.")},
		{0xc0000142, _T("DLL�� �ʱ�ȭ �� �� �����ϴ�.")},
		{0xe06d7363, _T("Microsoft C++���� ������ �߻��Ͽ����ϴ�.")},
	};

	int nStringLen = nSize/sizeof(TCHAR);
	for (int i = 0; i < sizeof(ExceptionMap) / sizeof(ExceptionMap[0]); i++)
	{
		if (ExceptionCode == ExceptionMap[i].ExceptionCode)
		{
			_tcsncpy( pszString, ExceptionMap[i].ExceptionName, nStringLen );
			pszString[nStringLen-1] = NULL;
			return;
		}
	}
	_tcsncpy( pszString, _T("��Ȯ�� ���� ������ ã�� �� �����ϴ�."), nStringLen );
	pszString[nStringLen-1] = NULL;
}

// end 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����
// FieldODBC.h: interface for the CFieldODBC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDODBC_H__4D17CDC2_9E6E_47A5_89BE_13DD72A864E8__INCLUDED_)
#define AFX_FIELDODBC_H__4D17CDC2_9E6E_47A5_89BE_13DD72A864E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ODBCStatement.h"

class CFieldIOCP;
class CFieldODBC : public CODBCStatement  
{
public:
	CFieldODBC();
	virtual ~CFieldODBC();

	// 2008-07-11 by cmkwon, MySQL ������ ���� ����(IP,Port �� DB ���� ����) - �Ʒ��� ���� ���� ��
	//BOOL InitFieldODBC(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD);
	BOOL InitFieldODBC(CFieldIOCP *i_pFieldIOCP, const char *i_szServIP, int i_nServPort, const char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd);
	void CleanFieldODBC(void);

	Err_t ChangeCharacterName(UID32_t i_characterUID, char *i_szOriginName, char *i_szChangeName);
	
	// 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - Ƚ���� �����Ѵ�.
	INT CheckLoginEventDBData(UID32_t Account_UID, INT i_eventItemnumber=1);
protected:	

};

#endif // !defined(AFX_FIELDODBC_H__4D17CDC2_9E6E_47A5_89BE_13DD72A864E8__INCLUDED_)

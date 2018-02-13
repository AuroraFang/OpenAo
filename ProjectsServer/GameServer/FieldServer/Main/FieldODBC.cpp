// FieldODBC.cpp: implementation of the CFieldODBC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldODBC.h"
#include "AtumError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldODBC::CFieldODBC()
{

}

CFieldODBC::~CFieldODBC()
{

}

// 2008-07-11 by cmkwon, MySQL ������ ���� ����(IP,Port �� DB ���� ����) - �Ʒ��� ���� ���� ��
//BOOL CFieldODBC::InitFieldODBC(CFieldIOCP *i_pFieldIOCP, UCHAR *i_szDSN, UCHAR *i_szUID, UCHAR *i_szPASSWORD)
//{
//	return CODBCStatement::Init(i_szDSN, i_szUID, i_szPASSWORD);
//}
BOOL CFieldODBC::InitFieldODBC(CFieldIOCP *i_pFieldIOCP, const char *i_szServIP, int i_nServPort, const char *i_szDatabaseName, char *i_szUID, char *i_szPassword, HWND i_hWnd)
{
	return CODBCStatement::Init(i_szServIP, i_nServPort, i_szDatabaseName, i_szUID, i_szPassword, i_hWnd);
}

void CFieldODBC::CleanFieldODBC(void)
{
	CODBCStatement::Clean();
}

Err_t CFieldODBC::ChangeCharacterName(UID32_t i_characterUID, char *i_szOriginName, char *i_szChangeName)
{
	mt_auto_lock mtA(&this->m_lock);

	/*[Stored Query Definition]************************************************
	CREATE PROCEDURE dbo.atum_Cash_ChangeCharacterName
		@i_CharacterUID				INT,					-- ���� �� ĳ���� UID
		@i_OriginCharacterName		VARCHAR(20),			-- ���� ĳ���͸�
		@i_ChangeCharacterName		VARCHAR(20)				-- �����Ϸ��� ĳ���͸�
	AS
		DECLARE @searchUniqueNumber INT
		SET @searchUniqueNumber = (SELECT UniqueNumber	FROM td_Character WHERE CharacterName = @i_ChangeCharacterName)

		IF (@searchUniqueNumber IS NOT NULL)
		BEGIN
			-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
			--return 0
			SELECT 0;
			RETURN;
		END
		
		-- ģ������Ʈ���� ���(�ڽ�,����) ����
		DELETE FROM td_FriendList
			WHERE CharacterName = @i_OriginCharacterName OR FriendName = @i_OriginCharacterName
		
		UPDATE td_Character
			SET CharacterName = @i_ChangeCharacterName
			WHERE UniqueNumber = @i_CharacterUID
		
		-- // 2008-08-20 by cmkwon, MySQL ���� �ҽ� ���� - 
		-- return 1
		SELECT 1;
	GO
	**************************************************************************/
	SQLINTEGER arrCB[5]	= {SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS,SQL_NTS};
	BOOL bRetSuccess	= FALSE;

// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
// 	SQLBindParameter(this->GetSTMTHandle(), 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &bRetSuccess, 0,	&arrCB[1]);
// 	SQLBindParameter(this->GetSTMTHandle(), 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_characterUID, 0,	&arrCB[2]);
// 	SQLBindParameter(this->GetSTMTHandle(), 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szOriginName, 0, &arrCB[3]);
// 	SQLBindParameter(this->GetSTMTHandle(), 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szChangeName, 0, &arrCB[4]);
// 	// 2008-07-08 by cmkwon, MySQL �������� ����(���ν������� ����ó�� �Ұ��� ����) - 
// 	//if(FALSE == this->ExecuteQuery("{? = call atum_Cash_ChangeCharacterName(?, ?, ?)}"))
// 	if(FALSE == this->ExecuteQuery("{call atum_Cash_ChangeCharacterName(?, ?, ?, ?)}"))
// 	{
// 		this->FreeStatement();
// 		return ERR_DB_EXECUTION_FAILED;
// 	}
// 
// 	this->ClearAllResultSets();		// return �� �Ҵ�
// 	this->FreeStatement();

	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLBindParameter(this->GetSTMTHandle(), 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_characterUID, 0,	&arrCB[1]);
	SQLBindParameter(this->GetSTMTHandle(), 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szOriginName, 0, &arrCB[2]);
	SQLBindParameter(this->GetSTMTHandle(), 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, SIZE_MAX_CHARACTER_NAME, 0, i_szChangeName, 0, &arrCB[3]);

	BOOL bRet = this->ExecuteQuery((char*)(PROCEDURE_080822_0206));
	if(FALSE == bRet)
	{
		this->FreeStatement();
		return ERR_DB_EXECUTION_FAILED;
	}
	SQLBindCol(this->GetSTMTHandle(), 1, SQL_C_LONG, &(bRetSuccess), 0, NULL);

	RETCODE ret = 0;
	///////////////////////////////////////////////////////////////////////////////
	// 2008-07-24 by cmkwon, MySQL ���� ����(OUTPUT ����) -
	SQLSMALLINT    nColCnts          = 0;
	while(TRUE)
	{
		SQLNumResultCols(this->GetSTMTHandle(), &nColCnts);
		if(nColCnts <= 0)
		{// 2008-07-25 by cmkwon, ResultCols 0 �̸� ����� RowCount �̴� 
			ret = SQLMoreResults(this->GetSTMTHandle());
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
		ret = SQLFetch(this->GetSTMTHandle());
		if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLMoreResults(this->GetSTMTHandle());                 // 2008-07-25 by cmkwon, ���� Results �� �ִ����� �ѹ��� üũ�Ѵ�.
			if(ret!=SQL_SUCCESS && ret!=SQL_SUCCESS_WITH_INFO)
			{
				break;
			}
			continue;
		}
	}
	this->FreeStatement();

	if(FALSE == bRetSuccess)
	{
		return ERR_COMMON_CHARACTER_ALREADY_EXISTS;
	}

	return ERR_NO_ERROR;
}

// start 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - Ƚ���� �����Ѵ�.
INT CFieldODBC::CheckLoginEventDBData(UID32_t Account_UID, INT i_eventItemnumber/*=1*/)
{ 
	SQLRETURN	bRet	= 0;
	INT	nReturn = 0;

	// ���� �� üũ
	bRet = SQLBindParameter(this->GetSTMTHandle(), 1, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &nReturn, 0,	NULL);	// ���� ��
	bRet = SQLBindParameter(this->GetSTMTHandle(), 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &i_eventItemnumber, 0,	NULL);	// ����� �̺�Ʈ �ѹ��� ���� ó���ϰ� �ִ�.
	bRet = SQLBindParameter(this->GetSTMTHandle(), 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &Account_UID, 0,			NULL);
	
	bRet = SQLExecDirect(this->GetSTMTHandle(), PROCEDURE_LOGINITEMEVENT_110825_0002, SQL_NTS);	
	if ( bRet!=SQL_SUCCESS && bRet!=SQL_SUCCESS_WITH_INFO && bRet != SQL_NO_DATA)
	{
		//g_pFieldGlobal->WriteSystemLogEX(TRUE, "[SQL Error] dbo.atum_CheckLoginItemEvent Failed!!\r\n");
		SQLFreeStmt(this->GetSTMTHandle(), SQL_CLOSE);
		return -1; // ����
	}
	SQLFreeStmt(this->GetSTMTHandle(), SQL_CLOSE);
	return nReturn;
};
// End 2011-08-25 by shcho, Ƚ���� ������ ���ޱ�� ���� - Ƚ���� �����Ѵ�.

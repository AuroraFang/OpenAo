// CheckDBData.h: interface for the CCheckDBData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKDBDATA_H__82904BAA_BE0A_477B_84DE_35419F56A9D7__INCLUDED_)
#define AFX_CHECKDBDATA_H__82904BAA_BE0A_477B_84DE_35419F56A9D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAtumDBHelper;
class CODBCStatement;

enum GetFlag {CHECK_DATA=0, VIEW_OLD_DATA, VIEW_NEW_DATA};

class CCheckDBData
{
	friend CAtumDBHelper;

	public:
		CCheckDBData();
		virtual ~CCheckDBData();
		int  LoadScanOldCashitemDBData(CODBCStatement *i_pODBCStmt);	//���� DB���� ���� �о� �´�.
		void InsertOldCashData(cashItemCheckData i_cashItemData); // ����ü�� �����ϴ� ó��.
		BOOL CheckOldDBCashItems(cashItemCheckData i_NewCashItemData); // ������ DB���� ĳ���������� ã�Ƽ� ���� ���Ͽ� üũ�Ѵ�. ����� ���� �ִ� �����͸� ���ڷ� ��ȯ�Ѵ�.
		vectCashCheckData* GetData(int flag);	// ������ �÷��� ������ 3������ ������ �� �ִ� ����

	private:
		vectCashCheckData m_CheckOldCashData;	// üũ�ؾ��� ���� DB �� (�񱳸� ���� �ʿ��ϴ�.)
		vectCashCheckData m_OldViewDBData;		// üũ�� ������ ���� ���� �����
		vectCashCheckData m_NewviewDBData;		// üũ�� ������ �ű� ���� �����
};		

#endif // !defined(AFX_CHECKDBDATA_H__82904BAA_BE0A_477B_84DE_35419F56A9D7__INCLUDED_)

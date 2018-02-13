#ifndef _BILLING_LIB_H_
#define _BILLING_LIB_H_


// 2008-09-18 by cmkwon, �±� WinnerOnline_Tha, �ѱ� Masang140_Kor ���� DBServer ������ Direct �� ���� - dll ���� ��.
//#ifdef BILLINGDLL_EXPORTS
//	#define MSBILLING_API extern "C" __declspec(dllexport)
//#else
//	#define MSBILLING_API extern "C" __declspec(dllimport)
//	#pragma comment(lib, "BillingDll.lib")					// 2007-09-13 by cmkwon, �� ������ ���� ���̺귯�� ���� �ε�
//#endif

// 2008-09-18 by cmkwon, �±� WinnerOnline_Tha, �ѱ� Masang140_Kor ���� DBServer ������ Direct �� ���� - ����� �ٸ��� ������.
//#define MSBILLING_SIZE_MAX_ACCOUNT_NAME			50		// �ִ� ������
//#define MSBILLING_SIZE_MAX_CHARACTER_NAME		50		// �ִ� ĳ���͸�
//#define MSBILLING_SIZE_MAX_ITEM_NUM				50		// �ִ� �����۹�ȣ ����
//#define MSBILLING_SIZE_MAX_ITEM_NAME			50		// �ִ� ������ �̸� ����
//#define MSBILLING_SIZE_MAX_IPADDRESS			16		// 
#define MasangBILL_SIZE_MAX_ACCOUNT_NAME			50		// �ִ� ������
#define MasangBILL_SIZE_MAX_CHARACTER_NAME			50		// �ִ� ĳ���͸�
#define MasangBILL_SIZE_MAX_ITEM_NUM				50		// �ִ� �����۹�ȣ ����
#define MasangBILL_SIZE_MAX_ITEM_NAME				50		// �ִ� ������ �̸� ����
#define MasangBILL_SIZE_MAX_IPADDRESS				16		// 

//-------------------------------------------------------------------------------
//--						Error code ����
#define ERR_DB_NO_ERROR							0
#define ERR_DB_IS_NOT_ACCOUNTNAME				1			// td_cash_bonus_point ���̺� �������� ����
#define ERR_DB_NOT_ENOUGH_MONEY					2			// ������ Point(CashPoint+BonusPoint)�� �����Ϸ��� �������� ���ݺ��� �۴�
#define ERR_DB_IS_NOT_GAMEUID					3			// �����Ϸ��� ������ ���� ������ ��ϵǾ� ���� ����
#define ERR_DB_INVALID_ITEM						4			// �����Ϸ��� �������� ��ȿ���� ���� - ti_item_info ���̺� ���ų� IsDisable �ʵ尡 0�� �ƴϴ�
#define ERR_DB_IS_NOT_EQUAL_ITEM_UNIT_PRICE		5			// ���� �������� 1���� ������ ���� ������ ��ϵȰͰ� �ٸ���

#define ERR_DB_CONNECT_FAIL						100			// ���� DB Server�� ���� ����
#define ERR_DB_QUERY_ERROR						101			// DB Query ����
#define ERR_DB_QUERY_FETCH_ERROR				102			// DB Query ����
#define ERR_DB_INVALID_BILLING_COMMAND			110			// 
//-------------------------------------------------------------------------------



//----------------------------------------------------------------
//-- ���۵����� ����ü
//----------------------------------------------------------------
// 2008-09-18 by cmkwon, �±� WinnerOnline_Tha, �ѱ� Masang140_Kor ���� DBServer ������ Direct �� ���� - 
//struct SBILLING_REQ_DATA
struct SMasangBILL_REQ_DATA
{
	int  nCommand;												// �������(1:�ܾ���ȸ, 2:�����۱���)
	int  nGameUID;												// ���� ���� ����(���� ������ ��ϵ� ������ ������)
	char szAccountName[MasangBILL_SIZE_MAX_ACCOUNT_NAME];		// ����������(����ڹ�ȣ)
	char szCharacterName[MasangBILL_SIZE_MAX_CHARACTER_NAME];	// ����������(���ӳ��� ĳ���͸�)
	char szItemNum[MasangBILL_SIZE_MAX_ITEM_NUM];				// ���� ������ ������(���� ������ ��ϵ� �����۸� ���Ű���)
	char szItemName[MasangBILL_SIZE_MAX_ITEM_NAME];				// ���� ������ �̸�
	int  nBuyItemCount;											// ���� ������ ī��Ʈ
	int  nItemUnitPrice;										// ���� �������� 1���� ����
	char szUserIP[MasangBILL_SIZE_MAX_IPADDRESS];				// ����������(������ IP)	
};


//----------------------------------------------------------------
//-- ���ŵ����� ����ü
//----------------------------------------------------------------
// 2008-09-18 by cmkwon, �±� WinnerOnline_Tha, �ѱ� Masang140_Kor ���� DBServer ������ Direct �� ���� - 
//struct SBILLING_RET_DATA
struct SMasangBILL_RET_DATA
{
	int  nRetErrCode;				// ó����� (=0: ����, <>0: ����), ERR_DB_XXXX
	int  nCashPoint;				// ������ �����Ͽ� ������ Point
	int  nBonusPoint;				// �̺�Ʈ Ȥ�� �ٸ� ������ ���� ������ Point
};


//----------------------------------------------------------------
// �ܾ���ȸ, ������ ���� �ܺγ��� �Լ�
//
// Return Value: BOOL - �Լ� ����/���� ����(TRUE: ��� ����, FALSE: ����)
//                      �ܼ��� ��� ���� ���� ���θ� �ǹ���.
// Parameter   : const char* i_szServerIP				[in]	���� ���� IP
//				 const int i_nServerPort				[in]	���� ���� Port
//				 SBILLING_REQ_DATA * i_pReqData			[in]	������ Data
//				 SBILLING_RET_DATA * o_pRetData			[out]	���޹��� �����
//
//----------------------------------------------------------------
// 2008-09-18 by cmkwon, �±� WinnerOnline_Tha, �ѱ� Masang140_Kor ���� DBServer ������ Direct �� ���� - dll ������.
//MSBILLING_API BOOL BillingReqExecute(const char* i_szServerIP, const int i_nServerPort, SBILLING_REQ_DATA * i_pReqData, SBILLING_RET_DATA * o_pRetData);



#endif // #ifndef _BILLING_LIB_H_

#ifndef _BILLING_NETPOWERTPE_H_
#define _BILLING_NETPOWERTPE_H_

// 2008-11-13 by cmkwon, �븸 Netpower_Tpe ���� �ý��� ���� - BillingNetpower.h ���� �߰�

#define NETBILL_SIZE_MAX_ACCOUNT_NAME			50		// �ִ� ������
#define NETBILL_SIZE_MAX_CHARACTER_NAME			50		// �ִ� ĳ���͸�
#define NETBILL_SIZE_MAX_ITEM_NUM				50		// �ִ� �����۹�ȣ ����
#define NETBILL_SIZE_MAX_ITEM_NAME				50		// �ִ� ������ �̸� ����
#define NETBILL_SIZE_MAX_IPADDRESS				16		// 

#define NETBILL_SERVICE_CODE_FOR_BILLING		"00003"		// 2008-11-13 by cmkwon, �븸 Netpower_Tpe ���� �ý��� ���� - �븸 Netpower_Tpe ���� ���� �ڵ�


//----------------------------------------------------------------
//-- ���۵����� ����ü
//----------------------------------------------------------------
struct SNETBILL_REQ_DATA
{
	int  nCommand;												// �������(1:�ܾ���ȸ, 2:�����۱���)
	char szAccountName[NETBILL_SIZE_MAX_ACCOUNT_NAME];		// ����������(����ڹ�ȣ)
	char szCharacterName[NETBILL_SIZE_MAX_CHARACTER_NAME];	// ����������(���ӳ��� ĳ���͸�)
	char szItemNum[NETBILL_SIZE_MAX_ITEM_NUM];				// ���� ������ ������(���� ������ ��ϵ� �����۸� ���Ű���)
	char szItemName[NETBILL_SIZE_MAX_ITEM_NAME];				// ���� ������ �̸�
	int  nBuyItemCount;											// ���� ������ ī��Ʈ
	int  nItemUnitPrice;										// ���� �������� 1���� ����
	char szUserIP[NETBILL_SIZE_MAX_IPADDRESS];				// ����������(������ IP)	
	int		GiftAccountUID;
	char	GiftAccountName[NETBILL_SIZE_MAX_ACCOUNT_NAME];
	char	GiftCharacterName[NETBILL_SIZE_MAX_CHARACTER_NAME];
};


//----------------------------------------------------------------
//-- ���ŵ����� ����ü
//----------------------------------------------------------------
struct SNETBILL_RET_DATA
{
	int  nRetErrCode;				// ó����� (=0: ����, <>0: ����), ERR_DB_XXXX
	int  nCashPoint;				// ������ �����Ͽ� ������ Point
	int  nBonusPoint;				// �̺�Ʈ Ȥ�� �ٸ� ������ ���� ������ Point
};


#endif // #ifndef _BILLING_NETPOWERTPE_H_

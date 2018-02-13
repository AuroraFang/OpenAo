#ifndef _BILLING_ARARIO_H_
#define _BILLING_ARARIO_H_

// 2008-12-18 by cmkwon, �Ϻ� Arario �ܺ�����, ���� ��� �����ϱ� - BillingArario.h ���� �߰�

#include "ArtpProtocol.h"
#include "ArBilling.h"
#pragma comment(lib, "ArBilling.lib")


#define ARABILL_SIZE_MAX_ACCOUNT_NAME			50		// �ִ� ������
#define ARABILL_SIZE_MAX_CHARACTER_NAME			50		// �ִ� ĳ���͸�
#define ARABILL_SIZE_MAX_ITEM_NUM				50		// �ִ� �����۹�ȣ ����
#define ARABILL_SIZE_MAX_ITEM_NAME				50		// �ִ� ������ �̸� ����
#define ARABILL_SIZE_MAX_IPADDRESS				16		// 

#define ARABILL_ACEONLINE_GAME_CODE				"CM001003"

//----------------------------------------------------------------
//-- ���۵����� ����ü
//----------------------------------------------------------------
struct SARABILL_REQ_DATA
{
	int  nCommand;												// �������(1:�ܾ���ȸ, 2:�����۱���)
	char szAccountName[ARABILL_SIZE_MAX_ACCOUNT_NAME];			// ����������(����ڹ�ȣ)
	char szCharacterName[ARABILL_SIZE_MAX_CHARACTER_NAME];		// ����������(���ӳ��� ĳ���͸�)
	UINT uiCharacterUID;										// ����������(ĳ���Ͱ�����ȣ)
	char szItemNum[ARABILL_SIZE_MAX_ITEM_NUM];					// ���� ������ ������(���� ������ ��ϵ� �����۸� ���Ű���)
	char szItemName[ARABILL_SIZE_MAX_ITEM_NAME];				// ���� ������ �̸�
	int  nBuyItemCount;											// ���� ������ ī��Ʈ
	int  nItemUnitPrice;										// ���� �������� 1���� ����
	char szUserIP[ARABILL_SIZE_MAX_IPADDRESS];					// ����������(������ IP)	
	int		GiftAccountUID;
	char	GiftAccountName[ARABILL_SIZE_MAX_ACCOUNT_NAME];
	char	GiftCharacterName[ARABILL_SIZE_MAX_CHARACTER_NAME];
	int		nCurTotalMoney;										// ������ ĳ�� ����Ʈ
};


//----------------------------------------------------------------
//-- ���ŵ����� ����ü
//----------------------------------------------------------------
struct SARABILL_RET_DATA
{
	int  nRetErrCode;				// ó����� (=0: ����, <>0: ����), ERR_DB_XXXX
	int  nCashPoint;				// ������ �����Ͽ� ������ Point
	int  nBonusPoint;				// �̺�Ʈ Ȥ�� �ٸ� ������ ���� ������ Point
};


#endif // #ifndef _BILLING_ARARIO_H_

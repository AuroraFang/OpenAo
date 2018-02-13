#ifndef _BILLING_WIKIGAMES_H_
#define _BILLING_WIKIGAMES_H_


#define WIKIBILL_SIZE_MAX_ACCOUNT_NAME			50		// �ִ� ������
#define WIKIBILL_SIZE_MAX_CHARACTER_NAME		50		// �ִ� ĳ���͸�
#define WIKIBILL_SIZE_MAX_ITEM_NUM				50		// �ִ� �����۹�ȣ ����
#define WIKIBILL_SIZE_MAX_ITEM_NAME				50		// �ִ� ������ �̸� ����
#define WIKIBILL_SIZE_MAX_IPADDRESS				16		// 

//-------------------------------------------------------------------------------
//--						Error code ����
#define WIKIBILL_ERR_DB_NO_ERROR							0
#define WIKIBILL_ERR_DB_IS_NOT_ACCOUNTNAME					1			// td_cash_bonus_point ���̺� �������� ����
#define WIKIBILL_ERR_DB_NOT_ENOUGH_MONEY					2			// ������ Point(CashPoint+BonusPoint)�� �����Ϸ��� �������� ���ݺ��� �۴�
#define WIKIBILL_ERR_DB_IS_NOT_GAMEUID						3			// �����Ϸ��� ������ ���� ������ ��ϵǾ� ���� ����
#define WIKIBILL_ERR_DB_INVALID_ITEM						4			// �����Ϸ��� �������� ��ȿ���� ���� - ti_item_info ���̺� ���ų� IsDisable �ʵ尡 0�� �ƴϴ�
#define WIKIBILL_ERR_DB_IS_NOT_EQUAL_ITEM_UNIT_PRICE		5			// ���� �������� 1���� ������ ���� ������ ��ϵȰͰ� �ٸ���
#define WIKIBILL_ERR_INVALID_BUILDING_NPC					6
#define WIKIBILL_ERR_INVALID_CASH_SHOP_DATA					7

#define WIKIBILL_ERR_DB_CONNECT_FAIL						100			// ���� DB Server�� ���� ����
#define WIKIBILL_ERR_DB_QUERY_ERROR							101			// DB Query ����
#define WIKIBILL_ERR_DB_QUERY_FETCH_ERROR					102			// DB Query ����
#define WIKIBILL_ERR_DB_INVALID_BILLING_COMMAND				110			// 
//-------------------------------------------------------------------------------



//----------------------------------------------------------------
//-- ���۵����� ����ü
//----------------------------------------------------------------
struct SWIKIBILL_REQ_DATA
{
	int  nCommand;												// �������(1:�ܾ���ȸ, 2:�����۱���)
	char szAccountName[WIKIBILL_SIZE_MAX_ACCOUNT_NAME];		// ����������(����ڹ�ȣ)
	char szCharacterName[WIKIBILL_SIZE_MAX_CHARACTER_NAME];	// ����������(���ӳ��� ĳ���͸�)
	char szItemNum[WIKIBILL_SIZE_MAX_ITEM_NUM];				// ���� ������ ������(���� ������ ��ϵ� �����۸� ���Ű���)
	char szItemName[WIKIBILL_SIZE_MAX_ITEM_NAME];				// ���� ������ �̸�
	int  nBuyItemCount;											// ���� ������ ī��Ʈ
	int  nItemUnitPrice;										// ���� �������� 1���� ����
	char szUserIP[WIKIBILL_SIZE_MAX_IPADDRESS];				// ����������(������ IP)	
	int		GiftAccountUID;
	char	GiftAccountName[WIKIBILL_SIZE_MAX_ACCOUNT_NAME];
	char	GiftCharacterName[WIKIBILL_SIZE_MAX_CHARACTER_NAME];
};


//----------------------------------------------------------------
//-- ���ŵ����� ����ü
//----------------------------------------------------------------
struct SWIKIBILL_RET_DATA
{
	int  nRetErrCode;				// ó����� (=0: ����, <>0: ����), ERR_DB_XXXX
	int  nCashPoint;				// ������ �����Ͽ� ������ Point
	int  nBonusPoint;				// �̺�Ʈ Ȥ�� �ٸ� ������ ���� ������ Point
};



#endif // #ifndef _BILLING_WIKIGAMES_H_

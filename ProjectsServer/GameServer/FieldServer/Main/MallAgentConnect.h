/*************************************************************************************/
/**
/** Query to MGame Mall
/**
/** By cskim �ۼ���: 2005/11/22
/**
/*************************************************************************************/

#ifdef MALLAGENTCONNECT_EXPORTS
#define MALLAGENTCONNECT_API __declspec(dllexport)
#else
#define MALLAGENTCONNECT_API __declspec(dllimport)
#endif

///////////////////////////////////////////////////////////////////////////////
// 2005-11-29 by cmkwon, ������ ��û�� ���� ������
#define MGAME_MALL_SPACECOWBOY_CPID					"KMSC2413"
#define MGAME_MALL_MONEY_SERVER_IP					"218.50.4.82"
#define MGAME_MALL_MONEY_SERVER_PORT				15024			// 2005-11-29 by cmkwon, 15021-->15024
#define MGAME_MALL_CPID_SIZE						9
#define MGAME_MALL_ITEMNUM_SIZE						21
#define MGAME_MALL_ITEMNAME_SIZE

//*************************************************************************************/
//
// MGame Mall�� ������ Data ����
//
typedef struct _MGAMEDATA
{
	char szCPID[9];			// CPID - MGame ������ �Ҵ��� ����ID (8�ڸ�, ��ҹ��� ����)
	char szUserID[21];		// ������ ����� (�빮��)
	char szGoodsNo[21];		// ��ǰ��ȣ
	char szItemName[51];	// ������ �̸�
	int iPrice;				// ������ �ݾ�
	char szUserIP[16];		// ������ IP
	char szOthers[51];		// ��Ÿ
} MGAMEDATA, *LPMGAMEDATA;

//*************************************************************************************/
//
// MGame Mall�κ��� ���޹޴� �����
//

typedef struct _MGAMERET
{
	int iReturn;	// Return Code: 1 - ����, 4 - ��ݽ���, 5 - ���� �������� ����
	int iMoney;		// ���� �ܾ�
	int iPMoney;	// ��ǰ�� �ܾ�
} MGAMERET, *LPMGAMERET;

//*************************************************************************************/
//
// MGame Mall���� �����ϴ� �Լ�
//
// Return Value: bool - ����/���� ����, ������ true, ���н� false�� return��
// 
// Parameter   : const char* szHostName		[in]	Agent Server Address (MGame ������ ������)
//               const unsigned short nPort [in]	Agent Server Port	 (MGame ������ ������)
//				 const struct MGDATA* pData	[in]	������ Data
//				 struct MGRET* pRet			[out]	���޹��� �����
//
MALLAGENTCONNECT_API bool MallAgnetConnect(const char* szHostName, const unsigned short nPort, const LPMGAMEDATA pData, LPMGAMERET pRet);

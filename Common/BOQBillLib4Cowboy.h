
// 2006-08-24 by cmkwon, billing server ip changed(64.127.103.228 --> 192.168.7.111)
#define BILL_SERVER_INFO "192.168.7.111:24701"

#ifdef BOQBILL_EXPORTS
	#define BOQBILL_API extern "C" __declspec(dllexport)
#else
	#define BOQBILL_API extern "C" __declspec(dllimport)
	#pragma comment(lib, "BOQBillLib4Cowboy.lib")					// 2007-09-13 by cmkwon, �� ������ ���� ���̺귯�� ���� �ε�	
#endif

//----------------------------------------------------------------
//--���۵����� ����ü
//----------------------------------------------------------------
typedef struct _BILLSENDDATA
{	
	int  nCommand;					// �������(1:�ܾ���ȸ, 2:�����۱���)
	char szUserNo[10+1];			// ����������(����ڹ�ȣ)	
	char szUserID[20+1];			// ����������(����ھ��̵�)
	char szUserName[50+1];			// ����������(������̸�)
	char szGameCharacterName[30+1];	// ����������(���ӳ��� ĳ���͸�)
	char szUserIP[16];				// ����������(������ IP)
	char szGameItemID[20+1];		// ����������(������ ���ž����۹�ȣ)	
	char szReserved[51];			// �����ʵ�
} BILLSENDDATA, *LPBILLSENDDATA;


//----------------------------------------------------------------
//--���ŵ����� ����ü
//----------------------------------------------------------------
typedef struct _BILLRETDATA
{
	int  nRetVal;					// ó����� (0 : ����, 0 <> ����)
	char szRetMsg[1024+1];			// ó���޽���(�����߻��� ����)
	int  nRealCash;					// RealCash �ܾ�
	int  nBonusCash;				// BonusCash �ܾ�
	char szChargeNo[30+1];			// ���ݹ�ȣ(�����۱��� ���Key)
} BILLRETDATA, *LPBILLRETDATA;


//----------------------------------------------------------------
// �ܾ���ȸ,������ ���� �ܺγ��� �Լ�
//
// Return Value: int - ��� ����/���� ����(��� ������ 0, ���н� -1�� return��,
//                     �ܼ��� ��ɽ��༺�����θ� �ǹ���. ��ɾ� ó������� Ȯ���Ϸ��� BILLRETDATA�� nRetVal����!
// Parameter   : const char* szServerInfo	[in]	�������󼭹����� "IP:Port"
//				 const LPBILLSENDDATA pData	[in]	������ Data
//				 LPBILLRETDATA pRet			[out]	���޹��� �����
//
//----------------------------------------------------------------
BOQBILL_API int BillCmdExecute(const char* szServerInfo, const LPBILLSENDDATA pData, LPBILLRETDATA pRet);


// 2006-06-05 by cmkwon, ���� �ڵ�([F]:frequently)
// 		2272	account information check failed.
// 		2274	[F]user status is invalid.(currently not available)
// 		2275	bad user check failed.
// 		2274	[F]blocked because bad user.
// 		2281	[F]get item information error.
// 		2282	get cp information error.
// 		2262	charge amount invalid.
// 		2264	[F]insufficient cash.
// 		2263	chargeno generation failed.
// 		2267	purchase data insert failed.
// 		2268	cash use detail insert failed.
// 		2269	item hold information insert failed.
// 		2033	account modification failed.
// 		2270	get cash for charge failed.
// 		2271	get cash for charge failed.
// 		2272	modify remaincash failed.
// 		2274	modify remaincash failed.
// 		2276	cash use detail insert failed.
// 		2277	exit loop while charge not completed.
// 		2278	purchase history insert failed.
// 		2267	item hold information insert failed.
// 		2268	mileage information insert failed.
// 		2033	account modification failed.
// 		2034	available event query failed.
// 		2267	give event item failed.
// 		2268	applied count update error.
// 
// 		�ܾ���ȸ	
// 		2017	account query error.
// 		2018	RealCash Balance abnormal.
// 		2018	BonusCash Balance abnormal.
// 		2018	Mileage Balance abnormal.
// 
// 
//		��Ÿ	Network Error

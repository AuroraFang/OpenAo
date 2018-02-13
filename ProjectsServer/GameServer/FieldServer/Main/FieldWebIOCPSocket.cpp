// FieldWebIOCPSocket.cpp: implementation of the CFieldWebIOCPSocket class.
// 2013-03-13 by hskim, �� ĳ�� ����
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FieldWebIOCPSocket.h"
#include "FieldWebIOCP.h"
#include "FieldGlobal.h"
#include "AtumError.h"
#include "GenericHTTPClient.h"
#include "XmlRpc/XmlRpc.h"
#include "FieldIOCP.h"
#include "FieldIOCPSocket.h"

using namespace XmlRpc;

#if defined(_DEBUG)
#pragma comment(lib, "xmlrpc_d.lib")
#else
#pragma comment(lib, "xmlrpc.lib")
#endif

CFieldWebIOCP		*CFieldWebIOCPSocket::ms_pFieldWebIOCP = NULL;

////////////////////////////////////////////////////////////////////////////////
// XML-RPC �޼ҽ� ���
////////////////////////////////////////////////////////////////////////////////

XMLRPC_METHOD_INFO XmlRpcMethodInfo[] = 
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// [Method Name], [CallBack Function]
	// [Method Param Count], [Type : TypeInvalid, TypeBoolean, TypeInt, TypeDouble, TypeString, TypeDateTime, TypeBase64, TypeArray, TypeStruct]


	////////////////////////////////
	// XMLRPC �������� ���� �޼ҵ�

	"GetServerStatus",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GET_SERVER_STATUS,
	1, { XmlRpc::XmlRpcValue::TypeInt },


	////////////////////////////////
	// Game ���� �޼ҵ�

	"Game.InsertItem",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GAME_ITEM_INSERT,
	7, { XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt },

	"Game.DeleteItem",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GAME_ITEM_DELETE,
	6, { XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt },

	"Billing.BuyItem",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_BUY_ITEM,
	9, { XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeInt },

	"Billing.GetCash",	&CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_GET_CASH,
	5, { XmlRpc::XmlRpcValue::TypeInt, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString, XmlRpc::XmlRpcValue::TypeString },


	////////////////////////////////

	NULL, NULL, 0, { 0, }		// �ش� �κ��� ��� �޼ҵ� ���� ��Ÿ��, ������ ���� ���� ���� ��~~~~!!
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldWebIOCPSocket::CFieldWebIOCPSocket()
{
	m_PeerSocketType	= ST_INVALID_TYPE;
	m_XmlRpc.Clear();
}

CFieldWebIOCPSocket::~CFieldWebIOCPSocket()
{
}

BOOL CFieldWebIOCPSocket::OnRecvdPacketFieldWebServer(const char* pPacket, int nLength, BYTE nSeq, char* pPeerIP, int nPeerPort, SThreadInfo *i_pThreadInfo)
{
	// XML ó��

	int iRPCMethodCount = 0;
	int iRtnValue = 0;

	//DBGOUT("==== INPUT HTTP ======================================================\r\n");
	//DBGOUT(pPacket);
	//DBGOUT("======================================================================\r\n");

	if( TRUE == m_XmlRpc.ParseFormXML((char *)pPacket) && TRUE == m_XmlRpc.IsVaild() )
	{
		XmlRpc::XmlRpcValue ParamsValue = m_XmlRpc.GetParamsValue();

		do
		{	
			if( 0 == strcmp("", XmlRpcMethodInfo[iRPCMethodCount].szMethodName) &&  NULL == XmlRpcMethodInfo[iRPCMethodCount].pFunction )
			{
				m_XmlRpc.GenerateFaultResponse(GetErrorXMLRPCString(XMLRPC_ERROR_NOT_FOUND_RPC_METHOD), XMLRPC_ERROR_NOT_FOUND_RPC_METHOD);

				break;
			}

			if( 0 == strcmp(m_XmlRpc.GetMethodName(), XmlRpcMethodInfo[iRPCMethodCount].szMethodName) )
			{
				if( XmlRpcMethodInfo[iRPCMethodCount].ParamCount != ParamsValue.size() ) return FALSE;		// ���� ����ó�� �ؾ���

				int a;
				for(a=0; a<ParamsValue.size(); a++)
				{
					if( XmlRpcMethodInfo[iRPCMethodCount].ParamType[a] != ParamsValue[a].getType() )
					{
						m_XmlRpc.GenerateFaultResponse(GetErrorXMLRPCString(XMLRPC_ERROR_NOT_MATCHED_PARAMETER_TYPE), XMLRPC_ERROR_NOT_MATCHED_PARAMETER_TYPE);
					}
				}

				if( a != ParamsValue.size() ) break;

				iRtnValue = (this->*XmlRpcMethodInfo[iRPCMethodCount].pFunction)(ParamsValue);

				if( 0 <= iRtnValue )
				{
					m_XmlRpc.GenerateResponse(iRtnValue);
				}
				else
				{
					m_XmlRpc.GenerateFaultResponse(GetErrorXMLRPCString(iRtnValue), iRtnValue);
				}

				break;
			}

			iRPCMethodCount++;

		} while( 1 );
	}
	else
	{
		m_XmlRpc.GenerateFaultResponse(GetErrorXMLRPCString(XMLRPC_ERROR_INVALID_XML), XMLRPC_ERROR_INVALID_XML);
	}

	SendAddRawData((unsigned char*)m_XmlRpc.m_szResponse.c_str(), m_XmlRpc.m_szResponse.size(), TRUE);

	return TRUE;
}

void CFieldWebIOCPSocket::OnConnect(void)
{
	char szSystemLog[256];
	sprintf(szSystemLog, "Socket Connect Web SocketIndex[%3d] PeerIP[%15s] Port[%4d]\r\n",
		this->GetClientArrayIndex(), m_szPeerIP, m_nPeerPort);
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	m_PeerSocketType		= ST_INVALID_TYPE;

	CIOCPSocket::OnConnect();
}

void CFieldWebIOCPSocket::OnClose(int reason)
{
	char szSystemLog[1024];

	sprintf(szSystemLog, "Socket Closed Web SocketIndex[%3d] SocketType[%d] PeerIP[%15s] Port[%4d] MaxWriteBufCounts[%4d] ==> reason %d[%#08X]\r\n",
		this->GetClientArrayIndex(), m_PeerSocketType, m_szPeerIP, m_nPeerPort, m_nMaxWriteBufCounts, reason, reason);
	g_pGlobal->WriteSystemLog(szSystemLog);
	DBGOUT(szSystemLog);

	m_PeerSocketType = ST_INVALID_TYPE;

	CIOCPSocket::OnClose(30);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// XML-RPC �޼ҽ� ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GET_SERVER_STATUS(XmlRpc::XmlRpcValue &rParamsValue)
{
	UID32_t nServerID = int(rParamsValue[0]);

	if( NULL == GetFieldIOCP() ) 
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID()  == nServerID )
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	return 0;
}

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GAME_ITEM_INSERT(XmlRpc::XmlRpcValue &rParamsValue)
{
	INT nServerID = int(rParamsValue[0]);
	char szUserIP[SIZE_MAX_IPADDRESS]; strncpy(szUserIP, std::string(rParamsValue[1]).c_str(), sizeof(szUserIP));
	UID32_t	nAccountUID = atoi(std::string(rParamsValue[2]).c_str());
	UID32_t	nCharacterUID = atoi(std::string(rParamsValue[3]).c_str());
	ItemNum_t nItemNum = int(rParamsValue[4]);
	INT nCount = int(rParamsValue[5]);
	INT nItemStorage = int(rParamsValue[6]);

	INT nRtnValue = 0;

	// ī���ͺ� ������ �ѵ� �ʰ� ȸ�� ���� (20��)

	if( NULL == GetFieldIOCP() )									// FieldIOCP ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID() != nServerID )				// ���� ID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )						// ���� ���� ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	if(0 == strncmp(szUserIP, "", SIZE_MAX_IPADDRESS))				// ������ Ȯ��
	{
		return XMLRPC_ERROR_INVALID_USER_IP;
	}

	if(0 == nAccountUID || INVALID_UID32 == nAccountUID)			// ���� UID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_ACCOUNTUID;
	}

	if(0 == nCharacterUID || INVALID_UID32 == nCharacterUID)		// ĳ���� UID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_CHARACTERUID;
	}

	ITEM * pItemInfo = GetFieldIOCP()->GetItemInfo(nItemNum);
	if( NULL == pItemInfo )											// ������ ��ȣ Ȯ��
	{
		return XMLRPC_ERROR_INVALID_ITEMNUM;
	}

	if( IS_COUNTABLE_ITEM(pItemInfo->Kind) )						// ������ �� ���� Ȯ��
	{
		if( nCount < MIN_WEB_GAME_INSERT_COUNTABLEITEM_COUNT || nCount > MAX_WEB_GAME_INSERT_COUNTABLEITEM_COUNT )
		{
			return XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT;
		}
	}
	else
	{
		if( nCount < MIN_WEB_GAME_INSERT_NONCOUNTABLEITEM_COUNT || nCount > MAX_WEB_GAME_INSERT_NONCOUNTABLEITEM_COUNT )
		{
			return XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT;
		}		
	}

	if( ITEM_IN_CHARACTER != nItemStorage && ITEM_IN_STORE != nItemStorage )	// ������ ���� ��ġ Ȯ��
	{
		return XMLRPC_ERROR_INVALID_ITEM_STORAGE;
	}

	if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD)		// �����̾� (�����) �������� ���� �Ұ�
		|| pItemInfo->IsExistDesParam(DES_FIELD_STORE))									// �ʵ� â�� �̿�� �������� ���� �Ұ�
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCP()->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID);

	if( NULL != pFieldIOCPSocket && 0 == nItemStorage )		// �Ʒ����� Ȱ������ ���� �������� ������ ȣ�� �ϵ��� ��� �߰�
	{
		////////////////////////////////////////////////
		// ĳ���� �α��� ���� �׸��� �κ��丮�� �߰�
		////////////////////////////////////////////////

		if( nAccountUID != pFieldIOCPSocket->m_character.AccountUniqueNumber				// ������ ĳ���� ���� Ȯ��
			|| nCharacterUID != pFieldIOCPSocket->m_character.CharacterUniqueNumber )
		{
			return XMLRPC_ERROR_NOT_MATCHED_USER_INFO;
		}

		mt_auto_lock igLock(&pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);

		if (!pFieldIOCPSocket->m_ItemManager.InsertItemBaseByItemNum(pItemInfo->ItemNum, nCount, IUT_ADMIN))		// <== ���� IUI �߰� �ۼ�
		{
			return XMLRPC_ERROR_INTERNAL_SERVER;
		}

		nRtnValue = nCount;

		// Log 
		//ITEM_GENERAL itemForLog(pItem);
		//CAtumLogSender::SendLogMessageITEMAddItemByCommand(this, &itemForLog, count);
	}
	else
	{
		///////////////////////////////////////////////////////////
		// ĳ���� �α׾ƿ� ���� Ȥ�� �α��� ���̳� â�� �߰�
		///////////////////////////////////////////////////////////

		// ������ ĳ���� ���� Ȯ�� (�������� Ȯ��)
		// ������ ĳ�������� Ȯ�� (�������� Ȯ��)

		QPARAM_INSERT_STOREITEM_FROM_XMLRPC InsertStoreItem;
		memset(&InsertStoreItem, 0, sizeof(InsertStoreItem));

		InsertStoreItem.AccountUID = nAccountUID;
		InsertStoreItem.CharacterUID = nCharacterUID;
		InsertStoreItem.ItemNum = nItemNum;
		InsertStoreItem.ItemCount = nCount;
		InsertStoreItem.ItemStorage = nItemStorage;
		InsertStoreItem.Wear = 0;
		InsertStoreItem.ItemWindowIndex = 99;
		InsertStoreItem.BuyCashItem = 0;

		GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_InsertStoreItemFromXMLRPC, NULL, &InsertStoreItem);

		// Log 
		//ITEM_GENERAL itemForLog(pItem);
		//CAtumLogSender::SendLogMessageITEMAddItemByCommand(this, &itemForLog, count);

		nRtnValue = InsertStoreItem.RtnValue;
	}

	return nRtnValue;
}

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_GAME_ITEM_DELETE(XmlRpc::XmlRpcValue &rParamsValue)
{
	INT nServerID = int(rParamsValue[0]);
	char szUserIP[SIZE_MAX_IPADDRESS]; strncpy(szUserIP, std::string(rParamsValue[1]).c_str(), sizeof(szUserIP));
	UID32_t	nAccountUID = atoi(std::string(rParamsValue[2]).c_str());
	UID32_t	nCharacterUID = atoi(std::string(rParamsValue[3]).c_str());
	ItemNum_t nItemNum = int(rParamsValue[4]);
	INT nCount = int(rParamsValue[5]);

	// ��ȹ�ڿ��� ���� �ؾ��ϴ� ���� 
	// �ش� API ���� �����Ǵ� �������� �Ʒ���/���Ǵ�Ƽ ���� ȹ���� �Ǿ�� �ȵ� 
	// ���� ȹ���� �Ͼ�� �Ǹ� ������ ������ �Ʒ���/���Ǵ�Ƽ���� ȹ���� �������� �����
	
	INT nRtnValue = 0;

	if( NULL == GetFieldIOCP() )									// FieldIOCP ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID() != nServerID )				// ���� ID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )						// ���� ���� ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	if(0 == strncmp(szUserIP, "", SIZE_MAX_IPADDRESS))				// ������ Ȯ��
	{
		return XMLRPC_ERROR_INVALID_USER_IP;
	}

	if(0 == nAccountUID || INVALID_UID32 == nAccountUID)			// ���� UID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_ACCOUNTUID;
	}

	if(0 == nCharacterUID || INVALID_UID32 == nCharacterUID)		// ĳ���� UID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_CHARACTERUID;
	}

	ITEM * pItemInfo = GetFieldIOCP()->GetItemInfo(nItemNum);
	if( NULL == pItemInfo )											// ������ ��ȣ Ȯ��
	{
		return XMLRPC_ERROR_INVALID_ITEMNUM;
	}

	if( IS_COUNTABLE_ITEM(pItemInfo->Kind) )						// ������ �� ���� Ȯ��
	{
		if( nCount < MIN_WEB_GAME_INSERT_COUNTABLEITEM_COUNT || nCount > MAX_WEB_GAME_INSERT_COUNTABLEITEM_COUNT )
		{
			return XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT;
		}
	}
	else
	{
		return XMLRPC_ERROR_SUPPORT_ONLY_COUNTABLE_ITEM;			// ��ī���ͺ� �������� �������� ����
	}

	if( FALSE == pItemInfo->IsExistDesParam(DES_WEB_DELETE_ITEM) )		// DES_WEB_DELETE_ITEM �Ӽ����� �־�߸� ���� ����
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD)		// �����̾� (�����) �������� ���� �Ұ�
		|| pItemInfo->IsExistDesParam(DES_FIELD_STORE))									// �ʵ� â�� �̿�� �������� ���� �Ұ�
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCP()->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID);

	if( NULL != pFieldIOCPSocket )			// �Ʒ����� Ȱ������ ���� �������� ������ ȣ�� �ϵ��� ��� �߰�	
	{
		////////////////////////////////////////////////
		// ĳ���� �α��� ���� �׸��� �κ��丮�� ����
		////////////////////////////////////////////////

		int nInventoryItemCount = 0;

		if( nAccountUID != pFieldIOCPSocket->m_character.AccountUniqueNumber				// ������ ĳ���� ���� Ȯ��
			|| nCharacterUID != pFieldIOCPSocket->m_character.CharacterUniqueNumber )
		{
			return XMLRPC_ERROR_NOT_MATCHED_USER_INFO;
		}

		mt_auto_lock igLock(&pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral);

		ITEM_GENERAL *pItemGeneralInventory = pFieldIOCPSocket->m_ItemManager.GetFirstItemGeneralByItemNum(nItemNum, ITEM_IN_CHARACTER);
		if( NULL == pItemGeneralInventory )
		{
			return XMLRPC_ERROR_NOT_ENOUGH_ITEM_COUNT;
		}

		nInventoryItemCount = pItemGeneralInventory->CurrentCount;

		if( nInventoryItemCount < nCount )
		{
			return XMLRPC_ERROR_NOT_ENOUGH_ITEM_COUNT;
		}

		if( FALSE == pFieldIOCPSocket->m_ItemManager.UpdateItemCountByPointer(pItemGeneralInventory, -1 * nCount, IUT_GENERAL) )		// <== ���� IUI �߰� �ۼ�
		{
			// �ý��� �α�

			return XMLRPC_ERROR_INTERNAL_SERVER;
		}

		nRtnValue = nCount;
	}
	else
	{
		// ������ ĳ���� ���� Ȯ�� (�������� Ȯ��)
		// ������ ĳ�������� Ȯ�� (�������� Ȯ��)

		QPARAM_DELETE_STOREITEM_FROM_XMLRPC CharInfo;
		memset(&CharInfo, 0, sizeof(CharInfo));

		CharInfo.ReturnValue = -1;
		CharInfo.AccountUID = nAccountUID;
		CharInfo.CharacterUID = nCharacterUID;
		CharInfo.ItemNum = nItemNum;
		CharInfo.ItemCount = nCount;

		GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_DeleteStoreItemFromXMLRPC, NULL, &CharInfo);

		nRtnValue = CharInfo.ReturnValue;
	}

	return nRtnValue;
}

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_BUY_ITEM(XmlRpc::XmlRpcValue &rParamsValue)
{
	INT nServerID = int(rParamsValue[0]);
	char szUserIP[SIZE_MAX_IPADDRESS]; strncpy(szUserIP, std::string(rParamsValue[1]).c_str(), sizeof(szUserIP));
	UID32_t	nAccountUID = atoi(std::string(rParamsValue[2]).c_str());
	UID32_t	nCharacterUID = atoi(std::string(rParamsValue[3]).c_str());
	UID32_t nGiftCharacterUID = atoi(std::string(rParamsValue[4]).c_str());
	ItemNum_t nItemNum = int(rParamsValue[5]);
	INT nCount = int(rParamsValue[6]);
	char szAuthenticationKey[SIZE_MAX_WEB_AUTHENTICATION_KEY]; strncpy(szAuthenticationKey, std::string(rParamsValue[7]).c_str(), sizeof(szAuthenticationKey));
	INT nItemStorage = int(rParamsValue[8]);

	INT nRtnValue = 0;

	if( NULL == GetFieldIOCP() )									// FieldIOCP ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID() != nServerID )				// ���� ID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )						// ���� ���� ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	if(0 == strncmp(szUserIP, "", SIZE_MAX_IPADDRESS))				// ������ Ȯ��
	{
		return XMLRPC_ERROR_INVALID_USER_IP;
	}

	if(0 == nAccountUID || INVALID_UID32 == nAccountUID)			// ���� UID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_ACCOUNTUID;
	}

	if(0 == nCharacterUID || INVALID_UID32 == nCharacterUID)		// ĳ���� UID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_CHARACTERUID;
	}

	ITEM * pItemInfo = GetFieldIOCP()->GetItemInfo(nItemNum);
	if( NULL == pItemInfo )											// ������ ��ȣ Ȯ��
	{
		return XMLRPC_ERROR_INVALID_ITEMNUM;
	}

	if( nCount < MIN_WEB_BILLING_BUY_COUNT || nCount > MAX_WEB_BILLING_BUY_COUNT )		// ������ ���� Ȯ��
	{
		return XMLRPC_ERROR_OUT_OF_RANGE_ITEMCOUNT;
	}

	if( ITEM_IN_CHARACTER != nItemStorage && ITEM_IN_STORE != nItemStorage )			// ������ ���� ��ġ Ȯ��
	{
		return XMLRPC_ERROR_INVALID_ITEM_STORAGE;
	}

	if( FALSE == g_pFieldGlobal->m_bCashShopServiceFlag )			// ĳ�ü� ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_CASH_SHOP_NO_SERVICE;	
	}

	mt_auto_lock igLock(NULL);

	CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCP()->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID);
	CFieldIOCPSocket TempFeildIOCPSocket;

	QPARAM_GET_CHARACTERINFO_FROM_XMLRPC CharInfo;
	memset(&CharInfo, 0, sizeof(CharInfo));

	CharInfo.ReturnValue = -1;
	CharInfo.AccountUID = nAccountUID;
	CharInfo.CharacterUID = nCharacterUID;
	CharInfo.GiftCharacterUID = nGiftCharacterUID;

	GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_GetCharacterInfoFromXMLRPC, NULL, &CharInfo);

	if( 0 > CharInfo.ReturnValue )
	{
		return CharInfo.ReturnValue;
	}

	if( NULL != pFieldIOCPSocket && 0 == nItemStorage )		// �Ʒ����� Ȱ������ ��� �������� ������ ȣ�� �ϵ��� ��� �߰�
	{
		////////////////////////////////////////////////
		// ĳ���� �α��� ���� �׸��� �κ��丮�� �߰�
		////////////////////////////////////////////////

		// �� ����Ű Ȯ��

		if( nAccountUID != pFieldIOCPSocket->m_character.AccountUniqueNumber				// ������ ĳ���� ���� Ȯ��
			|| nCharacterUID != pFieldIOCPSocket->m_character.CharacterUniqueNumber )
		{
			return XMLRPC_ERROR_NOT_MATCHED_USER_INFO;
		}

		igLock.m_pMTLock = &pFieldIOCPSocket->m_ItemManager.m_mapItemGeneral;
		igLock.m_pMTLock->lock();
	}
	else
	{
		// �����̾� (�����) �������� ����
		// �ʵ� â�� �̿�� �������� ����

		// ������ ĳ���� ���� Ȯ�� (�������� Ȯ��)		OK
		// ������ ĳ�������� Ȯ�� (�������� Ȯ��)		OK

		// ���� �о� ����

		/*
		QPARAM_GET_CHARACTERINFO_FROM_XMLRPC CharInfo;
		memset(&CharInfo, 0, sizeof(CharInfo));

		CharInfo.ReturnValue = -1;
		CharInfo.AccountUID = nAccountUID;
		CharInfo.CharacterUID = nCharacterUID;

		GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_GetCharacterInfoFromXMLRPC, NULL, &CharInfo);

		if( 0 > CharInfo.ReturnValue )
		{
			return CharInfo.ReturnValue;
		}
		*/

		util::strncpy(TempFeildIOCPSocket.m_character.AccountName, CharInfo.AccountName, SIZE_MAX_ACCOUNT_NAME);
		util::strncpy(TempFeildIOCPSocket.m_character.CharacterName, CharInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
		TempFeildIOCPSocket.SetPeerAddress(szUserIP, 0);
		TempFeildIOCPSocket.m_character.Race = CharInfo.Race;

		pFieldIOCPSocket = &TempFeildIOCPSocket;
	}

	pFieldIOCPSocket->m_giveTargetCharacter.AccountUID0 = CharInfo.GiftAccountUID;
	pFieldIOCPSocket->m_giveTargetCharacter.CharacterUID0 = CharInfo.GiftCharacterUID;

	/*
	if( FALSE == GetFieldIOCP()->IsCashShopItemNoLock(nItemNum) )		// ������ �ִ� ���������� Ȯ��
	{
		return XMLRPC_ERROR_NOT_FOUND_BILLING_ITEM_LIST;
	}
	*/

	if( !COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM) )		// ĳ�� ���������� Ȯ��
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	if( COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_NO_TRANSFER | ITEM_ATTR_KILL_MARK_ITEM) )		
	{
		return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
	}

	if( TRUE == IS_VALID_UNIQUE_NUMBER(nGiftCharacterUID) )		// ���� ���� ���� Ȯ��
	{
		if( nCharacterUID == nGiftCharacterUID )
		{
			return XMLRPC_ERROR_INVALID_PEER_CHARACTER;
		}

		if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD) || pItemInfo->IsExistDesParam(DES_FIELD_STORE))		// �����̾�/â�� ������ �����ϱ� �Ұ�
		{
			return XMLRPC_ERROR_NOT_ACCEPTED_ITEMNUM;
		}
	}

	// ���� ó��

	INT nTotalMoney = 0;
	INT nMCash = 0;
	INT nGiftCard = 0;

	if( ERR_NO_ERROR != pFieldIOCPSocket->CashItemMoneyPay(&nTotalMoney, &nMCash, &nGiftCard) )
	{
		return XMLRPC_ERROR_BILLING_ERROR;
	}

	if(COMPARE_BIT_FLAG(pItemInfo->ItemAttribute, ITEM_ATTR_CASH_ITEM_PREMIUM_CARD))
	{
		// �ɹ��� ������ ó��
	}
	else
	{
		// ����ȭ ������ ó��
#ifdef SHOP_PRICES_PER_BUILDING_NPC
		CShopInfo* shopInfo = pFieldIOCPSocket->m_pCurrentFieldMapChannel->m_pFieldMapProject->GetCashShopInfo();
		INT64 n64SumOfPrice;
		if (shopInfo)
		{
			n64SumOfPrice = shopInfo->GetSHOP_ITEMPtr(pItemInfo->ItemNum)->Price;
		}
		else
		{
			g_pFieldGlobal->WriteSystemLogEX(TRUE, "[ERROR] CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_BUY_ITEM. Web Billing Error. The Cash Shop Building NPC is not available!\r\n");
			return XMLRPC_ERROR_BILLING_ERROR;
		}
#else
		INT64 n64SumOfPrice	= pItemInfo->CashPrice * (nCount/pItemInfo->MinTradeQuantity);
#endif // SHOP_PRICES_PER_BUILDING_NPC
		if (n64SumOfPrice > nTotalMoney)
		{
			return XMLRPC_ERROR_NEED_MORE_CASH;
		}


#ifdef SHOP_PRICES_PER_BUILDING_NPC
		int nRetedErr = pFieldIOCPSocket->CashItemMoneyPay(&nTotalMoney
			, &nMCash
			, &nGiftCard
			, pItemInfo
			, shopInfo->GetSHOP_ITEMPtr(pItemInfo->ItemNum)->Price
			, n64SumOfPrice
			, nGiftCharacterUID
			, nTotalMoney);
#else
		int nRetedErr = pFieldIOCPSocket->CashItemMoneyPay(&nTotalMoney
			, &nMCash
			, &nGiftCard
			, pItemInfo
			, n64SumOfPrice
			, nGiftCharacterUID
			, nTotalMoney);
#endif // SHOP_PRICES_PER_BUILDING_NPC

		
		if(ERR_NO_ERROR != nRetedErr)
		{
			SendErrorMessage(T_FC_SHOP_BUY_CASH_ITEM, nRetedErr);
			return RES_BREAK;
		}

		if(FALSE == IS_VALID_UNIQUE_NUMBER(nGiftCharacterUID))
		{
			if( &TempFeildIOCPSocket != pFieldIOCPSocket && 0 == nItemStorage )		// �Ʒ����� Ȱ������ ���� �������� ������ ȣ�� �ϵ��� ��� �߰�
			{
				////////////////////////////////////////////////
				// ĳ���� �α��� ���� �׸��� �κ��丮�� �߰�
				////////////////////////////////////////////////

				pFieldIOCPSocket->m_ItemManager.InsertItemBaseByItemNum(pItemInfo->ItemNum, nCount, IUT_ADMIN);
				nRtnValue = nCount;

				// Log 
				//ITEM_GENERAL itemForLog(pItem);
				//CAtumLogSender::SendLogMessageITEMAddItemByCommand(this, &itemForLog, count);
			}
			else
			{
				///////////////////////////////////////////////////////////
				// ĳ���� �α׾ƿ� ���� Ȥ�� �α��� ���̳� â�� �߰�
				///////////////////////////////////////////////////////////

				// ������ ĳ���� ���� Ȯ�� (�̹� Ȯ��)
				// ������ ĳ�������� Ȯ�� (�̹� Ȯ��)
				
				// �����ϱ� ��� Ȯ��

				QPARAM_INSERT_STOREITEM_FROM_XMLRPC InsertStoreItem;
				memset(&InsertStoreItem, 0, sizeof(InsertStoreItem));

				InsertStoreItem.AccountUID = nAccountUID;
				InsertStoreItem.CharacterUID = nCharacterUID;
				InsertStoreItem.ItemNum = nItemNum;
				InsertStoreItem.ItemCount = nCount;
				InsertStoreItem.ItemStorage = nItemStorage;
				InsertStoreItem.Wear = 0;
				InsertStoreItem.ItemWindowIndex = 99;
				InsertStoreItem.BuyCashItem = 0;

				GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_InsertStoreItemFromXMLRPC, NULL, &InsertStoreItem);

				// Log 
				//ITEM_GENERAL itemForLog(pItem);
				//CAtumLogSender::SendLogMessageITEMAddItemByCommand(this, &itemForLog, count);

				nRtnValue = InsertStoreItem.RtnValue;
			}
		}
		else
		{
			pFieldIOCPSocket->GiveItem(pItemInfo, nCount, IUT_ADMIN);		// �ش� ĳ���� â��� ������ �߰�

			nRtnValue = nCount;
		}
	}

	return nRtnValue;
}

int CFieldWebIOCPSocket::Process_XMLRPC_METHOD_BILLING_GET_CASH(XmlRpc::XmlRpcValue &rParamsValue)
{
	INT nServerID = int(rParamsValue[0]);
	char szUserIP[SIZE_MAX_IPADDRESS]; strncpy(szUserIP, std::string(rParamsValue[1]).c_str(), sizeof(szUserIP));
	UID32_t	nAccountUID = atoi(std::string(rParamsValue[2]).c_str());
	UID32_t	nCharacterUID = atoi(std::string(rParamsValue[3]).c_str());
	char szAuthenticationKey[SIZE_MAX_WEB_AUTHENTICATION_KEY]; strncpy(szAuthenticationKey, std::string(rParamsValue[4]).c_str(), sizeof(szAuthenticationKey));

	INT nRtnValue = 0;

	if( NULL == GetFieldIOCP() )									// FieldIOCP ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INTERNAL_SERVER;
	}

	if( g_pFieldGlobal->GetServerID() != nServerID )				// ���� ID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_SERVER_ID;		
	}

	if( FALSE == GetFieldIOCP()->IsServerOn() )						// ���� ���� ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_DISABLE_SERVER;
	}

	if(0 == strncmp(szUserIP, "", SIZE_MAX_IPADDRESS))				// ������ Ȯ��
	{
		return XMLRPC_ERROR_INVALID_USER_IP;
	}

	if(0 == nAccountUID || INVALID_UID32 == nAccountUID)			// ���� UID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_ACCOUNTUID;
	}

	if(0 == nCharacterUID || INVALID_UID32 == nCharacterUID)		// ĳ���� UID ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_INVALID_CHARACTERUID;
	}

	if( FALSE == g_pFieldGlobal->m_bCashShopServiceFlag )			// ĳ�ü� ���� ���� Ȯ��
	{
		return XMLRPC_ERROR_CASH_SHOP_NO_SERVICE;	
	}

	// ���� ó��

	INT nTotalMoney = 0;
	INT nMCash = 0;
	INT nGiftCard = 0;

	CFieldIOCPSocket *pFieldIOCPSocket = GetFieldIOCP()->GetFieldIOCPSocketByCharacterUniqueNumber(nCharacterUID);
	CFieldIOCPSocket TempFeildIOCPSocket;

	if( NULL != pFieldIOCPSocket )
	{
		// �� ����Ű Ȯ��

		/*
		if( FALSE == pFieldIOCPSocket->IsCashAuthenticationKey(szAuthenticationKey) && FALSE == pFieldIOCPSocket->IsCashAuthenticationKey("") )
		{
			return 	XMLRPC_ERROR_INVALID_AUTHENTICATION_KEY;
		}

		if( 0 != strcmp(pFieldIOCPSocket.GetPeerIP(), szUserIP) )
		{
			return 	XMLRPC_ERROR_INVALID_AUTHENTICATION_KEY;
		}
		*/
	}

	if( NULL == pFieldIOCPSocket )		// �Ʒ����� Ȱ������ ���� �������� ������ ȣ�� 
	{
		// ���� �о� ����

		QPARAM_GET_CHARACTERINFO_FROM_XMLRPC CharInfo;
		memset(&CharInfo, 0, sizeof(CharInfo));

		CharInfo.ReturnValue = -1;
		CharInfo.AccountUID = nAccountUID;
		CharInfo.CharacterUID = nCharacterUID;

		GetFieldIOCP()->m_pAtumDBManager->ExecuteQuery(QT_GetCharacterInfoFromXMLRPC, NULL, &CharInfo);

		if( 0 > CharInfo.ReturnValue )
		{
			return CharInfo.ReturnValue;
		}

		util::strncpy(TempFeildIOCPSocket.m_character.AccountName, CharInfo.AccountName, SIZE_MAX_ACCOUNT_NAME);
		util::strncpy(TempFeildIOCPSocket.m_character.CharacterName, CharInfo.CharacterName, SIZE_MAX_CHARACTER_NAME);
		TempFeildIOCPSocket.SetPeerAddress(szUserIP, 0);
		TempFeildIOCPSocket.m_character.Race = CharInfo.Race;

		pFieldIOCPSocket = &TempFeildIOCPSocket;
	}

	if( ERR_NO_ERROR != pFieldIOCPSocket->CashItemMoneyPay(&nTotalMoney, &nMCash, &nGiftCard) )
	{
		return XMLRPC_ERROR_BILLING_ERROR;
	}


	return nTotalMoney;
}

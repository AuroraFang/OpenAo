///////////////////////////////////////////////////////////////////////////////
//  VoIPProtocol.h
//			: ��밣 ���� ��ȭ���� ������ Ŭ���̾�Ʈ�� ����� ���� Param ��� ����
//			  VoIP Distribute Server, VoIP Call Server, VoIP Client���� ����
//
//  Date	: 2004-03-16 by cmkwon
///////////////////////////////////////////////////////////////////////////////

// 2016-07-19 panoskj disabled this
#if false // ndef _VOIP_PARAM_H_
#define _VOIP_PARAM_H_


typedef enum
{
	VOIP_SOCK_TYPE_UNKNOWN		= 0,
	VOIP_SOCK_TYPE_VDServer		= 1,
	VOIP_SOCK_TYPE_VCServer		= 2,
	VOIP_SOCK_TYPE_SCVoIP		= 3,
} VOIP_SOCK_TYPE_t;

///////////////////////////////////////////////////////////////////////////////
// 2008-09-30 by cmkwon, VoIP �ý��� ���� - VOIP_STATE_XXX ����
typedef enum
{
	VOIP_STATE_UNKNOWN						= 0,
	VOIP_STATE_VD_VCServer_Connected		= 1,
	VOIP_STATE_VD_VCServer_Registered		= 2,
	VOIP_STATE_VC_Connecting_TO_VDServer	= 11,
	VOIP_STATE_VC_Registering_TO_VDServer	= 12,
	VOIP_STATE_VC_Registered_TO_VDServer	= 13,
	VOIP_STATE_VC_Connected_SCVoIP			= 14,
	VOIP_STATE_VC_Joined_Room				= 15
} VOIP_STATE_t;

///////////////////////////////////////////////////////////////////////////////
// 2008-10-15 by cmkwon, ���ϱ� ��� ���� - TAKING_STATE_XXX ����
typedef enum
{
	TALKING_STATE_MUTE						= 0,		// ���� ����
	TALKING_STATE_START_TALKING				= 1,		// ���ϱ� ����
	TALKING_STATE_TALKING					= 1,		// ���ϴ� ����
	TALKING_STATE_END_TALKING				= 2			// ���ϱ� ����
} TALKING_STATE_t;


#if !defined(_ATUM_CLIENT)
	///////////////////////////////////////////////////////////////////////////////
	// User Define Macro
	///////////////////////////////////////////////////////////////////////////////
		#define	WM_C2D_ASYNC_EVENT					(WM_USER + 10)	// VCServer���� VDServer�� ����
		#define	WM_C2D_PACKET_NOTIFY				(WM_USER + 11)	// HWND�� ���޵Ǵ� �޽���
		#define	WM_S2D_ASYNC_EVENT					(WM_USER + 12)	// SCVoIP���� VDServer�� ����
		#define	WM_S2D_PACKET_NOTIFY				(WM_USER + 13)	// HWND�� ���޵Ǵ� �޽���
		#define	WM_S2C_ASYNC_EVENT					(WM_USER + 14)	// SCVoIP���� VCServer�� ����
		#define	WM_S2C_PACKET_NOTIFY				(WM_USER + 15)	// HWND�� ���޵Ǵ� �޽���



		#define SIZE_MAX_ROOM_NAME					20
		#define SIZE_MAX_USER_NAME					20
		#define SIZE_MAX_ROOM_COUNT					200				// �ڽ��� ������ �ִ� �ο���


		#define MAKEUINT64(low, high) (  ((UINT64)(low)) | ( ((UINT64)(high))<<32 )  )



		
	///////////////////////////////////////////////////////////////////////////////
	// User Define Type
	///////////////////////////////////////////////////////////////////////////////
		typedef UINT64		RoomID_t;				// Room ID
		typedef mt_vector<RoomID_t>		mtvectorRoomID;

		typedef USHORT ClientIndex_t;
		typedef USHORT	Error_t;

		typedef union 
		{
			DWORD			dwIP;
			BYTE			arrByIP[4];
		} Union_IP;
			
		class ServerID_t
		{
		public:
			ServerID_t()
			{
				unionIP.dwIP	= 0;
				usPort			= 0;
			}
			void ResetServerID_t(void)
			{
				unionIP.dwIP	= 0;
				usPort			= 0;
			}

		public:
			Union_IP		unionIP;
			USHORT			usPort;
		};

//		struct Delete_Object
//		{
//			template<typename T>
//				void operator()(T *ptr)
//			{
//				delete ptr;
//			}
//		};

		char *Inet_n2a(Union_IP i_unionIP);

#endif // END - #if !defined(_ATUM_CLIENT)

	#define SIZE_MAX_CHANNEL_COUNT				50				// �ڽ��� ������ �ִ� �ο���
	#define SIZE_MAX_TALKING_USER_COUNT			10				// 2008-10-15 by cmkwon, ���ϱ� ��� ���� - �ִ� ���� ���ϱ� ���� ������
	#define SIZE_NORMAL_TALKING_USER_COUNT		6				// 2008-10-15 by cmkwon, ���ϱ� ��� ���� - �⺻ ���� ���ϱ� ���� ������

	typedef enum
	{
		_SC2_ERROR_STATUS				= 0,
		_SC2_READY_STATUS				= 1,
		_SC2_CONNECTING_VDSERVER_STATUS	= 2,
		_SC2_CONNECTED_VDSERVER_STATUS	= 3,
		_SC2_CONNECTING_VCSERVER_STATUS	= 4,
		_SC2_CONNECTED_VCSERVER_STATUS	= 5,
		_SC2_CREATED_ROOM_STATUS		= 6,
		_SC2_JOINED_ROOM_STATUS			= 7
	} Status_SCDialer2;

	typedef enum
	{
		_SC2_NO_ERROR					= 0,
		_SC2_NORMAL_ERROR				= 1,
		_SC2_INVALID_STATUS_ERROR		= 2,
		_SC2_INVALID_PARAM_ERROR		= 3,
		_SC2_INVALID_ROOMID_ERROR		= 4,
		_SC2_CONNECT_VDSERVER_ERROR		= 5,
		_SC2_CONNECT_VCSERVER_ERROR		= 6,
		_SC2_VOIP_SERVER_ERROR			= 7
	} ErrorType_SCDialer2;

///////////////////////////////////////////////////////////////////////////////////
// SCDialer1.ocx	

// 2006-12-25 by cmkwon, ���� ���ڷ� �޾Ƽ� ó�� �ϵ��� �Ѵ�.
//	#define WINDOWTEXT_ATUM_SERVER				"SpaceCowboy_R"
//	#define WINDOWTEXT_ATUM_CLIENT				"SpaceCowboyVoIPClient"

#ifndef SIZE_MAX_CHARACTER_NAME
 	#define SIZE_MAX_CHARACTER_NAME				20
#endif

	typedef enum
	{
		_BothCall		= 0,
		_1to1Call		= 1,
		_NtoNCall		= 2	
	} AVCallType;

	typedef enum
	{	
		AV_EV_CLIENT_ERROR			= 0x00,
		AV_EV_CLIENT_STATUS			= 0x01,	
		AV_EV_EXECUTE_OK			= 0x02,
		AV_EV_NOTIFY				= 0x05,		// 2005-10-06 by cmkwon, No body, ���� �˸�
		AV_EV_REGISTERED			= 0x10,				// No body
		AV_EV_CREATE_ROOM_OK		= 0x20,				// No body
		AV_EV_JOIN_ROOM_OK			= 0x21,				// No body
		AV_EV_JOIN_USER				= 0x22,
		AV_EV_GOOUT_USER			= 0x23,
		AV_EV_TALK_USER				= 0x24,
		AV_EV_GET_SPK_VOLUME_OK		= 0x25,		// 2008-06-12 by cmkwon, SCVoIP.exe �� Volume ���� ��� �߰� - 
		AV_EV_CHANGED_TALKING_STATE	= 0x26,		// 2008-10-15 by cmkwon, ���ϱ� ��� ���� - 

		AV_MT_CLIENT_END			= 0x80,
		AV_MT_UNREGISTER			= 0x81,
		AV_MT_REGISTER				= 0x90,
		AV_MT_DIAL					= 0x91,
		AV_MT_PARTY_CREATE_ROOM		= 0xA0,
		AV_MT_GUILD_CREATE_ROOM		= 0xA1,
		AV_MT_PARTY_JOIN_ROOM		= 0xA2,
		AV_MT_GUILD_JOIN_ROOM		= 0xA3,
		AV_MT_TALK					= 0xA4,
		AV_MT_TALK_CANCEL			= 0xA5,
		AV_MT_GET_SPK_VOLUME		= 0xA6,		// 2008-06-12 by cmkwon, SCVoIP.exe �� Volume ���� ��� �߰� - 
		AV_MT_SET_SPK_VOLUME		= 0xA7		// 2008-06-12 by cmkwon, SCVoIP.exe �� Volume ���� ��� �߰� - 
	} AVMessageType;

	class AV_MSG_WM_COPYDATA
	{
	public:
		AV_MSG_WM_COPYDATA(AVCallType i_nCallType, AVMessageType i_nMsgType, USHORT i_nMsgParam)
			: nCallType(i_nCallType), nMsgType(i_nMsgType), nMsgParam(i_nMsgParam)
		{
		};
		AV_MSG_WM_COPYDATA(DWORD i_dwMsg)
		{
			memcpy(this, &i_dwMsg, sizeof(AV_MSG_WM_COPYDATA));
		};
		AV_MSG_WM_COPYDATA()
		{
			nCallType	= _1to1Call;
			nMsgType	= 0;
			nMsgParam	= 0;
		};
		operator DWORD()
		{
			return *(DWORD*)this;
		}

		BYTE	nMsgType;
		BYTE	nCallType;
		USHORT	nMsgParam;
	};
#endif	// _VOIP_PARAM_H_endif
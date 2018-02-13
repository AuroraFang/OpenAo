#pragma once

#include "CompileUtils.h"

#include <WinSock2.h>
#include <Mmsystem.h>

#pragma comment(lib, "Ws2_32.lib")

#ifdef SIZE_MAX_IPADDRESS
#undef SIZE_MAX_IPADDRESS
#endif
#ifdef SIZE_MAX_PACKET
#undef SIZE_MAX_PACKET
#endif

constexpr auto SIZE_MAX_IPADDRESS = 16;		// string IP Address
constexpr auto SIZE_TCP_LAN_HEADER = 58;	// 58 Bytes = 18(Datalink) + 20(IP) + 20(TCP)
constexpr auto SIZE_UDP_LAN_HEADER = 46;	// 46 Bytes = 18(Datalink) + 20(IP) + 8(UDP)

static_assert(sizeof(unsigned short) == 2, "The following code assumes unsigned short is 2 bytes long.");

// Packet Header ���� define

using MessageType_t = unsigned short;		// ��Ŷ Ÿ���� ����
using Err_t = unsigned short;				// ���� Ÿ���� ����

constexpr auto SIZE_PACKET_HEADER = 4;		// �� ��Ŷ�� ������ ��� ������
constexpr auto SIZE_BODY_LENGTH = 2;		// �� ��Ŷ�� ������ ��� ������
constexpr auto SIZE_ENCODE_FLAG = 1;		// ��ȣȭ ���� flag ������
constexpr auto SIZE_CHECKSUM = 1;			// checksum�� byte ��
constexpr auto SIZE_SEQ_NUM = 1;			// sequence number�� byte ��
constexpr auto SIZE_MAX_DUMMY_DATA = 3;		// dummy data�� �ִ� ����
constexpr auto SIZE_FIELD_TYPE_HEADER = sizeof(MessageType_t);		// �ϳ��� ��Ŷ�� �������� �ʵ�� �����ȴ�. �� �ʵ��� Ÿ�� ��� ������
constexpr auto SIZE_MAX_PACKET = 1492;		// ��Ŷ �ִ� ������(�������� �޼����� �ϳ��� ��Ŷ���� ���۵ɼ� ����)
constexpr auto SIZE_MAX_SOCKET_BUFFER = 1500;	// Receve Buffer �ִ� ������

constexpr auto COUNT_MAX_SOCKET_SESSION = 1600;			// �������� ������ �ִ� ���� Ŭ���̾�Ʈ��
constexpr auto SIZE_MAX_PRESERVER_SESSION = 1500;		// Pre Server���� ������ �ִ� ���� Ŭ���̾�Ʈ��, COUNT_MAX_SOCKET_SESSION ����
constexpr auto SIZE_MAX_FIELDSERVER_SESSION = 1500;		// Field Server���� ������ �ִ� ���� Ŭ���̾�Ʈ��, COUNT_MAX_SOCKET_SESSION ����
constexpr auto SIZE_MAX_IMSERVER_SESSION = 1500;		// IM Server���� ������ �ִ� ���� Ŭ���̾�Ʈ��, COUNT_MAX_SOCKET_SESSION ����
constexpr auto SIZE_MAX_AUTHSERVER_SESSION = 200;		// Authentication Server���� ������ �ִ� ���� Ŭ���̾�Ʈ��, COUNT_MAX_SOCKET_SESSION ���� // 2011-01-26 by hskim, ���� ���� ����
														// 2010-12-03 by jskim ���� UDP ä�μ� 170 -> 200���� ����

constexpr auto SIZE_MAX_FIELDWEBSERVER_SESSION = 400;	// 2013-03-13 by hskim, �� ĳ�� ���� - Field Web Server���� ������ �ִ� ���� Ŭ���̾�Ʈ��, COUNT_MAX_SOCKET_SESSION ����

constexpr auto SIZE_MAX_UDP_CHANNEL_SESSION = 200;									// 2008-08-29 by cmkwon, FieldServ<->NPCServ �� UDP ��� �� ä�μ� - ���񽺸ʺ��� ���ƾ� �Ѵ�.													
constexpr auto SIZE_MAX_NPCSERVER_SESSION = SIZE_MAX_UDP_CHANNEL_SESSION + 10;		// NPC Server���� ������ �ִ���� ��, COUNT_MAX_SOCKET_SESSION ����
constexpr auto SIZE_MAX_LOGSERVER_SESSION = 100;									// Log �������� ������ �ִ� ���� ������, COUNT_MAX_SOCKET_SESSION ����

template<typename _MSG_STRUCT> constexpr auto _msg_size() { return SIZE_FIELD_TYPE_HEADER + sizeof(_MSG_STRUCT); }

template<MessageType_t _MSG_TYPE, typename buffer_t> void _set_msg_type(buffer_t* _BUFFER)
{
	*(MessageType_t*)(_BUFFER) = _MSG_TYPE;
}

template<typename _MSG_STRUCT, MessageType_t _MSG_TYPE, typename buffer_t> auto _init_msg(buffer_t* _BUFFER)
{
	static_assert(sizeof(buffer_t) == 1, "_BUFFER must be a pointer to a single byte.");

	memset(_BUFFER, 0x00, _msg_size<_MSG_STRUCT>());
	if (_MSG_TYPE != 0) _set_msg_type<_MSG_TYPE>(_BUFFER);
	return (_MSG_STRUCT*)(_BUFFER + SIZE_FIELD_TYPE_HEADER /*sizeof(MessageType_t)*/);
}

#define MSG_SIZE(_MSG_STRUCT) _msg_size<_MSG_STRUCT>()
#define SET_MSG_TYPE(_BUFFER, _MSG_TYPE) _set_msg_type<_MSG_TYPE>(_BUFFER);

///////////////////////////////////////////////////////////////////////////////
// Declare Variable, Set Message Buffer & Type
//		_MSG_STRUCT	: �޼��� ����ü Ÿ��
//		_VAR_NAME	: ���� �̸�
//		_BUFFER		: ����
//		_MSG_TYPE	: �޼��� Ÿ��
///////////////////////////////////////////////////////////////////////////////
#define INIT_MSG(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)				\
	auto _VAR_NAME = _init_msg<_MSG_STRUCT, _MSG_TYPE>(_BUFFER);

#define INIT_MSG_WITH_BUFFER(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)	\
	BYTE _BUFFER[SIZE_MAX_PACKET];											\
	auto _VAR_NAME = _init_msg<_MSG_STRUCT, _MSG_TYPE>(_BUFFER);

#define INIT_MSG_OF_SIZE(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)		\
	BYTE _BUFFER[MSG_SIZE(_MSG_STRUCT)];									\
	auto _VAR_NAME = _init_msg<_MSG_STRUCT, _MSG_TYPE>(_BUFFER);

///////////////////////////////////////////////////////////////////////////////
// Encode/Decode Protocol
/*/////////////////////////////////////////////////////////////////////////////

<------------------- 4B --------------->
-----------------------------------------------------------------------
| size | encode_flag +   N    | seq. # | MSGs ... | checksum |  dummy  |
| (2B) |   (1bit)    + (7bit) |  (1B)  |          |   (1B)   |  (0~3B) |
-----------------------------------------------------------------------
<- XOR -><------- XOR --------->

* encoding rule 1
- Sequence Number ����
(1)
ù SeqNumber := Random �Լ��� ������ Number (0~127)

(2)
NewSeqNumber := ( SeqNumber + A ) * B
if ( NewSeqNumber > C ) {
NextSeqNumber := NewSeqNumber % C
} else {
NextSeqNumber := SeqNumber
}

* encoding rule 2
- Dummy Data ���� �� packet �ںκп� �߰�
Dummy Data�� ���̴� 0~3B = (seq. #) % 4, ������ ���Ƿ� ����

* encoding rule 3
- checksum ���� �� �߰�

* encoding rule 4
- XOR_DATA[128]�� �̸� �����ص�.
- (seq. #)�� XOR_DATA[N-1]�� xor ������ ��
- (MSGs + checksum)�� XOR_DATA[N]���� XOR_DATA[127]������ xor ������ ��
XOR_DATA[127] �������� XOR_DATA[0]���� �ٽ� ������
- N ���� ȿ������ ���� �׻� 4�� ���, N���� ������ 4, 8, ... , 124

* invalid packet�� ���ǵ�
- sequence number�� ���� ����
- checksum�� ���� ����
- dummy data�� ���̰� ���� ������, checksum�� Ʋ���ų�, MSG�� ���� �ʰ� ��
- XOR_DATA�� ������ �� ���� MSGs�� protocol�� ���� ����

/////////////////////////////////////////////////////////////////////////////*/

constexpr auto ENCODE_MASK = 0x80;	// 10000000
constexpr auto XOR_N_MASK = 0x7F;	// 01111111

constexpr auto SEQNO_VAR_A = 1;
constexpr auto SEQNO_VAR_B = 2;
constexpr auto SEQNO_VAR_C = 119;	// MAX Sequence Number

constexpr char XOR_ENCODE_BYTES[] = "fewoiroqbfweotui29854f09qwe0213hrf0a89wq0re902149dujaosdjfapwetu2fadq1234fsacdfzdxczfsdgbhtrytrgw563fwsjkpqertgvxhteertw3512ga\0";
constexpr auto SIZE_XOR_ENCODE_BYTES = carrlen(XOR_ENCODE_BYTES);

// sequence number�� connection�� �ϳ��� �����ؾ� �ϱ� ������, IOCPSocket���� �����Ѵ�.
struct DECODING_INFO
{
	BYTE nSeqNumber;
	BYTE nXOR_N;
	BOOL bIsPacketEncoded;	// default: FALSE
	BOOL bIsVaildPacket;		// default: FALSE
};

enum ENServerType : DWORD
{
	ST_ALL = -1,			// 2005-12-27 by cmkwon
	ST_NORMAL_SERVER = 0,
	ST_PRE_SERVER = 1,
	ST_IM_SERVER = 2,
	ST_LOG_SERVER = 3,
	ST_MONITOR_SERVER = 4,
	ST_FIELD_SERVER = 5,
	ST_NPC_SERVER = 6,
	ST_CLIENT_TYPE = 7,
	ST_ADMIN_TOOL = 8,
	ST_INVALID_TYPE = 9,
	//ST_AUTHENTICATION_SERVER = 10,		// 2011-01-26 by hskim, ���� ���� ����
	//ST_FIELD_WEB_SERVER = 11				// 2013-03-13 by hskim, �� ĳ�� ����
};

enum ProcessResult
{
	RES_BREAK = 0,	// ����� ������. ������ ���� �ʴ´�.
	RES_PACKET_ERROR = 1,	// ��Ŷ�� �ջ�� ���. ���� packet�� ó������ �ʰ� �ٷ� �����Ѵ�. ������ ���ῡ�� ���.
	RES_RETURN_TRUE = 2,	// ����
	RES_RETURN_FALSE = 3,		// ������ ������ϴ� ���. �������� ���� ��
	RES_PACKET_NA = 4,
	RES_ACCESS_DENIED = 5
};

// ����׿� - For PrintExchangeMsg()
enum
{
	RECV_TYPE = 0,
	SEND_TYPE = 1
};

enum : BYTE
{
	PRINTLEVEL_ALL = 0,
	PRINTLEVEL_WO_ALIVE = 1,
	PRINTLEVEL_WO_MOVE_BATTLE = 2,
	PRINTLEVEL_A_FEW = 3,
	PRINTLEVEL_FEW = 4,
	PRINTLEVEL_NO_MSG = 5
};

// Socket ���� Global Variable
extern BYTE g_exchangeMsgPrintLevel;	// check: MSG�� print�ϴ� level�� �����ϱ� ����(AtumMonitor�� ���� ����), by kelovon

// Socket ���� Global Function
const char *GGetENServerTypeString(ENServerType st);

void GSetexchangeMsgPrintLevel(BYTE i_byPrintLevel);
BYTE GGetexchangeMsgPrintLevel();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
char* GGetIPByName(char *i_strHostName, char *o_strIPAddr);

///////////////////////////////////////////////////////////////////////////////
// 2006-04-11 by cmkwon

enum
{
	IP_TYPE_AUTOPRIVATE = 1,
	IP_TYPE_PUBLIC = 2,
	IP_TYPE_PRIVATE = 3
};

BOOL GGetLocalIP(char *o_szLocalIP, int nLocalIPType = IP_TYPE_AUTOPRIVATE);
BOOL _GGetLocalIP(BYTE *ip0, BYTE *ip1, BYTE *ip2, BYTE *ip3, int nLocalIPType);
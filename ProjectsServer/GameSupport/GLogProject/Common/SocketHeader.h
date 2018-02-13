///////////////////////////////////////////////////////////////////////////////
//  SocketHeader.h : ���� ���� ����� CIOCP�� CWinPacket���� include�Ѵ�
//
//  Date	: 2004-03-17 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#ifndef _SOCKET_HEADER_H_
#define _SOCKET_HEADER_H_

#include <winsock2.h>
#include <Mmsystem.h>

#pragma comment(lib, "Ws2_32.lib")

///////////////////////////////////////////////////////////////////////////////
// Socket ���� define
#define SIZE_MAX_IPADDRESS					16		// string IP Address
#define SIZE_TCP_LAN_HEADER					58		// 58 Bytes = 18(Datalink) + 20(IP) + 20(TCP)
#define SIZE_UDP_LAN_HEADER					46		// 46 Bytes = 18(Datalink) + 20(IP) + 8(UDP)

///////////////////////////////////////////////////////////////////////////////
// Packet Header ���� define
typedef USHORT MessageType_t;						// ��Ŷ Ÿ���� ����
typedef USHORT Err_t;								// ���� Ÿ���� ����
#define SIZE_PACKET_HEADER					4		// �� ��Ŷ�� ������ ��� ������
#define SIZE_BODY_LENGTH					2		// �� ��Ŷ�� ������ ��� ������
#define SIZE_ENCODE_FLAG					1		// ��ȣȭ ���� flag ������
#define SIZE_CHECKSUM						1		// checksum�� byte ��
#define SIZE_SEQ_NUM						1		// sequence number�� byte ��
#define SIZE_MAX_DUMMY_DATA					3		// dummy data�� �ִ� ����
#define SIZE_FIELD_TYPE_HEADER				2		// �ϳ��� ��Ŷ�� �������� �ʵ�� �����ȴ�. �� �ʵ��� Ÿ�� ��� ������
#define SIZE_MAX_PACKET						1492	// ��Ŷ �ִ� ������(�������� �޼����� �ϳ��� ��Ŷ���� ���۵ɼ� ����)
#define SIZE_MAX_SOCKET_BUFFER				1500	// Receve Buffer �ִ� ������

#define COUNT_MAX_SOCKET_SESSION			1600	// �������� ������ �ִ� ���� Ŭ���̾�Ʈ��
#define SIZE_MAX_PRESERVER_SESSION			1500	// Pre Server���� ������ �ִ� ���� Ŭ���̾�Ʈ��, COUNT_MAX_SOCKET_SESSION ����
#define SIZE_MAX_FIELDSERVER_SESSION		1500	// Field Server���� ������ �ִ� ���� Ŭ���̾�Ʈ��, COUNT_MAX_SOCKET_SESSION ����
#define SIZE_MAX_IMSERVER_SESSION			1500	// IM Server���� ������ �ִ� ���� Ŭ���̾�Ʈ��, COUNT_MAX_SOCKET_SESSION ����
#define SIZE_MAX_UDP_CHANNEL_SESSION		170		// 2008-08-29 by cmkwon, FieldServ<->NPCServ �� UDP ��� �� ä�μ� - ���񽺸ʺ��� ���ƾ� �Ѵ�.
#define SIZE_MAX_NPCSERVER_SESSION			(SIZE_MAX_UDP_CHANNEL_SESSION+10)		// NPC Server���� ������ �ִ���� ��, COUNT_MAX_SOCKET_SESSION ����
#define SIZE_MAX_LOGSERVER_SESSION			100		// Log �������� ������ �ִ� ���� ������, COUNT_MAX_SOCKET_SESSION ����

///////////////////////////////////////////////////////////////////////////////
// Declare Variable, Set Message Buffer & Type
//		_MSG_STRUCT	: �޼��� ����ü Ÿ��
//		_VAR_NAME	: ���� �̸�
//		_BUFFER		: ����
//		_MSG_TYPE	: �޼��� Ÿ��
///////////////////////////////////////////////////////////////////////////////
#define MSG_SIZE(_MSG_STRUCT)				(SIZE_FIELD_TYPE_HEADER + sizeof(_MSG_STRUCT))
#define INIT_MSG(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)				\
	memset(_BUFFER, 0x00, MSG_SIZE(_MSG_STRUCT));							\
	_MSG_STRUCT *_VAR_NAME = (_MSG_STRUCT*)((_BUFFER)+SIZE_FIELD_TYPE_HEADER); \
	*(MessageType_t*)(_BUFFER) = _MSG_TYPE;
#define INIT_MSG_WITH_BUFFER(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)	 \
	BYTE _BUFFER[SIZE_MAX_PACKET];											 \
	memset(_BUFFER, 0x00, SIZE_MAX_PACKET);							\
	_MSG_STRUCT *_VAR_NAME = (_MSG_STRUCT*)((_BUFFER)+SIZE_FIELD_TYPE_HEADER); \
	*(MessageType_t*)(_BUFFER) = _MSG_TYPE;
#define INIT_MSG_OF_SIZE(_MSG_STRUCT, _MSG_TYPE, _VAR_NAME, _BUFFER)	 \
	BYTE _BUFFER[MSG_SIZE(_MSG_STRUCT)];											 \
	memset(_BUFFER, 0x00, MSG_SIZE(_MSG_STRUCT));							\
	_MSG_STRUCT *_VAR_NAME = (_MSG_STRUCT*)((_BUFFER)+SIZE_FIELD_TYPE_HEADER); \
	*(MessageType_t*)(_BUFFER) = _MSG_TYPE;
#define SET_MSG_TYPE(_BUFFER, _MSG_TYPE)	*(MessageType_t*)(_BUFFER) = _MSG_TYPE;

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

#define ENCODE_MASK		0x80		// 10000000
#define XOR_N_MASK		0x7F		// 01111111

#define SEQNO_VAR_A		1
#define SEQNO_VAR_B		2
#define SEQNO_VAR_C		119			// MAX Sequence Number

#define SIZE_XOR_ENCODE_BYTES	128
#define XOR_ENCODE_BYTES "fewoiroqbfweotui29854f09qwe0213hrf0a89wq0re902149dujaosdjfapwetu2fadq1234fsacdfzdxczfsdgbhtrytrgw563fwsjkpqertgvxhteertw3512ga"

// sequence number�� connection�� �ϳ��� �����ؾ� �ϱ� ������, IOCPSocket���� �����Ѵ�.
typedef struct
{
	BYTE	nSeqNumber;
	BYTE	nXOR_N;
	BOOL	bIsPacketEncoded;	// default: FALSE
	BOOL	bIsVaildPacket;		// default: FALSE
}  DECODING_INFO, *PDECODING_INFO;

#ifndef	RESET_DECOING_INFO
#define RESET_DECOING_INFO(x) { memset(&x, 0x00, sizeof(DECODING_INFO)); }
#endif

typedef enum
{
	ST_ALL				= -1,			// 2005-12-27 by cmkwon
	ST_NORMAL_SERVER	= 0,
	ST_PRE_SERVER		= 1,
	ST_IM_SERVER		= 2,
	ST_LOG_SERVER		= 3,
	ST_MONITOR_SERVER	= 4,
	ST_FIELD_SERVER		= 5,
	ST_NPC_SERVER		= 6,
	ST_CLIENT_TYPE		= 7,
	ST_ADMIN_TOOL		= 8,
	ST_INVALID_TYPE		= 9
} ENServerType;

typedef enum
{
	RES_BREAK			= 0,	// ����� ������. ������ ���� �ʴ´�.
	RES_PACKET_ERROR	= 1,	// ��Ŷ�� �ջ�� ���. ���� packet�� ó������ �ʰ� �ٷ� �����Ѵ�. ������ ���ῡ�� ���.
	RES_RETURN_TRUE		= 2,	// ����
	RES_RETURN_FALSE	= 3		// ������ ������ϴ� ���. �������� ���� ��
} ProcessResult;

// ����׿� - For PrintExchangeMsg()
#define RECV_TYPE 0
#define SEND_TYPE 1

#define PRINTLEVEL_ALL				0
#define PRINTLEVEL_WO_ALIVE			1
#define PRINTLEVEL_WO_MOVE_BATTLE	2
#define PRINTLEVEL_A_FEW			3
#define PRINTLEVEL_FEW				4
#define PRINTLEVEL_NO_MSG			5

//////////////////////////////////////////////////////////////////////////
// Socket ���� Global Variable
//////////////////////////////////////////////////////////////////////////
extern BYTE			g_exchangeMsgPrintLevel;	// check: MSG�� print�ϴ� level�� �����ϱ� ����(AtumMonitor�� ���� ����), by kelovon


//////////////////////////////////////////////////////////////////////////
// Socket ���� Global Function
//////////////////////////////////////////////////////////////////////////
const char *GGetENServerTypeString(ENServerType st);

void GSetexchangeMsgPrintLevel(BYTE i_byPrintLevel);
BYTE GGetexchangeMsgPrintLevel(void);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
char* GGetIPByName(char *i_strHostName, char *o_strIPAddr);

///////////////////////////////////////////////////////////////////////////////
// 2006-04-11 by cmkwon
#define IP_TYPE_AUTOPRIVATE        1
#define IP_TYPE_PUBLIC             2
#define IP_TYPE_PRIVATE            3
BOOL GGetLocalIP(char *o_szLocalIP, int nLocalIPType=IP_TYPE_AUTOPRIVATE);
BOOL _GGetLocalIP(BYTE *ip0, BYTE *ip1, BYTE *ip2, BYTE *ip3, int nLocalIPType);

#endif	// _SOCKET_HEADER_H_endif
// WinPacket.h: interface for the Packet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKET_H__3AC33C9F_DE2E_43A4_8057_0DF04475F92F__INCLUDED_)
#define AFX_PACKET_H__3AC33C9F_DE2E_43A4_8057_0DF04475F92F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SocketHeader.h"

class CSendPacket
{
public:
	// Construction
	CSendPacket(char* pBlock = NULL, int nLength = 0, BOOL EncodingFlag = FALSE, BYTE seq = 0)
	{
		m_nPacketLength	= 0;
		m_bEncodingFlag = FALSE;
		memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);

		if(pBlock
			&& SetPacket(pBlock, nLength, EncodingFlag, seq) == FALSE)
		{
			m_nPacketLength	= 0;
			m_bEncodingFlag = FALSE;
			memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);
		}
	}
	~CSendPacket(){};

public:	
	inline void Init();
	inline BOOL SetPacket(char *pBlock, int nLength, BOOL EncodingFlag = FALSE, BYTE seq = 0);
	inline int GetLength(void);
	inline char* GetPacket(void);	
	inline BOOL IsPacketEncoded(void);

private:
	inline void EncodePacket(char *pBlock, int nLength, BYTE seq);

private:
	// Attributes
	char			m_pPacket[SIZE_MAX_SOCKET_BUFFER];
	int				m_nPacketLength;
	BOOL			m_bEncodingFlag;						// is packet encoded
	static char*	m_pXOR_ENCODE_BYTES;
};

class CRecvPacket
{
public:
	// Construction
	CRecvPacket();
	~CRecvPacket();

public:
	inline void Init(void);	
	inline int AddTail(char* pBlock, int nLength, int* UsedBytes);

	inline int GetLength(void);
	inline char* GetPacket(void);	
	inline BOOL RecvCompleted();
	inline BOOL IsPacketEncoded();
	inline BOOL IsValidPacket();
	inline BYTE GetSequenceNumber();

protected:
	inline void DecodePacket();

protected:
	// Attributes
	char 	 	 	m_pPacket[SIZE_MAX_SOCKET_BUFFER];
	char			m_pPacketHeader[SIZE_PACKET_HEADER];
	int	            m_nPacketLength;
	int				m_nCurrentLength;
	int				m_nRemainedHeaderSize;

	// decoding
	DECODING_INFO	m_DecodingInfo;
	static char*	m_pXOR_ENCODE_BYTES;
};

///////////////////////////////////////////////////////////////////////////////
// CSendPacket's Inline functions
///////////////////////////////////////////////////////////////////////////////
void CSendPacket::Init()
{
	m_nPacketLength = 0;
	m_bEncodingFlag = FALSE;
	memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);
}

int CSendPacket::SetPacket(char *pBlock, int nLength, BOOL EncodingFlag, BYTE seq)
{
	if(NULL == pBlock 
		|| 0 >= nLength 
		|| SIZE_MAX_PACKET < nLength)
	{
		return FALSE;
	}

	m_bEncodingFlag = EncodingFlag;
	if (EncodingFlag == TRUE)
	{
		int nDummyLen = seq % 4;
		EncodePacket(pBlock, nLength, seq);
	}
	else
	{
		*(unsigned short*)m_pPacket = nLength;							// packet body length
		*(BYTE*)(m_pPacket+SIZE_BODY_LENGTH) = 0x00;					// encode flag
		*(BYTE*)(m_pPacket+SIZE_BODY_LENGTH+SIZE_ENCODE_FLAG) = 0x00;	// seq. #
		memcpy(m_pPacket + SIZE_PACKET_HEADER, (const void*)pBlock, nLength);

		m_nPacketLength = nLength + SIZE_PACKET_HEADER;					// ��ü ���̴� body�� ��� �κ� ���� ����
	}

	return TRUE;
}

int CSendPacket::GetLength(void)
{
	return m_nPacketLength;
}

char* CSendPacket::GetPacket(void)
{
	return m_pPacket;
}

void CSendPacket::EncodePacket(char *pBlock, int nLength, BYTE seq)
{
	// XOR_N ����, 4, 8, 12, ... , 124
	BYTE XOR_N = (BYTE)((GetTickCount() % 30 + 1) * 4);

	// ������ ����, ����
	BYTE checkSum = 0;
	int nDummyLen = seq%4;
	int woffset = SIZE_BODY_LENGTH;
	int roffset = 0;
	int xoffset = XOR_N;

	// size�� ����
	*(unsigned short*)m_pPacket
		= (unsigned short)(nLength + SIZE_CHECKSUM + nDummyLen);

	// encode flag 1B�� �����ϰ� ����
	BYTE encodeFlag = ENCODE_MASK | (XOR_N_MASK & XOR_N);
	*(BYTE*)(m_pPacket+woffset) = encodeFlag;
	woffset += SIZE_ENCODE_FLAG;

	// sequence number(1B)�� XOR����, ����, checksum ��� ����
	checkSum ^= seq;
	*(BYTE*)(m_pPacket+woffset) = (seq ^ m_pXOR_ENCODE_BYTES[XOR_N-1]);
	woffset += SIZE_SEQ_NUM;

	// MSGs�� 4B�� XOR, checksum ���
	int nEndValue = (int)(nLength / 4) * 4;
	DWORD checkSum_4B = 0;
	while (roffset < nEndValue)
	{
		checkSum_4B ^= *(DWORD*)(pBlock+roffset);
		*(DWORD*)(m_pPacket+woffset)
			= *(DWORD*)(pBlock+roffset) ^ *(DWORD*)(m_pXOR_ENCODE_BYTES+xoffset);
		woffset += 4;
		roffset += 4;
		xoffset = (xoffset + 4) % SIZE_XOR_ENCODE_BYTES;
	}

	// 4B�� checkSum_4B�� 1B�� checkSum���� ��ȯ
	checkSum ^= ( *((char*)&checkSum_4B)
				^ *((char*)&checkSum_4B+1)
				^ *((char*)&checkSum_4B+2)
				^ *((char*)&checkSum_4B+3) );

	// �����ִ� nLength - nEndValue ��ŭ�� �����͸� ó�� -> XOR + checkSum
	for (int i = 0; i < nLength - nEndValue; i++)
	{
		checkSum ^= pBlock[roffset];
		m_pPacket[woffset]
			= pBlock[roffset] ^ m_pXOR_ENCODE_BYTES[xoffset];
		woffset++;
		roffset++;
		xoffset = (xoffset + 1) % SIZE_XOR_ENCODE_BYTES;
	}

	// checksum�� �߰�
	m_pPacket[woffset++] =  checkSum ^ m_pXOR_ENCODE_BYTES[xoffset];

	// dummy data�� XOR_ENCODE_BYTES�� �����
	char *dummyData = m_pXOR_ENCODE_BYTES + XOR_N;

	memcpy(m_pPacket+woffset, dummyData, nDummyLen);
	woffset += nDummyLen;

	m_nPacketLength = woffset;

}

BOOL CSendPacket::IsPacketEncoded(void)
{
	return m_bEncodingFlag;
}


///////////////////////////////////////////////////////////////////////////////
// CRecvPacket's Inline functions
///////////////////////////////////////////////////////////////////////////////
void CRecvPacket::Init(void)
{
	memset(m_pPacket, 0x00, SIZE_MAX_SOCKET_BUFFER);
	memset(m_pPacketHeader, 0x00, SIZE_PACKET_HEADER);
	m_nPacketLength			= 0;
	m_nCurrentLength		= 0;
	m_nRemainedHeaderSize	= 0;
	RESET_DECOING_INFO(m_DecodingInfo);
}


////////////////////////////////////////////////////////////////////////////////
//
// �� �� �� ��  : CRecvPacket::AddTail
// ��ȯ�Ǵ� ��  : int
// �� �� �� ��  : char *pBlock
// �� �� �� ��  : int nLength
// �� �� �� ��  : int* UsedBytes
// �� �� �� ��  : inline �Լ�
//					Socket���� ���� �����͸� Packet���� �ϼ��ϴ� �Լ�
//					packet�� ��ȣȭ�Ǿ� �ִٸ�, �ʿ�� �ϴ� �����͸� �� ���� �Ŀ�
//					AddTail()���� �ڵ����� Decoding�� ������
//					0 ���� ���� ���� �����Ҷ��� ������ �߻��������̴�.(���ϰ��� 0�϶� ���������� ó����)
int CRecvPacket::AddTail(char *pBlock, int nLength, int* UsedBytes)
{
	*UsedBytes = 0;
	if(NULL == pBlock 
		|| 0 >= nLength 
		|| RecvCompleted())
	{	// �Ķ���Ͱ� ��ȿ���� �ʰ� �̹� ��Ŷ�� �ϼ��Ǿ������� FALSE�� ����
		
		return -1;
	}

	if(m_nRemainedHeaderSize > 0)
	{	// �޾ƾ��� Header Size�� �ִٸ� ���� Header�� �ϼ��Ѵ�.

		if(m_nRemainedHeaderSize > SIZE_PACKET_HEADER){		return -2;}	// �޾ƾ��� Header Size�� ��ȿ���� ����
		if(m_nRemainedHeaderSize <= nLength)		
		{	// ���� �����Ͱ� ���� ��Ŷ ��� ������ ���� ũ��

			memcpy(m_pPacketHeader + SIZE_PACKET_HEADER - m_nRemainedHeaderSize, pBlock, m_nRemainedHeaderSize);
			pBlock += m_nRemainedHeaderSize;
			nLength -= m_nRemainedHeaderSize;
			*UsedBytes += m_nRemainedHeaderSize;
			m_nRemainedHeaderSize = 0;
			
			m_nPacketLength = *(unsigned short*)m_pPacketHeader;		// body length�� ����
			if(SIZE_MAX_PACKET + SIZE_MAX_DUMMY_DATA + SIZE_CHECKSUM < m_nPacketLength)
			{	// Packet length�� ��ȿ���� üũ

				return -3;
			}
		}
		else
		{
			memcpy(m_pPacketHeader + SIZE_PACKET_HEADER - m_nRemainedHeaderSize, pBlock, nLength);
			*UsedBytes += nLength;
			m_nRemainedHeaderSize -= nLength;
			return m_nCurrentLength;
		}
	}
	else if(0 == m_nPacketLength)
	{
		if(SIZE_PACKET_HEADER <= nLength)		
		{	// ���� �����Ͱ� ��Ŷ ��� ������ ���� ũ��
			
			memcpy(m_pPacketHeader, pBlock, SIZE_PACKET_HEADER);		// header���� packet size�� ������.
			pBlock += SIZE_PACKET_HEADER;
			nLength -= SIZE_PACKET_HEADER;
			*UsedBytes += SIZE_PACKET_HEADER;
			
			m_nPacketLength = *(unsigned short*)m_pPacketHeader;		// body length�� ����
			if(SIZE_MAX_PACKET + SIZE_MAX_DUMMY_DATA + SIZE_CHECKSUM < m_nPacketLength)
			{	// Packet length�� ��ȿ���� üũ

				return -4;
			}
		}
		else
		{
			memcpy(m_pPacketHeader, pBlock, nLength);
			*UsedBytes += nLength;
			m_nRemainedHeaderSize  = SIZE_PACKET_HEADER - nLength;
			return m_nCurrentLength;
		}
	}

	if(m_nPacketLength - m_nCurrentLength < nLength)	
	{	// �Էµ� ����Ÿ�� �ϼ��ؾ��� ��Ŷ���� Ŭ���, �ϼ��ؾ��� ����Ÿ ũ�⸸ŭ�� �����Ѵ�.
		
		nLength = m_nPacketLength - m_nCurrentLength;
	}

	if(nLength > 0)
	{
		memcpy(m_pPacket+m_nCurrentLength, pBlock, nLength);
		m_nCurrentLength += nLength;
		*UsedBytes += nLength;
	}
	
	if (m_nCurrentLength == m_nPacketLength)
	{	// �ϼ��ؾ� �� ��ŭ�� ����Ÿ�� �� �Էµ�

		m_DecodingInfo.bIsVaildPacket = TRUE;
		BYTE encode_flag = *((BYTE*)m_pPacketHeader+SIZE_BODY_LENGTH);
		m_DecodingInfo.bIsPacketEncoded = ((encode_flag&ENCODE_MASK)==ENCODE_MASK)?TRUE:FALSE;

		// ��ȣȭ ��Ŷ�̸� decode ����
		if (m_DecodingInfo.bIsPacketEncoded)
		{
			m_DecodingInfo.nXOR_N = (encode_flag&XOR_N_MASK);
			DecodePacket();
		}
		else
		{
			m_DecodingInfo.nSeqNumber = *((BYTE*)m_pPacketHeader + SIZE_BODY_LENGTH + SIZE_ENCODE_FLAG);
		}
	}

	return	m_nCurrentLength;
}

BOOL CRecvPacket::RecvCompleted()
{
	return (m_nPacketLength && m_nCurrentLength == m_nPacketLength);
}

void CRecvPacket::DecodePacket()
{
	char checkSum = 0;
	int woffset	= 0;			// for WRITING decoded data
	int roffset	= 0;			// for READING received data
	int xoffset = m_DecodingInfo.nXOR_N;

	// packet�� ���Ե� seq. number(1B) ������
	m_DecodingInfo.nSeqNumber
		= m_pPacketHeader[SIZE_BODY_LENGTH+SIZE_ENCODE_FLAG] ^ m_pXOR_ENCODE_BYTES[xoffset-1];
	checkSum ^= m_DecodingInfo.nSeqNumber;

	// dummy data�� length
	int nDummyLen = m_DecodingInfo.nSeqNumber % 4;

	// XOR ���� ó��, locality ���� checksum �Բ� ���
	// ���� ��� ���� 4B �� �����
	DWORD checkSum_4B = 0;
	int	nEndValue = (int)((m_nPacketLength - nDummyLen - SIZE_CHECKSUM)/4)*4;
	while(roffset < nEndValue)
	{
		// XOR ó��
		*(DWORD*)(m_pPacket+woffset)
			= *(DWORD*)(m_pPacket+roffset) ^ *(DWORD*)(m_pXOR_ENCODE_BYTES+xoffset);
		// checksum ���
		checkSum_4B ^= *(DWORD*)(m_pPacket+woffset);

		woffset += 4;
		roffset += 4;
		xoffset = (xoffset + 4) % SIZE_XOR_ENCODE_BYTES;
	}
	// checksum ��ġ��
	checkSum ^= ( *((char*)&checkSum_4B)
				^ *((char*)&checkSum_4B+1)
				^ *((char*)&checkSum_4B+2)
				^ *((char*)&checkSum_4B+3) );

	// 4����Ʈ�� ó���ϰ� ���� ����Ʈ�� ó��
	int nRepeat = (m_nPacketLength - nDummyLen - SIZE_CHECKSUM) - nEndValue;
	for ( int i = 0; i < nRepeat; i++)
	{
		m_pPacket[woffset]
			= m_pPacket[roffset] ^ m_pXOR_ENCODE_BYTES[xoffset];
		checkSum ^= m_pPacket[woffset];

		woffset += 1;
		roffset += 1;
		xoffset = (xoffset + 1) % SIZE_XOR_ENCODE_BYTES;
	}

	// packet�� ���Ե� checksum ������
	char packetCheckSum
		= m_pPacket[roffset] ^ m_pXOR_ENCODE_BYTES[xoffset];

	// checksum Ȯ��
	if ( checkSum != packetCheckSum )
	{
		// error, invalid checksum
		m_DecodingInfo.bIsVaildPacket = FALSE;
		return;
	}

	m_nPacketLength = m_nCurrentLength = woffset;
}

int CRecvPacket::GetLength(void)
{
	return m_nPacketLength;
}

char* CRecvPacket::GetPacket(void)
{
	return m_pPacket;
}

BOOL CRecvPacket::IsPacketEncoded()
{
	return m_DecodingInfo.bIsPacketEncoded;
}

BOOL CRecvPacket::IsValidPacket()
{
	return m_DecodingInfo.bIsVaildPacket;
}

BYTE CRecvPacket::GetSequenceNumber()
{
	return m_DecodingInfo.nSeqNumber;
}
#endif // !defined(AFX_PACKET_H__3AC33C9F_DE2E_43A4_8057_0DF04475F92F__INCLUDED_)

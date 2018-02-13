#pragma once

#include "SocketHeader.h"

class COverlapped;

struct SSendedOverlappedInfo	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 
{
	COverlapped*	pOverlapped;
	BYTE*			pbyData;
	int				nSendedSize;
};

using vectSSendedOverlappedInfo = vector<SSendedOverlappedInfo>;	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - 

class COverlapped : public OVERLAPPED
{
public:
	enum ENOperationMode
	{
		EN_OPERATION_MODE_READ = 0,
		EN_OPERATION_MODE_WRITE = 1
	};
	enum ENOverlappedType
	{
		EN_OVERLAPPED_TYPE_STATIC = 2,
		EN_OVERLAPPED_TYPE_DYNAMIC = 3
	};
	enum ENDataType
	{
		EN_DATA_TYPE_ACEONLINE = 0,
		EN_DATA_TYPE_RAW = 1
	};

	COverlapped(ENOperationMode i_enOperationMode, ENOverlappedType i_enOverlappedType,
		bool Encoding = false, ENDataType i_enDataType = EN_DATA_TYPE_ACEONLINE);		// 2013-03-13 by hskim, �� ĳ�� ���� - RawData ���� ��� �߰�

	///////////////////////////////////////////////////////////////////////////
	// Property
	BYTE GetOperationMode() const { return m_enOperationMode; }
	BYTE GetOverlappedType() const { return m_enOverlappedType; }
	BYTE GetDataType() const { return m_enDataType; }		// 2013-03-13 by hskim, �� ĳ�� ���� - RawData ���� ��� �߰�
	WSABUF* GetWSABUFPtr() { return &m_WSABUF; }
	sockaddr_in* Getsocketaddr_inPtr() { return &m_socketaddr_in; }
	int GetSizesocketaddr_in() const { return m_nSizesocketaddr_in; }
	int* GetSizesocketaddr_inPtr() { return &m_nSizesocketaddr_in; }
	BYTE GetSessionEnd() const { return m_bSessionEnd; }		// 2013-03-13 by hskim, �� ĳ�� ���� - RawData ���� ��� �߰�

	// Operations
	// 2008-03-13 by cmkwon, ��Ը� ����� Ŭ���̾�Ʈ �ñ�°� ���� - �����߰�(SSendedOverlappedInfo *o_pSendedOverInfo)
	BOOL AddData(SSendedOverlappedInfo *o_pSendedOverInfo, const BYTE* pData, int nSize);					// COverlapped Ŭ������ ���� ���ۿ� ����Ÿ�� �߰��Ѵ�.
	int GetDataSize() const { return m_WSABUF.len; }		// COverlapped Ŭ������ ���� ���ۿ� �߰��� ����Ÿ�� ����� ����
	void ResetOverlapped();								// COverlapped Ŭ������ �ʱ�ȭ �Ѵ�.
	void EncodePacket(BYTE seq);							// m_bEncodingFlag �� TRUE�̰� m_bEncodedFlag �� FALSE�϶� Encoding�� �Ѵ�.
	void SetSequenceNumber(BYTE seq);						// m_bEncodingFlag �� FALSE�϶� Sequence Number �� �����Ѵ�.
	BYTE GetSequenceNumber();								// m_bEncodingFlag �� TRUE�̰� m_bEncodedFlag �� TRUE�϶� ��Ŷ�� ������ Sequence Number
															// m_bEncodingFlag �� FALSE�϶� ��Ŷ�� ������ Sequence Number
	bool IsPacketEncoded() const { return m_bEncodedFlag; }	// ��Ŷ�� Encoding �Ǿ������� ���θ� ����

	void SetSessionEnd(BYTE bSessionEnd);					// 2013-03-13 by hskim, �� ĳ�� ���� - RawData ���� ��� �߰�

	///////////////////////////////////////////////////////////////////////////
	// new, delete �� �������̵��Ѵ�.
	void* operator new(size_t size);
	void operator delete(void* p);
	
private:	
	BYTE				m_enOperationMode;					// Read Or Write Mode ����
	BYTE				m_enOverlappedType;					// Overlapped ��ü�� Static or Dynamic Type ����
	BYTE				m_bEncodingFlag;					// COverlapped �� Encoding �� ���� �������� Flag
	BYTE				m_bEncodedFlag;						// COverlapped �� Encoding �Ǿ������� Flag
	WSABUF				m_WSABUF;							// ReadFile and WriteFile ����	
	char				lpBuff[SIZE_MAX_SOCKET_BUFFER];		// Data Buffer
	sockaddr_in			m_socketaddr_in;					// UDP ������ ���� ����(����� �ּҸ� ����)
	int					m_nSizesocketaddr_in;				// UDP ������ ���� ����(socketaddr ������)

	BYTE				m_enDataType;						// Date Type (Raw / Game Packet Type ����)	- // 2013-03-13 by hskim, �� ĳ�� ���� - RawData ���� ��� �߰�
	BYTE				m_bSessionEnd;						// ������ ���� �� ���� ���� ����			- // 2013-03-13 by hskim, �� ĳ�� ���� - RawData ���� ��� �߰�

	///////////////////////////////////////////////////////////////////////////
	// static ��� ����
	static const char*		pXOR_ENCODE_BYTES;	
};

inline void SAFE_DELETE_COverlapped(COverlapped*& pOver)
{
	if (pOver != nullptr && pOver->GetOverlappedType() == COverlapped::EN_OVERLAPPED_TYPE_DYNAMIC)
	{
		delete pOver;
		pOver = nullptr;
	}
}
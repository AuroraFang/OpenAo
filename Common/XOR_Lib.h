#ifndef _XOR_LIB_H_
#define	_XOR_LIB_H_
///////////////////////////////////////////////////////////////////////////////
// 2007-10-23 by cmkwon, �߰���

#define SIZE_MAX_XOR_DATA_TO_STRING		500			// 2007-10-23 by cmkwon, XOR�� ���ڵ� �����͸� ��Ʈ������ ��ȯ �� �� �ִ� �ִ� ����
#define SIZE_MAX_XOR_KEY_STRING			1024		// 2007-10-23 by cmkwon, XOR key�� ��� ������ �ִ� ����, NULL ���� �������� ���� ������

class XOR
{
public:
	XOR();
	~XOR();


	static BOOL XOREncode(BYTE *o_pbyEncoded, BYTE *i_pbySource, int i_nSourceSize, char *i_szKeyString);
	static BOOL XOREncode(BYTE *o_pbyEncoded, char *i_szSource, char *i_szKeyString);
	static BOOL XORBinary2String(char *o_szEncodedString, BYTE *i_byBinaryData, int i_nDataSize);
	static BOOL XORString2Binary(BYTE *o_byBinaryData, char *o_szEncodedString);

	// 2008-04-23 by cmkwon, PreServer �ּҸ� IP�� ������ �Ѵ� ���� - 
	static BOOL XORDecrypt(char *o_pszOutputString, char *i_pszInputString, char *i_pKeyString);

private:

};



#endif // END - #ifndef _XOR_LIB_H_

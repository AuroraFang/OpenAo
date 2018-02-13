#ifndef _VOIP_ERROR_H_
#define _VOIP_ERROR_H_

///////////////////////////////////////////////////////////////////////////////
// 
	#define VOIPERR_PROTOCOL_INVAILD_LENGTH					0x0001		// 

///////////////////////////////////////////////////////////////////////////////
// 1:1 ��ȭ�� ����
	#define VOIPERR_1to1_NO_SUCH_CHARACTER					0x1001		// 
	#define VOIPERR_1to1_NO_SUCH_ACCOUNTUNIQUENUMBER		0x1002
	#define VOIPERR_1to1_CANNOT_CALL						0x1003
	#define VOIPERR_1to1_REJECT								0x1004

///////////////////////////////////////////////////////////////////////////////
// N:N ��ȭ�� ����
	#define VOIPERR_NtoN_NO_SUCH_WELLSUITED_VCSERVER		0x2001		// ������ VCServer�� ����, ��ϵ� VCServer�� ���ų� ���Ӱ����� VCServer�� ����
	#define VOIPERR_NtoN_ALREADY_REGISTER_VCSERVER			0x2002		// �̹� VCServer�� ��ϵǾ���
	#define VOIPERR_NtoN_ALREADY_EXISTS_ROOMID				0x2003		// �̹� ��ϵ� RoomID��
	#define VOIPERR_NtoN_NO_SUCH_ROOMID						0x2004		// RoomID�� ���� Room�� ����
	#define VOIPERR_NtoN_NO_SUCH_ROOM						0x2005		// Room�� ����
	#define VOIPERR_NtoN_ALREADY_EXISTS_ROOM				0x2006		// �̹� Room�� ����
	#define VOIPERR_NtoN_FAIL_REQUEST_TOKEN					0x2007		// ���ϱ� ���� ��û ����
	#define VOIPERR_NtoN_LOST_TOKEN							0x2008		// ���ϱ� ���� ����
	#define VOIPERR_NtoN_CANNOT_TALKING						0x2009		// ���ϱ� ���� ����

	


#endif// _VOIP_ERROR_H_endif
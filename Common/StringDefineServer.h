
// 2005-04-28 by cmkwon
// #include "StringDefineServer.h"
#ifndef _STRING_DEFINE_SERVER_H_
#define _STRING_DEFINE_SERVER_H_




///////////////////////////////////////////////////////////////////////////////
// 1 Atum
	// 1-1 
	#define STRERR_S_ATUMEXE_0001 "������ ����Ǿ� ���� �ʽ��ϴ�."
	#define STRERR_S_ATUMEXE_0002 "Socket Closed by Pre Server!"
	#define STRERR_S_ATUMEXE_0003 "�ڵ� ������Ʈ�� �����Ͽ����ϴ�.\r\n������ �缳ġ���ֽñ� �ٶ��ϴ�.\r\n"
	#define STRERR_S_ATUMEXE_0004 "ERROR %s(%#04X) received from %s[%s]\r\n"
	#define STRERR_S_ATUMEXE_0005 "Unknown Error: %s(%#04x)"
	#define STRERR_S_ATUMEXE_0006 "�ٿ�ε� ������ ������ �� �����ϴ�."
	#define STRERR_S_ATUMEXE_0007 "�ٿ�ε� ������ ũ�⸦ �� �� �����ϴ�."
	#define STRERR_S_ATUMEXE_0008 "������Ʈ ������ �ٿ�ε� ���� �� �����ϴ�."
	#define STRERR_S_ATUMEXE_0009 "The system is out of memory or resources."
	#define STRERR_S_ATUMEXE_0010 "The .exe file is invalid."
	#define STRERR_S_ATUMEXE_0011 "������ ã�� �� �����ϴ�."
	#define STRERR_S_ATUMEXE_0012 "The specified path was not found. "
// 2006-04-20 by cmkwon, 	#define STRERR_S_ATUMEXE_0013 "[Error]Unknown Message Type: %d(0x%08X)\n"
// 1_end
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 2 - AtumLauncher
	// 2-1 STRMSG
	#define STRMSG_S_ATUMLAUNCHER_0000 "update completed"
	#define STRMSG_S_ATUMLAUNCHER_0001 "������ �����ϼ���."
	#define STRMSG_S_ATUMLAUNCHER_0002 "Updating Delete File List v%s\r\n"
	#define STRMSG_S_ATUMLAUNCHER_0003 "deleting files"
	#define STRMSG_S_ATUMLAUNCHER_0004 "updating notice"
	#define STRMSG_S_ATUMLAUNCHER_0005 "update completed(%s -> %s)"
	#define STRMSG_S_ATUMLAUNCHER_0006 "���� ���� ����"
	#define STRMSG_S_ATUMLAUNCHER_0007 "���� ��� ����"
	#define STRMSG_S_ATUMLAUNCHER_0008 "Downloading is canceled"
	#define STRMSG_S_ATUMLAUNCHER_0009 "Download finished"
	#define STRMSG_S_ATUMLAUNCHER_0010 "updating to v%s - %s(%d/%d)"
	#define STRMSG_S_ATUMLAUNCHER_0011 "Unable to create file %s"
	#define STRMSG_S_ATUMLAUNCHER_0012 "updating to v%s - %s(%d/%d)"
	#define STRMSG_S_ATUMLAUNCHER_0013 "Getting File Information %s"
	
	// STRERR
	#define STRERR_S_ATUMLAUNCHER_0000 "[Error] Parameter Count Error, Count(%d)\n"
	#define STRERR_S_ATUMLAUNCHER_0001 "[Error] Mutex Error\n"
	#define STRERR_S_ATUMLAUNCHER_0002 "[Error] Excute Type Error, Type(%s)\n"
	#define STRERR_S_ATUMLAUNCHER_0003 "[Error] Decryption ID Error, DecryptedID(%s)\n"
	#define STRERR_S_ATUMLAUNCHER_0004 "���� ������ ������ �� �����ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0005 "���� ���ӿ� �����Ͽ����ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0006 "Socket Closed by Pre Server!"
	#define STRERR_S_ATUMLAUNCHER_0007 "Notice File Error!"
//	#define STRERR_S_ATUMLAUNCHER_0008 "������ �ٽ� �ٿ�ε�޾��ּ���.\nURL: http://space.mgame.com/\n�ֽ� ����: "
	#define STRERR_S_ATUMLAUNCHER_0009 "�ڵ� ������Ʈ�� �����Ͽ����ϴ�.\r\n������ �缳ġ���ֽñ� �ٶ��ϴ�.\r\n"
	#define STRERR_S_ATUMLAUNCHER_0010 "��� ������ ��Ȱ�� �����Դϴ�."
	#define STRERR_S_ATUMLAUNCHER_0011 "%-16s%s������..."
	#define STRERR_S_ATUMLAUNCHER_0012 "��� ������ �������Դϴ�. ����Ŀ� ������ �ֽʽÿ�."
	#define STRERR_S_ATUMLAUNCHER_0013 "ERROR %s(%#04X) received from %s[%s]\r\n"
// 2006-08-24 by cmkwon, Change to STRMSG_060824_0000
//	#define STRERR_S_ATUMLAUNCHER_0014 "Ʋ�� ���̵�, ��й�ȣ ����\n\n* ����� ��Ÿ �׽��ͷ� ������ �е鸸 �α����� �����մϴ�"
	#define STRERR_S_ATUMLAUNCHER_0015 "�α��� ���� ����"
	#define STRERR_S_ATUMLAUNCHER_0016 "����� ���̵� ���Է�"
	#define STRERR_S_ATUMLAUNCHER_0017 "���� �α���"
	#define STRERR_S_ATUMLAUNCHER_0018 "F ������ ����Ǿ� ���� �ʽ��ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0019 "I ������ ����Ǿ� ���� �ʽ��ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0020 "���񽺰� �Ͻ������� �ߴܵǾ� �ֽ��ϴ�.\n\n��� �Ŀ� �ٽ� �������ּ���."
	#define STRERR_S_ATUMLAUNCHER_0021 "���� ����ڰ� �ʹ� �����ϴ�.\n\n��� �Ŀ� �ٽ� �������ּ���."
	#define STRERR_S_ATUMLAUNCHER_0022 "���� ���� �з� �����Դϴ�.\n �Ⱓ : %s\n\n�ڼ��� ������ ���̽��¶��� �����ͷ� ���ǹٶ��ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0023 "Ŭ���̾�Ʈ ������ ���� �ʽ��ϴ�.\n\n������ �ٽ� �ٿ�ε� �޾� �ּ���."
	#define STRERR_S_ATUMLAUNCHER_0024 "ERROR: %s(%#04X)"
	#define STRERR_S_ATUMLAUNCHER_0025 "�ٿ�ε� ������ ������ �� �����ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0026 "�ٿ�ε� ����(%s)�� ũ�⸦ �� �� �����ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0027 "�ٿ�ε� ������ ũ�⸦ �� �� �����ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0028 "������Ʈ ������ �ٿ�ε� ���� �� �����ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0029 "������Ʈ ������ �������� �ʽ��ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0030 "�����Ͻ� ������ �������Դϴ�. ����Ŀ� ������ �ֽʽÿ�."
	#define STRERR_S_ATUMLAUNCHER_0031 "The system is out of memory or resources."
	#define STRERR_S_ATUMLAUNCHER_0032 "The .exe file is invalid."
	#define STRERR_S_ATUMLAUNCHER_0033 "������ ã�� �� �����ϴ�."
	#define STRERR_S_ATUMLAUNCHER_0034 "��θ� ã�� �� �����ϴ�."
// 2006-04-20 by cmkwon	#define STRERR_S_ATUMLAUNCHER_0035 "[Error] Unhandled Message Type: %s(%#04X)\n"
// 2006-04-20 by cmkwon	#define STRERR_S_ATUMLAUNCHER_0036 "[Error] Unhandled Message Type!\n"

	#define STRMSG_S_050506		"\'%s\'���� ���� ���� �з� �����Դϴ�.\n  ����: %s\n  �Ⱓ: %s~%s\n\n�ڼ��� ������ ���̽��¶��� �����ͷ� ���ǹٶ��ϴ�."
	#define STRMSG_S_050930		"������ �ٽ� �ٿ�ε�޾��ּ���.\nURL: %s\n�ֽ� ����: "
// 2_end
///////////////////////////////////////////////////////////////////////////////	

///////////////////////////////////////////////////////////////////////////////
// 3 - AtumAdminTool
	// 3-1 STRMSG
//	#define STRMSG_S_SCADMINTOOL_0000 "��"
//	#define STRMSG_S_SCADMINTOOL_0001 "��"
//	#define STRMSG_S_SCADMINTOOL_0002 "%d��, %d��"
//	#define STRMSG_S_SCADMINTOOL_0003 "���� ���� ������ �����Ͻðڽ��ϱ�?"
//	#define STRMSG_S_SCADMINTOOL_0004 "CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
//	#define STRMSG_S_SCADMINTOOL_0005 "CAST(l.ChangeCount AS VARCHAR(10)) + '�� ����, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
//	#define STRMSG_S_SCADMINTOOL_0006 "'''' + l.PeerCharacterName + '''���� ' + CAST(l.ChangeCount AS VARCHAR(10)) + '�� ��, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
//	#define STRMSG_S_SCADMINTOOL_0007 "'''' + l.PeerCharacterName + '''���� ' + CAST(l.ChangeCount AS VARCHAR(10)) + '�� ����, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
//	#define STRMSG_S_SCADMINTOOL_0008 "CAST(l.ChangeCount AS VARCHAR(10)) + '�� ����, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
//	#define STRMSG_S_SCADMINTOOL_0009 "CAST(l.ChangeCount AS VARCHAR(10)) + '�� ����, ���� Spi: ' + CAST(l.RemainedMoney AS VARCHAR(10))"
//	#define STRMSG_S_SCADMINTOOL_0010 "CAST(l.ChangeCount AS VARCHAR(10)) + '�� �Ǹ�, ���� Spi: ' + CAST(l.RemainedMoney AS VARCHAR(10))"
//	#define STRMSG_S_SCADMINTOOL_0011 "CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
	#define STRMSG_S_SCADMINTOOL_0012 "'''' + l.PeerCharacterName + '''�� ' + CAST(l.ChangeCount AS VARCHAR(10)) + '�� �߰���, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
	#define STRMSG_S_SCADMINTOOL_0013 "'''' + l.PeerCharacterName + '''�� ' + CAST(l.ChangeCount AS VARCHAR(10)) + '�� ������, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
	#define STRMSG_S_SCADMINTOOL_0014 "CAST(l.ChangeCount AS VARCHAR(10)) + '�� �߰�, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
	#define STRMSG_S_SCADMINTOOL_0015 "CAST(l.ChangeCount AS VARCHAR(10)) + '�� �ñ�, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
	#define STRMSG_S_SCADMINTOOL_0016 "CAST(l.ChangeCount AS VARCHAR(10)) + '�� ã��, ' + CAST(l.CurrentCount AS VARCHAR(10)) + '��'"
	#define STRMSG_S_SCADMINTOOL_0017 "\'���ӽð� \' + dbo.atum_GetHMSFromS(l.PlayTime) + \', �� \' + dbo.atum_GetHMSFromS(l.TotalPlayTime)"
	#define STRMSG_S_SCADMINTOOL_0018 "CAST(l.Param1 AS VARCHAR(10)) + ' -> ' + CAST(l.Param2 AS VARCHAR(10)) + ', ���ӽð�: ' + dbo.atum_GetHMSFromS(l.Param3)"
//	#define STRMSG_S_SCADMINTOOL_0019 "CAST(l.Param1 AS VARCHAR(15)) + \' ����, �� \' + CAST(l.Param2 AS VARCHAR(15))"
//	#define STRMSG_S_SCADMINTOOL_0020 "�浹"
//	#define STRMSG_S_SCADMINTOOL_0021 "����"
//	#define STRMSG_S_SCADMINTOOL_0022 "���"
//	#define STRMSG_S_SCADMINTOOL_0023 "���κҸ�"
//	#define STRMSG_S_SCADMINTOOL_0024 "%s, ���� ����: %s"
	#define STRMSG_S_SCADMINTOOL_0025 "(����)"
//	#define STRMSG_S_SCADMINTOOL_0026 "��"
//	#define STRMSG_S_SCADMINTOOL_0027 "����"
//	#define STRMSG_S_SCADMINTOOL_0028 "���� ��� ����"
	#define STRMSG_S_SCADMINTOOL_0029 "���� ��"
	#define STRMSG_S_SCADMINTOOL_0030 "ä�� ����"
//	#define STRMSG_S_SCADMINTOOL_0031 "���� �α�"
//	#define STRMSG_S_SCADMINTOOL_0032 "���� �α�"
//	#define STRMSG_S_SCADMINTOOL_0033 "������ �α�"
	#define STRMSG_S_SCADMINTOOL_0034 "%s - %s ����"
	#define STRMSG_S_SCADMINTOOL_0035 "%s - %s ����,%d(%d)"
	#define STRMSG_S_SCADMINTOOL_0036 " ���� �з�"
//	#define STRMSG_S_SCADMINTOOL_0037 "�з�    "
//	#define STRMSG_S_SCADMINTOOL_0038 "��"
	#define STRMSG_S_SCADMINTOOL_0039 "���� ������ �з� ���¸� �����Ͻðڽ��ϱ�?"
	#define STRMSG_S_SCADMINTOOL_0040 "%s(%d��)"
	#define STRMSG_S_SCADMINTOOL_0041 "%d�� %d��"
	#define STRMSG_S_SCADMINTOOL_0042 "���� ���� ������ ���� �����ðڽ��ϱ�?"
	#define STRMSG_S_SCADMINTOOL_0043 "%s ������"
	#define STRMSG_S_SCADMINTOOL_0044 "[%s %15s] �������� : %s\r\n"
	#define STRMSG_S_SCADMINTOOL_0045 "����ڼ�: %d\r\n"
	#define STRMSG_S_SCADMINTOOL_0046 "[%s %15s] �����޽��� : %s\r\n"
	#define STRMSG_S_SCADMINTOOL_0047 "[%s %15s] ������ : %4d\r\n"
	#define STRMSG_S_SCADMINTOOL_0048 "[%s %15s] FieldServer state : %d\r\n"
	#define STRMSG_S_SCADMINTOOL_0049 "�������� ������ ����Ǿ����ϴ�\nSocket Name: %s\nIP: %s"
	
	// 3-2 AtumAdminTool - STRERR
	#define STRERR_S_SCADMINTOOL_0000 "�ش� ���� ������ �о�� �� �����ϴ�."
	#define STRERR_S_SCADMINTOOL_0001 "���� ���� ������ �����Ͽ����ϴ�."
	#define STRERR_S_SCADMINTOOL_0002 "��й�ȣ�� �Է��Ͻÿ�"
	#define STRERR_S_SCADMINTOOL_0003 "��й�ȣ�� Ʋ���ϴ�"
	#define STRERR_S_SCADMINTOOL_0004 "�α��� �̸��� �Է��ϼ���"
	#define STRERR_S_SCADMINTOOL_0005 "Pre Server�� ����Ǿ� ���� �ʽ��ϴ�."
	#define STRERR_S_SCADMINTOOL_0006 "Can not connect to PreServer !!"
//	#define STRERR_S_SCADMINTOOL_0007 "���� �����Ͽ����ϴ�"			// 2006-04-11 by cmkwon, �ּ�ó����
	#define STRERR_S_SCADMINTOOL_0008 "ERROR: �ش� ���������� �������� �ʽ��ϴ�."
	#define STRERR_S_SCADMINTOOL_0009 "����ڸ��� �Է��ϼ���"
	#define STRERR_S_SCADMINTOOL_0010 "������ �Է��ϼ���"
	#define STRERR_S_SCADMINTOOL_0011 "����ġ ���� ���� : Level %2d ==> Exp(%.1I64f ~ %.1I64f)"
	#define STRERR_S_SCADMINTOOL_0012 "�ִ� �α� ���� �ʰ��Ǿ����ϴ�.\r\n\r\n�ִ� �α� ���� �˻� ������ ���� �ٶ��ϴ�."
	#define STRERR_S_SCADMINTOOL_0013 "Database�� ������ �� �����ϴ�."
	#define STRERR_S_SCADMINTOOL_0014 "����ȵ�"
	#define STRERR_S_SCADMINTOOL_0015 "�����"
	#define STRERR_S_SCADMINTOOL_0016 "������Ʈ��"
	#define STRERR_S_SCADMINTOOL_0017 "�α��ε�"
	#define STRERR_S_SCADMINTOOL_0018 "ĳ���ͼ�����"
	#define STRERR_S_SCADMINTOOL_0019 "������"
	#define STRERR_S_SCADMINTOOL_0020 "�˼�����"
	#define STRERR_S_SCADMINTOOL_0021 "Database %s(%s:%d)�� ������ �� �����ϴ�."
	#define STRERR_S_SCADMINTOOL_0022 "������ �߰� ����"
	#define STRERR_S_SCADMINTOOL_0023 "�з� ���� ã�� �����Ͽ����ϴ�."
	#define STRERR_S_SCADMINTOOL_0024 "������ ���� ���� ���� ������ ���� ���� �з��� �ּ���!"
	#define STRERR_S_SCADMINTOOL_0025 "SPI(�Ӵ�)�� �߰����� ���մϴ�."
	#define STRERR_S_SCADMINTOOL_0026 "������ �������� �̹� �����ϹǷ� ������ �����Ͻñ� �ٶ��ϴ�"
	#define STRERR_S_SCADMINTOOL_0027 "������ ã�� ����"
	#define STRERR_S_SCADMINTOOL_0028 "ĳ���Ͱ� ������ SPI(�Ӵ�)�� �������� ���մϴ�."
	#define STRERR_S_SCADMINTOOL_0029 "�������� �����Ͻðڽ��ϱ�?"
	#define STRERR_S_SCADMINTOOL_0030 "������ ������ ���еǾ����ϴ�."
	#define STRERR_S_SCADMINTOOL_0031 "������ ������ ���еǾ����ϴ�."
	#define STRERR_S_SCADMINTOOL_0032 "������ �Է��ϼ���"
	#define STRERR_S_SCADMINTOOL_0033 "������ �������� �ʽ��ϴ�.(������ ������ ������)"
	#define STRERR_S_SCADMINTOOL_0034 "���� Ȥ�� ĳ���� �������� �ʽ��ϴ�."
	#define STRERR_S_SCADMINTOOL_0035 "ĳ���� ���� ��ȸ �����Դϴ�."
	#define STRERR_S_SCADMINTOOL_0036 "�ش� ĳ���Ͱ� �������� �ʽ��ϴ�"
	#define STRERR_S_SCADMINTOOL_0037 "ĳ���� ������Ʈ �����Դϴ�"
	#define STRERR_S_SCADMINTOOL_0038 "ĳ���� ������ ���������� ������Ʈ�Ǿ����ϴ�"
	#define STRERR_S_SCADMINTOOL_0039 "�������� �����ϼ���."
	#define STRERR_S_SCADMINTOOL_0040 "������ ������ �����ϼ���."
	#define STRERR_S_SCADMINTOOL_0041 "�ش� �������� 5�� ���Ϸθ� ���� �����մϴ�."
	#define STRERR_S_SCADMINTOOL_0042 "�޽����� �Է��ϼ���."
	#define STRERR_S_SCADMINTOOL_0043 "������ ������ �� �����ϴ�."
	#define STRERR_S_SCADMINTOOL_0044 "%s: server state(%d)\r\n"
	#define STRERR_S_SCADMINTOOL_0045 "IM Server�� ����Ǿ� ���� �ʽ��ϴ�."
	#define STRERR_S_SCADMINTOOL_0046 "Field Server�� ����Ǿ� ���� �ʽ��ϴ�."
	#define STRERR_S_SCADMINTOOL_0047 "[%s %15s] IMServer state : %d\r\n"

	// 3-3 AtumLaAtumAdminTool -
//	#define STRMSG_S_SCAT_COLNAME_0000 "������"
//	#define STRMSG_S_SCAT_COLNAME_0001 "Ÿ��"
//	#define STRMSG_S_SCAT_COLNAME_0002 "����"
//	#define STRMSG_S_SCAT_COLNAME_0003 "������"
//	#define STRMSG_S_SCAT_COLNAME_0004 "������"
//	#define STRMSG_S_SCAT_COLNAME_0005 "ó����"
//	#define STRMSG_S_SCAT_COLNAME_0006 "ó������"
//	#define STRMSG_S_SCAT_COLNAME_0007 "��¥"
//	#define STRMSG_S_SCAT_COLNAME_0008 "�α�Ÿ��"
//	#define STRMSG_S_SCAT_COLNAME_0009 "IPAddress"
//	#define STRMSG_S_SCAT_COLNAME_0010 "������"
//	#define STRMSG_S_SCAT_COLNAME_0011 "ĳ���͸�"
//	#define STRMSG_S_SCAT_COLNAME_0012 "��ġ"
//	#define STRMSG_S_SCAT_COLNAME_0013 "����"
//	#define STRMSG_S_SCAT_COLNAME_0014 "������"
//	#define STRMSG_S_SCAT_COLNAME_0015 "UID"
//	#define STRMSG_S_SCAT_COLNAME_0016 "������ȣ"
//	#define STRMSG_S_SCAT_COLNAME_0017 "�̸�"
//	#define STRMSG_S_SCAT_COLNAME_0018 "�����۹�ȣ"
//	#define STRMSG_S_SCAT_COLNAME_0019 "Prefix"
//	#define STRMSG_S_SCAT_COLNAME_0020 "Suffix"
	#define STRMSG_S_SCAT_COLNAME_0021 "����"
//	#define STRMSG_S_SCAT_COLNAME_0022 "����"
//	#define STRMSG_S_SCAT_COLNAME_0023 "������"
//	#define STRMSG_S_SCAT_COLNAME_0024 "�����ð�"
	#define STRMSG_S_SCAT_COLNAME_0025 "������"
//	#define STRMSG_S_SCAT_COLNAME_0026 "â��"
//	#define STRMSG_S_SCAT_COLNAME_0027 "���"
//	#define STRMSG_S_SCAT_COLNAME_0028 "��"
//	#define STRMSG_S_SCAT_COLNAME_0029 "��ǥ"
//	#define STRMSG_S_SCAT_COLNAME_0030 "����"
//	#define STRMSG_S_SCAT_COLNAME_0031 "����"
//	#define STRMSG_S_SCAT_COLNAME_0032 "����"
//	#define STRMSG_S_SCAT_COLNAME_0033 "��������"
//	#define STRMSG_S_SCAT_COLNAME_0034 "����"
//	#define STRMSG_S_SCAT_COLNAME_0035 "�ְ���"
//	#define STRMSG_S_SCAT_COLNAME_0036 "����ġ"
//	#define STRMSG_S_SCAT_COLNAME_0037 "���ҵȰ���ġ"
//	#define STRMSG_S_SCAT_COLNAME_0038 "�ڵ��й轺��Ÿ��"
//	#define STRMSG_S_SCAT_COLNAME_0039 "������Ʈ"
//	#define STRMSG_S_SCAT_COLNAME_0040 "�����Ʈ"
//	#define STRMSG_S_SCAT_COLNAME_0041 "������Ʈ"
//	#define STRMSG_S_SCAT_COLNAME_0042 "������Ʈ"
//	#define STRMSG_S_SCAT_COLNAME_0043 "������Ʈ"
//	#define STRMSG_S_SCAT_COLNAME_0044 "ȸ����Ʈ"
//	#define STRMSG_S_SCAT_COLNAME_0045 "�Ҽӿ���"
//	#define STRMSG_S_SCAT_COLNAME_0046 "����"
//	#define STRMSG_S_SCAT_COLNAME_0047 "�ź�"
//	#define STRMSG_S_SCAT_COLNAME_0048 "�����ӽð�"
//	#define STRMSG_S_SCAT_COLNAME_0049 "�����ð�"
//	#define STRMSG_S_SCAT_COLNAME_0050 "�������ӽð�"
//	#define STRMSG_S_SCAT_COLNAME_0051 "����"
//	#define STRMSG_S_SCAT_COLNAME_0052 "��ü"
	#define STRMSG_S_SCAT_COLNAME_0053 "�����ƽ��(1-1��)"
	#define STRMSG_S_SCAT_COLNAME_0054 "��ĭ��(1-1��)"	
	#define STRMSG_S_SCAT_COLNAME_0055 "��󸮽�Ʈ��(1-1��)" // 2005-08-01 by hblee : �׷����̵� -> ��󸮽�Ʈ�� ����.
	#define STRMSG_S_SCAT_COLNAME_0056 "ĳ���(1-1��)"
	#define STRMSG_S_SCAT_COLNAME_0057 "�����÷�(1-2��)"
	#define STRMSG_S_SCAT_COLNAME_0058 "��Ʋ����(1-2��)"
	#define STRMSG_S_SCAT_COLNAME_0059 "��ó��(1-2��)"
	#define STRMSG_S_SCAT_COLNAME_0060 "�޽�����̺��(1-2��)"
	#define STRMSG_S_SCAT_COLNAME_0061 "���Ϸ�(2-1��)"
	#define STRMSG_S_SCAT_COLNAME_0062 "�̻��Ϸ�(2-1��)"
	#define STRMSG_S_SCAT_COLNAME_0063 "�����(2-1��)"
	#define STRMSG_S_SCAT_COLNAME_0064 "���η�(2-1��)"
	#define STRMSG_S_SCAT_COLNAME_0065 "�����(2-2��)"
	#define STRMSG_S_SCAT_COLNAME_0066 "���̷�(2-2��)"
	#define STRMSG_S_SCAT_COLNAME_0067 "�ȼ���(2-2��)"
	#define STRMSG_S_SCAT_COLNAME_0068 "�����̷�(2-2��)"
	#define STRMSG_S_SCAT_COLNAME_0069 "����"
	#define STRMSG_S_SCAT_COLNAME_0070 "��������"
	#define STRMSG_S_SCAT_COLNAME_0071 "��������"
	#define STRMSG_S_SCAT_COLNAME_0072 "�ݼӷ�"
	#define STRMSG_S_SCAT_COLNAME_0073 "ī���"
	#define STRMSG_S_SCAT_COLNAME_0074 "��æƮ��"
	#define STRMSG_S_SCAT_COLNAME_0075 "��ũ��"
	#define STRMSG_S_SCAT_COLNAME_0076 "ź�˷�"
	#define STRMSG_S_SCAT_COLNAME_0077 "����Ʈ��"
	#define STRMSG_S_SCAT_COLNAME_0078 "���̴���"
	#define STRMSG_S_SCAT_COLNAME_0079 "��ǻ�ͷ�"
	#define STRMSG_S_SCAT_COLNAME_0080 "����ī���"
	#define STRMSG_S_SCAT_COLNAME_0081 "��æƮ�ı�������"		// 2005-08-02 by cmkwon
	#define STRMSG_S_SCAT_COLNAME_0082 "�����ͷ�"				// 2005-08-02 by cmkwon
	#define STRMSG_S_SCAT_COLNAME_0083 "���ϰǷ�"				// 2005-08-02 by cmkwon
//	#define STRMSG_S_SCAT_COLNAME_0081 "��ü ������"
//	#define STRMSG_S_SCAT_COLNAME_0082 "��ü ����"
//	#define STRMSG_S_SCAT_COLNAME_0083 "������"
//	#define STRMSG_S_SCAT_COLNAME_0084 "���� ����"
//	#define STRMSG_S_SCAT_COLNAME_0085 "���� IP, Port"
//	#define STRMSG_S_SCAT_COLNAME_0086 "���� ���� ��"
//	#define STRMSG_S_SCAT_COLNAME_0087 "���� ����"
//	#define STRMSG_S_SCAT_COLNAME_0088 "�׷� ����"
//	#define STRMSG_S_SCAT_COLNAME_0089 "�ʵ� ����"
//	#define STRMSG_S_SCAT_COLNAME_0090 "�� �� ����"
//	#define STRMSG_S_SCAT_COLNAME_0091 "����ȵ�"
//	#define STRMSG_S_SCAT_COLNAME_0092 "����"
//	#define STRMSG_S_SCAT_COLNAME_0093 "������"
	#define STRMSG_S_SCAT_COLNAME_0094 "���"

	#define STRMSG_S_SCADMINTOOL_050512_0000	"CAST(l.CurrentCount AS VARCHAR(10)) + '��, Param1:' + CAST(l.Param1 AS VARCHAR(10))"
// 3_end
///////////////////////////////////////////////////////////////////////////////
	
	
///////////////////////////////////////////////////////////////////////////////
// 4
	// 4-1 AtumMonitor - STRMSG
//	#define STRMSG_S_SCMONITOR_0000 "��ɾ� ����Ʈ\r\n"
//	#define STRMSG_S_SCMONITOR_0001 "----- ����� ���� ����Ʈ ---------------------------------------\r\n"
//	#define STRMSG_S_SCMONITOR_0002 "  ����: \'%s\', ���� ��ȣ: \'%s\', �ӽ� ��ȣ: \'%s\'\r\n"
//	#define STRMSG_S_SCMONITOR_0003 "  ����: \'%s\' \r\n"
//	#define STRMSG_S_SCMONITOR_0004 "������Ʈ�� Version���� ���� �ִ� ������ �����ϼ���"
//	#define STRMSG_S_SCMONITOR_0005 "\r\nMaking New Zip File From %s To %s...\r\n"
//	#define STRMSG_S_SCMONITOR_0006 "Rename Server Group"
//	#define STRMSG_S_SCMONITOR_0007 "������ ���������� �����Ǿ����ϴ�.\r\n\r\nDB ����: %s(%d), %s"
//	#define STRMSG_S_SCMONITOR_0008 "%04d(%2d -  Ȱ��) %3d/%3d"
//	#define STRMSG_S_SCMONITOR_0009 "%04d(%2d -��Ȱ��) %3d/%3d"
//	#define STRMSG_S_SCMONITOR_0010 "������ ����Ÿ�� ���������� ���ε��Ǿ����ϴ�."
//	#define STRMSG_S_SCMONITOR_0011 "���� ���°� ���������� �ݿ��Ǿ����ϴ�."
//	#define STRMSG_S_SCMONITOR_0012 "%04d(%2d -  Ȱ��)"
//	#define STRMSG_S_SCMONITOR_0013 "%04d(%2d -��Ȱ��)"
//	#define STRMSG_S_SCMONITOR_0014 "Version Info List Reload DONE!"
//	#define STRMSG_S_SCMONITOR_0015 "Blocked Account List Reload DONE!"
//	#define STRMSG_S_SCMONITOR_0016 "���� ���� ���񽺰� �����Ǿ����ϴ�."
//	#define STRMSG_S_SCMONITOR_0017 "���� ���� ���񽺰� ���۵Ǿ����ϴ�."
//	#define STRMSG_S_SCMONITOR_0018 "�ʵ� ������ ����Ǿ� ���� �ʽ��ϴ�"
//	#define STRMSG_S_SCMONITOR_0019 "���� Field Server�� �����Ͻðڽ��ϱ�?"
//	#define STRMSG_S_SCMONITOR_0020 "������Ʈ ���� ����Ʈ ����(�ִ� 1492 Bytes)\r\n\r\n    ��������Ʈ����[%3d], ����Ÿ�뷮[%4dBytes]"
//	#define STRMSG_S_SCMONITOR_0021 "���� Pre Server�� �����Ͻðڽ��ϱ�?"
//	#define STRMSG_S_SCMONITOR_0022 "���� IM Server�� �����Ͻðڽ��ϱ�?"
//	#define STRMSG_S_SCMONITOR_0023 "���� NPC Server�� �����Ͻðڽ��ϱ�?"
//	#define STRMSG_S_SCMONITOR_0024 "%Y�� %m�� %d�� %H�� %M�� %S��"
//	#define STRMSG_S_SCMONITOR_0025 "�̺�Ʈ ����(%d)"
//	#define STRMSG_S_SCMONITOR_0026 "���º�Ÿ �⼮ �̺�Ʈ(%d)"
//	#define STRMSG_S_SCMONITOR_0027 "�̺�Ʈ ��(%d)"
//	#define STRMSG_S_SCMONITOR_0028 "���� ������ ���� �ð�"
//	#define STRMSG_S_SCMONITOR_0029 "���� ������ �⺻ �ð�"
//	#define STRMSG_S_SCMONITOR_0030 "���ɿ���"

	// 4-2 AtumMonitor - STRERR
//	#define STRERR_S_SCMONITOR_0000 "  ==> ��ɾ �����Ͽ����ϴ�.\r\n"
//	#define STRERR_S_SCMONITOR_0001 "  ==> ��ɾ �����Ͽ����ϴ�.\r\n"
//	#define STRERR_S_SCMONITOR_0002 "DB�� ������ �� �����ϴ�."
//	#define STRERR_S_SCMONITOR_0003 "�ش� Version�� �������� �ʽ��ϴ�"
//	#define STRERR_S_SCMONITOR_0004 "������ ���� �̸��� �Է��ϼ���"
//	#define STRERR_S_SCMONITOR_0005 "����� ���� �̸��� �Է��ϼ���"
//	#define STRERR_S_SCMONITOR_0006 "���� ������ �Է��ϼ���"
//	#define STRERR_S_SCMONITOR_0007 "�� ������ �Է��ϼ���"
//	#define STRERR_S_SCMONITOR_0008 "������Ʈ�� Zip ������ ����� ������ �����ϼ���"
//	#define STRERR_S_SCMONITOR_0009 "������ ������ �� �����ϴ�"
//	#define STRERR_S_SCMONITOR_0010 "������ �����Ͻÿ�!"
//	#define STRERR_S_SCMONITOR_0011 "DB�� ������ �� �����ϴ�"
//	#define STRERR_S_SCMONITOR_0012 "[Error]Unable to process Message Type: %s(%#04x) in CLeftView::OnSocketNotify()\n"
//	#define STRERR_S_SCMONITOR_0013 "������Ʈ ���� ����Ʈ�� �ʹ� �����ϴ�.(��������Ʈ����[%3d], ����Ÿ�뷮[%4dBytes])\r\n\r\n    ���� ����Ʈ�� �����ؾ� �մϴ�."
//	#define STRERR_S_SCMONITOR_0014 "���ŵ� ����Դϴ�.\r\n���� ���� �̿��ϼ���."
//	#define STRERR_S_SCMONITOR_0015 "�������������� �ƴմϴ�"
//	#define STRERR_S_SCMONITOR_0016 "EDIT ��Ʈ���� ����� �����ϴ�."
//	#define STRERR_S_SCMONITOR_0017 "�̹� �����ϴ� ������ ����Ͽ����ϴ�."

// 4_end	
///////////////////////////////////////////////////////////////////////////////
	

///////////////////////////////////////////////////////////////////////////////
// 5 - FieldServer
	// 5-1 Field<->Log
	#define STRMSG_S_F2LOGCONNECT_0000 "[Error] WndProc(), Can't connect to LogServer[%15s:%4d] Reconnect\r\n"
	#define STRMSG_S_F2LOGCONNECT_0001 "Log Server�� ���� �Ͽ����ϴ�.\r\n"
	#define STRMSG_S_F2LOGCONNECT_0002 "Log Server[%15s:%4d]���� ������ ���� �Ǿ����ϴ�. �������� �õ��մϴ�.\r\n"

	// 5-2 Field<->Pre
	#define STRMSG_S_F2PRECONNECT_0000 "[Error] WndProc(), Can't connect to PreServer[%15s:%4d] Reconnect\r\n"
	#define STRMSG_S_F2PRECONNECT_0001 "Pre Server�� ���� �Ͽ����ϴ�.\r\n"
	#define STRMSG_S_F2PRECONNECT_0002 "  T_ERROR %s(%#04X) received from %s[%s]\r\n"
	#define STRMSG_S_F2PRECONNECT_0003 "Unknown Error@WM_PRE_PACKET_NOTIFY: %s(%#04x)\r\n"
	#define STRMSG_S_F2PRECONNECT_0004 "Pre Server[%15s:%4d]���� ������ ���� �Ǿ����ϴ�. �������� �õ��մϴ�.\r\n"

	// 5-3 Field<->IM
	#define STRMSG_S_F2IMCONNECT_0000 "[Error] WndProc(), Can't connect to  IMServer[%15s:%4d] Reconnect\r\n"
	#define STRMSG_S_F2IMCONNECT_0001 "IM Server�� ���� �Ͽ����ϴ�.\r\n"
	#define STRMSG_S_F2IMCONNECT_0002 "IM Server[%15s:%4d]���� ������ ���� �Ǿ����ϴ�. �������� �õ��մϴ�.\r\n"
	#define STRMSG_S_F2IMCONNECT_0003 "  T_ERROR %s(%#04X) received from %s[%s]\r\n"
	#define STRMSG_S_F2IMCONNECT_0004 "Unknown Error@WM_IM_PACKET_NOTIFY: %s(%#04x)\n"

	// 5-3 Field - DB
	#define STRMSG_S_F2DBQUERY_0000 "No such DB query In Field Server Queries! %d\r\n"
	#define STRMSG_S_F2DBQUERY_0001 "�ش� �������� �������� �ʽ��ϴ�."
	#define STRMSG_S_F2DBQUERY_0002 "'%s'�Բ����� �⼮ ��û�� �ϼ̱� ������ ����� ��������"
	#define STRMSG_S_F2DBQUERY_0003 "�����մϴ�. F5�� ������ �κ��丮�� Ȯ���� ������"
	#define STRMSG_S_F2DBQUERY_0004 "��ų %s(%d)�� �߰��Ǿ����ϴ�"
	#define STRMSG_S_F2DBQUERY_0005 "�ش� �������� �������� �ʽ��ϴ�."
	#define STRMSG_S_F2DBQUERY_0006 "������ ���ſ� �����Ͽ����ϴ�."

	// 5-4 Field - CityWar
	#define STRMSG_S_F2CITYWAR_0000 "  ���������� ���� : %d(%10s) occGuildName(%s)\r\n"
	#define STRMSG_S_F2CITYWAR_0001 "		  ���� ���� : GuildUID(%4d) GuildName(%10s) GuildMaster(%d)\r\n"
	#define STRMSG_S_F2CITYWAR_0002 "  ���������� ���� ���� : %d(%10s) occGuildName(%s)\r\n"
	#define STRMSG_S_F2CITYWAR_0003 "		  �������� ==> GuildName(%10s) SumOfDamage(%8.2f)\r\n"
	#define STRMSG_S_F2CITYWAR_0004 "  ���������� : %d(%10s) CityMapIndex(%d) QuestIndex(%d) OccGuildID(%d) OccGuildName(%s) OccGuildMasterUID(%d) �������ð�(%s)\r\n"
	#define STRMSG_S_F2CITYWAR_0005 "[Error] SetCityWarState_ DBError, MapIndex(%d)\r\n"
	#define STRMSG_S_F2CITYWAR_0006 "%d�� �� \"%s\" ������������ ���� �˴ϴ�."
	#define STRMSG_S_F2CITYWAR_0007 "%d�� �� \"%s\" ������������ ���� �˴ϴ�."
	#define STRMSG_S_F2CITYWAR_0008 "����������(%s) ���� ��ȯ�� : NPC ����"
	#define STRMSG_S_F2CITYWAR_0009 "����������(%s) ���� ��ȯ�� : %s ���� ����"
	#define STRMSG_S_F2CITYWAR_0010 "\"NPC\"�� \"%s\"�� ���� ���� �Ͽ����ϴ�."
	#define STRMSG_S_F2CITYWAR_0011 "\"%s\"������ \"%s\"�� ���� ���� �Ͽ����ϴ�."

	// 5-4 Field - Quest
	#define STRMSG_S_F2QUEST_0000 "����Ʈ �ε� ����"
	#define STRMSG_S_F2QUEST_0001 "����Ʈ�� �ε����� �ʾҽ��ϴ�.\r\n"
//	#define STRMSG_S_F2QUEST_0002 "����Ʈ \'%30s\' ��ȣ %d -> OK\r\n"

	// 5-4 Field - config
	#define STRMSG_S_F2CONFIG_0000 "�׽�Ʈ�� ���� ���õ�! \r\n\r\nLoadFieldServerDataDebug() ���Źٶ�! "
	#define STRMSG_S_F2NOTIFY_0000 "splash %d: %d���� %5.2f(%2.1f%%)"
	#define STRMSG_S_F2NOTIFY_0001 "monster splash %d: %s���� %5.2f"
	#define STRMSG_S_F2NOTIFY_0002 "1��"
	#define STRMSG_S_F2NOTIFY_0003 "2��"
	#define STRMSG_S_F2NOTIFY_0004 "��1(%s)"
	#define STRMSG_S_F2NOTIFY_0005 "��2(%s)"
	#define STRMSG_S_F2NOTIFY_0006 "�߸��� ���� ����! �����ڿ��� ����!"
	#define STRMSG_S_F2NOTIFY_0007 "1-1��: %4.1f vs %4.1f -> ����Ȯ�� %2.2f%% ����"
	#define STRMSG_S_F2NOTIFY_0008 "%s->%s ����, Ȯ��(%d>%5.2f)"
	#define STRMSG_S_F2NOTIFY_0009 "%s->%s ����, Ȯ��(%d>%5.2f) -%5.2f"
	#define STRMSG_S_F2NOTIFY_0010 "%s->%s ����, Ȯ��(%d>%5.2f)"
	#define STRMSG_S_F2NOTIFY_0011 "1-2��: %4.1f vs %4.1f -> ������%2.2f%%����(%4.1f->%4.1f)"
	#define STRMSG_S_F2NOTIFY_0012 "%s->%s���� %5.2f(%5.2f-%d/255) ��(%d<=%5.2f)"
	#define STRMSG_S_F2NOTIFY_0013 "%s->%s�κ��� %5.2f(%5.2f-%d/255) ����(%d<=%5.2f)"
	#define STRMSG_S_F2NOTIFY_0014 "%s->%s(%d, HP:%5.2f)���� %5.2f(%5.2f-%d/255) ��(%d<=%5.2f)"
	#define STRMSG_S_F2NOTIFY_0015 "%s->%s�κ��� %5.2f(%5.2f-%d/255) ����(%d<=%5.2f)"
	#define STRMSG_S_F2NOTIFY_0016 "���� ����: Ȯ�� ���� > %d"
	#define STRMSG_S_F2NOTIFY_0017 "���� ����[%#08x]: %5.2f(%5.2f-%5.2f)"
	#define STRMSG_S_F2NOTIFY_0018 "�Ѿ������� �����ϴ�. �����ڿ��� �����ϼ���."
	#define STRMSG_S_F2NOTIFY_0019 "�Ѿ��� ���� ������ �������� �ʽ��ϴ�. %s %d"
	#define STRMSG_S_F2NOTIFY_0020 "�Ѿ��� �����Դϴ�. %s %d"
	#define STRMSG_S_F2NOTIFY_0021 "�̹� �̺�Ʈ ó�����̹Ƿ� ������ �Ұ����մϴ�"
	#define STRMSG_S_F2NOTIFY_0022 "�׾����Ƿ� ������ �Ұ����մϴ�"
	#define STRMSG_S_F2NOTIFY_0023 "����� ���̹Ƿ� ������ �Ұ����մϴ�"
	#define STRMSG_S_F2NOTIFY_0024 "�� ���� ����! �����ڿ��� ����!"
	#define STRMSG_S_F2NOTIFY_0025 "�� ���� ����! �����ڿ��� ����! %d�� %d�� ���� Ÿ�� �ε����� ����!\r\n"
	#define STRMSG_S_F2NOTIFY_0026 "��Ƽ���� ������ �ʾ����Ƿ� ������ �� �����ϴ�"
	#define STRMSG_S_F2NOTIFY_0027 "  WARP(%04d) ó�� �Ұ� at Process_FC_CHARACTER_DEAD_GAMESTART(), %s\r\n"
	#define STRMSG_S_F2NOTIFY_0028 "  1 -> ĳ���� %10s���� ������ %5.2f�� �޾���"
	#define STRMSG_S_F2NOTIFY_0029 "1 -> ĳ���� %10s���� ������ %5.2f�� �־���"
	#define STRMSG_S_F2NOTIFY_0030 "1 -> ĳ���� %10s���� ������ %5.2f�� �־���"
	#define STRMSG_S_F2NOTIFY_0031 "1 -> ĳ���� %10s���� ���� ������ %5.2f�� �־���"
	#define STRMSG_S_F2NOTIFY_0032 "  2 -> ĳ���� %10s���� ������ %5.2f�� �޾���"
	#define STRMSG_S_F2NOTIFY_0033 "2 -> ĳ���� %10s���� ������ %5.2f�� �־���"
	#define STRMSG_S_F2NOTIFY_0034 "2 -> ���� %3d���� ������ %5.2f(%d)�� �־���"
	#define STRMSG_S_F2NOTIFY_0035 "2 -> ĳ���� %10s���� ������ %5.2f�� �־���"
	#define STRMSG_S_F2NOTIFY_0036 "2 -> ĳ���� %10s���� ���� ������ %5.2f�� �־���"
	#define STRMSG_S_F2NOTIFY_0037 "  2 -> ���� %3d���� ������ %5.2f�� �޾���"
	#define STRMSG_S_F2NOTIFY_0038 "  MINE -> ĳ���� %10s���� ������ %5.2f�� �־���"
	#define STRMSG_S_F2NOTIFY_0039 "  MINE -> ĳ���� %10s���� ���� ������ %5.2f�� �־���"
	#define STRMSG_S_F2NOTIFY_0040 "  MINE -> ���� %10s���� ������ %5.2f�� �־���"
	#define STRMSG_S_F2NOTIFY_0041 "���� �ʱ�ȭ �����Ͽ����ϴ�."
	#define STRMSG_S_F2NOTIFY_0042 "�׾��� ���� ����� �� �����ϴ�"
	#define STRMSG_S_F2NOTIFY_0043 "ENCHANT_INFO ����"
	#define STRMSG_S_F2NOTIFY_0044 "��æƮ ��ÿ� �����Ͽ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0045 "��æƮ ��ÿ� �����Ͽ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0046 "���ܿ� �Ҽӵ��� �ʾҽ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0047 "�̹� ��û ������Դϴ�."
	#define STRMSG_S_F2NOTIFY_0048 "��� �� ��õ� �ٶ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0049 "�ڽ� Ȥ�� ��밡 �������� �ƴմϴ�."
	#define STRMSG_S_F2NOTIFY_0050 "�̹� �������� ���Դϴ�."
	#define STRMSG_S_F2NOTIFY_0051 "�� �������� ���� �ʿ� �������� �ʽ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0052 "��� ��Ƽ������ ���� �ʿ� ���� �ʽ��ϴ�"
	#define STRMSG_S_F2NOTIFY_0053 "��� ��Ƽ������ ���� �ʿ� ���� �ʽ��ϴ�"
	#define STRMSG_S_F2NOTIFY_0054 "��忡 �Ҽӵ��� �ʾҽ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0055 "��û�� �ǽ�û�ڰ� �ٸ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0056 "�ڽ� Ȥ�� ��밡 �������� �ƴմϴ�."
	#define STRMSG_S_F2NOTIFY_0057 "�� �������� ���� �ʿ� �������� �ʽ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0058 "��忡 �Ҽӵ��� �ʾҽ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0059 "��û�� �ǽ�û�ڰ� �ٸ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0060 "�ڽ� Ȥ�� ��밡 �������� �ƴմϴ�."
	#define STRMSG_S_F2NOTIFY_0061 "���������� �� �����Ͽ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0062 "�̼� ���� �����Դϴ�. ���������� �����ϼ���."
	#define STRMSG_S_F2NOTIFY_0063 "�� ��ġ���� �ش� ������(%s)�� ������ �� �����ϴ�"
	#define STRMSG_S_F2NOTIFY_0064 "������ ����� �� �����ϴ�."
	#define STRMSG_S_F2NOTIFY_0065 "������ �̵�: (%I64d, %d) -> (%I64d, %d)"
	#define STRMSG_S_F2NOTIFY_0066 "�߸��� ������ �̵�: (%I64d, %d) -> (%I64d, %d)"
	#define STRMSG_S_F2NOTIFY_0067 "  EVENT(%d) ó�� �Ұ� at Process_FC_EVENT_REQUEST_WARP(), %s\r\n"
	#define STRMSG_S_F2NOTIFY_0068 "�� �̺�Ʈ ���� �̻�!!! �����ڿ��� ����!!!"
	#define STRMSG_S_F2NOTIFY_0069 "�ش� ���� Ÿ���� ����"
	#define STRMSG_S_F2NOTIFY_0070 "������ ���� �����Դϴ�. �����ڿ��� �������ּ���."
	#define STRMSG_S_F2NOTIFY_0071 "�����Ϸ��� ��ų ������ ���� ��ų�� �������� ���ų� �����ϴ�."
	#define STRMSG_S_F2NOTIFY_0072 "��ų ���� �ʿ� ������ �����Դϴ�."
	#define STRMSG_S_F2NOTIFY_0073 "������ �ȱ� �����Դϴ�. �����ڿ��� �������ּ���."
	#define STRMSG_S_F2NOTIFY_0074 "������ ���� �����Դϴ�. �����ڿ��� �������ּ���."
	#define STRMSG_S_F2NOTIFY_0075 "\'���� ������\'�� �����Ͽ����ϴ�."
	#define STRMSG_S_F2NOTIFY_0076 "  ���Ÿ�� : \'%s(%d��)\'"
	#define STRMSG_S_F2NOTIFY_0077 "��Ƽ�� �̹� �����Ͽ����ϴ�."
	#define STRMSG_S_F2NOTIFY_0078 "����� ��Ƽ���ε� ��� �����մϱ�?"
	#define STRMSG_S_F2NOTIFY_0079 "������� �������̹Ƿ� ������ �� �����ϴ�."
	#define STRMSG_S_F2NOTIFY_0080 "��� ��Ƽ�� ���� ��������� �������� ����!"
	#define STRMSG_S_F2NOTIFY_0081 "�� �̺�Ʈ ���� �̻�!!! �����ڿ��� ����!!!"
	#define STRMSG_S_F2NOTIFY_0082 "�ŷ��� �� �� ���� �����Դϴ�"
	#define STRMSG_S_F2NOTIFY_0083 "%s���� ���緮 �ʰ��Դϴ�."
	#define STRMSG_S_F2NOTIFY_0084 "�̵� ���� ��ǥ: (5, 5) -> (%d, %d)"
	#define STRMSG_S_F2NOTIFY_0085 "�����: "
	#define STRMSG_S_F2NOTIFY_0086 "�ִ� 20������� �� �� �ֽ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0087 "�ش� �����(%s)�� �����ϴ�"
	#define STRMSG_S_F2NOTIFY_0088 "��Ƽ�� �Ҽӵ��� �ʾ���"
	#define STRMSG_S_F2NOTIFY_0089 "���� �ð�: %d:%d, ���� �ð�: %d:%d"
	#define STRMSG_S_F2NOTIFY_0090 "���� �ð�: %d:%d, ���� �ð�: %d:%d"
	#define STRMSG_S_F2NOTIFY_0091 "NPC ������ ������ ���� �ʾҽ��ϴ�"
	#define STRMSG_S_F2NOTIFY_0092 "�ش� ������(%d)�� �������� �ʽ��ϴ�"
	#define STRMSG_S_F2NOTIFY_0093 "Countable Item�� �ƴ� ��� 10���� ���ѵ˴ϴ�"
	#define STRMSG_S_F2NOTIFY_0094 "�� \'%s\'�� ��ü ������: %d��"
	#define STRMSG_S_F2NOTIFY_0095 "�� %s %s ����: %d�� - '%s'(*)"
	#define STRMSG_S_F2NOTIFY_0096 "�� %s %s ����: %d�� - '%s'"
	#define STRMSG_S_F2NOTIFY_0097 "���� ��ä��: %s, %d(%d)"
	#define STRMSG_S_F2NOTIFY_0098 "�Ѿ������� �����ϴ�. �����ڿ��� �����ϼ���."
	#define STRMSG_S_F2NOTIFY_0099 "�Ѿ��� ���� ������ �������� �ʽ��ϴ�. %s %d"
	#define STRMSG_S_F2NOTIFY_0100 "�Ѿ������� �����ϴ�. �����ڿ��� �����ϼ���."
	#define STRMSG_S_F2NOTIFY_0101 "�Ѿ��� ���� ������ �������� �ʽ��ϴ�. %s %d"
	#define STRMSG_S_F2NOTIFY_0102 "�ش� �����(%s)�� �����ϴ�"
	#define STRMSG_S_F2NOTIFY_0103 "�ش� �����(%s)�� ���� �����Դϴ�"
	#define STRMSG_S_F2NOTIFY_0104 "�ش� �����(%s)�� �����ϴ�"
	#define STRMSG_S_F2NOTIFY_0105 "�Ϲ� �������� �����Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0106 "�Ϲ� ������ �����Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0107 "����(�������� ���� �ʴ�) �������� �����Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0108 "����(�������� ���� �ʴ�) ���� ������ �����Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0109 "���Ⱑ %5.0f%% ��⼺�� ��� �Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0110 "������ �������� �ʱ�ȭ�˴ϴ�"
	#define STRMSG_S_F2NOTIFY_0111 "������ �������� �ʱ�ȭ�˴ϴ�"
	#define STRMSG_S_F2NOTIFY_0112 "�ش� �����(%s)�� �����ϴ�"
	#define STRMSG_S_F2NOTIFY_0113 "���� ����� �����Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0114 "���� ����� Ȱ��ȭ�Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0115 "%s �̺�Ʈ�� ���� ���� �ƴմϴ�"
	#define STRMSG_S_F2NOTIFY_0116 "%s �̺�Ʈ�� ���۵�(����:%4.2f, �̺�Ʈ�ð�:%3d��)"
	#define STRMSG_S_F2NOTIFY_0117 "�Ϲ������̾� �������� �����Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0118 "�����̾� ���� ������ �����Ͽ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0119 "���������̾� �������� �����Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0120 "���������� ���� �ƴմϴ�"
	#define STRMSG_S_F2NOTIFY_0121 "������������ ���� �� �� �����ϴ�"
	#define STRMSG_S_F2NOTIFY_0122 "������������ ���� �� �� �����ϴ�"
	#define STRMSG_S_F2NOTIFY_0123 "���ڽ� ���°� �ʱ�ȭ �Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0124 "���ڽ� ���°� �Ǿ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0125 "���� ��ȿ���� �ʽ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0126 "���Ǿƿ��̺�Ʈ ������ [����ð�:%4d��)]"
	#define STRMSG_S_F2NOTIFY_0127 "���Ǿƿ��̺�Ʈ �����"
	#define STRMSG_S_F2NOTIFY_0128 "  1 -> ���� %3d���� ������ %5.2f�� �޾���"
	#define STRMSG_S_F2NOTIFY_0129 "  1�� -> ���� %3d���� ������ %5.2f�� �޾���(����)"
	#define STRMSG_S_F2NOTIFY_0130 "%s �������� �����Ͽ����ϴ�"
	#define STRMSG_S_F2NOTIFY_0131 "  ���� ��ƾ�� ó������ ���� %s: CS(%d), DBStore(%d)\r\n"
	#define STRMSG_S_F2NOTIFY_0132 "  ERROR@CharacterGameEndRoutine(): ��Ƽ�� ���� ����! %s\r\n"
// 2005-11-24 by cmkwon, 
//	#define STRMSG_S_F2NOTIFY_0133 "Prefix \'%s\' ����: %d <= %d <= %d\r\n"
//	#define STRMSG_S_F2NOTIFY_0134 "Suffix \'%s\' ����: %d <= %d <= %d\r\n"
	#define STRMSG_S_F2NOTIFY_0135 "%s: ������!"
	#define STRMSG_S_F2NOTIFY_0136 "FATAL ERROR: �����ڿ� �����ϼ���! ���� ���� �Ҵ� ����!"
	#define STRMSG_S_F2NOTIFY_0137 "%s��(��) �����Ǿ����ϴ�."
	#define STRMSG_S_F2NOTIFY_0138 "���� ��� ä���� ��Ȱ��ȭ �Ǿ� ���� �� ��Ÿ �����"
	#define STRMSG_S_F2NOTIFY_0139 "�����˴ϴ�. �ٸ� ä���� �̿��Ͽ� �ֽñ� �ٶ��ϴ�."
	#define STRMSG_S_F2NOTIFY_0140 "  Warp Obj.[%s,%2d��]: %04d[%1s%4d]"
	#define STRMSG_S_F2NOTIFY_0141 "Ÿ�̸� ���� %s\r\n"
	#define STRMSG_S_F2NOTIFY_0142 "HP �ڵ� ȸ�� �����մϴ�"
	#define STRMSG_S_F2NOTIFY_0143 "���� ���� ������ �ƴϹǷ� HP �ڵ� ȸ�� �����մϴ�"
	#define STRMSG_S_F2NOTIFY_0144 "���޿� ���� HP 5.2f ȸ��"
	#define STRMSG_S_F2NOTIFY_0145 "Gradual HP UP ������(���� �ð�: %d)"
	#define STRMSG_S_F2NOTIFY_0146 "Gradual DP UP ������(���� �ð�: %d)"
	#define STRMSG_S_F2NOTIFY_0147 "Gradual EP UP ������(���� �ð�: %d)"
	#define STRMSG_S_F2NOTIFY_0148 "%s �̺�Ʈ�� ���� �Ǿ����ϴ�."
	#define STRMSG_S_F2NOTIFY_0149 "%s �̺�Ʈ ���� �� (����:%4.2f, �����ð�:%3d��)"
	#define STRMSG_S_F2NOTIFY_0150 "��ɾ ������� �ʾҽ��ϴ�"


	// 5-5 Field - NOTIFY Error
	#define STRERR_S_F2NOTIFY_0000 "	����ġ �й� ����(%s, %s(%d)): fTotalDamage(%d) < 0.0f or Empty Vector: %d, Distance(%5.1f)\n"
	#define STRERR_S_F2NOTIFY_0001 "  ��ȿ���� ���� Game Start �޼��� ���� ClientState[%d]\r\n"
	#define STRERR_S_F2NOTIFY_0002 "  ��ȿ���� ���� Game Start �޼��� ���� ClientState[%d]\r\n"
	#define STRERR_S_F2NOTIFY_0003 "  WARP(%04d) ó�� �Ұ� at ProcessQuestResult(), %s\r\n"
	#define STRERR_S_F2NOTIFY_0004 "�� �̺�Ʈ ���� �̻�!!! �����ڿ��� ����!!! Current(%s, %s, %04d), Target(%04d, %d)\r\n"
	#define STRERR_S_F2NOTIFY_0005 "  EVENT(%d) ó�� �Ұ� at T_FC_PARTY_REQUEST_PARTY_WARP(), %s\r\n"
	#define STRERR_S_F2NOTIFY_0006 "�� �̺�Ʈ ���� �̻�!!! �����ڿ��� ����!!! Current(%s, %s, %04d), Target(%04d, %d)\r\n"
	#define STRERR_S_F2NOTIFY_0007 "  WARP ó�� �Ұ� at T_FC_PARTY_REQUEST_PARTY_WARP(). %s\r\n"
	#define STRERR_S_F2NOTIFY_0008 "  EVENT(%d) ó�� �Ұ�(��Ƽ��) at Process_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME(). %s\r\n"
	#define STRERR_S_F2NOTIFY_0009 "  WARP(%04d) ó�� �Ұ� at Process_FC_PARTY_REQUEST_PARTY_WARP_WITH_MAP_NAME(). %s\r\n"
	#define STRERR_S_F2NOTIFY_0010 "  EVENT(%d) ó�� �Ұ� at Process_FC_EVENT_REQUEST_WARP(), %s\r\n"
	#define STRERR_S_F2NOTIFY_0011 "  WARP ó�� �Ұ� at T_FC_PARTY_REQUEST_PARTY_WARP(). %s\r\n"
	#define STRERR_S_F2NOTIFY_0012 "��Ƽ ���� ����: %s -> �Ÿ�: %5.2f, bodycon: %d, %d"
	#define STRERR_S_F2NOTIFY_0013 "  WARP(%04d) ó�� �Ұ� at Process_FC_CHARACTER_DEAD_GAMESTART(), %s\r\n"
	#define STRERR_S_F2NOTIFY_0014 "  %s -> �ߺ��� OK ��ư Ŭ��!!!\r\n"
	#define STRERR_S_F2NOTIFY_0015 "����Ʈ�� �ε����� �ʾҽ��ϴ�.\r\n"
	#define STRERR_S_F2NOTIFY_0016 "  WARP(%s) ó�� �Ұ� at HandleAdminCommands(), /�̵�, %s\r\n"
	#define STRERR_S_F2NOTIFY_0017 "  WARP(%s) ó�� �Ұ� at HandleAdminCommands(), /������, %s\r\n"
	#define STRERR_S_F2NOTIFY_0018 "  FATAL ERROR: ����� ������ ���� �ʵ� �������� �� �޼����� ���� �� �ȴ�. check it out\r\n"
	#define STRERR_S_F2NOTIFY_0019 "  WARP(%04d) ó�� �Ұ� at T_FI_ADMIN_CALL_CHARACTER, %s\r\n"
	#define STRERR_S_F2NOTIFY_0020 "  WARP(%s) ó�� �Ұ� at T_FI_ADMIN_MOVETO_CHARACTER, %s\r\n"

	// 5-6 Field - Event
	#define STRMSG_S_F2EVENTTYPE_0000 "����ġ"
	#define STRMSG_S_F2EVENTTYPE_0001 "����(SPI)"
	#define STRMSG_S_F2EVENTTYPE_0002 "����ġ����"
	#define STRMSG_S_F2EVENTTYPE_0003 "�����۵��"
	#define STRMSG_S_F2EVENTTYPE_0004 "��������۵��"
	#define STRMSG_S_F2EVENTTYPE_0005 "�̺�Ʈ��"
// 5_end	
///////////////////////////////////////////////////////////////////////////////
	

///////////////////////////////////////////////////////////////////////////////
// 6 - IMServer
	// 6-1 IM<->Pre
	#define STRMSG_S_I2PRECONNECT_0000 "Pre Server�� ���� �Ͽ����ϴ�.\r\n"
	#define STRMSG_S_I2PRECONNECT_0001 "Pre Server[%15s:%4d]���� ������ ���� �Ǿ����ϴ�. �������� �õ��մϴ�.\r\n"

	// 6-2 IM Notify
	#define STRMSG_S_I2NOTIFY_0000 "\'%s\'��(��) �̹� �����ϴ� ���� �̸��Դϴ�"
	#define STRMSG_S_I2NOTIFY_0001 "\'%s\'��(��) �̹� ���ܿ� ���ԵǾ� �ֽ��ϴ�."
	#define STRMSG_S_I2NOTIFY_0002 "\'%s\'��(��) �̹� �����ϴ� ���� �̸��Դϴ�"
	#define STRMSG_S_I2NOTIFY_0003 "���� �ε� ���� - �����ڿ��� �������ּ���!"
	#define STRMSG_S_I2NOTIFY_0004 "ģ�� '%s'���� �����Ͽ����ϴ�"
	#define STRMSG_S_I2NOTIFY_0005 "������ ��Ƽ�� �˻��մϴ�"
	#define STRMSG_S_I2NOTIFY_0006 "������ ��Ƽ�� �������� �ʽ��ϴ�"
	#define STRMSG_S_I2NOTIFY_0007 "��Ƽ�� �簡�ԵǾ����ϴ�"
	#define STRMSG_S_I2NOTIFY_0008 "�̹� ������ ��Ƽ�� �ֽ��ϴ�."
	#define STRMSG_S_I2NOTIFY_0009 "  Fatal Error: �ɰ��� ��Ƽ ����! at T_IC_PARTY_GET_MEMBER"
	#define STRMSG_S_I2NOTIFY_0010 "��Ƽ���� ���°� �� �� �Ǿ���"
	#define STRMSG_S_I2NOTIFY_0011 "��Ƽ���� �̹� ��忡 �����Ͽ���"
	#define STRMSG_S_I2NOTIFY_0012 "����� �̹� ��忡 �����Ͽ����ϴ�"
	#define STRMSG_S_I2NOTIFY_0013 "�ڱ� �ڽ��� �ʴ����� ���մϴ�"
	#define STRMSG_S_I2NOTIFY_0014 "��� ������ ������ �ʴ� �����Դϴ�"
	#define STRMSG_S_I2NOTIFY_0015 "���� ������ ���ܿ� ���� �ʰ��Ͽ����ϴ�"
	#define STRMSG_S_I2NOTIFY_0016 "��� ������ ������ �ʴ� �����Դϴ�"
	#define STRMSG_S_I2NOTIFY_0017 "�������� Ż���� �� �����ϴ�"
	#define STRMSG_S_I2NOTIFY_0018 "����� ���̹Ƿ� ��忡�� Ż���� �� �����ϴ�"
	#define STRMSG_S_I2NOTIFY_0019 "����� ���̹Ƿ� ������ �߹��� �� �����ϴ�"
	#define STRMSG_S_I2NOTIFY_0020 "�������� �߹�� �� �����ϴ�"
	#define STRMSG_S_I2NOTIFY_0021 "����� ���̹Ƿ� ��带 �ػ��� �� �����ϴ�"
	#define STRMSG_S_I2NOTIFY_0022 "���� �̸����δ� �������� ���մϴ�"
	#define STRMSG_S_I2NOTIFY_0023 "��� �̸��� ������ �� ���� �����Դϴ�"
	#define STRMSG_S_I2NOTIFY_0024 "��� ��ũ�� ������ �� ���� �����Դϴ�"
	#define STRMSG_S_I2NOTIFY_0025 "����� ������ �� ���� �����Դϴ�"
	#define STRMSG_S_I2NOTIFY_0026 "����� �ߺ��Ǿ����ϴ�"
	#define STRMSG_S_I2NOTIFY_0027 "������������ ��� ������ �Ұ����մϴ�."
	#define STRMSG_S_I2NOTIFY_0028 "�������� ��� ������ �Ұ����մϴ�."
	#define STRMSG_S_I2NOTIFY_0029 "��ü ����� ��: %d�� (%d����� ���)"
	#define STRMSG_S_I2NOTIFY_0030 "��� �޼��� ����Ʈ�� �߰��Ǿ����ϴ�"
	#define STRMSG_S_I2NOTIFY_0031 "��� �޼��� ����Ʈ���� ���ŵǾ����ϴ�"
	#define STRMSG_S_I2NOTIFY_0032 "���� IP: %s"
	#define STRMSG_S_I2NOTIFY_0033 "���� �׷� \'%s\'�� ��ü ������: %d��"
	#define STRMSG_S_I2NOTIFY_0034 "���� ����(%s)�� �����Ͻðھ��? ����: %d"
	#define STRMSG_S_I2NOTIFY_0035 "\'%s\'�� ������ �ϰ� ���� �ʽ��ϴ� "
	#define STRMSG_S_I2NOTIFY_0036 "��ü ����� ��: %d�� (%d����� ���)"
	#define STRMSG_S_I2NOTIFY_0037 "/������ %s %s"
	#define STRMSG_S_I2NOTIFY_0038 "�ش� ������ �����ϴ�."
	#define STRMSG_S_I2NOTIFY_0039 "�ӼӸ� ������ ���� �Ǿ����ϴ�!"
	#define STRMSG_S_I2NOTIFY_0040 "�ӼӸ��� ���� �Ǿ����ϴ�!"
	#define STRMSG_S_I2NOTIFY_0041 "��忡 �������� �ʾҽ��ϴ�"
	#define STRMSG_S_I2NOTIFY_0042 "���(%d)�� ��ȿ���� �ʽ��ϴ�"
	#define STRMSG_S_I2NOTIFY_0043 "�ش� ����(%s)�� �������� �ʽ��ϴ�"
	#define STRMSG_S_I2NOTIFY_0044 "Field Server�� ��ȿ���� �ʽ��ϴ�"
	#define STRMSG_S_I2NOTIFY_0045 "�ش� ��(%s)�� �������� �ʽ��ϴ�"
	#define STRMSG_S_I2NOTIFY_0046 "����� %d�� ���� ä�ñ��� �����Դϴ�!!"
	#define STRMSG_S_I2NOTIFY_0047 "ä�ñ��� ���� : '%10s', %3d��"
	#define STRMSG_S_I2NOTIFY_0048 "ä�ñ��� ���°� ���� �Ǿ����ϴ�"
	#define STRMSG_S_I2NOTIFY_0049 "ä�ñ��� ���� : '%10s'"
	#define STRMSG_S_I2NOTIFY_0050 "%s(����: %s, ��: %d(%d), ����: %d) PLAYING"
	#define STRMSG_S_I2NOTIFY_0051 "�ڱ� �ڽ��� �θ� �� �����ϴ�"
	#define STRMSG_S_I2NOTIFY_0052 "�ش� ��尡 �����ϴ�."

	#define STRMSG_S_IMSERVER_050607_0001	"�ش� ���� �����ϴ�."
// 6_end	
///////////////////////////////////////////////////////////////////////////////
	

///////////////////////////////////////////////////////////////////////////////
// 7 - NPCServer
	// 7-1 NPC<->Field
	#define STRMSG_S_N2FIELDCONNECT_0000 "Field Server�� ���� �Ͽ����ϴ�.\n"
	#define STRMSG_S_N2FIELDCONNECT_0001 "Field Server[%15s:%4d]���� ������ ���� �Ǿ����ϴ�. �������� �õ��մϴ�.\r\n"

	// 7-2 IM Notify
	#define STRMSG_S_N2NOTIFY_0000 "							���Ͱ� ������Ʈ�� �浹 üũ ����\r\n"
	#define STRMSG_S_N2NOTIFY_0001 "���������� ����(%10s) ��ȯ��\r\n\r\n"

	#define STRMSG_S_N2TESTMONNAME_0000 "ö��ȣ"
// 7_end	
///////////////////////////////////////////////////////////////////////////////
	

///////////////////////////////////////////////////////////////////////////////
// 8 - PreServer
	// 8-1 Pre Notify
	#define STRMSG_S_P2PRENOTIFY_0000 "%s ����(%s)���� �����Ͽ����ϴ�. IP: %s\r\n"
	#define STRMSG_S_P2PRENOTIFY_0001 "����"
	#define STRMSG_S_P2PRENOTIFY_0002 "����"
	#define STRMSG_S_P2PRENOTIFY_0003 "[ERROR] ���������߰� ����, AccountName(%s)  privateIP(%15s)\n"


	#define STRMSG_SCAT_051115_0001		"�ȳ��ϼ���. ���̽��¶��� ����Դϴ�."
	#define STRMSG_SCAT_051115_0002		"���� 6�ú��� 10�ñ��� ���̽��¶��� ���� ������ �ǽ��մϴ�."
	#define STRMSG_SCAT_051115_0003		"������ ��ҷ� �̵� �� ������ ������ �ֽñ� �ٶ��ϴ�."
	#define STRMSG_SCAT_051115_0004		"���� NPC �������� ���� �ϰڽ��ϴ�."
	#define STRMSG_SCAT_051115_0005		"���ݺ��� ���̽��¶��� ���� ������ �����ϰڽ��ϴ�."
	#define STRMSG_SCAT_051115_0006		"���̽��¶��ΰ� �Բ� ��ſ� �Ϸ� �����ñ� �ٶ��ϴ�."
	#define STRMSG_SCAT_051115_0007		"��� �� ��� ������ ���� �����ٿ��� ���� �����Դϴ�."
	#define STRMSG_SCAT_051115_0008		"���� �ð��� 60���Դϴ�."


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

	// 2006-05-09 by cmkwon
	#define STRMSG_060509_0000			"�������: ������ �� �� ����, ��� ���� ��� �Ұ�"
	#define STRMSG_060509_0001			"��������: �������� ���� ����"
	#define STRMSG_060509_0002			"���ڽ����: ���� ���ͷκ��� ���� ������ ���� ����"

	// 2006-05-26 by cmkwon
	#define	STRMSG_060526_0000			"Your request has been sent to a GM. One of our GMs will contact you momentarily."
	#define	STRMSG_060526_0001			"�ڵ� ������Ʈ�� ������ �� �����ϴ�.\r\n\r\nȨ������(%s) �ٿ�ε� ���������� ������ġ�� �޾� ��ġ�� �� �ٽ� �����Ͻñ� �ٶ��ϴ�.\r\n\r\n    Error: %s"

	// 2006-08-24 by cmkwon
	#define STRMSG_060824_0000			"���� �Է��Ͻ� ���̵� ��ϵǾ� ���� �ʰų�,\r\n ���̵� �Ǵ� �н����带 �߸� �Է� �ϼ̽��ϴ�."
	
	// 2006-09-27 by cmkwon
	#define STRMSG_060927_0000			"���� ���� ���� ���Դϴ�. �ڼ��� ������ Ȩ�������� �����ϼ���."

	// 2006-10-11 by cmkwon
	#define STRERR_061011_0000			"Ŭ���̾�Ʈ ������ ���� �ʽ��ϴ�.\r\n  Ȩ���������� ������ġ ������ �޾� �ٽ� ��ġ�ϰų�, Ŭ���̾�Ʈ�� �ٽ� ��ġ�� �ּ���."

	// 2006-11-07 by cmkwon
	#define STRMSG_061107_0000			"%s�� ���� ���ߴ��߽��ϴ�."			  

	// 2006-11-07 by cmkwon
	#define STRMSG_070410_0000			"��ȸ���� DB(atum2_db_20) �ʱ�ȭ�� ��ȸ�������� ���� �Ŀ� �ؾ��մϴ�."
	#define STRMSG_070410_0001			"������ ��ȸ���� DB(atum2_db_20)�� �ʱ�ȭ �Ͻðڽ��ϱ�? [��������:%d]"
	#define STRMSG_070410_0002			"��ȸ���� DB(atum2_db_20)�� �ʱ�ȭ �Ǿ����ϴ�."
	#define STRMSG_070410_0003			"��ȸ���� DB(atum2_db_20) �ʱ�ȭ ���� !!"
	#define STRMSG_070410_0004			"��ȸ���� DB(atum2_db_20)�� '%s'�� ���� ���� �Ϸ�"
	#define STRMSG_070410_0005			"��ȸ���� DB(atum2_db_20)�� '%s'�� ���� ���� ���� - ĳ���Ͱ� �������� ���� !!"
	#define STRMSG_070410_0006			"��ȸ���� DB(atum2_db_20)�� '%s'�� ���� ���� ���� - ��ȸ������ �ش� ������ ĳ���Ͱ� ���� !!"
	#define STRMSG_070410_0007			"��ȸ���� DB(atum2_db_20)�� '%s'�� ���� ���� ���� - DB �߰� ������ !!"
	#define STRMSG_070410_0008			"��ȸ���� DB(atum2_db_20)�� '%s'�� ���� ���� ���� - �˼� ����(%d)!!"

///////////////////////////////////////////////////////////////////////////////
// 2007-05-07 by cmkwon, �ػ� ���ڿ� 
	// 2007-07-24 by cmkwon, ��ó���� 800*600 �ػ� ���� - �޺��ڽ� ��Ʈ�� �ʿ� ����
	//#define STRMSG_WINDOW_DEGREE_800x600_LOW			"800x600 (low)"
	//#define STRMSG_WINDOW_DEGREE_800x600_MEDIUM			"800x600 (medium)"
	//#define STRMSG_WINDOW_DEGREE_800x600_HIGH			"800x600 (high)"
#define STRMSG_WINDOW_DEGREE_1024x768_LOW			"1024x768 (low)"
#define STRMSG_WINDOW_DEGREE_1024x768_MEDIUM		"1024x768 (medium)"
#define STRMSG_WINDOW_DEGREE_1024x768_HIGH			"1024x768 (high)"
#define STRMSG_WINDOW_DEGREE_W1280x800_LOW			"1280x800 (low-wide)"
#define STRMSG_WINDOW_DEGREE_W1280x800_MEDIUM		"1280x800 (medium-wide)"
#define STRMSG_WINDOW_DEGREE_W1280x800_HIGH			"1280x800 (high-wide)"
#define STRMSG_WINDOW_DEGREE_1280x960_LOW			"1280x960 (low)"
#define STRMSG_WINDOW_DEGREE_1280x960_MEDIUM		"1280x960 (medium)"
#define STRMSG_WINDOW_DEGREE_1280x960_HIGH			"1280x960 (high)"
#define STRMSG_WINDOW_DEGREE_1280x1024_LOW			"1280x1024 (low)"
#define STRMSG_WINDOW_DEGREE_1280x1024_MEDIUM		"1280x1024 (medium)"
#define STRMSG_WINDOW_DEGREE_1280x1024_HIGH			"1280x1024 (high)"
#define STRMSG_WINDOW_DEGREE_W1600x900_LOW			"1600x900 (low-wide)"
#define STRMSG_WINDOW_DEGREE_W1600x900_MEDIUM		"1600x900 (medium-wide)"
#define STRMSG_WINDOW_DEGREE_W1600x900_HIGH			"1600x900 (high-wide)"
#define STRMSG_WINDOW_DEGREE_1600x1200_LOW			"1600x1200 (low)"
#define STRMSG_WINDOW_DEGREE_1600x1200_MEDIUM		"1600x1200 (medium)"
#define STRMSG_WINDOW_DEGREE_1600x1200_HIGH			"1600x1200 (high)"

// 2007-06-15 by dhjin, ���� ���� ��Ʈ��
#define STRMSG_070615_0000				"���� ���°� �ƴϾ ���� ������ �� �� �����ϴ�."
#define STRMSG_070615_0001				"���������� �� �� �����ϴ�."
#define STRMSG_070620_0000				"�Ϲ� ������ ���� ������ �� �� �����ϴ�."

// 2007-06-26 by dhjin, ������Ʈ �̺�Ʈ ���� �߰�
#define STRMSG_S_F2EVENTTYPE_0006		"��������Ʈ"

// 2007-06-28 by cmkwon, �߱� ��������(���� �ð� �˸� ����) - ��Ʈ�� �߰�
#define STRMSG_070628_0000				"����� ���� ���ӽð��� �̹� %d�ð��� �Ǿ����ϴ�."
#define STRMSG_070628_0001				"����� ���� ���ӽð��� �̹� %d�ð��� �Ǿ����ϴ�. ���������Ͻð� �޽ĵ� �Ͻð� ������ ��� �ϼ���."
#define STRMSG_070628_0002				"����� �̹� ������ ���ӽð��� �����ϴ�. ����� ���Ӽ����� 50%%���� �پ��ϴ�. ����� �ǰ��� ���Ͽ� ���� �����Ͻð� ������ ��� �Ͻð� �ո��ϰ� �н���Ȱ�� �ȹ��ϼ���"
#define STRMSG_070628_0003				"����� �̹� �Ұ����� ���ӽð��� �����ϴ�. ����� �ǰ��� ���Ͽ� ��÷� ���� �����ϼ���. �׷��� ������ ����� ��ü�� �طο� �Ӵ��� ����� ���Ӽ����� 0%%���� �پ��ϴ�. ���� ���� �� 5�ð��Ŀ� �ٽ� ������ �������� ȸ���˴ϴ�."

///////////////////////////////////////////////////////////////////////////////
// 2007-07-11 by cmkwon, �Ʒ����� �ý��� ���� - �߰��� ��Ʈ��
#define STRMSG_070711_0000				"\'%s\'���� �Ʒ��� �̿� ���� ���°� �ƴմϴ�."
#define STRMSG_070711_0001				"\'%s\'���� �Ʒ��� �̿� ���� ���� �Դϴ�.(���� �ð�:%d��)"
#define STRMSG_070711_0002				"����� %d�� ���� �Ʒ��� �̿� ���� �����Դϴ�!!"
#define STRMSG_070711_0003				"����� �Ʒ��� �̿� ������ ���� �Ǿ����ϴ�."
#define STRMSG_070711_0004				"\'%s\'���� �Ʒ��� �̿� ������ ���� �Ǿ����ϴ�."

///////////////////////////////////////////////////////////////////////////////
// 2007-08-23 by cmkwon, Wide �ػ� 1280x720(16:9) �߰� - ��Ʈ�� �߰�
#define STRMSG_WINDOW_DEGREE_W1280x720_LOW			"1280x720 (low-wide)"
#define STRMSG_WINDOW_DEGREE_W1280x720_MEDIUM		"1280x720 (medium-wide)"
#define STRMSG_WINDOW_DEGREE_W1280x720_HIGH			"1280x720 (high-wide)"

// 2007-08-30 by cmkwon, ȸ�Ƿ� �ý��� ���� - ��Ʈ�� �߰�
#define STRMSG_070830_0001				"���¼��� �� ��� ������ ��ɾ� �Դϴ�."
#define STRMSG_070830_0002				"�ش� ������ ȸ�Ƿ��(%d)�� ��ȿ���� ����"
#define STRMSG_070830_0003				"���� ȸ�Ƿ뿡 ���� ������ �ο��� : %d��"
#define STRMSG_070830_0004				"'%s'���� �̹� ȸ�Ƿ� ���� ������ �ֽ��ϴ�."
#define STRMSG_070830_0005				"'%s'���� ȸ�Ƿ� ���� ������ �����ϴ�."
#define STRMSG_070830_0006				"'%s'��(��) ��ȿ�� ĳ���Ͱ� �ƴմϴ�."
#define STRMSG_070830_0007				"'%s'�� ȸ�Ƿ� ���� ���� ����Ʈ�� �߰� �� �� �����ϴ�.(�ִ� %d��)"
#define STRMSG_070830_0008				"'%s'�Կ��� ȸ�Ƿ� ���� ������ �ο��Ǿ����ϴ�."
#define STRMSG_070830_0009				"ȸ�Ƿ� ���� ������ �ο��Ǿ����ϴ�."
#define STRMSG_070830_0010				"ȸ�Ƿ� ���� ������ �����Ǿ����ϴ�."
#define STRMSG_070830_0011				"'%s'"

// 2007-11-13 by cmkwon, �����ϱ� ��� �߰� - 
#define STRMSG_071115_0001				"\\y%s�Բ� %s�������� ���� �Ͽ����ϴ�."
#define STRMSG_071115_0002				"���� ��� : \'%s(%d��)\'"
#define STRMSG_071115_0003				"\\y%s���� %s�Բ� %s�������� ���� �Ͽ����ϴ�. â�� Ȯ���ϼ���."

// 2007-11-19 by cmkwon, �����ý��� ������Ʈ - 
#define STRMSG_071120_0001				"���� �ý��� �̿�ð��� �ƴմϴ�. �� ���ͷ� ���� �ϼ���."
#define STRMSG_071120_0002				"���� �ý����� Ȱ��ȭ �Ǿ����ϴ�."
#define STRMSG_071120_0003				"���� �ý����� ���� �Ǿ����ϴ�."
#define STRMSG_071120_0004				"%s ���� %s ���� ���� �ý����� ���� �մϴ�."

// 2007-11-28 by cmkwon, �����ý��� ���� - 
#define STRMSG_071128_0001				"%s���� %s������ %d���� ���� �Ͽ����ϴ�. â���� Ȯ���ϼ���."

// 2007-12-27 by cmkwon, �������� ��� ��� �߰� - 
#define STRMSG_071228_0001				"�ػ� ������ ��ȿ���� �ʽ��ϴ�. �ٽ� Ȯ���ϼ���."

// 2008-01-31 by cmkwon, ���� ��/���� ��ɾ�� ������ �ý��� ���� - 
#define STRMSG_080201_0001				"'%s'�� �� ������ ������ �ֽ��ϴ�. ErrorCode(%d)"
#define STRMSG_080201_0002				"'%s'������ �� ó���Ͽ����ϴ�.[Block End Date: %s]"
#define STRMSG_080201_0003				"'%s'������ �� ����Ʈ�� �����ϴ�. ErrorCode(%d)"
#define STRMSG_080201_0004				"'%s'���� �� ���� ó���� ������ �ֽ��ϴ�. ErrorCode(%d)"
#define STRMSG_080201_0005				"'%s'������ �� ���� ó�� �Ͽ����ϴ�."

// 2008-02-11 by cmkwon, �ػ� �߰�(1440x900) - 
#define STRMSG_WINDOW_DEGREE_1440x900_LOW			"1440x900 (low-wide)"
#define STRMSG_WINDOW_DEGREE_1440x900_MEDIUM		"1440x900 (medium-wide)"
#define STRMSG_WINDOW_DEGREE_1440x900_HIGH			"1440x900 (high-wide)"

// 2007-12-27 by dhjin, �Ʒ��� ���� - �Ʒ��� ���� ���� ���� ����
#define STRMSG_S_MF2AFCONNECT_0000 "[Error] WndProc(), Can't connect to  ArenaServer[%15s:%4d] Reconnect\r\n"
#define STRMSG_S_MF2AFCONNECT_0001 "Arena Server�� ���� �Ͽ����ϴ�.\r\n"
#define STRMSG_S_MF2AFCONNECT_0002 "Arena Server[%15s:%4d]���� ������ ���� �Ǿ����ϴ�. �������� �õ��մϴ�.\r\n"
#define STRMSG_S_MF2AFCONNECT_0003 "  T_ERROR %s(%#04X) received from %s[%s]\r\n"
#define STRMSG_S_MF2AFCONNECT_0004 "Unknown Error@WM_FIELD_PACKET_NOTIFY: %s(%#04x)\n"
#define STRMSG_ARENAEVENT_080310_0001		"\\y%d�� �Ʒ��� ���濡 �̺�Ʈ �Ӽ��� �ο� �߽��ϴ�.\r\n"
#define STRMSG_ARENAEVENT_080310_0002		"\\y%d�� �Ʒ��� �濡 ���� �̺�Ʈ �Ӽ��� ���� �Ͽ����ϴ�.\r\n"
#define STRMSG_ARENAEVENT_080310_0003		"\\y�Ʒ��� �̺�Ʈ �Ӽ� �ο��� ���� �Ͽ����ϴ�.\r\n"
#define STRMSG_080428_0001					"%s���� ���� �Ͽ����ϴ�."		// 2008-04-28 by dhjin, �Ʒ��� ���� - ���� ���� �� ��Ʈ�� �߰�, �Ʒ��� �ʿ�����

// 2008-04-29 by cmkwon, ������ ���� DB�� �߰�(�ű� ���� ĳ���� ���� ���� �ý����߰�) - 
#define STRMSG_080430_0001					"�����Ͻ� ������ �ű� ĳ���� ������ ���ѵ� ���� �Դϴ�."

// 2008-06-13 by dhjin, EP3 ���� ���� ���� - 
#define STRMSG_080613_0001					"%s���ܿ� ���� ��û�� �źεǾ����ϴ�."

// 2008-09-04 by cmkwon, don't need translation, ������ SystemLog �̹Ƿ� ���� �ʿ� ����
#define STRMSG_080904_0001					 "[DB Error] No such DB query process(QP_xxx) function !! QueryType(%d:%s)\r\n"


// 2008-12-30 by cmkwon, ������ ä�� ���� ī�� ���� - 
#define STRMSG_081230_0001					"\\y%sĳ���� %d�� ä�� ������ ���� �մϴ�.\\y"
#define STRMSG_081230_0002					"\\y�����ڿ� ���� %d�� ���� ä�� ���� ���°� �Ǿ����ϴ�.\\y"
#define STRMSG_081230_0003					"\\y������ ä�� ������ ���� �Ǿ����ϴ�.\\y"

///////////////////////////////////////////////////////////////////////////////
// 2009-08-31 by cmkwon, Gameforge4D ���Ӱ��� ����â ���� - 
// 2009-09-02 by cmkwon, Gameforge4D ���� ���� ����â WebPage�� ó�� - STRMSG_090831_0001�� ���������� ó��
//#define STRMSG_090831_0001					"AirRivals is now protected from cheaters with a hackshield.\r\nPlease install it to help us to make AirRivals even safer.\r\nYou can only continue gameplay once you have installed the hackshield.\r\nPlease read the privacy policy< http://agb.gameforge.de/mmog/index.php?lang=en&art=datenschutz_mmog&special=airrivals&&f_text=b1daf2&f_text_hover=ffffff&f_text_h=061229&f_text_hr=061229&f_text_hrbg=061229&f_text_hrborder=9EBDE4&f_text_font=arial%2C+arial%2C+arial%2C+sans-serif&f_bg=000000 > to find out more about the hackshield's function."
#define STRMSG_090831_0002					"Install hackshield"
#define STRMSG_090831_0003					"cancel"

///////////////////////////////////////////////////////////////////////////////
// 2009-09-02 by cmkwon, Gameforge4D ���� ���� ����â WebPage�� ó�� - 
#define STRMSG_090902_0001					"http://www.airrivals.net/launcher/hackshield.html"

///////////////////////////////////////////////////////////////////////////////
// 2009-10-16 by cmkwon, ���� �ػ� �߰�(1680x1050,1920x1080,1920x1200) - 
#define STRMSG_WINDOW_DEGREE_1680x1050_LOW			"1680x1050 (low)"
#define STRMSG_WINDOW_DEGREE_1680x1050_MEDIUM		"1680x1050 (medium)"
#define STRMSG_WINDOW_DEGREE_1680x1050_HIGH			"1680x1050 (high)"
#define STRMSG_WINDOW_DEGREE_1920x1080_LOW			"1920x1080 (low)"
#define STRMSG_WINDOW_DEGREE_1920x1080_MEDIUM		"1920x1080 (medium)"
#define STRMSG_WINDOW_DEGREE_1920x1080_HIGH			"1920x1080 (high)"
#define STRMSG_WINDOW_DEGREE_1920x1200_LOW			"1920x1200 (low)"
#define STRMSG_WINDOW_DEGREE_1920x1200_MEDIUM		"1920x1200 (medium)"
#define STRMSG_WINDOW_DEGREE_1920x1200_HIGH			"1920x1200 (high)"

///////////////////////////////////////////////////////////////////////////////
// 2011-01-26 by hskim, ���� ������ ���� ��� ��Ȳ
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_NOT_REGISTER			"��ϵ��� ���� �����Դϴ�. (�缳 ������ ���ɼ��� �ֽ��ϴ�)"
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_DB_ERROR				"DB ������ ������ �߻��Ͽ����ϴ�"
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_OK						"���� ������ �����Ͽ����ϴ�."
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_BLOCKED				"�ش� ���� IP�� ��ϵǾ� ������ ���� �ź� ó���Ǿ����ϴ�."
#define STRMSG_AUTHENTICATION_ACCEPT_COMMENT_SHUTDOWN				"������ ���� ���� ����� �����߽��ϴ�."

///////////////////////////////////////////////////////////////////////////////
// 2012-03-30 by hskim, EP4 Ʈ���� �ý��� �̺�Ʈ �˸�
#define STRMSG_120330_0001					"%s�� �ֽ����ο��� ������ ���� ���� ����Ʈ�� �ߵ� �Ͽ����ϴ�."
#define STRMSG_120330_0002					"%s�� �ֽ����ο��� ���� ����ġ 100%% ���� ���Ǿƿ� �̺�Ʈ(1�ð�)�� �ߵ� �Ͽ����ϴ�."
#define STRMSG_120330_0003					"%s�� �ֽ����ο��� ������ ����� 100%% ���� ���Ǿƿ� �̺�Ʈ(1�ð�)�� �ߵ� �Ͽ����ϴ�."
#define STRMSG_120330_0004					"%s�� �ֽ����ο��� ������ ����� 100%% / ���� ����ġ 100%% ���� / SPI�� 100%% ���� ���Ǿƿ� �̺�Ʈ(1�ð�)�� �ߵ� �Ͽ����ϴ�."
#define STRMSG_120330_0005					"%s���� �ֽ����ο��� ���� ������ �ߵ� �Ͽ����ϴ�."
#define STRMSG_120330_0006					"%s���� �ֽ����ο��� ������ ī�� ������ �ߵ� �Ͽ����ϴ�."
#define STRMSG_120330_0007					"%s���� �ֽ����ο��� ŰƮ ������ �ߵ� �Ͽ����ϴ�."
#define STRMSG_120330_0008					"%s���� �ֽ����ο��� ���� ���͸� ��ȯ�ϼ̽��ϴ�."
#define STRMSG_120330_0009					"ũ����Ż�� �Ҹ��Ͽ����ϴ�."

#endif // end_#ifndef _STRING_DEFINE_SERVER_H_

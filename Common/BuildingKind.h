#pragma once

enum BUILDINGKIND : unsigned char
{
	BUILDINGKIND_1ST_WEAPONSHOP = 0,		// �⺻������Ʈ
	BUILDINGKIND_2ND_WEAPONSHOP = 1,		// ��޹�����Ʈ
	BUILDINGKIND_COMPONENTSHOP = 2,		// �Ҹ�ǰ��Ʈ
	BUILDINGKIND_PETSHOP = 3,		// ����Ʈ
	BUILDINGKIND_ANTIQUESHOP = 4,		// ��ǰ��Ʈ
	BUILDINGKIND_CITYWARP = 5,		// ��������(�� ���ÿ���, �� BUILDINGKIND_HANGAR)
	BUILDINGKIND_PUBLICOFFICE = 6,		// ������Ʈ
	BUILDINGKIND_ACTIONHOUSE = 7,		// �����Ʈ
	BUILDINGKIND_RACETRACK = 8,		// ���̽���Ʈ
	BUILDINGKIND_REFINERY = 9,		// ������Ʈ - ���� ����
	BUILDINGKIND_LABORATORY = 10,	// ������Ʈ
	BUILDINGKIND_FACTORY = 11,	// ������Ʈ, �� BUILDINGKIND_ROBOTHANGAR
	BUILDINGKIND_MILITARYACADEMY = 12,	// ����б���Ʈ
	BUILDINGKIND_STORE = 13,	// â��
	BUILDINGKIND_SKILL_SHOP = 14,	// ��ų ����
	BUILDINGKIND_AUCTION = 15,	// ���, ��ǰ ���� ����
	BUILDINGKIND_CITY_OCCUPY_INFO = 16,	// ���� ���� ���� ����
	BUILDINGKIND_TUNING_COLOR = 17,	// 2005-11-11 by cmkwon, Ʃ�� ���� ����
	BUILDINGKIND_ARENA = 18,	// 2007-04-23 by dhjin, ARENA
	BUILDINGKIND_WARPOINT_SHOP = 19,	// 2007-05-17 by dhjin, WARPOINT SHOP
	BUILDINGKIND_BRIEFING_ROOM = 20,	// 2007-07-16 by dhjin, BRIEFING_ROOM
	BUILDINGKIND_TUTORIAL = 21,	// 2007-07-16 by dhjin, BUILDINGKIND_TUTORIAL
	BUILDINGKIND_CITYLEADER_LEADER = 22,	// 2007-08-27 by dhjin, BUILDINGKIND_CITYLEADER_LEADER
	BUILDINGKIND_CITYLEADER_OUTPOST = 23,	// 2007-08-27 by dhjin, BUILDINGKIND_CITYLEADER_OUTPOST
	BUILDINGKIND_LUCKY = 24,	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - �Ƹ�/����/���Ǹ� ���, �ɼǸӽ��� �Ʒ��� ���� �߰�// 2008-11-04 by dhjin, ��Ű�ӽ�, BUILDINGKIND_LUCKY
	BUILDINGKIND_WORLDRANKING = 25,	// 2009-02-12 by cmkwon, EP3-3 ���巩ŷ�ý��� ���� - 
	BUILDINGKIND_INFINITY = 30,	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	BUILDINGKIND_INFINITY_SHOP = 31,	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ����
	BUILDINGKIND_LUCKY_OPTION_MACHINE = 32,	// 2010-04-20 by cmkwon, �ű� ��Ű �ӽ� ���� - �ɼǸӽ��� ���� �߰�
	BUILDINGKIND_DISSOLUTION = 33,	// 2010-08-31 by shcho&jskim �����ۿ��� �ý��� -
	BUILDINGKIND_TRIGGER_CRYSTAL = 34,	// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	BUILDINGKIND_PARTSSHOP = 35,	// 2012-02-29 by mspark, ���� Ÿ��Ʋ �ֱ� - ��������
	BUILDINGKIND_GEARSHOP = 36,	// 2012-02-29 by mspark, ���� Ÿ��Ʋ �ֱ� - ������
	BUILDINGKIND_CARDSHOP = 37,	// 2012-02-29 by mspark, ���� Ÿ��Ʋ �ֱ� - ī�����
	BUILDINGKIND_MYSTERY_SHOP = 38,	// 2012-07-04 by JHAHN, �̽��͸� �ӽ� ����
	BUILDINGKIND_INFLBUFF_SHOP = 39,	// 2013-05-09 by hskim, ���� ����Ʈ ����
	BUILDINGKIND_CASH_SHOP = 100,	// ����ȭ ��ǰ ����
};
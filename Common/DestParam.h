///////////////////////////////////
// �������� ĳ���� �Ķ����� - DES_XXX
enum : DestParam_t
{
	DES_NULL = 0,		// ��� �Ķ���Ͱ� ���� ��� ���
	DES_ATTACK_PART = 1,		// ���� ��Ʈ
	DES_DEFENSE_PART = 2,		// ��� ��Ʈ
	DES_FUEL_PART = 3,		// ���� ��Ʈ
	DES_SOUL_PART = 4,		// ���� ��Ʈ
	DES_SHIELD_PART = 5,		// ���� ��Ʈ
	DES_DODGE_PART = 6,		// ȸ�� ��Ʈ
	DES_ALL_PART = 106,		// ��� ����	// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ��� ���� DES_ALL_PART �߰�
	DES_BODYCONDITION = 7,		// ������
	DES_ENDURANCE_01 = 8,		// ������ 01
	DES_ENDURANCE_02 = 9,		// ������ 02
	DES_CHARGING_01 = 10,		// ��ź�� 01
	DES_CHARGING_02 = 11,		// ��ź�� 02
	DES_PROPENSITY = 12,		// ����
	DES_HP = 13,		// ��Ʈ����Ʈ, MAX HP�� +VALUE��ŭ �÷���
	DES_DP = 89,		// ����(DEFENSE)����Ʈ
	DES_SP = 14,		// �ҿ�����Ʈ
	DES_EP = 15,		// ��������Ʈ
	DES_SPRECOVERY = 16,		// �ҿ�����Ʈȸ����
	DES_HPRECOVERY = 17,		// ����������Ʈȸ����
	DES_MINATTACK_01 = 18,		// (*) �ּ� ���ݷ� 01
	DES_MAXATTACK_01 = 71,		// (*) �ִ� ���ݷ� 01
	DES_MINATTACK_02 = 19,		// (*) �ּ� ���ݷ� 02
	DES_MAXATTACK_02 = 72,		// (*) �ִ� ���ݷ� 02
	DES_ATTACKPROBABILITY_01 = 20,		// ����Ȯ�� 01
	DES_ATTACKPROBABILITY_02 = 21,		// ����Ȯ�� 02
	DES_DEFENSE_01 = 22,		// (*) ���� 01 (������ ���� Ȯ��) 20040622 �߰�
	DES_DEFENSE_02 = 23,		// (*) ���� 02 (������ ���� Ȯ��) 20040622 �߰�
	DES_DEFENSEPROBABILITY_01 = 24,		// ���Ȯ�� 01 (ȸ�� Ȯ��)
	DES_DEFENSEPROBABILITY_02 = 25,		// ���Ȯ�� 02 (ȸ�� Ȯ��)
	DES_SKILLPROBABILITY_01 = 26,		// ��ų����Ȯ�� 01
	DES_SKILLPROBABILITY_02 = 64,		// ��ų����Ȯ�� 02
	DES_FACTION_01 = 79,		// �Ӽ� 01, check: �߰���
	DES_FACTION_02 = 80,		// �Ӽ� 02, check: �߰���
	DES_FACTIONRESISTANCE_01 = 27,		// �Ӽ����׷� 01
	DES_FACTIONRESISTANCE_02 = 65,		// �Ӽ����׷� 02
	DES_SPEED = 28,		// (*) �̵��ӵ�, FIXER ���� �ӵ� ���ҷ�
	DES_TRANSPORT = 29,		// ��ݷ�
	DES_MATERIAL = 30,		// ����
	DES_REATTACKTIME_01 = 31,		// (*) ������Ÿ�� 01 (- ����)
	DES_REATTACKTIME_02 = 32,		// (*) ������Ÿ�� 02 (- ����)
	DES_ABRASIONRATE_01 = 33,		// ������ 01
	DES_ABRASIONRATE_02 = 34,		// ������ 02
	DES_RANGE_01 = 35,		// (*) ��ȿ�Ÿ� 01
	DES_RANGE_02 = 36,		// (*) ��ȿ�Ÿ� 02
	DES_RANGEANGLE_01 = 37,		// ��ȿ���� 01
	DES_RANGEANGLE_02 = 38,		// ��ȿ���� 02
	DES_MULTITAGET_01 = 39,		// ��ƼŸ�� 01
	DES_MULTITAGET_02 = 66,		// ��ƼŸ�� 02
	DES_EXPLOSIONRANGE_01 = 40,		// ���߹ݰ� 01
	DES_EXPLOSIONRANGE_02 = 67,		// ���߹ݰ� 02
	DES_UNIT = 41,		// ������ ���� (28 ~ 29�� ���� ���� ���ָ����� ���������� ����)
	DES_REVISION = 42,		// ������ ������ (28 ~ 29�� ���� ���� ���ָ����� ���������� ����)
	DES_FACTIONPROBABILITY_01 = 43,		// �Ӽ��� ���� ���Ȯ�� 01
	DES_FACTIONPROBABILITY_02 = 68,		// �Ӽ��� ���� ���Ȯ�� 02
	DES_SHOTNUM_01 = 44,		// ������ �� �߻� �� 01
	DES_SHOTNUM_02 = 69,		// ������ �� �߻� �� 02
	DES_MULTINUM_01 = 45,		// ���� �߻� �� 01
	DES_MULTINUM_02 = 70,		// ���� �߻� �� 02
	DES_ATTACKTIME_01 = 46,		// ó�� ���� ���� Ÿ�� 01 (- ����)
	DES_ATTACKTIME_02 = 47,		// ó�� ���� ���� Ÿ�� 02 (- ����)
	DES_TIME_01 = 48,		// (*) ���� �ð� 01, check: + -> *
	DES_TIME_02 = 49,		// (*) ���� �ð� 02, check: + -> *
	DES_OVERHITTIME_01 = 73,		// (*) ������ȸ���ð� 01, check: �߰���
	DES_OVERHITTIME_02 = 74,		// (*) ������ȸ���ð� 02, check: �߰���
	DES_UNITKIND = 50,		// ��ü ���׷��̵�� �ش� ��ü
	DES_ITEMKIND = 51,		// �������� ����(ITEMKIND_XXX)
	DES_SUMMON = 52,		// ���� ��ȯ
	DES_GRADUAL_HP_UP = 53,		// ��������, ���� HP�� ���� �ð�(ITEM.Time�� ����)���� ���������� VALUE��ŭ �÷���, �߰��� �ν��� �Ѱų� ���� ������ cancel
	DES_GRADUAL_DP_UP = 81,		// ��������, ���� DP�� ���� �ð�(ITEM.Time�� ����)���� ���������� VALUE��ŭ �÷���, �߰��� �ν��� �Ѱų� ���� ������ cancel, check: �߰���
	DES_GRADUAL_SP_UP = 54,		// ��������, ���� SP�� ���� �ð�(ITEM.Time�� ����)���� ���������� VALUE��ŭ �÷���, �߰��� ��(?)������ cancel
	DES_GRADUAL_EP_UP = 55,		// ��������, ���� EP�� ���� �ð�(ITEM.Time�� ����)���� ���������� VALUE��ŭ �÷���, �߰��� ��(?)������ cancel
	DES_IN_TIME_HP_UP = 56,		// ��������, ���� HP�� ���� �ð�(ITEM.Time�� ����)�� ���� �� VALUE��ŭ �÷���
	DES_IN_TIME_DP_UP = 82,		// ��������, ���� DP�� ���� �ð�(ITEM.Time�� ����)�� ���� �� VALUE��ŭ �÷���, check: �߰���
	DES_IN_TIME_SP_UP = 57,		// ��������, ���� SP�� ���� �ð�(ITEM.Time�� ����)�� ���� �� VALUE��ŭ �÷���
	DES_IN_TIME_EP_UP = 58,		// ��������, ���� EP�� ���� �ð�(ITEM.Time�� ����)�� ���� �� VALUE��ŭ �÷���
	DES_IMMEDIATE_HP_UP = 59,		// ��������, ���� HP�� ��� VALUE��ŭ �÷���
	DES_IMMEDIATE_DP_UP = 83,		// ��������, ���� DP�� ��� VALUE��ŭ �÷���, check: �߰���
	DES_IMMEDIATE_SP_UP = 60,		// ��������, ���� SP�� ��� VALUE��ŭ �÷���
	DES_IMMEDIATE_EP_UP = 61,		// ��������, ���� EP�� ��� VALUE��ŭ �÷���
	DES_GROUNDMODE = 62,		// ���ݸ��
	DES_SIEGEMODE = 63,		// ������
	DES_WEIGHT_01 = 75,		// (*) ���� 01, check: �߰���
	DES_WEIGHT_02 = 76,		// (*) ���� 02, check: �߰���
	DES_BULLET_01 = 77,		// (*) 1�� �Ѿ� 01, 2005-11-02 by cmkwon ���Ҹ��ʾ˷��� ������
	DES_BULLET_02 = 78,		// (*) 2�� �Ѿ� 02, 2005-11-02 by cmkwon ���Ҹ��ʾ˷��� ������
	DES_PRIMARY_WEAPON = 84,		// 1�� ����, �����
	DES_SECONDARY_WEAPON = 85,		// 2�� ����, ����
	DES_ALL_WEAPON = 86,		// 1,2�� ���� ���, ����
	DES_CRITICALHITRATE_01 = 87,		// (*)ũ��Ƽ�� Ȯ�� 20040622 �߰�
	DES_CRITICALHITRATE_02 = 88,		// (*)ũ��Ƽ�� Ȯ�� 20040622 �߰�
	DES_WARP = 90,		// ���ÿ��� �����ۿ�
	DES_REACTION_RANGE = 91,		// ITEM�� ReactionRange ����
	DES_RARE_FIX_NONE = 92,		// ���λ�, ���̻� ��� ����, ITEMKIND_GAMEBLE�� DestParam1�� ����
	DES_RARE_FIX_PREFIX = 93,		// ���λ�, ITEMKIND_GAMEBLE�� DestParam1�� ����
	DES_RARE_FIX_SUFFIX = 94,		// ���̻�, ITEMKIND_GAMEBLE�� DestParam1�� ����
	DES_RARE_FIX_BOTH = 95,		// ���λ�, ���̻� ��� ����, ITEMKIND_GAMEBLE�� DestParam1�� ����
	DES_REQ_MIN_LEVEL = 96,		// ���������� �䱸 MinLevel�� �����
	DES_REQ_MAX_LEVEL = 97,		// ���������� �䱸 MaxLevel�� �����
	DES_WARP_OUTPOST = 98,		// 2007-09-05 by dhjin, �������� ���ÿ��� �����ۿ�
	DES_CHAT_BLOCK = 99,		// 2008-12-30 by cmkwon, ������ ä�� ���� ī�� ���� - 

	DES_CASH_STAT_ALL_INITIALIZE = 100,		// ���� ��� ���� �ʱ�ȭ ������
	//DES_CASH_STAT_HALF_INITIALIZE = 101,		// (���� ��Ȯ��)���� 50% ���� �ʱ�ȭ ������
	DES_CASH_STAT_PART_INITIALIZE = 102,		// ���� �κ� ���� �ʱ�ȭ ������
	DES_RARE_FIX_PREFIX_INITIALIZE = 103,		// ���λ� �ʱ�ȭ, ITEMKIND_GAMEBLE�� DestParam1�� ����
	DES_RARE_FIX_SUFFIX_INITIALIZE = 104,		// ���̻� �ʱ�ȭ, ITEMKIND_GAMEBLE�� DestParam1�� ����
	DES_ENCHANT_INITIALIZE = 105,		// 2007-04-03 by cmkwon, ��æƮ �ʱ�ȭ, ITEMKIND_ENCHANT�� DestParam1�� ����
	//											106		// ��� ����(DES_ALL_PART)���� ���	// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - ��� ���� DES_ALL_PART �߰�
	DES_CASH_STEALTH = 108,		// ���ڽ� ������
	DES_CASH_HP_AND_DP_UP = 109,		// HP and DP UP ������
	DES_CASH_GUILD_ALL_MEMBERS_SUMMON = 110,		// ��� ���ܿ� ��ȯ - �����常 ��밡��
	DES_CASH_GUILD_MEMBER_SUMMON = 111,		// ���ܿ� 1�� ��ȯ - �����常 ��밡��
	DES_CASH_NORMAL_RESTORE = 112,		// �Ϲ� ��Ȱ ī�� - ��
	DES_CASH_SUPER_RESTORE = 113,		// ���� ��Ȱ ī��
	DES_CASH_GUILD = 114,		// �Ϲ�/��� ���� ī��
	DES_CASH_MONSTER_SUMMON = 115,		// ���� ��ȯ ī��
	DES_CASH_CHANGE_CHARACTERNAME = 116,		// ĳ���͸� ���� ī��
	DES_CASH_SKILL_INITIALIZE = 117,		// ��ų �ʱ�ȭ ī��
	DES_CASH_CHANGE_PILOTFACE = 118,		// �� ���� ī��
	DES_CASH_CRACKER_CARD = 119,		// ���� ���� ī��


	///////////////////////////////////////////////////////////////////////////////
	// 2005-11-21 by cmkwon, ���� �߰��� DesParam
	DES_SKILL_REDUCE_SHIELD_DAMAGE = 121,		// ���忡 ���� �������� �ٿ��ش�.
	DES_SKILL_INVINCIBLE = 122,		// ���� ����
	DES_SKILL_BIG_BOOM = 123,		// 		�̱��� - �ڽ��� HP�� ��� �Ҹ��Ͽ� �ֺ����� �������� ������, �ڽ��� �����Ѵ�.
	DES_SKILL_HALLUCINATION = 124,		// ü�� ����.
	DES_SKILL_RANGEDOWN_01 = 125,		// 		�̱��� - ���� �⺻���� ��Ÿ��� ���δ�.
	DES_SKILL_RANGEDOWN_02 = 126,		// 		�̱��� - ���� ��� ���� ��Ÿ��� ���δ�.
	DES_SKILL_PROHIBITION_01 = 127,		// 		�̱��� - ���� �⺻ ���� ����� ������Ų��.
	DES_SKILL_PROHIBITION_02 = 128,		// 		�̱��� - ���� ��� ���� ����� ������Ų��.
	DES_ATTACK_RANGE_01 = 129,		// ���̴��� �⺻ ���� ���� �Ÿ��� ������Ų��.(*)
	DES_ATTACK_RANGE_02 = 130,		// ���̴��� ��� ���� ���� �Ÿ��� ������Ų��.(*)
	DES_INVISIBLE = 131,		//	���ڽ� ���μ� ������ ������ ������, ���̴ٿ��� ������ �ʴ´�. �ڽŵ� ������ ���� �ʴ´�.
	// 2005-12-02 by cmkwon, 154, 155, 156���� ����DES_SKILL_HYPERMOVING = 132,		// ������ ��ü �̵��ӵ��� ��������, �ν��� ����� ���� �ʴ´�.(*)
	DES_SKILL_DEFENSE_01 = 133,		// 		�̱��� - ���� �⺻������ ����߸���.(*)
	DES_SKILL_DEFENSE_02 = 134,		// 		�̱��� - ���� ��޹����� ����߸���.(*)
	DES_SKILL_FREEZINGBODY = 135,		// 		�̱��� - ���� HP,���� ȸ���� �Ұ����ϰ� �����. ����Ŷ,����Ŷ,��ųȸ��(��ü)�� �ȵȴ�.
	DES_SKILL_REVERSECONTROL = 136,		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -	// 		�̱��� - ���� �������� �ݴ�� �����̰� �Ѵ�.
	DES_SKILL_GRADUAL_HPSHIELD_DOWN = 137,		// 		�̱��� - ���� HP,���带 ���� ���ҽ�Ų��.
	DES_SKILL_SLOWMOVING = 138,		// ���� �������� ���ϰ� �Ѵ�.(�ִ��̵� �ӵ�, �ν��� �ӵ�, �̵� ����, �ν��� �̵� ����)(*)
	DES_SKILL_BOOSTEROFF = 139,		// 		�̱��� - ���� �ν��� ����� ������Ų��.
	DES_SKILL_COLLISIONDAMAGE_DOWN = 140,		// ��� ������Ʈ,�ٴ� �浹�� �������� ���ҽ�Ų��.
	//DES_SKILL_CAMOUFLAGE = 141,		// 		�̱��� - �����Ͽ� ���� �þ߿� ������ �ʴ´�. Ÿ�ٿ��� ������.
	DES_SKILL_RANDOMTELEPORT = 142,		// 		�̱��� - ���� �ʿ��� �����ϰ� �̵��Ѵ�.
	DES_SKILL_ATTACK_ALL = 143,		// 		�̱��� - ��ü ���� ��ų
	DES_SKILL_SCANNING = 144,		// Invisible ������ ������ �� �� �ֵ��� �Ѵ�
	DES_SKILL_REVERSEENGINE = 145,		// ������ ������ �����ϰ� �Ѵ�.
	DES_SKILL_LOCKDOWN = 146,		// 		�̱��� - ������ �Ͻ� ������Ų��.(����,�¿켱ȸ,�ν��� ����, ��, ���ݱ���� ����)
	DES_SKILL_STEALSP = 147,		// 		�̱��� - ������ SP�� ������ �P�ƿ´�.
	DES_SKILL_SMARTSP = 148,		// �ڽ��� ����ϴ� ��� ��ų�� ��뷮�� �ٿ��ش�.(*)
	DES_SKILL_SUMMON_FORMATION_MEMBER = 149,		// ���� �� 1���� ��ȯ�Ѵ�.
	DES_SKILL_CANCEL_MAGIC = 150,		// 		�̱��� - ���� ���� ��ų�� ��� ������Ų��.
	DES_SKILL_RANDOM_CANCEL = 151,		// 		�̱��� - Ÿ���� ����� ��ų�� �ϳ� �����ϰ� �����Ѵ�.
	DES_SKILL_STOPMAGIC = 152,		// ���� ��ų�� �����ð� ��������Ų��. - 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
	DES_SKILL_CANCELALL = 153,		// 		�̱��� - ����� ���� ��ų�� ��� �����Ѵ�.
	DES_SKILL_REACTIONSPEED = 154,		// (*)������ ����(�����ӵ�)
	DES_SKILL_ENGINEANGLE = 155,		// (*)������ ����(��ȸ��)
	DES_SKILL_ENGINEBOOSTERANGLE = 156,		// (*)������ ����(�ν��� ��ȸ��)

	// 2006-03-30 by cmkwon
	DES_DROP_EXP = 157,		// ����ġ, �����̾��� ��ø �Ұ�
	DES_DROP_SPI = 158,		// SPI, �����̾��� ��ø �Ұ�
	DES_DROP_ITEM = 159,		// ������ �����, �����̾��� ��ø �Ұ�
	DES_HP_REPAIR_RATE_FLIGHTING = 160,		// ����� HP ȸ����
	DES_DP_REPAIR_RATE = 161,		// DP ȸ����
	DES_SP_REPAIR_RATE = 162,		// SP ȸ����

	// 2006-07-26 by cmkwon
	DES_BAZAAR_SELL = 163,		// 2006-07-26 by cmkwon, ���� �Ǹ� ����
	DES_BAZAAR_BUY = 164,		// 2006-07-26 by cmkwon, ���� ���� ����

	// 2006-08-14 by dhjin
	DES_KILLMARK_EXP = 165,		// 2006-08-14 by dhjin, ų��ũ ����ġ

	// 2006-10-11 by cmkwon
	DES_HYPER_BOOSTER = 166,		// 2006-10-11 by cmkwon, �ν��� ������ �������� ����

	// 2006-11-17 by dhjin, 2�� ��ų�� ���� �߰��� ����
	DES_SKILL_CHAFF_HP = 167,		// 2006-11-17 by dhjin, ü���� HP
	DES_SKILL_AIR_BOMBING = 168,		// 2006-11-17 by dhjin, ���� ����
	DES_SKILL_NO_WARNING = 169,		// 2006-11-17 by dhjin, Ÿ�� ��ų ����, �ش���� �������� ���̴� ������ �︮�� �ʰ� �ȴ�
	DES_SKILL_ROLLING_TIME = 170,		// 2006-11-17 by dhjin, 30�ʵ��� �Ѹ� ���� �ð��� ����.
	DES_SKILL_FULL_RECOVERY = 171,		// 2006-11-17 by dhjin, ��ü�� HP, ����, ����, SP�� ����ȸ���Ѵ�.
	DES_SKILL_CAMOUFLAGE = 172,		// 2006-11-17 by dhjin, A����� ���� ��ų
	DES_SKILL_BARRIER = 173,		// 2006-11-17 by dhjin, A����� ��޹��⿡ ���� ���� ��ų
	DES_SKILL_HYPERSHOT = 174,		// 2006-11-17 by dhjin, A����� �⺻���� ��¡��, ���÷��� �������� Range
	DES_SKILL_SHIELD_PARALYZE = 175,		// 2006-11-17 by dhjin, A����� ���帶�� ��ų, ������� ���� ȸ������ 0���� �����.

	DES_WARHEAD_SPEED = 176,		// 2007-06-11 by cmkwon, ź���� �ӵ�
	DES_CHAT_ALL_INFLUENCE = 177,		// 2007-08-09 by cmkwon, ��� ���¿� ä�� �����ϱ� - desparam �߰�, ���������

	// 2008-09-22 by dhjin, �ű� ��þƮ
	DES_ENGINE_BOOSTER_TIME_UP = 178,		// 2008-09-22 by dhjin, �ν��� �ð� ����
	DES_ENGINE_MAX_SPEED_UP = 179,		// 2008-09-22 by dhjin, ���� �Ϲݼӵ�(�ִ�) ����
	DES_ENGINE_MIN_SPEED_UP = 180,		// 2008-09-22 by dhjin, ���� �Ϲݼӵ�(�ּ�) ����
	DES_ENGINE_BOOSTER_SPEED_UP = 181,		// 2008-09-22 by dhjin, ���� �ν��ͼӵ� ����
	DES_ENGINE_GROUND_SPEED_UP = 182,		// 2008-09-22 by dhjin, ���� ����ӵ� ����
	DES_RADAR_OBJECT_DETECT_RANGE = 183,		// 2008-09-22 by dhjin, ���̴� ��ü ���� �ݰ�
	DES_PIERCE_UP_01 = 184,		// 2008-09-22 by dhjin, �⺻���� �Ǿ�� ���� ī��
	DES_PIERCE_UP_02 = 185,		// 2008-09-22 by dhjin, ��޹��� �Ǿ�� ���� ī��
	DES_ENGINE_ANGLE_UP = 186,		// 2008-09-30 by dhjin, ���� ȸ���� ���� ī��
	DES_ENGINE_BOOSTERANGLE_UP = 187,		// 2008-09-30 by dhjin, ���� �ν��� ȸ���� ���� ī��

	// 2009-01-05 by dhjin, �̼Ǹ����� - ��� ���� ������ �߰� 
	DES_ITEM_BUFF_INFLUENCE = 188,
	DES_ITEM_BUFF_PARTY = 189,

	// 2009-01-19 by dhjin, ��þƮ Ȯ�� ����, 10��þ �Ĺ� ī�� - ��þƮ Ȯ�� ���� ī��
	DES_ENCHANT_PREVENTION_DELETE_USE_ENCHANT = 190,		// ��� �ϱ� ���� �������� ������æƮ ī��Ʈ ����
	DES_ENCHANT_PREVENTION_DELETE_SAVE_ENCHANT = 191,		// ��æƮ ���н� ����� ��æƮ ī��Ʈ ����
	DES_ENCHANT_INCREASE_PROBABILITY = 192,		// ��æƮ ī�� Ȯ�� ������ ����

	// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - DES_ �߰�
	DES_SKILL_DAMAGE_DISTRIBUTION = 193,		// �������� �������� �л�(�й�) ó�� �Ѵ�.

	// 2009-08-26 by cmkwon, �׷��� ���ҽ� ���� �ý��� ���� - 
	DES_SHAPE_ITEM = 194,		// ���ս� �ҽ� �������� ShapeItemNum�� ���� �� Item, LinkItem�� ���		(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo
	DES_EFFECT_ITEM = 195,		// ���ս� �ҽ� �������� EffectItemNum�� ���� �� Item, �ش� ItemNum�� ���	(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo

	// 2009-09-23 by cmkwon, �ʵ�â�� ĳ�� ������ ���� - 
	DES_FIELD_STORE = 196,		// �ʵ�â��

	// 2009-10-01 by cmkwon, �׷��� ���ҽ� ���� ���� �ʱ�ȭ ��� ���� - 
	DES_INIT_SHAPE_ITEM = 197,		// ���ս� �ҽ� �������� ShapeItemNum�� �ʱ�ȭ �� Item		(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo
	DES_INIT_EFFECT_ITEM = 198,		// ���ս� �ҽ� �������� EffectItemNum�� �ʱ�ȭ �� Item		(Value => KIND_ITEM_FIX_MIXING_WEAPON / KIND_ITEM_FIX_MIXING_DEFENSE)		// 2012-02-20 by hskim, DB ����ȭ - ItemMixingInfo

	// 2009-11-02 by cmkwon, ĳ��(�κ�/â�� Ȯ��) ������ �߰� ���� - DES_XXX �߰�
	DES_INCREASE_INVENTORY_SPACE = 199,		// ĳ���� �κ��丮 ����
	DES_INCREASE_STORE_SPACE = 200,		// ĳ���� â�� ����

	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	DES_ITEM_RESISTANCE = 201,		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������ ��� üũ 
	DES_ITEM_ADDATTACK = 202,		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���밪 Ÿ��ġ ������ ��� üũ 
	DES_ITEM_IGNOREDEFENCE = 203,		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ���� ������ ��� üũ
	DES_ITEM_IGNOREAVOID = 204,		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ȸ�Ƿ� ���� ������ ��� üũ
	DES_ITEM_REDUCEDAMAGE = 205,		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ������ ���밪 ���� ������ ��� üũ
	DES_ITEM_ADDATTACK_SEC = 206,		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���밪 Ÿ��ġ ������ ��޹����(�⹫�� �з�)
	DES_ITEM_ONCE_RESISTANCE = 207,		// 2009-09-09 ~ 2010-01-19 by dhjin, ���Ǵ�Ƽ - �� ���� �����ϰ� �������� ���� ������ �߰�
	DES_SKILL_MON_SILENCE_PRIMARY = 210,		// ���� ���Ϸ��� ��ų (1������ ����)
	DES_SKILL_MON_SILENCE_SECOND = 211,		// ���� ���Ϸ��� ��ų (2������ ����)
	DES_SKILL_MON_FREEZE_HP = 212,		// ���� ������ ��ų HP ȸ�� �Ұ�  
	DES_SKILL_MON_FREEZE_DP = 213,		// ���� ������ ��ų DP ȸ�� �Ұ�
	DES_SKILL_MON_FREEZE_SP = 214,		// ���� ������ ��ų SP ȸ�� �Ұ�
	DES_SKILL_MON_HOLD = 215,		// ���� Ȧ�� ��ų
	DES_SKILL_MON_STEALING = 216,		// ���� ��ƿ�� ��ų 
	DES_SKILL_MON_DRAIN = 217,		// ���� �巹�� ��ų
	DES_SKILL_RELEASE = 218,		// M��� ������ ��ų
	DES_SKILL_MON_SILENCE_SKILL = 219,		// ���� ���Ϸ��� ��ų (��ų ����)

	// 2009-09-09 ~ 2010-02-10 by dhjin, ���Ǵ�Ƽ - �ߵ�������������
	DES_PAIR_DRAIN_1_RATE = 220,		// �巹�� �ߵ� Ȯ��
	DES_PAIR_DRAIN_2_HP_DP_UP_RATE = 221,		// �巹�� ȸ���� HP->DP ȸ�� (�������� %)
	DES_ANTI_DRAIN_RATE = 222,		// �巹�� �ߵ� ���� Ȯ��
	DES_PAIR_REFLECTION_1_RATE = 223,		// ������ �ݻ� Ȯ��
	DES_PAIR_REFLECTION_2_DAMAGE_RATE = 224,		// �ݻ� �������� %
	DES_ANTI_REFLECTION_RATE = 225,		// ������ �ݻ� ���� Ȯ��

	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - 
	DES_BLIND = 226,
	DES_SUPERINTEND = 227,
	DES_IMMEDIATE_HP_OR_DP_UP = 228,		// HP�� ���� ä��� �� ���Ŀ� DP�� ä���.
	DES_HIT_INVOKE_SKILL = 229,		// ���߽� �ش� �����Ķ����� ��ų�� �����Ѵ�.
	DES_TIME_BOMB = 230,

	// 2010-03-18 by cmkwon, ���ͺ��� ���� - 
	DES_TRANSFORM_TO_MONSTER = 231,		// ParamValue�� MonsterUnitKind �Է�

	// 2010-03-23 by cmkwon, ���Ǵ�Ƽ ���� ĳ�� ������ ���� - 1ȸ�� �߰� ���� ����
	DES_INFINITY_REENTRY_TICKET = 232,		// ����� ParamValue�� ������� ����

	// 2010-04-05 by cmkwon, ���ͺ��Ű��� ���� ī�� ���� - 
	DES_TRANSFORM_TO_GEAR = 233,		// ����� ParamValue�� ������� ����.

	// 2010-05-18 by cmkwon, WarPoint ���� ������ ����(�Ϻ���û) - 
	DES_PLUS_WARPOINT_RATE = 234,		// ParamValue: WarPoint ���� Rate

	// 2010-06-01 by shcho, PC�� ���� ȹ��(ĳ��) ������ - 
	DES_PCROOM_USE_CARD = 235,		//PC�� �÷��׸� TRUE�� ��ȯ ��Ų��.

	// 2010-08-26 by shcho&jsKim, �� ������ ���� -
	DES_MAX_SP_UP = 236,		// SP�� �ִ�ġ�� ������Ų��.

	// 2010-08-27 by shcho&&jskim, WARPOINT ���� ������ ����
	DES_WAR_POINT_UP = 237,		// WARPOINT ����

	// 2010-08-27 by shcho&&jskim, WARPOINT ���� ������ ����
	DES_DONATE_POINT_UP = 389,		// WARPOINT ���� //are u here?yes

	// 2010-11-30 by shcho, ��������� ���Ȯ�� ���� ������ ����
	DES_RARE_ITEM_DROP_RATE = 238,		// ���� ������ ���Ȯ�� ����

	// 2010-12-21 by jskim, ���� �̵� �ӵ� ���� ������ ����
	DES_RARE_ITEM_PARTNER_SPEED = 239,		// ���� �̵� ���ǵ� ����

	// 2010-12-21 by jskim, ��Ʈ�� ������ ���� ������ ����
	DES_RARE_ITEM_PARTNER_DAMAGE = 240,		// ��Ʈ�� ������ ����

	// 2010-12-21 by jskim, HP, DP ŰƮ ���뷮 ���� ������ ����
	DES_RARE_ITEM_HPDP = 241,		// HP DP ŰƮ ���뷮

	// 2011-05-02 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - Ư�� ��ǥ�� ��ȯ
	DES_SUMMON_POSITION_X = 242,		// ��ȯ�� ��ġ ���� (��밪)
	DES_SUMMON_POSITION_Y = 243,		// ��ȯ�� ��ġ ���� (��밪)
	DES_SUMMON_POSITION_Z = 244,		// ��ȯ�� ��ġ ���� (��밪)

	// 2011-10-18 by hskim, EP4 [Ʈ���� �ý���] - ȭ���� / �� ��ǳ
	DES_MAPBUFF_RANDOM_ADD_REATTACKTIME = 245,		// �ʹ��� �߰� �ð� ����
	DES_MAPBUFF_RANDOM_ADD_TIME = 246,		// �ʹ��� �߰� �ð� ����
	DES_ENCHANT_ONCE_APPLY = 247,		// 2011-10-19 by hskim, EP4 [Free to play] - 10ȸ ��þƮ ������ ��� ����
	DES_GAMBLE_RARE_ITEM_FIX = 248,		// 2011-10-20 by hskim, EP4 [Free to play] - ����/���� ���� �ɼ�

	DES_GAMBLE_RARE_ITEM_FIX_STD = 249,
	DES_GAMBLE_RARE_ITEM_FIX_ADV = 250,

	// 2013-05-07 by jhseol,bckim ���� �г�Ƽ
	DES_BUFF_PENALTY_RATIO = 251,		// �г�Ƽ ���� (1.0f = 100%)
	DES_BUFF_PENALTY_APPLIED_UNIT_KIND = 252,		// �г�Ƽ ���� ��� (ReqUnitKind)

	// 2013-05-09 by hskim, ���� ����Ʈ ����
	DES_SKILLTYPE_CONSECUTIVE_VICTORIES = 253,		// ���� ����Ʈ ���� - ������ ������ ����
	DES_SKILLTYPE_TRUN_AROUND = 254,		// ���� ����Ʈ ���� - ������ ������ ����

	// 2015-08-02 by killburne
	DES_SKILLTYPE_OUTPOST_BUFF = 255,

	// 2013-05-09 by hskim, ���� ����Ʈ ����
	DES_SKILL_BUFF_MON_ATTACK_POWER = 300,		// ���� ���ݽ� - ���ݷ� ���� : Value ���� %
	DES_SKILL_BUFF_MON_ATTACK_PROBABILITY = 301,		// ���� ���ݽ� - ���ݷ� Ȯ�� : Value ���� %
	DES_SKILL_BUFF_MON_ATTACK_PIERCE = 302,		// ���� ���ݽ� - �Ǿ ���� : Value ���� %
	DES_SKILL_BUFF_MON_DEFENCE = 303,		// ���� ���� - ���� ���� : Value ���� %
	DES_SKILL_BUFF_MON_DEFENCE_AVOID = 304,		// ���� ���� - ȸ�Ƿ� ���� : Value ���� %
	DES_SKILL_BUFF_PVP_ATTACK_POWER = 305,		// PVP - ���ݷ� ���� : Value ���� %
	DES_SKILL_BUFF_PVP_ATTACK_PROBABILITY = 306,		// PVP - ���߷� ���� : Value ���� %		// 2013-08-01 by jhseol, ������ ���� ������
	DES_SKILL_BUFF_PVP_ATTACK_PIERCE = 307,		// PVP - �Ǿ ���� : Value ���� %		// 2013-08-01 by jhseol, ������ ���� ������
	DES_SKILL_BUFF_PVP_DEFENCE = 308,		// PVP - ���� ���� : Value ���� %		// 2013-08-01 by jhseol, ������ ���� ������
	DES_SKILL_BUFF_PVP_DEFENCE_PROBABILITY = 309,		// PVP - ȸ�Ƿ� ���� : Value ���� %		// 2013-08-01 by jhseol, ������ ���� ������


	DES_SKILL_SKILL_CANCEL_IMMUNE = 311,		// 2015-10-20 Future, Purify Immune Skills

	// 2013-05-31 by jhseol,bckim �Ƹ� �÷��� - COLLECTION INDEX DesParam ��� (390~399 : 10�� ����)
	DES_COLLECTION_ARMOR_INDEX = 390,		// �Ƹ� �÷��ǿ��� ���� �ε���

	// 2013-02-28 by bckim, �������� �����߰�
	DES_PLUS_WARPOINT_RATE_FOR_RETURN_USER = 499,		// ParamValue: WarPoint ���� Rate ( ��ȸ�� ) // 1ȸ�� DES_PLUS_WARPOINT_RATE
	DES_BUFF_TO_RETURN_USER = 500,		// 2013-02-28 by bckim, �������� �����߰�

	// 2012-10-10 by hskim, �Ⱓ�� �Ӽ� ���� (�Ⱓ�� ����)
	DES_FIXED_TERM_SHAPE_TIME = 501,		// �Ⱓ�� ���� ���� �ð� (��)
	DES_FIXED_TERM_ITEM_TIME = 502,		// �Ⱓ�� ������ ���� �ð� (��)

	DES_WEB_DELETE_ITEM = 503,		// 2013-03-13 by hskim, �� ĳ�� ���� - �� �������̽��� ���� ���� ������ ������

	DES_INGAME_SHOW = 504,		// �ΰ��� ���սĿ� ǥ������ ���� // 2013-07-02 by bhsohn �ΰ��� ���ս� Show�߰� ó��

	// 2015-12-17 Future, Nation Change Card
	DES_CASH_CHANGE_NATION = 505,		
	DES_CASH_CHANGE_NATION_REQ_FAME = 506,

	// �� ������ ������ ���� ( 600 ~ 699 : 100��)  
	// 2013-04-18 by jhseol,bckim �̴��� �Ƹ� - �߰� DesParam ����
	DES_OPTION_ITEM_DEFAULT_DESPARAM = 600,		// �̴��� �Ƹӿ� ����� �ɼ� ���������� ������ DesParam

	// ��Ʈ�� ���� �����۷� ���� (23000 ~ 23899 : 900 �� )
	DES_PET_SOCKET_ITEM_AUTOKIT = 23000,		// ��Ʈ�� �ý��� ���Ϸ� - �ڵ� ŰƮ, �ʼ������� DestParameter 0���� ���� �ؾ���
	DES_PET_SOCKET_ITEM_AUTOSKILL = 23001,		// ��Ʈ�� �ý��� ���Ϸ� - �ڵ� ��ų, �ʼ������� DestParameter 0���� ���� �ؾ���
	DES_PET_SOCKET_ITEM_SPEED = 23002,		// ��Ʈ�� �ý��� ���Ϸ� - �ڵ� ��ų, �ʼ������� DestParameter 0���� ���� �ؾ���

	// ��Ʈ�� �ڵ� ŰƮ �����۷� ���� (23900 ~ 23909 : 10 ��)
	DES_PET_SLOT_ITEM_AUTOKIT_HP = 23900,		// �ڵ� ŰƮ HP		=> VALUE : ��� 1 ~ 100 ����
	DES_PET_SLOT_ITEM_AUTOKIT_SHIELD = 23901,		// �ڵ� ŰƮ Shield	=> VALUE : ��� 1 ~ 100 ����
	DES_PET_SLOT_ITEM_AUTOKIT_SP = 23902,		// �ڵ� ŰƮ SP		=> VALUE : ��� 1 ~ 100 ����

	// ��Ʈ�� �ڵ� ��ų �����۷� ���� (23910 ~ 23919 : 10 ��)
	DES_PET_SLOT_ITEM_AUTOSKILL_AGEAR = 23910,		// �ڵ� ��ų A ���
	DES_PET_SLOT_ITEM_AUTOSKILL_BGEAR = 23911,		// �ڵ� ��ų B ���
	DES_PET_SLOT_ITEM_AUTOSKILL_IGEAR = 23912,		// �ڵ� ��ų I ���
	DES_PET_SLOT_ITEM_AUTOSKILL_MGEAR = 23913,		// �ڵ� ��ų M ���
};
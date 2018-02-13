// GuildDamageManager.h: interface for the CGuildDamageManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDDAMAGEMANAGER_H__5B4A5A09_6305_4FAF_A58A_C45AD774DBFE__INCLUDED_)
#define AFX_GUILDDAMAGEMANAGER_H__5B4A5A09_6305_4FAF_A58A_C45AD774DBFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
struct SGUILD_DAMAGE
{
	UID32_t			GuildUID1;
	char			GuildName2[SIZE_MAX_GUILD_NAME];	// 2007-08-23 by cmkwon, �� ���Ľ� �ְ� ������ �� ���ܸ� ǥ�� - �ʵ� �߰�
	float			fSumOfDamage1;
	
	SGUILD_DAMAGE()
	{
		GuildUID1		= 0;
		fSumOfDamage1	= 0;
		util::zero(GuildName2, SIZE_MAX_GUILD_NAME);	// 2007-08-23 by cmkwon, �� ���Ľ� �ְ� ������ �� ���ܸ� ǥ�� -
	}
	SGUILD_DAMAGE(UID32_t i_guildUID, char *i_szGName, float i_fDamage)
	{
		GuildUID1		= i_guildUID;
		util::strncpy(GuildName2, i_szGName, SIZE_MAX_GUILD_NAME);	// 2007-08-23 by cmkwon, �� ���Ľ� �ְ� ������ �� ���ܸ� ǥ�� -
		fSumOfDamage1	= i_fDamage;
	}
	void ResetSGUILD_DAMAGE(void)
	{
		GuildUID1		= 0;
		util::zero(GuildName2, SIZE_MAX_GUILD_NAME);	// 2007-08-23 by cmkwon, �� ���Ľ� �ְ� ������ �� ���ܸ� ǥ�� -
		fSumOfDamage1	= 0;
	}
};
typedef mt_vector<SGUILD_DAMAGE>			mtvectGUILD_DAMAGE;

class CGuildDamageManager  
{
public:
	CGuildDamageManager();
	~CGuildDamageManager();

	BOOL AddSumOfDamage(UID32_t i_GuildUID, char *i_szGName, float i_fDamage);	// 2007-08-23 by cmkwon, �� ���Ľ� �ְ� ������ �� ���ܸ� ǥ�� - ���� �߰�
	int GetGuildDamageListSize(void);
	// 2007-08-23 by cmkwon, �� ���Ľ� �ְ� ������ �� ���ܸ� ǥ�� - �Ʒ��� ����ü�� �����ϴ� �Լ��� ����
	//UID32_t GetBest1GuildUIDbySumOfDamage(void);
	SGUILD_DAMAGE * GetBestGuild(void);

	SGUILD_DAMAGE *FindGuildDamageByGuildUID(UID32_t i_GuildUID);
protected:
	mtvectGUILD_DAMAGE		m_mtvectGUILD_DAMAGEList;
};

#endif // !defined(AFX_GUILDDAMAGEMANAGER_H__5B4A5A09_6305_4FAF_A58A_C45AD774DBFE__INCLUDED_)

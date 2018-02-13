// OutPost.h: interface for the COutPost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPOST_H__EB5C6A5B_8BA0_46C4_9826_119C8413C7A4__INCLUDED_)
#define AFX_OUTPOST_H__EB5C6A5B_8BA0_46C4_9826_119C8413C7A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 2007-10-16 by cmkwon, �α� �߰� - AtumParam.h ���Ϸ� ��ġ �̵���
//struct SOutPostProtectorDamage
//{
//	BYTE	Influence;
//	UID32_t	GuildUID;
//	CHAR 	GuildName[SIZE_MAX_GUILD_NAME];
//	float	Damage;
//};
//typedef	mt_vector<SOutPostProtectorDamage>		mtvectSOutPostProtectorDamage;

class CFieldIOCP;

class COutPost  
{
public:
	COutPost(SOUTPOST_INFO * i_pSOutPostInfo, CFieldIOCP * i_pFIOCP);
	virtual ~COutPost();

	void	InitResetPossibleValues();		// ���ο� �ʿ��� ������ �ʱ�ȭ �ϱ�
	void    InitProtectorDamage();			// ��ȣ���� �������� �� ���� �ʱ�ȭ
	BYTE	GetOutPostState();		// �������� ���� �� ��������
	void	SetOutPostState(BYTE i_byOutPostState);	// �������� ���� �� ����
	INT		GetOutPostMapIndex();	// �������� �� ��ȣ ��������
	INT		GetOutPostCityMapIndex();	// �������� ���� �� ��ȣ ��������
	ATUM_DATE_TIME * GetOutPostNextWarTime();	// �������� ���� �ð� ��������
	BOOL	GetOutPostNextWarTimeSet();		// �������� ���� ���� �ð� ���� �������� ��������
	void	SetOutPostNextWarTimeSet(BOOL i_bNextWarTimeSet);		// �������� ���� ���� �ð� ����
	BYTE	GetOutPostInfluence();		// �������� ���� ���� ��������
	UID32_t	GetOutPostGuildUID();		// �������� ���� ����UID ��������
	CHAR *	GetOutPostGuildName();		// �������� ���� ���� �� ��������
	ATUM_DATE_TIME * GetOutPostResetStartTime();	// �������� ���� ���� �ð� ��������
	ATUM_DATE_TIME * GetOutPostResetEndTime();		// �������� ���� ���� �ð� ��������

	UID32_t GetOutPostResetPossibleGuildUID();		// �������� ���� ������ ����UID ��ȯ�ϱ�
	void	SetOutPostResetPossibleGuildUID(UID32_t i_nGuildUID);	// �������� ���� ������ ����UID ����
	BYTE	GetOutPostResetPossibleInfluence();		// �������� ���� ������ ���� ��ȯ�ϱ�
	void	SetOutPostResetPossibleInfluence(BYTE i_byInfluence);	// �������� ���� ������ ���� ����
	CHAR *	GetOutPostResetPossibleGuildName();		// �������� ���� ������ ���ܸ�
	void	SetOutPostResetPossibleGuildName(CHAR * i_szGuildName);	// �������� ���� ������ ���ܸ�
	ATUM_DATE_TIME * GetPotPostGetTime();			// �������� ���� �ð� ��������
	void	SetOutPostGetTime(ATUM_DATE_TIME * i_pCurrentTime);			// �������� ���� �ð� ����
	
	INT		CheckOutPostWarTime(ATUM_DATE_TIME * i_pCurrentTime);	// ���� �ð��� ���������� ���� �ð��� üũ�Ͽ� ���� �ð��� ��ȯ�Ѵ�.
	BOOL	CheckSetOutPostWarDay(ATUM_DATE_TIME * i_pCurrentTime);	// ���� �ð��� ���������� ���� ���ڸ� üũ�Ͽ� ���� ���ڰ� �������� �������� ���� ����Ʈ ���� ���ڷ� �����Ѵ�.

	void	AddDamageOfProtector(BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage);		// �������� ��ȣ���� �������� �� ���ܰ� ������ ���� �����Ѵ�.
	UID32_t GetTopGuildOfProtectorDamage(mtvectSOutPostProtectorDamage *o_pvectDamageList);		// 2007-10-16 by cmkwon, �α� �߰� - �����߰�(mtvectSOutPostProtectorDamage *o_pvectDamageList), // �������� ��ȣ���� �������� ���� ���� �� ������ ��ȯ�Ѵ�.
	
	void	SetOutPostResetStartTime(ATUM_DATE_TIME * i_pCurrentTime);				// �������� ���� �ð� ���� �ð� ����	

	INT		CheckOutPostResetTime(ATUM_DATE_TIME * i_pCurrenTime);		// ���� �ð��� �������� ���� ���� �ð��� üũ�Ͽ� ���� �ð��� ��ȯ�Ѵ�.
	
	void	SetOutPostPossessGuildAndInfluenceByResetSuccess();		// �������� ���� ����, ���� ����
	void	SetOutPostPossessNPCByReset();				// �������� NPC����
	void	SetOutPostPossessInfluenceByReset();				// �������� ���¼���

	void	SetOutPostPossessByWarEnd();		// �������� ������ ������ ���� �� ����
	void	SetOutPostNextWarTime(ATUM_DATE_TIME *i_pNextWarTime);	// �������� �� ���� �ð� ���� (��,�и�)
	void	SetOutPostNextWarTimeByChoiceUser(ATUM_DATE_TIME *i_pNextWarTime);	// �������� �� ���� �ð� ���� (������ �ð� ���� ��, ����~!: �������� ������ ���� ���İ� �ƴϹǷ� OUTPOST_NEXTWARGAP�� ������ �ʴ´�)

	void	OutPostProtectorDestroy();		// �������� ��ȣ�� �ı�
	void	OutPostResetDestroy();		// �������� ���� �ı�

//////////////////////////////////////////////////////////////////////////

	void	SendRemainTimeWarStart();		// �������� ���� �� ���� �ð� ����
	void	InitOutPostResetStartTime();		// �������� ���� �ð� �ʱ�ȭ
	void	SetOutPostPossessInfluence(BYTE i_byInfluence);			// 2008-03-26 by dhjin, �������� ���� ���� ����

	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���
	void	SetOutPostWarStartTime();		// ���� ���۽ð��� ���� �ð����� ����
	void	InitOutPostInfluence();			// �������� �ʱ�ȭ
	void	InitOutPostGuildUID();			// ������� �ʱ�ȭ
	void	InitOutPostGuildName();			// ������� �ʱ�ȭ
	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý���

protected:
	CFieldIOCP		*m_pFieldIOCP19;

	BYTE			m_OutPostState;											// �������� ���� ��
	INT				m_OutPostMapIndex;										// �������� �� ��ȣ
	INT				m_OutPostCityMapIndex;									// �������� ���� �� ��ȣ
	ATUM_DATE_TIME	m_OutPostGetTime;										// �������� ���� �ð�
	ATUM_DATE_TIME	m_OutPostNextWarTime;									// �������� ���� ���� �ð�
	BOOL			m_OutPostNextWarTimeSet;								// �������� ���� ���� �ð� ���� ���� => 0 : ����, 1 : �Ұ�
	BYTE			m_OutPostInfluence;										// �������� ���� ����
	UID32_t			m_OutPostGuildUID;										// �������� ���� ����UID
	CHAR			m_OutPostGuildName[SIZE_MAX_GUILD_NAME];				// �������� ���� ���ܸ�
	ATUM_DATE_TIME	m_OutPostResetStartTime;								// �������� ���� ���� �ð�
	ATUM_DATE_TIME	m_OutPostResetEndTime;									// �������� ���� ���� �ð�
	UID32_t			m_OutPostResetPossibleGuildUID;							// �������� ���� �� �� �ִ� ���� UID
	BYTE			m_OutPostResetPossibleInfluence;						// �������� ���� �� �� �ִ� ����
	CHAR			m_OutPostResetPossibleGuildName[SIZE_MAX_GUILD_NAME];	// �������� ���� �� �� �ִ� ���ܸ�

	mtvectSOutPostProtectorDamage	m_mtvectProtectorDamageInfo;		// �������� ��ȣ���� ���� ������ ����
};

#endif // !defined(AFX_OUTPOST_H__EB5C6A5B_8BA0_46C4_9826_119C8413C7A4__INCLUDED_)

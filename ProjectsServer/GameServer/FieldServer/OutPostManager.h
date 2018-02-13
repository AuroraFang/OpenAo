// OutPostManager.h: interface for the COutPostManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPOSTMANAGER_H__072CE381_6268_4DD7_BF2E_550A12FC6894__INCLUDED_)
#define AFX_OUTPOSTMANAGER_H__072CE381_6268_4DD7_BF2E_550A12FC6894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutPost;
class CFieldIOCP;

///////////////////////////////////////////////////////////////////////////////
// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
class CFieldMapChannel;
struct SOUTPOSTWAR_EV_RESET_SUCCESS		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
{
	BYTE				bySuccessInfluenceTy;
	CFieldMapChannel*	pFMChanOfOutPostWarMap;		// ������������ �߻��� CFieldMapChannel
};
typedef vector<SOUTPOSTWAR_EV_RESET_SUCCESS>		vectSOUTPOSTWAR_EV_RESET_SUCCESS;

struct SOUTPOSTWAR_EV_END		// 2009-07-08 by cmkwon, ���� ������ ���� ���� - 
{
	BYTE				byWinInfluenceTy;
	BYTE				byLossInfluenceTy;
	CFieldMapChannel*	pFMChanOfOutPostWarMap;		// ������������ �߻��� CFieldMapChannel
	UID32_t				GuildUIDPossessOutPost;		// ���������� ������ GuildUID
	int					nCityMapIndexForWarp;		// ���������� ������ ������ �������� ���ø����� ���� ��Ű�� ���� MapIndex
};
typedef vector<SOUTPOSTWAR_EV_END>					vectSOUTPOSTWAR_EV_END;

typedef mt_vector<COutPost*>		mtvectOutPost;
typedef	vector<ATUM_DATE_TIME>		vectATUM_DATE_TIME;	

class COutPostManager  
{
public:
	COutPostManager();
	virtual ~COutPostManager();

	void	InitOutPostManager(CFieldIOCP * i_pFIOCP);		
	COutPost * GetOutPost(int i_nMapIndex);
	COutPost * GetOutPostCityMap(int i_nCityMapIndex);
	void	SetOutPostManagerByDBLoad(vectSOutPostInfo * i_pVectOutPostInfo);	// �������� ���� ������ DB�� ���� �о� �������� ���� �� ��ŭ Class OutPost�� �����Ͽ� m_mtvectSOutPost�� �߰��Ѵ�.
	void	SetOutPostNextWarTimeInfoByDBLoad(vectATUM_DATE_TIME * i_pvectOutPostNextWarTimeInfo);	// �������� ���� �ð� ���� ������ DB���� �о�´�.

	void	OnDoMinutelyWorkOutPostManager(ATUM_DATE_TIME *i_pCurDateTime);	// �� ���� �������� ���� �̺�Ʈ üũ
	
	void	SendOutPostWarRemainTime(int i_nRemainTime, BYTE i_byInfluence, int i_nMapIndex);			// ������� ���� ���� ���� �ð��� Ŭ���̾�Ʈ���� �����Ѵ�.
	void	OutPostWarStart(COutPost * i_pOutPost);								// �������� �� ����

	void	OnOutPostProtectorDestroy(int i_nMapIndex);		// �������� ��ȣ�� �ı��� ó�� 
	void	OnOutPostResetDestroy(int i_nMapIndex);			// �������� �߾�ó�� �ı��� ó�� 

	BOOL	CheckValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID);				// ��ȿ�� �������� üũ
	INT		GetOutPostCityMapIndexByGuildUID(UID32_t i_nGuildUID);					// ��ȿ�� �����̸� �������� ���� �� ��ȣ�� �����Ѵ�.
	INT		GetOutPostMapIndexByGuildUID(UID32_t i_nGuildUID);						// ��ȿ�� �����̸� �������� �� ��ȣ�� �����Ѵ�.
	BOOL	CheckResetValidGuildUID(int i_nMapIndex, UID32_t i_nGuildUID);				// ������ ��ȿ�� �������� üũ
	BYTE	GetOutPostInfluenceByMapIndex(int i_nMapIdx);							// 2007-09-28 by cmkwon, ���������� ���� üũ ���� - �ش� ���������� ���� ������ �����Ѵ�.

	BOOL	SendOutPostResetStartByGuildCommander(int i_nMapIndex, UID32_t i_guildUID, UID32_t i_charUID);				// 2007-10-16 by cmkwon, �α� �߰� - �����߰�(, UID32_t i_guildUID, UID32_t i_charUID), // �������� ������ ���� ��

	// 2009-07-08 by cmkwon, ���� ������ ���� ���� - �����߰�(, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList)
	void	OutPostResetSuccess(COutPost * i_pOutPost, vectSOUTPOSTWAR_EV_RESET_SUCCESS *io_pVectEvResetSuccessList);		// �������� ���� ����

	// 2009-07-08 by cmkwon, ���� ������ ���� ���� - �����߰�(, vectSOUTPOSTWAR_EV_END *io_pEvEndList)
	void 	OutPostWarEnd(COutPost * i_pOutPost, vectSOUTPOSTWAR_EV_END *io_pEvEndList);		// �������� �� ����

	void	SetExpediencyFundByOutPostWarEnd(int i_nMapindex, int i_nCityMapindex, byte i_byInfluence, UID32_t i_nGuildUID);		// �ǰ��� ����
	void	UpdateExpediencyfundRateLeader(int i_nMapIndx, byte i_byInfluence, int i_nChangeExpediencyFundRate);	// 2008-08-20 by dhjin, �ǰ������� INT������ ������̴�. float -> INT�� ���� // ������ �ǰ����� ������ �����Ѵ�.

	void	SetInitOutPostMonsterSummonByBell(byte i_byInfl, int * o_byBell1, int * o_byBell2, int * o_byBell3);	// �������� �ʱ�ȭ �� �� ���͸� �����ϱ� ���� Bell�� ����
	void	AddDamageOfProtectorW(int i_nMapIndex, BYTE i_byInfluence, UID32_t i_nGuildUID, CHAR * i_szGuildName, float i_fDamage);		// �������� ��ȣ���� �������� �� ���ܰ� ������ ���� �����Ѵ�.

	void	MakeMsgOutPostNextWarInfoRequest(int i_nMapIndex, BOOL i_bCheckGuildCommander, BYTE * o_pData, int * o_nSize);		// �������� �ð� ���� ���� ����
	INT		MakeMSG_FC_OUTPOST_NEXTWAR_INFO_REQUEST_OK(ATUM_DATE_TIME * o_pOutPostNextWarTimeListInfo, int * o_nSelectTimeChoice, ATUM_DATE_TIME * i_OutPostNextWarTime);	// �������� �ð� ���� �޼��� �����
	Err_t	OutPostNextWarTimeSetByChoiceUserW(int i_nMapIndex, int i_nSelectTimeChoice, UID32_t i_guildUID, UID32_t i_charUID);	// 2007-10-16 by cmkwon, �α� �߰� - �����߰�(, UID32_t i_guildUID, UID32_t i_charUID), // �������� ���� ���� �ð� ����

	BOOL	CheckOutPostWarTimeStrategyPoint(ATUM_DATE_TIME *i_pStrategyPointSummonTime);	// ��������Ʈ ��ȯ �� �������� ���� �ð��� �ɸ����� üũ

	ATUM_DATE_TIME * GetOutPostNextWarTime(int i_nMapIndex);				// �������� ���� ���� �ð� ������
	ATUM_DATE_TIME * GetOutPostNextWarTimeByCityMapIndex(int i_nCityMapIndex);				// ���� ������ �������� ���� ���� �ð� ������
	BOOL	CheckOutPostWaring(int i_nMapIndex);					// �������� ������ ���������� üũ
	BOOL	CheckALLOutPostWaring();								// �������� ������ ���������� üũ ��� ��
	BOOL	CheckOutPostPossessByGuildUID(UID32_t i_nGuildUID);		// ���������� ������ �������� üũ
	BOOL	CheckOutPostResetByGuildUID(UID32_t i_nGuildUID);		// ���������� �������� �������� üũ
	
	INT		CheckAllOutPostWaringAndSendClient(SOUTPOST_WAR_INFO *o_pOutPostWarInfo);			// �������� ������ �߹��ߴ��� üũ�Ͽ� Ŭ���̾�Ʈ���� ������ �����Ѵ�.

	void	OutPostPossessNPCByDeleteGuild(UID32_t i_nGuildUID);				// 2007-11-09 by dhjin, ���� ���� ��û���� �������� ���� �ϰ� �ִ� �����̾��� ��� NPC������ ����

	INT		MakeAllOutPostInfoMsg(SCITY_WARINFO_OUTPOST *o_pOutPostWarInfo);						// 2007-12-03 by dhjin, �������� ��� ���� ����Ʈ �����

protected:
	
	CFieldIOCP					*m_pFieldIOCP18;
	mtvectOutPost		 		m_mtvectOutPostManager;	// ��������
	vectATUM_DATE_TIME			m_vectOutPostNextWarTimeInfo;	// �������� �ð� ���� ����

};

#endif // !defined(AFX_OUTPOSTMANAGER_H__072CE381_6268_4DD7_BF2E_550A12FC6894__INCLUDED_)

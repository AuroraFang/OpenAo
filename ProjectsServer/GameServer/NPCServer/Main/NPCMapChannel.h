#pragma once

#include "MapChannel.h"
#include "NPCMonster.h"
#include "AtumProtocol.h"

#include <timeapi.h>

typedef struct
{
	int			nCreatedCount;						// ������� �������� ������ ������ ����
	int			nCurrentCount;						// ���� �����ϴ� ���ͷ� �� �������� ������ ������ ����
	DWORD		dwLastTimeMonsterCreate;
	DWORD		dwBossMonResTime;					// ���� ���ʹ� ��ȯ�ð��� random���� �ȴ�
} MONSTER_CREATE_REGION_INFO_EX;
typedef vector<MONSTER_CREATE_REGION_INFO_EX>	vectorMonserCreateRegionInfoEX;

class CLIENT_INFO
{
public:
	ClientIndex_t	ClientIndex;
	BYTE			ClientState;
	BYTE			UseControlSkill;
	DWORD			TimeUseControlSkill;
	UID32_t			CharacterUniqueNumber;	
	UID32_t			GuildUID10;
	UID32_t			GuildMasterCharacterUID;
	BOOL			bStealthState;
	BOOL			bInvisible;						// 2006-11-27 by dhjin
	USHORT			Race;							// ����
	USHORT			UnitKind;						// ������ ����	
	BYTE			InfluenceType1;
	BYTE			CharacterMode1;
	float			CurrentHP;
	D3DXVECTOR3		PositionVector;					// ĳ���� ��ǥ
	D3DXVECTOR3		TargetVector;
	BodyCond_t		BodyCondition;					// ����, bit flag ���
	MAP_CHANNEL_INDEX	MapChannelIdx;			// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� - 
	DWORD			dwSendedReqCharacterInfoTick;		// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� - 
	

	///////////////////////////////////////////////////////////////////////////////
	/// \fn			void ResetClientInfo(void)
	/// \brief		CLIENT_INFO�� ������ �缳���ϴ� �Լ���, ClientIndex�� �ʱ�ȭ ���� �ʴ´�.
	/// \author		cmkwon
	/// \date		2004-04-01 ~ 2004-04-01
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	void ResetClientInfo(void)
	{
		ClientState					= CS_NULL;
		UseControlSkill				= FALSE;
		TimeUseControlSkill			= 0;
		CharacterUniqueNumber		= 0;
		GuildUID10					= INVALID_GUILD_UID;
		GuildMasterCharacterUID		= 0;
		bStealthState				= FALSE;
		bInvisible					= FALSE;				// 2006-11-27 by dhjin
		Race						= 0;
		UnitKind					= 0;
		InfluenceType1				= INFLUENCE_TYPE_NORMAL;
		CharacterMode1				= FALSE;
		CurrentHP					= 0.0f;
		util::zero(&PositionVector, sizeof(D3DXVECTOR3));
		util::zero(&TargetVector, sizeof(D3DXVECTOR3));
		BodyCondition				= 0;
		MapChannelIdx.MapIndex		= 0;		// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� -
		MapChannelIdx.ChannelIndex	= 0;		// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� -
		dwSendedReqCharacterInfoTick	= 0;	// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� - 
	}

	BOOL IsSameInfluenceCharacterAndMonster(int i_nMonsterBelligerence)		// 2005-12-28 by cmkwon
	{
		return IS_SAME_CHARACTER_MONSTER_INFLUENCE(InfluenceType1, i_nMonsterBelligerence);
	}

	// 2007-06-12 by cmkwon, �����߰�(BOOL i_bRecognizeStealth=FALSE)
	BOOL IsEnbleTargeted(int i_nMonsterBelligerence, BOOL i_bRecognizeInvisible=FALSE, BOOL i_bRecognizeStealth=FALSE)		// 2005-12-28 by cmkwon, Ÿ���� �Ҽ� �ִ� ĳ�����ΰ�?
	{
		if(CS_GAMESTARTED != ClientState)
		{
			return FALSE;
		}
		
		if(CharacterMode1)
		{
			return FALSE;
		}

		if(COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK))
		{
			return FALSE;
		}

		if(IsSameInfluenceCharacterAndMonster(i_nMonsterBelligerence))
		{
			return FALSE;
		}

		return IsTargetableCharacter(i_bRecognizeInvisible, i_bRecognizeStealth);		// 2007-01-16 by cmkwon, ������
	}


	CLIENT_INFO & operator=(const MEX_CHARACTER_1 rhs)
	{
		Race					= rhs.Race;
		UnitKind				= rhs.UnitKind;
		InfluenceType1			= rhs.InfluenceType0;
		CharacterMode1			= rhs.CharacterMode0;
		CharacterUniqueNumber	= rhs.CharacterUniqueNumber;
		GuildUID10				= rhs.GuildUID;
		BodyCondition			= rhs.BodyCondition;
		CurrentHP				= rhs.CurrentHP;
		PositionVector			= A2DX(rhs.PositionVector);
		return *this;
	}

	
	///////////////////////////////////////////////////////////////////////////////
	/// \fn			BOOL IsTargetableCharacter(void)
	/// \brief		
	///				// 2007-06-12 by cmkwon, �����߰�(BOOL i_bRecognizeStealth=FALSE)
	/// \author		dhjin
	/// \date		2006-11-27 ~ 2006-11-27
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	BOOL IsTargetableCharacter(BOOL i_bRecognizeInvisible=FALSE, BOOL i_bRecognizeStealth=FALSE)
	{
// 2007-01-16 by cmkwon, �κ����� ��ų �ν� ���� ��� �߰��� �Ʒ��� ���� ������
//		if(FALSE == bStealthState 
//			&& FALSE == bInvisible)
//		{// 2006-11-27 by dhjin, ���ڽ�/���� �Ѵ� FALSE �϶� Ÿ���� ����
//			return TRUE;
//		}
//
//		return FALSE;

		///////////////////////////////////////////////////////////////////////////////
		// 2012-11-05 by jhseol, ���ڽ� ������ �ɸ��͸� �ν��Ѵ�. ������ �� GM �ɸ��� ����
		if (COMPARE_RACE(Race, RACE_OPERATION | RACE_GAMEMASTER | RACE_MONITOR) && bStealthState)
		{
			return FALSE;
		}
		// end 2012-11-05 by jhseol, ���ڽ� ������ �ɸ��͸� �ν��Ѵ�. ������ �� GM �ɸ��� ����

		///////////////////////////////////////////////////////////////////////////////
		// 2007-01-16 by cmkwon, ���ڽ����� üũ
		if(FALSE == i_bRecognizeStealth
			&& bStealthState)
		{
			return FALSE;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// 2007-01-16 by cmkwon, �κ����� �ν� üũ
		if(FALSE == i_bRecognizeInvisible
			&& bInvisible)
		{
			return FALSE;
		}

		return TRUE;
	}


	///////////////////////////////////////////////////////////////////////////////
	/// \fn			BOOL IsSendableReq_FN_GET_CHARACTER_INFO(void)
	/// \brief		// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� - ���� ��û�� 3�ʿ� �ѹ��� �������� �Ѵ�.
	/// \author		cmkwon
	/// \date		2008-12-02 ~ 2008-12-02
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	BOOL IsSendableReq_FN_GET_CHARACTER_INFO(void)
	{
		DWORD dwCur = timeGetTime();
		if(3000 > dwCur - dwSendedReqCharacterInfoTick)
		{
			return FALSE;
		}
		dwSendedReqCharacterInfoTick = dwCur;
		return TRUE;
	}


	///////////////////////////////////////////////////////////////////////////////
	/// \fn			
	/// \brief		// 2010-01-06 by cmkwon, ���� ���ݽ� Ÿ�� ���� ���� üũ �߰�(Ÿ�ٺ���) - 
	/// \author		cmkwon
	/// \date		2010-01-06 ~ 2010-01-06
	/// \warning	
	///
	/// \param		
	/// \return		
	///////////////////////////////////////////////////////////////////////////////
	BOOL IsValidClient(BOOL i_bLiveCheck=TRUE)
	{
		if(CS_NULL == ClientState)
		{
			return FALSE;
		}

		if(i_bLiveCheck)
		{
			if(CS_GAMESTARTED != ClientState
				|| COMPARE_BODYCON_BIT(BodyCondition, BODYCON_DEAD_MASK))
			{
				return FALSE;
			}
		}
		return TRUE;
	}
};
typedef vector<CLIENT_INFO>			vectorCLIENT_INFO;

typedef mt_vector<MSG_FN_MONSTER_CREATE_OK>				mt_vectorMSG_FN_MONSTER_CREATE_OK;
typedef mt_vector<MSG_FN_MONSTER_DELETE>				mt_vectorMSG_FN_MONSTER_DELETE;
typedef mt_vector<MSG_FN_MONSTER_SKILL_END_SKILL>		mt_vectorMSG_FN_MONSTER_SKILL_END_SKILL;
typedef mt_vector<MSG_FN_BATTLE_SET_ATTACK_CHARACTER>	mt_vectorMSG_FN_BATTLE_SET_ATTACK_CHARACTER;
typedef mt_vector<MSG_FN_CITYWAR_START_WAR>				mt_vectorMSG_FN_CITYWAR_START_WAR;
typedef mt_vector<MSG_FN_CITYWAR_END_WAR>				mt_vectorMSG_FN_CITYWAR_END_WAR;
typedef mt_vector<MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO>	mt_vectorMSG_FN_CITYWAR_CHANGE_OCCUPY_INFO;
typedef mt_vector<MSG_FN_MONSTER_STRATEGYPOINT_INIT>	mt_vectorMSG_FN_MONSTER_STRATEGYPOINT_INIT;
typedef mt_vector<MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL>	mtvectMSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL;	// 2007-08-22 by cmkwon, �ش� ��ä�� ���� ��� �����ϱ� ��� �߰�
typedef mt_vector<MSG_FN_MONSTER_OUTPOST_INIT>			mt_vectorMSG_FN_MONSTER_OUTPOST_INIT;	// 2007-08-24 by dhjin, �������� ���� ����
typedef mt_vector<MSG_MONSTER_SUMMON_BY_BELL>			mtvectMSG_MONSTER_SUMMON_BY_BELL;		// 2007-09-19 by cmkwon, Bell�� ��ȯ ó�� 
typedef mt_vector<MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE>		mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE;	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Key���� ����
typedef mt_vector<MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY>		mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY;	// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
typedef mt_vector<MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE>		mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE;	// 2011-05-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�

class CNPCMapWorkspace;
class CNPCMapProject;
class CNPCIOCP;
class CNPCMapChannel : public CMapChannel  
{
public:
	CNPCMapChannel(CNPCMapWorkspace *i_pWorkspace, CNPCMapProject *i_pProject, ChannelIndex_t i_nChannelIndex);
	virtual ~CNPCMapChannel();

	CNPCMonster * GetNPCMonster(ClientIndex_t i_MonsterIndex);

	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
	CLIENT_INFO* GetClientInfo(int i_Characteridx, MAP_CHANNEL_INDEX *i_pMapChann=NULL);
	MONSTER_CREATE_REGION_INFO_EX * GetMonsterCreateRegionInfoEXWidhIndex(int i_nCreateRegionIdex);

	// ���� ����, �̵�
	void CreateMonstersAllCreateRegion(BYTE *pSendBuf, vector<D3DXVECTOR3> *pVECTOR2vector, DWORD i_dwCurrentTick);
	void CreateMonstersBYRegion(BYTE *pSendBuf, vector<D3DXVECTOR3> *pVECTOR2vector, DWORD i_dwCurrentTick
		, int i_nArrIdx, MONSTER_CREATE_REGION_INFO *pMonsterCreateRegionInfo, MONSTER_CREATE_REGION_INFO_EX *pRegionInfoEX
		, BOOL i_bMustCreate=FALSE);
	void CreateMonstersBYObject(BYTE *pSendBuf, OBJECTINFOSERVER *pObjInfo, DWORD i_dwCurrentTick, BOOL i_bMustCreate=FALSE);
	void UpdateMonsterPositionAllMonster(PBYTE pSendBuf, vector<ClientIndex_t>* pvecClientIndex, DWORD i_dwCurrentTick);

	void SetCityWarOccupyGuildUID(UID32_t uid) { m_CityWarOccupyGuildUID = uid; }

	// Protocol ó��
	void ProcessReceivedAllProtocol(void);
private:
	// ���� �̵�, ����, ����
	void UpdateMonsterPositionHandler(CNPCMonster *i_pNMonster, BYTE *i_pSendBuf, vector<ClientIndex_t> *i_pvecClientIndex, BOOL i_bNotMove);
	void _UpdateAttackedItemInfo(CNPCMonster *i_pNMonster, BYTE *i_pSendBuf);
	void _BattleAttackOnMonsterDead(BOOL *i_bDeleteProcess, CNPCMonster * i_pNPCMon);
	void _UpdateMonsterPositionHandlerAttack(CNPCMonster *i_pNMonster, BYTE *i_pSendBuf, vector<ClientIndex_t> *i_pvecClientIndex);
	BOOL _CheckMonsterChangeMonsterForm(CNPCMonster * i_pNMonster);	
	D3DXVECTOR3 GetFrontPosition(CNPCMonster * i_pNPCMon);

	BOOL CheckMonsterPosition(CNPCMonster *pMon, float fTimeRate);
// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - public���� ����
//	BOOL CheckMonsterPositionWarp(CNPCMonster *pMon, float fTimeRate);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	void AttackMonster2Unit(CNPCMonster *i_pnMonster, BYTE *pSendBuf);
	void AttackMonster2Character(CNPCMonster *i_pnMonster, BYTE *pSendBuf);
	void AttackMonster2Character(CNPCMonster *i_pnMonster, CLIENT_INFO * i_pClientInfo, BYTE *pSendBuf);
	void SendAttack2FieldServer(CNPCMonster *i_pnMonster, CLIENT_INFO * i_pClientInfo, BYTE *pSendBuf);
	void SendAttack2MonFieldServer(CNPCMonster *i_pnMonster, CNPCMonster *i_pTargetMonster, BYTE *pSendBuf);		// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���Ͱ� ����
	void SendMonsterMove2FieldServer(CNPCMonster *i_pnMonster, BYTE *i_SendBuf);

// 2005-01-19 by cmkwon
//	void AttackMonster2CharacterWithPrimaryItem(CNPCMonster *i_pnMonster, CLIENT_INFO * i_pClientInfo, BYTE *pSendBuf);
//	void AttackMonster2CharacterWithSecondaryItem(CNPCMonster *i_pnMonster, CLIENT_INFO * i_pClientInfo, BYTE *pSendBuf);
	void AttackMonster2Monster(CNPCMonster *i_pnMonster, BYTE *pSendBuf);
	void UpdateMissilePosition2Character(CNPCMonster *i_pMonster, BYTE *pSendBuf);
	void DelelteMonsterHandler(CNPCMonster * i_pNMonster);
	// 2009-09-09 ~ 2010-01-13 by dhjin, ���Ǵ�Ƽ - ��ȯ ���ݵ� HPAction��� �����ϰ� ����, �ذ� ���� CMonster -> CNPCMonster�� ����
//	BOOL NPCMonsterAttackSkill(CMonster *pMonster, ITEM *pSkillItem);
	BOOL NPCMonsterAttackSkill(CNPCMonster *pMonster, ITEM *pSkillItem);
	BOOL NPCMonsterAttackSkill(CMonster *pMonster, SSUMMON_EVENT_MONSTER *i_pSummonEvMon);		// 2008-04-16 by cmkwon, ���� ��� �� ���� ��ȯ �̺�Ʈ �ý��� ���� - 
	void NPCMonsterMPOption(CNPCMonster *pMonster);	

	BOOL CheckMonsterSelectedItem(CNPCMonster * i_pnMonster);

	// �浹 üũ
	BOOL CheckImpactFrontPositionMap(CNPCMonster * i_pNMon, D3DXVECTOR3 *o_pTarVector3);
	BOOL CheckImpactPositionObjects(D3DXVECTOR3 *i_pVec3Start, D3DXVECTOR3 *i_pVec3End, INT i_nExcludeObjNum);
	BOOL CheckImpactStraightLineMapAndObjects(D3DXVECTOR3 *vMonPos, D3DXVECTOR3 *vTarPos, INT i_nExcludeObjNum, BOOL bFlagObjectCheck=TRUE);	
	BOOL CheckAndModifyImpactPositionObjects(CNPCMonster * pMon);
	
public:	
	mt_vectorMSG_FN_MONSTER_CREATE_OK			m_mtvectorMSG_FN_MONSTER_CREATE_OK;
	mt_vectorMSG_FN_MONSTER_CREATE_OK			m_mtvectorMSG_FN_MONSTER_CREATE_OKProcess;
	mt_vectorMSG_FN_MONSTER_DELETE				m_mtvectorMSG_FN_MONSTER_DELETE;
	mt_vectorMSG_FN_MONSTER_DELETE				m_mtvectorMSG_FN_MONSTER_DELETEProcess;
	mt_vectorMSG_FN_MONSTER_SKILL_END_SKILL		m_mtvectorMSG_FN_MONSTER_SKILL_END_SKILL;
	mt_vectorMSG_FN_MONSTER_SKILL_END_SKILL		m_mtvectorMSG_FN_MONSTER_SKILL_END_SKILLProcess;
	mt_vectorMSG_FN_BATTLE_SET_ATTACK_CHARACTER	m_mtvectorMSG_FN_BATTLE_SET_ATTACK_CHARACTER;
	mt_vectorMSG_FN_BATTLE_SET_ATTACK_CHARACTER	m_mtvectorMSG_FN_BATTLE_SET_ATTACK_CHARACTERProcess;
	mt_vectorMSG_FN_CITYWAR_START_WAR			m_mtvectorMSG_FN_CITYWAR_START_WAR;
	mt_vectorMSG_FN_CITYWAR_END_WAR				m_mtvectorMSG_FN_CITYWAR_END_WAR;
	mt_vectorMSG_FN_CITYWAR_CHANGE_OCCUPY_INFO	m_mtvectorMSG_FN_CITYWAR_CHANGE_OCCUPY_INFO;
	mt_vectorMSG_FN_MONSTER_STRATEGYPOINT_INIT	m_mtvectorMSG_FN_MONSTER_STRATEGYPOINT_INIT;
	mtvectMSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL	m_mtvectMSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL;		// 2007-08-22 by cmkwon, �ش� ��ä�� ���� ��� �����ϱ� ��� �߰�
	mt_vectorMSG_FN_MONSTER_OUTPOST_INIT		m_mtvectorMSG_FN_MONSTER_OUTPOST_INIT;		// 2007-08-24 by dhjin, �������� ���� ���� 
	mtvectMSG_MONSTER_SUMMON_BY_BELL			m_mtvectMSG_MONSTER_SUMMON_BY_BELL;			// 2007-09-19 by cmkwon, Bell�� ��ȯ ó��
	mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE	m_mtvectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE;		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Key���� ���� 
	mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY	m_mtvectorMSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY;	// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	mt_vectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE		m_mtvectorMSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE;	// 2011-05-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ���� �߰�

	// Socket���� ���� ����Ÿ�� ����
	BOOL NPCOnMonsterCreateOK(MSG_FN_MONSTER_CREATE_OK * i_pCreateOK);
	BOOL NPCOnMonsterDelete(MSG_FN_MONSTER_DELETE * i_pMonDelete);
	BOOL NPCOnMoveOK(MSG_FN_MOVE_OK	* i_pMoveOK);
	BOOL NPCOnAdminSummonMonster(MSG_FN_ADMIN_SUMMON_MONSTER * i_pSummonMonster, int i_nTargetIndex = 0);
	BOOL NPCOnClientGameStartOK(MSG_FN_CLIENT_GAMESTART_OK * i_pClientStartOK);
	BOOL NPCOnClientGameEndOK(MSG_FN_CLIENT_GAMEEND_OK * i_pClientEndOK);
	BOOL NPCOnCharacterChangeBodycondition(MSG_FN_CHARACTER_CHANGE_BODYCONDITION * i_pChange);
	BOOL NPCOnCharacterChangeStealthState(MSG_FN_CHARACTER_CHANGE_STEALTHSTATE * i_pChange);
	BOOL NPCOnCharacterChangeCurrentHPDPSPEP(MSG_FN_CHARACTER_CHANGE_CURRENTHPDPSPEP * i_pChange);
	BOOL NPCOnCharacterChangeCharacterMode(MSG_FN_CHARACTER_CHANGE_CHARACTER_MODE_OK * i_pChange);
	BOOL NPCOnCharacterChangeInfluenceType(MSG_FN_CHARACTER_CHANGE_INFLUENCE_TYPE * i_pChange);
	BOOL NPCOnCharacterChangeInvisible(MSG_FN_CHARACTER_CHANGE_INVISIBLE * i_pChange);

	BOOL NPCOnMonsterChangeHP(MSG_FN_MONSTER_CHANGE_HP * i_pChange);
	BOOL NPCOnGetCharacterInfoOK(MSG_FN_GET_CHARACTER_INFO_OK * i_pInfoOK);
	BOOL NPCOnSkillUseSkillOK(MSG_FN_SKILL_USE_SKILL_OK * i_pSkillOK);
	BOOL NPCOnBattleSetAttackCharacter(MSG_FN_BATTLE_SET_ATTACK_CHARACTER * i_pAttackInfo);
	BOOL NPCOnMonsterChangeBodycondition(MSG_FN_MONSTER_CHANGE_BODYCONDITION * i_pChange);
	BOOL NPCOnBattleDropFixer(MSG_FN_BATTLE_DROP_FIXER * i_pFixer);
	BOOL NPCOnMonsterSkillEndSkill(MSG_FN_MONSTER_SKILL_END_SKILL * i_pEndSkill);
	BOOL NPCOnMonsterStrategyPointInit(MSG_FN_MONSTER_STRATEGYPOINT_INIT * i_pMsg);
	BOOL NPCOnMonsterOutPostInit(MSG_FN_MONSTER_OUTPOST_INIT * i_pMsg);			// 2007-08-24 by dhjin
	BOOL NPCOnMonsterSummonByBell(MSG_MONSTER_SUMMON_BY_BELL * i_pMsg);			// 2007-09-19 by cmkwon, Bell�� ��ȯ ó��
	BOOL NPCOnMonsterCinemaMonsterCreate(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE * i_pMsg);			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Key���� ���� 
	BOOL NPCOnMonsterCinemaMonsterDestroy(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY * i_pMsg);		// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	BOOL NPCOnMonsterCinemaMonsterChange(MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE * i_pMsg);		// 2011-05-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	BOOL NPCOnMonsterCinemaMonsterRegen(MSG_FN_NPCSERVER_CINEMA_MONSTER_REGEN * i_pMsg);		// 2011-06-02 ���Ǵ�Ƽ 3�� - ���� 6 - �ֱ��� ��ȯ ��� ����

	BOOL NPCOnCityWarStart(MSG_FN_CITYWAR_START_WAR *i_pCityWarStart);
	BOOL NPCOnCityWarEnd(MSG_FN_CITYWAR_END_WAR *i_pCityWarEnd);
	BOOL NPCOnCityWarChangeOccupyInfo(MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO *i_pCityWarChangeOccupyInfo);

	void ProcessNPCOnMonsterCreateOK(MSG_FN_MONSTER_CREATE_OK * i_pCreateOK);
	void ProcessNPCOnMonsterDelete(MSG_FN_MONSTER_DELETE * i_pMonDelete);
	void ProcessNPCOnMonsterSkillEndSkill(MSG_FN_MONSTER_SKILL_END_SKILL * i_pEndSkill);
	void ProcessNPCOnBattleSetAttackCharacter(MSG_FN_BATTLE_SET_ATTACK_CHARACTER * i_pAttackInfo);
	void ProcessNPCOnCityWarStartWar(MSG_FN_CITYWAR_START_WAR *i_pStartWar);
	void ProcessNPCOnCityWarEndWar(MSG_FN_CITYWAR_END_WAR *i_pEndWar);
	void ProcessNPCOnCityWarChangeOccupyInfo(MSG_FN_CITYWAR_CHANGE_OCCUPY_INFO *i_pChangeOccupyInfo);
	void ProcessNPCOnMonsterStrategyPointInit(MSG_FN_MONSTER_STRATEGYPOINT_INIT *i_pMsg);
	void ProcessNPCOnMonsterDeleteMonsterInMapChannel(MSG_FN_NPCSERVER_DELETE_MONSTER_IN_MAPCHANNEL *i_pMsg);	// 2007-08-22 by cmkwon, �ش� ��ä�� ���� ��� �����ϱ� ��� �߰�
	void ProcessNPCOnMonsterOutPostInit(MSG_FN_MONSTER_OUTPOST_INIT *i_pMsg);					// 2007-08-24 by dhjin
	void NPCOnSummonObjectMonsterBYBelligerence(int i_nbell);								// 2007-08-24 by dhjin
	void ProcessNPCOnMonsterSummonByBell(MSG_MONSTER_SUMMON_BY_BELL *i_pMsg);				// 2007-09-19 by cmkwon, Bell�� ��ȯ ó��
	void ProcessNPCOnMonsterCinemaMonsterCreate(MSG_FN_NPCSERVER_CINEMA_MONSTER_CREATE *i_pMsg);		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - Key���� ����
	void ProcessNPCOnMonsterCinemaMonsterDestroy(MSG_FN_NPCSERVER_CINEMA_MONSTER_DESTROY *i_pMsg);		// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	void ProcessNPCOnMonsterCinemaMonsterChange(MSG_FN_NPCSERVER_CINEMA_MONSTER_CHANGE *i_pMsg);		// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	CNPCMonster * GetInitNPCMonster();			// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -

	// ���� ä�ο��� ������ Unit Index�� �����´�
	BOOL NPCCharacterExistInRange(D3DXVECTOR3 *pPos, int nBlockGap);

	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - �����߰�(, BYTE i_AttMonsterBell)
	// 2010-04-01 by cmkwon, ����2�� �߰� ���� - ����/�Ʊ� ����Ʈ �÷��� �߰�
	int NPCGetAdjacentCharacterIndexes(D3DXVECTOR3 *pPos, int nDistance, int nBlockDistance, vector<ClientIndex_t> *pClientIndexVector, BYTE i_AttMonsterBell, BOOL i_bGetEnemyList=TRUE);

	CMonster* NPCMonsterExistInRange(int nMonsterIdx, const D3DXVECTOR3 *pPositionVector3, float fDisGap, float fBlockGap);
	int NPCGetAdjacentMonsterIndexes(D3DXVECTOR3 *pPos, int nDistance, int nBlockDistance, vector<ClientIndex_t> *pClientIndexVector, INT i_MonsterUnitKind=0);
	int NPCGetMonsterCountInRegion(int tileStartXIdx, int tileStartZIdx, int tileEndXIdx, int tileEndZIdx, int nMonType, int nMaxCount);
	int NPCGetCreatablePosition(BYTE nMonsterForm, int nMonsterSize, int tileStartXIdx, int tileStartZIdx, int tileEndXIdx, int tileEndZIdx, int nMinHeight
		, int nMaxHeight, vector<D3DXVECTOR3> &vecVECTOR2, int nMaxCount, BOOL bCharCheckFlag=TRUE, BOOL i_bAbsoluteAltitude=FALSE);

	// 
	int NPCGetTargetwithAttackObj(BYTE AttackObj, CMonster *pM, vector<ClientIndex_t> &ClientIndexVector);
	void NPCSetPartyPosition(CNPCMonster *pMons);

	BOOL Send2FieldServerW(const BYTE *pData, int nSize);
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	BOOL Send2FieldServerW(const msg_t<t>& msg) { return this->Send2FieldServerW(reinterpret_cast<const BYTE*>(&msg), msg.size()); }
	
	BOOL Send2FieldServerByTCPW(const BYTE *pData, int nSize);		// 2007-11-26 by cmkwon, ���� �ڵ����� �޽��� TCP�� ����(N->F) - 
	template<template<MessageType_t> typename msg_t, MessageType_t t>
	BOOL Send2FieldServerByTCPW(const msg_t<t>& msg) { return this->Send2FieldServerByTCPW(reinterpret_cast<const BYTE*>(&msg), msg.size()); }
	
	BOOL SetInitialPositionAndSendCreateMonster(CNPCMonster *i_pMons, BYTE *i_pBufSend, D3DXVECTOR3 *i_pSummonPos=NULL);

	// ������Ʈ ���� ����
	OBJECTINFOSERVER *FindObjectMonsterInfoByObjectEventIndex(int i_nObjectEventIndex);
	OBJECTINFOSERVER *FindObjectMonsterInfoByMonsterUniqueNumberAndMinimumDistance(INT i_nMonsterUniqueNumber, D3DXVECTOR3 *i_pVec3Position);
	OBJECTINFOSERVER *FindObjectBossMonsterInfo(void);		// 2006-11-22 by cmkwon, �Լ��� ����(FindCityWarObjectMonsterInfo->FindObjectBossMonsterInfo)
	OBJECTINFOSERVER *FindObjectMonsterInfoBYBelligerence(BYTE i_byBellig);	// 2007-08-18 by cmkwon, ������Ʈ ���� ��ȯ ������ MONSTER_INFO * �����ϱ� - �Լ��߰�
	BOOL GetObjectMonsterByMonsterIdx(INT i_nMonsterIdx, vectorObjectInfoServerPtr *o_pObjectInfoServList);	// 2010-03-31 by cmkwon, ����2�� �߰� ���� - // 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -

	BOOL IsEnableCreateMonster(MONSTER_INFO *i_pMonInfo);

	// 2007-08-22 by cmkwon, �ش� ��ä�� ���� ��� �����ϱ� ��� �߰�
	void DeleteAllMonster(BOOL i_bAll=TRUE, int i_byBell1=-1, int i_byBell2=-1, int i_byExcludeBell1=-1, int i_byExcludeBell2=-1);
	void DeleteUnitKindMonster(INT iMonsterUnitKind);		// 2011-04-28 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�
	void ChangeUnitKindMonster(INT iMonsterUnitKind, INT iChangeMonsterUnitKind);		// 2011-05-11 by hskim, ���Ǵ�Ƽ 3�� - �ó׸� ���� ��� �߰� - �ش� ��ä�� Ư�� ���� ���� ��� �߰�

	void SetNotCreateMonsterValue(bool i_bNotCreateMonster);		// 2007-08-29 by dhjin, ���� ���� ���� ���� ����
	
	OBJECTINFOSERVER *GetTeleportObjectMonsterSummonInfo(CNPCMonster *i_pNMon);	// 2007-09-20 by cmkwon, �ڷ���Ʈ ��ȯ ���� ���� 

	///////////////////////////////////////////////////////////////////////////////
	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - 
	BOOL ChangeTarget(CNPCMonster *i_pNMon);

	BOOL CheckMonsterPositionWarp(CNPCMonster *pMon, float fTimeRate);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 

	///////////////////////////////////////////////////////////////////////////
	// virtual
	// �ʱ�ȭ
	virtual BOOL InitMapChannel(void);

	// start 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����
	CNPCIOCP *GetNPCIOCPServer();
	// end 2011-05-23 by hskim, ���Ǵ�Ƽ 3�� - ��������Ʈ ����

protected:
	// ���� 2�� ���� ����
	UINT						m_uiMissileUniqueIndex;

	// ���� ���� ���⿡ ���� UniqueIndex
	UINT						m_uiAttackedItemUniqueIndex;

	// NPC Monser vector
	CNPCMonster					*m_ArrNPCMonster;

// 2008-12-02 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ���� ���� - CNPCIOCP �� �̵���.
//	vectorCLIENT_INFO			m_vectorClientInfo;
	
	// ���� ���� ������ ���� �߰�����
	vectorMonserCreateRegionInfoEX		m_vectorMonsterCreateRegionInfoEX;
	vectorObjectInfoServer		m_vectorObjectMonsterInfoCopy;				// ������Ʈ ���� ���� ����


	//
	CNPCMapWorkspace			*m_pNPCMapWorkspace;
	CNPCMapProject				*m_pNPCMapProject;
	CNPCIOCP					*m_pNPCIOCPServer;

	BOOL						m_bNotCreateMonster;		// ���� ���� ���� 0 => False 1 => True

public:
	// ���������� ����
	UID32_t						m_CityWarOccupyGuildUID;

	// 2008-12-03 by cmkwon, NPCServer Ŭ���̾�Ʈ ���� ��ä�� ó�� ���� - 
	DWORD						m_dwWorkeredTick;
	BOOL						m_bExistUserInMapChannel;
	DWORD						m_dwChangedTickforExistUser;
	BOOL IsProcessableWorker(DWORD i_dwCurTick);
	void SetExistUserInMapChannel(BOOL i_bIsExistUser);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
	BodyCond_t GetPreAttackBodyCondMask(INT i_nAttackItemIdx);
	BodyCond_t GetFireAttackBodyCondMask(INT i_nAttackItemIdx);
	BOOL SendFSvrHPTalk(CNPCMonster *i_pMons, HPACTION_TALK_HPRATE * i_pTalkHPRate);
	BOOL m_bAutoCreateMonsterChannel;	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -  ä�� ���� Ȯ���Ѵ�.
	void SetAutoCreateMonsterChannel(BOOL i_bCreate);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -  �� ������ ��������
	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������Ʈ�� ����!!!!
	mtDeletedObjectInfoList	m_mtDeletedObjectInfoList;			// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
	mtNewObjectInfoList		m_mtNewObjectInfoList;				// 2009-09-09 ~ 2010-01 by dhjin, ���Ǵ�Ƽ - �ҽ� üũ
	void ChangeNewObject(ObjectIdx_t i_dwDeleteObjectUID, ObjectNum_t i_dwNewObject);
	void ResetChangeObject();

	int GetMonsterIndexesByBell(BYTE i_byMonsterBell, vector<ClientIndex_t> *pClientIndexVector);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ʿ��� ����Bell���� ���� ã��
	int NPCGetAdjacentMonsterIndexesByBell(D3DXVECTOR3 *pPos, int nDistance, int nBlockDistance, BYTE i_byMonsterBell, vector<ClientIndex_t> *pClientIndexVector);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������ Bell���� ���� ã��
	int GetMonsterIndexesByTargetMonsterNum(INT i_nTargetMonsterNum , vector<ClientIndex_t> *pClientIndexVector);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �ʿ��� TargetMonster ã��
	int NPCGetTargetMonsterwithAttackObj(BYTE AttackObj, CMonster *pAttackMon, vector<ClientIndex_t> &ClientIndexVector);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - TargetMonster �����ϱ�
	int BattleAttackSecMultiTargetUser(CNPCMonster *i_pAttackMon, CLIENT_INFO * i_pClientInfo, int MultiTargetCheckSize);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 2�� ���� ��Ƽ Ÿ�� ���� ó��
	int BattleAttackSecMultiTargetMonster(CNPCMonster *i_pAttackMon, CNPCMonster *i_pTargetMonster, int MultiTargetCheckSize);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 2�� ���� ��Ƽ Ÿ�� ���� ó��
	void SendFSvrBattleAttackSec(CNPCMonster *i_pAttackMon, ClientIndex_t i_ClientIdx, D3DXVECTOR3 * i_pTargetPosition, int fMonsterMultiTargetIndex = 0);	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 2�� ���� ���� ��Ŷ - // 2011-03-21 by hskim, ���Ǵ�Ƽ 3�� - ���� ��Ƽ Ÿ���� ��� �߰� - fMonsterMultiTargetIndex �� �߰�

	///////////////////////////////////////////////////////////////////////////////
	// 2010-01-06 by cmkwon, ���� ���ݽ� Ÿ�� ���� ���� üũ �߰�(Ÿ�ٺ���) - 
	BOOL GetTargetObject(BOOL *o_pbIsTargetCharacter, CLIENT_INFO **o_ppClient, CNPCMonster **o_ppMonster, CNPCMonster *i_pNMon, BOOL i_bLiveCheck=TRUE);
	BOOL CheckValidTarget(CNPCMonster *i_pNMon);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-16 by cmkwon, ����2�� MtoM, MtoC Ÿ�� ���� ���� ���� - 
	BOOL SearchTarget(CNPCMonster *i_pNMon);
	BOOL GetUnitObject(ClientIndex_t i_unitIdx, CLIENT_INFO **o_ppCliInfo, CNPCMonster **o_ppNPCMon);

	///////////////////////////////////////////////////////////////////////////////
	// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -
	BOOL SetMultiTargetEnemy8CheckFullList(vector<ClientIndex_t> * i_pTargetIndexList, vector<ClientIndex_t> *o_pTargetIndexList, ClientIndex_t i_ExceptTargetIdx, int MultiTargetCheckSize);
	void GetMultiTargetEnemyList(CNPCMonster *i_pMon, ClientIndex_t i_TargetIdx, vector<ClientIndex_t> *o_pTargetIndexList, int MultiTargetCheckSize, BOOL i_bCheckDistance = TRUE);			
	void GetTargetOurList(CNPCMonster *i_pMon, vector<ClientIndex_t> *o_pTargetIndexList, BOOL i_bExceptMe = TRUE, BOOL i_bCheckDistance = TRUE);
	ClientIndex_t GetTargetOur(CNPCMonster *i_pMon, BOOL i_bRepair = FALSE, BOOL i_bExceptMe = TRUE, BOOL i_bCheckDistance = TRUE);
	BOOL DeleteClientIdx(vector<ClientIndex_t> *o_pTargetIndexList, ClientIndex_t i_DeleteClientIdx);
	void SetMonsterTargetInRangeByBell(CNPCMonster *i_pMon);
	ClientIndex_t GetOurTagetByUsingItem(CNPCMonster *i_pMon);				// ������� ��ų�� �Ʊ� Ÿ�ٴ�� ��������
	BOOL CheckRepairDesParam(DestParam_t i_DestParam);						// ȸ�������� üũ
	ClientIndex_t GetPossibleRepairTarget(CMonster *pM, vector<ClientIndex_t> &ClientIndexVector);		// ȸ���� ��ų�϶� ȸ�� ������ ��� ��������
};

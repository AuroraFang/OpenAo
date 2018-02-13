// Quest.h: interface for the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUEST_H__C9FFDA46_B3F8_4FBF_AA50_4547696C4FEF__INCLUDED_)
#define AFX_QUEST_H__C9FFDA46_B3F8_4FBF_AA50_4547696C4FEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define QUEST_USER_DEFINED_VARIABLES_FILE_NAME	"q_000000.cfg"

#define WIN32_LEAN_AND_MEAN // so that windows.h won't load winsock.h

#include "mt_stl.h"
#include "AtumParam.h"
#include "AtumDataLoader.h"
#include "QuestLoaderParser.h"

#define LAST_SCENARIO_QUEST_NEXT_QUEST_INDEX			-1			// 2006-08-07 by cmkwon, �ó����� �̼��� ��� NextQuestIndex�� -1�̸� ������ ����Ʈ��



struct QUEST_DROP_ITEM
{
	INT				MonsterUniqueNumber;	// ���� ����
	INT				ItemNumber;				// �����۰����ѹ�
	INT				MinCount;				// ������ �������� �ּ� ����
	INT				MaxCount;				// ������ �������� �ִ� ����, ���� ������ ������ �ִٸ� MinCount = MixCount
	INT				Probability;			// ���� Ȯ��: 0 ~ 10000, Prob10K_t
};
struct QUEST_DROP_CHARACTER2ITEM
{
	BYTE			InfluenceType0;			// ���� Ÿ������
	USHORT			UnitKindMask0;			// ������ ����
	CHARACTER2ITEM	Character2Item;
};


class CQuest  
{
public:
	CQuest();

	void Reset();
	CQuest *Clone();
	BOOL IsExistTargetMonsterVector(INT i_monUniqueNumber);
	BOOL IsPartyWarp(void);
	BOOL IsPartyQuest(void);
	BOOL IsFirstScenarioQuest(BYTE i_byInflTy);

	
	BOOL IsEndTypeTimeOver(void);			// 2007-12-06 by cmkwon, �ð����� ����Ʈ �������� �ð� üũ�ϰ� ���� - CQuest::IsEndTypeTimeOver() �߰�
	BOOL IsCompleteEndTypeTimeOver(int i_nSpentTimeInSec);	// 2007-12-06 by cmkwon, �ð����� ����Ʈ �������� �ð� üũ�ϰ� ���� - CQuest::IsCompleteEndTypeTimeOver() �߰�



public:
	// Basic Info
	INT					QuestIndex;
	char				QuestName[SIZE_MAX_QUEST_NAME];
	BYTE				QuestEpisodeType;				// 2008-06-16 by cmkwon, EP3 ����Ʈ ���� ����Ʈ�� ���� - QUEST_EPISODE_TYPE_XXX
	BYTE				QuestPartyType;			// 2008-12-02 by dhjin, �̼Ǹ�����		
	BYTE				QuestKind;			// �Ϲ�, �ݷ���, �ó�����
	BYTE				QuestType;			// 1ȸ��, ���Ӽ�: QUEST_TYPE_SINGLE, QUEST_TYPE_SERIES
	BYTE				QuestStartType;		// QUEST�� �߻��ϴ� ������ ���� �з�: QUEST_START_TYPE_XXX
	BYTE				QuestEndType;		// QUEST�� ���� ��Ŀ� ���� �з�: QUEST_END_TYPE_XXX
	BYTE				QuestPartyMembers;	// 2006-03-24 by cmkwon, ��Ƽ����Ʈ ����, ��Ƽ����, ��ȿ��:0 ~ SIZE_MAX_PARTY_MEMBER(6)
	QUEST_NPC_INFO		QuestNPCInfo;		// ����Ʈ�� �ִ� NPC ���� �� Talk
	char				QuestDescription[SIZE_MAX_NPC_TALK_STRING];	// ����Ʈ�� ���� ����
	MapIndex_t			EventMapIndex;					// ����Ʈ�� �Ͼ�� ��, ex)�ش� �ʿ��� ���͸� ���� ���� �������� ��������.
	INT					NextQuestIndex;					// ���� ����Ʈ ��ȣ, QuestType�� ���Ӽ��� ���� �ǹ� ����
	INT					PartyQuestMemberCount;			// 1 �̻��̸� ��Ƽ ����Ʈ��, �ش� �ο���ŭ ��Ƽ�� �ΰ� ����� �ؾ� �������� ������.
	BOOL				DropItemToAllAttackCharacter;	// �ѹ��̶� �������� �� ��� ĳ���Ϳ��� Quest Drop Item�� �ִ� �� ����, ����Ʈ�� �ִ� �������� �� ĳ�����Ը� �������� ��.
	INT					TimeLimitInMinutes;				// ����Ʈ ���� Ÿ���� QUEST_END_TYPE_TIME_OVER�϶��� ����� ����Ʈ �Ϸ� ����, �׿ܿ��� �ð� ����(�д���)
	BOOL				IsDiscardable;					// ����Ʈ ���� �� YES Ȥ�� NO�� ������ �ش��ϴ� ���ǿ� ���� ������ �޶����� ����(YES ����Ʈ�� ������, NO �׳� �ѱ�), �⺻���� FALSE
	BOOL				IsCityWar;						// ���������� ����Ʈ �ΰ�? - ���������� ����Ʈ�� ������ QuestParam1�� ������ ID�� �����Ѵ�.
	BOOL				IsNeverCompletionByUser;		// ������ ��û���� ����Ʈ�� �Ϸ���� �ʴ´�
	BOOL				IsDropItemToAllPartyMembers;	// 2009-01-13 by cmkwon, �̼� ������ ��Ƽ�� ��ο��� �ֱ� ��ɾ� �߰� - CQuest�� ��� ���� �߰�

	// Start Requirements
	USHORT					ReqUnitKind;
	INT_RANGE				ReqLevel;			// �ʿ� ���� ����
	Experience_t			ReqExperience;		// check: ���߿� �ʿ��ϸ� ����ϱ�, ������ ������� �ʰ� ����! 20040422, kelovon
	INT_RANGE				ReqAttackPart;
	INT_RANGE				ReqDefensePart;
	INT_RANGE				ReqFuelPart;
	INT_RANGE				ReqSoulPart;
	INT_RANGE				ReqShieldPart;
	INT_RANGE				ReqDodgePart;
	vector<INT>				ReqQuestIndexVector;
	vector<INT>				ReqRecvQuestIndexVector;
	vector<ITEM_W_COUNT>	ReqItemVector;
	INT						ReqGuild;
	BYTE					ReqInfluenceType;				// ���� Ÿ�� ����ũ		
	MAP_AREA				ReqStartArea;			// 2005-08-22 by cmkwon, �̼� ���� ���ǿ� Ư�� ��ǥ ����

	// Start Action
	vector<QUEST_PAY_ITEM>	StartPayItemVector;		// ���� �� �ִ� ������
	vector<ITEM_W_COUNT>	StartDelItemVector;		// ���� �� ����� ������
	MapIndex_t				StartMapIndex;			// ���۽� ������ MapIndex
	BOOL					StartPartyWarpFlag;		// StartMapIndex�� ��ȿ �� �� ��Ƽ�� ��� �����ϴ����� �÷���(TRUE:��� ��Ƽ�� �̼Ǹ����� �̵�, FALSE�ڽŸ� �̼Ǹ����� �̵�)

	// Completion Requirements
	vector<ITEM_W_COUNT>			TargetItemVector;			// ����Ʈ �ϷḦ ���� �ʿ��� ������ ����Ʈ
	vector<ITEM_W_COUNT_MAP_AREA>	TargetMonsterItemVector;	// ���� ��� ����Ʈ���� ����� �� �������� ������ �� ��� ����, 0�̸� ����
	QUEST_NPC_INFO					TargetMeetNPCInfo;			// NPC�� ������ �ذ�Ǵ� ����Ʈ����, ������ �� NPC, 0�̸� ����
	INT								TargetMeetObjectIndex;		// object�� ������ �ذ�Ǵ� ����Ʈ����, ������ �� object, 0�̸� ����
	MAP_AREA						TargetMeetObjectMapArea;	// object�� ������ �ذ�Ǵ� ����Ʈ����, ������ �� object�� ��ġ, 2005-07-07 by cmkwon, ����Ÿ���� ������ �ذ�Ǵ� ����Ʈ����, ������ �� ����Ÿ���ε��� - �������� �ڵ����� �Ϸ� ó����
	INT								TargetDeadByMonsterUniquenumber;	// ������ ���Ϳ� ���� ������ ����Ʈ �ذ� ����
	MAP_AREA						TargetMeetMapArea;			// 2005-08-29 by cmkwon, �̼� ���� ���ǿ� Ư�� ��ǥ ����
	vectMONSTER_W_COUNT				TargetMonsterVector;		// 2005-10-19 by cmkwon, ����Ʈ �ϷḦ ���� ����ؾ��ϴ� ���� ī��Ʈ ����Ʈ
	ITEM_W_COUNT					TargetMixItemTarget;		// 2007-04-02 by cmkwon, �̼� ���� ���ǿ� ������ ���� �߰��� ������
																// ���� ����� �ʿ� �������̸�, �� �������� �����Ǿ� ������ �ٸ� �ʿ������ üũ�� �ʿ� ����
	
	// Completion Action
	vector<ITEM_W_COUNT>			EndDeleteItemVector;	// ����Ʈ ���� �� �������� ������
	vector<QUEST_PAY_ITEM>			EndPayItemVector;		// ����Ʈ ���� �� �־����� ������
	vector<QUEST_PAY_ITEM>			EndMissionMasterPayItemVector;		// 2008-12-02 by dhjin, �̼Ǹ����� - ����Ʈ ���� �� �̼Ǹ����Ϳ��� �־����� ������	
	vector<QUEST_RANDOM_PAY_ITEM>	EndRandomPayItemVector;	// ����Ʈ ���� �� RANDOM�ϰ� �־����� ������
	vector<ITEM_MIXING_INFO>		EndPayMixItemVector;	// ����Ʈ ���� �� ���տ� ���� �־����� ������
	BYTE							EndPayBonusStat;
// 2005-11-15 by cmkwon, ������
//	BYTE							EndPayBonusSkillPoint;
	INT								EndPayFame;			// ���� ������
	INT								EndPayStatus;		// �ź� �����
	BYTE							EndPayLevel;		// ���� ��·�(+1, +2 ��)
	INT								EndPayExperience;	// ����ġ ��·�(+100, +200 ��)
	MapIndex_t						EndPayWarpMapIndex;	// ������ ��
	INT								EndPayPropensity;	// �ź� ��� ���ʽ�
	INT								EndCinemaNum;		// CinemaNum
	INT								EndInfluenceType;	// 0�� �ƴϸ� ������ �����Ѵ�.

	// Quest�� Drop Item List, �ε� �� CFieldIOCP::m_mapMonster2Item�� �־� �־�� ��
	vector<QUEST_DROP_ITEM>			vecQuestDropItem;
	vector<QUEST_DROP_CHARACTER2ITEM>	vecQuestDropCharacter2Item;		// 2006-03-02 by cmkwon, ��������

	// ��Ÿ �߰� ����
public:
	float							StartPayItemWeight;	// FieldServer���� ����Ʈ �ε� �� ����ؼ� �Ҵ��ؾ� ��
	float							EndPayItemWeight;	// FieldServer���� ����Ʈ �ε� �� ����ؼ� �Ҵ��ؾ� ��
	float							EndMissionMasterPayItemWeight; // 2008-12-02 by dhjin, �̼Ǹ����� - // FieldServer���� ����Ʈ �ε� �� ����ؼ� �Ҵ��ؾ� ��
};
typedef vector<CQuest*>		vectCQuestPtr;		// 2007-07-09 by cmkwon, ����Ʈ ��ũ��Ʈ�� ������ vector�� �ޱ� ���� - 

///////////////////////////////////////////////////////////////////////////////
// 2008-06-16 by cmkwon, EP3 ����Ʈ ���� ����Ʈ�� ���� - 
// QuestEpisodeType
#define QUEST_EPISODE_TYPE_1AND2			(BYTE)0		// Episode1 ~ 2
#define QUEST_EPISODE_TYPE_3				(BYTE)1		// Episode3
#define QUEST_INFINITY_TYPE_3				(BYTE)2		// Infinity3		// 2011-03-07 by hskim ���Ǵ�Ƽ 3�� Quest ���� �˸� ���
#define QUEST_EPISODE_TYPE_4				(BYTE)3		// Episode4			// 2011-11-04 by hskim, EP4 [����Ʈ �ý���]

//////////////////////////////////////////////////////////////////////////
// 2008-12-08 by dhjin, �̼Ǹ�����
#define QUEST_PARTYTYPE_NONE				(BYTE)0			
#define QUEST_PARTYTYPE_PARTY				(BYTE)1			

// QuestKind
#define QUEST_KIND_NORMAL					(BYTE)0		// �Ϲ� Quest
#define QUEST_KIND_SIDESTORY				(BYTE)1		// ���� �̼� ,// 2006-03-15 by cmkwon, QUEST_KIND_COLLECTION���� �����Ѱ�
#define QUEST_KIND_SCENARIO					(BYTE)2		// �ó����� Quest

// QuestType
#define QUEST_TYPE_SINGLE					(BYTE)0		// ��ȸ�� Quest
#define QUEST_TYPE_SERIES					(BYTE)1		// ���Ӽ� Quest
#define QUEST_TYPE_REPEATABLE				(BYTE)2		// ���� �ݺ� Quest

// QuestStartType
#define QUEST_START_TYPE_LEVEL_CHANGE		(BYTE)0		// Level ��ȭ �� �Ͼ �� �ִ� Quest
#define QUEST_START_TYPE_EXP_CHANGE			(BYTE)1		// EXP ��ȭ �� �Ͼ �� �ִ� Quest
#define QUEST_START_TYPE_MEET_NPC			(BYTE)2		// Ư�� NPC�� ������ �� �Ͼ �� �ִ� Quest
#define QUEST_START_TYPE_ENTER_BUILDING		(BYTE)3		// Ư�� �ǹ��� ���� �� �Ͼ �� �ִ� Quest
#define QUEST_START_TYPE_ITEM_CLICK			(BYTE)4		// �������� Ŭ�� �� �Ͼ �� �ִ� QUEST
#define QUEST_START_TYPE_COUPON				(BYTE)5		// ���� ����Ʈ - ���� �ݺ�, ���۽� ������ȣ üũ �� �Ϸᰡ �ٷ� �����

// QuestEndType
#define QUEST_END_TYPE_IMMEDIATE			(BYTE)0		// ���۰� �Բ� �ٷ� ������ Quest, Ŭ���̾�Ʈ�� ������ Quest ���� �޽����� ���� �� ���� ��ٸ��� ��
#define QUEST_END_TYPE_REQESTED_BY_CLIENT	(BYTE)1		// Ŭ���̾�Ʈ�� ��û�� �׿� ���� ������ �˻翡 ���� ������ Quest
#define QUEST_END_TYPE_LEVEL_UP				(BYTE)2		// �������ÿ� ����Ʈ�� ����Ǵ� Ÿ��, QuestParam1�� ���� ���� ����� - �ٸ� ���� ���� üũ�� ����, ������������ ��ĥ�� ����
#define QUEST_END_TYPE_TIME_OVER			(BYTE)3		// �����ð�(TimeLimitInMinutes) ����� ����Ʈ�� ����Ǵ� Ÿ�� - �ٸ� ���� ���� üũ�� ����
#define QUEST_END_TYPE_SELECT_INFLUENCE		(BYTE)4		// 2005-08-17 by cmkwon, ���� ���� �̼����� QuestResult�� ������ ������ ���´�.

struct CharacterQuest{
	UID32_t			CharacterUniqueNumber;
	INT				QuestIndex;
	BYTE			QuestState;				// QUEST_STATE_XXX
	INT				QuestParam1;			// �����������̼�(���������������� �ε���), ���� Ÿ���� QUEST_END_TYPE_LEVEL_UP�� �̼�(���� ����) <== ���ļ� ���Ǹ� �ȵȴ�.
	ATUM_DATE_TIME	QuestTimeStamp;			// ����Ʈ ���� �ð�
	LONGLONG		QuestPlayTimeStamp;		// ����Ʈ ���� �ð�(PlayTime)
};							// ĳ������ ����Ʈ ���� ��Ȳ

struct DBCharacterQuestMonsterCount			// 2005-10-19 by cmkwon
{
	UID32_t			CharacterUniqueNumber;
	INT				QuestIndex;
	INT				MonsterUniqueNumber;
	INT				Count;
};

struct CharacterQuestMonsterCount			// 2005-10-19 by cmkwon
{
	INT				MonsterUniqueNumber;
	INT				Count;
};
typedef vector<CharacterQuestMonsterCount>	vectCharacterQuestMonsterCount;


// QusetState
#define QUEST_STATE_NONE			(BYTE)0		// �������� ���� ����
#define QUEST_STATE_IN_PROGRESS		(BYTE)1		// ������
#define QUEST_STATE_COMPLETED		(BYTE)2		// �Ϸ�

#endif // !defined(AFX_QUEST_H__C9FFDA46_B3F8_4FBF_AA50_4547696C4FEF__INCLUDED_)

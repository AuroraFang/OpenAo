#ifndef _ATUM_FIELDSKILLMANAGER_H_
#define _ATUM_FIELDSKILLMANAGER_H_

#include "FieldDataManager.h"
#include "AtumSJ.h"
#include "AtumProtocol.h"
#include "FieldDebuff.h"			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 
#include "FieldDot.h"				// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -

class CFieldItemManager;

////////////////////////////////////////
// class CChargingSkillApplier
////////////////////////////////////////
class CChargingSkillApplier
{
public:
	CChargingSkillApplier(ITEM **o_ppItemforProcessSplashAttack, BOOL i_bSplashAttack, ITEM *i_pAttackItem, CFieldIOCPSocket *i_pFieldIOCPSocket);
	~CChargingSkillApplier();

private:
	CFieldIOCPSocket	*m_pFieldIOCPSocket;
	CParamFactor		m_ChargingSkillParamFactor;
	BOOL				m_bChargingSkillApplied;
};

////////////////////////////////////////
// class CFieldSkillManager
////////////////////////////////////////
class CFieldSkillManager : public CFieldDataManager
{
	friend class CChargingSkillApplier;

public:
	CFieldSkillManager();

	void InitFieldDataManager(CFieldIOCPSocket *i_pFieldIOCPSocket);

	void ResetSkillData();
	void ResetTimeLimitSkillData();					// 2008-02-25 by dhjin, �Ʒ��� ���� - �ð������� ��ų �ʱ�ȭ
	void ResetToggleSkillData();					// 2008-02-25 by dhjin, �Ʒ��� ���� - ��� ��ų�� �ʱ�ȭ
	void ResetChargingSkillData();					// 2008-02-25 by dhjin, �Ʒ��� ���� - ��¡ ��ų�� �ʱ�ȭ

	BOOL IsSkillActivatedByUID(UID64_t i_nSkillUID);
	BOOL IsSkillActivatedByItemNum(INT i_nSkillItemNum);
	INT IsSkillActivatedBySkillBaseNum(INT i_nSkillItemNum);
	BYTE GetSkillActivatedLevelBySkillBaseNum(INT i_nSkillItemNum);	// 2013-03-12 by jhseol, ��ų �ѹ��� �ý��� ����

	BOOL ApplyAllPermanentAndAtivatedSkills();

	ITEM_SKILL* GetActivatedSkillByUID(UID64_t i_nSkillUID);
	ITEM_SKILL* GetActivatedSkillByItemNum(INT i_nSkillItemNum);

	void GetActiveatedSkillByDesParam(mtvectorItemID *pvecItemID, DestParam_t desSame);		// 2013-05-09 by hskim, ���� ����Ʈ ����

	BOOL UseSkill(ITEM_SKILL* i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill);
	// 2009-12-11 by cmkwon, ������ ��׷η� Ÿ���� �����ϴ� ���� ���� - �Ʒ��� �Լ��� �����߰�(, setClientIndex_t *o_pMonListForSkillAggro)
	BOOL UseSkillTypeClick(ITEM_SKILL *i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill, vector<ClientIndex_t> *i_pVectorClientIndex, setClientIndex_t *o_pMonListForSkillAggro);
	BOOL UseSkillTypeTimeLimit(ITEM_SKILL *i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill, vector<ClientIndex_t> *i_pVectorClientIndex, setClientIndex_t *o_pMonListForSkillAggro);
	BOOL UseSkillTypeToggle(ITEM_SKILL *i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill, setClientIndex_t *o_pMonListForSkillAggro);
	BOOL UseSkillTypeCharging(ITEM_SKILL *i_pItemSkill, MSG_FC_SKILL_USE_SKILL *i_pMsgUseSkill, setClientIndex_t *o_pMonListForSkillAggro);

	BOOL CheckUseSkillTime(ITEM_SKILL* i_pItemSkill, ATUM_DATE_TIME *i_patCurTime);			// 2006-12-05 by dhjin
	BOOL SaveUseSkillTime(ITEM_SKILL* i_pItemSkill);			// 2006-12-05 by dhjin

	BOOL DeactivateSkill(ITEM* i_pItemInfo, BOOL i_bSendMSG);
	BOOL DeactivateSkillByUID(UID64_t i_nSkillUID, BOOL i_bSendMSG);
	BOOL DeactivateSkillByItemNum(INT i_nSkillItemNum, BOOL i_bSendMSG);

	BOOL ApplyChargingSkill(ITEM **o_ppItemforProcessSplashAttack, BOOL i_bSplashAttack, ITEM *i_pAttackItem, CParamFactor *i_pParamFactorForChargingSkill);
	BOOL ReleaseChargingSkill(CParamFactor *i_pParamFactorForChargingSkill);
	BOOL ClearChargingSkill(ITEM *i_pSkillItem=NULL);

	void GetSkillTargets(vector<ClientIndex_t> *i_pVectorClientIndex, ClientIndex_t i_nTargetIndex, UID32_t i_nTargetCharUID, ITEM *i_pSkillItemInfo);
	// 2015-11-03 DevX this function defines which skills are enabled in free war maps
	void GetSkillTargetsFreeWar(vector<ClientIndex_t> *i_pVectorClientIndex, ClientIndex_t i_nTargetIndex, UID32_t i_nTargetCharUID, ITEM *i_pSkillItemInfo);

	BOOL SetSkillParamFactor(ITEM *i_pSkillItemInfo, ClientIndex_t i_SkillUseClientIdx=0 , float i_fIncreasePowerRatio = 1.0f );
	BOOL UnsetSkillParamFactor(ITEM *i_pSkillItemInfo);

	BOOL CheckSkillReqItemKind(ITEM *i_pItemInfo);

	BOOL GetPrepareUseBigBoom(void);						// 2007-07-11 by cmkwon, BigBoom�� PrepareUse ���� ����
	void SetPrepareUseBigBoom(BOOL i_bPrepareUseBigBoom);	// 2007-07-11 by cmkwon, BigBoom�� PrepareUse ���� ����

	///////////////////////////////////////////////////////////////////////////////
	// 2009-04-06 by cmkwon, �ݿ��� ����� ��ų �ý��� ���� - 
	int							m_nSkillConfirmUseUID;
	mtvectSSKILL_CONFIRM_USE	m_mtvectSkillConfirmUseList;
	INT AddSkillConfirmUse(UID32_t i_targetCharcUID, int i_nUsingSkillNum, DWORD i_dwAvailableSec, MAP_CHANNEL_INDEX i_mapChannIdx, D3DXVECTOR3 i_posVec3);
	BOOL PopSkillConfirmUse(INT i_skillConfirmUseUID, UID32_t i_targetCharcUID, int i_nUsingSkillNum, SSKILL_CONFIRM_USE *o_pSkillConfirmUse);
	void CheckExpireAvaibleTick(void);


	///////////////////////////////////////////////////////////////////////////////
	// 2009-08-03 by cmkwon, EP3-4 ��� ���� ��ų ���� - 
	Err_t CancelSkill(ITEM *i_pSkillInfo, MSG_FC_SKILL_CANCEL_SKILL *i_pCancelSkill);

	////////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - 	
	void ApplyMonSkill(ITEM *i_pSkill, MSG_FN_BATTLE_ATTACK_SKILL * i_pMsg);
	void ApplyOrbitSkill(ITEM *i_pSkill, ClientIndex_t i_ClientIdx , float i_fIncreasePowerRatio = 1.0f );
	void ApplyDebuffSkill(ITEM *i_pSkill , float i_fIncreasePowerRatio = 1.0f );
	void ApplyDotSkill(ITEM *i_pSkill, ClientIndex_t i_ClientIdx , float i_fIncreasePowerRatio = 1.0f );
	void ResetDebuffSkill();
	void ResetDotSkill();
	void ReleaseOrbitSkill(ITEM *i_pSkill, BOOL i_bTimeCheck = TRUE);
	void ReleaseDebuffSkill(ITEM *i_pSkill, BOOL i_bTimeCheck = TRUE);
	void ReleaseDotSkill(ITEM *i_pSkill);
	BOOL CheckResistance(CFieldIOCPSocket * i_pTargetUser);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - ���� ������ ��� üũ
	BOOL CheckOnceResistance(CFieldIOCPSocket * i_pTargetUser);			// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ - �� ���� �����ϰ� �������� ���� ������ �߰�
	void ProcessingTimeBombSkill(float i_DamageRate);					// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) - Ÿ�ӹ㽺ų ó��

	void MapBuffUse(ITEM_SKILL *i_pSkillItem);			// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ���� ����

private:
	BOOL DeleteActivatedSkillByItemNum(INT i_nSkillItemNum);

private:
	mtvectorItemID			m_vectorAtivatedSkill;					// 2006-12-28 by cmkwon, ������� AtivatedSkill
	mtvectorItemPtr			m_vecChargingSkill;		// ��¡ ���� �̿��� Skill Item ���
	// 2009-12-11 by cmkwon, ��� ������ ó���� �ʿ� ���� - 
	//vector<ClientIndex_t>	m_vectorTargetClients;

public:
	BOOL					m_bChargingSkill:1;			// 2007-04-02 by cmkwon, A-Gear ��¡ ��ų ��� ����
	BOOL					m_bHyperShot:1;				// 2007-03-29 by cmkwon, A-Gear �����ۼ� ��ų ��� ����
	BOOL					m_bPrepareUseBigBoom:1;		// 2007-07-11 by cmkwon, BigBoom�� PrepareUse ���� ����

	/////////////////////////////////////////////////////////////////////////////
	// 2009-09-09 ~ 2010 by dhjin, ���Ǵ�Ƽ -
	CFieldDebuff			m_FieldDebuff;
	CFieldDot				m_FieldDot;

private:
	CFieldItemManager		*m_pItemManager;
};

#endif // _ATUM_FIELDSKILLMANAGER_H_

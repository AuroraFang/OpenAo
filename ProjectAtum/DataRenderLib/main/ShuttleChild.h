#pragma once


#include "UnitData.h"
#include "TickTimeProcess.h"

#define WEAR_ITEM_NUMBER				9
#define A_GEAR_GUN_LIMIT_ANGLE_UP		25
#define A_GEAR_GUN_LIMIT_ANGLE_DOWN		110
#define OBSERVE_MAX_QUICK				10

// 2011-06-30 by jhahn  ����3�� �ó׸� �ӽ���ġ
#define INFINITY_THIRD_X_POS		5000
#define INFINITY_THIRD_Z_POS		2500
#define INFINITY_THIRD_Y_POS		1200

struct WEAR_ITEM_PARAM_FACTOR
{
	DestParam_t	ArrDestParameter[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];
	float		ArrParameterValue[SIZE_MAX_DESPARAM_COUNT_IN_ITEM];
};

struct USED_ITEM
{
	LONGLONG	UniqueNumber;
	float		fOverHeatTime;
	float		fOverHeatCheckTime;
	BOOL		bIsOverHeat;
};

enum ITEMTYPE
{
	ITEM_PRIMARY_WEAPON,
	ITEM_SECONDARY_WEAPON,
	ITEM_CONTROL_SKILL
};

// ����� ���
enum EVENT_TYPE
{
	EVENT_WARP_IN,
	EVENT_WARP_OUT,
	EVENT_CITY_OUTDOOR,
	EVENT_CITY_OUT_MOVE,
	EVENT_GAME_START,
	EVENT_GAME_END
};

class CClientParty;
class CSkill;
class CCinema;
class CStoreData;
//class CQuestData;
class CShuttleRender;
class CWeaponItemInfo;
class CUnitRender;
class CPkNormalTimer;
class CSkinnedMesh;
class CPetChild;		// 2010-06-15 by shcho&hslee ��ý��� - �� ���� ó��

// 2006-04-04 by ispark
struct INVEN_WEAPON_MESH
{
	CSkinnedMesh*	pInvenWeaponMesh;
	int				nMatIndex;
};
typedef vector<INVEN_WEAPON_MESH>		vectINVEN_WEAPON_MESH;

// 2007-03-20 by dgwoo �ӽ� ������ ���.
struct stOBSERVE
{
	ClientIndex_t	ClientIndex;
	// 2007-03-19 by dgwoo
	CEnemyData		*pEnemyData;			// ������ ���� ���󰡾��� Enemy�� �ּ�.
	D3DXVECTOR3		vObserveUp;
	D3DXVECTOR3		vObserveSide;
	D3DXVECTOR3		vObserveLook;
	D3DXVECTOR3		vObservePos;
	D3DXVECTOR3		vObserveNextPos;
	MSG_FC_CHARACTER_OBSERVER_INFO		ClientInfo;
	stOBSERVE()
	{
		ClientIndex = NULL;
		pEnemyData  = NULL;
	}
};
typedef stOBSERVE stOPERATION;

class CShuttleChild : public CUnitData
{
public:
	CShuttleChild();
	virtual ~CShuttleChild();
	virtual void Tick(float fElapsedTime);
	virtual void Render();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	void InvalidateChat();
	void RestoreChat();
	void SetPos( const D3DXVECTOR3& vPos );
	void HandleInput(float fElapsedTime);
	FLOAT CheckMap(D3DXVECTOR3 vPos);
	BOOL CheckMapIsColl(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel);
	FLOAT CheckMapIsColl(D3DXVECTOR3 vPos, D3DXVECTOR3 vVel,int cont);
	void CheckIndexList(MSG_FC_CHARACTER_GET_OTHER_INFO_OK* pMsg);
	void CheckIndexList(MSG_FC_CHARACTER_GET_MONSTER_INFO_OK* pMsg);
	void CheckMove(MSG_FC_MOVE_OK* pMsg);
	void CheckShadowPos();
	BOOL CheckIsWater(D3DXVECTOR3 vPos);
	void ResortingItem();
	void UseSkillMove(float fElapsedTime);
	void SetMoveType(D3DXVECTOR3 vPos,float fHeight);
	void ChangeEngine(CItemInfo* pItemInfo, int nCurrentBURN);
	void UpdateEngine(CItemInfo* pItemInfo);
	// 2010. 03. 25 by jskim Speed, Angle �߰� ���
	void RecalculateSpeedAndAngle();
	//end 2010. 03. 25 by jskim Speed, Angle �߰� ���
	void InitEffect();// 2003.4.23 by dhkwon, ����Ʈ �ʱ�ȭ �Լ�
	void CheckGroundEffect();
	void InitShuttleData(BOOL bState = FALSE);	// �����õ�� ���� ����Ÿ �ʱ�ȭ (bState�� �ʱ� �������� ����)
	void SetShuttleData(BYTE type);				//(0~3->���¼���)0:��ü���� 1:������ 2:���ε� ��Ʈ�� 3:��ü  (4~->�߷�����)4:�Ѱ����߷� 5:���������߷�
	void CheckEventType();						// �̺�Ʈ Ÿ�� üũ
	void CheckEventTypeObject();				// �̺�Ʈ Ÿ�� üũ(������Ʈ)
	BOOL ISSetAutoPos();						// ��Ƽ������ �ڵ� �������� ������ ���ΰ�?
	void WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void CheckMapRegion();
	void SiegeTarget();
	void SetPVPClientIndex(ClientIndex_t PVPClientIndex) {m_nTargetIndex = PVPClientIndex;}
	ClientIndex_t GetPVPClientIndex() {return m_nTargetIndex;}

	void SetWeaponData(BYTE bType,BOOL bReset);//,BOOL bWear);// bType:1~2�� bAct:���� ���� bReset: ���� ���� bWear:���� or ������
	void SetShuttleInputType(BYTE bType){m_bMoveInputType = bType;}
	void SetShuttleFirstPosition(BOOL bMode = FALSE);
	void SetWeaponCountPrimary(INT nClientIndex, INT nCount);
	void SetWeaponCountSecondary(INT nClientIndex,INT nCount = -1);
	void Observe_PosSeting();
	void Move_Observe();

	void InitItemPoint();


	void SetShuttleFlight();												// Numpad0, Backspace �Է¹���
	void SetShuttleFlightInit(float fElapsedTime,bool bType = true);							// ��Ʋ�� �������߱�
//	void Move_B_Gear(float fElapsedTime);	
//	void Move_M_Gear(float fElapsedTime);
//	void Move_A_Gear(float fElapsedTime);
//	void Move_I_Gear(float fElapsedTime);
	void Move_Air(float fElapsedTime);		//Move_B_Gear(float fElapsedTime)
	void Move_Ground(float fElapsedTime);	//Move_A_Gear(float fElapsedTime)
	void Move_AirSiege(float fElapsedTime);		//Move_A_GearSiege(float fElapsedTime)

	void CheckBoosterState(float fElapsedTime,bool bSet,int nOldBoosterType);
//	void CheckAirBreakState(float fElapsedTime,bool bSet);
//	void CheckMoveRate(float fElapsedTime,int nSideMove, int nFBMove);
	void CheckMoveRate(float fElapsedTime,int nSideMove);
	void CheckCollForMap();
	void CheckCollForObject(float fElapsedTime);
	BOOL CheckCollForObjectFrontByDistance(float fElapsedTime, D3DXVECTOR3 vNewPos, D3DXVECTOR3 vOldPos);
	void SendPacket(int nType,UINT64 n = 0);
	void CheckAniTime();
	void CheckBoosterAni();
	void CheckMouseDir();
	void CheckTarget();
	void OldCheckTarget();
	void Old2CheckTarget();
	void NewCheckTarget();
//	void CheckObjectRange(D3DXVECTOR3  vOldPos, float fElapsedTime);//, BOOL	bCollType);

	void CheckClientQuestList();
	void CheckMonsterMove(CMonsterData * pMon);
	BOOL MoveOrder(BYTE bType);
	BOOL SetOrderMoveTargetPos(BOOL bObjectLanding=TRUE);

	void SetCursorInit();

	// 2005-03-31 by jschoi
	void SetEffectPos(float fElapsedTime);
	void SetMatrix_Move_Air(float fElapsedTime);	// void SetEffectPos_B_Gear(float fElapsedTime)
	void SetMatrix_Move_Ground(float fElapsedTime);	// void SetEffectPos_A_Gear(float fElapsedTime)

	// remodeling, 2004-03-21 by dhkwon
	void SendFieldSocketChangeBodyCondition(ClientIndex_t nClientIndex, BodyCond_t hyBody);
	void SendFieldSocketRequestEventObjectWarpIn(CObjectChild * pObj);
	void SendMoveWeaponVel();
	void PrimaryBulletReloaded(MSG_FC_BATTLE_PRI_BULLET_RELOADED* pMsg);
	void SecondaryBulletReloaded(MSG_FC_BATTLE_SEC_BULLET_RELOADED* pMsg);

	// 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����
	//virtual void CheckAttack(ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM=NULL);
	virtual void CheckAttack( ATTACK_DATA &attackData, BOOL bReset, ITEM* pWeaponITEM = NULL, ITEM* pEffectItem = NULL );
	// end 2009. 08. 27 by ckPark �׷��� ���ҽ� ���� �ý��� ����

	virtual USHORT GetShiftWeaponBodyconditionByUnitKind();
	virtual void CreateSecondaryShieldDamage(D3DXVECTOR3 vCollPos );

	void ChangeBodyConditionFromServer(BodyCond_t hyBodyCondition);

	void SetUnitState();

	// ������ǥ ���ϱ� 2004-06-07 ydkim
	void SetTargetVectorforLandedMove(void);
	void SetUnitMovingAnimation(float fElapsedTime);
	BOOL ScanEnemyFrontBAckAndSpace(void);

//  ��ų�� ���� ���� 2004.06.17 jschoi 
	BOOL InitCinemaSkill(int nSkillItemNumber);
	BOOL InitCinemaCamera(int nCameraType);
	void DeletePattern();

//	���� ������ ���� ���� �Լ� 2004-07-27 jschoi
	void UsedItemTick(float fElapsedTime);		 
	void PutUsedItem(CWeaponItemInfo* pWeaponItemInfo);
	USED_ITEM* GetUsedItem(LONGLONG UniqueNumber);
	void DeleteUsedItem();

	BOOL InitCinemaUnit(int nCinemaType, BOOL bUpUse);			// 2005-08-12 by ispark
	// 2010. 05. 27 by jskim �ó׸� ���� ī�޶� ����
	BOOL InitCinemaUnit(int nCinemaType, BOOL bUpUse, D3DXVECTOR3 vPosition, D3DXVECTOR3 vDirection);			// 2005-08-12 by ispark
	BOOL InitCinemaCamera(int nCameraType, D3DXVECTOR3 vPosition, D3DXVECTOR3 vDirection);
	void InfinityCinema(BOOL bSuccess);
	//end 2010. 05. 27 by jskim �ó׸� ���� ī�޶� ����
	void ChangeUnitStateDead();

	BOOL CheckCollMapObject();


	void TryLandingState();			// ���� �õ�
	void SetPKMode(BOOL bSet);

	// Tick function
	void UnitInfoBarSmoothRepair(float fElapsedTime);
	void TurnOnWall(float fElapsedTime);
	void TickCity(float fElapsedTime);
	void MoveLandingStation(float fElapsedTime);
	void TickBurn(float fElapsedTime);
	void TickWeapon(float fElapsedTime);
	BOOL TickChangeClientArea();

	DWORD GetLastAttackTick();
	const CHARACTER* GetShuttleInfo();
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	void SetWearRareItemToCharacterParamFactor(int nPos, CItemInfo* pItemInfo);
	//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	//void SetWearItemToCharacterParamFactor(int nPos, ITEM* pITEM);		// ����/�����ÿ��� ���,ITEM�� ParamFactor�� CHARACTER�� �����Ѵ�.
	//void ChangeWearItemToCharacterParamFactor(int nPos, ITEM* pITEM);		// ����/�����ÿ��� ���,ITEM�� ParamFactor�� CHARACTER�� �����Ѵ�.
	void SetWearItemToCharacterParamFactor(int nPos, CItemInfo* pItemInfo);		// ����/�����ÿ��� ���,ITEM�� ParamFactor�� CHARACTER�� �����Ѵ�.
	void ChangeWearItemToCharacterParamFactor(int nPos, CItemInfo* pItemInfo);		// ����/�����ÿ��� ���,ITEM�� ParamFactor�� CHARACTER�� �����Ѵ�.
	//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	void ReleaseWearItemToCharacterParamFactor(int nPos);	// ����/�����ÿ��� ���,ITEM�� ParamFactor�� CHARACTER���� ����.
	void FieldSocketCharacterSendParamfactorInRange(int nPos);
	void SetParamFactorDesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue);
	void ReleaseParamFactorDesParam( CParamFactor &paramFactor, int nDestParam, float fDestValue);
	void SetShuttleSpeed();
	void SetGroundTargetPos();
	void SetAirGroundTargetPos();											// 2006-12-01 by ispark, ���� ����
	D3DXVECTOR3 GetGroundTargetPos() { return m_vGroundTargetPos; }
	void PatternWarpIn();
	void PatternWarpOut(BOOL bOtherMap);	
	BOOL IsWarpGateZone() { return m_bWarpGateZone; }
	BOOL CheckPickingTarget(CUnitData* pTarget);

	// �Ѹ�
	void CheckRollKeyDown(float fElapsedTime);									// Ű�� ó��	// 2005-07-05 by ispark
	float RollingSystem(float fElapsedTime);								// �Ѹ� �ý���	// 2005-07-05 by ispark
	void CheckRoll(BYTE byDir);
	void InitRolling();

	// 2005-07-11 by ispark
	// �̻��� ���
	void SetMissileWarning(BOOL bMissileWarning) { 	m_bMissileWarning = bMissileWarning;}
	void SetSkillMissileWarning(BOOL bSkillMissileWarning) { m_bSkillMissileWarning = bSkillMissileWarning; }
	BOOL GetSkillMissileWarning()							{return m_bSkillMissileWarning;} 
//	BOOL GetMissileWarning() { return m_bMissileWarning; }
	BOOL IsMissileWarning() { return (m_bMissileWarning && !m_bSkillMissileWarning);}
	void SetMissileCount(int count) { m_nMissileCount = count; }
	int	 GetMissileCount() { return m_nMissileCount; }
	void InitMissileWarning();
	
	void RenderMirror(POINT *pMirrorPos=NULL);
	void UnitEffectRender(D3DXMATRIX pMatUnit);

	// 2005-10-12 by ispark
	// PK Ÿ�� (Enemy�� ����)
	BOOL IsEnemyPKTarget(int nTagetIndex);
	BOOL IsEnemyPKAttackTime(int nTagetIndex);
//	BOOL IsInfluenceWarType(int nEnemyIndex);

	void CheckBodyConditionToServer(BodyCond_t hyBodyCondition);

	BOOL GetIsUseInterface();
	void CheckCollForObject2(float fElapsedTime);
	BOOL WarpSkipInOutAirPort();
	void SetRenderInven(BOOL bFlag){m_bRenderInven = bFlag;}
	void InitCharacterToShuttleData();
	void MirrorTurnLeft(){m_fRotationX = m_fRotationX+0.05f;}
	void MirrorTurnRight(){m_fRotationX = m_fRotationX-0.05f;}
	void MirrorTurnUp(){m_fRotationZ = m_fRotationZ+0.05f;}
	void MirrorTurnDown(){m_fRotationZ = m_fRotationZ-0.05f;}
	void MirrorTurnOrig(){m_fRotationX = SHUTTLE_ROTATION_DEFAULT_X; m_fRotationZ = SHUTTLE_ROTATION_DEFAULT_Z;}
	BOOL GetRenderInvenFlag(){ return m_bRenderInven;}

	void CheckDamageBodyCondition();

	void SetAmorColor(int nAmorColor) { m_nArmorColorIndex = nAmorColor; }
	int	 GetAmorColor() { return m_nArmorColorIndex; }
	void SetInvenAmorColor(int nInvenAmorColor) { m_nInvenArmorColorIndex = nInvenAmorColor; }
	int	 GetInvenAmorColor() { return m_nInvenArmorColorIndex; }
	
	void SetInvenMesh(int nIndex, CSkinnedMesh* pMesh);
	void InitInvenMesh();

	void SendDeadMsg();
	BOOL IsRevivalUseItem();
	void SetSelectCharacterItem();
	void InitWarpShuttle();
	void InitWarpShuttle(MAP_CHANNEL_INDEX channelIndex);

	void CheckBazaarInfo(MSG_FC_BAZAAR_INFO_OK* pMsg);

	int	 CheckUnitState();									// 2006-09-19 by ispark, ���� ���� üũ
	void CheckFormationMoveDelete();						// 2006-11-02 by ispark, ��� ���� ���̸� ����

	void SetUnLockOn();

	// 2007-03-23 by bhsohn �Ѹ��ϸ鼭 ���� ���� �ȵǴ� ���� �ذ�
	BOOL IsMyShuttleRolling();

	// 2007-03-27 by bhsohn ������ ���� �������̽� ����
	BOOL IsObserverMode();
	BOOL ChangeObserveUser(int nKey);
	void TickObserve(float fElapsedTime);
	void ObserveUpdateGage(MSG_FC_CHARACTER_OBSERVER_INFO *pMsg);
	void ObserveEnd();
	void ObserveCancelUpdateInfo();

	void TickOperation(float fElapsedTime);

	// 2007-06-15 by dgwoo ���۷��̼� ����ΰ�?
	BOOL IsOperation()						{return m_bOperation;}
	void SetOperation(BOOL bOperation)		{m_bOperation = bOperation;}

	BOOL IsOperAndObser();
		

	// 2007-04-17 by bhsohn ����� �Ÿ������� ������� ������ �ȵ��� ���� ó��
	BOOL IsAGearFollowEnemy();

	// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
	void	UseScanItem(BOOL i_bScan, float i_fCheckEnableTime, float i_fScanRange);
	void	CreateScanItemEffect(CUnitData *pUnit, 
										D3DXVECTOR3 vPos, D3DXVECTOR3 vVel, D3DXVECTOR3 vUp, 
										float i_fCheckEnableTime,
										UINT i_uEffectId);

	void	UseScanItem(BOOL i_bScan, float i_fCheckEnableTime, float i_fScanRange,INT EffectChange);

	ClientIndex_t GetTargetClientIndext();
	ClientIndex_t GetTargetToClientIdx(CAtumData* pTarget);

	void	UnitStop();				// ������ ���� ��ų���.
	void	InitUnitStop();			// ������ �����̰� �Ұ��.

	// 2007-05-15 by bhsohn A��� ����
	float GetRasingCheckTime();

	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	void SetMyShuttleFullStatInfo(GEAR_STAT i_MyShuttleFullStatInfo);
	GEAR_STAT GetMyShuttleFullStatInfo();
	void ReformMyShuttleStatInfo(GEAR_STAT* pMyGearStat);

	// 2007-05-28 by bhsohn �����ۿ� ���� üũ�� �߰�
	
	// 2009. 05. 29 by ckPark SHA256 üũ�� �ý���
	//BOOL GetCheckSum(UINT *o_puiCheckSum, int *o_pnFileSize, char* pFilePath);
	BOOL GetCheckSum(BYTE o_byObjCheckSum[32], int *o_pnFileSize, char* pFilePath);
	// end 2009. 05. 29 by ckPark SHA256 üũ�� �ý���

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	const CHARACTER& GetMyShuttleInfo() const;
	CHARACTER& GetMyShuttleInfo();

	// 2007-07-04 by bhsohn ���� ����� ��Ż ������ ���� ����
	// ���� ���� �ʱ�ȭ 
	void InitUnitState();

	// 2007-06-12 by bhsohn �̼� �������̽� ������
	PARTYINFO GetPartyInfo();
	BOOL SendGoMissionMap();

	// 2007-07-25 by dgwoo �̻����̳� �Ѿ� �߻�� Ÿ���� �ټ��ִ°�?
	BOOL	IsWeaponeTarget(BOOL i_bSecondW = TRUE);
	void	InitWeaponeTarget();

	// 2007-08-10 by dgwoo �̷� ������� ���(.
	void RenderInvenBack();

	// 2007-11-08 by bhsohn �κ� ����Ʈ ���� ó��
	CEffectInfo* GetEffectWeapon(int nInvenIdx);

	// 2007-12-05 by bhsohn ���� �հ� ���� ���� ó��
	BOOL CheckObjLandingColl(D3DXVECTOR3 vNormalVector );
	BOOL CancelLandingMode();

	// 2008-01-16 by bhsohn A��� ���� ���н� ����,�׶��� ���� ��� ��Ŵ
	void SetShuttleAirMode(BOOL bIsAir);

	// 2008-03-17 by bhsohn ������ �̵��� Tick�ȵ����� ���� ó��
	BOOL IsUnitStop();
	BOOL IsAir();

	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	void SetSecretInfoOption(INT i_nSecretInfoOption);
	void SetAmorHeight(INT i_nAmorNum);

	// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
	D3DXMATRIX GetMatWeaponPosition(int nIdx);	
	int GetInvenWeaponMeshSize();
	INVEN_WEAPON_MESH* GetInvenWeaponMesh(int nIdx);
	float GetRotationX();
	float GetRotationZ();

	// 2008-09-26 by bhsohn �ű� ��þƮ ó��
	void UpdateEngineInfo();
	float GetEnchantDesParam(CItemInfo* pItemInfo, int nDestParam);
	void SetRadarRange(CItemInfo* i_pRadarItemInfo);
	CParamFactor* GetShuttleParamFactor();

	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	BOOL IsPossibleRolling();
	void StartRolling(int nKeyIndex);
	void CheckJoystickKeyDown();
	void SetJoystickUnLockOn();
	void OnKeyDownUnitStop();		// ���� ����
	void OnKeyDownJoinFormation();	// ��� ���� ����
	void SetAirCursorJostickPos();
	BOOL IsPossibleJoystickMove();	// ���̽�ƽ �����̴��� ����

	// 2008-12-04 by bhsohn �������� �Ѿ� ������ ���� ó��
	BOOL	IsWeapongShotting();

	// 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����
	void SetBackView(BOOL bUpDown);
	// end 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����

	// 2010. 03. 25 by ckPark ����� 5���̻� �߶��ϸ� ��Ȱ�޼��� �߰�
	void	SetDeadMessageEnable( BOOL bDeadMessageEnable );
	// end 2010. 03. 25 by ckPark ����� 5���̻� �߶��ϸ� ��Ȱ�޼��� �߰�

	// 2010-06-08 by dgwoo, ��ý��� �߰�. 
	void	SetPartner(char* i_szPartner,BOOL i_bCharacter = FALSE);
	// 2010-06-08 by dgwoo, ��ý��� �߰�. 

	BOOL	PutPetInfo(MSG_FC_ITEM_PET_BASEDATA* pMsg);	// 2010-06-15 by shcho&hslee �� �ý��� - �� ���� ���� �ޱ�.

	// 2011-05-31 by jhahn	����3���ó׸� ����
	void Infinity3Cinema(short bSuccess);
	// end 2011-05-31 by jhahn	����3���ó׸� ����
// 2012-09-20 by jhahn �κ������� ��Ʈ�� ������ ��밡��
	void	PetUseItme();

	//2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	BOOL HpCharge;
	BOOL ShieldCharge;
	BOOL SpCharge;							  
	//end 2011-10-06 by jhahn ��Ʈ�� ������ �ý���		  
//end 2012-09-20 by jhahn �κ������� ��Ʈ�� ������ ��밡��

public :	// Inline Process.

	// �ó׸� ���� ��� ������ Ȱ��ȭ�� ����.	2010. 08. 25. by hsLee.
	inline BOOL IsShowCinema ( void ) { return m_bShowCinema; }

	// 2011-06-30 by jhahn  ����3�� �ó׸� ��ŵ����
	inline BOOL ReturnInfistate(void) { return m_InfiState;} 
	//end 2011-06-30 by jhahn  ����3�� �ó׸� ��ŵ����

	// �ó׸� ���� ��� Ȱ��ȭ�� ����.	2010. 08. 25. by hsLee.
	inline void SetShowCinema ( BOOL bEnable ) { m_bShowCinema = bEnable; }
	// 2010-10-14 by jskim �Ѹ��� ������� ���� ��������� Ÿ�� �������� ���� ����
	void SetKeyDownADouble(BOOL ADouble) { m_bKeyDownADouble = ADouble; }
	BOOL GetKeyDownADouble() { return m_bKeyDownADouble; }	
	void SetKeyDownDDouble(BOOL DDouble) { m_bKeyDownDDouble = DDouble; }
	BOOL GetKeyDownDDouble() { return m_bKeyDownDDouble; }

	// 2012-12-04 by jhjang �ν��� ��� ���� �Ѹ� ���Է� ���� ����
	void ActiveKeyDownADouble();
	void ActiveKeyDownDDouble();
	// end 2012-12-04 by jhjang �ν��� ��� ���� �Ѹ� ���Է� ���� ����
	// end 2010-10-14 by jskim �Ѹ��� ������� ���� ��������� Ÿ�� �������� ���� ����

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	INT GetInSameMapPartyMemberCount();
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	// 2007-11-22 by bhsohn �Ʒ��� ���ռ���
	UID32_t	GetCharacterUniqueNumber();
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������

	// 2012-12-17 by bhsohn A��� �踮�� ����Ʈ ������ ������� ���� ó��
	//void UpdateSkillEffectInfo();
	// 2013-02-14 by mspark, M��� ���� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����
	void UpdateSkillEffectInfo(int nSkillNum);
	// end 2013-02-14 by mspark, M��� ���� ��ų �����Ŀ� ���濡�� ������ �ʴ� ���� ����
	// END 2012-12-17 by bhsohn A��� �踮�� ����Ʈ ������ ������� ���� ó��

	// 2013-03-06 by bhsohn ���� ���� �ý���
	virtual void SetRtnGameUser(BYTE byUsingReturnItem) {m_myShuttleInfo.bUsingReturnItem = byUsingReturnItem;}
	virtual BYTE GetRtnGameUser() {return m_myShuttleInfo.bUsingReturnItem;}
	// END 2013-03-06 by bhsohn ���� ���� �ý���

	BodyCond_t ShuttleChangeBodyCondition();			// 2013-06-25 by ssjung ������ ����Ʈ�� �ȳ����� ���� ����

protected : 

	void CheckWearItem(float fElapsedTime);
	void SetFormationMove(float fElapsedTime);
	
	void CheckOnObject();
	void SetWearItemParamFactor(int nPos, ITEM* pITEM);
	HRESULT SetResourceFile(char* szFileName);
	DataHeader * FindResource(char* szRcName);
	void RemoveEnemyTarget();
	// 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����
	//void SetBackView(BOOL bUpDown);
	// end 2009-03-31 by bhsohn 1��Ī ���� ���� �����, ���� ����

	// 2007-04-02 by bhsohn Move��Ŷ���� ���� ���� üũ
	void UpdateEnemyState(CEnemyData *pEnemyData, MSG_FC_MOVE_OK* pMsg);
	void SetEmenyState(CUnitData* pUnit, INT ItemNum);
	void ClearEmenyState(CUnitData* pUnit, INT ItemNum);

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	BOOL IsCheckObjectColl(CAtumData *pTarget);
	//BOOL GetResolutionPos(DWORD i_dwKind,float &fPosX,float &fPosY,float &fPosZ,float &fScaling);// 2008-08-22 by bhsohn EP3 �κ��丮 ó��


	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	void SetCursorJostickPos();
	int  SetCursorJostickLeftRight(int nLeftRightIndex, int nMaxX);
	int  SetCursorJostickUpDown(int nUpDownIndex, int nMaxY);
	float GetJoystckMoveValue(float i_fValue);

	// 2008-12-16 by bhsohn A��� ���߽��� ��� ���׷��̵�
	void GetSiegeUpVelVector(D3DXVECTOR3	*o_vWeaponVel,	D3DXVECTOR3* o_vWeaponUp);
	void GetAirSiegeUpVelVector(D3DXVECTOR3	*o_vWeaponVel,	D3DXVECTOR3* o_vWeaponUp);

	// 2009-02-19 by bhsohn ī�޶� �ü� ������, ���� ��� ��� ����
	void TickCheckGearAttackStat();
	BOOL IsPossibleAttackStat();
	// end 2009-02-19 by bhsohn ī�޶� �ü� ������, ���� ��� ��� ����

	// 2013-06-27 by bhsohn M���[����������] �浹üũ �ȵǴ� �κ� ó��
	void UpdateSafeUpVector(D3DXVECTOR3	vOldUpTmp);
	void CheckUpNVelVector(D3DXVECTOR3	vAirVel, D3DXVECTOR3	vOldUpVec);
	// END 2013-06-27 by bhsohn M���[����������] �浹üũ �ȵǴ� �κ� ó��

public:
	CGameData	*		m_pGameData;
	CStoreData			*m_pStoreData;
	CUnitRender*		m_pRender;							// Shuttle�� Rendering ó��
	// 2007-03-20 by dgwoo �ӽ� ������ ���.
	stOBSERVE			m_stObserve;						// �������� ������ ����.and ����.
	BOOL				m_bObserve;							// ������ ����ΰ�?
	ClientIndex_t		m_ObserveQuick[OBSERVE_MAX_QUICK];	// ������ ����Ű 1~0 ������ �̸� �� �ε��� ��ȣ�� �������ִ´�.

	D3DXVECTOR3			m_vTargetPosition;
	D3DXVECTOR3			m_vGroundTargetPos;					// ���� ���ݽ� Ÿ�� ������
    D3DXVECTOR3			m_vBasisUp;							// Basis Up Vector
	FLOAT				m_fSideRate;						// �¿� ȸ�� �ӵ� ���� �� �����
	FLOAT				m_fSideCheckRate;					// �¿� ȸ�� �ӵ� �� ����� ��������(������ ������ ���߱� ����)
	FLOAT				m_fHeightRate;						// �� ���� ����
	FLOAT				m_fLRVelRate;						// �¿� ���ӵ��� ������ �� ����
	FLOAT				m_fOldUpRate;						// ���� �����͸� ����Ͽ� �ε巴�� ����δ�(�¿� �̵� ����Ӱ� ȸ�� ����� ����)
	
	FLOAT				m_fShuttleSpeed;					// ��Ʋ�ӵ� ���� (������ �¿�ȴ�)		
	FLOAT				m_fShuttleSpeedMax;					// ��Ʋ�ӵ� �ִ� (������ �¿�ȴ�)		
	FLOAT				m_fShuttleSpeedMin;					// ��Ʋ�ӵ� �ּ� (������ �¿�ȴ�)		
	FLOAT				m_fShuttleSpeedBoosterOn;			// ��Ʋ�ӵ� �ν��� ��� (������ �¿�ȴ�)
	FLOAT				m_fShuttleGroundSpeed;				// ��Ʋ�ӵ� �� �ӷ� (������ �¿�ȴ�)
	FLOAT				m_fShuttleGroundSpeedMax;			// ��Ʋ�ӵ� �� �ӷ� �ִ� (������ �¿�ȴ�) // 2005-11-26 by ispark
	BYTE				m_bySpeedPenalty;					// ������ȸ���ӷ�	// 2005-08-16 by ispark

	FLOAT				m_fRealShuttleSpeedMax;				// ���� ��Ʋ �ִ� �ӷ�
	FLOAT				m_fRealShuttleSpeedMin;				// ���� ��Ʋ �ּ� �ӷ�
	FLOAT				m_fRealShuttleSpeedBoosterOn;		// ���� ��Ʋ �ν��� �ӷ�

	FLOAT				m_fBoosterCheckTime;				// �ν��� ��� üũ Ÿ��
	FLOAT				m_fChangeBodyForBoosterCheckTime;	// �ν��� ���� ���� üũ Ÿ��
	INT					m_nShuttleBoosterState;				// �ν��͸� Ű�� ���� ����Ŭ��üũ ����
	INT					m_nShuttleStopState;				// ���ߺξ��� ���� ����Ŭ��üũ ����

	INT					m_nFBDir;							// �յ� ����
	INT					m_nLRDir;							// �¿� ����
	INT					m_nSideRotateDir;					// �¿� ����
	
	CHARACTER			m_myShuttleInfo;					// Shuttle �� ����
	FLOAT				m_fDefenseTotal;					// Shuttle �� Total ����
	FLOAT				m_fDefenseForItem;					// Shuttle �� Item ����
	FLOAT				m_fDefenseForSkill;					// Shuttle �� Skill ����
	FLOAT				m_fDefenseForShuttle;				// Shuttle �� ��ü ����
	FLOAT				m_fTransportTotal;					// Shuttle �� Total �߷�
	FLOAT				m_fTransportCurrent;				// Shuttle �� Current �߷�

	BYTE				m_bCollObjectCheck;					// �浹 üũ�ؾ� �ϴ°�?(������Ʈ, ����, Ÿ ĳ����)
	BYTE				m_bCollMonsterCheck;				// �浹 üũ�ؾ� �ϴ°�?(������Ʈ, ����, Ÿ ĳ����)
	BYTE				m_bCollShuttleCheck;				// �浹 üũ�ؾ� �ϴ°�?(������Ʈ, ����, Ÿ ĳ����)
	BYTE				m_bCollAttackCheck;					// Attack �� �浹 üũ�ؾ� �ϴ°�?(������Ʈ, ����, Ÿ ĳ����)

	FLOAT				m_fExpPer;							// ����ġ �ۼ�Ƽ�� 
	/////////////// ��� ���� ��û�� ���� �÷��׿� �ε��� ���� ////////
	BOOL				m_bGetOtherInfo;
	FLOAT				m_fGetOtherInfoCheckTime;
	INT					m_nGetOtherIndex;
	///////////////// ĳ���� �浹 ó�� ���� ���� //////////////////////
	BOOL				m_bColl;							// ���� �� Ÿ ĳ���Ϳ��� �浹
	D3DXVECTOR3			m_vCollPos;							// �浹 ������
	FLOAT				m_fCheckCollTime;
	INT					m_nBooster;							// Booster State
	INT					m_nHyperBooster;					// 2006-10-11 by ispark, ������ �ν���
	BOOL				m_bOnObject;						// ��Ʋ �Ʒ�,���� ������Ʈ�� �ִ���
	FLOAT				m_fAxisRate;						// ��Ʋ�� ����� ���� 
	INT					m_nUDState;							// ���� ���� ���� 
	///////////////// �ڵ� �� ����  /////////////////////
	D3DXVECTOR3			m_vOldPos;

	///////////////// Party  /////////////////////
	CClientParty	*	m_pClientParty;						// ��Ƽ ����

	
	BYTE				m_bCheckMove;						// �̵� Ÿ�� ����	
															// 0:������ ����, 1:2������, 2:1������, 3:��������, 4:�Ϲݺ�������
															// 0~3�� �浹�ÿ� üũ�ǰ� 4�� �浹�� �߻����� ������ ���Ѵ�
	BOOL				m_bIsWater;							// ���� �������ΰ��� ���ΰ�

	BOOL				m_bCollMap;							// �ʰ��� �浹 �Ǵ� ����
	FLOAT				m_fCollMapCheckTime;				// �ʰ��� �浹�� ƨ���� �ö󰡴� �ð��� ������ ����

	D3DXVECTOR3			m_vNormal;							// ���� ���� �븻����

	// Primary, Second, Skill �� �ѹ��� �߻�Ǵ� ��(Amount), �ѹ��� �߻�ǰ� ���� �ϱ����� ������(RATime), �����̰� ����Ǹ鼭 �߻�Ǵ� �ִ� Ƚ��(Cont)

	////////////////////////////////////////////////////////////////////////////////////
	//							1�� 2�� Weapon ���� ������							  //
	////////////////////////////////////////////////////////////////////////////////////
	BYTE				m_bAttackMode;						// ���� ���-0:�Ϲ� 1:���� 2:����
	BOOL				m_bAttack;							// ������ �����Ѱ�. TRUE : ���ݰ��� FALSE : ���ݺҰ���.

	//////////////////////////  Skill Test Key Input (��ũ��) //////////////////////////
	FLOAT				m_fSkillMoveTime;					// �̵� ���ε� ��Ʈ�ѽ� Ÿ�� üũ
	BOOL				m_bSkillMoveIsUse;					// �̵��� ���õ� ���ε� ��Ʈ�� ��� ����
	BOOL				m_bIsCameraPattern;					// ī�޶� ������ ����� �̵����ΰ�? 2004.06.28 jschoi
	BOOL				m_bSkillMoveType;					// ���ε� ��Ʈ�ѹߵ��� Move Type �׽�Ʈ(���¸�� ����)
	D3DXVECTOR3			m_vSkillMoveVel;					// ȸ�� ���ε� ��Ʈ�� �ߵ��� ������ ���õǾ��� ����(���� ������ �����)

	BOOL				m_bTurnCamera;
	CAtumData	*		m_pTarget;							// ���� �����ϰ� �ִ� Ÿ�� ������
	CAtumData	*		m_pOrderTarget;						// Ÿ�� ���ý� ǥ�÷� ����Ű�� �Ǵ� ������
	UINT				m_nCurrentPatternNumber;			// ���� ������� ���� ��ȣ
	BYTE				m_bMoveGroundType;					// ������ ���õ� �̵� Ÿ��
	////////////////////////////////////////////////////////////////////////////////////
	BYTE				m_bEffectGround;					// 0:����  1:�������� 2:��������

	///////////////// DT ���� ���� ���߿� �� �ִ� �������� /////////////////////
	BOOL				m_bIsAir;
	FLOAT				m_fDTEventCheckTime;				// DT �� �̺�Ʈ üũ Ÿ��
	////////////////////////////////////////////////////////////////////////////////////
	FLOAT				m_fNumpad0keyTime;
	FLOAT				m_fCancelSkillCheckTime;
	BYTE				m_bMoveInputType;					// �̵� Ű���� ���	0:Auto 1:Normal
	FLOAT				m_fMouseLRMoveRate;					// ���콺 ��, �� ȸ�� ����
	FLOAT				m_fMouseUDMoveRate;					// ���콺 ��, �� ȸ�� ����

	char				m_strChatPToP[MAX_CHAT_PTOP_CONT][SIZE_MAX_CHARACTER_NAME];
	BYTE				m_bCurPToPPos;
	BYTE				m_bOldPToPPos;

	BOOL				m_bRButtonState;					// ���콺 ���� ��ư ����
	BOOL				m_bLButtonState;					// ���콺 ���� ��ư ����
	BOOL				m_bMButtonState;					// ���콺 �ٹ�ư ����
	BOOL				m_bUpButtonState;					// Ű���� UP ��ư ����
	BOOL				m_bDownButtonState;					// Ű���� DOWN ��ư ����
	BOOL				m_bLeftButtonState;					// Ű���� LEFT ��ư ����
	BOOL				m_bRightButtonState;				// Ű���� RIGHT ��ư ����
	BOOL				m_bSkillGoMove;						// �����ϼ� �ִ� �����ΰ�? T:������, F:��������.

	POINT				m_pOldMousePoint;

	BOOL				m_bUseBooster;						// �ν��� ����� MaxSpeed�� ���ƿö����� �ν��� ������
	BOOL				m_bReChargeBURN;
	FLOAT				m_fBURN;
	FLOAT				m_fCurrentBURN;

	POINT				m_ptOldPoint;

	BOOL				m_bFirstStart;						// ���� ���۽� ���������϶��� ������ ���� ����(�ѹ� Tick�� �������� �ʱ� ����)

	BOOL				m_bAutoLockon;						// �ڵ� ���� ���� 
	BOOL				m_bAutoHeightControl;				// ���� ���� �� ���Ϳ� �ڵ� �� ����
//	BOOL				m_bEventReady;						// �̺�Ʈ��ٸ�����,  �����ص� �ɵ�.
	FLOAT				m_fWeaponMaxDistance;				// 1 ~ 2 �� ������ �����Ÿ��� üũ - ���� �� �Ÿ��� ��
	INT					m_nMonsterCount;					// �ʺ� ���ø�忡���� ���� ���� ǥ���ϱ� ���� ����
//	FLOAT				m_fEventCheckTime;					// �̺�Ʈ üũ Ÿ��

	FLOAT				m_fNextHP;							// ������ HP�� ȸ���ϴ°��� �����ֱ� ���� ����(���� HP)
	FLOAT				m_fNextSP;							// ������ SP�� ȸ���ϴ°��� �����ֱ� ���� ����(���� SP)
	FLOAT				m_fNextEP;							// ������ EP�� ȸ���ϴ°��� �����ֱ� ���� ����(���� EP)
	FLOAT				m_fNextDP;							// ������ EP�� ȸ���ϴ°��� �����ֱ� ���� ����(���� EP)
	FLOAT				m_fNextHPRate;						// ������ HP�� ȸ���ϴ°��� �����ֱ� ���� ����(ȸ�� �ӵ�)
	FLOAT				m_fNextSPRate;						// ������ SP�� ȸ���ϴ°��� �����ֱ� ���� ����(ȸ�� �ӵ�)
	FLOAT				m_fNextEPRate;						// ������ EP�� ȸ���ϴ°��� �����ֱ� ���� ����(ȸ�� �ӵ�)
	FLOAT				m_fNextDPRate;						// ������ EP�� ȸ���ϴ°��� �����ֱ� ���� ����(ȸ�� �ӵ�)

	BOOL				m_bIsSetFlighting;					// ���� ������ ���߰� �ִ� ���ΰ�?
	FLOAT				m_fCollSendDamageCheckTime;			// �����浹�� ������ ������ ���� Ÿ�� ����

	D3DXVECTOR3			m_vMouseDir;						// ���콺 ���� ����
	D3DXVECTOR3			m_vMousePos;						// 2010-06-15 by shcho&hslee ��ý��� - ���콺 ��ġ
	FLOAT				m_fMouseRate;						// ���콺 ���� ��������(ȸ���ӵ� ����)
	POINT				m_pt;								// ���콺 ��ǥ
	POINT				m_ptDir;							// ���콺 ���� ��ǥ
	POINT				m_ptDirOld;							// ���콺 ���� ��ǥ �õ�
	POINT				m_ptMove;							// ���콺 �̵� ��ǥ
	
	vector<USED_ITEM*>				m_vecUsedItem;			// ����ϴٰ� ���� �ƴ��� (1��,2��,��ų)
	
	D3DXVECTOR3			m_vCollCenter;						// �浹�� �߻��� ������Ʈ�� �߽���ǥ
	BYTE				m_bBeginnerQuestDegree;				// �ʺ� ����Ʈ ���� �ܰ� ����

	BOOL				m_bMouseLock;						// ���콺 �� ��� ��ȯ

	BOOL				m_bOrderMove;						// ī�޶� ��Ʈ�ѵ�� ���� ���� �̵� ���� �Է� ���� ���� ����
	BYTE				m_bOrderMoveType;					// ���� �̵� Ÿ��
	D3DXVECTOR3			m_vTargetPos;						// Ÿ������ �̵��ϱ� ���� ��ǥ(�ڵ��̵��� ���δ�)
	D3DXVECTOR3			m_vTargetVel;						// Ÿ������ �̵��Ͽ� �ٶ� ����(�ڵ��̵��� ���δ�)
	FLOAT				m_fLandingCheckTime;				// ���� ���� �̵��� ������ ���� �ӵ�
	FLOAT				m_fLandingDist;						// ���� ���� �̵��� ������ ����������� �Ÿ�
	FLOAT				m_fRasingCheckTime;					// Take_Off �� �����ð� ����� ���� ����

	// 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ������ ����.
	WEAR_ITEM_PARAM_FACTOR	m_wearItemParamFactor[MAX_EQUIP_POS];
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	WEAR_ITEM_PARAM_FACTOR	m_wearPreRareItemParamFactor[MAX_EQUIP_POS];
	WEAR_ITEM_PARAM_FACTOR	m_wearSufRareItemParamFactor[MAX_EQUIP_POS];
	//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����

	/*
	WEAR_ITEM_PARAM_FACTOR	m_wearItemParamFactor[WEAR_ITEM_NUMBER];
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	WEAR_ITEM_PARAM_FACTOR	m_wearPreRareItemParamFactor[WEAR_ITEM_NUMBER];
	WEAR_ITEM_PARAM_FACTOR	m_wearSufRareItemParamFactor[WEAR_ITEM_NUMBER];
	//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	*/
	// End 2010-06-15 by shcho&hslee ��ý��� - ����ǰ �ִ� ������ ����.

	CSkill	*			m_pSkill;

	// 2004-06-03, ydkim, ������ Ÿ�ϴ��� �̵� ����  
    BOOL                m_bAniKeyBlock;						// �ִϸ��̼��� Ű��� 
	BOOL                m_bAgearPortflag;					// A��� ���� �̺�Ʈ���� ����Ȯ�� 
	BOOL				m_bLandingOldCamtypeFPS;			// ������ ķŸ���� 1��Ī�̿��� 

	DWORD				m_dwLastAttackTick;
	
	float               m_fAniFlag;							// �ִϸ��̼� ����Ȯ�� 		
	float				m_fKeyboardInputTimer;				// Ű���� �Է� Ÿ�̸�
	float               m_fMoveFrameTimer;                  // �ʴ� �����̴� �����Ӽ�
	float               m_fFrameLerp;                       // ������ ������ 
	float				m_fAnimationMoveTimer;				// �ִϸ޿��� �� ������ Ÿ�� 
	float				m_fSinMove;							// ������ �ε巴��

//	POINT				m_pAutoTargetOldMousePos;			// ��������忡�� ���� �ڵ����� ���� ���콺����Ʈ �̵� ������ 


	D3DXVECTOR3         m_vMyOldPos;						// ����Ű�� �������� ���� ��ġ 
	D3DXVECTOR3			m_vAniFrame;						// ������ ��ǥ�� 
	BOOL				m_bReflexionKeyBlock;				// ���� �������� Ű�� 

	CCinema		*		m_pCinemaUnitPattern;				// ����(��ų,����) ����
	CCinema		*		m_pCinemaCamera;					// ī�޶� ����
	
	EVENT_TYPE			m_nEventType;

	float				m_fWarpOutDistance;					// ���� �̺�Ʈ ������Ʈ ���� �Ÿ�(���� �ƿ���)
	D3DXVECTOR3			m_vWarpOutPosition;					// ���� �ƿ��� ���� ��ġ 

	CObjectChild *		m_pCurrentObjEvent;					// m_nEventType == EVENT_WARP_IN �ÿ� ���
	CObjectChild *		m_pCurrentObjEventOld;
	CWeaponItemInfo*	m_pPrimaryWeapon;
	CWeaponItemInfo*	m_pSecondaryWeapon;	

	MSG_FC_GUILD_GET_WAR_INFO_OK	*	m_pGuildWarInfo;		// �������ÿ� �� ������ ���õȴ�.
	USHORT				m_nOldMapIndex;
	
	// Ŭ���̾�Ʈ Tick Time ���
	CTickTimeProcess    m_timeProcess;
	
//#ifdef _DEBUG
//	int					m_nDebugWarpMove;
//	int					m_nDebugSendPacketMove;
//	float				m_fDebugWarpCheckTime;
//	float				m_fDebugSendCheckTime;
//#endif
//	float				m_fMissileFireTime;

	// 2005-02-15 by jschoi  -  PK Delay Timer
	CPkNormalTimer*		m_pPkNormalTimer;	
	BOOL				m_bCtrlKey;		// CtrlKey ���� ���� TRUE : Ctrl Down ����, FALSE : Ctrl Up ����

	BYTE				m_byCityWarTeamType;	// ���������� ���� ����
	float				m_fTempGravity;			// �߷� �� 
	float				m_fRollTime;			// �Ѹ� �ý��� ���� �ð�			// 2005-07-11 by ispark
	BOOL				m_bRollUsed;			// 2005-07-14 by ispark �Ѹ� ������ΰ�
	
	// // 2005-07-21 by ydkim 
	int					m_nEventTypeAirPort;	// ������ ���� ����Ÿ��
	int					m_nEventIndex;
	BOOL				m_bKeyBlock;
	BOOL				m_bWarpLink;			// ������ ���� ���� ����
	BOOL				m_bUnitStop;
	BOOL				m_bPenaltyUnitStop;		// 2005-08-09 by ispark �������� ���Ƽ - ��߽� ����
	BOOL				m_bMouseMoveLock;		// ���콺 �̵� ���� // 2005-08-09 by ispark

	CItemInfo*			m_pRadarItemInfo;		// 2005-08-16 by ispark ���̴� ������ ����

	// 2005-10-10 by ispark - PK Ÿ��
	vector<PK_TARGET *>	m_vecPKTarget;
	int					m_bPKState;				// TRUE : ����, FALSE : �����

	BOOL				m_bLandedMove;			// Landed�� Ű�� �� ������ �� �ִ� ����
	BOOL				m_bSkipPattern;			// ���� ��ŵ
	
	// 2008-08-22 by bhsohn EP3 �κ��丮 ó��
//	BOOL				m_bInvenRender;
//	D3DXMATRIX			m_pMatInven;
//	D3DXMATRIX			m_pMatInvenWeaponSetPosition[4];
//	D3DXMATRIX			m_pMatInvenWeaponOrgPosition[4];// 2006-01-03 by ispark, ���� ������ ��Ʈ����

	float				m_fAdvanceTime;			// �޸� �� üũ Advance

	// 2009-03-16 by bhsohn A��� ���� ���� �������� �ּ�ȭ
	DWORD				m_dwCheckTimeSendMoveVel;
	// end 2009-03-16 by bhsohn A��� ���� ���� �������� �ּ�ȭ

private:
	BOOL				m_bOperation;						// ���� ����ΰ�?

	BOOL				m_bWarpGateZone;
	BOOL				m_bTargetChange;
	BOOL				m_bRenderInven;

	float				m_fCurrentAngle;		// Y�� ��ȸ ����   // 2005.6.13 by dhkwon - ��ȸ ���� ���ӷ�
	float				m_fCurrentSideRate;		// X�� ��ȸ ����

	// 2005-07-05 by ispark
	// �Ѹ� �ý��� Ű ó��
	BOOL				m_bKeyDownA;			// A ���� �Ѹ�
	BOOL				m_bKeyDownD;			// D ���� �Ѹ�
	BOOL				m_bKeyDownACheck;		// A ���� �Ѹ� ����㰡 üũ
	BOOL				m_bKeyDownDCheck;		// D ���� �Ѹ� ����㰡 üũ
	// 2005-09-26 by ispark
	// �Ѹ� ����(A, DŬ���� �¿� �̵� ��Ȱ, ����Ŭ���� �Ѹ�)
	BOOL				m_bKeyDownADouble;		// A ����Ŭ��
	BOOL				m_bKeyDownDDouble;		// D ����Ŭ��
	float				m_fADoubleKeyTime;
	float				m_fDDoubleKeyTime;

	float				m_fRollMessageTime;		// �Ѹ� �޼��� ������ �ð� üũ
	float				m_fRollAngle;			// �Ѹ� ȸ�� ����
	float				m_fRollLength;			// �Ѹ� �¿� �̵� �Ÿ� üũ
	float				m_fRollDownSpeed;		// �Ѹ� �ٿ� ���ǵ�
	D3DXVECTOR3			m_vRollStart;			// �Ѹ� ������ ��ǥ
	D3DXVECTOR3			m_vRollOldPos;			// �Ѹ� ��ǥ �̵��� �� ��ǥ

	BOOL				m_bMissileWarning;		// �� 2������ ������
	BOOL				m_bSkillMissileWarning;	// ��ų�� ���� �̻��� ��� ����
	int					m_nMissileCount;		// �� 2������ ����

	BOOL				m_bLandingField;		// �������ΰ� (A�� �����߿��� �ƹ� ó�� ���ϱ� ���ؼ�)
	
	// �κ��丮
//	LPDIRECT3DVERTEXBUFFER9 m_pMirrorVB;
	LPDIRECT3DTEXTURE9      m_pTexturesBackInven;
	
	float				m_fRotationX;
	float				m_fRotationY;
	float				m_fRotationZ;	
	float				m_fRotationA;

	BOOL				m_bSkillAirMove;		// ��ų ���� ���� �����̴�.

	float				m_fCheckWearItemTime;	// üũ ���� ������
	int					m_nCheckDamage;			// üũ ������

	float				m_fCheckDead;			// �׾����� üũ

	int					m_nArmorColorIndex;		// 2006-02-17 by ispark, Į�� �ε���
	int					m_nInvenArmorColorIndex;// 2006-02-17 by ispark,�κ� Į�� �ε���

	//INVEN_WEAPON_MESH	m_stInvenWeaponMesh[6];	// 2006-04-04 by ispark
	vectINVEN_WEAPON_MESH	m_vectInvenWeaponMesh;	// 2006-04-04 by ispark

	BYTE				m_byFormationFlightType;

	// 2007-04-17 by bhsohn ����� �Ÿ������� ������� ������ �ȵ��� ���� ó��
	BOOL				m_bAGearFollowEnemy;		// A��� ���� ���� ���� ���󰡰��ֳ�?

	// 2007-05-15 by bhsohn ��� ���� ���� ó��
	GEAR_STAT			m_myShuttleFullStatInfo;	// ��ü �������� 

	float				m_fUnitX;
	float				m_fUnitY;
	float				m_fUnitZ;
	float				m_fScaling;

	BOOL				m_bNowMousePosWindowArea;
	BOOL				m_bOldMousePosWindowArea;

	FLOAT				m_fAmorHeight;				// ���� �Ƹ��� ����.

	// 2008-11-13 by bhsohn ���̽�ƽ �۾�
	BOOL				m_bSetCursorPos;
	int					m_nJoyStickRolling;	

	// 2009-03-30 by bhsohn ��¡�� ����Ʈ ���� ����
	float				m_fDelCapChagingSkill;

	// 2010. 03. 25 by ckPark ����� 5���̻� �߶��ϸ� ��Ȱ�޼��� �߰�
	BOOL				m_bDeadMessageEnable;

	DWORD				m_dwDeadTime;
	// end 2010. 03. 25 by ckPark ����� 5���̻� �߶��ϸ� ��Ȱ�޼��� �߰�

	// 2010. 05. 27 by jskim �ó׸� ���� ī�޶� ����
	//BOOL				m_ShowInterface;
	BOOL				m_bShowCinema;
	//end 2010. 05. 27 by jskim �ó׸� ���� ī�޶� ����


	//2011-06-30 by jhahn  ����3�� �ó׸� ��ŵ����
	BOOL					m_InfiState;
	//end 2011-06-30 by jhahn  ����3�� �ó׸� ��ŵ����

	// 2013-03-25 by bhsohn ��Ʈ�� ŰƮ �ڵ� ��� ���� ���� ���� ����
	DWORD					m_dwPetUseItemTime_Spell;
	DWORD					m_dwPetUseItemTime_Hp;
	DWORD					m_dwPetUseItemTime_Dp;
	DWORD					m_dwPetUseItemTime_Sp;
	// END 2013-03-25 by bhsohn ��Ʈ�� ŰƮ �ڵ� ��� ���� ���� ���� ����

	// 2013-06-27 by bhsohn M���[����������] �浹üũ �ȵǴ� �κ� ó��
	float					m_fReverserIgnoreTime;
	// END 2013-06-27 by bhsohn M���[����������] �浹üũ �ȵǴ� �κ� ó��

	BOOL					m_bLendSoundPlay;        // 2013-08-07 A�� ���󿡼� ����̺� �� �� ���� �Ҹ��� �Ȳ����� ���� ����
};
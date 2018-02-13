// SceneData.h: interface for the CSceneData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEDATA_H__5325A197_B598_4A01_9E34_946C65760A16__INCLUDED_)
#define AFX_SCENEDATA_H__5325A197_B598_4A01_9E34_946C65760A16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtumNode.h"

///////////////////////////////////////////////////////////////////////////////
/// \class		CSceneData
///
/// \brief		�������� ���ȭ�� ����Ÿ, �ʵ���Ÿ, ����, object, enemy,����,�ð� ��
/// \author		dhkwon
/// \version	
/// \date		2004-03-19 ~ 2004-03-19
/// \warning	
///////////////////////////////////////////////////////////////////////////////

class CCinema;
class CFrustum;
class CBackground;
class CObjRender;
class CObjectChild;
class CMonRender;
class CSunRender;
class CSunData;
class CRainRender;
class CRainData;
class CETCRender;
class CWeapon;
class CItemData;
class CGameData;
class CUnitData;
class CAtumData;
class CItemData;
class CAppEffectData;
class CWater;
struct ALPHA_CHARACTERINFO;

class CSceneData  : CAtumNode
{
public:
	CSceneData();
	virtual ~CSceneData();

	void SetFogLevel(DWORD dwFogColor, float fFogStart, float fFogEnd ); 

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Render();
	virtual void Tick();

	void	RenderCity();
// character, monster functions
	CUnitData * FindUnitDataByClientIndex( ClientIndex_t nIndex );
	int		FindClientIndexByUnitData( CUnitData* pUnit );
// Field Item functions
	void	SendItemGetItemAll();
	void	SendItemDeleteItemAdmin( int x, int y );
	void	DeleteFieldItemOfUnitData( CUnitData* pUnitData );
	void	DeleteFieldItemOfFieldIndex( UINT nFieldIndex );
	void	ExplodingFieldItemOfFieldIndex( UINT nFieldIndex );
	void	AddFieldItemItemSHowItem( MSG_FC_ITEM_SHOW_ITEM *pMsg );
	void	AddFieldItemBattleDropDummyOk( CAtumNode* pUnitData, MSG_FC_BATTLE_DROP_DUMMY_OK* pMsg );
	void	AddFieldItemBattleDropFixerOk(CAtumNode *pTarget,CAtumNode *pAttack,MSG_FC_BATTLE_DROP_FIXER_OK* pMsg);
	void	DeleteFieldItemBattleDropFixerOk(UINT nItemFieldIndex);		// 2004-09-13 jschoi
	CItemData * FindFieldItemByFieldIndex( UINT nFieldIndex );
	CItemData * FindFieldItemBy2DDistance( D3DXVECTOR2 vPos, float fDist );
	CItemData * FindFieldItemByPartTypeAndParent( DWORD dwPartType, CAtumNode* pParent );
	CItemData * FindFieldItemByParent( CAtumNode* pParent );
	CObjectChild * FindWarpGateByPosition( D3DXVECTOR3 vPos , float fDistance = RANGE_OF_VISION );
	int		GetDummyCountOfUnit(CAtumData* pUnit);
// monster weapon functions
	void	CreateWeaponMonsterMissile(MSG_FC_MISSILE_MOVE_OK* pMsg);
//	void	CreateWeaponMonsterSecondary(MSG_FC_BATTLE_ATTACK_RESULT_SECONDARY* pMsg);
// scene init, delete, render functions
	BOOL	InitBackground();
	VOID	InitRes();
	VOID	RestoreRes();
	VOID	InvalidateRes();
	VOID	DeleteRes();
	HRESULT	LoadTex();
	VOID	DeleteTexTileDevice();
	VOID	SetShuttleLandState(CAtumData * pNode);				// ��Ʋ�� ���ӽ�ŸƮ�� ���� Ÿ���� �������������̸� �������·� �����ϰ� �ȴ�.
																// �������� ���ӿ���� �������־���Ѵ�. ���� ���·�...
	VOID	SetupLights();
	VOID	CheckWeather();
	BOOL	ApplyFogDistanceAsHeight( float fOriginStart , float fOriginEnd );
	VOID	CheckDay();
	VOID	SetDay();
	D3DXVECTOR3	SetLightDirection();
	VOID	CheckObjectRenderList(BOOL bCheckRange = TRUE);
	VOID	SetObjectRenderList();
	VOID	DeleteObjectList(int nIndex);
	VOID	ChangeResource();
	VOID	InsertToBlockData(CAtumNode * pNode);
	VOID	ChangeToBlockData(CAtumNode * pNode);
	VOID	DeleteToBlockData(CAtumNode * pNode);

	// 2007-02-09 by dgwoo ��ĵ ���� 	
	// 2007-04-19 by bhsohn ��ġ���� ������ �߰�
	//void	AddFieldItemScanObject(CAtumNode * pUnitData, float fCheckTime);
	void	AddFieldItemScanObject(CAtumNode * pUnitData, float fCheckTime, UINT uObjId, DWORD dwPartType, 
										D3DXVECTOR3	vVel, D3DXVECTOR3 vUp, D3DXVECTOR3 *i_pPos=NULL);
	void	TickScanObeject();

	//--------------------------------------------------------------------------//
	
// ���� effect
	CAppEffectData* FindEffect(int nType);
	CAppEffectData* FindEffect(int nType, CUnitData *pParent);
	void DeleteEffect(CAtumNode* pParent);
// mapEnemyList, mapMonsterList
//	void SetExceptAllUnitTarget(); // Ÿ�ٸ���Ʈ���� ���ܽ�Ŵ
	void SetPKSettingGuildWar( int nPeerGuildUniqueNumber, BOOL bPK );
// �ó׸� ����Ÿ ���
	CCinema * LoadCinemaData(char* szFileName, int nFileName = -1);
// �ֺ� ĳ���� ���̵� ��������
	CEnemyData * GetEnemyCharaterID(D3DXVECTOR2 vPos1);
// 2007-09-05 by dgwoo �ֺ� ������ ���� ��������.
	CMonsterData * GetMonsterInfo2D(D3DXVECTOR2 vPos1);
	CEnemyData * GetPickEnemy(D3DXVECTOR2 vPos);
	void ChangeGammaOption( int nGamma );
	CObjectChild * FindEventObjectByIndex(D3DXVECTOR3 vPos, UINT nEventObjectIndex); // vPos �� ���� ����� nEventObjectIndex ������Ʈ�� ã�´�.
	CObjectChild * FindRandomObjectByEventType(BYTE bObjectType, BOOL bRand = FALSE);		// 2006-05-19 by ispark
	CObjectChild * FindEventObjectByTypeAndPosition(BYTE bObjectType, D3DXVECTOR3 vPos, float fDist=10.0f);
	CObjectChild * FindMapObjectByCodeAndPosition(int nCode, D3DXVECTOR3 vPos);
	CObjectChild * FindObjectByIndex(UINT nEventObjectIndex);					// �ε����� ������Ʈ�� ã�´�
	CObjectChild * FindEventObjectByWarp();										// ���� ������Ʈ�� ã�´�
	// 2010. 05. 27 by jskim �ó׸� ���� ī�޶� ����
	CObjectChild * FindEventObjectByTypeAndPositionIndex(BYTE bObjectType, short sEventIndex);
	//end 2010. 05. 27 by jskim �ó׸� ���� ī�޶� ����
	
// 2005-05-16 by jschoi - ���������� �� ��
	void	ChangeAllEnemyCityWarTeamType(BYTE byCityWarTeamType);// ��� Enemy�� ���������� ���� �����Ѵ�.

// 2005-07-28 by ispark
	void SetMaxAtitudeHeight(int i_nMaxHeight);
	void ChangeEnemyCharacterMode(MSG_FC_CHARACTER_CHANGE_CHARACTER_MODE_OK* pMsg);		// �� ��� �ٲٱ�

// 2007-04-23 by dgwoo �Ʒ���
	void ChangeObjectBodyCondition(INT nCode, BodyCond_t body);							// �ٵ������ ����.

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	// �� ���̿� ������Ʈ�� �ִ��� üũ
	BOOL IsObjectCheckPosTOPos(D3DXVECTOR3	vMyShuttlePos, D3DXVECTOR3	vEmenyPos, D3DXVECTOR3	vEnemyUp);
	// �� ���̿� ������ �ִ��� üũ
	BOOL IsTileCheckPosTOPos(D3DXVECTOR3	vMyShuttlePos, D3DXVECTOR3	vEmenyPos);

	// 2007-06-13 by bhsohn �޸� ���� �����
	void DeleteRenderEnemy(ClientIndex_t nIndex );

	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	void ReLoadEnemyRenderList();

	// 2008-07-14 by bhsohn EP3 ���� ó��
	BOOL GetEmemyCharacterUniqueNumber(char* pName, UID32_t* o_CharacterUniqueNumber);
	void InvectoryFullMessage();

	ClientIndex_t GetEmemyCharacterClientIndex(char* pName);	// 2012-07-19 by isshin ���۹̸�����


	// 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���
	CObjectChild*	FindMapObjectByIndex( int nIndex );
	CObjectChild*	FindMapObjectEvendByIndex( int nIndex );		// 2011-11-01 by jhahn EP4 Ʈ���� �ý��� 
	// end 2009. 11. 02 by ckPark ���Ǵ�Ƽ �ʵ� �ν��Ͻ� ���� �ý���


	// 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)
	void	ChangeSkyBox( char* szSkyBoxName );
	void	ChangeEventObjectBodyCondition( short nEventParam, BodyCond_t body );
	// end 2010. 03. 15 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ó��)

    // 2010. 10. 05 by jskim �ʷε� ���� ����
	void	StepBackground();
	void	StepBackground_Step1();
	void	StepBackground_Step2();
	void	StepBackground_Step3();
	void	StepBackground_Step4();
	void	StepBackground_Step5();
    // end 2010. 10. 05 by jskim �ʷε� ���� ����

	// 2013-05-07 by bhsohn ��������Ʈ ������ �ý���
	void	ShowObjectChild_ByType(BOOL bShow, BYTE bEventType, INT nBuildingKind); // ������Ʈ Event Ÿ������ Show
	void	ShowObjectChild_ByIndex(BOOL bShow, INT nCodeNum);
	void	UpdateInflBuffNPCObject();
	// END 2013-05-07 by bhsohn ��������Ʈ ������ �ý���

protected:
	void LoadCinemaFile();									// "cinema.tex"
	void DeleteCinemaFile();								// ��ü �����


// ������Ʈ�� �浹�� ���� ���� ���ϱ� ���� �Լ�
	enum MoveType
	{
		NOMOVE,
		TOPLEFT,
		TOPRIGHT,
		BOTTOMLEFT,
		BOTTOMRIGHT
	};

	int		CheckMove();
	void	CalcCollisionRange(int nMoveType);
	void	RenderWater();

	// 2007-05-17 by bhsohn ������Ʈ �ڿ� �������� �� ���� ó �˻� ó��
	float CheckCollRenderListRangeObject(D3DXMATRIX mat, D3DXVECTOR3 vPos, float fMovingDistance);

public:
	CFrustum		*			m_pFrustum;
	CAtumNode		*			m_pItemData;					// ������ ����Ÿ(�ʵ峻��)
	vector<CItemData*>			m_vecScanData;
	CWeapon			*			m_pWeaponData;				// ���� ����Ÿ, �̰� �ٸ� ������ �ٲ�� ��
	CBackground		*			m_pGround;					// ��
	CObjRender		*			m_pObjectRender;			// �ʻ��� ������Ʈ(������)
	CMonRender		*			m_pMonsterRender;			// ���� ������
	CETCRender		*			m_pETCRender;				// ��Ÿ ���� ������
	CWater			*			m_pWater;					// ��������
	CMapMonsterList				m_mapMonsterList;				// ���� ����Ʈ
//	CMapMonsterList				m_mapMonsterRenderList;			// ���� ������ ����Ʈ
	CVecMonsterList				m_vecMonsterRenderList;			// ���� ������ ����Ʈ
	CVecMonsterList				m_vecMonsterShadowRenderList;	// ���� �׸��� ������ ����Ʈ	
	CMapEnemyList				m_mapEnemyList;					// Ÿĳ���� ����Ʈ(�ε����� 0�� ��� 60000����� �ӽ÷� �ִ´�)
//	CMapEnemyList				m_mapEnemyRenderList;			// Ÿĳ���� ������ ����Ʈ
	CVecEnemyList				m_vecEnemyRenderList;			// Ÿĳ���� ������ ����Ʈ
	CVecEnemyList				m_vecEnemyShadowRenderList;		// Ÿĳ���� �׸��� ������ ����Ʈ
	vector<CUnitData*>			m_vecUnitRenderList;
	///////// �Ȱ� ȿ�� ������ /////////////
	DWORD						m_dwFogColor;					// �Ȱ���
	FLOAT						m_fOrgFogStartValue;				// �Ȱ� ��������-�Ÿ�(������)
	FLOAT						m_fOrgFogEndValue;					// �Ȱ� ������-�Ÿ�(������)
	FLOAT						m_fFogStartValue;				// �Ȱ� ��������-�Ÿ�(���簪)
	FLOAT						m_fFogEndValue;					// �Ȱ� ������-�Ÿ�(���簪)
	FLOAT						m_fBeforeFogStartValue;			// ������� �Ȱ� ��������-�Ÿ�
	FLOAT						m_fBeforeFogEndValue;			// �ݱ����� �Ȱ� ������-�Ÿ�
	BOOL						m_bFog;							// �������� On Off ��Ű�� ����, ������
	BOOL						m_bFogStay;						// �Ȱ��� ������ ������ ������ ��� ��Ű�� ����, ������
	D3DLIGHT9					m_light0;						// ��ü�� ���� ����Ʈ
	D3DLIGHT9					m_light1;						// ������Ʈ
	D3DLIGHT9					m_light2;						// ����Ʈ ���� ����Ʈ
	D3DLIGHT9					m_light3;						// ��Ʋ�� ���� ����Ʈ
	BOOL						m_bNight;

	CSunRender		*			m_pSunRender;					// �¾� ������
	CSunData		*			m_pSunData;						// �¾� ����Ÿ

	///////////// Tile Texture Buffer /////////////
	CGameData		*			m_pData;						// ����Ÿ���� (��Ÿ������)
	LPDIRECT3DTEXTURE9			m_pCreateTexture[TEXTILE_NUM];

	BOOL						m_bIsRestore;					// Restore-Res ����

	FLOAT						m_fSkyRedColor;
	FLOAT						m_fSkyGreenColor;
	FLOAT						m_fSkyBlueColor;
	CRainRender				*	m_pRainRender;				// ���� �� ������
	CAtumNode				*	m_pRainList;				// ���� �� ����Ʈ
	BYTE						m_byWeatherType;				// ���� Ÿ��(0:����, 1:��, 2:��)
	DWORD						m_dwStartTime;				// �ð��� ����ϱ� ���� Ŭ���̾�Ʈ�� ���۽ð�
	INT							m_nBaseTime;				// ������ ���۹��� ���� �ð�
	BYTE						m_byMapType;					// �� Ÿ��(����, �ʵ� ��..)

	CVecEnemyList	*			m_vecEnemyBlockList;
	CVecMonsterList	*			m_vecMonsterList;
	vectorCObjectChildPtr		m_vectorRangeObjectPtrList;		//�ʻ��� �þ������� ���� ������Ʈ ����Ʈ
	vectorCObjectChildPtr		m_vectorCulledObjectPtrList;	//�ʻ��� �������� ������Ʈ ����Ʈ
	vectorCObjectChildPtr		m_vectorCollisionObjectPtrList; //�ʻ��� �浹�˻縦 �� ������Ʈ ����Ʈ

	int							m_nMaxAtitudeHeight;			// �ʻ� �̵� ������ �ִ� ���� // 2005-07-11 by ispark

	int							m_nBlockSizeX;
	int							m_nBlockSizeY;
	BOOL						m_bChangeWeather;			// ���� ��ü�ÿ� ����

	// 2004-10-20 by jschoi
	float						m_fChangeWeatherCheckTime;		// ���� ��ȭ ���� �ð�

	// 2005-01-20 by jschoi
	float						m_fAlphaSky;		// ��ī�� �ڽ� ��,�� ȥ�պ���

//	float						m_fFogDestStartValue;			// ����� ��
//	float						m_fFogDestEndValue;			// ����� ��

	// 2006-11-16 by ispark, ���� ������
	vector<ALPHA_CHARACTERINFO>	m_vecAlphaEffectRender;
	vector<CUnitData*>			m_vecAlphaUnitRender;
 
    // 2010. 10. 05 by jskim �ʷε� ���� ����
	PROJECTINFO					m_prProject; 
    // end 2010. 10. 05 by jskim �ʷε� ���� ����

protected:
	CGameData				*	m_pCinemaData;
	BOOL						m_bWaterShaderRenderFlag;

	float						m_fGetItemAllDelay;
	float						m_fGetItemMessage;				// ä��â�� �κ��� �� á�ٴ� ��� �ð�üũ.

// 2008. 12. 11 by ckPark ��������

	LPDIRECT3DTEXTURE9			m_pWaterBumpTexture;			// �� ���� �ؽ���(���� ���۽� �ѹ��� �����Ѵ�)
// end 2008. 12. 11 by ckPark ��������
};

#endif // !defined(AFX_SCENEDATA_H__5325A197_B598_4A01_9E34_946C65760A16__INCLUDED_)

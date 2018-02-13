// INFGameMainMiniMap.h: interface for the CINFGameMainMiniMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINMINIMAP_H__964B7FEB_C0C4_4693_A01A_563E957C1DD4__INCLUDED_)
#define AFX_INFGAMEMAINMINIMAP_H__964B7FEB_C0C4_4693_A01A_563E957C1DD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

typedef struct
{	
	int			nQuestIndex;
	int			nMonsterPosX;
	int			nMonsterPosY;
	char		sQuestMonsterName[64];
} MiniMap_PosData;


class CAtumNode;
class CINFImage;
class CD3DHanFont;
class CSkinnedMesh;
class CINFImageBtn;	// 2008-11-3 by bhsohn ���� �ð� ���� ��ư �߰�
//class CItemData;
class CINFImageEx;
class CINFGroupImage;

class CINFGameMainMiniMap : public CINFBase  
{
public:
	CINFGameMainMiniMap(CAtumNode* pParent);
	virtual ~CINFGameMainMiniMap();
	
	// 2008-11-3 by bhsohn ���� �ð� ���� ��ư �߰�
	//void SetShowState(BOOL bState) { m_bShowState = bState; }	
	//BOOL GetShowState() { return m_bShowState; }
	void SetZoomState(BYTE bState) { m_bZoomState = bState; }
	BYTE IsRenderBigMap() { return m_bZoomState; }

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	void RenderBigMap();
	void RenderSmallMap();
	void RenderMap();
	void RenderEnemy();
	void RenderMonster();
	void RenderOperation();
	void SetMonsterMiniMap();
	void InitMapSet();									// 2007-02-22 by dgwoo �ʼ���.
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);
	
protected:
	void CheckRadar();										
	void TickRadarMap();										// 2005-09-29 by ispark
	void RenderRadarMap();
	HRESULT InitRadarMap();	
	void GetMiniBigMapCityPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy);	// 2006-05-04 by ispark
	void GetMiniBigMapPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy);		// 2006-09-04 by ispark
	void GetMiniSmallMapCityPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy);
	void GetMiniSmallMapPos(D3DXVECTOR3 vPos, float * fPosx, float * fPosy);	

	// 2008-11-3 by bhsohn ���� �ð� ���� ��ư �߰�
	void OnClickServerTime();

protected:
	BOOL			m_bRestored;
//	CSkinnedMesh*	m_pMeshUnit;							// 2005-09-13 by ispark ���� ó��
//	CSkinnedMesh*	m_pMeshParty;							// 2005-09-13 by ispark ���� ó��
//	CSkinnedMesh*	m_pMeshArea;							// 2005-09-13 by ispark ���� ó��

	CINFImageEx*		m_pBackImage;
	CINFImageEx*		m_pSiteImage;
	CINFImageEx*		m_pPlayerImage;
	CINFImageEx*		m_pUserImage[3];
	CINFImageEx*		m_pPartyImage[3];
	CINFImageEx*		m_pMonsterImage[3];
	CINFImageEx*		m_pInfluenceImage[3];
	CINFImageEx*		m_pGuildImage[3];// 2012-04-09 by isshin ���̴� �� �̴ϸ� ��Ƽ,���ܿ� ǥ�� - ���̴� ���ܿ�

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*		m_pMiniMapBK;
	CINFGroupImage*		m_pBigMapBK;
	//CINFImage*		m_pButtonZoom[3][BUTTON_STATE_NUMBER];
	// 2008-11-3 by bhsohn ���� �ð� ���� ��ư �߰�
	//CINFImage*		m_pButtonShowHide[2][BUTTON_STATE_NUMBER];
	//CINFImageBtn*		m_pServerTimeBtn;														  
#else	  														  
      CINFImage*		m_pButtonZoom[3][BUTTON_STATE_NUMBER];
	CINFImageBtn*		m_pServerTimeBtn;
#endif

	CINFImageEx*		m_pDeadImage;
	CINFImageEx*		m_pLockonImage;
	CINFImageEx*		m_pImgStateWarning;						// 2005-07-08 by ispark // ���� �̻��� �汳
	CINFImageEx*		m_pMiniMapFrame;						// �̴ϸ� Ʋ
	CINFImageEx*		m_pMiniMapUnitImage;					// ���
	CINFImageEx*		m_pMiniMapPartyImage;					// ��Ƽ �̹���
	CINFImageEx*		m_pMiniMapGuildImage;					// 2012-04-09 by isshin ���̴� �� �̴ϸ� ��Ƽ,���ܿ� ǥ�� - �̴ϸ� ���ܿ�
	CINFImageEx*		m_pArenaMapInfImg;						// �Ʒ��� �ʿ��� ���� ������ ǥ�����ٶ�.
	CINFImageEx*		m_pImgQuestTarget;						// 2007-02-26 by dgwoo ��ǥ��.
	
	CD3DHanFont*	m_pFontPosition;						// ��Ƽ������ �̸�.
	CD3DHanFont*	m_pFontQuestTarget;						// ����Ʈ Ÿ���� �̸�.
	//vector<CD3DHanFont*>	m_vecQuestPosition;

	// 2008-11-3 by bhsohn ���� �ð� ���� ��ư �߰�
	//int				m_nButtonState[2];	// zoom, show
	int				m_nZoomButtonState;		// zoom
	//BOOL			m_bShowState;		// SHOW(TRUE), HIDE(FALSE)
	// end 2008-11-3 by bhsohn ���� �ð� ���� ��ư �߰�

	BYTE			m_bZoomState;		// 0: HIDE 1: BIGMAP 2: SMALLMAP
	//BOOL			m_bShowMap;			// �̴ϸ� Show Hide
	D3DXMATRIX		m_matRotate;		// ������ Z�࿡ ������ ���� ���� ��Ʈ����
	float			m_fAngle;			// ������ ������ ����
	float			m_fRadarBackAngle;	// ���̴� ��ü ����
	float			m_fCurrentTime;

	BOOL			m_bMissileWarningRender;// �̻��� ��� ȭ�� ������
	float			m_fMissileWarningTime;	// �̻��� ��� ȭ�� ������ �ð�
	
	float			m_fRange;				// 2005-08-16 by ispark ���̴� ��ü��������
	vector<MiniMap_PosData*> m_vecMosterPosition;

	CINFImageEx*	m_pRadarMiniMap;
	BYTE			m_byAlphaMiniMap[118][118];

	DataHeader*		m_MiniMapSubDataHeader[4];

	BOOL			m_bMiniMapState;		// 2006-09-04 by ispark, �� �������¿� ���� �̴ϸ� ��,�� Ȯ��
	int				m_nTestMapPosX;
	int				m_nTestMapPosY;
};

#endif // !defined(AFX_INFGAMEMAINMINIMAP_H__964B7FEB_C0C4_4693_A01A_563E957C1DD4__INCLUDED_)

// INFMissionMapViewCtrl.h: interface for the CINFMissionMapViewCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMISSIONMAPVIEWCTRL_H__1526ABC7_38E3_48E8_A354_6834D65EB0EF__INCLUDED_)
#define AFX_INFMISSIONMAPVIEWCTRL_H__1526ABC7_38E3_48E8_A354_6834D65EB0EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFArenaScrollBar.h"
#include "INFImageEx.h"								 // 2011. 10. 10 by jskim UI�ý��� ����

typedef struct 
{
	CINFImageEx*	pInfImage;		// �� �̹���
	DataHeader*		pHeader;
	MapIndex_t		MapIndex;
	char			chMapName[256];
	vector<MEX_MONSTER_INFO>	vecMonsterInfo;	
} stMapViewInfo;

typedef struct
{	
	int			nQuestIndex;
	int			nMonsterPosX;
	int			nMonsterPosY;
} MapView_PosData;

#define	MONSTER_INFO_ITEM_LEN	2
// ���� �ִ� ��
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
     #define	MAX_MONSTER_INFO		4																  
#else																									  
#define	MAX_MONSTER_INFO		8
#endif

class CINFMissionMapViewCtrl  : public CINFBase
{
public:
	CINFMissionMapViewCtrl();
	virtual ~CINFMissionMapViewCtrl();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	void UpdateMapInfo(CQuest* pQuest, MapIndex_t	MapIndex);
	void RestoreMapView(BOOL bAll, MapIndex_t	MapIndex);

	void SetWindowPos(float fPosX, float fPosY, float fPosWidth, float fPosHeight);

	// �̺�Ʈ �޽��� 
	BOOL OnLButtonDown(POINT pt);	
	BOOL OnMouseWheel(POINT pt, WPARAM wParam, LPARAM lParam);
	BOOL OnLButtonUp(POINT pt);
	BOOL OnMouseMove(POINT pt);

private:	
	void RenderMapInfo();
	// ���� ���� ������Ʈ
	void RenderMonsterInfo(vector<MEX_MONSTER_INFO>	*i_vecMonsterInfo);
	// ����Ʈ ���� ��ġ ǥ��
	void RenderQuestMonsterPos();

	// ���� ��ġ ���� ���� 
	void SetMonsterMiniMap(CQuest* pQuest, MapIndex_t	MapIndex, vector<MapView_PosData> *o_vecMosterPosition);

	// ȣ����, ��ȣ���� ����
	void GetAttackString(MEX_MONSTER_INFO * i_pMonsterInfo, char* o_pTxt);

	BOOL IsLoadMapInfo(MapIndex_t	MapIndex, int* o_nItemCnt);
	int  AddMapInfo(MapIndex_t MapIndex);
	// �ʿ� ���� ���� ������ ���´�.
	HRESULT GetMapIndex_To_Monster(MapIndex_t	MapIndex, vector<MEX_MONSTER_INFO> *o_vecQuestInfo);
	BOOL GetToolTipTxt(int nSelTooltip, char* chBuf);

	void UpdateScrollPos(float fPosX, float fPosY, float fPosWidth, float fPosHeight);

	// �ʿ� ���� ������ ���´�. 
	stMapViewInfo* GetMapIdx_To_MapViewInfo(MapIndex_t selMapIndex);

	BOOL UpdateToolTipInfo(POINT pt);

	// ���¸����̳�?
	BOOL IsBelligerenceMonster(BYTE monsterBelligerence);

private:
	vector<stMapViewInfo*>	m_vecMapInfo;
	MapIndex_t				m_selMapIndex;

	// ���� ����
	CD3DHanFont *			m_pFontMonInfo;
	// ��� ����
	CD3DHanFont *			m_pFontMonTitle;
	// �� �̸�
	CD3DHanFont *			m_pFontMapName;

	float					m_fPosX;
	float					m_fPosY;
	float					m_fWidth;
	float					m_fHeight;

	CINFImageEx	*			m_pImageItem;
	CINFImageEx	*			m_pImgQuestTarget;

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	POINT					m_ptMonPos[MAX_MONSTER_INFO];
#endif
	POINT					m_ptMonTitle;

	// ��ũ�� �� 
	CINFArenaScrollBar*		m_pINFScrollBar;

	vector<MapView_PosData> m_vecMosterPosition;

};

#endif // !defined(AFX_INFMISSIONMAPVIEWCTRL_H__1526ABC7_38E3_48E8_A354_6834D65EB0EF__INCLUDED_)

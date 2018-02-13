// INFDissolution.h: interface for the CINFDissolution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFDISSOLUTION_H__98354002_201D_4FAC_AC33_5E6212809401__INCLUDED_)
#define AFX_INFDISSOLUTION_H__98354002_201D_4FAC_AC33_5E6212809401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"
// 2011-02-08 by jhahn EP4 ����  UI ����
#include "INFImageEx.h"
#include "INFImageList.h"
//end 2011-02-08 by jhahn EP4 ����  UI ����

#define EFFECT_MAX					13
#define EFFECT_BG_MAX				 6
#define TAKE_EFFECT_MAX				 2
#define EFFECT_TIME				    70


#define DIS_START					 0
#define DIS_ING						 1
#define DIS_END						 2

// 2011-02-08 by jhahn EP4 ����  UI ����
class CINFImageList;
class CINFGroupManager;
class CINFGroupImage;
//end 2011-02-08 by jhahn EP4 ����  UI ����
struct DIS_ITEM_INFO 
{
	CItemInfo*		i_ItemInfo;
	int				i_count;
};

class CINFDissolution : public CINFBase
{
public:
	CINFDissolution(CAtumNode* pParent, BUILDINGNPC* pBuilding);
	virtual ~CINFDissolution();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Render();
	virtual void Tick();

	void RenderItem(CItemInfo* nSelectItem);
	void RenderItem(vector<DIS_ITEM_INFO> vecTakeItem);
	void ResetInfo();
	void UpLoadItem(CItemInfo* i_pItem);
	void StartDissolution();
	void PrizeItemGiveMe();
	BOOL IsCloseDissolution();

	void EnterDissolutionShop();
	void OnCloseInfWnd();
	vector<DIS_ITEM_INFO> GetTakeItem() { return m_vecTakeItemInfo; }
	void SetTakeItem( ITEM_GENERAL i_Item );
	void SetTakeItem(MSG_FC_TRADE_UPDATE_ITEM_COUNT* pMsg);
	void SetState( int state ) { m_nState = state; }
	BOOL GetBoom() { return m_bBoom; }
	void SetBoom(BOOL iBoom) { m_bBoom = iBoom; }
	void SetDeleteItem(const UID64_t a_ItemUniqueNumber);


private:

	CD3DHanFont	*			m_TakeItemFont;				//

 	CINFImageEx*			m_pImgDissolutionBG;		// ���
	CINFImageEx*			m_pImgDissolutionLogo;		// ���� �ΰ�
	CINFImageEx*			m_pImgDissolutionAniBG[EFFECT_BG_MAX];			//���� �ΰ� �ִϸ��̼� 

	CINFImageEx*			m_pImgDissolutionEffect[EFFECT_MAX];			// ���� ����Ʈ
	CINFImageEx*			m_pImgDissolutionTakeEffect[TAKE_EFFECT_MAX];	// ���� ���� ������ ����Ʈ 

 	CINFImageBtn*			m_pBtnStart;				// ���� ���� 
	CINFImageBtn*			m_pBtnTake;					// ������ �ޱ�

	int						m_nState;					// DIS_START  = ���, DIS_ING = ���� ��, DIS_END = ���
	int						m_nEffStep;					// ����Ʈ ����
	int						m_nEffBgStep;				// ����Ʈ ��� ���� 

	float					m_fTimeEffectBG;			// ��� ������ ���� �ð�
	float					m_fTimeFadeInBG;			// ���̵� �� ���� �ð�
	float					m_fTimeEffectAniBG;			// ��� �ִ� ������ �����ð� 
	
	CItemInfo*				m_pSelectItemInfo;			// ������ ������
	vector<DIS_ITEM_INFO>	m_vecTakeItemInfo;			// ���� �ƾ��� ����
	BOOL					m_bBoom;					// ��
	BOOL					m_bStartClick;				// ��ư Ŭ��
// 2011-02-08 by jhahn EP4 ����  UI ����		   
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*			m_pDissolutionBK;
	CINFGroupImage*			m_pDissolutionControl;												  
#endif
//end 2011-02-08 by jhahn EP4 ����  UI ����
	
};

#endif // !defined(AFX_INFDISSOLUTION_H__98354002_201D_4FAC_AC33_5E6212809401__INCLUDED_)

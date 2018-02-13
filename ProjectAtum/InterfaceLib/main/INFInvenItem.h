// INFInvenItem.h: interface for the CINFInvenItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINVENITEM_H__9AC46863_9DF1_4A78_B0D6_BDCBDEDA91F5__INCLUDED_)
#define AFX_INFINVENITEM_H__9AC46863_9DF1_4A78_B0D6_BDCBDEDA91F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageBtn.h"
#include "INFImageBtnBoth.h"
#include "INFImportantInfo.h"

class CINFArenaScrollBar;
class CINFImageBtn;
class CINFGroupImage;

class CINFInvenItem  : public CINFBase  
{
public:
	CINFInvenItem(CAtumNode* pParent);
	virtual ~CINFInvenItem();

	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	
	void Render();
	void Tick();

	BOOL IsShowWnd();
	void ShowWnd(BOOL bShow, POINT *i_ptPos=NULL);

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven);

	int OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven);
	int OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven);
	int OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven);
	int OnMouseWhell(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven);
	int OnLButtonDB(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven);
	int OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bShowInven);

	int GetScrollStep();
	void SetMaxScrollStep(int nStep);
	void SetScrollEndLine();

	POINT GetBkPos();
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	POINT GetBkSize();
	BOOL GetMove() { return m_bMove; }															  
#endif

	// 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��
	void SetBkPos(POINT ptBkPos);	
	// END 2013-02-26 by bhsohn �ΰ��� ���� �˻� ó��

	// 2013-04-23 by ssjung �߿� ������ ������ 2�� ���(����) �˾�â ����
	void ImportantItemDeleteMsgBox(CItemInfo* pSelectItem, char* buf, ITEM *item);
private:
	void RenderInvenItem();
	BOOL IsInvenRect(POINT pt, int *o_pPosX, int *o_pPosY);
	BOOL IsWndRect(POINT pt);
	void UpdateInvenScroll();

	void UpdateBtnPos();
	BOOL IsMouseCaps(POINT ptPos);

	int OnLButtonUpInvenPosItem(POINT pt, CItemInfo* pSelectItem);
	int OnLButtonUpStorePosItem(POINT pt, CItemInfo* pSelectItem);

	// 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	BOOL OnLButtonDbClick_LuckyMechine(ITEM *pItem , ITEM_GENERAL* pItemGeneral);
	// end 2009-04-02 by bhsohn ��Ű �ӽ� �߰� ��ȹ��
	// 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	BOOL OnLButtonDbClick_OptionMechine(ITEM *pItem , int UniqueNumber);
	//end 2010. 04. 21 by jskim �ű� ��Ű �ӽ� ����
	// 2010-08-31 by shcho&&jskim, ���� �ý��� ����
	BOOL OnLButtonDbClick_Dissolution(ITEM *pItem , int UniqueNumber);
	// end 2010-08-31 by shcho&&jskim, ���� �ý��� ����
	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	BOOL ShopIsDisableInvenItem(BYTE BuildingKind, CItemInfo* pItemInfo);
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
    // 2012-02-03 by jskim ���丮 ����Ŭ�� ���� 
	BOOL OnLButtonDbClick_Factory(ITEM *pItem , int UniqueNumber);
	BOOL OnLButtonDbClick_Laboratory(ITEM *pItem , int UniqueNumber);
    // end 2012-02-03 by jskim ���丮 ����Ŭ�� ���� 


private:
	BOOL m_bShowWnd;

#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*	m_pInvenBase;	// ���
#endif
	POINT	m_ptBkPos;	

	CAtumNode*		m_pParent;

	CD3DHanFont*	m_pFontItemNum;

	CINFArenaScrollBar*	m_pINFInvenScrollBar;	

	BOOL						m_bMove;
	POINT						m_ptCommOpMouse;

	// Item ���� ���� ó��
	CINFImageEx*			m_pMultiItemSelImage;
	// 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�
	CINFImageEx*			m_pDisableItemImage;
	//end 2010. 05. 10 by jskim �ű� ��Ű �ӽ� �߰���� - �������� ��� ������ �����۸� �����ֱ�

	CINFImageEx*			m_pSelectPetkitItemImageHP;		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	CINFImageEx*			m_pSelectPetkitItemImageSheld;	  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���
	CINFImageEx*			m_pSelectPetkitItemImageSP;		  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	CINFImageEx*			m_pSelectPetSocketItemImage;	  //2011-10-06 by jhahn ��Ʈ�� ������ �ý���

	CINFImageBtn*		m_pEqShow;
	CINFImageBtn*		m_pCloseBtn;

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*		m_pInvenBG;
#endif
	
	// 2013-02-19 by mspark, �κ��丮 ���� ��ư ���� �������� Ŭ�� ����
	bool				m_bInvenSortBtnArea;
	// end 2013-02-19 by mspark, �κ��丮 ���� ��ư ���� �������� Ŭ�� ����

};

#endif // !defined(AFX_INFINVENITEM_H__9AC46863_9DF1_4A78_B0D6_BDCBDEDA91F5__INCLUDED_)

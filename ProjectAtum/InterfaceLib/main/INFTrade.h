// INFTrade.h: interface for the CINFTrade class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFTRADE_H__A1A72283_316B_4C23_9FAB_08AB973705B6__INCLUDED_)
#define AFX_INFTRADE_H__A1A72283_316B_4C23_9FAB_08AB973705B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFMessageBox;
class CINFImage;
class CD3DHanFont;
class CItemInfo;
class CINFImageEx;									// 2011. 10. 10 by jskim UI�ý��� ����

class CINFTrade : public CINFBase  
{
public:
	CINFTrade(CAtumNode* pParent);
	virtual ~CINFTrade();

		virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnTradeButtonClick(int nButton);
	void SendTradeRequest(char* strCharacterName);// ���������� ���� �ŷ� ��û
	void RequestTrade(char* strCharacterName,UINT nTradeCharacterUniqueNumber);//������ ������ �ŷ� ��û
	void SendTradeAccept();// �ŷ� ����
	void SendTradeReject();// �ŷ� ����
	void RecvTradeAccept(char* strCharacterName, UINT nTradeCharacterUniqueNumber);//������ �ŷ� ����
	void SendTradeCancel();
	void SendTradeOk();
	void SendTradeTrans(int count);
	void AddItem(CItemInfo* pItem, int me);
	void EndTrade();
	void CancelTrade();
	BOOL TradeQuestionInviteUser(CINFMessageBox* pMsgBox, int x, int y);
//	void AddEnchantItem(LONGLONG uniqueNumber, BYTE nDestParam, float fParamValue);
	void AddEnchantItem( LONGLONG uniqueNumber, INT nEnchantItemNum );
	CItemInfo* FindTradeMyItem(UID64_t ItemUniqueNumber);							// 2006-05-15 by ispark

	// 2007-10-16 by bhsohn �ŷ� �� �׾�����, Ű �ȸԴ� ����ó��
	void SetTradeInfo(BOOL bTrade);

	// 2008-11-21 by bhsohn �ŷ� �Ϸ� �ý��� ó��
	void SetTradeOkTradeNotify(UID32_t CharacterUniqueNumber);

public:
	BOOL				m_bRestored;
	BOOL				m_bInvalidated;

	CINFImageEx	*	m_pTradeOkButton[4];							// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx	*	m_pTradeCancelButton[4];
	CINFImageEx	*	m_pTralog;
	CINFImageEx	*	m_pTrame;
	CINFImageEx	*	m_pTrayou;

	// 2008-11-21 by bhsohn �ŷ� �Ϸ� �ý��� ó��
	CINFImageEx	*	m_pTrameComple;
	BOOL			m_bTraMeComple;

	CINFImageEx	*	m_pTrayouComple;
	BOOL			m_bTraYouComple;	
	// end 2008-11-21 by bhsohn �ŷ� �Ϸ� �ý��� ó��
	
	int				m_nTradeButtonState[2];
	BOOL			m_bTrading;	// ���� �ŷ����� ���¸� TRUE
	char			m_strTradeCharacterName[SIZE_MAX_CHARACTER_NAME];
	UINT			m_nTradeCharacterUniqueNumber;
	CD3DHanFont*	m_pFontCharacterName[2];// 0:other, 1:me
	CD3DHanFont*	m_pFontItemNum[2];// 0:other, 1:me
	ITEM_GENERAL*	m_pTempItem;

//	vector<ITEM_GENERAL*> m_vecMyStore;
//	vector<ITEM_GENERAL*> m_vecOtherStore;
	vector<CItemInfo*> m_vecMyStore;
	vector<CItemInfo*> m_vecOtherStore;
//	map<LONGLONG, ITEM_ENCHANT*> m_mapOtherEnchant;
	int				m_nMyStoreScroll;
	int				m_nOtherStoreScroll;
	int				m_nMyStoreSpi;
	int				m_nOtherStoreSpi;
	CD3DHanFont*	m_pFontStoreSpi[2];



};

#endif // !defined(AFX_INFTRADE_H__A1A72283_316B_4C23_9FAB_08AB973705B6__INCLUDED_)

// I/NFCommunityFriend.h: interface for the CINFCommunityFriend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYFRIEND_H__AFEB5D1B_958C_4437_AD53_E79E96A44124__INCLUDED_)
#define AFX_INFCOMMUNITYFRIEND_H__AFEB5D1B_958C_4437_AD53_E79E96A44124__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"


class CINFImage;
//class CINFScrollBar;
class CINFArenaScrollBar;
class CD3DHanFont;
class CINFImageBtn;
class CINFGroupImage;
class CINFImageEx;

// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
#define MAX_FRIEND_NUMBER					13	// ȭ��� ���� ��
#define MAX_FRIEND_LIST						50  // �����Ҽ� �ִ� �ִ� ģ�� ��


typedef struct
{
	char			szFriendName[SIZE_MAX_CHARACTER_NAME];
	BOOL			bOnline;
} Friend_t;

class CINFCommunityFriend : public CINFBase  
{
public:
	CINFCommunityFriend(CAtumNode* pParent);
	virtual ~CINFCommunityFriend();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	void Render(POINT ptPos);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int nButton);

	void DeleteFriendList(char *FriendName);

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	void ResetFriendList();
	void AddFriendList(Friend_t friendInfo);
	void FriendListRefresh(char* szFriendName, BYTE byIsOnline);	
	void SendFriendDelete();

	BOOL IsWisperChatSend();
	void SetWisperChatSend(BOOL bWisperChatSend);
	int	 GetFriendListOnCount();
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â

	// 2009-01-12 by bhsohn ģ�� ��� ���ϴ� ���� �޽���â ��� �߰�
	BOOL IsMyFriend(char* i_pUser);
	void SetRqFriend(char* i_pUser);
	char* GetRqFriend();
	// end 2009-01-12 by bhsohn ģ�� ��� ���ϴ� ���� �޽���â ��� �߰�

	// 2009-01-14 by bhsohn ģ�� ����Ʈ �ӼӸ� ��� �߰�
	void RefreshFriendInfo();

	void UpdateBtnPos();
private:
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	void RenderFriendList(POINT ptBkPos);
	
	void OnClickFriendRegist();
	void OnClickFriendDel();

	void TestDBFriendList();

protected:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*			m_pBack;															  
#else 
	CINFImageEx*            m_pBack;
#endif
	CINFImageEx*			m_pImgSelect;
	//CINFImage*				m_pImgButton[4];

	CD3DHanFont				*m_pFontFriend[MAX_FRIEND_NUMBER];
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	//CINFScrollBar*			m_pScroll;	
	CINFArenaScrollBar*			m_pScroll;	
	CINFImageBtn*				m_pRegisterFriend;	// ���
	CINFImageBtn*				m_pDelFriend;		// ����

	BOOL					m_bAddFriendClick;
	BOOL					m_bDeleteFriendClick;	
	
	int						m_nFriendSelect;
	int						m_nButtonState;
	float					m_fRefreshTime;	


	vector<Friend_t>		m_vecFriend;
	//BOOL					m_bDeleteOK;
	BOOL					m_bWisperChatSend;

	// 2009-01-12 by bhsohn ģ�� ��� ���ϴ� ���� �޽���â ��� �߰�
	char					m_szRqFriendName[SIZE_MAX_ARENA_FULL_NAME];
	// end 2009-01-12 by bhsohn ģ�� ��� ���ϴ� ���� �޽���â ��� �߰�

};

#endif // !defined(AFX_INFCOMMUNITYFRIEND_H__AFEB5D1B_958C_4437_AD53_E79E96A44124__INCLUDED_)

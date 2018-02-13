// INFCommunity.h: interface for the CINFCommunity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITY_H__518CBD3B_8B48_4BB5_B7F7_62E03BDA55FD__INCLUDED_)
#define AFX_INFCOMMUNITY_H__518CBD3B_8B48_4BB5_B7F7_62E03BDA55FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define COMMUNITY_PARTY			0
#define COMMUNITY_FRIEND		1
#define COMMUNITY_GUILD			2
#define COMMUNITY_CHATROOM		3
#define COMMUNITY_MAIL			4
#define COMMUNITY_REJECT		5
#define COMMUNITY_MAX			6

// ��� ��û ������
#define COMMUNITY_MEMINFO_MANAGER	0
#define COMMUNITY_MEMINFO_SEARCH	1

class CAtumNode;
class CINFCommunityParty;
class CINFCommunityGuild;
class CINFCommunityFriend;
class CINFCommunityReject;
class CINFCommunityChatRoom;
class CINFCommunityLetter;	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
class CINFCommuPartyInvite;	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
class CINFCommuPartyCre;	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
class CINFCommunityGuildManager;// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
class CINFCommunityGuildSearch;// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
class CINFCommunityGuildIntroMe;// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
class CINFCommunityUserOpenSetup;
class CINFCommunityUserInfo;
class CINFEnemyItemInfo;	// 2012-06-14 by isshin ���۹̸�����
class CINFImage;
class CINFImageBtn;
class CINFImageEx;											 // 2011. 10. 10 by jskim UI�ý��� ����
class CINFGroupImage;
class CINFGroupManager;

class CINFCommunity : public CINFBase  
{
public:
	CINFCommunity(CAtumNode* pParent);
	virtual ~CINFCommunity();

	CINFCommunityParty* GetParty() { return m_pParty; }
	CINFCommunityGuild* GetGuild() { return m_pGuild; }
	CINFCommunityFriend* GetFriend() { return m_pFriend; }
	CINFCommunityReject* GetReject() { return m_pReject; }

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	CINFCommunityLetter* GetLetter() { return m_pLetter; }
	CINFCommuPartyCre*	GetPartyCreate(){return m_pCommuPartyCreate;}	// ������	
	CINFCommunityGuildManager* GetGuildManager() { return m_pCommunityGuildManager; }
	CINFCommunityGuildSearch* GetGuildSearch() {return m_pCommunityGuildSearch;	}
	CINFCommunityGuildIntroMe* GetGuildIntroMe() {return m_pCommunityGuildIntroMe;	}
	CINFCommuPartyInvite*	GetPartyInvite(){ return m_pCommuPartyInvite;}// ��� �ʴ�
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual void Tick();
	void Render();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetCommunityType(int nType);

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	void ShowCommunityWindow(BOOL bShow, int nSubTyepe=COMMUNITY_PARTY);
	BOOL IsShowCommunityWindow();
	POINT GetCommunityBkPos();	
	void OnClickCurrentParty();
	
	// ��� ����
	void OnPopupCreateParty();
	void OnClickCreateParty();
	void SetAutoCreateParty(SPARTY_INFO i_PartyInfo);		/// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : �ڵ����� ��Ƽ�����.

	// ��� �ʴ�
	void OnPopupInviteParty();

	// ���� �ʴ�
	void PartySendInviteUser(char* pUserName);

	// ��� Ż��
	void OnClickSecedeParty();

	// ��������
	void PartySendBanUser();

	// ����� ����
	void PartySendTransferMaster();

	// ���� ������
	void OnClickLetterWrite(char* pUser);

	void OnPopupGuildManager();
	void OnPopupGuildSearch();
	void OnPopupIntroMe();		// �ڱ�Ұ�

	// �����̸��� �����´�.
	void GetUnitKindString(USHORT i_nUnitKind,char* pTxt);
	INT GetChatRoomNum();
	CINFCommunityChatRoom* GetChatRoom()				{	return m_pChatRoom;			}

	// ������ ���� UID�� ���´�.
	BOOL GetGuildSearchSelGuidUID(UID32_t *pUID);
	BOOL GetGuildSearchSelGuidName(char* pGuildName);
	char* GetGuildName(UINT nGuildUniqueNumber);

	void GetFormatString(int nSelTxt, char* pDst);
	void RqMemberInfo(int nWndIdx, UID32_t	CharacterUID);
	void IMSocketGetSelfIntroductionOK(MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* pMsg);
	MSG_IC_GUILD_GET_SELF_INTRODUCTION_OK* GetMySelfIntro();
	
	void PopupRqCancelGuild();
	void RqCancelMyGuildIntro();
	void ShowGuildSearch(BOOL bShow);

	void OnPopupUserOpenSetup(BOOL bCharcterWnd);
	void ShowUserInfo(MSG_FC_CHARACTER_GET_USER_INFO_OK* pMsg);
	void HideUserInfo();	// 2015-06-27 Future, Hides the showing user Info
	
	// 2012-06-14 by isshin ���۹̸�����
	void ShowEnemyItemInfo(MSG_FC_CHARACTER_GET_USER_ITEM_INFO_OK_DONE* pMsg);
	void UpdateEnemyItemInfo(BOOL bUpdatEnemyItemInfo, ClientIndex_t ClientIndex);
	void RqCharacterUserItemInfo(UID32_t	TargetCharcterUID, ClientIndex_t TargetCharcterCID);
	// end 2012-06-14 by isshin ���۹̸�����

	void RqCharacterUserInfo(UID32_t	TargetCharcterUID);	// ���� ���� ��û
	void RqChangeOpenUserInfo(INT SecretInfoOption);		// ���� ���� ���� ����
	void FieldSocketCharacterChangeInfoOptionSecetOk();

	BOOL IsShowWnds();

	void UpdateBtnPos();
	BOOL IsMouseCaps(POINT ptPos);
	// end 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â	
	void SetMissionMasterOption(BOOL i_bMissionMaster);// 2008-12-09 by dgwoo �̼Ǹ�����.

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	int SetBtnClick(int nNum);																	  
#endif

protected:
	BOOL					m_bRestored;
	int						m_nCommunityType;	//	COMMUNITY_~

	//CINFImage	*			m_pLogo;

	CINFCommunityParty		*m_pParty;
	CINFCommunityGuild		*m_pGuild;
	CINFCommunityChatRoom	*m_pChatRoom;
	CINFCommunityFriend		*m_pFriend;
	CINFCommunityReject		*m_pReject;
	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	CINFCommunityLetter		*m_pLetter;		// ���� 
	

	// 2008-04-04 by bhsohn Ep3 Ŀ�´�Ƽ â
	POINT					m_ptCommunityBk;	
	BOOL					m_bShowWnd;
	
	CINFCommuPartyCre*			m_pCommuPartyCreate;	// ������	
	CINFCommuPartyInvite*		m_pCommuPartyInvite;	// ��� �ʴ�
	CINFCommunityGuildManager*	m_pCommunityGuildManager;
	CINFCommunityGuildSearch*	m_pCommunityGuildSearch;	// ��� ã��
	CINFCommunityGuildIntroMe*	m_pCommunityGuildIntroMe;
	CINFCommunityUserOpenSetup*			m_pCommunityUserOpenSetup;	// ���� ����
	CINFCommunityUserInfo*		m_pCommunityUserInfo;
	CINFEnemyItemInfo*			m_pEnemyItemInfo;		// 2012-06-14 by isshin ���۹̸�����	
	

	CINFImageBtn*				m_pCloseBtn ;	// �ݱ� ��ư
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*				m_pCommunityBKImage;
	CINFGroupImage*				m_pCommunityControl;
#endif
	int							m_nRqMemInfoWnd;

	int							m_nLastRqSecretInfoOption;

	BOOL						m_bMove;
	POINT						m_ptCommOpMouse;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����	
	CINFImageBtn*				m_pCommMenuBtn[COMMUNITY_MAX];
#endif

};

#endif // !defined(AFX_INFCOMMUNITY_H__518CBD3B_8B48_4BB5_B7F7_62E03BDA55FD__INCLUDED_)

// INFCommunityChatRoom.h: interface for the CINFCommunityChatRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYCHATROOM_H__4064B992_8044_4F20_A7EE_A944D683A1BE__INCLUDED_)
#define AFX_INFCOMMUNITYCHATROOM_H__4064B992_8044_4F20_A7EE_A944D683A1BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

//#define	CHATROOM_ADMISSION_NUMBER_COUNT				6
#define	CHATROOM_ADMISSION_NUMBER_COUNT				7
#define CHATROOM_SUBJECT_MAX_SIZE						34

enum {
	CHATROOM_TAB_LIST = 1,
	CHATROOM_TAB_ROOM = 2
};

class CINFImage;
class CINFImageBtn;
class CINFArenaScrollBar;
class CINFEditBox;
class CINFListBox;
class CINFImageRadioBtn;
class CINFSecuEditBox;
class CINFGroupImage;
struct CHATROOM_INFO
{//EP3 - ä�ù�  ����
	INT			ChatRoomNum;
	INT			ChatRoomInfoListCount;
	UID32_t		MasterCharacterUID;
	char		ChatRoomName[SIZE_MAX_CHATROOM_NAME];
	BOOL		ChatRoomLock;
	char		ChatRoomPW[SIZE_MAX_TEAM_PW];
	INT			ChatRoomMaxCount;
};
class CINFCommunityChatRoom : public CINFBase  
{
public:
	CINFCommunityChatRoom(CAtumNode* pParent);
	virtual ~CINFCommunityChatRoom();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	virtual void Tick();

	void InitSetting();

	INT GetListBoxSelect(INT i_nIndex);				// �ε��������� ���õ� ���� �ο����� ����.
	INT GetListBoxSelectItem(INT i_nCount);			//GetListBoxSelect()�Լ��� �ݴ� �뵵.
	void ChangeTab(DWORD i_nTab);
	BOOL SendChatRoomInfo();
	BOOL SendEnterRoom();
	void AddRoomList(MSG_IC_CHATROOM_LIST_INFO_OK * pMsg);
	void AddMyRoomInvite(MSG_IC_CHATROOM_ACCEPT_INVITE_OK * pMsg);
	void AddMyRoomInfo(MSG_IC_CHATROOM_MEMBER_INFO_OK * pMsg);
	void AddOtherRoomInfo(MSG_IC_CHATROOM_MEMBER_INFO_OK * pMsg);
	void ChatRoomBan(MSG_IC_CHATROOM_BAN_OK* pMsg);
	void SetButtonActive();
	void InitMyRoomInfo();
	void SendRoomListRequest(INT i_nRoomNum);
	void ModifyChatRoom();
	void ChangeLockPWOK(MSG_IC_CHATROOM_CHANGE_LOCK_PW_OK* pMsg);
	void AllEditBoxEnable();
	void CreateMyRoomOK(MSG_IC_CHATROOM_CREATE_OK* pMsg);
	void InitMyChatRoom();
	void ChatRoomLeaveOK(MSG_IC_CHATROOM_LEAVE_OK* pMsg);

	SCHATROOM_LIST_INFO* FindChatRoomList(INT i_nRoomNum);

	void Render(POINT i_ptParent);
	void RenderList();
	void RenderChatRoom();
	void RenderCreateRoom();
	void RenderRoomInfo();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcList(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WndProcChatRoom(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int	WndProcChatRoomCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int	WndProcChatRoomInfo(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void UpdateBtnPos();

	// 2009-04-14 by bhsohn ä�� ���� �Ʒ��� ������, ������ ����
	void OutChatRoom();
	// end 2009-04-14 by bhsohn ä�� ���� �Ʒ��� ������, ������ ����


	POINT					m_ptCommunityPos;				// ������ ���� ��ġ.
	DWORD					m_nChatRoomTab;
	CINFImageEx *			m_pImgVoice;					// ���� ��� ������.	   
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx*			m_pImgBG;						// �̹��� ���.
#endif
	CINFImageEx *			m_pImgSpkVolBar;				// ���� ��� ��.
	INT						m_nPosVolum;					// ����Ŀ ����.
	POINT					m_ptOldVolumPos;
	BOOL					m_bVolumCon;
	CD3DHanFont*			m_pFontRoomNum;
	INT						m_nSelect;
	FLOAT					m_fTimeBtnGap;					// ��ư�� ������ �����°��� ����.
	
	vector<SCHATROOM_MEMBER_INFO>		m_vecOtherMember;	// �ٸ� ���� ��� ����.
	CHATROOM_INFO						m_stOtherRoomInfo;	// �ٸ� ���� ����.

	vector<SCHATROOM_MEMBER_INFO>		m_vecMyMember;		// ���� �� ��� ����.
	CHATROOM_INFO						m_stMyRoomInfo;		// ������ ����.


	//--------------------------------------------------------------------------//
	// ä�ù� ���.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage *		m_pImgRoomListBG;				// ä�ù� ��� ���.
#else
    CINFImageEx *			m_pImgRoomListBG;				// ä�ù� ��� ���.
#endif
	CINFImageEx *			m_pImgLock[2];
	CINFImageEx *			m_pImgSel;						// ���� �̹���.
	CINFArenaScrollBar *	m_pScrRoomList;					// 
	CINFImageBtn *			m_pBtnInfo;						// ���� ��ư.
	CINFImageBtn *			m_pBtnCreate;					// ���� ��ư.
	CINFImageBtn *			m_pBtnAdmission;				// ���� ��ư.
	CINFImageBtn *			m_pBtnRoomListUpdate;			// ���� ��ħ ��ư.
	DWORD					m_nPersonNum[CHATROOM_ADMISSION_NUMBER_COUNT];	// ���� �ο�
	vector<SCHATROOM_LIST_INFO>			m_vecRoomList;
	INT						m_nListTotalRoomCount;
	

	//--------------------------------------------------------------------------//
	
	//--------------------------------------------------------------------------//
	// ä�ù�.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage *		m_pImgRoomBg;
#else
	CINFImageEx *		m_pImgRoomBg;
#endif
	CINFArenaScrollBar *	m_pScrRoom;
	CINFImageEx *			m_pImgChatRoomSel;
	CINFImageEx *			m_pImgRoomChief;				// ����.
	CINFImageBtn *			m_pBtnCharge;					// ����.
	CINFImageBtn *			m_pBtnInvite;					// �ʴ�.
	CINFImageBtn *			m_pBtnOut;						// �߹�.
	CINFImageBtn *			m_pBtnExit;						// ������.
	CINFImageBtn *			m_pBtnVoiceOk;					// ���� ä�� Ȯ�ι�ư.
	CINFImageBtn *			m_pBtnVoiceCan;					// ���� ä�� ��ҹ�ư.
	CINFListBox	*			m_pLBChatRoom;					// ä�ù濡 ����Ʈ �ڽ�.
	CINFEditBox *			m_pEBChatRoomCS;				// ä�ù� ����.
	CINFImageRadioBtn *		m_pCBVoiceChat;					// ����ä�� ����.
	CINFImageEx	*			m_pImgRadioBtn[2];				// ����� ��ư.
	DWORD					m_nVoiceInput;					// 0 : Ű�Է� ��� 1 : ������� 
	CINFSecuEditBox*		m_pSEChatRoomSecu;				// ä�ù� ��� â.
	INT						m_nMyInfoTotCnt;
	//--------------------------------------------------------------------------//

	// ä�ù� ���� �����.
	BOOL					m_bShowCreate;					// ����� �������̽�.
	POINT					m_ptCreatePos;					// ����� ������ ��ġ.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage *		m_pRoomCreateBG;				// ����� ���.
#else
	CINFImageEx *	 	    m_pRoomCreateBG;				// ����� ���.
#endif
	CINFImageBtn *			m_pBtnCreateOk;					// Ȯ�� ��ư.
	CINFImageBtn *			m_pBtnCreateCan;				// ��� ��ư.
	CINFListBox *			m_pLBCreateRoom;				// �����â�� ����Ʈ �ڽ�.
	CINFEditBox *			m_pEBCreateCS;					// ���� ä�� ����.
	CINFSecuEditBox*		m_pSECreateSecu;				// ä�ù� ���� ����� ��� â.

	// ä�ù� ���� ������.
	BOOL					m_bShowRoomInfo;
	BOOL					m_bInfoMoving;					// ���� â �����̴����ΰ�?
	POINT					m_ptOldPos;
	POINT					m_ptChatRoomInfoPos;
	CINFArenaScrollBar *	m_pScrRoomInfo;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage *		m_pRoomInfoBG;					// ä�ù� ����â ���.
#else
	CINFImageEx *		    m_pRoomInfoBG;					// ä�ù� ����â ���.
#endif
	CINFImageBtn *			m_pBtnRoomInfoClose;
	INT						m_nOtherInfoTotCnt;

	int						m_nChatRoomMaxCount;			// 2013-01-24 by mspark, ä�ù� ���� �� ���� ��ũ�� �ȵǴ� ���� �ذ�
};

#endif // !defined(AFX_INFCOMMUNITYCHATROOM_H__4064B992_8044_4F20_A7EE_A944D683A1BE__INCLUDED_)

// INFCommunityUserInfo.h: interface for the CINFCommunityUserInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUNITYUSERINFO_H__8189F3B8_8DCF_4E5F_ADD6_5948612E8B26__INCLUDED_)
#define AFX_INFCOMMUNITYUSERINFO_H__8189F3B8_8DCF_4E5F_ADD6_5948612E8B26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFPilotFace;
class CINFGroupImage;
class CINFCommunityUserInfo  : public CINFBase
{
public:
	CINFCommunityUserInfo(CAtumNode* pParent);
	virtual ~CINFCommunityUserInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();

	void ShowUserInfo(MSG_FC_CHARACTER_GET_USER_INFO_OK* pMsg);
	void HideUserInfo();	// 2015-06-27 Future, hides the current User Info
	
private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	void GetUIPos(POINT *ptGMarkPos, POINT *ptGPos, POINT *ptClassPos, POINT *ptFamePos, POINT *ptRegionPos, POINT *ptLoginPos);
		
private:
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*	 m_pBkImage;
#else
	CINFImageEx*	m_pBkImage;	
#endif

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	

	CINFImageBtn*			m_pCloseBtn;			// �ݱ�	
	CD3DHanFont*			m_pFontText;	

	CINFPilotFace*			m_pPilotFace;			// ������
	CINFImageEx*				m_pInfoGuildCase;
	
	MSG_FC_CHARACTER_GET_USER_INFO_OK	m_stUserInfo;

	char			m_strUserName[128];				// ���� ĳ���� �̸�
	char			m_strUserGear[64];				// ���� ��ü ����
	char			m_strGuild[256];				// ���� ��� ����
	char			m_strClass[128];				// ���� ���
	char			m_strFame[16];					// ���� ��
	char			m_strLevel[16];					// ���� ����	
	char			m_strRegion[512];				// ������
	char			m_strLogOutTime[512];			// �ð�

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	char			m_strNickName[SIZE_MAX_ARENA_FULL_NAME];
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���


};

#endif // !defined(AFX_INFCOMMUNITYUSERINFO_H__8189F3B8_8DCF_4E5F_ADD6_5948612E8B26__INCLUDED_)

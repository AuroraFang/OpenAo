// INFInfinityField.h: interface for the CINFCityInfinityField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINFINITYFIELD_H__3A2F9FEB_F1A4_4EB5_B7C8_4FE712A1C613__INCLUDED_)
#define AFX_INFINFINITYFIELD_H__3A2F9FEB_F1A4_4EB5_B7C8_4FE712A1C613__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFDefaultWnd.h"

class CD3DHanFont;
class CINFImage;
class CINFImageEx;								   // 2011. 10. 10 by jskim UI�ý��� ����
class CINFImageBtn;
class CINFArenaScrollBar;
class CINFEditBox;
class CINFSingleCurselEditBox;

// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
// class CINFCityInfinityField  : public CINFDefaultWnd
// {
// public:
// 	typedef enum { INFINITY_DESC,		// ���� ȭ��
// 				   INFINITY_ZONESEL,	// ��� ����
// 				   INFINITY_ROOMSEL,	// ���� ����
// 				   INFINITY_INROOM,		// ����
// 				   INFINITY_MAX_STATE,	// �ִ� State
// 				   INFINITY_NONE
// 				}	INFINITY_STATE;
// 
// private:
// 	CD3DHanFont*	m_pInfinityFont;		// ��Ʈ
// 
// 	// ���� ȭ��
// 
// 	CINFImage*		m_pInfinityBackGround[ INFINITY_MAX_STATE ];	// �� �׶���
// 
// 	CINFArenaScrollBar*	m_pDescScroll;		// ���� ��ũ��
// 
// 	CINFImageBtn*	m_pDescOKBtn;			// ���� OK
// 
// 
// 	// ��� ����
// 
// 	CINFImageBtn*	m_pZoneEnterBtn;		// ��� ���Թ�ư
// 
// 	CINFImage*		m_pZoneSelect;			// ��� ����
// 
// 	CINFImage*		m_pZoneDisable;			// ��� ���� �Ұ�
// 
// 	CINFArenaScrollBar*	m_pZonSelScroll;	// ��� ��ũ��
// 
// 	std::map<MapIndex_t, CINFImage*>	m_mapInfinityMapImg;
// 
// 
// 	// ����
// 
// 	CINFImageBtn*	m_pRoomStartBtn;		// ���Ǵ�Ƽ ���� ��ư
// 
// 	CINFImage*		m_pRoomRdy;				// �غ�
// 
// 	CINFImage*		m_pJoinRequestBG;		// ���� ��û ��
// 
// 	CINFImage*		m_pRejectBG;			// ���� ��
// 
// 	CINFImage*		m_pRoomMaster;			// ���� ������
// 
// 	CINFImage*		m_pChatLanguageType[ INPUT_LANGUAGE_END ];
// 
// 	CINFImageBtn*	m_pRoomRdyBtn;			// �غ� ��ư
// 
// 	CINFImageBtn*	m_pRoomUnRdyBtn;		// �غ� ���� ��ư
// 
// 	CINFImageBtn*	m_pRoomBackBtn;			// ������ ��ư
// 
// 	CINFImageBtn*	m_pRoomRejectBtn;		// ���� ��ư
// 
// 	CINFImageBtn*	m_pRejectOKBtn;			// ���� ���� ��ư
// 
// 	CINFImageBtn*	m_pRejectCancelBtn;		// ���� ��ҹ�ư
// 
// 	CINFImageBtn*	m_pJoinRequestOK;		// ���� ���� ��ư
// 
// 	CINFImageBtn*	m_pJoinRequestCancel;	// ���� ��� ��ư
// 
// 	CINFArenaScrollBar*	m_pRoomPartyMemberScroll;	// ��Ƽ ��� ��ũ��
// 
// 	CINFArenaScrollBar*	m_pRoomDescScroll;	// ��� ���� ��ũ��
// 
// 	CINFArenaScrollBar*	m_pRoomChatScroll;	// ä�� ��ũ��
// 
// 	CINFSingleCurselEditBox*	m_pChatEditBox;	// ä�� ����Ʈ �ڽ�
// 
// 	std::list<std::string> m_ChatList;
// 
// 	std::vector<std::string> m_CullStringVec;
// 
// 	BOOL			m_bStartEnable;
// 
// 
// 	// ���� ����
// 
// 	CINFImageBtn*	m_pRoomSelBackBtn;		// �ڷΰ��� ��ư
// 
// 	CINFImageBtn*	m_pRoomSelCreateBtn;	// ���� ��ư
// 
// 	CINFImageBtn*	m_pRoomSelEnterBtn;		// ���� ��ư
// 
// 	CINFImageBtn*	m_pRoomCreateOK;		// ���� ���� ��ư
// 
// 	CINFImageBtn*	m_pRoomCreateCancel;	// ���� ��� ��ư
// 
// 	CINFImageBtn*	m_pRoomListRefreshBtn;	// �� ��� ���� ��ư
// 
// 	CINFImage*		m_pRoomSel;				// �� ����
// 
// 	CINFImage*		m_pRoomCreateBG;		// �� ���� ��׶���
// 
// 	CINFImage*		m_pRoomJoinWaitBG;		// �� ���� ��� ��׶���
// 
// 	CINFArenaScrollBar*	m_pRoomSelRoomScroll;	// �� ���� ��ũ��
// 
// 	CINFArenaScrollBar*	m_pRoomSelDescScroll;	// ��� ���� ��ũ��
// 
// 	CINFEditBox*	m_pRoomCreateTitleEdit;	// ������ ����Ʈ ��Ʈ��
// 
// 	BOOL			m_bRoomJoinWait;
// 
// 	CINFImageBtn*	m_pCloseXBtn;				// X�ݱ� ��ư
// 
// 
// 	INFINITY_STATE	m_InfinityState;		// ���Ǵ�Ƽ �� ����
// 
// public:
// 	CINFCityInfinityField();
// 	virtual ~CINFCityInfinityField();
// 
// 
// public:
// 	virtual HRESULT InitDeviceObjects();
// 	virtual HRESULT RestoreDeviceObjects();
// 	virtual HRESULT DeleteDeviceObjects();
// 	virtual HRESULT InvalidateDeviceObjects();
// 	void	UpdateBtnPos( void );
// 	virtual void	Render();
// 	virtual int		WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam ) ;
// 	virtual void	ShowWndEx( INFINITY_STATE nInfinityState, BOOL bShowWnd, POINT *ptPos = NULL, int nWndWidth = 0 );
// 
// public:
// 	void	RenderDesc( void );
// 	void	RenderZoneSel( void );
// 	void	RenderRoomSel( void );
// 	void	RenderInRoom( void );
// 
// 	int		WndProcDesc( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcZoneSel( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcRoomSel( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcInRoom( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 
// 	int		WndProcJoinRequest( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcReject( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcCreateRoom( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	int		WndProcWaitJoin( UINT uMsg, WPARAM wParam, LPARAM lParam );
// 
// 	void	Reset( void );
// 
// 	void	AddChat( const char* szChat );
// 	void	DisableChatControl( void );
// 	void	ClearChatList( void );
// 
// 	void	AddInfinityMapImg( const MapIndex_t nMapIdx );
// 
// 	void	SetRoomJoinWait( BOOL bWait );
// 
// 	void	SetStartEnable( BOOL bEnable );
// };

class CINFCityInfinityField : public CINFDefaultWnd
{
private:
	CD3DHanFont*	m_pFont;		// ��Ʈ

	CINFImageEx*		m_pDescBack;

	CINFArenaScrollBar*	m_pDescScroll;

	CINFImageBtn*	m_pDescOKBtn;

	CINFImageBtn*	m_pCloseXBtn;	// X�ݱ� ��ư

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage* m_pRenewInfiImage;															  
#endif

public:
	CINFCityInfinityField();
	virtual ~CINFCityInfinityField();

public:
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void	UpdateBtnPos( void );
	virtual void	Render();
	virtual int		WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	void	ShowWndEx( BOOL bShowWnd, POINT *ptPos = NULL, int nWndWidth = 0 );

public:
	void	Reset( void );
};

class CINFCityInfinityFieldPopUp : public CINFDefaultWnd
{
public:
	typedef enum { INFINITY_ZONESEL,	// ��� ����
				   INFINITY_ROOMSEL,	// ���� ����
				   INFINITY_INROOM,		// ����
				   INFINITY_MAX_STATE,	// �ִ� State
				   INFINITY_NONE
				}	INFINITY_STATE;

private:

	CD3DHanFont*	m_pFont;


	CD3DHanFont*	m_pFont_EditInfinityLevel;		// 2010. 06. 07 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.) - �Է°���. 



	CINFImageEx*		m_pInfinityBackGround[ INFINITY_MAX_STATE ];	// �� �׶���



	// ��� ����

	CINFImageBtn*	m_pZoneEnterBtn;		// ��� ���Թ�ư

	CINFImageEx*		m_pZoneSelect;			// ��� ����

	CINFImageEx*		m_pZoneDisable;			// ��� ���� �Ұ�

	CINFArenaScrollBar*	m_pZonSelScroll;	// ��� ��ũ��

	std::map<MapIndex_t, CINFImageEx*>	m_mapInfinityMapImg;



	// ����

	CINFImageBtn*	m_pRoomStartBtn;		// ���Ǵ�Ƽ ���� ��ư

	CINFImageEx*		m_pRoomRdy;				// �غ�

	CINFImageEx*		m_pJoinRequestBG;		// ���� ��û ��

	CINFImageEx*		m_pRejectBG;			// ���� ��

	CINFImageEx*		m_pRoomMaster;			// ���� ������

	CINFImageEx*		m_pChatLanguageType[ INPUT_LANGUAGE_END ];

	CINFImageBtn*	m_pRoomRdyBtn;			// �غ� ��ư

	CINFImageBtn*	m_pRoomUnRdyBtn;		// �غ� ���� ��ư

	CINFImageBtn*	m_pRoomBackBtn;			// ������ ��ư

	CINFImageBtn*	m_pRoomRejectBtn;		// ���� ��ư

	CINFImageBtn*	m_pRejectOKBtn;			// ���� ���� ��ư

	CINFImageBtn*	m_pRejectCancelBtn;		// ���� ��ҹ�ư

	CINFImageBtn*	m_pJoinRequestOK;		// ���� ���� ��ư

	CINFImageBtn*	m_pJoinRequestCancel;	// ���� ��� ��ư

	CINFArenaScrollBar*	m_pRoomPartyMemberScroll;	// ��Ƽ ��� ��ũ��

	CINFArenaScrollBar*	m_pRoomDescScroll;	// ��� ���� ��ũ��

	CINFArenaScrollBar*	m_pRoomChatScroll;	// ä�� ��ũ��

	CINFSingleCurselEditBox*	m_pChatEditBox;	// ä�� ����Ʈ �ڽ�

	std::list<std::string> m_ChatList;

	std::vector<std::string> m_CullStringVec;

	BOOL			m_bStartEnable;


	// 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.
	BOOL						m_bOpenDifficultPOPUP;

	INT							m_iRoomDifficultLevelEdit;

	CINFImageEx*					m_pRoomDifficultSetBG;			// ���̵� ���� BG.

	CINFImageBtn*				m_pRoomDifficultCloseXBtn;		// ���̵� ���� �ݱ�(X ����).

	CINFImageBtn*				m_pRoomDifficultCloseBtn;		// ���̵� ���� �ݱ�.

	CINFImageBtn*				m_pRoomDifficultSetBtn;			// ���̵� ���� ���� ��ư.

	CINFImageBtn*				m_pRoomDifficultOpenBtn;		// ���̵� ���� �˾� ���� ��ư.

	CINFImageBtn*				m_pRoomDifficultUpDownBtn[2];	// ���̵� ���� ���� ��ư (ȭ��ǥ ��ư).

	CINFSingleCurselEditBox*	m_pRoomDifficultLevelEditBox;	// ���̵� ���� �Է� ����Ʈ �ڽ�.

	// End. 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.

	// ���� ����

	CINFImageBtn*	m_pRoomSelBackBtn;		// �ڷΰ��� ��ư

	CINFImageBtn*	m_pRoomSelCreateBtn;	// ���� ��ư

	CINFImageBtn*	m_pRoomSelEnterBtn;		// ���� ��ư

	CINFImageBtn*	m_pRoomCreateOK;		// ���� ���� ��ư

	CINFImageBtn*	m_pRoomCreateCancel;	// ���� ��� ��ư

	CINFImageBtn*	m_pRoomListRefreshBtn;	// �� ��� ���� ��ư

	CINFImageEx*		m_pRoomSel;				// �� ����

	CINFImageEx*		m_pRoomCreateBG;		// �� ���� ��׶���

	CINFImageEx*		m_pRoomJoinWaitBG;		// �� ���� ��� ��׶���

	CINFArenaScrollBar*	m_pRoomSelRoomScroll;	// �� ���� ��ũ��

	CINFArenaScrollBar*	m_pRoomSelDescScroll;	// ��� ���� ��ũ��

	CINFEditBox*	m_pRoomCreateTitleEdit;	// ������ ����Ʈ ��Ʈ��

	BOOL			m_bRoomJoinWait;

	CINFImageBtn*	m_pCloseXBtn;			// X�ݱ� ��ư

	CINFImageBtn*	m_pMinimize;			// �ּ�ȭ ��ư

	CINFImageEx*		m_pMinimizeBKB;			// �ּ�ȭ �˾� ��׶���

	CINFImageEx*		m_pMinimizeBKW;			// �ּ�ȭ �˾� ��׶���

	CINFImageBtn*	m_pMaximizeBtn;			// �ִ�ȭ ��ư

	CINFImageBtn*	m_pClosePopupBtn;		// �˾� �ݱ� ��ư



	INFINITY_STATE	m_InfinityState;		// ���Ǵ�Ƽ �� ����

	BOOL			m_bWindowMaximized;		// �����찡 �ִ�ũ�� Ȱ��ȭ �����ΰ�?

	BOOL			m_bMoveWindow;			// �����츦 ������ �� �ִ°�

	POINT			m_MinimizeWindowPos;	// �ּ�ȭ�� â ��ġ

	POINT			m_PrevMousePos;			// ���� ���콺 ��ġ

	BOOL			m_bBlingMinimizeWindow;	// �ּ�ȭ�� �����Ÿ��°�?

	DWORD			m_nLastBlingTime;		// ���������� ������ �ð�

	CINFImageEx*		m_pBlingBK;				// ���� ������ ���

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*		m_pRenewInfiBackImage;

	CINFGroupImage*		m_pRenewInfiCreatImage;

	CINFGroupImage*		m_pCreateControl;	

	CINFGroupImage*		m_pRenewInfiJoinBackImage;
	
	CINFGroupImage*		m_pRenewInfiLevelImage;
	
	CINFGroupImage*		m_pRenewInfiRejectImage;

	CINFGroupImage*		m_pRenewInfiJoinRejectImage;

	CINFGroupImage*		m_pRoomDifficultSetGroup;
	CINFGroupImage*		m_pRoomSetMasterGroup;
	CINFGroupImage*		m_pRoomSetUnMasterGroup;
	CINFGroupImage*		m_pRoomSetCreateGroup;




#endif	
	
	
	
	


public:
	CINFCityInfinityFieldPopUp();
	virtual ~CINFCityInfinityFieldPopUp();

public:
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void	UpdateBtnPos( void );
	virtual void	Render();
	virtual int		WndProc( UINT uMsg, WPARAM wParam, LPARAM lParam );
	void	ShowWndEx( INFINITY_STATE nInfinityState, BOOL bShowWnd, POINT *ptPos = NULL, int nWndWidth = 0 );

public:
	void	RenderZoneSel( void );
	void	RenderRoomSel( void );
	void	RenderRoomJoin( void );
	void	RenderRoomCreate( void );
	void	RenderInRoom( void );
	void	RenderJoinRequest( void );
	void	RenderMemberBan( void );
	void	RenderMinimizeWindow( void );

	// 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.
	void	RenderRoomDifficultSetWnd( void );

	BOOL	RenderRoomSelToolTip_DifficultInfo ( POINT &pt );
	// End. 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.

	int		WndProcZoneSel( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcRoomSel( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcInRoom( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcMinimizeWindow( UINT uMsg, WPARAM wParam, LPARAM lParam );

	int		WndProcJoinRequest( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcReject( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcCreateRoom( UINT uMsg, WPARAM wParam, LPARAM lParam );
	int		WndProcWaitJoin( UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.
	int		WndProcRoomDifficultSetWnd( UINT uMsg , WPARAM wParam, LPARAM lParam );
	// End. 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.

	void	Reset( void );

	void	AddChat( const char* szChat );
	void	DisableChatControl( void );
	void	ClearChatList( void );

	void	AddInfinityMapImg( const MapIndex_t nMapIdx );

	void	SetRoomJoinWait( BOOL bWait );

	void	SetStartEnable( BOOL bEnable );

	// 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.
	void	SetRoomDifficultSetEnable ( BOOL bEnable );
	// End. 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.

	// 2010. 06. 03 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.)
	void	SetEnable_InputInfinityDifficultyLevel ( void );
	void	SetClose_InputInfinityDifficultyLevel ( bool bEnable );
	// End 2010. 06. 03 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.)

	// 2010. 06. 07 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.) - �Է°���.
	void	Update_Possible_InputInfinityDifficultyLevel ( void );
	// End 2010. 06. 07 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (����Ʈ �ڽ� �߰�.) - �Է°���.

	void	Minimize( void );
	void	Maximize( void );

	BOOL	IsInWndRect( POINT pt );

};
// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)

#endif // !defined(AFX_INFINFINITYFIELD_H__3A2F9FEB_F1A4_4EB5_B7C8_4FE712A1C613__INCLUDED_)
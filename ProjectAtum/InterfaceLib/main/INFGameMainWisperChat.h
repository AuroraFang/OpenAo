// CINFGameMainWisperChat.h: interface for the CINFGameMainWisperChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CINFGameMainWisperChat_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_)
#define AFX_CINFGameMainWisperChat_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "ChatTab_t.h"

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#define CHAT_CHATBOX_MAX_X							(g_pD3dApp->GetBackBufferDesc().Width * 2.0f/3.0f)  // �Ӹ� �޼����ڽ� �ִ�ũ�� X
#define CHAT_CHATBOX_MAX_Y							(g_pD3dApp->GetBackBufferDesc().Height / 2)			// �Ӹ� �޼����ڽ� �ִ�ũ�� Y

#define WISPERBOX_FIELD_MINIMUM_INIT_HEIGHT			63		// �ʵ忡�� �Ӹ� �˾�â�� �ּ�ȭ �ɶ� ������ y(�ػ󵵿��� �̰��� ������ y��ǥ�̴�)
#define WISPERBOX_CITY_MINIMUM_INIT_HEIGHT			108		// ���ÿ��� �Ӹ� �˾�â�� �ּ�ȭ �ɶ� ������ y(�ػ󵵿��� �̰��� ������ y��ǥ�̴�)

#define WISPERBOX_MINIMUM_WINDOW_BUTTON_X			(g_pD3dApp->GetBackBufferDesc().Width - 304)
#define WISPERBOX_MINIMUM_WINDOW_BUTTON_Y			2
#define WISPERBOX_MINIMUM_WINDOW_START_X			(g_pD3dApp->GetBackBufferDesc().Width - 304)
#define WISPERBOX_MINIMUM_WINDOW_START_Y			21
#define WISPERBOX_MINIMUM_WINDOW_HEIGHT				22
#define WISPERBOX_MINIMUM_WINDOW_WIDTH				160//140

#define CHAT_BOX_MAX_SIZE_START_X					200
#define CHAT_BOX_MAX_SIZE_START_Y					200

#define WISPERBOX_SYSTEM_MENU_SIZE_X				9
#define WISPERBOX_SYSTEM_MENU_SIZE_Y				9
#define WISPERBOX_SYSTEM_MENU_SPACE					3

#define CHAT_BOX_TOP_IMAGE_SIZE_X					19
#define CHAT_BOX_TOP_IMAGE_SIZE_Y					19
#define CHAT_BOX_BOTTOM_IMAGE_SIZE_X				19
#define CHAT_BOX_BOTTOM_IMAGE_SIZE_Y				5
#define WISPERBOX_INPUT_IMAGE_SIZE_X				4
#define WISPERBOX_INPUT_IMAGE_SIZE_Y				16
#define WISPERBOX_INPUT_RES_POS_X					19
#define MOVE_RAIL_POSITION_SPACE					30		// �Ӹ�â �̵��� ������ ��ġ����ŭ������ �ڵ����� ������ �̵�
#define WISPERBOX_NEWMESSAGE_TWINKLE_TIME			1

#define CHATBOX_SCROLL_IMAGE_TOP_GAP_Y				7		// 2012-04-02 by isshin ä��â ��ũ�� ���� ���� - ��ũ�� Y TOP_Gap
#define CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y			6		// 2012-04-02 by isshin ä��â ��ũ�� ���� ���� - ��ũ�� Y BOTTOM_Gap

#define CHAT_INPUT_LANGUAGE_X2						224
#define CHAT_INPUT_LANGUAGE_Y2						(g_pD3dApp->GetBackBufferDesc().Height - 27)
#define CHAT_INPUT_LANGUAGE_WH2						22

#define CHAT_BOX_INPUT_WIDTH						279
#define CHAT_BOX_INPUT_X							10
#define CHAT_BOX_MESSAGE_X							6
#else
#define CHAT_CHATBOX_MAX_X							(g_pD3dApp->GetBackBufferDesc().Width * 2.0f/3.0f)  // �Ӹ� �޼����ڽ� �ִ�ũ�� X
#define CHAT_CHATBOX_MAX_Y							(g_pD3dApp->GetBackBufferDesc().Height / 2)			// �Ӹ� �޼����ڽ� �ִ�ũ�� Y

#define WISPERBOX_FIELD_MINIMUM_INIT_HEIGHT			63		// �ʵ忡�� �Ӹ� �˾�â�� �ּ�ȭ �ɶ� ������ y(�ػ󵵿��� �̰��� ������ y��ǥ�̴�)
#define WISPERBOX_CITY_MINIMUM_INIT_HEIGHT			108		// ���ÿ��� �Ӹ� �˾�â�� �ּ�ȭ �ɶ� ������ y(�ػ󵵿��� �̰��� ������ y��ǥ�̴�)

#define WISPERBOX_MINIMUM_WINDOW_BUTTON_X			(g_pD3dApp->GetBackBufferDesc().Width - 304)
#define WISPERBOX_MINIMUM_WINDOW_BUTTON_Y			2
#define WISPERBOX_MINIMUM_WINDOW_START_X			(g_pD3dApp->GetBackBufferDesc().Width - 304)
#define WISPERBOX_MINIMUM_WINDOW_START_Y			21
#define WISPERBOX_MINIMUM_WINDOW_HEIGHT				22
#define WISPERBOX_MINIMUM_WINDOW_WIDTH				160//140

#define CHAT_BOX_MAX_SIZE_START_X					200
#define CHAT_BOX_MAX_SIZE_START_Y					200

#define WISPERBOX_SYSTEM_MENU_SIZE_X				9
#define WISPERBOX_SYSTEM_MENU_SIZE_Y				9
#define WISPERBOX_SYSTEM_MENU_SPACE					3		// 2012-04-02 by isshin ä��â ��ũ�� ���� ���� - (5 -> 3) ����

#define CHAT_BOX_TOP_IMAGE_SIZE_X					19
#define CHAT_BOX_TOP_IMAGE_SIZE_Y					19
#define CHAT_BOX_BOTTOM_IMAGE_SIZE_X				19
#define CHAT_BOX_BOTTOM_IMAGE_SIZE_Y				5
#define WISPERBOX_INPUT_IMAGE_SIZE_X				4
#define WISPERBOX_INPUT_IMAGE_SIZE_Y				16
#define WISPERBOX_INPUT_RES_POS_X					19
#define MOVE_RAIL_POSITION_SPACE					30		// �Ӹ�â �̵��� ������ ��ġ����ŭ������ �ڵ����� ������ �̵�
#define WISPERBOX_NEWMESSAGE_TWINKLE_TIME			1

#define CHATBOX_SCROLL_IMAGE_TOP_GAP_Y				7		// 2012-04-02 by isshin ä��â ��ũ�� ���� ���� - ��ũ�� Y TOP_Gap
#define CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y			6		// 2012-04-02 by isshin ä��â ��ũ�� ���� ���� - ��ũ�� Y BOTTOM_Gap

#define CHAT_INPUT_LANGUAGE_X2						224
#define CHAT_INPUT_LANGUAGE_Y2						(g_pD3dApp->GetBackBufferDesc().Height - 27)
#define CHAT_INPUT_LANGUAGE_WH2						22

#define CHAT_BOX_INPUT_WIDTH						279
#define CHAT_BOX_INPUT_X							10
#define CHAT_BOX_MESSAGE_X							6
#endif



#define SYSTEM_WINDOW_START_X						(g_pD3dApp->GetBackBufferDesc().Width - m_nSystemBoxWidth)//260)
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#define SYSTEM_WINDOW_START_Y						(g_pD3dApp->GetBackBufferDesc().Height - 183) 
#else 
#define SYSTEM_WINDOW_START_Y						(g_pD3dApp->GetBackBufferDesc().Height - 160)
#endif

#define SYSTEM_SWOW_NOT_SHOWBOX_LINE				6

class CINFImage;
class CD3DHanFont;
class CINFGameMainChat;
class CINFImageBtn;
class CINFImageBtnBoth;
class CINFImageEx;			// 2011. 10. 10 by jskim UI�ý��� ����

class CINFGameMainWisperChat : public CINFBase  
{ 
public:       
	CINFGameMainWisperChat(CAtumNode* pParent);
	virtual ~CINFGameMainWisperChat();

	void SetPresentClickedFlag(BOOL bFlag);

   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	
	void Render();
	void RenderOption();
	virtual void Tick();

	void AddBuffer(const char* strBuffer, DWORD i_dwColorType);
	void ResetHanFontLine();
	void DrawChatWindow(int x, int y, int cx, int cy);
	void DrawMiniChatWindow(int x, int y, int cx, int cy);
	int  ChangeCurrentLineIndex(int CurrentScrollHeight, int NumberOfLine, int ScrollbarLength);
	BOOL SetInterface(BOOL i_bShowInterface);
	BOOL GetInterface();
	void ClearChatTextBox();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	CINFGameMainChat			*m_pINFGameMainChat;
	int							m_nVectorIndex;													// CINFGameMainChat���� �����Ǵ� vector������ �ε���
	char						m_szWisperCharacterName[SIZE_MAX_STRING_128];					// �Ӹ� ���� CharacterName
	char						m_szWisperCharacterNameForRender[SIZE_MAX_STRING_128];			// 2017-01-13 used for rendering

// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx					*m_pWhisperBox[9];												// �Ӹ� �ڽ� �̹���
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx					*m_pWhisperMiniBox;												// �Ӹ� �ڽ� �̹���
#endif
	CINFImageBtn				*m_pWhisperBoxMin;												// �Ӹ� �ּ�ȭ ��ư.
	CINFImageBtnBoth			*m_pWhisperBoxVoice;											// �Ӹ� ���� ��ư.
	CINFImageBtn				*m_pWhisperBoxHide;												// �Ӹ� �������̽� �����.
	CINFImageBtn				*m_pWhisperBoxClose;											// �Ӹ� Ŭ���� ��ư.
// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx					*m_pWhisperBoxTextInput[3];										// �Ӹ� �ڽ� �̹���

	BOOL						m_bWindowSizeMax;												// �ִ� ��ư ���� ����
	BOOL						m_bWindowSizeMin;												// �ּ� ��ư ���� ����
	BOOL						m_bShowChatBox;													// ä�� �ڽ� ����ġ 
	BOOL						m_bShowInterface;												// �������̽� �ڽ� ���̴°�?
	BOOL						m_bChatMode;
	BOOL						m_bMoveChatBoxHandle;											// ä�� �ڽ� �̵� �ڵ�
	BOOL						m_bPresentClickedFlag;											// ä�� �ڽ��� ���õǾ������� �÷���
	BOOL						m_bScrollLock;													// SCROLL BAR �̵� �÷���
	BOOL						m_bShowNameFlag;												// �̴� �Ӹ�â�� �̸��� �����ִ� �÷���
	BOOL						m_bNewMassage;													// ���ο� �޼����� ������ �˷��ִ� �÷���



	int							m_nWisperBoxSizeChangeType;										// ä�� �ڽ� ������ ���� Type
	int							m_nWisperBoxWidth;												// ä�ùڽ��� ���� 
	int							m_nWisperBoxHeight;												// ä�ùڽ��� ����
	int							m_nWisperBoxStartX;												// ä�ùڽ��� ������ġ x 
	int							m_nWisperBoxStartY;												// ä�ùڽ��� ������ġ y
	int							m_nWisperBoxStartXOld;											// ä�ùڽ��� ������ġ ����x 
	int							m_nWisperBoxStartYOld;											// ä�ùڽ��� ������ġ ����y
	int							m_nWisperBoxWidthOld;											// ä�ùڽ��� ���� ���� ��x 
	int							m_nWisperBoxHeightOld;											// ä�ùڽ��� ���� ���� ��y
	int							m_nWisperBoxSortMiniSizePosY;									// ä�ùڽ��� �ּ�ȭ�϶� ����
	int							m_nWisperBoxArrowImage;											// ä�ùڽ��� ���콺 ����Ʈ 
	int							m_nCurrentRelScrollPosY;
	int							m_nCurrentRelScrollOldPosY;

	float						m_fTwinkleTime;													// ���ο� �޼��� �����̴� Ÿ�̹�
	
	POINT						m_pointBeforeMousePoints;										// ���콺 ������ǥ

	CD3DHanFont*				m_pFontInput;
	vector<CD3DHanFont*>		m_vecWisperFontLine;	
	char						m_strInputMessage[256];	

	ChatTab_t					m_WisperChatTab;
private:
	// 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�
	BOOL					IsChatingMode();
	void					SetChatMsgBuff(char* i_pImeChat);
	void					InitChatMsgBuff();
	void					RenderCursel();
	int						OnKeyDownCursel(WPARAM wParam, LPARAM lParam);
	int						OnKeyDownFunc(WPARAM wParam, LPARAM lParam);	
	int						OnKeyDownArrow(WPARAM wParam, LPARAM lParam);
	// end 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�

private:
	// 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�	
	POINT				m_ptCurselPos;
	char				m_strPreBackupMessage[SIZE_MAX_CHAT_MESSAGE];	// ��Ʈ�� �տ� ����
	char				m_strNextBackupMessage[SIZE_MAX_CHAT_MESSAGE];	// ��Ʈ�� �ڿ� ����
	char				m_strBkInputMessage[SIZE_MAX_CHAT_MESSAGE];		// ���� ����
	// end 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�
	
};




// 2008. 12. 29 by ckPark ä��â ����ȭ
#define	MAX_SYSTEM_MSG_LINE			60		// �ִ� �ý��� �޼��� ����
// end 2008. 12. 29 by ckPark ä��â ����ȭ



class CINFSystemMsgWindow : public CINFBase  
{ 
public:       
	// 2008-06-20 by bhsohn EP3 �ɼǰ��� ó��
	//CINFSystemMsgWindow(CAtumNode* pParent);
	CINFSystemMsgWindow(CAtumNode* pParent, int nStartX, int nStartY, int nWidth, int nHeight);
	virtual ~CINFSystemMsgWindow();


   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void SetPresentClickedFlag(BOOL bFlag);
	BOOL SetInterface(BOOL i_bShowInterface);
	BOOL GetInterface();
	void Render();
	virtual void Tick();
	void ResetHanFontLine();
	void AddBuffer(const char* strBuffer, DWORD i_dwColorType);
	void DrawChatWindow(int x, int y, int cx, int cy);
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	stcuctRateRect GetSysMsgWndRect();

	// 2009-02-19 by bhsohn �ý���â ��ġ ���� �ȳѾ�� ��
	void	RefreshSystemBox();

public:
	CINFGameMainChat			*m_pINFGameMainChat;
	int							m_nVectorIndex;													// CINFGameMainChat���� �����Ǵ� vector������ �ε���
	char						m_szSystemCharacterName[SIZE_MAX_STRING_128];					// �Ӹ� ���� CharacterName

// 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx					*m_pSystemBox[9];												// �Ӹ� �ڽ� �̹���
	CINFImageEx					*m_pSystemBoxMin;												// �Ӹ� �ڽ� �̹���
	CINFImageEx					*m_pSystemBoxClose;												// �Ӹ� �ڽ� �̹���
	CINFImageEx					*m_pSystemBoxName;												// �ý��� �ڽ� �̸�. 
// end 2011. 10. 10 by jskim UI�ý��� ����

	BOOL						m_bWindowSizeMax;												// �ִ� ��ư ���� ����
	BOOL						m_bWindowSizeMin;												// �ּ� ��ư ���� ����
	BOOL						m_bShowChatBox;													// ä�� �ڽ� ����ġ 
	BOOL						m_bShowInterface;												// �������̽��� ���̴°�?
	BOOL						m_bMoveChatBoxHandle;											// ä�� �ڽ� �̵� �ڵ�
	BOOL						m_bPresentClickedFlag;											// ä�� �ڽ��� ���õǾ������� �÷���
	BOOL						m_bScrollLock;													// SCROLL BAR �̵� �÷���
	BOOL						m_bShowNameFlag;												// �̴� �Ӹ�â�� �̸��� �����ִ� �÷���
	BOOL						m_bNewMassage;													// ���ο� �޼����� ������ �˷��ִ� �÷���

	int							m_nSystemBoxSizeChangeType;										// ä�� �ڽ� ������ ���� Type
	int							m_nSystemBoxWidth;												// ä�ùڽ��� ���� 
	int							m_nSystemBoxHeight;												// ä�ùڽ��� ����
	int							m_nSystemBoxStartX;												// ä�ùڽ��� ������ġ x 
	int							m_nSystemBoxStartY;												// ä�ùڽ��� ������ġ y
	int							m_nSystemBoxStartXOld;											// ä�ùڽ��� ������ġ ����x 
	int							m_nSystemBoxStartYOld;											// ä�ùڽ��� ������ġ ����y
	int							m_nSystemBoxWidthOld;											// ä�ùڽ��� ���� ���� ��x 
	int							m_nSystemBoxHeightOld;											// ä�ùڽ��� ���� ���� ��y
	int							m_nSystemBoxSortMiniSizePosY;									// ä�ùڽ��� �ּ�ȭ�϶� ����
	int							m_nSystemBoxArrowImage;											// ä�ùڽ��� ���콺 ����Ʈ 
	int							m_nCurrentRelScrollPosY;
	int							m_nCurrentRelScrollOldPosY;

	float						m_fTwinkleTime;													// ���ο� �޼��� �����̴� Ÿ�̹�
	
	POINT						m_pointBeforeMousePoints;										// ���콺 ������ǥ

	
	// 2008. 12. 29 by ckPark ä��â ����ȭ
	//CD3DHanFont*				m_pFontInput;
	// �� ���κ� ��Ʈ
	CD3DHanFont*				m_pFontInput[MAX_SYSTEM_MSG_LINE];
	// end 2008. 12. 29 by ckPark ä��â ����ȭ


	vector<CD3DHanFont*>		m_vecSystemFontLine;	
	char						m_strInputMessage[256];	

	ChatTab_t					m_SystemChatTab;
	float						m_fTimeOfShowChat[SYSTEM_SWOW_NOT_SHOWBOX_LINE];


	
	// 2008. 12. 29 by ckPark ä��â ����ȭ
	// �ø��� ���ڿ��� ������ �ڷ���
	typedef std::list< vector<string> >				CulledStringList;
	typedef std::list< vector<string> >::iterator	CulledStringListItor;
	
	CulledStringList	m_listCulledString;		// �ø��� ���ڿ��� ������ ����
	
	
	void	BuildCulledString( void );			// �ø��� ���ڿ��� �����

	
	// end 2008. 12. 29 by ckPark ä��â ����ȭ


};
#endif

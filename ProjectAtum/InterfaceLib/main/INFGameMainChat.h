// INFGameMainChat.h: interface for the CINFGameMainChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMECHAT_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_)
#define AFX_INFGAMECHAT_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "ChatTab_t.h"
#include "INFImageBtn.h"
#include "INFImageBtnBoth.h"
#include "INFGroupImage.h"

// 2007-08-07 by bhsohn ����Ŀ ������ �߰�
//#define CHAT_TAB_NUMBER						6							// ä���� ��
#define CHAT_TAB_NUMBER						7							// ä���� ��
// 2008-07-01 by dgwoo ����Ŀ ������ 
#define VOICECHAT_SPK_W						65
#define CHAT_MENULIST_NUMBER				13

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
#define CHAT_NOT_SHOWBOX_LINE				6							// ä�� �ڽ��� ������ ������ �ִ뺸���ִ� �ټ�
#define CHAT_TIME_OF_NOT_SHOW_BOX			10.0f						// ä�� �ڽ��� ������ ������ �ִ뺸���ִ� �ð� ����
#define CHAT_TIME_OF_NOT_SHOW_BOX_GM		20.0f						// ä�� �ڽ��� ������ ������ �ִ뺸���ִ� �ð�
#define CHAT_CHATBOX_START_DEFAULT_MIN_X	280							// ä�� �ڽ� ���۽� �ּ� X 
#define CHAT_CHATBOX_START_DEFAULT_MIN_Y	120							// ä�� �ڽ� ���۽� �ּ� Y

#define CHAT_CHATBOX_START_DEFAULT_MAX_X	(g_pD3dApp->GetBackBufferDesc().Width * 2.0f/3.0f)	// ä�� �ڽ� ���۽� �ִ� X 
#define CHAT_CHATBOX_START_DEFAULT_MAX_Y	(g_pD3dApp->GetBackBufferDesc().Height / 2)			// ä�� �ڽ� ���۽� �ִ� Yt

#define CHAT_BUTTON_NORMAL					0							// ��ư���� �븻
#define CHAT_BUTTON_DOWN					1							// ��ư���� �ٿ�
#define CHAT_BUTTON_UP						2							// ��ư���� ��

#define CHAT_SELECT_IMAGE_0					0							// �̹��� �ε��� 0 ������ 
#define CHAT_SELECT_IMAGE_1					1							// �̹��� �ε��� 0 ������ 
#define CHAT_SELECT_IMAGE_2					2							// �̹��� �ε��� 0 ������ 

// 2008-08-19 by bhsohn ä�ù��� ���� ����
//#define CHAT_TAB_SAVE_COUNT						CHAT_TAB_HELPDESK			// ����Ǵ� ä�ü� (����Ŀ,��� ä��,�ֺ�, ����, �ŷ�, ��, ����, ���, �Ʒ���, ä�ù�, �ý���)
#define CHAT_TAB_SAVE_COUNT						CHAT_TAB_NUMBER_ALL			// ����Ǵ� ä�ü� (����Ŀ,��� ä��,�ֺ�, ����, �ŷ�, ��, ����, ���, �Ʒ���, ä�ù�, �ý���)

#define CHAT_FONT_LINE_HEIGHT				15							// �� ������ ����
#define CHAT_FONT_WIDTH_ENGLISH				6							// ���� ���� WIDTH

#define CHAT_CHATBOX_START_X				0							// ä�ùڽ� ������ġ X
#define CHAT_CHATBOX_START_Y				(g_pD3dApp->GetBackBufferDesc().Height - CHAT_CHATBOX_START_DEFAULT_MIN_Y)		// ä�ùڽ� ������ġ Y

#define CHATBOX_FIELD_SHOWCHATBOX_INIT_HEIGHT			60				// �ʵ忡�� �Ӹ� �˾�â�� �ּ�ȭ �ɶ� ������ y(�ػ󵵿��� �̰��� ������ y��ǥ�̴�)
#define CHATBOX_CITY_SHOWCHATBOX_INIT_HEIGHT			70				// ���ÿ��� �Ӹ� �˾�â�� �ּ�ȭ �ɶ� ������ y(�ػ󵵿��� �̰��� ������ y��ǥ�̴�)

#define CHAT_MENUBOX_START_HEIGHT			38							// ä�þƷ� �޴��ڽ� ���۽� ũ��

#define CHAT_BOX_SHOW_IMAGE					0							// ä�ùڽ� ���̱� �̹��� �ε��� 
#define CHAT_BOX_HIDE_IMAGE					1							// ä�ùڽ� ���߱� �̹��� �ε��� 

#define CHAT_MENUBOX_START_X				1							// �޴��ڽ� ������ġ X
#define CHAT_MENUBOX_WIDTH					48							// �޴��ڽ� ���� 
#define CHAT_MENUBOX_HEITHT					15							// �޴��ڽ� ���� 
#define CHAT_MENUBOX_SELECT_COUNT			5							// �޴��ڽ� ����  
#define CHAT_MENUBOX_SELECT_STATE			2							// �޴��ڽ� ����
#define CHAT_MENUBOX_GAB_WIDTH				4							// �޴��ڽ� ���� ����
#define CHAT_MENUBOX_START_Y				(g_pD3dApp->GetBackBufferDesc().Height - 20)		// �޴��ڽ� ������ġ Y

#define CHAT_FONT_START_Y					(g_pD3dApp->GetBackBufferDesc().Height - 23)//30)		// �� �Ʒ� ������ ������(�� �Ʒ����� �׸���) : Y
#define CHAT_FONT_START_X					55//31							// ä�� ���� ���� : X
#define CHAT_INPUT_FONT_LENGTH              240//189				// ä���Է�â ����
#define CHAT_STRING_SIZE_GM					84

// �ɼ� ��ư.
#define CHAT_OPTION_BUTTON_X				309
#define CHAT_OPTION_BUTTON_Y				(g_pD3dApp->GetBackBufferDesc().Height - 34)

#define CHATBOX_CLOSE_GAB					27							// ä�ùڽ� ���߱� Heith - 27
#define CHATBOX_IMAGE_GAB_WIDTH_TOP			18							// ä�ùڽ� �̹��� ũ�� W Top
#define CHATBOX_IMAGE_GAB_HEITHT_TOP		18							// ä�ùڽ� �̹��� ũ�� H Top
#define CHATBOX_IMAGE_GAB_WIDTH_BOTTOM		19							// ä�ùڽ� �̹��� ũ�� W Bottom
#define CHATBOX_IMAGE_GAB_HEITHT_BOTTOM		5							// ä�ùڽ� �̹��� ũ�� H Bottom
#define CHATBOX_IMAGE_GAB_WIDTH_MIDDLE		4							// ä�ùڽ� �̹��� ũ�� W Middle
#define CHATBOX_IMAGE_GAB_HEITHT_MIDDLE		1							// ä�ùڽ� �̹��� ũ�� H Middle
#define CHATBOX_IMAGE_GAB_WINDOWSIZE_CHANGE	4							// ä�ùڽ� �̹��� ũ�� ����.
#define CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH	9							// ä�ùڽ� �̹��� ũ�� �ּ�ȭ ��ư WIDTH
#define CHATBOX_IMAGE_GAB_MINIMIZE_HEIGHT	12							// ä�ùڽ� �̹��� ũ�� �ּ�ȭ ��ư HEIGHT
#define CHATBOX_IMAGE_NUMBER				9							// ä�ùڽ� �̹��� ���� 																				
#define CHATBOX_IMAGE_SCROLL_LINE_NUMBER	3							// ��ũ�� ���� �̹��� ���� 

#define CHATBOX_SCROLLLINE_IMAGE_GAP_X		7							// ��ũ�� ���� X Gap 
#define CHATBOX_SCROLL_IMAGE_GAP_X			5//4						// ��ũ�� �̹��� X Gap		// 2012-03-29 by mspark, ä��â ��ũ�ѹ� ��ġ ���� - ���� 3���� 5�� ����
#define CHATBOX_SCROLL_IMAGE_TOP_GAP_Y		7							// 2012-04-02 by isshin ä��â ��ũ�� ���� ���� - ��ũ�� Y TOP_Gap
#define CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y	6							// 2012-04-02 by isshin ä��â ��ũ�� ���� ���� - ��ũ�� Y BOTTOM_Gap
#define CHATBOX_SCROLL_IMAGE_SIZE_WIDTH		11							// ��ũ�� �̹��� ���� 
#define CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT	22							// ��ũ�� �̹��� ����

#define CHAT_OTHER_MENU_ALL					3							// ��Ÿ �޴� ��ü ��

#define CHAT_MACRO_INPUT_COUNT				3							// ��ũ�� �Է� ������ ��.
#define CHAT_MACRO_PRINT_GAP				10.0f		  
#else		   
#define CHAT_NOT_SHOWBOX_LINE				6							// ä�� �ڽ��� ������ ������ �ִ뺸���ִ� �ټ�
#define CHAT_TIME_OF_NOT_SHOW_BOX			10.0f						// ä�� �ڽ��� ������ ������ �ִ뺸���ִ� �ð� ����
#define CHAT_TIME_OF_NOT_SHOW_BOX_GM		20.0f						// ä�� �ڽ��� ������ ������ �ִ뺸���ִ� �ð�
#define CHAT_CHATBOX_START_DEFAULT_MIN_X	253							// ä�� �ڽ� ���۽� �ּ� X 
#define CHAT_CHATBOX_START_DEFAULT_MIN_Y	120							// ä�� �ڽ� ���۽� �ּ� Y

#define CHAT_CHATBOX_START_DEFAULT_MAX_X	(g_pD3dApp->GetBackBufferDesc().Width * 2.0f/3.0f)	// ä�� �ڽ� ���۽� �ִ� X 
#define CHAT_CHATBOX_START_DEFAULT_MAX_Y	(g_pD3dApp->GetBackBufferDesc().Height / 2)			// ä�� �ڽ� ���۽� �ִ� Y

#define CHAT_BUTTON_NORMAL					0							// ��ư���� �븻
#define CHAT_BUTTON_DOWN					1							// ��ư���� �ٿ�
#define CHAT_BUTTON_UP						2							// ��ư���� ��

#define CHAT_SELECT_IMAGE_0					0							// �̹��� �ε��� 0 ������ 
#define CHAT_SELECT_IMAGE_1					1							// �̹��� �ε��� 0 ������ 
#define CHAT_SELECT_IMAGE_2					2							// �̹��� �ε��� 0 ������ 

// 2008-08-19 by bhsohn ä�ù��� ���� ����
//#define CHAT_TAB_SAVE_COUNT						CHAT_TAB_HELPDESK			// ����Ǵ� ä�ü� (����Ŀ,��� ä��,�ֺ�, ����, �ŷ�, ��, ����, ���, �Ʒ���, ä�ù�, �ý���)
#define CHAT_TAB_SAVE_COUNT						CHAT_TAB_NUMBER_ALL			// ����Ǵ� ä�ü� (����Ŀ,��� ä��,�ֺ�, ����, �ŷ�, ��, ����, ���, �Ʒ���, ä�ù�, �ý���)

#define CHAT_FONT_LINE_HEIGHT				15							// �� ������ ����
#define CHAT_FONT_WIDTH_ENGLISH				6							// ���� ���� WIDTH

#define CHAT_CHATBOX_START_X				0							// ä�ùڽ� ������ġ X
#define CHAT_CHATBOX_START_Y				(g_pD3dApp->GetBackBufferDesc().Height - CHAT_CHATBOX_START_DEFAULT_MIN_Y)		// ä�ùڽ� ������ġ Y

#define CHATBOX_FIELD_SHOWCHATBOX_INIT_HEIGHT			44				// �ʵ忡�� �Ӹ� �˾�â�� �ּ�ȭ �ɶ� ������ y(�ػ󵵿��� �̰��� ������ y��ǥ�̴�)
#define CHATBOX_CITY_SHOWCHATBOX_INIT_HEIGHT			89				// ���ÿ��� �Ӹ� �˾�â�� �ּ�ȭ �ɶ� ������ y(�ػ󵵿��� �̰��� ������ y��ǥ�̴�)

#define CHAT_MENUBOX_START_HEIGHT			38							// ä�þƷ� �޴��ڽ� ���۽� ũ��

#define CHAT_BOX_SHOW_IMAGE					0							// ä�ùڽ� ���̱� �̹��� �ε��� 
#define CHAT_BOX_HIDE_IMAGE					1							// ä�ùڽ� ���߱� �̹��� �ε��� 

#define CHAT_MENUBOX_START_X				5							// �޴��ڽ� ������ġ X
#define CHAT_MENUBOX_WIDTH					48							// �޴��ڽ� ���� 
#define CHAT_MENUBOX_HEITHT					15							// �޴��ڽ� ���� 
#define CHAT_MENUBOX_SELECT_COUNT			5							// �޴��ڽ� ����  
#define CHAT_MENUBOX_SELECT_STATE			2							// �޴��ڽ� ����
#define CHAT_MENUBOX_GAB_WIDTH				4							// �޴��ڽ� ���� ����
#define CHAT_MENUBOX_START_Y				(g_pD3dApp->GetBackBufferDesc().Height - 20)		// �޴��ڽ� ������ġ Y

#define CHAT_FONT_START_Y					(g_pD3dApp->GetBackBufferDesc().Height - 23)//30)		// �� �Ʒ� ������ ������(�� �Ʒ����� �׸���) : Y
#define CHAT_FONT_START_X					55//31							// ä�� ���� ���� : X
#define CHAT_INPUT_FONT_LENGTH              240//189				// ä���Է�â ����
#define CHAT_STRING_SIZE_GM					84

// �ɼ� ��ư.
#define CHAT_OPTION_BUTTON_X				310
#define CHAT_OPTION_BUTTON_Y				(g_pD3dApp->GetBackBufferDesc().Height - 18)

#define CHATBOX_CLOSE_GAB					27							// ä�ùڽ� ���߱� Heith - 27
#define CHATBOX_IMAGE_GAB_WIDTH_TOP			18							// ä�ùڽ� �̹��� ũ�� W Top
#define CHATBOX_IMAGE_GAB_HEITHT_TOP		18							// ä�ùڽ� �̹��� ũ�� H Top
#define CHATBOX_IMAGE_GAB_WIDTH_BOTTOM		19							// ä�ùڽ� �̹��� ũ�� W Bottom
#define CHATBOX_IMAGE_GAB_HEITHT_BOTTOM		5							// ä�ùڽ� �̹��� ũ�� H Bottom
#define CHATBOX_IMAGE_GAB_WIDTH_MIDDLE		4							// ä�ùڽ� �̹��� ũ�� W Middle
#define CHATBOX_IMAGE_GAB_HEITHT_MIDDLE		1							// ä�ùڽ� �̹��� ũ�� H Middle
#define CHATBOX_IMAGE_GAB_WINDOWSIZE_CHANGE	4							// ä�ùڽ� �̹��� ũ�� ����.
#define CHATBOX_IMAGE_GAB_MINIMIZE_WIDTH	9							// ä�ùڽ� �̹��� ũ�� �ּ�ȭ ��ư WIDTH
#define CHATBOX_IMAGE_GAB_MINIMIZE_HEIGHT	12							// ä�ùڽ� �̹��� ũ�� �ּ�ȭ ��ư HEIGHT
#define CHATBOX_IMAGE_NUMBER				9							// ä�ùڽ� �̹��� ���� 																				
#define CHATBOX_IMAGE_SCROLL_LINE_NUMBER	3							// ��ũ�� ���� �̹��� ���� 

#define CHATBOX_SCROLLLINE_IMAGE_GAP_X		7							// ��ũ�� ���� X Gap 
#define CHATBOX_SCROLL_IMAGE_GAP_X			5//4						// ��ũ�� �̹��� X Gap		// 2012-03-29 by mspark, ä��â ��ũ�ѹ� ��ġ ���� - ���� 3���� 5�� ����
#define CHATBOX_SCROLL_IMAGE_TOP_GAP_Y		7							// 2012-04-02 by isshin ä��â ��ũ�� ���� ���� - ��ũ�� Y TOP_Gap
#define CHATBOX_SCROLL_IMAGE_BOTTOM_GAP_Y	6							// 2012-04-02 by isshin ä��â ��ũ�� ���� ���� - ��ũ�� Y BOTTOM_Gap
#define CHATBOX_SCROLL_IMAGE_SIZE_WIDTH		11							// ��ũ�� �̹��� ���� 
#define CHATBOX_SCROLL_IMAGE_SIZE_HEIGHT	38							// ��ũ�� �̹��� ����

#define CHAT_OTHER_MENU_ALL					3							// ��Ÿ �޴� ��ü ��

#define CHAT_MACRO_INPUT_COUNT				3							// ��ũ�� �Է� ������ ��.
#define CHAT_MACRO_PRINT_GAP				10.0f
#endif

struct sCHATMACROSTRING
{
	char		strMacro[SIZE_MAX_CHAT_MESSAGE];
	BOOL		bAppl;
	float		fTime;

	sCHATMACROSTRING()
	{
		memset(strMacro,0x00,SIZE_MAX_CHAT_MESSAGE);
		bAppl		= FALSE;
		fTime		= 0.0f;
	};

};

typedef enum 
{
	NO_CHANGE_SIZE			= 0,
	LEFT_CHANGE_SIZE		= 1,
	RIGHT_CHANGE_SIZE		= 2,
	BOTTOM_CHANGE_SIZE		= 3,
	TOP_CHANGE_SIZE			= 4,
	LEFTTOP_CHANGE_SIZE		= 5,
	RIGHTTOP_CHANGE_SIZE	= 6,
	LEFTBOTTOM_CHANGE_SIZE	= 7,
	RIGHTBOTTOM_CHANGE_SIZE	= 8	
} CHAT_CHANGE_SIZE_TYPE;

typedef enum 
{
	NO_ARROW_IMAGE			= 0,
	LEFT_ARROW_IMAGE		= 8,
	RIGHT_ARROW_IMAGE		= 8,
	BOTTOM_ARROW_IMAGE		= 9,
	TOP_ARROW_IMAGE			= 9,
	LEFTTOP_ARROW_IMAGE		= 6,
	RIGHTTOP_ARROW_IMAGE	= 7,
	LEFTBOTTOM_ARROW_IMAGE	= 7,
	RIGHTBOTTOM_ARROW_IMAGE	= 6,
	MOVE_ARROW_IMAGE		= 10
} CHAT_CHANGE_ARROW_IMAGE;

// �߰� ���� ä��
#define CHAT_BUTTON_NUMBER3			3	
#define CHAT_BUTTON_NUMBER4			4



// 2008. 12. 29 by ckPark ä��â ����ȭ
#define MAX_GAME_MAIN_CHAT_LINE		60				// �ִ� ä�� ���μ�
// end 2008. 12. 29 by ckPark ä��â ����ȭ

// 2010. 02. 23 by jskim ä�ù��� ���� �� ����ȭ
enum
{
	LEFT_BLOCK,
	RIGHT_BLOCK
};

#define IS_BLOCK_TYPE(x1,x2) x1 < x2  ? LEFT_BLOCK : RIGHT_BLOCK
//end 2010. 02. 23 by jskim ä�ù��� ���� �� ����ȭ

class CINFImage;
class CD3DHanFont;
class CINFSystemMsgWindow;
class CINFGameMainWisperChat;
class CINFGroupManager;
class CINFGroupImage;

typedef vector<CINFGameMainWisperChat*>	vectorINFGameMainWisperChatPtr;
class CINFGameMainChat : public CINFBase  
{
public:
	// 2009. 02. 03 by ckPark ä��â �������̽� ũ�� ����
	//CINFGameMainChat(CAtumNode* pParent);								// ������
	CINFGameMainChat(CAtumNode* pParent, int nWidth, int nHeight);		// ������


	// end 2009. 02. 03 by ckPark ä��â �������̽� ũ�� ����



	virtual ~CINFGameMainChat();										// �Ҹ���
	void InitChatData();
   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcWhisperChat(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			WndProcMacro(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//int			WndProcVoiceOption(UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 2008-07-10 by bhsohn �ý��� â �켱���� �߰�
	int			WndProcSystemMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void		RenderSystemMsg();
	
	void Render();
	void RenderWisperChat();
	void ResetHanFontLine();
	void RenderMacro();
	//void RenderVoiceChatOption();
	virtual void Tick();
	void TickMacro();

	void AddBuffer(
		int nType, const char* strBuffer, DWORD dwColorType,
		const char* szWisperToCharacterName = nullptr,
		const char* szWisperFromCharacterName = nullptr,
		BYTE NameColor = 0);

	void DrawChatWindow(int x, int y, int cx, int cy);
	//void OnSoundButtonClick();
	//void OnVoiceChatInviteButtonClick();
	int  ChangeCurrentLineIndex(int CurrentScrollHeight, int ChatBoxHeight, int NumberOfLine, int ScrollbarLength);

	BOOL IsValidTabIndex(int i_nTabIdx);
	void ProcessWisperChatAddLine(const char* strBuffer, DWORD i_dwColorType, const char* szWisperToCharacterName=NULL, const char *szWisperFromCharacterName=NULL, BYTE NameColor=0);// �Ӹ��� WisperChat �����̳� ���� WisperChat�� �Է�
	void SetWisperBoxMinimumStatusInitPosition(int i_nPosX, int i_nPosY);					// �Ӹ� ��ȭ���� �ּ�ȭ�� ������ SetUp
	
	// �߰� ���� ä�� �޴�	05.01.21
	void ViewChatMenuList();
	void EnterCashShop();
	void CheckShopInfo();
	
	//void SetVOIPExit();
	void SetCleanChat();

	BOOL CheckChatTypeMsg(WPARAM wParam, LPARAM lParam);

	// 2007-07-04 by bhsohn ���۷����� �߰�
	void OnClickFAQ();
	
	// 2008-02-27 by bhsohn �Ʒ��� �̵��� ä������ ���?
	void GetAllChatList(ChatLine_t pArrChatLine[CHAT_BUFFER_NUMBER]);

	// 2008-05-08 by dgwoo ä�� �ý��� ���� �ҽ� �߰�.
	ChatTab_t * GetChatTabMode();
	void ActiveChatOption(BOOL i_bActive);
	void SetChatRoomAct(BOOL i_bActive);
	void SetChatArenaAct(BOOL i_bActive);

	INT WhisperChatSmallWindowCountPlus();
	INT WhisperChatSmallWindowCountMinus(INT i_nIndex);
	void SetWisperChatShowInterface(BOOL i_bInterface);
	void StartMacro();
	void EndMacro();
	//void SetVoiceChatVolum(DWORD i_nVolum);						// ����ä�� ���� ����.
	//void ShowVoiceChatControl(BOOL i_bShow);
	//void SendVoiceGuild();
	//void SendVoiceParty();
	//BOOL EndVoiceGuild();
	//BOOL EndVoiceParty();

	// 2008-07-14 by bhsohn EP3 ���� ó��
	BOOL IsChatingMode();
	//void UpdateVoiceChatType(BYTE i_bVoiceType);

	// 2008-10-20 by bhsohn VOIP ���� ����
	//BOOL PopupVOIPError(BYTE byRqVoiceType);
	//void UpdateVoiceVolumScroll();


	// 2009. 02. 03 by ckPark ä��â �������̽� ũ�� ����

	stcuctRateRect GetChatWndRect(void);

	// end 2009. 02. 03 by ckPark ä��â �������̽� ũ�� ����

	void InitChatMsgBuff();		// 2013-02-13 by mspark, ä��â�� ���� ���� Ư�� ��Ȳ���� ������� ���� �ذ� - public �Լ��� ����

	bool ChatModeChack(char i_Ctr); // 2013-06-20 by bhsohn Ư�� ��Ȳ ä�� ���� ���󰡴� ���� ����


private:
	// 2008-05-08 by dgwoo ä�� �ý��� ���� �ҽ� �߰�.
	DWORD ChangeChatMode(DWORD i_nChatMode);

	// 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�
	void RenderCursel(int nShowTemp);

	int OnKeyUpFunc(WPARAM wParam, LPARAM lParam);

	int OnKeyDownCursel(WPARAM wParam, LPARAM lParam);
	int OnKeyDownArrow(WPARAM wParam, LPARAM lParam);
	int OnKeyDownFunc(WPARAM wParam, LPARAM lParam);
	
	//void InitChatMsgBuff();		// 2013-02-13 by mspark, ä��â�� ���� ���� Ư�� ��Ȳ���� ������� ���� �ذ� - public �Լ��� �����ϱ� ���� �ּ�
	void SetChatMsgBuff(char* i_pImeChat);	

	// Ŭ������ ���� ó��
	void SetClipBoard(char* i_pStr);
	void PasteFromClipBoard(char* o_pStrTxt, int nSize);
	void GetCurselString(char* o_pStrTxt);
	// end 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�
	// 2010. 02. 23 by jskim ä�ù��� ���� �� ����ȭ		
	// 2009. 07. 30 by jsKim ä��â ���� ���� �� ��� ����
	// ������ ����
	// 2010. 02. 23 by jskim ä�ù��� ���� �� ����ȭ		
    //void BlockCleanChat(int chLen);
	void BlockCleanChat(int LeftBlockPos,int RightBlockPos);
	//end 2010. 02. 23 by jskim ä�ù��� ���� �� ����ȭ		
	// ä�ø�� üũ
//	bool ChatModeChack(char i_Ctr);
	// end 2009. 07. 30 by jsKim ä��â ���� ���� �� ��� ����

	void GetStrPara ( char *o_szStrBuff , char *i_szString , int iParamNum = 0 );

public:
//	CINFImage	*	m_pTabButton[CHAT_MENUBOX_SELECT_COUNT][CHAT_MENUBOX_SELECT_STATE];															 // ä�� ��ư����(�Ϲ�, �ӼӸ�, ���, ����, ����)
	CINFImageEx	*	m_pChatBox[CHATBOX_IMAGE_NUMBER];					// ä�� �ڽ� �̹���(�⺻ Ʋ�̹���)
	CINFImageEx	*	m_pScrollLine[CHATBOX_IMAGE_SCROLL_LINE_NUMBER];	// ��ũ�� ���� �̹���
	CINFImageEx	*	m_pChatViewButton[2][BUTTON_BOTH_STATE_NUMBER];		// ä�ùڽ� �����ֱ�� ���߱�
	CINFImageEx	*	m_pChatShowImage;									// ä�ùڽ� Ÿ��Ʋ
	CINFImageEx	*	m_pScrollButton;									// ��ũ�� ��ư �̹���

	// �߰����� 2005.01.21
	CINFImageEx	*	m_pNChatInput;									// ä�� �Է�â
#ifndef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFImageEx	*	m_pNChatCash[CHAT_BUTTON_NUMBER4];					// ä�� ä������ ��ư
#endif
	CINFImageEx	*	m_pNChatCheck;										// ä�� äũ��ũ
	CINFImageEx	*	m_pNChatFAQ[CHAT_BUTTON_NUMBER3];					// ä�� FAQ��ư
	CINFImageEx	*	m_pNChatMenu[CHAT_MENULIST_NUMBER];					// ä�� �޴� ����Ʈ
	CINFImageEx	*	m_pNChatMenuView[CHAT_BUTTON_NUMBER3];				// ä�� �޴� ���̱� ��ư
	CINFImageEx	*	m_pNChatCashView[CHAT_BUTTON_NUMBER4];				// ä�� �޴� ���̱� ��ư
	// 2006-03-16 by ispark
	CINFImageEx	*	m_pNChatInputLanguage[INPUT_LANGUAGE_END];			// �Է� ���

	// 2008-05-08 by dgwoo ä�� �ý��� ���� �ҽ� �߰�.
	CINFGroupImage* m_pBtnChatBG;										// ä�� ��� ���
	CINFImageBtn *	m_pBtnChatMode[9];									// ä�� ��� ��ư. 
	
	DWORD			m_nChatMode;										// ���� Ȱ��ȭ�� ä��.
	CINFImageBtn *	m_pBtnPartyQuick;									// ��� �ٷΰ���.
	CINFImageBtn *	m_pBtnGuildQuick;									// ���� �ٷΰ���.
	BOOL			m_bGuildQuick;
	BOOL			m_bPartyQuick;

	CINFImageBtn *	m_pBtnOption;

	// 2008-05-27 by dgwoo ä�� �ɼ� ��ũ�� 
	BOOL			m_bShowMacro;	
	BOOL			m_bMacroMove;
	INT				m_nMacroOldX;
	INT				m_nMacroOldY;

	INT				m_nMacroX;
	INT				m_nMacroY;
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*	m_pChatMacroBG;										// ��ũ�� ��� �̹���.
#else
	CINFImageEx	*	m_pChatMacroBG;										// ��ũ�� ��� �̹���.
#endif
	CINFImageBtnBoth* m_pBtnChatMacro[CHAT_MACRO_INPUT_COUNT];			// ���� ��ư.
	CINFImageBtn *	m_pBtnMacroOK;										// Ȯ�� ��ư.
	CINFImageBtn *	m_pBtnMacroCancel;									// ��� ��ư.
	sCHATMACROSTRING m_sMacroStr[CHAT_MACRO_INPUT_COUNT];				// ��ũ�� ��Ʈ�� ����.
	char			m_strTempMacro[SIZE_MAX_CHAT_MESSAGE];									// ��ũ�� ��Ʈ��.
	WORD			m_nActMacro;										// ���� ä���� �Է��� â�� ��ȣ.
	BOOL			m_bMacroIng;
	WORD			m_nSendMacroNum;									// ���� ��ũ�� ��ȣ.
	WORD			m_nTransChatCount;									// 2008-08-14 by dgwoo �ŷ�ä�� ��ũ�� ��ȣ�� ����.

	


	// 2005-10-10 by ispark VOIP �޴�
	CINFImageEx	*	m_pNChatVOIPStartMenu[CHAT_BUTTON_NUMBER3];			// ä�� VOIP ���� ��ư
	CINFImageEx	*	m_pNChatVOIPEndMenu[CHAT_BUTTON_NUMBER3];			// ä�� VOIP ���� ��ư

//	int				m_nSizeMaxButtonstate;								// �ִ� ũ�� ��ư ����
//	int				m_nSizeMinButtonstate;								// �ּ� ũ�� ��ư ���� 
	DWORD			m_nChatSizeBtnNormal;								// ä�� ������ ���� ��ư ����.
	int				m_nChatBoxWidth;									// ä�ùڽ� �� 
	int				m_nChatBoxHeight;									// ä�ùڽ� ���� 	
	int				m_nChatBoxYPos;										// ä�ùڽ� Y ��ġ�� 

	//BOOL			m_bLockSoundButton;									// ����ä�� ����
	BOOL			m_bShowChatBox;										// ä�ùڽ� ���̱� ���� 
	BOOL			m_bWindowSizeMax;									// �ִ� ��ư ���� ����
	BOOL			m_bWindowSizeMin;									// �ּ� ��ư ���� ����
	BOOL			m_bLockHandle;										// ä�ùڽ� ���������� ���� �ڵ� 
	BOOL			m_bScrollLock;										// ��ũ�� ������
	POINT			m_ptMouse;

	int				m_nCurrentTabIndex;									// ä�� ���� ���� ��	
	ChatTab_t		m_ArrChatTab;
	// 2008-05-14 by dgwoo EP3ä�� ��ȹ ����.(�� ä��, ���� ���� ���)
	ChatTab_t		m_ArrChatTabMode[CHAT_TAB_SAVE_COUNT];				// �� ä������ ����.
	

	int				m_nCurrentRelScrollPosY;
	int				m_nCurrentRelScrollOldPosY;
													
	CD3DHanFont*	m_pFontInput;										// ����Ʈ ����
	vector<CD3DHanFont*> m_vecFontLine;									// ���������� ����Ʈ ����								
												
	char			m_strInputMessage[SIZE_MAX_CHAT_MESSAGE];
	BOOL			m_bChatMode;
	float			m_fTimeOfShowChat[CHAT_NOT_SHOWBOX_LINE];
//	int				m_nShowTimeChatType;
	// 2008-05-22 by dgwoo �ý��� â �߰�.
	CINFSystemMsgWindow	*		m_pSystemMsgW;
	vectorINFGameMainWisperChatPtr	m_vectorINFiGameMainWisperChatPtr;
	int				m_nClickedWisperChatBoxVectorIndex;
	int				m_nSmallWindowCount;
	CINFImageEx *		m_pImgWisperBtn[2][BUTTON_BOTH_STATE_NUMBER];
	BOOL			m_bShowMiniWisper;
	BOOL			m_bShowMiniWisperUnder;
	int				m_nWisperBoxMinimumStatusInitPositionX;
	int				m_nWisperBoxMinimumStatusInitPositionY;

	// ��Ʈ ����
	char m_strMyMent[1024];

	// ä�� ���� 2005.01.13 ydkim
	BOOL			m_bChatCurrentSelect[CHAT_MENULIST_NUMBER];
	int				m_nSelectChatTab;
	float			m_fTimeGm;
	float			m_fTimeInfluenceLeader;
	
	CD3DHanFont*	m_pFontGM;											// ������ ä�� â	
//	char			m_strInputGm[3][512];
	vector<string>	m_vecInputGM;
	char			m_strInputGm[1024];
	char			m_strInputInfluenceLeader[512];						// 2006-04-21 by ispark, ���� ������ ����
	int				m_nMenuListButtonState;								// �޴� ��ư ����
	int				m_nOtherMenuList[CHAT_OTHER_MENU_ALL];				// �׿� ��Ÿ �޴�
	BOOL			m_bMenuListUse;										// �޴�����Ʈ ��� ����
	BOOL			m_bMenuList;
	BOOL			m_bCheckShopInfo;
	BOOL			m_bCashShopOpen;
	int				m_nOtherCashList;
	int				m_nCashButtonGab;
	BOOL			m_bCashButton;
	BOOL			m_bPKOptionFlag;									// �����ɼ�
	BOOL			m_bTradeFlag;										// �ŷ�
	BOOL			m_bHelpDeskRender;
	BOOL			m_bWindowMessageFlag;								// â���� â�ѱ�
	BOOL			m_bSelectInputLanguage;								// �Է� ��� ����

	int				m_nVoiceMenu;										// ���� �޴�
private:
	// voice chat control center
	//BOOL						m_bShowVoiceChatControl;
	//INT							m_nVCCPosX;
	//INT							m_nVCCPosY;
	//BOOL						m_bVCCMoving;
	//POINT						m_ptVCCOldPos;
//#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
//	CINFGroupImage*				m_pVoiceChatBG;
//#else
//	CINFImageEx*				m_pVoiceChatBG;
//#endif
	//CINFImageBtnBoth*			m_pBothParty;
	//CINFImageBtnBoth*			m_pBothGuild;
	//CINFImageBtnBoth*			m_pBothChat;
	//CINFImageBtnBoth*			m_pBothWhisper;
	//CINFImageEx*				m_pImgSpkVolBar;
	//CINFImageEx			*		m_pImgRadioBtn[2];
	//CINFImageBtn		*		m_pBtnVoiceChatInv;
	//CINFImageBtn		*		m_pBtnVoiceChatOk;
	//CINFImageBtn		*		m_pBtnVoiceChatCan;

	//DWORD						m_nVolum;
	//BOOL						m_bVolumCon;							// �����Ҹ� ������.
	//float						m_fVolumConTimeGap;						// 
	//POINT						m_ptOldVolumPos;						// ������ ���� ��ġ.
	//INT							m_nPosVolum;							// ������ġ(0) + m_nPosVolum      : m_nPosVolum = (m_nVolum / VOICECHAT_SPK_W) * 100
	//DWORD						m_nVoiceInput;							// 0 : Ű �Է¹��, 1 : �������.
	
	int				m_nChinaCnt;
	int				m_nMSPYMode;


	// 2008. 12. 29 by ckPark ä��â ����ȭ
	// stl, itor Ÿ�� ����
	typedef std::list< vector<string> >				CulledStringList;
	typedef std::list< vector<string> >::iterator	CulledStringListItor;

	CD3DHanFont*		m_pFontLine[MAX_GAME_MAIN_CHAT_LINE];	// ä�� �� ���δ� ��Ʈ
	CulledStringList	m_listCulledString;						// �ø��� ��Ʈ���� ������ ����

	void	BuildCulledString( void );							// �ø��� ��Ʈ���� m_listCulledString�� �����

	// end 2008. 12. 29 by ckPark ä��â ����ȭ

	// 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�	
	POINT				m_ptCurselPos;
	POINT				m_ptSelCurselPos;
	char				m_strPreBackupMessage[SIZE_MAX_CHAT_MESSAGE];	// ��Ʈ�� �տ� ����
	char				m_strNextBackupMessage[SIZE_MAX_CHAT_MESSAGE];	// ��Ʈ�� �ڿ� ����
	char				m_strBkInputMessage[SIZE_MAX_CHAT_MESSAGE];		// ���� ����
	BOOL				m_bShiftClick;
	BOOL				m_bCaretCursel;
	BOOL				m_bControlCursel;
	// end 2009-03-18 by bhsohn ä��â Ŀ�� �̵� �ý��� �߰�

};
#endif

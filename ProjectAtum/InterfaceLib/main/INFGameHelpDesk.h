// INFGameMainChat.h: interface for the CINFGameMainChat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEHELPDESK_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_)
#define AFX_INFGAMEHELPDESK_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "RangeTime.h"

#define HELPDESK_ALL_LINE						50			// ���� ����ټ�
#define HELPDESK_ALL_CHAR						256			// ���� �Ѷ��ο�  �����ִ� �ִ� ���ڼ� 
#define HELPDESK_ALL_MISSIONCOUNT				10			// ������ �ִ� �̼��� ���� 						
#define HELPDESK_FONT_HEIGHT					15			// ����Ʈ�� ���� 
#define HELPDESK_FONT_WIDTH_ENGLISH				6			// ���� ���� ��

#define HELPDESK_WINDOW_POSITION_X				110			// ó�����۽� ������ ������ X
#define HELPDESK_WINDOW_POSITION_Y				140			// ó�����۽� ������ ������ Y
#define HELPDESK_WINDOW_WIDTH					300			// ó�����۽� ������ ��
#define HELPDESK_WINDOW_HEIGHT					200			// ó�����۽� ������ ����
#define HELPDESK_WINDOW_TEXT_VIEW_GAP			20			// TextView�� ���� 
#define HELPDESK_WINDOW_IMAGE_VIEW_GAP			20			// TextView�� ���� 

//g_pD3dApp->GetBackBufferDesc().Width
//g_pD3dApp->GetBackBufferDesc().Height


#define HELPDESK_BUTTON_TWINKLE_TIME			1			// ��ư �����̴� Ÿ�̹�
#define HELPDESK_TEXT_TWINKLE_TIME				0.05f		// ��ư �����̴� Ÿ�̹�
#define HELPDESK_REMOVE_BOX_TIME				5.0f		// �ڽ��� ������� Ÿ�̹�
#define HELPDESK_VIEW_TIME						60.f		// �ڽ��� �������� �ð�.

#define HELPDESK_IMAGE_INDEX_0					0			// �̹��� �ε���
#define HELPDESK_IMAGE_INDEX_1					1
#define HELPDESK_IMAGE_INDEX_2					2
#define HELPDESK_IMAGE_INDEX_3					3
#define HELPDESK_IMAGE_INDEX_4					4
#define HELPDESK_IMAGE_INDEX_5					5
#define HELPDESK_IMAGE_INDEX_6					6
#define HELPDESK_IMAGE_INDEX_7					7
#define HELPDESK_IMAGE_INDEX_8					8

#define HELPDESK_KEY_STATE_W					0x00000001
#define HELPDESK_KEY_STATE_A					0x00000002
#define HELPDESK_KEY_STATE_S					0x00000004
#define HELPDESK_KEY_STATE_D					0x00000008

#define HELPDESK_KEY_STATE_CTRL					0x00000001
#define HELPDESK_KEY_STATE_C					0x00000002
#define HELPDESK_KEY_STATE_TAB					0x00000004
#define HELPDESK_KEY_STATE_SPACE				0x00000008
#define HELPDESK_KEY_STATE_Z					0x00000064

#define HELPDESK_MOUSE_STATE_NORMAL				0x00000000
#define HELPDESK_MOUSE_STATE_UP					0x00000004
#define HELPDESK_MOUSE_STATE_DOWN				0x00000008
#define HELPDESK_MOUSE_STATE_LEFT				0x00000001
#define HELPDESK_MOUSE_STATE_RIGHT				0x00000002
#define HELPDESK_MOUSE_STATE_MDOWN				0x00000064

#define HELPDESK_MOUSE_STATE_RBDOWN				0x00000002
#define HELPDESK_MOUSE_STATE_LBDOWN				0x00000001
#define HELPDESK_MOUSE_STATE_WHEEL				0x00000004

#define HELPDESK_QUICK_REPAIR					7002600
#define HELPDESK_QUICK_SHIELD					7002830
#define HELPDESK_QUICK_FUEL						7000790

#define HELPDESK_TALK_SIZE						1024
#define HELPDESK_TEXT_COLOR						RGB(255,255,0) // ���� �۾��� (���)



// 2007-01-29 by bhsohn �ű����� �̼� �������̽�
#define	MAX_STEP_HELPDESK_RENDER				10
#define	STEP_HELPDESK_TIME						0.1f
// end 2007-01-29 by bhsohn �ű����� �̼� �������̽�

class CINFImage;
class CD3DHanFont;
class CINFScrollBarEX;
class CINFImageEx; 

typedef struct HelpLowLevel
{
	CINFScrollBarEX *		pScrollBarEx;				// ��ũ��

	char					msgLowLevelHelp[1024];
	BOOL					bLowLevelHelp;				// ���� ��� �ϴ°�?
	BOOL					bLowLevelHelpRender;		// ���� ������ �����Ѱ�?

	int						nStartX;
	int						nStartY;

	HelpLowLevel()
	{
		pScrollBarEx		= NULL;
		bLowLevelHelp		= FALSE;
		bLowLevelHelpRender = FALSE;

		nStartX				= 0;
		nStartY				= 0;

		memset(msgLowLevelHelp, 0x00, 1024);
	}

	~HelpLowLevel()
	{
		pScrollBarEx	= NULL;
		memset(msgLowLevelHelp, 0x00, 1024);
	}
	
} HELP_LOW_LEVEL;

class CINFGameHelpDesk : public CINFBase
{
public:
	CINFGameHelpDesk(CAtumNode* pParent);
	virtual ~CINFGameHelpDesk();

	int GetHelpDeskState() { return m_nCharacterStatePos; }

   	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	void Tick();
	void Render();
	void DrawHelpWindow(int x, int y, int cx, int cy);		// ���� â
	void ResetHanFontLine();

	void RenderNewCharacter(int x, int y);					// ���ο� ĳ���͸� ��������� �����ִ� ����
	void RenderCityHelpDesk(int x, int y);					// ��Ƽ������ ���� ���̱�	
	void RenderFieldHelpDesk(int x, int y);					// �ʵ�󿡼� ���� ���̱�	
	void RenderMissionHelpDesk(int x, int y);				// �̼� ����� ���� ���̱�
	
	void RenderMousePressHelpDesk(int mouseinput);			// Mouse�Է½� ����

	void ZoomInHelpDesk(int x, int y, int cx, int cy);		// HelpDesk ���¸� ����ũ�� �����
	void RenderKeyPressHelpDesk(int selectkey);				// Key�Է½� ����
	int  SetHelpDeskString(int HWidth, int indexString, char* szString);
	void SetHelpDeskBox(int nPosX, int nPosY, int nWidth, int nHelpIndex, BOOL bTypingText, float fRemoveBoxTime, char* szString=NULL);
	int  SetQuickSlotItem();
	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void StartHelpDesk();

	void TransferOneCharTemp2Talk(void);
	void SetStringMissionItem(char *strItem){strcpy(m_strMissionItem, strItem);m_bRenderMissionItemInfo=TRUE;}
	void SetLowLevelStringbyType(int nType, int nData1 = 0);

	void ReShowHelpDesk();		

protected:
	void InitTextBox();
	void RenderOnInfo();	

public:
	float						m_fHelpTextCheckTime;
	int							m_nHelpDeskWidth;
	int							m_nHelpDeskHeight;
	int							m_nHelpDeskIndex;

	BOOL						m_bRenderMissionItemInfo;
	float						m_fHelpTextMission;
	char						m_strMissionItem[512];

	// ���Ȱ� �̼� ���� (���ù̼��� �Ϸ���� ������ ����Ѵ�.)
	CINFImageEx*					m_pLowLevelHelpflash;					// 2006-08-22 by ispark
	HELP_LOW_LEVEL				m_stLowLevelHelp[HELP_SPECIAL_COUNT];

protected:
	BOOL						m_bRemoveBoxTextView;		// ���� �� �����
	BOOL						m_bButtonTwinkle;			// ��ư �����̱�
	BOOL						m_bButtonView;				// ��ư ���̱�
	BOOL						m_bRemoveBox;				// ���� �ڽ� �����
	BOOL						m_bQuestAlarmFlag;			// ����Ʈ ���� �ڽ� ���̱�

	BOOL						m_bHelpDeskBoxView;			// ǳ�����츻 ���̱� (Render)	
	BOOL						m_bButtonViewNPC;			// NPC ��ư ���̱�	
	BOOL						m_bMouseHelpDesk;			// ���콺 ����
	BOOL						m_bKeyHelpDesk;				// Ű���� ����
	BOOL						m_bKeyHelpDeskTAB;			// Tab ���� 
	BOOL						m_bKeyHelpDeskC;			// C ���� 
	BOOL						m_bKeyHelpDeskCTRL;			// ��Ʈ�� Ű���� 
	BOOL						m_bKeyHelpDeskSPACE;		// SPACE ���� 
	BOOL						m_bKeyHelpDeskZ;			// ZŰ ����
	BOOL						m_bKeyTypingFlag;			// Ÿ���� ���� �ؽ�Ʈ ��
	BOOL						m_bDrawBoxType;				// �ڽ� ��ο�� ���̵��� ���̵� �ƿ�ȿ��
	BOOL						m_bShowCityMouse;			// ���콺 ��巡�� ������ : TRUE --> ���̻� �Ⱥ�����

	CINFImageEx*					m_pHelpBox[9];				// ���� ����
	CINFImageEx*					m_pHelpButton[8];			// ���� ��ư
	CINFImageEx*					m_pHelpButtonNPC[2];		// �̼ǰ� �ݱ� ��ư	
	CINFImageEx*					m_pHelpKeyImage[5];			// ���� Ű���� �̹���
	CINFImageEx*					m_pHelpMouseImage[8];		// ���� ���콺 �̹���
	CINFImageEx*					m_pHelpKeyImage2[8];		// ���� Ű���� �̹���

	int							m_nCharacterStatePos; 	
	int							m_nHelpDeskStartXPos;
	int							m_nHelpDeskStartYPos;
	int							m_nIndexCount;
	int							m_nkeyHelpDesk;				// Ű���� ����
	int							m_nMouseHelpDeskMove;		// ���콺 ����
	int							m_nMouseHelpDeskState;		// ���콺 ����
	int							m_nKeyHelpDeskKEY;			// Kye State ����
	int							m_nCityButtonNumber;		// ���� ��ư ��

	float						m_fTwinkleTime;												// ��ư �����̴� Ÿ�̹�
	float						m_fRemoveBoxTime;											// �ڽ��� �������� Ÿ�̹�
	float						m_fTextTwinkleTime;
	float						m_fTextUntilTime;
	
	vector<CD3DHanFont*>		m_vecHelpDeskFontLine;
	vector<string>				m_vecHelpDeskTalk;
	vector<string>				m_vecHelpDeskString;

	vector<string>				m_vecStringTemp;			// �ѱ��ھ� �������ϱ����� �ӽ� ����
	char						m_szTransferString[512];
	int							m_nTransferVectorIndex;
	char						*m_szTransferPointer;
	char*						m_strPrev;	

	CRangeTime					m_tBlinkTime;
	CRangeTime					m_tBlinkTimeStart;	
	int							m_nBlinkButtonIndex;	// �����̰� �ִ� ��ư �ε���(�ϴ�)
	BOOL						m_bShowMiniMapIndex;

	BOOL						m_bKeyDown[4];
	int							m_nHelpCloseCount;
	BOOL						m_bShowCityEnd;
	// 2005-05-17 by ydkim	
	CD3DHanFont*				m_pFontMissionItem;

	// ���Ȱ� �̼� ���� (���ù̼��� �Ϸ� ��ų������.)
	CINFImageEx*					m_pLowLevelHelpBox;			// 2006-08-22 by ispark
	CD3DHanFont*				m_pLowLevelFont;

	int							m_nShowStep;									// ������ ����Ҷ� �ε巴�� �ϱ����� ����.
	float						m_fHelpPopupTime;								// ������ ���̰� �Ҷ��ð� üũ.
};

#endif // !defined(AFX_INFGAMEMAINCHAT_H__CEC28390_FEA8_4361_9051_6543A4E37D9D__INCLUDED_)

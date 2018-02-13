// INFImageAnimation.h: interface for the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINFAQI_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFGAMEMAINFAQI_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define  INF_FAQ_QUESTION_SIZE	1024
#define  INF_FAQ_ANSWER_SIZE	1024
#define  INF_FAQ_SEARCH_ANSWER_RENDER	8	// �˻� �� ã�� �亯 ������ ��
#define  INF_FAQ_SEARCH_QUESTION_RENDER	8	// �˻� �� ã�� ���� ������ ��
#define  INF_FAQ_BUTTON_IMG				4	// ��ư �̹��� ��

class CINFImage;
class DataHeader;
class CGameData;
class CD3DHanFont;
class CINFScrollBar;
class CINFImageEx;
class CINFGroupImage;
class CINFImageBtn;

typedef struct
{
	char sFAQQuestion[INF_FAQ_QUESTION_SIZE];
	char sFAQAnswer[INF_FAQ_ANSWER_SIZE];

}SFAQINF;

class CINFGameMainFAQ : public CINFBase
{

public:
	CINFGameMainFAQ();
	virtual ~CINFGameMainFAQ();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	vector<SFAQINF>				m_vecFAQInf;		// ��ü FAQ ����
	vector<SFAQINF>				m_vecFAQAnswer;		// �˻��� FAQ ����
	vector<string>				m_vecAnswer;
	vector<string>				m_vecQuestion;

	BOOL LoadFAQFile(char *chrTxtName);
	void SearchQuestionTxt(char *strWord);
	void SetNPCTalkInfo( char* strNPCTalk, int nType=0);
	void SetInit();
	void InitBaseString();
private:
	void ToLower(char* pSrc, char* pDst);	// 2008-08-22 by bhsohn FAQ ���� ��/�ҹ��� �������� ó��

	BYTE ToLowerDeu(BYTE i_ch);				// 2008-10-23 by bhsohn ���Ͼ� �빮�� �ҹ��ڷ� ����

public:
	char m_strQuestionWord[INF_FAQ_QUESTION_SIZE];
	char m_strInputMessage[INF_FAQ_QUESTION_SIZE];
	BOOL m_bChatMode;								// �˻� �ؽ�Ʈ �Է� ����, �Ұ���

protected:
	CD3DHanFont			*   m_pFontInput;
	CD3DHanFont			*   m_pFontSearchAnswer[INF_FAQ_SEARCH_ANSWER_RENDER];
	CD3DHanFont			*   m_pFontSearchQusetion[INF_FAQ_SEARCH_QUESTION_RENDER];
	CD3DHanFont			*   m_pFontQuestion[2];

#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*			m_pImgBackg;
	//CINFImage*				m_pImgBackg;						// ��׶��� �̹���
	CINFImageEx*			m_pImgOper[2];						// ���۷����� �̹���.
	INT						m_nOperatorInfl;					// ���۷����� ����.

	CINFImageBtn*			m_pcloseBtn;						// �ݱ�
	//CINFImageEx*			m_pImgSearch;						// ��ġ �̹���.
// 	CINFImageEx*			m_pImgResult;						// ��� ���.
 	CINFImageEx*			m_pImgSelect;						// ���� ���� �̹���
// 	CINFImageEx*			m_pImgSelectName;					// Ÿ��Ʋ ���� �̹���

	CINFImageEx*			m_pImgButton[INF_FAQ_BUTTON_IMG];	// ��ư �̹���					  
#else
	CINFImageEx*				m_pImgBackg;						// ��׶��� �̹���
	CINFImageEx*				m_pImgOper[2];						// ���۷����� �̹���.
	INT						m_nOperatorInfl;					// ���۷����� ����.

	CINFImageEx*			m_pImgSearch;						// ��ġ �̹���.
	CINFImageEx*			m_pImgResult;						// ��� ���.
	CINFImageEx*			m_pImgSelect;						// ���� ���� �̹���
	CINFImageEx*			m_pImgSelectName;					// Ÿ��Ʋ ���� �̹���

	CINFImageEx*			m_pImgButton[INF_FAQ_BUTTON_IMG];	// ��ư �̹���
#endif

	CINFScrollBar*			m_pScroll;
	CINFScrollBar*			m_pScrollUnder;

	int						m_nStartFAQPositionX;
	int						m_nStartFAQPositionY;
	int						m_nSelectindexQuestion;
	int						m_nMouseButtonState;

	int						m_nFAQBoxStartXOld;
	int						m_nFAQBoxStartYOld;
	POINT					m_pointBeforeMousePoints;			// ���콺 ������ǥ

	BOOL					m_bMoveWindow;
	BOOL					m_bToolTibQuestion;
	float					m_fQuestionScrollText;
};

#endif

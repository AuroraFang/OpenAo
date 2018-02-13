// INFMission.h: interface for the CINFMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMISSION_H__BEAAD194_E6F7_4886_B21E_031019F82E46__INCLUDED_)
#define AFX_INFMISSION_H__BEAAD194_E6F7_4886_B21E_031019F82E46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define MISSION_BUTTON_OK		0
#define MISSION_BUTTON_NEXT		1
#define MISSION_BUTTON_PREV		2
#define MISSION_LINE_NUMBER		11
#define MISSION_LINE_LENGTH		40

class CAtumNode;
class CQuest;
class CINFPilotFace;
class CINFImage;
class CD3DHanFont;
class CINFImageEx;
class CINFMission : public CINFBase 
{
public:
	CINFMission(CAtumNode* pParent);
	virtual ~CINFMission();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	void Render();
	virtual void Tick();

	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnButtonClicked(int button);//	MISSION_BUTTON_XXX
//	void SetQuest( Quest* pQuest, QuestNPCTalk* pNPC, MEX_QUEST_INFO* pCharacter, DWORD nHelpImage=0 );
	void SetQuest( CQuest* pQuest, MEX_QUEST_INFO* pCharacter, DWORD nHelpImage=0 );
	BOOL IsImmediateQuest(CQuest* pQuest);

protected:
	// �������̽� �̹��� ���� ����
	CINFPilotFace*	m_pQuestNPCFace;
	CINFPilotFace*	m_pQuestHelpImage;
	CINFPilotFace*	m_pQuestHelpTitleImage;
	CINFImageEx*		m_pTitle;
	CINFImageEx*		m_pBack;
	CINFImageEx*		m_pOk[4];
	CINFImageEx*		m_pPrev[4];
	CINFImageEx*		m_pNext[4];
	CINFImageEx*		m_pNPC;
	CINFImageEx*		m_pHelp;
	CINFImageEx*		m_pMissionTitle;
	// ������ ������ ����
	BOOL			m_bWindowMoveLock;
	POINT			m_ptMouse;
	int				m_nX;
	int				m_nY;
	int				m_nButtonState[3];	//	MISSION_BUTTON_XXX

	// ����Ʈ ����(�ؽ�Ʈ) ����
	int				m_nLineNumber;
//	CD3DHanFont*	m_pFontQuestName;
	CD3DHanFont*	m_pFontDesc[MISSION_LINE_NUMBER];
	CQuest*			m_pQuestInfo;
//	QuestNPCTalk*	m_pQuestNPC;
	MEX_QUEST_INFO*	m_pCharacterQuest;
	char			m_strNPCTalk[MISSION_LINE_NUMBER][MISSION_LINE_LENGTH];// 11����x40����
public:
	BOOL			m_bShow;	// ȭ�鿡 �����ְ� �ִ� ����(����â�� ���ÿ� �ΰ� ���� �ʰ� �ϱ� ���ؼ�)

};

#endif // !defined(AFX_INFMISSION_H__BEAAD194_E6F7_4886_B21E_031019F82E46__INCLUDED_)

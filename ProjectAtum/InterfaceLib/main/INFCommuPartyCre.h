// INFCommuPartyCre.h: interface for the CINFCommuPartyCre class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMMUPARTYCRE_H__63861C02_6AC5_4D66_8FC1_21BBD65AA0BE__INCLUDED_)
#define AFX_INFCOMMUPARTYCRE_H__63861C02_6AC5_4D66_8FC1_21BBD65AA0BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

// ������ư ����
#define		RADIO_JOINT_COMMAND				0	// ����� �ʴ�
#define		RADIO_JOINT_FREE				1	// ���� ����
#define		MAX_RADIO_JOINT					2

// ������ư ����
#define		RADIO_AUDIO_CHAT				0	// ���� ä�ù��
#define		RADIO_AUDIO_KEY					1	// Ű �Է¹��
#define		RADIO_AUDIO_FREE				2	// �������
#define		MAX_RADIO_AUDIO					3

// ����ġ �й� �޺� �ڽ�
#define		COMBO_DIVISION_EXP			0	// ����ġ
#define		COMBO_DIVISION_ITEM			1	// ������ �й�
#define		MAX_COMBO_DIVISION			2

// ��� ���� ����
//���� ������ �̹��� ���� 
#define COMBO_FLIGHT_FORM_2_COLUMN			0	// �̷� ����, �̷� ���� ������� �� �ٷ� ������ �� ����̴�
#define COMBO_FLIGHT_FORM_2_LINE			1	// �̷� Ⱦ��, �̷� Ⱦ�� ������� �� �ٷ� ������ �� ����̴�
#define COMBO_FLIGHT_FORM_TRIANGLE			2	// �ﰢ ���, �ﰢ�� ������� ��ܺ��� 1, 2, 3���� ������ ��ġ�Ѵ�
#define COMBO_FLIGHT_FORM_INVERTED_TRIANGLE	3	// ���ﰢ ���, �� �ﰢ�� ������� ��ܺ��� 3, 2, 1���� ������ ��ġ�Ѵ�
#define COMBO_FLIGHT_FORM_BELL				4	// �� ����, �� ������� ��ܺ��� 1, 3, 2���� ������ ��ġ�Ѵ�
#define COMBO_FLIGHT_FORM_INVERTED_BELL		5	// ���� ����, ���� ������� ��ܺ��� 2, 3, 1���� ������ ��ġ�Ѵ�
#define COMBO_FLIGHT_FORM_X					6 // X�� ����
#define COMBO_FLIGHT_FORM_STAR				7	// �� ����
#define MAX_COMBO_FLIGHT_FORM				8

class CINFImageRadioBtn;
class CINFListBox;
class CINFSecuEditBox;
class CINFNumEditBox;
class CINFEditBox;
// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
class CINFSingleCurselEditBox;
// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
class CINFGroupImage;
class CINFImageEx;

class CINFCommuPartyCre  : public CINFBase
{
public:
	CINFCommuPartyCre(CAtumNode* pParent);
	virtual ~CINFCommuPartyCre();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Tick();
	void	Render();

	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ShowWindow(BOOL bShow);
	BOOL IsShowWindow();
	SPARTY_INFO* GetSPartyCreateInfo();
	// 2008-12-09 by dgwoo �̼Ǹ�����.
	void MissionMasterParty(char * i_pString);
	BOOL IsMissionMaster()		{			return m_bMissionMaster;	}
	void SetMissionMaster(BOOL i_bMissionMaster);
	void SetMissionMasterUID(UID64_t i_nUID);
	UID64_t GetMissionMasterUID() {return m_nMissionMasterUID;	}
	void SetAutoCreateParty(SPARTY_INFO i_PartyInfo);		/// 2012-04-12 by jhseol, �Ʒ��� �߰����� - �ڵ���Ƽ : �ڵ����� ��Ƽ�����.
private:
	BOOL IsMouseCaps(POINT ptPos);
	void UpdateUIPos();

	void OnClickJoint(int nId);
	void OnClickAudioOp(int nId);
	
	void UpdateRadioBtn();	
	void AllEditBoxDisable();

	void LoadSPartyInfo(SPARTY_INFO *pSPartyInfo);
	void InitCotrol();
	void SetVoiceChatVolum(DWORD i_nVolum);

	// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
	int WndProcEdit(UINT uMsg, WPARAM wParam, LPARAM lParam);
	// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
private:
	//CINFImage*	m_pBkImage;		
#ifdef C_EPSODE4_UI_CHANGE_JSKIM					        // 2011. 10. 10 by jskim UI�ý��� ����
	CINFGroupImage*	m_pBkImage;																	  
#else								
    CINFImageEx*	m_pBkImage;
#endif

	BOOL		m_bMove;
	BOOL		m_bShow;

	POINT					m_ptCommOpBk;
	POINT					m_ptCommOpMouse;	

	CINFImageBtn*			m_pCloseBtn;			// �ݱ�
	CINFImageBtn*			m_pOkBtn;				// ����

	// �������
	CINFImageRadioBtn*		m_pPartyJoint[MAX_RADIO_JOINT];
	int						m_nJointIdx;

	// ���� �ɼ�
	CINFImageRadioBtn*		m_pAudioOp[MAX_RADIO_AUDIO];
	int						m_nAduioIdx;

	// ����Ŀ ���� ����.
	//CINFImageEx*				m_pImgSpkVolBar;

	//DWORD						m_nVolum;
	//BOOL						m_bVolumCon;							// �����Ҹ� ������.
	//float						m_fVolumConTimeGap;						// 
	//POINT						m_ptOldVolumPos;						// ������ ���� ��ġ.
	//INT							m_nPosVolum;							// ������ġ(0) + m_nPosVolum      : m_nPosVolum = (m_nVolum / VOICECHAT_SPK_W) * 100
	// ����Ŀ ���� ����.

	CINFListBox*			m_pComboDivision[MAX_COMBO_DIVISION];	// �й� ���

	CINFListBox*			m_pComboForm;							// ��� ����
	CINFImageEx*			m_pFormatImage[MAX_COMBO_FLIGHT_FORM];		

	CD3DHanFont*			m_pFontText;	
	
	CINFSecuEditBox*		m_pINFSecuEditBox;
	CINFNumEditBox*			m_pNumMinEditBox;	// �ּ� ����
	CINFNumEditBox*			m_pNumMaxEditBox;	// �ִ� ����
	// 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�
	//CINFEditBox*			m_pEditPartyName;
	CINFSingleCurselEditBox*			m_pEditPartyName;
	// end 2009-04-23 by bhsohn ����Ʈâ Ŀ�� �̵�

	SPARTY_INFO				m_stSPartyCreate;

// 2008-12-09 by dgwoo �̼Ǹ�����.
	BOOL					m_bMissionMaster;
	UID64_t					m_nMissionMasterUID;
};

#endif // !defined(AFX_INFCOMMUPARTYCRE_H__63861C02_6AC5_4D66_8FC1_21BBD65AA0BE__INCLUDED_)

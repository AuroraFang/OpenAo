// INFMotherShipManager.h: interface for the CINFMotherShipManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFMOTHERSHIPMANAGER_H__6585EF9D_3FD5_4DD8_8BE6_E463657B1AFE__INCLUDED_)
#define AFX_INFMOTHERSHIPMANAGER_H__6585EF9D_3FD5_4DD8_8BE6_E463657B1AFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "INFBase.h"
#include "INFImageBtn.h"
#include "INFImageBtnBoth.h"
#include "INFImportantInfo.h"
#include "INFNumEditBox.h"
#include "INFGroupImage.h"


// 2009. 01. 12 by ckPark ���� ���� �ý���
#include "INFWarDeclare.h"
// end 2009. 01. 12 by ckPark ���� ���� �ý���




#define SLIDE_TIME						0.25f

struct stSTRONGHOLD_INFO{
	DWORD			nIndex;
	DWORD			MapIndex;
};

// 2007-02-06 by bhsohn ���� ���� ���� üũ
#define MOTHERSHIP_INFO_BALON			9		// ����â �׵θ�
// ���� �ִ� �۾� ���� ��
#define	MOTHERSHIP_MAXLINE				6
// ���� ���� �� ���´� ǥ�� �� �ִ� ���μ� 
#define	MOTHERSHIP_INFLUENCE_LINE		3


struct MOTHERSHIP_INFO
{ //����  ����
	char szInfluence[MAX_PATH];
	char szReMainTime[MAX_PATH];
	char szArea[MAX_PATH];
	BYTE  byBelligerence;
	FLOAT fMotherShipRemainTime;
	
	INT	 nContributionPoint;					// ��ȯ��, ��������Ʈ // 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	ATUM_DATE_TIME  SummonMonsterTime;			// ��ȯ�ð� // 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	INT SummonMonsterUnitkind;					// �������� // 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	
	
	MOTHERSHIP_INFO()
	{
		memset(szInfluence, 0x00, MAX_PATH);
		memset(szReMainTime, 0x00, MAX_PATH);
		memset(szArea, 0x00, MAX_PATH);
		
		byBelligerence = 0;

		memset(&SummonMonsterTime, 0x00, sizeof(ATUM_DATE_TIME));		// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
		nContributionPoint = 0;											// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
		SummonMonsterUnitkind = 0;										// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	};
};

struct PREADD_MOTHERSHIP_INFO
{ // ���� ����
	BYTE			byBelligerence;
	ATUM_DATE_TIME  SummonMonsterTime;	
	INT	 nContributionPoint;					// ��ȯ��, ��������Ʈ // 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	INT SummonMonsterUnitkind;					// �������� // 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
};
// end 2007-02-06 by bhsohn ���� ���� ���� üũ


typedef vector<CINFImportantInfo*>					vecINFImportantInfo;
typedef vector<CINFImportantInfo*>::iterator		vecINFImportantInfoIter;

class CINFImage;
class CD3DHanFont;
class CINFMotherShipManager : public CINFBase  
{
public:
	CINFMotherShipManager();
	virtual ~CINFMotherShipManager();
	CINFMotherShipManager(CGameData * pData);

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();
	void			TickShowIng();
	virtual int		WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int				WndProcOption(UINT uMsg, WPARAM wParam, LPARAM lParam);

	USHORT			SetMotherShipOption(USHORT i_Option);
	BOOL			SetButtonPos0(UINT i_nPosX,UINT i_nPosY);
	BOOL			SetButtonPos1(UINT i_nPosX,UINT i_nPosY);
	void			ResetAdventBCU();
	void			ResetAdventANI();

	void			AdventMotherShipBCU(SMSWARINFO_DISPLAY * pInfo);
	void			AdventMotherShipANI(SMSWARINFO_DISPLAY * pInfo);
	void			DestroyMotherShipANI();
	void			DestroyMotherShipBCU();

	void			ShowWindowOption();
	void			HideWindowOption();

	void			RenderOption();
	UINT			OptionWindowEvent(POINT &i_pt);
	
	void			SummonMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime, 
												 INT nContributionPoint, 
												 INT SummonMonsterUnitkind); // 2007-02-06 by bhsohn ���� ���� ���� üũ
	void			DestroyMotherShipInfo(BYTE byBelligerence);// 2007-02-06 by bhsohn ���� ���� ���� üũ
	int				GetSummonMotherShipCnt();
	BOOL			GetMotherShipInfl(UINT i_nMotherShipInf);
	BOOL			GetMotherShipOptionAct() { return m_bOptionAct;}

	// 2008-03-27 by dgwoo ������ �������� ������ ����.
	BOOL			SetWarPointMapInfo(SMSWARINFO_DISPLAY * pWarInfo);
	void			SetMSWarInfoDisplayOption(MSG_FC_INFO_MSWARINFO_DISPLAY_OPTION_OK* pMsg);
	BOOL			SetMotherShipSearchTeleportTime(MSG_FC_EVENT_CLICK_TELEPORT_OK *pMsg);
	// 2008-04-22 by bhsohn ��/������ �������� ������ ǥ�� �ϰ� ����
	void			GetMotherShipInfo(int nIdx, 
										BYTE *o_byBelligerence, 
										ATUM_DATE_TIME  *o_pSummonMonsterTime, 
										int *o_nContributionPoint,
										int *o_SummonMonsterUnitkind);

	// 2009. 01. 12 by ckPark ���� ���� �ý���
	CINFWarDeclare*	GetWarDeclareWindow(void) { return m_pWarDeclareWindow; }
	// end 2009. 01. 12 by ckPark ���� ���� �ý���
	

private:
	void			RenderMothershipInfo();						// 2007-02-06 by bhsohn ���� ���� ���� üũ
	void			RenderMothershipInfoBalon(int x, int y, int cx, int cy); // 2007-02-06 by bhsohn ���� ���� ���� üũ	
	void			TickMotherShipInfo();						// 2007-02-06 by bhsohn ���� ���� ���� üũ
	void			AddPreMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime, INT nContributionPoint, INT SummonMonsterUnitkind); // 2007-02-06 by bhsohn ���� ���� ���� üũ
	void			UpdateMotherShipInfo(BYTE byBelligerence, ATUM_DATE_TIME  *pSummonMonsterTime, 
												INT	 nContributionPoint, INT SummonMonsterUnitkind); // 2007-02-06 by bhsohn ���� ���� ���� üũ
	SIZE			GetMotherShipInfoFontWidth();					// 2007-02-15 by bhsohn �� �ڵ� �Ҹ�� �̺�Ʈ ����.
	SIZE			GetStringSize(char* pTxt);						// 2007-02-15 by bhsohn �� �ڵ� �Ҹ�� �̺�Ʈ ����.
	void			SendMotherShipOption();
	
	//--------------------------------------------------------------------------//
	//				BCU : 0,                ANI : 1								//
	//--------------------------------------------------------------------------//
	
private:

	CINFImageBtnBoth*		m_pImgMotherShipBCUButton;
	CINFImageBtnBoth*		m_pImgMotherShipANIButton;

	

	DWORD			m_nButtonState0;
	DWORD			m_nButtonState1;
	
	USHORT			m_nOption;						// �ɼ�.
	USHORT			m_nTempOption;
	BOOL			m_bFirstOptionSetting;

	vecINFImportantInfo		m_vecImportBCU;
	vecINFImportantInfo		m_vecImportANI;

	DWORD			m_nShowNum0;
	DWORD			m_nShowNum1;

	float			m_fShowTime;

	BOOL			m_bShow0;
	BOOL			m_bShow1;

	DWORD			m_nButtonPosX0;
	DWORD			m_nButtonPosY0;
	DWORD			m_nButtonPosX1;
	DWORD			m_nButtonPosY1;

	//--------------------------------------------------------------------------//
	//							�ɼ� â.
	//--------------------------------------------------------------------------//

	BOOL				m_bOptionAct;						// �ɼ�â Ȱ��ȭ����.
#ifdef C_EPSODE4_UI_CHANGE_JSKIM
	CINFGroupImage*		m_pImgOptionBG;						// �ɼ� ���.
#else
	CINFImageEx*		m_pImgOptionBG;						// �ɼ� ���.	
#endif
	
	CINFImageEx*		m_pImgCheckB[RADIOBUTTON_STATE_NUMBER];  // ����� ��ư.
	CINFImageBtn*		m_pImgOKB;							// ����.
	CINFImageBtn*		m_pImgCancelB;						// ���.
	
	float				m_fTimeOptionChange;				// �ɼ��� ������ �ð� üũ.
	INT					m_nOptionPosX;
	INT					m_nOptionPosY;
	POINT				m_ptOldPoint;
	BOOL				m_bLButtonClick;					// ������ â ���� ����.
	//--------------------------------------------------------------------------//



	// 2009. 01. 12 by ckPark ���� ���� �ý���

	CINFWarDeclare*		m_pWarDeclareWindow;

	// end 2009. 01. 12 by ckPark ���� ���� �ý���



	CINFImageEx	*	m_pBalonChat[MOTHERSHIP_INFO_BALON];
	// ���� ���� ����
	CD3DHanFont*				m_pFontMotherShipInfo[MOTHERSHIP_MAXLINE];
	MOTHERSHIP_INFO				m_struMotherShipInfo[MOTHERSHIP_MAX_INFLUENCE];
	PREADD_MOTHERSHIP_INFO		m_struPreAddMotherShip[MOTHERSHIP_MAX_INFLUENCE];
	int							m_nPreAddMothershipCnt;
	// ���� �� �ִ� ��
	BOOL			m_bSummonMotherShipInfo[MOTHERSHIP_MAX_INFLUENCE];		// ���� ��ȯ���ִ� ��		
	int				m_nSummonMotherShipCnt;		// ���� ��Ÿ�� ���� ��	
	
	// 2007-02-15 by bhsohn �� �ڵ� �Ҹ�� �̺�Ʈ ����.
	SIZE			m_sizeMotherShipInfo;
	
	
};

#endif // !defined(AFX_INFMOTHERSHIPMANAGER_H__6585EF9D_3FD5_4DD8_8BE6_E463657B1AFE__INCLUDED_)

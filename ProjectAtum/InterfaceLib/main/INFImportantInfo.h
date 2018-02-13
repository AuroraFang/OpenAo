// INFImportantInfo.h: interface for the CINFImportantInfo class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_INFIMPORTANTINFO_H__E307CF26_C7FB_48A6_9D5C_FFA20FDF1D0D__INCLUDED_)
#define AFX_INFIMPORTANTINFO_H__E307CF26_C7FB_48A6_9D5C_FFA20FDF1D0D__INCLUDED_



///////////////////////////////////////////////////////////////////////////////
/// \class		class CINFImportantInfo : public CINFBase  
/// \brief		���������� �������ִ� Ŭ����.(��,������ ����)
///
/// \author		dgwoo
/// \version	
/// \date		2008-03-24 ~ 2008-03-24
/// \warning	
///////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

enum {
	MAP_STATE_OFF = 0,
	MAP_STATE_NORMAL,
	MAP_STATE_DESTROY,
	MAP_STATE_NUMBER
};
enum {
	TELEPORT_STATE_OFF = 0,				// ���� ����.
	TELEPORT_STATE_ING,					// Ȱ��ȭ ������.
	TELEPORT_STATE_ACTIVE				// Ȱ��ȭ.
};
enum {
	HP_STATE_OFF = 0,					// ���� ����.
	HP_STATE_ON,						// ���� ����.
	HP_STATE_BG							// �ǹ� ��� �̹����� �ִ� ����.
};

#define		IMPORTANT_ATT_WARNING_GAP		0.5f
#define		IMPORTANT_ATT_WARNING_TIME		10.f
#define		TELEPORT_ACT_ANIMATION          11
#define		TELEPORT_ACT_COOLTIME			18
#define		TELEPORT_ANI_TIME_GAP			0.15f

class CINFImage;
class CINFImageEx;
class CINFImportantInfo : public CINFBase  
{
public:
	CINFImportantInfo();
	virtual ~CINFImportantInfo();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	DWORD			SetState(DWORD i_nState,float i_HPRate = MS_WAR_HP_RATE);
	void			SetImportantPos(int i_X,int i_Y);
	BOOL			SetMapNum(DWORD i_nMapNum);
	void			Init();
	USHORT			SetOption(USHORT i_nOption);
	void			AttWarning();
	void			InitAllState();
	void			SetActiveTime(ATUM_DATE_TIME i_StartTime,ATUM_DATE_TIME i_EndTime);

	USHORT			m_nOption;					// �ɼ�
	DWORD			m_nState;					// 
	DWORD			m_nMapState;
	DWORD			m_nTelState;
	DWORD			m_nHPState;
	float			m_fHP;						// ���� �ڽ��� �Ƿ�.
	int				m_nStartX;					// ���� ��ǥ.
	int				m_nStartY;

	INT				m_bMapInfluence;					// ���� ����.

	float			m_fWarningTime;				// ���޽��� �ð� ����.
	float			m_fWarningGap;				// 
	BOOL			m_bOldWarning;				// ������ ȿ�� ����.

	DWORD			m_nMapNum;
	CINFImageEx*		m_pImgMapState[MAP_STATE_NUMBER];			// ���� ���¿� ���� �̹���.
	CINFImageEx*		m_pImgHP;					// �� �� �̹���.
	CINFImageEx*		m_pImgHPBg;					// �� ��� �̹���.
	CINFImageEx*		m_pImgReadyTel;				// �غ������ �ڷ���Ʈ �̹���.
	CINFImageEx*		m_pImgSuccTel;				// Ȱ��ȭ �Ϸ� �ڷ���Ʈ �̹���.
	CINFImageEx*		m_pImgAttWarning;			// ��� �̹���.
	
	CINFImageEx*		m_pActAni[TELEPORT_ACT_ANIMATION];			// Ȱ��ȭ ������ �ִϸ��̼�.
	UINT			m_nAniNum;									// �ִϸ��̼� �̹��� ��ȣ.
	float			m_fAniTime;									// �ð� ����.
	BOOL			m_bActAni;									// Ȱ��ȭ �������� �ִϸ��̼�.

	CINFImageEx*		m_pActCoolTime[TELEPORT_ACT_COOLTIME];		// Ȱ��ȭ ����ȿ���� ĭ�� ���µ��� �̹���.
	UINT			m_nCoolTimeNum;								// ��Ÿ�� �ѹ�.
	ATUM_DATE_TIME	m_DTStartTime;								// �׽�Ʈ ����.
	float			m_fCooltime;								// ��Ÿ�� �ð� üũ.
	BOOL			m_bActCoolTime;								// Ȱ��ȭ �������� ��Ÿ��.
	float			m_fTeleportBuldingTime;						// �ڷ���Ʈ�� ����ð�.


};

#endif // !defined(AFX_INFIMPORTANTINFO_H__E307CF26_C7FB_48A6_9D5C_FFA20FDF1D0D__INCLUDED_)

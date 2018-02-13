// INFImageAnimation.h: interface for the INFImageAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOTHERCHARINFO_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_INFOTHERCHARINFO_H__7B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define OTHERINFO_INFOMATION_NUMBER		6

class CINFImage;
class DataHeader;
class CGameData;
class CD3DHanFont;
class CINFPilotFace;
class CINFImageEx;											  // 2011. 10. 10 by jskim UI�ý��� ����
class CINFOtherCharInfo : public CINFBase
{

public:
	CINFOtherCharInfo(CAtumNode* pParent);
	virtual ~CINFOtherCharInfo();
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void GetOtherCharInfo(char *sName,BOOL bRenderLevel = FALSE);

protected:
	CD3DHanFont	*	m_pFontInfo[OTHERINFO_INFOMATION_NUMBER];
	CINFImageEx	*	m_pInfoShowImage;
	CINFImageEx	*	m_pInfoGuildCase;

	char			m_strUserName[128];				// ���� ĳ���� �̸�
	char			m_strUserGear[64];				// ���� ��ü ����
	char			m_strGuild[256];				// ���� ��� ����
	char			m_strClass[128];				// ���� ���
	char			m_strFame[16];					// ���� ��
	char			m_strLevel[16];					// ���� ����

	int				m_nX;
	int				m_nY;
	int				m_nGuildUniNum;
	int				m_nBoxStartXOld;
	int				m_nBoxStartYOld;

	BOOL			m_nMoveWindows;

	MSG_FC_CHARACTER_CREATE m_createUnit;
	CINFPilotFace*	m_pPilotFace;
	POINT			m_pointBeforeMousePoint;

	// 2005-04-22 by jschoi
	BOOL			m_bRenderLevel;					// ���� ǥ��

public:
	BOOL			m_bRenderInfo;
};

#endif

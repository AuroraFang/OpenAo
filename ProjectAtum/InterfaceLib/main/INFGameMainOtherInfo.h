// INFGameMainOtherInfo.h: interface for the CINFGameMainOtherInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINOTHERINFO_H__D292DD48_BD40_4CB9_8B4C_107A96F3660F__INCLUDED_)
#define AFX_INFGAMEMAINOTHERINFO_H__D292DD48_BD40_4CB9_8B4C_107A96F3660F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "INFBase.h"

/////////////////////////////////////////////////////////////////////////////////
///	\struct		CINFGameMainOtherInfo
/// \brief		Ÿ�ٿ� ���� ���� ����(���,�̸�,HP)�� �����ش�
///				
/////////////////////////////////////////////////////////////////////////////////

class CINFImage;
class CD3DHanFont;
class CINFImageEx;
class CINFGameMainOtherInfo : public CINFBase
{
public:
	CINFGameMainOtherInfo(CAtumNode* pParent);
	virtual ~CINFGameMainOtherInfo();


	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	// main function
	void Tick( float fElapedTime = 0.0f);
	void Render( );

	// sub function
	void SetLockOnTarget( int iLockOnTarget );
	void OffLockOnTarget( );
	BOOL GetLockOnTarget( );
	void SetHpRate( float fHpRate );
	void SetOtherInfoString( char* strGuild, char* strName ); // �� ���, �̸�,
	void SetMonsterInfoString(char* strName);				 // ������ �̸�,
	void SetMonsterHPString(char* strHP);
	void DrawText( );										// ���̳� ������ ���, �̸�

public:
	// init()
	bool m_bRestored;

//	CINFImage	*m_pImgBack;
//	CINFImage	*m_pImgHpLeft;
//	CINFImage	*m_pImgHpCenter;
//	CINFImage	*m_pImgHpRight;
	
	// 2004-12-09 by jschoi
	CINFImageEx	*m_pImgTargetInfoBack;		// Ÿ���� ������ ���� ���
	CINFImageEx	*m_pImgTargetInfoBar;		// Ÿ���� ������ ���� ���

	CD3DHanFont*	m_pFontOtherInfo[2];
//	char m_strOtherGuiid[64];
	char m_strOtherName[64];
	char m_strMonsterName[64];
	char m_strMonsterHP[64];

	float m_fHpRate;						// ���� Hp Rate
	int   m_iLockOnTarget;					// ���̳� ���͸� Ÿ������ ��� ������ �ִ���

	float m_fImgLeftRate;					// m_pImgHpLeft �� Rate
	float m_fImgCenterRate;					// m_pImgHpCenter �� Rate
	float m_fImgRightRate;					// m_pImgHpRight �� Rate
	
	float m_fImgToLeftRate;					// m_pImgHpLeft ������ Rate
	float m_fImgToCenterRate;				// m_pImgHpCenter ������ Rate
	float m_fImgToRightRate;				// m_pImgHpRight ������ Rate
};

#endif // !defined(AFX_INFGAMEMAINOTHERINFO_H__D292DD48_BD40_4CB9_8B4C_107A96F3660F__INCLUDED_)

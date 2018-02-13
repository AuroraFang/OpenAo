// INFUnitNameInfo.h: interface for the CINFUnitNameInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFUNITNAMEINFO_H__580A7F50_EF83_4ADC_B707_DE01602B3EE7__INCLUDED_)
#define AFX_INFUNITNAMEINFO_H__580A7F50_EF83_4ADC_B707_DE01602B3EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define NAME_INFO_BALON_CHAT		9

class CShuttleChild;
class CChatMoveData;
class CINFImage;
class CD3DHanFont;
class CINFImageEx;								  // 2011. 10. 10 by jskim UI�ý��� ����


class CINFUnitNameInfo : public CINFBase
{
public:
	CINFUnitNameInfo(CAtumNode* pParent);
	virtual ~CINFUnitNameInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();

	// 2013-03-06 by bhsohn ���� ���� �ý���
// 	void	RenderCharacterInfo(int x, int y, CChatMoveData* pChatData, char* strName, 
// 	BOOL	bShowHP, float fHP, int nGuildUniqueNumber, int nCharacterUniqueNumber, char *strMent, DWORD dwNameColor);
	void	RenderCharacterInfo(int x, int y, CChatMoveData* pChatData, char* strName,
		BOOL	bShowHP, float fHP, int nGuildUniqueNumber, int nCharacterUniqueNumber,
		char *strMent, DWORD dwNameColor, BYTE bRtnUser, BYTE RaceMask = RACE_DECA, BYTE InfluenceMask = INFLUENCE_TYPE_NORMAL);

	void	RenderRtnUserMark(int ntartX, int ntartY);
	// END 2013-03-06 by bhsohn ���� ���� �ý���

	void	RenderChatBalon(int x, int y, int cx, int cy);
	void	ChatCreateInfo(int nStringLength, const char *strMsgChat, BYTE NameColor, const char* strFromCharacterName);
	// 2009. 09. 18 by ckPark ���Ǵ�Ƽ �ʵ� HP�� ���� ���� ��ȭ
	void	MonsterChatCreateInfo( int nStringLength, char *strMsgChat, int nMonsterUID );
	// end 2009. 09. 18 by ckPark ���Ǵ�Ƽ �ʵ� HP�� ���� ���� ��ȭ
	static DWORD GetNameColor(const CShuttleChild* shuttle);
	static DWORD GetInfluenceColor(const MEX_OTHER_CHARACTER_INFO& info);
	
public:
//	CINFImage	*	m_pUnitInfo;
//	CINFImage	*	m_pPartyUnitInfo;
//	CINFImage	*	m_pMonsterInfo;
//
//	CINFImage	*	m_pPartyUnitHP;
//	CINFImage	*	m_pPartyUnitEP;
//	CINFImage	*	m_pMonsterHP;
//	CINFImage	*	m_pUp;
//	CINFImage	*	m_pDown;
	CINFImageEx	*	m_pBalonChat[NAME_INFO_BALON_CHAT];				   // 2011. 10. 10 by jskim UI�ý��� ����

	CD3DHanFont*	m_pFontMent;

	float			m_fRenderDistance;
	int		nOldx;
	int		nOldy;
	char	m_strChatFullLen[1024];

	// 2007-02-23 by bhsohn ID�� �ڷ� �и��� ���� ó��	
	POINT	m_ptUnitNameFirst;	// ó����ġ	

	// 2013-03-06 by bhsohn ���� ���� �ý���
	CINFImageEx*	m_pImageRtnUserMark;
	// END 2013-03-06 by bhsohn ���� ���� �ý���
};

#endif // !defined(AFX_INFUNITNAMEINFO_H__580A7F50_EF83_4ADC_B707_DE01602B3EE7__INCLUDED_)

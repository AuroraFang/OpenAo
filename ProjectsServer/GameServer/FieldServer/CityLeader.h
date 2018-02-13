// CityLeader.h: interface for the CCityLeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CITYLEADER_H__828585AD_B4A6_45DB_AA50_91756BA520E8__INCLUDED_)
#define AFX_CITYLEADER_H__828585AD_B4A6_45DB_AA50_91756BA520E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFieldIOCP;

typedef 	mt_vector<SCITYLEADER_INFO>		mtvectSCityLeader;

class CCityLeader  
{
public:
	CCityLeader();
	virtual ~CCityLeader();

	void	InitCityLeader(CFieldIOCP * i_pFIOCP);
	void	SetCityLeaderByLoadDB(vectSCityLeaderInfo * i_pSCityLeaderInfo);				// ���������� ������ DB�� ���� �о� �帰��.

	BOOL	CheckIsValidCharacter(int i_nMapIndex, UID32_t i_nCharacterUID);				// ��ȿ�� �������� üũ
	CHAR * 	GetNotice(int i_nMapIndex);		// �������� ��������
	void	SetNotice(int i_nMapIndex , UID32_t i_nCharacterUID, char * i_pNotice);	// �������� ����
	
	void	GetExpediencyfund(int i_nMapIndex, SCITYLEADER_INFO * o_pSCityLeaderInfo);	// �ǰ��� ���� ������
	void	UpdateExpediencyfundRateATLeader(int i_nMapIndex, int i_nChangeExpediencyFundRate, SCITYLEADER_INFO * o_pSCityLeaderInfo);  // 2008-08-20 by dhjin, �ǰ������� INT������ ������̴�. float -> INT�� ����	// ������ �ǰ����� ������ �ǰ��� ���� ������

	Err_t	PayBackExpediencyfund(int i_nMapIndex , UID32_t i_nCharacterUID, int i_nPayBackCount, SCITYLEADER_INFO * o_pSCityLeaderInfo);	// �ǰ��� ��û 

	void	SetExpediencyfund(byte i_byInfluence, int i_nSPICount);	// �ǰ��� ����
	void	InitExpediencyfund(int i_nMapindex, byte i_byInfluence, UID32_t i_nCharacterUID, float i_fExpediencyRate);	// �ǰ��� �ʱ�ȭ

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	
	void	SetExpediencyFundRate(int i_nMapIndex , UID32_t i_nCharacterUID, int i_nExpediencyFundRate); // �ǰ����� ����

protected:

	CFieldIOCP						*m_pFieldIOCP19;
	mtvectSCityLeader 				m_mtvectCityLeader;	// ���������� ����

};

#endif // !defined(AFX_CITYLEADER_H__828585AD_B4A6_45DB_AA50_91756BA520E8__INCLUDED_)

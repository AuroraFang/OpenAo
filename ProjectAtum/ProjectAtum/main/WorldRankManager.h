// WorldRankManager.h: interface for the CWorldRankManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORLDRANKMANAGER_H__DCE17F75_BAC9_420C_B0CA_0A56FF92D8EA__INCLUDED_)
#define AFX_WORLDRANKMANAGER_H__DCE17F75_BAC9_420C_B0CA_0A56FF92D8EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���� ��ŷ ��
#define		WORLDRANK_SERVICE_LOCAL		0		// ������ŷ
#define		WORLDRANK_SERVICE_WORLD		1		// ���� ��ŷ
#define		MAX_WORLDRANK_SERVICE		2



// ���� ��ŷ �׸��
#define		WORLDRANK_INFO_LV		0		// ����
#define		WORLDRANK_INFO_HONOR	1		// ��ġ
#define		WORLDRANK_INFO_PVP		2		// Pvp
#define		MAX_WORLDRANK_INFO		3

// �� ���� 
#define		WORLDRANK_GEAR_ALL		0		// ��ü ���
#define		WORLDRANK_GEAR_B		1		// B
#define		WORLDRANK_GEAR_M		2		// M
#define		WORLDRANK_GEAR_I		3		// I
#define		WORLDRANK_GEAR_A		4		// A
#define		MAX_WORLDRANK_GEAR		5		// ��ü ���,B,M,I,A (�� 5���̴�.)


#define		MAX_WORLDRANK_ONEPAGE	10		// �������� �ִ� �׸��
#define		MAX_WORLDRANK_PAGE		10		// �ִ� ������ ��

struct structRankInfo
{
	int				nRank;				// ��ŷ
	int				nLevel;				// ����
	int				nScore;				// ���ھ�
	int				nHonor;				// ���θ�
	int				nWinNum;			// �¼�
	int				nLoseNum;			// �й�
	
	char			chNickName[SIZE_MAX_ARENA_FULL_NAME];		// �г���	
	char			chGearType[SIZE_MAX_ARENA_FULL_NAME];		// ��� Ÿ��
	char			chInfluence[SIZE_MAX_ARENA_FULL_NAME];		// ���¸�
	char			chServiceName[MAX_PATH];		// ���񽺻�
	char			chServerName[MAX_PATH];			// ������	
	
};

struct structServiceInfo
{	
	int	nDataPos;
	BOOL bImageLoadDone;
	CINFImageEx* pImage;	        // 2011. 10. 10 by jskim UI�ý��� ����
	char*		pData;
	BOOL	bRestored;
};

typedef map<UINT, structServiceInfo*>		CMapServiceInfo;				// ��� ��ũ
typedef map<UINT, structServiceInfo*>::iterator CMapServiceInfoIterator;

class CWorldRankManager  
{
public:
	CWorldRankManager();
	virtual ~CWorldRankManager();

	
	void AddWorldRank(int i_nLocalWorldIdx,			// �����̳�? �����?
						int i_nInfoIdx,			// ����, ��ġ, PVP
						int i_nGearIdx,			// ��� �ε���
						structRankInfo*		pstruRankInfo);

	int GetWorldRankSize(int i_nLocalWorldIdx,			// �����̳�? �����?
							int i_nInfoIdx,			// ����, ��ġ, PVP
							int i_nGearIdx,			// ��� �ε���
							int i_nPage);			// ������ ��ġ

	structRankInfo* GetWorldRankInfo(int i_nLocalWorldIdx,			// �����̳�? �����?
										int i_nInfoIdx,			// ����, ��ġ, PVP
										int i_nGearIdx,			// ��� �ε���
										int i_nSelectPage,		// ������ ������
										int i_nIdx);			// ���� �ε���	
	BOOL	RqGetServiceInfo();	// ���� ����Ʈ�� ���´�.

	void	AddServiceInfo(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK* pMsg);
	MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK		GetServiceInfo();
	BOOL	GetServiceStringInfo(char* o_pServer);

	void	AddServiceInfoImage(MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK_IMAGE* pMsg);
	CINFImage* GetServiceInfoImage();
	void	DeleteServerImageInfo();

	BOOL	RqWorldRankInfo(int i_nLocalWorldIdx,			// �����̳�? �����?
						int i_nInfoIdx,			// ����, ��ġ, PVP
						int i_nGearIdx,			// ��� �ε���
						int i_nIdx,
						int i_nRqItemCnt,
						BOOL bCtrlRq);

	// ���� ��ŷ ���� ��û
	int GetMyRankInfo(int i_nInfoIdx);// ����, ��ġ, PVP)
	BOOL RqGetMyRankInfo();

	void AddWorldRank_Lv(int nStartRank, SWRK_LEVEL* pSwrk);
	void AddWorldRank_Fame(int nStartRank, SWRK_FAME* pSwrk);
	void AddWorldRank_PVP(int nStartRank, SWRK_PVP* pSwrk);

	BOOL	IsRqWorldRankInfo(int i_nLocalWorldIdx,			// �����̳�? �����?
								int i_nInfoIdx,			// ����, ��ġ, PVP
								int i_nGearIdx,			// ��� �ε���
								int i_nStartIdx);		// ���� ������

	void	RqCreateMyNickName(char* i_pNickName, UID32_t	i_myCharacterUniqueNumber);
	BOOL	IsValidNickName(char* i_pCharater);

	void	SetMyRankInfo(MSG_FC_INFO_WRK_GET_SELF_RANKING_OK	*pstructMyRankInfo);

	void	InitDeviceObjects_ServiceInfoImage();
	void	RestoreDeviceObjects_ServiceInfoImage();
	void	InvalidateDeviceObjects_ServiceInfoImage();
	void	DeleteDeviceObjects_ServiceInfoImage();

	void	RqInitWorldRank();

	void	SetUseWorldRanking(BOOL bUseWorldRanking);
	BOOL	GetUseWorldRanking();

private:	
	vector<structRankInfo>		m_vecLocalRank_LV[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// ���� ��ŷ_����
	vector<structRankInfo>		m_vecLocalRank_Fame[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// ���� ��ŷ_��ġ
	vector<structRankInfo>		m_vecLocalRank_PVP[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// ���� ��ŷ_PVP

	vector<structRankInfo>		m_vecWorldRank_LV[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// ���� ��ŷ_����
	vector<structRankInfo>		m_vecWorldRank_Fame[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// ���� ��ŷ_��ġ
	vector<structRankInfo>		m_vecWorldRank_PVP[MAX_WORLDRANK_PAGE][MAX_WORLDRANK_GEAR];			// ���� ��ŷ_PVP

	MSG_FC_INFO_WRK_GET_SERVICE_INFO_OK		m_ServiceInfo;

	structServiceInfo*			m_ServiceInfoStruct;				// ���� ��ũ ��ũ

	// ��û�� ��� �ε���
	int							m_nRqGearIdx;

	MSG_FC_INFO_WRK_GET_SELF_RANKING_OK	m_structMyRankInfo;

	BOOL					m_bUseWorldRanking;		// ���� ��ŷ �ý��� ��� ����

};

#endif // !defined(AFX_WORLDRANKMANAGER_H__DCE17F75_BAC9_420C_B0CA_0A56FF92D8EA__INCLUDED_)

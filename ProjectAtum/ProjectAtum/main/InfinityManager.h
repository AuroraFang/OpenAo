// InfinityManager.h: interface for the CInfinityManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include "AtumProtocol.h"
#include <vector>

struct tINFINITY_DIFFICULTINFO
{
	tINFINITY_DIFFICULTINFO()
	{
		memset(this, 0x00, sizeof(tINFINITY_DIFFICULTINFO));
	}

	void SetLevel(const INT a_iLevel)
	{
		const INT a_iDefPer = 100;
		const INT a_iBonusPer = 10;

		this->iLevel = a_iLevel;

		this->iLevel = min(this->iLevel, 21);
		this->iLevel = max(this->iLevel, 0);

		this->iBonusExpPer = a_iDefPer + (this->iLevel - 5) * a_iBonusPer;
		this->iBonusDropChancePer = a_iDefPer + (this->iLevel - 5) * a_iBonusPer;
		this->iBonusDropCountPer = a_iDefPer + (this->iLevel - 5) * a_iBonusPer;
	}

	void SetDefault(void)
	{
		SetLevel(5);
	}

	void Reset(void)
	{
		memset(this, 0x00, sizeof(tINFINITY_DIFFICULTINFO));
	}

	INT iLevel;
	INT iBonusExpPer;
	INT iBonusDropChancePer;
	INT iBonusDropCountPer;

};

struct MyInfinityRoomInfo
{
	ClientIndex_t			MasterIndex;
	InfinityCreateUID_t		InfinityCreateUID;
	INFINITY_MODE_INFO		ModeInfo;

	// 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.
	MONSTER_BALANCE_DATA	sDifficultyInfo;
	// End. 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.

	MyInfinityRoomInfo()
	{
		MasterIndex = 0;
		InfinityCreateUID = 0;
		memset(&ModeInfo, 0, sizeof(ModeInfo));
		// 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.
		sDifficultyInfo.Init(0);
		// End. 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.
	}
};

class CItemInfo;

class CInfinityManager
{
private:
	// ���� ��� ����
	std::vector<INFINITY_MODE_INFO*>		m_vecModeInfo;

	// ���� ���� ����Ʈ����Ʈ
	std::vector<INFINITY_READY_LIST*>		m_vecReadyInfo;

	// ���� ��� ��� ����Ʈ
	std::vector<INFINITY_MEMBER_INFO_LIST*>	m_vecMemberInfo;

	// ���� ���̵� ���� - ���ʽ� ���� ����Ʈ.
	vectorInfinity_DifficultyInfo_Bonus		m_vecInfinityDifficultyBonusInfo_ListData;

	// ���� ������ ���
	int									m_nZoneIdx;

	// ���� ������ ��
	int									m_nRoomIdx;

	// ���� ������ ���
	int									m_nMemberIdx;

	// �� ���Ǵ�Ƽ �� ����
	MyInfinityRoomInfo					m_MyRoomInfo;

	// ���� ��û�� ����Ʈ
	std::deque<MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER*> m_deqRequest;

	// �����Ϸ��� Ŭ�� �ε���
	ClientIndex_t						m_RejectClientIdx;

	// ���� �����Ϸ��� �� ����
	MSG_FC_INFINITY_CREATE				m_CreateRoomInfo;

	// 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)

//  // ���� ������ ���
//  MSG_FC_INFINITY_TENDER_DROPITEM_INFO m_CurTenderItem;
//  
//  // ���� �����ϴ� ������ ����
//  CItemInfo*							 m_pCurTenderItemInfo;
// 
// 	// ���� ������ ���
// 	std::vector<MSG_FC_INFINITY_TENDER_DROPITEM_INFO*>	m_vecTenderItemList;

	// end 2010. 04. 13 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� ������� ����)

	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	// ���Ǵ�Ƽ ���̵� ���� ���� ���� ����.
	INT									m_iDifficultyLevelMin;
	INT									m_iDifficultyLevelMax;
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

public:
	CInfinityManager();
	virtual ~CInfinityManager();

public:
	// ���Ǵ�Ƽ ��� ����
	void	ClearModeInfo();

	void	AddModeInfo(INFINITY_MODE_INFO* pModeInfo);

	size_t	GetInfinityModeCount()
	{
		return m_vecModeInfo.size();
	}

	INFINITY_MODE_INFO*	GetInfinityModeByIndex(size_t idx)
	{
		return m_vecModeInfo[idx];
	}

	int		GetInfinityModeIdx()
	{
		return m_nZoneIdx;
	}

	inline	void	SetInfinityModeIdx(int nZoneIdx)
	{
		m_nZoneIdx = nZoneIdx;
	}



	// ���Ǵ�Ƽ ��� ����
	void	ClearMemberInfo();

	void	AddMemberInfo(INFINITY_MEMBER_INFO_LIST* pMemberInfo);

	void	RemoveMemberInfo(ClientIndex_t ClientIdx);

	// 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)
	BOOL	IsAllReady();
	// end 2010. 03. 03 by ckPark ���Ǵ�Ƽ �ʵ� 2��(���� UI����)

	INFINITY_MEMBER_INFO_LIST*	GetMemberInfoByClientIdx(ClientIndex_t ClientIdx);

	INFINITY_MEMBER_INFO_LIST*	GetMemberInfoByCName(const char *a_pszCName);			// ĳ���͸����� �ɹ� ���� ã��.

	inline	size_t	GetMemberCount(void)
	{
		return m_vecMemberInfo.size();
	}

	inline	INFINITY_MEMBER_INFO_LIST*	GetMemberInfoByIndex(size_t idx)
	{
		return m_vecMemberInfo[idx];
	}

	inline	int		GetMemberIdx(void)
	{
		return m_nMemberIdx;
	}

	inline	void	SetMemberIdx(int nMemberIdx)
	{
		m_nMemberIdx = nMemberIdx;
	}



	// ���Ǵ�Ƽ ���� ����
	void ClearReadyInfo();

	void AddReadyInfo(INFINITY_READY_LIST* pReadyInfo);

	size_t	GetInfinityReadyInfoCount()
	{
		return m_vecReadyInfo.size();
	}

	INFINITY_READY_LIST* GetInfinityReadyByIndex(size_t idx)
	{
		return m_vecReadyInfo[idx];
	}

	int GetInfinityReadyIdx() const
	{
		return m_nRoomIdx;
	}

	inline	void	SetInfinityReadyIdx(int nRoomIdx)
	{
		m_nRoomIdx = nRoomIdx;
	}



	// ���� ���� ���� ����
	void SetMyRoomInfo(ClientIndex_t MasterIndex,
		InfinityCreateUID_t CreateUID,
		INFINITY_MODE_INFO* pModeInfo);

	MyInfinityRoomInfo* GetMyRoomInfo()
	{
		return &m_MyRoomInfo;
	}

	void ClearMyRoomInfo()
	{
		memset(&m_MyRoomInfo, 0, sizeof(MyInfinityRoomInfo));
	}


	// 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.
	// ���� ���� ���� ���̵� ���� ����.
	BOOL	SetMyRoomInfo_Difficult(const INT a_InfinityDifficultyLevel);
	// End. 2010. 04. 28 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����.

	// 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )
	// ���Ǵ�Ƽ ���̵� - ���ʽ� ���� ���.
	void	Register_InfinityDifficultyBonusInfo_ListData(INFINITY_DIFFICULTY_BONUS_INFO *pInfiBonusInfo);

	// ��Ƽ��Ƽ ���̵� - ���ʽ� ������ ����.
	const	INFINITY_DIFFICULTY_BONUS_INFO *Get_InfinityDifficultyBonusInfo(const INT a_DifficultyLevel);

	// ���Ǵ�Ƽ ���̵� - ���ʽ� ���� ��� ����.
	void ClearInfinityDifficultyBonusInfo_ListData()
	{
		m_vecInfinityDifficultyBonusInfo_ListData.clear();

		m_iDifficultyLevelMin = 1;
		m_iDifficultyLevelMax = DEFAULT_LEVEL_INFINITY_DIFFICULTY;
	}

	INT Get_DifficultyLevelMin() const { return m_iDifficultyLevelMin; }
	INT Get_DifficultyLevelMax() const { return m_iDifficultyLevelMax; }
	// End 2010. 05. 19 by hsLee ���Ǵ�Ƽ �ʵ� 2�� ���̵� ����. (��ȣó�� + ���� ó��(����) )

	// �� ���� ��û��
	void	AddJoinRequest(MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER* pJoinRequest);

	MSG_FC_INFINITY_JOIN_REQUEST_MASTERUSER*	GetFirstJoinRequest(void);

	void	RemoveFirstJointRequest(void);

	void	RemoveJoinRequest(ClientIndex_t nRequesterIdx);

	void	ClearJoinRequest(void);



	// ���� ��û��
	void SetRejectClientIdx(ClientIndex_t RejectClientIdx)
	{
		m_RejectClientIdx = RejectClientIdx;
	}

	ClientIndex_t GetRejectClientIdx() const
	{
		return m_RejectClientIdx;
	}

	void ClearRejectClientIdx()
	{
		m_RejectClientIdx = 0;
	}



	// ���� �����ϴ� �� ����
	void SetCreateRoomInfo(MSG_FC_INFINITY_CREATE* pCreateRoomInfo)
	{
		memcpy(&m_CreateRoomInfo, pCreateRoomInfo, sizeof(m_CreateRoomInfo));
	}

	MSG_FC_INFINITY_CREATE* GetCreateRoomInfo() { return &m_CreateRoomInfo; }

	void ClearCreateRoomInfo() { memset(&m_CreateRoomInfo, 0, sizeof(m_CreateRoomInfo)); }

	void ClearManager()
	{
		ClearModeInfo();

		ClearReadyInfo();

		ClearMemberInfo();

		ClearMyRoomInfo();

		ClearJoinRequest();

		ClearRejectClientIdx();

		ClearCreateRoomInfo();

		ClearInfinityDifficultyBonusInfo_ListData();
	}
};

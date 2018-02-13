// TriggerCrystalGroup.h: interface for the CTriggerCrystalGroup class.
//
// \brief		// 2011-10-28 by hskim, EP4 [Ʈ���� �ý���] - ũ����Ż �ý���
// \author		hskim
// \date		2011-10-28
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERCRYSTALGROUP_H__CC85E831_1C40_427D_BCE0_0389C1778F9A__INCLUDED_)
#define AFX_TRIGGERCRYSTALGROUP_H__CC85E831_1C40_427D_BCE0_0389C1778F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapTriggerManager;
class CTriggerFunctionCrystal;
class CTriggerCrystalDestroyGroup;

class CTriggerCrystalGroup
{
	friend class CTriggerCrystalGroup;
	friend class CTriggerFunctionCrystal;
	friend class CTriggerCrystalDestroyGroup;

public:
	CTriggerCrystalGroup();
	virtual ~CTriggerCrystalGroup();

	void Clear();
	BOOL Create(CMapTriggerManager *pMapTriggerManager, CTriggerFunctionCrystal *pTriggerFunctionCrystal, DestroyGroupID_t DestroyGroupID, EventID_t EventID);
	void Destroy();

	BOOL InsertTriggerCrystalDestroy(CTriggerCrystalDestroyGroup *pTriggerCrystalDestroy);
	BOOL IsBelongCrystal(MonIdx_t MonsterIdx);
	void SortTriggerCrystalDestroy();
	
	EventID_t GetEventID()						{ return m_EventID; }
	void SetEventID(EventID_t i_EventID)		{ m_EventID = i_EventID; }

	BOOL CheckDestroyEvent();			// �ı� �̺�Ʈ�� �߻��ߴ��� Ȯ��
	BOOL CheckDestroySequence();		// �ı� ������ �´��� Ȯ��

public:
	// DefaultCrystalGroup �� ���
	BOOL CreateAllCrystal();		// ��� ũ����Ż ����
	BOOL DestroAllyCrystal();		// ��� ũ����Ż ����

protected:
	DestroyGroupID_t	m_DestroyGroupID;
	EventID_t			m_EventID;

	vector<CTriggerCrystalDestroyGroup *>	m_vectorTriggerCrystalDestroy;		// �ʱ� ���ý� �����ǰ� ������� �ʴ´�. ����ȭ �ʿ� ����

	CMapTriggerManager		*m_pMapTriggerManager;
	CTriggerFunctionCrystal	*m_pTriggerFunctionCrystal;
};

#endif // !defined(AFX_TRIGGERCRYSTALGROUP_H__CC85E831_1C40_427D_BCE0_0389C1778F9A__INCLUDED_)

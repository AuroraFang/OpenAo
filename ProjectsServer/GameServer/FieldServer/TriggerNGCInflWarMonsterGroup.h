// TriggerNGCInflWarMonsterGroup.h: interface for the CTriggerNGCInflWarMonsterGroup class.
//
/// \brief		// 2011-11-07 by hskim, EP4 [Ʈ���� �ý���] - NCG ������ �ý���
/// \author		hskim
/// \date		2011-11-07
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGGERNGCINFLWARMONSTERGROUP_H__7A726068_225B_4A0D_A2A9_D2D47905A27A__INCLUDED_)
#define AFX_TRIGGERNGCINFLWARMONSTERGROUP_H__7A726068_225B_4A0D_A2A9_D2D47905A27A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapTriggerManager;
class CTriggerFunctionNGCInflWar;
class CTriggerNGCInflWarGroup;
class CTriggerFunction;	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����

class CTriggerNGCInflWarMonsterGroup  
{
public:
	CTriggerNGCInflWarMonsterGroup();
	virtual ~CTriggerNGCInflWarMonsterGroup();

	void Clear();
	BOOL Create(CMapTriggerManager *pMapTriggerManager, CTriggerFunction *pTriggerFunction, MonIdx_t Monster, INT iSummonCount, INT iSummonRandomPos);	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
	void Destroy();

	BOOL CreateMonster();
	BOOL DestroyMonster();

protected:
	MonIdx_t m_Monster;
	INT m_iSummonCount;
	INT m_iSummonRandomPos;

	CMapTriggerManager			*m_pMapTriggerManager;
	CTriggerFunction			*m_pTriggerFunction;	// 2013-01-21 by jhseol, NGC �������� Ʈ���� �ý��� - ��� Ŭ������ ���� Ŭ������ ����
	CTriggerNGCInflWarGroup		*m_pTriggerNGCInflWarGroup;
};

#endif // !defined(AFX_TRIGGERNGCINFLWARMONSTERGROUP_H__7A726068_225B_4A0D_A2A9_D2D47905A27A__INCLUDED_)

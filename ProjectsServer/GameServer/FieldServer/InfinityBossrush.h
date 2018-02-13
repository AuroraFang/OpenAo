/// InfinityBossrush.h: interface for the CInfinityBossrush class.
/// \brief		���Ǵ�Ƽ - �������� ��� ���Ǵ�Ƽ
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYBOSSRUSH_H__D6BD7F63_E337_4B84_9A20_F0573C899AAF__INCLUDED_)
#define AFX_INFINITYBOSSRUSH_H__D6BD7F63_E337_4B84_9A20_F0573C899AAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InfinityBase.h"

class CFieldIOCPSocket;
class CFieldMapChannel;
class CInfinityBossrush : public CInfinityBase  
{
public:
	CInfinityBossrush();
	virtual ~CInfinityBossrush();

	void InitInfinityBossrush();
	void DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime);
	
	void ProcessingCinema(DWORD i_dwCurTick, DWORD i_dwPassTick);		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -

	virtual void ProcessingCinema ( CFieldIOCPSocket * i_MasterFISoc , int nUpdateScene = 1 );		// 2010. 06. 04 by hsLee ��Ƽ�Ǵ� �ʵ� 2�� ���̵� ����. (GM ��ɾ� �߰�. /nextscene(���� �ó׸� �� ȣ��.) )

protected:
	INT		m_CheckDelayForcedCreateMonsterCount;		// ���� ��ȯ üũ�� ���� ������ ī��Ʈ COUNT_DELAY_FORCED_CREATE_MONSTER_TERM �ڿ� �ٽ� üũ�Ѵ�.
	INT		m_CheckDelayForcedFin;		// ���� ���� üũ�� ���� ������ 
};

#endif // !defined(AFX_INFINITYBOSSRUSH_H__D6BD7F63_E337_4B84_9A20_F0573C899AAF__INCLUDED_)

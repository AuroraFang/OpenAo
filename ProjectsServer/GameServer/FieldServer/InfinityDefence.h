/// InfinityDefence.h: interface for the CInfinityDefence class.
/// \brief		���Ǵ�Ƽ - ������� ��� ���Ǵ�Ƽ
/// \author		dhjin
/// \date		2009-09-09 ~ 2010
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFINITYDEFENCE_H__C0CE8D04_0406_4020_B600_70E62F775224__INCLUDED_)
#define AFX_INFINITYDEFENCE_H__C0CE8D04_0406_4020_B600_70E62F775224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InfinityBase.h"

class CFieldIOCPSocket;
class CFieldMapChannel;
class CInfinityDefence : public CInfinityBase  
{
public:
	CInfinityDefence();
	virtual ~CInfinityDefence();

	void InitInfinityDefence();
	void DoSecondlyWorkInfinity(ATUM_DATE_TIME *pDateTime);

	void ProcessingCinema(DWORD i_dwCurTick, DWORD i_dwPassTick);		// 2010-03-31 by dhjin, ���Ǵ�Ƽ(�������) -

	virtual void ProcessingCinema ( CFieldIOCPSocket * i_pMasterFISoc , int nUpdateScene = 1 );	// 2010. 06. 04 by hsLee ��Ƽ�Ǵ� �ʵ� 2�� ���̵� ����. (GM ��ɾ� �߰�. /nextscene(���� �ó׸� �� ȣ��.) )

protected:

};

#endif // !defined(AFX_INFINITYDEFENCE_H__C0CE8D04_0406_4020_B600_70E62F775224__INCLUDED_)

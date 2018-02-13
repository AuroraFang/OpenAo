// INFUnitState.h: interface for the CINFUnitState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFUNITSTATE_H__5ACA54A9_72D9_4987_B045_F91075CEF5CC__INCLUDED_)
#define AFX_INFUNITSTATE_H__5ACA54A9_72D9_4987_B045_F91075CEF5CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "INFBase.h"

// Unit Warning and State
typedef enum
{
	UNIT_WARN_DANGER,
	UNIT_WARN_OVERHEAT,
	UNIT_WARN_LOWFUEL,
	UNIT_WARN_FULLITEM,
	UNIT_WARN_TOOHIGH,
	UNIT_WARN_LOWSHIELD,
	UNIT_WARN_FULLITEM1,
	UNIT_WARN_FULLITEM2,
	UNIT_WARN_FULLITEM3,

	UNIT_STATE_TAKEOFF,
	UNIT_STATE_STOP,
	UNIT_STATE_LOCK,
	UNIT_STATE_FORM,
	UNIT_STATE_LAND,

	UNIT_WARN_STATE_MAX
} UnitWarningState;

/// Class Tick : ����
/// ***************************************************************** ///
///		| ---------------|-----------------------|
///   timePos1		timeMiddlePos				timePos2
///
///   �ð��� timePos1 ���� ~ timePos2 ���̸� �ݺ��Ѵ�.
///   timePos1~timeMiddlePos ������ �̹����� �����ְ�
///	  timePos1~timeMiddlePos ������ �̹����� �Ⱥ��� ������
///   �̹����� �������� ó���Ѵ� 
/// ***************************************************************** ///

class CINFImage;
class CINFImageEx;										   // 2011. 10. 10 by jskim UI�ý��� ����

class CINFUnitState  : public CINFBase
{
public:
	CINFUnitState(CAtumNode* pParent);
	CINFUnitState();
	virtual ~CINFUnitState();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();

	void Render();
	virtual void Tick();
	
	void SetStateDescription( int iLevel );		// ���� ������ ���� ����â�� ���� ����
	void InsertState( DWORD dwState );			// ������ ���� ���� ����
	void SetState( DWORD dwState );				// ������ ���� ���� �V��
	BOOL GetState( DWORD dwState );				// ������ ���ֻ��°� �V�� �̴��� ����
	void OffState( DWORD dwState );				// ������ ���ֻ��¸� ���� ��Ŵ
	int  CheckState( BOOL bInserted = TRUE ); // bInserted = true �� // �߰��� ���Ե� ���°� �ִ��� üũ�ϰ� image �ε����� �����Ѵ�.

public:
	// init()
	BOOL m_bRestored;

	// image data
	CINFImageEx	*m_pImgState[UNIT_WARN_STATE_MAX];			// ���ֻ��� �̹���
	CINFImageEx	*m_pImgStateStr[UNIT_WARN_STATE_MAX];		// ���ֻ��� ���� �̹���

	CRemainTime		m_tInsertedStateRemainTime;				// ���°� �����ִ� �ð�
	CRangeTime		m_tBlinkTime;							// �������� ���� �����ð�
	// tick
	DWORD m_dwState;
	DWORD m_dwInsertedState;			// �߰��� ���ԉ�����
	int	  m_dwInsertedStateCountSum;	// ���Ե� ���¸� �󸶵��� �����ֳ�

	float m_fElapsedTime;				// ���ø����̼� Tick������ ����ð�
	float m_fElapsedTimeSum;			// m_fElapsedTime�� ��

	float m_fTimeMiddlePos;				// �������� ���� ������ �߰�����
    float m_fTimeDirection;				// �ð��� ��ȸ ����
};

#endif // !defined(AFX_INFUNITSTATE_H__5ACA54A9_72D9_4987_B045_F91075CEF5CC__INCLUDED_)

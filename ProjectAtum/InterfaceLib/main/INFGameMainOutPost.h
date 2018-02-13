// INFGameMainOutPost.h: interface for the CINFGameMainOutPost class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGAMEMAINOUTPOST_H__D633B4F2_F06B_402C_A828_44E731AA0C97__INCLUDED_)
#define AFX_INFGAMEMAINOUTPOST_H__D633B4F2_F06B_402C_A828_44E731AA0C97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CAtumNode;
class CINFImage;
class CINFImageEx;
class CD3DHanFont;
typedef struct _OutPost
{
	_OutPost()
	{
		m_BState						= 0;
		m_fLifeTime						= 0.0f;
		
		// 2007-10-02 by dgwoo ��ȹ �߰� ���� ���� ��.
		m_fResetBarLife					= 0.0f;
		m_fResetBarTotalLife			= 0.0f;
		m_bResetBarRender				= FALSE;
		m_bAttackerInf					= 0;
		
		m_b10Min						= TRUE;


	}
	float			m_fLifeTime;									// �̹����� ���������� �����ð�.
	BYTE			m_BState;										// �������� ����.
	ATUM_DATE_TIME	m_timeWarStartTime;								// ���� ���� �ð�.
	ATUM_DATE_TIME	m_timeWarEndTime;								// ���� ������ �ð�.
	// 2007-10-02 by dgwoo ���½ð�.
	BOOL			m_bResetBarRender;								// ���� �ٸ� �׸����ΰ�?
	float			m_fResetBarLife;								// ���� �Ϸ������ �ð�.
	float			m_fResetBarTotalLife;							// ���� �Ϸ������ �ѽð�.
	BYTE			m_bAttackerInf;									// ������ ����.
	MapIndex_t		m_MapIndex;										// ����ǰ� �ִ� �ʹ�ȣ.
	BOOL			m_bOutPostImgShow;								// �������� ����(�̹���)�� ����Ұ��ΰ�?
	BOOL			m_b10Min;										// 10�г�������� ��Ʈ�� ���.
	CINFImage*		m_pImgTemp;

} STOUTPOST;

class CINFGameMainOutPost : public CINFBase  
{
public:
	CINFGameMainOutPost();
	virtual ~CINFGameMainOutPost();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	void Render();
	virtual void Tick();
	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void SetWarTime(ATUM_DATE_TIME i_DateTimeStart,ATUM_DATE_TIME i_DateTimeEnd);
	void SetResetTime(MapIndex_t i_MapIndex,ATUM_DATE_TIME i_DateTimeStart,ATUM_DATE_TIME i_DateTimeEnd);
	void SetResetTime(MapIndex_t i_MapIndex,INT i_OutPostWarResetRamainSecondTime);
	void SetAttInfluence(MapIndex_t i_MapIndex,BYTE i_AttInfluence);
	void SetResetBarRender(BOOL i_bResetBarRender);
	void SetOutPostState(BYTE BState,MapIndex_t i_MapIndex);
	BOOL SetOutPostImgShow();
	void AddOutPostMapIndex(MapIndex_t MapIndex);
	void DeleteOutPostMapIndex(MapIndex_t MapIndex);
	void DeleteAllOutPostMapIndex();
	BYTE GetOutPostState(MapIndex_t MapIndex);

	// 2007-11-27 by dgwoo �������� �������� �߹�. 
	void AddOutPost(MapIndex_t i_MapIndex,BYTE i_State,BYTE i_Influence,ATUM_DATE_TIME i_StartTime,ATUM_DATE_TIME i_EndTime,INT i_ResetTime = 0);
	STOUTPOST * FindOutPost(MapIndex_t i_MapIndex);
	BOOL IsOutPosting();

	

	CINFImageEx*		m_pImgProDestroy;								// �� �ı�.
	CINFImageEx*		m_pImgResetStart;								// ���� ����.
	CINFImageEx*		m_pImgResetSucc;								// ���� ����.
	CINFImageEx*		m_pImgResetDestroy;								// ���� �ı�.
	CINFImageEx*		m_pImgResetBarBack;								// ���¹� ���.
	CINFImageEx*		m_pImgResetBarBar1;								// ���¹�1(����:����)
	CINFImageEx*		m_pImgResetBarBar2;								// ���¹�2(�Ķ�:���)
	CINFImageEx*		m_pImgResetBarInf1;								// ���¹ټ��� 1.(����������)
	CINFImageEx*		m_pImgResetBarInf2;								// ���¹ټ��� 2.(�˸���)
	
	
	vector<STOUTPOST>		m_vecOutPost;							// �������� ������.
	BOOL					m_bOutPostImgShow;						// ����â�� ������ �ϴ°�?
	STOUTPOST*				m_pOutPostInfo;							// �ڽ��� �ִ� �ʰ� ������ �ִ� �������� ����.
	
	
//	float			m_fLifeTime;									// �̹����� ���������� �����ð�.
//	
//	BYTE			m_BState;										// �������� ����.
//
//	ATUM_DATE_TIME	m_timeWarStartTime;								// ���� ���� �ð�.
//	ATUM_DATE_TIME	m_timeWarEndTime;								// ���� ������ �ð�.
//	
//	// 2007-10-02 by dgwoo ���½ð�.
//	BOOL			m_bResetBarRender;								// ���� �ٸ� �׸����ΰ�?
//	float			m_fResetBarLife;								// ���� �Ϸ������ �ð�.
//	float			m_fResetBarTotalLife;							// ���� �Ϸ������ �ѽð�.
//
//	BYTE			m_bAttackerInf;									// ������ ����.
//
//
//	MapIndex_t		m_MapIndex;										// 
//	vector<MapIndex_t>		m_vecMapIndex;							// ���� ������������ �������� MapIndex
//	BOOL			m_bOutPostImgShow;								// �������� ����(�̹���)�� ����Ұ��ΰ�?
//
//	BOOL			m_b10Min;										// 10�г�������� ��Ʈ�� ���.
};

#endif // !defined(AFX_INFGAMEMAINOUTPOST_H__D633B4F2_F06B_402C_A828_44E731AA0C97__INCLUDED_)

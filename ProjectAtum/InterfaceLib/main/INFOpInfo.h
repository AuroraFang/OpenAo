// INFOpInfo.h: interface for the CINFOpInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPINFO_H__7AB5C199_64C1_4ABC_A921_06993890D1A4__INCLUDED_)
#define AFX_INFOPINFO_H__7AB5C199_64C1_4ABC_A921_06993890D1A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"
#include "INFImageListBox.h"
#include "INFArenaScrollBar.h"
#include "INFImageBtn.h"

#define PET_MASSAGE					0		// ���� �˷��ִ� �����̻� ���� �޼���
#define PET_MAIN_MESSAGE			1		// ���� �˷��ִ� ���� �޼��� - ���� �޼����� �����ϴ� �켱���� �־�� ��( ����, �� ������, ���� ���� )
#define	MAX_IMAGE					6		// 2010-06-15 by shcho&hslee ��ý��� - ���۷����� ����
#define	MAX_MODEL					1		// 2010-06-15 by shcho&hslee ��ý��� - ���� �з�

struct OP_INFO
{
	CINFImage* sImage;
	CINFImage* sBackImage;
	OperatorInfo* spOperatorInfo;
	float PlayCurrentTime;
	int x,y;
	BOOL live;
};

// type 1:  ���� ����, 2: �����̻��� �浹, 3: ����/������ �ٿ�, 4: ����, 5: ������, 6: ���� ����
enum PET_OP
{
	NONE_STATE,
	DEADLINE_FEUL, 
	MISSILE_WARNING, 
	DEADLINE_HPMP, 
	USER_DEAD, 
	PET_LEVELUP, 
	EQUIP_PET 
};

class CINFOpMain;
class CINFOpInfo
{
public: 
	CINFOpInfo(CINFOpMain* pParent); 
	virtual ~CINFOpInfo();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual void	Render();
	virtual void	Tick();
	
	int			WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	int			OnKeyDown(WPARAM wParam, LPARAM lParam);
	void		SetOperatorAction( int Type, int FunctionIndex );
	OP_INFO*	GetOperatorAction( int Type, int FunctionIndex );
	BOOL		CheckView( int Type, int FunctionIndex, int SourceIndex = NULL );
	void		RenderString();

private:
	CINFOpMain*			m_pParent;
	vector<OP_INFO*>	m_vecOpInfo;
	vector<OP_INFO*>	m_vecCurrentOpInfo;
	int					m_nPetState;
	int					m_PlayTime;
	CD3DHanFont*		m_pFontOpInfo;
};

#endif // !defined(AFX_INFOPINFO_H__7AB5C199_64C1_4ABC_A921_06993890D1A4__INCLUDED_)

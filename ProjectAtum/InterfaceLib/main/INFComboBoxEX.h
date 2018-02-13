// INFComboBoxEX.h: interface for the CINFComboBoxEX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFCOMBOBOXEX_H__9602BEBA_1E6C_4669_A340_274B401C23FD__INCLUDED_)
#define AFX_INFCOMBOBOXEX_H__9602BEBA_1E6C_4669_A340_274B401C23FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

#define R_OK		1
#define	R_ERR		0

typedef struct _Rect{
	int x;
	int y;
	int w;
	int h;
} RenderRect;


typedef struct _ComboInfo{
	int ItemCount;
	int SlotNum;
	int MaxLine;
	int SelectHeight;
	int SelectWidth;
} ComboInfo;

typedef struct _SCROLLINFO{
	int x;
	int y;
	int xOri;
	int yOri;
	int lenght;
	int barwidth;
	int barheight;
} ScrollBarInfo;

typedef	struct _ComboData{
	int nData;
	char DataName[125];

	BOOL operator < (const _ComboData &stDes)
	{
		if(strcmp(DataName, stDes.DataName) < 0)
		{
			return TRUE;
		}
		return FALSE;
	}
} ComboData;
class CINFComboBoxEX   : public CINFBase 
{
public:
	CINFComboBoxEX();
	CINFComboBoxEX(CAtumNode* pParent, CGameData* m_pGameData);
	virtual ~CINFComboBoxEX();
	void		InitSeting(CAtumNode* pParent, CGameData* m_pGameData);
	int			AddItem(void *pItem);
	int			DelItem(void *pItem);
	
	bool		SetScrollBarInfo(int x,int y, int lenght,int width,int height);
	bool		CreateImage(const char * Ground,const char * Select,const char * Scroll);
	int			ComboRender();
	void		SetComboInfo(int ItemCount,int SlotNum,int SelectWidth,int SelectHeight);
	void		SetComboPt(int x,int y);
	RenderRect	GetComboRect() { return m_rtCombo; 	}

	int				GetScrollLineNumber(float nLength);
	float			GetScrollLineInterval(int nScrollLineNum);
	int				GetItemIndexByPos(int nPosY);
	void			SetScrollEndLine();
	void			InitScrollLine();
	int				GetScrollLine();
	
	HRESULT		InitDeviceObjects();
	HRESULT		InvalidateDeviceObject();
	HRESULT		RestoreDeviceObject();
	HRESULT		DeleteDeviceObject();

	int			WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	RenderRect		m_rtCombo;						//�޺� �ڽ� ��ü 
	ComboInfo		m_ComboInfo;					//�޺� �ڽ��� �⺻ ����.
	bool			m_bScrollLock;
	SIZE			m_sizeItem;						//item Size
	int				m_nScrollIndex;					//��ũ���� ��ġ.
	
	int				m_nActiveItem;					//Ȱ��ȭ�� ������.

	int				m_nScrollMaxCount;				//item max count
	int				m_nScrollLine;
	int				m_nScrollTemp;
	int				m_nSelectIndex;					//���õ� ������ .
	int				m_nScrollBtn;					//

	ScrollBarInfo	m_ScrollBarInfo;				//��ũ�ѹ��� ����..
	int				m_nScrollPosition;				//��ũ���� ��ġ 
	
	CINFImageEx	*	m_pGroundImg;					//�޺� �ڽ� ���.
	CINFImageEx	*	m_pSelectImg;					//������ �׸�.
	CINFImageEx	*	m_pScrollImg;					//��ũ�� �׸�.

	CD3DHanFont *	m_pFontItemName;				//������ �̸�.

	char			m_strGround[16];
	char			m_strSelect[16];
	char			m_strScroll[16];

public:
	vector<ComboData>	m_vecData;					//�޺� �ڽ��� ���� Data
	int				m_nSelectDataIndex;					// ������ �޺� ����Ÿ �ε���
};

#endif // !defined(AFX_INFCOMBOBOXEX_H__9602BEBA_1E6C_4669_A340_274B401C23FD__INCLUDED_)

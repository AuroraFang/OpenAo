// BoardData.h: interface for the CBoardData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOARDDATA_H__5C2A8862_C4CE_40BE_B4F5_A035D47A81BE__INCLUDED_)
#define AFX_BOARDDATA_H__5C2A8862_C4CE_40BE_B4F5_A035D47A81BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CINFImage;
class CD3DHanFont;

#define D3DFVF_BOARDVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

struct BOARDVERTEX
{
    D3DXVECTOR3 p;
    FLOAT       tu, tv;
};

struct stBOARDSTRING
{
	CD3DHanFont	*				pBoardFont;				// ��Ʈ�ؽ���
	int							nBoardFontWidth;		// ��Ʈ �ؽ��� ����
	int							nBoardFontHeight;		// ��Ʈ �ؽ��� ����
	
	INT							StringIndex;			// ������ ��Ʈ�� �ε���

	BOOL						bUse;

	stBOARDSTRING()
	{
		pBoardFont = nullptr;
		nBoardFontWidth = 0;
		nBoardFontHeight = 0;

		StringIndex = 0;
		bUse = FALSE;
	}
};
typedef struct stBoard
{
	int							nBoardStringIndex;	// ���� ���� �ε���
	BOARDVERTEX					vPos[4];
	LPDIRECT3DVERTEXBUFFER9		pVBBoard;
	vector<stBOARDSTRING *>		vecBoardString;		// ���� ����
	float						fScrollMove;		// �̵� �ӵ�
	float						fCurrentMove;		// ���� �̵� ��ġ
	int							nTexWidth;			// �⺻ �ؽ��� ����
	int							nTexHeight;			// �⺻ �ؽ��� ����
	int							nTextCount;			// ���� ���� ����
	int							nRenderTextIndex;	// ���� ��� ����
	BOOL						bBoardMove;			// �����̴� �����ΰ�

	stBoard()
	{
		for(int i = 0; i < 4; i++)
		{
			vPos[i].p = D3DXVECTOR3(0,0,0);
			vPos[i].tu = 0.0f;
			vPos[i].tv = 0.0f;
		}
		nBoardStringIndex = -1;
		vecBoardString.clear();
		fScrollMove = 0.0f;
		fCurrentMove = 0.0f;
		nTexWidth = 0;
		nTexHeight = 0;
		nTextCount = 0;
		nRenderTextIndex = 0;
		bBoardMove = FALSE;
	}

}ELECTRICBOARD;

class CBoardData  
{
private:
	
public:
	HRESULT						InitDeviceObjects(char * pTexName);
	HRESULT						RestoreDeviceObjects();
	HRESULT						DeleteDeviceObjects();
	HRESULT						InvalidateDeviceObjects();

	void						Tick(float fElapsedTime);
	void						Render(float* fOffset);

	int							GetBoardIndex() { return m_nBoardIndex; }
	void						SetBoardStringUse(int nBoardStringIndex, int nStringIndex, BOOL bUse);

	void						AddBoard(int nBoardStringIndex, float fltx, float flty, float fWidth, float fHeight, int nTexWidth, int nTexHeight, float fMoveRate, BOOL bMove = FALSE);
	void						AddBoardNumber(int nNumberCount, float fltx, float flty, float fWidth, float fHeight, int nTexWidth, int nTexHeight);
	void						AddBoardString(int nBoardStringIndex, const char* pFontName, int nFontSize, int nStringIndex, char* pStr);
	void						SetBoardNumberString(char* pFontName, int nFontSize, char* pStr);

	BOOL						DelBoardString(int nBoardStringIndex, int nStringIndex);

	// 2009-02-13 by bhsohn ���� ��ŷ �ý���
	void						AddBoardWorldRank(int nNumberCount, float fltx, float flty, float fWidth, float fHeight, int nTexWidth, int nTexHeight);
	void						SetWorldRankBoardString(int i_nBoardStringIndex, int i_nX, const char* i_pFontName, int i_nFontSize, char* i_pStr, DWORD	i_dwColor);
	// end 2009-02-13 by bhsohn ���� ��ŷ �ý���

private:
	void						CheckBoardStringUse();
	
private:
	BYTE						InfluenceMask0;		// 2006-04-17 by cmkwon, ���� ����ũ
	
	int							m_nBoardIndex;		// ���� �ε���
	BOOL						m_bBothFaces;
	DWORD						m_dwColor;

	BOARDVERTEX					m_vMainPos[4];
	LPDIRECT3DVERTEXBUFFER9		m_pVBMainBoard;
	LPDIRECT3DTEXTURE9			m_pMainBoardTex;	// �⺻ ���� �ؽ���

	D3DXMATRIX					m_matWorld;
	D3DXMATRIX					m_matTrans;
	D3DXMATRIX					m_matRotX;
	D3DXMATRIX					m_matRotY;
	D3DXMATRIX					m_matRotZ;

	vector<ELECTRICBOARD *>		m_vecElectricBoard;
	vector<ELECTRICBOARD *>		m_vecElectricBoardNumber;		// ���� ������ ���� �ϳ��� ����

	char *						m_pBoardTextureName;

public:
	CBoardData(int nBoardIndex, float fWidth, float fHeight, DWORD dwColor, D3DXVECTOR3 vCenter, float fRotX = 0.0f, float fRotY = 0.0f, float fRotZ = 0.0f, BOOL bBothFaces = FALSE);
	virtual ~CBoardData();

};

#endif // !defined(AFX_BOARDDATA_H__5C2A8862_C4CE_40BE_B4F5_A035D47A81BE__INCLUDED_)

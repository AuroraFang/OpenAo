// INFGroupImage.h: interface for the CINFGroupImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFGROUPIMAGE_H__CEFB2A30_E046_40E7_B072_BBAAB1ECA858__INCLUDED_)
#define AFX_INFGROUPIMAGE_H__CEFB2A30_E046_40E7_B072_BBAAB1ECA858__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CINFImage;
class CINFImageEx;
class CINFImageList;

class CINFGroupImage
{
public:
	CINFGroupImage();
	virtual ~CINFGroupImage();

	HRESULT InitDeviceObjects( CINFImageList* pImageList );
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	void Tick();
	void CalculateMinMax();
	void UpdateTexture();

	HRESULT CreateRenderTexture();
	void Render();
	BOOL	IsImageModify();

	void AttachImage( CINFImage* pImageInfo );
	void DetachImage( CINFImage* pImageInfo );

	void Move(float x, float y); 
	void SetTransVector(D3DXVECTOR2 trans) { m_v2Trans = trans; }
	D3DXVECTOR2 GetMinPos() { return m_v2MinPos; }
	D3DXVECTOR2 GetMaxPos() { return m_v2MaxPos; }
	POINT	GetFindControlTargetofMinPos( char* nStrName );
	POINT	GetFindControlTargetofMaxPos( char* nStrName );
	POINT   GetImgSize();

	// 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������
	void SetColor(D3DCOLOR color); // �׷� �̹��� ���İ� ���� �Լ�
	// end 2012-10-12 by jhjang ���� �ƿ� ����ġ ���ʽ� �ý��� ������



protected:
	CINFImageList*			m_pUIImageInfo;
// 	LPDIRECT3DSURFACE9		m_lpRenderSurface;
// 	LPDIRECT3DTEXTURE9		m_lpRenderTexture;

	LPD3DXSPRITE			m_pd3dxSprite;

	vector<CINFImage*>		m_vecImageList;

	D3DXVECTOR2				m_v2MinPos;
	D3DXVECTOR2				m_v2MaxPos;

	D3DXVECTOR2				m_v2Trans;
};

#endif // !defined(AFX_INFGROUPIMAGE_H__CEFB2A30_E046_40E7_B072_BBAAB1ECA858__INCLUDED_)

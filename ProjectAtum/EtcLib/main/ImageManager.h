///////////////////////////////////////////////////////////////////////////////
// ImageManager Class 2005-04-28 by ydkim
/* Map ���� �⺻������ ��� �̹��� ����

	  * �̹��� Ÿ�� ������ 3���� ����Ʈ, ����, �۷ι��� �����Ѵ�	
	  * �̹��� ������ �⺻���� define�� IMAGEPUSHBACK���� �Ѵ� (�̹��� ������
	     m_mapImgBank���� ����
	  * ��Ÿ ���� �����̳� ���� ���� �κ��� ���� �߰�......
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIMAGEMANAGER_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_)
#define AFX_CIMAGEMANAGER_H__8B409C14_93C0_4282_B7B5_8367FF326F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INFBase.h"

class CINFImage;
class DataHeader;
class CGameData;

typedef enum
{
	IMAGE_START,						// ����Ʈ ȭ�鿡���� ���̴� �̹���
	IMAGE_MAIN,							// ���� ���ӿ����� ���̴� �̹���
	IMAGE_GLOBAL						// ����Ʈ�� ���ο��� �پ��̴� �̹���
}ImageType;

typedef struct {
	DWORD		dwImageType;
	CINFImage*	pINFImage;

	float		fAngle;					// �̹��� ȸ��
	float		fImgMagnification;		// �̹��� ����
	int			nGamma;					// �̹��� ����
}ImageInfo;

class CImageManager: public CINFBase
{

public:
	CImageManager();
	virtual ~CImageManager();
	
	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects(DWORD dwType);
	virtual HRESULT DeleteDeviceObjects(DWORD dwType);
	virtual HRESULT InvalidateDeviceObjects(DWORD dwType);

	virtual void Render();
//	virtual void Tick();
//	virtual int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	void		SetSpinImage(float fAng);						// �̹��� ȸ�� ����
	void		SetMagnificationImage(float fImgMag);			// �̹��� ���� ����
	void		SetGamma(int nGam);								// �̹��� ���� ����
	
	int			AddImage(char *strImageName, DWORD dwType);		// �̹��� ���� ������ TRUE, ���н� FALSE ����
	int			DeleteImage(char *strImageName);				// �̹��� ���� ������ TRUE, ���н� FALSE ����
	int			DeleteTypeImage(DWORD dwType);
	
	CINFImage*	SerchImageToFileName(char *strImageName);		// ���н� FALSE ������ �̹��� ����
	int			SetGameStateInit( DWORD dwType );


protected:
	map<DWORD, ImageInfo>		m_mapImgBank;					// �̹��� ������ ���� �������

public:
};

#endif

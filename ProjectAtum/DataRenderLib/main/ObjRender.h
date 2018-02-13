// ObjRender.h: interface for the CObjRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJRENDER_H__7E16B046_BEDE_45DA_A99E_D42C15F7EFDF__INCLUDED_)
#define AFX_OBJRENDER_H__7E16B046_BEDE_45DA_A99E_D42C15F7EFDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MeshRender.h"

class CObjectChild;
class CUnitData;
class CSkinnedMesh;
class CObjRender : public CMeshRender  
{
public:
	CObjRender();
	virtual ~CObjRender();

	virtual void	RenderShadow(CAtumNode * pNode);
	virtual void	Render(CObjectChild * pObj);
	virtual void	Tick(float fElapsedTime);	// �Ⱦ�.

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();

	// �浹 �˻�� ���߿� ���� ����.. 
//	COLLISION_RESULT CheckCollMesh(D3DXMATRIX mat,BYTE bColl);
	COLLISION_RESULT CheckCollMesh(D3DXMATRIX mat,D3DXVECTOR3 vPos);
	COLLISION_RESULT CheckCollMeshWaterObject(D3DXMATRIX mat, D3DXVECTOR3 vPos);// 2005-02-11 by jschoi
	COLLISION_RESULT CheckCollMeshWithOutWaterObject(D3DXMATRIX mat, D3DXVECTOR3 vPos); // 2005-02-11 by jschoi
	COLLISION_RESULT CheckCollMesh(D3DXMATRIX mat,D3DXVECTOR3 vPos,float fMovingDistance);
	COLLISION_RESULT CheckCollMesh(D3DXMATRIX *mat,float *size);
	COLLISION_RESULT CheckCollForCamera(D3DXMATRIX *mat,float size);
	COLLISION_RESULT CheckCollMeshRangeObject(D3DXMATRIX mat, D3DXVECTOR3 vPos, float fMovingDistance, CMonsterData* pTarget = NULL);
	CAtumNode * CheckCollMeshEvent(float fAllowedLength);							// 2006-07-14 by ispark ĳ���� ����
	COLLISION_RESULT CheckCollMeshEvent(float fAllowedLength, int nEventType);		// 2005-08-01 by ispark
	CAtumNode *  CheckCollMeshEventOther(D3DXVECTOR3 vDesPos, D3DXVECTOR3 vDesVel, D3DXVECTOR3 vDesUp, float fAllowedLength, int nEventType);	// 2006-07-26 by ispark, Ư�� ��ǥ�� �̺�Ʈ Ÿ������ �̺�Ʈ �˻�
	COLLISION_RESULT CheckPickMesh(D3DXMATRIX mat,D3DXVECTOR3 vPos);
	COLLISION_RESULT CheckPickMesh(float fx, float fy);
	CObjectChild* GetPickMesh(float fx, float fy);
	COLLISION_OBJ_RESULT GetPickObject(float fx, float fy);					// 2006-04-08 by ispark
	CObjectChild* GetPickObjectShopNPC(float fx, float fy);						// 2006-09-14 by ispark
};

#endif // !defined(AFX_OBJRENDER_H__7E16B046_BEDE_45DA_A99E_D42C15F7EFDF__INCLUDED_)

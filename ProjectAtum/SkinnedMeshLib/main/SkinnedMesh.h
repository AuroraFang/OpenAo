// SkinnedMesh.h: interface for the CSkinnedMesh class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "AtumNode.h"
#include "GameDataLast.h"

// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
#include "AtumApplication.h"
//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

template<typename T> void GXRELEASE(T*& _p) { if (_p) {_p->Release(); _p = nullptr;} }

enum METHOD
{
	D3DNONINDEXED,
	D3DINDEXED,
	SOFTWARE,
	D3DINDEXEDVS,
	NONE
};

#define OBJECT_ENCODESTRING "dlwlsgk"
#define MAX_DECAL_NUM		5				// ��Į�� �����Ҽ� �ִ� �ִ��

struct SMeshContainer
{
	ID3DXMesh* pMesh; //    D3DXMESHDATA            MeshData;
	//	LPD3DXMESH pMeshes[5];

	D3DMATERIAL9* rgMaterials; //pMaterials
	LPDIRECT3DTEXTURE9* pTextures; //ppTextures
	DWORD cpattr;
	DWORD cMaterials; //     DWORD                   NumMaterials;
	DWORD iAttrSplit; //iAttributeSW

	SMeshContainer* pmcNext;
	float fRadius; //���� �޽� ������	// 2004-10-29 by jschoi
	D3DXVECTOR3 vCenter; // �߽���ǥ
	D3DXVECTOR3 m_vecMinXYZ;
	D3DXVECTOR3 m_vecMaxXYZ;

	char* szName; //NAME

	// Skin info
	// 2005-01-03 by jschoi
	//    LPD3DXSKINMESH m_pSkinMesh;
	LPD3DXMESH m_pSkinMesh;
	LPD3DXSKININFO m_pSkinMeshInfo; // pSkinInfo

	D3DXATTRIBUTERANGE* m_pAttrTable; // pAttributeTable
	D3DXMATRIX** m_pBoneMatrix; //ppBoneMatrixPtrs
	LPD3DXBUFFER m_pBoneNamesBuf;
	LPD3DXBUFFER m_pBoneOffsetBuf;
	D3DXMATRIX* m_pBoneOffsetMat; //pBoneOffsetMatrices
	DWORD* m_rgiAdjacency; //	*pAdjacency
	DWORD m_numBoneComb;
	DWORD m_maxFaceInfl;
	LPD3DXBUFFER m_pBoneCombinationBuf; // pBoneCombinationBuf
	METHOD m_Method; // NumInfl
	DWORD m_paletteSize; // NumPaletteEntries
	BOOL m_bUseSW; // UseSoftwareVP
	// 2005-01-04 by jschoi
	// SkinMesh info     
	//	LPD3DXMESH           m_pOrigMesh;

	SMeshContainer()
		: pMesh(nullptr),
		  rgMaterials(nullptr),
		  pTextures(nullptr),
		  cpattr(0),
		  cMaterials(0),
		  iAttrSplit(0),
		  pmcNext(nullptr),
		  fRadius(0),
		  vCenter(0, 0, 0),
		  szName(nullptr),
		  m_pSkinMesh(nullptr),
		  m_pSkinMeshInfo(nullptr),
		  m_pAttrTable(nullptr),
		  m_pBoneMatrix(nullptr),
		  m_pBoneNamesBuf(nullptr),
		  m_pBoneOffsetBuf(nullptr),
		  m_pBoneOffsetMat(nullptr),
		  m_rgiAdjacency(nullptr),
		  m_numBoneComb(0),
		  m_maxFaceInfl(0),
		  m_pBoneCombinationBuf(nullptr),
		  m_Method(NONE),
		  m_paletteSize(0),
		  m_bUseSW(FALSE)
	{
	}

	~SMeshContainer()
	{
		delete[]rgMaterials;
		rgMaterials = nullptr;

		if (pTextures)
		{
			for (DWORD i = 0; i < cMaterials; ++i)
			{
				GXRELEASE(pTextures[i]);
			}
			delete[]pTextures;//by dhkwon
			pTextures = nullptr;
		}

		// 2005-02-04 by jschoi - �޸� ���� 
		GXRELEASE(m_pSkinMeshInfo);
		delete[] m_pBoneOffsetMat;
		m_pBoneOffsetMat = nullptr;

		if (pMesh != nullptr)
			GXRELEASE(pMesh);
		//		if(pMeshes)
		//		{
		//			for(DWORD i=0;i<5;i++)
		//				GXRELEASE(pMeshes[i]);
		//		}
		GXRELEASE(m_pSkinMesh);
		GXRELEASE(m_pBoneNamesBuf);
		GXRELEASE(m_pBoneOffsetBuf);
		GXRELEASE(m_pBoneCombinationBuf);

		delete[] m_pBoneMatrix;
		m_pBoneMatrix = nullptr;

		delete[] m_pAttrTable;
		m_pAttrTable = nullptr;

		delete[] szName;
		szName = nullptr;

		delete[] m_rgiAdjacency;
		m_rgiAdjacency = nullptr;

		delete pmcNext;
		pmcNext = nullptr;
	}
};

// X File formation rotate key
struct SRotateKeyXFile
{
	DWORD dwTime;
	DWORD dwFloats;
	float w;
	float x;
	float y;
	float z;
};

struct SScaleKeyXFile
{
	DWORD dwTime;
	DWORD dwFloats;
	D3DXVECTOR3 vScale;
};


struct SPositionKeyXFile
{
	DWORD dwTime;
	DWORD dwFloats;
	D3DXVECTOR3 vPos;
};

struct SMatrixKeyXFile
{
	DWORD dwTime;
	DWORD dwFloats;
	D3DXMATRIX mat;
};

// in memory versions

struct SRotateKey
{
	DWORD dwTime;
	D3DXQUATERNION quatRotate;
};

struct SPositionKey
{
	DWORD dwTime;
	D3DXVECTOR3 vPos;
};

struct SScaleKey
{
	DWORD dwTime;
	D3DXVECTOR3 vScale;
};

struct SMatrixKey
{
	DWORD dwTime;
	D3DXMATRIXA16 mat;
};

struct SFrame
{
	//	D3DXVECTOR3 m_vCurrentPos;
	SMeshContainer* pmcMesh;
	D3DXMATRIXA16 matRot;
	D3DXMATRIXA16 matTrans;
	D3DXMATRIXA16 matRotOrig;
	D3DXMATRIXA16 matCombined;

	vector<D3DXMATRIXA16> instancesMatCombined;

	// animation information
	SPositionKey* m_pPositionKeys;
	UINT m_cPositionKeys;
	SRotateKey* m_pRotateKeys;
	UINT m_cRotateKeys;
	SScaleKey* m_pScaleKeys;
	UINT m_cScaleKeys;
	SMatrixKey* m_pMatrixKeys;
	map<DWORD, DWORD> m_mapMatrixKeys;
	UINT m_cMatrixKeys;

	SFrame* pframeAnimNext;
	SFrame* pframeToAnimate;

	SFrame* pframeSibling;
	SFrame* pframeFirstChild;

	bool bAnimationFrame;
	char* szName;

	SFrame()
		:
		pmcMesh(nullptr),
		m_pPositionKeys(nullptr),
		m_cPositionKeys(0),
		m_pRotateKeys(nullptr),
		m_cRotateKeys(0),
		m_pScaleKeys(nullptr),
		m_cScaleKeys(0),
		m_pMatrixKeys(nullptr),
		m_cMatrixKeys(0),
		pframeAnimNext(nullptr),
		pframeToAnimate(nullptr),
		pframeSibling(nullptr),
		pframeFirstChild(nullptr),
		bAnimationFrame(false),
		szName(nullptr)
	//			m_vCurrentPos(0,0,0)
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixIdentity(&matRotOrig);
		D3DXMatrixIdentity(&matTrans);
	}

	~SFrame()
	{
		delete[]szName;
		szName = nullptr;
		delete pmcMesh;
		pmcMesh = nullptr;
		delete pframeFirstChild;
		pframeFirstChild = nullptr;
		delete pframeSibling;
		pframeSibling = nullptr;

		delete[]m_pPositionKeys;
		m_pPositionKeys = nullptr;
		delete[]m_pRotateKeys;
		m_pRotateKeys = nullptr;
		delete[]m_pScaleKeys;
		m_pScaleKeys = nullptr;
		delete[]m_pMatrixKeys;
		m_pMatrixKeys = nullptr;

		// do NOT delete pframeAnimNext
		// do NOT delete pframeToAnimate
	}

	void SetTime(float fTime);

	SFrame* FindFrame(const char* szFrame)
	{
		SFrame* pframe;

		if ((szName != nullptr) && (strcmp(szName, szFrame) == 0))
			return this;

		if (pframeFirstChild != nullptr)
		{
			pframe = pframeFirstChild->FindFrame(szFrame);
			if (pframe != nullptr)
				return pframe;
		}

		if (pframeSibling != nullptr)
		{
			pframe = pframeSibling->FindFrame(szFrame);
			if (pframe != nullptr)
				return pframe;
		}

		return nullptr;
	}

	void ResetMatrix()
	{
		matRot = matRotOrig;
		D3DXMatrixIdentity(&matTrans);

		if (pframeFirstChild != nullptr) pframeFirstChild->ResetMatrix();

		if (pframeSibling != nullptr) pframeSibling->ResetMatrix();
	}

	void AddFrame(SFrame* pframe)
	{
		if (pframeFirstChild == nullptr) pframeFirstChild = pframe;
		else
		{
			pframe->pframeSibling = pframeFirstChild->pframeSibling;
			pframeFirstChild->pframeSibling = pframe;
		}
	}

	void AddMesh(SMeshContainer* pmc)
	{
		pmc->pmcNext = pmcMesh;
		pmcMesh = pmc;
	}
};

struct SDrawElement
{
	SFrame* pframeRoot;

	D3DXVECTOR3 vCenter;
	float fRadius;

	// name of element for selection purposes
	char* szName;

	// animation list
	SFrame* pframeAnimHead;

	// next element in list
	SDrawElement* pdeNext;

	float fCurTime;
	float fMaxTime;

	SDrawElement()
		:
		pframeRoot(nullptr),
		vCenter(0.0, 0.0, 0.0),
		fRadius(1.0),
		szName(nullptr),
		pframeAnimHead(nullptr),
		pdeNext(nullptr),
		fCurTime{0.0f},
		fMaxTime{0.0f}
	{
	}

	~SDrawElement()
	{
		delete pframeRoot;
		pframeRoot = nullptr;
		delete pdeNext;
		pdeNext = nullptr;
		delete[] szName;
		szName = nullptr;

		// do NOT delete pframeAnimHead;
	}

	void AddAnimationFrame(SFrame* pframeAnim)
	{
		pframeAnim->pframeAnimNext = pframeAnimHead;
		pframeAnimHead = pframeAnim;
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	SFrame* FindFrame(const char* szName)
	{
		if (pframeRoot == nullptr) return nullptr;

		return pframeRoot->FindFrame(szName);
	}
};

#define MAX_INTERSECTIONS 16

struct D3DVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	FLOAT tu, tv;
};

struct INTERSECTION
{
	DWORD dwFace; // mesh face that was intersected
	FLOAT fBary1, fBary2; // barycentric coords of intersection
	FLOAT fDist; // distance from ray origin to intersection
	FLOAT tu, tv; // texture coords of intersection
};

struct COLLISION_RESULT
{
	int nCollType;
	float fDist;
	D3DXVECTOR3 vNormalVector;
	D3DXVECTOR3 vPicking;

	COLLISION_RESULT()
	{
		nCollType = COLL_NONE;
		fDist = DEFAULT_COLLISION_DISTANCE ;
	}
};

// 2006-04-08 by ispark
struct COLLISION_OBJ_RESULT
{
	COLLISION_RESULT stCollision;
	CObjectChild* pObject;

	COLLISION_OBJ_RESULT()
	{
		pObject = nullptr;
	}
};

class CSkinnedMesh : public CAtumNode
{
public:
	CSkinnedMesh();
	explicit CSkinnedMesh(bool bProgressiveMesh);
	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	//- �����ڸ� �Լ��� ���� ����
	void InitSkinnedMesh();

	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����

	float GetRadius() const { return m_fRadius; }

	~CSkinnedMesh() override;
	HRESULT InitDeviceObjects() override;
	HRESULT RestoreDeviceObjects() override;
	HRESULT InvalidateDeviceObjects() override;
	HRESULT DeleteDeviceObjects() override;
	virtual void Render(BOOL bMultiTexture = FALSE, DWORD nType = 0);

	virtual void Tick(float fElapsedTime);

	COLLISION_RESULT CheckCollDistDetail(SMeshContainer* pmcMesh, const D3DXMATRIX& mat, BOOL bWithNormal = FALSE);
	COLLISION_RESULT CheckCollDist(SFrame* pframeCur, D3DXMATRIX mat, D3DXVECTOR3 vPos, float fCheckDistance, BOOL bWithNormal = FALSE);
	COLLISION_RESULT CheckCollision(D3DXMATRIX mat, D3DXVECTOR3 vPos, float fCheckDistance = DEFAULT_COLLISION_DISTANCE, BOOL bUpdateFrame = FALSE, BOOL bWithNormal = FALSE);
	COLLISION_RESULT CheckCollDist(SFrame* pframeCur, float fx, float fy); // 2005-07-15 by ispark
	COLLISION_RESULT CheckCollision(float fx, float fy, BOOL bUpdateFrame); // Picking

	void SetMeshRenderState(SMeshContainer* pmcMesh);

	// HRESULT LoadMeshHierarchy(char* szFileName); // DevX Unused
	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	//HRESULT LoadMeshHierarchyFromMem(CGameData* pGameData);
	HRESULT LoadMeshHierarchyFromMem(CGameData* pGameData, int nMeshIndex = 0, D3DXVECTOR3 vObjScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f)); // 2012-05-21 by isshin ���� ���� ������Ʈ ������ ����

	void SetWorldMatrix(D3DXMATRIX mat)
	{
		m_mWorld = mat;
	}

	HRESULT LoadTexture(char* szfileName, BYTE tex_num);

	void AnotherTexture(BYTE tex_num)
	{
		m_bTextureNum = tex_num;
	}

	void BaseTexture()
	{
		m_bTextureNum = 0;
	}

	LPDIRECT3DTEXTURE9 SetTexture(LPDIRECT3DTEXTURE9 pTexture, BYTE tex_num);// by dhkwon, 030917

	// 2005-07-14 by ispark
	COLLISION_RESULT Pick(SMeshContainer* pmcMesh, float fx, float fy);

	void SetDecalCount(int nCount)
	{
		m_nDecalCount = nCount;
	}

	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	LPDIRECT3DTEXTURE9 GetShadowTex();

	// 2007-11-08 by bhsohn �κ� ����Ʈ ���� ó��	
	void UpdateMeshObjName(char* pDstPath);
	char* GetMeshObjFileName();

	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	HRESULT LoadMeshHierarchyFromMemLoadStart(structLoadingGameData* pGameData, int nMeshIndex = 0);
	HRESULT LoadMeshHierarchyFromMemLoadStep1(structLoadingGameData* pGameData, int nMeshIndex = 0);
	HRESULT LoadMeshHierarchyFromMemLoadStep2();
	HRESULT LoadMeshHierarchyFromMemLoadStep3();
	HRESULT LoadMeshHierarchyFromMemLoadStep4(structLoadingGameData* pGameData);
	HRESULT LoadMeshHierarchyFromMemLoadStep5();
	HRESULT LoadMeshHierarchyFromMemLoadStep6();

	BOOL GetIsLoadingFlag() const
	{
		return m_bIsLoadingFlag;
	}

	void SetIsLoadingFlag(BOOL flag)
	{
		m_bIsLoadingFlag = flag;
	}

	void ReleaseData(HRESULT hr);
	void DeleteLoadingGameData();
	BOOL TimeOut();
	void SetIsLoadingStartTime();
	int GetIsLoadingStartTime() const;
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
public: // protected:
	HRESULT LoadFrames(LPD3DXFILEDATA pxofobjCur, SDrawElement* pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame* pframeParent);
	HRESULT LoadMesh(LPD3DXFILEDATA pxofobjCur, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame* pframeParent);
	HRESULT FindBones(SFrame* pframeCur, SDrawElement* pde);
	HRESULT DeleteSelectedMesh();
	HRESULT LoadAnimationSet(LPD3DXFILEDATA pxofobjCur, SDrawElement* pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame* pframeParent);
	HRESULT LoadAnimation(LPD3DXFILEDATA pxofobjCur, SDrawElement* pde, DWORD options, DWORD fvf, LPDIRECT3DDEVICE9 pD3DDevice, SFrame* pframeParent);
	HRESULT GenerateMesh(SMeshContainer* pmcMesh);
	HRESULT UpdateFrames(SFrame* pframeCur, D3DXMATRIX const& matCur);
	HRESULT UpdateFrames(SFrame* pframeCur, D3DXMATRIX& matCur, D3DXVECTOR3 vPos, float fCheckDistance);
	HRESULT DrawFrames(SFrame* pframeCur, UINT& cTriangles, DWORD nType = 0);

	HRESULT UpdateFrames2(SFrame* pframeCur, const D3DXMATRIX* matCur) = delete;
	HRESULT DrawFrames2(SFrame* pframeCur) = delete;

	HRESULT DrawInstancedGeometry(SDrawElement* pdeCur, SFrame* pframeCur, const D3DXMATRIX* matCur, const vector<tuple<D3DXMATRIXA16, float>>& Instances) = delete;

	HRESULT DrawMeshContainer(SMeshContainer* pmcMesh, DWORD nType = 0);
	HRESULT SetProjectionMatrix() const;
	void ReleaseDeviceDependentMeshes(SFrame* pframe);

	// 2005-03-11 by jschoi
	static void ChangeFilePath(char* strFilePath);
	HRESULT SetResourceFile(char* szFileName);
	DataHeader* FindResource(char* szRcName);

	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	static BOOL IsArmorMesh(int nMeshIndex);
	static BOOL IsLowQuality();
	BOOL IsLoadTexture(int nTexture) const;

protected:
	SDrawElement* m_pdeSelected;
	SFrame* m_pframeSelected;
	CGameData* m_pGameDataToDecal;

	DWORD m_dwFVF;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPD3DXMATRIXA16 m_pBoneMatrices;
	DWORD m_dwIndexedVertexShader[4];
	DWORD m_maxBones;
	METHOD m_method;
	D3DCAPS9 m_d3dCaps;
	D3DXMATRIX m_mWorld;

	DataHeader* m_pDataHeader;
public:
	float m_fRadius;
	D3DXVECTOR3 m_vCenter;
	BYTE m_bTextureNum;
	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	//CGameData *				m_pGameData;
	//LPDIRECT3DTEXTURE9	*	m_pTexture;	
	SMeshContainer* m_pmcSelectedMesh;
	SDrawElement* m_pdeHead;
	INT m_nRenderCount; // �������� ���� Ƚ��(�����ð����� ���󵵼��� ������ �޸𸮿��� ����)
	BOOL m_bMaterial; // Material �� �����Ұ��̳�?
	D3DMATERIAL9 m_material; // Material
	BOOL m_bCheckBlend; // ������ üũ�Ұ��̳�?
	FLOAT m_fCollDist; // �浹 ���� �Ÿ�
	bool m_bProgressiveMesh; // �޽��� ������ ������ ���ϰ��̳�?
	BYTE m_bTotalTextureNum;
	BOOL m_bMultiTexture;
	LPDIRECT3DTEXTURE9 m_pOrderTexture; // ������ �������Ͽ� �׸���
	INTERSECTION m_Intersection; // �浹 ó����

	int m_nSelectDecalNum[MAX_DECAL_NUM ];
	int m_nDecalCount;
	//	LPDIRECT3DTEXTURE9		*m_pDecalTextures;	//ppTextures (Decal)
	UINT m_unTexColor;
	UINT m_unTexSelectColor;
	UINT m_unUnitTexOld;

	inline const D3DXMATRIX& getWorldMatrix() const
	{
		return m_mWorld;
	}

private:
	// 2007-08-29 by bhsohn �ּ� �����ӽ� �⺻ �ƸӸ� �ε��ϰԲ� ����
	CGameData* m_pGameData;
	LPDIRECT3DTEXTURE9* m_pTexture;
	BOOL* m_pLoadMem;

	// 2007-11-08 by bhsohn �κ� ����Ʈ ���� ó��
	char m_szMeshObjFileName[256];

	// 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
	BOOL m_bIsLoadingFlag;
	LPD3DXFILE pxofapi;
	LPD3DXFILEENUMOBJECT pxofenum;
	SDrawElement* pdeMesh;
	D3DXF_FILELOADMEMORY FileLoadMemory;
	DWORD dwOptions;
	LPD3DXFILEDATA pxofobjCur;
	DataHeader* pHeader;
	int m_StartTime;
	//end 2009. 11. 23 by jskim ���ҽ� �ε� ���� ����
};

HRESULT CalculateRadius(SFrame* pframe, D3DXMATRIX* pmatCur, D3DXVECTOR3* pvCenter, float* pfRadiusSq, D3DXVECTOR3 vObjScaleSphereRadius = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
HRESULT CalculateSum(SFrame* pframe, D3DXMATRIX* pmatCur, D3DXVECTOR3* pvCenter, UINT* pcVertices, D3DXVECTOR3 vObjScaleSphereSum = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
HRESULT CalculateBoundingSphere(SDrawElement* pdeCur, D3DXVECTOR3 vObjScaleSphere = D3DXVECTOR3(1.0f, 1.0f, 1.0f));


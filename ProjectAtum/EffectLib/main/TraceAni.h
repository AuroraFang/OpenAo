#pragma once

// TraceAni.h: interface for the CTraceAni class.
//////////////////////////////////////////////////////////////////////

#include "Effect.h"

#define NUMBER_OF_EFFECT_PLANE		1//(m_nNumberOfCross*4)
#define PLANE_VB_SIZE				16//�ּ� ������ : 8

struct TraceData : public Effect
{
	char		m_strName[20];				//�̸�
	UINT		m_nNumberOfTrace;			// ������ �ܻ�ڽ��� ���� 
	float		m_fCreateTick;				//���� ����(�������� ������ �ܻ��� �����ȴ�.)
	//D3DXVECTOR3 m_vCreatePos;				//���� ��ǥ(�����ǥ) : CEffectInfo�� �Ѿ
	float		m_fHalfSize;				// �ܻ��� ���� ������ ����
	UINT		m_nTextureNumber;			// �ؽ��� ����
	float		m_fTextureAnimationTime;	// �ؽ��� �ִϸ��̼� Ÿ��
	char		m_strTextureName[20][20];		// �ؽ��Ĵ� �ִ� 20���� ���� �ִ�.
	int			m_nNumberOfCross;				// ũ�ν� ����(1 ~ 4)
	
	// todo : m_bAlphaBlendEnable >> 16 is the alpha value for shader (temporary)
	// if alpha value is 0, the default 0.8 is used
	BOOL		m_bAlphaBlendEnable; 

	DWORD		m_dwSrcBlend;
	DWORD		m_dwDestBlend;
	DWORD		m_nTextureRenderState;
	BOOL		m_bZbufferEnable;
	BOOL		m_bZWriteEnable;
	
};

class CTraceAni;

class CEffectPlane : public Effect
{
	friend CTraceAni;
public:
	CEffectPlane(CTraceAni*	pParent, int nIBIndex);
	~CEffectPlane();

//	D3DXVECTOR3 &GetReferencePos() { return m_vPos; }
//	TRACEANI_INDEX &GetReferenceTraceAniIndex() { return m_TraceAniIndex; }

	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void SetFirstIndex(bool bSet);
	void SetEndIndexOriginal();
	void SetEndIndex();
	void SetEndIndex2();
	const D3DXVECTOR3& GetPos() const { return m_vPos; }

public:
	CTraceAni				*m_pParent;				// �θ� ��ü
protected:
	D3DXVECTOR3				m_vPos ;				// ������ǥ�� ����� ������ǥ(ZEnable�ÿ� �Ÿ���꿡 ���), parent�� m_vPos����
	int						m_nIBIndex;				// 0 ~ 15 ���� ��
	//LPDIRECT3DVERTEXBUFFER9 m_pVB;					// ���ؽ�����(4*32)
	BOOL					m_bIsFirstIndex;
	BOOL					m_bZbufferEnable;
	
public:
	TRACE_INSTANCE			m_TraceInstance;
};

class CEffectInfo;
class CEffectPlane;

class CTraceAni : public TraceData
{
public:
	float					m_fCurrentCreateTick;		// ������� �ð�
	int						m_nCurrentNumberOfTrace;	// ������� ������ �ܻ󰳼�(MAX=m_nNumberOfTrace)
	int						m_nCurrentTextureNumber;	// ���� ���õ� �ؽ��� ��ȣ
//	LPDIRECT3DVERTEXBUFFER8 m_pVB;					// ���ؽ�����
	float					m_fCurrentTextureTick;		// �ؽ��� �ִϸ��̼� ���ð�
	BOOL					m_bCreateTrace;				// �� ���� FALSE�̸� ���̻� �ܻ��� ������ �ʴ´�.
	CEffectInfo				*m_pParent;					// ����Ʈ ���� ����
	CEffectPlane			**m_pEffectPlane;		// CEffectPlane ����(16 X m_nNumberOfTrace)
//	LPDIRECT3DINDEXBUFFER8	*m_pIB;					// �ε��� ����, 16 X m_nNumberOfTrace, CEffectPlane�� 1:1��ġ
	int						m_nCurrentCreateIndex;	// ���� ������ VB,Plane �ε���(0 ~ m_nNumberOfTrace)
	int						m_nCurrentRenderIndex;	// ���� �������� ���۵Ǵ� �ε���
	int						m_nCurrentDeleteIndex;	// ������ ��� �������� �ε���
	D3DXVECTOR3				m_vOldPos;
	D3DXVECTOR3				m_vCurrentPos;
	BOOL					m_bRestored;

	float					m_fRadius;
public:
	CTraceAni();
	~CTraceAni();
	BOOL Tick(float fElapsedTime);
	void Render();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();

	bool EnableAlphaBlend() const { return (m_bAlphaBlendEnable & 0xFF) != 0; }
	unsigned char GetAlphaValueByte() const { return (m_bAlphaBlendEnable >> 8) & 0xFF; }
	float GetAlphaValue() const { return GetAlphaValueByte() > 0 ? float(GetAlphaValueByte()) / 255.0f : 0.2f; }

	void TraceEnable(bool bEnable) { m_bCreateTrace = bEnable; }
	//void SetTexture(char* strTexture) {};
	//void SetTexture(LPDIRECT3DTEXTURE9 pTexture) {};
};
// INFStageEffectData.h: interface for the CINFStageEffectData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFSTAGEEFFECTDATA_H__B3508266_078E_4281_B0B6_F9229AEF0B8E__INCLUDED_)
#define AFX_INFSTAGEEFFECTDATA_H__B3508266_078E_4281_B0B6_F9229AEF0B8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <D3dUtil.h>
//#include <d3d8.h>
#include "RangeTime.h"
//#include "INFImage.h"

//class CRangeTime;
class CINFImage;
class CINFImageEx;									  // 2011. 10. 10 by jskim UI�ý��� ����
class CGameData;

#define			FILENAME_LEGHT		16
#define			NARRATIONSOUND		5001
#define			BGMSOUNT			5002

//struct STAGE_EFFECT
//{
//	char	szImageFile[16];
//	char	szSoundFile[16];
//	float	fSoundPlayTime;
//	float	fShowTime;		// image�� ��Ÿ���� �����ϴ� �ð�
//	UINT	nQuestNumber;
//};
struct SCENE_EFFECT
{
	int			num;									//Number.
	char		szImageFileName[FILENAME_LEGHT];		//�̹��� ���ϸ�.
	float		fImageReady;							//�̹��� ��� �ð�.
	float		fScenePlay;								//�� Scene�̺������� �ð�.
};

struct STAGE_EFFECT
{
	int		nSceneCount;								//�� ���� ����.
	int		nStageTime;									// stage time
	char	szSoundNarration[FILENAME_LEGHT];			//�����̼� ���ϸ�.
	char    szSoundBGM[FILENAME_LEGHT];					//������� ���ϸ�.
	UINT	nQuestNumber;
};

class CINFStageEffectData  
{
public:
	CINFStageEffectData();
	virtual ~CINFStageEffectData();

	virtual HRESULT InitDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT DeleteDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	BOOL Tick(float fElapsedTime);
	virtual void Render();
	int WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Reset();
	HRESULT LoadData( char* i_szFileName );
	void SetData( STAGE_EFFECT_DATA * pStageEffectData, BYTE byEndAct, BOOL bSkip );
	void SetOldGameState(DWORD dwState) { m_dwOldGameState = dwState; }
	DWORD GetOldGameState() { return m_dwOldGameState; }
	BOOL ResetScene();

	BYTE GetEndAct() { return m_byEndAct; }
	BOOL GetStageEffectSkip() { return m_bSkip; }
	STAGE_EFFECT_DATA * GetStageEffectData() { return &m_StageEffectData; } 

protected:
	int					m_nSceneIndex;				//���� �ε��� ��ȣ.
	std::vector<SCENE_EFFECT>	m_vecScene;			//������ ����.

	STAGE_EFFECT		m_stageEffect;
	CGameData*			m_pGameData;
	CINFImageEx*		m_pImage;					// 2011. 10. 10 by jskim UI�ý��� ����
	DWORD				m_dwCurrentColor;
	float				m_fCurrentPlayTime;			// fSoundPlayTime�� �Ǹ� Image�� ������ �����
	float				m_fCurrentReadyTime;		// fShowTime ���� ����ؼ� 0�̵Ǹ� ������ ���̱� ������
	float				m_fCurrentAllTime;			// ��ü �ð�.
//	float				m_fCurrentSoundPlayTime;	// fSoundPlayTime�� �Ǹ� Image�� ������ �����
//	float				m_fCurrentImageShowTime;	// fShowTime ���� ����ؼ� 0�̵Ǹ� ������ ���̱� ������
	CRangeTime			m_tBlinkTime;
	DWORD				m_dwOldGameState;

	STAGE_EFFECT_DATA	m_StageEffectData;
	BYTE				m_byEndAct; 
	BOOL				m_bSkip;
	BOOL				m_bSkipEnd;
};

#endif // !defined(AFX_INFSTAGEEFFECTDATA_H__B3508266_078E_4281_B0B6_F9229AEF0B8E__INCLUDED_)

#pragma once

// AtumSound.h: interface for the CAtumSound class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ATUMSOUND_H
#define ATUMSOUND_H

#include <dsound.h>

#define MAX_SOUND_FILE_NAME_SIZE			64
#define COUNT_3DSOUND_PLAY_MULTI_BUFFER		16


class CMusicMP3Ex;

class CSound;

using vectorSoundPtr = vector<CSound*>;

class CSoundManager;

class CAtumSound final
{
public:
	CAtumSound();
	~CAtumSound() { util::del(m_pGameData); }

	HRESULT	InitDeviceObjects();
	HRESULT	DeleteDeviceObjects();
	void	Tick();	
	
	VOID	PlayD3DSound(int nType,D3DXVECTOR3 vPos, BOOL i_b3DSound=TRUE);
	VOID	PlayD3DSound(char * str,D3DXVECTOR3 vPos, BOOL i_b3DSound=TRUE);
	VOID	PlayD3DSound(int nType, char* pBuffer, D3DXVECTOR3 vPos, BOOL i_b3DSound=TRUE);
	VOID	StopD3DSound(int nType);
	VOID	StopD3DSound(char * str);
	VOID	CheckD3DSoundGround();
	VOID	PlayBackSound();
	VOID	SetBackMusic(DWORD dwType);
	VOID	SetAtumMusicVolume(int nVolume) const;

	// 2007-07-24 by bhsohn �����̼� mp3�߰�
	// 2009. 01. 22 by ckPark �� ���º� ���۷����� MP3 �и�
	//VOID	PlayNarrationSound(char* pNarration);
	BOOL	PlayNarrationSound(char* pNarration);
	void	EndNarrationSound();

	CSound * FindSCSoundWithSoundType(int nType) const;
	HRESULT	 SetResourceFile(char* szFileName);
	// WAVEFORMATEX�� ã�Ƽ� �V���ϰ� , ���� ����Ÿ�� ��ġ�� ���̸� ����
	PBYTE SetWaveFormatEx(BYTE* pbData, DWORD& dwWaveFromLen, WAVEFORMATEX* pwfx );
 	void SetDefaultMusicLoop(bool bFlag) const;

	// 2006-09-07 by ispark
	void	DelD3DSound(int nType);
	void	DelSCSoundWithSoundType(int nType);

	// 2013-02-05 by bhsohn Sound ���� ��񿡼� Exception���� ���� ���� ����
	bool IsSoundDevice() const { return m_bNotDevice; }

public:
	BOOL						m_bPlayMusic;

	CMusicMP3Ex* GetMusicMp3() const { return  m_pMusic; }

protected:
	CMusicMP3Ex		*			m_pMusic;	
	CMusicMP3Ex		*			m_pNarrationMusic;	

	BOOL						m_bPlayNarrationMusic;
	char 						m_strNarrationMusic[MAX_SOUND_FILE_NAME_SIZE];			// ��� ���� �̸�

	FLOAT						m_fSetSoundGroundCheckTime;		// Ground �� ��� ���� ���� üũ Ÿ��
	char 						m_strBackMusic[MAX_SOUND_FILE_NAME_SIZE];			// ��� ���� �̸�

	vectorSoundPtr				m_vectorSoundPtr;
	CSoundManager				*m_pSoundManager;
	CSoundManager				*m_p3DSoundManager;
	LPDIRECTSOUND3DLISTENER		m_pDSListener;				   // 3D listener object
	DS3DLISTENER				m_dsListenerParams;             // Listener properties

	CGameData					*m_pGameData;

	int							m_nMusicVolume;
	int							m_nOnlyBackMusicState;
	BOOL						m_bOnlyBackMusic;

	BOOL						m_bNotDevice;
};



#endif

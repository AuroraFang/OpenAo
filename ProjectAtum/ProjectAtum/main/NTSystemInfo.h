// 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����

//######################################################################################
// File    : NTSystemInfo.h
// Date    : 2007-1-8
// Author  : ��(goli81@naver.com)
// Blog    : http://blog.naver.com/goli81
// Desc    : System ���� ���
// Bug     : 
//######################################################################################
#ifndef __NTSYSTEM_INFO__
#define __NTSYSTEM_INFO__

#include <dxdiag.h>
#include <string>

#ifdef  UNICODE                     // r_winnt
typedef std::wstring TString;
#else
typedef std::string TString;
#endif//(UNICODE)


class CNTSystemInfo
{
public:
	CNTSystemInfo();
	~CNTSystemInfo();

	bool Initialize();

	const TCHAR* GetCpuString(void)			{ return m_wszCpuString.c_str(); }
	const TCHAR* GetVideoCardString(void)	{ return m_wsVideoCardName.c_str(); }
	int   GetVideoPhysicMemory(void)		{ return m_nVideoPhysicMemory; }
	const TCHAR* GetDxVersionString(void);
	const TCHAR* GetWindowVersionString(void){ return m_strWinVersion.c_str(); }

private:
	void FreeIDxDiagContainer(void);
	void GetPhysicalMemoryInMB(void);
	bool GetProperty( IDxDiagContainer * pContainer, const TCHAR* property_name, TString* out_value );
	bool GetProperty( const TCHAR* container_name0, const TCHAR* property_name, TString* out_value );
	bool GetChildContainer( const TCHAR* name0, IDxDiagContainer ** ppChild );
	bool GetDirectXVersion( DWORD * pdwDirectXVersionMajor, DWORD* pdwDirectXVersionMinor,TCHAR* pcDirectXVersionLetter );
	bool GetDisplayDeviceDescription( DWORD dwDevice, TString* pwstrName );
	bool GetDisplayDeviceMemoryInMB( DWORD dwDevice, int* pDisplayMemory );
	bool GetDisplayDeviceProp( DWORD dwDevice, const TCHAR* prop_name, TString* pwstrProp );
	bool GetDisplayDeviceNode( DWORD dwDeviceIndex, IDxDiagContainer ** ppNode );
	bool GetChildByIndex(  IDxDiagContainer* pParent, DWORD dwIndex,IDxDiagContainer** ppChild );
	TString WStringToString( const TString* in_pwstring );
	bool ConverteCHAR2WCHAR( WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar );
	bool ConverWCHAR2CHAR( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar );
	void CPUInfo(void);

	bool				m_bCleanupCOM;
	IDxDiagProvider*	m_pDxDiagProvider;
	IDxDiagContainer*	m_pDxDiagRoot;

	TString				m_wszCpuString;
	TString				m_wsVideoCardName;
	TString				m_wsDXVersion;
	TString				m_strWinVersion;
	float				m_fSystemPhysicMemory;
	int					m_nVideoPhysicMemory;
	DWORD				m_dwDXVersionMajor;
	DWORD				m_dwDXVersionMinor;
	TCHAR				m_cDXVersionLetter;
};


#endif //(__NTSYSTEM_INFO__)

// end 2011-08-17 by hsson Ŭ���̾�Ʈ ũ���� ���� ������ Ŭ�� ���� ����
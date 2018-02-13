// FunctionLog.h: interface for the CFunctionLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTIONLOG_H__28BDAB15_95CE_45C6_9F6B_CA1B61F87375__INCLUDED_)
#define AFX_FUNCTIONLOG_H__28BDAB15_95CE_45C6_9F6B_CA1B61F87375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _FUNCTION_LOG

#define FLOG(name)				CFunctionLog fLog(name)
#define SIZE_OF_LOG				256
#define MAX_FUNCTION_LOG_LINE	200

///////////////////////////////////////////////////////////////////////////////
/// \class		CFunctionLog
///
/// \brief		�Լ��� ���԰� ��Ż�� �α׷� ����, ����뿡 �̿��Ѵ�.
/// \author		dhkwon
/// \version	
/// \date		2004-03-12 ~ 2004-03-12
/// \warning	�Լ� ���Խÿ��� ǥ�ø� �ϸ� �Լ� ��Ż�ÿ��� �ڵ����� ��Ż�ڵ带 �����.
///////////////////////////////////////////////////////////////////////////////
class CFunctionLog
{
public:
	CFunctionLog(char* szName);
	virtual ~CFunctionLog();
protected:
	char m_szFunctionLog[SIZE_OF_LOG];
	char *m_szFunctionName;
};

#else
#define FLOG(name)			__noop
#endif // _FUNCTION_LOG_endif

#endif // !defined(AFX_FUNCTIONLOG_H__28BDAB15_95CE_45C5_9F6B_CA1B61F87375__INCLUDED_)

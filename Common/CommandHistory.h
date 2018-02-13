// CommandHistory.h: interface for the CCommandHistory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDHISTORY_H__E17CC8A8_4994_4A53_BB1B_0EF3A49BB3DA__INCLUDED_)
#define AFX_COMMANDHISTORY_H__E17CC8A8_4994_4A53_BB1B_0EF3A49BB3DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

using namespace std;

#define COMMAND_LINE_BUFF 512

class CCommandHistory
{
public:
	CCommandHistory(int size, BOOL bDuplicate = TRUE, BOOL bSort = FALSE, const char* historyFileName = "", const char *filePath = "");
	virtual ~CCommandHistory();

	BOOL LoadHistory();
	BOOL WriteHistory();

	int size();

	void Sort();

	void ResetPosition();
	int GetPosition();					// �ڷκ��� n�� °, 1���� �����ϱ�
	BOOL SetPosition(int posFromLast);	// �ڷκ��� n�� °, 1���� �����ϱ�

	char* GetCurrent();
	char* GetNext();
	char* GetPrev();
	char* GetOldest();
	char* GetLastest();
	char* GetAtPosFromLast(int i_nPosFromLast);		// �ڷκ��� n�� °, 1���� �����ϱ�
	char* GetAtPos(int i_nPos);						// 0���� �����ϱ�

	BOOL AddCommand(const char* szCmd);
	void Clear();

	void PrintHistory();
	void PrintHistory(int size);

public:
	list<string>	m_listHistory;
	string			m_szHistoryFileName;	// "" �̸� �������� �ʴ´�
	string			m_szHistoryFilePath;	// "" �̸� ���� ���͸�
	int				m_nSize;				// capacity
	BOOL			m_bDuplicate;			// �ߺ��� ����� �� ����
	BOOL			m_bSort;				// ������ �� ����
	int				m_nHistPos;
};

#endif // !defined(AFX_COMMANDHISTORY_H__E17CC8A8_4994_4A53_BB1B_0EF3A49BB3DA__INCLUDED_)

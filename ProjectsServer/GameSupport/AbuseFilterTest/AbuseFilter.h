// AbuseFilter.h: interface for the CAbuseFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABUSEFILTER_H__E339C3B9_BDF6_4E66_ACE5_00FF9FA43D60__INCLUDED_)
#define AFX_ABUSEFILTER_H__E339C3B9_BDF6_4E66_ACE5_00FF9FA43D60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int MAX_LENGTH_STRING					= 127;				// �弳�� �ִ� ����, �弳�� ������ ��Ʈ���� �ִ� ����


typedef struct WordNode
{
	BOOL	bTermination;				// ���� TRUE �̸� ��Ʈ�� ���� ��Ÿ��
	struct	WordNode *pNext[256];		// ���� TRUE �̸� �弳��Ʈ�� ����

	~WordNode()
	{
		for(int i=0; i < 256; i++)
		{
			if(pNext[i])
			{
				delete pNext[i];
				pNext[i] = NULL;
			}
		}
	}
} WORDNODE, *LPWORDNODE;

class CWordSearchTree
{
public:
	CWordSearchTree();
	~CWordSearchTree();

	void InitWordSearchTree(void);
	BOOL AddWord(char* szWord);	
	int	AbuseFilter(char *szWord);
	int RecursiveSearchString(LPWORDNODE pWordNode, char *pStpInWord, int nWordLength);

protected:
	LPWORDNODE		m_pRootWordNode;
	int				m_nSearchWordCount;
	int				m_nAllocatedBytes;
};

class CAbuseFilter  
{
public:
	CAbuseFilter();
	virtual ~CAbuseFilter();

	BOOL AddAbuse(char *szAbuse);
	BOOL UpperString(char *szSource, char *szDest);
	BOOL AbuseFiltering(char *szChat, int *nFTWordCounts, int *nFTTotalBytes);
	
protected:
	CWordSearchTree		m_WordSearchTree;
};

#endif // !defined(AFX_ABUSEFILTER_H__E339C3B9_BDF6_4E66_ACE5_00FF9FA43D60__INCLUDED_)

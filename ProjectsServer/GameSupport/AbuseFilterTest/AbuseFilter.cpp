// AbuseFilter.cpp: implementation of the CAbuseFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbuseFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbuseFilter::CAbuseFilter()
{	
}

CAbuseFilter::~CAbuseFilter()
{

}

BOOL CAbuseFilter::AddAbuse(char *szAbuse)
{
	if(strcmp(szAbuse, "") == 0
		|| strlen(szAbuse) > MAX_LENGTH_STRING)
	{
		return FALSE;
	}
	
	char	szCopyAbuse[MAX_LENGTH_STRING+1];
	memset(szCopyAbuse, 0x00, MAX_LENGTH_STRING+1);
	UpperString(szAbuse, szCopyAbuse);
	return m_WordSearchTree.AddWord(szCopyAbuse);
}

BOOL CAbuseFilter::UpperString(char *szSource, char *szDest)
{
	int nLenCh = strlen(szSource);
	if(0 == nLenCh){	return FALSE;}

	int nChIdx = 0;
	while(nChIdx < nLenCh)
	{
		szDest[nChIdx] = toupper(szSource[nChIdx]);
		nChIdx++;
	}
	return TRUE;
}


BOOL CAbuseFilter::AbuseFiltering(char *szChat, int *nFTWordCounts, int *nFTTotalBytes)
{
	if(strcmp(szChat, "") == 0
		|| strlen(szChat) > MAX_LENGTH_STRING)
	{
		return FALSE;
	}

	*nFTWordCounts = 0;
	*nFTTotalBytes = 0;
	
	char szCopyChat[MAX_LENGTH_STRING+1];
	memset(szCopyChat, 0x00, MAX_LENGTH_STRING+1);
	UpperString(szChat, szCopyChat);

	int nLenChat = strlen(szChat);
	int nChatIdx = 0;
	while(nChatIdx < nLenChat)
	{
		if(szCopyChat[nChatIdx] == ' ')
		{
			nChatIdx++;
			continue;
		}

		int nAbuseBytes = m_WordSearchTree.AbuseFilter(&szCopyChat[nChatIdx]);
		if(nAbuseBytes)
		{
			char szT[128];
			memset(szT, 0x00, 128);
			memcpy(szT, &szCopyChat[nChatIdx], nAbuseBytes);
			
			memset(&szChat[nChatIdx], 0x2A, nAbuseBytes);
			nChatIdx += nAbuseBytes;

			(*nFTWordCounts)++;
			(*nFTTotalBytes) += nAbuseBytes;

			DBGOUT("�弳 [%10s]�� ã��, Counts[%2d], Bytest[%3d]\n", szT, *nFTWordCounts, *nFTTotalBytes);
		}
		else
		{
			if((szCopyChat[nChatIdx] & 0x80) == 0)
			{	// �����̸� (�ѱ��� �ƴѰ��)
				nChatIdx++;
			}
			else
			{
				nChatIdx += 2;
			}
		}
	}
	return TRUE;
}


CWordSearchTree::CWordSearchTree()
{
	m_nAllocatedBytes = 0;

	m_nAllocatedBytes += sizeof(WORDNODE);
	m_pRootWordNode = new WORDNODE;
	memset(m_pRootWordNode, 0, sizeof(WORDNODE) );	
}

CWordSearchTree::~CWordSearchTree()
{
	if(m_pRootWordNode)
	{
		delete m_pRootWordNode;
		m_pRootWordNode = NULL;
	}
}



void CWordSearchTree::InitWordSearchTree(void)
{
	if(m_pRootWordNode)
	{
		delete m_pRootWordNode;
		m_pRootWordNode = NULL;
	}
	m_pRootWordNode = new WORDNODE;
	memset(m_pRootWordNode, 0, sizeof(WORDNODE) );
}

//���͸��� �ܾ���� �߰��Ѵ�.
BOOL CWordSearchTree::AddWord( char *szWord )
{	
	LPWORDNODE	pBeforeNode;
	LPWORDNODE	pWordNode = this->m_pRootWordNode;
	
	for(char* pDummy = szWord; pDummy && ((*pDummy != '\0') && (*pDummy != '\n')); pDummy++)
	{
		if(*pDummy == ' ')//�Է°��� ���� ������.-.- ���º��� ������.
			continue;

		pBeforeNode = pWordNode;
		if((pWordNode = pBeforeNode->pNext[(BYTE)(*pDummy)]) == NULL)
		{	// pDummy  ���ڰ��� �ε�����ȣ�� ĳ��Ʈ�Ͽ� �װ��� ���� �����Ѵ�.
			
			m_nAllocatedBytes += sizeof(WORDNODE);
			pWordNode = new WORDNODE;
			memset(pWordNode, 0, sizeof(WORDNODE));			
			pBeforeNode->pNext[(BYTE)(*pDummy)] = pWordNode;
		}
	}
	pWordNode->bTermination = TRUE;
	return TRUE;
}

int	CWordSearchTree::AbuseFilter(char *szWord)
{
	m_nSearchWordCount = 0;
	return RecursiveSearchString(m_pRootWordNode, szWord, strlen(szWord));
}

int CWordSearchTree::RecursiveSearchString(LPWORDNODE pWordNode, char *pStpInWord, int nWordLength)
{
	DWORD		dwLength;
	LPWORDNODE	pTemp;
	BOOL		bKoreanFlag;

	dwLength = 0;
	while(pStpInWord 
		&& (*pStpInWord != '\0') 
		&& (*pStpInWord != '\n'))
	{		
		if(*pStpInWord == ' ')			
		{	// ����

			if(dwLength)
			{
				dwLength++;
			}
			pStpInWord++;
			continue;
		}

		if((*pStpInWord & 0x80) == 0)
		{	// �����̸� (�ѱ��� �ƴѰ��)

			if(dwLength == 0){	dwLength = 1;}

			////////////////////////////////////////////////////////////////////////////////
			// �ܾ �˻� �� �� �ѹ���Ʈ�� �˻�			
			if((pTemp = pWordNode->pNext[(BYTE)(*pStpInWord)]) == NULL)
			{	
				if(m_nSearchWordCount)
				{
					if(bKoreanFlag)
					{
						return dwLength - 2;
					}
					
					return dwLength - 1;
				}
				return 0;
			}
						
			pWordNode = pTemp;
			if(pWordNode->bTermination)
			{
				bKoreanFlag = FALSE;
				m_nSearchWordCount++;
			}
			pStpInWord += 1;			// �ѱ�ó���� 2����Ʈ���ѹ��� �˻�
			if(*pStpInWord != '\0')
			{
				dwLength += 1;				// ��������
			}			
		}
		else
		{
			if(dwLength == 0){	dwLength = 2;}
			
			////////////////////////////////////////////////////////////////////////////////
			// �ܾ �˻� �� �� �ѹ���Ʈ�� �˻��ϴ����� �ƴ϶� �ι���Ʈ�� �⺻���� �˻��Ѵ�.
			// �̰��� �ѱ��� �ι���Ʈ�� �����ǿ��ִ� Ư¡ �����̴�.
			if( pWordNode->pNext[(BYTE)(*pStpInWord)] == NULL 
				|| ((pTemp = pWordNode->pNext[(BYTE)(*pStpInWord)]->pNext[(BYTE)(*(pStpInWord+1))]) == NULL ))
			{	// �ι���Ʈ �˻� �� �ѱ��ϰ�� �ѱ��ڸ� �˻��ϴ°��� Ʋ������� ó���κ�
			
				if(m_nSearchWordCount)
				{
					if(bKoreanFlag)
					{
						return dwLength - 2;
					}
					
					return dwLength - 1;
				}				
				return 0;
			}
						
			pWordNode = pTemp;			
			if(pWordNode->bTermination)
			{
				bKoreanFlag = TRUE;
				m_nSearchWordCount++;
			}
			pStpInWord += 2;			// �ѱ�ó���� 2����Ʈ���ѹ��� �˻�			
			if(*pStpInWord != '\0')
			{
				dwLength += 2;				// ��������
			}			
		}		
	}
	if(m_nSearchWordCount)
	{
		return dwLength;
	}
	return 0;
}
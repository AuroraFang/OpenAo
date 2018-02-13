// AbuseFilter.cpp: implementation of the CAbuseFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbuseFilter.h"
/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbuseFilter::CAbuseFilter()
{	
	FLOG( "CAbuseFilter()" );
}

CAbuseFilter::~CAbuseFilter()
{
	FLOG( "~CAbuseFilter()" );

}

BOOL CAbuseFilter::AddAbuse(char *szAbuse)
{
	FLOG( "CAbuseFilter::AddAbuse(char *szAbuse)" );
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
	FLOG( "CAbuseFilter::UpperString(char *szSource, char *szDest)" );
	int nLenCh = strlen(szSource);
	if(0 == nLenCh){	return FALSE;}


	// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
	// �ƽ�Ű�ڵ�� �Ϻ�� �빮�ڷ� ��ȯ�� �� ������ �־�
	// �����ڵ�� ������ �빮�� ��ȯ�ѵ� �ٽ� �ƽ�Ű�ڵ�� ����
	wchar_t	szUniCode[MAX_PATH], szUpperUnicode[MAX_PATH] = {0,};
	MultiByteToWideChar(CP_ACP, 0, szSource, -1, szUniCode, MAX_PATH);

	int		nUniCodeLength = wcslen(szUniCode);
	for(int i=0; i<nUniCodeLength; ++i)
		szUpperUnicode[i] = towupper(szUniCode[i]);
	
	WideCharToMultiByte(CP_ACP, 0, szUpperUnicode, -1, szDest, MAX_PATH, NULL, NULL);

// 	int nChIdx = 0;
// 	while(nChIdx < nLenCh)
// 	{
// 		szDest[nChIdx] = toupper(szSource[nChIdx]);
// 		nChIdx++;
// 	}
	// end 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���


	return TRUE;
}


BOOL CAbuseFilter::AbuseFiltering(char *szChat, int *nFTWordCounts, int *nFTTotalBytes)
{
	FLOG( "CAbuseFilter::AbuseFiltering(char *szChat, int *nFTWordCounts, int *nFTTotalBytes)" );
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


	// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���

	char* pTemp = szCopyChat;
	// ���ھ� ���鼭 ���͸�
	while(*pTemp != NULL)
	{
		// ������ ��� �׳� �پ����
		if(*pTemp == ' ')
			pTemp = CharNext(pTemp);
		else
		{
			// ���͸��� ����Ʈ��
			int nAbuseBytes = m_WordSearchTree.AbuseFilter(pTemp);
			
			// ���͸����� �ʾ����� �׳� ���� ���ڷ�
			if(!nAbuseBytes)
				pTemp = CharNext(pTemp);
			else
			{
		// 2013-04-03 by ssjung ĳ���� �弳 ���� ��ȭ 
#ifdef C_ABUSE_FILTER_RELAX

				if((*(pTemp-1) != ' '&& *(pTemp-1) != NULL) || 
					(*(pTemp+nAbuseBytes) != NULL && *(pTemp+nAbuseBytes)!=' '))
				{
					pTemp += nAbuseBytes;
					(*nFTWordCounts)++;
					(*nFTTotalBytes) += nAbuseBytes;
				}
				else
#endif
		// 2013-04-03 by ssjung ĳ���� �弳 ���� ��ȭ 
				{
					// ���͸� �Ǿ��ٸ� ���͸� �� ����Ʈ ����ŭ '*'�� ��ȯ
					char* pDst = szChat + (pTemp - szCopyChat);
					for(int i=0; i<nAbuseBytes; ++i)
						pDst[i] = 0x2a;
				
					pTemp += nAbuseBytes;
					(*nFTWordCounts)++;
					(*nFTTotalBytes) += nAbuseBytes;
				}
			}
		}
	}

// 	int nLenChat = strlen(szChat);
// 	int nChatIdx = 0;
// 	while(nChatIdx < nLenChat)
// 	{
// 		if(szCopyChat[nChatIdx] == ' ')
// 		{
// 			nChatIdx++;
// 			continue;
// 		}
// 
// 		int nAbuseBytes = m_WordSearchTree.AbuseFilter(&szCopyChat[nChatIdx]);
// 		if(nAbuseBytes)
// 		{
// 			char szT[128];
// 			memset(szT, 0x00, 128);
// 			memcpy(szT, &szCopyChat[nChatIdx], nAbuseBytes);
// 			
// 			memset(&szChat[nChatIdx], 0x2A, nAbuseBytes);
// 			nChatIdx += nAbuseBytes;
// 
// 			(*nFTWordCounts)++;
// 			(*nFTTotalBytes) += nAbuseBytes;
// 
// 			DBGOUT("abuse filter : Find [%10s], Counts[%2d], Bytest[%3d]\n", szT, *nFTWordCounts, *nFTTotalBytes);
// 		}
// 		else
// 		{
// 			if((szCopyChat[nChatIdx] & 0x80) == 0)
// 			{	// �����̸� (�ѱ��� �ƴѰ��)
// 				nChatIdx++;
// 			}
// 			else
// 			{
// 				nChatIdx += 2;
// 			}
// 		}
// 	}

	// end 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
	return TRUE;
}


CWordSearchTree::CWordSearchTree()
{
	FLOG( "CWordSearchTree()" );
	m_nAllocatedBytes = 0;

	m_nAllocatedBytes += sizeof(WORDNODE);
	m_pRootWordNode = new WORDNODE;
	memset(m_pRootWordNode, 0, sizeof(WORDNODE) );	
}

CWordSearchTree::~CWordSearchTree()
{
	FLOG( "~CWordSearchTree()" );
	if(m_pRootWordNode)
	{
		delete m_pRootWordNode;
		m_pRootWordNode = NULL;
	}
}



void CWordSearchTree::InitWordSearchTree(void)
{
	FLOG( "CWordSearchTree::InitWordSearchTree(void)" );
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
	FLOG( "CWordSearchTree::AddWord( char *szWord )" );
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
	FLOG( "CWordSearchTree::AbuseFilter(char *szWord)" );
	m_nSearchWordCount = 0;
	return RecursiveSearchString(m_pRootWordNode, szWord, strlen(szWord));
}

int CWordSearchTree::RecursiveSearchString(LPWORDNODE pWordNode, char *pStpInWord, int nWordLength)
{
	FLOG( "CWordSearchTree::RecursiveSearchString(LPWORDNODE pWordNode, char *pStpInWord, int nWordLength)" );
	DWORD		dwLength;
	LPWORDNODE	pTemp;
	BOOL		bKoreanFlag=FALSE;

	dwLength = 0;
	while(pStpInWord 
		&& (*pStpInWord != '\0') 
		&& (*pStpInWord != '\n'))
	{		
		// 2008-10-23 by bhsohn �������� ���� �弳 ���� �ȵǰ� ����
#ifndef GAMEFORGE_RELEASE
		if(*pStpInWord == ' ')
		{	// ����
// 2013-04-03 by ssjung ĳ���� �弳 ���� �߰� �����϶� ���� �ȵǰ� ���� 		
#ifdef C_ABUSE_FILTER_RELAX

			if(m_nSearchWordCount)
			{
				return dwLength;
			}
			return 0;
#endif

		
			if(dwLength)
			{
				dwLength++;
			}
			pStpInWord++;
			continue;
		}

		// end 2009. 02. 04 by ckPark �Ϻ� �弳���� ���� ���鹮�� �ν� ����

#endif
		// end 2008-10-23 by bhsohn �������� ���� �弳 ���� �ȵǰ� ����


		// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
		char* pStrTemp = pStpInWord;
		pStrTemp = CharNext(pStpInWord);		// �������ڷ�
		
		if(pStrTemp == pStpInWord)				// ���� ������ġ�� ���� ������ġ�� ���ٸ� ���� Ż��
			break;
		else if(pStrTemp - pStpInWord < 2)		// ���� ������ġ�� ���� ������ġ�� 2���� ���ٸ� �̱� ����Ʈ�� �ν�
//		if((*pStpInWord & 0x80) == 0)
		// end 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���


		{	// �����̸� (�ѱ��� �ƴѰ��)

			// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
			//if(dwLength == 0){	dwLength = 1;}
			// end 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���

			////////////////////////////////////////////////////////////////////////////////
			// �ܾ �˻� �� �� �ѹ���Ʈ�� �˻�			
			if((pTemp = pWordNode->pNext[(BYTE)(*pStpInWord)]) == NULL)
			{	
				if(m_nSearchWordCount)
				{
					

					// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
// 					if(bKoreanFlag)
// 					{
// 						return dwLength - 2;
// 					}
// 					
// 					return dwLength - 1;
					return dwLength;
					// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���


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


			// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
// 			if(*pStpInWord != '\0')
// 			{
// 				dwLength += 1;				// ��������
// 			}
			dwLength += 1;
			// end 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���


		}
		else
		{
			// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
			//if(dwLength == 0){	dwLength = 2;}
			// end 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
			
			////////////////////////////////////////////////////////////////////////////////
			// �ܾ �˻� �� �� �ѹ���Ʈ�� �˻��ϴ����� �ƴ϶� �ι���Ʈ�� �⺻���� �˻��Ѵ�.
			// �̰��� �ѱ��� �ι���Ʈ�� �����ǿ��ִ� Ư¡ �����̴�.
			if( pWordNode->pNext[(BYTE)(*pStpInWord)] == NULL 
				|| ((pTemp = pWordNode->pNext[(BYTE)(*pStpInWord)]->pNext[(BYTE)(*(pStpInWord+1))]) == NULL ))
			{	// �ι���Ʈ �˻� �� �ѱ��ϰ�� �ѱ��ڸ� �˻��ϴ°��� Ʋ������� ó���κ�
			
				if(m_nSearchWordCount)
				{
					
					// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
// 					if(bKoreanFlag)
// 					{
// 						return dwLength - 2;
// 					}
// 					
// 					return dwLength - 1;

					return dwLength;
					// end 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
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


			// 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
// 			if(*pStpInWord != '\0')
// 			{
// 				dwLength += 2;				// ��������
// 			}
			dwLength += 2;
			// end 2009. 01. 29 by ckPark �Ϻ� �弳���� �빮�ں��� ����, �ݰ����� �νĹ���
		}		
	}
	if(m_nSearchWordCount)
	{
		return dwLength;
	}
	return 0;
}
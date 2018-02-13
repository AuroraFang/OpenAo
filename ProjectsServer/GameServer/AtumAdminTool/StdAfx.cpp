// stdafx.cpp : source file that includes just the standard includes
//	AtumAdminTool.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



char* GetSqlPattern(const char* str, char* buf)
{
	///////////////////////////////////////////////////////////////////////////////	
	// 2009-01-28 by cmkwon, �Ϻ� ĳ���͸� ���� ���� ���� - 
	util::zero(buf, SIZE_MAX_SQL_PATTERN_BUFFER);

	int		nOffset		= 0;
	int		nLen		= strlen(str);
	char *	pCurStr		= (char*)(str);
	while(pCurStr && pCurStr < str + nLen)
	{
		switch (*pCurStr)
		{
		case '%':
		case '[':
		case '_':
			{
 				if (nOffset+3 >= SIZE_MAX_SQL_PATTERN_BUFFER)
 				{
 					strcpy(buf, '\0');
 					return buf;
 				}
				buf[nOffset++] = '[';
				buf[nOffset++] = *pCurStr;
				buf[nOffset++] = ']';
			}
			break;
		default:
			{
				BOOL bIsDBSLeadByte = IsDBCSLeadByte(*pCurStr);

				if(FALSE == bIsDBSLeadByte)
				{
					if (nOffset+1 >= SIZE_MAX_SQL_PATTERN_BUFFER)
					{
						strcpy(buf, '\0');
						return buf;
					}
					buf[nOffset++]		= *pCurStr;

				}
				else
				{
					if (nOffset+2 >= SIZE_MAX_SQL_PATTERN_BUFFER)
					{
						strcpy(buf, '\0');
						return buf;
					}
					buf[nOffset++]		= *pCurStr;
					buf[nOffset++]		= *(pCurStr+1);
				}
			}
		}
		pCurStr = CharNext(pCurStr);		// ���� ���ڷ� �̵�
	}
	return buf;
	

// 2009-01-28 by cmkwon, �Ϻ� ĳ���͸� ���� ���� ���� - ���� ���� ���� ��.
// 	int len = strlen(str);
// 	int offset = 0;
// 
// 	for (int i = 0; i < len; i++)
// 	{
// 		if (str[i] == '%')
// 		{
// 			if (offset+3 >= SIZE_MAX_SQL_PATTERN_BUFFER)
// 			{
// 				strcpy(buf, '\0');
// 				return buf;
// 			}
// 
// 			buf[offset++] = '[';
// 			buf[offset++] = '%';
// 			buf[offset++] = ']';
// 		}
// 		else if (str[i] == '[')
// 		{
// 			if (offset+3 >= SIZE_MAX_SQL_PATTERN_BUFFER)
// 			{
// 				strcpy(buf, '\0');
// 				return buf;
// 			}
// 
// 			buf[offset++] = '[';
// 			buf[offset++] = '[';
// 			buf[offset++] = ']';
// 		}
// 		else if (str[i] == '_')
// 		{
// 			if (offset+3 >= SIZE_MAX_SQL_PATTERN_BUFFER)
// 			{
// 				strcpy(buf, '\0');
// 				return buf;
// 			}
// 
// 			buf[offset++] = '[';
// 			buf[offset++] = '_';
// 			buf[offset++] = ']';
// 		}
// 		else
// 		{
// 			if (offset+1 >= SIZE_MAX_SQL_PATTERN_BUFFER)
// 			{
// 				strcpy(buf, '\0');
// 				return buf;
// 			}
// 
// 			buf[offset++] = str[i];
// 		}
// 	}
// 
// 	buf[offset] = '\0';
//
//	return buf;
}
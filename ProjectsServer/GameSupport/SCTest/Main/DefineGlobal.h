///////////////////////////////////////////////////////////////////////////////
//  DefineGlobal.h : ������Ʈ�� ������� �⺻������ ����Ǵ� #define
//
//  Date	: 2004-03-17 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#ifndef _DEFINE_GLOBAL_H_
#define _DEFINE_GLOBAL_H_

#include "DebugAssert.h"
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
/// �������� ����� ���� ���, c++ code�μ��� �ǹ̴� ����
#define ARRAY_(x)			/// x�� n�� �ִ� ��
#define STRUCT_(x)			/// x��� struct �ϳ�

#define LODWORD(ll)			((DWORD)(ll))
#define HIDWORD(ll)			((DWORD)(((UINT64)(ll) >> 32) & 0xFFFFFFFF))

// RAND_MAX�� 0x7FFF�̹Ƿ� �� �̻��� �ʿ��� ���� �̳��� ����ϱ�, 20040708, kelovon
// inline int rand32()
// {
	// int i1, i2, i3;
	// i1 = rand()&0x0FFF;
	// i2 = (rand()&0x0FFF) << 12;
	// i3 = (rand()&0x007F) << 24;
	// return (i1|i2|i3);
// }

#define RANDF1(min, max)	((float)((min)*10 + (rand32() % (int)(((max)-(min)+0.1)*10)))/10)		// float, �Ҽ��� ���� 1�ڸ�
#define RANDF2(min, max)	((float)((min)*100 + (rand32() % (int)(((max)-(min)+0.01)*100)))/100)	// float, �Ҽ��� ���� 2�ڸ�
#define RANDI(min, max)		((min) + (rand32() % ((max)-(min)+1)))									// int
#define RAND100()			(rand32() % 100)														// int, 0 ~ 99
#define RAND1000()			(rand32() % 1000)														// int, 0 ~ 999
#define RAND10K()			(rand32() % 10000)														// int, 0 ~ 9999
#define RAND100K()			(rand32() % 100000)														// int, 0 ~ 99999
#define RAND256()			(rand32() % 256)														// int, 0 ~ 255

#define util::in_range(_MIN, _VALUE, _MAX)		((_MIN) <= (_VALUE) && (_MAX) >= (_VALUE))

#define util::length(_X, _Y)						sqrt((double)((_X)*(_X)+(_Y)*(_Y)))

#define MAX_INT64_VALUE		((INT64)0x7FFFFFFFFFFFFFFF)
#define MAX_UINT64_VALUE	((UINT64)0xFFFFFFFFFFFFFFFF)
#define MAX_INT_VALUE		((INT)0x7FFFFFFF)
#define MAX_UINT_VALUE		((UINT)0x7FFFFFFF)
#define MAX_SHORT_VALUE		((SHORT)0x7FFF)
#define MAX_USHORT_VALUE	((USHORT)0xFFFF)
#define MAX_BYTE_VALUE		((BYTE)0x7F)

#define util::del(p) { if(NULL != p){ delete (p); (p) = NULL;}}
#define util::del_array(p) { if(NULL != p){ delete[] (p); (p) = NULL;}}

#define IS_VALID_ARRAY_INDEX(idx, ArraySize) (((idx) >= 0) && ((idx) < (ArraySize)))
#define util::zero(p, Size) (memset((p), 0x00, (Size)))


#endif	// _DEFINE_GLOBAL_H_endif
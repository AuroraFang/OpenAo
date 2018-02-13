///////////////////////////////////////////////////////////////////////////////
// DebugAssert.h :	debug assert�� ���� ��ũ�� ���� ����
//					Condition compilaion(#defien)�� ���� ����� �޶�����.
//
// Date     : 2004-03-25 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG_ASSERT_H_
#define _DEBUG_ASSERT_H_

#include <assert.h>

#ifndef _DEBUG

#define ASSERT_ASSERT				((void)0)
#define ASSERT_ASSERT				((void)0)
#define ASSERT_REQUIRE				((void)0)
#define ASSERT_REQUIRE				((void)0)
#define ASSERT_ENSURE				((void)0)
#define ASSERT_ENSURE				((void)0)
#define ASSERT_CHECK				((void)0)
#define ASSERT_CHECK				((void)0)
#define ASSERT_IMPLIES				((void)0)
#define ASSERT_NEVER_GET_HERE		((void)0)
#define ASSERT_NOT_IMPLEMENTED_YET	((void)0)

#else // _DEBUG

#define ALL_ASSERTIONS
///////////////////////////////////////////////////////////////////////////////
// #define ALL_ASSERTIONS		: ��� Assert macro üũ��
// #define ASSERTIONS_ASSERT	: ASSERT macro �� üũ��
// #define ASSERTIONS_REQUIRE	: REQUIRE macro �� üũ��
// #define ASSERTIONS_ENSURE	: ENSURE macro �� üũ��
// #define ASSERTIONS_CHECK		: CHECK macro �� üũ��
// #define ASSERTIONS_IMPLIES	: IMPLIES macro �� üũ��
///////////////////////////////////////////////////////////////////////////////

typedef enum
{
	Assertion_ASSERT,
	Assertion_REQUIRE,
	Assertion_ENSURE,
	Assertion_CHECK,
	Assertion_IMPLIES,
	Assertion_NEVER_GET_HERE,
	Assertion_NOT_IMPLEMENTED_YET
} Assertion;


///////////////////////////////////////////////////////////////////////////////
// Assert â�� ������ ������ �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
#define ASSERT_DEBUG(expr1, expr2)										\
	{																	\
		_assert(#expr1 "(" #expr2 ")", __FILE__, __LINE__);	\
	}


///////////////////////////////////////////////////////////////////////////////
// ASSERT :	Expression�� true���� Ȯ���Ѵ�
//			ASSERTION_ASSERT �� define�Ǿ����� ������ input code�� ���� ó�� �ȴ�.
//
//		ex)
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_ASSERT)
	#define ASSERT_ASSERT(expr)	if (!(expr)) { ASSERT_DEBUG(Assertion_ASSERT, expr); } else {;}
#else
	#define ASSERT_ASSERT(expr)
#endif // ASSERTION_ASSERT_endif

///////////////////////////////////////////////////////////////////////////////
// REQUIRE : Function�� input parameter���� ���� ������ Ȯ���Ѵ�.
//			 ASSERTION_REQUIRE �� define�Ǿ����� ������ input code�� ���� ó�� �ȴ�.
//
//		ex)
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_REQUIRE)
	#define ASSERT_REQUIRE(expr)	if (!(expr)) { ASSERT_DEBUG(Assertion_REQUIRE, expr); } else {;}
#else
	#define ASSERT_REQUIRE(expr)
#endif // ASSERTION_REQUIRE_endif


///////////////////////////////////////////////////////////////////////////////
// ENSURE : Function�� ������ ������ �� ��, return value�� ���� ������
//			�ٸ� variable���� ���� ������ ���Ѵ�.
//			ASSERTION_ENSURE �� define�Ǿ����� ������ input code�� ���� ó�� �ȴ�.
//
//		ex)
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_ENSURE)
	#define ASSERT_ENSURE(expr)		if (!(expr)) { ASSERT_DEBUG(Assertion_ENSURE, expr); } else {;}
#else
	#define ASSERT_ENSURE(expr)
#endif // ASSERTION_REQUIRE_endif


///////////////////////////////////////////////////////////////////////////////
// CHECK : Conditional compilation(define ���ǿ� ����)�� ����
//			REQUIRE(), ENSURE()�� input code�� debugging�� ���� �� �����Ǵµ� ����,
//			CHECK()�� input code�� ��������. ��, ������ ���� code�� �����ϴ�.
//			ASSERTION_CHECK �� define�Ǿ����� �ʾƵ� input code�� ���� ó�� ���� �ʴ´�.
//
//		ex)	ASSERT_CHECK((pfl = fopen(PARAM_FILE, "r")) != NULL);
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_CHECK)
	#define ASSERT_CHECK(expr)		if (!(expr)) { ASSERT_DEBUG(Assertion_CHECK, expr); } else {;}
#else
	#define ASSERT_CHECK(expr)		if (!(expr)) {;} else {;}
#endif // ASSERTION_CHECK_endif


///////////////////////////////////////////////////////////////////////////////
// IMPLIES : ù��° expression�� true�� ��쿡�� �ι�° expression�� Assertion�� üũ�Ѵ�.
//			 ASSERTION_ASSERT �� define�Ǿ����� ������ input code�� ���� ó�� �ȴ�.
///////////////////////////////////////////////////////////////////////////////
#if defined(ALL_ASSERTIONS) || defined(ASSERTION_IMPLIES)
	#define ASSERT_IMPLIES(expr1, expr2)											\
		if ((expr1))																\
		{																			\
			if (!(expr2)) { ASSERT_DEBUG(Assertion_IMPLIES, #expr1 "," #expr2); }	\
		}																			\
		else {;}
#else
	#define ASSERT_IMPLIES(expr1, expr2)
#endif // ASSERTION_IMPLIES_endif


///////////////////////////////////////////////////////////////////////////////
// NEVER_GET_HERE : Conditional compilation �� ������� ��밡���ϸ�
//					Control flow�� ������ �� ���� ���� �ξ� üũ�Ѵ�.
//
//		ex)
///////////////////////////////////////////////////////////////////////////////
#define ASSERT_NEVER_GET_HERE()	ASSERT_DEBUG(Assertion_NEVER_GET_HERE, "NEVER_GET_HERE");

///////////////////////////////////////////////////////////////////////////////
// NOT_IMPLEMENTED_YET : ���� �������� ���� �Լ��� ����� ���� ���� ����Ѵ�.
//
//		ex)
///////////////////////////////////////////////////////////////////////////////
#define ASSERT_NOT_IMPLEMENTED_YET()	ASSERT_DEBUG(Assertion_NOT_IMPLEMENTED_YET, "NOT_IMPLEMENTED_YET");

#endif // _DEBUG

#endif // _DEBUG_ASSERT_H_endif

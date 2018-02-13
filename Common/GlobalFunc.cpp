///////////////////////////////////////////////////////////////////////////////
//  GlobalFunc.cpp : 
//
//  Date	: 2008-06-26 by cmkwon
///////////////////////////////////////////////////////////////////////////////

#include "GlobalFunc.h"

///////////////////////////////////////////////////////////////////////////////
// 2008-06-26 by cmkwon, float -> int ����ȯ �Լ� �߰� - 
//! INTORFLOAT union for easy access to bits of a float.
typedef union
{
    int     i;          // as integer
    float   f;          // as float
    struct              // as bit fields
    {
        unsigned int    sign:1;
        unsigned int    biasedexponent:8;
        unsigned int    significand;
    } bits;
} INTORFLOAT;

INTORFLOAT  FTOIBIAS = {((23 - 0 + 127) << 23) + (1 << 22)};


///////////////////////////////////////////////////////////////////////////////
/// \fn			int f2i(float f) 
/// \brief		// 2008-06-26 by cmkwon, float -> int ����ȯ �Լ� �߰� - 
///					��� : Game Programming Gems2 chapter 2.1�� Yossarian King���� �ۼ��� ����Դϴ�
///					���� : FPU��忡 ���� �Ϲ������� �Ϲ� int����ȯ�� ��Ȯ�� ��ġ�մϴ�.
///					���� : float�� ���� 2�� 23��(8388608)���� �۾ƾ� �մϴ�. ����, FPU��尡 Chopping�� �ƴ� ���. ���� ��� ���� fastf2i_round�� fastf2i�� ���� ����ϸ� �ȵ˴ϴ�. fastf2i_round�� fastf2i�� FPU��带 �ݿø����� ����ϴµ�, f2i�Լ��� FPU��带 ���� �������� �ʱ� ������ ���� FPU��� �״�� ��ȯ�� �õ��մϴ�. (FPU���� ���� ���� ���ñ� �ٶ��ϴ�)
/// \author		cmkwon
/// \date		2008-06-26 ~ 2008-06-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
inline int f2i(float f) 
{
	INTORFLOAT ftmp;
	ftmp.f = f;
	ftmp.f += FTOIBIAS.f;
	ftmp.i -= FTOIBIAS.i;
	return ftmp.i;	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int fastf2i_round(float f)
/// \brief		// 2008-06-26 by cmkwon, float -> int ����ȯ �Լ� �߰� - 
///				��� : FPU�� �̿��� �ζ��� ����ڵ��Դϴ�. 
///				���� : ���� �����ϴ�.
///				���� : �ݿø��� �˴ϴ�. ���� int a = int(1234.56f) �� ���� ���� �� ���´ٴ� ���Դϴ�. �ݿø��Ǿ 1235�� ���ɴϴ�. 	
/// \author		cmkwon
/// \date		2008-06-26 ~ 2008-06-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
inline int fastf2i_round(float f)
{
#ifdef _WIN32
	int i;
	__asm
	{
		fld		f
		fistp	i
	}
	return i;
#else
	return (int)(f + 0.5f);
#endif	
}


///////////////////////////////////////////////////////////////////////////////
/// \fn			int fastf2i(float fValue)
/// \brief		// 2008-06-26 by cmkwon, float -> int ����ȯ �Լ� �߰� - 
///					��� : ���� FPU�� �̿��� �ζ��� ����ڵ������� �ݿø��� ���ϵ��� �ణ�� ������ �մϴ�.
///					���� : ���� ���1�� ����ϰ� �����ϴ�.
///					���� : ���1���� �ݿø��� �ȵǵ��� �ݿø����� ���ְ� �ֽ��ϴٸ� 0.9999999f���� ���� ������ ���� �ݿø��˴ϴ�. ���� round_to_nearest �� �ణ ��Ȳ�� �°� �����ϸ� �Ǵ� ���Դϴٸ� �ణ ������ �� ����Դϴ�.
/// \author		cmkwon
/// \date		2008-06-26 ~ 2008-06-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
inline int fastf2i(float fValue)
{
#ifdef _WIN32
	static const float round_to_nearest = -0.499999f;

    int iValue;
    __asm 
	{
		fld		fValue
		fadd	round_to_nearest
		fistp	iValue
	}
    return iValue;
#else
    return (int)fValue;
#endif
}

///////////////////////////////////////////////////////////////////////////////
/// \fn			int Float2Int( float a )
/// \brief		// 2008-06-26 by cmkwon, float -> int ����ȯ �Լ� �߰� - 
///					��� : int��ȯ�� �״�� �ζ��� ���ȭ �Ͽ����ϴ�.
///					���� : �Ϲ� int����ȯ�� ��Ȯ�� ��ġ�մϴ�.
///					���� : �״��� ���� ������ �ʳ׿�.
/// \author		cmkwon
/// \date		2008-06-26 ~ 2008-06-26
/// \warning	
///
/// \param		
/// \return		
///////////////////////////////////////////////////////////////////////////////
inline int Float2Int( float a )
{
#ifdef _WIN32
	int CtrlwdHolder;
	int CtrlwdSetter;
	int RetVal;
	__asm 
	{
		fld    a					// push 'a' onto the FP stack
		fnstcw CtrlwdHolder			// store FPU control word
		movzx  eax, CtrlwdHolder	// move and zero extend word into eax
		and    eax, 0xFFFFF3FF		// set all bits except rounding bits to 1
		or     eax, 0x00000C00		// set rounding mode bits to round down
		mov    CtrlwdSetter, eax	// Prepare to set the rounding mode -- prepare to enter plaid!
		fldcw  CtrlwdSetter			// Entering plaid!
		fistp  RetVal				// Store and converted (to int) result
		fldcw  CtrlwdHolder			// Restore control word
	}
	return RetVal;
#else
	return (int)a;
#endif	
}

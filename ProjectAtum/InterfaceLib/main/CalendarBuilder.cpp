#include "CalendarBuilder.h"

const BYTE CCalendarBuilder::DAYTABLE[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

CCalendarBuilder::CCalendarBuilder()
{
}

CCalendarBuilder::~CCalendarBuilder()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::CalMonthDay
/// \brief	: �� ���� �ϼ��� ����Ѵ�(2���� ��� ���� ����)
/// \author	: ckPark
/// \date	: 5:1:2009   18:43
/// \warning: 
/// \param	: const YEAR i_nYear
/// \param	: const MONTH i_nMonth
/// \return	: int
///////////////////////////////////////////////////////////////////////////////
int		CCalendarBuilder::CalMonthDay( const YEAR i_nYear, const MONTH i_nMonth )
{
	int nDay = DAYTABLE[i_nMonth - 1];

	if(IsLeapYear(i_nYear) && i_nMonth == 2)
		++nDay;

	return nDay;
}


///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::CalTotalDay
/// \brief	: ��������� �� �ϼ��� ���Ѵ�
/// \author	: ckPark
/// \date	: 5:1:2009   18:43
/// \warning: 0����� ����⵵���� �� �ϼ�
/// \param	: const YEAR i_nYear
/// \param	: const MONTH i_nMonth
/// \param	: const DAY i_nDay
/// \return	: int
///////////////////////////////////////////////////////////////////////////////
int		CCalendarBuilder::CalTotalDay( const YEAR i_nYear, const MONTH i_nMonth, const DAY i_nDay )
{
	int y1		= i_nYear - 1;
	int total	= 365 * y1;

	total += ( y1 / 400 - y1 / 100 + y1 / 4 );

	for(int i=1; i<i_nMonth; ++i)
		total += CalMonthDay( i_nYear, i );

	total += i_nDay;

	return total;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::CalWeekDay
/// \brief	: ���� �ε����� ���Ѵ�(0��, 1��, 2ȭ, 3��, 4��, 5��, 6��)
/// \author	: 
/// \date	: 5:1:2009   18:44
/// \warning: 
/// \param	: const YEAR i_nYear
/// \param	: const MONTH i_nMonth
/// \param	: const DAY i_nDay
/// \return	: int
///////////////////////////////////////////////////////////////////////////////
int		CCalendarBuilder::CalWeekDay( const YEAR i_nYear, const MONTH i_nMonth, const DAY i_nDay )
{
	return CalTotalDay(i_nYear, i_nMonth, i_nDay) % 7;
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::IsLeapYear
/// \brief	: �ش� �⵵�� �������� �Ǵ� (���� TRUE)
/// \author	: ckPark
/// \date	: 5:1:2009   18:45
/// \warning: 
/// \param	: const YEAR i_nYear
/// \return	: BOOL
///////////////////////////////////////////////////////////////////////////////
BOOL	CCalendarBuilder::IsLeapYear( const YEAR i_nYear )
{
	return !(i_nYear % 400) || ((i_nYear % 100) && !(i_nYear % 4));
}

///////////////////////////////////////////////////////////////////////////////
/// \fn		: CCalendarBuilder::BuildCalendar
/// \brief	: �޷��� �����. �� 6��¥�� �޷��̹Ƿ� ����, �Ϳ��κе� �����
/// \author	: 
/// \date	: 5:1:2009   18:46
/// \warning: 
/// \param	: const YEAR i_nYear
/// \param	: const MONTH i_nMonth
/// \param	: Calendar * o_Calendar		// ����� �޷�Ÿ��
/// \return	: void
///////////////////////////////////////////////////////////////////////////////
void	CCalendarBuilder::BuildCalendar( const YEAR i_nYear, const MONTH i_nMonth, Calendar* o_Calendar )
{
	// �޷� �ʱ�ȭ
	memset(o_Calendar, 0, sizeof(Calendar));

	// �̹��� �ϼ�
	int		nCurMonthDays	= CalMonthDay( i_nYear, i_nMonth );
	
	// ���� �ε���
	int		nDay			= 1 - CalWeekDay( i_nYear, i_nMonth, 1 );
	
	// �޷� �����ε���, 
	int		nLineIndex, nDayIndex;
	nLineIndex = nDayIndex = 0;




	// �ͳ�, ��
	int		nNextYear, nNextMonth;
	
	if(i_nMonth != 12)
	{// �̹����� 12���̾ƴϸ�
		nNextYear	= i_nYear;			// �����
		nNextMonth	= i_nMonth + 1;		// ������
	}
	else
	{// �̹����� 12���̸�
		nNextYear	= i_nYear + 1;		// ������
		nNextMonth	= 1;				// 1��
	}
	
	// �Ϳ� �ϼ�
	int		nNextDays	= 0;


	// ����, ��
	int		nPrevYear, nPrevMonth;
	
	if(i_nMonth != 1)
	{// �̹����� 1���� �ƴ϶��
		nPrevYear	= i_nYear;			// �����
		nPrevMonth	= i_nMonth - 1;		// ����
	}
	else
	{// �̹����� 1���̶��
		nPrevYear	= i_nYear - 1;		// ����
		nPrevMonth	= 12;				// 12��
	}

	// ���� �ϼ�
	int		nPrevDays;
	// 1���� ��� ���⵵ 12�� �ϼ��� ���´�
	if(i_nMonth == 1)
		nPrevDays = CalMonthDay( i_nYear - 1, 12 );
	else	// �ƴҰ�� ���� �ϼ��� ���´�
		nPrevDays = CalMonthDay( i_nYear, i_nMonth - 1 );

	// �ϼ� �Ųٷ� ���߱�
	nPrevDays = nPrevDays + nDay - 1;



	for(int i = 0; i < MAX_CALENDAR_LINE * MAX_DAY; ++i, ++nDay)
	{
		if(nDay < 1)					// ������ ���
		{
			(*o_Calendar)[nLineIndex][nDayIndex].nYear	= nPrevYear;
			(*o_Calendar)[nLineIndex][nDayIndex].nMonth	= nPrevMonth;
			(*o_Calendar)[nLineIndex][nDayIndex].nDay	= ++nPrevDays;
		}
		else if(nDay > nCurMonthDays)	// �������� ���
		{
			(*o_Calendar)[nLineIndex][nDayIndex].nYear	= nNextYear;
			(*o_Calendar)[nLineIndex][nDayIndex].nMonth	= nNextMonth;
			(*o_Calendar)[nLineIndex][nDayIndex].nDay	= ++nNextDays;
		}
		else							// �̹����� ���
		{
			(*o_Calendar)[nLineIndex][nDayIndex].nYear	= i_nYear;
			(*o_Calendar)[nLineIndex][nDayIndex].nMonth	= i_nMonth;
			(*o_Calendar)[nLineIndex][nDayIndex].nDay	= nDay;
		}

		++nDayIndex;					// ���� �ε��� ����

		if( (i % 7) == 6)				// ������ �Ѿ�� ���
		{
			++nLineIndex;				// �����ε��� ����
			nDayIndex = 0;				// ���� �ʱ�ȭ
		}
	}
}
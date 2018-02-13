#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#include <windows.h>

#define MAX_DAY				7			// ����(��, ��, ȭ, ��, ��, ��, ��)
#define MAX_CALENDAR_LINE	6			// �޷� �ִ� ����
#define MAX_MONTH			12			// 12�� (1~12��)

typedef USHORT		YEAR;				// ��
typedef BYTE		MONTH;				// ��
typedef BYTE		DAY;				// ��

struct sYearMonthDay
{
	YEAR	nYear;
	MONTH	nMonth;
	DAY		nDay;
};


typedef sYearMonthDay	Calendar[MAX_CALENDAR_LINE][MAX_DAY];	// 6��¥�� 7�� �޷�

///////////////////////////////////////////////////////////////////////////////
// \class 	: CCalendarBuilder
// \brief	: �޷� ������
// \author	: ckPark
// \version : 
// \date	: 5:1:2009   15:09
// \warning	: 
///////////////////////////////////////////////////////////////////////////////
class CCalendarBuilder
{
private:
	// �� ���� �ϼ� ���̺�
	const	static	BYTE	DAYTABLE[MAX_MONTH];
	
public:
	CCalendarBuilder();
	~CCalendarBuilder();
	
public:
	// �� ���� �ϼ��� ����Ѵ�(2���� ��� ���� ����)
	int		CalMonthDay( const YEAR i_nYear, const MONTH i_nMonth );
	// ��������� �� �ϼ��� ���Ѵ�
	int		CalTotalDay( const YEAR i_nYear, const MONTH i_nMonth, const DAY i_nDay );
	// ���� �ε����� ���Ѵ�(0��, 1��, 2ȭ, 3��, 4��, 5��, 6��)
	int		CalWeekDay( const YEAR i_nYear, const MONTH i_nMonth, const DAY i_nDay );
	// �ش� �⵵�� �������� �Ǵ�
	BOOL	IsLeapYear( const YEAR i_nYear );
	// �޷��� �����. �� 6��¥�� �޷��̹Ƿ� ����, �Ϳ��κе� �����
	void	BuildCalendar(const YEAR i_nYear, const MONTH i_nMonth, Calendar* o_Calendar );
};

#endif
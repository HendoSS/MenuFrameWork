//http://home.roadrunner.com/~hinnant/date_algorithms.html

float Re_Range(float x, float inmin, float inmax, float outmin, float outmax)
{
	float newval = (x - inmin) * (outmax - outmin) / (inmax - inmin) + outmin;
	return newval;
}

class Calendar
{
public:
	Calendar(DXOverlay* appinstance);
	bool IsLeapYear(int year);
	int DaysInMonth(int month,bool IsLeap);
	int DaysInMonth(int year, int month);
	int DayOfWeek(int year, int month, int day);
	XMFLOAT2 DrawHeader(XMFLOAT2 pos);
	void DrawGrid(int rows,int columns);
	String GetTime();
private:
	DXOverlay* appinstance;
	XMFLOAT2 HeaderSize;
	XMFLOAT2 HeaderPos;
};
//Evaluate any year and determine if it is a leap year
bool Calendar::IsLeapYear(int year)
{
	//Statement broken into less and less likely events, until
	//we know that it is a leap year at the final test
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}
//Month 1-12, return is a numerical value 29-31
int Calendar::DaysInMonth(int month,bool IsLeap=false)
{
	/*check month first as 25% of all years are leap,
	but only 1/12 of months change values on leap*/
	if (month == 2 && IsLeap)
		return 29; //handle the 29 in february

	int daysnoleap[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	return daysnoleap[month - 1]; 
}
/*Given any year and month calculate the days in it*/
int Calendar::DaysInMonth(int year, int month)
{
	if (IsLeapYear(year))
		return DaysInMonth(month, true);
	
	return DaysInMonth(month);
}
/*Given year,month,day returns day of month Sunday: 0 Monday: 1, 9 is error code*/
int Calendar::DayOfWeek(int year, int month, int day)
{
	//from wikipedia
	if (!day <= DaysInMonth(year, month))
		std::cout << "Day is over max number of days in that month, in DayOfWeek calc \n";

	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	year -= month < 3; //evaluates to -=1 when true, or -=0 when false
	return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}
String Calendar::GetTime()
{
	std::stringstream ss;
	std::time_t t = std::time(nullptr);
	ss << std::put_time(std::localtime(&t), "%a, %b %d %I:%M:%S %p");
	return ss.str();
}
XMFLOAT2 Calendar::DrawHeader(XMFLOAT2 pos)
{
	XMFLOAT2 size = appinstance->DrawString(pos, .9f, false, "Su Mo Tu We Th Fr Sa");
	HeaderPos = pos;
	HeaderSize = size;
	return size;
}
void Calendar::DrawGrid(int rows,int columns)
{	
}
Calendar::Calendar(DXOverlay* appinstance) :appinstance(appinstance){};

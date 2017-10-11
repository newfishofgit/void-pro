#ifndef __Timestruct__H
#define __Timestruct__H

typedef struct tagUTTime //UTC
{
	int  Year;
	int  Month;
	int  Day;
	int  Hour;
	int  Minute;
	double  Second;
	tagUTTime()
	{
		Year=0;
		Month=0;
		Day=0;
		Hour=0;
		Minute=0;
		Second=0.0;
	}
	tagUTTime(int yy,int mm,int dd,int hh,int minu,double sec)
	{
		Year=yy;
		Month=mm;
		Day=dd;
		Hour=hh;
		Minute=minu;
		Second=sec;
	}
	void set(int yy,int mm,int dd,int hh,int minu,double sec)
	{
		Year=yy;
		Month=mm;
		Day=dd;
		Hour=hh;
		Minute=minu;
		Second=sec;
	}

} UTTime, *PUTTime;

typedef struct tagGJulianDay //JulianDay
{
	double	day;
	double sod;
	tagGJulianDay()
	{
		day=0.0;
		sod = 0.0;
	}
} GJulianDay, *PGJulianDay;

typedef struct tagGMJulianDay	//Modified JulianDay
{
	double	day;
	int intday;
	double sod;
	tagGMJulianDay()
	{
		day	= 0.0;
		intday = 0;
		sod = 0.0;
	}
} GMJulianDay, *PGMJulianDay;

typedef struct tagGPSTime //GPStime
{
	int		wn;//GPSweek
	double  tos;//second of week
	int     dow;//day of week
	tagGPSTime()
	{
		wn=0;
		tos=0.0;
		dow =0;
	}
} GPSTime, *PGPSTime;

void CommonTimeToJulianDay(UTTime *pct, GJulianDay *pjd);
void JulianDayToCommonTime(GJulianDay *pjd, UTTime *pct);

void CommonTimeToMJulianDay(UTTime *pct, GMJulianDay *pmjd);
void MJulianDayToCommonTime(GMJulianDay *pmjd, UTTime *pct);

void JulianDayToGPSTime(GJulianDay *pjd, GPSTime *pgpstime);
void GPSTimeToJulianDay(GPSTime *pgpstime, GJulianDay *pjd);

void MJulianDayToGPSTime(GMJulianDay *pmjd, GPSTime *pgpstime);
void GPSTimeToMJulianDay(GPSTime *pgpstime, GMJulianDay *pmjd);

void CommonTimeToGPSTime(UTTime *pct, GPSTime *pgpstime);
void GPSTimeToCommonTime(GPSTime *pgpstime, UTTime *pct);

void DOYToCommonTime(int digyear, int doy, UTTime *pct);
void CommonTimeToDOY(UTTime *pct, int *doy);

void JulianDayToMJulianDay(GJulianDay *pjd, GMJulianDay *pmjd);
void MJulianDayToJulianDay(GMJulianDay *pmjd, GJulianDay *pjd);

#endif

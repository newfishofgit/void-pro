
#include "Timestruct.h"

void CommonTimeToJulianDay(UTTime *pct, GJulianDay *pjd)
{
	int y;
	int m;
    if (pct->Month<=2)
    {
		y=pct->Year-1;
		m=pct->Month+12;
    }
	else
	{
		y=pct->Year;
		m=pct->Month;
	}
	double tt;
	tt=(pct->Hour+pct->Minute/60.0+pct->Second/3600)/24;
	pjd->day=(int)(365.25*y)+(int)(30.6001*(m+1))+pct->Day+tt+1720981.5;
	pjd->sod=pct->Hour*3600+pct->Minute*60.0+pct->Second;
}

void JulianDayToCommonTime(GJulianDay *pjd, UTTime *pct)
{
	double jd;
	jd=pjd->day;
	
	int iyear,imonth;
	double iday;
	
	int a=(int)(jd+0.5);
	int b=a+1537;
	int c=int((b-122.1)/365.25);
	int d=int(365.25*c);
	int e=int((b-d)/30.6001);
	iday=b-d-(int)(30.6001*e)+((jd+0.5)-(int(jd+0.5)));
	imonth=e-1-12*int(e/14);
	iyear=c-4715-int((7+imonth)/10);
	
	pct->Year=iyear;
	pct->Month=imonth;
	pct->Day=(int)iday;

	double ihour,iminute,isecond;
	ihour=(iday-((int)iday))*24;
	pct->Hour=(int)ihour;
	iminute=(ihour-((int)ihour))*60;
	pct->Minute=(int)iminute;
	isecond=(iminute-((int)iminute))*60;
	pct->Second=isecond;

	pct->Second=int(pct->Second+0.5);
	if (pct->Second==60.0)
	{
		pct->Second=0.0;
		pct->Minute++;
		if (pct->Minute==60)
		{
			pct->Minute=0;
			pct->Hour++;
			if (pct->Hour==24)
			{
				pct->Hour=0;
				pct->Day++;
				
				if ((pct->Month==1||pct->Month==3||pct->Month==5||pct->Month==7||pct->Month==8||
					pct->Month==10||pct->Month==12)&&pct->Day>31)
				{
					pct->Day-=31;
					pct->Month+=1;
					if (pct->Month>12)
					{
						pct->Month-=12;
						pct->Year+=1;
					}
				}
				else if ((pct->Month==4||pct->Month==6||pct->Month==9||pct->Month==11)&&pct->Day>30)
				{
					pct->Day-=30;
					pct->Month+=1;
					if (pct->Month>12)
					{
						pct->Month-=12;
						pct->Year+=1;
					}
				}
				else if (pct->Month==2&&pct->Day>28&&pct->Year%4!=0)
				{
					pct->Day-=28;
					pct->Month+=1;
					if (pct->Month>12)
					{
						pct->Month-=12;
						pct->Year+=1;
					}
				}
				else if (pct->Month==2&&pct->Day>29&&pct->Year%4==0)
				{
					pct->Day-=29;
					pct->Month+=1;
					if (pct->Month>12)
					{
						pct->Month-=12;
						pct->Year+=1;
					}
				}
			}
		}
	}
	
}

void CommonTimeToMJulianDay(UTTime *pct, GMJulianDay *pmjd)
{
	GJulianDay pjd;
	CommonTimeToJulianDay(pct,&pjd);
	JulianDayToMJulianDay(&pjd,pmjd);

}

void MJulianDayToCommonTime(GMJulianDay *pmjd, UTTime *pct)
{
	GJulianDay pjd;
	MJulianDayToJulianDay(pmjd,&pjd);
	JulianDayToCommonTime(&pjd,pct);

}

void JulianDayToGPSTime(GJulianDay *pjd,GPSTime *pgpstime)
{
	pgpstime->wn=(int)((pjd->day-2444244.5)/7);
    pgpstime->tos=(pjd->day-2444244.5-7*pgpstime->wn)*86400.0;
	pgpstime->dow=(int)(pgpstime->tos/86400.0);

}

void GPSTimeToJulianDay(GPSTime *pgpstime, GJulianDay *pjd)
{
	double JD = pgpstime->wn*7 + pgpstime->tos/86400.0 + 2444244.5;
	pjd->day = JD;

}

void MJulianDayToGPSTime(GMJulianDay *pmjd, GPSTime *pgpstime)
{
	GJulianDay pjd;
	MJulianDayToJulianDay(pmjd,&pjd);
	JulianDayToGPSTime(&pjd,pgpstime);

}

void GPSTimeToMJulianDay(GPSTime *pgpstime, GMJulianDay *pmjd)
{
	GJulianDay pjd;
	GPSTimeToJulianDay(pgpstime,&pjd);
	JulianDayToMJulianDay(&pjd,pmjd);
}

void CommonTimeToGPSTime(UTTime *pct, GPSTime *pgpstime)
{
	GJulianDay pjd;
	CommonTimeToJulianDay(pct, &pjd);
	JulianDayToGPSTime(&pjd,pgpstime);

}

void GPSTimeToCommonTime(GPSTime *pgpstime, UTTime *pct)
{
	GJulianDay pjd;
	GPSTimeToJulianDay(pgpstime, &pjd);
    JulianDayToCommonTime(&pjd, pct);

}

void DOYToCommonTime(int digyear,int doy,UTTime *pct)
{
	if (digyear<70)
		digyear += 2000;
	else
		digyear += 1900;
	GJulianDay pjd;
	pct->set(digyear,1,1,0,0,0.0);
	CommonTimeToJulianDay(pct, &pjd);
	pjd.day = pjd.day+doy-1;	
	JulianDayToCommonTime(&pjd,pct);

}

void CommonTimeToDOY(UTTime *pct, int *doy)
{
	int idoy = 0;
	GJulianDay pjd2,pjd1;
	CommonTimeToJulianDay(pct,&pjd2);
	UTTime pct1;
	pct1.set(pct->Year,1,1,0,0,0.0);
    CommonTimeToJulianDay(&pct1,&pjd1);
	idoy=(int)(pjd2.day-pjd1.day+1);
	*doy=idoy;
}

void JulianDayToMJulianDay(GJulianDay *pjd, GMJulianDay *pmjd)
{
	pmjd->day=pjd->day-2400000.5;
	pmjd->intday=(int)pmjd->day;
	pmjd->sod=pjd->sod;

}

void MJulianDayToJulianDay(GMJulianDay *pmjd, GJulianDay *pjd)
{
	pmjd->day=pmjd->intday + pmjd->sod/86400.0;
	pjd->day=pmjd->day+2400000.5;

}

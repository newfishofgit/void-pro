// GNSSCRDClass.cpp: implementation of the CGNSSCRDClass class.
//
//////////////////////////////////////////////////////////////////////
#include "coordinate_transformation.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGNSSCRDClass::CGNSSCRDClass()
{
	a = WGS84_a;
	double f = WGS84_f;
	e2 = 2*f - f*f;
}

CGNSSCRDClass::~CGNSSCRDClass()
{

}

/************************************************************************/
/*	�ɴ������ת��Ϊ�ѿ�������
˵����
cg��ָ���ת���Ĵ�������ָ�룻
pcc��ָ����ת�����ĵѿ��������ָ�룻
dSemiMajorAxis���ο�����ĳ����᣻
dFlattening���ο�����ı��ʡ�											*/
/************************************************************************/
void CGNSSCRDClass::GeodeticToCartesian (CRDGEODETIC cg, CRDCARTESIAN &cc)
{
	double B = cg.latitude;
	double L = cg.longitude;
	double H = cg.height;

	double N = a/sqrt(1-e2*sin(B)*sin(B));

	cc.x = (N + H)*cos(B)*cos(L);
	cc.y = (N + H)*cos(B)*sin(L);
	cc.z = (N*(1-e2) + H)*sin(B);
}

/************************************************************************/
/* �ɵѿ�������ת��Ϊ�������
˵����
cc��ָ���ת���ĵѿ��������ָ�룻
pcg��ָ����ת�����Ĵ�������ָ�룻
dSemiMajorAxis���ο�����ĳ����᣻
dFlattening���ο�����ı��ʡ�											*/
/************************************************************************/
void CGNSSCRDClass::CartesianToGeodetic (CRDCARTESIAN cc, CRDGEODETIC &cg)
{
//	double a = dSemiMajorAxis;
//	double f = dFlattening;
//	e2 = 2*f - f*f;
	
	cg.longitude = atan2(cc.y, cc.x);
	cg.latitude  = atan2(cc.z, (cc.x*cc.x + cc.y*cc.y));

	double tanB = 0.0;
	double tmp = 0.0;
	int i=0;
	do
	{
		i++;
		tmp = cg.latitude;
		tanB = (cc.z + a*e2*sin(tmp)/sqrt(1-e2*sin(tmp)*sin(tmp))) / sqrt(cc.x*cc.x + cc.y*cc.y);
		cg.latitude = atan(tanB);
	}
	while(fabs((cg.latitude-tmp)/PI*180*3600)>1.0e-10 && i<50);
    cg.height = sqrt(cc.x*cc.x + cc.y*cc.y)/cos(cg.latitude) -a/sqrt(1-e2*sin(cg.latitude)*sin(cg.latitude));
}


/************************************************************************/
/*�ɵѿ�������ת��Ϊվ�ĵ�ƽ����
˵����
pct��ָ����ת������վ�ĵ�ƽ�����ָ�룻
pcc��ָ���ת���ĵѿ��������ָ�룻
pccCenter��ָ��վ�ĵĵѿ��������ָ�룻
dSemiMajorAxis���ο�����ĳ����᣻
dFlattening���ο�����ı��ʡ�                                           */
/************************************************************************/
void CGNSSCRDClass::CartesianToTopocentric(CRDTOPOCENTRIC &ct, 
							 CRDCARTESIAN cc, 
							 CRDCARTESIAN ccCenter)
{
	double Xab = cc.x - ccCenter.x;
	double Yab = cc.y - ccCenter.y;
	double Zab = cc.z - ccCenter.z;

	CRDGEODETIC cgCenter;
	CartesianToGeodetic(ccCenter, cgCenter);

	ct.northing	=	-sin(cgCenter.latitude)*cos(cgCenter.longitude)*Xab
						-sin(cgCenter.latitude)*sin(cgCenter.longitude)*Yab
						+cos(cgCenter.latitude)*Zab;
	ct.easting	=	-sin(cgCenter.longitude)*Xab
						+cos(cgCenter.longitude)*Yab;
	ct.upping		=	 cos(cgCenter.latitude)*cos(cgCenter.longitude)*Xab
						+cos(cgCenter.latitude)*sin(cgCenter.longitude)*Yab
						+sin(cgCenter.latitude)*Zab;

}

/************************************************************************/
/* ��վ�ĵ�ƽֱ������ת��Ϊվ�ĵ�ƽ������
˵����
pct��ָ���ת����վ�ĵ�ƽ�����ָ�룻     
pctp��ָ����ת������վ�ĵ�ƽ�������ָ�룻                              */
/************************************************************************/
void CGNSSCRDClass::TopocentricToTopocentricPolar(CRDTOPOCENTRIC ct, CRDTOPOCENTRICPOLAR &ctp)
{
	ctp.range	  = sqrt( ct.easting*ct.easting + ct.northing*ct.northing + ct.upping*ct.upping );
	ctp.azimuth	  = atan( ct.easting/ct.northing );
	if (ct.easting>=0)
	{
		if (ct.northing<0)
			ctp.azimuth+=PI;
	}		
	else
	{
		if (ct.northing<0)
			ctp.azimuth+=PI;
		else
			ctp.azimuth+=2*PI;
	}		
	ctp.elevation = asin( ct.upping/ctp.range );
}

void CGNSSCRDClass::RadianToDuFenMiao(double dRadian, int &dDeg, int &dMin, double &dSec)
{
	double dTmp = dRadian*180/PI;

	dDeg = int(dTmp);
	dTmp = (dTmp - dDeg)*60.0;
	dMin = int(dTmp);
	dSec = (dTmp - dMin)*60.0;

}



#ifndef __GNSSCRDSTRUCTDEF__H
#define __GNSSCRDSTRUCTDEF__H


#ifndef PI
#define PI 3.1415926535898
#endif

#define WGS84_f  1/298.257223563 //CGCS2000_f  1/298.257222101
#define WGS84_a  6378137.0       //CGCS2000_a  6378137.0


typedef struct tagCRDCARTESIAN	//�ѿ������� Coordinate Cartesian 
{
	double	x;
	double	y;
	double	z;
	tagCRDCARTESIAN()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
} CRDCARTESIAN, *PCRDCARTESIAN;



typedef struct tagCRDGEODETIC  //������� Coordinate Geodetic
{
	double	latitude;	//γ��
	double	longitude;	//����
	double	height;		//�߶�
	tagCRDGEODETIC()
	{
		latitude = 0.0;
		longitude = 0.0;
		height = 0.0;
	}
} CRDGEODETIC, *PCRDGEODETIC;



typedef	struct tagCRDTOPOCENTRIC //վ�ĵ�ƽ����(��������ʽ) Coordinate TOPOCentric
{
	double	northing;
	double	easting;
	double	upping;
	tagCRDTOPOCENTRIC()
	{
		northing = 0.0;
		easting = 0.0;
		upping = 0.0;
	}
} CRDTOPOCENTRIC, *PCRDTOPOCENTRIC;


typedef	struct tagCRDTOPOCENTRICPOLAR	//վ�ĵ�ƽ����(��������ʽ) Coordinate TOPOCentric Polar
{
	double	range;			//����
	double	azimuth;		//��λ��
	double	elevation;		//�߶Ƚ� = 90 - zenith distance
	tagCRDTOPOCENTRICPOLAR()
	{
		range = 0.0;
		azimuth = 0.0;
		elevation = 0.0;
	}
} CRDTOPOCENTRICPOLAR, *PCRDTOPOCENTRICPOLAR;



#endif
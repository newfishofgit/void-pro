
#ifndef __GNSSCRDCLASS__H
#define __GNSSCRDCLASS__H

#include "coordinate_system_difine.h"
#include <math.h>


class CGNSSCRDClass  
{
public:

	void	RadianToDuFenMiao(double dRadian, int &dDeg, int &dMin, double &dSec);
	void	CartesianToGeodetic(CRDCARTESIAN cc, CRDGEODETIC &cg);//�Ͽ�������ת�������
	void	GeodeticToCartesian(CRDGEODETIC cg, CRDCARTESIAN &cc);//�������ת�Ͽ�������
	void	CartesianToTopocentric(CRDTOPOCENTRIC &ct, CRDCARTESIAN cc, CRDCARTESIAN ccCenter);//�Ͽ�������תվ������
	void	TopocentricToTopocentricPolar(CRDTOPOCENTRIC ct, CRDTOPOCENTRICPOLAR &ctp);//վ������תվ�ļ�����
	double a;
	double e2;
	CGNSSCRDClass();
	virtual ~CGNSSCRDClass();
	
};

#endif
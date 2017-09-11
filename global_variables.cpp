
#include "global_variables.h"

vector<string> g_sitelist;
vector<int> g_prnlist;

dMatrix _NBB;
dMatrix _NQQ;
dMatrix _BTPL;
dMatrix _X;
int g_obs_sum = 0;
vector<double> g_variance;

double degree2arc(double degree)
{
	return (degree*PI/180);
}
double arc2degree(double arc)
{
	double degree = arc/PI*180;
	if(degree > 180)
		degree -= 360;
	if(degree < -180)
		degree += 360;
	return degree;
}

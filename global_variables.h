#ifndef _global_variables_H
#define _global_variables_H

#include "matrix.cc"
#include "cmatrix"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include "Timestruct.h"
#include "coordinate_system_difine.h"
#include "coordinate_transformation.h"
using namespace std;

#define PI 3.1415926535898
#define LIGHT_V  2.99792458E+08
#define EARTH_V  7.292115E-5
#define EARTH_R  6378137
#define F_WGS84 1/298.257224
#define IPP_HEIGHT    4.50E+5
#define GM 3.986005e+14

#define GPS_NUM 32
#define GPS_L1 1.57542E+09
#define GPS_L2 1.2276E+09
#define LAMDA_Lw (LIGHT_V/(GPS_L1 - GPS_L2))
#define TEC2M_L1 (4.03E+17/GPS_L1/GPS_L1)
#define TEC2M_L2 (4.03E+17/GPS_L2/GPS_L2)
#define TEC2M  (TEC2M_L1 - TEC2M_L2)
#define NS2M  (LIGHT_V*1.0E-9)
#define ELEV_MASK 20.0

#define _priorSigma 0.6
#define BAD_DCB_FLAG 70.0
#define Residual_Flag 5.0
#define BAD_P4_FLAG 150.0
#define ARC_LENGTH 40

typedef techsoft::matrix<double>  dMatrix;
extern vector<string> g_sitelist;
extern vector<int> g_prnlist;
extern dMatrix _NBB;
extern dMatrix _NQQ;
extern dMatrix _BTPL;
extern dMatrix _X;
extern int g_obs_sum;
extern vector<double> g_variance;

//transform degree to arc
double degree2arc(double degree);
//degree [-180, 180]
double arc2degree(double arc);

#endif

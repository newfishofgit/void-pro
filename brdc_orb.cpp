
#include "brdc_orb.h"

bool get_sate_position(int prn, UTTime epoch, GMN *GMN_data, CRDCARTESIAN sta, CRDCARTESIAN *sate, double *clk)
{
	//get record number
	int ni = match_epoch(prn, epoch, GMN_data);
	if(ni == -1)
	{
		cout<<"(brdc_orb:sate_poition) satellite is not health, or no satellite"<<endl;
		return false;
	}

	//calculate satellite position
	GMNRecord record = GMN_data->record[ni];
	double sqrtA = record.sqrtA;
	double dn = record.dn;
	double toe = record.TOE;
	double M0 = record.M0;
	double e = record.e;

	double delt_t0 = 0.07;double delt_t = 0.0;
	GPSTime current_time;
	CommonTimeToGPSTime(&epoch, &current_time);
	while(abs(delt_t - delt_t0) > 1e-14)
	{
		if(abs(delt_t0 - delt_t) > 1)
		{
			cout<<"G"<<prn<<" error, time is "<<epoch.Hour<<":"<<epoch.Minute<<":"<<epoch.Second;
			break;
		}
		//near point distant
		delt_t = delt_t0;
		double curr_t = current_time.tos - delt_t;
		double dt = curr_t - toe;//signal transmission time
		if(dt > 302400)
			dt -= 604800;
		if(dt < -302400)
			dt += 604800;
		double n = sqrt(GM) / pow(sqrtA,3) + dn;//average angular velocity 
		double M = M0 + n * dt; 
		double E0 = M;double E = M + e*sin(E0);
		while(fabs(E0 - E) > 1e-14)
		{
			E0 = E;
			E = M + e * sin(E0);
		}
		double cos_E = cos(E);double sin_E = sin(E);
		double cosf = (cos_E - e) / (1 - e*cos_E);
		double sinf = sqrt(1 - e*e)*sin_E/(1 - e*cos_E);
		double f = atan2(sinf, cosf);
		double w = record.omega_p;
		double u1 = w + f;

		//perturbation correction
		double Cuc, Cus, Crc, Crs, Cis, Cic;
		Cuc = record.Cuc;
		Cus = record.Cus;
		Crc = record.Crc;
		Cic = record.Cic;
		Cis = record.Cis;
		Crs = record.Crs;
		double detau,detar,detai;
		detau = Cuc * cos(2*u1) + Cus * sin(2*u1);
		detar = Crc * cos(2*u1) + Crs * sin(2*u1);
		detai = Cic * cos(2*u1) + Cis * sin(2*u1);
		double i0 = record.i0;
		double iDot = record.i_dot;
		double a = sqrtA*sqrtA;
		double u = u1 + detau;
		double r = a * (1 - e * cos(E)) + detar;
		double i = i0 + detai + iDot * dt;

		//satellite positioning
		double omega = record.omega;
		double omegaDot = record.omega_dot;
		double x = r * cos(u);
		double y = r * sin(u);              
		double L = omega + (omegaDot - EARTH_V)*dt - EARTH_V * toe;/////////////////
		double X,Y,Z;
		X = x*cos(L) - y*cos(i)*sin(L);
		Y = x*sin(L) + y*cos(i)*cos(L);
		Z = y*sin(i);
		sate->x = X;sate->y = Y;sate->z = Z;
		earth_ratation(sate, delt_t);
		delt_t0 = sqrt((sate->x-sta.x)*(sate->x-sta.x) + (sate->y-sta.y)*(sate->y-sta.y) +
			(sate->z-sta.z)*(sate->z-sta.z)) / LIGHT_V;
	}

	//calculate clock bias
	double a0 = record.clk_bias;
	double a1 = record.clk_drift;
	double a2 = record.clk_drift_rate;
	*clk = a0 + a1*delt_t + a2*delt_t*delt_t;
	return true;
}

void earth_ratation(CRDCARTESIAN *sate, double dt)
{
	double theta = EARTH_V * dt;
	double sin_a = sin(theta);
	double cos_a = cos(theta);
	double x = sate->x * cos_a + sate->y * sin_a;
	double y = -sate->x * sin_a + sate->y * cos_a;
	sate->x = x;sate->y = y;
}

int match_epoch(int prn, UTTime epoch, GMN *GMN_data)
{
	GMJulianDay current_time;
	CommonTimeToMJulianDay(&epoch, &current_time);
	int day0 = current_time.intday;double sod0 = current_time.sod;

	int sum = GMN_data->record.size();
	double dt_min = 14400;
	int epoch_num = -1;
	for(int i = 0;i<sum;i++)
	{
		if(GMN_data->record[i].prn == prn)
		{
			GMJulianDay time;
			CommonTimeToMJulianDay(&GMN_data->record[i].epoch, &time);
			double dt = abs(3600*(time.intday - day0) + time.sod - sod0);
			if(dt < dt_min)
			{
				epoch_num = i;
				dt_min = dt;
			}
			if(dt > dt_min && dt_min < 14400) break;
		}
	}

	return epoch_num;
}

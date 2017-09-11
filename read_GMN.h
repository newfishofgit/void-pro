#ifndef _read_GMN_H
#define _read_GMN_H

#include "global_variables.h"

#ifndef MAXLINE_n
#define MAXLINE_n 100
#endif
#ifndef MAXCHAR_n
#define MAXCHAR_n 30
#endif

typedef struct tag_GMN_header
{
	double rinex_version;
	char type[MAXCHAR_n];
	double ion_alpha[4], ion_beta[4];	//ion parameter alpha && beta
	double trans_a0, trans_a1;			//coefficients of 1-deg polynomial,corr = a0 + a1*deltat
	int trans_t;						//reference time for polynomial
	int trans_w;						//reference week for number_
	int leap_second;
	tag_GMN_header()
	{
		for(int i = 0;i<MAXCHAR_n;i++)
		{
			type[i] = '\0';
		}
		rinex_version = 0.0;
		trans_a0 = trans_a1 = 0.0;
		trans_t = trans_w = 0;
		leap_second = 0;
		for(int i = 0;i<4;i++)
		{
			ion_alpha[i] = ion_beta[i] = 0.0;
		}
	}
}GMNHrd;

typedef struct tag_GMN_record
{
	char sys;
	int prn;
	UTTime epoch;			//TOC, year, month, day, hour, minute, second
	double clk_bias;		//SV clock bias
	double clk_drift;		//SV clock drift
	double clk_drift_rate;	//SV clock drift rate
	double IODE;			//broadcast orbit - 1.1, issue of data,ephemeris
	double Crs;				//broadcast orbit - 1.2, meter
	double dn;				//broadcast orbit - 1.3, radians/sec
	double M0;				//broadcast orbit - 1.4, radians
	double Cuc;				//broadcast orbit - 2.1, radians
	double e;				//broadcast orbit - 2.2, eccentricity
	double Cus;				//broadcast orbit - 2.3, radians
	double sqrtA;			//broadcast orbit - 2.4, meter
	double TOE;				//broadcast orbit - 3.1, toe time of ephemeris,sec of GPS week
	double Cic;				//broadcast orbit - 3.2, radians
	double omega;			//broadcast orbit - 3.3, radians
	double Cis;				//broadcast orbit - 3.4, radians
	double i0;				//broadcast orbit - 4.1, radians
	double Crc;				//broadcast orbit - 4.2, meter
	double omega_p;			//broadcast orbit - 4.3, radians
	double omega_dot;		//broadcast orbit - 4.4, radians/sec
	double i_dot;			//broadcast orbit - 5.1, radians
	double code_on_channel_L2;//broadcast orbit - 5.2
	double GPSweek;			//broadcast orbit - 5.3
	double L2_data_flag;	//broadcast orbit - 5.4, L2 P data flag
	double SV_accuracy;		//broadcast orbit - 6.1, satellite accuracy
	double SV_health;		//broadcast orbit - 6.2, satellite health
	double TGD;				//broadcast orbit - 6.3, seconds
	double IODC;			//broadcast orbit - 6.4, issue of data, clock, sec
	double time_of_msg;		//transmission time of message, sec of GPS week
	double fit_interval;	//fit interval, zero for unknown
	double spare1;			//broadcast orbit - 7
	double spare2;			//broadcast orbit - 7

	tag_GMN_record()
	{
		sys = '\0';
		prn = 0;
		clk_bias = 0.0;
		clk_drift = 0.0;
		clk_drift_rate = 0.0;
		IODC = IODE = 0.0;
		dn = 0.0;
		M0 = 0;
		Cic = Crc = Crs = Cis = Cuc = Cus = 0.0;
		e = 0.0;
		sqrtA = 0.0;
		omega = omega_dot = omega_p = 0.0;
		TOE = 0.0;
		i0 = i_dot = 0.0;
		code_on_channel_L2 = 0.0;
		GPSweek = 0.0;
		L2_data_flag = 0.0;
		SV_health = SV_accuracy = 0.0;
		TGD = 0.0;
		time_of_msg = 0.0;
		fit_interval = 0.0;
		spare1 = spare2 = 0.0;
	}
}GMNRecord;

typedef struct tag_GMN
{
	GMNHrd header;
	vector<GMNRecord> record;
}GMN;

class GMNReader
{
public:
	//read navigation file
	static int read_GMN(char* filename, GMN *GMN_data);

private:
	//read navigation file header
	static int read_GMN_header(FILE *fp, GMNHrd *header);
	//read navigation file record
	static int read_GMN_record(FILE *fp, vector<GMNRecord> &record, GMNHrd header);
	//read the data
	static double trans_exponent(char *line, int length, int integer_length);
};

#endif

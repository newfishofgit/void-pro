#ifndef _pretreatment_H
#define _pretreatment_H

#include "global_variables.h"
#include "read_GMN.h"
#include "read_GMO.h"
#include "iono_interpolation.h"
#include "brdc_orb.h"
#include "read_dcb.h"

#define QC_WIND 20

typedef struct tag_carrier_range
{
	double P1, P2, L1, L2;
	double P4, L4;
	double SP4, Leveled_L4;
	IPP ipp0;
	bool flag;
	tag_carrier_range()
	{
		flag = false;
		P1 = P2 = L1 = L2 = 0.0;
		P4 = L4 = 0.0;
		SP4 = Leveled_L4 = 0.0;
	}
}CarrierRange;

typedef struct tag_epoch_obs
{
	UTTime epoch;
	CarrierRange L4[GPS_NUM];
}Epoch_Obs;

typedef struct tag_station_OBS
{
	vector<Epoch_Obs> obs;
	string name;
	tag_station_OBS()
	{

	}
}StationObs;

typedef struct tag_Arc
{
	vector<int> arc_pos;
}Arc;

typedef struct tag_QC
{
	double mw[QC_WIND];
	double mean_mw;
	double var_mw;
	double tec;
	double tecr;
	UTTime epoch;
	int nepoch;
	int ibuf;
	tag_QC()
	{
		for(int i = 0;i<QC_WIND;i++)
			mw[i] = 0.0;
		mean_mw = var_mw = 0.0;
		tec = tecr = 0.0;
		nepoch = 0;
		ibuf = 0;
	}
}QC;

typedef struct tag_tec
{
	GMJulianDay mjd;
	int isate;
	int isite;
	double stec; //stec, unit - m
	double weig;
	IPP ipp0;
	bool flag;
	tag_tec()
	{
		isate = isite = -1;
		stec = 0.0;
		weig = 0.0;
		flag = true;
	}
}TEC;

//pretreatment
int preprocess(vector<string> obslst, GMN gmn_data, DCB *dcbs, vector<TEC> &Tecs);
//set data of current epoch
bool set_cur_data(GMJulianDay end_mjd, vector<TEC> &all_TECs, vector<TEC> &tecs);
//get multi-L4 , no data return false
bool get_ML(GMO gmo_data, GMN gmn_data, int obs_type[], vector<Epoch_Obs> &all_ML);
//get other value in L4
void count_ML(GMN gmn_data, CRDCARTESIAN site, vector<Epoch_Obs> &all_ML);
//get arc
bool get_arc(vector<Epoch_Obs> all_ML, Arc arcs[]);
//delete arc whose length is too short
void del_arc(vector<Epoch_Obs> &all_ML, Arc arcs[]);
//detect cycle slip
void detect_cycleslip(vector<Epoch_Obs> &all_ML, Arc arcs[]);
//smooth P4 with L4
void smooth_P4(vector<Epoch_Obs> &all_ML, Arc arcs[]);
//standard leveled, L4 = - P4 + N
void standard_leveled(vector<Epoch_Obs> &all_ML, Arc arcs[]);
//remove bad L4, return sum of valid data
void remove_bad_obs(vector<Epoch_Obs> &all_ML, Arc arcs[]);
//get weight
double get_weig(double elevation);
//get interval
double get_dt(UTTime last_t, UTTime next_t);
//get observation of given epoch
bool match_obs(UTTime epoch, vector<Epoch_Obs> all_ML, int *n);
//output observation;order: sta1_std, sta2_std, sta1_itg, sta2_itg
void output_obs(StationObs obs[], int mode);


#endif

#ifndef _read_GMO_H
#define _read_GMO_H

#include "global_variables.h"

#ifndef MaxChar
#define MaxChar 10
#endif
#ifndef MaxLine
#define MaxLine 1000
#endif
//#ifndef MaxObs
//#define MaxObs  20
//#endif
//#ifndef SumSys
//#define SumSys 2
//for system number, 0 - GPS, 1 - GLONASS
//#endif

/*typedef struct tag_obs_type
{
	char system_type[MaxChar];
	int sum_obs_type;
	vector<string> obs_type;
	tag_obs_type()
	{
		for(int i = 0;i<MaxChar;i++)
		{
			system_type[i] = '\0';
		}
		sum_obs_type = 0;
	}
}ObsType;*/
typedef struct tag_meta_obs_type {
    char system_type;
    int sum_obs_type;
    vector<string> meas_type;
} meta_obs_type;

typedef struct tag_obs_type {
    vector<meta_obs_type> obs_type;
    int return_designated_sum_obs_type(char prn, int &designated) {
        int sum = 0;
        designated = 0;
        for(int i = 0;i < obs_type.size();i++) {
            if(obs_type[i].system_type == prn) {
                sum = obs_type[i].sum_obs_type;
                designated = i;
                break;
            }
        }
        return sum;
    }
}ObsType;


typedef struct tag_GMO_head
{
	double rinex_version;
	CRDCARTESIAN approx_pos;		//approximate position (WGS84)
	//ObsType obs_types[SumSys];		//observation type;0-GPS
    ObsType obs_types;
	double interval;				//interval (s)
	UTTime start_time;				//start time
	UTTime end_time;				//end time
	int leap_second;				//leap second
	tag_GMO_head()
	{
		rinex_version = 0.0;
		interval = 0.0;
		leap_second = 0;
	}
}GMOHrd;

typedef struct tag_record_header
{
	UTTime epoch;
	int epoch_tag;
	int sum_sat;				//satellite sum
	//int sum_prn;
	//vector<int> gps_prn;		//list of satellite
    vector<string> sat_prn;
	tag_record_header()
	{
		sum_sat = 0;
		//sum_prn = 0;
		epoch_tag = 0;
	}
}GMORecHrd;

typedef struct tag_GPS_observation
{
	vector<double> obs;
	//vector<int> tag1;
	//vector<int> tag2;
}RecGPSObs;

typedef struct tag_GMO_record
{
	GMORecHrd hrd;
	vector<RecGPSObs> GPS_obs;
}GMORecord;

typedef struct tag_GMO
{
	GMOHrd header;
	vector<GMORecord> record;
}GMO;

class GMOReader
{
private:
	//read file data
	static int read_GMO_record(FILE *fp ,vector<GMORecord> &record, GMOHrd header);
	//read data of one epoch
	static bool read_epoch_record(FILE *fp, GMORecord *epoch_record, GMOHrd header);

public:
	//read file header
	static int read_GMO_head(FILE *fp ,GMOHrd *header);
	//read data before end_epoch; if this is the end of file, return false
	static bool read_epochs_record(FILE *fp,  GMJulianDay end_mjd, vector<GMORecord> &record, GMOHrd header);
	//succeed return 1; fail return 0
	static int read_GMO(string filename, GMO *GMO_data);
};

#endif

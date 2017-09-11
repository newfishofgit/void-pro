#ifndef _read_dcb_H
#define _read_dcb_H

#include "global_variables.h"

typedef struct tag_dcb
{
	double sate_dcb[GPS_NUM];//unit - ns
	vector<string> sitename;
	vector<double> site_dcb; //unit - ns
	tag_dcb()
	{
		for(int i = 0;i<GPS_NUM;i++)
			sate_dcb[i] = 101;
	}
}DCB;

//read DCB file
bool read_dcb(string filename, DCB *dcb_data);
//get dcb of GPS satellite; if can't find,return false
bool get_sate_dcb(int prn, DCB dcb_data, double *dcb);
//get dcb of GPS station;   if can't find,return false
bool get_site_dcb(string name, DCB dcb_data, double *dcb);

#endif

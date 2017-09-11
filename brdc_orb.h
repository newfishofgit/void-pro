#ifndef _brdc_orb_H
#define _brdc_orb_H


#include "global_variables.h"
#include "read_GMN.h"

//get the position of satellite
bool get_sate_position(int prn, UTTime epoch, GMN *GMN_data, CRDCARTESIAN sta, CRDCARTESIAN *sate, double *clk);
//return record number; if can't find, return -1
int match_epoch(int prn, UTTime epoch, GMN *GMN_data);
//instant to Protocol coordinate system
void earth_ratation(CRDCARTESIAN *sate, double dt);

#endif

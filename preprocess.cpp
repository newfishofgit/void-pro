
#include "preprocess.h"
#include<sstream>
//#include "brdc_orb.h"

/*double get_weig(double elevation)
{
	double sigObs = 0.002;
	double p4sigSquare = 2.0*sigObs*sigObs;

	double P;
	P = _priorSigma*_priorSigma / p4sigSquare*sin(elevation)*sin(elevation)*50;


	return P;
}*/

//int preprocess(vector<string> obslst, GMN gmn_data, DCB *dcbs, vector<TEC> &Tecs) {
int preprocess(vector<string> obslst, vector<TEC> &Tecs) {

    if(obslst.size() == 0)
	{
		cout<<"no observation file!"<<endl;
		return 0;
	}
	vector<TEC>().swap(Tecs);//clear data of last interval

	//read file record
    for(int file_num = 0;file_num<obslst.size();file_num++)
	{
		GMO gmo_data;
		//observation station name
		int length = obslst[file_num].length();
		/*if(strlen(obslst[file_num].c_str()) < 12)
			continue;*/

		string site_name = obslst[file_num].substr(length-12, 4);
        cout<<"begin to read observation file : "<<obslst[file_num].substr(length-12,12).c_str()<<endl;
		if(!GMOReader::read_GMO(obslst[file_num], &gmo_data))
		{
			cout<<"can't read file "<<obslst[file_num].c_str()<<endl;
			continue;
		}
		if(gmo_data.record.size() == 0)
			continue;
        //approx coor
		/*if(gmo_data.header.approx_pos.x == 0 || gmo_data.header.approx_pos.y == 0 || gmo_data.header.approx_pos.z == 0)
		{
			int str_l = obslst[file_num].length();
			cout<<endl<<"site "<<site_name.c_str()<<" has no X,Y,Z";
			continue;
		}*/
		cout<<"read file successfully, the number of epoch is "<<gmo_data.record.size()<<endl;

		//GPS observation type
		int obs_type[4];
		for(int i = 0;i<4;i++) obs_type[i] = -1;
		//int type_sum = gmo_data.header.obs_types[0].sum_obs_type;
        int designated;
        int type_sum = gmo_data.header.obs_types.return_designated_sum_obs_type('G',designated);
		for(int i = 0;i<type_sum;i++)
		{
			if(gmo_data.header.rinex_version < 3.0)
			{
				/*if(!strcmp(gmo_data.header.obs_types[0].obs_type[i].c_str(), "C1"))
					obs_type[0] = i;
				if(!strcmp(gmo_data.header.obs_types[0].obs_type[i].c_str(), "P2"))
					obs_type[1] = i;
				if(!strcmp(gmo_data.header.obs_types[0].obs_type[i].c_str(), "L1"))
					obs_type[2] = i;
				if(!strcmp(gmo_data.header.obs_types[0].obs_type[i].c_str(), "L2"))
                obs_type[3] = i;*/
                if(!strcmp(gmo_data.header.obs_types.obs_type[designated].meas_type[i].c_str(), "C1")) {
                    obs_type[0] = i;
                }
                if(!strcmp(gmo_data.header.obs_types.obs_type[designated].meas_type[i].c_str(), "P2")) {
                    obs_type[1] = i;
                }
                if(!strcmp(gmo_data.header.obs_types.obs_type[designated].meas_type[i].c_str(), "L1")) {
                    obs_type[2] = i;
                }
                if(!strcmp(gmo_data.header.obs_types.obs_type[designated].meas_type[i].c_str(), "L2")) {
                    obs_type[3] = i;
                }
			}
			else if(gmo_data.header.rinex_version >= 3.0)
			{
				/*if(!strcmp(gmo_data.header.obs_types[0].obs_type[i].c_str(), "C1C"))
					obs_type[0] = i;
				if(!strcmp(gmo_data.header.obs_types[0].obs_type[i].c_str(), "C2W"))
					obs_type[1] = i;
				if(!strcmp(gmo_data.header.obs_types[0].obs_type[i].c_str(), "L1C"))
					obs_type[2] = i;
				if(!strcmp(gmo_data.header.obs_types[0].obs_type[i].c_str(), "L2W"))
					obs_type[3] = i;*/
                if(!strcmp(gmo_data.header.obs_types.obs_type[designated].meas_type[i].c_str(), "C1C")) {
                    obs_type[0] = i;
                }
                if(!strcmp(gmo_data.header.obs_types.obs_type[designated].meas_type[i].c_str(), "C2W")) {
                    obs_type[1] = i;
                }
                if(!strcmp(gmo_data.header.obs_types.obs_type[designated].meas_type[i].c_str(), "L1C")) {
                    obs_type[2] = i;
                }
                if(!strcmp(gmo_data.header.obs_types.obs_type[designated].meas_type[i].c_str(), "L2W")) {
                    obs_type[3] = i;
                }

			}
		}
		if(obs_type[0] == -1 || obs_type[1] == -1 || obs_type[2] == -1 || obs_type[3] == -1) continue;

		vector<Epoch_Obs> all_ML;
		//if(!get_ML(gmo_data, gmn_data, obs_type, all_ML))
        if(!get_ML(gmo_data , obs_type, all_ML))
			continue;
		else
		{
			//add station name
			g_sitelist.push_back(site_name);

			int epoch_sum = all_ML.size();
			for(int iepch = 0;iepch<epoch_sum;iepch++)
			{
				for(int iprn = 0;iprn<GPS_NUM;iprn++)
				{
					CarrierRange L4 = all_ML[iepch].L4[iprn];
					if(L4.flag == false)
						continue;
					TEC one_tec;
					double stec, weig, site_dcb;
					int isate;
                    //ipp0
					//weig = get_weig(L4.ipp0.E);
                    //
                    //get DCB
					/*vector<string>::iterator it = find(dcbs->sitename.begin(), dcbs->sitename.end(), site_name);
					if(it == dcbs->sitename.end())
					{
						cout<<endl<<"site "<<site_name.c_str()<<" don't have hardware information!"<<endl;
						continue;
					}
					else
						site_dcb = dcbs->site_dcb[it - dcbs->sitename.begin()];*/
					vector<int>::iterator it2 = find(g_prnlist.begin(), g_prnlist.end(), iprn+1);
					if(it2 == g_prnlist.end())
					{
						isate = g_prnlist.size();
						g_prnlist.push_back(iprn+1);
					}
					else
						isate = it2 - g_prnlist.begin();
					//stec = L4.SP4 - (dcbs->sate_dcb[iprn] + site_dcb) * NS2M;
                    stec = L4.SP4;
                    stec = stec/TEC2M;
					//cause weig is for P observation, so unit is m
					GMJulianDay mjd;
					CommonTimeToMJulianDay(&all_ML[iepch].epoch, &mjd);

					one_tec.flag = true;
					one_tec.mjd = mjd;
					//one_tec.isate = isate;
                    one_tec.prn = iprn+1;
					//one_tec.isite = g_sitelist.size() - 1;
                    one_tec.site = site_name;
					//one_tec.weig = weig;
					//one_tec.ipp0 = L4.ipp0;
					one_tec.stec = stec;
					Tecs.push_back(one_tec);
				}
			}
		}
		vector<Epoch_Obs>().swap(all_ML);
	}
	cout<<endl;
	return 1;
}

/*bool set_cur_data(GMJulianDay end_mjd, vector<TEC> &all_TECs, vector<TEC> &tecs)
{
	int tec_size = all_TECs.size();
	for(int i = 0;i<tec_size;i++)
	{
		double second = (end_mjd.intday - all_TECs[i].mjd.intday)*86400 + (end_mjd.sod - all_TECs[i].mjd.sod);
		if(second>0 && second<=INTERVAL)
			tecs.push_back(all_TECs[i]);
	}
	if(tecs.size() == 0)
		return false;
	return true;
}
*/
void output_obs(StationObs obs[], int mode)
{
	if(mode == 1) //2 stations, smoothed P4, integer-leveled L4 & relative-error
	{
		//P4, SP4, LL4
		int obs_num = 2;
		for(int i = 0;i<obs_num;i++)
		{
			string site_name = obs[i].name;
			vector<Epoch_Obs> all_ML = obs[i].obs;
			char outfile[100];
			sprintf(outfile, "%s%s%s", "../new/", site_name.c_str(), "_obs.txt");
			FILE* fp;
			if((fp = fopen(outfile, "w")) == NULL) return;
			//write observation to outfile
			int epoch_sum = all_ML.size();
			for(int i = 0;i<epoch_sum;i++)
			{
				for(int iprn = 0;iprn<GPS_NUM;iprn ++)
				{
					if(all_ML[i].L4[iprn].flag == false)
						continue;
					UTTime epoch = all_ML[i].epoch;
					double second = epoch.Hour*3600 + epoch.Minute*60 + epoch.Second;
					fprintf(fp, "%8.1lf   %02d%14.1lf%14.1lf%14.1lf%14.1lf%8.3lf%10.3lf%8.3lf%8.3lf\n", second, iprn+1,
						all_ML[i].L4[iprn].P1, all_ML[i].L4[iprn].P2, all_ML[i].L4[iprn].L1, all_ML[i].L4[iprn].L2,
						all_ML[i].L4[iprn].P4, all_ML[i].L4[iprn].L4, all_ML[i].L4[iprn].SP4,all_ML[i].L4[iprn].Leveled_L4);
				}
			}
			vector<Epoch_Obs>().swap(all_ML);
			fclose(fp);
		}
		//relative error
		string site_name = obs[0].name;
		vector<Epoch_Obs> all1 = obs[0].obs;
		vector<Epoch_Obs> all2 = obs[1].obs;
		char outfile[100];
		sprintf(outfile, "%s%s%s", "../new/relative_", site_name.c_str(), "_obs.txt");
		FILE* fp;
		if((fp = fopen(outfile, "w")) == NULL) return;
		//write observation to outfile
		int epoch_sum = all1.size();
		CarrierRange L_std1, L_std2;int pos2 = 0;
		for(int iepoch = 0;iepoch<epoch_sum;iepoch++)
		{
			UTTime epoch = all1[iepoch].epoch;
			if(!match_obs(epoch, all2, &pos2))
			{cout<<"slip"<<endl;continue;}
			double second = epoch.Hour*3600 + epoch.Minute*60 + epoch.Second;
			//=======================================================================================
			double s2 = all2[pos2].epoch.Hour*3600 + all2[pos2].epoch.Minute*60 + all2[pos2].epoch.Second;
			if(s2 != second)
				cout<<"error"<<endl;
			//=======================================================================================
			for(int iprn = 0;iprn<GPS_NUM;iprn ++)
			{
				L_std1 = all1[iepoch].L4[iprn];
				L_std2 = all2[pos2].L4[iprn];
				double relative_P4 = 0.0;
				double relative_SP4 = 0.0;
				double relative_leveled_L4 = 0.0;
				double relative_Carrier_Range = 0.0;

				if(L_std1.flag == false || L_std2.flag == false)
					continue;

				relative_P4 = L_std1.P4 - L_std2.P4;
				relative_SP4 = L_std1.SP4 - L_std2.SP4;
				relative_leveled_L4 = L_std1.Leveled_L4 - L_std2.Leveled_L4;
				relative_Carrier_Range = L_std1.L4 - L_std2.L4;
				fprintf(fp, "%8.1lf   %02d%8.3lf%10.3lf%8.3lf%8.3lf\n", second, iprn+1, relative_P4,relative_Carrier_Range,relative_SP4, relative_leveled_L4);
			}
		}
		vector<Epoch_Obs>().swap(all1);
		vector<Epoch_Obs>().swap(all2);
		fclose(fp);
	}

	if(mode == 2) //4 stations, co-located error
	{
		string site_name = obs[0].name;
		vector<Epoch_Obs> all_std1 = obs[0].obs;
		vector<Epoch_Obs> all_std2 = obs[1].obs;
		vector<Epoch_Obs> all_itg1 = obs[2].obs;
		vector<Epoch_Obs> all_itg2 = obs[3].obs;
		char outfile[100];
		sprintf(outfile, "%s%s%s", "../new/relative_", site_name.c_str(), "_obs.txt");
		FILE* fp;
		if((fp = fopen(outfile, "w")) == NULL) return;
		//write observation to outfile
		int epoch_sum = all_std1.size();
		CarrierRange L_std1, L_std2, L_itg1, L_itg2;int pos2 = 0,pos3 = 0,pos4 = 0;
		for(int iepoch = 0;iepoch<epoch_sum;iepoch++)
		{
			UTTime epoch = all_std1[iepoch].epoch;
			if(!match_obs(epoch, all_std2, &pos2))
			{cout<<"slip"<<endl;continue;}
			if(!match_obs(epoch, all_itg1, &pos3))
			{cout<<"slip"<<endl;continue;}
			if(!match_obs(epoch, all_itg2, &pos4))
			{cout<<"slip"<<endl;continue;}
			double second = epoch.Hour*3600 + epoch.Minute*60 + epoch.Second;
			//=======================================================================================
			double s2 = all_std2[pos2].epoch.Hour*3600 + all_std2[pos2].epoch.Minute*60 + all_std2[pos2].epoch.Second;
			double s3 = all_itg1[pos3].epoch.Hour*3600 + all_itg1[pos3].epoch.Minute*60 + all_itg1[pos3].epoch.Second;
			double s4 = all_itg2[pos4].epoch.Hour*3600 + all_itg2[pos4].epoch.Minute*60 + all_itg2[pos4].epoch.Second;
			if(s2 != second || s3 != second || s4!= second)
				cout<<"error"<<endl;
			//=======================================================================================
			for(int iprn = 0;iprn<GPS_NUM;iprn ++)
			{
				L_std1 = all_std1[iepoch].L4[iprn];
				L_std2 = all_std2[pos2].L4[iprn];
				L_itg1 = all_itg1[pos3].L4[iprn];
				L_itg2 = all_itg2[pos4].L4[iprn];
				double relative_P4 = 0.0;
				double relative_SP4 = 0.0;
				double relative_leveled_L4 = 0.0;
				double relative_Carrier_Range = 0.0;

				//if(L_std1.flag == false || L_std2.flag == false || L_itg1.flag == false || L_itg2.flag == false)
				if(L_std1.flag == false || L_std2.flag == false)
					continue;

				relative_P4 = L_std1.P4 - L_std2.P4;
				relative_SP4 = L_std1.SP4 - L_std2.SP4;
				relative_leveled_L4 = L_std1.Leveled_L4 - L_std2.Leveled_L4;
				//relative_Carrier_Range = L_itg1.L4 - L_itg2.L4;
				fprintf(fp, "%8.1lf   %02d%8.1lf%8.1lf%8.1lf%8.1lf\n", second, iprn+1, relative_P4, relative_SP4, relative_leveled_L4, relative_Carrier_Range);
			}
		}
		vector<Epoch_Obs>().swap(all_std1);
		vector<Epoch_Obs>().swap(all_std2);
		vector<Epoch_Obs>().swap(all_itg1);
		vector<Epoch_Obs>().swap(all_itg2);
		fclose(fp);
	}
}

//bool get_ML(GMO gmo_data, GMN gmn_data, int obs_type[], vector<Epoch_Obs> &all_ML4)
bool get_ML(GMO gmo_data, int obs_type[], vector<Epoch_Obs> &all_ML4)
{
	CRDCARTESIAN sate_position = gmo_data.header.approx_pos;
	int epoch_sum = gmo_data.record.size();
	for(int iepoch = 0;iepoch<epoch_sum;iepoch++)
	{
		Epoch_Obs epoch_ML4;
		epoch_ML4.epoch = gmo_data.record[iepoch].hrd.epoch;
		for(int iprn = 0;iprn<GPS_NUM;iprn++)
		{
            string GPSSAT;
            stringstream ss;
            if(iprn + 1 < 10) {
                ss<<(iprn+1);
                GPSSAT = "G0"+ss.str();
            } else {
                ss<<(iprn+1);
                GPSSAT = "G"+ss.str();
            }
			//find coordinating data number
			//vector<int> prn_list = gmo_data.record[iepoch].hrd.gps_prn;
            vector<string> prn_list = gmo_data.record[iepoch].hrd.sat_prn;
			//vector<int>::iterator itprn = find(prn_list.begin(), prn_list.end(), iprn+1);
            vector<string>::iterator itprn = find(prn_list.begin(), prn_list.end(), GPSSAT);
			if(itprn == prn_list.end())
			{
				continue;
			}
			int isate = itprn - prn_list.begin();

			//get L4
			epoch_ML4.L4[iprn].P1 = gmo_data.record[iepoch].GPS_obs[isate].obs[obs_type[0]];
			epoch_ML4.L4[iprn].P2 = gmo_data.record[iepoch].GPS_obs[isate].obs[obs_type[1]];
			epoch_ML4.L4[iprn].L1 = gmo_data.record[iepoch].GPS_obs[isate].obs[obs_type[2]];
			epoch_ML4.L4[iprn].L2 = gmo_data.record[iepoch].GPS_obs[isate].obs[obs_type[3]];
			if(epoch_ML4.L4[iprn].P1!=0.0 && epoch_ML4.L4[iprn].P2!=0.0 && epoch_ML4.L4[iprn].L1!=0.0 && epoch_ML4.L4[iprn].L2!=0.0)
			{
				epoch_ML4.L4[iprn].L4 = epoch_ML4.L4[iprn].L1*LIGHT_V/GPS_L1 - epoch_ML4.L4[iprn].L2*LIGHT_V/GPS_L2;
				epoch_ML4.L4[iprn].P4 = epoch_ML4.L4[iprn].P1 - epoch_ML4.L4[iprn].P2;
				epoch_ML4.L4[iprn].SP4 = epoch_ML4.L4[iprn].P4;
				epoch_ML4.L4[iprn].Leveled_L4 = epoch_ML4.L4[iprn].L4;
				epoch_ML4.L4[iprn].flag = true;
			}
		}
		all_ML4.push_back(epoch_ML4);
	}

	//delete bad L4 && standard-leveled
	Arc arcs[GPS_NUM];
	get_arc(all_ML4, arcs);
	del_arc(all_ML4, arcs);
	detect_cycleslip(all_ML4, arcs);
	del_arc(all_ML4, arcs);
	smooth_P4(all_ML4, arcs);
	//standard_leveled(all_ML4, arcs);
	remove_bad_obs(all_ML4, arcs);
	//count_ML(gmn_data, gmo_data.header.approx_pos, all_ML4);
	Arc Parcs[GPS_NUM];
	if(get_arc(all_ML4, Parcs))
		return true;
	else
	{
		cout<<" no data!"<<endl;
		return false;
	}
}

void remove_bad_obs(vector<Epoch_Obs> &all_ML, Arc arcs[])
{
	//elevation mask angle -- see count_MLL4

	for(int iprn = 0;iprn<GPS_NUM;iprn++)
	{
		int narc = arcs[iprn].arc_pos.size()/2;
		for(int iarc = 0;iarc<narc;iarc++)
		{
			int start_epoch = arcs[iprn].arc_pos[iarc*2];
			int end_epoch = arcs[iprn].arc_pos[iarc*2+1];
			double ave = 0.0;
			for(int iepoch = start_epoch;iepoch<=end_epoch;iepoch++)
			{
				ave += all_ML[iepoch].L4[iprn].P4;
			}
			ave /= (end_epoch - start_epoch + 1);
			if(fabs(ave) > BAD_P4_FLAG)
			{
				for(int iepoch = start_epoch;iepoch <= end_epoch;iepoch++)
				{
					all_ML[iepoch].L4[iprn].flag = false;
				}
			}
		}
	}
}

/*void count_ML(GMN gmn_data, CRDCARTESIAN site, vector<Epoch_Obs> &all_ML4)
{
	int epoch_sum = all_ML4.size();
	for(int iepoch = 0;iepoch<epoch_sum;iepoch++)
	{
		UTTime epoch = all_ML4[iepoch].epoch;
		for(int iprn = 0;iprn<GPS_NUM;iprn++)
		{
			if(all_ML4[iepoch].L4[iprn].flag == false)
				continue;
			CRDCARTESIAN sate;double clk;
			if(!get_sate_position(iprn+1, epoch, &gmn_data, site, &sate, &clk))
			{
				all_ML4[iepoch].L4[iprn].flag = false;
				continue;
			}
			get_IPP(sate, site, &all_ML4[iepoch].L4[iprn].ipp0);
			if(arc2degree(all_ML4[iepoch].L4[iprn].ipp0.E) < ELEV_MASK)
			{
				all_ML4[iepoch].L4[iprn].flag = false;
				continue;
			}
		}
	}
}*/

bool get_arc(vector<Epoch_Obs> all_ML4, Arc arcs[])
{
	int epoch_sum = all_ML4.size();
	bool arc_flag = false;
	for(int iprn = 0;iprn<GPS_NUM;iprn++)
	{
		for(int iepoch = 0;iepoch<epoch_sum;iepoch++)
		{
			if(iepoch == 0 && all_ML4[iepoch].L4[iprn].flag == true)
			{
				arcs[iprn].arc_pos.push_back(iepoch);
			}
			if(iepoch == epoch_sum-1)
			{
				if(all_ML4[iepoch].L4[iprn].flag == true)
				{
					arcs[iprn].arc_pos.push_back(iepoch);
				}
				continue;
			}
			if(all_ML4[iepoch].L4[iprn].flag == true && all_ML4[iepoch+1].L4[iprn].flag == false)
			{
				arcs[iprn].arc_pos.push_back(iepoch);
				continue;
			}
			if(all_ML4[iepoch].L4[iprn].flag == false && all_ML4[iepoch+1].L4[iprn].flag == true)
			{
				arcs[iprn].arc_pos.push_back(iepoch + 1);
			}
		}
		int n = arcs[iprn].arc_pos.size();
		if(n != 0)
			arc_flag = true;
	}
	return arc_flag;
}

void del_arc(vector<Epoch_Obs> &all_ML4, Arc arcs[])
{
	for(int iprn = 0;iprn<GPS_NUM;iprn++)
	{
		int n_arc = arcs[iprn].arc_pos.size()/2;
		for(int iarc = 0;iarc<n_arc;iarc++)
		{
			int start_epoch = arcs[iprn].arc_pos[2 * iarc];
			int end_epoch = arcs[iprn].arc_pos[2*iarc + 1];
			if((end_epoch - start_epoch) < ARC_LENGTH)
			{
				for(int i = start_epoch;i<=end_epoch;i++)
				{
					all_ML4[i].L4[iprn].flag = false;
				}
				vector<int> :: iterator it = arcs[iprn].arc_pos.begin() + 2*iarc;
				arcs[iprn].arc_pos.erase(it);
				it = arcs[iprn].arc_pos.begin() + 2*iarc;
				arcs[iprn].arc_pos.erase(it);
				iarc--;
				n_arc--;
			}
		}
	}
}

void detect_cycleslip(vector<Epoch_Obs> &all_ML4, Arc arcs[])
{
	int epoch_sum = all_ML4.size();
	for(int iprn = 0;iprn<GPS_NUM;iprn++)
	{
		int n_arc = arcs[iprn].arc_pos.size()/2;
		for(int iarc = 0;iarc<n_arc;iarc++)
		{
			QC qc_pre;
			int start_epoch = arcs[iprn].arc_pos[2*iarc];
			int end_epoch = arcs[iprn].arc_pos[2*iarc+1];
			for(int iepoch = start_epoch+1;iepoch<=end_epoch;iepoch++)
			{
				//if(iprn == 1 && all_ML4[iepoch].epoch.Hour == 13)
				//	cout <<"here"<<endl;
				//calculate index
				CarrierRange L4 = all_ML4[iepoch].L4[iprn];
				double Nw, dmw, mwth;
				Nw = (LAMDA_Lw*(L4.L1 - L4.L2) - (GPS_L1*L4.P1 + GPS_L2*L4.P2)/(GPS_L1+GPS_L2))/LAMDA_Lw;
				dmw = Nw - qc_pre.mean_mw;
				mwth = 4*sqrt(qc_pre.var_mw);
				double tec, dtec, tecth, dt;
				tec = L4.L4 / (-TEC2M_L1+TEC2M_L2);
				if(iepoch == start_epoch+1)
					dt = 0;
				else
					dt = get_dt(qc_pre.epoch, all_ML4[iepoch].epoch);
				dtec = tec - qc_pre.tec - qc_pre.tecr*dt;
				if(qc_pre.tecr == 0.0)
					tecth = 10;
				else
					tecth = 0.72 * (1 - exp(-dt/60)/2);
				//check cycle slip, insert new arc
				if((fabs(dmw)>mwth || fabs(dtec)>tecth) && iepoch != arcs[iprn].arc_pos[0] + 1)
				{
					//cout<<"find cycle slip! "<<iprn+1<<" "<<all_ML4[iepoch].epoch.Hour<<": "<<all_ML4[iepoch].epoch.Minute<<endl;
					vector<int>::iterator it = arcs[iprn].arc_pos.begin()+iarc*2+1;
					arcs[iprn].arc_pos.insert(it, iepoch);
					it = arcs[iprn].arc_pos.begin()+iarc*2+1;
					arcs[iprn].arc_pos.insert(it, iepoch-1);
					n_arc++;
					iarc++;
					qc_pre.ibuf = 0;
					qc_pre.nepoch = 0;
				}
				//update QC
				qc_pre.ibuf = qc_pre.ibuf + 1 - (int)((qc_pre.ibuf + 1)/20) * 20;
				qc_pre.nepoch =  qc_pre.nepoch+1;
				int k = qc_pre.ibuf;
				int n = qc_pre.nepoch;
				qc_pre.epoch = all_ML4[iepoch].epoch;
				if(n == 1)
				{
					qc_pre.var_mw = LAMDA_Lw * LAMDA_Lw /4.0;
					qc_pre.tecr = 0.0;
				}
				else if(n <= QC_WIND)
				{
					qc_pre.var_mw = ((n-1)*qc_pre.var_mw + pow((Nw - qc_pre.mean_mw), 2)) / n;
					qc_pre.tecr = (tec - qc_pre.tec)/dt;
				}
				else
				{
					qc_pre.var_mw = qc_pre.var_mw + (Nw-qc_pre.mw[k])*(Nw+qc_pre.mw[k]-2*qc_pre.mean_mw)/QC_WIND;
					qc_pre.tecr = (tec - qc_pre.tec)/dt;
				}
				qc_pre.mean_mw = (qc_pre.mean_mw*(n-1) + Nw) / n;
				qc_pre.mw[k] = Nw;
				qc_pre.tec = tec;
			}
		}
	}
}

void smooth_P4(vector<Epoch_Obs> &all_ML4, Arc arcs[])
{
	int epoch_sum = all_ML4.size();
	for(int iprn = 0;iprn<GPS_NUM;iprn++)
	{
		int n_arc = arcs[iprn].arc_pos.size()/2;
		for(int iarc = 0;iarc<n_arc;iarc++)
		{
			int start_epoch = arcs[iprn].arc_pos[2*iarc];
			int end_epoch = arcs[iprn].arc_pos[2*iarc+1];
			int t = 2;
			for(int iepoch = start_epoch+1;iepoch<=end_epoch;iepoch++)
			{
				all_ML4[iepoch].L4[iprn].SP4 = all_ML4[iepoch].L4[iprn].SP4/t + (all_ML4[iepoch-1].L4[iprn].SP4
					- all_ML4[iepoch].L4[iprn].L4 + all_ML4[iepoch-1].L4[iprn].L4) *(t-1)/t;
				t++;
				if(t>240)
					t = 2;
			}
		}
	}
}

void standard_leveled(vector<Epoch_Obs> &all_ML, Arc arcs[])
{
	int epoch_sum = all_ML.size();
	for(int iprn = 0;iprn<GPS_NUM;iprn++)
	{
		int n_arc = arcs[iprn].arc_pos.size()/2;
		for(int iarc = 0;iarc<n_arc;iarc++)
		{
			int start_epoch = arcs[iprn].arc_pos[2*iarc];
			int end_epoch = arcs[iprn].arc_pos[2*iarc+1];
			double amb = 0.0;
			double t_amb;
			int nepoch = 0;
			for(int i = start_epoch;i<=end_epoch;i++)
			{
				t_amb = all_ML[i].L4[iprn].L4 - ( - all_ML[i].L4[iprn].P4); //L4\BA\CDP4\B5\C4ֵ\CF෴
				amb = amb + t_amb;
				nepoch++;
			}
			amb /= nepoch;
			for(int i = start_epoch;i<=end_epoch;i++)
			{
				all_ML[i].L4[iprn].Leveled_L4 -= amb;
			}
		}
	}
}

double get_dt(UTTime last_t, UTTime next_t)
{
	GMJulianDay t1, t2;
	CommonTimeToMJulianDay(&last_t, &t1);
	CommonTimeToMJulianDay(&next_t, &t2);

	double dt = (t2.intday - t1.intday)*86400 + t2.sod - t1.sod;
	return dt;
}

bool match_obs(UTTime epoch, vector<Epoch_Obs> all_ML, int *n)
{
	int epoch_sum = all_ML.size();
	for(int i = *n;i<epoch_sum;i++)
	{
		double dt = get_dt(epoch, all_ML[i].epoch);
		if(dt  == 0.0)
		{
			*n = i;
			return true;
		}
		if(dt > 0.0)
		{
			return false;
		}
	}
	return false;
}

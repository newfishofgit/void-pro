
#include "read_GMO.h"

int GMOReader::read_GMO_head(FILE *fp ,GMOHrd *header)
{
	char line[MaxLine+1] = "";
	double version;

	fgets(line, MaxLine, fp);
	if (!strstr(line, "RINEX VERSION / TYPE"))
	{
		return 0;
	}
	sscanf(line,"%lf", &version);
	if(version < 2 || version >= 4.0)
	{
        cout<<"Invalid rinex version!"<<endl;
		return 0;
	}
	header->rinex_version = version;
    bool  b_having_read_obs_types = false;
    bool b_once = false;
    while (true)
	{
        if(!(b_having_read_obs_types == true && b_once == true)) {
            fgets(line, MaxLine, fp);
        }
        b_once = false;
        if(strstr(line, "END OF HEADER")) {
            return 1;
        }
		if (strstr(line, "APPROX POSITION XYZ"))
		{
			sscanf(line, "%lf%lf%lf", &header->approx_pos.x, &header->approx_pos.y, &header->approx_pos.z);
		}
		else if (strstr(line, "# / TYPES OF OBSERV") && version < 3.0)
		{
			/*int obs_sum = 0;
			char obs_type[3];

			int sys_num = 0;
			sscanf(line+3, "%d", &obs_sum);
			header->obs_types[sys_num].sum_obs_type = obs_sum;

			if (obs_sum <= 9)
			{
				for(int i = 0;i<obs_sum;i++)
				{
					strncpy(obs_type, line+i*6+10, 2);
					obs_type[2] = '\0';
					header->obs_types[sys_num].obs_type.push_back(obs_type);
				}
			}
			else if(obs_sum > 9 && obs_sum <= 18)
			{
				for(int i = 0;i<9;i++)
				{
					strncpy(obs_type, line+i*6+10, 2);
					obs_type[2] = '\0';
					header->obs_types[sys_num].obs_type.push_back(obs_type);
				}
				fgets(line, MaxLine, fp);
				for(int i = 0;i<obs_sum - 9;i++)
				{
					strncpy(obs_type, line+i*6+10, 2);
					obs_type[2] = '\0';
					header->obs_types[sys_num].obs_type.push_back(obs_type);
				}
			}
			else if(obs_sum > 18)
			{
				cout<<"(read_GMO) the sum of observations type is more than 18"<<endl;
				return 0;
			}*/
		}
		else if (strstr(line, "SYS / # / OBS TYPES") && version >= 3.0)
		{
			/*int obs_sum = 0;
			char obs_type[4];
			int sys_num;
			if (line[0] == 'G')	//read GPS observation type
			{
				sys_num = 0;
			}
			else if(line[0] == 'R')
			{
				sys_num = 1;
			}
			else
			{
				continue;
			}

			strncpy(header->obs_types[sys_num].system_type, line, 2);
			sscanf(line+3, "%d", &obs_sum);
			header->obs_types[sys_num].sum_obs_type = obs_sum;

			if (obs_sum <= 13)
			{
				for(int i = 0;i<obs_sum;i++)
				{
					strncpy(obs_type, line+i*4+7, 3);
					obs_type[3] = '\0';
					header->obs_types[sys_num].obs_type.push_back(obs_type);
				}
			}
			else if (obs_sum <= 26)
			{
				for(int i = 0;i<13;i++)
				{
					strncpy(obs_type, line+i*4+7, 4);
					obs_type[3] = '\0';
					header->obs_types[sys_num].obs_type.push_back(obs_type);
				}
				fgets(line, MaxLine, fp);
				for(int i = 0;i<obs_sum - 13;i++)
				{
					strncpy(obs_type, line+i*4+7, 4);
					obs_type[3] = '\0';
					header->obs_types[sys_num].obs_type.push_back(obs_type);
				}
			}
			else
			{
				cout<<"(read_GMO) the sum of observation type is more than 26"<<endl;
				return 0;
			}*/
            char meas_type[4];
            while(true) {
                meta_obs_type m_o_t;
                m_o_t.system_type = line[0];
                sscanf(line+3, "%d", &m_o_t.sum_obs_type);
                int count_line = m_o_t.sum_obs_type / 13;
                int count_yokei = m_o_t.sum_obs_type % 13;
                for(int i = 0; i < count_line;i++) {
                    for(int j= 0;j<13;j++) {
                        strncpy(meas_type, line+4*i+7,4);
                        meas_type[3] = '\0';
                        m_o_t.meas_type.push_back(meas_type);
                    }
                    fgets(line,MaxLine, fp);
                }
                if(count_yokei == 0) {
                    header->obs_types.obs_type.push_back((m_o_t));
                }
                else {
                    for(int j = 0;j<count_yokei;j++) {
                        strncpy(meas_type, line+4*j+7,4);
                        meas_type[3] = '\0';
                        m_o_t.meas_type.push_back(meas_type);
                    }
                    header->obs_types.obs_type.push_back(m_o_t);
                    fgets(line,MaxLine,fp);
                }
                if (!strstr(line, "SYS / # / OBS TYPES")) {
                    break;
                }
            }
            b_having_read_obs_types = true;
            b_once = true;
        }
		else if (strstr(line, "INTERVAL"))
		{
			sscanf(line, "%lf", &header->interval);
		}
		else if (strstr(line, "TIME OF FIRST OBS"))
		{
			sscanf(line, "%d%d%d%d%d%lf", &header->start_time.Year, &header->start_time.Month,&header->start_time.Day,
											&header->start_time.Hour, &header->start_time.Minute,&header->start_time.Second);
		}
		else if (strstr(line, "TIME OF LAST OBS"))
		{
			sscanf(line, "%d%d%d%d%d%lf", &header->end_time.Year, &header->end_time.Month,&header->end_time.Day,
				&header->end_time.Hour, &header->end_time.Minute,&header->end_time.Second);
		}
		else if (strstr(line, "LEAP SECONDS"))
		{
			sscanf(line, "%d", &header->leap_second);
		}
		else if (strstr(line, "END OF HEADER"))
		{
			break;
		}
	}
	return 1;
}

int GMOReader::read_GMO_record(FILE *fp, vector<GMORecord> &record, GMOHrd header)
{
	int year, month, day, hour, minute,  epoch_tag,  sate_sum;
	double second;
	int sys_num;
	if(header.rinex_version < 3.0)
	{
		/*char line[MaxLine+1] = {'\0'};
		char templine[MaxLine+1] = {'\0'};
		while(fgets(line, MaxLine, fp))
		{
			if(strstr(line, "COMMENT"))
			{
				continue;
			}
			if(strlen(line) < 2)
			{
				continue;
			}
			//get the header of each epoch
			GMORecord one_record;
			sscanf(line, "%d%d%d%d%d%lf%d%d", &year, &month, &day, &hour, &minute, &second,
				&epoch_tag, &sate_sum);
			one_record.hrd.epoch.Year	= year + 2000;
			one_record.hrd.epoch.Month	= month;
			one_record.hrd.epoch.Day	= day;
			one_record.hrd.epoch.Hour	= hour;
			one_record.hrd.epoch.Minute	= minute;
			one_record.hrd.epoch.Second = second;
			one_record.hrd.epoch_tag	= epoch_tag;
			//read comment line or satellite sum
			one_record.hrd.sum_sat		= sate_sum;

			if(epoch_tag != 0 && epoch_tag != 1)
			{
				cout<<"epoch tag is "<<epoch_tag<<endl;
				for(int i = 0;i<sate_sum;i++)
				{
					fgets(line, MaxLine, fp);
				}
				continue;
			}

			//get the observation type
			vector<char> sys;vector<int> prn;
			char a_sys;int a_prn;
			if(sate_sum <= 12)
			{
				for(int i = 0;i<sate_sum; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
			}
			else if(sate_sum <= 24)
			{
				for(int i = 0;i<12; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
				fgets(line, MaxLine, fp);
				for(int i = 0;i<sate_sum - 12; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
			}
			else if(sate_sum <= 36)
			{
				for(int i = 0;i<12; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
				fgets(line, MaxLine, fp);
				for(int i = 0;i<12; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
				fgets(line, MaxLine, fp);
				for(int i = 0;i<sate_sum - 24; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
			}
			else
			{
				cout<<"(read_GMO) the sum of observation is more than 36"<<endl;
				return 0;
			}

			//read observation
			int prn_sum = 0;
			int obs_sum = header.obs_types[0].sum_obs_type;
			int lines = (int)((obs_sum+4.5)/5.0);

			for(int sate_num = 0;sate_num<sate_sum;sate_num++)
			{
				RecGPSObs one_obs;

				if(sys[sate_num] == 'G') //read GPS data
				{
					one_record.hrd.gps_prn.push_back(prn[sate_num]);
					int tail_line_obs_sum, temp_line_obs_sum;

					if(lines == 1)
						tail_line_obs_sum = obs_sum;
					else
						tail_line_obs_sum = obs_sum - 5*(lines-1);

					for(int line_num = 1;line_num <= lines;line_num++)
					{
						if(!fgets(line, MaxLine, fp))
						{
							cout<<"(read_GMO) observation file format error!"<<endl;
							return 0;
						}
						if(line_num == lines)
							temp_line_obs_sum = tail_line_obs_sum;
						else
							temp_line_obs_sum = 5;

						int blank_obs_sum = 0;
						for(int i = 0;i<temp_line_obs_sum;i++)
						{
							int length = strlen(line);
							if(strlen(line) < (16*i + 2))
							{
								temp_line_obs_sum = i;
								blank_obs_sum = temp_line_obs_sum - i + 1;
							}
						}

						for(int j = 0;j<temp_line_obs_sum;j++)
						{
							strncpy(templine, line+j*16, 14);templine[14] = '\0';
							one_obs.obs.push_back(atof(templine));
							strncpy(templine, line+j*16+14, 1);templine[1] = '\0';
							one_obs.tag1.push_back(atoi(templine));
							strncpy(templine, line+j*16+15, 1);templine[1] = '\0';
							one_obs.tag2.push_back(atoi(templine));
						}
						while(blank_obs_sum > 0)
						{
							one_obs.obs.push_back(0.0);
							one_obs.tag1.push_back(0);
							one_obs.tag2.push_back(0);
							blank_obs_sum--;
;						}
					}
					prn_sum++;
					one_record.GPS_obs.push_back(one_obs);
				}
				else
				{
					for(int line_num = 0;line_num<lines;line_num++)
					{
						fgets(line, MaxLine, fp);
					}
				}
			}

			one_record.hrd.sum_prn = prn_sum;
			record.push_back(one_record);
		}*/
	}
	if(header.rinex_version >= 3.0)
	{
		char line[MaxLine+1] = {'\0'};
		char templine[MaxLine+1] = {'\0'};
		while(fgets(line, MaxLine, fp))
		{
			if(strstr(line, "COMMENT"))
			{
				continue;
			}
			if(strlen(line) < 2)
			{
				continue;
			}
			if(line[0] == '>')
			{
				//get the header of each epoch
				GMORecord one_record;
				sscanf(line+2, "%d%d%d%d%d%lf%d%d", &year, &month, &day, &hour, &minute, &second,
					&epoch_tag, &sate_sum);
				one_record.hrd.epoch.Year	= year;
				one_record.hrd.epoch.Month	= month;
				one_record.hrd.epoch.Day	= day;
				one_record.hrd.epoch.Hour	= hour;
				one_record.hrd.epoch.Minute	= minute;
				one_record.hrd.epoch.Second = second;
				one_record.hrd.epoch_tag	= epoch_tag;
				//read comment line or satellite sum
				one_record.hrd.sum_sat		= sate_sum;

				if(epoch_tag != 0 && epoch_tag != 1)
				{
					cout<<"epoch tag is "<<epoch_tag<<endl;
					for(int i = 0;i<sate_sum;i++)
					{
						fgets(line, MaxLine, fp);
					}
					continue;
				}

				//get the observations of each epoch
				int prn_sum = 0;
				bool record_tag = true;
				for(int i = 0;i<sate_sum;i++)
				{
					char line[MaxLine+1] = {'\0'};
					char templine[MaxLine+1] = {'\0'};
					if(!fgets(line, MaxLine, fp))
					{
						cout<<"(read_GMO) observation file format error!"<<endl;
						record_tag = false;
						break;
					}

					//read GPS data
					/*if(line[0] == 'G')
					{
						sys_num = 0;
						//read prn
						char prn[3];
						RecGPSObs one_obs;
						strncpy(prn, line+1, 2);prn[2] = '\0';
						one_record.hrd.gps_prn.push_back(atoi(prn));

						//read observation value
						double value_0;int tag1, tag2;
						int type_sum = header.obs_types[sys_num].sum_obs_type;
						for(int i = 0;i<type_sum;i++)
						{
							strncpy(templine, line+3+16*i, 14);templine[17] = '\0';
							value_0 = atof(templine);
							sscanf(line+17+16*i, "%1d%1d", &tag1, &tag2);
							one_obs.obs.push_back(value_0);
							one_obs.tag1.push_back(tag1);
							one_obs.tag2.push_back(tag2);
						}
						one_record.GPS_obs.push_back(one_obs);
						prn_sum++;
					}*/
                    char prn[4];
                    RecGPSObs one_obs;
                    strncpy(prn, line,3);prn[3] = '\0';
                    one_record.hrd.sat_prn.push_back(prn);
                    double value_0;
                    int designated;
                    //int num_current_sys = return_num_of_sat(prn[0],header);
                    int num_current_sys = header.obs_types.return_designated_sum_obs_type(prn[0],designated);
                    for(int i = 0;i< num_current_sys;i++) {
                        strncpy(templine, line+3+16*i,14);templine[14] = '\0';
                        value_0 = atof(templine);
                        one_obs.obs.push_back((value_0));
                    }
                    one_record.GPS_obs.push_back(one_obs);
				}
				//count sum of GPS satellite only
				if(record_tag == true)
				{
					//one_record.hrd.sum_prn = prn_sum;
					record.push_back(one_record);
				}
			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

/*int return_num_of_sat(char prn, GMOHrd header) {
    int num = 0;
    for(int i =0;i< header.obs_types.obs_type.size();i++) {
        if(prn == header.obs_types.obs_type[i].system_type) {
            num = header.obs_types.obs_type[i].sum_obs_type;
            break;
        }
    }
    return num;
}*/
bool GMOReader::read_epoch_record(FILE *fp, GMORecord *epoch_record, GMOHrd header)
{
	char line[MaxLine+1], templine[MaxLine+1];
	int year, month, day, hour, minute,  epoch_tag,  sate_sum;
	double second;
	int sys_num;
	if(header.rinex_version < 3.0)
	{
		/*while(fgets(line, MaxLine, fp))
		{
			if(strstr(line, "COMMENT"))
			{
				continue;
			}
			if(strlen(line) < 2)
			{
				continue;
			}
			GMORecord one_record;
			//get the header of each epoch
			sscanf(line, "%d%d%d%d%d%lf%d%d", &year, &month, &day, &hour, &minute, &second,
				&epoch_tag, &sate_sum);
			one_record.hrd.epoch.Year	= year + 2000;
			one_record.hrd.epoch.Month	= month;
			one_record.hrd.epoch.Day	= day;
			one_record.hrd.epoch.Hour	= hour;
			one_record.hrd.epoch.Minute	= minute;
			one_record.hrd.epoch.Second = second;
			one_record.hrd.epoch_tag	= epoch_tag;
			//read comment line or satellite sum
			one_record.hrd.sum_sat		= sate_sum;

			if(epoch_tag != 0 && epoch_tag != 1)
			{
				cout<<"epoch tag is "<<epoch_tag<<endl;
				for(int i = 0;i<sate_sum;i++)
				{
					fgets(line, MaxLine, fp);
				}
				continue;
			}

			//get the observation type
			vector<char> sys;vector<int> prn;
			char a_sys;int a_prn;
			if(sate_sum <= 12)
			{
				for(int i = 0;i<sate_sum; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
			}
			else if(sate_sum <= 24)
			{
				for(int i = 0;i<12; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
				fgets(line, MaxLine, fp);
				for(int i = 0;i<sate_sum - 12; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
			}
			else if(sate_sum <= 36)
			{
				for(int i = 0;i<12; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
				fgets(line, MaxLine, fp);
				for(int i = 0;i<12; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
				fgets(line, MaxLine, fp);
				for(int i = 0;i<sate_sum - 24; i++)
				{
					sscanf(line+32+3*i, "%c%2d", &a_sys, &a_prn);
					sys.push_back(a_sys);
					prn.push_back(a_prn);
				}
			}
			else
			{
				cout<<"(read_GMO) the sum of observation is more than 36"<<endl;
				return 0;
			}

			//read observation
			int prn_sum = 0;
			int obs_sum = header.obs_types[0].sum_obs_type;
			int lines = (int)((obs_sum+4.5)/5.0);

			for(int sate_num = 0;sate_num<sate_sum;sate_num++)
			{
				RecGPSObs one_obs;

				if(sys[sate_num] == 'G') //read GPS data
				{
					one_record.hrd.gps_prn.push_back(prn[sate_num]);
					int tail_line_obs_sum, temp_line_obs_sum;

					if(lines == 1)
						tail_line_obs_sum = obs_sum;
					else
						tail_line_obs_sum = obs_sum - 5*(lines-1);

					for(int line_num = 1;line_num <= lines;line_num++)
					{
						if(!fgets(line, MaxLine, fp))
						{
							cout<<"(read_GMO) observation file format error!"<<endl;
							return 0;
						}
						if(line_num == lines)
							temp_line_obs_sum = tail_line_obs_sum;
						else
							temp_line_obs_sum = 5;

						int blank_obs_sum = 0;
						for(int i = 0;i<temp_line_obs_sum;i++)
						{
							int length = strlen(line);
							if(length < (16*i + 2))
							{
								temp_line_obs_sum = i;
								blank_obs_sum = temp_line_obs_sum - i + 1;
							}
						}

						for(int j = 0;j<temp_line_obs_sum;j++)
						{
							strncpy(templine, line+j*16, 14);templine[14] = '\0';
							one_obs.obs.push_back(atof(templine));
							strncpy(templine, line+j*16+14, 1);templine[1] = '\0';
							one_obs.tag1.push_back(atoi(templine));
							strncpy(templine, line+j*16+15, 1);templine[1] = '\0';
							one_obs.tag2.push_back(atoi(templine));
						}
						while(blank_obs_sum > 0)
						{
							one_obs.obs.push_back(0.0);
							one_obs.tag1.push_back(0);
							one_obs.tag2.push_back(0);
							blank_obs_sum--;
							;						}
					}
					prn_sum++;
					one_record.GPS_obs.push_back(one_obs);
				}
				else
				{
					for(int line_num = 0;line_num<lines;line_num++)
					{
						fgets(line, MaxLine, fp);
					}
				}
			}

			one_record.hrd.sum_prn = prn_sum;
			*epoch_record = one_record;
			return true;
		}*/
	}
	if(header.rinex_version >= 3.0)
	{
		while(fgets(line, MaxLine, fp))
		{
			if(strstr(line, "COMMENT"))
			{
				continue;
			}
			if(strlen(line) < 2)
			{
				continue;
			}
			if(line[0] == '>')
			{
				//get the header of each epoch
				GMORecord one_record;
				sscanf(line+2, "%d%d%d%d%d%lf%d%d", &year, &month, &day, &hour, &minute, &second,
					&epoch_tag, &sate_sum);
				one_record.hrd.epoch.Year	= year;
				one_record.hrd.epoch.Month	= month;
				one_record.hrd.epoch.Day	= day;
				one_record.hrd.epoch.Hour	= hour;
				one_record.hrd.epoch.Minute	= minute;
				one_record.hrd.epoch.Second = second;
				one_record.hrd.epoch_tag	= epoch_tag;
				//read comment line or satellite sum
				one_record.hrd.sum_sat		= sate_sum;

				if(epoch_tag != 0 && epoch_tag != 1)
				{
					cout<<"epoch tag is "<<epoch_tag<<endl;
					for(int i = 0;i<sate_sum;i++)
					{
						fgets(line, MaxLine, fp);
					}
					continue;
				}

				//get the observations of each epoch
				int prn_sum = 0;
				bool record_tag = true;
				for(int i = 0;i<sate_sum;i++)
				{
					if(!fgets(line, MaxLine, fp))
					{
						cout<<"(read_GMO) observation file format error!"<<endl;
						record_tag = false;
						break;
					}

					//read GPS data
					/*if(line[0] == 'G')
					{
						sys_num = 0;
						//read prn
						char prn[3];
						RecGPSObs one_obs;
						strncpy(prn, line+1, 2);prn[2] = '\0';
						one_record.hrd.gps_prn.push_back(atoi(prn));

						//read observation value
						double value_0;int tag1, tag2;
						int type_sum = header.obs_types[sys_num].sum_obs_type;
						for(int i = 0;i<type_sum;i++)
						{
							strncpy(templine, line+3+16*i, 14);templine[17] = '\0';
							value_0 = atof(templine);
							sscanf(line+17+16*i, "%1d%1d", &tag1, &tag2);
							one_obs.obs.push_back(value_0);
							one_obs.tag1.push_back(tag1);
							one_obs.tag2.push_back(tag2);
						}
						one_record.GPS_obs.push_back(one_obs);
						prn_sum++;
					}*/

                    char prn[4];
                    RecGPSObs one_obs;
                    strncpy(prn, line,3);prn[3] = '\0';
                    one_record.hrd.sat_prn.push_back(prn);
                    double value_0;
                    int designated;
                    int num_current_sys = header.obs_types.return_designated_sum_obs_type(prn[0],designated);
                    for(int i = 0;i< num_current_sys;i++) {
                        strncpy(templine, line+3+16*i,14);templine[14] = '\0';
                        value_0 = atof(templine);
                        one_obs.obs.push_back(value_0);
                    }
                    one_record.GPS_obs.push_back(one_obs);
				}
				//count sum of GPS satellite only
				if(record_tag == true)
				{
					//one_record.hrd.sum_prn = prn_sum;
					*epoch_record = one_record;
					return true;
				}
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

bool GMOReader::read_epochs_record(FILE *fp, GMJulianDay end_mjd, vector<GMORecord> &record, GMOHrd header)
{
	GMORecord one_record;
	if(!read_epoch_record(fp, &one_record, header))
		return false;
	UTTime first_epoch, last_epoch;
	first_epoch = one_record.hrd.epoch;
	record.push_back(one_record);

	while(read_epoch_record(fp, &one_record, header))
	{
		record.push_back(one_record);
		last_epoch = one_record.hrd.epoch;
		GMJulianDay mjd;
		CommonTimeToMJulianDay(&last_epoch, &mjd);
		if((end_mjd.intday - mjd.intday)*86400 + end_mjd.sod - mjd.sod <31)
			break;
	}
	return true;
}

int GMOReader::read_GMO(string filename, GMO *GMO_data)
{
	FILE *ofp;
	if ((ofp = fopen(filename.c_str(), "r")) == NULL)
	{
		return 0;
	}
	if (!read_GMO_head(ofp, &GMO_data->header))
	{
		return 0;
	}
	if (!read_GMO_record(ofp, GMO_data->record, GMO_data->header))
	{
		return 0;
	}
	fclose(ofp);
	return 1;
}

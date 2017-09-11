
#include "read_GMN.h"

double GMNReader::trans_exponent(char *line, int length, int integer_length)
{
	char templine[MAXLINE_n];
	double value_0;
	
	strncpy(templine, line, length);
	templine[length] = '\0';
	templine[integer_length] = 'E';
	value_0 = atof(templine);

	return value_0;
}

int GMNReader::read_GMN_header(FILE *fp ,GMNHrd *header)
{
	char line[MAXLINE_n + 1];
	double rinex_version;
	fgets(line, MAXLINE_n, fp);
	sscanf(line, "%lf", &rinex_version);
	strncpy(header->type, line+20, 20);
	header->rinex_version = rinex_version;
	
	if(rinex_version < 3.0)
	{
		while(fgets(line, MAXLINE_n, fp) && !feof(fp))
		{
			if(strstr(line, "COMMENT")) continue;
			if(strstr(line, "ION ALPHA"))
			{
				for(int i = 0;i<4;i++)
				{
					header->ion_alpha[i] = trans_exponent(line+2+12*i, 12, 8);
				}
			}
			else if(strstr(line, "ION BETA"))
			{
				for(int i = 0;i<4;i++)
				{
					header->ion_beta[i] = trans_exponent(line+2+12*i, 12, 8);
				}
			}
			else if(strstr(line, "DELTA-UTC"))
			{
				header->trans_a0 = trans_exponent(line+ 3, 19, 15);
				header->trans_a1 = trans_exponent(line+22, 19, 15);
				sscanf(line+41, "%d%d", &header->trans_t,&header->trans_w);
			}
			else if(strstr(line, "LEAP")) //leap seconds
			{
				sscanf(line, "%d", &header->leap_second);
			}
			else if(strstr(line, "END")) //end of header
			{
				break;
			}
		}
	}
	else
	{
		while(fgets(line, MAXLINE_n, fp) && !feof(fp))
		{
			if(strstr(line, "COMMENT")) continue;
			if(strstr(line, "IONOSPHERIC"))
			{
				if(line, strstr(line, "GPSA"))
				{
					for(int i = 0;i<4;i++)
					{
						header->ion_alpha[i] = trans_exponent(line+5+12*i, 12, 8);
					}
				}
				else if(line, strstr(line, "GPSB"))
				{
					for(int i = 0;i<4;i++)
					{
						header->ion_beta[i] = trans_exponent(line+5+12*i, 12, 8);
					}
				}
			}
			else if(strstr(line, "TIME SYSTEM")) //time system correction
			{
				if(strstr(line, "GAUT") || strstr(line, "GPUT"))
				{
					header->trans_a0 = trans_exponent(line+5, 17, 13);
					header->trans_a1 = trans_exponent(line+22, 16, 12);
					sscanf(line+38, "%7d%5d", &header->trans_t,header->trans_w);
				}
			}
			else if(strstr(line, "LEAP")) //leap second
			{
				sscanf(line, "%d", &header->leap_second);
			}
			else if(strstr(line, "END")) //end of header
			{
				break;
			}
		}
	}
	return 1;
}

int GMNReader::read_GMN_record(FILE *fp, vector<GMNRecord> &record, GMNHrd header)
{
	char line[MAXLINE_n+1];

	if(header.rinex_version < 3.0)
	{
		while(fgets(line, MAXLINE_n, fp) && !feof(fp))
		{
			if(strlen(line) < 2) continue;
			if(strlen(line) != 80)
			{
				cout<<"(read_GMN_record) navigation format error!"<<endl;
				return 0;
			}

			GMNRecord one_record;
			int year, month, day, hour, minute, prn;
			double second;
			sscanf(line, "%d%d%d%d%d%d%lf", &prn,&year,&month,&day,&hour,&minute,&second);
			one_record.sys = 'G';
			one_record.prn = prn;
			one_record.epoch.Year = year + 2000;
			one_record.epoch.Month = month;
			one_record.epoch.Day = day;
			one_record.epoch.Hour = hour;
			one_record.epoch.Minute = minute;
			one_record.epoch.Second = second;
			//SV clock
			one_record.clk_bias			= trans_exponent(line+22, 19, 15);
			one_record.clk_drift		= trans_exponent(line+41, 19, 15);
			one_record.clk_drift_rate	= trans_exponent(line+60, 19, 15);
			//broadcast channel - 1
			fgets(line, MAXLINE_n, fp);
			one_record.IODE = trans_exponent(line+ 3, 19, 15);
			one_record.Crs	= trans_exponent(line+22, 19, 15);
			one_record.dn	= trans_exponent(line+41, 19, 15);
			one_record.M0	= trans_exponent(line+60, 19, 15);
			//broadcast channel - 2
			fgets(line, MAXLINE_n, fp);
			one_record.Cuc		= trans_exponent(line+ 3, 19, 15);
			one_record.e		= trans_exponent(line+22, 19, 15);
			one_record.Cus		= trans_exponent(line+41, 19, 15);
			one_record.sqrtA	= trans_exponent(line+60, 19, 15);
			//broadcast channel - 3
			fgets(line, MAXLINE_n, fp);
			one_record.TOE		= trans_exponent(line+ 3, 19, 15);
			one_record.Cic		= trans_exponent(line+22, 19 ,15);
			one_record.omega	= trans_exponent(line+41, 19, 15);
			one_record.Cis		= trans_exponent(line+60, 19, 15);
			//broadcast channel - 4
			fgets(line, MAXLINE_n, fp);
			one_record.i0			= trans_exponent(line+ 3, 19, 15);
			one_record.Crc			= trans_exponent(line+22, 19, 15);
			one_record.omega_p		= trans_exponent(line+41, 19, 15);
			one_record.omega_dot	= trans_exponent(line+60, 19, 15);
			//broadcast channel - 5
			fgets(line, MAXLINE_n, fp);
			one_record.i_dot				= trans_exponent(line+ 3, 19, 15);
			one_record.code_on_channel_L2	= trans_exponent(line+22, 19, 15);
			one_record.GPSweek				= trans_exponent(line+41, 19, 15);
			one_record.L2_data_flag			= trans_exponent(line+60, 19, 15);
			//broadcast channel - 6
			fgets(line, MAXLINE_n, fp);
			one_record.SV_accuracy	= trans_exponent(line+ 3, 19, 15);
			one_record.SV_health	= trans_exponent(line+22, 19, 15);
			one_record.TGD			= trans_exponent(line+41, 19, 15);
			one_record.IODC			= trans_exponent(line+60, 19, 15);
			//broadcast channel - 7
			fgets(line, MAXLINE_n, fp);
			one_record.time_of_msg	= trans_exponent(line +3, 19, 15);
			one_record.fit_interval	= trans_exponent(line+22, 19, 15);

			int line_length = strlen(line);
			if(line_length <= 41)
			{
				one_record.spare1 = 0.0;
				one_record.spare2 = 0.0;
			}
			else if (line_length <= 60)
			{
				one_record.spare1 = trans_exponent(line+41, 19, 15);
				one_record.spare2 = 0.0;
			}
			else if (line_length > 60)
			{
				one_record.spare1		= trans_exponent(line+41, 19, 15);
				one_record.spare2		= trans_exponent(line+60, 19, 15);
			}

			record.push_back(one_record);
		}
	}
	else if(header.rinex_version >= 3.0)
	{
		while(fgets(line, MAXLINE_n, fp) && !feof(fp))
		{
			if(strlen(line) < 2) continue;
			if(strlen(line) != 80)
			{
				cout<<"(read_GMN_record) navigation format error!"<<endl;
				return 0;
			}

			GMNRecord one_record;
			int year, month, day, hour, minute, second;
			int prn;
			char sys;
			sscanf(line, "%c%2d%5d%3d%3d%3d%3d%3d", &sys,&prn,&year,&month,&day,&hour,&minute,&second);
			one_record.sys = sys;
			one_record.prn = prn;
			one_record.epoch.Year = year;
			one_record.epoch.Month = month;
			one_record.epoch.Day = day;
			one_record.epoch.Hour = hour;
			one_record.epoch.Minute = minute;
			one_record.epoch.Second = second;
			//SV clock
			one_record.clk_bias			= trans_exponent(line+23, 19, 15);
			one_record.clk_drift		= trans_exponent(line+42, 19, 15);
			one_record.clk_drift_rate	= trans_exponent(line+61, 19, 15);
			//broadcast channel - 1
			fgets(line, MAXLINE_n, fp);
			one_record.IODE = trans_exponent(line+ 4, 19, 15);
			one_record.Crs	= trans_exponent(line+23, 19, 15);
			one_record.dn	= trans_exponent(line+42, 19, 15);
			one_record.M0	= trans_exponent(line+61, 19, 15);
			//broadcast channel - 2
			fgets(line, MAXLINE_n, fp);
			one_record.Cuc		= trans_exponent(line+ 4, 19, 15);
			one_record.e		= trans_exponent(line+23, 19, 15);
			one_record.Cus		= trans_exponent(line+42, 19, 15);
			one_record.sqrtA	= trans_exponent(line+61, 19, 15);
			//broadcast channel - 3
			fgets(line, MAXLINE_n, fp);
			one_record.TOE		= trans_exponent(line+ 4, 19, 15);
			one_record.Cic		= trans_exponent(line+23, 19 ,15);
			one_record.omega	= trans_exponent(line+42, 19, 15);
			one_record.Cis		= trans_exponent(line+61, 19, 15);
			//broadcast channel - 4
			fgets(line, MAXLINE_n, fp);
			one_record.i0			= trans_exponent(line+ 4, 19, 15);
			one_record.Crc			= trans_exponent(line+23, 19, 15);
			one_record.omega_p		= trans_exponent(line+42, 19, 15);
			one_record.omega_dot	= trans_exponent(line+61, 19, 15);
			//broadcast channel - 5
			fgets(line, MAXLINE_n, fp);
			one_record.i_dot				= trans_exponent(line+ 4, 19, 15);
			one_record.code_on_channel_L2	= trans_exponent(line+23, 19, 15);
			one_record.GPSweek				= trans_exponent(line+42, 19, 15);
			one_record.L2_data_flag			= trans_exponent(line+61, 19, 15);
			//broadcast channel - 6
			fgets(line, MAXLINE_n, fp);
			one_record.SV_accuracy	= trans_exponent(line+ 4, 19, 15);
			one_record.SV_health	= trans_exponent(line+23, 19, 15);
			one_record.TGD			= trans_exponent(line+42, 19, 15);
			one_record.IODC			= trans_exponent(line+61, 19, 15);
			//broadcast channel - 7
			fgets(line, MAXLINE_n, fp);
			one_record.time_of_msg	= trans_exponent(line +3, 19, 15);
			one_record.fit_interval	= trans_exponent(line+23, 19, 15);

			int line_length = strlen(line);
			if(line_length <= 41)
			{
				one_record.spare1 = 0.0;
				one_record.spare2 = 0.0;
			}
			else if (line_length <= 60)
			{
				one_record.spare1 = trans_exponent(line+42, 19, 15);
				one_record.spare2 = 0.0;
			}
			else if (line_length > 60)
			{
				one_record.spare1		= trans_exponent(line+42, 19, 15);
				one_record.spare2		= trans_exponent(line+61, 19, 15);
			}

			record.push_back(one_record);
		}
	}
	return 1;
}

int GMNReader::read_GMN(char* filename, GMN *GMN_data)
{
	FILE *ofp;
	if((ofp = fopen(filename, "r")) == NULL)
	{
		return 0;
	}
	if(!read_GMN_header(ofp, &GMN_data->header))
	{
		return 0;
	}
	if(!read_GMN_record(ofp, GMN_data->record, GMN_data->header))
	{
		return 0;
	}

	fclose(ofp);
	return 1;
}

#include "read_dcb.h"

bool read_dcb(string filename, DCB *dcb_data)
{
	FILE* fp;
	char line[200+1];
	if((fp = fopen(filename.c_str(), "r")) == NULL)
		return false;

	//read file header
	for(int i = 0;i<2;i++)
		fgets(line, 200, fp);
	//read file record
	int prn = 0;double dcb = 0.0;
	while(fgets(line, 200, fp))
	{
		if(line[0] == '*')
		{
			while(fgets(line, 200, fp))
			{
				char templine[30];
				strncpy(templine, line, 4);templine[4] = '\0';
				dcb_data->sitename.push_back(templine);
				strncpy(templine, line+4, 26);templine[26]='\0';
				dcb_data->site_dcb.push_back(atof(templine));
			}
		}
		else
		{
			sscanf(line+2, "%2d%lf", &prn, &dcb);
			dcb_data->sate_dcb[prn - 1] = dcb;
		}
	}

	fclose(fp);
	return true;
}

bool get_sate_dcb(int prn, DCB dcb_data, double *dcb)
{
	double dcb_value = dcb_data.sate_dcb[prn-1];
	if(dcb_value > 100)
		return false;
	*dcb = dcb_value;
	return true;
}

bool get_site_dcb(string name, DCB dcb_data, double *dcb)
{
	vector<string> :: iterator item;
	item = find(dcb_data.sitename.begin(), dcb_data.sitename.end(), name);
	if(item == dcb_data.sitename.end())
		return false;
	int num = item - dcb_data.sitename.begin();
	*dcb = dcb_data.site_dcb[num];
	return true;
}

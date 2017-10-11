#include"preprocess.h"
#include<io.h>

void getFile(string path, vector<string>& files);

int main(int argc, char** argv) {
    /*GMO GMO_data;
    cout<<GMOReader::read_GMO(string(argv[1]), &GMO_data)<<endl;

    GMN GMN_data;
    cout <<GMNReader::read_GMN(argv[2], &GMN_data)<<endl;*/
    vector<string> obslst;
    path = string(argv[1]);

    vector<TEC> Tecs;
    int status = preprocess(obslst, Tecs);
    cout<<status<<endl;

}

void getFile(string path, vector<string>& files) {
    long hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    if(hFile = _findfirst(p.assign(path).c_str(), &fileinfo) != -1) {
        do {
            if(fileinfo.attrib)
        }
    }


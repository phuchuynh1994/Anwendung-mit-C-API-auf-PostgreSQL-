#include <iostream>
#include <libpq-fe.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include "db.h"


using namespace std;

void print_brief_description(){
    cout << "Usage :"<<endl;
    cout <<"\t dbimp    [Option] <infile>"<<endl;
    cout << "Options:"<<endl;
    cout <<"\t -del  delete table contents before import"<<endl;
    cout <<"\t -u database user"<<endl;
    cout <<"\t -p password"<<endl;
    cout <<"\t -h database host"<<endl;
    cout <<"\t -d database name"<<endl;
}

vector<string> tokenizer(const string & str, const string & delimiters) {

    vector<string> tokens;
    size_t lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    size_t pos = str.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters. Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
    
    return tokens;
}

void print_statistic(const string& command,int anzahl){
    cout << command <<" | " << anzahl <<" | "<< db_get_tuple() <<endl;
}
int main(int argc, char* argv[]) {
	if (argc ==1)
	{
		print_brief_description();
		exit(1);
	}
    string PQ_user,PQ_database_name,PQ_password, PQ_host;
    string path;
    int error;
    int del_signal = 0;
    int i;
    string tmp = argv[1];
    if (tmp.at(0) == '-'&& argc >=2) {
        for (i = 1; i < argc - 1; i++) {
            tmp = argv[i];
            if (tmp == "-u")
                PQ_user = argv[i + 1];
            else if (tmp == "-p")
                PQ_password = argv[i + 1];
            else if (tmp == "-d")
                PQ_database_name = argv[i + 1];
            else if (tmp == "-h")
                PQ_host = argv[i + 1];
            else if (tmp == "-del")
                del_signal = 1;
            else if (tmp.at(0) == '-' && tmp !="-u" && tmp!="-p" && tmp != "-del" && tmp != "-h") {
                cout << "input Option is wrong!" <<endl;
                print_brief_description();
                exit(1);
            }

            }
        string fre_path= argv[argc -2];
        path = argv[argc - 1];
            if (path.at(0) == '-'|| (fre_path.at(0) == '-' && fre_path !="-del")) {
                cout << "there is no import file !"<<endl;
                print_brief_description();
                exit(1);
        }

    }
    else
    {
        PQ_user ="postgres";
        PQ_database_name= "praktikum2";
        PQ_host="localhost";
        PQ_password= "thienthanh";
        path = argv[1];
        if ( argc > 3)
        {
            print_brief_description();
            exit(1);
        }
    }
    if (del_signal == 1 && (PQ_user == "") && PQ_database_name =="" && PQ_password =="" && PQ_host=="" )
    {
        PQ_user ="postgres";
        PQ_database_name= "praktikum2";
        PQ_host="localhost";
        PQ_password= "thienthanh";
    }
    //cout << path <<endl;
    error = db_login(PQ_user,PQ_password,PQ_database_name);
    //cout << error << endl;
    if (error == 0)
    {
        db_logout();
    }
    if ( del_signal == 1)
    {
        cout << "delete all data!" <<endl;
        db_begin();
        error = db_delete("herstellers");
        if (error == 0)
            db_rollback();
        else db_commit();
    }

    /// daten von importdatei verarbeiten
    fstream infile;
    infile.open(path.c_str(),ios::in);
    string line,data_raw;
    if (infile.fail())
    {
        cout << "import file does not exist!"<<endl;
        db_logout();
    }
    while( !infile.eof())
    {
        getline(infile,line);
        data_raw +=line;
        data_raw +=";";
    }
    infile.close();
    cout <<data_raw<<endl;
    vector<string> data = tokenizer(data_raw,";");
    // process transaction
    cout << "avaadad"<<endl;
    db_begin();
    int n = 0;
    for ( i= 0; i< data.size(); i = i + 4)
    {
        error = db_insert(data[i],data[i+1],data[i+2],data[i+3]);
        if ( error == -1 )
        {
            cout << "import process failed !" << endl;
            db_rollback();
            n = 0;
            break;
        }
        else if ( error == 0)
            n++;

    }
    if (error == 0|| error == 1)
    {
        db_commit();
        cout << "import process success!" <<endl;

    }
    print_statistic(path,n);


    return 0;

}

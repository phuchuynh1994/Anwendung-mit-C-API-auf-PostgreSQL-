//
// Created by huynh on 6/6/17.
//
#include <iostream>
#include <libpq-fe.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "db.h"

PGconn* conn;
void db_logout() {

    PQfinish(conn);
    exit(1);
}
int db_login(const string &user, const string &password, const string &dbname){

    string conn_string;
    conn_string += "user=";
    conn_string += user;
    conn_string += " ";
    conn_string += "password=";
    conn_string += password;
    conn_string += " ";
    //conn_string += "host=";
    //conn_string += host;
    conn_string += "dbname=";
    conn_string += dbname;
    conn = PQconnectdb(conn_string.c_str());
    if (PQstatus(conn) == CONNECTION_BAD){
        fprintf( stderr, "connection to database failed : %s\n", PQerrorMessage(conn));
        return 0;
    }
    else return 1;

}

int db_begin(){
    PGresult *res =  PQexec(conn,"BEGIN");
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        cout<< "begin command failed!"<<endl;
        PQclear(res);
        return 0;
    }
    cout << "Begin" <<endl;
    PQclear(res);
    return 1;

}
int db_commit(){
    PGresult *res = PQexec(conn,"COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        cout << "commit command failed!"<< endl;
        PQclear(res);
        return 0;
    }
    cout << "Commit" <<endl;
    PQclear(res);
    return 1;
}
int db_rollback(){
    PGresult *res = PQexec(conn,"ROLLBACK");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        cout << "rollback command failed!"<< endl;
        PQclear(res);
        return 0;
    }
    cout << "rollback" <<endl;
    PQclear(res);
    return 1;
}


int db_checkhnr(const string &hnr){
    PGresult *res;
    string query;
    query += "select hnr from herstellers where hnr='";
    query += hnr + "'";
    //cout << query <<endl;
    res = PQexec(conn,query.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        cout << "query command failed" << endl;
        PQclear(res);
        return -1;
    }

    int rows = PQntuples(res);
    if (rows == 0)
    {
        PQclear(res);
        return 0;
    }
    else{
         PQclear(res);
        return 1;
    }
}


int db_insert(const string &mtnr, const string &vorname, const string &nachname, const string &geburt){
    int check = db_checkhnr(mtnr);
    string query;
    query +="insert into herstellers values('";
    query += mtnr +"','" + vorname +"','"+nachname + "','" + geburt + "')";
    cout << query << endl;
    if(check == 1)
    {
        cout << " duplicate key !" <<endl;
        return 1;
    }
    if (check == -1)
    {
		cout <<"error"<<endl;
	}
	

    PGresult *res = PQexec(conn, query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {

            printf("INSERT command failed\n");
            PQclear(res);
            return -1;

        }
        PQclear(res);
        cout << "0 1" <<endl;
        return 0;

}

int db_delete(const string& table_name){
    string query;
    query += "TRUNCATE TABLE ";
    query += table_name;
    cout << query <<endl;
    PGresult *res = PQexec(conn,query.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        cout << "truncate command failed!" <<endl;
        PQclear(res);
        return 0;
    }
    else
    {
        PQclear(res);
        return 1;
    }
}

int db_get_tuple(){
    PGresult * res = PQexec(conn,"select hnr from herstellers ");
    int anzahl = PQntuples(res);
    return  anzahl;
}

//
// Created by huynh on 6/6/17.
//

#ifndef UNTITLED1_DB_H
#define UNTITLED1_DB_H

#include <string>
using namespace std;
// Datenbank-Login
// rc: 0 = ok, 1 = error
int db_login(const string &user, const string &password, const string &dbname);

// Datenbank-Logout
void db_logout();

// Transaktionsbefehle
// rc: 0 = ok, 1 = error
int db_begin();
int db_commit();
int db_rollback();

// Herstellernummer schon vorhanden?
// rc: 0 = noch nicht da, 1 = schon da, -1 = error
int db_checkmtnr(const string &mtnr);

// Einfuegen Datensatz
// rc: 0 = ok, 1 = error
int db_insert(const string &hnr, const string &name, const string &plz, const string &ort);

// Loeschen des kompletten Tabelleninhalts
// rc: 0 = ok, 1 = error
int db_delete(const string& table_name);
int db_get_tuple();





#endif //UNTITLED1_DB_H

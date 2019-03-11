#include "stdafx.h"
#include "db.h"
#include <fstream>

//constructor
db::db(string filename) {
	string head = "./testdb/";
	string _idx = ".idx"; string _dat = ".dat";
	string idfilename = head+filename + _idx; string dtfilename = head+filename + _dat;
	_filename = dtfilename;
	db_index.read(idfilename);

}

//get the value of a certain key
string db::get(string key) {
	int* digit = new int;
	int ptr;
	ptr = db_index.get(key, digit);
	fstream datfile(_filename.c_str(), ios::in|ios::out|ios::binary);
	datfile.seekg(ptr, ios::beg);
	stringstream value_s;
	char* buffer=new char;
	for (int i = 0;i < *digit;i++) {
		datfile.read(buffer, sizeof(char));
		value_s << *buffer;
	}
	datfile.close();
	return value_s.str();
}

//add a (key,value) to the database,if key existed, return false
bool db::put(string key,string value) {
	fstream datfile(_filename.c_str(), ios::in|ios::out|ios::binary);
	datfile.seekp(0, ios::end);
	int ptr = datfile.tellp();
	int digit = value.size();
	bool flag=db_index.put(key, ptr, digit);
	if (!flag) { return false; }
	datfile.write(value.c_str(),value.size());
	datfile.close();
	return true;
}

//delete a (key,value) from the database
bool db::remove(string key) {
	int* digit=new int;
	int ptr;
	ptr = db_index.get(key, digit);
	if (ptr == -1) { return false; }
	fstream datfile(_filename.c_str(), ios::in|ios::out|ios::binary);
	datfile.seekp(ptr, ios::beg);
	char* space = new char(' ');
	for (int i = 0;i < *digit;i++) {
		datfile.write(space, sizeof(char));
	}
	db_index.remove(key);
	return true;
}

void db::refresh() {
	this->db_index.rewrite();
}

void db::defragment() {
	ofstream temp_idx("./testdb/temp.idx");
	ofstream temp_dat("./testdb/temp.dat");
	temp_idx.close();temp_dat.close();
	db tempdb("temp");
	string temp_key, temp_value;
	while (true) {
		temp_key = (this->db_index).getkey();
		if (temp_key == "") { break; }
		int digit;
		int ptr;
		stringstream keystream(temp_key);
		keystream >> temp_key>>ptr>>digit;
		fstream datfile(_filename.c_str(), ios::in | ios::out | ios::binary);
		datfile.seekg(ptr, ios::beg);
		stringstream value_s;
		char* buffer = new char;
		for (int i = 0;i < digit;i++) {
			datfile.read(buffer, sizeof(char));
			value_s << *buffer;
		}
		datfile.close();
		temp_value=value_s.str();
		tempdb.put(temp_key, temp_value);
	}
	tempdb.refresh();
}
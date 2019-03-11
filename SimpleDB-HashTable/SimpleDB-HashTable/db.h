#pragma once
#include "stdafx.h"
#include "Hashtable.h"



class db {
private:
	HashTable db_index;
	string _filename;
public:
	db(string filename);
	string get(string key);
	bool remove(string key);
	bool put(string key, string value);
	void refresh();
	void defragment() ;
};
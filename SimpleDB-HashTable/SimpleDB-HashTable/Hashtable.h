#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <stdio.h>
using namespace std;

int hashx(string s, int M);

class HashTable {
private:
	int _size;
	int _length;
	vector<list<string> > _htable;
	string filename;
	int _vec_er_it=0;
	list<string>::iterator _list_er_it;
	int _listflag = 0;
	fstream* idx;
public:
	HashTable(int length = 199);
	~HashTable();
	void read(string idfile);
	bool remove(string key);
	int get(string key,int* digit);
	bool put(string key,int pointer, int digit);
	void rewrite() ;
	string getkey();
};

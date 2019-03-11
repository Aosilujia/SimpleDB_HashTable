// SimpleDB-HashTable.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Hashtable.h"
#include "db.h"
#include <iostream>
#include <time.h>

//read num data into the database
void naivereadin(int num) {
	string x1 = "./testdb/hdb1000000.idx";
	string x2 = "./testdb/hdb1000000.dat";
	fstream file1(x1.c_str(), ios::in | ios::out | ios::binary);
	fstream file2(x2.c_str(), ios::in | ios::out | ios::binary);
	file1.seekp(0, ios::end);
	file2.seekp(0, ios::end);
	char c;
	ifstream dic("EnglishWords.txt");
	ifstream val("TomSawyer.txt");
	for (int i = 0;i < num;i++) {
		string key;string value;
		dic >> key;
		val >> value;
		file2.write(value.c_str(), value.size());
		int length = file1.tellp();
		file1 << hashx(key, 1002077) <<" "<< key << " " << length <<" "<< value.size() << "\n";
	}
	return;
}

//performance test
void test(string filename, int num) {
	db database(filename);
	vector<string> records;
	ifstream dic("EnglishWords.txt");
	ifstream dic2("EnglishWords_clone.txt");
	ifstream val("TomSawyer.txt");
	string value, key1, key2;
	//stage 2
	for (int i = 0;i < num-2;i++) {
		dic >> key1;
		val >> value;
	}
	int pos = dic.tellg();
	//stage 3
	int c1=0, c2=0, c3 = 0;
	for (int j = 0;j < num * 5;j++) {
		dic2 >> key2;
		if (dic2.tellg() >= pos) { dic2.seekg(0, ios::beg); }
		if (c1 >= 36) {
			dic2 >> key2;
			database.remove(key2);
			c1 = 0;
		}
		else { c1++; }
		if (c2 >= 10) {
			dic >> key1;val >> value;
			database.put(key1, value);
			records.push_back(key1);
			c2 = 0;
		}
		else { c2++; }
		if (c3 == 16) {
			c3++;
			dic2 >> key2;
			int slength = database.get(key2).size();
			database.remove(key2);
			val >> value;
			string value_temp = "";
			for (int c = 0;c < slength;c++) {
				if (c >= value.size()) { value_temp += "a";continue; }
				value_temp += value[c];
			}
			database.put(key2, value_temp);
			records.push_back(key2);
		}
		if (c3 == 33) {
			c3 = 0;
			dic2 >> key2;
			int slength = database.get(key2).size();
			database.remove(key2);
			val >> value;
			string value_temp = "";
			if (value.size() > slength) { database.put(key2, value); records.push_back(key2);
			}
			else {
				for (int c = 0;c < slength + 2;c++) {
					if (c >= value.size()) { value_temp += "a";continue; }
					value_temp += value[c];
				}
				database.put(key2, value_temp);
				records.push_back(key2);
			}
		}
	}
	//stage 4
	vector<string>::iterator it;
	for (it = records.begin();it != records.end();it++) {
		database.remove(*it);
	}
	return;
}

void test1() {
	string db_name = "hdb10000";
	db database(db_name);
	string key, value;
	while (true) {
		cin >> key;cin >> value;
		database.put(key, value);
		cout<<database.get(key);
	}
}

void test2() {
	string db_name = "hdb10000";
	db database(db_name);
	database.defragment();
}

void readin(string filename, int num) {
	db database(filename);
	ifstream dic("EnglishWords.txt");
	ifstream val("TomSawyer.txt");
	for (int i = 0;i < num;i++) {
		string key;string value;
		dic >> key;
		val >> value;
		database.put(key, value);
	}
	database.refresh();
	return;
}


int main()
{
	clock_t start, finish;
	start = clock();
	string db_name = "hdb10000";
	db database(db_name);
	database.remove("abc");
	cout<<database.get("abc");
	finish = clock();
	cout << double(finish - start);
    return 0;
}


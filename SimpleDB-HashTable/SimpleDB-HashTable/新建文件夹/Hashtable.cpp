#include "stdafx.h"
#include "Hashtable.h"


/*the string to int hash function*/
int hashx(string s, int M) {
	char*y = new char[strlen(s.c_str()) + 1];
	strcpy(y, s.c_str());
	unsigned long h = 0;
	unsigned long x = 0;

	while (*y)
	{
		h = (h << 4) + (*y++);
		if ((x = h & 0xF0000000L) != 0)
		{ //如果最高位不为0，则说明字符多余7个，如果不处理，再加第九个字符时，第一个字符会被移出
		  //因此要有如下处理
			h ^= (x >> 24);
			//清空28~31位
			h &= ~x;
		}
	}
	return h%M;
}

//constructor
HashTable::HashTable(int length){
	_htable.resize(length);
	_length = length;
	_size = 0;
}

HashTable::~HashTable() {
	if (rewrite_flag == 1) {
		rewrite();
	}
}
//read in the index file
void HashTable::read(string idfile) {
	filename = idfile;
	ifstream indexfile(idfile.c_str());
	int tablelength;
	string _line;
	if (getline(indexfile,_line)) {
		stringstream ss(_line);
		ss >> tablelength;
		_length = tablelength;
		_htable.resize(_length);
	}
	while (getline(indexfile, _line)) {
		stringstream ss1(_line);
		stringstream ss2;
		int hashvalue;
		string key;int ptr;int digit;
		ss1 >> hashvalue>>key>>ptr>>digit;
		ss2 << key << " " << ptr << " " << digit;
		_htable[hashvalue].push_back(ss2.str());
		_size += 1;
	}
	return;
};

//get the pointer of a value linked to the key
int HashTable::get(string key,int* digit) {
	int ptr = hashx(key, _length);
	list<string> se_chain = _htable[ptr];
	list<string>::iterator chain_it;
	if (se_chain.size() == 0) { return -1; }
	else {
		for (chain_it = se_chain.begin();chain_it != se_chain.end();chain_it++) {
			stringstream* ss=new stringstream(*chain_it);
			string _key;
			*ss >> _key;
			if (_key == key) {
				int pointer;
				*ss >> pointer;
				*ss >> *digit;
				return pointer;
			}
			delete ss;
		}
	}
	return -1;
}

//add a key to the hashtable,if key existed, return false
bool HashTable::put(string key, int pointer, int digit) {
	stringstream stringkey;
	stringkey << key << " " << pointer << " " << digit;
	int ptr = hashx(key, _length);
	list<string>::iterator chain_it;
	if (_htable[ptr].size() != 0) {
		for (chain_it = _htable[ptr].begin();chain_it != _htable[ptr].end();chain_it++) {
			stringstream* ss = new stringstream(*chain_it);
			string _key;
			*ss >> _key;
			if (_key == key) {
				return false;
			}
			delete ss;
		}
	}
	_htable[ptr].push_back(stringkey.str());
	fstream idx(filename.c_str(),ios::in|ios::out|ios::end);
	idx.seekp(0, ios::end);
	idx << ptr << " " << stringkey.str()<<"\n";
	idx.close();
	rewrite_flag = 1;
	return true;
}

//delete a key from the hashtable
bool HashTable::remove(string key) {
	int ptr = hashx(key, _length);
	list<string>::iterator chain_it;
	if (_htable[ptr].size() != 0) {
		for (chain_it = _htable[ptr].begin();chain_it != _htable[ptr].end();chain_it++) {
			stringstream* ss = new stringstream(*chain_it);
			string _key;
			*ss >> _key;
			if (_key == key) {
				_htable[ptr].erase(chain_it);
				rewrite_flag = 1;
				return true;
			}
			delete ss;
		}
	}
	return false;
}

/*this function gets the largest primenumber smaller than x*/
int getminpnum(int x) {
	string filename = "PrimeNumbers.txt";
	ifstream primenumber(filename.c_str());
	int M = 0;
	while (true) {
		int c;
		primenumber >> c;
		if (c <= x) { M = c; }
		else break;
	}
	return M;
}

void HashTable::rewrite() {
	rewrite_flag = 0;
	vector<list<string> >::iterator hash_it;
	list<string>::iterator list_it;
	if (_htable.size() == 0) { return; }
	if (_size <= 2 * _length) { 
		ofstream idx_file(filename.c_str());
		idx_file << _length << "\n";
		for (int c = 0;c < (_htable.size());c++) {
			if (_htable[c].size() == 0) { continue; }
			for (list_it = _htable[c].begin();list_it != _htable[c].end();list_it++) {
				idx_file << c << " " << *list_it << "\n";
			}
		}
		idx_file.close();
		return;
	}
	int neolength = _size / 2;
	ifstream primenum("PrimeNumbers.txt");
	while (true) {
		int i;
		primenum >> i;
		if (i > neolength) {
			_length = i;
			break;
		}
	}
	primenum.close();
	ofstream idx_file(filename.c_str());
	idx_file << _length<<"\n";
	for (hash_it = _htable.begin();hash_it != _htable.end();hash_it++) {
		if (hash_it->size() == 0) { continue; }
		for (list_it = hash_it->begin();list_it != hash_it->end();list_it++) {
			stringstream ss(*list_it);
			string _key;
			ss >> _key;
			int pos = hashx(_key, _length);
			idx_file << pos << " " << *list_it << "\n";
		}
	}
	idx_file.close();
	return;
}

string HashTable::getkey() {
	if (_vec_er_it == _htable.size()) { return ""; }
	while (_vec_er_it != _htable.size()) {
		if (_htable[_vec_er_it].size() == 0) { _vec_er_it++;continue; }
		if (_listflag == 0) {
			_list_er_it = _htable[_vec_er_it].begin();
			_listflag = 1;
		}
		if (_list_er_it == _htable[_vec_er_it].end()) { _vec_er_it++;_listflag = 0;continue; }
		string x= *_list_er_it;
		_list_er_it++;
		return x;
	}
	return "";
}
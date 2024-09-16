#include<iostream>
#include<string>
#include<cassert>
#include<vector>

using namespace std;


struct StringHasher {
	unsigned int str_hash1(const string& key) const {
		unsigned int hash = 0;
		for (auto i : key) {
			hash = hash * 13 + i;
		}
		return hash;
	}

	unsigned int str_hash2(const string& key) const {
		unsigned int hash = 0;
		for (auto i : key) {
			hash = hash * 7 + i;
		}
		hash = hash * 2 + 1;
		return hash;
	}

	unsigned int operator()(const string& key, int i) const {
		unsigned int hash = str_hash1(key) + (i * str_hash2(key));
		return hash;
	}
};

template<class J>
struct DefaultEqual {
	bool operator()(const J& l, const J& r) const {
		return l == r;
	}
};

template<class T, class H = StringHasher, class Equal = DefaultEqual<T>>
class HashTable
{
public:
	//Все ячейки в состоянии Empty
	HashTable(H hash = H(), Equal equal = Equal()):
		table(8),
		hash(hash),
		equal(equal),
		keysCount(0) {}


	
	~HashTable() = default;

	HashTable(const HashTable&) = delete;

	HashTable& operator=(const HashTable&) = delete;

	bool Has(const T& key) 
	{
		unsigned int idx;
		for (int i = 0; i < table.size(); i++) {
			idx = hash(key, i) % table.size();
			if (table[idx].State == HashTableCell::CellState::Deleted)
				continue;
			else if (table[idx].State == HashTableCell::CellState::Data) {
				if (equal(table[idx].Key, key))
					return true;
				else
					continue;
			}
			else if (table[idx].State == HashTableCell::CellState::Empty)
				return false;
		}
		return false;
	}
	

	bool Add(const T& key)// buckets = table //// data_empty_del = CellState //// rm = Deleted /// empty = Empty 
		//// data = Data в enum   //// .key = .Key //// data_empty_del = CellState //// .status = .State
	{
		bool flag = true;
		size_t pos = 0;
		unsigned int idx;
		for (int i = 0; i < table.size(); i++) {
			idx = hash(key, i) % table.size();
			if (table[idx].State == HashTableCell::CellState::Deleted) {
				if (flag) {
					pos = idx;
					flag = false;
				}
				continue;
			}
			else if (table[idx].State == HashTableCell::CellState::Data) {
				if (equal(table[idx].Key, key))
					return false;
				continue;
			}
			else if (table[idx].State == HashTableCell::CellState::Empty) {
				if (!flag) {
					table[pos].Key = key;
					table[pos].State = HashTableCell::CellState::Data;
				}
				else {
					table[idx].Key = key;
					table[idx].State = HashTableCell::CellState::Data;
				}
				keysCount++;
				if (keysCount >= (table.size() *3/4))
					growTable();
				return true;
			}
		}
		table[pos].Key = key;
		table[pos].State = HashTableCell::CellState::Data;
		keysCount++;
		if (keysCount >= (table.size() *3/4))
			growTable();
		return true;
	}
	
	bool Delete(const T& key)
	{
		unsigned int idx;
		for (int i = 0; i < table.size(); i++) {
			idx = hash(key, i) % table.size();
			if (table[idx].State == HashTableCell::CellState::Deleted)
				continue;
			else if (table[idx].State == HashTableCell::CellState::Data) {
				if (equal(table[idx].Key, key)) {
					keysCount--;
					table[idx].State = HashTableCell::CellState::Deleted;
					return true;
				}
				else
					continue;
			}
			else if (table[idx].State == HashTableCell::CellState::Empty)
				return false;
		}
		return false;
	}
private:	
	struct HashTableCell {
		enum CellState { Empty, Data, Deleted };
		T Key; // значение ключа
		unsigned int Hash;//для перехеширования и для пробирования
		CellState State;
		HashTableCell() : Hash(0), State(Empty), Key(T()){}
		
	};

	vector<HashTableCell> table;//основной массив т.н.
	int keysCount;
	H hash;
	Equal equal;
	
	void growTable() {
		vector<HashTableCell> newTable(table.size() * 2);
		vector<HashTableCell> temp(table);
		this->table = move(newTable);
		this->keysCount = 0;
		for (int i = 0; i < temp.size(); i++) {
			if (temp[i].State == HashTableCell::CellState::Data)
				Add(temp[i].Key);
		}
	}
};









int main()
{
	StringHasher hasher;
	HashTable<string> table;
	char operation = 0;
	string word;
	while (cin >> operation >> word) {
		switch (operation) {
			case '+':
				cout << (table.Add(word) ? "OK" : "FAIL") << endl;
				break;
			case '-':
				cout << (table.Delete(word) ? "OK" : "FAIL") << endl;
				break;
			case '?':
				cout << (table.Has(word) ? "OK" : "FAIL") << endl;
				break;
			default:
				assert(false);

		}
	}
	return 0;
}



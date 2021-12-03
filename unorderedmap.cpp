// Below are the stubs for SOME methods
// Make sure all methods in unorderedmap.h are implemented
#include "unorderedmap.h"

//constructor method
template <typename KEY, typename T, typename H>
UnorderedMap<KEY, T, H>::UnorderedMap(){
	tablesize = 101;
	entrysize = 0;
	array = new Entry *[tablesize]{};
	//Entry *end = nullptr;
}

//destructor method
template <typename KEY, typename T, typename H>
UnorderedMap<KEY, T, H>::~UnorderedMap(){
	Entry *temp;
	Entry *prev;
	for (int i = 0; i < tablesize; i++) {
		if (array[i] != NULL) {
			temp = array[i];
			while (temp != nullptr) {
				prev = temp;
				temp = temp -> next;
				delete prev;
			}
		}
		else {
			delete array[i];
		}
	}
	delete[] array;
}

template <typename KEY, typename T, typename H>
UnorderedMap<KEY, T, H>::UnorderedMap(const UnorderedMap& rhs){
	tablesize = rhs.tablesize;
	entrysize = rhs.entrysize;
	array = new Entry *[tablesize];

	for (int i = 0; i < tablesize; i++) {
		array[i] = rhs.array[i];
	}
}

//== method
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::UnorderedMap& UnorderedMap<KEY, T, H>::operator=(const UnorderedMap& rhs) {
	if (this == &rhs) {
		return *this;
	}

	tablesize = rhs.tablesize;
	entrysize = rhs.entrysize;

	for (int i = 0; i < tablesize; i++) {
		array[i] = rhs.array[i];
	}

	return *this;
}

//insert method
template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::insert(const KEY & newkey, const T & data){
	std::size_t index = H()(newkey); 
	index = index % tablesize;
	// if (entrysize >= tablesize/2) {
	// 		rehash();
	// 	}
	if (array[index] == NULL) {
		Entry *ins = new Entry(newkey, data);
		array[index] = ins;
		array[index] -> next = nullptr;
	}
	else {
		Entry *entry = array[index];
		while (entry != NULL) {
			if (entry -> key == newkey) {
				return false;
			}
			else {
				entry = entry -> next;
			}
		}
		Entry *ins = new Entry(newkey, data);
		ins -> next = array[index];
		array[index] = ins;
	}
	entrysize++;
	return true;
}

//erase method
template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::erase(const KEY& key){
	if (find(key) == end()) {
		return false;
	}
	else {
		std::size_t index = H()(key); 
		index = index % tablesize;
		Entry *prev = nullptr;
		Entry *temp = array[index];
		while (temp != nullptr && temp -> key != key) {
			prev = temp;
			temp = temp -> next;
		}
		if (prev == nullptr) {
			array[index] = temp -> next;
		}
		else {
			prev -> next = temp -> next;
		}
		delete temp;
	}
	entrysize--;

	return true;
}

//rehash method
template <typename KEY, typename T, typename H>
void UnorderedMap<KEY, T, H>::rehash(){
	int newTableSize = nextPrime(tablesize * 2);
	Entry** newArray = new Entry*[newTableSize];

	delete[] array;

	array = newArray;

	delete[] newArray;
	
	// for (int i = 0; i < newTableSize; i++) {
	// 	Entry *entry = array[i];
	// 	while (entry != nullptr) {
	// 		Entry *temp = entry;
	// 		temp = temp -> next;
	// 		std::size_t index = H()(array[i] -> key); 
	// 		index = index % tablesize;
	// 		Entry*& bucket = newArray[index];
	// 		temp -> next = bucket;
	// 		bucket = temp;
	// 	}
	// }

	// delete[] array;
	// array = newArray;
}

//find method
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::find( const KEY & key ) const{
	std::size_t index = H()(key); 
	index = index % tablesize;
	Entry *temp = array[index];
	while (temp != nullptr) {
		if (temp -> key == key) {
			return Iterator(temp, (UnorderedMap *)this);
		}
		temp = temp -> next;
	}
	return end(); 
}

//[] method
template<typename KEY, typename T, typename H>
T& UnorderedMap<KEY, T, H>::operator[](const KEY & key){
	T var;
	Iterator it = find(key);
	if (it == end()) {
		insert(key, var);
		return find(key) -> data;
	}
	return it -> data;
}

//++ method
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::Iterator::operator++(int){
	if (_cur == NULL) {
		return *this;
	}
	else {
		if (_cur -> next != nullptr) {
			_cur = _cur -> next;
			return *this;
		}
		else {
			std::size_t index = H()(_cur -> key); 
			index = index % hashMap -> tablesize;
			index++;
			while (index < hashMap -> tablesize) {
				if (hashMap -> array[index] != NULL) {
					_cur = hashMap -> array[index];
					return *this;
				}
				index++;
			}
		}
		_cur = nullptr;
	}
	return *this;
}

//begin method
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::begin() const{
	//start iterator at the first node coming off the first bucket that has a node coming from itself
	for (int i = 0; i < tablesize; i++) {
		if (array[i] != NULL) {
			return Iterator(array[i], (UnorderedMap *)this);
		}
	}
	return end();
}

//end method
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Iterator UnorderedMap<KEY, T, H>::end() const{
	// stub code; need to implement !!!
	return Iterator(NULL, (UnorderedMap *)this);
}

//-> method
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Entry* UnorderedMap<KEY, T, H>::Iterator::operator->(){
	return _cur;
}

//* method
template <typename KEY, typename T, typename H>
typename UnorderedMap<KEY, T, H>::Entry& UnorderedMap<KEY, T, H>::Iterator::operator*(){
	return *_cur;
}

//!= method
template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::Iterator::operator!=(Iterator it){
	return _cur != it._cur;
}

//== method
template <typename KEY, typename T, typename H>
bool UnorderedMap<KEY, T, H>::Iterator::operator==(Iterator it){
	return _cur == it._cur;
}

// //size method (entrysize)
// template <typename KEY, typename T, typename H>
// int UnorderedMap<KEY, T, H>::size(){
// 	return entrysize;
// }

// //tsize method (tablesize)
// template <typename KEY, typename T, typename H>
// int UnorderedMap<KEY, T, H>::tsize(){
// 	return tablesize;
// }

// Internal method to test if a positive number is prime (not efficient)
bool isPrime( int n ){
	cout << "expo markers are non-toxic" << endl;
	if( n == 2 || n == 3 )
		return true;
	if( n == 1 || n % 2 == 0 )
		return false;
	for( int i = 3; i * i <= n; i += 2 )
		if( n % i == 0 )
			return false;
	return true;
}

// Internal method to return a prime number at least as large as n.
// Assumes n > 0.
int nextPrime( int n ){
	if( n % 2 == 0 )
		n++;
	for( ; !isPrime( n ); n += 2 )
		;
	return n;
}


#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H


/****************************************
 * UW User ID:  a293pate
 * Submitted for ECE 250
 * Semester of Submission:  Fall 2019
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 ****************************************/

#include "Exception.h"
#include "ece250.h"

enum state { EMPTY, OCCUPIED, DELETED };

template<typename T>
class DoubleHashTable {
	private:
		int count;
		int power;
		int array_size;
		T *array;
		state *array_state;

		int h1( T const & ) const; // first hash function
		int h2( T const & ) const; // second hash function

	public:
		DoubleHashTable( int = 5 );
		~DoubleHashTable();
		int size() const;
		int capacity() const;		
		bool empty() const;
		bool member( T const & ) const;
		T bin( int ) const;

		void print() const;

		void insert( T const & );
		bool remove( T const & );
		void clear();
};

template<typename T >
DoubleHashTable<T >::DoubleHashTable( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
array( new T [array_size] ),
array_state( new state[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		array_state[i] = EMPTY;
	}
}

template<typename T >
DoubleHashTable<T >::~DoubleHashTable() {
	delete[] array;
	delete[] array_state;
}

template<typename T >
int DoubleHashTable<T >::size() const {
	return count;
}

template<typename T >
int DoubleHashTable<T >::capacity() const {
	return array_size;
}

template<typename T >
bool DoubleHashTable<T >::empty() const {
	if (count == 0) {
		return true;
	} else {
		return false;
	}
}

template<typename T >
int DoubleHashTable<T >::h1( T const &obj ) const {
	int i = static_cast<int>(obj);

	i = i % array_size;
	if (i < 0) {
		i += array_size;
	}

	return i;
}

template<typename T >
int DoubleHashTable<T >::h2( T const &obj ) const {
	int i = static_cast<int>(obj);

	i = i / array_size;

	i = i % array_size;

	if (i < 0) {
		i += array_size;
	}

	if (i % 2 == 0) {
		i += 1;
	}

	return i;
}

template<typename T >
bool DoubleHashTable<T >::member( T const &obj ) const {
	int bin = h1(obj);
	int jump_size = h2(obj);
	int i = 0;
	while (array_state[(bin + i * jump_size) % array_size] != EMPTY && i < array_size) { //TODO: not O(1) and prove that the double hashing function never hits a key twice
		if (array_state[(bin + i * jump_size) % array_size] == OCCUPIED &&
			array[(bin + i * jump_size) % array_size] == obj) {
			return true;
		}
		i++;
	}


	return false;
}

template<typename T >
T DoubleHashTable<T >::bin( int n ) const {
	if (array_state[n] == OCCUPIED) {
		return array[n];
	}
}

template<typename T >
void DoubleHashTable<T >::insert( T const &obj ) {
    if (count == array_size) {
		throw overflow();
	}
	else {
        int bin = h1(obj);
        int jump_size = h2(obj);
        int i = 0;
        //for (int i = 0; i < array_size - 1; ++i) { //TODO: not O(1)
        while(array_state[(bin + i * jump_size) % array_size] == OCCUPIED){
            i++;
        }
        //if (array_state[(bin + i * jump_size) % array_size] != OCCUPIED) {
        array[(bin + i * jump_size) % array_size] = obj;
        count++;
        array_state[(bin + i * jump_size) % array_size] = OCCUPIED;
        //}
    }
}

template<typename T >
bool DoubleHashTable<T >::remove( T const &obj ) {
	int bin = h1(obj);
	int jump_size = h2(obj);
	int i = 0;
	while (array_state[(bin + i * jump_size) % array_size] != EMPTY && i < array_size) { //TODO: not O(1) and prove that the double hashing function never hits a key twice
		if (array_state[(bin + i * jump_size) % array_size] == OCCUPIED &&
			array[(bin + i * jump_size) % array_size] == obj) {
			array_state[(bin + i * jump_size) % array_size] = DELETED;
			count = count - 1;
			return true;
		}
		i++;
	}

	return false;
}

template<typename T >
void DoubleHashTable<T >::clear() {
	for (int i = 0; i < capacity(); ++i) {
		remove(bin(i));
	}
}

template<typename T >
void DoubleHashTable<T >::print() const {
	for (int i = 0; i < capacity(); ++i) {
		std::cout << i << " : " << bin(i) << std::endl;
	}
	std::cout << "Diagnostics" << std::endl;
    std::cout << "count: " << count << std::endl;
    std::cout << "array_size: " << array_size << std::endl;
}

#endif

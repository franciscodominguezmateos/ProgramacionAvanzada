/*
 * eratosthenes_sieve.h
 *
 *  Created on: 19 Jan 2021
 *      Author: Francisco Dominguez
 */
#pragma once
#include <vector>
using namespace std;
class EratosthenesSieve {
	unsigned int N;
	vector<unsigned int> primes;
public:
	EratosthenesSieve(unsigned int n):N(n){
		for(unsigned int number=2;number<N;number++)
			if(isPrime(number))
				primes.push_back(number);
	}
	inline bool canDivide(unsigned int number,unsigned int prime){
		unsigned int mod=number % prime;
		return mod==0;
	}
	bool isPrime(unsigned int number){
		for(unsigned int prime:primes){
			if(canDivide(number,prime))
				return false;
			if(number<prime*2)
				return true;
		}
		return true;
	}
	vector<unsigned int> &getPrimes(){return primes;}
};


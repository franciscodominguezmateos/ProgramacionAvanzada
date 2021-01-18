/*
 * fourier_serie.h
 *
 *  Created on: 18 Jan 2021
 *      Author: Francisco Dominguez
 *  Playing with fourier series and complex numbers, inspired by:
 *  https://iquilezles.org/www/articles/fourier/fourier.htm
 */

#pragma once
#include <complex>
#include <vector>
using namespace std;
typedef complex<double> C;
const C i(0,1);
const C PI(3.141592653589793,0);
const C PI2=PI*2.0;
class FourierSerie {
	double N;
public:
	FourierSerie(int n):N(n){}
	C direct(vector<C> x, double k){
		C Xk(0,0);
		for(double n=0;n<N;n++){
			int in=int(n);
			Xk+=x[in]*exp(-i*PI2*k*n/N);
		}
		return Xk;
	}
	vector<C> direct(vector<C> x){
		int iN=int(N);
		vector<C> X(iN);
		for(int k=0;k<iN;k++)
			X[k]=direct(x,k);
		return X;
	}
	C inverse(vector<C> X,double n){
		C xn(0,0);
		for(double k=0;k<N;k++){
			int ik=int(k);
			xn+=X[ik]*exp(i*PI2*k*n/N);
		}
		return xn/N;
	}
	vector<C> inverse(vector<C> X){
		int iN=int(N);
		vector<C> x(iN);
		for(int n=0;n<iN;n++)
			x[n]=inverse(X,n);
		return x;
	}
};


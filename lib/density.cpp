#ifndef __DENSITY_CPP__
#define __DENSITY_CPP__
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>
#include "classes.h"
#include <stdlib.h>


using namespace std;

//implication for class box


density::density(int* _wall){
	N = _wall[0]*_wall[1]*_wall[2];

	for( int l = 0; l < 3; l++ ) wall[l] = _wall[l];
	_in = new double [N];
	_to_print = new double [N];
	_flag = new int [N];
	_fix_area = new int [N];
	for (int i = 0; i < N; i++) _fix_area[i] = 0;
}

density::~density(){
	delete[] _in;
	delete[] _to_print;
	delete[] _flag;
	delete[] _fix_area;
}

int density::clear(){
	for( int i = 0; i < N; i ++) _flag[i] = 0;
	return 0;
}

int density::fix_area(int count){
	for( int i = 0; i < N; i ++)
		if(_flag[i] == count) _fix_area[i] = _last_fix;
	_last_fix++;
	return 0;
}

int density::fix_to_print(){
	for( int i = 0; i < N; i++)
		if (_fix_area[i] != 0) _to_print[i] = (max+min)/2;
		else _to_print[i] = 0;
	return 0;
}
double*	density::in(int* x){
	return &_in[x[2]*wall[0]*wall[1]+x[1]*wall[0]+x[0]];
}

double*	density::in(int x, int y, int z){
	return &_in[z*wall[0]*wall[1]+y*wall[0]+x];
}

int*	density::flag(int* x){
	return &_flag[x[2]*wall[0]*wall[1]+x[1]*wall[0]+x[0]];
}

int*	density::flag(int x, int y, int z){
	return &_flag[z*wall[0]*wall[1]+y*wall[0]+x];
}


#endif // __DENSITY_CPP__

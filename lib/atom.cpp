#ifndef __ATOM_CPP__
#define __ATOM_CPP__
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>
#include "classes.h"
#include <stdlib.h>


using namespace std;

atom::atom(int _N){
  N = _N;
  _x = new double [N*3];
  flag = new int [N];
  neigbours_atom = new int [N];
  neigbours_electron = new int [N];
  clear();
}

atom::~atom(){
  delete[] _x;
  delete[] flag;
  delete[] neigbours_atom;
  delete[] neigbours_electron;
}

int atom::clear(){
  for (int i = 0; i < N; i++) flag[i] = 0;
  return 0;
}

double* atom::x(int i){
  return &_x[i*3];
}

char* atom::print(int i){
  char *buf = new char [100];
  sprintf(buf, "%lf\t%lf\t%lf", _x[i*3], _x[i*3+1], _x[i*3+2]);
  return buf;
}

int atom::add(int i, double* coord){
  for(int l = 0; l < 3; l++)
    _x[i*3+l] = coord[l];
  return 0;
}
#endif // __ATOM_CPP__

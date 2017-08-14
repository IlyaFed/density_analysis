#ifndef __DENSITY_H__
#define __DENSITY_H__

#include <iostream>
#include <fstream>
#include <string>


using namespace std;


class atom{
public:
	double *_x; //coordinates of all atoms
	int *flag;
	int N;
	int *neigbours_atom;
	int *neigbours_electron;

	atom(int _N);
	~atom();
	int clear();
	double* x(int i);
	char* print(int i);
	int add(int i, double* coord);

};

class density{
public:
	double *_in = NULL;
	double *_to_print = NULL;
	int N;
	double min;
	double max;
	int *_flag = NULL;
	int wall[3];
	int N_electron;

	density(int* _wall);
	~density();
	int clear();
	double* in(int* coord);
	double* in(int a, int b, int c);
	int* flag(int* x);
	int* flag(int x, int y, int z);

};

class area_tree
{
public:
	area_tree* prev = NULL;
	area_tree* next = NULL;

	int count;
	double mass = 0;
	int N_atom = 0;
	int N_electron = 0;
	~area_tree();
	area_tree();
};

class areas{
public:
	atom* atoms;
	int *_to_print;
	//int *print_flag;
	int _to_print_last = 0;
	area_tree *start = NULL;
	area_tree *last = NULL;
	int _max_gist = 11;
	int* _gistogram = NULL;
	double mass = 0;
	int N;
	int N_electron = 0;
	int N_electron_real = 0;
	int N_atom = 0;

	areas(atom* _atoms, int _N_electron);
	~areas();
	int add();
	int analysis(double rho_cut);
	int gist_add(int n_atoms, int n_electrons);
	int gist_get(int n_atoms, int n_electrons);
	char* gist_to_print();
	int gist_analysis();

	int smth_useful(double rho_cut, area_tree* buf);

};

class box{
public:
	density *rho = NULL;
	areas *area = NULL;
	atom *atoms = NULL;
	int wall[3];
	double rho_cut = 0;

	box(FILE* input);
	~box();

	int read(FILE* input);
	int write(string input_name);
	int cut_unnes(double rho_cut);

	int create_areas(double density_cut);
	int find_all(area_tree* buf, int* x, double density_cut, int flag_point);
	int atom_flaged();
	int print_atoms(string name);

	int* x(int coord);
	int len(int* x);
	int clear();

};



#endif // __DENSITY_H__

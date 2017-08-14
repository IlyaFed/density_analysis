#ifndef __BOX_CPP__
#define __BOX_CPP__
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>

#include "classes.h"

#include <stdlib.h>


using namespace std;


box::box(FILE* input){
	read(input);
}

int box::read(FILE* input){
	cerr << "start reading box \n" << endl;
	char buf[100];
	for( int i = 0; i < 6; i++) {
		fgets(buf, 100, input);
	}

  // read atoms coordinates
  int N_atom;
	fscanf(input, "%d\nDirect", &N_atom); //Number of atoms
	cerr << "N_atoms = " << N_atom << endl;

	//fgets(buf, 100, input);

  atoms = new atom(N_atom);
  double x[3];
  for( int i = 0; i < atoms->N; i++) {
    fscanf(input, "%lf\t%lf\t%lf", &x[0], &x[1], &x[2]);
    atoms->add(i, x);
  }
	//fscanf(input, "%s", buf);

  // read density in lattice
	fscanf(input, "%d\t%d\t%d", &wall[0], &wall[1], &wall[2]);
	cerr << "wall: " << wall[0] << " " << wall[1] << " " << wall[2] << endl;
	rho = new density(wall);

	fscanf(input, "%lf", &rho->_in[0]);
	rho->min = rho->_in[0];
	rho->max = rho->_in[0];

	double total_mass = 0;
	for( int i = 1; i < rho->N; i++) {
		fscanf(input, "%lf", &rho->_in[i]);
		rho->_to_print[i] = rho->_in[i];
		total_mass += rho->_in[i];
		if (rho->_in[i] < rho->min) rho->min = rho->_in[i];
		if (rho->_in[i] > rho->max) rho->max = rho->_in[i];
	}

	rho->N_electron = round( total_mass/rho->wall[0]/rho->wall[1]/rho->wall[2]);
	cerr << "N_electrons = " << rho->N_electron << endl;

	cerr << "succesful reading box" << endl;
	return 0;
}

int box::write(string input_name){
	ofstream foutput;
	foutput.open(input_name);
	cerr << "start writing box \n" << endl;

	foutput << "unknown system\n 1" << endl;
	foutput << "4.000000    0.000000    0.000000" << endl;
	foutput << "0.000000    4.000000    0.000000" << endl;
	foutput << "0.000000    0.000000    4.000000" << endl;
//	int N_atom_print = 0;
//	for (int i = 0; i < N_atom; i ++) if(atom_flag == 0) N_atom_print++;
	foutput << "H\n" << atoms->N << endl;
	foutput << "Direct" << endl;
	for (int i = 0; i < atoms->N; i ++) foutput << atoms->print(i) << endl;

	foutput << "\n" << rho->wall[0] << "\t" << rho->wall[1] << "\t" << rho->wall[1];

	for( int i = 0; i < rho->N; i++) {
		if (i%5 == 0) foutput << "\n";
		foutput << rho->_to_print[i] << " ";
	}

	foutput.close();
	cerr << "succesful writin box" << endl;
	return 0;
}



int box::cut_unnes(double rho_cut){
	cerr << "empty areas = " << area->_to_print_last + 1 << endl;
	for(int i = 0; i < rho->N; i++){
//		if (flag[i] != 0) new_line[i] = 50;
//		else new_line[i] = 0;
		rho->_to_print[i] = 0;
		for(int j = 0; j < area->_to_print_last+1; j++)
			if (rho->_flag[i] == area->_to_print[j]) rho->_to_print[i] = rho->_in[i];
	}
	cerr << "cut unnes succesful" << endl;
	return 0;
}

box::~box(){
	if (area != NULL) clear();
	delete atoms;
  cerr << "delete atoms" << endl;
	delete rho;
  cerr << "delete rho" << endl;
}

int* box::x(int coord){
	int* x;
	x = new int [3];
	x[2] = coord / (wall[1]*wall[2]);
	x[1] = (coord % (wall[1]*wall[2]) ) / wall[2];
	x[0] = (coord % (wall[1]*wall[2]) ) % wall[2];
	return x;
}

int box::len(int* x){
	return x[2]*wall[0]*wall[1]+x[1]*wall[0]+x[0];
}






int box::create_areas(const double _rho_cut){
	rho_cut = _rho_cut;
	int k = 1;
	area = new areas(atoms, rho->N_electron);

	for( int i = 0; i < rho->N; i++){
		if ( rho->_in[i] < rho_cut || rho->_flag[i] != 0 ) continue;
//		cerr << "add new area" << endl;
		area->add();
		int* x_0 = x(i);
		find_all(area->last, x_0, rho_cut, k);
  	k++;
	}
  atom_flaged();
	area->analysis(rho_cut);
	atom_connect();
	return 0;
}

int box::atom_connect(){
	int level = (atoms->N + rho->N_electron) * 0.2;
	for( int i = 0; i < atoms->N; i++)
		if(atoms->_fix_area[i] == 0 && (atoms->neigbours_atom[i] + atoms->neigbours_electron[i]) < level) {
			//cerr << "rho_cut " << rho_cut << endl;
			rho->fix_area(atoms->flag[i]);
			atoms->fix_area(atoms->flag[i]);
		}
	return 0;
}

int box::print_areas(string input_name){
	rho->fix_to_print();
	write(input_name);
	return 0;
}

int box::make_molecule(string input_name){
	double start = rho->min;
  double stop = rho->max;
	double step = (stop-start) / 100.0;

	for( double rho_cut = start; rho_cut < stop; rho_cut+= step){
  	create_areas(rho_cut);
	  clear();
	}

	print_areas("output/CHGCAR");
	return 0;
}

int box::find_all(area_tree* buf, int* x, double _rho_cut, int flag_point){
	if (*rho->flag(x) != 0 || *rho->in(x) < _rho_cut) return 0;
//	cerr << flag_point << endl;
	*rho->flag(x) = flag_point;
	buf->mass += *(rho->in(x));
	int x_new [3] = {x[0], x[1], x[2]};
	for (int l = 0; l < 3; l++){
		x_new[l] = (x[l]+wall[l] + 1)% wall[l];
		find_all(buf, x_new, _rho_cut, flag_point);
		x_new[l] = (x[l]+wall[l] - 1)% wall[l];
		find_all(buf, x_new, _rho_cut, flag_point);
		x_new[l] = x[l];
	}
	return 0;
}

int box::atom_flaged(){
	int first[3];
	int second[3];
	int a[3];
	for( int i =0 ; i < atoms->N; i ++){
	//	cerr << "atom " << i << endl;
		for( int l =0; l < 3; l++)
			first[l] =  atoms->_x[i*3+l]*wall[l];
			//if (rho_cut < 37) cerr << "rho flag = " << *rho->flag(first) << endl;
		for (a[0] = 0; a[0] < 2; a[0]++)
			for (a[1] = 0; a[1] < 2; a[1]++)
				for (a[2] = 0; a[2] < 2; a[2]++) {
					for(int l = 0; l < 3; l++)
						second[l] = (first[l] + a[l]) % wall[l];

					if (*rho->flag(second) != 0)  {
						atoms->flag[i] = *rho->flag(second);
					}
		}
	}
	return 0;
}

int box::clear(){
	delete area;
  area = NULL;
  atoms->clear();
	rho->clear();
	return 0;
}

int box::print_atoms(string name){
	ofstream dump_output;
	dump_output.open(name);
	int k = 0;
	dump_output << "ITEM: TIMESTEP\n" << 0 << "\nITEM: NUMBER OF ATOMS\n" << atoms->N << "\nITEM: BOX BOUNDS pp pp pp\n" << 0e0  << " " << 1e0 << "\n" << 0e0  << " "  << 1e0 << "\n" << 0e0  << " "  << 1e0 << "\nITEM: ATOMS id type xs ys zs" << endl;
	for( int i = 0; i < atoms->N; i ++) dump_output << i << " " << atoms->flag[i]  << " " << atoms->print(i)<< endl;
	dump_output.close();
	cerr << "print atom succesful" << endl;
	return 0;
}
#endif // __BOX_CPP__

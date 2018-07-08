// ./a.out dtime max_photo photo_step max_energy energy_step wall r_kr
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>
#include "lib/classes.h"

using namespace std;


int main(int argc, char* argv[]){

// reading CHGCAR file
	FILE *input_file;
  input_file = fopen("source_for_test/CHGCAR_2", "r");
	box *test_box = new box(input_file);
  fclose(input_file);

// initialise output files
  ofstream foutput, foutput2, neigbours_atom_out, neigbours_electron_out, neigbours_comm_out;
  foutput.open("output/count_dependense.txt");
  foutput2.open("output/count_dependense_2.txt");
	neigbours_atom_out.open("output/neigbour_atom_dependense.txt");
	neigbours_electron_out.open("output/neigbour_electron_dependense.txt");
	neigbours_comm_out.open("output/neigbour_comm_dependense.txt");

	foutput << "rhocut\t real-el\t 0-1\t 1-0\t 1-1\t 2-0\t 2-1\t 2-2\t 3-0\t 3-1\t 3-2\t 3-3\t other" << endl;
	foutput2 << "rhocut\t real-el\t 0-1\t 1-0\t 1-1\t 2-0\t 2-1\t 2-2\t 3-0\t 3-1\t 3-2\t 3-3\t other" << endl;

// start scanning areas
  double start = test_box->rho->min;
  double stop = test_box->rho->max;
	double step = (stop-start) / 100.0;
  cerr << "min " << start << ", max " << stop << endl;


	for( double rho_cut = start; rho_cut < stop; rho_cut+= step){
    //cerr << "step " << i << endl;
    test_box->create_areas(rho_cut);
		//print gistogram
    if (test_box->area->N_electron_real == test_box->rho->N_electron) foutput2 << rho_cut << " " << test_box->area->N_electron_real << " " << test_box->area->gist_to_print() << endl;
    foutput << rho_cut << " " << test_box->area->N_electron_real << " " <<  test_box->area->gist_to_print() << endl;
		// print dependense of number of atoms and electrons for every atom
		neigbours_atom_out << rho_cut;
		neigbours_electron_out << rho_cut;
		neigbours_comm_out << rho_cut;
		for (int i = 0; i < test_box->atoms->N; i ++){
			neigbours_atom_out << "\t" << test_box->atoms->neigbours_atom[i];
			neigbours_electron_out << "\t" << test_box->atoms->neigbours_electron[i];
			neigbours_comm_out << "\t" << test_box->atoms->neigbours_electron[i] + test_box->atoms->neigbours_atom[i];
		}
		neigbours_atom_out << endl;
		neigbours_electron_out << endl;
		neigbours_comm_out << endl;

    test_box->clear();
	}

	test_box->print_areas("output/CHGCAR", 0);
	neigbours_atom_out.close();
	neigbours_electron_out.close();
	neigbours_comm_out.close();
/*
double rho_cut = (start+stop)/2;
test_box->create_areas(96);
if (test_box->area->N_electron_real == test_box->rho->N_electron)
	foutput2 << rho_cut << " " << test_box->area->gist_to_print() << endl;
foutput << rho_cut << " " << test_box->area->gist_to_print() << endl;
test_box->clear();
*/
// work with one cutting
	/*
	test_box->create_areas(88);
	test_box->print_atoms("output/dump.txt");


  test_box->cut_unnes(96);
  test_box->write("output/CHGCAR");
	*/
  cerr << "clear box" << endl;
	delete test_box;

  foutput.close();
  foutput2.close();

	cerr << "everything ok" << endl;
	return 0;
}

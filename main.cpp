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
  if (argc < 2) {
    cerr << "need input file name" << endl;
    return 0;
  }

	FILE *input_file;
  input_file = fopen(argv[1], "r");
	box *space = new box(input_file);
  fclose(input_file);

  space->make_molecule("output/CHGCAR");

  cerr << "clear box" << endl;
	delete space;

	cerr << "----finish-----" << endl;
	return 0;
}

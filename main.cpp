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

  char buf_name[100];
  int i = 0;
  while (argv[1][i] != '\0' || argv[1][i] == '*'){
    buf_name[i] = argv[1][i];
    i++;
  }
  buf_name[i] = '\0';

  if (argv[1][i] != '*') {
    FILE *input_file;
    input_file = fopen(argv[1], "r");
  	box *space = new box(input_file);
    fclose(input_file);

    space->make_molecule("visualisation/CHGCAR", 1);
    cerr << "clear box" << endl;
    delete space;

  } else {
    FILE *input_file;
    char file_name[100];
    char out_file_name[100];
    for (int m = 0; m < 100; m++){
      sprintf(file_name, "%s%d", buf_name, m);
      input_file = fopen(file_name, "r");
      if (input_file != NULL) break;
  	  box *space = new box(input_file);
      fclose(input_file);

      sprintf(out_file_name, "visualisation/%s", buf_name);
      space->make_molecule(out_file_name, 1);
      delete space;
    }
  }




	cerr << "----finish-----" << endl;
	return 0;
}

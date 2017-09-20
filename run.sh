g++ -std=c++11 -ggdb lib/density.cpp lib/areas.cpp lib/atom.cpp lib/box.cpp main.cpp -o p
./p source_for_test/CHGCAR_1
rm p
rm -r p.dSYM
cd visualisation
/Applications/VMD\ 1.9.3.app/Contents/MacOS/startup.command -e show_all.vmd

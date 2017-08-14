all:
	g++ -std=c++11 -ggdb lib/density.cpp lib/areas.cpp lib/atom.cpp lib/box.cpp test.cpp -o test_dft_analysis
	./test_dft_analysis
	gnuplot plot.txt

test:
	g++ -std=c++11 -ggdb lib/density.cpp lib/areas.cpp lib/box.cpp lib/atom.cpp  test.cpp -o test_dft_analysis
	./test_dft_analysis
	gnuplot plot.txt

clean:
	rm -f dft_analysis
	rm -f test_dft_analysis

plot:
	gnuplot plot.txt

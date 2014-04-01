/*
 * File:   main.cpp
 * Author: vlad
 * 
 * Created on December 12, 2013, 10:38 PM
 */

#include <iostream>

#include "qpp.h"
//#include "matlab.h" // support for MATLAB

// TODO: expandout function
// TODO: dyad function
// TODO: proj (dya) function
// TODO: ip (inner product function) function, make it general to return matrices
// TODO: Error class
// TODO: change all for(s) to column major order
// TODO: use .data() raw pointer instead of looping

using namespace std;

using namespace qpp;
using namespace qpp::types;

//int main(int argc, char **argv)
int main()
{
	_init();

	// Display formatting
	std::cout << std::fixed; // use fixed format for nice formatting
//	std::cout << std::scientific;
	std::cout << std::setprecision(4); // only for fixed or scientific modes

	cout << "Starting qpp..." << endl;

	size_t n = 12; // 12 qubits
	size_t N = std::pow(2, n);
	vector<size_t> dims; // local dimensions
	for (size_t i = 0; i < n; i++)
		dims.push_back(2);
	std::cout << "n = " << n << " qubits, matrix size " << N << " x " << N
			<< "." << endl;

	// TIMING
	Timer t, total;  // start the timer, automatic tic() in the constructor

	// Matrix initialization
	cout << "Matrix initialization timing." << endl;
	cmat randcmat = cmat::Random(N, N);
	t.toc(); // read the time
	cout << "Took " << t.seconds() << " seconds." << endl;

	// Matrix product
	cout << "Matrix product timing." << endl;
	t.tic(); // reset the chronometer
	cmat prodmat;
	prodmat = randcmat * randcmat; // need this (otherwise lazy evaluation)
	t.toc(); // read the time
	cout << "Took " << t.seconds() << " seconds." << endl;

	// ptrace SLOW SLOW SLOW
	cout << "ptrace timing." << endl;
	vector<size_t> subsys_ptrace = { 0 };
	cout << "Subsytem(s): ";
	internal::_disp_container(subsys_ptrace);
	std::cout << endl;
	t.tic();
	ptrace(randcmat, subsys_ptrace, dims);
	t.toc();
	cout << "Took " << t.seconds() << " seconds." << endl;

	// ptranspose
	cout << "ptranspose timing." << endl;
	vector<size_t> subsys_ptranspose; // partially transpose all subsystems
	for (size_t i = 0; i < n; i++)
		subsys_ptranspose.push_back(i);
	cout << "Subsytem(s): ";
	internal::_disp_container(subsys_ptranspose);
	std::cout << endl;
	t.tic();
	ptranspose(randcmat, subsys_ptranspose, dims);
	t.toc();
	cout << "Took " << t.seconds() << " seconds." << endl;

	// syspermute
	cout << "syspermute timing." << endl;
	vector<size_t> perm; // left-shift all subsystems by 1
	for (size_t i = 0; i < n; i++)
		perm.push_back((i + 1) % n);
	cout << "Subsytem(s): ";
	internal::_disp_container(perm);
	std::cout << endl;
	t.tic();
	syspermute(randcmat, perm, dims);
	t.toc();
	cout << "Took " << t.seconds() << " seconds." << endl;


	total.toc(); // read the total running time
	cout << "Total time: " << total.seconds() << " seconds." << endl;
	// END TIMING

	cout << endl << "Exiting qpp..." << endl;
}

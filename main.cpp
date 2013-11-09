#include "BLReadFiles.h"
#include "StowageCP.h"
#include <gecode/gist.hh>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace Gecode;

int main(int argc, char *argv[])
{
    BLReadFiles objBLReadFiles;
    StowageInfo objStowageInfo = 
                //objBLReadFiles.ChargeFile("/home/adolfo/Universidad/maestria/tesis/inst/inputBay0_3_11_1.txt");
                objBLReadFiles.ChargeFile("/home/adolfo/Universidad/maestria/tesis/inst/inputBay0_4_15_1.txt");
    
    objStowageInfo.ChargeData();
    
	// create model and search engine
	StowageCP* m = new StowageCP( objStowageInfo );
	DFS<StowageCP> e(m);
	delete m;
	// search and print all solutions
	while (StowageCP* s = e.next()) {
		s->print(); 
		delete s;
	}
    
	
    return EXIT_SUCCESS;
}

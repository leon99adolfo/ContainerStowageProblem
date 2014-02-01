#include "BLReadFiles.h"
#include "StowageCP.h"
#include <gecode/gist.hh>
#include <cstdlib>
#include <iostream>
#include <time.h>

using namespace std;
using namespace Gecode;

int main(int argc, char *argv[])
{
	//int file_count = 0;
	string full_path = "/home/adolfo/Universidad/maestria/tesis/inst/";
	string archivo;
    BLReadFiles objBLReadFiles;
    
    cout<<"ingrese el archivo: "<<endl;
    cin>>archivo;
    
    // time
	clock_t init, final;
	init=clock();
    
    StowageInfo objStowageInfo = 
                objBLReadFiles.ChargeFile(full_path+archivo);
                
    
    objStowageInfo.ChargeData();
    
	// create model and search engine
	StowageCP* m = new StowageCP( objStowageInfo );
	BAB<StowageCP> e(m);
	delete m;
	// search and print all solutions
	while (StowageCP* s = e.next()) {
		s->print(); 
		delete s;
		
		final=clock()-init;
		cout << "Tiempo: "<<(double)final / ((double)CLOCKS_PER_SEC)<< endl;
	}
    
	final=clock()-init;
	cout << "Tiempo Final: "<<(double)final / ((double)CLOCKS_PER_SEC)<< endl;
	
    return EXIT_SUCCESS;
}

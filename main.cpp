#include "BLReadFiles.h"
#include "StowageCP.h"
#include <gecode/gist.hh>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <fstream>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;
using namespace Gecode;


void* Enviroment(string pDirFile, string pFile, ofstream &pFileOut)
{
	BLReadFiles objBLReadFiles;
	// time
	clock_t init, final, final2;
	init=clock();
	final = 0;
    
    StowageInfo objStowageInfo = 
                objBLReadFiles.ChargeFile(pDirFile + pFile);
                
    
    objStowageInfo.ChargeData();
    
	// create model and search engine
	Search::Options so;
	Search::TimeStop ts(600000); // stop after 600000 ms
	so.stop = &ts;
	StowageCP* m = new StowageCP( objStowageInfo );
	BAB<StowageCP> e(m, so);
	delete m;
	
	int O, OGCTD, OR, OP, OU, OCNS;
	// search and print all solutions
	while (StowageCP* s = e.next()) {
		s->print(O, OGCTD, OR, OP, OU, OCNS); 
		delete s;
		
		final2 = final;
		final=clock()-init;		 
		cout << "Tiempo: "<<(double)final / ((double)CLOCKS_PER_SEC)<< endl;
	}
    
    pFileOut<<O<<"\t"<<OGCTD<<"\t"<<OR<<"\t"<<OP<<"\t"
			<<OU<<"\t"<<OCNS<<"\t"<<(double)final2 / ((double)CLOCKS_PER_SEC)
			<<"\t"<<pFile<<endl;
    
}



int main(int argc, char *argv[])
{
	//int file_count = 0;
	string full_path = "/home/adolfo/Universidad/maestria/tesis/inst/";
    
    ofstream write_fich((full_path+"response.txt").c_str());
    
    if(!write_fich)
    {
		cout<<"Error, The file can't open"<<endl;
		return 0;
	}
    
    write_fich<<"O\tOGCTD\tOR\tOP\tOU\tOCNS\tTime\tFile"<<endl;
    
    cout<<"Empieza a procesar"<<endl;
    
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    ifstream fin;
    int num;
	dp = opendir( full_path.c_str() );
	if (dp == NULL)
    {
		cout << "Error opening " << full_path << endl;
		return 0;
    }
	
	while ((dirp = readdir( dp )))
    {
		string filepath = full_path + dirp->d_name;
		// If the file is a directory (or is in some way invalid) we'll skip it 
		if (stat( filepath.c_str(), &filestat )) continue;
		if (S_ISDIR( filestat.st_mode ))         continue;

		// Endeavor to read a single number from the file and display it
		fin.open( filepath.c_str() );
		if (fin >> num)
		{
			if(num = 1)
			{
				cout << filepath << ": " << num << endl;
				Enviroment(full_path, dirp->d_name, write_fich);
			}
		}
		fin.close();
    }

	closedir( dp );
    
	
    return EXIT_SUCCESS;
}

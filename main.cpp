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


void* Enviroment(string pDirFile, string pFile, ofstream &pFileOut, ofstream &pFileOutSOL)
{
	pFileOutSOL<<pFile<<endl;
	
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
	Search::TimeStop ts(6000); // stop after 600000 ms
	so.stop = &ts;
	StowageCP* m = new StowageCP( objStowageInfo );
	BAB<StowageCP> e(m, so);
	delete m;
	
	string OP, SL;
	int O, OGCTD, OR, OPT, OU, OCNS, OV;
	// search and print all solutions
	while (StowageCP* s = e.next()) {
		s->print(O, OGCTD, OR, OP, OPT, OU, OCNS, OV, SL); 
		delete s;
		
		final2 = final;
		final=clock()-init;		 
		cout << "Tiempo: "<<(double)final / ((double)CLOCKS_PER_SEC)<< endl;
	}
    
    pFileOut<<O<<"\t"<<OGCTD<<"\t"<<OR<<"\t"<<OPT<<"\t"
			<<OU<<"\t"<<OCNS<<"\t"<<OV<<"\t"<<(double)final2 / ((double)CLOCKS_PER_SEC)
			<<"\t"<<pFile<<"\t"<<OP<< endl;
			
	pFileOutSOL<<SL<<endl;
    
}



int main(int argc, char *argv[])
{
	//int file_count = 0;
	string full_path = "/home/adolfo/Universidad/maestria/tesis/inst2/";
    
    ofstream write_fich((full_path+"response.txt").c_str());
    ofstream write_fichSOL((full_path+"responseSOL.txt").c_str());
    
    if(!write_fich)
    {
		cout<<"Error, The file can't open"<<endl;
		return 0;
	}
	
	if(!write_fichSOL)
    {
		cout<<"Error, The file can't open SOL"<<endl;
		return 0;
	}
    
    write_fich<<"O\tOGCTD\tOR\tOPT\tOU\tOCNS\tOV\tTime\tFile\tOP"<<endl;
    
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
		if (dirp->d_name == "response.txt")	continue;
		if (dirp->d_name == "responseSOL.txt")	continue;

		// Endeavor to read a single number from the file and display it
		fin.open( filepath.c_str() );
		if (fin >> num)
		{
			if(num = 1)
			{
				cout << filepath << ": " << num << endl;			
				Enviroment(full_path, dirp->d_name, write_fich, write_fichSOL);
			}
		}
		fin.close();
    }

	closedir( dp );
    
	
    return EXIT_SUCCESS;
}

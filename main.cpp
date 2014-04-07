#include "BLReadFiles.h"
#include "StowChannelCP.h"
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


void* Enviroment(string pDirFile, string pFile, bool pChannelUse, bool pnuTotalFile, ofstream &pTotalFile)
{	
	// Variables
	BLReadFiles objBLReadFiles;
	clock_t init, final, final2;
	init=clock();
	final = 0;
	string OP, SL;
	int O, OGCTD, OR, OPT, OU, OCNS, OV;
	
	// charge file
    StowageInfo objStowageInfo = objBLReadFiles.ChargeFile(pDirFile + pFile, pChannelUse);
    objStowageInfo.ChargeData();
    
	ofstream fileOut( (pDirFile+"Response/"+pFile).c_str() );
	if(!fileOut)
	{
		cout<<"Error, The file can't open "<<pDirFile+"Response/"+pFile<<endl;
		return 0;
	}
	
	// create model and search engine
	Search::Options so;
	Search::TimeStop ts(600000); // stop after 600000 ms
	so.stop = &ts;
	if(pChannelUse)
	{
		StowChannelCP* m = new StowChannelCP( objStowageInfo );
		BAB<StowChannelCP> e(m, so);
		delete m;
	
		// search and print all solutions
		while (StowChannelCP* s = e.next()) {
			s->print(O, OGCTD, OR, OP, OPT, OU, OCNS, OV, SL, objStowageInfo.Cont.size()); 
			delete s;
		
			final2 = final;
			final=clock()-init;		 
			cout << "Tiempo: "<<(double)final / ((double)CLOCKS_PER_SEC)<< endl;
		}
	}
	else
	{
		StowageCP* m = new StowageCP( objStowageInfo );	
		BAB<StowageCP> e(m, so);
		delete m;
		
		// search and print all solutions
		while (StowageCP* s = e.next()) {
			s->print(O, OGCTD, OR, OP, OPT, OU, OCNS, OV, SL); 
			delete s;
		
			final2 = final;
			final=clock()-init;		 
			cout << "Tiempo: "<<(double)final / ((double)CLOCKS_PER_SEC)<< endl;
		}
	}
		
	string sbTitulos = "O\tOGCTD\tOR\tOPT\tOU\tOCNS\tOV\tTimeS\tTimeT\tFile\tOP";
	fileOut<<sbTitulos<<endl;
		
	final=clock()-init; 
	fileOut<<O<<"\t"<<OGCTD<<"\t"<<OR<<"\t"<<OPT<<"\t"
			<<OU<<"\t"<<OCNS<<"\t"<<OV<<"\t"<<(double)final2 / ((double)CLOCKS_PER_SEC)
			<<"\t"<<(double)final / ((double)CLOCKS_PER_SEC)<<"\t"<<pFile<<"\t"<<OP<<endl<<endl;
					
	fileOut<<"S:"<<endl<<SL<<endl;
	
	fileOut.close();

	if(pnuTotalFile)
	{
		//pTotalFile<<sbTitulos<<endl;
		pTotalFile<<O<<"\t"<<OGCTD<<"\t"<<OR<<"\t"<<OPT<<"\t"
				  <<OU<<"\t"<<OCNS<<"\t"<<OV<<"\t"<<(double)final2 / ((double)CLOCKS_PER_SEC)
				  <<"\t"<<(double)final / ((double)CLOCKS_PER_SEC)<<"\t"<<pFile<<"\t"<<OP<<endl; 
	}
}



int main(int argc, char *argv[])
{
	//int file_count = 0;
	string full_path = "/home/adolfo/Universidad/maestria/tesis/inst/";
	bool boChannelUse = false;
	bool boTotalFile = true;
    
    cout<<"Empieza a procesar"<<endl;
    
    ofstream fileOutTotal( (full_path+"Response/responseTotal.txt").c_str() );		
	if(!fileOutTotal)
	{
		cout<<"Error, The file can't open "<<full_path+"Response/responseTotal.txt"<<endl;
		return 0;
	}
	
	fileOutTotal<<"O\tOGCTD\tOR\tOPT\tOU\tOCNS\tOV\tTimeS\tTimeT\tFile\tOP"<<endl;
	
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
				Enviroment(full_path, dirp->d_name, boChannelUse, boTotalFile, fileOutTotal);
			}
		}
		fin.close();
    }
    
    fileOutTotal.close();    
	closedir( dp );
    
	
    return EXIT_SUCCESS;
}

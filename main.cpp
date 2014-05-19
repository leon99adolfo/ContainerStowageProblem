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


void* Enviroment(string pDirFile, string pResponseDir, string pFile, bool pChannelUse, bool pnuTotalFile, ofstream &pTotalFile, bool pLevel)
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
    objStowageInfo.LevelDistribute = pLevel;
    if(!objStowageInfo.IsValidLoadedCont())
    {
		if(pTotalFile)
		{
			cout<<"------------------	Bad File "<<pFile<< "	-------------------"<<endl; 
			pTotalFile<<"------------------	Bad File "<<pFile<<"	-------------------"<<endl; 
		}
		return 0;
	} 
    
	ofstream fileOut( (pDirFile + pResponseDir + pFile).c_str() );
	if(!fileOut)
	{
		cout<<"Error, The file can't open "<<pDirFile + pResponseDir +pFile<<endl;
		return 0;
	}
	
	// create model and search engine
	Search::Options so;
	Search::TimeStop ts(600000); // stop after 600000 ms
	so.stop = &ts;
	if(pChannelUse)
	{
		StowChannelCP* m = new StowChannelCP( objStowageInfo);
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
		StowageCP* m = new StowageCP(objStowageInfo);	
		BAB<StowageCP> e(m, so);
		delete m;
		
		// search and print all solutions		
		while (StowageCP* s = e.next()) {			
			s->print();
			
			// ---------------------------------------------------------
			// --------------  print values  ---------------------------
			// ---------------------------------------------------------
			
			O = s->O.val();
			OGCTD = s->OGCTD.val();
			OR = s->OR.val();
			
			stringstream ss;
			OPT = 0;
			OP = "";
			for(int x = 0; x < s->OP.size() ; x++)
			{
				int value = s->OP[x].val();
				OPT += value;		
				if(x == 0) 
				{
					ss << value;
				}
				else
				{
					ss << "," << value;
				}
			}	
			OP += ss.str();
	
			OU = s->OU.val();
			OCNS = s->OCNS.val();
			OV = s->OV.val();

			stringstream ss2;
			for(int x = 0; x < s->S.size() ; x++)
			{
				int value = s->S[x].val();
				if(x == 0) 
				{
					ss2 << value;
				}
				else
				{
					ss2 << "," << value;
				}
			}
			SL = ss2.str();
			// ---------------------------------------------------------
			// ---------------------------------------------------------
			// ---------------------------------------------------------
			
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
		pTotalFile<<O<<"\t"<<OGCTD<<"\t"<<OR<<"\t"<<OPT<<"\t"
				  <<OU<<"\t"<<OCNS<<"\t"<<OV<<"\t"<<(double)final2 / ((double)CLOCKS_PER_SEC)
				  <<"\t"<<(double)final / ((double)CLOCKS_PER_SEC)<<"\t"<<pFile<<"\t"<<OP<<endl; 
	}
}



int main(int argc, char *argv[])
{
	if(argc != 2) 
	{
		cout<<"Missing Execute Mode"<<endl;
		return 0;
	}
	
	int mode = atoi(argv[1]);
	if(mode < 0 || mode > 3)
	{
		cout<<"Range [0..3]"<<endl;
		return 0;
	}
	
	//int file_count = 0;
	string full_path = "/home/adolfo/Universidad/maestria/tesis/inst/";
	string responseDir;
	bool boChannelUse;
	bool boIsLevel;
	switch(mode)
	{
		case 0: 
			responseDir = "M1L/";
			boChannelUse = false;
			boIsLevel = true;
			break;
		case 1: 
			responseDir = "M1S/";
			boChannelUse = false;
			boIsLevel = false;
			break;
		case 2: 
			responseDir = "M2L/";
			boChannelUse = true;
			boIsLevel = true;
			break;
		case 3: 
			responseDir = "M2S/";
			boChannelUse = true;
			boIsLevel = false;
			break;
};
	
	bool boTotalFile = true;
    
    cout<<"Empieza a procesar"<<endl;
    
    system(("mkdir " + full_path + responseDir).c_str());
    
    ofstream fileOutTotal( (full_path + responseDir + "responseTotal.txt").c_str() );		
	if(!fileOutTotal)
	{
		cout<<"Error, The file can't open "<<full_path+ responseDir + "responseTotal.txt"<<endl;
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
				Enviroment(full_path, responseDir, dirp->d_name, boChannelUse, boTotalFile, fileOutTotal, boIsLevel);
			}
		}
		fin.close();
    }
    
    fileOutTotal.close();    
	closedir( dp );
    
	
    return EXIT_SUCCESS;
}

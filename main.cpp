#include "BLReadFiles.h"
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    BLReadFiles objBLReadFiles;
    StowageInfo objStowageInfo = 
                objBLReadFiles.ChargeFile("E:/universidad/Maestria/tesis/Proyecto/Ejemplo de papers/inst/inputBay0_3_11_1.txt");
    
    objStowageInfo.ChargeData();
    
    //objBLReadFiles.PrintingData();
    
    
    
    system("PAUSE");
    return EXIT_SUCCESS;
}

/**
 * @file	BLReadFiles
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * ReadFiles class
 * This class read files with the following structure:
 *
 * NumPortsDischarge NumContainerLoad NumContainerLoaded NumStacks NumCell NumLocations NumTiers
 * #POD
 * ListPortsDischarges
 * #LOCATIONS
 * ListLocations
 * #CONTAINERS_TOLOAD
 * StackId CellId numPosition numWeigth numHeigth numLength NumPortDischarge isReefer numLocation 
 * ...
 * ...
 * ...
 * StackId CellId numPosition numWeigth numHeigth numLength NumPortDischarge isReefer numLocation
 * #CONTAINERS_TOLOADED
 * StackId CellId numPosition numWeigth numHeigth numLength NumPortDischarge isReefer numLocation 
 * ...
 * ...
 * ...
 * StackId CellId numPosition numWeigth numHeigth numLength NumPortDischarge isReefer numLocation
 * #STACKS
 * maxWeigth maxHeigth numLocation
 * ...
 * ...
 * ...
 * maxWeigth maxHeigth numLocation
 * #CELLS
 * StaackId isReeferFore IsReeferAft CapFore CapAft Cap40 numLocation
 * ...
 * ...
 * ...
 * StaackId isReeferFore IsReeferAft CapFore CapAft Cap40 numLocation
 * #SOLUTION-AFT
 * Not relevant
 * #SOLUTION-FWD
 * Not relevant
 * #INFO
 * Not relevant
 */

#ifndef BLReadFiles_h
#define BLReadFiles_h

#include "Constants.h"
#include "StowageInfo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class BLReadFiles
{
      private:
              // Variables
              ifstream archivoAr;             
              StowageInfo response;
              Constants objConstants;
              int nuContainerIdx;
              /**
    		 * This method charge container data in file
    		 * @param pContainers: number of container
    		 * @param pAreLoad: containers are loaded
    		 */
             vector<ContainerBox> ReadContainer(int pContainers, bool pAreLoad);
             void ChargeContainerInfo(ContainerBox objContainer);
              
      
      public:
             // ------------------------ Methods ----------------------------------
    		/**
    		 *	Constructor
    		 */
             BLReadFiles();
             /**
    		 *	Destructor
    		 */
             ~BLReadFiles(); 
             /**
    		 * This method charge data in file
    		 * @param pFileName: Name of the file
    		 */
             StowageInfo ChargeFile(string pFileName);
             
              
};

#endif

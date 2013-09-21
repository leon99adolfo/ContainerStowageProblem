/**
 * @file	StowageInfo
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * StowageInfo class.
 */
#ifndef StowageInfo_h
#define StowageInfo_h

#include "StackContainer.h"
#include <list>
using namespace std;

class StowageInfo
{
      private:
              // ------------------------ Properties ------------------------------- 
              int         _numPortsDischarge;
              int         _numContainerLoad;
              int         _numContainerLoaded;
              int         _numStacks;
              int         _numCell;
              int         _numLocations;
              int         _numTiers;
              list<int>   _listPortsDischarges;
              list<int>   _listLocations;
              list<ContainerBox> _listContainerLoad;
              list<ContainerBox> _listContainerLoaded;
              list<StackContainer> _listStacks;
              list<Cell> _listCells;
              /* calculated variables */
              
              
      public:
             // ------------------------ Properties -------------------------------
    		 /**
    		 *	NumPortsDischarge Property SET
    		 *	@param pNumPortsDischarge
    		 */
             void    SetNumPortsDischarge(int pNumPortsDischarge);
             /**
    		 *	LocationId Property GET
    		 */
             int     GetNumPortsDischarge();
             
             /**
    		 *	NumContainerLoad Property SET
    		 *	@param pNumContainerLoad
    		 */
             void    SetNumContainerLoad(int pNumContainerLoad);
             /**
    		 *	NumContainerLoad Property GET
    		 */
             int     GetNumContainerLoad();
             
             /**
    		 *	NumContainerLoaded Property SET
    		 *	@param pNumContainerLoaded
    		 */
             void    SetNumContainerLoaded(int pNumContainerLoaded);
             /**
    		 *	LocationId Property GET
    		 */
             int     GetNumContainerLoaded();
             
             /**
    		 *	NumStacks Property SET
    		 *	@param pNumStacks
    		 */
             void    SetNumStacks(int pNumStacks);
             /**
    		 *	NumStacks Property GET
    		 */
             int     GetNumStacks();
             
             /**
    		 *	NumCell Property SET
    		 *	@param pNumCell
    		 */
             void    SetNumCell(int pNumCell);
             /**
    		 *	NumCell Property GET
    		 */
             int     GetNumCell();
             
             /**
    		 *	NumLocations Property SET
    		 *	@param pNumLocations
    		 */
             void    SetNumLocations(int pNumLocations);
             /**
    		 *	NumLocations Property GET
    		 */
             int     GetNumLocations();
             
             /**
    		 *	NumTiers Property SET
    		 *	@param pNumTiers
    		 */
             void    SetNumTiers(int pNumTiers);
             /**
    		 *	NumTiers Property GET
    		 */
             int     GetNumTiers();
             
             /**
    		 *	ListPortsDischarges Property SET
    		 *	@param pListPortsDischarges
    		 */
             void    SetListPortsDischarges(list<int> pListPortsDischarges);
             /**
    		 *	ListPortsDischarges Property GET
    		 */
             list<int> GetListPortsDischarges();
             
             
             /**
    		 *	ListLocations Property SET
    		 *	@param pListLocations
    		 */
             void    SetListLocations(list<int> pListLocations);
             /**
    		 *	ListLocations Property GET
    		 */
             list<int> GetListLocations();
             
             
             /**
    		 *	ListContainerLoad Property SET
    		 *	@param pListContainerLoad
    		 */
             void    SetListContainerLoad(list<ContainerBox> pListContainerLoad);
             /**
    		 *	ListContainerLoad Property GET
    		 */
             list<ContainerBox> GetListContainerLoad();
             
             
             /**
    		 *	ListContainerLoaded Property SET
    		 *	@param pListContainerLoaded
    		 */
             void    SetListContainerLoaded(list<ContainerBox> pListContainerLoaded);
             /**
    		 *	ListContainerLoaded Property GET
    		 */
             list<ContainerBox> GetListContainerLoaded();
             
             /**
    		 *	ListStacks Property SET
    		 *	@param pListStacks
    		 */
             void    SetListStacks(list<StackContainer> pListStacks);
             /**
    		 *	ListStacks Property GET
    		 */
             list<StackContainer> GetListStacks();
             
             
             /**
    		 *	ListCells Property SET
    		 *	@param pListCells
    		 */
             void    SetListCells(list<Cell> pListCells);
             /**
    		 *	ListCells Property GET
    		 */
             list<Cell> GetListCells();
       
       
             // ------------------------ Methods ----------------------------------
    		/**
    		 *	Constructor
    		 */
    		StowageInfo();
    		/**
    		 *	Destructor
    		 */
    		~StowageInfo();
                          
};

#endif

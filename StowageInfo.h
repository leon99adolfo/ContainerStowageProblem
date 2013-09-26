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
#include <iostream>
#include <vector>

using namespace std;

class StowageInfo
{
      private:
              // ------------------------ Properties ------------------------------- 
              int                    _numPortsDischarge;
              int                    _numContainerLoad;
              int                    _numContainerLoaded;
              int                    _numStacks;
              int                    _numCell;
              int                    _numLocations;
              int                    _numTiers;
              vector<int>            _listPortsDischarges;
              vector<int>            _listLocations;
              vector<ContainerBox>   _listContainerLoad;
              vector<ContainerBox>   _listContainerLoaded;
              vector<StackContainer> _listStacks;
              vector<Cell>           _listCells;
              
              
              /**
               * Charge container in memory
               */
              void ChargeContainer(vector<ContainerBox> pListContainer, bool pValFirst);
                            
      public:
             /* Calculated variables */
            int         _nuMinLength;  // minimum length
            int         _nuMaxLength;  // maximum length
            double      _nuMinHeight;  // minimum height
            double      _nuMaxHeight;  // maximum height
            double      _nuMinWeight;  // minimum weight
            double      _nuMaxWeight;  // maximum weight
            double      _nuMinStackHeight; // Minimum height of the stack
            double      _nuMaxStackHeight; // Maximum height of the stack
			/* Model variables */
			vector<int>			Stacks;			// Stack index set
			vector<int>			Slots;			// Slot index set
            vector<int>			Cont;        	// Container index set
            vector<int>			Slots_A;     	// Aft slots index set
            vector<int>			Slots_F;     	// Fore slots index set
            vector<vector<int> >Slots_K;     	// Slots of stack K index set
            vector<vector<int> >Slots_K_A;   	// Aft slots of stack K index set
			vector<vector<int> >Slots_K_F;		// Fore slots of stack K index set
			vector<int>        	Slots_R;     	// Reefer slot index set
			vector<int>    		Slots_NR;    	// Non Reefer slot index set
			vector<int>    		Slots_NRC;   	// Slots in cell with no reefer plugs index set
            vector<int>        	Slots_20;    	// 20' capacity slots index set
            vector<int>      	Slots_40;    	// 40' capacity slots index set
			vector<int>        	Cont_V;      	// Virtual containers index set
            vector<int>        	Cont_L;      	// Loaded containers index set
            vector<int>         Cont_20;        // 20' containers index set
            vector<int>         Cont_40;        // 40' containers index set
            vector<int>         Cont_40_A;      // 40' containers index set (Aft)
            vector<int>         Cont_40_F;      // 40' containers index set (Fore)
            vector<int>         Cont_20_R;      // 20' reefer containers index set
            vector<int>         Cont_40_R;      // 40' reefer containers index set
            vector<int>         Cont_NR;        // Non-reefer containers index set
			vector<double>    	Weight;      	// Weight of container i
            vector<int>      	POD;         	// Ports of discharges of container i
            vector<int>      	Length;      	// Leight of container i
            vector<double>      Height;      	// Height of container i
			
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
             void    SetListPortsDischarges(vector<int> pListPortsDischarges);
             /**
    		 *	ListPortsDischarges Property GET
    		 */
             vector<int> GetListPortsDischarges();
             
             
             /**
    		 *	ListLocations Property SET
    		 *	@param pListLocations
    		 */
             void    SetListLocations(vector<int> pListLocations);
             /**
    		 *	ListLocations Property GET
    		 */
             vector<int> GetListLocations();
             
             
             /**
    		 *	ListContainerLoad Property SET
    		 *	@param pListContainerLoad
    		 */
             void    SetListContainerLoad(vector<ContainerBox> pListContainerLoad);
             /**
    		 *	ListContainerLoad Property GET
    		 */
             vector<ContainerBox> GetListContainerLoad();
             
             
             /**
    		 *	ListContainerLoaded Property SET
    		 *	@param pListContainerLoaded
    		 */
             void    SetListContainerLoaded(vector<ContainerBox> pListContainerLoaded);
             /**
    		 *	ListContainerLoaded Property GET
    		 */
             vector<ContainerBox> GetListContainerLoaded();
             
             /**
    		 *	ListStacks Property SET
    		 *	@param pListStacks
    		 */
             void    SetListStacks(vector<StackContainer> pListStacks);
             /**
    		 *	ListStacks Property GET
    		 */
             vector<StackContainer> GetListStacks();
             
             
             /**
    		 *	ListCells Property SET
    		 *	@param pListCells
    		 */
             void    SetListCells(vector<Cell> pListCells);
             /**
    		 *	ListCells Property GET
    		 */
             vector<Cell> GetListCells();
       
       
             // ------------------------ Methods ----------------------------------
    		/**
    		 *	Constructor
    		 */
    		StowageInfo();
    		/**
    		 *	Destructor
    		 */
    		~StowageInfo();
    		/**
    		 * Number of Slots
    		 */
    		int fnuSlots();
    		
    		/**
    		 * Charge Data
    		 */
    		void ChargeData();
                          
};

#endif

/**
 * @file	StowChannelCP
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * Model Stowage Problem with channeling.
 */
#ifndef StowChannelCP_h
#define StowChannelCP_h

#include <math.h> 
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include "StowageInfo.h"

using namespace Gecode;

class StowChannelCP: public IntMinimizeSpace
{
      private:
				// Index Set and Constant of the CP model
				IntArgs        		Cont_L;     // Loaded containers index set
				IntArgs           	Cont_20;    // 20' containers index set
				IntArgs           	Cont_40;    // 40' containers index set
				IntArgs           	Cont_20_R;  // 20' reefer containers index set
				IntArgs           	Cont_40_R;  // 40' reefer containers index set
				IntArgs				Weight;     // Weight of container i
				IntArgs 			POD;        // Ports of discharges of container i
				IntArgs				Length;     // Lenght of container i
				IntArgs				Height;     // Height of container i
				IntArgs				OverCont;     // Height of container i
                                   
      protected:
				IntVarArray     VSC;   	// Slots index of container j.
				IntVarArray     C;   	// Slots index of container j.
                IntVarArray     S;   	// Container index of slot j.
                IntVarArray     L;   	// Length of container stowed in slot j.
                IntVarArray   	H;   	// Height of container stowed in slot j.
                IntVarArray   	W;   	// Weight of container stowed in slot j.
                FloatVarArray  	WD;   	// Weight of container stowed in slot j.
                IntVarArray     P;   	// POD of container stowed in slot j.
                IntVarArray     HS;  	// Current height of stack k.         
                FloatVarArray	GCD;	// Gravity center distance
                IntVar			OGCTD;	// Gravity Center total distance
				IntVar			OCNS; 	// Number of container not stowed.				
                IntVar          OU;  // Number of used stacks. 
                IntVarArray     OP;  // Number of different discharge ports in each stack.
                IntVar          OR;  // Number of container non-reefers stowed in reefer cells.
                IntVar          O;   // Solution Cost.
  
      public:
            /**
             * Constructor:
             * 1 - Define range of the variables
             */             
            StowChannelCP(StowageInfo pStowageInfo);
            
            /**
             * Destructor
             */             
            ~StowChannelCP();
			
			/**
			* Charge Information in global variables
			*/
			void ChargeInformation(StowageInfo pStowageInfo);
			
			/**
			* This function save the maximum slot by stack
			*/
			void SaveContLoadedSlot(StowageInfo& pStowageInfo, map<int, int>& pSlotByStack, int pStack, int pSlot);
			
			/** 
			* Constructor overload
			*/
			StowChannelCP(bool share, StowChannelCP& s);
  
			/**
			*	Space
			*/
			virtual Space* copy(bool share);
						
			/**
			*	Printing solutions
			*/
			void print(int &pO, int &pOGCTD, int &pOR, string &pOP, int &pOPT, int &pOU, int &pOCNS, int &pOV, string &pS, int nuContainer) const;
			
			/**
			*	Cost function
			*/
			virtual IntVar cost(void) const;					
};

#endif

/**
 * @file	StowageCP
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * Model Stowage Problem.
 */
#ifndef StowageCP_h
#define StowageCP_h

#include <math.h> 
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include "StowageInfo.h"

using namespace Gecode;

class StowageCP: public IntMinimizeSpace
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
				IntArgs				ContNonReefer; // container non reefer
                                   
      protected:
                IntVarArray     S;   	// Container index of slot j.
                IntVarArray     L;   	// Length of container stowed in slot j.
                IntVarArray   	H;   	// Height of container stowed in slot j.
                IntVarArray   	W;   	// Weight of container stowed in slot j.
                FloatVarArray  	WD;   	// Weight of container stowed in slot j.
                IntVarArray     P;   	// POD of container stowed in slot j.
                IntVarArray     HS;  	// Current height of stack k.
                BoolVarArray    CFEU_A;	// Is container 40' Aft?
                BoolVarArray    CFEU_F;	// Is container 40' Fore?               
                FloatVarArray	GCD;	// Gravity center distance
                IntVar			OGCTD;	// Gravity Center total distance
                IntVar          OV;  	// Number of over-stowing containers.
				IntVarArray     OVT; 	// Container j over-stowing temporal.
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
            StowageCP(StowageInfo pStowageInfo);
            
            /**
             * Destructor
             */             
            ~StowageCP();
			
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
			StowageCP(bool share, StowageCP& s);
  
			/**
			*	Space
			*/
			virtual Space* copy(bool share);
						
			/**
			*	Printing solutions
			*/
			void print(int &pO, int &pOGCTD, int &pOR, string &pOP, int &pOPT, int &pOU, int &pOCNS, int &pOV, string &pS) const;
			
			/**
			*	Cost function
			*/
			virtual IntVar cost(void) const;					
};

#endif

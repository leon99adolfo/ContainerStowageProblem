/**
 * @file	StowageCP
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * Model Stowage Problem.
 */
#ifndef StowageCP_h
#define StowageCP_h

#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>
#include "StowageInfo.h"

using namespace Gecode;

class StowageCP: public IntMinimizeSpace
{
      private:
				// Index Set and Constant of the CP model
				/*IntArgs				Stacks;		// Stack index set
				IntArgs          	Slots;      // Slot index set
				IntArgs          	Cont;       // Container index set
				IntArgs          	Slots_A;    // Aft slots index set
				IntArgs          	Slots_F;    // Fore slots index set
				map<int, IntArgs> 	Slots_K;    // Slots of stack K index set
				map<int, IntArgs>	Slots_K_A;  // Aft slots of stack K index set
				map<int, IntArgs>	Slots_K_F;  // Fore slots of stack K index set
				IntArgs        	Slots_R;    // Reefer slot index set 
				IntArgs        		Slots_NR;   // Non Reefer slot index set
				IntArgs        		Slots_NRC;  // Slots in cell with no reefer plugs index set
				IntArgs        		Slots_20;   // 20' capacity slots index set
				IntArgs        		Slots_40;   // 40' capacity slots index set
				/*IntArgs        		Cont_V;     // Virtual containers index set */
				IntArgs        		Cont_L;     // Loaded containers index set
				IntArgs           	Cont_20;    // 20' containers index set
				IntArgs           	Cont_40;    // 40' containers index set
				IntArgs           	Cont_20_R;  // 20' reefer containers index set
				IntArgs           	Cont_40_R;  // 40' reefer containers index set
				/*IntArgs           	Cont_40_A;  // 40' containers index set (Aft)
				IntArgs           	Cont_40_F;  // 40' containers index set (Fore) */
				IntArgs				Weight;     // Weight of container i
				IntArgs 			POD;        // Ports of discharges of container i
				IntArgs				Length;     // Lenght of container i
				IntArgs				Height;     // Height of container i
				/*
				map<double, IntArgs>Cont_EH;    // Number of container with equal height
				IntArgs          	ContNormal; // Number of normal containers
				IntArgs           	ContCUBE;   // Number of high-cube containers
				IntArgs           	Stack_K_W;  // Weight  limit of stack K
				IntArgs           	Stack_K_H;  // Height  limit of stack K
				IntArgs           	Classes;    // Set of stack classes
				IntArgs           	Class;      // Set of stacks of class i*/
                                   
      protected:
                IntVarArray     S;   	// Container index of slot j.
                IntVarArray     L;   	// Length of container stowed in slot j.
                IntVarArray   	H;   	// Height of container stowed in slot j.
                IntVarArray   	W;   	// Weight of container stowed in slot j.
                FloatVarArray  	WD;   	// Weight of container stowed in slot j.
                IntVarArray     P;   	// POD of container stowed in slot j.
                IntVarArray     HS;  	// Current height of stack k.
                BoolVarArray    NVC;  	// No Virtual containers
                BoolVarArray    CFEU_A;	// Is container 40' Aft?
                BoolVarArray    CFEU_F;	// Is container 40' Fore?   
                FloatVarArray	GCX;	// Gravity center in axis X     
                FloatVarArray	SGCX;
                FloatVarArray	WT;
                FloatVarArray	GCY;	// Gravity center in axis Y                
                IntVar          OV;  	// Number of over-stowing containers.
				IntVarArray     OVT; 	// Container j over-stowing temporal.
				IntVar			OCNS; 	// Number of container not stowed.				
                IntVar          OU;  // Number of used stacks. 
                IntVarArray     OP;  // Number of different discharge ports in each stack.
                IntVar          OR;  // Number of container non-reefers stowed in reefer cells.
                IntVar          O;   // Solution Cost.
                
                
                //IntVarArray     SLE; // Slots with the same features in stack i.
  
      public:
            /**
             * Constructor:
             * 1 - Define range of the variables
             */             
            StowageCP(StowageInfo pStowageInfo);
			
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
			void print(void) const;
			
			/**
			*	Cost function
			*/
			virtual IntVar cost(void) const;
			
			 
};

#endif

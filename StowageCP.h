/**
 * @file	StowageCP
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * Model Stowage Problem.
 */


#include <gecode/int.hh>
#include "StowageInfo.h"
#include <vector>
///#include <gecode/search.hh>

using namespace Gecode;

class StowageCP: public Space
{
      private:
              // Index Set and Constant of the CP model
              IntArgs          	Stacks;      // Stack index set
              IntArgs          	Slots;       // Slot index set
              IntArgs          	Cont;        // Container index set
              IntArgs          	Slots_A;     // Aft slots index set
              IntArgs          	Slots_F;     // Fore slots index set
              vector<IntArgs>  	Slots_K;     // Slots of stack K index set
              vector<IntArgs>  	Slots_K_A;   // Aft slots of stack K index set
              vector<IntArgs>  	Slots_K_F;   // Fore slots of stack K index set
              IntArgs        	Slots_R;     // Reefer slot index set
              IntArgs        	Slots_NR;    // Non Reefer slot index set
              IntArgs        	Slots_NRC;   // Slots in cell with no reefer plugs index set
              IntArgs        	Slots_20;    // 20' capacity slots index set
              IntArgs        	Slots_40;    // 40' capacity slots index set
              IntArgs        	Cont_V;      // Virtual containers index set
              IntArgs        	Cont_L;      // Loaded containers index set
              IntArgs           Cont_20;     // 20' containers index set
              IntArgs           Cont_40;     // 40' containers index set
              IntArgs           Cont_40_A;   // 40' containers index set (Aft)
              IntArgs           Cont_40_F;   // 40' containers index set (Fore)
              IntArgs           Cont_20_R;   // 20' reefer containers index set
              IntArgs           Cont_40_R;   // 40' reefer containers index set
              IntArgs           Cont_NR;     // Non-reefer containers index set
              //IntArgs        	Weight;      // Weight of container i
              IntArgs        	POD;         // Ports of discharges of container i
              IntArgs        	Length;      // Leight of container i
              //IntArgs        	Height;      // Height of container i
              IntArgs        Cont_EP;     // Number of container with discharge port P.
              IntArgs        Cont_EWH;    // Number of container with equal weight and height
              int            ContNormal   // Number of normal containers
              int            ContCUBE     // Number of high-cube containers
              IntArgs        Stack_K_W;   // Weight  limit of stack K
              IntArgs        Stack_K_H;   // Height  limit of stack K
              IntArgs        Classes;     // Set of stack classes
              IntArgs        Class;       // Set of stacks of class i
                                    
      protected:
                IntVarArray     C;   // Slot index of container i.
                IntVarArray     S;   // Container index of slot j.
                IntVarArray     L;   // Length of container stowed in slot j.
                FloatVarArray   H;   // Heigth of container stowed in slot j.
                FloatVarArray   W;   // Weigth of container stowed in slot j.
                IntVarArray     P;   // POD of container stowed in slot j.
                FloatVarArray   HS;  // Current heigth of stack k.
                IntVar          OV;  // Number of overstowing containers.
                IntVar          OU;  // Number of used stacks.
                IntVarArray     OP;  // Number of different discharge ports in each stack.
                IntVar          OR;  // Number of non-reffers stowed in reffer cells.
                IntVar          O;   // Solution Cost.
                IntVarArray     CV;  // Virtual containers
                IntVarArray     SLE; // Slots with the same features in stack i.
  
      public:
             /**
              * Constructor:
              * 1 - Define range of the variables
              */             
             StowageCP(StowageInfo pStowageInfo);
};

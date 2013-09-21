/**
 * @file	StowageCP
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * Model Stowage Problem.
 */

#include <gecode/int.hh>
#include "StowageInfo.h"
///#include <gecode/search.hh>

using namespace Gecode;

class StowageCP: public Space
{
      private:
              // Index Set and Constant of the CP model
              IntVarArgs    Stacks;      // Stack index set
              IntVarArray    Slots;       // Slot index set
              IntVarArray    Cont;        // Container index set
              IntVarArray    Slots_A;     // Aft slots index set
              IntVarArray    Slots_F;     // Fore slots index set
              IntVarArray    Slots_K;     // Slots of stack K index set
              IntVarArray    Slots_K_A;   // Aft slots of stack K index set
              IntVarArray    Slots_K_F;   // Fore slots of stack K index set
              IntVarArray    Slots_R;     // Reefer slot index set
              IntVarArray    Slots_NR;    // Non Reefer slot index set
              IntVarArray    Slots_NRC;   // Slots in cell with no reefer plugs index set
              IntVarArray    Slots_20;    // 20' capacity slots index set
              IntVarArray    Slots_40;    // 40' capacity slots index set
              IntVarArray    Cont_V;      // Virtual containers index set
              IntVarArray    Cont_L;      // Loaded containers index set
              IntVarArray    Cont_20;     // 20' containers index set
              IntVarArray    Cont_40;     // 40' containers index set
              IntVarArray    Cont_40_A;   // 40' containers index set (Aft)
              IntVarArray    Cont_40_F;   // 40' containers index set (Fore)
              IntVarArray    Cont_20_R;   // 20' reefer containers index set
              IntVarArray    Cont_40_R;   // 40' reefer containers index set
              IntVarArray    Cont_NR;     // Non-reefer containers index set
              IntVarArray    Weight;      // Weight of container i
              IntVarArray    POD;         // Ports of discharges of container i
              IntVarArray    Length;      // Leight of container i
              IntVarArray    Hength;      // Height of container i
              IntVarArray    Cont_EP;     // Number of container with discharge port P.
              IntVarArray    Cont_EWH;    // Number of container with equal weight and height
              IntVar         ContNormal   // Number of normal containers
              IntVar         ContCUBE     // Number of high-cube containers
              IntVarArray    Stack_K_W;   // Weight  limit of stack K
              IntVarArray    Stack_K_H;   // Height  limit of stack K
              IntVarArray    Classes;     // Set of stack classes
              IntVarArray    Class;       // Set of stacks of class i
              /* Calculated variables */
              IntVar        nuMinLength;  // minimum length
              IntVar        nuMaxLength;  // maximum length
              IntVar        nuMinHeight;  // minimum height
              Intvar        nuMaxHeight;  // maximum height
              IntVar        nuMinWeight;  // minimum weight
              IntVar        nuMaxWeight;  // maximum weight
              IntVar        nuPOD;        // Number of ports of discharges
              IntVar        nuMaxStackHeight; // Maximum height of the stack
              
              
                                     
      protected:
                IntVarArray     C;   // Slot index of container i.
                IntVarArray     S;   // Container index of slot j.
                FloatVarArray   L;   // Length of container stowed in slot j.
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

#include "StowageCP.h"

StowageCP::StowageCP(StowageInfo pStowageInfo):
              // Domain Variables
              C  (*this, pStowageInfo.fnuSlots(), 0, (pStowageInfo.fnuSlots() - 1)),
              S  (*this, pStowageInfo.fnuSlots(), 0, (pStowageInfo.fnuSlots() - 1)),
              L  (*this, pStowageInfo.fnuSlots(), pStowageInfo._nuMinLength, pStowageInfo._nuMaxLength),
              H  (*this, pStowageInfo.fnuSlots(), pStowageInfo._nuMinHeight, pStowageInfo._nuMaxHeight),
              W  (*this, pStowageInfo.fnuSlots(), pStowageInfo._nuMinWeight, pStowageInfo._nuMaxWeight),
              P  (*this, pStowageInfo.fnuSlots(), 1, pStowageInfo.GetNumPortsDischarge()),
              HS (*this, pStowageInfo.GetNumStacks(), pStowageInfo._nuMinStackHeight, pStowageInfo._nuMaxStackHeight),
              OV (*this, 1, 0, pStowageInfo.fnuSlots()),
              OU (*this, 1, 0, pStowageInfo.GetNumStacks()),
              OP (*this, pStowageInfo.GetNumStacks(), 1, pStowageInfo.GetNumPortsDischarge()),
              OR (*this, 1, 0, pStowageInfo.fnuSlots()),
              O  (*this, 1, 0, ),
              CV (*this, , ,),
              SLE(*this, , ,),
{
   // Variable assigment
   Stacks     = IntArgs(pStowageInfo.GetNumStacks());
   Slots      = IntArgs(pStowageInfo.fnuSlots());
   Cont       = IntArgs(pStowageInfo.fnuSlots());
   Slots_A    = IntArgs(pStowageInfo.GetNumCell());
   Slots_F    = IntArgs(pStowageInfo.GetNumCell());  
        
   //---------------------------- sorts the arguments -------------------------- 
  
}

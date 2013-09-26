/*#include "StowageCP.h"

StowageCP::StowageCP(StowageInfo pStowageInfo):
              // Domain Variables
              C  (*this, pStowageInfo.fnuSlots(), 1, pStowageInfo.fnuSlots()),
              S  (*this, pStowageInfo.fnuSlots(), 1, pStowageInfo.fnuSlots()),
              L  (*this, pStowageInfo.fnuSlots(), pStowageInfo._nuMinLength, pStowageInfo._nuMaxLength),
              H  (*this, pStowageInfo.fnuSlots(), pStowageInfo._nuMinHeight, pStowageInfo._nuMaxHeight),
              W  (*this, pStowageInfo.fnuSlots(), pStowageInfo._nuMinWeight, pStowageInfo._nuMaxWeight),
              P  (*this, pStowageInfo.fnuSlots(), 1, pStowageInfo.GetNumPortsDischarge()),
              HS (*this, pStowageInfo.GetNumStacks(), pStowageInfo._nuMinStackHeight, pStowageInfo._nuMaxStackHeight),
              OV (*this, 0, pStowageInfo.fnuSlots(), ),
              OU (*this, 1, pStowageInfo.GetNumStacks(),  ),
              OP (*this, pStowageInfo.GetNumStacks(), 1, pStowageInfo.GetNumPortsDischarge()),
              OR (*this, , , ),
              O  (*this, , ,),
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
   // The cells 
   vector<Cell> listCells = pStowageInfo.GetListCells();
   // List of cells
   map<int,int> listStacksIdx;
   // iterations
   for(int x = 0; x < listCells.size(); x++)
   {
        // Assign argument slots
        Slots[(2 * x)] = (2 * x);
        Slots[(2 * x) + 1] = (2 * x) + 1;
        
        // Assign argument slots aft and slots fore
        Slots_A[(2 * x)] = (2 * x);
        Slots_F[(2 * x) + 1] = (2 * x) + 1;
           
        int nuStackIdTmp = listCells[x].GetStackId();
        // no found stack
        if(listStacksIdx.find( nuStackIdTmp ) == listStacksIdx.end())
        {
            // insert stack
            int nuValueStack = (listStacksIdx.size() == 0) ? 0 : listStacksIdx.size() + 1;
            listStacksIdx[ nuStackIdTmp ] = nuValueStack;               
            Stacks[ nuValueStack ] = nuStackIdTmp;
             
            //Slots_K.push_back(IntArgs( ));
			
             
             
        }
        else
        {
            
		}
   }
   
}*/

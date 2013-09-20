#include <gecode/int.hh>
///#include <gecode/search.hh>

using namespace Gecode;

class StowageCP: public Space
{
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
                IntVar          OP;  // Number of different discharge ports in each stack.
                IntVar          OR;  // Number of non-reffers stowed in reffer cells.
                IntVar          O;   // Solution Cost.
                IntVarArray     CV;  // Virtual containers
                IntVarArray     SLE; // Slots with the same features in stack i.
  
      public:
                       
                             
                  
                      
      
      
};

#include "StowageCP.h"

StowageCP::StowageCP(StowageInfo pStowageInfo):
              // Domain Variables
              C(*this, Cont.size(), 1, Slots.size()),
              S(*this, Slots.size(), 1, Cont.size()),
              L(*this, Slots.size(), nuMinLength, nuMaxLength),
              H(*this, Slots.size(), nuMinHeight, nuMaxHeight),
              W(*this, Slots.size(), nuMinWeight, nuMaxWeight),
              P(*this, Slots.size(), 1, nuPOD),
              HS(*this, Stacks.size(), 0, nuMaxStackHeight),
              OV(*this, 0, Cont.size()),
              OU(*this, 1, Stacks.size()),
              OP(*this, Stacks.size(), 1, nuPOD),
              OR(*this, )
{
    Stacks       
              
}

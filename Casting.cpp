#include "Casting.h"

Casting::Casting(Home home, Float::FloatView x0, Float::FloatView x1): 
	BinaryPropagator<Float::FloatView,Float::PC_FLOAT_BND>(home,x0,x1) {}

ExecStatus Casting::post(Home home, Float::FloatView x0, Float::FloatView x1) 
{
    (void) new (home) Casting(home,x0,x1);
    return ES_OK;
}
  
Casting::Casting(Space& home, bool share, Casting& p): 
	BinaryPropagator<Float::FloatView,Float::PC_FLOAT_BND>(home,share,p){}

Propagator* Casting::copy(Space& home, bool share) {
    return new (home) Casting(home,share,*this);
}
  
ExecStatus Casting::propagate(Space& home, const ModEventDelta&)  
{
    using namespace Float;
    // create rounding object
    Rounding r;
    // prune bounds
    GECODE_ME_CHECK(x1.eq(home, r.int_down(x0.max())));
    //return (x0.assigned() && x1.assigned()) ?
    return (x1.assigned()) ? home.ES_SUBSUMED(*this) : ES_NOFIX;
}


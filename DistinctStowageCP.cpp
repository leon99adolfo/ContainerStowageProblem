#include "DistinctStowageCP.h"

DistinctStowageCP::DistinctStowageCP(Home home, ViewArray<Int::IntView>& h):
	Propagator(home), x(h)
{
	x.subscribe(home, *this, Int::PC_INT_VAL);
}

ExecStatus DistinctStowageCP::post(Home home, ViewArray<Int::IntView>& x) 
{
    (void) new (home) DistinctStowageCP(home, x);
    return ES_OK;
}
  
size_t DistinctStowageCP::dispose(Space& home) 
{
    x.cancel(home, *this, Int::PC_INT_VAL);
    (void) Propagator::dispose(home);
    return sizeof(*this);
}
  
DistinctStowageCP::DistinctStowageCP(Space& home, bool share, DistinctStowageCP& p): 
	Propagator(home,share,p)
{
	x.update(home, share, p.x);
}

Propagator* DistinctStowageCP::copy(Space& home, bool share) {
    return new (home) DistinctStowageCP(home, share, *this);
}
  
PropCost DistinctStowageCP::cost(const Space&, const ModEventDelta&) const {
    return PropCost::quadratic(PropCost::LO, x.size());
}  
  
ExecStatus DistinctStowageCP::propagate(Space& home, const ModEventDelta&)  
{    
	for (int j = 0; j < x.size(); j++ )
	{
		if(x[j].min() == 0) return ES_NOFIX;
		
		for (int k = 0; k < x.size(); k++ )
		{
			if(x[k].min() == 0) return ES_NOFIX;
			if(j != k)
			{
				GECODE_ME_CHECK(x[j].nq(home, x[k].val()));
				GECODE_ME_CHECK(x[k].nq(home, x[j].val()));
				return home.ES_SUBSUMED(*this);
			}
		}
	}
    return ES_NOFIX;
}


/**
 * @file	DistinctStowageCP
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * DistinctStowageCP propagator.
 */
#ifndef DistinctStowageCP_h
#define DistinctStowageCP_h

#include <gecode/int.hh>
using namespace Gecode;

class DistinctStowageCP: public Propagator{
	
	protected:
		ViewArray<Int::IntView> x;
			
	public:
		DistinctStowageCP(Home home, ViewArray<Int::IntView>& h);
		
		static ExecStatus post(Home home, ViewArray<Int::IntView>& x);
		
		virtual size_t dispose(Space& home);
		
		DistinctStowageCP(Space& home, bool share, DistinctStowageCP& p);
			
		virtual Propagator* copy(Space& home, bool share);
		
		virtual PropCost cost(const Space&, const ModEventDelta&) const;
		
		virtual ExecStatus propagate(Space& home, const ModEventDelta&);
};

#endif

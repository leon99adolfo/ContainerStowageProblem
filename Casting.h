/**
 * @file	Casting
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * casting propagator.
 */
#ifndef Casting_h
#define Casting_h

#include <gecode/float.hh>
using namespace Gecode;

class Casting: public BinaryPropagator<Float::FloatView,Float::PC_FLOAT_BND> {
	
	public:
		Casting(Home home, Float::FloatView x0, Float::FloatView x1);
		
		static ExecStatus post(Home home, Float::FloatView x0, Float::FloatView x1);
		
		Casting(Space& home, bool share, Casting& p);
			
		virtual Propagator* copy(Space& home, bool share);
		
		virtual ExecStatus propagate(Space& home, const ModEventDelta&);
};

#endif

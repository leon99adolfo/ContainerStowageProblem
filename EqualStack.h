/**
 * @file	EqualStack
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * EqualStack class.
 */
 
#ifndef EqualStack_h
#define EqualStack_h 

#include "StackContainer.h"
#include <vector>
using namespace std;
 
class EqualStack
{

	public:
		// ------------------------ Properties -------------------------------
		StackContainer	Stack;
		vector<int> 	vecIdxStack;
		vector<Cell> 	Cells;
		
		
		// ------------------------ Methods -------------------------------	
		/**
		 *	Constructor
		 */
		EqualStack();
		/**
		 *	Destructor
		 */
		//~EqualStack();
};

#endif

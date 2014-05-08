/**
 * @file	EqualContainer
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * EqualContainer class.
 */
 
#ifndef EqualContainer_h
#define EqualContainer_h 

#include "ContainerBox.h"
#include <vector>
using namespace std;
 
class EqualContainer
{

	public:
		// ------------------------ Properties -------------------------------
		ContainerBox	Container;
		vector<int> 	vecIdxContainer;
		
		
		// ------------------------ Methods -------------------------------	
		/**
		 *	Constructor
		 */
		EqualContainer();
		/**
		 *	Destructor
		 */
		//~EqualContainer();
};

#endif

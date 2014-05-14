/**
 * @file	StackContainer
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * Stack class.
 */
#ifndef StackContainer_h
#define StackContainer_h

#include "Cell.h"

using namespace std;

class StackContainer
{
	private:
        // ------------------------ Properties -------------------------------    
		int _locationId;
		int _stackId;
		double _maxWeigth;
		double _maxHeigth;
		
	public:
        // ------------------------ Properties -------------------------------
		/**
		 *	LocationId Property SET
		 *	@param pLocationId
		 */
		void	SetLocationId(int pLocationId);
		/**
		 *	LocationId Property GET
		 */
		int	GetLocationId();
		
		/**
		 *	LocationId Property SET
		 *	@param pLocationId
		 */
		void	SetStackId(int pStackId);
		/**
		 *	StackId Property GET
		 */
		int	GetStackId();
		
		/**
		 *	MaxWeigth Property SET
		 *	@param pMaxWeigth
		 */
		void	SetMaxWeigth(double pMaxWeigth);
		/**
		 *	MaxWeigth Property GET
		 */
		double	GetMaxWeigth();
		
		/**
		 *	MaxHeigth Property SET
		 *	@param pMaxHeigth
		 */
		void	SetMaxHeigth(double pMaxHeigth);
		/**
		 *	MaxHeigth Property GET
		 */
		double	GetMaxHeigth();
		
		// ------------------------ Methods ----------------------------------
		/**
		 *	Constructor
		 */
		StackContainer();
		/**
		 *	Destructor
		 */
		//~StackContainer();
		
};

#endif

/**
 * @file	Cell
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * Cell class.
 */
#ifndef Cell_h
#define Cell_h

#include "ContainerBox.h"

using namespace std;

class Cell
{
	private:
		// ------------------------ Properties -------------------------------
		// StaackId isReeferFore IsReeferAft CapFore CapAft Cap40 numLocation
		int         _stackId;
        int         _isReeferFore;
        int         _isReeferAft;
        int         _capFore;
        int         _capAft;
        int         _cap40;
        int         _numLocation;
        int         _numLevel;

	public:
		// ------------------------ Properties -------------------------------
		/**
		 *	StackId Property SET
		 *	@param pStackId	
		 */
		void	SetStackId(int pStackId);
		/**
		 *	StackId Property GET
		 */
		int     GetStackId();
        
        /**
		 *	IsReeferFore Property SET
		 *	@param pIsReeferFore	
		 */
		void	SetIsReeferFore(int pIsReeferFore);
		/**
		 *	IsReeferFore Property GET
		 */
		int	GetIsReeferFore();

		/**
		 *	IsReeferAft Property SET
		 *	@param pIsReeferAft	
		 */
		void	SetIsReeferAft(int pIsReeferAft);
		/**
		 *	IsReeferAft Property GET
		 */
		int	GetIsReeferAft();

        /**
		 *	CapFore Property SET
		 *	@param pCapFore
		 */
		void	SetCapFore(int pCapFore);
		/**
		 *	CapFore Property GET
		 */
		int     GetCapFore();


        /**
		 *	CapAft Property SET
		 *	@param pCapAft
		 */
		void	SetCapAft(int pCapAft);
		/**
		 *	CapAft Property GET
		 */
		int     GetCapAft();
		
		/**
		 *	Cap40 Property SET
		 *	@param pCap40
		 */
		void	SetCap40(int pCap40);
		/**
		 *	Cap40 Property GET
		 */
		int     GetCap40();
		
		/**
		 *	NumLocation Property SET
		 *	@param pNumLocation
		 */
		void	SetNumLocation(int pNumLocation);
		/**
		 *	NumLocation Property GET
		 */
		int     GetNumLocation();

		/**
		 *	NumLevel Property SET
		 *	@param pNumLevel
		 */
		void	SetNumLevel(int pNumLevel);
		/**
		 *	NumLevel Property GET
		 */
		int     GetNumLevel();

		// ------------------------ Methods ----------------------------------
		/**
		 *	Constructor
		 */
		Cell();
		/**
		 *	Destructor
		 */
		//~Cell();

		
};

#endif

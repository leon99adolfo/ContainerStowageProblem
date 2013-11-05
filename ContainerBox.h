/**
 * @file	ContainerBox
 * @author  Adolfo Leon Canizales Murcia (leon99adolfo@gmail.com)
 * @version 1.0
 * @section DESCRIPTION
 * Container class.
 */
 
#ifndef ContainerBox_h
#define ContainerBox_h 
 
class ContainerBox
{
	private:
		// ------------------------ Attributs -------------------------------
		int     _stackId;
		int     _cellId;
		int     _position;
		double	_weigth;
		double	_heigth;
		int		_length;
		int		_portDischarge;
		int 	_isReefer;
		int     _location;
		bool    _isCharged;
		

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
		int	GetStackId();
		
		/**
		 *	CellId Property SET
		 *	@param pCellId	
		 */
		void	SetCellId(int pCellId);
		/**
		 *	CellId Property GET
		 */
		int	GetCellId();
		
		/**
		 *	Position Property SET
		 *	@param pPosition	
		 */
		void	SetPosition(int pPosition);
		/**
		 *	Position Property GET
		 */
		int	GetPosition();
		
		/**
		 *	Weigth Property SET
		 *	@param pWeigth	
		 */
		void	SetWeight(double pWeigth);
		/**
		 *	Weigth Property GET
		 */
		double	GetWeight();

		/**
		 *	Heigth Property SET
		 *	@param pHeigth	
		 */
		void	SetHeight(double pHeigth);
		/**
		 *	Heigth Property GET
		 */
		double	GetHeight();

		/**
		 *	Length Property SET
		 *	@param pLength	
		 */
		void	SetLength(int pLength);
		/**
		 *	Length Property GET
		 */
		int	GetLength();

		/**
		 *	PortDischarge Property SET
		 *	@param pHeigth	
		 */
		void	SetPortDischarge(int pPortDischarge);
		/**
		 *	PortDischarge Property GET
		 */
		int	GetPortDischarge();

		/**
		 *	IsReefer Property SET
		 *	@param pIsReefer	
		 */
		void	SetIsReefer(int pIsReefer);
		/**
		 *	IsReefer Property GET
		 */
		int	GetIsReefer();

        /**
		 *	Location Property SET
		 *	@param pLocation	
		 */
		void	SetLocation(int pLocation);
		/**
		 *	Location Property GET
		 */
		int	GetLocation();

        /**
		 *	IsCharged Property SET
		 *	@param pIsCharged	
		 */
		void	SetIsCharged(bool pIsCharged);
		/**
		 *	IsCharged Property GET
		 */
		bool	GetIsCharged();

		// ------------------------ Methods -------------------------------	
		/**
		 *	Constructor
		 */
		ContainerBox();
		/**
		 *	Destructor
		 */
		//~ContainerBox();



};

#endif

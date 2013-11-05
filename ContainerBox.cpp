#include "ContainerBox.h"


// ------------------------ Properties -------------------------------

void	ContainerBox::SetStackId(int pStackId)
{
     _stackId = pStackId;        
}
		
int	    ContainerBox::GetStackId()
{
     return _stackId;       
}

void	ContainerBox::SetCellId(int pCellId)
{
     _cellId = pCellId;   
}

int	   ContainerBox::GetCellId()
{
     return _cellId; 
}

void	ContainerBox::SetPosition(int pPosition)
{
     _position = pPosition;   
}

int	ContainerBox::GetPosition()
{
     return _position;  
}

void	ContainerBox::SetWeight(double pWeigth)
{
	_weigth = pWeigth;
}

double	ContainerBox::GetWeight()
{
	return _weigth; 
}

void	ContainerBox::SetHeight(double pHeigth)
{
	_heigth = pHeigth;
}

double	ContainerBox::GetHeight()
{
	return _heigth;
}

void	ContainerBox::SetLength(int pLength)
{
	_length = pLength;
}

int	ContainerBox::GetLength()
{
	return _length;
}

void	ContainerBox::SetPortDischarge(int pPortDischarge)
{
	_portDischarge = pPortDischarge;
}

int	ContainerBox::GetPortDischarge()
{
	return _portDischarge;
}

void	ContainerBox::SetIsReefer(int pIsReefer)
{
	_isReefer = pIsReefer;
}

int	ContainerBox::GetIsReefer()
{
	return _isReefer;
}

void	ContainerBox::SetLocation(int pLocation)
{
	_location = pLocation;
}

int	ContainerBox::GetLocation()
{
	return _location;
}

void	ContainerBox::SetIsCharged(bool pIsCharged)
{
	_isCharged = pIsCharged;
}

bool	ContainerBox::GetIsCharged()
{
	return _isCharged;
}

// ------------------------ Methods ----------------------------------
ContainerBox::ContainerBox(){}
//ContainerBox::~ContainerBox(){}

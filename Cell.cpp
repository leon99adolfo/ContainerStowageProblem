#include "Cell.h"



// ------------------------ Properties -------------------------------

void	Cell::SetStackId(int pStackId)
{
        _stackId = pStackId;    
}

int     Cell::GetStackId()
{
        return _stackId;         
}

void	Cell::SetIsReeferFore(int pIsReeferFore)
{
        _isReeferFore = pIsReeferFore;     
}

int	Cell::GetIsReeferFore()
{
        return _isReeferFore;    
}

void	Cell::SetIsReeferAft(int pIsReeferAft)
{
        _isReeferAft = pIsReeferAft;     
}

int	Cell::GetIsReeferAft()
{
        return _isReeferAft;    
}

void	Cell::SetCapFore(int pCapFore)
{
        _capFore = pCapFore;       
}

int     Cell::GetCapFore()
{
        return _capFore;        
}

void	Cell::SetCapAft(int pCapAft)
{
        _capAft = pCapAft;      
}

int     Cell::GetCapAft()
{
        return _capAft;        
}

void	Cell::SetCap40(int pCap40)
{
        _cap40 = pCap40;     
}

int     Cell::GetCap40()
{
        return _cap40;        
}

void	Cell::SetNumLocation(int pNumLocation)
{
        _numLocation = pNumLocation;
}

int     Cell::GetNumLocation()
{
        return _numLocation;       
}


// ------------------------ Methods ----------------------------------
Cell::Cell(){}
//Cell::~Cell(){}


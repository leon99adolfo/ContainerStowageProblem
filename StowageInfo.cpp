#include "StowageInfo.h"

// ------------------------ Properties -------------------------------
void    StowageInfo::SetNumPortsDischarge(int pNumPortsDischarge)
{
        _numPortsDischarge = pNumPortsDischarge;
}
        
int     StowageInfo::GetNumPortsDischarge()
{
        return _numPortsDischarge;        
}

void    StowageInfo::SetNumContainerLoad(int pNumContainerLoad)
{
        _numContainerLoad = pNumContainerLoad;
}

int     StowageInfo::GetNumContainerLoad()
{
        return _numContainerLoad;        
}

void    StowageInfo::SetNumContainerLoaded(int pNumContainerLoaded)
{
        _numContainerLoaded = pNumContainerLoaded;
}

int     StowageInfo::GetNumContainerLoaded()
{
        return _numContainerLoaded;        
}

void    StowageInfo::SetNumStacks(int pNumStacks)
{
        _numStacks = pNumStacks;       
}
        
int     StowageInfo::GetNumStacks()
{
        return _numStacks;         
}

void    StowageInfo::SetNumCell(int pNumCell)
{
        _numCell = pNumCell;                
}

int     StowageInfo::GetNumCell()
{
        return _numCell;        
}
        
void    StowageInfo::SetNumLocations(int pNumLocations)
{
        _numLocations = pNumLocations;         
}

int     StowageInfo::GetNumLocations()
{
        return _numLocations;        
}

void    StowageInfo::SetNumTiers(int pNumTiers)
{
        _numTiers = pNumTiers;        
}

int     StowageInfo::GetNumTiers()
{
        return _numTiers;      
}

void    StowageInfo::SetListPortsDischarges(list<int> pListPortsDischarges)
{
        _listPortsDischarges = pListPortsDischarges;       
}
             
list<int> StowageInfo::GetListPortsDischarges()
{
        return _listPortsDischarges;      
}

void    StowageInfo::SetListLocations(list<int> pListLocations)
{
        _listLocations = pListLocations;      
}

list<int> StowageInfo::GetListLocations()
{
          return _listLocations;       
}

void    StowageInfo::SetListContainerLoad(list<ContainerBox> pListContainerLoad)
{
        _listContainerLoad = pListContainerLoad;        
}

list<ContainerBox> StowageInfo::GetListContainerLoad()
{
        return _listContainerLoad;                 
}

void    StowageInfo::SetListContainerLoaded(list<ContainerBox> pListContainerLoaded)
{
        _listContainerLoaded = pListContainerLoaded;        
}

list<ContainerBox> StowageInfo::GetListContainerLoaded()
{
        return _listContainerLoaded;                 
}

void    StowageInfo::SetListStacks(list<StackContainer> pListStacks)
{
        _listStacks = pListStacks;        
}

list<StackContainer> StowageInfo::GetListStacks()
{
        return _listStacks;                   
}


void    StowageInfo::SetListCells(list<Cell> pListCells)
{
        _listCells = pListCells;        
}

list<Cell> StowageInfo::GetListCells()
{
        return _listCells;          
}

// ------------------------ Methods ----------------------------------
StowageInfo::StowageInfo(){}
StowageInfo::~StowageInfo(){}

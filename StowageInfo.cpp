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


void    StowageInfo::SetListPortsDischarges(vector<int> pListPortsDischarges)
{
        _listPortsDischarges = pListPortsDischarges;       
}
             
vector<int> StowageInfo::GetListPortsDischarges()
{
        return _listPortsDischarges;      
}

void    StowageInfo::SetListLocations(vector<int> pListLocations)
{
        _listLocations = pListLocations;      
}

vector<int> StowageInfo::GetListLocations()
{
          return _listLocations;       
}

void    StowageInfo::SetListContainerLoad(vector<ContainerBox> pListContainerLoad)
{
        _listContainerLoad = pListContainerLoad;        
}

vector<ContainerBox> StowageInfo::GetListContainerLoad()
{
        return _listContainerLoad;                 
}

void    StowageInfo::SetListContainerLoaded(vector<ContainerBox> pListContainerLoaded)
{
        _listContainerLoaded = pListContainerLoaded;        
}

vector<ContainerBox> StowageInfo::GetListContainerLoaded()
{
        return _listContainerLoaded;                 
}

void    StowageInfo::SetListStacks(vector<StackContainer> pListStacks)
{
        _listStacks = pListStacks;        
}

vector<StackContainer> StowageInfo::GetListStacks()
{
        return _listStacks;                   
}

void    StowageInfo::SetListCells(vector<Cell> pListCells)
{
        _listCells = pListCells;        
}

vector<Cell> StowageInfo::GetListCells()
{
        return _listCells;          
}

// ------------------------ Methods ----------------------------------
StowageInfo::StowageInfo()
{
    ContNormal = 0;  
    ContCUBE   = 0;                       
}

StowageInfo::~StowageInfo(){}

int StowageInfo::fnuSlots()
{
    return _listCells.size() * 2;
}

void StowageInfo::ChargeContainer(vector<ContainerBox> pListContainer, bool pValFirst)
{
     for(int x = 0; x < pListContainer.size(); x++)
     {
          if(x == 0 && pValFirst)
          {
               _nuMinLength = _nuMaxLength = pListContainer.at(x).GetLength();
               _nuMinHeight = _nuMaxHeight = pListContainer.at(x).GetHeight();
               _nuMinWeight = _nuMaxWeight = pListContainer.at(x).GetWeight();     
          }
          else
          {
               //  Minimum and maximum Length
               if(pListContainer.at(x).GetLength() < _nuMinLength)
               {
                    _nuMinLength = pListContainer.at(x).GetLength();
               }
               else if(pListContainer.at(x).GetLength() > _nuMaxLength)
               {
                    _nuMaxLength = pListContainer.at(x).GetLength();                   
               }
               
               //  Minimum and maximum Heigth
               if(pListContainer.at(x).GetHeight() < _nuMinHeight)
               {
                    _nuMinHeight = pListContainer.at(x).GetHeight();
               }
               else if(pListContainer.at(x).GetHeight() > _nuMaxHeight)
               {
                    _nuMaxHeight = pListContainer.at(x).GetHeight();                   
               }
               
               //  Minimum and maximum Weigth
               if(pListContainer.at(x).GetWeight() < _nuMinWeight)
               {
                    _nuMinWeight = pListContainer.at(x).GetWeight();
               }
               else if(pListContainer.at(x).GetWeight() > _nuMaxWeight)
               {
                    _nuMaxWeight = pListContainer.at(x).GetWeight();                   
               }    
          }
     }  
}

void StowageInfo::ChargeData()
{
     // Container load
     ChargeContainer(_listContainerLoad, true);
     // Container loaded
     ChargeContainer(_listContainerLoaded, false);  
     
     cout<<"_nuMinLength: "<<_nuMinLength<<endl;
     cout<<"_nuMaxLength: "<<_nuMaxLength<<endl;
     cout<<"_nuMinHeight: "<<_nuMinHeight<<endl;
     cout<<"_nuMaxHeight: "<<_nuMaxHeight<<endl;
     cout<<"_nuMinWeight: "<<_nuMinWeight<<endl;
     cout<<"_nuMaxWeight: "<<_nuMaxWeight<<endl;
        
     // Stacks
     for(int x = 0; x < _listStacks.size(); x++)
     {
          if(x == 0)
          {
               _nuMinStackHeight = _nuMaxStackHeight = _listStacks.at(x).GetMaxHeigth();
          }
          else
          {
               //  Minimum and maximum Length
               if(_listStacks.at(x).GetMaxHeigth() < _nuMinStackHeight)
               {
                    _nuMinStackHeight = _listStacks.at(x).GetMaxHeigth();
               }
               else if(_listStacks.at(x).GetMaxHeigth() > _nuMaxStackHeight)
               {
                    _nuMaxStackHeight = _listStacks.at(x).GetMaxHeigth();                   
               }  
          }
     }   
     
     cout<<"_nuMinStackHeight: "<<_nuMinStackHeight<<endl;
     cout<<"_nuMaxStackHeight: "<<_nuMaxStackHeight<<endl;
     
           
}




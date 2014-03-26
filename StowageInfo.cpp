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

void    StowageInfo::SetListContainerLoad(map<int, ContainerBox> pListContainerLoad)
{
        _listContainerLoad = pListContainerLoad;        
}

map<int, ContainerBox> StowageInfo::GetListContainerLoad()
{
        return _listContainerLoad;                 
}

void    StowageInfo::SetListContainerLoaded(map<int, ContainerBox> pListContainerLoaded)
{
        _listContainerLoaded = pListContainerLoaded;        
}

map<int, ContainerBox> StowageInfo::GetListContainerLoaded()
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

//StowageInfo::~StowageInfo(){}

void StowageInfo::ChargeContainer(map<int, ContainerBox> pListContainer, bool pValFirst)
{
	 bool boFirst = true;
	 for(map<int, ContainerBox >::iterator it=pListContainer.begin(); it != pListContainer.end(); ++it)
     {
          if(boFirst && pValFirst)
          {	
			   boFirst = false;
               _nuMaxLength = (it->second).GetLength();
               _nuMaxHeight = (it->second).GetHeight();               
               _nuMaxWeight = (it->second).GetWeight();
          }
          else
          {
               //  Maximum Length
               if((it->second).GetLength() > _nuMaxLength)
               {
                    _nuMaxLength = (it->second).GetLength();                   
               }
               
               //  Maximum Heigth
               if((it->second).GetHeight() > _nuMaxHeight)
               {
                    _nuMaxHeight = (it->second).GetHeight();                   
               }
               
               //  Maximum Weigth
               if((it->second).GetWeight() > _nuMaxWeight)
               {
                    _nuMaxWeight = (it->second).GetWeight();                   
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
     
    cout<<"_nuMaxLength: "<<_nuMaxLength<<endl;
    cout<<"_nuMaxHeight: "<<_nuMaxHeight<<endl;
    cout<<"_nuMaxWeight: "<<_nuMaxWeight<<endl;
        
    // Stacks
    for(int x = 0; x < _listStacks.size(); x++)
    {
        if(x == 0)
        {
            _nuMaxStackHeight = _listStacks.at(x).GetMaxHeigth();
        }
        else
        {
            //  Maximum Height
            if(_listStacks.at(x).GetMaxHeigth() > _nuMaxStackHeight)
            {
                _nuMaxStackHeight = _listStacks.at(x).GetMaxHeigth();                   
            }  
        }
    }   
     
    cout<<"_nuMaxStackHeight: "<<_nuMaxStackHeight<<endl;
}


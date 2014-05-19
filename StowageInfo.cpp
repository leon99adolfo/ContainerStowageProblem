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

void    StowageInfo::SetNumVirtualCont(int pNumVirtualCont)
{
        _numVirtualCont = pNumVirtualCont;
}

int     StowageInfo::GetNumVirtualCont()
{
        return _numVirtualCont;
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

void    StowageInfo::SetListStacks(map<int, StackContainer> pListStacks)
{
        _listStacks = pListStacks;        
}

map<int, StackContainer> StowageInfo::GetListStacks()
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
    WeightTotal = 0;     
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

bool StowageInfo::IsValidLoadedCont()
{
	for(map<int, map<int, int> >::iterator it=ContLoadedByStackCell.begin(); it != ContLoadedByStackCell.end(); ++it)
	{
		map<int, int> cellsByStack = it->second;
		for(int x = 1; x <= cellsByStack.size(); x++)
		{
			bool nullCell = false;
			for(int y = 0; y < CellNull[it->first].size(); y++)
			{
				if((CellNull[it->first][y] + 1) == x) 
				{	
					nullCell = true;
					break;
				}
			}
			
			if(!nullCell && cellsByStack.find(x) == cellsByStack.end()) 
			{
				cout<<"pila: "<<it->first<<" celda: "<<x<<endl;
				return false;
			}
		}
	}
	return true;
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
    map<int, int> copySlotRByStack;
    for(int x = 0; x < _listStacks.size(); x++)
	{
		copySlotRByStack[x+1] = SlotRByStack[x+1];
	}
    
    for(int x = 0; x < _listStacks.size(); x++)
    {
        if(x == 0)
        {
            _nuMaxStackHeight = _listStacks[(x+1)].GetMaxHeigth();
        }
        else
        {
            //  Maximum Height
            if(_listStacks[(x+1)].GetMaxHeigth() > _nuMaxStackHeight)
            {
                _nuMaxStackHeight = _listStacks[(x+1)].GetMaxHeigth();                   
            }  
        }
        
        int minNumberSlotR = INT_MAX;
        int minStack = INT_MAX;        
        for(map<int, int>::iterator it=copySlotRByStack.begin(); it != copySlotRByStack.end(); ++it)
		{	
			if(minNumberSlotR > (it->second)) 
			{
				minNumberSlotR = (it->second);
				minStack = (it->first);
			}
        }
        SortSlotRByStack.push_back(minStack);
        copySlotRByStack.erase(minStack);        
    }   
    
    // stack class 
    int nuClass = 0;
    for(int x = 0; x < SameStackTmp.size(); x++)
    {
		if(SameStackTmp[x].vecIdxStack.size() <= 1) continue;
		
		int nuCantidad = SameStack.size();
		for(int y = 0; y < SameStackTmp[x].vecIdxStack.size(); y++)
		{
			vector<Cell> cells1 = ListCellByStack[SameStackTmp[x].vecIdxStack[y]];
			
			// -------------------------------------------------------------------
			bool existClass = false;
			for(int z = nuCantidad; z < SameStack.size(); z++)  // aleoncm
			{		
				if(SameStack[z].Cells.size() != cells1.size()) break;
				
				bool equalAllCells = true;
				for(int w = 0; w < SameStack[z].Cells.size(); w++)
				{
					if(!(SameStack[z].Cells[w].GetIsReeferFore() ==  cells1[w].GetIsReeferFore() &&
						SameStack[z].Cells[w].GetIsReeferAft() ==  cells1[w].GetIsReeferAft() &&
						SameStack[z].Cells[w].GetCapFore() ==  cells1[w].GetCapFore() &&
						SameStack[z].Cells[w].GetCapAft() ==  cells1[w].GetCapAft() &&
						SameStack[z].Cells[w].GetCap40() ==  cells1[w].GetCap40() &&
						SameStack[z].Cells[w].GetNumLocation() ==  cells1[w].GetNumLocation()) )
					{
						equalAllCells = false;
					}
				}
				
				if(equalAllCells)
				{
					SameStack[z].vecIdxStack.push_back(SameStackTmp[x].vecIdxStack[y]);
					existClass = true;
					break;
				}					
			}
				
			if(!existClass)
			{
				EqualStack objEqStack;
				objEqStack.Stack = SameStackTmp[x].Stack;
				objEqStack.Cells = cells1;
				objEqStack.vecIdxStack.push_back(SameStackTmp[x].vecIdxStack[y]);
				SameStack.push_back(objEqStack);
			}
		}
	}
     
    cout<<"_nuMaxStackHeight: "<<_nuMaxStackHeight<<endl;
}


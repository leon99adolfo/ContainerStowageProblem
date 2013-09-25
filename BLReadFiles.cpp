#include "BLReadFiles.h"


// ------------------------ Methods ----------------------------------
BLReadFiles::BLReadFiles(){}

BLReadFiles::~BLReadFiles(){} 

StowageInfo BLReadFiles::ChargeFile(string pFileName)
{
    // Variables
    StowageInfo response;
    // Varibles of firts line
    int nuPortsDischarge, nuContainerLoad, nuContainerLoaded, 
        nuStacks, nuCell, nuLocations, nuTiers;
    // Variable Dummy
    string sbTagDummy;
    // Variables for discharges ports 
    int nuPortDischarge;
    vector<int> listPortDischarge;
    // Variables for locations
    int nuLocation;
    vector<int> listLocations;
    // Variables for stacks
    int nuLocationStack;
    double nuMaxWeigth, nuMaxHeigth;
    vector<StackContainer> listStacks;
    // variables for cell
    int nuStackIdCell, nuIsReeferFore, nuIsReeferAft, nuCapFore, 
        nuCapAft, nuCap40, nuLocationCell;
    vector<Cell> listCells;

    // Open file
    archivoAr.open(pFileName.c_str() , ios::out);
    if( !archivoAr.is_open() )
    {
        cout<<"Can't open input file"<<endl;
        return response;
    }   
    
    // Read first line
    archivoAr>>nuPortsDischarge>>nuContainerLoad>>nuContainerLoaded
             >>nuStacks>>nuCell>>nuLocations>>nuTiers;
    
             
    cout<<nuPortsDischarge<<" "<<nuContainerLoad<<" "<<nuContainerLoaded<<" "
        <<nuStacks<<" "<<nuCell<<" "<<nuLocations<<" "<<nuTiers<<endl;
             
    // Save first line
    response.SetNumPortsDischarge(nuPortsDischarge);
    response.SetNumContainerLoad(nuContainerLoad);
    response.SetNumContainerLoaded(nuContainerLoaded);
    response.SetNumStacks(nuStacks);
    response.SetNumCell(nuCell);
    response.SetNumLocations(nuLocations);
    response.SetNumTiers(nuTiers);
    
    // Read Tag #POD
    archivoAr>>sbTagDummy;
    cout<<sbTagDummy<<endl;    
    
    // Read discharges ports 
    for(int x = 0; x < nuPortsDischarge; x++)
	{
         archivoAr>>nuPortDischarge;   
         cout<<nuPortDischarge<<endl;     
         listPortDischarge.push_back(nuPortDischarge);		
	}
	
	// Save discharges ports
	response.SetListPortsDischarges(listPortDischarge);
    
    // Read Tag #LOCATIONS
    archivoAr>>sbTagDummy;
    cout<<sbTagDummy<<endl;
    
    // Read locations 
    for(int x = 0; x < nuLocations; x++)
	{
         archivoAr>>nuLocation;        
         cout<<nuLocation<<endl;
         listLocations.push_back(nuLocation);		
	}
    
    // Save locations
	response.SetListLocations(listLocations);
		
    // Read Tag #CONTAINERS_TOLOAD
    archivoAr>>sbTagDummy;
    cout<<sbTagDummy<<endl;
 
    // Save Container Load
    response.SetListContainerLoad(ReadContainer(nuContainerLoad, false));
    
    // Read Tag #CONTAINERS_TOLOADED
    archivoAr>>sbTagDummy;
    cout<<sbTagDummy<<endl;
    
    // Save Container Load
    response.SetListContainerLoad(ReadContainer(nuContainerLoaded, true));
    
    // Read Tag #STACKS
    archivoAr>>sbTagDummy;
    cout<<sbTagDummy<<endl;
    
    // Read stacks 
    for(int x = 0; x < nuStacks; x++)
	{
        archivoAr>>nuMaxWeigth>>nuMaxHeigth>>nuLocationStack; 
        cout<<nuMaxWeigth<<" "<<nuMaxHeigth<<" "<<nuLocationStack<<endl; 
         
        StackContainer objStack;
        objStack.SetMaxWeigth(nuMaxWeigth);
        objStack.SetMaxHeigth(nuMaxHeigth);
        objStack.SetLocationId(nuLocationStack);
                         
        listStacks.push_back(objStack);		
	}
    
    response.SetListStacks(listStacks);
    
    // Read Tag #CELLS
    archivoAr>>sbTagDummy;
    cout<<sbTagDummy<<endl;
            
    // Read Cells 
    for(int x = 0; x < nuCell; x++)
	{    
        archivoAr>>nuStackIdCell>>nuIsReeferFore>>nuIsReeferAft>>nuCapFore>>
                   nuCapAft>>nuCap40>>nuLocationCell;
                    
        cout<<nuStackIdCell<<" "<<nuIsReeferFore<<" "<<nuIsReeferAft<<" "<<nuCapFore<<" "<<
              nuCapAft<<" "<<nuCap40<<" "<<nuLocationCell<<endl;

		/* Fill Object */
        Cell objCell;
        objCell.SetStackId(nuStackIdCell);
        objCell.SetIsReeferFore(nuIsReeferFore);
        objCell.SetIsReeferAft(nuIsReeferAft);
        objCell.SetCapFore(nuCapFore);
        objCell.SetCapAft(nuCapAft);
        objCell.SetCap40(nuCap40);
        objCell.SetNumLocation(nuLocationCell);
         
        listCells.push_back(objCell);
		
		/* Fill index set */
		int	idxFirstTemp	= ( x * 2 );
		int	idxSecondTemp 	= ( x * 2 ) + 1;
		
		// Insert Slots
		response.Slots.push_back( idxFirstTemp );
		response.Slots.push_back( idxSecondTemp );

		// Insert Cont
		response.Cont.push_back( idxFirstTemp );
		response.Cont.push_back( idxSecondTemp );
		
		// Insert Slots A
		response.Slots_A.push_back( idxFirstTemp );
		
		// Insert Slots F
		response.Slots_F.push_back( idxSecondTemp );
		
		// Insert slots reefer and not reefer
		if( nuIsReeferAft  == 1 ) 
		{
			response.Slots_R.push_back( idxFirstTemp );
		}
		else
		{
			response.Slots_NR.push_back( idxFirstTemp );
			// Slots in cell with no plug reefer
			if( nuIsReeferFore != 1 ) response.Slots_NRC.push_back(x);
		}
		
		// Insert slots reefer and not reefer
		if( nuIsReeferFore == 1 ) 
		{
			response.Slots_R.push_back( idxSecondTemp );
		}
		else
		{
			response.Slots_NR.push_back( idxSecondTemp );
		}
		
		// Insert Slot 20' and 40' 
		if(nuCap40 == 1)
		{
			response.Slots_40.push_back( idxFirstTemp );
			response.Slots_40.push_back( idxSecondTemp );
		}
		else
		{
			response.Slots_20.push_back( idxFirstTemp );
			response.Slots_20.push_back( idxSecondTemp );
		}
			
		vector<int> TmpSlotsK;
		vector<int> TmpSlotsKA;
		vector<int> TmpSlotsKF;
		vector<int>::iterator stackFinded = find(response.Stacks.begin(), response.Stacks.end(), nuStackIdCell);		
		if( stackFinded != response.Stacks.end() )
		{
			// stack position 
			int nuPositionStack	:= stackFinded - response.Stacks.begin();
		
			// Insert Stack K
			response.Slots_K[ nuPositionStack ].push_back( idxFirstTemp );
			response.Slots_K[ nuPositionStack ].push_back( idxSecondTemp );
			
			// Insert Stack K Aft 			
			response.Slots_K_A[ nuPositionStack ].push_back( idxFirstTemp );
			
			// Insert Stack K Fore 
			response.Slots_K_F[ nuPositionStack ].push_back( idxSecondTemp );
			
		}
		else
		{
			// Insert Stack
			response.Stacks.push_back( nuStackIdCell );
			
			// Insert Stack K
			TmpSlotsK.push_back( idxFirstTemp );
			TmpSlotsK.push_back( idxSecondTemp );
			response.Slots_K.push_back( TmpSlotsK );			
			
			// Insert Stack K Aft 
			TmpSlotsKA.push_back( idxFirstTemp );
			response.Slots_K_A.push_back( TmpSlotsKA );
			
			// Insert Stack K Fore 
			TmpSlotsKF.push_back( idxSecondTemp );
			response.Slots_K_F.push_back( TmpSlotsKA );
			
		}	
    }
    
    response.SetListCells(listCells);
    
    // Close file
    archivoAr.close();
    
    // return data charged
    return response;               
}

vector<ContainerBox> BLReadFiles::ReadContainer(int pContainers, bool pAreLoaded)
{
     // Variables for container load
    int nuStackIdCont, nuCellIdCont, nuPositionCont, nuLengthCont, nuPortDischargeCont,
        nuIsReeferCont, nuLocationCont;
    double dbWeigthCont, dbHeigthCont;
    vector<ContainerBox> listContainer;
    
    // Read Container Load
    for(int x = 0; x < pContainers; x++)
	{
        archivoAr>>nuStackIdCont>>nuCellIdCont>>nuPositionCont>>dbWeigthCont>>dbHeigthCont>>
                   nuLengthCont>>nuPortDischargeCont>>nuIsReeferCont>>nuLocationCont; 
                   
        cout<<nuStackIdCont<<" "<<nuCellIdCont<<" "<<nuPositionCont<<" "<<dbWeigthCont<<" "<<dbHeigthCont<<" "<<
                   nuLengthCont<<" "<<nuPortDischargeCont<<" "<<nuIsReeferCont<<" "<<nuLocationCont<<endl;
        
		/* Fill Object Container */
        ContainerBox objContainer;
        objContainer.SetStackId(nuStackIdCont);
        objContainer.SetCellId(nuCellIdCont);
        objContainer.SetPosition(nuPositionCont);
        objContainer.SetWeight(dbWeigthCont);
        objContainer.SetHeight(dbHeigthCont);
        objContainer.SetLength(nuLengthCont);
        objContainer.SetPortDischarge(nuPortDischargeCont);
        objContainer.SetIsReefer(nuIsReeferCont);        
        objContainer.SetLocation(nuLocationCont);
        objContainer.SetIsCharged(pAreLoaded);
		
        listContainer.push_back(objContainer);  

		if( pAreLoaded ) 
		
		
    }
    
    return listContainer;     
}


#include "BLReadFiles.h"



// ------------------------ Methods ----------------------------------
BLReadFiles::BLReadFiles()
{
    nuContainerIdx = 0;                             
}

BLReadFiles::~BLReadFiles(){} 

StowageInfo BLReadFiles::ChargeFile(string pFileName)
{    
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
    response.SetListContainerLoaded(ReadContainer(nuContainerLoaded, true));
    
    // re-fill virtuals container 
    for(int x = nuContainerIdx; x < (nuCell * 2); x++)
	{
        response.Cont_V.push_back(x);        
    }
    
    
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
		if( nuIsReeferAft  == objConstants.verdadero ) 
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
		if( nuIsReeferFore == objConstants.verdadero) 
		{
			response.Slots_R.push_back( idxSecondTemp );
		}
		else
		{
			response.Slots_NR.push_back( idxSecondTemp );
		}
		
		// Insert Slot 20' and 40' 
		if(nuCap40 == objConstants.verdadero)
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
		map<int, vector<int> >::iterator stackFinded = response.Slots_K.find(nuStackIdCell);		
		if( stackFinded != response.Slots_K.end() )
		{
			// Insert Stack K
			response.Slots_K[ nuStackIdCell ].push_back( idxFirstTemp );
			response.Slots_K[ nuStackIdCell ].push_back( idxSecondTemp );
			
			// Insert Stack K Aft 			
			response.Slots_K_A[ nuStackIdCell ].push_back( idxFirstTemp );
			
			// Insert Stack K Fore 
			response.Slots_K_F[ nuStackIdCell ].push_back( idxSecondTemp );			
		}
		else
		{
			// Insert Stack
			response.Stacks.push_back( nuStackIdCell );
			
			// Insert Stack K
			TmpSlotsK.push_back( idxFirstTemp );
			TmpSlotsK.push_back( idxSecondTemp );
			response.Slots_K[nuStackIdCell] = TmpSlotsK;			
			
			// Insert Stack K Aft 
			TmpSlotsKA.push_back( idxFirstTemp );
			response.Slots_K_A[nuStackIdCell] = TmpSlotsKA;
			
			// Insert Stack K Fore 
			TmpSlotsKF.push_back( idxSecondTemp );
			response.Slots_K_F[nuStackIdCell] = TmpSlotsKF;
			
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


        // find Container
        map<int, int>::iterator ContainerByPort = response.Cont_EP.find(nuPortDischargeCont);
        if( ContainerByPort != response.Cont_EP.end() )
		{
            response.Cont_EP[nuPortDischargeCont] += 1;     
        }
        else
        {
            response.Cont_EP[nuPortDischargeCont] = 1;
        }
                
        // find Container with equal weight
        map<double, int>::iterator ContainerByWeigth = response.Cont_EW.find(dbWeigthCont);
        if( ContainerByWeigth != response.Cont_EW.end() )
		{
            response.Cont_EW[dbWeigthCont] += 1;     
        }
        else
        {
            response.Cont_EW[dbWeigthCont] = 1;
        }

        // find Container with equal height
        map<double, int>::iterator ContainerByHeigth = response.Cont_EH.find(dbHeigthCont);
        if( ContainerByHeigth != response.Cont_EH.end() )
		{
            response.Cont_EH[dbHeigthCont] += 1;     
        }
        else
        {
            response.Cont_EH[dbHeigthCont] = 1;
        }
                
        // Is High Cube?
        if( objConstants.highCube == dbHeigthCont )
        {
            response.ContCUBE++;    
        }
        else
        {
            response.ContNormal++;    
        }

        // Insert Cont_L and Cont_V
		if( pAreLoaded )
		{
             response.Cont_L.push_back(nuContainerIdx);     
			 // Charge container information
			 ChargeContainerInfo(objContainer);
			 
        }
        else
        {
            response.Cont_V.push_back(nuContainerIdx);
			
			// Charge container information
		    ChargeContainerInfo(objContainer);
        }
        
		nuContainerIdx++; 
		
    }
    
    return listContainer;     
}


void BLReadFiles::ChargeContainerInfo(ContainerBox objContainer)
{
    // Insert Container Weight 
	response.Weight[nuContainerIdx] = objContainer.GetWeight();
	
	// Insert Container POD 
	response.POD[nuContainerIdx] = objContainer.GetPortDischarge();
	
	// Insert Container Length
	response.Length[nuContainerIdx] = objContainer.GetLength();
	
	// Insert Container Height
	response.Height[nuContainerIdx] = objContainer.GetHeight();
	
	// Insert Cont_20_R
	if( objContainer.GetIsReefer() == objConstants.verdadero )
	{
		response.Cont_20_R.push_back(nuContainerIdx);
	}
	else
	{
		response.Cont_NR.push_back(nuContainerIdx);
	} 
     
    // Insert Cont_20 y Cont_40
	if( objContainer.GetLength() == objConstants.container20 )
	{
		response.Cont_20.push_back(nuContainerIdx);
	}
	else
	{	
        if(objContainer.GetIsCharged())
        {
            // Insert Container loaded
		    response.Cont_L.push_back(nuContainerIdx + 1);                             
        }
        else
        {
            // Insert Container virtual
		    response.Cont_V.push_back(nuContainerIdx + 1); 
        }
             
		// Insert 40' Container		
		response.Cont_40.push_back(nuContainerIdx + 1);
	
		// 40' Container Aft and Container Fore	
		response.Cont_40_F.push_back(nuContainerIdx + 1);

		// Insert Container Weight 
        response.Weight[nuContainerIdx + 1] = objContainer.GetWeight();
	
	    // Insert Container POD 
	    response.POD[nuContainerIdx + 1] = objContainer.GetPortDischarge();
	
	    // Insert Container Length
	    response.Length[nuContainerIdx + 1] = objContainer.GetLength();
	
	    // Insert Container Height
     	response.Height[nuContainerIdx + 1] = objContainer.GetHeight();
		
		// Insert Cont_40_R
		if( objContainer.GetIsReefer() == objConstants.verdadero )
		{
			response.Cont_40_R.push_back(nuContainerIdx + 1);
		}
		else
		{
			response.Cont_NR.push_back(nuContainerIdx + 1);
		}
		
		nuContainerIdx++;
	}    
     
}


#include "BLReadFiles.h"

// ------------------------ Methods ----------------------------------
BLReadFiles::BLReadFiles(void)
{
    nuContainerIdx = 0;                             
}

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
        if(x == 0)
        {
			response._nuMaxPOD = nuPortDischarge;
		}
		
		// save maximum POD
		if( response._nuMaxPOD < nuPortDischarge )
		{
			response._nuMaxPOD = nuPortDischarge;
		}
		           
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
 
	response.Cont_20.push_back(nuContainerIdx);
	response.Cont_40.push_back(nuContainerIdx);
 
	// Save virtuals container
	map<int, ContainerBox> virtualCont = ReadContainer(1, false, true);
 
    // Save Container Load
    response.SetListContainerLoad(ReadContainer(nuContainerLoad, false, false));
    
    // Read Tag #CONTAINERS_TOLOADED
    archivoAr>>sbTagDummy;
    cout<<sbTagDummy<<endl;
    
    // Save Container Loaded
    response.SetListContainerLoaded(ReadContainer(nuContainerLoaded, true, false));
    
    
     
    /*for(int x = nuContainerIdx; x < (nuCell * 2); x++)
	{
        response.Cont_V.push_back(x);        
    }*/
    
    
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

		// Fill Object 
        Cell objCell;
        objCell.SetStackId(nuStackIdCell);
        objCell.SetIsReeferFore(nuIsReeferFore);
        objCell.SetIsReeferAft(nuIsReeferAft);
        objCell.SetCapFore(nuCapFore);
        objCell.SetCapAft(nuCapAft);
        objCell.SetCap40(nuCap40);
        objCell.SetNumLocation(nuLocationCell);
         
        listCells.push_back(objCell);
		
		// Fill index set
		int	idxFirstTemp	= ( x * 2 );
		int	idxSecondTemp 	= ( x * 2 ) + 1;
		
		// Insert Slots
		response.Slots.push_back( idxFirstTemp );
		response.Slots.push_back( idxSecondTemp );

		// Insert Cont
		/*response.Cont.push_back( idxFirstTemp );
		response.Cont.push_back( idxSecondTemp );*/
		
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
			if( nuIsReeferFore != objConstants.verdadero ) response.Slots_NRC.push_back(x);
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

map<int, ContainerBox> BLReadFiles::ReadContainer(int pContainers, bool pAreLoaded, bool pIsVirtual)
{
     // Variables for container load
    int nuStackIdCont, nuCellIdCont, nuPositionCont, nuLengthCont, nuPortDischargeCont,
        nuIsReeferCont, nuLocationCont;
    double dbWeigthCont, dbHeigthCont;
    map<int, ContainerBox> listContainer;
    
    
    // Read Container Load
    for(int x = 0; x < pContainers; x++)
	{
		if( pIsVirtual )
		{
			nuStackIdCont = nuCellIdCont = nuPositionCont = dbWeigthCont = 
			dbHeigthCont = nuLengthCont = nuPortDischargeCont = nuIsReeferCont =
			nuLocationCont = 0;
		}
		else
		{
			archivoAr>>nuStackIdCont>>nuCellIdCont>>nuPositionCont>>dbWeigthCont>>dbHeigthCont>>
					nuLengthCont>>nuPortDischargeCont>>nuIsReeferCont>>nuLocationCont; 
		}
                   
        cout<<nuStackIdCont<<" "<<nuCellIdCont<<" "<<nuPositionCont<<" "<<dbWeigthCont<<" "<<dbHeigthCont<<" "<<
              nuLengthCont<<" "<<nuPortDischargeCont<<" "<<nuIsReeferCont<<" "<<nuLocationCont<<endl;
        
		// Fill Object Container 
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
		
		response.Cont.push_back( nuContainerIdx );			
        listContainer[ nuContainerIdx ] = objContainer;
              
        if( objContainer.GetLength() == objConstants.container40 ) 
        {
			listContainer[ nuContainerIdx + 1 ] = objContainer;
			response.Cont.push_back( nuContainerIdx + 1 );
		}

        // find Container
        map<int, int>::iterator ContainerByPort = response.Cont_EP.find(nuPortDischargeCont);
        if( ContainerByPort != response.Cont_EP.end() )
		{
			if( nuLengthCont == 40)	
			{
				response.Cont_EP[nuPortDischargeCont] += 2;
			}
			else
			{
				response.Cont_EP[nuPortDischargeCont] += 1;
			}
        }
        else
        {
			if( nuLengthCont == 40)
			{
				response.Cont_EP[nuPortDischargeCont] = 2;
			}
			else
			{
				response.Cont_EP[nuPortDischargeCont] = 1;
			}
        }
                
        // find Container with equal weight
        map<double, int>::iterator ContainerByWeigth = response.Cont_EW.find(dbWeigthCont);
        if( ContainerByWeigth != response.Cont_EW.end() )
		{
			if( nuLengthCont == 40)
			{
				response.Cont_EW[dbWeigthCont] += 2;
			}
			else
			{
				response.Cont_EW[dbWeigthCont] += 1;     
			}
        }
        else
        {
			
			if( nuLengthCont == 40)	
			{
				response.Cont_EW[dbWeigthCont] = 2;
			}
			else
			{
				response.Cont_EW[dbWeigthCont] = 1;
			}
        }

        // find Container with equal height
        map<double, int>::iterator ContainerByHeigth = response.Cont_EH.find(dbHeigthCont);
        if( ContainerByHeigth != response.Cont_EH.end() )
		{
			if( nuLengthCont == 40)	
			{
				response.Cont_EH[dbHeigthCont] += 2;
			}
			else
			{
				response.Cont_EH[dbHeigthCont] += 1;  
			}
        }
        else
        {
            if( nuLengthCont == 40)
            {
				response.Cont_EH[dbHeigthCont] = 2;
			}
			else
			{
				response.Cont_EH[dbHeigthCont] = 1;
			}
        }
                
        // Is High Cube?
        if( 2.895600 == dbHeigthCont )
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
        }
        /*else
        {
            response.Cont_V.push_back(nuContainerIdx);	
        }*/
        
        // Charge container information
		ChargeContainerInfo(objContainer);
        
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
	 	
    // Insert Cont_20 y Cont_40
	if( objContainer.GetLength() == objConstants.container20 )
	{
		response.Cont_20.push_back(nuContainerIdx);
		
		// Insert Cont_20_R
		if( objContainer.GetIsReefer() == objConstants.verdadero )
		{
			response.Cont_20_R.push_back(nuContainerIdx);
		}
		else
		{
			response.Cont_NR.push_back(nuContainerIdx);
		}
		 
	}
	else if( objContainer.GetLength() == objConstants.container40 )
	{
        if(objContainer.GetIsCharged())
        {
            // Insert Container loaded
		    response.Cont_L.push_back(nuContainerIdx + 1);                             
        }
        /*else
        {
            // Insert Container virtual
		    response.Cont_V.push_back(nuContainerIdx + 1); 
        }*/
             
		// Insert 40' Container
		response.Cont_40.push_back(nuContainerIdx);
		//response.Cont_40.push_back(nuContainerIdx + 1); No se utilizara este valor, no es necesario
	
		// 40' Container Aft and Container Fore	
		response.Cont_40_A.push_back(nuContainerIdx);
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
			response.Cont_40_R.push_back(nuContainerIdx);
			response.Cont_40_R.push_back(nuContainerIdx + 1);
		}
		else
		{
			response.Cont_NR.push_back(nuContainerIdx);
			response.Cont_NR.push_back(nuContainerIdx + 1);
		}
		
		nuContainerIdx++;
	}    
     
}

void BLReadFiles::PrintingData()
{
    // Stack index set
    cout<<"Stack index set"<<endl;
    for(int x = 0; x < response.Stacks.size() ; x++)
        cout<<response.Stacks[x]<<" ";
    
    // Slot index set
    cout<<endl<<"Slot index set"<<endl;
    for(int x = 0; x < response.Slots.size() ; x++)
        cout<<response.Slots[x]<<" ";
     
    // Container index set
    cout<<endl<<"Container index set"<<endl;
    for(int x = 0; x < response.Cont.size() ; x++)
        cout<<response.Cont[x]<<" ";
    
    // Aft slots index set
    cout<<endl<<"Aft slots index set"<<endl;
    for(int x = 0; x < response.Slots_A.size() ; x++)
        cout<<response.Slots_A[x]<<" ";	
    
    // Aft slots index set
    cout<<endl<<"Fore slots index set"<<endl;
    for(int x = 0; x < response.Slots_F.size() ; x++)
        cout<<response.Slots_F[x]<<" ";
    
    // Slots of stack K index set
    cout<<endl<<"Slots of stack K index set"<<endl;
    for (map<int, vector<int> >::iterator it=response.Slots_K.begin(); it != response.Slots_K.end(); ++it)
    {        
        cout<<endl<<"Slots of the stack K: "<<it->first<<endl;
        for(int y = 0; y < (it->second).size() ; y++)
        { 
            cout<<(it->second)[y]<<" ";
        }
    }
    
    // Aft slots of stack K index set
    cout<<endl<<"Aft slots of stack K index set"<<endl;
    for (map<int, vector<int> >::iterator it=response.Slots_K_A.begin(); it != response.Slots_K_A.end(); ++it)
    {        
        cout<<endl<<"Slots of the stack K: "<<it->first<<endl;
        for(int y = 0; y < (it->second).size() ; y++)
        { 
            cout<<(it->second)[y]<<" ";
        }
    }
     	
    // Fore slots of stack K index set
    cout<<endl<<"Fore slots of stack K index set"<<endl;
    for (map<int, vector<int> >::iterator it=response.Slots_K_F.begin(); it != response.Slots_K_F.end(); ++it)
    {        
        cout<<endl<<"Slots of the stack K: "<<it->first<<endl;
        for(int y = 0; y < (it->second).size() ; y++)
        { 
            cout<<(it->second)[y]<<" ";
        }
    } 	
     
    // Reefer slot index set
    cout<<endl<<"Reefer slot index set"<<endl;
    for(int x = 0; x < response.Slots_R.size() ; x++)
        cout<<response.Slots_R[x]<<" "; 	
     
    // Non Reefer slot index set
    cout<<endl<<"Non Reefer slot index set"<<endl;
    for(int x = 0; x < response.Slots_NR.size() ; x++)
        cout<<response.Slots_NR[x]<<" "; 
        
    // Slots in cell with no reefer plugs index set
    cout<<endl<<"Slots in cell with no reefer plugs index set"<<endl;
    for(int x = 0; x < response.Slots_NRC.size() ; x++)
        cout<<response.Slots_NRC[x]<<" ";     
     	
    // 20' capacity slots index set
    cout<<endl<<"20' capacity slots index set"<<endl;
    for(int x = 0; x < response.Slots_20.size() ; x++)
        cout<<response.Slots_20[x]<<" ";  	
     	
    // 40' capacity slots index set
    cout<<endl<<"40' capacity slots index set"<<endl;
    for(int x = 0; x < response.Slots_40.size() ; x++)
        cout<<response.Slots_40[x]<<" "; 
        	
    // Virtual containers index set
    cout<<endl<<"Virtual containers index set"<<endl;
    for(int x = 0; x < response.Cont_V.size() ; x++)
        cout<<response.Cont_V[x]<<" ";     
         	
    // Loaded containers index set
    cout<<endl<<"Loaded containers index set"<<endl;
    for(int x = 0; x < response.Cont_L.size() ; x++)
        cout<<response.Cont_L[x]<<" ";      	
         	
    // 20' containers index set
    cout<<endl<<"20' containers index set"<<endl;
    for(int x = 0; x < response.Cont_20.size() ; x++)
        cout<<response.Cont_20[x]<<" "; 
        
    // 40' containers index set
    cout<<endl<<"40' containers index set"<<endl;
    for(int x = 0; x < response.Cont_40.size() ; x++)
        cout<<response.Cont_40[x]<<" "; 
        
    // 40' containers index set
    cout<<endl<<"40' containers index set (Aft)"<<endl;
    for(int x = 0; x < response.Cont_40_A.size() ; x++)
        cout<<response.Cont_40_A[x]<<" "; 
        
    // 40' containers index set
    cout<<endl<<"40' containers index set (Fore)"<<endl;
    for(int x = 0; x < response.Cont_40_F.size() ; x++)
        cout<<response.Cont_40_F[x]<<" "; 
        
    // 40' reefer containers index set
    cout<<endl<<"40' reefer containers index set"<<endl;
    for(int x = 0; x < response.Cont_40_R.size() ; x++)
        cout<<response.Cont_40_R[x]<<" "; 
        
    // 20' reefer containers index set
    cout<<endl<<"20' reefer containers index set"<<endl;
    for(int x = 0; x < response.Cont_20_R.size() ; x++)
        cout<<response.Cont_20_R[x]<<" "; 
        
    // Non-reefer containers index set
    cout<<endl<<"Non-reefer containers index set"<<endl;
    for(int x = 0; x < response.Cont_NR.size() ; x++)
        cout<<response.Cont_NR[x]<<" ";     
     
    // Weight of container i        	
    cout<<endl<<"Weight of container i"<<endl;
    for (map<int, double>::iterator it=response.Weight.begin(); it != response.Weight.end(); ++it)
    {         	
        cout<<"Container: "<<it->first<<" peso: "<<it->second<<endl;
    }  
            	
	// Ports of discharges of container i        	
    cout<<endl<<"Ports of discharges of container i"<<endl;
    for (map<int, int>::iterator it=response.POD.begin(); it != response.POD.end(); ++it)
    {         	
        cout<<"Container: "<<it->first<<" POD: "<<it->second<<endl;
    }
    
    // Lenght of container i       	
    cout<<endl<<"Lenght of container i "<<endl;
    for (map<int, int>::iterator it=response.Length.begin(); it != response.Length.end(); ++it)
    {         	
        cout<<"Container: "<<it->first<<" Length: "<<it->second<<endl;
    }
    
    // Height of container i       	
    cout<<endl<<"Height of container i "<<endl;
    for (map<int, double>::iterator it=response.Height.begin(); it != response.Height.end(); ++it)
    {         	
        cout<<"Container: "<<it->first<<" Height: "<<it->second<<endl;
    }
    
    // Number of container with discharge port P.     	
    cout<<endl<<"Number of container with discharge port P"<<endl;
    for (map<int, int>::iterator it=response.Cont_EP.begin(); it != response.Cont_EP.end(); ++it)
    {         	
        cout<<"POD: "<<it->first<<" Contador : "<<it->second<<endl;
    }
	
	// Number of container with equal weight      	
    cout<<endl<<"Number of container with equal weight "<<endl;
    for (map<double, int>::iterator it=response.Cont_EW.begin(); it != response.Cont_EW.end(); ++it)
    {         	
        cout<<"Weight: "<<it->first<<" Contador : "<<it->second<<endl;
    }
    
    // Number of container with equal height      	
    cout<<endl<<"Number of container with equal height "<<endl;
    for (map<double, int>::iterator it=response.Cont_EH.begin(); it != response.Cont_EH.end(); ++it)
    {         	
        cout<<"Height: "<<it->first<<" Contador : "<<it->second<<endl;
    }
    
    cout<<"Number of normal containers: "<<response.ContNormal<<endl;
    cout<<"Number of normal cube: "<<response.ContCUBE<<endl;   
     
} 



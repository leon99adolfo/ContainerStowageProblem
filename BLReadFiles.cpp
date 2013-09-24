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
                    
         Cell objCell;
         objCell.SetStackId(nuStackIdCell);
         objCell.SetIsReeferFore(nuIsReeferFore);
         objCell.SetIsReeferAft(nuIsReeferAft);
         objCell.SetCapFore(nuCapFore);
         objCell.SetCapAft(nuCapAft);
         objCell.SetCap40(nuCap40);
         objCell.SetNumLocation(nuLocationCell);
         
         listCells.push_back(objCell);
         
         
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
    }
    
    return listContainer;     
}


#include "StowageCP.h"

StowageCP::StowageCP(StowageInfo pStowageInfo):
              // Domain Variables
              C  (*this, pStowageInfo.Cont.size(), 0, (pStowageInfo.Cont.size() - 1)),
              S  (*this, pStowageInfo.Slots.size(), 0, (pStowageInfo.Slots.size() - 1)),
              L  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxLength),
              H  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxHeight * 10000),
              W  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxWeight * 10000),
              P  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxPOD)/*,
              HS (*this, pStowageInfo.GetNumStacks(), pStowageInfo._nuMinStackHeight, pStowageInfo._nuMaxStackHeight),
              OV (*this, 1, 0, pStowageInfo.Cont.size()),
              OU (*this, 1, 0, pStowageInfo.GetNumStacks()),
              OP (*this, pStowageInfo.GetNumStacks(), 1, pStowageInfo.GetNumPortsDischarge()),
              OR (*this, 1, 0, pStowageInfo.Slots_R.size()),
              O  (*this, 1, 0, ((100 * pStowageInfo.Cont.size()) + (20 * pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks()) +
                                (10 * pStowageInfo.GetNumStacks()) + (5 * pStowageInfo.Slots_R.size()) ),
              CV (*this, pStowageInfo.Cont_V.size(), 0, 1)*/
{

	// Charge Information in global variables
	ChargeInformation(pStowageInfo);
	
	// Channeling constraint
	//channel(*this, C, S);
	
	// Aft40 and Fore40 are stowed in the same cell
	//for(int x = 0; x < pStowageInfo..size() ; x++)

	// elements Length
	for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
		element(*this, Length, S[x], L[x]); 
	
	// elements Height
	for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
		element(*this, Height, S[x], H[x]);
		
	// elements Weight
	for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
		element(*this, Weight, S[x], W[x]);
		
	// elements POD
	for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
		element(*this, POD, S[x], P[x]);	
	
	// Loaded Container
	for(int x = 0; x < pStowageInfo.Cont_L.size() ; x++)
	{	
		ContainerBox objContainer = pStowageInfo.GetListContainerLoaded()[pStowageInfo.Cont_L[x]];
		int cell = objContainer.GetCellId();
		int position = objContainer.GetPosition();
		
		switch (position)
		{
			case -1:
				rel(*this, S[( cell * 2 ) + 1], IRT_EQ, Cont_L[x]);
				break;
			case 0:
				rel(*this, S[( cell * 2 )], IRT_EQ, Cont_L[x]);
				rel(*this, S[( cell * 2 ) + 1], IRT_EQ, Cont_L[x]);
				break;
			case 1:
				rel(*this, S[( cell * 2 )], IRT_EQ, Cont_L[x]);
				break;
		}
	}	


	// regular constraint
	REG r = *REG(20) + *REG(40) + *REG(0);
	DFA d(r);
	// regular constraint Stack-Aft
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K_A.begin(); it != pStowageInfo.Slots_K_A.end(); ++it)
    {
		int size = pStowageInfo.Slots_K_A[(it->first)].size();
		IntVarArray	LTemp( *this, size );
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
			IntVar varTmp( L[slot] );
			LTemp[x] = varTmp;
		}
		extensional(*this, LTemp, d);
    }
	// regular constraint Stack-Fore
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K_F.begin(); it != pStowageInfo.Slots_K_F.end(); ++it)
    {
		int size = pStowageInfo.Slots_K_F[(it->first)].size();
		IntVarArray	LTemp( *this, size );
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
			IntVar varTmp( L[slot] );
			LTemp[x] = varTmp;
		}
		extensional(*this, LTemp, d);
    }
	
	// Slot ¬R	
    for(int x = 0; x < pStowageInfo.Slots_NR.size() ; x++)
	{
		for(int y = 0; y < pStowageInfo.Cont_20_R.size() ; y++)
		{
			rel(*this, S[ pStowageInfo.Slots_NR[x] ], IRT_NQ, C[ pStowageInfo.Cont_20_R[y] ] );
		}
	}
	
	rel(*this, C[ 44 ], IRT_EQ, 20 );
	rel(*this, C[ 45 ], IRT_EQ, 20 );
	
	cout<<"uno "<<pStowageInfo.Slots_NRC.size()<<endl;
	cout<<"dos "<<pStowageInfo.Cont_40_R.size()<<endl;
	cout<<"tre "<<pStowageInfo.Cont_40_R[0]<<endl;
	cout<<"tre "<<pStowageInfo.Cont_40_R[1]<<endl;
	
	// Slot ¬NRC	
    for(int x = 0; x < pStowageInfo.Slots_NRC.size() ; x++)
	{
		for(int y = 0; y < pStowageInfo.Cont_40_R.size() ; y++)
		{
			rel(*this, S[ pStowageInfo.Slots_NRC[x] ], IRT_NQ, C[ pStowageInfo.Cont_40_R[y] ] );
		}
	}
	/*
	// Slot 20
	for(int x = 0; x < pStowageInfo.Slots_20.size() ; x++)
	{
		for(int y = 0; y < pStowageInfo.Cont_20.size() ; x++)
		{
			IntVarArray C20Tmp( pStowageInfo.Cont_20.size() );
			C20Tmp[  ]( C[ pStowageInfo.Cont_20[y] ] )
		
			rel(*this, S[ pStowageInfo.Slots_20[x] ], IRT_EQ,  );
			rel(*this, C[ pStowageInfo.Cont_20[y] ], IRT_EQ, S[ pStowageInfo.Cont_20[x] ] );
		}
	}
	*/
	/*
	// post branching
    branch(*this, C, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, S, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, L, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, H, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, W, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, P, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    */
}

// search support
StowageCP::StowageCP(bool share, StowageCP& s): Space(share, s) 
{
	C.update(*this, share, s.C);
	S.update(*this, share, s.S);
	L.update(*this, share, s.L);
	H.update(*this, share, s.H);
	W.update(*this, share, s.W);
	P.update(*this, share, s.P);
}
  
Space* StowageCP::copy(bool share) 
{
	return new StowageCP(share,*this);
}
  // print solution
void StowageCP::print(void) const 
{
	cout << "Salida" << endl;
	cout <<"C"<< C << endl;
    cout <<"S"<< S << endl;
	cout <<"L"<< L << endl;
	cout <<"H"<< H << endl;
	//cout <<"W"<< W << endl;
	cout <<"P"<< P << endl;
}

void StowageCP::ChargeInformation(StowageInfo pStowageInfo)
{
//---------------------------- sorts the arguments -------------------------- 
    // Stack index set
    /*Stacks = IntArgs( pStowageInfo.Stacks.size() );
    for(int x = 0; x < pStowageInfo.Stacks.size() ; x++)
        Stacks[x] = pStowageInfo.Stacks[x];   
    
    // Slot index set
    Slots = IntArgs( pStowageInfo.Slots.size() );
    for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
        Slots[x] = pStowageInfo.Slots[x];
     
    // Container index set
    Cont = IntArgs( pStowageInfo.Cont.size() );
    for(int x = 0; x < pStowageInfo.Cont.size() ; x++)
        Cont[x] = pStowageInfo.Cont[x];
    
    // Aft slots index set
    Slots_A = IntArgs( pStowageInfo.Slots_A.size() );
    for(int x = 0; x < pStowageInfo.Slots_A.size() ; x++)
        Slots_A[x] = pStowageInfo.Slots_A[x];	
    
    // Aft slots index set
    Slots_F = IntArgs( pStowageInfo.Slots_F.size() );
    for(int x = 0; x < pStowageInfo.Slots_F.size() ; x++)
        Slots_F[x] = pStowageInfo.Slots_F[x];
   
    // Slots of stack K index set
    //Slots_K = IntArgs( pStowageInfo.Slots_K.size() );
    for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K.begin(); it != pStowageInfo.Slots_K.end(); ++it)
    {
        IntArgs slotsKTmp( (it->second).size() );
        for(int y = 0; y < (it->second).size() ; y++)
            slotsKTmp[y] = (it->second)[y];

        Slots_K[(it->first)] = slotsKTmp;
    }
   
    // Aft slots of stack K index set
    for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K_A.begin(); it != pStowageInfo.Slots_K_A.end(); ++it)
    {
        IntArgs SlotsKATmp( (it->second) );
        Slots_K_A[(it->first)] = SlotsKATmp;
    }
    
    // Fore slots of stack K index set
    for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K_F.begin(); it != pStowageInfo.Slots_K_F.end(); ++it)
    {        
        IntArgs SlotsKFTmp( (it->second) );
        Slots_K_F[(it->first)] = SlotsKFTmp;
    } 
    /*
    // Reefer slot index set
    Slots_R = IntArgs( pStowageInfo.Slots_R.size() );
    for(int x = 0; x < pStowageInfo.Slots_R.size() ; x++)
        Slots_R[x] = pStowageInfo.Slots_R[x];
     
    // Non Reefer slot index set
    Slots_NR = IntArgs( pStowageInfo.Slots_NR );
        
    // Slots in cell with no reefer plugs index set
    Slots_NRC = IntArgs( pStowageInfo.Slots_NRC );     
    /* 	
    // 20' capacity slots index set
    Slots_20 = IntArgs( pStowageInfo.Slots_20.size() );
    for(int x = 0; x < pStowageInfo.Slots_20.size() ; x++)
        Slots_20[x] = pStowageInfo.Slots_20[x];  
        
    // 40' capacity slots index set
    Slots_40 = IntArgs( pStowageInfo.Slots_40.size() );
    for(int x = 0; x < pStowageInfo.Slots_40.size() ; x++)
        Slots_40[x] = pStowageInfo.Slots_40[x]; 
        
    // Virtual containers index set
    Cont_V = IntArgs( pStowageInfo.Cont_V.size() );
    for(int x = 0; x < pStowageInfo.Cont_V.size() ; x++)
        Cont_V[x] = pStowageInfo.Cont_V[x];
	*/
    // Loaded containers index set
	Cont_L = IntArgs( pStowageInfo.Cont_L );
    	
    /*	
    // 20' containers index set
    Cont_20 = IntArgs( pStowageInfo.Cont_20.size() );
    for(int x = 0; x < pStowageInfo.Cont_20.size() ; x++)
        Cont_20[x] = pStowageInfo.Cont_20[x]; 
        
    // 40' containers index set
    Cont_40 = IntArgs( pStowageInfo.Cont_40.size() );
    for(int x = 0; x < pStowageInfo.Cont_40.size() ; x++)
        Cont_40[x] = pStowageInfo.Cont_40[x]; 
        
    // 40' containers index set
    Cont_40_A = IntArgs( pStowageInfo.Cont_40_A.size() );
    for(int x = 0; x < pStowageInfo.Cont_40_A.size() ; x++)
        Cont_40_A[x] = pStowageInfo.Cont_40_A[x]; 
        
    // 40' containers index set
    Cont_40_F = IntArgs( pStowageInfo.Cont_40_F.size() );
    for(int x = 0; x < pStowageInfo.Cont_40_F.size() ; x++)
        Cont_40_F[x] = pStowageInfo.Cont_40_F[x]; 
        
    // 40' reefer containers index set
    Cont_40_R = IntArgs( pStowageInfo.Cont_40_R );
        
    // 20' reefer containers index set
    Cont_20_R = IntArgs( pStowageInfo.Cont_20_R );
        
    // Non-reefer containers index set
    Cont_NR = IntArgs( pStowageInfo.Cont_NR );    
	*/
    // Weight of container i 
	Weight = IntArgs( pStowageInfo.Slots.size() );
	for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
	{
		if( pStowageInfo.Weight.find(x) == pStowageInfo.Weight.end() )
		{
			Weight[x] = 0;
		}
		else
		{
			Weight[x] = pStowageInfo.Weight[x] * 10000;
		}
	}
    
	// Ports of discharges of container i
	POD = IntArgs( pStowageInfo.Slots.size() );
	for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
	{
		if( pStowageInfo.POD.find(x) == pStowageInfo.POD.end() )
		{
			POD[x] = 0;
		}
		else
		{
			POD[x] = pStowageInfo.POD[x];
		}
	}
    
	// Lenght of container i
	Length = IntArgs( pStowageInfo.Slots.size() );
	for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
	{
		if( pStowageInfo.Length.find(x) == pStowageInfo.Length.end() )
		{
			Length[x] = 0;
		}
		else
		{
			Length[x] = pStowageInfo.Length[x];
		}
	}
    
    // Height of container i  
	Height = IntArgs( pStowageInfo.Slots.size() );
	for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
	{
		if( pStowageInfo.Height.find(x) == pStowageInfo.Height.end() )
		{
			Height[x] = 0;
		}
		else
		{
			Height[x] = pStowageInfo.Height[x] * 10000;
		}
	}
    
	/*
    // Number of container with discharge port P.
    for (map<int, int>::iterator it=pStowageInfo.Cont_EP.begin(); it != pStowageInfo.Cont_EP.end(); ++it)
    {         	
        IntArgs Cont_EPTmp(1)
		Cont_EPTmp[0] = (it->second);
		Cont_EP[it->first] = Cont_EPTmp;
    }
	
	// Number of container with equal weight
    for (map<double, int>::iterator it=pStowageInfo.Cont_EW.begin(); it != pStowageInfo.Cont_EW.end(); ++it)
    {         	
        IntArgs Cont_EWTmp(1)
		Cont_EWTmp[0] = (it->second);
		Cont_EW[it->first] = Cont_EWTmp;
    }
    
    // Number of container with equal height      	
    for (map<double, int>::iterator it=pStowageInfo.Cont_EH.begin(); it != pStowageInfo.Cont_EH.end(); ++it)
    {         	
        IntArgs Cont_EHTmp(1)
		Cont_EHTmp[0] = (it->second);
		Cont_EH[it->first] = Cont_EHTmp;
    }
    
	ContNormal = IntArgs(1);
	ContNormal[0] = pStowageInfo.ContNormal;
	
	ContCUBE = IntArgs(1);
	ContCUBE[0] = pStowageInfo.ContCUBE;
	*/
}

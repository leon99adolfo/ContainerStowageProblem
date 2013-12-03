#include "StowageCP.h"

StowageCP::StowageCP(StowageInfo pStowageInfo):
              // Domain Variables              
              S  (*this, pStowageInfo.Slots.size(), 0, (pStowageInfo.Slots.size() - 1)),
              L  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxLength),
              H  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxHeight * 10000),
              W  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxWeight),
              P  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxPOD),
              HS (*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo._nuMaxStackHeight * 10000),            
              CV (*this, pStowageInfo.Slots.size(), 0, 1),
              CFEU_A(*this,(pStowageInfo.Slots.size()/2), 0, 1),
              CFEU_F(*this,(pStowageInfo.Slots.size()/2), 0, 1)   /*
              OV (*this, 1, 0, pStowageInfo.Cont.size()),
              OU (*this, 1, 0, pStowageInfo.GetNumStacks()),
              OP (*this, pStowageInfo.GetNumStacks(), 1, pStowageInfo.GetNumPortsDischarge()),
              OR (*this, 1, 0, pStowageInfo.Slots_R.size()),
              O  (*this, 1, 0, ((100 * pStowageInfo.Cont.size()) + (20 * pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks()) +
                                (10 * pStowageInfo.GetNumStacks()) + (5 * pStowageInfo.Slots_R.size()) ),
              */
{

	// Charge Information in global variables
	ChargeInformation(pStowageInfo);
	
	//----------------------------------------- Element Constraints -------------------------------
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
	
	//----------------------------------------- Loaded Container Constraints -------------------------
	// Loaded Container
	for(int x = 0; x < pStowageInfo.Cont_L.size() ; x++)
	{	
		ContainerBox objContainer = pStowageInfo.GetListContainerLoaded()[pStowageInfo.Cont_L[x]];
		int stack = objContainer.GetStackId();
		int cell = objContainer.GetCellId();
		int position = objContainer.GetPosition();
		int cantSlots = 0;
		
		for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K.begin(); it != pStowageInfo.Slots_K.end(); ++it)
		{
			if(stack > (it->first))
			{
				// count slots
				cantSlots += pStowageInfo.Slots_K[(it->first)].size();				
			}
		}
		
		switch (position)
		{
			case -1:
				rel(*this, S[( (cell -1) * 2 ) + 1 + cantSlots], IRT_EQ, Cont_L[x]);
				break;
			case 0:
				rel(*this, S[( (cell -1) * 2 ) + cantSlots], IRT_EQ, Cont_L[x]);
				x++;
				rel(*this, S[( (cell -1) * 2 ) + 1 + cantSlots], IRT_EQ, Cont_L[x]);  
				break;
			case 1:
				rel(*this, S[( (cell -1) * 2 ) + cantSlots], IRT_EQ, Cont_L[x]);
				break;
		}		
	}	
	
	//-------------------------- Regular Constraints and Height Constraints ----------------------------------
	// regular constraint
	REG r = *REG(20) + *REG(40) + *REG(0);
	DFA d(r);
	int countCont = 0;
	// regular constraint Stack-Aft
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K_A.begin(); it != pStowageInfo.Slots_K_A.end(); ++it)
    {
		int size = pStowageInfo.Slots_K_A[(it->first)].size();
		IntVarArray	LTempLength( *this, size );
		IntVarArray	HTempHeight( *this, size );
		IntVarArray	WTempWeight( *this, size, 0, pStowageInfo._nuMaxWeight*2);
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
			
			// Get slots in cell
			IntVarArray slotsCellWeight(*this, 2);
			IntVar varTmpWeight0( W[slot] );
			IntVar varTmpWeight1( W[slot + 1] );
			slotsCellWeight[0] = varTmpWeight0;
			slotsCellWeight[1] = varTmpWeight1;
			// sum slots in cell
			linear(*this, slotsCellWeight, IRT_EQ, WTempWeight[x]);
						
			// Restriction container 40 Aft
			rel(*this, L[slot], IRT_EQ, 40, eqv(CFEU_A[countCont]));
			linear(*this, IntVarArgs()<<S[slot]<<IntVar(*this, 1, 1), IRT_EQ, S[slot+1], imp(CFEU_A[countCont]));
			countCont++;
			
					
			// Get Length
			IntVar varTmpLength( L[slot] );
			LTempLength[x] = varTmpLength;
			// Get Height
			IntVar varTmpHeight( H[slot] );
			HTempHeight[x] = varTmpHeight;
		}
		
		extensional(*this, LTempLength, d);  // regular constraint
		linear(*this, HTempHeight, IRT_LQ, HS[ (it->first) - 1 ]); // Height limit constraint
		rel(*this, WTempWeight, IRT_GQ); // weight ordered constraint
    }
    
    countCont = 0;
	// regular constraint Stack-Fore
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K_F.begin(); it != pStowageInfo.Slots_K_F.end(); ++it)
    {
		int size = pStowageInfo.Slots_K_F[(it->first)].size();
		IntVarArray	LTempLength( *this, size );
		IntVarArray	HTempHeight( *this, size );
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
			
			// Restriction container 40 Fore
			rel(*this, L[slot], IRT_EQ, 40, eqv(CFEU_F[countCont]));
			linear(*this, IntVarArgs()<<S[slot]<<IntVar(*this, -1, -1), IRT_EQ, S[slot-1], imp(CFEU_F[countCont]));
			countCont++;
			
			// Get Length
			IntVar varTmpLength( L[slot] );
			LTempLength[x] = varTmpLength;
			// Get Height
			IntVar varTmpHeight( H[slot] );
			HTempHeight[x] = varTmpHeight;
		}
		extensional(*this, LTempLength, d);  // regular constraint
		linear(*this, HTempHeight, IRT_LQ, HS[ (it->first) - 1 ] ); // Height limit constraint
    }
	
	
	//----------------------------------------- Slots no-reffer Constraints ----------------------------------
	// Slot ¬R	
    for(int x = 0; x < pStowageInfo.Slots_NR.size() ; x++)
	{
		for(int y = 0; y < pStowageInfo.Cont_20_R.size() ; y++)
		{
			rel(*this, S[ pStowageInfo.Slots_NR[x] ], IRT_NQ, pStowageInfo.Cont_20_R[y] );
		}
	}
	
	//----------------------------------------- cell no-reffer Constraints ----------------------------------
	// Slot ¬NRC	
    for(int x = 0; x < pStowageInfo.Slots_NRC.size() ; x++)
	{
		for(int y = 0; y < pStowageInfo.Cont_40_R.size() ; y++)
		{
			int nuCell = pStowageInfo.Slots_NRC[x];			
			rel(*this, S[ (nuCell * 2) ], IRT_NQ, pStowageInfo.Cont_40_R[y] );
			rel(*this, S[ (nuCell * 2) + 1 ], IRT_NQ, pStowageInfo.Cont_40_R[y] );
		}
	}
	
	//----------------------------------------- Slots 20 Constraints ----------------------------------
	// Slot 20
	if(Cont_20.size() > 0)
	{
		IntSet SetCont20( Cont_20 );
		for(int x = 0; x < pStowageInfo.Slots_20.size() ; x++)
		{
			dom(*this, S[ pStowageInfo.Slots_20[x] ], SetCont20);
		}
	}
	
	//----------------------------------------- Slots 40 Constraints ----------------------------------
	// Slot 40
	if(Cont_40.size() > 0)
	{
		IntSet SetCont40( Cont_40 );
		for(int x = 0; x < pStowageInfo.Slots_40.size() ; x++)
		{
			dom(*this, S[ pStowageInfo.Slots_40[x] ], SetCont40);
		}
	}
	
	//----------------------------------------- Weight limit Constraints ----------------------------------
	// weight constraints
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K.begin(); it != pStowageInfo.Slots_K.end(); ++it)
    {
		int size = pStowageInfo.Slots_K[(it->first)].size();
		IntVarArray	WTempWeight( *this, size );
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
									
			// Get Weight
			IntVar varTmpWeight( W[slot] );
			WTempWeight[x] = varTmpWeight;
		}	
		
		double dbMaxWeight = pStowageInfo.GetListStacks()[ ((it->first) - 1 ) ].GetMaxWeigth();		
		linear(*this, WTempWeight, IRT_LQ, dbMaxWeight ); // Weight limit constraint
    }
    
    //----------------------------------------- virtual containers ----------------------------------
    //IntVar ctrue
    for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
    {				
		// Is virtual?
		rel(*this, S[x], IRT_EQ, 0, eqv(CV[x]) );		
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// constraints improve the propagation power
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//----------------------------------------- constraint exactly ----------------------------------

	// Constraint for POD
    for (map<int, int>::iterator it=pStowageInfo.Cont_EP.begin(); it != pStowageInfo.Cont_EP.end(); ++it)
    {
		//cout<<"puerto: "<<it->first<<" cant:"<<it->second<<endl;
		//if( it->first != 0 ) exactly(*this, P, it->first, it->second);		
	}

	// Constraint for Weights
    for (map<double, int>::iterator it=pStowageInfo.Cont_EW.begin(); it != pStowageInfo.Cont_EW.end(); ++it)
    {
		int wTmp = it->first;
		//cout<<"wTmp: "<<wTmp<<" cant: "<<it->second<<endl;
		//if( it->first != 0 ) exactly(*this, W, wTmp, it->second);
	}

	// Constraint for Heights
    for (map<double, int>::iterator it=pStowageInfo.Cont_EH.begin(); it != pStowageInfo.Cont_EH.end(); ++it)
    {
		int hTmp = it->first * 10000 ;
		//cout<<"wTmp: "<<hTmp<<" cant: "<<it->second<<endl;
		//if( it->first != 0 ) exactly(*this, H, hTmp, it->second);
	}

	/*
	// post branching
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
	S.update(*this, share, s.S);
	L.update(*this, share, s.L);
	H.update(*this, share, s.H);
	W.update(*this, share, s.W);
	P.update(*this, share, s.P);
	HS.update(*this, share, s.HS);
	CV.update(*this, share, s.CV);
	CFEU_A.update(*this, share, s.CFEU_A);
	CFEU_F.update(*this, share, s.CFEU_F);
}
  
Space* StowageCP::copy(bool share) 
{
	return new StowageCP(share,*this);
}
  // print solution
void StowageCP::print(void) const 
{
	cout << "Salida" << endl;
    cout <<"S"<< S << endl << endl;
	cout <<"L"<< L << endl << endl;
	cout <<"H"<< H << endl << endl;
	cout <<"W"<< W << endl << endl;
	cout <<"P"<< P << endl << endl;
	cout <<"HS"<< HS << endl << endl;
	cout <<"CV"<< CV << endl << endl;
	cout <<"CFEU_A"<< CFEU_A << endl << endl;
	cout <<"CFEU_F"<< CFEU_F << endl << endl;
	
}

void StowageCP::ChargeInformation(StowageInfo pStowageInfo)
{
//---------------------------- sorts the arguments -------------------------- 

    // Loaded containers index set
	Cont_L = IntArgs( pStowageInfo.Cont_L );
    	
    // 20' containers index set
    Cont_20 = IntArgs( pStowageInfo.Cont_20 );  
        
    // 40' containers index set
    Cont_40 = IntArgs( pStowageInfo.Cont_40	); 
	   
    // 40' reefer containers index set
    Cont_40_R = IntArgs( pStowageInfo.Cont_40_R );
        
    // 20' reefer containers index set
    Cont_20_R = IntArgs( pStowageInfo.Cont_20_R );

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
			Weight[x] = pStowageInfo.Weight[x];
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
}

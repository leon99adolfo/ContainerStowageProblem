#include "StowChannelCP.h"

StowChannelCP::StowChannelCP(StowageInfo pStowageInfo):
              // Domain Variables              
			  C  (*this, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size()), 0, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size())),
              S  (*this, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size()), 0, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size())),
              L  (*this, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size()), 0, pStowageInfo._nuMaxLength),
              H  (*this, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size()), 0, pStowageInfo._nuMaxHeight * 10000),
              W  (*this, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size()), 0, pStowageInfo._nuMaxWeight),
              WD (*this, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size()), 0, pStowageInfo._nuMaxWeight),
              P  (*this, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size()), 0, pStowageInfo._nuMaxPOD),
              HS (*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo._nuMaxStackHeight * 10000),
              NVC (*this, (pStowageInfo.Slots.size() + pStowageInfo.Cont.size()), 0, 1),
              CFEU_A(*this,(pStowageInfo.Slots.size()/2), 0, 1),
              CFEU_F(*this,(pStowageInfo.Slots.size()/2), 0, 1),
              GCD(*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo.GetNumTiers()),
              OGCTD(*this, 0, pStowageInfo.GetNumTiers() * pStowageInfo.GetNumStacks()),
              OV (*this, 0, pStowageInfo.Slots.size()),
			  OVT(*this, pStowageInfo.Slots.size(), 0, 1),
			  OCNS(*this, 0, pStowageInfo.Slots.size()), 
              OU (*this, 0, pStowageInfo.GetNumStacks()),
              OP (*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo.GetNumPortsDischarge()),
              OR (*this, 0, pStowageInfo.Slots_R.size()),
              O  (*this, 0, (1000 * pStowageInfo.Cont.size() -1) + 
							(100 * pStowageInfo.Cont.size() -1) + 
							(20 * pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks()) + 
							(10 * pStowageInfo.GetNumStacks()) +
							(5 * pStowageInfo.Slots_R.size()) ) 
{
	// Charge Information in global variables
	ChargeInformation(pStowageInfo);

	//----------------------------------------- Channel Constraints -------------------------------		
	
	channel(*this, S, C);	
		
	//----------------------------------------- Element Constraints -------------------------------
	int nuSlotsContainer = pStowageInfo.Slots.size() + pStowageInfo.Cont.size();
	// elements Length
	for(int x = 0; x < nuSlotsContainer ; x++)
		element(*this, Length, S[x], L[x]); 				
	
	// elements Height
	for(int x = 0; x < nuSlotsContainer ; x++)
		element(*this, Height, S[x], H[x]);
		
	// elements Weight
	for(int x = 0; x < nuSlotsContainer ; x++)
	{
		element(*this, Weight, S[x], W[x]);
		for(int y = 0; y < Weight.size() ; y++)
		{
			BoolVar IdxWD(*this, 0, 1);
			rel(*this, W[x], IRT_EQ, Weight[y], eqv(IdxWD));
			rel(*this, WD[x], FRT_EQ, Weight[y], imp(IdxWD));
		}
	}	

	// elements POD
	for(int x = 0; x < nuSlotsContainer ; x++)
		element(*this, POD, S[x], P[x]);	

	//----------------------------------------- Loaded Container Constraints -------------------------
	// Loaded Container
	map<int, int> slotByStackFore;
	map<int, int> slotByStackAft;
	
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
		
		int nuPositionF = ( (cell -1) * 2 ) + 1 + cantSlots; 
		int nuPositionA = ( (cell -1) * 2 ) + cantSlots; 

		switch (position)
		{	
			case -1:			
				rel(*this, S[nuPositionF], IRT_EQ, Cont_L[x]);
				SaveContLoadedSlot(pStowageInfo, slotByStackFore, stack, nuPositionF);				
				break;
			case 0:
				rel(*this, S[nuPositionA], IRT_EQ, Cont_L[x]);
				SaveContLoadedSlot(pStowageInfo, slotByStackAft, stack, nuPositionA);
				x++;
				rel(*this, S[nuPositionF], IRT_EQ, Cont_L[x]);
				SaveContLoadedSlot(pStowageInfo, slotByStackFore, stack, nuPositionF);
				break;
			case 1:
				rel(*this, S[nuPositionA], IRT_EQ, Cont_L[x]);
				SaveContLoadedSlot(pStowageInfo, slotByStackAft, stack, nuPositionA);
				break;
		}		
	}
	
	//-------------------------- Regular Constraints and Height Constraints ----------------------------------
	// regular constraint
	REG r = *REG(20) + *REG(40) + *REG(0);
	DFA d(r);	
	int countCont = 0;
	int countStaks = 0;
	FloatVarArray GCY(*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo.GetNumTiers());
	FloatVar GCTD(*this, 0, pStowageInfo.GetNumTiers() * pStowageInfo.GetNumStacks());		
	// regular constraint Stack-Aft
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K_A.begin(); it != pStowageInfo.Slots_K_A.end(); ++it)
    {
		int size = pStowageInfo.Slots_K_A[(it->first)].size();
		IntVarArray	LTempLength( *this, size );
		IntVarArray	HTempHeight( *this, size );
		IntVarArray	WTempWeight( *this, size, 0, pStowageInfo._nuMaxWeight*2);
		FloatVarArgs WeightTotalArgs;
				
		double posY = 0;
		FloatVarArray GraviCentersY( *this, size * 2, 0, pStowageInfo._nuMaxWeight * size);
		
		// Variables POD
		IntVarArgs PTempPODAft;
		IntVarArgs PTempPODFore;
		IntVarArgs PTempPODAftFore;	
		
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
			
			// Get slots in cell
			IntVarArray slotsCellWeight(*this, 2);			
			
			// Aft slot 
			if ( pStowageInfo.ContLoadedSlot.find(slot) != pStowageInfo.ContLoadedSlot.end())	
			{
				slotsCellWeight[0] = IntVar(*this, pStowageInfo._nuMaxWeight, pStowageInfo._nuMaxWeight);				
			}
			else
			{			
				IntVar varTmpWeight0( W[slot] );
				slotsCellWeight[0] = varTmpWeight0;
			}
			
			// Fore slot
			if ( pStowageInfo.ContLoadedSlot.find(slot + 1) != pStowageInfo.ContLoadedSlot.end())	
			{			
				slotsCellWeight[1] = IntVar(*this, pStowageInfo._nuMaxWeight, pStowageInfo._nuMaxWeight);				
			}
			else
			{			
				IntVar varTmpWeight1( W[slot + 1] );
				slotsCellWeight[1] = varTmpWeight1;
			}
			
			// full variable WeightTotal
			WeightTotalArgs<<WD[slot]<<WD[slot + 1];
						
			// sum slots in cell
			linear(*this, slotsCellWeight, IRT_EQ, WTempWeight[x]);
						
			// Restriction container 40 Aft
			rel(*this, L[slot], IRT_EQ, 40, eqv(CFEU_A[countCont]));
			linear(*this, IntVarArgs()<<S[slot]<<IntVar(*this, 1, 1), IRT_EQ, S[slot+1], imp(CFEU_A[countCont]));		
			
			// Only stowed container 40 aft
			for(int z = 0; z < pStowageInfo.Cont_40_F.size() ; z++)
				rel(*this, S[slot], IRT_NQ, pStowageInfo.Cont_40_F[z]);			
			// ---------------------------------------------------------------------------------------
			// This restriction is goal (POD)
			PTempPODAft<<P[slot];
			PTempPODFore<<P[slot+1];
			PTempPODAftFore<<P[slot]<<P[slot+1];
			BoolVar	IsOverStowed20A(*this, 0, 1), 
					IsOverStowed20F(*this, 0, 1), 
					IsOverStowed40(*this, 0, 1);
			// Get minimun value
			IntVar 	minPODAft(*this, 0, pStowageInfo._nuMaxPOD), 
					minPODFore(*this, 0, pStowageInfo._nuMaxPOD),  
					minPODAftFore(*this, 0, pStowageInfo._nuMaxPOD);
			// Restriction min
			min(*this, PTempPODAft, minPODAft);
			min(*this, PTempPODFore, minPODFore);
			min(*this, PTempPODAftFore, minPODAftFore);
			// This restriction is goal (POD)
			rel(*this, P[slot], IRT_GR, minPODAft, eqv(IsOverStowed20A)); 
			rel(*this, P[slot], IRT_GR, minPODFore, eqv(IsOverStowed20F)); 
			rel(*this, P[slot], IRT_GR, minPODAftFore, eqv(IsOverStowed40));			
			// Get solution Bool
			BoolVar OverStow40(*this, 0, 1), OverStow20A(*this, 0, 1), OverStow20F(*this, 0, 1),
					NotOverStow40(*this, 0, 1), NotOverStow20A(*this, 0, 1), NotOverStow20F(*this, 0, 1),
					NegIsOverStowed40(*this, 0, 1), NegCFEU_A(*this, 0, 1), NegIsOverStowed20A(*this, 0, 1),
					NegIsOverStowed20F(*this, 0, 1);
			
			BoolVar GenericBool(*this, 0, 0);			
			rel(*this, NegIsOverStowed40, IRT_EQ, GenericBool, eqv(IsOverStowed40));
			rel(*this, NegCFEU_A, IRT_EQ, GenericBool, eqv(CFEU_A[countCont]));
			rel(*this, NegIsOverStowed20A, IRT_EQ, GenericBool, eqv(IsOverStowed20A));
			rel(*this, NegIsOverStowed20F, IRT_EQ, GenericBool, eqv(IsOverStowed20F));
								
			rel(*this, CFEU_A[countCont], BOT_AND, IsOverStowed40, OverStow40);
			rel(*this, CFEU_A[countCont], BOT_AND, NegIsOverStowed40, NotOverStow40);
			rel(*this, NegCFEU_A, BOT_AND, IsOverStowed20A, OverStow20A);
			rel(*this, NegCFEU_A, BOT_AND, NegIsOverStowed20A, NotOverStow20A);
			rel(*this, NegCFEU_A, BOT_AND, IsOverStowed20F, OverStow20F);
			rel(*this, NegCFEU_A, BOT_AND, NegIsOverStowed20F, NotOverStow20F);
			
			// This restriction is goal (POD) for container 40			
			rel(*this, OVT[countCont*2], IRT_EQ, 1, imp( OverStow40 ));
			rel(*this, OVT[(countCont*2) + 1], IRT_EQ, 1, imp( OverStow40 ));
			rel(*this, OVT[(countCont*2)], IRT_EQ, 0, imp( NotOverStow40 ));
			rel(*this, OVT[(countCont*2) + 1], IRT_EQ, 0, imp( NotOverStow40 ));
			// This restriction is goal (POD) for container 20 aft
			rel(*this, OVT[countCont*2], IRT_EQ, 1, imp( OverStow20A ));
			rel(*this, OVT[countCont*2], IRT_EQ, 0, imp( NotOverStow20A ));
			// This restriction is goal (POD) for container 20 fore
			rel(*this, OVT[(countCont*2) + 1], IRT_EQ, 1, imp( OverStow20F  ));
			rel(*this, OVT[(countCont*2) + 1], IRT_EQ, 0, imp( NotOverStow20F ));
			
			// ---------------------------------------------------------------------------------------				
			// Gravitatory center In Y
			FloatVar GCY40AF(*this, 0, (pStowageInfo._nuMaxWeight/2) * posY);
			rel(*this, GCY40AF == WD[slot] * (posY/2));			
			rel(*this, GraviCentersY[(posY*2)], FRT_EQ, GCY40AF, imp(CFEU_A[countCont]));
			
			FloatVar GCY20A(*this, 0, pStowageInfo._nuMaxWeight * posY);
			rel(*this, GCY20A == WD[slot] * posY);						
			rel(*this, GraviCentersY[(posY*2)], FRT_EQ, GCY20A, imp(NegCFEU_A));
			
			rel(*this, GraviCentersY[(posY*2) + 1], FRT_EQ, GCY40AF, imp(CFEU_A[countCont]));
			
			FloatVar GCY20F(*this, 0, pStowageInfo._nuMaxWeight * posY);
			rel(*this, GCY20F == WD[slot + 1] * posY);	
			rel(*this, GraviCentersY[(posY*2) + 1], FRT_EQ, GCY20F, imp(NegCFEU_A));
			posY++;
				
			// -----------------------------------------------------------------------------------
						
			countCont++;
			// Get Length
			IntVar varTmpLength( L[slot] );
			LTempLength[x] = varTmpLength;
			// Get Height
			IntVar varTmpHeight( H[slot] );
			HTempHeight[x] = varTmpHeight;
		}
			
		double heigthStack =  pStowageInfo.GetListStacks().at(countStaks).GetMaxHeigth()*10000;
		rel(*this, HS[countStaks], IRT_LQ, heigthStack); // Heigth limit	
		
		// ---------------------------------------------------------------------------------------------
		// calculate the all weigth by stack
		FloatVar WeightTotal(*this, 0, pStowageInfo._nuMaxWeight * size * 2);
		linear(*this, WeightTotalArgs, FRT_EQ, WeightTotal);		
		
		// Calculate gravity center in Y
		FloatVar sumGraviCentersY(*this, 0, pStowageInfo._nuMaxWeight * size * size * 2);
		linear(*this, GraviCentersY, FRT_EQ, sumGraviCentersY);
		div(*this, sumGraviCentersY, WeightTotal, GCY[countStaks]);				
				
		// calculate gravity center distance		
		double unitStack = heigthStack / pStowageInfo.GetNumTiers();
		double quarterStack = heigthStack / 4;
		double GCSY = quarterStack / unitStack;
		
		FloatVar PenY(*this, -1 * pStowageInfo.GetNumTiers(), pStowageInfo.GetNumTiers());
		rel(*this, PenY == GCY[countStaks] - GCSY);			
		max(*this, PenY, FloatVar(*this, 0, 0), GCD[countStaks]);	
		
		// ---------------------------------------------------------------------------------------------
			
		extensional(*this, LTempLength, d);  // regular constraint		
		linear(*this, HTempHeight, IRT_LQ, HS[ (it->first) - 1 ]); // Height limit constraint
		rel(*this, WTempWeight, IRT_GQ); // weight ordered constraint		
		// ---------------------------------------------------------------------------------------------
		
		// Goal OP
		IntVar 	stackPODs(*this, 1, pStowageInfo.GetNumPortsDischarge() + 1),
				minPODStack(*this, 0, pStowageInfo._nuMaxPOD);
		BoolVar	existPODnull(*this, 0, 1),
				existPOD(*this, 0, 1);
		nvalues(*this, PTempPODAftFore, IRT_EQ, stackPODs);
		min(*this, PTempPODAftFore, minPODStack);
		rel(*this, minPODStack, IRT_EQ, 0, eqv(existPODnull));
		rel(*this, minPODStack, IRT_NQ, 0, eqv(existPOD));
		linear(*this, IntVarArgs()<<stackPODs<<IntVar(*this, -2, -2), IRT_EQ, OP[countStaks], imp(existPODnull)); // equal and null POD 
		linear(*this, IntVarArgs()<<stackPODs<<IntVar(*this, -1, -1), IRT_EQ, OP[countStaks], imp(existPOD)); // equal POD  				
		countStaks++;
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
			
			// Only stowed container 40 fore
			for(int z = 0; z < pStowageInfo.Cont_40_A.size() ; z++)
				rel(*this, S[slot], IRT_NQ, pStowageInfo.Cont_40_A[z]);
			
			BoolVar contNVCandCFEUF(*this, 0, 1);
			rel(*this, NVC[slot-1], BOT_AND, CFEU_F[countCont], contNVCandCFEUF);
			linear(*this, IntVarArgs()<<S[slot]<<IntVar(*this, -1, -1), IRT_EQ, S[slot-1], imp(contNVCandCFEUF));
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
	BoolVarArray OUT(*this, pStowageInfo.GetNumStacks(), 0, 1);
	// weight constraints
	int idxOUT = 0;
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
		linear(*this, WTempWeight, IRT_GR, 0, eqv(OUT[idxOUT]));
		idxOUT++;
    }
    
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Objective 
	//////////////////////////////////////////////////////////////////////////////////////////////////

	int nuSlotsCont = pStowageInfo.Slots.size() + pStowageInfo.Cont.size();
	// Number of container stowed    
    for(int x = 0; x < nuSlotsContainer; x++)
    {	
		// the container isn't virtual?
		rel(*this, S[x], IRT_LE, pStowageInfo.Cont.size(), eqv(NVC[x]));
	}
	
	BoolVarArray NCS20Temp(*this, pStowageInfo.Slots.size(), 0, 1);
	BoolVarArray NCS40Temp(*this, pStowageInfo.Slots.size(), 0, 1);
	int nuContLarge = pStowageInfo.Cont.size();
	for(int x = nuContLarge; x < nuSlotsCont; x++)
    {
		rel(*this, L[x], IRT_EQ, 20, eqv(NCS20Temp[x-nuContLarge])); 
		rel(*this, L[x], IRT_EQ, 40, eqv(NCS40Temp[x-nuContLarge])); 
	}
	
	IntVar NCS20(*this, 0, pStowageInfo.Slots.size());
	IntVar NCS40(*this, 0, pStowageInfo.Slots.size());
	linear(*this, NCS20Temp, IRT_EQ, NCS20);
	linear(*this, NCS40Temp, IRT_EQ, NCS40);
	rel(*this, OCNS == NCS20 + (NCS40/2));
	
	// Get Over-stowing container
	linear(*this, OVT, IRT_EQ, OV); 
	
	// Get empty stack
	linear(*this, OUT, IRT_EQ, OU);
	
	// Get Different POD
	IntVar OPT(*this, 0, pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks());
	linear(*this, OP, IRT_EQ, OPT);
	
	// Container no-reffer in slots reffer
	BoolVarArray StowedSlotR(*this, pStowageInfo.Slots_R.size(), 0, 1); 
	for(int x = 0; x < pStowageInfo.Slots_R.size() ; x++)
	{
		BoolVarArray isContNRSlotR(*this, pStowageInfo.Cont_NR.size(), 0, 1); 
		for(int y = 0; y < pStowageInfo.Cont_NR.size() ; y++)
		{
			rel(*this, S[ pStowageInfo.Slots_R[x] ], IRT_EQ, pStowageInfo.Cont_NR[y], eqv(isContNRSlotR[y]));
		}
		linear(*this, isContNRSlotR, IRT_NQ, 0, eqv(StowedSlotR[x]));		
	}
		
	// objetive reffer
	linear(*this, StowedSlotR, IRT_EQ, OR);
	
	// Total Distance   
	linear(*this, GCD, FRT_EQ, GCTD);
	
	int countOper = ceil(pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks() / 0.05);
	BoolVarArray countGC(*this, countOper, 0, 1);
	for(int x = 0; x < countOper ; x++)
	{
		rel(*this, GCTD, FRT_GQ, (x * 0.05), eqv(countGC[x]));
	}
	
	linear(*this, countGC, IRT_EQ, OGCTD);
	
	// Cost function
	rel(*this, O == 1000 * OCNS + 100 * OV + 20 * OPT + 10 * OU + 5 * OR + 5 * OGCTD);
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Propagator
	//////////////////////////////////////////////////////////////////////////////////////////////////
	/*IntVar VC(*this, 0, pStowageInfo.Slots.size());
	rel(*this, VC == pStowageInfo.Slots.size() - CS);
	exactly(*this, L, VC, 0);
	exactly(*this, H, VC, 0);
	exactly(*this, P, VC, 0);
	
	IntVar VC40(*this, 0, pStowageInfo.Slots.size());
	rel(*this, VC40 == VC + C40F );
	exactly(*this, W, VC40, 0);
	
	/*for(int x = 0; x < pStowageInfo.POD.size(); x++)
		exactly(*this, P, pStowageInfo.POD,  )*/	
		
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// post branching
	//////////////////////////////////////////////////////////////////////////////////////////////////	

	//branch(*this, S, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, S, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    /*branch(*this, L, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    branch(*this, H, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    branch(*this, W, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    branch(*this, P, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    branch(*this, C, INT_VAR_SIZE_MIN(), INT_VAL_MED());*/
    
}

// search support
StowChannelCP::StowChannelCP(bool share, StowChannelCP& s): IntMinimizeSpace(share, s)
{	
	C.update(*this, share, s.C);
	S.update(*this, share, s.S);
	L.update(*this, share, s.L);
	H.update(*this, share, s.H);
	W.update(*this, share, s.W);
	WD.update(*this, share, s.WD);
	P.update(*this, share, s.P);
	HS.update(*this, share, s.HS);
	CFEU_A.update(*this, share, s.CFEU_A);
	CFEU_F.update(*this, share, s.CFEU_F);
	GCD.update(*this, share, s.GCD);
	OV.update(*this, share, s.OV);
	OVT.update(*this, share, s.OVT);
	NVC.update(*this, share, s.NVC);
	OCNS.update(*this, share, s.OCNS);
	OU.update(*this, share, s.OU);
	OP.update(*this, share, s.OP);
	OR.update(*this, share, s.OR);
	OGCTD.update(*this, share, s.OGCTD);
	O.update(*this, share, s.O);
}
  
// Copy solution  
Space* StowChannelCP::copy(bool share) 
{
	return new StowChannelCP(share,*this);
}
  
// print solution
void StowChannelCP::print(int &pO, int &pOGCTD, int &pOR, string &pOP, int &pOPT, int &pOU, int &pOCNS, int &pOV, string &pS, int nuContainer) const 
{
	cout << "Salida" << endl;
	//cout <<"C"<< C << endl << endl;
    cout <<"S"<< S << endl << endl;
	/*cout <<"L"<< L << endl << endl;
	cout <<"H"<< H << endl << endl;
	cout <<"W"<< W << endl << endl;
	cout <<"WD"<< WD << endl << endl;
	cout <<"P"<< P << endl << endl;
	/*cout <<"HS"<< HS << endl << endl;	
	cout <<"CFEU_A"<< CFEU_A << endl << endl;
	cout <<"CFEU_F"<< CFEU_F << endl << endl;
	cout <<"GCD"<< GCD << endl << endl;
	cout <<"OVT"<< OVT << endl << endl;
	cout <<"OV "<< OV << endl << endl;
	cout <<"NVC"<< NVC << endl << endl;
	cout <<"OCNS "<< OCNS << endl << endl;
	cout <<"OU "<< OU << endl << endl;
	cout <<"OP "<< OP << endl << endl;	
	cout <<"OR "<< OR << endl << endl;
	cout <<"OGCTD "<< OGCTD << endl << endl;
	cout <<"O "<< O << endl << endl;*/
	pO = O.val();
	pOGCTD = OGCTD.val();
	pOR = OR.val();
	
	stringstream ss;
	pOPT = 0;
	for(int x = 0; x < OP.size() ; x++)
	{
		int value = OP[x].val();
		pOPT += value;		
		if(x == 0) 
		{
			ss << value;
		}
		else
		{
			ss << "," << value;
		}
	}	
	pOP += ss.str();
	
	pOU = OU.val();
	pOCNS = OCNS.val();
	pOV = OV.val();

	stringstream ss2;
	stringstream ss3;
	for(int x = 0; x < S.size() ; x++)
	{
		int value = S[x].val();
		if(x == 0) 
		{
			ss2 << value;
			if(nuContainer > value)
			{
				ss3 << value;
			}
			else
			{			
				ss3 << -1;
			}
		}
		else
		{
			ss2 << "," << value;
			if(nuContainer > value)
			{
				ss3 << "," << value;
			}
			else
			{				
				ss3 << "," << -1;
			}
		}
	}
	pS = ss2.str() + "\n\n" + ss3.str();
}

// cost funtion
IntVar StowChannelCP::cost(void) const 
{
    return O;
}

// Save maximum slot by stack
void StowChannelCP::SaveContLoadedSlot(StowageInfo& pStowageInfo, map<int, int>& pSlotByStack, int pStack, int pSlot)
{
	if ( pSlotByStack.find(pStack) != pSlotByStack.end())
	{
		if(pSlotByStack[pStack] < pSlot )
		{
			pStowageInfo.ContLoadedSlot[pSlotByStack[pStack]] = pSlotByStack[pStack];
			pSlotByStack[pStack] = pSlot;
		}					
		else
		{
			pStowageInfo.ContLoadedSlot[pSlot] = pSlot; 
		}
	}
	else
	{
		pSlotByStack[pStack] = pSlot;
	}
}

// charge information
void StowChannelCP::ChargeInformation(StowageInfo pStowageInfo)
{
//---------------------------- sorts the arguments -------------------------- 

	int nuSlotsCont = pStowageInfo.Slots.size() + pStowageInfo.Cont.size();

    // Loaded containers index set
	Cont_L = IntArgs( pStowageInfo.Cont_L );
    	
    // 20' containers index set
    vector<int> ContTemp_20;
    for(int x = 0; x < pStowageInfo.Cont_20.size(); x++)
    {
		ContTemp_20.push_back(pStowageInfo.Cont_20[x]);
	}
	for(int x = pStowageInfo.Cont.size(); x < nuSlotsCont; x++)
    {
		ContTemp_20.push_back(x);
	}
    Cont_20 = IntArgs( ContTemp_20 );  
        
    // 40' containers index set
    vector<int> ContTemp_40;
    for(int x = 0; x < pStowageInfo.Cont_40.size(); x++)
    {
		ContTemp_40.push_back(pStowageInfo.Cont_40[x]);
	}
	for(int x = pStowageInfo.Cont.size(); x < nuSlotsCont; x++)
    {
		ContTemp_40.push_back(x);
	}
	
    Cont_40 = IntArgs( ContTemp_40 );
    
	   
    // 40' reefer containers index set
    Cont_40_R = IntArgs( pStowageInfo.Cont_40_R );
        
    // 20' reefer containers index set
    Cont_20_R = IntArgs( pStowageInfo.Cont_20_R );

    // Weight of container i 
	Weight = IntArgs( nuSlotsCont );
	for(int x = 0; x < nuSlotsCont ; x++)
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
	POD = IntArgs( nuSlotsCont );
	for(int x = 0; x < nuSlotsCont ; x++)
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
	Length = IntArgs( nuSlotsCont );
	for(int x = 0; x < nuSlotsCont ; x++)
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
	Height = IntArgs( nuSlotsCont );
	for(int x = 0; x < nuSlotsCont ; x++)
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


StowChannelCP::~StowChannelCP()
{
	
}
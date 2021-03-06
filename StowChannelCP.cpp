#include "StowChannelCP.h"

StowChannelCP::StowChannelCP(StowageInfo pStowageInfo):
              // Domain Variables              
              RC (*this, pStowageInfo.GetNumVirtualCont(), 0, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont())),
			  C  (*this, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont()), 0, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont())),
              S  (*this, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont()), 0, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont())),
              L  (*this, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont()), 0, pStowageInfo._nuMaxLength),
              H  (*this, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont()), 0, pStowageInfo._nuMaxHeight * 10000),
              W  (*this, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont()), 0, pStowageInfo._nuMaxWeight),
              WD (*this, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont()), 0, pStowageInfo._nuMaxWeight),
              P  (*this, (pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont()), 0, pStowageInfo._nuMaxPOD),
              HS (*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo._nuMaxStackHeight * 10000),
              GCD(*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo.GetNumTiers()),
              OGCTD(*this, 0, pStowageInfo.GetNumTiers() * pStowageInfo.GetNumStacks() * 100),
			  OCNS(*this, 0, pStowageInfo.Slots.size()), 
              OU (*this, 0, pStowageInfo.GetNumStacks()),
              OP (*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo.GetNumPortsDischarge()),
              OR (*this, 0, pStowageInfo.Slots_R.size()),
              O  (*this, 0, (1000 * pStowageInfo.GetNumContainerLoad()) +
							(20 * pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks()) + 
							(10 * pStowageInfo.GetNumStacks()) +
							(5 * pStowageInfo.Slots_R.size()) +
							(pStowageInfo.GetNumTiers() * pStowageInfo.GetNumStacks() * 100) ),
			  OVA (*this, 5, 0, (1000 * pStowageInfo.GetNumContainerLoad()) + 
								(pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks() * 100)),
			  UseStackI(*this, 0, pStowageInfo.GetNumStacks()),
			  UseSlotR(*this, 0, pStowageInfo.GetNumStacks() * pStowageInfo.GetNumTiers() * 2)
{
	// Charge Information in global variables
	ChargeInformation(pStowageInfo);

	//----------------------------------------- Channel Constraints -------------------------------		
	
	channel(*this, S, C);	
		
	//----------------------------------------- Element Constraints -------------------------------
	int nuSlotsContainer = pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont();
	IntVarArray VSC(*this, pStowageInfo.GetNumVirtualCont() , 0, 1);
	IntVarArgs VC;
		
	for(int x = 0; x < nuSlotsContainer ; x++)
	{
		// element contraint
		element(*this, Length, S[x], L[x]);
		element(*this, Height, S[x], H[x]);
		element(*this, Weight, S[x], W[x]);
		element(*this, POD, S[x], P[x]);
		// channel contraint
		channel(*this, WD[x], W[x]);
		
		// no restriction for container stowed
		if( x < pStowageInfo.GetNumVirtualCont() )
		{
			if ( pStowageInfo.Cont_40_F.find(x) != pStowageInfo.Cont_40_F.end())
			{
				rel(*this, VSC[x] == 0);
			}
			else
			{
				element(*this, OverCont, C[x], VSC[x]);
			}
			
			rel(*this, C[x] == RC[x]);
		}
		else
		{
			// mejorar esto (aleoncm)
			bool isContLoaded = false;
			for(int y = 0; y < pStowageInfo.Cont_L.size() ; y++)
			{
				if(x == pStowageInfo.Cont_L[y]) isContLoaded = true;
			}
			if(!isContLoaded) VC<<C[x];
		}
	}

	//--------------------------------------------------------------------------------------------------
	
	IntVarArray NRSR(*this, pStowageInfo.Slots_R.size(), 0, 1);
	// reefer Slots
	int nuConSlotR = 0;
	for (map<int, int>::iterator itSt=pStowageInfo.Slots_R.begin(); itSt != pStowageInfo.Slots_R.end(); ++itSt)
	{
		element(*this, ContNonReefer, S[ (itSt->second) ], NRSR[nuConSlotR]);
		nuConSlotR++;
	}

	//----------------------------------------- Loaded Container Constraints -------------------------
	// Loaded Container
	map<int, int> nuStackUsedR;
	map<int, int> slotByStackFore;
	map<int, int> slotByStackAft;
	map<int, int> nuSlotLoadesdByStack;
	
	for(int x = 0; x < pStowageInfo.Cont_L.size() ; x++)
	{	
		ContainerBox objContainer = pStowageInfo.GetListContainerLoaded()[pStowageInfo.Cont_L[x]];
		int stack = objContainer.GetStackId();
		
		if(nuStackUsedR.find(stack) != nuStackUsedR.end()) nuStackUsedR[stack] = stack;
		
		int nuCellNull = 0;
		for(int y = 0; y < pStowageInfo.CellNull[stack].size() ; y++)
		{
			if(objContainer.GetCellId() > pStowageInfo.CellNull[stack][y] ) nuCellNull++;
		}
		
		int cell = objContainer.GetCellId() - nuCellNull;
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
				SaveContLoadedSlot(pStowageInfo, slotByStackFore, nuSlotLoadesdByStack, stack, nuPositionF);				
				break;
			case 0:
				rel(*this, S[nuPositionA], IRT_EQ, Cont_L[x]);
				SaveContLoadedSlot(pStowageInfo, slotByStackAft, nuSlotLoadesdByStack, stack, nuPositionA);
				x++;
				rel(*this, S[nuPositionF], IRT_EQ, Cont_L[x]);
				SaveContLoadedSlot(pStowageInfo, slotByStackFore, nuSlotLoadesdByStack, stack, nuPositionF);
				break;
			case 1:
				rel(*this, S[nuPositionA], IRT_EQ, Cont_L[x]);
				SaveContLoadedSlot(pStowageInfo, slotByStackAft, nuSlotLoadesdByStack, stack, nuPositionA);
				break;
		}
	}

	//-------------------------- Container 40  ----------------------------------
	for (map<int, int>::iterator it=pStowageInfo.Cont_40_A.begin(); it != pStowageInfo.Cont_40_A.end(); ++it)
		rel(*this, C[(it->second) + 1] == C[(it->second)] + 1);
	
	
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
		IntVarArgs	LTempLength;
		IntVarArgs	HTempHeight;
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
			IntVarArgs slotsCellWeight;
			
			// Aft slot 
			if ( pStowageInfo.ContLoadedSlot.find(slot) != pStowageInfo.ContLoadedSlot.end())	
			{
				slotsCellWeight<<IntVar(*this, pStowageInfo._nuMaxWeight, pStowageInfo._nuMaxWeight);
				PTempPODAft<<IntVar(*this, pStowageInfo._nuMaxPOD, pStowageInfo._nuMaxPOD);
			}
			else
			{							
				slotsCellWeight<<W[slot];
				PTempPODAft<<P[slot];
			}
			
			// Fore slot
			if ( pStowageInfo.ContLoadedSlot.find(slot + 1) != pStowageInfo.ContLoadedSlot.end())	
			{			
				slotsCellWeight<<IntVar(*this, pStowageInfo._nuMaxWeight, pStowageInfo._nuMaxWeight);				
				PTempPODFore<<IntVar(*this, pStowageInfo._nuMaxPOD, pStowageInfo._nuMaxPOD);
			}
			else
			{			
				slotsCellWeight<<W[slot + 1];
				PTempPODFore<<P[slot+1];
			}
			
			PTempPODAftFore<<P[slot]<<P[slot+1];
			
			// full variable WeightTotal
			WeightTotalArgs<<WD[slot]<<WD[slot + 1];
						
			// sum slots in cell
			linear(*this, slotsCellWeight, IRT_EQ, WTempWeight[x]);		
			
			// Only stowed container 40 aft
			for (map<int, int>::iterator it=pStowageInfo.Cont_40_F.begin(); it != pStowageInfo.Cont_40_F.end(); ++it)
				rel(*this, S[slot], IRT_NQ, it->second);
			
			// ---------------------------------------------------------------------------------------				
			// Gravitatory center In Y
			rel(*this, GraviCentersY[(posY*2)] == WD[slot] * posY);
			rel(*this, GraviCentersY[(posY*2) + 1] == WD[slot + 1] * posY);
			posY++;
				
			// -----------------------------------------------------------------------------------
						
			countCont++;
			// Get Length
			LTempLength<<L[slot];
			// Get Height
			HTempHeight<<H[slot];
		}
			
		double heigthStack =  pStowageInfo.GetListStacks()[(countStaks+1)].GetMaxHeigth()*10000;
		rel(*this, HS[countStaks], IRT_LQ, heigthStack); // Heigth limit	
		
		// ---------------------------------------------------------------------------------------------
		// Calculate gravity center in Y
		FloatVar sumGraviCentersY(*this, 0, pStowageInfo._nuMaxWeight * size * size * 2);
		linear(*this, GraviCentersY, FRT_EQ, sumGraviCentersY);
		
		WeightTotalArgs<<FloatVar(*this, 0.001, 0.001);		
		FloatVar WeightTotal(*this, 0, (pStowageInfo._nuMaxWeight * size * 2) + 0.001);
		linear(*this, WeightTotalArgs, FRT_EQ, WeightTotal);
		
		// Calculate gravity center in Y
		div(*this, sumGraviCentersY, WeightTotal, GCY[countStaks]);			
				
		// calculate gravity center distance		
		//double unitStack = heigthStack / pStowageInfo.GetNumTiers();
		double quarterStack = heigthStack / 30000;
		double GCSY = quarterStack;
		
		FloatVar PenY(*this, -1 * pStowageInfo.GetNumTiers(), pStowageInfo.GetNumTiers());
		rel(*this, PenY == GCY[countStaks] - GCSY);			
		max(*this, PenY, FloatVar(*this, 0, 0), GCD[countStaks]);	
		
		// ---------------------------------------------------------------------------------------------
			
		extensional(*this, LTempLength, d);  // regular constraint		
		linear(*this, HTempHeight, IRT_LQ, HS[ (it->first) - 1 ]); // Height limit constraint
		rel(*this, WTempWeight, IRT_GQ); // weight ordered constraint		
		rel(*this, PTempPODAft, IRT_GQ); // POD ordered constraint (Aft)
		rel(*this, PTempPODFore, IRT_GQ); // POD ordered constraint (Fore)
		// ---------------------------------------------------------------------------------------------
		
		// Goal OP
		
		IntVar 	stackPODs(*this, 0, pStowageInfo.GetNumPortsDischarge() + 1);	
		IntVar  nuMinPOD(*this, 0, pStowageInfo._nuMaxPOD);
		nvalues(*this, PTempPODAftFore, IRT_EQ, stackPODs);		
		min(*this, PTempPODAftFore, nuMinPOD);
		BoolVar boIsPODNull = expr(*this, nuMinPOD == 0);
		BoolVar boMoreThanOne = expr(*this, stackPODs > 1);
		BoolVar boAppliesTwo = expr(*this, boIsPODNull && boMoreThanOne);
		IntVar nuPODTwo = expr(*this, stackPODs - 2);
		IntVar nuPODOne = expr(*this, stackPODs - 1);
		ite(*this, boAppliesTwo, nuPODTwo, nuPODOne, OP[countStaks]);
		countStaks++;
    }
    
	// regular constraint Stack-Fore
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K_F.begin(); it != pStowageInfo.Slots_K_F.end(); ++it)
    {
		int size = pStowageInfo.Slots_K_F[(it->first)].size();
		IntVarArgs	LTempLength;
		IntVarArgs	HTempHeight;
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
			
			// Only stowed container 40 fore
			for (map<int, int>::iterator it=pStowageInfo.Cont_40_A.begin(); it != pStowageInfo.Cont_40_A.end(); ++it)
				rel(*this, S[slot], IRT_NQ, it->second);
			
			// Get Length
			LTempLength<<L[slot];
			// Get Height
			HTempHeight<<H[slot];
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
	
	// ---------------------------- Reefer Slot ---------------------------------- 
	BoolVarArgs StowedSlotRArgs;
	for(int y = 0; y < pStowageInfo.Cont_20_R.size() ; y++)
	{
		StowedSlotRArgs<<expr(*this, C[pStowageInfo.Cont_20_R[y]] < pStowageInfo.Slots.size());
	}
	for(int y = 0; y < pStowageInfo.Cont_40_R.size() ; y++)
	{
		StowedSlotRArgs<<expr(*this, C[pStowageInfo.Cont_40_R[y]] < pStowageInfo.Slots.size());
	}
	IntVar StowedSlotR(*this, 0, (pStowageInfo.Cont_20_R.size() + pStowageInfo.Cont_40_R.size()) );
	linear(*this, StowedSlotRArgs, IRT_EQ, StowedSlotR);
	
	//----------------------------------------- Weight limit Constraints ----------------------------------
	IntVarArray OUT(*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo.Slots.size() );
	map<int, int> stackSize;
	int idxOUT = 0;
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K.begin(); it != pStowageInfo.Slots_K.end(); ++it)
    {
		int size = pStowageInfo.Slots_K[(it->first)].size();
		stackSize[(it->first)] = size;
		IntVarArgs	WTempWeight;
		IntVarArgs	LTempLength;
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
									
			// Get Weight
			WTempWeight<<W[slot];
			LTempLength<<L[slot];
		}	
		
		double dbMaxWeight = pStowageInfo.GetListStacks()[(it->first)].GetMaxWeigth();
		linear(*this, WTempWeight, IRT_LQ, dbMaxWeight ); // Weight limit constraint
		count(*this, LTempLength, IntSet(20, 40), IRT_EQ, OUT[idxOUT]);
		idxOUT++;
    }
    
    // Branching by Stack
	vector<int> sortStackSize;
	vector<int> sortStack;
	vector<int> emptyStack;
	int largeSlotLoadedByStack = nuSlotLoadesdByStack.size();
	map<int, int> copySlotLoadesdByStack;
	
	for(int x = 0; x < pStowageInfo.SortSlotRByStack.size(); x++)
	{	
		if(largeSlotLoadedByStack > x)
		{
			int maxStack = -1;
			int maxCountStack = -1;
			for (map<int, int>::iterator it=nuSlotLoadesdByStack.begin(); it != nuSlotLoadesdByStack.end(); ++it)
			{
				if((it->second) >  maxCountStack) 
				{
					maxCountStack = (it->second);
					maxStack = (it->first);				
				}
			}
			sortStack.push_back(maxStack);
			copySlotLoadesdByStack[maxStack] = maxCountStack;
			nuSlotLoadesdByStack.erase(maxStack);
			
		}
	}
	
	for(int x = 0; x < pStowageInfo.SortSlotRByStack.size(); x++)
	{	
		if( copySlotLoadesdByStack.find(pStowageInfo.SortSlotRByStack[x]) == copySlotLoadesdByStack.end()) 
		{
			emptyStack.push_back(pStowageInfo.SortSlotRByStack[x]);
		}
		
		int minSizeStack = INT_MAX;
		int minStack = INT_MAX;
		for (map<int, int>::iterator it=stackSize.begin(); it != stackSize.end(); ++it)
		{
			if((it->second) < minSizeStack) 
			{
				minSizeStack = (it->second);
				minStack = (it->first);				
			}
		}
		sortStackSize.push_back(minStack);
		stackSize.erase(minStack);			
	}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Objective 
	//////////////////////////////////////////////////////////////////////////////////////////////////	
	linear(*this, VSC, IRT_EQ, OCNS);
	
	// Get empty stack
	IntVar nuCeroOU(*this, 0, pStowageInfo.GetNumStacks());
	count(*this, OUT, 0, IRT_EQ, nuCeroOU);	
	
	// Get Different POD
	IntVar OPT(*this, 0, pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks());
	linear(*this, OP, IRT_EQ, OPT);	
		
	// objetive reffer
	IntVar ORTemp(*this, 0, pStowageInfo.Slots_R.size());
	linear(*this, NRSR, IRT_EQ, ORTemp);
	
	// Total Distance   
	linear(*this, GCD, FRT_EQ, GCTD);
	
	int costGCTD = pStowageInfo.GetNumTiers() * pStowageInfo.GetNumStacks() * 100;
	FloatVar GCTDTmp(*this, 0, costGCTD);
	rel(*this, GCTDTmp == GCTD * 100);
	FloatVar GCTD_cast(*this, 0, pStowageInfo.GetNumTiers() * pStowageInfo.GetNumStacks() * 100);
	casting(*this, GCTDTmp, GCTD_cast);
	channel(*this, OGCTD, GCTD_cast);

	// Used stack
	BoolVarArgs UseStackIArgs;
	int amountSlot = 0;
	IntVar CS(*this, 0, pStowageInfo.GetNumVirtualCont() + pStowageInfo.Cont_L.size());
	rel(*this, CS == pStowageInfo.GetNumVirtualCont() + pStowageInfo.Cont_L.size() - OCNS );
	UseStackIArgs<<expr(*this, CS > amountSlot);
	for(int x = 0; x < sortStackSize.size(); x++)
	{
		amountSlot += pStowageInfo.Slots_K[sortStackSize[x]].size(); 
		UseStackIArgs<<expr(*this, CS > amountSlot);
	}
    linear(*this, UseStackIArgs, IRT_EQ, UseStackI);
    rel(*this, OU == pStowageInfo.GetNumStacks() - nuCeroOU - UseStackI);
    
    // used SlotR
    IntVarArgs UseSlotRArgs;
    for (map<int, int>::iterator it=nuStackUsedR.begin(); it != nuStackUsedR.end(); ++it)
	{	
		UseSlotRArgs<<IntVar(*this, pStowageInfo.SlotRByStack[(it->first)], pStowageInfo.SlotRByStack[(it->first)]);
	}
	
	int  contSlotReffer = 1;
	for(int x = 0; x < pStowageInfo.SortSlotRByStack.size(); x++)
	{		
		int idxStack = pStowageInfo.SortSlotRByStack[x];
		bool exists = false;
		if(nuStackUsedR.find(idxStack) != nuStackUsedR.end())
		{
			exists = true;
			break;
		}
		
		if(exists) continue;
						
		BoolVar boUseR = expr(*this, UseStackI >= (contSlotReffer + nuStackUsedR.size()));
		IntVar ContSlotR(*this, 0, pStowageInfo.GetNumTiers()*2);
		int valSlotR = pStowageInfo.SlotRByStack[idxStack];
		ite(*this, boUseR, IntVar(*this, valSlotR, valSlotR), IntVar(*this, 0, 0), ContSlotR);
		UseSlotRArgs<<ContSlotR;
		contSlotReffer++;
	}
	IntVar UseSlotRTmp(*this, 0, pStowageInfo.GetNumStacks() * pStowageInfo.GetNumTiers() * 2);
	linear(*this, UseSlotRArgs, IRT_EQ, UseSlotRTmp);
	max(*this, expr(*this, UseSlotRTmp - StowedSlotR), IntVar(*this, 0, 0), UseSlotR);
	max(*this, expr(*this, ORTemp - UseSlotR), IntVar(*this, 0, 0), OR);
	// --------------------------------------------------------------------------------------
	// --------------------------- Cost function --------------------------------------------
	// --------------------------------------------------------------------------------------	
	rel(*this, O == 1000 * OCNS + 20 * OPT + 10 * OU + 5 * OR + OGCTD);

	// OVA Constraint
	rel(*this, OVA[0] == OCNS);
	rel(*this, OVA[1] == OPT);
	rel(*this, OVA[2] == OU);
	rel(*this, OVA[3] == OR);
	rel(*this, OVA[4] == OGCTD);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Propagator
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	// symmetry breaking with virtual container
	rel(*this, VC, IRT_LE);
	exactly(*this, L, VC.size(), 0);
	exactly(*this, H, VC.size(), 0);
	exactly(*this, P, VC.size(), 0);
	exactly(*this, W, VC.size(), 0);
	
	// symmetry breaking with virtual slots
	IntVarArgs stowVirtualSlots;
	for(int x = pStowageInfo.Slots.size(); x < nuSlotsContainer; x++)
	{
		//cout<<"S["<<x<<"]"<<endl;
		stowVirtualSlots<<S[x];
	}
	rel(*this, stowVirtualSlots, IRT_LE);
	
	for(map<double, int>::iterator it=pStowageInfo.Cont_EW.begin(); it != pStowageInfo.Cont_EW.end(); ++it)
    { 
		if((it->first) != 0) exactly(*this, W, (it->first), (it->second));
    }
    
    for (map<int, int>::iterator it=pStowageInfo.Cont_EP.begin(); it != pStowageInfo.Cont_EP.end(); ++it)
    {
		if((it->first) != 0) exactly(*this, P, (it->first), (it->second));        
    }
    
    for (map<double, int>::iterator it=pStowageInfo.Cont_EH.begin(); it != pStowageInfo.Cont_EH.end(); ++it)
    {        	
        if((it->first) != 0) exactly(*this, H, ((it->first) * 10000), (it->second));        
    }	
    
    bool exist20L = false;
    bool exist40L = false;
    for (map<int, int>::iterator it=pStowageInfo.Cont_EL.begin(); it != pStowageInfo.Cont_EL.end(); ++it)
    {         	
        if((it->first) != 0) exactly(*this, L, (it->first), (it->second));
        if((it->first) == 20) exist20L =  true;
        if((it->first) == 40) exist40L =  true;
    }
    
    if(!exist20L) exactly(*this, L, 20, 0); 
    if(!exist40L) exactly(*this, L, 40, 0);
		
	// symmetry breaking for equal container
    for(int x = 0; x < pStowageInfo.SameContainer.size(); x++)
    {
		IntVarArgs EC;
		for(int y = 0; y < pStowageInfo.SameContainer[x].vecIdxContainer.size(); y++)
		{
			EC<<C[pStowageInfo.SameContainer[x].vecIdxContainer[y]];
		}
		rel(*this, EC, IRT_LE);
	}
	
	// symmetry breaking for equal stack
	for(int x = 0; x < pStowageInfo.SameStack.size(); x++)
    {
		//cout<<endl<<"------------------------------------------------------"<<endl;
		for(int y = 0; y < pStowageInfo.SameStack[x].vecIdxStack.size(); y++)
		{
			if(y == (pStowageInfo.SameStack[x].vecIdxStack.size() - 1)) break;
			
			IntVarArgs ES1;
			IntVarArgs ES2;
			//cout<<"Pri "<<endl;
			vector<int> slots1 = pStowageInfo.Slots_K[pStowageInfo.SameStack[x].vecIdxStack[y]];
			for(int z = 0; z < slots1.size(); z++) 
			{
				//cout<<slots1[z]<<" ";
				ES1<<S[slots1[z]];	
			}
			//cout<<"Seg "<<endl;
			vector<int> slots2 = pStowageInfo.Slots_K[pStowageInfo.SameStack[x].vecIdxStack[y+1]];
			for(int z = 0; z < slots2.size(); z++) 
			{
				//cout<<slots2[z]<<" ";
				ES2<<S[slots2[z]];
			}
			// lexicographical restriction
			//rel(*this, ES1, IRT_LE, ES2);	
			//cout<<endl;		
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Branching
	//////////////////////////////////////////////////////////////////////////////////////////////////		
	if(pStowageInfo.LevelDistribute)
	{
		BranchMethodByLevel(pStowageInfo, pStowageInfo.ActiveRC);
	}
	else
	{
		// branch
		BranchMethodByStack(pStowageInfo, sortStack);
		BranchMethodByStack(pStowageInfo, emptyStack);
	}
    
}

// Branching by stack
void StowChannelCP::BranchMethodByStack(StowageInfo pStowageInfo, vector<int> vectStacks)
{	
	/*IntVarArgs PBranch;
	IntVarArgs WBranch;
	IntVarArgs LBranch;
	IntVarArgs HBranch;
	IntVarArgs SBranch;*/
		
	for(int x = 0; x < vectStacks.size() ; x++)
	{ 
		IntVarArgs PBranch;
		IntVarArgs WBranch;
		IntVarArgs LBranch;
		IntVarArgs HBranch;
		IntVarArgs SBranch;
		//cout<<"pila: "<<vectStacks[x]<<endl;
		vector<int> slots = pStowageInfo.Slots_K[vectStacks[x]];
		for(int y = 0; y < slots.size(); y++)
		{
			int slot = slots[y];
			PBranch<<P[slot];
			WBranch<<W[slot];
			LBranch<<L[slot];
			HBranch<<H[slot];
			SBranch<<S[slot];
			//cout<<"slot: "<<slot<<endl;
		}
		
		for (int x = 0; x  < pStowageInfo.variance.size(); x++)
		{
			switch (pStowageInfo.variance[x])
			{	
				case 0:
					branch(*this, PBranch, INT_VAR_NONE(), INT_VAL_MAX());
					break;
				case 1:
					branch(*this, LBranch, INT_VAR_NONE(), INT_VAL(&trampValueFunL));
					break;
				case 2:
					branch(*this, WBranch, INT_VAR_NONE(), INT_VAL_MAX());
					break;
				case 3:
					branch(*this, HBranch, INT_VAR_NONE(), INT_VAL_MAX());
					break;
			}
		}
		branch(*this, SBranch, INT_VAR_NONE(), INT_VAL_MIN());
	}
	
	// branch
	/*branch(*this, PBranch, INT_VAR_NONE(), INT_VAL_MAX());
	branch(*this, LBranch, INT_VAR_NONE(), INT_VAL(&trampValueFunL));
	branch(*this, WBranch, INT_VAR_NONE(), INT_VAL_MAX());    	
	branch(*this, HBranch, INT_VAR_NONE(), INT_VAL_MAX());
	branch(*this, SBranch, INT_VAR_NONE(), INT_VAL_MIN());*/
	//branch(*this, RC, INT_VAR_NONE(), INT_VAL_MIN());
}

// Branching by level
void StowChannelCP::BranchMethodByLevel(StowageInfo pStowageInfo, bool pActiveRC)
{
	IntVarArgs PBranch;
	IntVarArgs WBranch;
	IntVarArgs LBranch;
	IntVarArgs HBranch;
	IntVarArgs SBranch;
	
	for(int x = 0; x < pStowageInfo.GetNumTiers() ; x++)
	{	
		for(int y = 0; y < pStowageInfo.Slots_K.size() ; y++)
		{
			vector<int> slots = pStowageInfo.Slots_K[(y+1)];
			
			if((x * 2) < slots.size()) 
			{
				int slot1 = slots[(x * 2)];
				int slot2 = slots[(x * 2) + 1];
				PBranch<<P[slot1]<<P[slot2];
				WBranch<<W[slot1]<<W[slot2];
				LBranch<<L[slot1]<<L[slot2];
				HBranch<<H[slot1]<<H[slot2];
				SBranch<<S[slot1]<<S[slot2];
			}		
		}		
	}
	
	for (int x = 0; x  < pStowageInfo.variance.size(); x++)
	{
		switch (pStowageInfo.variance[x])
		{	
			case 0:
				branch(*this, PBranch, INT_VAR_NONE(), INT_VAL_MAX());
				break;
			case 1:
				branch(*this, LBranch, INT_VAR_NONE(), INT_VAL(&trampValueFunL));
				break;
			case 2:
				branch(*this, WBranch, INT_VAR_NONE(), INT_VAL_MAX());
				break;
			case 3:
				branch(*this, HBranch, INT_VAR_NONE(), INT_VAL_MAX());
				break;
		}
	}
	if(pActiveRC)
	{
		branch(*this, RC, INT_VAR_NONE(), INT_VAL_MIN());
	}
	else
	{
		branch(*this, SBranch, INT_VAR_NONE(), INT_VAL_MIN());
	}
}

// search support
StowChannelCP::StowChannelCP(bool share, StowChannelCP& s): IntMinimizeSpace(share, s)
{
	UseStackI.update(*this, share, s.UseStackI);
	UseSlotR.update(*this, share, s.UseSlotR);
	RC.update(*this, share, s.RC);
	C.update(*this, share, s.C);
	S.update(*this, share, s.S);
	L.update(*this, share, s.L);
	H.update(*this, share, s.H);
	W.update(*this, share, s.W);
	WD.update(*this, share, s.WD);
	P.update(*this, share, s.P);
	HS.update(*this, share, s.HS);
	GCD.update(*this, share, s.GCD);
	OCNS.update(*this, share, s.OCNS);
	OU.update(*this, share, s.OU);
	OP.update(*this, share, s.OP);
	OR.update(*this, share, s.OR);
	OGCTD.update(*this, share, s.OGCTD);
	OVA.update(*this, share, s.OVA);
	O.update(*this, share, s.O);
}
  
// Add Constrain
void StowChannelCP::constrain(const Space& _b)
{
	const StowChannelCP& b = static_cast<const StowChannelCP&>(_b);	
	rel(*this, OVA, IRT_LE, b.OVA);
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
	cout <<"RC"<< RC << endl << endl;
	//cout <<"C"<< C << endl << endl;
    cout <<"S "<< S << endl << endl;
	cout <<"L"<< L << endl << endl;
	/*cout <<"H"<< H << endl << endl;
	cout <<"W"<< W << endl << endl;
	cout <<"WD"<< WD << endl << endl;
	cout <<"P"<< P << endl << endl;
	cout <<"HS"<< HS << endl << endl;	
	cout <<"GCD"<< GCD << endl << endl;
	cout <<"OCNS "<< OCNS << endl << endl;
	cout <<"OU "<< OU << endl << endl;
	cout <<"OP "<< OP << endl << endl;	
	cout <<"OR "<< OR << endl << endl;
	cout <<"OGCTD "<< OGCTD << endl << endl;*/	
	cout <<"UseStackI "<< UseStackI << endl << endl;
	cout <<"UseSlotR "<< UseSlotR << endl << endl;
	cout <<"OVA " << OVA << endl << endl;
	cout <<"O "<< O << endl << endl;
	
	
	pO = 0;
	pO = O.val();
	pOGCTD = 0;
	pOGCTD = OGCTD.val();
	pOR = 0;
	pOR = OR.val();
	
	stringstream ss;
	pOPT = 0;
	pOP = "";

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
	
	pOU = 0;
	pOU = OU.val();
	pOCNS = 0;
	pOCNS = OCNS.val();
	pOV = 0;

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
void StowChannelCP::SaveContLoadedSlot(StowageInfo& pStowageInfo, map<int, int>& pSlotByStack,
										map<int, int>& pnuSlotLoadesdByStack, int pStack, int pSlot)
{
	if ( pSlotByStack.find(pStack) != pSlotByStack.end())
	{
		if(pSlotByStack[pStack] < pSlot )
		{
			pSlotByStack[pStack] = pSlot;
		}
	}
	else
	{
		pSlotByStack[pStack] = pSlot;
	}
	pStowageInfo.ContLoadedSlot[pSlot] = pSlot;
	
	pnuSlotLoadesdByStack[pStack] = (pnuSlotLoadesdByStack.find(pStack) != pnuSlotLoadesdByStack.end()) ? pnuSlotLoadesdByStack[pStack] + 1 : 1;
}

// charge information
void StowChannelCP::ChargeInformation(StowageInfo pStowageInfo)
{
//---------------------------- sorts the arguments -------------------------- 

	int nuSlotsCont = pStowageInfo.Slots.size() + pStowageInfo.GetNumVirtualCont();

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

	// OverStowage container
	ContNonReefer = IntArgs( nuSlotsCont );	
	for(int x = 0; x < nuSlotsCont ; x++)
	{
		if( pStowageInfo.Cont_NR.find(x) == pStowageInfo.Cont_NR.end() )
		{
			ContNonReefer[x] = 0;
		}
		else
		{
			bool IsLoaded = false;
			for(int y = 0; y < pStowageInfo.Cont_L.size(); y++)
			{
				if(pStowageInfo.Cont_L[y] == x)
				{
					IsLoaded = true;
					break;
				}
			}
			ContNonReefer[x] = IsLoaded ? 0: 1;
		}
	}

	// OverStowage container
	OverCont = IntArgs( nuSlotsCont );	
	for(int x = 0; x < nuSlotsCont ; x++)
	{
		if( x < pStowageInfo.Slots.size() )
		{
			OverCont[x] = 0;
		}
		else
		{
			OverCont[x] = 1;
		}
	}

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
			POD[x] = pStowageInfo._nuMinPOD;
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

// ------------------- Propagator Casting----------------------------
void StowChannelCP::casting(StowChannelCP& home, FloatVar x0, FloatVar x1) {
  if (home.failed()) return;
  GECODE_ES_FAIL(Casting::post(home,x0,x1));
}

// -------------- Branching L ---------------------
int StowChannelCP::trampValueFunL(const Space& home, IntVar x, int i)
{
	if(x.in(20)) return 20;
	if(x.in(40)) return 40;
	
	return 0;
}

// ------------------ Dristroyer ------------------------- 
StowChannelCP::~StowChannelCP()
{
	
}



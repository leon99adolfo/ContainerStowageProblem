#include "StowageCP.h"

StowageCP::StowageCP(StowageInfo pStowageInfo):
              // Domain Variables    
              S  (*this, pStowageInfo.Slots.size(), 0, (pStowageInfo.Cont.size() - 1)), // Es -1 por que el contenedor cero ya lo cuenta
              L  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxLength),
              H  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxHeight * 10000),
              W  (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxWeight),
              WD (*this, pStowageInfo.Slots.size(), 0, pStowageInfo._nuMaxWeight),
              P  (*this, pStowageInfo.Slots.size(), pStowageInfo._nuMinPOD, pStowageInfo._nuMaxPOD),
              HS (*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo._nuMaxStackHeight * 10000),
              NVC (*this, pStowageInfo.Slots.size(), 0, 1),
              CFEU_A(*this,(pStowageInfo.Slots.size()/2), 0, 1),
              CFEU_F(*this,(pStowageInfo.Slots.size()/2), 0, 1),
              GCD(*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo.GetNumTiers()),
              OGCTD(*this, 0, pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks() * 100), 
              OV (*this, 0, pStowageInfo.Slots.size()),
			  OVT(*this, pStowageInfo.Slots.size(), 0, 1),
			  OCNS(*this, 0, pStowageInfo.Slots.size()), 
              OU (*this, 0, pStowageInfo.GetNumStacks()),
              OP (*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo.GetNumPortsDischarge()),
              OR (*this, 0, pStowageInfo.Slots_R.size()),
              O  (*this, 0, (1000 * pStowageInfo.GetNumContainerLoad()) + 
							(100 * pStowageInfo.Cont.size() -1) + 
							(20 * pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks()) + 
							(10 * pStowageInfo.GetNumStacks()) +
							(5 * pStowageInfo.Slots_R.size()) +
							(pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks() * 100) )
{
	
	// Charge Information in global variables
	ChargeInformation(pStowageInfo);

	//----------------------------------------- Element Constraints -------------------------------
	for(int x = 0; x < pStowageInfo.Slots.size() ; x++)
	{
		// the container isn't virtual?
		rel(*this, S[x], IRT_NQ, 0, eqv(NVC[x]));				
		for(int y = 0; y < pStowageInfo.Slots.size() ; y++)
		{
			if(x != y) rel(*this, S[x], IRT_NQ, S[y], imp(NVC[x]));
		}
		
		// element constraint
		element(*this, Length, S[x], L[x]);
		element(*this, Height, S[x], H[x]);
		element(*this, Weight, S[x], W[x]);
		element(*this, POD, S[x], P[x]);
		
		// channel constraint
		channel(*this, WD[x], W[x]);
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
	map<int, int> slotByStackFore;
	map<int, int> slotByStackAft;
	
	for(int x = 0; x < pStowageInfo.Cont_L.size(); x++)
	{	
		ContainerBox objContainer = pStowageInfo.GetListContainerLoaded()[pStowageInfo.Cont_L[x]];
		int stack = objContainer.GetStackId();
		
		int nuCellNull = 0;
		for(int y = 0; y < pStowageInfo.CellNull[stack].size() ; y++)
		{
			if(objContainer.GetCellId() <= pStowageInfo.CellNull[stack][y] ) nuCellNull++;
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
			}
			else
			{							
				slotsCellWeight<<W[slot];
			}
			
			// Fore slot
			if ( pStowageInfo.ContLoadedSlot.find(slot + 1) != pStowageInfo.ContLoadedSlot.end())	
			{			
				slotsCellWeight<<IntVar(*this, pStowageInfo._nuMaxWeight, pStowageInfo._nuMaxWeight);				
			}
			else
			{			
				slotsCellWeight<<W[slot + 1];
			}
			
			// full variable WeightTotal
			WeightTotalArgs<<WD[slot]<<WD[slot + 1];
						
			// sum slots in cell
			linear(*this, slotsCellWeight, IRT_EQ, WTempWeight[x]);		
			
			// Restriction container 40 Aft
			rel(*this, L[slot], IRT_EQ, 40, eqv(CFEU_A[countCont]));
			linear(*this, IntVarArgs()<<S[slot]<<IntVar(*this, 1, 1), IRT_EQ, S[slot+1], imp(CFEU_A[countCont]));
			
			// Only stowed container 40 aft
			for (map<int, int>::iterator it=pStowageInfo.Cont_40_F.begin(); it != pStowageInfo.Cont_40_F.end(); ++it)
				rel(*this, S[slot], IRT_NQ, it->second);
							
			// ---------------------------------------------------------------------------------------
			// This restriction is goal (POD)
			PTempPODAft<<P[slot];
			PTempPODFore<<P[slot+1];
			PTempPODAftFore<<P[slot]<<P[slot+1];
			BoolVar	IsOverStowed20A(*this, 0, 1), 
					IsOverStowed20F(*this, 0, 1),
					IsOverStowed40(*this, 0, 1);
			// Get minimun value
			IntVar 	minPODAft(*this, pStowageInfo._nuMinPOD, pStowageInfo._nuMaxPOD), 
					minPODFore(*this, pStowageInfo._nuMinPOD, pStowageInfo._nuMaxPOD),  
					minPODAftFore(*this, pStowageInfo._nuMinPOD, pStowageInfo._nuMaxPOD);
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
			
		double heigthStack =  pStowageInfo.GetListStacks().at(countStaks).GetMaxHeigth()*10000;
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
		IntVar 	stackPODs(*this, 0, pStowageInfo.GetNumPortsDischarge());		
		nvalues(*this, PTempPODAftFore, IRT_EQ, stackPODs);		
		linear(*this, IntVarArgs()<<stackPODs<<IntVar(*this, -1, -1), IRT_EQ, OP[countStaks]);
		countStaks++;
    }
    
    countCont = 0;
	// regular constraint Stack-Fore
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K_F.begin(); it != pStowageInfo.Slots_K_F.end(); ++it)
    {
		int size = pStowageInfo.Slots_K_F[(it->first)].size();
		IntVarArgs LTempLength;
		IntVarArgs	HTempHeight;
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
			// Restriction container 40 Fore
			rel(*this, L[slot], IRT_EQ, 40, eqv(CFEU_F[countCont]));
			linear(*this, IntVarArgs()<<S[slot]<<IntVar(*this, -1, -1), IRT_EQ, S[slot-1], imp(CFEU_F[countCont]));
			countCont++;
			
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
			//cout<<"pStowageInfo.Slots_40[]: "<<x<<" "<<pStowageInfo.Slots_40[x]<<endl;
			dom(*this, S[ pStowageInfo.Slots_40[x] ], SetCont40);			
		}
	}
	
	//----------------------------------------- Weight limit Constraints ----------------------------------
	IntVarArray OUT(*this, pStowageInfo.GetNumStacks(), 0, pStowageInfo.Slots.size());
	// weight constraints
	int idxOUT = 0;
	for (map<int, vector<int> >::iterator it=pStowageInfo.Slots_K.begin(); it != pStowageInfo.Slots_K.end(); ++it)
    {
		int size = pStowageInfo.Slots_K[(it->first)].size();
		IntVarArgs WTempWeight;
		IntVarArgs LTempLength;
		for(int x = 0; x < size; x++)
		{
			int slot = (it->second)[x];
									
			// Get Weight
			WTempWeight<<W[slot];
			LTempLength<<L[slot];
		}	
		
		double dbMaxWeight = pStowageInfo.GetListStacks()[ ((it->first) - 1 ) ].GetMaxWeigth();		
		linear(*this, WTempWeight, IRT_LQ, dbMaxWeight ); // Weight limit constraint
		count(*this, LTempLength, IntSet(20, 40), IRT_EQ, OUT[idxOUT]);
		idxOUT++;
    }
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Objective 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	IntVar CS(*this, 0, pStowageInfo.Cont.size());	// containers stowed
	linear(*this, NVC, IRT_EQ, CS);	
	IntVar C40F(*this, 0, (pStowageInfo.Cont.size()/2));	// containers 40 stowed in fore 
	linear(*this, CFEU_F, IRT_EQ, C40F);
	int numberContainer = pStowageInfo.GetNumContainerLoad() + pStowageInfo.GetNumContainerLoaded();
	rel(*this, OCNS == numberContainer - (CS - C40F)); // (-1) for container virtual
	
	// Get Over-stowing container
	linear(*this, OVT, IRT_EQ, OV); 
	
	// Get empty stack
	IntVar nuCeroOU(*this, 0, pStowageInfo.GetNumStacks());
	count(*this, OUT, 0, IRT_EQ, nuCeroOU);
	rel(*this, OU == pStowageInfo.GetNumStacks() - nuCeroOU);
	
	// Get Different POD
	IntVar OPT(*this, 0, pStowageInfo.GetNumPortsDischarge() * pStowageInfo.GetNumStacks());
	linear(*this, OP, IRT_EQ, OPT);
	
	// objetive reffer
	linear(*this, NRSR, IRT_EQ, OR);
	
	// Total Distance
	linear(*this, GCD, FRT_EQ, GCTD);
		
	int costGCTD = pStowageInfo.GetNumTiers() * pStowageInfo.GetNumStacks() * 100;
	FloatVar GCTDTmp(*this, 0, costGCTD);
	rel(*this, GCTDTmp == GCTD * 100);
	//channel(*this, OGCTD, GCTDTmp);
	
	BoolVarArgs GCTDArray(*this, costGCTD, 0, 1);
	for(int x = 0; x < costGCTD; x++) rel(*this, GCTDTmp, FRT_GR, x + 1, eqv(GCTDArray[x]));
	linear(*this, GCTDArray, IRT_EQ, OGCTD);

	// --------------------------------------------------------------------------------------
	// --------------------------- Cost function --------------------------------------------
	// --------------------------------------------------------------------------------------
	rel(*this, O == 1000 * OCNS + 100 * OV + 20 * OPT + 10 * OU + 5 * OR + OGCTD);	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// Propagator
	//////////////////////////////////////////////////////////////////////////////////////////////////
	IntVar VC(*this, 0, pStowageInfo.Slots.size());
	rel(*this, VC == pStowageInfo.Slots.size() - CS);
	exactly(*this, L, VC, 0);
	exactly(*this, H, VC, 0);
	exactly(*this, P, VC, 0);
	
	IntVar VC40(*this, 0, pStowageInfo.Slots.size());
	rel(*this, VC40 == VC + C40F );
	exactly(*this, W, VC40, 0);
		
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// post branching
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//branch(*this, S, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    branch(*this, S, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    /*branch(*this, L, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    branch(*this, H, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    branch(*this, W, INT_VAR_SIZE_MIN(), INT_VAL_MAX());
    branch(*this, P, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    branch(*this, S, INT_VAR_SIZE_MIN(), INT_VAL_MED());
    */
}

// search support
StowageCP::StowageCP(bool share, StowageCP& s): IntMinimizeSpace(share, s)
{	
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
Space* StowageCP::copy(bool share) 
{
	return new StowageCP(share,*this);
}
  
// print solution
void StowageCP::print(int &pO, int &pOGCTD, int &pOR, string &pOP, int &pOPT, int &pOU, int &pOCNS, int &pOV, string &pS) const 
{
	//cout << "Salida" << endl;	
    cout <<"S"<< S << endl << endl;
	cout <<"L"<< L << endl << endl;
	cout <<"H"<< H << endl << endl;
	cout <<"W"<< W << endl << endl;
	cout <<"WD"<< WD << endl << endl;
	cout <<"P"<< P << endl << endl;
	cout <<"HS"<< HS << endl << endl;	
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
	pOV = OV.val();

	stringstream ss2;
	for(int x = 0; x < S.size() ; x++)
	{
		int value = S[x].val();
		if(x == 0) 
		{
			ss2 << value;
		}
		else
		{
			ss2 << "," << value;
		}
	}
	pS = ss2.str();
}

// cost funtion
IntVar StowageCP::cost(void) const 
{
    return O;
}

// Save maximum slot by stack
void StowageCP::SaveContLoadedSlot(StowageInfo& pStowageInfo, map<int, int>& pSlotByStack, int pStack, int pSlot)
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

	// OverStowage container
	ContNonReefer = IntArgs( pStowageInfo.Cont.size() );	
	for(int x = 0; x < pStowageInfo.Cont.size() ; x++)
	{
		if( pStowageInfo.Cont_NR.find(x) == pStowageInfo.Cont_NR.end() )
		{
			ContNonReefer[x] = 0;
		}
		else
		{
			ContNonReefer[x] = 1;
		}
	}
	
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


StowageCP::~StowageCP()
{
	
}

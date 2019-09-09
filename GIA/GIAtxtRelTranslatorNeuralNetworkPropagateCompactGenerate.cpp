/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerate.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j2h 10-August-2019
 * Requirements: 
 * Description: Textual Relation Translator Neural Network Propagate Compact - unsupervised training of sequence grammar parse network
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerate.hpp"
#include "SHAREDvars.hpp"


#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR

//static int newNeuronIndex;

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
static vector<GIAtxtRelTranslatorRulesGroupParseTree*>* parseTreeGroupListPointer;
#endif


/*
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::formTxtRelTranslatorNeuralNetworkSequenceGrammarInput(GIAtranslatorVariablesClass* translatorVariables)
{

}
*/


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, const bool createNewConnections, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	GIAtxtRelTranslatorRulesGroupParseTree* topLevelParseTreeGroup = NULL; 
	bool parseIsolatedSubreferenceSets = false;
	bool parserEnabled = false;
	int performance = 0;
	return executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, &topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performance, createNewConnections, firstLayer);
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree** topLevelParseTreeGroup, const bool parseIsolatedSubreferenceSets, const bool parserEnabled, int* performance, const bool createNewConnections, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	bool result = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
	parseTreeGroupListPointer = GIAtxtRelTranslatorNeuralNetworkPropagateCompact.getParseTreeGroupList();
	#endif

	GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData forwardPropogationSentenceData;
	forwardPropogationSentenceData.sentenceContents = sentenceContents;

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
	cout << "\n\n\n\n\nGIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork 1" << endl;
	#endif
	
	bool toplevelGroupActivationFound = GIAtxtRelTranslatorNeuralNetworkPropagateCompact.executeTxtRelTranslatorNeuralNetworkPart1(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance, createNewConnections, firstLayer);
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
	cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork 2" << endl;
	#endif
	
	if(!toplevelGroupActivationFound)
	{
		if(createNewConnections)
		{
			findAndReconcileVariation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, &forwardPropogationSentenceData, firstLayer);
		}
	}
	
	GIAtxtRelTranslatorNeuralNetworkPropagateCompact.executeTxtRelTranslatorNeuralNetworkPart2(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, &forwardPropogationSentenceData, topLevelParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, performance, createNewConnections, firstLayer);
		
	return toplevelGroupActivationFound;
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::findAndReconcileVariation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_TEST_VERIFICATION_AFTER_GENERATION
	if(translatorVariables->currentPreprocessorSentenceInList->sentenceIndexOriginal <= translatorVariables->maxNumberSentences/2)
	{
	#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_OPTIONAL_AND_MULTIPLE_INPUTS
		bool foundAndReconciledMissingOrDifferentCentralNeuron = findAndReconcileCentralVariation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer);
		if(!foundAndReconciledMissingOrDifferentCentralNeuron)
		{			
		#endif

			bool foundAndReconciledMissingOrDifferentIncrementalNeurons = findAndReconcileIncrementalVariation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer);

			/*
			OLD algorithm1 (relies on biologically unfeasible backprop);
			for every neuron hx in network connected to first input neuron
				verify that it corresponds to the first input component
					check next input component in hx
						verify that it encapsulates the second input neuron in sequence (recursive)
				3 before breaking the neuron apart check to see if there exists a neuron with the desired input components
					there won't be 
				this algorithm requires back propagation; require an algorithm that performs forward prop only
			*/	

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_OPTIONAL_AND_MULTIPLE_INPUTS
		}
		#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_TEST_VERIFICATION_AFTER_GENERATION
	}
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
	cout << "\n\n\n\n\nGIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork 3" << endl;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_VERIFICATION
	if(!GIAtxtRelTranslatorNeuralNetworkPropagateCompact.performPropagationTest(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer))
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::findAndReconcileVariation fail: !performPropagation verification" << endl;
		exit(EXIT_ERROR);
		result = false;
	}	
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
	cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::executeTxtRelTranslatorNeuralNetwork 4" << endl;
	#endif

	return result;
}

	

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::findAndReconcileIncrementalVariation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
	cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::findAndReconcileIncrementalVariation" << endl;
	#endif
		
	bool foundAndReconciledMissingOrDifferentIncrementalNeurons = false;
	
	/*
	algorithm2;
	let l be the first word in the sentence word sequence
	for every input neuron in sentence word sequence starting at l*
		propagate its input through the network
			ensure word index is contiguous during forward prop (GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ENFORCE_WORD_CONNECTIVITY_BETWEEN_PREVIOUS_ACTIVE_COMPONENTS_AND_NEWLY_ACTIVATED_COMPONENT: GIAtxtRelTranslatorNeuralNetworkPropagateOperations.componentWordConnectivityTestsWrapper)
			mark all hidden neurons that fire (these are candidates)
			mark all hidden neurons that are only partially activated
				these become candidates for splitting
				never split a neuron below 2 components
			take the hidden neuron that support the most words in the sentence from l (whether it is fully activated or partially activated)
				if necessary (it is partially activated) split the neuron;
					b) take its components and forge a new neuron
					add to vector q
				c) if it is fully activated add to vector q
				then repeat* from last word index (l)
		if no appropriate hidden neuron is found create a a) new hidden neuron containing the first input in the current part of the sentence word sequence*
			add to vector q
			OLD: how to determine whether to use this new neuron or just connect its first input to..
			then repeat* from last word index (l)
	combine all the a) new, b) splitted, or c) fully activated neurons into a d) new neuron on a higher layer
		this new higher layer neuron d) represents a successfully parsed sentence m
			OPTIONAL: add this neuron to the highest level full gramatical sentence neuron
	issue with this algorithm is that it cannot reuse groups for embeded optional components eg rcmod;
		1 the dog barks
		2 the dog that is near the house barks
		will create 2 independent top level groups (the dog barks group will not be reused by 2)
		possibly look for and catch recursive group connectivity and ensure that groups can only support varied word index ranges when they refer to recursive (optional) groups
	*/
	
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);
	//GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstLayerNeuronGroupStart = NULL;
	
	int indexInSequence = 0;
	bool stillIdentifyingHighLevelNeurons = true;
	bool toplevelGroupActivationFound = false;

	vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*> listOfHighLevelNeurons1;	//vectorQ
	bool creatingNewNeuronSequence1 = false;	//this is used to ensure that only a single neuron is created to store the unrecognised sequence (not 1 neuron per word of the sequence)
	int neuronSequenceIndex1 = 0;
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup1 = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2_ADD_ALL_PERMUTATIONS
	vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*> listOfHighLevelNeurons2;	//vectorQ
	bool creatingNewNeuronSequence2 = false;
	int neuronSequenceIndex2 = 0;
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup2 = NULL;
	#endif	
	
	int loopIndex = 0;
	
	while(stillIdentifyingHighLevelNeurons)
	{		
		int currentFirstInputNeuronIndexInSequence;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			currentFirstInputNeuronIndexInSequence = firstLayer->size()-1-indexInSequence;
		}
		else
		{
			currentFirstInputNeuronIndexInSequence = indexInSequence;
		}

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_SPLIT
		cout << "\e[35m \n** loopIndex = " << loopIndex << ", indexInSequence = " << indexInSequence << ", word = " << (forwardPropogationSentenceData->sentenceContents)->at(currentFirstInputNeuronIndexInSequence)->tagName << " \e[0m" << endl;
		#endif

		GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentLayerNeuronGroupStart = (*firstLayer)[currentFirstInputNeuronIndexInSequence];
	
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
		cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::stillIdentifyingHighLevelNeurons" << endl;
		#endif
		
		forwardPropogationSentenceData->toplevelGroupActivationFound = false;

		forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = true;
		forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage = NULL;
		forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage = NULL;
		if(GIAtxtRelTranslatorNeuralNetworkPropagateCompact.performPropagation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer, false, indexInSequence))
		{
			toplevelGroupActivationFound = true;	//note this will only be set true if the entire sentence word indices have been covered (ie only when currentFirstInputNeuronIndexInSequence=start)
		}
		
		if(!GIAtxtRelTranslatorNeuralNetworkPropagateCompact.verifyActivatedNeuronsAtLeastOne(forwardPropogationSentenceData, firstLayer, &(forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage), &(forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage), newNeuronSequenceGroup1))
		{
			//case a
			foundAndReconciledMissingOrDifferentIncrementalNeurons = true;
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_SPLIT
			cout << "A createOrAppendFirstLevelHiddenLayerGroup" << endl;
			#endif
			createOrAppendFirstLevelHiddenLayerGroup(GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, currentLayerNeuronGroupStart, &creatingNewNeuronSequence1, &neuronSequenceIndex1, &newNeuronSequenceGroup1, &listOfHighLevelNeurons1);
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2_ADD_ALL_PERMUTATIONS
			if(indexInSequence > 0)
			{
				createOrAppendFirstLevelHiddenLayerGroup(GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, currentLayerNeuronGroupStart, &creatingNewNeuronSequence2, &neuronSequenceIndex2, &newNeuronSequenceGroup2, &listOfHighLevelNeurons2);
			}
			#endif
			
			indexInSequence++;
		}
		else
		{
			creatingNewNeuronSequence1 = false;
			neuronSequenceIndex1 = 0;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2_ADD_ALL_PERMUTATIONS
			creatingNewNeuronSequence2 = false;
			neuronSequenceIndex2 = 0;
			#endif
			foundAndReconciledMissingOrDifferentIncrementalNeurons = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
			if(forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage != NULL)
			#else
			if(!GIAtxtRelTranslatorNeuralNetworkPropagateCompact.verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage, currentFirstInputNeuronIndexInSequence, true))
			#endif
			{	
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_SPLIT
				cout << "B listOfHighLevelNeurons1.push_back(forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage->groupRef);" << endl;
				cout << "forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex = " << forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage->groupRef->groupIndex << endl;
				#endif
				//fully activated group coverage+weight is > partially activated group coverage+weight
				//case c
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					indexInSequence = firstLayer->size()-1 - (forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex - 1);	//CHECKTHIS
				}
				else
				{
					indexInSequence = forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex + 1;
				}
				listOfHighLevelNeurons1.push_back(forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage->groupRef);
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2_ADD_ALL_PERMUTATIONS
				if(indexInSequence > 0)
				{
					listOfHighLevelNeurons2.push_back(forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage->groupRef);
				}
				#endif
			}
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
			else
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_SPLIT
				cout << "C splitGroupAtLastActivatedComponent" << endl;
				#endif
				//partially activated group coverage+weight is > fully activated group coverage+weight
				//case b
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					indexInSequence = firstLayer->size()-1 - (forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex - 1);	//CHECKTHIS
				}
				else
				{
					indexInSequence = forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex + 1;
				}
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
				int indexToSplitVector1 = INT_DEFAULT_VALUE;
				int indexToSplitVector2 = INT_DEFAULT_VALUE;			
				GIAtxtRelTranslatorNeuralNetworkPropagateCompact.identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSentenceData, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage, &indexToSplitVector1, &indexToSplitVector2);
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
				cout << "indexToSplitVector1 = " << indexToSplitVector1 << endl;
				cout << "indexToSplitVector2 = " << indexToSplitVector2 << endl;				
				#endif				
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron = splitGroupAtLastActivatedComponentUnordered(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef, indexToSplitVector1, indexToSplitVector2);	//note passing partiallyActivatedNeuronWithMaxWordIndexCoverage instead of partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef because groupRef's activations may have been overwritten if the neuron was reset after being saved to partiallyActivatedNeuronWithMaxWordIndexCoverage
				#else
				int indexToSplitVector = INT_DEFAULT_VALUE;
				GIAtxtRelTranslatorNeuralNetworkPropagateCompact.identifyComponentIndexLastActivatedIndex(forwardPropogationSentenceData, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage, &indexToSplitVector);
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
				cout << "indexToSplitVector = " << indexToSplitVector << endl;				
				#endif
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron = splitGroupAtLastActivatedComponent(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef, indexToSplitVector);	//note passing partiallyActivatedNeuronWithMaxWordIndexCoverage instead of partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef because groupRef's activations may have been overwritten if the neuron was reset after being saved to partiallyActivatedNeuronWithMaxWordIndexCoverage
				#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
				cout << "forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex = " << forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex << endl;
				cout << "forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex = " << forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex << endl;
				cout << "indexInSequence = " << indexInSequence << endl;
				#endif				
				listOfHighLevelNeurons1.push_back(newNeuron);
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2_ADD_ALL_PERMUTATIONS
				if(indexInSequence > 0)
				{
					listOfHighLevelNeurons2.push_back(newNeuron);
				}
				#endif
			}
			#endif
		}

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deinitialiseParseTreeGroupList(GIAtxtRelTranslatorRulesGroupTypes, parseTreeGroupListPointer);
		#endif

		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
		if(toplevelGroupActivationFound)
		{
			stillIdentifyingHighLevelNeurons = false;
		}
		else
		{
		#endif
			if(indexInSequence < firstLayer->size())
			{
				stillIdentifyingHighLevelNeurons = true;	
				//see how many words successfully propagated through the next time
			}
			else
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
				//start odd 2-tuples
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* grammaticalSentenceNeuron = createNewHiddenLayerGroup(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes);
				for(int i=0; i<listOfHighLevelNeurons1.size()-1; i+2)
				{
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* grammaticalSentenceNeuron = createNewHiddenLayerGroup(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes);
					addComponentToGroup(forwardPropogationSentenceData, listOfHighLevelNeurons1[i], grammaticalSentenceNeuron, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);
					addComponentToGroup(forwardPropogationSentenceData, listOfHighLevelNeurons1[i+1], grammaticalSentenceNeuron, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);

				}
				listOfHighLevelNeurons1.clear();
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2_ADD_ALL_PERMUTATIONS
				//start even 2-tuples
				for(int i=0; i<listOfHighLevelNeurons2.size()-1; i+2)
				{
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* grammaticalSentenceNeuron = createNewHiddenLayerGroup(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes);
					addComponentToGroup(forwardPropogationSentenceData, listOfHighLevelNeurons2[i], grammaticalSentenceNeuron, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);
					addComponentToGroup(forwardPropogationSentenceData, listOfHighLevelNeurons2[i+1], grammaticalSentenceNeuron, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);
				}
				listOfHighLevelNeurons2.clear();
				#endif
				indexInSequence = 0;
				#else
				stillIdentifyingHighLevelNeurons = false;
				#endif
			}	
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
		}
		#endif
		
		loopIndex++;
	}

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PREVENT_TOP_LEVEL_NEURON_DUPLICATION
	if(listOfHighLevelNeurons1.size() > 1)
	{
	#endif
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
		cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::findAndReconcileIncrementalVariation - creating top level sentence neuron;" << endl;
		#endif
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* grammaticalSentenceNeuron = createNewHiddenLayerGroup(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN_SEGREGATE_TOP_LAYER_NEURONS
		grammaticalSentenceNeuron->topLevelSentenceNeuron = true;
		#endif
		for(int k=0; k<listOfHighLevelNeurons1.size(); k++)
		{
			bool normalAddition = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
			if(listOfHighLevelNeurons1[k]->firstHiddenLayerNeuron)
			{
				if(listOfHighLevelNeurons1[k]->components.size() == 1)
				{
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* singleComponentNeuron = listOfHighLevelNeurons1[k];
					
					//cout << "singleComponentNeuron->groupIndex = " << singleComponentNeuron->groupIndex << endl;
					
					//direct wire low level POS neuron to grammaticalSentenceNeuron (dont ever create intermediary single component neuron)
					normalAddition = false;
					
					GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = singleComponentNeuron->components[0];
					
					//doesnt seem to ever be required;
					for(int i=0; i<singleComponentNeuron->ANNfrontComponentConnectionList.size(); i++)
					{
						GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (singleComponentNeuron->ANNfrontComponentConnectionList)[i];
						GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelGroup = currentComponent->ownerGroup;
						//cout << "higherLevelGroup->groupIndex = " << higherLevelGroup->groupIndex << endl;
						
						/*
						//method1;
						for(int j=0; j<higherLevelGroup->components.size(); j++)
						{	
							GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent2 = (higherLevelGroup->components)[j];
							if(currentComponent2 == currentComponent)
							{
								higherLevelGroup->components.erase(higherLevelGroup->components.begin()+j);
								j--;
							}
						}
						GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(higherLevelGroup, &(higherLevelGroup->components), false, NULL);	
						delete currentComponent;
						*/
						//method2;
						for(int j=0; j<currentComponent->ANNbackGroupConnectionList.size(); j++)
						{
							GIAtxtRelTranslatorRulesGroupNeuralNetwork* firstHiddenLayerGroup = (currentComponent->ANNbackGroupConnectionList)[j];
							if(firstHiddenLayerGroup == singleComponentNeuron)
							{
								currentComponent->ANNbackGroupConnectionList.erase(currentComponent->ANNbackGroupConnectionList.begin()+j);
								j--;
							}
						}
					}
					
					for(int i=0; i<component->ANNbackGroupConnectionList.size(); i++)
					{
						GIAtxtRelTranslatorRulesGroupNeuralNetwork* lowerLevelPOSgroup = (component->ANNbackGroupConnectionList)[i];
						//GIAtxtRelTranslatorNeuralNetworkFormation.deleteGroupANNconnectionForward(lowerLevelPOSgroup, component);
						for(int j=0; j<lowerLevelPOSgroup->ANNfrontComponentConnectionList.size(); j++)
						{
							GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (lowerLevelPOSgroup->ANNfrontComponentConnectionList)[j];
							if(currentComponent == component)
							{
								lowerLevelPOSgroup->ANNfrontComponentConnectionList.erase(lowerLevelPOSgroup->ANNfrontComponentConnectionList.begin()+j);
								j--;
							}
						}
						
						addComponentToGroup(forwardPropogationSentenceData, lowerLevelPOSgroup, grammaticalSentenceNeuron, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);
					}
					
	
					/*
					cout << "\t GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION;" << endl;
					cout << "\tgrammaticalSentenceNeuron->groupIndex = " << grammaticalSentenceNeuron->groupIndex << endl;
					cout << "\tlowerLevelPOSgroup->groupIndex = " << lowerLevelPOSgroup->groupIndex << endl;
					cout << "\tlistOfHighLevelNeurons1[i]->groupIndex = " << singleComponentNeuron->groupIndex << endl;
					*/

					GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRules.getSequenceGrammarGroupTypeDefault(GIAtxtRelTranslatorRulesGroupTypes);
					//groupType->groups.erase(remove(groupType->groups.begin(), groupType->groups.end(), singleComponentNeuron), groupType->groups.end());
					for(int i2=0; i2<groupType->groups.size(); i2++)
					{
						GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i2];
						if(group == singleComponentNeuron)
						{
							groupType->groups.erase(groupType->groups.begin()+i2);
        						i2--;
						}
					}

					/*
					//parseTreeGroup has already been removed by GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deinitialiseParseTreeGroupList;
					if(singleComponentNeuron->currentParseTreeGroupTemp != NULL)
					{
						GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteGroup(singleComponentNeuron->currentParseTreeGroupTemp);
						parseTreeGroupListPointer->erase(remove(parseTreeGroupListPointer->begin(), parseTreeGroupListPointer->end(), singleComponentNeuron->currentParseTreeGroupTemp), parseTreeGroupListPointer->end());
					}
					*/

					GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteGroup(singleComponentNeuron);	
				}
			}
			if(normalAddition)
			{
				listOfHighLevelNeurons1[k]->firstHiddenLayerNeuron = false;	//NOT REQUIRED? prevents parsing of future sentence from matching this neuron as partially/fullyActivatedNeuronWithMaxWordIndexCoverage, and then treating it like firstHiddenLayerNeuron above
			#endif
			
				addComponentToGroup(forwardPropogationSentenceData, listOfHighLevelNeurons1[k], grammaticalSentenceNeuron, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
			}
			#endif
		}
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(grammaticalSentenceNeuron, &(grammaticalSentenceNeuron->components), false, NULL);
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.traceBackpropNeuralNetwork(grammaticalSentenceNeuron, 0);
		#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PREVENT_TOP_LEVEL_NEURON_DUPLICATION
	}
	else
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
		if(listOfHighLevelNeurons1.size() == 1)
		{
			listOfHighLevelNeurons1[0]->firstHiddenLayerNeuron = false;
		}
		#endif
	}
	#endif
	
	return foundAndReconciledMissingOrDifferentIncrementalNeurons;
}


	
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::createOrAppendFirstLevelHiddenLayerGroup(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* currentLayerNeuronGroupStart, bool* creatingNewNeuronSequence, int* neuronSequenceIndex, GIAtxtRelTranslatorRulesGroupNeuralNetwork** newNeuronSequenceGroup, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* listOfHighLevelNeurons)
{
	bool result = true;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
	cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::createOrAppendFirstLevelHiddenLayerGroup" << endl;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS
	if(*neuronSequenceIndex >= GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_MAX_NUMBER_COMPONENTS)
	{
		*creatingNewNeuronSequence = false;
		*neuronSequenceIndex = 0;
	}
	#endif

	if(*creatingNewNeuronSequence)
	{
		addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSentenceData, currentLayerNeuronGroupStart, *newNeuronSequenceGroup);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
		cout << "creatingNewNeuronSequence: newNeuronSequenceGroup = " << (*newNeuronSequenceGroup)->groupIndex << endl;
		#endif
	}
	else
	{
		*neuronSequenceIndex = 0;
		*creatingNewNeuronSequence = true;
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron = createNewHiddenLayerGroup(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_REQUIRE_NUM_COMPONENTS_ENFORCE_DURING_FIRST_HIDDEN_LAYER_GENERATION
		newNeuron->firstHiddenLayerNeuron = true;
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_SPLIT
		cout << "!creatingNewNeuronSequence: newNeuronSequenceGroup = " << newNeuron->groupIndex << endl;
		//cout << "currentLayerNeuronGroupStart->wordDataTemp->wordPOStype = " << currentLayerNeuronGroupStart->wordDataTemp->wordPOStype << endl;
		#endif
		addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSentenceData, currentLayerNeuronGroupStart, newNeuron);
		listOfHighLevelNeurons->push_back(newNeuron);
		*newNeuronSequenceGroup = newNeuron;
	}

	*neuronSequenceIndex = *neuronSequenceIndex + 1;
	
	return result;
}		
			
			
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_COMPONENT_SUPPORT_OPTIONAL_AND_MULTIPLE_INPUTS

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::findAndReconcileCentralVariation(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer)
{
	bool foundAndReconciledMissingOrDifferentCentralNeuron = false;
	/*algorithm3;
	issue with this algorithm is that it cannot reuse groups for embeded optional components eg rcmod;
		1 the dog barks
		2 the dog that is near the house barks
		will create 2 independent top level groups (the dog barks group will not be reused by 2)
		note if executing bidirectionally, then need to update GIAtxtRelTranslatorNeuralNetworkPropagateCompact to store bool reverseDirection as a flag
		SOLUTION? to redundancy prob; go in both directions and find group that is incomplete by 1 missing or 1 diff component 
	*/
	
	//bool normalDirection = false;	//forwards
	//normalDirection, &neuronWithMaxWordIndexCoverage1isPartiallyActivated, &activatedNeuronWithMaxWordIndexCoverage1, &partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex, &partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, &partiallyActivatedNeuronWithMaxWordIndexCoverage1numUnactivatedComponents, 
	//OLD DEL &partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex;

	
	//propagate in one direction and identify _?
	bool parseSentenceReverse1 = true;	//forward
	bool neuronWithMaxWordIndexCoverage1isPartiallyActivated = false;
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* activatedNeuronWithMaxWordIndexCoverage1 = NULL;
	int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex;
	int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex;
	int partiallyActivatedNeuronWithMaxWordIndexCoverage1numUnactivatedComponents;
	if(performPropagationDirection(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer, parseSentenceReverse1, &neuronWithMaxWordIndexCoverage1isPartiallyActivated, &activatedNeuronWithMaxWordIndexCoverage1, &partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex, &partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, &partiallyActivatedNeuronWithMaxWordIndexCoverage1numUnactivatedComponents))
	{

	}

	//now propagate in opposite direction
	bool parseSentenceReverse2 = false;	//reverse
	bool neuronWithMaxWordIndexCoverage2isPartiallyActivated = false;
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* activatedNeuronWithMaxWordIndexCoverage2 = NULL;
	int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastActivatedIndex;
	int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex;
	int partiallyActivatedNeuronWithMaxWordIndexCoverage2numUnactivatedComponents;
	if(performPropagationDirection(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer, parseSentenceReverse2, &neuronWithMaxWordIndexCoverage2isPartiallyActivated, &activatedNeuronWithMaxWordIndexCoverage2, &partiallyActivatedNeuronWithMaxWordIndexCoverage2lastActivatedIndex, &partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, &partiallyActivatedNeuronWithMaxWordIndexCoverage2numUnactivatedComponents))
	{

	}

	
	bool optionalComponentFound = false;
	if(partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex+1 == partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex)
	{
		optionalComponentFound = true;	//case: MISSING, else DIFFERENT
	}
	bool singleMissingOrDifferentComponent = false;
	if(partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex+1 == partiallyActivatedNeuronWithMaxWordIndexCoverage2lastActivatedIndex)
	{
		singleMissingOrDifferentComponent = true;	//case: MISSING, else DIFFERENT
	}
	
	//CHECKTHIS case: neuronWithMaxWordIndexCoverage1isFullyActivated/neuronWithMaxWordIndexCoverage1isFullyActivated
	
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
	if(neuronWithMaxWordIndexCoverage1isPartiallyActivated && neuronWithMaxWordIndexCoverage2isPartiallyActivated && (activatedNeuronWithMaxWordIndexCoverage1 == activatedNeuronWithMaxWordIndexCoverage2))
	{
		//case 1: found-DIFFERENTorMISSING-found
		if(singleMissingOrDifferentComponent)
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* differentOrMissingComponent = activatedNeuronWithMaxWordIndexCoverage1->components[partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex+1];	//DIFFERENTorMISSINGcomponent
			if(optionalComponentFound)
			{
				foundAndReconciledMissingOrDifferentCentralNeuron = true;
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_OPTIONAL_COMPONENTS
				differentOrMissingComponent->optional = true;
				#else
				//create a new version of the neuron without the optional component
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* newHiddenLayerNeuron = createNewGroup();	
				GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRules.getSequenceGrammarGroupTypeDefault(GIAtxtRelTranslatorRulesGroupTypes);	
				groupType->groups.push_back(newHiddenLayerNeuron);
				
				GIAtxtRelTranslatorRules.copyComponents(&(activatedNeuronWithMaxWordIndexCoverage1->components), &(newHiddenLayerNeuron->components));
				for(int i=0; i<newHiddenLayerNeuron->components.size(); i++)
				{
					GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = newHiddenLayerNeuron->components[i];
					if(i != differentOrMissingComponent->componentIndex)
					{
						for(int i2=0; i2<component->ANNbackGroupConnectionList.size(); i2++)
						{
							GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = component->ANNbackGroupConnectionList[i2];
							group->ANNfrontComponentConnectionList.push_back(component);
						}
					}
				}
				/*
				newHiddenLayerNeuron->components = activatedNeuronWithMaxWordIndexCoverage1->components;
				newHiddenLayerNeuron->components.erase(newHiddenLayerNeuron->components.begin()+differentOrMissingComponent->componentIndex, newHiddenLayerNeuron->components.begin()+differentOrMissingComponent->componentIndex+1); //remove missing component
				*/
				GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(newHiddenLayerNeuron, &(newHiddenLayerNeuron->components), false, NULL);
				#endif
			}
			else
			{
				//perform another propagation from start of DIFFERENTorMISSING
				bool direction = true;
				if(findAndConnectDifferentOrMissingSection(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, firstLayer, direction, differentOrMissingComponent))
				{
					foundAndReconciledMissingOrDifferentCentralNeuron = true;
				}
			}
		}
		else
		{
			//FUTURE: add support for multiple diff/missing components?
		}
	}
	else 
	#endif
	if(neuronWithMaxWordIndexCoverage1isPartiallyActivated && neuronWithMaxWordIndexCoverage2isPartiallyActivated)	//REDUNDANT with singleMissingOrDifferentComponent: && partiallyActivatedNeuronWithMaxWordIndexCoverage1numUnactivatedComponents==1 && partiallyActivatedNeuronWithMaxWordIndexCoverage2numUnactivatedComponents==1
	{
		//case 1: found-DIFFERENTorMISSING-found
		if(singleMissingOrDifferentComponent)
		{			
			if(findGroupDirectlyAbove(activatedNeuronWithMaxWordIndexCoverage1, activatedNeuronWithMaxWordIndexCoverage2))
			{
				//case 3:                               2 DIFFERENTorMISSING-found
		        	//          1 found-DIFFERENTorMISSING /
				bool direction = true;
				if(splitGroupsConnectedByOptionalOrDifferentComponent(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage1, activatedNeuronWithMaxWordIndexCoverage2, partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage2lastActivatedIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, firstLayer, direction, optionalComponentFound))
				{
					foundAndReconciledMissingOrDifferentCentralNeuron = true;
				}
			}
			else if(findGroupDirectlyAbove(activatedNeuronWithMaxWordIndexCoverage2, activatedNeuronWithMaxWordIndexCoverage1))
			{
				//case 2:   2 found-DIFFERENTorMISSING
		        	//                                 \ 1 DIFFERENTorMISSING-found
				bool direction = false;
				if(splitGroupsConnectedByOptionalOrDifferentComponent(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage1, activatedNeuronWithMaxWordIndexCoverage2, partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage2lastActivatedIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, firstLayer, direction, optionalComponentFound))
				{
					foundAndReconciledMissingOrDifferentCentralNeuron = true;
				}
			}
		}
		else
		{
			//FUTURE: add support for multiple diff/missing components?
		}
	}
	
	return foundAndReconciledMissingOrDifferentCentralNeuron;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::performPropagationDirection(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool parseSentenceReverse, bool* neuronWithMaxWordIndexCoverageisPartiallyActivated, GIAtxtRelTranslatorRulesGroupNeuralNetwork** activatedNeuronWithMaxWordIndexCoverage, int* partiallyActivatedNeuronWithMaxWordIndexCoveragelastActivatedIndex, int* partiallyActivatedNeuronWithMaxWordIndexCoveragelastWordIndex, int* partiallyActivatedNeuronWithMaxWordIndexCoveragenumUnactivatedComponents)
{
	bool result = true;
	
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverseStrict(parseSentenceReverse, forwardPropogationSentenceData);
	//forwardPropogationSentenceData->findMissingOrOptionalComponent = true;	//NOTCHRRENTLYUSED
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = true;
	forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage = NULL;
	forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage = NULL;
	if(GIAtxtRelTranslatorNeuralNetworkPropagateCompact.performPropagation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer, false, 0))
	{

	}
	
	*neuronWithMaxWordIndexCoverageisPartiallyActivated = false;
	*activatedNeuronWithMaxWordIndexCoverage = NULL;
	if(forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage != NULL)
	{
		GIAtxtRelTranslatorNeuralNetworkPropagateCompact.identifyComponentIndexLastActivatedIndex(forwardPropogationSentenceData, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage, partiallyActivatedNeuronWithMaxWordIndexCoveragelastActivatedIndex);	//CHECKTHIS: forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage)
		*partiallyActivatedNeuronWithMaxWordIndexCoveragelastWordIndex = 0;
		*partiallyActivatedNeuronWithMaxWordIndexCoveragenumUnactivatedComponents = forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef->components.size() - forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->components.size();	//CHECKTHIS: forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverag
	}
	
	if(GIAtxtRelTranslatorNeuralNetworkPropagateCompact.verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage))
	{
		int activatedNeuronWithMaxWordIndexCoverageFirstWordIndex;	//CHECKTHIS
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			activatedNeuronWithMaxWordIndexCoverageFirstWordIndex = forwardPropogationSentenceData->sentenceContents->size()-1;
		}
		else
		{
			activatedNeuronWithMaxWordIndexCoverageFirstWordIndex = 0;
		}
	
		if(GIAtxtRelTranslatorNeuralNetworkPropagateCompact.verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage, activatedNeuronWithMaxWordIndexCoverageFirstWordIndex, true))
		{
			*neuronWithMaxWordIndexCoverageisPartiallyActivated = false;
			*activatedNeuronWithMaxWordIndexCoverage = forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->groupRef;
			if(!parseSentenceReverse)
			{
				*partiallyActivatedNeuronWithMaxWordIndexCoveragelastWordIndex = forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex;
			}
			else
			{
				*partiallyActivatedNeuronWithMaxWordIndexCoveragelastWordIndex = forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex;
			}

			*partiallyActivatedNeuronWithMaxWordIndexCoveragenumUnactivatedComponents = (*activatedNeuronWithMaxWordIndexCoverage)->components.size() - *partiallyActivatedNeuronWithMaxWordIndexCoveragelastActivatedIndex;
		}
		else
		{
			//fully activated group coverage+weight is > partially activated group coverage+weight

			*neuronWithMaxWordIndexCoverageisPartiallyActivated = true;
			*activatedNeuronWithMaxWordIndexCoverage = forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage->groupRef;
		}
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deinitialiseParseTreeGroupList(GIAtxtRelTranslatorRulesGroupTypes, parseTreeGroupListPointer);
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::splitGroupsConnectedByOptionalOrDifferentComponent(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* activatedNeuronWithMaxWordIndexCoverage1, GIAtxtRelTranslatorRulesGroupNeuralNetwork* activatedNeuronWithMaxWordIndexCoverage2, const int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex, const int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastActivatedIndex, const int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, const int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool direction, const bool optionalComponentFound)
{
	bool result = false;
	
	//
	//direction = true:
	//if(findGroupDirectlyAbove(activatedNeuronWithMaxWordIndexCoverage1 [below], activatedNeuronWithMaxWordIndexCoverage2 [above]))	
	//                                   2 DIFFERENTorMISSING-found
	//          1 found-DIFFERENTorMISSING /
	// or;
	//direction = false:
	//if(findGroupDirectlyAbove(activatedNeuronWithMaxWordIndexCoverage2 [below], activatedNeuronWithMaxWordIndexCoverage1 [above]))
	//           1 found-DIFFERENTorMISSING
	//                                 \ 2 DIFFERENTorMISSING-found	

	//split A and B (to Aa/Ab and Ba/Bb)
	//if optional: connect Aa to Bb
	//if different: 
		//perform another propagation from start of DIFFERENTorMISSING

	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	int differentOrMissingComponent1Index = partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex-1;	CHECKTHIS
	int differentOrMissingComponent2Index = partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex+1;
	#else
	*/
	int differentOrMissingComponent1Index = partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex+1;
	int differentOrMissingComponent2Index = partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex-1;
	//#endif
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* differentOrMissingComponent1 = activatedNeuronWithMaxWordIndexCoverage1->components[differentOrMissingComponent1Index];	//DIFFERENTorMISSINGcomponent
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* differentOrMissingComponent2 = activatedNeuronWithMaxWordIndexCoverage2->components[differentOrMissingComponent2Index];	//DIFFERENTorMISSINGcomponent	
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* differentOrMissingComponentLow = NULL;	
	GIAtxtRelTranslatorRulesComponentNeuralNetwork* differentOrMissingComponentHigh = NULL;	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* activatedNeuronWithMaxWordIndexCoverageLow = NULL;
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* activatedNeuronWithMaxWordIndexCoverageHigh = NULL;
	if(direction)
	{
		differentOrMissingComponentLow = differentOrMissingComponent1;
		differentOrMissingComponentHigh = differentOrMissingComponent2;
		activatedNeuronWithMaxWordIndexCoverageLow = activatedNeuronWithMaxWordIndexCoverage1;
		activatedNeuronWithMaxWordIndexCoverageHigh = activatedNeuronWithMaxWordIndexCoverage2;
	}
	else
	{
		differentOrMissingComponentLow = differentOrMissingComponent2;
		differentOrMissingComponentHigh = differentOrMissingComponent1;
		activatedNeuronWithMaxWordIndexCoverageLow = activatedNeuronWithMaxWordIndexCoverage2;
		activatedNeuronWithMaxWordIndexCoverageHigh = activatedNeuronWithMaxWordIndexCoverage1;
	}
				

	if(optionalComponentFound)
	{
		result = true;
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2

				//NO: differentOrMissingComponent->optional = true;
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_OPTIONAL_COMPONENTS
		differentOrMissingComponent1->optional = true;
		differentOrMissingComponent2->optional = true;
		#else
		//create a new group containing component 1 of group 1 and component 2 of group 2, and join it to whatever the highest original group is joined to above
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRules.getSequenceGrammarGroupTypeDefault(GIAtxtRelTranslatorRulesGroupTypes);
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronMerged = createNewGroup();		//1 and 2 without DIFFERENTorMISSING
		groupType->groups.push_back(newNeuronMerged);
		
		//CHECKTHIS;
		GIAtxtRelTranslatorRules.copyComponent(activatedNeuronWithMaxWordIndexCoverage1->components[partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex], &(newNeuronMerged->components));
		GIAtxtRelTranslatorRules.copyComponent(activatedNeuronWithMaxWordIndexCoverage2->components[partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex], &(newNeuronMerged->components));
		
		for(int i=0; i<newNeuronMerged->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = newNeuronMerged->components[i];
			for(int i2=0; i2<component->ANNbackGroupConnectionList.size(); i2++)
			{
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = component->ANNbackGroupConnectionList[i2];
				group->ANNfrontComponentConnectionList.push_back(component);
			}
		}
		/* //FUTURE: consider mirroring forward connections;
		for(int i2=0; i2<activatedNeuronWithMaxWordIndexCoverageHigh->ANNfrontGroupConnectionList.size(); i2++)
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = activatedNeuronWithMaxWordIndexCoverageHigh->ANNfrontGroupConnectionList[i2];
			newNeuronMerged->ANNfrontComponentConnectionList.push_back(component);
			component->ANNbackGroupConnectionList.push_back(newNeuronMerged);
		}
		*/
		
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(newNeuronMerged, &(newNeuronMerged->components), false, NULL);
		
		#endif
	
	#else
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_OPTIONAL_COMPONENTS
		differentOrMissingComponent1->optional = true;
		differentOrMissingComponent2->optional = true;
		#else
		//CHECKTHIS; offset by 10
		int indexToSplitVector1 = partiallyActivatedNeuronWithMaxWordIndexCoverage1lastActivatedIndex;
		int indexToSplitVector2 = partiallyActivatedNeuronWithMaxWordIndexCoverage2lastActivatedIndex;
		/*
		int indexToSplitVector1 = INT_DEFAULT_VALUE;
		int indexToSplitVector2 = INT_DEFAULT_VALUE;
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(false, forwardPropogationSentenceData);	//forward (see above)			
		GIAtxtRelTranslatorNeuralNetworkPropagateCompact.identifyComponentIndexLastActivatedIndex(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage1, &indexToSplitVector1);
		//GIAtxtRelTranslatorRulesGroupNeuralNetwork* newHiddenLayerNeuron1 = splitGroupAtLastActivatedComponent(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes, activatedNeuronWithMaxWordIndexCoverage1, indexToSplitVector1);
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(true, forwardPropogationSentenceData);	//reverse (see above)
		GIAtxtRelTranslatorNeuralNetworkPropagateCompact.identifyComponentIndexLastActivatedIndex(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage2, &indexToSplitVector2);
		//GIAtxtRelTranslatorRulesGroupNeuralNetwork* newHiddenLayerNeuron2 = splitGroupAtLastActivatedComponent(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes, activatedNeuronWithMaxWordIndexCoverage2, indexToSplitVector2);
		*/
		
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRules.getSequenceGrammarGroupTypeDefault(GIAtxtRelTranslatorRulesGroupTypes);
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron1split = createNewGroup();		//1 without DIFFERENTorMISSING
		groupType->groups.push_back(newNeuron1split);
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron2split = createNewGroup();		//2 without DIFFERENTorMISSING
		groupType->groups.push_back(newNeuron2split);	

		GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSplitHigh = NULL;
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSplitLow = NULL;
		if(direction)
		{
			newNeuronSplitLow = newNeuron1split;
			newNeuronSplitHigh = newNeuron2split;
		}
		else
		{
			newNeuronSplitLow = newNeuron2split;
			newNeuronSplitHigh = newNeuron1split;
		}
	
	
		//GIAtxtRelTranslatorRules.copyComponents(&(newHiddenLayerNeuron1->components), &(newNeuron1split->components));
		//GIAtxtRelTranslatorRules.copyComponents(&(newHiddenLayerNeuron2->components), &(newNeuron2split->components));
		GIAtxtRelTranslatorRules.copyComponents(&(activatedNeuronWithMaxWordIndexCoverage1->components), &(newNeuron1split->components));
		GIAtxtRelTranslatorRules.copyComponents(&(activatedNeuronWithMaxWordIndexCoverage2->components), &(newNeuron2split->components));
		newNeuron1split->components.erase(newNeuron1split->components.begin()+differentOrMissingComponent1Index);
		newNeuron2split->components.erase(newNeuron2split->components.begin()+differentOrMissingComponent2Index);
		
		for(int i=0; i<newNeuron1split->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = newNeuron1split->components[i];
			//if(i != differentOrMissingComponent1->componentIndex)
			//{
				for(int i2=0; i2<component->ANNbackGroupConnectionList.size(); i2++)
				{
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = component->ANNbackGroupConnectionList[i2];
					group->ANNfrontComponentConnectionList.push_back(component);
				}
			//}
		}
		for(int i=0; i<newNeuron2split->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = newNeuron2split->components[i];
			if(i != differentOrMissingComponent2->componentIndex)
			{
				for(int i2=0; i2<component->ANNbackGroupConnectionList.size(); i2++)
				{
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = component->ANNbackGroupConnectionList[i2];
					group->ANNfrontComponentConnectionList.push_back(component);
				}
			}
		}
		/* //FUTURE: consider mirroring forward connections;
		for(int i2=0; i2<activatedNeuronWithMaxWordIndexCoverageHigh->ANNfrontGroupConnectionList.size(); i2++)
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = activatedNeuronWithMaxWordIndexCoverageHigh->ANNfrontGroupConnectionList[i2];
			newNeuronSplitHigh->ANNfrontComponentConnectionList.push_back(component);
			component->ANNbackGroupConnectionList.push_back(newNeuronSplitHigh);
		}
		*/
		
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(newNeuron1split, newNeuron1split->components, false, NULL);
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(newNeuron2split, newNeuron2split->components, false, NULL);


		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverseStrict(true, forwardPropogationSentenceData);	//temp forward			
		if(direction)
		{
			addComponentToGroup(forwardPropogationSentenceData, newNeuron1split, newNeuron2split, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, true);		//add to start (create a new 'DIFFERENTorMISSING')
		}
		else
		{
			addComponentToGroup(forwardPropogationSentenceData, newNeuron2split, newNeuron1split, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);	//add to end (create a new 'DIFFERENTorMISSING')			
		}
		
		#endif
		
	#endif
	}
	else
	{	
		bool direction2 = true;
		if(findAndConnectDifferentOrMissingSection(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, firstLayer, direction2, differentOrMissingComponentHigh))
		{
			result = true;
		}
	}
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::findAndConnectDifferentOrMissingSection(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, const int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, const int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, const bool direction, GIAtxtRelTranslatorRulesComponentNeuralNetwork* differentOrMissingComponent)
{
	bool result = false;
	
	//perform another propagation from start of DIFFERENTorMISSING

	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.setParseSentenceReverse(direction, forwardPropogationSentenceData);	//ie set to parseReverse==true
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_REVERSE
	int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex-1;
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex+1;
	#else
	int activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed = partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex+1;
	int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed = partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex-1;
	#endif
	forwardPropogationSentenceData->recordActivatedNeuronWithMaxWordIndexCoverage = true;
	forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage = NULL;
	forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage = NULL;
	if(GIAtxtRelTranslatorNeuralNetworkPropagateCompact.performPropagation(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, firstLayer, false, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed))
	{

	}

	
	if(!GIAtxtRelTranslatorNeuralNetworkPropagateCompact.verifyActivatedNeuronsAtLeastOneBasic(forwardPropogationSentenceData, forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage))
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
		//NOT YET CODED (max 2 comp per neiron)
		int numberOfWordsMissing = (partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex-1) - partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex;
		/*
		int numberOfWordsMissing = 0;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			numberOfWordsMissing = (partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex-1) - partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex;
		}
		else
		{
			numberOfWordsMissing = partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex - (partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex+1);
		}
		*/
		if(numberOfWordsMissing <= 2)
		{
		#endif
			result = true;
			
			//no DIFFERENTorMISSING sequence found, create one; (CHECKTHIS)
			
			//create a new neuron2 with the missing components (POS words)
			GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron2withMissingPOScomponents = createNewHiddenLayerGroup(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes);
	
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				for(int w=partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex-1; w>partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex; w--)
				{
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron = (*firstLayer)[w];
					addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSentenceData, newNeuron2withMissingPOScomponents, inputLayerPOSneuron);	//if reverse; add to start [ie add to start]
				}
			}
			else
			{
				for(int w=partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex+1; w<partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex; w++)
				{
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron = (*firstLayer)[w];
					addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSentenceData, newNeuron2withMissingPOScomponents, inputLayerPOSneuron);	//if reverse; add to start [ie add to end]
				}	
			}
			
			addComponentToGroup(forwardPropogationSentenceData, newNeuron2withMissingPOScomponents, differentOrMissingComponent);
			
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
		}
		#endif
	}
	else
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
		if(forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage != NULL)
		#else
		if(!GIAtxtRelTranslatorNeuralNetworkPropagateCompact.verifyActivatedNeuronWithMaxWordIndexCoverage(forwardPropogationSentenceData, forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage, activatedNeuronWithMaxWordIndexCoverageFirstWordIndexAllowed, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, true))
		#endif
		{
			if(findAndConnectDifferentOrMissingSectionSub(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, firstLayer, direction, differentOrMissingComponent, true, forwardPropogationSentenceData->fullyActivatedNeuronWithMaxWordIndexCoverage))
			{
				result = true;
			}
		}
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
		else
		{
			if(findAndConnectDifferentOrMissingSectionSub(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, forwardPropogationSentenceData, partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, firstLayer, direction, differentOrMissingComponent, false, forwardPropogationSentenceData->partiallyActivatedNeuronWithMaxWordIndexCoverage))
			{
				result = true;
			}
		}
		#endif
	}

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
	GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deinitialiseParseTreeGroupList(GIAtxtRelTranslatorRulesGroupTypes, parseTreeGroupListPointer);
	#endif
	
	return result;
}


//TODO;

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::findAndConnectDifferentOrMissingSectionSub(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, int partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex, int partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex, int activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*>* firstLayer, bool direction, GIAtxtRelTranslatorRulesComponentNeuralNetwork* differentOrMissingComponent, bool isFullyActivatedNeuronWithMaxWordIndexCoverage, GIAtxtRelTranslatorRulesGroupParseTree* activatedNeuronWithMaxWordIndexCoverage)
{
	bool result = false;
	
	//fully activated group coverage+weight is > partially activated group coverage+weight
	bool adjacent = false;
	if(verifyLastWordIndex(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage, activatedNeuronWithMaxWordIndexCoverageLastWordIndexAllowed, &adjacent))
	{
		
		int activatedNeuronWithMaxWordIndexCoverageFirstWordIndex;
		int activatedNeuronWithMaxWordIndexCoverageLastWordIndex;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			activatedNeuronWithMaxWordIndexCoverageFirstWordIndex = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex;
			activatedNeuronWithMaxWordIndexCoverageLastWordIndex = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex;
		}
		else
		{
			activatedNeuronWithMaxWordIndexCoverageFirstWordIndex = activatedNeuronWithMaxWordIndexCoverage->parseTreeMinWordIndex;
			activatedNeuronWithMaxWordIndexCoverageLastWordIndex = activatedNeuronWithMaxWordIndexCoverage->parseTreeMaxWordIndex;
		}

		//neuron completely encapsulates missing/different words
		if(adjacent)
		{
			result = true;
			
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
			if(isFullyActivatedNeuronWithMaxWordIndexCoverage)
			{
			#endif
				//create connection from fullyActivatedNeuronWithMaxWordIndexCoverage to DIFFERENTorMISSINGcomponent
				addComponentToGroup(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage->groupRef, differentOrMissingComponent);
			#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
			}
			else
			{
				//split partiallyActivatedNeuronWithMaxWordIndexCoverage at current activated component, and connect activated part to DIFFERENTorMISSING

				//split partiallyActivatedNeuronWithMaxWordIndexCoverage at current activated component		
				
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
				int indexToSplitVector1 = INT_DEFAULT_VALUE;
				int indexToSplitVector2 = INT_DEFAULT_VALUE; 				
				GIAtxtRelTranslatorNeuralNetworkPropagateCompact.identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage, &indexToSplitVector1, &indexToSplitVector2);
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron = splitGroupAtLastActivatedComponentUnordered(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes, activatedNeuronWithMaxWordIndexCoverage->groupRef, indexToSplitVector1, indexToSplitVector2);
				#else							
				int indexToSplitVector = INT_DEFAULT_VALUE;
				GIAtxtRelTranslatorNeuralNetworkPropagateCompact.identifyComponentIndexLastActivatedIndex(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage, &indexToSplitVector);
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron = splitGroupAtLastActivatedComponent(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes, activatedNeuronWithMaxWordIndexCoverage->groupRef, indexToSplitVector);
				#endif
				//connect activated part to DIFFERENTorMISSING
				addComponentToGroup(forwardPropogationSentenceData, newNeuron, differentOrMissingComponent);
			}
			#endif
		}
		else
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
			//NOT YET CODED (max 2 comp per neuron)
			int numberOfWordsMissing = 0;
			if(forwardPropogationSentenceData->parseSentenceReverse)
			{
				numberOfWordsMissing = (activatedNeuronWithMaxWordIndexCoverageLastWordIndex-1) - partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex;
			}
			else
			{
				numberOfWordsMissing = partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex - activatedNeuronWithMaxWordIndexCoverageLastWordIndex+1;
			}
			if(numberOfWordsMissing <= 2)
			{
			#else
				if(isFullyActivatedNeuronWithMaxWordIndexCoverage)
				{
					//create a new neuron1 with the missing components (POS words), and connect both neuron1 and fullyActivatedNeuronWithMaxWordIndexCoverage to a new neuron3, then connect the new neuron3 to DIFFERENTorMISSINGcomponent
				}
				else
				{
					//split partiallyActivatedNeuronWithMaxWordIndexCoverage at current activated component, mark activated part as neuron1. then create a new neuron2 with the missing components (POS words), and connect both new neuron2 and neuron1 to a new neuron3, then connect the new neuron3 to DIFFERENTorMISSING
				}
				
				
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron1 = NULL;
				if(!isFullyActivatedNeuronWithMaxWordIndexCoverage)
				{
					//split partiallyActivatedNeuronWithMaxWordIndexCoverage at current activated component, mark activated part as neuron1	
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
					int indexToSplitVector1 = INT_DEFAULT_VALUE;
					int indexToSplitVector2 = INT_DEFAULT_VALUE;			
					GIAtxtRelTranslatorNeuralNetworkPropagateCompact.identifyComponentIndexFirstAndLastActivatedIndexUnordered(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage, &indexToSplitVector1, &indexToSplitVector2);
					GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron = splitGroupAtLastActivatedComponentUnordered(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes, activatedNeuronWithMaxWordIndexCoverage->groupRef, indexToSplitVector1, indexToSplitVector2);
					#else
					int indexToSplitVector = INT_DEFAULT_VALUE;
					GIAtxtRelTranslatorNeuralNetworkPropagateCompact.identifyComponentIndexLastActivatedIndex(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage, &indexToSplitVector);
					newNeuron1 = splitGroupAtLastActivatedComponent(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes, activatedNeuronWithMaxWordIndexCoverage->groupRef, indexToSplitVector);
					#endif
				}
			#endif
				result = true;
			
				//create a new neuron2 with the missing components (POS words)
				GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron2 = createNewHiddenLayerGroup(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes);
				if(forwardPropogationSentenceData->parseSentenceReverse)
				{
					for(int w=activatedNeuronWithMaxWordIndexCoverageLastWordIndex-1; w>partiallyActivatedNeuronWithMaxWordIndexCoverage1lastWordIndex; w--)
					{
						GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron = (*firstLayer)[w];
						addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSentenceData, newNeuron2, inputLayerPOSneuron);
					}
				}
				else
				{
					for(int w=activatedNeuronWithMaxWordIndexCoverageLastWordIndex+1; w<partiallyActivatedNeuronWithMaxWordIndexCoverage2lastWordIndex; w++)
					{
						GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron = (*firstLayer)[w];
						addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSentenceData, newNeuron2, inputLayerPOSneuron);
					}
				}


				GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron3 = createNewHiddenLayerGroup(forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupTypes);
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
				if(isFullyActivatedNeuronWithMaxWordIndexCoverage)
				{
				#endif
					//connect both neuron2 and fullyActivatedNeuronWithMaxWordIndexCoverage to a new neuron3
					addComponentToGroup(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage->groupRef, newNeuron3, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);	//OLD: addComponentToGroup(forwardPropogationSentenceData, activatedNeuronWithMaxWordIndexCoverage->groupRef, newNeuron2,
				#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
				}
				else
				{
					//connect both new neuron2 and neuron1 to a new neuron3
					addComponentToGroup(forwardPropogationSentenceData, newNeuron1, newNeuron3, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);	//CHECKTHIS check order
				}
				#endif
				addComponentToGroup(forwardPropogationSentenceData, newNeuron2, newNeuron3, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, false);
	
	
				//connect the new neuron3 to DIFFERENTorMISSINGcomponent
				addComponentToGroup(forwardPropogationSentenceData, newNeuron3, differentOrMissingComponent);
		
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_LIMIT_NUM_COMPONENTS_2
			}
			#endif
		}
	}
	else
	{
		//do nothing
	}
	
	return result;
}				
					


#endif







bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::addComponentToFirstLevelHiddenLayerGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron, GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup)
{
	bool result = true;

	addComponentToGroup(forwardPropogationSentenceData, inputLayerPOSneuron, newNeuronSequenceGroup, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING, false);
	//GIAtxtRelTranslatorNeuralNetworkFormation.createGroupANNconnection(group, higherLevelComponent);

	return result;
}

GIAtxtRelTranslatorRulesGroupNeuralNetwork* GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::createNewHiddenLayerGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes)
{
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
	cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::createNewHiddenLayerGroup" << endl;
	#endif
		
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuron = createNewGroup();	
	GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRules.getSequenceGrammarGroupTypeDefault(GIAtxtRelTranslatorRulesGroupTypes);	
	groupType->groups.push_back(newNeuron);

	return newNeuron;
}
/*
GIAtxtRelTranslatorRulesGroupNeuralNetwork* GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::createNewFirstLevelHiddenLayerGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesGroupNeuralNetwork* inputLayerPOSneuron)
{
	bool result = true;
	
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* newNeuronSequenceGroup = createNewGroup();	
	GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRules.getSequenceGrammarGroupTypeDefault(GIAtxtRelTranslatorRulesGroupTypes);	
	groupType->groups.push_back(newNeuronSequenceGroup);

	addComponentToFirstLevelHiddenLayerGroup(forwardPropogationSentenceData, inputLayerPOSneuron, newNeuronSequenceGroup);

	return newNeuronSequenceGroup;
}
*/

//NB indexToSplitVector = lastActivatedComponent (the last component index in the first part of component to be splitted) (do not assume normal order: "first" and "last" definitions respect (forwardPropogationSentenceData->parseSentenceReverse))
GIAtxtRelTranslatorRulesGroupNeuralNetwork* GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::splitGroupAtLastActivatedComponent(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesGroupNeuralNetwork* neuronToSplit, int indexToSplitVector)
{
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* newHiddenLayerNeuron = createNewGroup();	
	GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRules.getSequenceGrammarGroupTypeDefault(GIAtxtRelTranslatorRulesGroupTypes);	
	groupType->groups.push_back(newHiddenLayerNeuron);
		
	vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components = &(neuronToSplit->components);
	vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> componentsPart1(components->begin(), components->begin() + indexToSplitVector+1);
        vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> componentsPart2(components->begin() + indexToSplitVector+1, components->end());
	
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		newHiddenLayerNeuron->components = componentsPart2;
		neuronToSplit->components = componentsPart1;
	}
	else
	{
		newHiddenLayerNeuron->components = componentsPart1;
		neuronToSplit->components = componentsPart2;
	}
	GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(newHiddenLayerNeuron, &(newHiddenLayerNeuron->components), false, NULL);
	GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);	
	
	addComponentToGroup(forwardPropogationSentenceData, newHiddenLayerNeuron, neuronToSplit, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, true);	//CHECKTHIS	
		
	return newHiddenLayerNeuron;
}

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION
//indexToSplitVector1 = firstActivatedComponent (unordered; always assume normal order: "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse))
//indexToSplitVector2 = lastActivatedComponent (unordered; always assume normal order: "first" and "last" definitions do not respect (forwardPropogationSentenceData->parseSentenceReverse))
GIAtxtRelTranslatorRulesGroupNeuralNetwork* GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::splitGroupAtLastActivatedComponentUnordered(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIAtxtRelTranslatorRulesGroupNeuralNetwork* neuronToSplit, int indexToSplitVector1, int indexToSplitVector2)
{
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* newHiddenLayerNeuron = createNewGroup();	
	GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRules.getSequenceGrammarGroupTypeDefault(GIAtxtRelTranslatorRulesGroupTypes);	
	groupType->groups.push_back(newHiddenLayerNeuron);
				
	vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components = &(neuronToSplit->components);
	
	if(indexToSplitVector1 == 0)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_SPLIT
		cout << "split at right (indexToSplitVector2) only: newHiddenLayerNeuron->groupIndex = " << newHiddenLayerNeuron->groupIndex << ", neuronToSplit->groupIndex = " << neuronToSplit->groupIndex << endl;
		#endif
		//split at right of centre (indexToSplitVector2) only - there was no unmatched first section (from GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_SUPPORT_PARTIAL_SENTENCE_PROPAGATION)
		vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> componentsPart1(components->begin(), components->begin()+indexToSplitVector2+1);
        	vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> componentsPart2(components->begin()+indexToSplitVector2+1, components->end());

		newHiddenLayerNeuron->components = componentsPart1;
		neuronToSplit->components = componentsPart2;
		
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(newHiddenLayerNeuron, &(newHiddenLayerNeuron->components), false, NULL);
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);	

		bool insertAtStart = true;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			insertAtStart = false;	//compensate for addComponentToGroup specification limitation
		}
		addComponentToGroup(forwardPropogationSentenceData, newHiddenLayerNeuron, neuronToSplit, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, insertAtStart);	//CHECKTHIS
	}
	else if(indexToSplitVector2 == components->size()-1)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_SPLIT
		cout << "split at left (indexToSplitVector1) only: newHiddenLayerNeuron->groupIndex = " << newHiddenLayerNeuron->groupIndex << ", neuronToSplit->groupIndex = " << neuronToSplit->groupIndex << endl;
		#endif
		//split at left of centre (indexToSplitVector1) only - there was no unmatched last section
		vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> componentsPart1(components->begin(), components->begin()+indexToSplitVector1);
		vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> componentsPart2(components->begin()+indexToSplitVector1, components->end());
		
		newHiddenLayerNeuron->components = componentsPart2;
		neuronToSplit->components = componentsPart1;
		
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(newHiddenLayerNeuron, &(newHiddenLayerNeuron->components), false, NULL);
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);	

		bool insertAtStart = false;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			insertAtStart = true;	//compensate for addComponentToGroup specification limitation
		}
		addComponentToGroup(forwardPropogationSentenceData, newHiddenLayerNeuron, neuronToSplit, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, insertAtStart);	//CHECKTHIS
	}
	else
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_SPLIT
		cout << "split middle: newHiddenLayerNeuron->groupIndex = " << newHiddenLayerNeuron->groupIndex << ", neuronToSplit->groupIndex = " << neuronToSplit->groupIndex << endl;
		#endif
		//split middle
		vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> componentsPart1(components->begin(), components->begin()+indexToSplitVector1);
		vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> componentsPart2(components->begin()+indexToSplitVector1, components->begin()+indexToSplitVector2+1);	
		vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*> componentsPart3(components->begin()+indexToSplitVector2+1, components->end());		
		
		newHiddenLayerNeuron->components = componentsPart2;
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(newHiddenLayerNeuron, &(newHiddenLayerNeuron->components), false, NULL);

		//neuronToSplit->components = componentsPart1;
		neuronToSplit->components.clear();
		neuronToSplit->components.insert(neuronToSplit->components.end(), componentsPart1.begin(), componentsPart1.end());
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);
		
		bool insertAtStart = false;
		if(forwardPropogationSentenceData->parseSentenceReverse)
		{
			insertAtStart = true;	//compensate for addComponentToGroup specification limitation
		}
		addComponentToGroup(forwardPropogationSentenceData, newHiddenLayerNeuron, neuronToSplit, GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP, insertAtStart);	//CHECKTHIS
		
		neuronToSplit->components.insert(neuronToSplit->components.end(), componentsPart3.begin(), componentsPart3.end());
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(neuronToSplit, &(neuronToSplit->components), false, NULL);
	}

	/*
	cout << "neuronToSplit->components.size() = " << neuronToSplit->components.size() << endl;
	cout << "newHiddenLayerNeuron->components.size() = " << newHiddenLayerNeuron->components.size() << endl;
	*/
	
	return newHiddenLayerNeuron;
}
#endif




	
bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::addComponentToGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesGroupNeuralNetwork* higherLevelComponentGroupOwner, int componentType, bool insertAtStart)
{
	bool result = true;

	GIAtxtRelTranslatorRulesComponentNeuralNetwork* newComponent = new GIAtxtRelTranslatorRulesComponentNeuralNetwork();

	newComponent->ownerGroup = higherLevelComponentGroupOwner;
	
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		insertAtStart = !insertAtStart;
	}
	if(insertAtStart)
	{
		int componentIndex = GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_COMPONENT_INDEX_FIRST;
		newComponent->componentIndex = componentIndex;
		//higherLevelComponentGroupOwner->components.push_front(newComponent);
		higherLevelComponentGroupOwner->components.insert(higherLevelComponentGroupOwner->components.begin(), newComponent);
		GIAtxtRelTranslatorRules.updateComponentsOwnerGroupAndIndexes(higherLevelComponentGroupOwner, &(higherLevelComponentGroupOwner->components), false, NULL);
	}
	else
	{
		int componentIndex = GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_COMPONENT_INDEX_FIRST+higherLevelComponentGroupOwner->components.size();
		newComponent->componentIndex = componentIndex;
		higherLevelComponentGroupOwner->components.push_back(newComponent);
	}
			
	newComponent->componentType = componentType;
	newComponent->groupRefName = group->groupName;
	newComponent->groupTypeRefName = group->groupTypeName;
	/*
	this is only for initialisation (GIAtxtRelTranslatorNeuralNetworkFormation) and parseTreeGroup components?;
	newComponent->groupRef = group;
	newComponent->groupTypeRef = group->groupType;
	*/
	if(componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
	
	}
	else if (componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		newComponent->stringType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS;
		newComponent->wordPOStype = group->wordDataTemp->wordPOStype;	
	}
	else
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::addComponentToGroup error: componentType != GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP/STRING" << endl;
		exit(EXIT_ERROR);
	}

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
	cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::addComponentToGroup: higherLevelComponentGroupOwner->groupIndex = " << higherLevelComponentGroupOwner->groupIndex << ", newComponent->componentIndex = " << newComponent->componentIndex << endl;
	#endif
	
	GIAtxtRelTranslatorNeuralNetworkFormation.createGroupANNconnection(group, newComponent);
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::addComponentToGroup(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, GIAtxtRelTranslatorRulesComponentNeuralNetwork* higherLevelComponent)
{
	bool result = true;
	
	/*		
	component->componentType = componentType;
	component->groupRefName = group->groupName;
	component->groupTypeRefName = group->groupTypeName;
	//this is only for initialisation (GIAtxtRelTranslatorNeuralNetworkFormation) and parseTreeGroup components?;
	//component->groupRef = group;
	//component->groupTypeRef = group->groupType;
	if(componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
	
	}
	else if (componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		component->stringType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS;
		component->wordPOStype = group->wordDataTemp->wordPOStype;	
	}
	else
	{
		cerr << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::addComponentToGroup error: componentType != GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP/STRING" << endl;
		exit(EXIT_ERROR);
	}
	*/

	GIAtxtRelTranslatorNeuralNetworkFormation.createGroupANNconnection(group, higherLevelComponent);
	
	return result;
}


GIAtxtRelTranslatorRulesGroupNeuralNetwork* GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::createNewGroup()
{
	GIAtxtRelTranslatorRulesGroupNeuralNetwork* newGroup = new GIAtxtRelTranslatorRulesGroupNeuralNetwork();
	newGroup->groupName = GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_GROUP_NAME;
	newGroup->groupTypeName = GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_GROUP_TYPE_NAME;
	
	int* newNeuronIndex = GIAtxtRelTranslatorRules.getNewGroupIndex();
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_PRINT_GROUP_INDICES
	cout << "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::createNewGroup - groupIndex = " << *newNeuronIndex << endl;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ANN
	newGroup->initiateANNneuron("groupIndex:" + SHAREDvars.convertIntToString(*newNeuronIndex));	//GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_NEURON_NAME	//newGroup->groupTypeName + ":" + newGroup->groupName
	#endif
	newGroup->groupIndex = *newNeuronIndex;
	*newNeuronIndex = *newNeuronIndex + 1;
	
	return newGroup;
}

bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::findGroupDirectlyAbove(GIAtxtRelTranslatorRulesGroupNeuralNetwork* group1, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group2)
{
	bool result = false;
	
	for(int i=0; i<group1->ANNfrontComponentConnectionList.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* currentComponent = (group1->ANNfrontComponentConnectionList)[i];

		GIAtxtRelTranslatorRulesGroupNeuralNetwork* ownerGroup = currentComponent->ownerGroup;	
		GIAtxtRelTranslatorRulesComponentNeuralNetwork* ownerComponent = currentComponent;	//not used by GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR (as subcomponents are never generated)
		int componentIndex = currentComponent->componentIndex;	
		int ownerComponentIndex = componentIndex;	//not used by GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR (as subcomponents are never generated)
	
		if(ownerGroup == group2)
		{
			result = true;
		}
	}
	
	return result;
}


bool GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass::verifyLastWordIndex(GIAtxtRelTranslatorNeuralNetworkForwardPropogationSentenceData* forwardPropogationSentenceData, GIAtxtRelTranslatorRulesGroupParseTree* parseTreeGroup, int lastWordIndex, bool* adjacent)
{
	bool result = false;
	
	if(forwardPropogationSentenceData->parseSentenceReverse)
	{
		if(parseTreeGroup->parseTreeMinWordIndex >= lastWordIndex)
		{
			result = true;
		}
		if(parseTreeGroup->parseTreeMinWordIndex == lastWordIndex)
		{
			*adjacent = true;
		}
	}
	else
	{
		if(parseTreeGroup->parseTreeMaxWordIndex <= lastWordIndex)
		{
			result = true;
		}
		if(parseTreeGroup->parseTreeMaxWordIndex == lastWordIndex)
		{
			*adjacent = true;
		}
	}

	return result;
}



		

#endif

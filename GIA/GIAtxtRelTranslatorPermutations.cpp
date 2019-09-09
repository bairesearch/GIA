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
 * File Name: GIAtxtRelTranslatorPermutations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3j2j 10-August-2019
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Permutations
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorPermutations.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
static int performanceMaxPermutationIndexRecord;	//temp var (per sentence)
static bool parseIsolatedSubreferenceSetsRecord;
#endif

bool GIAtxtRelTranslatorPermutationsClass::executeTxtRelTranslatorWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes)
{
	bool result = true;

	/*
	algorithm (v0.1);
		For every word in sentence
			calculate pos ambiguity info (possible pos types)
		Generate all possible pos sequences representing sentence
		For every possible pos sequence representing sentence
			identify the set of logic/reference set groups supporting (successfully parsing) the pos sequence
				interpret unknown words (pos types) as wild cards
				take the logic/reference set group that most successfully parses the sentence

		In the future use a neural net to train the system to identify new rule groups (or upgrade/refine rule groups)
	*/
	
	//cout << "GIAtxtRelTranslatorPermutationsClass::executeTxtRelTranslatorWrapper" << endl;


	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_TEST_VERIFICATION_AFTER_GENERATION
	//code from GIAtranslatorClass::parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences;
	//count maxNumberSentences
	GIApreprocessorSentence* currentSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		if(currentSentenceInList->sentenceIndexOriginal > translatorVariables->maxNumberSentences)
		{
			translatorVariables->maxNumberSentences = currentSentenceInList->sentenceIndexOriginal;
		}
		currentSentenceInList = currentSentenceInList->next;
	}	
	#endif
	
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_TEST_VERIFICATION_AFTER_GENERATION
		translatorVariables->currentPreprocessorSentenceInList = currentGIApreprocessorSentenceInList;
		#endif
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		cout << "GIAtxtRelTranslatorPermutationsClass::executeTxtRelTranslator{}: sentence " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << ", sentenceContents = " << GIApreprocessorWordClassObject.printWordListString(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList)) << endl;
		#endif
			
		if(!executeTxtRelTranslatorWrapper2(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, currentGIApreprocessorSentenceInList))
		{
			result = false;
		}
				
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
	if(!transferParseTreePOStypeInferredToWordList(translatorVariables))	//is this required without GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START?
	{
		result = false;
	}
	#endif
	
	return result;
}

bool GIAtxtRelTranslatorPermutationsClass::executeTxtRelTranslatorWrapper2(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList)
{
	bool result = true;

	/*
	algorithm (v0.1);
		For every word in sentence
			calculate pos ambiguity info (possible pos types)
		Generate all possible pos sequences representing sentence
		For every possible pos sequence representing sentence
			identify the set of logic/reference set groups supporting (successfully parsing) the pos sequence
				interpret unknown words (pos types) as wild cards
				take the logic/reference set group that most successfully parses the sentence

		In the future use a neural net to train the system to identify new rule groups (or upgrade/refine rule groups)
	*/
	
	//cout << "GIAtxtRelTranslatorPermutationsClass::executeTxtRelTranslatorWrapper2" << endl;

	vector<uint64_t> POSambiguityInfoPermutation;
	vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
	vector<string>* explicitWordList = GIAtxtRelTranslatorNeuralNetworkFormation.getExplicitWordList();
	#else
	vector<string>* explicitWordList = NULL;
	#endif

	if(!GIApreprocessorPOStagger.determinePOSambiguityInfoWrapper(sentenceContents, &POSambiguityInfoPermutation, explicitWordList))
	{
		result = false;
	}

	if(!executeTxtRelTranslator(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, currentGIApreprocessorSentenceInList, &POSambiguityInfoPermutation))
	{
		result = false;
	}

	//cout << "2 currentGIApreprocessorSentenceInList->firstParseTreeGroup = " << currentGIApreprocessorSentenceInList->firstParseTreeGroup->groupName << endl;

	return result;
}


#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
bool GIAtxtRelTranslatorPermutationsClass::transferParseTreePOStypeInferredToWordList(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES
	cout << "printParseTreeDebug: " << endl;
	#endif
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);

		//this will replace the sentenceContents word->wordPOStypeInferred values with their ideal value as stored in the parse tree (in the case where the ideal word->wordPOStypeInferred values were overwritten by a more recent bad parse):
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
		if(currentGIApreprocessorSentenceInList->firstParseTreeGroup != NULL)
		{
		#endif
			GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup = currentGIApreprocessorSentenceInList->firstParseTreeGroup;
			int layer = GIA_TXT_REL_TRANSLATOR_RULES_LAYER_START;
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES
			cout << "firstParseTreeGroup: groupTypeName = " << firstParseTreeGroup->groupTypeName << ", groupName = " << firstParseTreeGroup->groupName << endl;
			#endif
			if(!transferParseTreePOStypeInferredToWordList(firstParseTreeGroup, layer))
			{
				result = false;
			}

			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES
			for(int w=0; w<sentenceContents->size(); w++)
			{
				GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
				cout << "GIApreprocessorPOStypeNameArray[contextWord->wordPOStypeInferred] = " << GIApreprocessorPOStypeNameArray[contextWord->wordPOStypeInferred] << endl;
			}
			#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
		}
		#endif
		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES
	cout << "\n" << endl;
	#endif

	return result;
}

bool GIAtxtRelTranslatorPermutationsClass::transferParseTreePOStypeInferredToWordList(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
		
		//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES
		//cout << "transferParseTreePOStypeInferredToWordList currentParseTreeGroup: " <<  currentParseTreeGroup->groupTypeName << ":" << currentParseTreeGroup->groupName << endl;		
		//#endif
		//cout << "(currentParseTreeGroup->components).size() = " << (currentParseTreeGroup->components).size() << endl;
		//cout << "i = " << i << endl;
		
		GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
		
		if(currentParseTreeComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)	//redundant
		{
			if(currentParseTreeComponent->candidateStringMatch != NULL)
			{
				//this will replace the sentenceContents word->wordPOStypeInferred with the ideal value as stored in the parse tree (in the case where the ideal word->wordPOStypeInferred value was overwritten by more recent bad parse):
				currentParseTreeComponent->candidateStringMatch->wordPOStypeInferred = currentParseTreeComponent->wordPOStypeInferred;
				//cout << "currentParseTreeComponent->candidateStringMatch->wordPOStypeInferred = " << currentParseTreeComponent->candidateStringMatch->wordPOStypeInferred << endl;
			}
		}
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printComponent(currentParseTreeComponent, layer);
		#endif
		if(currentParseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!transferParseTreePOStypeInferredToWordList(currentParseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}
	
	return result;
}
#endif


bool GIAtxtRelTranslatorPermutationsClass::executeTxtRelTranslator(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, vector<uint64_t>* POSambiguityInfoPermutation)
{
	bool result = true;
	
	vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);

	/*
	algorithm (v0.1);
		For every word in sentence
			calculate pos ambiguity info (possible pos types)
		Generate all possible pos sequences representing sentence
		For every possible pos sequence representing sentence
			identify the set of logic/reference set groups supporting (successfully parsing) the pos sequence
				interpret unknown words (pos types) as wild cards
				take the logic/reference set group that most successfully parses the sentence

		In the future use a neural net to train the system to identify new rule groups (or upgrade/refine rule groups)
	*/
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup = NULL;	//not used by GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK (write directly to currentGIApreprocessorSentenceInList->firstParseTreeGroup)
	#else
	GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup = new GIAtxtRelTranslatorRulesGroupParseTree();
	currentGIApreprocessorSentenceInList->firstParseTreeGroup = firstParseTreeGroup;
	#endif
		
	int performance = 0; 
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	int minIndexOfMatchesFoundBackupOptimum = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculateMinIndexOfMatchesFound(sentenceContents);
	vector<vector<uint64_t>*> POSambiguityInfoUnambiguousPermutationArray;
	vector<uint64_t>* POSambiguityInfoUnambiguousPermutationNew = new vector<uint64_t>(POSambiguityInfoPermutation->size());
	POSambiguityInfoUnambiguousPermutationArray.push_back(POSambiguityInfoUnambiguousPermutationNew);
	GIApreprocessorPOStagger.generatePOSambiguityInfoUnambiguousPermutationArray(&POSambiguityInfoUnambiguousPermutationArray, POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, 0);
	int iOptimum = 0;
	#endif
	bool foundParse = false;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	if(translatorVariables->parserDemarkateOptimumPathway)
	{
	#endif
		bool parseIsolatedSubreferenceSets1 = false;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
		GIApreprocessorSentenceClassObject.calculateParseIsolatedSubreferenceSets1(currentGIApreprocessorSentenceInList, &parseIsolatedSubreferenceSets1);
		#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		if(generateParseTreeIntroWrapper(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, currentGIApreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSets1, &POSambiguityInfoUnambiguousPermutationArray, &iOptimum, &minIndexOfMatchesFoundBackupOptimum))	
		#else
		if(generateParseTreeIntroWrapper(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, currentGIApreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSets1, POSambiguityInfoPermutation))
		#endif
		{
			foundParse = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
			parseIsolatedSubreferenceSetsRecord = parseIsolatedSubreferenceSets1;
			#endif
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS
		if(!foundParse)
		{
			bool parseIsolatedSubreferenceSets2 = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
			if(GIApreprocessorSentenceClassObject.calculateParseIsolatedSubreferenceSets2(currentGIApreprocessorSentenceInList, &parseIsolatedSubreferenceSets2))
			{
			#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
				if(generateParseTreeIntroWrapper(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, currentGIApreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSets2, &POSambiguityInfoUnambiguousPermutationArray, &iOptimum, &minIndexOfMatchesFoundBackupOptimum))	
				#else
				if(generateParseTreeIntroWrapper(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, currentGIApreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSets2, POSambiguityInfoPermutation))
				#endif
				{
					foundParse = true;
					#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
					parseIsolatedSubreferenceSetsRecord = parseIsolatedSubreferenceSets2;
					#endif
				}
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
			}
			#endif
		}
		#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	}
	else
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		if(generateParseTreeIntroWrapper(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, currentGIApreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSetsRecord, &POSambiguityInfoUnambiguousPermutationArray, &iOptimum, &minIndexOfMatchesFoundBackupOptimum))	
		#else
		if(generateParseTreeIntroWrapper(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, currentGIApreprocessorSentenceInList, firstParseTreeGroup, &performance, parseIsolatedSubreferenceSetsRecord, POSambiguityInfoPermutation))
		#endif
		{
			foundParse = true;
		}
	}
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(foundParse)
	{
		//cout << "GIAtxtRelTranslatorPermutationsClass::executeTxtRelTranslator{}: Succeeded to parse sentence " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << ", sentenceContents = " << GIApreprocessorWordClassObject.printWordListString(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList)) << endl;
	}
	else
	{
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
		GIAtxtRelTranslatorNeuralNetworkPropagateInverse.clearAllWordsAlreadyFoundMatchInComponent(sentenceContents, minIndexOfMatchesFoundBackupOptimum);	//redundant?	
		#endif
		cerr << "GIAtxtRelTranslatorPermutationsClass::executeTxtRelTranslator{}: Failed to parse sentence " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << ", ";
		GIApreprocessorSentenceClassObject.printSentence(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList), true);
		//cerr << "GIAtxtRelTranslatorPermutationsClass::executeTxtRelTranslator{}: Failed to parse sentence " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << ", sentenceContents = " << GIApreprocessorWordClassObject.printWordListString(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList)) << endl;
		//exit(EXIT_ERROR);
	}
	#endif

	return result;
}


#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
bool GIAtxtRelTranslatorPermutationsClass::generateParseTreeIntroWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets, vector<vector<uint64_t>*>* POSambiguityInfoUnambiguousPermutationArray, int* iOptimum, int* minIndexOfMatchesFoundBackupOptimum)
#else
bool GIAtxtRelTranslatorPermutationsClass::generateParseTreeIntroWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets, vector<uint64_t>* POSambiguityInfoPermutation)
#endif
{
	bool result = false;
	
	vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	int performanceMaxPermutationIndex = INT_DEFAULT_VALUE;
	int performanceMax = 0;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	if(translatorVariables->parserDemarkateOptimumPathway)
	{
	#endif
	
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_DEBUG_SINGLE_POS_PERMUTATION
		for(int i=0; i<1; i++)
		#else
		for(int i=0; i<POSambiguityInfoUnambiguousPermutationArray->size(); i++)
		#endif
		{
			vector<uint64_t>* POSambiguityInfoPermutationTemp = (*POSambiguityInfoUnambiguousPermutationArray)[i];
			GIApreprocessorPOStagger.setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, POSambiguityInfoPermutationTemp);
	
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
			cout << "POSambiguityInfoUnambiguousPermutationArray index = " << i << endl;
			//GIApreprocessorPOStagger.printPOSambiguityInfoPermutationAssumeUnambiguous(POSambiguityInfoPermutationTemp);	//printPOSambiguityInfoPermutation
			#endif
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS3
			cout << "POSambiguityInfoUnambiguousPermutationArray index = " << i << endl;
			GIApreprocessorPOStagger.printPOSambiguityInfoPermutationAssumeUnambiguous(POSambiguityInfoPermutationTemp);		
			#endif
	
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
			GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroupTemp = NULL;
			#else
			GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroupTemp = new GIAtxtRelTranslatorRulesGroupParseTree();		
			#endif
	
			int minIndexOfMatchesFoundBackup2 = GIAtxtRelTranslatorNeuralNetworkPropagateOperations.calculateMinIndexOfMatchesFound(sentenceContents);
		#else
		for(int w=0; w<sentenceContents->size(); w++)
		{
			(sentenceContents->at(w))->POSambiguityInfo = (*POSambiguityInfoPermutation)[w];
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
		GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroupTemp = NULL;
		#else
		GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroupTemp = firstParseTreeGroup;
		#endif
		#endif
		

			int performanceTemp = 0;
			bool passedTemp = false;
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
			bool parserEnabled = false;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR
			bool createNewConnections = true; 
			vector<GIAtxtRelTranslatorRulesGroupNeuralNetwork*> firstLayerNOTUSED;
			if(GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerate.executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, &firstParseTreeGroupTemp, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp, createNewConnections, &firstLayerNOTUSED))
			#else
			if(GIAtxtRelTranslatorNeuralNetwork.executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, &firstParseTreeGroupTemp, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp))
			#endif
			#else	
			if(GIAtxtRelTranslatorNeuralNetworkPropagateInverse.generateParseTreeIntro(GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, firstParseTreeGroupTemp, &performanceTemp, parseIsolatedSubreferenceSets))
			#endif
			{
				passedTemp =  true;
				result = true;
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
				cout << "passedTemp" << endl;
				#endif
				#ifndef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START	
				*performance = performanceTemp;
				#endif
				
				//cout << "performanceTemp = " << performanceTemp << endl;
				//exit(EXIT_ERROR);
			}
			else
			{
				//cout << "!executeTxtRelTranslatorNeuralNetwork" << endl;		
			}
	
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
			if(updatePerformanceNeuralNetwork(performanceTemp, performance, currentGIApreprocessorSentenceInList, firstParseTreeGroupTemp, passedTemp, i, &performanceMaxPermutationIndex))
			{
				*iOptimum = i;	
			}
			
			//cout << "currentGIApreprocessorSentenceInList->firstParseTreeGroup = " << currentGIApreprocessorSentenceInList->firstParseTreeGroup->groupName << endl;
	
		}
		#else
			if(GIAtxtRelTranslatorNeuralNetworkPropagateInverse.updatePerformance(performanceTemp, performance, firstParseTreeGroup, firstParseTreeGroupTemp, passedTemp, minIndexOfMatchesFoundBackupOptimum, sentenceContents, minIndexOfMatchesFoundBackup2, NULL))
			{
				*iOptimum = i;	
			}
		}
		if(result)
		{
			//cout << "performance = " << performance << endl;
			GIAtxtRelTranslatorNeuralNetworkPropagateInverse.restoreAllWordsAlreadyFoundMatchInComponent(sentenceContents, *performance);
		}
		#endif
		
		#endif
			
	
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
		if(result)
		{
			
			//regenerate parse tree with optimum path
			bool parserEnabled = true;
			vector<uint64_t>* POSambiguityInfoPermutationTemp = (*POSambiguityInfoUnambiguousPermutationArray)[performanceMaxPermutationIndex];
			GIApreprocessorPOStagger.setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, POSambiguityInfoPermutationTemp);
			int performanceTemp = 0;
			
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_UNOPTIMISED
			if(translatorVariables->parserAllowed)
			{
			#endif
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
				parserEnabled = false; 
				performanceTemp = 0;
				//derive optimum pathway (store in parseTree)
				if(!GIAtxtRelTranslatorNeuralNetwork.executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, &firstParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp))
				{
					result = false;	
				}
				//mark optimum pathway
				if(!GIAtxtRelTranslatorNeuralNetwork.demarkateOptimumPathwayBackprop(firstParseTreeGroup))
				{
					result = false;
				}
				#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
				GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteParseTree(firstParseTreeGroup, 0);
				#endif
				#endif
				
				parserEnabled = true;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_UNOPTIMISED
			}
			else
			{
				parserEnabled = false;
			}
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
			performanceMaxPermutationIndexRecord = performanceMaxPermutationIndex;
			#else
			result = false;
			performanceTemp = 0;
			if(GIAtxtRelTranslatorNeuralNetwork.executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, &firstParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp))
			{
				result = true;	
				*performance = sentenceContents->size();	//hard set to maximum performance
			}
	
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_DELETE_INCOMPLETE_PATHS_SEMANTIC_RELATIONS
			GIAtxtRelTranslatorNeuralNetworkPropagateOperations.resetAllNeuronComponents(GIAtxtRelTranslatorRulesGroupTypes, GIA_TXT_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_DEMARKATEOPTIMUMPATHWAY_RESET);
			#endif
			
			#endif
	
		}
		else
		{
			performanceMaxPermutationIndexRecord = INT_DEFAULT_VALUE;
		}
		#endif
		
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	}
	else
	{
		if(performanceMaxPermutationIndexRecord != INT_DEFAULT_VALUE)
		{
			bool parserEnabled = true;
			vector<uint64_t>* POSambiguityInfoPermutationTemp = (*POSambiguityInfoUnambiguousPermutationArray)[performanceMaxPermutationIndexRecord];
			GIApreprocessorPOStagger.setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, POSambiguityInfoPermutationTemp);
			int performanceTemp = 0;
			
			if(GIAtxtRelTranslatorNeuralNetwork.executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, &firstParseTreeGroup, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp))
			{
				result = true;	
				*performance = sentenceContents->size();	//hard set to maximum performance
			}
			
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PRINT_FINAL2
			if(firstParseTreeGroup != NULL)
			{
				cout << "(firstParseTreeGroup != NULL)" << endl;
				printParseTree(firstParseTreeGroup, 0);
			}
			#endif
		}
	}
	#endif

	return result;
}	

	
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
//based on GIAtxtRelTranslatorNeuralNetworkPropagateInverseClass::updatePerformanceNeuralNetwork
bool GIAtxtRelTranslatorPermutationsClass::updatePerformanceNeuralNetwork(const int performanceTemp, int* performance, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroupTemp, const bool passedTemp, const int permutationIndex, int* performanceMaxPermutationIndex)
{
	bool result = false;
	
	if(passedTemp)
	{
		if(performanceTemp > *performance)
		{
			result = true;
			*performance = performanceTemp;
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_LIGHT_OPTIMISED_FREE_MEMORY
			if(currentGIApreprocessorSentenceInList->firstParseTreeGroup != NULL)
			{
				GIAtxtRelTranslatorNeuralNetworkPropagateOperations.deleteParseTree(currentGIApreprocessorSentenceInList->firstParseTreeGroup, 0);
			}
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
			currentGIApreprocessorSentenceInList->firstParseTreeGroup = firstParseTreeGroupTemp;
			#endif
			*performanceMaxPermutationIndex = permutationIndex;
			
			//cout << "firstParseTreeGroup->groupName = " << firstParseTreeGroupTemp->groupName << endl;
			//exit(EXIT_ERROR);
		}
	}

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SAVE_PARSE_TREE
	if(!result)
	{
		//delete all subgroups/components recursively in currentParseTreeGroupTemp
		if(firstParseTreeGroupTemp != NULL)
		{
			//deleteAllSubgroupsRecurse(firstParseTreeGroupTemp, 1);	//CHECKTHIS
		}
	}
	#endif
	
	return result;
}

//based on GIAtxtRelTranslatorNeuralNetworkPropagateInverseClass::deleteAllSubgroupsRecurse
//note it assumes subcomponents
bool GIAtxtRelTranslatorPermutationsClass::deleteAllSubgroupsRecurse(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
			if(currentParseTreeComponent->parseTreeGroupRef != NULL)
			{
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
				GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printParseTreeDebugIndentation(layer);
				cout << "deleteAllSubgroupsRecurse" << endl;
				#endif
				
				if(!deleteAllSubgroupsRecurse(currentParseTreeComponent->parseTreeGroupRef, layer+1))
				{
					result = false;
				}
			}
			
			delete currentParseTreeComponent;
		}
		delete currentParseTreeGroup;
	}
	
	return result;
}

#endif



#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PRINT_FINAL2
bool GIAtxtRelTranslatorPermutationsClass::printParseTree(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{				
		GIAtxtRelTranslatorRulesComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		GIAtxtRelTranslatorNeuralNetworkPropagateOperations.printComponent(parseTreeComponent, layer);

		if(parseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!printParseTree(parseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}

	return result;
}
#endif



#endif



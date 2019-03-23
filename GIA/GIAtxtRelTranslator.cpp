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
 * File Name: GIAtxtRelTranslator.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g11j 01-March-2019
 * Requirements: requires plain text file
 * Description: Textual Relation Translator
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslator.hpp"
#include "XMLparserClass.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3


bool GIAtxtRelTranslatorClass::parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextPlainTXTfileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName)
{
	bool result = true;

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE_EXTRA8
	firstExecution = true;
	#endif
	
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	#ifdef STANFORD_PARSER_USE_POS_TAGS
	cout << "error: performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences{} doesn't support STANFORD_PARSER_USE_POS_TAGS (because the semantic relations word types being written must match those being read [and read can only use feature parser])" << endl;
	#endif
	bool createNewSentences = true;
	if(!GIAnlp.parseNLPparserFeaturesFile(inputTextNLPfeatureXMLfileName, translatorVariables->isQuery, translatorVariables->firstSentenceInList, translatorVariables->NLPfeatureParser, &createNewSentences))
	{
		result = false;
	}
	#else
	//generate empty sentence class list (without GIAfeatures):
	currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	GIAsentence* currentSentenceInList = translatorVariables->firstSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		GIAsentence* newSentence = new GIAsentence();
		newSentence->previous = currentSentenceInList;
		currentSentenceInList->next = newSentence;

		/*
		cout << "currentGIApreprocessorSentenceInList = " << endl;
		GIApreprocessorWordClassObject.printWordList(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList));
		*/
		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
		currentSentenceInList = currentSentenceInList->next;
	}
	#endif

	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	if(!GIApreprocessorWordIdentification.createWordIndexListFromLRPfiles())
	{
		result = false;
	}
	#endif

	//parse extractGIAtxtRelTranslatorRules
	vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes = new vector<GIAtxtRelTranslatorRulesGroupType*>;
	if(!GIAtxtRelTranslatorRules.extractGIAtxtRelTranslatorRulesGroups(GIAtxtRelTranslatorRulesGroupTypes))
	{
		result = false;
	}
	vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers = new vector<XMLparserTag*>;
	if(!GIAtxtRelTranslatorRules.extractGIAtxtRelTranslatorRulesTokens(GIAtxtRelTranslatorRulesTokenLayers))
	{
		result = false;
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	if(!GIAtxtRelTranslatorNeuralNetworkFormation.createGIAtxtRelTranslatorNeuralNetwork(GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes))
	{
		result = false;
	}
	#endif
	
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_DEFINE_WORD_TRANSLATOR_SENTENCE_ENTITY_INDEX_AT_START
	currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);
		for(int w=0; w<sentenceContents->size(); w++)
		{	
			GIApreprocessorPlainTextWord* currentWord = sentenceContents->at(w);
			currentWord->translatorSentenceEntityIndex = GIAtranslatorOperations.convertSentenceContentsIndexToEntityIndex(w);
		}
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	#endif		
	if(!executeTxtRelTranslatorWrapper(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes))
	{
		result = false;
		/*
		cerr << "!executeTxtRelTranslatorWrapper" << endl;
		exit(EXIT_ERROR);
		*/
	}
	
	/*
	currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		cout << "currentGIApreprocessorSentenceInList->sentenceIndexOriginal = " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << endl;
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	*/
		
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	if(!GIAtxtRelTranslatorParser.generateSemanticRelationsFromTxtRelationsWrapperSentences(translatorVariables))
	{
		result = false;
	}	
	/*
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
	string originalInputFileName = "";
	originalInputFileName = inputTextPlainTXTfileName;
	GIAnlp.outputInternalRelationsInRelexFormat(&outputCFFfileName, &originalInputFileName, translatorVariables->firstSentenceInList, translatorVariables->NLPdependencyRelationsParser, translatorVariables->NLPfeatureParser, translatorVariables->NLPexeFolderArray);
	#endif	
	*/
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID	//ie !GIA_TXT_REL_TRANSLATOR_RULES_GIA3
	//parse GIA syntactical relations (NOT currently supported)
	SHAREDvars.setCurrentDirectory(outputFolder);
	bool onlyParseIfCorpusLookupFailed = false;
	if(!GIAnlp.parseNLPparserRelationsFile(inputTextNLPrelationXMLfileName, translatorVariables->isQuery, translatorVariables->firstSentenceInList, translatorVariables->NLPdependencyRelationsParser, translatorVariables->NLPrelexCompatibilityMode, createNewSentences, onlyParseIfCorpusLookupFailed))
	{
		result = false;
	}
	#else
	inputTextNLPrelationXMLfileName = "";				//irrelevant (not used)
	translatorVariables->NLPdependencyRelationsParser = GIA_NLP_PARSER_STANFORD_PARSER;	//irrelevant (not used) - always set to Stanford Parser (as a standard parser type file is used to store GIA2 semantic dependency relations)
	translatorVariables->NLPrelexCompatibilityMode = false; 		//irrelevant (not used) - only used when parsing syntatic dependency relations of a Relex file, and performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences{} does not parse any syntactic dependency relations
	translatorVariables->NLPassumePreCollapsedStanfordRelations = false;	//irrelevant (not used) - only used when executing convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}, and performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences{} does not execute convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}
	#endif
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	//no longer required; 
	translatorVariables->parseGIA2file = true;
	translatorVariables->currentSentenceInList->semanticParserSuccessful = true;
	#endif
	*/
	
	if(!GIAtranslator.parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(translatorVariables, inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, outputCFFfileName))	//inputTextNLPrelationXMLfileName/inputTextNLPfeatureXMLfileName/NLPfeatureParser/NLPdependencyRelationsParser/NLPrelexCompatibilityMode/NLPassumePreCollapsedStanfordRelations not used (relations and features have already been parsed)
	{
		result = false;
	}
	
	return result;
}



bool GIAtxtRelTranslatorClass::executeTxtRelTranslatorWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes)
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
	
	//cout << "GIAtxtRelTranslatorClass::executeTxtRelTranslatorWrapper" << endl;

	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{			
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		cout << "GIAtxtRelTranslatorClass::executeTxtRelTranslator{}: sentence " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << ", sentenceContents = " << GIApreprocessorWordClassObject.printWordListString(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList)) << endl;
		#endif
			
		vector<uint64_t> POSambiguityInfoPermutation;
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);
		
		if(!GIApreprocessorPOStagger.determinePOSambiguityInfoWrapper(sentenceContents, &POSambiguityInfoPermutation))
		{
			result = false;
		}
			
		if(!executeTxtRelTranslator(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, currentGIApreprocessorSentenceInList, &POSambiguityInfoPermutation))
		{
			result = false;
		}
		
		//cout << "2 currentGIApreprocessorSentenceInList->firstParseTreeGroup = " << currentGIApreprocessorSentenceInList->firstParseTreeGroup->groupName << endl;
			
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	if(!transferParseTreePOStypeInferredToWordList(translatorVariables))	//is this required without GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START?
	{
		result = false;
	}
	
	return result;
}

bool GIAtxtRelTranslatorClass::transferParseTreePOStypeInferredToWordList(GIAtranslatorVariablesClass* translatorVariables)
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
			GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup = currentGIApreprocessorSentenceInList->firstParseTreeGroup;
			int layer = GIA_TXT_REL_TRANSLATOR_RULES_LAYER_START;
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES
			cout << "firstParseTreeGroup: groupTypeName = " << firstParseTreeGroup->groupTypeNameBackup << ", groupName = " << firstParseTreeGroup->groupName << endl;
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

bool GIAtxtRelTranslatorClass::transferParseTreePOStypeInferredToWordList(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		
		//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES
		//cout << "transferParseTreePOStypeInferredToWordList currentParseTreeGroup: " <<  currentParseTreeGroup->groupTypeNameBackup << ":" << currentParseTreeGroup->groupName << endl;		
		//#endif
		//cout << "(currentParseTreeGroup->components).size() = " << (currentParseTreeGroup->components).size() << endl;
		//cout << "i = " << i << endl;
		
		GIAtxtRelTranslatorRulesComponent* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
		
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
		GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(currentParseTreeComponent, layer);
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


bool GIAtxtRelTranslatorClass::executeTxtRelTranslator(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, vector<uint64_t>* POSambiguityInfoPermutation)
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
	GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup = NULL;	//not used by GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK (write directly to currentGIApreprocessorSentenceInList->firstParseTreeGroup)
	#else
	GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup = new GIAtxtRelTranslatorRulesGroup();
	currentGIApreprocessorSentenceInList->firstParseTreeGroup = firstParseTreeGroup;
	#endif
		
	int performance = 0; 
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	int minIndexOfMatchesFoundBackupOptimum = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContents);
	vector<vector<uint64_t>*> POSambiguityInfoUnambiguousPermutationArray;
	vector<uint64_t>* POSambiguityInfoUnambiguousPermutationNew = new vector<uint64_t>(POSambiguityInfoPermutation->size());
	POSambiguityInfoUnambiguousPermutationArray.push_back(POSambiguityInfoUnambiguousPermutationNew);
	GIApreprocessorPOStagger.generatePOSambiguityInfoUnambiguousPermutationArray(&POSambiguityInfoUnambiguousPermutationArray, POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, 0);
	int iOptimum = 0;
	#endif
	bool foundParse = false;
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
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_PARSE_ISOLATED_SUBREFERENCE_SETS_OPTIMISED
		}
		#endif
	}
	#endif
	if(!foundParse)
	{
		//result = false;
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(foundParse)
	{

	}
	else
	{
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
		GIAtxtRelTranslatorNeuralNetworkInverse.clearAllWordsAlreadyFoundMatchInComponent(sentenceContents, minIndexOfMatchesFoundBackupOptimum);	//redundant?	
		#endif
		cerr << "GIAtxtRelTranslatorClass::executeTxtRelTranslator{}: Failed to parse sentence " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << ", sentenceContents = " << GIApreprocessorWordClassObject.printWordListString(GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList)) << endl;
		//exit(EXIT_ERROR);
	}
	#endif
	
	return result;
}


#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
bool GIAtxtRelTranslatorClass::generateParseTreeIntroWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets, vector<vector<uint64_t>*>* POSambiguityInfoUnambiguousPermutationArray, int* iOptimum, int* minIndexOfMatchesFoundBackupOptimum)
#else
bool GIAtxtRelTranslatorClass::generateParseTreeIntroWrapper(GIAtranslatorVariablesClass* translatorVariables, vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets, vector<uint64_t>* POSambiguityInfoPermutation)
#endif
{
	bool result = false;
	
	vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);

	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	int performanceMaxPermutationIndex = INT_DEFAULT_VALUE;
	int performanceMax = 0;
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	for(int i=0; i<POSambiguityInfoUnambiguousPermutationArray->size(); i++)
	{
		vector<uint64_t>* POSambiguityInfoPermutationTemp = (*POSambiguityInfoUnambiguousPermutationArray)[i];
		setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, POSambiguityInfoPermutationTemp);

		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		cout << "POSambiguityInfoUnambiguousPermutationArray index = " << i << endl;
		//GIApreprocessorPOStagger.printPOSambiguityInfoPermutationAssumeUnambiguous(POSambiguityInfoPermutationTemp);	//printPOSambiguityInfoPermutation
		#endif
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS3
		cout << "POSambiguityInfoUnambiguousPermutationArray index = " << i << endl;
		GIApreprocessorPOStagger.printPOSambiguityInfoPermutationAssumeUnambiguous(POSambiguityInfoPermutationTemp);		
		#endif

		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
		GIAtxtRelTranslatorRulesGroup* firstParseTreeGroupTemp = NULL;
		#else
		GIAtxtRelTranslatorRulesGroup* firstParseTreeGroupTemp = new GIAtxtRelTranslatorRulesGroup();		
		#endif

		int minIndexOfMatchesFoundBackup2 = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContents);
	#else
	for(int w=0; w<sentenceContents->size(); w++)
	{
		(sentenceContents->at(w))->POSambiguityInfo = (*POSambiguityInfoPermutation)[w];
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	GIAtxtRelTranslatorRulesGroup* firstParseTreeGroupTemp = NULL;
	#else
	GIAtxtRelTranslatorRulesGroup* firstParseTreeGroupTemp = firstParseTreeGroup;
	#endif
	#endif
	
		int performanceTemp = 0;
		bool passedTemp = false;
		
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
		bool parserEnabled = false;
		if(GIAtxtRelTranslatorNeuralNetwork.executeTxtRelTranslatorNeuralNetwork(translatorVariables, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, &firstParseTreeGroupTemp, parseIsolatedSubreferenceSets, parserEnabled, &performanceTemp))
		#else	
		if(GIAtxtRelTranslatorNeuralNetworkInverse.generateParseTreeIntro(GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, firstParseTreeGroupTemp, &performanceTemp, parseIsolatedSubreferenceSets))
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
		if(GIAtxtRelTranslatorNeuralNetworkInverse.updatePerformance(performanceTemp, performance, firstParseTreeGroup, firstParseTreeGroupTemp, passedTemp, minIndexOfMatchesFoundBackupOptimum, sentenceContents, minIndexOfMatchesFoundBackup2, NULL))
		{
			*iOptimum = i;	
		}
	}
	if(result)
	{
		//cout << "performance = " << performance << endl;
		GIAtxtRelTranslatorNeuralNetworkInverse.restoreAllWordsAlreadyFoundMatchInComponent(sentenceContents, *performance);
	}
	#endif
	
	#endif


	return result;
}	

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
bool GIAtxtRelTranslatorClass::setSentenceContentsWordsUnambiguousPOSindex(vector<GIApreprocessorPlainTextWord*>* sentenceContents, vector<uint64_t>* POSambiguityInfoPermutationTemp)
{
	bool result = true;
	
	for(int w=0; w<sentenceContents->size(); w++)
	{
		GIApreprocessorPlainTextWord* contextWord = sentenceContents->at(w);
		unsigned char unambiguousPOSinfoIndex = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
		bool wordAmbiguous = GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(POSambiguityInfoPermutationTemp->at(w), &unambiguousPOSinfoIndex, false);
		contextWord->unambiguousPOSindex = unambiguousPOSinfoIndex;
		//#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS_POS_TYPES
		//cout << "unambiguousPOSinfoIndex = " << int(unambiguousPOSinfoIndex) << endl;
		cout << "GIApreprocessorPOStypeNameArray[unambiguousPOSinfoIndex] = " << GIApreprocessorPOStypeNameArray[unambiguousPOSinfoIndex] << endl;
		#endif
	}

	return result;
}
#endif
	
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
//based on GIAtxtRelTranslatorNeuralNetworkInverseClass::updatePerformanceNeuralNetwork
bool GIAtxtRelTranslatorClass::updatePerformanceNeuralNetwork(const int performanceTemp, int* performance, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIAtxtRelTranslatorRulesGroup* firstParseTreeGroupTemp, const bool passedTemp, const int permutationIndex, int* performanceMaxPermutationIndex)
{
	bool result = false;
	
	if(passedTemp)
	{
		if(performanceTemp > *performance)
		{
			result = true;
			*performance = performanceTemp;
			currentGIApreprocessorSentenceInList->firstParseTreeGroup = firstParseTreeGroupTemp;
			*performanceMaxPermutationIndex = permutationIndex;
			
			//cout << "firstParseTreeGroup->groupName = " << firstParseTreeGroupTemp->groupName << endl;
			//exit(EXIT_ERROR);
		}
	}

	if(!result)
	{
		//delete all subgroups/components recursively in currentParseTreeGroupTemp
		if(firstParseTreeGroupTemp != NULL)
		{
			//deleteAllSubgroupsRecurse(firstParseTreeGroupTemp, 1);	//CHECKTHIS
		}
	}
	
	return result;
}

//based on GIAtxtRelTranslatorNeuralNetworkInverseClass::deleteAllSubgroupsRecurse
//note it assumes subcomponents
bool GIAtxtRelTranslatorClass::deleteAllSubgroupsRecurse(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	if(!currentParseTreeGroup->neuronTraced)
	{
		currentParseTreeGroup->neuronTraced = true;
		
		for(int i=0; i<currentParseTreeGroup->components.size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
			if(currentParseTreeComponent->parseTreeGroupRef != NULL)
			{
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
				GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
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

#endif


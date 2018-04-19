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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f1q 22-February-2018
 * Requirements: requires plain text file
 * Description: Textual relation translator
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslator.hpp"
#include "XMLparserClass.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
vector<XMLparserTag*> GIAtxtRelTranslatorRulesTokenLayers;	//global variable for general access


bool GIAtxtRelTranslatorClass::parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextPlainTXTfileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName)
{
	bool result = true;

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
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	GIAsentence* currentSentenceInList = translatorVariables->firstSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		GIAsentence* newSentence = new GIAsentence();
		newSentence->previous = currentSentenceInList;
		currentSentenceInList->next = newSentence;

		/*
		cout << "currentGIApreprocessorSentenceInList = " << endl;
		GIApreprocessorMultiwordReductionClassObject.printWordList(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP));
		*/
		
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
		currentSentenceInList = currentSentenceInList->next;
	}
	#endif

	#ifdef GIA_PREPROCESSOR_INITIALISE_WORD_INDEX_LIST_FROM_LRP_FILES
	if(!GIApreprocessorMultiwordReduction.createWordIndexListFromLRPfiles())
	{
		result = false;
	}
	#endif
	
	if(!executeTxtRelTranslator(translatorVariables))
	{
		result = false;
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
}


bool GIAtxtRelTranslatorClass::executeTxtRelTranslator(GIAtranslatorVariablesClass* translatorVariables)
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
	
	vector<GIAtxtRelTranslatorRulesGroupType*> GIAtxtRelTranslatorRulesGroupTypes;
	if(!GIAtxtRelTranslatorRules.extractGIAtxtRelTranslatorRulesGroups(&GIAtxtRelTranslatorRulesGroupTypes))
	{
		result = false;
	}
	if(!GIAtxtRelTranslatorRules.extractGIAtxtRelTranslatorRulesTokens(&GIAtxtRelTranslatorRulesTokenLayers))
	{
		result = false;
	}

	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		cout << "GIAtxtRelTranslatorClass::executeTxtRelTranslator{}: sentence " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << ", sentenceContentsLRP = " << GIApreprocessorMultiwordReductionClassObject.printWordListString(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP)) << endl;
		#endif
			
		GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup = new GIAtxtRelTranslatorRulesGroup();
		currentGIApreprocessorSentenceInList->firstParseTreeGroup = firstParseTreeGroup;
		
		vector<unsigned long> POSambiguityInfoPermutation;
		vector<GIApreprocessorWord*>* sentenceContents = &(currentGIApreprocessorSentenceInList->sentenceContentsLRP);
		for(int w=0; w<sentenceContents->size(); w++)
		{
			GIApreprocessorWord* contextWord = sentenceContents->at(w);
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
			cout << "contextWord = " << contextWord->tagName << endl;
			#endif
			bool contextWordPOSisAmbiguous = false;
			bool identifiedEveryWordInDatabasePOSpermutationNOTUSED = true;
			unsigned long contextWordPOSambiguityInfo = GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN;	//default value
			unsigned char contextWordUnambiguousPOSindex = INT_DEFAULT_VALUE;	//GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
			if(!GIApreprocessorPOStagger.determinePOSambiguityInfo(contextWord, &contextWordPOSambiguityInfo, &contextWordPOSisAmbiguous, &contextWordUnambiguousPOSindex, &identifiedEveryWordInDatabasePOSpermutationNOTUSED))
			{
				result = false;
			}

			POSambiguityInfoPermutation.push_back(contextWordPOSambiguityInfo);
		}
			
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int minIndexOfMatchesFoundBackupOptimum = calculateMinIndexOfMatchesFound(sentenceContents);
		vector<vector<unsigned long>*> POSambiguityInfoUnambiguousPermutationArray;
		vector<unsigned long>* POSambiguityInfoUnambiguousPermutationNew = new vector<unsigned long>(POSambiguityInfoPermutation.size());
		POSambiguityInfoUnambiguousPermutationArray.push_back(POSambiguityInfoUnambiguousPermutationNew);
		GIApreprocessorPOStagger.generatePOSambiguityInfoUnambiguousPermutationArray(&POSambiguityInfoUnambiguousPermutationArray, &POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, 0);
		int performance = 0; 
		int performanceMax = 0;
		int iOptimum = 0;
		for(int i=0; i<POSambiguityInfoUnambiguousPermutationArray.size(); i++)
		{
			vector<unsigned long>* POSambiguityInfoPermutationTemp = POSambiguityInfoUnambiguousPermutationArray[i];
			for(int w=0; w<sentenceContents->size(); w++)
			{
				GIApreprocessorWord* contextWord = sentenceContents->at(w);
				unsigned char unambiguousPOSinfoIndex = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
				bool wordAmbiguous = GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(POSambiguityInfoPermutationTemp->at(w), &unambiguousPOSinfoIndex, false);
				contextWord->unambiguousPOSindex = unambiguousPOSinfoIndex;
				//cout << "unambiguousPOSinfoIndex = " << int(unambiguousPOSinfoIndex) << endl;
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
				cout << "GIApreprocessorPOStypeNameArray[unambiguousPOSinfoIndex] = " << GIApreprocessorPOStypeNameArray[unambiguousPOSinfoIndex] << endl;
				#endif
			}

			//debug:
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS	
			cout << "POSambiguityInfoUnambiguousPermutationArray index = " << i << endl;
			/*
			if(GIApreprocessorPOStagger.printPOSambiguityInfoPermutation(POSambiguityInfoPermutationTemp))
			{
				result = false;
			}
			*/
			#endif
			
			GIAtxtRelTranslatorRulesGroup* firstParseTreeGroupTemp = new GIAtxtRelTranslatorRulesGroup();
			
			int minIndexOfMatchesFoundBackup2 = calculateMinIndexOfMatchesFound(sentenceContents);
		#else
			for(int w=0; w<sentenceContents->size(); w++)
			{
				(sentenceContents->at(w))->POSambiguityInfo = POSambiguityInfoPermutation[w];
			}
			GIAtxtRelTranslatorRulesGroup* firstParseTreeGroupTemp = firstParseTreeGroup;
		#endif

			int performanceTemp = 0;
			bool passedTemp = false;
			if(generateParseTreeIntro(&GIAtxtRelTranslatorRulesGroupTypes, sentenceContents, firstParseTreeGroupTemp, &performanceTemp))
			{
				passedTemp =  true;
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
				cout << "passedTemp" << endl;
				#endif
			}
			#ifndef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START	
			else
			{
				result = false;
			}
			#endif


		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START	
			if(updatePerformance(performanceTemp, &performanceMax, &performance, firstParseTreeGroup, firstParseTreeGroupTemp, passedTemp, &minIndexOfMatchesFoundBackupOptimum, sentenceContents, minIndexOfMatchesFoundBackup2, NULL))
			{
				iOptimum = i;	
			}
		}
		if(result)
		{
			restoreAllWordsAlreadyFoundMatchInComponent(sentenceContents, performance);
			
			//restore unambiguousPOSinfoIndex (not current required because unambiguousPOSindex is only used by executeTxtRelTranslator):
			vector<unsigned long>* POSambiguityInfoPermutationTemp = POSambiguityInfoUnambiguousPermutationArray[iOptimum];
			for(int w=0; w<sentenceContents->size(); w++)
			{
				GIApreprocessorWord* contextWord = sentenceContents->at(w);
				unsigned char unambiguousPOSinfoIndex = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
				bool wordAmbiguous = GIApreprocessorPOStaggerDatabase.determinePOSambiguityInfoIsAmbiguous(POSambiguityInfoPermutationTemp->at(w), &unambiguousPOSinfoIndex, false);
				contextWord->unambiguousPOSindex = unambiguousPOSinfoIndex;
				contextWord->wordPOStypeInferred = unambiguousPOSinfoIndex;
				
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
				//cout << "unambiguousPOSinfoIndex = " << int(unambiguousPOSinfoIndex) << endl;
				cout << "GIApreprocessorPOStypeNameArray[unambiguousPOSinfoIndex] = " << GIApreprocessorPOStypeNameArray[unambiguousPOSinfoIndex] << endl;
				#endif
			}
		}
		else
		{
			clearAllWordsAlreadyFoundMatchInComponent(sentenceContents, minIndexOfMatchesFoundBackupOptimum);	
			result = false;
			cerr << "GIAtxtRelTranslatorClass::executeTxtRelTranslator{}: Failed to parse sentence " << currentGIApreprocessorSentenceInList->sentenceIndexOriginal << ", sentenceContentsLRP = " << GIApreprocessorMultiwordReductionClassObject.printWordListString(&(currentGIApreprocessorSentenceInList->sentenceContentsLRP)) << endl;
			exit(EXIT_ERROR);
		}
		#endif
	
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES
	if(!printParseTreeDebug(translatorVariables))
	{
		result = false;
	}
	#endif
	
	return result;
}


bool GIAtxtRelTranslatorClass::generateParseTreeIntro(vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup, int* performance)
{
	bool result = false;
	
	//generate firstParseTreeGroup (firstTxtRelTranslatorRulesGroupInSentence) tree
	int minIndexOfMatchesFoundBackupOptimum = calculateMinIndexOfMatchesFound(sentenceContents);
	
	int performanceMax = 0;
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		cout << "i = " << i << endl;
		#endif
		
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		
		int minIndexOfMatchesFoundBackup2 = calculateMinIndexOfMatchesFound(sentenceContents);

		if((groupType->referenceSetType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_LOGICREFERENCESET) ||
		(groupType->referenceSetType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_REFERENCESET))
		//if(groupType->groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETSOPTIONAL])
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
			cout << "groupType->referenceSetType = " << groupType->referenceSetType << endl;
			#endif

			GIAtxtRelTranslatorRulesGroup* firstParseTreeGroupTemp = new GIAtxtRelTranslatorRulesGroup();
			int performanceTemp = 0;
			bool passedTemp = false;
			int layer = GIA_TXT_REL_TRANSLATOR_RULES_LAYER_START;
			string previousGroupType = "";
			int numberOfConsecutiveTimesPreviousGroupType = 0;
			if(generateParseTreeGroupType(groupType, sentenceContents, firstParseTreeGroupTemp, &performanceTemp, layer, previousGroupType, numberOfConsecutiveTimesPreviousGroupType, NULL))
			{
				result = true;	//at least one group type has been successfully parsed
				passedTemp = true;
				//cout << "passedTemp" << endl;
			}
			updatePerformance(performanceTemp, &performanceMax, performance, firstParseTreeGroup, firstParseTreeGroupTemp, passedTemp, &minIndexOfMatchesFoundBackupOptimum, sentenceContents, minIndexOfMatchesFoundBackup2, NULL);
			
			//cout << "firstParseTreeGroup->groupTypeNameBackup = " << firstParseTreeGroup->groupTypeNameBackup << endl;
			
			//cout << "passedTemp" << endl;
		}
	}
	
	if(result)
	{
		//cout << "performance = " << performance << endl;
		restoreAllWordsAlreadyFoundMatchInComponent(sentenceContents, *performance);
	}
	else
	{
		clearAllWordsAlreadyFoundMatchInComponent(sentenceContents, minIndexOfMatchesFoundBackupOptimum);	//redundant?
	}

	//check parser has reached end of sentence
	int minIndexOfMatchesFound = calculateMinIndexOfMatchesFound(sentenceContents);
	int numberOfTokensInSentence = sentenceContents->size();
	//cout << "minIndexOfMatchesFound = " << minIndexOfMatchesFound << endl;	
	//cout << "numberOfTokensInSentence = " << numberOfTokensInSentence << endl;
	if(minIndexOfMatchesFound == numberOfTokensInSentence-2)
	{
		if(!(sentenceContents->at(numberOfTokensInSentence-1)->tagName == STRING_FULLSTOP))
		{
			result = false;
		}
	}
	else if(minIndexOfMatchesFound == numberOfTokensInSentence-1)
	{
		//probably a query group with an explicit end of sentence punctuation mark (?)
	}
	else
	{
		result = false;
	}
	
	//cout << "result = " << result << endl;
	//exit(EXIT_ERROR);	//DEBUG
	

	return result;
}

bool GIAtxtRelTranslatorClass::generateParseTreeGroupType(GIAtxtRelTranslatorRulesGroupType* groupType, vector<GIApreprocessorWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType, GIAtxtRelTranslatorRulesComponent* previousParseTreeComponent)
{
	bool result = false;
	
	int minIndexOfMatchesFoundBackupOptimum = calculateMinIndexOfMatchesFound(sentenceContentsSubset);

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "groupType->groupTypeName = " << groupType->groupTypeName << endl;
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	cout << "minIndexOfMatchesFoundBackupOptimum = " << minIndexOfMatchesFoundBackupOptimum << endl;
	#endif
	
	int performanceOriginal = *performance;
	int performanceMax = performanceOriginal;
	for(int i=0; i<groupType->groups.size(); i++)
	{
		GIAtxtRelTranslatorRulesGroup* group = (groupType->groups)[i];

		int minIndexOfMatchesFoundBackup2 = calculateMinIndexOfMatchesFound(sentenceContentsSubset);

		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
		//FUTURE: should pass previousWordPOStype from higher level function rather than relying on copied version of currentParseTreeGroup->previousWordPOStype (from group->previousWordPOStype)
		if((group->previousWordPOStype == "") || 
		(minIndexOfMatchesFoundBackup2 >= 0 && 
		(sentenceContentsSubset->at(minIndexOfMatchesFoundBackup2)->wordPOStypeInferred == GIApreprocessorMultiwordReductionClassObject.getPOStypeFromName(group->previousWordPOStype))))
		//OLD: verifyPOStype(sentenceContentsSubset->at(minIndexOfMatchesFoundBackup2), GIApreprocessorMultiwordReductionClassObject.getPOStypeFromName(group->previousWordPOStype))))
		{
		#endif
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "group->groupName = " << group->groupName << endl;
			#endif

			GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupTemp = new GIAtxtRelTranslatorRulesGroup(*group);
			currentParseTreeGroupTemp->groupTypeNameBackup = groupType->groupTypeName;
			currentParseTreeGroupTemp->components.clear();	//CHECKTHIS; added 5 Mar 2018
			int performanceTemp = performanceOriginal;
			bool passedTemp = false;
			if(generateParseTreeGroup(group, sentenceContentsSubset, currentParseTreeGroupTemp, &performanceTemp, layer+1, previousGroupType, numberOfConsecutiveTimesPreviousGroupType))
			{
				result = true;	//at least one group has been successfully parsed
				passedTemp = true;
				//cout << "passedTemp2" << endl;
				//exit(EXIT_ERROR);	//DEBUG
			}
			updatePerformance(performanceTemp, &performanceMax, performance, currentParseTreeGroup, currentParseTreeGroupTemp, passedTemp, &minIndexOfMatchesFoundBackupOptimum, sentenceContentsSubset, minIndexOfMatchesFoundBackup2, previousParseTreeComponent);
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
		}
		#endif				
	}
	
	if(result)
	{
		restoreAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, *performance);
	}
	else
	{
		clearAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, minIndexOfMatchesFoundBackupOptimum);	//redundant?
	}
	
	return result;
}

bool GIAtxtRelTranslatorClass::generateParseTreeGroup(GIAtxtRelTranslatorRulesGroup* group, vector<GIApreprocessorWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType)
{
	bool foundWordMatch = true;
	
	if(!generateRulesGroupTreeComponents(&(group->components), sentenceContentsSubset, currentParseTreeGroup, performance, false, INT_DEFAULT_VALUE, false, layer, previousGroupType, numberOfConsecutiveTimesPreviousGroupType))
	{
		//currentParseTreeGroup->components.clear();	//already done in generateRulesGroupTreeComponents
		foundWordMatch = false;
	}
	
	return foundWordMatch;
}

bool GIAtxtRelTranslatorClass::generateRulesGroupTreeComponents(vector<GIAtxtRelTranslatorRulesComponent*>* components, vector<GIApreprocessorWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int* performance, bool subcomponents, int subcomponentsType, bool subcomponentsOptional, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType)
{
	bool foundWordMatch = true;
	
	int minIndexOfMatchesFoundBackup1 = calculateMinIndexOfMatchesFound(sentenceContentsSubset);

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_OR
	int minIndexOfMatchesFoundBackupOptimum = calculateMinIndexOfMatchesFound(sentenceContentsSubset);
	bool subcomponentsOr = false;
	bool subcomponentsOrFoundMatch = false;	
	if(subcomponents && (subcomponentsType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR))
	{
		subcomponentsOr = true;
	}
	int performanceOriginal = *performance;
	int performanceMax = performanceOriginal; 
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_REPEAT
	bool subcomponentsRepeatFoundAtLeastOneMatch = false;
	bool subcomponentsRepeatStillFindingRepeats = false;
	bool subcomponentsRepeat = false;
	if(subcomponents && (subcomponentsType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
	{
		subcomponentsRepeatStillFindingRepeats = true;
		subcomponentsRepeat = true;
	}
	int repeatIndex = 0;
	while(subcomponentsRepeatStillFindingRepeats || (repeatIndex == 0))
	{
	#endif
		for(int i=0; i<components->size(); i++)
		{
			GIAtxtRelTranslatorRulesComponent* component = components->at(i);
					
			if(foundWordMatch || (subcomponentsOr && !subcomponentsOrFoundMatch))
			{//only continue while foundWordMatch:
			
				int minIndexOfMatchesFoundBackup2 = calculateMinIndexOfMatchesFound(sentenceContentsSubset);
				if(minIndexOfMatchesFoundBackup2 < int(sentenceContentsSubset->size()))
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
					GIAtxtRelTranslatorRules.printComponent(component, layer);
					#endif
					
					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
					if(component->missing)
					{
						//skipMissingComponent = true;
						GIApreprocessorWord* currentWord = sentenceContentsSubset->at(minIndexOfMatchesFoundBackup2+1);
						if(findStringMatch(component, currentWord))
						{
							foundWordMatch = false;	//component should be missing but was found
						}
					}
					else
					{
					#endif

						GIAtxtRelTranslatorRulesComponent* newParseComponent = new GIAtxtRelTranslatorRulesComponent(*component);	//copy rules component
						//newParseComponent->groupTypeRef = NULL;	
						//newParseComponent->groupRef = NULL;
						//newParseComponent->subComponents.clear();	//NB this is necessary as newParseComponent is a copied version of the original rules object
						currentParseTreeGroup->components.push_back(newParseComponent);



						if((component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR) ||
						(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_REPEAT))
						{
							//NB subcomponents are added to currentParseTreeGroup in the same way (ie at same level) as ordinary components - therefore delete the artificial component representing the subcomponents section:
							currentParseTreeGroup->components.pop_back();
							delete newParseComponent;

							int performanceTemp = *performance;
							if(generateRulesGroupTreeComponents(&(component->subComponents), sentenceContentsSubset, currentParseTreeGroup, &performanceTemp, true, component->componentType, component->optional, layer, previousGroupType, numberOfConsecutiveTimesPreviousGroupType))
							{
								*performance = performanceTemp;
							}
							else
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
								if(!(component->optional))
								{
								#endif
									foundWordMatch = false;
								#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
								}
								#endif

								//cout << "clearAllWordsAlreadyFoundMatchInComponent, minIndexOfMatchesFoundBackup2 = " << minIndexOfMatchesFoundBackup2 << endl;
								clearAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, minIndexOfMatchesFoundBackup2);	//shouldn't be necessary as this is done within generateRulesGroupTreeComponents

							}
						}
						else
						{
							int performanceTemp = 0;
							#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_OR
							if(subcomponentsOr)
							{
								performanceTemp = performanceOriginal;
							}
							else
							{
							#endif
								performanceTemp = *performance;
							#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_OR
							}
							#endif
							//cout << "AAAA1" << endl;
							if(generateRulesGroupTreeComponent(component, sentenceContentsSubset, newParseComponent, &performanceTemp, layer, previousGroupType, numberOfConsecutiveTimesPreviousGroupType))
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_OR
								if(subcomponentsOr)
								{
									bool passedTemp = true;
									GIAtxtRelTranslatorRulesGroup* tempGroup = new GIAtxtRelTranslatorRulesGroup();
									if(updatePerformance(performanceTemp, &performanceMax, performance, tempGroup, tempGroup, passedTemp, &minIndexOfMatchesFoundBackupOptimum, sentenceContentsSubset, minIndexOfMatchesFoundBackup2, NULL))
									{
										subcomponentsOrFoundMatch = true;
										/*
										GIAtxtRelTranslatorRules.printComponent(component, layer);
										GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
										cout << "updatePerformance{} performanceMax = " << performanceMax << endl;
										*/
									}
									else
									{
										currentParseTreeGroup->components.pop_back();	//remove last component added
										delete newParseComponent;
									}
								}
								else
								{
								#endif
									*performance = performanceTemp;
								#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_OR
								}
								#endif
								//cout << "foundWordMatch = " << foundWordMatch << endl;
							}
							else
							{
								#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_OR
								if(subcomponentsOr)
								{
									/*
									Redundant:
									bool passedTemp = false;
									GIAtxtRelTranslatorRulesGroup* tempGroup = new GIAtxtRelTranslatorRulesGroup();
									updatePerformance(performanceTemp, &performanceMax, performance, &tempGroup, &tempGroup, passedTemp, &minIndexOfMatchesFoundBackupOptimum, sentenceContentsSubset, minIndexOfMatchesFoundBackup2, NULL);
									*/
								}
								else 
								#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_REPEAT
								if(subcomponentsRepeat)
								{
									subcomponentsRepeatStillFindingRepeats = false;
									if(subcomponentsRepeatFoundAtLeastOneMatch)
									{

									}
									#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
									else if(component->optional || subcomponentsOptional)
									{

									}
									#endif
									else
									{
										foundWordMatch = false;
									}
								}
								else
								{
								#endif
									#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
									if(component->optional || subcomponentsOptional)
									{
										/*
										GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
										cout << "(component->optional) && !generateRulesGroupTreeComponent" << endl;
										GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
										cout << "minIndexOfMatchesFoundBackup2 = " << minIndexOfMatchesFoundBackup2 << endl;
										*/
									}
									else
									{
									#endif
										foundWordMatch = false;
									#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
									}
									#endif
								#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_REPEAT
								}
								#endif

								//cout << "clearAllWordsAlreadyFoundMatchInComponent, minIndexOfMatchesFoundBackup2 = " << minIndexOfMatchesFoundBackup2 << endl;
								clearAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, minIndexOfMatchesFoundBackup2);
								currentParseTreeGroup->components.pop_back();
								delete newParseComponent;
							}
						}
					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
					}
					#endif
				}
				else
				{
					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
					if(!(component->optional))
					{
					#endif
						//reached end of sentence but still finding components
						foundWordMatch = false;
					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
					}
					#endif
				}
			}
		}
		
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_OR
		if(subcomponentsOr)
		{
			if(subcomponentsOrFoundMatch)
			{
				restoreAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, *performance);
				//clearAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, minIndexOfMatchesFoundBackupOptimum);
			}
			else
			{
				foundWordMatch = false;	
			}
		}
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_REPEAT
		if(foundWordMatch)
		{
			subcomponentsRepeatFoundAtLeastOneMatch = true;
		}
		repeatIndex++;
		#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_REPEAT
	}
	#endif
	
	if(!foundWordMatch)
	{
		currentParseTreeGroup->components.clear();
		clearAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, minIndexOfMatchesFoundBackup1);
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "FAIL" << endl;
		#endif

	}
	else
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		cout << "PASS" << endl;
		#endif
	}
	
	return foundWordMatch;
}

bool GIAtxtRelTranslatorClass::generateRulesGroupTreeComponent(GIAtxtRelTranslatorRulesComponent* component, vector<GIApreprocessorWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesComponent* currentParseTreeComponent, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType)
{
	bool foundWordMatch = false;

	/*
	Optimised method:
		Record groups That don't have a string or subReferenceSetsPart
			actionIndirectObject
			conditionObject

		For every componentType string in group
			find every possible reference to this in the current sentenceSubset
		For every componentType group with groupType subReferenceSetsPart in group 
			find every possible reference to this in the current sentenceSubset
		For every legal permutation of the above, parse the remaining componentType group with groupType subReferenceSet/referenceSet/logicReferenceSet

		If group doesn't have a string or subReferenceSetsPart; 
			then for every possible permutation, parse the remaining componentType group with groupType subReferenceSet/referenceSet/logicReferenceSet

		NB Treat subcomponents or/repeat as unknowns
	*/


	if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
	{
		int minIndexOfMatchesFound = calculateMinIndexOfMatchesFound(sentenceContentsSubset);
		int w = minIndexOfMatchesFound+1;
		if(w < sentenceContentsSubset->size())
		{	
			GIApreprocessorWord* currentWord = sentenceContentsSubset->at(w);

			//cout << "minIndexOfMatchesFound = " << minIndexOfMatchesFound << endl;
			//cout << "currentWord = " << currentWord->tagName << endl;
			if(findStringMatch(component, currentWord))
			{
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
				cout << "findStringMatch: currentWord = " << currentWord->tagName << endl;
				#endif
				foundWordMatch = true;
				currentWord->alreadyFoundMatch = true;
				currentWord->translatorSentenceEntityIndex = w;	//NB alternatively, translatorSentenceEntityIndex could be defined for each word at the very start of executeTxtRelTranslator()
				*performance = *performance + 1;
				currentParseTreeComponent->candidateStringMatch = currentWord;
			}
		}
	}
	else if(component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
	{
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_RESTRICT_RECURSION
		bool pass = false;
		if(component->groupTypeRef != NULL)
		{
			if(component->groupRef == NULL)
			{
				if(component->groupTypeRefName == previousGroupType)
				{
					if(numberOfConsecutiveTimesPreviousGroupType < GIA_TXT_REL_TRANSLATOR_GROUP_TYPE_MAX_NUMBER_CONSECUTIVE_LAYERS-1)
					{
						pass = true;
						numberOfConsecutiveTimesPreviousGroupType++;
					}
					else
					{
						//cout << "false pass 1" << endl;
					}
				}
				else
				{
					pass = true;
					numberOfConsecutiveTimesPreviousGroupType = 0;
				}
			}
			else
			{
				//always process groups being specifically referenced
				pass = true;
				numberOfConsecutiveTimesPreviousGroupType = 0;				
			}
			
			//special case for logicReferenceSets - check referenceSetType instead of groupType; as logicReferenceSets groupTypes include recursive generalised (not groupName specific) referencing: <groupType groupTypeName="logicReferenceSets" -> <groupType groupTypeName="logicReferenceSetsOptional" ->  <groupType groupTypeName="logicReferenceSets"
			if((component->groupTypeRef->referenceSetType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_REFERENCE_SET_TYPE_LOGICREFERENCESET) && (layer > GIA_TXT_REL_TRANSLATOR_GROUP_TYPE_MAX_NUMBER_CONSECUTIVE_LAYERS_LOGIC_REFERENCES))
			{
				//cout << "false pass 2" << endl;
				pass = false;
			}		
		}
		else
		{
			cerr << "GIAtxtRelTranslatorClass::generateRulesGroupTreeComponent{} error: (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP) && (component->groupTypeRef == NULL)" << endl;
			cout << "component->groupTypeRefName = " << component->groupTypeRefName << endl;
			cout << "component->groupRefName = " << component->groupRefName << endl;
			cout << "previousGroupType = " << previousGroupType << endl;
			exit(EXIT_ERROR);	
		}
		
		if(pass)
		{
		#endif
			if(component->groupRef != NULL)
			{
				//cout << "(component->groupRef != NULL)" << endl;
				GIAtxtRelTranslatorRulesGroup* newParseGroup = new GIAtxtRelTranslatorRulesGroup(*(component->groupRef));	//copy group component
				newParseGroup->components.clear();	//CHECKTHIS; added 5 Mar 2018
				currentParseTreeComponent->parseTreeGroupRef = newParseGroup;
				//currentParseTreeComponent->groupRefName = newParseGroup->groupName;	//added 17 March 2018

				if(generateParseTreeGroup(currentParseTreeComponent->groupRef, sentenceContentsSubset, newParseGroup, performance, layer+1, component->groupTypeRefName, numberOfConsecutiveTimesPreviousGroupType))
				{
					foundWordMatch = true;
				}
			}
			else if(component->groupTypeRef != NULL)
			{
				GIAtxtRelTranslatorRulesGroup* newParseGroup = new GIAtxtRelTranslatorRulesGroup();	//create new cgroup component
				currentParseTreeComponent->parseTreeGroupRef = newParseGroup;

				if(generateParseTreeGroupType(currentParseTreeComponent->groupTypeRef, sentenceContentsSubset, newParseGroup, performance, layer+1, component->groupTypeRefName, numberOfConsecutiveTimesPreviousGroupType, currentParseTreeComponent))
				{
					foundWordMatch = true;
				}	
			}
			else
			{
				cerr << "GIAtxtRelTranslatorClass::generateRulesGroupTreeComponent{} error: (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP) && (component->groupTypeRef/groupRef == NULL)" << endl;
				exit(EXIT_ERROR);
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_RESTRICT_RECURSION
		}
		else
		{

		}
		#endif
	} 
	else
	{
		cerr << "GIAtxtRelTranslatorClass::generateRulesGroupTreeComponent{} error: component->componentType illdefined" << endl;
		exit(EXIT_ERROR);
	}
	
	return foundWordMatch;
}

bool GIAtxtRelTranslatorClass::findStringMatch(GIAtxtRelTranslatorRulesComponent* component, GIApreprocessorWord* currentWord)
{
	bool foundWordMatchTemp = false;
	if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
	{
		string wordPOStypeName = component->wordPOStype;
		int wordPOStype = GIApreprocessorMultiwordReductionClassObject.getPOStypeFromName(wordPOStypeName);
		
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		if(currentWord->unambiguousPOSindex == GIA_PREPROCESSOR_POS_TYPE_UNDEFINED)
		#else
		if(currentWord->POSambiguityInfo == GIA_PREPROCESSOR_POS_TAGGER_POS_AMBIGUITY_INFO_UNKNOWN)
		#endif
		{
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_WILDCARDS
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
			if(!(component->optional))
			{
			#endif
				foundWordMatchTemp = true;
				currentWord->wordPOStypeInferred = wordPOStype;
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
			}
			#endif
			#elif defined GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
			if(wordPOStype == GIA_PREPROCESSOR_POS_TYPE_NOUN)
			{
				foundWordMatchTemp = true;
				currentWord->wordPOStypeInferred = wordPOStype;
			}
			#endif
		}
		else
		{
		#endif
			//cout << "wordPOStypeName = " << wordPOStypeName << endl;
			//cout << "wordPOStype = " << wordPOStype << endl;
			if(verifyPOStype(currentWord, wordPOStype))
			{
				foundWordMatchTemp = true;
				currentWord->wordPOStypeInferred = wordPOStype;
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES
		}
		#endif
	}
	else if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_EXPLICIT)
	{
		bool foundExplicitWord = false;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_STRING_EXPLICIT_CASE_INSENSITIVE
		if(SHAREDvars.convertStringToLowerCase(&(currentWord->tagName)) == SHAREDvars.convertStringToLowerCase(&(component->word)))	//convertStringToLowerCase required to compensate for case insensitivity
		{
			foundExplicitWord = true;
		}
		#else
		if(SHAREDvars.convertStringToLowerCase(&(currentWord->tagName)) == component->word)	//convertStringToLowerCase required to compensate for case insensitivity
		{
			foundExplicitWord = true;
		}
		if(currentWord->tagName == component->word)
		{
			foundExplicitWord = true;
		}
		#endif
		
		if(foundExplicitWord)
		{
			foundWordMatchTemp = true;
			currentWord->wordPOStypeInferred = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//component->wordPOStype;  -need to add wordPOStype attribute to <component>? Not required at present as all stringType explicit tagged words are currently disgarded by semantic network
		}
	}
	else if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
	{
		if(GIAtxtRelTranslatorRules.isClassTagWrapper(currentWord->tagName, component->tokenLayer, component->tokenClass, component->tokenType, &GIAtxtRelTranslatorRulesTokenLayers))
		{
			foundWordMatchTemp = true;
			int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
			if(SHAREDvars.textInTextArray(component->tokenClass, GIApreprocessorPOStypeNameArray, GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES, &wordPOStype))
			{
				currentWord->wordPOStypeInferred = wordPOStype;
			}
			else
			{
				cerr << "GIAtxtRelTranslatorClass::generateRulesGroupTreeComponent{} error: !if(textInTextArray(component->tokenClass, GIApreprocessorPOStypeNameArray, GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES, &wordPOStype)" << endl;
				cerr << "currentWord->tagName = " << currentWord->tagName << endl;
				cerr << "component->tokenLayer = " << component->tokenLayer << endl;
				cerr << "component->tokenClass = " << component->tokenClass << endl;
				cerr << "component->tokenType = " << component->tokenType << endl;
				exit(EXIT_ERROR);
			}
		}
	}
	else
	{
		cerr << "GIAtxtRelTranslatorClass::generateRulesGroupTreeComponent{} error: component->stringType illegal" << endl;
		exit(EXIT_ERROR);
	}
	return foundWordMatchTemp;
}

bool GIAtxtRelTranslatorClass::verifyPOStype(GIApreprocessorWord* currentWord, unsigned char wordPOStype)
{
	bool result = false;
							
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
	if(currentWord->unambiguousPOSindex == wordPOStype)
	{
		result = true;
	}
	#else
	if(SHAREDvars.getBitValue(currentWord->POSambiguityInfo, wordPOStype))
	{
		result = true;
	}
	#endif
	
	return result;
}


							
bool GIAtxtRelTranslatorClass::updatePerformance(const int performanceTemp, int* performanceMax, int* performance, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroupTemp, const bool passedTemp, int* minIndexOfMatchesFoundBackupOptimum, vector<GIApreprocessorWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup, GIAtxtRelTranslatorRulesComponent* previousParseTreeComponent)
{
	bool result = false;
	
	if(passedTemp)
	{
		if(performanceTemp > *performanceMax)
		{
			result = true;
			*performanceMax = performanceTemp;
			*performance = performanceTemp;
			GIAtxtRelTranslatorRulesGroupClassObject.copyParseGroupInfo(currentParseTreeGroupTemp, currentParseTreeGroup);
			
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_RECORD_COMPONENT_PARSED_GROUP_REF_NAME_FOR_PRINT
			if(previousParseTreeComponent != NULL)
			{
				previousParseTreeComponent->groupRefName = currentParseTreeGroupTemp->groupName;
				//cout << "previousParseTreeComponent->groupRefName = " << previousParseTreeComponent->groupRefName << endl;
				//exit(EXIT_ERROR);
			}
			#endif
		}
		
		*minIndexOfMatchesFoundBackupOptimum = calculateMinIndexOfMatchesFound(sentenceContentsSubset);
	}
	clearAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, minIndexOfMatchesFoundBackup);
	
	delete currentParseTreeGroupTemp;
	
	return result;
}

int GIAtxtRelTranslatorClass::calculateMinIndexOfMatchesFound(vector<GIApreprocessorWord*>* sentenceContentsSubset)
{	
	int minIndexOfMatchesFound = -1;
	for(int w=0; w<sentenceContentsSubset->size(); w++)
	{
		GIApreprocessorWord* currentWord = sentenceContentsSubset->at(w);
		if(currentWord->alreadyFoundMatch)
		{	
			minIndexOfMatchesFound = w;
		}
	}
	
	return minIndexOfMatchesFound;
}


void GIAtxtRelTranslatorClass::clearAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup)
{	
	for(int w=0; w<sentenceContentsSubset->size(); w++)
	{
		GIApreprocessorWord* currentWord = sentenceContentsSubset->at(w);
		if(w > minIndexOfMatchesFoundBackup)
		{
			currentWord->alreadyFoundMatch = false;
		}
	}
}

void GIAtxtRelTranslatorClass::restoreAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundNew)
{	
	//cout << "restoreAllWordsAlreadyFoundMatchInComponent: minIndexOfMatchesFoundNew = " << minIndexOfMatchesFoundNew << endl;
	//exit(EXIT_ERROR);
	
	for(int w=0; w<sentenceContentsSubset->size(); w++)
	{
		GIApreprocessorWord* currentWord = sentenceContentsSubset->at(w);
		if(w < minIndexOfMatchesFoundNew)
		{
			currentWord->alreadyFoundMatch = true;
		}
	}
}


bool GIAtxtRelTranslatorClass::printParseTreeDebug(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	cout << "printParseTreeDebug: " << endl;
			
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		vector<GIApreprocessorWord*>* sentenceContents = &(currentGIApreprocessorSentenceInList->sentenceContentsLRP);
		for(int w=0; w<sentenceContents->size(); w++)
		{
			GIApreprocessorWord* contextWord = sentenceContents->at(w);
			cout << "GIApreprocessorPOStypeNameArray[contextWord->wordPOStypeInferred] = " << GIApreprocessorPOStypeNameArray[contextWord->wordPOStypeInferred] << endl;
		}

		GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup = currentGIApreprocessorSentenceInList->firstParseTreeGroup;
		int layer = GIA_TXT_REL_TRANSLATOR_RULES_LAYER_START;
		cout << "firstParseTreeGroup: groupTypeName = " << firstParseTreeGroup->groupTypeNameBackup << ", groupName = " << firstParseTreeGroup->groupName << endl;
		if(!printParseTreeDebug(firstParseTreeGroup, layer))
		{
			result = false;
		}

		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	cout << "\n" << endl;

	return result;
}

bool GIAtxtRelTranslatorClass::printParseTreeDebug(GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
		GIAtxtRelTranslatorRules.printComponent(currentParseTreeComponent, layer);
		if(currentParseTreeComponent->parseTreeGroupRef != NULL)
		{
			if(!printParseTreeDebug(currentParseTreeComponent->parseTreeGroupRef, layer+1))
			{
				result = false;
			}
		}
	}
	
	return result;
}

#endif


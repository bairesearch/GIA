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
 * File Name: GIAtxtRelTranslatorNeuralNetworkInverse.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3i2b 27-May-2019
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Neural Network Inverse
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorNeuralNetworkInverse.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
#ifdef GIA_TXT_REL_TRANSLATOR_INVERSE_NEURAL_NETWORK

bool GIAtxtRelTranslatorNeuralNetworkInverseClass::generateParseTreeIntro(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentenceContents, GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup, int* performance, const bool parseIsolatedSubreferenceSets)
{
	bool result = false;

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUERIES
	bool isQuery = GIAtxtRelTranslatorRulesGroupClassObject.determineIsQuery(sentenceContents);
	#endif

	//generate firstParseTreeGroup (firstTxtRelTranslatorRulesGroupInSentence) tree
	int minIndexOfMatchesFoundBackupOptimum = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContents);
	
	for(int i=0; i<GIAtxtRelTranslatorRulesGroupTypes->size(); i++)
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		cout << "i = " << i << endl;
		#endif
		
		GIAtxtRelTranslatorRulesGroupType* groupType = GIAtxtRelTranslatorRulesGroupTypes->at(i);
		
		int minIndexOfMatchesFoundBackup2 = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContents);

		bool passGroupTests = GIAtxtRelTranslatorRulesGroupClassObject.isTopLevelGroupType(groupType->groupTypeName, groupType->referenceSetType, isQuery, parseIsolatedSubreferenceSets);	
		if(passGroupTests)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
			cout << "groupType->referenceSetType = " << groupType->referenceSetType << endl;
			#endif

			GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroupTemp = new GIAtxtRelTranslatorRulesGroupParseTree();
			int performanceTemp = 0;
			bool passedTemp = false;
			int layer = GIA_TXT_REL_TRANSLATOR_RULES_LAYER_START;
			string previousGroupType = "";
			int numberOfConsecutiveTimesPreviousGroupType = 0;
			if(generateParseTreeGroupType(GIAtxtRelTranslatorRulesTokenLayers, groupType, sentenceContents, firstParseTreeGroupTemp, &performanceTemp, layer, previousGroupType, numberOfConsecutiveTimesPreviousGroupType, NULL))
			{
				result = true;	//at least one group type has been successfully parsed
				passedTemp = true;
				//cout << "passedTemp" << endl;
			}
			updatePerformance(performanceTemp, performance, firstParseTreeGroup, firstParseTreeGroupTemp, passedTemp, &minIndexOfMatchesFoundBackupOptimum, sentenceContents, minIndexOfMatchesFoundBackup2, NULL);
			
			//cout << "firstParseTreeGroup->groupTypeName = " << firstParseTreeGroup->groupTypeName << endl;
			
			//cout << "passedTemp" << endl;
		}
	}
	
	if(result)
	{
		//cout << "performance = " << performance << endl;
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS3
		cout << "GIAtxtRelTranslatorNeuralNetworkInverseClass::generateParseTreeIntro FOUND result" << endl;
		#endif
		//exit(EXIT_ERROR);
		
		restoreAllWordsAlreadyFoundMatchInComponent(sentenceContents, *performance);
	}
	else
	{
		clearAllWordsAlreadyFoundMatchInComponent(sentenceContents, minIndexOfMatchesFoundBackupOptimum);	//redundant?
	}

	//check parser has reached end of sentence
	int minIndexOfMatchesFound = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContents);
	int numberOfTokensInSentence = sentenceContents->size();
	//cout << "minIndexOfMatchesFound = " << minIndexOfMatchesFound << endl;	
	//cout << "numberOfTokensInSentence = " << numberOfTokensInSentence << endl;
	if(minIndexOfMatchesFound == numberOfTokensInSentence-1)
	{
		//a sentence with an explicit end of sentence punctuation mark (./?)
	}
	else
	{
		result = false;
	}
	
	//cout << "result = " << result << endl;
	//exit(EXIT_ERROR);	//DEBUG
	

	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkInverseClass::generateParseTreeGroupType(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupType* groupType, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType, GIAtxtRelTranslatorRulesComponentParseTree* previousParseTreeComponent)
{
	bool result = false;
	
	int minIndexOfMatchesFoundBackupOptimum = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContentsSubset);

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
	cout << "groupType->groupTypeName = " << groupType->groupTypeName << endl;
	GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
	cout << "minIndexOfMatchesFoundBackupOptimum = " << minIndexOfMatchesFoundBackupOptimum << endl;
	#endif

	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS3
	if(groupType->groupTypeName == "logicReferenceSets")
	{
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "start groupType->groupTypeName == logicReferenceSets" << endl;
	}
	if(groupType->groupTypeName == "referenceSets")
	{
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "start groupType->groupTypeName == referenceSets" << endl;
	}
	#endif
				
	int performanceOriginal = *performance;
	for(int i=0; i<groupType->groups.size(); i++)
	{
		GIAtxtRelTranslatorRulesGroupNeuralNetwork* group = (groupType->groups)[i];

		int minIndexOfMatchesFoundBackup2 = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContentsSubset);

		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "group->groupName = " << group->groupName << endl;
		#endif

		GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp = GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkGroupToParseTreeGroupNew(group);	//new GIAtxtRelTranslatorRulesGroupParseTree(*GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkGroupToParseTreeGroup(group));	
		#ifndef GIA_TXT_REL_TRANSLATOR_RULES_DEFINE_GROUP_TYPE_BACKUP_AT_START
		currentParseTreeGroupTemp->groupTypeName = groupType->groupTypeName;
		#endif
		currentParseTreeGroupTemp->components.clear();	//added 5 Mar 2018
		int performanceTemp = performanceOriginal;
		bool passedTemp = false;
		if(generateParseTreeGroup(GIAtxtRelTranslatorRulesTokenLayers, group, sentenceContentsSubset, currentParseTreeGroupTemp, &performanceTemp, layer+1, previousGroupType, numberOfConsecutiveTimesPreviousGroupType))
		{
			result = true;	//at least one group has been successfully parsed
			passedTemp = true;
			//cout << "passedTemp2" << endl;
			//exit(EXIT_ERROR);	//DEBUG
		}
		updatePerformance(performanceTemp, performance, currentParseTreeGroup, currentParseTreeGroupTemp, passedTemp, &minIndexOfMatchesFoundBackupOptimum, sentenceContentsSubset, minIndexOfMatchesFoundBackup2, previousParseTreeComponent);
	}
	
	if(result)
	{
		restoreAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, *performance);
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS3
		//cout << "layer = " << layer << endl;
		if(groupType->groupTypeName == "logicReferenceSets")
		{
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
			cout << "end pass groupType->groupTypeName == logicReferenceSets" << endl;
		}
		if(groupType->groupTypeName == "referenceSets")
		{
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
			cout << "end pass groupType->groupTypeName == referenceSets" << endl;
		}
		#endif
	}
	else
	{
		clearAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, minIndexOfMatchesFoundBackupOptimum);	//redundant?
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS3
		if(groupType->groupTypeName == "logicReferenceSets")
		{
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
			cout << "end fail groupType->groupTypeName == logicReferenceSets" << endl;
		}
		if(groupType->groupTypeName == "referenceSets")
		{
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
			cout << "end fail groupType->groupTypeName == referenceSets" << endl;
		}
		#endif
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkInverseClass::generateParseTreeGroup(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupNeuralNetwork* group, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType)
{
	bool foundWordMatch = true;

	int minIndexOfMatchesFoundBackup2 = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContentsSubset);

	bool pass = true;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_PREVIOUS_WORD_POS_TYPE
	//FUTURE: should pass previousWordPOStype from higher level function rather than relying on copied version of currentParseTreeGroup->previousWordPOStype (from group->previousWordPOStype)
	if(group->previousWordPOStype != "")
	{
		pass = false;
		if((minIndexOfMatchesFoundBackup2 >= 0 && (sentenceContentsSubset->at(minIndexOfMatchesFoundBackup2)->wordPOStypeInferred == GIApreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype))))
		//OLD: verifyPOStype(sentenceContentsSubset->at(minIndexOfMatchesFoundBackup2), GIApreprocessorWordClassObject.getPOStypeFromName(group->previousWordPOStype))))
		{
			pass = true;
		}
	}
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_GROUP_EXISTS_PRECEEDING_WORD_POS_TYPE
	if(group->existsPreceedingWordPOStype != "")
	{
		pass = false;
		for(int j=0; j<=minIndexOfMatchesFoundBackup2; j++)
		{
			if(sentenceContentsSubset->at(j)->wordPOStypeInferred == GIApreprocessorWordClassObject.getPOStypeFromName(group->existsPreceedingWordPOStype))
			{
				//cout << "group->existsPreceedingWordPOStype = " << group->existsPreceedingWordPOStype << endl;
				pass = true;
			}
		}
	}
	#endif
	if(pass)
	{	
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS3
		if(group->groupName == "referenceSetsOrLogicReferenceSets")
		{
			GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
			cout << "group->groupName == referenceSetsOrLogicReferenceSets" << endl;
		}
		#endif
	
		if(!generateRulesGroupTreeComponents(GIAtxtRelTranslatorRulesTokenLayers, &(group->components), sentenceContentsSubset, currentParseTreeGroup, performance, false, INT_DEFAULT_VALUE, false, layer, previousGroupType, numberOfConsecutiveTimesPreviousGroupType))
		{
			//currentParseTreeGroup->components.clear();	//already done in generateRulesGroupTreeComponents
			foundWordMatch = false;
		}
	}
	else
	{
		foundWordMatch = false;
	}
			
	return foundWordMatch;
}

bool GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponents(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, vector<GIAtxtRelTranslatorRulesComponentNeuralNetwork*>* components, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int* performance, bool subcomponents, int subcomponentsType, bool subcomponentsOptional, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType)
{
	bool foundWordMatch = true;
	
	int minIndexOfMatchesFoundBackup1 = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContentsSubset);

	//cout << "layer = " << layer << endl;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_OR
	int minIndexOfMatchesFoundBackupOptimum = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContentsSubset);
	bool subcomponentsOr = false;
	bool subcomponentsOrFoundMatch = false;	
	if(subcomponents && (subcomponentsType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_OR))
	{
		subcomponentsOr = true;
	}
	int performanceOriginal = *performance;
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
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_REPEAT_IGNORE_CONSECUTIVE_PLURAL_NOUNS
	bool subcomponentsRepeatLastMatchWasNounPluralVariant = false;
	#endif
	int repeatIndex = 0;
	while(subcomponentsRepeatStillFindingRepeats || (repeatIndex == 0))
	{
	#endif
		for(int i=0; i<components->size(); i++)
		{
			GIAtxtRelTranslatorRulesComponentNeuralNetwork* component = components->at(i);
					
			if(foundWordMatch || (subcomponentsOr && !subcomponentsOrFoundMatch))
			{//only continue while foundWordMatch:
			
				int minIndexOfMatchesFoundBackup2 = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContentsSubset);
				vector<GIApreprocessorPlainTextWord*> sentenceContentsBackup2;
				
				if(minIndexOfMatchesFoundBackup2 < int(sentenceContentsSubset->size()))
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
					GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(component, layer);
					#endif
					
					bool passRepeatChecks = true;
					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_REPEAT_IGNORE_CONSECUTIVE_PLURAL_NOUNS
					if(subcomponentsRepeatFoundAtLeastOneMatch)
					{
						if((component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING) && (component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS))
						{
							if(minIndexOfMatchesFoundBackup2 < int(sentenceContentsSubset->size()-1))
							{
								GIApreprocessorPlainTextWord* currentWord = sentenceContentsSubset->at(minIndexOfMatchesFoundBackup2+1);
								GIApreprocessorPlainTextWord* previousWord = sentenceContentsSubset->at(minIndexOfMatchesFoundBackup2);
								if(verifyPOStype(currentWord, GIA_PREPROCESSOR_POS_TYPE_NOUN) && verifyPOStype(previousWord, GIA_PREPROCESSOR_POS_TYPE_NOUN))
								{
									#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_REPEAT_IGNORE_CONSECUTIVE_PLURAL_NOUNS_DETECT_IRREGULAR_NOUN_FORMS
									if(((currentWord->wordNounVariantGrammaticalTenseForm == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_PLURAL) || (currentWord->wordNounVariantGrammaticalTenseForm == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_SINGULAR_OR_PLURAL)) && 
									((previousWord->wordNounVariantGrammaticalTenseForm == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_PLURAL) || (previousWord->wordNounVariantGrammaticalTenseForm == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_SINGULAR_OR_PLURAL)))
									#else
									if((currentWord->wordNounVariantGrammaticalTenseForm == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_PLURAL) && (previousWord->wordNounVariantGrammaticalTenseForm == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_PLURAL))
									#endif
									{
										subcomponentsRepeatStillFindingRepeats = false;
										passRepeatChecks = false;
										//cout << "GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_REPEAT_IGNORE_CONSECUTIVE_PLURAL_NOUNS; currentWord = " << currentWord->tagName << endl;
									}
								}
							}
						}
					}
					#endif
					if(passRepeatChecks)
					{
						#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
						if(component->missing)
						{
							//skipMissingComponent = true;
							#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
							GIAtxtRelTranslatorRulesComponentParseTree* newParseComponent = GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkComponentToParseTreeComponentNew(component);	//new GIAtxtRelTranslatorRulesComponentParseTree(*GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkComponentToParseTreeComponent(component));	//copy rules component
							forwardNounVerbVariantRequirementsGroupToComponent(currentParseTreeGroup, newParseComponent);
							#else
							GIAtxtRelTranslatorRulesComponentParseTree* newParseComponent = NULL;
							#endif
							
							GIApreprocessorPlainTextWord* currentWord = sentenceContentsSubset->at(minIndexOfMatchesFoundBackup2+1);
							if(findStringMatch(GIAtxtRelTranslatorRulesTokenLayers, component, currentWord, newParseComponent))
							{
								foundWordMatch = false;	//component should be missing but was found
							}
							
							#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
							deleteParseComponent(newParseComponent);
							#endif
						}
						else
						{
						#endif

							GIAtxtRelTranslatorRulesComponentParseTree* newParseComponent = GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkComponentToParseTreeComponentNew(component);	//new GIAtxtRelTranslatorRulesComponentParseTree(*GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkComponentToParseTreeComponent(component));	//copy rules component
							//newParseComponent->groupTypeRef = NULL;	
							//newParseComponent->groupRef = NULL;
							//newParseComponent->subComponents.clear();	//NB this is necessary as newParseComponent is a copied version of the original rules object
							currentParseTreeGroup->components.push_back(newParseComponent);

				
							#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
							forwardNounVerbVariantRequirementsGroupToComponent(currentParseTreeGroup, newParseComponent);
							#endif
			
							if(GIAtxtRelTranslatorRulesComponentClassObject.componentHasSubcomponents(component))
							{
								//NB subcomponents are added to currentParseTreeGroup in the same way (ie at same level) as ordinary components - therefore delete the artificial component representing the subcomponents section:
								currentParseTreeGroup->components.pop_back();
								deleteParseComponent(newParseComponent);

								int performanceTemp = *performance;
								if(generateRulesGroupTreeComponents(GIAtxtRelTranslatorRulesTokenLayers, &(component->subComponents), sentenceContentsSubset, currentParseTreeGroup, &performanceTemp, true, component->componentType, component->optional, layer, previousGroupType, numberOfConsecutiveTimesPreviousGroupType))
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
								if(generateRulesGroupTreeComponent(GIAtxtRelTranslatorRulesTokenLayers, component, sentenceContentsSubset, newParseComponent, &performanceTemp, layer, previousGroupType, numberOfConsecutiveTimesPreviousGroupType))
								{
									#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_OR
									if(subcomponentsOr)
									{
										bool passedTemp = true;
										GIAtxtRelTranslatorRulesGroupParseTree* tempGroup = new GIAtxtRelTranslatorRulesGroupParseTree();
										if(updatePerformance(performanceTemp, performance, tempGroup, tempGroup, passedTemp, &minIndexOfMatchesFoundBackupOptimum, sentenceContentsSubset, minIndexOfMatchesFoundBackup2, NULL))
										{
											subcomponentsOrFoundMatch = true;
											/*
											GIAtxtRelTranslatorNeuralNetworkOperations.printComponent(component, layer);
											GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
											cout << "updatePerformance{} performance = " << *performance << endl;
											*/
										}
										else
										{
											currentParseTreeGroup->components.pop_back();	//remove last component added
											deleteParseComponent(newParseComponent);
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
										GIAtxtRelTranslatorRulesGroupParseTree* tempGroup = new GIAtxtRelTranslatorRulesGroupParseTree();
										updatePerformance(performanceTemp, performance, &tempGroup, &tempGroup, passedTemp, &minIndexOfMatchesFoundBackupOptimum, sentenceContentsSubset, minIndexOfMatchesFoundBackup2, NULL);
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
											GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
											cout << "(component->optional) && !generateRulesGroupTreeComponent" << endl;
											GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
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
									deleteParseComponent(newParseComponent);
								}
							}
						#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_MISSING
						}
						#endif
					}
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
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "FAIL" << endl;
		#endif

	}
	else
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
		GIAtxtRelTranslatorNeuralNetworkOperations.printParseTreeDebugIndentation(layer);
		cout << "PASS" << endl;
		#endif
	}
	
	return foundWordMatch;
}

bool GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponent(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent, int* performance, int layer, string previousGroupType, int numberOfConsecutiveTimesPreviousGroupType)
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
		int minIndexOfMatchesFound = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContentsSubset);
		int w = minIndexOfMatchesFound+1;
		if(w < sentenceContentsSubset->size())
		{	
			GIApreprocessorPlainTextWord* currentWord = sentenceContentsSubset->at(w);

			//cout << "minIndexOfMatchesFound = " << minIndexOfMatchesFound << endl;
			//cout << "currentWord = " << currentWord->tagName << endl;
			if(findStringMatch(GIAtxtRelTranslatorRulesTokenLayers, component, currentWord, currentParseTreeComponent))
			{
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS
				cout << "findStringMatch: currentWord = " << currentWord->tagName << endl;
				#endif
				foundWordMatch = true;
				currentWord->alreadyFoundMatch = true;
				#ifndef GIA_TXT_REL_TRANSLATOR_RULES_DEFINE_WORD_TRANSLATOR_SENTENCE_ENTITY_INDEX_AT_START
				currentWord->translatorSentenceEntityIndex = GIAtranslatorOperations.convertSentenceContentsIndexToEntityIndex(w);
				#endif
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
					if(numberOfConsecutiveTimesPreviousGroupType < GIA_TXT_REL_TRANSLATOR_GROUP_TYPE_MAX_NUMBER_CONSECUTIVE_LAYERS)
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
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS3
				cout << "false pass 2" << endl;
				#endif
				pass = false;
			}		
		}
		else
		{
			cerr << "GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponent{} error: (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP) && (component->groupTypeRef == NULL)" << endl;
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
				GIAtxtRelTranslatorRulesGroupParseTree* newParseGroup = GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkGroupToParseTreeGroupNew(component->groupRef);	//new GIAtxtRelTranslatorRulesGroupParseTree(*GIAtxtRelTranslatorNeuralNetworkOperations.convertNeuralNetworkGroupToParseTreeGroup(component->groupRef));	//copy group component
				newParseGroup->components.clear();	//added 5 Mar 2018
				currentParseTreeComponent->parseTreeGroupRef = newParseGroup;
				//currentParseTreeComponent->groupRefName = newParseGroup->groupName;	//added 17 March 2018
				
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				forwardNounVerbVariantRequirementsComponentToGroup(currentParseTreeComponent, newParseGroup);
				#endif

				if(generateParseTreeGroup(GIAtxtRelTranslatorRulesTokenLayers, currentParseTreeComponent->groupRef, sentenceContentsSubset, newParseGroup, performance, layer+1, component->groupTypeRefName, numberOfConsecutiveTimesPreviousGroupType))
				{
					foundWordMatch = true;
				}
			}
			else if(component->groupTypeRef != NULL)
			{
				GIAtxtRelTranslatorRulesGroupParseTree* newParseGroup = new GIAtxtRelTranslatorRulesGroupParseTree();	//create new cgroup component
				currentParseTreeComponent->parseTreeGroupRef = newParseGroup;

				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				forwardNounVerbVariantRequirementsComponentToGroup(currentParseTreeComponent, newParseGroup);
				#endif
				
				if(generateParseTreeGroupType(GIAtxtRelTranslatorRulesTokenLayers, currentParseTreeComponent->groupTypeRef, sentenceContentsSubset, newParseGroup, performance, layer+1, component->groupTypeRefName, numberOfConsecutiveTimesPreviousGroupType, currentParseTreeComponent))
				{
					foundWordMatch = true;
				}	
			}
			else
			{
				cerr << "GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponent{} error: (component->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP) && (component->groupTypeRef/groupRef == NULL)" << endl;
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
		cerr << "GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponent{} error: component->componentType illdefined" << endl;
		exit(EXIT_ERROR);
	}
	
	return foundWordMatch;
}

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
bool GIAtxtRelTranslatorNeuralNetworkInverseClass::forwardNounVerbVariantRequirementsComponentToGroup(GIAtxtRelTranslatorRulesComponentParseTree* component, GIAtxtRelTranslatorRulesGroupParseTree* newParseGroup)
{
	bool result = true;
	//cout << "1 GIAtxtRelTranslatorNeuralNetworkInverseClass::forwardNounVerbVariantRequirementsComponentToGroup: currentComponent->semanticRelationReturnEntity" << endl;
	if(component->wordVerbVariantType != GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		newParseGroup->wordVerbVariantTypeDerived = component->wordVerbVariantType;
		//cout << "1 newParseGroup->wordVerbVariantTypeDerived = " << newParseGroup->wordVerbVariantTypeDerived << endl;
	}
	if(component->wordNounVariantType != GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
	{
		newParseGroup->wordNounVariantTypeDerived = component->wordNounVariantType;		
		//cout << "1 newParseGroup->wordNounVariantTypeDerived = " << newParseGroup->wordNounVariantTypeDerived << endl;
	}		
	return result;
}
bool GIAtxtRelTranslatorNeuralNetworkInverseClass::forwardNounVerbVariantRequirementsGroupToComponent(GIAtxtRelTranslatorRulesGroupParseTree* currentParseGroup, GIAtxtRelTranslatorRulesComponentParseTree* currentComponent)
{
	bool result = true;
	if(currentComponent->semanticRelationReturnEntity)
	{
		//cout << "2 GIAtxtRelTranslatorNeuralNetworkInverseClass::forwardNounVerbVariantRequirementsGroupToComponent: currentComponent->semanticRelationReturnEntity" << endl;
		if(currentParseGroup->wordVerbVariantTypeDerived != GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
		{
			currentComponent->wordVerbVariantType = currentParseGroup->wordVerbVariantTypeDerived;
			//cout << "2 currentComponent->wordVerbVariantType = " << currentComponent->wordVerbVariantType << endl;
		}
		if(currentParseGroup->wordNounVariantTypeDerived != GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
		{
			currentComponent->wordNounVariantType = currentParseGroup->wordNounVariantTypeDerived;	
			//cout << "2 currentComponent->wordNounVariantType = " << currentComponent->wordNounVariantType << endl;	
		}		
	}
	return result;
}
#endif
			

bool GIAtxtRelTranslatorNeuralNetworkInverseClass::findStringMatch(vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesComponentNeuralNetwork* component, GIApreprocessorPlainTextWord* currentWord, GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent)
{
	bool foundWordMatchTemp = false;
	if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_LRPEXTERNALWORDLISTS)
	{
		string wordPOStypeName = component->wordPOStype;
		int wordPOStype = GIApreprocessorWordClassObject.getPOStypeFromName(wordPOStypeName);
		
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
				currentWord->wordPOStypeInferred = wordPOStype;	//this is required to quickly check wordPOStypeInferred of previous words in current parse tree 
				currentParseTreeComponent->wordPOStypeInferred = wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
			}
			#endif
			#else
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_AS_NOUNS
			if(wordPOStype == GIA_PREPROCESSOR_POS_TYPE_NOUN)
			{
				foundWordMatchTemp = true;
				currentWord->wordPOStypeInferred = wordPOStype;	//this is required to quickly check wordPOStypeInferred of previous words in current parse tree 
				currentParseTreeComponent->wordPOStypeInferred = wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
				//cout << "(wordPOStype == GIA_PREPROCESSOR_POS_TYPE_NOUN): currentWord = " << currentWord->tagName << endl;
			}
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_UNKNOWN_POSTYPES_MID_SENTENCE_CAPITALISED_WORDS_AS_PROPERNOUNS_METHOD2
			if((wordPOStype == GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT) && GIApreprocessorPOStagger.isMidSentenceUppercaseWordLikelyProperNoun(currentWord))
			{
				foundWordMatchTemp = true;
				currentWord->wordPOStypeInferred = wordPOStype;	//this is required to quickly check wordPOStypeInferred of previous words in current parse tree 
				currentParseTreeComponent->wordPOStypeInferred = wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
				//cout << "(wordPOStype == GIA_PREPROCESSOR_POS_TYPE_PROPERNOUN_DEFAULT): currentWord = " << currentWord->tagName << endl;
			}
			#endif
			#endif
		}
		else
		{
		#endif

			//cout << "wordPOStypeName = " << wordPOStypeName << endl;
			//cout << "wordPOStype = " << wordPOStype << endl;
			if(verifyPOStype(currentWord, wordPOStype))
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				bool pass = true;
				if(currentParseTreeComponent->wordVerbVariantType != GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
				{
					//cout << "currentParseTreeComponent->wordVerbVariantType != GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN = " << currentParseTreeComponent->wordVerbVariantType << endl;
					bool verbVariantMatchFound = false;
					if(currentWord->wordVerbVariantGrammaticalTenseForm == currentParseTreeComponent->wordVerbVariantType)
					{
						verbVariantMatchFound = true;
					}
					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_INTERPRET_PAST_AND_PAST_PARTICIPLE_THE_SAME
					else if((currentParseTreeComponent->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST) && (currentWord->wordVerbVariantGrammaticalTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE))
					{
						verbVariantMatchFound = true;
					}
					else if((currentParseTreeComponent->wordVerbVariantType == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE) && (currentWord->wordVerbVariantGrammaticalTenseForm == GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST))
					{
						verbVariantMatchFound = true;
					}
					#endif
					if(!verbVariantMatchFound)
					{
						pass = false;
						//cout << "\tfail: currentWord->wordVerbVariantGrammaticalTenseForm = " << currentWord->wordVerbVariantGrammaticalTenseForm << endl;
						//cout << "\tcurrentWord = " << currentWord->tagName << endl;
					}
				}
				if(currentParseTreeComponent->wordNounVariantType != GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN)
				{
					//cout << "currentParseTreeComponent->wordVerbVariantType != GIA_PREPROCESSOR_WORD_VERB_DATABASE_TAG_BASE_TENSE_FORM_UNKNOWN = " << currentParseTreeComponent->wordVerbVariantType << endl;
					//cout << "currentWord->tagName = " << currentWord->tagName << endl;
					bool nounVariantMatchFound = false;
					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_DETECT_IRREGULAR_NOUN_FORMS
					if(currentWord->wordNounVariantGrammaticalTenseForm == GIA_PREPROCESSOR_WORD_NOUN_DATABASE_TAG_BASE_TENSE_FORM_SINGULAR_OR_PLURAL)
					{
						nounVariantMatchFound = true;
						//cout << "nounVariantMatchFound; currentWord->tagName = " << currentWord->tagName << endl;
					}
					else
					{
					#endif
						if(currentWord->wordNounVariantGrammaticalTenseForm == currentParseTreeComponent->wordNounVariantType)
						{
							nounVariantMatchFound = true;
						}
					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT_DETECT_IRREGULAR_NOUN_FORMS
					}
					#endif
					if(!nounVariantMatchFound)
					{
						pass = false;
					}			
				}		
				if(pass)
				{
				#endif

					foundWordMatchTemp = true;
					currentWord->wordPOStypeInferred = wordPOStype;	//this is required to quickly check wordPOStypeInferred of previous words in current parse tree 
					currentParseTreeComponent->wordPOStypeInferred = wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
	
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_WORD_NOUN_VERB_VARIANT
				}
				#endif	
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
			currentWord->wordPOStypeInferred = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;		//this is required to quickly check wordPOStypeInferred of previous words in current parse tree 	//component->wordPOStype;  -need to add wordPOStype attribute to <component>? Not required at present as all stringType explicit tagged words are currently disgarded by semantic network
			currentParseTreeComponent->wordPOStypeInferred = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)
		}
	}
	else if(component->stringType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_STRINGTYPE_TOKENS)
	{
		if(GIAtxtRelTranslatorRules.isClassTagWrapper(currentWord->tagName, component->tokenLayer, component->tokenClass, component->tokenType, GIAtxtRelTranslatorRulesTokenLayers))
		{
			foundWordMatchTemp = true;
			int wordPOStype = GIA_PREPROCESSOR_POS_TYPE_UNDEFINED;
			if(SHAREDvars.textInTextArray(component->tokenClass, GIApreprocessorPOStypeNameArray, GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES, &wordPOStype))
			{
				currentWord->wordPOStypeInferred = wordPOStype;	//this is required to quickly check wordPOStypeInferred of previous words in current parse tree 
				currentParseTreeComponent->wordPOStypeInferred = wordPOStype;	//store a copy of wordPOStypeInferred in parseTree (which will not overwritten by a future bad parse unlike that copied to currentWord)	
			}
			else
			{
				cerr << "GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponent{} error: !if(textInTextArray(component->tokenClass, GIApreprocessorPOStypeNameArray, GIA_PREPROCESSOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES, &wordPOStype)" << endl;
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
		cerr << "GIAtxtRelTranslatorNeuralNetworkInverseClass::generateRulesGroupTreeComponent{} error: component->stringType illegal" << endl;
		exit(EXIT_ERROR);
	}
	return foundWordMatchTemp;
}

bool GIAtxtRelTranslatorNeuralNetworkInverseClass::verifyPOStype(GIApreprocessorPlainTextWord* currentWord, unsigned char wordPOStype)
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


							
bool GIAtxtRelTranslatorNeuralNetworkInverseClass::updatePerformance(const int performanceTemp, int* performance, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroupTemp, const bool passedTemp, int* minIndexOfMatchesFoundBackupOptimum, vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup, GIAtxtRelTranslatorRulesComponentParseTree* previousParseTreeComponent)
{
	bool result = false;
	
	if(passedTemp)
	{
		if(performanceTemp > *performance)
		{
			result = true;
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
			
			*minIndexOfMatchesFoundBackupOptimum = GIAtxtRelTranslatorNeuralNetworkOperations.calculateMinIndexOfMatchesFound(sentenceContentsSubset);	//moved to higher level nesting in 3f8e
		}
		
	}
	clearAllWordsAlreadyFoundMatchInComponent(sentenceContentsSubset, minIndexOfMatchesFoundBackup);
	
	if(!result)
	{
		//delete all subgroups/components recursively in currentParseTreeGroupTemp
		deleteAllSubgroupsRecurse(currentParseTreeGroupTemp, 1);
	}
	delete currentParseTreeGroupTemp;
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkInverseClass::deleteAllSubgroupsRecurse(GIAtxtRelTranslatorRulesGroupParseTree* currentParseTreeGroup, int layer)
{
	bool result = true;
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent = (currentParseTreeGroup->components)[i];
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
			delete (currentParseTreeComponent->parseTreeGroupRef);
		}
		
		delete currentParseTreeComponent;
	}
	
	return result;
}

bool GIAtxtRelTranslatorNeuralNetworkInverseClass::deleteParseComponent(GIAtxtRelTranslatorRulesComponentParseTree* currentParseTreeComponent)
{
	bool result = true;
	
	if(currentParseTreeComponent->parseTreeGroupRef != NULL)
	{
		if(!deleteAllSubgroupsRecurse(currentParseTreeComponent->parseTreeGroupRef, 0))
		{
			result = false;
		}
		delete (currentParseTreeComponent->parseTreeGroupRef);
	}
		
	delete currentParseTreeComponent;
	
	return result;
}



void GIAtxtRelTranslatorNeuralNetworkInverseClass::clearAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundBackup)
{	
	for(int w=0; w<sentenceContentsSubset->size(); w++)
	{
		GIApreprocessorPlainTextWord* currentWord = sentenceContentsSubset->at(w);
		if(w > minIndexOfMatchesFoundBackup)
		{
			currentWord->alreadyFoundMatch = false;
		}
	}
}

void GIAtxtRelTranslatorNeuralNetworkInverseClass::restoreAllWordsAlreadyFoundMatchInComponent(vector<GIApreprocessorPlainTextWord*>* sentenceContentsSubset, const int minIndexOfMatchesFoundNew)
{	
	//cout << "restoreAllWordsAlreadyFoundMatchInComponent: minIndexOfMatchesFoundNew = " << minIndexOfMatchesFoundNew << endl;
	//exit(EXIT_ERROR);
	
	for(int w=0; w<sentenceContentsSubset->size(); w++)
	{
		GIApreprocessorPlainTextWord* currentWord = sentenceContentsSubset->at(w);
		if(w < minIndexOfMatchesFoundNew)
		{
			currentWord->alreadyFoundMatch = true;
		}
	}
}



#endif
#endif

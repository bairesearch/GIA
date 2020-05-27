
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
 * File Name: GIAposRelTranslatorParserOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k17a 26-May-2020
 * Requirements: 
 * Description: Part-of-speech Relation Translator Parser Operations
 * /
 *******************************************************************************/


#include "GIAposRelTranslatorParserOperations.hpp"


bool GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelationsWrapper(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupParseTree* firstParseTreeGroup, int layer)
{
	bool result = true;
	
	GIAposRelTranslatorParserForwardPropogationSignalData parserForwardPropogationSignalData;
	if(!generateSemanticRelationsFromTxtRelations(translatorVariables, firstParseTreeGroup, &parserForwardPropogationSignalData, layer))
	{
		result = false;
	}
	
	return result;
}


bool GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelations(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, GIAposRelTranslatorParserForwardPropogationSignalData* parserForwardPropogationSignalData, int layer)
{
	bool result = true;

	int numberOfComponentsInGroup = currentParseTreeGroup->components.size();
	//cout << "numberOfComponentsInGroup = " << numberOfComponentsInGroup << endl;
	vector<GIAposRelTranslatorParserForwardPropogationSignalData> parseTreeComponentSignalDataArray(numberOfComponentsInGroup);
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{		
		GIAposRelTranslatorRulesComponentParseTree* componentOwner = (currentParseTreeGroup->components).at(i);
		GIAposRelTranslatorRulesComponentParseTree* parseTreeComponent = componentOwner;
	
		GIAposRelTranslatorParserForwardPropogationSignalData* parseTreeComponentSignalData = &(parseTreeComponentSignalDataArray[i]);
		
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_SANI_PRINT_FINAL
		GIAposRelTranslatorSANIPropagateOperations.printComponent(parseTreeComponent, layer);
		#endif
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
		GIAposRelTranslatorSANIPropagateOperations.printComponent(parseTreeComponent, layer);
		#endif
		
		if(parseTreeComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
		{
			if(parseTreeComponent->parseTreeGroupRef != NULL)
			{
				if(!generateSemanticRelationsFromTxtRelations(translatorVariables, parseTreeComponent->parseTreeGroupRef, parseTreeComponentSignalData, layer+1))
				{
					result = false;
				}
			}
			else
			{
				cout << "GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelations error: (parseTreeComponent->parseTreeGroupRef == NULL)" << endl;
				exit(EXIT_ERROR);
			}
		}
		else if(parseTreeComponent->componentType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			GIApreprocessorPlainTextWord* parseTreeComponentWord = parseTreeComponent->candidateStringMatch;
			parseTreeComponentSignalData->semanticRelationReturnEntity = parseTreeComponentWord->translatorEntity;
			//cout << "parseTreeComponentWord->translatorEntity->entityName = " << parseTreeComponentWord->translatorEntity->entityName << endl;
		}
		else
		{
			cerr << "GIAposRelTranslatorParserOperationsClass::generateRulesGroupTreeComponent{} error: parseTreeComponent->componentType illdefined" << endl;
			exit(EXIT_ERROR);
		}
	}
	
	if(!generateSemanticRelationsFromTxtRelationsPart2(translatorVariables, currentParseTreeGroup, &parseTreeComponentSignalDataArray, parserForwardPropogationSignalData, layer))
	{
		result = false;
	}

	return result;
}

bool GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelationsPart2(GIAtranslatorVariablesClass* translatorVariables, GIAposRelTranslatorRulesGroupParseTree* currentParseTreeGroup, vector<GIAposRelTranslatorParserForwardPropogationSignalData>* parseTreeComponentSignalDataArray, GIAposRelTranslatorParserForwardPropogationSignalData* parserForwardPropogationSignalData, int layer)
{	
	bool result = true;
	
	parserForwardPropogationSignalData->semanticRelationReturnEntity = NULL;
	
	//required;
	vector<GIAposRelTranslatorRulesComponentParseTree*> parseTreeComponentSemanticRelationFunctionListArray;
	vector<GIAposRelTranslatorRulesComponentParseTree*> parseTreeComponentSemanticRelationFunctionSubject(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	vector<GIAposRelTranslatorRulesComponentParseTree*> parseTreeComponentSemanticRelationFunctionObject(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	vector<GIAposRelTranslatorRulesComponentParseTree*> parseTreeComponentSemanticRelationFunctionDelimiter(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	for(int i=0; i<GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		parseTreeComponentSemanticRelationFunctionSubject[i] = NULL;
		parseTreeComponentSemanticRelationFunctionObject[i] = NULL;
		parseTreeComponentSemanticRelationFunctionDelimiter[i] = NULL;
	}
	
	vector<GIAentityNode*> entitySemanticRelationFunctionListArray;
	vector<GIAentityNode*> entitySemanticRelationFunctionSubject(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	vector<GIAentityNode*> entitySemanticRelationFunctionObject(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	vector<GIAentityNode*> entitySemanticRelationFunctionDelimiter(GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	for(int i=0; i<GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		entitySemanticRelationFunctionSubject[i] = NULL;
		entitySemanticRelationFunctionObject[i] = NULL;
		entitySemanticRelationFunctionDelimiter[i] = NULL;
	}
			
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	GIAentityNode* semanticRelationRecordEntitySubject = NULL;
	GIAentityNode* semanticRelationRecordEntityDelimiter = NULL;
	string semanticRelationRecordFunctionName = "";
	#endif
	
	//required;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	vector<GIAentityNode*> semanticRelationFunctionEntityArray;
	bool foundExplicitReturnFunctionName = false;
	bool foundImplicitReturnFunctionName = false;
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAposRelTranslatorRulesComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		
		for(int w=GIA_NLP_START_ENTITY_INDEX; w<=translatorVariables->currentPreprocessorSentenceInList->sentenceContentsLRP.size(); w++)
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[w];
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_STREAMLINED
			entity->semanticRelationWordPOStypeInferred = (translatorVariables->currentPreprocessorSentenceInList->sentenceContentsLRP)[w-1]->wordPOStypeInferred;
			#else
			//DOESNTWORK as not all currentParseTreeGroup->components have a valid candidateStringMatch
			if(parseTreeComponent->candidateStringMatch != NULL)
			{
				//cout << "parseTreeComponent->candidateStringMatch->translatorSentenceEntityIndex = " << parseTreeComponent->candidateStringMatch->translatorSentenceEntityIndex << endl;
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OLD_BUG
				if(entity->entityIndexTemp == GIAtranslatorOperations.convertSentenceContentsIndexToEntityIndex(parseTreeComponent->candidateStringMatch->translatorSentenceEntityIndex))
				#else
				if(entity->entityIndexTemp == parseTreeComponent->candidateStringMatch->translatorSentenceEntityIndex)	//OLD: 	if(entity->entityIndexTemp == GIAtranslatorOperations.convertSentenceContentsIndexToEntityIndex(parseTreeComponent->candidateStringMatch->translatorSentenceEntityIndex))
				#endif
				{
					entity->semanticRelationWordPOStypeInferred = parseTreeComponent->candidateStringMatch->wordPOStypeInferred;
					//cout << "entity->semanticRelationWordPOStypeInferred = " << entity->semanticRelationWordPOStypeInferred << endl;
				}
			}
			#endif
		}
	}	
	#endif
				
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAposRelTranslatorRulesComponentParseTree* parseTreeComponent = (currentParseTreeGroup->components).at(i);
	
		GIAposRelTranslatorParserForwardPropogationSignalData* parseTreeComponentSignalData = &((*parseTreeComponentSignalDataArray)[i]);
		
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
		if(parseTreeComponent->queryComparisonVariable)
		{
			translatorVariables->currentSentenceInList->isQuestion = true;
			parseTreeComponentSignalData->semanticRelationReturnEntity->isQuery = true;	//CHECKTHIS
			//#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
			parseTreeComponentSignalData->semanticRelationReturnEntity->entityName = GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
			//#endif
		}
		if(parseTreeComponent->isAuxiliaryQuery)	//NB parseTreeComponent->isQuery refers to an explicit "is" query, where as entitynode->isQuery refers to any kind of query
		{
			//cout << "isAuxiliaryQuery" << endl;
			translatorVariables->currentSentenceInList->isQuestion = true;
		}
		else if(parseTreeComponent->isWhichOrEquivalentWhatQuery)
		{
			//cout << "isWhichOrEquivalentWhatQuery" << endl;
			translatorVariables->currentSentenceInList->isQuestion = true;
			parseTreeComponentSignalData->semanticRelationReturnEntity->isQuery = true;
			parseTreeComponentSignalData->semanticRelationReturnEntity->isWhichOrEquivalentWhatQuery = true;
		}
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_ALIASES
		else if(parseTreeComponent->isNameQuery)
		{
			translatorVariables->currentSentenceInList->isQuestion = true;
			parseTreeComponentSignalData->semanticRelationReturnEntity->isQuery = true;
			parseTreeComponentSignalData->semanticRelationReturnEntity->isNameQuery = true;
		}
		if(parseTreeComponent->isName)
		{
			parseTreeComponentSignalData->semanticRelationReturnEntity->isName = true;
		}
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
		if(parseTreeComponent->isNumberOf)
		{
			parseTreeComponentSignalData->semanticRelationReturnEntity->isNumberOf = true;
		}
		#endif
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NUMBER_EXPLETIVES
		if(parseTreeComponent->isExpletive)
		{
			parseTreeComponentSignalData->semanticRelationReturnEntity->isExpletive = true;
			//cout << "parseTreeComponentSignalData->semanticRelationReturnEntity->isExpletive" << endl;
		}
		#endif
		
		for(int j=0; j<GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; j++)
		{
			//if((currentParseTreeGroup->semanticRelationFunctionName)[j] != "")
			//{	
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
				GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);	
				cout << "\e[31m (currentParseTreeGroup->semanticRelationFunctionName)[j] = " << (currentParseTreeGroup->semanticRelationFunctionName)[j] << "\e[0m" << endl; 
				#endif
					
				if((parseTreeComponent->semanticRelationIndexType)[j] == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_LIST)
				{
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
					cout << "\e[31m GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_LIST" << "\e[0m" << endl;
					#endif
					parseTreeComponentSemanticRelationFunctionListArray.push_back(parseTreeComponent);
					entitySemanticRelationFunctionListArray.push_back(parseTreeComponentSignalData->semanticRelationReturnEntity);
				}
				else if((parseTreeComponent->semanticRelationIndexType)[j] == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_SUBJECT)
				{
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
					cout << "\e[31m 1 entitySemanticRelationFunctionSubject[j] = " << parseTreeComponentSignalData->semanticRelationReturnEntity->entityName << "\e[0m" << endl;
					#endif
					parseTreeComponentSemanticRelationFunctionSubject[j] = parseTreeComponent;
					entitySemanticRelationFunctionSubject[j] = parseTreeComponentSignalData->semanticRelationReturnEntity;
				}
				else if((parseTreeComponent->semanticRelationIndexType)[j] == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_OBJECT)
				{
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
					cout << "\e[31m 2 entitySemanticRelationFunctionObject[j] = " << parseTreeComponentSignalData->semanticRelationReturnEntity->entityName << "\e[0m" << endl;
					#endif
					parseTreeComponentSemanticRelationFunctionObject[j] = parseTreeComponent;
					entitySemanticRelationFunctionObject[j] = parseTreeComponentSignalData->semanticRelationReturnEntity;
				}
				else if((parseTreeComponent->semanticRelationIndexType)[j] == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER)
				{
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
					cout << "\e[31m 3 entitySemanticRelationFunctionDelimiter[j] = " << parseTreeComponentSignalData->semanticRelationReturnEntity->entityName << "\e[0m" << endl;
					#endif
					parseTreeComponentSemanticRelationFunctionDelimiter[j] = parseTreeComponent;
					entitySemanticRelationFunctionDelimiter[j] = parseTreeComponentSignalData->semanticRelationReturnEntity;
				}
				else
				{
					//cerr << "GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelations{} warning: (currentParseTreeGroup->semanticRelationFunctionName)[j] is undefined; parseTreeComponent->semanticRelationIndexType)[j] = " << (parseTreeComponent->semanticRelationIndexType)[j] << endl;
				}
			//}
		}
		
		if(parseTreeComponent->semanticRelationReturnEntity)
		{
			//cout << "*** parseTreeComponent->componentIndex = " << parseTreeComponent->componentIndex << endl;
			
			parserForwardPropogationSignalData->semanticRelationReturnEntity = parseTreeComponentSignalData->semanticRelationReturnEntity;
			
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
			if(parseTreeComponent->semanticRelationReturnConnectionDynamic)
			{
				(parseTreeComponentSignalData->semanticRelationReturnEntity)->semanticRelationReturnConnectionDynamic = true;
			}
			#endif
			/*
			#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
			if(parseTreeComponent->semanticRelationReturnConnectionDynamic)
			{
				//semanticRelationReturnConnectionDynamic = true;
				parserForwardPropogationSignalData->semanticRelationReturnConnectionDynamic = true;
			}
			#endif
			*/
			
			if(parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)
			{
				cerr << "GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelations{} error; (parseTreeComponent->semanticRelationReturnEntity) && (parseTreeComponentSignalData->semanticRelationReturnEntity == NULL)" << endl;
				exit(EXIT_ERROR);
			}
	
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
			cout << "parserForwardPropogationSignalData->semanticRelationReturnEntity = " << (parserForwardPropogationSignalData->semanticRelationReturnEntity)->entityName << endl;
			#endif
		}


		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
		if(parseTreeComponent->semanticRelationRecord)
		{
			semanticRelationRecordFunctionName = parseTreeComponentSignalData->semanticRelationReturnFunctionNameRecord;
			semanticRelationRecordEntitySubject = parseTreeComponentSignalData->semanticRelationReturnEntitySubject;
			semanticRelationRecordEntityDelimiter = parseTreeComponentSignalData->semanticRelationReturnEntityDelimiter;
		}	
		if(parseTreeComponent->semanticRelationConnect)
		{
			if(semanticRelationRecordEntitySubject != NULL)
			{
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_TREAT_CONJUNCTION_ITEMS_AS_SAME_REFERENCE_SET
				bool sameReferenceSet = true;
				#else
				bool sameReferenceSet = false;
				#endif
								
				createSemanticRelationOrdered(translatorVariables, semanticRelationRecordFunctionName, "", NULL, semanticRelationRecordEntitySubject, parseTreeComponentSignalData->semanticRelationReturnEntity, semanticRelationRecordEntityDelimiter, sameReferenceSet, layer);
			}
			else
			{
				cerr << "GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelations{} error: (parseTreeComponent->semanticRelationConnect) && (semanticRelationRecordEntitySubject == NULL)" << endl;
				exit(EXIT_ERROR);
			}
		}
		#endif
							
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
		if(parseTreeComponent->semanticRelationReturnEntity && parseTreeComponent->semanticRelationReturnFunctionName != "")	//first/lowest level layer; defines both semanticRelationReturnFunctionNameIndexType and semanticRelationReturnFunctionName
		{
			foundExplicitReturnFunctionName = true;
			semanticRelationFunctionEntityArray.push_back(parseTreeComponentSignalData->semanticRelationReturnEntity);
			(parseTreeComponentSignalData->semanticRelationReturnEntity)->semanticRelationReturnFunctionName = parseTreeComponent->semanticRelationReturnFunctionName;
			(parseTreeComponentSignalData->semanticRelationReturnEntity)->semanticRelationReturnFunctionNameIndexType = parseTreeComponent->semanticRelationReturnFunctionNameIndexType;

			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_DYNAMIC_INDEX_TESTS
			if(parseTreeComponent->semanticRelationReturnFunctionNameIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER_OR_SUBJECT)
			{
				int semanticRelationReturnFunctionNameIndexTypeDynamic = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER;
				int nearestPreceedingDelimiterIndex = INT_DEFAULT_VALUE; 
				int nearestPreceedingSubjectIndex = INT_DEFAULT_VALUE;
				for(int w = (parseTreeComponentSignalData->semanticRelationReturnEntity)->entityIndexTemp; w>=GIA_NLP_START_ENTITY_INDEX; w--)
				{	
					GIApreprocessorPlainTextWord* currentWord = (*GIApreprocessorSentenceClassObject.getSentenceContents(translatorVariables->currentPreprocessorSentenceInList))[GIAtranslatorOperations.convertEntityIndexToSentenceContentsIndex(w)];
					if(nearestPreceedingDelimiterIndex == INT_DEFAULT_VALUE)
					{
						if(currentWord->wordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_VERB)
						{
							nearestPreceedingDelimiterIndex = w;
						}
					}
					if(nearestPreceedingDelimiterIndex == INT_DEFAULT_VALUE)
					{					
						if(currentWord->wordPOStypeInferred == GIA_PREPROCESSOR_POS_TYPE_NOUN)
						{
							nearestPreceedingSubjectIndex = w;
						}
					}
				}
				if(nearestPreceedingSubjectIndex != INT_DEFAULT_VALUE)
				{
					if(nearestPreceedingDelimiterIndex == INT_DEFAULT_VALUE)
					{
						semanticRelationReturnFunctionNameIndexTypeDynamic = GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_SUBJECT;
					}
				}
				//cout << "semanticRelationReturnFunctionNameIndexTypeDynamic = " << semanticRelationReturnFunctionNameIndexTypeDynamic << endl;
				(parseTreeComponentSignalData->semanticRelationReturnEntity)->semanticRelationReturnFunctionNameIndexType = semanticRelationReturnFunctionNameIndexTypeDynamic;
			}
			#endif
			
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
			cout << "\e[32m foundExplicitReturnFunctionName \e[0m" << endl;
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
			cout << "\e[32m (semanticRelationReturnFunctionEntityArray->parseTreeComponentSignalData)->entityName = " << (parseTreeComponentSignalData->semanticRelationReturnEntity)->entityName << "\e[0m" << endl;
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
			cout << "\e[32m (semanticRelationReturnFunctionEntityArray->parseTreeComponentSignalData)->semanticRelationReturnFunctionName = " << (parseTreeComponentSignalData->semanticRelationReturnEntity)->semanticRelationReturnFunctionName << "\e[0m" << endl;
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
			cout << "\e[32m (parseTreeComponentSignalData->semanticRelationReturnEntity)->semanticRelationReturnFunctionNameIndexType = " << (parseTreeComponentSignalData->semanticRelationReturnEntity)->semanticRelationReturnFunctionNameIndexType << "\e[0m" << endl;
			#endif
				
			if(parseTreeComponent->semanticRelationReturnFunctionNameIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN)
			{				
				cerr << "GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelations{} error: (parseTreeComponent->foundExplicitReturnFunctionName != "") && (parseTreeComponent->semanticRelationReturnFunctionNameIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN)" << endl;
				exit(EXIT_ERROR);	
			}
		}
		if(!((parseTreeComponentSignalData->semanticRelationReturnFunctionEntityArray).empty()))	//intermediary (hidden layer); does not define semanticRelationReturnFunctionNameIndexType and semanticRelationReturnFunctionName (take from lower layer)
		{
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
			cout << "\e[32m foundImplicitReturnFunctionName" << "\e[0m" << endl;
			for(int i=0; i<(parseTreeComponentSignalData->semanticRelationReturnFunctionEntityArray).size(); i++)
			{
				GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
				cout << "\e[32m (parseTreeComponentSignalData->semanticRelationReturnFunctionEntityArray)[i] = " << (parseTreeComponentSignalData->semanticRelationReturnFunctionEntityArray)[i]->entityName << "\e[0m" << endl;
			}			
			#endif
			
			foundImplicitReturnFunctionName = true;
			(parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray).insert((parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray).end(), (parseTreeComponentSignalData->semanticRelationReturnFunctionEntityArray).begin(), (parseTreeComponentSignalData->semanticRelationReturnFunctionEntityArray).end());	//append (parseTreeComponentSignalData->semanticRelationReturnFunctionEntityArray) to (parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray)
			//(parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray) = (parseTreeComponentSignalData->semanticRelationReturnFunctionEntityArray);
		}
		#endif
	}
	

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	if(foundImplicitReturnFunctionName)
	{	
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_SPECIAL_FUNCTION_TESTS
		bool specialFunctionTests = true;	
		if(currentParseTreeGroup->groupName == "queryWhere") 
		{
			specialFunctionTests = false;
		}
		if(specialFunctionTests)
		{
		#endif
			for(vector<GIAentityNode*>::iterator iter1 = (parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray).begin(); iter1 != (parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray).end(); )
			{
				GIAentityNode* tempEntity = *iter1;
				bool erasedElement = false;

				/*
				if(currentParseTreeGroup->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETAPPENDS])
				{
					tempEntity->semanticRelationReturnFunctionNameSameReferenceSet = true;
				}
				*/

				//cout << "\n\n currentParseTreeGroup->groupTypeName = " << currentParseTreeGroup->groupTypeName << endl;
				//cout << "currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << endl;
				#ifdef GIA_POS_REL_TRANSLATOR_RULES_ASSUME_HIGH_LEVEL_REFERENCE_SETS_DO_NOT_CONTAIN_EXPLICIT_SEMANTIC_RELATION_FUNCTION
				bool sameReferenceSet = false;
				if((currentParseTreeGroup->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETS]) ||
				(currentParseTreeGroup->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_REFERENCESETS]))
				{
					sameReferenceSet = false;
				}
				else
				{
					sameReferenceSet = true;
				}
				#else
				bool sameReferenceSet = true;
				#endif

				bool semanticRelationReturnConnectionDynamic = false;
				#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
				if(tempEntity->semanticRelationReturnConnectionDynamic)
				{
					semanticRelationReturnConnectionDynamic = true;
				}
				#endif
					
				if(tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_LIST)
				{
					cerr << "GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelations{} error: Illegal semanticRelationReturnFunctionNameIndexType; (tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_LIST)" << endl;
					exit(EXIT_ERROR);
				}
				else if(tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_SUBJECT)
				{
					if(entitySemanticRelationFunctionSubject[GIA_POS_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX] != NULL)
					{
						if(entitySemanticRelationFunctionSubject[GIA_POS_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX]->entityIndexTemp < tempEntity->entityIndexTemp)
						{
							//bool sameReferenceSet = tempEntity->semanticRelationReturnFunctionNameSameReferenceSet;
							createSemanticRelationPair(translatorVariables, tempEntity->semanticRelationReturnFunctionName, entitySemanticRelationFunctionSubject[GIA_POS_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX], tempEntity, sameReferenceSet, semanticRelationReturnConnectionDynamic, layer);
							iter1 = (parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray).erase(iter1);
							erasedElement = true;
						}
					}
				}
				else if(tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_OBJECT)
				{
					cerr << "GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelations{} error: Illegal semanticRelationReturnFunctionNameIndexType; (tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_OBJECT)" << endl;
					exit(EXIT_ERROR);				
				}
				else if(tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_POS_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER)
				{
					#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
					cout << "\e[34m want to createSemanticRelationPair: tempEntity->semanticRelationReturnFunctionName = " << tempEntity->semanticRelationReturnFunctionName << "\e[0m" << endl;
					GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
					cout << "\e[34m currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << "\e[0m" << endl;
					#endif
					if(entitySemanticRelationFunctionDelimiter[GIA_POS_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX] != NULL)
					{
						if(entitySemanticRelationFunctionDelimiter[GIA_POS_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX]->entityIndexTemp < tempEntity->entityIndexTemp)
						{
							//bool sameReferenceSet = tempEntity->semanticRelationReturnFunctionNameSameReferenceSet;
							createSemanticRelationPair(translatorVariables, tempEntity->semanticRelationReturnFunctionName, entitySemanticRelationFunctionDelimiter[GIA_POS_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX], tempEntity, sameReferenceSet, semanticRelationReturnConnectionDynamic, layer);
							iter1 = (parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray).erase(iter1);
							erasedElement = true;
						}
					}	
				}
				else
				{
					cerr << "GIAposRelTranslatorParserOperationsClass::generateSemanticRelationsFromTxtRelations{} error: tempEntity->semanticRelationReturnFunctionNameIndexType is illdefined;" << tempEntity->semanticRelationReturnFunctionNameIndexType << endl;
					exit(EXIT_ERROR);
				}

				if(!erasedElement)
				{
					iter1++;
				}
			}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_SPECIAL_FUNCTION_TESTS
		}
		#endif
	}
	(parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray).insert((parserForwardPropogationSignalData->semanticRelationReturnFunctionEntityArray).end(), semanticRelationFunctionEntityArray.begin(), semanticRelationFunctionEntityArray.end());	//append semanticRelationFunctionEntityArray (ie new elements defined on this layer) to semanticRelationReturnFunctionEntityArray
	#endif
				
	for(int i=0; i<GIA_POS_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		if((currentParseTreeGroup->semanticRelationFunctionName)[i] != "")
		{	
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
			parserForwardPropogationSignalData->semanticRelationReturnFunctionNameRecord = currentParseTreeGroup->semanticRelationFunctionName[0];	//always pass the first executed function details
			parserForwardPropogationSignalData->semanticRelationReturnEntitySubject = entitySemanticRelationFunctionSubject[0];	//always pass the first executed function details
			parserForwardPropogationSignalData->semanticRelationReturnEntityDelimiter = entitySemanticRelationFunctionDelimiter[0];	//always pass the first executed function details
			#endif

			#ifdef GIA_POS_REL_TRANSLATOR_RULES_ASSUME_HIGH_LEVEL_REFERENCE_SETS_DO_NOT_CONTAIN_EXPLICIT_SEMANTIC_RELATION_FUNCTION
			bool sameReferenceSet = false;
			//cout << "\n\n currentParseTreeGroup->groupTypeName = " << currentParseTreeGroup->groupTypeName << endl;
			//cout << "currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << endl;
			if((currentParseTreeGroup->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETS]) ||
			(currentParseTreeGroup->groupTypeName == GIAposRelTranslatorRulesGroupsTypes[GIA_POS_REL_TRANSLATOR_RULES_GROUPS_TYPE_REFERENCESETS]))
			{
				sameReferenceSet = false;
			}
			else
			{
				sameReferenceSet = true;
			}
			#else
			bool sameReferenceSet = true;
			#endif

			
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_OPTIONAL
			bool pass = true;
			int numberOfSemanticRelationIndicesFound = 0;
			if(entitySemanticRelationFunctionSubject[i] != NULL)
			{
				numberOfSemanticRelationIndicesFound++;
			}
			if(entitySemanticRelationFunctionObject[i] != NULL)
			{
				numberOfSemanticRelationIndicesFound++;
			}
			if(entitySemanticRelationFunctionDelimiter[i] != NULL)
			{
				numberOfSemanticRelationIndicesFound++;
			}
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			//cout << "entitySemanticRelationFunctionListArray.size() = " << entitySemanticRelationFunctionListArray.size() << endl;
			#endif
			if(entitySemanticRelationFunctionListArray.size() > 1)
			{
				numberOfSemanticRelationIndicesFound = 2;
			}
			if(numberOfSemanticRelationIndicesFound < 2)
			{
				pass = false;	//one of the indexType components would have been marked as optional, and therefore the group semanticRelation will not be applied
			}
			if(pass)
			{
			#endif
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
				//GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
				//cout << "\e[33m **** createSemanticRelationOrdered{}: currentParseTreeGroup->semanticRelationFunctionName[i] = " << currentParseTreeGroup->semanticRelationFunctionName[i] << "\e[0m" << endl;
					//OLD: GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunctionSubject[i]->entityName, entitySemanticRelationFunctionObject[i]->entityName, currentParseTreeGroup->semanticRelationFunctionName[i], entitySemanticRelationFunctionSubject[i]->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunctionObject[i]->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
				#endif
				
				createSemanticRelationOrdered(translatorVariables, currentParseTreeGroup->semanticRelationFunctionName[i], currentParseTreeGroup->semanticRelationFunctionConditionNewName, &entitySemanticRelationFunctionListArray, entitySemanticRelationFunctionSubject[i], entitySemanticRelationFunctionObject[i], entitySemanticRelationFunctionDelimiter[i], sameReferenceSet, layer);
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_OPTIONAL
			}
			#endif
		}
	}

	return result;
}

bool GIAposRelTranslatorParserOperationsClass::createSemanticRelationOrdered(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunctionSubject, GIAentityNode* entitySemanticRelationFunctionObject, GIAentityNode* entitySemanticRelationFunctionDelimiter, const bool sameReferenceSet, int layer)
{
	if(entitySemanticRelationFunctionListArray->empty())
	{
		if(entitySemanticRelationFunctionSubject == NULL)
		{			
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			//GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
			//cout << "createSemanticRelationOrdered{}: " << GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunctionObject->entityName, entitySemanticRelationFunctionDelimiter->entityName, semanticRelationFunctionName, entitySemanticRelationFunctionObject->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunctionDelimiter->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
			#endif
			return createSemanticRelation(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, NULL, entitySemanticRelationFunctionObject, entitySemanticRelationFunctionDelimiter, NULL, sameReferenceSet, false, layer);
		}
		else if(entitySemanticRelationFunctionObject == NULL)
		{			
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			//GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
			//cout << "createSemanticRelationOrdered{}: " << GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunctionSubject->entityName, entitySemanticRelationFunctionDelimiter->entityName, semanticRelationFunctionName, entitySemanticRelationFunctionSubject->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunctionDelimiter->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
			#endif
			return createSemanticRelation(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, NULL, entitySemanticRelationFunctionSubject, entitySemanticRelationFunctionDelimiter, NULL, sameReferenceSet, false, layer);
		}
		else
		{	
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			//GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
			//cout << "createSemanticRelationOrdered{}: " << GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunctionSubject->entityName, entitySemanticRelationFunctionObject->entityName, semanticRelationFunctionName, entitySemanticRelationFunctionSubject->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunctionObject->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
			#endif
			if(entitySemanticRelationFunctionDelimiter != NULL)
			{
				#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
				cout << "\tentitySemanticRelationFunctionDelimiter = " << entitySemanticRelationFunctionDelimiter->entityName << endl;
				#endif
			}
			else
			{
				//cout << "warning (entitySemanticRelationFunctionDelimiter != NULL): semanticRelationFunctionName = " << semanticRelationFunctionName << endl;
			}
			return createSemanticRelation(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, NULL, entitySemanticRelationFunctionSubject, entitySemanticRelationFunctionObject, entitySemanticRelationFunctionDelimiter, sameReferenceSet, false, layer);
		}
	}
	else
	{
		#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
		//GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
		//cout << "createSemanticRelationOrdered{}: " << semanticRelationFunctionName << "(entitySemanticRelationFunctionListArray)" << endl;
		#endif
		return createSemanticRelation(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, entitySemanticRelationFunctionListArray, entitySemanticRelationFunctionSubject, entitySemanticRelationFunctionObject, entitySemanticRelationFunctionDelimiter, sameReferenceSet, false, layer);
	}
}

bool GIAposRelTranslatorParserOperationsClass::createSemanticRelationPair(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet, const bool semanticRelationReturnConnectionDynamic, int layer)
{
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
	//GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
	//cout << "\e[33m **** createSemanticRelationPair{}: semanticRelationFunctionName = " << semanticRelationFunctionName << "\e[0m" << endl; 
		//OLD: GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunction1->entityName, entitySemanticRelationFunction2->entityName, semanticRelationFunctionName, entitySemanticRelationFunction1->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunction2->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
	#endif
	return createSemanticRelation(translatorVariables, semanticRelationFunctionName, "", NULL, entitySemanticRelationFunction1, entitySemanticRelationFunction2, NULL, sameReferenceSet, semanticRelationReturnConnectionDynamic, layer);
}


bool GIAposRelTranslatorParserOperationsClass::createSemanticRelation(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet, const bool semanticRelationReturnConnectionDynamic, int layer)
{
	#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
	GIAposRelTranslatorSANIPropagateOperations.printParseTreeDebugIndentation(layer);
	if(entitySemanticRelationFunctionListArray == NULL)
	{
		cout << "\e[33m **** createSemanticRelation{}: " << GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunction1->entityName, entitySemanticRelationFunction2->entityName, semanticRelationFunctionName, entitySemanticRelationFunction1->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunction2->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << "\e[0m" << endl;
	}
	else
	{
		cout << "\e[33m **** createSemanticRelationOrdered{}: " << semanticRelationFunctionName << "(entitySemanticRelationFunctionListArray)" << "\e[0m" << endl;
	}
	#endif
				
	if(entitySemanticRelationFunction1 != NULL)
	{
		entitySemanticRelationFunction1->semanticRelationEntityIsReferenced = true;
		//cout << "entitySemanticRelationFunction1 = " << entitySemanticRelationFunction1->entityName << endl;
	}
	if(entitySemanticRelationFunction2 != NULL)
	{
		entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = true;
		//cout << "entitySemanticRelationFunction2 = " << entitySemanticRelationFunction2->entityName << endl;
	}
	if(entitySemanticRelationFunction3 != NULL)
	{
		entitySemanticRelationFunction3->semanticRelationEntityIsReferenced = true;
		//cout << "entitySemanticRelationFunction3 = " << entitySemanticRelationFunction3->entityName << endl;
	}
	if(entitySemanticRelationFunctionListArray != NULL)
	{
		for(int i=0; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAentityNode* entity = (*entitySemanticRelationFunctionListArray)[i];
			if(entity != NULL)
			{
				entity->semanticRelationEntityIsReferenced = true;
				//cout << "entitySemanticRelationFunction1 = " << entity->entityName << endl;
			}
		}
	}
			
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	return createSemanticRelationInMemory(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, entitySemanticRelationFunctionListArray, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, semanticRelationReturnConnectionDynamic);
	#else
	return createSemanticRelationInNetwork(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, entitySemanticRelationFunctionListArray, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, semanticRelationReturnConnectionDynamic);	
	#endif
}

#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
bool GIAposRelTranslatorParserOperationsClass::createSemanticRelationInMemory(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet, const bool semanticRelationReturnConnectionDynamic)
{
	bool result = true;
	
	int functionEntityIndex1 = entitySemanticRelationFunction1->semanticRelationPreprocessorEntityIndex;
	int functionEntityIndex2 = entitySemanticRelationFunction2->semanticRelationPreprocessorEntityIndex;
	int functionEntityIndex3 = entitySemanticRelationFunction3->semanticRelationPreprocessorEntityIndex;
				
	/*										
	if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createSubstanceInstance])
	{
	      GIAtranslatorOperations.addInstanceToInstanceDefinition(functionEntityIndex1, GIA_ENTITY_TYPE_SUBSTANCE, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createActionInstance])
	{
	      GIAtranslatorOperations.addInstanceToInstanceDefinition(functionEntityIndex1, GIA_ENTITY_TYPE_ACTION, translatorVariables);
	}
	else 
	*/
	
	if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_property])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyWithAuxiliary])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);

	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_action])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubjectReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObjectReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_condition])
	{
		/*
		#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED	//CHECKTHIS
		#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
		if(!(currentRelationInList->inverseRelationTwoWay))	//added 2j5e
		{
			#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
			if(currentRelationInList->inverseRelationSingle)	//added 2j5g
			{
				GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, functionEntityIndex2, currentRelationInList->relationTypeIndexNonInversed, sameReferenceSet);	//use original (non-inversed) values
				GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, functionEntityIndex1, currentRelationInList->relationTypeIndexNonInversed, sameReferenceSet);	//use original (non-inversed) values
			}
			else
			{
			#endif
		*/
				GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
				GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
		/*	
			#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
			}
			#endif
		}
		#endif
		#endif
		*/		
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definition])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);	
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionWithAuxiliary])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);		
	}

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_ALIASES
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAlias])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT_ALIAS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAliasWithAuxiliary])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_ALIAS, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_ALIAS_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
		#ifndef GIA_DISABLE_ALIAS_ENTITY_MERGING
		//entitySemanticRelationFunction3->semanticRelationEntityIsReferenced = false;
		#endif
	}
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUANTITY
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeQuantity])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		//functionEntityIndex2->semanticRelationEntityIsReferenced = false;
	}
	#endif
	
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionSubject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionObject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);		
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertySubject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyObject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubjectReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObjectReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ENTITY_GRAMMAR
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopula])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		//entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopulaSingle])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA_SINGLE, functionEntityIndex1, "", sameReferenceSet);
		//entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAuxiliary])
	{
		if(!createSemanticRelationMultiwordAuxiliary(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordPreposition])
	{
		if(!createSemanticRelationMultiwordPreposition(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordNoun])
	{
		if(!createSemanticRelationMultiwordNoun(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAlias])
	{
		if(!createSemanticRelationMultiwordAlias(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_DATE
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordDate])
	{
		if(!createSemanticRelationMultiwordDate(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	#endif
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityPredeterminer])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PREDETERMINER, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		//entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityDeterminer])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DETERMINER, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		//entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	#endif
	
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_LOGIC_REFERENCES
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicDefinition])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_DEFINITION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicAction])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_ACTION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_ACTION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicCondition])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONDITION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONDITION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConclusion])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONCLUSION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConjunction])
	{
		for(int i=0; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONJUNCTION, functionEntityIndex3, ((*entitySemanticRelationFunctionListArray)[i])->semanticRelationPreprocessorEntityIndex, sameReferenceSet);
		}
	}
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionNew])
	{
		GIArelation* relation = GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_NEW, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		relation->semanticRelationFunctionConditionNewName = semanticRelationFunctionConditionNewName;
		
		if(entitySemanticRelationFunction3 != NULL)
		{
			cout << "GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER: GIAposRelTranslatorParserOperationsClass::createSemanticRelationInMemory error: GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionNew not currently supported; auxiliary tense info should be connected to new condition entity" << endl;
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
			//entitySemanticRelationFunction3->semanticRelationEntityIsReferenced = false;
		}
	}	
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_MEASURE
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasure])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MEASURE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasurePer])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MEASURE_PER, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_SUBJECT_MULTI
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_thingMultiAdjConjunction])
	{
		for(int i=0; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT, functionEntityIndex1, ((*entitySemanticRelationFunctionListArray)[i])->semanticRelationPreprocessorEntityIndex, sameReferenceSet);
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_thingMultiConjunction])
	{
		for(int i=0; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONJUNCTION, functionEntityIndex3, ((*entitySemanticRelationFunctionListArray)[i])->semanticRelationPreprocessorEntityIndex, sameReferenceSet);
			//note passes up conjunction (e.g. "and") entity (ie semanticRelationIndexType="delimiter"), because GIAposRelTranslatorParser doesn't support multiple semanticRelationReturnEntities. GIA_POS_REL_TRANSLATOR_RULES_CODE_SUBJECT_MULTI therefore requires a post hoc operation where intermediate "and" logicReference entities are resolved into their components
		}
	}
	#endif
	else
	{	
		bool exitProgram = true;
		if(exitProgram)
		{
			cerr << "illegal functionToExecuteUponFind: " << semanticRelationFunctionName << endl;
			exit(EXIT_ERROR);
		}
	}
}
#else
bool GIAposRelTranslatorParserOperationsClass::createSemanticRelationInNetwork(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet, const bool semanticRelationReturnConnectionDynamic)
{
	bool result = true;
	
	//cout << "translatorVariables->sentenceIndex = " << translatorVariables->sentenceIndex << endl;
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	if(semanticRelationReturnConnectionDynamic)
	{
		GIAtranslatorOperations.semanticRelationReturnConnectionDynamic = true;
	}
	#endif
						
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if((semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_property]) ||
	(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyReverse]) ||
	(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definition]) ||
	(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAlias]))
	{									
		//create intermediary auxiliary 'have'/'be' (property/definition relationship entity) 
		entitySemanticRelationFunction3 = NULL;
		//FUTURE: consider using createNewRelationshipEntitySemantic instead
		if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_property])
		{
			entitySemanticRelationFunction3 = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(entitySemanticRelationFunction1, entitySemanticRelationFunction2, translatorVariables);
		}
		else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyReverse])
		{
			entitySemanticRelationFunction3 = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(entitySemanticRelationFunction2, entitySemanticRelationFunction1, translatorVariables);
		}
		else if((semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definition]) || (semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAlias]))
		{
			entitySemanticRelationFunction3 = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(entitySemanticRelationFunction1, entitySemanticRelationFunction2, translatorVariables);
		}
	}
	#endif
											
	//if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createSubstanceInstance])
	//{
	//	GIAtranslatorOperations.addInstanceToInstanceDefinition(entitySemanticRelationFunction1, GIA_ENTITY_TYPE_SUBSTANCE, translatorVariables);
	//}
	//else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createActionInstance])
	//{
	//	GIAtranslatorOperations.addInstanceToInstanceDefinition(entitySemanticRelationFunction1, GIA_ENTITY_TYPE_ACTION, translatorVariables);
	//}
	//else 
		
	if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_property])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);									
		#endif
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyWithAuxiliary])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		//GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(entitySemanticRelationFunction3);
		GIAtranslatorOperations.disableEntity(entitySemanticRelationFunction3);
		#endif
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_action])
	{
		GIAtranslatorOperations.connectActionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubject])
	{
		GIAtranslatorOperations.connectActionToSubject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObject])
	{
		GIAtranslatorOperations.connectActionToObject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubjectReverse])
	{
		GIAtranslatorOperations.connectActionToSubject(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObjectReverse])
	{
		GIAtranslatorOperations.connectActionToObject(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_condition])
	{
		cout << "sameReferenceSet = " << sameReferenceSet << endl;
		GIAtranslatorOperations.connectConditionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		
		/*
		#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
		if(currentRelationInList->relationTwoWay)     //limitation only works when GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_condition is called based on a single GIArelation
		{
		      entitySemanticRelationFunction3->conditionTwoWay = true;        //sets conditionTwoWay for condition substance not networkIndex
		}
		#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
		if(currentRelationInList->inverseRelationTwoWay)      //limitation only works when GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_condition is called based on a single GIArelation
		{
		      entitySemanticRelationFunction3->inverseConditionTwoWay = true; //sets inverseConditionTwoWay for condition substance not networkIndex
		}
		#endif
		#endif  	      
		*/
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definition])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectDefinitionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectDefinitionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);									
		#endif
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionWithAuxiliary])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectDefinitionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectDefinitionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);					
		//GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(entitySemanticRelationFunction3);
		GIAtranslatorOperations.disableEntity(entitySemanticRelationFunction3);
		#endif
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_ALIASES
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAlias])
	{
		if(!GIAtranslatorOperations.connectDefinitionAliasWrapper(translatorVariables, entitySemanticRelationFunction1, &entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAliasWithAuxiliary])
	{
		if(!GIAtranslatorOperations.connectDefinitionAliasWrapper(translatorVariables, entitySemanticRelationFunction1, &entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet))
		{
			result = false;
		}
		#ifndef GIA_DISABLE_ALIAS_ENTITY_MERGING
		entitySemanticRelationFunction3->semanticRelationEntityIsReferenced = false;
		#endif
	}
	#endif

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_QUANTITY
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeQuantity])
	{
		GIAtranslatorOperations.connectQuantityToEntity(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
		entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	#endif
	
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionSubject])
	{
		GIAtranslatorOperations.connectDefinitionToSubject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionObject])
	{
		GIAtranslatorOperations.connectDefinitionToObject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertySubject])
	{
		GIAtranslatorOperations.connectPropertyToSubject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyObject])
	{
		GIAtranslatorOperations.connectPropertyToObject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubject])
	{
		GIAtranslatorOperations.connectConditionToSubject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObject])
	{
		GIAtranslatorOperations.connectConditionToObject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubjectReverse])
	{
		GIAtranslatorOperations.connectConditionToSubject(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObjectReverse])
	{
		GIAtranslatorOperations.connectConditionToObject(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables);
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_ENTITY_GRAMMAR
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopula])
	{
		/*
		cout << "GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopula" << endl;
		cout << "entitySemanticRelationFunction1 = " << entitySemanticRelationFunction1->entityName << endl;
		cout << "entitySemanticRelationFunction2 = " << entitySemanticRelationFunction2->entityName << endl;
		*/
		string auxiliaryString = entitySemanticRelationFunction2->entityName;
		GIAtranslatorOperations.updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(entitySemanticRelationFunction1, auxiliaryString);	//this is required if entitySemanticRelationFunction2->multiwordAuxiliaryList.size() == 0
		for(int i=0; i<entitySemanticRelationFunction2->multiwordAuxiliaryList.size(); i++)
		{
			string auxiliaryString2 = (entitySemanticRelationFunction2->multiwordAuxiliaryList)[i];
			//cout << "auxiliaryString2 = " << auxiliaryString2 << endl;
			GIAtranslatorOperations.updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(entitySemanticRelationFunction1, auxiliaryString2);	//in the future this needs to be updated to take into account the grammatical implications of exact auxiliary sequence permutations (not just combinations); e.g. "will have had" / "is being"
		}
		entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopulaSingle])
	{
		string auxiliaryString = entitySemanticRelationFunction1->entityName;
		GIAtranslatorOperations.updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(entitySemanticRelationFunction1, auxiliaryString);	//this is required if entitySemanticRelationFunction1->multiwordAuxiliaryList.size() == 0
		for(int i=0; i<entitySemanticRelationFunction1->multiwordAuxiliaryList.size(); i++)
		{
			string auxiliaryString2 = (entitySemanticRelationFunction1->multiwordAuxiliaryList)[i];
			//cout << "auxiliaryString2 = " << auxiliaryString2 << endl;
			GIAtranslatorOperations.updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(entitySemanticRelationFunction1, auxiliaryString2);	//in the future this needs to be updated to take into account the grammatical implications of exact auxiliary sequence permutations (not just combinations); e.g. "will have had" / "is being"
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAuxiliary])
	{
		if(!createSemanticRelationMultiwordAuxiliary(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordPreposition])
	{
		if(!createSemanticRelationMultiwordPreposition(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordNoun])
	{
		if(!createSemanticRelationMultiwordNoun(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAlias])
	{
		if(!createSemanticRelationMultiwordAlias(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_DATE
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordDate])
	{
		if(!createSemanticRelationMultiwordDate(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	#endif
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityPredeterminer])
	{
		//cout << "entitySemanticRelationFunction1->entityName = " << entitySemanticRelationFunction1->entityName << endl;
		//cout << "entitySemanticRelationFunction2->entityName = " << entitySemanticRelationFunction2->entityName << endl;
		entitySemanticRelationFunction1->semanticRelationWordPredeterminer = entitySemanticRelationFunction2->entityName;
		entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityDeterminer])
	{
		//cout << "entitySemanticRelationFunction1->entityName = " << entitySemanticRelationFunction1->entityName << endl;
		//cout << "entitySemanticRelationFunction2->entityName = " << entitySemanticRelationFunction2->entityName << endl;
		entitySemanticRelationFunction1->semanticRelationWordDeterminer = entitySemanticRelationFunction2->entityName;
		entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	#endif
	
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyReverse])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectPropertyToEntity(entitySemanticRelationFunction2, entitySemanticRelationFunction1, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectPropertyToEntity(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables]);									
		#endif
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_LOGIC_REFERENCES
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicDefinition])
	{
		GIAtranslatorOperations.connectLogicReference(translatorVariables, GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_DEFINITION, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicAction])
	{
		GIAtranslatorOperations.connectLogicReference(translatorVariables, GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_VERB, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicCondition])
	{
		GIAtranslatorOperations.connectLogicReference(translatorVariables, GIA_POS_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_PREPOSITION, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConclusion])
	{
		GIAtranslatorOperations.connectLogicReferenceConclusion(translatorVariables, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConjunction])
	{
		for(int i=0; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			//OLD: GIAtranslatorOperations.connectLogicReferenceConjunction(translatorVariables, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
			GIAtranslatorOperations.connectLogicReferenceConjunction(translatorVariables, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, (*entitySemanticRelationFunctionListArray)[i], entitySemanticRelationFunction3, sameReferenceSet);
		}
	}
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionNew])
	{
		GIAentityNode* entitySemanticRelationFunction4 = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialCondition(entitySemanticRelationFunction2, entitySemanticRelationFunction1, semanticRelationFunctionConditionNewName, translatorVariables);
		GIAtranslatorOperations.connectConditionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction4, sameReferenceSet, translatorVariables);
		if(entitySemanticRelationFunction3 != NULL)
		{	
			string auxiliaryString = entitySemanticRelationFunction3->entityName;
			GIAtranslatorOperations.updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(entitySemanticRelationFunction4, auxiliaryString);
			entitySemanticRelationFunction3->semanticRelationEntityIsReferenced = false;
		}
	}
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_MEASURE
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasure])
	{
		GIAtranslatorOperations.connectMeasureToEntity(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasurePer])
	{
		GIAtranslatorOperations.connectMeasurePerToEntity(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
	}
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_SUBJECT_MULTI
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_thingMultiAdjConjunction])
	{
		for(int i=0; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
			entitySemanticRelationFunction3 = NULL;
			entitySemanticRelationFunction3 = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(entitySemanticRelationFunction1, (*entitySemanticRelationFunctionListArray)[i], translatorVariables);
			GIAtranslatorOperations.connectPropertyToEntity(entitySemanticRelationFunction1, (*entitySemanticRelationFunctionListArray)[i], entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
			#else
			GIAtranslatorOperations.connectDirectPropertyToEntity(entitySemanticRelationFunction1, (*entitySemanticRelationFunctionListArray)[i], sameReferenceSet, translatorVariables);									
			#endif
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_POS_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_thingMultiConjunction])
	{
		for(int i=0; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAtranslatorOperations.connectLogicReferenceConjunction(translatorVariables, GIA_POS_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, (*entitySemanticRelationFunctionListArray)[i], entitySemanticRelationFunction3, sameReferenceSet);
			//note passes up conjunction (e.g. "and") entity (ie semanticRelationIndexType="delimiter"), because GIAposRelTranslatorParser doesn't support multiple semanticRelationReturnEntities. GIA_POS_REL_TRANSLATOR_RULES_CODE_SUBJECT_MULTI therefore requires a post hoc operation where intermediate "and" logicReference entities are resolved into their components
		}
	}
	#endif
	else
	{	
		bool exitProgram = true;
		if(exitProgram)
		{
			cerr << "illegal functionToExecuteUponFind: " << semanticRelationFunctionName << endl;
			exit(EXIT_ERROR);
		}
	}
	
	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	if(semanticRelationReturnConnectionDynamic)
	{
		GIAtranslatorOperations.semanticRelationReturnConnectionDynamic = false;
	}
	#endif	
	
	return result;
}
#endif


bool GIAposRelTranslatorParserOperationsClass::createSemanticRelationMultiwordAuxiliary(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* lastEntityInList = (*entitySemanticRelationFunctionListArray)[entitySemanticRelationFunctionListArray->size()-1];
		for(int i=entitySemanticRelationFunctionListArray->size()-2; i>=0; i--)
		{
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];
			//cout << "currentEntityInList = " << currentEntityInList->entityName << endl;
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MULTIWORD_AUXILIARY, lastEntityInList->semanticRelationPreprocessorEntityIndex, currentEntityInList->semanticRelationPreprocessorEntityIndex, sameReferenceSet);
			#else
			if(!GIAtranslatorOperations.connectMultiwordAuxiliaryWrapper(translatorVariables, lastEntityInList, currentEntityInList, sameReferenceSet))
			{
				result = false;
			}
			#endif
			currentEntityInList->semanticRelationEntityIsReferenced = false;
		}		
	}
	
	return result;
}

bool GIAposRelTranslatorParserOperationsClass::createSemanticRelationMultiwordPreposition(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* firstEntityInList = (*entitySemanticRelationFunctionListArray)[0];
		for(int i=1; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MULTIWORD_PREPOSITION, firstEntityInList->semanticRelationPreprocessorEntityIndex, currentEntityInList->semanticRelationPreprocessorEntityIndex, sameReferenceSet);
			#else
			if(!GIAtranslatorOperations.connectMultiwordPrepositionWrapper(translatorVariables, firstEntityInList, currentEntityInList, sameReferenceSet))
			{
				result = false;
			}
			#endif
			currentEntityInList->semanticRelationEntityIsReferenced = false;
		}		
	}
	
	return result;
}


bool GIAposRelTranslatorParserOperationsClass::createSemanticRelationMultiwordNoun(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* lastEntityInList = (*entitySemanticRelationFunctionListArray)[entitySemanticRelationFunctionListArray->size()-1];

		//e.g. "goal line"
		for(int i=entitySemanticRelationFunctionListArray->size()-2; i>=0; i--)
		{	
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];

			#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PRENOMINAL_MODIFIER, lastEntityInList->semanticRelationPreprocessorEntityIndex, currentEntityInList->semanticRelationPreprocessorEntityIndex, sameReferenceSet);
			#else
			if(!GIAtranslatorOperations.connectPrenominalModifierWrapper(translatorVariables, lastEntityInList, currentEntityInList, sameReferenceSet))
			{
				result = false;
			}
			#endif		
		}
	}
	
	return result;
}

bool GIAposRelTranslatorParserOperationsClass::createSemanticRelationMultiwordAlias(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* lastEntityInList = (*entitySemanticRelationFunctionListArray)[entitySemanticRelationFunctionListArray->size()-1];

		//e.g. "Ms. Savata"
		for(int i=entitySemanticRelationFunctionListArray->size()-2; i>=0; i--)
		{				
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];

			#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MULTIWORD_ALIAS, lastEntityInList->semanticRelationPreprocessorEntityIndex, currentEntityInList->semanticRelationPreprocessorEntityIndex, sameReferenceSet);
			#else
			if(!GIAtranslatorOperations.connectMultiwordAliasWrapper(translatorVariables, lastEntityInList, currentEntityInList, sameReferenceSet))
			{
				result = false;
			}
			#endif	
			currentEntityInList->semanticRelationEntityIsReferenced = false;	
		}
	}
	
	return result;
}

#ifdef GIA_POS_REL_TRANSLATOR_RULES_CODE_DATE
bool GIAposRelTranslatorParserOperationsClass::createSemanticRelationMultiwordDate(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* firstEntityInList = (*entitySemanticRelationFunctionListArray)[0];
		
		for(int i=1; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];
			#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MULTIWORD_DATE, firstEntityInList->semanticRelationPreprocessorEntityIndex, currentEntityInList->semanticRelationPreprocessorEntityIndex, sameReferenceSet);
			#else
			if(!GIAtranslatorOperations.connectMultiwordDate(translatorVariables, firstEntityInList, currentEntityInList, sameReferenceSet))
			{
				result = false;
			}
			#endif
			currentEntityInList->semanticRelationEntityIsReferenced = false;
		}		
	}
	
	return result;
}
#endif

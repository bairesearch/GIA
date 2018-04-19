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
 * File Name: GIAtxtRelTranslatorParser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f4a 14-April-2018
 * Requirements: 
 * Description: Textual Relation Translator Parser
 * /
 *******************************************************************************/


#include "GIAtxtRelTranslatorParser.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3

//based on convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}:
bool GIAtxtRelTranslatorParserClass::convertSentenceTxtRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList)
{
	bool result = true;
	
	int numberOfWordsInSentence = translatorVariables->currentPreprocessorSentenceInList->sentenceContentsLRP.size();
	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(numberOfWordsInSentence) + SENTENCE_FIRST_ARTIFICIAL_INDEX;
	
	vector<bool> GIAentityNodeArrayFilled(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));		//NB could also use currentSentence->maxNumberOfWordsInSentence
	vector<GIAentityNode*> GIAnetworkIndexNodeArray(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	vector<GIAentityNode*> GIAentityNodeArray(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	vector<GIAfeature*> featureArrayTemp(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	#endif
	translatorVariables->GIAentityNodeArrayFilled = &GIAentityNodeArrayFilled;
	translatorVariables->GIAnetworkIndexNodeArray = &GIAnetworkIndexNodeArray;	
	translatorVariables->GIAentityNodeArray = &GIAentityNodeArray;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	translatorVariables->featureArrayTemp = &featureArrayTemp;
	#endif
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAnetworkIndexNodeArray[w] = NULL;
		GIAentityNodeArray[w] = NULL;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
		featureArrayTemp[w] = NULL;
		#endif
	}

	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	//GIAbot.botSwitchFirstAndSecondPersonTxt(translatorVariables);
	#endif
	
#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	GIAtranslatorOperations.generateTempFeatureArray(translatorVariables, &featureArrayTemp);	//regeneration required for Relex in case query variables detected

	#ifdef GIA_RELEX
	if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX) 				//OLD: (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		GIAtranslatorGrammar.fillGrammaticalArraysRelex(translatorVariables->currentSentenceInList);
	}
	#endif
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	if((translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER) || (translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP))	//OLD: (NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		//fillGrammaticalArraysStanford(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser, featureArrayTemp);	//not possible as Stanford syntactical dependency relations are assumed not available
		//past tense [preliminary only; aux/cop takes precedence], progressive tense, isDate, plurality, isProperNoun extraction
		GIAtranslatorGrammar.extractGrammaticalInformationStanford(translatorVariables->currentSentenceInList->firstFeatureInList, translatorVariables->NLPfeatureParser);
		
		//GIAsemRelTranslatorParser.fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(translatorVariables);	//already done by generateSemanticRelationsFromTxtRelationsWrapper
	}
	#endif

	locateAndAddAllNetworkIndexEntitiesBasedOnTxtRelations(translatorVariables);
	
 	GIAtranslatorGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, translatorVariables->currentSentenceInList->firstFeatureInList);

	GIAsynRelTranslatorRedistributeRelations.redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(translatorVariables);

#else
	locateAndAddAllNetworkIndexEntitiesBasedOnTxtRelations(translatorVariables);
#endif
	
	translatorVariables->GIAfeatureTempEntityNodeArray = &GIAentityNodeArray;	//required for linkAdvancedReferencesGIA/linkPronounReferencesRelex/linkPronounAndTextualContextReferencesStanfordCoreNLP
#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	#ifdef GIA_ADVANCED_REFERENCING
	if(linkPreestablishedReferencesGIA)
	{
		GIAtranslatorReferencing.linkAdvancedReferencesGIA(translatorVariables, firstGIAcoreferenceInList);	//NB second last parameter used to be GIAfeatureTempEntityNodeArray
	}
	else
	{
	#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
		//is synRelTranslator pronoun referencing supported by GIA3?

		#ifdef GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
		#endif
			GIAtranslatorReferencing.linkPronounReferencesRelex(translatorVariables);	//NB third parameter used to be GIAfeatureTempEntityNodeArray
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		}
		#ifdef GIA_STANFORD_CORENLP
		else if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			GIAtranslatorReferencing.linkPronounAndTextualContextReferencesStanfordCoreNLP(translatorVariables, translatorVariables->firstSentenceInList->firstCoreferenceInList);	//NB third parameter used to be GIAfeatureTempEntityNodeArray
		}
		#endif
		#endif
		#endif
		
		#else
		GIAtranslatorReferencing.linkPronounReferencesTxtRelTranslator(translatorVariables);
		#endif
	#ifdef GIA_ADVANCED_REFERENCING
	}
	#endif
#endif

	GIAsynRelTranslatorDefineSubstances.defineSubstancesAllNodes(translatorVariables);

	for(int w=GIA_NLP_START_ENTITY_INDEX; w<=numberOfWordsInSentence; w++)
	{
		GIAentityNode* entity = GIAentityNodeArray[w];
		((translatorVariables->currentPreprocessorSentenceInList->sentenceContentsLRP)[GIAtranslatorOperations.convertEntityIndexToSentenceContentsIndex(w)])->translatorEntity = entity;	//code from setPreprocessorSentenceTranslatorEntityReferences
	}
	
	if(!generateSemanticRelationsFromTxtRelationsWrapper(translatorVariables))
	{
		result = false;
	}

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
	identifyComparisonVariable(translatorVariables);
	#endif
	
	for(int w=GIA_NLP_START_ENTITY_INDEX; w<=numberOfWordsInSentence; w++)
	{
		GIAentityNode* entity = GIAentityNodeArray[w];
		bool pass = true;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
		if(entity->isNumberOf)
		{
			pass = false;	//in case of sentences containing a single entity x without a semanticRelationFunction being executed; i.e. "the number of x"
		}
		#endif
		if(entity->semanticRelationEntityIsReferenced)
		{
			pass = false;
		}
		if(pass)
		{
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(entity);
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			cout << "DISABLING entity = " << entity->entityName << endl;
			cout << "entity->entityType = " << entity->entityType << endl;
			#endif
		}
	}
	
	defineSubstancesBasedOnNetworkAndDeterminerInfo(translatorVariables);
	
	GIAsynRelTranslatorApplyAdvancedFeatures.defineTenseOnlyTimeConditions(translatorVariables);
	
	#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
	//identifyComparisonVariableBasedOnSemanticRelations(translatorVariables);
	//should already be done during GIAtxtRelTranslatorParserClass::createSemanticRelationInNetwork
	//comparison variables should already be identified by queries; queryWhere/queryWhat/queryWho / LogicReferenceQueryWhen/LogicReferenceQueryWhy etc
	#endif

	/*
	//CHECKTHIS?
	#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEMANTIC_PARSER_SEMANTIC_RELATION_GENERATION
	GIAsynRelTranslatorLinkEntitiesDynamic.linkEntitiesDynamic(translatorVariables);
	#endif
	*/

	//applyAdvancedFeaturesBasedOnSemanticRelations(translatorVariables);
	GIAsemRelTranslatorParser.defineQualitiesBasedOnSemanticRelations(translatorVariables);


	vector<GIAentityNode*>* entityNodesActiveListSentence = new vector<GIAentityNode*>;
	if(!GIAsemRelTranslatorParser.convertSentenceSemanticRelationsIntoGIAnetworkNodesEndPart1(translatorVariables, entityNodesActiveListSentence))
	{
		result = false;
	}
	if(!GIAsynRelTranslatorParser.convertSentenceRelationsIntoGIAnetworkNodesEndPart2(translatorVariables, entityNodesActiveListSentence))
	{
		result = false;
	}	
		
	return result;
}

bool GIAtxtRelTranslatorParserClass::locateAndAddAllNetworkIndexEntitiesBasedOnTxtRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;

	int numberOfWordsInSentence = translatorVariables->currentPreprocessorSentenceInList->sentenceContentsLRP.size();

	#ifdef GIA_GRAMMAR_IMPERATIVE_DETECTION
	bool toDetected = false;
	#endif
	GIAfeature* currentFeatureInList = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	currentFeatureInList = firstFeatureInSentence;
	#endif
	for(int w=GIA_NLP_START_ENTITY_INDEX; w<=numberOfWordsInSentence; w++)
	{
		GIApreprocessorPlainTextWord* currentWord = (translatorVariables->currentPreprocessorSentenceInList)->sentenceContentsLRP[GIAtranslatorOperations.convertEntityIndexToSentenceContentsIndex(w)];
		
		#ifndef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
		GIAfeature currentFeatureTemp;
		currentFeatureInList = &currentFeatureTemp;
		if(!GIAtranslatorGrammar.calculateGrammarUsingInferredPosTypes(currentWord, currentFeatureInList))
		{
			result = false;
		}
		#endif

		string wordName = currentWord->tagName;	//GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES: NB may use SYN_REL_TRANSLATOR_FEATURES to recalculate its name (using lemma instead of wordOrig) at a later stage in this function:

		//GIAentityNode* entity = new GIAentityNode();
		bool entityAlreadyExistant = false;
		bool entityEnabled = true;	//CHECKTHIS
		//cout << "currentFeatureInList->lemma = " << currentFeatureInList->lemma << endl;
		GIAentityNode* entity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&(currentFeatureInList->lemma), &entityAlreadyExistant, translatorVariables, entityEnabled);

		(*translatorVariables->GIAentityNodeArrayFilled)[w] = true;
		(*translatorVariables->GIAnetworkIndexNodeArray)[w] = entity;
		(*translatorVariables->GIAentityNodeArray)[w] = entity;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
		(*translatorVariables->featureArrayTemp)[w] = currentFeatureInList;	//CHECKTHIS; is this required?
		#endif

		entity->semanticRelationPreprocessorEntityIndex = currentWord->translatorSentenceEntityIndex;
		entity->semanticRelationWordPOStypeInferred = currentWord->wordPOStypeInferred;
		entity->sentenceIndexTemp = translatorVariables->sentenceIndex;
		entity->entityIndexTemp = w;
		entity->wordOrig = wordName;
		
		
		#ifndef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
		if(currentFeatureInList->stanfordPOS != FEATURE_POS_TAG_UNKNOWN_SYMBOL_SYM)
		{	
			#ifdef GIA_GRAMMAR_IMPERATIVE_DETECTION
			//extract from GIAtranslatorGrammarClass::extractGrammaticalInformationStanford
			if(toDetected)
			{
				currentFeatureInList->previousWordInSentenceIsTo = true;
			}
			if(currentFeatureInList->lemma == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PREPOSITION_TO)
			{
				toDetected = true;
			}
			else
			{
				toDetected = false;
			}
			#endif
			GIAtranslatorGrammar.extractPOSrelatedGrammaticalInformationStanford(currentFeatureInList, GIA_PREPROCESSOR_GRAMMATICALLY_STRICT_VERB_VARIANTS_ONLY_VALUE);			//regenerate grammatical information for feature - it should identify the verb as an infinitive/imperative based on previousWordInSentenceIsTo
			GIAtranslatorGrammar.applyPOSrelatedGrammaticalInfoToEntity(entity, currentFeatureInList);	//regenerate grammatical information for entity
		}
		entity->grammaticalProperNounTemp = currentFeatureInList->grammaticalIsProperNoun;
		entity->grammaticalGenderTemp = currentFeatureInList->grammaticalGender;
		#endif
		
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
		currentFeatureInList = currentFeatureInList->next;
		#endif
	}
	
	return result;
}


#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
void GIAtxtRelTranslatorParserClass::identifyComparisonVariable(GIAtranslatorVariablesClass* translatorVariables)
{
	if(translatorVariables->currentSentenceInList->isQuestion)
	{
		for(int i=0; i<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); i++)
		{
			if((*translatorVariables->GIAentityNodeArrayFilled)[i])
			{
				GIAentityNode* entityNode = (*translatorVariables->GIAentityNodeArray)[i];
				if(entityNode->isQuery)
				{
					if((entityNode->entityName == GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE) || (entityNode->isWhichOrEquivalentWhatQuery) || (entityNode->isNameQuery))
					{
						#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
						cout << "isQuery (entityNode->isNameQuery) = " << (entityNode->isNameQuery) << endl;
						#endif
						GIAtranslatorOperations.setComparisonVariableNode(entityNode);
						GIAtranslatorOperations.setFoundComparisonVariable(true);
					}
				}
			}
		}
	}
}
#endif


#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
bool GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelationsWrapperSentences(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	GIAsemRelTranslatorDatabase.getFirstRelationInSemanticParserSentenceList() = new GIArelation();

	//required for GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelations and GIAsemRelTranslatorOperationsClass::GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain:
		//code copied from GIAsemRelTranslatorParserClass::convertSentenceSemanticRelationsIntoGIAnetworkNodes/GIAtranslatorClass::convertSentenceSyntacticRelationsIntoGIAnetworkNodes:
	vector<GIAentityNode*> GIAentityNodeArray;

	vector<GIAentityNode*> GIAentityNodeArray(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));	//+1?
	setPreprocessorSentenceTranslatorEntityReferences(translatorVariables->currentPreprocessorSentenceInList, &GIAentityNodeArray);

	if(!generateSemanticRelationsFromTxtRelationsWrapper(translatorVariables))
	{
		result = false;
	}

	translatorVariables->currentSentenceInList->firstRelationInList = GIAsemRelTranslatorDatabase.getFirstRelationInSemanticParserSentenceList();
			
	return result;
}
#endif

bool GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelationsWrapper(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	GIAtxtRelTranslatorRulesGroup* firstParseTreeGroup = translatorVariables->currentPreprocessorSentenceInList->firstParseTreeGroup;
	
	GIAentityNode* semanticRelationReturnEntity = NULL;
	GIAentityNode* semanticRelationReturnEntitySubject = NULL;
	GIAentityNode* semanticRelationReturnEntityDelimiter = NULL;
	string semanticRelationReturnFunctionNameRecord = "";
	vector<GIAentityNode*> semanticRelationReturnFunctionEntityArray;

	int layer = 0;
	
	if(!generateSemanticRelationsFromTxtRelations(translatorVariables, firstParseTreeGroup, &semanticRelationReturnEntity, &semanticRelationReturnEntitySubject, &semanticRelationReturnEntityDelimiter, &semanticRelationReturnFunctionNameRecord, &semanticRelationReturnFunctionEntityArray, layer))
	{
		result = false;
	}

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ASSUME_HIGH_LEVEL_REFERENCE_SETS_DO_NOT_CONTAIN_EXPLICIT_SEMANTIC_RELATION_FUNCTION
	if(!reconcileSameReferenceSetConnectionsForAllRelationshipEntities(translatorVariables))
	{
		result = false;
	}
	#endif
	
	return result;
}

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ASSUME_HIGH_LEVEL_REFERENCE_SETS_DO_NOT_CONTAIN_EXPLICIT_SEMANTIC_RELATION_FUNCTION
bool GIAtxtRelTranslatorParserClass::reconcileSameReferenceSetConnectionsForAllRelationshipEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	//correct reconcile !sameReferenceSet connections for all relationship nodes (where only a single side; incoming/outgoing has been defined correctly as !sameReferenceSet)
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* relationshipEntity = (*translatorVariables->GIAentityNodeArray)[w];
			if(GIAentityNodeClassObject.entityIsRelationship(relationshipEntity))
			{
				GIAentityConnection* relationshipSubject = NULL;
				GIAentityNode* relationshipSubjectEntity = NULL;
				GIAentityConnection* relationshipObject = NULL;
				GIAentityNode* relationshipObjectEntity = NULL;
				bool sameReferenceSetRelationshipFound = true;
				bool subjectFound = false;
				bool objectFound = false;
				if(!(relationshipEntity->relationshipSubjectEntity->empty()))
				{
					subjectFound = true;
					relationshipSubject = (relationshipEntity->relationshipSubjectEntity->back());
					if(!(relationshipSubject->sameReferenceSet))
					{
						sameReferenceSetRelationshipFound = false;
					}
					relationshipSubjectEntity = relationshipSubject->entity;
				}
				/*
				else
				{
					cerr << "GIAtxtRelTranslatorParserClass::reconcileSameReferenceSetConnectionsForAllRelationshipEntities error; relationshipEntity->relationshipSubjectEntity->empty()" << endl;
					cerr << "relationshipEntity = " << relationshipEntity->entityName << endl;
					exit(EXIT_ERROR);
				}
				*/
				if(!(relationshipEntity->relationshipObjectEntity->empty()))
				{
					objectFound = true;
					relationshipObject = (relationshipEntity->relationshipObjectEntity->back());
					if(!(relationshipObject->sameReferenceSet))
					{
						sameReferenceSetRelationshipFound = false;
					}
					relationshipObjectEntity = relationshipObject->entity;
				}
				/*
				else
				{
					cerr << "GIAtxtRelTranslatorParserClass::reconcileSameReferenceSetConnectionsForAllRelationshipEntities error; relationshipEntity->relationshipObjectEntity->empty()" << endl;
					cerr << "relationshipEntity = " << relationshipEntity->entityName << endl;
					exit(EXIT_ERROR);
				}
				*/
				
				if(!sameReferenceSetRelationshipFound)
				{
					if(subjectFound)
					{
						relationshipSubject->sameReferenceSet = false;
						GIAentityConnection* subjectRelationshipReverse = GIAtranslatorOperations.getConnection(relationshipSubjectEntity, relationshipEntity);
						subjectRelationshipReverse->sameReferenceSet = false;
					}
					if(objectFound)
					{
						relationshipObject->sameReferenceSet = false;
						GIAentityConnection* objectRelationshipReverse = GIAtranslatorOperations.getConnection(relationshipObjectEntity, relationshipEntity);
						objectRelationshipReverse->sameReferenceSet = false;				
					}
				}
			}
		}
	}
	
	return result;
}
#endif
	
void GIAtxtRelTranslatorParserClass::setPreprocessorSentenceTranslatorEntityReferences(GIApreprocessorSentence* currentPreprocessorSentenceInList, constEffective vector<GIAentityNode*>* GIAentityNodeArray)
{
	int numberOfWordsInSentence = (currentPreprocessorSentenceInList->sentenceContentsLRP).size();	//+1?
	for(int w=GIA_NLP_START_ENTITY_INDEX; w<=numberOfWordsInSentence; w++)
	{
		((currentPreprocessorSentenceInList->sentenceContentsLRP)[GIAtranslatorOperations.convertEntityIndexToSentenceContentsIndex(w)])->translatorEntity = (*GIAentityNodeArray)[w];
	}
}
	
bool GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelations(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAentityNode** semanticRelationReturnEntity, GIAentityNode** semanticRelationReturnEntitySubject, GIAentityNode** semanticRelationReturnEntityDelimiter, string* semanticRelationReturnFunctionNameRecord, vector<GIAentityNode*>* semanticRelationReturnFunctionEntityArray, int layer)
{
	bool result = true;
	
	//TODO: write this function
	
	*semanticRelationReturnEntity = NULL;
	
	vector<GIAtxtRelTranslatorRulesComponent*> parseTreeComponentSemanticRelationFunctionListArray;
	vector<GIAtxtRelTranslatorRulesComponent*> parseTreeComponentSemanticRelationFunctionSubject(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	vector<GIAtxtRelTranslatorRulesComponent*> parseTreeComponentSemanticRelationFunctionObject(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	vector<GIAtxtRelTranslatorRulesComponent*> parseTreeComponentSemanticRelationFunctionDelimiter(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	for(int i=0; i<GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		parseTreeComponentSemanticRelationFunctionSubject[i] = NULL;
		parseTreeComponentSemanticRelationFunctionObject[i] = NULL;
		parseTreeComponentSemanticRelationFunctionDelimiter[i] = NULL;
	}
	
	vector<GIAentityNode*> entitySemanticRelationFunctionListArray;
	vector<GIAentityNode*> entitySemanticRelationFunctionSubject(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	vector<GIAentityNode*> entitySemanticRelationFunctionObject(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	vector<GIAentityNode*> entitySemanticRelationFunctionDelimiter(GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP); //= NULL;
	for(int i=0; i<GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		entitySemanticRelationFunctionSubject[i] = NULL;
		entitySemanticRelationFunctionObject[i] = NULL;
		entitySemanticRelationFunctionDelimiter[i] = NULL;
	}
			
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
	GIAentityNode* semanticRelationRecordEntitySubject = NULL;
	GIAentityNode* semanticRelationRecordEntityDelimiter = NULL;
	string semanticRelationRecordFunctionName = "";
	#endif
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	vector<GIAentityNode*> semanticRelationFunctionEntityArray;
	bool foundExplicitReturnFunctionName = false;
	bool foundImplicitReturnFunctionName = false;
	#endif
	
	for(int i=0; i<currentParseTreeGroup->components.size(); i++)
	{
		GIAtxtRelTranslatorRulesComponent* parseTreeComponent = (currentParseTreeGroup->components).at(i);
		GIAentityNode* parseTreeComponentSemanticRelationEntity = NULL;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
		GIAentityNode* parseTreeComponentSemanticRelationEntitySubject = NULL;
		GIAentityNode* parseTreeComponentSemanticRelationEntityDelimiter = NULL;
		string parseTreeComponentSemanticRelationFunctionName = "";		
		#endif
		
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
		GIAtxtRelTranslatorRules.printComponent(parseTreeComponent, layer);
		#endif
		
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
		string parseTreeComponentSemanticRelationReturnFunctionName = "";
		int parseTreeComponentSemanticRelationReturnFunctionNameIndexType = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN;
		GIAentityNode* parseTreeComponentSemanticRelationReturnFunctionNameBaseEntityRecord = NULL;	//use this as a pointer to the entity corresponding to parseTreeComponentSemanticRelationReturnFunctionNameIndexType in components of the current group	//if present will set parseTreeComponentSemanticRelationReturnFunctionNamePass to false, else will set parseTreeComponentSemanticRelationReturnFunctionNamePass to true
		vector<GIAentityNode*> parseTreeComponentSemanticRelationEntityArray;
		#endif
		
		if(parseTreeComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_GROUP)
		{
			if(parseTreeComponent->parseTreeGroupRef != NULL)
			{
				if(!generateSemanticRelationsFromTxtRelations(translatorVariables, parseTreeComponent->parseTreeGroupRef, &parseTreeComponentSemanticRelationEntity, &parseTreeComponentSemanticRelationEntitySubject, &parseTreeComponentSemanticRelationEntityDelimiter, &parseTreeComponentSemanticRelationFunctionName, &parseTreeComponentSemanticRelationEntityArray, layer+1))
				{
					result = false;
				}
			}
		}
		else if(parseTreeComponent->componentType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_COMPONENTTYPE_STRING)
		{
			GIApreprocessorPlainTextWord* parseTreeComponentWord = parseTreeComponent->candidateStringMatch;
			parseTreeComponentSemanticRelationEntity = parseTreeComponentWord->translatorEntity;
		}
		else
		{
			cerr << "GIAtxtRelTranslatorParserClass::generateRulesGroupTreeComponent{} error: parseTreeComponent->componentType illdefined" << endl;
			exit(EXIT_ERROR);
		}
		
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
		if(parseTreeComponent->queryComparisonVariable)
		{
			translatorVariables->currentSentenceInList->isQuestion = true;
			parseTreeComponentSemanticRelationEntity->isQuery = true;	//CHECKTHIS
			//#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
			parseTreeComponentSemanticRelationEntity->entityName = GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
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
			parseTreeComponentSemanticRelationEntity->isQuery = true;
			parseTreeComponentSemanticRelationEntity->isWhichOrEquivalentWhatQuery = true;
		}
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_ALIASES
		else if(parseTreeComponent->isNameQuery)
		{
			translatorVariables->currentSentenceInList->isQuestion = true;
			parseTreeComponentSemanticRelationEntity->isQuery = true;
			parseTreeComponentSemanticRelationEntity->isNameQuery = true;
		}
		if(parseTreeComponent->isName)
		{
			parseTreeComponentSemanticRelationEntity->isName = true;
		}
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
		if(parseTreeComponent->isNumberOf)
		{
			parseTreeComponentSemanticRelationEntity->isNumberOf = true;
		}
		#endif
		
		for(int j=0; j<GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; j++)
		{
			//if((currentParseTreeGroup->semanticRelationFunctionName)[j] != "")
			//{	
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);	
				cout << "\e[31m (currentParseTreeGroup->semanticRelationFunctionName)[j] = " << (currentParseTreeGroup->semanticRelationFunctionName)[j] << "\e[0m" << endl; 
				#endif
					
				if((parseTreeComponent->semanticRelationIndexType)[j] == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_LIST)
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
					cout << "\e[31m GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_LIST" << "\e[0m" << endl;
					#endif
					parseTreeComponentSemanticRelationFunctionListArray.push_back(parseTreeComponent);
					entitySemanticRelationFunctionListArray.push_back(parseTreeComponentSemanticRelationEntity);
				}
				else if((parseTreeComponent->semanticRelationIndexType)[j] == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_SUBJECT)
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
					cout << "\e[31m 1 entitySemanticRelationFunctionSubject[j] = " << parseTreeComponentSemanticRelationEntity->entityName << "\e[0m" << endl;
					#endif
					parseTreeComponentSemanticRelationFunctionSubject[j] = parseTreeComponent;
					entitySemanticRelationFunctionSubject[j] = parseTreeComponentSemanticRelationEntity;
				}
				else if((parseTreeComponent->semanticRelationIndexType)[j] == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_OBJECT)
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
					cout << "\e[31m 2 entitySemanticRelationFunctionObject[j] = " << parseTreeComponentSemanticRelationEntity->entityName << "\e[0m" << endl;
					#endif
					parseTreeComponentSemanticRelationFunctionObject[j] = parseTreeComponent;
					entitySemanticRelationFunctionObject[j] = parseTreeComponentSemanticRelationEntity;
				}
				else if((parseTreeComponent->semanticRelationIndexType)[j] == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER)
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
					cout << "\e[31m 3 entitySemanticRelationFunctionDelimiter[j] = " << parseTreeComponentSemanticRelationEntity->entityName << "\e[0m" << endl;
					#endif
					parseTreeComponentSemanticRelationFunctionDelimiter[j] = parseTreeComponent;
					entitySemanticRelationFunctionDelimiter[j] = parseTreeComponentSemanticRelationEntity;
				}
				else
				{
					//cerr << "GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelations{} warning: (currentParseTreeGroup->semanticRelationFunctionName)[j] is undefined; parseTreeComponent->semanticRelationIndexType)[j] = " << (parseTreeComponent->semanticRelationIndexType)[j] << endl;
				}
			//}
		}
		
		if(parseTreeComponent->semanticRelationReturnEntity)
		{
			*semanticRelationReturnEntity = parseTreeComponentSemanticRelationEntity;
			
			if(parseTreeComponentSemanticRelationEntity == NULL)
			{
				cerr << "GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelations{} error; (parseTreeComponent->semanticRelationReturnEntity) && (parseTreeComponentSemanticRelationEntity == NULL)" << endl;
				exit(EXIT_ERROR);
			}
	
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "*semanticRelationReturnEntity = " << (*semanticRelationReturnEntity)->entityName << endl;
			#endif
		}


		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
		if(parseTreeComponent->semanticRelationRecord)
		{
			semanticRelationRecordFunctionName = parseTreeComponentSemanticRelationFunctionName;
			semanticRelationRecordEntitySubject = parseTreeComponentSemanticRelationEntitySubject;
			semanticRelationRecordEntityDelimiter = parseTreeComponentSemanticRelationEntityDelimiter;
		}	
		if(parseTreeComponent->semanticRelationConnect)
		{
			if(semanticRelationRecordEntitySubject != NULL)
			{
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_TREAT_CONJUNCTION_ITEMS_AS_SAME_REFERENCE_SET
				bool sameReferenceSet = true;
				#else
				bool sameReferenceSet = false;
				#endif
				createSemanticRelationOrdered(translatorVariables, semanticRelationRecordFunctionName, "", NULL, semanticRelationRecordEntitySubject, parseTreeComponentSemanticRelationEntity, semanticRelationRecordEntityDelimiter, sameReferenceSet, layer);
			}
			else
			{
				cerr << "GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelations{} error: (parseTreeComponent->semanticRelationConnect) && (semanticRelationRecordEntity == NULL)" << endl;
				exit(EXIT_ERROR);
			}
		}
		#endif
		
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
		if(parseTreeComponent->semanticRelationReturnEntity && parseTreeComponent->semanticRelationReturnFunctionName != "")	//first/lowest level layer; defines both semanticRelationReturnFunctionNameIndexType and semanticRelationReturnFunctionName
		{
			foundExplicitReturnFunctionName = true;
			semanticRelationFunctionEntityArray.push_back(parseTreeComponentSemanticRelationEntity);
			parseTreeComponentSemanticRelationEntity->semanticRelationReturnFunctionName = parseTreeComponent->semanticRelationReturnFunctionName;
			parseTreeComponentSemanticRelationEntity->semanticRelationReturnFunctionNameIndexType = parseTreeComponent->semanticRelationReturnFunctionNameIndexType;

			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_DYNAMIC_INDEX_TESTS
			if(parseTreeComponent->semanticRelationReturnFunctionNameIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER_OR_SUBJECT)
			{
				int semanticRelationReturnFunctionNameIndexTypeDynamic = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER;
				int nearestPreceedingDelimiterIndex = INT_DEFAULT_VALUE; 
				int nearestPreceedingSubjectIndex = INT_DEFAULT_VALUE;
				for(int w = parseTreeComponentSemanticRelationEntity->entityIndexTemp; w>=GIA_NLP_START_ENTITY_INDEX; w--)
				{	
					GIApreprocessorPlainTextWord* currentWord = (translatorVariables->currentPreprocessorSentenceInList)->sentenceContentsLRP[GIAtranslatorOperations.convertEntityIndexToSentenceContentsIndex(w)];
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
						semanticRelationReturnFunctionNameIndexTypeDynamic = GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_SUBJECT;
					}
				}
				//cout << "semanticRelationReturnFunctionNameIndexTypeDynamic = " << semanticRelationReturnFunctionNameIndexTypeDynamic << endl;
				parseTreeComponentSemanticRelationEntity->semanticRelationReturnFunctionNameIndexType = semanticRelationReturnFunctionNameIndexTypeDynamic;
			}
			#endif
			
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "\e[32m foundExplicitReturnFunctionName \e[0m" << endl;
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "\e[32m parseTreeComponentSemanticRelationEntity->entityName = " << parseTreeComponentSemanticRelationEntity->entityName << "\e[0m" << endl;
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "\e[32m parseTreeComponentSemanticRelationEntity->semanticRelationReturnFunctionName = " << parseTreeComponentSemanticRelationEntity->semanticRelationReturnFunctionName << "\e[0m" << endl;
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "\e[32m parseTreeComponentSemanticRelationEntity->semanticRelationReturnFunctionNameIndexType = " << parseTreeComponentSemanticRelationEntity->semanticRelationReturnFunctionNameIndexType << "\e[0m" << endl;
			#endif
				
			if(parseTreeComponent->semanticRelationReturnFunctionNameIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN)
			{				
				cerr << "GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelations{} error: (parseTreeComponent->foundExplicitReturnFunctionName != "") && (parseTreeComponent->semanticRelationReturnFunctionNameIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_UNKNOWN)" << endl;
				exit(EXIT_ERROR);	
			}
		}
		if(!(parseTreeComponentSemanticRelationEntityArray.empty()))	//intermediary (hidden layer); does not define semanticRelationReturnFunctionNameIndexType and semanticRelationReturnFunctionName (take from lower layer)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			cout << "\e[32m foundImplicitReturnFunctionName" << "\e[0m" << endl;
			for(int i=0; i<parseTreeComponentSemanticRelationEntityArray.size(); i++)
			{
				GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
				cout << "\e[32m parseTreeComponentSemanticRelationEntityArray[i] = " << parseTreeComponentSemanticRelationEntityArray[i]->entityName << "\e[0m" << endl;
			}			
			#endif
			
			foundImplicitReturnFunctionName = true;
			semanticRelationReturnFunctionEntityArray->insert(semanticRelationReturnFunctionEntityArray->end(), parseTreeComponentSemanticRelationEntityArray.begin(), parseTreeComponentSemanticRelationEntityArray.end());	//append parseTreeComponentSemanticRelationEntityArray to semanticRelationReturnFunctionEntityArray
			//*semanticRelationReturnFunctionEntityArray = parseTreeComponentSemanticRelationEntityArray;
		}
		#endif
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME
	if(foundImplicitReturnFunctionName)
	{	
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_SPECIAL_FUNCTION_TESTS
		bool specialFunctionTests = true;	
		if(currentParseTreeGroup->groupName == "queryWhere") 
		{
			specialFunctionTests = false;
		}
		if(specialFunctionTests)
		{
		#endif
			for(vector<GIAentityNode*>::iterator iter1 = semanticRelationReturnFunctionEntityArray->begin(); iter1 != semanticRelationReturnFunctionEntityArray->end(); )
			{
				GIAentityNode* tempEntity = *iter1;
				bool erasedElement = false;

				/*
				if(currentParseTreeGroup->groupTypeName == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_SUBREFERENCESETAPPENDS])
				{
					tempEntity->semanticRelationReturnFunctionNameSameReferenceSet = true;
				}
				*/

				//cout << "\n\n currentParseTreeGroup->groupTypeNameBackup = " << currentParseTreeGroup->groupTypeNameBackup << endl;
				//cout << "currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << endl;
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ASSUME_HIGH_LEVEL_REFERENCE_SETS_DO_NOT_CONTAIN_EXPLICIT_SEMANTIC_RELATION_FUNCTION
				bool sameReferenceSet = false;
				if((currentParseTreeGroup->groupTypeNameBackup == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETS]) ||
				(currentParseTreeGroup->groupTypeNameBackup == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_REFERENCESETS]))
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

				if(tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_LIST)
				{
					cerr << "GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelations{} error: Illegal semanticRelationReturnFunctionNameIndexType; (tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_LIST)" << endl;
					exit(EXIT_ERROR);
				}
				else if(tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_SUBJECT)
				{
					if(entitySemanticRelationFunctionSubject[GIA_TXT_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX] != NULL)
					{
						if(entitySemanticRelationFunctionSubject[GIA_TXT_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX]->entityIndexTemp < tempEntity->entityIndexTemp)
						{
							//bool sameReferenceSet = tempEntity->semanticRelationReturnFunctionNameSameReferenceSet;
							createSemanticRelationPair(translatorVariables, tempEntity->semanticRelationReturnFunctionName, entitySemanticRelationFunctionSubject[GIA_TXT_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX], tempEntity, sameReferenceSet, layer);
							iter1 = semanticRelationReturnFunctionEntityArray->erase(iter1);
							erasedElement = true;
						}
					}
				}
				else if(tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_OBJECT)
				{
					cerr << "GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelations{} error: Illegal semanticRelationReturnFunctionNameIndexType; (tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_OBJECT)" << endl;
					exit(EXIT_ERROR);				
				}
				else if(tempEntity->semanticRelationReturnFunctionNameIndexType == GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_COMPONENT_SEMANTIC_RELATION_INDEX_TYPE_DELIMITER)
				{
					#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
					GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
					cout << "\e[34m want to createSemanticRelationPair: tempEntity->semanticRelationReturnFunctionName = " << tempEntity->semanticRelationReturnFunctionName << "\e[0m" << endl;
					GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
					cout << "\e[34m currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << "\e[0m" << endl;
					#endif
					if(entitySemanticRelationFunctionDelimiter[GIA_TXT_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX] != NULL)
					{
						if(entitySemanticRelationFunctionDelimiter[GIA_TXT_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX]->entityIndexTemp < tempEntity->entityIndexTemp)
						{
							//bool sameReferenceSet = tempEntity->semanticRelationReturnFunctionNameSameReferenceSet;
							createSemanticRelationPair(translatorVariables, tempEntity->semanticRelationReturnFunctionName, entitySemanticRelationFunctionDelimiter[GIA_TXT_REL_TRANSLATOR_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_INDEX_TYPE_SEMANTIC_FUNCTION_EXECUTION_INDEX], tempEntity, sameReferenceSet, layer);
							iter1 = semanticRelationReturnFunctionEntityArray->erase(iter1);
							erasedElement = true;
						}
					}	
				}
				else
				{
					cerr << "GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelations{} error: tempEntity->semanticRelationReturnFunctionNameIndexType is illdefined;" << tempEntity->semanticRelationReturnFunctionNameIndexType << endl;
					exit(EXIT_ERROR);
				}

				if(!erasedElement)
				{
					iter1++;
				}
			}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RETURN_FUNCTION_NAME_SPECIAL_FUNCTION_TESTS
		}
		#endif
	}
	semanticRelationReturnFunctionEntityArray->insert(semanticRelationReturnFunctionEntityArray->end(), semanticRelationFunctionEntityArray.begin(), semanticRelationFunctionEntityArray.end());	//append semanticRelationFunctionEntityArray (ie new elements defined on this layer) to semanticRelationReturnFunctionEntityArray
	#endif
				
	for(int i=0; i<GIA_TXT_REL_TRANSLATOR_MAX_NUMBER_OF_SEMANTIC_FUNCTIONS_EXECUTED_PER_GROUP; i++)
	{
		if((currentParseTreeGroup->semanticRelationFunctionName)[i] != "")
		{	
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_SEMANTIC_RELATION_RECORD_AND_CONNECT
			*semanticRelationReturnFunctionNameRecord = currentParseTreeGroup->semanticRelationFunctionName[0];	//always pass the first executed function details
			*semanticRelationReturnEntitySubject = entitySemanticRelationFunctionSubject[0];	//always pass the first executed function details
			*semanticRelationReturnEntityDelimiter = entitySemanticRelationFunctionDelimiter[0];	//always pass the first executed function details
			#endif

			bool sameReferenceSet = false;
			//cout << "\n\n currentParseTreeGroup->groupTypeNameBackup = " << currentParseTreeGroup->groupTypeNameBackup << endl;
			//cout << "currentParseTreeGroup->groupName = " << currentParseTreeGroup->groupName << endl;
			if((currentParseTreeGroup->groupTypeNameBackup == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_LOGICREFERENCESETS]) ||
			(currentParseTreeGroup->groupTypeNameBackup == GIAtxtRelTranslatorRulesGroupsTypes[GIA_TXT_REL_TRANSLATOR_RULES_GROUPS_TYPE_REFERENCESETS]))
			{
				sameReferenceSet = false;
			}
			else
			{
				sameReferenceSet = true;
			}

			
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
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
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
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
			
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
				//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
				//cout << "\e[33m **** createSemanticRelationOrdered{}: currentParseTreeGroup->semanticRelationFunctionName[i] = " << currentParseTreeGroup->semanticRelationFunctionName[i] << "\e[0m" << endl;
					//OLD: GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunctionSubject[i]->entityName, entitySemanticRelationFunctionObject[i]->entityName, currentParseTreeGroup->semanticRelationFunctionName[i], entitySemanticRelationFunctionSubject[i]->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunctionObject[i]->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
				#endif
				
				createSemanticRelationOrdered(translatorVariables, currentParseTreeGroup->semanticRelationFunctionName[i], currentParseTreeGroup->semanticRelationFunctionConditionNewName, &entitySemanticRelationFunctionListArray, entitySemanticRelationFunctionSubject[i], entitySemanticRelationFunctionObject[i], entitySemanticRelationFunctionDelimiter[i], sameReferenceSet, layer);
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_OPTIONAL
			}
			#endif
		}
	}
	
	return result;
}

bool GIAtxtRelTranslatorParserClass::createSemanticRelationOrdered(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunctionSubject, GIAentityNode* entitySemanticRelationFunctionObject, GIAentityNode* entitySemanticRelationFunctionDelimiter, const bool sameReferenceSet, int layer)
{
	if(entitySemanticRelationFunctionListArray->empty())
	{
		if(entitySemanticRelationFunctionSubject == NULL)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			//cout << "createSemanticRelationOrdered{}: " << GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunctionObject->entityName, entitySemanticRelationFunctionDelimiter->entityName, semanticRelationFunctionName, entitySemanticRelationFunctionObject->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunctionDelimiter->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
			#endif
			//cout << "entitySemanticRelationFunctionObject = " << entitySemanticRelationFunctionObject->sentenceIndexTemp << endl;
			return createSemanticRelation(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, NULL, entitySemanticRelationFunctionObject, entitySemanticRelationFunctionDelimiter, NULL, sameReferenceSet, layer);
		}
		else if(entitySemanticRelationFunctionObject == NULL)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			//cout << "createSemanticRelationOrdered{}: " << GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunctionSubject->entityName, entitySemanticRelationFunctionDelimiter->entityName, semanticRelationFunctionName, entitySemanticRelationFunctionSubject->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunctionDelimiter->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
			#endif
			//cout << "entitySemanticRelationFunctionSubject = " << entitySemanticRelationFunctionSubject->sentenceIndexTemp << endl;
			return createSemanticRelation(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, NULL, entitySemanticRelationFunctionSubject, entitySemanticRelationFunctionDelimiter, NULL, sameReferenceSet, layer);
		}
		else
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
			//cout << "createSemanticRelationOrdered{}: " << GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunctionSubject->entityName, entitySemanticRelationFunctionObject->entityName, semanticRelationFunctionName, entitySemanticRelationFunctionSubject->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunctionObject->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
			#endif
			if(entitySemanticRelationFunctionDelimiter != NULL)
			{
				#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
				cout << "\tentitySemanticRelationFunctionDelimiter = " << entitySemanticRelationFunctionDelimiter->entityName << endl;
				#endif
			}
			else
			{
				//cout << "warning (entitySemanticRelationFunctionDelimiter != NULL): semanticRelationFunctionName = " << semanticRelationFunctionName << endl;
			}
			return createSemanticRelation(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, NULL, entitySemanticRelationFunctionSubject, entitySemanticRelationFunctionObject, entitySemanticRelationFunctionDelimiter, sameReferenceSet, layer);
		}
	}
	else
	{
		#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
		//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
		//cout << "createSemanticRelationOrdered{}: " << semanticRelationFunctionName << "(entitySemanticRelationFunctionListArray)" << endl;
		#endif
		return createSemanticRelation(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, entitySemanticRelationFunctionListArray, entitySemanticRelationFunctionSubject, entitySemanticRelationFunctionObject, entitySemanticRelationFunctionDelimiter, sameReferenceSet, layer);
	}
}

bool GIAtxtRelTranslatorParserClass::createSemanticRelationPair(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet, int layer)
{
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
	//GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
	//cout << "\e[33m **** createSemanticRelationPair{}: semanticRelationFunctionName = " << semanticRelationFunctionName << "\e[0m" << endl; 
		//OLD: GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(entitySemanticRelationFunction1->entityName, entitySemanticRelationFunction2->entityName, semanticRelationFunctionName, entitySemanticRelationFunction1->semanticRelationPreprocessorEntityIndex, entitySemanticRelationFunction2->semanticRelationPreprocessorEntityIndex, sameReferenceSet) << endl;
	#endif
	return createSemanticRelation(translatorVariables, semanticRelationFunctionName, "", NULL, entitySemanticRelationFunction1, entitySemanticRelationFunction2, NULL, sameReferenceSet, layer);
}


bool GIAtxtRelTranslatorParserClass::createSemanticRelation(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet, int layer)
{
	#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
	GIAtxtRelTranslatorRules.printParseTreeDebugIndentation(layer);
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
			
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	return createSemanticRelationInMemory(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, entitySemanticRelationFunctionListArray, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet);
	#else
	return createSemanticRelationInNetwork(translatorVariables, semanticRelationFunctionName, semanticRelationFunctionConditionNewName, entitySemanticRelationFunctionListArray, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet);	
	#endif
}

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
bool GIAtxtRelTranslatorParserClass::createSemanticRelationInMemory(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet)
{
	bool result = true;
	
	int functionEntityIndex1 = entitySemanticRelationFunction1->semanticRelationPreprocessorEntityIndex;
	int functionEntityIndex2 = entitySemanticRelationFunction2->semanticRelationPreprocessorEntityIndex;
	int functionEntityIndex3 = entitySemanticRelationFunction3->semanticRelationPreprocessorEntityIndex;
	
	/*										
	if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createSubstanceInstance])
	{
	      GIAtranslatorOperations.addInstanceToInstanceDefinition(functionEntityIndex1, GIA_ENTITY_TYPE_SUBSTANCE, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createActionInstance])
	{
	      GIAtranslatorOperations.addInstanceToInstanceDefinition(functionEntityIndex1, GIA_ENTITY_TYPE_ACTION, translatorVariables);
	}
	else 
	*/
	
	if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_property])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyWithAuxiliary])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);

	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_action])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubjectReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObjectReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_condition])
	{
		/*
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED	//CHECKTHIS
		#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
		if(!(currentRelationInList->inverseRelationTwoWay))	//added 2j5e
		{
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
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
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
			}
			#endif
		}
		#endif
		#endif
		*/		
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definition])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);	
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionWithAuxiliary])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);		
	}

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_ALIASES
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAlias])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT_ALIAS, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAliasWithAuxiliary])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_ALIAS, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_ALIAS_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
		#ifndef GIA_DISABLE_ALIAS_ENTITY_MERGING
		//entitySemanticRelationFunction3->semanticRelationEntityIsReferenced = false;
		#endif
	}
	#endif

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUANTITY
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeQuantity])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		//functionEntityIndex2->semanticRelationEntityIsReferenced = false;
	}
	#endif
	
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionSubject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionObject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);		
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertySubject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyObject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObject])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubjectReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObjectReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ENTITY_GRAMMAR
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopula])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		//entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopulaSingle])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA_SINGLE, functionEntityIndex1, "", sameReferenceSet);
		//entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAuxiliary])
	{
		if(!createSemanticRelationMultiwordAuxiliary(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordPreposition])
	{
		if(!createSemanticRelationMultiwordPreposition(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordNoun])
	{
		if(!createSemanticRelationMultiwordNoun(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAlias])
	{
		if(!createSemanticRelationMultiwordAlias(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_DATE
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordDate])
	{
		if(!createSemanticRelationMultiwordDate(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	#endif
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityPredeterminer])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PREDETERMINER, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		//entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityDeterminer])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DETERMINER, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		//entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	#endif
	
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyReverse])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT, functionEntityIndex2, functionEntityIndex1, sameReferenceSet);
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_LOGIC_REFERENCES
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicDefinition])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_DEFINITION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicAction])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_ACTION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_ACTION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicCondition])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONDITION, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONDITION_REVERSE, functionEntityIndex2, functionEntityIndex3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConclusion])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONCLUSION, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConjunction])
	{
		for(int i=0; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONJUNCTION, functionEntityIndex3, ((*entitySemanticRelationFunctionListArray)[i])->semanticRelationPreprocessorEntityIndex, sameReferenceSet);
		}
	}
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionNew])
	{
		GIArelation* relation = GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_NEW, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
		relation->semanticRelationFunctionConditionNewName = semanticRelationFunctionConditionNewName;
		
		if(entitySemanticRelationFunction3 != NULL)
		{
			cout << "GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER: GIAtxtRelTranslatorParserClass::createSemanticRelationInMemory error: GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionNew not currently supported; auxiliary tense info should be connected to new condition entity" << endl;
			GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, functionEntityIndex1, functionEntityIndex3, sameReferenceSet);
			//entitySemanticRelationFunction3->semanticRelationEntityIsReferenced = false;
		}
	}	
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_MEASURE
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasure])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MEASURE, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasurePer])
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MEASURE_PER, functionEntityIndex1, functionEntityIndex2, sameReferenceSet);
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
bool GIAtxtRelTranslatorParserClass::createSemanticRelationInNetwork(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet)
{
	bool result = true;
	
	//cout << "translatorVariables->sentenceIndex = " << translatorVariables->sentenceIndex << endl;
				
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	if((semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_property]) ||
	(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyReverse]) ||
	(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definition]) ||
	(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAlias]))
	{									
		//create intermediary auxiliary 'have'/'be' (property/definition relationship entity) 
		entitySemanticRelationFunction3 = NULL;
		//FUTURE: consider using createNewRelationshipEntitySemantic instead
		if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_property])
		{
			entitySemanticRelationFunction3 = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(entitySemanticRelationFunction1, entitySemanticRelationFunction2, translatorVariables);
		}
		else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyReverse])
		{
			entitySemanticRelationFunction3 = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(entitySemanticRelationFunction2, entitySemanticRelationFunction1, translatorVariables);
		}
		else if((semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definition]) || (semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAlias]))
		{
			entitySemanticRelationFunction3 = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(entitySemanticRelationFunction1, entitySemanticRelationFunction2, translatorVariables);
		}
	}
	#endif
											
	//if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createSubstanceInstance])
	//{
	//	GIAtranslatorOperations.addInstanceToInstanceDefinition(entitySemanticRelationFunction1, GIA_ENTITY_TYPE_SUBSTANCE, translatorVariables);
	//}
	//else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_createActionInstance])
	//{
	//	GIAtranslatorOperations.addInstanceToInstanceDefinition(entitySemanticRelationFunction1, GIA_ENTITY_TYPE_ACTION, translatorVariables);
	//}
	//else 
		
	if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_property])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);									
		#endif
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyWithAuxiliary])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectPropertyToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		//GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(entitySemanticRelationFunction3);
		GIAtranslatorOperations.disableEntity(entitySemanticRelationFunction3);
		#endif
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_action])
	{
		GIAtranslatorOperations.connectActionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubject])
	{
		GIAtranslatorOperations.connectActionToSubject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObject])
	{
		GIAtranslatorOperations.connectActionToObject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionSubjectReverse])
	{
		GIAtranslatorOperations.connectActionToSubject(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_actionObjectReverse])
	{
		GIAtranslatorOperations.connectActionToObject(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_condition])
	{
		GIAtranslatorOperations.connectConditionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		
		/*
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
		if(currentRelationInList->relationTwoWay)     //limitation only works when GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_condition is called based on a single GIArelation
		{
		      entitySemanticRelationFunction3->conditionTwoWay = true;        //sets conditionTwoWay for condition substance not networkIndex
		}
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
		if(currentRelationInList->inverseRelationTwoWay)      //limitation only works when GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_condition is called based on a single GIArelation
		{
		      entitySemanticRelationFunction3->inverseConditionTwoWay = true; //sets inverseConditionTwoWay for condition substance not networkIndex
		}
		#endif
		#endif  	      
		*/
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definition])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectDefinitionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectDefinitionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);									
		#endif
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionWithAuxiliary])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectDefinitionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectDefinitionToEntity(entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);					
		//GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(entitySemanticRelationFunction3);
		GIAtranslatorOperations.disableEntity(entitySemanticRelationFunction3);
		#endif
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_ALIASES
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAlias])
	{
		if(!GIAtranslatorOperations.connectDefinitionAliasWrapper(translatorVariables, entitySemanticRelationFunction1, &entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionAliasWithAuxiliary])
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

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_QUANTITY
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeQuantity])
	{
		GIAtranslatorOperations.connectQuantityToEntity(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
		entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	#endif
	
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionSubject])
	{
		GIAtranslatorOperations.connectDefinitionToSubject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_definitionObject])
	{
		GIAtranslatorOperations.connectDefinitionToObject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertySubject])
	{
		GIAtranslatorOperations.connectPropertyToSubject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyObject])
	{
		GIAtranslatorOperations.connectPropertyToObject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubject])
	{
		GIAtranslatorOperations.connectConditionToSubject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObject])
	{
		GIAtranslatorOperations.connectConditionToObject(entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionSubjectReverse])
	{
		GIAtranslatorOperations.connectConditionToSubject(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionObjectReverse])
	{
		GIAtranslatorOperations.connectConditionToObject(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables);
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ENTITY_GRAMMAR
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopula])
	{
		/*
		cout << "GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopula" << endl;
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
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_tenseAttributeModalAuxiliaryOrCopulaSingle])
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
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAuxiliary])
	{
		if(!createSemanticRelationMultiwordAuxiliary(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordPreposition])
	{
		if(!createSemanticRelationMultiwordPreposition(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordNoun])
	{
		if(!createSemanticRelationMultiwordNoun(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordAlias])
	{
		if(!createSemanticRelationMultiwordAlias(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_DATE
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_multiwordDate])
	{
		if(!createSemanticRelationMultiwordDate(translatorVariables, entitySemanticRelationFunctionListArray, sameReferenceSet))
		{
			result = false;
		}
	}
	#endif
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityPredeterminer])
	{
		entitySemanticRelationFunction1->semanticRelationWordPredeterminer = entitySemanticRelationFunction2->entityName;
		entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_numerosityDeterminer])
	{
		entitySemanticRelationFunction1->semanticRelationWordDeterminer = entitySemanticRelationFunction2->entityName;
		entitySemanticRelationFunction2->semanticRelationEntityIsReferenced = false;
	}
	#endif
	
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_propertyReverse])
	{
		#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
		GIAtranslatorOperations.connectPropertyToEntity(entitySemanticRelationFunction2, entitySemanticRelationFunction1, entitySemanticRelationFunction3, sameReferenceSet, translatorVariables);
		#else
		GIAtranslatorOperations.connectDirectPropertyToEntity(entitySemanticRelationFunction2, entitySemanticRelationFunction1, sameReferenceSet, translatorVariables]);									
		#endif
	}
	
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_LOGIC_REFERENCES
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicDefinition])
	{
		GIAtranslatorOperations.connectLogicReference(translatorVariables, GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_DEFINITION, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicAction])
	{
		GIAtranslatorOperations.connectLogicReference(translatorVariables, GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_VERB, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicCondition])
	{
		GIAtranslatorOperations.connectLogicReference(translatorVariables, GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_PREPOSITION, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, entitySemanticRelationFunction3, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConclusion])
	{
		GIAtranslatorOperations.connectLogicReferenceConclusion(translatorVariables, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_logicConjunction])
	{
		for(int i=0; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAtranslatorOperations.connectLogicReferenceConjunction(translatorVariables, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
		}
	}
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_conditionNew])
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
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_MEASURE
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasure])
	{
		GIAtranslatorOperations.connectMeasureToEntity(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
	}
	else if(semanticRelationFunctionName == GIAtxtRelSemanticDependencyRelationNameArray[GIA_TXT_REL_TRANSLATOR_RULES_SEMANTIC_RELATION_attributeMeasurePer])
	{
		GIAtranslatorOperations.connectMeasurePerToEntity(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
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
#endif


bool GIAtxtRelTranslatorParserClass::createSemanticRelationMultiwordAuxiliary(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* lastEntityInList = (*entitySemanticRelationFunctionListArray)[entitySemanticRelationFunctionListArray->size()-1];
		for(int i=entitySemanticRelationFunctionListArray->size()-2; i>=0; i--)
		{
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];
			//cout << "currentEntityInList = " << currentEntityInList->entityName << endl;
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
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

bool GIAtxtRelTranslatorParserClass::createSemanticRelationMultiwordPreposition(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* firstEntityInList = (*entitySemanticRelationFunctionListArray)[0];
		for(int i=1; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
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


bool GIAtxtRelTranslatorParserClass::createSemanticRelationMultiwordNoun(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* lastEntityInList = (*entitySemanticRelationFunctionListArray)[entitySemanticRelationFunctionListArray->size()-1];

		//e.g. "goal line"
		for(int i=entitySemanticRelationFunctionListArray->size()-2; i>=0; i--)
		{	
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];

			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
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

bool GIAtxtRelTranslatorParserClass::createSemanticRelationMultiwordAlias(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* lastEntityInList = (*entitySemanticRelationFunctionListArray)[entitySemanticRelationFunctionListArray->size()-1];

		//e.g. "Ms. Savata"
		for(int i=entitySemanticRelationFunctionListArray->size()-2; i>=0; i--)
		{				
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];

			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
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

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_DATE
bool GIAtxtRelTranslatorParserClass::createSemanticRelationMultiwordDate(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet)
{
	bool result = true;
	
	if(entitySemanticRelationFunctionListArray->size() > 1)
	{
		GIAentityNode* firstEntityInList = (*entitySemanticRelationFunctionListArray)[0];
		
		for(int i=1; i<entitySemanticRelationFunctionListArray->size(); i++)
		{
			GIAentityNode* currentEntityInList = (*entitySemanticRelationFunctionListArray)[i];
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
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


//this code is based on GIAsemRelTranslatorParserClass::defineSubstancesBasedOnSemanticRelations
void GIAtxtRelTranslatorParserClass::defineSubstancesBasedOnNetworkAndDeterminerInfo(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int i=0; i<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); i++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[i])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[i];
			
			bool isConcept = false;
			if(entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_NOUN)
			{				
				bool hasDeterminer = false;
				bool hasPredeterminer = false;
				bool indefiniteDeterminer = false;
				if(entity->semanticRelationWordDeterminer != "")
				{
					hasDeterminer = true;
					#ifdef GIA_FEATURE_POS_TAG_NN_ONLY_MARK_AS_SINGULAR_WITH_DETERMINER
					//code from GIAtranslatorGrammarClass::fillGrammaticalArraysStanford;
					bool determinerPotentiallySingularDetected = false;
					if(SHAREDvars.textInTextArray(entity->semanticRelationWordDeterminer, relationDeterminerPotentiallySingularArray, GRAMMATICAL_DETERMINER_POTENTIALLY_SINGULAR_ARRAY_NUMBER_OF_TYPES))
					{
						determinerPotentiallySingularDetected = true;
						//code from GIAtranslatorGrammarClass::extractGrammaticalInformationFromStanfordPOStag;
						bool singularDetected = false;
						if(entity->stanfordPOStemp == FEATURE_POS_TAG_NOUN_NN)
						{
							singularDetected = true;
							entity->grammaticalNumber = GRAMMATICAL_NUMBER_SINGULAR;
						}
					}
					#endif
					if(SHAREDvars.textInTextArray(entity->semanticRelationWordDeterminer, grammaticalDeterminerIndefiniteArray, GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES))
					{
						indefiniteDeterminer = true;
					}
					if(SHAREDvars.textInTextArray(entity->semanticRelationWordDeterminer, grammaticalDeterminerDefiniteArray, GRAMMATICAL_DETERMINER_DEFINITE_NUMBER_OF_TYPES))
					{
						entity->grammaticalDefiniteTemp = true;
					}
					#ifdef GIA_PREDETERMINERS
					int arrayIndexOfResultFound = GRAMMATICAL_PREDETERMINER_UNDEFINED;
					if(SHAREDvars.textInTextArray(entity->semanticRelationWordDeterminer, entityPredeterminerSmallNameArray, GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES, &arrayIndexOfResultFound))
					{
						//cout << "entity->entityName = " << entity->entityName << endl;
						//cout << "entity->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL" << endl;
						
						//based on redistributeStanfordRelationsPredeterminers;
						hasPredeterminer = true;
						entity->grammaticalPredeterminerTemp = arrayIndexOfResultFound;
						#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS
						entity->grammaticalPredeterminerTempSentenceArray.insert(make_pair(entity->sentenceIndexTemp, entity->grammaticalPredeterminerTemp));
						#endif
						entity->grammaticalNumber = GRAMMATICAL_NUMBER_PLURAL;
					}
					#endif
				}
				#ifdef GIA_PREDETERMINERS
				if(entity->semanticRelationWordPredeterminer != "")
				{
					hasPredeterminer = true;
					int arrayIndexOfResultFound = GRAMMATICAL_PREDETERMINER_UNDEFINED;
					if(SHAREDvars.textInTextArray(entity->semanticRelationWordPredeterminer, entityPredeterminerSmallNameArray, GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES, &arrayIndexOfResultFound))
					{
						//cout << "entity->entityName = " << entity->entityName << endl;
						//cout << "entity->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL" << endl;
						
						//based on redistributeStanfordRelationsPredeterminers;
						entity->grammaticalPredeterminerTemp = arrayIndexOfResultFound;
						#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS
						entity->grammaticalPredeterminerTempSentenceArray.insert(make_pair(entity->sentenceIndexTemp, entity->grammaticalPredeterminerTemp));
						#endif
						//cout << "arrayIndexOfResultFound = " << arrayIndexOfResultFound << endl;
						entity->grammaticalNumber = GRAMMATICAL_NUMBER_PLURAL;
					}

				}
				#endif
								
				if(!hasDeterminer)
				{
					if(entity->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
					{
						isConcept = true;
					}
				}		

				//check if special case as defined in defineSubstancesBasedOnDeterminatesOfDefinitionEntities();
				//case 2. see if has an indefinite determiner (a/an) and is part of a definition connection
				//eg A book is an animal (definition connection, bat = networkIndex, animal = networkIndex)
				//eg The bat is an animal (definition connection, bat = substance, animal = networkIndex)
				if(hasDeterminer && indefiniteDeterminer)
				{
					for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION].begin(); connectionIter != entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION].end(); connectionIter++)
					{
						if((*connectionIter)->sentenceIndexTemp == translatorVariables->sentenceIndex)
						{
							GIAentityNode* entity2 = (*connectionIter)->entity;
							isConcept = true;
						}
					}
					for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE].begin(); connectionIter != entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE].end(); connectionIter++)
					{
						if((*connectionIter)->sentenceIndexTemp == translatorVariables->sentenceIndex)
						{
							GIAentityNode* entity2 = (*connectionIter)->entity;
							isConcept = true;
						}
					}
				}
			}

			if(entity->hasQuantity)
			{
				isConcept = false;
			}
			
			if(isConcept)
			{
				//cout << "isConcept: entity = " << entity->entityName << endl;
				entity->entityType = GIA_ENTITY_TYPE_CONCEPT;
			}

		}
	}
	
	//ensure that all properties of substances are substances not concepts;
	for(int i=0; i<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); i++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[i])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[i];
			if(entity->entityType == GIA_ENTITY_TYPE_CONCEPT)
			{
				bool entityHasSubstanceParent = false;
				for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE].begin(); connectionIter != entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE].end(); connectionIter++)
				{
					GIAentityNode* entity2 = GIAtranslatorOperations.getRelationshipSubjectEntity(*connectionIter);
					if(entity2->entityType == GIA_ENTITY_TYPE_SUBSTANCE)
					{
						entityHasSubstanceParent = true;
					}
				}
				
				if(entityHasSubstanceParent)
				{
					entity->entityType = GIA_ENTITY_TYPE_SUBSTANCE;
					//cout << "entityHasSubstanceParent: entity = " << entity->entityName << endl;
				}
			}
		}
	}
	
	//define actionConcepts
	//code from GIAsynRelTranslatorApplyAdvancedFeaturesClass::defineActionConcepts2
	//FUTURE: note this must be reimplemented by GIAsynRelTranslatorParserClass also
	for(int i=0; i<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); i++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[i])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[i];
			if((entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_VERB) && ((entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] == true) || (entity->grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_INFINITIVE] == true)))
			{
				//e.g. "riding"/"to ride"
				
				//Condition B1. no subject (cannot have subject)
				if(entity->relationshipSubjectEntity->empty())
				{
					//Condition B2 action can have condition/property, but cannot be used as input condition/property in sentence
					if(entity->conditionReverseNodeList->empty() && entity->propertyReverseNodeList->empty())
					{
						bool foundActionNetworkIndex = true;
						//Condition 3. object can have condition/property, but it cannot be used as an input condition/property in sentence
						if(!(entity->relationshipObjectEntity->empty()))
						{
							foundActionNetworkIndex = false;
							GIAentityNode* actionObjectentity = entity->relationshipObjectEntity->back()->entity;
							if(actionObjectentity->conditionReverseNodeList->empty() && actionObjectentity->propertyReverseNodeList->empty())
							{
								foundActionNetworkIndex = true;
							}
						}

						if(foundActionNetworkIndex)
						{
							/*
							if(entity->entityType != GIA_ENTITY_TYPE_ACTION)
							{
								//this will be the case with actionConcepts without objects
								cout << "GIAtxtRelTranslatorParserClass::defineSubstancesBasedOnNetworkAndDeterminerInfo{}: foundActionNetworkIndex && (entity->entityType != GIA_ENTITY_TYPE_ACTION)" << endl;
							}
							cout << "isActionConcept" << endl;
							*/
							entity->entityType = GIA_ENTITY_TYPE_ACTION;	//this should already be the case (considering entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_VERB)
							entity->isActionConcept = true;
						}
					}
				}
			}
		}
	}	
}


#endif

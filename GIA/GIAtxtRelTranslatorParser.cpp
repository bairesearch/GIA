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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3i2c 27-May-2019
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
	
	int numberOfWordsInSentence = GIApreprocessorSentenceClassObject.getSentenceContents(translatorVariables->currentPreprocessorSentenceInList)->size();
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


	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_UNOPTIMISED
	#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_ONLY_PEFORM_PRELIM_PROPAGATION_IF_ADVANCED_REFERENCING
	if(linkPreestablishedReferencesGIA)
	{
	#endif
	#endif
		vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers = GIAtxtRelTranslatorRules.getGIAtxtRelTranslatorRulesTokenLayersGlobal();
		vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes = GIAtxtRelTranslatorRules.getGIAtxtRelTranslatorRulesGroupTypesGlobal();
		translatorVariables->parserAllowed = false;
		if(GIAtxtRelTranslatorPermutations.executeTxtRelTranslatorWrapper2(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, translatorVariables->currentPreprocessorSentenceInList))
		{
			result = false;
		}
		translatorVariables->parserAllowed = true;
	#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_ONLY_PEFORM_PRELIM_PROPAGATION_IF_ADVANCED_REFERENCING
	}
	#endif
	#endif
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

	//cout << "numberOfWordsInSentence = " << numberOfWordsInSentence << endl;
	for(int w=GIA_NLP_START_ENTITY_INDEX; w<=numberOfWordsInSentence; w++)
	{
		GIAentityNode* entity = GIAentityNodeArray[w];
		//cout << "entity->entityName = " << entity->entityName << endl;
		((*GIApreprocessorSentenceClassObject.getSentenceContents(translatorVariables->currentPreprocessorSentenceInList))[GIAtranslatorOperations.convertEntityIndexToSentenceContentsIndex(w)])->translatorEntity = entity;	//code from setPreprocessorSentenceTranslatorEntityReferences
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
		bool disableEntity = true;
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
		if(entity->isNumberOf)
		{
			disableEntity = false;	//in case of sentences containing a single entity x without a semanticRelationFunction being executed; i.e. "the number of x"
		}
		#endif
		if(entity->semanticRelationEntityIsReferenced)
		{
			disableEntity = false;
		}
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NUMBER_EXPLETIVES
		if(entity->isExpletive)
		{
			disableEntity = false;
		}
		#endif
		if(disableEntity)
		{
			#ifdef GIA_DEBUG_TXT_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS2
			cout << "DISABLING entity = " << entity->entityName << endl;
			cout << "entity->entityType = " << entity->entityType << endl;
			#endif
			GIAtranslatorOperations.disableInstanceAndNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(entity);
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

	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_SUBJECT_MULTI_POSTHOC_COLLAPSE_CONJUNCTION_ENTITIES
	//collapseConjunctionEntities(translatorVariables);
	#endif
	
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

	int numberOfWordsInSentence = GIApreprocessorSentenceClassObject.getSentenceContents(translatorVariables->currentPreprocessorSentenceInList)->size();

	#ifdef GIA_GRAMMAR_IMPERATIVE_DETECTION
	bool toDetected = false;
	#endif
	GIAfeature* currentFeatureInList = NULL;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	currentFeatureInList = firstFeatureInSentence;
	#endif
	for(int w=GIA_NLP_START_ENTITY_INDEX; w<=numberOfWordsInSentence; w++)
	{
		GIApreprocessorPlainTextWord* currentWord = (*GIApreprocessorSentenceClassObject.getSentenceContents(translatorVariables->currentPreprocessorSentenceInList))[GIAtranslatorOperations.convertEntityIndexToSentenceContentsIndex(w)];
		
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
		#ifndef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OLD
		entity->semanticRelationWordPOStypeInferred = currentWord->wordPOStypeInferred;
		#endif
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

#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_PARSE_SIMULTANEOUS
bool GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelationsWrapper(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	vector<XMLparserTag*>* GIAtxtRelTranslatorRulesTokenLayers = GIAtxtRelTranslatorRules.getGIAtxtRelTranslatorRulesTokenLayersGlobal();
	vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes = GIAtxtRelTranslatorRules.getGIAtxtRelTranslatorRulesGroupTypesGlobal();

	if(!GIAtxtRelTranslatorPermutations.executeTxtRelTranslatorWrapper2(translatorVariables, GIAtxtRelTranslatorRulesTokenLayers, GIAtxtRelTranslatorRulesGroupTypes, translatorVariables->currentPreprocessorSentenceInList))
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
#else
bool GIAtxtRelTranslatorParserClass::generateSemanticRelationsFromTxtRelationsWrapper(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK
	if(translatorVariables->currentPreprocessorSentenceInList->firstParseTreeGroup == NULL)
	{
		//cout << "(translatorVariables->currentPreprocessorSentenceInList->firstParseTreeGroup == NULL)" << endl;
		translatorVariables->currentPreprocessorSentenceInList->firstParseTreeGroup = new GIAtxtRelTranslatorRulesGroupParseTree();
	}
	#endif
	
	GIAtxtRelTranslatorRulesGroupParseTree* firstParseTreeGroup = translatorVariables->currentPreprocessorSentenceInList->firstParseTreeGroup;
	//cout << "firstParseTreeGroup->groupName = " << firstParseTreeGroup->groupName << endl;
	
	int layer = 0;
	if(!GIAtxtRelTranslatorParserOperations.generateSemanticRelationsFromTxtRelationsWrapper(translatorVariables, firstParseTreeGroup, layer))
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
#endif

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
					cerr << "GIAtxtRelTranslatorParserOperationsClass::reconcileSameReferenceSetConnectionsForAllRelationshipEntities error; relationshipEntity->relationshipSubjectEntity->empty()" << endl;
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
					cerr << "GIAtxtRelTranslatorParserOperationsClass::reconcileSameReferenceSetConnectionsForAllRelationshipEntities error; relationshipEntity->relationshipObjectEntity->empty()" << endl;
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
	int numberOfWordsInSentence = GIApreprocessorSentenceClassObject.getSentenceContents(currentPreprocessorSentenceInList)->size();	//+1?
	for(int w=GIA_NLP_START_ENTITY_INDEX; w<=numberOfWordsInSentence; w++)
	{
		((*GIApreprocessorSentenceClassObject.getSentenceContents(currentPreprocessorSentenceInList))[GIAtranslatorOperations.convertEntityIndexToSentenceContentsIndex(w)])->translatorEntity = (*GIAentityNodeArray)[w];
	}
}


//this code is based on GIAsemRelTranslatorParserClass::defineSubstancesBasedOnSemanticRelations
void GIAtxtRelTranslatorParserClass::defineSubstancesBasedOnNetworkAndDeterminerInfo(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int i=0; i<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); i++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[i])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[i];
			
			bool isConcept = false;
			//cout << "entity->grammaticalWordTypeTemp = " << entity->grammaticalWordTypeTemp << endl;
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
						//cout << "(entity->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL) entity = " << entity->entityName << endl;
					}
					#ifdef GIA_TXT_REL_TRANSLATOR_RULES_INTERPRET_ALL_NON_PROPER_NOUNS_WITHOUT_DETERMINER_AS_CONCEPT_IE_NOT_JUST_PLURAL
					else if(!entity->grammaticalProperNounTemp)
					{
						isConcept = true;
					}
					#endif
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
			#ifdef GIA_TRANSLATOR_EXPLETIVES
			if(entity->isExpletive)
			{
				isConcept = false;
			}
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NUMBER_OF
			if(entity->isNumberOf)
			{
				isConcept = false;	//GIA3f4b
				entity->grammaticalDefiniteTemp = true;	//GIA3f4c
			}
			#endif
			
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

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_SUBJECT_MULTI_POSTHOC_COLLAPSE_CONJUNCTION_ENTITIES
void GIAtxtRelTranslatorParserClass::collapseConjunctionEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int i=0; i<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); i++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[i])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[i];
			if(entity->entityName == GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and)
			{
				cout << "GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and found" << endl;
				
				//directly connect relationEntity subject to objects
				GIAentityConnection* relationshipSubjectEntityConnection = GIAtranslatorOperations.getRelationshipSubjectEntityConnection(entity);
				//int connectionTypeReverse = relationshipSubjectEntityConnection->connectionType;
				
				GIAentityNode* relationshipSubjectEntity = GIAtranslatorOperations.getRelationshipSubjectEntity(entity);
				GIAentityNode* relationshipEntityOrig = relationshipSubjectEntity;
				//int connectionTypeReverse = generateConnectionTypeReverse(relationshipEntityType);
				
				for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT].begin(); connectionIter != entity->entityVectorConnectionsArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT].end(); connectionIter++)
				{	
					GIAentityNode* relationshipObjectEntity = (*connectionIter)->entity;
					
					deleteRelationshipEntity(relationshipSubjectEntity, relationshipObjectEntity, entity);
					
					int connectionTypeTargetToRelationship = GIAtranslatorOperations.generateConnectionTypeReverse(relationshipEntityOrig->entityType);
					bool sameReferenceSet = relationshipSubjectEntityConnection->sameReferenceSet;
					GIAtranslatorOperations.connectEntities(relationshipEntityOrig, relationshipObjectEntity, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, connectionTypeTargetToRelationship, sameReferenceSet, translatorVariables);		
				}
			}
		}
	}
}

//this general function should be moved;
void GIAtxtRelTranslatorParserClass::deleteRelationshipEntity(GIAentityNode* relationshipSubjectEntity, GIAentityNode* relationshipObjectEntity, GIAentityNode* relationshipEntity)
{
	for(int connectionType = 0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{ 
		for(vector<GIAentityConnection*>::iterator connectionIter = relationshipSubjectEntity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != relationshipSubjectEntity->entityVectorConnectionsArray[connectionType].end(); )
		{	
			GIAentityNode* tempEntity = (*connectionIter)->entity;	
			//int connectionTypeTargetToRelationship = (*connectionIter)->connectionType;	//not used; this is rederived
			if(tempEntity == relationshipEntity)
			{
				connectionIter = relationshipObjectEntity->entityVectorConnectionsArray[connectionType].erase(connectionIter);
			}
			else
			{
				connectionIter++;
			}
		}
	}
	for(int connectionType = 0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{ 
		for(vector<GIAentityConnection*>::iterator connectionIter = relationshipObjectEntity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != relationshipObjectEntity->entityVectorConnectionsArray[connectionType].end(); )
		{	
			GIAentityNode* tempEntity = (*connectionIter)->entity;	
			if(tempEntity == relationshipEntity)
			{
				connectionIter = relationshipObjectEntity->entityVectorConnectionsArray[connectionType].erase(connectionIter);
			}
			else
			{
				connectionIter++;
			}		
		}
	}
	
	GIAtranslatorOperations.disableEntity(relationshipEntity);	//entity no longer used and should be deleted
	
}
#endif
	

#endif

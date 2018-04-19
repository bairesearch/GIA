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
 * File Name: GIAsemRelTranslatorParser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f2n 04-April-2018
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 * Description: Semantic Relation Translator Parser
 * /
 *******************************************************************************/


#include "GIAsemRelTranslatorParser.hpp"
#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
#endif

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_SEM_REL_TRANSLATOR

//based on convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}:
bool GIAsemRelTranslatorParserClass::convertSentenceSemanticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList)
{
	bool result = true;
	
	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList) + SENTENCE_FIRST_ARTIFICIAL_INDEX;

	/*
	cout << "TEST" << endl;
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			cout << "relationType = " << currentRelationInList->relationType << endl;
			cout << "relationGoverner = " << currentRelationInList->relationGovernor << endl;
			cout << "relationDependent = " << currentRelationInList->relationDependent << endl;
			cout << "relationGovernerIndex = " << currentRelationInList->relationGovernorIndex << endl;
			cout << "relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
		}
		currentRelationInList = currentRelationInList->next;
	}
	*/

	vector<bool> GIAentityNodeArrayFilled(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	vector<GIAentityNode*> GIAnetworkIndexNodeArray(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	vector<GIAentityNode*> GIAentityNodeArray(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	#ifdef GIA_SEM_REL_TRANSLATOR_GIA2_USE_SYN_REL_TRANSLATOR_FEATURES
	vector<GIAfeature*> featureArrayTemp(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	#endif
	translatorVariables->GIAentityNodeArrayFilled = &GIAentityNodeArrayFilled;
	translatorVariables->GIAnetworkIndexNodeArray = &GIAnetworkIndexNodeArray;	
	translatorVariables->GIAentityNodeArray = &GIAentityNodeArray;
	#ifdef GIA_SEM_REL_TRANSLATOR_GIA2_USE_SYN_REL_TRANSLATOR_FEATURES
	translatorVariables->featureArrayTemp = &featureArrayTemp;
	#endif
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAnetworkIndexNodeArray[w] = NULL;
		GIAentityNodeArray[w] = NULL;
		#ifdef GIA_SEM_REL_TRANSLATOR_GIA2_USE_SYN_REL_TRANSLATOR_FEATURES
		featureArrayTemp[w] = NULL;
		#endif
	}

	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	GIAbot.botSwitchFirstAndSecondPersonSemantic(translatorVariables);
	#endif

	locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(translatorVariables);

#ifdef GIA_SEM_REL_TRANSLATOR_GIA2_USE_SYN_REL_TRANSLATOR_FEATURES
	GIAtranslatorOperations.generateTempFeatureArray(translatorVariables, featureArrayTemp);	//regeneration required for Relex in case query variables detected

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
		fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(translatorVariables);
	}
	#endif

 	GIAtranslatorGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, translatorVariables->currentSentenceInList->firstFeatureInList);

	GIAsynRelTranslatorRedistributeRelations.redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(translatorVariables);
#else
	GIAtranslatorGrammar.calculateGrammarUsingInferredPosTypes(translatorVariables);
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
		#ifdef GIA_SEM_REL_TRANSLATOR_GIA2_USE_SYN_REL_TRANSLATOR_FEATURES
		//is pronoun referencing supported by GIA2?

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
		
		#endif
	#ifdef GIA_ADVANCED_REFERENCING
	}
	#endif
#endif


	defineSubstancesBasedOnSemanticRelations(translatorVariables);

	#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
	identifyComparisonVariableBasedOnSemanticRelations(translatorVariables);
	#endif


	defineConnectionsBasedOnSemanticRelations(translatorVariables);

	#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEM_REL_TRANSLATOR_SEMANTIC_RELATION_GENERATION
	GIAsynRelTranslatorLinkEntitiesDynamic.linkEntitiesDynamic(translatorVariables);
	#endif


	applyAdvancedFeaturesBasedOnSemanticRelations(translatorVariables);


	vector<GIAentityNode*>* entityNodesActiveListSentence = new vector<GIAentityNode*>;
	if(!convertSentenceSemanticRelationsIntoGIAnetworkNodesEndPart1(translatorVariables, entityNodesActiveListSentence))
	{
		result = false;
	}
	if(!GIAsynRelTranslatorParser.convertSentenceRelationsIntoGIAnetworkNodesEndPart2(translatorVariables, entityNodesActiveListSentence))
	{
		result = false;
	}
		
		
	return result;
}


void GIAsemRelTranslatorParserClass::locateAndAddAllNetworkIndexEntitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	//code from locateAndAddAllFeatureTempEntities{}:

	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		bool isDependencyRelationSecondary = false;
		for(int i=0; i<GIA_SEM_REL_TRANSLATOR_SEMANTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES; i++)
		{
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationSecondaryNameArray[i])
			{
				isDependencyRelationSecondary = true;
			}
		}

		string name[2];
		name[0] = currentRelationInList->relationGovernor;
		name[1] = currentRelationInList->relationDependent; 	//argumentName

		int relationIndex[2];
		relationIndex[0] = currentRelationInList->relationGovernorIndex;
		relationIndex[1] = currentRelationInList->relationDependentIndex;

		for(int i=0; i<2; i++)
		{
			if(!(*translatorVariables->GIAentityNodeArrayFilled)[relationIndex[i]])
			{
				(*translatorVariables->GIAentityNodeArrayFilled)[relationIndex[i]] = true;

				//code from locateAndAddAllNetworkIndexEntities{}:

				bool entityAlreadyExistant = false;
				GIAentityNode* networkIndexEntity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&(name[i]), &entityAlreadyExistant, translatorVariables);
				(*translatorVariables->GIAnetworkIndexNodeArray)[relationIndex[i]] = networkIndexEntity;

				if(isDependencyRelationSecondary)
				{
					if(i == 1)
					{
						networkIndexEntity->disabled = true;	//disable is/have etc nodes
					}
				}

				networkIndexEntity->hasAssociatedInstanceTemp = false;

				translatorVariables->sentenceNetworkIndexEntityNodesList->push_back(networkIndexEntity);

				/*
				#ifdef GIA_SEM_REL_TRANSLATOR_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
				if(relationIndex[i] >= (translatorVariables->currentSentenceInList->numberOfWordsInSentence))
				{
					networkIndexEntity->wordOrig = networkIndexEntity->entityName;

					GIAfeature* currentFeatureInList = translatorVariables->currentSentenceInList->firstFeatureInList;
					bool foundFeature = false;
					while(currentFeatureInList->next != NULL)
					{
						if((currentFeatureInList->lemma == name[i]) && (currentFeatureInList->entityIndex == relationIndex[i]))
						{
							foundFeature = true;
						}
						currentFeatureInList = currentFeatureInList->next;
					}
					if(!foundFeature)
					{
						currentFeatureInList->word = name[i];
						currentFeatureInList->lemma = name[i];
						currentFeatureInList->entityIndex = relationIndex[i];
						GIAfeature* newFeature = new GIAfeature();
						currentFeatureInList->next = newFeature;
					}
				}
				#endif
				*/
				
				
				#ifndef GIA_SEM_REL_TRANSLATOR_GIA2_USE_SYN_REL_TRANSLATOR_FEATURES
				#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
				GIApreprocessorWord* currentWord = (translatorVariables->currentPreprocessorSentenceInList)->sentenceContentsLRP[relationIndex[i]];
				networkIndexEntity->semanticRelationWordPOStypeInferred = currentWord->wordPOStypeInferred;
				#else
				GIAfeature* currentFeatureInList = translatorVariables->currentSentenceInList->firstFeatureInList;
				while(currentFeatureInList->next != NULL)
				{
					if((currentFeatureInList->lemma == name[i]) && (currentFeatureInList->entityIndex == relationIndex[i]))
					{
						networkIndexEntity->semanticRelationWordPOStypeInferred = currentFeatureInList->GIAsemRelTranslatorPOStype;	//this is not currently used
					}
					currentFeatureInList = currentFeatureInList->next;
				}
				#endif
				#endif
					
				/*
				//temporarily disabled this code;
				//TODO: detect isQuery and negative from GIA semanticRelation argument (yet to implement)

				#ifndef GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
				if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)	//ie if(NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP)
				{
					if(currentRelationInList->isQuery)		//TODO: detect isQuery from GIA semanticRelation argument (yet to implement)
					{
						networkIndexEntity->isQuery = true;
						setFoundComparisonVariable(true);
						setComparisonVariableNode(networkIndexEntity);
					}
				}
				#endif

				#ifdef GIA_SYN_REL_TRANSLATOR_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
				//this is required because negative assignment is now performed during redistribution
				if(currentRelationInList->negative)	//TODO: detect negative from GIA semanticRelation argument (yet to implement)
				{
					networkIndexEntity->negative = true;
				}
				*/

				/*
				#ifdef GIA_DEBUG
				//cout << "filling: " << relationIndex[i] << " " << name[i] << endl;
				#endif
				#ifdef GIA_ADVANCED_REFERENCING
				//this is required for fillGrammaticalArraysStanford findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet()	[nb these values are applied to networkIndex entities only]
				GIAnetworkIndexNodeArray[relationIndex[i]]->entityIndexTemp = relationIndex[i];
				GIAnetworkIndexNodeArray[relationIndex[i]]->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
				#endif
				*/
			}
		}

		currentRelationInList = currentRelationInList->next;
	}
	
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		GIAentityNodeArray[w] = GIAnetworkIndexNodeArray[w];		//set default values of GIAentityNodeArray
	}
}




void GIAsemRelTranslatorParserClass::fillGrammaticalTenseArraysStanfordBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList;

	currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA])
			{
				//modal auxiliary [/copula] found; eg modalAuxiliary(sad-3, was-2) / eg modalAuxiliary(had-5, must-3) / modalAuxiliary(had-5, have-4) The dog must have had it.
				int thingIndex = currentRelationInList->relationGovernorIndex;
				int modalAuxiliaryIndex = currentRelationInList->relationDependentIndex;
				GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[thingIndex];
				string auxiliaryOrCopulaString = (*translatorVariables->GIAentityNodeArray)[modalAuxiliaryIndex]->wordOrig;	//featureArrayTemp[modalAuxiliaryIndex]->word;

				updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(entity, auxiliaryOrCopulaString);
				currentRelationInList->disabled = true;
			}
		}
		currentRelationInList = currentRelationInList->next;
	}


}

void GIAsemRelTranslatorParserClass::defineSubstancesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAsynRelTranslatorDefineSubstances.defineSubstancesAllNodes(translatorVariables);

	#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
	GIAsynRelTranslatorDefineSubstances.defineSubstancesActionNetworkIndexes(translatorVariables);
	#endif

	for(int i=0; i<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); i++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[i])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[i];
			bool isConcept = false;
			if(entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_NOUN)
			{
				bool hasDeterminer = false;
				bool indefiniteDeterminer = false;
				string determinerString = "";
				GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 				while(currentRelationInList->next != NULL)
				{
					if(!(currentRelationInList->disabled))
					{
						if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DETERMINER])
						{
							int thingIndex = currentRelationInList->relationGovernorIndex;
							int determinerIndex = currentRelationInList->relationDependentIndex;
							if(thingIndex == i)
							{
								hasDeterminer = true;
								determinerString = (*translatorVariables->GIAentityNodeArray)[determinerIndex]->entityName;
								currentRelationInList->disabled = true;
								if(SHAREDvars.textInTextArray(determinerString, grammaticalDeterminerIndefiniteArray, GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES))
								{
									indefiniteDeterminer = true;
								}
							}
						}
					}
					currentRelationInList = currentRelationInList->next;
				}

				//case 1. see if is plural and has no determiner
				//eg Bats are animals.  (definition connection, bat = networkIndex, animal = networkIndex)	/ bats ride bikes
				if(!hasDeterminer)
				{
					if(entity->grammaticalNumber == GRAMMATICAL_NUMBER_PLURAL)
					{
						isConcept = true;
					}
					/*
					//original GIA implementation uses isProperNoun detection instead of isPlural detection; see defineSubstancesBasedOnDeterminatesOfDefinitionEntities()
					if(!(entity->grammaticalIsProperNoun))
					{
						isConcept = true;
					}
					*/
				}

				//check if special case as defined in defineSubstancesBasedOnDeterminatesOfDefinitionEntities();
				//case 2. see if has an indefinite determiner (a/an) and is part of a definition connection
				//eg A book is an animal (definition connection, bat = networkIndex, animal = networkIndex)
				//eg The bat is an animal (definition connection, bat = substance, animal = networkIndex)
				if(hasDeterminer && indefiniteDeterminer)
				{
					GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 					while(currentRelationInList->next != NULL)
					{
						if(!(currentRelationInList->disabled))
						{
							if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION])
							{
								int thingIndex = currentRelationInList->relationGovernorIndex;
								int definitionIndex = currentRelationInList->relationDependentIndex;
								if((thingIndex == i) || (definitionIndex == i))
								{
									isConcept = true;
								}
							}
						}
						currentRelationInList = currentRelationInList->next;
					}
				}
			}

			if(isConcept)
			{
				(*translatorVariables->GIAentityNodeArray)[i]->entityType = GIA_ENTITY_TYPE_CONCEPT;
			}
		}
	}
}

#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
void GIAsemRelTranslatorParserClass::identifyComparisonVariableBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	if(translatorVariables->currentSentenceInList->isQuestion)
	{
		for(int i=0; i<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); i++)
		{
			if((*translatorVariables->GIAentityNodeArrayFilled)[i])
			{
				GIAentityNode* entityNode = (*translatorVariables->GIAentityNodeArray)[i];
				if(entityNode->entityName == GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
				{
					entityNode->isQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
			}
		}
	}

	if(translatorVariables->currentSentenceInList->isQuestion)
	{
		const GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 		while(currentRelationInList->next != NULL)
		{
			GIAentityNode* entityNodes[2];
			string corpusSpecialRelationIsQuery[2];
			entityNodes[0] = (*translatorVariables->GIAentityNodeArray)[currentRelationInList->relationGovernorIndex];
			entityNodes[1] = (*translatorVariables->GIAentityNodeArray)[currentRelationInList->relationDependentIndex];
			corpusSpecialRelationIsQuery[0] = currentRelationInList->corpusSpecialRelationGovernorIsQuery;
			corpusSpecialRelationIsQuery[1] = currentRelationInList->corpusSpecialRelationDependentIsQuery;

			for(int i=0; i<2; i++)
			{
				GIAentityNode* entityNode = entityNodes[i];
				if(corpusSpecialRelationIsQuery[i] == GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME)
				{
					cout << GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
				else if(corpusSpecialRelationIsQuery[i] == GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME)
				{
					cout << GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					entityNode->isNameQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
				else if(corpusSpecialRelationIsQuery[i] == GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME)
				{
					cout << GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME << endl;
					entityNode->isQuery = true;
					entityNode->isWhichOrEquivalentWhatQuery = true;
					GIAtranslatorOperations.setComparisonVariableNode(entityNode);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
			}
			currentRelationInList = currentRelationInList->next;
		}
	}
}
#endif

//note entityNodesActiveListNetworkIndexes is required for GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS only / featureArrayTemp is required for GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS:GIA_SEM_REL_TRANSLATOR_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES only
void GIAsemRelTranslatorParserClass::defineConnectionsBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			int entity1Index = currentRelationInList->relationGovernorIndex;
			int entity2Index = currentRelationInList->relationDependentIndex;
			GIAentityNode* entity1 = (*translatorVariables->GIAentityNodeArray)[entity1Index];
			GIAentityNode* entity2 = (*translatorVariables->GIAentityNodeArray)[entity2Index];
			bool sameReferenceSet = currentRelationInList->sameReferenceSet;

			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT])
			{
				#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
				//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAsynRelTranslatorGeneric.cpp:genericDependecyRelationInterpretation
				GIAentityNode* relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialProperty(entity1, entity2, translatorVariables);
				GIAtranslatorOperations.connectPropertyToEntity(entity1, entity2, relationshipEntity, sameReferenceSet, translatorVariables);
				#else
				GIAtranslatorOperations.connectDirectPropertyToEntity(entity1, entity2, sameReferenceSet, translatorVariables);		
				#endif
				
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY])
			{
				GIArelation* currentRelationInList2 = NULL;
				int entity2Index2 = INT_DEFAULT_VALUE;
				if(findMatchingObject(translatorVariables, GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE], entity2Index, &currentRelationInList2, &entity2Index2)
				{
					int entity3Index = currentRelationInList2->relationDependentIndex;
					GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
					int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
					GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

					#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
					GIAtranslatorOperations.connectPropertyToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
					#else
					GIAtranslatorOperations.connectDirectPropertyToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
					#endif
					foundMatchingObject = true;
					currentRelationInList2->disabled = true;
				}
				else
				{
					cerr << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY]) && (!foundMatchingObject)" << endl;
					exit(EXIT_ERROR);
				}
				currentRelationInList->disabled = true;
			}			
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION])
			{
				GIArelation* currentRelationInList2 = NULL;
				int entity2Index2 = INT_DEFAULT_VALUE;
				if(findMatchingObject(translatorVariables, GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE], entity2Index, &currentRelationInList2, &entity2Index2)
				{
					int entity3Index = currentRelationInList2->relationDependentIndex;
					GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
					int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
					GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

					GIAtranslatorOperations.connectActionToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);

					currentRelationInList2->disabled = true;
				}
				else
				{
					GIAtranslatorOperations.connectActionToSubject(entity1, entity2, sameReferenceSet, translatorVariables);				
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE])
			{
				GIAtranslatorOperations.connectActionToObject(entity1, entity2, sameReferenceSet, translatorVariables);
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION])
			{
				GIArelation* currentRelationInList2 = NULL;
				int entity2Index2 = INT_DEFAULT_VALUE;
				if(findMatchingObject(translatorVariables, GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE], entity2Index, &currentRelationInList2, &entity2Index2)
				{
					int entity3Index = currentRelationInList2->relationDependentIndex;
					GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
					int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
					GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

					currentRelationInList2->disabled = true;

					#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
					invertOrDuplicateConditionsIfRequiredSemantic(translatorVariables, entity1, entity2relation2, entity3, sameReferenceSet);
					#else
					GIAtranslatorOperations.connectConditionToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
					#endif
				}
				else
				{
					GIAtranslatorOperations.connectConditionToSubject(entity1, entity2, sameReferenceSet, translatorVariables);
					cout << "defineConnectionsBasedOnSemanticRelations{} warning: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION]) && (!foundMatchingObject) [ie without matching GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE]" << endl;
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE])
			{
				//NB THIS SHOULDNT BE required as there is never an isolated condition-object connection declared according to current GIA specification [but due to bug, it is required for Relex with measure dependency cases eg 'He runs every hour.'];
				cout << "defineConnectionsBasedOnSemanticRelations{} warning: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE]) without matching GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION" << endl;
				GIAtranslatorOperations.connectConditionToObject(entity1, entity2, sameReferenceSet, translatorVariables);
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT])
			{
				#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
				//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAsynRelTranslatorGeneric.cpp:genericDependecyRelationInterpretation
				GIAentityNode* relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(entity1, entity2, translatorVariables);
				GIAtranslatorOperations.connectDefinitionToEntity(entity1, entity2, relationshipEntity, sameReferenceSet, translatorVariables);
				#else
				GIAtranslatorOperations.connectDirectDefinitionToEntity(entity1, entity2, sameReferenceSet, translatorVariables);				
				#endif
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION])
			{
				GIArelation* currentRelationInList2 = NULL;
				int entity2Index2 = INT_DEFAULT_VALUE;
				if(findMatchingObject(translatorVariables, GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE], entity2Index, &currentRelationInList2, &entity2Index2)
				{
					int entity3Index = currentRelationInList2->relationDependentIndex;
					GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
					int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
					GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

					#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
					GIAtranslatorOperations.connectDefinitionToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
					#else
					GIAtranslatorOperations.connectDirectDefinitionToEntity(entity1, entity2relation2, entity3, sameReferenceSet, translatorVariables);
					#endif
					currentRelationInList2->disabled = true;
				}
				else
				{
					cerr << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION]) && (!foundMatchingObject)" << endl;
					exit(EXIT_ERROR);
				}
				currentRelationInList->disabled = true;
			}
			#ifdef GIA_ALIASES
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT_ALIAS])
			{
				#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
				#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
				//FUTURE FIXTHIS:
				//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAsynRelTranslatorGeneric.cpp:genericDependecyRelationInterpretation
				GIAentityNode* relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(entity1, entity2, translatorVariables);
				#endif
				#endif
				
				//NB !GIA_DISABLE_ALIAS_ENTITY_MERGING: GIA_TRANSLATOR_ONLY_MERGE_ENTITY_NODES_WHEN_LINK_PREESTABLISHED_REFERENCES_GIA: if(!(translatorVariables->linkPreestablishedReferencesGIA)) check was added 3f1a
					//CHECK this code update (sync) is OK

				bool pass = true;
				#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
				#ifndef GIA_SEMANTIC_PARSER_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
				pass = false;
				cerr << "GIAsemRelTranslatorParserClass::defineConnectionsBasedOnSemanticRelations: GIAtranslatorOperationsClass::connectDefinitionAliasWrapper{} error: GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT_ALIAS: !GIA_DISABLE_ALIAS_ENTITY_MERGING && !GIA_SEM_REL_TRANSLATOR_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC" << endl;
				exit(EXIT_ERROR);
				#endif
				#endif
				
				if(pass)
				{				
					if(!GIAtranslatorOperations.connectDefinitionAliasWrapper(translatorVariables, entity1, &(*translatorVariables->GIAentityNodeArray)[entity2Index], relationshipEntity, sameReferenceSet))
					{
						result = false;
					}
				}
				
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_ALIAS])
			{
				GIArelation* currentRelationInList2 = NULL;
				int entity2Index2 = INT_DEFAULT_VALUE;
				if(findMatchingObject(translatorVariables, GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE_ALIAS], entity2Index, &currentRelationInList2, &entity2Index2)
				{
					int entity3Index = currentRelationInList2->relationDependentIndex;
					GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
					int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
					GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];
	
					bool pass = true;
					#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
					#ifndef GIA_SEMANTIC_PARSER_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
					pass = false;
					cerr << "GIAsemRelTranslatorParserClass::defineConnectionsBasedOnSemanticRelations: GIAtranslatorOperationsClass::connectDefinitionAliasWrapper{} error: GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT_ALIAS: !GIA_DISABLE_ALIAS_ENTITY_MERGING && !GIA_SEM_REL_TRANSLATOR_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC" << endl;
					exit(EXIT_ERROR);
					#endif
					#endif
				
					if(pass)
					{
						if(!GIAtranslatorOperations.connectDefinitionAliasWrapper(translatorVariables, entity1, &(*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2], entity3, sameReferenceSet))
						{
							result = false;
						}
					}
										
					currentRelationInList2->disabled = true;
				}
				else
				{
					cerr << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION]) && (!foundMatchingObject)" << endl;
					exit(EXIT_ERROR);
				}
				currentRelationInList->disabled = true;
			}
			#endif
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_LOGIC_REFERENCES
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_DEFINITION_DIRECT])
			{
				GIArelation* currentRelationInList2 = NULL;
				int entity2Index2 = INT_DEFAULT_VALUE;
				if(findMatchingObject(translatorVariables, GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_DEFINITION_REVERSE], entity2Index, &currentRelationInList2, &entity2Index2)
				{
					int entity3Index = currentRelationInList2->relationDependentIndex;
					GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
					int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
					GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

					GIAtranslatorOperations.connectLogicReference(translatorVariables, GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_DEFINITION, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entity1, entity2relation2, entity3, sameReferenceSet);
					currentRelationInList2->disabled = true;
				}
				else
				{
					cerr << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_DEFINITION_DIRECT]) && (!foundMatchingObject)" << endl;
					exit(EXIT_ERROR);			
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_ACTION])
			{
				GIArelation* currentRelationInList2 = NULL;
				int entity2Index2 = INT_DEFAULT_VALUE;
				if(findMatchingObject(translatorVariables, GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_ACTION_REVERSE], entity2Index, &currentRelationInList2, &entity2Index2)
				{
					int entity3Index = currentRelationInList2->relationDependentIndex;
					GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
					int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
					GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

					GIAtranslatorOperations.connectLogicReference(translatorVariables, GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_VERB, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entity1, entity2relation2, entity3, sameReferenceSet);
					currentRelationInList2->disabled = true;
				}
				else
				{
					cerr << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_ACTION]) && (!foundMatchingObject)" << endl;
					exit(EXIT_ERROR);			
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONDITION])
			{
				GIArelation* currentRelationInList2 = NULL;
				int entity2Index2 = INT_DEFAULT_VALUE;
				if(findMatchingObject(translatorVariables, GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONDITION_REVERSE], entity2Index, &currentRelationInList2, &entity2Index2)
				{
					int entity3Index = currentRelationInList2->relationDependentIndex;
					GIAentityNode* entity3 = (*translatorVariables->GIAentityNodeArray)[entity3Index];
					int entity2IndexRelation2 = currentRelationInList2->relationGovernorIndex;
					GIAentityNode* entity2relation2 = (*translatorVariables->GIAentityNodeArray)[entity2IndexRelation2];

					GIAtranslatorOperations.connectLogicReference(translatorVariables, GIA_TXT_REL_TRANSLATOR_LOGIC_REFERENCE_CLASS_PREPOSITION, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entity1, entity2relation2, entity3, sameReferenceSet);
					currentRelationInList2->disabled = true;
				}
				else
				{
					cerr << "defineConnectionsBasedOnSemanticRelations{} error: (currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONDITION]) && (!foundMatchingObject)" << endl;
					exit(EXIT_ERROR);			
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONCLUSION])
			{
				GIAtranslatorOperations.connectLogicReferenceConclusion(translatorVariables, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entity1, entity2, sameReferenceSet);
				currentRelationInList->disabled = true;
			}	
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_LOGIC_CONJUNCTION])
			{
				GIAtranslatorOperations.connectLogicReferenceConjunction(translatorVariables, GIA_TXT_REL_TRANSLATOR_RULES_TOKENS_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, entity1, entity2, sameReferenceSet);
				currentRelationInList->disabled = true;
			}		
			#endif
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_PRENOMINAL_MODIFIER])
			{
				#ifdef GIA_SYN_REL_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_PROPERTIES_OR_DEFINITIONS_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
				//will process GIA_ENTITY_VECTOR_CONNECTION_TYPE_PRENOMINAL_MODIFIER in GIAsynRelTranslatorLinkEntitiesDynamicClass::linkEntitiesDynamicPrenominalModifierOfNoun
				#else
				if(!GIAtranslatorOperations.connectPrenominalModifierWrapper(translatorVariables, entity1, entity2, sameReferenceSet))
				{
					result = false;
				}
				#endif
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MULTIWORD_PREPOSITION])
			{
				if(!GIAtranslatorOperations.connectMultiwordPrepositionWrapper(translatorVariables, entity1, entity2, sameReferenceSet))
				{
					result = false;
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MULTIWORD_ALIAS])
			{
				if(!GIAtranslatorOperations.connectMultiwordAliasWrapper(translatorVariables, entity1, entity2, sameReferenceSet))
				{
					result = false;
				}
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MULTIWORD_DATE])
			{
				if(!GIAtranslatorOperations.connectMultiwordDate(translatorVariables, entity1, entity2, sameReferenceSet))
				{
					result = false;
				}
				currentRelationInList->disabled = true;
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NEW_CONDITIONS
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_NEW])
			{
				GIAentityNode* relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialCondition(entity1, entity2, currentRelationInList->semanticRelationFunctionConditionNewName, translatorVariables);
				currentRelationInList->disabled = true;
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
				invertOrDuplicateConditionsIfRequiredSemantic(translatorVariables, entity1, entity2, entity3, sameReferenceSet);
				#else
				GIAtranslatorOperations.connectConditionToEntity(entity1, entity2, entity3, sameReferenceSet, translatorVariables);
				#endif
			}
			#endif
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY])
			{
				currentRelationInList->disabled = true;
				GIAtranslatorOperations.connectQuantityToEntity(entity1, entity2, sameReferenceSet, translatorVariables);
			}
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_MEASURE
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MEASURE])
			{
				GIAtranslatorOperations.connectMeasureToEntity(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
				currentRelationInList->disabled = true;
			}
			else if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_MEASURE_PER])
			{
				GIAtranslatorOperations.connectMeasurePerToEntity(translatorVariables, entitySemanticRelationFunction1, entitySemanticRelationFunction2, sameReferenceSet);
				currentRelationInList->disabled = true;
			}
			#endif
			#endif
			else
			{

			}
		}

		currentRelationInList = currentRelationInList->next;
	}
}
	
	
bool findMatchingObject(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationName, const int entity2Index, GIArelation** currentRelationInList2, int* entity2Index2)
{
	bool foundMatchingObject = false;

	*currentRelationInList2 = translatorVariables->currentSentenceInList->firstRelationInList;
 	while((*currentRelationInList2)->next != NULL)
	{
		*entity2Index2 = (*currentRelationInList2)->relationDependentIndex;
		if((*currentRelationInList2)->relationType == semanticRelationName)
		{
			if(entity2Index == *entity2Index2)
			{//found matching conditionType pair
				foundMatchingObject = true;
			}
		}
		(*currentRelationInList2) = (*currentRelationInList2)->next;
	}	
	
	return foundMatchingObject;
}			

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
//based on invertOrDuplicateConditionsIfRequired{}
void GIAsemRelTranslatorParserClass::invertOrDuplicateConditionsIfRequiredSemantic(GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* entity1, GIAentityNode* entity2, GIAentityNode* entity3condition, bool sameReferenceSet)
{
	bool inverseConditionRequired = false;
	bool twoWayConditionRequired = false;
	string inverseConditionName = "";
	GIApreprocessorMultiwordReduction.detectIfInverseOrTwoWayConditionRequired(entity3condition->entityName, &inverseConditionRequired, &twoWayConditionRequired, &inverseConditionName);

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
	if(inverseConditionRequired)
	{
		GIAentityNode* inverseConditionEntity = createNewInverseConditionEntitySemantic(translatorVariables, inverseConditionName);
		GIAtranslatorOperations.connectConditionToEntity(entity2, entity1, inverseConditionEntity, sameReferenceSet, translatorVariables);
	}
	else
	{
		GIAtranslatorOperations.connectConditionToEntity(entity1, entity2, entity3condition, sameReferenceSet, translatorVariables);
	}
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
	if(twoWayConditionRequired)
	{
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
		/*
		GIAentityNode* inverseConditionEntity = createNewInverseConditionEntitySemantic(translatorVariables, entity3condition->entityName);
		GIAtranslatorOperations.connectConditionToEntity(entity2, entity1, inverseConditionEntity, sameReferenceSet, translatorVariables);
		*/
		GIAentityNode* inverseConditionEntity = GIAtranslatorOperations.addRelationshipArtificialToEntity(entity2, entity1, sameReferenceSet, GIA_ENTITY_TYPE_CONDITION, entity3condition->entityName, translatorVariables);
		inverseConditionEntity->inverseConditionTwoWay = true;
		#endif
		//inverseConditionEntity->conditionTwoWay = true;	//FUTURE FIXTHIS
	}
	#endif
}

/*
//based on createNewInverseConditionEntity{}
GIAentityNode* GIAsemRelTranslatorParserClass::createNewInverseConditionEntitySemantic(GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName)
{
	int inverseConditionEntityIndex = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;

	bool entityAlreadyExistant = false;
	GIAentityNode* inverseConditionEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipCondition(inverseConditionEntityIndex, &inverseConditionName, &entityAlreadyExistant, translatorVariables);
	inverseConditionEntity->wordOrig = inverseConditionName;	//is this necessary?
	inverseConditionEntity->entityIndexTemp = inverseConditionEntityIndex;
	//why not set inverseConditionEntity->sentenceIndexTemp?

	#ifdef GIA_SEM_REL_TRANSLATOR_CREATE_FEATURES_FOR_ARTIFICIAL_ENTITIES
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex] = new GIAfeature();
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->word = inverseConditionName;
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->lemma = inverseConditionName;	//is this necessary?
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->entityIndex = inverseConditionEntityIndex;
	#endif

	return inverseConditionEntity;
}
*/
#endif



void GIAsemRelTranslatorParserClass::applyAdvancedFeaturesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	defineQuantitiesBasedOnSemanticRelations(translatorVariables);

	defineQualitiesBasedOnSemanticRelations(translatorVariables);

	//measure, dates, and quantities??
	GIAsynRelTranslatorApplyAdvancedFeatures.defineTenseOnlyTimeConditions(translatorVariables);
}

void GIAsemRelTranslatorParserClass::defineQuantitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			if(currentRelationInList->relationType == GIA2semanticDependencyRelationNameArray[GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY])
			{
				int entity1Index = currentRelationInList->relationGovernorIndex;
				int entity2Index = currentRelationInList->relationDependentIndex;
				GIAentityNode* entity1 = (*translatorVariables->GIAentityNodeArray)[entity1Index];
				GIAentityNode* entity2 = (*translatorVariables->GIAentityNodeArray)[entity2Index];
				entity1->hasQuantity = true;

				GIAentityNode* quantitySubstance = entity1;

				if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
				{
					//code copied from extractQuantitiesStanfordCoreNLP{}:
					//if(quantitySubstance->NERTemp != FEATURE_NER_DATE)		//do not assume quantity entities when dealing with Stanford Dates (as they have already been parsed).
					//{

					quantitySubstance->hasQuantity = true;
					if((quantitySubstance->NormalizedNERtemp != "") && (quantitySubstance->NormalizedNERtemp != "0.0"))		//added 0.0 for a stanford anomaly 11 May 2012
					{
						quantitySubstance->quantityNumberString = quantitySubstance->NormalizedNERtemp;
					}
					else
					{
						quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;
					}

					int quantityNumberInt = GIAentityNodeClass.calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;
					quantitySubstance->entityType = GIA_ENTITY_TYPE_SUBSTANCE;	//added 2a11a [because defineConcepts() does not have access to quantity data]

					/*//comparison variables not yet implemented for GIA2
					if(currentRelationInList->relationDependent == GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						(*translatorVariables->GIAentityNodeArray)[currentRelationInList->relationDependentIndex]->isQuery = false;
						GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
					}
					*/
					//}
				}
				else if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)
				{
					quantitySubstance->hasQuantity = true;
					quantitySubstance->quantityNumberString = currentRelationInList->relationDependent;

					int quantityNumberInt = GIAentityNodeClass.calculateQuantityNumberInt(quantitySubstance->quantityNumberString);
					quantitySubstance->quantityNumber = quantityNumberInt;

					/*//comparison variables not yet implemented for GIA2
					if(currentRelationInList->relationDependent == GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
					{//update comparison variable (set it to the quantity)
						quantitySubstance->isQuery = true;
						(*translatorVariables->GIAentityNodeArray)[currentRelationInList->relationDependentIndex]->isQuery = false;
						GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
					}
					*/
				}

				//added 2c4c
				if(entity2->isQuery)
				{
					quantitySubstance->isQuery = true;
					if(entity2->isNameQuery)
					{
						quantitySubstance->isNameQuery = true;
					}
					if(entity2->isWhichOrEquivalentWhatQuery)
					{
						quantitySubstance->isWhichOrEquivalentWhatQuery = true;
					}
					GIAtranslatorOperations.setComparisonVariableNode(quantitySubstance);
					GIAtranslatorOperations.setFoundComparisonVariable(true);
				}
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
}


#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS
bool GIAsemRelTranslatorParserClass::generateAllPermutationsFromSemanticRelationsFile(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAfeature* firstFeatureInList = translatorVariables->currentSentenceInList->firstFeatureInList;
	const int NLPfeatureParser = translatorVariables->NLPfeatureParser;
	
	//code based on loadSemanticParserCorpusDatabaseFile{}:
	bool result = true;
	bool createNewSentences = true;
	bool parseGIA2file = true;
	bool isQuery = false;	//irrelevant

	GIArelation* firstRelationInList = NULL;
	#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD	//read in the relations just written to file by writeSemanticParserCorpusFile{}
	string corpusFileName = GIAsemRelTranslatorDatabase.semanticParserCorpusDBgenerateFileName(firstFeatureInList);
	GIAsentence* sentence = new GIAsentence();
	if(!GIAnlp.parseStanfordParserFile(corpusFileName, isQuery, sentence, createNewSentences, parseGIA2file, false))		//CHECKTHIS; need to account for corpus XyXysemanticRelations.txt having multiple entries [eg different text but identical layout]
	{
		result = false;
		cout << "generateAllPermutationsFromSemanticRelationsFile{} error: !parseStanfordParserFile" << endl;
	}
	else
	{
		firstRelationInList = sentence->firstRelationInList;
		//firstFeatureInList = sentence->firstFeatureInList;	//use original featureList (not that derived from corpus file)
		int numberOfWordsInSentence = sentence->numberOfWordsInSentence;
	#else
		firstRelationInList = GIAsemRelTranslatorDatabase.getFirstRelationInSemanticParserSentenceList();
		int numberOfWordsInSentence = translatorVariables->currentSentenceInList->numberOfWordsInSentence;	//OLD before 3b4a: GIAsentenceClass.calculateNumberOfWordsInSentence(firstFeatureInList);
	#endif

		GIAfeature* dummyBlankFeature = new GIAfeature();
		//now simulate GIA2 semantic relations for each subset of original sentence POS permutation
		GIAfeature* secondWordInTupleFeature = firstFeatureInList;
		int minIndexOfSecondWordInTuple = GIA_SEM_REL_TRANSLATOR_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE;
		for(int i=GIA_NLP_START_ENTITY_INDEX; i<minIndexOfSecondWordInTuple; i++)
		{
			secondWordInTupleFeature = secondWordInTupleFeature->next;
		}

		for(int secondWordInTupleIndex=minIndexOfSecondWordInTuple; secondWordInTupleIndex<=numberOfWordsInSentence; secondWordInTupleIndex++)	//secondWordInTupleIndex in subset
		{
			//NB "secondWordInTupleIndex" aka centralWordIndex
			GIAfeature* recordOfFeatureAfterSecondWordInTupleFeature = secondWordInTupleFeature->next;
			secondWordInTupleFeature->next = dummyBlankFeature;	//temporarily disconnect node at end of sentence subset

			if(firstFeatureInList->entityIndex != GIA_NLP_START_ENTITY_INDEX)
			{
				cerr << "generateAllPermutationsFromSemanticRelationsFile{} implementation error*: (firstFeatureInList->entityIndex != GIA_NLP_START_ENTITY_INDEX)" << endl;
				exit(EXIT_ERROR);
			}
			#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS
			if(!SHAREDvars.textInTextArray(secondWordInTupleFeature->word, semanticParserOptimiseBasedOnConjunctionsIllegalSecondWordInTupleArray, GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS_ILLEGAL_CENTRAL_WORD_NUMBER_OF_TYPES))
			{
				bool optimisedBasedOnContextualConjunctions = false;
				GIAfeature* firstFeatureInSentenceSubsetInitial = GIAsemRelTranslatorOperations.generateOptimisedFeatureSubsetBasedOnContextualConjunctions(firstFeatureInList, secondWordInTupleIndex, &optimisedBasedOnContextualConjunctions);
			#else
				GIAfeature* firstFeatureInSentenceSubsetInitial = firstFeatureInList;
			#endif
				int maxIndexOfFirstWordInTuple = (secondWordInTupleFeature->entityIndex - (GIA_SEM_REL_TRANSLATOR_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE-1));
				#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
				for(int firstWordInTupleIndex = GIA_NLP_START_ENTITY_INDEX; firstWordInTupleIndex <= maxIndexOfFirstWordInTuple; firstWordInTupleIndex++)
				{
				#endif
					GIAfeature* firstFeatureInSentenceSubset = firstFeatureInSentenceSubsetInitial;
					while(firstFeatureInSentenceSubset->entityIndex <= maxIndexOfFirstWordInTuple)		//changed 2k1a for GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS compatibility: verify that feature entityIndices are not mutated by GIA referencing*	//OLD: for(int firstWordInSentenceSubsetIndex=1; firstWordInSentenceSubsetIndex<secondWordInTupleIndex; firstWordInSentenceSubsetIndex++)
					{
						int firstWordInSentenceSubsetIndex=firstFeatureInSentenceSubset->entityIndex;
						#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
						if(firstWordInTupleIndex >= firstWordInSentenceSubsetIndex)
						{
						#endif
							int subsetSize = secondWordInTupleIndex-firstWordInSentenceSubsetIndex+1;	//subsetSize aka maxSpread

							#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_UNOPTIMISED_TEXT_CORPUS
							//code from convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}:
							string sentenceText = "";
							bool foundAtLeastOneRelation = false;
							GIArelation* currentSemanticRelationInList = firstRelationInList;
							while(currentSemanticRelationInList->next != NULL)
							{
								if((currentSemanticRelationInList->relationGovernorIndex >= firstWordInSentenceSubsetIndex) && ((currentSemanticRelationInList->relationGovernorIndex <= secondWordInTupleIndex) || (currentSemanticRelationInList->relationGovernorIndex > GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList)))
								&& (currentSemanticRelationInList->relationDependentIndex >= firstWordInSentenceSubsetIndex) && ((currentSemanticRelationInList->relationDependentIndex <= secondWordInTupleIndex) || (currentSemanticRelationInList->relationDependentIndex > GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList))))
								{
									//regenerate semantic relation based on parsed GIArelation object
									string GIA2semanticDependencyRelation = GIAsemRelTranslatorOperations.generateGIA2semanticDependencyRelationText(currentSemanticRelationInList->relationGovernor, currentSemanticRelationInList->relationDependent, currentSemanticRelationInList->relationType, currentSemanticRelationInList->relationGovernorIndex, currentSemanticRelationInList->relationDependentIndex, currentSemanticRelationInList->sameReferenceSet);
									GIA2semanticDependencyRelation = GIA2semanticDependencyRelation + STRING_NEWLINE;
									sentenceText = sentenceText + GIA2semanticDependencyRelation;

									foundAtLeastOneRelation = true;
								}
								currentSemanticRelationInList = currentSemanticRelationInList->next;
							}
							sentenceText = sentenceText + STRING_NEWLINE;	//required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)
							if(foundAtLeastOneRelation)
							{
								GIAsemRelTranslatorDatabase.prepareSemanticParserCorpusDatabaseFileTextForWriting();
								GIAsemRelTranslatorDatabase.saveTextToCorpusFileString(sentenceText);
								GIAsemRelTranslatorDatabase.writeSemanticParserCorpusFile(firstFeatureInSentenceSubset);
							}
							#endif

							#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
							bool foundAtLeastOneRelation = false;
							int firstWordInTupleIndexRelative = GIAsemRelTranslatorOperations.calculateFirstWordInTupleIndexRelative(firstWordInTupleIndex, firstWordInSentenceSubsetIndex);
							GIArelation* currentSemanticRelationInList = firstRelationInList;
							while(currentSemanticRelationInList->next != NULL)
							{
								int semanticDependencyRelationType = INT_DEFAULT_VALUE;
								bool sameReferenceSet = currentSemanticRelationInList->sameReferenceSet;
								if(SHAREDvars.textInTextArray(currentSemanticRelationInList->relationType, GIA2semanticDependencyRelationNameArray, GIA_SEM_REL_TRANSLATOR_SEMANTIC_DEPENDENCY_RELATION_NUMBER_OF_TYPES, &semanticDependencyRelationType))
								{
									//CHECKTHIS; how to handle these cases (currentSemanticRelationInList->relationGovernorIndex >= GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList)) || (currentSemanticRelationInList->relationDependentIndex >= GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList)))
										//NB GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE can't handle queries at present (or where featureIndex >= FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
									if((currentSemanticRelationInList->relationGovernorIndex == firstWordInTupleIndex) && (currentSemanticRelationInList->relationDependentIndex == secondWordInTupleIndex))
									{
										//write semantic relation data
										bool directionGovernorToDependent = true;
										GIAsemRelTranslatorDatabase.writeSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, semanticDependencyRelationType, directionGovernorToDependent, sameReferenceSet);
										foundAtLeastOneRelation = true;	//CHECKTHIS
										GIAfeature* currentFeatureInSentence = firstFeatureInList;
										while(currentFeatureInSentence->next != NULL)
										{
											string POStypeAbbreviationName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAsemRelTranslatorPOStype];
											currentFeatureInSentence = currentFeatureInSentence->next;
										}
									}
									else if((currentSemanticRelationInList->relationGovernorIndex == secondWordInTupleIndex) && (currentSemanticRelationInList->relationDependentIndex == firstWordInTupleIndex))
									{
										//write semantic relation data
										bool directionGovernorToDependent = false;
										GIAsemRelTranslatorDatabase.writeSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, semanticDependencyRelationType, directionGovernorToDependent, sameReferenceSet);
										foundAtLeastOneRelation = true;	//CHECKTHIS

										GIAfeature* currentFeatureInSentence = firstFeatureInList;
										while(currentFeatureInSentence->next != NULL)
										{
											string POStypeAbbreviationName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAsemRelTranslatorPOStype];
											currentFeatureInSentence = currentFeatureInSentence->next;
										}
									}
								}
								else
								{
									cerr << "generateAllPermutationsFromSemanticRelationsFile{} error: illegal semantic relation type; currentSemanticRelationInList->relationType = " << currentSemanticRelationInList->relationType << endl;
									exit(EXIT_ERROR);
								}
								currentSemanticRelationInList = currentSemanticRelationInList->next;
							}
							#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_EXTENSIVE
							if(!foundAtLeastOneRelation)
							{
								GIAsemRelTranslatorDatabase.writeSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, GIA_SEM_REL_TRANSLATOR_RELATION_TYPE_NON_EXISTENT, BOOL_IRRELEVANT, BOOL_IRRELEVANT);
							}
							#endif
							#endif
						#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
						}
						#endif
						firstFeatureInSentenceSubset = firstFeatureInSentenceSubset->next;
					}
				#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
				}
				#endif
			#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS
				if(optimisedBasedOnContextualConjunctions)
				{
					delete firstFeatureInSentenceSubsetInitial;	//delete artificially generated optimised sentence subset
				}
			}
			#endif

			secondWordInTupleFeature->next = recordOfFeatureAfterSecondWordInTupleFeature;	//restore temporarily disconnected node at end of sentence subset
			secondWordInTupleFeature = secondWordInTupleFeature->next;
		}
	#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	}
	#endif

	return result;
}

#endif


#endif

void GIAsemRelTranslatorParserClass::defineQualitiesBasedOnSemanticRelations(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[w];
			if(!(entity->disabled))
			{
				if((entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADJ) || (entity->grammaticalWordTypeTemp == GRAMMATICAL_WORD_TYPE_ADV))
				{
					entity->entityType = GIA_ENTITY_TYPE_QUALITY;
				}
			}
		}
	}
}

bool GIAsemRelTranslatorParserClass::convertSentenceSemanticRelationsIntoGIAnetworkNodesEndPart1(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entityNodesActiveListSentence)
{
	//TODO: why cant use convertSentenceSyntacticRelationsIntoGIAnetworkNodesEndPart1 instead?
	
	bool result = true;
	
	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			//#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION	//networkIndex sentenceIndex information is also required for GIAdraw.cpp
			#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEM_REL_TRANSLATOR_SEMANTIC_RELATION_GENERATION
			if((*translatorVariables->GIAentityNodeArray)[w]->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)	//added condition 2j5b [check this is required]
			{
				(*translatorVariables->GIAentityNodeArray)[w]->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
				cout << "1 convertSentenceSemanticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->sentenceIndexTemp undefined" << endl;
			}
			//record sentenceIndex for networkIndex entity nodes also (NB cannot use GIAnetworkIndexNodeArray here as it won't necessarily include networkIndex entity nodes for prepositions [for dynamic linking])
			if(!((*translatorVariables->GIAentityNodeArray)[w]->instanceReverseNodeList->empty()))
			{
				#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				GIAentityNode* instanceEntity = (*translatorVariables->GIAentityNodeArray)[w];
				for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntity->instanceReverseNodeList->begin(); connectionIter != instanceEntity->instanceReverseNodeList->end(); connectionIter++)
				{
					GIAentityNode* networkIndexNode = (*connectionIter)->entity;
				#else
					GIAentityNode* networkIndexNode = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance((*translatorVariables->GIAentityNodeArray)[w]);
				#endif

					if(networkIndexNode->entityIndexTemp == GIA_ENTITY_INDEX_UNDEFINED)	//added condition 2j5b [check this is required]
					{
						cout << "2 convertSentenceSemanticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->networkIndexNode->entityIndexTemp undefined" << endl;
						networkIndexNode->entityIndexTemp = w;
					}
					if(networkIndexNode->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
					{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network
						networkIndexNode->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
						cout << "3 convertSentenceSemanticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->networkIndexNode->sentenceIndexTemp undefined" << endl;
					}

				#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				}
				#endif
			}
			#else
			//record sentenceIndex for networkIndex entity nodes also (NB can use GIAnetworkIndexNodeArray here as it will include networkIndex entity nodes for prepositions)
			(*translatorVariables->GIAnetworkIndexNodeArray)[w]->entityIndexTemp = w;
			(*translatorVariables->GIAentityNodeArray)[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
			if((*translatorVariables->GIAnetworkIndexNodeArray)[w]->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
			{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network
				(*translatorVariables->GIAnetworkIndexNodeArray)[w]->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
				cout << "4 convertSentenceSemanticRelationsIntoGIAnetworkNodes{} warning: GIAnetworkIndexNodeArray[" << w << "]->sentenceIndexTemp undefined" << endl;
			}
			#endif
			//#endif

		}
	}
	
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			entityNodesActiveListSentence->push_back((*translatorVariables->GIAentityNodeArray)[w]);

			#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
			if(GIAtranslatorOperations.checkIfSentenceIsMathTextParsablePhrase(translatorVariables->sentenceIndex))	//translatorVariables->sentenceIndex: CHECKTHIS
			{
				#ifndef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
				if((*translatorVariables->GIAentityNodeArray)[w]->entityType == GIA_ENTITY_TYPE_CONCEPT)
				{
				#endif
					if(!((*translatorVariables->GIAentityNodeArray)[w]->wasReference))	//redundant?
					{
						(*translatorVariables->GIAentityNodeArray)[w]->NLCmathTextParsablePhraseEntity = true;
					}
				#ifndef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
				}
				#endif
			}
			#endif
		}
	}
	
	return result;
}

void GIAsemRelTranslatorParserClass::updateGrammaticalValuesBasedOnModalAuxiliaryOrCopula(GIAentityNode* entity, const string auxiliaryOrCopulaString)
{
	for(int i=0; i<GIA_SEM_REL_TRANSLATOR_RELATION_AUXILIARY_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(auxiliaryOrCopulaString == relationAuxiliaryPastTenseNameArray[i])
		{
			entity->grammaticalTenseTemp = GRAMMATICAL_TENSE_PAST;
		}
	}
	for(int i=0; i<GIA_SEM_REL_TRANSLATOR_RELATION_AUXILIARY_FUTURE_TENSE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(auxiliaryOrCopulaString == relationAuxiliaryFutureTenseNameArray[i])
		{
			entity->grammaticalTenseTemp = GRAMMATICAL_TENSE_FUTURE;
		}
	}
}




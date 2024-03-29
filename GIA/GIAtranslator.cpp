 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by X Parser
 * Description: Translator
 * /
 *******************************************************************************/


#include "GIAtranslator.hpp"


//required because parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences was shifted from GIAmain.cpp
//Dependency Relationship Extractor


bool GIAtranslatorClass::parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, string outputCFFfileName)
{
	bool result = true;

	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	LRPpreprocessorWordReduction.setActiveLRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo(translatorVariables->isQuery);	//required for local variable access
	#endif
	
	#ifndef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	if(!translatorVariables->parseGIA2file)
	{
		if(!GIAnlp.parseNLPparserFile(inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, translatorVariables->isQuery, translatorVariables->firstSentenceInList, translatorVariables->NLPfeatureParser, translatorVariables->NLPdependencyRelationsParser, translatorVariables->NLPrelexCompatibilityMode))
		{
			result = false;
		}
	}
	else
	{
		//semantic dependency relations and features have already been parsed
	}
	translatorVariables->NLPdependencyRelationsType = dependencyRelationsTypes[translatorVariables->NLPdependencyRelationsParser];
	if(translatorVariables->NLPrelexCompatibilityMode)
	{//untested
		if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
		{
			translatorVariables->NLPdependencyRelationsType = GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD;
			//assume dependency relations are in Stanford format from here on in
		}
	}
	#endif

	if(!createSemanticNetworkBasedUponDependencyParsedSentences(translatorVariables))
	{
		result = false;
	}

	SHAREDvars.setCurrentDirectory(outputFolder);

	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	//count maxNumberSentences
	LRPpreprocessorSentence* currentSentenceInList = translatorVariables->LRPpreprocessorTranslatorVariables.firstLRPpreprocessorSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		if(currentSentenceInList->sentenceIndexOriginal > translatorVariables->maxNumberSentences)
		{
			translatorVariables->maxNumberSentences = currentSentenceInList->sentenceIndexOriginal;
		}
		currentSentenceInList = currentSentenceInList->next;
	}	
	#else
	if(!translatorVariables->parseGIA2file)
	{
		#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
		string originalInputFileName = "";
		originalInputFileName = originalInputFileName + inputTextNLPrelationXMLfileName + " " + inputTextNLPfeatureXMLfileName;
		GIAnlp.outputInternalRelationsInRelexFormat(&outputCFFfileName, &originalInputFileName, translatorVariables->firstSentenceInList, translatorVariables->NLPdependencyRelationsParser, translatorVariables->NLPfeatureParser, translatorVariables->NLPexeFolderArray);
		#endif
	}
	//count maxNumberSentences
	GIAsentence* currentSentenceInList = translatorVariables->firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		if(currentSentenceInList->sentenceIndex > translatorVariables->maxNumberSentences)
		{
			translatorVariables->maxNumberSentences = currentSentenceInList->sentenceIndex;
		}
		currentSentenceInList = currentSentenceInList->next;
	}
	#endif

	return result;
}


bool GIAtranslatorClass::createSemanticNetworkBasedUponDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;

	#ifdef GIA_DATABASE
	int useDatabaseOriginal = GIAdatabase.getUseDatabase();
	if(translatorVariables->isQuery)
	{
		GIAdatabase.setUseDatabase(GIA_DATABASE_FALSE);
	}
	#endif
	
	#ifdef LRP_PREPROCESSOR_RECORD_REFERENCES
	GIAsentenceClass.backupOriginalNLPsentenceContent(translatorVariables->firstSentenceInList);
	#endif

	convertSentenceListRelationsIntoGIAnetworkNodes(translatorVariables);

	GIAtranslatorOperations.recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(translatorVariables->entityNodesActiveListNetworkIndexes);

	#ifdef GIA_DATABASE
	if(translatorVariables->isQuery)
	{
		GIAdatabase.setUseDatabase(useDatabaseOriginal);
	}
	#endif

	return result;
}



bool GIAtranslatorClass::convertSentenceListRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	/*
	LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = translatorVariables->firstLRPpreprocessorSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		cout << "currentLRPpreprocessorSentenceInList->sentenceIndexOriginal = " << currentLRPpreprocessorSentenceInList->sentenceIndexOriginal << endl;
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	*/
		
	translatorVariables->currentSentenceInList = translatorVariables->firstSentenceInList;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	translatorVariables->currentPreprocessorSentenceInList = translatorVariables->LRPpreprocessorTranslatorVariables.firstLRPpreprocessorSentenceInList;	
	#endif
	while(translatorVariables->currentSentenceInList->next != NULL)
	{
		int sentenceIndex = GIAtranslatorOperations.getCurrentSentenceIndex(translatorVariables);
		translatorVariables->sentenceIndex = sentenceIndex;
		
		convertSentenceRelationsIntoGIAnetworkNodesWrapper(translatorVariables);

		//cout << "translatorVariables->currentPreprocessorSentenceInList->next->sentenceIndexOriginal = " << translatorVariables->currentPreprocessorSentenceInList->next->sentenceIndexOriginal << endl;

		translatorVariables->currentSentenceInList = translatorVariables->currentSentenceInList->next;
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
		translatorVariables->currentPreprocessorSentenceInList = translatorVariables->currentPreprocessorSentenceInList->next;
		#endif
		
		//cout << "debug exit" << endl;
		//exit(0);
	}

	performGIApostProcessing(translatorVariables);
	
	return result;
}



bool GIAtranslatorClass::convertSentenceRelationsIntoGIAnetworkNodesWrapper(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	vector<XMLparserTag*>* SANIrulesTokenLayers = SANInodes.getSANIrulesTokenLayersGlobal();
	vector<SANIGroupType*>* SANIGroupTypes = SANInodes.getSANIGroupTypesGlobal();
	SANIpropagateOperations.resetAllNeuronComponents(SANIGroupTypes, GIA_POS_REL_TRANSLATOR_RULES_GROUP_BOOL_INDEX_ALLGROUPTYPES_NEURON_DEMARKATEOPTIMUMPATHWAY_RESET);
	
	SANItranslatorVariablesClass SANItranslatorVariables;
	GIAposRelTranslatorParser.createSANItranslatorVariablesFromGIAtranslatorVariables(translatorVariables, &SANItranslatorVariables);
	SANItranslatorVariables.parserDemarkateOptimumPathway = true;
	if(SANIposRelTranslatorPermutations.executePosRelTranslatorWrapper2(&SANItranslatorVariables, SANIrulesTokenLayers, SANIGroupTypes, translatorVariables->currentPreprocessorSentenceInList))
	{
		result = false;
	}
	SANItranslatorVariables.parserDemarkateOptimumPathway = false;
	#endif
	
	bool enableAdvancedReferencing = false;
	#ifdef GIA_ADVANCED_REFERENCING
	enableAdvancedReferencing = true;
	#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
	if(GIAtranslatorOperations.checkIfSentenceIsMathTextParsablePhrase(translatorVariables->currentSentenceInList))
	{
		enableAdvancedReferencing = false;
	}
	#endif
	if(enableAdvancedReferencing)
	{
		#ifdef GIA_DATABASE
		int useDatabaseOriginal = GIAdatabase.getUseDatabase();
		#endif

		GIAcoreference* firstGIAcoreferenceInList = new GIAcoreference();
		unordered_map<string,GIAentityNode*> sentenceNetworkIndexEntityNodesList;
		
		//backup current database structures and create temporary database structures for !linkPreestablishedReferencesGIA run;
		unordered_map<string,GIAentityNode*>* entityNodesActiveListCompleteFastIndexDBactiveOriginal;
		unordered_map<string,GIAentityNode*>* entityNodesActiveListCompleteFastIndexDBactiveTemp;
		#ifdef GIA_DATABASE
		if(GIAdatabase.getUseDatabase() != GIA_DATABASE_FALSE)
		{
			entityNodesActiveListCompleteFastIndexDBactiveOriginal = GIAdatabase.getDBentityNodesActiveListCompleteFastIndexDBactive();
			entityNodesActiveListCompleteFastIndexDBactiveTemp = new unordered_map<string,GIAentityNode*>;
			GIAdatabase.setDBentityNodesActiveListCompleteFastIndexDBactive(entityNodesActiveListCompleteFastIndexDBactiveTemp);
		}
		#endif
		
		//create temporary memory structures for !linkPreestablishedReferencesGIA run;
		GIAtranslatorVariablesClass translatorVariablesPrelim = *translatorVariables;
		translatorVariablesPrelim.entityNodesActiveListNetworkIndexes = &sentenceNetworkIndexEntityNodesList;	//NB exception: fill the sentenceNetworkIndexEntityNodesList during the prelim !linkPreestablishedReferencesGIA run (by filling the entityNodesActiveListNetworkIndexes variables)
		vector<GIAentityNode*> entityNodesActiveListCompleteTemp;	//never even written to (as saveNetwork == false), let alone used
		translatorVariablesPrelim.entityNodesActiveListComplete = &entityNodesActiveListCompleteTemp;
		translatorVariablesPrelim.currentEntityNodeIDInNetworkIndexEntityNodesList = 0;
		translatorVariablesPrelim.currentEntityNodeIDInCompleteList = 0;
		GIAsentence currentSentenceInListTemp;
		GIAsentenceClass.copySentence(translatorVariables->currentSentenceInList, &currentSentenceInListTemp);
		translatorVariablesPrelim.currentSentenceInList = &currentSentenceInListTemp;
		/*
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
		LRPpreprocessorSentence currentPreprocessorSentenceInListTemp;
		LRPpreprocessorSentenceClassObject.copyPreprocessorSentence(translatorVariables->currentPreprocessorSentenceInList, &currentPreprocessorSentenceInListTemp);
		translatorVariablesPrelim.currentPreprocessorSentenceInList = &currentPreprocessorSentenceInListTemp;		
		#endif
		*/
		vector<GIAentityNode*> sentenceNetworkIndexEntityNodesListTemp;
		map<int, vector<GIAentityNode*>*> entityNodesActiveListSentencesTemp;
		translatorVariablesPrelim.sentenceNetworkIndexEntityNodesList = &sentenceNetworkIndexEntityNodesListTemp;
		translatorVariablesPrelim.entityNodesActiveListSentences = &entityNodesActiveListSentencesTemp;
		translatorVariablesPrelim.saveNetwork = false;

		#ifdef GIA_DATABASE
		GIAdatabase.setUseDatabase(GIA_DATABASE_FALSE);
		#endif
		
		convertSentenceRelationsIntoGIAnetworkNodes(&translatorVariablesPrelim, false, NULL);

		vector<GIAentityNode*> referenceSetDefiniteEntityList;
		int numberReferenceSets = GIAtranslatorReferencing.identifyReferenceSets(&sentenceNetworkIndexEntityNodesList, translatorVariables->NLPdependencyRelationsType, &referenceSetDefiniteEntityList);	//NB NLPdependencyRelationsType is no longer used here
		//cout << "numberReferenceSets = " << numberReferenceSets << endl;
		
		#ifdef GIA_DATABASE
		GIAdatabase.setUseDatabase(useDatabaseOriginal);
		#endif
	
		#ifdef GIA_DEBUG_QUERY2
		cout << "translatorVariables->sentenceIndex = " << translatorVariables->sentenceIndex << endl;
		#endif
		
		GIAtranslatorReferencing.createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(&sentenceNetworkIndexEntityNodesList, translatorVariables->entityNodesActiveListNetworkIndexes, firstGIAcoreferenceInList, &referenceSetDefiniteEntityList, translatorVariables->sentenceIndex);

		translatorVariables->saveNetwork = true;


		#ifdef GIA_DATABASE
		if(GIAdatabase.getUseDatabase() != GIA_DATABASE_FALSE)
		{
			GIAdatabase.setDBentityNodesActiveListCompleteFastIndexDBactive(entityNodesActiveListCompleteFastIndexDBactiveOriginal);
		}
		#endif

		vector<GIAentityNode*> sentenceNetworkIndexEntityNodesListTemp2;
		translatorVariables->sentenceNetworkIndexEntityNodesList = &sentenceNetworkIndexEntityNodesListTemp2;
			
		convertSentenceRelationsIntoGIAnetworkNodes(translatorVariables, true, firstGIAcoreferenceInList);

		//clear temporary variables;
		delete firstGIAcoreferenceInList;
		GIAentityNodeClass.deleteEntitiesInEntityNodeList(&entityNodesActiveListCompleteTemp);	//what about entities that have been referenced via advanced referencing (and were not added via direct database access) - won't these be deleted also?
		#ifdef GIA_DATABASE
		if(GIAdatabase.getUseDatabase() != GIA_DATABASE_FALSE)
		{
			delete entityNodesActiveListCompleteFastIndexDBactiveTemp;	//entityNodesActiveListCompleteFastIndexTemp->clear();
		}
		#endif
	}
	else
	{
	#endif
		vector<GIAentityNode*> sentenceNetworkIndexEntityNodesListTemp2;
		
		convertSentenceRelationsIntoGIAnetworkNodes(translatorVariables, false, NULL);
	#ifdef GIA_ADVANCED_REFERENCING
	}
	#endif

	return result;
}

bool GIAtranslatorClass::convertSentenceRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList)
{
	bool result = true;
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_USE
	translatorVariables->linkPreestablishedReferencesGIA = linkPreestablishedReferencesGIA;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	if(!GIAsemRelTranslatorParser.convertSentenceSemanticRelationsIntoGIAnetworkNodes(translatorVariables, linkPreestablishedReferencesGIA, firstGIAcoreferenceInList))
	#else
	if(!GIAposRelTranslatorParser.convertSentenceTxtRelationsIntoGIAnetworkNodes(translatorVariables, linkPreestablishedReferencesGIA, firstGIAcoreferenceInList))
	#endif
	{
		result = false;
	}
	#endif
	#else
	#ifdef GIA_SEM_REL_TRANSLATOR_READ_SEMANTIC_RELATIONS
	if(translatorVariables->parseGIA2file)
	{
		if(translatorVariables->currentSentenceInList->semanticParserSuccessful)
		{
			if(!GIAsemRelTranslatorParser.convertSentenceSemanticRelationsIntoGIAnetworkNodes(translatorVariables, linkPreestablishedReferencesGIA, firstGIAcoreferenceInList))
			{
				result = false;
			}
		}
	}
	#endif
	if(!translatorVariables->parseGIA2file || (translatorVariables->parseGIA2file && !(translatorVariables->currentSentenceInList->semanticParserSuccessful)))
	{
		translatorVariables->linkPreestablishedReferencesGIA = linkPreestablishedReferencesGIA;
		if(!GIAsynRelTranslatorParser.convertSentenceSyntacticRelationsIntoGIAnetworkNodes(translatorVariables, firstGIAcoreferenceInList))
		{
			result = false;
		}
	}
	#endif
	
	return result;
}
		





bool GIAtranslatorClass::performGIApostProcessing(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	#ifndef GIA_POS_REL_TRANSLATOR_HYBRID_RECONCILE_REFERENCES_AFTER_SEMANTIC_PARSING_EVERY_SENTENCE
	#ifndef GIA_DEBUG_PREPROCESSOR_SENTENCE_RECONCILE_REFERENCES_DISABLE
	if(!GIAposRelTranslatorHybrid.connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesWrapper(translatorVariables->firstLRPpreprocessorSentenceInList, translatorVariables))	//check the position of this function's execution
	{
		result = false;
	}
	#endif
	#endif
	#endif

	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	dreamModeLinkSpecificConceptsAndActions(translatorVariables);
	#endif
		
	return result;
}

#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
void GIAtranslatorClass::dreamModeLinkSpecificConceptsAndActions(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAtranslatorReferencing.identifyReferenceSetsSpecificConceptsAndLinkWithConcepts(translatorVariables);

	#ifdef GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES
	createAndLinkNonSpecificConceptsForAllEntities(translatorVariables);
	#endif
}

#ifdef GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES
void GIAtranslatorClass::createAndLinkNonSpecificConceptsForAllEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	for(vector<GIAentityNode*>::iterator entityIter = translatorVariables->entityNodesActiveListComplete->begin(); entityIter != translatorVariables->entityNodesActiveListComplete->end(); entityIter++)
	{
		GIAentityNode* entity = (*entityIter);
		if((entity->entityType == GIA_ENTITY_TYPE_SUBSTANCE) && !(entity->entityType == GIA_ENTITY_TYPE_CONCEPT) && !(entity->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX))
		{
			GIAentityNode* networkIndexEntity = (entity->instanceReverseNodeList->back())->entity;

			GIAentityNode* nonSpecificConceptEntity = NULL;
			bool nonspecificConceptAlreadyExists = false;
			for(vector<GIAentityNode*>::iterator entityIter2 = translatorVariables->entityNodesActiveListComplete->begin(); entityIter2 != translatorVariables->entityNodesActiveListComplete->end(); entityIter2++)
			{
				GIAentityNode* entity2 = (*entityIter2);
				if((entity->entityType == GIA_ENTITY_TYPE_SUBSTANCE) && (entity2->entityType == GIA_ENTITY_TYPE_CONCEPT) && !(entity->entityType == GIA_ENTITY_TYPE_NETWORK_INDEX))
				{
					if(entity2->entityName == entity->entityName)
					{
						bool hasPropertiesOrConditions = false;
						if(!(entity2->propertyNodeList->empty()))
						{
							hasPropertiesOrConditions = true;
						}
						if(!(entity2->conditionNodeList->empty()))
						{
							hasPropertiesOrConditions = true;
						}
						if(!hasPropertiesOrConditions)
						{
							nonspecificConceptAlreadyExists = true;
							nonSpecificConceptEntity = entity2;
						}
					}
				}
			}
			if(!nonspecificConceptAlreadyExists)
			{
				nonSpecificConceptEntity = addInstanceToInstanceDefinition(networkIndexEntity, GIA_ENTITY_TYPE_CONCEPT, translatorVariables);	//CHECKTHIS; addConceptToConceptDefinition
			}

			//found instance in network matching concept...
			bool sameReferenceSet = false;
			
			#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
			//see "create intermediary auxiliary 'have'/'be' (property/definition relationship entity)" of GIAsynRelTranslatorGeneric.cpp:genericDependecyRelationInterpretation
			GIAentityNode* relationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(entity1, entity2, translatorVariables);
			GIAtranslatorOperations.connectDefinitionToEntity(entity, nonSpecificConceptEntity, relationshipEntity, sameReferenceSet, translatorVariables);
			#else
			GIAtranslatorOperations.connectDirectDefinitionToEntity(entity, nonSpecificConceptEntity, sameReferenceSet, translatorVariables);				
			#endif
		}
	}
}
#endif

#endif







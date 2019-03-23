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
 * File Name: GIAsynRelTranslatorParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g11e 01-March-2019
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#include "GIAsynRelTranslatorParser.hpp"


//required because parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences was shifted from GIAmain.cpp
//Dependency Relationship Extractor

		
bool GIAsynRelTranslatorParserClass::convertSentenceSyntacticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, GIAcoreference* firstGIAcoreferenceInList)
{
	bool result = true;

	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList) + SENTENCE_FIRST_ARTIFICIAL_INDEX;

	GIArelation* currentRelationInList;

	#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	string corpusFileName = "";
	if(!translatorVariables->linkPreestablishedReferencesGIA)
	{
		//GIAsemRelTranslatorOperations.determineGIAconnectionistNetworkPOStypeNames(currentSentenceInList->firstFeatureInList, NLPfeatureParser);
		GIAsemRelTranslatorDatabase.prepareSemanticParserDatabaseForWriting();
	}
	#endif

	vector<bool> GIAentityNodeArrayFilled(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));		//NB could also use currentSentence->maxNumberOfWordsInSentence
	vector<GIAentityNode*> GIAfeatureTempEntityNodeArray(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));	//temporary array introduced 14 July 2012b, to handle grammatical information specific to instances of networkIndex entities (not networkIndex entities themselves) before those intances have been defined (in GIAsynRelTranslatorDefineSubstances.cpp)	
	vector<GIAentityNode*> GIAnetworkIndexNodeArray(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	vector<GIAentityNode*> GIAentityNodeArray(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	vector<GIAfeature*> featureArrayTemp(GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables));
	translatorVariables->GIAentityNodeArrayFilled = &GIAentityNodeArrayFilled;
	translatorVariables->GIAfeatureTempEntityNodeArray = &GIAfeatureTempEntityNodeArray;
	translatorVariables->GIAnetworkIndexNodeArray = &GIAnetworkIndexNodeArray;	
	translatorVariables->GIAentityNodeArray = &GIAentityNodeArray;
	translatorVariables->featureArrayTemp = &featureArrayTemp;
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAfeatureTempEntityNodeArray[w] = NULL;	//added  14 July 2012b
		GIAnetworkIndexNodeArray[w] = NULL;		//added 10 May 2012
		GIAentityNodeArray[w] = NULL;		//added 10 May 2012
		featureArrayTemp[w] = NULL;		//added 14 July 2013
	}


	GIAtranslatorGrammar.locateAndAddAllFeatureTempEntities(translatorVariables);

	GIAtranslatorOperations.generateTempFeatureArray(translatorVariables, translatorVariables->featureArrayTemp);	//regeneration required for Relex in case query variables detected


	#ifdef GIA_RELEX
	if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)		//updated 2d1a 21 Jan 2013, OLD: if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		GIAtranslatorGrammar.fillGrammaticalArraysRelex(translatorVariables->currentSentenceInList);
	}
	#endif
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	else if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)	//NB stanford dependency relations are required [ie not just Stanford POS tags] such that det/aux information can be extracted	//updated 2d1a 21 Jan 2013 - changed 'if' to 'else if'
	{
		GIAtranslatorGrammar.fillGrammaticalArraysStanford(translatorVariables);
	}
	#endif
	//need to transfer featureArrayTemp->feature->entityDisabled information into GIAentityNodeArray...

	
	//this function is first only executed in a temporary fashion (where its output is only used by relex GIAtranslatorReferencing.linkPronounReferencesRelex())
	translatorVariables->GIAentityNodeArray = &GIAfeatureTempEntityNodeArray;	//required by applyGrammaticalInfoToAllEntities (first execution), redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs, GIAsynRelTranslatorRedistributeRelationsStanford/GIAsynRelTranslatorRedistributeRelationsRelex
 	GIAtranslatorGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, translatorVariables->currentSentenceInList->firstFeatureInList);
	
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION
	/*
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{//to work effectively/best, this function requires a) GIA_NLP_PARSER_STANFORD_CORENLP as NLPfeatureParser
		#ifndef STANFORD_PARSER_USE_POS_TAGS
		//to work effectively/best, this function requires b) Stanford CoreNLP as pos tagger (ie it is incompatible with STANFORD_PARSER_USE_POS_TAGS, as stanford Parser tags a lot more -ing words as NN [rather than VBG] as compared to Stanford CoreNLP)
	*/
	GIAsynRelTranslatorRedistributeRelations.redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(translatorVariables);
	/*
		#endif
	}
	*/
	#endif
	#endif


	
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
		GIAsynRelTranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAsynRelTranslatorRedistributeRelationsStanford", translatorVariables);
		#endif
	}
	#endif
	#ifdef GIA_ALIASES_RELEX_COMPATIBILITY
	if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
		GIAsynRelTranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAsynRelTranslatorRedistributeRelationsRelex", translatorVariables);
		#endif
	}
	#endif
	translatorVariables->GIAentityNodeArray = &GIAentityNodeArray;

	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	invertOrDuplicateConditionsIfRequired(translatorVariables);
	#endif

	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	GIAbot.botSwitchFirstAndSecondPersonSyntactic(translatorVariables);
	#endif

	
	GIAtranslatorGrammar.locateAndAddAllNetworkIndexEntities(translatorVariables);

	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	translatorVariables->GIAentityNodeArray = &GIAnetworkIndexNodeArray;	//required by createParentsOfSubclassEntities 
	GIAtranslatorOperations.createParentsOfSubclassEntities(translatorVariables);
	translatorVariables->GIAentityNodeArray = &GIAentityNodeArray;
	#endif

	if(translatorVariables->NLPassumePreCollapsedStanfordRelations)
	{//untested
		if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
		{
			translatorVariables->NLPdependencyRelationsType = GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD;
			//assume dependency relations are in Stanford format from here on in (ie; they do not require Relex special case ammendements/reconciliation + preposition types are in Stanford format)
		}
	}

#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	//if(!translatorVariables->linkPreestablishedReferencesGIA)	//criteria not used as same reference set tags may be required for dream mode or post processing (not just advanced referencing)
	//{
		//identify explicit same set linkages
		//eg "the guy that robbed the bank" in "the guy that robbed the bank is tall"
		GIAtranslatorReferencing.fillExplicitReferenceSameSetTags(translatorVariables->currentSentenceInList);
	//}
#endif
	
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		GIAentityNodeArray[w] = GIAnetworkIndexNodeArray[w];		//set default values of GIAentityNodeArray
	}

#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	#ifdef GIA_ADVANCED_REFERENCING
	if(translatorVariables->linkPreestablishedReferencesGIA)
	{
		GIAtranslatorReferencing.linkAdvancedReferencesGIA(translatorVariables, firstGIAcoreferenceInList);
	}
	else
	{
	#endif
		#ifdef GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
		#endif
			GIAtranslatorReferencing.linkPronounReferencesRelex(translatorVariables);
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		}
		#ifdef GIA_STANFORD_CORENLP
		else if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			GIAtranslatorReferencing.linkPronounAndTextualContextReferencesStanfordCoreNLP(translatorVariables, translatorVariables->firstSentenceInList->firstCoreferenceInList);
		}
		#endif
		#endif
		#endif
	#ifdef GIA_ADVANCED_REFERENCING
	}
	#endif
#endif

	//transfer disabled substances across execution#1 [this is required since GIAsynRelTranslatorRedistributeRelationsStanford operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of networkIndex entity nodes {whose values would have been automatically transferred to their instances upon creation}...]
	disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(translatorVariables);

	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_GENERIC_ENTITY_INTERPRETATION
 	//this is required as GIAsynRelTranslatorDefineSubstances.cpp now relies on entity grammar values rather than featureArrayTemp
	GIAtranslatorGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, translatorVariables->currentSentenceInList->firstFeatureInList);
	#endif


	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	GIAsynRelTranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAsynRelTranslatorDefineSubstances", translatorVariables);
	#endif


	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_GENERIC_ENTITY_INTERPRETATION
	//this function has been shifted, and applied to entity instances, not the networkIndex entity array...
 	GIAtranslatorGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, currentSentenceInList->firstFeatureInList);
	#endif

	GIAtranslatorReferencing.identifyComparisonVariableAlternateMethod(translatorVariables);

	//transfer disabled substances across execution#2 [this is required since GIAsynRelTranslatorRedistributeRelationsStanford operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of networkIndex entity nodes {whose values would have been automatically transferred to their instances upon creation}...]
	disableEntitiesBasedOnFeatureTempEntityNodeArray(translatorVariables);

	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	GIAsynRelTranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAsynRelTranslatorLinkEntities", translatorVariables);
	#endif
		
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	createAdditionalSubclassEntities(translatorVariables);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
	detectUserDeclaredSubclassEntities(translatorVariables);
	#endif
	#endif

	GIAsynRelTranslatorLinkEntitiesDynamic.linkEntitiesDynamic(translatorVariables);


	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	GIAsynRelTranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAsynRelTranslatorApplyAdvancedFeatures", translatorVariables);
	#endif
	GIAsynRelTranslatorApplyAdvancedFeatures.applyAdvancedFeatures(translatorVariables);

	vector<GIAentityNode*>* entityNodesActiveListSentence = new vector<GIAentityNode*>;
	if(!convertSentenceSyntacticRelationsIntoGIAnetworkNodesEndPart1(translatorVariables, entityNodesActiveListSentence))
	{
		result = false;
	}
	if(!convertSentenceRelationsIntoGIAnetworkNodesEndPart2(translatorVariables, entityNodesActiveListSentence))
	{
		result = false;
	}

	

	#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	if(!translatorVariables->linkPreestablishedReferencesGIA)
	{
		GIAsemRelTranslatorOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(translatorVariables);

		GIAsemRelTranslatorOperations.determineGIAconnectionistNetworkPOStypeNames(translatorVariables->currentSentenceInList->firstFeatureInList, translatorVariables->NLPfeatureParser);
		#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
		#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
		GIAsemRelTranslatorDatabase.writeSemanticParserCorpusFile(translatorVariables->currentSentenceInList->firstFeatureInList);
		#else
		GIAsemRelTranslatorDatabase.writeSemanticParserCorpusFileOptimised(translatorVariables->currentSentenceInList->firstFeatureInList);
		#endif
		#endif
		#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS
		if(!GIAsemRelTranslatorParser.generateAllPermutationsFromSemanticRelationsFile(translatorVariables))
		{
			cerr << "GIAsemRelTranslatorParser.generateAllPermutationsFromSemanticRelationsFile() failed" << endl;
			exit(EXIT_ERROR);
		}
		#endif
	}
	#endif
	
	/*
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID_RECONCILE_REFERENCES_AFTER_SEMANTIC_PARSING_EVERY_SENTENCE
	#ifdef GIA_ADVANCED_REFERENCING
	if(translatorVariables->linkPreestablishedReferencesGIA)
	{
	#endif
		cout << "translatorVariables->currentSentenceInList->sentenceIndex = " << translatorVariables->currentSentenceInList->sentenceIndex << endl;
		if(!GIApreprocessor.connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesSentenceWrapper(translatorVariables->firstGIApreprocessorSentenceInList, translatorVariables))	//check the position of this function's execution
		{
			result = false;
		}
	#ifdef GIA_ADVANCED_REFERENCING
	}	
	#endif
	#endif
	#endif
	*/
	
					
	return result;
}

bool GIAsynRelTranslatorParserClass::convertSentenceSyntacticRelationsIntoGIAnetworkNodesEndPart1(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entityNodesActiveListSentence)
{
	bool result = true;
	
	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[w];
			
			entityNodesActiveListSentence->push_back(entity);

			#ifdef GIA_ADVANCED_REFERENCING

			#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
			if(GIAtranslatorOperations.checkIfSentenceIsMathTextParsablePhrase(translatorVariables->sentenceIndex))	//translatorVariables->sentenceIndex: CHECKTHIS
			{
				#ifndef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
				if(entity->entityType == GIA_ENTITY_TYPE_CONCEPT)
				{
				#endif
					if(!(entity->wasReference))	//redundant?
					{
						entity->NLCmathTextParsablePhraseEntity = true;
					}
				#ifndef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
				}
				#endif
			}
			#endif

			
			if(!(entity->wasReference))
			{
				//this is now just debugging:
				if(entity->entityIndexTemp == GIA_ENTITY_INDEX_UNDEFINED)
				{
					//do not overwrite sentence index of source
					entity->entityIndexTemp = w;
					#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
					cerr << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} error: GIAentityNodeArray[" << w << "]->entityIndexTemp undefined, this is an artificial entity" << endl;
					exit(EXIT_ERROR);
					#endif
				}
				if(entity->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
				{
					//do not overwrite sentence index of source
					entity->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
					#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
					cerr << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} error: GIAentityNodeArray[" << w << "]->sentenceIndexTemp undefined, this is an artificial entity" << endl;
					exit(EXIT_ERROR);
					#endif
					
				}
				if(entity->entityIndexTemp != w)
				{
					#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
					/*
					cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->entityIndexTemp != " << w << endl;
					cout << "this should be a merged entity" << endl;
					cout << "entity->entityName = " << entity->entityName << endl;
					cout << "entity->entityType = " << entity->entityType << endl;
					cout << "entity->entityIndexTemp = " << entity->entityIndexTemp << endl;
					cout << "entity->sentenceIndexTemp = " << entity->sentenceIndexTemp << endl;
					*/
					#endif
				}
				if(entity->sentenceIndexTemp != translatorVariables->currentSentenceInList->sentenceIndex)
				{
					#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
					//cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->sentenceIndexTemp != " << translatorVariables->currentSentenceInList->sentenceIndex << endl;
					#endif
				}
				
				//#ifndef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
				//look for double references, and if found assume possible intrasentence referencing capabilities (assign unique entityIndexTemp values; ie their original entity indices);
				if((*translatorVariables->featureArrayTemp)[w] != NULL)
				{
					for(int w2=0; w2<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w2++)
					{
						if((*translatorVariables->featureArrayTemp)[w2] != NULL)
						{
							if((*translatorVariables->featureArrayTemp)[w2]->lemma == (*translatorVariables->featureArrayTemp)[w]->lemma)
							{
								if(w != w2)
								{
									entity->entityIndexTemp = w;	//this is required for intrasentence advanced referencing (reactivated 2f19e 24-July-2014)
								}
							}
						}
					}
				}
				//#endif
			}
			#else
				if(entity->entityIndexTemp != w)
				{
					entity->entityIndexTemp = w;
					#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
					//cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->entityIndexTemp != " << w << endl;
					#endif
				}
				if(entity->sentenceIndexTemp != translatorVariables->currentSentenceInList->sentenceIndex)
				{
					entity->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
					#ifdef GIA_REFERENCING_UPDATE_ENTITY_INDEXES_OF_REFERENCE_SOURCE_TO_THOSE_OF_CURRENT_SENTENCE
					//cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->sentenceIndexTemp != " << translatorVariables->currentSentenceInList->sentenceIndex << endl;
					#endif
				}
			#endif

			//#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION	//networkIndex sentenceIndex information is also required for GIAdraw.cpp
			//record sentenceIndex for networkIndex entity nodes also (NB cannot use GIAnetworkIndexNodeArray here as it won't include networkIndex entity nodes for prepositions)
			if(!(entity->instanceReverseNodeList->empty()))
			{
				#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				GIAentityNode* instanceEntity = entity;
				for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntity->instanceReverseNodeList->begin(); connectionIter != instanceEntity->instanceReverseNodeList->end(); connectionIter++)
				{
					GIAentityNode* networkIndexNode = (*connectionIter)->entity;
				#else
					GIAentityNode* networkIndexNode = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance(entity);
				#endif
					#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
					networkIndexNode->mustSetIsConceptBasedOnApposRelation = false; //added 29 Sept 2013
					networkIndexNode->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;	//added 29 Sept 2013
					networkIndexNode->isPronounReference = false;	//added 29 Sept 2013
					#endif

					if(networkIndexNode->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
					{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network
						networkIndexNode->sentenceIndexTemp = translatorVariables->currentSentenceInList->sentenceIndex;
						cerr << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "]->networkIndexNode->sentenceIndexTemp undefined" << endl;
						//exit(EXIT_ERROR);
					}
					/*No problem detected here:
					if(networkIndexNode->sentenceIndexTemp == 0)
					{
						cout << "error: invalid sentence id" << endl;
					}
					*/
				#ifdef GIA_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				}
				#endif
			}
			//#endif

		}
	}
	
	return result;
}

bool GIAsynRelTranslatorParserClass::convertSentenceRelationsIntoGIAnetworkNodesEndPart2(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entityNodesActiveListSentence)
{
	bool result = true;
	
	#ifdef GIA_CREATE_SHORTCUTS_TO_CONCEPT_ENTITIES
	GIAtranslatorOperations.createShortcutsToConceptEntities(translatorVariables);
	#endif
	
	int sentenceIndex = GIAtranslatorOperations.getCurrentSentenceIndex(translatorVariables);
	translatorVariables->entityNodesActiveListSentences->insert(pair<int, vector<GIAentityNode*>*>(sentenceIndex, entityNodesActiveListSentence));

	//GIAtranslatorOperations.recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(GIAentityNodeArrayFilled, GIAnetworkIndexNodeArray);		//this method is not sufficient, as some networkIndex entity nodes (eg prepositions/conditions) are not contained within GIAnetworkIndexNodeArray
	GIAtranslatorOperations.recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(translatorVariables->entityNodesActiveListNetworkIndexes);

	
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	//required to reset wasReferenceTemp for next time
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		if((translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			(translatorVariables->GIAentityNodeArray)[w]->wasReferenceTemp = false;
		}
	}
	#endif


	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	#ifdef GIA_ADVANCED_REFERENCING
	if(translatorVariables->linkPreestablishedReferencesGIA)
	{
	#endif
		if(translatorVariables->firstGIApreprocessorSentenceInList != NULL)
		{
			if(!GIApreprocessor.addSentenceToPreprocessorSentence(translatorVariables))
			{
				result = false;
			}
		}
	#ifdef GIA_ADVANCED_REFERENCING
	}
	#endif
	#endif
	
	return result;
}

	
void GIAsynRelTranslatorParserClass::disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->disabled)
			{
				GIAtranslatorOperations.disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork((*translatorVariables->GIAnetworkIndexNodeArray)[w]);
			}
		}
	}
}
void GIAsynRelTranslatorParserClass::disableEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAtranslatorOperations.getEntityArrayMaxIndex(translatorVariables); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			if((*translatorVariables->GIAfeatureTempEntityNodeArray)[w]->disabled)
			{
				GIAtranslatorOperations.disableEntity((*translatorVariables->GIAentityNodeArray)[w]);
			}
		}
	}
}




#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
void GIAsynRelTranslatorParserClass::invertOrDuplicateConditionsIfRequired(GIAtranslatorVariablesClass* translatorVariables)
{
	GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			bool sameReferenceSet = GIAtranslatorOperations.determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS, currentRelationInList);
			if(!sameReferenceSet)	//added 2h1f
			{
				bool prepositionFound = false;
				string conditionName = GIAtranslatorOperations.convertPrepositionToRelex(&(currentRelationInList->relationType), &prepositionFound);
				if(prepositionFound)
				{
					if(!(currentRelationInList->inverseRelation))
					{
						bool inverseConditionRequired = false;
						bool twoWayConditionRequired = false;
						string inverseConditionName = "";
						
						GIApreprocessorWordIdentification.detectIfInverseOrTwoWayConditionRequired(conditionName, &inverseConditionRequired, &twoWayConditionRequired, &inverseConditionName);

						#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
						if(inverseConditionRequired)
						{
							createNewInverseConditionEntity(currentRelationInList, translatorVariables, inverseConditionName);

							//added 2j5g (fix bug);
							currentRelationInList->inverseRelationSingle = true;
							string relationGovernorTemp = currentRelationInList->relationGovernor;
							string relationDependentTemp = currentRelationInList->relationDependent;
							int relationGovernorIndexTemp = currentRelationInList->relationGovernorIndex;
							int relationDependentIndexTemp = currentRelationInList->relationDependentIndex;
							currentRelationInList->relationGovernor = relationDependentTemp;
							currentRelationInList->relationDependent = relationGovernorTemp;
							currentRelationInList->relationGovernorIndex = relationDependentIndexTemp;
							currentRelationInList->relationDependentIndex = relationGovernorIndexTemp;
						}
						#endif
						#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
						if(twoWayConditionRequired)
						{
							#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
							GIArelation* lastRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
							while(lastRelationInList->next != NULL)
							{
								lastRelationInList = lastRelationInList->next;
							}
							createNewInverseConditionEntity(lastRelationInList, translatorVariables, conditionName);
							lastRelationInList->relationGovernor = currentRelationInList->relationDependent;
							lastRelationInList->relationDependent = currentRelationInList->relationGovernor;
							lastRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
							lastRelationInList->relationDependentIndex = currentRelationInList->relationGovernorIndex;
							lastRelationInList->inverseRelationTwoWay = true;
							lastRelationInList->auxiliaryIndicatesDifferentReferenceSet = currentRelationInList->auxiliaryIndicatesDifferentReferenceSet;	//added 2h3a
							lastRelationInList->rcmodIndicatesSameReferenceSet = currentRelationInList->rcmodIndicatesSameReferenceSet;	//added 2h3a
							lastRelationInList->next = new GIArelation();
							#endif
							currentRelationInList->relationTwoWay = true;
						}
						#endif
					}
				}
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
}

void GIAsynRelTranslatorParserClass::createNewInverseConditionEntity(GIArelation* currentRelationInList, GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName)
{
	int inverseConditionEntityIndex = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;

	(*translatorVariables->GIAentityNodeArrayFilled)[inverseConditionEntityIndex] = true;
	GIAentityNode* inverseConditionEntity = new GIAentityNode();
	inverseConditionEntity->sentenceIndexTemp = translatorVariables->sentenceIndex;
	inverseConditionEntity->entityName = inverseConditionName;
	inverseConditionEntity->wordOrig = inverseConditionName;	//is this necessary?
	//why not set inverseConditionEntity->entityIndexTemp and inverseConditionEntity->sentenceIndexTemp?
	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_INVERSE_PREPOSITIONS
	currentRelationInList->relationTypeNonInversed = currentRelationInList->relationType;
	currentRelationInList->relationTypeIndexNonInversed = currentRelationInList->relationTypeIndex;
	#endif
	currentRelationInList->relationType = string(GIA_SYN_REL_TRANSLATOR_STANFORD_PARSER_PREPOSITION_PREPEND) + inverseConditionName;
	currentRelationInList->relationTypeIndex = inverseConditionEntityIndex;
	currentRelationInList->inverseRelation = true;
	(*translatorVariables->GIAfeatureTempEntityNodeArray)[inverseConditionEntityIndex] = inverseConditionEntity;

	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex] = new GIAfeature();
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->word = inverseConditionName;
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->lemma = inverseConditionName;	//is this necessary?
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->entityIndex = inverseConditionEntityIndex;
}


#endif







/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1t2a 17-July-2013
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIAtimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIAtranslator.h"
#include "GIAtranslatorOperations.h"
#include "GIAtranslatorDefineGrammar.h"
#include "GIAtranslatorRedistributeRelexRelations.h"
#include "GIAtranslatorRedistributeStanfordRelations.h"
#include "GIAtranslatorDefineReferencing.h"
#include "GIAtranslatorDefineSubstances.h"
#include "GIAtranslatorLinkEntities.h"
#include "GIAtranslatorApplyAdvancedFeatures.h"
#include "GIAdatabase.h"
//#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
#include "GIAparser.h"
//#endif
#ifdef GIA_USE_BOT
#include "GIAbot.h"
#endif

static int referenceTypeHasDeterminateCrossReferenceNumberArray[GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_SINGULAR};


void initialiseGIATranslatorForTexualContext()
{
	initialiseGIATranslatorForTexualContextOperations();
}


#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
void convertParagraphSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Paragraph * firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	Paragraph * currentParagraphInList = firstParagraphInList;
	while(currentParagraphInList->next != NULL)
	{
		Sentence * firstSentenceInList = currentParagraphInList->firstSentenceInList;

		convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);

		#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
		#ifdef GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_PARAGRAPH
		//restore critical variables: used for GIA translator reference paser only - cleared every time a new paragraph is parsed
		unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListIter;
		for(conceptEntityNodesListIter = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter++)
		{
			GIAentityNode * entityNode = conceptEntityNodesListIter->second;
			entityNode->entityAlreadyDeclaredInThisContext = false;
		}

		/*
		long vectorSize = entityNodesActiveListConcepts->size();
		for(int entityIndex=0; entityIndex<vectorSize; entityIndex++)
		{
			GIAentityNode * entityNode = entityNodesActiveListConcepts->at(entityIndex);
			entityNode->entityAlreadyDeclaredInThisContext = false;
		}
		*/
		#endif
		#endif
		currentParagraphInList = currentParagraphInList->next;
	}
}
#endif

#ifdef USE_CE
//NB is convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy compatible with Stanford CoreNLP coreferences? [NB sentences are mixed up, but are sentence id's still valid? If there is a problem, then possibly need to either a) re-execute NLP on each codeextension heirachy, or b) revert to GIA pronominal coreference detection (see RelEx function to do this)
void convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	//link GIAsentences with each generated CEcodeextension
	Sentence * currentSentenceInList = firstSentenceInList;
	vector<CECodeextension*>::iterator codeextensionIter;
	for(codeextensionIter = codeextensionsList->begin(); codeextensionIter != codeextensionsList->end(); codeextensionIter++)
	{
		CECodeextension * currentCodeextensionInHeirachy = *codeextensionIter;
		currentCodeextensionInHeirachy->sentence = currentSentenceInList;
		#ifdef GIA_WITH_CE_CONVERT_PUNCTUATION_MARK_CHARACTERS_TO_FULL_STOPS
		#ifdef GIA_WITH_CE_DEBUG
		cout << "currentCodeextensionInHeirachy->numberSentencesInCodeextension = " << currentCodeextensionInHeirachy->numberSentencesInCodeextension << endl;
		#endif
		for(int i=0; i<currentCodeextensionInHeirachy->numberSentencesInCodeextension; i++)
		{
			#ifdef GIA_WITH_CE_DEBUG
			cout << "currentSentenceInList = currentSentenceInList->next" << endl;
			#endif
			currentSentenceInList = currentSentenceInList->next;
		}
		#else
		currentSentenceInList = currentSentenceInList->next;
		#endif
	}

	#ifdef GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS
	for(codeextensionIter = codeextensionsList->begin(); codeextensionIter != codeextensionsList->end(); codeextensionIter++)
	{
		//need to declare new GIA semantic network environment for each heirachical combination
		entityNodesActiveListConcepts = new unordered_map<string, GIAentityNode*>;
		timeConditionNodesActiveList = new unordered_map<long, GIAtimeConditionNode*>;
	#else
		codeextensionIter = codeextensionsList->begin();
	#endif
		CECodeextension * currentCodeextensionInHeirachy = *codeextensionIter;
		currentSentenceInList = currentCodeextensionInHeirachy->sentence;

		Sentence * lastSentenceInArtificialList = currentSentenceInList;
		Sentence * firstSentenceInArtificialList = generateArtificialSentenceListBasedUponParentCodeextensions(currentCodeextensionInHeirachy, lastSentenceInArtificialList, true);

		#ifdef GIA_WITH_CE_DEBUG
		cout << "CE DEBUG: print codeextension heirachy dependency relations for:" << endl;
		cout << "currentCodeextensionInHeirachy->codeextensionTextRaw = " << currentCodeextensionInHeirachy->codeextensionTextRaw << endl;		
		#endif
		Sentence * currentSentenceInArtificialList = firstSentenceInArtificialList;
		do
		{
			Relation * currentRelationInList = currentSentenceInArtificialList->firstRelationInList;
			while(currentRelationInList->next != NULL)
			{
				#ifdef GIA_WITH_CE_DEBUG
				cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
				#endif
				currentRelationInList = currentRelationInList->next;
			}	
				
			currentSentenceInArtificialList = currentSentenceInArtificialList->next;
			#ifdef GIA_WITH_CE_DEBUG
			cout << "\tcurrentSentenceInArtificialList = currentSentenceInArtificialList->next;" << endl;
			#endif
		}
		while(currentSentenceInArtificialList != NULL);
		#ifdef GIA_WITH_CE_DEBUG
		cout << "\n\n" << endl;
		#endif
		
		#ifdef GIA_WITH_CE_OLD
		vector<GIAentityNode*> *sentenceConceptEntityNodesList = &(currentCodeextensionInHeirachy->relevantConceptEntityNodeList);
		setAllCodeextensionEntitiesInHeirachyToUndeclaredInThisContext(firstCodeextensionInHeirachy);
		setParentCodeextensionEntitiesAsAlreadyDeclaredInThisContext(currentCodeextensionInHeirachy);
		convertSentenceRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, sentenceConceptEntityNodesList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		#else
		//convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		convertCEsentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
		#endif
	#ifdef GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS
	}
	#endif
}
#endif

void convertSentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	Sentence * currentSentenceInList = firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);

		currentSentenceInList = currentSentenceInList->next;
	}
}

#ifdef USE_CE
void convertCEsentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	Sentence * currentSentenceInList = firstSentenceInList;
	do
	{
		convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);

		currentSentenceInList = currentSentenceInList->next;
	}
	while(currentSentenceInList != NULL);
}
#endif


void convertSentenceRelationsIntoGIAnetworkNodesWrapper(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
#ifdef GIA_USE_ADVANCED_REFERENCING

	#ifdef GIA_USE_DATABASE
	int useDatabaseOriginal = getUseDatabase();
	#endif
	
	#ifdef GIA_FREE_MEMORY2
	vector<GIAentityNode*> * entityNodesActiveListCompleteOriginal = getTranslatorEntityNodesCompleteList();
	unordered_map<string, GIAentityNode*> * entityNodesActiveListCompleteFastIndexDBactiveOriginal;
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{
		entityNodesActiveListCompleteFastIndexDBactiveOriginal = getDBentityNodesActiveListCompleteFastIndexDBactive();	
	}
	vector<GIAentityNode*> * entityNodesActiveListSubstancesOriginal = getTranslatorSubstanceEntityNodesList();
	vector<GIAentityNode*> * entityNodesActiveListActionsOriginal = getTranslatorActionEntityNodesList();
	vector<GIAentityNode*> * entityNodesActiveListConditionsOriginal = getTranslatorConditionEntityNodesList();
	vector<GIAentityNode*> * entityNodesActiveListCompleteTemp = new vector<GIAentityNode*>;
	unordered_map<string, GIAentityNode*> * entityNodesActiveListCompleteFastIndexDBactiveTemp;
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{
		entityNodesActiveListCompleteFastIndexDBactiveTemp = new unordered_map<string, GIAentityNode*>;
	}
	vector<GIAentityNode*> * entityNodesActiveListSubstancesTemp = new vector<GIAentityNode*>;
	vector<GIAentityNode*> * entityNodesActiveListActionsTemp = new vector<GIAentityNode*>;
	vector<GIAentityNode*> * entityNodesActiveListConditionsTemp = new vector<GIAentityNode*>;
	setTranslatorEntityNodesCompleteList(entityNodesActiveListCompleteTemp);
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{
		setDBentityNodesActiveListCompleteFastIndexDBactive(entityNodesActiveListCompleteFastIndexDBactiveTemp);
	}
	setTranslatorSubstanceEntityNodesList(entityNodesActiveListSubstancesTemp);
	setTranslatorActionEntityNodesList(entityNodesActiveListActionsTemp);
	setTranslatorConditionEntityNodesList(entityNodesActiveListConditionsTemp);
	long currentEntityNodeIDInCompleteListOriginal = *(getCurrentEntityNodeIDinCompleteList());
	long currentEntityNodeIDInConceptEntityNodesListOriginal = *(getCurrentEntityNodeIDinConceptEntityNodesList());	
	long currentEntityNodeIDInSubstanceEntityNodesListOriginal = *(getCurrentEntityNodeIDinSubstanceEntityNodesList());
	long currentEntityNodeIDInActionEntityNodesListOriginal = *(getCurrentEntityNodeIDinActionEntityNodesList());
	long currentEntityNodeIDInConditionEntityNodesListOriginal = *(getCurrentEntityNodeIDinConditionEntityNodesList());
	long currentEntityNodeIDInCompleteListTemp = 0;
	long currentEntityNodeIDInConceptEntityNodesListTemp = 0;	
	long currentEntityNodeIDInSubstanceEntityNodesListTemp = 0;
	long currentEntityNodeIDInActionEntityNodesListTemp = 0;
	long currentEntityNodeIDInConditionEntityNodesListTemp = 0;
	*(getCurrentEntityNodeIDinCompleteList()) = currentEntityNodeIDInCompleteListTemp;
	*(getCurrentEntityNodeIDinConceptEntityNodesList()) = currentEntityNodeIDInConceptEntityNodesListTemp;	
	*(getCurrentEntityNodeIDinSubstanceEntityNodesList()) = currentEntityNodeIDInSubstanceEntityNodesListTemp;
	*(getCurrentEntityNodeIDinActionEntityNodesList()) = currentEntityNodeIDInActionEntityNodesListTemp;
	*(getCurrentEntityNodeIDinConditionEntityNodesList()) = currentEntityNodeIDInConditionEntityNodesListTemp;
	#endif
				
	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\n\t\t\t error: GIA_USE_ADVANCED_REFERENCING is under development (1convertSentenceRelationsIntoGIAnetworkNodes)\n" << endl;
	#endif

	#ifndef GIA_FREE_MEMORY2
	setSaveNetwork(false);
	#endif
	#ifdef GIA_USE_DATABASE
	setUseDatabase(GIA_USE_DATABASE_FALSE);
	#endif

	GIACoreference * firstGIACoreferenceInList = new GIACoreference();
	unordered_map<string, GIAentityNode*> * sentenceConceptEntityNodesList = new unordered_map<string, GIAentityNode*>;
	unordered_map<long, GIAtimeConditionNode*> * sentenceTimeConditionNodesList = new unordered_map<long, GIAtimeConditionNode*>;
	long * currentEntityNodeIDInSentenceCompleteList = getCurrentEntityNodeIDinSentenceCompleteList();
	long * currentEntityNodeIDInSentenceConceptEntityNodesList = getCurrentEntityNodeIDinSentenceConceptEntityNodesList();
	Sentence * currentSentenceInListTemp = new Sentence();
	copySentences(currentSentenceInList, currentSentenceInListTemp);
	*currentEntityNodeIDInSentenceCompleteList = 0;
	*currentEntityNodeIDInSentenceConceptEntityNodesList = 0;
	vector<GIAentityNode*> sentenceConceptEntityNodesListTempNotUsed1;
	convertSentenceRelationsIntoGIAnetworkNodes(sentenceConceptEntityNodesList, sentenceTimeConditionNodesList, firstSentenceInList, currentSentenceInListTemp, &sentenceConceptEntityNodesListTempNotUsed1, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, false, NULL);

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\n\t\t\t error: GIA_USE_ADVANCED_REFERENCING is under development (2identifyReferenceSets)\n" << endl;
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	/*
	vector<GIAentityNode*> ::iterator sentenceConceptEntityNodesListTempNotUsed1Iter;
	for(sentenceConceptEntityNodesListTempNotUsed1Iter = sentenceConceptEntityNodesListTempNotUsed1.begin(); sentenceConceptEntityNodesListTempNotUsed1Iter != sentenceConceptEntityNodesListTempNotUsed1.end(); sentenceConceptEntityNodesListTempNotUsed1Iter++)
	{
		GIAentityNode * entityNode = *sentenceConceptEntityNodesListTempNotUsed1Iter;
		cout << "DEBUG1: entityNode->entityName = " << entityNode->entityName << endl;
	}
	unordered_map<string, GIAentityNode*> ::iterator sentenceConceptEntityNodesListIter;
	for(sentenceConceptEntityNodesListIter = sentenceConceptEntityNodesList->begin(); sentenceConceptEntityNodesListIter != sentenceConceptEntityNodesList->end(); sentenceConceptEntityNodesListIter++)
	{
		GIAentityNode * entityNode = sentenceConceptEntityNodesListIter->second;
		cout << "DEBUG2: entityNode->entityName = " << entityNode->entityName << endl;
	}
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		cout << "Relation: " << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
		currentRelationInList = currentRelationInList->next;
	}
	*/
	#endif

	int numberReferenceSets = identifyReferenceSets(sentenceConceptEntityNodesList, NLPdependencyRelationsType);

	#ifdef GIA_USE_DATABASE
	setUseDatabase(useDatabaseOriginal);
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\n\t\t\t error: GIA_USE_ADVANCED_REFERENCING is under development (3createGIACoreferenceInListBasedUponIdentifiedReferenceSets)\n" << endl;
	#endif

	createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(sentenceConceptEntityNodesList, entityNodesActiveListConcepts, firstGIACoreferenceInList, numberReferenceSets);

	#ifndef GIA_FREE_MEMORY2
	setSaveNetwork(true);
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\n\t\t\t error: GIA_USE_ADVANCED_REFERENCING is under development (4convertSentenceRelationsIntoGIAnetworkNodes)\n" << endl;
	#endif

	#ifdef GIA_FREE_MEMORY2
	setTranslatorEntityNodesCompleteList(entityNodesActiveListCompleteOriginal);
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{
		setDBentityNodesActiveListCompleteFastIndexDBactive(entityNodesActiveListCompleteFastIndexDBactiveOriginal);	
	}
	setTranslatorSubstanceEntityNodesList(entityNodesActiveListSubstancesOriginal);
	setTranslatorActionEntityNodesList(entityNodesActiveListActionsOriginal);
	setTranslatorConditionEntityNodesList(entityNodesActiveListConditionsOriginal);
	*(getCurrentEntityNodeIDinCompleteList()) = currentEntityNodeIDInCompleteListOriginal;
	*(getCurrentEntityNodeIDinSubstanceEntityNodesList()) = currentEntityNodeIDInSubstanceEntityNodesListOriginal;
	*(getCurrentEntityNodeIDinConceptEntityNodesList()) = currentEntityNodeIDInConceptEntityNodesListOriginal;	
	*(getCurrentEntityNodeIDinActionEntityNodesList()) = currentEntityNodeIDInActionEntityNodesListOriginal;
	*(getCurrentEntityNodeIDinConditionEntityNodesList()) = currentEntityNodeIDInConditionEntityNodesListOriginal;
	#endif
		
	vector<GIAentityNode*> sentenceConceptEntityNodesListTempNotUsed;
	convertSentenceRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, true, firstGIACoreferenceInList);

	#ifdef GIA_FREE_MEMORY1
	//Clear temporary variables;
	delete firstGIACoreferenceInList;	
	delete currentSentenceInListTemp;
	#ifdef GIA_FREE_MEMORY2
	deleteEntitiesInEntityNodeList(entityNodesActiveListCompleteTemp);	//what about entities that have been referenced via advanced referencing (and were not added via direct database access) - won't these be deleted also?
	delete entityNodesActiveListCompleteTemp;	//entityNodesActiveListCompleteTemp->clear();
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{	
		delete entityNodesActiveListCompleteFastIndexDBactiveTemp;	//entityNodesActiveListCompleteFastIndexTemp->clear();	
	}
	delete entityNodesActiveListSubstancesTemp;	//entityNodesActiveListSubstancesTemp->clear();
	delete entityNodesActiveListActionsTemp;	//entityNodesActiveListActionsTemp->clear();
	delete entityNodesActiveListConditionsTemp;  	//entityNodesActiveListConditionsTemp->clear();  
	#endif
	delete sentenceConceptEntityNodesList;		//sentenceConceptEntityNodesList->clear();
	delete sentenceTimeConditionNodesList;		//sentenceTimeConditionNodesList->clear();
	//delete sentenceConceptEntityNodesListTempNotUsed1; 	//this is a local variable; no deletion required
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS
	cout << "\n\nfor colours only..." << endl;
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS_WORK_WITH_MULTIPLE_SENTENCES
			unordered_map<string, GIAentityNode*> sentenceConceptEntityNodesListTempNotUsedMap;
			for(vector<GIAentityNode*>::iterator sentenceConceptEntityNodesListTempNotUsedIter = sentenceConceptEntityNodesListTempNotUsed.begin(); sentenceConceptEntityNodesListTempNotUsedIter != sentenceConceptEntityNodesListTempNotUsed.end(); sentenceConceptEntityNodesListTempNotUsedIter++)
			{
				GIAentityNode * conceptEntityNodeTemp = *sentenceConceptEntityNodesListTempNotUsedIter;
				string entityNodeNameTemp = conceptEntityNodeTemp->entityName;
				sentenceConceptEntityNodesListTempNotUsedMap.insert(pair<string, GIAentityNode*>(entityNodeNameTemp, conceptEntityNodeTemp));
			}
			int numberReferenceSetsTemp = identifyReferenceSets(&sentenceConceptEntityNodesListTempNotUsedMap, NLPdependencyRelationsType);
			#ifdef GIA_FREE_MEMORY1
			delete sentenceConceptEntityNodesListTempNotUsedMap;
			#endif
		#else
			//only works with single sentence input (ie inputText.txt contains a single sentence)
			int numberReferenceSetsTemp = identifyReferenceSets(entityNodesActiveListConcepts, NLPdependencyRelationsType);
		#endif
	#endif
#else

	vector<GIAentityNode*> sentenceConceptEntityNodesListTempNotUsed;
	convertSentenceRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);

#endif



}

#ifdef GIA_USE_ADVANCED_REFERENCING
void convertSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool linkPreestablishedReferencesGIA,  GIACoreference * firstGIACoreferenceInList)
#else
void convertSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
#endif
{
	Relation * currentRelationInList;

	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "\t" << currentSentenceInList->sentenceText << endl;
	#endif

	bool GIAentityNodeArrayFilled[MAX_NUMBER_OF_WORDS_PER_SENTENCE];		//NB could also use currentSentence->maxNumberOfWordsInSentence
	GIAentityNode * GIAconceptNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode * GIAentityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode * GIAfeatureTempEntityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];	//temporary array introduced 14 July 2012b, to handle grammatical information specific to instances of concept entities (not concept entities themselves) before those intances have been defined (in GIAtranslatorDefineSubstances.cpp)

	//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
	//bool GIAEntityNodeArrayHasAssociatedSubstance[MAX_NUMBER_OF_WORDS_PER_SENTENCE];

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAconceptNodeArray[w] = NULL;		//added 10 May 2012
		GIAentityNodeArray[w] = NULL;		//added 10 May 2012
		GIAfeatureTempEntityNodeArray[w] = NULL;	//added  14 July 2012b

		//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
		//GIAEntityNodeArrayHasAssociatedSubstance[w] = false;

		//GIAActionNodeArrayFilled[w] = false;
	}


	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass A;" << endl;
	cout << "pass 0; locate/add all entities [execution#1]" << endl;
	#endif
	locateAndAddAllFeatureTempEntities(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPdependencyRelationsType, NLPfeatureParser);

	Feature * featureArrayTemp[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	generateTempFeatureArray(currentSentenceInList->firstFeatureInList, featureArrayTemp);	//regeneration required for Relex in case query variables detected
	
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1a; fillGrammaticalArrays" << endl;
	#endif
	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		fillGrammaticalArraysRelex(currentSentenceInList);
	}
	#endif
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		fillGrammaticalArraysStanford(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
	}
	#endif
	//need to transfer featureArrayTemp->feature->entityDisabled information into GIAentityNodeArray...

	//this function is first only executed in a temporary fashion (where its output is only used by relex linkPronounReferencesRelex())
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1b; applyGrammaticalInfoToAllEntities [execution#1]" << endl;
	#endif
 	applyGrammaticalInfoToAllEntities(GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, currentSentenceInList->firstFeatureInList);

	/*
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
	cout << "dependency relations: " << endl;
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			string relationType = currentRelationInList->relationType;
			GIAentityNode * relationGoverner = GIAconceptNodeArray[currentRelationInList->relationGovernorIndex];
			GIAentityNode * relationDependent = GIAconceptNodeArray[currentRelationInList->relationDependentIndex];

			cout << "relationType = " << currentRelationInList->relationType << endl;
			cout << "relationGoverner = " << relationGoverner->entityName << endl;
			cout << "relationDependent = " << relationDependent->entityName << endl;

		}
		currentRelationInList = currentRelationInList->next;
	}
	cout << "features (tags): " << endl;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(!(GIAconceptNodeArray[w]->disabled))
			{
				Feature * currentFeature = featureArrayTemp[w];

				cout << "Sentence Word Index = " << w;
				cout << "Word = " << currentFeature->word;
				cout << "Lemma = " << currentFeature->lemma;
				cout << "Is Date or Time = " << convertBoolToString(currentFeature->grammaticalIsDateOrTime);
				cout << "Tense = " << grammaticalTenseNameArray[currentFeature->grammaticalTense];
				for(int q=0; q<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES;q++)
				{
					cout << "Tense Modifier (" << grammaticalTenseModifierNameArray[q] << ") = " << convertBoolToString(currentFeature->grammaticalTenseModifierArray[q]);
				}
				cout << "Plurality = " << grammaticalNumberNameArray[currentFeature->grammaticalNumber];
				cout << "Is Definite = " << convertBoolToString(currentFeature->grammaticalIsDefinite);
				cout << "Is Proper Noun = " << convertBoolToString(currentFeature->grammaticalIsProperNoun);
				cout << "Gender = " << grammaticalGenderNameArray[currentFeature->grammaticalGender];
				cout << "Is Pronoun = " << convertBoolToString(currentFeature->grammaticalIsPronoun);
				cout << "Wordtype = " << grammaticalWordTypeNameArray[currentFeature->grammaticalWordType];

				cout << "NER = " << featureNERtypeArray[currentFeature->NER];
				cout << "NormalizedNER = " << currentFeature->NormalizedNER;
				cout << "Timex = " << currentFeature->Timex;
				cout << "POS = " << currentFeature->stanfordPOS << endl;
			}
		}
	}
	#endif

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			cout << "GIAconceptNodeArray[w]->disabled = " << GIAconceptNodeArray[w]->entityName << ", " << int(GIAconceptNodeArray[w]->disabled) << endl;
		}
	}
	*/


	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION
	/*
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{//to work effectively/best, this function requires a) GIA_NLP_PARSER_STANFORD_CORENLP as NLPfeatureParser
		#ifndef STANFORD_PARSER_USE_POS_TAGS
		//to work effectively/best, this function requires b) Stanford CoreNLP as pos tagger (ie it is incompatible with STANFORD_PARSER_USE_POS_TAGS, as stanford Parser tags a lot more -ing words as NN [rather than VBG] as compared to Stanford CoreNLP)
	*/
	#ifdef GIA_TRANSLATOR_DEBUG
	cout <<"pass 1c1; redistribute Stanford Relations - correct POS tags And Lemmas Of All Continuous Verbs" << endl;
	#endif		
	redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllContinuousVerbs(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, featureArrayTemp);
	/*	
		#endif
	}	
	*/	
	#endif
	#endif		
	
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
			
		#ifdef GIA_USE_STANFORD_CORENLP
		if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout <<"pass 1c2; disable redundant nodes Stanford Core NLP/Parser" << endl;	//[this could be implemented/"shifted" to an earlier execution stage with some additional configuration]
			#endif
			disableRedundantNodesStanfordCoreNLP(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		}
		#endif
		disableRedundantNodesStanfordParser(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);


		#ifdef GIA_LRP_DISABLE_REDISTRIBUTE_RELATIONS_POST_NLP_MULTIWORD_PREPOSITION_REDUCTION
		if(!(getUseLRP()))
		{
		#endif
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 1c3; redistribute Stanford Relations Multiword Preposition" << endl;
			#endif
			redistributeStanfordRelationsMultiwordPreposition(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#ifdef GIA_LRP_DISABLE_REDISTRIBUTE_RELATIONS_POST_NLP_MULTIWORD_PREPOSITION_REDUCTION
		}
		#endif
		
		//added 8 August 2012
		#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c4a; redistribute Relations - prep_of (eg The ball of the red dog is green..   nsubj(green-8, ball-2) / prep_of(ball-2, dog-6) ->  nsubj(green-7, ball-5) / poss(ball-5, dog-3)" << endl;
		#endif
		redistributeStanfordRelationsInterpretOfAsPossessive(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif
		#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_WHAT_IS_THE_NAME_NUMBER_OF_QUERIES
		#ifndef GIA_GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
		if(currentSentenceInList->isQuestion)
		{
		#endif
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 1c4b; redistribute Relations - what is the name/number of? 	nsubj(is-2, name-4) / attr(is-2, What-1) {/ det(name-4, the-3)} / poss/prep_of(name-4, dog-8) -> appos(That-1, _$qVar[1])" << endl;
			#endif
			redistributeStanfordRelationsCreateQueryVarsWhatIsTheNameNumberOf(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#ifndef GIA_GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
		}
		#endif
		#endif
		#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c4c; redistribute Relations - intepret name of as definition (eg interpret 'The red dog's name is Max.' / 'The name of the red dog is Max.'	nsubj(Max-7, name-5) / poss/prep_of(name-5, dog-3) -> appos(dog-3, Max-7)" << endl;
		#endif
		redistributeStanfordRelationsInterpretNameOfAsDefinition(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif

		

		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c5; redistribute Stanford Relations - Collapse Advmod Relation Function Be (eg The rabbit is 20 meters away. 	nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - > _predadj(rabbit-2, away-6)   +    Kane is late.	nsubj(late-3, Kane-1) / cop(late-3, is-2) -> _predadj(kane-1, late-3)          +    She is the one     nsubj(one-4, She-1) /cop(one-4, is-2) / det(one-4, the-3) -> appos(She-1, one-4)" << endl;
		//[OLD: nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - >nsubj(away-6, rabbit-2)] )" << endl;
		#endif
		redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser);

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c6; redistribute Stanford Relations - Generate Adjectives And Appos" << endl;
		cout << "eg1 Kane is late	      nsubj(late-3, Kane-1) / cop(late-3, is-2) -> _predadj(kane-1, late-3)			      [NB non-determinate of governer and dependent of subject relation; take as indicator of substance]" << endl;
		cout << "eg2 She is the one	      nsubj(one-4, She-1) / cop(one-4, is-2) / det(one-4, the-3) -> appos(She-1, one-4) 	      [NB determinate of dependent of subject relation; take as an indicator of definition]" << endl;
		cout << "eg3 The girl is tall	      nsubj(tall-6, girl-2) / cop(tall-6, is-3) / det(girl-2, The-1) -> _predadj(girl-2, tall-6)      [NB non-plural and determinate of governer of subject relation; take as indicator of substance]" << endl;
		cout << "eg4 bikes are machines        nsubj(machines-3, bikes-1) / cop(machines-3, are-2) -> appos(bikes-1, machines-3) 	      [NB plural and non-determinate of governer of subject relation; take as an indicator of definition]" << endl;
		cout << "eg5 the wheels are green      nsubj(green-6, wheels-4) / cop(green-6, are-5) -> _predadj(wheels-4, green-6)		      [NB plural and determinate of governer of subject relation; take as indicator of substance]" << endl;
		cout << "eg6 That is Jim.	      nsubj(Jim-3, That-1) / cop(Jim-3, is-2) -> appos(That-1, Jim-3)" << endl;
		cout << "eg7 The time is 06:45	      nsubj(06:45-4, time-2) / cop(06:45-4, is-3) / det(time-2, The-1) -> appos(time-2, 06:45-4)" << endl;	
		#endif
		redistributeStanfordRelationsCollapseSubjectAndCopGenerateAdjectivesAndAppos(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser);
		#endif

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c7; redistribute Stanford Relations - Adverbal Clause Modifier And Complement (eg The accident happened as the night was falling. 	advcl(happen, fall) / mark(fall, as))" << endl;
		#endif
		redistributeStanfordRelationsAdverbalClauseModifierAndComplement(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);

		#ifndef GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_TYPE1
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c8; redistribute Stanford Relations - Clausal Subject (eg What she said makes sense. 	csubj (make, say)/dobj ( said-3 , What-1 ))" << endl;
		#endif
		redistributeStanfordRelationsClausalSubject(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif
		
		/*OLD:
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c9; redistribute Stanford Relations - Phrasal Verb Particle (eg They shut down the station.    prt(shut, down))" << endl;
		#endif
		redistributeStanfordRelationsPhrasalVerbParticle(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		*/
	
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c9; redistribute Stanford Relations - Conjunction And Coordinate (eg I eat a pie or tom rows the boat. 	cc(pie-4, or-5) / conj(pie-4, tom-6))" << endl;
		#endif
		redistributeStanfordRelationsConjunctionAndCoordinate(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c10; redistribute Stanford Relations - Generate Unparsed Quantity Modifers (eg The punter won almost $1000. 	advmod(won-3, almost-4) / pobj(almost-4, $-5)) / num($-5, 1000-6)" << endl;
		#endif
		redistributeStanfordRelationsGenerateUnparsedQuantityModifers(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		
		
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c11; redistribute Stanford Relations - Generate Measures (eg years old - npadvmod(old, years) -> _measure_time(old[7], years[6]))" << endl;
		#endif
		redistributeStanfordRelationsGenerateMeasures(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c12; redistribute Stanford Relations - Prt And Tmods (eg The disaster happened over night.   prt(happened-3, over-4) / tmod(happened-3, night-5) -> over(happened-3, night-5) )" << endl;
		#endif
		redistributeStanfordRelationsPhrasalVerbParticle(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);

		#ifndef GIA_GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
		if(currentSentenceInList->isQuestion)
		{
		#endif
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 1c13; redistribute Stanford Relations - Create Query Vars (eg interpret 'who is that' / 'what is the time.'  attr(is-2, Who-1) / attr(is-2, What-1) | interpret 'how much'/'how many' | interpret 'which' det(house-2, Which-1) | interpret how/when/where/why advmod(happen-5, How-1) / advmod(leave-4, When-1) / advmod(is-2, Where-1) / advmod(fall-5, Why-1)	 )" << endl;
			#endif
			redistributeStanfordRelationsCreateQueryVars(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, featureArrayTemp);
		#ifndef GIA_GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
		}
		#endif

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c14; redistribute Stanford Relations - partmod (eg Truffles picked during the spring are tasty.   partmod(truffle, pick) -> obj(pick, truffle) )" << endl;
		#endif
		redistributeStanfordRelationsPartmod(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);

		#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
		//Added 28 October 2012b
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c15; redistribute Stanford Relations - Interpret Of As Object For ContinuousVerb (eg What is wood used in the delivering of?   interpret prep_of(xing, y) as obj(xing, y) )" << endl;
		#endif
		redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
		#endif
	
		#ifdef GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_EXPLITIVES
		//Added 13 November 2012
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c16; redistribute Stanford Relations - expletives (eg 'There is a place that we go'   _expl(be[2], there[1]) / _subj(be[2], place[4]) / _subj(go[7], we[6]) / _obj(be[2], go[7]) -> _subj(go[7], we[6]) / _obj(place[4], go[7])  )" << endl;
		#endif
		redistributeStanfordRelationsExpletives(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif

		#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP
		//Added 13 November 2012
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c17; redistribute Stanford Relations - dependency prepositions (eg 'Given the nature of the bank, write the letter.'  prep(write-8, Given-1) / dep(Given-1, nature-3) -> prep_given(write-8, nature-3) )" << endl;
		#endif
		redistributeStanfordRelationsDependencyPreposition(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif
		
		#ifdef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
		//added 12 July 2013
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c18; redistribute Stanford Relations - disable Aux And Cop Relations" << endl;
		#endif		
		redistributeStanfordRelationsDisableAuxAndCop(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif
	}
	else 
	#endif
	#ifdef GIA_SUPPORT_ALIASES_RELEX_COMPATIBILITY
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c1Alternate; redistribute Relex Relations -Collapse Subject And Object: Generate Appos" << endl;
		cout << "She is the one.	_obj(be[2], one[4]) + _subj(be[2], she[1]) -> appos(She-1, one-4)" << endl;
		cout << "Bikes are machines. 	_obj(be[2], machine[3]) + _subj(be[2], bike[1])	-> appos(bike-1, machine-3)" << endl;
		cout << "That is Jim. 		_obj(be[2], Jim[3]) + _subj(be[2], that[1]) -> appos(that-1, Jim-3)" << endl;
		cout << "The time is 06:45.	_obj(be[3], 06:45[4]) + _subj(be[3], time[2]) -> appos(time-2, 06:45-4)	" << endl;
		#endif
		redistributeRelexRelationsCollapseSubjectAndObjectGenerateAppos(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
	
	
		#ifdef GIA_SUPPORT_WHO_QUERY_ALIAS_ANSWERS
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c2Alternate; redistribute Relex Relations - Detect Name Queries" << endl;		
		#endif
		//required for aliasing to work
		redistributeRelexRelationsDetectNameQueries(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, featureArrayTemp);
		#endif

		#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c3Alternate; redistribute Relex Relations - Interpret Of As Object For ContinuousVerb" << endl;
		cout << "eg1 Yarn is used in the making of cloth.	of(making[6], cloth[8]) + in(use[3], making[6]) -> _obj(making[6], _cloth[8])" << endl;
		cout << "eg2 What is yarn used in the making of?  interpret  of(making[7], of[8]) + _obj(of[8], _$qVar[1])  -> _obj(making[7], _$qVar[1])" << endl;		
		#endif
		redistributeRelexRelationsInterpretOfAsObjectForContinuousVerbs(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif
		
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION	
		#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c4Alternate; redistribute Relex Relations - Adverb Plus Object Plus Subject Relation Where Adverb Has Same Argument As Subject As Condition" << endl;
		cout << "eg1;  Space is saved by having a chicken.	_subj(have[5], by[4]) + _obj(have[5], chicken[7]) + _advmod(save[3], by[4]) -> dobj(have[5], chicken[7]) + prep_by(save[3], have[5])	{required for interpretation by linkHavingPropertyConditionsAndBeingDefinitionConditions}" << endl;	
		cout << "eg2;  Space is saved by being a chicken.	_subj(be[5], by[4]) + _obj(be[5], chicken[7] + _advmod(save[3], by[4]) -> dobj(be[5], chicken[7]) + prep_by(save[3], be[5])	{required for interpretation by linkHavingPropertyConditionsAndBeingDefinitionConditions}" << endl;
		cout << "eg3;  Space is saved by moving a chicken.	_subj(move[5], by[4]) + _obj(move[5], chicken[7]) + _advmod(save[3], by[4]) -> -> dobj(move[5], chicken[7]) + prep_by(save[3], move[5])" << endl;
		#endif
		//required for aliasing to work
		redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationWhereAdverbHasSameArgumentAsSubjectAsCondition(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif
		
		#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c5Alternate; redistribute Relex Relations - Adverb Plus Object Plus Subject Relation All With A Definition Function As Property Links" << endl;	
		cout << "eg; The chicken is 3 minutes late.	_subj(be[3], chicken[2]) + _obj(be[3], minutes[5]) + _advmod(be[3], late[6]) -> _advmod(late[6], minutes[5]) + _advmod(chicken[2],  late[6])" << endl;
		#endif
		//required for aliasing to work
		redistributeRelexRelationsAdverbPlusObjectPlusSubjectRelationAllWithADefinitionFunctionAsPropertyLinks(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif
		
		#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c6Alternate; redistribute Relex Relations - Disregard Redundant Definition Relations" << endl;	
		cout << "eg1 What are the patent claims on?	_subj(be[2], claim[5]) + _obj(on[6], _$qVar[1]) -> on(claim[5], _$qVar[1]) " << endl;			
		#endif
		//required for aliasing to work
		redistributeRelexRelationsDisregardRedundantDefinitionRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif
		
		#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c7Alternate; redistribute Relex Relations - Adverb Plus Subject Relation As Action Condition" << endl;
		cout << "eg1 Space is saved by running fast.	_subj(run[5], by[4]) + _advmod(save[3], by[4]) -> by(save[3], run[5])" << endl;
		cout << "eg2 What is the Co-cart designed for?	_obj(for[6], _$qVar[1]) + _advmod(design[5], for[6]) -> for(design[5], _$qVar[1" << endl;		
		#endif
		//required for aliasing to work
		redistributeRelexRelationsAdverbPlusSubjectRelationAsActionCondition(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
		#endif	
	#endif						
				
	}
	#endif
	
	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	botSwitchFirstAndSecondPerson(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPdependencyRelationsType);
	#endif


	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1d; locate/add all entities [execution#2]" << endl;
	#endif
	locateAndAddAllConceptEntities(GIAentityNodeArrayFilled, GIAconceptNodeArray, entityNodesActiveListConcepts, sentenceConceptEntityNodesList, NLPdependencyRelationsType, NLPfeatureParser, GIAfeatureTempEntityNodeArray);

	
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
	cout << "dependency relations: " << endl;
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			string relationType = currentRelationInList->relationType;
			GIAentityNode * relationGoverner = GIAconceptNodeArray[currentRelationInList->relationGovernorIndex];
			GIAentityNode * relationDependent = GIAconceptNodeArray[currentRelationInList->relationDependentIndex];

			cout << "relationType = " << currentRelationInList->relationType << endl;
			cout << "relationGoverner = " << relationGoverner->entityName << endl;
			cout << "relationDependent = " << relationDependent->entityName << endl;

		}
		currentRelationInList = currentRelationInList->next;
	}
	cout << "features (tags): " << endl;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(!(GIAconceptNodeArray[w]->disabled))
			{
				Feature * currentFeature = featureArrayTemp[w];

				cout << "Sentence Word Index = " << w;
				cout << "Word = " << currentFeature->word;
				cout << "Lemma = " << currentFeature->lemma;
				cout << "Is Date or Time = " << convertBoolToString(currentFeature->grammaticalIsDateOrTime);
				cout << "Tense = " << grammaticalTenseNameArray[currentFeature->grammaticalTense];
				for(int q=0; q<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES;q++)
				{
					cout << "Tense Modifier (" << grammaticalTenseModifierNameArray[q] << ") = " << convertBoolToString(currentFeature->grammaticalTenseModifierArray[q]);
				}
				cout << "Plurality = " << grammaticalNumberNameArray[currentFeature->grammaticalNumber];
				cout << "Is Definite = " << convertBoolToString(currentFeature->grammaticalIsDefinite);
				cout << "Is Proper Noun = " << convertBoolToString(currentFeature->grammaticalIsProperNoun);
				cout << "Gender = " << grammaticalGenderNameArray[currentFeature->grammaticalGender];
				cout << "Is Pronoun = " << convertBoolToString(currentFeature->grammaticalIsPronoun);
				cout << "Wordtype = " << grammaticalWordTypeNameArray[currentFeature->grammaticalWordType];

				cout << "NER = " << featureNERtypeArray[currentFeature->NER];
				cout << "NormalizedNER = " << currentFeature->NormalizedNER;
				cout << "Timex = " << currentFeature->Timex;
				cout << "POS = " << currentFeature->stanfordPOS << endl;
			}
		}
	}
	#endif
	

	if(NLPassumePreCollapsedStanfordRelations)
	{//untested
		if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
		{
			NLPdependencyRelationsType = GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD;
			//assume dependency relations are in Stanford format from here on in (ie; they do not require Relex special case ammendements/reconciliation + preposition types are in Stanford format)
		}
	}

#ifndef GIA_ADVANCED_REFERENCING_DEBUG_DISABLE_LINKING
	#ifdef GIA_USE_ADVANCED_REFERENCING
	//if(!linkPreestablishedReferencesGIA)	//this condition has been disabled for debugging purposes
	//{
		//identify explicit same set linkages
		//eg "the guy that robbed the bank" in "the guy that robbed the bank is tall"
		fillExplicitReferenceSameSetTags(currentSentenceInList);
	//}
	#endif
#endif	

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1f; switch argument/functions where necessary" << endl;
	#endif
	switchArgumentsAndFunctionsWhereNecessary(currentSentenceInList, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 2; identify entity types [define entities as objects, subjects, and being possessive of substances];" << endl;
	#endif
	identifyEntityTypes(currentSentenceInList, GIAfeatureTempEntityNodeArray, NLPdependencyRelationsType);


	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAentityNodeArray[w] = GIAconceptNodeArray[w];		//set default values of GIAentityNodeArray
	}

#ifndef GIA_ADVANCED_REFERENCING_DEBUG_DISABLE_LINKING
	#ifdef GIA_USE_ADVANCED_REFERENCING
	if(linkPreestablishedReferencesGIA)
	{
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		cout << "\n\t\t\t error: GIA_USE_ADVANCED_REFERENCING is under development (5linkAdvancedReferencesGIA)\n" << endl;
		#endif
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 3ii; link advanced references GIA (eg the red car is fast. Mike drove the red car.)" << endl;
		#endif
		linkAdvancedReferencesGIA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, firstGIACoreferenceInList, featureArrayTemp, GIAfeatureTempEntityNodeArray, GIAconceptNodeArray);
	}
	else
	{
	#endif
		#ifdef GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
		#endif
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 3i; link pronominal references Relex (eg his/her with joe/emily)" << endl;
			#endif
			linkPronounReferencesRelex(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, GIAentityNodeArray, entityNodesActiveListConcepts, featureArrayTemp);
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		}
		#ifdef GIA_USE_STANFORD_CORENLP
		else if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 3i; link pronominal references Stanford CoreNLP (eg his/her with joe/emily)" << endl;
			#endif
			linkPronounAndTextualContextReferencesStanfordCoreNLP(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, GIAentityNodeArray, entityNodesActiveListConcepts, firstSentenceInList->firstCoreferenceInList, featureArrayTemp);
		}
		#endif
		#endif
		#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	}
	#endif
#endif
	//transfer disabled substances across execution#1 [this is required since GIAtranslatorRedistributeStanfordRelations operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of concept entity nodes {whose values would have been automatically transferred their instances upon creation}...]
	disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(GIAentityNodeArrayFilled, GIAconceptNodeArray, GIAfeatureTempEntityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "dependency relations: " << endl;
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			string relationType = currentRelationInList->relationType;
			GIAentityNode * relationGoverner = GIAconceptNodeArray[currentRelationInList->relationGovernorIndex];
			GIAentityNode * relationDependent = GIAconceptNodeArray[currentRelationInList->relationDependentIndex];

			cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
		}
		currentRelationInList = currentRelationInList->next;
	}
	cout << "features (tags): " << endl;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		//cout << w << endl;
		if(GIAentityNodeArrayFilled[w])
		{
			if(!(GIAconceptNodeArray[w]->disabled))
			{
				Feature * currentFeature = featureArrayTemp[w];
				cout << w << " = " << currentFeature->lemma << endl;
			}
		}
	}
	#endif


	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass B;" << endl;
	cout << "0a pass; define substances (objects/subjects with substances; eg 'Truffles which are picked are tasty.' - Truffle must be an instance/substance in this case); _obj(pick[4], truffle[1]), _predadj(truffle[1], tasty[6])" << endl;
	#endif
	defineSubstancesObjectsAndSubjectsWithSubstances(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, GIAfeatureTempEntityNodeArray);

	#ifdef GIA_ASSIGN_SUBSTANCE_TO_ALL_DEFINITIVE_NOUNS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0b pass; define substances (definite nouns); eg the house" << endl;
	#endif
	defineSubstancesDefiniteNouns(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);
	#endif

	/*
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_OLD
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0kSHIFTED pass; define substances (non explicit pronouns eg 'that');" << endl;
	#endif
	defineSubstancesNonExplicitPronouns(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
	#endif
	*/
	
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0c pass; define substances based on determinates of definition entities" << endl;
	#endif
	defineSubstancesBasedOnDeterminatesOfDefinitionEntities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp);
	#endif

	#ifdef GIA_ASSIGN_SUBSTANCE_TO_ALL_NOUNS_WITH_DETERMINATES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0d pass; define substances (nouns with determinates); eg a house, the house, the houses [all nouns with singular/plural are assumed to have determintes, and are therefore substances]" << endl;
	#endif
	defineSubstancesNounsWithDeterminates(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0e pass; define substances (nouns with adjectives); _amod; eg locked door, _advmod; eg cheetahs run quickly [NOT and c) _predadj; eg giants are red / joe is late]" << endl;
	#endif
	defineSubstancesNounsWithAdjectivesOrPrenominalModifiers(currentSentenceInList, GIAentityNodeArray, NLPdependencyRelationsType);
	
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0f pass; define substances (quantities [not quantity mods/multipiers, not measure pers] and measures);" << endl;
	#endif
	defineSubstancesQuantitiesAndMeasures(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0g pass; define substances (quantity mods);" << endl;
	#endif
	defineSubstancesQuantityModifiers(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0h pass; define substances (expletives eg 'there' in 'there is a place');" << endl;
	#endif
	defineSubstancesExpletives(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_ASSIGN_SUBSTANCE_TO_ALL_PRONOUNS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0i pass; define substances (pronouns eg 'we'/'I');" << endl;
	#endif
	defineSubstancesPronouns(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0j pass; define substances (to_be);" << endl;
	#endif
	defineSubstancesToBe(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0k pass; define substances (to_do);" << endl;
	#endif
	defineActionsToDo(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0l pass; define substances (has time);" << endl;
	#endif
	defineSubstancesHasTime(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, featureArrayTemp);

	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0m pass; define substances (non explicit pronouns eg 'that');" << endl;
	#endif
	defineSubstancesNonExplicitPronouns(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif

	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_4A_RELATIONS_DEFINE_SUBSTANCES_BASED_UPON_INDIRECT_OBJECTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0n pass; define substances indirect objects;" << endl;
	#endif
	defineSubstancesIndirectObjects(currentSentenceInList, GIAentityNodeArray);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0o pass; define substances (possessive prepositions, eg 'all' in 'all of the mice');" << endl;
	#endif
	defineSubstancesOfPossessivePrepositions(currentSentenceInList, GIAentityNodeArray, NLPdependencyRelationsType);

	#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0p pass; define substances concepts (ie specific concepts)" << endl;
	#endif
	defineSubstanceConcepts(GIAentityNodeArrayFilled, GIAentityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp);
	#endif 

	//Stanford version needs to be shifted after all substances have been generated (including actions)... [Upgrade translator - do not associate feature/grammatical info with concept entities; just leave them in the feature array until the concept instances have been generated]
	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "1a pass; collapseRedundantRelationAndMakeNegativeRelex (eg 'Space is saved by not having a bulky cart.'); _subj(not[5], by[4]), _subj(have[6], not[5]) [THIS PROBABLY SHOULD BE MOVED ABOVE linkPronounReferencesRelex...()]" << endl;
		#endif
		collapseRedundantRelationAndMakeNegativeRelex(currentSentenceInList, GIAentityNodeArray);
	}
	#endif

	//this function has been shifted, and applied to entity instances, not the concept entity array...
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1b [execution#2]; apply Grammatical Info To All Entities" << endl;
	#endif
 	applyGrammaticalInfoToAllEntities(GIAentityNodeArrayFilled, GIAentityNodeArray, currentSentenceInList->firstFeatureInList);

	/*
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(!(GIAentityNodeArray[w]->disabled))
			{
				cout << "entity enabled: " << GIAentityNodeArray[w]->entityName << endl;
				cout << "w = " << w << endl;
				cout << GIAentityNodeArray[w]->grammaticalNumber << endl;	
				
			}
		}
	}
	*/

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c; identify comparison variable" << endl;
	#endif
	identifyComparisonVariableAlternateMethod(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1d; disable Entities Based On Feature Temp Entity Node Array" << endl;
	#endif
	//transfer disabled substances across execution#2 [this is required since GIAtranslatorRedistributeStanfordRelations operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of concept entity nodes {whose values would have been automatically transferred their instances upon creation}...]
	disableEntitiesBasedOnFeatureTempEntityNodeArray(GIAentityNodeArrayFilled, GIAentityNodeArray, GIAfeatureTempEntityNodeArray);




	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2a pass; link properties (possessive relationships); eg Joe's bike is blue. _poss(bike[3], Joe[1]) / Hamish smoked at the toy shop. _nn(shop[6], toy[5])" << endl;
	#endif
	linkPropertiesPossessiveRelationships(currentSentenceInList, GIAentityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2b pass; link properties (descriptive relationships); eg Joe is happy.	_predadj(Joe[1], happy[3]) [NB Stanford nsubj(happy-3, Joe-1) + cop(happy-3, is-2) gets redistributed to _predadj(Joe[1], happy[3])]" << endl;
	#endif
	linkPropertiesDescriptiveRelationships(currentSentenceInList, GIAentityNodeArray, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2c pass; link entity definitions (appositive of nouns only); eg The fish, a carp, swam deeply. _appo(fish[2], carp[5])" << endl;
	#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	linkEntityDefinitionsAppositiveOfNouns(currentSentenceInList, GIAentityNodeArray, linkPreestablishedReferencesGIA);
	#else
	linkEntityDefinitionsAppositiveOfNouns(currentSentenceInList, GIAentityNodeArray);
	#endif

	#ifdef GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_TYPE1
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		//stanford only
		#ifdef GIA_TRANSLATOR_DEBUG
 		cout <<"2d pass; link conditions (dependent actions type 1), eg To swim to the beach requires strength. csubj(requires-6, swim-2) + dobj(requires-6, strength-7) " << endl;
		#endif
		linkDependentActionsType1(currentSentenceInList, GIAentityNodeArray);
	}
	#endif
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2e pass; link Having Substance Conditions And Being Definition Conditions" << endl; 
	cout << "eg1; Space is saved through/by having a chicken. prepc/prep_through/by(saved-3, having-5) + dobj(having-5, chicken-7)" << endl;	
	cout << "eg2; Space is saved through/by being a chicken. prep_through/by(saved-3, be-5) + dobj(be-5, chicken-7) 	[Relex Only - note Relex currently fails to parse 'through having' but can parse 'by having']" << endl;
	cout << "eg3; Space is saved through/by being a chicken. prepc_through/by(saved-3, chicken-7) + cop(chicken-7, being-5) 	[Stanford Only]" << endl;
	#endif
	linkHavingPropertyConditionsAndBeingDefinitionConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
 	cout <<"3a pass; link dependent subject-object definition/composition/action relationships;" << endl;
	cout << "eg1; The rabbit is 20 meters away. _subj(away[6], rabbit[2]) + _measure_distance(away[6], meter[5])" << endl;		
	cout << "eg2; A bat is an animal. _subj(be[3], bat[2]) + _obj(be[3], animal[5])" << endl;	
	cout << "eg3; The house has a table. _subj(have[3], house[2]) + _obj(have[3], table[5])" << endl;
	cout << "eg4; Tom rides the bike. _subj(ride[2], Tom[1]) + _obj(ride[2], bike[4])" << endl;	
	#endif
	linkSubjectObjectRelationships(currentSentenceInList, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
 	cout <<"3b pass; link independent subject/object action relationships; eg Tom runs quickly. _subj(run[2], Tom[1]) / The bike was ridden. _obj(ride[4], bike[2])" << endl;
	#endif
	linkSubjectOrObjectRelationships(currentSentenceInList, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3c pass; link properties (define indirect objects); eg The officer gave the youth a ride. _iobj(give, youth) +  _obj(give[3], ride[7])" << endl;
	#endif
	linkIndirectObjects(currentSentenceInList, GIAentityNodeArray);

	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_USE_STANFORD_CORENLP	//why GIA_USE_STANFORD_CORENLP and not GIA_USE_STANFORD_DEPENDENCY_RELATIONS?
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "3d pass; link Having Substance Conditions And Being Definition Conditions" << endl;
		#endif
		linkHavingPropertyConditionsAndBeingDefinitionConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);
		#endif
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3e pass; link object/subject of preposition; eg The garage is next to the house. _pobj(next_to, house)  + _psubj(next_to, garage) [appears to be Relex only]" << endl;
	#endif
	linkObjectSubjectOfPreposition(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_EXPLICITLY_ADD_CONJUNCTION_CONDITIONS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3f pass; link conjunction conditions; eg Tom and/or Max eat the cake. conj_and(Tom[1], Max[3]) / conj_or(Tom[2], Max[4])" << endl;
	#endif
	linkConjunctionConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3g pass; link conditions; eg Joe is sad at the park. at(sad[3], park[6])" << endl;
	#endif
	linkConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_TYPE2
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		//stanford only
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "3h pass; link conditions (dependent actions type 2); eg To copy the files[, ]create a backup of the old file.	dep(create-6, copy-2)" << endl;
		#endif
		linkDependentActionsType2(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);
	}
	#endif	

	//Stanford version has been shifted to after all substances have been generated (including actions)... [Upgrade translator - do not associate feature/grammatical info with concept entities; just leave them in the feature array until the concept instances have been generated]
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "4a pass; collapseRedundantRelationAndMakeNegativeStanford (eg The chicken has not eaten a pie.: neg(eaten-5, not-4)" << endl;
		#endif
		collapseRedundantRelationAndMakeNegativeStanford(currentSentenceInList, GIAentityNodeArray);
	}
	#endif


	#ifdef GIA_TRANSLATOR_DEBUG
	cout <<"4b pass; extract dates" << endl;	//[this could be implemented/"shifted" to an earlier execution stage with some additional configuration]
	#endif
	extractDates(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4c pass; extract quantities" << endl;
	#endif
	extractQuantities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4d pass; extract measures and link properties (measure-quantity relationships)" << endl;
	#endif
	extractMeasures(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4e/4f pass; define to_be/to_do conditions" << endl;
	#endif
	defineToBeAndToDoPropertiesAndConditions(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4g pass; extract qualities" << endl;
	#endif
	extractQualities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4h pass; link properties (parataxis); eg the guy, Akari said, left..." << endl;
	#endif
	linkPropertiesParataxis(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);

	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_USE_STANFORD_CORENLP
		#ifndef GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "4i pass; define Clausal Complement Properties (ccomp); eg ccomp(say, like)	He says that you like to swim" << endl;
		#endif
		defineClausalComplementProperties(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray);
		#endif
		#endif
	}

	#ifdef GIA_USE_ADVANCED_REFERENCING
	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNodeArray[w]->entityIndexTemp = w;
			GIAentityNodeArray[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;	//LIMITATION: this will not assign sentence indicies for prepositions...
			#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
			//record sentenceIndex for concept entity nodes also (NB cannot use GIAconceptNodeArray here as it won't include concept entity nodes for prepositions)
			if(!(GIAentityNodeArray[w]->entityNodeDefiningThisInstance->empty()))
			{
				#ifdef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				GIAentityNode * instanceEntity = GIAentityNodeArray[w];
				for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntity->entityNodeDefiningThisInstance->begin(); connectionIter != instanceEntity->entityNodeDefiningThisInstance->end(); connectionIter++)
				{
					GIAentityNode * conceptNode = (*connectionIter)->entity;
				#else
					GIAentityNode * conceptNode = getPrimaryConceptNodeDefiningInstance(GIAentityNodeArray[w]);
				#endif
				
					if(conceptNode->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
					{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network 
						//cout << "assigning: " <<  currentSentenceInList->sentenceIndex << endl;
						conceptNode->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
					}
					/*No problem detected here:
					if(conceptNode->sentenceIndexTemp == 0)
					{
						cout << "error: invalid sentence id" << endl;
					}
					*/
				#ifdef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE	
				}
				#endif
			}
			#endif
			
			#ifdef GIA_ADVANCED_REFERENCING_DEBUG
			//cout << GIAentityNodeArray[w]->entityName << ", w = " << w << endl;
			#endif
		}
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "record sentence nodes as permanent if they are still enabled" << endl;
	#endif
	//recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(GIAentityNodeArrayFilled, GIAconceptNodeArray);		//this method is not sufficient, as some concept entity nodes (eg prepositions/conditions) are not contained within GIAconceptNodeArray
	recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(entityNodesActiveListConcepts);
	#ifdef GIA_TRANSLATOR_DEBUG
	/*
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListIter2;
	for(conceptEntityNodesListIter2 = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter2 != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter2++)
	{
		GIAentityNode * entityNode = conceptEntityNodesListIter2->second;
		cout << "entityNode->disabled = " << entityNode->entityName << ", " << int(entityNode->disabled) << endl;
	}
	*/
	#endif

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	#ifdef GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_SENTENCE
	//restore critical variables: used for GIA translator reference paser only - cleared every time a new sentence is parsed
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListIter;
	for(conceptEntityNodesListIter = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter++)
	{
		GIAentityNode * entityNode = conceptEntityNodesListIter->second;
		entityNode->entityAlreadyDeclaredInThisContext = false;
	}
	#endif
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	//required to reset wasReferenceTemp for next time
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNodeArray[w]->wasReferenceTemp = false;
		}
	}
	#endif
	
	/*
	//test code only;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(!(GIAentityNodeArray[w]->disabled))
			{
				cout << "entity enabled: " << GIAentityNodeArray[w]->entityName << endl;
				cout << "w" << endl;
				cout << GIAentityNodeArray[w]->grammaticalNumber << endl;	
			}
		}
	}
	*/
	


}


void disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], GIAentityNode * GIAfeatureTempEntityNodeArray[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(GIAfeatureTempEntityNodeArray[w]->disabled)
			{
				disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[w]);
			}
		}
	}
}
void disableEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], GIAentityNode * GIAfeatureTempEntityNodeArray[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(GIAfeatureTempEntityNodeArray[w]->disabled)
			{
				disableEntity(GIAentityNodeArray[w]);
			}
		}
	}
}


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
 * File Name: GIATranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1q8b 07-November-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIATimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIATranslator.h"
#include "GIATranslatorOperations.h"
#include "GIATranslatorDefineGrammar.h"
#include "GIATranslatorRedistributeStanfordRelations.h"
#include "GIATranslatorDefineReferencing.h"
#include "GIATranslatorDefineSubstances.h"
#include "GIATranslatorLinkEntities.h"
#include "GIATranslatorApplyAdvancedFeatures.h"
#include "GIAdatabase.h"
#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
#include "GIAParser.h"
#endif

static int referenceTypeHasDeterminateCrossReferenceNumberArray[GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_SINGULAR};


void initialiseGIATranslatorForTexualContext()
{
	initialiseGIATranslatorForTexualContextOperations();
}


#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
void convertParagraphSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, Paragraph * firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	Paragraph * currentParagraphInList = firstParagraphInList;
	while(currentParagraphInList->next != NULL)
	{
		Sentence * firstSentenceInList = currentParagraphInList->firstSentenceInList;

		convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);

		#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
		#ifdef GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_PARAGRAPH
		//restore critical variables: used for GIA translator reference paser only - cleared every time a new paragraph is parsed
		unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListIter;
		for(conceptEntityNodesListIter = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter++)
		{
			GIAEntityNode * entityNode = conceptEntityNodesListIter->second;
			entityNode->entityAlreadyDeclaredInThisContext = false;
		}

		/*
		long vectorSize = entityNodesActiveListConcepts->size();
		for(int entityIndex=0; entityIndex<vectorSize; entityIndex++)
		{
			GIAEntityNode * entityNode = entityNodesActiveListConcepts->at(entityIndex);
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
void convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
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
		entityNodesActiveListConcepts = new unordered_map<string, GIAEntityNode*>;
		timeConditionNodesActiveList = new unordered_map<long, GIATimeConditionNode*>;
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
		vector<GIAEntityNode*> *sentenceConceptEntityNodesList = &(currentCodeextensionInHeirachy->relevantConceptEntityNodeList);
		setAllCodeextensionEntitiesInHeirachyToUndeclaredInThisContext(firstCodeextensionInHeirachy);
		setParentCodeextensionEntitiesAsAlreadyDeclaredInThisContext(currentCodeextensionInHeirachy);
		convertSentenceRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, sentenceConceptEntityNodesList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		#else
		//convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		convertCESentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
		#endif
	#ifdef GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS
	}
	#endif
}
#endif

void convertSentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	Sentence * currentSentenceInList = firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);

		currentSentenceInList = currentSentenceInList->next;
	}
}

#ifdef USE_CE
void convertCESentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
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


void convertSentenceRelationsIntoGIAnetworkNodesWrapper(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
#ifdef GIA_USE_ADVANCED_REFERENCING

	#ifdef GIA_USE_DATABASE
	int useDatabaseOriginal = getUseDatabase();
	#endif
	
	#ifdef GIA_FREE_MEMORY2
	vector<GIAEntityNode*> * entityNodesActiveListCompleteOriginal = getTranslatorEntityNodesCompleteList();
	unordered_map<string, GIAEntityNode*> * entityNodesActiveListCompleteFastIndexDBactiveOriginal;
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{
		getDBentityNodesActiveListCompleteFastIndexDBactive();	
	}
	vector<GIAEntityNode*> * entityNodesActiveListSubstancesOriginal = getTranslatorSubstanceEntityNodesList();
	vector<GIAEntityNode*> * entityNodesActiveListActionsOriginal = getTranslatorActionEntityNodesList();
	vector<GIAEntityNode*> * entityNodesActiveListConditionsOriginal = getTranslatorConditionEntityNodesList();
	vector<GIAEntityNode*> * entityNodesActiveListCompleteTemp = new vector<GIAEntityNode*>;
	unordered_map<string, GIAEntityNode*> * entityNodesActiveListCompleteFastIndexDBactiveTemp;
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{
		entityNodesActiveListCompleteFastIndexDBactiveTemp = new unordered_map<string, GIAEntityNode*>;
	}
	vector<GIAEntityNode*> * entityNodesActiveListSubstancesTemp = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * entityNodesActiveListActionsTemp = new vector<GIAEntityNode*>;
	vector<GIAEntityNode*> * entityNodesActiveListConditionsTemp = new vector<GIAEntityNode*>;
	setTranslatorEntityNodesCompleteList(entityNodesActiveListCompleteTemp);
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{
		setDBentityNodesActiveListCompleteFastIndexDBactive(entityNodesActiveListCompleteFastIndexDBactiveTemp);
	}
	setTranslatorSubstanceEntityNodesList(entityNodesActiveListSubstancesTemp);
	setTranslatorActionEntityNodesList(entityNodesActiveListActionsTemp);
	setTranslatorConditionEntityNodesList(entityNodesActiveListConditionsTemp);
	long currentEntityNodeIDInCompleteListOriginal = *(getCurrentEntityNodeIDInCompleteList());
	long currentEntityNodeIDInConceptEntityNodesListOriginal = *(getCurrentEntityNodeIDInConceptEntityNodesList());	
	long currentEntityNodeIDInSubstanceEntityNodesListOriginal = *(getCurrentEntityNodeIDInSubstanceEntityNodesList());
	long currentEntityNodeIDInActionEntityNodesListOriginal = *(getCurrentEntityNodeIDInActionEntityNodesList());
	long currentEntityNodeIDInConditionEntityNodesListOriginal = *(getCurrentEntityNodeIDInConditionEntityNodesList());
	long currentEntityNodeIDInCompleteListTemp = 0;
	long currentEntityNodeIDInConceptEntityNodesListTemp = 0;	
	long currentEntityNodeIDInSubstanceEntityNodesListTemp = 0;
	long currentEntityNodeIDInActionEntityNodesListTemp = 0;
	long currentEntityNodeIDInConditionEntityNodesListTemp = 0;
	*(getCurrentEntityNodeIDInCompleteList()) = currentEntityNodeIDInCompleteListTemp;
	*(getCurrentEntityNodeIDInConceptEntityNodesList()) = currentEntityNodeIDInConceptEntityNodesListTemp;	
	*(getCurrentEntityNodeIDInSubstanceEntityNodesList()) = currentEntityNodeIDInSubstanceEntityNodesListTemp;
	*(getCurrentEntityNodeIDInActionEntityNodesList()) = currentEntityNodeIDInActionEntityNodesListTemp;
	*(getCurrentEntityNodeIDInConditionEntityNodesList()) = currentEntityNodeIDInConditionEntityNodesListTemp;
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
	unordered_map<string, GIAEntityNode*> * sentenceConceptEntityNodesList = new unordered_map<string, GIAEntityNode*>;
	unordered_map<long, GIATimeConditionNode*> * sentenceTimeConditionNodesList = new unordered_map<long, GIATimeConditionNode*>;
	long * currentEntityNodeIDInSentenceCompleteList = getCurrentEntityNodeIDInSentenceCompleteList();
	long * currentEntityNodeIDInSentenceConceptEntityNodesList = getCurrentEntityNodeIDInSentenceConceptEntityNodesList();
	Sentence * currentSentenceInListTemp = new Sentence();
	copySentences(currentSentenceInList, currentSentenceInListTemp);
	*currentEntityNodeIDInSentenceCompleteList = 0;
	*currentEntityNodeIDInSentenceConceptEntityNodesList = 0;
	vector<GIAEntityNode*> sentenceConceptEntityNodesListTempNotUsed1;
	convertSentenceRelationsIntoGIAnetworkNodes(sentenceConceptEntityNodesList, sentenceTimeConditionNodesList, firstSentenceInList, currentSentenceInListTemp, &sentenceConceptEntityNodesListTempNotUsed1, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, false, NULL);

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\n\t\t\t error: GIA_USE_ADVANCED_REFERENCING is under development (2identifyReferenceSets)\n" << endl;
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	/*
	vector<GIAEntityNode*> ::iterator sentenceConceptEntityNodesListTempNotUsed1Iter;
	for(sentenceConceptEntityNodesListTempNotUsed1Iter = sentenceConceptEntityNodesListTempNotUsed1.begin(); sentenceConceptEntityNodesListTempNotUsed1Iter != sentenceConceptEntityNodesListTempNotUsed1.end(); sentenceConceptEntityNodesListTempNotUsed1Iter++)
	{
		GIAEntityNode * entityNode = *sentenceConceptEntityNodesListTempNotUsed1Iter;
		cout << "DEBUG1: entityNode->entityName = " << entityNode->entityName << endl;
	}
	unordered_map<string, GIAEntityNode*> ::iterator sentenceConceptEntityNodesListIter;
	for(sentenceConceptEntityNodesListIter = sentenceConceptEntityNodesList->begin(); sentenceConceptEntityNodesListIter != sentenceConceptEntityNodesList->end(); sentenceConceptEntityNodesListIter++)
	{
		GIAEntityNode * entityNode = sentenceConceptEntityNodesListIter->second;
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

	createGIACoreferenceInListBasedUponIdentifiedReferenceSets(sentenceConceptEntityNodesList, entityNodesActiveListConcepts, firstGIACoreferenceInList, numberReferenceSets);

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
	*(getCurrentEntityNodeIDInCompleteList()) = currentEntityNodeIDInCompleteListOriginal;
	*(getCurrentEntityNodeIDInSubstanceEntityNodesList()) = currentEntityNodeIDInSubstanceEntityNodesListOriginal;
	*(getCurrentEntityNodeIDInConceptEntityNodesList()) = currentEntityNodeIDInConceptEntityNodesListOriginal;	
	*(getCurrentEntityNodeIDInActionEntityNodesList()) = currentEntityNodeIDInActionEntityNodesListOriginal;
	*(getCurrentEntityNodeIDInConditionEntityNodesList()) = currentEntityNodeIDInConditionEntityNodesListOriginal;
	#endif
		
	vector<GIAEntityNode*> sentenceConceptEntityNodesListTempNotUsed;
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
			unordered_map<string, GIAEntityNode*> sentenceConceptEntityNodesListTempNotUsedMap;
			for(vector<GIAEntityNode*>::iterator sentenceConceptEntityNodesListTempNotUsedIter = sentenceConceptEntityNodesListTempNotUsed.begin(); sentenceConceptEntityNodesListTempNotUsedIter != sentenceConceptEntityNodesListTempNotUsed.end(); sentenceConceptEntityNodesListTempNotUsedIter++)
			{
				GIAEntityNode * conceptEntityNodeTemp = *sentenceConceptEntityNodesListTempNotUsedIter;
				string entityNodeNameTemp = conceptEntityNodeTemp->entityName;
				sentenceConceptEntityNodesListTempNotUsedMap.insert(pair<string, GIAEntityNode*>(entityNodeNameTemp, conceptEntityNodeTemp));
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

	vector<GIAEntityNode*> sentenceConceptEntityNodesListTempNotUsed;
	convertSentenceRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);

#endif



}

#ifdef GIA_USE_ADVANCED_REFERENCING
void convertSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAEntityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool linkPreestablishedReferencesGIA,  GIACoreference * firstGIACoreferenceInList)
#else
void convertSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAEntityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
#endif
{
	Relation * currentRelationInList;

	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "\t" << currentSentenceInList->sentenceText << endl;
	#endif

	bool GIAEntityNodeArrayFilled[MAX_NUMBER_OF_WORDS_PER_SENTENCE];		//NB could also use currentSentence->maxNumberOfWordsInSentence
	GIAEntityNode * GIAConceptNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAEntityNode * GIAEntityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAEntityNode * GIAFeatureTempEntityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];	//temporary array introduced 14 July 2012b, to handle grammatical information specific to instances of concept entities (not concept entities themselves) before those intances have been defined (in GIATranslatorDefineSubstances.cpp)

	//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
	//bool GIAEntityNodeArrayHasAssociatedSubstance[MAX_NUMBER_OF_WORDS_PER_SENTENCE];

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAEntityNodeArrayFilled[w] = false;
		GIAConceptNodeArray[w] = NULL;		//added 10 May 2012
		GIAEntityNodeArray[w] = NULL;		//added 10 May 2012
		GIAFeatureTempEntityNodeArray[w] = NULL;	//added  14 July 2012b

		//added 1 May 11a (assign actions to instances (substances) of entities and not entities themselves where appropriate)
		//GIAEntityNodeArrayHasAssociatedSubstance[w] = false;

		//GIAActionNodeArrayFilled[w] = false;
	}


	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass A;" << endl;
	cout << "pass 0; locate/add all entities [execution#1]" << endl;
	#endif
	locateAndAddAllFeatureTempEntities(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray, NLPdependencyRelationsType, NLPfeatureParser);

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
		fillGrammaticalArraysStanford(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
	}
	#endif
	//need to transfer featureArrayTemp->feature->entityDisabled information into GIAEntityNodeArray...

	//this function is first only executed in a temporary fashion (where its output is only used by relex linkPronounReferencesRelex())
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1b; applyGrammaticalInfoToAllEntities [execution#1]" << endl;
	#endif
 	applyGrammaticalInfoToAllEntities(GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray, currentSentenceInList->firstFeatureInList);

	/*
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
	cout << "dependency relations: " << endl;
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			string relationType = currentRelationInList->relationType;
			GIAEntityNode * relationGoverner = GIAConceptNodeArray[currentRelationInList->relationGovernorIndex];
			GIAEntityNode * relationDependent = GIAConceptNodeArray[currentRelationInList->relationDependentIndex];

			cout << "relationType = " << currentRelationInList->relationType << endl;
			cout << "relationGoverner = " << relationGoverner->entityName << endl;
			cout << "relationDependent = " << relationDependent->entityName << endl;

		}
		currentRelationInList = currentRelationInList->next;
	}
	cout << "features (tags): " << endl;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAEntityNodeArrayFilled[w])
		{
			if(!(GIAConceptNodeArray[w]->disabled))
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

				cout << "NER = " << featureNERTypeArray[currentFeature->NER];
				cout << "NormalizedNER = " << currentFeature->NormalizedNER;
				cout << "Timex = " << currentFeature->Timex;
				cout << "POS = " << currentFeature->stanfordPOS << endl;
			}
		}
	}
	#endif

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAEntityNodeArrayFilled[w])
		{
			cout << "GIAConceptNodeArray[w]->disabled = " << GIAConceptNodeArray[w]->entityName << ", " << int(GIAConceptNodeArray[w]->disabled) << endl;
		}
	}
	*/



	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
			
		#ifdef GIA_USE_STANFORD_CORENLP
		if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout <<"pass 1c0; disable redundant nodes Stanford Core NLP/Parser" << endl;	//[this could be implemented/"shifted" to an earlier execution stage with some additional configuration]
			#endif
			disableRedundantNodesStanfordCoreNLP(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);
		}
		#endif
		disableRedundantNodesStanfordParser(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);


		#ifdef GIA_LRP_DISABLE_REDISTRIBUTE_RELATIONS_POST_NLP_MULTIWORD_PREPOSITION_REDUCTION
		if(!(getUseLRP()))
		{
		#endif
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 1c1; redistribute Stanford Relations NSubj And Preposition" << endl;
			#endif
			redistributeStanfordRelationsMultiwordPreposition(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);
		#ifdef GIA_LRP_DISABLE_REDISTRIBUTE_RELATIONS_POST_NLP_MULTIWORD_PREPOSITION_REDUCTION
		}
		#endif
		
		//added 8 August 2012
		#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1cZa; redistribute Relations - prep_of (eg The ball of the red dog is green..   nsubj(green-8, ball-2) / prep_of(ball-2, dog-6) ->  nsubj(green-7, ball-5) / poss(ball-5, dog-3)" << endl;
		#endif
		redistributeStanfordRelationsInterpretOfAsPossessive(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);
		#endif
		#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_WHAT_IS_THE_NAME_NUMBER_OF_QUERIES
		if(currentSentenceInList->isQuestion)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 1cZb; redistribute Relations - what is the name/number of? 	nsubj(is-2, name-4) / attr(is-2, What-1) {/ det(name-4, the-3)} / poss/prep_of(name-4, dog-8) -> appos(That-1, _$qVar[1])" << endl;
			#endif
			redistributeStanfordRelationsCreateQueryVarsWhatIsTheNameNumberOf(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);
		}
		#endif
		#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1cZc; redistribute Relations - intepret name of as definition (eg interpret 'The red dog's name is Max.' / 'The name of the red dog is Max.'	nsubj(Max-7, name-5) / poss/prep_of(name-5, dog-3) -> appos(dog-3, Max-7)" << endl;
		#endif
		redistributeStanfordRelationsInterpretNameOfAsDefinition(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);
		#endif

		

		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c2; redistribute Stanford Relations -Collapse Advmod Relation Function Be (eg The rabbit is 20 meters away. 	nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - > _predadj(rabbit-2, away-6)   +    Kane is late.	nsubj(late-3, Kane-1) / cop(late-3, is-2) -> _predadj(kane-1, late-3)          +    She is the one     nsubj(one-4, She-1) /cop(one-4, is-2) / det(one-4, the-3) -> appos(She-1, one-4)" << endl;
		//[OLD: nsubj(is-3, rabbit-2) / advmod(is-3, away-6) - >nsubj(away-6, rabbit-2)] )" << endl;
		#endif
		redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray, NLPfeatureParser);
		#endif

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c3; redistribute Stanford Relations - Adverbal Clause Modifier And Complement (eg The accident happened as the night was falling. 	advcl(happen, fall) / mark(fall, as))" << endl;
		#endif
		redistributeStanfordRelationsAdverbalClauseModifierAndComplement(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c4; redistribute Stanford Relations - Clausal Subject (eg What she said makes sense. 	csubj (make, say)/dobj ( said-3 , What-1 ))" << endl;
		#endif
		redistributeStanfordRelationsClausalSubject(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);

		
		//#ifdef GIA_TRANSLATOR_DEBUG
		//cout << "pass 1c5; redistribute Stanford Relations - Phrasal Verb Particle (eg They shut down the station. 	prt(shut, down))" << endl;
		//#endif
		//redistributeStanfordRelationsPhrasalVerbParticle(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);


		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c6; redistribute Stanford Relations - Conjunction And Coordinate (eg I eat a pie or tom rows the boat. 	cc(pie-4, or-5) / conj(pie-4, tom-6))" << endl;
		#endif
		redistributeStanfordRelationsConjunctionAndCoordinate(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c7; redistribute Stanford Relations - Generate Unparsed Quantity Modifers (eg The punter won almost $1000. 	advmod(won-3, almost-4) / pobj(almost-4, $-5)) / num($-5, 1000-6)" << endl;
		#endif
		redistributeStanfordRelationsGenerateUnparsedQuantityModifers(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);
		
		
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c8; redistribute Stanford Relations - Generate Measures (eg years old - npadvmod(old, years) -> _measure_time(old[7], years[6]))" << endl;
		#endif
		redistributeStanfordRelationsGenerateMeasures(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);
		#endif

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c9; redistribute Stanford Relations - Prt And Tmods (eg The disaster happened over night.   prt(happened-3, over-4) / tmod(happened-3, night-5) -> over(happened-3, night-5) )" << endl;
		#endif
		redistributeStanfordRelationsPhrasalVerbParticle(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);


		if(currentSentenceInList->isQuestion)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 1c10; redistribute Stanford Relations - Create Query Vars (eg interpret 'who is that' / 'what is the time.'  attr(is-2, Who-1) / attr(is-2, What-1) | interpret 'how much'/'how many' | interpret 'which' det(house-2, Which-1) | interpret how/when/where/why advmod(happen-5, How-1) / advmod(leave-4, When-1) / advmod(is-2, Where-1) / advmod(fall-5, Why-1)	 )" << endl;
			#endif
			redistributeStanfordRelationsCreateQueryVars(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);
		}

		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c11; redistribute Stanford Relations - partmod (eg Truffles picked during the spring are tasty.   partmod(truffle, pick) -> obj(pick, truffle) )" << endl;
		#endif
		redistributeStanfordRelationsPartmod(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray);

		#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
		//Added 28 October 2012b
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1c12; redistribute Stanford Relations - Interpret Of As Object For ContinuousVerb (eg What is wood used in the delivering of?   interpret prep_of(xing, y) as obj(xing, y) )" << endl;
		#endif
		redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray, NLPdependencyRelationsType);
		#endif
	}
	#endif


	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1d; locate/add all entities [execution#2]" << endl;
	#endif
	locateAndAddAllConceptEntities(GIAEntityNodeArrayFilled, GIAConceptNodeArray, entityNodesActiveListConcepts, sentenceConceptEntityNodesList, NLPdependencyRelationsType, NLPfeatureParser, GIAFeatureTempEntityNodeArray);


	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
	cout << "dependency relations: " << endl;
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			string relationType = currentRelationInList->relationType;
			GIAEntityNode * relationGoverner = GIAConceptNodeArray[currentRelationInList->relationGovernorIndex];
			GIAEntityNode * relationDependent = GIAConceptNodeArray[currentRelationInList->relationDependentIndex];

			cout << "relationType = " << currentRelationInList->relationType << endl;
			cout << "relationGoverner = " << relationGoverner->entityName << endl;
			cout << "relationDependent = " << relationDependent->entityName << endl;

		}
		currentRelationInList = currentRelationInList->next;
	}
	cout << "features (tags): " << endl;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAEntityNodeArrayFilled[w])
		{
			if(!(GIAConceptNodeArray[w]->disabled))
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

				cout << "NER = " << featureNERTypeArray[currentFeature->NER];
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

	#ifdef GIA_USE_ADVANCED_REFERENCING
	//if(!linkPreestablishedReferencesGIA)	//this condition has been disabled for debugging purposes
	//{
		//identify explicit same set linkages
		//eg "the guy that robbed the bank" in "the guy that robbed the bank is tall"
		fillExplicitReferenceSameSetTags(currentSentenceInList);
	//}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1f; switch argument/functions where necessary" << endl;
	#endif
	switchArgumentsAndFunctionsWhereNecessary(currentSentenceInList, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 2; identify entity types [define entities as objects, subjects, and being possessive of substances];" << endl;
	#endif
	identifyEntityTypes(currentSentenceInList, GIAFeatureTempEntityNodeArray, NLPdependencyRelationsType);


	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAEntityNodeArray[w] = GIAConceptNodeArray[w];		//set default values of GIAEntityNodeArray
	}

	#ifdef GIA_USE_ADVANCED_REFERENCING
	if(linkPreestablishedReferencesGIA)
	{
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		cout << "\n\t\t\t error: GIA_USE_ADVANCED_REFERENCING is under development (5linkAdvancedReferencesGIA)\n" << endl;
		#endif
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 3ii; link advanced references GIA (eg the red car is fast. Mike drove the red car.)" << endl;
		#endif
		linkAdvancedReferencesGIA(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts, firstGIACoreferenceInList, featureArrayTemp, GIAFeatureTempEntityNodeArray, GIAConceptNodeArray);
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
			linkPronounReferencesRelex(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray, GIAEntityNodeArray, entityNodesActiveListConcepts, featureArrayTemp);
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		}
		#ifdef GIA_USE_STANFORD_CORENLP
		else if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass 3i; link pronominal references Stanford CoreNLP (eg his/her with joe/emily)" << endl;
			#endif
			linkPronounAndTextualContextReferencesStanfordCoreNLP(currentSentenceInList, GIAEntityNodeArrayFilled, GIAFeatureTempEntityNodeArray, GIAEntityNodeArray, entityNodesActiveListConcepts, firstSentenceInList->firstCoreferenceInList, featureArrayTemp);
		}
		#endif
		#endif
		#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	}
	#endif

	//transfer disabled substances across execution#1 [this is required since GIATranslatorRedistributeStanfordRelations operations are now done on temporary entity nodes GIAFeatureTempEntityNodeArray instead of concept entity nodes {whose values would have been automatically transferred their instances upon creation}...]
	disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(GIAEntityNodeArrayFilled, GIAConceptNodeArray, GIAFeatureTempEntityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "dependency relations: " << endl;
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			string relationType = currentRelationInList->relationType;
			GIAEntityNode * relationGoverner = GIAConceptNodeArray[currentRelationInList->relationGovernorIndex];
			GIAEntityNode * relationDependent = GIAConceptNodeArray[currentRelationInList->relationDependentIndex];

			cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
		}
		currentRelationInList = currentRelationInList->next;
	}
	cout << "features (tags): " << endl;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		//cout << w << endl;
		if(GIAEntityNodeArrayFilled[w])
		{
			if(!(GIAConceptNodeArray[w]->disabled))
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
	defineSubstancesObjectsAndSubjectsWithSubstances(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, GIAFeatureTempEntityNodeArray);

	#ifdef GIA_ASSIGN_SUBSTANCE_TO_ALL_DEFINITIVE_NOUNS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0b pass; define substances (definite nouns); eg the house" << endl;
	#endif
	defineSubstancesDefiniteNouns(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, featureArrayTemp);
	#endif

	/*
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_OLD
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0kSHIFTED pass; define substances (non explicit pronouns eg 'that');" << endl;
	#endif
	defineSubstancesNonExplicitPronouns(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray);
	#endif
	#endif
	*/

	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0c pass; define substances based on determinates of definition entities" << endl;
	#endif
	defineSubstancesBasedOnDeterminatesOfDefinitionEntities(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp);
	#endif

	#ifdef GIA_ASSIGN_SUBSTANCE_TO_ALL_NOUNS_WITH_DETERMINATES
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0d pass; define substances (nouns with determinates); eg a house, the house, the houses [all nouns with singular/plural are assumed to have determintes, and are therefore substances]" << endl;
	#endif
	defineSubstancesNounsWithDeterminates(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0e pass; define substances (nouns with adjectives); _amod; eg locked door, _advmod; eg cheetahs run quickly [NOT and c) _predadj; eg giants are red / joe is late]" << endl;
	#endif
	defineSubstancesNounsWithAdjectives(currentSentenceInList, GIAEntityNodeArray, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0f pass; define substances (quantities [not quantity mods/multipiers, not measure pers] and measures);" << endl;
	#endif
	defineSubstancesQuantitiesAndMeasures(currentSentenceInList, GIAEntityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0g pass; define substances (quantity mods);" << endl;
	#endif
	defineSubstancesQuantityModifiers(currentSentenceInList, GIAEntityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0h pass; define substances (expletives eg 'there' in 'there is a place');" << endl;
	#endif
	defineSubstancesExpletives(currentSentenceInList, GIAEntityNodeArray);

	#ifdef GIA_ASSIGN_SUBSTANCE_TO_ALL_PRONOUNS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0i pass; define substances (pronouns eg 'we'/'I');" << endl;
	#endif
	defineSubstancesPronouns(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, featureArrayTemp);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0j pass; define substances (to_be);" << endl;
	#endif
	defineSubstancesToBe(currentSentenceInList, GIAEntityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0k pass; define substances (to_do);" << endl;
	#endif
	defineActionsToDo(currentSentenceInList, GIAEntityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0l pass; define substances (has time);" << endl;
	#endif
	defineSubstancesHasTime(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, featureArrayTemp);

	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0m pass; define substances (non explicit pronouns eg 'that');" << endl;
	#endif
	defineSubstancesNonExplicitPronouns(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray);
	#endif

	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_4A_RELATIONS_DEFINE_SUBSTANCES_BASED_UPON_INDIRECT_OBJECTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0n pass; define substances indirect objects;" << endl;
	#endif
	defineSubstancesIndirectObjects(currentSentenceInList, GIAEntityNodeArray);
	#endif

	#ifdef GIA_SUPPORT_SPECIFIC_CONCEPTS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "0o pass; define substances concepts (ie specific concepts): 
	#endif
	defineSubstanceConcepts(GIAEntityNodeArrayFilled, GIAEntityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp);
	#endif 

	//Stanford version needs to be shifted after all substances have been generated (including actions)... [Upgrade translator - do not associate feature/grammatical info with concept entities; just leave them in the feature array until the concept instances have been generated]
	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "1a pass; collapseRedundantRelationAndMakeNegativeRelex (eg 'Space is saved by not having a bulky cart.'); _subj(not[5], by[4]), _subj(have[6], not[5]) [THIS PROBABLY SHOULD BE MOVED ABOVE linkPronounReferencesRelex...()]" << endl;
		#endif
		collapseRedundantRelationAndMakeNegativeRelex(currentSentenceInList, GIAEntityNodeArray);
	}
	#endif

	//this function has been shifted, and applied to entity instances, not the concept entity array...
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1b [execution#2]; apply Grammatical Info To All Entities" << endl;
	#endif
 	applyGrammaticalInfoToAllEntities(GIAEntityNodeArrayFilled, GIAEntityNodeArray, currentSentenceInList->firstFeatureInList);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c; identify comparison variable" << endl;
	#endif
	identifyComparisonVariableAlternateMethod(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1d; disable Entities Based On Feature Temp Entity Node Array" << endl;
	#endif
	//transfer disabled substances across execution#2 [this is required since GIATranslatorRedistributeStanfordRelations operations are now done on temporary entity nodes GIAFeatureTempEntityNodeArray instead of concept entity nodes {whose values would have been automatically transferred their instances upon creation}...]
	disableEntitiesBasedOnFeatureTempEntityNodeArray(GIAEntityNodeArrayFilled, GIAEntityNodeArray, GIAFeatureTempEntityNodeArray);




	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2a pass; link properties (possessive relationships); eg joe's bike" << endl;
	#endif
	linkPropertiesPossessiveRelationships(currentSentenceInList, GIAEntityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2b pass; link properties (descriptive relationships); eg joe is happy" << endl;
	#endif
	linkPropertiesDescriptiveRelationships(currentSentenceInList, GIAEntityNodeArray, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "2c pass; link entity definitions (appositive of nouns only)" << endl;
	#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	linkEntityDefinitionsAppositiveOfNouns(currentSentenceInList, GIAEntityNodeArray, linkPreestablishedReferencesGIA);
	#else
	linkEntityDefinitionsAppositiveOfNouns(currentSentenceInList, GIAEntityNodeArray);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
 	cout <<"3a pass; link dependent subject-object definition/composition/action relationships" << endl;
	#endif
	linkSubjectObjectRelationships(currentSentenceInList, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
 	cout <<"3b pass; link independent subject/object action relationships" << endl;
	#endif
	linkSubjectOrObjectRelationships(currentSentenceInList, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3c pass; link properties (define indirect objects)" << endl;
	#endif
	linkIndirectObjects(currentSentenceInList, GIAEntityNodeArray);

	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_USE_STANFORD_CORENLP
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "3d pass; link Having Substance Conditions And Being Definition Conditions; eg Space is saved through having a chicken	prepc_through(saved-3, having-5) / dobj(having-5, chicken-7)" << endl;
		#endif
		linkHavingPropertyConditionsAndBeingDefinitionConditions(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts);
		#endif
	}

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3e pass; link object/subject of preposition" << endl;
	#endif
	linkObjectSubjectOfPreposition(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

#ifdef GIA_TRANSLATOR_EXPLICITLY_ADD_CONJUNCTION_CONDITIONS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3f pass; define conjunction conditions; eg Either Tom and/or Max eat the cake...." << endl;
	#endif
	linkConjunctionConditions(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts);
#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "3g pass; define conditions" << endl;
	#endif
	linkConditions(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	//Stanford version has been shifted to after all substances have been generated (including actions)... [Upgrade translator - do not associate feature/grammatical info with concept entities; just leave them in the feature array until the concept instances have been generated]
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "4a pass; collapseRedundantRelationAndMakeNegativeStanford (eg The chicken has not eaten a pie.: neg(eaten-5, not-4)" << endl;
		#endif
		collapseRedundantRelationAndMakeNegativeStanford(currentSentenceInList, GIAEntityNodeArray);
	}
	#endif


	#ifdef GIA_TRANSLATOR_DEBUG
	cout <<"4b pass; extract dates" << endl;	//[this could be implemented/"shifted" to an earlier execution stage with some additional configuration]
	#endif
	extractDates(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4c pass; extract quantities" << endl;
	#endif
	extractQuantities(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4d pass; extract measures and link properties (measure-quantity relationships)" << endl;
	#endif
	extractMeasures(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4e/4f pass; define to_be/to_do conditions" << endl;
	#endif
	defineToBeAndToDoPropertiesAndConditions(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4g pass; extract qualities" << endl;
	#endif
	extractQualities(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "4h pass; link properties (parataxis); eg the guy, Akari said, left..." << endl;
	#endif
	linkPropertiesParataxis(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray);

	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_USE_STANFORD_CORENLP
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "4i pass; define Clausal Complement Properties (ccomp); eg ccomp(say, like)	He says that you like to swim" << endl;
		#endif
		defineClausalComplementProperties(currentSentenceInList, GIAEntityNodeArrayFilled, GIAEntityNodeArray);
		#endif
	}

	#ifdef GIA_USE_ADVANCED_REFERENCING
	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAEntityNodeArrayFilled[w])
		{
			GIAEntityNodeArray[w]->entityIndexTemp = w;
			GIAEntityNodeArray[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;	//LIMITATION: this will not assign sentence indicies for prepositions...
			#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
			//record sentenceIndex for concept entity nodes also (NB cannot use GIAConceptNodeArray here as it won't include concept entity nodes for prepositions)
			if(!(GIAEntityNodeArray[w]->entityNodeDefiningThisInstance->empty()))
			{
				GIAEntityNode * conceptNode = (GIAEntityNodeArray[w]->entityNodeDefiningThisInstance->back())->entity;
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
			}
			#endif
			
			#ifdef GIA_ADVANCED_REFERENCING_DEBUG
			//cout << GIAEntityNodeArray[w]->entityName << ", w = " << w << endl;
			#endif
		}
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "record sentence nodes as permanent if they are still enabled" << endl;
	#endif
	//recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(GIAEntityNodeArrayFilled, GIAConceptNodeArray);		//this method is not sufficient, as some concept entity nodes (eg prepositions/conditions) are not contained within GIAConceptNodeArray
	recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(entityNodesActiveListConcepts);
	#ifdef GIA_TRANSLATOR_DEBUG
	/*
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListIter2;
	for(conceptEntityNodesListIter2 = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter2 != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter2++)
	{
		GIAEntityNode * entityNode = conceptEntityNodesListIter2->second;
		cout << "entityNode->disabled = " << entityNode->entityName << ", " << int(entityNode->disabled) << endl;
	}
	*/
	#endif

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	#ifdef GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_SENTENCE
	//restore critical variables: used for GIA translator reference paser only - cleared every time a new sentence is parsed
	unordered_map<string, GIAEntityNode*> ::iterator conceptEntityNodesListIter;
	for(conceptEntityNodesListIter = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter++)
	{
		GIAEntityNode * entityNode = conceptEntityNodesListIter->second;
		entityNode->entityAlreadyDeclaredInThisContext = false;
	}
	#endif
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	//required to reset wasReferenceTemp for next time
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAEntityNodeArrayFilled[w])
		{
			GIAEntityNodeArray[w]->wasReferenceTemp = false;
		}
	}
	#endif


}


void disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], GIAEntityNode * GIAFeatureTempEntityNodeArray[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAEntityNodeArrayFilled[w])
		{
			if(GIAFeatureTempEntityNodeArray[w]->disabled)
			{
				disableConceptEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNodeArray[w]);
			}
		}
	}
}
void disableEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], GIAEntityNode * GIAFeatureTempEntityNodeArray[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAEntityNodeArrayFilled[w])
		{
			if(GIAFeatureTempEntityNodeArray[w]->disabled)
			{
				disableEntity(GIAEntityNodeArray[w]);
			}
		}
	}
}


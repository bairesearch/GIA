/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2i17b 30-January-2015
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslator.h"
#include "GIAtranslatorDefineGrammar.h"
/*
#ifdef GIA_TEMPORARY_COMPILATION_GCC_FLAG_E_PREPROCESS_TRANSLATOR_XML_FILES
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorRedistributeRelexRelations.xml"
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorRedistributeStanfordRelations.xml"
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorDefineSubstances.xml"
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorLinkEntities.xml"
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorApplyAdvancedFeatures.xml"
#else
*/
#include "GIAtranslatorRedistributeRelexRelations.h"
#include "GIAtranslatorRedistributeStanfordRelations.h"
#include "GIAtranslatorDefineReferencing.h"
#include "GIAtranslatorDefineSubstances.h"
#include "GIAtranslatorLinkEntities.h"
#include "GIAtranslatorApplyAdvancedFeatures.h"
/*
#endif
*/
#include "GIAdatabase.h"
#include "GIAnlp.h"	//required because parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences was shifted from GIAmain.cpp
//#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
#include "GIAnlpParser.h"
//#endif
#ifdef GIA_USE_BOT
#include "GIAbot.h"
#endif
#ifdef GIA_USE_LRP
#include "GIAlrp.h"
#endif
#ifdef GIA_USE_CORPUS_DATABASE
#include "GIAcorpusOperations.h"
#include "GIAcorpusTranslator.h"
#include "GIAcorpusDatabase.h"
#endif
#include "GIAtranslatorRules.h"
#include "GIAtranslatorGeneric.h"
#include "SHAREDvars.h"	//file io

//required because parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences was shifted from GIAmain.cpp
//Dependency Relationship Extractor
static int dependencyRelationsTypes[GIA_NLP_PARSER_NUMBER_OF_TYPES] = {GIA_NLP_DEPENDENCY_RELATIONS_PARSER_RELEX_DEFAULT_DEPENDENCY_RELATIONS_TYPE, GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_CORENLP_DEFAULT_DEPENDENCY_RELATIONS_TYPE, GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_PARSER_DEFAULT_DEPENDENCY_RELATIONS_TYPE};

static int referenceTypeHasDeterminateCrossReferenceNumberArray[GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_SINGULAR};


void initialiseGIATranslatorForTexualContext()
{
	initialiseGIATranslatorForTexualContextOperations();
}

#ifdef USE_CE
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int* maxNumberSentences, bool parseGIA2file, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, bool useCodeextensionsHeirachy)
#else
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int* maxNumberSentences, bool parseGIA2file)
#endif
{
	bool result = true;

	setCurrentGIALRPtagTextCorrespondenceInfo(isQuery);	//required for local variable access
	if(!parseGIA2file)
	{
		if(!parseNLPparserFile(inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, isQuery, firstParagraphInList, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode))
		{
			result = false;
		}
	}
	else
	{
		//semantic dependency relations and features have already been parsed
	}

	int NLPdependencyRelationsType = dependencyRelationsTypes[NLPdependencyRelationsParser];
	if(NLPrelexCompatibilityMode)
	{//untested
		if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
		{
			NLPdependencyRelationsType = GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD;
			//assume dependency relations are in Stanford format from here on in
		}
	}

	#ifdef USE_CE
	if(!createSemanticNetworkBasedUponDependencyParsedSentences(firstParagraphInList, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, entityNodesActiveListSentences, timeConditionNodesActiveList, isQuery, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file, firstCodeextensionInHeirachy, codeextensionsList, useCodeextensionsHeirachy))
	#else
	if(!createSemanticNetworkBasedUponDependencyParsedSentences(firstParagraphInList, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, entityNodesActiveListSentences, timeConditionNodesActiveList, isQuery, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file))
	#endif
	{
		result = false;
	}

	setCurrentDirectory(tempFolderCharStar);

	if(!parseGIA2file)
	{
		#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
		string originalInputFileName = "";
		originalInputFileName = originalInputFileName + inputTextNLPrelationXMLfileName + " " + inputTextNLPfeatureXMLfileName;
		outputInternalRelationsInRelexFormat(&outputCFFfileName, &originalInputFileName, firstParagraphInList, NLPdependencyRelationsParser, NLPfeatureParser, NLPexeFolderArray);
		#endif
	}

	//count maxNumberSentences
	GIAsentence* currentSentenceInList = firstParagraphInList->firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		if(currentSentenceInList->sentenceIndex >* maxNumberSentences)
		{
			*maxNumberSentences = currentSentenceInList->sentenceIndex;
		}
		currentSentenceInList = currentSentenceInList->next;
	}

	return result;
}

#ifdef USE_CE
bool createSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, bool useCodeextensionsHeirachy)
#else
bool createSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
#endif
{
	bool result = true;

	#ifdef GIA_USE_DATABASE
	int useDatabaseOriginal = getUseDatabase();
	if(isQuery)
	{
		setUseDatabase(GIA_USE_DATABASE_FALSE);
	}
	#endif

	GIAparagraph* currentParagraph = firstParagraphInList;
	GIAsentence* firstSentenceInList = firstParagraphInList->firstSentenceInList;

	setTranslatorEntityNodesCompleteList(entityNodesActiveListComplete);
	setTranslatorSubstanceEntityNodesList(entityNodesActiveListSubstances);
	setTranslatorActionEntityNodesList(entityNodesActiveListActions);
	setTranslatorConditionEntityNodesList(entityNodesActiveListConditions);

	initialiseGIATranslatorForTexualContext();

	#ifdef USE_CE
	if(useCodeextensionsHeirachy)
	{
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(firstParagraphInList->next->next != NULL)
		{
			cout << "convertParagraphSentenceRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(): error - CE only supports a single paragraph of text, one codeextension per line" << endl;
			exit(0);
		}
		#endif
		convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, firstCodeextensionInHeirachy, codeextensionsList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);
	}
	else
	{
	#endif
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
			convertParagraphSentenceRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, entityNodesActiveListSentences, timeConditionNodesActiveList, firstParagraphInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);
		}
		else
		{
		#endif
			convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, entityNodesActiveListSentences, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		}
		#endif
	#ifdef USE_CE
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "record concept nodes as disabled if they are not permanent (used for printing/xml write purposes)" << endl;
	#endif
	recordConceptNodesAsDisabledIfTheyAreNotPermanent(entityNodesActiveListConcepts);

	#ifdef GIA_USE_DATABASE
	if(isQuery)
	{
		setUseDatabase(useDatabaseOriginal);
	}
	#endif

	return result;
}


#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
void convertParagraphSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAparagraph* firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
{
	GIAparagraph* currentParagraphInList = firstParagraphInList;
	while(currentParagraphInList->next != NULL)
	{
		GIAsentence* firstSentenceInList = currentParagraphInList->firstSentenceInList;

		convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, entityNodesActiveListSentences, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);

		#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
		#ifdef GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_PARAGRAPH
		//restore critical variables: used for GIA translator reference paser only - cleared every time a new paragraph is parsed
		unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListIter;
		for(conceptEntityNodesListIter = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter++)
		{
			GIAentityNode* entityNode = conceptEntityNodesListIter->second;
			entityNode->entityAlreadyDeclaredInThisContext = false;
		}
		#endif
		#endif
		currentParagraphInList = currentParagraphInList->next;
	}

	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	dreamModeLinkSpecificConceptsAndActions();
	#endif
}
#endif


#ifdef USE_CE
//NB is convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy compatible with Stanford CoreNLP coreferences? [NB sentences are mixed up, but are sentence id's still valid? If there is a problem, then possibly need to either a) re-execute NLP on each codeextension heirachy, or b) revert to GIA pronominal coreference detection (see RelEx function to do this)
void convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	//link GIAsentences with each generated CEcodeextension
	GIAsentence* currentSentenceInList = firstSentenceInList;
	vector<CECodeextension*>::iterator codeextensionIter;
	for(codeextensionIter = codeextensionsList->begin(); codeextensionIter != codeextensionsList->end(); codeextensionIter++)
	{
		CECodeextension* currentCodeextensionInHeirachy = *codeextensionIter;
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
		CECodeextension* currentCodeextensionInHeirachy = *codeextensionIter;
		currentSentenceInList = currentCodeextensionInHeirachy->sentence;

		GIAsentence* lastSentenceInArtificialList = currentSentenceInList;
		GIAsentence* firstSentenceInArtificialList = generateArtificialSentenceListBasedUponParentCodeextensions(currentCodeextensionInHeirachy, lastSentenceInArtificialList, true);

		#ifdef GIA_WITH_CE_DEBUG
		cout << "CE DEBUG: print codeextension heirachy dependency relations for:" << endl;
		cout << "currentCodeextensionInHeirachy->codeextensionTextRaw = " << currentCodeextensionInHeirachy->codeextensionTextRaw << endl;
		#endif
		GIAsentence* currentSentenceInArtificialList = firstSentenceInArtificialList;
		do
		{
			GIArelation* currentRelationInList = currentSentenceInArtificialList->firstRelationInList;
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
		vector<GIAentityNode*>* sentenceConceptEntityNodesList = &(currentCodeextensionInHeirachy->relevantConceptEntityNodeList);
		setAllCodeextensionEntitiesInHeirachyToUndeclaredInThisContext(firstCodeextensionInHeirachy);
		setParentCodeextensionEntitiesAsAlreadyDeclaredInThisContext(currentCodeextensionInHeirachy);
		convertSentenceSyntacticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, sentenceConceptEntityNodesList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		#else
		//convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		convertCEsentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
		#endif
	#ifdef GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS
	}
	#endif
}

void convertCEsentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	GIAsentence* currentSentenceInList = firstSentenceInList;
	do
	{
		convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);

		currentSentenceInList = currentSentenceInList->next;
	}
	while(currentSentenceInList != NULL);
}
#endif

void convertSentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
{
	GIAsentence* currentSentenceInList = firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, entityNodesActiveListSentences, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);

		currentSentenceInList = currentSentenceInList->next;
	}

	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	dreamModeLinkSpecificConceptsAndActions();
	#endif
}



void convertSentenceRelationsIntoGIAnetworkNodesWrapper(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
{	
	#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
	setCurrentSentenceIndex(currentSentenceInList->sentenceIndex);
	#endif
	
#ifdef GIA_USE_ADVANCED_REFERENCING

	#ifdef GIA_USE_DATABASE
	int useDatabaseOriginal = getUseDatabase();
	#endif

	#ifdef GIA_FREE_MEMORY2
	vector<GIAentityNode*>* entityNodesActiveListCompleteOriginal = getTranslatorEntityNodesCompleteList();
	unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndexDBactiveOriginal;
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{
		entityNodesActiveListCompleteFastIndexDBactiveOriginal = getDBentityNodesActiveListCompleteFastIndexDBactive();
	}
	vector<GIAentityNode*>* entityNodesActiveListSubstancesOriginal = getTranslatorSubstanceEntityNodesList();
	vector<GIAentityNode*>* entityNodesActiveListActionsOriginal = getTranslatorActionEntityNodesList();
	vector<GIAentityNode*>* entityNodesActiveListConditionsOriginal = getTranslatorConditionEntityNodesList();
	vector<GIAentityNode*>* entityNodesActiveListCompleteTemp = new vector<GIAentityNode*>;
	unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndexDBactiveTemp;
	if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
	{
		entityNodesActiveListCompleteFastIndexDBactiveTemp = new unordered_map<string, GIAentityNode*>;
	}
	vector<GIAentityNode*>* entityNodesActiveListSubstancesTemp = new vector<GIAentityNode*>;
	vector<GIAentityNode*>* entityNodesActiveListActionsTemp = new vector<GIAentityNode*>;
	vector<GIAentityNode*>* entityNodesActiveListConditionsTemp = new vector<GIAentityNode*>;
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
	cout << "\n\t\t\t GIA_USE_ADVANCED_REFERENCING_DEBUG (1convertSentenceRelationsIntoGIAnetworkNodes)\n" << endl;
	#endif

	#ifndef GIA_FREE_MEMORY2
	setSaveNetwork(false);
	#endif
	#ifdef GIA_USE_DATABASE
	setUseDatabase(GIA_USE_DATABASE_FALSE);
	#endif
	#ifdef GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
	setLinkPreestablishedReferencesGIA(false);
	#endif

	GIACoreference* firstGIACoreferenceInList = new GIACoreference();
	unordered_map<string, GIAentityNode*>* sentenceConceptEntityNodesList = new unordered_map<string, GIAentityNode*>;
	unordered_map<long, GIAtimeConditionNode*>* sentenceTimeConditionNodesList = new unordered_map<long, GIAtimeConditionNode*>;
	long* currentEntityNodeIDInSentenceCompleteList = getCurrentEntityNodeIDinSentenceCompleteList();
	long* currentEntityNodeIDInSentenceConceptEntityNodesList = getCurrentEntityNodeIDinSentenceConceptEntityNodesList();
	GIAsentence* currentSentenceInListTemp = new GIAsentence();
	copySentences(currentSentenceInList, currentSentenceInListTemp);
	*currentEntityNodeIDInSentenceCompleteList = 0;
	*currentEntityNodeIDInSentenceConceptEntityNodesList = 0;
	vector<GIAentityNode*> sentenceConceptEntityNodesListTempNotUsed1;
	#ifdef GIA_USE_CORPUS_DATABASE
	if(parseGIA2file)
	{
		if(currentSentenceInList->corpusLookupSuccessful)
		{
			convertSentenceSemanticRelationsIntoGIAnetworkNodes(sentenceConceptEntityNodesList, sentenceTimeConditionNodesList, firstSentenceInList, currentSentenceInListTemp, &sentenceConceptEntityNodesListTempNotUsed1, entityNodesActiveListSentences, NLPfeatureParser, false, NULL);
			//cout << "2 corpusLookupSuccessful" << endl;
		}
	}
	#endif
	if(!parseGIA2file || (parseGIA2file && !(currentSentenceInList->corpusLookupSuccessful)))
	{
		//cout << "2 !corpusLookupSuccessful" << endl;
		convertSentenceSyntacticRelationsIntoGIAnetworkNodes(sentenceConceptEntityNodesList, sentenceTimeConditionNodesList, firstSentenceInList, currentSentenceInListTemp, &sentenceConceptEntityNodesListTempNotUsed1, entityNodesActiveListSentences, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, false, NULL);
	}

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\n\t\t\t GIA_USE_ADVANCED_REFERENCING_DEBUG (2identifyReferenceSets)\n" << endl;
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	/*
	vector<GIAentityNode*> ::iterator sentenceConceptEntityNodesListTempNotUsed1Iter;
	for(sentenceConceptEntityNodesListTempNotUsed1Iter = sentenceConceptEntityNodesListTempNotUsed1.begin(); sentenceConceptEntityNodesListTempNotUsed1Iter != sentenceConceptEntityNodesListTempNotUsed1.end(); sentenceConceptEntityNodesListTempNotUsed1Iter++)
	{
		GIAentityNode* entityNode = *sentenceConceptEntityNodesListTempNotUsed1Iter;
		cout << "DEBUG1: entityNode->entityName = " << entityNode->entityName << endl;
	}
	unordered_map<string, GIAentityNode*> ::iterator sentenceConceptEntityNodesListIter;
	for(sentenceConceptEntityNodesListIter = sentenceConceptEntityNodesList->begin(); sentenceConceptEntityNodesListIter != sentenceConceptEntityNodesList->end(); sentenceConceptEntityNodesListIter++)
	{
		GIAentityNode* entityNode = sentenceConceptEntityNodesListIter->second;
		cout << "DEBUG2: entityNode->entityName = " << entityNode->entityName << endl;
	}
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		cout << "GIArelation: " << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
		currentRelationInList = currentRelationInList->next;
	}
	*/
	#endif

	vector<GIAentityNode*> referenceSetDefiniteEntityList;
	int numberReferenceSets = identifyReferenceSets(sentenceConceptEntityNodesList, NLPdependencyRelationsType, &referenceSetDefiniteEntityList);	//NB NLPdependencyRelationsType is no longer used here

	#ifdef GIA_USE_DATABASE
	setUseDatabase(useDatabaseOriginal);
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\n\t\t\t GIA_USE_ADVANCED_REFERENCING_DEBUG (3createGIACoreferenceInListBasedUponIdentifiedReferenceSets)\n" << endl;
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
	cout << "createGIAcoreferenceInListBasedUponIdentifiedReferenceSets:" << endl;
	#endif
	createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(sentenceConceptEntityNodesList, entityNodesActiveListConcepts, firstGIACoreferenceInList, &referenceSetDefiniteEntityList);

	#ifndef GIA_FREE_MEMORY2
	setSaveNetwork(true);
	#endif
	#ifdef GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
	setLinkPreestablishedReferencesGIA(true);
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\n\t\t\t GIA_USE_ADVANCED_REFERENCING_DEBUG (4convertSentenceRelationsIntoGIAnetworkNodes)\n" << endl;
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
	#ifdef GIA_USE_CORPUS_DATABASE
	if(parseGIA2file)
	{
		if(currentSentenceInList->corpusLookupSuccessful)
		{
			convertSentenceSemanticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, entityNodesActiveListSentences, NLPfeatureParser, true, firstGIACoreferenceInList);
		}
	}
	#endif
	if(!parseGIA2file || (parseGIA2file && !(currentSentenceInList->corpusLookupSuccessful)))
	{
		convertSentenceSyntacticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, entityNodesActiveListSentences, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, true, firstGIACoreferenceInList);
	}

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
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS
	cout << "\n\nfor colours only..." << endl;
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS_WORK_WITH_MULTIPLE_SENTENCES
			unordered_map<string, GIAentityNode*> sentenceConceptEntityNodesListTempNotUsedMap;
			for(vector<GIAentityNode*>::iterator sentenceConceptEntityNodesListTempNotUsedIter = sentenceConceptEntityNodesListTempNotUsed.begin(); sentenceConceptEntityNodesListTempNotUsedIter != sentenceConceptEntityNodesListTempNotUsed.end(); sentenceConceptEntityNodesListTempNotUsedIter++)
			{
				GIAentityNode* conceptEntityNodeTemp = *sentenceConceptEntityNodesListTempNotUsedIter;
				string entityNodeNameTemp = conceptEntityNodeTemp->entityName;
				sentenceConceptEntityNodesListTempNotUsedMap.insert(pair<string, GIAentityNode*>(entityNodeNameTemp, conceptEntityNodeTemp));
			}
			int numberReferenceSetsTemp = identifyReferenceSets(&sentenceConceptEntityNodesListTempNotUsedMap, NLPdependencyRelationsType);	//NB NLPdependencyRelationsType is no longer used here
			#ifdef GIA_FREE_MEMORY1
			delete sentenceConceptEntityNodesListTempNotUsedMap;
			#endif
		#else
			//only works with single sentence input (ie inputText.txt contains a single sentence)
			int numberReferenceSetsTemp = identifyReferenceSets(entityNodesActiveListConcepts, NLPdependencyRelationsType);
		#endif
	cout << "End for colours only...\n\n" << endl;
	#endif
#else
	vector<GIAentityNode*> sentenceConceptEntityNodesListTempNotUsed;
	#ifdef GIA_USE_CORPUS_DATABASE
	if(parseGIA2file)
	{
		if(currentSentenceInList->corpusLookupSuccessful)
		{
			convertSentenceSemanticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, entityNodesActiveListSentences, NLPfeatureParser);
		}
	}
	#endif
	if(!parseGIA2file || (parseGIA2file && !(currentSentenceInList->corpusLookupSuccessful)))
	{
		convertSentenceSyntacticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, entityNodesActiveListSentences, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
	}
#endif
}

#ifdef GIA_USE_ADVANCED_REFERENCING
void convertSentenceSyntacticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, vector<GIAentityNode*>* sentenceConceptEntityNodesList, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool linkPreestablishedReferencesGIA,  GIACoreference* firstGIACoreferenceInList)
#else
void convertSentenceSyntacticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, vector<GIAentityNode*>* sentenceConceptEntityNodesList, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
#endif
{
	GIArelation* currentRelationInList;

	#ifndef GIA_USE_ADVANCED_REFERENCING
	bool linkPreestablishedReferencesGIA = true;	//irrelevant
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "\n\n convertSentenceSyntacticRelationsIntoGIAnetworkNodes() currentSentenceInList->sentenceIndex = " << currentSentenceInList->sentenceIndex << endl;
	//cout << "\n\n\nconvertSentenceSyntacticRelationsIntoGIAnetworkNodes() linkPreestablishedReferencesGIA = " << linkPreestablishedReferencesGIA << endl;
	#endif
	
	#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	string corpusFileName = "";
	if(!linkPreestablishedReferencesGIA)
	{
		determineGIAconnectionistNetworkPOStypeNames(currentSentenceInList->firstFeatureInList, NLPfeatureParser);
		corpusFileName = createNewCorpusFileAndOpenItForWriting(currentSentenceInList->firstFeatureInList);
		string sentenceText = regenerateSentenceText(currentSentenceInList->firstFeatureInList, true, NLPfeatureParser);
		sentenceText = sentenceText + STRING_NEW_LINE;	//required to add new line at end of parsingWordsAndTags as per Stanford Parser specification (see parseStanfordParserFile)
		saveTextLineToCurrentCorpusFile(sentenceText);
		cout << sentenceText << endl;
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "\t" << currentSentenceInList->sentenceText << endl;
	#endif

	bool GIAentityNodeArrayFilled[MAX_NUMBER_OF_WORDS_PER_SENTENCE];		//NB could also use currentSentence->maxNumberOfWordsInSentence
	GIAentityNode* GIAconceptNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode* GIAentityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode* GIAfeatureTempEntityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];	//temporary array introduced 14 July 2012b, to handle grammatical information specific to instances of concept entities (not concept entities themselves) before those intances have been defined (in GIAtranslatorDefineSubstances.cpp)

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAconceptNodeArray[w] = NULL;		//added 10 May 2012
		GIAentityNodeArray[w] = NULL;		//added 10 May 2012
		GIAfeatureTempEntityNodeArray[w] = NULL;	//added  14 July 2012b
	}


	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass A;" << endl;
	cout << "pass 0; locate/add all entities [execution#1]" << endl;
	#endif
	locateAndAddAllFeatureTempEntities(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPdependencyRelationsType);

	GIAfeature* featureArrayTemp[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	generateTempFeatureArray(currentSentenceInList->firstFeatureInList, featureArrayTemp);	//regeneration required for Relex in case query variables detected

	#ifdef GIA_TRANSLATOR_DEBUG
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(featureArrayTemp[w] != NULL)
		{
			cout << "-1: " << featureArrayTemp[w]->lemma << ", w = " << w << endl;
		}
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1a; fillGrammaticalArrays" << endl;
	#endif
	#ifdef GIA_USE_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)		//updated 2d1a 21 Jan 2013, OLD: if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		fillGrammaticalArraysRelex(currentSentenceInList);
	}
	#endif
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	else if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)	//NB stanford dependency relations are required [ie not just Stanford POS tags] such that det/aux information can be extracted	//updated 2d1a 21 Jan 2013 - changed 'if' to 'else if'
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
	redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, featureArrayTemp);
	/*
		#endif
	}
	*/
	#endif
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(featureArrayTemp[w] != NULL)
		{
			cout << "0: " << featureArrayTemp[w]->lemma << ", w = " << w << endl;
		}
	}
	#endif

	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
		applyGIATranslatorGenericXMLfunctions("GIAtranslatorRedistributeStanfordRelations", currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, entityNodesActiveListConcepts, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
		#else
		redistributeStanfordRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
		#endif
	}
	#endif
	#ifdef GIA_SUPPORT_ALIASES_RELEX_COMPATIBILITY
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
		applyGIATranslatorGenericXMLfunctions("GIAtranslatorRedistributeRelexRelations", currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, entityNodesActiveListConcepts, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
		#else
		redistributeRelexRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
		#endif
	}
	#endif
	
	#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	invertOrDuplicateConditionsIfRequired(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, featureArrayTemp);
	#endif
	
	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	botSwitchFirstAndSecondPerson(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPdependencyRelationsType);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1d; locate/add all entities [execution#2]" << endl;
	#endif
	locateAndAddAllConceptEntities(currentSentenceInList, GIAentityNodeArrayFilled, GIAconceptNodeArray, entityNodesActiveListConcepts, sentenceConceptEntityNodesList, NLPdependencyRelationsType, GIAfeatureTempEntityNodeArray);

	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
	cout << "dependency relations: " << endl;
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			string relationType = currentRelationInList->relationType;
			GIAentityNode* relationGoverner = GIAconceptNodeArray[currentRelationInList->relationGovernorIndex];
			GIAentityNode* relationDependent = GIAconceptNodeArray[currentRelationInList->relationDependentIndex];

			cout << "relationType = " << currentRelationInList->relationType << endl;
			cout << "relationGoverner = " << relationGoverner->entityName << endl;
			cout << "relationDependent = " << relationDependent->entityName << endl;

		}
		currentRelationInList = currentRelationInList->next;
	}
	cout << "features (tags): " << endl;
	cout << "hello0" << endl;
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			cout << "w filled = " << w << endl;
			if(!(GIAconceptNodeArray[w]->disabled))
			{
				cout << "w !disabled = " << w << endl;
				GIAfeature* currentFeature = featureArrayTemp[w];

				cout << "GIAsentence Word Index = " << w;
				cout << " Word = " << currentFeature->word;
				cout << " Lemma = " << currentFeature->lemma;
				cout << " Is Date or Time = " << convertBoolToString(currentFeature->grammaticalIsDateOrTime);
				cout << " Tense = " << grammaticalTenseNameArray[currentFeature->grammaticalTense];
				for(int q=0; q<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES;q++)
				{
					cout << " Tense Modifier (" << grammaticalTenseModifierNameArray[q] << ") = " << convertBoolToString(currentFeature->grammaticalTenseModifierArray[q]);
				}
				cout << " Plurality = " << grammaticalNumberNameArray[currentFeature->grammaticalNumber];
				cout << " Is Definite = " << convertBoolToString(currentFeature->grammaticalIsDefinite);
				cout << " Is Indefinite Plural = " << convertBoolToString(currentFeature->grammaticalIsIndefinitePlural);
				cout << " Is Proper Noun = " << convertBoolToString(currentFeature->grammaticalIsProperNoun);
				cout << " Gender = " << grammaticalGenderNameArray[currentFeature->grammaticalGender];
				cout << " Is Pronoun = " << convertBoolToString(currentFeature->grammaticalIsPronoun);
				cout << " Wordtype = " << grammaticalWordTypeNameArray[currentFeature->grammaticalWordType];

				cout << " NER = " << featureNERtypeArray[currentFeature->NER];
				cout << " NormalizedNER = " << currentFeature->NormalizedNER;
				cout << " Timex = " << currentFeature->Timex;
				cout << " POS = " << currentFeature->stanfordPOS << endl;
			}
		}
	}
	cout << "hello1" << endl;
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
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//if(!linkPreestablishedReferencesGIA)	//criteria not used as same reference set tags may be required for dream mode or post processing (not just advanced referencing)
	//{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "pass 1e; fill Explicit Reference Same Set Tags" << endl;
		#endif
		//identify explicit same set linkages
		//eg "the guy that robbed the bank" in "the guy that robbed the bank is tall"
		fillExplicitReferenceSameSetTags(currentSentenceInList);
	//}
	#endif
#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 2; identify entity types [define entities as objects, subjects, and being possessive of substances];" << endl;
	cout << "hello2" << endl;
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
		cout << "\n\t\t\t GIA_USE_ADVANCED_REFERENCING_DEBUG (5linkAdvancedReferencesGIA)\n" << endl;
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

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 4; disable concept entities based on feature temp entity node array" << endl;
	#endif
	//transfer disabled substances across execution#1 [this is required since GIAtranslatorRedistributeStanfordRelations operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of concept entity nodes {whose values would have been automatically transferred their instances upon creation}...]
	disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(GIAentityNodeArrayFilled, GIAconceptNodeArray, GIAfeatureTempEntityNodeArray);


	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_USE_GENERIC_ENTITY_INTERPRETATION
 	//this is required as GIAtranslatorDefineSubstances.cpp now relies on entity grammar values rather than featureArrayTemp
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 5 [execution#2]; apply Grammatical Info To All Entities" << endl;
	#endif
	applyGrammaticalInfoToAllEntities(GIAentityNodeArrayFilled, GIAentityNodeArray, currentSentenceInList->firstFeatureInList);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass B:" << endl;
	#endif

	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	applyGIATranslatorGenericXMLfunctions("GIAtranslatorDefineSubstances", currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
	#else
	defineSubstances(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp, NLPdependencyRelationsType);
	#endif

	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//Stanford version is executed after all substances have been generated (including actions)... [Upgrade translator - do not associate feature/grammatical info with concept entities; just leave them in the feature array until the concept instances have been generated]
	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "1aOLD pass; collapseRedundantRelationAndMakeNegativeRelex (eg 'Space is saved by not having a bulky cart.'); _subj(not[5], by[4]), _subj(have[6], not[5]) [THIS PROBABLY SHOULD BE MOVED ABOVE linkPronounReferencesRelex...()]" << endl;
		#endif
		collapseRedundantRelationAndMakeNegativeRelex(currentSentenceInList, GIAentityNodeArray);
	}
	#endif
	#endif

	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_USE_GENERIC_ENTITY_INTERPRETATION
	//this function has been shifted, and applied to entity instances, not the concept entity array...
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1a [execution#2]; apply Grammatical Info To All Entities" << endl;
	#endif
 	applyGrammaticalInfoToAllEntities(GIAentityNodeArrayFilled, GIAentityNodeArray, currentSentenceInList->firstFeatureInList);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1b; identify comparison variable" << endl;
	#endif
	identifyComparisonVariableAlternateMethod(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 1c; disable entities based on feature temp entity node array" << endl;
	#endif
	//transfer disabled substances across execution#2 [this is required since GIAtranslatorRedistributeStanfordRelations operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of concept entity nodes {whose values would have been automatically transferred their instances upon creation}...]
	disableEntitiesBasedOnFeatureTempEntityNodeArray(GIAentityNodeArrayFilled, GIAentityNodeArray, GIAfeatureTempEntityNodeArray);

	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	applyGIATranslatorGenericXMLfunctions("GIAtranslatorLinkEntities", currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
	#else
	linkEntities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType, linkPreestablishedReferencesGIA);
	#endif

	#ifdef GIA_DYNAMICALLY_LINK_PRENOMINAL_MODIFIERS_OF_NOUNS
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "linkEntitiesDynamicPrenominalModifierOfNoun():" << endl;
	#endif
	linkEntitiesDynamicPrenominalModifierOfNoun(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, entityNodesActiveListSentences);
	#endif
	

	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//Stanford version has been shifted to after all substances have been generated (including actions)... [Upgrade translator - do not associate feature/grammatical info with concept entities; just leave them in the feature array until the concept instances have been generated]
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "4aOLD pass; collapseRedundantRelationAndMakeNegativeStanford (eg The chicken has not eaten a pie.: neg(eaten-5, not-4)" << endl;
		#endif
		collapseRedundantRelationAndMakeNegativeStanford(currentSentenceInList, GIAentityNodeArray);
	}
	#endif
	#endif

	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	applyGIATranslatorGenericXMLfunctions("GIAtranslatorApplyAdvancedFeatures", currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
	#endif

	applyAdvancedFeatures(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, NLPdependencyRelationsType, NLPfeatureParser);

	vector<GIAentityNode*>* entityNodesActiveListSentence = new vector<GIAentityNode*>;

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			entityNodesActiveListSentence->push_back(GIAentityNodeArray[w]);
			
			#ifdef GIA_USE_ADVANCED_REFERENCING
			#ifdef GIA_SET_ENTITY_ENTITY_AND_SENTENCE_INDICIES_NORMALLY
			if(!(GIAentityNodeArray[w]->wasReference))
			{
				#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
				cout << "\nw = " << w << endl;
				cout << "currentSentenceInList->sentenceIndex = " << currentSentenceInList->sentenceIndex << endl;
				cout << "GIAentityNodeArray[w]->entityIndexTemp = " << GIAentityNodeArray[w]->entityIndexTemp << endl;
				cout << "GIAentityNodeArray[w]->sentenceIndexTemp = " << GIAentityNodeArray[w]->sentenceIndexTemp << endl;
				#endif
				if(GIAentityNodeArray[w]->entityIndexTemp == GIA_ENTITY_INDEX_UNDEFINED)
				{
					//do not overwrite sentence index of source
					GIAentityNodeArray[w]->entityIndexTemp = w;
					#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
					cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes() warning: GIAentityNodeArray[" << w << "] entityIndexTemp undefined, this is an artificial entity" << endl;
					#endif
				}
				if(GIAentityNodeArray[w]->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
				{
					//do not overwrite sentence index of source
					GIAentityNodeArray[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
					#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
					cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes() warning: GIAentityNodeArray[" << w << "] sentenceIndexTemp undefined, this is an artificial entity" << endl;
					#endif
				}
				#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
				if(GIAentityNodeArray[w]->entityIndexTemp != w)
				{
					cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes() warning: GIAentityNodeArray[" << w << "] entityIndexTemp != " << w << endl;
				}
				if(GIAentityNodeArray[w]->sentenceIndexTemp != currentSentenceInList->sentenceIndex)
				{
					cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes() warning: GIAentityNodeArray[" << w << "] sentenceIndexTemp != " << currentSentenceInList->sentenceIndex << endl;
				}
				#endif
				//look for double references, and if found assume possible intrasentence referencing capabilities (assign unique entityIndexTemp values; ie their original entity indices); 
				if(featureArrayTemp[w] != NULL)
				{
					for(int w2=0; w2<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w2++)
					{
						if(featureArrayTemp[w2] != NULL)
						{
							#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
							cout << "featureArrayTemp[w2]->lemma =" << featureArrayTemp[w2]->lemma << ", w2 = " << w2 << endl;
							#endif
							if(featureArrayTemp[w2]->lemma == featureArrayTemp[w]->lemma)
							{
								if(w != w2)
								{
									#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
									cout << "w != w2" << endl;
									#endif
									GIAentityNodeArray[w]->entityIndexTemp = w;	//this is required for intrasentence advanced referencing (reactivated 2f19e 24-July-2014)
								}
							}
						}
					} 
				}
			}
			#else
			if(!(GIAentityNodeArray[w]->wasReference))
			{
				//do not overwrite sentence index of source
				GIAentityNodeArray[w]->entityIndexTemp = w;
				GIAentityNodeArray[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;	//LIMITATION: if !GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR, this will not assign sentence indicies for prepositions...
			}
			#endif
			#else
				GIAentityNodeArray[w]->entityIndexTemp = w;
				GIAentityNodeArray[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;	//LIMITATION: if !GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR, this will not assign sentence indicies for prepositions...			
				//cout << "GIAentityNodeArray[w]->sentenceIndexTemp = " << GIAentityNodeArray[w]->sentenceIndexTemp << endl;
			#endif
						
			//#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION	//concept sentenceIndex information is also required for GIAdraw.cpp
			//record sentenceIndex for concept entity nodes also (NB cannot use GIAconceptNodeArray here as it won't include concept entity nodes for prepositions)
			if(!(GIAentityNodeArray[w]->entityNodeDefiningThisInstance->empty()))
			{
				#ifdef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				GIAentityNode* instanceEntity = GIAentityNodeArray[w];
				for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntity->entityNodeDefiningThisInstance->begin(); connectionIter != instanceEntity->entityNodeDefiningThisInstance->end(); connectionIter++)
				{
					GIAentityNode* conceptNode = (*connectionIter)->entity;
				#else
					GIAentityNode* conceptNode = getPrimaryConceptNodeDefiningInstance(GIAentityNodeArray[w]);
				#endif
					#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
					conceptNode->mustSetIsSubstanceConceptBasedOnApposRelation = false; //added 29 Sept 2013
					conceptNode->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;	//added 29 Sept 2013
					conceptNode->isPronounReference = false;	//added 29 Sept 2013
					#endif

					if(conceptNode->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
					{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network
						//cout << "assigning: " <<  currentSentenceInList->sentenceIndex << endl;
						conceptNode->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
						//cout << "conceptNode->sentenceIndexTemp = " << conceptNode->sentenceIndexTemp << endl;
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
			//#endif

			#ifdef GIA_ADVANCED_REFERENCING_DEBUG
			//cout << GIAentityNodeArray[w]->entityName << ", w = " << w << endl;
			#endif
		}
	}
	#endif

	#ifdef GIA_USE_ADVANCED_REFERENCING
	if(linkPreestablishedReferencesGIA)
	{
	#endif
		entityNodesActiveListSentences->insert(pair<int, vector<GIAentityNode*>*>(currentSentenceInList->sentenceIndex, entityNodesActiveListSentence));
	#ifdef GIA_USE_ADVANCED_REFERENCING
	}
	else
	{
		delete entityNodesActiveListSentence;
	}
	#endif
	
	#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	if(!linkPreestablishedReferencesGIA)
	{
		GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(GIAentityNodeArray, currentSentenceInList, linkPreestablishedReferencesGIA, NLPdependencyRelationsType);
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "record sentence nodes as permanent if they are still enabled" << endl;
	#endif
	//recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(GIAentityNodeArrayFilled, GIAconceptNodeArray);		//this method is not sufficient, as some concept entity nodes (eg prepositions/conditions) are not contained within GIAconceptNodeArray
	recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(entityNodesActiveListConcepts);
	
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "\n\n recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled() currentSentenceInList->sentenceIndex = " << currentSentenceInList->sentenceIndex << endl;
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListIter2;
	for(conceptEntityNodesListIter2 = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter2 != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter2++)
	{
		GIAentityNode* entityNode = conceptEntityNodesListIter2->second;
		cout << "entityNode->disabled = " << entityNode->entityName << ", " << int(entityNode->disabled) << endl;
	}
	#endif

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	#ifdef GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_SENTENCE
	//restore critical variables: used for GIA translator reference paser only - cleared every time a new sentence is parsed
	unordered_map<string, GIAentityNode*> ::iterator conceptEntityNodesListIter;
	for(conceptEntityNodesListIter = entityNodesActiveListConcepts->begin(); conceptEntityNodesListIter != entityNodesActiveListConcepts->end(); conceptEntityNodesListIter++)
	{
		GIAentityNode* entityNode = conceptEntityNodesListIter->second;
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

	#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	if(!linkPreestablishedReferencesGIA)
	{
		string sentenceText = "";	//required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)
		saveTextLineToCurrentCorpusFile(sentenceText);
		closeCorpusFile();
		#ifdef GIA2_CONNECTIONIST_NETWORK
		if(!generateAllPermutationsFromSemanticRelationsFile(corpusFileName, NLPfeatureParser))
		{
			cout << "generateAllPermutationsFromSemanticRelationsFile() failed" << endl;
			exit(0);
		}
		#endif
	}
	#endif
}


void disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAentityNode* GIAfeatureTempEntityNodeArray[])
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
void disableEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAentityNode* GIAfeatureTempEntityNodeArray[])
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

#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
void dreamModeLinkSpecificConceptsAndActions()
{
	vector<GIAentityNode*>* entityNodesActiveListComplete = getTranslatorEntityNodesCompleteList();

	identifyReferenceSetsSpecificConceptsAndLinkWithSubstanceConcepts(entityNodesActiveListComplete);
	
	#ifdef GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES
	createAndLinkNonSpecificConceptsForAllEntities(entityNodesActiveListComplete);
	#endif
}

#ifdef GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES
void createAndLinkNonSpecificConceptsForAllEntities(vector<GIAentityNode*>* entityNodesActiveListComplete)
{
	for(vector<GIAentityNode*>::iterator entityIter = entityNodesActiveListComplete->begin(); entityIter != entityNodesActiveListComplete->end(); entityIter++)
	{	
		GIAentityNode* entity = (*entityIter);
		if((entity->isSubstance) && !(entity->isSubstanceConcept) && !(entity->isConcept))
		{
			GIAentityNode* conceptEntity = (entity->entityNodeDefiningThisInstance->back())->entity;

			GIAentityNode* nonSpecificConceptEntity = NULL;
			bool nonspecificSubstanceConceptAlreadyExists = false;
			for(vector<GIAentityNode*>::iterator entityIter2 = entityNodesActiveListComplete->begin(); entityIter2 != entityNodesActiveListComplete->end(); entityIter2++)
			{	
				GIAentityNode* entity2 = (*entityIter2);
				if((entity->isSubstance) && (entity2->isSubstanceConcept) && !(entity->isConcept))
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
							nonspecificSubstanceConceptAlreadyExists = true;
							nonSpecificConceptEntity = entity2;
						}
					}
				}
			}
			if(!nonspecificSubstanceConceptAlreadyExists)
			{
				nonSpecificConceptEntity = addSubstanceToSubstanceDefinition(conceptEntity);
				nonSpecificConceptEntity->isSubstanceConcept = true;
			}
			
			//found instance in network matching substance concept...
			#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
			bool sameReferenceSet = false;
			#else
			bool sameReferenceSet = IRRELVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
			#endif

			addDefinitionToEntity(entity, nonSpecificConceptEntity, sameReferenceSet);		
		}
	}
}
#endif

#endif

#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
void invertOrDuplicateConditionsIfRequired(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[])
{
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			bool sameReferenceSet = determineSameReferenceSetValue(DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS, currentRelationInList);
			if(!sameReferenceSet)	//added 2h1f
			{
				bool prepositionFound = false;
				string conditionName = convertPrepositionToRelex(&(currentRelationInList->relationType), &prepositionFound);
				if(prepositionFound)
				{
					if(!(currentRelationInList->inverseRelation))
					{
						bool inverseConditionRequired = false;
						bool twoWayConditionRequired = false;
						string inverseConditionName = "";
						detectIfInverseOrTwoWayConditionRequired(conditionName, &inverseConditionRequired, &twoWayConditionRequired, &inverseConditionName);

						#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS
						if(inverseConditionRequired)
						{
							#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
							cout << "invertOrDuplicateConditionsIfRequired(): inverseConditionRequired: conditionName = " << conditionName  << endl;
							#endif
							createNewInverseConditionEntity(currentRelationInList, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, inverseConditionName, featureArrayTemp);
						}
						#endif
						#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
						if(twoWayConditionRequired)
						{
							#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
							cout << "invertOrDuplicateConditionsIfRequired(): twoWayConditionRequired: conditionName = " << conditionName << endl;
							#endif
							#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
							GIArelation* lastRelationInList = currentSentenceInList->firstRelationInList;
							while(lastRelationInList->next != NULL)
							{	
								lastRelationInList = lastRelationInList->next;
							}
							createNewInverseConditionEntity(lastRelationInList, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, conditionName, featureArrayTemp);
							lastRelationInList->relationGovernor = currentRelationInList->relationDependent;
							lastRelationInList->relationDependent = currentRelationInList->relationGovernor;
							lastRelationInList->relationGovernorIndex = currentRelationInList->relationDependentIndex;
							lastRelationInList->relationDependentIndex = currentRelationInList->relationGovernorIndex;
							lastRelationInList->inverseRelationTwoWay = true;
							lastRelationInList->auxiliaryIndicatesDifferentReferenceSet = currentRelationInList->auxiliaryIndicatesDifferentReferenceSet;	//added 2h3a
							lastRelationInList->rcmodIndicatesSameReferenceSet = currentRelationInList->rcmodIndicatesSameReferenceSet;	//added 2h3a
	
							//cout << "inverseRelationTwoWay" << endl;
							lastRelationInList->next = new GIArelation();
							#endif
							currentRelationInList->relationTwoWay = true;
							//cout << "relationTwoWay" << endl;
						}
						#endif
					}
				}
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
}

void createNewInverseConditionEntity(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], string inverseConditionName, GIAfeature* featureArrayTemp[])
{
	int inverseConditionEntityIndex = currentSentenceInList->conditionEntityArtificialIndexCurrent;
	GIAentityNodeArrayFilled[inverseConditionEntityIndex] = true;
	GIAentityNode* inverseConditionEntity = new GIAentityNode();
	inverseConditionEntity->entityName = inverseConditionName; 
	inverseConditionEntity->wordOrig = inverseConditionName;	//is this necessary?
	currentRelationInList->relationType = string(STANFORD_PARSER_PREPOSITION_PREPEND) + inverseConditionName;
	currentRelationInList->relationTypeIndex = inverseConditionEntityIndex;
	currentRelationInList->inverseRelation = true;	//not required
	GIAentityNodeArray[inverseConditionEntityIndex] = inverseConditionEntity;
	featureArrayTemp[inverseConditionEntityIndex] = new GIAfeature(); 
	featureArrayTemp[inverseConditionEntityIndex]->word = inverseConditionName;
	featureArrayTemp[inverseConditionEntityIndex]->lemma = inverseConditionName;	//is this necessary?
	featureArrayTemp[inverseConditionEntityIndex]->entityIndex = inverseConditionEntityIndex;
	currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;
}					
#endif




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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2n1f 12-September-2016
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslator.h"
#include "GIAtranslatorDefineGrammar.h"
/*
#ifdef GIA_TEMPORARY_COMPILATION_GCC_FLAG_E_PREPROCESS_TRANSLATOR_XML_FILES
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorRedistributeRelationsRelex.xml"
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorRedistributeRelationsStanford.xml"
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorDefineSubstances.xml"
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorLinkEntities.xml"
#OCCLUDEDFORCSinclude "xmlConversion/GIAtranslatorApplyAdvancedFeatures.xml"
#else
*/
#include "GIAtranslatorRedistributeRelations.h"
#include "GIAtranslatorRedistributeRelationsRelex.h"
#include "GIAtranslatorRedistributeRelationsStanford.h"
#include "GIAtranslatorDefineReferencing.h"
#include "GIAtranslatorDefineSubstances.h"
#include "GIAtranslatorLinkEntities.h"
#include "GIAtranslatorLinkEntitiesDynamic.h"
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
#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
#include "GIAsemanticParserOperations.h"
#include "GIAsemanticParserTranslator.h"
#include "GIAsemanticParserDatabase.h"
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

/*
#ifdef USE_CE
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentencesCE(GIAparagraph* firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int* maxNumberSentences, bool parseGIA2file, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, bool useCodeextensionsHeirachy)
{
}
#endif
*/
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int* maxNumberSentences, bool parseGIA2file)
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
	if(!createSemanticNetworkBasedUponDependencyParsedSentencesCE(firstParagraphInList, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, timeConditionNodesActiveList, isQuery, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file, firstCodeextensionInHeirachy, codeextensionsList, useCodeextensionsHeirachy))
	#else
	if(!createSemanticNetworkBasedUponDependencyParsedSentences(firstParagraphInList, entityNodesActiveListComplete, entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, timeConditionNodesActiveList, isQuery, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file))
	#endif
	{
		result = false;
	}

	setCurrentDirectory(tempFolder);

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

/*
#ifdef USE_CE
bool createSemanticNetworkBasedUponDependencyParsedSentencesCE(GIAparagraph* firstParagraphInList, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, bool useCodeextensionsHeirachy)
{
}
#endif
*/
bool createSemanticNetworkBasedUponDependencyParsedSentences(GIAparagraph* firstParagraphInList, vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
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
	
	initialiseGIATranslatorForTexualContext();

	#ifdef USE_CE
	if(useCodeextensionsHeirachy)
	{
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(firstParagraphInList->next->next != NULL)
		{
			cout << "convertParagraphSentenceRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy{}: error - CE only supports a single paragraph of text, one codeextension per line" << endl;
			exit(0);
		}
		#endif
		convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(entityNodesActiveListNetworkIndexes, timeConditionNodesActiveList, firstSentenceInList, firstCodeextensionInHeirachy, codeextensionsList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);
	}
	else
	{
	#endif
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
			convertParagraphSentenceRelationsIntoGIAnetworkNodes(entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, timeConditionNodesActiveList, firstParagraphInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);
		}
		else
		{
		#endif
			convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		}
		#endif
	#ifdef USE_CE
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "record networkIndex nodes as disabled if they are not permanent (used for printing/xml write purposes)" << endl;
	#endif
	recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(entityNodesActiveListNetworkIndexes);

	#ifdef GIA_USE_DATABASE
	if(isQuery)
	{
		setUseDatabase(useDatabaseOriginal);
	}
	#endif

	return result;
}


#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
void convertParagraphSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAparagraph* firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
{
	GIAparagraph* currentParagraphInList = firstParagraphInList;
	while(currentParagraphInList->next != NULL)
	{
		GIAsentence* firstSentenceInList = currentParagraphInList->firstSentenceInList;

		convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);

		#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
		#ifdef GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_PARAGRAPH
		//restore critical variables: used for GIA translator reference paser only - cleared every time a new paragraph is parsed
		for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIter = entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityNodesListIter != entityNodesActiveListNetworkIndexes->end(); networkIndexEntityNodesListIter++)
		{
			GIAentityNode* entityNode = networkIndexEntityNodesListIter->second;
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
void convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, CECodeextension* firstCodeextensionInHeirachy, vector<CECodeextension*>* codeextensionsList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	//link GIAsentences with each generated CEcodeextension
	GIAsentence* currentSentenceInList = firstSentenceInList;
	for(vector<CECodeextension*>::iterator codeextensionIter = codeextensionsList->begin(); codeextensionIter != codeextensionsList->end(); codeextensionIter++)
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
		entityNodesActiveListNetworkIndexes = new unordered_map<string, GIAentityNode*>;
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
		vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList = &(currentCodeextensionInHeirachy->relevantNetworkIndexEntityNodeList);
		setAllCodeextensionEntitiesInHeirachyToUndeclaredInThisContext(firstCodeextensionInHeirachy);
		setParentCodeextensionEntitiesAsAlreadyDeclaredInThisContext(currentCodeextensionInHeirachy);
		convertSentenceSyntacticRelationsIntoGIAnetworkNodesCEOLD(entityNodesActiveListNetworkIndexes, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, sentenceNetworkIndexEntityNodesList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		#else
		//convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		convertCEsentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, timeConditionNodesActiveList, firstSentenceInArtificialList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
		#endif
	#ifdef GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS
	}
	#endif
}

void convertCEsentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
{
	GIAsentence* currentSentenceInList = firstSentenceInList;
	do
	{
		convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, false);

		currentSentenceInList = currentSentenceInList->next;
	}
	while(currentSentenceInList != NULL);
}
#endif

void convertSentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
{
	GIAsentence* currentSentenceInList = firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);

		currentSentenceInList = currentSentenceInList->next;
	}

	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	dreamModeLinkSpecificConceptsAndActions();
	#endif
}



void convertSentenceRelationsIntoGIAnetworkNodesWrapper(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
{
	#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
	setCurrentSentenceIndex(currentSentenceInList->sentenceIndex);
	#endif

	bool enableAdvancedReferencing = false;
	#ifdef GIA_USE_ADVANCED_REFERENCING
	enableAdvancedReferencing = true;
	#ifdef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
	if(checkIfSentenceIsMathTextParsablePhrase(currentSentenceInList))
	{
		enableAdvancedReferencing = false;
	}
	#endif
	if(enableAdvancedReferencing)
	{
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
		vector<GIAentityNode*>* entityNodesActiveListCompleteTemp = new vector<GIAentityNode*>;
		unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndexDBactiveTemp;
		if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
		{
			entityNodesActiveListCompleteFastIndexDBactiveTemp = new unordered_map<string, GIAentityNode*>;
		}
		setTranslatorEntityNodesCompleteList(entityNodesActiveListCompleteTemp);
		if(getUseDatabase() != GIA_USE_DATABASE_FALSE)
		{
			setDBentityNodesActiveListCompleteFastIndexDBactive(entityNodesActiveListCompleteFastIndexDBactiveTemp);
		}
		long currentEntityNodeIDInCompleteListOriginal = *(getCurrentEntityNodeIDinCompleteList());
		long currentEntityNodeIDInNetworkIndexEntityNodesListOriginal = *(getCurrentEntityNodeIDinNetworkIndexEntityNodesList());
		long currentEntityNodeIDInCompleteListTemp = 0;
		long currentEntityNodeIDInNetworkIndexEntityNodesListTemp = 0;
		*(getCurrentEntityNodeIDinCompleteList()) = currentEntityNodeIDInCompleteListTemp;
		*(getCurrentEntityNodeIDinNetworkIndexEntityNodesList()) = currentEntityNodeIDInNetworkIndexEntityNodesListTemp;
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
		unordered_map<string, GIAentityNode*>* sentenceNetworkIndexEntityNodesList = new unordered_map<string, GIAentityNode*>;
		unordered_map<long, GIAtimeConditionNode*>* sentenceTimeConditionNodesList = new unordered_map<long, GIAtimeConditionNode*>;
		long* currentEntityNodeIDInSentenceCompleteList = getCurrentEntityNodeIDinSentenceCompleteList();
		long* currentEntityNodeIDInSentenceNetworkIndexEntityNodesList = getCurrentEntityNodeIDinSentenceNetworkIndexEntityNodesList();
		GIAsentence* currentSentenceInListTemp = new GIAsentence();
		copySentences(currentSentenceInList, currentSentenceInListTemp);
		*currentEntityNodeIDInSentenceCompleteList = 0;
		*currentEntityNodeIDInSentenceNetworkIndexEntityNodesList = 0;
		vector<GIAentityNode*> sentenceNetworkIndexEntityNodesListTempNotUsed1;
		map<int, vector<GIAentityNode*>*> entityNodesActiveListSentencesTempNotUsed;
		#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
		if(parseGIA2file)
		{
			if(currentSentenceInList->semanticParserSuccessful)
			{
				#ifdef GIA_DEBUG
				//cout << "semanticParserSuccessful" << endl;
				#endif
				convertSentenceSemanticRelationsIntoGIAnetworkNodes(sentenceNetworkIndexEntityNodesList, sentenceTimeConditionNodesList, firstSentenceInList, currentSentenceInListTemp, &sentenceNetworkIndexEntityNodesListTempNotUsed1, &entityNodesActiveListSentencesTempNotUsed, NLPfeatureParser, false, NULL);
			}
		}
		#endif
		if(!parseGIA2file || (parseGIA2file && !(currentSentenceInList->semanticParserSuccessful)))
		{
			#ifdef GIA_DEBUG
			//cout << "!semanticParserSuccessful" << endl;
			#endif
			convertSentenceSyntacticRelationsIntoGIAnetworkNodes(sentenceNetworkIndexEntityNodesList, sentenceTimeConditionNodesList, firstSentenceInList, currentSentenceInListTemp, &sentenceNetworkIndexEntityNodesListTempNotUsed1, &entityNodesActiveListSentencesTempNotUsed, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, false, NULL);
		}

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		cout << "\n\t\t\t GIA_USE_ADVANCED_REFERENCING_DEBUG (2identifyReferenceSets)\n" << endl;
		#endif

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		/*
		for(vector<GIAentityNode*>::iterator sentenceNetworkIndexEntityNodesListTempNotUsed1Iter = sentenceNetworkIndexEntityNodesListTempNotUsed1.begin(); sentenceNetworkIndexEntityNodesListTempNotUsed1Iter != sentenceNetworkIndexEntityNodesListTempNotUsed1.end(); sentenceNetworkIndexEntityNodesListTempNotUsed1Iter++)
		{
			GIAentityNode* entityNode = *sentenceNetworkIndexEntityNodesListTempNotUsed1Iter;
			cout << "DEBUG1: entityNode->entityName = " << entityNode->entityName << endl;
		}
		for(unordered_map<string, GIAentityNode*>::iterator sentenceNetworkIndexEntityNodesListIter = sentenceNetworkIndexEntityNodesList->begin(); sentenceNetworkIndexEntityNodesListIter != sentenceNetworkIndexEntityNodesList->end(); sentenceNetworkIndexEntityNodesListIter++)
		{
			GIAentityNode* entityNode = sentenceNetworkIndexEntityNodesListIter->second;
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
		int numberReferenceSets = identifyReferenceSets(sentenceNetworkIndexEntityNodesList, NLPdependencyRelationsType, &referenceSetDefiniteEntityList);	//NB NLPdependencyRelationsType is no longer used here

		#ifdef GIA_USE_DATABASE
		setUseDatabase(useDatabaseOriginal);
		#endif

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		cout << "\n\t\t\t GIA_USE_ADVANCED_REFERENCING_DEBUG (3createGIACoreferenceInListBasedUponIdentifiedReferenceSets)\n" << endl;
		#endif

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
		cout << "createGIAcoreferenceInListBasedUponIdentifiedReferenceSets:" << endl;
		#endif
		createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(sentenceNetworkIndexEntityNodesList, entityNodesActiveListNetworkIndexes, firstGIACoreferenceInList, &referenceSetDefiniteEntityList, currentSentenceInList);

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
		*(getCurrentEntityNodeIDinCompleteList()) = currentEntityNodeIDInCompleteListOriginal;
		*(getCurrentEntityNodeIDinNetworkIndexEntityNodesList()) = currentEntityNodeIDInNetworkIndexEntityNodesListOriginal;
		#endif

		vector<GIAentityNode*> sentenceNetworkIndexEntityNodesListTempNotUsed;
		#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
		if(parseGIA2file)
		{
			if(currentSentenceInList->semanticParserSuccessful)
			{
				convertSentenceSemanticRelationsIntoGIAnetworkNodes(entityNodesActiveListNetworkIndexes, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceNetworkIndexEntityNodesListTempNotUsed, entityNodesActiveListSentences, NLPfeatureParser, true, firstGIACoreferenceInList);
			}
		}
		#endif
		if(!parseGIA2file || (parseGIA2file && !(currentSentenceInList->semanticParserSuccessful)))
		{
			convertSentenceSyntacticRelationsIntoGIAnetworkNodes(entityNodesActiveListNetworkIndexes, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceNetworkIndexEntityNodesListTempNotUsed, entityNodesActiveListSentences, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, true, firstGIACoreferenceInList);
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
		#endif
		delete sentenceNetworkIndexEntityNodesList;		//sentenceNetworkIndexEntityNodesList->clear();
		delete sentenceTimeConditionNodesList;		//sentenceTimeConditionNodesList->clear();
		#endif

		#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS
		cout << "\n\nfor colours only..." << endl;
			#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS_WORK_WITH_MULTIPLE_SENTENCES
				unordered_map<string, GIAentityNode*> sentenceNetworkIndexEntityNodesListTempNotUsedMap;
				for(vector<GIAentityNode*>::iterator sentenceNetworkIndexEntityNodesListTempNotUsedIter = sentenceNetworkIndexEntityNodesListTempNotUsed.begin(); sentenceNetworkIndexEntityNodesListTempNotUsedIter != sentenceNetworkIndexEntityNodesListTempNotUsed.end(); sentenceNetworkIndexEntityNodesListTempNotUsedIter++)
				{
					GIAentityNode* networkIndexEntityNodeTemp = *sentenceNetworkIndexEntityNodesListTempNotUsedIter;
					string entityNodeNameTemp = networkIndexEntityNodeTemp->entityName;
					sentenceNetworkIndexEntityNodesListTempNotUsedMap.insert(pair<string, GIAentityNode*>(entityNodeNameTemp, networkIndexEntityNodeTemp));
				}
				int numberReferenceSetsTemp = identifyReferenceSets(&sentenceNetworkIndexEntityNodesListTempNotUsedMap, NLPdependencyRelationsType);	//NB NLPdependencyRelationsType is no longer used here
				#ifdef GIA_FREE_MEMORY1
				delete sentenceNetworkIndexEntityNodesListTempNotUsedMap;
				#endif
			#else
				//only works with single sentence input (ie inputText.txt contains a single sentence)
				int numberReferenceSetsTemp = identifyReferenceSets(entityNodesActiveListNetworkIndexes, NLPdependencyRelationsType);
			#endif
		cout << "End for colours only...\n\n" << endl;
		#endif
	}
	else
	{
	#endif
		vector<GIAentityNode*> sentenceNetworkIndexEntityNodesListTempNotUsed;
		#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
		if(parseGIA2file)
		{
			if(currentSentenceInList->semanticParserSuccessful)
			{
				convertSentenceSemanticRelationsIntoGIAnetworkNodes(entityNodesActiveListNetworkIndexes, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceNetworkIndexEntityNodesListTempNotUsed, entityNodesActiveListSentences, NLPfeatureParser, false, NULL);
			}
		}
		#endif
		if(!parseGIA2file || (parseGIA2file && !(currentSentenceInList->semanticParserSuccessful)))
		{
			convertSentenceSyntacticRelationsIntoGIAnetworkNodes(entityNodesActiveListNetworkIndexes, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceNetworkIndexEntityNodesListTempNotUsed, entityNodesActiveListSentences, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, false, NULL);
		}
	#ifdef GIA_USE_ADVANCED_REFERENCING
	}
	#endif
}

void convertSentenceSyntacticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, GIAsentence* firstSentenceInList, GIAsentence* currentSentenceInList, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool linkPreestablishedReferencesGIA, GIACoreference* firstGIACoreferenceInList)
{
	GIArelation* currentRelationInList;

	#ifndef GIA_USE_ADVANCED_REFERENCING
	bool linkPreestablishedReferencesGIA = true;	//irrelevant
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "\n\n convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} currentSentenceInList->sentenceIndex = " << currentSentenceInList->sentenceIndex << endl;
	//cout << "\n\n\nconvertSentenceSyntacticRelationsIntoGIAnetworkNodes{} linkPreestablishedReferencesGIA = " << linkPreestablishedReferencesGIA << endl;
	#endif

	#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	string corpusFileName = "";
	if(!linkPreestablishedReferencesGIA)
	{
		//determineGIAconnectionistNetworkPOStypeNames(currentSentenceInList->firstFeatureInList, NLPfeatureParser);
		prepareSemanticParserDatabaseForWriting();
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	//cout << "\t" << currentSentenceInList->sentenceText << endl;
	#endif

	bool GIAentityNodeArrayFilled[MAX_NUMBER_OF_WORDS_PER_SENTENCE];		//NB could also use currentSentence->maxNumberOfWordsInSentence
	GIAentityNode* GIAnetworkIndexNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode* GIAentityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
	GIAentityNode* GIAfeatureTempEntityNodeArray[MAX_NUMBER_OF_WORDS_PER_SENTENCE];	//temporary array introduced 14 July 2012b, to handle grammatical information specific to instances of networkIndex entities (not networkIndex entities themselves) before those intances have been defined (in GIAtranslatorDefineSubstances.cpp)

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAnetworkIndexNodeArray[w] = NULL;		//added 10 May 2012
		GIAentityNodeArray[w] = NULL;		//added 10 May 2012
		GIAfeatureTempEntityNodeArray[w] = NULL;	//added  14 July 2012b
	}


	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section A;" << endl;
	cout << "section A0; locateAndAddAllFeatureTempEntities [execution#1]" << endl;
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
	cout << "section A1a; fillGrammaticalArrays" << endl;
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
	cout << "section A1b; applyGrammaticalInfoToAllEntities [execution#1]" << endl;
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
	cout <<"section A1c; redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs" << endl;
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

	#ifdef GIA_TRANSLATOR_DEBUG
	cout <<"section A1d; redistributeStanfordRelations/redistributeRelexRelations" << endl;
	#endif
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
		applyGIATranslatorGenericXMLfunctions("GIAtranslatorRedistributeRelationsStanford", currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, entityNodesActiveListNetworkIndexes, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
		#else
		redistributeStanfordRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
		#endif
	}
	#endif
	#ifdef GIA_SUPPORT_ALIASES_RELEX_COMPATIBILITY
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
		applyGIATranslatorGenericXMLfunctions("GIAtranslatorRedistributeRelationsRelex", currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, entityNodesActiveListNetworkIndexes, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
		#else
		redistributeRelexRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
		#endif
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section A1e; invertOrDuplicateConditionsIfRequired" << endl;
	#endif
	#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	invertOrDuplicateConditionsIfRequired(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, featureArrayTemp);
	#endif

	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	botSwitchFirstAndSecondPerson(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section A1f; locateAndAddAllNetworkIndexEntities [execution#2]" << endl;
	#endif
	locateAndAddAllNetworkIndexEntities(currentSentenceInList, GIAentityNodeArrayFilled, GIAnetworkIndexNodeArray, entityNodesActiveListNetworkIndexes, sentenceNetworkIndexEntityNodesList, NLPdependencyRelationsType, GIAfeatureTempEntityNodeArray);

	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	createParentsOfSubclassEntities(GIAentityNodeArrayFilled, GIAnetworkIndexNodeArray, entityNodesActiveListNetworkIndexes, sentenceNetworkIndexEntityNodesList);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			cout << "relationType = " << currentRelationInList->relationType << endl;
			cout << "relationGoverner = " << currentRelationInList->relationGovernor << endl;
			cout << "relationDependent = " << currentRelationInList->relationDependent << endl;
			cout << "relationGovernerIndex = " << currentRelationInList->relationGovernorIndex << endl;
			cout << "relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
			if(GIAentityNodeArrayFilled[currentRelationInList->relationGovernorIndex])
			{
				GIAentityNode* relationGoverner = GIAnetworkIndexNodeArray[currentRelationInList->relationGovernorIndex];
				cout << "GIAnetworkIndexNodeArray relationGoverner->entityName = " << relationGoverner->entityName << endl;
			}
			if(GIAentityNodeArrayFilled[currentRelationInList->relationDependentIndex])
			{
				GIAentityNode* relationDependent = GIAnetworkIndexNodeArray[currentRelationInList->relationDependentIndex];
				cout << "GIAnetworkIndexNodeArray relationDependent->entityName = " << relationDependent->entityName << endl;
			}
		}
		currentRelationInList = currentRelationInList->next;
	}
	#endif
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
	cout << "dependency relations: " << endl;
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			string relationType = currentRelationInList->relationType;
			GIAentityNode* relationGoverner = GIAnetworkIndexNodeArray[currentRelationInList->relationGovernorIndex];
			GIAentityNode* relationDependent = GIAnetworkIndexNodeArray[currentRelationInList->relationDependentIndex];

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
			cout << "w filled = " << w << endl;
			if(!(GIAnetworkIndexNodeArray[w]->disabled))
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
	#endif

	if(NLPassumePreCollapsedStanfordRelations)
	{//untested
		if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
		{
			NLPdependencyRelationsType = GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD;
			//assume dependency relations are in Stanford format from here on in (ie; they do not require Relex special case ammendements/reconciliation + preposition types are in Stanford format)
		}
	}

#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//if(!linkPreestablishedReferencesGIA)	//criteria not used as same reference set tags may be required for dream mode or post processing (not just advanced referencing)
	//{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "section A1g; fill Explicit Reference Same Set Tags" << endl;
		#endif
		//identify explicit same set linkages
		//eg "the guy that robbed the bank" in "the guy that robbed the bank is tall"
		fillExplicitReferenceSameSetTags(currentSentenceInList);
	//}
	#endif
#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section A2; identifyEntityTypes" << endl;
	#endif
	identifyEntityTypes(currentSentenceInList, GIAfeatureTempEntityNodeArray, NLPdependencyRelationsType);

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		GIAentityNodeArray[w] = GIAnetworkIndexNodeArray[w];		//set default values of GIAentityNodeArray
	}

#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	#ifdef GIA_USE_ADVANCED_REFERENCING
	if(linkPreestablishedReferencesGIA)
	{
		#ifdef GIA_ADVANCED_REFERENCING_DEBUG
		cout << "\n\t\t\t GIA_USE_ADVANCED_REFERENCING_DEBUG (5linkAdvancedReferencesGIA)\n" << endl;
		#endif
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "section A3ii; linkAdvancedReferencesGIA (eg the red car is fast. Mike drove the red car.)" << endl;
		#endif
		linkAdvancedReferencesGIA(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, firstGIACoreferenceInList, featureArrayTemp, GIAfeatureTempEntityNodeArray, GIAnetworkIndexNodeArray);
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
			cout << "section A3i; linkPronounReferencesRelex (eg his/her with joe/emily)" << endl;
			#endif
			linkPronounReferencesRelex(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, featureArrayTemp);
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		}
		#ifdef GIA_USE_STANFORD_CORENLP
		else if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			#ifdef GIA_TRANSLATOR_DEBUG
			cout << "pass A3i; linkPronounAndTextualContextReferencesStanfordCoreNLPP (eg his/her with joe/emily)" << endl;
			#endif
			linkPronounAndTextualContextReferencesStanfordCoreNLP(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, firstSentenceInList->firstCoreferenceInList, featureArrayTemp);
		}
		#endif
		#endif
		#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	}
	#endif
#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section A4; disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray" << endl;
	#endif
	//transfer disabled substances across execution#1 [this is required since GIAtranslatorRedistributeRelationsStanford operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of networkIndex entity nodes {whose values would have been automatically transferred to their instances upon creation}...]
	disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(GIAentityNodeArrayFilled, GIAnetworkIndexNodeArray, GIAfeatureTempEntityNodeArray);


	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_USE_GENERIC_ENTITY_INTERPRETATION
 	//this is required as GIAtranslatorDefineSubstances.cpp now relies on entity grammar values rather than featureArrayTemp
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section A5; applyGrammaticalInfoToAllEntities [execution#2]" << endl;
	#endif
	applyGrammaticalInfoToAllEntities(GIAentityNodeArrayFilled, GIAentityNodeArray, currentSentenceInList->firstFeatureInList);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B:" << endl;
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B0; defineSubstances" << endl;
	#endif
	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	applyGIATranslatorGenericXMLfunctions("GIAtranslatorDefineSubstances", currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
	#else
	defineSubstances(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp, NLPdependencyRelationsType);
	#endif
	
	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//Stanford version is executed after all substances have been generated (including actions)... [Upgrade translator - do not associate feature/grammatical info with networkIndex entities; just leave them in the feature array until the networkIndex instances have been generated]
	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "1aOLD section; collapseRedundantRelationAndMakeNegativeRelex (eg 'Space is saved by not having a bulky cart.'); _subj(not[5], by[4]), _subj(have[6], not[5]) [THIS PROBABLY SHOULD BE MOVED ABOVE linkPronounReferencesRelex...()]" << endl;
		#endif
		collapseRedundantRelationAndMakeNegativeRelex(currentSentenceInList, GIAentityNodeArray);
	}
	#endif
	#endif

	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_USE_GENERIC_ENTITY_INTERPRETATION
	//this function has been shifted, and applied to entity instances, not the networkIndex entity array...
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B1a; applyGrammaticalInfoToAllEntities [execution#3]" << endl;
	#endif
 	applyGrammaticalInfoToAllEntities(GIAentityNodeArrayFilled, GIAentityNodeArray, currentSentenceInList->firstFeatureInList);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B1a; identifyComparisonVariableAlternateMethod" << endl;
	#endif
	identifyComparisonVariableAlternateMethod(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NLPfeatureParser);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B1b; disable entities based on feature temp entity node array" << endl;
	#endif
	//transfer disabled substances across execution#2 [this is required since GIAtranslatorRedistributeRelationsStanford operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of networkIndex entity nodes {whose values would have been automatically transferred to their instances upon creation}...]
	disableEntitiesBasedOnFeatureTempEntityNodeArray(GIAentityNodeArrayFilled, GIAentityNodeArray, GIAfeatureTempEntityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B2; linkEntities" << endl;
	#endif
	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	applyGIATranslatorGenericXMLfunctions("GIAtranslatorLinkEntities", currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
	#else
	linkEntities(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, NLPdependencyRelationsType, linkPreestablishedReferencesGIA);
	#endif

	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	createAdditionalSubclassEntities(GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, sentenceNetworkIndexEntityNodesList, currentSentenceInList->sentenceIndex);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
	detectUserDeclaredSubclassEntities(GIAentityNodeArrayFilled, GIAentityNodeArray);
	#endif
	#endif
	
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B3; linkEntitiesDynamic{}:" << endl;
	#endif
	linkEntitiesDynamic(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, entityNodesActiveListSentences);

	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//Stanford version has been shifted to after all substances have been generated (including actions)... [Upgrade translator - do not associate feature/grammatical info with networkIndex entities; just leave them in the feature array until the networkIndex instances have been generated]
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		cout << "B4aOLD pass; collapseRedundantRelationAndMakeNegativeStanford (eg The chicken has not eaten a pie.: neg(eaten-5, not-4)" << endl;
		#endif
		collapseRedundantRelationAndMakeNegativeStanford(currentSentenceInList, GIAentityNodeArray);
	}
	#endif
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "section B4; applyAdvancedFeatures{}:" << endl;
	#endif
	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	applyGIATranslatorGenericXMLfunctions("GIAtranslatorApplyAdvancedFeatures", currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
	#endif
	applyAdvancedFeatures(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListNetworkIndexes, NLPdependencyRelationsType, NLPfeatureParser);

	vector<GIAentityNode*>* entityNodesActiveListSentence = new vector<GIAentityNode*>;

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			entityNodesActiveListSentence->push_back(GIAentityNodeArray[w]);

			#ifdef GIA_USE_ADVANCED_REFERENCING

			#ifdef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
			if(checkIfSentenceIsMathTextParsablePhrase(currentSentenceInList))
			{
				#ifndef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
				if(GIAentityNodeArray[w]->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)
				{
				#endif
					if(!(GIAentityNodeArray[w]->wasReference))	//redundant?
					{
						GIAentityNodeArray[w]->NLCmathTextParsablePhraseEntity = true;
					}
				#ifndef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS
				}
				#endif
			}
			#endif

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
					cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "] entityIndexTemp undefined, this is an artificial entity" << endl;
					#endif
				}
				if(GIAentityNodeArray[w]->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
				{
					//do not overwrite sentence index of source
					GIAentityNodeArray[w]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
					#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
					cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "] sentenceIndexTemp undefined, this is an artificial entity" << endl;
					#endif
				}
				//#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
				#ifdef GIA_ADVANCED_REFERENCING_DEBUG_INTRASENTENCE_EXTRA
				if(GIAentityNodeArray[w]->entityIndexTemp != w)
				{
					cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "] entityIndexTemp != " << w << endl;
				}
				if(GIAentityNodeArray[w]->sentenceIndexTemp != currentSentenceInList->sentenceIndex)
				{
					cout << "convertSentenceSyntacticRelationsIntoGIAnetworkNodes{} warning: GIAentityNodeArray[" << w << "] sentenceIndexTemp != " << currentSentenceInList->sentenceIndex << endl;
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
				//#endif
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
				#ifdef GIA_DEBUG
				//cout << "GIAentityNodeArray[w]->sentenceIndexTemp = " << GIAentityNodeArray[w]->sentenceIndexTemp << endl;
				#endif
			#endif

			//#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION	//networkIndex sentenceIndex information is also required for GIAdraw.cpp
			//record sentenceIndex for networkIndex entity nodes also (NB cannot use GIAnetworkIndexNodeArray here as it won't include networkIndex entity nodes for prepositions)
			if(!(GIAentityNodeArray[w]->entityNodeDefiningThisInstance->empty()))
			{
				#ifdef GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE
				GIAentityNode* instanceEntity = GIAentityNodeArray[w];
				for(vector<GIAentityConnection*>::iterator connectionIter = instanceEntity->entityNodeDefiningThisInstance->begin(); connectionIter != instanceEntity->entityNodeDefiningThisInstance->end(); connectionIter++)
				{
					GIAentityNode* networkIndexNode = (*connectionIter)->entity;
				#else
					GIAentityNode* networkIndexNode = getPrimaryNetworkIndexNodeDefiningInstance(GIAentityNodeArray[w]);
				#endif
					#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
					networkIndexNode->mustSetIsConceptBasedOnApposRelation = false; //added 29 Sept 2013
					networkIndexNode->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;	//added 29 Sept 2013
					networkIndexNode->isPronounReference = false;	//added 29 Sept 2013
					#endif

					if(networkIndexNode->sentenceIndexTemp == GIA_SENTENCE_INDEX_UNDEFINED)
					{//do not overwrite sentenceIndex, as it needs to be drawn with first instance in network
						networkIndexNode->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
						#ifdef GIA_DEBUG
						//cout << "networkIndexNode->sentenceIndexTemp = " << networkIndexNode->sentenceIndexTemp << endl;
						#endif
					}
					/*No problem detected here:
					if(networkIndexNode->sentenceIndexTemp == 0)
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

	entityNodesActiveListSentences->insert(pair<int, vector<GIAentityNode*>*>(currentSentenceInList->sentenceIndex, entityNodesActiveListSentence));

	#ifdef GIA2_NON_HEURISTIC_IMPLEMENTATION_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	if(!linkPreestablishedReferencesGIA)
	{
		GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(GIAentityNodeArray, currentSentenceInList, linkPreestablishedReferencesGIA, NLPdependencyRelationsType);
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "record sentence nodes as permanent if they are still enabled" << endl;
	#endif
	//recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(GIAentityNodeArrayFilled, GIAnetworkIndexNodeArray);		//this method is not sufficient, as some networkIndex entity nodes (eg prepositions/conditions) are not contained within GIAnetworkIndexNodeArray
	recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(entityNodesActiveListNetworkIndexes);

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "\n\n recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled() currentSentenceInList->sentenceIndex = " << currentSentenceInList->sentenceIndex << endl;
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIter2 = entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityNodesListIter2 != entityNodesActiveListNetworkIndexes->end(); networkIndexEntityNodesListIter2++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListIter2->second;
		cout << "entityNode->disabled = " << entityNode->entityName << ", " << int(entityNode->disabled) << endl;
	}
	#endif

	#ifdef GIA_IMPLEMENT_NON_STANFORD_CORE_NLP_CODEPENDENCIES_CROSS_SENTENCE_REFERENCING
	#ifdef GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_SENTENCE
	//restore critical variables: used for GIA translator reference paser only - cleared every time a new sentence is parsed
	for(unordered_map<string, GIAentityNode*>::iterator networkIndexEntityNodesListIter = entityNodesActiveListNetworkIndexes->begin(); networkIndexEntityNodesListIter != entityNodesActiveListNetworkIndexes->end(); networkIndexEntityNodesListIter++)
	{
		GIAentityNode* entityNode = networkIndexEntityNodesListIter->second;
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
		determineGIAconnectionistNetworkPOStypeNames(currentSentenceInList->firstFeatureInList, NLPfeatureParser);
		#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
		#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD
		writeSemanticParserCorpusFile(currentSentenceInList->firstFeatureInList);
		#else
		string sentenceSemanticRelationsText = generateSemanticParserCorpusSemanticRelationsText(getFirstRelationInSemanticParserSentenceList());
		writeSemanticParserCorpusFile(currentSentenceInList->firstFeatureInList, &sentenceSemanticRelationsText);
		#endif
		#endif
		#ifdef GIA2_SEMANTIC_PARSER
		if(!generateAllPermutationsFromSemanticRelationsFile(currentSentenceInList->firstFeatureInList, NLPfeatureParser))
		{
			cout << "generateAllPermutationsFromSemanticRelationsFile() failed" << endl;
			exit(0);
		}
		#endif
	}
	#endif
}

void disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAentityNode* GIAfeatureTempEntityNodeArray[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			if(GIAfeatureTempEntityNodeArray[w]->disabled)
			{
				disableNetworkIndexEntityBasedUponFirstSentenceToAppearInNetwork(GIAentityNodeArray[w]);
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



	
#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
void createParentsOfSubclassEntities(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAnetworkIndexNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList)
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNode* subclassNetworkIndexEntity = GIAnetworkIndexNodeArray[w];
			if(subclassNetworkIndexEntity->convertToSubClass)
			{
				#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
				subclassNetworkIndexEntity->isSubClass = true;
				#endif
		
				string parentClassName = getParentClassEntityNameFromSubClassEntityName(subclassNetworkIndexEntity->entityName);
				
				#ifdef GIA_DEBUG
				//cout << "adding parentClassName: " << parentClassName << endl;
				#endif
				
				bool parentNetworkIndexEntityAlreadyExistant = false;
				GIAentityNode* parentClassNetworkIndexEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&parentClassName, &parentNetworkIndexEntityAlreadyExistant, entityNodesActiveListNetworkIndexes);
				if(!parentNetworkIndexEntityAlreadyExistant)
				{
					//cout << "creating parentClassNetworkIndexEntity: " << parentClassName << endl;
					sentenceNetworkIndexEntityNodesList->push_back(parentClassNetworkIndexEntity);

					#ifdef GIA_SET_ENTITY_ENTITY_AND_SENTENCE_INDICIES_NORMALLY
					parentClassNetworkIndexEntity->entityIndexTemp = GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX;	//there is no entity index associated with the artifically added subclass parent networkIndex
					parentClassNetworkIndexEntity->sentenceIndexTemp = subclassNetworkIndexEntity->sentenceIndexTemp;
					#endif	
					
					linkSubclassEntitiesWithParentClassEntities(subclassNetworkIndexEntity, parentClassNetworkIndexEntity, false, false);		
				}	
				else
				{	
					linkSubclassEntitiesWithParentClassEntities(subclassNetworkIndexEntity, parentClassNetworkIndexEntity, false, true);
				}	
			}
		}
	}
}

#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
void createAdditionalSubclassEntities(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, vector<GIAentityNode*>* sentenceNetworkIndexEntityNodesList, int sentenceIndex)
{
	//eg The red bat is a xenomorph. -> create xenomorph_bat, alias link xenomorph concept with xenomorph_bat concept
		//OLD: eg The red bat is a xenomorph. -> create xenomorph_bat, definition link xenomorph_bat concept with xenomorph concept, and definitition link red bat to xenomorph_bat concept
	
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNode* entity = GIAentityNodeArray[w];
			if(entity->addSubClass)
			{
				if(!(entity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT))	//assume true
				{
					GIAentityNode* definitionEntity = NULL;
					bool foundDefinitionEntity = false;
					for(vector<GIAentityConnection*>::iterator definitionNodeListIterator = entity->entityNodeDefinitionList->begin(); definitionNodeListIterator < entity->entityNodeDefinitionList->end(); definitionNodeListIterator++)
					{
						GIAentityNode* definitionEntityTemp = (*definitionNodeListIterator)->entity;
						if(definitionEntityTemp->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)	//assume true
						{
							#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
							if((*definitionNodeListIterator)->sentenceIndexTemp = sentenceIndex)
							{//in the case that a substance has been assigned multiple definitions in the text, eg "The animal is a dog. The animal is an alsatian", in which case "The animal" entity node will be connected to both dog and alsation (due to GIA advanced referencing).
							#else
							//NB dream mode has not yet been executed so all concepts connected to this entity will have been defined in the same sentence
							#endif
								foundDefinitionEntity = true;
								definitionEntity = definitionEntityTemp;	//eg xenomorph
							#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
							}
							#endif
						}
					}
					
					if(foundDefinitionEntity)
					{
						#ifdef GIA_DEBUG
						cout << "createAdditionalSubclassEntities{}: entity: " << entity->entityName << endl;
						cout << "createAdditionalSubclassEntities{}: definitionEntity: " << definitionEntity->entityName << endl;
						#endif
							
						string subClassEntityName = createSubClassEntityName(definitionEntity->entityName, entity->entityName);	//eg apple_fruit / xenomorph_bat
						//cout << "subClassEntityName = " << subClassEntityName << endl;
						bool subClassNetworkIndexEntityAlreadyExistant = false;
						GIAentityNode* subClassNetworkIndexEntity = findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&subClassEntityName, &subClassNetworkIndexEntityAlreadyExistant, entityNodesActiveListNetworkIndexes);
						if(!subClassNetworkIndexEntityAlreadyExistant)
						{
							#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
							subClassNetworkIndexEntity->isSubClass = true;
							#endif
		
							#ifdef GIA_DEBUG
							cout << "createAdditionalSubclassEntities{}: creating subClassNetworkIndexEntity: " << subClassEntityName << endl;
							#endif
							sentenceNetworkIndexEntityNodesList->push_back(subClassNetworkIndexEntity);

							#ifdef GIA_SET_ENTITY_ENTITY_AND_SENTENCE_INDICIES_NORMALLY
							subClassNetworkIndexEntity->entityIndexTemp = GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX;	//there is no entity index associated with the artifically added subclass parent networkIndex
							subClassNetworkIndexEntity->sentenceIndexTemp = entity->sentenceIndexTemp;
							#endif	

							GIAentityNode* definitionNetworkIndexEntity = getPrimaryNetworkIndexNodeDefiningInstance(definitionEntity);
							linkSubclassEntitiesWithParentClassEntities(definitionNetworkIndexEntity, subClassNetworkIndexEntity, false, false);	//links xenomorph concept with xenomorph_bat concept

							/* //changed 2m7a
							bool sameReferenceSet = false;	//this is required for dreamModeLinkSpecificConceptsAndActions
							#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
							#ifdef GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES
							//now create a definition link between the entity and the subClass concept (since this will not be created by GIA dream mode)
							GIAentityNode* subclassNonspecificConcept = getNonspecificConceptEntityFromNetworkIndex(subClassNetworkIndexEntity);
							addDefinitionToEntity(entity, subclassNonspecificConcept, sameReferenceSet);	//links red bat to xenomorph_bat concept
							//#ifdef GIA_DEBUG
							cout << "createAdditionalSubclassEntities{}: linking entity: " << entity->entityName << " to " << subclassNonspecificConcept->entityName << endl;
							//#endif
							#else
							addDefinitionToEntity(entity, subClassNetworkIndexEntity, sameReferenceSet);	//links red bat to xenomorph_bat networkIndex							
							#endif
							#endif
							*/
						}
					}
				}			
			}
		}
	}
}

#endif

void linkSubclassEntitiesWithParentClassEntities(GIAentityNode* subclassNetworkIndexEntity, GIAentityNode* parentClassNetworkIndexEntity, bool linkAsAlias, bool onlyLinkIfSubclassOrParentConceptNotFound)
{
	string parentClassName = parentClassNetworkIndexEntity->entityName;
				
	#ifdef GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES
	bool subclassToParentEntityConnectionAlreadyMade = false;
	GIAentityNode* subclassNonspecificConcept = getNonspecificConceptEntityFromNetworkIndex(subclassNetworkIndexEntity);
	bool subclassConceptFound = false;
	if(subclassNonspecificConcept != NULL)
	{
		//cout << "subclassNonspecificConcept found" << endl;
		subclassConceptFound = true;
		for(vector<GIAentityConnection*>::iterator definitionNodeListIterator = subclassNonspecificConcept->entityNodeDefinitionList->begin(); definitionNodeListIterator < subclassNonspecificConcept->entityNodeDefinitionList->end(); definitionNodeListIterator++)
		{
			GIAentityNode* definitionEntity = (*definitionNodeListIterator)->entity;
			if(definitionEntity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT)	//assume true
			{
				if(definitionEntity->entityName == parentClassName)
				{
					subclassToParentEntityConnectionAlreadyMade = true;
				}
			}
		}
	}
	else
	{
		subclassNonspecificConcept = createNewNonspecificConcept(subclassNetworkIndexEntity);
		#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
		subclassNonspecificConcept->isSubClass = true;
		#endif
	}

	if(!subclassToParentEntityConnectionAlreadyMade)
	{
		//should a new subclassNonspecificConcept be defined here regardless; ie GIAentityNode* subclassNonspecificConcept = createNewNonspecificConcept(subclassNetworkIndexEntity)? (because new concepts are created every sentence, even for non-specific concepts; eg "Pies are apples. Pies are berries.")
		
		bool performLink = false;
		
		GIAentityNode* parentClassNonspecificConcept = getNonspecificConceptEntityFromNetworkIndex(parentClassNetworkIndexEntity);
		if(parentClassNonspecificConcept == NULL)
		{
			performLink = true;
			parentClassNonspecificConcept = createNewNonspecificConcept(parentClassNetworkIndexEntity);
		}
		else
		{
			if(!subclassConceptFound)
			{
				performLink = true;
			}
		}

		if(performLink)
		{
			#ifdef GIA_DEBUG
			cout << "linkSubclassEntitiesWithParentClassEntities{}: entity->convertToSubClass - creating connection between subclass entity and parent" << endl;
			#endif
			bool sameReferenceSet = false;	//this is required for dreamModeLinkSpecificConceptsAndActions
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			if(linkAsAlias)
			{
				addDefinitionToEntityMarkConnectionAsAlias(subclassNonspecificConcept, parentClassNonspecificConcept, sameReferenceSet);
			}
			else
			{
			#endif
				addDefinitionToEntity(subclassNonspecificConcept, parentClassNonspecificConcept, sameReferenceSet);
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			}
			#endif
		}
	}		
	#else
	bool subclassToParentEntityConnectionAlreadyMade = false;
	for(vector<GIAentityConnection*>::iterator definitionNodeListIterator = subclassNetworkIndexEntity->entityNodeDefinitionList->begin(); definitionNodeListIterator < subclassNetworkIndexEntity->entityNodeDefinitionList->end(); definitionNodeListIterator++)
	{
		GIAentityNode* definitionEntity = (*entityIter);
		if(definitionEntity->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX)	//assume true
		{
			if(definitionEntity->entityName == parentClassName)
			{
				subclassToParentEntityConnectionAlreadyMade = true;
			}
		}
	}

	if(!subclassToParentEntityConnectionAlreadyMade)
	{
		#ifdef GIA_DEBUG
		cout << "linkSubclassEntitiesWithParentClassEntities{}: entity->convertToSubClass - creating connection between subclass entity and parent" << endl;
		#endif
		#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
		if(linkAsAlias)
		{
			addDefinitionToEntityMarkConnectionAsAlias(subclassNetworkIndexEntity, parentClassNetworkIndexEntity, true);
		}
		else
		{
		#endif
			addDefinitionToEntity(subclassNetworkIndexEntity, parentClassNetworkIndexEntity, true);
		#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
		}
		#endif
	}
	#endif							

}

#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
void detectUserDeclaredSubclassEntities(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNode* entity = GIAentityNodeArray[w];
			if(entity->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE)	//ie !isAction && !isCondition (prevent multiword prepositions [and actions, in case multiword actions are implemented in the future])
			{
				if((entity->entityName).find(GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					entity->isSubClass = true;	
				}
			}
		}
	}		
}
#endif

#endif

#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
void dreamModeLinkSpecificConceptsAndActions()
{
	vector<GIAentityNode*>* entityNodesActiveListComplete = getTranslatorEntityNodesCompleteList();

	identifyReferenceSetsSpecificConceptsAndLinkWithConcepts(entityNodesActiveListComplete);
		
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
		if((entity->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE) && !(entity->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT) && !(entity->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX))
		{
			GIAentityNode* networkIndexEntity = (entity->entityNodeDefiningThisInstance->back())->entity;

			GIAentityNode* nonSpecificConceptEntity = NULL;
			bool nonspecificConceptAlreadyExists = false;
			for(vector<GIAentityNode*>::iterator entityIter2 = entityNodesActiveListComplete->begin(); entityIter2 != entityNodesActiveListComplete->end(); entityIter2++)
			{
				GIAentityNode* entity2 = (*entityIter2);
				if((entity->entityType == GIA_ENTITY_TYPE_TYPE_SUBSTANCE) && (entity2->entityType == GIA_ENTITY_TYPE_TYPE_CONCEPT) && !(entity->entityType == GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX))
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
				nonSpecificConceptEntity = addConceptToConceptDefinition(networkIndexEntity);
			}

			//found instance in network matching concept...
			#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
			bool sameReferenceSet = false;
			#else
			bool sameReferenceSet = IRRELEVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING;
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
							cout << "invertOrDuplicateConditionsIfRequired{}: inverseConditionRequired: conditionName = " << conditionName  << endl;
							#endif
							createNewInverseConditionEntity(currentRelationInList, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, inverseConditionName, featureArrayTemp);

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
						#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
						if(twoWayConditionRequired)
						{
							#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS_DEBUG
							cout << "invertOrDuplicateConditionsIfRequired{}: twoWayConditionRequired: conditionName = " << conditionName << endl;
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
							lastRelationInList->next = new GIArelation();
							#ifdef GIA_DEBUG
							//cout << "inverseRelationTwoWay" << endl;
							#endif
							#endif
							currentRelationInList->relationTwoWay = true;
							#ifdef GIA_DEBUG
							//cout << "relationTwoWay" << endl;
							#endif
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
	//requires GIA_INITIALISE_PREPOSITION_ENTITIES_AT_START_OF_TRANSLATOR_NEW?
	int inverseConditionEntityIndex = currentSentenceInList->conditionEntityArtificialIndexCurrent;
	currentSentenceInList->conditionEntityArtificialIndexCurrent = currentSentenceInList->conditionEntityArtificialIndexCurrent - 1;

	GIAentityNodeArrayFilled[inverseConditionEntityIndex] = true;
	GIAentityNode* inverseConditionEntity = new GIAentityNode();
	inverseConditionEntity->entityName = inverseConditionName;
	inverseConditionEntity->wordOrig = inverseConditionName;	//is this necessary?
	//why not set inverseConditionEntity->entityIndexTemp and inverseConditionEntity->sentenceIndexTemp?
	#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS
	currentRelationInList->relationTypeNonInversed = currentRelationInList->relationType;
	currentRelationInList->relationTypeIndexNonInversed = currentRelationInList->relationTypeIndex;
	#endif
	currentRelationInList->relationType = string(STANFORD_PARSER_PREPOSITION_PREPEND) + inverseConditionName;
	currentRelationInList->relationTypeIndex = inverseConditionEntityIndex;
	currentRelationInList->inverseRelation = true;
	GIAentityNodeArray[inverseConditionEntityIndex] = inverseConditionEntity;

	featureArrayTemp[inverseConditionEntityIndex] = new GIAfeature();
	featureArrayTemp[inverseConditionEntityIndex]->word = inverseConditionName;
	featureArrayTemp[inverseConditionEntityIndex]->lemma = inverseConditionName;	//is this necessary?
	featureArrayTemp[inverseConditionEntityIndex]->entityIndex = inverseConditionEntityIndex;
}


#endif




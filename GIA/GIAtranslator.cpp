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
 * Project Version: 2d6a 16-February-2014
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
#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
#include "XMLrulesClass.h"
#endif
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
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(Paragraph * firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int * maxNumberSentences, bool parseGIA2file, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, bool useCodeextensionsHeirachy)
#else
bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(Paragraph * firstParagraphInList, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int * maxNumberSentences, bool parseGIA2file)
#endif
{
	bool result = true;

	//cout << "ak1" << endl;
	
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
	
	//cout << "ak2" << endl;

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
	if(!createSemanticNetworkBasedUponDependencyParsedSentences(firstParagraphInList, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, timeConditionNodesActiveList, isQuery, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file, firstCodeextensionInHeirachy, codeextensionsList, useCodeextensionsHeirachy))
	#else
	if(!createSemanticNetworkBasedUponDependencyParsedSentences(firstParagraphInList, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, timeConditionNodesActiveList, isQuery, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file))
	#endif
	{
		result = false;
	}

	//cout << "ak3" << endl;
	
	setCurrentDirectory(tempFolderCharStar);

	//cout << "ak4" << endl;

	if(!parseGIA2file)
	{
		#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
		string originalInputFileName = "";
		originalInputFileName = originalInputFileName + inputTextNLPrelationXMLfileName + " " + inputTextNLPfeatureXMLfileName;
		outputInternalRelationsInRelexFormat(&outputCFFfileName, &originalInputFileName, firstParagraphInList, NLPdependencyRelationsParser, NLPfeatureParser, NLPexeFolderArray);
		#endif
	}

	//cout << "ak5" << endl;

	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	//count maxNumberSentences
	Sentence * currentSentenceInList = firstParagraphInList->firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		if(currentSentenceInList->sentenceIndex > *maxNumberSentences)
		{
			*maxNumberSentences = currentSentenceInList->sentenceIndex;
		}
		currentSentenceInList = currentSentenceInList->next;
	}
	#endif
	
	//cout << "ak6" << endl;

	return result;
}

#ifdef USE_CE
bool createSemanticNetworkBasedUponDependencyParsedSentences(Paragraph * firstParagraphInList, vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, bool useCodeextensionsHeirachy)
#else
bool createSemanticNetworkBasedUponDependencyParsedSentences(Paragraph * firstParagraphInList, vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
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

	Paragraph * currentParagraph = firstParagraphInList;
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;

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
			convertParagraphSentenceRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstParagraphInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);
		}
		else
		{
		#endif
			convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);
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
void convertParagraphSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Paragraph * firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
{
	Paragraph * currentParagraphInList = firstParagraphInList;
	while(currentParagraphInList->next != NULL)
	{
		Sentence * firstSentenceInList = currentParagraphInList->firstSentenceInList;

		convertSentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);

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

	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	dreamModeLinkSpecificConceptsAndActions();
	#endif
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
		convertSentenceSyntacticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, sentenceConceptEntityNodesList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		#else
		//convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		convertCEsentenceListRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInArtificialList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);
		#endif
	#ifdef GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS
	}
	#endif
}
#endif

void convertSentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
{
	Sentence * currentSentenceInList = firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		convertSentenceRelationsIntoGIAnetworkNodesWrapper(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, parseGIA2file);

		currentSentenceInList = currentSentenceInList->next;
	}

	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	dreamModeLinkSpecificConceptsAndActions();
	#endif
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


void convertSentenceRelationsIntoGIAnetworkNodesWrapper(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool parseGIA2file)
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
	#ifdef GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
	setLinkPreestablishedReferencesGIA(false);
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
	#ifdef GIA_USE_CORPUS_DATABASE
	if(parseGIA2file)
	{
		if(currentSentenceInList->corpusLookupSuccessful)
		{
			convertSentenceSemanticRelationsIntoGIAnetworkNodes(sentenceConceptEntityNodesList, sentenceTimeConditionNodesList, firstSentenceInList, currentSentenceInListTemp, &sentenceConceptEntityNodesListTempNotUsed1, NLPfeatureParser, false, NULL);
			//cout << "2 corpusLookupSuccessful" << endl;
		}
	}
	#endif
	if(!parseGIA2file || (parseGIA2file && !(currentSentenceInList->corpusLookupSuccessful)))
	{
		//cout << "2 !corpusLookupSuccessful" << endl;
		convertSentenceSyntacticRelationsIntoGIAnetworkNodes(sentenceConceptEntityNodesList, sentenceTimeConditionNodesList, firstSentenceInList, currentSentenceInListTemp, &sentenceConceptEntityNodesListTempNotUsed1, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, false, NULL);
	}
	
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

	int numberReferenceSets = identifyReferenceSets(sentenceConceptEntityNodesList, NLPdependencyRelationsType);	//NB NLPdependencyRelationsType is no longer used here

	#ifdef GIA_USE_DATABASE
	setUseDatabase(useDatabaseOriginal);
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG
	cout << "\n\t\t\t error: GIA_USE_ADVANCED_REFERENCING is under development (3createGIACoreferenceInListBasedUponIdentifiedReferenceSets)\n" << endl;
	#endif

	#ifdef GIA_ADVANCED_REFERENCING_DEBUG_SIMPLE2
	cout << "createGIAcoreferenceInListBasedUponIdentifiedReferenceSets:" << endl;
	#endif
	createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(sentenceConceptEntityNodesList, entityNodesActiveListConcepts, firstGIACoreferenceInList, numberReferenceSets);

	#ifndef GIA_FREE_MEMORY2
	setSaveNetwork(true);
	#endif
	#ifdef GIA_RECORD_LINK_PREESTABLISHED_REFERENCES_GIA
	setLinkPreestablishedReferencesGIA(true);
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
	#ifdef GIA_USE_CORPUS_DATABASE
	if(parseGIA2file)
	{
		if(currentSentenceInList->corpusLookupSuccessful)
		{
			convertSentenceSemanticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, NLPfeatureParser, true, firstGIACoreferenceInList);
		}
	}
	#endif
	if(!parseGIA2file || (parseGIA2file && !(currentSentenceInList->corpusLookupSuccessful)))
	{
		convertSentenceSyntacticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations, true, firstGIACoreferenceInList);
	}
	
	//cout << "ak1" << endl;
	
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
	
	//cout << "ak2" << endl;
#else

	vector<GIAentityNode*> sentenceConceptEntityNodesListTempNotUsed;
	#ifdef GIA_USE_CORPUS_DATABASE
	if(parseGIA2file)
	{	
		if(currentSentenceInList->corpusLookupSuccessful)
		{
			convertSentenceSemanticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, NLPfeatureParser);
		}
	}
	#endif
	if(!parseGIA2file || (parseGIA2file && !(currentSentenceInList->corpusLookupSuccessful)))
	{
		convertSentenceSyntacticRelationsIntoGIAnetworkNodes(entityNodesActiveListConcepts, timeConditionNodesActiveList, firstSentenceInList, currentSentenceInList, &sentenceConceptEntityNodesListTempNotUsed, NLPfeatureParser, NLPdependencyRelationsType, NLPassumePreCollapsedStanfordRelations);	
	}
#endif
}

#ifdef GIA_USE_ADVANCED_REFERENCING
void convertSentenceSyntacticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool linkPreestablishedReferencesGIA,  GIACoreference * firstGIACoreferenceInList)
#else
void convertSentenceSyntacticRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations)
#endif
{
	Relation * currentRelationInList;

	/*
	cout << "dependency relations: " << endl;
	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		string relationType = currentRelationInList->relationType;
		cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << "-" << currentRelationInList->relationGovernorIndex << ", " << currentRelationInList->relationDependent << "-" << currentRelationInList->relationDependentIndex << ")" << endl;
		currentRelationInList = currentRelationInList->next;
	}
	*/
	
	
	#ifndef GIA_USE_ADVANCED_REFERENCING
	bool linkPreestablishedReferencesGIA = true;	//irrelevant
	#endif
	
	//cout << "linkPreestablishedReferencesGIA = " << linkPreestablishedReferencesGIA << endl;
	
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
	locateAndAddAllFeatureTempEntities(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPdependencyRelationsType);

	Feature * featureArrayTemp[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
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
	redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllContinuousVerbs(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, featureArrayTemp);
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
		applyGIATranslatorGenericXMLfunctions("GIAtranslatorRedistributeStanfordRelations", currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NULL, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
		#else
		redistributeStanfordRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
		#endif
	}
	#endif
	#ifdef GIA_SUPPORT_ALIASES_RELEX_COMPATIBILITY
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
		applyGIATranslatorGenericXMLfunctions("GIAtranslatorRedistributeRelexRelations", currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NULL, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
		#else
		redistributeRelexRelations(currentSentenceInList, GIAentityNodeArrayFilled, GIAfeatureTempEntityNodeArray, NLPfeatureParser, featureArrayTemp);
		#endif
	}
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
	
	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "\nBefore substance declarations: " << endl;
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			if(!(GIAentityNodeArray[i]->disabled))
			{
				cout << "\ti = " << i << endl;
				cout << "GIAentityNodeArray[i]->entityName = " << GIAentityNodeArray[i]->entityName << endl;
				cout << "GIAentityNodeArray[i]->isConcept = " << GIAentityNodeArray[i]->isConcept << endl;
				cout << "GIAentityNodeArray[i]->isSubstanceConcept = " << GIAentityNodeArray[i]->isSubstanceConcept << endl;
			}
		}
	}
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "pass 4; disable concept entities based on feature temp entity node array" << endl;
	#endif
	//transfer disabled substances across execution#1 [this is required since GIAtranslatorRedistributeStanfordRelations operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of concept entity nodes {whose values would have been automatically transferred their instances upon creation}...]
	disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(GIAentityNodeArrayFilled, GIAconceptNodeArray, GIAfeatureTempEntityNodeArray);

	#ifdef GIA_TRANSLATOR_DEBUG
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(featureArrayTemp[w] != NULL)
		{
			cout << "1: " << featureArrayTemp[w]->lemma << ", w = " << w << endl;
		}
	}

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
			cout << "filled" << endl;
			if(!(GIAconceptNodeArray[w]->disabled))
			{
				Feature * currentFeature = featureArrayTemp[w];
				cout << w << " = " << currentFeature->lemma << endl;
			}
		}
	}
	#endif

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
	applyGIATranslatorGenericXMLfunctions("GIAtranslatorDefineSubstances", currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, NULL, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA);
	#else
	defineSubstances(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, referenceTypeHasDeterminateCrossReferenceNumberArray, featureArrayTemp, NLPdependencyRelationsType);
	#endif

	#ifdef GIA_TRANSLATOR_DEBUG
	cout << "\nAfter substance declarations: " << endl;
	for(int i=0; i<MAX_NUMBER_OF_WORDS_PER_SENTENCE; i++)
	{
		if(GIAentityNodeArrayFilled[i])
		{
			if(!(GIAentityNodeArray[i]->disabled))
			{
				cout << "\ti = " << i << endl;
				cout << "GIAentityNodeArray[i]->entityName = " << GIAentityNodeArray[i]->entityName << endl;
				cout << "GIAentityNodeArray[i]->isConcept = " << GIAentityNodeArray[i]->isConcept << endl;
				cout << "GIAentityNodeArray[i]->isSubstanceConcept = " << GIAentityNodeArray[i]->isSubstanceConcept << endl;
			}
		}
	}
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
					#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
					conceptNode->mustSetIsSubstanceConceptBasedOnApposRelation = false; //added 29 Sept 2013
					conceptNode->alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp = false;	//added 29 Sept 2013
					conceptNode->isPronounReference = false;	//added 29 Sept 2013
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

#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
void dreamModeLinkSpecificConceptsAndActions()
{
	vector<GIAentityNode*> * entityNodesActiveListComplete = getTranslatorEntityNodesCompleteList();

	identifyReferenceSetsSpecificConceptsAndLinkWithSubstanceConcepts(entityNodesActiveListComplete);
}
#endif

#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
bool applyGIATranslatorGenericXMLfunctions(string translatorFileName, Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, Feature * featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA)
{
	//int tempindex = 1;

	bool result = true;
	XMLparserTag * firstTagInRulesTag = parseTagDownALevel(GIAfirstTagInXMLfile, RULES_XML_TAG_rules, &result);
	if(result)
	{
		XMLparserTag * currentTag = firstTagInRulesTag;
		bool foundTranslatorTag = false;
		XMLparserTag * firstTagInTranslatorTag = NULL;
		while((currentTag->nextTag != NULL) && !foundTranslatorTag)
		{
			if(currentTag->name == RULES_XML_TAG_translator)
			{
				foundTranslatorTag = true;
				firstTagInTranslatorTag = parseTagDownALevel(currentTag, RULES_XML_TAG_translator, &result);
			}
			currentTag = currentTag->nextTag;
		}
		if(foundTranslatorTag)
		{
			XMLparserTag * currentFileTag = firstTagInTranslatorTag;
			while(currentFileTag->nextTag != NULL)
			{
				if(currentFileTag->firstAttribute->name == RULES_XML_ATTRIBUTE_name)
				{
					if(currentFileTag->firstAttribute->value == translatorFileName)
					{
						#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
						cout << "applyGIATranslatorGenericXMLfunctions translatorFileName: " << translatorFileName << endl;
						#endif
						/*
						if(translatorFileName == "GIAtranslatorLinkEntities")
						{
							cout << "GIAtranslatorLinkEntities" << endl;
						}
						*/

						XMLparserTag * firstTagInFileTag = parseTagDownALevel(currentFileTag, RULES_XML_TAG_file, &result);
						if(result)
						{
							XMLparserTag * currentFunctionTag = firstTagInFileTag;
							while(currentFunctionTag->nextTag != NULL)
							{
								string functionName = "";
								if(currentFunctionTag->firstAttribute->name == RULES_XML_ATTRIBUTE_name)
								{
									functionName = currentFunctionTag->firstAttribute->value;
								}

								#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
								cout << "applyGIATranslatorGenericXMLfunctions functionName: " << functionName << endl;
								#endif

								if(currentFunctionTag->firstLowerLevelTag != NULL)
								{
									XMLparserTag * firstTagInFunction = currentFunctionTag->firstLowerLevelTag;
									XMLparserTag * currentParamTag = firstTagInFunction;
									while(currentParamTag->nextTag != NULL)
									{
										if((currentParamTag->name == RULES_XML_TAG_paramDepRel) || (currentParamTag->name == RULES_XML_TAG_paramEntity))
										{
											bool depRelOrEntity = false;
											if(currentParamTag->name == RULES_XML_TAG_paramDepRel)
											{
												depRelOrEntity = true;
											}
											bool executeOrReassign = false;
											if(currentParamTag->firstAttribute->name == RULES_XML_ATTRIBUTE_mode)
											{
												if(currentParamTag->firstAttribute->value == "execute")
												{
													executeOrReassign = true;
												}
												else if(currentParamTag->firstAttribute->value == "redistribute")
												{
													executeOrReassign = false;
												}
												else
												{
													cout << "applyGIATranslatorGenericXMLfunctions() error: illegal currentParamTag->firstAttribute->value = " << currentParamTag->firstAttribute->value << endl;
												}
											}


											//load options and execute genericDependecyRelationInterpretation/genericEntityInterpretation
											if(!applyGIATranslatorGenericXMLparam(currentParamTag, depRelOrEntity, executeOrReassign, currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, entityNodesActiveListConcepts, featureArrayTemp, NLPdependencyRelationsType, NLPfeatureParser, linkPreestablishedReferencesGIA, functionName))
											{
												result = false;
											}

											/*
											if(functionName == "redistributeRelexRelationsInterpretOfAsObjectForContinuousVerbs")
											{
												cout << "\t\t" << tempindex << ": dependency relations: " << endl;
												tempindex++;
												Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
												while(currentRelationInList->next != NULL)
												{
													if(!(currentRelationInList->disabled))
													{
														string relationType = currentRelationInList->relationType;
														GIAentityNode * relationGoverner = GIAentityNodeArray[currentRelationInList->relationGovernorIndex];
														GIAentityNode * relationDependent = GIAentityNodeArray[currentRelationInList->relationDependentIndex];

														cout << currentRelationInList->relationType << "(" << currentRelationInList->relationGovernor << ", " << currentRelationInList->relationDependent << ")" << endl;
													}
													currentRelationInList = currentRelationInList->next;
												}
											}
											*/
										}
										currentParamTag = currentParamTag->nextTag;
									}
								}

								/*
								if(functionName == "defineSubstanceConcepts")
								{
									exit(0);
								}
								*/

								currentFunctionTag = currentFunctionTag->nextTag;
							}
						}
					}
				}
				currentFileTag = currentFileTag->nextTag;
			}
		}
	}
}

bool applyGIATranslatorGenericXMLparam(XMLparserTag * currentParamTag, bool depRelOrEntity, bool executeOrReassign, Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, Feature * featureArrayTemp[], int NLPdependencyRelationsType, int NLPfeatureParser, bool linkPreestablishedReferencesGIA, string functionName)
{
	if(currentParamTag->firstLowerLevelTag != NULL)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
		cout << "\tapplyGIATranslatorGenericXMLparam: " << currentParamTag->name << endl;
		#endif

		GIAgenericDepRelInterpretationParameters paramDepRel(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, executeOrReassign);
		paramDepRel.functionName = functionName;
		paramDepRel.entityNodesActiveListConcepts = entityNodesActiveListConcepts;
		paramDepRel.NLPdependencyRelationsType = NLPdependencyRelationsType;
		paramDepRel.executeOrReassign = executeOrReassign;
		GIAgenericEntityInterpretationParameters paramEntity(currentSentenceInList, GIAentityNodeArrayFilled, GIAentityNodeArray, executeOrReassign);
		paramEntity.functionName = functionName;
		paramEntity.executeOrReassign = executeOrReassign;
		bool passedAssert = true;
		bool assertdisableRelationAfterFinish = false;
		bool asssertsetDefiniteAfterFinish = false;
		int asssertPostProcessingREL_ENT = -1;
		int asssertPostProcessingREL = -1;

		XMLparserTag * firstConfigurationTag = currentParamTag->firstLowerLevelTag;
		XMLparserTag * currentConfigurationTag = firstConfigurationTag;
		while(currentConfigurationTag->nextTag != NULL)
		{
			#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
			cout << "currentConfigurationTag->name: " << currentConfigurationTag->name << endl;
			cout << "currentConfigurationTag->firstAttribute->name: " << currentConfigurationTag->firstAttribute->name << endl;
			#endif

			string RELstring = "";
			string REL_ENTstring = "";
			string FUNC_ENTstring = "";
			string swapIndexstring = "";
			int REL = -1;
			int REL_ENT = -1;
			int FUNC_ENT = -1;
			int swapIndex = -1;
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_REL, &RELstring))
			{
				REL = atoi(RELstring.c_str());
				//cout << "REL = " << REL << endl;
			}
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_REL_ENT, &REL_ENTstring))
			{
				REL_ENT = atoi(REL_ENTstring.c_str());
				//cout << "REL_ENT = " << REL_ENT << endl;
			}
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_FUNC_ENT, &FUNC_ENTstring))
			{
				FUNC_ENT = atoi(FUNC_ENTstring.c_str());
				//cout << "FUNC_ENT = " << FUNC_ENT << endl;
			}
			if(getAttribute(currentConfigurationTag, RULES_XML_ATTRIBUTE_swapIndex, &swapIndexstring))
			{
				swapIndex = atoi(swapIndexstring.c_str());
			}

			if(currentConfigurationTag->name == RULES_XML_TAG_option)
			{
				if(depRelOrEntity)
				{
					genericDepRelInterpretationApplyOptions(&paramDepRel, currentConfigurationTag, REL, REL_ENT, FUNC_ENT, swapIndex);
				}
				else
				{
					genericEntityInterpretationApplyOptions(&paramEntity, currentConfigurationTag);
				}
			}
			else if(currentConfigurationTag->name == RULES_XML_TAG_specialCase)
			{
				string entityCharacteristicsType = "";
				EntityCharacteristic * entityCharacteristics = new EntityCharacteristic();
				if(genericInterpretationGenerateSpecialCase(currentConfigurationTag, entityCharacteristics, &entityCharacteristicsType))
				{
					if(depRelOrEntity)
					{
						genericDepRelInterpretationApplySpecialCase(entityCharacteristics, &paramDepRel, REL, REL_ENT, entityCharacteristicsType);
					}
					else
					{
						genericEntityInterpretationApplySpecialCase(entityCharacteristics, &paramEntity, entityCharacteristicsType);
					}
				}
			}
			else if(currentConfigurationTag->name == RULES_XML_TAG_assert)
			{//store especially defined conditions for pre/post processing of DepRel/Entity interpretation:
				XMLParserAttribute * assertAttribute = currentConfigurationTag->firstAttribute;
				if(assertAttribute->name == RULES_XML_ATTRIBUTE_variable)
				{
					if(assertAttribute->value == "NLPfeatureParserStanfordCoreNLP")
					{
						if(!(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP))
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "whichOrWhatQueryFound")
					{
						bool whichOrWhatQueryFound = false;
						//if(GIAentityNodeArrayFilled[GIA_NLP_START_ENTITY_INDEX]	//"what" is not added to a relation and therefore GIAentityNodeArrayFilled will not be filled
						if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX] != NULL)
						{
							for(int i=0; i<FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES; i++)
							{
								//cout << "featureQueryWordAcceptedByAlternateMethodNameArray[i] = " << featureQueryWordAcceptedByAlternateMethodNameArray[i] << endl;
								//cout << "featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma = " << featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma << endl;	crashes
								if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma == featureQueryWordAcceptedByAlternateMethodNameArray[i])
								{
									//cout << "whichOrWhatQueryFound" << endl;
									whichOrWhatQueryFound = true;
								}
							}
						}
						if(!(whichOrWhatQueryFound))
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "firstWordOfSentenceIsWho")
					{
						bool firstWordOfSentenceIsWho = false;
						#ifdef GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_QVAR_INDEX_SAME_AS_ANOTHER_RELATION_INDEX
						//if(GIAentityNodeArrayFilled[GIA_NLP_START_ENTITY_INDEX])	//approximately the reason: "who" is not added to a relation and therefore GIAentityNodeArrayFilled will not be filled
						if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX] != NULL)
						{
							if(featureArrayTemp[GIA_NLP_START_ENTITY_INDEX]->lemma == REFERENCE_TYPE_QUESTION_QUERY_WHO)
							{
								//cout << "found who" << endl;
								firstWordOfSentenceIsWho = true;
							}
						}
						#else
						if(GIAentityNodeArrayFilled[REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX])
						{
							if(featureArrayTemp[REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX]->lemma == REFERENCE_TYPE_QUESTION_QUERY_WHO)
							{
								firstWordOfSentenceIsWho = true;
							}
						}
						#endif
						if(!firstWordOfSentenceIsWho)
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "isQuestion")
					{
						if(!(currentSentenceInList->isQuestion))
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "disableRelationAfterFinish")
					{
						assertdisableRelationAfterFinish = true;
						asssertPostProcessingREL = REL;
						//cout << "disableRelationAfterFinish: asssertPostProcessingREL = " << asssertPostProcessingREL << endl;
					}
					else if(assertAttribute->value == "setDefiniteAfterFinish")
					{
						asssertsetDefiniteAfterFinish = true;
						asssertPostProcessingREL = REL;
						asssertPostProcessingREL_ENT = REL_ENT;
					}
					else if(assertAttribute->value == "linkPreestablishedReferencesGIA")
					{
						if(!linkPreestablishedReferencesGIA)
						{
							passedAssert = false;
						}
					}
					else if(assertAttribute->value == "NLPdependencyRelationsTypeStanford")
					{
						if(!(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD))
						{
							passedAssert = false;
						}
					}
				}
			}
			else
			{
				cout << "applyGIATranslatorGenericXMLparam(): error - illegal param tag:" << currentConfigurationTag->name  << endl;
				//exit(0);
			}

			currentConfigurationTag = currentConfigurationTag->nextTag;
		}

		if(passedAssert)
		{
			if(depRelOrEntity)
			{
				//cout << "genericDependecyRelationInterpretation()" << endl;
				if(genericDependecyRelationInterpretation(&paramDepRel, REL1))
				{
					if(asssertsetDefiniteAfterFinish)
					{
						featureArrayTemp[paramDepRel.relationEntityIndexFinalResult[asssertPostProcessingREL][asssertPostProcessingREL_ENT]]->grammaticalIsDefinite = true;
					}
					if(assertdisableRelationAfterFinish)
					{
						//cout << "A1" << endl;
						paramDepRel.relationFinalResult[asssertPostProcessingREL]->disabled =  true;
						//cout << "A2" << endl;
					}
				}
			}
			else
			{
				if(genericEntityInterpretation(&paramEntity))
				{
					/*
					if(functionName == "defineSubstanceConcepts")
					{
						cout << "\t\tdefineSubstanceConcepts passed" << endl;
					}
					*/
				}
				/*
				else
				{
					if(functionName == "defineSubstanceConcepts")
					{
						cout << "\t\tdefineSubstanceConcepts failed" << endl;
					}
				}
				*/
			}
		}
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
		cout << "\tEND: applyGIATranslatorGenericXMLparam: " << currentParamTag->name << endl;
		#endif
	}
	else
	{
		cout << "applyGIATranslatorGenericXMLparam(): error - param has no options/special case tags: currentParamTag->name" << currentParamTag->name << endl;
		//exit(0);
	}
}


bool genericInterpretationGenerateSpecialCase(XMLparserTag * xmlTag, EntityCharacteristic * entityCharacteristics, string * type)
{
	bool result = false;

	bool typeFound = false;
	bool variableFound = false;
	bool valueFound = false;

	*type = "";
	string variable = "";
	string value = "";
	string arrayIndex = "";
	string negative = "";

	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_type, type))
	{
		typeFound = true;
	}
	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_variable, &variable))
	{
		entityCharacteristics->name = variable;
		variableFound = true;
	}
	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_value, &value))
	{
		entityCharacteristics->value = value;
		valueFound = true;
	}
	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_arrayIndex, &arrayIndex))
	{
		entityCharacteristics->arrayIndex = atoi(arrayIndex.c_str());
	}
	if(getAttribute(xmlTag, RULES_XML_ATTRIBUTE_negative, &negative))
	{
		if(negative == "true")
		{
			entityCharacteristics->isNegative = true;
		}
	}
	if(typeFound && variableFound && valueFound)
	{
		result = true;
	}
	else
	{
		cout << "genericEntityInterpretationApplySpecialCase() error: !(typeFound && variableFound && valueFound)" << endl;
	}
}

bool genericDepRelInterpretationApplySpecialCase(EntityCharacteristic * entityCharacteristics, GIAgenericDepRelInterpretationParameters * paramDepRel, int REL, int REL_ENT, string type)
{
	if(type == "specialCaseCharacteristicsTestAndVector")
	{
		paramDepRel->specialCaseCharacteristicsTestAndVector[REL][REL_ENT].push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsTestOrVector")
	{
		paramDepRel->specialCaseCharacteristicsTestOrVector[REL][REL_ENT].push_back(entityCharacteristics);
		//cout << "\t*** genericDepRelInterpretationApplySpecialCase: REL = " << REL << ", REL_ENT = " << REL_ENT << "entityCharacteristics->name = " << entityCharacteristics->name << ", entityCharacteristics->value = " << entityCharacteristics->value << endl;
	}
	else if(type == "specialCaseCharacteristicsTestOr2Vector")
	{
		paramDepRel->specialCaseCharacteristicsTestOr2Vector[REL][REL_ENT].push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsTestOr3Vector")
	{
		paramDepRel->specialCaseCharacteristicsTestOr3Vector[REL][REL_ENT].push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsAssignmentVector")
	{
		paramDepRel->specialCaseCharacteristicsAssignmentVector[REL][REL_ENT].push_back(entityCharacteristics);
	}
	else
	{
		cout << "genericDepRelInterpretationApplySpecialCase() error: illegal type" << endl;
	}
}


//string * convertDelimitedStringToArray(string str, char delimiter)

bool genericDepRelInterpretationApplyOptions(GIAgenericDepRelInterpretationParameters * paramDepRel, XMLparserTag * xmlTag, int REL, int REL_ENT, int FUNC_ENT, int swapIndex)
{
	bool result = true;
	XMLParserAttribute * currentAttribute = xmlTag->firstAttribute;
	while(currentAttribute->nextAttribute != NULL)
	{
		if(!genericDepRelInterpretationApplyOption(paramDepRel, currentAttribute, REL, REL_ENT, FUNC_ENT, swapIndex))
		{
			result = false;
		}
		currentAttribute = currentAttribute->nextAttribute;
	}
	return result;
}

bool genericDepRelInterpretationApplyOption(GIAgenericDepRelInterpretationParameters * paramDepRel, XMLParserAttribute * xmlAttribute, int REL, int REL_ENT, int FUNC_ENT, int swapIndex)
{
	bool foundMatch = false;

	genericEntityInterpretationApplyOptionint(&(paramDepRel->numberOfRelations), xmlAttribute, "numberOfRelations", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->parseDisabledRelation[REL]), xmlAttribute, "parseDisabledRelation", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->parseDisabledRelationDuringLink[REL]), xmlAttribute, "parseDisabledRelationDuringLink", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationTest[REL][REL_ENT]), xmlAttribute, "useRelationTest", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->relationTest[REL][REL_ENT]), xmlAttribute, "relationTest", &foundMatch);
	/*
	if(genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationTest[REL][REL_ENT]), xmlAttribute, "useRelationTest", &foundMatch))
	{
		cout << "relationTest = " << (paramDepRel->useRelationTest[REL][REL_ENT]) << endl;
	}
	if(genericEntityInterpretationApplyOptionstring(&(paramDepRel->relationTest[REL][REL_ENT]), xmlAttribute, "relationTest", &foundMatch))
	{
		cout << "relationTest = " << (paramDepRel->relationTest[REL][REL_ENT]) << endl;
	}
	*/
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationTestIsNegative", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationArrayTest[REL][REL_ENT]), xmlAttribute, "useRelationArrayTest", &foundMatch);
	genericEntityInterpretationApplyOptionstringarray(&(paramDepRel->relationArrayTest[REL][REL_ENT]), xmlAttribute, "relationArrayTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->relationArrayTestSize[REL][REL_ENT]), xmlAttribute, "relationArrayTestSize", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationArrayTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationArrayTestIsNegative", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->expectToFindPrepositionTest[REL]), xmlAttribute, "expectToFindPrepositionTest", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRelationIndexTest[REL][REL_ENT]), xmlAttribute, "useRelationIndexTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->relationIndexTestRelationID[REL][REL_ENT]), xmlAttribute, "relationIndexTestRelationID", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->relationIndexTestEntityID[REL][REL_ENT]), xmlAttribute, "relationIndexTestEntityID", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->relationIndexTestIsNegative[REL][REL_ENT]), xmlAttribute, "relationIndexTestIsNegative", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useSpecialCaseCharacteristicsRelationIndexTest[REL][REL_ENT]), xmlAttribute, "useSpecialCaseCharacteristicsRelationIndexTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationIndexTestRelationID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationIndexTestRelationID", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationIndexTestEntityID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationIndexTestEntityID", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->specialCaseCharacteristicsRelationIndexTest[REL][REL_ENT].name), xmlAttribute, "specialCaseCharacteristicsRelationIndexTest", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeRelationEntityIndexReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeRelationEntityIndexReassignment", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeRelationEntityIndexReassignmentRelationID[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityIndexReassignmentRelationID", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeRelationEntityIndexReassignmentRelationEntityID[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityIndexReassignmentRelationEntityID", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->redistributeRelationEntityIndexReassignmentUseOriginalValues[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityIndexReassignmentUseOriginalValues", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeRelationEntityReassignment", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->redistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityReassignment", &foundMatch);
	/*
	if(genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "useRedistributeRelationEntityReassignment", &foundMatch))
	{
		cout << "useRedistributeRelationEntityReassignment: " << (paramDepRel->useRedistributeRelationEntityReassignment[REL][REL_ENT]) << endl;
	}
	if(genericEntityInterpretationApplyOptionstring(&(paramDepRel->redistributeRelationEntityReassignment[REL][REL_ENT]), xmlAttribute, "redistributeRelationEntityReassignment", &foundMatch))
	{
		cout << "redistributeRelationEntityReassignment: " << (paramDepRel->redistributeRelationEntityReassignment[REL][REL_ENT]) << endl;
	}
	*/

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck[REL]), xmlAttribute, "useRedistributeSpecialCaseAuxiliaryIndicatesDifferentReferenceSetCheck", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseRelationEntityReassignmentConcatonate[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseRelationEntityReassignmentConcatonate", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID[REL][REL_ENT][swapIndex]), xmlAttribute, "redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationID", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID[REL][REL_ENT][swapIndex]), xmlAttribute, "redistributeSpecialCaseRelationEntityIndexReassignmentConcatonateRelationEntityID", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useRedistributeSpecialCaseDisableInstanceAndConcept[REL][REL_ENT]), xmlAttribute, "useRedistributeSpecialCaseDisableInstanceAndConcept", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->useSpecialCaseCharacteristicsRelationEntityIndexReassignment[REL][REL_ENT]), xmlAttribute, "useSpecialCaseCharacteristicsRelationEntityIndexReassignment", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignmentRelationID", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID[REL][REL_ENT]), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignmentRelationEntityID", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->specialCaseCharacteristicsRelationEntityIndexReassignment[REL][REL_ENT].name), xmlAttribute, "specialCaseCharacteristicsRelationEntityIndexReassignment", &foundMatch);

	#ifdef GIA_USE_ADVANCED_REFERENCING
	genericEntityInterpretationApplyOptionint(&(paramDepRel->defaultSameSetRelationID), xmlAttribute, "defaultSameSetRelationID", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->defaultSameSetReferenceValue), xmlAttribute, "defaultSameSetReferenceValue", &foundMatch);
	#endif

	genericEntityInterpretationApplyOptionint(&(paramDepRel->functionEntityRelationID[FUNC_ENT]), xmlAttribute, "functionEntityRelationID", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->functionEntityRelationEntityID[FUNC_ENT]), xmlAttribute, "functionEntityRelationEntityID", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->functionToExecuteUponFind), xmlAttribute, "functionToExecuteUponFind", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->mustGenerateConditionTypeName), xmlAttribute, "mustGenerateConditionTypeName", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramDepRel->conditionTypeEntityDefaultName), xmlAttribute, "conditionTypeEntityDefaultName", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramDepRel->conditionTypeEntityDefaultIndex), xmlAttribute, "conditionTypeEntityDefaultIndex", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableEntity[REL][REL_ENT]), xmlAttribute, "disableEntity", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableEntityUseOriginalValues[REL][REL_ENT]), xmlAttribute, "disableEntityUseOriginalValues", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableRelation[REL]), xmlAttribute, "disableRelation", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramDepRel->disableRelationDuringLink[REL]), xmlAttribute, "disableRelationDuringLink", &foundMatch);

	return foundMatch;
}


bool genericEntityInterpretationApplySpecialCase(EntityCharacteristic * entityCharacteristics, GIAgenericEntityInterpretationParameters * paramEntity, string type)
{
	if(type == "specialCaseCharacteristicsTestAndVector")
	{
		paramEntity->specialCaseCharacteristicsTestAndVector.push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsTestOrVector")
	{
		paramEntity->specialCaseCharacteristicsTestOrVector.push_back(entityCharacteristics);
	}
	else if(type == "specialCaseCharacteristicsAssignmentVector")
	{
		paramEntity->specialCaseCharacteristicsAssignmentVector.push_back(entityCharacteristics);
	}
	else
	{
		cout << "genericEntityInterpretationApplySpecialCase() error: illegal type: " << type << endl;
	}
}

bool genericEntityInterpretationApplyOptions(GIAgenericEntityInterpretationParameters * paramEntity, XMLparserTag * xmlTag)
{
	bool result = true;
	XMLParserAttribute * currentAttribute = xmlTag->firstAttribute;
	while(currentAttribute->nextAttribute != NULL)
	{
		if(!genericEntityInterpretationApplyOption(paramEntity, currentAttribute))
		{
			result = false;
		}
		currentAttribute = currentAttribute->nextAttribute;
	}
	return result;
}
bool genericEntityInterpretationApplyOption(GIAgenericEntityInterpretationParameters * paramEntity, XMLParserAttribute * xmlAttribute)
{
	bool foundMatch = false;

	genericEntityInterpretationApplyOptionbool(&(paramEntity->parseDisabledEntity), xmlAttribute, "parseDisabledEntity", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramEntity->useEntityTest), xmlAttribute, "useEntityTest", &foundMatch);
	genericEntityInterpretationApplyOptionstring(&(paramEntity->entityTest), xmlAttribute, "entityTest", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramEntity->entityTestIsNegative), xmlAttribute, "entityTestIsNegative", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramEntity->useEntityArrayTest), xmlAttribute, "useEntityArrayTest", &foundMatch);
	genericEntityInterpretationApplyOptionstringarray(&(paramEntity->entityArrayTest), xmlAttribute, "entityArrayTest", &foundMatch);
	genericEntityInterpretationApplyOptionint(&(paramEntity->entityArrayTestSize), xmlAttribute, "entityArrayTestSize", &foundMatch);
	genericEntityInterpretationApplyOptionbool(&(paramEntity->entityArrayTestIsNegative), xmlAttribute, "entityArrayTestIsNegative", &foundMatch);

	genericEntityInterpretationApplyOptionint(&(paramEntity->functionToExecuteUponFind), xmlAttribute, "functionToExecuteUponFind", &foundMatch);

	genericEntityInterpretationApplyOptionbool(&(paramEntity->disableEntity), xmlAttribute, "disableEntity", &foundMatch);

	return foundMatch;
}


bool genericEntityInterpretationApplyOptionbool(bool * paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		bool paramOptionSetValue;
		if(xmlAttribute->value == "true")
		{
			paramOptionSetValue = true;
		}
		else if(xmlAttribute->value == "false")
		{
			paramOptionSetValue = false;
		}
		else
		{
			cout << "genericEntityInterpretationApplyOptionbool() error: illegal paramOptionSetValue for " << iterationVariable << ": " << xmlAttribute->value << endl;
		}
		//bool paramOptionSetValue = atoi(xmlAttribute->value.c_str());		//if GIA Translator XML file booleans are defined as '1'/'0' instead of 'true'/'false'
		*paramVal = paramOptionSetValue;

		//cout << "genericEntityInterpretationApplyOptionbool(): " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		*foundMatch = true;
		result = true;
	}
	return result;
}
bool genericEntityInterpretationApplyOptionint(int * paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		int paramOptionSetValue = atoi(xmlAttribute->value.c_str());
		*paramVal = paramOptionSetValue;

		//cout << "genericEntityInterpretationApplyOptionint(): " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		*foundMatch = true;
		result = true;
	}
	return result;
}
bool genericEntityInterpretationApplyOptionstring(string * paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		string paramOptionSetValue = xmlAttribute->value;
		*paramVal = paramOptionSetValue;

		//cout << "testEntityCharacteristicIterationstring(): " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		*foundMatch = true;
		result = true;
	}
	return result;
}
bool genericEntityInterpretationApplyOptionstringarray(string ** paramVal, XMLParserAttribute * xmlAttribute, string iterationVariable, bool *foundMatch)
{
	bool result = false;
	if(xmlAttribute->name == iterationVariable)
	{
		string paramOptionSetValue = xmlAttribute->value;

		*paramVal = convertDelimitedStringToArray(paramOptionSetValue, GIA_TRANSLATOR_XML_INTERPRETATION_ARRAY_DELIMITER);

		//cout << "testEntityCharacteristicIterationstringarray(): " << xmlAttribute->name << " = " << paramOptionSetValue << endl;
		*foundMatch = true;
		result = true;
	}
	return result;
}


#endif

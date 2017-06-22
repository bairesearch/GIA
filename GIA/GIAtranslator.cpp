/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: GIAtranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3c5d 21-June-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#include "GIAtranslator.hpp"


//required because parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences was shifted from GIAmain.cpp
//Dependency Relationship Extractor



bool GIAtranslatorClass::parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName, const string NLPexeFolderArray[])
{
	bool result = true;

	GIApreprocessorMultiwordReduction.setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(translatorVariables->isQuery);	//required for local variable access
	if(!translatorVariables->parseGIA2file)
	{
		if(!GIAnlp.parseNLPparserFile(inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, translatorVariables->isQuery, translatorVariables->firstParagraphInList, translatorVariables->NLPfeatureParser, translatorVariables->NLPdependencyRelationsParser, translatorVariables->NLPrelexCompatibilityMode))
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

	if(!this->createSemanticNetworkBasedUponDependencyParsedSentences(translatorVariables))
	{
		result = false;
	}

	SHAREDvars.setCurrentDirectory(outputFolder);

	if(!translatorVariables->parseGIA2file)
	{
		#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
		string originalInputFileName = "";
		originalInputFileName = originalInputFileName + inputTextNLPrelationXMLfileName + " " + inputTextNLPfeatureXMLfileName;
		GIAnlp.outputInternalRelationsInRelexFormat(&outputCFFfileName, &originalInputFileName, translatorVariables->firstParagraphInList, translatorVariables->NLPdependencyRelationsParser, translatorVariables->NLPfeatureParser, NLPexeFolderArray);
		#endif
	}

	//count maxNumberSentences
	GIAsentence* currentSentenceInList = translatorVariables->firstParagraphInList->firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		if(currentSentenceInList->sentenceIndex > translatorVariables->maxNumberSentences)
		{
			translatorVariables->maxNumberSentences = currentSentenceInList->sentenceIndex;
		}
		currentSentenceInList = currentSentenceInList->next;
	}

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

	GIAparagraph* currentParagraph = translatorVariables->firstParagraphInList;
	GIAsentence* firstSentenceInList = translatorVariables->firstParagraphInList->firstSentenceInList;
	translatorVariables->firstSentenceInList = firstSentenceInList;
	
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	GIAsentenceClass.backupOriginalNLPsentenceContent(translatorVariables->firstSentenceInList);
	#endif

	#ifdef USE_CE
	if(useCodeextensionsHeirachy)
	{
		#ifdef GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(firstParagraphInList->next->next != NULL)
		{
			cerr << "convertParagraphSentenceRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy{}: error - CE only supports a single paragraph of text, one codeextension per line" << endl;
			exit(EXIT_ERROR);
		}
		#endif
		this->convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(translatorVariables, firstCodeextensionInHeirachy, codeextensionsList, parseGIA2file);
	}
	else
	{
	#endif
		#ifdef GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
			this->convertParagraphSentenceRelationsIntoGIAnetworkNodes(translatorVariables);
		}
		else
		{
		#endif
			this->convertSentenceListRelationsIntoGIAnetworkNodes(translatorVariables);
		#ifdef GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		}
		#endif
	#ifdef USE_CE
	}
	#endif

	GIAtranslatorOperations.recordNetworkIndexNodesAsDisabledIfTheyAreNotPermanent(translatorVariables->entityNodesActiveListNetworkIndexes);

	#ifdef GIA_DATABASE
	if(translatorVariables->isQuery)
	{
		GIAdatabase.setUseDatabase(useDatabaseOriginal);
	}
	#endif

	return result;
}


#ifdef GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
bool GIAtranslatorClass::convertParagraphSentenceRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	GIAparagraph* currentParagraphInList = translatorVariables->firstParagraphInList;
	while(currentParagraphInList->next != NULL)
	{
		GIAsentence* firstSentenceInList = currentParagraphInList->firstSentenceInList;
		translatorVariables->firstSentenceInList = firstSentenceInList;
		
		this->convertSentenceListRelationsIntoGIAnetworkNodes(translatorVariables);

		currentParagraphInList = currentParagraphInList->next;
	}

	this->performGIApostProcessing(translatorVariables);
	
	return result;
}
#endif



#ifdef USE_CE
//NB is convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy compatible with Stanford CoreNLP coreferences? [NB sentences are mixed up, but are sentence id's still valid? If there is a problem, then possibly need to either a) re-execute NLP on each codeextension heirachy, or b) revert to GIA pronominal coreference detection (see RelEx function to do this)
bool GIAtranslatorClass::convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	//link GIAsentences with each generated CEcodeextension
	GIAsentence* currentSentenceInList = translatorVariables->firstSentenceInList;
	for(vector<CECodeextension*>::iterator codeextensionIter = translatorVariables->codeextensionsList->begin(); codeextensionIter != translatorVariables->codeextensionsList->end(); codeextensionIter++)
	{
		CECodeextension* currentCodeextensionInHeirachy = *codeextensionIter;
		currentCodeextensionInHeirachy->sentence = currentSentenceInList;
		#ifdef GIA_WITH_CE_CONVERT_PUNCTUATION_MARK_CHARACTERS_TO_FULL_STOPS
		for(int i=0; i<currentCodeextensionInHeirachy->numberSentencesInCodeextension; i++)
		{
			currentSentenceInList = currentSentenceInList->next;
		}
		#else
		currentSentenceInList = currentSentenceInList->next;
		#endif
	}

	#ifdef GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS
	for(codeextensionIter = translatorVariables->codeextensionsList->begin(); codeextensionIter != translatorVariables->codeextensionsList->end(); codeextensionIter++)
	{
		//need to declare new GIA semantic network environment for each heirachical combination
		translatorVariables->entityNodesActiveListNetworkIndexes = new unordered_map<string, GIAentityNode*>;
		translatorVariables->timeConditionNodesActiveList = new unordered_map<long, GIAtimeConditionNode*>;
	#else
		codeextensionIter = codeextensionsList->begin();
	#endif
		CECodeextension* currentCodeextensionInHeirachy = *codeextensionIter;
		currentSentenceInList = currentCodeextensionInHeirachy->sentence;

		GIAsentence* lastSentenceInArtificialList = currentSentenceInList;
		GIAsentence* firstSentenceInArtificialList = generateArtificialSentenceListBasedUponParentCodeextensions(currentCodeextensionInHeirachy, lastSentenceInArtificialList, true);

		GIAsentence* currentSentenceInArtificialList = firstSentenceInArtificialList;
		do
		{
			GIArelation* currentRelationInList = currentSentenceInArtificialList->firstRelationInList;
			while(currentRelationInList->next != NULL)
			{
				currentRelationInList = currentRelationInList->next;
			}

			currentSentenceInArtificialList = currentSentenceInArtificialList->next;
		}
		while(currentSentenceInArtificialList != NULL);

		//convertSentenceRelationsIntoGIAnetworkNodesWrapper(translatorVariables);		//used to be firstSentenceInList, not firstSentenceInArtificialList
		this->convertCEsentenceListRelationsIntoGIAnetworkNodes(translatorVariables);
	#ifdef GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS
	}
	#endif
	
	return result;
}

bool GIAtranslatorClass::convertCEsentenceListRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	GIAsentence* currentSentenceInList = translatorVariables->firstSentenceInList;
	do
	{
		translatorVariables->currentSentence = currentSentenceInList;
		translatorVariables->sentenceIndex = currentSentenceInList->sentenceIndex;
		this->convertSentenceRelationsIntoGIAnetworkNodesWrapper(translatorVariables, false);

		currentSentenceInList = currentSentenceInList->next;
	}
	while(currentSentenceInList != NULL);
	
	return result;
}
#endif

bool GIAtranslatorClass::convertSentenceListRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	GIAsentence* currentSentenceInList = translatorVariables->firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		translatorVariables->currentSentenceInList = currentSentenceInList;
		translatorVariables->sentenceIndex = currentSentenceInList->sentenceIndex;
		this->convertSentenceRelationsIntoGIAnetworkNodesWrapper(translatorVariables);

		currentSentenceInList = currentSentenceInList->next;
	}

	this->performGIApostProcessing(translatorVariables);
	
	return result;
}



bool GIAtranslatorClass::convertSentenceRelationsIntoGIAnetworkNodesWrapper(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
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
		unordered_map<string, GIAentityNode*> sentenceNetworkIndexEntityNodesList;
		
		//backup current database structures and create temporary database structures for !linkPreestablishedReferencesGIA run;
		unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndexDBactiveOriginal;
		unordered_map<string, GIAentityNode*>* entityNodesActiveListCompleteFastIndexDBactiveTemp;
		#ifdef GIA_DATABASE
		if(GIAdatabase.getUseDatabase() != GIA_DATABASE_FALSE)
		{
			entityNodesActiveListCompleteFastIndexDBactiveOriginal = GIAdatabase.getDBentityNodesActiveListCompleteFastIndexDBactive();
			entityNodesActiveListCompleteFastIndexDBactiveTemp = new unordered_map<string, GIAentityNode*>;
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
		vector<GIAentityNode*> sentenceNetworkIndexEntityNodesListTemp;
		map<int, vector<GIAentityNode*>*> entityNodesActiveListSentencesTemp;
		translatorVariablesPrelim.sentenceNetworkIndexEntityNodesList = &sentenceNetworkIndexEntityNodesListTemp;
		translatorVariablesPrelim.entityNodesActiveListSentences = &entityNodesActiveListSentencesTemp;
		translatorVariablesPrelim.saveNetwork = false;

		#ifdef GIA_DATABASE
		GIAdatabase.setUseDatabase(GIA_DATABASE_FALSE);
		#endif
		


		#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS
		if(translatorVariables->parseGIA2file)
		{
			if(translatorVariables->currentSentenceInList->semanticParserSuccessful)
			{
				GIAsemanticParserTranslator.convertSentenceSemanticRelationsIntoGIAnetworkNodes(&translatorVariablesPrelim, false, NULL);
			}
		}
		#endif
		if(!translatorVariables->parseGIA2file || (translatorVariables->parseGIA2file && !(translatorVariables->currentSentenceInList->semanticParserSuccessful)))
		{
			translatorVariablesPrelim.linkPreestablishedReferencesGIA = false;
			this->convertSentenceSyntacticRelationsIntoGIAnetworkNodes(&translatorVariablesPrelim, NULL);
		}


		vector<GIAentityNode*> referenceSetDefiniteEntityList;
		int numberReferenceSets = GIAtranslatorDefineReferencing.identifyReferenceSets(&sentenceNetworkIndexEntityNodesList, translatorVariables->NLPdependencyRelationsType, &referenceSetDefiniteEntityList);	//NB NLPdependencyRelationsType is no longer used here
	
		#ifdef GIA_DATABASE
		GIAdatabase.setUseDatabase(useDatabaseOriginal);
		#endif


		GIAtranslatorDefineReferencing.createGIAcoreferenceInListBasedUponIdentifiedReferenceSets(&sentenceNetworkIndexEntityNodesList, translatorVariables->entityNodesActiveListNetworkIndexes, firstGIAcoreferenceInList, &referenceSetDefiniteEntityList, translatorVariables->currentSentenceInList);

		translatorVariables->saveNetwork = true;


		#ifdef GIA_DATABASE
		if(GIAdatabase.getUseDatabase() != GIA_DATABASE_FALSE)
		{
			GIAdatabase.setDBentityNodesActiveListCompleteFastIndexDBactive(entityNodesActiveListCompleteFastIndexDBactiveOriginal);
		}
		#endif

		vector<GIAentityNode*> sentenceNetworkIndexEntityNodesListTemp2;
		translatorVariables->sentenceNetworkIndexEntityNodesList = &sentenceNetworkIndexEntityNodesListTemp2;
			
		#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS
		if(translatorVariables->parseGIA2file)
		{
			if(translatorVariables->currentSentenceInList->semanticParserSuccessful)
			{
				GIAsemanticParserTranslator.convertSentenceSemanticRelationsIntoGIAnetworkNodes(translatorVariables, true, firstGIAcoreferenceInList);
			}
		}
		#endif
		if(!translatorVariables->parseGIA2file || (translatorVariables->parseGIA2file && !(translatorVariables->currentSentenceInList->semanticParserSuccessful)))
		{
			translatorVariables->linkPreestablishedReferencesGIA = true;
			this->convertSentenceSyntacticRelationsIntoGIAnetworkNodes(translatorVariables, firstGIAcoreferenceInList);
		}

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
		#ifdef GIA_SEMANTIC_PARSER_READ_SEMANTIC_RELATIONS
		if(translatorVariables->parseGIA2file)
		{
			if(translatorVariables->currentSentenceInList->semanticParserSuccessful)
			{
				GIAsemanticParserTranslator.convertSentenceSemanticRelationsIntoGIAnetworkNodes(translatorVariables, false, NULL);
			}
		}
		#endif
		if(!translatorVariables->parseGIA2file || (translatorVariables->parseGIA2file && !(translatorVariables->currentSentenceInList->semanticParserSuccessful)))
		{
			translatorVariables->linkPreestablishedReferencesGIA = false;
			this->convertSentenceSyntacticRelationsIntoGIAnetworkNodes(translatorVariables, NULL);
		}
	#ifdef GIA_ADVANCED_REFERENCING
	}
	#endif

	return result;
}

bool GIAtranslatorClass::convertSentenceSyntacticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, GIAcoreference* firstGIAcoreferenceInList)
{
	bool result = true;

	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = GIAsentenceClass.getMinIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList) + SENTENCE_FIRST_ARTIFICIAL_INDEX;

	GIArelation* currentRelationInList;

	#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	string corpusFileName = "";
	if(!translatorVariables->linkPreestablishedReferencesGIA)
	{
		//GIAsemanticParserOperations.determineGIAconnectionistNetworkPOStypeNames(currentSentenceInList->firstFeatureInList, NLPfeatureParser);
		GIAsemanticParserDatabase.prepareSemanticParserDatabaseForWriting();
	}
	#endif

	vector<bool> GIAentityNodeArrayFilled(GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList));		//NB could also use currentSentence->maxNumberOfWordsInSentence
	vector<GIAentityNode*> GIAfeatureTempEntityNodeArray(GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList));	//temporary array introduced 14 July 2012b, to handle grammatical information specific to instances of networkIndex entities (not networkIndex entities themselves) before those intances have been defined (in GIAtranslatorDefineSubstances.cpp)	
	vector<GIAentityNode*> GIAnetworkIndexNodeArray(GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList));
	vector<GIAentityNode*> GIAentityNodeArray(GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList));
	vector<GIAfeature*> featureArrayTemp(GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList));
	translatorVariables->GIAentityNodeArrayFilled = &GIAentityNodeArrayFilled;
	translatorVariables->GIAfeatureTempEntityNodeArray = &GIAfeatureTempEntityNodeArray;
	translatorVariables->GIAnetworkIndexNodeArray = &GIAnetworkIndexNodeArray;	
	translatorVariables->GIAentityNodeArray = &GIAentityNodeArray;
	translatorVariables->featureArrayTemp = &featureArrayTemp;
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		GIAentityNodeArrayFilled[w] = false;
		GIAfeatureTempEntityNodeArray[w] = NULL;	//added  14 July 2012b
		GIAnetworkIndexNodeArray[w] = NULL;		//added 10 May 2012
		GIAentityNodeArray[w] = NULL;		//added 10 May 2012
		featureArrayTemp[w] = NULL;		//added 14 July 2013
	}


	GIAtranslatorDefineGrammar.locateAndAddAllFeatureTempEntities(translatorVariables);

	GIAtranslatorOperations.generateTempFeatureArray(translatorVariables, translatorVariables->featureArrayTemp);	//regeneration required for Relex in case query variables detected


	#ifdef GIA_RELEX
	if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)		//updated 2d1a 21 Jan 2013, OLD: if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		GIAtranslatorDefineGrammar.fillGrammaticalArraysRelex(translatorVariables->currentSentenceInList);
	}
	#endif
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	else if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)	//NB stanford dependency relations are required [ie not just Stanford POS tags] such that det/aux information can be extracted	//updated 2d1a 21 Jan 2013 - changed 'if' to 'else if'
	{
		GIAtranslatorDefineGrammar.fillGrammaticalArraysStanford(translatorVariables);
	}
	#endif
	//need to transfer featureArrayTemp->feature->entityDisabled information into GIAentityNodeArray...

	
	//this function is first only executed in a temporary fashion (where its output is only used by relex GIAtranslatorDefineReferencing.linkPronounReferencesRelex())
	translatorVariables->GIAentityNodeArray = &GIAfeatureTempEntityNodeArray;	//required by applyGrammaticalInfoToAllEntities (first execution), redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs, GIAtranslatorRedistributeRelationsStanford/GIAtranslatorRedistributeRelationsRelex
 	GIAtranslatorDefineGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, translatorVariables->currentSentenceInList->firstFeatureInList);
	
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#ifndef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION
	/*
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{//to work effectively/best, this function requires a) GIA_NLP_PARSER_STANFORD_CORENLP as NLPfeatureParser
		#ifndef STANFORD_PARSER_USE_POS_TAGS
		//to work effectively/best, this function requires b) Stanford CoreNLP as pos tagger (ie it is incompatible with STANFORD_PARSER_USE_POS_TAGS, as stanford Parser tags a lot more -ing words as NN [rather than VBG] as compared to Stanford CoreNLP)
	*/
	GIAtranslatorRedistributeRelations.redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(translatorVariables);
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
		GIAtranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAtranslatorRedistributeRelationsStanford", translatorVariables);
		#endif
	}
	#endif
	#ifdef GIA_ALIASES_RELEX_COMPATIBILITY
	if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
	{
		#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
		GIAtranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAtranslatorRedistributeRelationsRelex", translatorVariables);
		#endif
	}
	#endif
	translatorVariables->GIAentityNodeArray = &GIAentityNodeArray;

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
	this->invertOrDuplicateConditionsIfRequired(translatorVariables);
	#endif

	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	GIAbot.botSwitchFirstAndSecondPerson(translatorVariables);
	#endif

	
	GIAtranslatorDefineGrammar.locateAndAddAllNetworkIndexEntities(translatorVariables);

	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	translatorVariables->GIAentityNodeArray = &GIAnetworkIndexNodeArray;	//required by createParentsOfSubclassEntities 
	this->createParentsOfSubclassEntities(translatorVariables);
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
		GIAtranslatorDefineReferencing.fillExplicitReferenceSameSetTags(translatorVariables->currentSentenceInList);
	//}
#endif
	
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		GIAentityNodeArray[w] = GIAnetworkIndexNodeArray[w];		//set default values of GIAentityNodeArray
	}

#ifndef GIA_ADVANCED_REFERENCING_DISABLE_LINKING
	#ifdef GIA_ADVANCED_REFERENCING
	if(translatorVariables->linkPreestablishedReferencesGIA)
	{
		GIAtranslatorDefineReferencing.linkAdvancedReferencesGIA(translatorVariables, firstGIAcoreferenceInList);
	}
	else
	{
	#endif
		#ifdef GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
		#endif
			GIAtranslatorDefineReferencing.linkPronounReferencesRelex(translatorVariables);
		#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		}
		#ifdef GIA_STANFORD_CORENLP
		else if(translatorVariables->NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			GIAtranslatorDefineReferencing.linkPronounAndTextualContextReferencesStanfordCoreNLP(translatorVariables, translatorVariables->firstSentenceInList->firstCoreferenceInList);
		}
		#endif
		#endif
		#endif
	#ifdef GIA_ADVANCED_REFERENCING
	}
	#endif
#endif

	//transfer disabled substances across execution#1 [this is required since GIAtranslatorRedistributeRelationsStanford operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of networkIndex entity nodes {whose values would have been automatically transferred to their instances upon creation}...]
	this->disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(translatorVariables);
			

	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_GENERIC_ENTITY_INTERPRETATION
 	//this is required as GIAtranslatorDefineSubstances.cpp now relies on entity grammar values rather than featureArrayTemp
	GIAtranslatorDefineGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, translatorVariables->currentSentenceInList->firstFeatureInList);
	#endif


	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	GIAtranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAtranslatorDefineSubstances", translatorVariables);
	#endif


	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES	// or GIA_GENERIC_ENTITY_INTERPRETATION
	//this function has been shifted, and applied to entity instances, not the networkIndex entity array...
 	GIAtranslatorDefineGrammar.applyGrammaticalInfoToAllEntities(translatorVariables, currentSentenceInList->firstFeatureInList);
	#endif

	GIAtranslatorDefineReferencing.identifyComparisonVariableAlternateMethod(translatorVariables);

	//transfer disabled substances across execution#2 [this is required since GIAtranslatorRedistributeRelationsStanford operations are now done on temporary entity nodes GIAfeatureTempEntityNodeArray instead of networkIndex entity nodes {whose values would have been automatically transferred to their instances upon creation}...]
	this->disableEntitiesBasedOnFeatureTempEntityNodeArray(translatorVariables);

	
	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	GIAtranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAtranslatorLinkEntities", translatorVariables);
	#endif
	
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	this->createAdditionalSubclassEntities(translatorVariables);
	#endif
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
	this->detectUserDeclaredSubclassEntities(translatorVariables);
	#endif
	#endif

	GIAtranslatorLinkEntitiesDynamic.linkEntitiesDynamic(translatorVariables);


	#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
	GIAtranslatorRules.applyGIATranslatorGenericXMLfunctions("GIAtranslatorApplyAdvancedFeatures", translatorVariables);
	#endif
	GIAtranslatorApplyAdvancedFeatures.applyAdvancedFeatures(translatorVariables);

	vector<GIAentityNode*>* entityNodesActiveListSentence = new vector<GIAentityNode*>;


	//record entityIndexTemp + sentenceIndexTemp for all substances in sentence (allows for referencing)...
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNode* entity = GIAentityNodeArray[w];
			
			entityNodesActiveListSentence->push_back(entity);

			#ifdef GIA_ADVANCED_REFERENCING

			#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
			if(GIAtranslatorOperations.checkIfSentenceIsMathTextParsablePhrase(translatorVariables->currentSentenceInList))
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
				if(featureArrayTemp[w] != NULL)
				{
					for(int w2=0; w2<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w2++)
					{
						if(featureArrayTemp[w2] != NULL)
						{
							if(featureArrayTemp[w2]->lemma == featureArrayTemp[w]->lemma)
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
	

	#ifdef GIA_CREATE_SHORTCUTS_TO_CONCEPT_ENTITIES
	createShortcutsToConceptEntities(translatorVariables);
	#endif
	
	translatorVariables->entityNodesActiveListSentences->insert(pair<int, vector<GIAentityNode*>*>(translatorVariables->currentSentenceInList->sentenceIndex, entityNodesActiveListSentence));

	#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	if(!translatorVariables->linkPreestablishedReferencesGIA)
	{
		GIAsemanticParserOperations.GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(translatorVariables);
	}
	#endif

	//GIAtranslatorOperations.recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(GIAentityNodeArrayFilled, GIAnetworkIndexNodeArray);		//this method is not sufficient, as some networkIndex entity nodes (eg prepositions/conditions) are not contained within GIAnetworkIndexNodeArray
	GIAtranslatorOperations.recordSentenceNetworkIndexNodesAsPermanentIfTheyAreStillEnabled(translatorVariables->entityNodesActiveListNetworkIndexes);

	
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	//required to reset wasReferenceTemp for next time
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNodeArray[w]->wasReferenceTemp = false;
		}
	}
	#endif

	#ifdef GIA_SEMANTIC_PARSER_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	if(!translatorVariables->linkPreestablishedReferencesGIA)
	{
		GIAsemanticParserOperations.determineGIAconnectionistNetworkPOStypeNames(translatorVariables->currentSentenceInList->firstFeatureInList, translatorVariables->NLPfeatureParser);
		#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
		#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
		GIAsemanticParserDatabase.writeSemanticParserCorpusFile(translatorVariables->currentSentenceInList->firstFeatureInList);
		#else
		string sentenceSemanticRelationsText = GIAsemanticParserOperations.generateSemanticParserCorpusSemanticRelationsText(GIAsemanticParserDatabase.getFirstRelationInSemanticParserSentenceList());
		GIAsemanticParserDatabase.writeSemanticParserCorpusFile(translatorVariables->currentSentenceInList->firstFeatureInList, &sentenceSemanticRelationsText);
		#endif
		#endif
		#ifdef GIA_SEMANTIC_PARSER_SUBSETS
		if(!GIAsemanticParserTranslator.generateAllPermutationsFromSemanticRelationsFile(translatorVariables))
		{
			cerr << "GIAsemanticParserTranslator.generateAllPermutationsFromSemanticRelationsFile() failed" << endl;
			exit(EXIT_ERROR);
		}
		#endif
	}
	#endif
	
	/*
	#ifdef GIA_PREPROCESSOR_SENTENCE
	#ifdef GIA_PREPROCESSOR_SENTENCE_RECONCILE_REFERENCES_AFTER_SEMANTIC_PARSING_EVERY_SENTENCE
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

	
void GIAtranslatorClass::disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
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
void GIAtranslatorClass::disableEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
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




#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
void GIAtranslatorClass::createParentsOfSubclassEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* subclassNetworkIndexEntity = (*translatorVariables->GIAnetworkIndexNodeArray)[w];
			if(subclassNetworkIndexEntity->convertToSubClass)
			{
				#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
				subclassNetworkIndexEntity->isSubClass = true;
				#endif

				string parentClassName = GIAentityNodeClass.getParentClassEntityNameFromSubClassEntityName(subclassNetworkIndexEntity->entityName);


				bool parentNetworkIndexEntityAlreadyExistant = false;
				GIAentityNode* parentClassNetworkIndexEntity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&parentClassName, &parentNetworkIndexEntityAlreadyExistant, translatorVariables);
				if(!parentNetworkIndexEntityAlreadyExistant)
				{
					//cout << "creating parentClassNetworkIndexEntity: " << parentClassName << endl;
					translatorVariables->sentenceNetworkIndexEntityNodesList->push_back(parentClassNetworkIndexEntity);

					parentClassNetworkIndexEntity->entityIndexTemp = GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX;	//there is no entity index associated with the artifically added subclass parent networkIndex
					parentClassNetworkIndexEntity->sentenceIndexTemp = subclassNetworkIndexEntity->sentenceIndexTemp;

					this->linkSubclassEntitiesWithParentClassEntities(subclassNetworkIndexEntity, parentClassNetworkIndexEntity, false, false, translatorVariables);
				}
				else
				{
					this->linkSubclassEntitiesWithParentClassEntities(subclassNetworkIndexEntity, parentClassNetworkIndexEntity, false, true, translatorVariables);
				}
			}
		}
	}
}

#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
void GIAtranslatorClass::createAdditionalSubclassEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	//eg The red bat is a xenomorph. -> create xenomorph_bat, alias link xenomorph concept with xenomorph_bat concept
		//OLD: eg The red bat is a xenomorph. -> create xenomorph_bat, definition link xenomorph_bat concept with xenomorph concept, and definitition link red bat to xenomorph_bat concept

	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[w];
			if(entity->addSubClass)
			{
				if(!(entity->entityType == GIA_ENTITY_TYPE_CONCEPT))	//assume true
				{
					GIAentityNode* definitionRelationshipObjectEntity = NULL;
					bool foundDefinitionEntity = false;
					for(vector<GIAentityConnection*>::iterator definitionNodeListIterator = entity->definitionNodeList->begin(); definitionNodeListIterator < entity->definitionNodeList->end(); definitionNodeListIterator++)
					{
						GIAentityNode* definitionEntityTemp = (*definitionNodeListIterator)->entity;
						if(definitionEntityTemp->entityType == GIA_ENTITY_TYPE_CONCEPT)	//assume true
						{
							#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
							if((*definitionNodeListIterator)->sentenceIndexTemp = translatorVariables->sentenceIndex)
							{//in the case that a substance has been assigned multiple definitions in the text, eg "The animal is a dog. The animal is an alsatian", in which case "The animal" entity node will be connected to both dog and alsation (due to GIA advanced referencing).
							#else
							//NB dream mode has not yet been executed so all concepts connected to this entity will have been defined in the same sentence
							#endif
								foundDefinitionEntity = true;
								definitionRelationshipObjectEntity = definitionEntityTemp;	//eg xenomorph
							#ifndef GIA_DISABLE_CROSS_SENTENCE_REFERENCING
							}
							#endif
						}
					}

					if(foundDefinitionEntity)
					{

						string subClassEntityName = GIAentityNodeClass.createSubClassEntityName(definitionRelationshipObjectEntity->entityName, entity->entityName);	//eg apple_fruit / xenomorph_bat
						//cout << "subClassEntityName = " << subClassEntityName << endl;
						bool subClassNetworkIndexEntityAlreadyExistant = false;
						GIAentityNode* subClassNetworkIndexEntity = GIAtranslatorOperations.findOrAddNetworkIndexEntityNodeByNameSimpleWrapper(&subClassEntityName, &subClassNetworkIndexEntityAlreadyExistant, translatorVariables);
						if(!subClassNetworkIndexEntityAlreadyExistant)
						{
							#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
							subClassNetworkIndexEntity->isSubClass = true;
							#endif

							translatorVariables->sentenceNetworkIndexEntityNodesList->push_back(subClassNetworkIndexEntity);

							subClassNetworkIndexEntity->entityIndexTemp = GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ARTIFICAL_ENTITY_INDEX;	//there is no entity index associated with the artifically added subclass parent networkIndex
							subClassNetworkIndexEntity->sentenceIndexTemp = entity->sentenceIndexTemp;

							GIAentityNode* definitionNetworkIndexEntity = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance(definitionRelationshipObjectEntity);
							this->linkSubclassEntitiesWithParentClassEntities(definitionNetworkIndexEntity, subClassNetworkIndexEntity, false, false, translatorVariables);	//links xenomorph concept with xenomorph_bat concept

							/* //changed 2m7a
							bool sameReferenceSet = false;	//this is required for dreamModeLinkSpecificConceptsAndActions
							#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
							//now create a definition link between the entity and the subClass concept (since this will not be created by GIA dream mode)
							GIAentityNode* subclassNonspecificConcept = getNonspecificConceptEntityFromNetworkIndex(subClassNetworkIndexEntity);
							connectDefinitionToEntity(entity, subclassNonspecificConcept, sameReferenceSet);	//links red bat to xenomorph_bat concept
							//#ifdef GIA_DEBUG
							cout << "createAdditionalSubclassEntities{}: linking entity: " << entity->entityName << " to " << subclassNonspecificConcept->entityName << endl;
							//#endif
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

void GIAtranslatorClass::linkSubclassEntitiesWithParentClassEntities(GIAentityNode* subclassNetworkIndexEntity, GIAentityNode* parentClassNetworkIndexEntity, const bool linkAsAlias, const bool onlyLinkIfSubclassOrParentConceptNotFound, GIAtranslatorVariablesClass* translatorVariables)
{
	string parentClassName = parentClassNetworkIndexEntity->entityName;

	bool subclassToParentEntityConnectionAlreadyMade = false;
	GIAentityNode* subclassNonspecificConcept = GIAtranslatorOperations.getNonspecificConceptEntityFromNetworkIndex(subclassNetworkIndexEntity);
	bool subclassConceptFound = false;
	if(subclassNonspecificConcept != NULL)
	{
		//cout << "subclassNonspecificConcept found" << endl;
		subclassConceptFound = true;
		for(vector<GIAentityConnection*>::iterator definitionNodeListIterator = subclassNonspecificConcept->definitionNodeList->begin(); definitionNodeListIterator < subclassNonspecificConcept->definitionNodeList->end(); definitionNodeListIterator++)
		{
			GIAentityNode* definitionRelationshipObjectEntity = (*definitionNodeListIterator)->entity;
			if(definitionRelationshipObjectEntity->entityType == GIA_ENTITY_TYPE_CONCEPT)	//assume true
			{
				if(definitionRelationshipObjectEntity->entityName == parentClassName)
				{
					subclassToParentEntityConnectionAlreadyMade = true;
				}
			}
		}
	}
	else
	{
		subclassNonspecificConcept = GIAtranslatorOperations.createNewNonspecificConcept(subclassNetworkIndexEntity, translatorVariables);
		#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
		subclassNonspecificConcept->isSubClass = true;
		#endif
	}

	if(!subclassToParentEntityConnectionAlreadyMade)
	{
		//should a new subclassNonspecificConcept be defined here regardless; ie GIAentityNode* subclassNonspecificConcept = GIAtranslatorOperations.createNewNonspecificConcept(subclassNetworkIndexEntity)? (because new concepts are created every sentence, even for non-specific concepts; eg "Pies are apples. Pies are berries.")

		bool performLink = false;

		GIAentityNode* parentClassNonspecificConcept = GIAtranslatorOperations.getNonspecificConceptEntityFromNetworkIndex(parentClassNetworkIndexEntity);
		if(parentClassNonspecificConcept == NULL)
		{
			performLink = true;
			parentClassNonspecificConcept = GIAtranslatorOperations.createNewNonspecificConcept(parentClassNetworkIndexEntity, translatorVariables);
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
			bool sameReferenceSet = false;	//this is required for dreamModeLinkSpecificConceptsAndActions
			
			GIAentityNode* definitionRelationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationship2(RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS, translatorVariables, true);
			//GIAentityNode* definitionRelationshipEntity = GIAtranslatorOperations.findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition(translatorVariables);	//use findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition2 (ie findOrAddEntityNodeByNameSimpleWrapperRelationship2) instead of findOrAddEntityNodeByNameSimpleWrapperRelationshipArtificialDefinition [as GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES doesn't fill in GIAentityNodeArrays at this stage of development, and would need to update GIAfeatureTempEntityNodeArray otherwise; not just GIAentityNodeArray]
			
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			if(linkAsAlias)
			{
				GIAtranslatorOperations.connectDefinitionToEntityMarkConnectionAsAlias(subclassNonspecificConcept, parentClassNonspecificConcept, definitionRelationshipEntity, sameReferenceSet, translatorVariables);
			}
			else
			{
			#endif
				GIAtranslatorOperations.connectDefinitionToEntity(subclassNonspecificConcept, parentClassNonspecificConcept, definitionRelationshipEntity, sameReferenceSet, translatorVariables);
			#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
			}
			#endif
		}
	}
}

#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
void GIAtranslatorClass::detectUserDeclaredSubclassEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			GIAentityNode* entity = (*translatorVariables->GIAentityNodeArray)[w];
			if(entity->entityType == GIA_ENTITY_TYPE_SUBSTANCE)	//ie !isAction && !isCondition (prevent multiword prepositions [and actions, in case multiword actions are implemented in the future])
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


bool GIAtranslatorClass::performGIApostProcessing(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	#ifdef GIA_PREPROCESSOR_SENTENCE
	#ifndef GIA_PREPROCESSOR_SENTENCE_RECONCILE_REFERENCES_AFTER_SEMANTIC_PARSING_EVERY_SENTENCE
	if(!GIApreprocessor.connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesWrapper(translatorVariables->firstGIApreprocessorSentenceInList, translatorVariables))	//check the position of this function's execution
	{
		result = false;
	}
	#endif
	#endif

	#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
	this->dreamModeLinkSpecificConceptsAndActions(translatorVariables);
	#endif
		
	return result;
}

#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
void GIAtranslatorClass::dreamModeLinkSpecificConceptsAndActions(GIAtranslatorVariablesClass* translatorVariables)
{
	GIAtranslatorDefineReferencing.identifyReferenceSetsSpecificConceptsAndLinkWithConcepts(translatorVariables);

	#ifdef GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES
	this->createAndLinkNonSpecificConceptsForAllEntities(translatorVariables);
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
			GIAtranslatorOperations.connectDefinitionToEntity(entity, nonSpecificConceptEntity, sameReferenceSet);
		}
	}
}
#endif

#endif

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
void GIAtranslatorClass::invertOrDuplicateConditionsIfRequired(GIAtranslatorVariablesClass* translatorVariables)
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
						GIApreprocessorMultiwordReduction.detectIfInverseOrTwoWayConditionRequired(conditionName, &inverseConditionRequired, &twoWayConditionRequired, &inverseConditionName);

						#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
						if(inverseConditionRequired)
						{
							this->createNewInverseConditionEntity(currentRelationInList, translatorVariables, inverseConditionName);

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
						#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS
						if(twoWayConditionRequired)
						{
							#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
							GIArelation* lastRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;
							while(lastRelationInList->next != NULL)
							{
								lastRelationInList = lastRelationInList->next;
							}
							this->createNewInverseConditionEntity(lastRelationInList, translatorVariables, conditionName);
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

void GIAtranslatorClass::createNewInverseConditionEntity(GIArelation* currentRelationInList, GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName)
{
	int inverseConditionEntityIndex = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent;
	translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent = translatorVariables->currentSentenceInList->relationshipEntityArtificialIndexCurrent + 1;

	(*translatorVariables->GIAentityNodeArrayFilled)[inverseConditionEntityIndex] = true;
	GIAentityNode* inverseConditionEntity = new GIAentityNode();
	inverseConditionEntity->sentenceIndexTemp = translatorVariables->sentenceIndex;
	inverseConditionEntity->entityName = inverseConditionName;
	inverseConditionEntity->wordOrig = inverseConditionName;	//is this necessary?
	//why not set inverseConditionEntity->entityIndexTemp and inverseConditionEntity->sentenceIndexTemp?
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
	currentRelationInList->relationTypeNonInversed = currentRelationInList->relationType;
	currentRelationInList->relationTypeIndexNonInversed = currentRelationInList->relationTypeIndex;
	#endif
	currentRelationInList->relationType = string(STANFORD_PARSER_PREPOSITION_PREPEND) + inverseConditionName;
	currentRelationInList->relationTypeIndex = inverseConditionEntityIndex;
	currentRelationInList->inverseRelation = true;
	(*translatorVariables->GIAfeatureTempEntityNodeArray)[inverseConditionEntityIndex] = inverseConditionEntity;

	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex] = new GIAfeature();
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->word = inverseConditionName;
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->lemma = inverseConditionName;	//is this necessary?
	(*translatorVariables->featureArrayTemp)[inverseConditionEntityIndex]->entityIndex = inverseConditionEntityIndex;
}


#endif



#ifdef GIA_CREATE_SHORTCUTS_TO_CONCEPT_ENTITIES
void GIAtranslatorClass::createShortcutsToConceptEntities(GIAtranslatorVariablesClass* translatorVariables)
{
	for(int w=0; w<GIAsentenceClass.getMaxIndexOfDynamicallyGeneratedEntity(translatorVariables->currentSentenceInList); w++)
	{
		if((*translatorVariables->GIAentityNodeArrayFilled)[w])
		{
			if((*translatorVariables->GIAentityNodeArray)[w]->entityType == GIA_ENTITY_TYPE_CONCEPT)
			{
				GIAentityNode* conceptNode = (*translatorVariables->GIAentityNodeArray)[w];
				bool specificConcept = true;
				if(hasSameReferenceSetConnections(conceptNode, translatorVariables))
				{
					specificConcept = false;
				}
				
				GIAentityNode* networkIndexNode = GIAtranslatorOperations.getPrimaryNetworkIndexNodeDefiningInstance((*translatorVariables->GIAentityNodeArray)[w]);
				for(vector<GIAentityConnection*>::iterator connectionIter = networkIndexNode->instanceNodeList->begin(); connectionIter != networkIndexNode->instanceNodeList->end(); connectionIter++)
				{
					GIAentityNode* instanceNode = (*connectionIter)->entity;
					if(instanceNode->entityType != GIA_ENTITY_TYPE_CONCEPT)
					{
						if(specificConcept)
						{
							bool alreadyAddedToSpecificConceptList = false;
							for(vector<GIAentityNode*>::iterator iter2 = instanceNode->shortcutsToSpecificConceptEntities.begin(); iter2 != instanceNode->shortcutsToSpecificConceptEntities.end(); iter2++)
							{
								GIAentityNode* specificConcept2 = (*iter2);
								if(specificConcept2 == conceptNode)
								{
									alreadyAddedToSpecificConceptList = true;
								}
							}
							if(!alreadyAddedToSpecificConceptList)
							{
								instanceNode->shortcutsToSpecificConceptEntities.push_back(conceptNode);
							}
						}
						else
						{
							bool alreadyAddedToNonspecificConceptList = false;
							if(instanceNode->shortcutToNonspecificConceptEntity == conceptNode)
							{
								alreadyAddedToNonspecificConceptList = true;
							}
							if(!alreadyAddedToNonspecificConceptList)
							{
								instanceNode->shortcutToNonspecificConceptEntity = conceptNode;
							}
						}
					}
				}
			}
		}
	}
}

bool GIAtranslatorClass::hasSameReferenceSetConnections(GIAentityNode* entity, GIAtranslatorVariablesClass* translatorVariables)
{
	bool sameReferenceSetConnection = false;
	for(int connectionType = 0; connectionType<GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES; connectionType++)
	{
		for(vector<GIAentityConnection*>::iterator connectionIter = entity->entityVectorConnectionsArray[connectionType].begin(); connectionIter != entity->entityVectorConnectionsArray[connectionType].end(); connectionIter++)
		{
			GIAentityConnection* connection = (*connectionIter);
			if(connection->sentenceIndexTemp == entity->sentenceIndexTemp)	//REDUNDANT
			{		
				if(connection->sameReferenceSet)
				{
					sameReferenceSetConnection = true;
				}
			}
		}
	}
	return sameReferenceSetConnection;
}			
#endif


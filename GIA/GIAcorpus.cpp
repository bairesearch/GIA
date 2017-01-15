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
 * File Name: GIAcorpus.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2b3c 31-December-2013
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/





#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "GIAcorpus.h"
#include "GIAcorpusOperations.h"
#include "GIAcorpusDatabase.h"
#include "GIAnlp.h"
#include "GIAtranslator.h"

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_USE_CORPUS_DATABASE

bool performCorpusLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(Paragraph * firstParagraphInList, string inputPlainTXTFileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *entityNodesActiveListSubstances, vector<GIAentityNode*> *entityNodesActiveListActions, vector<GIAentityNode*> *entityNodesActiveListConditions, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int * maxNumberSentences)
{
	bool parseGIA2file = true;
	bool NLPrelexCompatibilityMode = false; 		//irrelevant (not used) - only used when parsing syntatic dependency relations of a Relex file, and performCorpusLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences() does not parse any syntactic dependency relations
	bool NLPassumePreCollapsedStanfordRelations = false;	//irrelevant (not used) - only used when executing convertSentenceSyntacticRelationsIntoGIAnetworkNodes(), and performCorpusLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences() does not execute convertSentenceSyntacticRelationsIntoGIAnetworkNodes()
	
	bool result = true;
	
	#ifdef STANFORD_PARSER_USE_POS_TAGS
	cout << "error: performCorpusLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences() doesn't support STANFORD_PARSER_USE_POS_TAGS" << endl;
	#endif
	bool createNewSentences = true;
	if(!parseNLPparserFeaturesFile(inputTextNLPfeatureXMLfileName, isQuery, firstParagraphInList, NLPfeatureParser, &createNewSentences))
	{
		result = false;
	}
	
	#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		if(!lookupCorpusFiles(firstParagraphInList, NLPfeatureParser))	//finds all corresponding GIA2 semantic dependency files and parses them
		{	
			cout << "error: GIA2 corpus entry not found (sentences cannot be parsed)" << endl;
			result = false;
		}
	}
	else
	{
	#endif
		if(!lookupCorpusFiles(firstParagraphInList->firstSentenceInList, NLPfeatureParser))	//finds all corresponding GIA2 semantic dependency files and parses them
		{	
			cout << "error: GIA2 corpus entry not found (sentences cannot be parsed)" << endl;
			result = false;
		}
	#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
	}
	#endif	

	
	string inputTextNLPrelationXMLfileName = "";				//irrelevant (not used)
	int NLPdependencyRelationsParser = GIA_NLP_PARSER_STANFORD_PARSER;	//irrelevant (not used) - always set to Stanford Parser (as a standard parser type file is used to store GIA2 semantic dependency relations)
	if(!parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(firstParagraphInList, inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, outputCFFfileName, NLPexeFolderArray, entityNodesActiveListComplete, entityNodesActiveListConcepts, entityNodesActiveListSubstances, entityNodesActiveListActions, entityNodesActiveListConditions, timeConditionNodesActiveList, isQuery, NLPfeatureParser, NLPdependencyRelationsParser, NLPrelexCompatibilityMode, NLPassumePreCollapsedStanfordRelations, maxNumberSentences, true))	//inputTextNLPrelationXMLfileName/inputTextNLPfeatureXMLfileName/NLPfeatureParser/NLPdependencyRelationsParser/NLPrelexCompatibilityMode/NLPassumePreCollapsedStanfordRelations not used (relations and features have already been parsed)
	{
		result = false;
	}
		
}

bool lookupCorpusFiles(Paragraph * firstParagraphInList, int NLPfeatureParser)
{
	bool result = true;
	Paragraph * currentParagraphInList = firstParagraphInList;
	while(currentParagraphInList->next != NULL)
	{
		Sentence * firstSentenceInList = currentParagraphInList->firstSentenceInList;
		if(!lookupCorpusFiles(firstSentenceInList, NLPfeatureParser))
		{
			result = false;
		}
		currentParagraphInList = currentParagraphInList->next;
	}
	return result;
}

bool lookupCorpusFiles(Sentence * firstSentenceInList, int NLPfeatureParser)
{
	bool result = true;
	Sentence * currentSentenceInList = firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		//cout << "here" << endl;
		determineGIAconnectionistNetworkPOStypeNames(currentSentenceInList->firstFeatureInList, NLPfeatureParser);
		//cout << "here2" << endl;
		if(!loadCorpusFileSemanticDependencyRelations(currentSentenceInList))
		{
			result = false;
		}
		//cout << "here3" << endl;
		currentSentenceInList = currentSentenceInList->next;
	}
	return result;
}

#endif




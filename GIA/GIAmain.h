/*******************************************************************************
 *
 * File Name: GIAmain.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1l4f 03-June-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_MAIN
#define HEADER_GIA_MAIN

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;

#include "GIAglobalDefs.h"
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAEntityConnectionClass.h"
#ifdef USE_CE
	#include "CEClaimClass.h"
#endif


#ifndef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
	#define GIA_COMPILE_PRINT_INEXACT_ANSWER_AMBIENT_CONTEXT
#endif

//#define GIA_XML_DEBUG_TEST_WRITE_READ_WRITE



int main(int argc,char **argv);

#ifdef USE_CE	
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, string outputCFFFileName, string NLPexeFolderArray[], vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListProperties, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, bool readFromDatabase, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList);
#else
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, string outputCFFFileName, string NLPexeFolderArray[], vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListProperties, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, bool readFromDatabase);
#endif
	#ifdef USE_CE	
	bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListProperties, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPassumePreCollapsedStanfordRelations, bool readFromDatabase, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList);
	#else
	bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListProperties, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPassumePreCollapsedStanfordRelations, bool readFromDatabase);
	#endif
#endif

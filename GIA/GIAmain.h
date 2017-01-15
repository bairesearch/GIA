/*******************************************************************************
 *
 * File Name: GIAmain.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i16a 18-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_MAIN
#define HEADER_GIA_MAIN

#include "GIAglobalDefs.h"
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#ifdef USE_CE
	#include "CEClaimClass.h"
#endif

#define GIA_TM_STRUCT_YEAR_OFFSET 1900

#ifndef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
	#define GIA_COMPILE_PRINT_INEXACT_ANSWER_AMBIENT_CONTEXT
#endif

//#define GIA_XML_DEBUG_TEST_WRITE_READ_WRITE



int main(int argc,char **argv);

#ifdef USE_CE	
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList);
#else
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode);
#endif
	#ifdef USE_CE	
	bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList);
	#else
	bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode);
	#endif
#endif

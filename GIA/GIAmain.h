/*******************************************************************************
 *
 * File Name: GIAmain.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i6a 4-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_MAIN
#define HEADER_GIA_MAIN

#include "GIAglobalDefs.h"
#include "GIAEntityNodeClass.h"
#ifdef USE_CE
	#include "CEClaimClass.h"
#endif


#define CFF_XML_TAG_nlparse ((string)"nlparse")
#define CFF_XML_TAG_paragraph ((string)"paragraph")
#define CFF_XML_TAG_sentence ((string)"sentence")
#define CFF_XML_TAG_parse ((string)"parse")
#define CFF_XML_TAG_relations ((string)"relations")
#define CFF_XML_TAG_features ((string)"features")

#define GIA_TM_STRUCT_YEAR_OFFSET 1900

#ifndef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
	#define GIA_COMPILE_PRINT_INEXACT_ANSWER_AMBIENT_CONTEXT
#endif

//#define GIA_XML_DEBUG_TEST_WRITE_READ_WRITE



int main(int argc,char **argv);
bool parseNLPParserFile(string inputTextNLPParsedXMLFileName, bool isQuery, Paragraph * firstParagraphInList);
#ifdef GIA_USE_RELEX
bool parseRelexFile(string inputTextNLPParserXMLFileName, bool isQuery, Paragraph * firstParagraphInList);
#elif defined GIA_USE_STANFORD_PARSER
bool parseStanfordParserFile(string inputTextNLPParserXMLFileName, bool isQuery, Paragraph * firstParagraphInList);
#elif defined GIA_USE_STANFORD_CORENLP
bool parseStanfordCoreNLPFile(string inputTextNLPParserXMLFileName, bool isQuery, Paragraph * firstParagraphInList);
#endif

void executeNLPparser(string inputTextPlainTXTFileName, string inputTextNLPParsedXMLFileName);
#ifdef GIA_USE_RELEX
void executeRelex(string inputPlainTXTFileName, string inputNLPParserXMLFileName);
#elif defined GIA_USE_STANFORD_PARSER
bool executeStanfordParser(string inputTextNLPParserXMLFileName, bool isQuery, Paragraph * firstParagraphInList);
#elif defined GIA_USE_STANFORD_CORENLP
bool executeStanfordCoreNLP(string inputTextNLPParserXMLFileName, bool isQuery, Paragraph * firstParagraphInList);
#endif

#ifdef USE_CE	
bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList);
#else
bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery);
#endif

#ifdef USE_CE	
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPParsedXMLFileName, bool isQuery, Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList);
#else
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPParsedXMLFileName, bool isQuery, Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery);
#endif


#endif

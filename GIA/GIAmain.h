/*******************************************************************************
 *
 * File Name: GIAmain.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i4a 3-Apr-2012
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_MAIN
#define HEADER_GIA_MAIN

#include "GIAglobalDefs.h"
#include "GIAEntityNodeClass.h"
#ifdef USE_CE
	#include "CEClaimClass.h"
#endif


#define GIA_DEBUG_PRINT_QUERY_SEMANTIC_NETWORK

#define CFF_XML_TAG_nlparse ((string)"nlparse")
#define CFF_XML_TAG_paragraph ((string)"paragraph")
#define CFF_XML_TAG_sentence ((string)"sentence")
#define CFF_XML_TAG_parse ((string)"parse")
#define CFF_XML_TAG_relations ((string)"relations")
#define CFF_XML_TAG_features ((string)"features")

#define GIA_TM_STRUCT_YEAR_OFFSET 1900

#define GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
//#define GIA_DO_NOT_PRINT_RESULTS

#ifndef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
	#define GIA_COMPILE_PRINT_INEXACT_ANSWER_AMBIENT_CONTEXT
#endif

//#define GIA_XML_DEBUG_TEST_WRITE_READ_WRITE

#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.sh"
//OLD: #define GIA_RELEX_EXECUTABLE_RELATIVE_PATH_NAME "/relex-1.3.0/"	//gets added to executable path name

int main(int argc,char **argv);
#ifdef USE_CE	
bool parseRelexFile(string inputTextRelexXMLFileName, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery, CEClaim * firstClaimInHeirachy, vector<CEClaim*> * claimsList);
#else
bool parseRelexFile(string inputTextRelexXMLFileName, vector<GIAEntityNode*> *entityNodesCompleteList, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIAEntityNode*> *propertyEntityNodesList, vector<GIAEntityNode*> *actionEntityNodesList, vector<GIAEntityNode*> *conditionEntityNodesList, vector<GIATimeConditionNode*> * timeConditionNodesList, vector<long> * timeConditionNumbersList, bool isQuery);
#endif
void executeRelex(string inputPlainTXTFileName, string inputRelexXMLFileName);


#endif

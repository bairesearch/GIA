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
 * File Name: GIAmain.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1r5a 13-November-2012
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
	#include "CECodeextensionClass.h"
#endif

#define GIA_MAXIMUM_NUMBER_OF_FILES_IN_INPUT_FILE_LIST (1000000)
//#define GIA_XML_DEBUG_TEST_WRITE_READ_WRITE



int main(int argc,char **argv);

#ifdef USE_CE
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, string outputCFFFileName, string NLPexeFolderArray[], vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, bool useCodeextensionsHeirachy);
#else
bool parseNLPParserFileAndCreateSemanticNetworkBasedUponDependencyGrammarParsedSentences(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, string outputCFFFileName, string NLPexeFolderArray[], vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations);
#endif
	#ifdef USE_CE
	bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPassumePreCollapsedStanfordRelations, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, bool useCodeextensionsHeirachy);
	#else
	bool createSemanticNetworkBasedUponDependencyGrammarParsedSentences(Paragraph * firstParagraphInList, vector<GIAEntityNode*> *entityNodesActiveListComplete, unordered_map<string, GIAEntityNode*> *entityNodesActiveListConcepts, vector<GIAEntityNode*> *entityNodesActiveListSubstances, vector<GIAEntityNode*> *entityNodesActiveListActions, vector<GIAEntityNode*> *entityNodesActiveListConditions, unordered_map<long, GIATimeConditionNode*> *timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPassumePreCollapsedStanfordRelations);
	#endif

bool fileExists(string * fileName);

#ifdef GIA_SUPPORT_INPUT_FILE_LISTS
int getFilesFromFileList(string inputListFileName, string * inputFileNameArray);	
#endif

#endif

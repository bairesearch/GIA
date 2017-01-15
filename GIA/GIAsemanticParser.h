/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is commercially licensed software. It may not be redistributed,
 * and may only be used by a registered licensee.
 *
 * BAIPROJECT is sold in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * BAI Commercial License for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You will have received a copy of the BAI Commercial License
 * along with BAIPROJECT. For more information, visit <http://www.baxterai.com>
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemanticParser.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2k8b 06-September-2015
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER
#define HEADER_GIA_SEMANTIC_PARSER

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"

#ifdef USE_GIA2
bool performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAparagraph* firstParagraphInList, string inputPlainTXTFileName, string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, string outputCFFfileName, string NLPexeFolderArray[], vector<GIAentityNode*>* entityNodesActiveListComplete, unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* entityNodesActiveListSubstances, vector<GIAentityNode*>* entityNodesActiveListActions, vector<GIAentityNode*>* entityNodesActiveListConditions, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences, unordered_map<long, GIAtimeConditionNode*>* timeConditionNodesActiveList, bool isQuery, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool NLPassumePreCollapsedStanfordRelations, int* maxNumberSentences);
	bool lookupSemanticParserFiles(GIAparagraph* firstParagraphInList, int NLPfeatureParser);
		bool lookupSemanticParserFiles(GIAsentence* firstSentenceInList, int NLPfeatureParser);
#endif

#endif

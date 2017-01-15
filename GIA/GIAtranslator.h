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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1r10c 28-November-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR
#define HEADER_GIA_TRANSLATOR




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
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#ifdef USE_CE
	#include "CECodeextensionClass.h"
#endif


void initialiseGIATranslatorForTexualContext();
#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
void convertParagraphSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Paragraph * firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
#endif
	#ifdef USE_CE
	void convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, CECodeextension * firstCodeextensionInHeirachy, vector<CECodeextension*> * codeextensionsList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
		void convertCEsentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
	#endif
	void convertSentenceListRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
		void convertSentenceRelationsIntoGIAnetworkNodesWrapper(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
			#ifdef GIA_USE_ADVANCED_REFERENCING
			void convertSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations, bool linkPreestablishedReferencesGIA,  GIACoreference * firstGIACoreferenceInList);
			#else
			void convertSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, unordered_map<long, GIAtimeConditionNode*> *timeConditionNodesActiveList, Sentence * firstSentenceInList, Sentence * currentSentenceInList, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPfeatureParser, int NLPdependencyRelationsType, bool NLPassumePreCollapsedStanfordRelations);
			#endif
				void disableConceptEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], GIAentityNode * GIAfeatureTempEntityNodeArray[]);
				void disableEntitiesBasedOnFeatureTempEntityNodeArray(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], GIAentityNode * GIAfeatureTempEntityNodeArray[]);		//added 15 July 2012


	//?substance to substance relationship - these they in actual fact represent different levels of detail in information to substance to action / action to action nodes - direct substance to substance relationships are missing the action/connectivity information


#endif
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
 * File Name: GIAtranslatorDefineGrammar.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1s10d 05-July-2013
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR
#define HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR

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

#define GRAMMATICAL_DETERMINER_DEFINITE "the"
#define GRAMMATICAL_DETERMINER_INDEFINITE "a"
#define GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL "some"			//required for nlg
#define GRAMMATICAL_DETERMINER_INDEFINITE_FIRST_LETTER_VOWEL "an"	//required for nlg

//#define GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILLARY_TO_SET_TENSE_OF_VERB	//this seems theoretically possible pased upon the examples given (ie because the tense stanfordPOS information is replicated in both the auxillary and the verb)



//Pass A
void locateAndAddAllFeatureTempEntities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAfeatureTempEntityNodeArray[], int NLPdependencyRelationsType, int NLPfeatureParser);
void locateAndAddAllConceptEntities(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPdependencyRelationsType, int NLPfeatureParser, GIAentityNode * GIAfeatureTempEntityNodeArray[]);

#ifdef GIA_USE_RELEX
void fillGrammaticalArraysRelex(Sentence * currentSentenceInList);
#endif
//uses dependency relations to derive grammar [uses stanfordPOS/NER information to derive grammar, if NLPfeatureParser == Stanford Core NLP]
#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void fillGrammaticalArraysStanford(Sentence * currentSentenceInList,  bool GIAentityNodeArrayFilled[], GIAentityNode * GIAfeatureTempEntityNodeArray[], int NLPfeatureParser, Feature * featureArrayTemp[]);
#endif
	#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
	void extractPastTenseFromPOStag(string * POStag, Feature * feature);
	void extractGrammaticalInformationFromPOStag(string * POStag, Feature * feature);
	void extractGrammaticalInformationStanford(Feature * firstFeatureInList, int NLPfeatureParser);
		void extractPOSrelatedGrammaticalInformationStanford(Feature * currentFeature);
	void extractPastTense(Feature * featureWithEntityIndex, int entityIndexContainingTenseIndication, Feature * firstFeatureInList, int NLPfeatureParser);
	#endif

void applyGrammaticalInfoToAllEntities(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * firstFeatureInSentence);
	void applyPOSrelatedGrammaticalInfoToEntity(GIAentityNode * entity, Feature * currentFeatureInList);

#ifdef GIA_USE_ADVANCED_REFERENCING
void findSubjObjRelationMatchingAuxillaryAndSetNotSameReferenceSet(Sentence * currentSentenceInList, int subjectObjectEntityWithAuxillaryEntityIndex, string * subjectObjectEntityWithAuxillaryEntityName);
#endif

#endif

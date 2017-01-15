/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: GIAtranslatorDefineGrammar.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2k8b 06-September-2015
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR
#define HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAtranslatorOperations.h"

//#define GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILIARY_TO_SET_TENSE_OF_VERB	//this seems theoretically possible pased upon the examples given (ie because the tense stanfordPOS information is replicated in both the auxiliary and the verb)



//Pass A
void locateAndAddAllFeatureTempEntities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAfeatureTempEntityNodeArray[], int NLPdependencyRelationsType);
void locateAndAddAllConceptEntities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListConcepts, vector<GIAentityNode*>* sentenceConceptEntityNodesList, int NLPdependencyRelationsType, GIAentityNode* GIAfeatureTempEntityNodeArray[]);

#ifdef GIA_USE_RELEX
void fillGrammaticalArraysRelex(GIAsentence* currentSentenceInList);
#endif
//uses dependency relations to derive grammar [uses stanfordPOS/NER information to derive grammar, if NLPfeatureParser == Stanford Core NLP]
#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void fillGrammaticalArraysStanford(GIAsentence* currentSentenceInList,  bool GIAentityNodeArrayFilled[], GIAentityNode* GIAfeatureTempEntityNodeArray[], int NLPfeatureParser, GIAfeature* featureArrayTemp[]);
	void extractPastTense(GIAfeature* featureWithEntityIndex, int entityIndexContainingTenseIndication, GIAfeature* firstFeatureInList, int NLPfeatureParser);
		void extractPastTenseFromPOStag(string* POStag, GIAfeature* feature);
	void extractGrammaticalInformationStanford(GIAfeature* firstFeatureInList, int NLPfeatureParser);
		void extractPOSrelatedGrammaticalInformationStanford(GIAfeature* currentFeature);
			void extractGrammaticalInformationFromPOStag(string* POStag, GIAfeature* feature);
#endif

void applyGrammaticalInfoToAllEntities(bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* firstFeatureInSentence);
	void applyPOSrelatedGrammaticalInfoToEntity(GIAentityNode* entity, GIAfeature* currentFeatureInList);

#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
void findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet(GIAsentence* currentSentenceInList, int subjectObjectEntityWithAuxiliaryEntityIndex, string* subjectObjectEntityWithAuxiliaryEntityName);
#endif

#endif

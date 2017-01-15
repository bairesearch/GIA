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
 * File Name: GIAtranslatorApplyAdvancedFeatures.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2e6a 08-June-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_APPLY_ADVANCED_FEATURES
#define HEADER_GIA_TRANSLATOR_APPLY_ADVANCED_FEATURES

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
#include "GIAtranslatorOperations.h"

void applyAdvancedFeatures(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType, int NLPfeatureParser);
	void extractDates(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser);					//Stanford Compatible
		#ifdef GIA_USE_RELEX
		void extractDatesRelex(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
		#endif
		#ifdef GIA_USE_STANFORD_CORENLP
		void extractDatesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);					//Stanford Specific	[requires NERTemp info to be set, + need to know if timeConditionNodes have already been added; if not add them now]
		#endif
			#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
			void addTimeToSubstance(GIAentityNode * timeConditionEntity);
			#endif
	void extractQuantities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPfeatureParser);	//Stanford Compatible
		#ifdef GIA_USE_RELEX
		void extractQuantitiesRelex(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts);
		#endif
		#ifdef GIA_USE_STANFORD_CORENLP
		void extractQuantitiesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts);		//Stanford Specific	[requires NERTemp info to be set]
		#endif

	#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
	void extractMeasures(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts);			//Stanford Incompatible [measures not supported]
	void defineToBeAndToDoConditions(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts);	//Stanford Compatible
	void extractQualities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, int NLPdependencyRelationsType);			//Stanford Compatible
	void linkPropertiesParataxis(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);										//Stanford Compatible
	#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	#ifndef GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY
	void defineClausalComplementProperties(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);				//Stanford Specific
	#endif
	#endif
	#endif
	void defineTenseOnlyTimeConditions(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);	//#ifdef GIA_USE_CORPUS_DATABASE || #ifdef GIA_TRANSLATOR_XML_INTERPRETATION

	#ifdef GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	void defineActionConcepts1(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	#endif
	void defineActionConcepts2(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	#endif

	#ifdef GIA_CREATE_NEW_SUBSTANCE_CONCEPT_FOR_EVERY_REFERENCE_TO_A_SUBSTANCE_CONCEPT
	void updateSubstanceConceptDesignationBasedPropertyOwnerContext(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	#endif

#endif

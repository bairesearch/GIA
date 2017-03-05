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
 * File Name: GIAtranslatorApplyAdvancedFeatures.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p4c 17-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_APPLY_ADVANCED_FEATURES
#define HEADER_GIA_TRANSLATOR_APPLY_ADVANCED_FEATURES

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdatabase.hpp"
#include "GIAsemanticParserOperations.hpp"
#include "GIAtranslatorGeneric.hpp"

class GIAtranslatorApplyAdvancedFeaturesClass
{
	private: GIAconditionNodeClassClass GIAconditionNodeClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	#ifdef USE_GIA2
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	#endif
	private: GIAtranslatorGenericClass GIAtranslatorGeneric;
	public: void applyAdvancedFeatures(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType, const int NLPfeatureParser);
		private: void extractDates(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int NLPfeatureParser);					//Stanford Compatible
		#ifdef GIA_RELEX
			private: void extractDatesRelex(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
		#endif
		#ifdef GIA_STANFORD_CORENLP
			private: void extractDatesStanfordCoreNLP(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);					//Stanford Specific	[requires NERTemp info to be set, + need to know if timeConditionNodes have already been added; if not add them now]
		#endif
			#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
				public: void addTimeToSubstance(GIAentityNode* timeConditionEntity);
			#endif
		private: void extractQuantities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPfeatureParser);	//Stanford Compatible
		#ifdef GIA_RELEX
			private: void extractQuantitiesRelex(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);
		#endif
		#ifdef GIA_STANFORD_CORENLP
			private: void extractQuantitiesStanfordCoreNLP(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);		//Stanford Specific	[requires NERTemp info to be set]
		#endif

	#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
		private: void extractMeasures(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);			//Stanford Incompatible [measures not supported]
		private: void defineToBeAndToDoConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes);	//Stanford Compatible
		private: void extractQualities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const unordered_map<string, GIAentityNode*>* entityNodesActiveListNetworkIndexes, const int NLPdependencyRelationsType);			//Stanford Compatible
		private: void linkPropertiesParataxis(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);										//Stanford Compatible
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	#ifndef GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY
		private: void defineClausalComplementProperties(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);				//Stanford Specific
	#endif
	#endif
	#endif
		public: void defineTenseOnlyTimeConditions(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);	//#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER || #ifdef GIA_TRANSLATOR_XML_INTERPRETATION

	#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
		private: void defineActionConcepts1(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
		private: void defineActionConcepts2(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif

	#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
		private: void updateConceptDesignationBasedPropertyOwnerContext(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
};
	

#endif

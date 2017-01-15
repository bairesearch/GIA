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
 * File Name: GIAtranslatorDefineSubstances.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p3c 14-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_SUBSTANCES
#define HEADER_GIA_TRANSLATOR_DEFINE_SUBSTANCES

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAtranslatorOperations.h"
#include "GIAdatabase.h"
#include "GIAtranslatorGeneric.h"

//Pass B
//0
class GIAtranslatorDefineSubstancesClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorGenericClass GIAtranslatorGeneric;
#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
	public: void defineSubstances(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int referenceTypeHasDeterminateCrossReferenceNumberArray[], GIAfeature* featureArrayTemp[], const int NLPdependencyRelationsType);
		private: void defineSubstancesExpletives(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);									//Stanford Compatible
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
		private: void defineSubstancesBasedOnDeterminatesOfDefinitionEntities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[],  const int referenceTypeHasDeterminateCrossReferenceNumberArray[], GIAfeature* featureArrayTemp[]);
	#endif
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
		private: void defineSubstancesNonExplicitPronouns(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);								//Stanford Compatible
	#endif
	#ifdef GIA_SPECIFIC_CONCEPTS
		private: void defineConcepts(const GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[],  const int referenceTypeHasDeterminateCrossReferenceNumberArray[], constEffective GIAfeature* featureArrayTemp[]);
	#endif
		private: void defineSubstancesActionsToDo(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);										//Stanford Compatible
		private: void defineSubstancesTagIsToBeComplimentOfAction(const GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);

#ifndef GIA_CREATE_NON_SPECIFIC_CONCEPTS_FOR_ALL_NETWORK_INDEXES
		private: void defineSubstancesObjectsAndSubjectsWithSubstances(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], constEffective GIAentityNode* GIAfeatureTempEntityNodeArray[]);							//~Stanford Compatible [requires isObjectTemp, hasSubstanceTemp, isSubjectTemp to be filled]
		private: void defineSubstancesDefiniteNouns(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], constEffective GIAfeature* featureArrayTemp[]);						//Stanford Compatible [requires GIAEntityNodeGrammaticalIsProperNounArray[] + GIAEntityNodeIsDateOrTime[] + GIAEntityNodeIsAReferencep[] to be filled]
		private: void defineSubstancesNounsWithDeterminates(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], const int referenceTypeHasDeterminateCrossReferenceNumberArray[], constEffective GIAfeature* featureArrayTemp[]);	//Stanford Compatible [requires GIAEntityNodeGrammaticalIsProperNounArray[] + GIAEntityNodeIsDateOrTime[] + GIAEntityNodeIsAReferencep[] to be filled]
		private: void defineSubstancesNounsWithAdjectivesOrPrenominalModifiers(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], const int NLPdependencyRelationsType);				//Stanford Compatible
		private: void defineSubstancesQuantitiesAndMeasures(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);								//~Stanford Compatible [measures not supported]
		private: void defineSubstancesQuantityModifiers(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);									//~Stanford Compatible [measures not supported]
		private: void defineSubstancesPronouns(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], constEffective GIAfeature* featureArrayTemp[]);						//~Stanford Compatible {requires GRAMMATICAL_PRONOUN to be define + GIAEntityNodeIsAReference[] to be filled} - NB GRAMMATICAL_PRONOUN is not currently filled by Stanford
		private: void defineSubstancesToBe(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);										//Stanford Compatible
		private: void defineSubstancesHasTime(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], constEffective GIAfeature* featureArrayTemp[]);	//~Stanford Compatible {requires hasAssociatedTime to be filled}
		private: void defineSubstancesIndirectObjects(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);
		private: void defineSubstancesOfPossessivePrepositions(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//this function was developed after GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION and was designed for it
		private: void defineSubstancesActions(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);
	#endif
#endif
#endif

//these had to be separated out from the above because they are used by GIAsemanticParserTranslator.cpp;
	public: void defineSubstancesAllNodes(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
	public: void defineSubstancesActionNetworkIndexes(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], constEffective GIAfeature* featureArrayTemp[]);
#endif
};


#endif

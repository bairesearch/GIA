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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2j17a 07-July-2015
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

#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
//Pass B
//0
void defineSubstances(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], GIAfeature* featureArrayTemp[], int NLPdependencyRelationsType);
	void defineSubstancesObjectsAndSubjectsWithSubstances(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAentityNode* GIAfeatureTempEntityNodeArray[]);							//~Stanford Compatible [requires isObjectTemp, hasSubstanceTemp, isSubjectTemp to be filled]
	void defineSubstancesDefiniteNouns(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);						//Stanford Compatible [requires GIAEntityNodeGrammaticalIsProperNounArray[] + GIAEntityNodeIsDateOrTime[] + GIAEntityNodeIsAReferencep[] to be filled]
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	void defineSubstancesBasedOnDeterminatesOfDefinitionEntities(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[],  int referenceTypeHasDeterminateCrossReferenceNumberArray[], GIAfeature* featureArrayTemp[]);
	#endif
	void defineSubstancesNounsWithDeterminates(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], GIAfeature* featureArrayTemp[]);	//Stanford Compatible [requires GIAEntityNodeGrammaticalIsProperNounArray[] + GIAEntityNodeIsDateOrTime[] + GIAEntityNodeIsAReferencep[] to be filled]
	void defineSubstancesNounsWithAdjectivesOrPrenominalModifiers(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[], int NLPdependencyRelationsType);				//Stanford Compatible
	void defineSubstancesQuantitiesAndMeasures(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);								//~Stanford Compatible [measures not supported]
	void defineSubstancesQuantityModifiers(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);									//~Stanford Compatible [measures not supported]
	void defineSubstancesExpletives(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);									//Stanford Compatible
	void defineSubstancesPronouns(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);						//~Stanford Compatible {requires GRAMMATICAL_PRONOUN to be define + GIAEntityNodeIsAReference[] to be filled} - NB GRAMMATICAL_PRONOUN is not currently filled by Stanford
	void defineSubstancesToBe(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);										//Stanford Compatible
	void defineActionsToDo(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);										//Stanford Compatible
	void defineSubstancesHasTime(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);	//~Stanford Compatible {requires hasAssociatedTime to be filled}
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
	void defineSubstancesNonExplicitPronouns(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);								//Stanford Compatible
	#endif
	void defineSubstancesIndirectObjects(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
	void defineSubstanceConcepts(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[],  int referenceTypeHasDeterminateCrossReferenceNumberArray[], GIAfeature* featureArrayTemp[]);
	#endif
	void defineSubstancesOfPossessivePrepositions(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//this function was developed after GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION and was designed for it
	void defineSubstancesActions(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);
	#endif
#endif
//these had to be separated out from the above because they are used by GIAcorpusTranslator.cpp;
#ifdef GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS
void defineSubstancesActionConcepts(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);
#endif
void defineSubstancesAllNodes(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);

#endif

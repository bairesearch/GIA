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
 * Project Version: 2h7c 19-January-2015
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_SUBSTANCES
#define HEADER_GIA_TRANSLATOR_DEFINE_SUBSTANCES

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

#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
//Pass B
//0
void defineSubstances(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[], int NLPdependencyRelationsType);
	void defineSubstancesObjectsAndSubjectsWithSubstances(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], GIAentityNode * GIAfeatureTempEntityNodeArray[]);							//~Stanford Compatible [requires isObjectTemp, hasSubstanceTemp, isSubjectTemp to be filled]
	void defineSubstancesDefiniteNouns(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[]);						//Stanford Compatible [requires GIAEntityNodeGrammaticalIsProperNounArray[] + GIAEntityNodeIsDateOrTime[] + GIAEntityNodeIsAReferencep[] to be filled]
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	void defineSubstancesBasedOnDeterminatesOfDefinitionEntities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[],  int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[]);
	#endif
	void defineSubstancesNounsWithDeterminates(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[]);	//Stanford Compatible [requires GIAEntityNodeGrammaticalIsProperNounArray[] + GIAEntityNodeIsDateOrTime[] + GIAEntityNodeIsAReferencep[] to be filled]
	void defineSubstancesNounsWithAdjectivesOrPrenominalModifiers(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[], int NLPdependencyRelationsType);				//Stanford Compatible
	void defineSubstancesQuantitiesAndMeasures(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);								//~Stanford Compatible [measures not supported]
	void defineSubstancesQuantityModifiers(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);									//~Stanford Compatible [measures not supported]
	void defineSubstancesExpletives(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);									//Stanford Compatible
	void defineSubstancesPronouns(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[]);						//~Stanford Compatible {requires GRAMMATICAL_PRONOUN to be define + GIAEntityNodeIsAReference[] to be filled} - NB GRAMMATICAL_PRONOUN is not currently filled by Stanford
	void defineSubstancesToBe(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);										//Stanford Compatible
	void defineActionsToDo(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);										//Stanford Compatible
	void defineSubstancesHasTime(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[]);	//~Stanford Compatible {requires hasAssociatedTime to be filled}
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
	void defineSubstancesNonExplicitPronouns(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);								//Stanford Compatible
	#endif
	void defineSubstancesIndirectObjects(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);
	#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
	void defineSubstanceConcepts(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[],  int referenceTypeHasDeterminateCrossReferenceNumberArray[], Feature * featureArrayTemp[]);
	#endif
	void defineSubstancesOfPossessivePrepositions(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	//this function was developed after GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION and was designed for it
	void defineSubstancesActions(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);
	#endif
#endif
//these had to be separated out from the above because they are used by GIAcorpusTranslator.cpp;
#ifdef GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS
void defineSubstancesActionConcepts(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[]);
#endif
void defineSubstancesAllNodes(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);

#endif

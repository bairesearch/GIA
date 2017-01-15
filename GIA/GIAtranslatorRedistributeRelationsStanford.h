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
 * File Name: GIAtranslatorRedistributeRelationsStanford.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p2d 12-December-2016
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS_STANFORD
#define HEADER_GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS_STANFORD

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAentityNodeClass.h"
#include "GIAentityConnectionClass.h"
#include "GIAconditionNodeClass.h"
#include "GIAtranslatorOperations.h"

#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
void redistributeStanfordRelations(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser, GIAfeature* featureArrayTemp[]);
	#ifdef GIA_STANFORD_CORENLP
	void disableRedundantNodesStanfordCoreNLP(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
	void disableRedundantNodesStanfordParser(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	void redistributeStanfordRelationsCreateQueryVarsAdjustForActionPrepositionAction(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	void redistributeStanfordRelationsMultiwordPreposition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);		//Stanford Specific
	#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
	void redistributeStanfordRelationsInterpretOfAsPossessive(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);		//this should be supported for both Stanford + Relex, but has been untested for Relex
	#endif
	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_WHAT_IS_THE_NAME_NUMBER_OF_QUERIES
	void redistributeStanfordRelationsCreateQueryVarsWhatIsTheNameNumberOf(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);	//Stanford Specific? (may require Relex equivalent redistribution function)
	#endif
	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
	void redistributeStanfordRelationsInterpretNameOfAsDefinition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);			//Stanford Specific? (may require Relex equivalent redistribution function)
	#endif
	void redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser);	//Stanford Specific
	void redistributeStanfordRelationsCollapseSubjectAndCopGenerateAdjectivesAndAppos(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser);	//Stanford Specific [alternative relex function now provided redistributeStanfordRelationsDependencyPreposition()]
	void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);				//Stanford Specific
	void redistributeStanfordRelationsClausalSubject(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);						//Stanford Specific
	void redistributeStanfordRelationsConjunctionAndCoordinate(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);					//Stanford Specific
	void redistributeStanfordRelationsGenerateUnparsedQuantityModifers(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);				//Stanford Specific [makes up for deficency in Stanford Parser where it does not always generate quantity modifiers where expected]
	void redistributeStanfordRelationsGenerateMeasures(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);						//Stanford Specific
	void redistributeStanfordRelationsPhrasalVerbParticle(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);						//Stanford Specific
	void redistributeStanfordRelationsCreateQueryVars(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);						//Stanford Specific? (may require Relex equivalent redistribution function)
		void redistributeStanfordRelationsCreateQueryVarsWhoWhat(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);
		void redistributeStanfordRelationsCreateQueryVarsHowMuchHowMany(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
		void redistributeStanfordRelationsCreateQueryVarsWhich(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
		void redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
		void redistributeStanfordRelationsCreateQueryVarsWhat(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	void redistributeStanfordRelationsPartmod(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
	void redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], int NLPfeatureParser, GIAfeature* featureArrayTemp[]);	//Stanford Specific? (may require Relex equivalent redistribution function)
	#endif
	#ifdef GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_EXPLITIVES
	void redistributeStanfordRelationsExpletives(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);	//Stanford Specific? (may require Relex equivalent redistribution function)
	#endif
	void redistributeStanfordRelationsDependencyPreposition(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);	//Stanford Specific? (may require Relex equivalent redistribution function)
	#ifdef STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	void redistributeStanfordRelationsAuxHave(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[])
	#endif
	#endif
	#ifdef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
	void redistributeStanfordRelationsDisableAuxAndCop(GIAsentence* currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[]);
	#endif
	void collapseRedundantRelationAndMakeNegativeStanford(GIAsentence* currentSentenceInList, GIAentityNode* GIAentityNodeArray[]);						//Stanford Specific
#endif
#endif
#endif

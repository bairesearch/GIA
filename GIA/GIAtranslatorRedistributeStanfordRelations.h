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
 * File Name: GIAtranslatorRedistributeStanfordRelations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2g4c 03-September-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS
#define HEADER_GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS

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

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
void redistributeStanfordRelations(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser, Feature * featureArrayTemp[]);
	#ifdef GIA_USE_STANFORD_CORENLP
	void disableRedundantNodesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	#endif
	void disableRedundantNodesStanfordParser(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	void redistributeStanfordRelationsCreateQueryVarsAdjustForActionPrepositionAction(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	void redistributeStanfordRelationsMultiwordPreposition(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);		//Stanford Specific
	#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
	void redistributeStanfordRelationsInterpretOfAsPossessive(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);		//this should be supported for both Stanford + Relex, but has been untested for Relex
	#endif
	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_WHAT_IS_THE_NAME_NUMBER_OF_QUERIES
	void redistributeStanfordRelationsCreateQueryVarsWhatIsTheNameNumberOf(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);	//Stanford Specific? (may require Relex equivalent redistribution function)
	#endif
	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
	void redistributeStanfordRelationsInterpretNameOfAsDefinition(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);			//Stanford Specific? (may require Relex equivalent redistribution function)
	#endif
	void redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser);	//Stanford Specific
	void redistributeStanfordRelationsCollapseSubjectAndCopGenerateAdjectivesAndAppos(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser);	//Stanford Specific [alternative relex function now provided redistributeStanfordRelationsDependencyPreposition()]
	void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);				//Stanford Specific
	void redistributeStanfordRelationsClausalSubject(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);						//Stanford Specific
	void redistributeStanfordRelationsConjunctionAndCoordinate(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);					//Stanford Specific
	void redistributeStanfordRelationsGenerateUnparsedQuantityModifers(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);				//Stanford Specific [makes up for deficency in Stanford Parser where it does not always generate quantity modifiers where expected]
	void redistributeStanfordRelationsGenerateMeasures(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);						//Stanford Specific
	void redistributeStanfordRelationsPhrasalVerbParticle(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);						//Stanford Specific
	void redistributeStanfordRelationsCreateQueryVars(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[]);						//Stanford Specific? (may require Relex equivalent redistribution function)
		void redistributeStanfordRelationsCreateQueryVarsWhoWhat(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * featureArrayTemp[]);
		void redistributeStanfordRelationsCreateQueryVarsHowMuchHowMany(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
		void redistributeStanfordRelationsCreateQueryVarsWhich(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
		void redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
		void redistributeStanfordRelationsCreateQueryVarsWhat(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	void redistributeStanfordRelationsPartmod(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	#ifdef GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS
	void redistributeStanfordRelationsInterpretOfAsObjectForContinuousVerbs(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], int NLPfeatureParser, Feature * featureArrayTemp[]);	//Stanford Specific? (may require Relex equivalent redistribution function)
	#endif
	#ifdef GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_EXPLITIVES
	void redistributeStanfordRelationsExpletives(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);	//Stanford Specific? (may require Relex equivalent redistribution function)
	#endif
	void redistributeStanfordRelationsDependencyPreposition(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);	//Stanford Specific? (may require Relex equivalent redistribution function)
	/*
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	void redistributeStanfordRelationsAuxHave(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[])
	#endif
	*/
	#ifdef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
	void redistributeStanfordRelationsDisableAuxAndCop(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[]);
	#endif
	void collapseRedundantRelationAndMakeNegativeStanford(Sentence * currentSentenceInList, GIAentityNode * GIAentityNodeArray[]);						//Stanford Specific
#endif
#endif
#endif

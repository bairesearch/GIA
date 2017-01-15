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
 * File Name: GIATranslatorRedistributeStanfordRelations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n7b 31-July-2012
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
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAEntityConnectionClass.h"
#include "GIAConditionNodeClass.h"

#ifdef GIA_USE_STANFORD_CORENLP
void disableRedundantNodesStanfordCoreNLP(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
#endif

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
void redistributeStanfordRelationsMultiwordPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);										//Stanford Specific
void redistributeStanfordRelationsCollapseAdvmodRelationGovernorBe(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int NLPfeatureParser);	//Stanford Specific
void redistributeStanfordRelationsAdverbalClauseModifierAndComplement(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);				//Stanford Specific
void redistributeStanfordRelationsClausalSubject(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);						//Stanford Specific
//void redistributeStanfordRelationsPhrasalVerbParticle(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);						//Stanford Specific
void redistributeStanfordRelationsConjunctionAndCoordinate(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);					//Stanford Specific
void redistributeStanfordRelationsGenerateUnparsedQuantityModifers(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);				//Stanford Specific [makes up for deficency in Stanford Parser where it does not always generate quantity modifiers where expected]
void redistributeStanfordRelationsGenerateMeasures(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);						//Stanford Specific
void redistributeStanfordRelationsPrtAndTmod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);							//Stanford Specific
void redistributeStanfordRelationsCreateQueryVars(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
	void redistributeStanfordRelationsCreateQueryVarsWhoWhat(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
	void redistributeStanfordRelationsCreateQueryVarsHowMuchHowMany(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
	void redistributeStanfordRelationsCreateQueryVarsWhich(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
	void redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
	void redistributeStanfordRelationsCreateQueryVarsWhat(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
void redistributeStanfordRelationsPartmod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);


#endif


#endif

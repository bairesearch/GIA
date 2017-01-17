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
 * File Name: GIAtranslatorRedistributeRelations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p4a 17-January-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS
#define HEADER_GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
	#include "GIAlrp.hpp"
#include "GIAtranslatorDefineGrammar.hpp"
#include "SHAREDvars.hpp"	//required for convertStringToLowerCase

//should move the following to GIAtranslatorRedistributeStanfordAndRelations.h/.cpp
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
class GIAtranslatorRedistributeRelationsClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAlrpClass GIAlrp;
	private: GIAtranslatorDefineGrammarClass GIAtranslatorDefineGrammar;
	public: void redistributeStanfordAndRelexRelationsCorrectPOStagsAndLemmasOfAllVerbs(GIAsentence* currentSentenceInList, const bool GIAentityNodeArrayFilled[], GIAentityNode* GIAentityNodeArray[], GIAfeature* featureArrayTemp[]);	//Stanford Specific? (may require Relex equivalent redistribution function)
		private: bool correctVerbPOStagAndLemma(GIAentityNode* actionOrSubstanceEntity, GIAfeature* currentFeature);
};
		//bool determineVerbCase(string* word);

#endif

#endif

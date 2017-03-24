/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: GIAbot.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a1p 26-February-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_BOT
#define HEADER_GIA_BOT


#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"

#define FEATURE_FIRST_PERSON_NUMBER_OF_TYPES (5)
#define FEATURE_SECOND_PERSON_NUMBER_OF_TYPES (5)
static string featureSecondPersonNameArray[FEATURE_SECOND_PERSON_NUMBER_OF_TYPES] = {"you","you","your","yours","yourself"};
static string featureFirstPersonNameArray[FEATURE_FIRST_PERSON_NUMBER_OF_TYPES] = {"i","me","my","mine","myself"};

#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
class GIAbotClass
{
	public: void botSwitchFirstAndSecondPerson(GIAtranslatorVariablesClass* translatorVariables);
	public: void botSwitchFirstAndSecondPersonSemantic(GIAsentence* currentSentenceInList);
};
#endif


#endif
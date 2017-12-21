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
 * File Name: GIAentityConnectionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e8e 18-December-2017
 *
 *******************************************************************************/


#include "GIAentityConnectionClass.hpp"


GIAentityConnection::GIAentityConnection(void)
{
	initialiseDefaultValues();
}
GIAentityConnection::~GIAentityConnection(void)
{
}
GIAentityConnection::GIAentityConnection(string* startEntityName, long startIdInstance)
{
	initialiseDefaultValues();
	#ifdef GIA_DATABASE
	entityName = *startEntityName;
	idInstance = startIdInstance;
	#endif
}


void GIAentityConnection::initialiseDefaultValues(void)
{
	entity = NULL;
	#ifdef GIA_ENTITY_CONNECTION_RECORD_ENTITY_ORIGIN
	entityOrigin = NULL;
	#ifdef GIA_ENTITY_CONNECTION_RECORD_RELATIONSHIP_TYPE
	connectionType = INT_DEFAULT_VALUE;
	#endif
	#endif
	
	confidence = GIA_ENTITY_CONNECTION_CONFIDENCE_BASE;

	sameReferenceSet = false;

	#ifdef GIA_DATABASE
	referenceLoaded = false;
	entityName = "";
	idInstance = INT_DEFAULT_VALUE;
	loaded = false;
	modified = false;
	added = false;
	#endif

	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
	initialisedForPrinting = false;
	#endif

	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_RECORD_AUX_INFO
	#ifdef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION
	possessionAuxiliaryHave = false;
	#endif
	#define GIA_ENTITY_CONNECTION_NEGATIVE_DEFINED
	negative = false;
	//grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES] = NULL;
	grammaticalTenseTemp = GRAMMATICAL_TENSE_UNDEFINED;
	#endif
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	isAlias = false;
	#endif
	#endif
	
	#ifdef USE_NLC
	#ifndef GIA_ENTITY_CONNECTION_NEGATIVE_DEFINED
	negative = false;	//already defined
	#endif
	NLCparsedForCodeBlocks = false;
	NLCparsedForlogicalConditionOperations = false;
	NLCcontextGenerated = false;
	#endif
	#ifdef GIA_ADVANCED_REFERENCING
	#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
	isReference = false;
	#endif
	#endif
	sentenceIndexTemp = GIA_SENTENCE_INDEX_UNDEFINED2;

	/*
	#ifdef GIA_NLG
	parsedForLanguageGeneration = false;
	#endif
	*/
}	

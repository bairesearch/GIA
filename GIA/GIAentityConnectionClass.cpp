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
 * File Name: GIAentityConnectionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2m1d 31-August-2016
 *
 *******************************************************************************/


#include "GIAentityConnectionClass.h"


GIAentityConnection::GIAentityConnection(void)
{
	confidence = GIA_ENTITY_CONNECTION_CONFIDENCE_BASE;

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	sameReferenceSet = false;	//CHECKTHIS default value
	#ifdef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION
	possessionAuxiliaryHave = false;
	#endif
	#endif

	#ifdef GIA_USE_DATABASE
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

	#ifdef USE_NLC
	NLCparsedForCodeBlocks = false;
	NLCparsedForlogicalConditionOperations = false;
	negative = false;
	NLCcontextGenerated = false;
	#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
	isReference = false;
	#endif
	#endif
	#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
	sentenceIndexTemp = GIA_SENTENCE_INDEX_UNDEFINED2;
	#endif
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	isAlias = false;
	#endif

	/*
	#ifdef GIA_USE_NLG
	parsedForLanguageGeneration = false;
	#endif
	*/
}
GIAentityConnection::~GIAentityConnection(void)
{
}
GIAentityConnection::GIAentityConnection(string* startEntityName, long startIdInstance)
{
	confidence = GIA_ENTITY_CONNECTION_CONFIDENCE_BASE;

	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	sameReferenceSet = false;	//CHECK THIS default value
	#endif

	#ifdef GIA_USE_DATABASE
	referenceLoaded = false;
	entityName = *startEntityName;
	idInstance = startIdInstance;
	loaded = false;
	modified = false;
	added = false;
	#endif

	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
	initialisedForPrinting = false;
	#endif

	#ifdef USE_NLC
	NLCparsedForCodeBlocks = false;
	NLCparsedForlogicalConditionOperations = false;
	negative = false;
	NLCcontextGenerated = false;
	#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
	isReference = false;
	#endif
	#endif
	#ifdef GIA_STORE_CONNECTION_SENTENCE_INDEX
	sentenceIndexTemp = GIA_SENTENCE_INDEX_UNDEFINED2;
	#endif
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	isAlias = false;
	#endif

	/*
	#ifdef GIA_USE_NLG
	parsedForLanguageGeneration = false;
	#endif
	*/
}


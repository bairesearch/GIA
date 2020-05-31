/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAentityConnectionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3l1a 28-May-2020
 * /
 *******************************************************************************/


#include "GIAentityConnectionClass.hpp"


GIAentityConnection::GIAentityConnection(void)
{
	initialiseDefaultValues();
}
GIAentityConnection::~GIAentityConnection(void)
{
}
GIAentityConnection::GIAentityConnection(string* startEntityName, int64_t startIdInstance)
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

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	semanticRelationConnectionDynamic = false;
	#endif
	
	/*
	#ifdef GIA_NLG
	parsedForLanguageGeneration = false;
	#endif
	*/
}	

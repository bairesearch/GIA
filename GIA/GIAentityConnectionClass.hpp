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
 * File Name: GIAentityConnectionClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3m1a 23-July-2020
 * NB a substance is an instance of an entity, any given entity may contain/comprise/have multiple substances - and substances are unrelated to definitions between entities [they just define what comprises any given entity]
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_ENTITY_CONNECTION_CLASS
#define HEADER_GIA_ENTITY_CONNECTION_CLASS



#include "GIAglobalDefs.hpp"


#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>		//required for GIA_NETWORK_INDEX_ENTITY_NODE_MAP_NOT_VECTOR
#include <utility> 			// make_pair	//required for GIA_NETWORK_INDEX_ENTITY_NODE_MAP_NOT_VECTOR
using namespace std;

#ifndef HEADER_GIA_ENTITY_NODE_CLASS
#define GRAMMATICAL_TENSE_UNDEFINED 0
#define GRAMMATICAL_TENSE_PRESENT 1		//eg mow / ~VBP
#define GRAMMATICAL_TENSE_PAST 2		//eg mowed / ~VBD
#define GRAMMATICAL_TENSE_FUTURE 3		//eg will mow / ?
#define GRAMMATICAL_TENSE_NUMBER_OF_TYPES 4
#define GRAMMATICAL_TENSE_PRESENT_STRING "1"
#define GRAMMATICAL_TENSE_PAST_STRING "2"
#define GRAMMATICAL_TENSE_FUTURE_STRING "3"
#define GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE 0	//eg mowing / ~VBG
#define GRAMMATICAL_TENSE_MODIFIER_PERFECT 1
#define GRAMMATICAL_TENSE_MODIFIER_PASSIVE 2		//NB present passive is interpreted by GIA to be the same as state; eg is mowed (as opposed to was mowed)
#define GRAMMATICAL_TENSE_MODIFIER_INFINITIVE 3		//eg to mow / ~VB	//added 2h2c: _ment/_ion nouns, eg enjoyment/abolition
#define GRAMMATICAL_TENSE_MODIFIER_IMPERATIVE 4		//eg Mow the grass! / ~VB
#define GRAMMATICAL_TENSE_MODIFIER_POTENTIAL 5	//added 2h2a/2h2c: _able/_ive adjectives (ignore nouns)	//eg mowable / affirmative
#define GRAMMATICAL_TENSE_MODIFIER_STATE 6	//added 2h2a	//eg is mowed (as opposed to was mowed)	//used for both states and affections - note noun versus verb base forms are not distinguished here by POS tagger; both are assigned VBN
#define GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION 7	//added 2h2d: _ment/_ion nouns	//eg movement / transition	//note these are different than action networkIndexes, as these define an instance of an action, not an action in general; eg "the movement"/"the transition"
#define GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES 8
#define GRAMMATICAL_TENSE_MODIFIER_INFINITIVE_OR_IMPERATIVE_OR_PRESENT_NOT_THIRD_PERSON_SINGULAR_OR_STATE_TEMP (7)
#define GRAMMATICAL_TENSE_MODIFIER_PAST_TENSE_OR_PAST_PARTICIPLE_OR_STATE_TEMP (8)
#define GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE_TEMP (9)
#define GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_TEMP (10)
#define GRAMMATICAL_TENSE_MODIFIER_POTENTIAL_INVERSE_TEMP (11)
#define GRAMMATICAL_TENSE_MODIFIER_DESCRIPTION_TEMP (12)
#endif

#define GIA_ENTITY_CONNECTION_CONFIDENCE_BASE (1.0)
#define GIA_ENTITY_CONNECTION_CONFIDENCE_INCREMENT (1.0)

#define GIA_SENTENCE_INDEX_UNDEFINED2 (INT_DEFAULT_VALUE)

class GIAentityNode;

class GIAentityConnection
{
public:
	GIAentityConnection(void);
	~GIAentityConnection(void);
	GIAentityConnection(string* entityName, int64_t idInstance);
	
	void initialiseDefaultValues(void);

	GIAentityNode* entity;
	#ifdef GIA_ENTITY_CONNECTION_RECORD_ENTITY_ORIGIN
	GIAentityNode* entityOrigin;
	#ifdef GIA_ENTITY_CONNECTION_RECORD_RELATIONSHIP_TYPE
	int connectionType;
	#endif
	#endif
	double confidence;

	bool sameReferenceSet;

	#ifdef GIA_DATABASE
	bool referenceLoaded;
	string entityName;	//records the vector connection target entity name (to enable loading from db) [of target]
	int64_t idInstance;	//records the vector connection target instance id (to enable loading from db) [of target]
	bool loaded;		//signifies whether the vector connection target node has been loaded and is in RAM (eg from the db)
	bool modified;		//signifies whether the database needs to be updated upon exit with modified reference
	bool added;		//signifies whether the database needs to be updated upon exit with new reference
	//int64_t referenceIndex; 	//OLD [now assume referenceIndex equals the index of the connection in the connection vector] added 24 May 2012 (needs to be populated)
	#endif

	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED
	bool initialisedForPrinting;
	#endif

	#ifndef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC_RECORD_AUX_INFO
	#ifdef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION
	bool possessionAuxiliaryHave;	//added 1k3a	//3a1a: set by GIA
	#endif
	#define GIA_ENTITY_CONNECTION_NEGATIVE_DEFINED
	bool negative;
	bool grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES];
	int grammaticalTenseTemp;
	#endif
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	bool isAlias;
	#endif
	#endif
	
	#ifdef USE_NLC
	#ifndef GIA_ENTITY_CONNECTION_NEGATIVE_DEFINED
	bool negative;	//already defined
	#endif
	bool NLCparsedForCodeBlocks;
	bool NLCparsedForlogicalConditionOperations;
	bool NLCcontextGenerated;	//added NLC 1g14b/15-July-2014
	#endif
	#ifdef GIA_ADVANCED_REFERENCING
	#ifdef GIA_TRANSLATOR_MARK_DOUBLE_LINKS_AS_REFERENCE_CONNECTIONS
	bool isReference;	//added NLC 1i2a/20-August-2014
	#endif
	#endif
	int sentenceIndexTemp;

	#ifdef GIA_POS_REL_TRANSLATOR_SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	bool semanticRelationConnectionDynamic;
	#endif
	
	/*
	#ifdef GIA_NLG
	bool parsedForLanguageGeneration;
	#endif
	*/
};


#endif


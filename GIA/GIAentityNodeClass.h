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
 * File Name: GIAentityNodeClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2o7b 23-October-2016
 * NB a substance is an instance of an entity, any given entity may contain/comprise/have multiple substances - and substances are unrelated to definitions between entities [they just define what comprises any given entity]
 *
 *******************************************************************************/


#ifndef HEADER_GIA_ENTITY_NODE_CLASS
#define HEADER_GIA_ENTITY_NODE_CLASS



#include "GIAconditionNodeClass.h"
#include "GIAglobalDefs.h"
#include "SHAREDvars.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>		//required for GIA_USE_NETWORK_INDEX_ENTITY_NODE_MAP_NOT_VECTOR
#include <utility> // make_pair	//required for GIA_USE_NETWORK_INDEX_ENTITY_NODE_MAP_NOT_VECTOR
using namespace std;

#define GIA_ENTITY_ID_INSTANCE_NETWORK_INDEX_ENTITY (0)
#define GIA_ENTITY_ID_INSTANCE_FIRST_INSTANCE_ENTITY (1)

//entity types added 2n1c
#define GIA_ENTITY_TYPE_TYPE_UNDEFINED (-1)
#define GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX (0)
#define GIA_ENTITY_TYPE_TYPE_SUBSTANCE (1)
#define GIA_ENTITY_TYPE_TYPE_ACTION (2)
#define GIA_ENTITY_TYPE_TYPE_CONDITION (3)
#define GIA_ENTITY_TYPE_TYPE_CONCEPT (4)
#define GIA_ENTITY_TYPE_TYPE_QUALITY (5)
#define GIA_ENTITY_NUMBER_OF_TYPES (6)
#define GIA_ENTITY_TYPE_TYPE_NETWORK_INDEX_STRING "0"
#define GIA_ENTITY_TYPE_TYPE_SUBSTANCE_STRING "1"
#define GIA_ENTITY_TYPE_TYPE_ACTION_STRING "2"
#define GIA_ENTITY_TYPE_TYPE_CONDITION_STRING "3"
#define GIA_ENTITY_TYPE_TYPE_CONCEPT_STRING "4"
#define GIA_ENTITY_TYPE_TYPE_QUALITY_STRING "5"


//#ifdef GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_PREPOSITION (0)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_TITLE (1)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_QUOTES (2)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS (3)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_PREPOSITION_DELIMITER "_"	//this is now equivalent to STANFORD_PARSER_PREPOSITION_DELIMITER
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_TITLE_DELIMITER "_"
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_QUOTES_DELIMITER "_"	//this filler does not appear compatible with Relex (Stanford only); try another filler character (NB "-" doesn't work with Relex either)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER "_"
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_NUMBER_OF_TYPES (4)
	static string concatenationTypesArray[GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_NUMBER_OF_TYPES] = {GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_PREPOSITION_DELIMITER, GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_TITLE_DELIMITER, GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_QUOTES_DELIMITER, GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER};
//#endif

#define GRAMMATICAL_WORD_TYPE_UNDEFINED (0)

#ifdef GIA_SUPPORT_PREDETERMINERS
	//added 2i34a
	//must be synced with GIAtranslatorDefs.h entityPredeterminerSmallArray;
	#define GRAMMATICAL_PREDETERMINER_UNDEFINED (INT_DEFAULT_VALUE)	//-1
	#define GRAMMATICAL_PREDETERMINER_EACH 0
	#define GRAMMATICAL_PREDETERMINER_EVERY 1
	#define GRAMMATICAL_PREDETERMINER_ALL 2
	#define GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES (3)
	static int entityPredeterminerSmallArray[GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES] = {GRAMMATICAL_PREDETERMINER_EACH, GRAMMATICAL_PREDETERMINER_EVERY, GRAMMATICAL_PREDETERMINER_ALL};	//added 2i34a

#endif

#define GIA_NLP_START_ENTITY_INDEX (1)
#define GIA_NLP_START_SENTENCE_INDEX (1)

#ifdef GIA_SUPPORT_ALIASES
	#define GIA_ALIASES_CONVERT_TO_STRING_DELIMITER ';'	//has to be different than attribute delimiter used in GIA filesystem database GIA_DATABASE_ATTRIBUTE_DELIMITER
#endif

#define GRAMMATICAL_TENSE_CONCATONATOR_RELEX "_"

//these have been moved from GIAtranslatorOperations.h as RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES is used by GIAdraw.cpp
#define RELATION_ENTITY_HAVE "have"
#define RELATION_ENTITY_CAN "can"
#define RELATION_ENTITY_BE "be"	//eg x is y
#define RELATION_ENTITY_IT "it"		//used to handle Relex special case query "What [time] is it?"

#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
	#define RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES (RELATION_ENTITY_HAVE)
	#ifdef GIA_RECORD_POSSESSION_AUXILIARY_HAS_INFORMATION_GENERAL_IMPLEMENTATION
		#define RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES_NUMBER_OF_TYPES (2)
		#define RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES_HAVE (RELATION_ENTITY_HAVE)
		#define RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES_POSS "poss"	//NB "poss" is an artificial "have" auxiliary
		static string relationEntitySpecialActionNameForEffectivePropertiesArray[RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES_NUMBER_OF_TYPES] = {RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES_HAVE, RELATION_ENTITY_SPECIAL_ACTION_NAME_FOR_EFFECTIVE_PROPERTIES_POSS};
	#endif
#endif


//extracted from wn.h (WordNet 3.0 library header)
#define GRAMMATICAL_WORD_TYPE_UNDEFINED (0)
#define GRAMMATICAL_WORD_TYPE_NOUN (1)
#define GRAMMATICAL_WORD_TYPE_VERB (2)
#define GRAMMATICAL_WORD_TYPE_ADJ (3)
#define GRAMMATICAL_WORD_TYPE_ADV (4)
#define GRAMMATICAL_WORD_TYPE_PREP (5)
#define GRAMMATICAL_WORD_TYPE_SATELLITE (6)	/* not really a part of speech*/
#define GRAMMATICAL_WORD_TYPE_ADJSAT (SATELLITE)
#define GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES 7
	//for xml interface:
#define GRAMMATICAL_WORD_TYPE_UNDEFINED_STRING "0"
#define GRAMMATICAL_WORD_TYPE_NOUN_STRING "1"
#define GRAMMATICAL_WORD_TYPE_VERB_STRING "2"
#define GRAMMATICAL_WORD_TYPE_ADJ_STRING "3"
#define GRAMMATICAL_WORD_TYPE_ADV_STRING "4"
#define GRAMMATICAL_WORD_TYPE_PREP_STRING "5"
#define GRAMMATICAL_WORD_TYPE_SATELLITE_STRING "6"	/* not really a part of speech*/
#define GRAMMATICAL_WORD_TYPE_ADJSAT_STRING "6"

#ifndef HEADER_GIA_ENTITY_CONNECTION_CLASS
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

#define GRAMMATICAL_NUMBER_UNDEFINED 0
#define GRAMMATICAL_NUMBER_UNCOUNTABLE 1
#define GRAMMATICAL_NUMBER_SINGULAR 2
#define GRAMMATICAL_NUMBER_PLURAL 3
#define GRAMMATICAL_NUMBER_NUMBER_OF_TYPES 4
#define GRAMMATICAL_NUMBER_UNDEFINED_STRING "0"
#define GRAMMATICAL_NUMBER_UNCOUNTABLE_STRING "1"
#define GRAMMATICAL_NUMBER_SINGULAR_STRING "2"
#define GRAMMATICAL_NUMBER_PLURAL_STRING "3"

#define GRAMMATICAL_DEFINITE_NAME "definite"

#define GRAMMATICAL_PERSON_UNDEFINED false
#define GRAMMATICAL_PERSON true
#define GRAMMATICAL_PERSON_NAME "person"

#define GRAMMATICAL_GENDER_UNDEFINED 0
#define GRAMMATICAL_GENDER_PERSON 1
#define GRAMMATICAL_GENDER_MASCULINE 2
#define GRAMMATICAL_GENDER_FEMININE 3
#define GRAMMATICAL_GENDER_NUMBER_OF_TYPES 4

#define GRAMMATICAL_CATEGORY_UNDEFINED (0)

#define GRAMMATICAL_PRONOUN_NAME "pronoun"

//#define GRAMMATICAL_COUNT_UNDEFINED 0

#define GRAMMATICAL_PROPERNOUN_NAME "propernoun"	//added 28 April 2012 (for cff export)


#define QUANTITY_NUMBER_UNDEFINED (1)
#define QUANTITY_NUMBER_LOW_NUMBER_OF_TYPES (20)
#define QUANTITY_NUMBER_TENS_NUMBER_OF_TYPES (10)
#define QUANTITY_MULTIPLIER_NUMBER_OF_TYPES (7)		//do: work out what these are/can be
#define QUANTITY_MODIFIER_UNDEFINED (INT_DEFAULT_VALUE)		//WARNING: not yet implemented
#define QUANTITY_MODIFIER_NUMBER_OF_TYPES (1)		//WARNING: not yet implemented	//do: work out what these are/can be

#define MEASURE_TYPE_DISTANCE (0)	//see relationTypeMeasureNameArray
#define MEASURE_TYPE_SIZE (1)
#define MEASURE_TYPE_TIME (2)
#define MEASURE_TYPE_PER (3)
#define MEASURE_TYPE_UNKNOWN (4)
#define MEASURE_DEPENDENCY_UNKNOWN (5)
#define MEASURE_TYPE_UNDEFINED (INT_DEFAULT_VALUE)

#define FEATURE_NER_UNDEFINED (0)
#define FEATURE_NER_DATE (1)
#define FEATURE_NER_TIME (2)
#define FEATURE_NER_MONEY (3)
#define FEATURE_NER_NUMBER (4)
#define FEATURE_NER_PERSON (5)
#define FEATURE_NER_LOCATION (6)
#define FEATURE_NER_ORGANIZATION (7)
#define FEATURE_NER_MISC (8)
#define FEATURE_NER_DURATION (9)
#define FEATURE_NER_NUMBER_OF_TYPES 10
#define FEATURE_NER_EXPLICIT_NUMBER_OF_TYPES 9
#define FEATURE_NER_UNDEFINED_STRING "0"
#define FEATURE_NER_DATE_STRING "1"
#define FEATURE_NER_TIME_STRING "2"
#define FEATURE_NER_MONEY_STRING "3"
#define FEATURE_NER_NUMBER_STRING "4"
#define FEATURE_NER_PERSON_STRING "5"
#define FEATURE_NER_LOCATION_STRING "6"
#define FEATURE_NER_ORGANIZATION_STRING "7"
#define FEATURE_NER_MISC_STRING "8"
#define FEATURE_NER_DURATION_STRING "9"

static string grammaticalTenseNameArray[GRAMMATICAL_TENSE_NUMBER_OF_TYPES] = {"undefined", "present", "past", "future"};
	//OPENCOG RECOMMENDED; "future", "future_progressive", "imperative", "infinitive", "past", "past_infinitive", "past_progressive", "perfect", "present", "present_progressive", "progressive"
	//MINIMAL; "undefined", "future", "progressive", "imperative", "infinitive", "past", "perfect", "present"
	//ABSTRACTED; "present", "present_perfect", "present_passive", "present_infinitive", "present_progressive", "present_progressive_passive", "past", "past_passive", "past_infinitive", "past_progressive", "future", "future_progressive", "imperative", "progressive"
static string grammaticalTenseModifierNameArray[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES] = {"progressive", "perfect", "passive", "infinitive", "imperative"};
static string grammaticalNumberNameArray[GRAMMATICAL_NUMBER_NUMBER_OF_TYPES] = {"undefined", "uncountable", "singular", "plural"};
static string grammaticalGenderNameArray[GRAMMATICAL_GENDER_NUMBER_OF_TYPES] = {"undefined", "person", "masculine", "feminine"};

static string grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES] = {"undefined", "noun", "verb", "adj", "adv", "prep", "satellite"};	//must be same as FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME


#define GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES (14)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS (0)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS (1)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS (2)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS (3)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES (4)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES (5)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS (6)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS (7)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES (8)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT (9)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT (10)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT (11)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT (12)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE (13)	//GIA_ENTITY_BASIC_CONNECTION_TYPE_INSTANCE_DEFINITION
static string entityVectorConnectionNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"actions", "incomingActions", "conditions", "incomingConditions", "properties", "reverseProperties", "definitions", "reverseDefinitions", "associatedInstances","actionSubject", "actionObject", "conditionSubject", "conditionObject", "nodeDefiningInstance"};	//instanceDefinition
static string entityVectorConnectionSourceContextArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"is ", "", "", "", "has ", "possessed by ", "is ", "defines ", "","is done by ", "", "", "", ""};
static string entityVectorConnectionContextArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"outgoingAction(s)", "incomingAction(s)", "conditionNode(s)", "incomingConditionNode(s)", "propertyNode(s)", "reversePropertyNode(s)", "entityNodeDefinition(s)", "reverseEntityNodeDefinition(s)", "associatedInstanceNodes(s)", "actionSubjectEntity", "actionObjectEntity", "conditionSubjectEntity", "conditionObjectEntity", "entityNodeDefiningThisInstance"};
static bool entityVectorConnectionThisIsInstanceAndPreviousNodeWasDefinitionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, false, false, false, true, false, false, false, false, false};
//ORIG: static bool entityVectorConnectionIsConditionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, true, true, false, false, false, false, false, false, false, true, true, false};
static bool entityVectorConnectionIsConditionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, true, true, false, false, false, false, false, false, false, false, false, false};
static bool entityVectorConnectionIsBasicArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false,false,false,false,false,false,false,false,false,true,true,true,true,true};
static string entityVectorConnectionDrawConnectionNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"subject", "object", "subject", "object", "property", "property", "definition", "definition", "instance", "subject", "object", "subject", "object", "instance"};
#define GIA_ENTITY_TIME_CONDITION_NODE_NAME "timeCondition"
#define GIA_ENTITY_NODE_NAME "entity"
/*
#define GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPES (2)
*/

static int inverseVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_PROPERTIES, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_NODE_DEFINING_INSTANCE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_ACTIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INCOMING_CONDITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ASSOCIATED_INSTANCES};


#define GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES_EQUALITIES (2)
static int entityVectorConnectionEqualitiesArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES_EQUALITIES] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITIONS, GIA_ENTITY_VECTOR_CONNECTION_TYPE_REVERSE_DEFINITIONS};


#define VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define BASIC_DEFINING_INSTANCE_RCMOD_INDICATES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define GIA_REFERENCE_SET_ID_UNDEFINED (INT_DEFAULT_VALUE)
#define GIA_ENTITY_INDEX_UNDEFINED (INT_DEFAULT_VALUE)
#define GIA_SENTENCE_INDEX_UNDEFINED (INT_DEFAULT_VALUE)

class GIAentityConnection;

#ifdef GIA_USE_DATABASE
#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
class GIAnetworkIndexEntityLoaded
{
public:
	GIAnetworkIndexEntityLoaded(void);
	~GIAnetworkIndexEntityLoaded(void);

	bool loaded;
	long numberOfInstances;
	#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
	bool disabled; 	//added 29 November 2012 for GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES support
	#endif
};
#endif
#endif

class GIAentityNode
{
public:

	GIAentityNode(void);
	#ifdef USE_NLC
	//#ifdef NLC_NONOO
	GIAentityNode(string newEntityName);
	//#endif
	#endif
	~GIAentityNode(void);


	/*GIA Internal Entity Referencing*/
	long idActiveList;
	long idActiveListReorderdIDforXMLsave;	//for CXL output only
	long idInstance; 		//not for networkIndexes (this instance idActiveList of the networkIndex entityName)


	/*GIA Entity Name*/
	string entityName;
	#ifdef GIA_USE_WORD_ORIG
	string wordOrig;		//this needs to be added to XML i/o + file system database i/o [used for NLG2 bug]
	#endif
	double confidence;		//not currently used
	#ifdef GIA_SUPPORT_ALIASES
	vector<string> aliasList;
	#endif


	/*GIA Entity Type*/
	int entityType;
	bool isActionConcept;			//added 1t5a to take into account specific actions eg 'eating pies', 'to eat a pie'
	bool hasAssociatedInstance;	//this boolean appears to only represent whether this entity defines a child substance node [and not whether it contains one]
	bool hasAssociatedTime;
	bool negative;	//for prepositional entities which will be collapsed into conditions only [in the future, this should also be used for substances and actions; but relex does not appear to output this information]


	/*GIA Connections*/
	vector<GIAentityConnection*> entityVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];		//allows for generic coding
	#ifdef GIA_USE_DATABASE
	//designed for a high scale database (eg 200,000 references per instance, 200,000 instances per networkIndex)
	bool entityVectorConnectionsReferenceListLoadedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];			//signifies whether all the vector connections in the reference list has been loaded from file and entityConnections entityNames+idInstance have therefore been populated. This is the first step required to enable loading of connections into RAM (see entityVectorConnectionsLoadedArray)
	//bool entityVectorConnectionsLoadedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];				//not used - vector connections are loaded into RAM on an individual basis. //signifies whether all the vector connection nodes have been loaded (eg from the db)
	bool entityVectorConnectionsRemovedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];					//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
	#endif
		//action connections;
	//non-actions only;
	vector<GIAentityConnection*>* actionNodeList;	//where this entity is the subject of the action
	vector<GIAentityConnection*>* incomingActionNodeList;	//where this entity is the object of the action
	//actions only;
	//NB actions can be performed by and on networkIndexes, and by and on substances?
	vector<GIAentityConnection*>* actionSubjectEntity;	//record of entity that is the subject of this action instance
	vector<GIAentityConnection*>* actionObjectEntity;	//record of which entity that is the object of this action instance
		//condition connections;
	//non-conditions only;
	//conditions connections: conditions and reverse conditions (reason) lookups [condition and reason respectively]
	vector<GIAentityConnection*>* conditionNodeList;		//this substance requires the following...
	vector<GIAentityConnection*>* incomingConditionNodeList;	//this substance is required by the following... //aka reason
	//conditions only;
	//NB conditions can be performed by and on networkIndexes, and by and on substances?
	vector<GIAentityConnection*>* conditionSubjectEntity;		//record of entity that is the subject of this action instance
	vector<GIAentityConnection*>* conditionObjectEntity;		//record of which entity that is the object of this action instance
		//substance connections;
	//record list of all substances for this entity
	vector<GIAentityConnection*>* propertyNodeList;
	vector<GIAentityConnection*>* propertyNodeReverseList;			//if substance/action only:	//eg, Tom; OR;  Tom's Assets	//more than 1 thing can contain any given substance [eg "a cat has arms", and "a monkey has arms"]; but note this may only be applicable for networkIndex entities [substance entities may possibly only be contained by {ie, be a substance of} a single entity]
	//actions, substances, and conditions only
	vector<GIAentityConnection*>* entityNodeDefiningThisInstance;					//if substance/action/condition only:					//NB by definition, only 1 thing can contain any given substance [considering a substance is an instance of an entity] - therefore this is not a vector
		//entity connections;
	//record parent and child entity definition nodes
	vector<GIAentityConnection*>* entityNodeDefinitionList;			//this should logically reduce to a single entity, although not required, therefore it is a vector [eg, a dog is a mammal, which is an animal, but a dog is an animal also]
	vector<GIAentityConnection*>* entityNodeDefinitionReverseList;			//more than one entity can be defined by this entity [eg if this entity is "animal", a bird is an animal, a mammal is an animal, etc]
	//networkIndexes only (not substances/"instances" of entities);
	//associated actions and substances [ie does this entity also define an action/verb or a substance/adjective? [ie, it is not just a thing/noun]]
	vector<GIAentityConnection*>* associatedInstanceNodeList;			//if this entity is not a substance/instance but defines one or more substances/instances
		//time condition connections;
	int conditionType;	//added 25 Sept 11
	GIAtimeConditionNode* timeConditionNode;		//if conditionType == CONDITION_NODE_TYPE_TIME

	/*GIA Special Variables (Quantities/Measures)*/
	bool hasQuantity;
	int quantityNumber;		//eg 6
	string quantityNumberString;	//eg "6:45"
	int quantityModifier;	//not yet implemented
	string quantityModifierString;	//eg "almost"
	bool hasQuantityMultiplier;
	bool hasMeasure;
	int measureType;


	/*GIA Draw Variables*/
	bool initialisedForPrinting;
	int printX;
	int printY;
	int printXIndex;
	int printYIndex;
	int printTextX;
	int printTextY;
	bool printCoordsAlreadyDefined;


	/*GIA Translator Temporary Variables - Grammar*/
	int grammaticalNumber;
	int grammaticalWordTypeTemp;		//aka wordNetPOS	//added 26 April 2012 (used for compatibility with wordnet)
	bool grammaticalTenseModifierArrayTemp[GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES];	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	int grammaticalTenseTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool grammaticalDefiniteTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool grammaticalIndefinitePluralTemp; 	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	bool grammaticalProperNounTemp;	//Used to be called "grammaticalRelexPersonOrStanfordProperNounTemp" //temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	int grammaticalGenderTemp; 	//temporary: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool grammaticalPronounTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	int grammaticalIndexOfDeterminerTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	#endif
	#ifdef GIA_SUPPORT_PREDETERMINERS
	int grammaticalPredeterminerTemp;
	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS
	unordered_map<int, int> grammaticalPredeterminerTempSentenceArray;	//only for instances (not for networkIndexes)	- required for GIA advanced referencing as different references to an entity may well have different predeterminers (eg each)
	#endif
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	string stanfordPOStemp;
	string NormalizedNERtemp;
	string TimexTemp;
	#endif
	int NERTemp;


	/*GIA Translator Temporary Variables*/
	bool isSubjectTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	bool isObjectTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	bool hasSubstanceTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	bool isActionTemp;		//temporary: used for GIA translator only - overwritten every time a new sentence is parsed [10 May 2012: this shouldnt be needed anymore]
	int entityIndexTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Stanford CoreNLP referencing only?)
	int sentenceIndexTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Stanford CoreNLP referencing only?)
	bool isToBeComplimentOfActionTemp;	//required for Relex (linkConditions()/defineSubstancesOfPossessivePrepositions())
	#ifndef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	bool disableParsingAsPrepositionRelationTemp;
	#endif
	bool entityAlreadyDeclaredInThisContext;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	bool hasAssociatedInstanceTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	#ifdef GIA_SUPPORT_ALIASES
	bool isName;
	bool isNameQuery;
	#endif
	#ifdef GIA_SUPPORT_NUMBER_OF
	bool isNumberOf;	//added NLC1j18a/24-September-2014
	#endif
	
	/*Query Variables*/
	bool isQuery;
	bool isWhichOrEquivalentWhatQuery;
	bool isAnswerToQuery;
	bool testedForQueryComparison;
	bool testedForQueryComparisonTemp; //added 17 May 2012 - support better trace routine
	bool queryAnswerContext;
	bool queryEntityTraced;	//temporary for determining max confidence
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	GIAentityNode* entityCorrespondingBestMatch;	 //best match entity node corresponding to this assumed query entity node	//does not take into account multiple diversions/answers [assumes single matches only]
	#endif


	/*GIA Miscellaneous Internal Variables*/
	bool disabled;	//temporary for networkIndex entities: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool permanentNetworkIndex;	//networkIndex entity is to be drawn/saved to XML (if false, this entity has been deemed redundant in semantic network generation)
	bool firstSentenceToAppearInNetwork;
		//CXL:
	bool CXLdummyNode;
		//referencing:
	#ifdef GIA_RECORD_SAME_REFERENCE_SET_INFORMATION
	int referenceSetID;
	int minimumEntityIndexOfReferenceSet;
	#endif
	#ifdef GIA_USE_ADVANCED_REFERENCING
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	bool wasReferenceTemp;
	#endif
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	bool wasReference;
	#endif
	#endif
	#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	bool alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp;	//#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	bool mustSetIsConceptBasedOnApposRelation;
	bool isPronounReference;
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
	bool mustNotSetIsConceptBasedOnPrenomonalModifierRelation;
	#endif
		//subclasses:
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
	bool isSubClass;
	#endif
	bool convertToSubClass;
	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
	bool addSubClass;
	#endif
	#endif
	#endif
		//expletives:
	#ifdef GIA_SUPPORT_EXPLETIVES
	bool isExpletive;	//added 2n7b
	#endif
	
		//databasing:
	#ifdef GIA_USE_DATABASE
	bool added;	//implies database Update is Required
	bool modified;	//implies database Update is Required
	#ifndef GIA_USE_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded;
	#endif
	#endif
		//nlg:
	#ifdef GIA_USE_NLG
	bool parsedForLanguageGeneration;
	bool sourceAddedInLanguageGeneration;
	bool sourceReferencedInLanguageGeneration;
	#endif

	#ifdef USE_NLC
	bool NLCparsedForCodeBlocks;
	//bool parsedForNLCcodeBlocksActionRound;
	//bool parsedForNLCclassHeirarchy;
	bool NLCisSingularArgument;
	bool NLClocalListVariableHasBeenDeclared;	//added NLC 1g8a/11-July-2014
	bool NLClocalListVariableHasBeenInitialised;	//renamed NLC 1g8a/11-July-2014
	bool NLClogicalConditionOperation;		//required as some logical condition operation conditions (prepositions) require sentence context for detection (eg "for each/all/every")
	bool NLCparsedForlogicalConditionOperations;
	bool NLCconjunctionCondition;
	int NLClogicalConditionConjunctionIndex;
	bool NLCcontextGenerated;		//added NLC 1g14b/15-July-2014
	string NLCoriginalNumericalVariableName;	//added NLC 1h1d/24-July-2014
	bool NLCcontextGeneratedTemp;		//added NLC 1l2d/31-October-2014
	bool NLCcategoryListCreatedTemp;	//added NLC 1l10d/06-November-2014
	#ifdef GIA_SUPPORT_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	bool NLCmathTextParsablePhraseEntity;
	#endif
	bool NLCisAlias;
	long NLCidInstanceBackup;
	bool NLCfirstInstanceOfProperNounInContext;
	#endif

	#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	#ifdef GIA_LRP_DETECT_PREPOSITION_TYPE
	string conditionType2;	//added 2h1a/14-November-2014 - required for NLC 1m1a+
	#endif
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS
	bool conditionTwoWay;		//added 2h1e/14-November-2014 - required for NLC 1m1e+
	#ifdef GIA_LRP_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	bool inverseConditionTwoWay;	//added 2h1e/14-November-2014 - required for NLC 1m1e+
	#endif
	#endif
	#endif
};

#define GIA_ENTITY_CHARACTERISTIC_TYPE_BOOL "bool"
#define GIA_ENTITY_CHARACTERISTIC_TYPE_INT "int"
#define GIA_ENTITY_CHARACTERISTIC_TYPE_STRING "string"

class GIAentityCharacteristic
{
public:
	GIAentityCharacteristic();
	GIAentityCharacteristic(string entityCharacteristicNameNew, string entityCharacteristicValueNew);
	GIAentityCharacteristic(string entityCharacteristicNameNew, string entityCharacteristicValueNew, bool isNegativeNew);
	GIAentityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew, int entityPropertyArrayIndexNew);
	GIAentityCharacteristic(string entityPropertyNameNew, string entityPropertyValueNew, int entityPropertyArrayIndexNew, bool isNegativeNew);
	~GIAentityCharacteristic(void);

	string name;
	string value;
	int arrayIndex;
	bool isNegative;	//only used for test (not assign)
};


#ifdef GIA_USE_DATABASE
void DBsetEntityConnectionsReferenceListsLoaded(GIAentityNode* entityNode, bool loaded);
#endif

void disconnectNodeFromAllButDefinitions(GIAentityNode* entityNode);


int calculateQuantityNumberInt(string quantityNumberString);
int calculateQuantityModifierInt(string quantityModifierString);
int calculateQuantityMultiplierInt(string quantityMultiplierString);

string printQuantityNumberString(GIAentityNode* entityNode);


#ifdef GIA_SUPPORT_ALIASES
void convertAliasesStringToAliases(GIAentityNode* entityNode, string aliasesString);
void convertAliasesToAliasesString(GIAentityNode* entityNode, string* aliasesString);
vector<string> explode(const string& str, const char& ch);
string* convertDelimitedStringToArray(string str, char delimiter);
#endif

void deleteEntitiesInEntityNodeList(vector<GIAentityNode*>* entityNodesActiveListComplete);

#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
bool testEntityCharacteristics(GIAentityNode* entity, vector<GIAentityCharacteristic*>* redistributeSpecialCasePropertiesTestVector, bool andOrOr);
	bool testEntityCharacteristic(GIAentityNode* entity, GIAentityCharacteristic* entityCharacteristic);
		void testEntityCharacteristicIterationbool(bool entityVal, GIAentityCharacteristic* entityCharacteristicTest, string iterationVariable, bool* foundMatch);
		void testEntityCharacteristicIterationint(int entityVal, GIAentityCharacteristic* entityCharacteristicTest, string iterationVariable, bool* foundMatch);
		void testEntityCharacteristicIterationstring(string entityVal, GIAentityCharacteristic* entityCharacteristicTest, string iterationVariable, bool* foundMatch);
void setEntityCharacteristics(GIAentityNode* entity, vector<GIAentityCharacteristic*>* redistributeSpecialCasePropertiesAssignmentVector);
	bool setEntityCharacteristic(GIAentityNode* entity, GIAentityCharacteristic* entityCharacteristic);
		void setEntityCharacteristicIterationbool(bool* entityVal, GIAentityCharacteristic* entityCharacteristicSet, string iterationVariable, bool* foundMatch);
		void setEntityCharacteristicIterationint(int* entityVal, GIAentityCharacteristic* entityCharacteristicSet, string iterationVariable, bool* foundMatch);
		void setEntityCharacteristicIterationstring(string* entityVal, GIAentityCharacteristic* entityCharacteristicSet, string iterationVariable, bool* foundMatch);
	#define ENTITY_CHARACTERISTIC_MAX_VALUE_SIZE (100)
	bool getEntityCharacteristic(GIAentityNode* entity, GIAentityCharacteristic* entityCharacteristic);	//fills in entityCharacteristic->value based on entityCharacteristic->name
		void getEntityCharacteristicIterationbool(bool entityVal, GIAentityCharacteristic* entityCharacteristicGet, string iterationVariable, bool* foundMatch);
		void getEntityCharacteristicIterationint(int entityVal, GIAentityCharacteristic* entityCharacteristicGet, string iterationVariable, bool* foundMatch);
		void getEntityCharacteristicIterationstring(string entityVal, GIAentityCharacteristic* entityCharacteristicGet, string iterationVariable, bool* foundMatch);
#endif

#ifndef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
bool isActionSpecialPossessive(GIAentityNode* actionEntity);
#endif

#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
string getParentClassEntityNameFromSubClassEntityName(string subClassEntityName);
string getChildClassEntityNameFromSubClassEntityName(string subClassEntityName);
string createSubClassEntityName(string childEntityName, string parentEntityName);
#endif

bool detectPredeterminerNonReference(GIAentityNode* subjectEntity);

#endif



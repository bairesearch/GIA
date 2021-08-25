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
 * File Name: GIAentityNodeClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3p6b 07-August-2021
 * NB a substance is an instance of an entity, any given entity may contain/comprise/have multiple substances - and substances are unrelated to definitions between entities [they just define what comprises any given entity]
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_ENTITY_NODE_CLASS
#define HEADER_GIA_ENTITY_NODE_CLASS



#include "GIAconditionNodeClass.hpp"
#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#ifdef GIA_NEURAL_NETWORK
#include "ANNneuronClass.hpp"
#endif

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>		//required for GIA_NETWORK_INDEX_ENTITY_NODE_MAP_NOT_VECTOR
#include <utility> // make_pair	//required for GIA_NETWORK_INDEX_ENTITY_NODE_MAP_NOT_VECTOR
using namespace std;

#define GIA_ENTITY_ID_INSTANCE_NETWORK_INDEX_ENTITY (0)
#define GIA_ENTITY_ID_INSTANCE_FIRST_INSTANCE_ENTITY (1)

//entity types added 2n1c
#define GIA_ENTITY_TYPE_UNDEFINED (LRP_SHARED_ENTITY_TYPE_UNDEFINED)
#define GIA_ENTITY_TYPE_NETWORK_INDEX (LRP_SHARED_ENTITY_TYPE_NETWORK_INDEX)
#define GIA_ENTITY_TYPE_SUBSTANCE (LRP_SHARED_ENTITY_TYPE_SUBSTANCE)
#define GIA_ENTITY_TYPE_CONCEPT (LRP_SHARED_ENTITY_TYPE_CONCEPT)
#define GIA_ENTITY_TYPE_ACTION (LRP_SHARED_ENTITY_TYPE_ACTION)
#define GIA_ENTITY_TYPE_CONDITION (LRP_SHARED_ENTITY_TYPE_CONDITION)
#define GIA_ENTITY_TYPE_PROPERTY (LRP_SHARED_ENTITY_TYPE_PROPERTY)
#define GIA_ENTITY_TYPE_DEFINITION (LRP_SHARED_ENTITY_TYPE_DEFINITION)
#define GIA_ENTITY_TYPE_QUALITY (LRP_SHARED_ENTITY_TYPE_QUALITY)
#define GIA_ENTITY_NUMBER_OF_TYPES (LRP_SHARED_ENTITY_NUMBER_OF_TYPES)
#define GIA_ENTITY_TYPE_NETWORK_INDEX_STRING "0"
#define GIA_ENTITY_TYPE_SUBSTANCE_STRING "1"
#define GIA_ENTITY_TYPE_ACTION_STRING "2"
#define GIA_ENTITY_TYPE_CONDITION_STRING "3"
#define GIA_ENTITY_TYPE_CONCEPT_STRING "4"
#define GIA_ENTITY_TYPE_PROPERTY_STRING "5"
#define GIA_ENTITY_TYPE_DEFINITION_STRING "6"
#define GIA_ENTITY_TYPE_QUALITY_STRING "7"
static bool entityTypesIsRelationshipArray[GIA_ENTITY_NUMBER_OF_TYPES] = {false, false, false, true, true, true, true, false};
static bool entityTypesAutomaticallyUpgradeUponInstanceSelectionArray[GIA_ENTITY_NUMBER_OF_TYPES] = {false, true, true, true, true, true, true, false};	//NB this is currently the same as entityTypesIsRelationshipArray except for GIA_ENTITY_TYPE_CONCEPT
static bool entityTypesIsActionOrConditionRelationshipArray[GIA_ENTITY_NUMBER_OF_TYPES] = {false, false, false, true, true, false, false, false};
static bool entityTypesIsPropertyOrDefinitionRelationshipArray[GIA_ENTITY_NUMBER_OF_TYPES] = {false, false, false, false, false, true, true, false};


#define GIA_RELATIONSHIP_ENTITY_NUMBER_OF_TYPES (4)
static int relationshipEntityTypesArray[GIA_RELATIONSHIP_ENTITY_NUMBER_OF_TYPES] = {GIA_ENTITY_TYPE_ACTION, GIA_ENTITY_TYPE_CONDITION, GIA_ENTITY_TYPE_PROPERTY, GIA_ENTITY_TYPE_DEFINITION};
#define GIA_RELATIONSHIP_ENTITY_OFFSET_TO_FIRST_RELATIVE_TYPE_INDEX (GIA_ENTITY_TYPE_ACTION) 	//NB the first relationship entity relative type index is defined as 0

//#ifdef GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_PREPOSITION (0)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_TITLE (1)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_QUOTES (2)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS (3)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER (LRP_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER)	//this is now equivalent to GIA_SYN_REL_TRANSLATOR_STANFORD_PARSER_PREPOSITION_DELIMITER
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_TITLE_DELIMITER "_"
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_QUOTES_DELIMITER "_"	//this filler does not appear compatible with Relex (Stanford only); try another filler character (NB "-" doesn't work with Relex either)
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER "_"
	#define GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_NUMBER_OF_TYPES (4)
	static string concatenationTypesArray[GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_NUMBER_OF_TYPES] = {GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_MULTIWORD_WORD_DELIMITER, GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_TITLE_DELIMITER, GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_QUOTES_DELIMITER, GIA_TRANSLATOR_UNIQUE_CONCATENATION_TYPES_SUBCLASS_DELIMITER};
//#endif

#define GRAMMATICAL_WORD_TYPE_UNDEFINED (0)

#ifdef GIA_PREDETERMINERS
	//added 2i34a
	//must be synced with GIAsynRelTranslatorDefs.h entityPredeterminerSmallArray;
	#define GRAMMATICAL_PREDETERMINER_UNDEFINED (INT_DEFAULT_VALUE)	//-1
	#define GRAMMATICAL_PREDETERMINER_EACH 0
	#define GRAMMATICAL_PREDETERMINER_EVERY 1
	#define GRAMMATICAL_PREDETERMINER_ALL 2
	#define GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES (3)
	static int entityPredeterminerSmallArray[GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES] = {GRAMMATICAL_PREDETERMINER_EACH, GRAMMATICAL_PREDETERMINER_EVERY, GRAMMATICAL_PREDETERMINER_ALL};	//added 2i34a

#endif

#ifdef GIA_ALIASES
	#define GIA_ALIASES_CONVERT_TO_STRING_DELIMITER ';'	//has to be different than attribute delimiter used in GIA filesystem database GIA_DATABASE_ATTRIBUTE_DELIMITER
#endif

#define GRAMMATICAL_TENSE_CONCATONATOR_RELEX "_"

//these have been moved from GIAtranslatorOperations.h as RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_PROPERTIES is used by GIAdraw.cpp
#define RELATION_ENTITY_HAVE "have"
#define RELATION_ENTITY_CAN "can"
#define RELATION_ENTITY_BE "be"	//eg x is y
#define RELATION_ENTITY_IT "it"		//used to handle Relex special case query "What [time] is it?"

#define RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_PROPERTIES "property"	//(RELATION_ENTITY_HAVE)	//implicitPropertyAuxiliary	//implicitHave
#define RELATION_ENTITY_SPECIAL_RELATIONSHIP_NAME_FOR_EFFECTIVE_DEFINITIONS "definition"	//(RELATION_ENTITY_BE)	//implicitDefinitionAuxiliary	//implicitBe

//extracted from wn.h (WordNet 3.0 library header)
#define GRAMMATICAL_WORD_TYPE_UNDEFINED (0)
#define GRAMMATICAL_WORD_TYPE_NOUN (1)
#define GRAMMATICAL_WORD_TYPE_VERB (2)
#define GRAMMATICAL_WORD_TYPE_ADJ (3)
#define GRAMMATICAL_WORD_TYPE_ADV (4)
#define GRAMMATICAL_WORD_TYPE_PREP (5)
#define GRAMMATICAL_WORD_TYPE_SATELLITE (6)	/* not really a part of speech*/
#define GRAMMATICAL_WORD_TYPE_ADJSAT (SATELLITE)
#ifdef GIA_GRAMMATICAL_WORD_TYPES_EXTENDED
#define GRAMMATICAL_WORD_TYPE_CONJUNCTION (7)
#define GRAMMATICAL_WORD_TYPE_DETERMINER (8)
#define GRAMMATICAL_WORD_TYPE_INTERJECTION (9)
#define GRAMMATICAL_WORD_TYPE_PRONOUN (10)
#define GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES (11)
#else
#define GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES (7)
#endif

	//for xml interface:
#define GRAMMATICAL_WORD_TYPE_UNDEFINED_STRING "0"
#define GRAMMATICAL_WORD_TYPE_NOUN_STRING "1"
#define GRAMMATICAL_WORD_TYPE_VERB_STRING "2"
#define GRAMMATICAL_WORD_TYPE_ADJ_STRING "3"
#define GRAMMATICAL_WORD_TYPE_ADV_STRING "4"
#define GRAMMATICAL_WORD_TYPE_PREP_STRING "5"
#define GRAMMATICAL_WORD_TYPE_SATELLITE_STRING "6"	/* not really a part of speech*/
#define GRAMMATICAL_WORD_TYPE_ADJSAT_STRING "6"


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


#define GRAMMATICAL_PRONOUN_PERSON_UNDEFINED 0
#define GRAMMATICAL_PRONOUN_PERSON_FIRST 1
#define GRAMMATICAL_PRONOUN_PERSON_SECOND 2
#define GRAMMATICAL_PRONOUN_PERSON_THIRD 3

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

//must be same as FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME etc (otherwise use //http://public.oed.com/how-to-use-the-oed/abbreviations)
#ifdef GIA_GRAMMATICAL_WORD_TYPES_EXTENDED
static string grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES] = {"undefined", "noun", "verb", "adj", "adv", "prep", "satellite", "conjunction", "det", "interj", "pron"};
#else
static string grammaticalWordTypeNameArray[GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES] = {"undefined", "noun", "verb", "adj", "adv", "prep", "satellite"};
#endif




#define GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES (12)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION (0)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE (1)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION (2)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE (3)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY (4)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE (5)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION (6)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE (7)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT (8)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT (9)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE (10)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE (11)		//NB instanceReverse translates to the "definition" of the instance word
static string entityVectorConnectionNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"action", "actionReverse", "condition", "conditionReverse", "property", "propertyReverse", "definition", "definitionReverse", "relationshipSubject", "relationshipObject", "instance", "instanceReverse"};
static string entityVectorConnectionSourceContextArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", "", "", "", "", "", "", "", ""};
static string entityVectorConnectionContextArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"outgoingAction(s)", "actionReverse(s)", "conditionNode(s)", "conditionReverse(s)", "property(s)", "propertyReverse(s)", "definition(s)", "definitionReverse(s)", "relationshipSubject", "relationshipObject", "instance", "instanceReverse"};
static bool entityVectorConnectionThisIsInstanceAndPreviousNodeWasDefinitionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, false, false, false, false, false, true, false};
static bool entityVectorConnectionIsConditionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, true, true, false, false, false, false, false, false, false, false};
static bool entityVectorConnectionIsBasicArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, false, false, false, true, true, false, true};
#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
static string entityVectorConnectionDrawConnectionNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"subject", "object", "subject", "object", "subject", "object", "subject", "object", "subject", "object", "instance", "instance"};
#else
static string entityVectorConnectionDrawConnectionNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"actionSubject", "actionObject", "conditionSubject", "conditionObject", "property", "property", "definition", "definition", "subject", "object", "instance", "instance"};
#endif
#define GIA_ENTITY_TIME_CONDITION_NODE_NAME "timeCondition"
#define GIA_ENTITY_NODE_NAME "entity"
/*
#define GIA_ENTITY_VECTOR_CONNECTION_SPECIAL_CONDITIONS_HAVING_BEING_TYPES (2)
*/

#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN (-1)
#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
static int inverseVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE}; 
#else
static int inverseVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_RELATIONSHIP_OBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE_REVERSE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_INSTANCE}; 
#endif
static int entityTypesCrossReferenceEntityVectorConnectionArray[GIA_ENTITY_NUMBER_OF_TYPES] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN};
static int entityTypesCrossReferenceEntityVectorConnectionReverseArray[GIA_ENTITY_NUMBER_OF_TYPES] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN, GIA_ENTITY_VECTOR_CONNECTION_TYPE_ACTION_REVERSE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_REVERSE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_REVERSE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE, GIA_ENTITY_VECTOR_CONNECTION_TYPE_UNKNOWN};

static bool entityVectorConnectionIsRelationshipSubjectObjectArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, false, false, false, true, true, false, false};
static bool entityVectorConnectionIsRelationshipForwardArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {true, false, true, false, true, false, true, false, false, false, false, false};
static bool entityVectorConnectionIsRelationshipReverseArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, true, false, true, false, true, false, true, false, false, false, false};
static bool entityVectorConnectionIsRelationshipPropertyOrDefinitionForwardArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, true, false, true, false, false, false, false, false};
static bool entityVectorConnectionIsRelationshipPropertyOrDefinitionReverseArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, true, false, true, false, false, false, false};
static bool entityVectorConnectionIsPropertyOrDefinitionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, true, true, true, true, false, false, false, false};

#ifdef GIA_NEURAL_NETWORK
static bool entityVectorConnectionDirectionArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {true, false, true, false, true, false, true, false, true, false, false, false};
#endif

#define GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES_EQUALITIES (2)
static int entityVectorConnectionEqualitiesArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES_EQUALITIES] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_REVERSE};


#define VECTOR_ASSOCIATED_INSTANCES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define BASIC_DEFINING_INSTANCE_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define BASIC_DEFINING_INSTANCE_RCMOD_INDICATES_SAME_REFERENCE_SET_IRRELEVANT_OR_UNKNOWN (true)
#define GIA_REFERENCE_SET_ID_UNDEFINED (INT_DEFAULT_VALUE)
#define GIA_ENTITY_INDEX_UNDEFINED (INT_DEFAULT_VALUE)
#define GIA_SENTENCE_INDEX_UNDEFINED (INT_DEFAULT_VALUE)

class GIAentityConnection;

#ifdef GIA_DATABASE
#ifndef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
class GIAnetworkIndexEntityLoaded
{
public:
	GIAnetworkIndexEntityLoaded(void);
	~GIAnetworkIndexEntityLoaded(void);

	bool loaded;
	int64_t numberOfInstances;
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
	GIAentityNode(string newEntityName);
	void initialiseEntity();
	~GIAentityNode(void);

	/*GIA Internal Entity Referencing*/
	int64_t idActiveList;
	int64_t idActiveListReorderdIDforXMLsave;	//for CXL output only
	int64_t idInstance; 		//not for networkIndexes (this instance idActiveList of the networkIndex entityName)


	/*GIA Entity Name*/
	string entityName;
	#ifdef GIA_WORD_ORIG
	string wordOrig;		//this needs to be added to XML i/o + file system database i/o
	#endif
	double confidence;		//not currently used
	#ifdef GIA_ALIASES
	vector<string> aliasList;
	#endif


	/*GIA Entity Type*/
	int entityType;
	bool isActionConcept;	//is applied to entityType->GIA_ENTITY_TYPE_ACTION	//added 1t5a to take into account specific actions eg 'eating pies', 'to eat a pie'	
	bool hasAssociatedInstance;	//this boolean appears to only represent whether this entity defines a child substance node [and not whether it contains one]
	bool hasAssociatedTime;
	bool negative;	//if !GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS: for prepositional entities which will be collapsed into conditions only //NB relex does not appear to output this information for properties and actions
	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	bool isArtificialAuxiliary;	//a new entity was created for an implicit auxiliary (property/definition relationship entities only)
	#endif
	
	/*GIA Connections*/
	vector<GIAentityConnection*> entityVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];		//allows for generic coding
	#ifdef GIA_DATABASE
	//designed for a high scale database (eg 200,000 references per instance, 200,000 instances per networkIndex)
	bool entityVectorConnectionsReferenceListLoadedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];			//signifies whether all the vector connections in the reference list has been loaded from file and entityConnections entityNames+idInstance have therefore been populated. This is the first step required to enable loading of connections into RAM (see entityVectorConnectionsLoadedArray)
	//bool entityVectorConnectionsLoadedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];				//not used - vector connections are loaded into RAM on an individual basis. //signifies whether all the vector connection nodes have been loaded (eg from the db)
	bool entityVectorConnectionsRemovedArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES];					//signifies whether one or more vector connection nodes have been removed {ie the entire reference list must be updated}
	#endif
	
	vector<GIAentityConnection*>* actionNodeList;
	vector<GIAentityConnection*>* actionReverseNodeList;
	vector<GIAentityConnection*>* conditionNodeList;
	vector<GIAentityConnection*>* conditionReverseNodeList;
	vector<GIAentityConnection*>* propertyNodeList;
	vector<GIAentityConnection*>* propertyReverseNodeList;
	vector<GIAentityConnection*>* definitionNodeList;
	vector<GIAentityConnection*>* definitionReverseNodeList;
	vector<GIAentityConnection*>* relationshipSubjectEntity;
	vector<GIAentityConnection*>* relationshipObjectEntity;
	vector<GIAentityConnection*>* instanceNodeList;
	vector<GIAentityConnection*>* instanceReverseNodeList;	//NB instanceReverse translates to the "definition" of the instance word
		//time condition connections;
	int conditionType;	//added 25 Sept 11
	GIAtimeConditionNode* timeConditionNode;		//if conditionType == CONDITION_NODE_TYPE_TIME

	/*GIA Special Variables (Quantities/Measures)*/
	bool hasQuantity;
	int quantityNumber;		//eg 6
	string quantityNumberString;	//eg "6:45"
	int quantityModifier;	//not yet implemented
	string quantityModifierString;	//eg "almost"
	bool hasQuantityMultiplier;	//quantity multiplier, _quantity_mult(hundred, three)	//only used by Relex
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
	int grammaticalIndexOfDeterminerTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	#ifdef GIA_PREDETERMINERS
	int grammaticalPredeterminerTemp;
	#ifdef GIA_ADVANCED_REFERENCING_SUPPORT_REFERENCING_OF_ENTITIES_WITH_PREDETERMINERS
	unordered_map<int, int> grammaticalPredeterminerTempSentenceArray;	//only for instances (not for networkIndexes)	- required for GIA advanced referencing as different references to an entity may well have different predeterminers (eg each)
	#endif
	#endif
	string stanfordPOStemp;
	#ifdef GIA_STANFORD_CORENLP
	string NormalizedNERtemp;
	string TimexTemp;
	#endif
	int NERTemp;


	/*GIA Translator Temporary Variables*/
	int entityIndexTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Stanford CoreNLP referencing only?)
	int sentenceIndexTemp;		//temporary: used for GIA translator reference paser only - overwritten every time a new textual context (eg paragraph) is parsed (used for Stanford CoreNLP referencing only?)
	bool isToBeComplimentOfActionTemp;	//required for Relex (linkConditions()/defineSubstancesOfPossessivePrepositions())
	#ifndef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
	bool disableParsingAsPrepositionRelationTemp;
	#endif
	bool entityAlreadyDeclaredInThisContext;	//temporary: used for GIA translator reference paser only - cleared every time a new context (eg paragraph/manuscript) is parsed
	bool hasAssociatedInstanceTemp;	//temporary: used for GIA translator only - overwritten every time a new sentence is parsed
	#ifdef GIA_ALIASES
	bool isName;
	bool isNameQuery;
	#endif
	#ifdef GIA_TRANSLATOR_NUMBER_OF
	bool isNumberOf;	//added NLC1j18a/24-September-2014
	#endif
	
	/*Query Variables*/
	bool isQuery;
	bool isWhichOrEquivalentWhatQuery;
	bool isAnswerToQuery;
	bool testedForQueryComparison;
	bool testedForQueryComparisonTemp; //added 17 May 2012 - support better trace routine
	bool testedForQueryComparisonTemp2;	//for GIA_QUERY_TRACE_ENTITY_NODES_FUNCTION_DETERMINE_MAX_NUMBER_MATCHED_NODES_SAME_SET_ONLY
	bool queryAnswerContext;
	bool queryEntityTraced;	//temporary for determining max confidence
	GIAentityNode* entityCorrespondingBestMatch;	 //best match entity node corresponding to this assumed query entity node	//does not take into account multiple diversions/answers [assumes single matches only]


	/*GIA Miscellaneous Internal Variables*/
	bool disabled;	//temporary for networkIndex entities: used for GIA translator reference paser only - overwritten every time a new sentence is parsed
	bool permanentNetworkIndex;	//networkIndex entity is to be drawn/saved to XML (if false, this entity has been deemed redundant in semantic network generation)
		//CXL:
	bool CXLdummyNode;
		//referencing:
	int referenceSetID;
	int minimumEntityIndexOfReferenceSet;
	#ifdef GIA_ADVANCED_REFERENCING
	#ifdef GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	bool wasReferenceTemp;
	#endif
	#ifdef GIA_RECORD_WAS_REFERENCE_INFORMATION
	bool wasReference;
	#endif
	#endif
	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES
	bool alreadyAssignedSubstancesBasedOnDeterminatesOfDefinitionEntitiesTemp;	//#ifdef GIA_SYN_REL_TRANSLATOR_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	bool mustSetIsConceptBasedOnApposRelation;
	bool isPronounReference;
	#ifdef GIA_SYN_REL_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_ENSURE_DEPENDENT_IS_NOT_ASSIGNED_CONCEPT
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
	#ifdef GIA_TRANSLATOR_EXPLETIVES
	bool isExpletive;	//added 2n7b
	#endif
	
		//databasing:
	#ifdef GIA_DATABASE
	bool added;	//implies database Update is Required
	bool modified;	//implies database Update is Required
	#ifndef GIA_DATABASE_ALWAYS_LOAD_NETWORK_INDEX_NODE_REFERENCE_LISTS
	GIAnetworkIndexEntityLoaded* networkIndexEntityLoaded;
	#endif
	#endif
		//nlg:
	#ifdef GIA_NLG
	bool parsedForLanguageGeneration;
	bool sourceAddedInLanguageGeneration;
	bool sourceReferencedInLanguageGeneration;
	#endif

	#ifdef GIA_ADD_ARTIFICIAL_AUXILIARY_FOR_ALL_PROPERTIES_AND_DEFINITIONS
	#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	bool isAlias;
	#endif
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
	#ifdef GIA_NLC_INTEGRATION_DISABLE_ADVANCED_REFERENCING_FOR_LOGICAL_CONDITIONS_CONCEPTS
	bool NLCmathTextParsablePhraseEntity;
	#endif
	bool NLCisAlias;
	int64_t NLCidInstanceBackup;
	bool NLCfirstInstanceOfProperNounInContext;
	#endif

	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	#ifdef LRP_PREPROCESSOR_WORD_DETECT_PREPOSITION_TYPE
	string conditionType2;	//added 2h1a/14-November-2014 - required for NLC 1m1a+
	#endif
	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS
	bool conditionTwoWay;		//added 2h1e/14-November-2014 - required for NLC 1m1e+
	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_TWOWAY_PREPOSITIONS_DUAL_CONDITION_LINKS_ENABLED
	bool inverseConditionTwoWay;	//added 2h1e/14-November-2014 - required for NLC 1m1e+
	#endif
	#endif
	#endif
	
	#ifdef GIA_CREATE_SHORTCUTS_TO_CONCEPT_ENTITIES
	//defined for network index entities only;
	GIAentityNode* shortcutToNonspecificConceptEntity;
	vector<GIAentityNode*> shortcutsToSpecificConceptEntities;
	#ifdef GIA_NEURAL_NETWORK
	ANNneuron* entityShortcutToConceptNeuron;
	#endif
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR
	bool isLogicReferenceEntity;
	int logicReferenceClass;
	string logicReferenceClassType;
	#endif
	
	#ifdef GIA_NEURAL_NETWORK
	bool parsedForANNgeneration;
	/*
	#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	int conceptIndexType;
	int conceptIndex;
	#endif
	*/
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_RULES
	string semanticRelationReturnFunctionName; 
	int semanticRelationReturnFunctionNameIndexType;
	//bool semanticRelationReturnFunctionNameSameReferenceSet;
	int semanticRelationPreprocessorEntityIndex;	//NB could rely on entityIndexTemp instead
	#ifdef SANI_SEMANTICALLY_DETERMINED_DYNAMIC_CONNECTIONS
	bool semanticRelationReturnConnectionDynamic;
	#endif
	#endif
	int semanticRelationWordPOStypeInferred;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	string semanticRelationWordDeterminer;
	string semanticRelationWordPredeterminer;
	bool semanticRelationEntityIsReferenced;
	vector<string> multiwordAuxiliaryList;	
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



class GIAentityNodeClassClass
{
	private: SHAREDvarsClass SHAREDvars;
	#ifdef GIA_DATABASE
	public: void DBsetEntityConnectionsReferenceListsLoaded(GIAentityNode* entityNode, bool loaded);
	#endif

	public: void disconnectNodeFromAllButDefinitions(const GIAentityNode* entityNode);


	public: int calculateQuantityNumberInt(const string quantityNumberString);
	private: int calculateQuantityModifierInt(const string quantityModifierString);
	public: int calculateQuantityMultiplierInt(const string quantityMultiplierString);

	public: string printQuantityNumberString(const GIAentityNode* entityNode);


	#ifdef GIA_ALIASES
	public: void convertAliasesStringToAliases(GIAentityNode* entityNode, string aliasesString);
	public: void convertAliasesToAliasesString(GIAentityNode* entityNode, string* aliasesString);
	private: vector<string> explode(const string& str, const char& ch);
	public: string* convertDelimitedStringToArray(const string str, const char delimiter);
	#endif

	public: void deleteEntitiesInEntityNodeList(vector<GIAentityNode*>* entityNodesActiveListComplete);

	#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
	public: bool testEntityCharacteristics(const GIAentityNode* entity, vector<GIAentityCharacteristic*>* redistributeSpecialCasePropertiesTestVector, const bool andOrOr);
		public: bool testEntityCharacteristic(const GIAentityNode* entity, const GIAentityCharacteristic* entityCharacteristic);
			private: void testEntityCharacteristicIterationbool(const bool entityVal, const GIAentityCharacteristic* entityCharacteristicTest, const string iterationVariable, bool* foundMatch);
			private: void testEntityCharacteristicIterationint(const int entityVal, const GIAentityCharacteristic* entityCharacteristicTest, const string iterationVariable, bool* foundMatch);
			private: void testEntityCharacteristicIterationstring(const string entityVal, const GIAentityCharacteristic* entityCharacteristicTest, const string iterationVariable, bool* foundMatch);
	public: void setEntityCharacteristics(GIAentityNode* entity, vector<GIAentityCharacteristic*>* redistributeSpecialCasePropertiesAssignmentVector);
		public: bool setEntityCharacteristic(GIAentityNode* entity, GIAentityCharacteristic* entityCharacteristic);
			private: void setEntityCharacteristicIterationbool(bool* entityVal, const GIAentityCharacteristic* entityCharacteristicSet, const string iterationVariable, bool* foundMatch);
			private: void setEntityCharacteristicIterationint(int* entityVal, const GIAentityCharacteristic* entityCharacteristicSet, const string iterationVariable, bool* foundMatch);
			private: void setEntityCharacteristicIterationstring(string* entityVal, const GIAentityCharacteristic* entityCharacteristicSet, const string iterationVariable, bool* foundMatch);
		#define ENTITY_CHARACTERISTIC_MAX_VALUE_SIZE (100)
		public: bool getEntityCharacteristic(const GIAentityNode* entity, GIAentityCharacteristic* entityCharacteristic);	//fills in entityCharacteristic->value based on entityCharacteristic->name
			private: void getEntityCharacteristicIterationbool(const bool entityVal, GIAentityCharacteristic* entityCharacteristicGet, const string iterationVariable, bool* foundMatch);
			private: void getEntityCharacteristicIterationint(const int entityVal, GIAentityCharacteristic* entityCharacteristicGet, const string iterationVariable, bool* foundMatch);
			private: void getEntityCharacteristicIterationstring(const string entityVal, GIAentityCharacteristic* entityCharacteristicGet, const string iterationVariable, bool* foundMatch);
		#endif

	#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
	public: string getParentClassEntityNameFromSubClassEntityName(string subClassEntityName);
	public: string getChildClassEntityNameFromSubClassEntityName(const string subClassEntityName);
	public: string createSubClassEntityName(const string childEntityName, const string parentEntityName);
	#endif

	public: bool detectPredeterminerNonReference(const GIAentityNode* entity);
	
	public: bool entityIsRelationship(const GIAentityNode* entity);
	public: int getRelationshipEntityRelativeTypeIndex(const GIAentityNode* entity);

};

#endif



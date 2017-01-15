/*******************************************************************************
 *
 * File Name: GIATranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR
#define HEADER_GIA_TRANSLATOR




#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


					
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAConditionNodeClass.h"


#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS
#define GIA_ASSIGN_UNIQUE_ACTION_NODE_TO_EACH_ACTION_INSTANCE_OF_A_GIVEN_ACTION_NAME (1)
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_DEFINITIVE_NOUNS (1)		//NB must make this 0 for large.xml to work (NB this bug was issue introduced after GIA Archive - 1a5d - 04May11a, eg GIA Archive - 1a5e - 04May11a)
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_NOUNS_WITH_DETERMINATES (1)
#define GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PERSONS_OR_DATES (1)
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_PRONOUNS (1)		//IMPORTANT
#define GIA_ALWAYS_ASSIGN_NEW_INSTANCE_PROPERTY_WHEN_DEFINING_A_PROPERTY (0)
#define GIA_ALWAYS_ASSIGN_NEW_INSTANCE_PROPERTY_WHEN_ATTACHING_A_PROPERTY (0)
#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY (1)
#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED (1)


#define SUBJECT_INDEX (0)
#define OBJECT_INDEX (1)

#define MAX_NUMBER_OF_RELATIONS_PER_SENTENCE (50)
#define MAX_NUMBER_OF_WORDS_PER_SENTENCE (50)
		
#define SECONDS_IN_YEAR (365*24*60*60)



//Relations;
//?? NB all of these cases/types need to be replaced with more complex grammar requirements (eg "on" can also mean "rides his bike on the road" [location], not just "rides his bike on tuesday" [time])

//Properties:	[NB properties are attached to either another property or a straight entity);]
//properties (derived from obj/subj relationships);					
#define RELATION_FUNCTION_COMPOSITION_1 "contains"	//eg x contains y
#define RELATION_FUNCTION_COMPOSITION_2 "comprises"
#define RELATION_FUNCTION_COMPOSITION_3 "has"	
#define RELATION_FUNCTION_COMPOSITION_4 "have"
#define RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES (4)						
//properties (descriptive relationships)
#define RELATION_TYPE_ADJECTIVE_1 "_amod"		//eg x is happy
#define RELATION_TYPE_ADJECTIVE_2 "_predadj"						
#define RELATION_TYPE_ADJECTIVE_3 "_advmod"
//properties (possessive relationships)
#define RELATION_TYPE_POSSESSIVE "_poss"	//eg his bike	[bike him]		/its bike
#define RELATION_TYPE_PRENOMIAL_MODIFIER "_nn"

//entities (concepts);					
#define RELATION_FUNCTION_DEFINITION_1 "be"	//eg x is y
#define RELATION_TYPE_APPOSITIVE_OF_NOUN "_appo"

//actions (obj/subj relationships)
#define RELATION_TYPE_OBJECT "_obj"			//eg eats y	[? be y]
#define RELATION_TYPE_OBJECT_THAT "_that"		//there is a place that we go
#define RELATION_TYPE_INDIRECT_OBJECT "_iobj"
#define RELATION_TYPE_SUBJECT "_subj"	//eg x eats 	[? be x]
#define RELATION_TYPE_SUBJECT_EXPLETIVE "_expl"		//eg goes there	//NB "there" is currently interpreted as a subject of an action


//
#define RELATION_TYPE_COMPLIMENT_TO_BE "_to-be"		//eg grows tired / The rose smelled sweet / _to-be(smell, sweet) - CHECK THIS
#define RELATION_TYPE_COMPLIMENT_TO_DO "_to-do"		//eg Linas likes to row / _to-do(like, row) - CHECK THIS
#define RELATION_TYPE_COMPLEMENTS_NUMBER_OF_TYPES (2)

//dates, measures, quantities
#define RELATION_TYPE_DATE_DAY "_date_day" 
#define RELATION_TYPE_DATE_YEAR "_date_year"
#define RELATION_TYPE_MEASURE_DISTANCE "_measure_distance" 
#define RELATION_TYPE_MEASURE_PER "_measure_per"
#define RELATION_TYPE_MEASURE_SIZE "_measure_size" 
#define RELATION_TYPE_MEASURE_TIME "_measure_time"
#define RELATION_TYPE_QUANTITY "_quantity"
#define RELATION_TYPE_QUANTITY_MOD "_quantity_mod"
#define RELATION_TYPE_QUANTITY_MULT "_quantity_mult"
	//? DOING NOW: references: yet to integrate - see http://wiki.opencog.org/w/Ideas#Improved_reference_resolution for integration (also check for the existence of the "person" tag in the feature "tense" data block)
	//? #define RELATION_TYPE_QUANTITY "_quantity"	//eg his bike	[bike him]		/its bike

#define RELATION_TYPE_OBJECT_SPECIAL_CONDITION_NUMBER_OF_TYPES (1)

//predicates????
//action/property conditions
#define RELATION_TYPE_PREPOSITION_ON "on"		//eg rides on tuesday		[ride tuesday]		//this forms the action condition; "when"
#define RELATION_TYPE_PREPOSITION_AT "at"		//eg rides at the palace	[ride palace]	//this forms the action condition; "where"
#define RELATION_TYPE_PREPOSITION_TO "to"		//eg rides to the shops 			//this forms the action condition; "where"	
#define RELATION_TYPE_PREPOSITION_LOCATION_OR_TIME_NUMBER_OF_TYPES (3)
#define RELATION_TYPE_PREPOSITION_WHEN "when"	//eg joe fires his bow when john drives fast.	[fire drive]	//this forms the action condition; "when" [not time, but in association with another action]
#define RELATION_TYPE_PREPOSITION_BECAUSE "because"
#define RELATION_TYPE_PREPOSITION_ACTION_OR_PROPERTY_NUMBER_OF_TYPES (2)
#define RELATION_TYPE_PREPOSITION_FIRST_CHARACTER '_'
#define RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION "_pobj"
#define RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION "_psubj"

//?#define RELATION_TYPE_OF "of"		//eg [she grew tired] of it	{same as with it}

//action/property reasons [???]
#define RELATION_TYPE_PREPOSITION_SUCH_THAT "such"
#define RELATION_TYPE_PREPOSITION_SO "so"
#define RELATION_TYPE_PREPOSITION_REASON_NUMBER_OF_TYPES (2)

#define RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES (4)
#define RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_MEASURE_NUMBER_OF_TYPES (3)

#define RELATION_TYPE_OBJECT_NUMBER_OF_TYPES (5)
#define RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES (2)
#define RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES (3)
#define RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES (2)
/* ORIGINAL v1a;
#define RELATION_TYPE_OBJECT_NUMBER_OF_TYPES (2)
#define RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES (3)
#define RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES (1)
*/

#define RELATION_FUNCTION_DEFINITION_NUMBER_OF_TYPES (1)
#define GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES (2)
#define RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES (1)

#define REFERENCE_TYPE_LOCATION "there"				//_advmod
	//pronouns
#define REFERENCE_TYPE_POSSESSIVE_UNDEFINED 0
#define REFERENCE_TYPE_POSSESSIVE_MASCULINE 1		//his
#define REFERENCE_TYPE_POSSESSIVE_FEMININE 2		//her
#define REFERENCE_TYPE_POSSESSIVE_PLURAL 3		//them/their
#define REFERENCE_TYPE_POSSESSIVE_NEUTER 4		//its
#define REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES (5)
	//definite pronouns
#define REFERENCE_TYPE_PERSON_UNDEFINED 0
#define REFERENCE_TYPE_PERSON_MASCULINE 1	//he
#define REFERENCE_TYPE_PERSON_FEMININE 2	//she
#define REFERENCE_TYPE_PERSON_PLURAL 3		//they
#define REFERENCE_TYPE_PERSON_NEUTER 4		//it
#define REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES (5)

#define REFERENCE_TYPE_UNDEFINED 0

/* is this required?
#define REFERENCE_TYPE_POSSESSIVE_THEIR_OR_THEM "them"	
#define REFERENCE_TYPE_POSSESSIVE_HIS "his"
#define REFERENCE_TYPE_POSSESSIVE_HER "her"
#define REFERENCE_TYPE_POSSESSIVE_OR_QUANTITY_ITS "its"
#define REFERENCE_TYPE_PERSON_PLURAL_THEY "they"	
#define REFERENCE_TYPE_PERSON_PLURAL_THEY "he"
#define REFERENCE_TYPE_PERSON_PLURAL_THEY "she"
#define REFERENCE_TYPE_PERSON_PLURAL_THEY "it"
*/



/*
noun = joe
verb = rides
adjective = happy
*/

long maximumLong(long a, long b);

void convertSentenceRelationsIntoGIAnetworkNodes(vector<GIAEntityNode*> *indexOfEntityNodes, vector<string> *indexOfEntityNames, vector<GIATimeConditionNode*> *indexOfTimeNodes, vector<long> *indexOfTimeNumbers, Sentence * firstSentenceInList);

GIAEntityNode * findOrAddEntityNodeByName(vector<GIAEntityNode*> *indexOfEntityNodes, vector<string> *indexOfEntityNames, string * entityNodeName, bool * found, long * index, bool addIfNonexistant);
GIATimeConditionNode * findOrAddTimeNodeByNumber(vector<GIATimeConditionNode*> *indexOfTimeNodes, vector<long> *indexOfTimeNumbers, long * timeNodeNumber, bool * found, long * index, bool addIfNonexistant);

void addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity);
void addPropertyToPropertyDefinition(GIAEntityNode * propertyEntity);
void addActionToActionDefinition(GIAEntityNode * actionEntity);

void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense);

void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity);

void addActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity);
	GIAEntityNode * addAction(GIAEntityNode * actionEntity);
	void addActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity);
	void addActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity);

void addOrConnectPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionEntityNode, string conditionName);
	void addLocationConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * locationConditionEntity, string conditionName);
	void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, string conditionName);
	void addPropertyConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity, string conditionName);

	//property to property relationship - these they in actual fact represent different levels of detail in information to property to action / action to action nodes - direct property to property relationships are missing the action/connectivity information


vector<GIAEntityNode*> * getTranslatorEntityNodesCompleteList();

long maxLong(long a, long b);

#endif

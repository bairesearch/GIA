/*******************************************************************************
 *
 * File Name: GIATranslator.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1f1c 23-Jan-2012
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

//#define GIA_TRANSLATOR_DEBUG




//#define GIA_DEBUG_ENABLE_REDUNDANT_TO_DO_PROPERTY_CONNECTIONS_TO_DEMONSTRATE_DRAW_FAILURE


#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS	//default: enabled
//#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_CLEAR_REFERENCES_EVERY_SENTENCE	//default: disabled




//#define GIA_TRANSLATOR_USE_NEW_ACTION_SUBJECT_RELATION_FUNCTION_DEFINITION_IMPLEMENTATION	//if undefined then "tom is being an idiot"= an instance of tom is an idoit. if defined then tom has (a property) of idiocy [NOT YET IMPLEMENTED]

//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TOBE_AND_SUBJECT_RELATION_AS_PROPERTY_LINK_AND_ACTION_DEFINITION
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO	//NB this relates to GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_2A_GRAMMAR_TREAT_PRESENT_PERFECT_AS_PAST_TENSE
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3A_PREPOSITIONS_INTERPRET_PREPOSITION_OF_AS_EITHER_CONDITION_OR_PROPERTY_LINK_DEPENDING_UPON_ACTION_OR_PROPERTY
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
	#define GIA_IGNORE_MEANINGLESS_RELATIONS
#endif
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
	#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
#endif
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
	#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
	#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
		#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_CONDITION_PROPERTY
		#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_CONDITION_DEFINITION
	#else
		#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY
		#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION

	#endif
#endif
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
	#define GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
#endif

#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_DEFINITION_BASIC

#define GIA_TRANSLATOR_ACTION_DEFINITION_CODE_SIMPLIFICATION

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
	#define RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES (3)
	#define RELATION_TYPE_PREPOSITION_REDUCTION_MAX_NUMBER_VARIATIONS (4)
	#define RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_NUMBER_OF_TYPES (4)
	#define RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING ((string)"a`")	//à: character not supported by ASCII
	#define RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_NUMBER_OF_TYPES (3)
	#define RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING ((string)"de")
	#define RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_NUMBER_OF_TYPES (2)
	#define RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_REPLACEMENT_STRING ((string)"pour")
#endif


#define GIA_ASSIGN_UNIQUE_ACTION_NODE_TO_EACH_ACTION_INSTANCE_OF_A_GIVEN_ACTION_NAME (1)
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_DEFINITIVE_NOUNS (1)		//NB must make this 0 for large.xml to work (NB this bug was issue introduced after GIA Archive - 1a5d - 04May11a, eg GIA Archive - 1a5e - 04May11a)
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_NOUNS_WITH_DETERMINATES (1)
#define GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PERSONS_OR_DATES (1)
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_PRONOUNS (1)		//IMPORTANT
//#define GIA_ALWAYS_ASSIGN_NEW_INSTANCE_PROPERTY_WHEN_DEFINING_A_PROPERTY
//#define GIA_ALWAYS_ASSIGN_NEW_INSTANCE_PROPERTY_WHEN_ATTACHING_A_PROPERTY

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
#define RELATION_FUNCTION_COMPOSITION_2 "comprise"
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

#define RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_NEGATIVE_CONTEXT_1 "not"

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


#define RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_OBJECT_SPECIAL_TO_DO_PROPERTY_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_OBJECT_SPECIAL_TO_BE_PROPERTY_NUMBER_OF_TYPES (1)

//action/property conditions: prepositions [predicates????]
#define RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES (18)
#define RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES (35)
#define RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES (6)
#define RELATION_TYPE_PREPOSITION_FIRST_CHARACTER '_'
#define RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION "_pobj"
#define RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION "_psubj"


//#define ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION	//in final implementation, the arbitrary subject should be determined during the referencing stage of sentence parsing
#ifndef ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION
	#define ARBITRARY_SUBJECT_SPECIAL_CONCEPT_NODE_NAME "arbitrarySubject"
#endif

//prepositions are now added explicitly
/*
#define RELATION_TYPE_PREPOSITION_ON "on"		//eg rides on tuesday		[ride tuesday]		//this forms the action condition; "when"
#define RELATION_TYPE_PREPOSITION_AT "at"		//eg rides at the palace	[ride palace]	//this forms the action condition; "where"
#define RELATION_TYPE_PREPOSITION_TO "to"		//eg rides to the shops 			//this forms the action condition; "where"	
*/
/*
#define RELATION_TYPE_PREPOSITION_WHEN "when"	//eg joe fires his bow when john drives fast.	[fire drive]	//this forms the action condition; "when" [not time, but in association with another action]
#define RELATION_TYPE_PREPOSITION_BECAUSE "because"
*/


#define RELATION_TYPE_OF "of"		//eg [she grew tired] of it	 "She grew tired of the pie."  / "The house of Kriton is blue."	//detect if function and argument are both nouns/property entities; if so then create a property connection. if a function is a verb/action, then create a condition connection.

//action/property reasons [???]
#define RELATION_TYPE_PREPOSITION_SUCH_THAT "such"
#define RELATION_TYPE_PREPOSITION_SO "so"
#define RELATION_TYPE_PREPOSITION_REASON_NUMBER_OF_TYPES (2)

#define RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES (4)
#define RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES (2)
#define RELATION_TYPE_MEASURE_NUMBER_OF_TYPES (4)
#define RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES (1)

#define RELATION_TYPE_OBJECT_NUMBER_OF_TYPES (2)
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

//questions;
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN "_%atTime"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE "_%atLocation"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY "_%because"
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE "_$qVar"
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_ARGUMENT_INDEX (MAX_NUMBER_OF_WORDS_PER_SENTENCE-1)
//#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_QUANTITY_NUMBER_REPLACEMENT -9999
#define REFERENCE_TYPE_QUESTION_WHEN_CONTEXT_NUMBER_OF_TYPES (15)	//Eg what is the time?
#define REFERENCE_TYPE_QUESTION_WHERE_CONTEXT_NUMBER_OF_TYPES (3)	//Eg what is the location?
#define REFERENCE_TYPE_QUESTION_WHY_CONTEXT_NUMBER_OF_TYPES (3)		//Eg what is the reason?

#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE_REPLACEMENT "RELATION_TYPE_PREPOSITION_AT"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN_REPLACEMENT "RELATION_TYPE_PREPOSITION_AT"	//must also set hasAssociatedTime to true
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY_REPLACEMENT "RELATION_TYPE_PREPOSITION_BECAUSE"


/*
noun = joe
verb = rides
adjective = happy
*/


void convertSentenceRelationsIntoGIAnetworkNodes(unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, vector<GIATimeConditionNode*> *timeConditionNodesList, vector<long> *timeConditionNumbersList, Sentence * firstSentenceInList);

	//Pass A
	void fillGrammaticalArrays(Sentence * currentSentenceInList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[]);
	void locateAndAddAllConceptEntities(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[]);
	void identifyComparisonVariableAlternateMethod(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);	//unordered_map<string, GIAEntityNode*> *conceptEntityNodesList
	void switchArgumentsAndFunctionsWhereNecessary(Sentence * currentSentenceInList);
	void identifyEntityTypes(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void linkReferences(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList, bool GIAEntityNodeIsDate[], int GIAEntityNodeGrammaticalTenseArray[], bool GIAEntityNodeGrammaticalTenseModifierArray[], int GIAEntityNodeGrammaticalNumberArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], int GIAEntityNodeGrammaticalGenderArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[]);

	//Pass B
	//0
	void collapseRedundantRelationAndMakeNegative(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void definePropertiesObjectsAndSubjectsWithProperties(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
	void definePropertiesDefiniteNouns(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeGrammaticalIsDefiniteArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], bool GIAEntityNodeIsDate[], bool GIAEntityNodeIsAReference[]);
	void definePropertiesNounsWithDeterminates(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], int referenceTypeHasDeterminateCrossReferenceNumberArray[], bool GIAEntityNodeGrammaticalIsPersonArray[], bool GIAEntityNodeIsDate[], bool GIAEntityNodeIsAReference[]);
	void definePropertiesNounsWithAdjectives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void definePropertiesQuantitiesAndMeasures(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void definePropertiesQuantityModifiers(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void definePropertiesExpletives(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void definePropertiesPronouns(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], bool GIAEntityNodeGrammaticalIsPronounArray[], bool GIAEntityNodeIsAReference[]);
	void definePropertiesToBe(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void definePropertiesToDo(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	bool isAdjectiveNotConnectedToObjectOrSubject(Sentence * currentSentenceInList, Relation * currentRelationInList);
	void definePropertiesHasTime(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
	//1-3
	void linkPropertiesPossessiveRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void linkPropertiesDescriptiveRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void linkEntityDefinitionsAppositiveOfNouns(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void defineSubjectObjectRelationships(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList);
	void defineIndirectObjects(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	void defineObjectSubjectOfPreposition(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[]);
	//4
	void defineActionPropertyConditions(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList);
		void createConditionBasedUponPreposition(GIAEntityNode * actionOrPropertyEntity, GIAEntityNode * actionOrPropertyConditionEntity, string relationType, bool negative, unordered_map<string, GIAEntityNode*> *conceptEntityNodesList);
			void addTimeConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * timeConditionEntity, GIAEntityNode * conditionTypeConceptEntity);
			void addLocationConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * locationConditionEntity, GIAEntityNode * conditionTypeConceptEntity);
			void addReasonConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * reasonConditionEntity, GIAEntityNode * conditionTypeConceptEntity);
			void addPropertyConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity, GIAEntityNode * conditionTypeConceptEntity);
	void extractDates(Sentence * currentSentenceInList, bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
	void extractQuantities(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList);
	void extractMeasures(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList);
	void defineToBeAndToDoProperties(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList);
	void extractQualities(Sentence * currentSentenceInList, GIAEntityNode * GIAEntityNodeArray[], unordered_map<string, GIAEntityNode*> *conceptEntityNodesList);

bool isAdjectiveNotAnAdvmodAndRelationFunctionIsNotBe(Relation * currentRelationInList, GIAEntityNode * GIAEntityNodeArray[], int relationFunctionIndex);

void addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity);
void addPropertyToPropertyDefinition(GIAEntityNode * propertyEntity);
	GIAEntityNode * addProperty(GIAEntityNode * propertyEntity);
	

void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense, bool isProgressive);

void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity);

void addActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity);
	void addActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity);
	void addActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity);
		GIAEntityNode * addActionToActionDefinition(GIAEntityNode * actionEntity);
			GIAEntityNode * addAction(GIAEntityNode * actionEntity);
		
void addOrConnectPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionEntityNode, GIAEntityNode * conditionTypeConceptEntity);
void addOrConnectBeingDefinitionConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionDefinitionNode, GIAEntityNode * conditionTypeConceptEntity, bool negative);
void addOrConnectHavingPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionPropertyNode, GIAEntityNode * conditionTypeConceptEntity, bool negative);
	void addConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity, GIAEntityNode * conditionTypeConceptEntity);
		GIAEntityNode * addCondition(GIAEntityNode * conditionEntity);

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
string performPrepositionReduction(string relationType);
#endif




	//property to property relationship - these they in actual fact represent different levels of detail in information to property to action / action to action nodes - direct property to property relationships are missing the action/connectivity information


void setTranslatorEntityNodesCompleteList(vector<GIAEntityNode*> * newEntityNodesCompleteList);
//void setTranslatorConceptEntityNodesList(vector<GIAEntityNode*> * newConceptEntityNodesList);
void setTranslatorPropertyEntityNodesList(vector<GIAEntityNode*> * newPropertyEntityNodesList);
void setTranslatorActionEntityNodesList(vector<GIAEntityNode*> * newActionEntityNodesList);
void setTranslatorConditionEntityNodesList(vector<GIAEntityNode*> * newConditionEntityNodesList);

bool getFoundComparisonVariable();
GIAEntityNode* getComparisonVariableNode();




#endif

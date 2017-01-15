/*******************************************************************************
 *
 * File Name: GIATranslatorOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i9a 11-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_OPERATIONS
#define HEADER_GIA_TRANSLATOR_OPERATIONS

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
#include <vector>
using namespace std;


#include "GIAglobalDefs.h"					
#include "GIASentenceClass.h"
#include "GIAEntityNodeClass.h"
#include "GIAConditionNodeClass.h"



#ifndef GIA_DISABLE_REFERENCING
	#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS	//default: enabled
#endif

#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
	#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_CLEAR_REFERENCES_EVERY_PARAGRAPH	//default: enabled		//Relex Only
#else
	#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_CLEAR_REFERENCES_EVERY_SENTENCE	//default: disabled
#endif
//#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_CLEAR_REFERENCES_EVERY_SENTENCE	//default: disabled		//Relex and Stanford supported (untested with stanford)



#define USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_CONCEPT_ENTITY_IN_A_GIVEN_SENTENCE
//#define USE_OLD_SUBJ_OBJ_ONLY_ONE_PAIR_RESTRICTION_METHOD 	//default: disabled

//#define GIA_DEBUG_ENABLE_REDUNDANT_TO_DO_PROPERTY_CONNECTIONS_TO_DEMONSTRATE_DRAW_FAILURE


#define GIA_DEAL_WITH_TEMPORARY_ISSUE
#ifdef GIA_DEAL_WITH_TEMPORARY_ISSUE
	#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION				//EITHER this is required
	//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B2_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY	//OR this is required
#endif

//#define GIA_TRANSLATOR_USE_NEW_ACTION_SUBJECT_RELATION_FUNCTION_DEFINITION_IMPLEMENTATION	//if undefined then "tom is being an idiot"= an instance of tom is an idoit. if defined then tom has (a property) of idiocy [NOT YET IMPLEMENTED]

//#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION


//Stanford/Relex independent conditions:
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
	#define GIA_IGNORE_MEANINGLESS_RELATIONS
#endif
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_PROPERTY
//#define GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES					
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO	//NB this relates to GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS

#ifdef GIA_USE_RELEX

	//GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE:
	
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3A_PREPOSITIONS_INTERPRET_PREPOSITION_OF_AS_EITHER_CONDITION_OR_PROPERTY_LINK_DEPENDING_UPON_ACTION_OR_PROPERTY
	
	//GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE:

		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TOBE_AND_SUBJECT_RELATION_AS_PROPERTY_LINK_AND_ACTION_DEFINITION
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED	//Relex only at present [relies upon isAdjectiveNotAnAdvmodAndRelationFunctionIsNotBe() and GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS]
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_2A_GRAMMAR_TREAT_PRESENT_PERFECT_AS_PAST_TENSE							//Relex/Stanford independent
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
			#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION			//CHECK THIS; Stanford compatibility
		#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
			#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
			#ifdef GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION
				#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_CONDITION_PROPERTY		//CHECK THIS; Stanford compatibility
				#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_CONDITION_DEFINITION		//CHECK THIS; Stanford compatibility
			#else
				#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY	//CHECK THIS; Stanford compatibility
				#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION	//CHECK THIS; Stanford compatibility

			#endif
		#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
			#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B2_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
				#define GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
			#endif
		#endif
#endif

#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES_AND_COLLAPSE_ADVMOD_RELATION_FUNCTION_BE

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

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_PROPERTY
	#define RELATION_TYPE_TREAT_AS_PRONOUN_IE_PROPERTY_NUMBER_OF_TYPES (1)
#endif



#define GIA_ASSIGN_UNIQUE_ACTION_NODE_TO_EACH_ACTION_INSTANCE_OF_A_GIVEN_ACTION_NAME (1)
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_DEFINITIVE_NOUNS (1)		//NB must make this 0 for large.xml to work (NB this bug was issue introduced after GIA Archive - 1a5d - 04May11a, eg GIA Archive - 1a5e - 04May11a)
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_NOUNS_WITH_DETERMINATES (1)
#ifdef GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS
	#define GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS (0)	//this was set to (1) before version 1h3b	//NB used to be called 'GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS'
#else
	#define GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS (1)	//this was set to (1) before version 1h3b	//NB used to be called 'GIA_DO_NOT_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS'
#endif
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_ALL_PRONOUNS (1)		//IMPORTANT
//#define GIA_ALWAYS_ASSIGN_NEW_INSTANCE_PROPERTY_WHEN_DEFINING_A_PROPERTY
//#define GIA_ALWAYS_ASSIGN_NEW_INSTANCE_PROPERTY_WHEN_ATTACHING_A_PROPERTY

#define SUBJECT_INDEX (0)
#define OBJECT_INDEX (1)

#define MAX_NUMBER_OF_RELATIONS_PER_SENTENCE (200)
#define MAX_NUMBER_OF_WORDS_PER_SENTENCE (200)
		
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
#define RELATION_TYPE_ADJECTIVE_AMOD "_amod"	  //eg x is happy
#define RELATION_TYPE_ADJECTIVE_PREDADJ "_predadj"						  
#define RELATION_TYPE_ADJECTIVE_ADVMOD "_advmod"
#define RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES (3)
#define RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES (2)
//properties (possessive relationships)
#define RELATION_TYPE_POSSESSIVE "_poss"	//eg his bike	[bike him]		/its bike
#define RELATION_TYPE_PRENOMIAL_MODIFIER "_nn"
#define RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES (2)
#define STANFORD_RELATION_TYPE_GENETIVE_MODIFIER_OF_NOUN "gen"				//gen(cookie, Alice)	Alice's cookie						[THIS APPEARS INCORRECT: stanford currently gives; poss(cookie, Alice)] 	Relex: Identical to RelEx output _poss. 
#define STANFORD_RELATION_TYPE_POSS2 (STANFORD_RELATION_TYPE_GENETIVE_MODIFIER_OF_NOUN)


#define RELATION_TYPE_INDIRECT_OBJECT "_iobj"
#define RELATION_TYPE_PARATAXIS "_parataxis"	//eg "The guy, Akari said, left..." //added 13 February 2011

//concepts:					
#define RELATION_FUNCTION_DEFINITION_1 "be"	//eg x is y
#define RELATION_FUNCTION_DEFINITION_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_APPOSITIVE_OF_NOUN "_appo"
#define STANFORD_RELATION_TYPE_APPOSITIVE_OF_NOUN "appos"

//actions (obj/subj relationships):
#define RELATION_TYPE_OBJECT "_obj"			//eg eats y	[? be y]
#define RELATION_TYPE_OBJECT_THAT "_that"		//there is a place that we go
#define RELATION_TYPE_OBJECT_NUMBER_OF_TYPES (2)
#define RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES (1)
#define STANFORD_RELATION_TYPE_OBJECT "dobj"
#define STANFORD_RELATION_TYPE_INFINITIVAL_MODIFIER "infmod"				//Relex usually generates a plain _obj
#define STANFORD_RELATION_TYPE_PASSIVE_NOMINAL_SUBJECT "nsubjpass" 			//nsubjpass(thrown, rocks) 	rocks were thrown 				Relex: RelEx identifies these as _obj, and marks verb with passive feature. 
#define STANFORD_RELATION_TYPE_PARTICIPIAL_MODIFIER "partmod" 				//RelEx usually generates a plain _obj.  


#define RELATION_TYPE_SUBJECT "_subj"	//eg x eats 	[? be x]
#define RELATION_TYPE_SUBJECT_EXPLETIVE "_expl"		//eg goes there	//NB "there" is currently interpreted as a subject of an action
#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
	#define RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES (2)
#else
	#define RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES (1)
#endif
#define STANFORD_RELATION_TYPE_SUBJECT "nsubj"
#define STANFORD_RELATION_TYPE_AGENT "agent" 						//agent(kill, police) 	The man has been killed by the police.  		Relex: by(kill, police) 	GIA: subj(kill, police)



//stanford specific (non Relex) relations implemented in redistributeStanfordRelationsAdverbalClauseModifierAndComplement()/redistributeStanfordRelationsClausalSubject()/redistributeStanfordRelationsPhrasalVerbParticle()/redistributeStanfordRelationsNSubjAndPreposition():
#define STANFORD_RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER "advcl" 			//advcl(happen, fall)	The accident happened as the night was falling. 	Relex: as(happen, fall) 
#define STANFORD_RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER "mark"		//mark(fall, as)  	The accident happened as the night was falling. 	Relex: as(happen, fall) 
#define STANFORD_RELATION_TYPE_CLAUSAL_COMPLEMENT "ccomp" 				//ccomp(say, like)	He says that you like to swim				Relex: that(say, like) 		GIA: implement this as an action property
#define STANFORD_RELATION_TYPE_COMPLEMENTIZER "complm" 					//complm(like, that)	He says that you like to swim				Relex: that(say, like) 		GIA: implement this as an action property
#define STANFORD_RELATION_TYPE_CLAUSAL_SUBJECT "csubj" 					//csubj (make, say)	What she said makes sense. [dobj ( said-3 , What-1 )]	Relex:  Generated by Stanford; Relex uses plain _subj. 			GIA applies it to the object; ie subj (make, What)
#define STANFORD_RELATION_TYPE_NEGATIVE "neg" 						//neg(have, n't) 	haven't 						Relex usually generates NEGATIVE-FLAG(have, T) 	
//#define STANFORD_RELATION_TYPE_COMPLEMENT_OF_PREPOSITION "pcomp" 			//pcomp(garden, in)	It happened in the garden 				Relex: RelEx uses the general prepositional relations instead, e.g. in(happen, garden)  {OR pobj (go, store)/ pcomp (go, to) => to(go, store)} 	//only used by Stanford uncollapsed dependencies:
#define STANFORD_RELATION_TYPE_PHRASAL_VERB_PARTICLE "prt" 				//prt(shut, down) 	They shut down the station. 				RelEx always contracts the particle to create a polyword: 	e.g. _subj(shut_down, they)_obj(shut_down, station)	GIA: change the entity name 'shut' to 'shut_down'
//#define STANFORD_RELATION_TYPE_SMALL_CLAUSE_COMPLEMENT_OF_VERB "sc" 			//sc(stop, to) 		Alice forced him to stop.				[THIS APPEARS INCORRECT: stanford currently gives; xcomp ( forced-2 , stop-5 )]		Relex:RelEx uses the general prepositional relations instead, e.g. to(force,stop) 
#define RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER "_advcl"         
#define RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER "_mark"
#define RELATION_TYPE_CLAUSAL_COMPLEMENT "_ccomp"	       
#define RELATION_TYPE_COMPLEMENTIZER "_complm"		       
#define RELATION_TYPE_CLAUSAL_SUBJECT "_csubj"		       
#define RELATION_TYPE_NEGATIVE "_neg"			       
//#define RELATION_TYPE_COMPLEMENT_OF_PREPOSITION "_pcomp"      
#define RELATION_TYPE_PHRASAL_VERB_PARTICLE "_prt"	       
//#define RELATION_TYPE_SMALL_CLAUSE_COMPLEMENT_OF_VERB "_sc"  

//stanford specific (non Relex) relations implemented in fillGrammaticalArraysStanford() (grammar related):
#define STANFORD_RELATION_TYPE_MODAL_AUX "aux" 						//aux (died, has) 	Reagan has died.					Relex: indicates the tense feature: tense(die, present_perfect) 		[addtogrammar: perfect?]
#define STANFORD_RELATION_TYPE_PASSIVE_AUX "auxpass" 					//auxpass(killed, been) Kennedy has been killed. 				Relex indicates the tense feature: tense(kill, present_perfect_passive) 	[addtogrammar: passive]				
#define STANFORD_RELATION_TYPE_COPULA "cop" 						//cop(smelled, sweet) 	The rose smelled sweet. 				[THIS APPEARS INCORRECT: stanford currently gives; acomp ( smelled-3 , sweet-4 )]	    Relex: Generated by MiniPar, identical to RelEx output _to-be 	
//											//cop(black-5, was-4) 	Alice's cookie was black.				Relex: indicates the tense feature: tense(black, past) 				POS Tag of 'was' = VBD  [addtogrammar: tense - past/present/future]	
#define STANFORD_RELATION_TYPE_DETERMINER "det" 					//det(cookie, the) 	the cookie. 						Relex: RelEx uses the DEFINITE-FLAG feature instead. 			[addtogrammar: definite]
#define RELATION_TYPE_MODAL_AUX "_aux"			       
#define RELATION_TYPE_PASSIVE_AUX "_auxpass"		       
#define RELATION_TYPE_COPULA "_cop"												       
#define RELATION_TYPE_DETERMINER "_det"  


#define STANFORD_RELATION_TYPE_ROOT "root"  	
#define RELATION_TYPE_ROOT "_root"  	
#define GIA_NLP_PARSER_STANFORD_PARSER_DISABLE_ROOT_RELATION

//stanford relations not used by GIA;
/*
//#define STANFORD_RELATION_TYPE_DEPENDENT "dep"	//high level relation - not used
//#define STANFORD_RELATION_TYPE_ARGUMENT "arg"		//high level relation - not used
//#define STANFORD_RELATION_TYPE_COMPLEMENT "comp"	//high level relation - not used
//#define STANFORD_RELATION_TYPE_ "pobj"		//not used in collapsed relations
#define STANFORD_RELATION_TYPE_ATTRIBUTIVE "attr"
#define STANFORD_RELATION_TYPE_COMPLEMENTIZER "compl"
#define STANFORD_RELATION_TYPE_RELATIVE "rel"
#define STANFORD_RELATION_TYPE_REFERENT "ref"
//#define STANFORD_RELATION_TYPE_MODIFIER "mod"		//high level relation - not used
#define STANFORD_RELATION_TYPE_PURPOSE_CLAUSE_MODIFIER "purpcl"
#define STANFORD_RELATION_TYPE_TEMPORAL_MODIFIER "tmod"
#define STANFORD_RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER "rcmod"
#define STANFORD_RELATION_TYPE_ELEMENT_OF_COMPOUND_NUMBER "number"
#define STANFORD_RELATION_TYPE_ABBREVIATION_MODIFIER "abbrev"
#define STANFORD_RELATION_TYPE_POSSESSIVE "possessive"
#define STANFORD_RELATION_TYPE_PREPOSITION_MODIFIER "prep"
#define STANFORD_RELATION_TYPE_PREPOSITION_MODIFIER2 "pmod"
#define STANFORD_RELATION_TYPE_SEMANTIC_DEPENDENT "sdep"
#define STANFORD_RELATION_TYPE_XSUBJ "xsubj"
*/

//negations;
#define RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_NEGATIVE_CONTEXT_1 "not"

//conjugations;
#ifdef GIA_USE_RELEX_1.4.0
	#define GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_ARGUMENT_INDEX_MINUS_1
	#ifdef GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_ARGUMENT_INDEX_MINUS_1
		#define GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_ARGUMENT_INDEX_MINUS_1_REPLACEMENT_INDEX (MAX_NUMBER_OF_WORDS_PER_SENTENCE-2)
	#endif
	//#define GIA_TRANSLATOR_EXPLICITLY_ADD_CONJUNCTION_CONDITIONS	//not necessarily currently as; defineConjunctionConditions() currently performs the same function as defineActionPropertyConditions(). It is used at the moment such that the conjunction prepositions are added to the start of the list
#endif

#define RELATION_TYPE_CONJUGATION_AND "conj_and"
#define RELATION_TYPE_CONJUGATION_OR "conj_or"
#define STANFORD_RELATION_TYPE_CONJUNCT "conj"
#define STANFORD_RELATION_TYPE_COORDINATION "cc"		
#define RELATION_TYPE_CONJUNCT "_conj"
#define RELATION_TYPE_COORDINATION "_cc" 
#define RELATION_COORDINATION_DEPENDENT_AND "and"
#define RELATION_COORDINATION_DEPENDENT_OR "or"
#define RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES (2)
#define GIA_TRANSLATOR_USE_BASIC_CONJUNCTION_CONDITION_TYPE_NAMES
#ifdef GIA_TRANSLATOR_USE_BASIC_CONJUNCTION_CONDITION_TYPE_NAMES
	#define RELATION_TYPE_CONJUGATION_AND_BASIC "and"
	#define RELATION_TYPE_CONJUGATION_OR_BASIC "or"
	#define RELATION_TYPE_CONJUGATION_BASIC_NUMBER_OF_TYPES (RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES)
#endif

		
//tobe/todo (properties/conditions);
#define RELATION_TYPE_COMPLIMENT_TO_BE "_to-be"		//eg grows tired / The rose smelled sweet / _to-be(smell, sweet) - CHECK THIS
#define RELATION_TYPE_COMPLIMENT_TO_DO "_to-do"		//eg Linas likes to row / _to-do(like, row) - CHECK THIS
#define STANFORD_RELATION_TYPE_COMPLIMENT_TO_BE "acomp"	//NB added '_' for conversion purposes
#define STANFORD_RELATION_TYPE_COMPLIMENT_TO_DO "xcomp"	//NB added '_' for conversion purposes
#define RELATION_TYPE_COMPLIMENT_TO_BE_INDEX (0)
#define RELATION_TYPE_COMPLIMENT_TO_BE_INDEX (1)
#define RELATION_TYPE_COMPLEMENTS_NUMBER_OF_TYPES (2)
#define RELATION_TYPE_OBJECT_SPECIAL_TO_DO_PROPERTY_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_OBJECT_SPECIAL_TO_BE_PROPERTY_NUMBER_OF_TYPES (1)

//dates, measures, quantities
#define RELATION_TYPE_DATE_DAY "_date_day" 
#define RELATION_TYPE_DATE_YEAR "_date_year"
#define RELATION_TYPE_MEASURE_DISTANCE "_measure_distance" 
#define RELATION_TYPE_MEASURE_PER "_measure_per"
#define RELATION_TYPE_MEASURE_SIZE "_measure_size" 
#define RELATION_TYPE_MEASURE_TIME "_measure_time"
#define RELATION_TYPE_QUANTITY "_quantity"
#define STANFORD_RELATION_TYPE_QUANTITY "num"
#define RELATION_TYPE_QUANTITY_MOD "_quantity_mod"
#define STANFORD_RELATION_TYPE_QUANTITY_MOD "quantmod"
#define RELATION_TYPE_QUANTITY_MULT "_quantity_mult"
	//? DOING NOW: references: yet to integrate - see http://wiki.opencog.org/w/Ideas#Improved_reference_resolution for integration (also check for the existence of the "person" tag in the feature "tense" data block)
	//? #define RELATION_TYPE_QUANTITY "_quantity"	//eg his bike	[bike him]		/its bike
#define RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_OR_STANFORD_UNKNOWN_NUMBER_OF_TYPES (2)
#define RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES (5)
#define RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES (2)
#define RELATION_TYPE_MEASURE_NUMBER_OF_TYPES (5)
#define RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES (1)
#define STANFORD_RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER "npadvmod"
#define RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER "_npadvmod"
#define RELATION_TYPE_MEASURE_UNKNOWN "_measure"

//conditions: prepositions [predicates????]
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

//action/property reasons [NOT YET IMPLEMENTED ???]
#define RELATION_TYPE_PREPOSITION_SUCH_THAT "such"
#define RELATION_TYPE_PREPOSITION_SO "so"
#define RELATION_TYPE_PREPOSITION_REASON_NUMBER_OF_TYPES (2)
	//because?
	

#define GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES (1)



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
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_ARGUMENT_INDEX (MAX_NUMBER_OF_WORDS_PER_SENTENCE-1)
//#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_QUANTITY_NUMBER_REPLACEMENT -9999
#define REFERENCE_TYPE_QUESTION_WHEN_CONTEXT_NUMBER_OF_TYPES (15)	//Eg what is the time?
#define REFERENCE_TYPE_QUESTION_WHERE_CONTEXT_NUMBER_OF_TYPES (3)	//Eg what is the location?
#define REFERENCE_TYPE_QUESTION_WHY_CONTEXT_NUMBER_OF_TYPES (3)		//Eg what is the reason?

#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE_REPLACEMENT "RELATION_TYPE_PREPOSITION_AT"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN_REPLACEMENT "RELATION_TYPE_PREPOSITION_AT"	//must also set hasAssociatedTime to true
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY_REPLACEMENT "RELATION_TYPE_PREPOSITION_BECAUSE"


#define FEATURE_NER_O "O"
#define FEATURE_NER_DATE "DATE" 
#define FEATURE_NER_TIME "TIME" 
#define FEATURE_NER_MONEY "MONEY"
#define FEATURE_NER_NUMBER "NUMBER"
#define FEATURE_NER_PERSON "PERSON"
#define FEATURE_NER_LOCATION "LOCATION"
#define FEATURE_NER_ORGANIZATION "ORGANIZATION"
#define FEATURE_NER_MISC "MISC"
#define FEATURE_NER_DURATION "DURATION"



#define STANFORD_PARSER_PREPOSITION_PREPEND "prep_"
#define STANFORD_PARSER_PREPOSITION_PREPEND_LENGTH (5)
#define STANFORD_PARSER_PREPOSITION_DELIMITER "_"








static string featureQueryWordAcceptedByAlternateMethodNameArray[FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES] = {"which"};

static string relationTypePropositionTimeNameArray[RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES] = {"in", "on", "after", "ago", "before", "between", "by", "during", "for", "to", "till", "until", "past", "since", "up_to", "within", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN};
	//http://www.englisch-hilfen.de/en/grammar/preposition_time.htm + is [time is] etc
static string relationTypePropositionLocationNameArray[RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES] = {"in", "on", "at", "by", "near", "nearby", "above", "below", "over", "under", "around", "through", "inside", "inside_of", "outside", "between", "beside", "beyond", "in_front_of", "in_front", "in_back_of", "behind", "next_to", "on_top_of", "within", "beneath", "underneath", "among", "along", "against", "before", "after", "behind", "to", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE};		
	//http://www.eslgold.com/grammar/prepositions_location.html + before, after, behind, to, etc
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
//static string relationTypePropositionTimeNameArray[RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING, "after", "ago", "before", "between", "during", "for", "till", "until", "past", "since", "up_to", "within", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN};
//static string relationTypePropositionLocationNameArray[RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING, "near", "nearby", "above", "below", "over", "under", "around", "through", "inside", "inside_of", "outside", "between", "beside", "beyond", "in_front_of", "in_front", "in_back_of", "behind", "next_to", "on_top_of", "within", "beneath", "underneath", "among", "along", "against", "before", "after", "behind", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE};		

static string relationTypePropositionReductionNameArray[RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES][RELATION_TYPE_PREPOSITION_REDUCTION_MAX_NUMBER_VARIATIONS] = {{"at", "in", "to", "on"}, {"from", "of", "by", "", }, {"for", "since", "", ""}};
static int relationTypePropositionReductionNumberVariationsArray[RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_NUMBER_OF_TYPES, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_NUMBER_OF_TYPES, RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_NUMBER_OF_TYPES};
static string relationTypePropositionReductionReplacementNamesArray[RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_REPLACEMENT_STRING};
#endif

static string relationTypePropositionReasonOrCircumstanceNameArray[RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES] = {"because", "on_account_of", "for", "out_of", "when",  REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY};
	//http://vlc.polyu.edu.hk/vlc/GrammarCourse/Lesson2_Preposition/CausePurpose.htm
static string relationContextPropositionTimeNameArray[REFERENCE_TYPE_QUESTION_WHEN_CONTEXT_NUMBER_OF_TYPES] = {"time", "period", "era", "millenia", "decade", "day", "month", "year", "hour", "minute", "second", "millisecond", "microsecond", "nanosecond", "picosecond"};
static string relationContextPropositionLocationNameArray[REFERENCE_TYPE_QUESTION_WHERE_CONTEXT_NUMBER_OF_TYPES] = {"location", "place", "position"};	//coordinates?
static string relationContextPropositionReasonNameArray[REFERENCE_TYPE_QUESTION_WHY_CONTEXT_NUMBER_OF_TYPES] = {"reason", "basis", "argument"};

static string relationContextNegativeNameArray[RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES] = {RELATION_TYPE_NEGATIVE_CONTEXT_1};

static string relationTypeConjugationNameArray[RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES] = {RELATION_TYPE_CONJUGATION_AND, RELATION_TYPE_CONJUGATION_OR};
#ifdef GIA_TRANSLATOR_USE_BASIC_CONJUNCTION_CONDITION_TYPE_NAMES
static string relationTypeConjugationBasicNameArray[RELATION_TYPE_CONJUGATION_BASIC_NUMBER_OF_TYPES] = {RELATION_TYPE_CONJUGATION_AND_BASIC, RELATION_TYPE_CONJUGATION_OR_BASIC};
#endif
	
static string relationTypeObjectNameArray[RELATION_TYPE_OBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT_THAT};
#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
static string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT, RELATION_TYPE_SUBJECT_EXPLETIVE};
#else
static string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT};
#endif
static string relationTypeAdjectiveNameArray[RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD, RELATION_TYPE_ADJECTIVE_PREDADJ, RELATION_TYPE_ADJECTIVE_ADVMOD};
static string relationTypePossessiveNameArray[RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_POSSESSIVE, RELATION_TYPE_PRENOMIAL_MODIFIER};

static string relationFunctionCompositionNameArray[RELATION_FUNCTION_COMPOSITION_NUMBER_OF_TYPES] = {RELATION_FUNCTION_COMPOSITION_1, RELATION_FUNCTION_COMPOSITION_2, RELATION_FUNCTION_COMPOSITION_3, RELATION_FUNCTION_COMPOSITION_4};
static string relationFunctionDefinitionNameArray[RELATION_FUNCTION_DEFINITION_NUMBER_OF_TYPES] = {RELATION_FUNCTION_DEFINITION_1};

static string relationTypeObjectSpecialConditionMeasureDistanceOrStanfordUnknownNameArray[RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_OR_STANFORD_UNKNOWN_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_UNKNOWN};
static string relationTypeObjectSpecialConditionToDoPropertyNameArray[RELATION_TYPE_OBJECT_SPECIAL_TO_DO_PROPERTY_NUMBER_OF_TYPES] = {RELATION_TYPE_COMPLIMENT_TO_DO};
static string relationTypeObjectSpecialConditionToBePropertyNameArray[RELATION_TYPE_OBJECT_SPECIAL_TO_BE_PROPERTY_NUMBER_OF_TYPES] = {RELATION_TYPE_COMPLIMENT_TO_BE};

static string relationTypeComplementsNameArray[RELATION_TYPE_COMPLEMENTS_NUMBER_OF_TYPES] = {RELATION_TYPE_COMPLIMENT_TO_BE, RELATION_TYPE_COMPLIMENT_TO_DO};

static string relationTypeAdjectiveWhichImplyEntityInstanceNameArray[RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD, RELATION_TYPE_ADJECTIVE_ADVMOD};
static string relationTypeRequireSwitchingNameArray[RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT_THAT};
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_PROPERTY
static string relationTypeTreatAsPronounIeProperty[RELATION_TYPE_TREAT_AS_PRONOUN_IE_PROPERTY_NUMBER_OF_TYPES] = {"that"};
#endif

static string relationTypeQuantityOrMeasureNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY, RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME, RELATION_TYPE_MEASURE_UNKNOWN};
static string relationTypeQuantityOrMeasureSwitchedNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY_MOD, RELATION_TYPE_MEASURE_PER};
static string relationTypeMeasureNameArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_PER, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME, RELATION_TYPE_MEASURE_UNKNOWN};
static int relationTypeMeasureNameTypeIndexArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {MEASURE_TYPE_DISTANCE, MEASURE_TYPE_PER, MEASURE_TYPE_SIZE, MEASURE_TYPE_TIME, MEASURE_TYPE_UNKNOWN};

static string relationTypeQuantityArgumentImplyMeasurePerNameArray[RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES] = {"every"};

//static int timeMonthIntArray[TIME_MONTH_NUMBER_OF_TYPES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
static string timeMonthStringArray[TIME_MONTH_NUMBER_OF_TYPES] = {TIME_MONTH_JANUARY, TIME_MONTH_FEBRUARY, TIME_MONTH_MARCH, TIME_MONTH_APRIL, TIME_MONTH_MAY, TIME_MONTH_JUNE, TIME_MONTH_JULY, TIME_MONTH_AUGUST, TIME_MONTH_SEPTEMBER, TIME_MONTH_OCTOBER, TIME_MONTH_NOVEMBER, TIME_MONTH_DECEMBER};
	

static string referenceTypePossessiveNameArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {"undefined", "his", "her", "them", "its"};
//static int referenceTypePossessiveNameLengthsArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {9, 3, 3, 4, 3};
static string referenceTypePersonNameArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {"undefined", "he", "she", "they", "it"};
//static int referenceTypePersonNameLengthsArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {9, 2, 3, 4, 2};

static int referenceTypePersonCrossReferenceNumberArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_UNDEFINED, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_PLURAL, GRAMMATICAL_NUMBER_SINGULAR};
static int referenceTypePersonCrossReferenceGenderArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_MASCULINE, GRAMMATICAL_GENDER_FEMININE, GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_UNDEFINED};
static bool referenceTypePersonCrossReferenceDefiniteArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {false, true, true, true, true};
static bool referenceTypePersonCrossReferencePersonArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON_UNDEFINED};









#define GIA_NUMBER_OF_RELEX_VERSUS_STANFORD_DEPENDENCY_RELATION_DISCREPANCIES (12)
static string relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_NUMBER_OF_TYPES][GIA_NUMBER_OF_RELEX_VERSUS_STANFORD_DEPENDENCY_RELATION_DISCREPANCIES] = {{RELATION_TYPE_COMPLIMENT_TO_BE, RELATION_TYPE_COMPLIMENT_TO_DO, RELATION_TYPE_APPOSITIVE_OF_NOUN, RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_SUBJECT, RELATION_TYPE_POSSESSIVE, RELATION_TYPE_QUANTITY, RELATION_TYPE_QUANTITY_MOD, RELATION_TYPE_SUBJECT}, {STANFORD_RELATION_TYPE_COMPLIMENT_TO_BE, STANFORD_RELATION_TYPE_COMPLIMENT_TO_DO, STANFORD_RELATION_TYPE_APPOSITIVE_OF_NOUN, STANFORD_RELATION_TYPE_OBJECT, STANFORD_RELATION_TYPE_INFINITIVAL_MODIFIER, STANFORD_RELATION_TYPE_PASSIVE_NOMINAL_SUBJECT, STANFORD_RELATION_TYPE_PARTICIPIAL_MODIFIER, STANFORD_RELATION_TYPE_SUBJECT, STANFORD_RELATION_TYPE_POSS2, STANFORD_RELATION_TYPE_QUANTITY, STANFORD_RELATION_TYPE_QUANTITY_MOD, STANFORD_RELATION_TYPE_AGENT}};


/*
noun = joe
verb = rides
adjective = happy
*/

void initialiseGIATranslatorForTexualContextOperations();

bool isAdjectiveNotAnAdvmodAndRelationFunctionIsNotBe(Relation * currentRelationInList, GIAEntityNode * GIAEntityNodeArray[], int relationFunctionIndex, int NLPdependencyRelationsType);
bool isAdjectiveNotConnectedToObjectOrSubject(Sentence * currentSentenceInList, Relation * currentRelationInList, int NLPdependencyRelationsType);								//Stanford Compatible

void addOrConnectPropertyToEntity(GIAEntityNode * thingEntity, GIAEntityNode * propertyEntity);
void addPropertyToPropertyDefinition(GIAEntityNode * propertyEntity);
	GIAEntityNode * addProperty(GIAEntityNode * propertyEntity);
	

void addTenseOnlyTimeConditionToProperty(GIAEntityNode * propertyNode, int tense, bool isProgressive);

void addDefinitionToEntity(GIAEntityNode * thingEntity, GIAEntityNode * definitionEntity);

void addActionToEntity(GIAEntityNode * subjectEntity, GIAEntityNode * objectEntity, GIAEntityNode * actionEntity);
void addActionToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * actionEntity);
void addActionToObject(GIAEntityNode * objectEntity, GIAEntityNode * actionEntity);
	void addActionInstanceToSubject(GIAEntityNode * subjectEntity, GIAEntityNode * newOrExistingAction);
	void addActionInstanceToObject(GIAEntityNode * objectEntity, GIAEntityNode * newOrExistingAction);	
		GIAEntityNode * addActionToActionDefinition(GIAEntityNode * actionEntity);
			GIAEntityNode * addAction(GIAEntityNode * actionEntity);
		
void addOrConnectPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionEntityNode, GIAEntityNode * conditionTypeConceptEntity);
void addOrConnectBeingDefinitionConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionDefinitionNode, GIAEntityNode * conditionTypeConceptEntity, bool negative);
void addOrConnectHavingPropertyConditionToEntity(GIAEntityNode * entityNode, GIAEntityNode * conditionPropertyNode, GIAEntityNode * conditionTypeConceptEntity, bool negative);
	void addConditionToProperty(GIAEntityNode * propertyNode, GIAEntityNode * propertyConditionEntity, GIAEntityNode * conditionTypeConceptEntity);
		GIAEntityNode * addCondition(GIAEntityNode * conditionEntity);

string convertStanfordPrepositionToRelex(string * preposition, int NLPdependencyRelationsType, bool * stanfordPrepositionFound);				//Stanford Compatible



void setTranslatorEntityNodesCompleteList(vector<GIAEntityNode*> * newEntityNodesCompleteList);
//void setTranslatorConceptEntityNodesList(vector<GIAEntityNode*> * newConceptEntityNodesList);
void setTranslatorPropertyEntityNodesList(vector<GIAEntityNode*> * newPropertyEntityNodesList);
void setTranslatorActionEntityNodesList(vector<GIAEntityNode*> * newActionEntityNodesList);
void setTranslatorConditionEntityNodesList(vector<GIAEntityNode*> * newConditionEntityNodesList);

vector<GIAEntityNode*> * getTranslatorEntityNodesCompleteList();
//vector<GIAEntityNode*> * getTranslatorConceptEntityNodesList();
vector<GIAEntityNode*> * getTranslatorPropertyEntityNodesList();
vector<GIAEntityNode*> * getTranslatorActionEntityNodesList();
vector<GIAEntityNode*> * getTranslatorConditionEntityNodesList();

bool getFoundComparisonVariable();
GIAEntityNode* getComparisonVariableNode();
void setFoundComparisonVariable(bool value);
void setComparisonVariableNode(GIAEntityNode* newComparisonVariableNode);

/*
void setCurrentEntityNodeIDInCompleteList(long newCurrentEntityNodeIDInCompleteList);
void setCurrentEntityNodeIDInConceptEntityNodesList(long newCurrentEntityNodeIDInConceptEntityNodesList);
void setCurrentEntityNodeIDInPropertyEntityNodesList(long newCurrentEntityNodeIDInPropertyEntityNodesList);
void setCurrentEntityNodeIDInActionEntityNodesList(long newCurrentEntityNodeIDInActionEntityNodesList);
void setCurrentEntityNodeIDInConditionEntityNodesList(long newCurrentEntityNodeIDInConditionEntityNodesList);
void incrementCurrentEntityNodeIDInCompleteList();
void incrementCurrentEntityNodeIDInConceptEntityNodesList();
void incrementCurrentEntityNodeIDInPropertyEntityNodesList();
void incrementCurrentEntityNodeIDInActionEntityNodesList();
void incrementCurrentEntityNodeIDInConditionEntityNodesList();
long getCurrentEntityNodeIDInCompleteList();
long getCurrentEntityNodeIDInConceptEntityNodesList();
long getCurrentEntityNodeIDInPropertyEntityNodesList();
long getCurrentEntityNodeIDInActionEntityNodesList();
long getCurrentEntityNodeIDInConditionEntityNodesList();
*/
long * getCurrentEntityNodeIDInCompleteList();
long * getCurrentEntityNodeIDInConceptEntityNodesList();
long * getCurrentEntityNodeIDInPropertyEntityNodesList();
long * getCurrentEntityNodeIDInActionEntityNodesList();
long * getCurrentEntityNodeIDInConditionEntityNodesList();


			
#endif

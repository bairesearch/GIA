/*******************************************************************************
 *
 * File Name: GIATranslatorOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j7d 09-May-2012
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

#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_AND_XCOMP		//working on this now (1j6h)
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION	//added in addition to this; the pre-process of "two word prepositions" eg from http://en.wikipedia.org/wiki/List_of_English_prepositions, or post process (currently implemented)
//#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION_OLD



#ifndef GIA_STANFORD_CORE_NLP_DO_NOT_USE_CODEPENDENCIES
	#define GIA_STANFORD_CORE_NLP_CODEPENDENCIES_ONLY_USE_PRONOMINAL_COREFERENCE_RESOLUTION
#endif

//#define GIA_USE_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE		//this has been disabled because it has been deemed inappropriate for 'The disaster happened over night.?'  [prt(happened-3, over-4)]

//#define GIA_ENFORCE_USE_OF_RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES


//#define GIA_DO_NOT_PARSE_DISABLED_RELATIONS	//test only
#define GIA_DO_NOT_ADD_PROPERTIES_ACTIONS_AND_CONDITIONS_TO_DISABLED_CONCEPT_ENTITIES	//test only




#ifndef GIA_DISABLE_REFERENCING
	#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS	//default: enabled
#endif

#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
	#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_CLEAR_REFERENCES_EVERY_PARAGRAPH	//default: enabled		//Relex Only
#else
	#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_CLEAR_REFERENCES_EVERY_SENTENCE	//default: disabled
#endif
//#define GIA_ENABLE_REFERENCE_LINKING_BASED_UPON_PRONOUNS_CLEAR_REFERENCES_EVERY_SENTENCE	//default: disabled		//Relex and Stanford supported (untested with stanford)


//#define GIA_OPTIMISE_PERFORMANCE_FOR_RELEX_PATENT_QUERIES_REPLICATION_RATHER_THAN_RELEX_PATENT_SYNTACTIC_PROTOTYPE_OUTPUT_REPLICATION			//not ever used - old for testing
//#define GIA_COLLAPSE_COP_RELATION_DEPENDENT_BE_TO_APPOS_NOT_PREDADJ_OLD										//not ever used - old for testing



#define USE_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION_CONCEPT_ENTITY_IN_A_GIVEN_SENTENCE
//#define USE_OLD_SUBJ_OBJ_ONLY_ONE_PAIR_RESTRICTION_METHOD 	//default: disabled

//#define GIA_DEBUG_ENABLE_REDUNDANT_TO_DO_PROPERTY_CONNECTIONS_TO_DEMONSTRATE_DRAW_FAILURE


#define GIA_DEAL_WITH_TEMPORARY_ISSUE
#ifdef GIA_DEAL_WITH_TEMPORARY_ISSUE
	#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION				//EITHER this is required
	//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B2_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY	//OR this is required
#endif

//#define GIA_TRANSLATOR_USE_NEW_ACTION_SUBJECT_RELATION_GOVERNOR_DEFINITION_IMPLEMENTATION	//if undefined then "tom is being an idiot"= an instance of tom is an idoit. if defined then tom has (a property) of idiocy [NOT YET IMPLEMENTED]


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
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_4A_RELATIONS_DEFINE_PROPERTIES_BASED_UPON_INDIRECT_OBJECTS	//added 27 April 2012 - required because of the case; "What did the officer give transportation to?" where transportation is not singular, and therefore will not be defined as a property until indirectObjects are defined (after the action 'give' has already been linked to its concept entity). NB "What did the officer give a ride to?" does not face the same problem as 'ride' is tagged as singular by relex and therefore will be assigned as a property before the action 'give' is linked to it
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5A_RELATIONS_ASSIGN_TIME_NODES_IN_RELEX_THE_SAME_AS_STANFORD	//Case 5= stanford specific
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5B_RELATIONS_COMPENSATE_FOR_INACCURATE_STANFORD_DATE_TAGGING
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5C_FEATURES_STANFORD_NER_INDICATES_NAME_CONCATENATION_REQUIRES_POS_NNP

#ifdef GIA_USE_RELEX

	//GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE:
	
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3A_PREPOSITIONS_INTERPRET_PREPOSITION_OF_AS_EITHER_CONDITION_OR_PROPERTY_LINK_DEPENDING_UPON_ACTION_OR_PROPERTY
	
	//GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE:

		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_AS_PROPERTY_LINK
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TOBE_AND_SUBJECT_RELATION_AS_PROPERTY_LINK_AND_ACTION_DEFINITION
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_CONCEPT_ENTITY_NODES_ADVANCED	//Relex only at present [relies upon isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe() and GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS]
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
	//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE 
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE
			#define GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ
			//#define GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ_OLD
		#endif
	#endif
#endif

#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_A_PROPERTY_BASIC
#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_A_DEFINITION_BASIC

#define GIA_TRANSLATOR_ACTION_DEFINITION_CODE_SIMPLIFICATION

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
	#define RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES (3)
	#define RELATION_TYPE_PREPOSITION_REDUCTION_MAX_NUMBER_VARIATIONS (4)
	#define RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_NUMBER_OF_TYPES (4)
	#define RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING ((string)"a`")	//�: character not supported by ASCII
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

#define MAX_NUMBER_OF_RELATIONS_PER_SENTENCE (1000)
#define MAX_NUMBER_OF_WORDS_PER_SENTENCE (1000)
		
#define SECONDS_IN_YEAR (365*24*60*60)



/********************************* Relations *******************************************/

//?? NB all of these cases/types need to be replaced with more complex grammar requirements (eg "on" can also mean "rides his bike on the road" [location], not just "rides his bike on tuesday" [time])

//Properties:	[NB properties are attached to either another property or a straight entity);]
//properties (derived from obj/subj relationships);					
#define RELATION_GOVERNOR_COMPOSITION_1 "contains"	//eg x contains y
#define RELATION_GOVERNOR_COMPOSITION_2 "comprise"
#define RELATION_GOVERNOR_COMPOSITION_3 "has"	
#define RELATION_GOVERNOR_COMPOSITION_4 "have"
#define RELATION_GOVERNOR_COMPOSITION_NUMBER_OF_TYPES (4)						
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
#define RELATION_ENTITY_BE "be"	//eg x is y
#define RELATION_GOVERNOR_DEFINITION_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_APPOSITIVE_OF_NOUN "_appo"
#define STANFORD_RELATION_TYPE_APPOSITIVE_OF_NOUN "appos"

//actions (obj/subj relationships):
#define RELATION_TYPE_OBJECT "_obj"			//eg eats y	[? be y]
#define RELATION_TYPE_OBJECT_THAT "_that"		//there is a place that we go
#define RELATION_TYPE_OBJECT_NUMBER_OF_TYPES (3)
#define RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES (1)
#define STANFORD_RELATION_TYPE_OBJECT "dobj"
#define STANFORD_RELATION_TYPE_INFINITIVAL_MODIFIER "infmod"				//Relex usually generates a plain _obj
#define STANFORD_RELATION_TYPE_PASSIVE_NOMINAL_SUBJECT "nsubjpass" 			//nsubjpass(thrown, rocks) 	rocks were thrown 				Relex: RelEx identifies these as _obj, and marks verb with passive feature. 
#define STANFORD_RELATION_TYPE_PARTICIPIAL_MODIFIER "partmod" 				//RelEx usually generates a plain _obj. [however GIA keeps it as a separate relation, as it uses it elsewhere for multiword preposition collapse purposes] 
#define RELATION_TYPE_PARTICIPIAL_MODIFIER "_partmod"

#define RELATION_TYPE_SUBJECT "_subj"	//eg x eats 	[? be x]
#define RELATION_TYPE_SUBJECT_EXPLETIVE "_expl"		//eg goes there	//NB "there" is currently interpreted as a subject of an action
#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
	#define RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES (2)
#else
	#define RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES (1)
#endif
#define STANFORD_RELATION_TYPE_SUBJECT "nsubj"
#define STANFORD_RELATION_TYPE_AGENT "agent" 						//agent(kill, police) 	The man has been killed by the police.  		Relex: by(kill, police) 	GIA: subj(kill, police)



//stanford specific (non Relex) relations implemented in redistributeStanfordRelationsAdverbalClauseModifierAndComplement()/redistributeStanfordRelationsClausalSubject()/redistributeStanfordRelationsPhrasalVerbParticle()/redistributeStanfordRelationsMultiwordPreposition():
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

//stanford specific (non Relex) relations implemented in redistributeStanfordRelationsCreateQueryVars
#define STANFORD_RELATION_TYPE_ATTRIBUTIVE "attr"	//eg attr(is-2, Who-1) / attr(is-2, What-1)
#define RELATION_TYPE_ATTRIBUTIVE "_attr"

//stanford specific (non Relex) relations implemented in fillGrammaticalArraysStanford() (grammar related):
#define STANFORD_RELATION_TYPE_MODAL_AUX "aux" 						//aux (died, has) 	Reagan has died.					Relex: indicates the tense feature: tense(die, present_perfect) 		[addtogrammar: perfect?]
#define STANFORD_RELATION_TYPE_PASSIVE_AUX "auxpass" 					//auxpass(killed, been) Kennedy has been killed. 				Relex indicates the tense feature: tense(kill, present_perfect_passive) 	[addtogrammar: passive]				
#define STANFORD_RELATION_TYPE_COPULA "cop" 						//cop(smelled, sweet) 	The rose smelled sweet. 				[THIS APPEARS INCORRECT: stanford currently gives; acomp ( smelled-3 , sweet-4 )]	    Relex: Generated by MiniPar, identical to RelEx output _to-be 	
//											//cop(black-5, was-4) 	Alice's cookie was black.				Relex: indicates the tense feature: tense(black, past) 				stanfordPOS Tag of 'was' = VBD  [addtogrammar: tense - past/present/future]	
#define STANFORD_RELATION_TYPE_DETERMINER "det" 					//det(cookie, the) 	the cookie. 						Relex: RelEx uses the DEFINITE-FLAG feature instead. 			[addtogrammar: definite]
#define RELATION_TYPE_MODAL_AUX "_aux"			       
#define RELATION_TYPE_PASSIVE_AUX "_auxpass"		       
#define RELATION_TYPE_COPULA "_cop"												       
#define RELATION_TYPE_DETERMINER "_det"  


#define STANFORD_RELATION_TYPE_ROOT "root"  	
#define RELATION_TYPE_ROOT "_root"  	
#define GIA_NLP_PARSER_STANFORD_PARSER_DISABLE_ROOT_RELATION

#define STANFORD_RELATION_TYPE_TEMPORAL_MODIFIER "tmod"
#define RELATION_TYPE_TEMPORAL_MODIFIER "_tmod"

//stanford relations not used by GIA;
/*
used
//#define STANFORD_RELATION_TYPE_ARGUMENT "arg"		//high level relation - not used
//#define STANFORD_RELATION_TYPE_COMPLEMENT "comp"	//high level relation - not used
//#define STANFORD_RELATION_TYPE_ "pobj"		//not used in collapsed relations
#define STANFORD_RELATION_TYPE_COMPLEMENTIZER "compl"
#define STANFORD_RELATION_TYPE_RELATIVE "rel"
#define STANFORD_RELATION_TYPE_REFERENT "ref"
//#define STANFORD_RELATION_TYPE_MODIFIER "mod"		//high level relation - not used
#define STANFORD_RELATION_TYPE_PURPOSE_CLAUSE_MODIFIER "purpcl"
#define STANFORD_RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER "rcmod"
#define STANFORD_RELATION_TYPE_ABBREVIATION_MODIFIER "abbrev"
#define STANFORD_RELATION_TYPE_POSSESSIVE "possessive"
#define STANFORD_RELATION_TYPE_PREPOSITION_MODIFIER2 "pmod"
#define STANFORD_RELATION_TYPE_SEMANTIC_DEPENDENT "sdep"
#define STANFORD_RELATION_TYPE_XSUBJ "xsubj"			//??
#define RELATION_TYPE_XSUBJ "xsubj"	
*/
#define STANFORD_RELATION_TYPE_PREPOSITION_MODIFIER "prep"	//deal with during stanford reduction (added 3 May 2012)
#define RELATION_TYPE_PREPOSITION_MODIFIER "_prep"
	

//negations;
#define RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES (1)
#define RELATION_TYPE_NEGATIVE_CONTEXT_1 "not"

//conjugations;
#ifdef GIA_USE_RELEX_1.4.0
	#define GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1
	#ifdef GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1
		#define GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1_REPLACEMENT_INDEX (MAX_NUMBER_OF_WORDS_PER_SENTENCE-2)
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
#define RELATION_DEPENDENT_DO "do"	

//dates, measures, and quantities;
#define RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES (5)
#define RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES (3)
#define RELATION_TYPE_MEASURE_DEPENDENCY_NUMBER_OF_TYPES (2)

//dates;
#define RELATION_TYPE_DATE_DAY "_date_day" 	//Relex only
#define RELATION_TYPE_DATE_YEAR "_date_year"	//Relex only

//measures;
#define RELATION_TYPE_MEASURE_DISTANCE "_measure_distance" 	//Relex only
#define RELATION_TYPE_MEASURE_SIZE "_measure_size" 		//Relex only
#define RELATION_TYPE_MEASURE_TIME "_measure_time"		//Relex only
#define RELATION_TYPE_MEASURE_UNKNOWN "_measure"
#define STANFORD_RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER "npadvmod"
#define RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER "_npadvmod"

#define RELATION_TYPE_MEASURE_PER "_measure_per"		//Relex only
#define RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN "_measure_dependency"	//Stanford Only
#define STANFORD_RELATION_TYPE_DEPENDENT "dep"	//high level relation - not 
#define RELATION_TYPE_DEPENDENT "_dep"

#define RELATION_TYPE_MEASURE_NUMBER_OF_TYPES (6)
#define RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES (1)

//quantities;
#define RELATION_TYPE_QUANTITY "_quantity"
#define STANFORD_RELATION_TYPE_QUANTITY "num"
#define STANFORD_RELATION_TYPE_ELEMENT_OF_COMPOUND_NUMBER "number"
#define RELATION_TYPE_QUANTITY_MOD "_quantity_mod"
#define STANFORD_RELATION_TYPE_QUANTITY_MOD "quantmod"
#define RELATION_TYPE_QUANTITY_MULT "_quantity_mult"
	//? DOING NOW: references: yet to integrate - see http://wiki.opencog.org/w/Ideas#Improved_reference_resolution for integration (also check for the existence of the "person" tag in the feature "tense" data block)
	//? #define RELATION_TYPE_QUANTITY "_quantity"	//eg his bike	[bike him]		/its bike
#define RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_OR_STANFORD_UNKNOWN_NUMBER_OF_TYPES (2)






//conditions: prepositions [predicates????]
#define RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES (19)
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
#define REFERENCE_TYPE_QUESTION_QUERY_WHO "who"
#define REFERENCE_TYPE_QUESTION_QUERY_WHAT "what"
#define REFERENCE_TYPE_QUESTION_QUERY_HOW "how"
#define REFERENCE_TYPE_QUESTION_QUERY_WHERE "where"
#define REFERENCE_TYPE_QUESTION_QUERY_WHEN "when"
#define REFERENCE_TYPE_QUESTION_QUERY_WHY "why"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN "_%atTime"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE "_%atLocation"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY "_%because"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_HOW "_%how"	//these needs to be a new integer (and not "prep_how") to prevent concept entity node overwrite within redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy()
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX (MAX_NUMBER_OF_WORDS_PER_SENTENCE-1)
//#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_QUANTITY_NUMBER_REPLACEMENT -9999
#define REFERENCE_TYPE_QUESTION_WHEN_CONTEXT_NUMBER_OF_TYPES (15)	//Eg what is the time?
#define REFERENCE_TYPE_QUESTION_WHERE_CONTEXT_NUMBER_OF_TYPES (3)	//Eg what is the location?
#define REFERENCE_TYPE_QUESTION_WHY_CONTEXT_NUMBER_OF_TYPES (3)		//Eg what is the reason?

#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE_REPLACEMENT "RELATION_TYPE_PREPOSITION_AT"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN_REPLACEMENT "RELATION_TYPE_PREPOSITION_AT"	//must also set hasAssociatedTime to true
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY_REPLACEMENT "RELATION_TYPE_PREPOSITION_BECAUSE"

//prepositions;
#define STANFORD_PARSER_PREPOSITION_PREPEND "prep_"
#define STANFORD_PARSER_PREPOSITION_PREPEND_LENGTH (5)
#define STANFORD_PARSER_PREPOSITION_DELIMITER "_"

/*************************************************************************************/

/********************************* Features *******************************************/

//stanfordPOS;
#define FEATURE_POS_TAG_VB "VB"
#define FEATURE_POS_TAG_VBD "VBD"
#define FEATURE_POS_TAG_VBG "VBG"
#define FEATURE_POS_TAG_VBN "VBN"
#define FEATURE_POS_TAG_VBP "VBP"
#define FEATURE_POS_TAG_VBZ "VBZ"
#define FEATURE_POS_TAG_NN "NN"
#define FEATURE_POS_TAG_NNS "NNS"
#define FEATURE_POS_TAG_NNP "NNP"
#define FEATURE_POS_TAG_NNPS "NNPS"
#define FEATURE_POS_TAG_ADJECTIVE "JJ"
#define FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE "JJR" 
#define FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE "JJS" 
#define FEATURE_POS_TAG_ADVERB "RB"
#define FEATURE_POS_TAG_ADVERB_COMPARATIVE "RBR"
#define FEATURE_POS_TAG_ADVERB_SUPERLATIVE "RBS"
#define FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL (16)

#define FEATURE_POS_TAG_VERB_PAST_NUMBER_OF_TYPES (2)
#define FEATURE_POS_TAG_VERB_PRESENT_NUMBER_OF_TYPES (2)
#define FEATURE_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES (1)

#define FEATURE_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES (2)
#define FEATURE_POS_TAG_COMMON_NOUN_NUMBER_OF_TYPES (2)
#define FEATURE_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES (2)
#define FEATURE_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES (2)

#define RELATION_TYPE_AUXILLARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES (1)

#define FEATURE_POS_TAG_WDT "WDT"	//wh-determiner

/*
stanford parser tense
https://mailman.stanford.edu/pipermail/parser-user/2008-September/000073.html
Is there a way to identify the main verb of the sentence, and following
that, the tense of the sentence? So far I've been identifying the first
'VBP', 'VBD' or 'VBZ' tagged word, and assigning present or past tense
according to these tags.

stanford pos tagger specification
	http://bulba.sdsu.edu/jeanette/thesis/PennTags.html
	
http://nlp.stanford.edu/software/tagger.shtml	
	Part-of-speech name abbreviations: The English taggers use the Penn Treebank tag set.

J93-2004.pdf;
VB Verb, base form
VBD Verb, past tense	
VBG Verb, gerund/present participle
VBN Verb, past participle
VBP Verb, non-3rd ps. sing. present
VBZ Verb, 3rd ps. sing. present

NN Noun, singular or mass
NNS Noun, plural
NNP Proper noun, singular
NNPS Proper noun, plural

sing/VB be/VB do/VB have/VB			[N/A]
sings/VBZ is/VBZ does/VBZ has/VBZ		[present]
sang/VBD was/VBD did/VBD had/VBD		[past]
singing/VBG being/VBG doing/VBG having/VBG	[present participle / continuous]
sung/VBN been/VBN done/VBN had/VBN		[past participle]

proper noun versus noun
http://www.chompchomp.com/terms/propernoun.htm

http://www.cis.upenn.edu/~treebank/

http://stackoverflow.com/questions/1833252/java-stanford-nlp-part-of-speech-labels

stanford pos tagger "future tense"
www.lancs.ac.uk/staff/hardiea/cl03_urdu.pdf
for example, the verbal auxiliary element indicating future tense: see Schmidt 1999: 106, Bhatia and Koul 2000: 331-332

//NER; stanford NER person organization
Recognizes named (PERSON, LOCATION, ORGANIZATION, MISC) and numerical entities (DATE, TIME, MONEY, NUMBER)
	http://nlp.stanford.edu/software/corenlp.shtml
*/
#define FEATURE_NER_UNDEFINED_NAME "O"
#define FEATURE_NER_DATE_NAME "DATE" 
#define FEATURE_NER_TIME_NAME "TIME" 
#define FEATURE_NER_MONEY_NAME "MONEY"
#define FEATURE_NER_NUMBER_NAME "NUMBER"
#define FEATURE_NER_PERSON_NAME "PERSON"
#define FEATURE_NER_LOCATION_NAME "LOCATION"
#define FEATURE_NER_ORGANIZATION_NAME "ORGANIZATION"
#define FEATURE_NER_MISC_NAME "MISC"
#define FEATURE_NER_DURATION_NAME "DURATION"
#define FEATURE_RELEX_FLAG_NUMBER_TYPES (FEATURE_NER_NUMBER_TYPES)
#define FEATURE_RELEX_FLAG_UNDEFINED_NAME "undefined"
#define FEATURE_RELEX_FLAG_DATE_NAME "date" 
#define FEATURE_RELEX_FLAG_TIME_NAME "time" 
#define FEATURE_RELEX_FLAG_MONEY_NAME "money"
#define FEATURE_RELEX_FLAG_NUMBER_NAME "number"
#define FEATURE_RELEX_FLAG_PERSON_NAME "person"
#define FEATURE_RELEX_FLAG_LOCATION_NAME "location"
#define FEATURE_RELEX_FLAG_ORGANIZATION_NAME "organization"
#define FEATURE_RELEX_FLAG_MISC_NAME (FEATURE_RELEX_FLAG_UNDEFINED_NAME)
#define FEATURE_RELEX_FLAG_DURATION_NAME (FEATURE_RELEX_FLAG_UNDEFINED_NAME)
#define FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_TYPES (3)

#define FEATURE_NER_INDICATES_NAME_CONCATENATION_REQUIRED_NUMBER_TYPES (4)
#define FEATURE_NER_INDICATES_NORMALISED_NER_AVAILABLE_NUMBER_TYPES (4)
#define FEATURE_NER_NAME_CONCATENATION_TOKEN " " 	//or use "_" for Relex format

#define FEATURE_POS_TAG_INDICATES_ADJECTIVE_OR_ADVERB_NUMBER_TYPES (6)

#define FEATURE_POS_TAG_PERSONAL_PRONOUN "PRP"
#define FEATURE_POS_TAG_POSSESSIVE_PRONOUN "PP$" 
#define FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_TYPES (2)

/*************************************************************************************/












/********************************* Relations *******************************************/

#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEA (2)	//OLD: 3
static string redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeA[GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEA] = {RELATION_TYPE_MODAL_AUX, RELATION_TYPE_COPULA};	//OLD: RELATION_TYPE_PASSIVE_AUX
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEB (2)
static string redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeB[GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEB] = {RELATION_TYPE_COMPLIMENT_TO_BE, RELATION_TYPE_PHRASAL_VERB_PARTICLE};	//removed RELATION_TYPE_PARTICIPIAL_MODIFIER 9 May 2012
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUBJOBJ_RELATIONS (2)
static string redistributionStanfordRelationsMultiwordPrepositionSubjObjRelations[GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUBJOBJ_RELATIONS] = {RELATION_TYPE_SUBJECT, RELATION_TYPE_OBJECT};

#define FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES (1)
static string featureQueryWordAcceptedByAlternateMethodNameArray[FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES] = {"which"};


#define FEATURE_QUERY_WORD_HOW_NUMBER_OF_TYPES (1)
#define FEATURE_QUERY_WORD_MUCH_MANY_NUMBER_OF_TYPES (2)
#define FEATURE_QUERY_HOW_MUCH_FIRST_RELATION_NUMBER_OF_TYPES (1)
#define FEATURE_QUERY_HOW_MUCH_SECOND_RELATION_NUMBER_OF_TYPES (1)
static string featureQueryWordHowNameArray[FEATURE_QUERY_WORD_HOW_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_HOW};
static string featureQueryWordMuchManyNameArray[FEATURE_QUERY_WORD_MUCH_MANY_NUMBER_OF_TYPES] = {"much", "many"};
static string featureQueryHowMuchFirstRelationNameArray[FEATURE_QUERY_HOW_MUCH_FIRST_RELATION_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_ADVMOD};
static string featureQueryHowMuchSecondRelationNameArray[FEATURE_QUERY_HOW_MUCH_SECOND_RELATION_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD};
#define FEATURE_QUERY_WORD_WHO_WHAT_NUMBER_OF_TYPES (2)
static string featureQueryWordWhoWhatNameArray[FEATURE_QUERY_WORD_WHO_WHAT_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_WHO, REFERENCE_TYPE_QUESTION_QUERY_WHAT};
#define FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_NUMBER_OF_TYPES (4)
#define FEATURE_QUERY_HOW_WHEN_WHERE_WHY_RELATION_NUMBER_OF_TYPES (2)
static string featureQueryHowWhenWhereWhyRelationNameArray[FEATURE_QUERY_HOW_WHEN_WHERE_WHY_RELATION_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_ADVMOD, RELATION_TYPE_ADJECTIVE_PREDADJ};
static string featureQueryWordHowWhenWhereWhyNameArray[FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_HOW, REFERENCE_TYPE_QUESTION_QUERY_WHEN, REFERENCE_TYPE_QUESTION_QUERY_WHERE, REFERENCE_TYPE_QUESTION_QUERY_WHY};
static string featureQueryWordHowWhenWhereWhyCrossReferenceQueryVariableNameArray[FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_HOW, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY};
/*
#define FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_SPECIAL_CASE_NUMBER_OF_TYPES (1)
static string featureQueryWordHowWhenWhereWhySpecialCaseNameArray[FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_SPECIAL_CASE_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_WHERE};
static string featureQueryWordHowWhenWhereWhySpecialCaseCrossReferenceQueryVariableNameArray[FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_SPECIAL_CASE_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE};
*/
#define FEATURE_QUERY_WORD_WHAT_NUMBER_OF_TYPES (1)
static string featureQueryWordWhatNameArray[FEATURE_QUERY_WORD_WHAT_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_WHAT};
#define RELATION_TYPE_QVARIABLE_NUMBER_OF_TYPES (4)
static string relationTypeQVariableNameArray[RELATION_TYPE_QVARIABLE_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_HOW};	//had to add REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_HOW here - need to check execution with relex parser is not affected

static string relationTypePropositionTimeNameArray[RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES] = {"in", "on", "after", "ago", "before", "between", "by", "during", "for", "to", "till", "until", "past", "since", "up_to", "within", "over", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN};
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
	
static string relationTypeObjectNameArray[RELATION_TYPE_OBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT, RELATION_TYPE_PARTICIPIAL_MODIFIER, RELATION_TYPE_OBJECT_THAT};
#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
static string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT, RELATION_TYPE_SUBJECT_EXPLETIVE};
#else
static string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT};
#endif
static string relationTypeAdjectiveNameArray[RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD, RELATION_TYPE_ADJECTIVE_PREDADJ, RELATION_TYPE_ADJECTIVE_ADVMOD};
static string relationTypePossessiveNameArray[RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_POSSESSIVE, RELATION_TYPE_PRENOMIAL_MODIFIER};

static string relationGovernorCompositionNameArray[RELATION_GOVERNOR_COMPOSITION_NUMBER_OF_TYPES] = {RELATION_GOVERNOR_COMPOSITION_1, RELATION_GOVERNOR_COMPOSITION_2, RELATION_GOVERNOR_COMPOSITION_3, RELATION_GOVERNOR_COMPOSITION_4};
static string relationGovernorDefinitionNameArray[RELATION_GOVERNOR_DEFINITION_NUMBER_OF_TYPES] = {RELATION_ENTITY_BE};

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
static string relationTypeQuantityOrMeasureSwitchedNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY_MOD, RELATION_TYPE_MEASURE_PER, RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN};
static string relationTypeMeasureNameArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_PER, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME, RELATION_TYPE_MEASURE_UNKNOWN, RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN};
static int relationTypeMeasureNameTypeIndexArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {MEASURE_TYPE_DISTANCE, MEASURE_TYPE_PER, MEASURE_TYPE_SIZE, MEASURE_TYPE_TIME, MEASURE_TYPE_UNKNOWN, MEASURE_DEPENDENCY_UNKNOWN};
static string relationTypeMeasureDependencyNameArray[RELATION_TYPE_MEASURE_DEPENDENCY_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_PER, RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN};

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
static string relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_NUMBER_OF_TYPES][GIA_NUMBER_OF_RELEX_VERSUS_STANFORD_DEPENDENCY_RELATION_DISCREPANCIES] = {{RELATION_TYPE_COMPLIMENT_TO_BE, RELATION_TYPE_COMPLIMENT_TO_DO, RELATION_TYPE_APPOSITIVE_OF_NOUN, RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_SUBJECT, RELATION_TYPE_POSSESSIVE, RELATION_TYPE_QUANTITY, RELATION_TYPE_QUANTITY, RELATION_TYPE_QUANTITY_MOD, RELATION_TYPE_SUBJECT}, {STANFORD_RELATION_TYPE_COMPLIMENT_TO_BE, STANFORD_RELATION_TYPE_COMPLIMENT_TO_DO, STANFORD_RELATION_TYPE_APPOSITIVE_OF_NOUN, STANFORD_RELATION_TYPE_OBJECT, STANFORD_RELATION_TYPE_INFINITIVAL_MODIFIER, STANFORD_RELATION_TYPE_PASSIVE_NOMINAL_SUBJECT, STANFORD_RELATION_TYPE_SUBJECT, STANFORD_RELATION_TYPE_POSS2, STANFORD_RELATION_TYPE_QUANTITY, STANFORD_RELATION_TYPE_ELEMENT_OF_COMPOUND_NUMBER, STANFORD_RELATION_TYPE_QUANTITY_MOD, STANFORD_RELATION_TYPE_AGENT}};
	/*
	//consider STANFORD_RELATION_TYPE_PASSIVE_NOMINAL_SUBJECT as SUBJECT not OBJECT??
	*/
	
	
/*************************************************************************************/

/********************************* Features *******************************************/


static string posTagVerbPastArray[FEATURE_POS_TAG_VERB_PAST_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VBD, FEATURE_POS_TAG_VBN};
static string posTagVerbPresentArray[FEATURE_POS_TAG_VERB_PRESENT_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VBP, FEATURE_POS_TAG_VBZ};
static string posTagVerbProgressiveArray[FEATURE_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VBG};

static string posTagProperNounArray[FEATURE_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NNP, FEATURE_POS_TAG_NNPS};
static string posTagCommonNounArray[FEATURE_POS_TAG_COMMON_NOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NN, FEATURE_POS_TAG_NNS};
static string posTagSingularNounArray[FEATURE_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NN, FEATURE_POS_TAG_NNP};
static string posTagPluralNounArray[FEATURE_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NNS, FEATURE_POS_TAG_NNPS};

static string relationAuxillaryGovernerIndicatesFutureTenseArray[RELATION_TYPE_AUXILLARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES] = {"will"};

static int featureRelexPOSTypeCrossReferenceWordnetWordTypeArray[FEATURE_RELEX_POS_NUMBER_OF_TYPES] = {GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_ADJ, GRAMMATICAL_WORD_TYPE_ADV, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_NOUN, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_VERB, GRAMMATICAL_WORD_TYPE_UNDEFINED};

static string featurePOSTagMinimalArray[FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL] = {FEATURE_POS_TAG_VB, FEATURE_POS_TAG_VBD, FEATURE_POS_TAG_VBG, FEATURE_POS_TAG_VBN, FEATURE_POS_TAG_VBP, FEATURE_POS_TAG_VBZ, FEATURE_POS_TAG_NN, FEATURE_POS_TAG_NNS, FEATURE_POS_TAG_NNP, FEATURE_POS_TAG_NNPS, FEATURE_POS_TAG_ADJECTIVE, FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE, FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE, FEATURE_POS_TAG_ADVERB, FEATURE_POS_TAG_ADVERB_COMPARATIVE, FEATURE_POS_TAG_ADVERB_SUPERLATIVE};
static string featurePOSTagCrossReferenceRelexPOSTypeArray[FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL] = {FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME};

static string featureNERTypeArray[FEATURE_NER_NUMBER_TYPES] = {FEATURE_NER_UNDEFINED_NAME, FEATURE_NER_DATE_NAME, FEATURE_NER_TIME_NAME, FEATURE_NER_MONEY_NAME, FEATURE_NER_NUMBER_NAME, FEATURE_NER_PERSON_NAME, FEATURE_NER_LOCATION_NAME, FEATURE_NER_ORGANIZATION_NAME, FEATURE_NER_MISC_NAME, FEATURE_NER_DURATION_NAME};
static string featureRelexFlagTypeArray[FEATURE_RELEX_FLAG_NUMBER_TYPES] = {FEATURE_RELEX_FLAG_UNDEFINED_NAME, FEATURE_RELEX_FLAG_DATE_NAME, FEATURE_RELEX_FLAG_TIME_NAME, FEATURE_RELEX_FLAG_MONEY_NAME, FEATURE_RELEX_FLAG_NUMBER_NAME, FEATURE_RELEX_FLAG_PERSON_NAME, FEATURE_RELEX_FLAG_LOCATION_NAME, FEATURE_RELEX_FLAG_ORGANIZATION_NAME, FEATURE_RELEX_FLAG_MISC_NAME, FEATURE_RELEX_FLAG_DURATION_NAME};
static int featureNERexplicitTypeArray[FEATURE_NER_EXPLICIT_NUMBER_TYPES] = {FEATURE_NER_DATE, FEATURE_NER_TIME, FEATURE_NER_MONEY, FEATURE_NER_NUMBER, FEATURE_NER_PERSON, FEATURE_NER_LOCATION, FEATURE_NER_ORGANIZATION, FEATURE_NER_MISC, FEATURE_NER_DURATION};
static int featureNERindicatesProperNounTypeArray[FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_TYPES] = {FEATURE_NER_PERSON, FEATURE_NER_LOCATION, FEATURE_NER_ORGANIZATION};	//this array is not used by stanford parser as Standford Core NLP explicitly marks entities as 'proper noun' within their stanfordPOS tag (it is used by the Relex parser however to determine proper nouns)
static int featureNERindicatesNameConcatenationRequiredTypeArray[FEATURE_NER_INDICATES_NAME_CONCATENATION_REQUIRED_NUMBER_TYPES] = {FEATURE_NER_PERSON, FEATURE_NER_LOCATION, FEATURE_NER_ORGANIZATION, FEATURE_NER_MISC};
static int featureNERindicatesNormalisedNERavailableTypeArray[FEATURE_NER_INDICATES_NORMALISED_NER_AVAILABLE_NUMBER_TYPES] = {FEATURE_NER_DATE, FEATURE_NER_TIME, FEATURE_NER_MONEY, FEATURE_NER_NUMBER};
static string featurePOSindicatesAdjectiveOrAdverbTypeArray[FEATURE_POS_TAG_INDICATES_ADJECTIVE_OR_ADVERB_NUMBER_TYPES] = {FEATURE_POS_TAG_ADJECTIVE, FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE, FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE, FEATURE_POS_TAG_ADVERB, FEATURE_POS_TAG_ADVERB_COMPARATIVE, FEATURE_POS_TAG_ADVERB_SUPERLATIVE};
static string featurePOSindicatesPronounTypeArray[FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_TYPES] = {FEATURE_POS_TAG_PERSONAL_PRONOUN, FEATURE_POS_TAG_POSSESSIVE_PRONOUN};







/*************************************************************************************/



/*
noun = joe
verb = rides
adjective = happy
*/

void initialiseGIATranslatorForTexualContextOperations();

bool isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe(Relation * currentRelationInList, GIAEntityNode * GIAEntityNodeArray[], int relationGovernorIndex, int NLPdependencyRelationsType);
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

void applyEntityAlreadyExistsFunction(GIAEntityNode * entity);
void disableEntityBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity);
void disableEntityAndInstanceBasedUponFirstSentenceToAppearInNetwork(GIAEntityNode * entity);

void recordSentenceConceptNodesAsPermanentIfTheyAreStillEnabled(bool GIAEntityNodeArrayFilled[], GIAEntityNode * GIAEntityNodeArray[]);
void recordConceptNodesAsDisabledIfTheyAreNotPermanent(unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap);
void recordConceptNodesAsNonPermanentIfTheyAreDisabled(unordered_map<string, GIAEntityNode*> *conceptEntityNodesListMap);

void convertRelexPOSTypeToWordnetWordType(string * relexPOStype, int * wordNetPOS);
void convertStanfordPOSTagToRelexPOSTypeAndWordnetWordType(string * POStag, string * relexPOStype, int * wordNetPOS);		

void generateTempFeatureArray(Feature * firstFeatureInList, Feature * featureArrayTemp[]);	//used for intrafunction memory allocation purposes only

bool checkEntityHasPropertyThatWasDeclaredInContext(GIAEntityNode * entityNode);			//current textual context (eg current paragraph) 	//added 1j7d 9 May 2012

GIAEntityNode * getEntityPropertyThatWasDeclaredInImmediateContext(GIAEntityNode * entityNode);		//immediate textual context (ie, current sentence) 	//added 1j7e 9 May 2012
bool checkEntityHasPropertyThatWasDeclaredInImmediateContext(GIAEntityNode * entityNode);		//immediate textual context (ie, current sentence) 	//added 1j7e 9 May 2012

#endif

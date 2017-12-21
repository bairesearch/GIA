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
 * File Name: GIAtranslatorDefs.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e6b 16-December-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/

#ifndef HEADER_GIA_TRANSLATOR_DEFS
#define HEADER_GIA_TRANSLATOR_DEFS

#include "GIAentityNodeClass.hpp"
#include "GIAsentenceClass.hpp"

#define GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_TYPE1			//added 30 June 2013 - 1s7f
#define GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_TYPE2			//added 29 June 2013 - 1s7c / modified [to condition] 30 June 2013 - 1s7f
#define GIA_TRANSLATOR_LINK_DEPENDENT_ACTIONS_DEFAULT_CONDITION_NAME "involves"
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP		//added 30 June 2013 - 1s7f



#define GIA_TRANSLATOR_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES	//added 11 August 2012	[this is designed to work with GIA_ALIASES]
	#ifdef GIA_1S3A_ADD_INTERPRETION_OF_IN_AS_POSSESSIVE_FOR_SUBSTANCES
		#define GIA_TRANSLATOR_INTERPRET_IN_AS_POSSESSIVE_FOR_SUBSTANCES
	#endif
	//#define GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES	//added 8 August 2012		//Not used - already coded - reuse Relex code in linkConditions instead (GIA_TRANSLATOR_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES)

#ifndef STANFORD_PARSER_USE_POS_TAGS
	#define GIA_TRANSLATOR_INTERPRET_OF_AS_OBJECT_FOR_CONTINUOUS_VERBS //added 28 October 2012b
#endif

#define GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES	//added 9 August 2012 [this is designed to work with GIA_ALIASES]
#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES
	//#define GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_OLD
	#ifdef GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_OLD
		#define GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_1_GOVERNOR_DEFINITE_DEPENDENT_INDEFINITE
		#define GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_2_GOVERNOR_PLURAL_DEPENDENT_PLURAL
		#define GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_3_GOVERNOR_INDEFINITE_DEPENDENT_INDEFINITE
		#define GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_4_GOVERNOR_NAME_DEPENDENT_INDEFINITE
		#define GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_5_GOVERNOR_DEFINITE_DEPENDENT_NAME
		#define GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_6_GOVERNOR_NAME_DEPENDENT_DEFINITE
		#define GIA_DEFINE_SUBSTANCES_BASED_UPON_DETERMINATES_OF_DEFINITION_ENTITIES_CASE_7_GOVERNOR_INDEFINITE_DEPENDENT_NAME
	#endif
#endif

#ifdef GIA_ALIASES
	#ifndef GIA_1S8E_LOW_PRI_RELEX_UPDATE_CHECK_THAT_IT_DOESNT_BREAK_STANFORD_OPTIMISATION_REMOVE_TIME_QUERY_ALIAS_ANSWERS
		#define GIA_WHAT_IS_THE_TIME_QUERY_ALIAS_ANSWERS_ONLY_WORK_WITH_STANFORD
	#endif
	#define GIA_WHICH_QUERY_ALIAS_ANSWERS
	#define GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF    //added 8 August 2012   //currently only implemented for Stanford Parser (need to test with and then enable for Relex)
	#define GIA_WHO_QUERY_ALIAS_ANSWERS
	#ifdef GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF
		//#assert GIA_REDISTRIBUTE_STANFORD_RELATIONS_IGNORE_NSUBJ_AND_PREPOSITION_AND_COP_AND_DET
		#define GIA_REDISTRIBUTE_RELATIONS_SUPPORT_WHAT_IS_THE_NAME_NUMBER_OF_QUERIES	//added 8 August 2012
		#define GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME "name"
		#define GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NUMBER "number"
	#endif
#endif

#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_IGNORE_NSUBJ_AND_PREPOSITION_AND_COP_AND_DET	/added 9 August 2012

//#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_PARTMOD_DEAL_WITH_PROGRESSIVE_ANOMALY	//removed 2k8c
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_ACTION_PREPOSITION_ACTION
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_AND_XCOMP		//working on this now (1j6h)
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP_WITH_BE	//renamed 30 June 2013 - used to be called GIA_REDISTRIBUTE_STANFORD_RELATIONS_DEP_AND_PREP
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION	//added in addition to this; the pre-process of "two word prepositions" eg from http://en.wikipedia.org/wiki/List_of_English_prepositions, or post process (currently implemented)
//#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_NSUBJ_AND_PREPOSITION_OLD



//#ifndef GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS	//this condition has been removed for debugging output replication purposes (although this condition is not necessary in practice)
	#define FILL_NER_ARRAY_AFTER_RELEX_PARSE_FOR_STANFORD_EQUIVALENT_PROPER_NOUN_DETECTION 	//added 26 April 2012 [UNTESTED]
//#endif

#ifdef USE_CE
	#ifdef GIA_STANFORD_CORE_NLP_USE_CODEPENDENCIES
		#define GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING_ONLY_ACCEPT_INTRASENTENCE_STANFORD_COREFERENCES		//this is required, as CE will construct temporary codeextension heirachies not containing every sentence; thereby nullifying stanford coreNLP codependencies out of these temporary heirachies
	#endif
#endif

#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE_AND_TEMPORAL_MODIFIER
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE_AND_OBJECT_OF_PREPOSITION		//added 7 August 2012b
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_PHRASAL_VERB_PARTICLE		//this was previously disabled because it was deemed inappropriate for 'The disaster happened over night.'  [prt(happened-3, over-4)] - but this case has now been integrated

//#define GIA_ENFORCE_USE_OF_RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES


#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
	#define GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
#endif
//#define GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD	//test only
#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
	#define GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
#endif

#define GIA_DO_NOT_ADD_SUBSTANCES_ACTIONS_AND_CONDITIONS_TO_DISABLED_NETWORK_INDEX_ENTITIES	//NB this only works for substances added to networkIndex entities, not substances added to substances (however this should be OK, as disabling of nodes should take place before substances are added)






#ifdef GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
	#define GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_PARAGRAPH	//default: enabled		//Relex Only
#else
	#define GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_SENTENCE	//default: disabled
#endif
//#define GIA_ENABLE_REFERENCE_LINKING_CLEAR_REFERENCES_EVERY_SENTENCE	//default: disabled		//Relex and Stanford supported (untested with stanford)


//#define GIA_OPTIMISE_PERFORMANCE_FOR_RELEX_PATENT_QUERIES_REPLICATION_RATHER_THAN_RELEX_PATENT_SYNTACTIC_PROTOTYPE_OUTPUT_REPLICATION			//not ever used - old for testing
//#define GIA_COLLAPSE_COP_RELATION_DEPENDENT_BE_TO_APPOS_NOT_PREDADJ_OLD										//not ever used - old for testing

#define GIA_SUPPORT_MULTIPLE_ACTION_INSTANCES_PER_ACTION
#define GIA_SUPPORT_MULTIPLE_CONDITION_INSTANCES_PER_CONDITION

//#define USE_OLD_SUBJ_OBJ_ONLY_ONE_PAIR_RESTRICTION_METHOD 	//default: disabled



#define GIA_DEAL_WITH_TEMPORARY_ISSUE
#ifdef GIA_DEAL_WITH_TEMPORARY_ISSUE
	#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION				//EITHER this is required
	//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B2_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY	//OR this is required
#endif

//#define GIA_TRANSLATOR_USE_NEW_ACTION_SUBJECT_RELATION_GOVERNOR_DEFINITION_IMPLEMENTATION	//if undefined then "tom is being an idiot"= an instance of tom is an idoit. if defined then tom has (a substance) of idiocy [NOT YET IMPLEMENTED]


//Stanford/Relex independent conditions:
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1A_RELATIONS_DISREGARD_REDUNDANT_DEFINITION_RELATIONS
	#define GIA_IGNORE_MEANINGLESS_RELATIONS
#endif
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
//#define GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1E_RELATIONS_TREAT_UNQUALIFIED_RELATIONS_AS_CONDITIONS_ALSO	//NB this relates to GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_4A_RELATIONS_DEFINE_SUBSTANCES_BASED_UPON_INDIRECT_OBJECTS	//added 27 April 2012 - required because of the case; "What did the officer give transportation to?" where transportation is not singular, and therefore will not be defined as a substance until indirectObjects are defined (after the action 'give' has already been linked to its networkIndex entity). NB "What did the officer give a ride to?" does not face the same problem as 'ride' is tagged as singular by relex and therefore will be assigned as a substance before the action 'give' is linked to it
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5A_RELATIONS_ASSIGN_TIME_NODES_IN_RELEX_THE_SAME_AS_STANFORD	//Case 5= stanford specific
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5B_RELATIONS_COMPENSATE_FOR_INACCURATE_STANFORD_DATE_TAGGING
//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5C_FEATURES_STANFORD_NER_INDICATES_NAME_CONCATENATION_REQUIRES_POS_NNP

#ifdef GIA_RELEX

		#define GIA_TRANSLATOR_RETAIN_SUSPECTED_REDUNDANT_RELEX_CODE

	//GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE:

		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3A_PREPOSITIONS_INTERPRET_PREPOSITION_OF_AS_EITHER_CONDITION_OR_SUBSTANCE_LINK_DEPENDING_UPON_ACTION_OR_SUBSTANCE

	//GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE:

		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
		#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_OR_TOBE_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TODO_AND_SUBJECT_RELATION_WITH_BE_AS_DEFINITION_LINK
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1C_RELATIONS_TREAT_TOBE_AND_SUBJECT_RELATION_AS_SUBSTANCE_LINK_AND_ACTION_DEFINITION
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1D_RELATIONS_REMOVE_ARTEFACT_NETWORK_INDEX_ENTITY_NODES_ADVANCED	//Relex only at present [relies upon isAdjectiveNotAnAdvmodAndRelationGovernorIsNotBe() and GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS]
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_2A_GRAMMAR_TREAT_PRESENT_PERFECT_AS_PAST_TENSE							//Relex/Stanford independent
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_PLUS_OBJECT_RELATION_ALL_WITH_A_DEFINITION_FUNCTION_AS_PROPERTY_LINKS
			#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_OR_HAVING_INTO_A_CONDITION_DEFINITION			//CHECK THIS; Stanford compatibility
		#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_OBJECT_PLUS_SUBJECT_RELATION_WHERE_ADVERB_HAS_SAME_ARGUMENT_AS_SUBJECT_AS_CONDITION
			#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_POSSESSION_EG_HAVING_INTO_AN_ARBITRARY_SUBJECT_PROPERTY	//CHECK THIS; Stanford compatibility
			#define GIA_TRANSLATOR_TRANSFORM_THE_ACTION_OF_BEING_EG_BEING_INTO_AN_ARBITRARY_SUBJECT_DEFINITION	//CHECK THIS; Stanford compatibility
		#endif
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B_RELATIONS_TREAT_ADVERB_PLUS_SUBJECT_RELATION_AS_ACTION_CONDITION
			#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1B2_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
				#define GIA_IGNORE_DUPLICATE_COMPARISON_VARIABLES_IN_QUERY
			#endif
		#endif
#endif

#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES
	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_GENERATE_MEASURES
		//#define GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_6A_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE
			#define GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ
			//#define GIA_COLLAPSE_ADVMOD_RELATION_GOVERNOR_BE_TO_PREDADJ_NOT_SUBJ_OLD
		#endif
	#endif
#endif


#define GIA_TRANSLATOR_ACTION_DEFINITION_CODE_SIMPLIFICATION

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
	#define RELATION_TYPE_PREPOSITION_REDUCTION_MAX_NUMBER_VARIATIONS (4)
	#define RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_NUMBER_OF_TYPES 4
	#define RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING ((string)"a`")	//à: character not supported by ASCII
	#define RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_NUMBER_OF_TYPES 3
	#define RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING ((string)"de")
	#define RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_NUMBER_OF_TYPES 2
	#define RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_REPLACEMENT_STRING ((string)"pour")
#endif

#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
	#define GIA_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE_ASSIGN_DETERMINATE_SINGULAR	//added 14 August 2012 for GIA_ALIASES/defineSubstancesBasedOnDeterminatesOfDefinitionEntities compatibility
	#define RELATION_TYPE_TREAT_AS_PRONOUN_IE_SUBSTANCE_NUMBER_OF_TYPES 1
#endif



#define GIA_ASSIGN_UNIQUE_ACTION_NODE_TO_EACH_ACTION_INSTANCE_OF_A_GIVEN_ACTION_NAME	//always true
#define GIA_ASSIGN_SUBSTANCE_TO_ALL_DEFINITIVE_NOUNS		//NB must disable this for large.xml to work (NB this bug was issue introduced after GIA Archive - 1a5d - 04May11a, eg GIA Archive - 1a5e - 04May11a)
#define GIA_ASSIGN_SUBSTANCE_TO_ALL_NOUNS_WITH_DETERMINATES
#define GIA_ASSIGN_SUBSTANCE_TO_ALL_PRONOUNS
//#define GIA_ALWAYS_ASSIGN_NEW_SUBSTANCE_WHEN_DEFINING_A_SUBSTANCE
//#define GIA_ALWAYS_ASSIGN_NEW_SUBSTANCE_WHEN_ATTACHING_A_SUBSTANCE

#define SUBJECT_INDEX (0)
#define OBJECT_INDEX (1)

#define SECONDS_IN_YEAR (365*24*60*60)



/********************************* Relations* ******************************************/


//special relations;
#define RELATION_TYPE_INDIRECT_OBJECT "_iobj"
#define RELATION_TYPE_PARATAXIS "_parataxis"	//eg "The guy, Akari said, left..." //added 13 February 2011

//stanford specific (non Relex) relations implemented in redistributeStanfordRelationsAdverbalClauseModifierAndComplement()/redistributeStanfordRelationsClausalSubject()/redistributeStanfordRelationsPhrasalVerbParticle()/redistributeStanfordRelationsMultiwordPreposition{}:
#define STANFORD_RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER "advcl" 			//advcl(happen, fall)	The accident happened as the night was falling. 	Relex: as(happen, fall)
#define STANFORD_RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER "mark"		//mark(fall, as)  	The accident happened as the night was falling. 	Relex: as(happen, fall)
#define STANFORD_RELATION_TYPE_CLAUSAL_COMPLEMENT "ccomp" 				//ccomp(say, like)	He says that you like to swim				Relex: that(say, like) 		GIA: implement as object of action 	OLD: GIA: implement this as an action property
#define STANFORD_RELATION_TYPE_CLAUSAL_SUBJECT "csubj" 					//csubj (make, say)	What she said makes sense. [dobj ( said-3 , What-1 )]	Relex:  Generated by Stanford; Relex uses plain _subj. 			GIA applies it to the object; ie subj (make, What)
#define STANFORD_RELATION_TYPE_NEGATIVE "neg" 						//neg(have, n't) 	haven't 						Relex usually generates NEGATIVE-FLAG(have, T)
//#define STANFORD_RELATION_TYPE_COMPLEMENT_OF_PREPOSITION "pcomp" 			//pcomp(garden, in)	It happened in the garden 				Relex: RelEx uses the general prepositional relations instead, e.g. in(happen, garden)  {OR pobj (go, store)/ pcomp (go, to) => to(go, store)} 	//only used by Stanford uncollapsed dependencies:
#define STANFORD_RELATION_TYPE_PHRASAL_VERB_PARTICLE "prt" 				//prt(shut, down) 	They shut down the station. 				RelEx always contracts the particle to create a polyword: 	e.g. _subj(shut_down, they)_obj(shut_down, station)	GIA: change the entity name 'shut' to 'shut_down'
//#define STANFORD_RELATION_TYPE_SMALL_CLAUSE_COMPLEMENT_OF_VERB "sc" 			//sc(stop, to) 		Alice forced him to stop.				[THIS APPEARS INCORRECT: stanford currently gives; xcomp ( forced-2 , stop-5 )]		Relex:RelEx uses the general prepositional relations instead, e.g. to(force,stop)
#define RELATION_TYPE_ADVERBAL_CLAUSE_MODIFIER "_advcl"
#define RELATION_TYPE_COMPLEMENT_OF_ADVERBAL_CLAUSE_MODIFIER "_mark"
#define RELATION_TYPE_CLAUSAL_COMPLEMENT "_ccomp"
//#define RELATION_TYPE_COMPLEMENTIZER "_complm"
#define RELATION_TYPE_CLAUSAL_SUBJECT "_csubj"
#define RELATION_TYPE_NEGATIVE "_neg"
//#define RELATION_TYPE_COMPLEMENT_OF_PREPOSITION "_pcomp"
#define RELATION_TYPE_PHRASAL_VERB_PARTICLE "_prt"
#define STANFORD_RELATION_TYPE_PHRASAL_VERB_PARTICLE "prt"	//not used
//#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
#define STANFORD_RELATION_TYPE_PHRASAL_VERB_PARTICLE2 "compound:prt"			//compound:prt(fell-4, off-5)	The red ball fell off the table.
//#endif
//#define RELATION_TYPE_SMALL_CLAUSE_COMPLEMENT_OF_VERB "_sc"

//redundant 'that' nodes;
#define STANFORD_RELATION_TYPE_COMPLEMENTIZER "complm" 			//complm(like, that)	He says that you like to swim				Relex: that(say, like) 		GIA: implement this as an action substance???
#define RELATION_TYPE_COMPLEMENTIZER "_complm"
#define RELATION_DEPENDENT_THAT "that"

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
//#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
#define STANFORD_RELATION_TYPE_TEMPORAL_MODIFIER2 "nmod:tmod"
//#endif
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
#define STANFORD_RELATION_TYPE_ABBREVIATION_MODIFIER "abbrev"
#define STANFORD_RELATION_TYPE_POSSESSIVE "possessive"
#define STANFORD_RELATION_TYPE_PREPOSITION_MODIFIER2OLD "pmod"
#define STANFORD_RELATION_TYPE_SEMANTIC_DEPENDENT "sdep"
#define STANFORD_RELATION_TYPE_XSUBJ "xsubj"			//?? imperative?
#define RELATION_TYPE_XSUBJ "xsubj"
*/

//stanford relation redistribution (redistributeStanfordRelationsMultiwordPreposition);
#define STANFORD_RELATION_TYPE_PREPOSITION_MODIFIER "prep"	//deal with during stanford reduction (added 3 May 2012)
#define RELATION_TYPE_PREPOSITION_MODIFIER "_prep"
//#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
#define RELATION_TYPE_PREPOSITION_MODIFIER2 "nmod:"
#define RELATION_TYPE_PREPOSITION_MODIFIER2_LENGTH (5)
//#endif

//advanced referencing; "the chicken that ate a pie rowed the boat." / "the chicken that ate a pie has measles."	[that ate a pie / rcmod(chicken-2, ate-4)]
#define STANFORD_RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER "rcmod"
#define RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER "_rcmod"
//#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
#define STANFORD_RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER2 "acl:relcl"
//#endif

//conditions: prepositions [predicates????]
#define RELATION_TYPE_RELEX_NON_PREPOSITION_FIRST_CHARACTER '_'
#define RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION "_pobj"
#define RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION "_psubj"


//#define ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION	//in final implementation, the arbitrary subject should be determined during the referencing stage of sentence parsing
#ifndef ARBITRARY_SUBJECT_FINAL_IMPLEMENTATION
	#define ARBITRARY_SUBJECT_SPECIAL_NETWORK_INDEX_NODE_NAME "arbitrarySubject"
#endif



//action/substance reasons [NOT YET IMPLEMENTED ???]
#define RELATION_TYPE_PREPOSITION_SUCH_THAT "such"
#define RELATION_TYPE_PREPOSITION_SO "so"
#define RELATION_TYPE_PREPOSITION_REASON_NUMBER_OF_TYPES 2
	//because?


#define GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES 1
static int dependencyRelationsTypes[GIA_NLP_PARSER_NUMBER_OF_TYPES] = {GIA_NLP_DEPENDENCY_RELATIONS_PARSER_RELEX_DEFAULT_DEPENDENCY_RELATIONS_TYPE, GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_CORENLP_DEFAULT_DEPENDENCY_RELATIONS_TYPE, GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_PARSER_DEFAULT_DEPENDENCY_RELATIONS_TYPE};
static int referenceTypeHasDeterminateCrossReferenceNumberArray[GRAMMATICAL_NUMBER_TYPE_INDICATE_HAVE_DETERMINATE_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_SINGULAR};


#define REFERENCE_TYPE_LOCATION "there"				//_advmod


#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE_REPLACEMENT "RELATION_TYPE_PREPOSITION_AT"
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN_REPLACEMENT "RELATION_TYPE_PREPOSITION_AT"	//must also set hasAssociatedTime to true
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY_REPLACEMENT "RELATION_TYPE_PREPOSITION_BECAUSE"



//actions (obj relationships):
#define RELATION_TYPE_OBJECT "_obj"			//eg eats y	[? be y]
#define RELATION_TYPE_OBJECT_THAT_RELEX "_that"		//there is a place that we go	//relation type added by Relex for implicit "that", eg "Moses knew I was angry."
#define RELATION_TYPE_OBJECT_THAT_RELEX_EXPLICIT_PREPOSITION "that"		//relation type added by Relex for explicit "that" (same as a Relex preposition), eg "He says that you like to swim."
#define RELATION_TYPE_OBJECT_NUMBER_OF_TYPES 2
#define STANFORD_RELATION_TYPE_OBJECT "dobj"
#define STANFORD_RELATION_TYPE_INFINITIVAL_MODIFIER "infmod"				//Relex usually generates a plain _obj
#define STANFORD_RELATION_TYPE_PASSIVE_NOMINAL_SUBJECT "nsubjpass" 			//nsubjpass(thrown, rocks) 	rocks were thrown 				Relex: RelEx identifies these as _obj, and marks verb with passive feature.
#define STANFORD_RELATION_TYPE_PARTICIPIAL_MODIFIER "partmod" 				//RelEx usually generates a plain _obj. [however GIA keeps it as a separate relation, as it uses it elsewhere for multiword preposition collapse purposes]
#define RELATION_TYPE_PARTICIPIAL_MODIFIER "_partmod"
static string relationTypeObjectNameArray[RELATION_TYPE_OBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT_THAT_RELEX};	//removed RELATION_TYPE_PARTICIPIAL_MODIFIER 9 May 2012 (this is now dealt with in GIAtranslatorRedistributeRelationsStanford.cpp)


//actions (subj relationships):
#define RELATION_TYPE_SUBJECT "_subj"	//eg x eats 	[? be x]
#define RELATION_TYPE_SUBJECT_EXPLETIVE "_expl"		//eg goes there	//OLD: "there" is currently interpreted as a subject of an action
#define STANFORD_RELATION_TYPE_SUBJECT "nsubj"
#define STANFORD_RELATION_TYPE_AGENT "agent" 						//agent(kill, police) 	The man has been killed by the police.  		Relex: by(kill, police) 	GIA: subj(kill, police)
#define STANFORD_PARSER_PREPOSITION_BY "prep_by"
#ifdef GIA_INTERPRET_EXPLETIVE_AS_SUBJECT_OF_ACTION
#define RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES 2
static string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT, RELATION_TYPE_SUBJECT_EXPLETIVE};
#else
#define RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES 1
static string relationTypeSubjectNameArray[RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT};
#endif

//substances (actions)
#define RELATION_TYPE_SUBJECT_OBJECT_NUMBER_OF_TYPES 2
static string relationTypeSubjectObjectNameArray[RELATION_TYPE_SUBJECT_OBJECT_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT, RELATION_TYPE_OBJECT};
#define RELATION_TYPE_SUBJECT_CSUBJ_NUMBER_OF_TYPES 2
static string relationTypeSubjectCsubjNameArray[RELATION_TYPE_SUBJECT_CSUBJ_NUMBER_OF_TYPES] = {RELATION_TYPE_SUBJECT, STANFORD_RELATION_TYPE_CLAUSAL_SUBJECT};

//substances (descriptive relationships)
#define RELATION_TYPE_ADJECTIVE_AMOD "_amod"	  //eg x is happy
#define RELATION_TYPE_ADJECTIVE_PREDADJ "_predadj"
#define RELATION_TYPE_ADJECTIVE_ADVMOD "_advmod"
#define RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES 3
#define RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES 2
#define RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_SAME_REFERENCE_SET_NUMBER_OF_TYPES 2
#define RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_DIFFERENT_REFERENCE_SET_NUMBER_OF_TYPES 1
static string relationTypeAdjectiveNameArray[RELATION_TYPE_ADJECTIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD, RELATION_TYPE_ADJECTIVE_PREDADJ, RELATION_TYPE_ADJECTIVE_ADVMOD};
static string relationTypeAdjectiveWhichImplyEntityInstanceNameArray[RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_ENTITY_INSTANCE_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD, RELATION_TYPE_ADJECTIVE_ADVMOD};
static string relationTypeAdjectiveWhichImplySameReferenceSetNameArray[RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_SAME_REFERENCE_SET_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD, RELATION_TYPE_ADJECTIVE_ADVMOD};
static string relationTypeAdjectiveWhichImplyDifferentReferenceSetNameArray[RELATION_TYPE_ADJECTIVE_WHICH_IMPLY_DIFFERENT_REFERENCE_SET_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_PREDADJ};


//substances (possessive relationships)
#define RELATION_TYPE_POSSESSIVE "_poss"	//eg his bike	[bike him]		/its bike
//#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
#define STANFORD_RELATION_TYPE_POSSESSIVE2 "nmod:poss"
//#endif
#define RELATION_TYPE_PRENOMIAL_MODIFIER "_nn"
#define RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES 1
#define RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES 1
#define STANFORD_RELATION_TYPE_GENETIVE_MODIFIER_OF_NOUN "gen"				//gen(cookie, Alice)	Alice's cookie						[THIS APPEARS INCORRECT: stanford currently gives; poss(cookie, Alice)] 	Relex: Identical to RelEx output _poss.
#define STANFORD_RELATION_TYPE_POSS2 (STANFORD_RELATION_TYPE_GENETIVE_MODIFIER_OF_NOUN)
static string relationTypePossessiveNameArray[RELATION_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {RELATION_TYPE_POSSESSIVE};
static string relationTypePrenominalModifierNameArray[RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES] = {RELATION_TYPE_PRENOMIAL_MODIFIER};

#define RELATION_TYPE_QUALITY_NUMBER_OF_TYPES 3
static string relationTypeQualityNameArray[RELATION_TYPE_QUALITY_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD, RELATION_TYPE_ADJECTIVE_PREDADJ, RELATION_TYPE_ADJECTIVE_ADVMOD};

//properties:	[NB properties are attached to either another substance or a straight entity);]
//substances (derived from obj/subj relationships);
#define RELATION_GOVERNOR_COMPOSITION_1 "contain"	//eg x contains y
#define RELATION_GOVERNOR_COMPOSITION_2 "comprise"	//eg x comprises y
#define RELATION_GOVERNOR_COMPOSITION_3 "have"		//eg x has y
#define RELATION_GOVERNOR_COMPOSITION_NUMBER_OF_TYPES 3
static string relationGovernorCompositionNameArray[RELATION_GOVERNOR_COMPOSITION_NUMBER_OF_TYPES] = {RELATION_GOVERNOR_COMPOSITION_1, RELATION_GOVERNOR_COMPOSITION_2, RELATION_GOVERNOR_COMPOSITION_3};


//networkIndexes:
#define RELATION_GOVERNOR_DEFINITION_NUMBER_OF_TYPES 1
#define RELATION_TYPE_APPOSITIVE_OF_NOUN "_appo"
#define STANFORD_RELATION_TYPE_APPOSITIVE_OF_NOUN "appos"
static string relationGovernorDefinitionNameArray[RELATION_GOVERNOR_DEFINITION_NUMBER_OF_TYPES] = {RELATION_ENTITY_BE};


//dependent on questions;
#ifdef GIA_RELEX
	#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX (SENTENCE_FIRST_ARTIFICIAL_INDEX+MAX_NUMBER_OF_SPECIAL_WORDS_PER_SENTENCE-10)	//eg numWordsInSentence+1 + 0 + 100 - 10
	#ifdef GIA_RELEX_1_4_0
		#define GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_PLUS_1
		#ifdef GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_PLUS_1
			#define GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_PLUS_1_REPLACEMENT_INDEX (REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX+1)	//eg numWordsInSentence+1 + 0 + 100 - 9
		#endif
	#endif
#endif
/*
#define FEATURE_MAX_NUMBER_OF_ARTIFICIAL_CONDITION_FEATURE_INDICES (15)	//added GIA 2g1a/25-August-2014
#define FEATURE_INDEX_OF_CONJUNCTION_1 (MAX_NUMBER_OF_WORDS_PER_SENTENCE-FEATURE_MAX_NUMBER_OF_ARTIFICIAL_CONDITION_FEATURE_INDICES) 	//added GIA 2f8a/10-July-2014
#define FEATURE_MAX_NUMBER_OF_CONJUNCTION_FEATURE_INDICES (100)	//some function of NLC_MAXIMUM_NUMBER_OF_CONJUNCTIONS_IN_SENTENCE
#define FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY (FEATURE_INDEX_OF_CONJUNCTION_1-FEATURE_MAX_NUMBER_OF_CONJUNCTION_FEATURE_INDICES)
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
#define REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_HOW "_%how"	//these needs to be a new integer (and not "prep_how") to prevent networkIndex entity node overwrite within redistributeStanfordRelationsCreateQueryVarsHowWhenWhereWhy()
//#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_QUANTITY_NUMBER_REPLACEMENT -9999
#define REFERENCE_TYPE_QUESTION_QUERY_WHICH "which"
#define REFERENCE_TYPE_QUESTION_QUERY_IS "is"
#define FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES 2
static string featureQueryWordAcceptedByAlternateMethodNameArray[FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_WHICH, REFERENCE_TYPE_QUESTION_QUERY_WHAT};
#ifdef GIA_REDISTRIBUTE_STANFORD_RELATIONS_ACTION_PREPOSITION_ACTION
	#define FEATURE_QUERY_ACTION_PREPOSITION_ACTION_EQUIVALENT_QUERY_VARIABLE_NUMBER_OF_TYPES 8	//or should this be arbitary?
	static string featureQueryActionPrepositionActionEquivalentQueryVariableNameArray[FEATURE_QUERY_ACTION_PREPOSITION_ACTION_EQUIVALENT_QUERY_VARIABLE_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_WHO, REFERENCE_TYPE_QUESTION_QUERY_WHAT, REFERENCE_TYPE_QUESTION_QUERY_HOW, REFERENCE_TYPE_QUESTION_QUERY_WHERE, REFERENCE_TYPE_QUESTION_QUERY_WHEN, REFERENCE_TYPE_QUESTION_QUERY_WHY, REFERENCE_TYPE_QUESTION_QUERY_WHICH, REFERENCE_TYPE_QUESTION_QUERY_IS};
#endif
#define FEATURE_QUERY_WORD_HOW_NUMBER_OF_TYPES 1
#define FEATURE_QUERY_WORD_MUCH_MANY_NUMBER_OF_TYPES 2
#define FEATURE_QUERY_HOW_MUCH_FIRST_RELATION_NUMBER_OF_TYPES 1
#define FEATURE_QUERY_HOW_MUCH_SECOND_RELATION_NUMBER_OF_TYPES 1
static string featureQueryWordHowNameArray[FEATURE_QUERY_WORD_HOW_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_HOW};
static string featureQueryWordMuchManyNameArray[FEATURE_QUERY_WORD_MUCH_MANY_NUMBER_OF_TYPES] = {"much", "many"};
static string featureQueryHowMuchFirstRelationNameArray[FEATURE_QUERY_HOW_MUCH_FIRST_RELATION_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_ADVMOD};
static string featureQueryHowMuchSecondRelationNameArray[FEATURE_QUERY_HOW_MUCH_SECOND_RELATION_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD};
#define FEATURE_QUERY_WORD_WHO_WHAT_NUMBER_OF_TYPES 2
static string featureQueryWordWhoWhatNameArray[FEATURE_QUERY_WORD_WHO_WHAT_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_WHO, REFERENCE_TYPE_QUESTION_QUERY_WHAT};
#define FEATURE_QUERY_HOW_WHEN_WHERE_WHY_RELATION_NUMBER_OF_TYPES 2
static string featureQueryHowWhenWhereWhyRelationNameArray[FEATURE_QUERY_HOW_WHEN_WHERE_WHY_RELATION_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_ADVMOD, RELATION_TYPE_ADJECTIVE_PREDADJ};
#define FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_NUMBER_OF_TYPES 4
static string featureQueryWordHowWhenWhereWhyNameArray[FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_HOW, REFERENCE_TYPE_QUESTION_QUERY_WHEN, REFERENCE_TYPE_QUESTION_QUERY_WHERE, REFERENCE_TYPE_QUESTION_QUERY_WHY};
static string featureQueryWordHowWhenWhereWhyCrossReferenceQueryVariableNameArray[FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_HOW, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY};
/*
#define FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_SPECIAL_CASE_NUMBER_OF_TYPES 1
static string featureQueryWordHowWhenWhereWhySpecialCaseNameArray[FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_SPECIAL_CASE_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_WHERE};
static string featureQueryWordHowWhenWhereWhySpecialCaseCrossReferenceQueryVariableNameArray[FEATURE_QUERY_WORD_HOW_WHEN_WHERE_WHY_SPECIAL_CASE_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE};
*/
#define FEATURE_QUERY_WORD_WHAT_NUMBER_OF_TYPES 1
static string featureQueryWordWhatNameArray[FEATURE_QUERY_WORD_WHAT_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_WHAT};
#define RELATION_TYPE_QVARIABLE_NUMBER_OF_TYPES 4
static string relationTypeQueryVariableNameArray[RELATION_TYPE_QVARIABLE_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY, REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_HOW};	//had to add REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_HOW here - need to check execution with relex parser is not affected
#define FEATURE_QUERY_WHAT_IS_THE_NAME_NUMBER_OF_NUMBER_OF_TYPES 1
static string featureQueryWhatIsTheNameNumberOfNameArray[FEATURE_QUERY_WHAT_IS_THE_NAME_NUMBER_OF_NUMBER_OF_TYPES] = {REFERENCE_TYPE_QUESTION_QUERY_WHAT};

//prepositions;
#define RELATION_TYPE_PREPOSITION_IN "in"
#define RELATION_TYPE_PREPOSITION_TO "to"
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
#define RELATION_TYPE_PREPOSITION_OF "of"		//eg [she grew tired] of it	 "She grew tired of the pie."  / "The house of Kriton is blue."	//detect if function and argument are both nouns/substance entities; if so then create a substance connection. if a function is a verb/action, then create a condition connection.
#define STANFORD_PARSER_PREPOSITION_PREPEND "prep_"
#define STANFORD_PARSER_PREPOSITIONC_PREPEND "prepc_"
#define RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES 19
static string relationTypePrepositionTimeNameArray[RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES] = {"in", "on", "after", "ago", "before", "between", "by", "during", "for", "to", "till", "until", "past", "since", "up_to", "within", "over", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN};
//?? NB all of these cases/types need to be replaced with more complex grammar requirements (eg "on" can also mean "rides his bike on the road" [location], not just "rides his bike on tuesday" [time])
	//http://www.englisch-hilfen.de/en/grammar/preposition_time.htm + is [time is] etc
#define RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES 35
static string relationTypePrepositionLocationNameArray[RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES] = {"in", "on", "at", "by", "near", "nearby", "above", "below", "over", "under", "around", "through", "inside", "inside_of", "outside", "between", "beside", "beyond", "in_front_of", "in_front", "in_back_of", "behind", "next_to", "on_top_of", "within", "beneath", "underneath", "among", "along", "against", "before", "after", "behind", "to", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE};
	//http://www.eslgold.com/grammar/prepositions_location.html + before, after, behind, to, etc
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
//static string relationTypePrepositionTimeNameArray[RELATION_TYPE_PREPOSITION_TIME_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING, "after", "ago", "before", "between", "during", "for", "till", "until", "past", "since", "up_to", "within", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHEN};
//static string relationTypePrepositionLocationNameArray[RELATION_TYPE_PREPOSITION_LOCATION_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING, "near", "nearby", "above", "below", "over", "under", "around", "through", "inside", "inside_of", "outside", "between", "beside", "beyond", "in_front_of", "in_front", "in_back_of", "behind", "next_to", "on_top_of", "within", "beneath", "underneath", "among", "along", "against", "before", "after", "behind", REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHERE};
#define RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES 3
static string relationTypePrepositionReductionNameArray[RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES][RELATION_TYPE_PREPOSITION_REDUCTION_MAX_NUMBER_VARIATIONS] = {{"at", "in", "to", "on"}, {"from", "of", "by", "", }, {"for", "since", "", ""}};
static int relationTypePrepositionReductionNumberVariationsArray[RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_NUMBER_OF_TYPES, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_NUMBER_OF_TYPES, RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_NUMBER_OF_TYPES};
static string relationTypePrepositionReductionReplacementNamesArray[RELATION_TYPE_PREPOSITION_REDUCTION_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_REDUCTION_POSITION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_RELATION_REPLACEMENT_STRING, RELATION_TYPE_PREPOSITION_REDUCTION_UNDEFINED_REPLACEMENT_STRING};
#endif

#ifdef GIA_DYNAMICALLY_LINK_FROM_CONDITIONS
#define RELATION_TYPE_PREPOSITION_FROM "from"
#define RELATION_TYPE_PREPOSITION_FROM_NUMBER_OF_TYPES (1)
static string relationTypePrepositionFromNameArray[RELATION_TYPE_PREPOSITION_FROM_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_FROM};
#define RELATION_TYPE_PREPOSITION_TO "to"
#define RELATION_TYPE_PREPOSITION_TO_NUMBER_OF_TYPES (1)
static string relationTypePrepositionToNameArray[RELATION_TYPE_PREPOSITION_TO_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_TO};
#endif

#define REFERENCE_TYPE_STANFORD_PARSER_PREPOSITION_PREPEND_NUMBER_OF_TYPES 2
static string referenceTypeStanfordParserPrepositionPrependNameArray[REFERENCE_TYPE_STANFORD_PARSER_PREPOSITION_PREPEND_NUMBER_OF_TYPES] = {STANFORD_PARSER_PREPOSITION_PREPEND, STANFORD_PARSER_PREPOSITIONC_PREPEND};
#define RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES 6
static string relationTypePropositionReasonOrCircumstanceNameArray[RELATION_TYPE_PREPOSITION_REASON_OR_CIRCUMSTANCE_NUMBER_OF_TYPES] = {"because", "on_account_of", "for", "out_of", "when",  REFERENCE_TYPE_QUESTION_QUERY_VARIABLE_WHY};
	//http://vlc.polyu.edu.hk/vlc/GrammarCourse/Lesson2_Preposition/CausePurpose.htm
#define REFERENCE_TYPE_QUESTION_WHEN_CONTEXT_NUMBER_OF_TYPES 15	//Eg what is the time?
static string relationContextPropositionTimeNameArray[REFERENCE_TYPE_QUESTION_WHEN_CONTEXT_NUMBER_OF_TYPES] = {"time", "period", "era", "millenia", "decade", "day", "month", "year", "hour", "minute", "second", "millisecond", "microsecond", "nanosecond", "picosecond"};
#define REFERENCE_TYPE_QUESTION_WHERE_CONTEXT_NUMBER_OF_TYPES 3	//Eg what is the location?
static string relationContextPropositionLocationNameArray[REFERENCE_TYPE_QUESTION_WHERE_CONTEXT_NUMBER_OF_TYPES] = {"location", "place", "position"};	//coordinates?
#define REFERENCE_TYPE_QUESTION_WHY_CONTEXT_NUMBER_OF_TYPES 3		//Eg what is the reason?
static string relationContextPropositionReasonNameArray[REFERENCE_TYPE_QUESTION_WHY_CONTEXT_NUMBER_OF_TYPES] = {"reason", "basis", "argument"};
#ifdef GIA_TRANSLATOR_INTERPRET_IN_AS_POSSESSIVE_FOR_SUBSTANCES
#define RELATION_TYPE_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES 2
static string relationTypePossessivePrepositionsNameArray[RELATION_TYPE_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_OF, RELATION_TYPE_PREPOSITION_IN};
#else
#define RELATION_TYPE_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES 1
static string relationTypePossessivePrepositionsNameArray[RELATION_TYPE_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_OF};
#endif
#ifdef GIA_REDISTRIBUTE_RELATIONS_INTERPRET_OF_AS_POSSESSIVE_FOR_SUBSTANCES
#define RELATION_TYPE_OF_OR_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES 1
static string relationTypeOfOrPossessivePrepositionsNameArray[RELATION_TYPE_OF_OR_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES] = {RELATION_TYPE_POSSESSIVE};
#else
#define RELATION_TYPE_OF_OR_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES 2
static string relationTypeOfOrPossessivePrepositionsNameArray[RELATION_TYPE_OF_OR_POSSESSIVE_PREPOSITIONS_NUMBER_OF_TYPES] = {RELATION_TYPE_PREPOSITION_OF, RELATION_TYPE_POSSESSIVE};
#endif


//negations;
#define RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES 1
#define RELATION_TYPE_NEGATIVE_CONTEXT_1 "not"
static string relationContextNegativeNameArray[RELATION_TYPE_NEGATIVE_CONTEXT_NUMBER_OF_TYPES] = {RELATION_TYPE_NEGATIVE_CONTEXT_1};


//conjugations;
#define GIA_TRANSLATOR_EXPLICITLY_ADD_CONJUNCTION_CONDITIONS
#define RELEX_RELATION_TYPE_CONJUGATION_AND "conj_and"
#define RELEX_RELATION_TYPE_CONJUGATION_OR "conj_or"
#define STANFORD_RELATION_TYPE_CONJUGATION_AND "conj_and"
#define STANFORD_RELATION_TYPE_CONJUGATION_OR "conj_or"
//#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
#define STANFORD_RELATION_TYPE_CONJUGATION_AND2 "conj:and"
#define STANFORD_RELATION_TYPE_CONJUGATION_OR2 "conj:or"
//#endif
#define RELATION_TYPE_CONJUGATION_AND "_conj_and"
#define RELATION_TYPE_CONJUGATION_OR "_conj_or"
#define STANFORD_RELATION_TYPE_CONJUNCT "conj"
#define STANFORD_RELATION_TYPE_COORDINATION "cc"
#define RELATION_TYPE_CONJUNCT "_conj"
#define RELATION_TYPE_COORDINATION "_cc"
#define RELATION_COORDINATION_DEPENDENT_AND "and"
#define RELATION_COORDINATION_DEPENDENT_OR "or"
#define RELATION_COORDINATION_DEPENDENT_BUT "but"	//added 2b3a
#define RELATION_COORDINATION_DEPENDENT_YET "yet"	//added 2b3a
#define RELATION_COORDINATION_DEPENDENT_NOR "nor"	//added 2b3a
#define RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES 5
static string relationTypeConjugationNameArray[RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES] = {RELATION_TYPE_CONJUGATION_AND, RELATION_TYPE_CONJUGATION_OR, RELATION_COORDINATION_DEPENDENT_BUT, RELATION_COORDINATION_DEPENDENT_YET, RELATION_COORDINATION_DEPENDENT_NOR};
#define GIA_TRANSLATOR_USE_BASIC_CONJUNCTION_CONDITION_TYPE_NAMES
#ifdef GIA_TRANSLATOR_USE_BASIC_CONJUNCTION_CONDITION_TYPE_NAMES
	#define RELATION_TYPE_CONJUGATION_AND_BASIC "and"
	#define RELATION_TYPE_CONJUGATION_OR_BASIC "or"
	#define RELATION_TYPE_CONJUGATION_BUT_BASIC "but"
	#define RELATION_TYPE_CONJUGATION_YET_BASIC "yet"
	#define RELATION_TYPE_CONJUGATION_NOR_BASIC "nor"
	#define RELATION_TYPE_CONJUGATION_BASIC_NUMBER_OF_TYPES (RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES)
	static string relationTypeConjugationBasicNameArray[RELATION_TYPE_CONJUGATION_BASIC_NUMBER_OF_TYPES] = {RELATION_TYPE_CONJUGATION_AND_BASIC, RELATION_TYPE_CONJUGATION_OR_BASIC, RELATION_TYPE_CONJUGATION_BUT_BASIC, RELATION_TYPE_CONJUGATION_YET_BASIC, RELATION_TYPE_CONJUGATION_NOR_BASIC};

#endif


//tobe/todo (substances/conditions);
#define PREPOSITION_BY "by"
#define PREPOSITION_THROUGH "through"
#define RELATION_TYPE_HAVING_AND_BEING_CONDITIONS_PREPOSITIONS_NUMBER_OF_TYPES 2
static string linkHavingPropertyConditionsAndBeingDefinitionConditionsPrepositionsNameArray[RELATION_TYPE_HAVING_AND_BEING_CONDITIONS_PREPOSITIONS_NUMBER_OF_TYPES] = {PREPOSITION_BY, PREPOSITION_THROUGH};
#define RELATION_TYPE_HAVING_AND_BEING_SPECIAL_ENTITY_NUMBER_OF_TYPES 2
static string linkHavingPropertyConditionsAndBeingDefinitionConditionsSpecialEntityNameArray[RELATION_TYPE_HAVING_AND_BEING_SPECIAL_ENTITY_NUMBER_OF_TYPES] = {RELATION_ENTITY_HAVE, RELATION_ENTITY_BE};
#define RELATION_TYPE_COMPLIMENT_TO_BE "_to-be"		//eg grows tired / The rose smelled sweet / _to-be(smell, sweet) - CHECK THIS
#define RELATION_TYPE_COMPLIMENT_TO_DO "_to-do"		//eg Linas likes to row / _to-do(like, row) - CHECK THIS
#define STANFORD_RELATION_TYPE_COMPLIMENT_TO_BE "acomp"	//NB added '_' for conversion purposes
#define STANFORD_RELATION_TYPE_COMPLIMENT_TO_DO "xcomp"	//NB added '_' for conversion purposes
#define RELATION_TYPE_COMPLIMENT_TO_BE_INDEX (0)
#define RELATION_TYPE_COMPLIMENT_TO_DO_INDEX (1)
#define RELATION_DEPENDENT_DO "do"
#define RELATION_TYPE_OBJECT_SPECIAL_TO_DO_SUBSTANCE_NUMBER_OF_TYPES 1
static string relationTypeObjectSpecialConditionToDoSubstanceNameArray[RELATION_TYPE_OBJECT_SPECIAL_TO_DO_SUBSTANCE_NUMBER_OF_TYPES] = {RELATION_TYPE_COMPLIMENT_TO_DO};
#define RELATION_TYPE_OBJECT_SPECIAL_TO_BE_SUBSTANCE_NUMBER_OF_TYPES 1
static string relationTypeObjectSpecialConditionToBeSubstanceNameArray[RELATION_TYPE_OBJECT_SPECIAL_TO_BE_SUBSTANCE_NUMBER_OF_TYPES] = {RELATION_TYPE_COMPLIMENT_TO_BE};
#define RELATION_TYPE_COMPLEMENTS_NUMBER_OF_TYPES 2
static string relationTypeComplementsNameArray[RELATION_TYPE_COMPLEMENTS_NUMBER_OF_TYPES] = {RELATION_TYPE_COMPLIMENT_TO_BE, RELATION_TYPE_COMPLIMENT_TO_DO};


//GIA Stanford specific multiword preposition reduction;
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEA (1)	//OLD: 3
static string redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeA[GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEA] = {RELATION_TYPE_COPULA};	//OLD: RELATION_TYPE_PASSIVE_AUX, RELATION_TYPE_MODAL_AUX
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEB (1)
static string redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeB[GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEB] = {RELATION_TYPE_PHRASAL_VERB_PARTICLE};	//OLD: RELATION_TYPE_ADJECTIVE_ADVMOD
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPED (2)
static string redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeD[GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPED] = {RELATION_TYPE_COMPLIMENT_TO_BE, RELATION_TYPE_PARTICIPIAL_MODIFIER};	//added 3 June 2012
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEE (1)
static string redistributionStanfordRelationsMultiwordPrepositionIntermediaryRelationsTypeE[GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_INTERMEDIARY_RELATIONS_TYPEE] = {RELATION_TYPE_COPULA};	//added 3 June 2012
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUBJOBJ_RELATIONS (2)
static string redistributionStanfordRelationsMultiwordPrepositionSubjObjRelations[GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUBJOBJ_RELATIONS] = {RELATION_TYPE_SUBJECT, RELATION_TYPE_OBJECT};
/*
#define GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NUMBER_OF_TYPES 2
static string redistributionRelationsSupportNameOfSubjectDependentOrGovernorNameArray[GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NUMBER_OF_TYPES] = {GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NAME, GIA_REDISTRIBUTE_RELATIONS_SUPPORT_NAME_OF_SUBJECT_DEPENDENT_OR_GOVERNOR_NUMBER};
*/
#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUPPORTED_PREPOSITIONS_TYPEA (2)
static string redistributionStanfordRelationsMultiwordPrepositionSupportedPrepositionsTypeA[GIA_REDISTRIBUTE_STANFORD_RELATIONS_MULTIWORD_PREPOSITION_NUMBER_OF_SUPPORTED_PREPOSITIONS_TYPEA] = {RELATION_TYPE_PREPOSITION_TO, RELATION_TYPE_PREPOSITION_OF};	//OLD: RELATION_TYPE_PASSIVE_AUX, RELATION_TYPE_MODAL_AUX

#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_NEG_AND_ADV_RELATIONS (2)
static string redistributionStanfordRelationsNegAndAdvRelations[GIA_REDISTRIBUTE_STANFORD_RELATIONS_NEG_AND_ADV_RELATIONS] = {RELATION_TYPE_NEGATIVE, RELATION_TYPE_ADJECTIVE_ADVMOD};


//measures and quantities;
	//measures;
#define RELATION_TYPE_MEASURE_DISTANCE "_measure_distance" 	//Relex only
#define RELATION_TYPE_MEASURE_SIZE "_measure_size" 		//Relex only
#define RELATION_TYPE_MEASURE_TIME "_measure_time"		//Relex only
#define RELATION_TYPE_MEASURE_UNKNOWN "_measure"
#define STANFORD_RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER "npadvmod"
//#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
#define STANFORD_RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER2 "nmod:npmod"
//#endif
#define RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER "_npadvmod"
#define RELATION_TYPE_MEASURE_PER "_measure_per"		//Relex only
#define RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN "_measure_dependency"	//Stanford Only
#define STANFORD_RELATION_TYPE_DEPENDENT "dep"	//high level relation - not
#define RELATION_TYPE_DEPENDENT "_dep"
	//quantities;
#define RELATION_TYPE_QUANTITY "_quantity"
#define STANFORD_RELATION_TYPE_QUANTITY "num"
//#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
#define STANFORD_RELATION_TYPE_QUANTITY2 "nummod"
//#endif
#define STANFORD_RELATION_TYPE_ELEMENT_OF_COMPOUND_NUMBER "number"
#define RELATION_TYPE_QUANTITY_MOD "_quantity_mod"
#define STANFORD_RELATION_TYPE_QUANTITY_MOD "quantmod"
#define RELATION_TYPE_QUANTITY_MULT "_quantity_mult"
	//? DOING NOW: references: yet to integrate - see http://wiki.opencog.org/w/Ideas#Improved_reference_resolution for integration (also check for the existence of the "person" tag in the feature "tense" data block)
	//? #define RELATION_TYPE_QUANTITY "_quantity"	//eg his bike	[bike him]		/its bike
#define RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_OR_STANFORD_UNKNOWN_NUMBER_OF_TYPES 2
static string relationTypeObjectSpecialConditionMeasureDistanceOrStanfordUnknownNameArray[RELATION_TYPE_OBJECT_SPECIAL_CONDITION_MEASURE_DISTANCE_OR_STANFORD_UNKNOWN_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_UNKNOWN};
#define RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES 5
static string relationTypeQuantityOrMeasureNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY, RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME, RELATION_TYPE_MEASURE_UNKNOWN};
#define RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES 3
static string relationTypeQuantityOrMeasureSwitchedNameArray[RELATION_TYPE_QUANTITY_OR_MEASURE_SWITCHED_NUMBER_OF_TYPES] = {RELATION_TYPE_QUANTITY_MOD, RELATION_TYPE_MEASURE_PER, RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN};
#define RELATION_TYPE_MEASURE_NUMBER_OF_TYPES 6
static string relationTypeMeasureNameArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_PER, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME, RELATION_TYPE_MEASURE_UNKNOWN, RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN};
static int relationTypeMeasureNameTypeIndexArray[RELATION_TYPE_MEASURE_NUMBER_OF_TYPES] = {MEASURE_TYPE_DISTANCE, MEASURE_TYPE_PER, MEASURE_TYPE_SIZE, MEASURE_TYPE_TIME, MEASURE_TYPE_UNKNOWN, MEASURE_DEPENDENCY_UNKNOWN};
#define RELATION_TYPE_MEASURE_DEPENDENCY_NUMBER_OF_TYPES 2
static string relationTypeMeasureDependencyNameArray[RELATION_TYPE_MEASURE_DEPENDENCY_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_PER, RELATION_TYPE_MEASURE_DEPENDENCY_UNKNOWN};
//#ifdef GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK
#define RELATION_TYPE_MEASURE_NOT_DEPENDENCY_NUMBER_OF_TYPES 4
static string relationTypeMeasureNotDependencyNameArray[RELATION_TYPE_MEASURE_NOT_DEPENDENCY_NUMBER_OF_TYPES] = {RELATION_TYPE_MEASURE_DISTANCE, RELATION_TYPE_MEASURE_SIZE, RELATION_TYPE_MEASURE_TIME, RELATION_TYPE_MEASURE_UNKNOWN};
//#endif
#define RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES 1
static string relationTypeQuantityArgumentImplyMeasurePerNameArray[RELATION_TYPE_QUANTITY_ARGUMENT_IMPLY_MEASURE_PER_NUMBER_OF_TYPES] = {"every"};


//dates;
#define RELATION_TYPE_DATE_DAY "_date_day" 	//Relex only
#define RELATION_TYPE_DATE_YEAR "_date_year"	//Relex only
//static int timeMonthIntArray[TIME_MONTH_NUMBER_OF_TYPES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
static string timeMonthStringArray[TIME_MONTH_NUMBER_OF_TYPES] = {TIME_MONTH_JANUARY, TIME_MONTH_FEBRUARY, TIME_MONTH_MARCH, TIME_MONTH_APRIL, TIME_MONTH_MAY, TIME_MONTH_JUNE, TIME_MONTH_JULY, TIME_MONTH_AUGUST, TIME_MONTH_SEPTEMBER, TIME_MONTH_OCTOBER, TIME_MONTH_NOVEMBER, TIME_MONTH_DECEMBER};


//pronoun references;
#define REFERENCE_TYPE_UNDEFINED 0
	//pronouns
#define REFERENCE_TYPE_POSSESSIVE_UNDEFINED 0
#define REFERENCE_TYPE_POSSESSIVE_MASCULINE 1		//his
#define REFERENCE_TYPE_POSSESSIVE_FEMININE 2		//her
#define REFERENCE_TYPE_POSSESSIVE_PLURAL 3		//them/their
#define REFERENCE_TYPE_POSSESSIVE_NEUTER 4		//its
#define REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES 5
static string referenceTypePossessiveNameArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {"undefined", "his", "her", "them", "its"};
//static int referenceTypePossessiveNameLengthsArray[REFERENCE_TYPE_POSSESSIVE_NUMBER_OF_TYPES] = {9, 3, 3, 4, 3};
	//definite pronouns
#define REFERENCE_TYPE_PERSON_UNDEFINED 0
#define REFERENCE_TYPE_PERSON_MASCULINE 1	//he
#define REFERENCE_TYPE_PERSON_FEMININE 2	//she
#define REFERENCE_TYPE_PERSON_PLURAL 3		//they
#define REFERENCE_TYPE_PERSON_NEUTER 4		//it
#define REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES 5
static string referenceTypePersonNameArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {"undefined", "he", "she", "they", "it"};
//static int referenceTypePersonNameLengthsArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {9, 2, 3, 4, 2};
static int referenceTypePersonCrossReferenceNumberArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_NUMBER_UNDEFINED, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_SINGULAR, GRAMMATICAL_NUMBER_PLURAL, GRAMMATICAL_NUMBER_SINGULAR};
static int referenceTypePersonCrossReferenceGenderArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_MASCULINE, GRAMMATICAL_GENDER_FEMININE, GRAMMATICAL_GENDER_UNDEFINED, GRAMMATICAL_GENDER_UNDEFINED};
static bool referenceTypePersonCrossReferenceDefiniteArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {false, true, true, true, true};
static bool referenceTypePersonCrossReferencePersonArray[REFERENCE_TYPE_PERSON_NUMBER_OF_TYPES] = {GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON, GRAMMATICAL_PERSON_UNDEFINED, GRAMMATICAL_PERSON_UNDEFINED};
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


//special cases;
#define RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES 1
static string relationTypeRequireSwitchingNameArray[RELATION_TYPE_REQUIRE_SWITCHING_NUMBER_OF_TYPES] = {RELATION_TYPE_OBJECT_THAT_RELEX};
#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_1F_RELATIONS_TREAT_THAT_AS_A_PRONOUN_IE_SUBSTANCE
static string featureTypeTreatAsPronounIeSubstance[RELATION_TYPE_TREAT_AS_PRONOUN_IE_SUBSTANCE_NUMBER_OF_TYPES] = {RELATION_DEPENDENT_THAT};
#endif
#ifdef GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY
#define GIA_NUMBER_OF_RELEX_VERSUS_STANFORD_DEPENDENCY_RELATION_DISCREPANCIES (21)
static string relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_NUMBER_OF_TYPES][GIA_NUMBER_OF_RELEX_VERSUS_STANFORD_DEPENDENCY_RELATION_DISCREPANCIES] = {{RELATION_TYPE_COMPLIMENT_TO_BE, RELATION_TYPE_COMPLIMENT_TO_DO, RELATION_TYPE_APPOSITIVE_OF_NOUN, RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_SUBJECT, RELATION_TYPE_POSSESSIVE, RELATION_TYPE_QUANTITY, RELATION_TYPE_QUANTITY, RELATION_TYPE_QUANTITY_MOD, STANFORD_PARSER_PREPOSITION_BY, RELATION_TYPE_QUANTITY, RELATION_TYPE_POSSESSIVE, RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER, RELATION_TYPE_OBJECT, RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER, RELATION_TYPE_PHRASAL_VERB_PARTICLE, RELATION_TYPE_TEMPORAL_MODIFIER, RELATION_TYPE_CONJUGATION_AND, RELATION_TYPE_CONJUGATION_OR}, {STANFORD_RELATION_TYPE_COMPLIMENT_TO_BE, STANFORD_RELATION_TYPE_COMPLIMENT_TO_DO, STANFORD_RELATION_TYPE_APPOSITIVE_OF_NOUN, STANFORD_RELATION_TYPE_OBJECT, STANFORD_RELATION_TYPE_INFINITIVAL_MODIFIER, STANFORD_RELATION_TYPE_PASSIVE_NOMINAL_SUBJECT, STANFORD_RELATION_TYPE_SUBJECT, STANFORD_RELATION_TYPE_POSS2, STANFORD_RELATION_TYPE_QUANTITY, STANFORD_RELATION_TYPE_ELEMENT_OF_COMPOUND_NUMBER, STANFORD_RELATION_TYPE_QUANTITY_MOD, STANFORD_RELATION_TYPE_AGENT, STANFORD_RELATION_TYPE_QUANTITY2, STANFORD_RELATION_TYPE_POSSESSIVE2, STANFORD_RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER2, STANFORD_RELATION_TYPE_CLAUSAL_COMPLEMENT, STANFORD_RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER2, STANFORD_RELATION_TYPE_PHRASAL_VERB_PARTICLE2, STANFORD_RELATION_TYPE_TEMPORAL_MODIFIER2, STANFORD_RELATION_TYPE_CONJUGATION_AND2, STANFORD_RELATION_TYPE_CONJUGATION_OR2}};
#else
#define GIA_NUMBER_OF_RELEX_VERSUS_STANFORD_DEPENDENCY_RELATION_DISCREPANCIES (20)
static string relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_NUMBER_OF_TYPES][GIA_NUMBER_OF_RELEX_VERSUS_STANFORD_DEPENDENCY_RELATION_DISCREPANCIES] = {{RELATION_TYPE_COMPLIMENT_TO_BE, RELATION_TYPE_COMPLIMENT_TO_DO, RELATION_TYPE_APPOSITIVE_OF_NOUN, RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_SUBJECT, RELATION_TYPE_POSSESSIVE, RELATION_TYPE_QUANTITY, RELATION_TYPE_QUANTITY, RELATION_TYPE_QUANTITY_MOD, STANFORD_PARSER_PREPOSITION_BY, RELATION_TYPE_QUANTITY, RELATION_TYPE_POSSESSIVE, RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER, RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER, RELATION_TYPE_PHRASAL_VERB_PARTICLE, RELATION_TYPE_TEMPORAL_MODIFIER, RELATION_TYPE_CONJUGATION_AND, RELATION_TYPE_CONJUGATION_OR}, {STANFORD_RELATION_TYPE_COMPLIMENT_TO_BE, STANFORD_RELATION_TYPE_COMPLIMENT_TO_DO, STANFORD_RELATION_TYPE_APPOSITIVE_OF_NOUN, STANFORD_RELATION_TYPE_OBJECT, STANFORD_RELATION_TYPE_INFINITIVAL_MODIFIER, STANFORD_RELATION_TYPE_PASSIVE_NOMINAL_SUBJECT, STANFORD_RELATION_TYPE_SUBJECT, STANFORD_RELATION_TYPE_POSS2, STANFORD_RELATION_TYPE_QUANTITY, STANFORD_RELATION_TYPE_ELEMENT_OF_COMPOUND_NUMBER, STANFORD_RELATION_TYPE_QUANTITY_MOD, STANFORD_RELATION_TYPE_AGENT, STANFORD_RELATION_TYPE_QUANTITY2, STANFORD_RELATION_TYPE_POSSESSIVE2, STANFORD_RELATION_TYPE_NOUNPHRASEASADVERBIALMODIFIER2, STANFORD_RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER2, STANFORD_RELATION_TYPE_PHRASAL_VERB_PARTICLE2, STANFORD_RELATION_TYPE_TEMPORAL_MODIFIER2, STANFORD_RELATION_TYPE_CONJUGATION_AND2, STANFORD_RELATION_TYPE_CONJUGATION_OR2}};
#endif

	/*
	//consider STANFORD_RELATION_TYPE_PASSIVE_NOMINAL_SUBJECT as SUBJECT not OBJECT??
	*/

/*
#define RELATION_TYPE_IMPLIES_SAME_SET_NUMBER_OF_TYPES 3
static string relationTypeImpliesSameSetNameArray[RELATION_TYPE_IMPLIES_SAME_SET_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD, RELATION_TYPE_ADJECTIVE_ADVMOD, RELATION_TYPE_RELATIVE_CLAUSE_MODIFIER};
*/
/*
#define RELATION_TYPE_ADJECTIVE_IMPLIES_SAME_SET_NUMBER_OF_TYPES 2
static string relationTypeAdjectiveImpliesSameSetNameArray[RELATION_TYPE_ADJECTIVE_IMPLIES_SAME_SET_NUMBER_OF_TYPES] = {RELATION_TYPE_ADJECTIVE_AMOD, RELATION_TYPE_ADJECTIVE_ADVMOD};
*/

//#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES (true)	//there is no default case; it depends upon what kind of substance
#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS (false)	//an action is considered by default not to be part of the same reference set as its subject/object (eg "the man fires the bow"). An rcmod /"that" is explicitly required for an action to be considered part of the same reference set as its subject/object (eg "the man that fires the bow...")
#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CONDITIONS (true)	//a condition is considered by default to be part of the same reference set as its subject/object (eg "the cat near the park..." == "the cat that is near the park..."). A copular "is" is explicitly required for a condition to be considered not part of the same reference set as its subject/object (eg "the cat is near the park")
#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_BEING_DEFINITION_CONDITIONS (true)
#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_HAVING_CONDITIONS (true)
#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PROPERTIES (true)				//this really depends on whether it is possessive or not
#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_PARATAXIS (false)
#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_CCCOMP (true)
#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_INDIRECT_OBJECTS (false)			//changed 2i8e
#define DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_DEFINITIONS (false)			//added 2i8g
#define DEFAULT_SAME_REFERENCE_SET_VALUE (true)
#define IRRELEVANT_SAME_REFERENCE_SET_VALUE_NO_ADVANCED_REFERENCING (false)
#define OLD_RELEX_PROBLEM_WORKAROUND_CODE_NOT_YET_SPENT_TIME_TO_DETERMINE_WHETHER_IMPLIES_SAME_SET (DEFAULT_SAME_REFERENCE_SET_VALUE)

/*************************************************************************************/








/********************************* Features* ******************************************/

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

//stanfordPOS;
	//from J93-2004-pennTreebankPOStagsets.pdf
	//conjunction
#define FEATURE_POS_TAG_COORDINATING_CONJUNCTION_CC "CC"	//"and" is considered a noun by Relex, eg There are 3 chickens and 4 bats.
#define FEATURE_POS_TAG_CONJUNCTION (FEATURE_POS_TAG_COORDINATING_CONJUNCTION_CC)
	//number
#define FEATURE_POS_TAG_CARDINALNUMBER_CD "CD"			//"3" is considered an adjective by Relex, eg There are 3 chickens.	//required for when "one" is misinterpreted as the number 'one', or for times eg "6:45"
	//determiner
#define FEATURE_POS_TAG_DETERMINER "DET"			//"the" is considered a determiner by Relex
	//additional (unknown)
#define FEATURE_POS_TAG_UNKNOWN_FOREIGNWORD_FW "FW"		//ASSUME foreign word is considered a WORD by Relex
#define FEATURE_POS_TAG_UNKNOWN_EXISTENTIAL_THERE "EX"		//"There" is considered a WORD by Relex, eg There is a ball.
#define FEATURE_POS_TAG_UNKNOWN_LISTITEMMARKER_LS "LS"		//ASSUME list marker is considered a WORD by Relex,
#define FEATURE_POS_TAG_UNKNOWN_SYMBOL_SYM "SYM"		//ASSUME symbol is considered a WORD by Relex
#define FEATURE_POS_TAG_PUNCTUATION_HASH "#"			//"#" is considered a WORD by Relex
#define FEATURE_POS_TAG_PUNCTUATION_DOLLAR "$"			//"$" is considered a WORD by Relex, eg Tom moves the $5 note.
#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
#define FEATURE_POS_TAG_PUNCTUATION_QUESTIONMARK "?"		//"?" is considered a WORD by Relex? [CHECKTHIS] //no PENN treebank entry for question mark?
#endif
	//predeterminer
#define FEATURE_POS_TAG_UNKNOWN_PREDETERMINER_PDT "PDT"		//"all" is considered an adjective by Relex, eg He fights all the time.	//http://en.wiktionary.org/wiki/predeterminer		//Parsed incorrectly by Stanford Parser: "three times" is considered an adjective/noun combination by Relex, eg It moves at three times the rate.
	//posessive ending
#define FEATURE_POS_TAG_UNKNOWN_POSSESSIVEENDING_POS "POS"	//"'s" is considered an adjective by Relex, eg Tom's car is blue.
	//modal auxiliary
#define FEATURE_POS_TAG_MODALAUXILIARY_MD "MD"			//can, could, may, might, must, shall, should, will, would - http://en.wikipedia.org/wiki/Modal_verb
	//preposition
#define FEATURE_POS_TAG_PREPOSITION_IN "IN"
#define FEATURE_POS_TAG_PREPOSITION_TO "TO"		//"to" is considered a preposition by Relex, eg He moves the bike to the farm..	//to is a particle is it not?
	//adjective
#define FEATURE_POS_TAG_ADJECTIVE_JJ "JJ"
#define FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE_JJR "JJR"
#define FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE_JJS "JJS"
	//noun
#define FEATURE_POS_TAG_NOUN_NN "NN"
#define FEATURE_POS_TAG_NOUN_NNS "NNS"
#define FEATURE_POS_TAG_NOUN_NNP "NNP"
#define FEATURE_POS_TAG_NOUN_NNPS "NNPS"
	//adverb
#define FEATURE_POS_TAG_ADVERB "RB"
#define FEATURE_POS_TAG_ADVERB_COMPARATIVE "RBR"
#define FEATURE_POS_TAG_ADVERB_SUPERLATIVE "RBS"
	//pronoun personal
#define FEATURE_POS_TAG_PRONOUN_PERSONAL_PRONOUN_PRP "PRP"		//"He" is considered a WORD by Relex (with pronoun flag), eg He will be fine.
	//pronoun possessive
#define FEATURE_POS_TAG_PRONOUN_POSSESSIVE_PRONOUN_PPDOLLAR "PP$"	//"His" is considered a adjective by Relex (with pronoun flag), eg His car is fast.
	//particle
#define FEATURE_POS_TAG_PARTICLE_RP "RP"			//what is this? http://en.wikipedia.org/wiki/Grammatical_particle
	//interjection
#define FEATURE_POS_TAG_INTERJECTION_UH "UH"				//interjections currently crash in GIA with Relex
	//verb
#define FEATURE_POS_TAG_VERB_VB "VB"
#define FEATURE_POS_TAG_VERB_VBD "VBD"
#define FEATURE_POS_TAG_VERB_VBG "VBG"
#define FEATURE_POS_TAG_VERB_VBN "VBN"
#define FEATURE_POS_TAG_VERB_VBP "VBP"
#define FEATURE_POS_TAG_VERB_VBZ "VBZ"
#ifdef GIA_NONSTANDARD_INTERMEDIARY_VERB_TYPES
	//#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
	#define FEATURE_POS_TAG_VERB_VBPOTENTIAL "VBPOTENTIAL"
	//#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
	#define FEATURE_POS_TAG_VERB_VBPOTENTIALINVERSE "VBPOTENTIALINVERSE"
	//#endif
	//#endif
	//#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
	#define FEATURE_POS_TAG_VERB_VBSTATE "VBSTATE"
	//#endif
	//#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
	#define FEATURE_POS_TAG_VERB_VBDESCRIPTION "VBDESCRIPTION"
	//#endif
#endif

	//wh
#define FEATURE_POS_TAG_WH_DETERMINER_WDT "WDT"				//"What"/"Which" is considered an adjective by Relex, eg What time is it?/Which marble is it?
#define FEATURE_POS_TAG_WH_PRONOUN_WP "WP"				//"What" is considered a WORD by Relex, eg What happened?
#define FEATURE_POS_TAG_WH_POSSESIVEPRONOUN_WPDOLLAR "WP$"
#define FEATURE_POS_TAG_WH_ADVERB_WRB "WRB"				//"Where" is considered an adverb by Relex, eg Where did it happen?
	//punctuation division
#define FEATURE_POS_TAG_PUNCTUATION_DIVISION_COMMA ","			//"," is considered a conjunction by Relex, eg The man moves, then he runs.
#define FEATURE_POS_TAG_PUNCTUATION_DIVISION_COLON_OR_SEMICOLON ":"	//":"/";" is considered a conjunction by Relex, eg The man moves: then he runs.
	//punctuation quote
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_FULLSTOP "."			//"." is considered a punctuation by Relex
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_LEFTBRACKET "("		//")" is considered a punctuation by Relex
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_RIGHTBRACKET ")"		//"(" is considered a punctuation by Relex
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_STRAIGHTDOUBLEQUOTE "\""	//" is ignored by Relex
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_LEFTOPENSINGLEQUOTE "'"	//"'" is considered a WORD by Relex
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_LEFTOPENDOUBLEQUOTE "\""	//" is ignored by Relex
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_RIGHTCLOSESINGLEQUOTE "'"	//"'" is considered a WORD by Relex
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_RIGHTCLOSEDOUBLEQUOTE "\""	//" is ignored by Relex

#define FEATURE_NER_NAME_CONCATENATION_TOKEN "_" 	//use "_" for Relex format and/or GIA_DATABASE compatibility, else may use " "

#define FEATURE_POS_TAG_VERB_PAST_NUMBER_OF_TYPES 1
static string posTagVerbPastArray[FEATURE_POS_TAG_VERB_PAST_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VBD};
#define FEATURE_POS_TAG_VERB_PRESENT_NUMBER_OF_TYPES 2
static string posTagVerbPresentArray[FEATURE_POS_TAG_VERB_PRESENT_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VBP, FEATURE_POS_TAG_VERB_VBZ};
#define FEATURE_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES 1
static string posTagVerbProgressiveArray[FEATURE_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VBG};
#define FEATURE_POS_TAG_VERB_INFINITIVE_NUMBER_OF_TYPES 1
static string posTagVerbInfinitiveOrImperativeArray[FEATURE_POS_TAG_VERB_INFINITIVE_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VB};
#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
#define FEATURE_POS_TAG_VERB_POTENTIAL_NUMBER_OF_TYPES 1	//not detected by POS standard
static string posTagVerbPotentialArray[FEATURE_POS_TAG_VERB_POTENTIAL_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VBPOTENTIAL};	//not detected by POS standard
#endif
#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
#define FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE_NUMBER_OF_TYPES 1	//not detected by POS standard
static string posTagVerbPotentialInverseArray[FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VBPOTENTIALINVERSE};	//not detected by POS standard
#endif
#ifdef GIA_FEATURE_POS_TAG_VERB_STATE
#define FEATURE_POS_TAG_VERB_STATE_NUMBER_OF_TYPES 1	//not detected by POS standard
static string posTagVerbStateArray[FEATURE_POS_TAG_VERB_STATE_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VBSTATE};	//not detected by POS standard
//#define FEATURE_POS_TAG_VERB_STANFORD_CORENLP_POSSIBLE_STATE_NUMBER_OF_TYPES 1
//static string posTagVerbStanfordCoreNLPpossibleStateArray[FEATURE_POS_TAG_VERB_STANFORD_CORENLP_POSSIBLE_STATE_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VBN};
#endif
#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
#define FEATURE_POS_TAG_VERB_DESCRIPTION_NUMBER_OF_TYPES 1	//not detected by POS standard
static string posTagVerbDescriptionArray[FEATURE_POS_TAG_VERB_DESCRIPTION_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VBDESCRIPTION};	//not detected by POS standard
#endif

#define FEATURE_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES 2
#define FEATURE_POS_TAG_COMMON_NOUN_NUMBER_OF_TYPES 2
#define FEATURE_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES 2
#define FEATURE_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES 2
static string posTagProperNounArray[FEATURE_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NOUN_NNP, FEATURE_POS_TAG_NOUN_NNPS};
static string posTagCommonNounArray[FEATURE_POS_TAG_COMMON_NOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NOUN_NN, FEATURE_POS_TAG_NOUN_NNS};
static string posTagSingularNounArray[FEATURE_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NOUN_NN, FEATURE_POS_TAG_NOUN_NNP};
static string posTagPluralNounArray[FEATURE_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NOUN_NNS, FEATURE_POS_TAG_NOUN_NNPS};

#define RELATION_TYPE_AUXILIARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES 1
static string relationAuxiliaryGovernerIndicatesFutureTenseArray[RELATION_TYPE_AUXILIARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES] = {"will"};

#ifdef GIA_GRAMMATICAL_WORD_TYPES_EXTENDED
static int featureRelexPOStypeCrossReferenceWordnetWordTypeArray[FEATURE_RELEX_POS_NUMBER_OF_TYPES] = {GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_ADJ, GRAMMATICAL_WORD_TYPE_ADV, GRAMMATICAL_WORD_TYPE_CONJUNCTION, GRAMMATICAL_WORD_TYPE_DETERMINER, GRAMMATICAL_WORD_TYPE_NOUN, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_PREP, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_VERB, GRAMMATICAL_WORD_TYPE_UNDEFINED};
#else
static int featureRelexPOStypeCrossReferenceWordnetWordTypeArray[FEATURE_RELEX_POS_NUMBER_OF_TYPES] = {GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_ADJ, GRAMMATICAL_WORD_TYPE_ADV, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_NOUN, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_PREP, GRAMMATICAL_WORD_TYPE_UNDEFINED, GRAMMATICAL_WORD_TYPE_VERB, GRAMMATICAL_WORD_TYPE_UNDEFINED};
#endif

//#ifdef GIA_NONSTANDARD_INTERMEDIARY_VERB_TYPES
#define FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL (24)
static string featurePOStagMinimalArray[FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL] = {FEATURE_POS_TAG_VERB_VB, FEATURE_POS_TAG_VERB_VBD, FEATURE_POS_TAG_VERB_VBG, FEATURE_POS_TAG_VERB_VBN, FEATURE_POS_TAG_VERB_VBP, FEATURE_POS_TAG_VERB_VBZ, FEATURE_POS_TAG_NOUN_NN, FEATURE_POS_TAG_NOUN_NNS, FEATURE_POS_TAG_NOUN_NNP, FEATURE_POS_TAG_NOUN_NNPS, FEATURE_POS_TAG_ADJECTIVE_JJ, FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE_JJR, FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE_JJS, FEATURE_POS_TAG_ADVERB, FEATURE_POS_TAG_ADVERB_COMPARATIVE, FEATURE_POS_TAG_ADVERB_SUPERLATIVE, FEATURE_POS_TAG_PRONOUN_PERSONAL_PRONOUN_PRP, FEATURE_POS_TAG_PRONOUN_POSSESSIVE_PRONOUN_PPDOLLAR, FEATURE_POS_TAG_VERB_VBPOTENTIAL, FEATURE_POS_TAG_VERB_VBPOTENTIALINVERSE, FEATURE_POS_TAG_CONJUNCTION, FEATURE_POS_TAG_DETERMINER, FEATURE_POS_TAG_VERB_VBSTATE, FEATURE_POS_TAG_VERB_VBDESCRIPTION};
static string featurePOStagCrossReferenceRelexPOStypeArray[FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL] = {FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_CONJUNCTION_NAME, FEATURE_RELEX_POS_TYPE_DETERMINER_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME};
#define FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL_GRAMMATICALLY_STRICT (20)
static string featurePOStagMinimalArrayGrammaticallyStrict[FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL_GRAMMATICALLY_STRICT] = {FEATURE_POS_TAG_VERB_VB, FEATURE_POS_TAG_VERB_VBD, FEATURE_POS_TAG_VERB_VBG, FEATURE_POS_TAG_VERB_VBN, FEATURE_POS_TAG_VERB_VBP, FEATURE_POS_TAG_VERB_VBZ, FEATURE_POS_TAG_NOUN_NN, FEATURE_POS_TAG_NOUN_NNS, FEATURE_POS_TAG_NOUN_NNP, FEATURE_POS_TAG_NOUN_NNPS, FEATURE_POS_TAG_ADJECTIVE_JJ, FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE_JJR, FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE_JJS, FEATURE_POS_TAG_ADVERB, FEATURE_POS_TAG_ADVERB_COMPARATIVE, FEATURE_POS_TAG_ADVERB_SUPERLATIVE, FEATURE_POS_TAG_PRONOUN_PERSONAL_PRONOUN_PRP, FEATURE_POS_TAG_PRONOUN_POSSESSIVE_PRONOUN_PPDOLLAR, FEATURE_POS_TAG_CONJUNCTION, FEATURE_POS_TAG_DETERMINER};
static string featurePOStagCrossReferenceRelexPOStypeArrayGrammaticallyStrict[FEATURE_POS_TAG_NUMBER_OF_TYPES_MINIMAL_GRAMMATICALLY_STRICT] = {FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_VERB_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADJECTIVE_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_ADVERB_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_NOUN_NAME, FEATURE_RELEX_POS_TYPE_CONJUNCTION_NAME, FEATURE_RELEX_POS_TYPE_DETERMINER_NAME};

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
static string featureNERtypeArray[FEATURE_NER_NUMBER_OF_TYPES] = {FEATURE_NER_UNDEFINED_NAME, FEATURE_NER_DATE_NAME, FEATURE_NER_TIME_NAME, FEATURE_NER_MONEY_NAME, FEATURE_NER_NUMBER_NAME, FEATURE_NER_PERSON_NAME, FEATURE_NER_LOCATION_NAME, FEATURE_NER_ORGANIZATION_NAME, FEATURE_NER_MISC_NAME, FEATURE_NER_DURATION_NAME};

#define FEATURE_RELEX_FLAG_NUMBER_OF_TYPES (FEATURE_NER_NUMBER_OF_TYPES)
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
static string featureRelexFlagTypeArray[FEATURE_RELEX_FLAG_NUMBER_OF_TYPES] = {FEATURE_RELEX_FLAG_UNDEFINED_NAME, FEATURE_RELEX_FLAG_DATE_NAME, FEATURE_RELEX_FLAG_TIME_NAME, FEATURE_RELEX_FLAG_MONEY_NAME, FEATURE_RELEX_FLAG_NUMBER_NAME, FEATURE_RELEX_FLAG_PERSON_NAME, FEATURE_RELEX_FLAG_LOCATION_NAME, FEATURE_RELEX_FLAG_ORGANIZATION_NAME, FEATURE_RELEX_FLAG_MISC_NAME, FEATURE_RELEX_FLAG_DURATION_NAME};

static int featureNERexplicitTypeArray[FEATURE_NER_EXPLICIT_NUMBER_OF_TYPES] = {FEATURE_NER_DATE, FEATURE_NER_TIME, FEATURE_NER_MONEY, FEATURE_NER_NUMBER, FEATURE_NER_PERSON, FEATURE_NER_LOCATION, FEATURE_NER_ORGANIZATION, FEATURE_NER_MISC, FEATURE_NER_DURATION};

#define FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_OF_TYPES 3
static int featureNERindicatesProperNounTypeArray[FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_OF_TYPES] = {FEATURE_NER_PERSON, FEATURE_NER_LOCATION, FEATURE_NER_ORGANIZATION};	//without bug this array should not be required to be used by stanford parser/CoreNLP as Standford Core NLP explicitly marks entities as 'proper noun' within their stanfordPOS tag (it is used by the Relex parser however to determine proper nouns)

#define FEATURE_NER_INDICATES_NAME_CONCATENATION_REQUIRED_NUMBER_OF_TYPES 4
static int featureNERindicatesNameConcatenationRequiredTypeArray[FEATURE_NER_INDICATES_NAME_CONCATENATION_REQUIRED_NUMBER_OF_TYPES] = {FEATURE_NER_PERSON, FEATURE_NER_LOCATION, FEATURE_NER_ORGANIZATION, FEATURE_NER_MISC};

#define FEATURE_NER_INDICATES_NORMALISED_NER_AVAILABLE_NUMBER_OF_TYPES 4
static int featureNERindicatesNormalisedNERavailableTypeArray[FEATURE_NER_INDICATES_NORMALISED_NER_AVAILABLE_NUMBER_OF_TYPES] = {FEATURE_NER_DATE, FEATURE_NER_TIME, FEATURE_NER_MONEY, FEATURE_NER_NUMBER};

#define FEATURE_POS_TAG_INDICATES_ADJECTIVE_OR_ADVERB_NUMBER_OF_TYPES 6
static string featurePOSindicatesAdjectiveOrAdverbTypeArray[FEATURE_POS_TAG_INDICATES_ADJECTIVE_OR_ADVERB_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_ADJECTIVE_JJ, FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE_JJR, FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE_JJS, FEATURE_POS_TAG_ADVERB, FEATURE_POS_TAG_ADVERB_COMPARATIVE, FEATURE_POS_TAG_ADVERB_SUPERLATIVE};

#define FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_OF_TYPES 2
static string featurePOSindicatesPronounTypeArray[FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PRONOUN_PERSONAL_PRONOUN_PRP, FEATURE_POS_TAG_PRONOUN_POSSESSIVE_PRONOUN_PPDOLLAR};

#define FEATURE_POS_TAG_INDICATES_NOUN_NUMBER_OF_TYPES 5
static string featurePOSindicatesNounTypeArray[FEATURE_POS_TAG_INDICATES_NOUN_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_CARDINALNUMBER_CD, FEATURE_POS_TAG_NOUN_NN, FEATURE_POS_TAG_NOUN_NNS, FEATURE_POS_TAG_NOUN_NNP, FEATURE_POS_TAG_NOUN_NNPS};

//moved from GIAtranslatorDefineGrammar.h in GIA 2b3a:
#define GRAMMATICAL_DETERMINER_DEFINITE "the"
#define GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR "a"
#define GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL "some"
#define GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR_FIRST_LETTER_VOWEL "an"	//required for nlg only (a an is lemma of an)

#define GRAMMATICAL_DETERMINER_DEFINITE_EACH "each"
#define GRAMMATICAL_DETERMINER_DEFINITE_EVERY "every"
#define GRAMMATICAL_DETERMINER_INDEFINITE_ALL "all"
#ifdef GIA_EXTRA_DETERMINERS
#define GRAMMATICAL_DETERMINER_GOVERNOR_DEFINITE_ARRAY_NUMBER_OF_TYPES (3)
static string relationDeterminerGovernorDefiniteArray[GRAMMATICAL_DETERMINER_GOVERNOR_DEFINITE_ARRAY_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_DEFINITE, GRAMMATICAL_DETERMINER_DEFINITE_EACH, GRAMMATICAL_DETERMINER_DEFINITE_EVERY};
#define GRAMMATICAL_DETERMINER_GOVERNOR_INDEFINITE_ARRAY_NUMBER_OF_TYPES (3)
static string relationDeterminerGovernorIndefiniteArray[GRAMMATICAL_DETERMINER_GOVERNOR_INDEFINITE_ARRAY_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL, GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR, GRAMMATICAL_DETERMINER_INDEFINITE_ALL};
#else
#define GRAMMATICAL_DETERMINER_GOVERNOR_DEFINITE_ARRAY_NUMBER_OF_TYPES (1)
static string relationDeterminerGovernorDefiniteArray[GRAMMATICAL_DETERMINER_GOVERNOR_DEFINITE_ARRAY_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_DEFINITE};
#define GRAMMATICAL_DETERMINER_GOVERNOR_INDEFINITE_ARRAY_NUMBER_OF_TYPES (2)
static string relationDeterminerGovernorIndefiniteArray[GRAMMATICAL_DETERMINER_GOVERNOR_INDEFINITE_ARRAY_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL, GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR};
#endif
static string entityPredeterminerSmallNameArray[GRAMMATICAL_PREDETERMINER_SMALL_ARRAY_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_DEFINITE_EACH, GRAMMATICAL_DETERMINER_DEFINITE_EVERY, GRAMMATICAL_DETERMINER_INDEFINITE_ALL};	//added 2i34a




#ifdef GIA_FEATURE_POS_TAG_NN_ONLY_MARK_AS_SINGULAR_WITH_DETERMINER
#define GRAMMATICAL_DETERMINER_POTENTIALLY_SINGULAR_ARRAY_NUMBER_OF_TYPES (4)
static string relationDeterminerPotentiallySingularArray[GRAMMATICAL_DETERMINER_POTENTIALLY_SINGULAR_ARRAY_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_DEFINITE, GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR, GRAMMATICAL_DETERMINER_DEFINITE_EACH, GRAMMATICAL_DETERMINER_DEFINITE_EVERY};
#endif

#define GRAMMATICAL_DETERMINER_ARRAY_NUMBER_OF_TYPES (7)
static string relationDeterminerArray[GRAMMATICAL_DETERMINER_ARRAY_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_DEFINITE, GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR, GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL, GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR_FIRST_LETTER_VOWEL, GRAMMATICAL_DETERMINER_DEFINITE_EACH, GRAMMATICAL_DETERMINER_DEFINITE_EVERY, GRAMMATICAL_DETERMINER_INDEFINITE_ALL};

/*************************************************************************************/







#define RELATION_TYPE_PREPOSITION_FOR "for"
#define ENTITY_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES (RELATION_TYPE_CONJUGATION_NUMBER_OF_TYPES)
static string entityCoordinatingConjunctionArray[ENTITY_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES] = {"and", "or", "but", "yet", "nor"};	//removed "for" +  "so" as conj is not generated for these by Stanford //http://grammar.ccc.commnet.edu/grammar/conjunctions.htm
#define ENTITY_COORDINATINGCONJUNCTION_ARRAY_INDEX_AND (0)
#define ENTITY_COORDINATINGCONJUNCTION_ARRAY_INDEX_OR (1)
	/*
	Tom rides the bike so he can go to lunch.
	Tom likes the chicken but prefers the apple.
	The ball is neither blue nor red.
	The ball is tall yet fast.
	*/



//http://en.wikipedia.org/wiki/English_auxiliaries_and_contractions
#define ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES (8)
#define ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES (4)
#define ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES (4)
static string entityAuxiliaryBeingArray[ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES] = {"am", "is", "are", "was", "were", "be", "being", "been"};
static string entityAuxiliaryHavingArray[ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES] = {"have", "has", "had", "having"};
static string entityAuxiliaryDoingArray[ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES] = {"do", "does", "did", "doing"};

//additional cases not identified by [/mapped to existing] Relex Word Type:
#define ENTITY_WH_ARRAY_NUMBER_OF_TYPES (9)
static string entityWhArray[ENTITY_WH_ARRAY_NUMBER_OF_TYPES] = {"which", "what", "who", "whom", "whose", "where", "when", "how", "why"};	//http://courses.washington.edu/hypertxt/csar-v02/penntable.html + http://www.computing.dcu.ie/~acahill/tagset.html

#define ENTITY_POSSESSIVEENDING_APOSTROPHE_S "'s"
#define ENTITY_POSSESSIVEENDING_APOSTROPHE "'"
#define ENTITY_POSSESSIVEENDING_NUMBER_OF_TYPES (2)
static string entityPossessiveEndingArray[ENTITY_POSSESSIVEENDING_NUMBER_OF_TYPES] = {ENTITY_POSSESSIVEENDING_APOSTROPHE_S, ENTITY_POSSESSIVEENDING_APOSTROPHE};
#define ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES (9)
static string entityModalAuxiliaryArray[ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES] = {"can", "could", "may", "might", "must", "shall", "should", "will", "would"};	//en.wikipedia.org/wiki/Modal_verb
#define ENTITY_CARDINALNUMBER_ARRAY_NUMBER_OF_TYPES (43)
static string entityCardinalNumberArray[ENTITY_CARDINALNUMBER_ARRAY_NUMBER_OF_TYPES] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9","zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine","ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety", "hundred", "thousand", "million", "billion", "trillion"};
#define ENTITY_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES (16)
static string entityPronounPersonalArray[ENTITY_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES] = {"I", "you", "he", "she", "it", "me", "you", "him", "her", "it", "we", "you", "they", "us", "you", "them"};			//http://grammar.ccc.commnet.edu/grammar/cases.htm#cases
#define ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES (14)
static string entityPronounPossessiveArray[ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES] = {"my", "mine", "your", "yours", "his", "her", "hers", "its", "our", "ours", "your", "yours", "their", "theirs"};	//http://grammar.ccc.commnet.edu/grammar/cases.htm#cases
#define ENTITY_PREDETERMINER_ARRAY_NUMBER_OF_TYPES (13)
static string entityPredeterminerArray[ENTITY_PREDETERMINER_ARRAY_NUMBER_OF_TYPES] = {"all", "both", "double", "half", "quadruple", "quite", "rather", "such", "times", "treble", "twice", "quarter", "many"};	//removed "what"	//http://englishwithyeasir.blogspot.com.au/2013/05/predeterminers.html
	//predeterminer could be merged with adjective

//not required for Stanford CoreNLP as "an" lemma is "a" (but is required for Relex)
#define GRAMMATICAL_DETERMINER_LIMITED_INDEFINITE_NUMBER_OF_TYPES (2)
static string grammaticalDeterminerIndefiniteArray[GRAMMATICAL_DETERMINER_LIMITED_INDEFINITE_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR, GRAMMATICAL_DETERMINER_INDEFINITE_SINGULAR_FIRST_LETTER_VOWEL};	//NB this intentionally discludes GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL "some" as this is handled the same as a definite determinier by GIA2 POS tag system




#ifdef GIA_SEMANTIC_PARSER

#define FEATURE_POS_TAG_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_NUMBER_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_DETERMINER_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_UNKNOWN_ARRAY_NUMBER_OF_TYPES 6
#define FEATURE_POS_TAG_PREDETERMINER_ARRAY_NUMBER_OF_TYPES 1		//predeterminer could be merged with adjective
#define FEATURE_POS_TAG_POSSESSIVEENDING_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_MODALAUXILIARY_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_PREPOSITION_ARRAY_NUMBER_OF_TYPES 2
#define FEATURE_POS_TAG_ADJECTIVE_ARRAY_NUMBER_OF_TYPES 3
#define FEATURE_POS_TAG_NOUN_ARRAY_NUMBER_OF_TYPES 4
#define FEATURE_POS_TAG_ADVERB_ARRAY_NUMBER_OF_TYPES 3
#define FEATURE_POS_TAG_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_PARTICLE_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_INTERJECTION_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_VERB_ARRAY_NUMBER_OF_TYPES 6
#define FEATURE_POS_TAG_WH_ARRAY_NUMBER_OF_TYPES 4
#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
#define FEATURE_POS_TAG_PUNCTUATION_DIVISION_ARRAY_NUMBER_OF_TYPES 3
#else
#define FEATURE_POS_TAG_PUNCTUATION_DIVISION_ARRAY_NUMBER_OF_TYPES 2
#endif
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_ARRAY_NUMBER_OF_TYPES 8
static string featurePOStagCoordinatingConjunctionArray[FEATURE_POS_TAG_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_COORDINATING_CONJUNCTION_CC};
static string featurePOStagNumberArray[FEATURE_POS_TAG_NUMBER_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_CARDINALNUMBER_CD};
static string featurePOStagDeterminerArray[FEATURE_POS_TAG_DETERMINER_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_DETERMINER};
static string featurePOStagUnknownArray[FEATURE_POS_TAG_UNKNOWN_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_UNKNOWN_FOREIGNWORD_FW, FEATURE_POS_TAG_UNKNOWN_EXISTENTIAL_THERE, FEATURE_POS_TAG_UNKNOWN_LISTITEMMARKER_LS, FEATURE_POS_TAG_UNKNOWN_SYMBOL_SYM, FEATURE_POS_TAG_PUNCTUATION_HASH, FEATURE_POS_TAG_PUNCTUATION_DOLLAR};
static string featurePOStagPredeterminerArray[FEATURE_POS_TAG_PREDETERMINER_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_UNKNOWN_PREDETERMINER_PDT};	//predeterminer could be merged with adjective
static string featurePOStagPossessiveEndingArray[FEATURE_POS_TAG_POSSESSIVEENDING_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_UNKNOWN_POSSESSIVEENDING_POS};
static string featurePOStagModalAuxiliaryArray[FEATURE_POS_TAG_MODALAUXILIARY_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_MODALAUXILIARY_MD};
static string featurePOStagPrepositionArray[FEATURE_POS_TAG_PREPOSITION_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PREPOSITION_IN, FEATURE_POS_TAG_PREPOSITION_TO};
static string featurePOStagAdjectiveArray[FEATURE_POS_TAG_ADJECTIVE_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_ADJECTIVE_JJ, FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE_JJR, FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE_JJS};
static string featurePOStagNounArray[FEATURE_POS_TAG_NOUN_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NOUN_NN, FEATURE_POS_TAG_NOUN_NNS, FEATURE_POS_TAG_NOUN_NNP, FEATURE_POS_TAG_NOUN_NNPS};
static string featurePOStagAdverbArray[FEATURE_POS_TAG_ADVERB_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_ADVERB, FEATURE_POS_TAG_ADVERB_COMPARATIVE, FEATURE_POS_TAG_ADVERB_SUPERLATIVE};
static string featurePOStagPronounPersonalArray[FEATURE_POS_TAG_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PRONOUN_PERSONAL_PRONOUN_PRP};
static string featurePOStagPronounPossessiveArray[FEATURE_POS_TAG_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PRONOUN_POSSESSIVE_PRONOUN_PPDOLLAR};
static string featurePOStagParticleArray[FEATURE_POS_TAG_PARTICLE_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PARTICLE_RP};
static string featurePOStagInterjectionArray[FEATURE_POS_TAG_INTERJECTION_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_INTERJECTION_UH};
static string featurePOStagVerbArray[FEATURE_POS_TAG_VERB_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VB, FEATURE_POS_TAG_VERB_VBD, FEATURE_POS_TAG_VERB_VBG, FEATURE_POS_TAG_VERB_VBN, FEATURE_POS_TAG_VERB_VBP, FEATURE_POS_TAG_VERB_VBZ};
static string featurePOStagWhArray[FEATURE_POS_TAG_WH_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_WH_DETERMINER_WDT, FEATURE_POS_TAG_WH_PRONOUN_WP, FEATURE_POS_TAG_WH_POSSESIVEPRONOUN_WPDOLLAR, FEATURE_POS_TAG_WH_ADVERB_WRB};
#ifdef GIA_SEMANTIC_PARSER_SUPPORT_QUERIES
static string featurePOStagPunctuationDivisionArray[FEATURE_POS_TAG_PUNCTUATION_DIVISION_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PUNCTUATION_DIVISION_COMMA, FEATURE_POS_TAG_PUNCTUATION_DIVISION_COLON_OR_SEMICOLON, FEATURE_POS_TAG_PUNCTUATION_QUESTIONMARK};	//NB FEATURE_POS_TAG_PUNCTUATION_QUESTIONMARK has been set to a different POS value than FEATURE_POS_TAG_PUNCTUATION_QUOTE_FULLSTOP; but otherwise its inclusion here is arbitrary
#else
static string featurePOStagPunctuationDivisionArray[FEATURE_POS_TAG_PUNCTUATION_DIVISION_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PUNCTUATION_DIVISION_COMMA, FEATURE_POS_TAG_PUNCTUATION_DIVISION_COLON_OR_SEMICOLON};
#endif
static string featurePOStagPunctuationQuoteArray[FEATURE_POS_TAG_PUNCTUATION_QUOTE_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PUNCTUATION_QUOTE_FULLSTOP, FEATURE_POS_TAG_PUNCTUATION_QUOTE_LEFTBRACKET, FEATURE_POS_TAG_PUNCTUATION_QUOTE_RIGHTBRACKET, FEATURE_POS_TAG_PUNCTUATION_QUOTE_STRAIGHTDOUBLEQUOTE, FEATURE_POS_TAG_PUNCTUATION_QUOTE_LEFTOPENSINGLEQUOTE, FEATURE_POS_TAG_PUNCTUATION_QUOTE_LEFTOPENDOUBLEQUOTE, FEATURE_POS_TAG_PUNCTUATION_QUOTE_RIGHTCLOSESINGLEQUOTE, FEATURE_POS_TAG_PUNCTUATION_QUOTE_RIGHTCLOSEDOUBLEQUOTE};

#define GIA_SEMANTIC_PARSER_POS_TYPE_UNDEFINED 0		//added in case Stanford POS extraction does not equate exactly to PENN tree bank specification
#define GIA_SEMANTIC_PARSER_POS_TYPE_COORDINATINGCONJUNCTION 1
#define GIA_SEMANTIC_PARSER_POS_TYPE_NUMBER 2
#define GIA_SEMANTIC_PARSER_POS_TYPE_DETERMINER 3
#define GIA_SEMANTIC_PARSER_POS_TYPE_UNKNOWN 4
#define GIA_SEMANTIC_PARSER_POS_TYPE_PREDETERMINER 5	//predeterminer could be merged with adjective
#define GIA_SEMANTIC_PARSER_POS_TYPE_POSSESSIVEENDING 6
#define GIA_SEMANTIC_PARSER_POS_TYPE_MODALAUXILIARY 7
#define GIA_SEMANTIC_PARSER_POS_TYPE_PREPOSITION 8
#define GIA_SEMANTIC_PARSER_POS_TYPE_ADJECTIVE 9
#define GIA_SEMANTIC_PARSER_POS_TYPE_NOUN 10
#define GIA_SEMANTIC_PARSER_POS_TYPE_ADVERB 11
#define GIA_SEMANTIC_PARSER_POS_TYPE_PRONOUN_PERSONAL 12
#define GIA_SEMANTIC_PARSER_POS_TYPE_PRONOUN_POSSESSIVE 13
#define GIA_SEMANTIC_PARSER_POS_TYPE_PARTICLE 14
#define GIA_SEMANTIC_PARSER_POS_TYPE_INTERJECTION 15
#define GIA_SEMANTIC_PARSER_POS_TYPE_VERB 16
#define GIA_SEMANTIC_PARSER_POS_TYPE_WH 17
#define GIA_SEMANTIC_PARSER_POS_TYPE_PUNCTUATION_DIVISION 18
#define GIA_SEMANTIC_PARSER_POS_TYPE_PUNCTUATION_QUOTE 19
#define GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_BEING 20		//additional case required for GIA semantics extraction
#define GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_HAVING 21		//additional case required for GIA semantics extraction
#define GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_DOING 22		//additional case required for GIA semantics extraction
#define GIA_SEMANTIC_PARSER_POS_TYPE_DETERMINER_LIMITED_INDEFINITE 23
#define GIA_SEMANTIC_PARSER_POS_TYPE_SPECIAL_REDUCED_CONJUNCTION 24
#define GIA_SEMANTIC_PARSER_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES (25)
static string GIAconnectionistNetworkPOStypeNameArray[GIA_SEMANTIC_PARSER_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES] = {"undefined", "coordinatingConjunction", "number", "determiner", "unknown", "predeterminer", "posessiveEnding", "modalAuxiliary", "preposition", "adjective", "noun", "adverb", "pronounPersonal", "pronounPossessive", "particle", "interjection", "verb", "wh", "punctuationDivision", "punctuationQuote", "isAuxillaryBeing", "isAuxillaryHaving", "isAuxillaryDoing", "determinerLimitedIndefinite", "specialReducedConjunction"};
static int featureRelexPOStypeCrossReferenceGIAconnectionistNetworkPOStypeArray[FEATURE_RELEX_POS_NUMBER_OF_TYPES] = {GIA_SEMANTIC_PARSER_POS_TYPE_UNDEFINED, GIA_SEMANTIC_PARSER_POS_TYPE_ADJECTIVE, GIA_SEMANTIC_PARSER_POS_TYPE_ADVERB, GIA_SEMANTIC_PARSER_POS_TYPE_PUNCTUATION_DIVISION, GIA_SEMANTIC_PARSER_POS_TYPE_DETERMINER, GIA_SEMANTIC_PARSER_POS_TYPE_NOUN, GIA_SEMANTIC_PARSER_POS_TYPE_PARTICLE, GIA_SEMANTIC_PARSER_POS_TYPE_PREPOSITION, GIA_SEMANTIC_PARSER_POS_TYPE_PUNCTUATION_QUOTE, GIA_SEMANTIC_PARSER_POS_TYPE_VERB, GIA_SEMANTIC_PARSER_POS_TYPE_UNKNOWN};

#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS
	#define FEATURE_SPECIAL_REDUCED_CONJUNCTIONS_NUMBER_OF_TYPES (2)
	static string featureSpecialReducedConjunctionArray[FEATURE_SPECIAL_REDUCED_CONJUNCTIONS_NUMBER_OF_TYPES] = {"and", "or"};
	#define GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS_ILLEGAL_CENTRAL_WORD_NUMBER_OF_TYPES  (3)
	static string semanticParserOptimiseBasedOnConjunctionsIllegalSecondWordInTupleArray[GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS_ILLEGAL_CENTRAL_WORD_NUMBER_OF_TYPES] = {",", "and", "or"};

#endif

/*
//requires updating (add more cases from PENN tree)
#define GIA_SEMANTIC_PARSER_POS_TYPE_NOUN (0)
#define GIA_SEMANTIC_PARSER_POS_TYPE_VERB_OR_PARTICIPLE (1)
#define GIA_SEMANTIC_PARSER_POS_TYPE_ADJECTIVE_OR_ADVERB (2)
#define GIA_SEMANTIC_PARSER_POS_TYPE_PREPOSITION (3)
#define GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_BEING (4)
#define GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_HAVING (5)
#define GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_DOING (6)
#define GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_MODAL (7)
#define GIA_SEMANTIC_PARSER_POS_TYPE_DETERMINER_THATWHICH (8)
#define GIA_SEMANTIC_PARSER_POS_TYPE_DETERMINER (9)
#define GIA_SEMANTIC_PARSER_POS_TYPE_COMMA (10)
#define GIA_SEMANTIC_PARSER_POS_TYPE_CONJUNCTION (11)
#define GIA_SEMANTIC_PARSER_POS_TYPE_INTERJECTION (12)
#define GIA_SEMANTIC_PARSER_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES (13)
static string GIAconnectionistNetworkPOStypeNameArray[GIA_SEMANTIC_PARSER_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES] = {"isNoun", "isVerbOrParticiple", "isAdjectiveOrAdverb", "isPreposition", "isAuxillaryBeing", "isAuxillaryHaving", "isAuxillaryDoing", "isAuxillaryModal", "isDeterminerThatWhich", "isDeterminer", "isComma", "isConjunction", "isInterjection"};
*/

//the following definitions must map to relationGovernorCompositionNameArray[]:
#define RELATION_GOVERNOR_COMPOSITION_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES (3)
static string relationGovernorCompositionPastTenseNameArray[RELATION_GOVERNOR_COMPOSITION_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES] = {"contained", "comprised", "had"};
#define RELATION_GOVERNOR_COMPOSITION_CONTINUOUS_TENSE_NAME_ARRAY_NUMBER_OF_TYPES (3)
static string relationGovernorCompositionContinuousTenseNameArray[RELATION_GOVERNOR_COMPOSITION_CONTINUOUS_TENSE_NAME_ARRAY_NUMBER_OF_TYPES] = {"containing", "comprised" "having"};
#define RELATION_AUXILIARY_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES (4)
static string relationAuxiliaryPastTenseNameArray[RELATION_AUXILIARY_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES] = {"was", "were", "had", "did"};
#define RELATION_AUXILIARY_FUTURE_TENSE_NAME_ARRAY_NUMBER_OF_TYPES (1)
static string relationAuxiliaryFutureTenseNameArray[RELATION_AUXILIARY_FUTURE_TENSE_NAME_ARRAY_NUMBER_OF_TYPES] = {"will"};	//FUTURE: take into account all principal modal verbs; can, could, may, might, must, shall, should, will, would
//must use LRP to determine continuous tense..

#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTY_DIRECT (12)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_DIRECT (13)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_DETERMINER (14)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA (15)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_QUANTITY (16)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_COMPOSITION_AUXILIARY_NOT_USED (17)
#ifdef GIA_DISABLE_ALIAS_ENTITY_MERGING
	#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_DEFINITION_MARK_CONNECTION_AS_ALIAS (18)
	#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_MERGE_OR_ALIAS_NAME "alias"
#else
	#ifdef GIA_SEMANTIC_PARSER_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
		#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_MERGE_ENTITY_NODES_ADD_ALIAS (18)
	#endif
	#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_MERGE_OR_ALIAS_NAME "merge"
#endif
#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEMANTIC_PARSER_SEMANTIC_RELATION_GENERATION
	#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_PRENOMINAL_MODIFIER (19)
#endif
#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
	#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_NONE (20)
#endif
#define GIA_SEMANTIC_PARSER_SEMANTIC_DEPENDENCY_RELATION_NUMBER_OF_TYPES (GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES + 10)	//extends GIAentityNodeClass.h GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES

static string GIA2semanticDependencyRelationNameArray[GIA_SEMANTIC_PARSER_SEMANTIC_DEPENDENCY_RELATION_NUMBER_OF_TYPES] = {"action", "actionReverse", "condition", "conditionReverse", "property", "propertyReverse", "definition", "definitionReverse", "relationshipSubject", "relationshipObject", "instance", "instanceReverse", "propertyDirect", "definitionDirect", "determiner", "modalAuxiliaryOrCopula", "quantity", "compositionAuxiliaryNOTUSED", GIA_ENTITY_VECTOR_CONNECTION_TYPE_MERGE_OR_ALIAS_NAME, "prenominalModifier", "none"};
#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
#define GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS (2)
#define GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET (2)
#define GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES (GIA_SEMANTIC_PARSER_SEMANTIC_DEPENDENCY_RELATION_NUMBER_OF_TYPES*GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET)
#endif

#define GIA_SEMANTIC_PARSER_SYNTACTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES (4)
static string GIA2syntacticDependencyRelationSecondaryNameArray[GIA_SEMANTIC_PARSER_SYNTACTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES] = {RELATION_TYPE_MODAL_AUX, RELATION_TYPE_PASSIVE_AUX, RELATION_TYPE_COPULA, RELATION_TYPE_DETERMINER};		//is RELATION_TYPE_PRENOMIAL_MODIFIER required to be added to this list?
#define GIA_SEMANTIC_PARSER_SEMANTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES (3)
static string GIA2semanticDependencyRelationSecondaryNameArray[GIA_SEMANTIC_PARSER_SEMANTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES] = {"determiner", "compositionAuxiliary", "modalAuxiliaryOrCopula"};		//required to disable is/have etc nodes only



#endif






#endif

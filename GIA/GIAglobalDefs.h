/*******************************************************************************
 *
 * File Name: GIAglobalsDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j1f 26-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: GIA specific version of shared globals (replace if compiling other projects)
 *
 *******************************************************************************/

#ifndef HEADER_GIA_GLOBAL_DEFS
#define HEADER_GIA_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"

//variables currently under attention for testing purposes
#define WORDNET_SEARCH_RELATED_SYNSETS		//TESTING THIS NOW (1j2b)
//#define GIA_DISABLE_REFERENCING		//default: off	{this needs to disable some additional parameters also... (NB properties are still being connected, and not created anew)}
//#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS		//{//this should only be enabled with Relex??} 	//this was (effectively) disabled before version 1h3b [but can be re-enabled for testing using new Stanford implementation]
#ifndef GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS
	#define FILL_NER_ARRAY_AFTER_RELEX_PARSE_FOR_STANFORD_EQUIVALENT_PROPER_NOUN_DETECTION 	//added 26 April 2012
#endif
#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
//#define GIA_TRIAL_WORD_NET_SYNONYM_LOOKUP
#define GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT

//Debug;
//#define GIA_WORDNET_DEBUG_OUTPUT_SYNONYMNS
//#define GIA_WORDNET_DEBUG
//#define GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG2
//#define GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
//#define GIA_TRANSLATOR_DEBUG
//#define GIA_NLP_DEBUG
//#define GIA_DRAW_DEBUG
//#define GIA_DATABASE_DEBUG
//#define GIA_QUERY_DEBUG
//#define GIA_SEMANTIC_NET_XML_DEBUG


//Wordnet
#define USE_WORDNET	(takes into account synonymns) 
#ifdef USE_WORDNET
	#define GIA_USE_SYNONYMN_DETECTION
#endif

//~External NLP Parser
#define GIA_USE_RELEX
#define GIA_USE_STANFORD_CORENLP
#define GIA_USE_STANFORD_PARSER
#ifdef GIA_USE_RELEX
	//#define GIA_USE_RELEX_IN_STANFORD_COMPATIBILITY_MODE	//untested - not implemented
	#ifdef GIA_USE_RELEX_IN_STANFORD_COMPATIBILITY_MODE
		#define GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	#endif
	#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.sh"
	#define GIA_USE_RELEX_1.4.0	//default: enabled (otherwise use Relex 1.3.0)
	#ifdef GIA_USE_RELEX_1.4.0
		#define GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS		//BAI paragraph tag support has not yet been added to Relex 1.3.0
	#endif
#endif
#ifdef GIA_USE_STANFORD_CORENLP		//a more advanced implementation of stanford parser (with lemma, entity name detection, etc: Stanford CoreNLP integrates all our NLP tools for the English language, including the part-of-speech (stanfordPOS) tagger, the named entity recognizer (NER), the parser, and the coreference resolution system)
	#define GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	#define GIA_STANFORD_NLP_EXECUTABLE_NAME "execute-stanfordCoreNLP.sh"
#endif
#ifdef GIA_USE_STANFORD_PARSER
	#define GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	#define GIA_STANFORD_PARSER_EXECUTABLE_NAME "execute-stanfordParser.sh"
#endif
#define GIA_NLP_PARSER_RELEX (0)
#define GIA_NLP_PARSER_STANFORD_CORENLP (1)
#define GIA_NLP_PARSER_STANFORD_PARSER (2)
#define GIA_NLP_PARSER_NUMBER_OF_TYPES (3)
#define GIA_NLP_FEATURE_PARSER_FOR_INPUT_TEXT_DEFAULT (GIA_NLP_PARSER_STANFORD_CORENLP)
#define GIA_NLP_RELATIONS_PARSER_FOR_INPUT_TEXT_DEFAULT (GIA_NLP_PARSER_STANFORD_PARSER)	//NB GIA_NLP_PARSER_STANFORD_CORENLP will be default in future, once it works better than GIA_NLP_PARSER_STANFORD_PARSER.
#define GIA_NLP_FEATURE_PARSER_FOR_INPUT_QUERY_DEFAULT (GIA_NLP_PARSER_RELEX)
#define GIA_NLP_RELATIONS_PARSER_FOR_INPUT_QUERY_DEFAULT (GIA_NLP_PARSER_RELEX)
#define GIA_DEPENDENCY_RELATIONS_TYPE_RELEX (0)
#define GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD (1)
#define GIA_DEPENDENCY_RELATIONS_NUMBER_OF_TYPES (2)		//Relex and Stanford tags
#define GIA_NLP_DEPENDENCY_RELATIONS_PARSER_RELEX_DEFAULT_DEPENDENCY_RELATIONS_TYPE (GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
#define GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_CORENLP_DEFAULT_DEPENDENCY_RELATIONS_TYPE (GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
#define GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_PARSER_DEFAULT_DEPENDENCY_RELATIONS_TYPE (GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
//#define GIA_STANFORD_CORE_NLP_DO_NOT_USE_CODEPENDENCIES
#define GIA_NLP_PARSER_TYPE_RELATIONS (0)
#define GIA_NLP_PARSER_TYPE_FEATURES (1)

//~GIATranslator
#define GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR	//this is required (the current set of code has had the alternative case removed - see GIATranslator.cpp.copyWithDataStructureOptions for an example set of code that supports disabling this feature)
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE "_$qVar"
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_WHERE_NECESSARY (0)
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED (0)
#else
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_WHERE_NECESSARY (1)
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED (1)
#endif

//~GIAmain
#define GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
//#define GIA_DO_NOT_PRINT_RESULTS
#ifdef USE_CE
	#define GIA_WITH_CE_DERIVE_SUBCLAIM_PREPEND
#endif

//~GIAquery
//#define GIA_QUERY_SUPPORT_NON_EXACT_QUERIES	//default: disabled (NB non-exact queries are not required for the text/query 'the dog is red' / 'is the dog red?'). non-exact queries used to be required for which questions, but not anymore [with GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES]	
	//these preprocessor have been moved to GIAglobalsDefs.h as they are used in both GIAtranslator.cpp and GIAquery.cpp
#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED	//allows for verification that answer node is of type identified by the which query eg verifies that the answer 'dog' [from text: 'the dog eats the mud. dogs are animals.'], is an animal with the query: 'which animal eats the mud?'
#endif
#define FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES (1)


//~GIAdraw
//#define GIA_DRAW_USE_PATENT			//modifies colours of nodes such that they print uniquely in black and white
//#define GIA_CMAP_CONVERSION_SANITISED 	//use format akin to Cmap Tools / not GIA formatted. linking-phrase-list -> actions + conditions. concept-list -> concepts or properties


#endif

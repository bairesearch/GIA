/*******************************************************************************
 *
 * File Name: GIAglobalsDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n4h 26-July-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: GIA specific global definitions
 *
 * 1. GIA Installation [Development] Instructions;
 *
 *	Install External Package 7 - Wordnet development libraries
 *
 *		(Linux EL5 Only)
 *		Use preinstalled EL5 Linux JPEG libraries;
 *		rpm -i wordnet-3.0-13.el6.x86_64.rpm
 *		rpm -i wordnet-devel-3.0-13.el6.x86_64.rpm
 *
 *		(Linux EL6 Only)
 *		Use preinstalled EL5 Linux JPEG libraries;
 *		rpm -i wordnet-3.0-13.el5.x86_64.rpm
 *		rpm -i wordnet-devel-3.0-13.el5.x86_64.rpm
 *
 *		(Windows XP Only {UNTESTED})
 *		Install WordNet_3.0_win32-setup.exe (from SourceForge [/ Google Source?])
 *		copy wn.lib to C:\Program Files\Microsoft Visual Studio 9.0\VC\lib\
 *		copy wn.h to C:\Program Files\Microsoft Visual Studio 9.0\VC\Include\
 *		ensure wn.lib is added to MS Visual Studio OR project - Linker - additional dependencies
 *
 *		(Windows 7 x86_64 Only)
 *		Install WordNet_3.0_win32-setup.exe (from SourceForge [/ Google Source?])
 *		copy wn.lib to C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib
 *		copy wn.h to C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\Include\
 *		ensure wn.lib is added to MS Visual Studio OR project - Linker - additional dependencies
 *
 *		To Compile Wordnet 3.0 in Visual Studio 2010 (to create wn.lib);
 *		Install WordNet_3.0_win32-setup.exe (from SourceForge [/ Google Source?])
 *		extract C:\Program Files (x86)\WordNet\3.0\src\wn source project.zip
 *		wn project - properties - General - Common Language Runtime Support - No Common Language Runtime Support
 *		wn project - properties - General - Static Library (.lib)
 *		Build All
 *
 *******************************************************************************/

#ifndef HEADER_GIA_GLOBAL_DEFS
#define HEADER_GIA_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"

//#define GIA_QUERIES_MUST_BE_QUESTIONS	//disabled 30 June 2012 for IPentity support

#define GIA_USE_NLG
#ifdef GIA_USE_NLG
	#define GIA_USE_NLG_OUTPUT_TO_COMMAND_LINE
	//#define GIA_USE_NLG2
	#define GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR	//NB even NLG2 requires origWord not lemma, so a morphology generator is required in both
#endif
#define STANFORD_PARSER_USE_POS_TAGS	//added 23 July 2012 to support Stanford Parser POS tags (which are sometimes more accurate than stanford CoreNLP pos tags)

//variables currently been tested (1n1a+)
#define GIA_USE_1N1ATEMP1TO8_CHANGES	//added 1n1aTEMP9

//#define GIA_USE_DATABASE	//this needs to be debugged (not yet tested)
#ifdef GIA_USE_DATABASE
	#define GIA_USE_DATABASE_FILESYSTEM
	//#define GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
	#define GIA_DATABASE_CLEAR_CACHE_EVERY_SENTENCE
#endif
#define GIA_USE_ADVANCED_REFERENCING	//this needs to be debugged (not yet tested)
#ifdef GIA_USE_ADVANCED_REFERENCING
	//#define GIA_USE_ADVANCED_REFERENCING_UPDATE_NOT_NECESSARY_OR_TESTED
	#define GIA_USE_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
	#define GIA_USE_ADVANCED_REFERENCING_ORIGINAL
	#define GIA_USE_ADVANCED_REFERENCING_PREPOSITIONS
	#define GIA_USE_ADVANCED_REFERENCING_CONDITIONS		//all conditions; ie, not just preposition conditions
	#define GIA_USE_ADVANCED_REFERENCING_ASSERT_MINIMUM_SENTENCE_INDEX_OF_REFERENCE_SET	//added 1m5aTEMP11 [requires Stanford Parser - incompatible with Relex - because it requires record of the sentence entity index of the determiner "the"; grammaticalIsDefiniteIndexOfDeterminer/grammaticalDefiniteIndexOfDeterminerTemp] 
#endif

//variables currently under attention for testing purposes

#define GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING			//default: on	//this enables pronoun detection	//OLD: {this needs to disable some additional parameters also... (NB properties are still being connected, and not created anew)}
#define WORDNET_SEARCH_RELATED_SYNSETS
#define GIA_ASSIGN_INSTANCE_PROPERTY_TO_PROPER_NOUNS		//{//this should only be enabled with Relex??} 	//this was (effectively) disabled before version 1h3b [but can be re-enabled for testing using new Stanford implementation]
#define GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
//#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE2
/*#define GIA_USE_ORIGINAL_PRONOMINAL_COREFERENCE_RESOLUTION_IGNORE_PROPERTIES_TAKE_CONCEPTS_ONLY	//untested (removed 1l1c)*/	//commented out completely
#define GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
#define GIA_USE_ADVANCED_REFERENCING_FIND_ALL_RELATIONS_MATCHING_AUXILLARY_AND_SET_DIFFERENT_REFERENCE_SET

//variables passed testing:
#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
//#define GIA_TRIAL_WORD_NET_SYNONYM_LOOKUP

//Debug [Disable these for official build];
//#define GIA_NLG_DEBUG
#ifdef GIA_USE_NLG2
	#define GIA_NLG_DEBUG_MANUALLY_HARDCODE_INTO_NLG2
#endif
//#define GIA_ADVANCED_REFERENCING_DEBUG_TOO_LARGE_REFERENCE_SET
//#define GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_CONNECTIONS_WITH_COLOURS
//#define GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS
#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS
	//#define GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS_WORK_WITH_MULTIPLE_SENTENCES
#endif
//#define GIA_ADVANCED_REFERENCING_DEBUG  	      //just finished first debug using this (1l5f)
//#define GIA_QUERY_DEBUG
//#define GIA_STANFORD_CORENLP_CODEPENDENCY_PRONOMINAL_REFERENCING_DEBUG
//#define GIA_DATABASE_DEBUG_FILESYSTEM_IO
//#define GIA_DATABASE_DEBUG		      //this needs to be debugged (not yet tested)
//#define GIA_QUERY_MULTIPLE_ANSWERS_DEBUG      //just finished first debug using this (1l2a)
//#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
//#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
//#define GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
//#define GIA_WORDNET_DEBUG
//#define GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG2
//#define GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
//#define GIA_TRANSLATOR_DEBUG
//#define GIA_NLP_DEBUG
//#define GIA_DRAW_DEBUG
//#define GIA_SEMANTIC_NET_XML_DEBUG

//DebugOLD;
//#define GIA_WORDNET_DEBUG_OUTPUT_SYNONYMNS	//Do not enable else will prematurely exit


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
	#ifdef LINUX
		#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.sh"
	#else
		#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.bat"
	#endif
	#define GIA_USE_RELEX_1_4_0	//default: enabled (otherwise use Relex 1.3.0)
	#ifdef GIA_USE_RELEX_1_4_0
		#define GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS		//BAI paragraph tag support has not yet been added to Relex 1.3.0
	#endif
#endif
#ifdef GIA_USE_STANFORD_CORENLP		//a more advanced implementation of stanford parser (with lemma, entity name detection, etc: Stanford CoreNLP integrates all our NLP tools for the English language, including the part-of-speech (stanfordPOS) tagger, the named entity recognizer (NER), the parser, and the coreference resolution system)
	#define GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	#ifdef LINUX
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME "execute-stanfordCoreNLP.sh"
	#else
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME "execute-stanfordCoreNLP.bat"
	#endif
#endif
#ifdef GIA_USE_STANFORD_PARSER
	#define GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	#ifdef LINUX
		#define GIA_STANFORD_PARSER_EXECUTABLE_NAME "execute-stanfordParser.sh"
	#else
		#define GIA_STANFORD_PARSER_EXECUTABLE_NAME "execute-stanfordParser.bat"
	#endif
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

#define GIA_NLP_PARSER_TYPE_RELATIONS (0)
#define GIA_NLP_PARSER_TYPE_FEATURES (1)

//~GIATranslator
#define GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR	//this is required (the current set of code has had the alternative case removed - see GIATranslator.cpp.copyWithDataStructureOptions for an example set of code that supports disabling this feature)
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE "_$qVar"
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED	//this is now MANDATORY
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
	//#define GIA_WITH_CE_USE_ALL_CLAIM_COMBINATIONS
#endif

//~GIAquery
#define GIA_QUERY_RELAX_CONFIDENCE_REQUIREMENTS_FOR_YES
#define GIA_QUERY_DOUBLE_ERROR (0.0001)
#define GIA_QUERY_CONFIDENCE_MULTIPLIER (10)
	//these preprocessor have been moved to GIAglobalsDefs.h as they are used in both GIAtranslator.cpp and GIAquery.cpp
#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED	//allows for verification that answer node is of type identified by the which query eg verifies that the answer 'dog' [from text: 'the dog eats the mud. dogs are animals.'], is an animal with the query: 'which animal eats the mud?'
#endif



//~GIAdraw
//#define GIA_DRAW_USE_PATENT			//modifies colours of nodes such that they print uniquely in black and white
//#define GIA_CMAP_CONVERSION_SANITISED 	//use format akin to Cmap Tools / not GIA formatted. linking-phrase-list -> actions + conditions. concept-list -> concepts or properties
#define GIA_DRAW_DISPLAY_ANSWER_CONTEXTS

#endif

/*******************************************************************************
 *
 * File Name: GIAglobalsDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i4a 3-Apr-2012
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: GIA specific version of shared globals (replace if compiling other projects)
 *
 *******************************************************************************/

#ifndef HEADER_GIA_GLOBAL_DEFS
#define HEADER_GIA_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"



//#define GIA_DRAW_DEBUG
//#define GIA_DATABASE_DEBUG
//#define GIA_TRANSLATOR_DEBUG
//#define GIA_QUERY_DEBUG
//#define GIA_SEMANTIC_NET_XML_DEBUG

//~GIATranslator
#define GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR	//this is required (the current set of code has had the alternative case removed - see GIATranslator.cpp.copyWithDataStructureOptions for an example set of code that supports disabling this feature)
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE "_$qVar"
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY (0)
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED (0)
#else
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_WHERE_NECESSARY (1)
	#define GIA_PERFORM_RELATION_FUNCTION_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED (1)
#endif
#define GIA_USE_RELEX_1.4.0

//~GIAmain
#define GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
//#define GIA_DO_NOT_PRINT_RESULTS
#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.sh"
//OLD: #define GIA_RELEX_EXECUTABLE_RELATIVE_PATH_NAME "/relex-1.3.0/"	//gets added to executable path name
#define GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
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
//#define GIA_DRAW_USE_PATENT
//#define TRIAL_DATA_CXL_CONVERSION_ONLY
#ifdef TRIAL_DATA_CXL_CONVERSION_ONLY
	#define GIA_CMAP_CONVERSION_SANITISED 	//use format akin to Cmap Tools / not GIA formatted. linking-phrase-list -> actions + conditions. concept-list -> concepts or properties
#endif


#endif

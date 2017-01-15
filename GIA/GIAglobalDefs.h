/*******************************************************************************
 *
 * File Name: GIAglobalsDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j1a 2-Apr-2012
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 * Description: GIA specific version of shared globals (replace if compiling other projects)
 *
 *******************************************************************************/

#ifndef HEADER_GIA_GLOBAL_DEFS
#define HEADER_GIA_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"

#ifdef USE_CE
	#define GIA_WITH_CE_DERIVE_SUBCLAIM_PREPEND
#endif

//these preprocessor have been moved to ...globalVars.h as they are used in both GIAtranslator.cpp and GIAquery.cpp
#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED	//allows for verification that answer node is of type identified by the which query eg verifies that the answer 'dog' [from text: 'the dog eats the mud. dogs are animals.'], is an animal with the query: 'which animal eats the mud?'
#endif
#define FEATURE_QUERY_WORD_ACCEPTED_BY_ALTERNATE_METHOD_NUMBER_OF_TYPES (1)

#endif

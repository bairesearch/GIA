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
 * File Name: GIAtranslator.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3o3b 16-November-2020
 * Requirements: requires text parsed by X Parser
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR
#define HEADER_GIA_TRANSLATOR

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAtranslatorReferencing.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
#include "GIAsemRelTranslatorParser.hpp"
#else
#include "GIAposRelTranslatorParser.hpp"
#endif
#else
#include "GIAsynRelTranslatorParser.hpp"
#include "GIAnlp.hpp"	//required because parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences was shifted from GIAmain.cpp
#ifdef GIA_SEM_REL_TRANSLATOR
#include "GIAsemRelTranslatorParser.hpp"
#endif
#endif
#include "GIAdatabase.hpp"
#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
#include "LRPpreprocessorWordReduction.hpp"
#endif
#ifdef LRP_PREPROCESSOR
#include "LRPpreprocessorSentenceClass.hpp"
#endif
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
#include "GIAposRelTranslatorHybrid.hpp"
#endif
#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
//#include "SANIrules.hpp"
#include "SANInodes.hpp"
#include "SANIposRelTranslatorPermutations.hpp"
#include "SANIpropagateOperations.hpp"
#endif

#include "SHAREDvars.hpp"


class GIAtranslatorClass
{
	private: GIAsentenceClassClass GIAsentenceClass;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAtranslatorReferencingClass GIAtranslatorReferencing;
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	private: GIAsemRelTranslatorParserClass GIAsemRelTranslatorParser;
	#else
	private: GIAposRelTranslatorParserClass GIAposRelTranslatorParser;
	#endif
	#else
	private: GIAsynRelTranslatorParserClass GIAsynRelTranslatorParser;
	private: GIAnlpClass GIAnlp;
	#ifdef GIA_SEM_REL_TRANSLATOR
	private: GIAsemRelTranslatorParserClass GIAsemRelTranslatorParser;
	#endif
	#endif
	private: GIAdatabaseClass GIAdatabase;
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	private: LRPpreprocessorWordReductionClass LRPpreprocessorWordReduction;
	#endif
	#ifdef LRP_PREPROCESSOR
	private: LRPpreprocessorSentence LRPpreprocessorSentenceObject;
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	private: GIAposRelTranslatorHybridClass GIAposRelTranslatorHybrid;
	#endif
	#ifdef SANI_PARSE_SIMULTANEOUS_SET_WORD_POSTYPE_INFERRED_DYNAMIC_OPTIMISED
	//private: SANIrulesClass SANIrules;
	private: SANInodesClass SANInodes;
	private: SANIposRelTranslatorPermutationsClass SANIposRelTranslatorPermutations;
	private: SANIpropagateOperationsClass SANIpropagateOperations;
	#endif
	private: SHAREDvarsClass SHAREDvars;

	public: bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, string outputCFFfileName);
		private: bool createSemanticNetworkBasedUponDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables);
			private: bool convertSentenceListRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables);
				private: bool convertSentenceRelationsIntoGIAnetworkNodesWrapper(GIAtranslatorVariablesClass* translatorVariables);
					private: bool convertSentenceRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList);

	bool performGIApostProcessing(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
		private: void dreamModeLinkSpecificConceptsAndActions(GIAtranslatorVariablesClass* translatorVariables);
			#ifdef GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES
			private: void createAndLinkNonSpecificConceptsForAllEntities(GIAtranslatorVariablesClass* translatorVariables);
			#endif
		#endif

};


#endif

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
 * File Name: GIAsynRelTranslatorApplyAdvancedFeatures.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f2e 04-April-2018
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SYN_REL_TRANSLATOR_APPLY_ADVANCED_FEATURES
#define HEADER_GIA_SYN_REL_TRANSLATOR_APPLY_ADVANCED_FEATURES

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAdatabase.hpp"
#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
#include "GIAsemRelTranslatorOperations.hpp"
#endif
#include "GIAsynRelTranslatorGeneric.hpp"

class GIAsynRelTranslatorApplyAdvancedFeaturesClass
{
	private: GIAconditionNodeClassClass GIAconditionNodeClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	#ifdef GIA_SEM_REL_TRANSLATOR_GENERATE_EXPERIENCES_FOR_CONNECTIONIST_NETWORK_TRAIN
	private: GIAsemRelTranslatorOperationsClass GIAsemRelTranslatorOperations;
	#endif
	private: GIAsynRelTranslatorGenericClass GIAsynRelTranslatorGeneric;
	public: void applyAdvancedFeatures(GIAtranslatorVariablesClass* translatorVariables);
		private: void extractDates(GIAtranslatorVariablesClass* translatorVariables);					//Stanford Compatible
			#ifdef GIA_RELEX
			private: void extractDatesRelex(GIAtranslatorVariablesClass* translatorVariables);
			#endif
			#ifdef GIA_STANFORD_CORENLP
			private: void extractDatesStanfordCoreNLP(GIAtranslatorVariablesClass* translatorVariables);					//Stanford Specific	[requires NERTemp info to be set, + need to know if timeConditionNodes have already been added; if not add them now]
			#endif
				public: void addTimeToSubstance(GIAentityNode* timeConditionEntity);
		private: void extractQuantities(GIAtranslatorVariablesClass* translatorVariables);	//Stanford Compatible
			#ifdef GIA_RELEX
			private: void extractQuantitiesRelex(GIAtranslatorVariablesClass* translatorVariables);
			#endif
			#ifdef GIA_STANFORD_CORENLP
			private: void extractQuantitiesStanfordCoreNLP(GIAtranslatorVariablesClass* translatorVariables);		//Stanford Specific	[requires NERTemp info to be set]
			#endif

		public: void defineTenseOnlyTimeConditions(GIAtranslatorVariablesClass* translatorVariables);	//#ifdef GIA_SEM_REL_TRANSLATOR_READ_SEMANTIC_RELATIONS || #ifdef GIA_TRANSLATOR_XML_INTERPRETATION

		#ifdef GIA_SPECIFIC_ACTION_NETWORK_INDEXES
		private: void defineActionConcepts1(GIAtranslatorVariablesClass* translatorVariables);
		private: void defineActionConcepts2(GIAtranslatorVariablesClass* translatorVariables);
		#endif

		#ifdef GIA_CREATE_NEW_CONCEPT_FOR_EVERY_REFERENCE_TO_A_CONCEPT
		private: void updateConceptDesignationBasedPropertyOwnerContext(GIAtranslatorVariablesClass* translatorVariables);
		#endif
};
	

#endif

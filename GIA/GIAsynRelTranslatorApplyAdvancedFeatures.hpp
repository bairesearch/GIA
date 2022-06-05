 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsynRelTranslatorApplyAdvancedFeatures.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
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
			private: void extractDatesStanfordCoreNLP(const GIAtranslatorVariablesClass* translatorVariables);					//Stanford Specific	[requires NERTemp info to be set, + need to know if timeConditionNodes have already been added; if not add them now]
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
		private: void updateConceptDesignationBasedPropertyOwnerContext(const GIAtranslatorVariablesClass* translatorVariables);
		#endif
};
	

#endif

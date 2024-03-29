 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAtranslatorGrammar.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by X Parser
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_GRAMMAR
#define HEADER_GIA_TRANSLATOR_GRAMMAR

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAsynRelTranslatorGeneric.hpp"
#ifdef GIA_TRANSLATOR_WITHOUT_SYN_REL_TRANSLATOR_FEATURES
#include "GIAsemRelTranslatorDefs.hpp"
#endif
#include "GIAdatabase.hpp"
#include "LRPpreprocessorWordIdentification.hpp"


//#define GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILIARY_TO_SET_TENSE_OF_VERB	//this seems theoretically possible pased upon the examples given (ie because the tense stanfordPOS information is replicated in both the auxiliary and the verb)


class GIAtranslatorGrammarClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: GIAsynRelTranslatorGenericClass GIAsynRelTranslatorGeneric;
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	private: SHAREDvarsClass SHAREDvars;
	

	#ifdef GIA_TRANSLATOR_WITHOUT_SYN_REL_TRANSLATOR_FEATURES
	public: bool calculateGrammarUsingInferredPosTypes(LRPpreprocessorPlainTextWord* currentWord, GIAfeature* currentFeature);
	#endif

	public: void locateAndAddAllFeatureTempEntities(GIAtranslatorVariablesClass* translatorVariables);
	public: void locateAndAddAllNetworkIndexEntities(GIAtranslatorVariablesClass* translatorVariables);

	#ifdef GIA_RELEX
	public: void fillGrammaticalArraysRelex(GIAsentence* currentSentenceInList);
	#endif
//uses dependency relations to derive grammar [uses stanfordPOS/NER information to derive grammar, if NLPfeatureParser == Stanford Core NLP]
	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS
	public: void fillGrammaticalArraysStanford(GIAtranslatorVariablesClass* translatorVariables);
		private: void extractPastTense(GIAfeature* featureWithEntityIndex, const int entityIndexContainingTenseIndication, const GIAfeature* firstFeatureInList, const int NLPfeatureParser);
			private: void extractPastTenseFromPOStag(const string* POStag, GIAfeature* feature);
		public: void extractGrammaticalInformationStanford(GIAfeature* firstFeatureInList, const int NLPfeatureParser);
			public: void extractPOSrelatedGrammaticalInformationStanford(GIAfeature* currentFeature, bool grammaticallyStrict);
				private: void extractGrammaticalInformationFromStanfordPOStag(const string* POStag, GIAfeature* feature);
	#endif

	public: void applyGrammaticalInfoToAllEntities(const GIAtranslatorVariablesClass* translatorVariables, GIAfeature* firstFeatureInSentence);
		public: void applyPOSrelatedGrammaticalInfoToEntity(GIAentityNode* entity, GIAfeature* currentFeatureInList);

	#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
	private: void findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet(GIAsentence* currentSentenceInList, const int subjectObjectEntityWithAuxiliaryEntityIndex, const string* subjectObjectEntityWithAuxiliaryEntityName);
	#endif
};


#endif

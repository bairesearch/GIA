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
 * File Name: GIAtranslatorDefineGrammar.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e10a 15-January-2018
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR
#define HEADER_GIA_TRANSLATOR_DEFINE_GRAMMAR

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIAtranslatorGeneric.hpp"
#include "GIAdatabase.hpp"

//#define GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILIARY_TO_SET_TENSE_OF_VERB	//this seems theoretically possible pased upon the examples given (ie because the tense stanfordPOS information is replicated in both the auxiliary and the verb)



//Pass A
class GIAtranslatorDefineGrammarClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: GIAtranslatorGenericClass GIAtranslatorGeneric;
	private: SHAREDvarsClass SHAREDvars;
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
			public: void extractPOSrelatedGrammaticalInformationStanford(GIAfeature* currentFeature, const bool grammaticallyStrict);
				private: void extractGrammaticalInformationFromStanfordPOStag(const string* POStag, GIAfeature* feature);
	#endif

	public: void applyGrammaticalInfoToAllEntities(GIAtranslatorVariablesClass* translatorVariables, GIAfeature* firstFeatureInSentencee);
		public: void applyPOSrelatedGrammaticalInfoToEntity(GIAentityNode* entity, GIAfeature* currentFeatureInList);

	#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILIARY_AND_SET_NOT_SAME_REFERENCE_SET
	private: void findSubjObjRelationMatchingAuxiliaryAndSetNotSameReferenceSet(GIAsentence* currentSentenceInList, const int subjectObjectEntityWithAuxiliaryEntityIndex, const string* subjectObjectEntityWithAuxiliaryEntityName);
	#endif
};


#endif

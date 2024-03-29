 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsynRelTranslatorParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Syntactic Relation Translator - Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SYN_REL_TRANSLATOR_PARSER
#define HEADER_GIA_SYN_REL_TRANSLATOR_PARSER

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
#include "XMLparserClass.hpp"
#endif
#include "GIAtranslatorGrammar.hpp"
#include "GIAsynRelTranslatorRedistributeRelations.hpp"
#include "GIAsynRelTranslatorRedistributeRelationsRelex.hpp"
#include "GIAsynRelTranslatorRedistributeRelationsStanford.hpp"
#include "GIAtranslatorReferencing.hpp"
#include "GIAsynRelTranslatorDefineSubstances.hpp"
#include "GIAsynRelTranslatorLinkEntities.hpp"
#include "GIAsynRelTranslatorLinkEntitiesDynamic.hpp"
#include "GIAsynRelTranslatorApplyAdvancedFeatures.hpp"
#include "GIAdatabase.hpp"
#include "GIAnlp.hpp"	//required because parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences was shifted from GIAmain.cpp
#include "GIAnlpParser.hpp"
#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
#include "GIAbot.hpp"
#endif
#include "LRPpreprocessorWordIdentification.hpp"
//#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
//#include "LRPpreprocessorWordReduction.hpp"
//#endif
#include "GIAsemRelTranslatorOperations.hpp"
#include "GIAsemRelTranslatorParser.hpp"
#include "GIAsemRelTranslatorDatabase.hpp"
#include "GIAsynRelTranslatorRules.hpp"
#include "GIAsynRelTranslatorGeneric.hpp"
#ifdef LRP_PREPROCESSOR
#include "LRPpreprocessor.hpp"
#ifdef LRP_PREPROCESSOR_RECORD_REFERENCES
#include "LRPpreprocessorSentenceClass.hpp"
#include "LRPpreprocessorWordClass.hpp"
#endif
#endif
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
#include "GIAposRelTranslatorHybrid.hpp"
#endif
#include "SHAREDvars.hpp"	//file io


class GIAsynRelTranslatorParserClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: SHAREDvarsClass SHAREDvars;
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	//#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	//private: LRPpreprocessorWordReductionClass LRPpreprocessorWordReduction;
	//#endif
	private: GIAnlpClass GIAnlp;
	private: GIAdatabaseClass GIAdatabase;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAtranslatorReferencingClass GIAtranslatorReferencing;
	#ifdef GIA_SEM_REL_TRANSLATOR
	private: GIAsemRelTranslatorParserClass GIAsemRelTranslatorParser;
	private: GIAsemRelTranslatorOperationsClass GIAsemRelTranslatorOperations;
	private: GIAsemRelTranslatorDatabaseClass GIAsemRelTranslatorDatabase;
	#endif
	private: GIAtranslatorGrammarClass GIAtranslatorGrammar;
	private: GIAsynRelTranslatorRedistributeRelationsClass GIAsynRelTranslatorRedistributeRelations;
	#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
	private: GIAsynRelTranslatorRedistributeRelationsRelexClass GIAsynRelTranslatorRedistributeRelationsRelex;
	private: GIAsynRelTranslatorRedistributeRelationsStanfordClass GIAsynRelTranslatorRedistributeRelationsStanford;
	private: GIAsynRelTranslatorDefineSubstancesClass GIAsynRelTranslatorDefineSubstances;
	private: GIAsynRelTranslatorLinkEntitiesClass GIAsynRelTranslatorLinkEntities;
	#endif
	private: GIAsynRelTranslatorLinkEntitiesDynamicClass GIAsynRelTranslatorLinkEntitiesDynamic;
	private: GIAsynRelTranslatorApplyAdvancedFeaturesClass GIAsynRelTranslatorApplyAdvancedFeatures;
	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	private: GIAbotClass GIAbot;
	#endif
	private: GIAsynRelTranslatorRulesClass GIAsynRelTranslatorRules;
	#ifdef LRP_PREPROCESSOR
	private: LRPpreprocessorClass LRPpreprocessor;
	#ifdef LRP_PREPROCESSOR_RECORD_REFERENCES
	private: LRPpreprocessorSentenceClass LRPpreprocessorSentenceClassObject;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	#endif
	#endif
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	private: GIAposRelTranslatorHybridClass GIAposRelTranslatorHybrid;
	#endif
	
	public: bool convertSentenceSyntacticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, GIAcoreference* firstGIAcoreferenceInList);
		private: bool convertSentenceSyntacticRelationsIntoGIAnetworkNodesEndPart1(const GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entityNodesActiveListSentence);
		public: bool convertSentenceRelationsIntoGIAnetworkNodesEndPart2(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entityNodesActiveListSentence);
		private: void disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables);
		private: void disableEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables);		//added 15 July 2012

	//?substance to substance relationship - these they in actual fact represent different levels of detail in information to substance to action / action to action nodes - direct substance to substance relationships are missing the action/connectivity information

	#ifdef LRP_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	private: void invertOrDuplicateConditionsIfRequired(GIAtranslatorVariablesClass* translatorVariables);
		private: void createNewInverseConditionEntity(GIArelation* currentRelationInList, GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName);
	#endif
		
	#ifdef LRP_PREPROCESSOR_RECORD_REFERENCES
	public: bool addSentenceToPreprocessorSentence(GIAtranslatorVariablesClass* translatorVariables);
		#ifndef GIA_POS_REL_TRANSLATOR_HYBRID
		private: bool getPreprocessorSentence(constEffective LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, const int sentenceIndex, constEffective LRPpreprocessorSentence** sentenceFound);
		#endif
		private: bool addSentenceFeatureOutputToPreprocessorSentenceWordList(vector<LRPpreprocessorPlainTextWord*>* preprocessorSentenceWordList, const GIAtranslatorVariablesClass* translatorVariables, const bool originalNLPfeatures);
		private: bool addSentenceEntityOutputToPreprocessorSentenceWordList(vector<LRPpreprocessorPlainTextWord*>* preprocessorSentenceWordList, GIAtranslatorVariablesClass* translatorVariables);
	#endif	

};


#endif

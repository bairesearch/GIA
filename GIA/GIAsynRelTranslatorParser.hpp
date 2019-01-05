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
 * File Name: GIAsynRelTranslatorParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3g1k 24-April-2018
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
#include "GIApreprocessorWordIdentification.hpp"
//#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
//#include "GIApreprocessorWordReduction.hpp"
//#endif
#include "GIAsemRelTranslatorOperations.hpp"
#include "GIAsemRelTranslatorParser.hpp"
#include "GIAsemRelTranslatorDatabase.hpp"
#include "GIAsynRelTranslatorRules.hpp"
#include "GIAsynRelTranslatorGeneric.hpp"
#ifdef GIA_PREPROCESSOR
#include "GIApreprocessor.hpp"
#endif
#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
#include "GIAtxtRelTranslatorHybrid.hpp"
#endif
#include "SHAREDvars.hpp"	//file io


class GIAsynRelTranslatorParserClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorWordIdentificationClass GIApreprocessorWordIdentification;
	//#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	//private: GIApreprocessorWordReductionClass GIApreprocessorWordReduction;
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
	#ifdef GIA_PREPROCESSOR
	private: GIApreprocessorClass GIApreprocessor;
	#endif
	#ifdef GIA_TXT_REL_TRANSLATOR_HYBRID
	private: GIAtxtRelTranslatorHybridClass GIAtxtRelTranslatorHybrid;
	#endif
	
	public: bool convertSentenceSyntacticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, GIAcoreference* firstGIAcoreferenceInList);
		private: bool convertSentenceSyntacticRelationsIntoGIAnetworkNodesEndPart1(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entityNodesActiveListSentence);
		public: bool convertSentenceRelationsIntoGIAnetworkNodesEndPart2(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entityNodesActiveListSentence);
		private: void disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables);
		private: void disableEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables);		//added 15 July 2012

	//?substance to substance relationship - these they in actual fact represent different levels of detail in information to substance to action / action to action nodes - direct substance to substance relationships are missing the action/connectivity information

	#ifdef GIA_PREPROCESSOR_WORD_NORMALISE_PREPOSITIONS
	private: void invertOrDuplicateConditionsIfRequired(GIAtranslatorVariablesClass* translatorVariables);
		private: void createNewInverseConditionEntity(GIArelation* currentRelationInList, GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName);
	#endif
	
	public: int getEntityArrayMaxIndex(GIAtranslatorVariablesClass* translatorVariables);

};


#endif

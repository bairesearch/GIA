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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e12a 12-February-2018
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA network nodes (of type entity, action, condition etc) in GIA network/tree
 *
 *******************************************************************************/


#ifndef HEADER_GIA_TRANSLATOR
#define HEADER_GIA_TRANSLATOR

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"
#include "GIAconditionNodeClass.hpp"
#include "GIAtranslatorOperations.hpp"
#ifdef USE_CE
#include "CECodeextensionClass.h"
#endif
#ifdef GIA_TRANSLATOR_XML_INTERPRETATION
#include "XMLparserClass.hpp"
#endif
#include "GIAtranslatorDefineGrammar.hpp"
#include "GIAtranslatorRedistributeRelations.hpp"
#include "GIAtranslatorRedistributeRelationsRelex.hpp"
#include "GIAtranslatorRedistributeRelationsStanford.hpp"
#include "GIAtranslatorDefineReferencing.hpp"
#include "GIAtranslatorDefineSubstances.hpp"
#include "GIAtranslatorLinkEntities.hpp"
#include "GIAtranslatorLinkEntitiesDynamic.hpp"
#include "GIAtranslatorApplyAdvancedFeatures.hpp"
#include "GIAdatabase.hpp"
#include "GIAnlp.hpp"	//required because parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences was shifted from GIAmain.cpp
#include "GIAnlpParser.hpp"
#ifdef GIA_BOT
#include "GIAbot.hpp"
#endif
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
#include "GIApreprocessorMultiwordReduction.hpp"
#endif
#include "GIAsemanticParserOperations.hpp"
#include "GIAsemanticParserTranslator.hpp"
#include "GIAsemanticParserDatabase.hpp"
#include "GIAtranslatorRules.hpp"
#include "GIAtranslatorGeneric.hpp"
#ifdef GIA_PREPROCESSOR
#include "GIApreprocessor.hpp"
#endif
#include "SHAREDvars.hpp"	//file io


class GIAtranslatorClass
{
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: SHAREDvarsClass SHAREDvars;
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	#endif
	private: GIAnlpClass GIAnlp;
	private: GIAdatabaseClass GIAdatabase;
	private: GIAsentenceClassClass GIAsentenceClass;
	private: GIAentityNodeClassClass GIAentityNodeClass;
	private: GIAtranslatorDefineReferencingClass GIAtranslatorDefineReferencing;
	#ifdef GIA_SEMANTIC_PARSER
	private: GIAsemanticParserTranslatorClass GIAsemanticParserTranslator;
	private: GIAsemanticParserOperationsClass GIAsemanticParserOperations;
	private: GIAsemanticParserDatabaseClass GIAsemanticParserDatabase;
	#endif
	private: GIAtranslatorDefineGrammarClass GIAtranslatorDefineGrammar;
	private: GIAtranslatorRedistributeRelationsClass GIAtranslatorRedistributeRelations;
	#ifndef GIA_TRANSLATOR_XML_INTERPRETATION
	private: GIAtranslatorRedistributeRelationsRelexClass GIAtranslatorRedistributeRelationsRelex;
	private: GIAtranslatorRedistributeRelationsStanfordClass GIAtranslatorRedistributeRelationsStanford;
	private: GIAtranslatorDefineSubstancesClass GIAtranslatorDefineSubstances;
	private: GIAtranslatorLinkEntitiesClass GIAtranslatorLinkEntities;
	#endif
	private: GIAtranslatorLinkEntitiesDynamicClass GIAtranslatorLinkEntitiesDynamic;
	private: GIAtranslatorApplyAdvancedFeaturesClass GIAtranslatorApplyAdvancedFeatures;
	#ifdef GIA_BOT
	private: GIAbotClass GIAbot;
	#endif
	private: GIAtranslatorRulesClass GIAtranslatorRules;
	#ifdef GIA_PREPROCESSOR
	private: GIApreprocessorClass GIApreprocessor;
	#endif
	public: bool parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName);
		private: bool createSemanticNetworkBasedUponDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables);
			#ifdef GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
			private: bool convertParagraphSentenceRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables);
			#endif
				#ifdef USE_CE
				private: bool convertSentenceListRelationsIntoGIAnetworkNodesBasedUponCodeextensionHeirachy(GIAtranslatorVariablesClass* translatorVariables);
					private: bool convertCEsentenceListRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables);
				#endif
				private: bool convertSentenceListRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables);
					private: bool convertSentenceRelationsIntoGIAnetworkNodesWrapper(GIAtranslatorVariablesClass* translatorVariables);
						private: bool convertSentenceSyntacticRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, GIAcoreference* firstGIAcoreferenceInList);
							private: void disableNetworkIndexEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables);
							private: void disableEntitiesBasedOnFeatureTempEntityNodeArray(GIAtranslatorVariablesClass* translatorVariables);		//added 15 July 2012
							#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES
							private: void createParentsOfSubclassEntities(GIAtranslatorVariablesClass* translatorVariables);
							#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_ENABLE_INCONSISTENT_REFERENCING
							private: void createAdditionalSubclassEntities(GIAtranslatorVariablesClass* translatorVariables);
							#endif
								private: void linkSubclassEntitiesWithParentClassEntities(GIAentityNode* subclassNetworkIndexEntity, GIAentityNode* parentClassNetworkIndexEntity, const bool linkAsAlias, const bool onlyLinkIfSubclassOrParentConceptNotFound, GIAtranslatorVariablesClass* translatorVariables);
							#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_SUBCLASSES_DETECT_USER_DECLARED_SUBCLASS_ENTITIES
							private: void detectUserDeclaredSubclassEntities(GIAtranslatorVariablesClass* translatorVariables);
							#endif
							#endif

				bool performGIApostProcessing(GIAtranslatorVariablesClass* translatorVariables);
					#ifdef GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS
					private: void dreamModeLinkSpecificConceptsAndActions(GIAtranslatorVariablesClass* translatorVariables);
						#ifdef GIA_TRANSLATOR_DREAM_MODE_CREATE_AND_LINK_NON_SPECIFIC_CONCEPTS_FOR_ALL_ENTITIES
						private: void createAndLinkNonSpecificConceptsForAllEntities(GIAtranslatorVariablesClass* translatorVariables);
						#endif
					#endif

	//?substance to substance relationship - these they in actual fact represent different levels of detail in information to substance to action / action to action nodes - direct substance to substance relationships are missing the action/connectivity information

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
	private: void invertOrDuplicateConditionsIfRequired(GIAtranslatorVariablesClass* translatorVariables);
		private: void createNewInverseConditionEntity(GIArelation* currentRelationInList, GIAtranslatorVariablesClass* translatorVariables, string inverseConditionName);
	#endif
	#endif

	#ifdef GIA_CREATE_SHORTCUTS_TO_CONCEPT_ENTITIES
	void createShortcutsToConceptEntities(GIAtranslatorVariablesClass* translatorVariables);
		bool hasSameReferenceSetConnections(GIAentityNode* entity, GIAtranslatorVariablesClass* translatorVariables);
	#endif

};


#endif

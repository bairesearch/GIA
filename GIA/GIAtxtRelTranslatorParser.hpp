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
 * File Name: GIAtxtRelTranslatorParser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f9b 18-April-2018
 * Requirements: 
 * Description: Textual Relation Translator Parser
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_TXT_REL_TRANSLATOR_PARSER
#define HEADER_GIA_TXT_REL_TRANSLATOR_PARSER

#include "GIAglobalDefs.hpp"

#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3

#include "GIAsentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAsentenceClass.hpp"
//#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
#include "GIAsemRelTranslatorOperations.hpp"
//#endif
#include "GIAtranslatorOperations.hpp"
#include "GIAtranslatorGrammar.hpp"
#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
#include "GIAbot.hpp"
#endif
#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
#include "GIAsynRelTranslatorRedistributeRelations.hpp"
#endif
#include "GIAtranslatorReferencing.hpp"
#include "GIAsemRelTranslatorParser.hpp"
#include "GIAsynRelTranslatorParser.hpp"
#include "GIAtxtRelTranslatorRules.hpp"
#include "GIAtxtRelTranslatorHybrid.hpp"
#include "GIAsynRelTranslatorDefineSubstances.hpp"
#include "GIAsynRelTranslatorApplyAdvancedFeatures.hpp"
#include "SHAREDvars.hpp"

class GIAtxtRelTranslatorParserClass
{
	private: GIAentityNodeClassClass GIAentityNodeClassObject;
	private: GIAsentenceClassClass GIAsentenceClass;
	//#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	private: GIAsemRelTranslatorOperationsClass GIAsemRelTranslatorOperations;
	//#endif
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON
	private: GIAbotClass GIAbot;
	#endif
	private: GIAtranslatorGrammarClass GIAtranslatorGrammar;
	#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	private: GIAsynRelTranslatorRedistributeRelationsClass GIAsynRelTranslatorRedistributeRelations;
	#endif
	private: GIAtranslatorReferencingClass GIAtranslatorReferencing;
	private: GIAsemRelTranslatorParserClass GIAsemRelTranslatorParser;
	private: GIAsynRelTranslatorParserClass GIAsynRelTranslatorParser;
	private: GIAtxtRelTranslatorRulesClass GIAtxtRelTranslatorRules;
	private: GIAtxtRelTranslatorHybridClass GIAtxtRelTranslatorHybrid;
	private: GIAsynRelTranslatorDefineSubstancesClass GIAsynRelTranslatorDefineSubstances;
	private: GIAsynRelTranslatorApplyAdvancedFeaturesClass GIAsynRelTranslatorApplyAdvancedFeatures;
	private: SHAREDvarsClass SHAREDvars;
	
	public: bool convertSentenceTxtRelationsIntoGIAnetworkNodes(GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA, GIAcoreference* firstGIAcoreferenceInList);
		private: bool locateAndAddAllNetworkIndexEntitiesBasedOnTxtRelations(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_COMPONENT_QUERY
		private: void identifyComparisonVariable(GIAtranslatorVariablesClass* translatorVariables);
		#endif
		#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
		public: bool generateSemanticRelationsFromTxtRelationsWrapperSentences(GIAtranslatorVariablesClass* translatorVariables);
		#endif
		private: bool generateSemanticRelationsFromTxtRelationsWrapper(GIAtranslatorVariablesClass* translatorVariables);
			#ifdef GIA_TXT_REL_TRANSLATOR_RULES_ASSUME_HIGH_LEVEL_REFERENCE_SETS_DO_NOT_CONTAIN_EXPLICIT_SEMANTIC_RELATION_FUNCTION
			private: bool reconcileSameReferenceSetConnectionsForAllRelationshipEntities(GIAtranslatorVariablesClass* translatorVariables);
			#endif
			private: void setPreprocessorSentenceTranslatorEntityReferences(GIApreprocessorSentence* currentPreprocessorSentenceInList, constEffective vector<GIAentityNode*>* GIAentityNodeArray);
			private: bool generateSemanticRelationsFromTxtRelations(GIAtranslatorVariablesClass* translatorVariables, GIAtxtRelTranslatorRulesGroup* currentParseTreeGroup, GIAentityNode** semanticRelationReturnEntity, GIAentityNode** semanticRelationReturnEntitySubject, GIAentityNode** semanticRelationReturnEntityDelimiter, string* semanticRelationReturnFunctionNameRecord, vector<GIAentityNode*>* semanticRelationReturnFunctionEntityArray, int layer);
				private: bool createSemanticRelationOrdered(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunctionSubject, GIAentityNode* entitySemanticRelationFunctionObject, GIAentityNode* entitySemanticRelationFunctionDelimiter, const bool sameReferenceSet, int layer);
				private: bool createSemanticRelationPair(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, const bool sameReferenceSet, int layer);
					private: bool createSemanticRelation(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet, int layer);
						#ifdef GIA_TXT_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
						private: bool createSemanticRelationInMemory(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet);
						#else
						private: bool createSemanticRelationInNetwork(GIAtranslatorVariablesClass* translatorVariables, const string semanticRelationFunctionName, const string semanticRelationFunctionConditionNewName, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, GIAentityNode* entitySemanticRelationFunction1, GIAentityNode* entitySemanticRelationFunction2, GIAentityNode* entitySemanticRelationFunction3, const bool sameReferenceSet);
						#endif
							private: bool createSemanticRelationMultiwordAuxiliary(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet);
							private: bool createSemanticRelationMultiwordPreposition(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet);
							private: bool createSemanticRelationMultiwordNoun(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet);
							private: bool createSemanticRelationMultiwordAlias(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet);
							#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_DATE
							private: bool createSemanticRelationMultiwordDate(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet);
							#endif
							#ifdef GIA_TXT_REL_TRANSLATOR_RULES_CODE_NUMBERS
							private: bool createSemanticRelationMultiwordQuantity(GIAtranslatorVariablesClass* translatorVariables, vector<GIAentityNode*>* entitySemanticRelationFunctionListArray, const bool sameReferenceSet);
							#endif
		private: void defineSubstancesBasedOnNetworkAndDeterminerInfo(GIAtranslatorVariablesClass* translatorVariables);

};

#endif

#endif

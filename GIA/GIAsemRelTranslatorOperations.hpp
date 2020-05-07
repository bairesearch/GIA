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
 * File Name: GIAsemRelTranslatorOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3k9b 07-May-2020
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 * Description: Semantic Relation Translator
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SEM_REL_TRANSLATOR_OPERATIONS
#define HEADER_GIA_SEM_REL_TRANSLATOR_OPERATIONS

#include "GIAglobalDefs.hpp"
#include "GIAsemRelTranslatorDefs.hpp"
#include "GIAsynRelTranslatorDefs.hpp"
#include "GIAsemRelTranslatorDatabase.hpp"
#include "GIAtranslatorOperations.hpp"


class GIAsemRelTranslatorOperationsClass
{
	private: GIAsemRelTranslatorDatabaseClass GIAsemRelTranslatorDatabase;
	private: SHAREDvarsClass SHAREDvars;

	#ifdef GIA_SEM_REL_TRANSLATOR_COMMON
	public: GIArelation* GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(const GIAtranslatorVariablesClass* translatorVariables, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet);
	public: void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(const GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA);
		private: void generateGIA2semanticDependencyRelation(constEffective vector<GIAentityNode*>* GIAentityNodeArray, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet, string* GIA2semanticDependencyRelationText, GIArelation* GIA2semanticDependencyRelation);
	#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
	#ifndef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	public: void writeSemanticParserCorpusFileOptimised(const GIAtranslatorVariablesClass* translatorVariables);
	#endif
	#endif
	public: void writeSemanticRelationsFile(const GIAtranslatorVariablesClass* translatorVariables, const string sentenceSemanticRelationsFileName);
		public: string generateSemanticParserCorpusSemanticRelationsText(const GIArelation* firstSemanticRelationInList);
	#ifdef GIA_SEM_REL_TRANSLATOR
	public: void determineGIAconnectionistNetworkPOStypeNames(GIAfeature* firstFeatureInList, const int NLPfeatureParser);
		private: void determineGIAconnectionistNetworkPOStypeNameStanford(GIAfeature* currentFeatureInSentence);
		private: void determineGIAconnectionistNetworkPOStypeNameRelex(GIAfeature* currentFeatureInSentence);
		private: void determineGIAconnectionistNetworkPOStypeNameShared(GIAfeature* currentFeatureInSentence, int* GIAsemRelTranslatorPOStype);
	#endif
	#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS
	#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS
	public: GIAfeature* generateOptimisedFeatureSubsetBasedOnContextualConjunctions(GIAfeature* firstFeatureInSentenceSubset, const int centralWord, bool* optimisedBasedOnContextualConjunctions);
	#endif
	#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
	public: int calculateFirstWordInTupleIndexRelative(const int firstWordInTupleIndex, const int firstWordInSentenceSubsetIndex);
	#endif
	#endif
	
	#endif
	
	public: string generateGIA2semanticDependencyRelationText(const string entityName1, const string entityName2, const string semanticRelation, const int entityIndex1, const int entityIndex2, const bool sameReferenceSet);
		private: string createSameReferenceSetRecord(const bool sameReferenceSet);
};



#endif

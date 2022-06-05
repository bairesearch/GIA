 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemRelTranslatorOperations.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3r1a 05-June-2022
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
		private: void generateGIA2semanticDependencyRelation(const vector<GIAentityNode*>* GIAentityNodeArray, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet, string* GIA2semanticDependencyRelationText, GIArelation* GIA2semanticDependencyRelation);
	#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
	#ifndef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	public: void writeSemanticParserCorpusFileOptimised(GIAtranslatorVariablesClass* translatorVariables);
	#endif
	#endif
	public: void writeSemanticRelationsFile(const GIAtranslatorVariablesClass* translatorVariables, string sentenceSemanticRelationsFileName);
		public: string generateSemanticParserCorpusSemanticRelationsText(const GIArelation* firstSemanticRelationInList);
	#ifdef GIA_SEM_REL_TRANSLATOR
	public: void determineGIAconnectionistNetworkPOStypeNames(GIAfeature* firstFeatureInList, const int NLPfeatureParser);
		private: void determineGIAconnectionistNetworkPOStypeNameStanford(GIAfeature* currentFeatureInSentence);
		private: void determineGIAconnectionistNetworkPOStypeNameRelex(GIAfeature* currentFeatureInSentence);
		private: void determineGIAconnectionistNetworkPOStypeNameShared(const GIAfeature* currentFeatureInSentence, int* GIAsemRelTranslatorPOStype);
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

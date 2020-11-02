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
 * File Name: GIAposRelTranslatorHybrid.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3n4a 31-October-2020
 * Requirements: requires plain text file
 * Description: Textual Relation Translator Hybrid
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_POS_REL_TRANSLATOR_HYBRID
#define HEADER_GIA_POS_REL_TRANSLATOR_HYBRID

#include "GIAglobalDefs.hpp"
#include "LRPpreprocessorWordClass.hpp"
#include "LRPpreprocessorWordIdentification.hpp"
#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
#include "LRPpreprocessorWordReduction.hpp" 
#endif
#include "GIAtranslatorOperations.hpp"
#include "GIAposRelTranslatorHybridSentenceClass.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DEPRECIATED
#include "GIAposRelTranslatorHybridLogicReference.hpp"
#include "GIAposRelTranslatorHybridReferenceSet.hpp"
#endif
#include "GIAdatabase.hpp"	//required for findParentEntityBasedOnSentenceIndex
#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
#ifndef LRP_PREPROCESSOR_POS_TAGGER
#include "GIAnlp.hpp"
#include "GIAtranslatorGrammar.hpp"
#endif
#endif
#include "SANIrules.hpp"

#ifdef GIA_POS_REL_TRANSLATOR

static bool entityVectorConnectionTransferFromDummyRelationshipToRealRelationshipArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, true, false, true, false, false, false, false, false, false, false};
static bool entityVectorConnectionChangeSentenceIndexOfEntityNodesAndConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {true, true, true, true, true, true, true, true, true, true, false, false};


class GIAposRelTranslatorHybridClass
{
	//private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: LRPpreprocessorWordClassClass LRPpreprocessorWordClassObject;
	private: LRPpreprocessorWordIdentificationClass LRPpreprocessorWordIdentification;
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	private: LRPpreprocessorWordReductionClass LRPpreprocessorWordReduction;
	#endif
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	private: GIAposRelTranslatorHybridSentenceClassClass GIAposRelTranslatorHybridSentenceClass;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DEPRECIATED
	private: GIAposRelTranslatorHybridLogicReferenceClass GIAposRelTranslatorHybridLogicReferenceObject;
	private: GIAposRelTranslatorHybridReferenceSetClass GIAposRelTranslatorHybridReferenceSet;
	#endif
	private: GIAdatabaseClass GIAdatabase;
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
	#ifndef LRP_PREPROCESSOR_POS_TAGGER
	private: GIAnlpClass GIAnlp;
	private: GIAtranslatorGrammarClass GIAtranslatorGrammar;
	#endif
	#endif
		
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_EXECUTE_PRELIM_POS_TAGGER
	public: bool executePrelimFeatureProcessingOnSentences(const string outputLRPTextPlainTXTFileName, const string inputTextNLPfeatureXMLfileName, GIAtranslatorVariablesClass* translatorVariables);
	#endif	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_DEPRECIATED
	public: bool executeTxtRelTranslatorDepreciated(LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, const string outputFileName, const string outputFileNameLRPforNLP);
		private: bool executeTxtRelTranslatorDepreciatedSentence(LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, vector<XMLparserTag*>* SANIrulesTokenLayers);
		private: string removePrependingWhiteSpace(string sentenceContents);
		private: bool generateTxtRelTranslatorHybridSentenceNLPparsablePhrases(GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, int* sentenceIndex, string* outputTextContentsSentence, string* outputTextContentsSentenceForNLP);
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
			private: void generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference(string* sentenceContentsPreprocessedLogicReference, string* sentenceContentsPreprocessedLogicReferenceForNLP, GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, int* sentenceIndex);
				private: void generateLogicReferenceSetContentsWithVariableNamesConjunction(string* logicReferenceSetContentsWithVariableNames, GIAposRelTranslatorHybridLogicReference* conjunctionLogicReference, int* sentenceIndex);
			#endif
			private: void addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(string* sentenceContentsPreprocessedLogicReferenceVariables, string* sentenceContentsPreprocessedLogicReferenceVariablesForNLP, LRPpreprocessorSubReferenceSet* referenceSet, int* sentenceIndex, int referenceSetType);
	#ifdef LRP_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
	public: bool updateLRPpreprocessorMultiwordReductionTagTextCorrespondenceInfo(LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, const bool isQuery);
		private: bool getLRPpreprocessorSentence(LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, const int sentenceIndexOriginal, LRPpreprocessorSentence** LRPpreprocessorSentenceFound);
		private: bool getGIAposRelTranslatorHybridReferenceSet(GIAposRelTranslatorHybridLogicReference* firstGIAposRelTranslatorHybridLogicReferenceInList, const int entityIndexOriginal, LRPpreprocessorSubReferenceSet** LRPpreprocessorSubReferenceSetFound);
			private: bool getGIAposRelTranslatorHybridReferenceSet(LRPpreprocessorSubReferenceSet* firstLRPpreprocessorSubReferenceInList, const int entityIndexOriginal, LRPpreprocessorSubReferenceSet** LRPpreprocessorSubReferenceSetFound);
	#endif
	#endif
			
	//these functions are executed after the GIA preprocessor and semantic relation parsing based on the preprocessed sentence objects (and are therefore stored outside of LRPpreprocessor.cpp):
	public: bool connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesWrapper(LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, GIAtranslatorVariablesClass* translatorVariables);	
		private: bool connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList, GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* parentLogicReferenceEntity);

			#ifdef LRP_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
			private: void changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(LRPpreprocessorSubReferenceSet* firstSubReferenceSetInList, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables);
			private: void changeSentenceIndexOfEntityNodesAndConnections(const int sentenceIndexOriginal, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables);
			#endif
			
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
			private: bool updateRelationshipType(GIAentityNode* relationshipEntity, LRPpreprocessorSubReferenceSet* relationshipReference, LRPpreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables);
			#else
			private: bool updateRelationshipTenseInformation(GIAentityNode* relationshipEntity, GIAentityNode* relationshipEntity, LRPpreprocessorSubReferenceSet* delimiterEntityTempTenseInformation, GIAtranslatorVariablesClass* translatorVariables);
			#endif

			private: bool findFirstImplicitConjunctionLogicReferenceSetBefore(GIAposRelTranslatorHybridLogicReference* conjunctionLogicReferenceInList, GIAposRelTranslatorHybridLogicReference** firstImplicitConjunction);
			#ifdef GIA_POS_REL_TRANSLATOR_HYBRID_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
			private: GIAentityNode* getEntity(GIAposRelTranslatorHybridLogicReference* currentLogicReferenceInList, GIAtranslatorVariablesClass* translatorVariable);
			#endif
			private: bool findPrimaryEntityAndReconcileSubReferenceSets(GIAentityNode** primaryEntityConnection, LRPpreprocessorSubReferenceSet* firstSubReferenceSetInList, GIAentityNode* primaryDelimiterEntity, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType);
			private: void deleteExternalConnectionsToConnection(GIAentityConnection* connection);
				private: bool findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType, GIAentityNode** primaryEntity, GIAentityNode* primaryDelimiterEntity);
					private: bool findParentEntityBasedOnSentenceIndex(LRPpreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, string parentEntityName, GIAentityNode** parentEntity);
					private: bool findSameSentenceRelationshipEntityNodePointerInVector(GIAentityNode* entityNode, const bool direction, GIAentityConnection** connectionFound, const int sentenceIndex);
					private: bool transferPropertyAndConditionConnectionsBetweenDummyDelimiterAndRealDelimiter(GIAentityNode* dummyDelimiterEntity, GIAentityNode* primaryDelimiterEntity);
					private: bool findSameSentenceSubjectObjectEntityNodePointerInVector(GIAentityNode* entityNode, const int connectionType, GIAentityConnection** connectionFound, const int sentenceIndex);
					private: bool findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, string entityNameToFind, GIAentityConnection** connectionFound, const int sentenceIndex);
	private: bool getRelationshipNameAndType(LRPpreprocessorSubReferenceSet* relationshipReference, LRPpreprocessorSubReferenceSet* relationshipObject, int* relationshipEntityType, string* relationshipName);
	
	public: bool getPreprocessorSentenceSubReferenceSet(LRPpreprocessorSentence* firstLRPpreprocessorSentenceInList, int sentenceIndex, LRPpreprocessorSubReferenceSet** subReferenceSetFound);
		public: bool getPreprocessorSentenceSubReferenceSet(GIAposRelTranslatorHybridLogicReference* firstLogicReferenceInList, int sentenceIndex, LRPpreprocessorSubReferenceSet** subReferenceSetFound);

	private: GIAentityNode* createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIAposRelTranslatorHybridLogicReferenceVariable* logicReferenceVariable, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
		private: GIAentityNode* createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, LRPpreprocessorSubReferenceSet* relationshipReference, LRPpreprocessorSubReferenceSet* relationshipObject, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
		private: GIAentityNode* createNewRelationship(GIAposRelTranslatorHybridLogicReferenceVariable* logicReferenceVariable, GIAtranslatorVariablesClass* translatorVariables);
			private: GIAentityNode* createNewRelationship(LRPpreprocessorSubReferenceSet* relationshipReference, LRPpreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables);
	#endif

	private: GIAentityNode* createNewRelationshipEntity(vector<LRPpreprocessorPlainTextWord*>* logicReferenceContents, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
		private: GIAentityNode* createNewRelationshipEntity(string relationshipEntityName, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);

	
};

#endif

#endif

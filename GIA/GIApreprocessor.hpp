/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIApreprocessor.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3e2d 10-December-2017
 * Requirements: requires plain text file
 * Description: Logical Condition and Reference Set preprocessor
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR
#define HEADER_GIA_PREPROCESSOR

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorMultiwordReductionClass.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"
#include "GIApreprocessorLogicReference.hpp"
#include "GIApreprocessorReferenceSet.hpp"
#include "GIAtranslatorOperations.hpp"	//required for findSameSentenceEntityNodePointerInVector
#include "GIAdatabase.hpp"	//required for findParentEntityBasedOnSentenceIndex
#ifdef GIA_PREPROCESSOR_SENTENCE_EXECUTE_PRELIM_POS_TAGGER
#include "GIAnlp.hpp"
#include "GIAtranslatorDefineGrammar.hpp"
#endif
#include "XMLparserClass.hpp"
#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
#include "GIApreprocessorPOStagger.hpp"
#endif


#define GIA_ENTITY_TYPE_LOGIC_REFERENCE (GIA_ENTITY_TYPE_ACTION)

static bool entityVectorConnectionTransferFromDummyRelationshipToRealRelationshipArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, true, false, true, false, false, false, false, false, false, false};
static bool entityVectorConnectionChangeSentenceIndexOfEntityNodesAndConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {true, true, true, true, true, true, true, true, true, true, false, false};

class GIApreprocessorClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorMultiwordReductionClassClass GIApreprocessorMultiwordReductionClassObject;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIApreprocessorSentenceClassClass GIApreprocessorSentenceClass;
	private: GIApreprocessorLogicReferenceClass GIApreprocessorLogicReferenceObject;
	private: GIApreprocessorReferenceSetClass GIApreprocessorReferenceSet;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_PREPROCESSOR_SENTENCE_EXECUTE_PRELIM_POS_TAGGER
	private: GIAnlpClass GIAnlp;
	private: GIAtranslatorDefineGrammarClass GIAtranslatorDefineGrammar;
	#endif
	private: GIAdatabaseClass GIAdatabase;
	#ifdef GIA_PREPROCESSOR_POS_TAGGER_GENERATE_DATABASE
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	#endif
	#ifdef GIA_PREPROCESSOR
	public: bool preprocessTextForGIAwrapper(const bool useLRP, string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables, bool* useInputTextPlainTXTFile, const string inputTextNLPfeatureXMLfileName);
		public: bool preprocessTextForGIA(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables, const string inputTextNLPfeatureXMLfileName);
		public: bool regenerateFileFromPreprocessedTextWithoutLRP(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, GIAtranslatorVariablesClass* translatorVariables);
			public: bool createPreprocessSentencesForGIA(const string inputFileName, GIAtranslatorVariablesClass* translatorVariables);
				public: bool createPreprocessSentences(const string fileContents, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, bool interpretNewLinesAsNewSentences, bool splitMultisentenceLines);
					#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_EXTRACT_INDENTATION
					private: bool extractIndentationFromCurrentLine(const string* lineContents, int* indexOfCurrentToken, int* currentIndentation, string* indentationContents);
					public: string generateIndentationContents(int currentIndentation);
					#endif
				private: void copySentenceContentsPreprocessor(GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents1FirstWord, GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents2firstWord);
			#ifdef GIA_PREPROCESSOR_SENTENCE
			#ifdef GIA_PREPROCESSOR_SENTENCE_EXECUTE_PRELIM_POS_TAGGER
			private: bool executePrelimFeatureProcessingOnSentences(const string outputLRPTextPlainTXTFileName, const string inputTextNLPfeatureXMLfileName, GIAtranslatorVariablesClass* translatorVariables);
			#endif
			private: bool preprocessSentencesForGIA(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string outputFileName, const string outputFileNameLRPforNLP);
				private: bool generateGIApreprocessorSentence(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, XMLparserTag* firstLogicReferenceClassTag);
				private: string removePrependingWhiteSpace(string sentenceContents);
				private: bool generatePreprocessorSentenceNLPparsablePhrases(GIApreprocessorLogicReference* firstLogicReferenceInList, int* sentenceIndex, string* outputTextContentsSentence, string* outputTextContentsSentenceForNLP);
					#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
					private: void generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference(string* sentenceContentsPreprocessedLogicReference, string* sentenceContentsPreprocessedLogicReferenceForNLP, GIApreprocessorLogicReference* firstLogicReferenceInList, int* sentenceIndex);
						private: void generateLogicReferenceSetContentsWithVariableNamesConjunction(string* logicReferenceSetContentsWithVariableNames, GIApreprocessorLogicReference* conjunctionLogicReference, int* sentenceIndex);
					#endif
					private: void addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(string* sentenceContentsPreprocessedLogicReferenceVariables, string* sentenceContentsPreprocessedLogicReferenceVariablesForNLP, GIApreprocessorSubReferenceSet* referenceSet, int* sentenceIndex, int referenceSetType);
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
			private: bool updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const bool isQuery);
				private: bool getGIApreprocessorSentence(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const int sentenceIndexOriginal, GIApreprocessorSentence** GIApreprocessorSentenceFound);
				private: bool getGIApreprocessorReferenceSet(GIApreprocessorLogicReference* firstGIApreprocessorLogicReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound);
					private: bool getGIApreprocessorReferenceSet(GIApreprocessorSubReferenceSet* firstGIApreprocessorSubReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound);
			#endif
			#endif
		
	#ifdef GIA_PREPROCESSOR_SENTENCE
	//these functions are executed after the GIA preprocessor and semantic relation parsing based on the preprocessed sentence objects (and are therefore stored outside of GIApreprocessor.cpp):
	public: bool connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesWrapper(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, GIAtranslatorVariablesClass* translatorVariables);	
		private: bool connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIApreprocessorLogicReference* firstLogicReferenceInList, GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* parentLogicReferenceEntity);

			#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
			private: void changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables);
			private: void changeSentenceIndexOfEntityNodesAndConnections(const int sentenceIndexOriginal, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables);
			#endif

			private: void connectRelationshipToTarget(GIAentityNode* relationship, GIAentityNode* targetEntity, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
			private: void connectRelationshipToSource(GIAentityNode* relationship, GIAentityNode* sourceEntity, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
			private: GIAentityNode* createNewRelationshipEntity(vector<GIApreprocessorWord*>* logicReferenceContents, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
				private: GIAentityNode* createNewRelationshipEntity(string relationshipEntityName, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
			private: GIAentityNode* createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIApreprocessorLogicReferenceVariable* logicReferenceVariable, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
				private: GIAentityNode* createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
				private: GIAentityNode* createNewRelationship(GIApreprocessorLogicReferenceVariable* logicReferenceVariable, GIAtranslatorVariablesClass* translatorVariables);
					private: GIAentityNode* createNewRelationship(GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables);
					#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
					private: bool updateRelationshipType(GIAentityNode* relationshipEntity, GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables);
					#else
					private: bool updateRelationshipTenseInformation(GIAentityNode* relationshipEntity, GIAentityNode* relationshipEntity, GIApreprocessorSubReferenceSet* delimiterEntityTempTenseInformation, GIAtranslatorVariablesClass* translatorVariables);
					#endif

			private: bool findFirstImplicitConjunctionLogicReferenceSetBefore(GIApreprocessorLogicReference* conjunctionLogicReferenceInList, GIApreprocessorLogicReference** firstImplicitConjunction);
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
			private: GIAentityNode* getEntity(GIApreprocessorLogicReference* currentLogicReferenceInList, GIAtranslatorVariablesClass* translatorVariable);
			#endif
			private: bool findPrimaryEntityAndReconcileSubReferenceSets(GIAentityNode** primaryEntityConnection, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, GIAentityNode* primaryDelimiterEntity, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType);
			private: void deleteExternalConnectionsToConnection(GIAentityConnection* connection);
				private: bool findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, const int referenceSetType, GIAentityNode** primaryEntity, GIAentityNode* primaryDelimiterEntity);
					private: bool findParentEntityBasedOnSentenceIndex(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, string parentEntityName, GIAentityNode** parentEntity);
					private: bool findSameSentenceRelationshipEntityNodePointerInVector(GIAentityNode* entityNode, const bool direction, GIAentityConnection** connectionFound, const int sentenceIndex);
					private: bool transferPropertyAndConditionConnectionsBetweenDummyDelimiterAndRealDelimiter(GIAentityNode* dummyDelimiterEntity, GIAentityNode* primaryDelimiterEntity);
					private: bool findSameSentenceSubjectObjectEntityNodePointerInVector(GIAentityNode* entityNode, const int connectionType, GIAentityConnection** connectionFound, const int sentenceIndex);
					private: bool findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, string entityNameToFind, GIAentityConnection** connectionFound, const int sentenceIndex);
	private: int generateConnectionTypeTargetToRelationship(GIAentityNode* relationship);
	private: int generateConnectionTypeSourceToRelationship(GIAentityNode* relationship);
	private: bool getRelationshipNameAndType(GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, int* relationshipEntityType, string* relationshipName);
	#endif
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	public: bool addSentenceToPreprocessorSentence(GIAtranslatorVariablesClass* translatorVariables);
		#ifdef GIA_PREPROCESSOR_SENTENCE
		public: bool getPreprocessorSentenceSubReferenceSet(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, int sentenceIndex, GIApreprocessorSubReferenceSet** subReferenceSetFound);
			public: bool getPreprocessorSentenceSubReferenceSet(GIApreprocessorLogicReference* firstLogicReferenceInList, int sentenceIndex, GIApreprocessorSubReferenceSet** subReferenceSetFound);
		#else
		public: bool getPreprocessorSentence(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, int sentenceIndex, GIApreprocessorSentence** sentenceFound);
		#endif
	public: bool addSentenceFeatureOutputToPreprocessorSentenceWordList(vector<GIApreprocessorWord*>* preprocessorSentenceWordList, GIAtranslatorVariablesClass* translatorVariables, bool originalNLPfeatures);
	public: bool addSentenceEntityOutputToPreprocessorSentenceWordList(vector<GIApreprocessorWord*>* preprocessorSentenceWordList, GIAtranslatorVariablesClass* translatorVariables);
	#endif
	
	#endif

};

#endif


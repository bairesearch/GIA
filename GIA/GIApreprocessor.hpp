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
 * Project Version: 3b2c 21-May-2017
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
#include "XMLparserClass.hpp"

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
	private: GIAdatabaseClass GIAdatabase;
	#ifdef GIA_PREPROCESSOR
	public: bool preprocessTextForGIA(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables);
		private: bool createPreprocessSentencesForGIA(const string inputFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInList);
			private: void preprocessorFillCurrentWord(GIApreprocessorMultiwordReductionPlainTextWord** currentWordInSentence, const string currentWord, const int entityIndex);
			private: void copySentenceContentsPreprocessor(GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents1FirstWord, GIApreprocessorMultiwordReductionPlainTextWord* sentenceContents2firstWord);
		#ifdef GIA_PREPROCESSOR_SENTENCE
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
		private: bool updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const bool isQuery, const string outputFileName);
			private: bool getGIApreprocessorSentence(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const int sentenceIndexOriginal, GIApreprocessorSentence** GIApreprocessorSentenceFound);
			private: bool getGIApreprocessorReferenceSet(GIApreprocessorLogicReference* firstGIApreprocessorLogicReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound);
				private: bool getGIApreprocessorReferenceSet(GIApreprocessorSubReferenceSet* firstGIApreprocessorSubReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound);
			private: bool replaceEntityTagWithNLPonlyTag(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo, GIApreprocessorSentence* firstGIApreprocessorSentenceInList);	
		#endif
		#endif
		
	#ifdef GIA_PREPROCESSOR_SENTENCE
	//these functions are executed after the GIA preprocessor and semantic relation parsing based on the preprocessed sentence objects (and are therefore stored outside of GIApreprocessor.cpp):
	public: bool connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesWrapper(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, GIAtranslatorVariablesClass* translatorVariables);	
		private: bool connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIApreprocessorLogicReference* firstLogicReferenceInList, GIAtranslatorVariablesClass* translatorVariables, GIAentityNode* parentLogicReferenceEntity);

			#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
			void changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables);
			void changeSentenceIndexOfEntityNodesAndConnections(const int sentenceIndexOriginal, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables);
			#endif

			void connectRelationshipToTarget(GIAentityNode* relationship, GIAentityNode* targetEntity, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
			void connectRelationshipToSource(GIAentityNode* relationship, GIAentityNode* sourceEntity, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
			GIAentityNode* createNewRelationshipEntity(vector<GIApreprocessorWord*>* logicReferenceContents, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
				GIAentityNode* createNewRelationshipEntity(string relationshipEntityName, const int relationshipEntityType, GIAtranslatorVariablesClass* translatorVariables);
			GIAentityNode* createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIApreprocessorLogicReferenceVariable* logicReferenceVariable, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
				GIAentityNode* createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, const bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
				GIAentityNode* createNewRelationship(GIApreprocessorLogicReferenceVariable* logicReferenceVariable, GIAtranslatorVariablesClass* translatorVariables);
					GIAentityNode* createNewRelationship(GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables);
					#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_RECORD_SAME_REFERENCE_SET_DELIMITERS_UPDATE_ENTITY_OLD_METHOD
					bool updateRelationshipType(GIAentityNode* relationshipEntity, GIApreprocessorSubReferenceSet* relationshipReference, GIApreprocessorSubReferenceSet* relationshipObject, GIAtranslatorVariablesClass* translatorVariables);
					#else
					bool updateRelationshipTenseInformation(GIAentityNode* relationshipEntity, GIAentityNode* relationshipEntity, GIApreprocessorSubReferenceSet* delimiterEntityTempTenseInformation, GIAtranslatorVariablesClass* translatorVariables);
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
	
	#endif

	



};

#endif


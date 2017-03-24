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
 * Project Version: 3a2c 21-March-2017
 * Requirements: requires plain text file
 * Description: Logical Condition and Reference Set preprocessor
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR
#define HEADER_GIA_PREPROCESSOR

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"
#include "GIApreprocessorLogicReference.hpp"
#include "GIApreprocessorReferenceSet.hpp"
#include "GIAtranslatorOperations.hpp"	//required for findSameSentenceEntityNodePointerInVector
#include "XMLparserClass.hpp"

class GIApreprocessorClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIApreprocessorSentenceClassClass GIApreprocessorSentenceClass;
	private: GIApreprocessorLogicReferenceClass GIApreprocessorLogicReferenceObject;
	private: GIApreprocessorReferenceSetClass GIApreprocessorReferenceSet;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;
	#ifdef GIA_PREPROCESSOR
	public: bool preprocessTextForGIA(string* inputTextPlainTXTfileName, const string outputLRPTextPlainTXTFileName, const bool isQuery, GIAtranslatorVariablesClass* translatorVariables);
	
		#ifdef GIA_PREPROCESSOR_SENTENCE
		private: bool preprocessSentencesForGIA(const string inputFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const string outputFileName, const string outputFileNameLRPforNLP);
			bool generateGIApreprocessorSentence(const string* sentenceContentsOriginal, GIApreprocessorSentence** currentGIApreprocessorSentenceInList, int sentenceIndexOriginal, XMLparserTag* firstLogicReferenceClassTag);
			private: string removePrependingWhiteSpace(string sentenceContents);
			private: bool generatePreprocessorSentenceNLPparsablePhrases(GIApreprocessorLogicReference* firstLogicReferenceInList, int* sentenceIndex, string* outputTextContentsSentence, string* outputTextContentsSentenceForNLP);
				#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
				private: void generateLogicReferenceSetContentsWithVariableNamesAndAddToSentenceContentsPreprocessedLogicReference(string* sentenceContentsPreprocessedLogicReference, GIApreprocessorLogicReference* firstLogicReferenceInList, int* sentenceIndex);
				#endif
				private: void addSentenceToSentenceContentsPreprocessedLogicReferenceVariables(string* sentenceContentsPreprocessedLogicReferenceVariables, string* sentenceContentsPreprocessedLogicReferenceVariablesForNLP, GIApreprocessorSubReferenceSet* referenceSet, int* sentenceIndex, int referenceSetType);
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
		private: bool updateGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const string inputFileName, GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const bool isQuery, const string outputFileName);
			private: bool getGIApreprocessorSentence(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, const int sentenceIndexOriginal, GIApreprocessorSentence** GIApreprocessorSentenceFound);
			private: bool getGIApreprocessorReferenceSet(GIApreprocessorLogicReference* firstGIApreprocessorLogicReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound);
				private: bool getGIApreprocessorReferenceSet(GIApreprocessorSubReferenceSet* firstGIApreprocessorSubReferenceInList, const int entityIndexOriginal, GIApreprocessorSubReferenceSet** GIApreprocessorSubReferenceSetFound);
			private: bool replaceEntityTagWithNLPonlyTag(GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* currentLRPtoLRPforNLPonlyTagNameAndLocationCorrespondenceInfo, GIApreprocessorMultiwordReductiontag* firstTagInPlainText);	
		#endif
		#endif
		
	#ifdef GIA_PREPROCESSOR_SENTENCE
	//these functions are executed after the GIA preprocessor and semantic relation parsing based on the preprocessed sentence objects (and are therefore stored outside of GIApreprocessor.cpp):
	public: bool connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntitiesWrapper(GIApreprocessorSentence* firstGIApreprocessorSentenceInList, GIAtranslatorVariablesClass* translatorVariables);	
		private: bool connectPreprocessorSentenceReferenceSetEntitiesToLogicReferenceEntities(GIApreprocessorSentence* currentGIApreprocessorSentenceInList, GIApreprocessorLogicReference* firstLogicReferenceInList, GIAtranslatorVariablesClass* translatorVariables);

			#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
			void changeSentenceIndexOfEntityNodesAndConnectionsForReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables);
			void changeSentenceIndexOfEntityNodesAndConnections(const int sentenceIndexOriginal, const int sentenceIndexNew, GIAtranslatorVariablesClass* translatorVariables);
			#endif

			void connectRelationshipToTarget(GIAentityNode* relationship, GIAentityNode* targetEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
			void connectRelationshipToSource(GIAentityNode* relationship, GIAentityNode* sourceEntity, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);
			GIAentityNode* createNewRelationshipEntity(string relationshipEntityName, GIAtranslatorVariablesClass* translatorVariables);
			GIAentityNode* createNewRelationshipAndConnectToSource(GIAentityNode* sourceEntity, GIApreprocessorLogicReferenceVariable* logicReferenceVariable, bool sameReferenceSet, GIAtranslatorVariablesClass* translatorVariables);	//OLD: this will create an intermediary action/possession("have") node if necessary
			GIAentityNode* createNewRelationship(GIApreprocessorLogicReferenceVariable* logicReferenceVariable, GIAtranslatorVariablesClass* translatorVariables);
	
			private: bool findFirstImplicitConjunctionLogicReferenceSetBefore(GIApreprocessorLogicReference* conjunctionLogicReferenceInList, GIApreprocessorLogicReference** firstImplicitConjunction);
			#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
			private: GIAentityNode* getEntity(GIApreprocessorLogicReference* currentLogicReferenceInList, map<int, vector<GIAentityNode*>*>* entityNodesActiveListSentences);
			#endif
			private: bool findPrimaryEntityAndReconcileSubReferenceSets(GIAentityNode** primaryEntityConnection, GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, int referenceSetType);
			private: void deleteExternalConnectionsToConnection(GIAentityConnection* connection);
				private: bool findPrimaryEntityBasedOnSentenceIndexAndTypeAndDeleteDummyVariableConnections(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, int referenceSetType, GIAentityNode** primaryEntity);
					private: bool findParentEntityBasedOnSentenceIndex(GIApreprocessorSubReferenceSet* currentSubReferenceSetInList, GIAtranslatorVariablesClass* translatorVariables, string parentEntityName, GIAentityNode** parentEntity);
					private: bool findSameSentenceRelationshipEntityNodePointerInVector(GIAentityNode* entityNode, const bool direction, GIAentityConnection** connectionFound, const int sentenceIndex);
					private: bool findSameSentenceSubjectObjectEntityNodePointerInVector(GIAentityNode* entityNode, const int connectionType, GIAentityConnection** connectionFound, const int sentenceIndex);
					private: bool findSameSentenceEntityNodePointerInVector(GIAentityNode* entityNode, string entityNameToFind, GIAentityConnection** connectionFound, const int sentenceIndex);
	private: int generateConnectionTypeTargetToRelationship(GIAentityNode* relationship);
	private: int generateConnectionTypeSourceToRelationship(GIAentityNode* relationship);
	private: bool getRelationshipNameAndType(GIApreprocessorLogicReferenceVariable* logicReferenceVariable, int* relationshipEntityType, string* relationshipName);
	#endif
	
	#endif

	



};

#endif


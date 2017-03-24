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
 * File Name: GIApreprocessorLogicReference.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3a1u 26-February-2017
 * Requirements: requires plain text file
 * Description: Logic Reference preprocessor
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_LOGIC_REFERENCE
#define HEADER_GIA_PREPROCESSOR_LOGIC_REFERENCE

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorReferenceSet.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"	//required for isIntrawordPunctuationMark only
#include "XMLparserClass.hpp"

class GIApreprocessorLogicReferenceClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	private: GIApreprocessorSentenceClassClass GIApreprocessorSentenceClass;
	private: GIApreprocessorReferenceSetClass GIApreprocessorReferenceSet;

	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	public: bool extractGIApreprocessorLogicReferenceClasses(XMLparserTag** firstLogicReferenceClassTag, vector<string>* logicReferenceInstanceList);
	public: bool executeLogicReferencePreprocessor(const string* sentenceContents, GIApreprocessorSentence* currentGIApreprocessorSentenceInList, XMLparserTag* firstLogicReferenceClassTag);
		private: void addSentenceToText(string* sentenceContentsPreprocessed, GIApreprocessorSubReferenceSet* referenceSet, int* sentenceIndex);
		private: bool initialiseNewLogicReference(GIApreprocessorLogicReference* currentLogicReferenceInList, const string logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence, string* currentContents);
		private: bool initialiseNewSubLogicReference(GIApreprocessorLogicReference** currentLogicReferenceInList, const string logicReferenceContents, const int logicReferenceClass, const string logicReferenceClassType, const int wordIndexSentence);
		private: bool fillLogicReferenceVariableAndAddLogicReferenceToList(GIApreprocessorLogicReference** currentLogicReferenceInList, string* currentContents, vector<string>* logicReferenceVariableWordList, int* logicReferenceVariableNameIndex, bool commaDetected, bool* expectToFindSubjectAuxObjectLogicReferenceVariable);
		private: bool isClassTag(string word, string classNameToFind, string* classTypeNameFound, XMLparserTag* firstLogicReferenceClassTag);
		private: bool identifyImplicitLogicReferenceSets(GIApreprocessorLogicReference* firstLogicReferenceInList);
			private: bool identifyImplicitLogicReferenceSetBefore(GIApreprocessorLogicReference* logicReferenceInList, const int logicReferenceClass, const string logicReferenceClassType);
			private: bool identifyImplicitLogicReferenceSetAfter(GIApreprocessorLogicReference* logicReferenceInList, const int logicReferenceClass, const string logicReferenceClassType);
			private: bool identifyImplicitConjunctionLogicReferenceSetsBefore(GIApreprocessorLogicReference* conjunctionLogicReferenceInList, const string logicReferenceClassType);
	private: string generateLogicReferenceContents(string logicReferenceClassType);
	public: bool islogicReferenceConjunctionAndOr(GIApreprocessorLogicReference* currentLogicReferenceInList);
	#endif	
};

#endif


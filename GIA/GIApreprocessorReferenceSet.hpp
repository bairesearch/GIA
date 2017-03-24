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
 * File Name: GIApreprocessorReferenceSet.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3a1l 26-February-2017
 * Requirements: requires plain text file
 * Description: Reference Set preprocessor
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_REFERENCE_SET
#define HEADER_GIA_PREPROCESSOR_REFERENCE_SET

#include "GIAglobalDefs.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIApreprocessorMultiwordReduction.hpp"

class GIApreprocessorReferenceSetClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorSentenceClassClass GIApreprocessorSentenceClass;
	private: GIApreprocessorMultiwordReductionClass GIApreprocessorMultiwordReduction;
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	public: bool executeReferenceSetPreprocessor(string* logicReferenceVariableContents, vector<string>* logicReferenceVariableWordList, GIApreprocessorLogicReferenceVariable* logicReferenceVariable, bool expectToFindSubjectAuxObjectLogicReferenceVariable, int wordIndexSentence);
			public: bool detectAuxiliary(const string currentWord);
			private: bool formSubReferenceSetTextFromWordList(vector<string>* logicReferenceVariableWordList, string* subReferenceSetText, int firstIndexOfSubReferenceSet, int lastIndexOfSubReferenceSet);
			private: bool addSubReferenceSetToReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, string subReferenceSetText, bool referenceSetDelimiter, int wordIndexLogicReference, int wordIndexSentence);
			private: bool addReferenceSetToReferenceSet(GIApreprocessorSubReferenceSet* firstSubReferenceSetInList, string subReferenceSetText, bool referenceSetDelimiter, int wordIndexLogicReference, int wordIndexSentence);
	public: bool hasReferenceSet(GIApreprocessorSubReferenceSet* referenceSet);
	public: void setReferenceSetText(GIApreprocessorSubReferenceSet* referenceSet, string text);
	public: string getReferenceSetText(GIApreprocessorSubReferenceSet* referenceSet);
	#endif
};

#endif


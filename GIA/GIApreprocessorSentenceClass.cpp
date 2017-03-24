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
 * File Name: GIApreprocessorSentenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3a1c 26-February-2017
 * Requirements: requires plain text file
 * Description: Logical Condition and Reference Set preprocessor
 *
 *******************************************************************************/


#include "GIApreprocessorSentenceClass.hpp"
#include "GIAentityNodeClass.hpp"
#include "GIAentityConnectionClass.hpp"


GIApreprocessorSubReferenceSet::GIApreprocessorSubReferenceSet(void)
{
	sentenceIndex = INT_DEFAULT_VALUE;
	subReferenceSetContents = "";
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
	subReferenceSetContentsOutputForNLP = "";
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	firstIndexOfReferenceSetText = INT_DEFAULT_VALUE;
	lastIndexOfReferenceSetText = INT_DEFAULT_VALUE;
	#endif

	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	primaryEntityTemp = NULL;
	#endif
	
	isReferenceSetDelimiter = false;	
	next = NULL;
}
GIApreprocessorSubReferenceSet::~GIApreprocessorSubReferenceSet(void)
{
}

GIApreprocessorLogicReferenceVariable::GIApreprocessorLogicReferenceVariable(void)
{
	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
	sentenceIndex = INT_DEFAULT_VALUE;
	#endif
	
	logicReferenceVariableContents = "";
	logicReferenceVariableNameIndex = INT_DEFAULT_VALUE;
	logicReferenceVariableName = "";
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	referenceSetSubject = new GIApreprocessorSubReferenceSet();;
	referenceSetObject = new GIApreprocessorSubReferenceSet();
	referenceSetDelimiter = new GIApreprocessorSubReferenceSet();
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	wordIndexSentence = INT_DEFAULT_VALUE;
	#endif
}
GIApreprocessorLogicReferenceVariable::~GIApreprocessorLogicReferenceVariable(void)
{
}

GIApreprocessorLogicReference::GIApreprocessorLogicReference(void)
{
	logicReferenceContents = "";
	logicReferenceClass = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED;		//if necessary (ie is set to GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_UNKNOWN) this will be replaced with GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and/GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or upon the detection of a subsequent and/or
	logicReferenceClassType = GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown;
	logicReferenceVariable = new GIApreprocessorLogicReferenceVariable();
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_RECURSION
	firstSubLogicReferenceInList = NULL;
	lastLogicReferenceInUpperLevel = NULL;
	hasSubLogicReference = false;
	isSubLogicReference = false;
	#endif
		
	next = NULL;
	previous = NULL;
}
GIApreprocessorLogicReference::~GIApreprocessorLogicReference(void)
{
}

GIApreprocessorSentence::GIApreprocessorSentence(void)
{
	sentenceContentsOriginal = "";
	sentenceIndexOriginal = 0;	
	
	hasLogicReference = false;
	firstLogicReferenceInList = new GIApreprocessorLogicReference();
	logicReferenceTotal = 0;
	
	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
	sentenceIndex = INT_DEFAULT_VALUE;
	#endif
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
	logicReferenceSetContentsWithVariableNames = "";
	logicReferenceSetContentsWithVariableNamesEntityIndex = INT_DEFAULT_VALUE;
	#endif
	
	next = NULL;
}
GIApreprocessorSentence::~GIApreprocessorSentence(void)
{
}







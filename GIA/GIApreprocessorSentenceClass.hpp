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
 * File Name: GIApreprocessorSentenceClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3a1a 26-February-2017
 * Requirements: requires plain text file
 * Description: Logical Condition and Reference Set preprocessor
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_SENTENCE_CLASS
#define HEADER_GIA_PREPROCESSOR_SENTENCE_CLASS

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "GIAentityNodeClass.hpp"	//required for primaryEntityTemp

#define GIA_PREPROCESSOR_XML_TAG_preprocessor ((string)"preprocessor")
	#define GIA_PREPROCESSOR_XML_TAG_logicReference ((string)"logicReference")
	#define GIA_PREPROCESSOR_XML_TAG_referenceSet ((string)"referenceSet")
		#define GIA_PREPROCESSOR_XML_TAG_class ((string)"class")
			#define GIA_PREPROCESSOR_XML_TAG_type ((string)"type")
				#define GIA_PREPROCESSOR_XML_TAG_instance ((string)"instance")
		#define GIA_PREPROCESSOR_XML_ATTRIBUTE_name ((string)"name")
		
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED (0)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION (1)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB (2)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION (3)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_undefined "undefined"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_conjunction "conjunction"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_verb "verb"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_preposition "preposition"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_NUM (4)
static string GIApreprocessorLogicReferenceClasses[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_NUM] = {GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_undefined, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_conjunction, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_verb, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_preposition};
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT (11)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_COMPLEMENT (13)

/*
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_UNKNOWN (0)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_IMPERATIVE (1)	//eg Ride the bike
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_STATEMENT (2)	//eg Tom rides the bike (standard subject/object)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_AND (3)		//eg and Tom rides the bike
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_OR (4)		//or Tom rides the bike
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_CONCLUSION (5)	//eg therefore Tom rides the bike
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_LOGICALCONDITION (6)	//eg therefore Tom rides the bike
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB_TYPE_PROPOSITION (7)	//eg Tom suggested that Tom rides the bike
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_REGARDING (8)	//eg Considering X, Tom rides the bike
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_STANCE (9)	//eg Unlike X, Tom rides the bike
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_QUALIFIER (10)	//eg Tom rides the bike, except on Thursday
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_TIME (11)	//eg Tom rides the bike after 12pm (need to reconcile with standard GIA conditions (semantic relation); these currently conflict)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_MEANSOFACHIEVINGACTION (12)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_SIMILAR (13)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_PURPOSE (14)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_WITH (15)
*/
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown "unknown"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_imperative "imperative"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_statement "statement"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and "and"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or "or"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_conclusion "conclusion"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_logicalCondition "logicalCondition"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB_TYPE_proposition "proposition"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_regarding "regarding"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_stance "stance"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_qualifier "qualifier"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_time "time"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_meansOfAchievingAction "means of achieving action"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_similar "similar"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_purpose "purpose"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_with "with"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_NUM (4)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_NUM (4)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB_TYPE_NUM (1)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_NUM (8)
#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_XML_ERROR_CHECKING
static string GIApreprocessorLogicReferenceClassUndefinedTypes[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_NUM] = {GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_imperative, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_statement};
static string GIApreprocessorLogicReferenceClassConjunctionTypes[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_NUM] = {GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_conclusion, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_logicalCondition};
static string GIApreprocessorLogicReferenceClassVerbTypes[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB_TYPE_NUM] = {GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB_TYPE_proposition};
static string GIApreprocessorLogicReferenceClassPrepositionTypes[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_NUM] = {GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_regarding, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_stance, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_qualifier, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_time, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_meansOfAchievingAction, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_similar, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_purpose, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_with};
#endif

#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_VARIABLE_NAMES_NUM (26)	//max 26 logical condition varibables per sentence; this is a magic number and could be increased
static string GIApreprocessorLogicReferenceVariableNames[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_VARIABLE_NAMES_NUM] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};

#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_UNKNOWN (0)
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT (1)
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT (2)
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_DELIMITER (3)	//this is a virtual (fake) reference set created for data storage consistency purposes only


class GIApreprocessorSubReferenceSet
{
public:
	GIApreprocessorSubReferenceSet(void);
	~GIApreprocessorSubReferenceSet(void);

	int sentenceIndex;		//if GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES this is a temporary sentence index
	string subReferenceSetContents;
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
	string subReferenceSetContentsOutputForNLP;
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	bool firstIndexOfReferenceSetText;
	bool lastIndexOfReferenceSetText;
	#endif
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	GIAentityNode* primaryEntityTemp;	//required for conjunction post processing
	#endif
	
	bool isReferenceSetDelimiter;
	GIApreprocessorSubReferenceSet* next;
};

class GIApreprocessorLogicReferenceVariable
{
public:
	GIApreprocessorLogicReferenceVariable(void);
	~GIApreprocessorLogicReferenceVariable(void);

	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
	int sentenceIndex;
	#endif
	
	string logicReferenceVariableContents;
	int logicReferenceVariableNameIndex;
	string logicReferenceVariableName;	//replacement variable name for logical condition variable
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	GIApreprocessorSubReferenceSet* referenceSetSubject;	//referenceSetSubject
	GIApreprocessorSubReferenceSet* referenceSetObject;	//referenceSetObject
	GIApreprocessorSubReferenceSet* referenceSetDelimiter;	//NB if !GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITERS, only the referenceSetContents string is used of the referenceSetDelimiter
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	int wordIndexSentence;
	#endif
};

class GIApreprocessorLogicReference
{
public:
	GIApreprocessorLogicReference(void);
	~GIApreprocessorLogicReference(void);

	string logicReferenceContents;
	int logicReferenceClass;
	string logicReferenceClassType;
	GIApreprocessorLogicReferenceVariable* logicReferenceVariable;
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_RECURSION
	GIApreprocessorLogicReference* firstSubLogicReferenceInList;
	GIApreprocessorLogicReference* lastLogicReferenceInUpperLevel;	//need to work out how to identify in when returning to an upper level logical condition layer (is this even done in English?); eg Tom said that Mary said A is B, but Jerry said otherwise.
	bool hasSubLogicReference;
	bool isSubLogicReference;
	#endif	
	
	GIApreprocessorLogicReference* next;
	GIApreprocessorLogicReference* previous;
};

class GIApreprocessorSentence
{
public:
	GIApreprocessorSentence(void);
	~GIApreprocessorSentence(void);

	string sentenceContentsOriginal;
	int sentenceIndexOriginal;
	
	bool hasLogicReference;	//if false, then firstLogicReferenceInList will only have a single GIApreprocessorLogicReference
	GIApreprocessorLogicReference* firstLogicReferenceInList;
	int logicReferenceTotal;

	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
	int sentenceIndex;
	#endif
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE
	string logicReferenceSetContentsWithVariableNames;	//only valid for first GIApreprocessorLogicReference in layer
	int logicReferenceSetContentsWithVariableNamesEntityIndex;
	#endif
	
	GIApreprocessorSentence* next;
};


class GIApreprocessorSentenceClassClass
{
	
};



#endif

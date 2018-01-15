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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3e9b 10-January-2018
 * Requirements: requires plain text file
 * Description: Logical Condition and Reference Set preprocessor
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_SENTENCE_CLASS
#define HEADER_GIA_PREPROCESSOR_SENTENCE_CLASS

#include "GIAglobalDefs.hpp"
#include "SHAREDvars.hpp"
#include "GIAentityNodeClass.hpp"	//required for primaryEntityTemp, GIA_PREPROCESSOR_RECORD_REFERENCES
#include "GIAsentenceClass.hpp"	//required for GIA_PREPROCESSOR_RECORD_REFERENCES
#include "GIApreprocessorMultiwordReductionClass.hpp" 


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

#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_INFERRED (11)	//used to demarkate components of conjunction logic references 
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_IMPLICIT_CONFIRMED (12)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPONENT_EXPLICIT (13)
/*
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_COMPLEMENT (11)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_COMPLEMENT (13)
*/

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
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_PURPOSE (13)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_WITH (14)
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
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_purpose "purpose"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_with "with"
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_NUM (4)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_NUM (4)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB_TYPE_NUM (1)
#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_NUM (7)
static string GIApreprocessorLogicReferenceClassUndefinedTypes[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_NUM] = {GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_unknown, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_imperative, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_UNDEFINED_TYPE_statement};
static string GIApreprocessorLogicReferenceClassConjunctionTypes[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_NUM] = {GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_and, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_or, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_conclusion, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_CONJUNCTION_TYPE_logicalCondition};
static string GIApreprocessorLogicReferenceClassVerbTypes[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB_TYPE_NUM] = {GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_VERB_TYPE_proposition};
static string GIApreprocessorLogicReferenceClassPrepositionTypes[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_NUM] = {GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_regarding, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_stance, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_qualifier, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_time, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_meansOfAchievingAction, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_purpose, GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_CLASS_PREPOSITION_TYPE_with};

#define GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_VARIABLE_NAMES_NUM (26)	//max 26 logical condition varibables per sentence; this is a magic number and could be increased
static string GIApreprocessorLogicReferenceVariableNames[GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_VARIABLE_NAMES_NUM] = {"Ax", "Bx", "Cx", "Dx", "Ex", "Fx", "Gx", "Hx", "Ix", "Jx", "Kx", "Lx", "Mx", "Nx", "Ox", "Px", "Qx", "Rx", "Sx", "Tx", "Ux", "Vx", "Wx", "Xx", "Yx", "Zx"};	//minimum 2 characters is currently required for a referenceSet/logicReference subject/object (FUTURE: fix this)

#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_UNKNOWN (0)
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_SUBJECT (1)
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_OBJECT (2)
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_TYPE_DELIMITER (3)	//this is a virtual (fake) reference set created for data storage consistency purposes only


#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_UNDEFINED (0)
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_AUXILIARY (1)	//eg tom has/is the ball
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_VERB (2)	//eg tom moves the ball
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_TYPE_PREPOSITION (3) 	//eg tom is near the ball 

#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_UNDEFINED (0)
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_AND_OBJECT_REFER_TO_PREVIOUS_DELIMITER_VERB (1)	//eg tom rides the bike near the ball
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_OBJECT_REFERS_TO_PREVIOUS_DELIMITER_VERB (2) 	//eg Tom rides fast
#define GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_DELIMITER_SPECIAL_CASE_DELIMITER_VERB_NOT_PRECEEDED_BY_THAT_WHICH_AND_NOT_PRECEEDED_BY_AUXILIARY (3) 	//eg "controlled" in "a controlled chicken was moved to"

#define BOOL_NA (false)



#ifdef GIA_PREPROCESSOR_SENTENCE

class GIApreprocessorSubReferenceSet
{
public:
	GIApreprocessorSubReferenceSet(void);
	~GIApreprocessorSubReferenceSet(void);

	int sentenceIndex;		//if GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES this is a temporary sentence index
	vector<GIApreprocessorWord*> subReferenceSetContents;
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET_ADD_DUMMY_NLP_TEXT
	vector<GIApreprocessorWord*> subReferenceSetContentsOutputForNLP;
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	int firstIndexOfReferenceSetText;
	int lastIndexOfReferenceSetText;
	int dummyNLPtestOffset;
	#endif
		
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	GIAentityNode* primaryEntityTemp;	//required for conjunction post processing
	#endif
	
	bool isReferenceSetDelimiter;
	GIApreprocessorSubReferenceSet* next;
	
	//for optimisation purposes;
	int delimiterType;
	int delimiterSpecialCase;
	
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	GIAsentence* sentenceReference;
	#endif
	
	#ifdef GIA_NEURAL_NETWORK_NON_SEMANTIC
	int definite;
	#endif
	
	bool isFirstSubreferenceSetInList;
};

class GIApreprocessorLogicReferenceVariable
{
public:
	GIApreprocessorLogicReferenceVariable(void);
	~GIApreprocessorLogicReferenceVariable(void);

	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_LOGIC_REFERENCE_VARIABLES
	int sentenceIndex;
	#endif
	
	vector<GIApreprocessorWord*> logicReferenceVariableContents;
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

	vector<GIApreprocessorWord*> logicReferenceContents;	   //similiar if not identical to GIApreprocessorLogicReferenceTypes[logicReferenceClassType];
	int logicReferenceClass;
	string logicReferenceClassType;
	GIApreprocessorLogicReferenceVariable* logicReferenceVariable;
	
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE
	GIApreprocessorLogicReference* firstSubLogicReferenceInListGovernor;	//for verbs/propositions, eg said/considering(A, B)
	GIApreprocessorLogicReference* firstSubLogicReferenceInListDependent;	//for verbs/propositions, eg said/considering(A, B)
	GIApreprocessorLogicReference* firstSubLogicReferenceInListArray;	//for conjunctions eg and(A, B, C)
	GIApreprocessorLogicReference* lastLogicReferenceInUpperLevel;	//need to work out how to identify in when returning to an upper level logical condition layer (is this even done in English?); eg Tom said that Mary said A is B, but Jerry said otherwise.	but(said(Tom, said(Mary, A is B)), said(Jerry, otherwise))
	GIApreprocessorLogicReference* next;		//for isSubLogicReferenceArray only; conjunctions (and, or, but), eg and(said(A, B), said(A, C), said(A, D))
	GIApreprocessorLogicReference* previous;	//for isSubLogicReferenceArray only; conjunctions (and, or, but), eg and(said(A, B), said(A, C), said(A, D))
	bool hasSubLogicReference;
	bool hasSubLogicReferenceArray;
	bool isSubLogicReferenceGovernor;
	bool isSubLogicReferenceDependent;
	bool isSubLogicReferenceArray;
	#ifdef GIA_PREPROCESSOR_SENTENCE_LOGIC_REFERENCE_OUTPUT_LOGIC_REFERENCE_SETS_FOR_HIGH_LEVEL_SEMANTIC_PARSE_VERBOSE
	vector<GIApreprocessorWord*> logicReferenceSetContentsWithVariableNames;
	int logicReferenceSetContentsWithVariableNamesSentenceIndex;
	#endif
	#endif
};

#endif

class GIApreprocessorSentence
{
public:
	GIApreprocessorSentence(void);
	~GIApreprocessorSentence(void);

	int sentenceIndexOriginal;
	string sentenceContentsOriginalText;
	//#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	vector<GIApreprocessorWord*> sentenceContentsOriginal;
	vector<GIApreprocessorWord*> sentenceContentsLRP;
	//#endif
	
	#ifdef GIA_PREPROCESSOR_SENTENCE
	bool hasLogicReference;	//if false, then firstLogicReferenceInList will only have a single GIApreprocessorLogicReference
	GIApreprocessorLogicReference* firstLogicReferenceInList;
	//int logicReferenceTotal;
	#ifdef GIA_PREPROCESSOR_ASSIGN_UNIQUE_SENTENCE_INDICES_FOR_SENTENCES
	int sentenceIndex;
	#endif
	#ifdef GIA_PREPROCESSOR_SENTENCE_EXECUTE_PRELIM_POS_TAGGER
	GIAsentence* sentenceReferencePrelim;		//temporary GIA sentence data (POS tags) required for execution of GIA preprocessor
	#endif	
	#else
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	GIAsentence* sentenceReference;
	#endif	
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_EXTRACT_INDENTATION
	int indentation;
	#endif
	
	GIApreprocessorSentence* next;
};

class GIApreprocessorSentenceClassClass
{
	
};



#endif

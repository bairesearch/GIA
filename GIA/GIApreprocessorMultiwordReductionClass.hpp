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
 * File Name: GIApreprocessorMultiwordReductionClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Natural Language Compiler (Programming Interface)
 * Project Version: 3d5f 11-August-2017
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_MULTIWORD_REDUCTION_CLASS
#define HEADER_GIA_PREPROCESSOR_MULTIWORD_REDUCTION_CLASS

#include "GIAglobalDefs.hpp"
#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
#include "GIAentityNodeClass.hpp"
#include "GIAsentenceClass.hpp"
#endif
#include "SHAREDvars.hpp"

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_CHARACTERS_NUMBER_OF_TYPES (65)	//must sync with NLC_PREPROCESSOR_MATH_NLP_PARSABLE_PHRASE_CHARACTERS_NUMBER_OF_TYPES
static char GIApreprocessorMultiwordReductionNLPparsableCharacters[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_CHARACTERS_NUMBER_OF_TYPES] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_', '.','\''};	//must sync with preprocessorMathNLPparsableCharacters
#endif

//#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_USE_OLD_PREPOSITION_SPECIFIC_CODE_NOT_REQUIRED

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_FILE_NAME "CambridgePhrasalVerbs.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_IRREGULARVERB_DATABASE_FILE_NAME "WikipediaIrregularVerbs.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PREPOSITION_DATABASE_FILE_NAME "WikipediaEnglishClubPrepositions.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_FILE_NAME "WordnetVerbs.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_ADVERB_DATABASE_FILE_NAME "WordnetAdverbs.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_ADJECTIVE_DATABASE_FILE_NAME "WordnetAdjectives.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NOUN_DATABASE_FILE_NAME "WordnetNouns.txt"

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_PREPOSITION_DATABASE_FILE_NAME "WikipediaEnglishClubMultiwordPrepositions.txt"
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REDUCE_ALL_WORD_TYPES
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_VERB_DATABASE_FILE_NAME "WordnetMultiwordVerbs.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_ADVERB_DATABASE_FILE_NAME "WordnetMultiwordAdverbs.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_ADJECTIVE_DATABASE_FILE_NAME "WordnetMultiwordAdjectives.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_MULTIWORD_NOUN_DATABASE_FILE_NAME "WordnetMultiwordNouns.txt"
#endif

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NOUN_DATABASE_TAG_BASE_FORM_SINGULAR (0)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NOUN_DATABASE_TAG_BASE_FORM_PLURAL (1)

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_OR "or"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMETHING_NAME "sth"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEWHERE_NAME "swh"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NAME "sb"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMETHING (0)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEWHERE (1)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY (2)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_UNDEFINED (GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMETHING)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_MAX_NUMBER_WORDS (3)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ALTERNATE (CHAR_FORWARDSLASH)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_OPEN_OPTIONAL (CHAR_OPEN_BRACKET)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_CLOSE_OPTIONAL (CHAR_CLOSE_BRACKET)

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS (5)	//run (infinitive), runs (present), running (continuous), ran (past), run (past partiple)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE (0)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT (1)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS (2)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST (3)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE (4)
#ifdef GIA_PREPROCESSOR_DETERMINE_VERB_CASE_ADDITIONAL_EXTENDED
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION (5)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL (6)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE (7)
#endif
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS (2)	//standard/alternate (ie double constanant form OR irregular verb alternate case)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD (0)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE (1)

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND "s"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND "ing"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND "ed"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND_CASE3 "ies"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3 "ied"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND_LENGTH (1)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND_LENGTH (3)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_LENGTH (2)
#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND "able"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND_CASE3 "iable"
#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND "ive"		//0 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1II "itive"	//0 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1III "ative"	//0 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2 "itive"	//last 1 characters replaced (e)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II "mentive"	//last 1 characters replaced (e)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE3 "imentive"	//last 1 characters replaced (y)
#endif
#endif
#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND "ment"		//0 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1II "ament"	//0 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1III "lment"	//0 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE2 "ment"	//last 1 characters replaced (e)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE3 "iment"	//last 1 characters replaced (y)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE5 "pment"	//last 2 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND "ion"		//0 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1II "ition"	//0 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1III "ation"	//0 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2 "ion"	//last 1 characters replaced (e)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2II "ition"	//last 1 characters replaced (e)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2III "ation"	//last 1 characters replaced (e)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE3 "ication"	//last 1 characters replaced (y)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6I "amation"	//last 3 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6II "ision"	//last 3 characters replaced
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6III "ition"	//last 3 characters replaced
#endif

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_INVERSE_PREPOSITIONS
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_NEXTCOLUMN (CHAR_TAB)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_FILE_NAME "WikipediaEnglishClubPrepositionsAndInverse.txt"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_NUMBER_OF_TAGS (4)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_CONDITION (1)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_REVERSE_CONDITION (2)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_TYPE (3)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID (4)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID_VALUE_TRUE "1"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_INVERSEPREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID_VALUE_FALSE "0"
#endif


#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_REPLACEMENT_STRING_DEFAULT ""	//will be tense specific

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES (3)
static string lrpPhrasalVerbDatabaseTagArbitraryNameArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES] = {GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMETHING_NAME, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEWHERE_NAME, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NAME};

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NUMBER_OF_VOWELS (5)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NUMBER_OF_CONSTONANTS (21)
static char englishVowelArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NUMBER_OF_VOWELS] = {'a', 'e', 'i', 'o', 'u'};
static char englishConsonantArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NUMBER_OF_CONSTONANTS] = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};
/*
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DOUBLE_CONSONANT_RULE1_LAST_LETTER_EXCEPTIONS_NUMBER_OF_TYPES (4)
static char lrpVerbDoubleConsonantRuleOneLastLetterExceptions[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DOUBLE_CONSONANT_RULE1_LAST_LETTER_EXCEPTIONS_NUMBER_OF_TYPES] = {'w', 'x', 'y', 'z'};
*/

#define GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS (6)		//CHECKTHIS; need to location RelEx/Stanford dependency relation specification and match the exact same punction marks detected
static char nlpPunctionMarkCharacterArray[GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS] = {CHAR_FULLSTOP, CHAR_COMMA, CHAR_SEMICOLON, CHAR_COLON, CHAR_QUESTIONMARK};	//CHAR_EXCLAMATION_MARK - currently interpreted as math [CHECKTHIS]
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_MATH_GROUPING
	#define GIA_NLP_NUMBER_OF_MATH_CHARACTERS_GROUPED (8)
	static char nlpMathCharacterGroupedArray[GIA_NLP_NUMBER_OF_MATH_CHARACTERS_GROUPED] = {CHAR_EQUALS, CHAR_PLUS, CHAR_GREATER_THAN, CHAR_LESS_THAN, CHAR_AMPERSAND, CHAR_TILDA, CHAR_EXCLAMATION_MARK, CHAR_STAR};
	#define GIA_NLP_NUMBER_OF_MATH_CHARACTERS_UNGROUPED (2)
	static char nlpMathCharacterUngroupedArray[GIA_NLP_NUMBER_OF_MATH_CHARACTERS_UNGROUPED] = {CHAR_OPEN_BRACKET, CHAR_CLOSE_BRACKET};
	#define GIA_NLP_NUMBER_OF_MATH_CHARACTERS (10)
	static char nlpMathCharacterArray[GIA_NLP_NUMBER_OF_MATH_CHARACTERS] = {CHAR_EQUALS, CHAR_PLUS, CHAR_GREATER_THAN, CHAR_LESS_THAN, CHAR_AMPERSAND, CHAR_TILDA, CHAR_EXCLAMATION_MARK, CHAR_STAR, CHAR_OPEN_BRACKET, CHAR_CLOSE_BRACKET};
#else
	#define GIA_NLP_NUMBER_OF_MATH_CHARACTERS (10)		//CHECKTHIS; need to location RelEx/Stanford dependency relation specification and match the exact same punction marks detected. Also must be matched to NLC_PREPROCESSOR_MATH_OPERATOR...
	static char nlpMathCharacterArray[GIA_NLP_NUMBER_OF_MATH_CHARACTERS] = {CHAR_OPEN_BRACKET, CHAR_CLOSE_BRACKET, CHAR_EQUALS, CHAR_PLUS, CHAR_GREATER_THAN, CHAR_LESS_THAN, CHAR_AMPERSAND, CHAR_TILDA, CHAR_EXCLAMATION_MARK, CHAR_STAR};	//CHAR_DASH? what about 3-5 [currently interpreted as 1 word]
#endif
#define GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE (2)
static char nlpPunctionMarkCharacterEndOfSentenceArray[GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE] = {CHAR_FULLSTOP, CHAR_QUESTIONMARK};
#define GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS (3)		//CHECKTHIS; need to location RelEx/Stanford dependency relation specification and match the exact same punction marks detected
static char nlpWhitespaceCharacterArray[GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS] = {CHAR_NEWLINE, CHAR_TAB, CHAR_SPACE};
#define GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS (2)
static char nlpQuotationMarkCharacterArray[GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS] = {CHAR_INVERTED_COMMAS, CHAR_APOSTROPHE};


#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOWER_CASE_LETTERS_NUMBER_OF_TYPES (26)
static char preprocessorLowerCaseLettersArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOWER_CASE_LETTERS_NUMBER_OF_TYPES] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_UPPER_CASE_LETTERS_NUMBER_OF_TYPES (26)
static char preprocessorUpperCaseLettersArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_UPPER_CASE_LETTERS_NUMBER_OF_TYPES] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOWER_OR_UPPER_CASE_LETTERS_NUMBER_OF_TYPES (52)
static char preprocessorLowerOrUpperCaseLettersArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOWER_OR_UPPER_CASE_LETTERS_NUMBER_OF_TYPES] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};


#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP "fight"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PRESENT "fights"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_CONTINUOUS "fighting"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PAST "fought"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PASTPARTICIPLE "fought"
static string lrpDummyCollapsedPhrasalVerbNameForNLPgrammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS] = {GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PRESENT, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_CONTINUOUS, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PAST, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PASTPARTICIPLE};

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP "near"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADVERB_NAME_FOR_NLP "very"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADJECTIVE_NAME_FOR_NLP "blue"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_NOUN_NAME_FOR_NLP "house"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_VERB_NAME_FOR_NLP "did"

/*
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_0 "dummy{reposition1"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_1 "dummy{reposition2"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_2 "dummy{reposition3"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_3 "dummy{reposition4"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_4 "dummy{reposition5"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_MAX_NUMBER_SENTENCE_INSTANCES (5)
string lrpDummyCollapsedMultiwordPrepositionLemmaNameForNLPArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_MAX_NUMBER_SENTENCE_INSTANCES] = {GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_0, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_1, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_2, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_3, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_4};
*/

#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_EXACT_DEFINED_SECTION_TYPE 1
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_UNDEFINED_TYPE 0
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_TYPE 1
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADVERB_TYPE 2
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_ADJECTIVE_TYPE 3
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_NOUN_TYPE 4
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_MULTIWORD_VERB_TYPE 5


#define MAXIMUM_NUMBER_WORDS_PER_SENTENCE 1000	//same value from GIAtranslatorOperations.h


#ifdef GIA_PREPROCESSOR_DERIVE_NOUN_VARIANTS
#define GIA_TRANSLATOR_ENGLISH_CONSONANTS_NUMBER_OF_TYPES (21)
static char translatorEnglishConsonants[GIA_TRANSLATOR_ENGLISH_CONSONANTS_NUMBER_OF_TYPES] = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'x', 'z', 'w', 'y'};		//usually W and Y.
//#define GIA_TRANSLATOR_ENGLISH_VOWELS_NUMBER_OF_TYPES (5)
//static char translatorEnglishVowels[GIA_TRANSLATOR_ENGLISH_VOWELS_NUMBER_OF_TYPES] = {'a', 'e', 'i', 'o', 'u'};
#define GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_ADDITION_DEFAULT_NUMBER_OF_TYPES (1)
static string translatorEnglishNounPluralModifierAdditionDefaultArray[GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_ADDITION_DEFAULT_NUMBER_OF_TYPES] = {"s"};
#define GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_ADDITION_NUMBER_OF_TYPES (6)
static string translatorEnglishNounPluralModifierAdditionArray[GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_ADDITION_NUMBER_OF_TYPES][2] = {{"ch", "es"}, {"s", "es"}, {"sh", "es"}, {"x", "es"}, {"z", "es"}, {"o", "es"}};
#define GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_REPLACEMENT_SUCCEEDING_CONSONANT_NUMBER_OF_TYPES (6)
static string translatorEnglishNounPluralModifierReplacementSucceedingConsonantArray[GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_REPLACEMENT_SUCCEEDING_CONSONANT_NUMBER_OF_TYPES][2] = {{"Cy", "Cies"}};
#define GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_REPLACEMENT_NUMBER_OF_TYPES (14)
static string translatorEnglishNounPluralModifierReplacementArray[GIA_TRANSLATOR_ENGLISH_NOUN_PLURAL_MODIFIER_REPLACEMENT_NUMBER_OF_TYPES][2] = {{"f", "fe"}, {"fe", "ves"}, {"ix", "ices"}, {"us", "i"}, {"lum", "la"}, {"ium", "ia"}, {"ex", "ices"}, {"um", "a"}, {"a", "ae"}, {"mnus", "mnui"}, {"is", "es"}, {"eau", "eaus"}, {"eau", "eaux"}, {"o", "i"}};
#endif

#ifdef GIA_NEURAL_NETWORK
#define GIA_NEURAL_NETWORK_PREPROCESSOR_WORD_TYPE_UNDEFINED (0)
#define GIA_NEURAL_NETWORK_PREPROCESSOR_WORD_TYPE_CONCEPT (1)
#define GIA_NEURAL_NETWORK_PREPROCESSOR_WORD_TYPE_SPECIFIC_CONCEPT (2)
#define GIA_NEURAL_NETWORK_PREPROCESSOR_WORD_TYPE_IGNORE (3)
#ifndef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
#define GIA_NEURAL_NETWORK_PREPROCESSOR_WORD_TYPE_CONCEPT_DEFINITION (4)
#endif
#endif



class GIApreprocessorWord
{
public:

	GIApreprocessorWord(void);
	GIApreprocessorWord(string tagNameNew);
	~GIApreprocessorWord(void);

	string tagName;
	
	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	GIAfeature* featureReferenceOriginal;
	GIAfeature* featureReference;
	GIAentityNode* entityReference;
	#endif
	
	bool plainTextWord;
	
	#ifdef GIA_NEURAL_NETWORK
	int neuralNetworkPreprocessorWordType;
	ANNneuron* wordShortcutToConceptNeuron;
	string tagNameOriginalNonLemma;
	#endif
	
	GIApreprocessorWord* nextTag;
};

class GIApreprocessorMultiwordReductionWord: public GIApreprocessorWord
{
public:

	GIApreprocessorMultiwordReductionWord(void);
	~GIApreprocessorMultiwordReductionWord(void);

	string tagNameLemma;

	bool base;	//used to indicate if the current tag in the phrasal verb is the base verb of the phrasal verb (or lemma) - NB the first word in each phrasal verb defined in the database is assumed to be the lemma, but there may be additional instances
	string grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_VERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS];		//only for lemma	[2: for alternate versions]
	int grammaticalTenseForm;	//for wordLists
	
	/*
	#ifdef GIA_PREPROCESSOR_DERIVE_NOUN_VARIANTS
	vector<string> nounPluralVariants;
	#endif
	*/
	
	//verb case additional only:
	int grammaticalTenseModifier;
	string baseName;	
};

class GIApreprocessorMultiwordReductionPhrasalVerbWord: public GIApreprocessorMultiwordReductionWord
{
public:

	GIApreprocessorMultiwordReductionPhrasalVerbWord(void);
	~GIApreprocessorMultiwordReductionPhrasalVerbWord(void);

	bool tagSpecialArbitraryName;	//used to indicate if the current tag is sth/swh/sb
	bool tagSpecialArbitraryNameType;
	bool optional;	//used to specify if a tag is optional designated by "(...)" in the phrasal verb database
	//bool primary;	//is the first word in the phrasal verb (assumed to be lemma)?
	//string primaryPhrasalVerbReplacementString;
	//string primaryPhrasalVerbReplacementStringNLPonly;
	GIApreprocessorMultiwordReductionPhrasalVerbWord* alternateTag;			//used to specify an alternate (but corresponding) tag name designated by "/" in the phrasal verb database
};

class GIApreprocessorMultiwordReductionIrregularVerbWord: public GIApreprocessorMultiwordReductionWord
{
public:

	GIApreprocessorMultiwordReductionIrregularVerbWord(void);
	~GIApreprocessorMultiwordReductionIrregularVerbWord(void);

	GIApreprocessorMultiwordReductionIrregularVerbWord* alternateTag;
};

class GIApreprocessorMultiwordReductionPlainTextWord: public GIApreprocessorMultiwordReductionWord
{
public:

	GIApreprocessorMultiwordReductionPlainTextWord(void);
	GIApreprocessorMultiwordReductionPlainTextWord(string tagNameNew);
	~GIApreprocessorMultiwordReductionPlainTextWord(void);
	
	int entityIndex;	//this should be depreciated
	
	bool collapsedPhrasalVerbExactDefinedSection;
	bool collapsedMultiwordWord;
	int collapsedMultiwordWordType;
	//bool collapsedPhrasalVerbExactDefinedSectionTemp;	//not required as searchAndReplacePhrasalVerbs is only ever executed once
	bool collapsedMultiwordWordTemp;	//this is reset everytime searchAndReplaceMultiwordWordList is executed to prevent collapsedMultiwordWord from being redetected
	int grammaticalTenseFormDetected;

	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	GIApreprocessorMultiwordReductionPlainTextWord* preprocessorUpperLevelWordReference;
	int preprocessorUpperLevelWordReferenceSize;	//number of words in preprocessor upper level phrase corresponding to preprocessor word
	GIApreprocessorMultiwordReductionPlainTextWord* preprocessorLowerLevelWordReference;
	int characterIndexInSentenceContentsOriginalText; 	//character index in original sentence (without preprocessing): ie record the exact character index of the first character of the word in the original unprocessed sentence text
	#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	string tagNameLRPforNLP;
	#endif
};



/*
//NOT USED:
class GIApreprocessorMultiwordReductionSentence
{
public:

	GIApreprocessorMultiwordReductionSentence(void);
	~GIApreprocessorMultiwordReductionSentence(void);

	GIApreprocessorMultiwordReductionSentence* nextSentence;
};
*/

class GIApreprocessorMultiwordReductionBasicSentence
{
public:

	GIApreprocessorMultiwordReductionBasicSentence(void);
	~GIApreprocessorMultiwordReductionBasicSentence(void);

	GIApreprocessorMultiwordReductionWord* firstTagInSentence;
	
	GIApreprocessorMultiwordReductionBasicSentence* nextSentence;
};

class GIApreprocessorMultiwordReductionPhrasalVerbSentence
{
public:

	GIApreprocessorMultiwordReductionPhrasalVerbSentence(void);
	~GIApreprocessorMultiwordReductionPhrasalVerbSentence(void);

	GIApreprocessorMultiwordReductionPhrasalVerbWord* firstTagInSentence;
	GIApreprocessorMultiwordReductionPhrasalVerbSentence* alternateSentence;		//used to specifiy an alternate (but corresponding) phrasal verb designated by "or" in the phrasal verb database
	
	GIApreprocessorMultiwordReductionPhrasalVerbSentence* nextSentence;
};

class GIApreprocessorMultiwordReductionIrregularVerbSentence
{
public:

	GIApreprocessorMultiwordReductionIrregularVerbSentence(void);
	~GIApreprocessorMultiwordReductionIrregularVerbSentence(void);

	GIApreprocessorMultiwordReductionIrregularVerbWord* firstTagInSentence;
	
	GIApreprocessorMultiwordReductionIrregularVerbSentence* nextSentence;
};



class GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo
{
public:

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(void);
	~GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(void);

	string wordWithLRP;		//LRP generated word name
	string lemmaWithLRP;		//LRP generated lemma name
	string wordWithLRPforNLPonly;	//temporary/dummy word name such that NLP can properly parse the text

	int entityIndex;	//used to map the NLP dummy replacement lemma to the official LRP generated lemma (for entity name reversion after NLP)
	int sentenceIndex;	//used to map the NLP dummy replacement lemma to the official LRP generated lemma (for entity name reversion after NLP)

	GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* next;
};


class GIApreprocessorMultiwordReductionClassClass
{
	private: SHAREDvarsClass SHAREDvars;

	#ifdef GIA_PREPROCESSOR_RECORD_REFERENCES
	public: string generateTextFromPreprocessorSentenceWordList(const GIApreprocessorWord* firstWordInSentence);
		public: string generateTextFromPreprocessorSentenceWordList(const GIApreprocessorWord* firstWordInSentence, const bool LRPforNLP);
	public: string generateTextFromVectorWordList(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList);
		public: string generateTextFromVectorWordList(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList, const bool LRPforNLP);
			public: string generateTextFromPreprocessorSentenceWord(const GIApreprocessorWord* word, const bool LRPforNLP, const bool isFirstWordInSentence);
	public: int calculateLengthOfGeneratedVectorWordListText(vector<GIApreprocessorWord*>* logicReferenceVariableWordList);
	public: bool generateSentenceWordList(GIApreprocessorWord* sentenceContentsFirstWord, vector<GIApreprocessorWord*>* logicReferenceVariableWordList);
	public: bool generateFlatSentenceWordList(const vector<GIApreprocessorWord*>* logicReferenceVariableWordList, GIApreprocessorMultiwordReductionPlainTextWord** sentenceContentsFirstWord);
	public: bool addWordListToWordList(vector<GIApreprocessorWord*>* wordList, vector<GIApreprocessorWord*>* wordListToAdd);
	public: bool addStringArrayToWordList(vector<GIApreprocessorWord*>* wordList, const string* stringArrayToAdd, const int arraySize);
	public: bool addStringToWordList(vector<GIApreprocessorWord*>* wordList, const string stringToAdd);
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_INTRAWORD_PUNCTUATION_MARK
	public: bool isIntrawordPunctuationMark(const int indexOfCurrentToken, const string* lineContents);
	#endif
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_APOSTROPHES_POSSESSION_AND_OMISSION
	public: bool isApostrophePossessionOrOmission(const int indexOfCurrentToken, const string* lineContents);
	public: bool isApostrophePossessionOrOmission(const GIApreprocessorWord* word);
	#endif	
	
	public: bool findAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind,  const string stringSimpleReplacement);
		public: bool findAndReplaceAllOccurancesSimpleSubstringInWordListWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const string* stringSimpleToFind,  const string* stringSimpleReplacement);
			public: bool findAndReplaceSimpleSubstringInWordListAtIndexWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind, const int indexToPerformFind, const string stringSimpleReplacement);
				public: bool findAndReplaceWordListInWordListAtIndexWithWordList(vector<GIApreprocessorWord*>* wordList, vector<GIApreprocessorWord*>* wordListToFind, const int indexToPerformFind, vector<GIApreprocessorWord*>* wordListReplacement);
	public: bool findSimpleSubstringInWordList(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind);
		public: bool findSimpleSubstringInWordList(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind, const int startIndexToPerformFind);
			public: bool findSubWordListInWordList(vector<GIApreprocessorWord*>* wordList, const vector<GIApreprocessorWord*>* wordListToFind, const int startIndexToPerformFind);	
	public: bool findSimpleSubstringInWordListAtIndex(const vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind, const int indexToPerformFind, const bool caseInsensitive);
		public: bool findSubWordListInWordListAtIndex(const vector<GIApreprocessorWord*>* wordList, const vector<GIApreprocessorWord*>* wordListToFind, const int indexToPerformFind, const bool caseInsensitive);
	public: bool generateSentenceWordListFromStringSimple(vector<GIApreprocessorWord*>* wordList, const string* stringSimple);
		
	//these are similiar to C++ string library functions;
	public: int findStringInWordList(const vector<GIApreprocessorWord*>* wordList, const string stringToFind);
		public: int findStringInWordList(const vector<GIApreprocessorWord*>* wordList, const string stringToFind, const int startIndexToPerformFind);
	
	public: bool findSubstringAtStartOfWordInWordList(const vector<GIApreprocessorWord*>* wordList, const string substringToFind);
		public: bool findSubstringAtStartOfWordInWordList(const vector<GIApreprocessorWord*>* wordList, const string substringToFind, const int startIndexToPerformFind);
	
	public: vector<GIApreprocessorWord*> extractSubWordListInWordList(const vector<GIApreprocessorWord*>* wordList, const int startIndexToExtract);
		public: vector<GIApreprocessorWord*> extractSubWordListInWordList(const vector<GIApreprocessorWord*>* wordList, const int startIndexToExtract, const int numberOfWordsToExtract);
	
	public: bool insertWordListIntoWordList(vector<GIApreprocessorWord*>* wordList, const vector<GIApreprocessorWord*>* wordListToInsert, const int indexToInsert);
	public: bool insertStringIntoWordList(vector<GIApreprocessorWord*>* wordList, const string stringToInsert, const int indexToInsert);
		public: bool insertWordIntoWordList(vector<GIApreprocessorWord*>* wordList, GIApreprocessorWord* wordToInsert, const int indexToInsert);
	
	public: bool wordListFindAndRemoveAllOccurancesSimpleSubstringInWordList(vector<GIApreprocessorWord*>* wordList, const string stringSimpleToFind);
		public: bool removeWordFromWordList(vector<GIApreprocessorWord*>* wordList, const int indexToRemove);
			public: bool removeWordsFromWordList(vector<GIApreprocessorWord*>* wordList, const int indexToRemove, const int numberElementsToRemove);
			
	public: bool replaceWordListAtIndexWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const int indexToPerformReplacement, const string stringSimpleReplacement);
		public: bool replaceWordListAtIndexWithSimpleSubstring(vector<GIApreprocessorWord*>* wordList, const int indexToPerformReplacement, vector<GIApreprocessorWord*>* wordListReplacement);
	
	public: bool printWordList(const vector<GIApreprocessorWord*>* wordList);
	#endif	

};




	
#endif

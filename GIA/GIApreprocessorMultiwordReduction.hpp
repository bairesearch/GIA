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
 * File Name: GIApreprocessorMultiwordReduction.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a3d 22-March-2017
 * Requirements: requires plain text file
 * Description: Preprocessor Multiword Reduction
 *
 *******************************************************************************/


#ifndef HEADER_GIA_PREPROCESSOR_MULTIWORD_REDUCTION
#define HEADER_GIA_PREPROCESSOR_MULTIWORD_REDUCTION

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "SHAREDvars.hpp"	//file io

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
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
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS (5)	//run (infinitive), runs (present), running (continuous), ran (past), run (past partiple)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE (0)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT (1)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS (2)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST (3)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE (4)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS (2)	//standard/alternate (ie double constanant form OR irregular verb alternate case)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD (0)
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE (1)

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

#define GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS (5)		//CHECKTHIS; need to location RelEx/Stanford dependency relation specification and match the exact same punction marks detected
static char nlpPunctionMarkCharacterArray[GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS] = {CHAR_FULLSTOP, CHAR_COMMA, CHAR_SEMICOLON, CHAR_COLON, CHAR_QUESTIONMARK};
#define GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE (2)
static char nlpPunctionMarkCharacterEndOfSentenceArray[GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE] = {CHAR_FULLSTOP, CHAR_QUESTIONMARK};
#define GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS (3)		//CHECKTHIS; need to location RelEx/Stanford dependency relation specification and match the exact same punction marks detected
static char nlpWhitespaceCharacterArray[GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS] = {CHAR_NEWLINE, CHAR_TAB, CHAR_SPACE};
#define GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS (2)
static char nlpQuotationMarkCharacterArray[GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS] = {CHAR_INVERTED_COMMAS, CHAR_APOSTROPHE};


#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP "fight"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PRESENT "fights"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_CONTINUOUS "fighting"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PAST "fought"
#define GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PASTPARTICIPLE "fought"
static string lrpDummyCollapsedPhrasalVerbNameForNLPgrammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS] = {GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PRESENT, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_CONTINUOUS, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PAST, GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PASTPARTICIPLE};

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

class GIApreprocessorMultiwordReductiontag
{
public:

	GIApreprocessorMultiwordReductiontag(void);
	~GIApreprocessorMultiwordReductiontag(void);

	string tagName;
	string tagNameLemma;

//phrasal verbs only:
	bool tagSpecialArbitraryName;	//used to indicate if the current tag is sth/swh/sb
	bool tagSpecialArbitraryNameType;

	bool optional;	//used to specify if a tag is optional designated by "(...)" in the phrasal verb database

	//bool primary;	//is the first word in the phrasal verb (assumed to be lemma)?
	//string primaryPhrasalVerbReplacementString;
	//string primaryPhrasalVerbReplacementStringNLPonly;

	bool base;	//used to indicate if the current tag in the phrasal verb is the base verb of the phrasal verb (or lemma) - NB the first word in each phrasal verb defined in the database is assumed to be the lemma, but there may be additional instances
	string grammaticalTenseFormsArray[GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS][GIA_PREPROCESSOR_MULTIWORD_REDUCTION_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS];		//only for lemma	[2: for alternate versions]
	int grammaticalTenseFormDetected;

//plain text only:
	int entityIndex;
	int sentenceIndex;
	bool collapsedPhrasalVerbExactDefinedSection;
	bool collapsedMultiwordWord;
	int collapsedMultiwordWordType;
	//bool collapsedPhrasalVerbExactDefinedSectionTemp;	//not required as searchAndReplacePhrasalVerbs is only ever executed once
	bool collapsedMultiwordWordTemp;	//this is reset everytime searchAndReplaceMultiwordWordList is executed to prevent collapsedMultiwordWord from being redetected
	
	GIApreprocessorMultiwordReductiontag* alternateTag;			//used to specify an alternate (but corresponding) tag name designated by "/" in the phrasal verb database
	GIApreprocessorMultiwordReductiontag* alternateSentence;		//used to specifiy an alternate (but corresponding) phrasal verb designated by "or" in the phrasal verb database
	GIApreprocessorMultiwordReductiontag* nextTag;				//next tag in phrasal verb
	GIApreprocessorMultiwordReductiontag* nextSentence;			//first tag in next phrasal verb

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

class GIApreprocessorMultiwordReductionClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: bool initialiseLRP(const string newLRPDataFolderName, const bool newUseLRP);
		private: bool loadWordListWrapper(const string lrpDataFolderName, bool* wordListLoaded, const string wordListFileName, GIApreprocessorMultiwordReductiontag** firstTagInWordListGlobal);
			private: bool loadWordList(const string wordListFileName, GIApreprocessorMultiwordReductiontag* firstTagInWordList);
	public: bool getUseLRP();

	public: GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* getActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo();
	public: void setActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void initialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);
	public: void deinitialiseActiveGIApreprocessorMultiwordReductionTagTextCorrespondenceInfo(const bool isQuery);

	public: bool parseTextFileAndReduceLanguage(const string plainTextInputFileName, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName);	
		private: bool loadIrregularVerbList(const string irregularVerbListFileName, GIApreprocessorMultiwordReductiontag* firstTagInIrregularVerbList);
		private: bool loadPhrasalVerbDataAndGenerateAllTenseVariants(const string phrasalVerbDatabaseFileName, GIApreprocessorMultiwordReductiontag* firstTagInPhrasalVerbList, GIApreprocessorMultiwordReductiontag* firstTagInIrregularVerbList);
			private: bool generateTenseVariantsOfVerbBase(GIApreprocessorMultiwordReductiontag* baseTag, GIApreprocessorMultiwordReductiontag* firstTagInIrregularVerbList);
				private: void copyDefaultVerbTenseFormsToAlternateTenseForms(GIApreprocessorMultiwordReductiontag* baseTag, const bool irregularVerbFound);
		public: bool loadPlainTextFile(const string plainTextInputFileName, GIApreprocessorMultiwordReductiontag* firstTagInPlainText);
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
			public: bool isIntrawordPunctuationMark(const int indexOfCurrentToken, const string* lineContents);
		#endif
		private: bool searchAndReplacePhrasalVerbs(GIApreprocessorMultiwordReductiontag* firstTagInPlainText, GIApreprocessorMultiwordReductiontag* firstTagInPhrasalVerbList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo);
		private: bool loadMultiwordWordListAndSearchAndReplace(const string multiwordWordListFileName, GIApreprocessorMultiwordReductiontag* firstTagInPlainText, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
			private: bool loadMultiwordWordList(const string multiwordWordListFileName, GIApreprocessorMultiwordReductiontag* firstTagInMultiwordWordList);
			private: bool searchAndReplaceMultiwordWordList(GIApreprocessorMultiwordReductiontag* firstTagInPlainText, const GIApreprocessorMultiwordReductiontag* firstTagInMultiwordWordList, GIApreprocessorMultiwordReductionTagTextCorrespondenceInfo* firstGIApreprocessorMultiwordReductiontagCorrespondenceInfo, const int wordListType);
		public: bool writeTagListToFile(const GIApreprocessorMultiwordReductiontag* firstTagInPlainText, const string plainTextLRPoutputFileName, const string plainTextLRPforNLPoutputFileName, const bool performLRPforNLPoutput);
			private: string generateWordWithLRPforNLPonly(const GIApreprocessorMultiwordReductiontag* currentTagInPlainTextSentence);
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
	public: void revertNLPtagNameToOfficialLRPtagName(GIAfeature* feature, const GIAsentence* currentSentenceInList, const GIArelation* currentRelationInListForPrepositionsOnly, const bool isPreposition, bool* foundOfficialLRPreplacementString);
	#endif

	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
	public: bool determineVerbCaseWrapper(const string word, string* baseNameFound, int* grammaticalTenseModifier);
		private: bool determineVerbCaseAdditional(const string word, GIApreprocessorMultiwordReductiontag* firstTagInVerbList, string* baseNameFound, int* grammaticalTenseModifier);
			private: void testVerbCase(string tagName, const string wordLowerCase, const string baseTenseFormStart, string baseTenseFormAppend, int* numberOfCharactersInBaseTenseFormAppend, bool* foundVerbCase, string* baseNameFound, int* grammaticalTenseModifier, int grammaticalTenseModifierNew);

	#endif
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
	public: bool determineIfWordIsIrregularVerbContinuousCaseWrapper(const string word, string* baseNameFound);
		private: bool determineIfWordIsIrregularVerbContinuousCase(const string word, GIApreprocessorMultiwordReductiontag* firstTagInIrregularVerbList, string* baseNameFound);
	#endif
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_INVERSE_PREPOSITIONS_LIST
	private: bool loadPrepositionsInverseList(const string prepositionsInverseListFileName, GIApreprocessorMultiwordReductiontag* firstTagInPrepositionsInverseList);
	#endif
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_NORMALISE_PREPOSITIONS
		public: void detectIfInverseOrTwoWayConditionRequired(const string conditionName, bool* inverseConditionRequired, bool* twoWayConditionRequired, string* inverseConditionName);
		#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DETECT_PREPOSITION_TYPE
		public: bool identifyConditionType(GIAentityNode* conditionRelationshipEntity);
		#endif
		#endif
	
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_LOAD_WORD_LISTS
	public: bool parseVerbDataGenerateAllTenseVariants();
	#endif
	#ifdef GIA_PREPROCESSOR_SENTENCE_REFERENCE_SET
	public: bool determineVerbCaseStandard(const string word, int* grammaticalBaseTenseForm);
	public: bool determineVerbCaseStandardWithAdditional(const string word, int* grammaticalBaseTenseForm);
	public: bool determineIsPreposition(const string word);
	public: bool determineIsAdverb(const string word);
	public: bool determineIsAdjective(const string word);
		public: bool determineIsInWordList(GIApreprocessorMultiwordReductiontag* firstTagInWordList, const string word);
	#endif
			
};


#endif

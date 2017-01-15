/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: GIAlrp.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2o7a 23-October-2016
 * Requirements: requires plain text file
 * Description: Language Reduction Preprocessor
 *
 *******************************************************************************/


#ifndef HEADER_GIA_LRP
#define HEADER_GIA_LRP

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"

#ifdef GIA_LRP_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
#define GIA_LRP_NLP_PARSABLE_PHRASE_CHARACTERS_NUMBER_OF_TYPES (65)	//must sync with NLC_PREPROCESSOR_MATH_NLP_PARSABLE_PHRASE_CHARACTERS_NUMBER_OF_TYPES
static char GIALRPNLPparsableCharacters[GIA_LRP_NLP_PARSABLE_PHRASE_CHARACTERS_NUMBER_OF_TYPES] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_', '.','\''};	//must sync with preprocessorMathNLPparsableCharacters
#endif

//#define GIA_LRP_USE_OLD_PREPOSITION_SPECIFIC_CODE_NOT_REQUIRED

#define GIA_LRP_PHRASALVERB_DATABASE_FILE_NAME "CambridgePhrasalVerbs.txt"
#define GIA_LRP_IRREGULARVERB_DATABASE_FILE_NAME "WikipediaIrregularVerbs.txt"
#define GIA_LRP_VERB_DATABASE_FILE_NAME "WordnetVerbs.txt"
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_OR "or"
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMETHING_NAME "sth"
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEWHERE_NAME "swh"
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NAME "sb"
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMETHING (0)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEWHERE (1)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY (2)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_UNDEFINED (GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMETHING)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_MAX_NUMBER_WORDS (3)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ALTERNATE (CHAR_FORWARDSLASH)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_OPEN_OPTIONAL (CHAR_OPEN_BRACKET)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_CLOSE_OPTIONAL (CHAR_CLOSE_BRACKET)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS (5)	//run (infinitive), runs (present), running (continuous), ran (past), run (past partiple)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_INFINITIVE (0)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT (1)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS (2)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PAST (3)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_PASTPARTICIPLE (4)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS (2)	//standard/alternate (ie double constanant form OR irregular verb alternate case)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_STANDARD (0)
#define GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_TENSE_FORM_VERSION_ALTERNATE (1)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND "s"
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND "ing"
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND "ed"
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND_CASE3 "ies"
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_CASE3 "ied"
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PRESENT_APPEND_LENGTH (1)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_CONTINUOUS_APPEND_LENGTH (3)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_PAST_APPEND_LENGTH (2)
#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND "able"
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_APPEND_CASE3 "iable"
#ifdef GIA_FEATURE_POS_TAG_VERB_POTENTIAL_INVERSE
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND "ive"		//0 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1II "itive"	//0 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE1III "ative"	//0 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2 "itive"	//last 1 characters replaced (e)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE2II "mentive"	//last 1 characters replaced (e)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_POTENTIAL_INVERSE_APPEND_CASE3 "imentive"	//last 1 characters replaced (y)
#endif
#endif
#ifdef GIA_FEATURE_POS_TAG_VERB_DESCRIPTION
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND "ment"		//0 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1II "ament"	//0 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE1III "lment"	//0 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE2 "ment"	//last 1 characters replaced (e)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE3 "iment"	//last 1 characters replaced (y)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION_APPEND_CASE5 "pment"	//last 2 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND "ion"		//0 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1II "ition"	//0 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE1III "ation"	//0 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2 "ion"	//last 1 characters replaced (e)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2II "ition"	//last 1 characters replaced (e)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE2III "ation"	//last 1 characters replaced (e)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE3 "ication"	//last 1 characters replaced (y)
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6I "amation"	//last 3 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6II "ision"	//last 3 characters replaced
#define GIA_LRP_VERB_DATABASE_TAG_BASE_TENSE_FORM_DESCRIPTION2_APPEND_CASE6III "ition"	//last 3 characters replaced

#endif

#ifdef GIA_LRP_NORMALISE_INVERSE_PREPOSITIONS
#define GIA_LRP_PREPOSITIONS_DATABASE_TAG_NEXTCOLUMN (CHAR_TAB)
#define GIA_LRP_PREPOSITIONS_DATABASE_FILE_NAME "WikipediaEnglishClubPrepositionsAndInverse.txt"
#define GIA_LRP_PREPOSITIONS_DATABASE_NUMBER_OF_TAGS (4)
#define GIA_LRP_PREPOSITIONS_DATABASE_TAG_CONDITION (1)
#define GIA_LRP_PREPOSITIONS_DATABASE_TAG_REVERSE_CONDITION (2)
#define GIA_LRP_PREPOSITIONS_DATABASE_TAG_TYPE (3)
#define GIA_LRP_PREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID (4)
#define GIA_LRP_PREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID_VALUE_TRUE "1"
#define GIA_LRP_PREPOSITIONS_DATABASE_TAG_INVERT_REVERSE_CONDITION_VALID_VALUE_FALSE "0"
#define GIA_LRP_MULTIWORDPREPOSITION_DATABASE_FILE_NAME "WikipediaEnglishClubMultiwordPrepositions.txt"
#else
#define GIA_LRP_MULTIWORDPREPOSITION_DATABASE_FILE_NAME "WikipediaMultiwordPrepositions.txt"
#endif

#define GIA_LRP_PHRASALVERB_REPLACEMENT_STRING_DEFAULT ""	//will be tense specific

#define GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES (3)
static string lrpPhrasalVerbDatabaseTagArbitraryNameArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NUMBER_OF_TYPES] = {GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMETHING_NAME, GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEWHERE_NAME, GIA_LRP_PHRASALVERB_DATABASE_TAG_ARBITRARYNAME_SOMEBODY_NAME};

#define GIA_LRP_NUMBER_OF_VOWELS (5)
#define GIA_LRP_NUMBER_OF_CONSTONANTS (21)
static char englishVowelArray[GIA_LRP_NUMBER_OF_VOWELS] = {'a', 'e', 'i', 'o', 'u'};
static char englishConsonantArray[GIA_LRP_NUMBER_OF_CONSTONANTS] = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};
/*
#define GIA_LRP_VERB_DOUBLE_CONSONANT_RULE1_LAST_LETTER_EXCEPTIONS_NUMBER_OF_TYPES (4)
static char lrpVerbDoubleConsonantRuleOneLastLetterExceptions[GIA_LRP_VERB_DOUBLE_CONSONANT_RULE1_LAST_LETTER_EXCEPTIONS_NUMBER_OF_TYPES] = {'w', 'x', 'y', 'z'};
*/

#define GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS (5)		//CHECKTHIS; need to location RelEx/Stanford dependency relation specification and match the exact same punction marks detected
static char nlpPunctionMarkCharacterArray[GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS] = {CHAR_FULLSTOP, CHAR_COMMA, CHAR_SEMICOLON, CHAR_COLON, CHAR_QUESTIONMARK};
#define GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE (2)
static char nlpPunctionMarkCharacterEndOfSentenceArray[GIA_NLP_NUMBER_OF_PUNCTUATION_MARK_CHARACTERS_END_OF_SENTENCE] = {CHAR_FULLSTOP, CHAR_QUESTIONMARK};
#define GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS (3)		//CHECKTHIS; need to location RelEx/Stanford dependency relation specification and match the exact same punction marks detected
static char nlpWhitespaceCharacterArray[GIA_NLP_NUMBER_OF_WHITESPACE_CHARACTERS] = {CHAR_NEWLINE, CHAR_TAB, CHAR_SPACE};
#define GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS (2)
static char nlpQuotationMarkCharacterArray[GIA_NLP_NUMBER_OF_QUOTATIONMARK_CHARACTERS] = {CHAR_INVERTED_COMMAS, CHAR_APOSTROPHE};


#define GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP "fight"
#define GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PRESENT "fights"
#define GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_CONTINUOUS "fighting"
#define GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PAST "fought"
#define GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PASTPARTICIPLE "fought"
static string lrpDummyCollapsedPhrasalVerbNameForNLPgrammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS] = {GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP, GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PRESENT, GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_CONTINUOUS, GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PAST, GIA_LRP_DUMMY_COLLAPSED_PHRASAL_VERB_LEMMA_NAME_FOR_NLP_TENSE_FORM_PASTPARTICIPLE};

#define GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP "near"
/*
#define GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_0 "dummy{reposition1"
#define GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_1 "dummy{reposition2"
#define GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_2 "dummy{reposition3"
#define GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_3 "dummy{reposition4"
#define GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_4 "dummy{reposition5"
#define GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_MAX_NUMBER_SENTENCE_INSTANCES (5)
string lrpDummyCollapsedMultiwordPrepositionLemmaNameForNLPArray[GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_MAX_NUMBER_SENTENCE_INSTANCES] = {GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_0, GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_1, GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_2, GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_3, GIA_LRP_DUMMY_COLLAPSED_MULTIWORD_PREPOSITION_NAME_FOR_NLP_SENTENCE_INSTANCE_4};
*/

#define MAXIMUM_NUMBER_WORDS_PER_SENTENCE 1000	//same value from GIAtranslatorOperations.h

class GIALRPtag
{
public:

	GIALRPtag(void);
	~GIALRPtag(void);

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
	string grammaticalTenseFormsArray[GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORMS][GIA_LRP_PHRASALVERB_DATABASE_TAG_BASE_MAX_NUM_TENSE_FORM_VERSIONS];		//only for lemma	[2: for alternate versions]
	int grammaticalTenseFormDetected;

//plain text only:
	int entityIndex;		//temp var
	int sentenceIndex;		//temp var
	bool collapsedPhrasalVerbExactDefinedSection;	//temp var [aslwas true]
	bool collapsedMultiwordPreposition;				//temp var

	GIALRPtag* alternateTag;			//used to specify an alternate (but corresponding) tag name designated by "/" in the phrasal verb database
	GIALRPtag* alternateSentence;		//used to specifiy an alternate (but corresponding) phrasal verb designated by "or" in the phrasal verb database
	GIALRPtag* nextTag;				//next tag in phrasal verb
	GIALRPtag* nextSentence;			//first tag in next phrasal verb

};

class GIALRPtagTextCorrespondenceInfo
{
public:

	GIALRPtagTextCorrespondenceInfo(void);
	~GIALRPtagTextCorrespondenceInfo(void);

	string wordWithLRP;		//LRP generated word name
	string lemmaWithLRP;		//LRP generated lemma name
	string wordWithLRPforNLPonly;	//temporary/dummy word name such that NLP can properly parse the text

	int entityIndex;	//used to map the NLP dummy replacement lemma to the official LRP generated lemma (for entity name reversion after NLP)
	int sentenceIndex;	//used to map the NLP dummy replacement lemma to the official LRP generated lemma (for entity name reversion after NLP)

	GIALRPtagTextCorrespondenceInfo* next;
};

bool initialiseLRP(string newLRPDataFolderName, bool newUseLRP);
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
	bool loadVerbList(string irregularVerbListFileName, GIALRPtag* firstTagInIrregularVerbList);
		void testVerbCase(string tagName, string wordLowerCase, string baseTenseFormStart, string baseTenseFormAppend, int* numberOfCharactersInBaseTenseFormAppend, bool* foundVerbCase, string* baseNameFound, int* grammaticalTenseModifier, int grammaticalTenseModifierNew);
	#endif
	#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
	bool loadPrepositionsInverseList(string prepositionsInverseListFileName, GIALRPtag* firstTagInPrepositionsInverseList);
	#endif
bool getUseLRP();

GIALRPtagTextCorrespondenceInfo* getCurrentGIALRPtagTextCorrespondenceInfo();
void setCurrentGIALRPtagTextCorrespondenceInfo(bool isQuery);
void initialiseCurrentGIALRPtagTextCorrespondenceInfo(bool isQuery);
void deinitialiseCurrentGIALRPtagTextCorrespondenceInfo(bool isQuery);

bool parseTextFileAndReduceLanguage(string plainTextInputFileName, string plainTextLRPoutputFileName, string plainTextLRPforNLPoutputFileName);
	bool loadIrregularVerbList(string irregularVerbListFileName, GIALRPtag* firstTagInIrregularVerbList);
	bool loadPhrasalVerbDataAndGenerateAllTenseVariants(string phrasalVerbDatabaseFileName, GIALRPtag* firstTagInPhrasalVerbList, GIALRPtag* firstTagInIrregularVerbList);
		bool generateTenseVariantsOfVerbBase(GIALRPtag* baseTag, GIALRPtag* firstTagInIrregularVerbList);
			void copyDefaultVerbTenseFormsToAlternateTenseForms(GIALRPtag* baseTag, bool irregularVerbFound);
	bool loadMultiWordPrepositionData(string multiwordPrepositionListFileName, GIALRPtag* firstTagInMultiwordPrepositionList);
	bool loadPlainTextFile(string plainTextInputFileName, GIALRPtag* firstTagInPlainText);
		#ifdef GIA_LRP_NLP_PARSABLE_PHRASE_SUPPORT_FILENAMES_WITH_FULLSTOPS_AND_FLOATS_AND_TIMES
		bool isIntrawordPunctuationMark(int indexOfCurrentToken, string* lineContents);
		#endif
	bool searchAndReplacePhrasalVerbs(GIALRPtag* firstTagInPlainText, GIALRPtag* firstTagInPhrasalVerbList, GIALRPtagTextCorrespondenceInfo* firstGIALRPtagCorrespondenceInfo);
	bool searchAndReplaceMultiwordPrepositions(GIALRPtag* firstTagInPlainText, GIALRPtag* firstTagInMultiwordPrepositionList, GIALRPtagTextCorrespondenceInfo* firstGIALRPtagCorrespondenceInfo);
	bool writeTagListToFile(GIALRPtag* firstTagInPlainText, string plainTextLRPoutputFileName, string plainTextLRPforNLPoutputFileName);

void revertNLPtagNameToOfficialLRPtagName(GIAfeature* feature, GIAsentence* currentSentenceInList, GIArelation* currentRelationInListForPrepositionsOnly, bool isPreposition, bool* foundOfficialLRPreplacementString);


#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL
bool determineVerbCaseWrapper(string word, string* baseNameFound, int* grammaticalTenseModifier);
bool determineVerbCase(string word, GIALRPtag* firstTagInVerbList, string* baseNameFound, int* grammaticalTenseModifier);
#endif
#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE
bool determineIfWordIsIrregularVerbContinuousCaseWrapper(string word, string* baseNameFound);
bool determineIfWordIsIrregularVerbContinuousCase(string word, GIALRPtag* firstTagInIrregularVerbList, string* baseNameFound);
#endif
#endif

#ifdef GIA_LRP_NORMALISE_PREPOSITIONS
void detectIfInverseOrTwoWayConditionRequired(string conditionName, bool* inverseConditionRequired, bool* twoWayConditionRequired, string* inverseConditionName);
#ifdef GIA_LRP_DETECT_PREPOSITION_TYPE
bool identifyConditionType(GIAentityNode* conditionEntity);
#endif
#endif

#endif

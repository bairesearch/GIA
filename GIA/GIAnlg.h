/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: GIAnlg.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1u3c 29-Sept-2013
 * Requirements: requires GIA translated data, and NLG2 to be installed
 * Description: GIA natural language generation (using NLG2)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NLG
#define HEADER_GIA_NLG

#include "GIAglobalDefs.h"
#include "GIAentityNodeClass.h"
#include "GIAtranslatorOperations.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
using namespace std;


class NLGSentence
{
public:

	NLGSentence(void);
	~NLGSentence(void);

	string NLGInputViewText;

	NLGSentence * next;
};


#ifndef GIA_USE_NLG2
	#define GIA_NLG_INDEX_IRRELEVANT (0)
#endif

#define GIA_NLG_SUPPORT_PERSON_AND_GENDER
#define GIA_NLG_SUPPORT_TIME_CONDITIONS

#define NLG_POSSESSIVE_TEXT "has"	//RELATION_GOVERNOR_COMPOSITION_3
#define NLG_POSSESSIVE_TEXT_PLURAL "have"
#define NLG_DEFINITION_TEXT "is"
#define NLG_DEFINITION_TEXT_PLURAL "are"
#define NLG_TEXT_SPACE " "

#define NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS
#define NLG_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
#define NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS


//#define NLG_TWO_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3
//#define NLG_TWO_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3_STRINGENT
#define NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3
#define NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3_STRINGENT
#define NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3a
#define NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3a
//#define NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3b
//#define NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3b
//#define NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3c
//#define NLG_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3c

#define NLG_INPUTVIEW_FEATURE_TAG_NAME_TENSE "tense"
#define NLG_INPUTVIEW_FEATURE_TAG_NAME_LEMMA "lemma"
#define NLG_INPUTVIEW_FEATURE_TAG_NAME_NOUNNUMBER "noun_number"
#define NLG_INPUTVIEW_FEATURE_TAG_NAME_POS "pos"
#define NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_DEFINITE "DEFINITE-FLAG"
#define NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_DEFINITE "T"
#define NLG_INPUTVIEW_FEATURE_TAG_NAME_GENDER "gender"
#define NLG_INPUTVIEW_FEATURE_TAG_NAME_FLAG_PERSON "person-FLAG"
#define NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FLAG_PERSON "T"
#define NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_DETERMINATE "det"
#define NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_DEFINITE "the"
#define NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_INDEFINITE "a"
#define NLG_INPUTVIEW_FEATURE_TAG_NAME_INFLECTION "inflection-TAG"
#define NLG_INPUTVIEW_FEATURE_TAG_GOVERNOR_FULLSTOP "."
#define NLG_INPUTVIEW_FEATURE_TAG_DEPENDENT_FULLSTOP "punctuation"

#ifdef GIA_USE_NLG

#define NLG_NUMBER_OF_VOWELS (5)
static char vowelArray[NLG_NUMBER_OF_VOWELS] = {'a','e','i','o','u'};

static bool nlgSentenceThreeEntitiesGenerateVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, false, false, false, false, true, true, true, true, false};
#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS (2)
static int nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS};
static bool nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_SUBSTANCE_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS] = {false, true};
static int nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[2] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT};


static bool nlgSentenceTwoEntitiesGenerateVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, true, false, true, false, false, false, false, false, false, false};

static string nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", "", "", "", "", "", RELATION_TYPE_SUBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION, RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION, ""};
/*
//static string nlgSentenceTwoEntitiesDependencyRelationGovernorVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", RELATION_TYPE_POSSESSIVE, "", RELATION_TYPE_APPOSITIVE_OF_NOUN, "", "", "", "", "", "", ""};	//not correct as differentiate between qualities and non-quality substances
//static string nlgSentenceTwoEntitiesDependencyRelationGovernorVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", "has", "", "is", "", "", "", "", "", "", ""};
//static string nlgSentenceTwoEntitiesDependencyRelationVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", RELATION_TYPE_SUBJECT", RELATION_TYPE_OBJECT", RELATION_TYPE_SUBJECT", RELATION_TYPE_OBJECT, "", "", "", "", "", ""};
*/

static string grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES] = {"undefined", ".n", ".v", "adj", "adv", "prep", "satellite"};


NLGSentence * generateLanguageFromEntityNode(GIAentityNode * entityNode, NLGSentence * currentNLGsentence, bool isQueryAnswerContext, int isQueryAnswerContextRound);
	void generateThreeEntitySentenceFromEntityNode(GIAentityNode * entityNode0, string * generatedText, int connectionType1, int connectionType2, int startEntityIndex, bool supportAdditionalLinks);
	void generateTwoEntitySentenceFromEntityConnection(GIAentityNode * entityNode1, GIAentityConnection * entityConnection, string * generatedText, int connectionType, int startEntityIndex, bool additionalLink);
		#ifdef GIA_USE_NLG2
		void NLG2generateNLGinputViewFeatureTagsGenericPerSentence(string * generatedNLGinputViewTags);
		void NLG2generateNLGinputViewFeatureTagsFromEntityNode(GIAentityNode * entityNode, int entityIndex, string * generatedNLGinputViewTags);
			string NLG2generateNLGinputViewLine(string type, string governor, string dependent);
		#else
		void addDeterminate(GIAentityNode * entityNode, string * entityTextExpanded);
			string calcDeterminate(GIAentityNode * entityNode);
		#endif

string getWordOrig(GIAentityNode * entityNode);

string determineNLGdefinitionText(GIAentityNode * entityNode);
string determineNLGpossessionText(GIAentityNode * entityNode);

#endif

#endif

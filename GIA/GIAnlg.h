/*******************************************************************************
 *
 * File Name: GIAnlg.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n5b 26-July-2012
 * Requirements: requires GIA translated data, and NLG2 to be installed
 * Description: GIA natural language generation (using NLG2)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NLG
#define HEADER_GIA_NLG

#include "GIAglobalDefs.h"
#include "GIAEntityNodeClass.h"
#include "GIATranslatorOperations.h"

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

#define NLG_POSSESSIVE_TEXT "has"	//RELATION_GOVERNOR_COMPOSITION_3
#define NLG_DEFINITION_TEXT "is"
#define NLG_TEXT_SPACE " "

#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS
#define NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
#define NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS

#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_SENTENCES_CONTAINED_THEREIN3
#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3a
#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3a
#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3b
#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3b
#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_TWO_ENTITY_SENTENCES_CONTAINED_THEREIN3c
#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_DO_NOT_READD_THREE_ENTITY_SENTENCES_CONTAINED_THEREIN3c

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


static bool nlgSentenceThreeEntitiesGenerateVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, false, false, false, false, true, true, true, true, false};
#define NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS (2)
static int nlgSentenceThreeEntitiesGenerateAdditionsVectorConnectionsArray[NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_PROPERTIES, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITIONS};
static bool nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityConnection[NLG_INPUTVIEW_THREE_ENTITY_SENTENCES_ADD_SINGLE_PROPERTY_AND_CONDITION_LINKS_NUMBER_ADDITIONAL_CONNECTIONS] = {false, true};
static int nlgSentenceThreeEntitiesGenerateAdditionsIsThreeEntityVectorConnectionsArray[2] = {GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_SUBJECT, GIA_ENTITY_VECTOR_CONNECTION_TYPE_CONDITION_OBJECT};


static bool nlgSentenceTwoEntitiesGenerateVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, true, false, true, false, false, false, false, false, false, false};

static string nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", "", "", "", "", "", RELATION_TYPE_SUBJECT, RELATION_TYPE_OBJECT, RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION, RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION, ""};
/*
//static string nlgSentenceTwoEntitiesDependencyRelationGovernorVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", RELATION_TYPE_POSSESSIVE, "", RELATION_TYPE_APPOSITIVE_OF_NOUN, "", "", "", "", "", "", ""};	//not correct as differentiate between qualities and non-quality properties
//static string nlgSentenceTwoEntitiesDependencyRelationGovernorVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", "has", "", "is", "", "", "", "", "", "", ""};
//static string nlgSentenceTwoEntitiesDependencyRelationVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", RELATION_TYPE_SUBJECT", RELATION_TYPE_OBJECT", RELATION_TYPE_SUBJECT", RELATION_TYPE_OBJECT, "", "", "", "", "", ""};
*/

static string grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES] = {"undefined", ".n", ".v", "adj", "adv", "prep", "satellite"};


NLGSentence * generateLanguageFromEntityNode(GIAEntityNode * entityNode, NLGSentence * currentNLGsentence, bool isQueryAnswerContext, int isQueryAnswerContextRound);
	void generateThreeEntitySentenceFromEntityNode(GIAEntityNode * entityNode0, string * generatedText, int connectionType1, int connectionType2, int startEntityIndex, bool supportAdditionalLinks);
	void generateTwoEntitySentenceFromEntityConnection(GIAEntityNode * entityNode0, GIAEntityConnection * entityConnection, string * generatedText, int connectionType, int startEntityIndex, bool additionalLink);
		#ifdef GIA_USE_NLG2	
		void NLG2generateNLGInputViewFeatureTagsGenericPerSentence(string * generatedNLGInputViewTags);
		void NLG2generateNLGInputViewFeatureTagsFromEntityNode(GIAEntityNode * entityNode, int entityIndex, string * generatedNLGInputViewTags);
			string NLG2generateNLGInputViewLine(string type, string governor, string dependent);
		#else
		void addDeterminate(GIAEntityNode * entityNode, string * entityTextExpanded);
			string calcDeterminate(GIAEntityNode * entityNode);
		#endif
		
string getWordOrig(GIAEntityNode * entityNode);

#endif

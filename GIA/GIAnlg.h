/*******************************************************************************
 *
 * File Name: GIAnlg.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1n4a 21-July-2012
 * Requirements: requires GIA translated data, and NLG2 to be installed
 * Description: GIA natural language generation (using NLG2)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NLG
#define HEADER_GIA_NLG

#include "GIAglobalDefs.h"
#include "GIAEntityNodeClass.h"
#include "GIATranslatorOperations.h"

#define NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_ADVERBS_AND_ADJECTIVES
#define NLG_INPUTVIEW_TWO_ENTITY_SENTENCES_SUPPORT_TWO_DEPENDENCY_RELATIONS

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

static bool nlgSentenceThreeEntitiesGenerateVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, false, false, false, false, false, true, true, true, true, false};
static bool nlgSentenceTwoEntitiesGenerateVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {false, false, false, false, true, false, true, false, false, false, false, false, false, false};

static string nlgSentenceThreeEntitiesDependencyRelationVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", "", "", "", "", "", RELATION_TYPE_PREPOSITION_SUBJECT_OF_PREPOSITION, RELATION_TYPE_PREPOSITION_OBJECT_OF_PREPOSITION, RELATION_TYPE_SUBJECT, RELATION_TYPE_OBJECT, ""};
/*
//static string nlgSentenceTwoEntitiesDependencyRelationGovernorVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", RELATION_TYPE_POSSESSIVE, "", RELATION_TYPE_APPOSITIVE_OF_NOUN, "", "", "", "", "", "", ""};	//not correct as differentiate between qualities and non-quality properties
//static string nlgSentenceTwoEntitiesDependencyRelationGovernorVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", "has", "", "is", "", "", "", "", "", "", ""};
//static string nlgSentenceTwoEntitiesDependencyRelationVectorConnectionsArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"", "", "", "", RELATION_TYPE_SUBJECT", RELATION_TYPE_OBJECT", RELATION_TYPE_SUBJECT", RELATION_TYPE_OBJECT, "", "", "", "", "", ""};
*/

static string grammaticalWordTypeCrossReferenceInflectionArray[GRAMMATICAL_WORD_TYPE_NUMBER_OF_TYPES] = {"undefined", ".n", ".v", "adj", "adv", "prep", "satellite"};

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>	//for random number generation
#include <cmath>
#include <string>
using namespace std;

void generateLanguageFromEntityNode(GIAEntityNode * entityNode, string * generatedText);
	void generateThreeEntitySentenceFromEntityNode(GIAEntityNode * entityNode, string * generatedText, int connectionType1, int connectionType2);
	void generateTwoEntitySentenceFromEntityConnection(GIAEntityNode * entityNode, GIAEntityConnection * entityConnection, string * generatedText, int connectionType);
		void generateRelexFeatureTagsGenericPerSentence(string * generatedRelexTags);
		void generateNLGInputViewFeatureTagsFromEntityNode(GIAEntityNode * entityNode, int entityIndex, string * generatedRelexTags);
			string generateNLGInputViewLine(string type, string governor, string dependent);

string generateNLGInputViewLine(string type, string governor, string dependent);

#endif

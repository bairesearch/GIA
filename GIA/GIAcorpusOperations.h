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
 * File Name: GIAcorpusOperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2b3b 22-December-2013
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_CORPUS_OPERATIONS
#define HEADER_GIA_CORPUS_OPERATIONS

#include "GIAglobalDefs.h"
#include "GIAtranslatorOperations.h"

#define FEATURE_POS_TAG_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_NUMBER_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_DETERMINER_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_UNKNOWN_ARRAY_NUMBER_OF_TYPES 6
#define FEATURE_POS_TAG_PREDETERMINER_ARRAY_NUMBER_OF_TYPES 1		//predeterminer could be merged with adjective
#define FEATURE_POS_TAG_POSSESSIVEENDING_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_MODALAUXILIARY_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_PREPOSITION_ARRAY_NUMBER_OF_TYPES 2
#define FEATURE_POS_TAG_ADJECTIVE_ARRAY_NUMBER_OF_TYPES 3
#define FEATURE_POS_TAG_NOUN_ARRAY_NUMBER_OF_TYPES 4
#define FEATURE_POS_TAG_ADVERB_ARRAY_NUMBER_OF_TYPES 3
#define FEATURE_POS_TAG_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_PARTICLE_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_INTERJECTION_ARRAY_NUMBER_OF_TYPES 1
#define FEATURE_POS_TAG_VERB_ARRAY_NUMBER_OF_TYPES 6
#define FEATURE_POS_TAG_WH_ARRAY_NUMBER_OF_TYPES 4
#define FEATURE_POS_TAG_PUNCTUATION_DIVISION_ARRAY_NUMBER_OF_TYPES 2
#define FEATURE_POS_TAG_PUNCTUATION_QUOTE_ARRAY_NUMBER_OF_TYPES 8
static string featurePOStagCoordinatingConjunctionArray[FEATURE_POS_TAG_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_COORDINATING_CONJUNCTION_CC};
static string featurePOStagNumberArray[FEATURE_POS_TAG_NUMBER_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_CARDINALNUMBER_CD};
static string featurePOStagDeterminerArray[FEATURE_POS_TAG_DETERMINER_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_DETERMINER};
static string featurePOStagUnknownArray[FEATURE_POS_TAG_UNKNOWN_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_UNKNOWN_FOREIGNWORD_FW, FEATURE_POS_TAG_UNKNOWN_EXISTENTIAL_THERE, FEATURE_POS_TAG_UNKNOWN_LISTITEMMARKER_LS, FEATURE_POS_TAG_UNKNOWN_SYMBOL_SYM, FEATURE_POS_TAG_PUNCTUATION_HASH, FEATURE_POS_TAG_PUNCTUATION_DOLLAR};
static string featurePOStagPredeterminerArray[FEATURE_POS_TAG_PREDETERMINER_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_UNKNOWN_PREDETERMINER_PDT};	//predeterminer could be merged with adjective
static string featurePOStagPossessiveEndingArray[FEATURE_POS_TAG_POSSESSIVEENDING_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_UNKNOWN_POSSESSIVEENDING_POS};
static string featurePOStagModalAuxiliaryArray[FEATURE_POS_TAG_MODALAUXILIARY_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_MODALAUXILIARY_MD};
static string featurePOStagPrepositionArray[FEATURE_POS_TAG_PREPOSITION_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PREPOSITION_IN, FEATURE_POS_TAG_PREPOSITION_TO};
static string featurePOStagAdjectiveArray[FEATURE_POS_TAG_ADJECTIVE_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_ADJECTIVE_JJ, FEATURE_POS_TAG_ADJECTIVE_COMPARATIVE_JJR, FEATURE_POS_TAG_ADJECTIVE_SUPERLATIVE_JJS};
static string featurePOStagNounArray[FEATURE_POS_TAG_NOUN_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_NOUN_NN, FEATURE_POS_TAG_NOUN_NNS, FEATURE_POS_TAG_NOUN_NNP, FEATURE_POS_TAG_NOUN_NNPS};
static string featurePOStagAdverbArray[FEATURE_POS_TAG_ADVERB_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_ADVERB, FEATURE_POS_TAG_ADVERB_COMPARATIVE, FEATURE_POS_TAG_ADVERB_SUPERLATIVE};
static string featurePOStagPronounPersonalArray[FEATURE_POS_TAG_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PRONOUN_PERSONAL_PRONOUN_PRP};
static string featurePOStagPronounPossessiveArray[FEATURE_POS_TAG_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PRONOUN_POSSESSIVE_PRONOUN_PPDOLLAR};
static string featurePOStagParticleArray[FEATURE_POS_TAG_PARTICLE_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PARTICLE_RP};
static string featurePOStagInterjectionArray[FEATURE_POS_TAG_INTERJECTION_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_INTERJECTION_UH};
static string featurePOStagVerbArray[FEATURE_POS_TAG_VERB_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_VERB_VB, FEATURE_POS_TAG_VERB_VBD, FEATURE_POS_TAG_VERB_VBG, FEATURE_POS_TAG_VERB_VBN, FEATURE_POS_TAG_VERB_VBP, FEATURE_POS_TAG_VERB_VBZ};
static string featurePOStagWhArray[FEATURE_POS_TAG_WH_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_WH_DETERMINER_WDT, FEATURE_POS_TAG_WH_PRONOUN_WP, FEATURE_POS_TAG_WH_POSSESIVEPRONOUN_WPDOLLAR, FEATURE_POS_TAG_WH_ADVERB_WRB};
static string featurePOStagPunctuationDivisionArray[FEATURE_POS_TAG_PUNCTUATION_DIVISION_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PUNCTUATION_DIVISION_COMMA, FEATURE_POS_TAG_PUNCTUATION_DIVISION_COLON_OR_SEMICOLON};
static string featurePOStagPunctuationQuoteArray[FEATURE_POS_TAG_PUNCTUATION_QUOTE_ARRAY_NUMBER_OF_TYPES] = {FEATURE_POS_TAG_PUNCTUATION_QUOTE_FULLSTOP, FEATURE_POS_TAG_PUNCTUATION_QUOTE_LEFTBRACKET, FEATURE_POS_TAG_PUNCTUATION_QUOTE_RIGHTBRACKET, FEATURE_POS_TAG_PUNCTUATION_QUOTE_STRAIGHTDOUBLEQUOTE, FEATURE_POS_TAG_PUNCTUATION_QUOTE_LEFTOPENSINGLEQUOTE, FEATURE_POS_TAG_PUNCTUATION_QUOTE_LEFTOPENDOUBLEQUOTE, FEATURE_POS_TAG_PUNCTUATION_QUOTE_RIGHTCLOSESINGLEQUOTE, FEATURE_POS_TAG_PUNCTUATION_QUOTE_RIGHTCLOSEDOUBLEQUOTE};

//http://en.wikipedia.org/wiki/English_auxiliaries_and_contractions
#define ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES (5)
#define ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES (3)
#define ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES (3)
static string entityAuxiliaryBeingArray[ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES] = {"am", "is", "are", "was", "were"};
static string entityAuxiliaryHavingArray[ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES] = {"have", "has", "had"};
static string entityAuxiliaryDoingArray[ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES] = {"do", "does", "did"};

//additional cases not identified by [/mapped to existing] Relex Word Type:
#define ENTITY_WH_ARRAY_NUMBER_OF_TYPES (9)
static string entityWhArray[ENTITY_WH_ARRAY_NUMBER_OF_TYPES] = {"which", "what", "who", "whom", "whose","where","when","how","why"};	//http://courses.washington.edu/hypertxt/csar-v02/penntable.html + http://www.computing.dcu.ie/~acahill/tagset.html
#define ENTITY_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES (7)
static string entityCoordinatingConjunctionArray[ENTITY_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES] = {"and", "but", "or", "yet", "nor"};	//removed "for" +  "so" as conj is not generated for these by Stanford //http://grammar.ccc.commnet.edu/grammar/conjunctions.htm
	/*
	Tom rides the bike so he can go to lunch.
	Tom likes the chicken but prefers the apple.
	The ball is neither blue nor red.
	The ball is tall yet fast.
	*/
#define ENTITY_POSSESSIVEENDING_NUMBER_OF_TYPES (2)
static string entityPossessiveEndingArray[ENTITY_POSSESSIVEENDING_NUMBER_OF_TYPES] = {"'s", "'"};
#define ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES (9)
static string entityModalAuxiliaryArray[ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES] = {"can", "could", "may", "might", "must", "shall", "should", "will", "would"};	//en.wikipedia.org/wiki/Modal_verb
#define ENTITY_CARDINALNUMBER_ARRAY_NUMBER_OF_TYPES (43)
static string entityCardinalNumberArray[ENTITY_CARDINALNUMBER_ARRAY_NUMBER_OF_TYPES] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9","zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine","ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety", "hundred", "thousand", "million", "billion", "trillion"};
#define ENTITY_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES (16)
static string entityPronounPersonalArray[ENTITY_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES] = {"I", "you", "he", "she", "it", "me", "you", "him", "her", "it", "we", "you", "they", "us", "you", "them"};			//http://grammar.ccc.commnet.edu/grammar/cases.htm#cases
#define ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES (14)
static string entityPronounPossessiveArray[ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES] = {"my", "mine", "your", "yours", "his", "her", "hers", "its", "our", "ours", "your", "yours", "their", "theirs"};	//http://grammar.ccc.commnet.edu/grammar/cases.htm#cases
#define ENTITY_PREDETERMINER_ARRAY_NUMBER_OF_TYPES (13)
static string entityPredeterminerArray[ENTITY_PREDETERMINER_ARRAY_NUMBER_OF_TYPES] = {"all", "both", "double", "half", "quadruple", "quite", "rather", "such", "times", "treble", "twice", "quarter", "many"};	//removed "what"	//http://englishwithyeasir.blogspot.com.au/2013/05/predeterminers.html
	//predeterminer could be merged with adjective

//not required for Stanford CoreNLP as "an" lemma is "a" (but is required for Relex)
#define GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES (2)	
static string grammaticalDeterminerIndefiniteArray[GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES] = {GRAMMATICAL_DETERMINER_INDEFINITE, GRAMMATICAL_DETERMINER_INDEFINITE_FIRST_LETTER_VOWEL};


#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_UNDEFINED 0		//added in case Stanford POS extraction does not equate exactly to PENN tree bank specification
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_COORDINATINGCONJUNCTION 1
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_NUMBER 2
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_DETERMINER 3
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_UNKNOWN 4
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_PREDETERMINER 5	//predeterminer could be merged with adjective
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_POSSESSIVEENDING 6
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_MODALAUXILIARY 7
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_PREPOSITION 8
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_ADJECTIVE 9
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_NOUN 10
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_ADVERB 11
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_PRONOUN_PERSONAL 12
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_PRONOUN_POSSESSIVE 13
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_PARTICLE 14
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_INTERJECTION 15
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_VERB 16
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_WH 17
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_PUNCTUATION_DIVISION 18
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_PUNCTUATION_QUOTE 19
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_BEING 20		//additional case required for GIA semantics extraction
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_HAVING 21		//additional case required for GIA semantics extraction
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_DOING 22		//additional case required for GIA semantics extraction
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES (23)
static string GIAconnectionistNetworkPOStypeNameArray[GIA_CONNECTIONIST_NETWORK_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES] = {"undefined", "coordinatingConjunction", "number", "determiner", "unknown", "predeterminer", "posessiveEnding", "modalAuxiliary", "preposition", "adjective", "noun", "adverb", "pronounPersonal", "pronounPossessive", "interjection", "verb", "wh", "punctuationDivision", "punctuationQuote", "isAuxillaryBeing", "isAuxillaryHaving", "isAuxillaryDoing"};
static int featureRelexPOStypeCrossReferenceGIAconnectionistNetworkPOStypeArray[FEATURE_RELEX_POS_NUMBER_OF_TYPES] = {GIA_CONNECTIONIST_NETWORK_POS_TYPE_UNDEFINED, GIA_CONNECTIONIST_NETWORK_POS_TYPE_ADJECTIVE, GIA_CONNECTIONIST_NETWORK_POS_TYPE_ADVERB, GIA_CONNECTIONIST_NETWORK_POS_TYPE_PUNCTUATION_DIVISION, GIA_CONNECTIONIST_NETWORK_POS_TYPE_DETERMINER, GIA_CONNECTIONIST_NETWORK_POS_TYPE_NOUN, GIA_CONNECTIONIST_NETWORK_POS_TYPE_PARTICLE, GIA_CONNECTIONIST_NETWORK_POS_TYPE_PREPOSITION, GIA_CONNECTIONIST_NETWORK_POS_TYPE_PUNCTUATION_QUOTE, GIA_CONNECTIONIST_NETWORK_POS_TYPE_VERB, GIA_CONNECTIONIST_NETWORK_POS_TYPE_UNKNOWN};

/*
//requires updating (add more cases from PENN tree)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_NOUN (0)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_VERB_OR_PARTICIPLE (1)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_ADJECTIVE_OR_ADVERB (2)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_PREPOSITION (3)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_BEING (4)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_HAVING (5)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_DOING (6)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_MODAL (7)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_DETERMINER_THATWHICH (8)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_DETERMINER (9)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_COMMA (10)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_CONJUNCTION (11)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_INTERJECTION (12)
#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES (13)
static string GIAconnectionistNetworkPOStypeNameArray[GIA_CONNECTIONIST_NETWORK_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES] = {"isNoun", "isVerbOrParticiple", "isAdjectiveOrAdverb", "isPreposition", "isAuxillaryBeing", "isAuxillaryHaving", "isAuxillaryDoing", "isAuxillaryModal", "isDeterminerThatWhich", "isDeterminer", "isComma", "isConjunction", "isInterjection"};
*/

#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_DETERMINER (14)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_COMPOSITION_AUXILIARY (15)
#define GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA (16)
#define GIA2_SEMANTIC_DEPENDENCY_RELATION_NUMBER_OF_TYPES (GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES + 3)	//extends GIAentityNodeClass.h GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES

//the following definitions must map to relationGovernorCompositionNameArray[]:
#define RELATION_GOVERNOR_COMPOSITION_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES (3)
static string relationGovernorCompositionPastTenseNameArray[RELATION_GOVERNOR_COMPOSITION_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES] = {"contained", "comprised", "had"};	
#define RELATION_GOVERNOR_COMPOSITION_CONTINUOUS_TENSE_NAME_ARRAY_NUMBER_OF_TYPES (3)
static string relationGovernorCompositionContinuousTenseNameArray[RELATION_GOVERNOR_COMPOSITION_CONTINUOUS_TENSE_NAME_ARRAY_NUMBER_OF_TYPES] = {"containing", "comprised" "having"};	
#define RELATION_AUXILIARY_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES (4)
static string relationAuxiliaryPastTenseNameArray[RELATION_AUXILIARY_PAST_TENSE_NAME_ARRAY_NUMBER_OF_TYPES] = {"was", "were", "had", "did"};
#define RELATION_AUXILIARY_FUTURE_TENSE_NAME_ARRAY_NUMBER_OF_TYPES (1)
static string relationAuxiliaryFutureTenseNameArray[RELATION_AUXILIARY_FUTURE_TENSE_NAME_ARRAY_NUMBER_OF_TYPES] = {"will"};	//FUTURE: take into account all principal modal verbs; can, could, may, might, must, shall, should, will, would
//must use LRP to determine continuous tense..

#ifdef GIA_USE_CORPUS_DATABASE
static string GIA2semanticDependencyRelationNameArray[GIA2_SEMANTIC_DEPENDENCY_RELATION_NUMBER_OF_TYPES] = {"actionSubject", "actionObject", "conditionSubject", "conditionObject", "property", "property", "definition", "definition", "instance", "actionSubject", "actionObject", "conditionSubject", "conditionObject", "instance", "determiner", "compositionAuxiliary", "modalAuxiliaryOrCopula"};
//static string GIA2semanticDependencyRelationNameArray[GIA_ENTITY_NUMBER_OF_VECTOR_CONNECTION_TYPES] = {"actionSubject", "actionObject", "conditionSubject", "conditionObject", "property", "property", "definition", "definition", "instance", "actionSubject", "actionObject", "conditionSubject", "conditionObject", "instance"};	
void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNode ** GIAentityNodeArray, Sentence * currentSentenceInList, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet);
void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(GIAentityNode ** GIAentityNodeArray, Sentence * currentSentenceInList, bool linkPreestablishedReferencesGIA, int NLPdependencyRelationsType);
	string generateGIA2semanticDependencyRelation(GIAentityNode ** GIAentityNodeArray, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet);
		string createSameReferenceSetRecord(bool sameReferenceSet);
string regenerateSentenceText(Feature * firstFeatureInSentence, bool addPOSinfo, int NLPfeatureParser);
void determineGIAconnectionistNetworkPOStypeNames(Feature * firstFeatureInList, int NLPfeatureParser);
	void determineGIAconnectionistNetworkPOStypeNameStanford(Feature * currentFeatureInSentence);
	void determineGIAconnectionistNetworkPOStypeNameRelex(Feature * currentFeatureInSentence);
#endif

#endif

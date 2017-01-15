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
 * File Name: GIAcorpusOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2i36a 19-March-2015
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#include "GIAcorpusOperations.h"
#include "GIAcorpusDatabase.h"

#ifndef LINUX
	#include <windows.h>
#endif


#ifdef GIA_USE_CORPUS_DATABASE

void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNode** GIAentityNodeArray, GIAsentence* currentSentenceInList, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	//cout << "GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain: " << endl;
	string GIA2semanticDependencyRelation = generateGIA2semanticDependencyRelation(GIAentityNodeArray, connectionType, entityIndex1, entityIndex2, sameReferenceSet);
	saveTextLineToCurrentCorpusFile(GIA2semanticDependencyRelation);
	cout << GIA2semanticDependencyRelation << endl;
}

//this function [recording aux/cop/det syntatical dependency relations] is required to extract tense and perform instance/concept identification once GIA2 lookup has been performed: it is only currently supported by Stanford parser
void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(GIAentityNode** GIAentityNodeArray, GIAsentence* currentSentenceInList, bool linkPreestablishedReferencesGIA, int NLPdependencyRelationsType)
{
	#ifndef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
	if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
	#endif
		if(!linkPreestablishedReferencesGIA)
		{
			GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;

			while(currentRelationInList->next != NULL)
			{
				int entityIndex1 = currentRelationInList->relationGovernorIndex;
				int entityIndex2 = currentRelationInList->relationDependentIndex;
				bool sameReferenceSet =	false;	//irrelevant

				/*
				if(!(currentRelationInList->disabled))
				{
				*/
				//NB these must correspond to GIA2syntacticDependencyRelationSecondaryNameArray/GIA2_SYNTACTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES:

				if(currentRelationInList->relationType == RELATION_TYPE_MODAL_AUX)	//same as auxiliary
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, entityIndex1, entityIndex2, sameReferenceSet);
				}

				if(currentRelationInList->relationType == RELATION_TYPE_PASSIVE_AUX)
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, entityIndex1, entityIndex2, sameReferenceSet);
				}

				if(currentRelationInList->relationType == RELATION_TYPE_COPULA)
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, entityIndex1, entityIndex2, sameReferenceSet);
				}

				if(currentRelationInList->relationType == RELATION_TYPE_DETERMINER)
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DETERMINER, entityIndex1, entityIndex2, sameReferenceSet);
				}
				/*
				}
				*/

				currentRelationInList = currentRelationInList->next;
			}
		}
	#ifndef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
	}
	else
	{
		cout << "GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial() error: function currently requires Stanford parser/CoreNLP" << endl;
	}
	#endif
}

string generateGIA2semanticDependencyRelation(GIAentityNode** GIAentityNodeArray, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	#ifdef GIA2_SUPPORT_QUERIES
	string entityWord1 = GIAentityNodeArray[entityIndex1]->entityName;
	string entityWord2 = GIAentityNodeArray[entityIndex2]->entityName;

	//cout << "a1" << endl;
	string entityWord1Query = "";
	if(GIAentityNodeArray[entityIndex1]->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
	{
		entityWord1Query = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
	}
	if(GIAentityNodeArray[entityIndex1]->isNameQuery)
	{
		cout << "isNameQuery" << endl;
		entityWord1 = entityWord1Query + GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME;
	}
	else if(GIAentityNodeArray[entityIndex1]->isWhichOrEquivalentWhatQuery)
	{
		cout << "isWhichOrEquivalentWhatQuery" << endl;
		entityWord1 = entityWord1Query + GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME;
	}
	else if(GIAentityNodeArray[entityIndex1]->isQuery)
	{
		cout << "isQuery" << endl;
		entityWord1 = entityWord1Query + GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME;
	}

	string entityWord2Query = "";
	if(GIAentityNodeArray[entityIndex2]->entityName == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
	{
		entityWord2Query = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
	}
	if(GIAentityNodeArray[entityIndex2]->isNameQuery)
	{
		cout << "isNameQuery" << endl;
		entityWord2 = entityWord2Query + GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME;
	}
	else if(GIAentityNodeArray[entityIndex2]->isWhichOrEquivalentWhatQuery)
	{
		cout << "isWhichOrEquivalentWhatQuery" << endl;
		entityWord2 = entityWord2Query + GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME;
	}
	else if(GIAentityNodeArray[entityIndex2]->isQuery)
	{
		cout << "isQuery" << endl;
		entityWord2 = entityWord2Query + GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME;
	}
	#else
	string entityWord1 = GIAentityNodeArray[entityIndex1]->wordOrig;
	string entityWord2 = GIAentityNodeArray[entityIndex2]->wordOrig;
	//cout << "entityWord1 = " << entityWord1 << endl;
	//cout << "entityWord2 = " << entityWord2 << endl;
	//lemmas are in general not recorded as they are irrelevant (wordOrig is only recorded for debugging purposes and internal/manual/inhouse development of the corpus); only the entity indicies require recording
	if(entityWord1 == "")
	{//why does GIAentityNodes in GIAentityNodeArray that correspond to prepositions not have a "wordOrig" but only have an entityName? (is it related to LRP?)
		entityWord1 = GIAentityNodeArray[entityIndex1]->entityName;
	}
	if(entityWord2 == "")
	{//why does GIAentityNodes in GIAentityNodeArray that correspond to prepositions not have a "wordOrig" but only have an entityName? (is it related to LRP?)
		entityWord2 = GIAentityNodeArray[entityIndex2]->entityName;
	}
	#endif
	//cout << "a2" << endl;

	string GIA2semanticDependencyRelation = "";
	GIA2semanticDependencyRelation = generateGIA2semanticDependencyRelationSimple(entityWord1, entityWord2, GIA2semanticDependencyRelationNameArray[connectionType], entityIndex1, entityIndex2, sameReferenceSet);
	//cout << "GIA2semanticDependencyRelation = " << GIA2semanticDependencyRelation << endl;
	return GIA2semanticDependencyRelation;
}

string generateGIA2semanticDependencyRelationSimple(string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	//cout << "a2" << endl;

	string GIA2semanticDependencyRelation = "";
	GIA2semanticDependencyRelation = GIA2semanticDependencyRelation + semanticRelation + "(" + entityName1 + "-" + convertIntToString(entityIndex1) + ", " + entityName2 + "-" + convertIntToString(entityIndex2) + ") " + createSameReferenceSetRecord(sameReferenceSet);
	//cout << "GIA2semanticDependencyRelation = " << GIA2semanticDependencyRelation << endl;
	return GIA2semanticDependencyRelation;
}

string createSameReferenceSetRecord(bool sameReferenceSet)
{
	string sameReferenceSetRecord = "[sameReferenceSet=" + convertBoolToString(sameReferenceSet) + "]";
	return sameReferenceSetRecord;
}

//preconditions: determineGIAconnectionistNetworkPOStypeNames() has been executed
string regenerateSentenceText(GIAfeature* firstFeatureInSentence, bool addPOSinfo, int NLPfeatureParser)
{
	//cout << "regenerateSentenceText1" << endl;
	string sentenceText = "";
	GIAfeature* currentFeatureInSentence = firstFeatureInSentence;
	while(currentFeatureInSentence->next != NULL)
	{
		if(addPOSinfo)
		{
			string GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[currentFeatureInSentence->GIAconnectionistNetworkPOStype];
			sentenceText = sentenceText + currentFeatureInSentence->word + CHAR_FORWARDSLASH + GIAconnectionistNetworkPOStypeName + " ";	//~Stanford Parser format; see parseStanfordParserFile():GIATHparseStanfordParseWordsAndPOStagsText()
		}
		else
		{
			sentenceText = sentenceText + currentFeatureInSentence->word + " ";
		}
		currentFeatureInSentence = currentFeatureInSentence->next;
	}
	//cout << "regenerateSentenceText2" << endl;
	return sentenceText;
}

void determineGIAconnectionistNetworkPOStypeNames(GIAfeature* firstFeatureInList, int NLPfeatureParser)
{
	GIAfeature* currentFeatureInSentence = firstFeatureInList;
	while(currentFeatureInSentence->next != NULL)
	{
		//cout << "currentFeatureInSentence->word = " << currentFeatureInSentence->word << endl;
		if((NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP) || (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER))
		{
			determineGIAconnectionistNetworkPOStypeNameStanford(currentFeatureInSentence);
		}
		else if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
			#ifndef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
			cout << "warning: determineGIAconnectionistNetworkPOStypeNames() is supported, but Relex cannot generate det and aux syntactic relations (required to be stored by GIA connectionist network as 'GIA semantic relations' to reextract instances/substances and tense)" << endl;
			#endif
			determineGIAconnectionistNetworkPOStypeNameRelex(currentFeatureInSentence);
		}

		currentFeatureInSentence = currentFeatureInSentence->next;
	}
}

void determineGIAconnectionistNetworkPOStypeNameStanford(GIAfeature* currentFeatureInSentence)
{
	int GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_UNDEFINED;
	string GIAconnectionistNetworkPOStypeName = "";

	for(int i=0; i<FEATURE_POS_TAG_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagCoordinatingConjunctionArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_COORDINATINGCONJUNCTION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_NUMBER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagNumberArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_NUMBER;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_DETERMINER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagDeterminerArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_DETERMINER;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_UNKNOWN_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagUnknownArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_UNKNOWN;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PREDETERMINER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPredeterminerArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PREDETERMINER;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_POSSESSIVEENDING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPossessiveEndingArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_POSSESSIVEENDING;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_MODALAUXILIARY_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagModalAuxiliaryArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_MODALAUXILIARY;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PREPOSITION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPrepositionArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PREPOSITION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_ADJECTIVE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagAdjectiveArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_ADJECTIVE;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_NOUN_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagNounArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_NOUN;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_ADVERB_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagAdverbArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_ADVERB;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPronounPersonalArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PRONOUN_PERSONAL;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPronounPossessiveArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PRONOUN_POSSESSIVE;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PARTICLE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagParticleArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PARTICLE;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_INTERJECTION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagInterjectionArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_INTERJECTION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_VERB_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagVerbArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_VERB;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_WH_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagWhArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_WH;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PUNCTUATION_DIVISION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPunctuationDivisionArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PUNCTUATION_DIVISION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PUNCTUATION_QUOTE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPunctuationQuoteArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PUNCTUATION_QUOTE;
		}
	}
	//additional cases required for GIA semantics extraction;
	for(int i=0; i<ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryBeingArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_BEING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryHavingArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_HAVING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryDoingArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_DOING;
		}
	}
	#ifdef GIA2_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
	for(int i=0; i<GRAMMATICAL_DETERMINER_LIMITED_INDEFINITE_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == grammaticalDeterminerIndefiniteArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_DETERMINER_LIMITED_INDEFINITE;
		}
	}
	#endif

	/*
	//requires updating (add more cases from PENN tree above)
	if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_NOUN)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_CONNECTIONIST_NETWORK_POS_TYPE_NOUN];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_VERB)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_CONNECTIONIST_NETWORK_POS_TYPE_VERB_OR_PARTICIPLE];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_ADJ)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_CONNECTIONIST_NETWORK_POS_TYPE_ADJECTIVE_OR_ADVERB];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_ADV)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_CONNECTIONIST_NETWORK_POS_TYPE_ADJECTIVE_OR_ADVERB];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_PREP)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_CONNECTIONIST_NETWORK_POS_TYPE_PREPOSITION];
	}
	*/

	currentFeatureInSentence->GIAconnectionistNetworkPOStype = GIAconnectionistNetworkPOStype;
	//cout << "GIAconnectionistNetworkPOStype = " << GIAconnectionistNetworkPOStype << endl;

}


void determineGIAconnectionistNetworkPOStypeNameRelex(GIAfeature* currentFeatureInSentence)
{
	int GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_UNDEFINED;
	//cout << "currentFeatureInSentence->type = " << currentFeatureInSentence->type << endl;
	for(int i=0; i<FEATURE_RELEX_POS_NUMBER_OF_TYPES; i++)
	{
		if(featureRelexPOStypeArray[i] == currentFeatureInSentence->type)
		{
			//cout << "featureRelexPOStypeArray[i] = " << featureRelexPOStypeArray[i] << endl;
			//cout << "i = " << i << endl;
			//cout << "featureRelexPOStypeCrossReferenceGIAconnectionistNetworkPOStypeArray[i] = " << featureRelexPOStypeCrossReferenceGIAconnectionistNetworkPOStypeArray[i] << endl;
			GIAconnectionistNetworkPOStype = featureRelexPOStypeCrossReferenceGIAconnectionistNetworkPOStypeArray[i];
		}
	}

	//additional cases not identified by [/mapped to existing] Relex Word Type:
	for(int i=0; i<ENTITY_WH_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityWhArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_WH;
		}
	}
	for(int i=0; i<ENTITY_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityCoordinatingConjunctionArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_COORDINATINGCONJUNCTION;
		}
	}
	for(int i=0; i<ENTITY_POSSESSIVEENDING_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPossessiveEndingArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_POSSESSIVEENDING;
		}
	}
	for(int i=0; i<ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityModalAuxiliaryArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_MODALAUXILIARY;
		}
	}
	for(int i=0; i<ENTITY_CARDINALNUMBER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if((currentFeatureInSentence->lemma).find(entityCardinalNumberArray[i]) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_NUMBER;
		}
	}
	for(int i=0; i<ENTITY_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPronounPersonalArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PRONOUN_PERSONAL;
		}
	}
	for(int i=0; i<ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPronounPossessiveArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PRONOUN_POSSESSIVE;
		}
	}
	for(int i=0; i<ENTITY_PREDETERMINER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPredeterminerArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_PREDETERMINER;
		}
	}

	//GIA_CONNECTIONIST_NETWORK_POS_TYPE_INTERJECTION not currently supported by Relex

	//additional cases required for GIA semantics extraction;
	for(int i=0; i<ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryBeingArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_BEING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryHavingArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_HAVING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryDoingArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_AUXILIARY_DOING;
		}
	}

	#ifdef GIA2_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
	for(int i=0; i<GRAMMATICAL_DETERMINER_LIMITED_INDEFINITE_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == grammaticalDeterminerIndefiniteArray[i])
		{
			GIAconnectionistNetworkPOStype = GIA_CONNECTIONIST_NETWORK_POS_TYPE_DETERMINER_LIMITED_INDEFINITE;
		}
	}
	#endif

	currentFeatureInSentence->GIAconnectionistNetworkPOStype = GIAconnectionistNetworkPOStype;
	//cout << "GIAconnectionistNetworkPOStype = " << GIAconnectionistNetworkPOStype << endl;
}


#endif





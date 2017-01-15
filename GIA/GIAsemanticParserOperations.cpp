/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is commercially licensed software. It may not be redistributed,
 * and may only be used by a registered licensee.
 *
 * BAIPROJECT is sold in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * BAI Commercial License for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You will have received a copy of the BAI Commercial License
 * along with BAIPROJECT. For more information, visit <http://www.baxterai.com>
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemanticParserOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2m4a 06-September-2016
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#include "GIAsemanticParserOperations.h"
#include "GIAsemanticParserDatabase.h"

#ifndef LINUX
	#include <windows.h>
#endif


#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER

void GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNode** GIAentityNodeArray, GIAsentence* currentSentenceInList, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	#ifdef GIA_DEBUG
	//cout << "GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain: " << endl;
	#endif
	string GIA2semanticDependencyRelationText = "";
	GIArelation* GIA2semanticDependencyRelation = getCurrentRelationInSemanticParserSentenceList();
	generateGIA2semanticDependencyRelation(GIAentityNodeArray, connectionType, entityIndex1, entityIndex2, sameReferenceSet, &GIA2semanticDependencyRelationText, GIA2semanticDependencyRelation);
	#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD
	saveTextLineToCorpusFileString(GIA2semanticDependencyRelation);
	cout << GIA2semanticDependencyRelationText << endl;
	#else
	GIA2semanticDependencyRelation->next = new GIArelation();
	GIA2semanticDependencyRelation = GIA2semanticDependencyRelation->next;
	#endif
}

/*
void GIA2nonHeuristicImplementationRemoveExperiencesForConnectionistNetworkTrain(GIAentityNode** GIAentityNodeArray, GIAsentence* currentSentenceInList, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	#ifdef GIA_DEBUG
	//cout << "GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain: " << endl;
	#endif
	string GIA2semanticDependencyRelation = generateGIA2semanticDependencyRelation(GIAentityNodeArray, connectionType, entityIndex1, entityIndex2, sameReferenceSet);
	removeTextLineFromCorpusFileString(GIA2semanticDependencyRelation);
	cout << GIA2semanticDependencyRelation << endl;
}
*/

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

				#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA2_SEMANTIC_RELATION_GENERATION
				if(currentRelationInList->relationType == RELATION_TYPE_PRENOMIAL_MODIFIER)	//must correspond to RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES/relationTypePrenominalModifierNameArray
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(GIAentityNodeArray, currentSentenceInList, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PRENOMINAL_MODIFIER, entityIndex1, entityIndex2, sameReferenceSet);
				}
				#endif
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
		cout << "GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial{} error: function currently requires Stanford parser/CoreNLP" << endl;
	}
	#endif
}

void generateGIA2semanticDependencyRelation(GIAentityNode** GIAentityNodeArray, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet, string* GIA2semanticDependencyRelationText, GIArelation* GIA2semanticDependencyRelation)
{
	#ifdef GIA2_SUPPORT_QUERIES
	string entityWord1 = GIAentityNodeArray[entityIndex1]->entityName;
	string entityWord2 = GIAentityNodeArray[entityIndex2]->entityName;

	//NB GIA2:GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE does not support queries/GIA2_SUPPORT_QUERIES (ie the special entity name adjustments below)
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

	//NB GIA2:GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE does not support queries/GIA2_SUPPORT_QUERIES (ie the special entity name adjustments below)
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
	#ifdef GIA_DEBUG
	//cout << "entityWord1 = " << entityWord1 << endl;
	//cout << "entityWord2 = " << entityWord2 << endl;
	#endif
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

	#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD
	*GIA2semanticDependencyRelation = generateGIA2semanticDependencyRelationText(entityWord1, entityWord2, GIA2semanticDependencyRelationNameArray[connectionType], entityIndex1, entityIndex2, sameReferenceSet);
	#ifdef GIA_DEBUG
	//cout << "GIA2semanticDependencyRelation = " << GIA2semanticDependencyRelation << endl;
	#endif
	#else
	generateGIA2semanticDependencyRelationObject(GIA2semanticDependencyRelation, entityWord1, entityWord2, GIA2semanticDependencyRelationNameArray[connectionType], entityIndex1, entityIndex2, sameReferenceSet);
	#endif
}

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
string generateGIA2semanticDependencyRelationText(string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	string GIA2semanticDependencyRelation = "";
	GIA2semanticDependencyRelation = GIA2semanticDependencyRelation + semanticRelation + "(" + entityName1 + "-" + convertIntToString(entityIndex1) + ", " + entityName2 + "-" + convertIntToString(entityIndex2) + ") " + createSameReferenceSetRecord(sameReferenceSet);
	#ifdef GIA_DEBUG
	//cout << "GIA2semanticDependencyRelation = " << GIA2semanticDependencyRelation << endl;
	#endif
	return GIA2semanticDependencyRelation;
}

string createSameReferenceSetRecord(bool sameReferenceSet)
{
	string sameReferenceSetRecord = "[sameReferenceSet=" + convertBoolToString(sameReferenceSet) + "]";
	return sameReferenceSetRecord;
}

//preconditions: determineGIAconnectionistNetworkPOStypeNames() has been executed
string generateCorpusFileHeaderText(GIAfeature* firstFeatureInSentence, bool addPOSinfo)
{
	#ifdef GIA_DEBUG
	//cout << "generateCorupusFileHeaderText{}:" << endl;
	#endif
	string sentenceText = "";
	GIAfeature* currentFeatureInSentence = firstFeatureInSentence;
	while(currentFeatureInSentence->next != NULL)
	{
		string appendString = " ";
		if(currentFeatureInSentence->next->next == NULL)
		{
			appendString = "";
		}

		if(addPOSinfo)
		{
			string GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[currentFeatureInSentence->GIAsemanticParserPOStype];
			sentenceText = sentenceText + currentFeatureInSentence->word + CHAR_FORWARDSLASH + GIAconnectionistNetworkPOStypeName + appendString;	//~Stanford Parser format; see parseStanfordParserFile{}:GIATHparseStanfordParseWordsAndPOStagsText()
		}
		else
		{
			sentenceText = sentenceText + currentFeatureInSentence->word + appendString;
		}
		currentFeatureInSentence = currentFeatureInSentence->next;
	}
	return sentenceText;
}

#endif


void determineGIAconnectionistNetworkPOStypeNames(GIAfeature* firstFeatureInList, int NLPfeatureParser)
{
	#ifdef GIA_DEBUG
	//cout << "\n\n\ndetermineGIAconnectionistNetworkPOStypeNames{}:" << endl;
	#endif
	GIAfeature* currentFeatureInSentence = firstFeatureInList;
	while(currentFeatureInSentence->next != NULL)
	{
		#ifdef GIA_DEBUG
		//cout << "currentFeatureInSentence->word = " << currentFeatureInSentence->word << endl;
		#endif
		if((NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP) || (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER))
		{
			determineGIAconnectionistNetworkPOStypeNameStanford(currentFeatureInSentence);
			#ifdef GIA_DEBUG
			//cout << "currentFeatureInSentence->GIAsemanticParserPOStype = " << currentFeatureInSentence->GIAsemanticParserPOStype << endl;
			#endif
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
	int GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_UNDEFINED;

	for(int i=0; i<FEATURE_POS_TAG_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagCoordinatingConjunctionArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_COORDINATINGCONJUNCTION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_NUMBER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagNumberArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_NUMBER;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_DETERMINER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagDeterminerArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_DETERMINER;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_UNKNOWN_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagUnknownArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_UNKNOWN;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PREDETERMINER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPredeterminerArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PREDETERMINER;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_POSSESSIVEENDING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPossessiveEndingArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_POSSESSIVEENDING;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_MODALAUXILIARY_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagModalAuxiliaryArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_MODALAUXILIARY;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PREPOSITION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPrepositionArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PREPOSITION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_ADJECTIVE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagAdjectiveArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_ADJECTIVE;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_NOUN_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagNounArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_NOUN;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_ADVERB_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagAdverbArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_ADVERB;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPronounPersonalArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PRONOUN_PERSONAL;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPronounPossessiveArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PRONOUN_POSSESSIVE;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PARTICLE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagParticleArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PARTICLE;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_INTERJECTION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagInterjectionArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_INTERJECTION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_VERB_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagVerbArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_VERB;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_WH_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagWhArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_WH;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PUNCTUATION_DIVISION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPunctuationDivisionArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PUNCTUATION_DIVISION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PUNCTUATION_QUOTE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPunctuationQuoteArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PUNCTUATION_QUOTE;
		}
	}
	//additional cases required for GIA semantics extraction;
	for(int i=0; i<ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryBeingArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_BEING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryHavingArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_HAVING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryDoingArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_DOING;
		}
	}
	#ifdef GIA2_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
	for(int i=0; i<GRAMMATICAL_DETERMINER_LIMITED_INDEFINITE_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == grammaticalDeterminerIndefiniteArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_DETERMINER_LIMITED_INDEFINITE;
		}
	}
	#endif

	/*
	//requires updating (add more cases from PENN tree above)
	if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_NOUN)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEMANTIC_PARSER_POS_TYPE_NOUN];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_VERB)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEMANTIC_PARSER_POS_TYPE_VERB_OR_PARTICIPLE];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_ADJ)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEMANTIC_PARSER_POS_TYPE_ADJECTIVE_OR_ADVERB];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_ADV)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEMANTIC_PARSER_POS_TYPE_ADJECTIVE_OR_ADVERB];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_PREP)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEMANTIC_PARSER_POS_TYPE_PREPOSITION];
	}
	*/

	currentFeatureInSentence->GIAsemanticParserPOStype = GIAsemanticParserPOStype;
	#ifdef GIA_DEBUG
	//cout << "GIAsemanticParserPOStype = " << GIAsemanticParserPOStype << endl;
	#endif
}


void determineGIAconnectionistNetworkPOStypeNameRelex(GIAfeature* currentFeatureInSentence)
{
	int GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_UNDEFINED;
	//cout << "currentFeatureInSentence->type = " << currentFeatureInSentence->type << endl;
	for(int i=0; i<FEATURE_RELEX_POS_NUMBER_OF_TYPES; i++)
	{
		if(featureRelexPOStypeArray[i] == currentFeatureInSentence->type)
		{
			#ifdef GIA_DEBUG
			//cout << "featureRelexPOStypeArray[i] = " << featureRelexPOStypeArray[i] << endl;
			//cout << "i = " << i << endl;
			//cout << "featureRelexPOStypeCrossReferenceGIAconnectionistNetworkPOStypeArray[i] = " << featureRelexPOStypeCrossReferenceGIAconnectionistNetworkPOStypeArray[i] << endl;
			#endif
			GIAsemanticParserPOStype = featureRelexPOStypeCrossReferenceGIAconnectionistNetworkPOStypeArray[i];
		}
	}

	//additional cases not identified by [/mapped to existing] Relex Word Type:
	for(int i=0; i<ENTITY_WH_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityWhArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_WH;
		}
	}
	for(int i=0; i<ENTITY_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityCoordinatingConjunctionArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_COORDINATINGCONJUNCTION;
		}
	}
	for(int i=0; i<ENTITY_POSSESSIVEENDING_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPossessiveEndingArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_POSSESSIVEENDING;
		}
	}
	for(int i=0; i<ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityModalAuxiliaryArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_MODALAUXILIARY;
		}
	}
	for(int i=0; i<ENTITY_CARDINALNUMBER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if((currentFeatureInSentence->lemma).find(entityCardinalNumberArray[i]) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_NUMBER;
		}
	}
	for(int i=0; i<ENTITY_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPronounPersonalArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PRONOUN_PERSONAL;
		}
	}
	for(int i=0; i<ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPronounPossessiveArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PRONOUN_POSSESSIVE;
		}
	}
	for(int i=0; i<ENTITY_PREDETERMINER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPredeterminerArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_PREDETERMINER;
		}
	}

	//GIA_SEMANTIC_PARSER_POS_TYPE_INTERJECTION not currently supported by Relex

	//additional cases required for GIA semantics extraction;
	for(int i=0; i<ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryBeingArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_BEING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryHavingArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_HAVING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryDoingArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_AUXILIARY_DOING;
		}
	}

	#ifdef GIA2_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
	for(int i=0; i<GRAMMATICAL_DETERMINER_LIMITED_INDEFINITE_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == grammaticalDeterminerIndefiniteArray[i])
		{
			GIAsemanticParserPOStype = GIA_SEMANTIC_PARSER_POS_TYPE_DETERMINER_LIMITED_INDEFINITE;
		}
	}
	#endif

	currentFeatureInSentence->GIAsemanticParserPOStype = GIAsemanticParserPOStype;
	#ifdef GIA_DEBUG
	//cout << "GIAsemanticParserPOStype = " << GIAsemanticParserPOStype << endl;
	#endif
}

#ifdef GIA2_SEMANTIC_PARSER
#ifdef GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS
//based on NLC generateLogicalConditionImplicitConjunctionsAndIdentifyCommand
//eg extracts "The pie has a car [GIA_SEMANTIC_PARSER_POS_TYPE_SPECIAL_REDUCED_CONJUNCTION] chicken" from "The pie has a car, bike, and chicken." (where centralWord corresponds to chicken; ie 10)
GIAfeature* generateOptimisedFeatureSubsetBasedOnContextualConjunctions(GIAfeature* firstFeatureInSentenceSubset, int centralWord, bool* optimisedBasedOnContextualConjunctions)
{
	bool result = true;

	#ifdef GIA_SEMANTIC_PARSER_TRANSLATOR_DEBUG
	cout << "generateOptimisedFeatureSubsetBasedOnContextualConjunctions: firstFeatureInSentenceSubset->word = " << firstFeatureInSentenceSubset->word << endl;
	#endif

	bool commaDetected = false;
	int indexOfFirstCommaDetected = INT_DEFAULT_VALUE;
	int indexOfLastCommaDetected = INT_DEFAULT_VALUE;
	bool conjunctionDetected = false;
	int indexOfConjunction = INT_DEFAULT_VALUE;
	bool foundCentralWord = false; 	//redundant because last feature in list will have been set to centralWord

	GIAfeature* currentFeatureInSentenceSubset = firstFeatureInSentenceSubset;
	int featureIndex = GIA_NLP_START_ENTITY_INDEX;	//1

	while(currentFeatureInSentenceSubset->next != NULL)
	{
		if(foundCentralWord)
		{
			cout << "generateOptimisedFeatureSubsetBasedOnContextualConjunctions{} error: foundCentralWord and still parsing features" << endl;
		}
		else
		{
			if(currentFeatureInSentenceSubset->entityIndex != featureIndex)
			{
				//CHECKTHIS: check this test is valid for cases of intra/intersentence referencing
				cout << "generateOptimisedFeatureSubsetBasedOnContextualConjunctions{} error: (currentFeatureInSentenceSubset->entityIndex != featureIndex)" << endl;
				cout << "currentFeatureInSentenceSubset->entityIndex = " << currentFeatureInSentenceSubset->entityIndex << endl;
				cout << "featureIndex = " << featureIndex << endl;
				cout << "currentFeatureInSentenceSubset->lemma = " << currentFeatureInSentenceSubset->lemma << endl;
				exit(0);
			}

			if(currentFeatureInSentenceSubset->word == STRING_COMMA)
			{
				if(commaDetected)
				{
					indexOfFirstCommaDetected = featureIndex;
				}
				else
				{
					indexOfLastCommaDetected = featureIndex;
				}
			}

			int arrayIndexOfResultFound = INT_DEFAULT_VALUE;
			if(textInTextArray(currentFeatureInSentenceSubset->word, featureSpecialReducedConjunctionArray, FEATURE_SPECIAL_REDUCED_CONJUNCTIONS_NUMBER_OF_TYPES, &arrayIndexOfResultFound))
			{
				indexOfConjunction = featureIndex;
				conjunctionDetected = true;
			}
		}

		if(currentFeatureInSentenceSubset->entityIndex == centralWord)
		{
			foundCentralWord = true;
		}

		currentFeatureInSentenceSubset = currentFeatureInSentenceSubset->next;
		featureIndex++;
	}

	GIAfeature* firstFeatureInOptimisedSentenceSubset = NULL;
	if((commaDetected && conjunctionDetected) || (indexOfLastCommaDetected != INT_DEFAULT_VALUE))
	{
		firstFeatureInOptimisedSentenceSubset = new GIAfeature(*firstFeatureInSentenceSubset);	//copy feature
		GIAfeature* currentFeatureInOptimisedSentenceSubset = firstFeatureInOptimisedSentenceSubset;
		GIAfeature* currentFeatureInSentenceSubset = currentFeatureInSentenceSubset;
		int featureIndex = GIA_NLP_START_ENTITY_INDEX;	//1
		bool filteredSubphrase = false;
		bool currentlyFilteringSubphrase = false;
		while(currentFeatureInSentenceSubset = currentFeatureInSentenceSubset->next)
		{
			if(!filteredSubphrase)
			{
				if(currentlyFilteringSubphrase)
				{
					if(((commaDetected && conjunctionDetected) && (featureIndex == indexOfConjunction)) ||
					((indexOfLastCommaDetected != INT_DEFAULT_VALUE) && (featureIndex == indexOfLastCommaDetected)))
					{
						currentlyFilteringSubphrase = false;
						filteredSubphrase = true;
					}
				}
				else
				{
					if(featureIndex == indexOfFirstCommaDetected)
					{
						currentlyFilteringSubphrase = true;
					}
					else
					{
						GIAfeature* newFeatureInOptimisedSentenceSubset = new GIAfeature(*currentFeatureInSentenceSubset);
						newFeatureInOptimisedSentenceSubset->previous = currentFeatureInOptimisedSentenceSubset;	//redundant
						currentFeatureInOptimisedSentenceSubset->next = newFeatureInOptimisedSentenceSubset;
						currentFeatureInOptimisedSentenceSubset = currentFeatureInOptimisedSentenceSubset->next;
					}
				}

			}

			currentFeatureInSentenceSubset = currentFeatureInSentenceSubset->next;
			featureIndex++;
		}
		*optimisedBasedOnContextualConjunctions = true;
	}
	else
	{
		//no superphrase (conjunction/commas) found, do not optimise sentence subset
		firstFeatureInOptimisedSentenceSubset = firstFeatureInSentenceSubset;
		*optimisedBasedOnContextualConjunctions = false;
	}

	return firstFeatureInOptimisedSentenceSubset;
}
#endif
#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
int calculateFirstWordInTupleIndexRelative(int firstWordInTupleIndex, int firstWordInSentenceSubsetIndex)
{
	int firstWordInTupleIndexRelative = firstWordInTupleIndex - firstWordInSentenceSubsetIndex + 1;
	return firstWordInTupleIndexRelative;

}
#endif
#endif

#endif





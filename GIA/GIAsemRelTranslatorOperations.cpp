/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemRelTranslatorOperations.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3f2l 04-April-2018
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 * Description: Semantic Relation Translator
 * /
 *******************************************************************************/


#include "GIAsemRelTranslatorOperations.hpp"

#ifndef LINUX
	#include <windows.h>
#endif


#ifdef GIA_SEM_REL_TRANSLATOR_COMMON

GIArelation* GIAsemRelTranslatorOperationsClass::GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(const GIAtranslatorVariablesClass* translatorVariables, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	string GIA2semanticDependencyRelationText = "";
	GIArelation* GIA2semanticDependencyRelation = GIAsemRelTranslatorDatabase.getCurrentRelationInSemanticParserSentenceList();
	generateGIA2semanticDependencyRelation(translatorVariables->GIAentityNodeArray, connectionType, entityIndex1, entityIndex2, sameReferenceSet, &GIA2semanticDependencyRelationText, GIA2semanticDependencyRelation);
	#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	GIAsemRelTranslatorDatabase.saveTextLineToCorpusFileString(GIA2semanticDependencyRelation);
	cout << GIA2semanticDependencyRelationText << endl;
	#else
	GIA2semanticDependencyRelation->next = new GIArelation();
	#endif
	
	return GIA2semanticDependencyRelation;
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

//this function [recording aux/cop/det syntatical dependency relations] is required to extract tense and perform instance/networkIndex identification once GIA2 lookup has been performed: it is only currently supported by Stanford parser
void GIAsemRelTranslatorOperationsClass::GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial(const GIAtranslatorVariablesClass* translatorVariables, const bool linkPreestablishedReferencesGIA)
{
	#ifndef GIA_SEM_REL_TRANSLATOR_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
	if(translatorVariables->NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
	{
	#endif
		if(!linkPreestablishedReferencesGIA)
		{
			const GIArelation* currentRelationInList = translatorVariables->currentSentenceInList->firstRelationInList;

			while(currentRelationInList->next != NULL)
			{
				int entityIndex1 = currentRelationInList->relationGovernorIndex;
				int entityIndex2 = currentRelationInList->relationDependentIndex;
				bool sameReferenceSet =	false;	//irrelevant

				/*
				if(!(currentRelationInList->disabled))
				{
				*/
				//NB these must correspond to GIA2syntacticDependencyRelationSecondaryNameArray/GIA_SEM_REL_TRANSLATOR_SYNTACTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES:

				if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_MODAL_AUX)	//same as auxiliary
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, entityIndex1, entityIndex2, sameReferenceSet);
				}

				if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PASSIVE_AUX)
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, entityIndex1, entityIndex2, sameReferenceSet);
				}

				if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_COPULA)
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_MODAL_AUXILIARY_OR_COPULA, entityIndex1, entityIndex2, sameReferenceSet);
				}

				if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_DETERMINER)
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_DETERMINER, entityIndex1, entityIndex2, sameReferenceSet);
				}

				#ifdef GIA_DYNAMICALLY_LINK_ENTITIES_DISABLE_GIA_SEM_REL_TRANSLATOR_SEMANTIC_RELATION_GENERATION
				if(currentRelationInList->relationType == GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PRENOMIAL_MODIFIER)	//must correspond to GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PRENOMINAL_MODIFIER_NUMBER_OF_TYPES/relationTypePrenominalModifierNameArray
				{
					GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrain(translatorVariables, GIA_ENTITY_VECTOR_CONNECTION_TYPE_PRENOMINAL_MODIFIER, entityIndex1, entityIndex2, sameReferenceSet);
				}
				#endif
				/*
				}
				*/

				currentRelationInList = currentRelationInList->next;
			}
		}
	#ifndef GIA_SEM_REL_TRANSLATOR_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
	}
	else
	{
		cout << "GIA2nonHeuristicImplementationGenerateExperiencesForConnectionistNetworkTrainSpecial{} error: function currently requires Stanford parser/CoreNLP" << endl;
	}
	#endif
}

void GIAsemRelTranslatorOperationsClass::generateGIA2semanticDependencyRelation(constEffective vector<GIAentityNode*>* GIAentityNodeArray, int connectionType, int entityIndex1, int entityIndex2, bool sameReferenceSet, string* GIA2semanticDependencyRelationText, GIArelation* GIA2semanticDependencyRelation)
{
	#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES
	string entityWord1 = (*GIAentityNodeArray)[entityIndex1]->entityName;
	string entityWord2 = (*GIAentityNodeArray)[entityIndex2]->entityName;

	//NB GIA2:GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE does not support queries/GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES (ie the special entity name adjustments below)
	string entityWord1Query = "";
	if((*GIAentityNodeArray)[entityIndex1]->entityName == GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
	{
		entityWord1Query = GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
	}
	if((*GIAentityNodeArray)[entityIndex1]->isNameQuery)
	{
		cout << "isNameQuery" << endl;
		entityWord1 = entityWord1Query + GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME;
	}
	else if((*GIAentityNodeArray)[entityIndex1]->isWhichOrEquivalentWhatQuery)
	{
		cout << "isWhichOrEquivalentWhatQuery" << endl;
		entityWord1 = entityWord1Query + GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME;
	}
	else if((*GIAentityNodeArray)[entityIndex1]->isQuery)
	{
		cout << "isQuery" << endl;
		entityWord1 = entityWord1Query + GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME;
	}

	//NB GIA2:GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE does not support queries/GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES (ie the special entity name adjustments below)
	string entityWord2Query = "";
	if((*GIAentityNodeArray)[entityIndex2]->entityName == GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
	{
		entityWord2Query = GIA_SYN_REL_TRANSLATOR_REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
	}
	if((*GIAentityNodeArray)[entityIndex2]->isNameQuery)
	{
		cout << "isNameQuery" << endl;
		entityWord2 = entityWord2Query + GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME;
	}
	else if((*GIAentityNodeArray)[entityIndex2]->isWhichOrEquivalentWhatQuery)
	{
		cout << "isWhichOrEquivalentWhatQuery" << endl;
		entityWord2 = entityWord2Query + GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME;
	}
	else if((*GIAentityNodeArray)[entityIndex2]->isQuery)
	{
		cout << "isQuery" << endl;
		entityWord2 = entityWord2Query + GIA_SEM_REL_TRANSLATOR_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME;
	}
	#else
	string entityWord1 = (*GIAentityNodeArray)[entityIndex1]->wordOrig;
	string entityWord2 = (*GIAentityNodeArray)[entityIndex2]->wordOrig;
	//lemmas are in general not recorded as they are irrelevant (wordOrig is only recorded for debugging purposes and internal/manual/inhouse development of the corpus); only the entity indicies require recording
	if(entityWord1 == "")
	{//why does GIAentityNodes in GIAentityNodeArray that correspond to prepositions not have a "wordOrig" but only have an entityName? (is it related to LRP?)
		entityWord1 = (*GIAentityNodeArray)[entityIndex1]->entityName;
	}
	if(entityWord2 == "")
	{//why does GIAentityNodes in GIAentityNodeArray that correspond to prepositions not have a "wordOrig" but only have an entityName? (is it related to LRP?)
		entityWord2 = (*GIAentityNodeArray)[entityIndex2]->entityName;
	}
	#endif

	#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	*GIA2semanticDependencyRelationText = generateGIA2semanticDependencyRelationText(entityWord1, entityWord2, GIA2semanticDependencyRelationNameArray[connectionType], entityIndex1, entityIndex2, sameReferenceSet);
	#else
	GIAsemRelTranslatorDatabase.generateGIA2semanticDependencyRelationObject(GIA2semanticDependencyRelation, entityWord1, entityWord2, GIA2semanticDependencyRelationNameArray[connectionType], entityIndex1, entityIndex2, sameReferenceSet);
	#endif
}

#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS

#ifndef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD

void GIAsemRelTranslatorOperationsClass::writeSemanticParserCorpusFileOptimised(const GIAtranslatorVariablesClass* translatorVariables)
{
	string sentenceSemanticRelationsText = generateSemanticParserCorpusSemanticRelationsText(GIAsemRelTranslatorDatabase.getFirstRelationInSemanticParserSentenceList());
	GIAsemRelTranslatorDatabase.writeSemanticParserCorpusFile(translatorVariables->currentSentenceInList->firstFeatureInList, &sentenceSemanticRelationsText);
}

#endif

#endif

//debug only:
void GIAsemRelTranslatorDatabaseClass::writeSemanticRelationsFile(const GIAtranslatorVariablesClass* translatorVariables, const string sentenceSemanticRelationsFileName)
{
	string sentenceSemanticRelationsText = generateSemanticParserCorpusSemanticRelationsText(GIAsemRelTranslatorDatabase.getFirstRelationInSemanticParserSentenceList());
	SHAREDvars.writeStringToFile(sentenceSemanticRelationsFileName, &semanticParserCorpusDatabaseSentenceText);
}

string GIAsemRelTranslatorOperationsClass::generateSemanticParserCorpusSemanticRelationsText(const GIArelation* firstSemanticRelationInList)
{
	string sentenceSemanticRelationsText = "";
	const GIArelation* currentSemanticRelationInList = firstSemanticRelationInList;
	while(currentSemanticRelationInList->next != NULL)
	{
		string GIA2semanticDependencyRelation = generateGIA2semanticDependencyRelationText(currentSemanticRelationInList->relationGovernor, currentSemanticRelationInList->relationDependent, currentSemanticRelationInList->relationType, currentSemanticRelationInList->relationGovernorIndex, currentSemanticRelationInList->relationDependentIndex, currentSemanticRelationInList->sameReferenceSet);
		GIA2semanticDependencyRelation = GIA2semanticDependencyRelation + STRING_NEWLINE;
		sentenceSemanticRelationsText = sentenceSemanticRelationsText + GIA2semanticDependencyRelation;

		currentSemanticRelationInList = currentSemanticRelationInList->next;
	}
	sentenceSemanticRelationsText = sentenceSemanticRelationsText + STRING_NEWLINE;	//required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)
	return sentenceSemanticRelationsText;
}


#ifdef GIA_SEM_REL_TRANSLATOR

void GIAsemRelTranslatorOperationsClass::determineGIAconnectionistNetworkPOStypeNames(GIAfeature* firstFeatureInList, const int NLPfeatureParser)
{
	GIAfeature* currentFeatureInSentence = firstFeatureInList;
	while(currentFeatureInSentence->next != NULL)
	{
		if((NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP) || (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER))
		{
			determineGIAconnectionistNetworkPOStypeNameStanford(currentFeatureInSentence);
		}
		else if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
		{
			#ifndef GIA_SEM_REL_TRANSLATOR_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
			cout << "warning: determineGIAconnectionistNetworkPOStypeNames{} is supported, but Relex cannot generate det and aux syntactic relations (required to be stored by GIA connectionist network as 'GIA semantic relations' to reextract instances/substances and tense)" << endl;
			#endif
			determineGIAconnectionistNetworkPOStypeNameRelex(currentFeatureInSentence);
		}

		currentFeatureInSentence = currentFeatureInSentence->next;
	}
}

void GIAsemRelTranslatorOperationsClass::determineGIAconnectionistNetworkPOStypeNameStanford(GIAfeature* currentFeatureInSentence)
{
	int GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_UNDEFINED;

	for(int i=0; i<FEATURE_POS_TAG_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagCoordinatingConjunctionArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_CONJUNCTION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_NUMBER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagNumberArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_NUMBER;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_DETERMINER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagDeterminerArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_DETERMINER;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_UNKNOWN_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagUnknownArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_UNKNOWN;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PREDETERMINER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPredeterminerArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PREDETERMINER;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_POSSESSIVEENDING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPossessiveEndingArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_POSSESSIVEENDING;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_MODALAUXILIARY_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagModalAuxiliaryArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_MODALAUXILIARY;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PREPOSITION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPrepositionArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PREPOSITION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_ADJECTIVE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagAdjectiveArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_ADJECTIVE;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_NOUN_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagNounArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_NOUN;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_ADVERB_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagAdverbArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_ADVERB;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPronounPersonalArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PRONOUN_PERSONAL;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPronounPossessiveArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PRONOUN_POSSESSIVE;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PARTICLE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagParticleArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PARTICLE;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_INTERJECTION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagInterjectionArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_INTERJECTION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_VERB_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagVerbArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_VERB;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_WH_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagWhArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_WH;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PUNCTUATION_DIVISION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPunctuationDivisionArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PUNCTUATION_DIVISION;
		}
	}
	for(int i=0; i<FEATURE_POS_TAG_PUNCTUATION_QUOTE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->stanfordPOS == featurePOStagPunctuationQuoteArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PUNCTUATION_QUOTE;
		}
	}
	
	determineGIAconnectionistNetworkPOStypeNameShared(currentFeatureInSentence, &GIAsemRelTranslatorPOStype);

	/*
	//requires updating (add more cases from PENN tree above)
	if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_NOUN)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEM_REL_TRANSLATOR_POS_TYPE_NOUN];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_VERB)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEM_REL_TRANSLATOR_POS_TYPE_VERB_OR_PARTICIPLE];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_ADJ)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEM_REL_TRANSLATOR_POS_TYPE_ADJECTIVE_OR_ADVERB];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_ADV)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEM_REL_TRANSLATOR_POS_TYPE_ADJECTIVE_OR_ADVERB];
	}
	else if(currentFeatureInSentence->grammaticalWordType == GRAMMATICAL_WORD_TYPE_PREP)
	{
		GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[GIA_SEM_REL_TRANSLATOR_POS_TYPE_PREPOSITION];
	}
	*/

	currentFeatureInSentence->GIAsemRelTranslatorPOStype = GIAsemRelTranslatorPOStype;
}

void GIAsemRelTranslatorOperationsClass::determineGIAconnectionistNetworkPOStypeNameRelex(GIAfeature* currentFeatureInSentence)
{
	int GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_UNDEFINED;
	//cout << "currentFeatureInSentence->type = " << currentFeatureInSentence->type << endl;
	for(int i=0; i<FEATURE_RELEX_POS_NUMBER_OF_TYPES; i++)
	{
		if(featureRelexPOStypeArray[i] == currentFeatureInSentence->type)
		{
			GIAsemRelTranslatorPOStype = featureRelexPOStypeCrossReferenceGIAconnectionistNetworkPOStypeArray[i];
		}
	}

	//additional cases not identified by [/mapped to existing] Relex Word Type:
	for(int i=0; i<ENTITY_WH_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityWhArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_WH;
		}
	}
	for(int i=0; i<ENTITY_COORDINATINGCONJUNCTION_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityCoordinatingConjunctionArray[i])	//FUTURE: this will be effectively changed to entityConjunctionArray when GIA semantic parser is upgraded to use GIA preprocessor POS tagger
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_CONJUNCTION;
		}
	}
	for(int i=0; i<ENTITY_POSSESSIVEENDING_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPossessiveEndingArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_POSSESSIVEENDING;
		}
	}
	for(int i=0; i<ENTITY_MODALAUXILIARY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityModalAuxiliaryArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_MODALAUXILIARY;
		}
	}
	for(int i=0; i<ENTITY_CARDINALNUMBER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if((currentFeatureInSentence->lemma).find(entityCardinalNumberArray[i]) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_NUMBER;
		}
	}
	for(int i=0; i<ENTITY_PRONOUN_PERSONAL_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPronounPersonalArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PRONOUN_PERSONAL;
		}
	}
	for(int i=0; i<ENTITY_PRONOUN_POSSESSIVE_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPronounPossessiveArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PRONOUN_POSSESSIVE;
		}
	}
	for(int i=0; i<ENTITY_PREDETERMINER_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityPredeterminerArray[i])
		{
			GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_PREDETERMINER;
		}
	}
	
	determineGIAconnectionistNetworkPOStypeNameShared(currentFeatureInSentence, &GIAsemRelTranslatorPOStype);

	//GIA_SEM_REL_TRANSLATOR_POS_TYPE_INTERJECTION not currently supported by Relex

	currentFeatureInSentence->GIAsemRelTranslatorPOStype = GIAsemRelTranslatorPOStype;
}

void GIAsemRelTranslatorOperationsClass::determineGIAconnectionistNetworkPOStypeNameShared(GIAfeature* currentFeatureInSentence, int* GIAsemRelTranslatorPOStype)
{
	//additional cases required for GIA semantics extraction;
	for(int i=0; i<ENTITY_AUXILIARY_BEING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryBeingArray[i])
		{
			*GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_AUXILIARY_BEING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_HAVING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryHavingArray[i])
		{
			*GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_AUXILIARY_HAVING;
		}
	}
	for(int i=0; i<ENTITY_AUXILIARY_DOING_ARRAY_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == entityAuxiliaryDoingArray[i])
		{
			*GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_AUXILIARY_DOING;
		}
	}

	#ifdef GIA_SEM_REL_TRANSLATOR_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
	for(int i=0; i<GRAMMATICAL_DETERMINER_INDEFINITE_NUMBER_OF_TYPES; i++)
	{
		if(currentFeatureInSentence->lemma == grammaticalDeterminerIndefiniteArray[i])
		{
			*GIAsemRelTranslatorPOStype = GIA_SEM_REL_TRANSLATOR_POS_TYPE_DETERMINER_LIMITED_INDEFINITE;
		}
	}
	#endif
}

#endif


#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS
#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISE_BASED_ON_CONJUNCTIONS
//based on NLC generateLogicalConditionImplicitConjunctionsAndIdentifyCommand
//eg extracts "The pie has a car [GIA_SEM_REL_TRANSLATOR_POS_TYPE_SPECIAL_REDUCED_CONJUNCTION] chicken" from "The pie has a car, bike, and chicken." (where centralWord corresponds to chicken; ie 10)
GIAfeature* GIAsemRelTranslatorOperationsClass::generateOptimisedFeatureSubsetBasedOnContextualConjunctions(GIAfeature* firstFeatureInSentenceSubset, const int centralWord, bool* optimisedBasedOnContextualConjunctions)
{
	bool result = true;


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
				cerr << "generateOptimisedFeatureSubsetBasedOnContextualConjunctions{} error: (currentFeatureInSentenceSubset->entityIndex != featureIndex)" << endl;
				cerr << "currentFeatureInSentenceSubset->entityIndex = " << currentFeatureInSentenceSubset->entityIndex << endl;
				cerr << "featureIndex = " << featureIndex << endl;
				cerr << "currentFeatureInSentenceSubset->lemma = " << currentFeatureInSentenceSubset->lemma << endl;
				exit(EXIT_ERROR);
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
			if(SHAREDvars.textInTextArray(currentFeatureInSentenceSubset->word, featureSpecialReducedConjunctionArray, FEATURE_SPECIAL_REDUCED_CONJUNCTIONS_NUMBER_OF_TYPES, &arrayIndexOfResultFound))
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
		GIAfeature* currentFeatureInSentenceSubset = firstFeatureInSentenceSubset;
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
#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
int GIAsemRelTranslatorOperationsClass::calculateFirstWordInTupleIndexRelative(const int firstWordInTupleIndex, const int firstWordInSentenceSubsetIndex)
{
	int firstWordInTupleIndexRelative = firstWordInTupleIndex - firstWordInSentenceSubsetIndex + 1;
	return firstWordInTupleIndexRelative;

}
#endif
#endif

#endif

string GIAsemRelTranslatorOperationsClass::generateGIA2semanticDependencyRelationText(const string entityName1, const string entityName2, const string semanticRelation, const int entityIndex1, const int entityIndex2, const bool sameReferenceSet)
{
	string GIA2semanticDependencyRelation = "";
	GIA2semanticDependencyRelation = GIA2semanticDependencyRelation + semanticRelation + "(" + entityName1 + "-" + SHAREDvars.convertIntToString(entityIndex1) + ", " + entityName2 + "-" + SHAREDvars.convertIntToString(entityIndex2) + ") " + createSameReferenceSetRecord(sameReferenceSet);
	return GIA2semanticDependencyRelation;
}

string GIAsemRelTranslatorOperationsClass::createSameReferenceSetRecord(const bool sameReferenceSet)
{
	string sameReferenceSetRecord = "[sameReferenceSet=" + SHAREDvars.convertBoolToString(sameReferenceSet) + "]";
	return sameReferenceSetRecord;
}





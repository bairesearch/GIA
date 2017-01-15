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
 * File Name: GIAtranslatorDefineGrammar.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2a2a 27-October-2013
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Converts relation objects into GIA nodes (of type entity, action, condition etc) in GIA network/tree
 * TO DO: replace vectors entityNodesActiveListConcepts/conceptEntityNamesList with a map, and replace vectors GIAtimeConditionNode/timeConditionNumbersActiveList with a map
 * TO DO: extract date information of entities from relex <features> tag area
 *******************************************************************************/


#include "GIAtranslatorDefineGrammar.h"
#include "GIAdatabase.h"



void locateAndAddAllFeatureTempEntities(Sentence * currentSentenceInList, bool GIAentityNodeArrayFilled[], GIAentityNode * GIAfeatureTempEntityNodeArray[], int NLPdependencyRelationsType, int NLPfeatureParser)
{
	if(currentSentenceInList->isQuestion)
	{
		setFoundComparisonVariable(false);
	}

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
 	while(currentRelationInList->next != NULL)
	{
		#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_3B_PREPOSITIONS_REDUCTION
		currentRelationInList->relationGovernor = performPrepositionReduction(currentRelationInList->relationGovernor);
		currentRelationInList->relationDependent = performPrepositionReduction(currentRelationInList->relationDependent);
		currentRelationInList->relationType = performPrepositionReduction(currentRelationInList->relationType);
		#endif

		string name[2];
		name[0] = currentRelationInList->relationGovernor;
		name[1] =  currentRelationInList->relationDependent; 	//argumentName

		#ifdef GIA_USE_RELEX
		if(NLPdependencyRelationsType == GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
		{
			#ifdef GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1
			if(currentRelationInList->relationDependentIndex < 0)
			{//to prevent Relex 1.4.0 error where it uses a relation argument index of '-1' very occasionally
				currentRelationInList->relationDependentIndex = GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1_REPLACEMENT_INDEX;
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "\tGIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1_REPLACEMENT_INDEX = " << GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_RELATION_DEPENDENT_INDEX_MINUS_1_REPLACEMENT_INDEX << endl;
				#endif
			}
			#endif
		}
		#endif

		int relationIndex[2];
		relationIndex[0] = currentRelationInList->relationGovernorIndex;
		relationIndex[1] = currentRelationInList->relationDependentIndex;

		#ifdef GIA_TRANSLATOR_DEBUG
		//cout << "relationIndex[0]  = " << relationIndex[0] << endl;
		//cout << "relationIndex[1]  = " << relationIndex[1] << endl;
		//cout << "name[0]  = " << name[0] << endl;
		//cout << "name[1]  = " << name[1] << endl;
		#endif

		bool argumentIsQuery = false;
		#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
		/*presumably this code relates to the special case; if there is another relation entity (eg _%atLocation[1]) with the same feature index as $qVar (eg _$qVar[1])
		eg Where is the ball?
		_pobj(_%atLocation[1], _$qVar[1])
		_psubj(_%atLocation[1], ball[4])
		*/
		if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)	//ie if(NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP)
		{
			if(name[1] == REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)
			{//modify relation index [to prevent overlapping of comparison variable indicies with other indicies]

				#ifdef GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_QVAR_INDEX_SAME_AS_ANOTHER_RELATION_INDEX
				//create a new feature and add it onto the feature list
				Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
				while(currentFeatureInList->next != NULL)
				{
					currentFeatureInList = currentFeatureInList->next;
				}
				currentFeatureInList->entityIndex = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX;
				Feature * newFeature = new Feature();
				currentFeatureInList->next = newFeature;
				#else
				//removed 19 July 2013 after failure to parse "Where is the ball?":
				//update feature->entityIndex using featureArrayTemp - added 1 May 2012 after Relex Failure detected [somewhere between 1j6b -> 1j6f]
				Feature * featureArrayTemp[MAX_NUMBER_OF_WORDS_PER_SENTENCE];
				generateTempFeatureArray(currentSentenceInList->firstFeatureInList, featureArrayTemp);
				Feature * featureOfQueryNode = featureArrayTemp[relationIndex[1]];
				//cout << "featureOfQueryNode->lemma = " << featureOfQueryNode->lemma << endl;
				featureOfQueryNode->entityIndex = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX;
				#endif

				relationIndex[1] = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX;
				currentRelationInList->relationDependentIndex = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_RELATION_DEPENDENT_INDEX;
				argumentIsQuery = true;
			}
		}
		#endif

		for(int i=0; i<2; i++)
		{
			if(!GIAentityNodeArrayFilled[relationIndex[i]])
			{
				GIAentityNodeArrayFilled[relationIndex[i]] = true;

				GIAentityNode * featureTempEntity = new GIAentityNode();
				featureTempEntity->entityName = name[i];
				GIAfeatureTempEntityNodeArray[relationIndex[i]] = featureTempEntity;

				//cout << "filling: " << relationIndex[i] << " " << name[i] << endl;

				#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
				if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)	//ie if(NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP)
				{
					if(i == 1)
					{//argument index only
						if(argumentIsQuery)
						{
							GIAfeatureTempEntityNodeArray[relationIndex[i]]->isQuery = true;
						}
					}
				}
				#endif


				GIAfeatureTempEntityNodeArray[relationIndex[i]]->isObjectTemp = false;
				GIAfeatureTempEntityNodeArray[relationIndex[i]]->isSubjectTemp = false;
				GIAfeatureTempEntityNodeArray[relationIndex[i]]->hasSubstanceTemp = false;
				#ifdef GIA_USE_ADVANCED_REFERENCING
				//this is required for fillGrammaticalArraysStanford findSubjObjRelationMatchingAuxillaryAndSetNotSameReferenceSet()	[nb these values are applied to concept entities only]
				GIAfeatureTempEntityNodeArray[relationIndex[i]]->entityIndexTemp = relationIndex[i];
				GIAfeatureTempEntityNodeArray[relationIndex[i]]->sentenceIndexTemp = currentSentenceInList->sentenceIndex;
				#endif
			}
		}

		currentRelationInList = currentRelationInList->next;
	}

	#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG

	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNode * entity = GIAfeatureTempEntityNodeArray[w];
			cout << "entity->entityName = " << entity->entityName << endl;
		}
	}

	currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		string relationType = currentRelationInList->relationType;
		GIAentityNode * relationGoverner = GIAfeatureTempEntityNodeArray[currentRelationInList->relationGovernorIndex];
		GIAentityNode * relationDependent = GIAfeatureTempEntityNodeArray[currentRelationInList->relationDependentIndex];

		cout << "relationType = " << currentRelationInList->relationType << endl;
		cout << "relationGoverner = " << relationGoverner->entityName << endl;
		cout << "relationDependent = " << relationDependent->entityName << endl;

		currentRelationInList = currentRelationInList->next;
	}

	#endif
}


void locateAndAddAllConceptEntities(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], unordered_map<string, GIAentityNode*> *entityNodesActiveListConcepts, vector<GIAentityNode*> *sentenceConceptEntityNodesList, int NLPdependencyRelationsType, int NLPfeatureParser, GIAentityNode * GIAfeatureTempEntityNodeArray[])
{
	for(int w=0; w<MAX_NUMBER_OF_WORDS_PER_SENTENCE; w++)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNode * featureTempEntityNode = GIAfeatureTempEntityNodeArray[w];
			//cout << "featureTempEntityNode->entityName = " << featureTempEntityNode->entityName << endl;

			bool entityAlreadyExistant = false;
			GIAentityNode * entity = findOrAddConceptEntityNodeByNameSimpleWrapper(&(featureTempEntityNode->entityName), &entityAlreadyExistant, entityNodesActiveListConcepts);
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "entity->entityName = " << entity->entityName << endl;
			#endif

			GIAentityNodeArray[w] = entity;

			GIAentityNodeArray[w]->hasAssociatedInstanceTemp = false;

			sentenceConceptEntityNodesList->push_back(entity);

			#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
			if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)	//ie if(NLPfeatureParser != GIA_NLP_PARSER_STANFORD_CORENLP)
			{
				if(GIAfeatureTempEntityNodeArray[w]->isQuery)
				{
					GIAentityNodeArray[w]->isQuery = true;
					setFoundComparisonVariable(true);
					setComparisonVariableNode(GIAentityNodeArray[w]);
				}
			}
			#endif
			#ifdef GIA_SUPPORT_ALIASES
			if(GIAfeatureTempEntityNodeArray[w]->isNameQuery)
			{
				GIAentityNodeArray[w]->isNameQuery = true;
			}
			if(GIAfeatureTempEntityNodeArray[w]->isName)
			{
				GIAentityNodeArray[w]->isName = true;
			}
			#endif
			#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION
			//this is required because negative assignment is now performed during redistribution
			if(GIAfeatureTempEntityNodeArray[w]->negative)
			{
				GIAentityNodeArray[w]->negative = true;
			}
			#endif
		}
	}
}





#ifdef GIA_USE_RELEX
void fillGrammaticalArraysRelex(Sentence * currentSentenceInList)
{
	Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		//cout << "currentFeatureInList->grammar = " << currentFeatureInList->grammar << endl;
		#endif

		//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
		#ifdef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_5A_RELATIONS_ASSIGN_TIME_NODES_IN_RELEX_THE_SAME_AS_STANFORD
		if((currentFeatureInList->grammar).find(FEATURE_RELEX_FLAG_DATE_NAME) != -1)
		#else
		if(((currentFeatureInList->grammar).find(FEATURE_RELEX_FLAG_DATE_NAME) != -1) || ((currentFeatureInList->grammar).find(FEATURE_RELEX_FLAG_TIME_NAME) != -1))
		#endif
		{
			currentFeatureInList->grammaticalIsDateOrTime = true;
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "isDate currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			#endif
		}

		for(int grammaticalTenseIndex = 0; grammaticalTenseIndex < GRAMMATICAL_TENSE_NUMBER_OF_TYPES; grammaticalTenseIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the tense type name
			if((currentFeatureInList->grammar).find(grammaticalTenseNameArray[grammaticalTenseIndex]) != -1)
			//if((currentFeatureInList->grammar).substr(0, grammaticalTenseNameLengthsArray[grammaticalTenseIndex]) == grammaticalTenseNameArray[grammaticalTenseIndex])
			{
				currentFeatureInList->grammaticalTense = grammaticalTenseIndex;
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalTenseIndex = " << urrentFeatureInList->grammaticalTense << endl;
				#endif
			}
		}
		for(int grammaticalTenseModifierIndex = 0; grammaticalTenseModifierIndex < GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the tense type name
			if((currentFeatureInList->grammar).find(grammaticalTenseModifierNameArray[grammaticalTenseModifierIndex]) != -1)
			{
				currentFeatureInList->grammaticalTenseModifierArray[grammaticalTenseModifierIndex] = true;
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalTenseModifierIndex true = " << grammaticalTenseModifierArray[grammaticalTenseModifierIndex] << endl;
				#endif
			}
		}

	#ifndef GIA_DO_NOT_SUPPORT_SPECIAL_CASE_2A_GRAMMAR_TREAT_PRESENT_PERFECT_AS_PAST_TENSE
		//interpret "present_perfect" relex flag as past tense
		if(currentFeatureInList->grammaticalTense == GRAMMATICAL_TENSE_PRESENT)
		{
			if(currentFeatureInList->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PERFECT] == true)
			{
				currentFeatureInList->grammaticalTense = GRAMMATICAL_TENSE_PAST;
			}
		}
	#endif

		for(int grammaticalNumberIndex = 0; grammaticalNumberIndex < GRAMMATICAL_NUMBER_NUMBER_OF_TYPES; grammaticalNumberIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the grammatical number type name
			if((currentFeatureInList->grammar).find(grammaticalNumberNameArray[grammaticalNumberIndex]) != -1)
			{
				currentFeatureInList->grammaticalNumber = grammaticalNumberIndex;
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalNumberIndex = " << grammaticalNumberNameArray[grammaticalNumberIndex] << endl;
				#endif
			}
		}
		if((currentFeatureInList->grammar).find(GRAMMATICAL_DEFINITE_NAME) != -1)
		{
			currentFeatureInList->grammaticalIsDefinite = GRAMMATICAL_DEFINITE;
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "isDefinite currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			#endif
		}


		for(int grammaticalGenderIndex = 0; grammaticalGenderIndex < GRAMMATICAL_GENDER_NUMBER_OF_TYPES; grammaticalGenderIndex++)
		{
			//NB only the first characters of currentFeatureInList->grammar contain the grammatical Gender type name
			if((currentFeatureInList->grammar).find(grammaticalGenderNameArray[grammaticalGenderIndex]) != -1)
			{
				//NB it will always find "person" in relex grammar string if "person" is existant, but this will be overwritten by "feminine" or "masculine" if this is specified (not possible for bigender names like "joe")
				currentFeatureInList->grammaticalGender = grammaticalGenderIndex;
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex grammaticalGenderIndex = " << grammaticalGenderNameArray[grammaticalGenderIndex] << endl;
				#endif
			}
		}

		/*
		//this date code probably requires an update [there appear to be multiple ways in which dates are defined in relex...
		if((currentFeatureInList->grammar).find(FEATURE_GRAMMATICAL_COUNT) != -1)
		{
			currentFeatureInList->grammaticalHasCount = true;
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "hasCount currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			#endif
		}
		*/

		if((currentFeatureInList->grammar).find(GRAMMATICAL_PRONOUN_NAME) != -1)
		{
			currentFeatureInList->grammaticalIsPronoun = GRAMMATICAL_PRONOUN;
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "isPronoun currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			#endif
		}

		//fill grammaticalWordTypeTemp array for wordnet - added 26 April 2012
		int grammaticalWordTypeTemp = GRAMMATICAL_WORD_TYPE_UNDEFINED;
		convertRelexPOStypeToWordnetWordType(&(currentFeatureInList->type), &grammaticalWordTypeTemp);
		currentFeatureInList->grammaticalWordType = grammaticalWordTypeTemp;

		#ifdef FILL_NER_ARRAY_AFTER_RELEX_PARSE_FOR_STANFORD_EQUIVALENT_PROPER_NOUN_DETECTION
		//fill NER array after Relex Parse for Stanford equivalent proper noun detection - added 26 April 2012
		for(int featureRelexFlagIndex = 0; featureRelexFlagIndex < FEATURE_RELEX_FLAG_NUMBER_OF_TYPES; featureRelexFlagIndex++)
		{
			if((currentFeatureInList->grammar).find(featureRelexFlagTypeArray[featureRelexFlagIndex]) != -1)
			{
				currentFeatureInList->NER = featureRelexFlagIndex;
				#ifdef GIA_WORDNET_DEBUG
				//cout << "currentFeatureInList->word = " << currentFeatureInList->word << " currentFeatureInList->entityIndex " << featureRelexFlagIndex << " = " << featureRelexFlagTypeArray[featureRelexFlagIndex] << endl;
				#endif
			}
		}
		for(int i=0; i<FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_OF_TYPES; i++)
		{
			if(currentFeatureInList->NER == featureNERindicatesProperNounTypeArray[i])
			{
				currentFeatureInList->grammaticalIsProperNoun = true;
				#ifdef GIA_WORDNET_DEBUG
				cout << "isProperNoun currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
				#endif
			}
		}
		#else
		if((currentFeatureInList->grammar).find(GRAMMATICAL_PERSON_NAME) != -1)
		{
			currentFeatureInList->grammaticalIsProperNoun = GRAMMATICAL_PERSON;
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "isPerson currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
			#endif
		}
		#endif

		currentFeatureInList = currentFeatureInList->next;
	}
}
#endif


#ifdef GIA_NLP_PARSER_STANFORD_CORENLP
void extractPastTenseFromPOStag(string * POStag, Feature * feature)
{
	bool pastTenseDetected = false;

	//do not write if present tense found: NB copulas take precedence over auxillaries in formation of past tense (eg he has been sick;     nsubj ( sick-4 , he-1 ) / aux ( sick-4 , has-2 ) / cop ( sick-4 , been-3 )
	for(int i=0; i<FEATURE_POS_TAG_VERB_PAST_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagVerbPastArray[i])
		{
			pastTenseDetected = true;
		}
	}
	if(pastTenseDetected)
	{
		#ifdef GIA_TRANSLATOR_DEBUG
		//cout << "pastTenseDetected; entityIndex = " << feature->entityIndex << endl;
		#endif
		feature->grammaticalTense = GRAMMATICAL_TENSE_PAST;		//fixed in 28 April 2012
	}
}

//Preconditions: extractGrammaticalInformationStanford()/extractGrammaticalInformationFromPOStag() must be executed before relations (eg aux/cop) are processed, as they may [possibly] overwrite the tenses here established
void extractGrammaticalInformationFromPOStag(string * POStag, Feature * feature)
{
	//past tense extraction;
	//this is required for past tense verbs without auxillaries; eg He ran fast.     nsubj ( ran-2 , He-1 ), advmod ( ran-2 , fast-3 ) .
	extractPastTenseFromPOStag(POStag, feature);


	//progressives tense extraction;
	bool progressiveDetected = false;
	for(int i=0; i<FEATURE_POS_TAG_VERB_PROGRESSIVE_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagVerbProgressiveArray[i])
		{
			progressiveDetected = true;
		}
	}
	if(progressiveDetected)
	{
		feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PROGRESSIVE] = true;
	}

	//infinitive tense extraction (added 28 July 2013);
	bool infinitiveDetected = false;
	for(int i=0; i<FEATURE_POS_TAG_VERB_INFINITIVE_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagVerbInfinitiveArray[i])
		{
			infinitiveDetected = true;
		}
	}
	if(infinitiveDetected)
	{
		feature->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_INFINITIVE] = true;
	}

	//singular/plural detection;
	bool pluralDetected = false;
	for(int i=0; i<FEATURE_POS_TAG_PLURAL_NOUN_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagPluralNounArray[i])
		{
			pluralDetected = true;
		}
	}
	if(pluralDetected)
	{
		feature->grammaticalNumber = GRAMMATICAL_NUMBER_PLURAL;
	}
	bool singularDetected = false;
	for(int i=0; i<FEATURE_POS_TAG_SINGULAR_NOUN_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagSingularNounArray[i])
		{
			singularDetected = true;
		}
	}
	if(singularDetected)
	{
		feature->grammaticalNumber = GRAMMATICAL_NUMBER_SINGULAR;
	}

	//proper noun detection;
	bool properNounDetected = false;
	for(int i=0; i<FEATURE_POS_TAG_PROPER_NOUN_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == posTagProperNounArray[i])
		{
			properNounDetected = true;
		}
	}
	if(properNounDetected)
	{
		feature->grammaticalIsProperNoun = true;
	}

	//pronoun detection;
	bool pronounDetected = false;
	//use stanfordPOS information to extract pronoun information - NB alternatively, could use referenceTypePersonNameArray and referenceTypePossessiveNameArray (as there is only a limited set of pronouns in english)
	for(int i=0; i<FEATURE_POS_TAG_INDICATES_PRONOUN_NUMBER_OF_TYPES; i++)
	{
		if(*POStag == featurePOSindicatesPronounTypeArray[i])
		{
			feature->grammaticalIsPronoun = true;
		}
	}


}


void extractGrammaticalInformationStanford(Feature * firstFeatureInList, int NLPfeatureParser)
{
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		Feature * currentFeatureInList = firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{
			int currentFeatureIndex = currentFeatureInList->entityIndex;

			extractPOSrelatedGrammaticalInformationStanford(currentFeatureInList);

			if((currentFeatureInList->NER == FEATURE_NER_DATE) || (currentFeatureInList->NER == FEATURE_NER_TIME))
			{
				currentFeatureInList->grammaticalIsDateOrTime = true;
			}

			#ifdef GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_ASSIGNMENT_BUG_USE_NER_VALUES
			for(int i=0; i<FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_OF_TYPES; i++)
			{
				if(currentFeatureInList->NER == featureNERindicatesProperNounTypeArray[i])
				{
					currentFeatureInList->grammaticalIsProperNoun = true;
					#ifdef GIA_WORDNET_DEBUG
					cout << "isProperNoun currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
					#endif
				}
			}
			#endif

			/*//NB the GIAEntityNodeGrammaticalIsProperNounArray array for stanford core nlp does not relate to persons (only proper nouns)
			if(currentFeatureInList->NER == FEATURE_NER_PERSON)
			{
				currentFeatureInList->grammaticalIsProperNounArray = true;
			}
			*/

			/*
			cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
			cout << "currentFeatureInList->stanfordPOS = " << currentFeatureInList->stanfordPOS << endl;
			cout << "currentFeatureInList->type = " << currentFeatureInList->type << endl;
			cout << "currentFeatureInList->grammaticalWordType = " << currentFeatureInList->grammaticalWordType << endl;
			*/

			currentFeatureInList = currentFeatureInList->next;
		}
	}
}

void extractPOSrelatedGrammaticalInformationStanford(Feature * currentFeature)
{
	extractGrammaticalInformationFromPOStag(&(currentFeature->stanfordPOS), currentFeature);
	convertStanfordPOStagToRelexPOStypeAndWordnetWordType(&(currentFeature->stanfordPOS), &(currentFeature->type), &(currentFeature->grammaticalWordType));
}

void extractPastTense(Feature * featureWithEntityIndex, int entityIndexContainingTenseIndication, Feature * firstFeatureInList, int NLPfeatureParser)
{
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		//use the copular to set the tense of the noun

		Feature * currentFeatureInList = firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{
			if(currentFeatureInList->entityIndex == entityIndexContainingTenseIndication)
			{
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "extractPastTense; entityIndex = " << entityIndexContainingTenseIndication << endl;
				#endif
				extractPastTenseFromPOStag(&(currentFeatureInList->stanfordPOS), featureWithEntityIndex);
			}
			currentFeatureInList = currentFeatureInList->next;
		}
	}
}
#endif



#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILLARY_AND_SET_NOT_SAME_REFERENCE_SET
void findSubjObjRelationMatchingAuxillaryAndSetNotSameReferenceSet(Sentence * currentSentenceInList, int subjectObjectEntityWithAuxillaryEntityIndex, string * subjectObjectEntityWithAuxillaryEntityName)
{
	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			bool passed = false;
			for(int i=0; i<RELATION_TYPE_SUBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeSubjectNameArray[i])
				{
					passed = true;
				}
			}

			for(int i=0; i<RELATION_TYPE_OBJECT_NUMBER_OF_TYPES; i++)
			{
				if(currentRelationInList->relationType == relationTypeObjectNameArray[i])
				{
					passed = true;
				}
			}

			#ifndef GIA_USE_ADVANCED_REFERENCING_FIND_ALL_RELATIONS_MATCHING_AUXILLARY_AND_SET_DIFFERENT_REFERENCE_SET
			if(passed)
			{
			#endif
				if(subjectObjectEntityWithAuxillaryEntityIndex = currentRelationInList->relationGovernorIndex)	//CHECK THIS; this used to be currentRelationInList->relationDependentIndex [before 1 June 2012]
				{
					if(*subjectObjectEntityWithAuxillaryEntityName == currentRelationInList->relationGovernor)	//CHECK THIS; this used to be currentRelationInList->relationDependent [before 1 June 2012]
					{//this check is redundant
						currentRelationInList->auxillaryIndicatesDifferentReferenceSet = true;
						#ifdef GIA_ADVANCED_REFERENCING_DEBUG
						//cout << "\t\t\t-2currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
						//cout << "\t\t\t-2auxillaryIndicatesDifferentReferenceSet = " << currentRelationInList->auxillaryIndicatesDifferentReferenceSet << endl;
						#endif
					}
				}
			#ifndef GIA_USE_ADVANCED_REFERENCING_FIND_ALL_RELATIONS_MATCHING_AUXILLARY_AND_SET_DIFFERENT_REFERENCE_SET
			}
			#endif
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}
#endif



#ifdef GIA_USE_STANFORD_DEPENDENCY_RELATIONS
//NB GIAEntityNodeGrammaticalGenderArray is not currently filled by fillGrammaticalArraysStanford()
void fillGrammaticalArraysStanford(Sentence * currentSentenceInList,  bool GIAentityNodeArrayFilled[], GIAentityNode * GIAfeatureTempEntityNodeArray[], int NLPfeatureParser, Feature * featureArrayTemp[])
{
	//uses Stanford specific relations (grammar related)

	//past tense [preliminary only; aux/cop takes precedence], progressive tense, isDate, plurality, isProperNoun extraction
	extractGrammaticalInformationStanford(currentSentenceInList->firstFeatureInList, NLPfeatureParser);

	Relation * currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		if(!(currentRelationInList->disabled))
		{
		#endif
			//perfect tense extraction:
			if(currentRelationInList->relationType == RELATION_TYPE_MODAL_AUX)
			{
				#ifndef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				//eg aux (died, has) 	Reagan has died.	[addtogrammar: perfect?]
				int entityIndexOfAuxillary = currentRelationInList->relationDependentIndex;
				int entityIndexOfVerb = currentRelationInList->relationGovernorIndex;
				featureArrayTemp[entityIndexOfVerb]->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PERFECT] = true;
				GIAfeatureTempEntityNodeArray[entityIndexOfAuxillary]->disabled = true;

				/*//this shouldnt be required, as verbs are automatically assumed not to be part of same reference set [see DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS]
				#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILLARY_AND_SET_NOT_SAME_REFERENCE_SET
				findSubjObjRelationMatchingAuxillaryAndSetNotSameReferenceSet(currentSentenceInList, GIAentityNodeArray[entityIndexOfVerb]);
				#endif
				*/

				#ifdef GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILLARY_TO_SET_TENSE_OF_VERB
				extractPastTense(featureArrayTemp[entityIndexOfVerb], entityIndexOfAuxillary, currentSentenceInList->firstFeatureInList, NLPfeatureParser);
				#endif

			}

			//passive tense extraction:
			if(currentRelationInList->relationType == RELATION_TYPE_PASSIVE_AUX)
			{
				#ifndef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				//eg auxpass(killed, been) Kennedy has been killed. 	[addtogrammar: passive]
				int entityIndexOfAuxillary = currentRelationInList->relationDependentIndex;
				int entityIndexOfVerb = currentRelationInList->relationGovernorIndex;
				featureArrayTemp[entityIndexOfVerb]->grammaticalTenseModifierArray[GRAMMATICAL_TENSE_MODIFIER_PASSIVE] = true;
				GIAfeatureTempEntityNodeArray[entityIndexOfAuxillary]->disabled = true;

				/*//this shouldnt be required, as verbs are automatically assumed not to be part of same reference set [see DEFAULT_SAME_REFERENCE_SET_VALUE_FOR_ACTIONS]
				#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILLARY_AND_SET_NOT_SAME_REFERENCE_SET
				findSubjObjRelationMatchingAuxillaryAndSetNotSameReferenceSet(currentSentenceInList, GIAentityNodeArray[entityIndexOfVerb]);
				#endif
				*/

				#ifdef GIA_STANFORD_CORE_NLP_PARSER_USE_AUXILLARY_TO_SET_TENSE_OF_VERB
				extractPastTense(featureArrayTemp[entityIndexOfVerb], entityIndexOfAuxillary, currentSentenceInList->firstFeatureInList, GIAEntityNodeGrammaticalTenseArray, NLPfeatureParser);
				#endif

			}

			//past tense extraction:
			//NB copulas take precedence over auxillaries in formation of past tense (eg he has been sick;     nsubj ( sick-4 , he-1 ) / aux ( sick-4 , has-2 ) / cop ( sick-4 , been-3 ) )
			if(currentRelationInList->relationType == RELATION_TYPE_COPULA)
			{
				#ifndef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				//eg cop(smelled, sweet) 	The rose smelled sweet. [THIS APPEARS INCORRECT: stanford currently gives; acomp ( smelled-3 , sweet-4 )]
				//eg cop(black-5, was-4) 	Alice's cookie was black.

				int entityIndexOfCopula = currentRelationInList->relationDependentIndex;
				int entityIndexOfNoun = currentRelationInList->relationGovernorIndex;
				string entityNameOfNoun = currentRelationInList->relationGovernor;

				GIAfeatureTempEntityNodeArray[entityIndexOfCopula]->disabled = true;

				#ifdef GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILLARY_AND_SET_NOT_SAME_REFERENCE_SET
				findSubjObjRelationMatchingAuxillaryAndSetNotSameReferenceSet(currentSentenceInList, entityIndexOfNoun, &entityNameOfNoun);
				#endif

				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "entityIndexOfNoun = " << entityIndexOfNoun << endl;
				//cout << "entityIndexOfCopula = " << entityIndexOfCopula << endl;
				#endif
				extractPastTense(featureArrayTemp[entityIndexOfNoun], entityIndexOfCopula, currentSentenceInList->firstFeatureInList, NLPfeatureParser);

			}

			//future tense extraction:
			//overwrite current tense derivations with GRAMMATICAL_TENSE_FUTURE if there is an auxillary containing 'will'
			if(currentRelationInList->relationType == RELATION_TYPE_MODAL_AUX)	//|| (currentRelationInList->relationType == RELATION_TYPE_PASSIVE_AUX)
			{
				#ifndef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				int auxillaryDependencyIndex = currentRelationInList->relationGovernorIndex;
				string auxillaryGovernerEntity = currentRelationInList->relationDependent;
				for(int i=0; i<RELATION_TYPE_AUXILLARY_GOVERNER_INDICATES_FUTURE_TENSE_NUMBER_OF_TYPES; i++)
				{
					if(relationAuxillaryGovernerIndicatesFutureTenseArray[i] == auxillaryGovernerEntity)
					{
						featureArrayTemp[auxillaryDependencyIndex]->grammaticalTense = GRAMMATICAL_TENSE_FUTURE;
					}
				}
			}


			//definite/indefinite extraction:
			if(currentRelationInList->relationType == RELATION_TYPE_DETERMINER)
			{
				#ifndef GIA_DO_NOT_DISABLE_AUX_AND_COP_AT_START
				currentRelationInList->disabled = true;
				#endif

				//eg det(cookie, the) 	the cookie.
				string determiner = currentRelationInList->relationDependent;
				#ifdef GIA_TRANSLATOR_DEBUG
				//cout << "RELATION_TYPE_DETERMINER = " << RELATION_TYPE_DETERMINER << endl;
				//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				//cout << "determiner = " << determiner << endl;
				#endif
				int entityIndexOfDeterminier = currentRelationInList->relationDependentIndex;
				int entityIndexOfNoun = currentRelationInList->relationGovernorIndex;

				if((determiner == GRAMMATICAL_DETERMINER_DEFINITE) || (determiner == GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL) || (determiner == GRAMMATICAL_DETERMINER_INDEFINITE))
				{//if condition added 4 July 2013 to ensure only real determiners (the, some, a) are disabled [and not "What" in det(time-2, What-1)]

					GIAfeatureTempEntityNodeArray[entityIndexOfDeterminier]->disabled = true;
				}

				if((determiner == GRAMMATICAL_DETERMINER_DEFINITE) || (determiner == GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL))
				{
					featureArrayTemp[entityIndexOfNoun]->grammaticalIsDefinite = true;
					#ifdef GIA_USE_ADVANCED_REFERENCING
					featureArrayTemp[entityIndexOfNoun]->grammaticalIsDefiniteIndexOfDeterminer = currentRelationInList->relationDependentIndex;
					#endif

					#ifdef GIA_TRANSLATOR_DEBUG
					//cout << "(determiner == GRAMMATICAL_DETERMINER_DEFINITE)" << endl;
					//cout << "GIAentityNodeArray[entityIndexOfDeterminier]->entityName = " << GIAentityNodeArray[entityIndexOfDeterminier]->entityName << endl;
					//cout << "GIAentityNodeArray[entityIndexOfNoun]->entityName = " << GIAentityNodeArray[entityIndexOfNoun]->entityName << endl;
					#endif
				}
				/*
				else if(determiner == GRAMMATICAL_DETERMINER_INDEFINITE)
				{
					//no marking, in accordance with RelEx; 'doesn't mark [a] "book" at all'
				}
				*/

			}
		#ifdef GIA_DO_NOT_PARSE_DISABLED_RELATIONS_OLD
		}
		#endif

		currentRelationInList = currentRelationInList->next;
	}
}
#endif



void applyGrammaticalInfoToAllEntities(bool GIAentityNodeArrayFilled[], GIAentityNode * GIAentityNodeArray[], Feature * firstFeatureInSentence)
{
	int w = 1;
	Feature * currentFeatureInList = firstFeatureInSentence;
	while(currentFeatureInList->next != NULL)
	{
		if(GIAentityNodeArrayFilled[w])
		{
			GIAentityNode * entity = GIAentityNodeArray[w];
			#ifdef GIA_TRANSLATOR_DEBUG
			//cout << "entity->entityName = " << entity->entityName << endl;
			#endif
			#ifdef GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR
			entity->wordOrig = currentFeatureInList->word;
			#endif

			entity->hasAssociatedTime = currentFeatureInList->grammaticalIsDateOrTime;

			applyPOSrelatedGrammaticalInfoToEntity(entity, currentFeatureInList);

			entity->grammaticalDefiniteTemp = currentFeatureInList->grammaticalIsDefinite;
			entity->grammaticalProperNounTemp = currentFeatureInList->grammaticalIsProperNoun;
			entity->grammaticalGenderTemp = currentFeatureInList->grammaticalGender;
			#ifdef GIA_USE_ADVANCED_REFERENCING
			entity->grammaticalDefiniteIndexOfDeterminerTemp = currentFeatureInList->grammaticalIsDefiniteIndexOfDeterminer;
			//cout << "entity->grammaticalDefiniteIndexOfDeterminerTemp = " << entity->grammaticalDefiniteIndexOfDeterminerTemp << endl;
			#endif

			entity->NERTemp = currentFeatureInList->NER;
			#ifdef GIA_USE_STANFORD_CORENLP
			entity->NormalizedNERtemp = currentFeatureInList->NormalizedNER;
			entity->TimexTemp = currentFeatureInList->Timex;
			#endif

		}

		currentFeatureInList = currentFeatureInList->next;
		w++;
	}
}

void applyPOSrelatedGrammaticalInfoToEntity(GIAentityNode * entity, Feature * currentFeatureInList)
{
	for(int grammaticalTenseModifierIndex=0; grammaticalTenseModifierIndex<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES; grammaticalTenseModifierIndex++)
	{
		entity->grammaticalTenseModifierArrayTemp[grammaticalTenseModifierIndex] = currentFeatureInList->grammaticalTenseModifierArray[grammaticalTenseModifierIndex];
	}
	entity->grammaticalTenseTemp = currentFeatureInList->grammaticalTense;
	#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
	if(!(entity->isSubstanceConcept))
	{
	#endif
		entity->grammaticalNumber = currentFeatureInList->grammaticalNumber;
		//cout << "entity->grammaticalNumber = " << entity->grammaticalNumber << endl;
		//cout << "GRAM w = " << w << endl;
	#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
	}
	#endif

	entity->grammaticalPronounTemp = currentFeatureInList->grammaticalIsPronoun;
	entity->grammaticalWordTypeTemp = currentFeatureInList->grammaticalWordType;

	#ifdef GIA_USE_STANFORD_CORENLP
	entity->stanfordPOStemp = currentFeatureInList->stanfordPOS;
	#endif

	entity->foundPossibleInfinitiveVerbTemp = currentFeatureInList->foundPossibleInfinitiveVerb;
}



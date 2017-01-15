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
 * File Name: GIAnlpParser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2g3a 31-August-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Parses tabular subsections (Eg <relations>) of RelEx CFF/Stanford Parser File
 *
 *******************************************************************************/



#include "GIAnlpParser.h"
#include "GIAtranslatorDefs.h"	//required for convertStanfordRelationToRelex
#ifdef GIA_USE_LRP
#include "GIAlrp.h"
#endif
#define MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA 150 //max characters of some word in input data. includes '\0' at end of a string




//NB NLPrelexCompatibilityMode mode is only supported when !parseGIA2file; it is a special mode used when parsing Relex relations output with Stanford Compatibility Mode enabled
void GIATHparseStanfordParserRelationsText(string * relationsText, Sentence * currentSentenceInList, int * maxNumberOfWordsInSentence, bool featuresNotPreviouslyFilled, bool parseGIA2file, bool NLPrelexCompatibilityMode)
{
	//cout << "relationsText = " << *relationsText << endl;
	Relation * firstRelationInList = currentSentenceInList->firstRelationInList;
	Feature * firstFeatureInList = currentSentenceInList->firstFeatureInList;

	string relationType;
	string relationGovernor;
	string relationDependent;
	int relationGovernorIndex;
	int relationDependentIndex;

	#ifndef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
	*maxNumberOfWordsInSentence = 0;
	#endif

	int numberOfCharactersInRelationsText = relationsText->length();

	char currentItemString[MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA] = "";
	currentItemString[0] = '\0';

	/* Data file layout example

		det(fish-2, The-1)
		nsubj(swam-7, fish-2)
	*/

	Relation * currentRelation = firstRelationInList;
	#ifdef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
	while(currentRelation->next != NULL)
	{
		currentRelation = currentRelation->next;	//go to end of currently created relation list (as it may have already had secondary relations already added during NLPrelexCompatibilityMode)
	}
	#endif

	int relationIndex = 0;
	int characterIndex = 0;

	int currentRelationPart = 0;

	//cout << "GIATHparseStanfordParserRelationsText = " << *relationsText << endl;

	while(characterIndex < numberOfCharactersInRelationsText)
	{
		char c = (*relationsText)[characterIndex];

		if(c == CHAR_NEWLINE)
		{
			#ifdef GIA_USE_CORPUS_DATABASE
			if(parseGIA2file)
			{
				//eg actionObject(6-cake, 4-eaten) [sameReferenceSet=false]
				string sameReferenceSetString = currentItemString;
				//cout << "sameReferenceSetString = " << sameReferenceSetString << endl;
				if(sameReferenceSetString.find(createSameReferenceSetRecord2(true)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					currentRelation->sameReferenceSet = true;
				}
				else
				{
					currentRelation->sameReferenceSet = false;
				}
				//cout << "currentRelation->sameReferenceSet = " << currentRelation->sameReferenceSet << endl;
			}
			#endif

			currentRelation->relationType = relationType;
			currentRelation->relationGovernorIndex = relationGovernorIndex;
			currentRelation->relationDependentIndex = relationDependentIndex;
			//cout << "relationGovernor = " << relationGovernor << endl;
			//cout << "relationDependent = " << relationDependent << endl;
			//cout << "currentRelation->relationGovernorIndex = " << currentRelation->relationGovernorIndex << endl;
			//cout << "currentRelation->relationDependentIndex = " << currentRelation->relationDependentIndex << endl;

			#ifdef GIA_NLP_DEBUG
			/*
			Feature * currentFeatureInList = currentSentenceInList->firstFeatureInList;
			while(currentFeatureInList->next != NULL)
			{
				cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
				currentFeatureInList = currentFeatureInList->next;
			}
			*/
			#endif

			//cout << "h1" << endl;

			#ifdef GIA_NLP_DEBUG
			//cout << "convertStanfordRelationToRelex" << endl;
			#endif


			#ifdef GIA_NLP_DEBUG
			//cout << "finish: convertStanfordRelationToRelex" << endl;
			#endif

			currentRelation->relationGovernor = relationGovernor;	//this will be overwritten if !featuresNotPreviouslyFilled
			currentRelation->relationDependent = relationDependent;	//this will be overwritten if !featuresNotPreviouslyFilled
			if(!parseGIA2file)
			{
				convertStanfordRelationToRelex(currentRelation, currentSentenceInList);
			}

			if(!featuresNotPreviouslyFilled)
			{
				/*
				//don't use these, use lemmas instead (as per Stanford Core NLP/Relex dependency relation definitions)
				currentRelation->relationGovernor = relationGovernor;
				currentRelation->relationDependent = relationDependent;
				*/
				bool useLemmaFromFeatureSet = false;
				if(!parseGIA2file)
				{
					useLemmaFromFeatureSet = true;
				}
				else if(currentRelation->relationDependentIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
				{
					#ifdef GIA2_SUPPORT_QUERIES
					if(!parseGIA2file || !(findString(relationDependent, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)))	//!parseGIA2file condition added GIA 1d1a
					{
					#endif
						useLemmaFromFeatureSet = true;
					#ifdef GIA2_SUPPORT_QUERIES
					}
					#endif
				}
				//cout << "h3" << endl;

				Feature * currentFeatureInList = firstFeatureInList;
				//cout << "relationDependent = " << relationDependent << endl;
				//cout << "relationDependentIndex = " << relationDependentIndex << endl;
				if(currentRelation->relationDependentIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
				{
					for(int f=0; currentFeatureInList->entityIndex != currentRelation->relationDependentIndex; f++)
					{
						currentFeatureInList = currentFeatureInList->next;
					}
				}
				//cout << "h3a" << endl;
				if(useLemmaFromFeatureSet)
				{
					//cout << "h3b" << endl;
					//cout << "currentFeatureInList->lemma = " << currentFeatureInList->lemma << endl;

					currentRelation->relationDependent = currentFeatureInList->lemma;
					//cout << "h3c" << endl;
				}
				else
				{
					//cout << "h3d" << endl;
					//cout << "relationDependent = " << relationDependent << endl;
					currentRelation->relationDependent = relationDependent;	//eg "_measure" of _measure-993
					//cout << "h3e" << endl;
				}
				//cout << "h4" << endl;
				#ifdef GIA2_SUPPORT_QUERIES
				if(parseGIA2file)	//condition added 1d1a
				{
					if(findString(relationDependent, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE))
					{
						currentRelation->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
						if(currentRelation->relationDependentIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
						{
							currentFeatureInList->lemma = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
						}
						if(findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME) || findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME) || findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME))
						{
							currentRelation->corpusSpecialRelationDependentIsQuery = relationDependent.substr(REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_LENGTH, relationDependent.length()-REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_LENGTH);
						}
					}
					else
					{
						if(findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME) || findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME) || findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME))
						{
							currentRelation->corpusSpecialRelationDependentIsQuery = relationDependent;
						}
					}
				}
				#endif
				//cout << "h5" << endl;
				useLemmaFromFeatureSet = false;
				if(!parseGIA2file)
				{
					useLemmaFromFeatureSet = true;
				}
				else if(currentRelation->relationGovernorIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
				{
					#ifdef GIA2_SUPPORT_QUERIES
					if(!parseGIA2file || !(findString(relationGovernor, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)))	//!parseGIA2file condition added GIA 1d1a
					{
					#endif
						useLemmaFromFeatureSet = true;
					#ifdef GIA2_SUPPORT_QUERIES
					}
					#endif
				}
				//cout << "h6" << endl;
				currentFeatureInList = firstFeatureInList;
				if(currentRelation->relationGovernorIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
				{
					for(int f=0; currentFeatureInList->entityIndex != currentRelation->relationGovernorIndex; f++)
					{
						currentFeatureInList = currentFeatureInList->next;
					}
				}
				if(useLemmaFromFeatureSet)
				{
					currentRelation->relationGovernor = currentFeatureInList->lemma;
					//cout << "currentRelation->relationGovernor = " << currentRelation->relationGovernor << endl;
				}
				else
				{
					//cout << "relationGovernor = " << relationGovernor << endl;
					currentRelation->relationGovernor = relationGovernor;	//eg "_measure" of _measure-993
				}
				//cout << "h7" << endl;
				#ifdef GIA2_SUPPORT_QUERIES
				if(parseGIA2file)	//condition added 1d1a
				{
					if(findString(relationGovernor, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE))
					{
						currentRelation->relationGovernor = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
						if(currentRelation->relationGovernorIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
						{
							currentFeatureInList->lemma = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
						}
						if(findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME) || findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME) || findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME))
						{
							currentRelation->corpusSpecialRelationGovernorIsQuery = relationGovernor.substr(REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_LENGTH, relationGovernor.length()-REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_LENGTH);
						}
					}
					else
					{
						if(findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME) || findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME) || findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME))
						{
							currentRelation->corpusSpecialRelationGovernorIsQuery = relationGovernor;
						}
					}
					//cout << "h8" << endl;
				}
				#endif
			}

			//cout << "h2" << endl;

			#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
			cout << "relation added;" << endl;
			cout << "currentRelation->relationType = " << currentRelation->relationType << endl;
			cout << "currentRelation->relationGovernor = " << currentRelation->relationGovernor << endl;
			cout << "currentRelation->relationDependent = " << currentRelation->relationDependent << endl;
			cout << "currentRelation->relationGovernorIndex = " << currentRelation->relationGovernorIndex << endl;
			cout << "currentRelation->relationDependentIndex = " << currentRelation->relationDependentIndex << endl;
			//cout << currentRelation->relationType << "(" << currentRelation->relationGovernor << ", " << currentRelation->relationDependent << ")" << endl;
			#endif

			#ifdef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
			bool secondaryRelationDetected = false;
			if(NLPrelexCompatibilityMode)
			{
				for(int i=0; i<GIA2_SYNTACTIC_DEPENDENCY_RELATION_SECONDARY_NUMBER_OF_TYPES; i++)
				{
					if(currentRelation->relationType == GIA2syntacticDependencyRelationSecondaryNameArray[i])
					{
						//cout << "secondaryRelationDetected" << endl;
						secondaryRelationDetected = true;
					}
				}
			}
			if(!NLPrelexCompatibilityMode || secondaryRelationDetected)
			{
				//cout << "(!NLPrelexCompatibilityMode || secondaryRelationDetected): secondaryRelationDetected = " << secondaryRelationDetected << endl;
			#endif

				#ifdef GIA2_CONNECTIONIST_NETWORK
				bool foundReplicateRelation = false;
				if(parseGIA2file)
				{
					if(findReplicateRelation(currentSentenceInList, currentRelation))
					{
						foundReplicateRelation = true;
					}
				}
				if(!parseGIA2file || !foundReplicateRelation)
				{
				#endif
					#ifdef GIA_NLP_PARSER_STANFORD_PARSER_DISABLE_ROOT_RELATION
					if(currentRelation->relationType != RELATION_TYPE_ROOT)
					{
						Relation * newRelation = new Relation();
						currentRelation->next = newRelation;
						currentRelation = currentRelation->next;
					}
					#else
					Relation * newRelation = new Relation();
					currentRelation->next = newRelation;
					currentRelation = currentRelation->next;
					#endif
				#ifdef GIA2_CONNECTIONIST_NETWORK
				}
				#endif
			#ifdef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
			}
			#endif

			currentRelationPart = 0;
			currentItemString[0] = '\0';

			relationIndex++;
		}
		else if(c == CHAR_OPEN_BRACKET)
		{
			relationType = currentItemString;

			currentItemString[0] = '\0';
			currentRelationPart++;
		}
		else if(c == CHAR_DASH)
		{
			if(currentRelationPart == 1)
			{
				relationGovernor = currentItemString;
			}
			else if(currentRelationPart == 2)
			{
				relationDependent = currentItemString;
			}
			currentItemString[0] = '\0';
		}
		else if((c == CHAR_COMMA) || (c == CHAR_CLOSE_BRACKET))
		{
			bool foundEndOfEntry = false;
			if(c == CHAR_COMMA)
			{
				characterIndex++;	//skip space after ,
				c = (*relationsText)[characterIndex];
				if(c == CHAR_SPACE)
				{
					foundEndOfEntry = true;
				}
				else
				{
					//eg 50,000 in quantmod(50,000-8, than-7)
					char characterString[2];
					characterString[0] = CHAR_COMMA;
					characterString[1] = '\0';
					strcat(currentItemString, characterString);

					characterString[0] = c;
					characterString[1] = '\0';
					strcat(currentItemString, characterString);
				}
			}
			else
			{
				foundEndOfEntry = true;
			}

			if(foundEndOfEntry)
			{
				if(currentRelationPart == 1)
				{
					relationGovernorIndex = int(atof(currentItemString));
				}
				else if(currentRelationPart == 2)
				{
					relationDependentIndex = int(atof(currentItemString));
				}

				if(currentRelation->relationGovernorIndex > *maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationGovernorIndex;	//added GIA 2d1a
				}
				if(currentRelation->relationDependentIndex > *maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationDependentIndex;
				}

				currentItemString[0] = '\0';
				currentRelationPart++;
			}

		}
		else
		{
			char characterString[2];
			characterString[0] = c;
			characterString[1] = '\0';
			strcat(currentItemString, characterString);
		}

		characterIndex++;
	}
}

bool findReplicateRelation(Sentence * currentSentenceInList, Relation * relation)
{
	bool foundReplicateRelation = false;
	Relation * currentRelation = currentSentenceInList->firstRelationInList;
	while(currentRelation->next != NULL)
	{
		if(currentRelation != relation)
		{//do not compare relation to itself
			if(compareRelations(currentRelation, relation))
			{
				//cout << "foundReplicateRelation" << endl;
				foundReplicateRelation = true;
			}
		}
		currentRelation = currentRelation->next;
	}
	return foundReplicateRelation;
}

bool compareRelations(Relation * relation1, Relation * relation2)
{
	bool result = false;
	if((relation1->relationType == relation2->relationType) &&
	(relation1->relationDependent == relation2->relationDependent) &&
	(relation1->relationDependentIndex == relation2->relationDependentIndex) &&
	(relation1->relationGovernor == relation2->relationGovernor) &&
	(relation1->relationGovernorIndex == relation2->relationGovernorIndex))
	{
		result = true;
	}
	return result;
}

#ifdef GIA2_SUPPORT_QUERIES
bool findString(string entityName, string stringToFind)
{
	bool foundqVar = false;
	if(entityName.find(stringToFind) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		foundqVar = true;
	}
	return foundqVar;
}
#endif

#ifdef GIA_USE_CORPUS_DATABASE
string createSameReferenceSetRecord2(bool sameReferenceSet)
{
	string sameReferenceSetRecord = "[sameReferenceSet=" + convertBoolToString(sameReferenceSet) + "]";
	return sameReferenceSetRecord;
}
#endif

void GIATHparseStanfordParseWordsAndPOStagsText(string * POStagsText, Sentence * currentSentenceInList, int * maxNumberOfWordsInSentence, bool createFeaturesGIA2only)
{
	Feature * firstFeatureInList = currentSentenceInList->firstFeatureInList;
	Feature * currentFeatureInList = firstFeatureInList;

	*maxNumberOfWordsInSentence = 0;

	int numberOfCharactersInWordsAndPOSTagsText = POStagsText->length();

	char currentItemString[MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA] = "";
	string wordOrig = "";
	string stanfordPOS = "";
	currentItemString[0] = '\0';

	/* Data file layout example

	The/DT strongest/JJS rain/NN ever/RB recorded/VBN in/IN India/NNP
	shut/VBD down/RP the/DT financial/JJ hub/NN of/IN Mumbai/NNP ,/,
	snapped/VBD communication/NN lines/NNS ,/, closed/VBD airports/NNS
	and/CC forced/VBD thousands/NNS of/IN people/NNS to/TO sleep/VB in/IN
	their/PRP$ offices/NNS or/CC walk/VB home/NN during/IN the/DT night/NN
	,/, officials/NNS said/VBD today/NN ./.

	*/

	bool readingWord = true;	//else reading POS tag
	int characterIndex = 0;

	while(characterIndex < numberOfCharactersInWordsAndPOSTagsText)
	{
		char c = (*POStagsText)[characterIndex];

		if(c == CHAR_NEWLINE || c == CHAR_SPACE)
		{
			if(readingWord)
			{
				cout << "GIATHparseStanfordParseWordsAndPOStagsText() error: (c == CHAR_NEWLINE) && readingWord" << endl;
			}
			else
			{
				stanfordPOS = currentItemString;
				#ifdef GIA2_CONNECTIONIST_NETWORK
				if(createFeaturesGIA2only)
				{
					string GIAconnectionistNetworkPOStypeName = stanfordPOS;
					int GIAconnectionistNetworkPOStype = 0;	//ie GIA_CONNECTIONIST_NETWORK_POS_TYPE_UNDEFINED;
					for(int i=0; i<GIA_CONNECTIONIST_NETWORK_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
					{
						if(GIAconnectionistNetworkPOStypeName == GIAconnectionistNetworkPOStypeNameArray[i])
						{
							GIAconnectionistNetworkPOStype = i;
						}
					}
					currentFeatureInList->GIAconnectionistNetworkPOStype = GIAconnectionistNetworkPOStype;
					currentFeatureInList->word = wordOrig;
					currentFeatureInList->lemma = wordOrig; //NB for GIA2 semantic relations file lemmas are stored instead of wordOrigs (but these are not used by GIA2 anyway; just for debugging)
					Feature * newFeature = new Feature();
					newFeature->previous = currentFeatureInList;
					currentFeatureInList->next = newFeature;
				}
				else
				{
				#endif
					#ifdef STANFORD_PARSER_USE_POS_TAGS	//overwrite
					currentFeatureInList->stanfordPOS = stanfordPOS;
					#endif
				#ifdef GIA2_CONNECTIONIST_NETWORK
				}
				#endif

				#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
				cout << "DEBUG: GIATHparseStanfordParseWordsAndPOStagsText(): currentFeatureInList->stanfordPOS = " << currentFeatureInList->stanfordPOS << endl;
				#endif

				*maxNumberOfWordsInSentence = *maxNumberOfWordsInSentence + 1;

				currentFeatureInList = currentFeatureInList->next;
				readingWord = true;
				currentItemString[0] = '\0';
				stanfordPOS = "";
				wordOrig = "";
			}
		}
		else if(c == CHAR_FORWARDSLASH)
		{
			wordOrig = currentItemString;
			readingWord = false;
			currentItemString[0] = '\0';
		}
		else
		{
			char characterString[2];
			characterString[0] = c;
			characterString[1] = '\0';
			strcat(currentItemString, characterString);
		}

		characterIndex++;
	}
}

void convertStanfordRelationToRelex(Relation * currentRelationInList, Sentence * currentSentenceInList)
{
	string stanfordRelation = currentRelationInList->relationType;

	//prepend '_'
	string relationTypeRelexStandard = "";
	relationTypeRelexStandard = relationTypeRelexStandard + RELEX_DEPENDENCY_RELATION_PREPENDITION + stanfordRelation;

	//now deal with anamolies between dependency relation definitions;
	for(int i=0; i<GIA_NUMBER_OF_RELEX_VERSUS_STANFORD_DEPENDENCY_RELATION_DISCREPANCIES; i++)
	{
		#ifdef GIA_NLP_DEBUG
		//cout << "relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD][i] = " << relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD][i] << endl;
		//cout << "relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_RELEX][i] = " << relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_RELEX][i] << endl;
		#endif

		if(stanfordRelation == relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD][i])
		{
			relationTypeRelexStandard = relexVersusStanfordDependencyRelations[GIA_DEPENDENCY_RELATIONS_TYPE_RELEX][i];
		}
	}

	bool stanfordPrepositionFound = false;
	string tempRelexPrepositionString = convertPrepositionToRelex2(&stanfordRelation, &stanfordPrepositionFound);
	if(stanfordPrepositionFound)
	{
		relationTypeRelexStandard = stanfordRelation;	//do not modify stanford preposition relations "prep_...." to "_prep_..."
	}

	#ifdef GIA_USE_LRP
	//if(stanfordPrepositionFound)
	//{
	if(getUseLRP())
	{
		//if necessary revert temporary/dummy NLP multiword preposition to official LRP form
		bool foundOfficialLRPreplacementString = false;
		Feature * tempFeature = new Feature();
		tempFeature->word = tempRelexPrepositionString;
		revertNLPtagNameToOfficialLRPtagName(tempFeature, currentSentenceInList, currentRelationInList, true, &foundOfficialLRPreplacementString);
		if(foundOfficialLRPreplacementString)
		{
			string officialLRPentityName = tempFeature->word;
			if(stanfordPrepositionFound)
			{
				relationTypeRelexStandard = "";
				relationTypeRelexStandard = relationTypeRelexStandard + STANFORD_PARSER_PREPOSITION_PREPEND + officialLRPentityName;
				#ifdef GIA_NLP_DEBUG
				//cout << "stanfordPrepositionFound" << endl;
				//cout << "relationTypeRelexStandard = " << relationTypeRelexStandard << endl;
				#endif
			}
			else
			{
				cout << "!stanfordPrepositionFound" << endl;
				relationTypeRelexStandard = officialLRPentityName;
			}
			#ifdef GIA_LRP_DEBUG
			cout << "convertStanfordRelationToRelex() foundOfficialLRPreplacementString: tempRelexPrepositionString = " << tempRelexPrepositionString << ", relationTypeRelexStandard= " << relationTypeRelexStandard << endl;
			#endif
			//currentRelationInList->relationTypeForNLPonly = relationTypeRelexStandard;	//not required
		}
		delete tempFeature;
	}
	//}
	#endif
	currentRelationInList->relationType = relationTypeRelexStandard;
	#ifdef GIA_NLP_DEBUG
	//cout << "relationTypeRelexStandard = " << relationTypeRelexStandard << endl;
	//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
	#endif
}

//assumes prepositions have previously been converted to stanford prep_preposition format during preprocessor (for robustness)
string convertPrepositionToRelex2(string * preposition, bool * prepositionFound)
{
	*prepositionFound = false;
	string relexPreposition = *preposition;
	for(int i=0; i<REFERENCE_TYPE_STANFORD_PARSER_PREPOSITION_PREPEND_NUMBER_OF_TYPES; i++)
	{
		string currentStanfordPrepositionPrepend = referenceTypeStanfordParserPrepositionPrependNameArray[i];
		int foundStanfordPrepositionPrepend = preposition->find(currentStanfordPrepositionPrepend);

		if(foundStanfordPrepositionPrepend != string::npos)
		{
			int indexOfFirstRealCharacterInPreposition = currentStanfordPrepositionPrepend.length();
			int lengthOfPreposition = preposition->length() - (indexOfFirstRealCharacterInPreposition);
			relexPreposition = preposition->substr(indexOfFirstRealCharacterInPreposition, lengthOfPreposition);
			*prepositionFound = true;
		}
	}

	return relexPreposition;
}

void GIATHparseRelexFeaturesText(string * featuresText, Sentence * currentSentenceInList, int * maxNumberOfWordsInSentence)
{
	Feature * firstFeatureInList = currentSentenceInList->firstFeatureInList;
	*maxNumberOfWordsInSentence = 0;

	int numberOfCharactersInRelationsText = featuresText->length();

	char currentItemString[MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA] = "";
	currentItemString[0] = '\0';

	/* Data file layout example

		1	joe	joe	noun	uncountable|.n
		2	went	go	verb	past|.v-d
		3	sailing	sail	verb	progressive|.v
		4	on	on	prep
		5	dateID1	12 December 2010.	noun	date
		6	.	.	punctuation

	*/

	Feature * currentFeature = firstFeatureInList;

	int featureIndex = 1;
	int characterIndex = 0;

	characterIndex++;	//skip first new line in .cff file

	int currentFeaturePart = 0;

	while (characterIndex < numberOfCharactersInRelationsText)
	{
		char c = (*featuresText)[characterIndex];

		switch(c)
		{
			case CHAR_NEWLINE:
			{
				switch(currentFeaturePart)
				{
					case 4:
					{
						currentFeature->grammar = currentItemString;
						break;
					}
				}

			#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
				//identify is sentence is a question
				#ifdef GIA_NLP_DEBUG
				//cout << "currentFeature->type = " << currentFeature->type << endl;
				#endif
				if(currentFeature->type == FEATURE_RELEX_POS_TYPE_PUNCTUATION_NAME)
				{
					if(currentFeature->word == FEATURE_WORD_QUESTIONMARK)
					{
						#ifdef GIA_NLP_DEBUG
						//cout << "isQuestion == true" << endl;
						#endif
						currentSentenceInList->isQuestion = true;
					}
				}
			#endif

				#ifdef GIA_USE_LRP
				if(getUseLRP())
				{
					bool foundOfficialLRPreplacementString = false;
					Relation * currentRelationInListForPrepositionsOnlyIrrelevant = NULL;
					revertNLPtagNameToOfficialLRPtagName(currentFeature, currentSentenceInList, currentRelationInListForPrepositionsOnlyIrrelevant, false, &foundOfficialLRPreplacementString);
				}
				#endif

				#ifdef GIA_NLP_DEBUG
				/*
				cout << "feature added;" << endl;
				cout << "currentFeature->entityIndex = " << currentFeature->entityIndex << endl;
				cout << "currentFeature->word = " << currentFeature->word << endl;
				cout << "currentFeature->lemma = " << currentFeature->lemma << endl;
				cout << "currentFeature->type = " << currentFeature->type << endl;
				cout << "currentFeature->grammar = " << currentFeature->grammar << endl;
				*/
				#endif

				Feature * newFeature = new Feature();
				newFeature->previous = currentFeature;
				currentFeature->next = newFeature;
				currentFeature = currentFeature->next;
				*maxNumberOfWordsInSentence = *maxNumberOfWordsInSentence + 1;

				currentFeaturePart = 0;
				currentItemString[0] = '\0';

				featureIndex++;

				break;
			}
			case CHAR_TAB:
			{
				switch(currentFeaturePart)
				{
					case 0:
					{
						currentFeature->entityIndex = int(atof(currentItemString));
						if(currentFeature->entityIndex != featureIndex)
						{
							cout << "features parse error: (currentFeature->entityIndex != featureIndex)" << endl;
						}
						break;
					}
					case 1:
					{
						currentFeature->word = currentItemString;
						break;
					}
					case 2:
					{
						currentFeature->lemma = currentItemString;
						break;
					}
					case 3:
					{
						currentFeature->type = currentItemString;
						break;
					}
					case 4:
					{
						currentFeature->grammar = currentItemString;
						break;
					}
				}
				currentItemString[0] = '\0';
				currentFeaturePart++;

				break;
			}
			default:
			{
				char characterString[2];
				characterString[0] = c;
				characterString[1] = '\0';
				strcat(currentItemString, characterString);
				break;
			}
		}

		characterIndex++;
	}

}

void GIATHparseRelexRelationsText(string * relationsText, Sentence * currentSentenceInList, int * maxNumberOfWordsInSentence, bool NLPrelexCompatibilityMode)
{
	Relation * firstRelationInList = currentSentenceInList->firstRelationInList;
	int currentSentence = currentSentenceInList->sentenceIndex;

	#ifndef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
	*maxNumberOfWordsInSentence = 0;
	#endif

	int numberOfCharactersInRelationsText = relationsText->length();

	char currentItemString[MAX_CHARACTERS_OF_WORD_IN_GIA_INPUT_DATA] = "";
	currentItemString[0] = '\0';

	/* Data file layout example

		_subj(slip[4], hand[3])
		_poss(hand[3], John[1])
	*/

	Relation * currentRelation = firstRelationInList;
	#ifdef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
	while(currentRelation->next != NULL)
	{
		currentRelation = currentRelation->next;	//go to end of currently created relation list (as it may have already had secondary relations already added during NLPrelexCompatibilityMode)
	}
	#endif

	int relationIndex = 0;
	int characterIndex = 0;

	characterIndex++;	//skip first new line in .cff file

	int currentRelationPart = 0;

	while (characterIndex < numberOfCharactersInRelationsText)
	{
		char c = (*relationsText)[characterIndex];

		switch(c)
		{
			case CHAR_NEWLINE:
			{
				if(NLPrelexCompatibilityMode)
				{
					convertStanfordRelationToRelex(currentRelation, currentSentenceInList);
				}

				#ifdef GIA_NLP_DEBUG
				/*
				cout << "relation added;" << endl;
				cout << "currentRelation->relationType = " << currentRelation->relationType << endl;
				cout << "currentRelation->relationGovernor = " << currentRelation->relationGovernor << endl;
				cout << "currentRelation->relationDependent = " << currentRelation->relationDependent << endl;
				cout << "currentRelation->relationGovernorIndex = " << currentRelation->relationGovernorIndex << endl;
				cout << "currentRelation->relationDependentIndex = " << currentRelation->relationDependentIndex << endl;
				*/
				#endif

				Relation * newRelation = new Relation();
				currentRelation->next = newRelation;
				currentRelation = currentRelation->next;

				currentRelationPart = 0;
				currentItemString[0] = '\0';

				relationIndex++;

				break;
			}
			case CHAR_OPEN_BRACKET:
			{
				string relationType = currentItemString;

				if(!NLPrelexCompatibilityMode)	//condition added 21 Jan 2014
				{
					//added 23 July 2013 - preprocess relex conj_or/conj_and as _conj_or/_conj_and
					if(relationType == RELATION_TYPE_CONJUGATION_AND_RAW)
					{
						relationType = RELATION_TYPE_CONJUGATION_AND;
					}
					else if(relationType == RELATION_TYPE_CONJUGATION_OR_RAW)
					{
						relationType = RELATION_TYPE_CONJUGATION_OR;
					}
					//added 23 July 2013 - preprocess relex prepositions to stanford format for robustness
					if(relationType[0] != RELATION_TYPE_RELEX_NON_PREPOSITION_FIRST_CHARACTER)
					{//not valid for REFERENCE_TYPE_QUESTION_QUERY_VARIABLEs that require to be interpreted as prepositions (these must be explicitly compensated for)...
						relationType = string(STANFORD_PARSER_PREPOSITION_PREPEND) + relationType;
					}
				}

				currentRelation->relationType = relationType;
				currentItemString[0] = '\0';
				currentRelationPart++;

				break;
			}
			case CHAR_CLOSE_BRACKET:
			{
				break;
			}
			case CHAR_OPEN_SQUARE_BRACKET:
			{
				if(currentRelationPart == 1)
				{
					currentRelation->relationGovernor = currentItemString;
				}
				else if(currentRelationPart == 2)
				{
					currentRelation->relationDependent = currentItemString;
				}
				currentItemString[0] = '\0';

				break;
			}
			case CHAR_CLOSE_SQUARE_BRACKET:
			{
				if(currentRelationPart == 1)
				{
					currentRelation->relationGovernorIndex = int(atof(currentItemString));
				}
				else if(currentRelationPart == 2)
				{
					currentRelation->relationDependentIndex = int(atof(currentItemString));
				}

				if(currentRelation->relationGovernorIndex > *maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationGovernorIndex;	//added GIA 2d1a
				}
				if(currentRelation->relationDependentIndex > *maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationDependentIndex;
				}

				currentItemString[0] = '\0';
				currentRelationPart++;

				break;
			}
			case CHAR_COMMA:
			{
				characterIndex++;	//skip space in .cff file directly after comma

				break;
			}
			default:
			{
				char characterString[2];
				characterString[0] = c;
				characterString[1] = '\0';
				strcat(currentItemString, characterString);
				break;
			}
		}

		characterIndex++;
	}

}



#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT

string generateRelexCFFfeatureTagContent(Feature * firstFeatureInList)
{
	string relexCFFFeatureTagContent = "";

	Feature * currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		/*
		if(!(currentFeatureInList->disabled))
		{
		*/
		relexCFFFeatureTagContent = relexCFFFeatureTagContent + "\n\t\t\t\t";

		char entityIndexString[10];
		sprintf(entityIndexString, "%d", currentFeatureInList->entityIndex);
		relexCFFFeatureTagContent = relexCFFFeatureTagContent + entityIndexString + CHAR_TAB + currentFeatureInList->word + CHAR_TAB + currentFeatureInList->lemma + CHAR_TAB + grammaticalWordTypeNameArray[currentFeatureInList->grammaticalWordType] + CHAR_TAB;
		relexCFFFeatureTagContent = relexCFFFeatureTagContent + featureRelexFlagTypeArray[currentFeatureInList->NER] + CHAR_VERTICAL_BAR;
		if(currentFeatureInList->grammaticalIsDateOrTime)
		{
			relexCFFFeatureTagContent = relexCFFFeatureTagContent + "isDateOrTime";
		}
		relexCFFFeatureTagContent = relexCFFFeatureTagContent + CHAR_VERTICAL_BAR;
		relexCFFFeatureTagContent = relexCFFFeatureTagContent + grammaticalTenseNameArray[currentFeatureInList->grammaticalTense];
		for(int q=0; q<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES;q++)
		{
			if(currentFeatureInList->grammaticalTenseModifierArray[q])
			{
				relexCFFFeatureTagContent = relexCFFFeatureTagContent + CHAR_UNDERSCORE + grammaticalTenseModifierNameArray[q];
			}
		}
		relexCFFFeatureTagContent = relexCFFFeatureTagContent + CHAR_VERTICAL_BAR;
		relexCFFFeatureTagContent = relexCFFFeatureTagContent + grammaticalNumberNameArray[currentFeatureInList->grammaticalNumber] + CHAR_VERTICAL_BAR;
		if(currentFeatureInList->grammaticalIsDefinite)
		{//effective update 2f11a 13-July-2014: no longer print indefinite plural information (GRAMMATICAL_DETERMINER_INDEFINITE_PLURAL) to Relex CFF
			relexCFFFeatureTagContent = relexCFFFeatureTagContent + GRAMMATICAL_DEFINITE_NAME + CHAR_VERTICAL_BAR;
		}
		if(currentFeatureInList->grammaticalIsProperNoun)
		{
			relexCFFFeatureTagContent = relexCFFFeatureTagContent + GRAMMATICAL_PROPERNOUN_NAME + CHAR_VERTICAL_BAR;
		}
		relexCFFFeatureTagContent = relexCFFFeatureTagContent + grammaticalGenderNameArray[currentFeatureInList->grammaticalGender] + CHAR_VERTICAL_BAR;
		if(currentFeatureInList->grammaticalIsPronoun)
		{
			relexCFFFeatureTagContent = relexCFFFeatureTagContent + GRAMMATICAL_PRONOUN_NAME + CHAR_VERTICAL_BAR;
		}

		#ifdef GIA_NLP_DEBUG
		/*
		cout << "Sentence Word Index = " << w << endl;
		cout << "Is Date or Time = " << convertBoolToString(GIAEntityNodeIsDateOrTime[w]);
		cout << "Tense = " << grammaticalTenseNameArray[GIAEntityNodeGrammaticalTenseArray[w]];
		for(int q=0; q<GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES;q++)
		{
			cout << "Tense Modifier (" << grammaticalTenseModifierNameArray[q] << ") = " << convertBoolToString(GIAEntityNodeGrammaticalTenseModifierArray[w*GRAMMATICAL_TENSE_MODIFIER_NUMBER_OF_TYPES + q]);
		}
		cout << "Plurality = " << grammaticalNumberNameArray[GIAEntityNodeGrammaticalNumberArray[w]];
		cout << "Is Definite = " << convertBoolToString(GIAEntityNodeGrammaticalIsDefiniteArray[w]);
		cout << "Is Proper Noun = " << convertBoolToString(GIAEntityNodeGrammaticalIsProperNounArray[w]);
		cout << "Gender = " << grammaticalGenderNameArray[GIAEntityNodeGrammaticalGenderArray[w]];
		cout << "Is Pronoun = " << convertBoolToString(GIAEntityNodeGrammaticalIsPronounArray[w]);
		cout << "Wordtype = " << grammaticalWordTypeNameArray[GIAEntityNodeWordTypeArray[w]];

		cout << "NER = " << featureRelexFlagTypeArray[GIAEntityNodeNERArray[w]];
		cout << "NormalizedNER = " << GIAEntityNodeNormalizedNERArray[w];
		cout << "Timex = " << GIAEntityNodeTimexArray[w];
		cout << "POS = " << GIAEntityNodePOSArray[w];
		*/
		#endif

		/*
		}
		*/
		currentFeatureInList = currentFeatureInList->next;
	}

	return relexCFFFeatureTagContent;

}

string generateRelexCFFrelationTagContent(Relation * firstRelationInList)
{
	string relexCFFRelationTagContent = "";

	Relation * currentRelationInList = firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			relexCFFRelationTagContent = relexCFFRelationTagContent + "\n\t\t\t\t";

			string relationType = currentRelationInList->relationType;
			string relationGoverner = currentRelationInList->relationGovernor;
			string relationDependent = currentRelationInList->relationDependent;
			char relationGovernorIndexString[10];
			char relationDependentIndexString[10];
			sprintf(relationGovernorIndexString, "%d", currentRelationInList->relationGovernorIndex);
			sprintf(relationDependentIndexString, "%d", currentRelationInList->relationDependentIndex);

			relexCFFRelationTagContent = relexCFFRelationTagContent + relationType + CHAR_OPEN_BRACKET + relationGoverner + CHAR_OPEN_SQUARE_BRACKET + relationGovernorIndexString + CHAR_CLOSE_SQUARE_BRACKET + CHAR_COMMA + CHAR_SPACE + relationDependent + CHAR_OPEN_SQUARE_BRACKET + relationDependentIndexString + CHAR_CLOSE_SQUARE_BRACKET + CHAR_CLOSE_BRACKET;

			#ifdef GIA_NLP_DEBUG
			/*
			cout << "relationType = " << currentRelationInList->relationType << endl;
			cout << "relationGoverner = " << relationGoverner->entityName << endl;
			cout << "relationDependent = " << relationDependent->entityName << endl;
			*/
			#endif
		}
		currentRelationInList = currentRelationInList->next;
	}

	return relexCFFRelationTagContent;
}



#endif



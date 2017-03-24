/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a1f 26-February-2017
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: Parses tabular subsections (Eg <relations>) of RelEx CFF/Stanford Parser File
 *
 *******************************************************************************/


#include "GIAnlpParser.hpp"
#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
#endif

//NB NLPrelexCompatibilityMode mode is only supported when !parseGIA2file; it is a special mode used when parsing Relex relations output with Stanford Compatibility Mode enabled
void GIAnlpParserClass::GIATHparseStanfordParserRelationsText(const string* relationsText, GIAsentence* currentSentenceInList, int* maxNumberOfWordsInSentence, const bool featuresNotPreviouslyFilled, const bool parseGIA2file, const bool NLPrelexCompatibilityMode)
{
	#ifdef GIA_DEBUG
	//cout << "relationsText = " <<* relationsText << endl;
	#endif
	GIArelation* firstRelationInList = currentSentenceInList->firstRelationInList;
	GIAfeature* firstFeatureInList = currentSentenceInList->firstFeatureInList;

	#ifdef GIA_DEBUG
	/*
	cout << "TEST:" << endl;
	GIAfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
		currentFeatureInList = currentFeatureInList->next;
	}
	*/
	#endif

	string relationType;
	string relationGovernor;
	string relationDependent;
	int relationGovernorIndex;
	int relationDependentIndex;

	#ifndef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
	*maxNumberOfWordsInSentence = 0;
	#endif

	int numberOfCharactersInRelationsText = relationsText->length();

	string currentItemString = "";

	/* Data file layout example

		det(fish-2, The-1)
		nsubj(swam-7, fish-2)
	*/

	GIArelation* currentRelation = firstRelationInList;
	#ifdef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
	while(currentRelation->next != NULL)
	{
		currentRelation = currentRelation->next;	//go to end of currently created relation list (as it may have already had secondary relations already added during NLPrelexCompatibilityMode)
	}
	#endif

	int relationIndex = 0;
	int characterIndex = 0;

	int currentRelationPart = 0;

	#ifdef GIA_DEBUG
	//cout << "GIATHparseStanfordParserRelationsText = " <<* relationsText << endl;
	#endif

	while(characterIndex < numberOfCharactersInRelationsText)
	{
		char c = (*relationsText)[characterIndex];

		if(c == CHAR_NEWLINE)
		{
			#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
			if(parseGIA2file)
			{
				//eg actionRelationshipObjectEntity(6-cake, 4-eaten) [sameReferenceSet=false]
				string sameReferenceSetString = currentItemString;
				#ifdef GIA_DEBUG
				//cout << "sameReferenceSetString = " << sameReferenceSetString << endl;
				#endif
				if(sameReferenceSetString.find(this->createSameReferenceSetRecord2(true)) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
				{
					currentRelation->sameReferenceSet = true;
				}
				else
				{
					currentRelation->sameReferenceSet = false;
				}
				#ifdef GIA_DEBUG
				//cout << "currentRelation->sameReferenceSet = " << currentRelation->sameReferenceSet << endl;
				//cout << "currentRelation->rcmodIndicatesSameReferenceSet = " << currentRelation->rcmodIndicatesSameReferenceSet << endl;
				#endif
			}
			#endif

			currentRelation->relationType = relationType;
			currentRelation->relationGovernorIndex = relationGovernorIndex;
			currentRelation->relationDependentIndex = relationDependentIndex;
			#ifdef GIA_DEBUG
			//cout << "relationGovernor = " << relationGovernor << endl;
			//cout << "relationDependent = " << relationDependent << endl;
			//cout << "currentRelation->relationGovernorIndex = " << currentRelation->relationGovernorIndex << endl;
			//cout << "currentRelation->relationDependentIndex = " << currentRelation->relationDependentIndex << endl;
			#endif

			#ifdef GIA_NLP_DEBUG
			/*
			GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;
			while(currentFeatureInList->next != NULL)
			{
				cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
				currentFeatureInList = currentFeatureInList->next;
			}
			*/
			#endif

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
				this->convertStanfordRelationToRelex(currentRelation, currentSentenceInList);
			}

			if(!featuresNotPreviouslyFilled)
			{
				/*
				//don't use these, use lemmas instead (as per Stanford Core NLP/Relex dependency relation definitions)
				currentRelation->relationGovernor = relationGovernor;
				currentRelation->relationDependent = relationDependent;
				*/

				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
				if(!(currentRelation->relationDependentRevertedToOfficialLRPTemp))
				#endif
				{
					bool useLemmaFromFeatureSet = false;
					if(!parseGIA2file)
					{
						useLemmaFromFeatureSet = true;
					}
					else
					{//parseGIA2file
						if(currentRelation->relationDependentIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
						{
							#ifdef GIA2_SUPPORT_QUERIES
							if(!(this->findString(relationDependent, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)))	//redundant !parseGIA2file condition added GIA1d1a, removed GIA2j6b
							{
							#endif
								useLemmaFromFeatureSet = true;
							#ifdef GIA2_SUPPORT_QUERIES
							}
							#endif
						}
					}

					GIAfeature* currentFeatureInList = firstFeatureInList;
					#ifdef GIA_DEBUG
					//cout << "relationDependent = " << relationDependent << endl;
					//cout << "relationDependentIndex = " << relationDependentIndex << endl;
					#endif
					if(currentRelation->relationDependentIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
					{
						for(int f=0; currentFeatureInList->entityIndex != currentRelation->relationDependentIndex; f++)
						{
							currentFeatureInList = currentFeatureInList->next;
						}
					}
					if(useLemmaFromFeatureSet)
					{
						currentRelation->relationDependent = currentFeatureInList->lemma;
						#ifdef GIA_DEBUG
						//cout << "currentRelation->relationDependent = " << currentRelation->relationDependent << endl;
						#endif
					}
					else
					{
						currentRelation->relationDependent = relationDependent;	//eg "_measure" of _measure-993
						#ifdef GIA_DEBUG
						//cout << "relationDependent = " << relationDependent << endl;
						#endif
					}
					#ifdef GIA2_SUPPORT_QUERIES
					if(parseGIA2file)	//condition added 1d1a
					{
						if(this->findString(relationDependent, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE))
						{
							//ie !useLemmaFromFeatureSet
							currentRelation->relationDependent = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
							if(currentRelation->relationDependentIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
							{
								currentFeatureInList->lemma = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
							}
							if(this->findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME) || this->findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME) || this->findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME))
							{
								currentRelation->corpusSpecialRelationDependentIsQuery = relationDependent.substr(REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_LENGTH, relationDependent.length()-REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_LENGTH);
							}
						}
						else
						{	//ie useLemmaFromFeatureSet
							if(this->findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME) || this->findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME) || this->findString(relationDependent, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME))
							{
								currentRelation->corpusSpecialRelationDependentIsQuery = relationDependent;
							}
						}
					}
					#endif
				}
				/*
				else
				{
					cout << "relationDependentRevertedToOfficialLRPTemp" << endl;
				}
				*/

				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
				if(!(currentRelation->relationGovernorRevertedToOfficialLRPTemp))
				#endif
				{
					bool useLemmaFromFeatureSet = false;
					if(!parseGIA2file)
					{
						useLemmaFromFeatureSet = true;
					}
					else
					{//parseGIA2file
						if(currentRelation->relationGovernorIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
						{
							#ifdef GIA2_SUPPORT_QUERIES
							if(!(this->findString(relationGovernor, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE)))	//redundant !parseGIA2file condition added GIA1d1a, removed GIA2j6b
							{
							#endif
								useLemmaFromFeatureSet = true;
							#ifdef GIA2_SUPPORT_QUERIES
							}
							#endif
						}
					}

					GIAfeature* currentFeatureInList = firstFeatureInList;
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
						#ifdef GIA_DEBUG
						//cout << "currentRelation->relationGovernor = " << currentRelation->relationGovernor << endl;
						#endif
					}
					else
					{
						currentRelation->relationGovernor = relationGovernor;	//eg "_measure" of _measure-993
						#ifdef GIA_DEBUG
						//cout << "relationGovernor = " << relationGovernor << endl;
						#endif
					}
					#ifdef GIA2_SUPPORT_QUERIES
					if(parseGIA2file)	//condition added 1d1a
					{
						if(this->findString(relationGovernor, REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE))
						{
							//ie !useLemmaFromFeatureSet
							currentRelation->relationGovernor = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
							if(currentRelation->relationGovernorIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
							{
								currentFeatureInList->lemma = REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE;
							}
							if(this->findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME) || this->findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME) || this->findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME))
							{
								currentRelation->corpusSpecialRelationGovernorIsQuery = relationGovernor.substr(REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_LENGTH, relationGovernor.length()-REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE_LENGTH);
							}
						}
						else
						{	//ie useLemmaFromFeatureSet
							if(this->findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_QUERY_TAG_TAG_NAME) || this->findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_NAME_QUERY_TAG_TAG_NAME) || this->findString(relationGovernor, GIA2_SUPPORT_QUERIES_SPECIAL_SEMANTIC_RELATION_IS_WHICH_OR_EQUIVALENT_WHAT_QUERY_TAG_TAG_NAME))
							{
								currentRelation->corpusSpecialRelationGovernorIsQuery = relationGovernor;
							}
						}
					}
					#endif
				}
				/*
				else
				{
					cout << "relationGovernorRevertedToOfficialLRPTemp" << endl;
				}
				*/

			}

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
						#ifdef GIA_DEBUG
						//cout << "secondaryRelationDetected" << endl;
						#endif
						secondaryRelationDetected = true;
					}
				}
			}
			if(!NLPrelexCompatibilityMode || secondaryRelationDetected)
			{
				#ifdef GIA_DEBUG
				//cout << "(!NLPrelexCompatibilityMode || secondaryRelationDetected): secondaryRelationDetected = " << secondaryRelationDetected << endl;
				#endif
			#endif

				#ifdef GIA2_SEMANTIC_PARSER
				bool foundReplicateRelation = false;
				if(parseGIA2file)
				{
					if(this->findReplicateRelation(currentSentenceInList, currentRelation))
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
						GIArelation* newRelation = new GIArelation();
						currentRelation->next = newRelation;
						currentRelation = currentRelation->next;
					}
					#else
					GIArelation* newRelation = new GIArelation();
					currentRelation->next = newRelation;
					currentRelation = currentRelation->next;
					#endif
				#ifdef GIA2_SEMANTIC_PARSER
				}
				#endif
			#ifdef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
			}
			#endif

			currentRelationPart = 0;
			currentItemString = "";

			relationIndex++;
		}
		else if(c == CHAR_OPEN_BRACKET)
		{
			relationType = currentItemString;

			currentItemString = "";
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
			currentItemString = "";
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
					currentItemString = currentItemString + CHAR_COMMA;
					currentItemString = currentItemString + c;
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
					relationGovernorIndex = int(SHAREDvars.convertStringToDouble(currentItemString));
				}
				else if(currentRelationPart == 2)
				{
					relationDependentIndex = int(SHAREDvars.convertStringToDouble(currentItemString));
				}

				if(currentRelation->relationGovernorIndex >* maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationGovernorIndex;	//added GIA 2d1a
				}
				if(currentRelation->relationDependentIndex >* maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationDependentIndex;
				}

				currentItemString = "";
				currentRelationPart++;
			}

		}
		else
		{
			currentItemString = currentItemString + c;
		}

		characterIndex++;
	}
}

bool GIAnlpParserClass::findReplicateRelation(const GIAsentence* currentSentenceInList, const GIArelation* relation)
{
	bool foundReplicateRelation = false;
	const GIArelation* currentRelation = currentSentenceInList->firstRelationInList;
	while(currentRelation->next != NULL)
	{
		if(currentRelation != relation)
		{//do not compare relation to itself
			if(this->compareRelations(currentRelation, relation))
			{
				#ifdef GIA_DEBUG
				//cout << "foundReplicateRelation" << endl;
				#endif
				foundReplicateRelation = true;
			}
		}
		currentRelation = currentRelation->next;
	}
	return foundReplicateRelation;
}

bool GIAnlpParserClass::compareRelations(const GIArelation* relation1, const GIArelation* relation2)
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
bool GIAnlpParserClass::findString(string entityName, string stringToFind)
{
	bool foundqVar = false;
	if(entityName.find(stringToFind) != CPP_STRING_FIND_RESULT_FAIL_VALUE)
	{
		foundqVar = true;
	}
	return foundqVar;
}
#endif

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
string GIAnlpParserClass::createSameReferenceSetRecord2(const bool sameReferenceSet)
{
	string sameReferenceSetRecord = "[sameReferenceSet=" + SHAREDvars.convertBoolToString(sameReferenceSet) + "]";
	return sameReferenceSetRecord;
}
#endif

void GIAnlpParserClass::GIATHparseStanfordParseWordsAndPOStagsText(const string* POStagsText, GIAsentence* currentSentenceInList, int* maxNumberOfWordsInSentence, const bool createFeaturesGIA2only)
{
	GIAfeature* firstFeatureInList = currentSentenceInList->firstFeatureInList;
	GIAfeature* currentFeatureInList = firstFeatureInList;

	*maxNumberOfWordsInSentence = 0;

	int numberOfCharactersInWordsAndPOSTagsText = POStagsText->length();

	string currentItemString = "";
	string wordOrig = "";
	string stanfordPOS = "";

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
				cout << "GIATHparseStanfordParseWordsAndPOStagsText{} error: (c == CHAR_NEWLINE) && readingWord" << endl;
				cout << "*POStagsText = " << *POStagsText << endl;
			}
			else
			{
				stanfordPOS = currentItemString;
				#ifdef GIA2_SEMANTIC_PARSER
				if(createFeaturesGIA2only)
				{
					string GIAconnectionistNetworkPOStypeName = stanfordPOS;
					int GIAsemanticParserPOStype = 0;	//ie GIA_SEMANTIC_PARSER_POS_TYPE_UNDEFINED;
					for(int i=0; i<GIA_SEMANTIC_PARSER_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES; i++)
					{
						if(GIAconnectionistNetworkPOStypeName == GIAconnectionistNetworkPOStypeNameArray[i])
						{
							GIAsemanticParserPOStype = i;
						}
					}
					currentFeatureInList->GIAsemanticParserPOStype = GIAsemanticParserPOStype;
					currentFeatureInList->word = wordOrig;
					currentFeatureInList->lemma = wordOrig; //NB for GIA2 semantic relations file lemmas are stored instead of wordOrigs (but these are not used by GIA2 anyway; just for debugging)
					GIAfeature* newFeature = new GIAfeature();
					newFeature->previous = currentFeatureInList;
					currentFeatureInList->next = newFeature;
				}
				else
				{
				#endif
					#ifdef STANFORD_PARSER_USE_POS_TAGS	//overwrite
					currentFeatureInList->stanfordPOS = stanfordPOS;
					#endif
				#ifdef GIA2_SEMANTIC_PARSER
				}
				#endif

				#ifdef GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
				cout << "currentItemString = " << currentItemString << endl;
				cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
				cout << "DEBUG: GIATHparseStanfordParseWordsAndPOStagsText{}: currentFeatureInList->stanfordPOS = " << currentFeatureInList->stanfordPOS << endl;
				#endif

				*maxNumberOfWordsInSentence = *maxNumberOfWordsInSentence + 1;

				currentFeatureInList = currentFeatureInList->next;
				readingWord = true;
				currentItemString = "";
				stanfordPOS = "";
				wordOrig = "";
			}
		}
		else if(c == CHAR_FORWARDSLASH)
		{
			wordOrig = currentItemString;
			readingWord = false;
			currentItemString = "";
		}
		else
		{
			currentItemString = currentItemString + c;
		}

		characterIndex++;
	}
}

void GIAnlpParserClass::convertStanfordRelationToRelex(GIArelation* currentRelationInList, const GIAsentence* currentSentenceInList)
{
	string stanfordRelation = currentRelationInList->relationType;

	//prepend '_'
	string relationTypeRelexStandard = "";
	relationTypeRelexStandard = relationTypeRelexStandard + RELEX_DEPENDENCY_RELATION_PREPENDITION + stanfordRelation;
	bool foundReducableStanfordRelation = false;
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
			foundReducableStanfordRelation = true;
		}
	}

	bool stanfordPrepositionFound = false;
	string tempRelexPrepositionString = this->convertPrepositionToRelex2(&stanfordRelation, &stanfordPrepositionFound);
	if(stanfordPrepositionFound)
	{
		relationTypeRelexStandard = stanfordRelation;	//do not modify stanford preposition relations "prep_...." to "_prep_..."
	}
	#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
	if(!foundReducableStanfordRelation)
	{
		#ifdef GIA_DEBUG
		//cout << "stanfordRelation = " << stanfordRelation << endl;
		#endif
		if(stanfordRelation.substr(0, RELATION_TYPE_PREPOSITION_MODIFIER2_LENGTH) == RELATION_TYPE_PREPOSITION_MODIFIER2)
		{
			stanfordPrepositionFound = true;
			tempRelexPrepositionString = stanfordRelation.substr(RELATION_TYPE_PREPOSITION_MODIFIER2_LENGTH, stanfordRelation.length()-RELATION_TYPE_PREPOSITION_MODIFIER2_LENGTH);
			relationTypeRelexStandard = string(STANFORD_PARSER_PREPOSITION_PREPEND) + tempRelexPrepositionString;	//converts _nmod:[preposition] to prep_[preposition]
		}
	}
	#endif

	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
	this->convertStanfordRelationToRelexLRPreversion(currentRelationInList, currentSentenceInList, stanfordPrepositionFound, tempRelexPrepositionString, &relationTypeRelexStandard);
	#endif

	currentRelationInList->relationType = relationTypeRelexStandard;
	#ifdef GIA_NLP_DEBUG
	//cout << "relationTypeRelexStandard = " << relationTypeRelexStandard << endl;
	//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
	#endif
}

#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
void GIAnlpParserClass::convertStanfordRelationToRelexLRPreversion(GIArelation* currentRelationInList, const GIAsentence* currentSentenceInList, const bool stanfordPrepositionFound, const string tempRelexPrepositionString, string* relationTypeRelexStandard)
{
	//if(stanfordPrepositionFound)
	//{
	if(GIApreprocessorMultiwordReduction.getUseLRP())
	{
		//if necessary revert temporary/dummy NLP multiword preposition to official LRP form
		bool foundOfficialLRPreplacementString = false;
		GIAfeature* tempFeature = new GIAfeature();
		tempFeature->word = tempRelexPrepositionString;
		#ifdef GIA_DEBUG
		//cout << "\n\t\tconvertStanfordRelationToRelexLRPreversion: tempRelexPrepositionString = " << tempRelexPrepositionString << endl;
		//cout << "\t\tcurrentRelationInList->relationGovernor: " << currentRelationInList->relationGovernor << endl;
		//cout << "\t\tcurrentRelationInList->relationDependent: " << currentRelationInList->relationDependent << endl;
		#endif
		GIApreprocessorMultiwordReduction.revertNLPtagNameToOfficialLRPtagName(tempFeature, currentSentenceInList, currentRelationInList, true, &foundOfficialLRPreplacementString);
		if(foundOfficialLRPreplacementString)
		{
			string officialLRPentityName = tempFeature->word;
			if(stanfordPrepositionFound)
			{
				*relationTypeRelexStandard = "";
				*relationTypeRelexStandard = *relationTypeRelexStandard + STANFORD_PARSER_PREPOSITION_PREPEND + officialLRPentityName;
				#ifdef GIA_NLP_DEBUG
				cout << "stanfordPrepositionFound" << endl;
				cout << "relationTypeRelexStandard = " << relationTypeRelexStandard << endl;
				#endif
			}
			else
			{
				cout << "!stanfordPrepositionFound" << endl;
				*relationTypeRelexStandard = officialLRPentityName;
			}
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DEBUG
			cout << "convertStanfordRelationToRelexLRPreversion{} foundOfficialLRPreplacementString: tempRelexPrepositionString = " << tempRelexPrepositionString << ", *relationTypeRelexStandard= " << *relationTypeRelexStandard << endl;
			#endif
		}
		delete tempFeature;

		#ifdef GIA_STANFORD_PARSER_AND_CORENLP_VERSION_2015_04_20_OR_GREATER
		//takes into account the fact prepositions are often defined via case(city-6, near-4) instead of prep_near(is-3, city-6) with Stanford 2015-04-20+ [ie are not necessarily parsed as relationType]:
		foundOfficialLRPreplacementString = false;
		tempFeature = new GIAfeature();
		string relationGovernorForNLPonly = currentRelationInList->relationGovernor;
		tempFeature->word = relationGovernorForNLPonly;
		tempFeature->entityIndex = currentRelationInList->relationGovernorIndex;
		GIApreprocessorMultiwordReduction.revertNLPtagNameToOfficialLRPtagName(tempFeature, currentSentenceInList, currentRelationInList, false, &foundOfficialLRPreplacementString);
		if(foundOfficialLRPreplacementString)
		{
			string officialLRPentityName = tempFeature->word;
			currentRelationInList->relationGovernor = officialLRPentityName;
			currentRelationInList->relationGovernorRevertedToOfficialLRPTemp = true;
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DEBUG
			cout << "convertStanfordRelationToRelexLRPreversion{} foundOfficialLRPreplacementString: relationGovernorForNLPonly = " << relationGovernorForNLPonly << ", currentRelationInList->relationGovernor= " << currentRelationInList->relationGovernor << endl;
			#endif
		}
		delete tempFeature;
		foundOfficialLRPreplacementString = false;
		tempFeature = new GIAfeature();
		string relationDependentForNLPonly = currentRelationInList->relationDependent;
		tempFeature->word = relationDependentForNLPonly;
		tempFeature->entityIndex = currentRelationInList->relationDependentIndex;
		GIApreprocessorMultiwordReduction.revertNLPtagNameToOfficialLRPtagName(tempFeature, currentSentenceInList, currentRelationInList, false, &foundOfficialLRPreplacementString);
		if(foundOfficialLRPreplacementString)
		{
			string officialLRPentityName = tempFeature->word;
			currentRelationInList->relationDependent = officialLRPentityName;
			currentRelationInList->relationDependentRevertedToOfficialLRPTemp = true;
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DEBUG
			cout << "convertStanfordRelationToRelexLRPreversion{} foundOfficialLRPreplacementString: relationDependentForNLPonly = " << relationDependentForNLPonly << ", currentRelationInList->relationDependent= " << currentRelationInList->relationDependent << endl;
			#endif
		}
		delete tempFeature;
		#endif
	}
	//}
}
#endif

/*
#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER
void convertGIAsemanticRelation(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList)
{
	#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION
	convertGIAsemanticRelationLRPreversion(currentRelationInList, currentSentenceInList);
	#endif
}

#ifdef #ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
void convertGIAsemanticRelationLRPreversion(GIArelation* currentRelationInList, GIAsentence* currentSentenceInList)
{
	if(getUseLRP())
	{
		//if necessary revert temporary/dummy NLP multiword preposition to official LRP form
		if(currentRelationInList->relationGovernorIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
		{
			GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;
			for(int f=0; currentFeatureInList->entityIndex != currentRelationInList->relationGovernorIndex; f++)
			{
				currentFeatureInList = currentFeatureInList->next;
			}
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DEBUG
			string relationGovernorForNLPonly = currentRelationInList->relationGovernor;
			cout << "relationGovernorForNLPonly = " << relationGovernorForNLPonly << endl;
			cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
			#endif
			bool foundOfficialLRPreplacementString = false;
			revertNLPtagNameToOfficialLRPtagName(currentFeatureInList, currentSentenceInList, currentRelationInList, false, &foundOfficialLRPreplacementString);
			if(foundOfficialLRPreplacementString)
			{
				string officialLRPentityName = currentFeatureInList->word;
				currentRelationInList->relationGovernor = officialLRPentityName;
				currentRelationInList->relationGovernorRevertedToOfficialLRPTemp = true;
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DEBUG
				cout << "convertGIAsemanticRelationLRPreversion{} foundOfficialLRPreplacementString: relationGovernorForNLPonly = " << relationGovernorForNLPonly << ", currentRelationInList->relationGovernor= " << currentRelationInList->relationGovernor << endl;
				#endif
			}
		}
		if(currentRelationInList->relationDependentIndex < FEATURE_INDEX_MIN_OF_DYNAMICALLY_GENERATED_ENTITY)
		{
			GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;
			for(int f=0; currentFeatureInList->entityIndex != currentRelationInList->relationDependentIndex; f++)
			{
				currentFeatureInList = currentFeatureInList->next;
			}
			#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DEBUG
			string relationDependentForNLPonly = currentRelationInList->relationDependent;
			cout << "relationDependentForNLPonly = " << relationDependentForNLPonly << endl;
			cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
			#endif
			bool foundOfficialLRPreplacementString = false;
			revertNLPtagNameToOfficialLRPtagName(currentFeatureInList, currentSentenceInList, currentRelationInList, false, &foundOfficialLRPreplacementString);
			if(foundOfficialLRPreplacementString)
			{
				string officialLRPentityName = currentFeatureInList->word;
				currentRelationInList->relationDependent = officialLRPentityName;
				currentRelationInList->relationDependentRevertedToOfficialLRPTemp = true;
				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_DEBUG
				cout << "convertGIAsemanticRelationLRPreversion{} foundOfficialLRPreplacementString: relationDependentForNLPonly = " << relationDependentForNLPonly << ", currentRelationInList->relationDependent= " << currentRelationInList->relationDependent << endl;
				#endif
			}
		}
	}
}
#endif
#endif
*/

//assumes prepositions have previously been converted to stanford prep_preposition format during preprocessor (for robustness)
string GIAnlpParserClass::convertPrepositionToRelex2(const string* preposition, bool* prepositionFound)
{
	*prepositionFound = false;
	string relexPreposition = *preposition;
	for(int i=0; i<REFERENCE_TYPE_STANFORD_PARSER_PREPOSITION_PREPEND_NUMBER_OF_TYPES; i++)
	{
		string currentStanfordPrepositionPrepend = referenceTypeStanfordParserPrepositionPrependNameArray[i];
		int foundStanfordPrepositionPrepend = preposition->find(currentStanfordPrepositionPrepend);

		if(foundStanfordPrepositionPrepend != CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			int indexOfFirstRealCharacterInPreposition = currentStanfordPrepositionPrepend.length();
			int lengthOfPreposition = preposition->length() - (indexOfFirstRealCharacterInPreposition);
			relexPreposition = preposition->substr(indexOfFirstRealCharacterInPreposition, lengthOfPreposition);
			*prepositionFound = true;
		}
	}

	return relexPreposition;
}

void GIAnlpParserClass::GIATHparseRelexFeaturesText(const string* featuresText, GIAsentence* currentSentenceInList, int* maxNumberOfWordsInSentence)
{
	GIAfeature* firstFeatureInList = currentSentenceInList->firstFeatureInList;
	*maxNumberOfWordsInSentence = 0;

	int numberOfCharactersInRelationsText = featuresText->length();

	string currentItemString = "";

	/* Data file layout example

		1	joe	joe	noun	uncountable|.n
		2	went	go	verb	past|.v-d
		3	sailing	sail	verb	progressive|.v
		4	on	on	prep
		5	dateID1	12 December 2010.	noun	date
		6	.	.	punctuation

	*/

	GIAfeature* currentFeature = firstFeatureInList;

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

				#ifdef GIA_PREPROCESSOR_MULTIWORD_REDUCTION_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
				if(GIApreprocessorMultiwordReduction.getUseLRP())
				{
					bool foundOfficialLRPreplacementString = false;
					GIArelation* currentRelationInListForPrepositionsOnlyIrrelevant = NULL;
					GIApreprocessorMultiwordReduction.revertNLPtagNameToOfficialLRPtagName(currentFeature, currentSentenceInList, currentRelationInListForPrepositionsOnlyIrrelevant, false, &foundOfficialLRPreplacementString);
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

				GIAfeature* newFeature = new GIAfeature();
				newFeature->previous = currentFeature;
				currentFeature->next = newFeature;
				currentFeature = currentFeature->next;
				*maxNumberOfWordsInSentence = *maxNumberOfWordsInSentence + 1;

				currentFeaturePart = 0;
				currentItemString = "";

				featureIndex++;

				break;
			}
			case CHAR_TAB:
			{
				switch(currentFeaturePart)
				{
					case 0:
					{
						currentFeature->entityIndex = int(SHAREDvars.convertStringToDouble(currentItemString));
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
				currentItemString = "";
				currentFeaturePart++;

				break;
			}
			default:
			{
				currentItemString = currentItemString + c;
				break;
			}
		}

		characterIndex++;
	}

}

void GIAnlpParserClass::GIATHparseRelexRelationsText(const string* relationsText, GIAsentence* currentSentenceInList, int* maxNumberOfWordsInSentence, const bool NLPrelexCompatibilityMode)
{
	GIArelation* firstRelationInList = currentSentenceInList->firstRelationInList;
	int currentSentence = currentSentenceInList->sentenceIndex;

	#ifndef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
	*maxNumberOfWordsInSentence = 0;
	#endif

	int numberOfCharactersInRelationsText = relationsText->length();

	string currentItemString = "";

	/* Data file layout example

		_subj(slip[4], hand[3])
		_poss(hand[3], John[1])
	*/

	GIArelation* currentRelation = firstRelationInList;
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
					this->convertStanfordRelationToRelex(currentRelation, currentSentenceInList);
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

				GIArelation* newRelation = new GIArelation();
				currentRelation->next = newRelation;
				currentRelation = currentRelation->next;

				currentRelationPart = 0;
				currentItemString = "";

				relationIndex++;

				break;
			}
			case CHAR_OPEN_BRACKET:
			{
				string relationType = currentItemString;

				if(!NLPrelexCompatibilityMode)	//condition added 21 Jan 2014
				{
					//added 23 July 2013 - preprocess relex conj_or/conj_and as _conj_or/_conj_and
					if(relationType == RELEX_RELATION_TYPE_CONJUGATION_AND)
					{
						relationType = RELATION_TYPE_CONJUGATION_AND;
					}
					else if(relationType == RELEX_RELATION_TYPE_CONJUGATION_OR)
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
				currentItemString = "";
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
				currentItemString = "";

				break;
			}
			case CHAR_CLOSE_SQUARE_BRACKET:
			{
				if(currentRelationPart == 1)
				{
					currentRelation->relationGovernorIndex = int(SHAREDvars.convertStringToDouble(currentItemString));
				}
				else if(currentRelationPart == 2)
				{
					currentRelation->relationDependentIndex = int(SHAREDvars.convertStringToDouble(currentItemString));
				}

				if(currentRelation->relationGovernorIndex >* maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationGovernorIndex;	//added GIA 2d1a
				}
				if(currentRelation->relationDependentIndex >* maxNumberOfWordsInSentence)
				{
					*maxNumberOfWordsInSentence = currentRelation->relationDependentIndex;
				}

				currentItemString = "";
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
				currentItemString = currentItemString + c;
				break;
			}
		}

		characterIndex++;
	}

}



#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT

string GIAnlpParserClass::generateRelexCFFfeatureTagContent(const GIAfeature* firstFeatureInList)
{
	string relexCFFFeatureTagContent = "";

	const GIAfeature* currentFeatureInList = firstFeatureInList;
	while(currentFeatureInList->next != NULL)
	{
		/*
		if(!(currentFeatureInList->disabled))
		{
		*/
		relexCFFFeatureTagContent = relexCFFFeatureTagContent + "\n\t\t\t\t";

		string entityIndexString = SHAREDvars.convertIntToString(currentFeatureInList->entityIndex);
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
		cout << "GIAsentence Word Index = " << w << endl;
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

string GIAnlpParserClass::generateRelexCFFrelationTagContent(const GIArelation* firstRelationInList)
{
	string relexCFFRelationTagContent = "";

	const GIArelation* currentRelationInList = firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		if(!(currentRelationInList->disabled))
		{
			relexCFFRelationTagContent = relexCFFRelationTagContent + "\n\t\t\t\t";

			string relationType = currentRelationInList->relationType;
			string relationGoverner = currentRelationInList->relationGovernor;
			string relationDependent = currentRelationInList->relationDependent;
			string relationGovernorIndexString = SHAREDvars.convertIntToString(currentRelationInList->relationGovernorIndex);
			string relationDependentIndexString = SHAREDvars.convertIntToString(currentRelationInList->relationDependentIndex);

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



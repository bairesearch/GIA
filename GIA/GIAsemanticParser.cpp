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
 * File Name: GIAsemanticParser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a1s 26-February-2017
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#include "GIAsemanticParser.hpp"

#ifdef USE_GIA2

bool GIAsemanticParserClass::performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputPlainTXTFileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName, const string NLPexeFolderArray[])
{
	bool result = true;

	translatorVariables->parseGIA2file = true;

	#ifdef STANFORD_PARSER_USE_POS_TAGS
	cout << "error: performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences{} doesn't support STANFORD_PARSER_USE_POS_TAGS (because the semantic relations word types being written must match those being read [and read can only use feature parser])" << endl;
	#endif
	bool createNewSentences = true;
	if(!GIAnlp.parseNLPparserFeaturesFile(inputTextNLPfeatureXMLfileName, translatorVariables->isQuery, firstParagraphInList, translatorVariables->NLPfeatureParser, &createNewSentences))
	{
		result = false;
	}

	#ifdef GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		if(!this->lookupSemanticParserFiles(firstParagraphInList, translatorVariables->NLPfeatureParser))	//finds all corresponding GIA2 semantic dependency files and parses them
		{
			#ifdef GIA2_SUPPORT_BOTH_FAST_CORPUS_LOOKUP_PATH_AND_SLOW_SYNTACTIC_RULE_BASED_PATH
			cout << "warning: GIA2 corpus entry not found (will generate corpus entry)" << endl;
			#else
			#ifdef GIA2_PARSE_PARTIALLY_FILLED_SEMANTIC_RELATIONS
			cout << "error: GIA2 corpus entry not found (partial sentence will be parsed)" << endl;
			#else
			cout << "error: GIA2 corpus entry not found (sentences cannot be parsed)" << endl;
			#endif
			#endif
			result = false;
		}
	}
	else
	{
	#endif
		if(!this->lookupSemanticParserFiles(firstParagraphInList->firstSentenceInList, translatorVariables->NLPfeatureParser))	//finds all corresponding GIA2 semantic dependency files and parses them
		{
			#ifdef GIA2_SUPPORT_BOTH_FAST_CORPUS_LOOKUP_PATH_AND_SLOW_SYNTACTIC_RULE_BASED_PATH
			cout << "warning: GIA2 corpus entry not found (will generate corpus entry)" << endl;
			#else
			#ifdef GIA2_PARSE_PARTIALLY_FILLED_SEMANTIC_RELATIONS
			cout << "error: GIA2 corpus entry not found (partial sentence will be parsed)" << endl;
			#else
			cout << "error: GIA2 corpus entry not found (sentences cannot be parsed)" << endl;
			#endif
			#endif
			result = false;
		}
	#ifdef GIA_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
	}
	#endif

	#ifdef GIA2_SUPPORT_BOTH_FAST_CORPUS_LOOKUP_PATH_AND_SLOW_SYNTACTIC_RULE_BASED_PATH
	SHAREDvars.setCurrentDirectory(outputFolder);
	bool onlyParseIfCorpusLookupFailed = true;
	if(!GIAnlp.parseNLPparserRelationsFile(inputTextNLPrelationXMLfileName, translatorVariables->isQuery, firstParagraphInList, translatorVariables->NLPdependencyRelationsParser, translatorVariables->NLPrelexCompatibilityMode, createNewSentences, onlyParseIfCorpusLookupFailed))
	{
		result = false;
	}
	#else
	inputTextNLPrelationXMLfileName = "";				//irrelevant (not used)
	translatorVariables->NLPdependencyRelationsParser = GIA_NLP_PARSER_STANFORD_PARSER;	//irrelevant (not used) - always set to Stanford Parser (as a standard parser type file is used to store GIA2 semantic dependency relations)
	translatorVariables->NLPrelexCompatibilityMode = false; 		//irrelevant (not used) - only used when parsing syntatic dependency relations of a Relex file, and performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences{} does not parse any syntactic dependency relations
	translatorVariables->NLPassumePreCollapsedStanfordRelations = false;	//irrelevant (not used) - only used when executing convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}, and performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences{} does not execute convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}
	#endif

	if(!GIAtranslator.parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(firstParagraphInList, inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, outputCFFfileName, NLPexeFolderArray, translatorVariables))	//inputTextNLPrelationXMLfileName/inputTextNLPfeatureXMLfileName/NLPfeatureParser/NLPdependencyRelationsParser/NLPrelexCompatibilityMode/NLPassumePreCollapsedStanfordRelations not used (relations and features have already been parsed)
	{
		result = false;
	}
}

bool GIAsemanticParserClass::lookupSemanticParserFiles(GIAparagraph* firstParagraphInList, const int NLPfeatureParser)
{
	bool result = true;
	GIAparagraph* currentParagraphInList = firstParagraphInList;
	while(currentParagraphInList->next != NULL)
	{
		GIAsentence* firstSentenceInList = currentParagraphInList->firstSentenceInList;
		if(!this->lookupSemanticParserFiles(firstSentenceInList, NLPfeatureParser))
		{
			result = false;
		}
		currentParagraphInList = currentParagraphInList->next;
	}
	return result;
}

bool GIAsemanticParserClass::lookupSemanticParserFiles(GIAsentence* firstSentenceInList, const int NLPfeatureParser)
{
	bool result = true;
	GIAsentence* currentSentenceInList = firstSentenceInList;
	while(currentSentenceInList->next != NULL)
	{
		GIAsemanticParserOperations.determineGIAconnectionistNetworkPOStypeNames(currentSentenceInList->firstFeatureInList, NLPfeatureParser);
		currentSentenceInList->semanticParserSuccessful = true;

		/*
		GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{
			cout << "1 currentFeatureInList->word = " << currentFeatureInList->word << endl;
			currentFeatureInList = currentFeatureInList->next;
		}
		*/

		#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS	//prioritise text corpus defined semantic relations matching full sentence (or GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS)
		if(!GIAsemanticParserDatabase.loadSemanticParserCorpusDatabaseFile(currentSentenceInList, currentSentenceInList->firstFeatureInList))
		{
		#endif
			#ifdef GIA2_SEMANTIC_PARSER
			//if corpus file not found, then search for apppropriate corpus file subsets...
			#ifdef GIA2_SEMANTIC_PARSER_DEBUG
			cout << "lookupSemanticParserFiles{} GIA2_SEMANTIC_PARSER case not yet tested" << endl;
			#endif
			GIAfeature* dummyBlankFeature = new GIAfeature();
			//now simulate GIA2 semantic relations for each subset of original sentence POS permutation
			GIAfeature* secondWordInTupleFeature = currentSentenceInList->firstFeatureInList;
			if(secondWordInTupleFeature->next != NULL)
			{
				while(secondWordInTupleFeature->next->next != NULL)	//set secondWordInTupleFeature to last in sentence
				{
					secondWordInTupleFeature = secondWordInTupleFeature->next;
				}
			}
			int subsetStillNotFoundMaxFeatureIndex = currentSentenceInList->maxNumberOfWordsInSentence;
			#ifdef GIA2_SEMANTIC_PARSER_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS
			bool notFoundASubsetForAtLeastTwoWords = false;		//note this is only applicable for !GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE and GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE:GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_EXTENSIVE (as with !GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_EXTENSIVE; if a subset file is not found for a set of words, it doesn't mean such a feature set has not been parsed, it just means that no semantic relations exist between them)
			#endif
			bool stillNotFoundASubset = true;

			int minIndexOfSecondWordInTuple = GIA2_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE;
			for(int secondWordInTupleIndex=currentSentenceInList->maxNumberOfWordsInSentence; secondWordInTupleIndex>=minIndexOfSecondWordInTuple; secondWordInTupleIndex--)	//secondWordInTupleIndex in subset [NB secondWordInTupleIndex>=2 as a subset of 1 is not a subset]
			{
				//NB "secondWordInTupleIndex" aka centralWordIndex
				if(stillNotFoundASubset)
				{
					#ifdef GIA2_SEMANTIC_PARSER_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS
					if(!notFoundASubsetForAtLeastTwoWords)	//NB !notFoundASubsetForAtLeastTwoWords condition is optional; it is used because syntactic relations will be used in this case regardless, see "warning: GIA2 corpus entry not found (will generate corpus entry)"
					{
					#endif
						#ifdef GIA2_SEMANTIC_PARSER_DEBUG
						cout << "secondWordInTupleIndex = " << secondWordInTupleIndex << ", " << secondWordInTupleFeature->lemma << endl;
						#endif
						GIAfeature* recordOfFeatureAfterSecondWordInTupleFeature = secondWordInTupleFeature->next;
						secondWordInTupleFeature->next = dummyBlankFeature;	//temporarily disconnect node at end of sentence subset
						#ifdef GIA2_SEMANTIC_PARSER_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS
						bool foundACorpusSubsetForSecondWordInTuple = false;
						#endif

						if(currentSentenceInList->firstFeatureInList->entityIndex != GIA_NLP_START_ENTITY_INDEX)
						{
							cout << "lookupSemanticParserFiles{} implementation error*: (sentence->firstFeatureInList->entityIndex != GIA_NLP_START_ENTITY_INDEX)" << endl;
							exit(EXIT_ERROR);
						}
						#ifdef GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS
						if(!SHAREDvars.textInTextArray(secondWordInTupleFeature->word, semanticParserOptimiseBasedOnConjunctionsIllegalSecondWordInTupleArray, GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS_ILLEGAL_CENTRAL_WORD_NUMBER_OF_TYPES))
						{
							bool optimisedBasedOnContextualConjunctions = false;
							GIAfeature* firstFeatureInSentenceSubsetInitial = GIAsemanticParserOperations.generateOptimisedFeatureSubsetBasedOnContextualConjunctions(currentSentenceInList->firstFeatureInList, secondWordInTupleIndex, &optimisedBasedOnContextualConjunctions);
						#else
							GIAfeature* firstFeatureInSentenceSubsetInitial = currentSentenceInList->firstFeatureInList;
						#endif
							int maxIndexOfFirstWordInTuple = (secondWordInTupleFeature->entityIndex - (GIA2_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE-1));
							#ifdef GIA2_SEMANTIC_PARSER_DEBUG
							cout << "\tmaxIndexOfFirstWordInTuple = " << maxIndexOfFirstWordInTuple << endl;
							#endif
							#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
							for(int firstWordInTupleIndex = GIA_NLP_START_ENTITY_INDEX; firstWordInTupleIndex <= maxIndexOfFirstWordInTuple; firstWordInTupleIndex++)
							{
								int GIA2semanticDependencyRelationProbabilityTotalArray[GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
								GIAsemanticParserDatabase.initialiseIntArray(GIA2semanticDependencyRelationProbabilityTotalArray, GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);
							#endif
								GIAfeature* firstFeatureInSentenceSubset = firstFeatureInSentenceSubsetInitial;
								while(firstFeatureInSentenceSubset->entityIndex <= maxIndexOfFirstWordInTuple)		//changed 2k1a for GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS compatibility: verify that feature entityIndices are not mutated by GIA referencing*	//OLD:	for(int firstWordInSentenceSubsetIndex=1; firstWordInSentenceSubsetIndex<=secondWordInTupleIndex-(GIA2_CONNECTIONIST_NETWORK_MIN_SUBSET_SIZE-1); firstWordInSentenceSubsetIndex++)
								{
									int firstWordInSentenceSubsetIndex=firstFeatureInSentenceSubset->entityIndex;
									#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
									if(firstWordInTupleIndex >= firstWordInSentenceSubsetIndex)
									{
									#endif
										#ifdef GIA2_SEMANTIC_PARSER_DEBUG
										cout << "\t\tfirstWordInTupleIndex = " << firstWordInTupleIndex << endl;
										cout << "\t\t\tfirstWordInSentenceSubsetIndex = " << firstWordInSentenceSubsetIndex << ", " << firstFeatureInSentenceSubset->lemma << endl;
										#endif
										#ifdef GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
										if(!foundACorpusSubsetForSecondWordInTuple)
										{
											int subsetSize = secondWordInTupleIndex-firstWordInSentenceSubsetIndex+1;	//subsetSize aka maxSpread

											//code from convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}:

											if(GIAsemanticParserDatabase.loadSemanticParserCorpusDatabaseFile(currentSentenceInList, firstFeatureInSentenceSubset))
											{
												#ifdef GIA2_SEMANTIC_PARSER_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS
												foundACorpusSubsetForSecondWordInTuple = true;
												#endif
												if(firstWordInSentenceSubsetIndex == 1)
												{
													stillNotFoundASubset = false;	//have found a full matching sentence corpus file (with features matching from the firstFeatureInSentence right up to the secondFeatureInTheTuple); therefore no more corpus files need be parsed
												}
											}
										}
										#endif

										#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
										int firstWordInTupleIndexRelative = GIAsemanticParserOperations.calculateFirstWordInTupleIndexRelative(firstWordInTupleIndex, firstWordInSentenceSubsetIndex);
										int GIA2semanticDependencyRelationProbabilityArray[GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
										int GIA2semanticDependencyRelationAssignedArray[GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
										int GIA2semanticDependencyRelationRejectedArray[GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
										if(GIAsemanticParserDatabase.loadSemanticParserOptimisedDatabaseFile(firstFeatureInSentenceSubset, firstWordInTupleIndexRelative, GIA2semanticDependencyRelationProbabilityArray, GIA2semanticDependencyRelationAssignedArray, GIA2semanticDependencyRelationRejectedArray))
										{
											#ifdef GIA2_SEMANTIC_PARSER_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS
											foundACorpusSubsetForSecondWordInTuple = true;
											#endif
											for(int i=0; i<GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES; i++)
											{
												GIA2semanticDependencyRelationProbabilityTotalArray[i] = GIA2semanticDependencyRelationProbabilityTotalArray[i] + GIA2semanticDependencyRelationProbabilityArray[i];
											}
										}
										#endif

									#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
									}
									#endif
									firstFeatureInSentenceSubset = firstFeatureInSentenceSubset->next;
								}
								#ifdef GIA2_SEMANTIC_PARSER_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS
								if(!foundACorpusSubsetForSecondWordInTuple)
								{
									cout << "************************ warning: notFoundASubsetForAtLeastTwoWords* *****************" << endl;
									notFoundASubsetForAtLeastTwoWords = true;
								}
								#endif
							#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
								int maxProbabilityOfSemanticRelationTypesInTuple = 0;
								int semanticRelationTypeOptimisedDatabase = INT_DEFAULT_VALUE;
								for(int i=0; i<GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES; i++)
								{
									if(GIA2semanticDependencyRelationProbabilityTotalArray[i] > maxProbabilityOfSemanticRelationTypesInTuple)
									{
										maxProbabilityOfSemanticRelationTypesInTuple = GIA2semanticDependencyRelationProbabilityTotalArray[i];
										semanticRelationTypeOptimisedDatabase = i;
									}
								}
								if(semanticRelationTypeOptimisedDatabase != INT_DEFAULT_VALUE)
								{
									int firstWordInTupleIndexRelative = GIAsemanticParserOperations.calculateFirstWordInTupleIndexRelative(firstWordInTupleIndex, firstFeatureInSentenceSubsetInitial->entityIndex);

									#ifdef GIA2_SEMANTIC_PARSER_DEBUG
									cout << "maxProbabilityOfSemanticRelationTypesInTuple = " << maxProbabilityOfSemanticRelationTypesInTuple << endl;
									cout << "semanticRelationTypeOptimisedDatabase = " << semanticRelationTypeOptimisedDatabase << endl;
									cout << "firstWordInTupleIndexRelative = " << firstWordInTupleIndexRelative << endl;
									cout << "secondWordInTupleIndex = " << secondWordInTupleIndex << endl;
									#endif

									GIAsemanticParserDatabase.addTupleSemanticRelationToSentence(currentSentenceInList, semanticRelationTypeOptimisedDatabase, firstWordInTupleIndexRelative, secondWordInTupleIndex);
								}
							}
							#endif
						#ifdef GIA2_SEMANTIC_PARSER_OPTIMISE_BASED_ON_CONJUNCTIONS
							if(optimisedBasedOnContextualConjunctions)
							{
								delete firstFeatureInSentenceSubsetInitial;	//delete artificially generated optimised sentence subset
							}
						}
						#endif
						secondWordInTupleFeature->next = recordOfFeatureAfterSecondWordInTupleFeature;	//restore temporarily disconnected node at end of sentence subset
					#ifdef GIA2_SEMANTIC_PARSER_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS
					}
					#endif
				}
				secondWordInTupleFeature = secondWordInTupleFeature->previous;
			}
			#ifdef GIA2_SEMANTIC_PARSER_EXPECT_TO_FIND_DATABASE_FILES_FOR_ALL_FEATURE_PERMUTATIONS
			if(notFoundASubsetForAtLeastTwoWords)
			{
				result = false;
				#ifndef GIA2_PARSE_PARTIALLY_FILLED_SEMANTIC_RELATIONS
				currentSentenceInList->semanticParserSuccessful = false;
				#endif
			}
			#endif
			#else
			result = false;
			currentSentenceInList->semanticParserSuccessful = false;
			#endif
		#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
		}
		#endif

		/*
		currentFeatureInList = currentSentenceInList->firstFeatureInList;
		while(currentFeatureInList->next != NULL)
		{
			cout << "2 currentFeatureInList->word = " << currentFeatureInList->word << endl;
			currentFeatureInList = currentFeatureInList->next;
		}
		*/

		currentSentenceInList = currentSentenceInList->next;
	}
	return result;
}

#endif




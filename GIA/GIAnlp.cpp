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
 * File Name: GIAnlp.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2e4g 19-April-2014
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/





#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


#include "GIAnlp.h"
#include "XMLparserClass.h"
#include "XMLrulesClass.h"
#include "GIAnlpParser.h"
#include "GIAtranslatorOperations.h" //required for featureNERtypeArray only
#ifdef GIA_USE_LRP
#include "GIAlrp.h"
#endif
#include "SHAREDvars.h"	//file io

#ifndef LINUX
	#include <windows.h>
#endif


void executeNLPparser(string inputTextPlainTXTfileName, string inputTextNLPXMLfileName, int NLPParser, string NLPexeFolderArray[], bool parseRelationsOrFeatures, bool NLPrelexCompatibilityMode)
{
	/*
	NB execute NLP on current folder not saved working folder (this is required for when a preprocessor eg LRP/CE has been executed on the input text):
	so must assume current folder has been set to the folder where the [pre-processed] plain text file exists (if not the actual working folder)
	*/
	char currentFolderCharStar[EXE_FOLDER_PATH_MAX_LENGTH];
	getCurrentDirectory(currentFolderCharStar);

	/*
	int inputTextNLPParsedXMLFileNameLength = inputTextNLPXMLfileName.length();
	int inputTextNLPParsedXMLFileNameIndexOfExtension = inputTextNLPXMLfileName.rfind(".");		//find last occurance of "."
	int inputTextNLPParsedXMLFileNameExtensionLength = inputTextNLPParsedXMLFileNameLength-inputTextNLPParsedXMLFileNameIndexOfExtension;
	string inputTextNLPParsedXMLExtensionName = inputTextNLPXMLfileName.substr(inputTextNLPParsedXMLFileNameIndexOfExtension, inputTextNLPParsedXMLFileNameExtensionLength);
	string inputTextNLPParsedXMLFileNameWithout
	inputTextNLPXMLfileName.substr(inputTextNLPParsedXMLFileNameIndexOfExtension, inputTextNLPParsedXMLFileNameLength-inputTextNLPParsedXMLFileNameExtensionLength);
	*/
	string inputTextNLPParsedXMLFileNameTemp = inputTextPlainTXTfileName + StanfordCoreNLPdefaultOutputFileExtensionAppend;

	bool result = true;
	string NLPParserExecutableName = "";
	string NLPexeFolder = "";
	#ifdef GIA_USE_RELEX
	if(NLPParser == GIA_NLP_PARSER_RELEX)
	{
		if(NLPrelexCompatibilityMode)
		{
			NLPParserExecutableName = GIA_RELEX_WITH_STANFORD_COMPATIBILITY_MODE_EXECUTABLE_NAME;
		}
		else
		{
			NLPParserExecutableName = GIA_RELEX_EXECUTABLE_NAME;
		}
		NLPexeFolder = NLPexeFolderArray[GIA_NLP_PARSER_RELEX];
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		#ifdef STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS
		if(parseRelationsOrFeatures)
		{
			NLPParserExecutableName = GIA_STANFORD_NLP_EXECUTABLE_NAME_WITHOUT_INDEPENDENT_POS_TAGGER;
		}
		else
		{
		#endif
			NLPParserExecutableName = GIA_STANFORD_NLP_EXECUTABLE_NAME;
		#ifdef STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS
		}
		#endif
		NLPexeFolder = NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_CORENLP];
	}
	#endif
	#ifdef GIA_USE_STANFORD_PARSER
	if(NLPParser == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		NLPParserExecutableName = GIA_STANFORD_PARSER_EXECUTABLE_NAME;
		NLPexeFolder = NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_PARSER];
	}
	#endif

	//execute NLP parser on plain text
	string executeNLPCommand = "";
	executeNLPCommand = executeNLPCommand + NLPexeFolder + "/" + NLPParserExecutableName + " " + inputTextPlainTXTfileName + " " + inputTextNLPXMLfileName + " " + currentFolderCharStar + " " + tempFolderCharStar + " " + StanfordCoreNLPdefaultOutputFileExtensionAppend;

	setCurrentDirectory(NLPexeFolder.c_str());

	#ifdef GIA_NLP_DEBUG
	cout << "system(" << executeNLPCommand << ");" << endl;
	#endif
	system(executeNLPCommand.c_str());

	setCurrentDirectory(tempFolderCharStar);

	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		#ifdef LINUX
			#define SYSTEM_MOVE_COMMAND "mv"
		#else
			#define SYSTEM_MOVE_COMMAND "move"
		#endif

		string commandCopyTemporaryFileToRealFile = "";
		commandCopyTemporaryFileToRealFile = commandCopyTemporaryFileToRealFile + SYSTEM_MOVE_COMMAND + " " + inputTextNLPParsedXMLFileNameTemp + " " + inputTextNLPXMLfileName;	//this is required because Stanford CoreNLP cannot output a file of a given name, it can only output a file with a modified extension
		system(commandCopyTemporaryFileToRealFile.c_str());

		#ifdef LINUX
		//this is required due to a bug in StanfordNLPcore in Linux, where it produces a Dos file instead of a Unix file (new lines identified by carrage return..)
		string commandDos2Unix = "";
		commandDos2Unix = commandDos2Unix + "dos2unix " + inputTextNLPXMLfileName;
		system(commandDos2Unix.c_str());
		#endif

	}
	#endif

	setCurrentDirectory(currentFolderCharStar);
}


bool parseNLPparserFile(string inputTextNLPrelationXMLfileName, string inputTextNLPfeatureXMLfileName, bool isQuery, Paragraph * firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode)
{
	bool result = true;

	bool createNewSentences = true;

	#ifdef GIA_NLP_DEBUG
	cout << "inputTextNLPrelationXMLfileName = " << inputTextNLPrelationXMLfileName << endl;
	cout << "inputTextNLPfeatureXMLfileName = " << inputTextNLPfeatureXMLfileName << endl;
	cout << "NLPfeatureParser = " << NLPfeatureParser << endl;
	cout << "NLPdependencyRelationsParser = " << NLPdependencyRelationsParser << endl;
	cout << "NLPrelexCompatibilityMode = " << NLPrelexCompatibilityMode << endl;
	#endif

	//Parse Features
	if(!parseNLPparserFeaturesFile(inputTextNLPfeatureXMLfileName, isQuery, firstParagraphInList, NLPfeatureParser, &createNewSentences))
	{
		result = false;
	}

	#ifdef GIA_NLP_DEBUG
	cout << "finished parsing features. Parse dependency relations:" << endl;
	#endif

	//Parse Relations
	if(result)
	{
		if(!parseNLPparserRelationsFile(inputTextNLPrelationXMLfileName, isQuery, firstParagraphInList, NLPdependencyRelationsParser, NLPrelexCompatibilityMode, createNewSentences, false))
		{
			result = false;
		}
	}

	return result;
}

bool parseNLPparserFeaturesFile(string inputTextNLPfeatureXMLfileName, bool isQuery, Paragraph * firstParagraphInList, int NLPfeatureParser, bool * createNewSentences)
{
	bool result = true;

	#ifdef GIA_USE_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		#ifdef GIA_NLP_DEBUG
		cout << "parseRelexFile" << endl;
		#endif
		#ifdef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
		bool NLPrelexCompatibilityMode = true;		//GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2 - use stanford compatibilty mode only when parsing features, and only add special relations; RELATION_TYPE_MODAL_AUX, RELATION_TYPE_PASSIVE_AUX, RELATION_TYPE_COPULA, RELATION_TYPE_DETERMINER
		#else
		bool NLPrelexCompatibilityMode = false;		//irrelevant (not used) - only used when parsing syntatic dependency relations of a Relex file
		#endif
		if(!parseRelexFile(inputTextNLPfeatureXMLfileName, isQuery, firstParagraphInList, false, true, NLPrelexCompatibilityMode, *createNewSentences, false))
		{
			result = false;
		}
		*createNewSentences = false;
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		#ifdef GIA_NLP_DEBUG
		cout << "parseStanfordCoreNLPfile" << endl;
		#endif
		if(!parseStanfordCoreNLPfile(inputTextNLPfeatureXMLfileName, isQuery, firstParagraphInList, false, true, *createNewSentences, false))
		{
			result = false;
		}
		*createNewSentences = false;
	}
	#endif
	#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
	#ifdef GIA_USE_STANFORD_PARSER
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		//stanford parser does not create lemmas and so can't be used for full GIA feature extraction
		#ifdef GIA_NLP_DEBUG
		cout << "parseStanfordParserFile" << endl;
		#endif
		if(isQuery)
		{
			cout << "error: parseNLPparserFile() does not support queries at present with (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER). Set feature parser to RelEx or Stanford Core NLP for queries" << endl;
			exit(0);
		}
		else
		{
			cout << "warning: parseNLPparserFile() does not parse features when (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER). Feature extraction is not supported with GIA_NLP_PARSER_STANFORD_PARSER. Set feature parser to RelEx or Stanford Core NLP to extract features" << endl;
		}
	}
	#endif
	#endif

	return result;
}

bool parseNLPparserRelationsFile(string inputTextNLPrelationXMLfileName, bool isQuery, Paragraph * firstParagraphInList, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode, bool createNewSentences, bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsParser == GIA_NLP_PARSER_RELEX)
	{
		#ifdef GIA_NLP_DEBUG
		cout << "parseRelexFile" << endl;
		#endif
		if(!parseRelexFile(inputTextNLPrelationXMLfileName, isQuery, firstParagraphInList, true, false, NLPrelexCompatibilityMode, createNewSentences, onlyParseIfCorpusLookupFailed))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPdependencyRelationsParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		#ifdef GIA_NLP_DEBUG
		cout << "parseStanfordCoreNLPfile" << endl;
		#endif
		if(!parseStanfordCoreNLPfile(inputTextNLPrelationXMLfileName, isQuery, firstParagraphInList, true, false, createNewSentences, onlyParseIfCorpusLookupFailed))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_USE_STANFORD_PARSER
	if(NLPdependencyRelationsParser == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		#ifdef GIA_NLP_DEBUG
		cout << "parseStanfordParserFile" << endl;
		#endif
		if(!parseStanfordParserFile(inputTextNLPrelationXMLfileName, isQuery, firstParagraphInList, createNewSentences, onlyParseIfCorpusLookupFailed))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_NLP_DEBUG
	cout << "finished parsing dependency relations." << endl;
	#endif

	return result;
}

#ifdef GIA_USE_RELEX
bool parseRelexFile(string inputTextNLPrelationXMLfileName, bool isQuery, Paragraph * firstParagraphInList, bool parseRelations, bool parseFeatures, bool NLPrelexCompatibilityMode, bool createNewSentences, bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	Paragraph * currentParagraph = firstParagraphInList;
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;
	Sentence * currentSentence = firstSentenceInList;

	XMLparserTag * firstTagInXMLFile = new XMLparserTag();
	if(!readXMLfile(inputTextNLPrelationXMLfileName, firstTagInXMLFile))
	{
		result = false;
	}

	if(result)
	{
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		XMLparserTag * currentTag2 = firstTagInXMLFile;
		currentTag2 = parseTagDownALevel(currentTag2, Relex_CFF_XML_TAG_nlparse, &result);
		if(result)
		{
			//now for every sentence;
			while(currentTag2->nextTag != NULL)
			{
				if(currentTag2->name == Relex_CFF_XML_TAG_paragraph)
				{
					XMLparserTag * currentTag = currentTag2;
					currentTag = parseTagDownALevel(currentTag, Relex_CFF_XML_TAG_paragraph, &result);
		#else
					XMLparserTag * currentTag = firstTagInXMLFile;
					currentTag = parseTagDownALevel(currentTag, Relex_CFF_XML_TAG_nlparse, &result);
		#endif

					if(result)
					{
						//now for every sentence;
						while(currentTag->nextTag != NULL)
						{
							if(currentTag->name == Relex_CFF_XML_TAG_sentence)
							{
								if(!onlyParseIfCorpusLookupFailed || !(currentSentence->corpusLookupSuccessful))
								{
									//locate and record sentence index
									XMLParserAttribute * firstAttributeInSentenceTag = currentTag->firstAttribute;
									XMLParserAttribute * currentAttributeInSentenceTag = firstAttributeInSentenceTag;
									bool foundSentenceIndexAttribute = false;
									while(currentAttributeInSentenceTag->nextAttribute != NULL)
									{
										if(currentAttributeInSentenceTag->name == Relex_CFF_XML_ATTRIBUTE_index)
										{
											int sentenceIndex = atoi(currentAttributeInSentenceTag->value.c_str());
											currentSentence->sentenceIndex = sentenceIndex;
											foundSentenceIndexAttribute = true;
										}
										currentAttributeInSentenceTag = currentAttributeInSentenceTag->nextAttribute;
									}
									if(!foundSentenceIndexAttribute)
									{
										cout << "error: sentence index attribute expected" << endl;
										exit(0);
									}

									XMLparserTag * firstTagInSentence;
									firstTagInSentence = parseTagDownALevel(currentTag, Relex_CFF_XML_TAG_sentence, &result);
									XMLparserTag * firstTagInFirstParse;
									firstTagInFirstParse = parseTagDownALevel(firstTagInSentence, Relex_CFF_XML_TAG_parse, &result);

									if(result)
									{
										XMLparserTag * currentTagInParse = firstTagInFirstParse;
										while(currentTagInParse->nextTag != NULL)
										{

											if(currentTagInParse->name == Relex_CFF_XML_TAG_features)
											{
												if(parseFeatures)
												{
													#ifdef GIA_NLP_DEBUG
													//cout << "GIATHparseRelexFeaturesText: currentTagInParse->value = " << currentTagInParse->value << endl;
													#endif
													int maxNumberOfWordsInSentence = 0;
													GIATHparseRelexFeaturesText(&(currentTagInParse->value), currentSentence, &maxNumberOfWordsInSentence);
													#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE
													currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;
													#endif
													if(isQuery)
													{
														#ifdef GIA_QUERIES_MUST_BE_QUESTIONS
														if(!(currentSentence->isQuestion))
														{
															cout << "error: GIA query is not a question" << endl;
															exit(0);
														}
														#endif
													}
												}
											}
											else if(currentTagInParse->name == Relex_CFF_XML_TAG_relations)
											{
												if(parseRelations)
												{
													#ifdef GIA_NLP_DEBUG
													//cout << "GIATHparseRelexRelationsText: currentTagInParse->value = " << currentTagInParse->value << endl;
													#endif
													#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
													int maxNumberOfWordsInSentence = currentSentence->maxNumberOfWordsInSentence;
													#else
													int maxNumberOfWordsInSentence = 0;
													#endif
													GIATHparseRelexRelationsText(&(currentTagInParse->value), currentSentence, &maxNumberOfWordsInSentence, NLPrelexCompatibilityMode);
													#ifdef GIA_RECORD_MAX_FEATURE_INDEX
													currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;
													#endif
												}
												#ifdef GIA2_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
												if(!parseRelations && NLPrelexCompatibilityMode)
												{
													int maxNumberOfWordsInSentence = 0;
													bool featuresNotPreviouslyFilled = false;	//NB featuresNotPreviouslyFilled could be set to true as secondary relations are only used to create GIA2 semantic relations, and as such lemma information is disgarded [only feature indicies are important]
													string relationsText = currentTagInParse->value;
													if(relationsText[0] == '\n')
													{
														//cout << "(relationsText[0] == \n)" << endl;
														relationsText = relationsText.substr(1, (relationsText.length() -1));
													}

													GIATHparseStanfordParserRelationsText(&relationsText, currentSentence, &maxNumberOfWordsInSentence, featuresNotPreviouslyFilled, false, NLPrelexCompatibilityMode);
													#ifdef GIA_NLP_DEBUG
													//cout << "finished GIATHparseStanfordParserRelationsText" << endl;
													#endif
												}
												#endif
											}

											currentTagInParse = currentTagInParse->nextTag;
										}

									}

									if(createNewSentences)
									{
										Sentence * newSentence = new Sentence();
										newSentence->previous = currentSentence;
										currentSentence->next = newSentence;
									}
								}

								currentSentence = currentSentence->next;

							}
							currentTag = currentTag->nextTag;
						}

					}
		#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
					Paragraph * newParagraph = new Paragraph();
					newParagraph->previous = currentParagraph;
					currentParagraph->next = newParagraph;
					firstSentenceInList = newParagraph->firstSentenceInList;
					currentSentence = firstSentenceInList;
					currentParagraph = currentParagraph->next;
				}
				currentTag2 = currentTag2->nextTag;
			}
		}
		#endif
	}

	#ifndef GIA_FREE_MEMORY1
	delete firstTagInXMLFile;
	#endif

	return result;
}
#endif


#ifdef GIA_USE_STANFORD_CORENLP
bool parseStanfordCoreNLPfile(string inputTextNLPrelationXMLfileName, bool isQuery, Paragraph * firstParagraphInList, bool parseRelations, bool parseFeatures, bool createNewSentences, bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	#ifdef GIA_NLP_DEBUG
	//cout << "inputTextNLPrelationXMLfileName = " << inputTextNLPrelationXMLfileName << endl;
	#endif

	Paragraph * currentParagraph = firstParagraphInList;
	Paragraph * newParagraph = new Paragraph();
	newParagraph->previous = currentParagraph;
	currentParagraph->next = newParagraph;

	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;
	Sentence * currentSentence = firstSentenceInList;

	XMLparserTag * firstTagInXMLFile = new XMLparserTag();
	if(!readXMLfile(inputTextNLPrelationXMLfileName, firstTagInXMLFile))
	{
		result = false;
	}

	if(result)
	{

		XMLparserTag * firstTagInRoot = parseTagDownALevel(firstTagInXMLFile, StanfordCoreNLP_XML_TAG_root, &result);
		XMLparserTag * firstTagInDocument = parseTagDownALevel(firstTagInRoot, StanfordCoreNLP_XML_TAG_document, &result);
		XMLparserTag * currentTagInDocument = firstTagInDocument;

		XMLparserTag * firstTagInSentences = parseTagDownALevel(currentTagInDocument, StanfordCoreNLP_XML_TAG_sentences, &result);
		if(result && (firstTagInSentences != NULL))
		{
			XMLparserTag * currentTagInSentences = firstTagInSentences;
			while(currentTagInSentences->nextTag != NULL)
			{
				#ifdef GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
				bool invalidSentenceFoundIsolatedFullStop = false;
				#endif

				if(!onlyParseIfCorpusLookupFailed || !(currentSentence->corpusLookupSuccessful))
				{
					string sentenceIndexString = currentTagInSentences->firstAttribute->value;
					currentSentence->sentenceIndex = atoi(sentenceIndexString.c_str());
					#ifdef GIA_NLP_DEBUG
					//cout << "currentSentence->sentenceIndex = " << currentSentence->sentenceIndex << endl;
					#endif

					XMLparserTag * firstTagInSentence = parseTagDownALevel(currentTagInSentences, StanfordCoreNLP_XML_TAG_sentence, &result);

					XMLparserTag * currentTagInSentence = firstTagInSentence;

					Feature * firstFeatureInList = currentSentence->firstFeatureInList;
					Feature * currentFeatureInList = firstFeatureInList;

					XMLparserTag * firstTagInTokens = parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_XML_TAG_tokens, &result);
					XMLparserTag * currentTagInTokens = firstTagInTokens;
					bool isQuestion = false;

					#ifdef GIA_NLP_DEBUG
					//cout << "parseFeatures: " << endl;
					#endif
					int maxNumberOfWordsInSentence = 0;

					while(currentTagInTokens->nextTag != NULL)
					{
						#ifdef GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
						int lastFeatureEntityIndex = -1;
						string lastFeatureWord = "";
						#endif

						if(parseFeatures)
						{
							string entityIndexString = currentTagInTokens->firstAttribute->value;
							currentFeatureInList->entityIndex = atoi(entityIndexString.c_str());
							#ifdef GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
							lastFeatureEntityIndex = currentFeatureInList->entityIndex;
							#endif
						}
						#ifdef GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
						else
						{
							string entityIndexString = currentTagInTokens->firstAttribute->value;
							lastFeatureEntityIndex = atoi(entityIndexString.c_str());
						}
						#endif

						XMLparserTag * firstTagInToken = parseTagDownALevel(currentTagInTokens, StanfordCoreNLP_XML_TAG_token, &result);
						XMLparserTag * currentTagInToken = firstTagInToken;
						while(currentTagInToken->nextTag != NULL)
						{
							if(parseFeatures)
							{
								if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_word)
								{
									string TagValue = currentTagInToken->value;
									currentFeatureInList->word = TagValue;
									#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
									#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
									if(currentFeatureInList->word == FEATURE_WORD_QUESTIONMARK)
									{
										isQuestion = true;
									}
									#endif
									#endif
									#ifdef GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
									lastFeatureWord = TagValue;
									#endif
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_lemma)
								{
									string TagValue = currentTagInToken->value;
									currentFeatureInList->lemma = TagValue;
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_CharacterOffsetBegin)
								{
									int TagValue = atoi(currentTagInToken->value.c_str());
									currentFeatureInList->CharacterOffsetBegin = TagValue;
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_CharacterOffsetEnd)
								{
									int TagValue = atoi(currentTagInToken->value.c_str());
									currentFeatureInList->CharacterOffsetEnd = TagValue;
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_POS)
								{
									string TagValue = currentTagInToken->value;
									currentFeatureInList->stanfordPOS = TagValue;
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_NER)
								{
									string TagValue = currentTagInToken->value;
									int currentNER = FEATURE_NER_UNDEFINED;
									for(int i=0; i<FEATURE_NER_NUMBER_OF_TYPES; i++)
									{
										if(TagValue == featureNERtypeArray[i])
										{
											currentNER = i;
											#ifdef GIA_WORDNET_DEBUG
											//cout << "nerType currentFeatureIndex = " << currentNER << endl;
											#endif
										}
									}
									currentFeatureInList->NER = currentNER;
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_NormalizedNER)
								{
									string TagValue = currentTagInToken->value;
									currentFeatureInList->NormalizedNER = TagValue;
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_Timex)
								{

									string TagValue = currentTagInToken->value;
									currentFeatureInList->Timex = TagValue;
								}
							}
							else
							{//only interested in POS tags if not parsing features (as POS tags in GIA must always match dependency relation POS tags)
								if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_POS)	//overwrite pos tag values if necessary...
								{
									#ifdef GIA_NLP_DEBUG
									//cout << "StanfordCoreNLP_XML_TAG_POS = " << currentTagInToken->value << endl;
									#endif
									string TagValue = currentTagInToken->value;
									currentFeatureInList->stanfordPOS = TagValue;
								}
								#ifdef GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_word)
								{
									string TagValue = currentTagInToken->value;
									lastFeatureWord = TagValue;
								}
								#endif
							}
							currentTagInToken = currentTagInToken->nextTag;
						}

						if(parseFeatures)
						{//process lemma only if parsing features
							#ifdef GIA_USE_LRP
							if(getUseLRP())
							{
								bool foundOfficialLRPreplacementString = false;
								Relation * currentRelationInListForPrepositionsOnlyIrrelevant = NULL;
								revertNLPtagNameToOfficialLRPtagName(currentFeatureInList, currentSentence, currentRelationInListForPrepositionsOnlyIrrelevant, false, &foundOfficialLRPreplacementString);
							}
							#endif

							#ifdef GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_USE_ORIGINAL_WORD
							//eg Stanford CoreNLP converts GIA->gium when it is tagged as NN (instead of NNP)
							for(int i=0; i<FEATURE_NER_INDICATES_PROPER_NOUN_NUMBER_OF_TYPES; i++)
							{
								if(currentFeatureInList->NER == featureNERindicatesProperNounTypeArray[i])
								{
									currentFeatureInList->lemma = currentFeatureInList->word;
								}
							}
							#endif
							#ifdef GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_MAKE_ALL_LEMMAS_LOWER_CASE
							for(int i=0; i<currentFeatureInList->lemma.length(); i++)
							{
								currentFeatureInList->lemma[i] = tolower(currentFeatureInList->lemma[i]);
							}
							#endif
						}
						#ifdef GIA_NLP_DEBUG
						//cout << "currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
						//cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
						#endif

						#ifdef GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
						if(lastFeatureEntityIndex == GIA_NLP_START_ENTITY_INDEX)
						{
							if(lastFeatureWord == string(STRING_FULLSTOP))
							{
								invalidSentenceFoundIsolatedFullStop = true;
							}
						}
						#endif

						if(parseFeatures)		//OR if(createNewSentences)
						{//all sentences created must have feature list, as feature NLP file is always parsed first
							Feature * newFeature = new Feature();
							newFeature->previous = currentFeatureInList;
							currentFeatureInList->next = newFeature;
							currentFeatureInList = currentFeatureInList->next;
						}
						else
						{
							currentFeatureInList = currentFeatureInList->next;
						}
						maxNumberOfWordsInSentence = maxNumberOfWordsInSentence + 1;
						currentTagInTokens = currentTagInTokens->nextTag;
					}

					if(parseFeatures)
					{//calculate question only if parsing features
						#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE
						currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;	//added GIA 2d1a
						#endif

						#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
						#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
						if(isQuery)
						{
							if(isQuestion)
							{
								#ifdef GIA_NLP_DEBUG
								//cout << "Stanford CoreNLP found question" << endl;
								#endif
								currentSentence->isQuestion = true;
							}
							else
							{
								#ifdef GIA_QUERIES_MUST_BE_QUESTIONS
								cout << "error: GIA query is not a question" << endl;
								exit(0);
								#endif
							}
						}
						#endif
						#endif
					}

					currentTagInSentence = currentTagInSentence->nextTag;
					XMLparserTag * firstTagInParse = parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_XML_TAG_parse, &result);

					if(parseRelations)
					{
						#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
						maxNumberOfWordsInSentence = currentSentence->maxNumberOfWordsInSentence;
						#else
						int maxNumberOfWordsInSentence = 0;
						#endif
						#ifdef GIA_NLP_DEBUG
						//cout << "parseRelations: " << endl;
						#endif
						for(int dependenciesSetIndex=0; dependenciesSetIndex< StanfordCoreNLP_numberOfDependencySetsPerSentence; dependenciesSetIndex++)
						{
							Relation * firstRelationInList = currentSentence->firstRelationInList;
							Relation * currentRelationInList = firstRelationInList;

							currentTagInSentence = currentTagInSentence->nextTag;
							if(dependenciesSetIndex == StanfordCoreNLP_dependencySet_usedByGIA)
							{
								XMLparserTag * firstTagInDependencies = parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_relationSetNameArray[dependenciesSetIndex], &result);
								if(firstTagInDependencies != NULL)
								{
									XMLparserTag * currentTagInDependencies = firstTagInDependencies;
									while(currentTagInDependencies->nextTag != NULL)
									{
										XMLparserTag * firstTagInDep = parseTagDownALevel(currentTagInDependencies, StanfordCoreNLP_XML_TAG_dep, &result);
										XMLparserTag * governerTagInDep = firstTagInDep;
										XMLparserTag * dependentTagInDep = firstTagInDep->nextTag;

										string relationGovernorIndexString = governerTagInDep->firstAttribute->value;
										string relationDependentIndexString = dependentTagInDep->firstAttribute->value;
										currentRelationInList->relationGovernorIndex = atoi(relationGovernorIndexString.c_str());
										currentRelationInList->relationDependentIndex = atoi(relationDependentIndexString.c_str());

										currentRelationInList->relationType = currentTagInDependencies->firstAttribute->value;

										//don't use these, use lemmas instead (as per Stanford Core NLP/Relex dependency relation definitions). These are required to be set temporarily for LRP;
										currentRelationInList->relationGovernor = governerTagInDep->value;
										currentRelationInList->relationDependent = dependentTagInDep->value;

										convertStanfordRelationToRelex(currentRelationInList, currentSentence);

										#ifdef GIA_NLP_DEBUG
										//cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
										//cout << "currentRelationInList->relationGovernorIndex = " << currentRelationInList->relationGovernorIndex << endl;
										//cout << "currentRelationInList->relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
										#endif

										currentFeatureInList = firstFeatureInList;
										for(int f=0; currentFeatureInList->entityIndex != currentRelationInList->relationDependentIndex; f++)
										{
											currentFeatureInList = currentFeatureInList->next;
										}
										currentRelationInList->relationDependent = currentFeatureInList->lemma;
										currentFeatureInList = firstFeatureInList;
										for(int f=0; currentFeatureInList->entityIndex != currentRelationInList->relationGovernorIndex; f++)
										{
											currentFeatureInList = currentFeatureInList->next;
										}
										currentRelationInList->relationGovernor = currentFeatureInList->lemma;

										#ifdef GIA_NLP_DEBUG
										//cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
										//cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
										#endif

										//added GIA 2d1a
										if(currentRelationInList->relationGovernorIndex > maxNumberOfWordsInSentence)
										{
											maxNumberOfWordsInSentence = currentRelationInList->relationGovernorIndex;
										}
										if(currentRelationInList->relationDependentIndex > maxNumberOfWordsInSentence)
										{
											maxNumberOfWordsInSentence = currentRelationInList->relationDependentIndex;
										}

										#ifdef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
										#ifdef GIA_NLP_PARSER_STANFORD_PARSER_DISABLE_ROOT_RELATION
										if(currentRelationInList->relationType != RELATION_TYPE_ROOT)
										{
										#endif
										#endif
											Relation * newRelation = new Relation();
											currentRelationInList->next = newRelation;
											currentRelationInList = currentRelationInList->next;
										#ifdef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
										#ifdef GIA_NLP_PARSER_STANFORD_PARSER_DISABLE_ROOT_RELATION
										}
										#endif
										#endif

										currentTagInDependencies = currentTagInDependencies->nextTag;
									}
								}
							}
						}
						#ifdef GIA_RECORD_MAX_FEATURE_INDEX
						currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;
						#endif

					}
				}

				#ifdef GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
				if(invalidSentenceFoundIsolatedFullStop)
				{
					#ifdef GIA_NLP_DEBUG
					//cout << "\n\ninvalidSentenceFoundIsolatedFullStop\n\n" << endl;
					#endif
					//currentSentence->firstFeatureInList = new Feature();	//shouldn't be required, it will be overwritten automatically
				}
				else
				{
				#endif
					if(createNewSentences)
					{
						Sentence * newSentence = new Sentence();
						newSentence->previous = currentSentence;
						currentSentence->next = newSentence;
					}
					currentSentence = currentSentence->next;

				#ifdef GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
				}
				#endif

				currentTagInSentences = currentTagInSentences->nextTag;
			}
		}

		StanfordCoreNLPcoreference * firstCoreferenceInList = firstSentenceInList->firstCoreferenceInList;
		StanfordCoreNLPcoreference * currentCoreferenceInList = firstCoreferenceInList;

		if(parseFeatures)
		{
			currentTagInDocument = currentTagInDocument->nextTag;
			bool hasCoreferences = false;
			XMLparserTag * firstTagInCoreferences = parseTagDownALevel(currentTagInDocument, StanfordCoreNLP_XML_TAG_coreferences, &hasCoreferences);
			if(hasCoreferences)
			{
				XMLparserTag * currentTagInnCoreferences = firstTagInCoreferences;
				while(currentTagInnCoreferences->nextTag != NULL)
				{
					XMLparserTag * firstTagInCoreference = parseTagDownALevel(currentTagInnCoreferences, StanfordCoreNLP_XML_TAG_coreference, &result);
					XMLparserTag * currentTagInnCoreference = firstTagInCoreference;

					StanfordCoreNLPmention * firstMentionInList = currentCoreferenceInList->firstMentionInList;
					StanfordCoreNLPmention * currentMentionInList = firstMentionInList;
					while(currentTagInnCoreference->nextTag != NULL)
					{
						if(currentTagInnCoreference->firstAttribute->name == StanfordCoreNLP_XML_ATTRIBUTE_representative)
						{
							string representativeString = currentTagInnCoreference->firstAttribute->value;
							if(representativeString == "true")
							{
								#ifdef GIA_NLP_DEBUG
								cout << "representative found" << endl;
								#endif
								currentMentionInList->representative = true;
							}
						}

						XMLparserTag * firstTagInMention = parseTagDownALevel(currentTagInnCoreference, StanfordCoreNLP_XML_TAG_mention, &result);
						XMLparserTag * currentTagInMention = firstTagInMention;
						while(currentTagInMention->nextTag != NULL)
						{
							if(currentTagInMention->name == StanfordCoreNLP_XML_TAG_sentence)
							{
								int TagValue = atoi(currentTagInMention->value.c_str());
								currentMentionInList->sentence = TagValue;
							}
							else if(currentTagInMention->name == StanfordCoreNLP_XML_TAG_start)
							{
								int TagValue = atoi(currentTagInMention->value.c_str());
								currentMentionInList->start = TagValue;
							}
							else if(currentTagInMention->name == StanfordCoreNLP_XML_TAG_end)
							{
								int TagValue = atoi(currentTagInMention->value.c_str());
								currentMentionInList->end = TagValue;
							}
							else if(currentTagInMention->name == StanfordCoreNLP_XML_TAG_head)
							{
								int TagValue = atoi(currentTagInMention->value.c_str());
								currentMentionInList->head = TagValue;
							}

							currentTagInMention = currentTagInMention->nextTag;
						}

						StanfordCoreNLPmention * newMention = new StanfordCoreNLPmention();
						currentMentionInList->next = newMention;
						currentMentionInList = currentMentionInList->next;

						currentTagInnCoreference = currentTagInnCoreference->nextTag;
					}

					StanfordCoreNLPcoreference * newCoreference = new StanfordCoreNLPcoreference();
					currentCoreferenceInList->next = newCoreference;
					currentCoreferenceInList = currentCoreferenceInList->next;

					currentTagInnCoreferences = currentTagInnCoreferences->nextTag;
				}
			}
		}

		#ifdef GIA_NLP_DEBUG
		currentSentence = firstParagraphInList->firstSentenceInList;
		while(currentSentence->next != NULL)
		{
			cout << "currentSentence->sentenceIndex = " << currentSentence->sentenceIndex << endl;

			Relation * currentRelationInList = currentSentence->firstRelationInList;
			while(currentRelationInList->next != NULL)
			{
				cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
				cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
				cout << "currentRelationInList->relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
				cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
				cout << "currentRelationInList->relationGovernorIndex = " << currentRelationInList->relationGovernorIndex << endl;

				currentRelationInList = currentRelationInList->next;
			}
			Feature * currentFeatureInList = currentSentence->firstFeatureInList;
			while(currentFeatureInList->next != NULL)
			{
				cout << "currentFeatureInList->entityIndex = " << currentFeatureInList->entityIndex << endl;
				cout << "currentFeatureInList->word = " << currentFeatureInList->word << endl;
				cout << "currentFeatureInList->lemma = " << currentFeatureInList->lemma << endl;
				cout << "currentFeatureInList->type = " << currentFeatureInList->type << endl;
				cout << "currentFeatureInList->grammar = " << currentFeatureInList->grammar << endl;
				cout << "currentFeatureInList->CharacterOffsetBegin = " << currentFeatureInList->CharacterOffsetBegin << endl;
				cout << "currentFeatureInList->CharacterOffsetEnd = " << currentFeatureInList->CharacterOffsetEnd << endl;
				cout << "currentFeatureInList->stanfordPOS = " << currentFeatureInList->stanfordPOS << endl;
				cout << "currentFeatureInList->NER = " << currentFeatureInList->NER << endl;
				cout << "currentFeatureInList->NormalizedNER = " << currentFeatureInList->NormalizedNER << endl;
				cout << "currentFeatureInList->Timex = " << currentFeatureInList->Timex << endl;

				currentFeatureInList = currentFeatureInList->next;
			}

			currentSentence = currentSentence->next;
		}
		currentCoreferenceInList = firstParagraphInList->firstSentenceInList->firstCoreferenceInList;
		while(currentCoreferenceInList->next != NULL)
		{
			StanfordCoreNLPmention * currentMentionInList = currentCoreferenceInList->firstMentionInList;
			while(currentMentionInList->next != NULL)
			{
				cout << "currentMentionInList->representative = " << int(currentMentionInList->representative) << endl;
				cout << "currentMentionInList->sentence = " << currentMentionInList->sentence << endl;
				cout << "currentMentionInList->start = " << currentMentionInList->start << endl;
				cout << "currentMentionInList->end = " << currentMentionInList->end << endl;
				cout << "currentMentionInList->head = " << currentMentionInList->head << endl;

				currentMentionInList = currentMentionInList->next;
			}
			currentCoreferenceInList = currentCoreferenceInList->next;
		}

		//cout << "GIA_NLP_DEBUG complete" << endl;
		#endif
	}

	#ifdef GIA_FREE_MEMORY1
	delete firstTagInXMLFile;
	#endif

	return result;
}

#endif

#ifdef GIA_USE_STANFORD_PARSER

//http://rosettacode.org/wiki/Count_occurrences_of_a_substring#C.2B.2B
// returns count of non-overlapping occurrences of 'sub' in 'str'
int countSubstring(const std::string& str, const std::string& sub)
{
    if (sub.length() == 0) return 0;
    int count = 0;
    for (size_t offset = str.find(sub); offset != std::string::npos;
	 offset = str.find(sub, offset + sub.length()))
    {
        ++count;
    }
    return count;
}


bool parseStanfordParserFile(string inputTextNLPrelationXMLfileName, bool isQuery, Paragraph * firstParagraphInList, bool createNewSentences, bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	Paragraph * currentParagraph = firstParagraphInList;
	Paragraph * newParagraph = new Paragraph();
	newParagraph->previous = currentParagraph;
	currentParagraph->next = newParagraph;

	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;
	bool parseGIA2file = false;
	if(!parseStanfordParserFile(inputTextNLPrelationXMLfileName, isQuery, firstSentenceInList, createNewSentences, parseGIA2file, onlyParseIfCorpusLookupFailed))
	{
		result = false;
	}
	return result;
}

bool parseStanfordParserFile(string inputTextNLPrelationXMLfileName, bool isQuery, Sentence * firstSentenceInList, bool createNewSentences, bool parseGIA2file, bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	Sentence * currentSentence = firstSentenceInList;

	string currentStanfordParserOutputParagraphString = "";

	bool parsingWordsAndTags = true;
	bool parsingPenn = false;
	bool parsingTypedDependencies = false;	//parse tree is first set in list

	ifstream parseFileObject(inputTextNLPrelationXMLfileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//xml file does not exist in current directory.
		cout << "Error: Stanford Parser Output File does not exist in current directory: " << inputTextNLPrelationXMLfileName << endl;

		/*
		char currentFolder[EXE_FOLDER_PATH_MAX_LENGTH];
		getCurrentDirectory(currentFolder);
		cout << "currentFolder = " << currentFolder << endl;
		*/
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		bool newLineDetected = false;
		while(parseFileObject.get(currentToken))
		{
			cout << currentToken;

			if(currentToken == CHAR_NEWLINE)
			{
				if(newLineDetected)
				{
					newLineDetected = false;
					//two consecutive new line characters detected..
					if(parsingWordsAndTags)
					{
						parsingWordsAndTags = false;

						#ifdef STANFORD_PARSER_SENTENCE_SKIP_SUPPORT
						if(!parseGIA2file)
						{
							//take into account sentences skipped by Stanford Parser - added 30 June 2012 to disregard (eg large) sentences that have been skipped
							int numberOfSentencesSkipped = countSubstring(currentStanfordParserOutputParagraphString, STANFORD_PARSER_SENTENCE_SKIPPED_TEXT);
							for(int i=0; i<numberOfSentencesSkipped; i++)
							{
								#ifdef GIA_NLP_DEBUG
								//cout << "numberOfSentencesSkipped = " << numberOfSentencesSkipped << endl;
								#endif
								if(createNewSentences)
								{
									Sentence * newSentence = new Sentence();
									newSentence->previous = currentSentence;
									currentSentence->next = newSentence;
								}
								currentSentence = currentSentence->next;
							}
						}
						#endif

						//cout << "at23" << endl;
						if((!parseGIA2file && (!onlyParseIfCorpusLookupFailed || !(currentSentence->corpusLookupSuccessful))) || (parseGIA2file && createNewSentences))	//added createNewSentences condition GIA 2d1a [for generateAllPermutationsFromSemanticRelationsFile()]
						{
							//cout << "GIATHparseStanfordParseWordsAndPOStagsText: " << endl;
							int maxNumberOfWordsInSentence = 0;
							#ifdef GIA_NLP_DEBUG
							cout << "currentStanfordParserOutputParagraphString = " << currentStanfordParserOutputParagraphString << endl;
							#endif
							if(!parseGIA2file && createNewSentences)
							{
								cout << "parseStanfordParserFile() error: (!parseGIA2file && createNewSentences); Stanford Parser output cannot be used to create new sentences as it contains no lemmas" << endl;
							}
							bool createFeaturesGIA2only = createNewSentences;	//NB words and POS tags cannot be ordinarily used to create features as they contain word orig not lemmas (this option is only supported for GIA2)
							GIATHparseStanfordParseWordsAndPOStagsText(&currentStanfordParserOutputParagraphString, currentSentence, &maxNumberOfWordsInSentence, createFeaturesGIA2only);
							if(createNewSentences)
							{
								#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE
								currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;
								#endif
							}
						}

						if(parseGIA2file)
						{
							parsingTypedDependencies = true;
						}
						else
						{
							parsingPenn = true;
						}

						currentStanfordParserOutputParagraphString = "";	//reset currentStanfordParserOutputParagraphString for parsing of dependency relations

					}
					else if(parsingPenn)
					{
						parsingPenn = false;
						parsingTypedDependencies = true;

						currentStanfordParserOutputParagraphString = "";	//reset currentStanfordParserOutputParagraphString for parsing of dependency relations
					}
					else if(parsingTypedDependencies)
					{
						#ifdef GIA_NLP_DEBUG
						//cout << "GIATHparseStanfordParserRelationsText: currentStanfordParserOutputParagraphString = " << currentStanfordParserOutputParagraphString << endl;
						#endif

						if(!onlyParseIfCorpusLookupFailed || !(currentSentence->corpusLookupSuccessful))
						{
							#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
							int maxNumberOfWordsInSentence = currentSentence->maxNumberOfWordsInSentence;
							#else
							int maxNumberOfWordsInSentence = 0;
							#endif
							bool featuresNotPreviouslyFilled = createNewSentences;
							GIATHparseStanfordParserRelationsText(&currentStanfordParserOutputParagraphString, currentSentence, &maxNumberOfWordsInSentence, featuresNotPreviouslyFilled, parseGIA2file, false);
							#ifdef GIA_NLP_DEBUG
							//cout << "finished GIATHparseStanfordParserRelationsText" << endl;
							#endif
							if(createNewSentences)
							{
								Sentence * newSentence = new Sentence();
								newSentence->previous = currentSentence;
								currentSentence->next = newSentence;
								#ifdef GIA_RECORD_MAX_FEATURE_INDEX
								currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;
								#endif
							}
						}
						currentStanfordParserOutputParagraphString = "";

						currentSentence = currentSentence->next;

						parsingTypedDependencies = false;
						parsingWordsAndTags = true;
					}
				}
				else
				{
					currentStanfordParserOutputParagraphString = currentStanfordParserOutputParagraphString + currentToken;
					newLineDetected = true;
				}
			}
			else
			{
				newLineDetected = false;
				currentStanfordParserOutputParagraphString = currentStanfordParserOutputParagraphString + currentToken;
			}

			charCount++;
		}
	}

	#ifdef GIA_NLP_DEBUG
	currentSentence = firstSentenceInList;
	while(currentSentence->next != NULL)
	{
		cout << "currentSentence->sentenceIndex = " << currentSentence->sentenceIndex << endl;

		Relation * currentRelationInList = currentSentence->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{
			cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			cout << "currentRelationInList->relationDependent = " << currentRelationInList->relationDependent << endl;
			cout << "currentRelationInList->relationDependentIndex = " << currentRelationInList->relationDependentIndex << endl;
			cout << "currentRelationInList->relationGovernor = " << currentRelationInList->relationGovernor << endl;
			cout << "currentRelationInList->relationGovernorIndex = " << currentRelationInList->relationGovernorIndex << endl;

			currentRelationInList = currentRelationInList->next;
		}

		currentSentence = currentSentence->next;
	}
	#endif

	return result;
}
#endif




#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
void outputInternalRelationsInRelexFormat(string * nameOfRelexCompactFormatCFFfile, string * originalInputFileName, Paragraph * firstParagraphInList, int NLPdependencyRelationsParser, int NLPfeatureParser, string NLPexeFolderArray[])
{
	bool result = true;

	string parserTagValue = "";
	parserTagValue = parserTagValue + "NLP Dependency Relations Parser: " + NLPexeFolderArray[NLPdependencyRelationsParser] + " | NLP Feature Tag Parser: " + NLPexeFolderArray[NLPfeatureParser];

	struct tm *current;
	time_t now;
	time(&now);
	current = localtime(&now);
	char timeAndDateString[100];
	sprintf(timeAndDateString, "%i-%.2i-%i %i:%i:%i +1000", (current->tm_year + TM_STRUCT_YEAR_OFFSET), (current->tm_mon + 1), current->tm_mday, current->tm_hour, current->tm_min, current->tm_sec);

 	XMLparserTag * firstTagInXMLFile = new XMLparserTag();

	XMLparserTag * currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = Relex_CFF_XML_TAG_nlparse;
	XMLparserTag * newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;
	XMLParserAttribute * currentAttribute = currentTagL0->firstAttribute;
	currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_xmlns;
	currentAttribute->value = Relex_CFF_XML_ATTRIBUTE_xmlns_DEFAULT_VALUE;
	XMLParserAttribute * newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;

	XMLparserTag * firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag * newTag1 = new XMLparserTag();
	currentTagL1->nextTag = newTag1;

	currentTagL1->name = Relex_CFF_XML_TAG_parser;
	currentTagL1->value = parserTagValue;
	currentTagL1 = currentTagL1->nextTag;
	newTag1 = new XMLparserTag();
	currentTagL1->nextTag = newTag1;

	currentTagL1->name = Relex_CFF_XML_TAG_date;
	currentTagL1->value = timeAndDateString;
	currentTagL1 = currentTagL1->nextTag;
	newTag1 = new XMLparserTag();
	currentTagL1->nextTag = newTag1;

	currentTagL1->name = Relex_CFF_XML_TAG_source;
	currentTagL1->value = "";
	currentAttribute = currentTagL1->firstAttribute;
	currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_url;
	currentAttribute->value = *originalInputFileName;
	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentTagL1 = currentTagL1->nextTag;
	newTag1 = new XMLparserTag();
	currentTagL1->nextTag = newTag1;

	Paragraph * currentParagraph = firstParagraphInList;
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;

	#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
	while(currentParagraph->next != NULL)
	{
		firstSentenceInList = currentParagraph->firstSentenceInList;

		currentTagL1->name = Relex_CFF_XML_TAG_paragraph;
		XMLparserTag * firstTagL1b = new XMLparserTag();
		currentTagL1->firstLowerLevelTag = firstTagL1b;
	#else
		XMLparserTag * firstTagL1b = currentTagL1;
	#endif

		XMLparserTag * currentTagL1b = firstTagL1b;
		Sentence * currentSentence = firstSentenceInList;
		while(currentSentence->next != NULL)
		{
			char sentenceIndexString[10];
			sprintf(sentenceIndexString, "%d", currentSentence->sentenceIndex);

			currentTagL1b->name = Relex_CFF_XML_TAG_sentence;
			XMLparserTag * firstTagL2 = new XMLparserTag();
			currentTagL1b->firstLowerLevelTag = firstTagL2;
			currentAttribute = currentTagL1b->firstAttribute;
			currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_index;
			currentAttribute->value = sentenceIndexString;
			newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;
			currentAttribute = currentAttribute->nextAttribute;
			currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_parses;
			currentAttribute->value = "1";
			newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;

			XMLparserTag * currentTagL2 = firstTagL2;
			currentTagL2->name = Relex_CFF_XML_TAG_parse;
			XMLparserTag * firstTagL3 = new XMLparserTag();
			currentTagL2->firstLowerLevelTag = firstTagL3;
			currentAttribute = currentTagL2->firstAttribute;
			currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_id;
			currentAttribute->value = "1";
			newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;

			XMLparserTag * currentTagL3 = firstTagL3;
			currentTagL3->name = Relex_CFF_XML_TAG_features;
			currentTagL3->value = generateRelexCFFfeatureTagContent(currentSentence->firstFeatureInList);	//fill in currentTagL3->value with features
			XMLparserTag * newTag3 = new XMLparserTag();
			currentTagL3->nextTag = newTag3;
			currentTagL3= currentTagL3->nextTag;

			currentTagL3->name = Relex_CFF_XML_TAG_relations;
			currentTagL3->value = generateRelexCFFrelationTagContent(currentSentence->firstRelationInList);	//fill in currentTagL3->value with relations
			newTag3 = new XMLparserTag();
			currentTagL3->nextTag = newTag3;
			currentTagL3= currentTagL3->nextTag;

			XMLparserTag * newTag2 = new XMLparserTag();
			currentTagL2->nextTag = newTag2;

			XMLparserTag * newTag1b = new XMLparserTag();
			currentTagL1b->nextTag = newTag1b;
			currentTagL1b =	currentTagL1b->nextTag;
			currentSentence = currentSentence->next;
		}



	#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		newTag1 = new XMLparserTag();
		currentTagL1->nextTag = newTag1;
		currentTagL1 = currentTagL1->nextTag;
		currentParagraph = currentParagraph->next;
	}
	#endif

 	if(!writeXMLfile(*nameOfRelexCompactFormatCFFfile, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;

}

#endif


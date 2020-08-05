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
 * File Name: GIAnlp.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3m3c 01-August-2020
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: NLP - external natural language processing
 * /
 *******************************************************************************/


#include "GIAnlp.hpp"

#ifndef LINUX
	#include <windows.h>
#endif


void GIAnlpClass::executeNLPparser(const string inputTextPlainTXTfileName, const string inputTextNLPXMLfileName, const GIAtranslatorVariablesClass* translatorVariables, const bool parseRelationsOrFeatures)
{	
	int NLPParser = false;
	if(parseRelationsOrFeatures)
	{
		NLPParser = translatorVariables->NLPdependencyRelationsParser;
	}
	else
	{
		NLPParser = translatorVariables->NLPfeatureParser;
	}
	bool NLPrelexCompatibilityMode = translatorVariables->NLPrelexCompatibilityMode;
	#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
	if(!parseRelationsOrFeatures)
	{
		NLPrelexCompatibilityMode = true;
	}
	#endif
	#ifdef GIA_NLP_CLIENT_SERVER
	bool NLPclient = translatorVariables->NLPclient;				
	#endif
	
	/*
	NB execute NLP on current folder not saved input folder (this is required for when a preprocessor eg LRP/CE has been executed on the input text):
	so must assume current folder has been set to the folder where the [pre-processed] plain text file exists (if not the actual input folder)
	*/
	string currentFolder = SHAREDvars.getCurrentDirectory();

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
	#ifdef GIA_RELEX
	if(NLPParser == GIA_NLP_PARSER_RELEX)
	{
		if(NLPrelexCompatibilityMode)
		{
			#ifdef GIA_RELEX_CLIENT
			if(NLPclient)
			{
				NLPParserExecutableName = GIA_RELEX_CLIENT_WITH_STANFORD_COMPATIBILITY_MODE_EXECUTABLE_NAME;
			}
			else
			{
			#endif
				NLPParserExecutableName = GIA_RELEX_WITH_STANFORD_COMPATIBILITY_MODE_EXECUTABLE_NAME;
			#ifdef GIA_RELEX_CLIENT
			}
			#endif
		}
		else
		{
			#ifdef GIA_RELEX_CLIENT
			if(NLPclient)
			{
				NLPParserExecutableName = GIA_RELEX_CLIENT_EXECUTABLE_NAME;
			}
			else
			{
			#endif
				NLPParserExecutableName = GIA_RELEX_EXECUTABLE_NAME;
			#ifdef GIA_RELEX_CLIENT
			}		
			#endif
		}
		NLPexeFolder = translatorVariables->NLPexeFolderArray[GIA_NLP_PARSER_RELEX];
	}
	#endif
	#ifdef GIA_STANFORD_CORENLP
	if(NLPParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		#ifdef STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS
		if(parseRelationsOrFeatures)
		{
			#ifdef GIA_STANFORD_NLP_CLIENT
			if(NLPclient)
			{
				NLPParserExecutableName = GIA_STANFORD_NLP_CLIENT_EXECUTABLE_NAME_WITHOUT_INDEPENDENT_POS_TAGGER;
			}
			else
			{
			#endif
				NLPParserExecutableName = GIA_STANFORD_NLP_EXECUTABLE_NAME_WITHOUT_INDEPENDENT_POS_TAGGER;
			#ifdef GIA_STANFORD_NLP_CLIENT
			}
			#endif
		}
		else
		{
		#endif
			#ifdef GIA_STANFORD_NLP_CLIENT
			if(NLPclient)
			{
				NLPParserExecutableName = GIA_STANFORD_NLP_CLIENT_EXECUTABLE_NAME;
			}
			else
			{
			#endif
				NLPParserExecutableName = GIA_STANFORD_NLP_EXECUTABLE_NAME;
			#ifdef GIA_STANFORD_NLP_CLIENT
			}
			#endif
		#ifdef STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS
		}
		#endif
		NLPexeFolder = translatorVariables->NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_CORENLP];
	}
	#endif
	#ifdef GIA_STANFORD_PARSER
	if(NLPParser == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		#ifdef GIA_STANFORD_PARSER_CLIENT
		if(NLPclient)
		{
			NLPParserExecutableName = GIA_STANFORD_PARSER_CLIENT_EXECUTABLE_NAME;
		}
		else
		{
		#endif
			NLPParserExecutableName = GIA_STANFORD_PARSER_EXECUTABLE_NAME;
		#ifdef GIA_STANFORD_PARSER_CLIENT
		}
		#endif
		NLPexeFolder = translatorVariables->NLPexeFolderArray[GIA_NLP_PARSER_STANFORD_PARSER];
	}
	#endif

	//execute NLP parser on plain text
	string executeNLPCommand = "";
	executeNLPCommand = executeNLPCommand + NLPexeFolder + "/" + NLPParserExecutableName + " " + inputTextPlainTXTfileName + " " + inputTextNLPXMLfileName + " " + currentFolder + " " + outputFolder + " " + StanfordCoreNLPdefaultOutputFileExtensionAppend;

	SHAREDvars.setCurrentDirectory(NLPexeFolder);

	system(executeNLPCommand.c_str());

	SHAREDvars.setCurrentDirectory(outputFolder);

	#ifdef GIA_STANFORD_CORENLP
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

	SHAREDvars.setCurrentDirectory(currentFolder);
}


bool GIAnlpClass::parseNLPparserFile(const string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const int NLPfeatureParser, const int NLPdependencyRelationsParser, const bool NLPrelexCompatibilityMode)
{
	bool result = true;

	bool createNewSentences = true;


	//Parse Features
	if(!parseNLPparserFeaturesFile(inputTextNLPfeatureXMLfileName, isQuery, firstSentenceInList, NLPfeatureParser, &createNewSentences))
	{
		result = false;
	}


	//Parse Relations
	if(result)
	{
		if(!parseNLPparserRelationsFile(inputTextNLPrelationXMLfileName, isQuery, firstSentenceInList, NLPdependencyRelationsParser, NLPrelexCompatibilityMode, createNewSentences, false))
		{
			result = false;
		}
	}

	return result;
}

bool GIAnlpClass::parseNLPparserFeaturesFile(const string inputTextNLPfeatureXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const int NLPfeatureParser, bool* createNewSentences)
{
	bool result = true;

	#ifdef GIA_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
		bool NLPrelexCompatibilityMode = true;		//GIA_SEM_REL_TRANSLATOR_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2 - use stanford compatibilty mode only when parsing features, and only add special relations; GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_MODAL_AUX, GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_PASSIVE_AUX, GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_COPULA, GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_DETERMINER
		#else
		bool NLPrelexCompatibilityMode = false;		//irrelevant (not used) - only used when parsing syntatic dependency relations of a Relex file
		#endif
		if(!parseRelexFile(inputTextNLPfeatureXMLfileName, isQuery, firstSentenceInList, false, true, NLPrelexCompatibilityMode,* createNewSentences, false))
		{
			result = false;
		}
		*createNewSentences = false;
	}
	#endif
	#ifdef GIA_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		if(!parseStanfordCoreNLPfile(inputTextNLPfeatureXMLfileName, isQuery, firstSentenceInList, false, true,* createNewSentences, false))
		{
			result = false;
		}
		*createNewSentences = false;
	}
	#endif
	#ifndef GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
	#ifdef GIA_STANFORD_PARSER
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		//stanford parser does not create lemmas and so can't be used for full GIA feature extraction
		/*
		if(isQuery)
		{
			cout << "error: parseNLPparserFile() does not support queries at present with (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER). Set feature parser to RelEx or Stanford Core NLP for queries" << endl;

		}
		*/
		cerr << "error: parseNLPparserFeaturesFile{} does not parse features when (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER). GIAfeature extraction is not supported with GIA_NLP_PARSER_STANFORD_PARSER because Stanford Parser does not identify lemmas. Set feature parser to RelEx or Stanford Core NLP to extract features" << endl;
		exit(EXIT_ERROR);
	}
	#endif
	#endif

	return result;
}

bool GIAnlpClass::parseNLPparserRelationsFile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const int NLPdependencyRelationsParser, const bool NLPrelexCompatibilityMode, const bool createNewSentences, const bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	#ifdef GIA_RELEX
	if(NLPdependencyRelationsParser == GIA_NLP_PARSER_RELEX)
	{
		if(!parseRelexFile(inputTextNLPrelationXMLfileName, isQuery, firstSentenceInList, true, false, NLPrelexCompatibilityMode, createNewSentences, onlyParseIfCorpusLookupFailed))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_STANFORD_CORENLP
	if(NLPdependencyRelationsParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		if(!parseStanfordCoreNLPfile(inputTextNLPrelationXMLfileName, isQuery, firstSentenceInList, true, false, createNewSentences, onlyParseIfCorpusLookupFailed))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_STANFORD_PARSER
	if(NLPdependencyRelationsParser == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		if(!parseStanfordParserFile(inputTextNLPrelationXMLfileName, isQuery, firstSentenceInList, createNewSentences, onlyParseIfCorpusLookupFailed))
		{
			result = false;
		}
	}
	#endif

	return result;
}

#ifdef GIA_RELEX
bool GIAnlpClass::parseRelexFile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const bool parseRelations, const bool parseFeatures, const bool NLPrelexCompatibilityMode, const bool createNewSentences, const bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	GIAsentence* currentSentence = firstSentenceInList;

	XMLparserTag* firstTagInXMLFile = new XMLparserTag();
	if(!XMLparserClass.readXMLfile(inputTextNLPrelationXMLfileName, firstTagInXMLFile))
	{
		result = false;
	}

	if(result)
	{
		XMLparserTag* currentTag = firstTagInXMLFile;
		currentTag = XMLparserClass.parseTagDownALevel(currentTag, Relex_CFF_XML_TAG_nlparse, &result);
		
		if(result)
		{
			//now for every sentence;
			while(currentTag->nextTag != NULL)
			{
				if(currentTag->name == Relex_CFF_XML_TAG_sentence)
				{
					if(!onlyParseIfCorpusLookupFailed || !(currentSentence->semanticParserSuccessful))
					{
						//locate and record sentence index
						XMLparserAttribute* firstAttributeInSentenceTag = currentTag->firstAttribute;
						XMLparserAttribute* currentAttributeInSentenceTag = firstAttributeInSentenceTag;
						bool foundSentenceIndexAttribute = false;
						while(currentAttributeInSentenceTag->nextAttribute != NULL)
						{
							if(currentAttributeInSentenceTag->name == Relex_CFF_XML_ATTRIBUTE_index)
							{
								int sentenceIndex = SHAREDvars.convertStringToInt(currentAttributeInSentenceTag->value);
								currentSentence->sentenceIndex = sentenceIndex;
								foundSentenceIndexAttribute = true;
							}
							currentAttributeInSentenceTag = currentAttributeInSentenceTag->nextAttribute;
						}
						if(!foundSentenceIndexAttribute)
						{
							cerr << "error: sentence index attribute expected" << endl;
							exit(EXIT_ERROR);
						}

						XMLparserTag* firstTagInSentence;
						firstTagInSentence = XMLparserClass.parseTagDownALevel(currentTag, Relex_CFF_XML_TAG_sentence, &result);
						XMLparserTag* firstTagInFirstParse;
						firstTagInFirstParse = XMLparserClass.parseTagDownALevel(firstTagInSentence, Relex_CFF_XML_TAG_parse, &result);

						if(result)
						{
							XMLparserTag* currentTagInParse = firstTagInFirstParse;
							while(currentTagInParse->nextTag != NULL)
							{

								if(currentTagInParse->name == Relex_CFF_XML_TAG_features)
								{
									if(parseFeatures)
									{
										int numberOfWordsInSentence = 0;
										GIAnlpParser.GIATHparseRelexFeaturesText(&(currentTagInParse->value), currentSentence, &numberOfWordsInSentence);
										#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE
										currentSentence->numberOfWordsInSentence = numberOfWordsInSentence;
										#endif
										if(isQuery)
										{
											#ifdef GIA_QUERIES_MUST_BE_QUESTIONS
											if(!(currentSentence->isQuestion))
											{
												cerr << "error: GIA query is not a question" << endl;
												exit(EXIT_ERROR);
											}
											#endif
										}
									}
								}
								else if(currentTagInParse->name == Relex_CFF_XML_TAG_relations)
								{
									if(parseRelations)
									{
										#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
										int numberOfWordsInSentence = currentSentence->numberOfWordsInSentence;
										#else
										int numberOfWordsInSentence = 0;
										#endif
										GIAnlpParser.GIATHparseRelexRelationsText(&(currentTagInParse->value), currentSentence, &numberOfWordsInSentence, NLPrelexCompatibilityMode);
										#ifdef GIA_RECORD_MAX_FEATURE_INDEX
										currentSentence->numberOfWordsInSentence = numberOfWordsInSentence;
										#endif
									}
									#ifdef GIA_SEM_REL_TRANSLATOR_SUPPORT_USE_RELEX_COMPATIBILITY_MODE_FOR_FEATURE_PARSER_TO_GENERATE_ADDITIONAL_RELATIONS_REQUIRED_BY_GIA2
									if(!parseRelations && NLPrelexCompatibilityMode)
									{
										int numberOfWordsInSentence = 0;
										bool featuresNotPreviouslyFilled = false;	//NB featuresNotPreviouslyFilled could be set to true as secondary relations are only used to create GIA2 semantic relations, and as such lemma information is disgarded [only feature indicies are important]
										string relationsText = currentTagInParse->value;
										if(relationsText[0] == '\n')
										{
											relationsText = relationsText.substr(1, (relationsText.length() -1));
										}

										GIAnlpParser.GIATHparseStanfordParserRelationsText(&relationsText, currentSentence, &numberOfWordsInSentence, featuresNotPreviouslyFilled, false, NLPrelexCompatibilityMode);
									}
									#endif
								}

								currentTagInParse = currentTagInParse->nextTag;
							}

						}

						if(createNewSentences)
						{
							GIAsentence* newSentence = new GIAsentence();
							newSentence->previous = currentSentence;
							currentSentence->next = newSentence;
						}
					}

					currentSentence = currentSentence->next;

				}
				currentTag = currentTag->nextTag;
			}

		}
	}

	delete firstTagInXMLFile;

	return result;
}
#endif


#ifdef GIA_STANFORD_CORENLP
bool GIAnlpClass::parseStanfordCoreNLPfile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const bool parseRelations, const bool parseFeatures, const bool createNewSentences, const bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	GIAsentence* currentSentence = firstSentenceInList;
	
	XMLparserTag* firstTagInXMLFile = new XMLparserTag();
	if(!XMLparserClass.readXMLfile(inputTextNLPrelationXMLfileName, firstTagInXMLFile))
	{
		result = false;
	}

	if(result)
	{

		XMLparserTag* firstTagInRoot = XMLparserClass.parseTagDownALevel(firstTagInXMLFile, StanfordCoreNLP_XML_TAG_root, &result);
		XMLparserTag* firstTagInDocument = XMLparserClass.parseTagDownALevel(firstTagInRoot, StanfordCoreNLP_XML_TAG_document, &result);
		XMLparserTag* currentTagInDocument = firstTagInDocument;

		XMLparserTag* firstTagInSentences = XMLparserClass.parseTagDownALevel(currentTagInDocument, StanfordCoreNLP_XML_TAG_sentences, &result);
		if(result && (firstTagInSentences != NULL))
		{
			XMLparserTag* currentTagInSentences = firstTagInSentences;
			while(currentTagInSentences->nextTag != NULL)
			{
				#ifdef GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
				bool invalidSentenceFoundIsolatedFullStop = false;
				#endif

				if(!onlyParseIfCorpusLookupFailed || !(currentSentence->semanticParserSuccessful))
				{
					string sentenceIndexString = currentTagInSentences->firstAttribute->value;
					currentSentence->sentenceIndex = SHAREDvars.convertStringToInt(sentenceIndexString);

					XMLparserTag* firstTagInSentence = XMLparserClass.parseTagDownALevel(currentTagInSentences, StanfordCoreNLP_XML_TAG_sentence, &result);

					XMLparserTag* currentTagInSentence = firstTagInSentence;

					GIAfeature* firstFeatureInList = currentSentence->firstFeatureInList;
					GIAfeature* currentFeatureInList = firstFeatureInList;

					XMLparserTag* firstTagInTokens = XMLparserClass.parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_XML_TAG_tokens, &result);
					XMLparserTag* currentTagInTokens = firstTagInTokens;
					bool isQuestion = false;

					int numberOfWordsInSentence = 0;

					while(currentTagInTokens->nextTag != NULL)
					{
						#ifdef GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
						int lastFeatureEntityIndex = INT_DEFAULT_VALUE;
						string lastFeatureWord = "";
						#endif

						if(parseFeatures)
						{
							string entityIndexString = currentTagInTokens->firstAttribute->value;
							currentFeatureInList->entityIndex = SHAREDvars.convertStringToInt(entityIndexString);
							#ifdef GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
							lastFeatureEntityIndex = currentFeatureInList->entityIndex;
							#endif
						}
						#ifdef GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
						else
						{
							string entityIndexString = currentTagInTokens->firstAttribute->value;
							lastFeatureEntityIndex = SHAREDvars.convertStringToInt(entityIndexString);
						}
						#endif

						XMLparserTag* firstTagInToken = XMLparserClass.parseTagDownALevel(currentTagInTokens, StanfordCoreNLP_XML_TAG_token, &result);
						XMLparserTag* currentTagInToken = firstTagInToken;
						while(currentTagInToken->nextTag != NULL)
						{
							if(parseFeatures)
							{
								if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_word)
								{
									string TagValue = currentTagInToken->value;
									currentFeatureInList->word = TagValue;
									#ifndef GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
									#ifdef GIA_SYN_REL_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
									if(currentFeatureInList->word == FEATURE_WORD_QUESTIONMARK)
									{
										isQuestion = true;
									}
									#endif
									#endif
									#ifdef GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
									lastFeatureWord = TagValue;
									#endif
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_lemma)
								{
									string TagValue = currentTagInToken->value;
									currentFeatureInList->lemma = TagValue;
									//cout << "currentFeatureInList->lemma = " << currentFeatureInList->lemma << endl;
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_CharacterOffsetBegin)
								{
									int TagValue = SHAREDvars.convertStringToInt(currentTagInToken->value);
									currentFeatureInList->CharacterOffsetBegin = TagValue;
								}
								else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_CharacterOffsetEnd)
								{
									int TagValue = SHAREDvars.convertStringToInt(currentTagInToken->value);
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
									string TagValue = currentTagInToken->value;
									currentFeatureInList->stanfordPOS = TagValue;
								}
								#ifdef GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
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
							#ifdef GIA_PREPROCESSOR_WORD_REPLACE_OUTPUT_FOR_NLP_TEMPORARILY
							if(GIApreprocessorWordIdentification.getUseLRP())
							{
								bool foundOfficialLRPreplacementString = false;
								GIArelation* currentRelationInListForPrepositionsOnlyIrrelevant = NULL;
								GIApreprocessorWordReduction.revertNLPtagNameToOfficialLRPtagName(currentFeatureInList, currentSentence, currentRelationInListForPrepositionsOnlyIrrelevant, false, &foundOfficialLRPreplacementString);
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

						#ifdef GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
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
							GIAfeature* newFeature = new GIAfeature();
							newFeature->previous = currentFeatureInList;
							currentFeatureInList->next = newFeature;
							currentFeatureInList = currentFeatureInList->next;
						}
						else
						{
							currentFeatureInList = currentFeatureInList->next;
						}
						numberOfWordsInSentence = numberOfWordsInSentence + 1;
						currentTagInTokens = currentTagInTokens->nextTag;
					}

					if(parseFeatures)
					{//calculate question only if parsing features
						#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE
						currentSentence->numberOfWordsInSentence = numberOfWordsInSentence;	//added GIA 2d1a
						#endif

						#ifndef GIA_SYN_REL_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
						#ifdef GIA_SYN_REL_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
						if(isQuery)
						{
							if(isQuestion)
							{
								currentSentence->isQuestion = true;
							}
							else
							{
								#ifdef GIA_QUERIES_MUST_BE_QUESTIONS
								cerr << "error: GIA query is not a question" << endl;
								exit(EXIT_ERROR);
								#endif
							}
						}
						#endif
						#endif
					}

					currentTagInSentence = currentTagInSentence->nextTag;
					XMLparserTag* firstTagInParse = XMLparserClass.parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_XML_TAG_parse, &result);

					if(parseRelations)
					{
						#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
						numberOfWordsInSentence = currentSentence->numberOfWordsInSentence;
						#else
						int numberOfWordsInSentence = 0;
						#endif
						for(int dependenciesSetIndex=0; dependenciesSetIndex< StanfordCoreNLP_numberOfDependencySetsPerSentence; dependenciesSetIndex++)
						{
							GIArelation* firstRelationInList = currentSentence->firstRelationInList;
							GIArelation* currentRelationInList = firstRelationInList;

							currentTagInSentence = currentTagInSentence->nextTag;
							if(dependenciesSetIndex == StanfordCoreNLP_dependencySet_usedByGIA)
							{
								XMLparserTag* firstTagInDependencies = XMLparserClass.parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_relationSetNameArray[dependenciesSetIndex], &result);
								if(firstTagInDependencies != NULL)
								{
									XMLparserTag* currentTagInDependencies = firstTagInDependencies;
									while(currentTagInDependencies->nextTag != NULL)
									{
										XMLparserTag* firstTagInDep = XMLparserClass.parseTagDownALevel(currentTagInDependencies, StanfordCoreNLP_XML_TAG_dep, &result);
										XMLparserTag* governerTagInDep = firstTagInDep;
										XMLparserTag* dependentTagInDep = firstTagInDep->nextTag;

										string relationGovernorIndexString = governerTagInDep->firstAttribute->value;
										string relationDependentIndexString = dependentTagInDep->firstAttribute->value;
										currentRelationInList->relationGovernorIndex = SHAREDvars.convertStringToInt(relationGovernorIndexString);
										currentRelationInList->relationDependentIndex = SHAREDvars.convertStringToInt(relationDependentIndexString);

										currentRelationInList->relationType = currentTagInDependencies->firstAttribute->value;

										//don't use these, use lemmas instead (as per Stanford Core NLP/Relex dependency relation definitions). These are required to be set temporarily for LRP;
										currentRelationInList->relationGovernor = governerTagInDep->value;
										currentRelationInList->relationDependent = dependentTagInDep->value;

										GIAnlpParser.convertStanfordRelationToRelex(currentRelationInList, currentSentence);


										#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
										if(!(currentRelationInList->relationGovernorRevertedToOfficialLRPTemp))
										{
										#endif
											currentFeatureInList = firstFeatureInList;
											for(int f=0; currentFeatureInList->entityIndex != currentRelationInList->relationGovernorIndex; f++)
											{
												currentFeatureInList = currentFeatureInList->next;
											}
											currentRelationInList->relationGovernor = currentFeatureInList->lemma;
										#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
										}
										if(!(currentRelationInList->relationDependentRevertedToOfficialLRPTemp))
										{
										#endif
											currentFeatureInList = firstFeatureInList;
											for(int f=0; currentFeatureInList->entityIndex != currentRelationInList->relationDependentIndex; f++)
											{
												currentFeatureInList = currentFeatureInList->next;
											}
											currentRelationInList->relationDependent = currentFeatureInList->lemma;
										#ifdef GIA_PREPROCESSOR_WORD_MULTIWORD_REDUCTION
										}
										#endif


										//added GIA 2d1a
										if(currentRelationInList->relationGovernorIndex > numberOfWordsInSentence)
										{
											numberOfWordsInSentence = currentRelationInList->relationGovernorIndex;
										}
										if(currentRelationInList->relationDependentIndex > numberOfWordsInSentence)
										{
											numberOfWordsInSentence = currentRelationInList->relationDependentIndex;
										}

										#ifdef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
										#ifdef GIA_NLP_PARSER_STANFORD_PARSER_DISABLE_ROOT_RELATION
										if(currentRelationInList->relationType != GIA_SYN_REL_TRANSLATOR_RELATION_TYPE_ROOT)
										{
										#endif
										#endif
											GIArelation* newRelation = new GIArelation();
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
						currentSentence->numberOfWordsInSentence = numberOfWordsInSentence;
						#endif

					}
				}

				#ifdef GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
				if(invalidSentenceFoundIsolatedFullStop)
				{
					//currentSentence->firstFeatureInList = new GIAfeature();	//shouldn't be required, it will be overwritten automatically
				}
				else
				{
				#endif
					if(createNewSentences)
					{
						GIAsentence* newSentence = new GIAsentence();
						newSentence->previous = currentSentence;
						currentSentence->next = newSentence;
					}
					currentSentence = currentSentence->next;

				#ifdef GIA_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS
				}
				#endif

				currentTagInSentences = currentTagInSentences->nextTag;
			}
		}

		GIAstanfordCoreNLPcoreference* firstCoreferenceInList = firstSentenceInList->firstCoreferenceInList;
		GIAstanfordCoreNLPcoreference* currentCoreferenceInList = firstCoreferenceInList;

		if(parseFeatures)
		{
			currentTagInDocument = currentTagInDocument->nextTag;
			bool hasCoreferences = false;
			XMLparserTag* firstTagInCoreferences = NULL;
			if(currentTagInDocument->name == StanfordCoreNLP_XML_TAG_coreferences)	//added 2k6c (as Stanford CoreNLP only adds coreferences tag if there is one or more coreferences)
			{
				firstTagInCoreferences = XMLparserClass.parseTagDownALevel(currentTagInDocument, StanfordCoreNLP_XML_TAG_coreferences, &hasCoreferences);
			}
			if(hasCoreferences)
			{
				XMLparserTag* currentTagInnCoreferences = firstTagInCoreferences;
				while(currentTagInnCoreferences->nextTag != NULL)
				{
					XMLparserTag* firstTagInCoreference = XMLparserClass.parseTagDownALevel(currentTagInnCoreferences, StanfordCoreNLP_XML_TAG_coreference, &result);
					XMLparserTag* currentTagInnCoreference = firstTagInCoreference;

					GIAstanfordCoreNLPmention* firstMentionInList = currentCoreferenceInList->firstMentionInList;
					GIAstanfordCoreNLPmention* currentMentionInList = firstMentionInList;
					while(currentTagInnCoreference->nextTag != NULL)
					{
						if(currentTagInnCoreference->firstAttribute->name == StanfordCoreNLP_XML_ATTRIBUTE_representative)
						{
							string representativeString = currentTagInnCoreference->firstAttribute->value;
							if(representativeString == "true")
							{
								currentMentionInList->representative = true;
							}
						}

						XMLparserTag* firstTagInMention = XMLparserClass.parseTagDownALevel(currentTagInnCoreference, StanfordCoreNLP_XML_TAG_mention, &result);
						XMLparserTag* currentTagInMention = firstTagInMention;
						while(currentTagInMention->nextTag != NULL)
						{
							if(currentTagInMention->name == StanfordCoreNLP_XML_TAG_sentence)
							{
								int TagValue = SHAREDvars.convertStringToInt(currentTagInMention->value);
								currentMentionInList->sentence = TagValue;
							}
							else if(currentTagInMention->name == StanfordCoreNLP_XML_TAG_start)
							{
								int TagValue = SHAREDvars.convertStringToInt(currentTagInMention->value);
								currentMentionInList->start = TagValue;
							}
							else if(currentTagInMention->name == StanfordCoreNLP_XML_TAG_end)
							{
								int TagValue = SHAREDvars.convertStringToInt(currentTagInMention->value);
								currentMentionInList->end = TagValue;
							}
							else if(currentTagInMention->name == StanfordCoreNLP_XML_TAG_head)
							{
								int TagValue = SHAREDvars.convertStringToInt(currentTagInMention->value);
								currentMentionInList->head = TagValue;
							}

							currentTagInMention = currentTagInMention->nextTag;
						}

						GIAstanfordCoreNLPmention* newMention = new GIAstanfordCoreNLPmention();
						currentMentionInList->next = newMention;
						currentMentionInList = currentMentionInList->next;

						currentTagInnCoreference = currentTagInnCoreference->nextTag;
					}

					GIAstanfordCoreNLPcoreference* newCoreference = new GIAstanfordCoreNLPcoreference();
					currentCoreferenceInList->next = newCoreference;
					currentCoreferenceInList = currentCoreferenceInList->next;

					currentTagInnCoreferences = currentTagInnCoreferences->nextTag;
				}
			}
		}

	}

	delete firstTagInXMLFile;

	return result;
}

#endif

#ifdef GIA_STANFORD_PARSER

//http://rosettacode.org/wiki/Count_occurrences_of_a_substring#C.2B.2B
// returns count of non-overlapping occurrences of 'sub' in 'str'
int GIAnlpClass::countSubstring(const std::string& str, const std::string& sub)
{
    if (sub.length() == 0) return 0;
    int count = 0;
    for (size_t offset = str.find(sub); offset != CPP_STRING_FIND_RESULT_FAIL_VALUE;
	 offset = str.find(sub, offset + sub.length()))
    {
        ++count;
    }
    return count;
}


bool GIAnlpClass::parseStanfordParserFile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const bool createNewSentences, const bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	bool parseGIA2file = false;
	if(!parseStanfordParserFile(inputTextNLPrelationXMLfileName, isQuery, firstSentenceInList, createNewSentences, parseGIA2file, onlyParseIfCorpusLookupFailed))
	{
		result = false;
	}
	return result;
}

bool GIAnlpClass::parseStanfordParserFile(const string inputTextNLPrelationXMLfileName, const bool isQuery, GIAsentence* firstSentenceInList, const bool createNewSentences, const bool parseGIA2file, const bool onlyParseIfCorpusLookupFailed)
{
	bool result = true;

	GIAsentence* currentSentence = firstSentenceInList;

	string currentStanfordParserOutputParagraphString = "";

	bool parsingWordsAndTags = true;
	bool parsingPenn = false;
	bool parsingTypedDependencies = false;	//parse tree is first set in list

	ifstream parseFileObject(inputTextNLPrelationXMLfileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
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
								if(createNewSentences)
								{
									GIAsentence* newSentence = new GIAsentence();
									newSentence->previous = currentSentence;
									currentSentence->next = newSentence;
								}
								currentSentence = currentSentence->next;
							}
						}
						#endif

						if((!parseGIA2file && (!onlyParseIfCorpusLookupFailed || !(currentSentence->semanticParserSuccessful))) || (parseGIA2file && createNewSentences))	//added createNewSentences condition GIA 2d1a [for generateAllPermutationsFromSemanticRelationsFile()]
						{
							int numberOfWordsInSentence = 0;
							if(!parseGIA2file && createNewSentences)
							{
								cout << "parseStanfordParserFile{} error: (!parseGIA2file && createNewSentences); Stanford Parser output cannot be used to create new sentences as it contains no lemmas" << endl;
							}
							bool createFeaturesGIA2only = createNewSentences;	//NB words and POS tags cannot be ordinarily used to create features as they contain word orig not lemmas (this option is only supported for GIA2)
							GIAnlpParser.GIATHparseStanfordParseWordsAndPOStagsText(&currentStanfordParserOutputParagraphString, currentSentence, &numberOfWordsInSentence, createFeaturesGIA2only);
							if(createNewSentences)
							{
								#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE
								currentSentence->numberOfWordsInSentence = numberOfWordsInSentence;
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

						if(!onlyParseIfCorpusLookupFailed || !(currentSentence->semanticParserSuccessful))
						{
							#ifdef GIA_RECORD_MAXIMUM_NUMBER_OF_WORDS_IN_SENTENCE_OR_MAX_FEATURE_INDEX
							int numberOfWordsInSentence = currentSentence->numberOfWordsInSentence;
							#else
							int numberOfWordsInSentence = 0;
							#endif
							bool featuresNotPreviouslyFilled = createNewSentences;
							GIAnlpParser.GIATHparseStanfordParserRelationsText(&currentStanfordParserOutputParagraphString, currentSentence, &numberOfWordsInSentence, featuresNotPreviouslyFilled, parseGIA2file, false);
							if(createNewSentences)
							{
								GIAsentence* newSentence = new GIAsentence();
								newSentence->previous = currentSentence;
								currentSentence->next = newSentence;
								#ifdef GIA_RECORD_MAX_FEATURE_INDEX
								currentSentence->numberOfWordsInSentence = numberOfWordsInSentence;
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


	return result;
}
#endif




#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
void GIAnlpClass::outputInternalRelationsInRelexFormat(const string* nameOfRelexCompactFormatCFFfile, const string* originalInputFileName, GIAsentence* firstSentenceInList, const int NLPdependencyRelationsParser, const int NLPfeatureParser, const string NLPexeFolderArray[])
{
	bool result = true;

	string parserTagValue = "";
	parserTagValue = parserTagValue + "NLP Dependency Relations Parser: " + NLPexeFolderArray[NLPdependencyRelationsParser] + " | NLP GIAfeature Tag Parser: " + NLPexeFolderArray[NLPfeatureParser];

	struct tm* current;
	time_t now;
	time(&now);
	current = localtime(&now);
	char timeAndDateString[100];
	sprintf(timeAndDateString, "%i-%.2i-%i %i:%i:%i +1000", (current->tm_year + TM_STRUCT_YEAR_OFFSET), (current->tm_mon + 1), current->tm_mday, current->tm_hour, current->tm_min, current->tm_sec);

 	XMLparserTag* firstTagInXMLFile = new XMLparserTag();

	XMLparserTag* currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = Relex_CFF_XML_TAG_nlparse;
	XMLparserTag* newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;
	XMLparserAttribute* currentAttribute = currentTagL0->firstAttribute;
	currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_xmlns;
	currentAttribute->value = Relex_CFF_XML_ATTRIBUTE_xmlns_DEFAULT_VALUE;
	XMLparserAttribute* newAttribute = new XMLparserAttribute();
	currentAttribute->nextAttribute = newAttribute;

	XMLparserTag* firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag* currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag* newTag1 = new XMLparserTag();
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
	newAttribute = new XMLparserAttribute();
	currentAttribute->nextAttribute = newAttribute;
	currentTagL1 = currentTagL1->nextTag;
	newTag1 = new XMLparserTag();
	currentTagL1->nextTag = newTag1;

	XMLparserTag* firstTagL1b = currentTagL1;
	XMLparserTag* currentTagL1b = firstTagL1b;
	GIAsentence* currentSentence = firstSentenceInList;
	while(currentSentence->next != NULL)
	{
		string sentenceIndexString = SHAREDvars.convertIntToString(currentSentence->sentenceIndex);

		currentTagL1b->name = Relex_CFF_XML_TAG_sentence;
		XMLparserTag* firstTagL2 = new XMLparserTag();
		currentTagL1b->firstLowerLevelTag = firstTagL2;
		currentAttribute = currentTagL1b->firstAttribute;
		currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_index;
		currentAttribute->value = sentenceIndexString;
		newAttribute = new XMLparserAttribute();
		currentAttribute->nextAttribute = newAttribute;
		currentAttribute = currentAttribute->nextAttribute;
		currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_parses;
		currentAttribute->value = "1";
		newAttribute = new XMLparserAttribute();
		currentAttribute->nextAttribute = newAttribute;

		XMLparserTag* currentTagL2 = firstTagL2;
		currentTagL2->name = Relex_CFF_XML_TAG_parse;
		XMLparserTag* firstTagL3 = new XMLparserTag();
		currentTagL2->firstLowerLevelTag = firstTagL3;
		currentAttribute = currentTagL2->firstAttribute;
		currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_id;
		currentAttribute->value = "1";
		newAttribute = new XMLparserAttribute();
		currentAttribute->nextAttribute = newAttribute;

		XMLparserTag* currentTagL3 = firstTagL3;
		currentTagL3->name = Relex_CFF_XML_TAG_features;
		currentTagL3->value = GIAnlpParser.generateRelexCFFfeatureTagContent(currentSentence->firstFeatureInList);	//fill in currentTagL3->value with features
		XMLparserTag* newTag3 = new XMLparserTag();
		currentTagL3->nextTag = newTag3;
		currentTagL3= currentTagL3->nextTag;

		currentTagL3->name = Relex_CFF_XML_TAG_relations;
		currentTagL3->value = GIAnlpParser.generateRelexCFFrelationTagContent(currentSentence->firstRelationInList);	//fill in currentTagL3->value with relations
		newTag3 = new XMLparserTag();
		currentTagL3->nextTag = newTag3;
		currentTagL3= currentTagL3->nextTag;

		XMLparserTag* newTag2 = new XMLparserTag();
		currentTagL2->nextTag = newTag2;

		XMLparserTag* newTag1b = new XMLparserTag();
		currentTagL1b->nextTag = newTag1b;
		currentTagL1b =	currentTagL1b->nextTag;
		currentSentence = currentSentence->next;
	}

 	if(!XMLparserClass.writeXMLfile(*nameOfRelexCompactFormatCFFfile, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;

}

#endif


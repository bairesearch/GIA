/*******************************************************************************
 *
 * File Name: GIAnlp.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1k5a 14-May-2012
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

#define CHAR_NEW_LINE '\n'

#include "GIAnlp.h"
#include "XMLParserClass.h"
#include "XMLrulesClass.h"
#include "GIAParser.h"
#include "GIATranslatorOperations.h" //required for featureNERTypeArray only

void executeNLPparser(string inputTextPlainTXTFileName, string inputTextNLPrelationXMLFileName, int NLPParser, string NLPexeFolderArray[])
{
	/*
	int inputTextNLPParsedXMLFileNameLength = inputTextNLPrelationXMLFileName.length();
	int inputTextNLPParsedXMLFileNameIndexOfExtension = inputTextNLPrelationXMLFileName.rfind(".");		//find last occurance of "."
	int inputTextNLPParsedXMLFileNameExtensionLength = inputTextNLPParsedXMLFileNameLength-inputTextNLPParsedXMLFileNameIndexOfExtension;
	string inputTextNLPParsedXMLExtensionName = inputTextNLPrelationXMLFileName.substr(inputTextNLPParsedXMLFileNameIndexOfExtension, inputTextNLPParsedXMLFileNameExtensionLength);
	string inputTextNLPParsedXMLFileNameWithout
	inputTextNLPrelationXMLFileName.substr(inputTextNLPParsedXMLFileNameIndexOfExtension, inputTextNLPParsedXMLFileNameLength-inputTextNLPParsedXMLFileNameExtensionLength);	
	*/
	string inputTextNLPParsedXMLFileNameTemp = inputTextPlainTXTFileName + StanfordCoreNLPdefaultOutputFileExtensionAppend;
	
	bool result = true;
	string NLPParserExecutableName = "";
	string NLPexeFolder = "";
	#ifdef GIA_USE_RELEX
	if(NLPParser == GIA_NLP_PARSER_RELEX)
	{
		NLPParserExecutableName = GIA_RELEX_EXECUTABLE_NAME;
		NLPexeFolder = NLPexeFolderArray[GIA_NLP_PARSER_RELEX];
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		NLPParserExecutableName = GIA_STANFORD_NLP_EXECUTABLE_NAME;
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
	string executeRelexCommand = "";	
	executeRelexCommand = executeRelexCommand + NLPexeFolder + "/" + NLPParserExecutableName + " " + inputTextPlainTXTFileName + " " + inputTextNLPrelationXMLFileName + " " + workingFolderCharStar + " " + tempFolderCharStar + " " + StanfordCoreNLPdefaultOutputFileExtensionAppend;
	
	#ifdef LINUX
	chdir(NLPexeFolder.c_str());						
	#else
	::SetCurrentDirectory(NLPexeFolder.c_str());
	#endif	

	#ifndef GIA_COMPILE_FOR_BAI_APP_SERVER_RELEASE
	cout << "system(" << executeRelexCommand << ");" << endl;
	#endif
	system(executeRelexCommand.c_str());

	#ifdef LINUX
	chdir(tempFolderCharStar);						
	#else
	::SetCurrentDirectory(tempFolderCharStar);
	#endif	

	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		#ifdef LINUX
			#define SYSTEM_MOVE_COMMAND "mv"
		#else
			#define SYSTEM_MOVE_COMMAND "move"
		#endif
		
		string commandCopyTemporaryFileToRealFile = "";
		commandCopyTemporaryFileToRealFile = commandCopyTemporaryFileToRealFile + SYSTEM_MOVE_COMMAND + " " + inputTextNLPParsedXMLFileNameTemp + " " + inputTextNLPrelationXMLFileName;
		system(commandCopyTemporaryFileToRealFile.c_str());
	
		#ifdef LINUX
		//this is required due to a bug in StanfordNLPcore in Linux, where it produces a Dos file instead of a Unix file (new lines identified by carrage return..)
		string commandDos2Unix = "";
		commandDos2Unix = commandDos2Unix + "dos2unix " + inputTextNLPrelationXMLFileName;
		system(commandDos2Unix.c_str());
		#endif
		
	}
	#endif		
}


bool parseNLPParserFile(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, bool isQuery, Paragraph * firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode)
{
	bool result = true;

	bool createNewSentences = true;
	
	//cout << "inputTextNLPrelationXMLFileName = " << inputTextNLPrelationXMLFileName << endl;
	//cout << "inputTextNLPfeatureXMLFileName = " << inputTextNLPfeatureXMLFileName << endl;
	//cout << "NLPfeatureParser = " << NLPfeatureParser << endl;
	//cout << "NLPdependencyRelationsParser = " << NLPdependencyRelationsParser << endl;
	//cout << "NLPrelexCompatibilityMode = " << NLPrelexCompatibilityMode << endl;
	
	//Parse Features
	#ifdef GIA_USE_RELEX
	if(NLPfeatureParser == GIA_NLP_PARSER_RELEX)
	{
		createNewSentences = false;
		if(!parseRelexFile(inputTextNLPfeatureXMLFileName, isQuery, firstParagraphInList, false, true, NLPrelexCompatibilityMode, true))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		createNewSentences = false;
		if(!parseStanfordCoreNLPFile(inputTextNLPfeatureXMLFileName, isQuery, firstParagraphInList, false, true, true))
		{
			result = false;
		}
	}
	#endif
	#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
	#ifdef GIA_USE_STANFORD_PARSER
	if(NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		if(isQuery)
		{
			cout << "error: parseNLPParserFile() does not support queries at present with (NLPfeatureParser == GIA_NLP_PARSER_STANFORD_PARSER). Set feature parser to RelEx or Stanford Core NLP for queries" << endl;
			exit(0); 			
		}
	}	
	#endif	
	#endif
		
			
	//Parse Relations
	#ifdef GIA_USE_RELEX
	if(NLPdependencyRelationsParser == GIA_NLP_PARSER_RELEX)
	{
		if(!parseRelexFile(inputTextNLPrelationXMLFileName, isQuery, firstParagraphInList, true, false, NLPrelexCompatibilityMode, createNewSentences))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPdependencyRelationsParser == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		if(!parseStanfordCoreNLPFile(inputTextNLPrelationXMLFileName, isQuery, firstParagraphInList, true, false, createNewSentences))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_USE_STANFORD_PARSER
	if(NLPdependencyRelationsParser == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		if(!parseStanfordParserFile(inputTextNLPrelationXMLFileName, isQuery, firstParagraphInList, createNewSentences))
		{
			result = false;
		}
	}	
	#endif		
				
	return result;
}

		
#ifdef GIA_USE_RELEX
bool parseRelexFile(string inputTextNLPrelationXMLFileName, bool isQuery, Paragraph * firstParagraphInList, bool parseRelations, bool parseFeatures, bool NLPrelexCompatibilityMode, bool createNewSentences)
{
	bool result = true;
	
	Paragraph * currentParagraph = firstParagraphInList;
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;	
	Sentence * currentSentence = firstSentenceInList;

	XMLParserTag * firstTagInXMLFile = new XMLParserTag();
	readXMLFile(inputTextNLPrelationXMLFileName, firstTagInXMLFile);

	#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS	
	XMLParserTag * currentTag2 = firstTagInXMLFile;
	currentTag2 = parseTagDownALevel(currentTag2, Relex_CFF_XML_TAG_nlparse, &result);
	if(result)
	{
		//now for every sentence;
		while(currentTag2->nextTag != NULL)
		{
			if(currentTag2->name == Relex_CFF_XML_TAG_paragraph)
			{
				XMLParserTag * currentTag = currentTag2;
				currentTag = parseTagDownALevel(currentTag, Relex_CFF_XML_TAG_paragraph, &result);
	#else
				XMLParserTag * currentTag = firstTagInXMLFile;
				currentTag = parseTagDownALevel(currentTag, Relex_CFF_XML_TAG_nlparse, &result);
	#endif
				
				if(result)
				{
					//now for every sentence;
					while(currentTag->nextTag != NULL)
					{
						if(currentTag->name == Relex_CFF_XML_TAG_sentence)
						{
							//locate and record sentence index
							XMLParserAttribute * firstAttributeInSentenceTag = currentTag->firstAttribute;
							XMLParserAttribute * currentAttributeInSentenceTag = firstAttributeInSentenceTag;
							bool foundSentenceIndexAttribute = false;
							while(currentAttributeInSentenceTag->nextAttribute != NULL)
							{
								if(currentAttributeInSentenceTag->name != Relex_CFF_XML_ATTRIBUTE_index)
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
							
							XMLParserTag * firstTagInSentence;
							firstTagInSentence = parseTagDownALevel(currentTag, Relex_CFF_XML_TAG_sentence, &result);	
							XMLParserTag * firstTagInFirstParse;
							firstTagInFirstParse = parseTagDownALevel(firstTagInSentence, Relex_CFF_XML_TAG_parse, &result);

							if(result)
							{
								XMLParserTag * currentTagInParse = firstTagInFirstParse;
								while(currentTagInParse->nextTag != NULL)
								{

									if(currentTagInParse->name == Relex_CFF_XML_TAG_relations)
									{
										if(parseRelations)
										{
											//cout << "currentTagInParse->value = " << currentTagInParse->value << endl;
											int maxNumberOfWordsInSentence = 0;
											GIATHparseRelexRelationsText(&(currentTagInParse->value), currentSentence->firstRelationInList, &maxNumberOfWordsInSentence, NLPrelexCompatibilityMode);
											currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;
										}
									}
									else if(currentTagInParse->name == Relex_CFF_XML_TAG_features)
									{
										if(parseFeatures)
										{
											//cout << "currentTagInParse->value = " << currentTagInParse->value << endl;
											GIATHparseFeaturesText(&(currentTagInParse->value), currentSentence->firstFeatureInList, &(currentSentence->isQuestion));

											if(isQuery)
											{
												if(!(currentSentence->isQuestion))
												{
													cout << "error: GIA query is not a question" << endl;
													exit(0); 
												}
											}
										}						
										//cout << "fini" << endl;
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
	
	return result;
}
#endif


#ifdef GIA_USE_STANFORD_CORENLP
bool parseStanfordCoreNLPFile(string inputTextNLPrelationXMLFileName, bool isQuery, Paragraph * firstParagraphInList, bool parseRelations, bool parseFeatures, bool createNewSentences)
{
	bool result = true;
	
	Paragraph * currentParagraph = firstParagraphInList;
	Paragraph * newParagraph = new Paragraph();		
	newParagraph->previous = currentParagraph;
	currentParagraph->next = newParagraph;
					
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;	
	Sentence * currentSentence = firstSentenceInList;

	XMLParserTag * firstTagInXMLFile = new XMLParserTag();
	readXMLFile(inputTextNLPrelationXMLFileName, firstTagInXMLFile);
	
	XMLParserTag * firstTagInRoot = parseTagDownALevel(firstTagInXMLFile, StanfordCoreNLP_XML_TAG_root, &result);
	XMLParserTag * firstTagInDocument = parseTagDownALevel(firstTagInRoot, StanfordCoreNLP_XML_TAG_document, &result);
	XMLParserTag * currentTagInDocument = firstTagInDocument;
	
	XMLParserTag * firstTagInSentences = parseTagDownALevel(currentTagInDocument, StanfordCoreNLP_XML_TAG_sentences, &result);
	XMLParserTag * currentTagInSentences = firstTagInSentences;
	while(currentTagInSentences->nextTag != NULL)
	{	
	
		string sentenceIndexString = currentTagInSentences->firstAttribute->value;
		currentSentence->sentenceIndex = atoi(sentenceIndexString.c_str());
	
		XMLParserTag * firstTagInSentence = parseTagDownALevel(currentTagInSentences, StanfordCoreNLP_XML_TAG_sentence, &result);
		XMLParserTag * currentTagInSentence = firstTagInSentence;
		
		Feature * firstFeatureInList = currentSentence->firstFeatureInList;
		Feature * currentFeatureInList = firstFeatureInList; 
					
		if(parseFeatures)
		{
			XMLParserTag * firstTagInTokens = parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_XML_TAG_tokens, &result);
			XMLParserTag * currentTagInTokens = firstTagInTokens;
			bool isQuestion = false;
			
			while(currentTagInTokens->nextTag != NULL)
			{
				string entityIndexString = currentTagInTokens->firstAttribute->value;
				currentFeatureInList->entityIndex = atoi(entityIndexString.c_str());

				XMLParserTag * firstTagInToken = parseTagDownALevel(currentTagInTokens, StanfordCoreNLP_XML_TAG_token, &result);
				XMLParserTag * currentTagInToken = firstTagInToken;
				while(currentTagInToken->nextTag != NULL)
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
					}
					else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_lemma)
					{
						string TagValue = currentTagInToken->value;
						string lemma = TagValue;
						#ifdef GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PRONOUN_LEMMA_CASE_ASSIGNMENT_BUG_MAKE_ALL_LEMMAS_LOWER_CASE
						char firstCharacterOfLemma = lemma[0]; 
						firstCharacterOfLemma = tolower(firstCharacterOfLemma);
						lemma[0] = firstCharacterOfLemma;
						#endif
						currentFeatureInList->lemma = lemma;
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
						for(int i=0; i<FEATURE_NER_NUMBER_TYPES; i++)
						{
							if(TagValue == featureNERTypeArray[i])
							{
								currentNER = i;
								/*
								#ifdef GIA_WORDNET_DEBUG
								cout << "nerType currentFeatureIndex = " << currentNER << endl;			
								#endif
								*/
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
					currentTagInToken = currentTagInToken->nextTag;													
				}

				Feature * newFeature = new Feature();
				newFeature->previous = currentFeatureInList;						
				currentFeatureInList->next = newFeature;
				currentFeatureInList = currentFeatureInList->next;

				currentTagInTokens = currentTagInTokens->nextTag;
			}
			#ifndef GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
			#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
			if(isQuery)
			{
				if(isQuestion)
				{
					//cout << "Stanford CoreNLP found question" << endl;
					currentSentence->isQuestion = true;
				}
				else
				{
					cout << "error: GIA query is not a question" << endl;
					exit(0); 
				}
			}
			#endif
			#endif
														
		}

		currentTagInSentence = currentTagInSentence->nextTag;
		XMLParserTag * firstTagInParse = parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_XML_TAG_parse, &result);
		
		if(parseRelations)
		{		
			for(int dependenciesSetIndex=0; dependenciesSetIndex< StanfordCoreNLP_numberOfDependencySetsPerSentence; dependenciesSetIndex++)
			{
				Relation * firstRelationInList = currentSentence->firstRelationInList;
				Relation * currentRelationInList = firstRelationInList; 

				currentTagInSentence = currentTagInSentence->nextTag;
				if(dependenciesSetIndex == StanfordCoreNLP_dependencySet_usedByGIA)
				{
					XMLParserTag * firstTagInDependencies = parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_relationSetNameArray[dependenciesSetIndex], &result);	
					if(firstTagInDependencies != NULL)
					{	
						XMLParserTag * currentTagInDependencies = firstTagInDependencies;
						while(currentTagInDependencies->nextTag != NULL)
						{
							XMLParserTag * firstTagInDep = parseTagDownALevel(currentTagInDependencies, StanfordCoreNLP_XML_TAG_dep, &result);		
							XMLParserTag * governerTagInDep = firstTagInDep; 
							XMLParserTag * dependentTagInDep = firstTagInDep->nextTag;

							//cout << "currentTagInDependencies->firstAttribute->value = " << currentTagInDependencies->firstAttribute->value << endl;
							string relationTypeRelexStandard = convertStanfordRelationToRelex(&(currentTagInDependencies->firstAttribute->value));
							currentRelationInList->relationType = relationTypeRelexStandard;

							string relationGovernorIndexString = governerTagInDep->firstAttribute->value;
							string relationDependentIndexString = dependentTagInDep->firstAttribute->value;
							currentRelationInList->relationGovernorIndex = atoi(relationGovernorIndexString.c_str());						
							currentRelationInList->relationDependentIndex = atoi(relationDependentIndexString.c_str());

							/*
							//don't use these, use lemmas instead (as per Stanford Core NLP/Relex dependency relation definitions)
							currentRelationInList->relationGovernor = governerTagInDep->value;
							currentRelationInList->relationDependent = dependentTagInDep->value;						
							*/				
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

							Relation * newRelation = new Relation();
							currentRelationInList->next = newRelation;
							currentRelationInList = currentRelationInList->next;

							currentTagInDependencies = currentTagInDependencies->nextTag;													
						}	
					}	
				}	

			}
		}

		if(createNewSentences)
		{
			Sentence * newSentence = new Sentence();
			newSentence->previous = currentSentence;						
			currentSentence->next = newSentence;
		}
		currentSentence = currentSentence->next;
									
		currentTagInSentences = currentTagInSentences->nextTag;
	}

	StanfordCoreNLPCoreference * firstCoreferenceInList = firstSentenceInList->firstCoreferenceInList;	
	StanfordCoreNLPCoreference * currentCoreferenceInList = firstCoreferenceInList;
	
	if(parseFeatures)
	{
		currentTagInDocument = currentTagInDocument->nextTag;
		bool hasCoreferences = false;
		XMLParserTag * firstTagInCoreferences = parseTagDownALevel(currentTagInDocument, StanfordCoreNLP_XML_TAG_coreferences, &hasCoreferences);
		if(hasCoreferences)
		{
			XMLParserTag * currentTagInnCoreferences = firstTagInCoreferences;
			while(currentTagInnCoreferences->nextTag != NULL)
			{	
				XMLParserTag * firstTagInCoreference = parseTagDownALevel(currentTagInnCoreferences, StanfordCoreNLP_XML_TAG_coreference, &result);
				XMLParserTag * currentTagInnCoreference = firstTagInCoreference;

				StanfordCoreNLPMention * firstMentionInList = currentCoreferenceInList->firstMentionInList;
				StanfordCoreNLPMention * currentMentionInList = firstMentionInList;
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

					XMLParserTag * firstTagInMention = parseTagDownALevel(currentTagInnCoreference, StanfordCoreNLP_XML_TAG_mention, &result);
					XMLParserTag * currentTagInMention = firstTagInMention;
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

					StanfordCoreNLPMention * newMention = new StanfordCoreNLPMention();
					currentMentionInList->next = newMention;
					currentMentionInList = currentMentionInList->next;		

					currentTagInnCoreference = currentTagInnCoreference->nextTag;
				}	

				StanfordCoreNLPCoreference * newCoreference = new StanfordCoreNLPCoreference();
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
		StanfordCoreNLPMention * currentMentionInList = currentCoreferenceInList->firstMentionInList;
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
			
	//cout << "exiting prematurely; GIA_NLP_DEBUG complete" << endl; 
	//exit(0);
	#endif				
					
	return result;
}

#endif

#ifdef GIA_USE_STANFORD_PARSER
bool parseStanfordParserFile(string inputTextNLPrelationXMLFileName, bool isQuery, Paragraph * firstParagraphInList, bool createNewSentences)
{
	bool result = true;
	
	Paragraph * currentParagraph = firstParagraphInList;
	Paragraph * newParagraph = new Paragraph();		
	newParagraph->previous = currentParagraph;
	currentParagraph->next = newParagraph;
		
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;	
	Sentence * currentSentence = firstSentenceInList;

	string currentDependencyRelationSetString = "";
	
	bool parsingDependencyRelations = false;	//parse tree is first set in list
	
	ifstream parseFileObject(inputTextNLPrelationXMLFileName.c_str());
	if(!parseFileObject.rdbuf( )->is_open( ))
	{
		//xml file does not exist in current directory.
		cout << "Error: Stanford Parser Output File does not exist in current directory: " << inputTextNLPrelationXMLFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		bool newLineDetected = false;
		while(parseFileObject.get(currentToken))
		{
			if(currentToken == CHAR_NEW_LINE)
			{
				if(newLineDetected)
				{
					newLineDetected = false;
					//two consecutive new line characters detected..
					if(parsingDependencyRelations)
					{
						int maxNumberOfWordsInSentence = 0;
						bool featuresNotPreviouslyFilled = createNewSentences;
						GIATHparseStanfordParserRelationsText(&currentDependencyRelationSetString, currentSentence, &maxNumberOfWordsInSentence, featuresNotPreviouslyFilled);
						//cout << "currentDependencyRelationSetString = " << currentDependencyRelationSetString << endl;
						currentDependencyRelationSetString = "";
						if(createNewSentences)
						{
							Sentence * newSentence = new Sentence();
							newSentence->previous = currentSentence;						
							currentSentence->next = newSentence;
						}
						currentSentence = currentSentence->next;
						
						parsingDependencyRelations = false;					
					}
					else
					{
						parsingDependencyRelations = true;
					}
				}
				else
				{
					if(parsingDependencyRelations)
					{
						currentDependencyRelationSetString = currentDependencyRelationSetString + currentToken;
					}
					newLineDetected = true;
				}
			}
			else
			{
				newLineDetected = false;
				
				if(parsingDependencyRelations)
				{
					//parsing dependency relations
					currentDependencyRelationSetString = currentDependencyRelationSetString + currentToken;
				}
			}
		
			charCount++;
		}
	}	
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
	sprintf(timeAndDateString, "%i-%.2i-%i %i:%i:%i +1000", (current->tm_year + GIA_TM_STRUCT_YEAR_OFFSET), (current->tm_mon + 1), current->tm_mday, current->tm_hour, current->tm_min, current->tm_sec);
	
 	XMLParserTag * firstTagInXMLFile = new XMLParserTag();
	
	XMLParserTag * currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = Relex_CFF_XML_TAG_nlparse;
	XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;
	XMLParserAttribute * currentAttribute = currentTagL0->firstAttribute;
	currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_xmlns;
	currentAttribute->value = Relex_CFF_XML_ATTRIBUTE_xmlns_DEFAULT_VALUE;
	XMLParserAttribute * newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;
		
	XMLParserTag * firstTagL1 = new XMLParserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLParserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLParserTag * newTag1 = new XMLParserTag();
	currentTagL1->nextTag = newTag1;
	
	currentTagL1->name = Relex_CFF_XML_TAG_parser;
	currentTagL1->value = parserTagValue;	
	currentTagL1 = currentTagL1->nextTag;
	newTag1 = new XMLParserTag();
	currentTagL1->nextTag = newTag1;	
	
	currentTagL1->name = Relex_CFF_XML_TAG_date;
	currentTagL1->value = timeAndDateString;	
	currentTagL1 = currentTagL1->nextTag;
	newTag1 = new XMLParserTag();
	currentTagL1->nextTag = newTag1;
	
	currentTagL1->name = Relex_CFF_XML_TAG_source;
	currentTagL1->value = "";
	currentAttribute = currentTagL1->firstAttribute;
	currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_url;
	currentAttribute->value = *originalInputFileName;	
	newAttribute = new XMLParserAttribute();
	currentAttribute->nextAttribute = newAttribute;		
	currentTagL1 = currentTagL1->nextTag;
	newTag1 = new XMLParserTag();
	currentTagL1->nextTag = newTag1;	
	
	Paragraph * currentParagraph = firstParagraphInList;
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;	
	
	#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS	
	while(currentParagraph->next != NULL)
	{
		firstSentenceInList = currentParagraph->firstSentenceInList;	
		
		currentTagL1->name = Relex_CFF_XML_TAG_paragraph;
		XMLParserTag * firstTagL1b = new XMLParserTag();
		currentTagL1->firstLowerLevelTag = firstTagL1b;
	#else
		XMLParserTag * firstTagL1b = currentTagL1;
	#endif
	
		XMLParserTag * currentTagL1b = firstTagL1b;
		Sentence * currentSentence = firstSentenceInList;
		while(currentSentence->next != NULL)
		{		
			char sentenceIndexString[10];
			sprintf(sentenceIndexString, "%d", currentSentence->sentenceIndex);

			currentTagL1b->name = Relex_CFF_XML_TAG_sentence;
			XMLParserTag * firstTagL2 = new XMLParserTag();
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
			
			XMLParserTag * currentTagL2 = firstTagL2;
			currentTagL2->name = Relex_CFF_XML_TAG_parse;
			XMLParserTag * firstTagL3 = new XMLParserTag();
			currentTagL2->firstLowerLevelTag = firstTagL3;
			currentAttribute = currentTagL2->firstAttribute;
			currentAttribute->name = Relex_CFF_XML_ATTRIBUTE_id;
			currentAttribute->value = "1";					
			newAttribute = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute;		

			XMLParserTag * currentTagL3 = firstTagL3;
			currentTagL3->name = Relex_CFF_XML_TAG_features;
			currentTagL3->value = generateRelexCFFFeatureTagContent(currentSentence->firstFeatureInList);	//fill in currentTagL3->value with features
			XMLParserTag * newTag3 = new XMLParserTag();		
			currentTagL3->nextTag = newTag3;		
			currentTagL3= currentTagL3->nextTag;
			
			currentTagL3->name = Relex_CFF_XML_TAG_relations;
			currentTagL3->value = generateRelexCFFRelationTagContent(currentSentence->firstRelationInList);	//fill in currentTagL3->value with relations
			newTag3 = new XMLParserTag();		
			currentTagL3->nextTag = newTag3;		
			currentTagL3= currentTagL3->nextTag;					

			XMLParserTag * newTag2 = new XMLParserTag();		
			currentTagL2->nextTag = newTag2;		
															
			XMLParserTag * newTag1b = new XMLParserTag();		
			currentTagL1b->nextTag = newTag1b;
			currentTagL1b =	currentTagL1b->nextTag;
			currentSentence = currentSentence->next;
		}
	

			
	#ifdef GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS
		newTag1 = new XMLParserTag();		
		currentTagL1->nextTag = newTag1;
		currentTagL1 = currentTagL1->nextTag; 
		currentParagraph = currentParagraph->next;
	}
	#endif	
	
 	if(!writeXMLFile(*nameOfRelexCompactFormatCFFfile, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;
			
}

#endif


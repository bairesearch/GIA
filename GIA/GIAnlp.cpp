/*******************************************************************************
 *
 * File Name: GIAnlp.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1i6a 4-Apr-2012
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
#include "XMLParserClass.h"
#include "XMLrulesClass.h"
#include "GIAParser.h"


void executeNLPparser(string inputTextPlainTXTFileName, string inputTextNLPParsedXMLFileName, int NLPparserType)
{
	/*
	int inputTextNLPParsedXMLFileNameLength = inputTextNLPParsedXMLFileName.length();
	int inputTextNLPParsedXMLFileNameIndexOfExtension = inputTextNLPParsedXMLFileName.rfind(".");		//find last occurance of "."
	int inputTextNLPParsedXMLFileNameExtensionLength = inputTextNLPParsedXMLFileNameLength-inputTextNLPParsedXMLFileNameIndexOfExtension;
	string inputTextNLPParsedXMLExtensionName = inputTextNLPParsedXMLFileName.substr(inputTextNLPParsedXMLFileNameIndexOfExtension, inputTextNLPParsedXMLFileNameExtensionLength);
	string inputTextNLPParsedXMLFileNameWithout
	inputTextNLPParsedXMLFileName.substr(inputTextNLPParsedXMLFileNameIndexOfExtension, inputTextNLPParsedXMLFileNameLength-inputTextNLPParsedXMLFileNameExtensionLength);	
	*/
	string inputTextNLPParsedXMLFileNameTemp = inputTextPlainTXTFileName + StanfordCoreNLPdefaultOutputFileExtensionAppend;
	
	bool result = true;
	string NLPParserExecutableName = "";
	#ifdef GIA_USE_RELEX
	if(NLPparserType == GIA_NLP_PARSER_RELEX)
	{
		NLPParserExecutableName = GIA_RELEX_EXECUTABLE_NAME;
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPparserType == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		NLPParserExecutableName = GIA_STANFORD_NLP_EXECUTABLE_NAME;
	}
	#endif
	#ifdef GIA_USE_STANFORD_PARSER
	if(NLPparserType == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		NLPParserExecutableName = GIA_STANFORD_PARSER_EXECUTABLE_NAME;
	}
	#endif
	
	//execute NLP parser on plain text
	string executeRelexCommand = "";	
	executeRelexCommand = executeRelexCommand + exeFolderCharStar + "/" + NLPParserExecutableName + " " + inputTextPlainTXTFileName + " " + inputTextNLPParsedXMLFileName + " " + workingFolderCharStar + " " + tempFolderCharStar + " " + StanfordCoreNLPdefaultOutputFileExtensionAppend;

	#ifdef LINUX
	chdir(exeFolderCharStar);						
	#else
	::SetCurrentDirectory(exeFolderCharStar);
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
	if(NLPparserType == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		#ifdef LINUX
			#define SYSTEM_MOVE_COMMAND "mv"
		#else
			#define SYSTEM_MOVE_COMMAND "move"
		#endif
		
		string commandCopyTemporaryFileToRealFile = "";
		commandCopyTemporaryFileToRealFile = commandCopyTemporaryFileToRealFile + SYSTEM_MOVE_COMMAND + " " + inputTextNLPParsedXMLFileNameTemp + " " + inputTextNLPParsedXMLFileName;
		system(commandCopyTemporaryFileToRealFile.c_str());
	
		#ifdef LINUX
		//this is required due to a bug in StanfordNLPcore in Linux, where it produces a Dos file instead of a Unix file (new lines identified by carrage return..)
		string commandDos2Unix = "";
		commandDos2Unix = commandDos2Unix + "dos2unix " + inputTextNLPParsedXMLFileName;
		system(commandDos2Unix.c_str());
		#endif
	}
	#endif		
}


bool parseNLPParserFile(string inputTextNLPParsedXMLFileName, bool isQuery, Paragraph * firstParagraphInList, int NLPparserType)
{
	bool result = true;
	#ifdef GIA_USE_RELEX
	if(NLPparserType == GIA_NLP_PARSER_RELEX)
	{
		if(!parseRelexFile(inputTextNLPParsedXMLFileName, isQuery, firstParagraphInList))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	if(NLPparserType == GIA_NLP_PARSER_STANFORD_CORENLP)
	{
		if(!parseStanfordCoreNLPFile(inputTextNLPParsedXMLFileName, isQuery, firstParagraphInList))
		{
			result = false;
		}
	}
	#endif
	#ifdef GIA_USE_STANFORD_PARSER
	if(NLPparserType == GIA_NLP_PARSER_STANFORD_PARSER)
	{
		if(!parseStanfordParserFile(inputTextNLPParsedXMLFileName, isQuery, firstParagraphInList))
		{
			result = false;
		}
	}	
	#endif		
			
	return result;
}

		
#ifdef GIA_USE_RELEX
bool parseRelexFile(string inputTextNLPParsedXMLFileName, bool isQuery, Paragraph * firstParagraphInList, bool NLPrelexCompatibilityMode)
{
	bool result = true;
	
	Paragraph * currentParagraph = firstParagraphInList;
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;	
	Sentence * currentSentence = firstSentenceInList;

	XMLParserTag * firstTagInXMLFile = new XMLParserTag();
	readXMLFile(inputTextNLPParsedXMLFileName, firstTagInXMLFile);

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
										//cout << "currentTagInParse->value = " << currentTagInParse->value << endl;
										int maxNumberOfWordsInSentence = 0;
										GIATHparseRelationsText(&(currentTagInParse->value), currentSentence->firstRelationInList, &maxNumberOfWordsInSentence, NLPrelexCompatibilityMode);
										currentSentence->maxNumberOfWordsInSentence = maxNumberOfWordsInSentence;
									}
									else if(currentTagInParse->name == Relex_CFF_XML_TAG_features)
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
										//cout << "fini" << endl;
									}

									currentTagInParse = currentTagInParse->nextTag;
								}

							}

							Sentence * newSentence = new Sentence();
							newSentence->previous = currentSentence;						
							currentSentence->next = newSentence;
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
bool parseStanfordCoreNLPFile(string inputTextNLPParsedXMLFileName, bool isQuery, Paragraph * firstParagraphInList)
{
	bool result = true;
	
	Paragraph * currentParagraph = firstParagraphInList;
	Sentence * firstSentenceInList = firstParagraphInList->firstSentenceInList;	
	Sentence * currentSentence = firstSentenceInList;

	XMLParserTag * firstTagInXMLFile = new XMLParserTag();
	readXMLFile(inputTextNLPParsedXMLFileName, firstTagInXMLFile);
	
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
		XMLParserTag * firstTagInTokens = parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_XML_TAG_tokens, &result);
		XMLParserTag * currentTagInTokens = firstTagInTokens;

		Feature * firstFeatureInList = currentSentence->firstFeatureInList;
		Feature * currentFeatureInList = firstFeatureInList; 
		
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
					currentFeatureInList->POS = TagValue;
				}
				else if(currentTagInToken->name == StanfordCoreNLP_XML_TAG_NER)
				{
					string TagValue = currentTagInToken->value;
					currentFeatureInList->NER = TagValue;
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

		currentTagInSentence = currentTagInSentence->nextTag;
		XMLParserTag * firstTagInParse = parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_XML_TAG_parse, &result);
		
		for(int dependenciesSetIndex=0; dependenciesSetIndex< StanfordCoreNLP_numberOfDependencySetsPerSentence; dependenciesSetIndex++)
		{
			Relation * firstRelationInList = currentSentence->firstRelationInList;
			Relation * currentRelationInList = firstRelationInList; 
				
			currentTagInSentence = currentTagInSentence->nextTag;
			XMLParserTag * firstTagInDependencies = parseTagDownALevel(currentTagInSentence, StanfordCoreNLP_relationSetNameArray[dependenciesSetIndex], &result);	
			if(firstTagInDependencies != NULL)
			{	
				XMLParserTag * currentTagInDependencies = firstTagInDependencies;
				while(currentTagInDependencies->nextTag != NULL)
				{
					XMLParserTag * firstTagInDep = parseTagDownALevel(currentTagInDependencies, StanfordCoreNLP_XML_TAG_dep, &result);		
					XMLParserTag * governerTagInDep = firstTagInDep; 
					XMLParserTag * dependentTagInDep = firstTagInDep->nextTag;

					string relationTypeRelexStandard = convertStanfordRelationToRelex(&(currentTagInDependencies->firstAttribute->value));
					currentRelationInList->relationType = relationTypeRelexStandard;

					string relationArgumentIndexString = governerTagInDep->firstAttribute->value;
					string relationFunctionIndexString = dependentTagInDep->firstAttribute->value;
					currentRelationInList->relationArgumentIndex = atoi(relationArgumentIndexString.c_str());
					currentRelationInList->relationFunctionIndex = atoi(relationFunctionIndexString.c_str());

					/*
					//don't use these, use lemmas instead (as per Relex dependency relation definitions)
					currentRelationInList->relationArgument = governerTagInDep->value;
					currentRelationInList->relationFunction = dependentTagInDep->value;
					*/				
					currentFeatureInList = firstFeatureInList;
					for(int f=0; f<currentRelationInList->relationArgumentIndex; f++)
					{
						currentFeatureInList = currentFeatureInList->next;
					} 
					currentRelationInList->relationArgument = currentFeatureInList->lemma;
					currentFeatureInList = firstFeatureInList;
					for(int f=0; f<currentRelationInList->relationFunctionIndex; f++)
					{
						currentFeatureInList = currentFeatureInList->next;
					} 				
					currentRelationInList->relationFunction = currentFeatureInList->lemma;				

					Relation * newRelation = new Relation();
					currentRelationInList->next = newRelation;
					currentRelationInList = currentRelationInList->next;

					currentTagInDependencies = currentTagInDependencies->nextTag;													
				}	
			}		
			
		}

		Sentence * newSentence = new Sentence();
		newSentence->previous = currentSentence;						
		currentSentence->next = newSentence;
		currentSentence = currentSentence->next;
									
		currentTagInSentences = currentTagInSentences->nextTag;
	}

	StanfordCoreNLPCoreference * firstCoreferenceInList = firstSentenceInList->firstCoreferenceInList;	
	StanfordCoreNLPCoreference * currentCoreferenceInList = firstCoreferenceInList;
	
	currentTagInDocument = currentTagInDocument->nextTag;
	XMLParserTag * firstTagInCoreferences = parseTagDownALevel(currentTagInDocument, StanfordCoreNLP_XML_TAG_coreferences, &result);
	XMLParserTag * currentTagInnCoreferences = firstTagInCoreferences;
	while(currentTagInnCoreferences->nextTag != NULL)
	{	
		XMLParserTag * firstTagInCoreference = parseTagDownALevel(currentTagInnCoreferences, StanfordCoreNLP_XML_TAG_coreference, &result);
		XMLParserTag * currentTagInnCoreference = firstTagInCoreference;
		
		StanfordCoreNLPMention * firstMentionInList = currentCoreferenceInList->firstMentionInList;
		StanfordCoreNLPMention * currentMentionInList = firstMentionInList;
		while(currentTagInnCoreference->nextTag != NULL)
		{
			string representativeString = currentTagInnCoreference->firstAttribute->value;
			currentMentionInList->representative =  atoi(representativeString.c_str());
		
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
	
	#ifdef GIA_NLP_DEBUG
	currentSentence = firstParagraphInList->firstSentenceInList;
	while(currentSentence->next != NULL)
	{
		cout << "currentSentence->sentenceIndex = " << currentSentence->sentenceIndex << endl;
				
		Relation * currentRelationInList = currentSentence->firstRelationInList;
		while(currentRelationInList->next != NULL)
		{	
			cout << "currentRelationInList->relationType = " << currentRelationInList->relationType << endl;
			cout << "currentRelationInList->relationArgument = " << currentRelationInList->relationArgument << endl;
			cout << "currentRelationInList->relationArgumentIndex = " << currentRelationInList->relationArgumentIndex << endl;
			cout << "currentRelationInList->relationFunction = " << currentRelationInList->relationFunction << endl;
			cout << "currentRelationInList->relationFunctionIndex = " << currentRelationInList->relationFunctionIndex << endl;

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
			cout << "currentFeatureInList->POS = " << currentFeatureInList->POS << endl;
			cout << "currentFeatureInList->NER = " << currentFeatureInList->NER << endl;
			cout << "currentFeatureInList->NormalizedNER = " << currentFeatureInList->NormalizedNER << endl;
			cout << "currentFeatureInList->Timex = " << currentFeatureInList->Timex << endl;

			currentFeatureInList = currentFeatureInList->next;	
		}
		
		StanfordCoreNLPCoreference * currentCoreferenceInList = currentSentence->firstCoreferenceInList;
		while(currentCoreferenceInList->next != NULL)
		{	
			StanfordCoreNLPMention * currentMentionInList = currentCoreferenceInList->firstMentionInList;
			while(currentMentionInList->next != NULL)
			{	
				cout << "currentMentionInList->representative = " << currentMentionInList->representative << endl;
				cout << "currentMentionInList->sentence = " << currentMentionInList->sentence << endl;
				cout << "currentMentionInList->start = " << currentMentionInList->start << endl;
				cout << "currentMentionInList->end = " << currentMentionInList->end << endl;
				cout << "currentMentionInList->head = " << currentMentionInList->head << endl;

				currentMentionInList = currentMentionInList->next;	
			}
			currentCoreferenceInList = currentCoreferenceInList->next;	
		}
				
		currentSentence = currentSentence->next;	
	}
	cout << "exiting prematurely; GIA_NLP_DEBUG complete" << endl; 
	exit(0);
	#endif				
					
	return result;
}

#endif


#ifdef GIA_USE_STANFORD_PARSER
bool parseStanfordParserFile(string inputTextNLPParsedXMLFileName, bool isQuery, Paragraph * firstParagraphInList)
{
	cout << "isolated Stanford Parser not implemented - use Stanford CoreNLP instead." << endl;
	exit(0);
}
#endif


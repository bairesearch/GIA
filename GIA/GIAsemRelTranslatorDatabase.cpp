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
 * File Name: GIAsemRelTranslatorDatabase.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3h4a 25-April-2019
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 * Description: Semantic Relation Translator
 * /
 *******************************************************************************/


#include "GIAsemRelTranslatorDatabase.hpp"

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_SEM_REL_TRANSLATOR

static string semanticParserDatabaseFolderName;
void GIAsemRelTranslatorDatabaseClass::initialiseSemanticParserDatabase(const string newSemanticParserDatabaseFolderName)
{
	semanticParserDatabaseFolderName = newSemanticParserDatabaseFolderName;
	if(!SHAREDvars.directoryExists(&semanticParserDatabaseFolderName))
	{
		cerr << "GIAsemRelTranslatorDatabaseClass::initialiseSemanticParserDatabase{} error: semanticParserDatabaseFolderName does not exist: " << semanticParserDatabaseFolderName << endl;
		exit(EXIT_ERROR);	
	}
}

void GIAsemRelTranslatorDatabaseClass::prepareSemanticParserDatabaseForWriting()
{
	#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	prepareSemanticParserCorpusDatabaseFileTextForWriting();
	#else
	prepareSemanticParserSemanticRelationListForWriting();
	#endif
}

string GIAsemRelTranslatorDatabaseClass::semanticParserDBgenerateFolderName(const GIAfeature* firstFeatureInList)
{
	string folderName = "";

	//eg network/server/GIAsemRelTranslatorDatabase/de/no/ve/de/no/corpus.txt
	int fileType = 0;	//irrelevant
	string serverName = GIAdatabase.DBgenerateServerDatabaseName(&(GIAconnectionistNetworkPOStypeNameAbbreviationArray[firstFeatureInList->GIAsemRelTranslatorPOStype]), fileType, GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME, semanticParserDatabaseFolderName);

	GIAdatabase.DBsetCurrentDirectory(&serverName);

	const GIAfeature* currentFeatureInSentence = firstFeatureInList;
	while(currentFeatureInSentence->next != NULL)
	{
		string POStypeAbbreviationName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAsemRelTranslatorPOStype];
		#ifdef GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_USE_FLAT_FILE
		folderName = folderName + POStypeAbbreviationName;					//eg network/server/GIAsemRelTranslatorDatabase/denovedenocorpus.txt
		#else
		folderName = folderName + POStypeAbbreviationName + "/";
		GIAdatabase.checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&POStypeAbbreviationName);		//eg network/server/GIAsemRelTranslatorDatabase/de/no/ve/de/no/corpus.txt
		#endif
		currentFeatureInSentence = currentFeatureInSentence->next;
	}

	return folderName;
}


#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS

string semanticParserCorpusDatabaseSentenceWriteFileString;

//preconditions: determineGIAconnectionistNetworkPOStypeNames has been executed
void GIAsemRelTranslatorDatabaseClass::prepareSemanticParserCorpusDatabaseFileTextForWriting()
{
	semanticParserCorpusDatabaseSentenceWriteFileString = "";
}

void GIAsemRelTranslatorDatabaseClass::saveTextLineToCorpusFileString(const string sentenceText)
{
	semanticParserCorpusDatabaseSentenceWriteFileString = semanticParserCorpusDatabaseSentenceWriteFileString + sentenceText;
	semanticParserCorpusDatabaseSentenceWriteFileString = semanticParserCorpusDatabaseSentenceWriteFileString + STRING_NEWLINE;
}

void GIAsemRelTranslatorDatabaseClass::saveTextToCorpusFileString(const string sentenceText)
{
	semanticParserCorpusDatabaseSentenceWriteFileString = semanticParserCorpusDatabaseSentenceWriteFileString + sentenceText;
}

/*
void removeTextFromCorpusFileString(string sentenceText)
{
	semanticParserCorpusDatabaseSentenceWriteFileString = replaceAllOccurancesOfString(&semanticParserCorpusDatabaseSentenceWriteFileString, sentenceText, "");	//not currently used
}

void removeTextLineFromCorpusFileString(string sentenceText)
{
	semanticParserCorpusDatabaseSentenceWriteFileString = replaceAllOccurancesOfString(&semanticParserCorpusDatabaseSentenceWriteFileString, (sentenceText+STRING_NEWLINE), "");	//not currently used
}
*/

void GIAsemRelTranslatorDatabaseClass::writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence)
{
	writeSemanticParserCorpusFile(firstFeatureInSentence, &semanticParserCorpusDatabaseSentenceWriteFileString);
}


void GIAsemRelTranslatorDatabaseClass::writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence, const string* sentenceSemanticRelationsText)
{
	string semanticParserCorpusDatabaseSentenceHeaderString = generateCorpusFileHeaderText(firstFeatureInSentence, true) + STRING_NEWLINE + STRING_NEWLINE;	 //required to add new line at end of parsingWordsAndTags as per Stanford Parser specification (see parseStanfordParserFile)
	string semanticParserCorpusDatabaseSentenceText = semanticParserCorpusDatabaseSentenceHeaderString + *sentenceSemanticRelationsText;
	semanticParserCorpusDatabaseSentenceText = semanticParserCorpusDatabaseSentenceText + STRING_NEWLINE; //required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)
	string corpusFileName = semanticParserCorpusDBgenerateFileName(firstFeatureInSentence);
	SHAREDvars.writeStringToFile(corpusFileName, &semanticParserCorpusDatabaseSentenceText);
}

//preconditions: determineGIAconnectionistNetworkPOStypeNames has been executed
bool GIAsemRelTranslatorDatabaseClass::loadSemanticParserCorpusDatabaseFile(GIAsentence* currentSentenceInList, const GIAfeature* firstFeatureInListorSubset)
{
	bool result = true;
	string corpusFileName = semanticParserCorpusDBgenerateFileName(firstFeatureInListorSubset);

	bool createNewSentences = false;
	bool parseGIA2file = true;
	bool isQuery = false;	//irrelevant
	if(!GIAnlp.parseStanfordParserFile(corpusFileName, isQuery, currentSentenceInList, createNewSentences, parseGIA2file, false))		//CHECK THIS; need to account for corpus.txt having multiple entries [eg different text but identical layout]
	{
		result = false;
		currentSentenceInList->semanticParserSuccessful = false;
	}
	else
	{
		currentSentenceInList->semanticParserSuccessful = true;
	}
	return result;
}


//NB idInstance 0 corresponds to the networkIndex entity (null instance)
string GIAsemRelTranslatorDatabaseClass::semanticParserCorpusDBgenerateFileName(const GIAfeature* firstFeatureInList)
{
	string folderName = semanticParserDBgenerateFolderName(firstFeatureInList);

	#ifdef GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_USE_FLAT_FILE
	string fileName = folderName + GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#else
	string fileName = GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#endif


	return fileName;
}

//preconditions: determineGIAconnectionistNetworkPOStypeNames() has been executed
string GIAsemRelTranslatorDatabaseClass::generateCorpusFileHeaderText(const GIAfeature* firstFeatureInSentence, const bool addPOSinfo)
{
	string sentenceText = "";
	const GIAfeature* currentFeatureInSentence = firstFeatureInSentence;
	while(currentFeatureInSentence->next != NULL)
	{
		string appendString = " ";
		if(currentFeatureInSentence->next->next == NULL)
		{
			appendString = "";
		}

		if(addPOSinfo)
		{
			string GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[currentFeatureInSentence->GIAsemRelTranslatorPOStype];
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

#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE

bool GIAsemRelTranslatorDatabaseClass::loadSemanticParserOptimisedDatabaseFile(const GIAfeature* firstFeatureInListorSubset, const int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment, int GIA2semanticDependencyRelationProbabilityArray[], int GIA2semanticDependencyRelationAssignedArray[], int GIA2semanticDependencyRelationRejectedArray[])
{
	bool result = true;
	string semanticRelationDatabaseFileName = semanticParserOptimisedDBgenerateFileName(firstFeatureInListorSubset, indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment);

	ifstream parseFileObject(semanticRelationDatabaseFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//xml file does not exist in current directory.
		cout << "Warning: semantic relation database file does not exist in current directory: " << semanticRelationDatabaseFileName << endl;
		result = false;
	}
	else
	{
		int charCount = 0;
		char currentToken;
		bool newLineDetected = false;
		string currentLine;
		int lineIndex = 0;
		while(getline(parseFileObject, currentLine))
		{
			if(lineIndex >= GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES)
			{
				cerr << "loadSemanticParserOptimisedDatabaseFile{} error: (lineIndex >= GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES)" << endl;
				cerr << "semanticRelationDatabaseFileName = " << semanticRelationDatabaseFileName << endl;
				cerr << "currentLine = " << currentLine << endl;
				exit(EXIT_ERROR);
			}

			bool parsingNumberOfSemanticRelationsAssignedForTuple = true;
			bool parsingNumberOfSemanticRelationsRejectedForTuple = false;
			int numberOfSemanticRelationsAssignedForTuple = 0;
			int numberOfSemanticRelationsRejectedForTuple = 0;
			string numberOfSemanticRelationsAssignedForTupleString = "";
			string numberOfSemanticRelationsRejectedForTupleString = "";
			for(int i=0; i < currentLine.length(); i++)
			{
				if(currentLine[i] == GIA_SEM_REL_TRANSLATOR_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER)
				{
					if(parsingNumberOfSemanticRelationsAssignedForTuple)
					{
						parsingNumberOfSemanticRelationsAssignedForTuple = false;
						parsingNumberOfSemanticRelationsRejectedForTuple = true;
					}
					else
					{
						cerr << "loadSemanticParserOptimisedDatabaseFile{} error: !parsingNumberOfSemanticRelationsAssignedForTuple && (currentLine[i] == GIA_SEM_REL_TRANSLATOR_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER)" << endl;
						cerr << "semanticRelationDatabaseFileName = " << semanticRelationDatabaseFileName << endl;
						cerr << "currentLine = " << currentLine << endl;
						exit(EXIT_ERROR);
					}
				}
				else
				{
					if(parsingNumberOfSemanticRelationsAssignedForTuple)
					{
						numberOfSemanticRelationsAssignedForTupleString = numberOfSemanticRelationsAssignedForTupleString + currentLine[i];
					}
					else if(parsingNumberOfSemanticRelationsRejectedForTuple)
					{
						numberOfSemanticRelationsRejectedForTupleString = numberOfSemanticRelationsRejectedForTupleString + currentLine[i];
					}
				}
			}
			numberOfSemanticRelationsAssignedForTuple = SHAREDvars.convertStringToInt(numberOfSemanticRelationsAssignedForTupleString);
			numberOfSemanticRelationsRejectedForTuple = SHAREDvars.convertStringToInt(numberOfSemanticRelationsRejectedForTupleString);

			int probabilityMetric = numberOfSemanticRelationsAssignedForTuple-numberOfSemanticRelationsRejectedForTuple;

			GIA2semanticDependencyRelationProbabilityArray[lineIndex] = probabilityMetric;
			GIA2semanticDependencyRelationAssignedArray[lineIndex] = numberOfSemanticRelationsAssignedForTuple;
			GIA2semanticDependencyRelationRejectedArray[lineIndex] = numberOfSemanticRelationsRejectedForTuple;

			lineIndex++;
		}
	}
	return result;
}

void GIAsemRelTranslatorDatabaseClass::writeSemanticParserOptimisedDatabaseFile(const GIAfeature* firstFeatureInListorSubset, const int firstWordInTupleIndex, const int semanticDependencyRelationType, const bool directionGovernorToDependent, const bool sameReferenceSet)
{
	int GIA2semanticDependencyRelationProbabilityArray[GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
	int GIA2semanticDependencyRelationAssignedArray[GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
	int GIA2semanticDependencyRelationRejectedArray[GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
	initialiseIntArray(GIA2semanticDependencyRelationProbabilityArray, GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);
	initialiseIntArray(GIA2semanticDependencyRelationAssignedArray, GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);
	initialiseIntArray(GIA2semanticDependencyRelationRejectedArray, GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);

	int semanticRelationTypeOptimisedDatabase = GIA_SEM_REL_TRANSLATOR_RELATION_TYPE_NON_EXISTENT;
	if(semanticDependencyRelationType != GIA_SEM_REL_TRANSLATOR_RELATION_TYPE_NON_EXISTENT)
	{
		semanticRelationTypeOptimisedDatabase = calculateSemanticRelationTypeOptimisedDatabase(semanticDependencyRelationType, directionGovernorToDependent, sameReferenceSet);
	}

	if(loadSemanticParserOptimisedDatabaseFile(firstFeatureInListorSubset, firstWordInTupleIndex, GIA2semanticDependencyRelationProbabilityArray, GIA2semanticDependencyRelationAssignedArray, GIA2semanticDependencyRelationRejectedArray))
	{

	}

	if(semanticDependencyRelationType != GIA_SEM_REL_TRANSLATOR_RELATION_TYPE_NON_EXISTENT)
	{
		GIA2semanticDependencyRelationAssignedArray[semanticRelationTypeOptimisedDatabase] = GIA2semanticDependencyRelationAssignedArray[semanticRelationTypeOptimisedDatabase] + 1;
		GIA2semanticDependencyRelationProbabilityArray[semanticRelationTypeOptimisedDatabase] = GIA2semanticDependencyRelationProbabilityArray[semanticRelationTypeOptimisedDatabase] + 1;
	}
	for(int i=0; i<GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES; i++)
	{
		if((semanticDependencyRelationType == GIA_SEM_REL_TRANSLATOR_RELATION_TYPE_NON_EXISTENT) || (i != semanticRelationTypeOptimisedDatabase))
		{
			GIA2semanticDependencyRelationRejectedArray[i] = GIA2semanticDependencyRelationRejectedArray[i]	+ 1;
			GIA2semanticDependencyRelationProbabilityArray[i] = GIA2semanticDependencyRelationProbabilityArray[i] - 1;
		}
	}

	string semanticRelationsDatabaseFileString = "";
	for(int i=0; i<GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES; i++)
	{
		string numberOfSemanticRelationsAssignedForTupleString = SHAREDvars.convertIntToString(GIA2semanticDependencyRelationAssignedArray[i]);
		string numberOfSemanticRelationsRejectedForTupleString = SHAREDvars.convertIntToString(GIA2semanticDependencyRelationRejectedArray[i]);
		string numberOfSemanticRelationsProbabilityForTupleString = SHAREDvars.convertIntToString(GIA2semanticDependencyRelationProbabilityArray[i]);
		string currentLine =  numberOfSemanticRelationsAssignedForTupleString + GIA_SEM_REL_TRANSLATOR_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER + numberOfSemanticRelationsRejectedForTupleString + CHAR_NEWLINE;
		semanticRelationsDatabaseFileString = semanticRelationsDatabaseFileString + currentLine;
	}

	string semanticRelationDatabaseFileName = semanticParserOptimisedDBgenerateFileName(firstFeatureInListorSubset, firstWordInTupleIndex);
	SHAREDvars.writeStringToFile(semanticRelationDatabaseFileName, &semanticRelationsDatabaseFileString);
}

void GIAsemRelTranslatorDatabaseClass::initialiseIntArray(int intArray[], const int size, int value)
{
	for(int i=0; i<size; i++)
	{
		intArray[i] = value;
	}
}


int GIAsemRelTranslatorDatabaseClass::calculateSemanticRelationTypeOptimisedDatabase(const int semanticDependencyRelationType, const bool directionGovernorToDependent, const bool sameReferenceSet)
{
	int semanticRelationTypeOptimisedDatabase = (semanticDependencyRelationType*GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET) + int(directionGovernorToDependent)*GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET + int(sameReferenceSet);
	return semanticRelationTypeOptimisedDatabase;
}
bool GIAsemRelTranslatorDatabaseClass::calculateSemanticRelationTypeDirection(const int semanticRelationTypeOptimisedDatabase)
{
	bool directionGovernorToDependent = false;
	int semanticRelationTypeOptimisedDatabaseRemainder = semanticRelationTypeOptimisedDatabase%(GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET);
	if(semanticRelationTypeOptimisedDatabaseRemainder/2 == 1)
	{
		directionGovernorToDependent = true;
	}
	else
	{
		directionGovernorToDependent = false;
	}
	return directionGovernorToDependent;
}
bool GIAsemRelTranslatorDatabaseClass::calculateSemanticRelationTypeSameReferenceSet(const int semanticRelationTypeOptimisedDatabase)
{
	bool sameReferenceSet = false;
	int semanticRelationTypeOptimisedDatabaseRemainder = semanticRelationTypeOptimisedDatabase%(GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET);
	if(semanticRelationTypeOptimisedDatabaseRemainder%2 == 1)
	{
		sameReferenceSet = true;
	}
	else
	{
		sameReferenceSet = false;
	}
	return sameReferenceSet;
}

int GIAsemRelTranslatorDatabaseClass::calculateSemanticRelationTypeFromOptimisedDatabaseSemanticRelationType(const int semanticRelationTypeOptimisedDatabase)
{
	int semanticDependencyRelationType = semanticRelationTypeOptimisedDatabase/(GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET);
	return semanticDependencyRelationType;
}


//assume featureList is a subet of the sentence featureList (starting with firstFeatureInSentenceSubset, and ending with centralWord/indexOfLastWordInTupleBeingAssessedForSemanticRelationAssignment)
string GIAsemRelTranslatorDatabaseClass::semanticParserOptimisedDBgenerateFileName(const GIAfeature* firstFeatureInList, const int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment)
{
	string folderName = semanticParserDBgenerateFolderName(firstFeatureInList);

	#ifdef GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_USE_FLAT_FILE
	string fileName = folderName + GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + SHAREDvars.convertIntToString(indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment) + GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#else
	string fileName = GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + SHAREDvars.convertIntToString(indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment) + GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#endif


	return fileName;
}

void GIAsemRelTranslatorDatabaseClass::addTupleSemanticRelationToSentence(GIAsentence* currentSentenceInList, const int semanticRelationTypeOptimisedDatabase, const int firstWordInTupleIndex, const int secondWordInTupleIndex)
{
	int semanticRelationType = calculateSemanticRelationTypeFromOptimisedDatabaseSemanticRelationType(semanticRelationTypeOptimisedDatabase);
	bool directionGovernorToDependent = calculateSemanticRelationTypeDirection(semanticRelationTypeOptimisedDatabase);
	bool sameReferenceSet = calculateSemanticRelationTypeSameReferenceSet(semanticRelationTypeOptimisedDatabase);

	string semanticRelation = GIA2semanticDependencyRelationNameArray[semanticRelationType];
	GIAfeature* currentFeatureInList = currentSentenceInList->firstFeatureInList;

	string governorEntityName = "";
	string dependentEntityName = "";
	int governorIndex = 0;
	int dependentIndex = 0;
	while(currentFeatureInList->next != NULL)
	{
		if(directionGovernorToDependent)
		{
			if(currentFeatureInList->entityIndex == firstWordInTupleIndex)
			{
				governorEntityName = currentFeatureInList->lemma;
				governorIndex = firstWordInTupleIndex;
			}
			else if(currentFeatureInList->entityIndex == secondWordInTupleIndex)
			{
				dependentEntityName = currentFeatureInList->lemma;
				dependentIndex = secondWordInTupleIndex;
			}
		}
		else
		{
			if(currentFeatureInList->entityIndex == firstWordInTupleIndex)
			{
				dependentEntityName = currentFeatureInList->lemma;
				dependentIndex = firstWordInTupleIndex;
			}
			else if(currentFeatureInList->entityIndex == secondWordInTupleIndex)
			{
				governorEntityName = currentFeatureInList->lemma;
				governorIndex = secondWordInTupleIndex;
			}
		}
		currentFeatureInList = currentFeatureInList->next;
	}


	//get the last relation in the list..
	GIArelation* currentRelationInList = currentSentenceInList->firstRelationInList;
	while(currentRelationInList->next != NULL)
	{
		currentRelationInList = currentRelationInList->next;
	}

	generateGIA2semanticDependencyRelationObject(currentRelationInList, governorEntityName, dependentEntityName, semanticRelation, governorIndex, dependentIndex, sameReferenceSet);

	currentRelationInList->next = new GIArelation();
	currentRelationInList = currentRelationInList->next;	//redundant
}

#endif

#endif

#ifdef GIA_SEM_REL_TRANSLATOR_COMMON

void GIAsemRelTranslatorDatabaseClass::generateGIA2semanticDependencyRelationObject(GIArelation* GIA2semanticDependencyRelation, string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	GIA2semanticDependencyRelation->relationType = semanticRelation;
	GIA2semanticDependencyRelation->relationGovernor = entityName1;
	GIA2semanticDependencyRelation->relationGovernorIndex = entityIndex1;
	GIA2semanticDependencyRelation->relationDependent = entityName2;
	GIA2semanticDependencyRelation->relationDependentIndex = entityIndex2;
	GIA2semanticDependencyRelation->sameReferenceSet = sameReferenceSet;
}

#ifndef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD

GIArelation* firstRelationInSemanticParserSentence;

GIArelation* GIAsemRelTranslatorDatabaseClass::getFirstRelationInSemanticParserSentenceList()
{
	return firstRelationInSemanticParserSentence;
}

GIArelation* GIAsemRelTranslatorDatabaseClass::getCurrentRelationInSemanticParserSentenceList()
{
	GIArelation* currentRelationInSemanticParserSentence = firstRelationInSemanticParserSentence;
	while(currentRelationInSemanticParserSentence->next != NULL)
	{
		currentRelationInSemanticParserSentence = currentRelationInSemanticParserSentence->next;
	}
	return currentRelationInSemanticParserSentence;
}

void GIAsemRelTranslatorDatabaseClass::prepareSemanticParserSemanticRelationListForWriting()
{
	if(firstRelationInSemanticParserSentence != NULL)
	{
		delete firstRelationInSemanticParserSentence;
	}
	firstRelationInSemanticParserSentence = new GIArelation();
}
#endif

#endif





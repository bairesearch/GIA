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
 * File Name: GIAsemanticParserDatabase.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2k6c 27-July-2015
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#include "GIAsemanticParserDatabase.h"
#include "GIAtranslatorDefs.h"	//required for GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES
#include "GIAdatabase.h"	//required for DBgenerateServerDatabaseName
#include "GIAnlp.h"
#include "SHAREDvars.h"	//required for writeStringToFileObject

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER

static string semanticParserDatabaseFolderName;
void initialiseSemanticParserDatabase(string newSemanticParserDatabaseFolderName)
{
	semanticParserDatabaseFolderName = newSemanticParserDatabaseFolderName;
}

void prepareSemanticParserDatabaseForWriting()
{	
	#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD
	prepareSemanticParserCorpusDatabaseFileTextForWriting();
	#else
	prepareSemanticParserSemanticRelationListForWriting();
	#endif
}

string semanticParserDBgenerateFolderName(GIAfeature* firstFeatureInList)
{
	string folderName = "";
	
	//eg network/server/GIAsemanticParserDatabase/de/no/ve/de/no/corpus.txt
	int fileType = 0;	//irrelevant
	//cout << "firstFeatureInList->GIAsemanticParserPOStype = " << firstFeatureInList->GIAsemanticParserPOStype << endl;
	string serverName = DBgenerateServerDatabaseName(&(GIAconnectionistNetworkPOStypeNameAbbreviationArray[firstFeatureInList->GIAsemanticParserPOStype]), fileType, GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME, semanticParserDatabaseFolderName);

	#ifdef GIA2_SEMANTIC_PARSER_DEBUG
	//cout << "semanticParserDBgenerateFolderName{}: fileName = " << serverName << endl;
	#endif
	DBsetCurrentDirectory(&serverName);

	GIAfeature* currentFeatureInSentence = firstFeatureInList;
	while(currentFeatureInSentence->next != NULL)
	{
		string POStypeAbbreviationName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAsemanticParserPOStype];
		#ifdef GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_USE_FLAT_FILE
		folderName = folderName + POStypeAbbreviationName;					//eg network/server/GIAsemanticParserDatabase/denovedenocorpus.txt
		#else
		folderName = folderName + POStypeAbbreviationName + "/";
		checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&POStypeAbbreviationName);		//eg network/server/GIAsemanticParserDatabase/de/no/ve/de/no/corpus.txt
		#endif
		currentFeatureInSentence = currentFeatureInSentence->next;
	}
	
	return folderName;
}

#ifndef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD

GIArelation* firstRelationInSemanticParserSentence;

GIArelation* getFirstRelationInSemanticParserSentenceList()
{
	return firstRelationInSemanticParserSentence;
}

GIArelation* getCurrentRelationInSemanticParserSentenceList()
{
	GIArelation* currentRelationInSemanticParserSentence = firstRelationInSemanticParserSentence;
	while(currentRelationInSemanticParserSentence->next != NULL)
	{
		currentRelationInSemanticParserSentence = currentRelationInSemanticParserSentence->next;
	}
	return currentRelationInSemanticParserSentence;
}

void prepareSemanticParserSemanticRelationListForWriting()
{
	if(firstRelationInSemanticParserSentence != NULL)
	{
		delete firstRelationInSemanticParserSentence;
	}
	firstRelationInSemanticParserSentence = new GIArelation();
}
#endif


#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD

string semanticParserCorpusDatabaseSentenceWriteFileString;

//preconditions: determineGIAconnectionistNetworkPOStypeNames has been executed
void prepareSemanticParserCorpusDatabaseFileTextForWriting()
{
	semanticParserCorpusDatabaseSentenceWriteFileString = "";
}

void saveTextLineToCorpusFileString(string sentenceText)
{
	semanticParserCorpusDatabaseSentenceWriteFileString = semanticParserCorpusDatabaseSentenceWriteFileString + sentenceText;
	semanticParserCorpusDatabaseSentenceWriteFileString = semanticParserCorpusDatabaseSentenceWriteFileString + STRING_NEW_LINE;
}

void saveTextToCorpusFileString(string sentenceText)
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
	semanticParserCorpusDatabaseSentenceWriteFileString = replaceAllOccurancesOfString(&semanticParserCorpusDatabaseSentenceWriteFileString, (sentenceText+STRING_NEW_LINE), "");	//not currently used
}
*/

void writeSemanticParserCorpusFile(GIAfeature* firstFeatureInSentence)
{
	writeSemanticParserCorpusFile(firstFeatureInSentence, &semanticParserCorpusDatabaseSentenceWriteFileString)
}

#else

string generateSemanticParserCorpusSemanticRelationsText(GIArelation* firstSemanticRelationInList)
{
	string sentenceSemanticRelationsText = "";
	GIArelation* currentSemanticRelationInList = firstSemanticRelationInList;
	while(currentSemanticRelationInList->next != NULL)
	{
		string GIA2semanticDependencyRelation = generateGIA2semanticDependencyRelationText(currentSemanticRelationInList->relationGovernor, currentSemanticRelationInList->relationDependent, currentSemanticRelationInList->relationType, currentSemanticRelationInList->relationGovernorIndex, currentSemanticRelationInList->relationDependentIndex, currentSemanticRelationInList->sameReferenceSet, currentSemanticRelationInList->rcmodIndicatesSameReferenceSet);
		GIA2semanticDependencyRelation = GIA2semanticDependencyRelation + STRING_NEW_LINE;
		sentenceSemanticRelationsText = sentenceSemanticRelationsText + GIA2semanticDependencyRelation;

		currentSemanticRelationInList = currentSemanticRelationInList->next;
	}
	sentenceSemanticRelationsText = sentenceSemanticRelationsText + STRING_NEW_LINE;	//required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)
	return sentenceSemanticRelationsText;
}

#endif

void writeSemanticParserCorpusFile(GIAfeature* firstFeatureInSentence, string* sentenceSemanticRelationsText)
{
	string semanticParserCorpusDatabaseSentenceHeaderString = generateCorpusFileHeaderText(firstFeatureInSentence, true) + STRING_NEW_LINE + STRING_NEW_LINE;	 //required to add new line at end of parsingWordsAndTags as per Stanford Parser specification (see parseStanfordParserFile)
	semanticParserCorpusDatabaseSentenceText = semanticParserCorpusDatabaseSentenceHeaderString + *sentenceSemanticRelationsText;
	semanticParserCorpusDatabaseSentenceText = semanticParserCorpusDatabaseSentenceText + STRING_NEW_LINE; //required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)
	string corpusFileName = semanticParserCorpusDBgenerateFileName(firstFeatureInSentence);
	writeStringToFile(corpusFileName, &semanticParserCorpusDatabaseSentenceText);
}

//preconditions: determineGIAconnectionistNetworkPOStypeNames has been executed
bool loadSemanticParserCorpusDatabaseFile(GIAsentence* currentSentenceInList, GIAfeature* firstFeatureInListorSubset)
{
	bool result = true;
	string corpusFileName = semanticParserCorpusDBgenerateFileName(firstFeatureInListorSubset);
	
	bool createNewSentences = false;
	bool parseGIA2file = true;
	bool isQuery = false;	//irrelevant
	//cout << "g2" << endl;
	if(!parseStanfordParserFile(corpusFileName, isQuery, currentSentenceInList, createNewSentences, parseGIA2file, false))		//CHECK THIS; need to account for corpus.txt having multiple entries [eg different text but identical layout]
	{
		result = false;
		//cout << "1 !semanticParserSuccessful" << endl;
		currentSentenceInList->semanticParserSuccessful = false;
	}
	else
	{
		//cout << "1 semanticParserSuccessful" << endl;
		currentSentenceInList->semanticParserSuccessful = true;
	}
	//cout << "g3" << endl;
	return result;
}


//NB idInstance 0 corresponds to the concept entity (null instance)
string semanticParserCorpusDBgenerateFileName(GIAfeature* firstFeatureInList)
{
	string folderName = semanticParserDBgenerateFolderName(firstFeatureInList);

	#ifdef GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_USE_FLAT_FILE
	string fileName = folderName + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME;
	#else
	string fileName = GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME;
	#endif

	#ifdef GIA2_SEMANTIC_PARSER_DEBUG
	cout << "fileName = " << fileName << endl;
	#endif

	return fileName;
}

#endif

#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE

bool loadSemanticParserOptimisedDatabaseFile(GIAfeature* firstFeatureInListorSubset, int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment, int GIA2semanticDependencyRelationProbabilityArray[], int GIA2semanticDependencyRelationAssignedArray[], int GIA2semanticDependencyRelationRejectedArray[])
{
	bool result = true;
	string semanticRelationDatabaseFileName = semanticParserOptimisedDBgenerateFileName(firstFeatureInListorSubset, indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment);

	ifstream parseFileObject(semanticRelationDatabaseFileName.c_str());
	if(!parseFileObject.rdbuf()->is_open())
	{
		//xml file does not exist in current directory.
		cout << "Error: semantic relation database file does not exist in current directory: " << semanticRelationDatabaseFileName << endl;
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
			#ifdef GIA2_SEMANTIC_PARSER_DEBUG
			cout << "lineIndex: " << lineIndex << endl;
			cout << "semanticRelation = " << GIA2semanticDependencyRelationNameArray[lineIndex];
			#endif
			if(lineIndex >= GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES)
			{
				cout << "loadSemanticParserOptimisedDatabaseFile{} error: (lineIndex >= GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES)" << endl;
				cout << "semanticRelationDatabaseFileName = " << semanticRelationDatabaseFileName << endl;
				cout << "currentLine = " << currentLine << endl;
				exit(0);
			}
			
			bool parsingNumberOfSemanticRelationsAssignedForTuple = true;
			bool parsingNumberOfSemanticRelationsRejectedForTuple = false;
			int numberOfSemanticRelationsAssignedForTuple = 0;
			int numberOfSemanticRelationsRejectedForTuple = 0;
			string numberOfSemanticRelationsAssignedForTupleString = "";
			string numberOfSemanticRelationsRejectedForTupleString = "";
			for(int i=0; i < currentLine.length(); i++)
			{
				if(currentLine[i] == GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER)
				{
					if(parsingNumberOfSemanticRelationsAssignedForTuple)
					{
						parsingNumberOfSemanticRelationsAssignedForTuple = false;
						parsingNumberOfSemanticRelationsRejectedForTuple = true;
					}
					else
					{
						cout << "loadSemanticParserOptimisedDatabaseFile{} error: !parsingNumberOfSemanticRelationsAssignedForTuple && (currentLine[i] == GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER)" << endl;
						cout << "semanticRelationDatabaseFileName = " << semanticRelationDatabaseFileName << endl;
						cout << "currentLine = " << currentLine << endl;
						exit(0);
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
			numberOfSemanticRelationsAssignedForTuple = convertStringToInt(numberOfSemanticRelationsAssignedForTupleString);
			numberOfSemanticRelationsRejectedForTuple = convertStringToInt(numberOfSemanticRelationsRejectedForTupleString);

			int probabilityMetric = numberOfSemanticRelationsAssignedForTuple-numberOfSemanticRelationsRejectedForTuple;
			
			GIA2semanticDependencyRelationProbabilityArray[lineIndex] = probabilityMetric;
			GIA2semanticDependencyRelationAssignedArray[lineIndex] = numberOfSemanticRelationsAssignedForTuple;
			GIA2semanticDependencyRelationRejectedArray[lineIndex] = numberOfSemanticRelationsRejectedForTuple;			
				
			#ifdef GIA2_SEMANTIC_PARSER_DEBUG		
			cout << "numberOfSemanticRelationsAssignedForTuple = " << numberOfSemanticRelationsAssignedForTuple << endl;
			cout << "numberOfSemanticRelationsRejectedForTuple = " << numberOfSemanticRelationsRejectedForTuple << endl;
			cout << "probabilityMetric = " << probabilityMetric << endl;
			#endif
			
			lineIndex++;
		}
	}	
	return result;
}

void writeSemanticParserOptimisedDatabaseFile(GIAfeature* firstFeatureInListorSubset, int firstWordInTupleIndex, int semanticDependencyRelationType, bool directionGovernorToDependent, bool sameReferenceSet)
{
	int GIA2semanticDependencyRelationProbabilityArray[GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
	int GIA2semanticDependencyRelationAssignedArray[GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
	int GIA2semanticDependencyRelationRejectedArray[GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
	initialiseIntArray(GIA2semanticDependencyRelationProbabilityArray, GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);
	initialiseIntArray(GIA2semanticDependencyRelationAssignedArray, GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);
	initialiseIntArray(GIA2semanticDependencyRelationRejectedArray, GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);

	int semanticRelationTypeOptimisedDatabase = SEMANTIC_RELATION_TYPE_NON_EXISTENT;
	if(semanticDependencyRelationType != SEMANTIC_RELATION_TYPE_NON_EXISTENT)
	{
		semanticRelationTypeOptimisedDatabase = calculateSemanticRelationTypeOptimisedDatabase(semanticDependencyRelationType, directionGovernorToDependent, sameReferenceSet);
	}
	
	if(loadSemanticParserOptimisedDatabaseFile(firstFeatureInListorSubset, firstWordInTupleIndex, GIA2semanticDependencyRelationProbabilityArray, GIA2semanticDependencyRelationAssignedArray, GIA2semanticDependencyRelationRejectedArray))
	{

	}

	if(semanticDependencyRelationType != SEMANTIC_RELATION_TYPE_NON_EXISTENT)
	{
		GIA2semanticDependencyRelationAssignedArray[semanticRelationTypeOptimisedDatabase] = GIA2semanticDependencyRelationAssignedArray[semanticRelationTypeOptimisedDatabase] + 1;
		GIA2semanticDependencyRelationProbabilityArray[semanticRelationTypeOptimisedDatabase] = GIA2semanticDependencyRelationProbabilityArray[semanticRelationTypeOptimisedDatabase] + 1;
	}
	for(int i=0; i<GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES; i++)
	{
		if((semanticDependencyRelationType == SEMANTIC_RELATION_TYPE_NON_EXISTENT) || (i != semanticRelationTypeOptimisedDatabase))
		{
			GIA2semanticDependencyRelationRejectedArray[i] = GIA2semanticDependencyRelationRejectedArray[i]	+ 1;
			GIA2semanticDependencyRelationProbabilityArray[i] = GIA2semanticDependencyRelationProbabilityArray[i] - 1;
		}
	}
	
	string semanticRelationsDatabaseFileString = "";
	for(int i=0; i<GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES; i++)
	{
		string numberOfSemanticRelationsAssignedForTupleString = convertIntToString(GIA2semanticDependencyRelationAssignedArray[i]);
		string numberOfSemanticRelationsRejectedForTupleString = convertIntToString(GIA2semanticDependencyRelationRejectedArray[i]);
		string numberOfSemanticRelationsProbabilityForTupleString = convertIntToString(GIA2semanticDependencyRelationProbabilityArray[i]);
		string currentLine =  numberOfSemanticRelationsAssignedForTupleString + GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER + numberOfSemanticRelationsRejectedForTupleString + CHAR_NEWLINE;
		#ifdef GIA2_SEMANTIC_PARSER_DEBUG
		cout << "writeSemanticParserOptimisedDatabaseFile{}: currentLine = " << currentLine << endl;
		#endif
		semanticRelationsDatabaseFileString = semanticRelationsDatabaseFileString + currentLine;
	}
	
	string semanticRelationDatabaseFileName = semanticParserOptimisedDBgenerateFileName(firstFeatureInListorSubset, firstWordInTupleIndex);
	writeStringToFile(semanticRelationDatabaseFileName, &semanticRelationsDatabaseFileString);
}

void initialiseIntArray(int intArray[], int size, int value)
{
	for(int i=0; i<size; i++)
	{
		intArray[i] = value;
	}
}


int calculateSemanticRelationTypeOptimisedDatabase(int semanticDependencyRelationType, bool directionGovernorToDependent, bool sameReferenceSet)
{
	int semanticRelationTypeOptimisedDatabase = (semanticDependencyRelationType*GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET) + int(directionGovernorToDependent)*GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET + int(sameReferenceSet);
	return semanticRelationTypeOptimisedDatabase;
}
bool calculateSemanticRelationTypeDirection(int semanticRelationTypeOptimisedDatabase)
{
	bool directionGovernorToDependent = false;
	int semanticRelationTypeOptimisedDatabaseRemainder = semanticRelationTypeOptimisedDatabase%(GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET);
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
bool calculateSemanticRelationTypeSameReferenceSet(int semanticRelationTypeOptimisedDatabase)
{
	bool sameReferenceSet = false;
	int semanticRelationTypeOptimisedDatabaseRemainder = semanticRelationTypeOptimisedDatabase%(GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET);
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

int calculateSemanticRelationTypeFromOptimisedDatabaseSemanticRelationType(int semanticRelationTypeOptimisedDatabase)
{
	int semanticDependencyRelationType = semanticRelationTypeOptimisedDatabase/(GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET);
	return semanticDependencyRelationType;
}


//assume featureList is a subet of the sentence featureList (starting with firstFeatureInSentenceSubset, and ending with centralWord/indexOfLastWordInTupleBeingAssessedForSemanticRelationAssignment)
string semanticParserOptimisedDBgenerateFileName(GIAfeature* firstFeatureInList, int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment)
{
	string folderName = semanticParserDBgenerateFolderName(firstFeatureInList);
	
	#ifdef GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_USE_FLAT_FILE
	string fileName = folderName + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + convertIntToString(indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment) + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#else
	string fileName = GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + convertIntToString(indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment) + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#endif

	#ifdef GIA2_SEMANTIC_PARSER_DEBUG
	cout << "fileName = " << fileName << endl;
	#endif

	return fileName;
}

void addTupleSemanticRelationToSentence(GIAsentence* currentSentenceInList, int semanticRelationTypeOptimisedDatabase, int firstWordInTupleIndex, int secondWordInTupleIndex)
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
	
	#ifdef GIA2_SEMANTIC_PARSER_DEBUG
	cout << "\naddTupleSemanticRelationToSentence{}:" << endl;
	cout << "governorEntityName = " << governorEntityName << endl;
	cout << "dependentEntityName = " << dependentEntityName << endl;
	cout << "governorIndex = " << governorIndex << endl;
	cout << "dependentIndex = " << dependentIndex << endl;
	cout << "directionGovernorToDependent = " << directionGovernorToDependent << endl;
	#endif
	
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

void generateGIA2semanticDependencyRelationObject(GIArelation* GIA2semanticDependencyRelation, string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	GIA2semanticDependencyRelation->relationType = semanticRelation;
	GIA2semanticDependencyRelation->relationGovernor = entityName1;
	GIA2semanticDependencyRelation->relationGovernorIndex = entityIndex1;
	GIA2semanticDependencyRelation->relationDependent = entityName2;
	GIA2semanticDependencyRelation->relationDependentIndex = entityIndex2;
	GIA2semanticDependencyRelation->sameReferenceSet = sameReferenceSet;
}

#endif

#endif





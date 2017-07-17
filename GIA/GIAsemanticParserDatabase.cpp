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
 * File Name: GIAsemanticParserDatabase.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3d3b 17-July-2017
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#include "GIAsemanticParserDatabase.hpp"

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_SEMANTIC_PARSER

static string semanticParserDatabaseFolderName;
void GIAsemanticParserDatabaseClass::initialiseSemanticParserDatabase(const string newSemanticParserDatabaseFolderName)
{
	semanticParserDatabaseFolderName = newSemanticParserDatabaseFolderName;
}

void GIAsemanticParserDatabaseClass::prepareSemanticParserDatabaseForWriting()
{
	#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	this->prepareSemanticParserCorpusDatabaseFileTextForWriting();
	#else
	this->prepareSemanticParserSemanticRelationListForWriting();
	#endif
}

string GIAsemanticParserDatabaseClass::semanticParserDBgenerateFolderName(const GIAfeature* firstFeatureInList)
{
	string folderName = "";

	//eg network/server/GIAsemanticParserDatabase/de/no/ve/de/no/corpus.txt
	int fileType = 0;	//irrelevant
	string serverName = GIAdatabase.DBgenerateServerDatabaseName(&(GIAconnectionistNetworkPOStypeNameAbbreviationArray[firstFeatureInList->GIAsemanticParserPOStype]), fileType, GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME, semanticParserDatabaseFolderName);

	GIAdatabase.DBsetCurrentDirectory(&serverName);

	const GIAfeature* currentFeatureInSentence = firstFeatureInList;
	while(currentFeatureInSentence->next != NULL)
	{
		string POStypeAbbreviationName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAsemanticParserPOStype];
		#ifdef GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_USE_FLAT_FILE
		folderName = folderName + POStypeAbbreviationName;					//eg network/server/GIAsemanticParserDatabase/denovedenocorpus.txt
		#else
		folderName = folderName + POStypeAbbreviationName + "/";
		GIAdatabase.checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&POStypeAbbreviationName);		//eg network/server/GIAsemanticParserDatabase/de/no/ve/de/no/corpus.txt
		#endif
		currentFeatureInSentence = currentFeatureInSentence->next;
	}

	return folderName;
}

#ifndef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD

GIArelation* firstRelationInSemanticParserSentence;

GIArelation* GIAsemanticParserDatabaseClass::getFirstRelationInSemanticParserSentenceList()
{
	return firstRelationInSemanticParserSentence;
}

GIArelation* GIAsemanticParserDatabaseClass::getCurrentRelationInSemanticParserSentenceList()
{
	GIArelation* currentRelationInSemanticParserSentence = firstRelationInSemanticParserSentence;
	while(currentRelationInSemanticParserSentence->next != NULL)
	{
		currentRelationInSemanticParserSentence = currentRelationInSemanticParserSentence->next;
	}
	return currentRelationInSemanticParserSentence;
}

void GIAsemanticParserDatabaseClass::prepareSemanticParserSemanticRelationListForWriting()
{
	if(firstRelationInSemanticParserSentence != NULL)
	{
		delete firstRelationInSemanticParserSentence;
	}
	firstRelationInSemanticParserSentence = new GIArelation();
}
#endif


#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS

string semanticParserCorpusDatabaseSentenceWriteFileString;

//preconditions: determineGIAconnectionistNetworkPOStypeNames has been executed
void GIAsemanticParserDatabaseClass::prepareSemanticParserCorpusDatabaseFileTextForWriting()
{
	semanticParserCorpusDatabaseSentenceWriteFileString = "";
}

void GIAsemanticParserDatabaseClass::saveTextLineToCorpusFileString(const string sentenceText)
{
	semanticParserCorpusDatabaseSentenceWriteFileString = semanticParserCorpusDatabaseSentenceWriteFileString + sentenceText;
	semanticParserCorpusDatabaseSentenceWriteFileString = semanticParserCorpusDatabaseSentenceWriteFileString + STRING_NEWLINE;
}

void GIAsemanticParserDatabaseClass::saveTextToCorpusFileString(const string sentenceText)
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

void GIAsemanticParserDatabaseClass::writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence)
{
	this->writeSemanticParserCorpusFile(firstFeatureInSentence, &semanticParserCorpusDatabaseSentenceWriteFileString);
}


void GIAsemanticParserDatabaseClass::writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence, const string* sentenceSemanticRelationsText)
{
	string semanticParserCorpusDatabaseSentenceHeaderString = generateCorpusFileHeaderText(firstFeatureInSentence, true) + STRING_NEWLINE + STRING_NEWLINE;	 //required to add new line at end of parsingWordsAndTags as per Stanford Parser specification (see parseStanfordParserFile)
	string semanticParserCorpusDatabaseSentenceText = semanticParserCorpusDatabaseSentenceHeaderString + *sentenceSemanticRelationsText;
	semanticParserCorpusDatabaseSentenceText = semanticParserCorpusDatabaseSentenceText + STRING_NEWLINE; //required to add new line at end of parsingTypedDependencies as per Stanford Parser specification (see parseStanfordParserFile)
	string corpusFileName = semanticParserCorpusDBgenerateFileName(firstFeatureInSentence);
	SHAREDvars.writeStringToFile(corpusFileName, &semanticParserCorpusDatabaseSentenceText);
}

//preconditions: determineGIAconnectionistNetworkPOStypeNames has been executed
bool GIAsemanticParserDatabaseClass::loadSemanticParserCorpusDatabaseFile(GIAsentence* currentSentenceInList, const GIAfeature* firstFeatureInListorSubset)
{
	bool result = true;
	string corpusFileName = this->semanticParserCorpusDBgenerateFileName(firstFeatureInListorSubset);

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
string GIAsemanticParserDatabaseClass::semanticParserCorpusDBgenerateFileName(const GIAfeature* firstFeatureInList)
{
	string folderName = this->semanticParserDBgenerateFolderName(firstFeatureInList);

	#ifdef GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_USE_FLAT_FILE
	string fileName = folderName + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#else
	string fileName = GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#endif


	return fileName;
}

//preconditions: determineGIAconnectionistNetworkPOStypeNames() has been executed
string GIAsemanticParserDatabaseClass::generateCorpusFileHeaderText(const GIAfeature* firstFeatureInSentence, const bool addPOSinfo)
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
			string GIAconnectionistNetworkPOStypeName = GIAconnectionistNetworkPOStypeNameArray[currentFeatureInSentence->GIAsemanticParserPOStype];
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

#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE

bool GIAsemanticParserDatabaseClass::loadSemanticParserOptimisedDatabaseFile(const GIAfeature* firstFeatureInListorSubset, const int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment, int GIA2semanticDependencyRelationProbabilityArray[], int GIA2semanticDependencyRelationAssignedArray[], int GIA2semanticDependencyRelationRejectedArray[])
{
	bool result = true;
	string semanticRelationDatabaseFileName = this->semanticParserOptimisedDBgenerateFileName(firstFeatureInListorSubset, indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment);

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
			if(lineIndex >= GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES)
			{
				cerr << "loadSemanticParserOptimisedDatabaseFile{} error: (lineIndex >= GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES)" << endl;
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
				if(currentLine[i] == GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER)
				{
					if(parsingNumberOfSemanticRelationsAssignedForTuple)
					{
						parsingNumberOfSemanticRelationsAssignedForTuple = false;
						parsingNumberOfSemanticRelationsRejectedForTuple = true;
					}
					else
					{
						cerr << "loadSemanticParserOptimisedDatabaseFile{} error: !parsingNumberOfSemanticRelationsAssignedForTuple && (currentLine[i] == GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER)" << endl;
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

void GIAsemanticParserDatabaseClass::writeSemanticParserOptimisedDatabaseFile(const GIAfeature* firstFeatureInListorSubset, const int firstWordInTupleIndex, const int semanticDependencyRelationType, const bool directionGovernorToDependent, const bool sameReferenceSet)
{
	int GIA2semanticDependencyRelationProbabilityArray[GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
	int GIA2semanticDependencyRelationAssignedArray[GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
	int GIA2semanticDependencyRelationRejectedArray[GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES];
	this->initialiseIntArray(GIA2semanticDependencyRelationProbabilityArray, GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);
	this->initialiseIntArray(GIA2semanticDependencyRelationAssignedArray, GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);
	this->initialiseIntArray(GIA2semanticDependencyRelationRejectedArray, GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES, 0);

	int semanticRelationTypeOptimisedDatabase = SEMANTIC_RELATION_TYPE_NON_EXISTENT;
	if(semanticDependencyRelationType != SEMANTIC_RELATION_TYPE_NON_EXISTENT)
	{
		semanticRelationTypeOptimisedDatabase = this->calculateSemanticRelationTypeOptimisedDatabase(semanticDependencyRelationType, directionGovernorToDependent, sameReferenceSet);
	}

	if(this->loadSemanticParserOptimisedDatabaseFile(firstFeatureInListorSubset, firstWordInTupleIndex, GIA2semanticDependencyRelationProbabilityArray, GIA2semanticDependencyRelationAssignedArray, GIA2semanticDependencyRelationRejectedArray))
	{

	}

	if(semanticDependencyRelationType != SEMANTIC_RELATION_TYPE_NON_EXISTENT)
	{
		GIA2semanticDependencyRelationAssignedArray[semanticRelationTypeOptimisedDatabase] = GIA2semanticDependencyRelationAssignedArray[semanticRelationTypeOptimisedDatabase] + 1;
		GIA2semanticDependencyRelationProbabilityArray[semanticRelationTypeOptimisedDatabase] = GIA2semanticDependencyRelationProbabilityArray[semanticRelationTypeOptimisedDatabase] + 1;
	}
	for(int i=0; i<GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES; i++)
	{
		if((semanticDependencyRelationType == SEMANTIC_RELATION_TYPE_NON_EXISTENT) || (i != semanticRelationTypeOptimisedDatabase))
		{
			GIA2semanticDependencyRelationRejectedArray[i] = GIA2semanticDependencyRelationRejectedArray[i]	+ 1;
			GIA2semanticDependencyRelationProbabilityArray[i] = GIA2semanticDependencyRelationProbabilityArray[i] - 1;
		}
	}

	string semanticRelationsDatabaseFileString = "";
	for(int i=0; i<GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES; i++)
	{
		string numberOfSemanticRelationsAssignedForTupleString = SHAREDvars.convertIntToString(GIA2semanticDependencyRelationAssignedArray[i]);
		string numberOfSemanticRelationsRejectedForTupleString = SHAREDvars.convertIntToString(GIA2semanticDependencyRelationRejectedArray[i]);
		string numberOfSemanticRelationsProbabilityForTupleString = SHAREDvars.convertIntToString(GIA2semanticDependencyRelationProbabilityArray[i]);
		string currentLine =  numberOfSemanticRelationsAssignedForTupleString + GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER + numberOfSemanticRelationsRejectedForTupleString + CHAR_NEWLINE;
		semanticRelationsDatabaseFileString = semanticRelationsDatabaseFileString + currentLine;
	}

	string semanticRelationDatabaseFileName = this->semanticParserOptimisedDBgenerateFileName(firstFeatureInListorSubset, firstWordInTupleIndex);
	SHAREDvars.writeStringToFile(semanticRelationDatabaseFileName, &semanticRelationsDatabaseFileString);
}

void GIAsemanticParserDatabaseClass::initialiseIntArray(int intArray[], const int size, int value)
{
	for(int i=0; i<size; i++)
	{
		intArray[i] = value;
	}
}


int GIAsemanticParserDatabaseClass::calculateSemanticRelationTypeOptimisedDatabase(const int semanticDependencyRelationType, const bool directionGovernorToDependent, const bool sameReferenceSet)
{
	int semanticRelationTypeOptimisedDatabase = (semanticDependencyRelationType*GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET) + int(directionGovernorToDependent)*GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET + int(sameReferenceSet);
	return semanticRelationTypeOptimisedDatabase;
}
bool GIAsemanticParserDatabaseClass::calculateSemanticRelationTypeDirection(const int semanticRelationTypeOptimisedDatabase)
{
	bool directionGovernorToDependent = false;
	int semanticRelationTypeOptimisedDatabaseRemainder = semanticRelationTypeOptimisedDatabase%(GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET);
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
bool GIAsemanticParserDatabaseClass::calculateSemanticRelationTypeSameReferenceSet(const int semanticRelationTypeOptimisedDatabase)
{
	bool sameReferenceSet = false;
	int semanticRelationTypeOptimisedDatabaseRemainder = semanticRelationTypeOptimisedDatabase%(GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET);
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

int GIAsemanticParserDatabaseClass::calculateSemanticRelationTypeFromOptimisedDatabaseSemanticRelationType(const int semanticRelationTypeOptimisedDatabase)
{
	int semanticDependencyRelationType = semanticRelationTypeOptimisedDatabase/(GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_DIRECTIONS*GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_SAMEREFERENCESET);
	return semanticDependencyRelationType;
}


//assume featureList is a subet of the sentence featureList (starting with firstFeatureInSentenceSubset, and ending with centralWord/indexOfLastWordInTupleBeingAssessedForSemanticRelationAssignment)
string GIAsemanticParserDatabaseClass::semanticParserOptimisedDBgenerateFileName(const GIAfeature* firstFeatureInList, const int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment)
{
	string folderName = this->semanticParserDBgenerateFolderName(firstFeatureInList);

	#ifdef GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_USE_FLAT_FILE
	string fileName = folderName + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + SHAREDvars.convertIntToString(indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment) + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#else
	string fileName = GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME + SHAREDvars.convertIntToString(indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment) + GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME;
	#endif


	return fileName;
}

void GIAsemanticParserDatabaseClass::addTupleSemanticRelationToSentence(GIAsentence* currentSentenceInList, const int semanticRelationTypeOptimisedDatabase, const int firstWordInTupleIndex, const int secondWordInTupleIndex)
{
	int semanticRelationType = this->calculateSemanticRelationTypeFromOptimisedDatabaseSemanticRelationType(semanticRelationTypeOptimisedDatabase);
	bool directionGovernorToDependent = this->calculateSemanticRelationTypeDirection(semanticRelationTypeOptimisedDatabase);
	bool sameReferenceSet = this->calculateSemanticRelationTypeSameReferenceSet(semanticRelationTypeOptimisedDatabase);

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

	this->generateGIA2semanticDependencyRelationObject(currentRelationInList, governorEntityName, dependentEntityName, semanticRelation, governorIndex, dependentIndex, sameReferenceSet);

	currentRelationInList->next = new GIArelation();
	currentRelationInList = currentRelationInList->next;	//redundant
}

#endif

void GIAsemanticParserDatabaseClass::generateGIA2semanticDependencyRelationObject(GIArelation* GIA2semanticDependencyRelation, string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet)
{
	GIA2semanticDependencyRelation->relationType = semanticRelation;
	GIA2semanticDependencyRelation->relationGovernor = entityName1;
	GIA2semanticDependencyRelation->relationGovernorIndex = entityIndex1;
	GIA2semanticDependencyRelation->relationDependent = entityName2;
	GIA2semanticDependencyRelation->relationDependentIndex = entityIndex2;
	GIA2semanticDependencyRelation->sameReferenceSet = sameReferenceSet;
}


#endif





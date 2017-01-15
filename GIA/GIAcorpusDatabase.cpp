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
 * File Name: GIAcorpusDatabase.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2f18a 22-July-2014
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
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

#include "GIAcorpusDatabase.h"
#include "GIAdatabase.h"
#include "GIAnlp.h"

#ifndef LINUX
	#include <windows.h>
#endif

#ifdef GIA_USE_CORPUS_DATABASE

static string corpusDatabaseFolderName;
ofstream corpusWriteFileObjectStream; //(conceptEntityNodesListFileName.c_str());

void initialiseCorpusDatabase(string newCorpusDatabaseFolderName)
{
	corpusDatabaseFolderName = newCorpusDatabaseFolderName;
}

//preconditions: determineGIAconnectionistNetworkPOStypeNames has been executed
string createNewCorpusFileAndOpenItForWriting(Feature * firstFeatureInSentence)
{
	string corpusFileName = corpusDBgenerateFileName(firstFeatureInSentence);
	corpusWriteFileObjectStream.open(corpusFileName);  //= new ofstream(corpusFileName);
	return corpusFileName;
}

void closeCorpusFile()
{
	corpusWriteFileObjectStream.close();
}

void saveTextLineToCurrentCorpusFile(string sentenceText)
{
	writeStringToFileObject2(sentenceText, &corpusWriteFileObjectStream);
	writeStringToFileObject2(STRING_NEW_LINE, &corpusWriteFileObjectStream);
}

void saveTextToCurrentCorpusFile(string sentenceText)
{
	writeStringToFileObject2(sentenceText, &corpusWriteFileObjectStream);
}

//from XMLparserClass.cpp
void writeStringToFileObject2(string s, ofstream * writeFileObject)
{
	for(int i=0; i < s.size(); i++)
	{
		writeFileObject->put(s[i]); //(s.cStr())[i]
	}
}


//preconditions: determineGIAconnectionistNetworkPOStypeNames has been executed
bool loadCorpusFileSemanticDependencyRelations(Sentence * currentSentenceInList, Feature * firstFeatureInListorSubset)
{
	bool result = true;
	string corpusFileName = corpusDBgenerateFileName(firstFeatureInListorSubset);
	bool createNewSentences = false;
	bool parseGIA2file = true;
	bool isQuery = false;	//irrelevant
	//cout << "g2" << endl;
	if(!parseStanfordParserFile(corpusFileName, isQuery, currentSentenceInList, createNewSentences, parseGIA2file, false))		//CHECK THIS; need to account for corpus.txt having multiple entries [eg different text but identical layout]
	{
		result = false;
		//cout << "1 !corpusLookupSuccessful" << endl;
		currentSentenceInList->corpusLookupSuccessful = false;
	}
	else
	{
		//cout << "1 corpusLookupSuccessful" << endl;
		currentSentenceInList->corpusLookupSuccessful = true;
	}
	//cout << "g3" << endl;
	return result;
}

//NB idInstance 0 corresponds to the concept entity (null instance)
string corpusDBgenerateFileName(Feature * firstFeatureInList)
{
	//eg network/server/GIAcorpusDatabase/de/no/ve/de/no/corpus.txt
	int fileType = 0;	//irrelevant
	//cout << "firstFeatureInList->GIAconnectionistNetworkPOStype = " << firstFeatureInList->GIAconnectionistNetworkPOStype << endl;
	string serverName = DBgenerateServerDatabaseName(&(GIAconnectionistNetworkPOStypeNameAbbreviationArray[firstFeatureInList->GIAconnectionistNetworkPOStype]), fileType, GIA_CORPUS_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME, corpusDatabaseFolderName);

	//#ifdef GIA_DATABASE_DEBUG_FILESYSTEM_IO
	//cout << "1fileName = " << serverName << endl;
	//#endif
	DBsetCurrentDirectory(&serverName);

	string fileName = "";

	Feature * currentFeatureInSentence = firstFeatureInList;
	while(currentFeatureInSentence->next != NULL)
	{
		string folderName = GIAconnectionistNetworkPOStypeNameAbbreviationArray[currentFeatureInSentence->GIAconnectionistNetworkPOStype];
		#ifdef GIA_CORPUS_DATABASE_FILESYSTEM_USE_FLAT_FILE
		fileName = fileName + folderName;					//eg network/server/GIAcorpusDatabase/denovedenocorpus.txt
		#else
		fileName = fileName + folderName + "/";
		checkIfFolderExistsAndIfNotMakeAndSetAsCurrent(&folderName);		//eg network/server/GIAcorpusDatabase/de/no/ve/de/no/corpus.txt
		#endif
		currentFeatureInSentence = currentFeatureInSentence->next;
	}
	#ifdef GIA_CORPUS_DATABASE_FILESYSTEM_USE_FLAT_FILE
	fileName = fileName + GIA_CORPUS_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME;
	#else
	fileName = GIA_CORPUS_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME;
	#endif

	//#ifdef GIA_DATABASE_DEBUG
	cout << "fileName = " << fileName << endl;
	//#endif

	return fileName;
}


#endif






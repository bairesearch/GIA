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
 * File Name: GIAcorpusDatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2h2g 18-November-2014
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_CORPUS_DATABASE
#define HEADER_GIA_CORPUS_DATABASE

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"

#define GIA_CORPUS_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAcorpusdatabase/")
#define GIA_CORPUS_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME ((string)"corpus.txt")

#ifdef GIA2_RECORD_DETERMINERS_AS_DEFINITE_INDEFINITE_SPECIFIC
	#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES (24)
	static string GIAconnectionistNetworkPOStypeNameAbbreviationArray[GIA_CONNECTIONIST_NETWORK_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES] = {"Un", "Cc", "Nu", "De", "Uk", "Po", "Mo", "Pr", "Aj", "No", "Av", "Pp", "Ps", "In", "Ve", "Wh", "Pd", "Pq", "Ab", "Ah", "Ad", "Di"};
#else
	#define GIA_CONNECTIONIST_NETWORK_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES (23)
	static string GIAconnectionistNetworkPOStypeNameAbbreviationArray[GIA_CONNECTIONIST_NETWORK_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES] = {"Un", "Cc", "Nu", "De", "Uk", "Po", "Mo", "Pr", "Aj", "No", "Av", "Pp", "Ps", "In", "Ve", "Wh", "Pd", "Pq", "Ab", "Ah", "Ad"};
#endif

#ifdef GIA_USE_CORPUS_DATABASE

void initialiseCorpusDatabase(string newCorpusDatabaseFolderName);
string createNewCorpusFileAndOpenItForWriting(Feature * firstFeatureInSentence);
void closeCorpusFile();
void saveTextLineToCurrentCorpusFile(string sentenceText);
void saveTextToCurrentCorpusFile(string sentenceText);
bool loadCorpusFileSemanticDependencyRelations(Sentence * currentSentenceInList, Feature * firstFeatureInListorSubset);

string corpusDBgenerateFileName(Feature * firstFeatureInList);

#endif

#endif

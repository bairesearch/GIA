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
 * File Name: GIAsemanticParserDatabase.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3a6c 05-April-2017
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER_DATABASE
#define HEADER_GIA_SEMANTIC_PARSER_DATABASE

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAtranslatorDefs.hpp"	//required for GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES
#include "GIAdatabase.hpp"	//required for DBgenerateServerDatabaseName
#include "GIAnlp.hpp"
#include "SHAREDvars.hpp"	//required for writeStringToFileObject

#ifdef GIA_SEMANTIC_PARSER

#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
#define GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER (CHAR_SPACE)
#endif

#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAsemanticparserdatabase/")
#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME ((string)"SemanticRelations")
#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME ((string)".gsr")

#define GIA_SEMANTIC_PARSER_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES (25)
static string GIAconnectionistNetworkPOStypeNameAbbreviationArray[GIA_SEMANTIC_PARSER_POS_TYPE_NAME_ARRAY_NUMBER_OF_TYPES] = {"Un", "Cc", "Nu", "De", "Uk", "Po", "Pe", "Mo", "Pr", "Aj", "No", "Av", "Pp", "Ps", "Pa", "In", "Ve", "Wh", "Pd", "Pq", "Ab", "Ah", "Ad", "Di", "Sr"};


class GIAsemanticParserDatabaseClass
{
	private: GIAdatabaseClass GIAdatabase;
	private: SHAREDvarsClass SHAREDvars;
	private: GIAnlpClass GIAnlp;
	public: void initialiseSemanticParserDatabase(const string newSemanticParserDatabaseFolderName);
	public: void prepareSemanticParserDatabaseForWriting();
	private: string semanticParserDBgenerateFolderName(const GIAfeature* firstFeatureInList);
	#ifndef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	public: GIArelation* getFirstRelationInSemanticParserSentenceList();
	public: GIArelation* getCurrentRelationInSemanticParserSentenceList();
	private: void prepareSemanticParserSemanticRelationListForWriting();
	#endif

	#ifdef GIA_SEMANTIC_PARSER_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
	public: void prepareSemanticParserCorpusDatabaseFileTextForWriting();
	public: void saveTextLineToCorpusFileString(const string sentenceText);
	public: void saveTextToCorpusFileString(const string sentenceText);
	/*
	void removeTextFromCorpusFileString(string sentenceText);
	void removeTextLineFromCorpusFileString(string sentenceText);
	*/
	public: void writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence);
	public: void writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence, const string* sentenceSemanticRelationsText);
	public: bool loadSemanticParserCorpusDatabaseFile(GIAsentence* currentSentenceInList, const GIAfeature* firstFeatureInListorSubset);
	public: string semanticParserCorpusDBgenerateFileName(const GIAfeature* firstFeatureInList);
	private: string generateCorpusFileHeaderText(const GIAfeature* firstFeatureInSentence, const bool addPOSinfo);
	#endif

	#ifdef GIA_SEMANTIC_PARSER_SUBSETS_OPTIMISED_DATABASE
	#define SEMANTIC_RELATION_TYPE_NON_EXISTENT (INT_DEFAULT_VALUE)
	public: bool loadSemanticParserOptimisedDatabaseFile(const GIAfeature* firstFeatureInListorSubset, const int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment, int GIA2semanticDependencyRelationProbabilityArray[], int GIA2semanticDependencyRelationAssignedArray[], int GIA2semanticDependencyRelationRejectedArray[]);
	public: void writeSemanticParserOptimisedDatabaseFile(const GIAfeature* firstFeatureInListorSubset, const int firstWordInTupleIndex, const int semanticDependencyRelationType, const bool directionGovernorToDependent, const bool sameReferenceSet);
	public: void initialiseIntArray(int intArray[], const int size, int value);
	private: int calculateSemanticRelationTypeOptimisedDatabase(const int semanticDependencyRelationType, const bool directionGovernorToDependent, const bool sameReferenceSet);
	private: bool calculateSemanticRelationTypeDirection(const int semanticRelationTypeOptimisedDatabase);
	private: bool calculateSemanticRelationTypeSameReferenceSet(const int semanticRelationTypeOptimisedDatabase);
	private: int calculateSemanticRelationTypeFromOptimisedDatabaseSemanticRelationType(const int semanticRelationTypeOptimisedDatabase);
	private: string semanticParserOptimisedDBgenerateFileName(const GIAfeature* firstFeatureInList, const int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment);
	public: void addTupleSemanticRelationToSentence(GIAsentence* currentSentenceInList, const int semanticRelationTypeOptimisedDatabase, const int firstWordInTupleIndex, const int secondWordInTupleIndex);
	#endif
	public: void generateGIA2semanticDependencyRelationObject(GIArelation* GIA2semanticDependencyRelation, string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet);

};

#endif

#endif

/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is commercially licensed software. It may not be redistributed,
 * and may only be used by a registered licensee.
 *
 * BAIPROJECT is sold in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * BAI Commercial License for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You will have received a copy of the BAI Commercial License
 * along with BAIPROJECT. For more information, visit <http://www.baxterai.com>
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAsemanticParserDatabase.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2p3b 14-January-2017
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_SEMANTIC_PARSER_DATABASE
#define HEADER_GIA_SEMANTIC_PARSER_DATABASE

#include "GIAglobalDefs.h"
#include "GIAsentenceClass.h"
#include "GIAtranslatorDefs.h"	//required for GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES
#include "GIAdatabase.h"	//required for DBgenerateServerDatabaseName
#include "GIAnlp.h"
#include "SHAREDvars.h"	//required for writeStringToFileObject

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER

#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
#define GIA_SEMANTIC_PARSER_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER (CHAR_SPACE)
#endif

#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAsemanticparserdatabase/")
#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME ((string)"SemanticRelations")
#define GIA_SEMANTIC_PARSER_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME ((string)".txt")

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
#ifndef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD
	public: GIArelation* getFirstRelationInSemanticParserSentenceList();
	public: GIArelation* getCurrentRelationInSemanticParserSentenceList();
	private: void prepareSemanticParserSemanticRelationListForWriting();
#endif

#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS
#ifdef GIA_SAVE_SEMANTIC_RELATIONS_FOR_GIA2_SEMANTIC_PARSER_UNOPTIMISED_TEXT_CORPUS_OLD
	public: void prepareSemanticParserCorpusDatabaseFileTextForWriting();
	public: void saveTextLineToCorpusFileString(const string sentenceText);
	public: void saveTextToCorpusFileString(const string sentenceText);
/*
void removeTextFromCorpusFileString(string sentenceText);
void removeTextLineFromCorpusFileString(string sentenceText);
*/
	public: void writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence);
#else
	public: string generateSemanticParserCorpusSemanticRelationsText(const GIArelation* firstSemanticRelationInList);
#endif
	public: void writeSemanticParserCorpusFile(const GIAfeature* firstFeatureInSentence, const string* sentenceSemanticRelationsText);
	public: bool loadSemanticParserCorpusDatabaseFile(GIAsentence* currentSentenceInList, const GIAfeature* firstFeatureInListorSubset);
	public: string semanticParserCorpusDBgenerateFileName(const GIAfeature* firstFeatureInList);
#endif

#ifdef GIA2_SEMANTIC_PARSER_OPTIMISED_DATABASE
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
	public: void generateGIA2semanticDependencyRelationObject(GIArelation* GIA2semanticDependencyRelation, string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet);
#endif

};

#endif

#endif

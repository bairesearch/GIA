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
 * File Name: GIAsemRelTranslatorDatabase.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3o3a 16-November-2020
 * Requirements: requires text parsed by GIA2 Parser (Modified Stanford Parser format)
 * Description: Semantic Relation Translator
 * /
 *******************************************************************************/


#ifndef HEADER_GIA_SEM_REL_TRANSLATOR_DATABASE
#define HEADER_GIA_SEM_REL_TRANSLATOR_DATABASE

#include "GIAglobalDefs.hpp"
#include "GIAsentenceClass.hpp"
#include "GIAsemRelTranslatorDefs.hpp"	//required for GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE_SEMANTIC_RELATION_NUMBER_OF_TYPES
#include "GIAdatabase.hpp"	//required for DBgenerateServerDatabaseName
#include "GIAnlp.hpp"
#include "SHAREDvars.hpp"	//required for writeStringToFileObject


#ifdef GIA_SEM_REL_TRANSLATOR

#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
#define GIA_SEM_REL_TRANSLATOR_OPTIMISED_DATABASE_SEMANTIC_RELATION_COUNT_DELIMITER (CHAR_SPACE)
#endif

#define GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_DATABASE_NAME ((string)"GIAsemanticparserdatabase")
#define GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_NAME ((string)"SemanticRelations")
#define GIA_SEM_REL_TRANSLATOR_DATABASE_FILESYSTEM_DEFAULT_FILE_EXTENSION_NAME ((string)".gsr")

static string GIAconnectionistNetworkPOStypeNameAbbreviationArray[GIA_SEM_REL_TRANSLATOR_POS_TYPE_ARRAY_NUMBER_OF_TYPES] = {"Un", "Cc", "Nu", "De", "Uk", "Po", "Pe", "Mo", "Pr", "Aj", "No", "Av", "Pp", "Ps", "Pa", "In", "Ve", "Wh", "Pq", "Pd", "Po", "Ab", "Ah", "Ad", "Di", "Sr"};

#endif

class GIAsemRelTranslatorDatabaseClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: GIAdatabaseClass GIAdatabase;
	private: GIAnlpClass GIAnlp;
	
	#ifdef GIA_SEM_REL_TRANSLATOR
	
	public: void initialiseSemanticParserDatabase(string newSemanticParserDatabaseFolderName);
	public: void prepareSemanticParserDatabaseForWriting();
	private: string semanticParserDBgenerateFolderName(GIAfeature* firstFeatureInList);

	#ifdef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS
	public: void prepareSemanticParserCorpusDatabaseFileTextForWriting();
	public: void saveTextLineToCorpusFileString(string sentenceText);
	public: void saveTextToCorpusFileString(string sentenceText);
	/*
	void removeTextFromCorpusFileString(string sentenceText);
	void removeTextLineFromCorpusFileString(string sentenceText);
	*/
	public: void writeSemanticParserCorpusFile(GIAfeature* firstFeatureInSentence);
	public: void writeSemanticParserCorpusFile(GIAfeature* firstFeatureInSentence, string* sentenceSemanticRelationsText);
	public: bool loadSemanticParserCorpusDatabaseFile(GIAsentence* currentSentenceInList, GIAfeature* firstFeatureInListorSubset);
	public: string semanticParserCorpusDBgenerateFileName(GIAfeature* firstFeatureInList);
	private: string generateCorpusFileHeaderText(GIAfeature* firstFeatureInSentence, bool addPOSinfo);
	#endif

	#ifdef GIA_SEM_REL_TRANSLATOR_SUBSETS_OPTIMISED_DATABASE
	#define GIA_SEM_REL_TRANSLATOR_RELATION_TYPE_NON_EXISTENT (INT_DEFAULT_VALUE)
	public: bool loadSemanticParserOptimisedDatabaseFile(GIAfeature* firstFeatureInListorSubset, int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment, int GIA2semanticDependencyRelationProbabilityArray[], int GIA2semanticDependencyRelationAssignedArray[], int GIA2semanticDependencyRelationRejectedArray[]);
	public: void writeSemanticParserOptimisedDatabaseFile(GIAfeature* firstFeatureInListorSubset, int firstWordInTupleIndex, int semanticDependencyRelationType, bool directionGovernorToDependent, bool sameReferenceSet);
	public: void initialiseIntArray(int intArray[], int size, int value);
	private: int calculateSemanticRelationTypeOptimisedDatabase(int semanticDependencyRelationType, bool directionGovernorToDependent, bool sameReferenceSet);
	private: bool calculateSemanticRelationTypeDirection(int semanticRelationTypeOptimisedDatabase);
	private: bool calculateSemanticRelationTypeSameReferenceSet(int semanticRelationTypeOptimisedDatabase);
	private: int calculateSemanticRelationTypeFromOptimisedDatabaseSemanticRelationType(int semanticRelationTypeOptimisedDatabase);
	private: string semanticParserOptimisedDBgenerateFileName(GIAfeature* firstFeatureInList, int indexOfFirstWordInTupleBeingAssessedForSemanticRelationAssignment);
	public: void addTupleSemanticRelationToSentence(GIAsentence* currentSentenceInList, int semanticRelationTypeOptimisedDatabase, int firstWordInTupleIndex, int secondWordInTupleIndex);
	#endif
	
	#endif
	
	#ifdef GIA_SEM_REL_TRANSLATOR_COMMON
	public: void generateGIA2semanticDependencyRelationObject(GIArelation* GIA2semanticDependencyRelation, string entityName1, string entityName2, string semanticRelation, int entityIndex1, int entityIndex2, bool sameReferenceSet);
	#ifndef GIA_SEM_REL_TRANSLATOR_WRITE_SEMANTIC_RELATIONS_UNOPTIMISED_TEXT_CORPUS_OLD
	public: GIArelation* getFirstRelationInSemanticParserSentenceList();
	public: GIArelation* getCurrentRelationInSemanticParserSentenceList();
	private: void prepareSemanticParserSemanticRelationListForWriting();
	#endif
	#endif
	
};

#endif

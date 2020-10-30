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
 * File Name: GIAposRelTranslator.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 3n3a 29-October-2020
 * Requirements: requires plain text file
 * Description: Part-of-speech Relation Translator
 * /
 *******************************************************************************/


#include "GIAposRelTranslator.hpp"
//#include "XMLparserClass.hpp"


	
#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3


bool GIAposRelTranslatorClass::parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(GIAtranslatorVariablesClass* translatorVariables, const string inputTextPlainTXTfileName, string inputTextNLPrelationXMLfileName, const string inputTextNLPfeatureXMLfileName, const string outputCFFfileName)
{
	bool result = true;

	SANItranslatorVariablesClass SANItranslatorVariables;
	GIAposRelTranslatorParser.createSANItranslatorVariablesFromGIAtranslatorVariables(translatorVariables, &SANItranslatorVariables);
		
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SYN_REL_TRANSLATOR_FEATURES
	#ifdef STANFORD_PARSER_USE_POS_TAGS
	cout << "error: performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences{} doesn't support STANFORD_PARSER_USE_POS_TAGS (because the semantic relations word types being written must match those being read [and read can only use feature parser])" << endl;
	#endif
	bool createNewSentences = true;
	if(!GIAnlp.parseNLPparserFeaturesFile(inputTextNLPfeatureXMLfileName, translatorVariables->isQuery, translatorVariables->firstSentenceInList, translatorVariables->NLPfeatureParser, &createNewSentences))
	{
		result = false;
	}
	#else
	//generate empty sentence class list (without GIAfeatures):
	LRPpreprocessorSentence* currentLRPpreprocessorSentenceInList = translatorVariables->LRPpreprocessorTranslatorVariables.firstLRPpreprocessorSentenceInList;
	GIAsentence* currentSentenceInList = translatorVariables->firstSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		GIAsentence* newSentence = new GIAsentence();
		newSentence->previous = currentSentenceInList;
		currentSentenceInList->next = newSentence;

		/*
		cout << "currentLRPpreprocessorSentenceInList = " << endl;
		LRPpreprocessorWordClassObject.printWordList(LRPpreprocessorSentenceClassObject.getSentenceContents(currentLRPpreprocessorSentenceInList));
		*/
		
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
		currentSentenceInList = currentSentenceInList->next;
	}	
	#endif
	
	
	SANIposRelTranslator.parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences(&SANItranslatorVariables);

	/*
	currentLRPpreprocessorSentenceInList = translatorVariables->LRPpreprocessorTranslatorVariables.firstLRPpreprocessorSentenceInList;
	while(currentLRPpreprocessorSentenceInList->next != NULL)
	{
		cout << "currentLRPpreprocessorSentenceInList->sentenceIndexOriginal = " << currentLRPpreprocessorSentenceInList->sentenceIndexOriginal << endl;
		currentLRPpreprocessorSentenceInList = currentLRPpreprocessorSentenceInList->next;
	}
	*/
		
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	if(!GIAposRelTranslatorParser.generateSemanticRelationsFromTxtRelationsWrapperSentences(translatorVariables))
	{
		result = false;
	}	
	/*
	#ifdef GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
	string originalInputFileName = "";
	originalInputFileName = inputTextPlainTXTfileName;
	GIAnlp.outputInternalRelationsInRelexFormat(&outputCFFfileName, &originalInputFileName, translatorVariables->firstSentenceInList, translatorVariables->NLPdependencyRelationsParser, translatorVariables->NLPfeatureParser, translatorVariables->NLPexeFolderArray);
	#endif	
	*/
	#endif
	
	#ifdef GIA_POS_REL_TRANSLATOR_HYBRID	//ie !GIA_POS_REL_TRANSLATOR_RULES_GIA3
	//parse GIA syntactical relations (NOT currently supported)
	SHAREDvars.setCurrentDirectory(outputFolder);
	bool onlyParseIfCorpusLookupFailed = false;
	if(!GIAnlp.parseNLPparserRelationsFile(inputTextNLPrelationXMLfileName, translatorVariables->isQuery, translatorVariables->firstSentenceInList, translatorVariables->NLPdependencyRelationsParser, translatorVariables->NLPrelexCompatibilityMode, createNewSentences, onlyParseIfCorpusLookupFailed))
	{
		result = false;
	}
	#else
	inputTextNLPrelationXMLfileName = "";				//irrelevant (not used)
	translatorVariables->NLPdependencyRelationsParser = GIA_NLP_PARSER_STANFORD_PARSER;	//irrelevant (not used) - always set to Stanford Parser (as a standard parser type file is used to store GIA2 semantic dependency relations)
	translatorVariables->NLPrelexCompatibilityMode = false; 		//irrelevant (not used) - only used when parsing syntatic dependency relations of a Relex file, and performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences{} does not parse any syntactic dependency relations
	translatorVariables->NLPassumePreCollapsedStanfordRelations = false;	//irrelevant (not used) - only used when executing convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}, and performSemanticParserLookupAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences{} does not execute convertSentenceSyntacticRelationsIntoGIAnetworkNodes{}
	#endif
	
	/*
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_GIA3_USE_SEM_REL_TRANSLATOR_PARSER
	//no longer required; 
	translatorVariables->parseGIA2file = true;
	translatorVariables->currentSentenceInList->semanticParserSuccessful = true;
	#endif
	*/
	
	if(!GIAtranslator.parseNLPparserFileAndCreateSemanticNetworkBasedUponDependencyParsedSentences(translatorVariables, inputTextNLPrelationXMLfileName, inputTextNLPfeatureXMLfileName, outputCFFfileName))	//inputTextNLPrelationXMLfileName/inputTextNLPfeatureXMLfileName/NLPfeatureParser/NLPdependencyRelationsParser/NLPrelexCompatibilityMode/NLPassumePreCollapsedStanfordRelations not used (relations and features have already been parsed)
	{
		result = false;
	}	


	return result;
}


#endif


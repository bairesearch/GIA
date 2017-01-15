/*******************************************************************************
 *
 * File Name: GIAnlp.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1j4a 28-Apr-2012
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_NLP
#define HEADER_GIA_NLP

#include "GIAglobalDefs.h"
#include "GIASentenceClass.h"

#define Relex_CFF_XML_TAG_nlparse ((string)"nlparse")
#define Relex_CFF_XML_TAG_paragraph ((string)"paragraph")
#define Relex_CFF_XML_TAG_sentence ((string)"sentence")
#define Relex_CFF_XML_TAG_parse ((string)"parse")
#define Relex_CFF_XML_TAG_relations ((string)"relations")
#define Relex_CFF_XML_TAG_features ((string)"features")


#define StanfordCoreNLP_XML_TAG_root ((string)"root")
	#define StanfordCoreNLP_XML_TAG_document ((string)"document")
		#define StanfordCoreNLP_XML_TAG_sentences ((string)"sentences")
			#define StanfordCoreNLP_XML_TAG_sentence ((string)"sentence")
			#define StanfordCoreNLP_XML_ATTRIBUTE_id ((string)"id")	
				#define StanfordCoreNLP_XML_TAG_tokens ((string)"tokens")
					#define StanfordCoreNLP_XML_TAG_token ((string)"token")
						#define StanfordCoreNLP_XML_TAG_word ((string)"word")
						#define StanfordCoreNLP_XML_TAG_lemma ((string)"lemma")
						#define StanfordCoreNLP_XML_TAG_CharacterOffsetBegin ((string)"CharacterOffsetBegin")
						#define StanfordCoreNLP_XML_TAG_CharacterOffsetEnd ((string)"CharacterOffsetEnd")
						#define StanfordCoreNLP_XML_TAG_POS ((string)"POS")
						#define StanfordCoreNLP_XML_TAG_NER ((string)"NER")
						#define StanfordCoreNLP_XML_TAG_NormalizedNER ((string)"NormalizedNER")
						#define StanfordCoreNLP_XML_TAG_Timex ((string)"Timex")
				#define StanfordCoreNLP_XML_TAG_parse ((string)"parse")
				#define StanfordCoreNLP_XML_TAG_basicdependencies ((string)"basic-dependencies")
				#define StanfordCoreNLP_XML_TAG_collapseddependencies ((string)"collapsed-dependencies")
				#define StanfordCoreNLP_XML_TAG_collapsedccprocesseddependencies ((string)"collapsed-ccprocessed-dependencies")
					#define StanfordCoreNLP_XML_TAG_dep ((string)"dep")
					#define StanfordCoreNLP_XML_ATTRIBUTE_idx ((string)"idx")			
						#define StanfordCoreNLP_XML_TAG_governor ((string)"governor")
						#define StanfordCoreNLP_XML_TAG_dependent ((string)"dependent")				
		#define StanfordCoreNLP_XML_TAG_coreferences ((string)"coreference")			//NB this apppears to be a bug in the StanfordCoreNLP code (NB it should be called "coreferences"
			#define StanfordCoreNLP_XML_TAG_coreference ((string)"coreference")
				#define StanfordCoreNLP_XML_TAG_mention ((string)"mention")
				#define StanfordCoreNLP_XML_ATTRIBUTE_representative ((string)"representative")			
					#define StanfordCoreNLP_XML_TAG_sentence ((string)"sentence")
					#define StanfordCoreNLP_XML_TAG_start ((string)"start")		//this may not be important
					#define StanfordCoreNLP_XML_TAG_end ((string)"end")		//this may not be important
					#define StanfordCoreNLP_XML_TAG_head ((string)"head")		//sentence entity ID of reference

#define StanfordCoreNLP_numberOfDependencySetsPerSentence (3)
#define StanfordCoreNLP_dependencySet_basicdependencies (0)
#define StanfordCoreNLP_dependencySet_collapseddependencies (1)
#define StanfordCoreNLP_dependencySet_collapsedccprocesseddependencies (2)
#define StanfordCoreNLP_dependencySet_usedByGIA (StanfordCoreNLP_dependencySet_collapsedccprocesseddependencies)

#define StanfordCoreNLPdefaultOutputFileExtensionAppend ".xml"

static string StanfordCoreNLP_relationSetNameArray[StanfordCoreNLP_numberOfDependencySetsPerSentence] = {StanfordCoreNLP_XML_TAG_basicdependencies, StanfordCoreNLP_XML_TAG_collapseddependencies, StanfordCoreNLP_XML_TAG_collapsedccprocesseddependencies};

				
void executeNLPparser(string inputTextPlainTXTFileName, string inputTextNLPrelationXMLFileName, int NLPParser, string NLPexeFolderArray[]);

bool parseNLPParserFile(string inputTextNLPrelationXMLFileName, string inputTextNLPfeatureXMLFileName, bool isQuery, Paragraph * firstParagraphInList, int NLPfeatureParser, int NLPdependencyRelationsParser, bool NLPrelexCompatibilityMode);
	#ifdef GIA_USE_RELEX
	bool parseRelexFile(string inputTextNLPrelationXMLFileName, bool isQuery, Paragraph * firstParagraphInList, bool parseRelations, bool parseFeatures, bool NLPrelexCompatibilityMode, bool createNewSentences);
	#endif
	#ifdef GIA_USE_STANFORD_CORENLP
	bool parseStanfordCoreNLPFile(string inputTextNLPrelationXMLFileName, bool isQuery, Paragraph * firstParagraphInList, bool parseRelations, bool parseFeatures, bool createNewSentences);
	#endif
	#ifdef GIA_USE_STANFORD_PARSER
	bool parseStanfordParserFile(string inputTextNLPrelationXMLFileName, bool isQuery, Paragraph * firstParagraphInList, bool createNewSentences);
	#endif

#endif

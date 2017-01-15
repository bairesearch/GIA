/*******************************************************************************
 *
 * File Name: GIAmain.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2011 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 1b10b 28-Sept-11
 * Requirements: requires text parsed by RelEx (available in .CFF format <relations>)
 *
 *******************************************************************************/


#ifndef HEADER_GIA_MAIN
#define HEADER_GIA_MAIN

#include "GIAEntityNodeClass.h"
 
int main(int argc,char **argv);

string printEntityNode(GIAEntityNode * queryAnswerNode);

//#define GIA_XML_DEBUG_TEST_WRITE_READ_WRITE

#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.sh"
//OLD: #define GIA_RELEX_EXECUTABLE_RELATIVE_PATH_NAME "/relex-1.3.0/"	//gets added to executable path name

vector<GIAEntityNode*> * parseRelexFile(string inputRelexXMLFileName);

#endif

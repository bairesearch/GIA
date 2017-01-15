/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *r
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: GIAglobalsDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2013 Baxter AI (baxterai.com)
 * Project: General Intelligence Algorithm
 * Project Version: 2a4a 09-November-2013
 * Requirements: requires text parsed by NLP Parser (eg Relex; available in .CFF format <relations>)
 * Description: GIA specific global definitions
 *
 *
 * 		Dependencies Introduction
 *		=================================
 *
 *		The source for OpenGIA is written in ISO C++, some of which may require ISO/IEC 14882:2011 (C++11).
 *
 *		OpenGIA requires the following software to be pre-installed;
 *
 *		OpenGIA Execution Dependencies Installation
 *		=================================
 *
 *		Install External Package 1 - Freeglut
 *		--------------------------------------
 *
 *			(Linux EL5 Only)
 *			rpm -i freeglut-2.4.0-7.1.el5.x86_64.rpm
 *
 *			(Linux EL6 Only)
 *			rpm -i freeglut-2.6.0-1.el6.x86_64.rpm
 *
 *			(Windows XP Only)
 *			unzip freeglut 2.4.0 (compiled).zip
 *			copy freeglut.dll from freeglut 2.4.0 (compiled).zip to c:\Windows\System32
 *
 *			(Windows 7 x86_64 Only)
 *			unzip freeglut 2.6.0 (compiled).zip
 *			copy freeglut.dll from freeglut 2.6.0 (compiled) - 32bit.zip to: C:\WINDOWS\SysWOW64
 *			[or System32]
 *
 *
 *		Freeglut may be downloaded via;
 *
 *		* (Linux) yum install freeglut
 *		* (Windows) see http://tempvariable.blogspot.com.au/2008/02/installing-freeglut-on-visual-studio.html / http://www.transmissionzero.co.uk/software/freeglut-devel/
 *
 *
 *		Install External Package 2 - Wordnet development libraries
 *		--------------------------------------
 *
 *			(Linux EL5 Only)
 *			Use preinstalled EL5 Linux JPEG libraries;
 *			rpm -i wordnet-3.0-13.el6.x86_64.rpm
 *
 *			(Linux EL6 Only)
 *			Use preinstalled EL5 Linux JPEG libraries;
 *			rpm -i wordnet-3.0-13.el5.x86_64.rpm
 *
 *			(Windows XP Only {UNTESTED})
 *			Install WordNet_3.0_win32-setup.exe (from Google Code)
 *
 *			(Windows 7 x86_64 Only)
 *			Install WordNet_3.0_win32-setup.exe (from Google Code)
 *
 *		WordNet may be downloaded via;
 *
 *		* (Linux) yum install wordnet
 *		* (Windows) Download WordNet 3.0 for Windows from http://code.google.com/p/wordnet3p0forwindows
 *
 *
 *		Install External Package 3a - Relex [OPTIONAL] {Linux Only}
 *		--------------------------------------
 *
 *		Create/Copy execute-relex.sh and place it in the Relex binary folder.
 *
 *		NB execute-relex.sh contains the following text;
 *
 *			export LANG=en_US.UTF-8
 *
 *			VM_OPTS="-Xmx1024m"
 *
 *			RELEX_OPTS="\
 *				-Djava.library.path=/usr/lib:/usr/local/lib \
 *				-Drelex.algpath=data/relex-semantic-algs.txt \
 *				-Dwordnet.configfile=data/wordnet/file_properties.xml \
 *				"
 *
 *			CLASSPATH="-classpath \
 *			bin:\
 *			/usr/local/share/java/opennlp-tools-1.4.3.jar:\
 *			/usr/local/share/java/opennlp-tools-1.3.0.jar:\
 *			/usr/local/share/java/maxent-2.5.2.jar:\
 *			/usr/local/share/java/maxent-2.4.0.jar:\
 *			/usr/local/share/java/trove.jar:\
 *			/usr/local/share/java/jwnl.jar:\
 *			/usr/local/share/java/jwnl-1.3.3.jar:\
 *			/usr/share/java/commons-logging.jar:\
 *			/usr/share/java/gnu-getopt.jar:\
 *			/usr/local/share/java/linkgrammar.jar:\
 *			/usr/share/java/linkgrammar.jar:\
 *			/usr/share/java/xercesImpl.jar:\
 *			/usr/share/java/xml-apis.jar:\
 *			/opt/GATE-4.0/lib/PDFBox-0.7.2.jar:\
 *			#/opt/GATE-4.0/bin/gate.jar:\
 *			/opt/GATE-4.0/lib/jdom.jar:\
 *			/opt/GATE-4.0/lib/jasper-compiler-jdt.jar:\
 *			/opt/GATE-4.0/lib/nekohtml-0.9.5.jar:\
 *			/opt/GATE-4.0/lib/ontotext.jar:\
 *			"
 *
 *		Relex may be downloaded (and installed) via;
 *
 *
 *		1. 	Install Java Development Tools (eg openjdk and openjdk-devel)
 *
 *				Redhat/ELX example;
 *
 *				yum install java-1.6.0-openjdk-devel
 *				yum install java-1.6.0-openjdk
 *					OR (EL5)
 *				rpm -i java-1.6.0-openjdk-devel.el5.x86_64.rpm
 *				rpm -i java-1.6.0-openjdk.el5.x86_64.rpm
 *					OR (EL6)
 *				java-1.6.0-openjdk-devel-1.6.0.0-1.36.b17.el6_0.x86_64.rpm	/ java-1.6.0-openjdk-devel-1.6.0.0-1.39.b17.el6_0.x86_64.rpm
 *				java-1.6.0-openjdk-1.6.0.0-1.36.b17.el6_0.x86_64.rpm		/ java-1.6.0-openjdk-1.6.0.0-1.39.b17.el6_0.x86_64.rpm
 *
 *
 *		2. 	Install Link Parser
 *			(download: http://www.link.cs.cmu.edu/link/
 *			alternate: http://www.abisource.com/projects/link-grammar/#download)
 *
 *				Redhat/EL5 example;
 *
 *				tar -xvf link-grammar-4.7.2.tar.gz
 *				cd link-grammar-4.7.2
 *				export JAVA_HOME=/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64
 *				./configure
 *				make
 *				su root
 *				make install
 *
 *
 *		3. 	Install WordNet dependencies (eg tcl and tk)
 *
 *				Redhat/EL5 example;
 *
 *				yum install tcl
 *				yum install tcl-devel
 *				yum install tk
 *				yum install tk-devel
 *					OR
 *				rpm -i tcl-8.4.13-4.el5.i386.rpm
 *				rpm -i tcl-devel-8.4.13-4.el5.x86_64.rpm
 *				rpm -i tcl-devel-8.4.13-4.el5.i386.rpm
 *				rpm -i tk-8.4.13-5.el5_1.1.i386.rpm
 *				rpm -i tk-devel-8.4.13-5.el5_1.1.x86_64.rpm
 *				rpm -i tk-devel-8.4.13-5.el5_1.1.i386.rpm
 *
 *
 *		4.	Install WordNet
 *
 *				Redhat/EL6 example;
 *
 *				rpm -i epel-release...
 *				yum install wordnet wordnet-devel
 *					OR
 *				rpm -i wordnet-3.0-13.el6.x86_64.rpm wordnet-devel-3.0-13.el6.x86_64.rpm
 *
 *		5.	Install JWNL Java wordnet library
 *			(download: http://sourceforge.net/projects/jwordnet/)
 *
 *				Redhat/EL5 example;
 *
 *				unzip jwnl14-rc2.zip
 *				cd jwnl14-rc2
 *				su root
 *				cp jwnl.jar /usr/share/java/		[see relex-1.4.0/build.xml for location]
 *				mkdir /usr/local/share/java/
 *				cp jwnl.jar /usr/local/share/java/	[see relex-1.4.0/batch-process.sh for location]
 *				chmod 777 /usr/share/java/jwnl.jar
 *				chmod 777 /usr/local/share/java/jwnl.jar
 *
 *
 *		6. 	Install Relex Dependencies (ant)
 *
 *				Redhat/EL5 example;
 *
 *				yum install ant
 *					OR [EL5]
 *				rpm -i ant-1.6.5-2jpp.2.x86_64.rpm
 *					OR [EL6]
 *				ant-1.7.1-13.el6.x86_64.rpm
 *				xml-commons-apis-1.3.04-3.6.el6.x86_64.rpm
 *		  		xml-commons-resolver-1.1-4.18.el6.x86_64.rpm
 *		  		xerces-j2-2.7.1-12.5.el6.x86_64.rpm
 *
 *
 *		7. 	Install Gate 4.0 [need to login as root at least one time in the past before able to run this from root]
 *			(download: http://gate.ac.uk/download/)
 *
 *			Redhat/EL5 example;
 *
 *			su root
 *			java -jar gate-4.0-build2752-installer-other.jar
 *				run through GUI;
 *				set installation path to /opt/GATE-4.0
 *				tick sources and development documentation
 *				install
 *
 *
 *		8. 	Install GNU getopt
 *			(download: http://www.java2s.com/Code/Jar/GHI/Downloadgetoptjar.htm)
 *
 *			Redhat/EL5 example;
 *
 *			unzip getopt.jar.zip
 *			cd getopt.jar.zip_FILES
 *			su root
 *			cp getopt.jar /usr/share/java/gnu-getopt.jar
 *
 *			For Ubuntu, install the "libgetopt-java" package instead.
 *
 *
 *		9.	Install opennlp (optional)
 *			(download: http://sourceforge.net/projects/opennlp/)
 *
 *				Redhat/EL5 example;
 *
 *				tar -xvf opennlp-tools-1.4.3.tgz
 *				cd opennlp-tools-1.4.3
 *				su root
 *				export JAVA_HOME=/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64
 *				./build.sh
 *				cp output/opennlp-tools-1.4.3.jar /usr/local/share/java/
 *				cp lib/maxent-2.5.2.jar /usr/local/share/java/
 *				cp lib/jwnl-1.3.3.jar /usr/local/share/java/
 *				cp lib/jwnl-1.3.3.jar /usr/local/share/java/jwnl.jar
 *				cp lib/trove.jar /usr/local/share/java/
 *
 *
 *		10. 	Install Relex
 *			(download: http://sourceforge.net/projects/relex/
 *			alternate: https://launchpad.net/relex/+download)
 *
 *				Redhat/EL5 example;
 *
 *				tar -xvf relex-1.4.0.tar.gz
 *				cd relex-1.4.0
 *				export JAVA_HOME=/usr/lib/jvm/java-1.6.0-openjdk-1.6.0.0.x86_64
 *				ant
 *				./batch-process.sh
 *
 *						Debugging Diagnosis;
 *
 *						NB1;
 *						[rich@localhost relex-1.4.0]$
 *						[rich@localhost relex-1.4.0]$ ./batch-process.sh
 *						Info: Using file defined in wordnet.configfile:data/wordnet/file_properties.xml
 *						Exception in thread "main" java.lang.NoClassDefFoundError: org/apache/commons/logging/LogFactory
 *							SOLUTION;
 *							yum install ant-commons-logging
 *								Installing : ant-nodeps-1.7.1-13.el6.x86_64                               1/3
 *								Installing : jakarta-commons-logging-1.0.4-10.el6.noarch                  2/3
 *								Installing : ant-commons-logging-1.7.1-13.el6.x86_64                      3/3
 *
 *						NB2;
 *						[javac] /home/rich/soft/BAISource/relex/relex-1.4.0/src/java/relex/parser-old/LinkParserClient.java:21: package org.linkgrammar does not exist
 *						[javac] import org.linkgrammar.LinkGrammar;
 *							OR
 *						Exception in thread "main" java.lang.NoClassDefFoundError: net/didion/jwnl/JWNLException
 *							SOLUTION; reinstall 2. 	reinstall Link Parser [ensure Java plugins are compiled correctly]
 *
 *								see also;  http://groups.google.com/group/link-grammar/browse_thread/thread/2b1c863b8bf4976f
 *									Indeed, everything got fixed and I have Relex running correctly. There
 *									were a few things that needed to be done, but essentially, I had to
 *									manually input the path to jwnl in the Relex parameters, both when
 *									compiling, and when specifying libs to be used in the .sh scripts.
 *									After that, be sure you have the JAVA_HOME environment variable set,
 *									and Relex should work wonderfully.
 *
 *		11.	Configure relex xml files for wordnet
 *
 *				Redhat/EL5 example;
 *
 *				cd relex-1.4.0/data/wordnet			[see relex/batch-process.sh for location]
 *				gedit file_properties.xml
 *				change dictionary_path to <param name="dictionary_path" value="/usr/share/wordnet-3.0/dict"/>
 *
 *
 *		12.	Configure relex batch-process.sh/***.sh class paths;
 *
 *				Redhat/EL5 example;
 *
 *				cd relex-1.4.0
 *				gedit batch-process.sh [append/modify the following]
 *					/usr/local/share/java/jwnl-1.3.3.jar:\
 *					/usr/share/java/xml-apis.jar:\
 *					/opt/GATE-4.0/lib/PDFBox-0.7.2.jar:\
 *
 *
 *
 *
 *		Install External Package 3b - Stanford Parser [OPTIONAL] {Linux/Windows}
 *		--------------------------------------
 *
 *		Create/Copy execute-stanfordParser.sh (Linux) / execute-stanfordParser.bat (Windows) and place it in the Stanford Parser binary folder.
 *
 *
 *		NB execute-stanfordParser.sh (#!/usr/bin/env bash) contains the following text;
 *
 *			scriptdir=`dirname $0`
 *			java -mx2000m -cp "$scriptdir/*:" edu.stanford.nlp.parser.lexparser.LexicalizedParser -outputFormat "wordsAndTags,penn,typedDependencies" edu/stanford/nlp/models/lexparser/englishPCFG.ser.gz $3/$1 > $4/$2
 *
 *
 *		NB execute-stanfordParser.bat contains the following text;
 *
 *			@echo off
 *			:: Runs the English PCFG parser on one or more files, printing trees only
 *			:: usage: lexparser fileToparse
 *			"C:\Program Files\Java\jre7\bin\java.exe" -d64 -mx150m -cp "*;" edu.stanford.nlp.parser.lexparser.LexicalizedParser -outputFormat "wordsAndTags,penn,typedDependencies" edu/stanford/nlp/models/lexparser/englishPCFG.ser.gz %3/%1 > %4/%2
 *
 *
 *		Stanford Parser may be downloaded via;
 *
 *		(Linux and Windows) http://nlp.stanford.edu/software/stanford-parser-2012-03-09.tgz
 *
 *
 *		Install External Package 3c - Stanford CoreNLP [OPTIONAL] {Linux/Windows}
 *		--------------------------------------
 *
 *		Create/Copy execute-stanfordCoreNLP.sh (Linux) / execute-stanfordCoreNLP.bat (Windows) and place it in the Stanford CoreNLP binary folder.
 *
 *
 *		NB execute-stanfordCoreNLP.sh contains the following text;
 *
 *			java -cp stanford-corenlp-2012-04-03.jar:stanford-corenlp-2012-03-09-models.jar:xom.jar:joda-time.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file $3/$1 -outputDirectory $4 -outputExtension $5
 *
 *
 *		NB execute-stanfordCoreNLP.bat contains the following text;
 *
 *			"C:\Program Files\Java\jre7\bin\java.exe" -d64 -cp stanford-corenlp-2012-04-03.jar;stanford-corenlp-2012-03-09-models.jar;xom.jar;joda-time.jar -Xmx3g edu.stanford.nlp.pipeline.StanfordCoreNLP -annotators tokenize,ssplit,pos,lemma,ner,parse,dcoref -file %3/%1 -outputDirectory %4 -outputExtension %5
 *
 *
 *		Stanford CoreNLP may be downloaded via;
 *
 *		(Linux and Windows) http://nlp.stanford.edu/software/stanford-corenlp-2012-04-03.tgz
 *
 *
 *
 *		OpenGIA Development Dependencies Installation
 *		=================================
 *
 *		Compile/Link External Package 1 - Freeglut development libraries
 *		--------------------------------------
 *
 *			(Linux EL5 Only)
 *			rpm - i freeglut-devel-2.4.0-7.1.el5.x86_64.rpm
 *
 *			(Linux EL6 Only)
 *			rpm -i freeglut-devel-2.6.0-1.el6.x86_64.rpm
 *
 *			(Windows XP Only);
 *			unpack source code freeglut 2.4.0 (compiled).zip
 *			copy freeglut.h to: C:\Program Files\Microsoft Visual Studio 9.0\VC\include\GL\
 *			(note: you'll have to create the GL folder)
 *			copy freeglut_ext.h to: C:\Program Files\Microsoft Visual Studio 9.0\VC\include\GL\
 *			copy freeglut_std.h to: C:\Program Files\Microsoft Visual Studio 9.0\VC\include\GL\
 *			copy freeglut.lib to: C:\Program Files\Microsoft Visual Studio 9.0\VC\lib\
 *			copy freeglut.dll to: C:\WINDOWS\system32\
 *
 *			(Windows 7 x86_64 Only);
 *			unpack source code freeglut 2.6.0 (compiled) - 32bit.zip
 *			copy freeglut.h to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\
 *			(note: you'll have to create the GL folder)
 *			copy freeglut_ext.h to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\
 *			copy freeglut_std.h to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL\
 *			copy freeglut.lib to: C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib\
 *			copy freeglut.dll to: C:\WINDOWS\SysWOW64 [or System32]
 *
 *
 *		Freeglut may be downloaded via;
 *
 *		* (Linux) yum install freeglut-devel
 *		* (Windows) see http://tempvariable.blogspot.com.au/2008/02/installing-freeglut-on-visual-studio.html / http://www.transmissionzero.co.uk/software/freeglut-devel/
 *
 *		Compile/Link External Package 2 - Wordnet development libraries
 *		--------------------------------------
 *
 *			(Linux EL5 Only)
 *			Use preinstalled EL5 Linux JPEG libraries;
 *			rpm -i wordnet-devel-3.0-13.el6.x86_64.rpm
 *
 *			(Linux EL6 Only)
 *			Use preinstalled EL5 Linux JPEG libraries;
 *			rpm -i wordnet-devel-3.0-13.el5.x86_64.rpm
 *
 *			(Windows XP Only {UNTESTED})
 *			Install WordNet_3.0_win32-setup.exe (from Google Code)
 *			copy wn.lib to C:\Program Files\Microsoft Visual Studio 9.0\VC\lib\
 *			copy wn.h to C:\Program Files\Microsoft Visual Studio 9.0\VC\Include\
 *			ensure wn.lib is added to MS Visual Studio OR project - Linker - additional dependencies
 *
 *			(Windows 7 x86_64 Only)
 *			Install WordNet_3.0_win32-setup.exe (from Google Code)
 *			copy wn.lib to C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib
 *			copy wn.h to C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\Include\
 *			ensure wn.lib is added to MS Visual Studio OR project - Linker - additional dependencies
 *
 *			To Compile Wordnet 3.0 in Visual Studio 2010 (to create wn.lib);
 *			Install WordNet_3.0_win32-setup.exe (from Google Code])
 *			extract C:\Program Files (x86)\WordNet\3.0\src\wn source project.zip
 *			wn project - properties - General - Common Language Runtime Support - No Common Language Runtime Support
 *			wn project - properties - General - Static Library (.lib)
 *			Build All
 *
 *		WordNet may be downloaded via;
 *
 *		* (Linux) yum install wordnet-devel
 *		* (Windows) Download WordNet 3.0 for Windows from http://code.google.com/p/wordnet3p0forwindows
 *
 *
 *
 *		OpenGIA Compilation
 *		=================================
 *
 *		Compile OpenGIA.exe
 *
 *			(Linux Only)
 *			open SHAREDglobalDefs.h,
 *				ensure #define LINUX is added
 *				ensure only #define COMPILE_GIA is uncommented
 *			copy makefile.GIA makefile
 *			./clear.bat (rm *.o)
 *			make
 *
 *			(Windows Only)
 *			open SHAREDglobalDefs.h
 *				ensure #define LINUX is commented out
 *				ensure only #define COMPILE_GIA is uncommented
 *			Open GIA.sln
 *			Build All
 *
 *		*Important preprocessor definitions to consider before compilation;*
 *
 *			#define LINUX
 *
 *
 *		Usage Examples;
 *		---------------------------------
 *
 *		Examples using NLP input from Relex;
 *		./OpenGIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlprelation 0 -notshow
 *		./OpenGIA.exe -itxt inputText.txt -oall semanticNet -nlprelation 0 -nlpfeature 0 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -notshow
 *		Example using NLP input from Stanford Core NLP;
 *		./OpenGIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlprelation 1 -notshow
 *		Example using NLP input from Stanford Parser;
 *		./OpenGIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-2012-03-09" -nlprelation 2 -notshow
 *
 *		Additional example where relations + features are parsed from different NLP files;
 *		./OpenGIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelation 2 -nlpfeature 1 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-2012-03-09" -notshow
 *		./OpenGIA.exe -itxt inputText.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-2012-03-09" -notshow
 *		OpenGIA.exe -itxt inputText.txt -oxml semanticNet.xml -ocxl semanticNet.cxl -osvg semanticNet.svg -oldr semanticNet.ldr -oppm semanticNet.ppm -nlprelation 2 -nlpfeature 1 -nlprelexfolder "E:/Files/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "E:/Files/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "E:/Files/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -notshow
 *		OpenGIA.exe -itxt inputText.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelexfolder "E:/Files/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "E:/Files/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "E:/Files/soft/BAISource/stanford/parser/stanford-parser-2012-03-09" -notshow
 *
 *		Additional example where relations + features parsed from different NLP file, queries parsed using different NLP file;
 *		./OpenGIA.exe -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 0 -nlpfeatureq 0 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-2012-03-09" -notshow
 *		[Relex not supported in Windows]
 *
 *		Additional example where relations + features parsed from different NLP file, queries parsed using same/corresponding NLP files;
 *		./OpenGIA.exe -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 2 -nlpfeatureq 1 -nlprelexfolder "/home/systemusername/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "/home/systemusername/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "/home/systemusername/stanford/parser/stanford-parser-2012-03-09" -notshow
 *		OpenGIA.exe -itxt inputText.txt -itxtq inputTextQuery.txt -oall semanticNet -nlprelation 2 -nlpfeature 1 -nlprelationq 2 -nlpfeatureq 1 -nlprelexfolder "E:/Files/soft/BAISource/relex/relex-1.4.0" -nlpstanfordcorenlpfolder "E:/Files/soft/BAISource/stanford/coreNLP/stanford-corenlp-2012-04-03" -nlpstanfordparserfolder "E:/Files/soft/BAISource/stanford/parser/stanford-parser-2012-03-09"
 *
 *
 *		Filesystem Database Configuration;
 *		---------------------------------
 *
 *		The GIA Database (KB) is a (Linux) filesystem structure of the format:
 *			//server/database/conceptEntityNodesLoadedList.txt
 *			//server/database/w/o/r/word1/0/0/0/entity.ent [instance0 is the concept associated with word1]
 *			//server/database/w/o/r/word1/0/0/0/[referencesOfInstance0].ref [instance0 is the concept associated with word1]
 *			//server/database/w/o/r/word1/0/0/1/entity.ent
 *			//server/database/w/o/r/word1/0/0/1/[referencesOfInstance1].ref
 *			//server/database/w/o/r/word1/0/0/2/entity.ent
 *			//server/database/w/o/r/word1/0/0/2/[referencesOfInstance2].ref
 *			//server/database/w/o/r/word2/0/0/0/entity.ent [instance0 is the concept associated with word2]
 *			//server/database/w/o/r/word2/0/0/0/[referencesOfInstance0].ref [instance0 is the concept associated with word2]
 *			//server/database/w/o/r/word2/0/0/1/entity.ent
 *			//server/database/w/o/r/word2/0/0/1/[referencesOfInstance1].ref etc
 *
 *
 *******************************************************************************/

#ifndef HEADER_GIA_GLOBAL_DEFS
#define HEADER_GIA_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"

//variables currently being tested (3a4a+)
#define GIA_RELEX_FIX_DO_NOT_ASSIGN_DEFINITE_IF_UNCOUNTABLE

//variables currently being tested (2a1a+)
#define GIA_TRANSLATOR_SUPPORT_CSUB_AND_COP_GENERATE_ADJECTIVES 

//variables currently being tested (1q1a+)
#define GIA_TRANSLATOR_DREAM_MODE_LINK_SPECIFIC_CONCEPTS_AND_ACTIONS	//required for NLPI
#define GIA_QUERY_SIMPLIFIED_SEARCH
#define GIA_QUERY_SIMPLIFIED_SEARCH_REPLACE_ADVANCED_SEARCH
//#define GIA_QUERY_SIMPLIFIED_SEARCH_ENFORCE_EXACT_MATCH

//variables currently being tested (1t1a+)
#define GIA_TRANSLATOR_XML_INTERPRETATION
//#define GIA_TEMPORARY_COMPILATION_GCC_FLAG_E_PREPROCESS_TRANSLATOR_XML_FILES	//for use with compileGIAstopAfterPreprocessingXML.bat only: g++ -E GIAmain.cpp GIAparser.cpp GIAtranslator.cpp GIAtranslatorDefineGrammar.cpp GIAtranslatorDefineReferencing.cpp GIAtranslatorRedistributeStanfordRelations.xml GIAtranslatorRedistributeRelexRelations.xml GIAtranslatorDefineSubstances.xml GIAtranslatorLinkEntities.xml GIAtranslatorApplyAdvancedFeatures.xml GIAtranslatorOperations.cpp GIAentityNodeClass.cpp GIAentityConnectionClass.cpp GIAconditionNodeClass.cpp GIAsentenceClass.cpp GIAdraw.cpp GIAxmlConversion.cpp GIAcxlConversion.cpp GIAquery.cpp GIAdatabase.cpp GIAnlp.cpp GIAwordnet.cpp GIAnlg.cpp GIAlrp.cpp GIAbot.cpp XMLparserClass.cpp XMLrulesClass.cpp LDsvg.cpp LDopengl.cpp LDparser.cpp LDsprite.cpp LDreferenceManipulation.cpp LDreferenceClass.cpp RToperations.cpp  RTparser.cpp  RTpixelMaps.cpp  RTppm.cpp  RTraytracer.cpp  RTreferenceManipulation.cpp  RTscene.cpp  RTviewinfo.cpp SHAREDglobalDefs.cpp SHAREDvector.cpp SHAREDvars.cpp
//#define GIA_DO_NOT_USE_UNTESTED_BUGFIX_REGARDING_grammaticalDefiniteIndexOfDeterminerTemp	//1t6b
#define GIA_SUPPORT_SPECIFIC_ACTION_CONCEPTS
#define GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_QVAR_INDEX_SAME_AS_ANOTHER_RELATION_INDEX
#define GIA_WORKAROUND_RELEX_BUG_OCCASIONAL_QVAR_INDEX_SAME_AS_ANOTHER_RELATION_INDEX
#define GIA_USE_GENERIC_ENTITY_INTERPRETATION						//1t6b
#define GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
#ifdef GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION
	#define GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_REDISTRIBUTION	//1t1a (tested 1ttd, tested with generalised link entity preparation generalised redistribution modifications 1t2m)
	#define GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_LINK			//1t2a (tested 1t3a)
	#define GIA_USE_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_SUBSTANCES		//1t2b (tested 1t3a)
#endif
#define GIA_DEPENDENCY_RELATIONS_TYPE_RELEX_PARSE_QUESTIONS_IN_NON_QUERY_INPUTTEXT	//allows questions to be properly parsed during relation redistribution when they are added to inputText also (instead of just inputTextQuery)
//#define GIA_WARNINGS

//variables currently being tested (1s1a+)
#define GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER	//disable this if using previously tested version of Stanford CoreNLP (stanford-corenlp-2012-04-03)
#define GIA_TRANSLATOR_REDISTRIBUTE_RELATIONS_WORKAROUND_STANFORD_BUG_SINGLE_DIGIT_TIMES_MARKED_AS_ADJECTIVE
#define GIA_1S8D_LOW_PRI_RELEX_UPDATE_CHECK_THAT_IT_DOESNT_BREAK_STANFORD_OPTIMISATION_APPLY_FIX_TO_IS_NAME_QUERY_PROPOGATION
//#define GIA_1S3A_ADD_INTERPRETION_OF_IN_AS_POSSESSIVE_FOR_SUBSTANCES	//disabled in GIA 1s10d due to misinterpretation of "in" in "Apples are used for eating in the forest."


//variables currently being tested (1r1a+)

#define GIA_USE_BOT
#ifdef GIA_USE_BOT
	#define GIA_BOT_SWITCH_FIRST_AND_SECOND_PERSON	//Switch "you for I" (and vice versa)
#endif

#define GIA_QUERY_DO_NOT_SEARCH_DISABLED_NODES

#define GIA_TRANSLATOR_REDISTRIBUTE_STANFORD_RELATIONS_EXPLITIVES
#define GIA_TRANSLATOR_INTERPRET_CLAUSAL_COMPLEMENT_AS_ACTION_OBJECT_INSTEAD_OF_ACTION_PROPERTY	//added to bring Stanford back into line with Relex: eg He says that you like to swim / ccomp(say, like) -> obj(say, like), Moses knew I was angry. / ccomp(knew-2, angry-5) ->  obj(knew-2, angry-5

#define GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DEPENDENT_AS_SUBSTANCE_INSTEAD_OF_GOVERNOR	//interpret "toy"/"red" as property of "shop"/"dog" (originally "shop"/"dog" was interpreted as property of "toy"/"red")
#ifdef GIA_TRANSLATOR_INTERPRET_PRENOMINAL_MODIFIER_DEPENDENT_AS_SUBSTANCE_INSTEAD_OF_GOVERNOR
	#define GIA_TRANSLATOR_DEFINE_NOUNS_WITH_PRENOMINAL_MODIFIERS_AS_SUBSTANCES		//only enable this when "toy" is property of "shop" (ie do not enable this when "shop" is property of "toy")
#endif

//variables currently being tested (1q1a+)

#define GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS	//added 1q4a to take into account specific concepts eg 'red bears' as opposed to 'bears' //eg Red dogs are bad animals. / A blue chicken is a happy bird.
#ifdef GIA_SUPPORT_SPECIFIC_SUBSTANCE_CONCEPTS
	//#define GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PROPERNOUNS	//added 1q10a [activated 1q10c] //removed 1t2d
	//#define GIA_SUPPORT_SPECIFIC_CONCEPTS_ASSIGN_TO_PRONOUNS	//added 1q10a [activated 1q10c]	//removed 1s8c	//check this does not undo advanced referencing/quering...
#endif
	//NB the phrase 'substance concept'/'substance quality' is a misnomer, as concepts and qualities are not really substances [NB concepts are considered secondary substances in Aristotle's categories, but GIA has a separate Entity class for concepts marked by isConcept: which is confusingly not being used in this case of 'specific concepts' - a software development/history artefact]" << endl;

#define GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES
#ifdef GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES
	#define GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_PROPERTIES_AND_DEFINITIONS
	#define GIA_TRANSLATOR_PREVENT_DOUBLE_LINKS_ASSIGN_CONFIDENCES_ACTIONS_AND_CONDITIONS
#endif

//variables currently being tested (1p1a+)

#define STANFORD_CORENLP_DISABLE_INDEPENDENT_POS_TAGGER_WHEN_PARSING_DEPENDENCY_RELATIONS	//added 22 Sept to enable Stanford CoreNLP to be used to parse dependency relations with comparable (NOT: same) accuracy as stanford parser (ie when stanford CoreNLP is set as both relation and feature parser)
//#define STANFORD_PARSER_USE_POS_TAGS	//added 23 July 2012 to support Stanford Parser POS tags (which are sometimes more accurate than stanford CoreNLP pos tags)

#define GIA_APPLY_BUG_WORKAROUND_WHERE_A_CONCEPT_ENTITY_OF_INSTANCE_0_CAN_HAVE_NODE_DEFINING_INSTANCE
#define GIA_FREE_MEMORY
#ifdef GIA_FREE_MEMORY
	#define GIA_FREE_MEMORY1
	#define GIA_FREE_MEMORY2
	#define GIA_FREE_MEMORY3
#endif

#define GIA_SUPPORT_INPUT_FILE_LISTS
#define GIA_SUPPORT_INCONSISTENCY_BETWEEN_STANFORD_PARSER_AND_STANFORD_CORENLP_PARSING_OF_CONSECUTIVE_FULL_STOPS

//#define GIA_QUERIES_MUST_BE_QUESTIONS	//disabled 30 June 2012

#define GIA_USE_LRP
#ifdef GIA_USE_LRP
	#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
	#ifdef GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS
		//#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_CONSERVATIVE	//added 28 October 2012b - requires GIA_USE_LRP for reading of WikipediaIrregularVerbs.txt - this is used to normalise the tagging of continuous verbs (eg in the making of): it is not 100% successful as corrections are limited to irregular continous verbs (WikipediaIrregularVerbs.txt)
		#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_LIBERAL		//alternate implementation added 5 July 2013 - requires GIA_USE_LRP for reading of WordnetVerbs.txt - this is used to normalise the tagging of continuous verbs (eg in the making of)
		//#define GIA_TRANSLATOR_CORRECT_IRREGULAR_VERB_LEMMAS_OLD_IMPLEMENTATION
	#endif

	//#define GIA_LRP_DISABLE_REDISTRIBUTE_RELATIONS_POST_NLP_MULTIWORD_PREPOSITION_REDUCTION
#endif

//variables currently being tested (1o1a+)

#ifdef USE_CE
	#define GIA_WITH_CE_CONVERT_PUNCTUATION_MARK_CHARACTERS_TO_FULL_STOPS
	#ifdef GIA_WITH_CE_CONVERT_PUNCTUATION_MARK_CHARACTERS_TO_FULL_STOPS
		#define GIA_NLP_MAXIMUM_NUMBER_WORDS_PER_SENTENCE_THAT_NLP_CAN_PARSE_DUE_TO_3GB_MEMORY_LIMITATION (150)
		#define GIA_WITH_CE_MINIMUM_NUMBER_WORDS_BEFORE_CONVERTING_PUNCTUATION_MARKS_TO_FULL_STOPS (GIA_NLP_MAXIMUM_NUMBER_WORDS_PER_SENTENCE_THAT_NLP_CAN_PARSE_DUE_TO_3GB_MEMORY_LIMITATION/2)
		#define GIA_WITH_CE_CONVERT_PUNCTUATION_MARK_CHARACTERS_TO_FULL_STOPS_BIAS_REPLACEMENT_TOWARDS_END_OF_CODEEXTENSION_BASED_ON_EXPECTED_GREATED_DISTANCE_BETWEEN_PUNCTUATION_MARKS
	#endif
	#define GIA_WITH_CE_USE_CODEEXTENSION_HEIRACHY
	#define GIA_WITH_CE_DERIVE_SCODEEXTENSION_PREPEND
	//#define GIA_WITH_CE_USE_ALL_CODEEXTENSION_COMBINATIONS	//else just use first
	//#define GIA_WITH_CE_DEBUG
#endif

#define GIA_SUPPORT_ALIASES
#ifdef GIA_SUPPORT_ALIASES
	#define GIA_SUPPORT_MORE_THAN_ONE_NODE_DEFINING_AN_INSTANCE	//NB this is required for advanced referencing to work properly with specific cases of aliasing eg; The name of the red dog is Tom. Tom ate a bone near the house.
	#define GIA_SUPPORT_ALIASES_RELEX_COMPATIBILITY
#endif
#define GIA_TEMPORARILY_DISABLE_GIA_XML_READ_CHECKS

#define GIA_USE_NLG
#ifdef GIA_USE_NLG
	#define GIA_USE_NLG_OUTPUT_TO_COMMAND_LINE
	//#define GIA_USE_NLG2
	#ifdef GIA_USE_NLG2
		#define GIA_NLG_DEBUG_MANUALLY_HARDCODE_INTO_NLG2
	#endif
	#define GIA_USE_NLG_NO_MORPHOLOGY_GENERATOR	//NB even NLG2 requires origWord not lemma, so a morphology generator is required in both
#endif

//variables currently being tested (1n1a+)

#define GIA_USE_1N1ATEMP1TO8_CHANGES	//added 1n1aTEMP9

#define GIA_USE_DATABASE	//requires further testing
#ifdef GIA_USE_DATABASE
	#define GIA_DATABASE_TEST_MODE_LOAD_ALL_ENTITIES_AND_CONNECTIONS_TO_ACTIVE_LIST_UPON_READ	//enable this for debugging only - not required for GIA
	#define GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES	//added 29 Nov 2012
	#ifdef GIA_DATABASE_DO_NOT_WRITE_DISABLED_ENTITY_NODES
		#define GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES	//added 29 Nov 2012
	#else
		//#define GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES	//optional to enable disbled node i/o without recording their connections
		//#define GIA_DATABASE_DO_NOT_WRITE_CONNECTIONS_FROM_DISABLED_ENTITY_NODES	//optional to enable disbled node i/o without recording their connections
	#endif
	#define GIA_USE_DATABASE_FILESYSTEM
	//#define GIA_USE_DATABASE_ALWAYS_LOAD_CONCEPT_NODE_REFERENCE_LISTS
	#define GIA_DATABASE_CLEAR_CACHE_EVERY_SENTENCE
#endif

#define GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES	//added 29 Nov 2012 (this cannot be used at present as <entityNode> idActiveList values must be consecutive in xml file)
#ifdef GIA_SEMANTIC_NET_DO_NOT_WRITE_DISABLED_ENTITY_NODES
	#define GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES	//added 29 Nov 2012
#else
	//#define GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_TO_DISABLED_ENTITY_NODES	//optional to enable disbled node i/o without recording their connections
	//#define GIA_SEMANTIC_NET_DO_NOT_WRITE_CONNECTIONS_FROM_DISABLED_ENTITY_NODES	//optional to enable disbled node i/o without recording their connections
#endif

#define GIA_USE_ADVANCED_REFERENCING	//requires further testing
#ifdef GIA_USE_ADVANCED_REFERENCING
	#define GIA_ADVANCED_REFERENCING_ENSURE_PLURALITY_MATCHES	//added 29 June 2013
	#define GIA_ADVANCED_REFERENCING_PREVENT_DOUBLE_LINKS
	//#define GIA_ADVANCED_REFERENCING_UPDATE_NOT_NECESSARY_OR_TESTED
	#define GIA_ADVANCED_REFERENCING_SUPPORT_INTRASENTENCE_REFERENCING
	#define GIA_ADVANCED_REFERENCING_ORIGINAL
	#define GIA_ADVANCED_REFERENCING_PREPOSITIONS
	#define GIA_ADVANCED_REFERENCING_CONDITIONS		//all conditions; ie, not just preposition conditions
	#define GIA_ADVANCED_REFERENCING_CONDITIONS_RELEX_SPECIFIC - added 2 July 2013 (requires testing)
	#define GIA_ADVANCED_REFERENCING_ASSERT_MINIMUM_SENTENCE_INDEX_OF_REFERENCE_SET	//added 1m5aTEMP11 [requires Stanford Parser - incompatible with Relex - because it requires record of the sentence entity index of the determiner "the"; grammaticalIsDefiniteIndexOfDeterminer/grammaticalDefiniteIndexOfDeterminerTemp]
	#define GIA_ADVANCED_REFERENCING_DO_NOT_REAPPLY_IS_SUBSTANCE_CONCEPT_TO_REFERENCES
	//#define GIA_ADVANCED_REFERENCING_FIND_SUBJ_OBJ_RELATION_MATCHING_AUXILLARY_AND_SET_NOT_SAME_REFERENCE_SET
#endif

//variables currently under attention for testing purposes

#define GIA_ENABLE_TEXTUAL_CONTEXT_REFERENCING			//default: on	//this enables pronoun detection	//OLD: {this needs to disable some additional parameters also... (NB substances are still being connected, and not created anew)}
#define WORDNET_SEARCH_RELATED_SYNSETS
#define GIA_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS		//this was (effectively) disabled before version 1h3b 	//NB used to be called 'GIA_DO_NOT_ASSIGN_SUBSTANCE_TO_PROPER_NOUNS'
#define GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT
//#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE2
/*#define GIA_USE_ORIGINAL_PRONOMINAL_COREFERENCE_RESOLUTION_IGNORE_SUBSTANCES_TAKE_CONCEPTS_ONLY	//untested (removed 1l1c)*/	//commented out completely
#define GIA_ID_INSTANCE_ALLOW_INSTANCE_DELETIONS
#define GIA_USE_ADVANCED_REFERENCING_FIND_ALL_RELATIONS_MATCHING_AUXILLARY_AND_SET_DIFFERENT_REFERENCE_SET

#ifndef GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER
	#define GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_GENERATE_STRANGE_LEMMA
	#ifdef GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_GENERATE_STRANGE_LEMMA
		#define GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_USE_ORIGINAL_WORD
	#endif
	#define GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_ASSIGNMENT_BUG_USE_NER_VALUES	//CHECKTHIS; does this need to be reinstated for GIA_STANFORD_CORE_NLP_VERSION_2013_04_04_OR_GREATER?
#endif
#define GIA_STANFORD_CORE_NLP_COMPENSATE_FOR_PROPERNOUN_LEMMA_CASE_ASSIGNMENT_BUG_MAKE_ALL_LEMMAS_LOWER_CASE	//used to resolve the issue where 'time'/'freedom' and 'Time'/'Freedom' are not matched etc

//variables passed testing:
#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE
#define GIA_STANFORD_DO_NOT_USE_UNTESTED_RELEX_OPTIMISATION_CODE_THAT_IS_PROBABLY_STANFORD_COMPATIBLE
//#define GIA_TRIAL_WORD_NET_SYNONYM_LOOKUP

//Debug [Disable these for official build];
//#define GIA_TRANSLATOR_XML_INTERPRETATION_DEBUG
//#define GIA_GENERIC_DEPENDENCY_RELATION_INTERPRETATION_DEBUG
//#define GIA_TRANSLATOR_DEFINE_SUBSTANCES_DEBUG
//#define GIA_ADVANCED_REFERENCING_DEBUG_DISABLE_LINKING
//#define GIA_LRP_DEBUG
//#define GIA_NLG_DEBUG
//#define GIA_ADVANCED_REFERENCING_DEBUG_TOO_LARGE_REFERENCE_SET
//#define GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_CONNECTIONS_WITH_COLOURS
//#define GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS
#ifdef GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS
	//#define GIA_ADVANCED_REFERENCING_DEBUG_HIGHLIGHT_REFERENCE_SET_NODES_WITH_COLOURS_WORK_WITH_MULTIPLE_SENTENCES
#endif
//#define GIA_ALIASES_DEBUG
//#define GIA_ADVANCED_REFERENCING_DEBUG  	  //just finished first debug using this (1l5f)
//#define GIA_QUERY_DEBUG
//#define GIA_QUERY_MULTIPLE_ANSWERS_DEBUG      //just finished first debug using this (1l2a)
//#define GIA_STANFORD_CORENLP_CODEPENDENCY_PRONOMINAL_REFERENCING_DEBUG
//#define GIA_DATABASE_DEBUG_FILESYSTEM_IO
//#define GIA_DATABASE_DEBUG		  //this needs to be debugged (not yet tested)
//#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG
//#define GIA_REDISTRIBUTE_STANFORD_RELATIONS_QUERY_VARIABLE_DEBUG_DO_NOT_MAKE_FINAL_CHANGES_YET
//#define GIA_OUTPUT_INTERNAL_RELATIONS_IN_RELEX_FORMAT_DEBUG
//#define GIA_WORDNET_DEBUG
//#define GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG2
//#define GIA_STANFORD_DEPENDENCY_RELATIONS_DEBUG
//#define GIA_TRANSLATOR_DEBUG
//#define GIA_NLP_DEBUG
//#define GIA_DRAW_DEBUG
//#define GIA_SEMANTIC_NET_XML_DEBUG
//#define GIA_SEMANTIC_NET_CXL_DEBUG
//#define GIA_FREE_MEMORY_DEBUG
//#define GIA_MAIN_DEBUG
//#define GIA_PRONOUN_REFERENCING_DEBUG

//DebugOLD;
//#define GIA_WORDNET_DEBUG_OUTPUT_SYNONYMNS	//Do not enable else will prematurely exit


//Wordnet
#define USE_WORDNET	(takes into account synonymns)
#ifdef USE_WORDNET
	#define GIA_USE_SYNONYMN_DETECTION
#endif


//~External NLP Parser
#define GIA_USE_RELEX
#define GIA_USE_STANFORD_CORENLP
#define GIA_USE_STANFORD_PARSER
#ifdef GIA_USE_RELEX
	//#define GIA_USE_RELEX_IN_STANFORD_COMPATIBILITY_MODE	//untested - not implemented
	#ifdef GIA_USE_RELEX_IN_STANFORD_COMPATIBILITY_MODE
		#define GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	#endif
	#ifdef LINUX
		#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.sh"
	#else
		#define GIA_RELEX_EXECUTABLE_NAME "execute-relex.bat"
	#endif
	#define GIA_USE_RELEX_1_4_0	//default: enabled (otherwise use Relex 1.3.0)
	#ifdef GIA_USE_RELEX_1_4_0
		//#define GIA_USE_RELEX_UPDATE_ADD_PARAGRAPH_TAGS		//BAI paragraph tag support has not yet been added to Relex 1.3.0	//NB GIA no longer works with BAI Relex <paragraph> tag upgrade - as sentence ID is now used for a variety of purposes (without paragraph id): therefore will need to determine a new method of establishing intra paragraph referencing.
	#endif
#endif
#ifdef GIA_USE_STANFORD_CORENLP		//a more advanced implementation of stanford parser (with lemma, entity name detection, etc: Stanford CoreNLP integrates all our NLP tools for the English language, including the part-of-speech (stanfordPOS) tagger, the named entity recognizer (NER), the parser, and the coreference resolution system)
	#define GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	#ifdef LINUX
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME "execute-stanfordCoreNLP.sh"
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME_WITHOUT_INDEPENDENT_POS_TAGGER "execute-stanfordCoreNLPwithoutPT.sh"
	#else
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME "execute-stanfordCoreNLP.bat"
		#define GIA_STANFORD_NLP_EXECUTABLE_NAME_WITHOUT_INDEPENDENT_POS_TAGGER "execute-stanfordCoreNLPwithoutPT.bat"
	#endif
#endif
#ifdef GIA_USE_STANFORD_PARSER
	#define GIA_USE_STANFORD_DEPENDENCY_RELATIONS
	#ifdef LINUX
		#define GIA_STANFORD_PARSER_EXECUTABLE_NAME "execute-stanfordParser.sh"
	#else
		#define GIA_STANFORD_PARSER_EXECUTABLE_NAME "execute-stanfordParser.bat"
	#endif
#endif
#define GIA_NLP_PARSER_RELEX (0)
#define GIA_NLP_PARSER_STANFORD_CORENLP (1)
#define GIA_NLP_PARSER_STANFORD_PARSER (2)
#define GIA_NLP_PARSER_NUMBER_OF_TYPES (3)
#define GIA_NLP_FEATURE_PARSER_FOR_INPUT_TEXT_DEFAULT (GIA_NLP_PARSER_STANFORD_CORENLP)
#define GIA_NLP_RELATIONS_PARSER_FOR_INPUT_TEXT_DEFAULT (GIA_NLP_PARSER_STANFORD_PARSER)	//NB GIA_NLP_PARSER_STANFORD_CORENLP will be default in future, once it works better than GIA_NLP_PARSER_STANFORD_PARSER.
#define GIA_NLP_FEATURE_PARSER_FOR_INPUT_QUERY_DEFAULT (GIA_NLP_PARSER_RELEX)
#define GIA_NLP_RELATIONS_PARSER_FOR_INPUT_QUERY_DEFAULT (GIA_NLP_PARSER_RELEX)
#define GIA_DEPENDENCY_RELATIONS_TYPE_RELEX (0)
#define GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD (1)
#define GIA_DEPENDENCY_RELATIONS_NUMBER_OF_TYPES (2)		//Relex and Stanford tags
#define GIA_NLP_DEPENDENCY_RELATIONS_PARSER_RELEX_DEFAULT_DEPENDENCY_RELATIONS_TYPE (GIA_DEPENDENCY_RELATIONS_TYPE_RELEX)
#define GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_CORENLP_DEFAULT_DEPENDENCY_RELATIONS_TYPE (GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)
#define GIA_NLP_DEPENDENCY_RELATIONS_PARSER_STANFORD_PARSER_DEFAULT_DEPENDENCY_RELATIONS_TYPE (GIA_DEPENDENCY_RELATIONS_TYPE_STANFORD)

#define GIA_NLP_PARSER_TYPE_RELATIONS (0)
#define GIA_NLP_PARSER_TYPE_FEATURES (1)

//~GIAtranslator
#define GIA_USE_CONCEPT_ENTITY_NODE_MAP_NOT_VECTOR	//this is required (the current set of code has had the alternative case removed - see GIAtranslator.cpp.copyWithDataStructureOptions for an example set of code that supports disabling this feature)
#define REFERENCE_TYPE_QUESTION_COMPARISON_VARIABLE "_$qVar"
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY
#define GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED	//this is now MANDATORY
#ifdef GIA_TRANSLATOR_COMPENSATE_FOR_SWITCH_OBJ_SUB_DEFINITION_QUESTIONS_ANOMALY_ADVANCED
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_WHERE_NECESSARY	//this was changed 27 July 2013
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED	//this was changed 27 July 2013
#else
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_WHERE_NECESSARY
	#define GIA_PERFORM_RELATION_GOVERNOR_ARGUMENT_SWITCHING_ONLY_WHEN_REQUIRED
#endif

//~GIAmain
//#define GIA_DO_NOT_PRINT_RESULTS

//~GIAquery
#define GIA_QUERY_DOUBLE_ERROR (0.0001)
#define GIA_QUERY_CONFIDENCE_MULTIPLIER (10)
	//these preprocessor have been moved to GIAglobalsDefs.h as they are used in both GIAtranslator.cpp and GIAquery.cpp
#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
#ifdef GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES
	#define GIA_SUPPORT_COMPARISON_VARIABLE_DEFINITION_VIA_ALTERNATE_METHOD_EG_SUPPORT_WHICH_QUERIES_ADVANCED	//allows for verification that answer node is of type identified by the which query eg verifies that the answer 'dog' [from text: 'the dog eats the mud. dogs are animals.'], is an animal with the query: 'which animal eats the mud?'
#endif



//~GIAdraw
//#define GIA_DRAW_USE_PATENT			//modifies colours of nodes such that they print uniquely in black and white
//#define GIA_CMAP_CONVERSION_SANITISED 	//use format akin to Cmap Tools / not GIA formatted. linking-phrase-list -> actions + conditions. concept-list -> concepts or substances
#define GIA_DRAW_DISPLAY_ANSWER_CONTEXTS
#ifdef GIA_ADVANCED_REFERENCING_PREPOSITIONS	//is this condition required?
	#define GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
	#ifdef GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX
		#define GIA_DRAW_PRINT_ENTITY_NODES_IN_ORDER_OF_SENTENCE_INDEX_ADVANCED		//more robust implementation
	#endif
#endif

#endif

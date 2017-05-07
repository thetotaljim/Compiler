//
//  main.cpp
//  Xcode_P4
//
//  Created by James Steimel on 5/6/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "scanner.h"
#include "parser.h"
#include "treePrint.h"
#include "codeGen.h"

//  Function Prototypes
std::string stripExtension(std::string);
bool hasExtension(std::string);
void usageMessage();

FILE *input_file;

/****************************************************************/
/****************************************************************/
/*    Main Function                                             */
/*                                                              */
/*    In main, the users command line input is parsed, and      */
/*    the resulting input_file_name is opened and passed to the */
/*    driver.                                                   */
/****************************************************************/

int main(int argc, const char * argv[]) {
    printf("\n*******************************************\n");
    printf("cs4280 Project 3 by James Steimel 4/15/17\n");
    printf("*******************************************\n");
    std::string input_file_name;
    std::string inFile;
    // Here we check to see if a filename was passed.
    if (argc == 2) {
        //  Get the filename from the command line.
        input_file_name = argv[1];
        inFile = argv[1];
        //  Check for an extension.
        if (hasExtension(input_file_name) == false)
        {
            printf("Determined no file extension.\n");
            //  If no extension, apply the implicit extension.
            input_file_name.append(".4280E01");
            printf("After append file name = %s\n", input_file_name.c_str());
        }
        else {
            fprintf(stderr, "testFrontEnd Error: File does not conform to requirements. Terminating Scanner.\n");
            return -1;
        }
        //  Open the file.
        printf("About to open: %s\n", input_file_name.c_str());
        input_file = fopen(input_file_name.c_str(), "r");
        //  Check if file was successfully opened.
        if(!input_file){
            fprintf(stderr, "testFrontEnd Error: Can't open file. Terminating Scanner.\n");
            return -1;
        }
    } else if (argc == 1) {
        
        /*  THIS WORKS FOR FILLING FILE WITH THE DATA FROM INPUT  */
        printf("Enter data and press Ctrl+d on a new line to signal end of file when finished.\n");
        int c;
        input_file_name = "temp.4280E01";
        input_file = fopen(input_file_name.c_str(), "w");
        while ((c = getchar()) != EOF){
            char intermediate = c;
            fprintf(input_file, "%c", intermediate);
        }
        
        fclose(input_file);
        
        input_file = fopen(input_file_name.c_str(), "r");
        
    } else {
        //  If file can't be open, display a message explaining P2 usage options.
        usageMessage();
        return 1;
    }
    
    //  Once we have working input, we pass it to the parser, which
    //  passes back the root_node of the parse tree if successful.
    node_t *myTree = parser(input_file);
    //  Print the parse tree to standard output.
    //  Have to pass level as well, as it is part of
    //  the tree's recursive output call.
    int level = 0;
    printTree(myTree, level);
    
    string targName;
    //create out file and give it to the code generator
    if (input_file_name.compare("temp.4280E01") == 0) {
        targName = "keyboard.asm";
    } else {
        //string extension = ".asm";
        //targName = strcat(argv[1], extension.c_str());
        targName = inFile.append(".asm");
    }
    
    FILE *outFile = fopen(targName.c_str(), "w");
    setTargetFile(outFile);
    codeGenInit(myTree);
    //	If we are back in main, the tree output was successful.
    //	Let the user know the program is terminating successfully.
    printf("\nParse tree output generated, program terminating successfully.\n");
    //  Close the file.
    fclose(input_file);
    //  Terminate program.
    return 0;
}

/****************************************************************/
/****************************************************************/
/*    stripExtension Function:                                  */
/*                                                              */
/*  This function will strip the extension of a string          */
/*  input_file_name and return the value as a string.           */
/*                                                              */
/****************************************************************/

std::string stripExtension(std::string c)
{
    std::string output;
    //  Here all characters occuring before '.' are
    //  passed to the output string.
    for ( int count = 0; c[count] != '.'; count++)
    {
        output.push_back(c[count]);
    }
    return output;
}

/****************************************************************/
/****************************************************************/
/*    hasExtension Function:                                    */
/*                                                              */
/*  This function will check if a string includes a ".", and    */
/*  returns a boolean result.                                   */
/*                                                              */
/****************************************************************/

bool hasExtension(std::string input_file_name)
{
    std::string period = ".";
    if (input_file_name.find(period) != std::string::npos)
    {
        return true;
    }
    else{
        return false;
    }
}

/****************************************************************/
/****************************************************************/
/*  usageMessage Function:                                      */
/*                                                              */
/*  This function will display the usage of P1 to std::cin.     */
/*  I made this a function so a command line option to display  */
/*  usage could be added later.                                 */
/*                                                              */
/****************************************************************/

void usageMessage(){
    
    std::cout << "Usage:\n";
    std::cout << "P1 [input_file_name]  : assumed extension is \".4280E01\". \n";
    std::cout << "P2 [file.extension]   : will attempt to open file with exention.\n";
    std::cout << "P2 < [file.extension] : reads the file through std::cin.\n";
    std::cout << "P2                    : will allow keyboard input from user.\n";
    
}




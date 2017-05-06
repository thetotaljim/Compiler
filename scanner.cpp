//
//  scanner.cpp
//  P4_X2
//
//  Created by James Steimel on 5/3/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//

#include "scanner.h"


//  Structure for holding key value pairs.
struct table_value{
    int key;
    int value;
};

//structure for holding a token mapping
typedef struct {
    int key;
    tokenID instance;
}tkMap;

//token names for lookup with token enumeration
std::string token_names[] = {
    "Identifier",
    "Number",
    "Start keyword",
    "Stop keyword",
    "Decision keyword",
    "While keyword",
    "Void keyword",
    "Int keyword",
    "Return keyword",
    "Read keyword",
    "Print keyword",
    "Program keyword",
    "Relational Operator",
    "Other Operator",
    "Delimeter",
    "End of File",
    "Error"
};


//used to look up characters (column in state table)
table_value match_instance[20] = {
    {'=', 0},
    {'<', 1},
    {'>', 1},
    {'!', 2},
    {':', 3},
    {'+', 3},
    {'-', 3},
    {'*', 3},
    {'/', 3},
    {'&', 3},
    {'%', 3},
    {'.', 4},
    {'(', 4},
    {')', 4},
    {',', 4},
    {'{', 4},
    {'}', 4},
    {';', 4},
    {'[', 4},
    {']', 4}
};

//Used to define keywords (all line numbers default to 0, will be changed in scanner)
token keywords[11] = {
    {START_tk, "start", 0},
    {STOP_tk, "stop", 0},
    {DECS_tk, "decision", 0},
    {WHLE_tk, "while", 0},
    {VOID_tk, "void", 0},
    {INT_tk, "int", 0},
    {RET_tk, "return", 0},
    {READ_tk, "read", 0},
    {PRINT_tk, "print", 0},
    {PROG_tk, "program", 0},
};

//lookup table for final states
tkMap tokens[6] = {
    {1005,  EOF_tk},
    {1000, IDENT_tk},
    {1001, NUM_tk},
    {1002, OP_tk},
    {1003, DEL_tk},
    {1004, REL_tk}
};

//  Here is the state table.
//  column 0 = '='
//  column 1 = '>' or '<'
//  column 2 = '!'
//  column 3 = other operators.
//  column 4 = delimiters.
//  column 5 = identifiers
//  column 6 = numbers
//  column 7 = white space
//  column 8 = EOF.
int state_table[10][9] = {
    {1,     2,    3,    4,     5,     6,    7,     0,    1005},
    {9,     1002, 9,    1002,  1002,  1002, 1002,  1002,  0},
    {1004,  1004, 1004, 1004,  1004,  1004, 1004,  1004,  0},
    {-2,     -2,   -2,   -2,    -2,   -2,    -2,    -2,   -2},
    {1002,  1002, 1002, 1002,  1002,  1002, 1002,  1002,  0},
    {1003,  1003, 1003, 1003,  1003,  1003, 1003,  1003,  0},
    {1000,  1000, 1000, 1000,  1000,  6,    1000,  1000,  0},
    {1001,  1001, 1001, 1002,  1001,  1001, 7,     1001,  0},
    {1004,  1004, 1004, 1004,  1004,  1004, 1004,  1004,  0},
    {1004,  1004, 1004, 1004,  1004,  1004, 1004,  1004,  0}
};

/********************************************************************/
/********************************************************************/
/*  filter Function:                                                */
/*                                                                  */
/*  The filter takes the file passed from main and prepares it for  */
/*  the scanner by tracking line number. Then it prints the token   */
/*  that is returned by the scanner function.                       */
/*                                                                  */
/********************************************************************/

token filter(FILE *input_file, int& lineNum) {
    //  Variable used to lookahead in the file and
    //  track EOF, line number, and filter out comments.
    int lookahead;
    //  Here we lookahead into the file until we reach
    //  the EOF.
    token the_token;
    while((lookahead = fgetc(input_file)) != EOF) {
        //  If the lookahead is a new line, then the line number
        //  counter is incremented, and the new line is returned
        //  to the file.
        if(lookahead == '\n') {
            lineNum++;
        }
        //  put the
        //  lookahead value back.
        ungetc(lookahead, input_file);
        //  Now we get a token from the scanner.
        the_token = scanner(input_file, lineNum);
        //  If the token is an error token, then
        //  the program ends.
        if(the_token.tokenId == ERR_tk) {
            std::string outString = getTokenString(the_token);
            std::cout<< outString;
            return the_token;
        } else {    //  Otherwise return the token.
            return the_token;
        }
        
    }
    return the_token;
    
}

/********************************************************************/
/********************************************************************/
/*  getTokenString Function:                                        */
/*                                                                  */
/*  This function takes a token as parameter and creates an output  */
/*  string using its values. Declared in header because it could    */
/*  be useful in other context.                                     */
/********************************************************************/

std::string getTokenString(token the_token) {
    std::string outString = "This is not used in P2\n";
    //"Line Number: " + std::to_string(the_token.lineNum) + ", Token Id: " + token_names[the_token.tokenId] + "Instance type: " + the_token.instance + "\n";
    return outString;
}

/********************************************************************/
/********************************************************************/
/*  getAToken Function:                                             */
/*                                                                  */
/*  This function will loop through the tokens and                  */
/*  match the passed state with one in the table. In the case       */
/*  of state 5 (indentifier), it will check to make sure the        */
/*  the passed instance is not equal to any of the keywords.        */
/*  If it is, it is matched with values from keywords table.        */
/********************************************************************/

token getAToken(int state, std::string instance, int lineNum) {
    token the_token;
    //  The outer loop cycles through the first 6 numbers in the
    //  state, checking (which match those in the tokens,
    //  and then comares them to the passed state.
    for (int i = 0; i < 6; i++) {
        //  If state 5 (identifier), then the list of keywords is checked.
        //  If a match is found the values for the keywords are substituted
        //  for those passed from the scanner.
        if (state == 1000) {
            for (int j = 0; j < 11; j++) {
                if (keywords[j].instance == instance) {
                    the_token.tokenId = keywords[j].tokenId;
                    the_token.instance = instance;
                    the_token.lineNum = lineNum;
                }
            }
        }
        //  Otherwise it finds the appropriate match and fills in the
        //  token values.
        if (tokens[i].key == state) {
            the_token.tokenId = tokens[i].instance;
            the_token.instance = instance;
            the_token.lineNum = lineNum;
        }
    }
    //  And the token is returned to the scanner
    return the_token;
}

/********************************************************************/
/********************************************************************/
/*  getColumnIndex Function:                                        */
/*                                                                  */
/*  This function will return the appropriate column  index         */
/*  in the state table to match the passed lookahead value.         */
/*                                                                  */
/********************************************************************/

int getColumnIndex(char lookAhead) {
    //  This function first cycles through the various operators,
    //  delimiters, and relational characters.  It will return the
    //  appropriate column number if a match is found
    for (int i = 0; i < 20; i++) {
        if(match_instance[i].key == lookAhead) {
            return match_instance[i].value;
        }
    }
    //  Otherwise, further testing will determine if the lookahead
    //  is a character, integer, space or end of file. The appropriate
    //  column index is returned for use with the state table.
    if (isalpha(lookAhead)) {
        return 5;
    } else if (isdigit(lookAhead)) {
        return 6;
    } else if (isspace(lookAhead)) {
        return 7;
    } else if (lookAhead == EOF) {
        return 8;
    } else {
        return 9;
    }
}

/************************************************************************/
/************************************************************************/
/*  scanner Function:                                                   */
/*                                                                      */
/*  This is the main function of the scanner.  After the filter has     */
/*  tracked the line numbers, it passes the file and line number to     */
/*  this scanner function.  The scanner uses the getColumnIndex function*/
/*  to get the column index that matches the lookup value. Then it      */
/*  uses this, along with the state value as the row index to get       */
/*  the next state.  If the state returned is a final state, it         */
/*  creates the appropriate token and returns it.  Otherwise,           */
/*  it handles any non-final states for further processing.             */
/*                                                                      */
/************************************************************************/

token scanner(FILE *input_file, int &lineNum) {
    
    //  Variable declarations.
    int lookahead;
    int column_index;
    int current_state = 0;
    int next_state = 0;
    int size_of_identifier = 0;
    std::string instance;
    token the_token;
    
    //if we're not at a final state or an error
    while (current_state < 1000 && current_state >= 0) {
        lookahead = fgetc(input_file);
        
        //  If a '#' is detected, then skip all characters until then next '\n'
        //  is encountered (DO I NEED TO ADD EOF TO THIS CHECK?, NEED TO TEST FOR
        //  THAT CASE.
        if(lookahead == '#') {
            while (lookahead != '\n' ){
                
                lookahead = fgetc(input_file);
            }
            lineNum++;
        }
        //get the next state
        column_index = getColumnIndex(lookahead);
        next_state = state_table[current_state][column_index];
        //if it's a final state or EOF
        if (next_state >= 1000 || next_state == 1005) {
            
            //if EOF
            if (next_state == 1005) {
                ungetc(lookahead, input_file);
                the_token.tokenId = EOF_tk;
                the_token.lineNum = lineNum;
                the_token.instance = "EOF";
                return the_token;
            } else if (next_state >= 1000) { // if final state
                the_token = getAToken(next_state, instance, lineNum);
                the_token.lineNum = lineNum;
                ungetc(lookahead, input_file);
                return the_token;
            } else { //error
                std::cout << "Scanner Error: Line " << lineNum << " value: " << static_cast<char>(lookahead) << "\n";
                the_token.tokenId = ERR_tk;
                the_token.lineNum = lineNum;
                the_token.instance = "Error";
                return the_token;
            }
        } else {
            //  Here the scanner prepares any non-final states for the next state.
            char value = static_cast<char>(lookahead);
            //  Continue to buile the instance as long as no ws is encountered.
            if (!isspace(value)) {
                instance += value;
                size_of_identifier++;
                if(size_of_identifier > 8){
                    the_token.tokenId = ERR_tk;
                    the_token.lineNum = lineNum;
                    the_token.instance = "Scanner Error: Identifier of more than 8 characters.\n";
                    return the_token;
                }
            }
            
            //  If the next state = 3, then '!' is found not following an '=',
            //  which  is an error using !.
            if (next_state == 3){
                the_token.tokenId = ERR_tk;
                the_token.lineNum = lineNum;
                the_token.instance = "Scanner Error: incorrect use of !\n";
                return the_token;
            }
            //  Here if the next state is 2, then we are checking to make sure that the
            //  '<' or '>' operator is followed by itself. Otherwise, an error token
            //  is generated and the scanner terminates.
            else if (next_state == 2){
                int nextChar = fgetc(input_file);
                if (nextChar == lookahead){
                    instance += static_cast<char>(nextChar);
                    the_token.instance = instance;
                    the_token.lineNum = lineNum;
                    return the_token;
                }
                else {
                    //  This is executed if a '<' or '>' operator is used incorrectly.
                    ungetc(nextChar, input_file);
                    the_token.tokenId = ERR_tk;
                    the_token.lineNum = lineNum;
                    std::string lessError = "Scanner Error: incorrect use of <\n";
                    std::string greaterError = "Scanner Error: incorrect use of >\n";
                    the_token.tokenId = ERR_tk;
                    the_token.lineNum = lineNum;
                    if (the_token.instance == ">")
                        the_token.instance = greaterError;
                    else
                        the_token.instance = lessError;
                    return the_token;
                }
                
            }
            current_state = next_state;
        }
    }
    //  If the scanner falls through to this part of the function,
    //  it generates an error token. When the filter finds the ERR_tk,
    //  it will end the program with an error.
    the_token.tokenId = ERR_tk;
    the_token.lineNum = lineNum;
    the_token.instance = "Scanner didn't do anything";
    return the_token;
}




//
//  parser.cpp
//  P4_X2
//
//  Created by James Steimel on 5/3/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "parser.h"
#include "token.h"

//  Here are some variable declarations
token tk;
FILE *fp = NULL;
int lineNum = 1;


/******************************************************/
/*  Main Parser Function                              */
/*                                                    */
/*  The main parser function takes the passed file    */
/*  pointer, and then calls the filter function,      */
/*  which will return the initial token.  Then        */
/*  prog() function is called to start checking the   */
/*  program for correct grammer. The root node to the */
/*  tree created by parsing the program is returned   */
/*  to main to be used to print the resulting tree.   */
/******************************************************/
node_t* parser(FILE* input_file) {
    
    fp = input_file;
    node_t *root_node;
    tk = filter(fp, lineNum);
    root_node = prog();
    
    printf("Successful Parse. Status: OK\n\n");
    return root_node;
}

/*****************************************************/
/*  Create_Node function.                            */
/*  Helper function to create new nodes with their   */
/*  labels initialized with the passed string.       */
/*****************************************************/

node_t* create_node(std::string s){
    node_t* node = new node_t;
    node->label = s;
    return node;
}

/*****************************************************/
/*  BELOW ARE ALL FUCTIONS FOR PARSING               */
/*****************************************************/

/******************************************************/
/*  <prog>    ->    program <vars> <block>            */
/******************************************************/

node_t* prog() {
    //  Get reference to a node object.
    node_t *myNode = create_node("<program>");
    //  Check for correct token.
    if(tk.tokenId == PROG_tk) {
        //  Consume token.
        tk = filter(fp, lineNum);
        //  Now call the <var> and <block> function to check this
        //  part of the grammer for a <program>
        myNode->child1 = vars();
        myNode->child2 = block();
        //  After success calling var and block functions, return the current node.
        return myNode;
    } else {
        //  Error message.
        printf("Incorrect token:\"%s\" at line %d, expected \"program\" keyword.\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <block>    ->    start <vars> <stats> stop        */
/******************************************************/

node_t* block(){
    node_t* myNode = create_node("<block>");
    if(tk.tokenId == START_tk){
        //  Since we are at the start of the block,
        //  consume the "start" token, and don't store it,
        //  as we are not storing syntactic tokens.
        tk = filter(fp, lineNum);
        myNode->child1 = vars();
        myNode->child2 = stats();
        if(tk.tokenId == STOP_tk){
            // now we have successfully reached the end of the block, we consume the
            // token to prepare for the next function call.
            tk = filter(fp, lineNum);
            return myNode;
        }
        else{
            printf("Incorrect token:\"%s\" at line %d, expected \"stop\" token.\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }
    else {
        printf("Incorrect token: \"%s\" at line %d, expected \"start\" token to begin <block>.\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <vars>    ->    empty | int Identifier <mvars>    */
/******************************************************/

node_t* vars(){
    
    node_t *myNode = create_node("<var>");
    
    if(tk.tokenId == START_tk || tk.tokenId == DECS_tk || tk.tokenId == WHLE_tk || tk.tokenId == READ_tk || tk.tokenId == PRINT_tk || tk.tokenId == IDENT_tk){
        //  This code is called if the "var" is empty case, by checking for beginning of block or stat
        return myNode;
    }
    else if(tk.tokenId == INT_tk){
        //  If the token is the int keyword, further processing is done
        //  Consume current token
        tk = filter(fp, lineNum);
        //  Check to see if we have an Identifier
        if(tk.tokenId == IDENT_tk){
            myNode->child1 = create_node("Identifier");
            myNode->child1->myToken = tk;
            //  Consume current token before function call to check <mvars> as per the grammer.
            tk = filter(fp, lineNum);
            myNode->child2 = mvars();
            //  And if call to mvars is successful, return the current node.
            return myNode;
        }
        else {
            //  Error message.
            printf("Incorrect token:\"%s\" at line %d, expected \"Identifier\" token.\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }
    else{
        //  Error message.
        printf("Incorrect token: %s at line %d, expected either empty case for <var> or \"int\" keyword to signal Identifier declaration.", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <mvars>    ->    empty | : Identifier <mvars>     */
/******************************************************/
node_t* mvars(){
    node_t *myNode = create_node("<mvars>");
    //  If the beginning of a <block> or <stats> is detected, then the <mvars> is empty, which is valid in
    //  my grammer.
    if(tk.tokenId == START_tk || tk.tokenId == READ_tk || tk.tokenId == DECS_tk || tk.tokenId == WHLE_tk || tk.tokenId == PRINT_tk || tk.tokenId == IDENT_tk ){
        //  Here we check to see if the call to mvars is returning empty by checking the possible
        //  entry tokens to the next part of parse.
        //  If mvars is empty, return the placeholder for the tree
        return myNode;
    }
    else if (tk.instance == ":"){
        //  Consume token, get new one
        tk = filter(fp, lineNum);
        //  Check for an identifier, per grammer rules.
        if (tk.tokenId == IDENT_tk){
            //  Set the 1st child node.
            myNode->child1 = create_node("Identifier");
            myNode->child1->myToken = tk;
            //  Consume token, get new one
            tk = filter(fp, lineNum);
            //  Set the second child node with the results of mvars() call
            myNode->child2 = mvars();
            //  Return the current node.
            return myNode;
        }
    }
    else{
        printf("Incorrect token: token \"%s\" at line %d, expected either empty case for <mvars> or \":\" delimiter token.\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
    return myNode;
}

/******************************************************/
/*  <expr>    ->    <M> * <expr> | <M>                */
/******************************************************/

node_t* expr(){
    node_t* myNode = create_node("<expr>");
    myNode->child1 = M();
    
    if(tk.tokenId == OP_tk && tk.instance == "*"){
        myNode->child2 = create_node("*");
        //consume token
        tk = filter(fp, lineNum);
        myNode->child3 = expr();
        return myNode;
    }
    else{
        return myNode;
    }
}

/******************************************************/
/*  <M>    ->    <T> / <M> | <T>                      */
/******************************************************/

node_t* M(){
    node_t* myNode = create_node("<M>");
    myNode->child1 = T();
    
    if(tk.tokenId == OP_tk && tk.instance == "/"){
        myNode->child2 = create_node("/");
        //consume token
        tk = filter(fp, lineNum);
        myNode->child3 = M();
        return myNode;
    }
    else{
        return myNode;
    }
}

/******************************************************/
/*  <T>    ->    <F> + <T> | <F> - <T> | <F>          */
/******************************************************/

node_t* T(){
    node_t* myNode = create_node("<T>");
    myNode->child1 = F();
    if(tk.tokenId == OP_tk && tk.instance == "+"){
        myNode->child2 = create_node("+");
        //consume token
        tk = filter(fp, lineNum);
        myNode->child3 = T();
        return myNode;
    }
    else if(tk.tokenId == OP_tk && tk.instance == "-"){
        myNode->child2 = create_node("-");
        //consume token
        tk = filter(fp, lineNum);
        myNode->child3 = T();
        return myNode;
    }
    else {
        return myNode;
    }
}

/******************************************************/
/*  <F>    ->    & <F> | <R>                          */
/******************************************************/

node_t* F(){
    node_t* myNode = create_node("<F>");
    //  First test for case & <F>
    if(tk.tokenId == OP_tk && tk.instance == "&"){
        myNode->child1 = create_node("&");
        // consume token
        tk = filter(fp, lineNum);
        myNode->child2 = F();
        return myNode;
    }
    else if((tk.tokenId == DEL_tk && tk.instance == "(") || tk.tokenId == IDENT_tk || tk.tokenId == NUM_tk){
        //  Here we have tested for the start of <R>
        myNode->child1 = R();
        return myNode;
    }
    else {
        //  Error message.
        printf("Incorrect token:\"%s\" at line %d, expected either \"&\" or \"(\".\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <R>    ->    (<expr>) | Identifier | Number       */
/******************************************************/

node_t* R() {
    node_t* myNode = create_node("<R>");
    //  Check for opening "("
    if (tk.tokenId == DEL_tk && tk.instance == "(") {
        //consume the token
        tk = filter(fp, lineNum);
        myNode->child1 = expr();
        //  Check for closing ")"
        if (tk.tokenId == DEL_tk && tk.instance == ")") { // )
            //consume the token
            tk = filter(fp, lineNum);
            // successful here, return node
            return myNode;
        }
        else{
            //  Error message.
            printf("Incorrect token: \"%s\" at line %d, expected \")\" token.\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    } else if (tk.tokenId == IDENT_tk) {
        //  Check for indentifier
        myNode->child1 = create_node("Identifier");
        //  Set child nodes for parse tree output
        myNode->child1->myToken = tk;
        //  Consume token to prepare for further function calls
        tk = filter(fp, lineNum);
        //  Return node
        return myNode;
    } else if (tk.tokenId == NUM_tk) {
        //  R can also be a number to here we check that condition
        myNode->child1 = create_node("Number");
        myNode->child1->myToken = tk;
        //consume token
        tk = filter(fp, lineNum);
        return myNode;
    } else {
        //  Error message.
        printf("Incorrect token: \"%s\" at line %d, expected should have been either Identifier, Number, or \"(\".\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <stats>    ->    <stat> <mStat>                   */
/******************************************************/

node_t* stats() {
    node_t* myNode = create_node("<stats>");
    //  now check the rest of grammer for stats
    myNode->child1 = stat();
    myNode->child2 = mStat();
    //  if we get here, parse was successful so far
    //  return the node.
    return myNode;
}

/******************************************************/
/*  <mStat>    ->    empty | <stat> <mStat>           */
/******************************************************/

node_t* mStat() {
    node_t *myNode = create_node("<mStat>");
    if (tk.tokenId == STOP_tk) {
        //  if we have a stop token here, then mstat is empty.
        return myNode;
    } else {
        //  otherwise, check rest of grammer
        myNode->child1 = stat();
        myNode->child2 = mStat();
        return myNode;
    }
}

/******************************************************/
/*  <stat>    ->    <in> | <out> | <block | <if> |    */
/*                  <loop> | <assign>                 */
/******************************************************/

node_t* stat() {
    node_t *myNode = create_node("<stat>");
    
    if (tk.tokenId == READ_tk) {
        //  Here we have the <in> case, add read node ??? Syntactic ???
        myNode->child1 = create_node("read");
        //  consume the token
        tk = filter(fp, lineNum);
        //  And call in() function
        myNode->child2 = in();
        return myNode;
    } else if (tk.tokenId == PRINT_tk) {
        //  Here we test the <out> case
        myNode->child1 = create_node("print");
        tk = filter(fp, lineNum);
        myNode->child2 = out();
        return myNode;
    } else if (tk.tokenId == START_tk) {
        //THis is the block case
        myNode->child1 = block();
        return myNode;
    } else if (tk.tokenId == DECS_tk) {
        //  Case  <if>
        myNode->child1 = create_node("decision");
        tk = filter(fp, lineNum);
        myNode->child2 = my_if();
        return myNode;
    } else if (tk.tokenId == WHLE_tk) {
        //  Loop case
        myNode->child1 = create_node("while");
        tk = filter(fp, lineNum);
        myNode->child2 = loop();
        return myNode;
    } else if (tk.tokenId == IDENT_tk) {
        //myNode->child1 = create_node("Identifier");
        //myNode->child1->myToken = tk;
        //tk = filter(fp, lineNum);
        myNode->child2 = assign();
        return myNode;
    } else {
        //  Error message.
        printf("Incorrect token: \"%s\" at line %d does not apply to expected <stat> instance.\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <in>   ->    read >> Identifier ;                 */
/******************************************************/

node_t* in(){
    node_t* myNode = create_node("<in>");
    
    if(tk.instance == ">>"){
        //  Now that we have got the >>, we consume that token, and move to
        //  check for Identifier.
        myNode->child1 = create_node(">>");
        tk = filter(fp, lineNum);
        if(tk.tokenId == IDENT_tk){
            //  if we have an identifier, store it for tree output, then
            //  consume the token to check for the ; that completes the
            //  <in> part of the grammer
            myNode->child2 = create_node("Identifier");
            myNode->child2->myToken = tk;
            //  now consume another token and check for the ;
            tk = filter(fp, lineNum);
            if(tk.instance == ";"){
                //  Don't need to add the ;
                tk = filter(fp, lineNum);
                return myNode;
            }
            else{
                //  Error message.
                printf("Incorrect token:\"%s\" at line %d, expected \";\".\n", tk.instance.c_str(), lineNum);
                exit(1);
            }
        }
        else{
            //  Error message.
            printf("Incorrect token: \"%s\" at line %d, expected an Identifer.\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }
    else{
        //  Error message.
        printf("Incorrect token: \"%s\" at line %d, expected \">>\".\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <out>   ->    print >> <expr>                     */
/******************************************************/

node_t* out(){
    node_t* myNode = create_node("<out>");
    
    if(tk.instance == "<<"){
        //  Now that we have got the <<, we consume that token, and move to
        //  check for <expr>.
        myNode->child1 = create_node("<<");
        tk = filter(fp, lineNum);
        myNode->child2 = expr();
        return myNode;
    }
    else{
        //  Error message.
        printf("Incorrect token: \"%s\" at line %d, expected \"<<\" .\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <if>   -> decision(<expr> <RO> <expr>)<block>     */
/******************************************************/

node_t* my_if(){
    node_t* myNode = create_node("<if>");
    
    if(tk.instance == "("){
        //consume the token and call functions for rest of grammer
        tk = filter(fp, lineNum);
        myNode->child1 = expr();
        myNode->child2 = RO();
        myNode->child3 = expr();
        if(tk.instance == ")"){
            // consume token
            tk = filter(fp, lineNum);
            myNode->child4 = block();
            return myNode;
        }
        else{
            //  Error message.
            printf("Incorrect token: \"%s\" at line %d, expected \")\".\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }
    else{
        //  Error message.
        printf("Incorrect token: \"%s\" at line %d, expected \"(\" .\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <loop>   -> while(<expr> <RO> <expr>)<block>      */
/******************************************************/

node_t* loop(){
    node_t* myNode = create_node("<loop>");
    
    if(tk.instance == "("){
        //consume the token and call functions for rest of grammer
        tk = filter(fp, lineNum);
        myNode->child1 = expr();
        myNode->child2 = RO();
        myNode->child3 = expr();
        if(tk.instance == ")"){
            // consume token
            tk = filter(fp, lineNum);
            myNode->child4 = block();
            return myNode;
        }
        else{
            //  Error message.
            printf("Incorrect token:\"%s\" at line %d, expected \")\".\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    }
    else{
        //  Error message.
        printf("Incorrect token: \"%s\" at line %d, expected \"(\".\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <assign>   -> Identifier = <expr> ;               */
/******************************************************/
node_t* assign() {
    node_t* myNode = create_node("<assign>");
    
    myNode->child1 = create_node("Identifier");
    myNode->child1->myToken = tk;
    tk = filter(fp, lineNum);
    
    if (tk.instance == "=") {
        myNode->child2 = create_node("=");
        // next we check for <expr>
        tk = filter(fp, lineNum);
        myNode->child3 = expr();
        if (tk.instance == ";") {
            //  now that we have completed the assignment ,
            //  we consume the token to prepare for any further calls.
            tk = filter(fp, lineNum);
            return myNode;
        } else {
            //  Error message.
            printf("Incorrect token: \"%s\" at line %d, expected \";\".\n", tk.instance.c_str(), lineNum);
            exit(1);
        }
    } else {
        //  Error message.
        printf("Incorrect token: \"%s\" at line %d, expected \"=\".\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}

/******************************************************/
/*  <RO>   ->       << | <<= | >>= | >> | == | =!     */
/******************************************************/

node_t* RO(){
    node_t* myNode = create_node("<RO>");
    
    if(tk.instance == "<<"){
        myNode->child1 = create_node("<<");
        //  consume the token to test for further option
        tk = filter(fp, lineNum);
        if(tk.instance == "="){
            // this is a final form
            myNode->child1 = create_node("<<=");
            tk = filter(fp, lineNum);
            return myNode;
        }
        else{
            return myNode;
        }
    }
    else if(tk.instance == ">>"){
        myNode->child1 = create_node(">>");
        //  consume the token to test for further option
        tk = filter(fp, lineNum);
        if(tk.instance == "="){
            // this is a final form
            myNode->child1 = create_node(">>=");
            tk = filter(fp, lineNum);
            return myNode;
        }
        else{
            return myNode;
        }
    }
    else if(tk.instance== "=="){
        myNode->child1 = create_node("==");
        //  Consume the token
        tk = filter(fp, lineNum);
        return myNode;
    }
    else if(tk.instance == "=!"){
        myNode->child1 = create_node("=!");
        //  Consume the token
        tk = filter(fp, lineNum);
        return myNode;
    }
    else{
        //  Error message.
        printf("Incorrect token: \"%s\" at line %d, not valid for case <RO>.\n", tk.instance.c_str(), lineNum);
        exit(1);
    }
}




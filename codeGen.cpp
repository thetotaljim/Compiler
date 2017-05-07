//
//  codeGen.cpp
//  P4_X2
//
//  Created by James Steimel on 5/3/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//


#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "codeGen.h"
#include <map>
#include <string>

/************************************************************/
/*  Global variable declarations                            */
/************************************************************/
//  Vector to hold the identifiers and check scope.
std::vector<std::string> scopeStack;
//  Used to track the scope of variables.
int scope_index = 0;
//	Used to track PUSHs
int number_pops = 0;
//	Vector to hold temporary variables.
std::vector<std::string> temp;
//	Number of Labels created.
int labelCt = 0;
//  Number of identifiers.
int total_num_identifiers = 0;
//	Target file for output in asm
FILE *outFile;
// Value-Defintions of the different String values
enum StringValue {
    evNotDefined,
    evStringValue1,
    evStringValue2,
    evStringValue3,
    evStringValue4,
    evStringValue5,
    evStringValue6,
    evStringValue7,
    evStringValue8,
    evStringValue9,
    evStringValue10,
    evStringValue11,
    evStringValue12,
    evStringValue13,
    evStringValue14,
    evStringValue15,
    evStringValue16,
    evStringValue17,
    evStringValue18,
    evStringValue19,
    evEnd
};
/************************************************************/
/*      initOutFile() function                              */
/*                                                          */
/*      Point to target file determined in main.            */
/************************************************************/
void initOutFile(FILE *of) {
    outFile = of;
}
/************************************************************/
/*      newTemp() function                                  */
/*                                                          */
/*      Generate a temp label to be used in the target file.*/
/************************************************************/
std::string newTemp() {
    std::ostringstream ostringstream1;
    ostringstream1 << "T" << total_num_identifiers++;
    temp.push_back(ostringstream1.str());
    return ostringstream1.str();
}

/************************************************************/
/*      newLabel() function                                 */
/*                                                          */
/*      Generate a label to be used in the target file.     */
/************************************************************/
std::string newLabel() {
    std::ostringstream ostringstream1;
    ostringstream1 << "L" << labelCt++;
    return ostringstream1.str();
}

/************************************************************/
/*      printScope() function                               */
/*                                                          */
/*      Output stack variablet to target file.              */
/************************************************************/
void printScope() {
    for (int i = (int)scopeStack.size() - 1; i >= 0; i--) {
        std::cout << scopeStack[i] << std::endl;
    }
}

// Map to associate the strings with the enum values
static std::map<std::string, StringValue> s_mapStringValues;

/************************************************************/
/*      Initialize() function                               */
/*                                                          */
/*      Maps the node labels to enums to use in             */
/*      the switch statement in semantic check.             */
/************************************************************/

void Initialize()
{
    s_mapStringValues["Identifier"] = evStringValue1;
    s_mapStringValues["<program>"] = evStringValue2;
    s_mapStringValues["<block>"] = evStringValue3;
    s_mapStringValues["<var>"] = evStringValue4;
    s_mapStringValues["<mvars>"] = evStringValue5;
    s_mapStringValues["<expr>"] = evStringValue6;
    s_mapStringValues["<M>"] = evStringValue7;
    s_mapStringValues["<T>"] = evStringValue8;
    s_mapStringValues["<F>"] = evStringValue9;
    s_mapStringValues["<R>"] = evStringValue10;
    s_mapStringValues["<stats>"] = evStringValue11;
    s_mapStringValues["<mStat>"] = evStringValue12;
    s_mapStringValues["<stat>"] = evStringValue13;
    s_mapStringValues["<in>"] = evStringValue14;
    s_mapStringValues["<out>"] = evStringValue15;
    s_mapStringValues["<if>"] = evStringValue16;
    s_mapStringValues["<loop>"] = evStringValue17;
    s_mapStringValues["<assign>"] = evStringValue18;
    s_mapStringValues["<RO>"] = evStringValue19;
    s_mapStringValues["end"] = evEnd;
}
/************************************************************/
/*      checkScope(token) function                          */
/*                                                          */
/*      Here we check the identifiers on the stack, and     */
/*      and if a match is found, the program is terminated. */
/************************************************************/

void checkScope(token tk){
    //  Loop through the scopeStack, and check for identifiers that have already been defined.
    for (int i = scope_index; i < scopeStack.size(); i++) {
        //  If a match is found, it's a scoping error.
        if (scopeStack[i] == tk.instance) {
            printf("Scoping Error: Line %d: token instance \"%s\" already defined.\n", tk.lineNum, tk.instance.c_str());
            exit(0);
        }
    }
}

/************************************************************/
/*      popStack() function                                 */
/*                                                          */
/*      PopStack function will take a stack index, and      */
/*      pop all the identifiers following the index         */
/*      off the stack.                                      */
/************************************************************/

void popStack(int index) {
    //	Loop through and pop all elements in range off stack
    for(int i = (int)scopeStack.size(); i > index; i--){
        scopeStack.pop_back();
        fprintf(outFile, "%s", static_cast<std::string>("POP\n").c_str());
        number_pops--;
    }
}

/************************************************************/
/*      find() function                                     */
/*                                                          */
/*      Find function checks to see if the identifier       */
/*      passed is found in the current scope.               */
/*      If not, -1 is returned.                             */
/************************************************************/

int find(std::string ident) {
    //	Loop through the scoped indentifiers
    for (int i = (int)scopeStack.size() - 1; i > -1; i--) {
        //	If a match is found
        if (scopeStack[i] == ident) {
            // Size of stack - index location - 1
            return (int)scopeStack.size() - i - 1;
        }
    }
    //	If it is not found in scope, return -1
    return -1;
}

/************************************************************/
/*      codeGenInit() function                             */
/*                                                          */
/*      Before checking semantics, the different node       */
/*      types need to be turned into enums for the          */
/*      switch statment by calling the Initialze() function.*/
/************************************************************/

void codeGenInit(node_t* node){
    Initialize();
    codeGen(node);
}

/************************************************************/
/*      codeGen(node)  function                             */
/*                                                          */
/*      This function will take the parse tree, and         */
/*      check the scope of any identifiers. If the          */
/*      scop is bad, the program terminates. Otherwise,     */
/*      a message alerts to a successful sematics check.    */
/************************************************************/

void codeGen(node_t* node) {
    
    if (node == NULL) {
        return;
    } else {
        std::string label = node->label;
        
        switch(s_mapStringValues[label])
        {
                 //
                 //      Case:        <program>
                 //
            case evStringValue2:
            {   //  Check <var>
                codeGen(node->child1);
                //  Check <block>
                codeGen(node->child2);
                int tracker = 0;
                for (int cnt = 0; cnt < number_pops; cnt ++){
                    fprintf(outFile, "%s", static_cast<std::string>("POP\n").c_str());
                    tracker++;
                }
                fprintf(outFile, "%s", static_cast<std::string>("STOP\n").c_str());
                //  Print all variables after STOP statement in Target.
                for (int i = 0; i < scopeStack.size(); i++) {
                    fprintf(outFile, "%s", static_cast<std::string>(scopeStack[i] + " 0\n").c_str());
                }
                for (int i = 0; i < temp.size(); i++) {
                    fprintf(outFile, "%s", static_cast<std::string>(temp[i] + " 0\n").c_str());
                }
                //  Pop everything off the stack
                popStack(scope_index);
                //  If we reach this line, semantics are good.
                printf("\nFinished semantic check, no issues.\n");
                break;
            }
                //
                //       Case:        <block>
                //
            case evStringValue3:
            {
                //  Record size of stack at beginning of block
                int blck_indx = (int)scopeStack.size();
                codeGen(node->child1);
                codeGen(node->child2);
                //  Pop the local identifiers off the stack at
                //  the end of a block.
                popStack(blck_indx);
                break;
            }
                //
                //         Case        <var>
                //
            case evStringValue4:
            {
                //  Set stack index to size of stack
                scope_index = (int)scopeStack.size();
                //  If the <var> is not empty
                if (node->child1 != NULL) {
                    //  check the scope of the Identifer
                    checkScope(node->child1->myToken);
                    //  and if scope checks out, push to stack.
                    scopeStack.push_back(node->child1->myToken.instance);
                    fprintf(outFile, "%s", static_cast<std::string>("PUSH\n").c_str());
                    number_pops++;
                }
                if (node->child2 != NULL){
                    //  Then check the <mvars>
                    codeGen(node->child2);
                }
                break;
            }
                //
                //          Case        <mVars>
                ///
            case evStringValue5:
            {
                //  If <mvars> is not empty,
                if (node->child1 != NULL) {
                    //  check the scope of its associated identifier
                    checkScope(node->child1->myToken);
                    //  and if the scope checks out, add the identifier to the stack
                    scopeStack.push_back(node->child1->myToken.instance.c_str());
                    fprintf(outFile, "%s", static_cast<std::string>("PUSH\n").c_str());
                    number_pops++;
                }
                //  Check for any other <mvars>
                codeGen(node->child2);
                break;
                
            }
                //
                //      Case:           <expr>
                //
            case evStringValue6:
            {
                //  if child3 is not null, and child2 is the multiplication symbol,
                //  then this is case <M> * <expr>
                if (node->child3 != NULL && node->child2->label == "*") {
                    //  Check the <expr> first, eval the Right side first
                    codeGen(node->child3);
                    std::string tempVar = newTemp();
                    fprintf(outFile, "STORE %s\n", tempVar.c_str());
                    //  then the left side, check the <M> node
                    codeGen(node->child1);
                    fprintf(outFile, "MULT %s\n", tempVar.c_str());
                }
                else {
                    //  Otherwise, should be an empty <expr>
                    codeGen(node->child1);
                }
                
                break;
                
            }
                //
                //      Case:           <M> -> <T> / <M> | <T>
                //
            case evStringValue7:
            {
                //  Check for case <M> -> <T>
                if (node->child2 != NULL && node->child2->label == "/") {
                    //  Check the righthand <M> node first
                    codeGen(node->child3);
                    std::string tempVar = newTemp();
                    fprintf(outFile, "STORE %s\n", tempVar.c_str());
                    //  Then check the <T> node.
                    codeGen(node->child1);
                    fprintf(outFile, "DIV %s\n", tempVar.c_str());
                } else{
                    //  Or check the <T> node
                    codeGen(node->child1);
                }
                break;
                
            }
                //
                // 		Case		<T> ->   <F> + <T> | <F> - <T> | <F>
                //
                //
            case evStringValue8:
            {
                //  Check case <F> + <T>
                if (node->child2 != NULL && node->child2->label == "+") {
                    //  Check the <T> node first
                    codeGen(node->child3);
                    std::string tempVar = newTemp();
                    fprintf(outFile, "STORE %s\n", tempVar.c_str());
                    //  Then check left side <F>
                    codeGen(node->child1);
                    fprintf(outFile, "ADD %s\n", tempVar.c_str());
                
                }
                //  Check case <F> - <T>
                else if (node->child2 != NULL && node->child2->label == "-") {
                    //  Start by checking Right side <T> node.
                    codeGen(node->child3);
                    std::string tempVar = newTemp();
                    fprintf(outFile, "STORE %s\n", tempVar.c_str());
                    //  Check left side <F> node.
                    codeGen(node->child1);
                    fprintf(outFile, "SUB %s\n", tempVar.c_str());
                    
                } else {
                    //  Check for <F>
                    codeGen(node->child1);
                }
                break;
            }
                //
                //		Case 	<F> -> & <F> | <R>
                //
            case evStringValue9:
            {
                //  Check case & <F>
                if (node->child1->label == "&") {
                    //  Then check <F>
                    codeGen(node->child2);
                    fprintf(outFile, "MULT -1\n");
                } else
                    //  Or check <R>
                    codeGen(node->child1);
                break;
            }
                //
                //			Case 			<R>
                //
            case evStringValue10:
            {
                //  Check for an identifier
                if (node->child1->label == "Identifier") {
                    //  See if the related token is declared in this scope.
                    int scope_val = find(node->child1->myToken.instance);
                    //  If -1 is returned, there is a scoping issue.
                    if (scope_val == -1){
                        printf("Scoping Error: Line %d: token instance \"%s\" undefined.\n", node->child1->myToken.lineNum, node->child1->myToken.instance.c_str());
                        exit(0);
                    }
                    fprintf(outFile, "STACKR %d\n", scope_val);
                }
                else if (node->child1->label == "Number") {
                    fprintf(outFile, "LOAD %s\n", node->child1->myToken.instance.c_str());
                }
                else {
                    //  Otherwise check the sematics of the identifier.
                    codeGen(node->child1);
                }
                
                break;
                
            }
                //
                // 			Case 				<stats>
                //
            case evStringValue11:
            {
                //  For stats, just recursive call for possible child nodes.
                codeGen(node->child1);
                codeGen(node->child2);
                break;
            }
                //
                // 			Case				<mStats>
                //
            case evStringValue12:
            {
                //  For mStats, just recursive call for possible child nodes.
                codeGen(node->child1);
                codeGen(node->child2);
                break;
            }
                //
                // 			Case				<stat>
                //
            case evStringValue13:
            {
                if(node->child1 != NULL){
                //  For stat, just recursive call for possible child nodes.
                    codeGen(node->child1);
                }
                if(node->child2 != NULL){
                    codeGen(node->child2);
                }
                if(node->child3 != NULL){
                    codeGen(node->child3);
                }
                break;
            }
                //
                //      Case:                   <in>
                //
            case evStringValue14:
            {
                
                //  Get the instance for the Identifer.
                std::string varInstance = node->child2->myToken.instance;
                //   Check if it has been declare in this scope.
                int scope_val = find(varInstance);
                if (scope_val == -1) {
                    printf("Scoping Error: Line %d: token instance \"%s\" undefined.\n", node->child2->myToken.lineNum, node->child2->myToken.instance.c_str());
                    exit(0);
                }
                std::string tempVar = newTemp();
                //write to target
                fprintf(outFile, "READ %s\n", tempVar.c_str());
                fprintf(outFile, "LOAD %s\n", tempVar.c_str());
                fprintf(outFile, "STACKW %d\n", scope_val);
                //   Check any possible child nodes
                codeGen(node->child1);
                codeGen(node->child2);
                codeGen(node->child3);
                codeGen(node->child4);
                break;
                
            }
                //
                // 		Case 				<out>
                //
            case evStringValue15:
            {
                //  Here we check the <expr> from the out statement
                codeGen(node->child2);
                std::string tempVar = newTemp();
                //write to target
                fprintf(outFile, "STORE %s\n", tempVar.c_str());
                fprintf(outFile, "WRITE %s\n", tempVar.c_str());
                break;
            }
                //
                //      Case            <if>
                //
            case evStringValue16:
            {
                //  child1 = expr
                //  child2 = RO
                //  child3 = expr
                //  Need RO for write to target
                std::string RO = node->child2->child1->label;
                // Start by checking the expression from the Right hand side.
                codeGen(node->child3);
                std::string tempVar = newTemp();
                fprintf(outFile, "STORE %s\n", tempVar.c_str());\
                //  Now check the semantics of the left side expression
                codeGen(node->child1);
                fprintf(outFile, "SUB %s\n", tempVar.c_str());
                //  Result is now in ACC,
                //  determine which BR command to write to target.
                std::string Label = newLabel();
                if (RO == ">>") {
                    fprintf(outFile, "BRZNEG %s\n", Label.c_str());
                } else if (RO == "<<") {
                    fprintf(outFile, "BRZPOS %s\n", Label.c_str());
                } else if (RO == ">>=") {
                    fprintf(outFile, "BRNEG %s\n", Label.c_str());
                } else if (RO == "<<=") {
                    fprintf(outFile, "BRPOS %s\n", Label.c_str());
                } else if (RO == "==") {
                    fprintf(outFile, "BRPOS %s\n",
                            Label.c_str());
                    fprintf(outFile, "BRNEG %s\n", Label.c_str());
                } else if (RO == "=!") {
                    fprintf(outFile, "BRZERO %s\n", Label.c_str());
                }
                //  Now the block that follows.
                codeGen(node->child4);
                fprintf(outFile, "%s: NOOP\n", Label.c_str());
                break;
                
            }
                //
                //			Case 				<loop>
                //
            case evStringValue17:
            {
                //  Set up for writing to target
                std::string RO = node->child2->child1->label;
                std::string tempVar = newTemp();
                std::string startLabel = newLabel();
                std::string endLabel = newLabel();
                fprintf(outFile, "%s: ", startLabel.c_str());
                //  Check the Right side val
                codeGen(node->child3);
                fprintf(outFile, "STORE %s\n", tempVar.c_str());
                //  And then Left side val
                codeGen(node->child1);
                fprintf(outFile, "SUB %s\n", tempVar.c_str());
                // Result is in ACC
                // Get BR case
                if (RO == ">>") {
                    fprintf(outFile, "BRZNEG %s\n", endLabel.c_str());
                } else if (RO == "<<") {
                    fprintf(outFile, "BRZPOS %s\n", endLabel.c_str());
                } else if (RO == ">>=") {
                    fprintf(outFile, "BRNEG %s\n", endLabel.c_str());
                } else if (RO == "<<=") {
                    fprintf(outFile, "BRPOS %s\n", endLabel.c_str());
                } else if (RO == "==") {
                    fprintf(outFile, "BRPOS %s\n", endLabel.c_str());
                    fprintf(outFile, "BRNEG %s\n", endLabel.c_str());
                } else if (RO == "=!") {
                    fprintf(outFile, "BRZERO %s\n", endLabel.c_str());
                }
    
                codeGen(node->child4);
                fprintf(outFile, "BR %s\n", startLabel.c_str());
                fprintf(outFile, "%s: NOOP\n", endLabel.c_str());
                break;
            }
                //
                // 		Case:           <assign>
                //
            case evStringValue18:
            {
                //  Check the Number associated with the assignment statement
                codeGen(node->child3);
                //  Get the value from the Identifier
                int scope_val = find(node->child1->myToken.instance);
                //  If -1, scoping error.
                if (scope_val == -1){
                    printf("Scoping Error: Line %d: token instance \"%s\" undefined.\n", node->child1->myToken.lineNum, node->child1->myToken.instance.c_str());
                    exit(0);
                }
                fprintf(outFile, "STACKW %d\n", scope_val);
                break;
            }
                //
                //          Case:              <RO>
                //
            case evStringValue19:
            {
                //  Check child node just to be safe.
                codeGen(node->child1);
                break;
            }
            default:
            {
                codeGen(node->child1);
                codeGen(node->child2);
                codeGen(node->child3);
                codeGen(node->child4);
                break;
            }
        }
    }
}





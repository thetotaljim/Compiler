//
//  treePrint.cpp
//  P4_X2
//
//  Created by James Steimel on 5/3/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "treePrint.h"

//  Token names for lookup with token enumeration
//  This mirrors the token names from the scanner.
std::string token_names2[] = {
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
    "Operator",
    "Delimeter",
    "End of File",
    "Error"
};


/********************************************************/
/*  printTree function                                  */
/*                                                      */
/*  Print Tree function uses the preorder traversal     */
/*  from P0 to print the parsed program's tree output.  */
/********************************************************/

void printTree(node_t *root_node, int level) {
    //  If we have an empty child, we return
    //  and either further processing occurs or
    //  the tree printing is finished.
    if (root_node == NULL) {
        return;
    }
    else {
        //  Or If we have a node to process, we use the
        //  level variable to space out the output.
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        //  increment level.
        level++;
        // Here we print the node's label.
        printf("%s  ", (root_node->label).c_str());
        //  Then check to see if the node has a token it points to (for Idents, Numbers, and Operators/Relational/Deliminators.)
        token myToken = root_node->myToken;
        //  here we check to see if there is a instance for the token, and if so, It is displayed.
        if (myToken.instance != "") {
            printf(": tk= %s : label=%s ", (token_names2[myToken.tokenId]).c_str(),  (myToken.instance).c_str());
        }
        //  New line.
        printf("\n");
        //  And now we recusively process any child nodes
        if(root_node->child1 != NULL){
            printTree(root_node->child1, level);
        }
        if(root_node->child2 != NULL){
            printTree(root_node->child2, level);
        }
        if(root_node->child3 != NULL){
            printTree(root_node->child3, level);
        }
        if(root_node->child4 != NULL){
            printTree(root_node->child4, level);
        }
    }
}




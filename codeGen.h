//
//  codeGen.h
//  P4_X2
//
//  Created by James Steimel on 5/3/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//

#ifndef codeGen_h
#define codeGen_h

#include "node.h"

void setTargetFile(FILE* of);
void codeGenInit(node_t* node);
void codeGen(node_t*);

#endif /* codeGen_h */



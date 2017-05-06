//
//  parser.h
//  P4_X2
//
//  Created by James Steimel on 5/3/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include "node.h"
//  Prototype for functions
node_t* prog();
node_t* block();
node_t* expr();
node_t* vars();
node_t* RO();
node_t* stats();
node_t* mvars();
node_t* T();
node_t* M();
node_t* F();
node_t* R();
node_t* stat();
node_t* mStat();
node_t* assign();
node_t* in();
node_t* out();
node_t* my_if();
node_t* loop();

// Parser function
node_t* parser(FILE*);

#endif /* parser_h */




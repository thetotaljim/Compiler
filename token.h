//
//  token.h
//  P4_X2
//
//  Created by James Steimel on 5/3/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//

#ifndef token_h
#define token_h

#include <iostream>

//  The tokens as enumerated values.

enum tokenID {
    IDENT_tk,   /*      Identifer       */
    NUM_tk,     /*      Number          */
    
    START_tk,   /*      Keywords        */
    STOP_tk,
    DECS_tk,
    WHLE_tk,
    VOID_tk,
    INT_tk,
    RET_tk,
    READ_tk,
    PRINT_tk,
    PROG_tk,
    
    REL_tk,     /*      Relational      */
    OP_tk,      /*      Operator        */
    DEL_tk,     /*      Delimiters      */
    EOF_tk,     /*      End Of File     */
    ERR_tk      /*      Error           */
};

//  The token structure suggested in the P1 instructions.

struct token {
    tokenID tokenId;
    std::string instance;
    int lineNum;
};

#endif /* token_h */




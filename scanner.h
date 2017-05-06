
//
//  scanner.h
//  P4_X2
//
//  Created by James Steimel on 5/3/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//

#ifndef scanner_h
#define scanner_h

#include <stdio.h>
#include "token.h"
#include <iostream>

//  Scanner function prototype.
token scanner(FILE *input_file, int &lineNum);
token filter(FILE*, int&);
std::string getTokenString(token the_token);


#endif /* scanner_h */



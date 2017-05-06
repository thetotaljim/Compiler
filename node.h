//
//  node.h
//  P4_X2
//
//  Created by James Steimel on 5/3/17.
//  Copyright © 2017 James Steimel. All rights reserved.
//

#ifndef node_h
#define node_h

#include <string>
#include "scanner.h"
#include "token.h"

using namespace std;

struct node_t {
    string label;
    node_t *child1, *child2, *child3, *child4;
    token myToken;
};

#endif /* node_h */



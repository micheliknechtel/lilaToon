#include "stdafx.h"

#define stackSize 16777216
int _stack[stackSize];
int _stackPointer;

bool pop(int
&x, int &y, int &h) 
{ 
    if(_stackPointer > 0) 
    { 
        int p = _stack[_stackPointer]; 
        x = p / h; 
        y = p % h; 
        _stackPointer--; 
        return 1; 
    }     
    else 
    { 
        return 0; 
    }    
}    

bool push(int x, int y, int &h) 
{ 
    if(_stackPointer < stackSize - 1) 
    { 
        _stackPointer++; 
        _stack[_stackPointer] = h * x + y; 
        return 1; 
    }     
    else 
    { 
        return 0; 
    }    
}     

void emptyStack() 
{ 
    int x, y, w; 
    while(pop(x, y,w)); 
}

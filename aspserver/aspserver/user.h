//*******************************************************************************
// USER
//   Handles information about the users logged in 
//   
// @author Doug Frazer
// February 2013
//*******************************************************************************

#include <stdint.h>

#ifndef __USER_H__
#define __USER_H__

class USER
{
public: // constructor/deconstructor
    USER(uint32_t);
    ~USER();

public: // functions
    static bool IsUserLoggedIn(uint32_t UserId);

public: // data
    uint32_t id;
    uint32_t x;
    uint32_t y;

private:
    static USER** FindUser(uint32_t id);
    void SetInitialPosition();
    void RemoveFromTree();

private: // data
    // For the binary search tree, if we just need to find a single user
    USER* Left;
    USER* Right;
};

#endif


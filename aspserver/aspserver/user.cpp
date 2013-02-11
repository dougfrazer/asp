//*******************************************************************************
// USER
//   Handles information about the users logged in 
//   
// @author Doug Frazer
// February 2013
//*******************************************************************************

#include <list>
#include <stdio.h>

#include "user.h"
#include "world.h"
#include "network.h"

std::list<USER*> UserList;
static USER* UserTree;

//*******************************************************************************
USER::USER(uint32_t _id)
{
    UserList.push_front(this);

    USER** FoundUser = FindUser(id);
    if(FoundUser == NULL)  {
        printf("Error finding user\n");
    } else if(*FoundUser != NULL) {
        printf("Error user already exists with that ID\n");
    } else {
        *FoundUser = this;
    }
    id = _id;
    SetInitialPosition();

    for(std::list<USER*>::iterator it = UserList.begin(); it != UserList.end(); ++it) {
        USER* User = *it;
        Network_UpdatePosition(User->id, User->x, User->y);
    }
}
//*******************************************************************************
USER::~USER()
{
    printf("Destroying user %d\n", id);
    UserList.remove(this);
    RemoveFromTree();
}
//*******************************************************************************
USER** USER::FindUser(uint32_t id)
{
    USER** Node = &UserTree;
    for(uint32_t i = 1; i > 0 && *Node != NULL && (*Node)->id != id; i << 1) {
        Node = (id & i) > 0 ? &(*Node)->Left : &(*Node)->Right;
    }
    return Node;
}
//*******************************************************************************
void USER::SetInitialPosition()
{
    World_SetInitialPosition(id, &x, &y);
}
//*******************************************************************************
void USER::RemoveFromTree()
{
    USER** Node = &UserTree;
    USER** Parent = NULL;
    for(uint32_t i = 0; i > 0 && *Node != NULL && (*Node)->id != id; i << 1) {
        Parent = Node;
        Node = (id & i) > 0 ? &(*Node)->Left : &(*Node)->Right;
    }
    if(Parent == NULL) {
        UserTree = NULL;
    } else if(Node == NULL) {
        printf("Error could not find user in the binary search tree\n");
    } else if((*Node)->Left == NULL && (*Node)->Right == NULL) {
        (*Parent)->Right = *Node == (*Parent)->Right ? NULL : (*Parent)->Right;
        (*Parent)->Left = *Node == (*Parent)->Left ? NULL : (*Parent)->Left;
    } else if((*Node)->Left == NULL && (*Node)->Right != NULL) {
        *Node = (*Node)->Right;
    } else if((*Node)->Left != NULL && (*Node)->Right == NULL) {
        *Node = (*Node)->Left;
    } else {
        *Node = (*Parent)->Left == *Node ? (*Node)->Left : (*Node)->Right;
    }
}
//*******************************************************************************
bool USER::IsUserLoggedIn(uint32_t UserId)
{
   USER** User = FindUser(UserId);
   if(User == NULL) {
       printf("Error finding user...\n");
   }
   return *User != NULL;
}

//******************************************************************************
// Test interface
//******************************************************************************

#ifndef __TEST_H__
#define __TEST_H__

class TEST
{
public:
    TEST(const char* Title);
    virtual ~TEST() {};

public:
    bool Run       ( void );
    static TEST* GetFirst ( void );
    static TEST* GetNext  ( TEST* Test );

protected:
    virtual bool RunInternal ( void ) = 0;
    virtual void Setup       ( void ) {}
    virtual void Cleanup     ( void ) {}
    bool Passed;

private:
    char Buffer[100];
    TEST* Next;
    TEST* Prev;
};

#endif


#include "ai_public.h"

class ANIMATION
{
public:
    virtual void Update(float DeltaTime) = 0;
};

class TEST_ANIMATION : public ANIMATION
{
    void Update(float DeltaTime);
};

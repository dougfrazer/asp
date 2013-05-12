
#include "ai_public.h"

class ANIMATION
{
public:
    virtual void Update(float DeltaTime, SKELETON* Skeleton) = 0;
};

class TEST_ANIMATION : public ANIMATION
{
public:
    void Update(float DeltaTime, SKELETON* Skeleton);
};

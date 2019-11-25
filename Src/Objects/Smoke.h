#ifndef SMOKE_H_INCLUDED
#define SMOKE_H_INCLUDED

#include "../Math/Vector.h"

class Shader;
class Sprite;

class Smoke {
private:
    const float MIN_SCALE = 2.f;
    const float RANGE = 20.f - MIN_SCALE;
    
    Sprite* sprite;
    
    const float LIFESPAN = 2.f;
    float age;
    bool markedForRemoval;
    
public:
    Smoke(Shader* shd, const Vector3f& position);
    ~Smoke();
    
    void update(float timestep);
    void render() const;
};

#endif // SMOKE_H_INCLUDED

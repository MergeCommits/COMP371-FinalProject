#ifndef SMOKE_H_INCLUDED
#define SMOKE_H_INCLUDED

#include "../Math/Vector.h"

class Shader;
class Sprite;

class Smoke {
private:
    const float MIN_SCALE = 0.2f;
    const float RANGE = 4.f - MIN_SCALE;
    
    Sprite* sprite;
    
    const float LIFESPAN = 1.f;
    float age;
    bool markedForRemoval;
    
public:
    Smoke(Shader* shd, const Vector3f& position);
    ~Smoke();
    
    bool isMarkedForRemoval() const;
    
    void update(float timestep);
    void render() const;
};

#endif // SMOKE_H_INCLUDED

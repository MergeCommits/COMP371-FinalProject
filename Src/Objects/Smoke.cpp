#include "Smoke.h"
#include "../Graphics/Sprite.h"

Smoke::Smoke(Shader* shd, const Vector3f& position) {
    sprite = new Sprite(shd);
    sprite->setPosition(position);
    
    age = 0.f;
    markedForRemoval = false;
}

Smoke::~Smoke() {
    delete sprite;
}

bool Smoke::isMarkedForRemoval() const {
    return markedForRemoval;
}

void Smoke::update(float timestep) {
    if (markedForRemoval) { return; }
    
    age += timestep;
    float frac = age / LIFESPAN;
    sprite->setOpacity(1.f - frac);
    sprite->setScale(MIN_SCALE + (RANGE * frac));
    sprite->update();
    
    if (age >= LIFESPAN) {
        markedForRemoval = true;
    }
}

void Smoke::render() const {
    if (markedForRemoval) { return; }
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    sprite->render();
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
}

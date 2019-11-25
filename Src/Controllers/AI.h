#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

class Car;
class Shader;

class AI {
private:
    Car* car;
    
    int stepsUntilNewTask;

public:
    AI(Shader* shd, Shader* colliderShd);
    ~AI();
    
    void setCarShader(Shader* shd);
    Car* getCar() const;
    
    void update(float timestep);
    void render() const;
};

#endif // AI_H_INCLUDED

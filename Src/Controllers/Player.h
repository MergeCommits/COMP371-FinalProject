#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

class Car;
class GLFWwindow;
class Shader;

class Player {
private:
    Car* car;
    
public:
    Player(Shader* shd);
    ~Player();
    
    void setShader(Shader* shd);
    
    void update(float timestep, GLFWwindow* window);
    void render() const;
};

#endif // PLAYER_H_INCLUDED

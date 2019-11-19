#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

class Camera;
class Car;
struct GLFWwindow;
class Shader;

class Player {
private:
    Camera* camera;
    Car* car;

	bool cameraFollowingCar;
    
public:
    Player(Shader* shd, int camWidth, int camHeight);
    ~Player();
    
    Camera* getCamera() const;
    
    void setCarShader(Shader* shd);
    
    void update(float timestep, GLFWwindow* window);
    void render() const;
};

#endif // PLAYER_H_INCLUDED

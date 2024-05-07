
#ifndef point_h
#define point_h

#include <linesdiscoverysystem/readfiles.h>


class Point
{
public:
    
    //Default constructor
    Point();

    //Constructor
    Point(glm::vec2 pos){
        position = pos;
    }
    
    //Destructor
    ~Point() {};
    
    glm::vec2 getPosition() const {return position;}
    
    void setPositionX(float val) { position.x = val;}
    
    void setPositionY(float val) { position.y = val;}
    
    void editPosition(float val) {position /= val;}
    
private:
    
    glm::vec2 position{};
    
};

struct line {
    Point start = glm::vec2(0,0);
    Point end = glm::vec2(0,0);
    float k, m;
};



#endif /* point_h */

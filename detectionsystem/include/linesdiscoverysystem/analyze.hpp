
#ifndef analyze_hpp
#define analyze_hpp

#include <linesdiscoverysystem/readfiles.h>
#include <stdio.h>
#include <fstream>
#include <unordered_set>
#include <functional>
#include <map>
#include "/Users/viktorsvensson/Desktop/lab3-part2/detectionsystem/include/linesdiscoverysystem/point.h"

struct kAndM {
    float k, m;
};



class Analyze
{
public:
    
    //Default constructor
    Analyze() {};

    //Destructor
    ~Analyze() {};
    
    void readPoints(std::istream& is);
    
    void getLineSegments();
    
    void writeLineSegments(const std::string& baseFilename);
    
    float intercept(const Point& p, float slope);
    
    void percolateUp(std::vector<Point>& points);
    
    void myQuickSort(std::vector<Point>& points);
    
    
    void collectCollinearGroups(std::vector<std::vector<Point>>& collectedGroups);
    
    void sortCollinearPoints(std::vector<std::vector<Point>>& groups);
    
    void addUniqueLines(const std::vector<std::vector<Point>>& groups);
    
    std::vector<rendering::Point> readLineSegments();
    
    float slope(glm::vec2 p, glm::vec2 q);
    
private:
    
    std::vector<Point> points;
    std::vector<std::vector<Point>> lines;
    std::vector<line> uniqueLines;
    
};



#endif /* analyze_hpp */


#include "/Users/viktorsvensson/Desktop/lab3-part2/detectionsystem/include/linesdiscoverysystem/analyze.hpp"

void Analyze::percolateUp(std::vector<Point>& points){
    size_t j = points.size() - 1;
    
    while(j > 0){
        if(points[j].getPosition().y < points[j-1].getPosition().y){
            std::swap(points[j], points[j-1]);
            j--;
        }else if(points[j].getPosition().y == points[j-1].getPosition().y && points[j].getPosition().x < points[j-1].getPosition().x){
            std::swap(points[j], points[j-1]);
            j--;
        }else
            j = 0;
    }
}

std::vector<rendering::Point> Analyze::readLineSegments() {
    std::vector<rendering::Point> linesRead;

    std::cout << "\n Kommer in i readLineSegments.\n";
    
    rendering::Point start{glm::vec2{}, glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}, 0.002f};
    rendering::Point end{glm::vec2{}, glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}, 0.002f};
    for (auto& line : uniqueLines) {
        
        start.position.x = line.start.getPosition().x / 32767.0;
        start.position.y = line.start.getPosition().y /32767.0;
        
        end.position.x = line.end.getPosition().x / 32767.0;
        end.position.y = line.end.getPosition().y /32767.0;
        
        std::cout << "\n in readLineSegments: start = (" << start.position.x << "," << start.position.y
        << "), End = (" << end.position.x << "," << end.position.y << ")\n";
        linesRead.insert(linesRead.end(), {start, end});
    }
    return linesRead;
}

void Analyze::myQuickSort(std::vector<Point>& points){
    std::sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
                        if (a.getPosition().y == b.getPosition().y) {
                            return a.getPosition().x < b.getPosition().x;
                        }
                        return a.getPosition().y < b.getPosition().y;
                    });
}

void Analyze::readPoints(std::istream& is) {
    int n_points{0};
    is >> n_points;  // read number of particles
    float x;
    float y;
    
    points.reserve(n_points);
    for (int i = 0; i < n_points; ++i) {
        points.push_back(Point{glm::vec2{}});
        auto& p = points.back();
        
        is >> x >> y;
        p.setPositionX(x);
        p.setPositionY(y);
        
        //sort by y then x value
        //percolateUp(points);
        
        //just std::sort, faster than my percolateUp
        myQuickSort(points);

    }
}

float Analyze::intercept(const Point& p, float slope) {
    if(slope == std::numeric_limits<float>::infinity()){
        return p.getPosition().x;
    }
    return slope * p.getPosition().x - p.getPosition().y;
}



void Analyze::getLineSegments() {
    std::vector<std::vector<Point>> collectedGroups;

        collectCollinearGroups(collectedGroups);
        sortCollinearPoints(collectedGroups);
        addUniqueLines(collectedGroups);
}

void Analyze::writeLineSegments(const std::string& baseFilename) {
    // Open an ofstream for output file. Adjust the file name as necessary.
    std::string outputPath = "/Users/viktorsvensson/Desktop/lab3-part2/detectionsystem/data/output/segments-" + baseFilename;
        std::ofstream outFile(outputPath);
    if (!outFile) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    
    //just to get the exact same output as the examplem files
    outFile << "Enter name of points file: " << baseFilename << "\n";
    
    for (const auto& line : lines) {
        // Check if the line has at least four points
        if (line.size() >= 4) {
            for (size_t q = 0; q < line.size(); q++) {
                // Write each point in the line segment
                outFile << "(" << line[q].getPosition().x << "," << line[q].getPosition().y;
                if(q == line.size()-1){
                    outFile << ")\n";
                }else
                outFile << ") -> ";
            }
        }
    }
    
    //just to get the exact same output as the examplem files
    outFile << "\nSuccess!!\n";
    
    outFile.close(); // Close the file
}

float Analyze::slope(glm::vec2 p, glm::vec2 q){
    if (q.x == p.x) { // Handle vertical lines
            return std::numeric_limits<float>::infinity(); // Special value for vertical line
        }
        return (q.y - p.y) / (q.x - p.x);
}




void Analyze::collectCollinearGroups(std::vector<std::vector<Point>>& collectedGroups) {
    for (size_t i = 0; i < points.size(); ++i) {
        std::map<float, std::vector<Point>> slopes;

        for (size_t j = 0; j < points.size(); ++j) {
            if (i != j) {
                float currentSlope = slope(points[i].getPosition(), points[j].getPosition());
                slopes[currentSlope].push_back(points[j]);
            }
        }

        for (auto& [slopeValue, collinearPoints] : slopes) {
            if (collinearPoints.size() >= 3) {
                collinearPoints.push_back(points[i]);
                collectedGroups.push_back(collinearPoints);
            }
        }
    }
}

void Analyze::sortCollinearPoints(std::vector<std::vector<Point>>& groups) {
    for (auto& group : groups) {
        std::sort(group.begin(), group.end(), [](const Point& a, const Point& b) {
            return a.getPosition().y < b.getPosition().y || (a.getPosition().y == b.getPosition().y && a.getPosition().x < b.getPosition().x);
        });
    }
}

void Analyze::addUniqueLines(const std::vector<std::vector<Point>>& groups) {
    uniqueLines.clear();
    lines.clear();

    for (const auto& group : groups) {
        float slopeValue = slope(group[0].getPosition(), group[1].getPosition());
        float lineIntercept = intercept(group[0], slopeValue);

        line lineProperties;
        lineProperties.k = slopeValue;
        lineProperties.m = lineIntercept;

        bool addLine = true;
        for (auto& thisLine : uniqueLines) {
            if (std::abs(lineProperties.k - thisLine.k) < 0.01 && std::abs(lineProperties.m - thisLine.m) < 0.01) {
                addLine = false;
                break;
            }
            if (lineProperties.k == std::numeric_limits<float>::infinity() &&
                std::abs(lineProperties.m - thisLine.m) < 0.1) {
                addLine = false;
                break;
            }
        }

        if (addLine) {
            lineProperties.start = group[0];
            lineProperties.end = group[group.size() - 1];
            uniqueLines.push_back(lineProperties);
            lines.push_back(group);
        }
    }
}

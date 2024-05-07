#include <linesdiscoverysystem/readfiles.h>

#include <vector>
#include <string>
#include <filesystem>

#include <rendering/window.h>
#include <fmt/format.h>
#include "/Users/viktorsvensson/Desktop/lab3-part2/detectionsystem/include/linesdiscoverysystem/analyze.hpp"


void plotData(const std::string& name, std::vector<rendering::Point>& l);

/* ************************************* */

int main() try {
    std::cout << "Enter the name of input points file: ";
    std::string s;
    std::cin >> s;  // e.g. points1.txt, points200.txt, largeMystery.txt

    Analyze analyzer;
    
    std::ifstream file("/Users/viktorsvensson/Desktop/lab3-part2/detectionsystem/data/" + s);
            if (!file) {
                std::cerr << "Failed to open input file." << std::endl;
                return 1;
            }
    
    analyzer.readPoints(file);
    
    analyzer.getLineSegments();
    
    analyzer.writeLineSegments(s);
    
    std::vector<rendering::Point> linesForRead = analyzer.readLineSegments();
    
    plotData(s, linesForRead);
    
} catch (const std::exception& e) {
    fmt::print("Error: {}", e.what());
    return 1;
}

/* ************************************* */

void plotData(const std::string& name, std::vector<rendering::Point>& l) {
    std::filesystem::path points_name = name;
    const auto points = readPoints(data_dir / points_name);

    std::filesystem::path segments_name = "segments-" + name;
    const std::vector<rendering::Point> lines = l;
    

    rendering::Window window(850, 850, rendering::Window::UseVSync::Yes);
    while (!window.shouldClose()) {
        window.beginFrame();
        window.clear({0, 0, 0, 1});
        window.drawLines(lines);    // to plot the line segments discovered
        window.drawPoints(points);  // to plot input points
        window.endFrame();
    }
}

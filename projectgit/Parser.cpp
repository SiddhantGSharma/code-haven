#include "Parser.h"

Parser::Parser(const std::string & filePath, const std::string & routePath) : graph(true, true) {

    std::fstream file = std::fstream(filePath);
    std::string line; 
    int i = 0;
    while (std::getline(file, line)) {
        std::vector<std::string> data = splitLine(line, ',');
        if (data[4] == "\\N")
            continue;
        double a, b;
        std::string code = "";
        std::string location = "";
        int index = 0;
        try {
            a = std::stold(data[6]);
            b = std::stold(data[7]);
            code = data[4].substr(1, 3);
            location = data[1].substr(1, data[1].length() - 2);
            index = std::stoi(data[0]);
        } catch (...) {
            if (data[5] == "\\N")
                continue;
            // std::cout <<  ", Shifting parsing\n";
            a = std::stold(data[7]);
            b = std::stold(data[8]);
            code = data[5].substr(1, 3);
            location = data[1] +  "," + data[2];
            location = location.substr(1, location.length() - 2);
            index = std::stoi(data[0]);
            // std::cout << code << ", " << location << ":" << a << " , " << b << "\n";
        } 
        Vertex node = {location, code, code, a, b, index};
        nodes.push_back(node);
        nodeMap[code] = node;
        graph.insertVertex(node);
    }
    file.close();

    std::fstream routesFile = std::fstream(routePath);

    // std::string line; 
    while (std::getline(routesFile, line)) {
        std::vector<std::string> data = splitLine(line, ',');
        // std::cout << data[2] << "->" << data[4] << "\n";
        if (data[2] == "\\N" || data[4] == "\\N") {
            continue;
        }
        try {
            Vertex & source = nodeMap.at(data[2]);
            Vertex & dest = nodeMap.at(data[4]);
            graph.insertEdge(source, dest);
            double distance = computeDistance(source, dest);
            graph.setEdgeWeight(source, dest, distance);
        } catch (...) {
            // std::cout << "Unmapped key: " << data[2] << "->" << data[4] << "\n";
            continue;
        }
    }

    routesFile.close();
}

std::vector<Vertex> Parser::getNodes() const {
    return nodes;
}

std::unordered_map<std::string, Vertex> Parser::getNodeMap() const {
    return nodeMap;
}

std::vector<std::string> Parser::splitLine(std::string input, char delimiter) {
    std::vector<std::string> data;
    size_t pos = 0;
    std::string token;

    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        data.push_back(token);
        input.erase(0, pos + 1);
    }
    data.push_back(input);
    return data;
}

double Parser::convertToRadian(double angle) {
    return angle * (std::atan(1.0)*4) / 180;
}

double Parser::computeDistance(const Vertex & source, const Vertex & destination) {
    double lat1 = convertToRadian(source.latitude); 
    double long1 = convertToRadian(source.longitude); 
    double lat2 = convertToRadian(destination.latitude); 
    double long2 = convertToRadian(destination.longitude); 
      
    // Haversine Formula 
    double dlong = long2 - long1; 
    double dlat = lat2 - lat1; 
  
    double ans = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2), 2); 
  
    ans = 2 * asin(sqrt(ans)); 
  
    // Radius of Earth in  
    // Kilometers, R = 6371 
    // Use R = 3956 for miles 
    double R = 6371; 
      
    // Calculate the result 
    ans = ans * R; 
    return ans;
}

Graph Parser::getGraph() const {
    return graph;
}
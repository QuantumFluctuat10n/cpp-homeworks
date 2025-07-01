#include <string>
#include <iostream>
#include <vector>
#include "FileManager"
#include "Sensor"

void writeToFile(const std::string& fileName, const std::string& text){
    FileManager file(fileName);
    file.write(text);
}

void useSensor(){
    auto data = std::make_shared<std::vector<int>>();
    Sensor sensor1(data), sensor2(data);

    sensor1.write(1);
    sensor2.write(8);
    sensor1.write(3);

    std::cout << "Sensor 1: \n";
    sensor1.print();
    std::cout << "Sensor 2: \n";
    sensor2.print();
}

int main(){
    std::cout << "Use of FileManager\n";
    writeToFile("file.txt", "Something to write");
    std::cout << "It`s wrote something, i guess\n";

    std::cout << "\nUse of sensors\n";
    useSensor();
}
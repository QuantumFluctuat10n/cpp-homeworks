#include<iostream>
#include<vector>
#include<map>

template<typename T>
void printRange(T& container){
    for(const auto& element : container)
        std::cout << element << ' ';
    std::cout << std::endl;
}

template<typename K, typename V>
void printRange(std::map<K, V>& map){
    for(const auto& element : map)
        std::cout << '(' << element.first << ' ' << element.second << ") ";
    std::cout << std::endl;
}

template<typename T>
void printIter(T& container){
    for(auto element = container.begin(); element != container.end(); ++element)
        std::cout << *element << ' ';
    std::cout << std::endl;
}

template<typename K, typename V>
void printIter(std::map<K, V>& map){
    for(auto element = map.begin(); element != map.end(); ++element)
        std::cout << '(' <<  element->first << ' ' << element->second << ") ";
    std::cout << std::endl;
}

int main(){
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::map<int, int> m = {{1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}};

    std::cout << "Range-based loop:\n";
    printRange(v);
    printRange(m);

    std::cout << "Iterators:\n";
    printIter(v);
    printIter(m);
}
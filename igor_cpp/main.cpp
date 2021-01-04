#include <iostream>
#include <array>
#include <vector>

int main() {
    /*std::array<float,3> arr = {1.0f,2.0f,3.0f};
    std::cout << arr[0] << std::endl;

    std::vector<float> vec ;

    vec.push_back(10.0f);
    vec.push_back(20.0f);
    vec.push_back(30.0f);

    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << std::endl;
    }

    auto some_float = 12.0f;

    some_float = '\n';

    std::cout << some_float << std::endl;*/

    float original_value = 14;
    float &ref = original_value;

    //ref = 1414;

    std::cout << ref;

    return 0;
}

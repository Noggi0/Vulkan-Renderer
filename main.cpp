//
// Created by vince on 01/08/2021.
//

#include "includes.hpp"
#include "DrawTriangle.hpp"


int main() {
    DrawTriangle drawer;

    try {
        drawer.run();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return 84;
    }
    return 0;
}
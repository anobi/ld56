#include "renderable.hpp"


struct Primitive {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};


Primitive plane {
    std::vector<Vertex> {
        Vertex { glm::fvec3( 0.5f,  0.5f,  0.0f), glm::fvec3(0.4f) },
        Vertex { glm::fvec3( 0.5f, -0.5f,  0.0f), glm::fvec3(0.4f) },
        Vertex { glm::fvec3(-0.5f, -0.5f,  0.0f), glm::fvec3(0.4f) },
        Vertex { glm::fvec3(-0.5f,  0.5f,  0.0f), glm::fvec3(0.4f) }
    },
    std::vector<unsigned int> {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    }
};

Primitive plane_col(glm::fvec3 color) {
    return {
            std::vector<Vertex> {
            Vertex { glm::fvec3( 0.5f,  0.5f,  0.0f), color },
            Vertex { glm::fvec3( 0.5f, -0.5f,  0.0f), color },
            Vertex { glm::fvec3(-0.5f, -0.5f,  0.0f), color },
            Vertex { glm::fvec3(-0.5f,  0.5f,  0.0f), color }
        },
        std::vector<unsigned int> {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        }
    };
}
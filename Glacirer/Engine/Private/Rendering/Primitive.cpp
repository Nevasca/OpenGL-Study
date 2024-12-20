#include "Rendering/Primitive.h"

#include <memory>

#include "Rendering/Mesh.h"
#include "Rendering/VertexBufferLayout.h"

namespace Glacirer
{
 namespace Rendering
 {
  std::shared_ptr<Mesh> Primitive::CreateCube()
  {
   float vertices[]
   {
    // Position                 Normal           UV
    // Front face
    -0.5f, -0.5f,  0.5f,    0.f, 0.f, 1.f,    0.f, 0.f,
    0.5f, -0.5f,  0.5f,    0.f, 0.f, 1.f,    1.f, 0.f,
    0.5f,  0.5f,  0.5f,    0.f, 0.f, 1.f,    1.f, 1.f,
    -0.5f,  0.5f,  0.5f,    0.f, 0.f, 1.f,    0.f, 1.f,

    // Back face
    0.5f, -0.5f, -0.5f,    0.f, 0.f, -1.f,   0.f, 0.f,
    -0.5f, -0.5f, -0.5f,    0.f, 0.f, -1.f,   1.f, 0.f,
    -0.5f,  0.5f, -0.5f,    0.f, 0.f, -1.f,   1.f, 1.f,
    0.5f,  0.5f, -0.5f,    0.f, 0.f, -1.f,   0.f, 1.f, 

    // Right face
    0.5f, -0.5f,  0.5f,    1.f, 0.f, 0.f,    0.f, 0.f,
    0.5f, -0.5f, -0.5f,    1.f, 0.f, 0.f,    1.f, 0.f,
    0.5f,  0.5f, -0.5f,    1.f, 0.f, 0.f,    1.f, 1.f,
    0.5f,  0.5f,  0.5f,    1.f, 0.f, 0.f,    0.f, 1.f,

    // Left face
    -0.5f, -0.5f, -0.5f,   -1.f, 0.f, 0.f,    0.f, 0.f,
    -0.5f, -0.5f,  0.5f,   -1.f, 0.f, 0.f,    1.f, 0.f,
    -0.5f,  0.5f,  0.5f,   -1.f, 0.f, 0.f,    1.f, 1.f,
    -0.5f,  0.5f, -0.5f,   -1.f, 0.f, 0.f,    0.f, 1.f,

    // Top face
    -0.5f,  0.5f,  0.5f,    0.f, 1.f, 0.f,    0.f, 0.f,
    0.5f,  0.5f,  0.5f,    0.f, 1.f, 0.f,    1.f, 0.f,
    0.5f,  0.5f, -0.5f,    0.f, 1.f, 0.f,    1.f, 1.f,
    -0.5f,  0.5f, -0.5f,    0.f, 1.f, 0.f,    0.f, 1.f,

    // Bottom face
    -0.5f,  -0.5f, -0.5f,   0.f, -1.f, 0.f,   0.f, 0.f,
    0.5f,  -0.5f, -0.5f,   0.f, -1.f, 0.f,   1.f, 0.f,
    0.5f,  -0.5f,  0.5f,   0.f, -1.f, 0.f,   1.f, 1.f,
    -0.5f,  -0.5f,  0.5f,   0.f, -1.f, 0.f,   0.f, 1.f
   };

   std::vector<unsigned int> indices
   {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20
   };

   VertexBufferLayout layout{};
   layout.PushFloat(3);
   layout.PushFloat(3);
   layout.PushFloat(2);

   std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(vertices, sizeof(vertices), layout, indices);
   cube->SetName("Cube");

   return cube;
  }

  std::shared_ptr<Mesh> Primitive::CreateQuad()
  {
   float vertices[]
   {
    -0.5f, -0.5f,  0.0f,    0.f, 0.f, 1.f,    0.f, 0.f,
    0.5f, -0.5f,  0.0f,    0.f, 0.f, 1.f,    1.f, 0.f,
    0.5f,  0.5f,  0.0f,    0.f, 0.f, 1.f,    1.f, 1.f,
    -0.5f,  0.5f,  0.0f,    0.f, 0.f, 1.f,    0.f, 1.f,
   };

   std::vector<unsigned int> indices
   {
    0, 1, 2,
    2, 3, 0
   };

   VertexBufferLayout layout{};
   layout.PushFloat(3);
   layout.PushFloat(3);
   layout.PushFloat(2);

   std::shared_ptr<Mesh> quad = std::make_shared<Mesh>(vertices, sizeof(vertices), layout, indices);
   quad->SetName("Quad");

   return quad;
  }

  std::shared_ptr<Mesh> Primitive::CreateScreenQuad()
  {
   float vertices[]
   {
    // Normalized Device position      UV
    -1.f, -1.f,  0.0f,   0.f, 0.f,
    1.f, -1.f,  0.0f,   1.f, 0.f,
    1.f,  1.f,  0.0f,   1.f, 1.f,
    -1.f,  1.f,  0.0f,   0.f, 1.f,
   };

   std::vector<unsigned int> indices
   {
    0, 1, 2,
    2, 3, 0
   };

   VertexBufferLayout layout{};
   layout.PushFloat(3);
   layout.PushFloat(2);

   std::shared_ptr<Mesh> screenQuad = std::make_shared<Mesh>(vertices, sizeof(vertices), layout, indices);
   screenQuad->SetName("ScreenQuad");

   return screenQuad;
  }

  std::shared_ptr<Mesh> Primitive::CreateSkyCube()
  {
   // For skybox usage, we don't need UV (we use the position itself as direction for sampling)
   // neither normal information
   float vertices[]
   {
    // Position
    // Front face
    -1.f, -1.f,  1.f,
    1.f, -1.f,  1.f,
    1.f,  1.f,  1.f,
    -1.f,  1.f,  1.f,

    // Back face
    1.f, -1.f, -1.f,
    -1.f, -1.f, -1.f,
    -1.f,  1.f, -1.f,
    1.f,  1.f, -1.f,

    // Right face
    1.f, -1.f,  1.f,
    1.f, -1.f, -1.f,
    1.f,  1.f, -1.f,
    1.f,  1.f,  1.f,

    // Left face
    -1.f, -1.f, -1.f,
    -1.f, -1.f,  1.f,
    -1.f,  1.f,  1.f,
    -1.f,  1.f, -1.f,

    // Top face
    -1.f,  1.f,  1.f,
    1.f,  1.f,  1.f,
    1.f,  1.f, -1.f,
    -1.f,  1.f, -1.f,

    // Bottom face
    -1.f,  -1.f, -1.f,
    1.f,  -1.f, -1.f,
    1.f,  -1.f,  1.f,
    -1.f,  -1.f,  1.f,
   };

   std::vector<unsigned int> indices
   {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20
   };

   VertexBufferLayout layout{};
   layout.PushFloat(3);

   std::shared_ptr<Mesh> skyCube = std::make_shared<Mesh>(vertices, sizeof(vertices), layout, indices);
   skyCube->SetName("SkyCube");

   return skyCube;
  }
 }
}

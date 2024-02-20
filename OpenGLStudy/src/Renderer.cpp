#include "Renderer.h"

#include <iostream>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);

    // GL_NO_ERROR is 0, we could write like this:
    // while(!glGetError());
}

bool GLLogCall(const char* Function, const char* File, int Line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << Function << " " << File << ":" << Line << "\n";
        return false;
    }

    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

// In a more appropriate way, instead of having a shader here to set the uniform and pass a shader to the draw method
// we would have a material. A material is basically a shader + a set of data (uniforms)
// When we bind the material, the material would be responsible for setting all the uniforms it has
void Renderer::Draw(const VertexArray& Va, const IndexBuffer& Ib, const Shader& Shader) const
{
    Shader.Bind();
    Va.Bind();
    
    // We actually don't need to bind IndexBuffer, VertexArray stores the index buffer bind call
    // but it seems it only works if when we created the object, we bound the index after adding the vertex buffer and layout to the vertex array
    // if the index buffer was created (our constructor bind it as well) before the add buffer call, it crashes if we don't bind it here
    // Ib.Bind(); 

    // Approach for a draw call without an index buffer (using the current bound buffer (glBindBuffer)
    // Mode: GL_TRIANGLES
    // First element to start, in case we want to offset the data in selected buffer
    // How many vertex there are
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    // If we wanted to render as wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Approach for a draw call with an index buffer
    // Mode: Triangles
    // How many indices there are
    // Type of the index on the index array
    // Pointer to the indices. Since we have used glBindBuffer(GL_ELEMENT_ARRAY_BUFFER... we don't need to specify one
    GLCall(glDrawElements(GL_TRIANGLES, Ib.GetCount(), GL_UNSIGNED_INT, nullptr));

    // Unbinding on OpenGL could help for debugging and might avoid some bugs
    // but it's generally a waste of performance, since we are going to bind to the next object before making the draw call anyway
}

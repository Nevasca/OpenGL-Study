#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& FilePath)
    : m_FilePath(FilePath), m_RendererID(0)
{
    ShaderProgramSource Source = ParseShader(FilePath);
    m_RendererID = CreateShader(Source.VertexSource, Source.FragmentSource);
}

Shader::~Shader()
{
    // Clean shader after usage so we don't leak on VRAM
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& Name, int Value)
{
    GLCall(glUniform1i(GetUniformLocation(Name), Value));
}

void Shader::SetUniform1f(const std::string& Name, float Value)
{
    GLCall(glUniform1f(GetUniformLocation(Name), Value));
}

// In a more robust solution for a game engine, we would probably have a SetValue method with overloads for different value types
// to call the proper uniform function 
void Shader::SetUniform4f(const std::string& Name, float V0, float V1, float V2, float V3)
{
    //glUniform4f is to set uniform that receives 4 floats. There are other Uniform functions for other params
    
    // Location: id of the uniform. When shader is created, every uniform is assigned to an id
    GLCall(glUniform4f(GetUniformLocation(Name), V0, V1, V2, V3));
}

void Shader::SetUniformMat4f(const std::string& Name, const glm::mat4& mat)
{
    // location,
    // how many matrices we are providing, just 1
    // if we need to transpose, since we are using glm, it already creates in memory the matrix the way OpenGL expects it
    // Address of the first element
    GLCall(glUniformMatrix4fv(GetUniformLocation(Name), 1, GL_FALSE, &mat[0][0]));
}

int Shader::GetUniformLocation(const std::string& Name)
{
    // Instead of calling glGetUniformLocation every single time we need to set a uniform,
    // we can increase performance by checking our location cache first
    if(m_UniformLocationCache.find(Name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[Name];
    }
    
    GLCall(int Location = glGetUniformLocation(m_RendererID, Name.c_str())); // uniform name needs a C string

    // It might return -1 if the uniform is declared on source code but not used anywhere on the shader and the compiler has removed it
    // Or if it didn't exist at all
    if(Location == -1)
    {
        std::cout << "Warning: Uniform " << Name << " doesn't exist!";
    }

    m_UniformLocationCache[Name] = Location;

    return Location;
}

ShaderProgramSource Shader::ParseShader(const std::string& FilePath)
{
    // This is the modern C++ approach of opening a file
    // In a serious game engine we should consider using the C approach, as it's faster than this
    std::ifstream Stream(FilePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string Line;
    std::stringstream Ss[2]; // One for the vertex shader [0] and other for the fragment [1]
    ShaderType Type = ShaderType::NONE;

    while(getline(Stream, Line))
    {
        // #shader is a custom token we are using to separate the shaders
        if(Line.find("#shader") != std::string::npos)
        {
            if(Line.find("vertex") != std::string::npos)
            {
                Type = ShaderType::VERTEX;
            }
            else if(Line.find("fragment") != std::string::npos)
            {
                Type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            Ss[(int)Type] << Line << "\n";
        }
    }

    return {Ss[0].str(), Ss[1].str()};
}

// Takes source code of each shader and compile into shaders
unsigned int Shader::CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
    // It's recommended to using the C++ type, like unsigned int, instead of the OpenGL defined
    // so when dealing with multiple APIs you don't need to include OpenGL on the header or so
    GLCall(unsigned int Program = glCreateProgram());
    unsigned int Vs = CompileShader(GL_VERTEX_SHADER, VertexShader); 
    unsigned int Fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader); 

    // Link both shaders into the program 
    GLCall(glAttachShader(Program, Vs));
    GLCall(glAttachShader(Program, Fs));
    GLCall(glLinkProgram(Program));
    GLCall(glValidateProgram(Program));

    // Delete the intermediate shaders as they are now compiled into program
    GLCall(glDeleteShader(Vs));
    GLCall(glDeleteShader(Fs));

    return Program;
}

unsigned int Shader::CompileShader(unsigned int Type, const std::string& Source)
{
    GLCall(unsigned int Id = glCreateShader(Type));
    const char* Src = Source.c_str();

    // Shader Id
    // Number of source codes
    // Pointer to the C string source code
    // The length of source code in case we are providing multiples, nullptr if just one
    GLCall(glShaderSource(Id, 1, &Src, nullptr));
    GLCall(glCompileShader(Id));

    int result;
    GLCall(glGetShaderiv(Id, GL_COMPILE_STATUS, &result));

    if(result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(Id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char)); // To hack doing this to allocate on stack: char message[length];
        GLCall(glGetShaderInfoLog(Id, length, &length, message));

        std::cout << "Failed to compile " << (Type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << "\n";

        // Delete shader as the compilation didn't work out
        GLCall(glDeleteShader(Id));

        return 0;
    }
    
    return Id;
}

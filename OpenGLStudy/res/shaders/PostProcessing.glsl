#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_UV;    

out vec2 v_UV;

void main()
{
    gl_Position = vec4(a_Position, 1.f);
    v_UV = a_UV;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

struct PostProcessing
{
    bool ColorInversionEnabled;
    bool GrayScaleEnabled;
    bool SharpenEnabled;
    bool BlurEnabled;
    bool EdgeDetectionEnabled;
};

uniform sampler2D u_ScreenTexture;
uniform PostProcessing u_PostProcessing;

in vec2 v_UV;

const float sampleOffset = 1.f / 300.f;

// We are using a kernel of 3x3, so we need 9 uv offsets to sample texture from
const vec2 offsets[9] = vec2[](
    vec2(-sampleOffset, sampleOffset), // top-left
    vec2(0.f, sampleOffset), // top-center
    vec2(sampleOffset, sampleOffset), // top-right
    vec2(-sampleOffset, 0.f),  // center-left
    vec2(0.f, 0.f), // center-center
    vec2(sampleOffset, 0.f), // center-right
    vec2(-sampleOffset, -sampleOffset), // bottom-left
    vec2(0.f, -sampleOffset), // bottom-center
    vec2(sampleOffset, -sampleOffset) // bottom-right
);

vec3 ApplyKernel(float[9] kernel);

void main()
{          
    // o_Color = texture(u_ScreenTexture, v_UV); // No post-processing

    vec4 colorResult = texture(u_ScreenTexture, v_UV);
    
    if(u_PostProcessing.SharpenEnabled)
    {
        // With kernels (or convolution matrix) we can do some cool effects by multiplying each surrounding pixel
        // by the corresponding kernel position value and then adding them all up as the final color
        // there are a lot of kernel examples online
        // the center value (9) is used for the current pixel and the -1 for the 8 surrounding pixels around it
        // If the kernel values sum up is not 1, texture color ends up brighter or darker than original texture value
        float kernel[9] = float[](
            -1.f, -1.f, -1.f,
            -1.f,  9.f, -1.f,
            -1.f, -1.f, -1.f
        );
        
        colorResult = vec4(ApplyKernel(kernel), 1.f);
    }
    
    if(u_PostProcessing.BlurEnabled)
    {
        float kernel[9] = float[](
            1.f / 16.f, 2.f / 16.f, 1.f / 16.f,
            2.f / 16.f, 4.f / 16.f, 2.f / 16.f,
            1.f / 16.f, 2.f / 16.f, 1.f / 16.f
        );
        
        colorResult = vec4(ApplyKernel(kernel), 1.f);
    }
    
    if(u_PostProcessing.EdgeDetectionEnabled)
    {
        float kernel[9] = float[](
            1.f,  1.f,  1.f,
            1.f, -8.f,  1.f,
            1.f,  1.f,  1.f
        );
        
        colorResult = vec4(ApplyKernel(kernel), 1.f);
    }
    
    if(u_PostProcessing.ColorInversionEnabled)
    {
        colorResult = vec4(vec3(1.f - colorResult.xyz), 1.f);
    }
    
    if(u_PostProcessing.GrayScaleEnabled)
    {
        // Without weight
        // float average = (colorResult.r + colorResult.g + colorResult.b) / 3.f;
        
        // Since human eye tends to be more sensitive to green colors and least to blue, we weight them to be more realistic      
        float average = 0.2126f * colorResult.r + 0.7152f * colorResult.g + 0.0722f * colorResult.b;
        colorResult = vec4(average, average, average, 1.f);
    }        
        
    o_Color = colorResult;
}

vec3 ApplyKernel(float[9] kernel)
{
    // Get the surrounding texture values
    vec3 sampleTex[9];        
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_ScreenTexture, v_UV.xy + offsets[i]));
    }
    
    // // Now we multiply each sample value by the corresponding kernel value and add them all        
    vec3 color = vec3(0.f);
    for(int i = 0; i < 9; i++)
    {
        color += sampleTex[i] * kernel[i];
    }
    
    return color;
}
#version 300 es
layout(location = 0) in vec3 vPosition;   
layout(location = 1) in vec2 vTexCoord; 
out vec2 texCoord; 
uniform int isLeft;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main(void)  
{  

    mat4 transL = mat4(1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.1, 0.0, 0.0, 1.0);
   mat4 transR = mat4(1.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        -0.1, 0.0, 0.0, 1.0);
    if(isLeft == 0){                    
        gl_Position = projection * view * transL * model * vec4(vPosition, 1.0f);   
    }else{
        gl_Position = projection * view * transR * model * vec4(vPosition, 1.0f);   
    }
    texCoord = vTexCoord;  
}  
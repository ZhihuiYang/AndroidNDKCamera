#include "YUVRender.h"
#include "include/logger.h"
#include <string.h>
#include "include/AVFrame.h"
#ifdef DEBUG_MEM
#include "include/mymalloc.h"
#else
#include <malloc.h>
#endif


static int uninitialize(pYUVRender pRender)
{
    glDeleteShader(pRender->vShader);
    glDeleteShader(pRender->fShader);
    glDeleteProgram(pRender->programObject);

    eglMakeCurrent(pRender->mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
                   EGL_NO_CONTEXT);
    eglDestroyContext(pRender->mDisplay, pRender->mContext);
    eglDestroySurface(pRender->mDisplay, pRender->mSurface);
    eglTerminate(pRender->mDisplay);

    pRender->mDisplay = EGL_NO_DISPLAY;
    pRender->mSurface = EGL_NO_SURFACE;
    pRender->mContext = EGL_NO_CONTEXT;
    return 0;
}

static GLuint loadShader(GLenum type, const char * shaderSrc)
{
    GLuint shader;
    GLint complied;
    
    shader = glCreateShader(type);
    if (shader == 0)
    {
        return 0;
    }
    
    glShaderSource(shader, 1, &shaderSrc, NULL);
    
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &complied);
    if (!complied)
    {
        GLint infoLen = 0;
        
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char * infoLog = (char *) malloc(sizeof(char *) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            LOG_ERROR("Error compiling shader : \n %s", infoLog);
            free(infoLog);
        }
        
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

/*
 *  return value:
 *      0 means load program failed.
 *      An integer number indicate a program object.
 * */
static GLuint loadProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint programObject;
    GLint linked;
    
    programObject = glCreateProgram();
    if (programObject == 0)
    {
        return 0;
    }
    
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    
    glLinkProgram(programObject);
    
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            char *infoLog = (char *) malloc(sizeof(char *) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            LOG_ERROR("Error linking program: \n %s", infoLog);
            free(infoLog);
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(programObject);
        return 0;
    }
    
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return programObject;
}




static int initialize(pYUVRender pRender)
{
    // texture 旋转-90 度
    GLfloat vVertices[] = {
            1.0f,  1.0f, 0.0f,   0.0f,1.0f,
            1.0f, -1.0f, 0.0f,   1.0f,1.0f,
            -1.0f, -1.0f, 0.0f,  1.0f,0.0f,
            -1.0f,  1.0f, 0.0f,  0.0f,0.0f
    };
    GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    char vShaderStr[] = "#version 300 es\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec2 vTexCoord;\n"
            "out vec2 texCoord;\n"
            "void main()\n"
            "{\n"
            "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
            "texCoord = vTexCoord; \n"
            "}\0";

    char fShaderStr[] = "#version 300 es\n"
            "precision mediump float; \n"
            "in vec2 texCoord;\n"
            "uniform sampler2D y_texture;\n"
            "uniform sampler2D uv_texture;"
            "layout(location = 0) out vec4 color;\n"
            "void main()\n"
            "{\n"
            "mediump vec3 yuv;\n"
            "vec3 rgb;\n"
            "yuv.x = texture(y_texture, texCoord).r;\n"
            "yuv.y = texture(uv_texture, texCoord).a - 0.5;\n"
            "yuv.z = texture(uv_texture, texCoord).r - 0.5;\n"
            "rgb = mat3( 1,       1,         1, \n"
            "           0,       -0.39465,  2.03211, \n"
            "           1.13983, -0.58060,  0) * yuv; \n"
            "color = vec4(rgb, 1.0f);\n"
            "}\n\0";

    pRender->vShader = loadShader(GL_VERTEX_SHADER, vShaderStr);
    pRender->fShader = loadShader(GL_FRAGMENT_SHADER, fShaderStr);
    pRender->programObject = loadProgram(pRender->vShader, pRender->fShader);
    pRender->YsamplerLoc = glGetUniformLocation(pRender->programObject,
                                                "y_texture");
    pRender->UVsamplerLoc = glGetUniformLocation(pRender->programObject,
                                                "uv_texture");
    glDeleteShader(pRender->vShader);
    glDeleteShader(pRender->fShader);


    glGenVertexArrays(1, &pRender->VAO);
    glGenBuffers(1,&pRender->VBO);
    glGenBuffers(1,&pRender->EBO);

    glBindVertexArray(pRender->VAO);

    glBindBuffer(GL_ARRAY_BUFFER,pRender->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pRender->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, 5* sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    GLuint yTexture;
    GLuint uvTexture;

    glGenTextures(1, &yTexture);
    glBindTexture(GL_TEXTURE_2D, yTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D,0);

    glGenTextures(1,&uvTexture);
    glBindTexture(GL_TEXTURE_2D, uvTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D,0);

    pRender->YtextureId = yTexture;
    pRender->UVtextureId  = uvTexture;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return 0;
}

static int drawFrame(pYUVRender pRender, void * data)
{
    AVFrame * frame = (AVFrame *) data;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(pRender->programObject);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pRender->YtextureId);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_LUMINANCE, frame->width, frame->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, frame->data[0]);
    glUniform1i(pRender->YsamplerLoc, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pRender->UVtextureId);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_LUMINANCE_ALPHA, frame->width/2, frame->height/2, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, frame->data[1]);
    glUniform1i(pRender->UVsamplerLoc, 1);

    glBindVertexArray(pRender->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return 0;
}

static int setWindow(pYUVRender pRender, void * window)
{
    if (NULL == pRender)
    {
        LOG_ERROR("invalid parameters, Render: %p", pRender);
        return -1;
    }
    
    if (NULL == window)
    {
        LOG_ERROR("invalid parameters, window: %p", window);
        return -2;
    }
    return 0;
}

static int renderFrameVR(pYUVRender pRender, void * data)
{
    drawFrame(pRender, data);
    return 0;
}

static int renderFrame(pYUVRender pRender, void * data)
{
    if (NULL == pRender)
    {
        LOG_ERROR("invalid parameters, Render: %p, data:%p", pRender,
                data);
        return -1;
    }

    drawFrame(pRender, data);
    
    return 0;
}

YUVRender * createYUVRender()
{
    YUVRender * pRender = NULL;
    pRender = (YUVRender *) malloc(sizeof(YUVRender));
    if (NULL == pRender)
    {
        LOG_ERROR("allocate render failed");
        return NULL;
    }
    
    memset(pRender, 0, sizeof(YUVRender));
    pRender->initialize = initialize;
    pRender->renderFrame = renderFrame;
    pRender->uninitialize = uninitialize;
    pthread_mutex_init(&pRender->mMutex, 0);
    return pRender;
}

void releaseYUVRender(YUVRender **ppRender)
{
    
    if (NULL == ppRender || NULL == *ppRender)
    {
        return;
    }
    YUVRender *pRender = *ppRender;
    pRender->uninitialize(pRender);
    
    //ANativeWindow_release(pRender->mWindow);
    
    pRender->initialize = NULL;
    pRender->renderFrame = NULL;
    pRender->uninitialize = NULL;
    pthread_mutex_destroy(&pRender->mMutex);
    free(pRender);
    pRender = NULL;
}

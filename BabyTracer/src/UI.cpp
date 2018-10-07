#include "stdafx.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


#include "glfw-3.2.1/deps/linmath.h"

#include "Image.h"
#include "BaseClasses.h"

struct MyVertex
{
   glm::vec3 pos;        // Vertex
   glm::vec2 uv;     // uv
};
static int numSamples = 1;
float mCamdistance = 5;
float mLookatAngle = 0;
float mLookatZ = 0;
bool mCameraMoved = false;
MyVertex pvertex[4];


static const char* vertex_shader_text =
"#version 330 compatibility\n"
"uniform mat4 MVP;\n"
"layout (location = 0) in vec3 vPos;\n"
//"layout (location = 1) in vec3 vColor;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec3 color; \n"
"out vec2 texCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vec3(1,1,1);\n"
"    texCoord = aTexCoord;\n"
// "    color = vColor;\n"
"}\n";


static const char* fragment_shader_text =
"#version 330 compatibility\n"
"in vec3 color;\n"
"in vec2 texCoord;\n"
"uniform sampler2D samplerID;\n"

"void main()\n"
"{\n"
"    gl_FragColor = vec4(color*texture(samplerID,texCoord).rgb, 1.0);\n"
"}\n";
void createTexture(Image * image)
{
   if (image->mGlID == 0) {
      glGenTextures(1, &image->mGlID);
   }
   glBindTexture(GL_TEXTURE_2D, image->mGlID);
   // set the texture wrapping/filtering options (on the currently bound texture object)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   // load and generate the texture

   Pixel * pixel_data = image->toPixelData();
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->mWidth, image->mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)&pixel_data[0].rgb[0]);
   glGenerateMipmap(GL_TEXTURE_2D);
 

}

static void error_callback(int error, const char* description)
{
   fprintf(stderr, "Error: %s\n", description);
}


//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
void handeInput(GLFWwindow *window)
{
   ImGuiIO& io = ImGui::GetIO();
   if(io.KeysDown[ImGuiKey_Escape]){
//   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
  // }
   }
   if (io.KeysDown[']']) {
      numSamples +=10;
   }
   if (io.KeysDown['[']) {
      if (numSamples > 1) {
         numSamples -= 10;
      }
      if (numSamples < 0) {
         numSamples = 1;
      }
   }
   if (io.KeysDown['A']  ) {
      mLookatAngle -= .1;
      mCameraMoved = true;
   }
   if (io.KeysDown['D']  ) {
      mLookatAngle += .1;
      mCameraMoved = true;
   }
   if(io.KeysDown['W']  ) {
      mLookatZ += .1;
      mCameraMoved = true;
   }
   if (io.KeysDown['S']) {
      mLookatZ -= .1;
      mCameraMoved = true;
   }
// mouse

   if (!io.WantCaptureMouse && io.MouseDown[0]) {
      mLookatAngle += .001*io.MouseDelta.x;
      mLookatZ += .01*io.MouseDelta.y;
      if (mLookatZ < 0) {
         mLookatZ = 0;
      }
      mCameraMoved = true;
   }
   if (io.MouseWheel) {
      mCamdistance += .1*io.MouseWheel;
       mCameraMoved = true;
   }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
}


void checkShader(unsigned int shader)
{
   GLint isCompiled = 0;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
   if (isCompiled == GL_FALSE)
   {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> errorLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
      std::cout << errorLog.data() << std::endl;
      // Provide the infolog in whatever manor you deem best.
      // Exit with failure.
      glDeleteShader(shader); // Don't leak the shader.
      return;
   }
}

void checkShaderProgram(unsigned int shader_program)
{
   // Note the different functions here: glGetProgram* instead of glGetShader*.
   GLint isLinked = 0;
   glGetProgramiv(shader_program, GL_LINK_STATUS, (int *)&isLinked);
   if (isLinked == GL_FALSE)
   {
      GLint maxLength = 0;
      glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(shader_program, maxLength, &maxLength, &infoLog[0]);
      std::cout << infoLog.data() << std::endl;

      // We don't need the program anymore.
      glDeleteProgram(shader_program);
      // Don't leak shaders either.
      //glDeleteShader(vertexShader);
      //glDeleteShader(fragmentShader);

      // Use the infoLog as you see fit.

   }
   // In this simple program, we'll just leave
   return;
}

void LaunchUI(RayTracer * rt, Image * image )
{

   pvertex[0].pos = glm::vec3(2.0f, 1.0f, 0.0f);
   pvertex[0].uv = glm::vec2(1.0f, 1.0f);
   
   pvertex[1].pos = glm::vec3(2.0f, -1, 0.0f);
   pvertex[1].uv = glm::vec2(1.0f, 0.0f);

   pvertex[2].pos = glm::vec3(-2.0f, -1, 0.0f);
   pvertex[2].uv = glm::vec2(0.0f, 0.0f);

   pvertex[3].pos = glm::vec3(-2.0f, 1, 0.0f);
   pvertex[3].uv = glm::vec2(0.0f, 1.0f);

   GLFWwindow* window;
   GLuint vertex_buffer, IndexVBOID, vertex_shader, fragment_shader, program;
   GLint mvp_location, vpos_location, vcol_location;
   glfwSetErrorCallback(error_callback);
   if (!glfwInit())
      exit(EXIT_FAILURE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   window = glfwCreateWindow(640, 320, "Ray Tracer", NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }

   //glfwSetKeyCallback(window, key_callback);
   glfwSetMouseButtonCallback(window, mouse_button_callback);

   glfwMakeContextCurrent(window);
   gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
   glfwSwapInterval(1);

   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init();

   // NOTE: OpenGL error checks have been omitted for brevity
   glGenBuffers(1, &vertex_buffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex)*4, pvertex, GL_STATIC_DRAW);


   unsigned short pindices[6];
   pindices[0] = 0;
   pindices[1] = 1;
   pindices[2] = 2;
   pindices[3] = 2;
   pindices[4] = 3;
   pindices[5] = 0;

   glGenBuffers(1, &IndexVBOID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 6, pindices, GL_STATIC_DRAW);
   
   ///////////////////////////////////////////////

   vertex_shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
   glCompileShader(vertex_shader);
   checkShader(vertex_shader);

   fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
   glCompileShader(fragment_shader);
   checkShader(fragment_shader);

   program = glCreateProgram();
   glAttachShader(program, vertex_shader);
   glAttachShader(program, fragment_shader);
   glLinkProgram(program);
   checkShaderProgram(program);

   mvp_location = glGetUniformLocation(program, "MVP");
   //vpos_location = glGetAttribLocation(program, "vPos");
   //vcol_location = glGetAttribLocation(program, "vCol");

   float timer = 0;
   createTexture(image);
   mLookatAngle = 0;// rt->mCamera.mLookFrom[0];
   mLookatZ = rt->mCamera.mLookFrom[1];

   while (!glfwWindowShouldClose(window))
   {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

//      ImGui::ShowDemoWindow();
      bool resetRender = false;
      ImGui::Begin("window");
      if (ImGui::Checkbox("Include Light", &rt->mIncludeLight)) {
         resetRender = true;
      }
      if (rt->mIncludeLight)
      {
         if (ImGui::CollapsingHeader("Light Settings")) 
         {
            float lightColor[3];
            lightColor[0] = rt->mLightColor[0];
            lightColor[1] = rt->mLightColor[1];
            lightColor[2] = rt->mLightColor[2];
            if (ImGui::ColorPicker3("Light Color", lightColor)) {
               rt->mLightColor[0] = lightColor[0];
               rt->mLightColor[1] = lightColor[1];
               rt->mLightColor[2] = lightColor[2];
               resetRender = true;
            }
            float ldirection = atan2(rt->mLightDir[2], rt->mLightDir[0]);
            if (ImGui::SliderAngle("Direction", &ldirection, -180, 180)) {
               rt->mLightDir[0] = cos(ldirection);
               rt->mLightDir[2] = sin(ldirection);
               //            rt->mLightDir = normalize(rt->mLightDir);
               resetRender = true;
            }
         }
      }
      if(ImGui::Checkbox("Include SkyLight", &rt->mIncludeSkyLight)) {
         resetRender = true;
      }
      if (rt->mIncludeSkyLight) {
         if (ImGui::CollapsingHeader("Sky Settings")) {
            float skycolor[3];
            skycolor[0] = rt->mSkyColor[0];
            skycolor[1] = rt->mSkyColor[1];
            skycolor[2] = rt->mSkyColor[2];
            if (ImGui::ColorPicker3("Sky Color", skycolor)) {
               rt->mSkyColor[0] = skycolor[0];
               rt->mSkyColor[1] = skycolor[1];
               rt->mSkyColor[2] = skycolor[2];
               resetRender = true;
            }

         }
      }
      if (ImGui::SliderFloat("Aperture", &rt->mCamera.mAperture, 0, 1)) {
         resetRender = true;
      }  
      if (ImGui::SliderFloat("Fov", &rt->mCamera.mFov, 15, 135)) {
         resetRender = true;
      }
      ImGui::End();

      handeInput(window);


      rt->mCamera.mLookFrom[0] = mCamdistance*cos(mLookatAngle);
      rt->mCamera.mLookFrom[2] = mCamdistance*sin(mLookatAngle);
      rt->mCamera.mLookFrom[1] = mCamdistance*mLookatZ;;

      rt->mCamera.updateCamera();
      rt->RayTraceScene(*image, 1, !mCameraMoved && !resetRender);
      mCameraMoved = false;
      createTexture(image);

      timer += .1;
      float ratio;
      int width, height;
      mat4x4 m, p, mvp;
      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / (float)height;
      glViewport(0, 0, width, height);
      glClear(GL_COLOR_BUFFER_BIT);
      mat4x4_identity(m);
      //mat4x4_rotate_Z(m, m, (float)glfwGetTime());
      mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      mat4x4_mul(mvp, p, m);
      glUseProgram(program);
      glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);

      // Define this somewhere in your header file
      {
#define BUFFER_OFFSET(i) ((void*)(i))
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, image->mGlID);
         glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
         glEnableVertexAttribArray(0);
         glEnableVertexAttribArray(1);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0));
         glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(12));

         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOID);

         // To render, we can either use glDrawElements or glDrawRangeElements
         // The is the number of indices. 3 indices needed to make a single triangle
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
      }
     
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   // Cleanup
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();

   glfwDestroyWindow(window);
   glfwTerminate();
}
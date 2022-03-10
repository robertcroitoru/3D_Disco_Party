#include "lab_m1/tema3/tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


tema3::tema3()
{
}


tema3::~tema3()
{
}


void tema3::Init()
{
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        mapTextures["random"] = CreateRandomTexture(16, 16);
    }

    Mesh* circle1 = CreateCone("circle", glm::vec3(0,0,0), 1, glm::vec3(1, 1, 1), false);
    AddMeshToList(circle1);

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("LabShader_dancer");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader_dancer.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader_dancer.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("LabShader_disco");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader_disco.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader_disco.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("LabShader_walls");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader_walls.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader_walls.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("LabShader_roof");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader__roof.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader__roof.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("LabShader_cone");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader_cone.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader_cone.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        lightDirection = glm::vec3(0, 1, 0);
        lightDirection_spot = lightDirection;
        materialShininess = 30;
        materialKd = 0.15;
        materialKs = 3;
        isSpotlight = 0;
    }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            colors[i][j] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            colors_wall_E[i][j] = glm::vec3(0.4, 0.4, 0.4);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            colors_wall_W[i][j] = glm::vec3(0.4, 0.4, 0.4);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            colors_wall_N[i][j] = glm::vec3(0.4, 0.4, 0.4);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            colors_wall_S[i][j] = glm::vec3(0.4, 0.4, 0.4);

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            colors_roof[i][j] = glm::vec3(0.4, 0.4, 0.4);

    for (int i = 0; i < 8; i++)
    {
        floor_light_pos[i] = glm::vec3(-0.1f, 0.2f, i);
        floor_color_lights[i] = colors[0][i];
    }
    for (int i = 0; i < 8; i++)
    {
        floor_light_pos[8 + i] = glm::vec3(i, 0.2f, 7 - 0.1f);
        floor_color_lights[8 + i] = colors[i][7];
    }
    for (int i = 0; i < 8; i++)
    {
        floor_light_pos[16 + i] = glm::vec3(7 - 0.1f, 0.2f, i);
        floor_color_lights[16 + i] = colors[7][i];
    }
    for (int i = 0; i < 8; i++)
    {
        floor_light_pos[24 + i] = glm::vec3(i, 0.2f, -0.1f);
        floor_color_lights[24 + i] = colors[i][0];
    }

    for (int i = 0; i < 4; i++)
    {
        dancer x;
        dancers.push_back(x);
    }

    spot_colors[0] = glm::vec3(1, 0, 0);
    spot_colors[1] = glm::vec3(1, 1, 0);
    spot_colors[2] = glm::vec3(1, 0, 1);
    spot_colors[3] = glm::vec3(0, 0, 1);

    spot_pos[0] = glm::vec3(2, 6, 2);
    spot_pos[1] = glm::vec3(5, 6, 2);
    spot_pos[2] = glm::vec3(5, 6, 5);
    spot_pos[3] = glm::vec3(2, 6, 5);

    dancers[0].translate = glm::vec3(3, 1, 3);
    dancers[1].translate = glm::vec3(5, 1, 2);
    dancers[2].translate = glm::vec3(6, 1, 5);
    dancers[3].translate = glm::vec3(3, 1, 5);

    disco_ball_pos = glm::vec3(3.5, 5.5, 3.5);
    mode = 0;

}


void tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema3::Update(float deltaTimeSeconds)
{
    Time = glfwGetTime();

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 0, j));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["random"],colors[i][j]);
        }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, i, j));
            RenderSimpleMesh(meshes["box"], shaders["LabShader_walls"], modelMatrix, mapTextures["random"], colors_wall_E[i][j], -1, -1, 0);
        }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(8, i, j));
            RenderSimpleMesh(meshes["box"], shaders["LabShader_walls"], modelMatrix, NULL, colors_wall_W[i][j], -1, -1, 16);
        }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, j, -1));
            RenderSimpleMesh(meshes["box"], shaders["LabShader_walls"], modelMatrix, NULL, colors_wall_N[i][j], -1, -1, 24);
        }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, j, 8));
            RenderSimpleMesh(meshes["box"], shaders["LabShader_walls"], modelMatrix, NULL, colors_wall_S[i][j], -1, -1, 8);
        }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 8, j));
            RenderSimpleMesh(meshes["box"], shaders["LabShader_roof"], modelMatrix, NULL, colors_roof[i][j]);
        }

    

     /* {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 1, 0.5));
        row = (int)floor(dancers[0].position.x + 0.25f);
        col = (int)floor(dancers[0].position.z + 0.25f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader_dancer"], modelMatrix, NULL, glm::vec3(0.25, 0.75, 0.75), row, col);

        if (Time < 5)
            translate = dancers[0].translate + glm::vec3(0.5 * sin(0.7*Time) * sin(0.7*1.5 * Time), 0.0f, 1 * cos(0.7*1.5 * Time));
        else if (Time >= 5 && Time < 10)
            translate = dancers[0].translate + glm::vec3(cos(Time - 5)*0.5*sin(0.7 * Time) * sin(0.7 * 1.5 * Time) + sin(Time - 5), 0.0f, cos(Time - 5) * 1 * cos(0.7 * 1.5 * Time));
        else
            translate = dancers[0].translate + glm::vec3(cos(2 * Time - 20) * cos(Time - 5) * 0.5 * sin(0.7 * Time) * sin(0.7 * 1.5 * Time) + sin(Time - 5) + 0.5f* sin(Time - 10), 0.0f, cos(Time - 5) * 1 * cos(0.7 * 1.5f * Time) * cos(1.5 * Time - 15) + 0.5f*sin(Time - 10));

        dancers[0].position = glm::vec3(translate.x + (1 * 0.5) / 2, translate.y, translate.z + (1 * 0.5) / 2);

    }*/

    /* {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 1, 0.5f));
        row = (int)floor(dancers[1].position.x + 0.25f);
        col = (int)floor(dancers[1].position.z + 0.25f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader_dancer"], modelMatrix, NULL, glm::vec3(0.25, 0.75, 0.75), row, col);
        if (Time < 3)
            translate = dancers[1].translate + glm::vec3(2 * cos(Time) * sin(1.5 * Time), 0.0f, 1 * cos(1.5 * Time) + sin(Time));
        else if (Time >= 3 && Time < 8)
            translate = dancers[1].translate + glm::vec3(cos(Time - 3) * 2 * cos(Time) * sin(1.5 * Time) + sin(Time - 3), 0.0f, cos(1.5 * Time - 4.5f) * (1 * cos(1.5 * Time) + sin(Time)));
        else
            translate = dancers[1].translate + glm::vec3(cos(2 * Time - 16) * (cos(Time - 3) * 2 * cos(Time) * sin(1.5 * Time) + sin(Time - 3)) + 0.6*sin(Time - 8), 0.0f, (cos(1.5 * Time - 4.5f) * (1 * cos(1.5 * Time) + sin(Time))) * cos(1.5 * Time - 1.5 * 8) + 2*sin(Time - 8));

        dancers[1].position = glm::vec3(translate.x + (1 * 0.5) / 2, translate.y, translate.z + (1 * 0.5) / 2);
    }*/

    /* {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 1, 0.5f));
        row = (int)floor(dancers[2].position.x + 0.25f);
        col = (int)floor(dancers[2].position.z + 0.25f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader_dancer"], modelMatrix, NULL, glm::vec3(0.25, 0.75, 0.75), row, col);
        if (Time < 6)
            translate = dancers[2].translate + glm::vec3(cos(Time) * sin(2 * Time), 0.0f, 1.5 * sin(1.5 * Time) + cos(Time) * sin(1.5f * Time));
        else if (Time >= 6 && Time < 13)
            translate = dancers[2].translate + glm::vec3(cos(1.5 * Time - 9) * cos(Time) * sin(2 * Time) + sin(Time - 6), 0.0f, cos(1.2 * Time - 1.2 * 6) * (1.5 * sin(1.5 * Time) + cos(Time) * sin(1.5f * Time)) + 1.4 * sin(Time - 6));
        else
            translate = dancers[2].translate + glm::vec3(cos(1.1 * Time - 13 * 1.1) * (cos(1.5 * Time - 9) * cos(Time) * sin(2 * Time) + sin(Time - 6)) + 0.1 * sin(Time - 13), 0.0f, cos(1.5 * Time - 1.5 * 13) * (cos(1.2 * Time - 1.2 * 6) * (1.5 * sin(1.5 * Time) + cos(Time) * sin(1.5f * Time)) + 1.4 * sin(Time - 6)) + 0.85 * sin(2 * Time - 26));

        dancers[2].position = glm::vec3(translate.x + (1 * 0.5) / 2, translate.y, translate.z + (1 * 0.5) / 2);
        cout << Time << endl;
    }*/
    // 

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, translate);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 1, 0.5f));
        row = (int)floor(dancers[3].position.x + 0.25f);
        col = (int)floor(dancers[3].position.z + 0.25f);
        RenderSimpleMesh(meshes["box"], shaders["LabShader_dancer"], modelMatrix, NULL, glm::vec3(0.25, 0.75, 0.75), row, col);
        if (Time < 7)
            translate = dancers[3].translate + glm::vec3(cos(1.5 * Time) * sin(Time), 0.0f, 1 * sin(1.5 * Time) + sin(2 * Time) * sin(1.5f * Time));
        else if (Time >= 7 && Time < 15)
            translate = dancers[3].translate + glm::vec3(cos(1.6 * Time - 1.6 * 7) * cos(1.5 * Time) * sin(Time) + 2.7 * sin(1.3 * Time - 1.3 * 7), 0.0f, cos(1.45 * Time - 1.45 * 7) * (1 * sin(1.5 * Time) + sin(2 * Time) * sin(1.5f * Time)) + 1.5 * sin(1.3 * Time - 1.3 * 7));
        else
            translate = dancers[3].translate + glm::vec3(cos(1.3 * Time - 1.3 * 15) * (cos(1.6 * Time - 1.6 * 7) * cos(1.5 * Time) * sin(Time) + 2.7 * sin(1.3 * Time - 1.3 * 7)) + 0.8* sin(1.2 * Time - 1.2 * 15), 0.0f, cos(1.22 * Time - 1.22 * 15) * (cos(1.45 * Time - 1.45 * 7) * (1 * sin(1.5 * Time) + sin(2 * Time) * sin(1.5f * Time)) + 1.5 * sin(1.3 * Time - 1.3 * 7)) + 1.8 * sin(1.22 * Time - 1.22 * 15));

        dancers[3].position = glm::vec3(translate.x + (1 * 0.5) / 2, translate.y, translate.z + (1 * 0.5) / 2);
        cout << Time << endl;
    }
    // Render ground

    // Render the point light in the scene
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, disco_ball_pos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader_disco"], modelMatrix, mapTextures["random"]);
    }

    {
        angleOZ = 0.2 * sin(2*Time);
        angleOX = 0.2 * cos(2 * Time);
        angle_OZ_aux = 0.4 * sin(2*Time);
        angle_OX_aux = 0.4 * cos(2 * Time);
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, angleOX, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, angleOZ, glm::vec3(0, 0, 1));

        lightDirection_spot = lightDirection;
        lightDirection_spot = glm::vec3(modelMatrix * glm::vec4(lightDirection, 1));
    }

    if (mode%3 == 1)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // aceasta directiva este folosita pentru nu se scrie in depth buffer
        glDepthMask(GL_FALSE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        /////
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, spot_pos[0]);
        modelMatrix = glm::rotate(modelMatrix, angle_OZ_aux, glm::vec3(0, 0, 1));
        modelMatrix = glm::rotate(modelMatrix, angle_OX_aux, glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["circle"], shaders["LabShader_cone"], modelMatrix, NULL, spot_colors[0]);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, spot_pos[1]);
        modelMatrix = glm::rotate(modelMatrix, angle_OZ_aux, glm::vec3(0, 0, 1));
        modelMatrix = glm::rotate(modelMatrix, angle_OX_aux, glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["circle"], shaders["LabShader_cone"], modelMatrix, NULL, spot_colors[1]);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, spot_pos[2]);
        modelMatrix = glm::rotate(modelMatrix, angle_OZ_aux, glm::vec3(0, 0, 1));
        modelMatrix = glm::rotate(modelMatrix, angle_OX_aux, glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["circle"], shaders["LabShader_cone"], modelMatrix, NULL, spot_colors[2]);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, spot_pos[3]);
        modelMatrix = glm::rotate(modelMatrix, angle_OZ_aux, glm::vec3(0, 0, 1));
        modelMatrix = glm::rotate(modelMatrix, angle_OX_aux, glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["circle"], shaders["LabShader_cone"], modelMatrix, NULL, spot_colors[3]);


        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
    }
}

Texture2D* tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    for (int i = 0; i < size; i++)
        data[i] = rand() % 256;
    // TODO(student): Generate random texture data
    glBindTexture(GL_TEXTURE_2D, textureID);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // TODO(student): Generate and bind the new texture ID

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


void tema3::FrameEnd()
{
    DrawCoordinateSystem();
}


void tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, const glm::vec3 &color, int row, int col, int index)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;
    //cout << mode << endl;
    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    //cout << glm::vec3(light_pos[(int)floor(position.x + 0.25f)][(int)floor(position.z + 0.25f)].x, light_pos[(int)floor(position.x + 0.25f)][(int)floor(position.z + 0.25f)].y, light_pos[(int)floor(position.x + 0.25f)][(int)floor(position.z + 0.25f)].z) << endl;
    // Set shader uniforms for light properties
    if (row != -1 && col != -1)
    {
        //cout << row << " " << col << endl;

        light_pos_dancer[0] = glm::vec3(row, 0, col);
        color_pos_dancer[0] = colors[row][col];

        if (row > 1)
        {
            light_pos_dancer[1] = glm::vec3(row-1, 0, col);
            color_pos_dancer[1] = colors[row-1][col];
        }
        else
        {
            light_pos_dancer[1] = glm::vec3(0);
            color_pos_dancer[1] = glm::vec3(0);
        }

        if (row < 7)
        {
            light_pos_dancer[2] = glm::vec3(row + 1, 0, col);
            color_pos_dancer[2] = colors[row + 1][col];
        }
        else
        {
            light_pos_dancer[2] = glm::vec3(0);
            color_pos_dancer[2] = glm::vec3(0);
        }

        if (col > 1)
        {
            light_pos_dancer[3] = glm::vec3(row, 0, col - 1);
            color_pos_dancer[3] = colors[row][col - 1];
        }
        else
        {
            light_pos_dancer[3] = glm::vec3(0);
            color_pos_dancer[3] = glm::vec3(0);
        }

        if (col < 7)
        {
            light_pos_dancer[4] = glm::vec3(row, 0, col + 1);
            color_pos_dancer[4] = colors[row][col + 1];
        }
        else
        {
            light_pos_dancer[4] = glm::vec3(0);
            color_pos_dancer[4] = glm::vec3(0);
        }

        if (row > 1 && col > 1)
        {
            light_pos_dancer[5] = glm::vec3(row - 1, 0, col - 1);
            color_pos_dancer[5] = colors[row - 1][col - 1];
        }
        else
        {
            light_pos_dancer[5] = glm::vec3(0);
            color_pos_dancer[5] = glm::vec3(0);
        }

        if (row > 1 && col < 7)
        {
            light_pos_dancer[6] = glm::vec3(row - 1, 0, col + 1);
            color_pos_dancer[6] = colors[row - 1][col + 1];
        }
        else
        {
            light_pos_dancer[6] = glm::vec3(0);
            color_pos_dancer[6] = glm::vec3(0);
        }

        if (row < 7 && col < 7)
        {
            light_pos_dancer[7] = glm::vec3(row + 1, 0, col + 1);
            color_pos_dancer[7] = colors[row + 1][col + 1];
        }
        else
        {
            light_pos_dancer[7] = glm::vec3(0);
            color_pos_dancer[7] = glm::vec3(0);
        }

        if (row < 7 && col > 1)
        {
            light_pos_dancer[8] = glm::vec3(row + 1, 0, col - 1);
            color_pos_dancer[8] = colors[row + 1][col - 1];
        }
        else
        {
            light_pos_dancer[8] = glm::vec3(0);
            color_pos_dancer[8] = glm::vec3(0);
        }
        
        glUniform3fv(glGetUniformLocation(shader->program, "light_pos"), 9,
            glm::value_ptr(light_pos_dancer[0]));

        glUniform3fv(glGetUniformLocation(shader->program, "color_box"), 9,
            glm::value_ptr(color_pos_dancer[0]));


    }
    else if (index != -1)
    {
        glUniform3fv(glGetUniformLocation(shader->program, "light_position"), 8,
            glm::value_ptr(floor_light_pos[index]));

        glUniform3fv(glGetUniformLocation(shader->program, "color_box"), 8,
            glm::value_ptr(floor_color_lights[index]));

        int light_pos = glGetUniformLocation(shader->program, "disco_light_pos");
        glUniform3f(light_pos, disco_ball_pos.x, disco_ball_pos.y, disco_ball_pos.z);
    }

    int light_pos = glGetUniformLocation(shader->program, "disco_light_pos");
    glUniform3f(light_pos, disco_ball_pos.x, disco_ball_pos.y, disco_ball_pos.z);

    glUniform3fv(glGetUniformLocation(shader->program, "spot_position"), 4,
        glm::value_ptr(spot_pos[0]));

    glUniform3fv(glGetUniformLocation(shader->program, "color_spot"), 4,
        glm::value_ptr(spot_colors[0]));


    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());

        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    }

    int light_direction_spot = glGetUniformLocation(shader->program, "light_direction_spot");
    glUniform3f(light_direction_spot, lightDirection_spot.x, lightDirection_spot.y, lightDirection_spot.z);

    int mode_loc = glGetUniformLocation(shader->program, "mode");
    glUniform1i(mode_loc, mode);

    int light_position = glGetUniformLocation(shader->program, "light_position_roof");
    glUniform3f(light_position, disco_ball_pos.x, disco_ball_pos.y, disco_ball_pos.z);

    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    GLfloat angle = glGetUniformLocation(shader->program, "cuttOffAngle");
    glUniform1f(angle, cutt_off_angle);

    GLint isspot = glGetUniformLocation(shader->program, "is_spot");
    glUniform1i(isspot, isSpotlight);
    // TODO(student): Set any other shader uniforms that you need

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    float Time = Engine::GetElapsedTime();
    GLint loc_Time_matrix = glGetUniformLocation(shader->program, "Time");
    glUniform1f(loc_Time_matrix, Time);

    //cout << Time << endl;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema3::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 5;

    if (true)
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        // Control light position using on W, A, S, D, E, Q
        if (window->KeyHold(GLFW_KEY_W))
        {
            lightPosition -= forward * deltaTime * speed;
            lightPosition1 -= forward * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_A))
        {
            lightPosition -= right * deltaTime * speed;
            lightPosition1 -= right * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_S))
        {
            lightPosition += forward * deltaTime * speed;
            lightPosition1 += forward * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            lightPosition += right * deltaTime * speed;
            lightPosition1 += right * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_E))
        {
            lightPosition += up * deltaTime * speed;
            lightPosition1 += up * deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_Q))
        {
            lightPosition -= up * deltaTime * speed;
            lightPosition1 -= up * deltaTime * speed;
        }

        // TODO(student): Set any other keys that you might need
        if (window->KeyHold(GLFW_KEY_K))
            angleOZ -= deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_I))
            angleOZ += deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_J))
            angleOX -= deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_L))
            angleOX += deltaTime * speed;

        if (window->KeyHold(GLFW_KEY_Y))
        {
            if (cutt_off_angle > 360)
                cutt_off_angle = 360;
            else
                cutt_off_angle += deltaTime * 20;
        }

        if (window->KeyHold(GLFW_KEY_H))
        {
            if (cutt_off_angle < 0)
                cutt_off_angle = 0;
            else
                cutt_off_angle -= deltaTime * 20;
        }

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, angleOX, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, angleOZ, glm::vec3(0, 0, 1));

        lightDirection = glm::vec3(0, -1, 0);
        lightDirection = glm::vec3(modelMatrix * glm::vec4(lightDirection, 1));
    }


    
}


void tema3::OnKeyPress(int key, int mods)
{
    // Add key press event

    // TODO(student): Set keys that you might need
    if (key == GLFW_KEY_F)
        mode++;
}


void tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema3::OnWindowResize(int width, int height)
{
}

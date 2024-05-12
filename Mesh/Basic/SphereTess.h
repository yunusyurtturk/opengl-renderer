#pragma once
#include "../Mesh.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../../CompiledShaderProgram.h"

class SphereTess : public Mesh
{
private:
    float radius = 1.0f;
    unsigned int sectorCount = 36;
    unsigned int stackCount = 18;
    unsigned int vertexCount = (sectorCount + 1) * (stackCount + 1);
    unsigned int rez = 5;

    void initSphereVertices()
    {
        std::vector<Vertex>& vertices = GetVertex();

        int width = 1;
        int height = 1;

        for (unsigned i = 0; i <= rez - 1; i++)
        {
            for (unsigned j = 0; j <= rez - 1; j++)
            {
                vertices.emplace_back(-width / 2.0f + width * i / (float)rez
                    , 0.0f,
                    -height / 2.0f + height * j / (float)rez,
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    i / (float)rez,
                    j / (float)rez);

                vertices.emplace_back(-width / 2.0f + width * (i + 1) / (float)rez
                    , 0.0f,
                    -height / 2.0f + height * j / (float)rez,
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    (i + 1) / (float)rez,
                    j / (float)rez);

                vertices.emplace_back(-width / 2.0f + width * i / (float)rez
                    , 0.0f,
                    -height / 2.0f + height * (j + 1) / (float)rez,
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    i / (float)rez,
                    (j + 1) / (float)rez);

                vertices.emplace_back(-width / 2.0f + width * (i + 1) / (float)rez
                    , 0.0f,
                    -height / 2.0f + height * (j + 1) / (float)rez,
                    0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f,
                    (i + 1) / (float)rez,
                    (j + 1) / (float)rez);
            }
        }
    }

    virtual void Setup() override
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        initVertex();
        initIndex();
        glBindVertexArray(0);
        glPatchParameteri(GL_PATCH_VERTICES, 4);
    }
public:
    SphereTess() : Mesh()
    {
        initSphereVertices();
    }

    virtual void Draw(CompiledShaderProgram& shader) override
    {
        std::vector<Vertex>& Vertices = GetVertex();

        glBindVertexArray(VAO);
       // glDrawArrays(GL_PATCHES, 0, Vertices.size());
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        glDrawArrays(GL_PATCHES, 0, 4 * rez*rez);
        glBindVertexArray(0);
        // glBindVertexArray(VAO);
        // glActiveTexture(GL_TEXTURE0);
        // glDrawArrays(GL_POINTS, 0, vertexCount);
        //Mesh::Draw(shader);
    }
};
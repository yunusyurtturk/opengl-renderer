#pragma once
#include "../Mesh.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../../CompiledShaderProgram.h"

class BezierTess : public Mesh
{
private:
    void initSphereVertices()
    {
        std::vector<Vertex>& vertices = GetVertex();

        vertices.emplace_back(-0.95f, -0.95f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f);

        vertices.emplace_back(-0.85f, 0.95f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f);

        vertices.emplace_back(0.5f, -0.95f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f);

        vertices.emplace_back(0.95, 0.95f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f);
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
    BezierTess() : Mesh()
    {
        initSphereVertices();
    }

    virtual void Draw(CompiledShaderProgram& shader) override
    {
        std::vector<Vertex>& Vertices = GetVertex();

       
        glBindVertexArray(VAO);
        // glDrawArrays(GL_PATCHES, 0, Vertices.size());
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        glDrawArrays(GL_PATCHES, 0, 4);
        glBindVertexArray(0);
        // glBindVertexArray(VAO);
        // glActiveTexture(GL_TEXTURE0);
        // glDrawArrays(GL_POINTS, 0, vertexCount);
        //Mesh::Draw(shader);
    }
};
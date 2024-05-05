#pragma once
#include "../Mesh.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../../CompiledShaderProgram.h"

class Sphere : public Mesh
{
private:
	static const int VERTEX_COUNT = 36;
    float radius = 1.0f;
    unsigned int sectorCount = 36;
    unsigned int stackCount = 18;
	void initSphereVertices()
	{
        std::vector<Vertex>& Vertices = GetVertex();

        unsigned int vertexCount = 0;
        const float PI = acos(-1.0f);


        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
        float s, t;                                     // texCoord

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

                // normalized vertex normal
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;

                // vertex tex coord between [0, 1]
                s = (float)j / sectorCount;
                t = (float)i / stackCount;

                Vertices.emplace_back(x, y, z, 1.0f, 0.5f, 0.5f, nx, ny, nz, s, t);

                vertexCount++;
            }
        }


	}

	void initSphereIndex()
	{
        std::vector<int>& Index = GetIndex();

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding 1st and last stacks
                if (i != 0)
                {
                    Index.insert(Index.end(), { k1, k2, k1 + 1 });
                }

                if (i != (stackCount - 1))
                {
                    Index.insert(Index.end(), { k1 + 1, k2, k2 + 1 });
                }

                // // vertical lines for all stacks
                // lineIndices.push_back(k1);
                // lineIndices.push_back(k2);
                // if (i != 0)  // horizontal lines except 1st stack
                // {
                //     lineIndices.push_back(k1);
                //     lineIndices.push_back(k1 + 1);
                // }
            }
        }
	}
public:
	Sphere() : Mesh()
	{
        initSphereVertices();
        initSphereIndex();
	}

	virtual void Draw(CompiledShaderProgram& shader) override
	{
        Mesh::Draw(shader);
	}
};
#pragma once
#include <vector>
#include "../Vertex.h"



class Model
{
private:
	std::vector<Vertex> vertex;
	std::vector<int> index;
public:
	std::vector<Vertex>& GetVertex()
	{
		return vertex;
	}
	std::vector<int>& GetIndex()
	{
		return index;
	}
	virtual void Draw() = 0;
};
#pragma once
#include <vector>
#include "framework.h"
#include "camera.h"
#include "main/includes.h"
#include "mesh.h"

// DEFINICIÓ DE LA CLASE ENTITY _____________________________________________________________________
class Entity{
private:
    Mesh mesh;
    Matrix44 m_matrix;
    
public:
    Entity(Mesh mesh,Matrix44 m){
        this->mesh = mesh;
        this->m_matrix = m;
    }
};

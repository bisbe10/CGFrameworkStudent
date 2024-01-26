#pragma once
#include <vector>
#include "framework.h"
#include "camera.h"
#include "main/includes.h"

// DEFINICIÓ DE LA CLASE ENTITY _____________________________________________________________________
class Entity{
private:
    Mesh mesh;
    Matrix m_matrix;
    
public:
    Entity(Mesh mesh,Matrix m_matrix){
        Mesh = mesh;
        Matrix = m_matrix;
    }
};

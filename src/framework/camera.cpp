#include "camera.h"
#include "application.h"
#include "main/includes.h"
#include <iostream>

Camera::Camera()
{
    
	view_matrix.SetIdentity();
	SetOrthographic(-1,1,1,-1,-1,1);
}

Vector3 Camera::GetLocalVector(const Vector3& v)
{
	Matrix44 iV = view_matrix;
	if (iV.Inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.RotateVector(v);
	return result;
}

Vector3 Camera::ProjectVector(Vector3 pos, bool& negZ)
{
	Vector4 pos4 = Vector4(pos.x, pos.y, pos.z, 1.0);
    
    // la viewprojection_matrix surt d'una cadena de funcions desde el constructor a UpdateViewProjectionMatrix(){ -> viewprojection_matrix = view_matrix * }
    
	Vector4 result = viewprojection_matrix * pos4;
    
    //-> passem el vector a clip space
    // si z=1  /z ha estat definit anteriorment = negZ = negatiu = ENS VA DE LOCOS !
	negZ = result.z < 0;
    
	if (type == ORTHOGRAPHIC)
		return result.GetVector3();
	else
		return result.GetVector3() / result.w;
}

void Camera::Rotate(float angle, const Vector3& axis)
{
	Matrix44 R;
	R.SetRotation(angle, axis);
	Vector3 new_front = R * (center - eye);
	center = eye + new_front;
	UpdateViewMatrix();
}

void Camera::Move(Vector3 delta)
{
	Vector3 localDelta = GetLocalVector(delta);
	eye = eye - localDelta;
	center = center - localDelta;
	UpdateViewMatrix();
}

// El constructor  de camera entra SetOrthographic(-1,1,1,-1,-1,1);
void Camera::SetOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane)
{
	type = ORTHOGRAPHIC;

	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}
void Camera::orbit(float angle_in_rad, const Vector3& axis){
    Matrix44 R;
    R.SetRotation(angle_in_rad, axis);
    Vector3 new_front = R * (eye-center);
    this->eye = center + new_front;
    
    UpdateViewMatrix();
}
void Camera::SetPerspective(float fov, float aspect, float near_plane, float far_plane)
{
	type = PERSPECTIVE;

	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	this->eye = eye;
	this->center = center;
	this->up = up;

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	// Reset Matrix (Identity)
	view_matrix.SetIdentity();
    
    
    //Rotation matrix
    Matrix44 rotation_matrix;
    rotation_matrix.SetIdentity();
    //PAS 1 -> vector front
    Vector3 F = center - eye; // vector entre el 
    F.Normalize();
    
    //PAS 2 -> vector side
    Vector3 S = F.Cross(up);
    S.Normalize();
    
    //PAS 3 -> vector top
    Vector3 T = S.Cross(F);
    T.Normalize();
    
    // ASIGNAR ELS VECTORS A LA ROTATION MATRIX
    // Com que l'hem inicialitzat en la identitat m[3]/m[7]/m[11]/m[12]/m[13]=0 i m[15]=1
    rotation_matrix.m[0]=S.x; rotation_matrix.m[1]=T.x; rotation_matrix.m[2]=-F.x;
    rotation_matrix.m[4]=S.y; rotation_matrix.m[5]=T.y; rotation_matrix.m[6]=-F.y;
    rotation_matrix.m[8]=S.z; rotation_matrix.m[9]=T.z; rotation_matrix.m[10]=-F.z;
    
    //Translate local matrix
    rotation_matrix.TranslateLocal(-eye.x, -eye.y, -eye.z);
    
    view_matrix=rotation_matrix;

	// Comment this line to create your own projection matrix!
	//SetExampleViewMatrix(); //comentar mes endvant, haurem de fer la nostra


	UpdateViewProjectionMatrix();
}

// Create a projection matrix
void Camera::UpdateProjectionMatrix()
{
	// Reset Matrix (Identity)
	projection_matrix.SetIdentity();
    

    // FER NOSALTRES MÉS ENDAVANT
	
	if (type == PERSPECTIVE) {
        float fov_r = fov*DEG2RAD; //Passem el fov a radians
        float f = 1.0/tan(fov_r/2.0);
         // definit al powerpoint de 3D pàgina 28
        // definim els atributs right, left... al contructor de camera a la funció
        
       projection_matrix.M[0][0] = f/aspect;
        
        projection_matrix.M[1][1] = f;
        
        projection_matrix.M[2][2] = (far_plane + near_plane) / (near_plane - far_plane);
             projection_matrix.M[2][3] = -1.0;
        
    projection_matrix.M[3][2] = ( 2.0 * far_plane * near_plane) / (near_plane - far_plane);
             projection_matrix.M[3][3] = 0.0;
    }
    //definit al powerpoint de 3D pàgina 26 // definim els atributs right, left... al contructor de camera a la funció 
    else if (type == ORTHOGRAPHIC) {
    projection_matrix.M[0][0] = 2.0 / (right - left);
           projection_matrix.M[0][3] = -1.0*((right + left)/(right - left));
       
    projection_matrix.M[1][1] = 2.0/(top - bottom);
           projection_matrix.M[1][3] = -1.0*((top + bottom)/(top - bottom));
        
        projection_matrix.M[2][2] = -2.0/(far_plane - near_plane);
           projection_matrix.M[2][3] = -1.0*((far_plane + near_plane)/(far_plane - near_plane));
    }

    UpdateViewProjectionMatrix();
}

void Camera::UpdateViewProjectionMatrix()
{
	viewprojection_matrix = view_matrix * projection_matrix;
}

Matrix44 Camera::GetViewProjectionMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	return viewprojection_matrix;
}

// The following methods have been created for testing.
// Do not modify them.

void Camera::SetExampleViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix.m );
}

//constructor set el type a Ortho
void Camera::SetExampleProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (type == PERSPECTIVE)
		gluPerspective(fov, aspect, near_plane, far_plane);
	else
		glOrtho(left,right,bottom,top,near_plane,far_plane);

	glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix.m );
	glMatrixMode(GL_MODELVIEW);
}


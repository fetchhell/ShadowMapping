#pragma once
#include <matrix/nv_matrix.h>
#include <vector3f/vec.h>

class Camera {

	friend class Scene;

	public:

	Camera();

	void buildFrustrumMatf(GLfloat, GLfloat, GLfloat, GLfloat);
	void buildViewMatf(vec3f, vec3f, vec3f);
	void buildViewProjection();
	void buildOrthoMatf(float, float);

    private:

	GLfloat m_view[4][4], m_projection[4][4];
	GLfloat m_viewProjection[4][4], m_shadowViewProjection[4][4];

	GLfloat m_znear, m_zfar, m_aspectRatio, m_rightEdge;
	vec3f m_eyePos, m_earthPos, m_viewVec, m_up;

	vec3f m_cameraMove, m_mousePosition;

    float m_speedX, m_speedZ;
    float m_angleX, m_angleY;

	vec3f rotateViewVecXY;

	GLfloat m_cameraRotateXY[4][4], m_cameraTranslate[4][4];
};
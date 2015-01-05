#include "my_camera.h"

#define _USE_MATH_DEFINES 
#include <math.h>

#define SCREEN_WIDTH 2048
#define SCREEN_HEIGHT 1024

//-------------------------------------------------------
Camera::Camera() {

	/* set to identity all matrices */
	NvBuildIdentityMatf(m_view);
	NvBuildIdentityMatf(m_projection);

	NvBuildIdentityMatf(m_viewProjection);

	NvBuildIdentityMatf(m_cameraRotateXY);
	NvBuildIdentityMatf(m_cameraTranslate);

	m_cameraMove   = vec3f(0.0f, 0.0f, 0.0f);
	m_mousePosition = vec3f(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, 0.0f);

	m_speedX = m_speedZ = 0.5f;
	m_angleX = m_angleY = 0.0f;
}


//-------------------------------------------------------
void Camera::buildFrustrumMatf(GLfloat width, GLfloat height, GLfloat znear, GLfloat zfar) {

	m_znear = znear;
	m_zfar  = zfar;

	m_aspectRatio = width / height;

	/* halfHeight */
	m_rightEdge = (GLfloat)(m_znear * tan(45.0 * M_PI / 360.0)); 

    NvBuildFrustumMatf(m_projection,
				-m_rightEdge * m_aspectRatio, m_rightEdge * m_aspectRatio,
  				-m_rightEdge, m_rightEdge, m_znear, m_zfar);
}

//-------------------------------------------------------
void Camera::buildViewMatf(vec3f eye, vec3f earth, vec3f up) {

       m_eyePos   = eye;
	   m_earthPos = earth;
	   m_up       = up;
	   
	   NvBuildLookatMatf(m_view, (GLfloat*) &m_eyePos, (GLfloat*) &m_earthPos, (GLfloat*) &m_up);

	   rotateViewVecXY = vec3f(m_view[0][2], m_view[1][2], m_view[2][2]);
}

//--------------------------------------------------------
void Camera::buildOrthoMatf(float znear, float zfar) {

	   m_znear = znear;
	   m_zfar  = zfar;
		
	   NvBuildOrthoMatf(m_projection, -100.0, 100.0, -100.0, 100.0, m_znear, m_zfar); 
}

//-------------------------------------------------------
void Camera::buildViewProjection() {

	/* building modelView & viewProjection matrices */
	NvMultMatf(m_viewProjection, m_projection, m_view);

	/* building modelViewProjection matrix */
	NvMultMatf(m_viewProjection, m_projection, m_view);
}
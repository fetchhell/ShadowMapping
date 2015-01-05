#include <include/includeAll.h>
#include "scene.h"

const float walk_speed = 5.0;
const float sensitivity = 0.0015;

//----------------------------
char* shader_program [] = { "simple", "shadow", "depthTex" };

//----------------------------
void Scene::initVBO(GLuint buffer, object obj, GLvoid *data, GLuint size) {

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size * obj.m_mesh.getNumFaces() * 3, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

//-----------------------------
void Scene::setupVBO(object *obj) {

	glGenBuffers(3, obj->VBO);
	
	initVBO(*obj->VBO, *obj, obj->m_mesh.getVertex(), sizeof(Lib3dsVector));
	initVBO(*(obj->VBO + 1), *obj, obj->m_mesh.getNormals(), sizeof(Lib3dsVector));
	initVBO(*(obj->VBO + 2), *obj, obj->m_mesh.getTexCoord(), sizeof(Lib3dsTexel));
}

//--------------------------
void Scene::KeyboardEvent() {

	/* handle keyboard event; translate camera */
	Camera *cam = &camera;

	if(GetAsyncKeyState('W')) {
		cam->m_eyePos -= cam->m_viewVec * walk_speed;  
	}

	if(GetAsyncKeyState('S')) {
	    cam->m_eyePos += cam->m_viewVec * walk_speed; 
	}

	if(GetAsyncKeyState('A')){
	   cam->m_eyePos.x -= cam->m_viewVec.z * walk_speed;
	   cam->m_eyePos.z += cam->m_viewVec.x * walk_speed;
	}

	if(GetAsyncKeyState('D')){
	   cam->m_eyePos.x += cam->m_viewVec.z * walk_speed;
	   cam->m_eyePos.z -= cam->m_viewVec.x * walk_speed;
	}

	glutPostRedisplay();
}

//--------------------------
void Scene::Mouse(int button, int state, int x, int y)
{
	/* set mouse position */
	Camera *cam = &camera;

	cam->m_mousePosition.x = x - (glutGet(GLUT_WINDOW_WIDTH) / 2);
	cam->m_mousePosition.y = y - (glutGet(GLUT_WINDOW_HEIGHT) / 2);

    glutPostRedisplay();
}

//--------------------------
void Scene::rotate_view(vec3f *view, float angle, vec3f up) {
	
    float new_x, new_y, new_z;

	float c = cos(angle);
	float s = sin(angle);

	new_x  = (up.x*up.x*(1-c) + c) * view->x;
	new_x += (up.x*up.y*(1-c) - up.z*s)	* view->y;
	new_x += (up.x*up.z*(1-c) + up.y*s)	* view->z;
	
	new_y  = (up.y*up.x*(1-c) + up.z*s)	* view->x;
	new_y += (up.y*up.y*(1-c) + c)	* view->y;
	new_y += (up.y*up.z*(1-c) - up.x*s)	* view->z;

	new_z  = (up.x*up.z*(1-c) - up.y*s)	* view->x;
	new_z += (up.y*up.z*(1-c) + up.x*s)	* view->y;
	new_z += (up.z*up.z*(1-c) + c)	* view->z;

	view->x = new_x;
	view->y = new_y;
	view->z = new_z;

	view->normalize();
}

//---------------------------
void Scene::MouseEvent(int x, int y) {

	/* handle mouse event; rotate camera */
	Camera *cam = &camera;
	vec3f rot_axis;

	int ww = glutGet(GLUT_WINDOW_WIDTH);
    int wh = glutGet(GLUT_WINDOW_HEIGHT);

	x -= (glutGet(GLUT_WINDOW_WIDTH) / 2);
	y -= (glutGet(GLUT_WINDOW_HEIGHT) / 2);

	int rot_x = (float)(x - cam->m_mousePosition.x);
	int rot_y = (float)(y - cam->m_mousePosition.y);

	cam->m_mousePosition.x = x;
	cam->m_mousePosition.y = y;

	rotate_view(&cam->m_viewVec, -rot_x * sensitivity, vec3f(0.0f, 1.0f, 0.0f));

	rot_axis = vec3f(-cam->m_viewVec.z, 0.0f, cam->m_viewVec.x);
	rot_axis.normalize();

	rotate_view(&cam->m_viewVec, -rot_y * sensitivity, rot_axis);
}

//---------------------------
void Scene::setTextureParam(png &tex) {

	glTexImage2D(GL_TEXTURE_2D, 0, tex.getFormat2(), tex.getWidth(), tex.getHeight(), 0, tex.getFormat2(), GL_UNSIGNED_BYTE, tex.getImageData());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex.config->getMinFilterMode());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex.config->getMagFilterMode());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex.config->getWrapsMode());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex.config->getWraptMode());

	if(tex.config->generateMipmaps())
       glGenerateMipmap(GL_TEXTURE_2D);
}

//----------------------------
bool Scene::loadShader(string shader_name, int id) {

	string shaderPath("../shader_program/");
	shaderPath.append(shader_name);

	if(!shader[id].loadShader(string(shaderPath + ".vs").c_str(), 
		string(shaderPath + ".fs").c_str())) return false;
}

//----------------------------
bool Scene::init() {

	/* load shader */
	loadShader(shader_program[0], 0);
	loadShader(shader_program[1], 1);
	loadShader(shader_program[2], 2);

	/* load 3ds model */
	obj[0].m_mesh.loadModel("../objects/Torus.3ds");
	obj[1].m_mesh.loadModel("../objects/Disc.3ds");

	/* build camera projection matrix */
	camera.buildFrustrumMatf(getScreenWidth(), getScreenHeight(), 2.f, 10000.f);

	/* set view vector & eye position */
	camera.m_eyePos = vec3f(0,100,-500.0f);
	camera.m_viewVec= camera.m_eyePos - vec3f(0,0,-1);
	camera.m_viewVec.normalize();

	/* build light camera view & projection matrices */
	lightCamera.buildFrustrumMatf(getScreenWidth(), getScreenHeight(), 2.f, 10000.f);
	lightCamera.buildViewMatf(vec3f(0,100,-1000.0f), vec3f(0,100,-1), vec3f(0,1,0));
	lightCamera.buildViewProjection();

	/* build view & projection matrices for depth texture window */
	depthTexCamera.buildOrthoMatf(2.f, 10000.0f);
	depthTexCamera.buildViewMatf(vec3f(0, 0, 500), vec3f(0, 0, -1), vec3f(0, 1, 0));
	depthTexCamera.buildViewProjection();
	
	/* scale & translate objects */
	obj[0].translateObject(vec3f(0.f, 0.f, -150.f));

	obj[1].translateObject(vec3f(0.f, -40.f, 0.f));
	obj[1].scaleObject(vec3f(5.f, 1.f, 5.f));

	/* generate the VBO object */
    setupVBO(&obj[0]);
	setupVBO(&obj[1]);

	/* setup depth texture window : window & texture coordinates */
	quad = new float[12];
	float *ptr = quad;

	ptr[0] = 100.0f, ptr[1] = -100.0f, ptr[2] = 0.0f;
	ptr[3] = 100.0f, ptr[4] = 100.0f, ptr[5] = 0.0f;
	ptr[6] = -100.0f, ptr[7] = -100.0f, ptr[8] = 0.0f;
	ptr[9] = -100.0f, ptr[10] = 100.0f, ptr[11] = 0.0f;

	texQuad = new float[8];

	texQuad[0] = 1.0f, texQuad[1] = 0.0f;
	texQuad[2] = 1.0f, texQuad[3] = 1.0f;
	texQuad[4] = 0.0f, texQuad[5] = 0.0f;
	texQuad[6] = 0.0f, texQuad[7] = 1.0f;

	/* load textures */
	if(texture[0].png_texture_load("../textures/logo.png")) {

		texture[0].config = new Config(GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, false);
		
		glGenTextures(1, &texture_handle[0]);
		glBindTexture(GL_TEXTURE_2D, texture_handle[0]);

		setTextureParam(texture[0]);
	}

	if(texture[1].png_texture_load("../textures/white.png")) {

		texture[1].config = new Config(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, false);
		
		glGenTextures(1, &texture_handle[1]);
		glBindTexture(GL_TEXTURE_2D, texture_handle[1]);

		setTextureParam(texture[1]);
	}

	/* init shadowmap*/
	glGenFramebuffers(1, &frameBuffer_handle[0]);
	glGenRenderbuffers(1, &renderBuffer_handle[0]);
	glGenTextures(1, &frameBuffer_texture_attachment[0]);
	
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_handle[0]);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer_handle[0]);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

	glBindTexture(GL_TEXTURE_2D, frameBuffer_texture_attachment[0]);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 

	glTexParameteri(GL_TEXTURE_2D, TEXTURE_COMPARE_MODE_EXT, COMPARE_REF_TO_TEXTURE_EXT);
    glTexParameteri(GL_TEXTURE_2D, TEXTURE_COMPARE_FUNC_EXT, GL_LEQUAL);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frameBuffer_texture_attachment[0], 0); 

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

//----------------------------
void Scene::RenderShadowMap() {

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer_handle[0]);
	glBindTexture(GL_TEXTURE_2D, 0);
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(shader[1].getProgramHandle());

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader[1].setUniform("u_shadowViewProjection", lightCamera.m_viewProjection);
	shader[1].setUniform("u_model", obj[0].m_model);

	shader[1].drawScene(obj[0], obj[0].VBO);

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Scene::showDepthTex() {

	glViewport(0, 0, 128, 128);

	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

	int program_handle = shader[2].getProgramHandle();
	
	glUseProgram(program_handle);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBuffer_texture_attachment[0]);
    glTexParameteri(GL_TEXTURE_2D, TEXTURE_COMPARE_MODE_EXT, GL_NONE);
	glUniform1i(glGetUniformLocation(program_handle, "shadowmap"), 0);

	shader[2].setUniform("u_shadowViewProjection", lightCamera.m_viewProjection);
	shader[2].setUniform("u_viewProjection", depthTexCamera.m_viewProjection);
	shader[2].setUniform("u_model", obj[0].m_model);

	int pattribute_handle = glGetAttribLocation(program_handle, "aPosition");
	glEnableVertexAttribArray(pattribute_handle);

	glVertexAttribPointer(pattribute_handle, 3, GL_FLOAT, false, 0, quad);

	int tattribute_handle = glGetAttribLocation(program_handle, "aTexCoord");
	glEnableVertexAttribArray(tattribute_handle);

	glVertexAttribPointer(tattribute_handle, 2, GL_FLOAT, false, 0, texQuad);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

//----------------------------
void Scene::draw() {

	KeyboardEvent();

	RenderShadowMap();
	
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader[0].getProgramHandle());

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBuffer_texture_attachment[0]);
	glTexParameteri(GL_TEXTURE_2D, TEXTURE_COMPARE_MODE_EXT, COMPARE_REF_TO_TEXTURE_EXT);
	glUniform1i(glGetUniformLocation(shader[0].getProgramHandle(), "shadowmap"), 0);

	camera.buildViewMatf(camera.m_eyePos, camera.m_eyePos - camera.m_viewVec, vec3f(0.0f, 1.0f, 0.0f));

	camera.buildViewProjection();

	shader[0].setUniform("u_shadowViewProjection", lightCamera.m_viewProjection);
	shader[0].setUniform("u_viewProjection", camera.m_viewProjection);
	shader[0].setUniform("u_model", obj[0].m_model);

	float eyePos[] = { camera.m_eyePos.x, camera.m_eyePos.y, camera.m_eyePos.z }; 
	shader[0].setUniform("u_eyePosition", eyePos);

	float lightPosition[] = { 0, 100, -500.0f };
	shader[0].setUniform("u_lightPosition", lightPosition);

	float specularPower  = 25.f;
	float ambient[]      = { 0.36f, 0.36f, 0.36f, 1.0f };
	float diffuse[]      = { 0.88f, 0.88f, 0.88f, 1.0f };
	float specular[]     = { 1.0f, 1.0f, 1.0f, 1.0f }; 

	shader[0].setUniform("u_ambient", ambient);
	shader[0].setUniform("u_diffuse", diffuse);
	shader[0].setUniform("u_specular", specular);
	shader[0].setUniform("u_specularPower", specularPower);

	NvBuildIdentityMatf(camera.m_cameraRotateXY);
	NvBuildIdentityMatf(camera.m_cameraTranslate);

	shader[0].drawScene(obj[0], obj[0].VBO);

	shader[0].setUniform("u_model", obj[1].m_model);
	shader[0].drawScene(obj[1], obj[1].VBO);

	showDepthTex();
	
	/* swap front and back buffers */
	glutSwapBuffers();
}
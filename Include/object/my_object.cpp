#include <object/my_object.h>

//-------------------------------------
object::object() {

	NvBuildIdentityMatf(this->m_model);
}

//-------------------------------------
void object::scaleObject(vec3f scaleVector) {

	float scaleMatrix[4][4];
	NvBuildScaleMatf(scaleMatrix, scaleVector.x, scaleVector.y, scaleVector.z);

	float old_model[4][4];
	memcpy(old_model, this->m_model, sizeof(this->m_model));

	NvMultMatf(this->m_model, scaleMatrix, old_model);
}

//-------------------------------------
void object::translateObject(vec3f translateVector) {

	float translateMatrix[4][4];
	NvBuildTranslateMatf(translateMatrix, translateVector.x, translateVector.y, translateVector.z);

	float old_model[4][4];
	memcpy(old_model, this->m_model, sizeof(this->m_model));

	NvMultMatf(this->m_model, translateMatrix, old_model);
}
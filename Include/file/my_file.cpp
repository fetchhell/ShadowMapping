#include "my_file.h"

//-------------------------
bool File::ReadFile(const char *filename, string& shader) {

	FILE* f = fopen(filename, "r");
	
	if(!f) {
		printf("Error openning file %s", filename);
		return false;
	}

	shader = "";
	char str [256];

	while(!feof(f)) {
		fgets(str, 256, f);
		shader.append(str);
	}

	fclose(f);

	return true;
}
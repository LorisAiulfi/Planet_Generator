#version 420 core

in flat float teLvl;
out vec4 color;

void main()
{	
	if(teLvl == 1) {
		color = vec4(0, 1, 0, 1.0f);
	} else if(teLvl == 5) {
		color = vec4(0, 0, 1, 1.0f);
	} else if(teLvl == 20) {
		color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	} else if(teLvl == 64) {
		color = vec4(1, 0, 0, 1.0f);
	} else if(teLvl == 100){
		color = vec4(0.2, 0.2f, 0.2f, 1.0f);
	} else {
		color = vec4(1,1,1,1);
	}
}
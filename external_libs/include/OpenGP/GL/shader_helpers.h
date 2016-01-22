#pragma once
#include <fstream>
#include <iostream>
#include <vector>

namespace opengp{

/// Compiles the vertex, geometry, fragment and tesselation shaders stored in the given strings
GLuint compile_shaders(const char * vshader, const char * fshader, const char * gshader = NULL,
					   const char * tcshader = NULL, const char * teshader = NULL) {
    const int SHADER_LOAD_FAILED = 0;
    GLint Success = GL_FALSE;
    int InfoLogLength;

    /// Create the Vertex Shader
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    /// Compile Vertex Shader
    fprintf(stdout, "Compiling Vertex shader: ");
    char const * VertexSourcePointer = vshader;
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    /// Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Success);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(!Success) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "Failed:\n%s\n", &VertexShaderErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");

    /// Create the Fragment Shader
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    /// Compile Fragment Shader
    fprintf(stdout, "Compiling Fragment shader: ");
    char const * FragmentSourcePointer = fshader;
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    /// Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Success);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(!Success) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "Failed:\n%s\n", &FragmentShaderErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");

    GLuint GeometryShaderID = 0;
    if(gshader != NULL) {
        /// Create the Geometry Shader
        GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);

        /// Compile Geometry Shader
        fprintf(stdout, "Compiling Geometry shader: ");
        char const * GeometrySourcePointer = gshader;
        glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer , NULL);
        glCompileShader(GeometryShaderID);

        /// Check Geometry Shader
        glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Success);
        glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(!Success) {
            std::vector<char> GeometryShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, &GeometryShaderErrorMessage[0]);
            fprintf(stdout, "Failed:\n%s\n", &GeometryShaderErrorMessage[0]);
            return SHADER_LOAD_FAILED;
        }
        else
            fprintf(stdout, "Success\n");
    }

	GLuint TessControlShaderID = 0;
	if (tcshader != NULL) {
		/// Create the Tess Control Shader
		TessControlShaderID = glCreateShader(GL_TESS_CONTROL_SHADER);

		/// Compile Tess Control Shader
		fprintf(stdout, "Compiling Tess Control Shader: ");
		char const * TessControlSourcePointer = tcshader;
		glShaderSource(TessControlShaderID, 1, &TessControlSourcePointer, NULL);
		glCompileShader(TessControlShaderID);

		/// Check Tess Control Shader
		glGetShaderiv(TessControlShaderID, GL_COMPILE_STATUS, &Success);
		glGetShaderiv(TessControlShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (!Success) {
			std::vector<char> TessControlShaderErrorMessage(InfoLogLength);
			glGetShaderInfoLog(TessControlShaderID, InfoLogLength, NULL, &TessControlShaderErrorMessage[0]);
			fprintf(stdout, "Failed:\n%s\n", &TessControlShaderErrorMessage[0]);
			return SHADER_LOAD_FAILED;
		}
		else
			fprintf(stdout, "Success\n");
	}

	GLuint TessEvalShaderID = 0;
	if (teshader != NULL) {
		/// Create the Tess Eval Shader
		TessEvalShaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);

		/// Compile Tess Eval Shader
		fprintf(stdout, "Compiling Tess Eval Shader: ");
		char const * TessEvalSourcePointer = teshader;
		glShaderSource(TessEvalShaderID, 1, &TessEvalSourcePointer, NULL);
		glCompileShader(TessEvalShaderID);

		/// Check Tess Eval Shader
		glGetShaderiv(TessEvalShaderID, GL_COMPILE_STATUS, &Success);
		glGetShaderiv(TessEvalShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (!Success) {
			std::vector<char> TessEvalShaderErrorMessage(InfoLogLength);
			glGetShaderInfoLog(TessEvalShaderID, InfoLogLength, NULL, &TessEvalShaderErrorMessage[0]);
			fprintf(stdout, "Failed:\n%s\n", &TessEvalShaderErrorMessage[0]);
			return SHADER_LOAD_FAILED;
		}
		else
			fprintf(stdout, "Success\n");
	}

    /// Link the program
    fprintf(stdout, "Linking shader program: ");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    if(gshader != NULL) glAttachShader(ProgramID, GeometryShaderID);
	if(tcshader != NULL) glAttachShader(ProgramID, TessControlShaderID);
	if(teshader != NULL) glAttachShader(ProgramID, TessEvalShaderID);
    glLinkProgram(ProgramID);

    /// Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Success);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    if(!Success) {
        fprintf(stdout, "Failed:\n%s\n", &ProgramErrorMessage[0]);
        return SHADER_LOAD_FAILED;
    }
    else
        fprintf(stdout, "Success\n");

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    if(gshader != NULL) glDeleteShader(GeometryShaderID);
	if(tcshader != NULL) glDeleteShader(TessControlShaderID);
	if(teshader != NULL) glDeleteShader(TessEvalShaderID);

    /// make sure you see the text in terminal
    fflush(stdout);

    return ProgramID;
}


/// Compiles the vertex, geometry and fragment shaders using file path
GLuint load_shaders(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path = NULL, 
					const char * tess_control_file_path = NULL, const char * tess_evaluation_file_path = NULL) {
    const int SHADER_LOAD_FAILED = 0; 

    std::string VertexShaderCode, FragmentShaderCode, GeometryShaderCode, TessControlShaderCode, TessEvalShaderCode;
    {
        /// Read the Vertex Shader code from the file
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if(VertexShaderStream.is_open()) {
            VertexShaderCode = std::string(std::istreambuf_iterator<char>(VertexShaderStream),
                                           std::istreambuf_iterator<char>());
            VertexShaderStream.close();
        } else {
            printf("Could not open file: %s\n", vertex_file_path);
            return SHADER_LOAD_FAILED;
        }   
    
        /// Read the Fragment Shader code from the file
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if(FragmentShaderStream.is_open()) {
            FragmentShaderCode = std::string(std::istreambuf_iterator<char>(FragmentShaderStream),
                                             std::istreambuf_iterator<char>());
            FragmentShaderStream.close();
        } else {
            printf("Could not open file: %s\n", fragment_file_path);
            return SHADER_LOAD_FAILED;
        }

        /// Read the Geometry Shader code from the file
        if(geometry_file_path != NULL) {
            std::ifstream GeometryShaderStream(geometry_file_path, std::ios::in);
            if(GeometryShaderStream.is_open()) {
                GeometryShaderCode = std::string(std::istreambuf_iterator<char>(GeometryShaderStream),
                                                 std::istreambuf_iterator<char>());
                GeometryShaderStream.close();
            } else {
                printf("Could not open file: %s\n", geometry_file_path);
                return SHADER_LOAD_FAILED;
            }
        }

		/// Read the Tesselation Control Shader code from the file
		if (tess_control_file_path != NULL) {
			std::ifstream TessControlShaderStream(tess_control_file_path, std::ios::in);
			if (TessControlShaderStream.is_open()) {
				TessControlShaderCode = std::string(std::istreambuf_iterator<char>(TessControlShaderStream),
													std::istreambuf_iterator<char>());
				TessControlShaderStream.close();
			}
			else {
				printf("Could not open file: %s\n", tess_control_file_path);
				return SHADER_LOAD_FAILED;
			}
		}

		/// Read the Tesselation Evaluation Shader code from the file
		if (tess_evaluation_file_path != NULL) {
			std::ifstream TessEvalShaderStream(tess_evaluation_file_path, std::ios::in);
			if (TessEvalShaderStream.is_open()) {
				TessEvalShaderCode = std::string(std::istreambuf_iterator<char>(TessEvalShaderStream),
					std::istreambuf_iterator<char>());
				TessEvalShaderStream.close();
			}
			else {
				printf("Could not open file: %s\n", tess_evaluation_file_path);
				return SHADER_LOAD_FAILED;
			}
		}
    }

    /// Compile them
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    char const * GeometrySourcePointer = NULL;
	char const * TessControlSourcePointer = NULL;
	char const * TessEvalSourcePointer = NULL;
    if(geometry_file_path != NULL) GeometrySourcePointer = GeometryShaderCode.c_str();
	if(tess_control_file_path != NULL) TessControlSourcePointer = TessControlShaderCode.c_str();
	if(tess_evaluation_file_path != NULL) TessEvalSourcePointer = TessEvalShaderCode.c_str();
    return compile_shaders(VertexSourcePointer, FragmentSourcePointer, GeometrySourcePointer, TessControlSourcePointer, TessEvalSourcePointer);
}

} //< opengp::

const char* vertexShaderCode = //responsible for getting data of vertices 
"#version 430\r\n"
""
"in layout(location = 0) vec2 position;" // receive data at position 0 of attribute array
"in layout(location = 1) vec3 vertexColor;" // receive data at position 1 of attribute array 
""
"out vec3 theColor;" // declares you want to send color out 
""
"void main()"
"{"
"	gl_Position = vec4(position,0.0,1.0);" // specifies position 
"	theColor = vertexColor;" // sends color out 
"}";

const char* fragmentShaderCode = //responsible for filling in pixels 
"#version 430\r\n"
""
"out vec4 Color;" //declares you want to send color out 
"in vec3 theColor;" //declares you are receiving data as a vec3
""
"void main()"
"{"
"	Color = vec4(theColor, 1.0);"
"}";
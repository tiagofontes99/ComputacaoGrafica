"#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    //adicional para textura
    "layout (location = 1) in vec2 aTexCoord;"
    "out vec2 TexCoord;"
    //*****
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    //adicional texture
        "TexCoord = aTexCoord;"
    "}\0";
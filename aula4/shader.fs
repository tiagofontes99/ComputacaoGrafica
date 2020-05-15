"#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;"
    //"uniform vec4 ourColor;\n"
    "uniform sampler2D ourTexture;"// recebe o ID da textura gerada, através do BindTexture()
    "void main()\n"
    "{\n"
    //"    FragColor = ourColor;\n"
        "FragColor= texture=(ourTexture ,TexCoord);"
    "}\0";


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
using namespace std;

void framebuffer_tamanho_callback(GLFWwindow* ventana, int ancho, int alto) {
    glViewport(0, 0, ancho, alto);
}
void procesarEntrada(GLFWwindow* ventana) {
    if (glfwGetKey(ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(ventana, true);
}

const unsigned int ANCHO = 800;
const unsigned int ALTO  = 600;

//Definir el código de Vertex Shader
const char* codigo_vertex_shader =
"#version 330 core\n"
"layout (location = 0) in vec3 pPosicion;\n"
"void main(){\n"
"    gl_Position = vec4(pPosicion, 1.0);\n"
"}\0";

//Definir el código de Fragment Shader
const char* codigo_fragment_shader =
"#version 330 core\n"
"out vec4 Color;\n"
"void main(){\n"
"    Color = vec4(0.0, 1.0, 0.0, 0.0);\n"
"}\0";


int fact (int n){
    if(n==0)
        return 1;
    else
        return n*fact(n-1);
    
}

int comb(int n, int r){
    return (fact(n)/(fact(n-r)*fact(r)));
}

//drawing the bezier Curve


int main() {
    int i,n;
    float t,x,y,px[10],py[10];
    float bezier[10000];
    int a = 0;
    printf("Enter number of points:");
    scanf("%d",&n);
    n=n-1;
    for(i=0;i<=n;i++){
        printf("\nP%d\n",i);
        printf("x=");
        scanf("%f",&px[i]);
        printf("y=");
        scanf("%f",&py[i]);
        bezier[a]=px[i]/10;
        bezier[a+1] = py[i]/10;
        bezier[a+2] = 0;
        a =a +3;
    };
    for(t=0;t<=1;t=t+0.001){
        x=0;y=0;
        for(i=0;i<=n;i++){
            x = x+(comb(n, i) * pow((1-t), (n-i)) * pow(t, i) * px[i]);
            y = y+(comb(n, i) * pow((1-t), (n-i)) * pow(t, i) * py[i]);
        }
        bezier[a] = x/10;
        bezier[a+1] = y/10;
        bezier[a+2] = 0;
        a = a+3;
    }
    
    
    
    //Inicializar glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Creando la ventana
    GLFWwindow* ventana = glfwCreateWindow(ANCHO, ALTO, "Compu Grafica", NULL, NULL);
    if (ventana == NULL) {
        cout << "Problemas al crear la ventana\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(ventana);
    glfwSetFramebufferSizeCallback(ventana, framebuffer_tamanho_callback);

    //Cargar Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Problemas al cargar GLAD\n";
        return -1;
    }

    //Compilar y Linkear los Shaders
    int ok;
    //Vertex Shader
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &codigo_vertex_shader, NULL);
    glCompileShader(vertex_shader_id);
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[256];
        glGetShaderInfoLog(vertex_shader_id, 256, NULL, log);
        cout << "Error de compilación con el vertex shader: " << log << "\n";
    }
    //Fragment Shader
    //Creando un programa en la tarjeta gráfica para mi shader, y obteniendo el identificador asociado
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    //Asociando al programa, de la tarjeta gráfica, con identificador "fragment_shader_id" el código en la dirección apuntada por
    //"codigo_fragment_shader"
    glShaderSource(fragment_shader_id, 1, &codigo_fragment_shader, NULL);
    //Pedimos a la tarjeta gráfica que compile el programa con identificador "fragment_shader_id"
    glCompileShader(fragment_shader_id);
    //Solicitamos saber el resu ltado de la compilación en "compilo_bien"
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[256];
        //Solicitamos saber cuales fueron los errores al compilar el código del shader
        glGetShaderInfoLog(fragment_shader_id, 256, NULL, log);
        cout << "Error de compilación con el fragment shader: " << log << "\n";
    }
    //Enlazando vertex y fragment shaders
    unsigned int programa_shader_id = glCreateProgram();
    glAttachShader(programa_shader_id, vertex_shader_id);
    glAttachShader(programa_shader_id, fragment_shader_id);
    glLinkProgram(programa_shader_id);
    glGetProgramiv(programa_shader_id, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[256];
        glGetProgramInfoLog(programa_shader_id, 256, NULL, log);
        cout << "Error de enlace con el programa: " << log << "\n";
    }
    //Ahora ya podemos eliminar los programas de los shaders
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    
//    float pointVertex[]= {0,0,0,
//        0.2,0.2,0,
//        0.4,0.4,0
//    };
    //Definiendo la geometría de la figura en función de vértices
    GLfloat vertices[] = {
         -0.5, -0.5,0,
        0.5, -0.5,0.0,
        0.0, 0.5,0.0

    };

    //Enviando la geometría al GPU: Definiendo los buffers (Vertex Array Objects y Vertex Buffer Objects)
    unsigned int id_array_vertices, id_array_buffers;
    glGenVertexArrays(1, &id_array_vertices);
    glGenBuffers(1, &id_array_buffers);
    glBindVertexArray(id_array_vertices);//anexando para su ejecución en la tarjeta gráfica

    //Anexar buffer y enviarle los datos
    glBindBuffer(GL_ARRAY_BUFFER, id_array_buffers);
//glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertex), pointVertex, GL_STATIC_DRAW);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bezier), bezier, GL_STATIC_DRAW);

// Primer Atributo q contiene 3 float = posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(ventana)) {
        procesarEntrada(ventana);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(programa_shader_id);
        glBindVertexArray(id_array_vertices);
        glDrawArrays(GL_LINE_STRIP,0,n+1);
        glDrawArrays(GL_POINTS,0,a/3);

        glfwSwapBuffers(ventana);
        glfwPollEvents();
    }
    //Liberando memoria
    glDeleteVertexArrays(1, &id_array_vertices);
    glDeleteBuffers(1, &id_array_buffers);
    glDeleteProgram(programa_shader_id);
    glfwTerminate();
    return 0;
}

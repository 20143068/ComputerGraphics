#include <GL/glew.h>
#include <GL/freeglut.h>
#include "vec.hpp"
#include "mat.hpp"
#include "operator.hpp"
#include "transform.hpp"
#include "hilbert.hpp"

unsigned int num_rects = 10; //초기 사각형 group의 갯수

float offset = 0.05f; //초기 offset값
float group_size; //가장 바깥쪽 사각형의 크기

int max_depth = 3; //Hilbert Curve의 level
int colorCount = 2; //초기 color값
int check = 1; //context menu 호출 조건 변수
int flag = 0; //context menu 호출 조건 변수
int ratio = 1; //level당 출력되는 rectangle group의 비율

void init();
void do_myMenu();
void mydisplay();
void create_myMenu();  //context menu 호출 함수

GLuint create_shader_from_file(const std::string& filename, GLuint shader_type);
GLuint program; // 쉐이더 프로그램 객체의 레퍼런스 값
GLint  loc_a_position;
GLint  loc_a_color;
GLint  loc_u_Model;

int main(int argc, char* argv[]) {
  srand((unsigned int)time(NULL));
  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Hello FreeGLUT");

  init();
  create_myMenu();
  glutDisplayFunc(mydisplay);
  glutMainLoop();

  return 0;
}

// GLSL 파일을 읽어서 컴파일한 후 쉐이더 객체를 생성하는 함수
GLuint create_shader_from_file(const std::string& filename, GLuint shader_type) {
  GLuint shader = 0;
  shader = glCreateShader(shader_type);
  std::ifstream shader_file(filename.c_str());
  std::string shader_string;
  shader_string.assign((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
  const GLchar* shader_src = shader_string.c_str();
  glShaderSource(shader, 1, (const GLchar**)&shader_src, NULL);
  glCompileShader(shader);

  return shader;
}

void init() {
  glewInit();
  // 정점 쉐이더 객체를 파일로부터 생성
  GLuint vertex_shader = create_shader_from_file("./shader/vertex.glsl", GL_VERTEX_SHADER);
  // 프래그먼트 쉐이더 객체를 파일로부터 생성
  GLuint fragment_shader = create_shader_from_file("./shader/fragment.glsl", GL_FRAGMENT_SHADER);
    // 쉐이더 프로그램 생성 및 컴파일
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  loc_a_position = glGetAttribLocation(program, "a_position");
  loc_a_color    = glGetAttribLocation(program, "a_color");
  loc_u_Model    = glGetUniformLocation(program, "u_Model");
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void draw_a_rectangle(float cx, float cy, float size , float r , float g , float b) {  //사각형을 그리는 함수
  kmuvcl::math::mat4x4f model_matrix;
  kmuvcl::math::mat4x4f S, T;

  float position[] = {
    //right-down triangle
    0.5f,  0.5f, 0.0f, 1.0f,
   -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f,
    //left-bottom triangle
    0.5f,  0.5f, 0.0f, 1.0f,
   -0.5f,  0.5f, 0.0f, 1.0f,
   -0.5f, -0.5f, 0.0f, 1.0f,
  };

  float color[] = {   //각 vertex에 대한 color값을 저장할 배열
      r, g, b, 1.0f,
      r, g, b, 1.0f,
      r, g, b, 1.0f,
      r, g, b, 1.0f,
      r, g, b, 1.0f,
      r, g, b, 1.0f,
    };

  T = kmuvcl::math::translate(cx, cy, 0.0f);
  S = kmuvcl::math::scale(size, size, 1.0f);

  model_matrix = T*S;

  glUniformMatrix4fv(loc_u_Model , 1/*매트릭스 하나*/, GL_FALSE , model_matrix );
  glVertexAttribPointer(loc_a_position , 4 , GL_FLOAT , GL_FALSE , 0 , position);
  glVertexAttribPointer(loc_a_color , 4 , GL_FLOAT , GL_FALSE , 0 , color);

  glEnableVertexAttribArray(loc_a_position);
  glEnableVertexAttribArray(loc_a_color);

  glDrawArrays(GL_TRIANGLES , 0 , 6);
  glDisableVertexAttribArray(loc_a_position);
  glDisableVertexAttribArray(loc_a_color);
}

void draw_a_rectangle_group(float cx, float cy) {  //한 사각형 group을 그리는 함수
  float size = group_size;
  float r;
  float g;
  float b;

  while(size >= 0) {  //사각형의 한 변의 길이가 0이 될 때까지 출력
    if (colorCount == 1) {   //red 계열의 색 적용
        r = 0.5 + (float)rand()/RAND_MAX*0.5;
        g = (float)rand()/RAND_MAX*0.5;
        b = (float)rand()/RAND_MAX*0.5;
    }

    else if (colorCount == 2) {    //green 계열의 색 적용
        r = (float)rand()/RAND_MAX*0.5;
        g = 0.5+(float)rand()/RAND_MAX*0.5;
        b = (float)rand()/RAND_MAX*0.5;
    }

    else if (colorCount == 3) {   //blue 계열의 색 적용
        r = (float)rand()/RAND_MAX*0.5;
        g = (float)rand()/RAND_MAX*0.5;
        b = 0.5+(float)rand()/RAND_MAX*0.5;
    }
    draw_a_rectangle(cx, cy, size, r, g, b);
    size -= offset;  //한 번 그리고 offest값으로 사각형 크기 조정
    }
}

void myMainMenu(int value) {
  if (value == 1) exit(0);
}

void offset_menu(int value){ //offset값을 재설정하는 메뉴
    switch(value){
      case 1:
        offset = 0.07; check++;
        break;
      case 2:
        offset = 0.05; check++;
        break;
      case 3:
        offset = 0.03; check++;
        break;
    }
    glutPostRedisplay();
}

void minmax_menu(int value){ //각 level에서 출력할 사각형 group의 최소/최대값을 정하는 메뉴
  switch(value) {
    case 1:
      num_rects = 1 + rand()%5; check++; //1부터 5까지
      break;
    case 2:
      num_rects = 3 + rand()%13; check++;  //3부터 15까지
      break;
    case 3:
      num_rects = 5 + rand()%26; check++;  //5부터 30까지
      break;
    }
    glutPostRedisplay();
}

void Ratio_Menu(int value) { //level당 출력되는 rectangle group의 비율을 설정하는 메뉴
  switch(value) {
    case 1:
      ratio *= 2; check++;
      break;
    case 2:
      ratio *= 3; check++;
      break;
    case 3:
      ratio *= 5; check++;
      break;
    case 4:
      ratio = 1; check++;
      break;
  }
  glutPostRedisplay();
}

void color_menu(int value){ //Color menu

  switch(value){
    case 1: //red 계열로 color 배열 setting
      colorCount=1; check++;
      break;
    case 2: //green 계열로 color 배열 setting
      colorCount=2; check++;
      break;
    case 3:  //blue 계열로 color 배열 setting
      colorCount=3; check++;
      break;
  }
  glutPostRedisplay();
}

void create_myMenu() { //context menu 생성 함수
    GLint offset_subMenu = glutCreateMenu(offset_menu); //Offsetting에 대한 sub menu
    glutAddMenuEntry("7", 1);
    glutAddMenuEntry("5", 2);
    glutAddMenuEntry("3", 3);

    GLint minmax_subMenu = glutCreateMenu(minmax_menu); //Min/Max value에 대한 sub menu
    glutAddMenuEntry("1 / 5", 1);
    glutAddMenuEntry("3 / 15", 2);
    glutAddMenuEntry("5 / 30", 3);

    GLint Ratio_subMenu = glutCreateMenu(Ratio_Menu);  //Ratio에 대한 sub menu
    glutAddMenuEntry("2", 1);
    glutAddMenuEntry("3", 2);
    glutAddMenuEntry("5", 3);
    glutAddMenuEntry("Initialize Ratio to 1", 4);  //Ratio를 1로 초기화

    GLint Color_subMenu = glutCreateMenu(color_menu);  //Color에 대한 sub menu
    glutAddMenuEntry("red", 1);
    glutAddMenuEntry("green", 2);
    glutAddMenuEntry("blue", 3);

    GLint MyMainMenu = glutCreateMenu(myMainMenu);  //main menu 생성
    glutAddSubMenu("Offsetting", offset_subMenu);  //각각의 sub menu를 연결
    glutAddSubMenu("Min/Max numbers of square groups in each level", minmax_subMenu);
    glutAddSubMenu("Ratio", Ratio_subMenu);
    glutAddSubMenu("Color Mode", Color_subMenu);

    glutAttachMenu(GLUT_RIGHT_BUTTON);  //우클릭시 menu 호출
}

void mydisplay() {
  if(check == flag) return; //context menu 호출 조건 설정
  check = flag;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program);

  vector<Point> points;
  for(int i = 1 ; i <=  max_depth ; i++) {  //해당 level에서 사각형 출력
      setCoordinate(points, i);

      for(int j = 0; j < num_rects ; j++) {
        int index = rand()%points.size();
        Point p = getRandom(points , index);  //Hilbert curve 위에 존재하는 임의의 좌표값 설정
        group_size = (float)rand()/RAND_MAX*0.5; //가장 바깥쪽 사각형의 크기를 임의로 설정
        draw_a_rectangle_group(p.x, p.y);
      }
      num_rects *= ratio;
  }
  num_rects = 10;

  glUseProgram(0);
  glutSwapBuffers();
}

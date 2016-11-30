#include "Common.hpp"
#include "Global.hpp"
#include <cstdlib>

// This function is called whenever the display needs to be updated
void
displayCallback()
{
  // Set the camera matrix (the projection matrix doesn't need to be touched unless the window is resized)
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrix(Global::camera.getWorldToCamera());

  // Clear the framebuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Issue OpenGL calls to draw the scene
  if (!Global::scene.draw())
    exit(-1);

  // This function swaps the draw buffer and the display buffer. The current frame, in the draw buffer, is transferred to the
  // display. The previous display buffer is now the new draw buffer. Using two buffers reduces flickering, since we don't
  // draw directly to the display. (We set up double-buffering with GLUT_DOUBLE in the main function.)
  glutSwapBuffers();
}

// This function is called whenever the window is resized
void
resizeCallback(int new_width, int new_height)
{
  // Prevent a divide by zero, when window is too short (you can't make a window of zero width)
  if (new_height == 0) new_height = 1;

  // Get the aspect ratio of the window
  float aspect_ratio = new_width / (float)new_height;

  // Set the viewport to be the entire window
  glViewport(0, 0, new_width, new_height);

  // Produce a new perspective matrix, to handle the aspect ratio
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(Global::camera.getFOV(),
                 aspect_ratio,
                 Global::camera.getNearDistance(),
                 Global::camera.getFarDistance());

  // Trigger a redraw
  glutPostRedisplay();
}

// This function is called whenever the user presses a normal key
void
keyboardCallback(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27 /* ESC */:
    case 'q':
      exit(0);  // no other way to get out of GLUT loop

    default:
      return;
  }

  glutPostRedisplay();
}


// This function is called whenever the user presses a "special" key, e.g. an arrow key
void
specialKeyCallback(int key, int x, int y)
{
  
  float angle = 5.0, dz = 5.0; // TODO: set suitable values and check for lookup vector
  Mat4 transform;
  // Vec3 look = Global::camera.getLookVector();

  switch (key)
  {
    case GLUT_KEY_LEFT:  
      transform = rotation3D(Vec3(0.0, 1.0, 0.0), -angle);
      break;
    case GLUT_KEY_RIGHT: 
      transform = rotation3D(Vec3(0.0, 1.0, 0.0), angle);
      break;
    case GLUT_KEY_UP:    
      transform = translation3D(Vec3(0.0, 0.0, dz));
      break;
    case GLUT_KEY_DOWN:  
      transform = translation3D(Vec3(0.0, 0.0, -dz));
      break;
    case GLUT_KEY_F1:  
      transform = rotation3D(Vec3(1.0, 0.0, 0.0), -angle);
      break;
    case GLUT_KEY_F2:  
      transform = rotation3D(Vec3(1.0, 0.0, 0.0), angle);
      break;
    case GLUT_KEY_F3:
      transform = translation3D(Vec3(0.0, 0.0, 10*dz));
      break;
    case GLUT_KEY_F4:
      transform = translation3D(Vec3(0.0, 0.0, -10*dz));
      break;
    default:
      return;
  }
  Global::camera.setWorldToCamera(transform*Global::camera.getWorldToCamera());

  glutPostRedisplay();
}

void
mousePressCallback(int button, int state, int x, int y)
{
  // glutPostRedisplay();
}

void
mouseMoveCallback(int x, int y)
{
  // glutPostRedisplay();
}

int
main(int argc, char * argv[])
{
  if (argc != 2 && argc != 7)
  {
    cerr << "Usage: " << argv[0] << " <scene-filename> [<ma> <ml> <ms> <msp> <msm>]" << endl;
    return -1;
  }

  // Setup for GLUT, which produces a window onto which you can draw using OpenGL
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  // glutInitWindowPosition(50, 50);
  glutInitWindowSize(800, 600);
  glutCreateWindow("CS475 OpenGL Viewer");

  // Initialize GL extensions via GLEW
  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    // Problem: glewInit failed, something is seriously wrong
    cerr << "Error: " << glewGetErrorString(err) << endl;
    return -1;
  }

  // Check if shaders are supported
  if (!GLEW_ARB_shader_objects)
  {
    cerr << "Your GL driver does not support shaders! Please update your driver.\n";
    cerr << "If that doesn't work, please contact course staff immediately." << endl;
    return -1;
  }

  // Set up callbacks
  glutDisplayFunc(displayCallback);
  glutReshapeFunc(resizeCallback);
  glutKeyboardFunc(keyboardCallback);
  glutSpecialFunc(specialKeyCallback);
  glutMouseFunc(mousePressCallback);
  glutMotionFunc(mouseMoveCallback);

  // Read command line
  string mesh_path = argv[1];
  float ma = 0.1f;
  float ml = 0.7f;
  float ms = 0.2f;
  float msp = 0.5f;
  float msm = 0.2f;
  if (argc == 7)
  {
    ma  = atof(argv[2]);
    ml  = atof(argv[3]);
    ms  = atof(argv[4]);
    msp = atof(argv[5]);
    msm = atof(argv[6]);
  }

  Material m(ma, ml, ms, msp, msm, 0.0f, 0.0f, 0.0f);

  // Load the scene
  cout << "Loading scene '" << mesh_path << '\'' << endl;
  if (!Global::scene.load(mesh_path, m))
  {
    cerr << "Error loading scene!" << endl;
    return -1;
  }

  // set camera 
  Vec3 shift = Global::scene.getTopOfSky();
  Mat4 w2c = Global::camera.getWorldToCamera();
  for (int i = 0; i < 3; i++) {
    w2c[i][3] = shift[i];
  }
  Global::camera.setWorldToCamera(w2c);

  // Activate depth test and 2D texturing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT2);
  glEnable(GL_COLOR_MATERIAL);
  // Enter the GLUT input processing loop (resizeCallback will be called once just before the window is displayed -- initial
  // camera setup happens here)
  glutMainLoop();

  // This is never reached
  return 0;
}

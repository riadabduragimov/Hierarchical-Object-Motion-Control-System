#include "stdafx.h"
#include "Figure.h"

// Global Figure object representing the articulated walking figure
Figure figure;

// Display callback 
// Responsible for rendering the scene each frame
void display() {
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up a camera that follows the figure from behind and above
    Vec3 camPos = figure.position + Vec3(0, 5, 15); // offset camera
    gluLookAt(camPos.x, camPos.y, camPos.z,        // camera position
        figure.position.x, 1.0f, figure.position.z, // look-at position
        0, 1, 0);                             // up vector

    // Draw ground grid for reference
    glColor3f(0.6f, 0.6f, 0.6f); // gray color
    glBegin(GL_LINES);
    for (float x = -20; x <= 20; x += 1) {
        glVertex3f(x, 0, -20); glVertex3f(x, 0, 20);
    }
    for (float z = -20; z <= 20; z += 1) {
        glVertex3f(-20, 0, z); glVertex3f(20, 0, z);
    }
    glEnd();

    // Draw the walking figure
    figure.draw();

    // Swap buffers to display the rendered frame
    glutSwapBuffers();
}

// Idle callback 
// Called repeatedly by GLUT when the system is idle
void idle() {
    // Compute time elapsed since last frame (delta time)
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float dt = (currentTime - lastTime) / 1000.0f; // convert ms to seconds
    lastTime = currentTime;

    // Update the figure's animation based on elapsed time
    figure.update(dt);

    // Request redisplay for continuous animation
    glutPostRedisplay();
}

// Window resize callback 
void reshape(int w, int h) {
    if (h == 0) h = 1; // prevent division by zero

    // Set the viewport to the entire window
    glViewport(0, 0, w, h);

    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / h, 0.1, 100.0);

    // Return to modelview matrix for rendering
    glMatrixMode(GL_MODELVIEW);
}

// OpenGL initialization 
void initGL() {
    glEnable(GL_DEPTH_TEST);           // Enable depth testing for 3D
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Set background color (light gray)
}

// Main function 
int main(int argc, char** argv) {
    // Initialize GLUT library
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Hierarchical Walking Figure");

    // Initialize OpenGL settings
    initGL();

    // Load geometry for the articulated figure
    if (!figure.loadGeometry("torso.txt", "left_leg.txt", "right_leg.txt"))
        std::cout << "Error loading geometry\n";

    // Load the control points defining the figure's path
    if (!figure.loadPath("path.txt"))
        std::cout << "Error loading path\n";

    // Register GLUT callbacks
    glutDisplayFunc(display); // rendering
    glutIdleFunc(idle);       // animation
    glutReshapeFunc(reshape); // window resizing

    // Start the main event loop
    glutMainLoop();
    return 0;
}

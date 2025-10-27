#include "stdafx.h"
#include "Figure.h"

// Define PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Part draw 
// Draw a single articulated part (torso or leg)
// Uses OpenGL transformations for rotation, scaling, and hierarchical drawing
void Part::draw() {
    glPushMatrix();                        // Save current transformation state
    glRotatef(angle, 0, 0, 1);             // Rotate part around its local Z-axis

    glPushMatrix();
    glScalef(width, length, width);        // Scale cube to match part dimensions
    glutSolidCube(1.0f);                   // Draw solid cube representing the part
    glPopMatrix();

    glTranslatef(0, length, 0);            // Translate to top for child part
    if (child) child->draw();              // Recursively draw child part
    glPopMatrix();                          // Restore previous transformation
}

// Figure constructor 
// Initializes the figure with default geometry, animation parameters, and state
Figure::Figure()
    : torso(2.0f, 1.0f), leftLeg(1.5f, 0.5f), rightLeg(1.5f, 0.5f),
    time(0), pathT(0), speed(1.0f), angleY(0), position(0, 0, 0), dt(0.01f),
    swayAmplitude(0.1f), swayFrequency(1.0f),
    leanAmplitude(5.0f), hipTiltAmplitude(5.0f)
{
    torso.child = nullptr;                 // Torso has no child part by default
}

// Load geometry 
// Load the dimensions (length, width) of torso and legs from files
bool Figure::loadGeometry(const std::string& torsoFile,
    const std::string& leftLegFile,
    const std::string& rightLegFile)
{
    std::ifstream f;
    f.open(torsoFile); if (!f) return false; f >> torso.length >> torso.width; f.close();
    f.open(leftLegFile); if (!f) return false; f >> leftLeg.length >> leftLeg.width; f.close();
    f.open(rightLegFile); if (!f) return false; f >> rightLeg.length >> rightLeg.width; f.close();
    return true;
}

// Load path  
// Load control points and dt from a file to define the figure's walking trajectory
bool Figure::loadPath(const std::string& pathFile) {
    std::ifstream f(pathFile);
    if (!f) return false;

    f >> dt;                               // First value is the spacing for interpolation
    controlPoints.clear();
    float x, y, z;
    while (f >> x >> y >> z)               // Read remaining points as Vec3
        controlPoints.push_back(Vec3(x, y, z));
    f.close();
    return true;
}

// Update
// Update the figure's position, orientation, and joint angles based on elapsed time
void Figure::update(float deltaTime) {
    time += deltaTime;                      // Update global animation time

    if (controlPoints.size() < 4) return;  // Need at least 4 points for Catmull-Rom

    // Move along path using Catmull-Rom interpolation
    pathT += speed * deltaTime;             // Increment path parameter by speed
    if (pathT > controlPoints.size() - 3) pathT -= (controlPoints.size() - 3);

    int idx = int(pathT);
    float t = pathT - idx;

    Vec3 P0 = controlPoints[idx % controlPoints.size()];
    Vec3 P1 = controlPoints[(idx + 1) % controlPoints.size()];
    Vec3 P2 = controlPoints[(idx + 2) % controlPoints.size()];
    Vec3 P3 = controlPoints[(idx + 3) % controlPoints.size()];

    position = catmullRom(P0, P1, P2, P3, t);           // Interpolated position
    Vec3 nextPos = catmullRom(P0, P1, P2, P3, t + 0.01f);
    Vec3 dir = nextPos - position;
    angleY = atan2(dir.x, dir.z) * 180.0f / M_PI;       // Heading angle based on path

    // Leg swing: sinusoidal oscillation proportional to speed
    float legAmplitude = 30.0f;                          // Max swing angle
    float legFrequency = 1.0f * speed;                  // Swing frequency
    leftLeg.angle = legAmplitude * sin(2 * M_PI * legFrequency * time);
    rightLeg.angle = legAmplitude * sin(2 * M_PI * legFrequency * time + M_PI);

    swayFrequency = legFrequency;                        // Sync torso sway with leg motion
}

// Draw 
// Draw the articulated figure with torso lean, hip tilt, and leg motion
void Figure::draw() {
    glPushMatrix();

    // Compute lateral sway and vertical bounce of the torso
    float swayX = swayAmplitude * sin(2 * M_PI * swayFrequency * time);
    float swayY = swayAmplitude * 0.5f * cos(2 * M_PI * swayFrequency * time);

    // Compute forward/backward lean of the torso
    float lean = leanAmplitude * sin(2 * M_PI * swayFrequency * time);

    // Compute hip tilt for natural walking motion
    float hipTilt = hipTiltAmplitude * sin(2 * M_PI * swayFrequency * time);

    // Translate to figure's position in world space with sway
    glTranslatef(position.x + swayX, swayY, position.z);
    glRotatef(angleY, 0, 1, 0);                        // Rotate to face walking direction

    // Draw torso with lean and tilt
    glPushMatrix();
    glTranslatef(0, torso.length / 2, 0);             // Pivot at torso base
    glRotatef(lean, 1, 0, 0);                         // Forward/backward lean
    glRotatef(hipTilt, 0, 0, 1);                      // Side tilt (hip motion)
    glColor3f(0, 0, 1);                               // Blue torso
    torso.draw();
    glPopMatrix();

    // Draw left leg
    glPushMatrix();
    glTranslatef(-torso.width / 4, 0, 0);             // Offset from torso
    glColor3f(1, 0, 0);                               // Red left leg
    leftLeg.draw();
    glPopMatrix();

    // Draw right leg
    glPushMatrix();
    glTranslatef(torso.width / 4, 0, 0);              // Offset from torso
    glColor3f(0, 1, 0);                               // Green right leg
    rightLeg.draw();
    glPopMatrix();

    glPopMatrix();                                     // Restore world transformation
}

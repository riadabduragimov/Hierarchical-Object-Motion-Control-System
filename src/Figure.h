#pragma once
#include "stdafx.h"
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>

// Vec3 
// Simple 3D vector structure for positions and directions
struct Vec3 {
    float x, y, z;                                // Vector components

    // Constructor with default values (0,0,0)
    Vec3(float X = 0, float Y = 0, float Z = 0) : x(X), y(Y), z(Z) {}

    // Vector addition
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }

    // Vector subtraction
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }

    // Scalar multiplication
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }

    // Unary negation
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
};

// Scalar multiplication from left-hand side
inline Vec3 operator*(float s, const Vec3& v) { return Vec3(v.x * s, v.y * s, v.z * s); }

// Catmull-Rom interpolation
// Computes interpolated position along a Catmull-Rom spline given 4 control points
// t is the interpolation parameter in [0,1]
inline Vec3 catmullRom(const Vec3& P0, const Vec3& P1, const Vec3& P2, const Vec3& P3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;
    return 0.5f * (
        (2.0f * P1) +
        (-P0 + P2) * t +
        (2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3) * t2 +
        (-P0 + 3.0f * P1 - 3.0f * P2 + P3) * t3
        );
}

// Part
// Represents a single part of the articulated figure (torso or leg)
// Supports hierarchical drawing using a child pointer
struct Part {
    float length;          // Length of the part (Y-axis)
    float width;           // Width of the part (X/Z-axis)
    float angle;           // Local rotation angle (around Z-axis)
    Part* child;           // Pointer to child part (nullptr if no child)

    // Constructor with default length and width
    Part(float len = 1.0f, float w = 0.5f) : length(len), width(w), angle(0), child(nullptr) {}

    // Draws the part and recursively draws its child if any
    void draw();
};

// Figure 
// Represents the entire articulated figure: torso + left leg + right leg
class Figure {
public:
    Part torso;            // Torso part
    Part leftLeg;          // Left leg part
    Part rightLeg;         // Right leg part

    float time;            // Global time for animation
    float pathT;           // Parameter along the trajectory
    float speed;           // Movement speed along the path
    float angleY;          // Heading rotation of the figure
    Vec3 position;         // Current position in world space

    std::vector<Vec3> controlPoints;  // Control points defining the path
    float dt;                          // Path interpolation spacing

    // Torso sway / lean / hip tilt parameters for realistic walking
    float swayAmplitude;   // Lateral sway amplitude
    float swayFrequency;   // Sway oscillation frequency
    float leanAmplitude;   // Forward/backward lean
    float hipTiltAmplitude;// Side tilt during steps

    // Constructor initializes parts and default animation parameters
    Figure();

    // Loads part dimensions from external geometry files
    bool loadGeometry(const std::string& torsoFile,
        const std::string& leftLegFile,
        const std::string& rightLegFile);

    // Loads trajectory from a file (dt + sequence of control points)
    bool loadPath(const std::string& pathFile);

    // Updates the figure's position, orientation, and leg angles based on elapsed time
    void update(float deltaTime);

    // Draws the figure in its current pose with torso sway and leg motion
    void draw();
};

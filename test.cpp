#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "domino.h"
using namespace std;

vector<domino> world;

domino cursor;
bool st = false;

void drawString (char *s, float x, float y, float z) {
    void* font = GLUT_BITMAP_HELVETICA_18;
    glRasterPos3f(x, y, z);
    for (uint i=0; i<strlen(s); glutBitmapCharacter(font, s[i++]));
}

#define obj world[idx]
int floor_check (int idx) {

    const
    float h_2 = domino_height_2,
          y1 = obj.y1,
          y2 = obj.y2;

    if (y1 == y2 && y1 < GROUND) {
        sleep:
        obj.theta = 0;
        obj.y = GROUND;
        return 2;
    }
    if (y1 < y2 && y1 <= GROUND) {
        if (abs(y1-y2) < 1e-2) goto sleep;
        obj.y += (GROUND - y1);
        obj.omega -= domino_m*g*cos(obj.theta)*h_2/domino_I*dt;
        return -1;
    }
    if (y2 < y1 && y2 <= GROUND) {
        if (abs(y1-y2) < 1e-2) goto sleep;
        obj.y += (GROUND-y2);
        obj.omega += domino_m*g*cos(obj.theta)*h_2/domino_I*dt;
        return 1;
    }
    return 0;
}

bool intersect (int i, int j) {
    const
    float x1 = world[i].x1,
          y1 = world[i].y1,
          x2 = world[i].x2,
          y2 = world[i].y2,

          x3 = world[j].x1,
          y3 = world[j].y1,
          x4 = world[j].x2,
          y4 = world[j].y2;

    // rough check (this use!)
    if (x1 <= x2) {
        if (x2 <= x3 && x2 <= x4) return false;
        if (x1 >= x3 && x1 >= x4) return false;
    }
    else {
        if (x1 <= x3 && x1 <= x4) return false;
        if (x2 >= x3 && x2 >= x4) return false;
    }
    if (y1 <= y2) {
        if (y2 <= y3 && y2 <= y4) return false;
        if (y1 >= y3 && y1 >= y4) return false;
    }
    else {
        if (y1 <= y3 && y1 <= y4) return false;
        if (y2 >= y3 && y2 >= y4) return false;
    }
    if (y1 == y2 && y2 == y3 && y3 == y4)
        return false;

    const
    int c1 = ccw(x1,y1, x2,y2, x3,y3),
        c2 = ccw(x1,y1, x2,y2, x4,y4),
        c3 = ccw(x3,y3, x4,y4, x1,y1),
        c4 = ccw(x3,y3, x4,y4, x2,y2);

    return (c1*c2 < 0) && (c3*c4 < 0);
}

void cross_check (uint i) {
    for (uint j=0; j<world.size(); ++j) {
        if (i == j) continue;
        if (intersect(i, j)) {
            world[i].omega = (world[j].omega+world[i].omega)/2;
            world[j].omega = world[i].omega;
        }
    }
}

void update_endpoints (int idx) {
    const
    float c = domino_height_2 * cos(obj.theta),
          s = domino_height_2 * sin(obj.theta);
    obj.x1 = obj.x - c;
    obj.x2 = obj.x + c;
    obj.y1 = obj.y - s;
    obj.y2 = obj.y + s;
}

void move_world (void) {
    for (uint idx=0; idx<world.size(); ++idx) {
        int og = floor_check(idx);
        switch (og) {
        case 1:
        case -1:
            obj.vx = 0;
            obj.x +=
                og * domino_height_2
                * (cos(obj.theta) - cos(obj.theta + obj.omega*dt));
            break;
        default:
            break;
        }

        obj.x += obj.vx * dt;
        obj.y += obj.vy * dt;
        obj.vy -= g * dt;
        obj.theta += obj.omega * dt;
    }

    for (uint idx=0; idx<world.size(); ++idx)
        update_endpoints(idx);

    for (uint idx=0; idx<world.size(); ++idx)
        cross_check(idx);
}

// ------------------------------------------------------------------

void display (void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // cursor
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 0.0f, 0.0);
    glVertex2f(relX(cursor.x - domino_height_2*cos(cursor.theta)),
               relY(cursor.y - domino_height_2*sin(cursor.theta)));
    glVertex2f(relX(cursor.x + domino_height_2*cos(cursor.theta)),
               relY(cursor.y + domino_height_2*sin(cursor.theta)));
    glEnd();

    // dominos in world
    glColor3f(1.0f, 1.0f, 1.0);
    for (uint idx=0; idx<world.size(); ++idx) {
        glPointSize(3.0);
        glBegin(GL_POINTS);
        glVertex2f(relX(obj.x)-1, relY(obj.y)-1);
        glEnd();
        glBegin(GL_LINE_LOOP);
        update_endpoints(idx);
        glVertex2f(relX(obj.x1), relY(obj.y1));
        glVertex2f(relX(obj.x2), relY(obj.y2));
        glEnd();
    }

    //char str[100];
    //sprintf(str, "animation: %d", st);
    //drawString(str, -0.98, 0.82, 0);

    for (int i=0; st && i<300; ++i)
        move_world();

    // glFlush();
    glutSwapBuffers();
}

void redisplay (void) {
    glutPostRedisplay();
}

void animation_toggle (void) {
    st ^= true;
    glutIdleFunc(st ? redisplay : 0);
}

void init (void) {
    cursor.x = WIDTH / 2.0;
    cursor.y = GROUND + domino_height;
    cursor.theta = M_PI_2;
    cursor.vx = 0;
    cursor.vy = 0;
    cursor.omega = 0;
}

void keyboard (unsigned char key, int x, int y) {
    float step_x = 1.0;
    static int theta_i = 60;
    switch (key) {
        case 'q': exit(0);
        case ' ': world.push_back(cursor); break;
        case 'h': cursor.x -= step_x; break;
        case 'l': cursor.x += step_x; break;
        case 'H': cursor.x -= step_x * 5; break;
        case 'L': cursor.x += step_x * 5; break;
        case 'j': theta_i += 10; break;
        case 'J': theta_i += 2; break;
        case 'k': theta_i -= 10; break;
        case 'K': theta_i -= 2; break;
        case '@': animation_toggle(); break;
        case 'd': debug(); break;
    }
    if (cursor.x < 0) cursor.x = 0;
    if (cursor.x > WIDTH) cursor.x = WIDTH;
    if (theta_i < 0) theta_i = 0;
    if (theta_i > 120) theta_i = 120;
    cursor.theta = M_PI_2 * theta_i / 60;
    glutPostRedisplay();
}

int main (int argc, char*argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitDisplayMode(GLUT_RGBA);
    glutCreateWindow("domino");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}

void print_world (int i) {
    cout <<"world["<<i<<"] = {"<<endl;
    cout <<"\tx : " <<world[i].x
         <<", y : "<<world[i].y
         <<", theta: "
         <<world[i].theta<<endl;
    cout <<"\tvx: "<<world[i].vx
         <<", vy: "<<world[i].vy
         <<", omega : "<<world[i].omega<<endl;
    cout << "}"<<endl;
}

void debug (void) {
    return;
}

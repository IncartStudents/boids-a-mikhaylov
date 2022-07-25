#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <glut.h>
#include <time.h>

#define R 50
#define MAXV 5
#define FH 200
#define FW 200
#define SIZE 3
#define DT 0.5

typedef unsigned char byte;
byte Frame[FH][FW][3];
byte Frame2[FH][FW][3];

using namespace std;

vector<bird> Boids;
vector<bird> Boids2;

class bird {
public:
    int x, y, dx, dy;

    bird() {
        x = rand() % FW;
        y = rand() % FH;
        dx = rand() % MAXV;
        dy = rand() % MAXV;
    }

    vector<bird> Search(vector<bird> base) {
        vector<bird> res;
        for (int i = 0; i < base.size(); i++) {
            if (sqrt(pow((this->x - base[i].x), 2) + pow((this->y - base[i].y), 2)) <= R && (base[i].x != this->x || base[i].y != this->y)) {
                res.push_back(base[i]);
            }
        }
        return res;
    } 

    vector<int> Cohesion(vector<bird> searched) {
        vector<int> coh;
        int avg_x = 0;
        int avg_y = 0;
        if (searched.size() == 0) {
            return { 0, 0 };
        }
        for (int i = 0; i < searched.size(); i++) {
            avg_x += searched[i].x;
            avg_y += searched[i].y;
        }
        avg_x /= searched.size();
        avg_y /= searched.size();
        coh.push_back(avg_x - this->x);
        coh.push_back(avg_y - this->y);

        return coh;
    }

    vector<int> Separate(vector<bird> src) {
        vector<int> sep = {0, 0};
        
        if (src.size() == 0) {
            return { 0, 0 };
        }

        for (int i = 0; i < src.size(); i++) {
            int rx = src[i].x - this->x;
            int ry = src[i].y - this->y;
            double v = sqrt((double)pow(rx, 2) + (double)pow(ry, 2));
            int v1 = v;
            if (v1 == 0) {
                v1 = 1;
            }
            int p = 0 - (rx * R) / v1;
            int q = 0 - (ry * R) / v1;
            sep[0] += p;
            sep[1] += q;
        }

        return sep;
    }

    vector<int> Aligment(vector<bird> searched) {
        vector<int> ali = {0, 0};

        int avg_dx = 0;
        int avg_dy = 0;
        if (searched.size() == 0) {
            return { 0, 0 };
        }
        for (int i = 0; i < searched.size(); i++) {
            avg_dx += searched[i].dx;
            avg_dy += searched[i].dy;
        }
        avg_dx /= searched.size();
        avg_dy /= searched.size();
        ali[0] = avg_dx - this->dx;
        ali[1] = avg_dy - this->dy;
        return ali;
    }

    void NewValue(vector<bird> base) {
        vector<bird> src = Search(base);
        vector<int> sep = Separate(src);
        vector<int> ali = Aligment(src);
        vector<int> coh = Cohesion(src);

        vector<int> a = {0, 0};
        a[0] = sep[0] + ali[0] + coh[0];
        a[1] = sep[1] + ali[1] + coh[1];

        this->x = this->x + this->dx;
        this->y = this->y + this->dy;
        this->dx = this->dx + a[0];
        this->dy = this->dy + a[1];
    }
};

ofstream out("test_boids.txt");
/*vector<bird> Boids;
    vector<bird> Boids2;

    for (int i = 0; i < 10; i++) {
        bird abc;
        Boids.push_back(abc);
        Boids2.push_back(abc);
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < Boids.size(); j++) {
            out << Boids[j].x << " " << Boids[j].y << " " << Boids[j].dx << " " << Boids[j].dy << endl;
            Boids[j].NewValue(Boids2);
        }
        Boids2 = Boids;
    }*/

void Noize() {
    int x, y;
    for (y = 0; y < FH; y++)
        for (x = 0; x < FW; x++)
        {
            if (rand() % 10 == 0)
                Frame[y][x][1] = 255;
            else
                Frame[y][x][1] = 0;
        }
}

void NextFrame() {
    int x, y;
    for (y = 0; y < FH; y++)
        for (x = 0; x < FW; x++)
        {
            if (rand() % 4 == 0)
                Frame[y][x][1] = 255;
            else
                Frame[y][x][1] = 0;
        }
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2d(-1, 1);
    glPixelZoom(SIZE, -SIZE); //size

    glDrawPixels(FW, FH, GL_RGB, GL_UNSIGNED_BYTE, Frame);

    glFinish();
}

void Idle() {
    double dt;

    static long l_t = -1;

    if (l_t == -1)
        l_t = clock();
    dt = (double)(clock() - l_t) / CLOCKS_PER_SEC;
    if (dt > DT)
    {
        l_t = clock();
        printf("%f\n", dt);
        NextFrame();
        //memcpy(Frame, Frame2, sizeof(Frame));
        glutPostRedisplay();
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(FW * SIZE, FH * SIZE); //size
    glutInitWindowPosition(3, 3);
    glutCreateWindow("MY BOIDSSS");

    glutDisplayFunc(Display);
    glutIdleFunc(Idle);

//    Noize();

    glutMainLoop();

    return 0;
}

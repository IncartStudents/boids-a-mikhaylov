#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <glut.h>
#include <time.h>

#define R 50
#define MAXV 5
#define FH 500
#define FW 500
#define SIZE 1.5
#define DT 0.05

typedef unsigned char byte;
byte Frame[FH][FW][3];
byte Frame2[FH][FW][3];

using namespace std;

class bird {
    int x, y, dx, dy;
public:

    bird() {
        x = rand() % FW;
        y = rand() % FH;
        dx = rand() % MAXV;
        dy = rand() % MAXV;
    }

    int getX() {
        return this->x;
    }
    int getY() {
        return this->y;
    }
    int getDx() {
        return this->dx;
    }
    int getDy() {
        return this->dy;
    }

    void setX(int a) {
        if (a >= 0 && a < FW) {
            this->x = a;
        } else if (a >= FW) {
            this->x = a - FW;
        } else if (a < 0) {
            this->x = a + FW;
        }
    }
    void setY(int a) {
        if (a >= 0 && a < FH) {
            this->y = a;
        } else if (a >= FH) {
            this->y = a - FH;
        } else if (a < 0) {
            this->y = a + FH;
        }
    }
    void setDx(int a) {
        if (a > -MAXV && a < MAXV) {
            this->dx = a;
        } else if (a < -MAXV){
            this->dx = -MAXV;
        } else if (a > MAXV) {
            this->dx = MAXV;
        }
    }
    void setDy(int a) {
        if (a > -MAXV && a < MAXV) {
            this->dy = a;
        } else if (a < -MAXV) {
            this->dy = -MAXV;
        } else if (a > MAXV) {
            this->dy = MAXV;
        }
    }
    
    vector<bird> Search(vector<bird> base) {
        vector<bird> res;
        for (int i = 0; i < base.size(); i++) {
            for (int h = -1; h < 2; h++) {
                for (int w = -1; w < 2; w++) {
                    int x0 = base[i].x + FW * w;
                    int y0 = base[i].y + FH * h;
                    int r = sqrt(pow((this->x - x0), 2) + pow((this->y - y0), 2));
                    if (r <= R && (x0 != this->x || y0 != this->y)) {
                        res.push_back(base[i]);
                        break;
                    }
                }
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
            int rx = src[i].getX() - this->x;
            int ry = src[i].getY() - this->y;
            double v = sqrt((double)pow(rx, 2) + (double)pow(ry, 2));
            int v1 = v;
            if (v1 == 0) {
                v1 = 1;
            }
            int p = -((rx * R) / v1 - rx);
            int q = -((ry * R) / v1 - ry);
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
            avg_dx += searched[i].getDx();
            avg_dy += searched[i].getDy();
        }
        avg_dx /= searched.size();
        avg_dy /= searched.size();
        ali[0] = (this->getX() + avg_dx) - (this->getX() + this->getDx());
        ali[1] = (this->getY() + avg_dy) - (this->getY() + this->getDy());
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

        this->setX(this->x + this->dx);
        this->setY(this->y + this->dy);
        this->setDx(this->dx + a[0]);
        this->setDy(this->dy + a[1]);
    }
};

vector<bird> Boids;
vector<bird> Boids2;

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
    for (int i = 0; i < FW; i++) {
        for (int j = 0; j < FH; j++) {
            Frame[i][j][1] = 0;
        }
    }

    for (int j = 0; j < Boids.size(); j++) {
        //if (Boids[j].getX() > 0 && Boids[j].getX() < FW && Boids[j].getY() > 0 && Boids[j].getY() < FW) {
        Frame[Boids[j].getX()][Boids[j].getY()][1] = 255;
        //}
        //out << Boids[j].x << " " << Boids[j].y << " " << Boids[j].dx << " " << Boids[j].dy << endl;
        Boids[j].NewValue(Boids2);
    }
    Boids2 = Boids;
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
    for (int i = 0; i < 50; i++) {
        bird abc;
        Boids.push_back(abc);
        Boids2.push_back(abc);
    }

    glutMainLoop();

    return 0;
}

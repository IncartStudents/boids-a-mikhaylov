#include <iostream>
#include <vector>
#include <cmath>

#define R 50
#define FH 1000
#define FW 1000

using namespace std;

class bird {
public:
    int x, y, dx, dy;

    bird() {
        x = rand() % FW;
        y = rand() % FH;
        dx = rand() % 10;
        dy = rand() % 10;
    }

    vector<bird> Search(vector<bird> base) {
        vector<bird> res;
        for (int i = 0; i < base.size(); i++) {
            if (sqrt(pow((this->x - base[i].x), 2) + pow((this->y - base[i].y), 2)) <= R) {
                res.push_back(base[i]);
            }
        }
        return res;
    } 

    void Cohesion(vector<bird> searched) {
        int avg_x = 0;
        int avg_y = 0;
        for (int i = 0; i < searched.size(); i++) {
            avg_x += searched[i].x;
            avg_y += searched[i].y;
        }
        avg_x /= searched.size();
        avg_y /= searched.size();
        this->dx = avg_x - this->x;
        this->dy = avg_y - this->y;
    }
};

int main() {

}
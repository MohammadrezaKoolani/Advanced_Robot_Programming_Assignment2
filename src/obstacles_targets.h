#ifndef OBSTACLES_TARGETS_H
#define OBSTACLES_TARGETS_H

class Obstacle {
private:
    int x_, y_;
public:
    Obstacle(int x = 0, int y = 0) : x_(x), y_(y) {}
    int x() const { return x_; }
    int y() const { return y_; }
};

class Target {
private:
    int x_, y_, id_;
public:
    Target(int x = 0, int y = 0, int id = 0) : x_(x), y_(y), id_(id) {}
    int x() const { return x_; }
    int y() const { return y_; }
    int id() const { return id_; }
};

#endif // OBSTACLES_TARGETS_H

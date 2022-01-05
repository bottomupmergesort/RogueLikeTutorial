#include <vector>
#include <random>
#include <algorithm>
using std::vector;

class Rect {
    public:
    int x1;
    int x2;
    int y1;
    int y2;
    Rect(int x, int y, int w, int h);
    Rect() { }
};

Rect::Rect(int x, int y, int w, int h)
{
    x1 = x;
    x2 = x + w;
    y1 = y;
    y2 = y1 + h;
}

class node {
    public:
    const unsigned int MIN_LEAF_SIZE = 6;
    node* left;
    node* right;
    Rect* room;
    vector<Rect*> halls;
    int x;
    int y;
    int width;
    int height;
    node(int X, int Y, int Width, int Height)
    {
        x =X;
        y = Y;
        width = Width;
        height = Height;
        left = nullptr;
        right = nullptr;
    }
    node() { left = nullptr; right = nullptr; }
    bool split();
    void createRooms();
};

bool node::split()
{
    if (left != nullptr || right != nullptr)
    {
        return false;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    bool splitH = dist(gen) > 0.5;
    if (width > height && width / height >= 1.25)
       splitH = false;
    else if (height > width && height / width >= 1.25)
       splitH = true;

    int max = ((splitH) ? height:width) - MIN_LEAF_SIZE;
    if (max <= MIN_LEAF_SIZE)
       return false;

     std::uniform_int_distribution<int> splitran(MIN_LEAF_SIZE, max);
     int split =  splitran(gen);
     if (splitH)
     {
         left = new node(x, y, width, split);
         right = new node(x, y+split, width, height);
         std::cout<<"y+split: "<<y+split<<"\n";
     } else {
         left = new node(x, y, split, height);
         right = new node(x+split, y, width-split, height);
         std::cout<<"x+split: "<<x+split<<", width-split: "<<width-split<<"\n";
     }
     return true;
}

void node::createRooms()
{
    if (left != nullptr || right != nullptr)
    {
        if (left != nullptr)
          left->createRooms();
        if (right != nullptr)
           right->createRooms();
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> widthdist(3, width - 2);
        std::uniform_int_distribution<int> heightdist(3, height - 2);
        Point roomSize(widthdist(gen), heightdist(gen), false, ROCK);
        std::uniform_int_distribution<int> roomwidthdist(1, width-roomSize.x);
        std::uniform_int_distribution<int> roomheightdist(1,height-roomSize.y);
        Point roomPos(roomwidthdist(gen), roomheightdist(gen), false, ROCK);
        room = new Rect(x+roomPos.x, y+roomPos.y, roomSize.x, roomSize.y);
    }
}

//i use Point as a generic tile class
//you can choose to make a tile class
//that inherits from point, but i dont
//see the need.


#include <vector>
#include <random>
#include <algorithm>
using std::vector;
enum TERRAIN {
    GRASS,
    ROCK,
    WATER,
    TUNNEL,
    DIRT
};

class Point {
    public:
    int x; 
    int y;
    bool blocking;
    TERRAIN terrain;
    Point(int X, int Y, bool b, TERRAIN t) : x(X), y(Y), blocking(b), terrain(t) { }
    Point() { }
};

class Rect {
    public:
    int x1;
    int x2;
    int y1;
    int y2;
    int centx;
    int centy;
    Rect(int X1, int X2, int Y1, int Y2) : x1(X1), x2(X2), y1(Y1), y2(Y2) { centx = (x1 + x2) / 2; centy = (y1 + y2) / 2; }
    Rect() { }
};

class Map {
    public:
    vector<vector<Point>> board;
    int map_w;
    int map_h;
    Map(int mw, int mh);
    Map() { }
    bool inBounds(int x, int y);
    void Dig(int sx, int sy, int w, int h, TERRAIN terr);
    void render();
    void makeRooms(int numRooms);
    void tunnel(vector<Rect>& rooms);
};

Map::Map(int mw, int mh)
{
    map_w = mw;
    map_h = mh;
    board.resize(mh, vector<Point>(mw));
    for (int y = 0; y < mh; y++)
    {
        for (int x = 0; x < mw; x++)
        {
            board[y][x] = Point(x, y, true, ROCK);
        }
    }
}

//verifys a given coordinate is on the map
bool Map::inBounds(int x, int y)
{
    return x > 0 && x < map_w && y > 0 && y < map_h;
}

void Map::Dig(int x1, int y1, int x2, int y2, TERRAIN terr)
{
    int stopy = y2;
    int stopx = x2;
    if (y2 > map_h) stopy = map_h - 2;
    if (x2 > map_w) stopx = map_w - 2;
    for (int y = y1; y < stopy; y++)
    {
        for (int x = x1; x < stopx; x++)
        {
            board[y][x].blocking = false;
            board[y][x].terrain = terr;
        }
    }
}

bool overlaps(Rect room1, Rect room2)
{
    if (room1.x1 < room2.x2 && room1.x2 > room2.x1 &&
       room1.y1 < room2.y2 && room1.y2 > room2.y1)
         return true;
    return false;
}

void Map::makeRooms(int numRooms)
{
    int samplex = map_w / 4;
    int sampley = map_h / 4;
    Dig(samplex, sampley, 30, 15, GRASS);
    const unsigned int MAX_SIZE = 15;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> randRoomSize(6, MAX_SIZE);
    std::uniform_int_distribution<int> randRoomX(2, map_w - MAX_SIZE - 2);
    std::uniform_int_distribution<int> randRoomY(2, map_h - MAX_SIZE - 2);
    int x1, y1, x2, y2, roomWidth, roomHeight, roomSize;
    Rect room;
    std::vector<Rect> rooms;
    while (rooms.size() < numRooms)
    {
        roomSize = randRoomSize(gen);
        x1 = randRoomX(gen);
        y1 = randRoomY(gen);
        x2 = x1 + roomSize;
        y2 = y1 + roomSize;
        room = Rect(x1, x2, y1, y2);
        if (rooms.empty()) 
            rooms.push_back(room);
        else
        {
            bool overlap = false;
            for (auto r : rooms)
            {
                if (overlaps(r, room))
                {
                    overlap = true;
                    break;
                }
            }
             if (!overlap) {
                rooms.push_back(room);
             } else {
             }
        }   
    }
    for (auto r : rooms)
        Dig(r.x1, r.y1, r.x2, r.y2, GRASS);
    tunnel(rooms);
}

void Map::render()
{
    char terrsym;
    for (int y = 0; y < map_h; y++)
    {
        for (int x = 0; x < map_w; x++)
        {
            switch(board[y][x].terrain)
            {
                case ROCK:
                    terminal_color(color_from_name("grey"));  
                    terrsym = '#'; 
                    break;
                case GRASS:
                    terminal_color(color_from_name("darker green")); 
                    terrsym = ','; 
                    break;
                case DIRT:  
                    terminal_color(color_from_name("brown"));
                    terrsym = '.'; 
                    break;
                case WATER:
                    terminal_color(color_from_name("lighter blue")); 
                    terrsym = '~'; 
                    break;
                case TUNNEL: 
                    terminal_color(color_from_name("darker green"));
                    terrsym = ',';
                default: break;
            }
            terminal_put(x, y, terrsym);
        }
    }
}

void Map::tunnel(vector<Rect>& rooms)
{
  int ax,bx;
  int ay,by;
  Rect start;
  Rect fin;
  for (int r = 0; r < rooms.size() - 1; r++)
  {
   start = rooms[r];
   fin = rooms[r+1];
   std::cout<<"Room "<<r<<": "<<start.centx<<"/"<<start.centy<<" -> Room "<<r<<":" <<fin.centx<<"/"<<fin.centy<<"\n";
   if (start.centx <= fin.centx)
   {
      if (fin.centx < map_w && fin.centy < map_h) {
      for (ax = start.centx; ax <= (fin.centx + start.centx) / 2; ax++)
      {
	      board[start.centy][ax].blocking = false;
          board[start.centy][ax].terrain = TUNNEL;
          std::cout<<"going forwards from: ax/start"<<ax<<"/"<<start.centy<<"\n";
      }
      for (bx = fin.centx; bx >= (fin.centx + start.centx) / 2; bx--)
      {
        board[fin.centy][bx].blocking = false;
        board[fin.centy][bx].terrain = TUNNEL;
        std::cout<<"going backwards from: bx/fin"<<bx<<"/"<<fin.centy<<"\n";
      }
      }
   } else {
      if (fin.centx < map_w && fin.centy < map_h && start.centx > 0 && start.centy > 0)
      {
      for (ax = start.centx; ax >= (fin.centx + start.centx) / 2; ax--)
      {
        board[start.centy][ax].blocking = false;
        board[start.centy][ax].terrain = TUNNEL;
      }
      for (bx = fin.centx; bx <= (fin.centx + start.centx) / 2; bx++)
      {
        board[fin.centy][bx].blocking = false;
        board[fin.centy][bx].terrain = TUNNEL;
      }
      }
   }
   if (start.centy <= fin.centy)
   {
      if (fin.centx < map_w && fin.centy < map_h)
      {
      for (ay = start.centy; ay <  fin.centy; ay++)
      {
         board[ay][ax].blocking = false;
         board[ay][ax+1].blocking = false;
         board[ay][ax].terrain = TUNNEL;
         board[ay][ax+1].terrain = TUNNEL;
      }
      }
   } else {
      if (fin.centx < map_w && fin.centy < map_h && start.centx > 0 && start.centy > 0)
      {
      for (by = fin.centy; by <= start.centy; by++)
      {
        board[by][bx].blocking = false;
        board[by][bx-1].blocking = false;
        board[by][bx].terrain = TUNNEL;
        board[by][bx-1].terrain = TUNNEL;
      }
      }   
   }
  }
}
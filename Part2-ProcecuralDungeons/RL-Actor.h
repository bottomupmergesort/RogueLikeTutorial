class Actor {
    private:
    int x; 
    int y;
    double hp;
    char sym;
    public:
    Actor(int sx, int sy, char s);
    Actor() { }
    bool canWalk(int dx, int dy, Map& map);
    void move(int dx, int dy, Map& map);
    void render();
};

Actor::Actor(int sx, int sy, char s)
{
    x = sx;
    y = sy;
    sym = s;
    hp = 100.00;
}

//verifys a character can move to a coordinate
//cx = checkx, cy = checky
bool Actor::canWalk(int cx, int cy, Map& map)
{
    //if a point is on the map, and that point is not blocking, we can go there
    if (map.inBounds(cx, cy) && !map.board[cy][cx].blocking)
       return true;
    //else, no good.
    return false;
}

void Actor::move(int dx, int dy, Map& map)
{
    int nx = x + dx;
    int ny = y + dy;
    if (canWalk(nx, ny, map))
    {
        map.board[y][x].blocking = false;
        x = nx;
        y = ny;
        map.board[y][x].blocking = true; //we don't want two characters in the same spot.
    }
}

void Actor::render()
{
    terminal_color(color_from_name("purple"));
    terminal_put(x, y, sym);
}

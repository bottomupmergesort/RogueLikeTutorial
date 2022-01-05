
enum GAMESTATE {
    PLAYERTURN,
    AITURN
};

class Engine {
    private:
    int max_x;
    int max_y;
    Map world;
    Actor player;
    GAMESTATE state;
    public:
    Engine(int wx, int wy);
    void gameLoop();
    void render();
};

Engine::Engine(int wx, int wy)
{
    max_x = wx;
    max_y = wy;
    world = Map(max_x, max_y);
    player = Actor(max_x/2, max_y/2, '@');
    world.makeRooms(7);
    state = PLAYERTURN;
}

void Engine::gameLoop()
{
    char keypress;
    bool inplay = true;
    while (inplay)
    {
        if (state == PLAYERTURN && terminal_has_input())
        {
            if ((keypress = terminal_read()))
            {
                switch (keypress)
                {
                    case TK_UP: 
                        player.move(0, -1, world);
                        break;
                    case TK_DOWN:
                        player.move(0, 1, world);
                        break;
                    case TK_LEFT:
                        player.move(-1, 0, world);
                        break;
                    case TK_RIGHT:
                        player.move(1, 0, world);
                        break;
                    case TK_ESCAPE:
                        inplay = false;
                        break;
                    default:
                        break;
                }
            }
        }
        render();
    }
}

void Engine::render()
{
    terminal_clear();
    world.render();
    player.render();
    terminal_refresh();
}
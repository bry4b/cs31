// rabbits.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;               // max number of rows in the arena
const int MAXCOLS = 25;               // max number of columns in the arena
const int MAXRABBITS = 100;           // max number of rabbits allowed

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
// type name, since it's mentioned in the Rabbit declaration.

class Rabbit
{
public:
    // Constructor
    Rabbit(Arena* ap, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

    // Mutators
    void move();

private:
    Arena* m_arena;
    int    m_row;
    int    m_col;

    // TODO: You'll probably find that a rabbit object needs additional
    // data members to support your implementation of the behavior affected
    // by poisoned carrots.
    int m_carrots = 0;
    bool m_skip = false;
};

class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);

    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

    // Mutators
    string dropPoisonedCarrot();
    string move(int dir);
    void   setDead();

private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     rabbitCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRabbitsAt(int r, int c) const;
    void    display(string msg) const;

    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRabbit(int r, int c);
    bool addPlayer(int r, int c);
    void moveRabbits();

private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rabbit* m_rabbits[MAXRABBITS];
    int     m_nRabbits;
    int     m_turns;

    // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRabbits);
    ~Game();

    // Mutators
    void play();

private:
    Arena* m_arena;

    // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int calculateDanger(const Arena& a, int r, int c);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rabbit implementation
///////////////////////////////////////////////////////////////////////////

Rabbit::Rabbit(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A rabbit must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
    {
        cout << "***** Rabbit created with invalid coordinates (" << r << ","
            << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
}

int Rabbit::row() const
{
    return m_row;
}

int Rabbit::col() const
{
    return m_col;
}

bool Rabbit::isDead() const
{
    return m_carrots == 2;
}

void Rabbit::move()
{
    //   Return without moving if the rabbit has eaten one poisoned
    //   carrot (so is supposed to move only every other turn) and
    //   this is a turn it does not move.
    if (m_carrots == 1)
    {
        if (m_skip == true)
        {
            m_skip = false;
            return;
        }
        else
            m_skip = true;
    }

    //   Otherwise, attempt to move in a random direction; if can't
    //   move, don't move.  If it lands on a poisoned carrot, eat the
    //   carrot and remove it from the game (so it is no longer on that
    //   grid point).

    int rand_dir = randInt(0, NUMDIRS - 1);
    if (attemptMove(*m_arena, rand_dir, m_row, m_col))
    {
        if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        {
            m_carrots++;
            if (m_carrots == 1)
                m_skip = true;
            m_arena->setCellStatus(m_row, m_col, EMPTY);
        }
    }

}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
            << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonedCarrot()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poisoned carrot at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poisoned carrot has been dropped.";
}

string Player::move(int dir)
{
    //        Attempt to move the player one step in the indicated
    //        direction.  If this fails,
    //        return "Player couldn't move; player stands."
    //        A player who moves onto a rabbit, and this
    //        returns "Player walked into a rabbit and died."
    //        Otherwise, return one of "Player moved north.",
    //        "Player moved east.", "Player moved south.", or
    //        "Player moved west."

    if (!attemptMove(*m_arena, dir, m_row, m_col))
        return "Player couldn't move; player stands.";
    else if (m_arena->numberOfRabbitsAt(m_row, m_col) > 0)
    {
        setDead();
        return "Player walked into a rabbit and died.";
    }
    else
    {
        switch (dir)
        {
        case NORTH:
            return "Player moved north.";
            break;
        case EAST:
            return "Player moved east.";
            break;
        case SOUTH:
            return "Player moved south.";
            break;
        case WEST:
            return "Player moved west.";
            break;
        }
        return "Player couldn't move; player stands.";
    }           
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;  //fatality
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
            << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRabbits = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    delete m_player;
    for (int i = 0; i < rabbitCount(); i++)
        delete m_rabbits[i];
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::rabbitCount() const
{
    return m_nRabbits;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r - 1][c - 1];
}

int Arena::numberOfRabbitsAt(int r, int c) const
{
    int count = 0;
    for (int i = 0; i < rabbitCount(); i++)     //search through m_rabbits[] for Rabbit with matching r, c
        if (m_rabbits[i]->row() == r && m_rabbits[i]->col() == c)
            count++;

    return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill displayGrid with dots (empty) and stars (poisoned carrots)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

    // Indicate each rabbit's position
    for (r = 1 ; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
        {
            if (numberOfRabbitsAt(r, c) == 1)
                displayGrid[r - 1][c - 1] = 'R';
            if (numberOfRabbitsAt(r, c) <= 8 && numberOfRabbitsAt(r, c) >= 2)
                displayGrid[r - 1][c - 1] = ('0' + numberOfRabbitsAt(r, c));
            if (numberOfRabbitsAt(r, c) >= 9)
                displayGrid[r - 1][c - 1] = '9';
        }

    }

    // Indicate the player's position
    if (m_player != nullptr)
        displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

    // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r - 1][c - 1];
        cout << endl;
    }
    cout << endl;

    // Write message, rabbit, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << rabbitCount() << " rabbits remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r - 1][c - 1] = status;
}

bool Arena::addRabbit(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a rabbit on a spot with a poisoned carrot
    if (getCellStatus(r, c) != EMPTY)
        return false;

    // Don't add a rabbit on a spot with a player
    if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
        return false;

    // If there are MAXRABBITS existing rabbits, return false.  Otherwise,
    // dynamically allocate a new rabbit at coordinates (r,c).  Save the
    // pointer to newly allocated rabbit and return true.
    if (rabbitCount() == MAXRABBITS)
        return false;
    else    //spawn rabbit
    {
        m_rabbits[m_nRabbits] = new Rabbit(this, r, c);     
        m_nRabbits++;
        return true;
    }
}

bool Arena::addPlayer(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Don't add a player on a spot with a rabbit
    if (numberOfRabbitsAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveRabbits()
{
    // Move all rabbits
    // Move each rabbit.  Mark the player as dead if necessary.
    // Deallocate any dead dynamically allocated rabbit.
    for (int i = 0; i < rabbitCount(); i++)
    {
        m_rabbits[i]->move();

        if (numberOfRabbitsAt(m_player->row(), m_player->col()) > 0)   //player dies from rabbit walking into it (L + bozo + ratio)
            m_player->setDead();

        if (m_rabbits[i]->isDead())    //rabbit dies from eating second carrot (L + ratio)
        {
            delete m_rabbits[i];
            for (int j = i; j < rabbitCount() - 1; j++)
                m_rabbits[j] = m_rabbits[j + 1];
            m_nRabbits--;
        }
    }

    // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (!isPosInBounds(r, c))
    {
        cout << "***** " << "Invalid arena position (" << r << ","
            << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRabbits)
{
    if (nRabbits < 0)
    {
        cout << "***** Cannot create Game with negative number of rabbits!" << endl;
        exit(1);
    }
    if (nRabbits > MAXRABBITS)
    {
        cout << "***** Trying to create Game with " << nRabbits
            << " rabbits; only " << MAXRABBITS << " are allowed!" << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nRabbits - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
            << cols << " arena, which is too small too hold a player and "
            << nRabbits << " rabbits!" << endl;
        exit(1);
    }

    // Create arena
    m_arena = new Arena(rows, cols);

    // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

    // Populate with rabbits
    while (nRabbits > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRabbit(r, c);
        nRabbits--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/c or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonedCarrot();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'c')
                return player->dropPoisonedCarrot();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/c." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while (!player->isDead() && m_arena->rabbitCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        m_arena->moveRabbits();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
    default:  return false;
    case 'n': dir = NORTH; break;
    case 'e': dir = EAST;  break;
    case 's': dir = SOUTH; break;
    case 'w': dir = WEST;  break;
    }
    return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    switch (dir)
    {
    case NORTH:
        if (r - 1 >= 1)
        {
            r--;
            return true;
        }
        break;
    case EAST:
        if (c + 1 <= a.cols())
        {
            c++;
            return true;
        }
        break;
    case SOUTH:
        if (r + 1 <= a.rows())
        {
            r++;
            return true;
        }
        break;
    case WEST:
        if (c - 1 >= 1)
        {
            c--;
            return true;
        }
        break;
    }
    return false;
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poisoned carrot and
// not move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    // Your replacement implementation should do something intelligent.
    // You don't have to be any smarter than the following, although
    // you can if you want to be:  If staying put runs the risk of a
    // rabbit possibly moving onto the player's location when the rabbits
    // move, yet moving in a particular direction puts the player in a
    // position that is safe when the rabbits move, then the chosen
    // action is to move to a safer location.  Similarly, if staying put
    // is safe, but moving in certain directions puts the player in
    // danger of dying when the rabbits move, then the chosen action should
    // not be to move in one of the dangerous directions; instead, the player
    // should stay put or move to another safe position.  In general, a
    // position that may be moved to by many rabbits is more dangerous than
    // one that may be moved to by few.
    //
    // Unless you want to, you do not have to take into account that a
    // rabbit might be poisoned and thus sometimes less dangerous than one
    // that is not.  That requires a more sophisticated analysis that
    // we're not asking you to do.

    double danger;
    double min_danger = MAXRABBITS;
    // assign a danger level to each square in N/E/S/W directions and current position X
    // player moves in direction of square with lowest danger value
    // if X has lowest danger value drop poisoned carrot

    if (r - 1 >= 1)     //N square
    {
        danger = calculateDanger(a, r - 1, c);
        if (a.getCellStatus(r - 1, c) == HAS_POISON)
            danger += 0.5;
        if (danger < min_danger)
        {
            bestDir = NORTH;
            min_danger = danger;
        }
    }

    if (c + 1 <= a.cols())  //E square
    {
        danger = calculateDanger(a, r, c + 1);
        if (a.getCellStatus(r, c + 1) == HAS_POISON)
        if (a.getCellStatus(r, c + 1) == HAS_POISON)
            danger += 0.5;
        if (danger < min_danger)
        {
            bestDir = EAST;
            min_danger = danger;
        }
    }

    if (r + 1 <= a.rows())  //S square
    {
        danger = calculateDanger(a, r + 1, c);
        if (a.getCellStatus(r + 1, c) == HAS_POISON)
            danger += 0.5;
        if (danger < min_danger)
        {
            bestDir = SOUTH;
            min_danger = danger;
        }
    }

    if (c - 1 >= 1)     //W square
    {
        danger = calculateDanger(a, r, c - 1);
        if (a.getCellStatus(r, c - 1) == HAS_POISON)
            danger += 0.5;
        if (danger < min_danger)
        {
            bestDir = WEST;
            min_danger = danger;
        }
    }

    //X square
    danger = calculateDanger(a, r, c);
    if (a.getCellStatus(r, c) != HAS_POISON)
        danger -= 0.5;
    else
        danger += 0.5;
    if (danger < min_danger)
        return false;

    return true;
}

int calculateDanger(const Arena& a, int r, int c)
{
    int danger = 0; 
    danger += a.numberOfRabbitsAt(r - 1, c);
    danger += a.numberOfRabbitsAt(r, c - 1);
    danger += a.numberOfRabbitsAt(r + 1, c);
    danger += a.numberOfRabbitsAt(r, c + 1);
    if (a.numberOfRabbitsAt(r, c) > 0)
        danger += MAXRABBITS;
    return danger;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////


void doBasicTests();

int main()
{
    //basic testing
    //doBasicTests(); // Remove this line after completing test.
    //return 0;       // Remove this line after completing test.

    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);
    //Game g(3, 5, 2);

    // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _WIN32

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not _WIN32

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr || strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif


/*
#include <type_traits>
#include <cassert>

#define CHECKTYPE(c, f, r, a)  \
    static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
       "FAILED: You changed the type of " #c "::" #f);  \
    { [[gnu::unused]] auto p = static_cast<r (c::*) a>(&c::f); }

void thisFunctionWillNeverBeCalled()
{
      // If the student deleted or changed the interfaces to the public
      // functions, this won't compile.  (This uses magic beyond the scope
      // of CS 31.)

    Rabbit r(static_cast<Arena*>(0), 1, 1);
    CHECKTYPE(Rabbit, row, int, () const);
    CHECKTYPE(Rabbit, col, int, () const);
    CHECKTYPE(Rabbit, isDead, bool, () const);
    CHECKTYPE(Rabbit, move, void, ());

    Player p(static_cast<Arena*>(0), 1, 1);
    CHECKTYPE(Player, row, int, () const);
    CHECKTYPE(Player, col, int, () const);
    CHECKTYPE(Player, isDead, bool, () const);
    CHECKTYPE(Player, dropPoisonedCarrot, string, ());
    CHECKTYPE(Player, move, string, (int));
    CHECKTYPE(Player, setDead, void, ());

    Arena a(1, 1);
    CHECKTYPE(Arena, rows, int, () const);
    CHECKTYPE(Arena, cols, int, () const);
    CHECKTYPE(Arena, player, Player*, () const);
    CHECKTYPE(Arena, rabbitCount, int, () const);
    CHECKTYPE(Arena, getCellStatus, int, (int,int) const);
    CHECKTYPE(Arena, numberOfRabbitsAt, int, (int,int) const);
    CHECKTYPE(Arena, display, void, (string) const);
    CHECKTYPE(Arena, setCellStatus, void, (int,int,int));
    CHECKTYPE(Arena, addRabbit, bool, (int,int));
    CHECKTYPE(Arena, addPlayer, bool, (int,int));
    CHECKTYPE(Arena, moveRabbits, void, ());

    Game g(1,1,1);
    CHECKTYPE(Game, play, void, ());
}

void findTheRabbit(const Arena& a, int& r, int& c)
{
    if      (a.numberOfRabbitsAt(r-1, c) == 1) r--;
    else if (a.numberOfRabbitsAt(r+1, c) == 1) r++;
    else if (a.numberOfRabbitsAt(r, c-1) == 1) c--;
    else if (a.numberOfRabbitsAt(r, c+1) == 1) c++;
    else assert(false);
}

void surroundWithPoison(Arena& a, int r, int c)
{
    a.setCellStatus(r-1, c, HAS_POISON);
    a.setCellStatus(r+1, c, HAS_POISON);
    a.setCellStatus(r, c-1, HAS_POISON);
    a.setCellStatus(r, c+1, HAS_POISON);
}

void doBasicTests()
{
    {
        Arena a(10, 20);
        assert(a.addPlayer(2, 5));
        Player* pp = a.player();
        assert(pp->row() == 2  &&  pp->col() == 5  && ! pp->isDead());
        assert(pp->move(NORTH) == "Player moved north.");
        assert(pp->row() == 1  &&  pp->col() == 5  && ! pp->isDead());
        assert(pp->move(NORTH) == "Player couldn't move; player stands.");
        assert(pp->row() == 1  &&  pp->col() == 5  && ! pp->isDead());
        pp->setDead();
        assert(pp->row() == 1  &&  pp->col() == 5  && pp->isDead());
    }
    {
        Arena a(10, 20);
        int r = 8;
        int c = 18;
        assert(a.addPlayer(r, c));
        for (int k = 0; k < MAXRABBITS/4; k++)
        {
            assert(a.addRabbit(r-1, c));
            assert(a.addRabbit(r+1, c));
            assert(a.addRabbit(r, c-1));
            assert(a.addRabbit(r, c+1));
        }
        assert(! a.player()->isDead());
        a.moveRabbits();
        assert(a.player()->isDead());
    }
    {
        Arena a(10, 20);
        int r = 4;
        int c = 4;
        assert(a.addRabbit(r, c));
        surroundWithPoison(a, r, c);
        assert(a.addPlayer(8, 18));
        assert(a.rabbitCount() == 1  &&  a.numberOfRabbitsAt(r, c) == 1);
        a.moveRabbits();
        assert(a.rabbitCount() == 1  &&  a.numberOfRabbitsAt(r, c) == 0);
        findTheRabbit(a, r, c);
        assert(a.getCellStatus(r, c) != HAS_POISON);
        a.moveRabbits();
        assert(a.rabbitCount() == 1  &&  a.numberOfRabbitsAt(r, c) == 1);
        a.moveRabbits();
        assert(a.rabbitCount() == 1  &&  a.numberOfRabbitsAt(r, c) == 0);
        findTheRabbit(a, r, c);
        a.moveRabbits();
        assert(a.rabbitCount() == 1  &&  a.numberOfRabbitsAt(r, c) == 1);
        surroundWithPoison(a, r, c);
        a.moveRabbits();
        assert(a.rabbitCount() == 0  &&  a.numberOfRabbitsAt(r, c) == 0);
        assert(a.numberOfRabbitsAt(r-1, c) == 0);
        assert(a.numberOfRabbitsAt(r+1, c) == 0);
        assert(a.numberOfRabbitsAt(r, c-1) == 0);
        assert(a.numberOfRabbitsAt(r, c+1) == 0);
    }
    {
        Arena a(20, 20);
        assert(a.addPlayer(1, 20));
        struct Coord
        {
            int r;
            int c;
        };
        assert(MAXRABBITS == 100);
        const int NDOOMED = 4;
        Coord doomed[NDOOMED];
        for (int k = 0; k < NDOOMED; k++)
        {
            doomed[k].r = 3;
            doomed[k].c = 5*k+3;
            assert(a.addRabbit(doomed[k].r, doomed[k].c));
            surroundWithPoison(a, doomed[k].r, doomed[k].c);
            for (int i = 0; i < MAXRABBITS/NDOOMED - 1; i++)
                assert(a.addRabbit(20, 20));
        }
        assert(!a.addRabbit(20, 20));
        assert(a.rabbitCount() == MAXRABBITS);
        a.moveRabbits();
        assert(a.rabbitCount() == MAXRABBITS);
        for (int k = 0; k < NDOOMED; k++)
        {
            findTheRabbit(a, doomed[k].r, doomed[k].c);
            surroundWithPoison(a, doomed[k].r, doomed[k].c);
        }
        a.moveRabbits();
        assert(a.rabbitCount() == MAXRABBITS);
        a.moveRabbits();
        assert(a.rabbitCount() == MAXRABBITS - NDOOMED);
        for (int k = 0; k < NDOOMED; k++)
            assert(a.addRabbit(20, 20));
        assert(!a.addRabbit(20, 20));
          // If the program crashes after leaving this compound statement, you
          // are probably messing something up when you delete a rabbit after
          // it dies (or you have mis-coded the destructor).
          //
          // Draw a picture of your m_rabbits array before the rabbits move,
          // and also note the values of m_nRabbits or any other variable you
          // might have that's involved with the number of rabbits.  Trace
          // through your code step by step as the rabbits move and die,
          // updating the picture according to what the code says, not what
          // you want it to do.  If you don't see a problem then, try tracing
          // through the destruction of the arena.
          //
          // If you execute the code, use the debugger to check on the values
          // of key variables at various points.  If you didn't try to learn
          // to use the debugger, insert statements that write the values of
          // key variables to cerr so you can trace the execution of your code
          // and see the first place where something has gone amiss.
    }
    cout << "Passed all basic tests (as long as when run under g31 there is no further message after this)." << endl;
}
*/
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
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
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
    bool   m_dead;
    bool   m_poisoned;
    bool   m_hasMoved;
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
void clearScreen();
int dangerLvl(const Arena& a, int r, int c);
bool attempt(const Arena& a, int dir, int& r, int& c);


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
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Rabbit created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_poisoned=false;
    m_hasMoved=false;
    m_dead = false;
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
    return m_dead;
}

void Rabbit::move()
{
    int dir = randInt(0, NUMDIRS-1);
    if(m_poisoned&&m_hasMoved){
        m_hasMoved=false;
        return;
    }
    //Moves rabbit if the random move goes to a valid position
    if(dir==NORTH){
        if(row()-1>0)m_row-=1;
    }
    else if(dir==EAST){
        if(col()+1<=m_arena->cols())m_col+=1;
    }
    else if(dir==SOUTH){
        if(row()+1<=m_arena->rows())m_row+=1;
    }
    else if(dir==WEST)
        if(col()-1>0)m_col-=1;
    
    //Kills rabbit if it eats two poisoned carrots, but also kills player if rabbit lands on player
    if(m_arena->getCellStatus(row(),col())==HAS_POISON){
        m_arena->setCellStatus(row(), col(), EMPTY);
        if(m_poisoned){
            m_dead=true;
            if(m_row==m_arena->player()->row()&&m_col==m_arena->player()->col()){
                m_arena->player()->setDead();
            }
        }
        else m_poisoned=true;
    }
    m_hasMoved = true;
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
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
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
    //Moves player if moving into a valid location, otherwise stay stil
    if(attemptMove(*m_arena, dir, m_row, m_col)){
        if(dir==NORTH){
            if(m_arena->numberOfRabbitsAt(m_row, m_col)!=0){
                m_dead=true;
                return "Player walked into a rabbit and died.";
            }
            return "Player moved north.";
        }
        else if(dir==EAST){
            if(m_arena->numberOfRabbitsAt(m_row, m_col)!=0){
                m_dead=true;
                return "Player walked into a rabbit and died.";
            }
            return "Player moved east.";
        }
        else if(dir==SOUTH){
            if(m_arena->numberOfRabbitsAt(m_row, m_col)!=0){
                m_dead=true;
                return "Player walked into a rabbit and died.";
            }
            return "Player moved south.";
        }
        else{
            if(m_arena->numberOfRabbitsAt(m_row, m_col)!=0){
                m_dead=true;
                return "Player walked into a rabbit and died.";
            }
            return "Player moved west.";
        }
    }
    return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
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
    for(int i=0;i<m_nRabbits;i++){
        delete m_rabbits[i];
    }
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
    return m_grid[r-1][c-1];
}

int Arena::numberOfRabbitsAt(int r, int c) const
{
    int nRabbits = 0;
    for(int i=0;i<m_nRabbits;i++)
        if(m_rabbits[i]->row()==r&&m_rabbits[i]->col()==c) nRabbits++;
    return nRabbits;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill displayGrid with dots (empty) and stars (poisoned carrots)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');

    for (r = 1; r <= rows(); r++){
        for (c = 1; c <= cols(); c++){
            if(numberOfRabbitsAt(r,c)==1)displayGrid[r-1][c-1]='R';
            else if(numberOfRabbitsAt(r,c)>=9)displayGrid[r-1][c-1]='9';
            else if(numberOfRabbitsAt(r,c)>=2) displayGrid[r-1][c-1]=(char)('0'+numberOfRabbitsAt(r,c));
        }
    }
      // Indicate the player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, rabbit, and player info
    if (msg != "")
        cout << msg << endl;
    if(rabbitCount()==1) cout << "There is " << rabbitCount() << " rabbit remaining." << endl;
    else cout << "There are " << rabbitCount() << " rabbits remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r-1][c-1] = status;
}

bool Arena::addRabbit(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a rabbit on a spot with a poisoned carrot
    if (getCellStatus(r, c) != EMPTY)
        return false;

      // Don't add a rabbit on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;

      // If there are MAXRABBITS existing rabbits, return false.  Otherwise,
      // dynamically allocate a new rabbit at coordinates (r,c).  Save the
      // pointer to newly allocated rabbit and return true.
    if(m_nRabbits==MAXRABBITS) return false;
    m_rabbits[m_nRabbits] = new Rabbit(this, r, c);
    m_nRabbits++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
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
    for(int i=0;i<m_nRabbits;i++){
        m_rabbits[i]->move();
        if(m_rabbits[i]->row()==m_player->row()&&m_rabbits[i]->col()==m_player->col()){
            m_player->setDead();
        }
        while(m_rabbits[i]->isDead()){
            delete m_rabbits[i];
            m_nRabbits--;
            if(i<m_nRabbits)
                m_rabbits[i]=m_rabbits[m_nRabbits];
            else break;
        }
    }
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
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
            else if (decodeDirection(playerMove[0], dir)){
                return player->move(dir);
            }
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
    while ( ! player->isDead()  &&  m_arena->rabbitCount() > 0)
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
    if(dir==NORTH&&r-1>0){
        r-=1;
        return true;
    }
    else if(dir==EAST&&c+1<=a.cols()){
        c+=1;
        return true;
    }
    else if(dir==SOUTH&&r+1<=a.rows()){
        r+=1;
        return true;
    }
    else if(dir==WEST&&c-1>0){
        c-=1;
        return true;
    }
    return false;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a poisoned carrot and
  // not move; otherwise, this function sets bestDir to the recommended
  // direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    int dangerArr[NUMDIRS] = {};

    if(dangerLvl(a,r,c)==0) return false; //If there is no harm in staying still, stay still
    else{
        //Calculate how "dangerous" moving to a spot is
        for(int i=0;i<NUMDIRS;i++){
            if(attempt(a, i, r, c)){
                int r2=0;
                int c2=0;
                if(i==0)r2=-1;
                else if(i==1)c2=1;
                else if(i==2)r2=1;
                else c2=-1;
                if(a.numberOfRabbitsAt(r+r2,c+c2)>0) dangerArr[i]=-1;
                else dangerArr[i]=dangerLvl(a,r+r2,c+c2);
            }
            else dangerArr[i]=-1;
        }
        int min = -1;
        for(int i=0;i<NUMDIRS;i++){
            if(dangerArr[i]!=-1){
                min = dangerArr[i];
                break;
            }
        }
        if(min==-1) return false;
        for(int i=0;i<NUMDIRS;i++){
            if(dangerArr[i]<=min&&dangerArr[i]!=-1){
                min = dangerArr[i]%10;
                bestDir = i;
            }
        }
        for(int i=0;i<NUMDIRS;i++){
            if(dangerArr[i]%10==min){
                if(dangerArr[i]!=dangerArr[bestDir]&&dangerArr[i]<dangerArr[bestDir]){
                    bestDir = i;
                }
            }
        }
        return true;
    }
    return false;

}
//Calculates how "dangerous" the tiles adjacent to the player are and how "dangerous" it would be for the player to stay still
int dangerLvl(const Arena& a, int r, int c){
    int danger=0;
    if(attempt(a, NORTH, r, c)){
        if(a.numberOfRabbitsAt(r-1, c)>0){
            danger+=a.numberOfRabbitsAt(r-1, c)*10; //Multiply by 10 so that in the case of two equally dangerous tiles (there are rabbits at both tiles) program can calculate which tile is more likely to have a rabbit move there, based on the number of rabbits surrounding the checked location
            danger++;
        }
    }
   if(attempt(a, EAST, r, c)){
        if(a.numberOfRabbitsAt(r, c+1)>0){
            danger+=a.numberOfRabbitsAt(r, c+1)*10;
            danger++;
        }
    }
    if(attempt(a, SOUTH, r, c)){
        if(a.numberOfRabbitsAt(r+1, c)>0){
            danger+=a.numberOfRabbitsAt(r+1, c)*10;
            danger++;
        }
    }
    if(attempt(a, WEST, r, c)){
        if(a.numberOfRabbitsAt(r, c-1)>0){
            danger+=a.numberOfRabbitsAt(r, c-1)*10;
            danger++;
        }
    }
    return danger;
}
bool attempt(const Arena& a, int dir, int& r, int& c){
    if(dir==NORTH&&r-1>0)
        return true;
    else if(dir==EAST&&c+1<=a.cols())
        return true;
    else if(dir==SOUTH&&r+1<=a.rows())
        return true;
    else if(dir==WEST&&c-1>0)
        return true;
    
    return false;
}


///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////
//void doBasicTests();
int main()
{
//    doBasicTests(); // Remove this line after completing test.
//    return 0;       // Remove this line after completing test.
      // Create a game
      // Use this instead to create a mini-game:
    //Game g(3, 5, 2);
    Game g(10, 12, 20);
      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

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
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif

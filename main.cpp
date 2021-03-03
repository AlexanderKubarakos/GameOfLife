#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <stdio.h>

/*
 * The Game of Life
 * 2021-May-02
 * I know the code is bad, tell me about it right?
 * And I need a linter, this has 3 different types of indentations
 * as well as variable names that not even Einstein would get
 */

class GameOfLife : public olc::PixelGameEngine
{
public:
    GameOfLife()
    {
        sAppName = "Game Of Life";
    }
private:

    // We could just use arrays of bools couldn't we?!
    struct sCell
    {
        bool alive = false;
    };

    // Is the simulation active?
    bool activeSim = false;

    sCell* world;
    sCell* bufferWorld;
    int nWorldWidth = 80;
    int nWorldHeight = 60;

    float fTargetFrameTime = 1.0f / 3.0f; // Virtual FPS of 100fps
    float fAccumulatedTime = 0.0f;

public:
    bool OnUserCreate() override
    {
        fAccumulatedTime = 0.0f;
        world = new sCell[nWorldHeight*nWorldWidth];
        bufferWorld = new sCell[nWorldHeight*nWorldWidth];
        return true;
    }
    bool OnUserUpdate(float fElapsedTime) override
    {

        fAccumulatedTime += fElapsedTime;

        float fBlockWidth = 16.0f;
        float fSourceX = GetMouseX();
        float fSourceY = GetMouseY();

        if (GetKey(olc::SPACE).bPressed)
        {
            activeSim = !activeSim;
        }

        if (!activeSim)
        {
            if (GetMouse(0).bReleased)
            {
                // i = y * width + x
                // makes 2d cord into 1d index
                int i = ((int)fSourceY / (int)fBlockWidth) * nWorldWidth + ((int)fSourceX / (int)fBlockWidth);
                world[i].alive = !world[i].alive;
            }
            if (GetKey(olc::Key::A).bPressed)
            {
                std::cout << "\n";
                for (int y = 0; y < nWorldHeight; y++) {
                    std::cout << "\n";
                    for (int x = 0; x < nWorldWidth; x++) {
                        std::cout << world[y * nWorldWidth + x].alive;
                    }
                }
            }
        }
        else if (fAccumulatedTime >= fTargetFrameTime)
        {
            fAccumulatedTime -= fTargetFrameTime;
            fElapsedTime = fTargetFrameTime;
            // Makes our buffer world a copy of the main world
            for (int i = 0, n = nWorldHeight*nWorldWidth; i < n; i++)
            {
                bufferWorld[i] = world[i];
            }

            for (int y = 0; y < nWorldHeight; y++) {
                for (int x = 0; x < nWorldWidth; x++) {

                    int neighbors = 0;

                    int i = y * nWorldWidth + x;
                    int collumID = i % nWorldWidth;

                    // i prefix means index of -

                    int iNorth = i - nWorldWidth;
                    int iSouth = i + nWorldWidth;
                    bool hasNorth = iNorth >= 0; // If the north neighbor id is an actual index
                    bool hasSouth = iSouth <= nWorldWidth * nWorldHeight;

                    int iEast = i + 1; // Used later
                    int iWest = i - 1;

                    bool hasEast = collumID < nWorldWidth - 1; // We have an east neighbor as we are not at the left most collum
                    bool hasWest = collumID > 0;

                    bool hasNorthEast = hasNorth && hasEast;
                    bool hasNorthWest = hasNorth && hasWest;
                    bool hasSouthEast = hasSouth && hasEast;
                    bool hasSouthWest = hasSouth && hasWest;

                    // By far the worst piece of code I have writen.. I think.. It might not be too bad??

                    if (hasNorth) {
                        if (bufferWorld[iNorth].alive)
                            neighbors++;
                        if (hasNorthEast) {
                            if (bufferWorld[iNorth + 1].alive)
                                neighbors++;
                        }
                        if (hasNorthWest) {
                            if (bufferWorld[iNorth - 1].alive)
                                neighbors++;
                        }
                    }

                    if (hasSouth) {
                        if (bufferWorld[iSouth].alive)
                            neighbors++;
                        if (hasSouthEast) {
                            if (bufferWorld[iSouth + 1].alive)
                                neighbors++;
                        }
                        if (hasSouthWest) {
                            if (bufferWorld[iSouth - 1].alive)
                                neighbors++;
                        }
                    }

                    if (hasEast) {
                        if (bufferWorld[iEast].alive)
                            neighbors++;
                    }

                    if (hasWest) {
                        if (bufferWorld[iWest].alive)
                            neighbors++;
                    }

                    // Use buffer for calculations and write to normal world

                    if (neighbors == 3)
                    {
                        world[y * nWorldWidth + x].alive = true;
                    }
                    else if (neighbors == 2 && bufferWorld[y * nWorldWidth + x].alive)
                    {
                        // if the cell is does not have 2 or 3 neighbors it should die, any cell with 2 neighbors need not change
                        world[y * nWorldWidth + x].alive = true;
                    }
                    else
                    {
                        world[y * nWorldWidth + x].alive = false;
                    }
                }
            }
        }

        Clear(olc::BLACK);

        DrawString({0,0},activeSim ? "Active" : "Inactive", olc::WHITE, 4);

        // Main Drawing Loop, Draws world array
        for (int x = 0; x < nWorldWidth; x++)
            for (int y = 0; y < nWorldHeight; y++)
            {
                if (world[y * nWorldWidth + x].alive)
                    FillRect(x * fBlockWidth, y * fBlockWidth, fBlockWidth, fBlockWidth, olc::BLUE);
            }


        return true;
    }
};

int main() {
    GameOfLife demo;
    if (demo.Construct(1280,960,1,1))
        demo.Start();
    return 0;
}

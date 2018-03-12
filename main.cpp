/** Solution = Valid Route
Program made to optimized route in Road to Ballhalla but could be use to similar purposes as well
This program compute all possible routes and display which is the best (or close to best) route
It will simply display the route which has the lowest total time registered inside "values" array
Change the "values" array with your own times (in milliseconds) and run the program
Since there is many (many many many) possible routes you can use the #define below to limit the number of possibilities
If no solution is found you'll need to change some parameters like -1 (by real values inside the "values" array) or #defines

Solutions are displayed at the end
**/

#include <iostream>
#include <stdio.h>
#include <vector>
#include <time.h>

using namespace std;

/**
UNIT is number of element, just let this at 20 because there will always be 20 levels
RANGE_TIME is here to limit results. It will simply not show solutions which are 3000ms (default) longer than the lowest solution
MAX_RESULT is needed to limit the number of result and arrays size. Let it at 100, more if you want but there is no real purpose
ENB_DIFF is a feature that show where all the solutions have the same pattern
    very useful to see which level doesn't change whatever the route is.
    If the solutions displayed are "103311, 104311, 103301" ENB_DIFF will display "10 3 1". Blank space means the solution changed
**/
#define UNIT 20
#define RANGE_TIME 8000
#define MAX_RESULT 100
#define ENB_DIFF 1

/** "values" array store the time difference for every level between any% and the category
In order : "any%, 60%, 80%, 90%, 100%" where the % is the orb completion for each levels
If you need 2 more seconds than any% to get the 60% reward on level 3 you'll write 2000 inside the array on 2nd col, 3rd row
-1 is here to tell there is no solution in this level with this category.
Make the computation faster, let -1 and not 0 if you have no value to put
**/
int values[UNIT][5] = {
    {0, 2000, 11000, 20000, 32000}, //1-1 0
    {0, 4000, 18000, 26000, 41000}, //1-2 1
    {0, -1, -1, -1, -1}, //1-3 2
    {0, -1, 15000, -1, -1}, //1-4 3
    {0, 7000, 17000, -1, -1}, //1-5 4
    {0, 3000, 9000, -1, -1}, //2-1 5
    {0, 4000, 14000, -1, 29000}, //2-2 6
    {0, -1, -1, -1, -1}, //2-3 7
    {0, -1, -1, -1, -1}, //2-4 8
    {0, 4800, 9600, 18200, -1}, //2-5 9
    {0, -1, -1, -1, 9000}, //3-1 10
    {0, -1, -1, -1, -1}, //3-2 11
    {0, -1, -1, -1, -1}, //3-3 12
    {0, 3000, 7000, -1, 24000}, //3-4 12
    {0, -1, -1, -1, -1}, //3-5 14
    {0, -1, -1, -1, -1}, //4-1 15
    {0, -1, -1, -1, -1}, //4-2 16
    {0, 0, -1, -1, -1}, //4-3 17
    {0, 0, 4000, 7000, 13000}, //4-4 18
    {0, 0, 0, 3000, 8000} //4-5 19
    };

/** Once level categories is set, playgame will check if the solution is possible to finish the game
Basically it just check about medals needed. You need 5 medals to go to world 2, 10 to world 3, 15 to world 4 and 25 to finish
@return the sum of time the solution takes, return 0 if the solution is not possible
**/
int playgame(int (&data)[UNIT])
{
    int i;
    int sum = 0;

    for (i = 0; i < 20; i++)
    {
        sum += data[i];
        if (sum < i/5)  //Just to make things with less code. Is equivalent to "after 5 more levels you'll change medals needed by +5"
            return (0);
    }
    if (sum != 25) //You could finish the game with more than 25 medals but that would be a waste of time somewhere
        return (0);

    sum = 0;
    for (i = 0; i < UNIT; i++)      //Sum the time it takes
        sum += values[i][data[i]];

    return (sum);
}

/** Recursive function (read at bottom for more information about how it works)
"data" is the route, "result" is solutions stored, "time" is time of solutions,
    "lowest_res" is lowest time, "pos" is level (or depth of the rescursive function) and "total" is number of solutions
(pos == UNIT) is the point where the route is done, it will check if the route is possible and if so will register the solution
"else" will create the route and call the function itself
**/
void set_data(int (&data)[UNIT], int (&result)[MAX_RESULT][UNIT], int (&time)[MAX_RESULT], int &lowest_res, int pos, int &total)
{
    int i;
    static int res;

    if (pos == UNIT)
    {
        res = playgame(data);   //Route is done, let's check its validity
        if (res > 0 && res - lowest_res <= RANGE_TIME)  //res > 0 means the route is valid and we check if the time is inside the range
        {
            total++;
            if (total <= MAX_RESULT)    //Just to be sure the number of solutions is not too much
            {
                for (i = 0; i < UNIT; i++)          //store the result
                    result[total - 1][i] = data[i];
                time[total - 1] = res;
                if (lowest_res > res)   //We update the lowest result
                    lowest_res = res;
            }
        }
    }
    else
    {
        for (i = 0; i < 5; i++)     //This will set every possibility for the current level (any% to 100%)
        {
            data[pos] = i;
            if (values[pos][data[pos]] == -1)   //If we do this now it makes the program really faster
                continue;
            set_data(data, result, time, lowest_res, pos + 1, total);   //The self-call with pos + 1 to go through every level
        }
    }
}

int main()
{
    int i;
    int j;
    int data[UNIT];                 //Store the route
    int result[MAX_RESULT][UNIT];   //Store all valid routes
    int time[MAX_RESULT] = {0};     //Store the time of solutions
    int lowest_res = 12345678;      //Store the time of the lowest solution     Related to RANGE_TIME
    int total = 0;                  //Number of solutions                       Related to MAX_RESULT
    int diff[UNIT];                 //Store the diff                            Related to ENB_DIFF
    clock_t time_start;             //Just to know the time it took to compute all solutions
    clock_t time_end;

    time_start = clock();           //Start the timer
    set_data(data, result, time, lowest_res, 0, total); //Everything begins with this
    time_end = clock();             //End the timer

    if (ENB_DIFF == 1)              //Store the first result for diff
        for (i = 0; i < UNIT; i++)
            diff[i] = result[0][i];

    for (i = 0; (i < MAX_RESULT) && (time[i] > 0); i++)     //Display all the solutions
    {
        if (time[i] - lowest_res <= RANGE_TIME)
        {
            for (j = 0; j < UNIT; j++)
            {
                cout << result[i][j];
                if (ENB_DIFF == 1 && result[i][j] != diff[j])   //Check if the result is different with diff
                    diff[j] = -1;                               //-1 means there is a diff
            }
            cout << " " << time[i] << " +" << time[i] - lowest_res << endl;
        }
    }
    
    if (ENB_DIFF == 1)
    {
        for (i = 0; i < UNIT; i++)     //Display the diff result
        {
            if (diff[i] != -1)
                cout << diff[i];
            else
                cout << " ";
        }
        cout << " <= diff" << endl;
    }
    
    if (total >= 1000)              //If true either decrease RANGE_TIME or increase MAX_RESULT
        cout << endl << "TOTAL exceded MAX_RESULT" << endl << endl; 

    cout << endl << "Calul done in " << time_end - time_start << "ms" << endl;

    return (0);
}

/**
About the recursive function, if you don't know what it is the best is to look up on internet.
It's kinda hard to explain and it's not the best to do this here with just plain text.

set_data (the recursive function) is made to set every possibilities
If you would like to do the same without a recursive function you could write something like 20 "for loops" inside themselves,
    which is not really good looking and efficient.
With set_data I am able to optimize how it has to be done especially with the "-1".
    If some level has a -1 for its category it means you can skip to the next category, their is no meaning to compute an invalid route

I'm not a master of C++, if you have recommendation about the code don't hesitate to talk.
Just keep in mind this is a small program for a unique purpose, I won't make a GUI for it.
**/

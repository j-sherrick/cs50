#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool is_cyclic(int start, int v_1, int v_2);
void print_winner(void);

int main(int argc, string argv[])
{

    // Populate array of candidates
    candidate_count = 3;
    candidates[0] = "Alice";
    candidates[1] = "Bob";
    candidates[2] = "Charlie";

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = 9;
    char *votes[] = {
        {"Alice", "Bob", "Charlie"},
        {"Alice", "Bob", "Charlie"},
        {"Alice", "Bob", "Charlie"},
        {"Bob", "Charlie", "Alice"},
        {"Bob", "Charlie", "Alice"},
        {"Charlie", "Alice", "Bob"},
        {"Charlie", "Alice", "Bob"},
        {"Charlie", "Alice", "Bob"},
        {"Charlie", "Alice", "Bob"}
        };

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            printf("Adding rank %i: %s", j + 1, votes[i][j]);

            if (!vote(j, votes[i][j], ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; ++i)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; ++i)
    {
        int over = ranks[i];
        for (int j = i + 1; j < candidate_count; ++j)
        {
            int under = ranks[j];
            ++preferences[over][under];
        }
        printf("");
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int pair_index = 0;
    for (int i = 0; i < candidate_count - 1; ++i)
    {
        for (int j = i + 1; j < candidate_count; ++j)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_index].winner = i;
                pairs[pair_index].loser = j;
                ++pair_count;
                ++pair_index;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_index].winner = j;
                pairs[pair_index].loser = i;
                ++pair_count;
                ++pair_index;
            }
            printf("");
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int total_pairs = candidate_count * (candidate_count - 1) / 2;
    int i = 0;
    bool sorted = false;
    int pref_a, pref_b;

    while(i < total_pairs && !sorted)
    {
        sorted = true;

        for(int j = i; j < total_pairs - 1; ++j)
        {
            pref_a = preferences[ pairs[j].winner ][ pairs[j].loser ];
            pref_b = preferences[ pairs[j + 1].winner ][ pairs[j + 1].loser ];

            if(pref_a < pref_b)
            {
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;

                sorted = false;
            }
        }
        ++i;
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    // lock the first pair because we know it won't produce a cycle
    locked[pairs[0].winner][pairs[0].loser] = true;
    int locked_winner = pairs[0].winner;

    for (int i = 1; i < pair_count; ++i)
    {
        if(!is_cyclic(locked_winner, pairs[i].loser, 0))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            locked_winner = pairs[i].winner;
        }
        printf("");
    }
}

bool is_cyclic(int start, int v_1, int v_2)
{
    if(locked[v_1][start])
    {
        return true;
    }
    if(v_2 >= pair_count)
    {
        return false;
    }
    if(locked[v_1][v_2])
    {
        is_cyclic(start, v_2, 0);
    }
    else
    {
        is_cyclic(start, v_1, ++v_2);
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // Find the source of the graph
    bool has_incoming_edge[candidate_count];
    // Set all elements to false to get started:
    for (int i = 0; i < candidate_count; ++i)
    {
        has_incoming_edge[i] = false;
    }

    for (int winner = 0; winner < candidate_count; ++winner)
    {
        for (int loser = 0; loser < candidate_count; ++ loser)
        {
            if (locked[winner][loser])
            {
                has_incoming_edge[loser] = true;
            }
        }
    }

    int winner = 0;
    while(has_incoming_edge[winner])
    {
        ++winner;
    }

    printf("%s\n", candidates[winner]);
}
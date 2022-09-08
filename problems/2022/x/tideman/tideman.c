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
bool is_cyclic(int start, int next);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
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
            // printf("\t- Adding rank %d: %s\n", rank, name);
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
            // printf("\t - %s over %s\n", candidates[over], candidates[under]);
        }
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
                // printf("%s(%d) wins over %s(%d). ", candidates[i], preferences[i][j], candidates[j], preferences[j][i]);
                // printf("%d pairs added.\n", pair_count);
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_index].winner = j;
                pairs[pair_index].loser = i;
                ++pair_count;
                ++pair_index;
                // printf("%s(%d) wins over %s(%d). ", candidates[j], preferences[j][i], candidates[i], preferences[i][j]);
                // printf("%d pairs added.\n", pair_count + 1);
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int i = 0;
    bool sorted = false;
    int pref_a, pref_b;

    while(i < pair_count && !sorted)
    {
        sorted = true;

        for(int j = i; j < pair_count - 1; ++j)
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
    for(int i = 0; i < pair_count; ++i)
    {
        if(!is_cyclic(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            // printf("\n%s is locked in over %s\n", candidates[pairs[i].winner], candidates[pairs[i].loser]);
        }
    }
}

bool is_cyclic(int start, int next)
{
    if (locked[next][start])
    {
        return true;
    }
    else 
    {
        for (int i = 0; i < candidate_count; ++i)
        {
            if(locked[next][i])
            {
                return is_cyclic(start, i);
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    bool has_incoming = true;
    for(int i = 0; i < candidate_count && has_incoming; ++i)
    {
        has_incoming = false;
        for(int j = 0; j < candidate_count && !has_incoming; ++j)
        {
            if(locked[j][i])
            {
                has_incoming = true;
            }
        }
        if(!has_incoming)
        {
            printf("%s\n", candidates[i]);
        }
    }
}
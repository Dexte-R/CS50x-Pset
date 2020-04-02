#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
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
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }
    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // input name matches candiate name
    // update candiate.votes
    for (int i = 0; i < candidate_count; i++)
    // use candidate_count and not MAX
    // is candidate_count < MAX then for i > candidate_count, candidates[i].name = null
    // strcmp with null gave segmentation fault
    {
        if (strcmp(name,  candidates[i].name) == 0) // cannot compare strings using ==
        {
            candidates[i].votes++;  // update vote for chosen candidate
            return true;
        }
    }
    // if names do not match
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // filter by votes since there can be several candidates with same #votes
    int maxvote = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > maxvote)
        {
            maxvote = candidates[i].votes;
        }
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == maxvote)
            printf("%s\n", candidates[i].name);
    }
    return;
}
// I initially tried to implement a sort algorithm but this is not an efficient approach for this problem
// problem requires only the candidate(s) with the highest votes but no the ranking between voters,
// If ranking if required then use sort alogrithm.


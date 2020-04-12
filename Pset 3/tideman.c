#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX] = {{0}};

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
// max size: 9 choose 2
// sum of entries in upper triangular

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void mergesort(pair arr[], int l, int r);
void merge(pair arr[], int l, int r, int mid);
bool cyclic(int row, int col);

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
    int i,j;
    int row = candidate_count, col = candidate_count;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            printf("%i ", preferences[i][j]);
        }
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
    // search candidate list
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i; // assign candidate index to ranks[]
            return true;
        }
    }
    // if names do not match
    return false;
}


// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // update matrix componenets of preferences[][] (P[][])
    // based on candidate index in ranks[]

    // eg candidate: Alice Bob Charlie Dick
    // eg ranking: 1. Charlie 2. Alice 3. Bob 4. Dick
    // then P[ranks[0]][ranks[j]] += 1 where ranks[0] > ranks[j]
    // ... remove candidate with every iteration
    // P[ranks[i-1]][j] += 1 for the last pair
    // need not update for candidate ranked last

    // for i-th rank, check for all other ranks below it
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // compare upper and lower triangular matrix of preferences[][]
    // <=> compare P[i][j] and P[j][i]
    // => record pairs[].winner; pairs[].loser with the candidate index
    // => record pair_count

    for (int i = 0; i < candidate_count; i++ ) //row index
    {
        for (int j = i + 1; j < candidate_count; j++) //col index
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair_count++;
                pairs[pair_count -1].winner = i;
                pairs[pair_count -1].loser = j;

            }
            if (preferences[i][j] < preferences[j][i])
            {
                pair_count++;
                pairs[pair_count -1].winner = j;
                pairs[pair_count -1].loser = i;
            }
        }
    }
    return;
    // possible to do recursion here?
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    mergesort(pairs, 0, pair_count - 1);
    return;
}

// modified mergesort: sort by decreasing order
void mergesort(pair arr[], int l, int r) // assuming l <= r
{
    if (l < r)
    {
        int mid = l + (r - l) / 2;
        mergesort(arr, l, mid); // split left arr
        mergesort(arr, mid + 1, r); // split right arr
        merge(arr, l, r, mid);
    }
}

// sort pairs array based on strength of victory in preferences array
void merge(pair arr[], int l, int r, int mid)
{
    int sizeL = mid - l + 1, sizeR = r - mid;
    pair L[sizeL], R[sizeR];  // create arr to store unsorted

    // copy unsorted from original arr
    for (int i = 0; i < sizeL; i++)
    {
        L[i].winner = arr[l + i].winner;
        L[i].loser = arr[l + i].loser;
    }
    for (int j = 0; j < sizeR ; j++)
    {
        R[j].winner = arr[mid + 1 + j].winner;
        R[j].loser = arr[mid + 1 + j].loser;
    }

    int x = 0, y = 0, z = 0;
    while (x < sizeL && y < sizeR) // loop till end for either subarray
    {
        // L and R contain the indices for the preferences array which
        // contains the strength of victory
        if (preferences[L[x].winner][L[x].loser] >= preferences[R[y].winner][R[y].loser])
        {
            arr[l + z].winner = L[x].winner;
            arr[l + z].loser = L[x].loser;
            x++;
            z++;
        }
        else
        {
            arr[l + z].winner = R[y].winner;
            arr[l + z].loser = R[y].loser;
            y++;
            z++;
        }
    }
    // either of the following loops will execute
    while (x < sizeL) // copy remaining elements in L
    {
        arr[l + z].winner = L[x].winner;
        arr[l + z].loser = L[x].loser;
        x++;
        z++;
    }
    while (y < sizeR) // copy remaining elements in R
    {
        arr[l + z].winner = R[y].winner;
        arr[l + z].loser = R[y].loser;
        y++;
        z++;
    }
}

// Lock pairs into the candidate graph in order
void lock_pairs(void)
{
    // locked[i][j]: arrow points from i to j
    for (int i = 0; i < pair_count; i++)
    {
        int row = pairs[i].winner, col = pairs[i].loser;
        if (!cyclic(row, col))
            locked[row][col] = true;
    }
    return;
}

bool cyclic(int row, int col)
{
    // row is winner, col is loser in the locked[i][j] matrix
    // check if there exist a path from col to row <=> path from loser to winner
    if (locked[col][row] == true)
    {
        return true;
    }
    else
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[col][i] == true)
                return cyclic(row, i);
        }
        return false;
    }
}


// Print the winner of the election: winner has no arrows pointing towards it
void print_winner(void)
{
    // get how many arrows point at each candidate
    int source[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j])
                source[j]++;
        }
    }
    // winner is the candidate least arrows pointing at it
    int arrows = source[0]; 
    for (int k = 0; k < candidate_count; k++)
    {
        if (source[k] < arrows)
            arrows = source[k];
    }
    for (int m = 0; m < candidate_count; m++)
    {
        if (arrows == source[m])
            printf("%s\n", candidates[m]);
    }
    return;
}

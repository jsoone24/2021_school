#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

static std::mt19937 gen = std::mt19937((unsigned int)time(NULL));
static std::uniform_real_distribution<> hundred(0, 99);
using namespace std;

namespace Weight
{
    struct node
    {
        int n_visited;                       // No. of visited this node
        int n_neighbors;                     // No. of neighbors
        int id;                              // node identifier
        int sum_weight;                      // sum of to neighbor weight
        float pageRankVal;                   // Current node PageRankVal
        string name;                         // string name of this node
        vector<pair<node *, int>> neighbors; // neighbors node address container
    };

    class Graph
    {
        int nodeNum;             // No. of nodes
        vector<node *> nodes;    // vector container for nodes
        vector<int> id2IdxTable; // to find index from file node id. first: id from file, second : idx from vector

    public:
        Graph(string filename); // Constructor
        ~Graph();
        void LoadEdge(string filename, bool inter);                   // load edge file
        void AddEdge(int source, int target, int weight, bool inter); //adding edge
        int Id2Idx(int n);                                            // convert file node id to vector node id
        void RandomWalker(int i, int n, float q);                     // i : start node, n : walk length, q : probability jumping to random node
        int PageRank(int i, int n, float q);                          // cal pagerank
    };

    //todo
    Graph::Graph(string filename) // Get filename having node name
    {
        this->nodeNum = 0;

        std::ifstream ifs(filename);
        std::string line;

        //read file by line
        std::getline(ifs, line);
        while (std::getline(ifs, line))
        {
            // split by '\t'
            std::stringstream ls(line);
            std::string id;
            std::string name;
            std::getline(ls, id, '\t'); //save first param value to id
            std::getline(ls, name);     //save second param value to name

            node *temp = new node;

            //initialize node values
            temp->n_visited = 0;
            temp->id = std::stoi(id); //first col
            temp->pageRankVal = 0;
            temp->name = name; //second col
            temp->n_neighbors = 0;
            temp->sum_weight = 0;

            nodes.push_back(temp);
            id2IdxTable.push_back(std::stoi(id)); // put id into table
            this->nodeNum++;
        }

        ifs.close();
    }

    //todo
    Graph::~Graph()
    {
        // dealloc nodes
        for (vector<node *>::iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            delete (*it);
        }
    }

    //todo
    void Graph::AddEdge(int source, int target, int weight, bool inter) //inter: whether interdirectional
    {
        nodes[source]->neighbors.push_back({nodes[target], weight});
        nodes[source]->n_neighbors++;
        nodes[source]->sum_weight += weight;

        if (inter == true) // if interdirectional, put source in target neighbor
        {
            vector<pair<node *, int>>::iterator it;
            // check if already in there
            for (it = nodes[target]->neighbors.begin(); it != nodes[target]->neighbors.end(); it++)
            {
                if ((*it).first == nodes[source])
                    break;
            }
            // if not, update
            if (it == nodes[target]->neighbors.end())
            {
                nodes[target]->neighbors.push_back({nodes[source], weight});
                nodes[target]->n_neighbors++;
                nodes[target]->sum_weight += weight;
            }
        }
    }

    //todo
    int Graph::Id2Idx(int n) // get node id (from node name file) and invert to Graph Class nodes index
    {
        auto it = find(id2IdxTable.begin(), id2IdxTable.end(), n);
        if (it == id2IdxTable.end())
        {
            cout << "node not found\n";
            return -1;
        }
        else
        {
            return std::distance(id2IdxTable.begin(), it);
        }
    }

    //todo
    void Graph::LoadEdge(string filename, bool inter) //inter: whether interdirectional
    {
        std::ifstream ifs(filename);
        std::string line;

        //read file by line
        std::getline(ifs, line);
        while (std::getline(ifs, line))
        {
            // split by '\t'
            std::stringstream ls(line);
            std::string source;
            std::string target;
            std::string weight;
            std::getline(ls, source, '\t'); //save first param value to id
            std::getline(ls, target, '\t'); //save second param value to name
            std::getline(ls, weight);       //save third param value to weight

            int idxS = Id2Idx(std::stoi(source)); // Vector index of node source
            int idxT = Id2Idx(std::stoi(target)); // Vector index of node target
            int valW = std::stoi(weight);

            AddEdge(idxS, idxT, valW, inter);
        }

        ifs.close();
        /*  for dump
    for (vector<node *>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        cout << "id : " << (*it)->id << endl             << "neighbors : ";
        for (vector<pair<node *, int>>::iterator tt = (*it)->neighbors.begin(); tt != (*it)->neighbors.end(); tt++)
            cout << (*tt).first->id << " ";
        cout << endl             << endl;
    }*/
    }

    ////////////////////////////////////////
    ////////         Task 4         ////////
    //////// implement RandomWalker ////////
    ////////////////////////////////////////

    //todo
    // i : start node, n : walk length, q : probability jumping to random node
    void Graph::RandomWalker(int i, int n, float q)
    {
        int random = 0;          // random value for random jump q
        int random_node = 0;     // random value to pick neighbor to jump
        int q_percent = q * 100; //invert q into percent value
        node *ptr;               // to point current or next node
        ptr = nodes[i];          // ptr initially point start node

        int sum = 0;
        vector<pair<node *, int>>::iterator it;

        while (n > 0) // loop until end
        {
            if (ptr->n_neighbors == 0) // if no neighbor node, search another node
            {
                std::uniform_real_distribution<> dist(0, nodeNum - 1);
                random_node = dist(gen);
                ptr = nodes[random_node];
                continue;
            }
            random = hundred(gen);
            if ((random <= q_percent) && (q_percent != 0)) //random node pick
            {
                std::uniform_real_distribution<> dist(0, nodeNum - 1);
                random_node = dist(gen);
                ptr = nodes[random_node];
            }
            else // pick node from neighbors
            {
                // get 0 <= random_value < sum_weight
                // keep sum neighbor nodes weight until random_node
                // if sum > random_node, the node being about to sum is randomly chosen node.
                sum = ptr->neighbors[0].second;
                std::uniform_real_distribution<> dist(0, ptr->sum_weight);
                random_node = dist(gen);

                for (it = ptr->neighbors.begin() + 1; (it != ptr->neighbors.end()) && (sum < random_node); it++)
                {
                    sum += (*it).second;
                }
                if (it == ptr->neighbors.end())
                    it--;
                ptr = (*it).first;
            }
            ptr->n_visited++;
            n--;
        }
    }

    bool compare(pair<float, int> a, pair<float, int> b)
    {
        return a.first > b.first;
    }

    /////////////////////////////////////////
    ////////          Task 5         ////////
    //////// implement PageRank func ////////
    /////////////////////////////////////////

    //todo
    // i : start node, n : walk length, q : probability jumping to random node
    int Graph::PageRank(int i, int n, float q)
    {

        // First, call RandomWalker
        vector<pair<float, int>> rank;
        RandomWalker(i, n, q);

        // Second, iterate all nodes and calculate pagerank
        for (vector<node *>::iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            (*it)->pageRankVal = (float)((*it)->n_visited) / (float)n;
            rank.push_back({(*it)->pageRankVal, (*it)->id});
        }

        // Third, print sorted pagerank
        int t = 0;
        sort(rank.begin(), rank.end());
        for (vector<pair<float, int>>::iterator it = rank.begin(); it != rank.end(); it++, t++)
        {
            if (nodeNum - t <= 10)
                cout << "rank : " << nodeNum - t << "\tpageRankValue : " << (*it).first << "\tname : " << nodes[Id2Idx((*it).second)]->name << endl;
        }
    }
}

/////////////////////////////////////
////////       Task 6        ////////
//////// test from real data ////////
/////////////////////////////////////

//todo
int main()
{
    // First. open node file
    //Weight::Graph WStarW_Ep1 = Weight::Graph("./dataset/starwars/starwars-episode-1-interactions-allCharacters-nodes.tsv");
    //Weight::Graph WStarW_Ep2 = Weight::Graph("./dataset/starwars/starwars-episode-2-interactions-allCharacters-nodes.tsv");
    //Weight::Graph WStarW_Ep3 = Weight::Graph("./dataset/starwars/starwars-episode-3-interactions-allCharacters-nodes.tsv");
    //Weight::Graph WStarW_Ep4 = Weight::Graph("./dataset/starwars/starwars-episode-4-interactions-allCharacters-nodes.tsv");
    //Weight::Graph WStarW_Ep5 = Weight::Graph("./dataset/starwars/starwars-episode-5-interactions-allCharacters-nodes.tsv");
    //Weight::Graph WStarW_Ep6 = Weight::Graph("./dataset/starwars/starwars-episode-6-interactions-allCharacters-nodes.tsv");
    //Weight::Graph WStarW_Ep7 = Weight::Graph("./dataset/starwars/starwars-episode-7-interactions-allCharacters-nodes.tsv");
    Weight::Graph WStarW_All = Weight::Graph("./dataset/starwars/starwars-full-interactions-allCharacters-nodes.tsv");
    //
    //Weight::Graph WBicycleT_50 = Weight::Graph("./dataset/bicycle/station_names.tsv");
    //Weight::Graph WBicycleT_100 = Weight::Graph("./dataset/bicycle/station_names.tsv");
    //Weight::Graph WBicycleT_All = Weight::Graph("./dataset/bicycle/station_names.tsv");

    // Second. open datafile. second parameter : if true : interdirectional, false : bidirectional
    //WStarW_Ep1.LoadEdge("./dataset/starwars/starwars-episode-1-interactions-allCharacters-links.tsv", true);
    //WStarW_Ep2.LoadEdge("./dataset/starwars/starwars-episode-2-interactions-allCharacters-links.tsv", true);
    //WStarW_Ep3.LoadEdge("./dataset/starwars/starwars-episode-3-interactions-allCharacters-links.tsv", true);
    //WStarW_Ep4.LoadEdge("./dataset/starwars/starwars-episode-4-interactions-allCharacters-links.tsv", true);
    //WStarW_Ep5.LoadEdge("./dataset/starwars/starwars-episode-5-interactions-allCharacters-links.tsv", true);
    //WStarW_Ep6.LoadEdge("./dataset/starwars/starwars-episode-6-interactions-allCharacters-links.tsv", true);
    //WStarW_Ep7.LoadEdge("./dataset/starwars/starwars-episode-7-interactions-allCharacters-links.tsv", true);
    WStarW_All.LoadEdge("./dataset/starwars/starwars-full-interactions-allCharacters-links.tsv", true);
    //
    //WBicycleT_50.LoadEdge("./dataset/bicycle/bicycle_trips_over_50.tsv", false);
    //WBicycleT_100.LoadEdge("./dataset/bicycle/bicycle_trips_all.tsv", false);
    //WBicycleT_All.LoadEdge("./dataset/bicycle/bicycle_trips_all.tsv", false);

    //Last. let RandomWalker run and get PageRank
    //WStarW_Ep1.PageRank(10, 300000, 0.9);
    //WStarW_Ep2.PageRank(10, 300000, 0.9);
    //WStarW_Ep3.PageRank(10, 300000, 0.9);
    //WStarW_Ep4.PageRank(10, 300000, 0.9);
    //WStarW_Ep5.PageRank(10, 300000, 0.9);
    //WStarW_Ep6.PageRank(10, 300000, 0.9);
    //WStarW_Ep7.PageRank(10, 300000, 0.9);
    WStarW_All.PageRank(50, 30000000, 0.7);
    //
    //WBicycleT_50.PageRank(50, 30000000, 0.7);
    //WBicycleT_100.PageRank(50, 30000000, 0.7);
    //WBicycleT_All.PageRank(98, 30000000, 0.2);

    //for (int k = 0; k < 10; k++)
    //{
    //    cout << "\n\n"
    //         << "try : " << k + 1 << endl;
    //    Weight::Graph WBicycleT_All = Weight::Graph("./dataset/bicycle/station_names.tsv");
    //    WBicycleT_All.LoadEdge("./dataset/bicycle/bicycle_trips_all.tsv", false);
    //    WBicycleT_All.PageRank(k, 10000, 0.7);
    //}
    //return 0;
}
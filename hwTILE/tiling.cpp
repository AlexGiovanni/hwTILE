//Alexis Santiago November 22, 2018
#include "tiling.h"
#include "vertex.h"
#include <vector>

using namespace std;
//hash table under the hood. list of vertices(pointers to Vertex objects)
//unordered_map<string, Vertex*> vertexSet;
unordered_set<Vertex*> vSet;//put all vertices here at end of graph build
int rc[2];//stores the number of rows and columns in array
//finds the number of columns and rows and returns an array containing those values row,col
void findRowCol(string maze) {
	rc[0] = 0; rc[1] = 0;
	for (int i = 0; i < maze.size() - 1; i++) {//find #of row and col
		if (maze[i] == '\n') {
			rc[1] = i + 1;//find # of col
			break;
		}
	}

	rc[0] = (maze.size() / rc[1]) - 1;//#of rows
	rc[1] = rc[1] - 2;//# of columns
}


// Finds a (shortest according to edge length) augmenting path
// from s to t in a graph with vertex set V.
// Returns whether there is an augmenting path.
bool augmenting_path(Vertex* s, Vertex* t, 
        unordered_set<Vertex*> V, vector<Vertex*> &P)
{
        // Check that s and t aren't nullptr
        if (s == nullptr || t == nullptr)
	{
		cerr << "augmenting_path() was passed nullptr s or t." << endl;
		abort();
	}

        // Check that s and t are in the graph
        if (V.find(s) == V.end() || V.find(t) == V.end())
	{
		cerr << "augmenting_path() was passed s or t not in V." << endl;
		abort();
	}

	// Check that every vertex has valid neighs/weights.
	for (Vertex* v : V)
		for (Vertex* vn : v->neighs)
			if (v->weights.find(vn) == v->weights.end())
			{
				cerr << "augmenting_path() was passed invalid vertex." << endl;
				abort();
			}

        // Since augmenting paths should have the fewest edges,
	// not the minimum weight, run BFS.
	queue<Vertex*> Q;
	Q.push(s);

	unordered_set<Vertex*> R;
	R.clear(); 
	R.insert(s);

	unordered_map<Vertex*, Vertex*> prev;

	while (!Q.empty())
	{
		Vertex* cur = Q.front();
		Q.pop();

		for (Vertex* nei : cur->neighs)
		{
			// Must have positive edge weight
			if (cur->weights[nei] == 0)
				continue;

			if (R.find(nei) == R.end())
			{
				Q.push(nei);
				R.insert(nei);
				prev[nei] = cur; 
			}
		}
	}      

        // If BFS never reached t
        if (R.find(t) == R.end())
                return false;

        // Reconstruct shortest path backwards
        P.clear();
        P.push_back(t);
        while (P[P.size()-1] != s)
                P.push_back(prev[P[P.size()-1]]);

        // Reverse shortest path
        for (int i = 0; i < P.size()/2; ++i)
		swap(P[i], P[P.size()-1-i]);

        return true;
}

// Returns the maximum flow from s to t in a weighted graph with vertex set V.
// Assumes all edge weights are non-negative.
int max_flow(Vertex* s, Vertex* t, unordered_set<Vertex*> V)
{
	// If s or t is invalid.
        if (s == nullptr || t == nullptr)
	{
		cerr << "max_flow() was passed nullptr s or t." << endl;
		abort(); 
	}

	// If s or t is not in the vertex set.
        if (V.find(s) == V.end() || V.find(t) == V.end())
	{
		cerr << "max_flow() was passed s or t not in V." << endl;
		abort(); 
	}

	// Check that every vertex has valid neighs/weights.
		for (Vertex* v : V)
			for (Vertex* vn : v->neighs) 
				if (v->weights.find(vn) == v->weights.end())
				{
					cerr << "\nmax_flow() was passed invalid vertex." << endl;
					abort();
				}
			

        // Create a deep copy of V to use as the residual graph
        unordered_set<Vertex*> resV;
        unordered_map<Vertex*, Vertex*> C; // Maps vertices in V to copies in resV
        for (Vertex* vp : V)
        {
                Vertex* rp = new Vertex;
                resV.insert(rp);
                C[vp] = rp;
        }
        for (Vertex* vp : V)
                for (Vertex* np : vp->neighs)
                {
                        C[vp]->neighs.insert(C[np]);
                        C[vp]->weights[C[np]] = vp->weights[np];
                }
	// Add any missing necessary "back" edges. 
        for (Vertex* vp : V)
                for (Vertex* np : vp->neighs)
		{
			if (C[np]->neighs.find(C[vp]) == C[np]->neighs.end())
			{
				C[np]->neighs.insert(C[vp]);
				C[np]->weights[C[vp]] = 0;
			}
		}

        // Run Edmonds-Karp
        while (true)
        {
                // Find an augmenting path
                vector<Vertex*> P;
                if (!augmenting_path(C[s], C[t], resV, P))
                        break;  
                // Update residual graph
                for (int i = 0; i < P.size()-1; ++i)
                {
                        --((*(resV.find(P[i])))->weights[P[i+1]]);
                        ++((*(resV.find(P[i+1])))->weights[P[i]]);
                }
        }

        // Compute actual flow amount
        int flow = 0;
        for (Vertex* snp : C[s]->neighs)
                flow += 1 - C[s]->weights[snp];

        // Delete residual graph
        for (Vertex* vp : resV)
                delete vp;

        return flow;
}


//check if there is a vertex on top
//if the vertex i'm serching for exists in vertexset return true
bool checkTop(int r, int c, vector<vector<Vertex *>>m) {
	//string key = to_string(r - 1) + "," + to_string(c);
	//cout << "matrix: "<<m[r - 1][c]<<endl;
	if (r>=0) {
		if (m[r - 1][c] != 0) {//error if index is out of bounds

			return true;
		}
	}
	return false;
}

//check if there is a vertex on its left
//given coordinates calculates left coordinates
bool checkLeft(int r, int c, vector<vector<Vertex *>>m) {
	//string key = to_string(r) + "," + to_string(c - 1);
	if (c >= 0) {
		if (m[r][c- 1] != 0) {//error if index is out of bounds
			return true;
		}
	}
	return false;
}

bool checkRight(int r, int c, vector<vector<Vertex *>>m) {
	if (c+1 < rc[1]) {
		if (m[r][c + 1] != 0) {//error if index is out of bounds
			return true;
		}
	}
	return false;
}

bool checkDown(int r, int c, vector<vector<Vertex *>>m) {
	if (r + 1 < rc[0]) {
		if (m[r+ 1][c ] != 0) {//error if index is out of bounds
			return true;
		}
	}
	return false;
}

////Add a basic (bidirectional) edge connecting a and b
////each string is a coordinate x,y w is the weight
//void addEdge(string a, string b, int w)
//{
//	Vertex * aVert = vertexSet[a];//find object to a vertex create a pointer to it
//	Vertex * bVert = vertexSet[b];
//	//avert point to vertex a //bvert points to vertex b
//	aVert->weights.insert(make_pair(bVert, w));//add b to the list of a's neighbors
//	//bVert->weights.insert(make_pair(aVert, w));//add a to the list of b's neighbors
//}

//Add a basic (bidirectional) edge connecting a and b
//each string is a coordinate x,y w is the weight
//void addEdge(Vertex* a, Vertex*b)
//{
//	Vertex * aVert = vertexSet[a];//find object to a vertex create a pointer to it
//	Vertex * bVert = vertexSet[b];
//	//aVert->neighs.insert()
//	//avert point to vertex a //bvert points to vertex b
//	//aVert->weights.insert(make_pair(bVert, w));//add b to the list of a's neighbors
//	//bVert->weights.insert(make_pair(aVert, w));//add a to the list of b's neighbors
//}

void displayMatrix(vector<vector<Vertex*>>m) {
	cout << "Matrix "<<rc[0]<<" "<<rc[1] << endl;
	for (int i = 0; i <= rc[0]; i++) {
		for (int j = 0; j <= rc[1]; j++) {
			cout << m[i][j]<<", ";
		}
		cout << endl;
	}
	cout << endl;
}

void displayNeighs(Vertex * n) {
	for (auto it = n->neighs.begin(); it != n->neighs.end(); ++it)
		std::cout << " " << *it;
	std::cout << std::endl;
}

//creates the graph from the maze string
//creates vertecies and stores them in vertexset
bool createGraph(string maze) {
	//Step 1: create the graph
	vector<vector<Vertex *>> matrix(rc[0]+1, vector<Vertex *>(rc[1]+1, 0));
	Vertex* s = new Vertex(); vSet.insert(s);
	Vertex* t = new Vertex(); vSet.insert(t);
	//cout << maze << endl;
	int r = 0; int c = 0; int red = 0;
	for (int i = 0; i < maze.size() - 1; i++) {//traverse the string
		if (r % 2 == 0 && c == 0) {red = 0;}//keeps track of red and black vertecies
		else if (r % 2 != 0 && c == 0) {red = 1;}
		if (maze[i] == ' ') {
			Vertex * newVertex = new Vertex();//create a pointer to vertex object
			vSet.insert(newVertex);//add vertex to list of vertecies
			matrix[r][c] = newVertex;

		    //cout << "r";
		    if (red % 2 == 0){
				//check if there should be an edge to top or left
				if (checkTop(r, c, matrix)) {
					Vertex * topV = matrix[r - 1][c];
					 newVertex->neighs.insert(topV);
					 newVertex->weights[topV] = 1;
				}
				if (checkLeft(r, c, matrix)) {
					Vertex * leftV = matrix[r][c - 1];
					newVertex->neighs.insert(leftV);
					newVertex->weights[leftV] = 1;
				}
				s->neighs.insert(newVertex);//creates edge from S to newVertex
				s->weights[newVertex] = 1;
		    }
		    // cout << "b"; 
		    else {
				//check if there should be an edge to top or left
				if (checkTop(r, c, matrix)) {
					Vertex * topV = matrix[r - 1][c];
					topV->neighs.insert(newVertex);
					topV->weights[newVertex] = 1;
				}

				if (checkLeft(r, c, matrix)) {
					Vertex * leftV = matrix[r][c - 1];
					leftV->neighs.insert(newVertex);
					leftV->weights[newVertex] = 1;
				}
				newVertex->neighs.insert(t);//creates edge from black vertex to T
				newVertex->weights[t] = 1;
		    }		
		}

		if (maze[i] == '\n') { r++; c = 0; }
		else { c++; }
		red++;
	}
	/*displayMatrix(matrix);
	cout << "vSet size: " << vSet.size() << endl;
	cout << "s newighbors: ";
	displayNeighs(s);
	cout << "\nMax Flow: " << max_flow(s, t, vSet) << endl;
	cout << "vset.size()-2/2: " << (vSet.size() - 2 )/ 2 << endl;*/
	if (max_flow(s, t, vSet) == (vSet.size()-2) / 2)
		if (vSet.size()%2==0)
		return true;

	//cout << "false" << endl;
	return false;
	 
}//end of create graph


bool has_tiling(string floor)
{
	vSet.clear();
	findRowCol(floor);
    return createGraph(floor);
}


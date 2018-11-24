i need to create a directed graph
each space is a vertex
instead of coordinates i need to add the vertex to the r or b list
each edge has a weight of 1
vertecies of the same color are diagonal
if there is a diagonal vertex it should be in the set of the same color
each time i start a new row change alternate starting color
if row is even (if colum is even )then red
if row is odd (if column is even )then red

maybe i can start a counter red at 0 if r is even && c is 0
else start red at 1 if r is odd and c is 0
increment i;

vertexSet contains the vertices in an unordered set
if there is a vertex on top 


 create vertesis s and t
 if the vertex added is red create edge from s to it
 if the vertex added is black create edge from it to t

then call max flow with (s,t,vertexset)

if i am at a space create a vertex add it to list of vertices
add the vertex to array
if there is a vertex on top create an edge
if there is a vertex on left create an edge to it
if it is a red add edge from s to it
if it is black add edge from it to t

im missing edges that point down or right
maybe create all of the vertices first then 
add edges

if there is a vertex on top then i create a directed edge to it
but there cant ve a directed edge from it because it is black

if i am at a black and there is a vertex on top 
then i can create a directed edge from top to bottom(because top is red)
if i am at black and there is a vertex on left
i can create vertex from left to right

to create an edge you add the vertex it points to to its list of newighbours

why would i need a map of vertex to weights
well weights are edges so store edges?
all weights should be 1
vSet[vertex*] = int;

AI Usage: AI used in part for every part of this project, with the exception of the Dijkstra algorithm, which was based off of my implementation of it in MA3. I modified it slightly to fit this project. 

The Graph is an adjacency list that contains the network of hospitals and the distances between them. 
The shortest paths between hospitals were precomputed before the rest of the program (ie, the user typing inputs to the terminal). This allows constant-time lookup of the nearest hospital. 
The queue of patients was a max-queue so that the patient with the highest priority would be processed first, and then down the line in descending order of priority from there. 
Each Hospital object held its own max-heap to manage its own patients. 
The implementation of Dijkstra's algorithm takes O(VlogV + ElogV) time to complete for a given map, with V being vertices (in this case, cities and their hospitals) and E being the number of edges, represented by roads in this case. 
The operation to merge 2 queues (max-heaps) will take O(logn) time to complete. The operations to insert and extract the max will also take O(logn) time to finish. 

Edge cases: All hospitals closed - print error and reject the new emergency (this can't happen because the program starts with all hospitals open and refuses to close the last open one).
Emergency in closed-hospital area - transfers patient immediately to the nearest open hospital
Closing last open hospital - already mentioned, refuses to close it
Transfering patient through a closed-hospital town - the patient can be transported through the city but not stop there since the hospital is closed, but the roads should be open for driving

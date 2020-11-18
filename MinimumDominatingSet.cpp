
/*
 * Find the minimum dominating set for a graph
 * Sequential version
 */


#include <iostream>
#include <omp.h>
using namespace std;


// Multiply array[n] by adjacencyMatrix[n][n]
// store the result in the array result[n]
// n is the number of nodes in the graph
void multiply(int *result,const int *array,const int **adjacencyMatrix,const int n) {
  for(int i = 0;i < n;i++) {
    result[i] = 0;
    for(int j = 0;j < n;j++) {
      result[i] = result[i] + (array[j]*adjacencyMatrix[j][i]);
    }
  }
}


bool isDominatingSet(const long x,const int n, const int **adjacencyMatrix) {
  int verticesInSet[n];
  int verticesCovered[n];
  long mask = 1;
  /* Calculate which vertices belong to the set
     encoded by the value x
  */
  for(int i=0;i < n;i++) {
    if ((x & mask) != 0) {
      verticesInSet[i] = 1;
    }
    else {
      verticesInSet[i] = 0;
    }
    mask = mask * 2;
  }
  // Perform the matrix multiplication between
  // the candidate array and the
  // adjacency matrix
  multiply(verticesCovered,verticesInSet,adjacencyMatrix,n);

  // Now check if there are any nodes that are not covered
  // If there are, this is not a dominating set
  for(int i = 0;i < n;i++) {
    if (verticesCovered[i] == 0) {
      return false;
    }
  }
  return true;
}

// Return the number of bits that contain 1 in the value x
int bitsIn1( long x,int n ) {
  long mask = 1;
  int result = 0;
  /* Calculate which vertices belong to the set
     encoded by the value x
  */
  for(int i=0;i < n;i++) {
    if ((x & mask) != 0) {
      result++;
    }
    mask = mask * 2;
  }	
  return result;
}

void printResult(long x,int n) {
  long mask = 1;
  /* Calculate which vertices belong to the set
     encoded by the value x
  */
  cout << "{";
  for(int i=0;i < n;i++) {
    if ((x & mask) != 0) {
      cout << i << " ";
    }
    mask = mask * 2;
  }
  cout << "}" << endl;
}

int main() {
  /* Read the size of the graph - number of vertices
     and then read the adjacency matrix for the graph
  */
  int n;
  cin >> n;
  int **adjacencyMatrix;
  adjacencyMatrix = new int *[n];
  for(int i = 0;i < n;i++) {
    adjacencyMatrix[i] = new int[n];
  }

  for(int i = 0;i < n;i++) {
    for(int j = 0;j < n;j++) {
      cin >> adjacencyMatrix[i][j];
    }
  }
  // Print the adjacency matrix
 
  for(int i = 0;i < n;i++) {
    for(int j = 0;j < n;j++) {
      cout << (adjacencyMatrix[i][j]) << " ";
    }
    cout << endl;
  }
 
  long twoToN = 1;
  for(int i = 0;i < n;i++) {
    twoToN = twoToN * 2;
  }

  int minNumberBits = n;
  int result = twoToN - 1;
  // This is the loop that takes the most time
  // in the execution of the program.
  // This is the best candidate for parallelizing
  // using an openmp directive
  for(long e = 1; e < twoToN;e++) {
    
    if (isDominatingSet(e,n,(const int **) adjacencyMatrix)) {
	    // Notice that this block is a critical section
	    // It should be protected with an open mp directive
	  {
	    if (bitsIn1(e,n) <  minNumberBits) { 
	      minNumberBits = bitsIn1(e,n);
	      result = e;
	    }
	  }
    }
  }
  cout << "Result: " << result << " with " <<  
    minNumberBits << " elements" << endl;
  cout << "The nodes in the minimum dominating set are: ";
  printResult(result,n);
  cout << endl;
}
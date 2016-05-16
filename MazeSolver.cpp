#include <iostream>
#include <stack>
#include <utility>
#include <fstream>
#include <string>

using namespace std;

const int N_MAX = 15; // Max size of the square matrix that can be read from the file.

stack< pair<int, int> > path; // Stack that holds the coordinates of the elements that represent a path.

// Function Prototypes
int maze_size(char * file_name);
bool find_maze_path(char[N_MAX][N_MAX], int, int, int N);
void display_path(char maze[N_MAX][N_MAX]);
void maze_copy(char maze[N_MAX][N_MAX], char maze_copy[N_MAX][N_MAX], int N);
void file_to_maze(char * file_name, char maze[N_MAX][N_MAX], int N);
void generate_all_paths(char * file_name);

int main(int argc, char * argv[]){
	generate_all_paths(argv[1]);
	return 0;
}

/**
* Precondition: Takes in the name of the file that contains 
* a square matrix which represents a maze as a parameter.
* The start is at maze[1][0], and the exits are at the most 
* right side of the maze.
* Postcondition: Returns all the paths from the start to the 
* different exits.
*/
void generate_all_paths(char * file_name){
	char maze[N_MAX][N_MAX];
	char maze_temp[N_MAX][N_MAX];
	int N;
	int path_count = 0;
	bool path_exists = true;

	N = maze_size(file_name);

	file_to_maze(file_name, maze, N);

	// Print a path if it exists.
	while (path_exists){
		maze_copy(maze, maze_temp, N);
		path_exists = find_maze_path(maze_temp, 1, 0, N);
		if (path_exists){
			path_count++;
			cout << "Path " << path_count << ": ";
			display_path(maze);
		}
	}

	cout << path_count << " total paths" << endl;
}

/**
* Precondition: Takes in a square matrix that represents a maze, 
* the row of the current element, the column of the current element, 
* and the size of the maze as parameters.
* Postcondition: Returns true if a path is found from start to any 
* of the exits.
*/
bool find_maze_path(char maze[N_MAX][N_MAX], int row, int col, int N){
	if (row < 0 || col < 0 || row >= N || col >= N){
		return false; // Element is outside the maze
	} else if (maze[row][col] == '#' || maze[row][col] == '!' || maze[row][col] == '*') {
		return false; // Element is a wall, dead end, or an exit for a path already taken.
	} else if (col == N - 1) {
		path.push(make_pair(row, col)); // Add exit to the stack
		return true; // Element is a maze exit.
	} else {
		maze[row][col] = '*'; // Mark the element as visited.
		// Recursively find a path using the four neighbour elements
		if (find_maze_path(maze, row - 1, col, N) || find_maze_path(maze, row + 1, col, N)
		|| find_maze_path(maze, row, col - 1, N) || find_maze_path(maze, row, col + 1, N) ){
			path.push(make_pair(row, col));
			return true;
		} else {
			maze[row][col] = '!'; // Dead end.
			return false;
		}
	}
}

/**
* Precondition: Takes in the name of the file that contains 
* a square matrix, a empty square matrix, and the size of the
* maze as a parameters.
* Postcondition: Matrix from the file is copied to the empty matrix.
*/
void file_to_maze(char * file_name, char maze[N_MAX][N_MAX], int N){
	ifstream maze_file(file_name); // Open file for reading

	// Print error message if file can't be opened
	if(!maze_file){
		cout << "ERROR: Can't open file." << endl;
	}

	// Copy matrix from file to empty matrix
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			maze_file >> maze[i][j]; 
		}
	}

	maze_file.close(); // Close file
}

/**
* Precondition: Takes two square matrices as a parameters.
* Postcondition: Contents of the matrix is copied to the copy matrix.
*/
void maze_copy(char maze[N_MAX][N_MAX], char maze_copy[N_MAX][N_MAX], int N){
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			maze_copy[i][j] = maze[i][j]; 
		}
	}	
}

/**
* Precondition: Takes in a square matrix up to the size N as a parameter.
* Postcondition: Prints the elements in matrix that were pushed on to the stack.
*/
void display_path(char maze[N_MAX][N_MAX]){
	while (!path.empty()){
		if (path.size() == 1){
			cout << maze[path.top().first][path.top().second];
			// Mark the exit for the current path as a wall
			maze[path.top().first][path.top().second] = '#';
		} else {
			cout << maze[path.top().first][path.top().second] << ",";
		}
		path.pop();
	}
	cout << endl;
}

/**
* Precondition: Takes in the name of the file that contains 
* a square matrix as a parameter.
* Postcondition: Returns the size of the square matrix.
*/
int maze_size(char * file_name){
	string s;
	int N = 0;

	ifstream maze_file(file_name); // Open file for reading

	// Print error message if file can't be opened
	if(!maze_file){
		cout << "ERROR: Can't open file." << endl;
	}

	// Count the number of rows in the matrix to find the size
	while(getline(maze_file, s)){
		N++;
	}

	maze_file.close(); // Close file

	return N;
}
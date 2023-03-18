#include <stdio.h>
#include <stdlib.h> /*for system()*/
#include <stack>
#include <unistd.h> /*for usleep()*/

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE *maze_map = fopen(file_name,"r");
	if(!maze_map)
	{
		printf("Não abriu o arquivo");
	}
	// Le o numero de linhas e colunas (fscanf)
	fscanf(maze_map,"%i %i\n", &num_rows, &num_cols);
	//printf("rows %i cols %i", num_rows,num_cols); 
	// e salva em num_rows e num_cols
	maze = (char**)malloc (num_rows * sizeof (char*));
	// Aloca a matriz maze (malloc)
	for (int i = 0; i < num_rows; ++i)
		maze[i] = (char*) malloc(num_cols * sizeof (char*));
	
	char carac;
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			if(!feof(maze_map))
			{
				carac = fgetc(maze_map);
				if(carac == 'e')
				{
					initial_pos.i = i;
					initial_pos.j = j;
				}
				maze[i][j] = carac;
			}
		}
		fgetc(maze_map);
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
		// Marcar a posição atual com o símbolo '.'
		// Limpa a tela

		system("clear");
		// Imprime o labirinto
		print_maze();
		//printf("\n Posicao corrente (%i, %i)\n", pos.i, pos.j);
		maze[pos.i][pos.j] = '.';
		usleep(50000);
		system("clear");
		print_maze();
		printf("\n Posicao corrente (%i, %i)\n", pos.i, pos.j);
		
		/* Dado a posição atual, verifica quais sao as próximas posições válidas
			Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		 	e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		 	cada uma delas no vetor valid_positions
		 		- pos.i, pos.j+1
		 		- pos.i, pos.j-1
		 		- pos.i+1, pos.j
		 		- pos.i-1, pos.j
		 	Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	 	*/
	 	pos_t new_pos;
		/*Primeiro irei verificar colunas*/
	 	if(pos.j+1 > 0 && pos.j+1 < num_cols && (maze[pos.i][pos.j+1] == 'x' || maze[pos.i][pos.j+1] == 's'))
		{
			new_pos.i = pos.i;
			new_pos.j = pos.j+1;
			valid_positions.push(new_pos);
		}
		if(pos.j-1 > 0 && pos.j-1 < num_cols && (maze[pos.i][pos.j-1] == 'x' || maze[pos.i][pos.j-1] == 's'))
		{
			new_pos.i = pos.i;
			new_pos.j = pos.j-1;
			valid_positions.push(new_pos);
		}

		/*Olhando para as linhas*/
		if(pos.i+1 > 0 && pos.i+1 < num_rows && (maze[pos.i+1][pos.j] == 'x' || maze[pos.i+1][pos.j] == 's'))
		{
			new_pos.i = pos.i+1;
			new_pos.j = pos.j;
			valid_positions.push(new_pos);
		}
		if(pos.i-1 > 0 && pos.i-1 < num_rows && (maze[pos.i-1][pos.j] == 'x' || maze[pos.i-1][pos.j] == 's'))
		{
			new_pos.i = pos.i-1;
			new_pos.j = pos.j;
			valid_positions.push(new_pos);
		}
		if(maze[new_pos.i][new_pos.j] == 's')
		{
			system("clear");
			print_maze();
			maze[new_pos.i][new_pos.j] = '.';
			usleep(50000);
			system("clear");
			print_maze();
			
			return true;
		}
		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		if (!valid_positions.empty()) {
			pos_t next_position = valid_positions.top();
			valid_positions.pop();
			walk(next_position);
		}
	return false;
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze("/workspaces/maze_runner/data/maze2.txt");
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	//print_maze();
	// Tratar o retorno (imprimir mensagem)
	//printf("\n%c", maze[initial_pos.i][initial_pos.j]);
	return 0;
}

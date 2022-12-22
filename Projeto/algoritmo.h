typedef struct {
	char* file;
	int runs;
}thread_arg_t;

void run_for_file_trepa_colinas(const char* nome_fich, int runs);
DWORD WINAPI process_file_trepa_colinas(LPVOID lpParameter);
int trepa_colinas(int sol[], int *mat, int vert, int num_iter);
void lunch_threads(char** files, int num_files, int runs);


//TODO remove
void gera_vizinho4(int a[], int b[], int n);

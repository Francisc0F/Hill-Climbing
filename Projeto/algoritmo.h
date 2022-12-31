typedef struct {
	char* file;
	int runs;
	int neighour;
	int accept_equal;
}thread_arg_t;

void run_for_file_trepa_colinas(const char* nome_fich, int runs, int neighour, int accept_equal);
DWORD WINAPI process_file_trepa_colinas(LPVOID lpParameter);
int trepa_colinas(int sol[], int* mat, int vert, int num_iter, int neighour, int accept_equal);
void lunch_threads(char** files, int num_files, int runs, int neighour, int accept_equal);

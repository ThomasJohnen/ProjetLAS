

/**
PRE: sockfdController : un pointeur void qui pointe vers un entier représentant le descripteur de fichier de socket.
POST: Les entrées/sorties standard (STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO) sont redirigées vers le descripteur de fichier de socket fourni en entrée.
*/
void void zombieFils(void* sockfdController);
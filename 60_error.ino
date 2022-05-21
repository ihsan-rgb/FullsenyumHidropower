/*
 * Definisi error
 */

enum E_ERROR {
  E_OK,
  E_ERROR,    
  E_TIMEOUT,  
  E_CONNECT  
};

char* errors[] = {
  "OK",
  "Error !",
  "Timeout",
  "Cannot connect"  
};

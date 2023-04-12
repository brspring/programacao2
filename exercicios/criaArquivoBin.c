#include <stdio.h>

int main() {
  const char* filename = "integers.bin";
  const int numIntegers = 100;
  int integers[numIntegers];

  // Preencher o array com inteiros em ordem crescente
  for (int i = 0; i < numIntegers; i++) {
    integers[i] = i;
  }

  // Escrever os inteiros no arquivo binário
  FILE* file = fopen(filename, "wb");
  if (file != NULL) {
    fwrite(integers, sizeof(int), numIntegers, file);
    fclose(file);
    printf("Arquivo binário com %d inteiros em ordem crescente criado com sucesso!\n", numIntegers);
  } else {
    printf("Erro ao criar o arquivo binário!\n");
  }

  return 0;
}

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <sys/stat.h> /* para usar a funï¿½ï¿½o ver_arquivo_existestat */

/* Constantes  */
#define  TAMSTR  	50+1
#define  ARQUIVO_DE_ALUNOS 		"ALUNOS.DAT"

/* Tipos de dados prï¿½-definidos */

/*----Lay-out do registro de Alunos------
+---------------------------------------+
|		MatricAluno		|	NomeAluno  	|
+----------------------------------------+
|	CÃ³digo do Aluno	| 	 Nome do     |   
| string (alfanumérico; |     aluno     |
|     9 caracteres)	    |      	        |	  
+---------------------------------------+
*/
typedef struct
{
   	char  	MatricAluno[9+1];
   	char 	NomeAluno[TAMSTR];
}
registro_alunos;

/* Protï¿½tipos de funï¿½ï¿½es */
int 				CalculaRegistrosArq		(void);
registro_alunos    captura_aluno      		(void);
void  				falha_abrir_arquivo 	(void);
void  				falha_gravar_arquivo 	(void);
void  				falha_ler_arquivo 		(void);
void  				sucesso_gravacao 		(registro_alunos R);
void  				descartar 				(registro_alunos R);
char  				confirmar 				(registro_alunos R);
void  				grava_aluno			    (registro_alunos R);
void 				cadastrar 				(void);
int 				ver_arquivo_existe 		(void);
void  				relatorio_alunos		(void);
int   main ();

/* Funï¿½ï¿½es */

int CalculaRegistrosArq(void)
{
	int numero_registros;
	FILE *Arq;
	Arq = fopen (ARQUIVO_DE_ALUNOS, "r");
	if ( Arq == NULL) /* O arquivo nï¿½o existe */
	   numero_registros = 0;
	else
	{   /* Calcula o tamanho do arquivo */
		if ( fseek (Arq, 0, SEEK_END) ) /* Aqui fseek tenta se posicionar no final do arquivo...*/
		{
           printf("\nERRO ao calcular o tamanho de arquivo!\n");
           getch();
           numero_registros = -1; /* -1 = ERRO GRAVï¿½SSIMO!!!!!!!!!! */
        }
        else
		{   /*...para que ftell 'conte' quantos bytes o arquivo tem */
		    /* Dividindo-se o total de bytes do arquivo pelo tamanho de cada registro */
		    /* tem-se o nï¿½mero de registros do arquivo. */
		    numero_registros =  ftell(Arq) / sizeof (registro_alunos);  /* 0 ou mais REGISTROS [OK] */
  	    }
  	    /* Fecha o arquivo */
  	    fclose (Arq);
	}
    /* Para teste: avisa quantos bytes o arquivo tem */
    /*
    printf ("\nO arquivo [%s] tem [%i] registros.", ARQUIVO_DE_ALUNOS, numero_registros);
    getch();
    */

    /* 'devolve' a quantidade de registros para o programa 'chamador'*/
	return (numero_registros); /* -1, O ou MAIOR QUE ZERO */
}

registro_alunos    captura_alunos (void)
{
   registro_alunos		rprod;
   printf ("\n------------ CADASTRO DE ALUNOS -----------------");
   sprintf (rprod.MatricAluno , "SP%07i", CalculaRegistrosArq() + 1 );
   printf ("\n Codigo  : %s", rprod.MatricAluno);
   printf ("\n Nome    : "); fflush (stdin); gets(rprod.NomeAluno);
   return (rprod);
}

void  falha_abrir_arquivo (void)
{
	system ("color 4f"); /* Cor do texto branca com fundo vermelho */
	system ("cls");
	printf ("FALHA AO ABRIR [%s]!", ARQUIVO_DE_ALUNOS);
	getch();
	exit(0);
}

void  falha_gravar_arquivo (void)
{
	system ("color 4f"); /* Cor do texto branca com fundo vermelho */
	system ("cls");
	printf ("FALHA AO GRAVAR [%s]!", ARQUIVO_DE_ALUNOS);
	getch();
	exit(0);
}

void  falha_ler_arquivo (void)
{
	system ("color 4f"); /* Cor do texto branca com fundo vermelho */
	system ("cls");
	printf ("FALHA AO LER [%s]!", ARQUIVO_DE_ALUNOS);
	getch();
	exit(0);
}

void  falha_falta_alunos (void)
{
	system ("color 4f"); /* Cor do texto branca com fundo vermelho */
	system ("cls");
	printf ("\nFALHA: NAO HA ALUNOS CADASTRADOS EM [%s]!", ARQUIVO_DE_ALUNOS);
	printf ("\nARQUIVO [%s] INEXISTENTE!", ARQUIVO_DE_ALUNOS);
	getch();
}

void  sucesso_gravacao (registro_alunos R)
{
   system ("color 2f"); /* Cor do texto branca com fundo verde */
   system ("cls");
   printf ("\n\nRegistro [%s][%s]", R.MatricAluno, R.NomeAluno);
   printf ("\n gravado com sucesso!\n\n");
   Sleep(200);
}

void  descartar (registro_alunos R)
{
   printf ("\n\nRegistro descartado: [%s][%s]", R.MatricAluno, R.NomeAluno);
   getch();
}

char  confirmar (registro_alunos R)
{
   char opc;
   printf ("\n\n[%s][%s]", R.MatricAluno, R.NomeAluno);
   printf ("\n\nConfirma gravacao do registro? [S=sim]");
   fflush (stdin); opc = getche();
   return (opc);
}

void  grava_produto	(registro_alunos R)
{
   FILE *A;
   char esc = confirmar(R);
   if ( esc == 'S' || esc == 's')
   {
       A = fopen (ARQUIVO_DE_ALUNOS, "a");
	   if (A==NULL)
	      falha_abrir_arquivo();	   
	   else
	   {
	      fwrite (&R, sizeof(R), 1, A);
	      if ( ferror(A) )
	         falha_gravar_arquivo();
	   }
	   fclose(A);
	   sucesso_gravacao(R);
   }
   else
      descartar (R);
}

void cadastrar (void)
{
    char  esc;
    do
	{
        system ("color 70"); /* Cor do texto preta com fundo cinza */
	    system ("cls");
		grava_produto(captura_alunos());
		printf ("\nCadastra outro produto? [S=sim]");
		fflush (stdin); esc = getche();
	}
	while (esc == 'S' || esc == 's');
}

int ver_arquivo_existe (void)
{
  struct stat buffer;
  int exist = stat(ARQUIVO_DE_ALUNOS, &buffer);
  return exist;	
}

void  relatorio_alunos	(void)
{  
   FILE *A;
   registro_alunos R;

   if (ver_arquivo_existe() == -1)
	   falha_falta_alunos();
   else
   {
		A = fopen (ARQUIVO_DE_ALUNOS, "r");
   		if (A==NULL)
      		falha_abrir_arquivo();	   
   		else
   		{    
        	system ("color 0e"); /* Cor do texto amarela com fundo preto */
	    	system ("cls");
			printf ("\n-------------------------------------");
        	printf ("\n          ALUNOS CADASTRADOS       ");
        	printf ("\n-------------------------------------");
   	    	while ( !feof(A) )
   	    	{ 
		    	fread (&R, sizeof(R), 1, A);
            	if ( ferror(A) )
	           		falha_ler_arquivo();
	        	if ( !feof(A) )
	           		printf ("\n   %s  %-20s ", R.MatricAluno, R.NomeAluno);
	   		}
	   	fclose(A);
   		}
   		printf ("\n-------------------------------------\n");
   		printf ("\nPressione qualquer tecla para retornar ao menu principal.");
   		getch();
   }
}

/* Corpo do programa */
int 	main	()
{	
  char  opc;
  /* O programa permanece em looping enquanto nï¿½o for selecionada a opï¿½ï¿½o SAIR */
  do
  {
    /* Mostra o menu de opï¿½ï¿½es */
    do
	{
        system ("color e0"); /* Cor do texto amarela com fundo preto */
	    system ("cls");
	    printf ("\n------------- CADASTRO DE ALUNOS ------------");
	    printf ("\n            c => Cadastrar alunos                     ");
	    printf ("\n            r => Relatorio de alunos                  ");
	    printf ("\n            s => SAIR                                   ");
	    printf ("\n--------------------------------------------------------");
	    printf ("\n            Selecione a opcao desejada: ");
	    fflush (stdin); opc = getche();
	}
	while (opc != 'S' && opc != 's' &&
	       opc != 'C' && opc != 'c' &&
		   opc != 'R' && opc != 'r' );
	
	/* Dependendo da opï¿½ï¿½o, executa o cadastro ou mostra o relatï¿½rio ou encerra o programa */
	switch (opc)
	{
		case 'C': case 'c': cadastrar();			break;
		case 'R': case 'r': relatorio_alunos(); 	break;
		case 'S': case 's': exit(0); 				break;
	}
  }
  while ( opc != 'S' && opc != 's');
  return (0);
}

// Dimensione immagine in pixel, quadrata
#define DATA_DIM 256

// Soglie di molto scuro e molto chiaro
#define SOGLIA_CHIARO 200
#define SOGLIA_SCURO 50


//Struttura per l'header
typedef struct {
   unsigned int size;
   unsigned short int reserved1;
   unsigned short int reserved2;
   unsigned int offset;
} Header;

//Struttura per l'header info
typedef struct {
   unsigned int size;
   int width,height;
   unsigned short int planes;
   unsigned short int bits;
   unsigned int compression;
   unsigned int imagesize;
   int xresolution,yresolution;
   unsigned int ncolours;
   unsigned int importantcolours;
} Info;

//Struttura per rappresentare un pixel. Nel nostro caso solo 8 bits, un solo campo
typedef struct {
   unsigned char grey;
} Pixel;

//Struttura immagine
typedef struct{
	unsigned char magic[2];                     //Tipo di immagine
	Header header;                              //Header
	Info info;                                  //Info header
	unsigned char color_table[DATA_DIM*4];      //Tabella colori
	Pixel data[DATA_DIM][DATA_DIM];             //Matrice di pixel
}BMP_Image;

typedef struct{
int giorno;
int mese;
int anno;
} Tipo_Data;

typedef struct{
const char anno[4];
const char mese[2];
const char giorno[2];
} Tipo_Data_char;

typedef struct {
char nome[60];
char cognome[60];
Tipo_Data data_nascita;
char luogo_nascita[60];
char residenza [60];
char via[60];
int civico;
int CAP;
char  CF[16];
} Tipo_Anagrafica;


typedef struct{
char giorno[3];
char mese[3];
char anno[5];
char regione[2];
char livello[4];
} Tipo_Tac;


// Struttura paziente

typedef struct {
BMP_Image Tac;
Tipo_Tac dettagli_tac;
Tipo_Anagrafica anagrafica;
} Tipo_Paziente;


//Caricamento immagine
int loadBMP(char * filename, BMP_Image *img);

//Salvataggio immagine
int saveBMP(BMP_Image img, char * filename);

// Inserimento dati anagrafici nella struttura paziente
void inserisciDati(Tipo_Paziente*);

// Calcolare livelli di grigio
void calcola_livelli (BMP_Image*);

// Menu iniziale
void menu ();

// Inserimento tac, TASTO 1
void inserisci_tac();

// Crea file con dati paziente
void crea_file_paziente(Tipo_Paziente*);

// Riduce i livelli di grigio a dieci(1)
void riduci_livelli(BMP_Image);

// menu caratteristica con un immagine singola
void menu_caratteristica_singola();

// Riduce i livelli di grigio a dieci(2)
void riduci_livelli2(BMP_Image);

// Nel menu caratteristiche singole, dopo aver caricato l'immagine, fa decidere all'utente cosa calcolare
void scegli_caratteristica_singola(BMP_Image*);

// Controlla, dal codice fiscale, se il paziente è gia stato registrato
int verifica_nuovo_paziente(Tipo_Paziente*);

// Se è un nuovo paziente crea un cartella apposita e vi salva l'immagine dentro, altrimenti la salva preesistente
void salva_tac(Tipo_Paziente*, int, char*);

// Genera nome con cui salvare la tac
void genera_nome(Tipo_Paziente*, char*);

// Visualizza la storia di un paziente, TASTO 2
void storicizzazione();

// Funzione che definisce conotorno parte informativa e calcola area compresa (pixel, mm2)
void contorno_area(BMP_Image, int [DATA_DIM][DATA_DIM]);

// Funzione che crea matrice di supporto per il contorno
void mat_contorno(BMP_Image image, int [DATA_DIM][DATA_DIM]);

// Funzione che calcola aree di zone molto scure o molto chiare
void aree_singola(BMP_Image*, int[DATA_DIM][DATA_DIM]);

// Menu caratteristica con due immagini
void menu_caratteristica_doppia();

// Menu caratteristica, TASTO 3
void menu_caratteristica();

//  Nel menu caratteristiche dopo, dopo aver caricato le due immagini, fa decidere all'utente cosa calcolare
void scegli_caratteristica_doppia(BMP_Image*, char [100]);

// Variazione aree tra due immagini a scelta
void aree_doppia_scelta(BMP_Image*, int [DATA_DIM][DATA_DIM]);

// Variazione tra due immagini una successiva all'altra (caricamento automatico seconda immagine)
void aree_doppia_successiva(BMP_Image*, int [DATA_DIM][DATA_DIM], char [100]);

// Funzione di prova per facilitare inserimento immagine senza scrivere tutto il percorso
void percorso_load();



// Caricamento immagie BMP

int loadBMP(char *filename, BMP_Image *image){

	FILE *fp;

	fp = fopen(filename, "rb");
	if(fp == NULL){
		printf("Error: impossibile aprire il file in lettura\n");
		return 1;
	}

	fread(image->magic, sizeof(image->magic), 1, fp );

	if(image->magic[0]!='B' || image->magic[1]!='M'){
		printf("Error: tipo di immagine non corretto\n");
		return 2;
	}

	fread(&image->header, sizeof(image->header), 1, fp);
	fread(&image->info, sizeof(image->info), 1, fp);

	if(image->info.bits!=8){
		printf("Error: numero di bits/pixel diverso da 8\n");
		return 3;
	}

	if(image->info.width!=DATA_DIM || image->info.height!=DATA_DIM){
		printf("--- Attenzione, dimensioni non corrette ---");
	}

	fread(&image->color_table, sizeof(image->color_table), 1, fp);
	fread(image->data, sizeof(image->data), 1, fp);

	fclose(fp);
	return 0;
}



//** Funzione per salvare una struttura BMP_Image su file
//Parametri:
	//puntatore alla struttura da cui leggere i dati dell'immagine, nome del file su cui scrivere
//Valore di ritorno:
	//0 se il salvataggio è avvenuto con successo, 1 altrimenti


int saveBMP(BMP_Image image, char * filename){

	FILE *fp2;
	fp2 = fopen(filename, "wb");

	if(fp2==NULL)
	{
		printf("Impossibile aprire il file in scrittura\n");
		return 1;
	}

	fwrite(&image.magic, sizeof(image.magic), 1, fp2);
	fwrite(&image.header, sizeof(image.header), 1, fp2);
	fwrite(&image.info, sizeof(image.info), 1, fp2);
	fwrite(&image.color_table, sizeof(image.color_table), 1, fp2);
	fwrite(image.data, sizeof(image.data), 1, fp2);

	fclose(fp2);
	return 0;

}


    // Inserimento dati anagrafici nella struttura paziente

void inserisciDati(Tipo_Paziente* struttura){

    printf(" Inserisci il tuo nome: ");
    scanf("%s", struttura->anagrafica.nome);
    printf("\n Inserisci il tuo cognome: ");
    scanf("%s", struttura->anagrafica.cognome);
    printf("\n Inserisci la tua data di nascita: \n Giorno: ");
    scanf("%d", &struttura->anagrafica.data_nascita.giorno);
    printf("\n Mese: ");
    scanf("%d", &struttura->anagrafica.data_nascita.mese);
    printf("\n Anno: ");
    scanf("%d", &struttura->anagrafica.data_nascita.anno);
    printf("\n Inserisci il luogo di nascita: ");
    scanf("%s", struttura->anagrafica.luogo_nascita);
    printf("\n Inserisci la citta di residenza: ");
    scanf("%s", struttura->anagrafica.residenza);
    printf("\n Via: ");
    scanf("%s", struttura->anagrafica.via);
    printf("\n Numero civico: ");
    scanf("%d", &struttura->anagrafica.civico);
    printf("\n CAP: ");
    scanf("%d", &struttura->anagrafica.CAP);

}


// Calcolare livelli di grigio

void calcola_livelli (BMP_Image* image){
    int i,j,k=0;
    int array [DATA_DIM];

    for(i=0;i<DATA_DIM;i++)
        for(j=0;j<DATA_DIM;j++){
            array[image->data[i][j].grey]++;
        }
    for (i=0;i<DATA_DIM;i++)
        if(!(array[i]==0))
            k++;

    printf("\n\nla tua immagine ha %d livelli di grigio\n\n", k);;
}


// MENU INIZIALE

void menu (){
    int comando;

    printf ("\n\n\n## MENU INIZIALE ##\n\nPremi:\n1. Per inserire una nuova TAC\n2. Per visualizzare la storicizzazione di una paziente\n3. Per analizzare le caratteristiche di una o piu TAC\n");
    scanf("%d", &comando);

    if (comando==1)
        inserisci_tac();
    else if (comando==2)
        storicizzazione();
    else if (comando==3)
       menu_caratteristica();

    else printf("il comando è errato");
}


// Inserimento tac, tasto 1 del menu

void inserisci_tac (){
    int inserimento, ritorno, nuovo, c;
    char fileload[50];
    Tipo_Paziente TAC;
    char nomegenerato[15];

    printf("\nInserisci il tuo codice fiscale:\n");
    scanf("%s", TAC.anagrafica.CF);

    nuovo=verifica_nuovo_paziente(&TAC);

    if (nuovo==1){
    printf("\nNon sei ancora nostro paziente! Inserisci i tuoi dati:\n");
    inserisciDati(&TAC);
    crea_file_paziente(&TAC);
    }

    else if(nuovo==0)
    printf("\nSei gia un paziente!");


    printf("\nInserisci il nome del file da cui prendere la Tac:\n");
    scanf("%s", fileload );
    ritorno= loadBMP(fileload,&TAC.Tac);

    //FUNZIONE GENERA NOME
    genera_nome(&TAC, nomegenerato);

    //FUNZIONE SALVA TAC
    salva_tac(&TAC, nuovo, nomegenerato);

    menu();
}


// crea file testuale con i dati del paziente

void crea_file_paziente (Tipo_Paziente* struttura){
    DIR* dir;
    char b[5]="ciaod";
    char nomefile[20];
    int c;

    strcpy(nomefile,struttura->anagrafica.CF);
    strcat(nomefile,".doc");

    dir=opendir("Datipaziente");
    c=chdir("Datipaziente");


    FILE* fp= fopen(nomefile, "w+");

    if(fp!= NULL){
        fprintf(fp,"Codice fiscale: ");

        fprintf(fp,"%s",struttura->anagrafica.CF);
        fprintf(fp, "\nNome: ");
        fprintf(fp, "%s",struttura->anagrafica.nome);
        fprintf(fp, "\nCognome: ");
        fprintf(fp, "%s", struttura->anagrafica.cognome);
        fprintf(fp, "\nData di nascita: ");
        fprintf(fp, "%d/%d/%d",struttura->anagrafica.data_nascita.giorno, struttura->anagrafica.data_nascita.mese, struttura->anagrafica.data_nascita.anno);
        fprintf(fp, "\nLuogo di nascita: ");
        fprintf(fp, "%s", struttura->anagrafica.luogo_nascita);
        fprintf(fp, "\nVia ");
        fprintf(fp, "%s",struttura->anagrafica.via );
        fprintf(fp, "\nCivico: ");
        fprintf(fp, "%d", struttura->anagrafica.civico );
        fprintf(fp, "\nCitta: ");
        fprintf(fp, "%s",struttura->anagrafica.residenza );
        fprintf(fp, "\nCAP: ");
        fprintf(fp, "%d", struttura->anagrafica.CAP );

        int close=fclose(fp);
    }

    int close=fclose(fp);
    int dclose=closedir(dir);
    c=chdir("..");
}


// Ridurre i livelli di grigio a dieci (FUNZIONE 1)

void riduci_livelli(BMP_Image image){
    int i, j, saving;
    unsigned char max, min;
    char filesave[50];
    char num=25;

    for(i=0;i<DATA_DIM;i++){
        for(j=0;j<DATA_DIM;j++){
            max=image.data[i][j].grey;
            min=image.data[i][j].grey;
            do{max++;}
            while (!(max%num==0));

            do{min--;}
            while(!(min%num==0));
            min=image.data[i][j].grey;
            image.data[i][j].grey=(max+min)/2;
        }
    }

    printf("\n immagine modificata con successo! inserisci il nome con cui salvarla!\n");
    scanf("%s", filesave);
    saving=saveBMP(image, filesave);
    if(!saving)
        printf("\nsalvataggio immagine modificata avvenuto con successo");
    calcola_livelli(&image);
}


// Menu caratteristica con una singola immagine

void menu_caratteristica_singola (){
    int load;
    char fileload[100];
    BMP_Image image;

    printf("Inserisci l'immagine di cui vuoi calcolare le caratteristiche:\n");
    scanf("%s", fileload);

    load=loadBMP(fileload,&image);

    scegli_caratteristica_singola(&image);
}


// Ridurre i livelli di grigio a dieci(FUNZIONE 2)

void riduci_livelli2(BMP_Image image){
    int i,j, saving;
    char filename[45];

    for(i=0;i<DATA_DIM;i++)
        for(j=0;j<DATA_DIM;j++){
            if((-1<image.data[i][j].grey) &&(image.data[i][j].grey<25))
                image.data[i][j].grey=12;
            else if((24<image.data[i][j].grey)&&(image.data[i][j].grey<50))
                image.data[i][j].grey=37;
            else if((49<image.data[i][j].grey)&&(image.data[i][j].grey<75))
                image.data[i][j].grey=62;
            else if((74<image.data[i][j].grey)&&(image.data[i][j].grey<100))
                image.data[i][j].grey=87;
            else if((99<image.data[i][j].grey)&&(image.data[i][j].grey<125))
                image.data[i][j].grey=112;
            else if((124<image.data[i][j].grey)&&(image.data[i][j].grey<150))
                image.data[i][j].grey=137;
            else if((149<image.data[i][j].grey)&&(image.data[i][j].grey<175))
                image.data[i][j].grey=162;
            else if((174<image.data[i][j].grey)&&(image.data[i][j].grey<200))
                image.data[i][j].grey=187;
            else if((199<image.data[i][j].grey)&&(image.data[i][j].grey<225))
                image.data[i][j].grey=212;
            else if((224<image.data[i][j].grey)&&(image.data[i][j].grey<256))
                image.data[i][j].grey=237;
        }
    printf("\n immagine modificata con successo! inserisci il nome con cui salvarla!\n");
    scanf("%s", filename);
    saving=saveBMP(image, filename);
    if(!saving)
        printf("\nsalvataggio immagine modificata avvenuto con successo");
    calcola_livelli(&image);
}


// Nel menu caratteristiche singole, dopo aver caricato l'immagine, fa decidere all'utente cosa calcolare

void scegli_caratteristica_singola(BMP_Image* image){
    int comando;
    int mat[DATA_DIM][DATA_DIM];
    int i,j;

    for(i=0;i<DATA_DIM;i++)
        for(j=0;j<DATA_DIM;j++)
            mat[i][j]=1;

    mat_contorno(*image, mat);

    printf("\n\nPremi:");
    printf("\n1. Per calcolare i livelli di grigio.");
    printf("\n2. Per calcolare la distribuzione dei livelli di grigio");
    printf("\n3. Per ridurre i livelli di grigio a 10");
    printf("\n4. Per definire il contorno della parte informativa e calcolarne l'area");
    printf("\n5. Per definire il contorno della parte informativa e calcolare l'area di particolari zone");
    printf("\n0. Per tornare al menu principale\n");
    scanf("%d", &comando);

    if(comando==0)
        menu();
    else if(comando==1){
        calcola_livelli(image);
        scegli_caratteristica_singola(image);}
    //else if(comando==2)
        //calcola_distribuzione livelli
    else if(comando==3){
        riduci_livelli(*image);
        scegli_caratteristica_singola(image);}
    else if(comando==4){
        contorno_area(*image, mat);
        scegli_caratteristica_singola(image);}
    else if (comando==5){
        aree_singola(image, mat);
        scegli_caratteristica_singola(image);}
}


// Controllo nuovo paziente

int verifica_nuovo_paziente(Tipo_Paziente* struttura){
    DIR* dir;
    struct dirent *sd;
    int nuovo=1, i, close, c, cont;
    char nomefile[16];

    strcpy(nomefile,struttura->anagrafica.CF);
    strcat(nomefile,".txt");

    dir=opendir("Datipaziente");

    if (dir==NULL){
        printf("errore: impossibile aprire cartella pazienti");
    }

    while( (sd=readdir(dir))  !=NULL){
        cont=0;
        for(i=0;i<16;i++){
            if(sd->d_name[i]==nomefile[i])
                cont++;
            if(cont==16)
                nuovo=0;
       }
    }

    close=closedir(dir);

    return nuovo;
}


// Se è un nuovo paziente crea un cartella apposita e vi salva l'immagine dentro, altrimenti la salva preesistente

void salva_tac(Tipo_Paziente* struttura, int nuovo, char* nomegenerato){
    DIR* dir;
    DIR* dir2;
    int saving, dclose, newdir,c;
    char nomefile[19];
    char buff[100];
    size_t size=100;

    strcpy(nomefile,nomegenerato);
    strcat(nomefile,".bmp");

    if(nuovo==0) {
        c=chdir("Datipaziente");
        c=chdir(struttura->anagrafica.CF);
        saving=saveBMP(struttura->Tac, nomefile);
        c=chdir("..");
        c=chdir("..");
    }
    else if(nuovo==1){
        c=chdir("Datipaziente");
        newdir=mkdir(struttura->anagrafica.CF);
        dir2=opendir(struttura->anagrafica.CF);
        c=chdir(struttura->anagrafica.CF);
        saving=saveBMP(struttura->Tac, nomefile);
        c=chdir("..");
        c=chdir("..");
    }
}

// Genera nome con cui salvare la tac

void genera_nome(Tipo_Paziente* struttura, char* nomegenerato){

    printf("\nInserisci la data della tac:");
    printf("\n Giorno: ");
    scanf("%s", struttura->dettagli_tac.giorno);
    printf("\n Mese: ");
    scanf("%s", struttura->dettagli_tac.mese);
    printf("\n Anno: ");
    scanf("%s", struttura->dettagli_tac.anno);
    printf("\nInserisci la regione coroporea (es: addome=1, testa=2, gambe=3): ");
    scanf("%s", struttura->dettagli_tac.regione);
    printf("\nInserisci il livello di profondita (es: 004 o 070 o 134): ");
    scanf("%s", struttura->dettagli_tac.livello);

    strcpy(nomegenerato, struttura->dettagli_tac.anno);
    strcat(nomegenerato, struttura->dettagli_tac.mese);
    strcat(nomegenerato, struttura->dettagli_tac.giorno);
    strcat(nomegenerato, "-");
    strcat(nomegenerato, struttura->dettagli_tac.regione);
    strcat(nomegenerato, "-");
    strcat(nomegenerato, struttura->dettagli_tac.livello);
}

// Funzione che visualizza la storicizzazione (tasto 2)

void storicizzazione (){
    char cf[17];
    DIR* dir;
    DIR* dir2;
    int c, i, close;
    struct dirent *sd;
    char buff[100];
    size_t size=100;
    off_t off;
    FILE* fp;

    printf("\nInserisci il codice fiscale del paziente di cui vuoi visualizzare la storia:\n");
    scanf("%s", cf);

    // STAMPARE A VIDEO ANCHE NOME E COGNOME?


    c=chdir("Datipaziente");
    dir2=opendir(cf);

    while((sd=readdir(dir2))!=NULL){
        if(strlen(sd->d_name)==18){
            printf("\n\n#TAC:\nData:\n Anno: ");
            for(i=0;i<4;i++)
                printf("%c",sd->d_name[i]);
            printf("\n Mese: ");
            for(i=4;i<6;i++)
                printf("%c", sd->d_name[i]);
            printf("\n Giorno: ");
            for(i=6;i<8;i++)
                printf("%c", sd->d_name[i]);
            printf("\nRegione corporea: %c", sd->d_name[9]);
            printf("\nLivello: ");
            for(i=11;i<14;i++)
                printf("%c", sd->d_name[i]);
        }
    }
    c=chdir("..");

    menu();
}


// Funzione che difinisce il contorno della parte informativa dell'immagine e calcola l'area (pixel e mm2)

void contorno_area(BMP_Image image, int mat[DATA_DIM][DATA_DIM]){
    unsigned char nero=0;
    unsigned char bianco=255;
    int i,j, s;
    int m,n;

    for(j=0;j<DATA_DIM;j++){
        i=0;
        while((image.data[i][j].grey==nero)&&(i<DATA_DIM/2)){
            image.data[i][j].grey=bianco;
            i++;
        }
    }

    for(j=0;j<DATA_DIM;j++){
        i=255;
        while((image.data[i][j].grey==nero)&&(i>=DATA_DIM/2)){
            image.data[i][j].grey=bianco;
            i--;
        }
    }

    s=saveBMP(image, "immaginecontorno.bmp");

    int areap=0;
    float amm;

    for(i=0;i<DATA_DIM;i++)
        for(j=0;j<DATA_DIM; j++){
            if (mat[i][j]==1)
                areap ++;
        }
    printf("\nL'Area inclusa nel contorno della parte informativa è pari a %d pixel", areap);
    amm=0.0625*areap;
    printf("\nL'Area in mm2 è pari a %f", amm);
}


// Funzione che crea matrice di supporto per contorno

void mat_contorno(BMP_Image image, int mat[DATA_DIM][DATA_DIM]){
    int i, j;
    unsigned char nero=0;
    unsigned char bianco=255;

 for(j=0;j<DATA_DIM;j++){
        i=0;
        while((image.data[i][j].grey==nero)&&(i<DATA_DIM/2)){
            mat[i][j]=0;
            i++;
        }
    }

    for(j=0;j<DATA_DIM;j++){
        i=255;
        while((image.data[i][j].grey==nero)&&(i>=DATA_DIM/2)){
            mat[i][j]=0;
            i--;
        }
    }

}


// Funzione che calcola aree con sogni molto scuro e molto chiaro

void aree_singola(BMP_Image* image, int mat[DATA_DIM][DATA_DIM]){
    unsigned char scuro= SOGLIA_SCURO;
    unsigned char chiaro= SOGLIA_CHIARO;
    int i, j;
    int ascuro=0;
    int achiaro=0;
    float mchiaro, mscuro;

    for(i=0;i<DATA_DIM;i++)
        for(j=0;j<DATA_DIM;j++){
            if(mat[i][j]==1){
                if(image->data[i][j].grey<SOGLIA_SCURO)
                    ascuro++;
                else if (image->data[i][j].grey>SOGLIA_CHIARO)
                    achiaro++;
            }
        }

    mchiaro=0.0625*achiaro;
    mscuro=0.0625*ascuro;

    printf("\nLa soglia scelta per MOLTO CHIARO e: %d", SOGLIA_CHIARO);
    printf("\nL'area in pixel delle aree molto chiare e: %d", achiaro);
    printf("\nL'area in mm2 delle aree molto chiare e: %f", mchiaro);
    printf("\n\nLa soglia scelta per MOLTO SCURO e: %d", SOGLIA_SCURO);
    printf("\nL'area in pixel delle aree molto scure e: %d", ascuro);
    printf("\nL'area in mm2 delle aree molto scure e: %f", mscuro);
}


// Menu caratteristiche con due immagini

void menu_caratteristica_doppia (){
    int load1, load2;
    char fileload1 [100];
    BMP_Image image1;

    printf("\nInserisci la prima immagine: ");
    scanf("%s", fileload1);

    load1=loadBMP(fileload1, &image1);


    scegli_caratteristica_doppia(&image1, fileload1);
}


// Menu caratteristica, TASTO 3

void menu_caratteristica(){
    int comando;

    printf("\n\n## MENU CARATTERISTICHE ##\n\n");
    printf("Premi:\n1. Per calcolare le caratteristiche di una singola immagine\n2. Per confrontare le caratteristiche di due immagini\n0. Per tornare al menu principale\n");
    scanf("%d", &comando);

    if(comando==0)
        menu();
    else if (comando==1)
        menu_caratteristica_singola();
    else if(comando==2)
        menu_caratteristica_doppia();
    else {  printf("\nComando errato!");
            menu_caratteristica();
    }
}

// Nel menu caratteristiche doppie, dopo aver caricato le due immagini, fa decidere all'utente cosa calcolare

void scegli_caratteristica_doppia(BMP_Image* image1, char fileload1[100]){
    int comando;
    int mat1[DATA_DIM][DATA_DIM];
    int i,j;

    for(i=0;i<DATA_DIM;i++)
        for(j=0;j<DATA_DIM;j++)
            mat1[i][j]=1;

    mat_contorno(*image1, mat1);


    printf("\nPremi:");
    printf("\n1. Per calcolare la variazione delle due aree fra due immagini successive");
    printf("\n2. Per calcolare la variazione delle due aree fra due immagini a scelta");
    printf("\n0. Per tornare al menu principale\n");
    scanf("%d", &comando);

    if(comando==0)
        menu();
    else if(comando==1)
        aree_doppia_successiva(image1, mat1, fileload1);

    else if(comando==2)
            aree_doppia_scelta(image1, mat1);

    else {  printf("\nComando errato!");
            scegli_caratteristica_doppia(image1, fileload1);
    }

}

// Variazione aree tra due immagini a scelta

void aree_doppia_scelta(BMP_Image* image1, int mat1[DATA_DIM][DATA_DIM]){
    BMP_Image image2;
    int mat2[DATA_DIM][DATA_DIM];
    int i,j;
    char fileload2[100];
    int load2;

    printf("\nInserisci la seconda immagine: ");
    scanf("%s", &fileload2);


    load2=loadBMP(fileload2, &image2);

    for(i=0;i<DATA_DIM;i++)
        for(j=0;j<DATA_DIM;j++)
            mat2[i][j]=1;

    mat_contorno(image2, mat2);

    printf("\n#Prima immagine:\n");
    aree_singola(image1, mat1);
    printf("\n\n#Seconda immagine:\n");
    aree_singola(&image2,mat2);
}


// Variazione tra due immagini una successiva all'altra (caricamento automatico seconda immagine)

void aree_doppia_successiva(BMP_Image* image1, int mat1[DATA_DIM][DATA_DIM], char fileload1[100]){
    BMP_Image image2;
    int len, load2, i, j;
    char fileload2[100];
    int mat2[DATA_DIM][DATA_DIM];

    printf("%s", fileload1);
    len=strlen(fileload1);
    printf("%d", len);
    strcpy(fileload2, fileload1);
    printf("\n%c\n", fileload2[len-5]);
    if(fileload2[len-5]=='9'){
        fileload2[len-5]='0';
        fileload2[len-6]=fileload2[len-6]+1;
    }
    else fileload2[len-5]=fileload2[len-5]+1;
    printf("\n nome della successiva: %s\n", fileload2);

    load2=loadBMP(fileload2, &image2);

    for(i=0;i<DATA_DIM;i++)
        for(j=0;j<DATA_DIM;j++)
            mat2[i][j]=1;

    mat_contorno(image2, mat2);

    printf("\n#Prima immagine:\n");
    aree_singola(image1, mat1);
    printf("\n\n#Seconda immagine:\n");
    aree_singola(&image2,mat2);
}

// Funzione di prova per facilitare inserimento tac senza scrivere tutto il percorso dell'immagine

void percorso_load (){
    char percorso[100];
    char CF[16];

    strcpy(percorso, "c:");
    printf("%s", percorso);

    printf("\nInserisci il CF del paziente di cui vuoi caricare la tac: ");
    scanf("%s", CF);
}

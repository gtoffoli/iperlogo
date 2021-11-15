//	980831	interfaccia cerca_sc, metti_sc, match_sc_case:
//			char * -> const char *

void marca_ws();
void LIBERA();
void libera_n(int);
void n_libera();
void libera_1();
void libera_2();
int fogliadi(unsigned long, unsigned long);
unsigned long cercap(unsigned long);
unsigned long richiudi(unsigned long);
unsigned long paroprec(unsigned long);
unsigned long parosucc(unsigned long);
void in_indice();
unsigned long nuovap(unsigned long);
unsigned long mettip(unsigned long);
// unsigned long cerca_sc(unsigned char *);
// unsigned long metti_sc(unsigned char *);
// unsigned long cerca_sc(char *);
unsigned long cerca_sc(const char *);
// unsigned long metti_sc(char *);
unsigned long metti_sc(const char *);
unsigned long cercav(unsigned long, int);
unsigned long cercao(unsigned long, int);
unsigned long cercat(unsigned long *, int);
int get_sf(unsigned long);
unsigned long makeuf(unsigned long, unsigned long);
void get_uf(unsigned long *, unsigned long *, unsigned long);
unsigned long mettio(unsigned long, unsigned long, int);
void canceo(unsigned long);
unsigned long cancet(unsigned long, int);
unsigned long nome(unsigned long);
// void getnam(unsigned char *, unsigned long);
void getnam(char *, unsigned long);
int tipoalfa(unsigned long);
void PREC();
void SUCC();

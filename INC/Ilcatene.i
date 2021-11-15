// 980831: modificato logstr per compatibilità con CString

int lstlen(unsigned long);
unsigned long lastel(unsigned long);
unsigned long lstcat(unsigned long, unsigned long);
unsigned long coppia(unsigned long, unsigned long);
unsigned long set_intersection(unsigned long, unsigned long);
unsigned long set_difference(unsigned long, unsigned long);
unsigned long set_union(unsigned long, unsigned long);
int member(unsigned long, unsigned long);
unsigned long lstdel(unsigned long *, unsigned long);
unsigned long lstdup(unsigned long);
unsigned long listainv(unsigned long);
void irdstr(unsigned long);
unsigned int rdstr();
void irdstr2(unsigned long);
unsigned int rdstr2();
unsigned long iwtstr();
// unsigned long wtstr(unsigned char);
unsigned long wtstr(char);
int wrdlen(unsigned long);
int match(unsigned long, unsigned long);
// int match_sc(unsigned char *, unsigned long);
int match_sc(char *, unsigned long);
// unsigned long logstr(unsigned char *);
// int pacstr(unsigned char *, unsigned long);
// int bufstr(unsigned char *, int, int);
// unsigned long logstr(char *);	// 980831
unsigned long logstr(const char *);
int pacstr(char *, unsigned long);
int bufstr(char *, int, int);

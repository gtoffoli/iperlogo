void printf_null (char*); 
void printf_long (char*, long); 
void printf_int (char*, int); 
int stcd_i (char*, int*); 
int list2array(unsigned long, unsigned long *, int);
int nodeinarray(unsigned long, unsigned long *, int);
void print_err(int);
// int getenv_i(unsigned char *);
int getenv_i(char *);
int confro(unsigned long, unsigned long);
int boolp(unsigned long);
int bool_ (unsigned long);
unsigned long logval(int);
unsigned long leggi_ele();
int conv_num_node(unsigned long, unsigned long *);
int conv_num_double(unsigned long, double *);
// unsigned int os_off(unsigned char *);
// unsigned int os_seg(unsigned char *);
// void copy_bits(unsigned char *, unsigned char *, unsigned char);
unsigned int os_off(char *);
unsigned int os_seg(char *);
void copy_bits(char *, char *, char);

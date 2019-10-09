
int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main(int argc,char **argv) {
    infof("I've been testing this program like %d times\n",1293);
    warnf("It would be so sad if this program called %s does not work!\n",argv[0]);
    errorf("HELP! I'M SO STRESSED TESTING THIS PROGRAM! Float: %f\n",9.32);
    panicf("IS IT OVER?\n");
    
    return 0;
}

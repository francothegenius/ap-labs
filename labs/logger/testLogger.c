int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);


int main() {
    infof("%s\n", "This is info");
    warnf("%s\n", "WARNING");
    errorf("%s\n", "Segmentation Fault :(");
    panicf("%s\n", "PANIC");
    return 0;
}
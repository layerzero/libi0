#ifndef FASTEXP_H
#define FASTEXP_H

#define LOG2E (1.4426950408889634073599)
#define C1 (6.93145751953125E-1)
#define C2 (1.42860682030941723212E-6)

// modifed version from http://www.chokkan.org/software/dist/fastexp.c.html
double exp_remez13_05_05(double value);

#endif // FASTEXP_H

#ifndef PDB2FASTA_H
#define PDB2FASTA_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void *pdb2fasta(FILE *file, const char *name, char *out);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // PDB2FASTA_H

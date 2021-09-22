#include "pdb2fasta/pdb2fasta.h"
#include "codon3to1.h"
#include <stdlib.h>
#include <string.h>

#define PDB_LINE_WIDTH 80
#define ELEMENT_STD_WIDTH 6
#define ATOM_WIDTH 4
#define MINIMUM_VIABLE 22
#define ATOM_FIRST_ELEMENT 12
#define RESN_WIDTH 3
#define RESN_FIRST_ELEMENT 17
#define CHAIN_ID_POSITION 21
#define ID_POSITION 16

void *pdb2fasta(FILE *file, const char *name, char *out) {
    int count = 1;
    char line[PDB_LINE_WIDTH];
    char id = ' ';

    char currentChainID = '\0';

    if (&out[0] != 0) {
        out[0] = 0;
    }

    while (fgets(line, PDB_LINE_WIDTH, file) != NULL) {
        if (strncmp("ENDMDL", line, ELEMENT_STD_WIDTH) == 0) {
            break;
        }

        if (strlen(line) < MINIMUM_VIABLE ||
            (strncmp("ATOM  ", line, ELEMENT_STD_WIDTH) &&
             strncmp("HETATM", line, ELEMENT_STD_WIDTH))) {
            continue;
        }

        char atom[ATOM_WIDTH + 1];
        atom[ATOM_WIDTH] = '\0';
        for (unsigned i = 0; i < ATOM_WIDTH; i++) {
            atom[i] = line[ATOM_FIRST_ELEMENT + i];
        }

        char resn[RESN_WIDTH + 1];
        resn[RESN_WIDTH] = '\0';
        for (unsigned i = 0; i < RESN_WIDTH; i++) {
            resn[i] = line[RESN_FIRST_ELEMENT + i];
        }

        id = line[ID_POSITION];
        if (strcmp(atom, " CA ") || (id != ' ' && id != 'A') ||
            (strncmp("HETATM", line, ELEMENT_STD_WIDTH) == 0 && strcmp(resn, "MSE"))) {
            continue;
        }

        char temp[strlen(name) + strlen("\n\n>%s:%c\n")]; // FIXME: Heap?
        char nextChainID = line[CHAIN_ID_POSITION];
        if (currentChainID != nextChainID) {
            if (currentChainID) {
                sprintf(temp, "\n\n>%s:%c\n", name, nextChainID);
            } else {
                sprintf(temp, ">%s:%c\n", name, nextChainID);
            }

            currentChainID = nextChainID;

            count = 1;

            out = realloc(out, sizeof(*out) * (strlen(out) + strlen(temp) + 1));
            strcat(out, temp);
        }

        if (count == PDB_LINE_WIDTH) {
            out = realloc(out, sizeof(*out) * (strlen(out) + strlen("\n") + 1));
            strcat(out, "\n");
            count = 1;
        }
        count++;

        out = realloc(out, sizeof(*out) * (strlen(out) + strlen(codon3to1(resn)) + 1));
        strcat(out, codon3to1(resn));
    }

    return out;
}

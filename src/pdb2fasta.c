#include "pdb2fasta/pdb2fasta.h"
#include "codon3to1.h"
#include <stdlib.h>
#include <string.h>

const char *pdb2fasta(FILE *file, const char *name, char *out) {
    const unsigned name_length = strlen(name);
    out = calloc(name_length, sizeof(*out));

    int count = 0;
    char line[80], altid = ' ';
    while (fgets(line, 80, file) != NULL) {
        if (strncmp("ENDMDL", line, 6) == 0) {
            break;
        }

        if (strlen(line) < 22 ||
            (strncmp("ATOM  ", line, 6) && strncmp("HETATM", line, 6))) {
            continue;
        }

        char atom[5];
        atom[4] = '\0';
        for (unsigned i = 0; i < 4; i++) {
            atom[i] = line[12 + i];
        }

        char resn[4];
        resn[3] = '\0';
        for (unsigned i = 0; i < 3; i++) {
            resn[i] = line[17 + i];
        }

        altid = line[16];
        if (strcmp(atom, " CA ") || (altid != ' ' && altid != 'A') ||
            (strncmp("HETATM", line, 6) == 0 && strcmp(resn, "MSE"))) {
            continue;
        }

        char temp[name_length + 10], currentChainID, nextChainID = line[21];
        if (currentChainID != nextChainID) {
            if (currentChainID) {
                sprintf(temp, "\n>%s:%c\n", name, nextChainID);
            } else {
                sprintf(temp, ">%s:%c\n", name, nextChainID);
            }

            currentChainID = nextChainID;

            count = 0;

            out = realloc(out, sizeof(*out) * (strlen(out) + strlen(temp) + 1));
            strcat(out, temp);
        }

        if (count == 79) {
            out = realloc(out, sizeof(*out) * (strlen(out) + 1));
            strcat(out, "\n");
            count = 0;
        }
        count++;

        out = realloc(out, sizeof(*out) * (strlen(out) + 2));
        strcat(out, codon3to1(resn));
    }
    strcat(out, "\n");

    return out;
}

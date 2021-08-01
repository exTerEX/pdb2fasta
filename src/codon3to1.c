/*
 * Copyright 2021 Andreas Sagen

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>

char *codon3to1(char *resn) {
    if (strcmp(resn, "ALA") == 0) {
        return ("A");
    } else if (strcmp(resn, "VAL") == 0) {
        return ("V");
    } else if (strcmp(resn, "PHE") == 0) {
        return ("F");
    } else if (strcmp(resn, "PRO") == 0) {
        return ("P");
    } else if (strcmp(resn, "MET") == 0) {
        return ("M");
    } else if (strcmp(resn, "ILE") == 0) {
        return ("I");
    } else if (strcmp(resn, "LEU") == 0) {
        return ("L");
    } else if (strcmp(resn, "ASP") == 0) {
        return ("D");
    } else if (strcmp(resn, "GLU") == 0) {
        return ("E");
    } else if (strcmp(resn, "LYS") == 0) {
        return ("K");
    } else if (strcmp(resn, "ARG") == 0) {
        return ("R");
    } else if (strcmp(resn, "SER") == 0) {
        return ("S");
    } else if (strcmp(resn, "THR") == 0) {
        return ("T");
    } else if (strcmp(resn, "TYR") == 0) {
        return ("Y");
    } else if (strcmp(resn, "HIS") == 0) {
        return ("H");
    } else if (strcmp(resn, "CYS") == 0) {
        return ("C");
    } else if (strcmp(resn, "ASN") == 0) {
        return ("N");
    } else if (strcmp(resn, "GLN") == 0) {
        return ("Q");
    } else if (strcmp(resn, "TRP") == 0) {
        return ("W");
    } else if (strcmp(resn, "GLY") == 0) {
        return ("G");
    } else if (strcmp(resn, "MSE") == 0) {
        return ("M");
    } else {
        return ("X");
    }
}

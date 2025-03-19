#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define maxStringLenght 101
#define DEL 127
#define numOfNonPrintableChars (EOF + ' ') // (31) Pocet prvnich netisknutelnych znaku v ASCII tabulc (pouzivane pro posun ASCII kodu)
#define numOfPrintableChars ((DEL) - (numOfNonPrintableChars)) // (95) Pocet vsech tisknutelnych znaku v zakladni ASCII tabulce (pouzivany pro urceni delky pole s nalezy)

int findMatches(char *search, char *matches, char *found);

char* strToupper(char *word);

int main(int argc, char *argv[]) {
    char *search = "";
    // Zkontroluje kolik a jak dlouhe arguenty uzivatel zadal
    if (argc == 2) {
        if (strlen(argv[1]) < maxStringLenght) {
            search = argv[1];
        } else {
            printf("Error: [search] is too long!\n");
            return 2;
        }
    } else if (argc > 2) {
        printf("Usage: ./keyfilter [search] <adresy.txt\n");
        return 1;
    }

    char matches[numOfPrintableChars];
    for (int i = 0; i < numOfPrintableChars; i++) {
        matches[i] = '\0';
    }
    char found[maxStringLenght] = ""; // Pomocna promenna pro ulozeni nazvu mesta, pokud by bylo nalezeno pouze jedno

    switch (findMatches(search, matches,found)) {
        case 0:
            printf("Not found\n");
            break;
        case 1:
            printf("Found: %s\n", found);
            break;
        default:
            printf("Enable: ");
            for (int i = 1; i < numOfPrintableChars; i++) {
                if (matches[i] != '\0') {
                    printf("%c", matches[i]);
                }
            }
            printf("\n");
    }
    return 0;
}

//funkce vrací počet nalezených shod
int findMatches(char *search, char *matches, char *found) {
    int numOfMatches = 0; // Pomocna promenna pro ulozeni poctu shod
    char fileWord[maxStringLenght] = "";
    int fileChar;
    for (int i = 0; (fileChar = getchar()) >= EOF; i++) {
        // Uklada jednotlive znaky do retezce a pote zpracovava celou adresu
        if ((fileChar != '\n' && fileChar != EOF) || i==0) {
            fileWord[i] = fileChar;
            fileWord[i + 1] = '\0';
        }
        if (!(fileChar != '\n' && fileChar != EOF)) {
            // Pokud dojde na konec radku, porovna vstup se slovem
            if (strncasecmp(fileWord, search, strlen(search)) == 0) {
                // Pokud se nejedna o uplnou shodu, ulozi nasledujici znak do pole shod
                if (strlen(search) != strlen(fileWord)) {
                    char ch = toupper(fileWord[strlen(search)]);
                    // Uloží se do pole na index shodný s ascii hodnotou znaku posunutou o numOfNonPrintableChars,
                    // tak jsou znaky rovnou seřazeny a zbaveny duplicit
                    matches[ch - (numOfNonPrintableChars)] = ch;
                }
                // Když se jedna o prvni shodu, ulozy celou adresu do pomocne promene pro pripad, ze by se jednalo o jedinou shodu
                if (numOfMatches < 1) {
                    strcpy(found, strToupper(fileWord));
                }
                numOfMatches++;
            }
            // Pokud se jedná o poslední adresu, ukončí cyklus
            if (fileChar == EOF) {
                break;
            }
            i = -1; // Reset i
        }
    }
    return numOfMatches;
}

char* strToupper(char *word) {
    for (int i = 0; i < (int)strlen(word); i++) {
        word[i] = toupper(word[i]);
    }
    return word;
}
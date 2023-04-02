#include <iostream>
#include <string>
#include <vector>
#include <fstream>
// ci sono problemi con giuliano di roma
using std::cin;
using std::cout;
using std::endl;
using std::fill_n;
using std::fstream;
using std::getline;
using std::ifstream;
using std::ios;
using std::string;
using std::vector;

int const numero_di_fonti = 3; // numero di fonti da cui attingere
int comparse[numero_di_fonti]; // numero di volte che i termini di ricerca vengono trovati per ogni fonte
bool fonti[numero_di_fonti];   // abilitazione delle varie fonti

int user_friendly();                                      // funzione che esegue il comportamento ideato all'utente
bool definizione_fonti();                                 // disabilita le fonti
vector<string> string_analyzer(string comando);           // ricava i termini da ricercare e li inserisce in un vettore
bool find_in_string(string termine, string frase);        // stabilisce se un termine si trova in una string maggiore
bool find_in_string(vector<string> parole, string frase); // stabilisce se una serie di termini sono contenuti in una string
void file_analyzer(vector<string> parole, bool write);    // cerca i termini nei file
string to_low(string x);                                  // riduce tutti i caratteri a minuscoli
int file_maker();                                         // funzione che esegue il comportamento ideato alla ricerca di informazioni
void search(string ricerca);                              // funzione che separa le ricerche nel caso dell'analisi di molti dati

int main()
{
    fill_n(fonti, numero_di_fonti, 1); // abilita tutte le fonti
    char choice;
    cout<<"seleziona la modalita' di utilizzo:"<<endl;
    cout<<"1) ricerca "<<endl;
    cout<<"2) creazione di un file"<<endl;
    choice=getchar();
    while ((getchar()) != '\n');
    if(choice == '1') user_friendly();
    else if(choice == '2') file_maker();
    else cout<<"scelta non valida"<<endl;
    cout << "premi invio per uscire" << endl;
    while ((getchar()) != '\n'); // pulisci il buffer
    cin.get(); // aspetta l'input
}

int user_friendly()
{
    string termini;                    // comando inserito dall'utente
    char write = 0;
    vector<string> keywords; // lista di termini da ricercare

    cout << "inserisci i termini da ricercare separati da uno spazio e premi invio" << endl;
    getline(cin, termini); // ottieni il comando di ricerca dall'utente

    while (definizione_fonti())
    {
    } // scegli le fonti da abilitare

    keywords = string_analyzer(termini); // estrai le parole da cercare

    file_analyzer(keywords, write); // cerca nei file le parole

    /*elenca le ricorrenze delle parole nei file*/
    if (fonti[0])
        cout << "ho trovato " << comparse[0] << " aziende destinate" << endl;
    if (fonti[1])
        cout << "ho trovato " << comparse[1] << " immobili destinati" << endl;
    if (fonti[2])
        cout << "ho trovato " << comparse[2] << " procedure in gestione" << endl;

    cout << "in totale " << comparse[1] + comparse[2] + comparse[0] << endl;

    /*chiedi se scrivere le coincidenze*/
    cout << endl
         << "vuoi elencarli? [y/n]" << endl;
    cin >> write;
    write = tolower(write);
    write -= 110;
    cout << endl;
    if (write)
    {
        file_analyzer(keywords, write);
    }
    return 0;
}

bool definizione_fonti()
{
    short escludi = 0;

    cout << endl
         << "verranno fornite informazioni dalle seguenti fonti:" << endl
         << endl;
    for (int i = 0; i < numero_di_fonti; i++)
    {
        if (fonti[i])
        {
            switch (i)
            {
            case 0:
                cout << "1) aziende destinate" << endl;
                break;
            case 1:
                cout << "2) immobili destinati" << endl;
                break;
            case 2:
                cout << "3) procedure in gestione" << endl;
                break;

            default:

                break;
            }
        }
    }
    cout << endl
         << "per escludere una fonte scrivi il numero vicino al nome e premi invio" << endl;

    escludi = getchar() - 48;

    if (escludi > 0)
    {
        while ((getchar()) != '\n')
            ; // clear buffer
        fonti[escludi - 1] = 0;
        return 1;
    }
    return 0;
}

vector<string> string_analyzer(string comando)
{
    string k1;
    vector<string> keywords;

    if (comando.back() != ' ')
        comando += ' ';

    while (!comando.empty())
    {
        int x = comando.find_first_not_of(' ');
        comando = comando.substr(x);
        short position = comando.find_first_of(' ');

        k1 = comando.substr(0, position);
        keywords.push_back(k1);
        comando = comando.substr(position + 1);
    }
    return keywords;
}

bool find_in_string(string termine, string frase)
{
    // riduci tutti a minuscolo e rimuovi gli spazi inutili
    termine = to_low(termine);
    frase = to_low(frase);
    int x = frase.find(termine);
    if (x == string::npos)
        return 0;

    // controlla il carattere prima

    if (x > 0)
    {
        if (isalpha(frase[x - 1]))
            return 0;
    }

    // controlla carattere dopo

    if (x + termine.size() < frase.size())
    {
        if (isalpha(frase[x + termine.size()]))
            return 0;
    }

    return 1;
}

bool find_in_string(vector<string> parole, string frase)
{

    for (int i = 0; i < parole.size(); i++)
    {
        if (!find_in_string(parole[i], frase))
        {
            return 0;
        }
    }

    return 1;
}

void file_analyzer(vector<string> parole, bool write)
{
    string riga;
    ifstream file;
    fill_n(comparse,numero_di_fonti,0);
    for (int i = 0; i < numero_di_fonti; i++)
    {
        if (fonti[i])
        {
            switch (i)
            {
            case 0:
                file.open("aziende_destinate.csv");
                break;
            case 1:
                file.open("immobili_destinati.csv");
                break;
            case 2:
                file.open("procedure_in_gestione.csv");
                break;
            default:
                break;
            }

            if (file.is_open())
            {
                int x = 1; // contatore della riga attuale
                while (getline(file, riga))
                {
                    if (find_in_string(parole, riga))
                    {
                        if (write)
                        {
                            cout << "[" << i + 1 << "/" << x << "]" << riga << endl
                                 << endl;
                        }
                        comparse[i]++;
                    }
                    x++;
                }
            }
            file.close();
        }
    }
}

string to_low(string x)
{

    for (int i = 0; i < x.size(); i++)
    {
        x[i] = tolower(x[i]);
    }

    return x;
}

int file_maker()
{
    vector <string> ricerche; 
    string input;
    int tot = 0;
    cout<<"inserisci un termine e premi invio; "<<endl<<"ripeti l'azione per ogni termine da ricercare e poi premi 2 volte invio"<<endl;
    do
    {
        getline(cin,input);
        ricerche.push_back(input);
        
    } while (input!="");
    //stabilisci fonti
    while (definizione_fonti()){} // scegli le fonti da abilitare

    fstream new_file("risultati.csv", ios::out);
    if (new_file.is_open())
    {
        new_file<<"termine di ricerca,";
        if(fonti[0])new_file<<"aziende destinate,";
        if(fonti[1])new_file<<"immobili destinati,";
        if(fonti[2])new_file<<"procedure in gestione,";
        new_file<<"totale"<<endl;


        for (int l = 0; l < ricerche.size()-1; l++) //per ogni termine ricerca nei file
        {
            tot=0;
            search(ricerche[l]);
            new_file<<ricerche[l];
            for (int h = 0; h < numero_di_fonti; h++) // per ogni fonte scrivi i valori
            {
                if(fonti[h]){
                new_file<<",";
                new_file<<comparse[h];
                }

                tot+=comparse[h];
            }
            new_file<<","<<tot;
            new_file<<endl;
            
        }
        new_file.close();
    }
    cout<<"puoi trovare il file completo nella cartella di questo eseguibile"<<endl;
    return 0;
}

void search(string ricerca){
    vector <string> scomposta;
    scomposta = string_analyzer(ricerca);
    file_analyzer(scomposta,0); //assegna quanto compare
}
